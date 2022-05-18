// Database.h: interface for the CDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__D7CEB0BE_DBCB_4BCD_A2AE_75A650F0A2C5__INCLUDED_)
#define AFX_DATABASE_H__D7CEB0BE_DBCB_4BCD_A2AE_75A650F0A2C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum MyConnection_RetCode
{
	ConnRt_Exec_Success,
	ConnRt_Exec_Success_Record,
	ConnRt_Exec_Err_ConnClosed,
	ConnRt_Exec_Err_Exec,
	ConnRt_Exec_Err_Unknown,
	ConnRt_Exec_Mysql_GoneAway,

	ConnRt_Open_Success,
	ConnRt_Open_Err,
};

static CString MyConnectionErrorStr[] = 
{
	"ConnRt_Exec_Success",
	"ConnRt_Exec_Success_Record",
	"ConnRt_Exec_Err_ConnClosed",
	"ConnRt_Exec_Err_Exec",
	"ConnRt_Exec_Err_Unknown",
	"ConnRt_Exec_Mysql_GoneAway",
	"ConnRt_Open_Success",
	"ConnRt_Open_Err",
};
#include "CommandWrapper.h"
#include "LOG.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )

//********************************************
//	数据库封装体，所有的最底层的操作都在该类中
//	每个CMyConnection对象维持1个数据库连接资源
//********************************************
class CMyConnection  
{
public:
	CMyConnection();
	virtual ~CMyConnection();

	//执行命令
	_RecordsetPtr Execute(CCommandWrapper* pCommandWrapper, int* retCode=NULL, int* affectRows=NULL);
	// 三个事务命令
	BOOL BeginTrans(void);
	BOOL CommitTrans(void);
	BOOL RollbackTrans(void);
		
	//新建并打开一个Connection对象
	int OpenConnection(CString connstr);
	int CloseConnection();
private:
	_ConnectionPtr m_connection;	//连接
	_CommandPtr    m_cmd;	
    _RecordsetPtr  m_rs;
};

inline CMyConnection::CMyConnection()
{	
	m_connection.CreateInstance(__uuidof( Connection ));
	m_cmd.CreateInstance( __uuidof( Command ) );
	m_rs.CreateInstance(__uuidof(Recordset));
}

inline CMyConnection::~CMyConnection()
{
	try
	{
		m_cmd.Release();
		m_rs.Release();
		
		m_connection->Close();
		m_connection.Release();		
	}
	catch (...)
	{	
	}
}

/************************************************************************/
/* 返回
-1 关闭失败
1  关闭成功  
/************************************************************************/
inline int CMyConnection::CloseConnection()
{
	try
	{
		if(m_connection->State == adStateOpen)
			m_connection->Close();     //关闭失败
	}
	catch (...)
	{	
		return -1;
	}
	return 1;
}

/************************************************************************/
/*  
返回值
ConnRt_Exec_Success				执行成功
ConnRt_Exec_Success_Record		返回结果集成功
ConnRt_Exec_Err_ConnClosed		连接断开
ConnRt_Exec_Err_Exec			执行出错
ConnRt_Exec_Err_Unknown			未知错误
说明：
如果执行的存储过程里面有多个结果集返回的话，取最后一个结果集
/************************************************************************/

