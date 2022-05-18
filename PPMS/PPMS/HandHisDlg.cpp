// HandHisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PPMS.h"
#include "HandHisDlg.h"


// CHandHisDlg 对话框

IMPLEMENT_DYNAMIC(CHandHisDlg, CDialog)

CHandHisDlg::CHandHisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHandHisDlg::IDD, pParent)
{

}

CHandHisDlg::~CHandHisDlg()
{
}

void CHandHisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHandHisDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CHandHisDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CHandHisDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CHandHisDlg 消息处理程序

void CHandHisDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CHandHisDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}
