// SafServerApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
//
#pragma warning(disable:4996)
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../comm/CommDefine.h"

using namespace std;
//////
// CSafServerAppApp:
// �йش����ʵ�֣������ SafServerApp.cpp
////
//
class CSafServerAppApp : public CWinApp
{
public:
	CSafServerAppApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSafServerAppApp theApp;