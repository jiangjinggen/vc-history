#pragma once
#include "afxcmn.h"
#include "TunnelMonitorDisplay.h"

// CDlgZoneMng �Ի���

class CDlgZoneMng : public CDialog
{
	DECLARE_DYNAMIC(CDlgZoneMng)

public:
	CDlgZoneMng(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgZoneMng();

// �Ի�������
	enum { IDD = IDD_DLG_ZONE_MNG };
    BOOL LoadReaderAreaMapping();
	BOOL LoadAreaTotMapping();
	BOOL OnInitDialog();
public:
	map<int,stuReaderAreaMapping> m_raMap;
	//��������Ҷ�ӳ���ϵ
    map<int,string> m_totAreaMap;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnZoneSave();
public:
	afx_msg void OnBnClickedBtnZoneUpd();
public:
	CString m_zoneNo;
public:
	CString m_zoneName;
public:
	CListCtrl m_lstZone;
public:
	afx_msg void OnHdnItemchangedListZone(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnItemchangedListZone(NMHDR *pNMHDR, LRESULT *pResult);
};
