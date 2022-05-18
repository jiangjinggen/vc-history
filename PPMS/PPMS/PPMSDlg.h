// PPMSDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "../comm/ListCtrlCl.h"
#include "NetTcpTrans.h"
#include "../comm/CommDefine.h"
#include "mysql.h"
//
#include "BaseInfoDlg.h"
#include "HandHisDlg.h"
#include "HrateHisDlg.h"
#include "PoseHisDlg.h"
#include "TempHisDlg.h"
#include "WarnUdDlg.h"
//人员实时信息
struct stuCardRtInfo
{
	unsigned int card_no;
	unsigned char dev_id;
	unsigned int lbl_id;
	unsigned char pose;
	double body_temp;
	double heart_rate;
	unsigned int hand; //是否脱手
	time_t lst_time;
};
//人员基本信息
struct stuCardBaseInfo
{
	unsigned int card_no;
	string police;
	string name;
	string id_no;
	string phone_no;
	string addr;
};
//特征值报警上下限
struct stuCardWarnInfo
{
	unsigned int card_no;
	unsigned char pose_up;
	unsigned char pose_down;
	double body_temp_up;
	double body_temp_down;
	double heart_rate_up;
	double heart_rate_down;
	unsigned char hand_up;
	unsigned char hand_down;
};
//设备位置信息
struct stuDevPosInfo
{
	unsigned int dev_id;
	string name;
};
//标签位置信息
struct stuLblPosInfo
{
	unsigned int lbl_id;
	string name;
};
//
// CPPMSDlg dialog
class CPPMSDlg : public CDialog
{
// Construction
public:
	CPPMSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PPMS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CCriticalSection m_csElder;//
	int m_cx;
	int m_cy;
	HANDLE m_hConnFreshEvent;
	CDataQueue *m_pDataQueue;
	CNetTcpTrans m_netTcpSvr;
	CLogMessage *m_pLogMsg;
	CWinThread *m_logThread;
    CWinThread *m_dataThread;
	CWinThread *m_dbThread;
	CWinThread *m_helperThread;
	BOOL m_bolLog;
	BOOL m_bolData;
	BOOL m_bolDb;
	BOOL m_bolHelper;
	//
	CString m_svrIp;
	int m_svrPort;
	int m_freshTime;
	int m_logGrade;
	//
	int m_protocolType;
	int m_useDb;
	//
	stuMySqlCfg m_mySqlCfg;
    MYSQL m_myCont;
	//
	int m_connType;
	map<unsigned int, stuCardInOutStatus> m_mapCardInOutStatus;
	map<unsigned int, stuCardRtInfo> m_mapCardRtInfo;
	vector<stuElderInfo> m_vecElderInfo;
	map<unsigned int,stuCardBaseInfo> m_mapPeopleInfo;
	map<unsigned int,stuCardWarnInfo> m_mapWarnInfo;
	map<unsigned int,stuDevPosInfo> m_mapDevPosInfo;
	map<unsigned int,stuLblPosInfo> m_mapLblPosInfo;
public:
	void InitHead();
	void InitBody();
	void DisplayMonitorUnit(int i, int j,unsigned int card_no);
	void DisWarnZone_text(int m,int n,unsigned int card_no,CRect rect,CDC* pControlDC);
	void DisWarnZone(int m,int n,CRect rect,CDC* pControlDC);
	int GetPointPos(CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual BOOL DestroyWindow();
	void ParseWatchData(int len,const unsigned char *data,string &desc_str,stuElderInfo &elderInfo);
	void PutElderInfo(stuElderInfo elderInfo);
	void GetAllElderInfo(vector<stuElderInfo> &vecElderInfo);
	BOOL UpdateElderInfo_DB();
	BOOL UpdateElderInfo_MSDB();
	BOOL StartLogPro();
	BOOL StartDataPro();
	BOOL StartDbFresh();
	BOOL StartHelper();
	BOOL AddLogMsg(int grade, const char *sbody);
	BOOL WriteLogMsg(stuLogContent msg);
	BOOL LoadCardFromMSDb();
	BOOL LoadPeopleFromMSDb();
	BOOL LoadWarnFromMSDb();
	BOOL LoadDevPosFromMSDb();
	BOOL LoadLblPosFromMSDb();
	BOOL LoadPeopleFromIni();
	BOOL LoadWarnFromIni();
	BOOL LoadDevPosFromIni();
	BOOL LoadLblPosFromIni();
public:
	//CListCtrl m_lstDev;
	CListCtrlCl m_lstDev;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
