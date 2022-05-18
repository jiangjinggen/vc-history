// DbToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DbTools.h"
#include "DbToolsDlg.h"
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDbToolsDlg 对话框




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


// CDbToolsDlg 消息处理程序

BOOL CDbToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//
	CString strPath(".\\cfg\\syscfg.ini");
	//
	memset(&m_mySqlCfg,0,sizeof(stuMySqlCfg));
	m_mySqlCfg.my_port = GetPrivateProfileInt("mysql配置", "my_port",0,strPath.GetBuffer());
	if(m_mySqlCfg.my_port <= 0)m_mySqlCfg.my_port = 3306;
	GetPrivateProfileString("mysql配置", "my_host","",m_mySqlCfg.my_host,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql配置", "my_db","",m_mySqlCfg.my_db,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql配置", "my_user","",m_mySqlCfg.my_user,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql配置", "my_password","",m_mySqlCfg.my_password,256,strPath.GetBuffer());
	/////
	MYSQL *tmpRet = NULL;
	tmpRet = mysql_init(&m_myCont);
	if(NULL == tmpRet)
	{
		MessageBox("mysql数据库连接失败1！");
		//SendMessage(WM_CLOSE);
	}
	tmpRet = mysql_real_connect(&m_myCont, m_mySqlCfg.my_host, m_mySqlCfg.my_user, m_mySqlCfg.my_password, m_mySqlCfg.my_db, m_mySqlCfg.my_port, NULL, CLIENT_MULTI_STATEMENTS);
	if(NULL == tmpRet)
	{
		MessageBox("mysql数据库连接失败2！");
		//SendMessage(WM_CLOSE);
	}
	//
    DWORD dwStyle = m_lstCardInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstCardInfo.SetExtendedStyle(dwStyle);
	
	CRect rect;
	m_lstCardInfo.GetClientRect(&rect); //获得当前客户区信息 
    m_lstCardInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/12);
    m_lstCardInfo.InsertColumn(1, _T("卡号"), LVCFMT_LEFT, rect.Width()/3);
    m_lstCardInfo.InsertColumn(2, _T("状态"), LVCFMT_LEFT, rect.Width()/10);
    m_lstCardInfo.InsertColumn(3, _T("时间"), LVCFMT_LEFT, rect.Width()/3); 
	//m_lstCardInfo.SetRedraw(FALSE);//防止重绘
    ////
	dwStyle = m_lstInOutInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstInOutInfo.SetExtendedStyle(dwStyle);

	m_lstInOutInfo.GetClientRect(&rect); 
    m_lstInOutInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/12);
    m_lstInOutInfo.InsertColumn(1, _T("设备"), LVCFMT_LEFT, rect.Width()/12);
	m_lstInOutInfo.InsertColumn(2, _T("卡号"), LVCFMT_LEFT, rect.Width()/5);
    m_lstInOutInfo.InsertColumn(3, _T("标签"), LVCFMT_LEFT, rect.Width()/5);
	m_lstInOutInfo.InsertColumn(4, _T("上传"), LVCFMT_LEFT, rect.Width()/12);
    m_lstInOutInfo.InsertColumn(5, _T("时间"), LVCFMT_LEFT, rect.Width()/4); 
	m_lstInOutInfo.InsertColumn(6, _T("内容"), LVCFMT_LEFT, rect.Width()/3); 
    ////
	dwStyle = m_lstReadCard.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstReadCard.SetExtendedStyle(dwStyle);

	m_lstReadCard.GetClientRect(&rect); 
    m_lstReadCard.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/12);
	m_lstReadCard.InsertColumn(1, _T("设备号"), LVCFMT_LEFT, rect.Width()/8);
    m_lstReadCard.InsertColumn(2, _T("卡号"), LVCFMT_LEFT, rect.Width()/3);
    m_lstReadCard.InsertColumn(3, _T("状态"), LVCFMT_LEFT, rect.Width()/10);
    m_lstReadCard.InsertColumn(4, _T("时间"), LVCFMT_LEFT, rect.Width()/3);
	//m_lstInOutInfo.SetRedraw(FALSE);//
	m_fileName = ".\\card\\card.txt";
	UpdateData(FALSE);
	//
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDbToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
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
		//成功
        //获得返回参数@t,@t是传出参数
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
		//失败
		return false;
	}
}
//人员信息
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
		//成功
        //获得返回参数@t,@t是传出参数
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
		//失败
		return false;
	}
}
//报警上下限
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
		//成功
        //获得返回参数@t,@t是传出参数
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
		//失败
		return false;
	}
}
//读卡器位置
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
		//成功
        //获得返回参数@t,@t是传出参数
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
		//失败
		return false;
	}
}
//标签位置
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
		//成功
        //获得返回参数@t,@t是传出参数
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
		//失败
		return false;
	}
}
void CDbToolsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	unsigned int iCardNo = strtoul(m_insCardNo.GetBuffer(),NULL,10);
	bool bol = AddOneCard(iCardNo);
	if(bol)
	{
		MessageBox("插入卡片成功！");
	}
	else
	{
		MessageBox("插入卡片失败！");
	}
}

void CDbToolsDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
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
	strTmp.Format("插入成功[%d]张，失败[%d]张",okCnt,failCnt);
	MessageBox(strTmp);
}

void CDbToolsDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	char stmp[256];
	int okCnt = 0;
	int failCnt = 0;
	CString strTmp;
	bool bol = false;
	FILE *fp = NULL;
	//插入卡号
	fp = fopen(".\\card\\card.txt","r");
	if(NULL == fp)
	{
		MessageBox("card.txt文件不存在！");
	}
	else
	{
		okCnt = 0;
		failCnt = 0;
		while(!feof(fp))  
		{  
			memset(stmp, 0, sizeof(stmp));  
			fgets(stmp, sizeof(stmp) - 1, fp); // 包含了换行符 
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
		strTmp.Format("卡片插入成功[%d]张，失败[%d]张",okCnt,failCnt);
		MessageBox(strTmp);
	}
	//////////插入人员信息
	////////fp = fopen(".\\card\\people.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("people.txt文件不存在！");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // 包含了换行符 
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
	////////	strTmp.Format("人员信息插入成功[%d]张，失败[%d]张",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
	//////////插入报警上下限
	////////fp = fopen(".\\card\\warn.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("warn.txt文件不存在！");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // 包含了换行符 
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
	////////	strTmp.Format("报警上下限插入成功[%d]张，失败[%d]张",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
	//////////插入读卡器位置信息
	////////fp = fopen(".\\card\\dev_pos.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("dev_pos.txt文件不存在！");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // 包含了换行符 
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
	////////	strTmp.Format("读卡器位置信息插入成功[%d]张，失败[%d]张",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
	//////////插入标签位置信息
	////////fp = fopen(".\\card\\lbl_pos.txt","r");
	////////if(NULL == fp)
	////////{
	////////	MessageBox("lbl_pos.txt文件不存在！");
	////////}
	////////else
	////////{
	////////	okCnt = 0;
	////////	failCnt = 0;
	////////	while(!feof(fp))  
	////////	{  
	////////		memset(stmp, 0, sizeof(stmp));  
	////////		fgets(stmp, sizeof(stmp) - 1, fp); // 包含了换行符 
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
	////////	strTmp.Format("标签位置信息插入成功[%d]张，失败[%d]张",okCnt,failCnt);
	////////	MessageBox(strTmp);
	////////}
}

void CDbToolsDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_lstCardInfo.DeleteAllItems();//清空
	try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //设置编码格式
        int ret = mysql_query(&m_myCont, "select CARD_ID,IO_FLG,NOW_POS,CHG_DATE from pm_db.card_inf order by CARD_ID");//查询
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
				int iCount = 0;
				char stmp[256];
                while (m_sqlRow = mysql_fetch_row(m_myResult))//获取具体的数据
                {
					CString card_no = m_sqlRow[0];
					CString io_flg = m_sqlRow[1];
					CString room_no = m_sqlRow[2];
					CString pre_room_no = m_sqlRow[2];
					CString chg_time = m_sqlRow[3];
					//
					iCount++;
					//序号
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%03d",iCount);
					m_lstCardInfo.InsertItem(iCount-1, _T(stmp));
					//卡号
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
			MessageBox("mysql查询卡片失败1！");
			//SendMessage(WM_CLOSE);
			//return FALSE;
        }
	}
	catch(_com_error e)
	{
		//
		MessageBox("mysql查询卡片失败2！");
		//SendMessage(WM_CLOSE);
		//return FALSE;
	}
}

void CDbToolsDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	unsigned int iCardNo = strtoul(m_qryCardNo.GetBuffer(),NULL,10);
	char sCardNo[256];
	memset(sCardNo,0,sizeof(sCardNo));
	sprintf(sCardNo,"%010u",iCardNo);
	m_lstInOutInfo.DeleteAllItems();//清空
	try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //设置编码格式
		char sSql[512];
		memset(sSql,0,sizeof(sSql));
		sprintf(sSql,"select DEV_ID,CARD_ID,LBL_ID,X,Y,Z,TEMPERATURE,HEART_RATE,HAND,UP_FLG,CHECK_TIME from pm_db.people_health_inf where CARD_ID = '%010u' order by CHG_DATE",iCardNo);
        int ret = mysql_query(&m_myCont, sSql);//查询
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
				int iCount = 0;
				char stmp[256];
                while (m_sqlRow = mysql_fetch_row(m_myResult))//获取具体的数据
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
					//序号
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%03d",iCount);
					m_lstInOutInfo.InsertItem(iCount-1, _T(stmp));
					//设备
					m_lstInOutInfo.SetItemText(iCount-1, 1, dev_no);
					//卡号
					m_lstInOutInfo.SetItemText(iCount-1, 2, card_no);
					//标签
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
			MessageBox("mysql查询流水失败1！");
			//SendMessage(WM_CLOSE);
			//return FALSE;
        }
	}
	catch(_com_error e)
	{
		//
		MessageBox("mysql查询流水失败2！");
		//SendMessage(WM_CLOSE);
		//return FALSE;
	}
}

void CDbToolsDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if(::MessageBox(NULL,"确定要删除所有卡片吗？","删除卡片",MB_YESNO) != IDYES)return;
	m_lstCardInfo.DeleteAllItems();//清空
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
		//成功
		mysql_commit(&m_myCont);
		MessageBox("清除卡片表成功！");
		//return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//失败
		MessageBox("清除卡片表失败！");
		//return false;
	}
	////////清除人员信息表
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.card_pepole_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//成功
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("清除人员信息表成功！");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//失败
	//////	MessageBox("清除人员信息表失败！");
	//////	//return false;
	//////}
	////////清除报警上下限信息表
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.card_warn_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//成功
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("清除报警上下限表成功！");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//失败
	//////	MessageBox("清除报警上下限表失败！");
	//////	//return false;
	//////}
	////////清除读卡器位置表
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.dev_pos_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//成功
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("清除读卡器位置表成功！");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//失败
	//////	MessageBox("清除读卡器位置表失败！");
	//////	//return false;
	//////}
	////////清除标签位置表
	//////memset(sSql,0,sizeof(sSql));
	//////strcpy(sSql,"truncate table pm_db.lbl_pos_inf ");
	//////mysql_query(&m_myCont,"start transaction;");
	//////ret = mysql_query(&m_myCont,sSql);
	//////if(0 == ret)
	//////{
	//////	//成功
	//////	mysql_commit(&m_myCont);
	//////	MessageBox("清除标签位置表成功！");
	//////	//return true;
	//////}
	//////else
	//////{
	//////	mysql_rollback(&m_myCont);
	//////	//失败
	//////	MessageBox("清除标签位置表失败！");
	//////	//return false;
	//////}
}

void CDbToolsDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	if(::MessageBox(NULL,"确定要删除所有流水吗？","删除流水",MB_YESNO) != IDYES)return;
	m_lstInOutInfo.DeleteAllItems();//清空
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
		//成功
        //获得返回参数@t,@t是传出参数
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		MessageBox("清除成功！");
		//return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//失败
		MessageBox("清除失败！");
		//return false;
	}
}

void CDbToolsDlg::OnBnClickedBtnDelReadCardInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	if(::MessageBox(NULL,"确定要删除所有读卡流水吗？","删除流水",MB_YESNO) != IDYES)return;
	m_lstReadCard.DeleteAllItems();//清空
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
		//成功
        //获得返回参数@t,@t是传出参数
        mysql_query(&m_myCont, "select @t");
        res_ptr = mysql_store_result(&m_myCont);
        if (res_ptr)
        {
           sqlrow = mysql_fetch_row (res_ptr);
        }
        mysql_free_result (res_ptr);
		mysql_commit(&m_myCont);
		MessageBox("清除成功！");
		//return true;
	}
	else
	{
		mysql_rollback(&m_myCont);
		//失败
		MessageBox("清除失败！");
		//return false;
	}
}

void CDbToolsDlg::OnBnClickedBtnReadCardQry()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	unsigned int iCardNo = strtoul(m_readCardNo.GetBuffer(),NULL,10);
	char sCardNo[256];
	CString d_begin = m_readCardDate + "000000";
	CString d_end =  m_readCardDate + "235959";
	memset(sCardNo,0,sizeof(sCardNo));
	sprintf(sCardNo,"%010u",iCardNo);
	m_lstReadCard.DeleteAllItems();//清空
	try
	{
		MYSQL_RES *m_myResult;
		MYSQL_ROW m_sqlRow;
		mysql_query(&m_myCont, "SET NAMES GBK"); //设置编码格式
		char sSql[512];
		memset(sSql,0,sizeof(sSql));
		//sprintf(sSql,"select READ_ID,CARD_ID,IO_FLG,CHG_DATE from hm_db.read_card_inf where CARD_ID = '%010u'and date_format(CHG_DATE,'%Y%m%d') = '%s' order by CHG_DATE",iCardNo,m_readCardDate.GetBuffer());
		sprintf(sSql,"select READ_ID,CARD_ID,IO_FLG,CHG_DATE from pm_db.read_card_inf where CARD_ID = '%010u'and CHG_DATE between '%s' and '%s' order by CHG_DATE",iCardNo,d_begin.GetBuffer(),d_end.GetBuffer());
        int ret = mysql_query(&m_myCont, sSql);//查询
        if (0 == ret)
        {
            m_myResult = mysql_store_result(&m_myCont);
            if (m_myResult)
            {
				int iCount = 0;
				char stmp[256];
                while (m_sqlRow = mysql_fetch_row(m_myResult))//获取具体的数据
                {
					CString read_no = m_sqlRow[0];
					CString card_no = m_sqlRow[1];
					CString io_flg = m_sqlRow[2];
					CString chg_time = m_sqlRow[3];
					//
					iCount++;
					//序号
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%03d",iCount);
					m_lstReadCard.InsertItem(iCount-1, _T(stmp));
					m_lstReadCard.SetItemText(iCount-1, 1, read_no);
					//卡号
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
			MessageBox("mysql查询读卡流水失败1！");
			//SendMessage(WM_CLOSE);
			//return FALSE;
        }
	}
	catch(_com_error e)
	{
		//
		MessageBox("mysql查询读卡流水失败2！");
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
