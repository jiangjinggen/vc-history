// TunnelMonitorDisplay.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "TunnelMonitorDisplay.h"
#include "TcpCommSvrDlg.h"
#include <atlimage.h>
extern CTcpCommSvrDlg *g_pMainDlg;

// CTunnelMonitorDisplay 对话框

IMPLEMENT_DYNAMIC(CTunnelMonitorDisplay, CDialog)

CTunnelMonitorDisplay::CTunnelMonitorDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CTunnelMonitorDisplay::IDD, pParent)
{

}

CTunnelMonitorDisplay::~CTunnelMonitorDisplay()
{
}

void CTunnelMonitorDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAP, m_ctrlPic);
	DDX_Control(pDX, IDC_LIST_CARD_INFO, m_lstCardInfo);
	DDX_Control(pDX, IDC_LIST_CARD_INFO_X, m_lstCardInfo_x);
}


BEGIN_MESSAGE_MAP(CTunnelMonitorDisplay, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTunnelMonitorDisplay 消息处理程序
//
BOOL CTunnelMonitorDisplay::StartFreshDlg()
{
	while(m_bFresh)
	{
		PostMessage(WM_PAINT);
		Sleep(1000);
	}
	return TRUE;
}
UINT FreshDlg(LPVOID Ptr)
{
	CTunnelMonitorDisplay *pp = (CTunnelMonitorDisplay *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartFreshDlg();
	return 1;
}
//
BOOL CTunnelMonitorDisplay::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	m_curArea = 1;
	m_bDemo = FALSE;
	m_bList_x = TRUE;
	m_mapCardInfo.clear();
	m_mapInOutInfo.clear();
	//测试数据
	//m_mapCardInfo["1755605117"].readerNo = 1;
	//m_mapCardInfo["1755605117"].lstTime = time(NULL);
	//m_mapCardInfo["0019260306"].readerNo = 2;
	//m_mapCardInfo["0019260306"].lstTime = time(NULL);
	//m_mapCardInfo["3254563195"].readerNo = 3;
	//m_mapCardInfo["3254563195"].lstTime = time(NULL);
	//m_mapCardInfo["2942134076"].readerNo = 1;
	//m_mapCardInfo["2942134076"].lstTime = time(NULL);
	//////
	m_raMap.clear();
	m_csMap.clear();
	LoadReaderAreaMapping();
    LoadCardSignMapping();
	//
	SetTimer(1, 1000, NULL);
    //隐藏标题栏
    DWORD dwStyle = GetStyle();//获取旧样式  
    DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;  
    dwNewStyle&=dwStyle;//按位与将旧样式去掉  
    SetWindowLong(m_hWnd,GWL_STYLE,dwNewStyle);//设置成新的样式  
    DWORD dwExStyle = GetExStyle();//获取旧扩展样式  
    DWORD dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR;  
    dwNewExStyle&=dwExStyle;//按位与将旧扩展样式去掉  
    SetWindowLong(m_hWnd,GWL_EXSTYLE,dwNewExStyle);//设置新的扩展样式  
    SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！
	//
    ShowWindow(SW_MAXIMIZE);
    //
	int cx = GetSystemMetrics(SM_CXSCREEN);//屏幕像素宽度
	int cy = GetSystemMetrics(SM_CYSCREEN);//屏幕像素高度
	m_cx = cx;
	m_cy = cy;
	CRect picRect;
	picRect.left = 0;
	picRect.top = cy/4;
	picRect.right = cx;
	picRect.bottom = cy;
	m_ocx = 0;
	m_ocy = 0;
	GetDlgItem(IDC_STATIC_MAP)->MoveWindow(&picRect);
	//
	m_disW = m_cx;
	m_disH = picRect.bottom - picRect.top;
	//时钟
	CRect timeRect;
	timeRect.left = cx*5/6;
	timeRect.top = 0;
	timeRect.right = cx;
	timeRect.bottom = cy/18;
	GetDlgItem(IDC_STATIC_TIME)->MoveWindow(&timeRect);
	CFont timefont;
	timefont.CreatePointFont(200,"宋体");//
	GetDlgItem(IDC_STATIC_TIME)->SetFont(&timefont);
	DeleteObject(timefont);
	//
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TIME); // 得到 Picture Control 句柄
	//pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\time.bmp"), 
					IMAGE_BITMAP, 
					timeRect.Width(), 
					timeRect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	//进出显示区
	////CRect staticRect;
	////int x = 0;
	////int y = 0;
	////for(int i = 0; i < 9; i++)
	////{
	////	staticRect.left = cx*5*x/18;
	////	staticRect.top = cy*y/15;
	////	staticRect.right = staticRect.left + cx*5/18;
	////	staticRect.bottom = staticRect.top + cy/15;
	////	GetDlgItem(IDC_STATIC_1+i)->MoveWindow(&staticRect);
	////	GetDlgItem(IDC_STATIC_1+i)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	////	x += 1;
	////	if((i+1)%3 == 0)
	////	{
	////		x = 0;
	////		y += 1;
	////	}
	////}
	//
	CRect staticRect;
	staticRect.left = 0;
	staticRect.top = 0;
	staticRect.right = cx*5/6;
	staticRect.bottom = staticRect.top + cy/4 - cy/30 -cy/15;
	GetDlgItem(IDC_STATIC_1)->MoveWindow(&staticRect);
	GetDlgItem(IDC_STATIC_1)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	CString strTmp = "杭州隧道人员管理系统";
	GetDlgItem(IDC_STATIC_1)->SetWindowTextA(strTmp);
	//
	staticRect.left = 0;
	staticRect.top = cy/4 - cy/30 -cy/15;
	staticRect.right = cx*5/6;
	staticRect.bottom = staticRect.top + cy/15;
	GetDlgItem(IDC_STATIC_2)->MoveWindow(&staticRect);
	//GetDlgItem(IDC_STATIC_2)->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	//m_raMap
	CString strDisp = "汇总：";
	map<int,stuReaderAreaMapping>::iterator iter;
	for(iter = m_raMap.begin();iter != m_raMap.end(); iter++)
	{
		strTmp.Format("%s[%d]人 ",iter->second.areaName,iter->second.man_cnt);
		strDisp = strDisp + strTmp;
		break;
	}
	GetDlgItem(IDC_STATIC_2)->SetWindowTextA(strDisp);
	//
	//表格显示
	dwStyle = m_lstCardInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCardInfo.SetExtendedStyle(dwStyle);
	m_lstCardInfo.MoveWindow(picRect);
	CRect rect;
	m_lstCardInfo.GetClientRect(&rect); //获得当前客户区信息 
    m_lstCardInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/12);
    m_lstCardInfo.InsertColumn(1, _T("工号"), LVCFMT_LEFT, rect.Width()/6);
    m_lstCardInfo.InsertColumn(2, _T("姓名"), LVCFMT_LEFT, rect.Width()/6);
	m_lstCardInfo.InsertColumn(3, _T("工种"), LVCFMT_LEFT, rect.Width()/6);
	m_lstCardInfo.InsertColumn(4, _T("区域"), LVCFMT_LEFT, rect.Width()/6);
    m_lstCardInfo.InsertColumn(5, _T("时间"), LVCFMT_LEFT, rect.Width()/4); 
	m_lstCardInfo.ShowWindow(SW_HIDE);
	//m_lstCardInfo.SetBkColor(RGB(160,180,220)); 
	//m_lstCardInfo.SetTextBkColor(RGB(255,0,0));
	m_lstCardInfo.SetTextColor(RGB(255,0,0));
	CFont lstfont;
	lstfont.CreatePointFont(200,"宋体");//
	m_lstCardInfo.SetFont(&lstfont);
	DeleteObject(lstfont);
	//监控区
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP); // 得到 Picture Control 句柄
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
					IMAGE_BITMAP, 
					picRect.Width(), 
					picRect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	pWnd->ShowWindow(SW_HIDE);
	//
	GetDlgItem(IDC_STATIC_DEMO)->ShowWindow(SW_HIDE);
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_A);
	CRect titleRect;
	titleRect.left = cx/6 - cx/60;
	titleRect.top = cy/4 - cy/30;
	titleRect.right = titleRect.left + cx/30;
	titleRect.bottom = titleRect.top + cy/30;
	pWnd->MoveWindow(&titleRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	pWnd->ShowWindow(SW_HIDE);
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_B);
	titleRect.left = cx/2 - cx/60;
	titleRect.top = cy/4 - cy/30;
	titleRect.right = titleRect.left + cx/30;
	titleRect.bottom = titleRect.top + cy/30;
	pWnd->MoveWindow(&titleRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	pWnd->ShowWindow(SW_HIDE);
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_C);
	titleRect.left = cx*5/6 - cx/60;
	titleRect.top = cy/4 - cy/30;
	titleRect.right = titleRect.left + cx/30;
	titleRect.bottom = titleRect.top + cy/30;
	pWnd->MoveWindow(&titleRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	pWnd->ShowWindow(SW_HIDE);
	//表格显示，单段
	CRect picRect_x;
	picRect_x.left = 0;
	picRect_x.top = cy/4;
	picRect_x.right = cx/2;
	picRect_x.bottom = cy;
	dwStyle = m_lstCardInfo_x.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCardInfo_x.SetExtendedStyle(dwStyle);
	m_lstCardInfo_x.MoveWindow(picRect_x);
	//CRect rect;
	m_lstCardInfo_x.GetClientRect(&rect); //获得当前客户区信息 
    m_lstCardInfo_x.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, rect.Width()/4);
    m_lstCardInfo_x.InsertColumn(1, _T("工种"), LVCFMT_LEFT, rect.Width()/4);
    m_lstCardInfo_x.InsertColumn(2, _T("位置"), LVCFMT_LEFT, rect.Width()/4);
    m_lstCardInfo_x.InsertColumn(3, _T("时间"), LVCFMT_LEFT, rect.Width()/4); 
	//m_lstCardInfo_x.ShowWindow(SW_HIDE);
	//m_lstCardInfo.SetBkColor(RGB(160,180,220)); 
	//m_lstCardInfo.SetTextBkColor(RGB(255,0,0));
	m_lstCardInfo_x.SetTextColor(RGB(255,0,0));
	CFont lstfont_x;
	lstfont_x.CreatePointFont(200,"宋体");//
	m_lstCardInfo_x.SetFont(&lstfont_x);
	DeleteObject(lstfont_x);
	//监控区
	picRect_x.left = cx/2;
	picRect_x.right = cx;
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_X); // 得到 Picture Control 句柄
	pWnd->MoveWindow(&picRect_x);
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
					IMAGE_BITMAP, 
					picRect_x.Width(), 
					picRect_x.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	//
	//logo
	SetLogo();
    //目标半径
	m_dstR = m_disW/100;
	//文本宽度
	m_txtW = m_disW/30;
	//文本高度
	m_txtH = m_disH/40;
	//文本和目标距离
	m_dstTxtH = m_disH/100;
	//
	m_inOutIndex = 0;
    //刷新显示线程
	m_bFresh = TRUE;
	m_freshDlgThread = AfxBeginThread(FreshDlg,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_freshDlgThread->m_bAutoDelete = TRUE;
	m_freshDlgThread->ResumeThread();
	//
	return TRUE;
}

