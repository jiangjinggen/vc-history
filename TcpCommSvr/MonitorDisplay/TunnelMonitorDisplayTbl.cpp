// TunnelMonitorDisplayTbl.cpp : 实现文件
//

#include "stdafx.h"
#include "MonitorDisplay.h"
#include "TunnelMonitorDisplayTbl.h"
#include "io.h"
// CTunnelMonitorDisplayTbl 对话框

IMPLEMENT_DYNAMIC(CTunnelMonitorDisplayTbl, CDialog)
// 定义导出接口
CTunnelMonitorDisplayTbl *g_ph = NULL;
extern "C" __declspec(dllexport) void ShowDialog(string station_id)
{
	// 切记，此处需要加下面这行代码，不然对话框显示不出来。
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(NULL != g_ph)
	{
		delete g_ph;
		g_ph = NULL;
	}
	g_ph = new CTunnelMonitorDisplayTbl;
	g_ph->SetStationId(station_id);
	//ph->Create(CTunnelMonitorDisplayTbl::IDD);
	//ph->ShowWindow(SW_SHOW);
	g_ph->DoModal();
	return ;
}
extern "C" __declspec(dllexport) void SetData(unsigned int cardno,unsigned char readerno,time_t lsttime,int data_len,const unsigned char *data)
{
	// 切记，此处需要加下面这行代码，不然对话框显示不出来。
	if(NULL != g_ph)
	{
		g_ph->SetCardToDisplayData(cardno,readerno,lsttime,data_len,data);
	}
}
//
CTunnelMonitorDisplayTbl::CTunnelMonitorDisplayTbl(CWnd* pParent /*=NULL*/)
	: CDialog(CTunnelMonitorDisplayTbl::IDD, pParent)
{

}

CTunnelMonitorDisplayTbl::~CTunnelMonitorDisplayTbl()
{
}

void CTunnelMonitorDisplayTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTunnelMonitorDisplayTbl, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTunnelMonitorDisplayTbl 消息处理程序
//
BOOL CTunnelMonitorDisplayTbl::StartFreshDlg()
{
    int iNum = 0;
	while(m_bFresh)
	{
		//if(WAIT_OBJECT_0==WaitForSingleObject(m_hTblFreshEvent,INFINITE))
		if(WAIT_OBJECT_0==WaitForSingleObject(m_hTblFreshEvent,1000))
		{
			//Sleep(100);
			if(m_bFresh)
			{
			    PostMessage(WM_PAINT);
			}
			//Sleep(1000);
		}
		else
		{
			if(m_bFresh)
			{
			    PostMessage(WM_PAINT);
			}
		}
	}
	return TRUE;
}
UINT FreshTblDlg(LPVOID Ptr)
{
	CTunnelMonitorDisplayTbl *pp = (CTunnelMonitorDisplayTbl *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartFreshDlg();
	return 1;
}
//
BOOL CTunnelMonitorDisplayTbl::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	if("000" == m_stationId)
	{
		m_strPathHead = ".";
	}
	else
	{
	    m_strPathHead.Format(".\\cfg\\%s",m_stationId.c_str());
	}
	//
	m_posMap.clear();
	m_curTotPos1 = 0;
	m_curTotPos2 = 0;
	m_leftPos = 0;
	m_rightPos = 0;
	int cx = GetSystemMetrics(SM_CXSCREEN);//屏幕像素宽度
	int cy = GetSystemMetrics(SM_CYSCREEN);//屏幕像素高度
	m_cx = cx;
	m_cy = cy;
	//
	m_ocx = 0;
	m_ocy = 0;
	//
	m_disW = m_cx;
    m_disH = m_cy*17/24;
    //目标半径
	m_dstR = m_disW/100;
	//文本宽度
	m_txtW = m_disW/20;
	//文本高度
	m_txtH = m_disH/40;
	//文本和目标距离
	m_dstTxtH = m_disH/100;
	//
	m_tClrCnt = 0;
	//
	m_mapCardInfo.clear();
	//
	//if(m_freshTime <= 0)m_freshTime = 10;
	//if(m_freshTime >= 60)m_freshTime = 10;
	m_freshTime = 10;
	//
	m_shiftType = KEY_F;
	m_shiftTime = 30;
	m_disLoop = true;
	//
	m_raMap.clear();
	m_totAreaMap.clear();
	m_csMap.clear();
	LoadTblTitleMapping();
	LoadReaderAreaMapping();
	LoadAreaTotMapping();
    LoadCardSignMapping();
	LoadPicOrigin();
	LoadZoneAxisMapping();
	//
	m_mapCardFstTime.clear();
	//LoadCardFstTime();
	//
	m_mapAttData.clear();
	//
	CString strPath = m_strPathHead;
	strPath += "\\cfg\\syscfg.ini";
	m_displayType = IS_HZSD;
	m_displayType = GetPrivateProfileInt("主界面显示类型", "display_type",0,strPath.GetBuffer());
	if(m_displayType <= 0)m_displayType = IS_HZSD;
	//测试数据
	////SetCardToDisplayData(1755605117,1,time(NULL));
	////SetCardToDisplayData(19260306,2,time(NULL));
	////SetCardToDisplayData(3254563195,3,time(NULL));
	////SetCardToDisplayData(2387869110,1,time(NULL));
	//////
	////SetCardToDisplayData(2387869111,1,time(NULL));
	////SetCardToDisplayData(2387869112,1,time(NULL));
	////SetCardToDisplayData(2387869113,1,time(NULL));
	////SetCardToDisplayData(2387869114,1,time(NULL));
	////SetCardToDisplayData(2387869115,1,time(NULL));
	////SetCardToDisplayData(2387869116,1,time(NULL));
	////SetCardToDisplayData(2387869117,1,time(NULL));
	////SetCardToDisplayData(2387869118,1,time(NULL));
	////SetCardToDisplayData(2387869119,1,time(NULL));
	////SetCardToDisplayData(2387869120,1,time(NULL));
 ////   //
	////SetCardToDisplayData(1755605127,4,time(NULL));
	////SetCardToDisplayData(19260316,5,time(NULL));
	////SetCardToDisplayData(3254563185,6,time(NULL));
	////SetCardToDisplayData(2387869120,4,time(NULL));
	//////
	////SetCardToDisplayData(2387869121,4,time(NULL));
	////SetCardToDisplayData(2387869122,4,time(NULL));
	////SetCardToDisplayData(2387869123,4,time(NULL));
	////SetCardToDisplayData(2387869124,4,time(NULL));
	////SetCardToDisplayData(2387869125,4,time(NULL));
	////SetCardToDisplayData(2387869126,4,time(NULL));
	////SetCardToDisplayData(2387869127,4,time(NULL));
	////SetCardToDisplayData(2387869128,4,time(NULL));
	////SetCardToDisplayData(2387869129,4,time(NULL));
	////SetCardToDisplayData(2387869130,4,time(NULL));
	//
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
	m_bMode1 = TRUE;
	m_bMode2 = FALSE;
	m_bMode3 = FALSE;
	//m_bMode4 = FALSE;
	//
	GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_STATIC_MAP_A)->ShowWindow(SW_SHOW);
	//
	GetDlgItem(IDC_STATIC_TOT_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TOT_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TOT_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TOT_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TOT_A)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TOT_B)->ShowWindow(SW_HIDE);
    //
	GetDlgItem(IDC_STATIC_TITLE_M4_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TITLE_M4_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CONTENT_M4_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CONTENT_M4_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_LOGO_M4_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TAIL_M4_1)->ShowWindow(SW_HIDE);
	//
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowTextA(_T(""));
    GetDlgItem(IDC_STATIC_MAP_A)->SetWindowTextA(_T(""));
	//
	GetDlgItem(IDC_STATIC_TOT_1)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TOT_2)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TOT_3)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TOT_4)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TOT_A)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TOT_B)->SetWindowTextA(_T(""));
    //
	GetDlgItem(IDC_STATIC_TITLE_M4_1)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TITLE_M4_2)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_CONTENT_M4_1)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_CONTENT_M4_2)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_LOGO_M4_1)->SetWindowTextA(_T(""));
	GetDlgItem(IDC_STATIC_TAIL_M4_1)->SetWindowTextA(_T(""));
	//
	CRect picRect;
	picRect.left = 0;
	picRect.top = 0;
	picRect.right = cx;
	picRect.bottom = cy;
	//监控区
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_A); // 得到 Picture Control 句柄
	//pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	//pWnd->MoveWindow(&picRect)
	//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
	//				IMAGE_BITMAP, 
	//				picRect.Width(), 
	//				picRect.Height(),
	//				LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//pWnd->SetBitmap(hPic);
	//DeleteObject(hPic);
	//pWnd->ShowWindow(SW_HIDE);
	//
	SetTitleText();
	//
	SetTblM4TitleText();
	SetTitleLogo();
	SetTimer(1, 1000, NULL);
	SetTimer(2, 1000, NULL);
	SetTimer(3, m_shiftTime*1000, NULL);
    //刷新显示线程
	m_hTblFreshEvent = CreateEvent(NULL,FALSE,FALSE,NULL);//
	m_bFresh = TRUE;
	m_freshDlgThread = AfxBeginThread(FreshTblDlg,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_freshDlgThread->m_bAutoDelete = TRUE;
	m_freshDlgThread->ResumeThread();
	//
	m_bUpFresh = true;
	//
	return TRUE;
}
//
void CTunnelMonitorDisplayTbl::SetData(int n) 
{
	//m_nNum = n;
}
//
BOOL CTunnelMonitorDisplayTbl::DestroyWindow() 
{
	m_bFresh = FALSE;
	//////g_pMainDlg->ShowWindow(SW_NORMAL);
	return CDialog::DestroyWindow();
}

