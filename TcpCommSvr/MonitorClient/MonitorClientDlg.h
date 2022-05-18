// MonitorClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "NetTcpTransClient.h"
//#include "../MonitorDisplay/TunnelMonitorDisplayTbl.h"
typedef void (*hzpDllShowDlg)(string station_id);
typedef void (*hzpDllSetData)(unsigned int cardno,unsigned char readerno,time_t lsttime,int data_len,const unsigned char *data);
// CMonitorClientDlg 对话框
class CMonitorClientDlg : public CDialog
{
// 构造
public:
	CMonitorClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MONITORCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	CNetTcpTransClient m_tcpTransClient;
	HINSTANCE  m_hzDll;
	hzpDllShowDlg m_hzpShowDlg;
	hzpDllSetData m_hzpSetData;
    CWinThread *m_dataThread;
	CWinThread *m_helpThread;
	bool m_bDataPro;
	bool m_bolHelp;
	int m_logGrade;
	map<string,stuNodeInfo> m_mapStationNodeInfo;
	CDataQueue *m_pClientRecvQueue;
	CDataQueue *m_pClientSendQueue;
	CLogMessage *m_pLogMsg;
	string m_stationId;
	string m_clientId;
	string m_svrId;
	string m_svrIp;
	int    m_svrPort;
public:
	BOOL StartDataPro();
	BOOL StartHelpPro();
	void LoadNodeFromIni(string stitle,map<string,stuNodeInfo> &mapNodeInfo,int type);
	bool SetNodeInfo();
public:
	CListCtrl m_lstStation;
public:
	CListCtrl m_lstLog;
public:
	afx_msg void OnNMDblclkListStation(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedBtnSelStation();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnLvnItemchangedListStation(NMHDR *pNMHDR, LRESULT *pResult);
};
