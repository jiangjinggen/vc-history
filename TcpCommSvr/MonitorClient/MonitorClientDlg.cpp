// MonitorClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MonitorClient.h"
#include "MonitorClientDlg.h"
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


// CMonitorClientDlg �Ի���




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


// CMonitorClientDlg ��Ϣ�������
//
BOOL CMonitorClientDlg::StartDataPro()
{	
    //���ݴ���
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
			//���
			if(0x0B == tmpBuf[0])
			{
				ftype = tmpBuf[3];
				memset(stmp,0,sizeof(stmp));
				memcpy(stmp,&tmpBuf[8],3);
				station_id = stmp;
				if(station_id.length() <= 0)continue;
				pos = 11; //���ݵĿ�ʼ
				switch(ftype)
				{
				case CARD_DATA:
					//��Ƭ����
					if(m_stationId == station_id)
					{
						//����
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
						//��ʾ
						if(NULL != m_hzpSetData)
						{
							m_hzpSetData(card_no,room_no,lst_time,0,NULL);
						}
					}
					break;
				case ATT_DATA:
					{
						//��������
						//����
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
						WritePrivateProfileString("��������",tmpno,tmpdat,stmp);
					}
					break;
				case CFG_DATA:
					//����
					break;
				case NET_STATUS:
					{
						//����״̬
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
//�����߳�
BOOL CMonitorClientDlg::StartHelpPro()
{	
    //���ݴ���
	char stmp[256];
	int iCount = 0;
	int nIndex = 0;
    while(m_bolHelp)
	{
		//�������½ڵ�״̬
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
				m_lstStation.SetItemText(i, 4,"�Ͽ�");
			}
			else
			{
				m_lstStation.SetItemText(i, 4,"����");
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
			m_lstLog.DeleteAllItems();//���
            iCount = 0;
		}
		iCount++;
		//���
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_lstLog.InsertItem(0, _T(stmp));
		//�ȼ�
		memset(stmp,0,sizeof(stmp));
		if(GRADE_LOG_COMM == msg.grade)
		{
			strcpy(stmp,"��ͨ");
		}
		else if(GRADE_LOG_WARN == msg.grade)
		{
			strcpy(stmp,"����");
		}
		else if(GRADE_LOG_ERROR == msg.grade)
		{
			strcpy(stmp,"����");
		}
		else
		{
			strcpy(stmp,"δ֪");
		}
		m_lstLog.SetItemText(0, 1, _T(stmp));
		//����
		m_lstLog.SetItemText(0, 2, _T(msg.body.c_str()));
		//ʱ��
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
			if (str_first == "������" || str_first == "��·��")
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
		//����
		strTmp = "վ��";
		m_lstStation.SetItemText(iCount-1, 1,strTmp);
		//����
		strTmp = iter->second.node_name.c_str();
		m_lstStation.SetItemText(iCount-1, 2,strTmp);
		//ip
		strTmp = iter->second.node_ip.c_str();
		m_lstStation.SetItemText(iCount-1, 3,strTmp);
		//״̬
		strTmp = "δ����";
		m_lstStation.SetItemText(iCount-1, 4,strTmp);
		//m_lstStation.SetItemData(iCount-1, COLOR_RED);
	}
	return true;
}
//
BOOL CMonitorClientDlg::OnInitDialog()
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
	m_clientId = "901";
	m_stationId = "101";
	//1.���ض�̬��
	m_hzDll = LoadLibrary(_T("MonitorDisplay.dll"));
	if (NULL == m_hzDll)
	{
	    MessageBox("���� MonitorDisplay.dll ʧ��");
	}
	//2.��ȡ��������ָ��
	m_hzpShowDlg = (hzpDllShowDlg)GetProcAddress(m_hzDll,"ShowDialog");
	if (NULL==m_hzpShowDlg)
	{
		MessageBox("DLL�к���Ѱ��ʧ��1");
		//return;
	}
	m_hzpSetData = (hzpDllSetData)GetProcAddress(m_hzDll,"SetData");
	if (NULL==m_hzpSetData)
	{
		MessageBox("DLL�к���Ѱ��ʧ��2");
		//return;
	}
	//
	DWORD dwStyle = m_lstLog.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstLog.SetExtendedStyle(dwStyle);
	
	CRect rect;
	m_lstLog.GetClientRect(&rect); //��õ�ǰ�ͻ�����Ϣ 
    m_lstLog.InsertColumn(0, _T("���"), LVCFMT_LEFT, rect.Width()/12);
    m_lstLog.InsertColumn(1, _T("����"), LVCFMT_LEFT, rect.Width()/10);
    m_lstLog.InsertColumn(2, _T("����"), LVCFMT_LEFT, rect.Width()/2);
    m_lstLog.InsertColumn(3, _T("ʱ��"), LVCFMT_LEFT, rect.Width()/3); 
	//
	dwStyle = m_lstStation.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstStation.SetExtendedStyle(dwStyle);

	m_lstStation.GetClientRect(&rect); 
    m_lstStation.InsertColumn(0, _T("�ڵ�ID"), LVCFMT_LEFT, rect.Width()/6);
    m_lstStation.InsertColumn(1, _T("�ڵ�����"), LVCFMT_LEFT, rect.Width()/6);
	m_lstStation.InsertColumn(2, _T("�ڵ�����"), LVCFMT_LEFT, rect.Width()/4);
	m_lstStation.InsertColumn(3, _T("�ڵ�IP"), LVCFMT_LEFT, rect.Width()/4);
	m_lstStation.InsertColumn(4, _T("״̬"), LVCFMT_LEFT, rect.Width()/6);
	//������ʼ��
	char stmp[256];
	CString strPath(".\\cfg\\syscfg.ini");
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("��������", "svr_id","",stmp,256,strPath.GetBuffer());
	m_svrId = stmp;
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("��������", "client_id","",stmp,256,strPath.GetBuffer());
	m_clientId = stmp;
	m_logGrade = GetPrivateProfileInt("��������", "loggrade",0,strPath.GetBuffer());
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("������", "svr_ip","",stmp,256,strPath.GetBuffer());
	m_svrIp = stmp;
	m_svrPort = GetPrivateProfileInt("������", "svr_port",0,strPath.GetBuffer());
	//
	LoadNodeFromIni("���վ��",m_mapStationNodeInfo,NODE_STATION);
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
	//���ݴ����߳�
	m_dataThread = AfxBeginThread(dataProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_dataThread->m_bAutoDelete = TRUE;
	m_dataThread->ResumeThread();
	//���������߳�
	m_bolHelp = true;
	m_helpThread = AfxBeginThread(HelpProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_helpThread->m_bAutoDelete = TRUE;
	m_helpThread->ResumeThread();
	//
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMonitorClientDlg::OnPaint()
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
HCURSOR CMonitorClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorClientDlg::OnNMDblclkListStation(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_hzpShowDlg(m_stationId);
	*pResult = 0;
}

void CMonitorClientDlg::OnBnClickedBtnSelStation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ˴������Ϣ����������
}

void CMonitorClientDlg::OnLvnItemchangedListStation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION ps;
	int nIndex;
	ps = m_lstStation.GetFirstSelectedItemPosition();
	nIndex = m_lstStation.GetNextSelectedItem(ps);
	CString strTmp = m_lstStation.GetItemText(nIndex,0);
	m_stationId = strTmp.GetBuffer();
	//
	*pResult = 0;
}
