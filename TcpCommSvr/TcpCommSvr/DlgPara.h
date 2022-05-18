#pragma once
#include "afxwin.h"


// CDlgPara �Ի���

class CDlgPara : public CDialog
{
	DECLARE_DYNAMIC(CDlgPara)

public:
	CDlgPara(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPara();

// �Ի�������
	enum { IDD = IDD_DLG_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
