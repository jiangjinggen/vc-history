// MonitorClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MonitorClient.h"
#include "MonitorClientDlg.h"
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


// CMonitorClientDlg 对话框




CMonitorClientDlg::CMonitorClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonitorClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATION, m_lstStation);
	DDX_Control(pDX, IDC_LIST_LOG, m_lstLog);
}

BEGIN_MESSAGE_MAP(CMonitorClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STATION, &CMonitorClientDlg::OnNMDblclkListStation)
	ON_BN_CLICKED(IDC_BTN_SEL_STATION, &CMonitorClientDlg::OnBnClickedBtnSelStation)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STATION, &CMonitorClientDlg::OnLvnItemchangedListStation)
END_MESSAGE_MAP()


// CMonitorClientDlg 消息处理程序
//
BOOL CMonitorClientDlg::StartDataPro()
{	
    //数据处理
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
	char stmp[256];
	int n = 0;
	int pos = 0;
	unsigned char ftype = 0;
	string station_id = "";
	while(m_bDataPro)
	{
		if(!m_bDataPro)break;
		Sleep(3);
		while(!m_pClientRecvQueue->EmptyQueue()) 
		{
			dataSize = m_pClientRecvQueue->GetDataRecordSum();
			if(dataSize <= 0) 
				break;
			//
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
			m_pClientRecvQueue->ReadQueue(tmpBuf, &dataSize);
			if(dataSize < BASE_FRAME_LEN)continue;
			//解包
			if(0x0B == tmpBuf[0])
			{
				ftype = tmpBuf[3];
				memset(stmp,0,sizeof(stmp));
				memcpy(stmp,&tmpBuf[8],3);
				station_id = stmp;
				if(station_id.length() <= 0)continue;
				pos = 11; //数据的开始
				switch(ftype)
				{
				case CARD_DATA:
					//卡片数据
					if(m_stationId == station_id)
					{
						//卡号
						unsigned int card_no = 0;
						memcpy(&card_no,&tmpBuf[pos],sizeof(unsigned int));
						pos += sizeof(unsigned int);
						pos += 1;
						pos += sizeof(unsigned int);
						//room_no
						unsigned int room_no = 0;
						memcpy(&room_no,&tmpBuf[pos],sizeof(unsigned int));
						pos += sizeof(unsigned int);
						//lst_time
						time_t lst_time = 0;
						memcpy(&lst_time,&tmpBuf[pos],sizeof(unsigned int));
						pos += sizeof(unsigned int);
						//显示
						if(NULL != m_hzpSetData)
						{
							m_hzpSetData(card_no,room_no,lst_time,0,NULL);
						}
					}
					break;
				case ATT_DATA:
					{
						//考勤数据
						//卡号
						unsigned int card_no = 0;
						memcpy(&card_no,&tmpBuf[pos],sizeof(unsigned int));
						pos += sizeof(unsigned int);
						//fst_time
						time_t fst_time = 0;
						memcpy(&fst_time,&tmpBuf[pos],sizeof(unsigned int));
						pos += sizeof(unsigned int);
						//lst_time
						time_t lst_time = 0;
						memcpy(&lst_time,&tmpBuf[pos],sizeof(unsigned int));
						pos += sizeof(unsigned int);
						//
						char tmpno[256];
						char tmpdat[256];
						memset(tmpno,0,sizeof(tmpno));
						memset(tmpdat,0,sizeof(tmpdat));
						sprintf(tmpno,"%010u",card_no);
						sprintf(tmpdat,"%u,%u",fst_time,lst_time);
						//
						struct tm tmTmp;
						tmTmp = *localtime(&fst_time);
						memset(stmp,0,sizeof(stmp));
						sprintf(stmp,".\\cfg\\%s\\data\\%d-%02d-%02d_att.ini",station_id.c_str(),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
						WritePrivateProfileString("考勤数据",tmpno,tmpdat,stmp);
					}
					break;
				case CFG_DATA:
					//配置
					break;
				case NET_STATUS:
					{
						//网络状态
						bool bStatus = false;
						unsigned char node_station = 0x00;
						node_station = tmpBuf[pos];
						if(0x01 == node_station)
						{
							bStatus = true;
						}
						else
						{
							bStatus = false;
						}
						//map<string,stuNodeInfo> m_mapStationNodeInfo;
						map<string,stuNodeInfo>::iterator iter;
						iter = m_mapStationNodeInfo.find(station_id);
						if(m_mapStationNodeInfo.end() != iter)
						{
							iter->second.conn_status = bStatus;
						}
					}
					break;
				default:
					break;
				}
			}
		}
		Sleep(1000);
	}
    //
	return TRUE;				
}
UINT dataProcess(LPVOID Ptr)
{
	CMonitorClientDlg *pp = (CMonitorClientDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartDataPro();
	return 1;
}
//辅助线程
BOOL CMonitorClientDlg::StartHelpPro()
{	
    //数据处理
	char stmp[256];
	int iCount = 0;
	int nIndex = 0;
    while(m_bolHelp)
	{
		//遍历更新节点状态
		int lstCount = m_lstStation.GetItemCount();
		for(int i = 0; i < lstCount; i++)
	    {
		    CString strNodeId = m_lstStation.GetItemText(i,0);
			CString strNodeType = m_lstStation.GetItemText(i,1);
			string tmpNodeId = strNodeId.GetBuffer();
			bool bStatus = false;
			//map<string,stuNodeInfo> m_mapStationNodeInfo;
			map<string,stuNodeInfo>::iterator iter;
			iter = m_mapStationNodeInfo.find(strNodeId.GetBuffer());
			if(m_mapStationNodeInfo.end() != iter)
			{
				bStatus = iter->second.conn_status;
			}
			if(false == bStatus)
			{
				m_lstStation.SetItemText(i, 4,"断开");
			}
			else
			{
				m_lstStation.SetItemText(i, 4,"连接");
			}
		}
		//
	    stuLogContent msg;
	    if(!m_pLogMsg->GetLogMessage(&msg))
		{
			Sleep(1000);
			continue;
		}
		if(iCount > 300)
		{
			m_lstLog.DeleteAllItems();//清空
            iCount = 0;
		}
		iCount++;
		//序号
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_lstLog.InsertItem(0, _T(stmp));
		//等级
		memset(stmp,0,sizeof(stmp));
		if(GRADE_LOG_COMM == msg.grade)
		{
			strcpy(stmp,"普通");
		}
		else if(GRADE_LOG_WARN == msg.grade)
		{
			strcpy(stmp,"警告");
		}
		else if(GRADE_LOG_ERROR == msg.grade)
		{
			strcpy(stmp,"错误");
		}
		else
		{
			strcpy(stmp,"未知");
		}
		m_lstLog.SetItemText(0, 1, _T(stmp));
		//内容
		m_lstLog.SetItemText(0, 2, _T(msg.body.c_str()));
		//时间
		struct tm *now = NULL;
		now = localtime(&msg.datetime);
		sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
        m_lstLog.SetItemText(0, 3, _T(stmp)); 		
	}
    //
	return TRUE;				
}
UINT HelpProcess(LPVOID Ptr)
{
	CMonitorClientDlg *pp = (CMonitorClientDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartHelpPro();
	return 1;
}
/////////////
void CMonitorClientDlg::LoadNodeFromIni(string stitle,map<string,stuNodeInfo> &mapNodeInfo,int type)
{
	string filePath = ".\\cfg\\syscfg.ini";
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return ;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if (stitle != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "总行数" || str_first == "总路数")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 4)
			{
				continue;
			}
			mapNodeInfo[vStr[0]].node_type = type;
			mapNodeInfo[vStr[0]].node_id = vStr[0];
			mapNodeInfo[vStr[0]].node_code = vStr[1];
			mapNodeInfo[vStr[0]].node_name = vStr[2];
			mapNodeInfo[vStr[0]].node_ip = vStr[3];
			mapNodeInfo[vStr[0]].sock = -1;
			mapNodeInfo[vStr[0]].conn_status = false;
		}
	}
}
bool CMonitorClientDlg::SetNodeInfo()
{
	int iCount = 0;
	CString strTmp;
	char stmp[256];
	map<string,stuNodeInfo>::iterator iter;
	for(iter = m_mapStationNodeInfo.begin();iter != m_mapStationNodeInfo.end();iter++)
	{
		iCount++;
		//id
		strTmp = iter->second.node_id.c_str();
		m_lstStation.InsertItem(iCount-1, strTmp);
		//类型
		strTmp = "站点";
		m_lstStation.SetItemText(iCount-1, 1,strTmp);
		//名称
		strTmp = iter->second.node_name.c_str();
		m_lstStation.SetItemText(iCount-1, 2,strTmp);
		//ip
		strTmp = iter->second.node_ip.c_str();
		m_lstStation.SetItemText(iCount-1, 3,strTmp);
		//状态
		strTmp = "未连接";
		m_lstStation.SetItemText(iCount-1, 4,strTmp);
		//m_lstStation.SetItemData(iCount-1, COLOR_RED);
	}
	return true;
}
//
BOOL CMonitorClientDlg::OnInitDialog()
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
	m_clientId = "901";
	m_stationId = "101";
	//1.加载动态库
	m_hzDll = LoadLibrary(_T("MonitorDisplay.dll"));
	if (NULL == m_hzDll)
	{
	    MessageBox("加载 MonitorDisplay.dll 失败");
	}
	//2.获取导出函数指针
	m_hzpShowDlg = (hzpDllShowDlg)GetProcAddress(m_hzDll,"ShowDialog");
	if (NULL==m_hzpShowDlg)
	{
		MessageBox("DLL中函数寻找失败1");
		//return;
	}
	m_hzpSetData = (hzpDllSetData)GetProcAddress(m_hzDll,"SetData");
	if (NULL==m_hzpSetData)
	{
		MessageBox("DLL中函数寻找失败2");
		//return;
	}
	//
	DWORD dwStyle = m_lstLog.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstLog.SetExtendedStyle(dwStyle);
	
	CRect rect;
	m_lstLog.GetClientRect(&rect); //获得当前客户区信息 
    m_lstLog.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/12);
    m_lstLog.InsertColumn(1, _T("级别"), LVCFMT_LEFT, rect.Width()/10);
    m_lstLog.InsertColumn(2, _T("内容"), LVCFMT_LEFT, rect.Width()/2);
    m_lstLog.InsertColumn(3, _T("时间"), LVCFMT_LEFT, rect.Width()/3); 
	//
	dwStyle = m_lstStation.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstStation.SetExtendedStyle(dwStyle);

	m_lstStation.GetClientRect(&rect); 
    m_lstStation.InsertColumn(0, _T("节点ID"), LVCFMT_LEFT, rect.Width()/6);
    m_lstStation.InsertColumn(1, _T("节点类型"), LVCFMT_LEFT, rect.Width()/6);
	m_lstStation.InsertColumn(2, _T("节点名称"), LVCFMT_LEFT, rect.Width()/4);
	m_lstStation.InsertColumn(3, _T("节点IP"), LVCFMT_LEFT, rect.Width()/4);
	m_lstStation.InsertColumn(4, _T("状态"), LVCFMT_LEFT, rect.Width()/6);
	//参数初始化
	char stmp[256];
	CString strPath(".\\cfg\\syscfg.ini");
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("参数配置", "svr_id","",stmp,256,strPath.GetBuffer());
	m_svrId = stmp;
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("参数配置", "client_id","",stmp,256,strPath.GetBuffer());
	m_clientId = stmp;
	m_logGrade = GetPrivateProfileInt("参数配置", "loggrade",0,strPath.GetBuffer());
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("服务器", "svr_ip","",stmp,256,strPath.GetBuffer());
	m_svrIp = stmp;
	m_svrPort = GetPrivateProfileInt("服务器", "svr_port",0,strPath.GetBuffer());
	//
	LoadNodeFromIni("监控站点",m_mapStationNodeInfo,NODE_STATION);
	//
	SetNodeInfo();
	//
	m_pClientRecvQueue = new CDataQueue;
	m_pClientSendQueue = new CDataQueue;
	m_pLogMsg    = new CLogMessage;
	//
	m_bDataPro = true;
	//
	m_tcpTransClient.StartTcpClient(m_svrId.c_str(),m_clientId.c_str(),m_svrIp.c_str(),m_svrPort,m_pLogMsg,m_pClientRecvQueue);
	//数据处理线程
	m_dataThread = AfxBeginThread(dataProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_dataThread->m_bAutoDelete = TRUE;
	m_dataThread->ResumeThread();
	//启动辅助线程
	m_bolHelp = true;
	m_helpThread = AfxBeginThread(HelpProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_helpThread->m_bAutoDelete = TRUE;
	m_helpThread->ResumeThread();
	//
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMonitorClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMonitorClientDlg::OnPaint()
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
HCURSOR CMonitorClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorClientDlg::OnNMDblclkListStation(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	m_hzpShowDlg(m_stationId);
	*pResult = 0;
}

void CMonitorClientDlg::OnBnClickedBtnSelStation()
{
	// TODO: 在此添加控件通知处理程序代码
	//
	//m_clientId = 1;
	m_hzpShowDlg(m_stationId);
    //
}

void CMonitorClientDlg::OnDestroy()
{
	m_bDataPro = false;
	m_bolHelp = false;
	m_tcpTransClient.StopTcpClient();
	Sleep(2000);
	try
	{
		//
		if(m_pClientRecvQueue != NULL)
		{
			m_pClientRecvQueue->RemoveAll();
			delete m_pClientRecvQueue;
			m_pClientRecvQueue = NULL;
		}
		if(m_pClientSendQueue != NULL)
		{
			m_pClientSendQueue->RemoveAll();
			delete m_pClientSendQueue;
			m_pClientSendQueue = NULL;
		}
		//
		if(m_pLogMsg != NULL)
		{
			delete m_pLogMsg;
			m_pLogMsg = NULL;
		}
	}
	catch(int i)
	{
	}
	WSACleanup();
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}

void CMonitorClientDlg::OnLvnItemchangedListStation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION ps;
	int nIndex;
	ps = m_lstStation.GetFirstSelectedItemPosition();
	nIndex = m_lstStation.GetNextSelectedItem(ps);
	CString strTmp = m_lstStation.GetItemText(nIndex,0);
	m_stationId = strTmp.GetBuffer();
	//
	*pResult = 0;
}
