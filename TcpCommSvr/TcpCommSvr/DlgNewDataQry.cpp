// DlgNewDataQry.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "DlgNewDataQry.h"
#include "TcpCommSvrDlg.h"
//
extern CTcpCommSvrDlg *g_pMainDlg;

// CDlgNewDataQry �Ի���

IMPLEMENT_DYNAMIC(CDlgNewDataQry, CDialog)

CDlgNewDataQry::CDlgNewDataQry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewDataQry::IDD, pParent)
	, m_strCardNo(_T(""))
	, m_strRoomNo(_T(""))
	, m_strPointNo(_T(""))
{

}

CDlgNewDataQry::~CDlgNewDataQry()
{
}

void CDlgNewDataQry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_CARD_NO_NEW, m_cmbCardNoNew);
	DDX_CBString(pDX, IDC_CMB_CARD_NO_NEW, m_strCardNo);
	DDX_Control(pDX, IDC_CMB_ROOM_NO_NEW, m_cmbRoomNo);
	DDX_CBString(pDX, IDC_CMB_ROOM_NO_NEW, m_strRoomNo);
	DDX_Control(pDX, IDC_CMB_POINT_NO_NEW, m_cmbPointNo);
	DDX_CBString(pDX, IDC_CMB_POINT_NO_NEW, m_strPointNo);
	DDX_Control(pDX, IDC_LIST_CARD_INFO_NEW, m_lstCardInfo);
	DDX_Control(pDX, IDC_LIST_ROOM_INFO_NEW, m_lstRoomInfo);
	DDX_Control(pDX, IDC_LIST_POINT_INFO_NEW, m_lstPointInfo);
}


BEGIN_MESSAGE_MAP(CDlgNewDataQry, CDialog)
	ON_BN_CLICKED(IDC_BTN_CARD_QRY_NEW, &CDlgNewDataQry::OnBnClickedBtnCardQryNew)
	ON_BN_CLICKED(IDC_BTN_ROOM_QRY_NEW, &CDlgNewDataQry::OnBnClickedBtnRoomQryNew)
	ON_BN_CLICKED(IDC_BTN_POINT_QRY_NEW, &CDlgNewDataQry::OnBnClickedBtnPointQryNew)
END_MESSAGE_MAP()


// CDlgNewDataQry ��Ϣ�������
BOOL CDlgNewDataQry::OnInitDialog()
{
	CDialog::OnInitDialog();
	DWORD dwStyle = m_lstCardInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstCardInfo.SetExtendedStyle(dwStyle);

	CRect lstRect;
	m_lstCardInfo.GetClientRect(&lstRect); //��õ�ǰ�ͻ�����Ϣ 
	//m_lstCardInfo.MoveWindow(&lstRect);
	m_lstCardInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, lstRect.Width() / 5);
	m_lstCardInfo.InsertColumn(1, _T("���������"), LVCFMT_LEFT, lstRect.Width() / 5);
	m_lstCardInfo.InsertColumn(2, _T("�ź�ǿ��"), LVCFMT_LEFT, lstRect.Width() / 5);
	m_lstCardInfo.InsertColumn(3, _T("last_time"), LVCFMT_LEFT, 2 * lstRect.Width() / 5);
	//
	dwStyle = m_lstRoomInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstRoomInfo.SetExtendedStyle(dwStyle);
	m_lstRoomInfo.GetClientRect(&lstRect); //��õ�ǰ�ͻ�����Ϣ 
	//m_lstRoomInfo.MoveWindow(&lstRect);
	m_lstRoomInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, lstRect.Width() / 7);
	m_lstRoomInfo.InsertColumn(1, _T("����"), LVCFMT_LEFT, 3 * lstRect.Width() / 7);
	m_lstRoomInfo.InsertColumn(2, _T("last_time"), LVCFMT_LEFT, 3 * lstRect.Width() / 7);
	//
	dwStyle = m_lstPointInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_lstPointInfo.SetExtendedStyle(dwStyle);
	m_lstPointInfo.GetClientRect(&lstRect); //��õ�ǰ�ͻ�����Ϣ 
	//m_lstRoomInfo.MoveWindow(&lstRect);
	m_lstPointInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, lstRect.Width() / 7);
	m_lstPointInfo.InsertColumn(1, _T("����"), LVCFMT_LEFT, 3 * lstRect.Width() / 7);
	m_lstPointInfo.InsertColumn(2, _T("last_time"), LVCFMT_LEFT, 3 * lstRect.Width() / 7);
	//
	return TRUE;
}
void CDlgNewDataQry::OnBnClickedBtnCardQryNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstCardInfo.DeleteAllItems();//���
	UpdateData(TRUE);
	char stmp[256];
	char scontents[256];
	int cardNo = 0;
	CString strCardNo;
	memset(stmp, 0, sizeof(stmp));
	GetDlgItemText(IDC_CMB_CARD_NO_NEW, strCardNo);
	//w2c(stmp, strCardNo.GetBuffer(), 2 * wcslen(strCardNo.GetBuffer()) + 1);
	strcpy(stmp,strCardNo.GetBuffer());
	cardNo = strtoul(stmp,NULL,10);
	if (cardNo < 0)
	{
		MessageBox(_T("�������ݲ��Ϸ���"));
		return;
	}
	//
	map<unsigned int, stuPointInfo> mapPointInfo;
	g_pMainDlg->GetCardPointInfo(cardNo, mapPointInfo);
	//
	//
	int iCount = 0;
	map<unsigned int, stuPointInfo>::iterator iter;
	for (iter = mapPointInfo.begin(); iter != mapPointInfo.end(); iter++)
	{
		iCount++;
		//���
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%05d", iCount);
		m_lstCardInfo.InsertItem(iCount - 1, CString(stmp));
		//���������
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%05d", iter->first);
		m_lstCardInfo.SetItemText(iCount - 1, 1, CString(stmp));
		//�ź�ǿ��
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%05d", iter->second.point_signal_x+iter->second.point_signal_y+iter->second.point_signal_z);
		m_lstCardInfo.SetItemText(iCount - 1, 2, CString(stmp));
		//ʱ��
		struct tm *now = NULL;
		now = localtime(&iter->second.lst_time);
		sprintf(stmp, "%d-%02d-%02d %d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		m_lstCardInfo.SetItemText(iCount - 1, 3, CString(stmp));
	}
}

