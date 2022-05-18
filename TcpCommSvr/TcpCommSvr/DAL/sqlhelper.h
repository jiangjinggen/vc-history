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

		CString m_strCurConnectStr;		// 当前有效的连接字串

		void (*m_pfnDBChange)(LPCSTR pszCurDBIP);

		void (*m_pfnConnStateCallBack)(bool state);
	};

	static CSqlObject* User(DB_CFG* pDBCfg);      // 数据库连接等待时间默认5秒
	static CSqlObject* Auto(DB_CFG* pDBCfg);      // 数据库连接等待时间默认5秒
	static CSqlObject* NewUser(DB_CFG* pDBCfg);   // 多组件调用
	static int GetRecordBlob(_RecordsetPtr record , CString columnName, char* retBuf, int maxsize,int* retSize);
	static int GetRecordVarBinary(_RecordsetPtr record , CString columnName, char* retBuf,int maxsize, int* retSize);
	static int GetRecordBinarySize(_RecordsetPtr record , CString columnName);
	static time_t VariantToTime(_variant_t v);

	static CCriticalSection* m_cs_usersinglton; //user单体的时候，除了用safeexec外，可以外部自己加锁。
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
/*	配置连接式、User模式、线程单体（每个线程1个实体）类
推荐客户管理CSqlHelper实例， 使用NewUser创建一个实例

使用限制：
1 该函数只用于单组件程序，用于DLL中会出现异常情况。
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

		if(s_pSqlHelper->m_connectStr=="")//之后一定要通过ResetConnection配置连接
			return s_pSqlHelper;

		if(s_pSqlHelper->OpenConnection()==ConnRt_Open_Err) //数据库连接
		{	
			return NULL;
		}
	}
	return s_pSqlHelper;
}

/************************************************************************/
/* 该实例每个组件创建一个
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
/*该实例每个线程一个，创建时候则维护一个数据库连接
推荐客户管理CSqlHelper实例， 使用NewUser创建一个实例
该函数用于多组件程序
// 调用此函数得到的返回值不是thread-safe的!!!
/************************************************************************/
inline CSqlObject* CSqlHelper::NewUser(DB_CFG* pDBCfg) 
{
	CSqlObject* s_pSqlHelper=NULL;
	if(pDBCfg)	
		s_pSqlHelper= GetConfigMode(ENUM_User, pDBCfg);
	else
		s_pSqlHelper=new CSqlObject(ENUM_User);

	if(s_pSqlHelper->m_connectStr=="")//之后一定要通过ResetConnection配置连接
		return s_pSqlHelper;

	if( s_pSqlHelper->OpenConnection()==ConnRt_Open_Err) //数据库连接
	{
		CDBLogger::Ins()->Log("数据库连接出错");
		delete s_pSqlHelper;
		return NULL;
	}
	return s_pSqlHelper;
}
#define ChunkSize 5000 
/************************************************************************/
/* 辅助函数，获取Record对象的BLOB二进制数据
record: ADO RECORD对象
columnName: 包含二进制数据的列名
retBuf: 取到的数据的地址
retSize: 取到的数据量的大小 

返回值：
-1 取值失败
1  成功                                                                    */
/************************************************************************/
inline int CSqlHelper::GetRecordBlob(_RecordsetPtr record , CString columnName, char* retBuf,int maxsize, int* retSize)
{
	try
	{	
		CString tmp = (LPCSTR)record->Fields->GetItem(1l)->GetName();
		long   lPhotoSize=record->Fields->Item[_variant_t(columnName)]->ActualSize;   
		if(maxsize<lPhotoSize)
		{
			CDBLogger::Ins()->Log("GetRecordBlob的缓存区太小");
			return -1;
		}
		if (retSize)
			*retSize=lPhotoSize; 

		long   lRead=0;   		
		_variant_t   varChunk;   
		BYTE   buf[ChunkSize]; 
		long   destPoint = 0; //接收缓存的指针
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
/* 获取二进制数据 (VarBinary型变量)
/************************************************************************/
inline int CSqlHelper::GetRecordVarBinary(_RecordsetPtr record , CString columnName, char* retBuf, int maxsize,int* retSize)
{
	try
	{	
		CString tmp = (LPCSTR)record->Fields->GetItem(1l)->GetName();
		long   lPhotoSize=record->Fields->Item[_variant_t(columnName)]->ActualSize;   
		if(maxsize<lPhotoSize)
		{
			CDBLogger::Ins()->Log("GetRecordVarBinary的缓存区太小");
			return -1;
		}

		if (retSize)
			*retSize=lPhotoSize; 

		char   *pTmp   =   NULL;    
		_variant_t varBLOB= record->Fields->Item[_variant_t(columnName)]->GetValue();
		::SafeArrayAccessData(varBLOB.parray, (void **)&pTmp);   
		if(pTmp==NULL)
		{
			return -1;        //取不到数据?
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
/* 获取二进制数据 (BLOB/VARBINARY型变量)
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