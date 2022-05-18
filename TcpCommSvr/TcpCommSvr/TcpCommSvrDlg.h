// TcpCommSvrDlg.h : 头文件
//
#pragma once
#include "afxcmn.h"
#include "NetTcpTrans.h"
#include "NetTcpTransClient.h"
#include "DlgPara.h"
#include "DlgReaderQry.h"
#include "DlgCardQry.h"
#include "TunnelMonitorDisplay.h"
#include "TunneMonitorDisplayTbl.h"
#include "ParaMng.h"
//#include "mysql.h"
#include "SerialTrans.h"
//
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )
//using namespace ADODB;
typedef void (*hzpDllShowDlg)(string station_id);
typedef void (*hzpDllSetData)(unsigned int cardno,unsigned char readerno,time_t lsttime,int data_len,const unsigned char *data);
// CTcpCommSvrDlg 对话框
class CTcpCommSvrDlg : public CDialog
{
// 构造
public:
	CTcpCommSvrDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 对话框数据
	enum { IDD = IDD_TCPCOMMSVR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	HANDLE m_hConnFreshEvent;
	CCriticalSection m_csInOut;//
	CDataQueue *m_pDataQueue;
	CNetTcpTrans m_netTcpSvr;
	CNetTcpTransClient m_netTcpClient;
	CSerialTrans m_serialTrans;
	CLogMessage *m_pLogMsg;
	CWinThread *m_logThread;
    CWinThread *m_dataThread;
	CWinThread *m_dbThread;
	CWinThread *m_helperThread;
	BOOL m_bolLog;
	BOOL m_bolData;
	BOOL m_bolDb;
	BOOL m_bolHelper;
	CDlgPara m_dlgPara;
	//
	CDlgReaderQry m_dlgReaderQry;
	//
	CDlgCardQry m_dlgCardQry;
	//
	//CTunnelMonitorDisplay m_dlgTmd;
	//CTunneMonitorDisplayTbl m_dlgTblTmd;
    HINSTANCE  m_hzDll;
	hzpDllShowDlg m_hzpShowDlg;
	hzpDllSetData m_hzpSetData;
	//
	CString m_svrIp;
	CString m_dbStr;
	int m_svrPort;
	int m_freshTime;
	double m_freshRatio;
	int m_logGrade;
	//
	int m_protocolType;
	int m_useDb;
	//
	int m_runMode;
	CString m_remoteIp;
	int m_remotePort;
	//
	stuMySqlCfg m_mySqlCfg;
    //MYSQL m_myCont;
	//
	int m_connType;
	//读卡信息
	map<unsigned int,map<unsigned int,stuCardInfo>> m_mapReaderCardInfo;
	map<unsigned int,vector<stuCardInfo>> m_mapReaderCardInfoVec;
	map<unsigned int,map<unsigned int,stuCardInfo>> m_mapCardReaderInfo;
	map<unsigned int,vector<stuReaderInfo>> m_mapCardReaderInfoVec;
	//
	map<unsigned int,time_t> m_mapCardTimeInfo;
	//出入库信息
	vector<stuInOutInfo> m_vecInOutInfo;
	//
	_ConnectionPtr m_dbConnection;
	CParaMng m_pmDlg;
	bool m_pmDlgOk; 
	////////////新协议
	//激活器、房间信息
	map <unsigned int, stuPointRoomInfo> m_mapPointRoomInfo;
	//房间号、房间名称
	map<unsigned int, string> m_mapRoomNoName;
	//卡、激活器map
	map < unsigned int, map<unsigned int,stuPointInfo>> m_mapCardPointInfo;
	//激活器、卡map
	map < unsigned int, map<unsigned int, stuPointInfo>> m_mapPointCardInfo;
	//房间、卡map
	map<unsigned int, vector<unsigned int>> m_mapRoomCardInfoVec;
	//卡、房间map
	map< unsigned int, stuRoomInfo> m_mapCardRoomInfo;
	//激活器卡片绑定信息
	map<unsigned int,stuPointCardBind> m_mapPoindCardBindInfo;
	//卡片在库状态
    map<unsigned int, stuCardInOutStatus> m_mapCardInOutStatus;
	//激活器进出集合
	map<unsigned int,stuInOutPointInfo> m_mapPointInOutInfo;
	//读卡器进出集合
	map<unsigned int,stuInOutDevInfo> m_mapDevInOutInfo;
	//
	stuStationPara m_stuStationPara;
	//
	map<unsigned int,stuAttendanceData> m_mapAttData;
    //////////
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_isRecordData;
public:
	CListCtrl m_connInfo;
public:
	CListCtrl m_commInfo;
public:
	afx_msg void OnBnClickedBtnStart();
public:
	afx_msg void OnBnClickedBtnStop();
public:
	afx_msg void OnBnClickedBtnPara();
public:
	afx_msg void OnBnClickedBtnReaderQry();
public:
	afx_msg void OnBnClickedBtnCardQry();
    virtual BOOL DestroyWindow();
	BOOL StartLogPro();
	BOOL StartDataPro();
	BOOL StartDbFresh();
	BOOL StartHelper();
	void PutInOutInfo(stuInOutInfo inOutInfo);
	void GetAllInOutInfo(vector<stuInOutInfo> &vecInOutInfo);
	void GetCardInfo(int readerNo,map<unsigned int,stuCardInfo> &mapCardInfo);
	void GetCardInfoVec(int readerNo,vector<stuCardInfo> &vecCardInfo);
	void GetReaderInfo(unsigned int cardNo,map<unsigned int,stuCardInfo> &mapReaderInfo);
	void GetReaderInfoVec(unsigned int cardNo,vector<stuReaderInfo> &vecReaderInfo);
	BOOL FindSockInfoByReaderNo(int readerNo,stuSockInfo &sockInfo);
	void SendCmdInOut(int readerNo);
	BOOL FreshCardInfoDB();
	BOOL FreshInOutInfoDB();
	BOOL AddLogMsg(int grade, const char *sbody);
	void DelMapTest(int key);
	//
	BOOL FreshCardInfoDB_New();
	BOOL FreshInOutInfoDB_New();
	BOOL StartDbFresh_New();
	//
	BOOL UpdateCardInfo_DB();
	BOOL UpdateInOutInfo_DB();
	BOOL UpdateCardInfo_SSDB(int cardCount,const char *cardBuf,const char *preroomBuf,const char *roomBuf);
	BOOL UpdateInOutInfo_SSDB(int cardCount,const char *cardBuf,const char *timeBuf,const char *flagBuf);
	BOOL UpdateCardInfo_MSDB(int cardCount,const char *cardBuf,const char *timeBuf,const char *preroomBuf,const char *roomBuf);
	BOOL UpdateInOutInfo_MSDB(int cardCount,const char *cardBuf,const char *timeBuf,const char *flagBuf);
	//
	void StrToHex(char *pbDest, char *pbSrc, int nLen);
    BOOL LoadPointRoomMapping();
	BOOL LoadPointInOutMapping();
	BOOL LoadDevInOutMapping();
	BOOL LoadPointCardBindMapping();
	BOOL GetCardPointInfo(unsigned int cardNo, map<unsigned int, stuPointInfo> &mapPointInfo);
	//号查卡、房间信息
	BOOL GetCardRoomInfo(map< unsigned int, stuRoomInfo> &mapCardRoomInfo);
	//通过房间号查该房间下的卡信息
	BOOL GetRoomCardInfo(unsigned int roomNo, map< unsigned int, stuRoomInfo> &mapCardRoomInfo);
	//通过激活器查该激活器下的卡信息
	BOOL GetPointCardInfo(unsigned int pointNo, map<unsigned int, stuPointInfo> &mapCardInfo);
	//
	void LoadCardFromIni();
	BOOL LoadCardFromSSDb();
	BOOL LoadCardFromMSDb();
	bool LoadAttendanceData();
	void SetAttendanceData(unsigned int card_no, time_t lst_time);
public:
	afx_msg void OnBnClickedBtnTmdStart();
public:
	afx_msg void OnBnClickedBtnParaMng();
public:
	afx_msg void OnBnClickedBtnInfoQry();
public:
	afx_msg void OnBnClickedBtnStartConnect();
public:
	afx_msg void OnBnClickedBtnStopConnect();
public:
	afx_msg void OnBnClickedBtnStartCom();
public:
	afx_msg void OnBnClickedBtnStopCom();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