BOOL CTunnelMonitorDisplayTbl::PreTranslateMessage(MSG* pMsg) 
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
		if (pMsg->wParam == 'D' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{
			//
			m_bMode1 = TRUE;
			m_bMode2 = FALSE;
			m_bMode3 = FALSE;
			//
			GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_SHOW);
            GetDlgItem(IDC_STATIC_MAP_A)->ShowWindow(SW_SHOW);
			//
			GetDlgItem(IDC_STATIC_TOT_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_A)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_B)->ShowWindow(SW_HIDE);
			//
			GetDlgItem(IDC_STATIC_TITLE_M4_1)->ShowWindow(SW_HIDE); 
			GetDlgItem(IDC_STATIC_TITLE_M4_2)->ShowWindow(SW_HIDE); 
			GetDlgItem(IDC_STATIC_CONTENT_M4_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CONTENT_M4_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_LOGO_M4_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TAIL_M4_1)->ShowWindow(SW_HIDE);
			SetEvent(m_hTblFreshEvent);
			SetTitleLogo();
		}
		else if (pMsg->wParam == 'F' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)         
		{
			//
			m_bMode1 = FALSE;
			m_bMode2 = TRUE;
			m_bMode3 = FALSE;
			//
			GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_MAP_A)->ShowWindow(SW_HIDE);
			//
			GetDlgItem(IDC_STATIC_TOT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_TOT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_TOT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_TOT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_TOT_A)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_TOT_B)->ShowWindow(SW_SHOW);
			//
			GetDlgItem(IDC_STATIC_TITLE_M4_1)->ShowWindow(SW_HIDE); 
			GetDlgItem(IDC_STATIC_TITLE_M4_2)->ShowWindow(SW_HIDE); 
			GetDlgItem(IDC_STATIC_CONTENT_M4_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_CONTENT_M4_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_LOGO_M4_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TAIL_M4_1)->ShowWindow(SW_HIDE);
			SetEvent(m_hTblFreshEvent);
			SetTitleLogo();

		} 
		else if(pMsg->wParam == 'G' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)
		{
			//
			m_bMode1 = FALSE;
			m_bMode2 = FALSE;
			m_bMode3 = TRUE;
			GetDlgItem(IDC_STATIC_TITLE)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_STATIC_MAP_A)->ShowWindow(SW_HIDE);
			//
			GetDlgItem(IDC_STATIC_TOT_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_4)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_A)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TOT_B)->ShowWindow(SW_HIDE);
			//
			GetDlgItem(IDC_STATIC_TITLE_M4_1)->ShowWindow(SW_SHOW); 
			GetDlgItem(IDC_STATIC_TITLE_M4_2)->ShowWindow(SW_SHOW); 
			GetDlgItem(IDC_STATIC_CONTENT_M4_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_CONTENT_M4_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_LOGO_M4_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_TAIL_M4_1)->ShowWindow(SW_SHOW); 
			SetM4Display();
			SetEvent(m_hTblFreshEvent);
			SetTitleLogo();

		}
		else if(pMsg->wParam == 'O' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)
		{
			m_disLoop = !m_disLoop;
		}
		else if(pMsg->wParam == 'Q' && GetKeyState(VK_CONTROL) && pMsg->lParam & 0x20000000)
		{
			PostMessage(WM_CLOSE);  //关闭
			//ShowWindow(SW_HIDE);
			//Sleep(500);
			//g_pMainDlg->ShowWindow(SW_SHOW);
		}
		
	}
    return CDialog::PreTranslateMessage(pMsg);
}

