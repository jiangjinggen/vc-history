// SqlHelper.h: interface for the CSqlHelper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SQLOBJECT_H
#define SQLOBJECT_H

//Ϳ����2007/5/15���
//Ϊ������ݿ�ӿ�
#include <memory>
#include <afxmt.h>
#include <icrsint.h> 
#include "MyDatabase.h"
#include "param.h"
#include <io.h>
#include <vector>
using namespace std;
//********************************
//�ຯ���Ŀ���������
//Autoģʽ�²����ڿ��������⣬ÿ�κ������ж��Ƕ�ֻ�ܵ���safeexec

//Userģʽ�´��ڿ��������⣬����һ��CDatabaseָ�룬
//			ÿ���߳��ڲ�newһ��CSqlHelper������ܱ�֤���෽���Ŀ�����
//			��������safeexec
//			���Ҫ��trans�����÷��Լ���begin��end֮���������

//Userģʽ��Autoģʽ�Ľ��ܼ� Userģʽ��Ҫ�񣬲μ��ĵ���
//********************************
class  CSqlObject  
{
	friend class CSqlHelper;
public:	
	typedef void (*ConnStateCallBack)(bool state);
	CSqlObject():m_pfnConnStateCallBack(NULL),m_bConnState(true){};
	CSqlObject(EConnStrategy connStartegy);
	virtual ~CSqlObject();

	//�������ֻ���ڵ����ӣ�ע����������ģʽ�µ�MS_ResetConnectStr
	int ResetConnectStr(CString connStr); 
	virtual _RecordsetPtr Exec(CString sp,int sqlType, CInParamList* pParamList, int* retCode=NULL,int* affectRows = NULL);
	virtual _RecordsetPtr SafeExec(CString sp,int sqlType, CInParamList* pParamList, int* retCode=NULL,int* affectRows = NULL);
	virtual int MS_ResetConnectStr(CString connectstr){return ConnRt_Open_Err;};
	CMyConnection* m_Conn;
	virtual void DetectDB(){};
	virtual int OpenDbConnection();	// liuxiaofeng 2013-12-14

protected:
	virtual int OpenConnection();
	bool IsExecConnErr(int nExecCode);
		
protected:
	CString m_connectStr;			//�����ַ����ṹ
	EConnStrategy m_connStartegy;
	CCriticalSection m_cs;
	ConnStateCallBack  m_pfnConnStateCallBack;
	bool			   m_bConnState;
};


inline CSqlObject::CSqlObject(EConnStrategy connStartegy)
{
	m_connStartegy = connStartegy;
	CoInitialize(NULL);
	m_Conn=new CMyConnection();

	// liuxiaofeng- 2013-12-13
	m_pfnConnStateCallBack = NULL;	
	m_bConnState         = true;
}

inline CSqlObject::~CSqlObject()
{
	if(m_Conn!=NULL)
	{
		delete m_Conn;	
	}
	CoUninitialize();
}


/************************************************************************/
/* ������������
/***********************************************************************/
inline int  CSqlObject::ResetConnectStr(CString connStr)
{
	int ret=ConnRt_Open_Success;
	if(m_connStartegy==ENUM_Auto)
	{
		m_connectStr = connStr;
	}
	if(m_connStartegy==ENUM_User)
	{
		if(m_connectStr!=connStr)
		{
			m_connectStr=connStr;
			m_Conn->CloseConnection();
			ret = m_Conn->OpenConnection(m_connectStr);
		}
	}
	return ret;
}

/************************************************************************/
/* �������ݿ�
/***********************************************************************/
inline int CSqlObject::OpenDbConnection()
{
	if(m_connectStr=="")
	{
		CDBLogger::Ins()->Log("�����ַ���Ϊ�գ��޷��������ݿ⣬���������ļ�");
		return ConnRt_Open_Err;
	}

	m_Conn->CloseConnection();
	return m_Conn->OpenConnection(m_connectStr);
}


