// MonitorDisplay.h : MonitorDisplay DLL ����ͷ�ļ�
//

#pragma once
#pragma warning(disable:4996)
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../comm/CommDefine.h"

///////////////////
// CMonitorDisplayApp
// �йش���ʵ�ֵ���Ϣ������� MonitorDisplay.cpp
//

class CMonitorDisplayApp : public CWinApp
{
public:
	CMonitorDisplayApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
