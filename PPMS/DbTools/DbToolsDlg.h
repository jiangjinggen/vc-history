// DbToolsDlg.h : 头文件
//

#pragma once
#include "../comm/mystring.h"
#include "mysql.h"
#include "afxcmn.h"
#include "vector"
#include "list"
#include "map"
using namespace std;
//mysql配置参数
struct stuMySqlCfg
{
	unsigned int my_port;
	char my_host[32];
	char my_db[32];
	char my_user[32];
	char my_password[32];
};
// CDbToolsDlg 对话框
class CDbToolsDlg : public CDialog
{
// 构造
public:
	CDbToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DBTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//
	stuMySqlCfg m_mySqlCfg;
    MYSQL m_myCont;
	//
public:
    bool AddOneCard(unsigned int card_no);
	bool AddOnePeople(const char *src);
	bool AddOneWarn(const char *src);
	bool AddOneDevPos(const char *src);
	bool AddOneLblPos(const char *src);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
public:
	CString m_insCardNo;
public:
	CString m_insCardNoBegin;
public:
	CString m_insCardEnd;
public:
	CString m_qryCardNo;
public:
	CListCtrl m_lstCardInfo;
public:
	CListCtrl m_lstInOutInfo;
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnBnClickedButton7();
public:
	CString m_fileName;
public:
	CString m_readCardNo;
public:
	CString m_readCardDate;
public:
	CListCtrl m_lstReadCard;
public:
	afx_msg void OnBnClickedBtnDelReadCardInfo();
public:
	afx_msg void OnBnClickedBtnReadCardQry();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CString m_qryContent;
};
