// SqlHelper.h: interface for the CSqlHelper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SQLOBJECT_H
#define SQLOBJECT_H

//涂鹏飞2007/5/15添加
//为添加数据库接口
#include <memory>
#include <afxmt.h>
#include <icrsint.h> 
#include "MyDatabase.h"
#include "param.h"
#include <io.h>
#include <vector>
using namespace std;
//********************************
//类函数的可重入问题
//Auto模式下不存在可重入问题，每次函数运行都是都只能调用safeexec

//User模式下存在可重入问题，共用一个CDatabase指针，
//			每个线程内部new一个CSqlHelper对象才能保证该类方法的可重入
//			或者是用safeexec
//			如果要用trans，调用方自己在begin，end之间加锁防护

//User模式，Auto模式的介绍及 User模式需要否，参见文档。
//********************************
class  CSqlObject  
{
	friend class CSqlHelper;
public:	
	typedef void (*ConnStateCallBack)(bool state);
	CSqlObject():m_pfnConnStateCallBack(NULL),m_bConnState(true){};
	CSqlObject(EConnStrategy connStartegy);
	virtual ~CSqlObject();

	//这个函数只用在单连接，注意区分主备模式下的MS_ResetConnectStr
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
	CString m_connectStr;			//连接字符串结构
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
/* 重设设置连接
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
/* 重连数据库
/***********************************************************************/
inline int CSqlObject::OpenDbConnection()
{
	if(m_connectStr=="")
	{
		CDBLogger::Ins()->Log("连接字符串为空，无法连接数据库，请检查配置文件");
		return ConnRt_Open_Err;
	}

	m_Conn->CloseConnection();
	return m_Conn->OpenConnection(m_connectStr);
}


/************************************************************************/
/* 被重载
/************************************************************************/
inline int CSqlObject::OpenConnection()
{
	if(m_connectStr=="")
	{
		CDBLogger::Ins()->Log("连接字符串为空，请检查配置文件");
		return ConnRt_Open_Err;
	}
	return m_Conn->OpenConnection(m_connectStr);
}


/************************************************************************/
/* 被重载

返回值
ConnRt_Open_Err		AUTO连接失败
说明：
如果执行的存储过程里面有多个结果集返回的话，取最后一个结果集
如果有返回记录集，一定记住用完RS->Close()关闭它

USER 连接一直使用同一个MyConnection对象
AUTO 不允许执行这个exec，需要safeexec
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
		//user模式的exec可能会由于mysql的goneaway行为导致连接断开，需要重试一次
		//[LiuXiaofeng 2011-12-7] 此处优化处理: 每次发现连接中断后，都做重连处理；明确的连接中断期间不执行存储过程
		if(m_bConnState)		// 第一次进入该函数时前次长连接的状态肯定是正常的，否则无法正常创建SqlObject对象！
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
/* 使用于单体的user、auto模式。
auto模式只能用这个safeexec，不能exec

trans只能用在user。
如果单体user，需要用sqlhelper的锁手动加锁，再用exec
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
