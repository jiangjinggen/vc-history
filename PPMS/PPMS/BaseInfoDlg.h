#pragma once


// CBaseInfoDlg �Ի���

class CBaseInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBaseInfoDlg)

public:
	CBaseInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBaseInfoDlg();

// �Ի�������
	enum { IDD = IDD_BASE_INFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnModInfo();
public:
	afx_msg void OnBnClickedBtnInfoOk();
public:
	afx_msg void OnBnClickedBtnInfoClose();
};
