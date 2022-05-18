// TcpCommSvr.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "TcpCommSvrDlg.h"
#include "LoginDlg.h"

#include <winsock2.h>
#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>
#include <httpext.h>   
#include <windef.h>   
#include <Nb30.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"netapi32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTcpCommSvrApp

BEGIN_MESSAGE_MAP(CTcpCommSvrApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTcpCommSvrApp 构造

CTcpCommSvrApp::CTcpCommSvrApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTcpCommSvrApp 对象

CTcpCommSvrApp theApp;


// CTcpCommSvrApp 初始化
int getMAC(char * mac)
{
    NCB ncb;
    typedef struct _ASTAT_
    {
        ADAPTER_STATUS   adapt;
        NAME_BUFFER   NameBuff[30];
    }ASTAT, *PASTAT;

    ASTAT Adapter;
    typedef struct _LANA_ENUM
    {
        UCHAR length;
        UCHAR lana[MAX_LANA];
    }LANA_ENUM;
    LANA_ENUM lana_enum;
    UCHAR uRetCode;
    memset(&ncb, 0, sizeof(ncb));
    memset(&lana_enum, 0, sizeof(lana_enum));

    ncb.ncb_command = NCBENUM;
    ncb.ncb_buffer = (unsigned char *)&lana_enum;
    ncb.ncb_length = sizeof(LANA_ENUM);
    uRetCode = Netbios(&ncb);
    if (uRetCode != NRC_GOODRET)
        return uRetCode;

    for (int lana = 0; lana<lana_enum.length; lana++)
    {
        ncb.ncb_command = NCBRESET;
        ncb.ncb_lana_num = lana_enum.lana[lana];
        uRetCode = Netbios(&ncb);
        if (uRetCode == NRC_GOODRET)
            break;
    }
    if (uRetCode != NRC_GOODRET)
        return uRetCode;

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_enum.lana[0];
    strcpy_s((char*)ncb.ncb_callname,5, "*");
    ncb.ncb_buffer = (unsigned char *)&Adapter;
    ncb.ncb_length = sizeof(Adapter);
    uRetCode = Netbios(&ncb);
    if (uRetCode != NRC_GOODRET)
        return uRetCode;
	int iTmp1 = Adapter.adapt.adapter_address[0] + Adapter.adapt.adapter_address[1];
	int iTmp2 = Adapter.adapt.adapter_address[2] + Adapter.adapt.adapter_address[3];
	int iTmp3 = Adapter.adapt.adapter_address[4] + Adapter.adapt.adapter_address[5];
    sprintf_s(mac,18, "%03d-%03d-%03d",
        iTmp1,
        iTmp2,
        iTmp3
        );
    return 0;
}
BOOL CTcpCommSvrApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	//
	CString strPath(".\\cfg\\syscfg.ini");
	int protocolType = GetPrivateProfileInt("参数配置", "protocol_type",0,strPath.GetBuffer());
	if(1 == protocolType)
	{
		char stmp[256];
		memset(stmp,0,sizeof(stmp));
		getMAC(stmp);
		char sRegValue[256];
		memset(sRegValue,0,sizeof(sRegValue));
		char sRegCode[128];
		memset(sRegCode,0,sizeof(sRegCode));
		FILE *fp = fopen(stmp,"r");
		BOOL bReg = FALSE;
		if(NULL != fp)
		{
			fgets(sRegValue,256,fp);
			fclose(fp);
			fp = NULL;
			int len = strlen(sRegValue);
			if(22 != len)
			{
			}
			else
			{
				for(int i = 0; i < len/2; i++)
				{
					sRegCode[i] = (sRegValue[2*i] - 'A')*16 + (sRegValue[2*i+1] - 'A');
				}
				if(0 == strcmp(stmp,sRegCode))
				{
					bReg = TRUE;
				}
			}
		}
		if(!bReg)
		{
			CString strTmp = "未注册，注册码为：";
			strTmp += stmp;
			AfxMessageBox(strTmp);
			return FALSE;
		}
		//用户登录
		CLoginDlg lgDlg;
		INT_PTR ret = lgDlg.DoModal();
		if (ret == IDOK)
		{
			//if(("admin" == lgDlg.m_loginUser) && ("admin" == lgDlg.m_loginPassword))
			//{
			//	
			//}
			//else
			//{
			//	AfxMessageBox("用户名密码有误");
			//	return FALSE;
			//}
		}
		else
		{
			AfxMessageBox("取消登录");
			return FALSE;
		}
	}
	//
	CTcpCommSvrDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 TRUE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return TRUE;
}
