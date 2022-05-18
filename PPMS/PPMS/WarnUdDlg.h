#pragma once


// CWarnUdDlg 对话框

class CWarnUdDlg : public CDialog
{
	DECLARE_DYNAMIC(CWarnUdDlg)

public:
	CWarnUdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWarnUdDlg();

// 对话框数据
	enum { IDD = IDD_WARN_UD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUdMod();
public:
	afx_msg void OnBnClickedBtnUdOk();
public:
	afx_msg void OnBnClickedBtnUdClose();
};
