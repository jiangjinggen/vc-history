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
//����Ϣ
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
//����Ϣ
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
//������Ϣ
struct stuDisInOutInfo
{
	//char  cardNo[20]
	unsigned char ioFlg;
	char io_time[20];
	time_t lstTime;
};
//
// CTunnelMonitorDisplay �Ի���

class CTunnelMonitorDisplay : public CDialog
{
	DECLARE_DYNAMIC(CTunnelMonitorDisplay)

public:
	CTunnelMonitorDisplay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTunnelMonitorDisplay();
    BOOL OnInitDialog();
	BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// �Ի�������
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
	//Ŀ��뾶
	int m_dstR;
	//�ı����
	int m_txtW;
	//�ı��߶�
	int m_txtH;
	//�ı���Ŀ�����
	int m_dstTxtH;
	//������ʾλ��
	int m_inOutIndex;
	//��ǰ����
	int m_curArea;
	map<int,stuReaderAreaMapping> m_raMap;
    map<string,stuCardSignMapping> m_csMap;
	//����Ϣ
	map<string,stuDisCardInfo> m_mapCardInfo;
	//������Ϣ
	map<string,stuDisInOutInfo> m_mapInOutInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
