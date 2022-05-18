#pragma once


// CTempHisDlg 对话框

class CTempHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CTempHisDlg)

public:
	CTempHisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTempHisDlg();

// 对话框数据
	enum { IDD = IDD_TEMP_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnThQry();
public:
	afx_msg void OnBnClickedBtnThClose();
};
