// ParaMng.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "ParaMng.h"


// CParaMng 对话框

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


// CParaMng 消息处理程序
BOOL CParaMng::OnInitDialog()
{
	CDialog::OnInitDialog();
	//添加参数管理tree
	HTREEITEM hItem = m_treeParaMng.InsertItem("参数管理",NULL,NULL);///root就是节点的标题 
    //用户管理
	HTREEITEM hSubItem = m_treeParaMng.InsertItem("用户管理",NULL,NULL,hItem); 
	m_treeParaMng.InsertItem("添加用户",NULL,NULL,hSubItem); 
	//m_treeParaMng.InsertItem("删除用户",NULL,NULL,hSubItem);
	//m_treeParaMng.InsertItem("修改用户",NULL,NULL,hSubItem);
	//业务参数
    hSubItem = m_treeParaMng.InsertItem("参数设置",NULL,NULL,hItem); 
	m_treeParaMng.InsertItem("设备管理",NULL,NULL,hSubItem); 
	m_treeParaMng.InsertItem("区域管理",NULL,NULL,hSubItem);
	m_treeParaMng.InsertItem("卡片管理",NULL,NULL,hSubItem);
	//表格管理
    hSubItem = m_treeParaMng.InsertItem("表格管理",NULL,NULL,hItem); 
	//m_treeParaMng.InsertItem("汇总表格",NULL,NULL,hSubItem); 
	m_treeParaMng.InsertItem("明细表格",NULL,NULL,hSubItem);
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
	m_tabTab1.InsertItem(0, _T("参数管理"));
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
	// 根据调整好的mngRect放置m_dlgUserMng子对话框，并设置为显示   
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
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}

void CParaMng::OnNMDblclkTreePara(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CParaMng::OnTvnSelchangedTreePara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
    HTREEITEM hTreeItem = m_treeParaMng.GetSelectedItem();   
    //以GetItemText()函数为例：   
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

	//用户管理
	if("添加用户" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	else if("删除用户" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	else if("修改用户" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	//业务参数
	if("设备管理" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);

	}
	else if("区域管理" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	else if("卡片管理" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
	}
	//表格管理
	if("汇总表格" == strText)
	{
		m_dlgUserMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);   
		m_dlgDevMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgZoneMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgCardMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_HIDEWINDOW);
		m_dlgTblMng.SetWindowPos(NULL, mngRect.left, mngRect.top, mngRect.Width(), mngRect.Height(), SWP_SHOWWINDOW);
	}
	else if("明细表格" == strText)
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CParaMng::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlgCardMng.DoModal();
}
BOOL CParaMng::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
	//return CDialog::PreTranslateMessage(pMsg);
}
void CParaMng::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    MessageBox("修改参数后请重启软件！");
	CDialog::OnClose();
}
