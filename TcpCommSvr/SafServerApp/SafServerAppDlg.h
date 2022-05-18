// SafServerAppDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "StationSvr.h"
#include "ClientSvr.h"
#include "QuerySvr.h"
//
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )
//using namespace ADODB;
////mysql���ò���
//struct stuMySqlCfg
//{
//	unsigned int my_port;
//	char my_host[32];
//	char my_db[32];
//	char my_user[32];
//	char my_password[32];
//};
// CSafServerAppDlg �Ի���
class CSafServerAppDlg : public CDialog
{
// ����
public:
	CSafServerAppDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SAFSERVERAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
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
	//�ն����ݴ����߳�
    BOOL StartClientPro();
	//��ѯ���ݴ����߳�
	BOOL StartQueryPro();
	//�����߳�
	BOOL StartHelpPro();
	//��ȡ����
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
