#pragma once
#include "TunnelMonitorDisplay.h"
#include "DlgTblMng.h"
// CTunneMonitorDisplayTbl �Ի���
//��������
struct stuZoneAxis
{
	int x;
	int y;
	int w;
	int h;
};
class CTunneMonitorDisplayTbl : public CDialog
{
	DECLARE_DYNAMIC(CTunneMonitorDisplayTbl)

public:
	CTunneMonitorDisplayTbl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTunneMonitorDisplayTbl();
    BOOL OnInitDialog();
	BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
// �Ի�������
	enum { IDD = IDD_DLG_DISPLAY_TBL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CWinThread *m_freshDlgThread;
	HANDLE m_hTblFreshEvent;
	BOOL m_bFresh;
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
	//Ŀ��뾶
	int m_dstR;
	//�ı����
	int m_txtW;
	//�ı��߶�
	int m_txtH;
	//�ı���Ŀ�����
	int m_dstTxtH;
	//��ǰ����
	int m_curArea;
	map<int,stuReaderAreaMapping> m_raMap;
    map<string,stuCardSignMapping> m_csMap;
	//����Ϣ
	map<string,stuDisCardInfo> m_mapCardInfo;
	//������Ϣ
	map<unsigned char,vector<stuAreaCardInfo>> m_mapAreaCardInfo;
	//��������
	map<string,int> m_mapDeptWorkerNum;
	//��ǰ��ʾ���ڼ���
	map<unsigned char,int> m_posMap;
	//��¼����ʱ��
	map<string,time_t> m_mapCardFstTime;
	int m_curTotPos1;
	int m_curTotPos2;
	//
	int m_totCardNum;
	//
	int m_leftPos;
	int m_rightPos;
	//��������Ҷ�ӳ���ϵ
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
	map<unsigned int,stuAttendanceData> m_mapAttData;
public:
	BOOL StartFreshDlg();
	BOOL LoadTblTitleMapping();
	BOOL LoadReaderAreaMapping();
	BOOL LoadAreaTotMapping();
	BOOL LoadCardSignMapping();
	//
	BOOL LoadPicOrigin();
	//�������������ӳ���ϵ
    BOOL LoadZoneAxisMapping();
	void LoadCardFstTime();
	void SetCardToDisplayData(unsigned int cardno,unsigned char readerno,time_t lsttime,int data_len,const unsigned char *data);
	void GetDisplayContent();
	void SetM4Display();
	void SetTblM4Content(CStatic* pWnd,int n);
	void SetTblContent(CStatic* pWnd,unsigned char areaNo);
	void SetTitleText();
	void SetTblTitleText(int n,int cnt,int nid);
	void SetTblM4TitleText();
	//������ͷβ
	void SetTotHeadTail();
	//������
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
	void EraseWinBackground(int w,int h,CDC* pDC);
	//
	bool LoadAttData();
public:
	void ShowDlg();
	afx_msg void OnDestroy();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnPaint();
};
