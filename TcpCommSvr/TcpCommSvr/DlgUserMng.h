#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgUserMng �Ի���
struct stuUserData
{
	string username;
	string pswd;
	int    grade;
};

class CDlgUserMng : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserMng)

public:
	CDlgUserMng(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUserMng();

// �Ի�������
	enum { IDD = IDD_DLG_USER_MNG };
    BOOL OnInitDialog();
	BOOL LoadUserData();
	void Encrypt(char *dest, const char *src, int len);
	void Decrypt(char *dest, const char *src, int len);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	map<string,stuUserData> m_mapUserData;
public:
	CListCtrl m_lstUser;
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	CString m_userName;
public:
	CString m_passWord;
public:
	CComboBox m_cmbGrade;
public:
	afx_msg void OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedBtnUserAdd();
public:
	afx_msg void OnBnClickedBtnUserDel();
public:
	afx_msg void OnBnClickedBtnUserUpd();
public:
	CString m_cmbGradeTxt;
};
