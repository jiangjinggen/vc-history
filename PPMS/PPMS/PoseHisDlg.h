#pragma once


// CPoseHisDlg �Ի���

class CPoseHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CPoseHisDlg)

public:
	CPoseHisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPoseHisDlg();

// �Ի�������
	enum { IDD = IDD_POSE_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPhQry();
public:
	afx_msg void OnBnClickedBtnPhClose();
};
