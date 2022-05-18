// SafServerAppDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "StationSvr.h"
#include "ClientSvr.h"
#include "QuerySvr.h"
//
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )
//using namespace ADODB;
////mysql配置参数
//struct stuMySqlCfg
//{
//	unsigned int my_port;
//	char my_host[32];
//	char my_db[32];
//	char my_user[32];
//	char my_password[32];
//};
// CSafServerAppDlg 对话框
class CSafServerAppDlg : public CDialog
{
// 构造
public:
	CSafServerAppDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SAFSERVERAPP_DIALOG };

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
	//
	CDataQueue *m_pStationRecvQueue;
	CDataQueue *m_pStationSendQueue;
	CDataQueue *m_pClientRecvQueue;
	CDataQueue *m_pClientSendQueue;
	CDataQueue *m_pQueryRecvQueue;
	CDataQueue *m_pQuerySendQueue;
	CLogMessage *m_pLogMsg;
	CStationSvr m_stationSvr;
	CClientSvr m_clientSvr;
	CQuerySvr m_querySvr;
	//
	bool m_bolStation;
	bool m_bolClient;
	bool m_bolQuery;
	bool m_bolHelp;
	//
	CWinThread *m_stationThread;
	CWinThread *m_clientThread;
	CWinThread *m_queryThread;
	CWinThread *m_helpThread;
	//
	CString m_dbStr;
	int m_svrStationPort;
	int m_svrClientPort;
	int m_svrQueryPort;
	int m_logGrade;
	//
	int m_useDb;
	//
	_ConnectionPtr m_dbConnection;
	//
	stuMySqlCfg m_mySqlCfg;
	//
    map<string,stuNodeInfo> m_mapStationNodeInfo;
	map<string,stuNodeInfo> m_mapClientNodeInfo;
	map<string,stuNodeInfo> m_mapQueryNodeInfo;
public:
	int GBK2UTF8(char *szGbk,char *szUtf8,int Len) ;
	//
    void GetAndSndPhoneQryDataFix(const char *snode);
	void GetAndSndPhoneQryData(const char *snode);
	//
	BOOL StartStationPro();
	//终端数据处理线程
    BOOL StartClientPro();
	//查询数据处理线程
	BOOL StartQueryPro();
	//辅助线程
	BOOL StartHelpPro();
	//读取配置
	void LoadNodeFromIni(string stitle,map<string,stuNodeInfo> &mapNodeInfo,int type);
	//
	bool SetNodeInfo();
	bool SendNodeStatus(string node_id,bool status); 
public:
	CListCtrl m_lstPoint;
public:
	CListCtrl m_lstLog;
public:
	afx_msg void OnDestroy();
};
