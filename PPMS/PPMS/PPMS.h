// PPMS.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "../comm/CommDefine.h"
// CPPMSApp:
// See PPMS.cpp for the implementation of this class
//

class CPPMSApp : public CWinApp
{
public:
	CPPMSApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPPMSApp theApp;