#pragma once


// CPoseHisDlg 对话框

class CPoseHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CPoseHisDlg)

public:
	CPoseHisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPoseHisDlg();

// 对话框数据
	enum { IDD = IDD_POSE_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPhQry();
public:
	afx_msg void OnBnClickedBtnPhClose();
};
