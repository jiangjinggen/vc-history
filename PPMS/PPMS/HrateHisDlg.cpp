// HrateHisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPMS.h"
#include "HrateHisDlg.h"


// CHrateHisDlg �Ի���

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


// CHrateHisDlg ��Ϣ�������

void CHrateHisDlg::OnBnClickedBtnHrQry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CHrateHisDlg::OnBnClickedBtnHrClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
