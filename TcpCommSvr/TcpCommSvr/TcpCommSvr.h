// TcpCommSvr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../comm/CommDefine.h"

///////////////////
// CTcpCommSvrApp:
// �йش����ʵ�֣������ TcpCommSvr.cpp
//

class CTcpCommSvrApp : public CWinApp
{
public:
	CTcpCommSvrApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTcpCommSvrApp theApp;