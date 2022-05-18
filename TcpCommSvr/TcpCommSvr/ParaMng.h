#pragma once
#include "afxcmn.h"
#include "DlgUserMng.h"
#include "DlgDevMng.h"
#include "DlgZoneMng.h"
#include "DlgCardMng.h"
#include "DlgTblMng.h"

// CParaMng 对话框

class CParaMng : public CDialog
{
	DECLARE_DYNAMIC(CParaMng)

public:
	CParaMng(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaMng();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 对话框数据
	enum { IDD = IDD_DLG_PARA_MNG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();
public:
	CDlgUserMng m_dlgUserMng;
	CDlgDevMng m_dlgDevMng;
	CDlgZoneMng m_dlgZoneMng;
	CDlgCardMng m_dlgCardMng;
	CDlgTblMng m_dlgTblMng;
public:
	CTreeCtrl m_treeParaMng;
public:
	afx_msg void OnNMClickTreePara(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkTreePara(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTvnSelchangedTreePara(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CTabCtrl m_tabTab1;
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnClose();
};
