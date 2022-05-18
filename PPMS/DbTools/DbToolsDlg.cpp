// DbToolsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DbTools.h"
#include "DbToolsDlg.h"
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDbToolsDlg �Ի���




CDbToolsDlg::CDbToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDbToolsDlg::IDD, pParent)
	, m_insCardNo(_T(""))
	, m_insCardNoBegin(_T(""))
	, m_insCardEnd(_T(""))
	, m_qryCardNo(_T(""))
	, m_fileName(_T(""))
	, m_readCardNo(_T(""))
	, m_readCardDate(_T(""))
	, m_qryContent(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDbToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_insCardNo);
	DDX_Text(pDX, IDC_EDIT2, m_insCardNoBegin);
	DDX_Text(pDX, IDC_EDIT3, m_insCardEnd);
	DDX_Text(pDX, IDC_EDIT4, m_qryCardNo);
	DDX_Control(pDX, IDC_LIST1, m_lstCardInfo);
	DDX_Control(pDX, IDC_LIST2, m_lstInOutInfo);
	DDX_Text(pDX, IDC_EDIT5, m_fileName);
	DDX_Text(pDX, IDC_EDIT6, m_readCardNo);
	DDX_Text(pDX, IDC_EDIT7, m_readCardDate);
	DDX_Control(pDX, IDC_LIST4, m_lstReadCard);
	DDX_Text(pDX, IDC_EDIT8, m_qryContent);
}

BEGIN_MESSAGE_MAP(CDbToolsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDbToolsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDbToolsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDbToolsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDbToolsDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDbToolsDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDbToolsDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDbToolsDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BTN_DEL_READ_CARD_INFO, &CDbToolsDlg::OnBnClickedBtnDelReadCardInfo)
	ON_BN_CLICKED(IDC_BTN_READ_CARD_QRY, &CDbToolsDlg::OnBnClickedBtnReadCardQry)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CDbToolsDlg::OnLvnItemchangedList2)
END_MESSAGE_MAP()


// CDbToolsDlg ��Ϣ�������

