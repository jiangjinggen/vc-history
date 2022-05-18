#pragma once
#include "afxcmn.h"
#include "TunnelMonitorDisplay.h"

// CDlgZoneMng 对话框

class CDlgZoneMng : public CDialog
{
	DECLARE_DYNAMIC(CDlgZoneMng)

public:
	CDlgZoneMng(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgZoneMng();

// 对话框数据
	enum { IDD = IDD_DLG_ZONE_MNG };
    BOOL LoadReaderAreaMapping();
	BOOL LoadAreaTotMapping();
	BOOL OnInitDialog();
public:
	map<int,stuReaderAreaMapping> m_raMap;
	//区域和左右洞映射关系
    map<int,string> m_totAreaMap;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
