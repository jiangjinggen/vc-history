#pragma once
#include "afxwin.h"


// CDlgPara 对话框

class CDlgPara : public CDialog
{
	DECLARE_DYNAMIC(CDlgPara)

public:
	CDlgPara(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPara();

// 对话框数据
	enum { IDD = IDD_DLG_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CString m_svrIp;
public:
	int m_svrPort;
	int m_freshTime;
	int m_logGrade;
public:
	CComboBox m_cbFreshTime;
public:
	CComboBox m_cbLogGrade;
};
