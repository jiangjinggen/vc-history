#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgNewDataQry �Ի���

class CDlgNewDataQry : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewDataQry)

public:
	CDlgNewDataQry(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewDataQry();

// �Ի�������
	enum { IDD = IDD_DLG_NEW_DATA_QRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCardQryNew();
public:
	afx_msg void OnBnClickedBtnRoomQryNew();
public:
	afx_msg void OnBnClickedBtnPointQryNew();
	virtual BOOL OnInitDialog();
public:
	CComboBox m_cmbCardNoNew;
public:
	CString m_strCardNo;
public:
	CComboBox m_cmbRoomNo;
public:
	CString m_strRoomNo;
public:
	CComboBox m_cmbPointNo;
public:
	CString m_strPointNo;
public:
	CListCtrl m_lstCardInfo;
public:
	CListCtrl m_lstRoomInfo;
public:
	CListCtrl m_lstPointInfo;
};
