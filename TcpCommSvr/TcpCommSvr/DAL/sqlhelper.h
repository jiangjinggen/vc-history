#ifndef SQLHELPER_H
#define SQLHELPER_H

#include "SqlObject.h"
#include "SqlObject_Config_Ms.h"

class  CSqlHelper
{
public:
	struct DB_CFG
	{
		DB_CFG()
			: m_bShuangJiReBei(false),m_pfnDBChange(NULL),m_pfnConnStateCallBack(NULL)
		{
		}

		bool m_bShuangJiReBei;

		CString m_strMasterIP;
		CString m_strMasterConnectStr;

		CString m_strSlaveIP;
		CString m_strSlaveConnectStr;

		CString m_strCurConnectStr;		// ��ǰ��Ч�������ִ�

		void (*m_pfnDBChange)(LPCSTR pszCurDBIP);

		void (*m_pfnConnStateCallBack)(bool state);
	};

	static CSqlObject* User(DB_CFG* pDBCfg);      // ���ݿ����ӵȴ�ʱ��Ĭ��5��
	static CSqlObject* Auto(DB_CFG* pDBCfg);      // ���ݿ����ӵȴ�ʱ��Ĭ��5��
	static CSqlObject* NewUser(DB_CFG* pDBCfg);   // ���������
	static int GetRecordBlob(_RecordsetPtr record , CString columnName, char* retBuf, int maxsize,int* retSize);
	static int GetRecordVarBinary(_RecordsetPtr record , CString columnName, char* retBuf,int maxsize, int* retSize);
	static int GetRecordBinarySize(_RecordsetPtr record , CString columnName);
	static time_t VariantToTime(_variant_t v);

	static CCriticalSection* m_cs_usersinglton; //user�����ʱ�򣬳�����safeexec�⣬�����ⲿ�Լ�������
private:
	static CSqlObject* GetConfigMode(EConnStrategy connStartegy, DB_CFG* pDBCfg);
};

__declspec(selectany) CCriticalSection* CSqlHelper::m_cs_usersinglton=NULL;

inline time_t CSqlHelper::VariantToTime(_variant_t v)
{
	SYSTEMTIME   systime;    
	VariantTimeToSystemTime(v,   &systime);   
    
	CTime   tm(systime);   
	return tm.GetTime();
}

inline CSqlObject* CSqlHelper::GetConfigMode(EConnStrategy connStartegy, DB_CFG* pDBCfg)
{
	if(m_cs_usersinglton==NULL)
		m_cs_usersinglton= new CCriticalSection;

	if (pDBCfg == NULL)
		return new CSqlObject(connStartegy) ;
	
	CSqlObject* s_pSqlHelper=NULL;
	if (!pDBCfg->m_bShuangJiReBei)
	{
		s_pSqlHelper= new CSqlObject(connStartegy);
		s_pSqlHelper->m_connectStr= pDBCfg->m_strMasterConnectStr;
	}
	else
	{
		s_pSqlHelper= new CSqlObject_Config_Ms(connStartegy);
		((CSqlObject_Config_Ms*)s_pSqlHelper)->m_connectStr_1	= pDBCfg->m_strMasterConnectStr;
		((CSqlObject_Config_Ms*)s_pSqlHelper)->m_connectStr_2	= pDBCfg->m_strSlaveConnectStr;
		((CSqlObject_Config_Ms*)s_pSqlHelper)->m_strMasterIP	= pDBCfg->m_strMasterIP;
		((CSqlObject_Config_Ms*)s_pSqlHelper)->m_strSlaveIP		= pDBCfg->m_strSlaveIP;

		//((CSqlObject_Config_Ms*)s_pSqlHelper)->m_connectStr		= ((CSqlObject_Config_Ms*)s_pSqlHelper)->m_connectStr_1;
		((CSqlObject_Config_Ms*)s_pSqlHelper)->m_connectStr		= pDBCfg->m_strCurConnectStr;

		((CSqlObject_Config_Ms*)s_pSqlHelper)->m_pfn			= pDBCfg->m_pfnDBChange;
	}

	if(s_pSqlHelper != NULL)
	{
		s_pSqlHelper->m_pfnConnStateCallBack	= pDBCfg->m_pfnConnStateCallBack;
	}

	return s_pSqlHelper;
}

/************************************************************************/
/*	��������ʽ��Userģʽ���̵߳��壨ÿ���߳�1��ʵ�壩��
�Ƽ��ͻ�����CSqlHelperʵ���� ʹ��NewUser����һ��ʵ��

ʹ�����ƣ�
1 �ú���ֻ���ڵ������������DLL�л�����쳣�����
/************************************************************************/
inline CSqlObject* CSqlHelper::User(DB_CFG* pDBCfg) 
{
	__declspec(thread) static CSqlObject *s_pSqlHelper = NULL;
	if (s_pSqlHelper == NULL)
	{
		if(pDBCfg)
			s_pSqlHelper = GetConfigMode(ENUM_User, pDBCfg);
		else
			s_pSqlHelper= new CSqlObject(ENUM_User); 

		if(s_pSqlHelper->m_connectStr=="")//֮��һ��Ҫͨ��ResetConnection��������
			return s_pSqlHelper;

		if(s_pSqlHelper->OpenConnection()==ConnRt_Open_Err) //���ݿ�����
		{	
			return NULL;
		}
	}
	return s_pSqlHelper;
}

