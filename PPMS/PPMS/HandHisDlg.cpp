// HandHisDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PPMS.h"
#include "HandHisDlg.h"


// CHandHisDlg �Ի���

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


// CHandHisDlg ��Ϣ�������

void CHandHisDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CHandHisDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
