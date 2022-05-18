// ParaMng.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "ParaMng.h"


// CParaMng �Ի���

IMPLEMENT_DYNAMIC(CParaMng, CDialog)

CParaMng::CParaMng(CWnd* pParent /*=NULL*/)
	: CDialog(CParaMng::IDD, pParent)
{

}

CParaMng::~CParaMng()
{
}

void CParaMng::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PARA, m_treeParaMng);
	DDX_Control(pDX, IDC_TAB1, m_tabTab1);
}


BEGIN_MESSAGE_MAP(CParaMng, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PARA, &CParaMng::OnNMClickTreePara)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_PARA, &CParaMng::OnNMDblclkTreePara)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PARA, &CParaMng::OnTvnSelchangedTreePara)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CParaMng::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CParaMng ��Ϣ�������
BOOL CParaMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//��Ӳ�������tree
	HTREEITEM hItem = m_treeParaMng.InsertItem("��������",NULL,NULL);///root���ǽڵ�ı��� 
    //�û�����
	HTREEITEM hSubItem = m_treeParaMng.InsertItem("�û�����",NULL,NULL,hItem); 
	m_treeParaMng.InsertItem("����û�",NULL,NULL,hSubItem); 
	//m_treeParaMng.InsertItem("ɾ���û�",NULL,NULL,hSubItem);
	//m_treeParaMng.InsertItem("�޸��û�",NULL,NULL,hSubItem);
	//ҵ�����
    hSubItem = m_treeParaMng.InsertItem("��������",NULL,NULL,hItem); 
	m_treeParaMng.InsertItem("�豸����",NULL,NULL,hSubItem); 
	m_treeParaMng.InsertItem("�������",NULL,NULL,hSubItem);
	m_treeParaMng.InsertItem("��Ƭ����",NULL,NULL,hSubItem);
	//������
    hSubItem = m_treeParaMng.InsertItem("������",NULL,NULL,hItem); 
	//m_treeParaMng.InsertItem("���ܱ��",NULL,NULL,hSubItem); 
	m_treeParaMng.InsertItem("��ϸ���",NULL,NULL,hSubItem);
	//
	CRect dlgRect;
	GetClientRect(&dlgRect);
	//
	CRect tabRect;
	tabRect.left = dlgRect.Width()/5 - 10;
	tabRect.top = dlgRect.top + 10;
	tabRect.right = dlgRect.right - 10;
	tabRect.bottom = dlgRect.bottom - 10;
	//LPRECT lpRect;
	//m_tabTab1.MoveWindow(&tabRect);
	m_tabTab1.SetWindowPos(this, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	//CSize itemSize;
	//itemSize.cx = m_cx / 12;
	//itemSize.cy = m_cy / 24;
	//m_tabTab1.SetItemSize(itemSize);
	m_tabTab1.InsertItem(0, _T("��������"));
	m_tabTab1.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	//
	CRect mngRect;
	mngRect.left = dlgRect.Width()/5 + 20;
	mngRect.top = dlgRect.top;
	mngRect.right = dlgRect.right;
	mngRect.bottom = dlgRect.bottom;
	CWnd *pWnd = CWnd::FromHandle(this->m_hWnd);
	m_dlgUserMng.Create(IDD_DLG_USER_MNG,pWnd);
	m_dlgDevMng.Create(IDD_DLG_DEV_MNG,pWnd);
	m_dlgZoneMng.Create(IDD_DLG_ZONE_MNG,pWnd);
	m_dlgCardMng.Create(IDD_DLG_CARD_MNG,pWnd);
	m_dlgTblMng.Create(IDD_DLG_TBL_MNG,pWnd);
	// ���ݵ����õ�mngRect����m_dlgUserMng�ӶԻ��򣬲�����Ϊ��ʾ   
	m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
	m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	//
	return TRUE;
}

void CParaMng::OnNMClickTreePara(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

void CParaMng::OnNMDblclkTreePara(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CParaMng::OnTvnSelchangedTreePara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    HTREEITEM hTreeItem = m_treeParaMng.GetSelectedItem();   
    //��GetItemText()����Ϊ����   
    CString strText = m_treeParaMng.GetItemText(hTreeItem);
	//
	CRect dlgRect;
	GetClientRect(&dlgRect);
	//
	CRect mngRect;
	mngRect.left = dlgRect.Width()/5 + 20;
	mngRect.top = dlgRect.top;
	mngRect.right = dlgRect.right;
	mngRect.bottom = dlgRect.bottom;

	//�û�����
	if("����û�" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	else if("ɾ���û�" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	else if("�޸��û�" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	//ҵ�����
	if("�豸����" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);

	}
	else if("�������" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	else if("��Ƭ����" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	//������
	if("���ܱ��" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
	}
	else if("��ϸ���" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
	}
	// 
	*pResult = 0;
}

void CParaMng::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CParaMng::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlgCardMng.DoModal();
}
BOOL CParaMng::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
	//return CDialog::PreTranslateMessage(pMsg);
}
void CParaMng::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    MessageBox("�޸Ĳ����������������");
	CDialog::OnClose();
}
