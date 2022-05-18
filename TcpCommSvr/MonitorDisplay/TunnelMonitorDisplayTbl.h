#pragma once

#include "Resource.h"
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;
//
const int IN_OUT_TIME_OUT = 30;
const int CARD_TIME_OUT = 30;
//
enum KEY_TYPE
{
	KEY_D = 1,
	KEY_F = 2,
	KEY_G = 3,
};
//
struct stuTblTitle
{
	int item;
	string name;
	string flag;
};
//区域坐标
struct stuZoneAxis
{
	int x;
	int y;
	int w;
	int h;
};
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
//

//进出信息
struct stuDisInOutInfo
{
	//char  cardNo[20]
	unsigned char ioFlg;
	char io_time[20];
	time_t lstTime;
};
//
// CTunnelMonitorDisplayTbl 对话框

class CTunnelMonitorDisplayTbl : public CDialog
{
	DECLARE_DYNAMIC(CTunnelMonitorDisplayTbl)

public:
	CTunnelMonitorDisplayTbl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTunnelMonitorDisplayTbl();
    BOOL OnInitDialog();
	BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// 对话框数据
	enum { IDD = IDD_TUNNELMONITORDISPLAYTBL };
public:
	CWinThread *m_freshDlgThread;
	HANDLE m_hTblFreshEvent;
	BOOL m_bFresh;
	string m_stationId;
	CString m_strPathHead;
    BOOL m_bMode1;
	BOOL m_bMode2;
	BOOL m_bMode3;
	BOOL m_bMode4;
	int m_cx;
	int m_cy;
	int m_tClrCnt;
	int m_freshTime;
	//
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
	//当前区域
	int m_curArea;
	map<int,stuReaderAreaMapping> m_raMap;
    map<string,stuCardSignMapping> m_csMap;
	//卡信息
	map<string,stuDisCardInfo> m_mapCardInfo;
	//区域卡信息
	map<unsigned char,vector<stuAreaCardInfo>> m_mapAreaCardInfo;
	//部门人数
	map<string,int> m_mapDeptWorkerNum;
	//当前显示到第几个
	map<unsigned char,int> m_posMap;
	//记录考打卡时间
	map<string,time_t> m_mapCardFstTime;
	int m_curTotPos1;
	int m_curTotPos2;
	//
	int m_totCardNum;
	//
	int m_leftPos;
	int m_rightPos;
	//区域和左右洞映射关系
    map<int,string> m_totAreaMap;
	map<string,vector<stuAreaCardInfo>> m_mapTotCardInfo;
	//
	vector<stuTblTitle> m_vecTblTitle;
	//
	int m_Zx;
	int m_Zy;
	map<unsigned char,stuZoneAxis> m_mapZoneAxis;
	string m_strTitName;
	//
	int m_displayType;
	//
	bool m_bUpFresh;
	//
	map<unsigned int,stuAttendanceData> m_mapAttData;
	//
	int m_shiftType;
    int m_shiftTime;
	CCriticalSection m_csPaint;//
	bool m_disLoop;
public:
	BOOL StartFreshDlg();
	void SetData(int n);
    BOOL LoadTblTitleMapping();
	BOOL LoadReaderAreaMapping();
	BOOL LoadAreaTotMapping();
	BOOL LoadCardSignMapping();
	//
	void SetStationId(string station_id){m_stationId = station_id;}
	//
	BOOL LoadPicOrigin();
	//加载区域坐标的映射关系
    BOOL LoadZoneAxisMapping();
	void LoadCardFstTime();
	void SetCardToDisplayData(unsigned int cardno,unsigned char readerno,time_t lsttime,int data_len = 0,const unsigned char *data = NULL);
	void GetDisplayContent();
	void SetM4Display();
	void SetTblM4Content(CStatic* pWnd,int n);
	void SetTblContent(CStatic* pWnd,unsigned char areaNo);
	void SetTitleText();
	void SetTblTitleText(int n,int cnt,int nid);
	void SetTblM4TitleText();
	//汇总区头尾
	void SetTotHeadTail();
	//汇总区
    void SetTotContentA();
	//
	void SetTotContentB();
	//
	void DisplayMonitorInfo();
	//
	void DisplayMonitorInfo_Ex();
	//
	void ParseWatchData(int len,const unsigned char *data,string &desc_str);
	//
	void SetLogoTime();
	//
	void SetLogoM4Time();
	//
	void SetFreshTime(int itime);
	//
	void SetTitleLogo();
	//
	void EraseWinBackground(int w,int h,CDC* pDC);
	//
	bool LoadAttData();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
