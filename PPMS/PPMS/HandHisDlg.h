#pragma once


// CHandHisDlg 对话框

class CHandHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CHandHisDlg)

public:
	CHandHisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHandHisDlg();

// 对话框数据
	enum { IDD = IDD_HAND_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
};
