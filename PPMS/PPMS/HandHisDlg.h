#pragma once


// CHandHisDlg �Ի���

class CHandHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CHandHisDlg)

public:
	CHandHisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHandHisDlg();

// �Ի�������
	enum { IDD = IDD_HAND_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
};
