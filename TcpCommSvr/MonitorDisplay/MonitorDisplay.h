// MonitorDisplay.h : MonitorDisplay DLL 的主头文件
//

#pragma once
#pragma warning(disable:4996)
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "../comm/CommDefine.h"

///////////////////
// CMonitorDisplayApp
// 有关此类实现的信息，请参阅 MonitorDisplay.cpp
//

class CMonitorDisplayApp : public CWinApp
{
public:
	CMonitorDisplayApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
