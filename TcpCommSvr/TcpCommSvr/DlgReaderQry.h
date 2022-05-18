#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CDlgReaderQry �Ի���
class CDlgReaderQry : public CDialog
{
	DECLARE_DYNAMIC(CDlgReaderQry)

public:
	CDlgReaderQry(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgReaderQry();

// �Ի�������
	enum { IDD = IDD_DLG_READER_QRY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	int isHex( char ch ); //�ж��ַ�ch�Ƿ�16�����ַ����Ƿ���1���񷵻�0 
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
