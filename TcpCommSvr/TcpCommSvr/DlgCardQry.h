#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CDlgCardQry �Ի���

class CDlgCardQry : public CDialog
{
	DECLARE_DYNAMIC(CDlgCardQry)

public:
	CDlgCardQry(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCardQry();

// �Ի�������
	enum { IDD = IDD_DLG_CARD_QRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedBtnCardQry();
public:
	CComboBox m_cbCardNo;
public:
	CListCtrl m_lstReaderNo;
};