/************************************************************************/
/* ������
/************************************************************************/
inline int CSqlObject::OpenConnection()
{
	if(m_connectStr=="")
	{
		CDBLogger::Ins()->Log("�����ַ���Ϊ�գ����������ļ�");
		return ConnRt_Open_Err;
	}
	return m_Conn->OpenConnection(m_connectStr);
}


/************************************************************************/
/* ������

����ֵ
ConnRt_Open_Err		AUTO����ʧ��
˵����
���ִ�еĴ洢���������ж����������صĻ���ȡ���һ�������
����з��ؼ�¼����һ����ס����RS->Close()�ر���

USER ����һֱʹ��ͬһ��MyConnection����
AUTO ������ִ�����exec����Ҫsafeexec
/************************************************************************/
inline _RecordsetPtr CSqlObject::Exec(CString sp, int sqlType , CInParamList* pParamList, int* retCode, int* affectRows)
{
	_RecordsetPtr tmp=NULL;
	int inner_retCode=0;
	if (this->m_connStartegy == ENUM_User)
	{
		CCommandWrapper commandWrapper;
		commandWrapper.Build(sp, sqlType, pParamList);
		//////////////////////////////////////////////////////////////////////////
		//[5/5/2009 tupengfei]
		//userģʽ��exec���ܻ�����mysql��goneaway��Ϊ�������ӶϿ�����Ҫ����һ��
		//[LiuXiaofeng 2011-12-7] �˴��Ż�����: ÿ�η��������жϺ󣬶�������������ȷ�������ж��ڼ䲻ִ�д洢����
		if(m_bConnState)		// ��һ�ν���ú���ʱǰ�γ����ӵ�״̬�϶��������ģ������޷���������SqlObject����
		{
			tmp= m_Conn->Execute(&commandWrapper, &inner_retCode,affectRows);
			if(IsExecConnErr(inner_retCode))
			{
				m_bConnState = false;
				if(m_pfnConnStateCallBack)
					m_pfnConnStateCallBack(false);
			}
		}
		if(!m_bConnState)
		{
			if(OpenConnection() == ConnRt_Open_Success)
			{
				tmp= m_Conn->Execute(&commandWrapper, &inner_retCode,affectRows);
				if(!IsExecConnErr(inner_retCode))
				{
					m_bConnState = true;
					if(m_pfnConnStateCallBack)
						m_pfnConnStateCallBack(true);
				}
			}

			if(!m_bConnState)
			{
				inner_retCode = ConnRt_Open_Err;
			}
		}
	}
	if(this->m_connStartegy == ENUM_Auto)
	{
		if(OpenConnection()==ConnRt_Open_Err)
		{
			if(retCode!=NULL) *retCode = ConnRt_Open_Err;
			return NULL;
		}

		CCommandWrapper commandWrapper;
		commandWrapper.Build(sp, sqlType, pParamList);
		tmp=m_Conn->Execute(&commandWrapper,&inner_retCode, affectRows);
		m_Conn->CloseConnection();
	}
	if(retCode!=NULL) *retCode = inner_retCode;
	return tmp;
}

/************************************************************************/
/* ʹ���ڵ����user��autoģʽ��
autoģʽֻ�������safeexec������exec

transֻ������user��
�������user����Ҫ��sqlhelper�����ֶ�����������exec
/************************************************************************/
inline _RecordsetPtr CSqlObject::SafeExec(CString sp, int sqlType , CInParamList* pParamList, int* retCode, int* affectRows)
{
	m_cs.Lock();
	_RecordsetPtr tmp = Exec(sp,sqlType,pParamList,retCode,affectRows);
	m_cs.Unlock();
	return tmp;
}

inline bool CSqlObject::IsExecConnErr(int nExecCode)
{
	if(nExecCode == ConnRt_Exec_Err_ConnClosed
		||nExecCode == ConnRt_Exec_Mysql_GoneAway
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif 
