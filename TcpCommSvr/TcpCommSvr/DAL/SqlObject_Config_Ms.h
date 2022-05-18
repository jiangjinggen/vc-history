#ifndef SQLOBJECT_CONFIG_MS_H
#define SQLOBJECT_CONFIG_MS_H

#include "SqlObject.h"
class CSqlObject_Config_Ms:public CSqlObject
{
	friend class CSqlHelper;
public:
	typedef void (*ProcessDBChangeFn)(LPCSTR pszCurDBIP);

	CSqlObject_Config_Ms(EConnStrategy connStartegy);
	virtual ~CSqlObject_Config_Ms(){};

	virtual int OpenConnection();
	virtual int MS_ResetConnectStr(CString connectstr);
	virtual _RecordsetPtr Exec(CString sp,int sqlType, CInParamList* pParamList, int* retCode=NULL,int* affectRows = NULL);
	virtual void DetectDB();
protected:
	CString m_connectStr_1;
	CString m_connectStr_2;

	ProcessDBChangeFn m_pfn;
	CString m_strMasterIP;
	CString m_strSlaveIP;
};

inline CSqlObject_Config_Ms::CSqlObject_Config_Ms(EConnStrategy connStartegy)
: CSqlObject(connStartegy), m_pfn(NULL)
{
}

/************************************************************************/
/* [LiuXiaofeng] 检测当前可用数据库，优先连主机，主机连不上再连备机
/************************************************************************/
inline void CSqlObject_Config_Ms::DetectDB()
{
	m_connectStr = m_connectStr_1;	//检测Master DB
	if(CSqlObject::OpenConnection() == ConnRt_Open_Success)
	{
		if(m_pfn)
			m_pfn(m_strMasterIP);
	}
	m_Conn->CloseConnection();
}

/************************************************************************/
/* [LiuXiaofeng] 先连当前连接字符串，连不上时切换另外一个连接，连接成功时再回调应用层DB切换处理
/************************************************************************/
inline int CSqlObject_Config_Ms::OpenConnection()
{
	if(CSqlObject::OpenConnection() == ConnRt_Open_Err)
	{
		CString strChangeDB;
		if(m_connectStr==m_connectStr_1)
		{
			m_connectStr=m_connectStr_2;
			strChangeDB = m_strSlaveIP;
		}
		else if(m_connectStr==m_connectStr_2)
		{
			m_connectStr=m_connectStr_1;
			strChangeDB = m_strMasterIP;
		}
		else
		{
			ASSERT(0);
		}

		int nRet = CSqlObject::OpenConnection();
		if (m_pfn && (nRet== ConnRt_Open_Success))
			m_pfn(strChangeDB);

		return nRet;
	}
	return ConnRt_Open_Success;
}

/************************************************************************/
/* [LiuXiaofeng] 重新设置连接字符串，不回调DB切换
/************************************************************************/
inline int CSqlObject_Config_Ms::MS_ResetConnectStr(CString connectstr)
{
	if(m_connectStr==connectstr)
		return ConnRt_Open_Success;
	else
	{
		if(m_connectStr_1==connectstr)
		{
			m_connectStr=m_connectStr_1;
		}
		else if(m_connectStr_2==connectstr)
		{
			m_connectStr=m_connectStr_2;
		}
		else
		{
			CDBLogger::Ins()->Log("设置的连接错误");
			return ConnRt_Open_Err;
		}
		return CSqlObject::OpenConnection();
	}
}

/************************************************************************/
/* [LiuXiaofeng] 先执行一遍，Auto模式时如果执行时断开连接了，再重新执行一次
/* USER模式不需要，因为在基类Exec中已经做了重连处理
/************************************************************************/
inline _RecordsetPtr CSqlObject_Config_Ms::Exec(CString sp,int sqlType, CInParamList* pParamList, int* retCode,int* affectRows)
{
	int inner_retcode = 0;
	_RecordsetPtr ret =CSqlObject::Exec(sp,sqlType,pParamList,&inner_retcode,affectRows);
	if(IsExecConnErr(inner_retcode))
	{
		if(m_connStartegy==ENUM_Auto)
		{
			ret=CSqlObject::Exec(sp,sqlType,pParamList,retCode,affectRows);
		}
	}
	if(retCode!=NULL) *retCode=inner_retcode;
	return ret;
}


#endif