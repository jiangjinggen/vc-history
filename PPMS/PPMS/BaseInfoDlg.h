#pragma once


// CBaseInfoDlg 对话框

class CBaseInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBaseInfoDlg)

public:
	CBaseInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseInfoDlg();

// 对话框数据
	enum { IDD = IDD_BASE_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnModInfo();
public:
	afx_msg void OnBnClickedBtnInfoOk();
public:
	afx_msg void OnBnClickedBtnInfoClose();
};
