// PoseHisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPMS.h"
#include "PoseHisDlg.h"


// CPoseHisDlg �Ի���

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


// CPoseHisDlg ��Ϣ�������

void CPoseHisDlg::OnBnClickedBtnPhQry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CPoseHisDlg::OnBnClickedBtnPhClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
