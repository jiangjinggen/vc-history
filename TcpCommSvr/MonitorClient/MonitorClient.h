// MonitorClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../comm/CommDefine.h"
///////////////////
// CMonitorClientApp:
// �йش����ʵ�֣������ MonitorClient.cpp
//

class CMonitorClientApp : public CWinApp
{
public:
	CMonitorClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMonitorClientApp theApp;