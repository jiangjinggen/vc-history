// HrateHisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PPMS.h"
#include "HrateHisDlg.h"


// CHrateHisDlg 对话框

IMPLEMENT_DYNAMIC(CHrateHisDlg, CDialog)

CHrateHisDlg::CHrateHisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHrateHisDlg::IDD, pParent)
{

}

CHrateHisDlg::~CHrateHisDlg()
{
}

void CHrateHisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHrateHisDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_HR_QRY, &CHrateHisDlg::OnBnClickedBtnHrQry)
	ON_BN_CLICKED(IDC_BTN_HR_CLOSE, &CHrateHisDlg::OnBnClickedBtnHrClose)
END_MESSAGE_MAP()


// CHrateHisDlg 消息处理程序

void CHrateHisDlg::OnBnClickedBtnHrQry()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CHrateHisDlg::OnBnClickedBtnHrClose()
{
	// TODO: 在此添加控件通知处理程序代码
}
