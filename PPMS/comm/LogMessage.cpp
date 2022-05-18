#include "stdafx.h"
//日志处理
#include "LogMessage.h"

CLogMessage::CLogMessage()
{
    InitializeCriticalSection(&m_csloglist);
}

CLogMessage::~CLogMessage()
{
    DeleteCriticalSection(&m_csloglist);
}
//
bool CLogMessage::AddLogMessage(stuLogContent msg)
{
    EnterCriticalSection(&m_csloglist);
	//
	m_logmessage.push_back(msg);
	//
	LeaveCriticalSection(&m_csloglist);
	return true;
}
//
bool CLogMessage::GetLogMessage(stuLogContent *pmsg)
{
	bool bResult;
    EnterCriticalSection(&m_csloglist);
	//
	if (m_logmessage.empty()) 
	{
		bResult = false;
	}
	else 
	{
		bResult = true;
		*pmsg = m_logmessage.front();
		m_logmessage.pop_front();
	}
	//
	LeaveCriticalSection(&m_csloglist);
	return bResult;
}
