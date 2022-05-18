// DlgPara.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgPara.h"

// CDlgPara 对话框

IMPLEMENT_DYNAMIC(CDlgPara, CDialog)

CDlgPara::CDlgPara(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPara::IDD, pParent)
	, m_svrIp(_T(""))
	, m_svrPort(0)
{

}

CDlgPara::~CDlgPara()
{
}

void CDlgPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_svrIp);
	DDX_Text(pDX, IDC_EDIT2, m_svrPort);
	DDX_Control(pDX, IDC_COMBO1, m_cbFreshTime);
	DDX_Control(pDX, IDC_COMBO2, m_cbLogGrade);
}


BEGIN_MESSAGE_MAP(CDlgPara, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPara::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgPara::OnBnClickedCancel)
END_MESSAGE_MAP()

//
BOOL CDlgPara::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	m_svrIp = "127.0.0.1";
    m_svrPort = 8888;
	m_freshTime = 1;
	m_logGrade = 2;
	CString strPath(".\\cfg\\syscfg.ini");
	std::string strValue;
	char stmp[20];
	if (access(strPath, 0) != -1)
	{
		//TIniFile iniFile(strPath);
		//if (iniFile.GetIniSetting("参数配置", "ip", strValue))
		//{
		//	m_svrIp = strValue.c_str();
		//}
		////
		//if (iniFile.GetIniSetting("参数配置", "port", strValue))
		//{
		//	m_svrPort = atoi(strValue.c_str());
		//}
		////
		//if (iniFile.GetIniSetting("参数配置", "freshtime", strValue))
		//{
		//	m_freshTime = atoi(strValue.c_str());
		//}
		////
		//if (iniFile.GetIniSetting("参数配置", "loggrade", strValue))
		//{
		//	m_logGrade = atoi(strValue.c_str());
		//}
		memset(stmp,0,sizeof(stmp));
		GetPrivateProfileString("参数配置", "ip","",stmp,256,strPath.GetBuffer());
		m_svrIp = stmp;
		m_svrPort = GetPrivateProfileInt("参数配置", "port",0,strPath.GetBuffer());
		m_freshTime = GetPrivateProfileInt("参数配置", "freshtime",0,strPath.GetBuffer());
		m_logGrade = GetPrivateProfileInt("参数配置", "loggrade",0,strPath.GetBuffer());
	}
	//
	m_cbFreshTime.AddString("3秒");
	m_cbFreshTime.AddString("5秒");
	m_cbFreshTime.AddString("10秒");
	m_cbFreshTime.AddString("30秒");
	m_cbFreshTime.AddString("1分钟");
	m_cbFreshTime.AddString("2分钟");
	m_cbFreshTime.AddString("3分钟");
	m_cbFreshTime.SetCurSel(m_freshTime);
	//
	m_cbLogGrade.AddString("1普通");
	m_cbLogGrade.AddString("2警告");
	m_cbLogGrade.AddString("3错误");
	m_cbLogGrade.SetCurSel(m_logGrade-1);
    //
	UpdateData(FALSE);
	//
	return TRUE;
}
// CDlgPara 消息处理程序

void CDlgPara::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_freshTime = m_cbFreshTime.GetCurSel();
	m_logGrade = m_cbLogGrade.GetCurSel() + 1;
	//写配置文件
	CString strPath(".\\cfg\\syscfg.ini");
	std::string strValue;
	char stmp[20];
	if (access(strPath, 0) != -1)
	{
        //WritePrivateProfileString("参数配置","ip",m_svrIp.GetBuffer(),strPath.GetBuffer());
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",m_svrPort);
		WritePrivateProfileString("参数配置","port",stmp,strPath.GetBuffer());
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",m_freshTime);
		WritePrivateProfileString("参数配置","freshtime",stmp,strPath.GetBuffer());
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",m_logGrade);
		WritePrivateProfileString("参数配置","loggrade",stmp,strPath.GetBuffer());
	}
	OnOK();
}

void CDlgPara::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
