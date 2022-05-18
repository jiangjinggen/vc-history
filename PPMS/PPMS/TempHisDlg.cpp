// TempHisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PPMS.h"
#include "TempHisDlg.h"


// CTempHisDlg 对话框

IMPLEMENT_DYNAMIC(CTempHisDlg, CDialog)

CTempHisDlg::CTempHisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTempHisDlg::IDD, pParent)
{

}

CTempHisDlg::~CTempHisDlg()
{
}

void CTempHisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempHisDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_TH_QRY, &CTempHisDlg::OnBnClickedBtnThQry)
	ON_BN_CLICKED(IDC_BTN_TH_CLOSE, &CTempHisDlg::OnBnClickedBtnThClose)
END_MESSAGE_MAP()


// CTempHisDlg 消息处理程序

void CTempHisDlg::OnBnClickedBtnThQry()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTempHisDlg::OnBnClickedBtnThClose()
{
	// TODO: 在此添加控件通知处理程序代码
}
