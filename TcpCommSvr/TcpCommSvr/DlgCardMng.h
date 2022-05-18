#pragma once
#include "afxcmn.h"

#include "TunnelMonitorDisplay.h"
// CDlgCardMng 对话框

class CDlgCardMng : public CDialog
{
	DECLARE_DYNAMIC(CDlgCardMng)

public:
	CDlgCardMng(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCardMng();

// 对话框数据
	enum { IDD = IDD_DLG_CARD_MNG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();
	BOOL LoadCardSignMapping();
	BOOL SetListCard();
public:
	map<int,stuReaderAreaMapping> m_raMap;
    map<string,stuCardSignMapping> m_csMap;
public:
	CString m_cardNo;
public:
	CString m_workNo;
public:
	CString m_workName;
public:
	CString m_workType;
public:
	CString m_deptName;
public:
	CString m_workPos;
public:
	CListCtrl m_lstCard;
public:
	afx_msg void OnBnClickedBtnCardAdd();
public:
	afx_msg void OnBnClickedBtnCardDel();
public:
	afx_msg void OnBnClickedBtnCardUpd();
public:
	afx_msg void OnHdnItemchangedListCard(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnLvnItemchangedListCard(NMHDR *pNMHDR, LRESULT *pResult);
};
