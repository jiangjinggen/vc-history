// BaseInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPMS.h"
#include "BaseInfoDlg.h"


// CBaseInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CBaseInfoDlg, CDialog)

CBaseInfoDlg::CBaseInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBaseInfoDlg::IDD, pParent)
{

}

CBaseInfoDlg::~CBaseInfoDlg()
{
}

void CBaseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_MOD_INFO, &CBaseInfoDlg::OnBnClickedBtnModInfo)
	ON_BN_CLICKED(IDC_BTN_INFO_OK, &CBaseInfoDlg::OnBnClickedBtnInfoOk)
	ON_BN_CLICKED(IDC_BTN_INFO_CLOSE, &CBaseInfoDlg::OnBnClickedBtnInfoClose)
END_MESSAGE_MAP()


// CBaseInfoDlg ��Ϣ�������

void CBaseInfoDlg::OnBnClickedBtnModInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CBaseInfoDlg::OnBnClickedBtnInfoOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CBaseInfoDlg::OnBnClickedBtnInfoClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
