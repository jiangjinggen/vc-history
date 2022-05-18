// DlgCardQry.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgCardQry.h"
#include "TcpCommSvrDlg.h"
//
extern CTcpCommSvrDlg *g_pMainDlg;
// CDlgCardQry �Ի���

IMPLEMENT_DYNAMIC(CDlgCardQry, CDialog)

CDlgCardQry::CDlgCardQry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCardQry::IDD, pParent)
{

}

CDlgCardQry::~CDlgCardQry()
{
}

void CDlgCardQry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbCardNo);
	DDX_Control(pDX, IDC_LIST1, m_lstReaderNo);
}


BEGIN_MESSAGE_MAP(CDlgCardQry, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCardQry::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCardQry::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CARD_QRY, &CDlgCardQry::OnBnClickedBtnCardQry)
END_MESSAGE_MAP()

//
BOOL CDlgCardQry::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
    DWORD dwStyle = m_lstReaderNo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstReaderNo.SetExtendedStyle(dwStyle);
	
	CRect rect;
	m_lstReaderNo.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstReaderNo.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/12);
    m_lstReaderNo.InsertColumn(1, _T("�豸���"), LVCFMT_LEFT, rect.Width()/6);
    m_lstReaderNo.InsertColumn(2, _T("����"), LVCFMT_LEFT, rect.Width()/3);
	m_lstReaderNo.InsertColumn(3, _T("��������"), LVCFMT_LEFT, rect.Width()/6);
    m_lstReaderNo.InsertColumn(4, _T("ʱ��"), LVCFMT_LEFT, rect.Width()/3); 
    ////
	return TRUE;
}
// CDlgCardQry ��Ϣ�������

void CDlgCardQry::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDlgCardQry::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDlgCardQry::OnBnClickedBtnCardQry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstReaderNo.DeleteAllItems();//���
	UpdateData(TRUE);
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
	m_cbCardNo.GetWindowTextA(stmp,255);
	unsigned int cardNo = strtoul(stmp,NULL,10);
	if(cardNo <= 0)
	{
		MessageBox("�������ݲ��Ϸ���");
		return ;
	}
	//map<unsigned int,stuCardInfo> mapCardInfo;
	//mapCardInfo.clear();
	//g_pMainDlg->GetReaderInfo(cardNo,mapCardInfo);
	////
	//int iCount = 0;
	//CString strCardNo;
	//map<unsigned int,stuCardInfo>::iterator iter;
	//for(iter = mapCardInfo.begin();iter != mapCardInfo.end();iter++)
	vector<stuReaderInfo> vecCardInfo;
	vecCardInfo.clear();
	g_pMainDlg->GetReaderInfoVec(cardNo,vecCardInfo);
	//
	int iCount = 0;
	CString strCardNo;
	vector<stuReaderInfo>::iterator iter;
	for(iter = vecCardInfo.begin();iter != vecCardInfo.end();iter++)
	{
		if(0 == iter->readCount)continue;
		iCount++;
		//���
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_lstReaderNo.InsertItem(iCount-1, _T(stmp));
		//�豸���
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iter->readNo);
		m_lstReaderNo.SetItemText(iCount-1, 1, _T(stmp));
		//����
		memset(stmp,0,sizeof(stmp));
		strCardNo.Format("%010u",cardNo);
		sprintf(stmp,"%s",strCardNo.GetBuffer());
		m_lstReaderNo.SetItemText(iCount-1, 2, _T(stmp));
		//��������
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iter->readCount);
		m_lstReaderNo.SetItemText(iCount-1, 3, _T(stmp));
		//ʱ��
		struct tm *now = NULL;
		now = localtime(&iter->lstTime);
		sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
		m_lstReaderNo.SetItemText(iCount-1, 4, _T(stmp));
	}
}
