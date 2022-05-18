#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//
const int IN_OUT_TIME_OUT = 30;
const int CARD_TIME_OUT = 30;
//
struct stuReaderAreaMapping
{
	unsigned char readerNo;
	unsigned char areaNo;
	char          areaName[256];
	int           man_cnt;
};

struct stuCardSignMapping
{
	char  cardNo[20];
	char  signNo[20];
	char  signName[256];
	char  workType[256];
	char  deptName[256];
	char  workPost[256];
};
//卡信息
struct stuDisCardInfo
{
	//char  cardNo[20]
	unsigned char areaNo;
	string signName;
	string workType;
	string deptName;
	string workPost;
	time_t fstTime;
	time_t lstTime;
	unsigned char data_len;
	unsigned char data[32];
};
//卡信息
struct stuAreaCardInfo
{
	string cardNo;
	string signNo;
	string signName;
	string workType;
	string deptName;
	string workPost;
	time_t fstTime;
	time_t lstTime;
	unsigned char data_len;
	unsigned char data[32];
};
//进出信息
struct stuDisInOutInfo
{
	//char  cardNo[20]
	unsigned char ioFlg;
	char io_time[20];
	time_t lstTime;
};
//
// CTunnelMonitorDisplay 对话框

class CTunnelMonitorDisplay : public CDialog
{
	DECLARE_DYNAMIC(CTunnelMonitorDisplay)

public:
	CTunnelMonitorDisplay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTunnelMonitorDisplay();
    BOOL OnInitDialog();
	BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 对话框数据
	enum { IDD = IDD_DLG_DISPLAY };
public:
	BOOL StartFreshDlg();
	void DisplayMonitorInfo();
	void DisplayInOutInfo();
	void DisplayMonitorInfoX();
	BOOL LoadReaderAreaMapping();
	BOOL LoadCardSignMapping();
	void SetLogo();
	void SetCardToDisplayData(unsigned int cardno,unsigned char readerno,time_t lsttime);
public:
	CWinThread *m_freshDlgThread;
	BOOL m_bFresh;
	BOOL m_bDemo;
	BOOL m_bList;
	BOOL m_bList_x;
	int m_cx;
	int m_cy;
	int m_ocx;
	int m_ocy;
	int m_disW;
	int m_disH;
	//目标半径
	int m_dstR;
	//文本宽度
	int m_txtW;
	//文本高度
	int m_txtH;
	//文本和目标距离
	int m_dstTxtH;
	//进出显示位置
	int m_inOutIndex;
	//当前区域
	int m_curArea;
	map<int,stuReaderAreaMapping> m_raMap;
    map<string,stuCardSignMapping> m_csMap;
	//卡信息
	map<string,stuDisCardInfo> m_mapCardInfo;
	//进出信息
	map<string,stuDisInOutInfo> m_mapInOutInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	CStatic m_ctrlPic;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CListCtrl m_lstCardInfo;
public:
	CListCtrl m_lstCardInfo_x;
};
