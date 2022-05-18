#pragma once
#include "afxcmn.h"
#include "TunnelMonitorDisplay.h"

// CDlgDevMng �Ի���

class CDlgDevMng : public CDialog
{
	DECLARE_DYNAMIC(CDlgDevMng)

public:
	CDlgDevMng(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDevMng();

// �Ի�������
	enum { IDD = IDD_DLG_DEV_MNG };
    BOOL LoadReaderAreaMapping();
	BOOL OnInitDialog();
public:
	map<int,stuReaderAreaMapping> m_raMap;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDevAdd();
public:
	afx_msg void OnBnClickedBtnDevDel();
public:
	afx_msg void OnBnClickedBtnDevUpd();
public:
	CString m_devNo;
public:
	CString m_zoneNo;
public:
	CString m_zoneName;
public:
	CListCtrl m_lstDev;
public:
	afx_msg void OnLvnItemchangedListDev(NMHDR *pNMHDR, LRESULT *pResult);
};
