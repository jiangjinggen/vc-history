// TempHisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPMS.h"
#include "TempHisDlg.h"


// CTempHisDlg �Ի���

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


// CTempHisDlg ��Ϣ�������

void CTempHisDlg::OnBnClickedBtnThQry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CTempHisDlg::OnBnClickedBtnThClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
