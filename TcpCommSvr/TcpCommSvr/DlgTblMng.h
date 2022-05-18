#pragma once
#include "afxcmn.h"

struct stuTblTitle
{
	int item;
	string name;
	string flag;
};
// CDlgTblMng 对话框

class CDlgTblMng : public CDialog
{
	DECLARE_DYNAMIC(CDlgTblMng)

public:
	CDlgTblMng(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTblMng();

    // 对话框数据
	enum { IDD = IDD_DLG_TBL_MNG };
    BOOL OnInitDialog();
	BOOL LoadTblTitleMapping();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	map<int,stuTblTitle> m_mapTblTitle;
public:
	afx_msg void OnBnClickedBtnTblSave();
public:
	CListCtrl m_lstTbl;
public:
	afx_msg void OnLvnItemchangedListTbl1(NMHDR *pNMHDR, LRESULT *pResult);
};
