// BaseInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PPMS.h"
#include "BaseInfoDlg.h"


// CBaseInfoDlg 对话框

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


// CBaseInfoDlg 消息处理程序

void CBaseInfoDlg::OnBnClickedBtnModInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBaseInfoDlg::OnBnClickedBtnInfoOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBaseInfoDlg::OnBnClickedBtnInfoClose()
{
	// TODO: 在此添加控件通知处理程序代码
}
