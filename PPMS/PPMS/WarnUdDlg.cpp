// WarnUdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPMS.h"
#include "WarnUdDlg.h"


// CWarnUdDlg �Ի���

IMPLEMENT_DYNAMIC(CWarnUdDlg, CDialog)

CWarnUdDlg::CWarnUdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarnUdDlg::IDD, pParent)
{

}

CWarnUdDlg::~CWarnUdDlg()
{
}

void CWarnUdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarnUdDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_UD_MOD, &CWarnUdDlg::OnBnClickedBtnUdMod)
	ON_BN_CLICKED(IDC_BTN_UD_OK, &CWarnUdDlg::OnBnClickedBtnUdOk)
	ON_BN_CLICKED(IDC_BTN_UD_CLOSE, &CWarnUdDlg::OnBnClickedBtnUdClose)
END_MESSAGE_MAP()


// CWarnUdDlg ��Ϣ�������

void CWarnUdDlg::OnBnClickedBtnUdMod()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CWarnUdDlg::OnBnClickedBtnUdOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CWarnUdDlg::OnBnClickedBtnUdClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