inline _RecordsetPtr CMyConnection::Execute( CCommandWrapper* pCommandWrapper,  int* retCode,int* affectRows )
{	
	_bstr_t   bstrEmpty; 
	_bstr_t bstrProc =(LPCTSTR) pCommandWrapper->m_sp;

/*
	CString debug_sql=pCommandWrapper->m_sp;
	if(debug_sql.GetLength()>300)
		debug_sql=debug_sql.Left(300);
	TRACE("[%d][%s]\r\n",time(NULL),debug_sql);
*/
	try
    {  		                                
		m_cmd->ActiveConnection = m_connection;
		m_cmd->CommandText      = bstrProc;
		if(pCommandWrapper->m_sqlType == (ESqlType)ENUM_Stored)
		{
			m_cmd->CommandType      = adCmdStoredProc;
		}
		if(pCommandWrapper->m_sqlType == (ESqlType)ENUM_Text)
		{
			m_cmd->CommandType      = adCmdText;
		}
	
		//tupengfei [2011-07-05] : 防止第一次执行时可能抛出异常
		_ParameterPtr adoParam;
		int pCount =0;
		try
		{
			
			pCount= m_cmd->Parameters->Count;
			for(int i=0;i< pCount;i++)
			{
				m_cmd->Parameters->Delete(0l);
			}
		}
		catch (...)
		{
		}
		
		CInParamList::iterator it;
		CInParam* pParam;
			
		for(it= pCommandWrapper->m_pParams.begin(); it!=pCommandWrapper->m_pParams.end(); it++)
		{
			pParam = *it;
			if(pParam->m_type == (EParamType)ENUM_String)
			{
				adoParam=m_cmd->CreateParameter((_bstr_t)pParam->m_name,adVarChar,adParamInput,pParam->m_strVal.GetLength()+1,(_bstr_t)pParam->m_strVal);
				m_cmd->Parameters->Append(adoParam);
			}	
			if(pParam->m_type == (EParamType)ENUM_Num)
			{
				adoParam=m_cmd->CreateParameter((_bstr_t)pParam->m_name,adDouble,adParamInput,sizeof(float),pParam->m_numVal);
				m_cmd->Parameters->Append(adoParam);
			}		
			if(pParam->m_type == (EParamType)ENUM_Binary)
			{		
				char* pValue= (char*)(pParam->m_pValue);
				int tSize= pParam->m_size;
				
				SAFEARRAY* psa;
				SAFEARRAYBOUND rgsabound[1];
				VARIANT varChunk;
				
				rgsabound[0].cElements=tSize;
				rgsabound[0].lLbound=0;
				
				psa= SafeArrayCreate(VT_UI1,1,rgsabound);
				for(long i=0 ;i<tSize; i++)
					SafeArrayPutElement(psa,&i,pValue+i);
	
				varChunk.vt = VT_ARRAY|VT_UI1;
				varChunk.parray = psa;
				
				adoParam=m_cmd->CreateParameter((_bstr_t)pParam->m_name,adVarBinary,adParamInput,tSize);
				adoParam->AppendChunk(varChunk);
				m_cmd->Parameters->Append(adoParam);
				
				//::VariantClear(&varChunk); // 终于找到你了！
				::SafeArrayDestroy(psa); //::SafeArrayDestroyData( psa);
			}
		}
		if(m_rs->State == adStateOpen)
		{
			m_rs->Close();
		}

		m_rs->CursorLocation=adUseClient;
		m_rs->CursorType=adOpenStatic;
		_bstr_t     bstrEmpty(L""); 		
		_variant_t  vtEmpty(DISP_E_PARAMNOTFOUND,VT_ERROR);   

//int begin=GetTickCount();
		m_rs->Open((_variant_t((IDispatch *)m_cmd)),vtEmpty,adOpenStatic, adLockReadOnly, -1);   
//int end=GetTickCount();
//TRACE("%d---in\n",end-begin);
		if(m_rs->State == adStateClosed)
		{
			*retCode =ConnRt_Exec_Success;
			return NULL;
		}

		m_rs->putref_ActiveConnection(NULL);
		*retCode =ConnRt_Exec_Success_Record;
		return m_rs;		
	}
    catch( _com_error &e )
    {
		_bstr_t   bstrSource(e.Source());   
        _bstr_t   bstrDescription(e.Description());  
		CString   strDesp = (LPCSTR)bstrDescription;
		CString   strDesp1 = bstrProc + strDesp;

		if(strDesp.Left(10) == "[DBNETLIB]" || strDesp.Find( "连接失败")!=-1 || strDesp.Find("未连接") != -1 ||
			strDesp.Find("not connected") != -1)
		{	
			CDBLogger::Ins()->Log(strDesp1);
			*retCode=ConnRt_Exec_Err_ConnClosed;			
		}
		else if (strDesp.Find("MySQL server has gone away")!=-1)
		{
			*retCode =ConnRt_Exec_Mysql_GoneAway;
		}
		else
		{
			CDBLogger::Ins()->Log(strDesp1);
			if(retCode!=NULL) *retCode =ConnRt_Exec_Err_Exec;
		}
    }
    catch(...)
    {
		if(retCode!=NULL) *retCode =ConnRt_Exec_Err_Unknown;
		CDBLogger::Ins()->Log("%s : level%d", "ConnRt_Exec_Err_Unknown",5);
    }
	return NULL;
}

/************************************************************************/
/* 返回
ConnRt_Open_Err			打开失败
ConnRt_Open_Success		打开成功 
/************************************************************************/
inline int CMyConnection::OpenConnection(CString connstr)
{
	_bstr_t bstrEmpty(L""); 
	try
    {
		if(m_connection->State == adStateOpen)
		{
			m_connection->Close();
		}
		m_connection->Open((_bstr_t)connstr, bstrEmpty, bstrEmpty,adConnectUnspecified);
	}
    catch( _com_error &e )
    {
		_bstr_t   bstrSource(e.Source());   
		_bstr_t   bstrDescription(e.Description()); 
		CString   strDesp = (LPCSTR)bstrDescription;
		
		CDBLogger::Ins()->Log("%s : level %d", strDesp,5);		
		return ConnRt_Open_Err;
    }
	return ConnRt_Open_Success;
}

inline BOOL CMyConnection::BeginTrans()
{
	try
	{
		m_connection->BeginTrans();
		return TRUE;
	}
	catch (...)
	{
		TRACE("BeginTrans Exception!\n");
	}
	return FALSE;
}

inline BOOL CMyConnection::CommitTrans()
{
	try
	{
		m_connection->CommitTrans();
		return TRUE;
	}
	catch (...)
	{
		TRACE("CommitTrans Exception!\n");
	}
	return FALSE;
}

inline BOOL CMyConnection::RollbackTrans()
{
	try
	{
		m_connection->RollbackTrans();
		return TRUE;
	}
	catch (...)
	{
		TRACE("RollbackTrans Exception!\n");
	}
	return FALSE;
}

#endif // !defined(AFX_DATABASE_H__D7CEB0BE_DBCB_4BCD_A2AE_75A650F0A2C5__INCLUDED_)
