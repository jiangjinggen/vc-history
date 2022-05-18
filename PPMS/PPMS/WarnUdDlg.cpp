// WarnUdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PPMS.h"
#include "WarnUdDlg.h"


// CWarnUdDlg 对话框

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


// CWarnUdDlg 消息处理程序

void CWarnUdDlg::OnBnClickedBtnUdMod()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CWarnUdDlg::OnBnClickedBtnUdOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CWarnUdDlg::OnBnClickedBtnUdClose()
{
	// TODO: 在此添加控件通知处理程序代码
}
