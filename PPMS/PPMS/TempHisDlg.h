#pragma once


// CTempHisDlg �Ի���

class CTempHisDlg : public CDialog
{
	DECLARE_DYNAMIC(CTempHisDlg)

public:
	CTempHisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTempHisDlg();

// �Ի�������
	enum { IDD = IDD_TEMP_HIS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnThQry();
public:
	afx_msg void OnBnClickedBtnThClose();
};
