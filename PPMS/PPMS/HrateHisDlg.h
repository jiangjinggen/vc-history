#pragma once


// CHrateHisDlg �Ի���

class CHrateHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CHrateHisDlg)

public:
	CHrateHisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHrateHisDlg();

// �Ի�������
	enum { IDD = IDD_HRATE_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnHrQry();
public:
	afx_msg void OnBnClickedBtnHrClose();
};