void CDlgNewDataQry::OnBnClickedBtnRoomQryNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstRoomInfo.DeleteAllItems();//���
	UpdateData(TRUE);
	char stmp[256];
	char scontents[256];
	int roomNo = 0;
	CString strRoomNo;
	memset(stmp, 0, sizeof(stmp));
	GetDlgItemText(IDC_CMB_ROOM_NO_NEW, strRoomNo);
	//w2c(stmp, strRoomNo.GetBuffer(), 2 * wcslen(strRoomNo.GetBuffer()) + 1);
	strcpy(stmp, strRoomNo.GetBuffer());
	roomNo = atoi(stmp);
	if (roomNo < 0)
	{
		MessageBox(_T("�������ݲ��Ϸ���"));
		return;
	}
	//
	map< unsigned int, stuRoomInfo> mapCardRoomInfo;
	mapCardRoomInfo.clear();
	g_pMainDlg->GetRoomCardInfo(roomNo,mapCardRoomInfo);
	//
	int iCount = 0;
	map< unsigned int, stuRoomInfo>::iterator iter;
	for (iter = mapCardRoomInfo.begin(); iter != mapCardRoomInfo.end(); iter++)
	{
		iCount++;
		//���
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%05d", iCount);
		m_lstRoomInfo.InsertItem(iCount - 1, CString(stmp));
		//����
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%010d", iter->first);
		m_lstRoomInfo.SetItemText(iCount - 1, 1, CString(stmp));
		//ʱ��
		//struct tm *now = NULL;
		//now = localtime(&iter->second.lst_time);
		//sprintf(stmp, "%d-%02d-%02d %02d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		memset(stmp,0,sizeof(stmp));
		TimeToStr(stmp,iter->second.lst_time);
		m_lstRoomInfo.SetItemText(iCount - 1, 2, CString(stmp));
	}
}

void CDlgNewDataQry::OnBnClickedBtnPointQryNew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    m_lstPointInfo.DeleteAllItems();//���
	UpdateData(TRUE);
	char stmp[256];
	char scontents[256];
	int pointNo = 0;
	CString strRoomNo;
	memset(stmp, 0, sizeof(stmp));
	GetDlgItemText(IDC_CMB_POINT_NO_NEW, strRoomNo);
	//w2c(stmp, strRoomNo.GetBuffer(), 2 * wcslen(strRoomNo.GetBuffer()) + 1);
	strcpy(stmp, strRoomNo.GetBuffer());
	pointNo = atoi(stmp);
	if (pointNo < 0)
	{
		MessageBox(_T("�������ݲ��Ϸ���"));
		return;
	}
	//
	map<unsigned int, stuPointInfo> mapCardInfo;
	mapCardInfo.clear();
	g_pMainDlg->GetPointCardInfo(pointNo, mapCardInfo);
	//
	int iCount = 0;
	map<unsigned int, stuPointInfo>::iterator iter;
	for (iter = mapCardInfo.begin(); iter != mapCardInfo.end(); iter++)
	{
		iCount++;
		//���
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%05d", iCount);
		m_lstPointInfo.InsertItem(iCount - 1, CString(stmp));
		//����
		memset(stmp, 0, sizeof(stmp));
		sprintf(stmp, "%010d", iter->first);
		m_lstPointInfo.SetItemText(iCount - 1, 1, CString(stmp));
		//ʱ��
		struct tm *now = NULL;
		now = localtime(&iter->second.lst_time);
		sprintf(stmp, "%d-%02d-%02d %d:%02d:%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		m_lstPointInfo.SetItemText(iCount - 1, 2, CString(stmp));
	}
}