BOOL CDbToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//
	CString strPath(".\\cfg\\syscfg.ini");
	//
	memset(&m_mySqlCfg,0,sizeof(stuMySqlCfg));
	m_mySqlCfg.my_port = GetPrivateProfileInt("mysql����", "my_port",0,strPath.GetBuffer());
	if(m_mySqlCfg.my_port <= 0)m_mySqlCfg.my_port = 3306;
	GetPrivateProfileString("mysql����", "my_host","",m_mySqlCfg.my_host,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_db","",m_mySqlCfg.my_db,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_user","",m_mySqlCfg.my_user,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_password","",m_mySqlCfg.my_password,256,strPath.GetBuffer());
	/////
	MYSQL *tmpRet = NULL;
	tmpRet = mysql_init(&m_myCont);
	if(NULL == tmpRet)
	{
		MessageBox("mysql���ݿ�����ʧ��1��");
		//SendMessage(WM_CLOSE);
	}
	tmpRet = mysql_real_connect(&m_myCont, m_mySqlCfg.my_host, m_mySqlCfg.my_user, m_mySqlCfg.my_password, m_mySqlCfg.my_db, m_mySqlCfg.my_port, NULL, CLIENT_MULTI_STATEMENTS);
	if(NULL == tmpRet)
	{
		MessageBox("mysql���ݿ�����ʧ��2��");
		//SendMessage(WM_CLOSE);
	}
	//
    DWORD dwStyle = m_lstCardInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCardInfo.SetExtendedStyle(dwStyle);
	
	CRect rect;
	m_lstCardInfo.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstCardInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/12);
    m_lstCardInfo.InsertColumn(1, _T("����"), LVCFMT_LEFT, rect.Width()/3);
    m_lstCardInfo.InsertColumn(2, _T("״̬"), LVCFMT_LEFT, rect.Width()/10);
    m_lstCardInfo.InsertColumn(3, _T("ʱ��"), LVCFMT_LEFT, rect.Width()/3); 
	//m_lstCardInfo.SetRedraw(FALSE);//��ֹ�ػ�
    ////
	dwStyle = m_lstInOutInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstInOutInfo.SetExtendedStyle(dwStyle);

	m_lstInOutInfo.GetClientRect(&rect); 
    m_lstInOutInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/12);
    m_lstInOutInfo.InsertColumn(1, _T("�豸"), LVCFMT_LEFT, rect.Width()/12);
	m_lstInOutInfo.InsertColumn(2, _T("����"), LVCFMT_LEFT, rect.Width()/5);
    m_lstInOutInfo.InsertColumn(3, _T("��ǩ"), LVCFMT_LEFT, rect.Width()/5);
	m_lstInOutInfo.InsertColumn(4, _T("�ϴ�"), LVCFMT_LEFT, rect.Width()/12);
    m_lstInOutInfo.InsertColumn(5, _T("ʱ��"), LVCFMT_LEFT, rect.Width()/4); 
	m_lstInOutInfo.InsertColumn(6, _T("����"), LVCFMT_LEFT, rect.Width()/3); 
    ////
	dwStyle = m_lstReadCard.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstReadCard.SetExtendedStyle(dwStyle);

	m_lstReadCard.GetClientRect(&rect); 
    m_lstReadCard.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/12);
	m_lstReadCard.InsertColumn(1, _T("�豸��"), LVCFMT_LEFT, rect.Width()/8);
    m_lstReadCard.InsertColumn(2, _T("����"), LVCFMT_LEFT, rect.Width()/3);
    m_lstReadCard.InsertColumn(3, _T("״̬"), LVCFMT_LEFT, rect.Width()/10);
    m_lstReadCard.InsertColumn(4, _T("ʱ��"), LVCFMT_LEFT, rect.Width()/3);
	//m_lstInOutInfo.SetRedraw(FALSE);//
	m_fileName = ".\\card\\card.txt";
	UpdateData(FALSE);
	//
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDbToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDbToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CDbToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CDbToolsDlg::AddOneCard(unsigned int card_no)
{
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"insert into pm_db.card_inf (CARD_ID,IO_FLG,NOW_POS,PRE_POS) values ('%010u','0','000','000') ",card_no);
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		return false;
	}
}
//��Ա��Ϣ
bool CDbToolsDlg::AddOnePeople(const char *src)
{
	std::vector<std::string> vStr;
	SpliteStr(src, ",", vStr);
	if (vStr.size() != 6)
	{
		return false;
	}
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"insert into pm_db.card_pepole_inf (CARD_ID,POLICE,PEPOLE_NAME,ID_NO,PHONE_NO,ADDR) values ('%s','%s','%s','%s','%s','%s') ",vStr[0].c_str(),vStr[1].c_str(),vStr[2].c_str(),vStr[3].c_str(),vStr[4].c_str(),vStr[5].c_str());
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		return false;
	}
}
//����������
bool CDbToolsDlg::AddOneWarn(const char *src)
{
	std::vector<std::string> vStr;
	SpliteStr(src, ",", vStr);
	if (vStr.size() != 9)
	{
		return false;
	}
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"insert into pm_db.card_warn_inf (CARD_ID,POS_UP,POS_DOWN,BODY_TEMP_UP,BODY_TEMP_DOWN,HEART_RATE_UP,HEART_RATE_DOWN,HAND_UP,HAND_DOWN) values ('%s','%s','%s','%s','%s','%s','%s','%s','%s') ",vStr[0].c_str(),vStr[1].c_str(),vStr[2].c_str(),vStr[3].c_str(),vStr[4].c_str(),vStr[5].c_str(),vStr[6].c_str(),vStr[7].c_str(),vStr[8].c_str());
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		return false;
	}
}
//������λ��
bool CDbToolsDlg::AddOneDevPos(const char *src)
{
	std::vector<std::string> vStr;
	SpliteStr(src, ",", vStr);
	if (vStr.size() != 2)
	{
		return false;
	}
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"insert into pm_db.dev_pos_inf (DEV_ID,POS_NAME) values ('%s','%s') ",vStr[0].c_str(),vStr[1].c_str());
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		return false;
	}
}
//��ǩλ��
bool CDbToolsDlg::AddOneLblPos(const char *src)
{
	std::vector<std::string> vStr;
	SpliteStr(src, ",", vStr);
	if (vStr.size() != 2)
	{
		return false;
	}
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"insert into pm_db.lbl_pos_inf (LBL_ID,POS_NAME) values ('%s','%s') ",vStr[0].c_str(),vStr[1].c_str());
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		return false;
	}
}
void CDbToolsDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	unsigned int iCardNo = strtoul(m_insCardNo.GetBuffer(),NULL,10);
	bool bol = AddOneCard(iCardNo);
	if(bol)
	{
		MessageBox("���뿨Ƭ�ɹ���");
	}
	else
	{
		MessageBox("���뿨Ƭʧ�ܣ�");
	}
}

void CDbToolsDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	unsigned int iCardNoB = strtoul(m_insCardNoBegin.GetBuffer(),NULL,10);
	unsigned int iCardNoE = strtoul(m_insCardEnd.GetBuffer(),NULL,10);
	int okCnt = 0;
	int failCnt = 0;
	for(unsigned int i = iCardNoB; i <= iCardNoE; i++)
	{
	    bool bol = AddOneCard(i);
		if(bol)
		{
			okCnt++;
		}
		else
		{
			failCnt++;
		}
	}
	CString strTmp;
	strTmp.Format("����ɹ�[%d]�ţ�ʧ��[%d]��",okCnt,failCnt);
	MessageBox(strTmp);
}

void CDbToolsDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char stmp[256];
	int okCnt = 0;
	int failCnt = 0;
	CString strTmp;
	bool bol = false;
	FILE *fp = NULL;
	//���뿨��
	fp = fopen(".\\card\\card.txt","r");
	if(NULL == fp)
	{
		MessageBox("card.txt�ļ������ڣ�");
	}
	else
	{
		okCnt = 0;
		failCnt = 0;
		while(!feof(fp))  
		{  
			memset(stmp, 0, sizeof(stmp));  
			fgets(stmp, sizeof(stmp) - 1, fp); // �����˻��з� 
			if('#' == stmp[0])continue;
			unsigned int iCardNo = strtoul(stmp,NULL,10);
			if(iCardNo <= 0)continue;
			bol = AddOneCard(iCardNo);
			if(bol)
			{
				okCnt++;
			}
			else
			{
				failCnt++;
			}  
		}
		fclose(fp);
		fp = NULL;
		strTmp.Format("��Ƭ����ɹ�[%d]�ţ�ʧ��[%d]��",okCnt,failCnt);
		MessageBox(strTmp);
	}
	//////////������Ա��Ϣ
	////////fp = fopen(".\\card\\people.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("people.txt�ļ������ڣ�");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // �����˻��з� 
	////////		if('#' == stmp[0])continue;
	////////		bol = AddOnePeople(stmp);
	////////		if(bol)
	////////		{
	////////			okCnt++;
	////////		}
	////////		else
	////////		{
	////////			failCnt++;
	////////		}  
	////////	}
	////////	fclose(fp);
	////////	fp = NULL;
	////////	strTmp.Format("��Ա��Ϣ����ɹ�[%d]�ţ�ʧ��[%d]��",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
	//////////���뱨��������
	////////fp = fopen(".\\card\\warn.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("warn.txt�ļ������ڣ�");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // �����˻��з� 
	////////		if('#' == stmp[0])continue;
	////////		bol = AddOneWarn(stmp);
	////////		if(bol)
	////////		{
	////////			okCnt++;
	////////		}
	////////		else
	////////		{
	////////			failCnt++;
	////////		}  
	////////	}
	////////	fclose(fp);
	////////	fp = NULL;
	////////	strTmp.Format("���������޲���ɹ�[%d]�ţ�ʧ��[%d]��",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
	//////////���������λ����Ϣ
	////////fp = fopen(".\\card\\dev_pos.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("dev_pos.txt�ļ������ڣ�");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // �����˻��з� 
	////////		if('#' == stmp[0])continue;
	////////		bol = AddOneDevPos(stmp);
	////////		if(bol)
	////////		{
	////////			okCnt++;
	////////		}
	////////		else
	////////		{
	////////			failCnt++;
	////////		}  
	////////	}
	////////	fclose(fp);
	////////	fp = NULL;
	////////	strTmp.Format("������λ����Ϣ����ɹ�[%d]�ţ�ʧ��[%d]��",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
	//////////�����ǩλ����Ϣ
	////////fp = fopen(".\\card\\lbl_pos.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("lbl_pos.txt�ļ������ڣ�");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // �����˻��з� 
	////////		if('#' == stmp[0])continue;
	////////		bol = AddOneLblPos(stmp);
	////////		if(bol)
	////////		{
	////////			okCnt++;
	////////		}
	////////		else
	////////		{
	////////			failCnt++;
	////////		}  
	////////	}
	////////	fclose(fp);
	////////	fp = NULL;
	////////	strTmp.Format("��ǩλ����Ϣ����ɹ�[%d]�ţ�ʧ��[%d]��",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
}

void CDbToolsDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lstCardInfo.DeleteAllItems();//���
	try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
        int ret = mysql_query(&m_myCont, "select CARD_ID,IO_FLG,NOW_POS,CHG_DATE from pm_db.card_inf order by CARD_ID");//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
				int iCount = 0;
				char stmp[256];
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					CString card_no = m_sqlRow[0];
					CString io_flg = m_sqlRow[1];
					CString room_no = m_sqlRow[2];
					CString pre_room_no = m_sqlRow[2];
					CString chg_time = m_sqlRow[3];
					//
					iCount++;
					//���
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%03d",iCount);
					m_lstCardInfo.InsertItem(iCount-1, _T(stmp));
					//����
					m_lstCardInfo.SetItemText(iCount-1, 1, card_no);
					//
					m_lstCardInfo.SetItemText(iCount-1, 2, io_flg);
					m_lstCardInfo.SetItemText(iCount-1, 3, chg_time);
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ѯ��Ƭʧ��1��");
			//SendMessage(WM_CLOSE);
			//return FALSE;
        }
	}
	catch(_com_error e)
	{
		//
		MessageBox("mysql��ѯ��Ƭʧ��2��");
		//SendMessage(WM_CLOSE);
		//return FALSE;
	}
}

void CDbToolsDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	unsigned int iCardNo = strtoul(m_qryCardNo.GetBuffer(),NULL,10);
	char sCardNo[256];
	memset(sCardNo,0,sizeof(sCardNo));
	sprintf(sCardNo,"%010u",iCardNo);
	m_lstInOutInfo.DeleteAllItems();//���
	try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
		char sSql[512];
		memset(sSql,0,sizeof(sSql));
		sprintf(sSql,"select DEV_ID,CARD_ID,LBL_ID,X,Y,Z,TEMPERATURE,HEART_RATE,HAND,UP_FLG,CHECK_TIME from pm_db.people_health_inf where CARD_ID = '%010u' order by CHG_DATE",iCardNo);
        int ret = mysql_query(&m_myCont, sSql);//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
				int iCount = 0;
				char stmp[256];
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					CString dev_no = m_sqlRow[0];
					CString card_no = m_sqlRow[1];
					CString lbl_no = m_sqlRow[2];
					CString d_x = m_sqlRow[3];
					CString d_y = m_sqlRow[4];
					CString d_z = m_sqlRow[5];
					CString temperature = m_sqlRow[6];
					CString heart_rate = m_sqlRow[7];
					CString hand = m_sqlRow[8];
					CString up_flg = m_sqlRow[9];
					CString check_time = m_sqlRow[10];
					//
					iCount++;
					//���
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%03d",iCount);
					m_lstInOutInfo.InsertItem(iCount-1, _T(stmp));
					//�豸
					m_lstInOutInfo.SetItemText(iCount-1, 1, dev_no);
					//����
					m_lstInOutInfo.SetItemText(iCount-1, 2, card_no);
					//��ǩ
					m_lstInOutInfo.SetItemText(iCount-1, 3, lbl_no);
					//
					m_lstInOutInfo.SetItemText(iCount-1, 4, up_flg);
					m_lstInOutInfo.SetItemText(iCount-1, 5, check_time);
					CString strTmp = "";
					strTmp = d_x + "," + d_y + "," + d_z + "," + temperature + "," + heart_rate + "," + hand;
					//strTmp += d_x + ",";
					//strTmp += d_x + ",";
					m_lstInOutInfo.SetItemText(iCount-1, 6, strTmp);
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ѯ��ˮʧ��1��");
			//SendMessage(WM_CLOSE);
			//return FALSE;
        }
	}
	catch(_com_error e)
	{
		//
		MessageBox("mysql��ѯ��ˮʧ��2��");
		//SendMessage(WM_CLOSE);
		//return FALSE;
	}
}

void CDbToolsDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(::MessageBox(NULL,"ȷ��Ҫɾ�����п�Ƭ��","ɾ����Ƭ",MB_YESNO) != IDYES)return;
	m_lstCardInfo.DeleteAllItems();//���
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	int ret = -1;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	strcpy(sSql,"truncate table pm_db.card_inf ");
	mysql_query(&m_myCont,"start transaction;");
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
		mysql_commit(&m_myCont);
		MessageBox("�����Ƭ��ɹ���");
		//return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		MessageBox("�����Ƭ��ʧ�ܣ�");
		//return false;
	}
	////////�����Ա��Ϣ��
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.card_pepole_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//�ɹ�
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("�����Ա��Ϣ��ɹ���");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//ʧ��
	//////	MessageBox("�����Ա��Ϣ��ʧ�ܣ�");
	//////	//return false;
	//////}
	////////���������������Ϣ��
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.card_warn_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//�ɹ�
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("������������ޱ�ɹ���");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//ʧ��
	//////	MessageBox("������������ޱ�ʧ�ܣ�");
	//////	//return false;
	//////}
	////////���������λ�ñ�
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.dev_pos_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//�ɹ�
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("���������λ�ñ�ɹ���");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//ʧ��
	//////	MessageBox("���������λ�ñ�ʧ�ܣ�");
	//////	//return false;
	//////}
	////////�����ǩλ�ñ�
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.lbl_pos_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//�ɹ�
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("�����ǩλ�ñ�ɹ���");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//ʧ��
	//////	MessageBox("�����ǩλ�ñ�ʧ�ܣ�");
	//////	//return false;
	//////}
}

void CDbToolsDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(::MessageBox(NULL,"ȷ��Ҫɾ��������ˮ��","ɾ����ˮ",MB_YESNO) != IDYES)return;
	m_lstInOutInfo.DeleteAllItems();//���
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	strcpy(sSql,"truncate table pm_db.people_health_inf ");
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		MessageBox("����ɹ���");
		//return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		MessageBox("���ʧ�ܣ�");
		//return false;
	}
}

void CDbToolsDlg::OnBnClickedBtnDelReadCardInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(::MessageBox(NULL,"ȷ��Ҫɾ�����ж�����ˮ��","ɾ����ˮ",MB_YESNO) != IDYES)return;
	m_lstReadCard.DeleteAllItems();//���
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	char sSql[512];
	memset(sSql,0,sizeof(sSql));
	strcpy(sSql,"truncate table pm_db.read_card_inf ");
	mysql_query(&m_myCont,"start transaction;");
	int ret = -1;
	ret = mysql_query(&m_myCont,sSql);
	if(0 == ret)
	{
		//�ɹ�
        //��÷��ز���@t,@t�Ǵ�������
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		MessageBox("����ɹ���");
		//return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//ʧ��
		MessageBox("���ʧ�ܣ�");
		//return false;
	}
}

void CDbToolsDlg::OnBnClickedBtnReadCardQry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	unsigned int iCardNo = strtoul(m_readCardNo.GetBuffer(),NULL,10);
	char sCardNo[256];
	CString d_begin = m_readCardDate + "000000";
	CString d_end =  m_readCardDate + "235959";
	memset(sCardNo,0,sizeof(sCardNo));
	sprintf(sCardNo,"%010u",iCardNo);
	m_lstReadCard.DeleteAllItems();//���
	try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //���ñ����ʽ
		char sSql[512];
		memset(sSql,0,sizeof(sSql));
		//sprintf(sSql,"select READ_ID,CARD_ID,IO_FLG,CHG_DATE from hm_db.read_card_inf where CARD_ID = '%010u'and date_format(CHG_DATE,'%Y%m%d') = '%s' order by CHG_DATE",iCardNo,m_readCardDate.GetBuffer());
		sprintf(sSql,"select READ_ID,CARD_ID,IO_FLG,CHG_DATE from pm_db.read_card_inf where CARD_ID = '%010u'and CHG_DATE between '%s' and '%s' order by CHG_DATE",iCardNo,d_begin.GetBuffer(),d_end.GetBuffer());
        int ret = mysql_query(&m_myCont, sSql);//��ѯ
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
				int iCount = 0;
				char stmp[256];
                while (m_sqlRow = mysql_fetch_row(m_myResult))//��ȡ���������
                {
					CString read_no = m_sqlRow[0];
					CString card_no = m_sqlRow[1];
					CString io_flg = m_sqlRow[2];
					CString chg_time = m_sqlRow[3];
					//
					iCount++;
					//���
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%03d",iCount);
					m_lstReadCard.InsertItem(iCount-1, _T(stmp));
					m_lstReadCard.SetItemText(iCount-1, 1, read_no);
					//����
					m_lstReadCard.SetItemText(iCount-1, 2, card_no);
					//
					m_lstReadCard.SetItemText(iCount-1, 3, io_flg);
					m_lstReadCard.SetItemText(iCount-1, 4, chg_time);
                }
				mysql_free_result(m_myResult);
            }
        }
        else
        {
			MessageBox("mysql��ѯ������ˮʧ��1��");
			//SendMessage(WM_CLOSE);
			//return FALSE;
        }
	}
	catch(_com_error e)
	{
		//
		MessageBox("mysql��ѯ������ˮʧ��2��");
		//SendMessage(WM_CLOSE);
		//return FALSE;
	}
}

void CDbToolsDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION ps;
	int nIndex;
	ps=m_lstInOutInfo.GetFirstSelectedItemPosition();
	nIndex=m_lstInOutInfo.GetNextSelectedItem(ps);
	m_qryContent = m_lstInOutInfo.GetItemText(nIndex,6);
	UpdateData(FALSE);
	*pResult = 0;
}
