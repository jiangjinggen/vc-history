#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CDlgReaderQry 对话框
class CDlgReaderQry : public CDialog
{
	DECLARE_DYNAMIC(CDlgReaderQry)

public:
	CDlgReaderQry(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgReaderQry();

// 对话框数据
	enum { IDD = IDD_DLG_READER_QRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedBtnReaderQry();
public:
	CComboBox m_cbReaderNo;
public:
	CListCtrl m_lstCardNo;
public:
	CString m_sendCmd;
	map<string,int> m_mapLenCmd;
public:
	afx_msg void OnBnClickedBtnSend();
	void StrToHex(char *pbDest, char *pbSrc, int nLen);
	int isHex( char ch ); //判断字符ch是否16进制字符，是返回1，否返回0 
	BOOL LoadLenCmdMapping();
	BOOL LoadFName();
	int PackCmdBuf(char *sndBuf,CString strDevNo,CString strLen);
	void ParseWatchData(int len,const unsigned char *data,stuWatchInfo &stuWI);
public:
	CComboBox m_cbLength;
public:
	CComboBox m_cbFName;
public:
	CListCtrl m_lstCmd;
public:
	afx_msg void OnBnClickedBtnSaveOne();
public:
	afx_msg void OnBnClickedBtnSendOne();
public:
	afx_msg void OnBnClickedLoadDoc();
public:
	afx_msg void OnBnClickedBtnSaveAll();
public:
	afx_msg void OnBnClickedBtnSendAll();
public:
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnCbnSelchangeCombo3();
public:
	afx_msg void OnCbnSelendokCombo3();
};