void CTunnelMonitorDisplayTbl::SetFreshTime(int itime)
{
	m_freshTime = itime;
}
void CTunnelMonitorDisplayTbl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(1 == nIDEvent)
	{
		//
		////SetTitleText();
		//
	}
	else if(2 == nIDEvent)
	{
		//
		//SetLogoTime();
		//
		//汇总区头尾
		////SetTotHeadTail();
        ////SetLogoM4Time();
	}
	else if(3 == nIDEvent)
	{
		if(m_disLoop)
		{
			//68,70,71
			if(KEY_D == m_shiftType)
			{
	    		keybd_event(VK_CONTROL,0,0,0);
				keybd_event(18,0,0,0);
				keybd_event(68,0,0,0);
				keybd_event(68,0,KEYEVENTF_KEYUP,0);
				keybd_event(18,0,KEYEVENTF_KEYUP,0);
				keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
				m_shiftType = KEY_F;
			}
			else if(KEY_F == m_shiftType)
			{
				keybd_event(VK_CONTROL,0,0,0);
				keybd_event(18,0,0,0);
				keybd_event(70,0,0,0);
				keybd_event(70,0,KEYEVENTF_KEYUP,0);
				keybd_event(18,0,KEYEVENTF_KEYUP,0);
				keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
				m_shiftType = KEY_G;
			}
			else if(KEY_G == m_shiftType)
			{
				keybd_event(VK_CONTROL,0,0,0);
				keybd_event(18,0,0,0);
				keybd_event(71,0,0,0);
				keybd_event(71,0,KEYEVENTF_KEYUP,0);
				keybd_event(18,0,KEYEVENTF_KEYUP,0);
				keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
				m_shiftType = KEY_D;
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CTunnelMonitorDisplayTbl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
 ////   if(pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE)
	////{
	////	m_tClrCnt++;
	////	if(1 == m_tClrCnt)
	////	{
	////	    pDC->SetTextColor(RGB(255,0,0));//
	////	}
	////	else if(2 == m_tClrCnt)
	////	{
	////		pDC->SetTextColor(RGB(0,255,0));//
	////	}
	////	else if(3 == m_tClrCnt)
	////	{
	////		pDC->SetTextColor(RGB(255,255,255));//
	////		m_tClrCnt = 0;
	////	}
	////	CFont titlefont;
	////	titlefont.CreatePointFont(300,"宋体");//
	////	pDC->SelectObject(&titlefont);//
	////}
	////if((pWnd->GetDlgCtrlID() == IDC_STATIC_TAIL_M4_1)||(pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE_M4_1)||(pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE_M4_2))
	////{
	////	pDC->SetTextColor(RGB(0,255,0));//
	////	CFont titlefont;
	////	titlefont.CreatePointFont(300,"宋体");//
	////	pDC->SelectObject(&titlefont);//
	////}
	////if((pWnd->GetDlgCtrlID() == IDC_STATIC_TOT_1)||(pWnd->GetDlgCtrlID() == IDC_STATIC_TOT_4)||(pWnd->GetDlgCtrlID() == IDC_STATIC_TOT_A)||(pWnd->GetDlgCtrlID() == IDC_STATIC_TOT_B))
	////{
	////	pDC->SetTextColor(RGB(0,255,0));//
	////	CFont titlefont;
	////	titlefont.CreatePointFont(300,"宋体");//
	////	pDC->SelectObject(&titlefont);//
	////}
	////if (nCtlColor == CTLCOLOR_STATIC)
	////{
	////	pDC->SetBkMode(TRANSPARENT);
	////	pDC->SetBkColor(RGB(0,0,0));
	////	HBRUSH b=CreateSolidBrush(RGB(0,0,0));
	////	return b;
	////}
	return hbr;
}

void CTunnelMonitorDisplayTbl::OnPaint()
{
	//m_csPaint.Lock();
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
    //
	GetDisplayContent();
	//
	CRect picRect;
	CStatic* pWnd = NULL;
    if(m_bMode3)
	{
		SetTblM4TitleText();
		//
		pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CONTENT_M4_1); // 
		SetTblM4Content(pWnd,1);
		//
		picRect.left = m_cx*3/5  -1;
		picRect.top = m_cy*7/24 -1;
		picRect.right = m_cx;
		picRect.bottom = m_cy-m_cy/15 + 2;;
		//
		pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CONTENT_M4_2); // 
		SetTblM4Content(pWnd,2);
		//
		SetLogoM4Time();
	}
	if(m_bMode2)
	{
		//汇总区头尾
		SetTotHeadTail();
		//汇总区
		SetTotContentA();
		//
	    SetTotContentB();
	}
	if(m_bMode1)
	{
		SetTitleText();
		//图形显示
		picRect.left = 0;
		picRect.top = m_cy*7/24;
		picRect.right = m_cx;
		picRect.bottom = m_cy;
		//
		pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_A); // 
		pWnd->MoveWindow(&picRect);
		CRect rect;
		pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
		pWnd->GetWindowRect(&rect);
		CString strPath = m_strPathHead;
		strPath += "\\pic\\map.bmp";
		//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\map.bmp"), 
		HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(strPath.GetBuffer()),
						IMAGE_BITMAP, 
						rect.Width(), 
						rect.Height(),
						LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		pWnd->SetBitmap(hPic);
		DeleteObject(hPic);
		//
		DisplayMonitorInfo_Ex();
		//
	}
	if(m_bUpFresh)
	{
	    SetTitleLogo();
		m_bUpFresh = false;
	}
	//
	//m_csPaint.Unlock();
}
//
BOOL CTunnelMonitorDisplayTbl::LoadTblTitleMapping()
{
	m_vecTblTitle.clear();
	CString filePath = m_strPathHead;
	filePath += "\\cfg\\devcfg.ini";
	if (access(filePath.GetBuffer(), 0) == -1)
	{
		return false;
	}
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;

	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	int section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	int iCount = 0;
	char stmp[256];
	for(int sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("详细信息表格标题" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		//pos_ini.Next();	
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_data = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_data, ",", vStr);
			if (vStr.size() != 3)
			{
				continue;
			}
			//
			if("Y" != vStr[2])
			{
			    continue;
			}
			//
			iCount++;
			//
			stuTblTitle tt;
			tt.item = atoi(vStr[0].c_str());
			tt.name = vStr[1];
			tt.flag = vStr[2];
			m_vecTblTitle.push_back(tt);
		}
	}
	return TRUE;
}
//加载读卡器与区域映射关系
BOOL CTunnelMonitorDisplayTbl::LoadReaderAreaMapping()
{
	CString filePath = m_strPathHead;
	filePath += "\\cfg\\devcfg.ini";
	if (access(filePath.GetBuffer(), 0) == -1)
	{
		return false;
	}
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
			if(len > 3) len = 3;
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
//加区域与左右洞映射关系
BOOL CTunnelMonitorDisplayTbl::LoadAreaTotMapping()
{
	CString filePath = m_strPathHead;
	filePath += "\\cfg\\devcfg.ini";
	if (access(filePath.GetBuffer(), 0) == -1)
	{
		return false;
	}
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
		if("左右洞和区域号映射关系" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}	
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			int id = atoi(str_id.GetBuffer());
			m_totAreaMap[id] = pos_ini.Data().second;
			//
		}
	}
	return TRUE;
}
//加载卡片与工人的映射关系
BOOL CTunnelMonitorDisplayTbl::LoadCardSignMapping()
{
	CString filePath = m_strPathHead;
	filePath += "\\cfg\\devcfg.ini";
	if (access(filePath.GetBuffer(), 0) == -1)
	{
		return false;
	}
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
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 6)
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
			len = strlen(vStr[4].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.deptName,vStr[4].c_str(),len);
			len = strlen(vStr[5].c_str());
			if(len > 255) len = 255;
			memcpy(cardSignMapping.workPost,vStr[5].c_str(),len);
			//
			string tmpstr = cardSignMapping.cardNo;
			memcpy(&m_csMap[tmpstr],&cardSignMapping,sizeof(stuCardSignMapping));
			//
		}
	}
	return TRUE;
}
//
BOOL CTunnelMonitorDisplayTbl::LoadPicOrigin()
{
	//m_Zx = 60;
	//m_Zy = 30;
	CString strPath = m_strPathHead;
	strPath += "\\cfg\\devcfg.ini";
	if (access(strPath.GetBuffer(), 0) == -1)
	{
		return false;
	}
	m_Zx = GetPrivateProfileInt("图片区域刻度", "x",0,strPath.GetBuffer());
	m_Zy = GetPrivateProfileInt("图片区域刻度", "y",0,strPath.GetBuffer());
	if(0 == m_Zx || 0 == m_Zy)
	{
		m_Zx = 20;
		m_Zy = 10;
	}
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("标题", "name","",stmp,256,strPath.GetBuffer());
	m_strTitName = stmp;
	return TRUE;
}
//加载区域坐标的映射关系
BOOL CTunnelMonitorDisplayTbl::LoadZoneAxisMapping()
{
	m_mapZoneAxis.clear();
	CString filePath = m_strPathHead;
	filePath += "\\cfg\\devcfg.ini";
	if (access(filePath.GetBuffer(), 0) == -1)
	{
		return false;
	}
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
		if("区域坐标映射关系" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 5)
			{
				continue;
			}
			unsigned char zNo = atoi(vStr[0].c_str());
			m_mapZoneAxis[zNo].x = atoi(vStr[1].c_str());
			m_mapZoneAxis[zNo].y = atoi(vStr[2].c_str());
			m_mapZoneAxis[zNo].w = atoi(vStr[3].c_str());
			m_mapZoneAxis[zNo].h = atoi(vStr[4].c_str());
		}
	}
	return TRUE;
}
//
bool CTunnelMonitorDisplayTbl::LoadAttData()
{
	char stmp[256];
	time_t time_now = time(NULL);
	struct tm tmTmp;
	tmTmp = *localtime(&time_now);
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,"%s\\data\\%d-%02d-%02d_att.ini",m_strPathHead.GetBuffer(),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
	CString filePath = _T(stmp);
	if (access(filePath.GetBuffer(), 0) == -1)
	{
		return false;
	}
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
		if("考勤数据" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			//
			unsigned int card_id = strtoul(str_id.GetBuffer(), NULL, 10);
			//map<string,stuCardSignMapping> m_csMap;
			map<string,stuCardSignMapping>::iterator iter_cs;
			iter_cs = m_csMap.find(str_id.GetBuffer());
			if(m_csMap.end() == iter_cs)continue; 
			//
			m_mapAttData[card_id].card_no = card_id;
			m_mapAttData[card_id].fst_time = strtoul(vStr[0].c_str(), NULL, 10);
			m_mapAttData[card_id].lst_time = strtoul(vStr[1].c_str(), NULL, 10);
		}
	}
	return true;
}
//
void CTunnelMonitorDisplayTbl::LoadCardFstTime()
{
	char stmp[256];
	time_t time_now = time(NULL);
	struct tm tmTmp;
	tmTmp = *localtime(&time_now);
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,".\\data\\%s\\data\\%d-%02d-%02d.dat",m_stationId.c_str(),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
	
	FILE *fp = fopen(stmp,"r");
	if(NULL != fp)
	{
		while(true)
		{
			memset(stmp,0,sizeof(stmp));
			if(NULL == fgets(stmp,256,fp))break;
			CString strTmp = stmp;
			std::vector<std::string> vStr;
			SpliteStr(strTmp, ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			m_mapCardFstTime[vStr[0]] = strtoul(vStr[1].c_str(), NULL, 10);
		}
		fclose(fp);
		fp = NULL;
	}
}
//
void CTunnelMonitorDisplayTbl::SetCardToDisplayData(unsigned int cardno,unsigned char readerno,time_t lsttime,int data_len,const unsigned char *data)
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
	//map<string,stuDisCardInfo> m_mapCardInfo;
	map<string,stuDisCardInfo>::iterator iter_ci;
	iter_ci = m_mapCardInfo.find(strCardNo);
	if(m_mapCardInfo.end() == iter_ci)
	{
		time_t time_now = time(NULL);
		m_mapCardInfo[strCardNo].fstTime = time_now;
		m_mapCardInfo[strCardNo].signName = iter_cs->second.signName;
		m_mapCardInfo[strCardNo].workType = iter_cs->second.workType;
		m_mapCardInfo[strCardNo].deptName = iter_cs->second.deptName;
		m_mapCardInfo[strCardNo].workPost = iter_cs->second.workPost;
	}
	m_mapCardInfo[strCardNo].areaNo = iter_ra->second.areaNo;
	m_mapCardInfo[strCardNo].lstTime = lsttime;
	if((data_len > 0)&&(data_len < 32))
	{
		m_mapCardInfo[strCardNo].data_len = data_len;
		memcpy(m_mapCardInfo[strCardNo].data,data,data_len);
	}
	else
	{
		m_mapCardInfo[strCardNo].data_len = 0;
		memset(m_mapCardInfo[strCardNo].data,0,sizeof(m_mapCardInfo[strCardNo].data));
	}
	//map<string,time_t> m_mapCardFstTime;
	////map<string,time_t>::iterator iter_ct;
	////iter_ct = m_mapCardFstTime.find(strCardNo);
	////if(m_mapCardFstTime.end() == iter_ct)
	////{
	////	time_t time_now = time(NULL);
	////	struct tm tmTmp;
	////	tmTmp = *localtime(&time_now);
	////	memset(stmp,0,sizeof(stmp));
	////	sprintf(stmp,".\\data\\%s\\data\\%d-%02d-%02d.dat",m_stationId.c_str(),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
	////	FILE *fp = fopen(stmp,"a+");
	////	if(NULL != fp)
	////	{
	////		memset(stmp,0,sizeof(stmp));
	////		sprintf(stmp,"%010u,%u\n",cardno,m_mapCardInfo[strCardNo].fstTime);
	////		fputs(stmp,fp);
	////		fclose(fp);
	////		fp = NULL;
	////	}
	////	m_mapCardFstTime[strCardNo] = time_now;
	////}
}
void CTunnelMonitorDisplayTbl::GetDisplayContent()
{
	m_mapAreaCardInfo.clear();
	m_mapDeptWorkerNum.clear();
	m_mapTotCardInfo.clear();
	m_totCardNum = 0;
	map<string,stuDisCardInfo>::iterator iter;
	for(iter = m_mapCardInfo.begin(); iter != m_mapCardInfo.end(); iter++)
	{
		time_t now_time = time(NULL);
		time_t lst_time = iter->second.lstTime;
		if(now_time - lst_time > m_freshTime)continue;
		m_totCardNum += 1;
		//填入区域信息
		stuAreaCardInfo stuCardInfo;
		stuCardInfo.cardNo = iter->first;
		stuCardInfo.signName = iter->second.signName;
		stuCardInfo.workType = iter->second.workType;
		stuCardInfo.deptName = iter->second.deptName;
		stuCardInfo.workPost = iter->second.workPost;
		stuCardInfo.fstTime = iter->second.fstTime;
		stuCardInfo.lstTime = iter->second.lstTime;
		stuCardInfo.data_len = iter->second.data_len;
		memcpy(stuCardInfo.data,iter->second.data,stuCardInfo.data_len);
		m_mapAreaCardInfo[iter->second.areaNo].push_back(stuCardInfo);
		m_mapDeptWorkerNum[stuCardInfo.deptName] += 1;
		//填入左右洞信息
		map<int,string>::iterator iter_ta;
		int id = iter->second.areaNo;
		iter_ta = m_totAreaMap.find(id);
		if(m_totAreaMap.end() != iter_ta)
		{
			m_mapTotCardInfo[iter_ta->second].push_back(stuCardInfo);
		}
	}
}
void CTunnelMonitorDisplayTbl::SetM4Display()
{
	CRect picRect;
	CStatic* pWnd = NULL;
	picRect.left = 0;
	picRect.top = m_cy*7/24-1;
	picRect.right = m_cx*2/5 + 1;
	picRect.bottom = m_cy-m_cy/15 + 2;
	//监控区
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CONTENT_M4_1); // 
	pWnd->MoveWindow(&picRect);
	//
	//SetTblContent(pWnd,1);
	//
	picRect.left = m_cx*3/5  -1;
	picRect.top = m_cy*7/24 -1;
	picRect.right = m_cx;
	picRect.bottom = m_cy-m_cy/15 + 2;;
	//监控区
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CONTENT_M4_2); // 
	pWnd->MoveWindow(&picRect);
	//
	//SetTblContent(pWnd,2);
	//
	picRect.left = m_cx*2/5 - 1;
	picRect.top = m_cy*7/24-1;
	picRect.right = m_cx*3/5 + 1;
	picRect.bottom = m_cy-m_cy/15 + 2;
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_LOGO_M4_1); // 得到 Picture Control 句柄
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	pWnd->MoveWindow(&picRect);
	CString strPath = m_strPathHead;
	strPath += "\\pic\\sz_mid_logo.bmp";
	//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\sz_mid_logo.bmp"), 
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(strPath.GetBuffer()), 
					IMAGE_BITMAP, 
					picRect.Width(), 
					picRect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
}
//
void CTunnelMonitorDisplayTbl::SetTblM4Content(CStatic* pWnd,int n)
{
	//标题
	//
	CRect clientrect;
	pWnd->GetClientRect(&clientrect);
	CDC* pDC; //表示控件窗口的设备描述表.DC 
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	//
	EraseWinBackground(clientrect.Width(),clientrect.Height(),pDC);
	//表格
	//
	CPen cuRed,cuBlue;
	cuRed.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));//红色，5像素宽 
	cuBlue.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));//蓝色，5像素宽 
	CPen* oldPen = pDC->SelectObject(&cuRed);
	pDC->SetPixel(245, 211, RGB(0, 0, 255));
	//
	int x = clientrect.Width();
	int y = clientrect.Height();
	int x1 = 40;
	int y1 = 60;
	int w = x - 80;
	int h = y - 120;
	int nx = 4;
	int ny = 11;
	int i = 0;
	//
	for(i = 0; i < ny; i++)
	{
		pDC->MoveTo(x1, y1+h*i/(ny-1));
		pDC->LineTo(x1+w, y1+h*i/(ny-1));
	}
	for(i = 0; i< nx; i++)
	{
		pDC->MoveTo(x1+w*i/(nx-1), y1);
		pDC->LineTo(x1+w*i/(nx-1), y1+h);
	}
	cuRed.DeleteObject();
	cuBlue.DeleteObject();
	pDC->SelectObject(oldPen);
	//表格标题
	CFont font;
	font.CreateFont(
		h/(1.5*ny-1), // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
	pDC->SelectObject(&font);
	CRect TextRect;
	//姓名
	TextRect.left = x1;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	CString nameStr = "姓名";
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255, 255, 255)); 
	CRect temp = TextRect;
	int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//
	TextRect.left = TextRect.right;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	nameStr = "岗位";
	temp = TextRect;
	height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//
	TextRect.left = TextRect.right;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	nameStr = "进入时间"; 
	temp = TextRect;
	height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//表格内容
	int pos = 0;
	string strId;
	if(1 == n)
	{
	    strId = "左洞";
		pos = m_leftPos;
	}
	else
	{
		strId = "右洞";
		pos = m_rightPos;
	}
	int iCount = 0;
	vector<stuAreaCardInfo>::iterator iter;
	//for(iter = m_mapTotCardInfo[strId].begin(); iter != m_mapTotCardInfo[strId].end(); iter++)
	for(i = pos; i < m_mapTotCardInfo[strId].size(); i++)
	{
		iCount += 1;
		pos += 1;
		//姓名
		TextRect.left = x1;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);
		TextRect.bottom = TextRect.top+h/(ny-1);
		//CString nameStr = iter->signName.c_str();
		CString nameStr = m_mapTotCardInfo[strId][i].signName.c_str();
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(255, 255, 255)); 
		CRect temp = TextRect;
		int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		//
		TextRect.left = TextRect.right;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);
		TextRect.bottom = TextRect.top+h/(ny-1);
		//nameStr = iter->workPost.c_str();
		nameStr = m_mapTotCardInfo[strId][i].workPost.c_str();
		temp = TextRect;
		height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		//
		TextRect.left = TextRect.right;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);
		TextRect.bottom = TextRect.top+h/(ny-1);
		struct tm *now = NULL;
		//time_t time_fst = iter->fstTime;
		time_t time_fst = m_mapTotCardInfo[strId][i].fstTime;
		now = localtime(&time_fst);
		char stmp[256];
		sprintf(stmp,"%02d:%02d",now->tm_hour,now->tm_min);
		nameStr = stmp;  
		temp = TextRect;
		height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		if(iCount >= ny-2)break;
	}
	if(1 == n)
	{
		m_leftPos = pos;
	}
	else
	{
		m_rightPos = pos;
	}
	if(pos >= m_mapTotCardInfo[strId].size())
	{
		if(1 == n)
		{
			m_leftPos = 0;
		}
		else
		{
			m_rightPos = 0;
		}
	}
	font.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//显示该区总人数
	CString strTmp;
	if("左洞" == strId)
	{
	    strTmp.Format("%s内在岗人数[%d]人 ",strId.c_str(),m_mapTotCardInfo[strId].size());
	}
	else
	{
		strTmp.Format("    %s内在岗人数[%d]人 ",strId.c_str(),m_mapTotCardInfo[strId].size());
	}
	CStatic *pWndx = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_M4_1+n-1);
	pDC = pWndx->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWndx->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	CFont titlefont;
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	//pWndx->SetFont(&titlefont);
	//pWndx->SetWindowTextA(strTmp);
	temp = TextRect;
	height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWndx->ReleaseDC(pDC);
    
}
//
void CTunnelMonitorDisplayTbl::SetTblContent(CStatic* pWnd,unsigned char areaNo)
{
	
    
}
void CTunnelMonitorDisplayTbl::SetTitleText()
{
	struct tm *now = NULL;
	time_t time_now = time(NULL);
	now = localtime(&time_now);
	char stmp[256];
	sprintf(stmp,"%d-%02d-%02d %02d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	//
	CRect titRect;
	titRect.left = 0;
	titRect.top = m_cy*5/24-1;
	titRect.right = m_cx;
	titRect.bottom = m_cy*7/24;
	//titRect.top = 0;
	//titRect.right = m_cx*4/5;
	//titRect.bottom = m_cy/6+1;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TITLE); 
	pWnd->MoveWindow(&titRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	CString strTmp;// = "欢迎各位领导莅临检查指导工作";
	if(IS_COMM == m_displayType)
	{
		strTmp.Format("%s  %s - 现有人员：%d 人",stmp,m_strTitName.c_str(),m_totCardNum);
	}
	else
	{
		strTmp.Format("%s  %s施工现场 - 现有施工人员：%d 人",stmp,m_strTitName.c_str(),m_totCardNum);
	}
	//pWnd->SetWindowTextA(strTmp);
	CDC* pDC; //表示控件窗口的设备描述表.DC
	CRect TextRect,temp;
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	CFont titlefont;
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	//pWndx->SetFont(&titlefont);
	//pWndx->SetWindowTextA(strTmp);
	temp = TextRect;
	int height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//
}

void CTunnelMonitorDisplayTbl::SetTblTitleText(int n,int cnt,int nid)
{
	//
	CRect titRect;
	titRect.left = m_cx*n/cnt - 1;
	titRect.top = m_cy/6 - 1;
	titRect.right = titRect.left+m_cx/3 + 2;
	titRect.bottom = titRect.top + m_cy/8 +2;
	CStatic* pWnd = (CStatic*)GetDlgItem(nid); 
	pWnd->MoveWindow(&titRect);
	//pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	CString strTmp = "";
	strTmp.Format("[%d]段有[0]人",n);
	//pWnd->SetWindowTextA(strTmp);
	//
	CDC* pDC; //表示控件窗口的设备描述表.DC
	CRect TextRect,temp;
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,0,255));//
	CFont titlefont;
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	//pWndx->SetFont(&titlefont);
	//pWndx->SetWindowTextA(strTmp);
	temp = TextRect;
	int height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//
}
//
void CTunnelMonitorDisplayTbl::SetTblM4TitleText()
{
	//
	CRect titRect;
	titRect.left = 0;
	titRect.top = m_cy*5/24 - 1;
	titRect.right = titRect.left+m_cx/2 + 1;
	titRect.bottom = titRect.top + m_cy/8 +2;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_M4_1); 
	pWnd->MoveWindow(&titRect);
	//pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	////CString strTmp = "    左洞内在岗人数[0]人";
	////pWnd->SetWindowTextA(strTmp);
	//
	titRect.left = m_cx/2-1;
	titRect.top = m_cy*5/24 - 1;
	titRect.right = +m_cx;
	titRect.bottom = titRect.top + m_cy/8 +2;
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TITLE_M4_2); 
	pWnd->MoveWindow(&titRect);
	//pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	////strTmp = "            右洞内在岗人数[0]人";
	//////pWnd->SetWindowTextA(strTmp);
	////CDC* pDC; //表示控件窗口的设备描述表.DC
	////CRect TextRect,temp;
	////pDC = pWnd->GetDC();
	////if(NULL == pDC)return;
	////if(NULL == pDC->m_hDC)return;
	////pWnd->GetClientRect(&TextRect);
	////EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	////pDC->SetTextColor(RGB(0,255,0));//
	////CFont titlefont;
	////titlefont.CreatePointFont(300,"宋体");//
	////pDC->SelectObject(&titlefont);//
	//////pWndx->SetFont(&titlefont);
	//////pWndx->SetWindowTextA(strTmp);
	////temp = TextRect;
	////int height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	////TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	////pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	////titlefont.DeleteObject();
	////pWnd->ReleaseDC(pDC);
	//
	titRect.left = 0;
	titRect.top = m_cy-m_cy/15 - 1;
	titRect.right = m_cx;
	titRect.bottom = m_cy;
	//IDC_STATIC_TOT_1
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TAIL_M4_1); // 
	pWnd->MoveWindow(&titRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	CString strTmp = "杭州数众隧道人员管理系统";
	//pWnd->SetWindowTextA(strTmp);
	CDC* pDC; //表示控件窗口的设备描述表.DC
	CRect TextRect,temp;
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	CFont titlefont;
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	temp = TextRect;
	int height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//
}
//汇总区头尾
void CTunnelMonitorDisplayTbl::SetTotHeadTail()
{
	struct tm *now = NULL;
	time_t time_now = time(NULL);
	now = localtime(&time_now);
	struct tm mon_end;
	//IDC_STATIC_TOT_1
	CRect picRect;
	picRect.left = 0;
	picRect.top = m_cy*5/24 - 1;
	picRect.right = m_cx;
	picRect.bottom = picRect.top+m_cy/12 + 2;
	//
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TOT_1); // 
	pWnd->MoveWindow(&picRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
    mon_end = *localtime(&time_now);
	if(mon_end.tm_mon < 12)
	{
	    mon_end.tm_mon += 1;
	}
	else
	{
		mon_end.tm_year += 1;
		mon_end.tm_mon = 1;
	}
	mon_end.tm_mday = 1;
	mon_end.tm_hour = 0;
	mon_end.tm_min = 0;
	mon_end.tm_sec = 0;
	time_t tmpTime = difftime(mktime(&mon_end),time_now);
	CString strTmp;
	int tmpDay = tmpTime/(24*60*60);
	int tmpHour = (tmpTime-tmpDay*24*60*60)/3600;
	int tmpMin = (tmpTime-tmpDay*24*60*60-tmpHour*3600)/60;
	int tmpSec = tmpTime - tmpDay*24*60*60-tmpHour*3600-tmpMin*60;
	strTmp.Format("月计划倒计时 %d天%d小时%d分%d秒",tmpDay,tmpHour,tmpMin,tmpSec);
	//pWnd->SetWindowTextA(strTmp);
	CDC* pDC; //表示控件窗口的设备描述表.DC
	CRect TextRect,temp;
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	CFont titlefont;
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	temp = TextRect;
	int height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//
	//IDC_STATIC_TOT_A
	picRect.left = 0;
	picRect.top = picRect.bottom - 1;
	picRect.right = m_cx/2 + 1;
	picRect.bottom = picRect.top+m_cy/12 + 2;
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TOT_A); // 
	pWnd->MoveWindow(&picRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	strTmp.Format("隧道内在岗总人数[%d]人",m_totCardNum);
	//pWnd->SetWindowTextA(strTmp);
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	temp = TextRect;
	height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//IDC_STATIC_TOT_B
	picRect.left = m_cx/2 - 1;
	picRect.right = m_cx;
	picRect.bottom = picRect.top+m_cy/12 + 2;
	//
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TOT_B); // 
	pWnd->MoveWindow(&picRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	char stmp[256];
	sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	strTmp = stmp;
	//pWnd->SetWindowTextA(strTmp);
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	temp = TextRect;
	height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
	//
	picRect.left = 0;
	picRect.top = m_cy-m_cy/15 - 1;
	picRect.right = m_cx;
	picRect.bottom = m_cy;
	//IDC_STATIC_TOT_1
	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TOT_4); // 
	pWnd->MoveWindow(&picRect);
	pWnd->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	//struct tm year_begin;
	//year_begin = *localtime(&time_now);
	//year_begin.tm_mon = 1;
	//year_begin.tm_mday = 1;
	//year_begin.tm_hour = 0;
	//year_begin.tm_min = 0;
	//year_begin.tm_sec = 0;
	//tmpTime = difftime(time_now,mktime(&year_begin));
	//tmpDay = tmpTime/(24*60*60);
	//tmpHour = (tmpTime-tmpDay*24*6//0*60)/3600;
	//tmpMin = (tmpTime-tmpDay*24*60*60-tmpHour*3600)/60;
	//tmpSec = tmpTime - tmpDay*24*60*60-tmpHour*3600-tmpMin*60;
	strTmp.Format("安全生产天数 %d天%d小时%d分%d秒",now->tm_yday,now->tm_hour,now->tm_min,now->tm_sec);
	//pWnd->SetWindowTextA(strTmp);
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	pWnd->GetClientRect(&TextRect);
	EraseWinBackground(TextRect.Width(),TextRect.Height(),pDC);
	pDC->SetTextColor(RGB(0,255,0));//
	titlefont.CreatePointFont(300,"宋体");//
	pDC->SelectObject(&titlefont);//
	temp = TextRect;
	height = pDC->DrawText(strTmp, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(strTmp, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	titlefont.DeleteObject();
	pWnd->ReleaseDC(pDC);
}
//汇总区
void CTunnelMonitorDisplayTbl::SetTotContentA()
{
	//IDC_STATIC_TOT_2
	CRect picRect;
	picRect.left = 0;
	picRect.top = m_cy*3/8 - 1;
	picRect.right = m_cx/2 + 1;
	picRect.bottom = m_cy-m_cy/15  + 2;
	//
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TOT_2); // 
	pWnd->MoveWindow(&picRect);
    //标题
	//
	CRect clientrect;
	pWnd->GetClientRect(&clientrect);
	CDC* pDC; //表示控件窗口的设备描述表.DC 
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	//
	EraseWinBackground(clientrect.Width(),clientrect.Height(),pDC);
	//表格
	//
	CPen cuRed,cuBlue;
	cuRed.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));//红色，5像素宽 
	cuBlue.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));//蓝色，5像素宽 
	CPen* oldPen = pDC->SelectObject(&cuRed);
	pDC->SetPixel(245, 211, RGB(0, 0, 255));
	//
	int x = clientrect.Width();
	int y = clientrect.Height();
	int x1 = 80;
	int y1 = 60;
	int w = x - 160;
	int h = y - 120;
	int nx = 3;
	int ny = 9;
	int i = 0;
	//
	for(i = 0; i < ny; i++)
	{
		pDC->MoveTo(x1, y1+h*i/(ny-1));
		pDC->LineTo(x1+w, y1+h*i/(ny-1));
	}
	for(i = 0; i< nx; i++)
	{
		pDC->MoveTo(x1+w*i/(nx-1), y1);
		pDC->LineTo(x1+w*i/(nx-1), y1+h);
	}
	cuRed.DeleteObject();
	cuBlue.DeleteObject();
	pDC->SelectObject(oldPen);
	//表格标题
	CFont font;
	font.CreateFont(
		h/(1.5*ny-1), // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
	pDC->SelectObject(&font);
	CRect TextRect;
	//姓名
	TextRect.left = x1;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	CString nameStr = "姓名";
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255, 255, 255)); 
	CRect temp = TextRect;
	int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//
	TextRect.left = TextRect.right;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	nameStr = "上班时间";
	temp = TextRect;
	height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//表格内容
	////int iCount = 0;
	////////map<string,time_t> m_mapCardFstTime;
	////map<string,time_t>::iterator iter;
	////iter = m_mapCardFstTime.begin();
	////for(int i = 0; i < m_curTotPos1; i++)
	////{
	////    iter++;
	////}
	////for(; iter != m_mapCardFstTime.end(); iter++)
	////{
	////	iCount += 1;
	////	m_curTotPos1 += 1;
	////	//姓名
	////	TextRect.left = x1;
	////	TextRect.right = TextRect.left+w/(nx-1);
	////	TextRect.top = y1 + iCount*h/(ny-1);
	////	TextRect.bottom = TextRect.top+h/(ny-1);
	////	//CString nameStr = "  ";
	////	CString nameStr = m_csMap[iter->first].signName; 
	////	//nameStr = nameStr + "  ";
	////	CRect temp = TextRect;
	////	int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	////	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	////	pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	////	//
	////	TextRect.left = TextRect.right;
	////	TextRect.right = TextRect.left+w/(nx-1);
	////	TextRect.top = y1 + iCount*h/(ny-1);
	////	TextRect.bottom = TextRect.top+h/(ny-1);
	////	struct tm *now = NULL;
	////	time_t time_fst = iter->second;
	////	now = localtime(&time_fst);
	////	char stmp[256];
	////	//sprintf(stmp,"%d-%02d-%02d %02d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	////	sprintf(stmp,"%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
	////	nameStr = stmp;
	////	temp = TextRect;
	////	height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	////	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	////	pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	////	if(iCount >= ny-2)break;
    LoadAttData();
	//表格内容
	int iCount = 0;
	char stmp[256];
	////map<unsigned int,stuAttendanceData> m_mapAttData;
	map<unsigned int,stuAttendanceData>::iterator iter;
	iter = m_mapAttData.begin();
	for(int i = 0; i < m_curTotPos1; i++)
	{
	    iter++;
	}
	for(; iter != m_mapAttData.end(); iter++)
	{
		iCount += 1;
		m_curTotPos1 += 1;
		//姓名
		TextRect.left = x1;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);
		TextRect.bottom = TextRect.top+h/(ny-1);
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%010u",iter->first);
		string card_id = stmp;
		//CString nameStr = "  ";
		CString nameStr = m_csMap[card_id].signName; 
		//nameStr = nameStr + "  ";
		CRect temp = TextRect;
		int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		//
		TextRect.left = TextRect.right;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);
		TextRect.bottom = TextRect.top+h/(ny-1);
		struct tm *now = NULL;
		time_t time_fst = iter->second.fst_time;
		now = localtime(&time_fst);
		char stmp[256];
		//sprintf(stmp,"%d-%02d-%02d %02d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
		sprintf(stmp,"%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
		nameStr = stmp;
		temp = TextRect;
		height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		if(iCount >= ny-2)break;
	}
	if(m_curTotPos1 >= m_mapAttData.size())m_curTotPos1 = 0;
	//
	font.DeleteObject();
	pWnd->ReleaseDC(pDC);
}
//
void CTunnelMonitorDisplayTbl::SetTotContentB()
{
	//IDC_STATIC_TOT_3
	CRect picRect;
	picRect.left = m_cx/2 - 1;
	picRect.top = m_cy*11/30 - 1;
	picRect.right = m_cx;
	picRect.bottom = m_cy-m_cy/15 + 2;
	//
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TOT_3); // 
	pWnd->MoveWindow(&picRect);
	//
    //标题
	//
	CRect clientrect;
	pWnd->GetClientRect(&clientrect);
	CDC* pDC; //表示控件窗口的设备描述表.DC 
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	//
	EraseWinBackground(clientrect.Width(),clientrect.Height(),pDC);
	//表格
	//
	CPen cuRed,cuBlue;
	cuRed.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));//红色，5像素宽 
	cuBlue.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));//蓝色，5像素宽 
	CPen* oldPen = pDC->SelectObject(&cuRed);
	pDC->SetPixel(245, 211, RGB(0, 0, 255));
	//
	int x = clientrect.Width();
	int y = clientrect.Height();
	int x1 = 80;
	int y1 = 60;
	int w = x - 160;
	int h = y - 120;
	int nx = 3;
	int ny = 9;
	int i = 0;
	//
	for(i = 0; i < ny; i++)
	{
		pDC->MoveTo(x1, y1+h*i/(ny-1));
		pDC->LineTo(x1+w, y1+h*i/(ny-1));
	}
	for(i = 0; i< nx; i++)
	{
		pDC->MoveTo(x1+w*i/(nx-1), y1);
		pDC->LineTo(x1+w*i/(nx-1), y1+h);
	}
	cuRed.DeleteObject();
	cuBlue.DeleteObject();
	pDC->SelectObject(oldPen);
	//表格标题
	CFont font;
	font.CreateFont(
		h/(1.5*ny-1), // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
	pDC->SelectObject(&font);
	CRect TextRect;
	//部门
	TextRect.left = x1;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	CString nameStr = "部门";
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255, 255, 255)); 
	CRect temp = TextRect;
	int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//人数
	TextRect.left = TextRect.right;
	TextRect.right = TextRect.left+w/(nx-1);
	TextRect.top = y1;
	TextRect.bottom = TextRect.top+h/(ny-1);
	nameStr = "人数"; 
	temp = TextRect;
	height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
    pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//表格内容
	int iCount = 0;
	//map<string,int> m_mapDeptWorkerNum;
	map<string,int>::iterator iter;
	for(iter = m_mapDeptWorkerNum.begin(); iter != m_mapDeptWorkerNum.end(); iter++)
	{
		iCount += 1;
		//部门
		TextRect.left = x1;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);;
		TextRect.bottom = TextRect.top+h/(ny-1);
		CString nameStr = iter->first.c_str();
		CRect temp = TextRect;
		int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		//人数
		TextRect.left = TextRect.right;
		TextRect.right = TextRect.left+w/(nx-1);
		TextRect.top = y1 + iCount*h/(ny-1);;
		TextRect.bottom = TextRect.top+h/(ny-1);
		nameStr.Format("%d",iter->second); 
		temp = TextRect;
		height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
		TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
		pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
		if(iCount >= ny-2)break;
	}
	//
	font.DeleteObject();
	pWnd->ReleaseDC(pDC);
}
void CTunnelMonitorDisplayTbl::DisplayMonitorInfo()
{
 
}
//
void CTunnelMonitorDisplayTbl::DisplayMonitorInfo_Ex()
{
	CRect rect;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_A); // 
	pWnd->GetWindowRect(&rect);
	CDC* pControlDC; //表示控件窗口的设备描述表.DC 
	pControlDC = pWnd->GetDC();
	if(NULL == pControlDC)return;
	if(NULL == pControlDC->m_hDC)return;
	//
	CFont font,*oldfont;
	font.CreateFont(
		m_cy/27, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
    //
	int sw = rect.Width();
	int sh = rect.Height();
	int x = 0;
	int y = 0;
    //map<unsigned char,vector<stuAreaCardInfo>> m_mapAreaCardInfo;
    int iNo = 0;
    map<unsigned char,vector<stuAreaCardInfo>>::iterator iter;
	for(iter = m_mapAreaCardInfo.begin(); iter != m_mapAreaCardInfo.end(); iter++)
	{
		unsigned char iArea = iter->first;
		map<unsigned char,stuZoneAxis>::iterator iter_z;
		iter_z = m_mapZoneAxis.find(iArea);
		if(iter_z == m_mapZoneAxis.end())
		{
			continue;
		}
		for(int i = 0; i < iter->second.size(); i++)
		{
			unsigned int u;
			u = strtoul(iter->second[i].cardNo.c_str(), NULL, 10);
			srand(u/(20*i+100)); //(unsigned) GetTickCount()用时间做种，每次产生随机数不一样
			int number = rand();  //产生随机数
			//显示
			x = number%(iter_z->second.w*sw/m_Zx) + sw*iter_z->second.x/m_Zx;
			y = number%(iter_z->second.h*sh/m_Zy) + sh*iter_z->second.y/m_Zy;
			CBrush brush, *oldbrush; 
			brush.CreateSolidBrush(RGB(255, 0, 0)); 
			oldbrush = pControlDC->SelectObject(&brush);
			pControlDC->Ellipse(x, y, x+m_dstR, y+m_dstR);
			pControlDC->SelectObject(oldbrush);
			//DeleteObject(brush);
			brush.DeleteObject();
			//输出文本
			CRect TextRect;
			TextRect.left = x - m_txtW;
			TextRect.right = TextRect.left + m_txtW*2;
			TextRect.top = y - m_dstR - m_dstTxtH;
			TextRect.bottom = TextRect.top + m_txtH;
			CBrush brush1, *oldbrush1;
			brush1.CreateSolidBrush(RGB(255, 0, 0)); 
			oldbrush1 = pControlDC->SelectObject(&brush1);
			CString nameStr;
			if(iter->second[i].data_len > 0)
			{
				TextRect.left = x - 3*m_txtW;
				TextRect.right = TextRect.left + m_txtW*6;
				TextRect.top = y - m_dstR - 5*m_dstTxtH;
				TextRect.bottom = TextRect.top + 3*m_txtH;
				//腕带数据
				string watch_str = "";
				ParseWatchData(iter->second[i].data_len,iter->second[i].data,watch_str);
				nameStr.Format("%s,%s",iter->second[i].signName.c_str(),watch_str.c_str());
			}
			else
			{
			    nameStr.Format("%s,%s",iter->second[i].signName.c_str(),iter->second[i].workPost.c_str());
			}
			pControlDC->SetTextColor(RGB(0,0,255));
			pControlDC->DrawText(nameStr, nameStr.GetLength(), &TextRect, DT_CENTER);
			pControlDC->SelectObject(oldbrush1);
			//DeleteObject(brush1);
			brush1.DeleteObject();
		}
		if(IS_HZSD == m_displayType)
		{
			oldfont = pControlDC->SelectObject(&font);
			CString strTmp;
			strTmp.Format("%s[%d]人 ",m_raMap[iArea].areaName,iter->second.size());
			x = sw*iter_z->second.x/m_Zx;
			y = iter_z->second.h*sh/m_Zy + sh*iter_z->second.y/m_Zy;
			CRect TextRect;
			TextRect.left = x;
			TextRect.right = TextRect.left + m_txtW*2;
			TextRect.top = y + m_txtH;
			TextRect.bottom = TextRect.top + m_txtH*2;
			CBrush brush2, *oldbrush2;
			brush2.CreateSolidBrush(RGB(255, 0, 0)); 
			oldbrush2 = pControlDC->SelectObject(&brush2);
			pControlDC->SetTextColor(RGB(255,0,0));
			pControlDC->DrawText(strTmp, strTmp.GetLength(), &TextRect, DT_CENTER);
			pControlDC->SelectObject(oldbrush2);
			//DeleteObject(brush2);
			brush2.DeleteObject();
			pControlDC->SelectObject(oldfont);
		}
	}
	if(IS_HZSD == m_displayType)
	{
	    //左洞
		oldfont = pControlDC->SelectObject(&font);
		CString strTmp;
		string strId = "左洞";
		strTmp.Format("左洞[%d]人 ",m_mapTotCardInfo[strId].size());
		x = m_txtW/2;
		int n = m_mapZoneAxis.size();
		n = n/2;
		y = m_mapZoneAxis[n-1].h*sh/m_Zy + m_mapZoneAxis[n-1].y*sh/m_Zy;
		CRect TextRect;
		TextRect.left = x;
		TextRect.right = TextRect.left + m_txtW*2;
		TextRect.top = y + m_txtH;
		TextRect.bottom = TextRect.top + m_txtH*2;
		CBrush brush3, *oldbrush3;
		brush3.CreateSolidBrush(RGB(255, 0, 0)); 
		oldbrush3 = pControlDC->SelectObject(&brush3);
		pControlDC->SetTextColor(RGB(255,0,0));
		pControlDC->DrawText(strTmp, strTmp.GetLength(), &TextRect, DT_CENTER);;
		//右洞
		strId = "右洞";
		strTmp.Format("右洞[%d]人 ",m_mapTotCardInfo[strId].size());
		x = m_txtW/2;
		y = m_mapZoneAxis[n+1].h*sh/m_Zy + m_mapZoneAxis[n+1].y*sh/m_Zy;
		TextRect.left = x;
		TextRect.right = TextRect.left + m_txtW*2;
		TextRect.top = y + m_txtH;
		TextRect.bottom = TextRect.top + m_txtH*2;
		pControlDC->DrawText(strTmp, strTmp.GetLength(), &TextRect, DT_CENTER);
		pControlDC->SelectObject(oldbrush3);
		//DeleteObject(brush3);
		brush3.DeleteObject();
		pControlDC->SelectObject(oldfont);
	}
	//
	font.DeleteObject();
	pWnd->ReleaseDC(pControlDC);
}
//
void CTunnelMonitorDisplayTbl::ParseWatchData(int len,const unsigned char *data,string &desc_str)
{
	if(0 == len || NULL == data)return;
	char stmp[256];
	memset(stmp,0,sizeof(stmp));
	double d_x = 0;
	double d_y = 0;
	double d_z = 0;
	double d_temperature = 0;
	double d_rate = 0;
	int    i_ok = 0;
	short w_tmp;
	const unsigned char *p = data;
	//x
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_x = w_tmp * 3.9 / 1000;
	p += 2;
	//y
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_y = w_tmp * 3.9 / 1000;
	p += 2;
	//z
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_z = w_tmp * 3.9 / 1000;
	p += 2;
	//温度
	w_tmp = p[0] * 256 + p[1];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_temperature = w_tmp / 10;
	p += 2;
	//心率
	w_tmp = p[0];
	if(w_tmp < 0)w_tmp = w_tmp * (-1);
	d_rate = 60 / (w_tmp * 0.02);
	p += 1;
	//是否脱手
	i_ok = p[0];
	p += 1;
	string strTmp = "";
	if(i_ok > 127)strTmp = "脱手";
	else strTmp = "未脱手";
	//
	int i_rate = d_rate;
	sprintf(stmp,"%.04f,%.04f\n%.04f,%.02f,%d,",d_x,d_y,d_z,d_temperature,i_rate);
	desc_str = stmp;
	desc_str += strTmp;

}
//
void CTunnelMonitorDisplayTbl::SetLogoTime()
{
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_B); 
	CRect clientrect;
	pWnd->GetClientRect(&clientrect);
	CDC* pDC; //表示控件窗口的设备描述表.DC 
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	//
	CFont font;
	font.CreateFont(
		m_cy/27, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
	pDC->SelectObject(&font);
	CRect TextRect;
	//姓名
	TextRect.left = clientrect.left;
	TextRect.right = clientrect.right;
	TextRect.top = clientrect.Height() * 2/3;//m_cy/6 - m_cy/24;
	TextRect.bottom = clientrect.bottom;
	struct tm *now = NULL;
	time_t time_now = time(NULL);
	now = localtime(&time_now);
	char stmp[256];
	sprintf(stmp,"%d-%02d-%02d %02d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	CString nameStr = stmp;
	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0, 0, 255)); 
	CRect temp = TextRect;
	int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//
	font.DeleteObject();
	pWnd->ReleaseDC(pDC);
}
//
void CTunnelMonitorDisplayTbl::SetLogoM4Time()
{
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_LOGO_M4_1); 
	CRect clientrect;
	pWnd->GetClientRect(&clientrect);
	CDC* pDC; //表示控件窗口的设备描述表.DC 
	pDC = pWnd->GetDC();
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	//
	CFont font;
	font.CreateFont(
		m_cy/27, // nHeight
		0, // nWidth
		0, // nEscapement
		0, // nOrientation
		FW_NORMAL, // nWeight
		FALSE, // bItalic
		FALSE, // bUnderline
		0, // cStrikeOut
		ANSI_CHARSET, // nCharSet
		OUT_DEFAULT_PRECIS, // nOutPrecision
		CLIP_DEFAULT_PRECIS, // nClipPrecision
		DEFAULT_QUALITY, // nQuality
		DEFAULT_PITCH | FF_SWISS,
		_T("Arial") // nPitchAndFamily Arial
		);
	pDC->SelectObject(&font);
	CRect TextRect;
	//姓名
	TextRect.left = clientrect.left;
	TextRect.right = clientrect.right;
	TextRect.top = clientrect.Height() * 2/3;//m_cy/6 - m_cy/24;
	TextRect.bottom = clientrect.bottom;
	struct tm *now = NULL;
	time_t time_now = time(NULL);
	now = localtime(&time_now);
	char stmp[256];
	sprintf(stmp,"%d-%02d-%02d %02d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	CString nameStr = stmp;
	pDC->SetBkColor(RGB(255,255,255));
	pDC->SetTextColor(RGB(0, 0, 255)); 
	CRect temp = TextRect;
	int height = pDC->DrawText(nameStr, temp, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_CALCRECT);
	TextRect.DeflateRect(0, (TextRect.Height() - height)/2);// change rect
	pDC->DrawText(nameStr, TextRect, DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);
	//
	font.DeleteObject();
	pWnd->ReleaseDC(pDC);
}
//
void CTunnelMonitorDisplayTbl::SetTitleLogo()
{
	//logo
	CRect picRect;
	//picRect.left = m_cx*4/5;
	picRect.left = 0;
	picRect.top = 0;
	picRect.right = m_cx;
	picRect.bottom = m_cy*5/24+1;
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAP_B); // 得到 Picture Control 句柄
	pWnd->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE); // 修改它的属性为位图
	pWnd->MoveWindow(&picRect);
	//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\sz_main_logo.bmp"),
	CString strPath = m_strPathHead;
	strPath += "\\pic\\sz_up_logo.bmp";
	//HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(".\\pic\\sz_up_logo.bmp"),
	HBITMAP hPic = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), _T(strPath.GetBuffer()),
					IMAGE_BITMAP, 
					picRect.Width(), 
					picRect.Height(),
					LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	pWnd->SetBitmap(hPic);
	DeleteObject(hPic);
	//
}
//
void CTunnelMonitorDisplayTbl::EraseWinBackground(int w,int h,CDC* pDC)
{
	//
	if(NULL == pDC)return;
	if(NULL == pDC->m_hDC)return;
	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,0));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	pDC->FillSolidRect(0,0,w,h,RGB(0,0,0));//设置背景色
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}