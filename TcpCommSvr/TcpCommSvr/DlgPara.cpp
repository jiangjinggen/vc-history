// DlgPara.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgPara.h"

// CDlgPara �Ի���

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
		//if (iniFile.GetIniSetting("��������", "ip", strValue))
		//{
		//	m_svrIp = strValue.c_str();
		//}
		////
		//if (iniFile.GetIniSetting("��������", "port", strValue))
		//{
		//	m_svrPort = atoi(strValue.c_str());
		//}
		////
		//if (iniFile.GetIniSetting("��������", "freshtime", strValue))
		//{
		//	m_freshTime = atoi(strValue.c_str());
		//}
		////
		//if (iniFile.GetIniSetting("��������", "loggrade", strValue))
		//{
		//	m_logGrade = atoi(strValue.c_str());
		//}
		memset(stmp,0,sizeof(stmp));
		GetPrivateProfileString("��������", "ip","",stmp,256,strPath.GetBuffer());
		m_svrIp = stmp;
		m_svrPort = GetPrivateProfileInt("��������", "port",0,strPath.GetBuffer());
		m_freshTime = GetPrivateProfileInt("��������", "freshtime",0,strPath.GetBuffer());
		m_logGrade = GetPrivateProfileInt("��������", "loggrade",0,strPath.GetBuffer());
	}
	//
	m_cbFreshTime.AddString("3��");
	m_cbFreshTime.AddString("5��");
	m_cbFreshTime.AddString("10��");
	m_cbFreshTime.AddString("30��");
	m_cbFreshTime.AddString("1����");
	m_cbFreshTime.AddString("2����");
	m_cbFreshTime.AddString("3����");
	m_cbFreshTime.SetCurSel(m_freshTime);
	//
	m_cbLogGrade.AddString("1��ͨ");
	m_cbLogGrade.AddString("2����");
	m_cbLogGrade.AddString("3����");
	m_cbLogGrade.SetCurSel(m_logGrade-1);
    //
	UpdateData(FALSE);
	//
	return TRUE;
}
// CDlgPara ��Ϣ�������

void CDlgPara::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_freshTime = m_cbFreshTime.GetCurSel();
	m_logGrade = m_cbLogGrade.GetCurSel() + 1;
	//д�����ļ�
	CString strPath(".\\cfg\\syscfg.ini");
	std::string strValue;
	char stmp[20];
	if (access(strPath, 0) != -1)
	{
        //WritePrivateProfileString("��������","ip",m_svrIp.GetBuffer(),strPath.GetBuffer());
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",m_svrPort);
		WritePrivateProfileString("��������","port",stmp,strPath.GetBuffer());
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",m_freshTime);
		WritePrivateProfileString("��������","freshtime",stmp,strPath.GetBuffer());
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%d",m_logGrade);
		WritePrivateProfileString("��������","loggrade",stmp,strPath.GetBuffer());
	}
	OnOK();
}

void CDlgPara::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
