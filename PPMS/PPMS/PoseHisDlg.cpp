// PoseHisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PPMS.h"
#include "PoseHisDlg.h"


// CPoseHisDlg 对话框

IMPLEMENT_DYNAMIC(CPoseHisDlg, CDialog)

CPoseHisDlg::CPoseHisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPoseHisDlg::IDD, pParent)
{

}

CPoseHisDlg::~CPoseHisDlg()
{
}

void CPoseHisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPoseHisDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_PH_QRY, &CPoseHisDlg::OnBnClickedBtnPhQry)
	ON_BN_CLICKED(IDC_BTN_PH_CLOSE, &CPoseHisDlg::OnBnClickedBtnPhClose)
END_MESSAGE_MAP()


// CPoseHisDlg 消息处理程序

void CPoseHisDlg::OnBnClickedBtnPhQry()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CPoseHisDlg::OnBnClickedBtnPhClose()
{
	// TODO: 在此添加控件通知处理程序代码
}
