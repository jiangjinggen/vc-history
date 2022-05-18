// TcpCommSvr.cpp : ����Ӧ�ó��������Ϊ��
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


// CTcpCommSvrApp ����

CTcpCommSvrApp::CTcpCommSvrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTcpCommSvrApp ����

CTcpCommSvrApp theApp;


// CTcpCommSvrApp ��ʼ��
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
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	//
	CString strPath(".\\cfg\\syscfg.ini");
	int protocolType = GetPrivateProfileInt("��������", "protocol_type",0,strPath.GetBuffer());
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
			CString strTmp = "δע�ᣬע����Ϊ��";
			strTmp += stmp;
			AfxMessageBox(strTmp);
			return FALSE;
		}
		//�û���¼
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
			//	AfxMessageBox("�û�����������");
			//	return FALSE;
			//}
		}
		else
		{
			AfxMessageBox("ȡ����¼");
			return FALSE;
		}
	}
	//
	CTcpCommSvrDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� TRUE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return TRUE;
}