/************************************************************************/
/* ��ʵ��ÿ���������һ��
/************************************************************************/
inline CSqlObject* CSqlHelper::Auto(DB_CFG* pDBCfg) 
{
	static CSqlObject *s_pSqlHelper = NULL;
	if (s_pSqlHelper == NULL)
	{
		if(pDBCfg)
			s_pSqlHelper = GetConfigMode(ENUM_Auto, pDBCfg);
		else
			s_pSqlHelper= new CSqlObject(ENUM_Auto); 
	}
	return s_pSqlHelper;
}

/************************************************************************/
/*��ʵ��ÿ���߳�һ��������ʱ����ά��һ�����ݿ�����
�Ƽ��ͻ�����CSqlHelperʵ���� ʹ��NewUser����һ��ʵ��
�ú������ڶ��������
// ���ô˺����õ��ķ���ֵ����thread-safe��!!!
/************************************************************************/
inline CSqlObject* CSqlHelper::NewUser(DB_CFG* pDBCfg) 
{
	CSqlObject* s_pSqlHelper=NULL;
	if(pDBCfg)	
		s_pSqlHelper= GetConfigMode(ENUM_User, pDBCfg);
	else
		s_pSqlHelper=new CSqlObject(ENUM_User);

	if(s_pSqlHelper->m_connectStr=="")//֮��һ��Ҫͨ��ResetConnection��������
		return s_pSqlHelper;

	if( s_pSqlHelper->OpenConnection()==ConnRt_Open_Err) //���ݿ�����
	{
		CDBLogger::Ins()->Log("���ݿ����ӳ���");
		delete s_pSqlHelper;
		return NULL;
	}
	return s_pSqlHelper;
}
#define ChunkSize 5000 
/************************************************************************/
/* ������������ȡRecord�����BLOB����������
record: ADO RECORD����
columnName: �������������ݵ�����
retBuf: ȡ�������ݵĵ�ַ
retSize: ȡ�����������Ĵ�С 

����ֵ��
-1 ȡֵʧ��
1  �ɹ�                                                                    */
/************************************************************************/
inline int CSqlHelper::GetRecordBlob(_RecordsetPtr record , CString columnName, char* retBuf,int maxsize, int* retSize)
{
	try
	{	
		CString tmp = (LPCSTR)record->Fields->GetItem(1l)->GetName();
		long   lPhotoSize=record->Fields->Item[_variant_t(columnName)]->ActualSize;   
		if(maxsize<lPhotoSize)
		{
			CDBLogger::Ins()->Log("GetRecordBlob�Ļ�����̫С");
			return -1;
		}
		if (retSize)
			*retSize=lPhotoSize; 

		long   lRead=0;   		
		_variant_t   varChunk;   
		BYTE   buf[ChunkSize]; 
		long   destPoint = 0; //���ջ����ָ��
		while(lPhotoSize>0)   
		{   
			lRead=lPhotoSize>=ChunkSize?ChunkSize:lPhotoSize;  
			long l = record->State;
			FieldPtr f= record->Fields->Item[_variant_t(columnName)];
			varChunk   =   record->Fields->Item[_variant_t(columnName)]->GetChunk(lRead);   
			for(long   index=0;index<lRead;index++)                     
			{                         
				::SafeArrayGetElement(varChunk.parray,&index,buf+index);         
			}   
			memmove(retBuf + destPoint,buf,lRead);
			destPoint += lRead;
			lPhotoSize-=lRead;   
		}	
	}
	catch(_com_error e)
	{
		_bstr_t str = e.Description();
		CString s = (LPCSTR)str;
		CDBLogger::Ins()->Log("%s : level%d", s,5);
		return -1;
	}
	return 1;
}

/************************************************************************/
/* ��ȡ���������� (VarBinary�ͱ���)
/************************************************************************/
inline int CSqlHelper::GetRecordVarBinary(_RecordsetPtr record , CString columnName, char* retBuf, int maxsize,int* retSize)
{
	try
	{	
		CString tmp = (LPCSTR)record->Fields->GetItem(1l)->GetName();
		long   lPhotoSize=record->Fields->Item[_variant_t(columnName)]->ActualSize;   
		if(maxsize<lPhotoSize)
		{
			CDBLogger::Ins()->Log("GetRecordVarBinary�Ļ�����̫С");
			return -1;
		}

		if (retSize)
			*retSize=lPhotoSize; 

		char   *pTmp   =   NULL;    
		_variant_t varBLOB= record->Fields->Item[_variant_t(columnName)]->GetValue();
		::SafeArrayAccessData(varBLOB.parray, (void **)&pTmp);   
		if(pTmp==NULL)
		{
			return -1;        //ȡ��������?
		}
		memcpy(retBuf,pTmp,lPhotoSize); 
		::SafeArrayUnaccessData(varBLOB.parray);   		
	}
	catch(_com_error e)
	{
		_bstr_t str = e.Description();
		CString s = (LPCSTR)str;
		CDBLogger::Ins()->Log("%s : level%d", s,5);
		return -1;
	}
	return 1;	
}

/************************************************************************/
/* ��ȡ���������� (BLOB/VARBINARY�ͱ���)
/************************************************************************/
inline int CSqlHelper::GetRecordBinarySize(_RecordsetPtr record , CString columnName)
{
	try
	{
		return record->Fields->Item[_variant_t(columnName)]->ActualSize;
	}
	catch (...)
	{
		return 0;
	}
}

#endif