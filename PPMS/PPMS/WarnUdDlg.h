#pragma once


// CWarnUdDlg �Ի���

class CWarnUdDlg : public CDialog
{
	DECLARE_DYNAMIC(CWarnUdDlg)

public:
	CWarnUdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWarnUdDlg();

// �Ի�������
	enum { IDD = IDD_WARN_UD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUdMod();
public:
	afx_msg void OnBnClickedBtnUdOk();
public:
	afx_msg void OnBnClickedBtnUdClose();
};