BOOL CTunnelMonitorDisplay::DestroyWindow() 
{
	m_bFresh = FALSE;
	g_pMainDlg->ShowWindow(SW_NORMAL);
	return CDialog::DestroyWindow();
}

BOOL CTunnelMonitorDisplay::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN)
	{
		// 组合键响应keydown消息
		if( pMsg->wParam == VK_SPACE&& (GetKeyState(VK_SHIFT)& 0x8000))

		{
		    // 空格 + Shift

		}

	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		// Alt组合键响应syskeydown消息
		if( pMsg->wParam == 'Q'&& (HIWORD(pMsg->lParam) & KF_ALTDOWN))

		{
		    // Q + Alt
			//PostMessage(WM_CLOSE);  //关闭
		}
		if( pMsg->wParam == 'Q'&& (HIWORD(pMsg->lParam) & KF_ALTDOWN))
		{
			//GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_HIDE);
		}
		if( pMsg->wParam == 'S'&& (HIWORD(pMsg->lParam) & KF_ALTDOWN))
		{
            //GetDlgItem(IDC_STATIC_MAP)->ShowWindow(TRUE);  
		}


	}
	if (pMsg->message == WM_KEYDOWN)     
	{         
		if (pMsg->wParam == 'F' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{
			//
			m_bList_x = FALSE;
			m_lstCardInfo_x.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAP_X)->ShowWindow(SW_HIDE);
			//
			m_bDemo = FALSE;
			m_bList = TRUE;
		    GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_HIDE);
			m_lstCardInfo.ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_HIDE);
			SetLogo();
		} 
		else if(pMsg->wParam == 'G' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)
		{
			//
			m_bList_x = FALSE;
			m_lstCardInfo_x.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAP_X)->ShowWindow(SW_HIDE);
			//
			m_bDemo = FALSE;
			m_bList = FALSE;
			m_lstCardInfo.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_NORMAL);
		}
		else if(pMsg->wParam == 'Q' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)
		{
			PostMessage(WM_CLOSE);  //关闭
		}
		else if (pMsg->wParam == 'D' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{             
			////m_bDemo = !m_bDemo;
			////m_lstCardInfo.ShowWindow(SW_HIDE);
			////GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_NORMAL);
			////if(!m_bDemo && m_bList)
			////{
			////	m_lstCardInfo.ShowWindow(SW_NORMAL);
			////	GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_HIDE);				
			////}
			////if(m_bDemo || m_bList)
			////{
			////	GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_HIDE);
			////	GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_HIDE);
			////	GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_HIDE);
			////}
			////else
			////{
			////	GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_NORMAL);
			////	GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_NORMAL);
			////	GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_NORMAL);
			////}
			////SetLogo();
		} 
		else if (pMsg->wParam == '1' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{ 
			//
			m_curArea = 1;
			m_bList_x = TRUE;
			m_lstCardInfo.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_HIDE);
			m_lstCardInfo_x.ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_MAP_X)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_HIDE);
			//
		}
		else if (pMsg->wParam == '2' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{ 
			//
			m_curArea = 2;
			m_bList_x = TRUE;
			m_lstCardInfo.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_HIDE);
			m_lstCardInfo_x.ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_MAP_X)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_HIDE);
			//
		}
		else if (pMsg->wParam == '3' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{ 
			//
			m_curArea = 3;
			m_bList_x = TRUE;
			m_lstCardInfo.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_MAP)->ShowWindow(SW_HIDE);
			m_lstCardInfo_x.ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_MAP_X)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_A)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_B)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_C)->ShowWindow(SW_HIDE);
			//
		}
	}
    return CDialog::PreTranslateMessage(pMsg);
}
void CTunnelMonitorDisplay::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//
	if(m_bDemo)
	{
		CRect rect;
		CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP); // 得到 Picture Control 句柄
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
		GetDlgItem(IDC_STATIC_MAP)->GetWindowRect(&rect);
		HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\mapDemo.bmp"), 
						IMAGE_BITMAP, 
						rect.Width(), 
						rect.Height(),
						LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		pWnd->SetBitmap(hPic);
		DeleteObject(hPic);
		//
		if(m_ocx > m_cx)m_ocx = 0;
		m_ocx += 30;
		m_ocy = 500;
		//
		CDC* pControlDC; //表示控件窗口的设备描述表.DC 
		pControlDC = pWnd->GetDC();
		CBrush brush, *oldbrush; 
		brush.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush = pControlDC->SelectObject(&brush);
		pControlDC->Ellipse(m_ocx, m_ocy, m_ocx+m_dstR, m_ocy+m_dstR);
		pControlDC->SelectObject(oldbrush);
		DeleteObject(brush);
		//输出文本
		CRect TextRect;
		TextRect.left = m_ocx - m_dstR;
		TextRect.right = TextRect.left + m_txtW;
		TextRect.top = m_ocy - m_dstR - m_dstTxtH;
		TextRect.bottom = TextRect.top + m_txtH;
		CBrush brush1, *oldbrush1;
		brush1.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush1 = pControlDC->SelectObject(&brush1);
		CString nameStr = "    张三    ";
		pControlDC->SetTextColor(RGB(0,0,255));
		pControlDC->DrawText(nameStr, nameStr.GetLength(), &TextRect, DT_CENTER);
		pControlDC->SelectObject(oldbrush1);
		DeleteObject(brush1);
	}
	else
	{
		CRect rect;
		CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP); // 得到 Picture Control 句柄
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
		GetDlgItem(IDC_STATIC_MAP)->GetWindowRect(&rect);
		HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
						IMAGE_BITMAP, 
						rect.Width(), 
						rect.Height(),
						LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		pWnd->SetBitmap(hPic);
		DeleteObject(hPic);
		//
		CDC* pControlDC; //表示控件窗口的设备描述表.DC 
		pControlDC = pWnd->GetDC();
		CBrush brush, *oldbrush; 
		brush.CreateSolidBrush(RGB(0, 0, 255)); 
		CPen pen(PS_SOLID,1,RGB(255, 0, 0)), *oldpen;
		oldbrush = pControlDC->SelectObject(&brush);
		oldpen = pControlDC->SelectObject(&pen);
		pControlDC->MoveTo(m_disW/3,0);
		pControlDC->LineTo(m_disW/3,m_cy);
		pControlDC->MoveTo(m_disW*2/3,0);
		pControlDC->LineTo(m_disW*2/3,m_cy);
		pControlDC->SelectObject(oldbrush);
		pControlDC->SelectObject(oldpen);
		DeleteObject(brush);
		//
		DisplayMonitorInfo();
		//
		pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_X); // 得到 Picture Control 句柄
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
		GetDlgItem(IDC_STATIC_MAP)->GetWindowRect(&rect);
		hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
						IMAGE_BITMAP, 
						rect.Width(), 
						rect.Height(),
						LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		pWnd->SetBitmap(hPic);
		DeleteObject(hPic);
		//
		DisplayMonitorInfoX();
		//
	}

}
void CTunnelMonitorDisplay::DisplayMonitorInfo()
{
	m_lstCardInfo.DeleteAllItems();//清空
	//
	map<int,stuReaderAreaMapping>::iterator iter_ra;
	for(iter_ra = m_raMap.begin();iter_ra != m_raMap.end(); iter_ra++)
	{
		iter_ra->second.man_cnt = 0;
	}
	//
	char stmp[256];
	int iCount = 0;
	CString strCardNo;
	time_t timeNow = time(NULL);
	map<string,stuDisCardInfo>::iterator iter;
	for(iter = m_mapCardInfo.begin();iter != m_mapCardInfo.end();iter++)
	{
		//查找卡和区域数据
		map<int,stuReaderAreaMapping>::iterator iterA;
		iterA = m_raMap.find(iter->second.areaNo);
		if(iterA != m_raMap.end())
		{
		}
		else
		{
			continue;
		}
		map<string,stuCardSignMapping>::iterator iterC;
		iterC = m_csMap.find(iter->first);
		if(iterC != m_csMap.end())
		{
		}
		else
		{
			continue;
		}
		//
		iterA->second.man_cnt += 1;
		//
		if(timeNow - iter->second.lstTime > CARD_TIME_OUT)
		{
			map<string,stuDisInOutInfo>::iterator iterIO;
			iterIO = m_mapInOutInfo.find(iter->first);
			if(iterIO != m_mapInOutInfo.end())
			{
				if(STATUS_OUT == iterIO->second.ioFlg)
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		//
		iCount++;
		//序号
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_lstCardInfo.InsertItem(iCount-1, _T(stmp));
		//工号
		m_lstCardInfo.SetItemText(iCount-1, 1, _T(iterC->second.signNo));
		//姓名
		m_lstCardInfo.SetItemText(iCount-1, 2, _T(iterC->second.signName));
		//工种
		m_lstCardInfo.SetItemText(iCount-1, 3, _T(iterC->second.workType));
        //区域名称
		m_lstCardInfo.SetItemText(iCount-1, 4, _T(iterA->second.areaName));
		//时间
		struct tm *now = NULL;
		now = localtime(&iter->second.lstTime);
		sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
		m_lstCardInfo.SetItemText(iCount-1, 5, _T(stmp));
		//
		int iArea = iterA->second.areaNo - 1;
		unsigned int u;
		u = strtoul(iter->first.c_str(), NULL, 10);
		srand(u); //(unsigned) GetTickCount()用时间做种，每次产生随机数不一样
		int number = rand();  //产生随机数
		//显示
		CRect rect;
		CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP); // 
		pWnd->GetWindowRect(&rect);
		int x = number%(rect.right/3) + rect.right*iArea/3;
		int y = number%(rect.bottom - rect.top);
		CDC* pControlDC; //表示控件窗口的设备描述表.DC 
		pControlDC = pWnd->GetDC();
		CBrush brush, *oldbrush; 
		brush.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush = pControlDC->SelectObject(&brush);
		pControlDC->Ellipse(x, y, x+m_dstR, y+m_dstR);
		pControlDC->SelectObject(oldbrush);
		DeleteObject(brush);
		//输出文本
		CRect TextRect;
		TextRect.left = x - m_dstR;
		TextRect.right = TextRect.left + m_txtW;
		TextRect.top = y - m_dstR - m_dstTxtH;
		TextRect.bottom = TextRect.top + m_txtH;
		CBrush brush1, *oldbrush1;
		brush1.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush1 = pControlDC->SelectObject(&brush1);
		CString nameStr;
		nameStr.Format("    %s    ",iterC->second.signName);
		pControlDC->SetTextColor(RGB(0,0,255));
		pControlDC->DrawText(nameStr, nameStr.GetLength(), &TextRect, DT_CENTER);
		pControlDC->SelectObject(oldbrush1);
		DeleteObject(brush1);
		//
	}
	if(FALSE == m_bList_x)
	{
		CString strTmp = "";
		CString strDisp = "汇总：";
		map<int,stuReaderAreaMapping>::iterator iter_ra;
		for(iter_ra = m_raMap.begin();iter_ra != m_raMap.end(); iter_ra++)
		{
			strTmp.Format("%s[%d]人 ",iter_ra->second.areaName,iter_ra->second.man_cnt);
			strDisp = strDisp + strTmp;
		}
		GetDlgItem(IDC_STATIC_2)->SetWindowTextA(strDisp);
		//
	}
	//不显示进出信息
	////DisplayInOutInfo();
}
void CTunnelMonitorDisplay::DisplayInOutInfo()
{
	//显示进出，void CTcpCommSvrDlg::GetAllInOutInfo(vector<stuInOutInfo> &vecInOutInfo)
	vector<stuInOutInfo> vecInOutInfo;
	g_pMainDlg->GetAllInOutInfo(vecInOutInfo);
	//
	////stuInOutInfo inOutInfo;
	////memset(&inOutInfo,0,sizeof(stuInOutInfo));
	////strcpy(inOutInfo.card_no,"1755605117");
	////strcpy(inOutInfo.io_time,"20170117 0926");
	////strcpy(inOutInfo.sflag,"0");
	////vecInOutInfo.push_back(inOutInfo);
	//////
	////memset(&inOutInfo,0,sizeof(stuInOutInfo));
	////strcpy(inOutInfo.card_no,"0019260306");
	////strcpy(inOutInfo.io_time,"20170117 0926");
	////strcpy(inOutInfo.sflag,"1");
	////vecInOutInfo.push_back(inOutInfo);
	//////
	////memset(&inOutInfo,0,sizeof(stuInOutInfo));
	////strcpy(inOutInfo.card_no,"3254563195");
	////strcpy(inOutInfo.io_time,"20170117 0926");
	////strcpy(inOutInfo.sflag,"0");
	////vecInOutInfo.push_back(inOutInfo);
	//////
	////memset(&inOutInfo,0,sizeof(stuInOutInfo));
	////strcpy(inOutInfo.card_no,"2942134076");
	////strcpy(inOutInfo.io_time,"20170117 0926");
	////strcpy(inOutInfo.sflag,"1");
	////vecInOutInfo.push_back(inOutInfo);
	//
	vector<stuInOutInfo>::iterator iterVec;
	for(iterVec = vecInOutInfo.begin(); iterVec != vecInOutInfo.end(); iterVec++)
	{
		string strCardNo = iterVec->card_no;
		m_mapInOutInfo[strCardNo].ioFlg = atoi(iterVec->sflag);
		strcpy(m_mapInOutInfo[strCardNo].io_time,iterVec->io_time);
		m_mapInOutInfo[strCardNo].lstTime = time(NULL);
		//显示
		CString strTmp;
		map<string,stuCardSignMapping>::iterator iterC;
		iterC = m_csMap.find(strCardNo);
		if(iterC != m_csMap.end())
		{
		}
		else
		{
			continue;
		}
		if(STATUS_IN == m_mapInOutInfo[strCardNo].ioFlg)
		{
		    strTmp.Format("[%s][%s][%s]进隧道",iterC->second.signNo,iterC->second.signName,m_mapInOutInfo[strCardNo].io_time);
		}
		else
		{
			strTmp.Format("[%s][%s][%s]出隧道",iterC->second.signNo,iterC->second.signName,m_mapInOutInfo[strCardNo].io_time);
		}
		//
		bool bTmp = false;
		for(int i = IDC_STATIC_1;i <= IDC_STATIC_9;i++)
		{
			CString strOldTmp;
			CString strSN;
			GetDlgItem(i)->GetWindowTextA(strOldTmp);
			if(strOldTmp.Trim() == "")continue;
			int iStart = 0;
			int iEnd = 0;
			iStart =strOldTmp.Find('[');
			iEnd = strOldTmp.Find(']');
			CString strOldNo = strOldTmp.Mid(iStart+1,iEnd - iStart-1);
			strOldTmp = strOldTmp.Mid(iEnd+1);
			iStart =strOldTmp.Find('[');
			iEnd = strOldTmp.Find(']');
			CString strOldName = strOldTmp.Mid(iStart+1,iEnd - iStart-1);
			strOldTmp = strOldTmp.Mid(iEnd+1);
			iStart =strOldTmp.Find('[');
			iEnd = strOldTmp.Find(']');
			CString strOldTime = strOldTmp.Mid(iStart+1,iEnd - iStart-1);
			strOldTmp = strOldTmp.Mid(iEnd+1);
			CString strNo = iterC->second.signNo;
			CString strFlg = iterVec->sflag;
			CString strTime = iterVec->io_time;
			int iOldFlg = STATUS_IN;
			if("出隧道" == strOldTmp)
			{
				iOldFlg = STATUS_OUT;
			}
			if((strNo == strOldNo)&&(atoi(iterVec->sflag) == iOldFlg))
			{
				if(strTime > strOldTime)
				{
					GetDlgItem(i)->SetWindowTextA(strTmp);
				}
				bTmp = true;
				break;
			}
		}
		//
		if(false == bTmp)
		{
			GetDlgItem(IDC_STATIC_1+m_inOutIndex)->SetWindowTextA(strTmp);
			//
			m_inOutIndex += 1;
		}
		if(m_inOutIndex >= 9)
		{
			m_inOutIndex = 0;
		}
	}
	//m_mapInOutInfo
	////map<string,stuDisInOutInfo>::iterator iterMap;
	////int iCount = 0;
	////time_t timeNow = time(NULL);
	////for(iterMap = m_mapInOutInfo.begin(); iterMap != m_mapInOutInfo.end(); iterMap++)
	////{
	////	if(timeNow - iterMap->second.lstTime < IN_OUT_TIME_OUT)
	////	{
	////		//显示
	////		CString strTmp;
	////		map<string,stuCardSignMapping>::iterator iterC;
	////		iterC = m_csMap.find(iterMap->first);
	////		if(iterC != m_csMap.end())
	////		{
	////		}
	////		else
	////		{
	////			continue;
	////		}
	////		if(STATUS_IN == iterMap->second.ioFlg)
	////		{
	////		    strTmp.Format("[%s][%s][%s]进隧道",iterC->second.signNo,iterC->second.signName,iterMap->second.io_time);
	////		}
	////		else
	////		{
	////			strTmp.Format("[%s][%s][%s]出隧道",iterC->second.signNo,iterC->second.signName,iterMap->second.io_time);
	////		}
	////		//
	////		GetDlgItem(IDC_STATIC_1+iCount)->SetWindowTextA(strTmp);
	////		//
	////		iCount += 1;
	////	}
	////	if(iCount >= 9)
	////	{
	////		break;
	////	}

	////}
	//
}
void CTunnelMonitorDisplay::DisplayMonitorInfoX()
{
	m_lstCardInfo_x.DeleteAllItems();//清空
	//
	int cur_area = m_curArea;
	//
	int iNum = 0;
	map<int,stuReaderAreaMapping>::iterator iter_ra;
	for(iter_ra = m_raMap.begin();iter_ra != m_raMap.end(); iter_ra++)
	{
		iNum += 1;
		if(iNum == cur_area)
		{
			break;
		}
	}
	if(iter_ra == m_raMap.end())return;
	//
	char stmp[256];
	int iCount = 0;
	CString strCardNo;
	time_t timeNow = time(NULL);
	map<string,stuDisCardInfo>::iterator iter;
	for(iter = m_mapCardInfo.begin();iter != m_mapCardInfo.end();iter++)
	{
		//查找卡和区域数据
		map<int,stuReaderAreaMapping>::iterator iterA;
		iterA = m_raMap.find(iter->second.areaNo);
		if(iterA != m_raMap.end())
		{
		}
		else
		{
			continue;
		}
		map<string,stuCardSignMapping>::iterator iterC;
		iterC = m_csMap.find(iter->first);
		if(iterC != m_csMap.end())
		{
		}
		else
		{
			continue;
		}
		//
		if(iter->second.areaNo != iter_ra->second.areaNo)continue;
		//
		iCount++;
		//姓名
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_lstCardInfo_x.InsertItem(iCount-1, _T(iterC->second.signName));
		//工种
		m_lstCardInfo_x.SetItemText(iCount-1, 1, _T(iterC->second.workType));
		//位置
		m_lstCardInfo_x.SetItemText(iCount-1, 2, _T(iterA->second.areaName));
		//时间
		struct tm *now = NULL;
		now = localtime(&iter->second.lstTime);
		sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
		m_lstCardInfo_x.SetItemText(iCount-1, 3, _T(stmp));
		//
		int iArea = cur_area;
		unsigned int u;
		u = strtoul(iter->first.c_str(), NULL, 10);
		srand(u); //(unsigned) GetTickCount()用时间做种，每次产生随机数不一样
		int number = rand();  //产生随机数
		//显示
		CRect rect;
		CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_X); // 
		pWnd->GetWindowRect(&rect);
		int x = number%(rect.Width());
		int y = number%(rect.Height());
		CDC* pControlDC; //表示控件窗口的设备描述表.DC 
		pControlDC = pWnd->GetDC();
		CBrush brush, *oldbrush; 
		brush.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush = pControlDC->SelectObject(&brush);
		pControlDC->Ellipse(x, y, x+m_dstR, y+m_dstR);
		pControlDC->SelectObject(oldbrush);
		DeleteObject(brush);
		//输出文本
		CRect TextRect;
		TextRect.left = x - m_dstR;
		TextRect.right = TextRect.left + m_txtW;
		TextRect.top = y - m_dstR - m_dstTxtH;
		TextRect.bottom = TextRect.top + m_txtH;
		CBrush brush1, *oldbrush1;
		brush1.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush1 = pControlDC->SelectObject(&brush1);
		CString nameStr;
		nameStr.Format("    %s    ",iterC->second.signName);
		pControlDC->SetTextColor(RGB(0,0,255));
		pControlDC->DrawText(nameStr, nameStr.GetLength(), &TextRect, DT_CENTER);
		pControlDC->SelectObject(oldbrush1);
		DeleteObject(brush1);
		//
	}
	if(TRUE == m_bList_x)
	{
		CString strTmp = "";
		CString strDisp = "汇总：";
		strTmp.Format("%s[%d]人 ",iter_ra->second.areaName,iter_ra->second.man_cnt);
		strDisp = strDisp + strTmp;
		GetDlgItem(IDC_STATIC_2)->SetWindowTextA(strDisp);
		//
	}
	//不显示进出信息
	////DisplayInOutInfo();
}
void CTunnelMonitorDisplay::SetCardToDisplayData(unsigned int cardno,unsigned char readerno,time_t lsttime)
{
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,"%010u",cardno);
	string strCardNo = stmp;
	//map<int,stuReaderAreaMapping> m_raMap
	map<int,stuReaderAreaMapping>::iterator iter_ra;
	iter_ra = m_raMap.find(readerno);
	if(m_raMap.end() == iter_ra)return;
	//map<string,stuCardSignMapping> m_csMap;
	map<string,stuCardSignMapping>::iterator iter_cs;
	iter_cs = m_csMap.find(strCardNo);
	if(m_csMap.end() == iter_cs)return; 
	m_mapCardInfo[strCardNo].areaNo = iter_ra->second.areaNo;
	m_mapCardInfo[strCardNo].lstTime = lsttime;
}
void CTunnelMonitorDisplay::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTime t = CTime::GetCurrentTime(); 
	CString strTime = t.Format(_T( "%Y-%m-%d %H:%M:%S"));
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TIME);
	CFont timefont;
	timefont.CreatePointFont(200,"宋体");//
	pWnd->SetFont(&timefont);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	pWnd->SetWindowTextA(strTime);
    
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CTunnelMonitorDisplay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIME)
	{
		//pDC->SetBkColor(RGB(0,255,0));//背景色为绿色
		pDC->SetTextColor(RGB(0, 0, 255));//文字为蓝色
		//pDC->SelectObject(&m_font);//文字为15号字体，华文行楷
		//return m_brush;
	}
	else if(pWnd->GetDlgCtrlID() == IDC_STATIC_A || pWnd->GetDlgCtrlID() == IDC_STATIC_B || pWnd->GetDlgCtrlID() == IDC_STATIC_C)
	{
		pDC->SetTextColor(RGB(227,23,13));//
		CFont titlefont;
		titlefont.CreatePointFont(200,"宋体");//
		pDC->SelectObject(&titlefont);//
	}
	else if(pWnd->GetDlgCtrlID() == IDC_STATIC_1)
	{
		pDC->SetTextColor(RGB(255,0,0));//
		CFont titlefont;
		titlefont.CreatePointFont(500,"宋体");//
		pDC->SelectObject(&titlefont);//
	}
	else if(pWnd->GetDlgCtrlID() >= IDC_STATIC_2 && pWnd->GetDlgCtrlID() <= IDC_STATIC_9)
	{
		pDC->SetTextColor(RGB(0,0,255));//
		CFont titlefont;
		titlefont.CreatePointFont(300,"宋体");//
		pDC->SelectObject(&titlefont);//
	}
	else
	{
		//pDC->SetBkColor(RGB(0,0,0));//背景色为绿色
		//pDC->SetTextColor(RGB(255, 0, 0));//文字为红色
		//pDC->SelectObject(&m_font1);//
		//return m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//加载读卡器与区域映射关系
BOOL CTunnelMonitorDisplay::LoadReaderAreaMapping()
{
	CString filePath = _T(".\\cfg\\devcfg.ini");
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;

	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	int section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	for(int sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("读卡器和区域映射关系" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		pos_ini.Next();	
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 3)
			{
				continue;
			}
			stuReaderAreaMapping readerAreaMapping;
			memset(&readerAreaMapping,0,sizeof(stuReaderAreaMapping));
			readerAreaMapping.readerNo = atoi(vStr[0].c_str());
			int len = strlen(vStr[1].c_str());
			if(len > 9) len = 9;
			//memcpy(readerAreaMapping.areaNo,vStr[1].c_str(),len);
			readerAreaMapping.areaNo = atoi(vStr[1].c_str());
			len = strlen(vStr[2].c_str());
			if(len > 255) len = 255;
			memcpy(readerAreaMapping.areaName,vStr[2].c_str(),len);
			//
			readerAreaMapping.man_cnt = 0;
			//
			memcpy(&m_raMap[readerAreaMapping.readerNo],&readerAreaMapping,sizeof(stuReaderAreaMapping));
			//
		}
	}
	return TRUE;
}
//加载卡片与餐牌的映射关系
BOOL CTunnelMonitorDisplay::LoadCardSignMapping()
{
	CString filePath = _T(".\\cfg\\devcfg.ini");
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;

	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	int section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	for(int sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("卡号和工人映射关系" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		pos_ini.Next();	
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 4)
			{
				continue;
			}
			stuCardSignMapping cardSignMapping;
			memset(&cardSignMapping,0,sizeof(stuCardSignMapping));
			int len = strlen(vStr[0].c_str());
			if(len > 19) len = 19;
			memcpy(cardSignMapping.cardNo,vStr[0].c_str(),len);
			len = strlen(vStr[1].c_str());
			if(len > 19) len = 19;
			memcpy(cardSignMapping.signNo,vStr[1].c_str(),len);
			len = strlen(vStr[2].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.signName,vStr[2].c_str(),len);
			len = strlen(vStr[3].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.workType,vStr[3].c_str(),len);
			//
			string tmpstr = cardSignMapping.cardNo;
			memcpy(&m_csMap[tmpstr],&cardSignMapping,sizeof(stuCardSignMapping));
			//
		}
	}
	return TRUE;
}

void CTunnelMonitorDisplay::SetLogo()
{
	//logo
	CRect logoRect;
	logoRect.left = m_cx*5/6;
	logoRect.top = m_cy/18;
	logoRect.right = m_cx;
	logoRect.bottom = m_cy/4;
	GetDlgItem(IDC_STATIC_LOGO)->MoveWindow(&logoRect);
	//
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_LOGO); // 得到 Picture Control 句柄
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\logo.bmp"), 
					IMAGE_BITMAP, 
					logoRect.Width(), 
					logoRect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
}