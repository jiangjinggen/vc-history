// SafServerAppDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SafServerApp.h"
#include "SafServerAppDlg.h"
#include "direct.h"
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


// CSafServerAppDlg �Ի���




CSafServerAppDlg::CSafServerAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSafServerAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSafServerAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_POINT, m_lstPoint);
	DDX_Control(pDX, IDC_LIST_LOG, m_lstLog);
}

BEGIN_MESSAGE_MAP(CSafServerAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSafServerAppDlg ��Ϣ�������
// ע�ͣ����ֽڰ���GBK��UTF-8  
int CSafServerAppDlg::GBK2UTF8(char *szGbk,char *szUtf8,int Len)  
{  
    // �Ƚ����ֽ�GBK��CP_ACP��ANSI��ת���ɿ��ַ�UTF-16  
    // �õ�ת��������Ҫ���ڴ��ַ���  
    int n = MultiByteToWideChar(CP_ACP,0,szGbk,-1,NULL,0);  
    // �ַ������� sizeof(WCHAR) �õ��ֽ���  
    WCHAR *str1 = new WCHAR[sizeof(WCHAR) * n];  
    // ת��  
    MultiByteToWideChar(CP_ACP,  // MultiByte�Ĵ���ҳCode Page  
        0,            //���ӱ�־���������й�  
        szGbk,        // �����GBK�ַ���  
        -1,           // �����ַ������ȣ�-1��ʾ�ɺ����ڲ�����  
        str1,         // ���  
        n             // ������������ڴ�  
        );  
  
    // �ٽ����ַ���UTF-16��ת�����ֽڣ�UTF-8��  
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  
    ////if (n > Len)  
    ////{  
    ////    delete[]str1;  
    ////    return -1;  
    ////}  
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, szUtf8, n, NULL, NULL);  
    delete[]str1;  
    str1 = NULL;  
  
    return 0;  
}  
//UTF-8 GBK  
/*2.int UTF82GBK(char *szUtf8,char *szGbk,int Len)  
3.{  
4.    int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);  
5.    WCHAR * wszGBK = new WCHAR[sizeof(WCHAR) * n];  
6.    memset(wszGBK, 0, sizeof(WCHAR) * n);  
7.    MultiByteToWideChar(CP_UTF8, 0,szUtf8,-1, wszGBK, n);  
8.  
9.    n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
10.    if (n > Len)  
11.    {  
12.        delete[]wszGBK;  
13.        return -1;  
14.    }  
15.  
16.    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGbk, n, NULL, NULL);  
17.  
18.    delete[]wszGBK;  
19.    wszGBK = NULL;  
20.  
21.    return 0;  
22.} */ 
void CSafServerAppDlg::GetAndSndPhoneQryDataFix(const char *snode)
{
	if(2 != m_useDb)return ;
	char stmp[256];
	char stmp1[256];
	char sstmp[512];
	//
    try
	{
		_variant_t vCardNo,vCardName,vDevNo,vDateTime,vFlg,vRoomNo,vRoomName;
		_RecordsetPtr m_pRecordset;
		m_pRecordset.CreateInstance(__uuidof(Recordset)); 
		m_pRecordset->CursorLocation = adUseClient;
		m_pRecordset->Open(_bstr_t("SELECT t1.azdd as room_no,t2.Wzmc as room_name,t1.cardid as card_no,t1.yqmc as card_name,t1.yqbh as dev_no,CONVERT(varchar(30), t1.gxrq, 120) as rq,t1.rkbz as bz FROM DB_gdzcb t1 left join DB_StorageRack t2 on t2.sbh = t1.azdd where t1.azdd <> 0 and t1.gdwz = 1"),_variant_t((IDispatch *)m_dbConnection,true),adOpenStatic,adLockReadOnly,adCmdText);
        while(!m_pRecordset->adoEOF)
		{
			vCardNo =  m_pRecordset->GetCollect("card_no");//
			vCardName =  m_pRecordset->GetCollect("card_name");//
			vDevNo = m_pRecordset->GetCollect("dev_no");
			vRoomNo =  m_pRecordset->GetCollect("room_no");//
			vRoomName =  m_pRecordset->GetCollect("room_name");//
			vDateTime =  m_pRecordset->GetCollect("rq");
			vFlg =  m_pRecordset->GetCollect("bz");
			//
			memset(stmp1,0,sizeof(stmp1));
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vCardNo);
			strcpy(stmp1,stmp);
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vCardName);
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			//
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vDevNo);
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			//
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vRoomNo.vt)
			{
				strcpy(stmp,"000");
			}
			else
			{
				strcpy(stmp,(LPCTSTR)(_bstr_t)vRoomNo);
			}
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vRoomName.vt)
			{
				strcpy(stmp,"δ֪");
			}
			else
			{
				strcpy(stmp,(LPCTSTR)(_bstr_t)vRoomName);
			}
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			memset(stmp,0,sizeof(vDateTime));
			if(VT_NULL == vDateTime.vt)
			{
				
			}
			else
			{
			    
			}
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vFlg.vt)
			{
				 strcpy(stmp,"0");
			}
			else
			{
			    strcpy(stmp,(LPCTSTR)(_bstr_t)vFlg);
			}
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			strcat(stmp1,"\r\n");
			memset(sstmp,0,sizeof(sstmp));
			memcpy(sstmp,snode,3);
			//GBK2UTF8(stmp,sstmp+3,strlen(stmp));
			memcpy(sstmp+3,stmp1,strlen(stmp1));
			m_pQuerySendQueue->InsertQueue((BYTE*)sstmp,strlen(sstmp));
			//
			m_pRecordset->MoveNext();///�Ƶ���һ����¼
		}
		m_pRecordset->Close();
		//m_pRecordset->Release();
	}
	catch(_com_error e)
	{
		//
		;
	}
	//
}
void CSafServerAppDlg::GetAndSndPhoneQryData(const char *snode)
{
	if(2 != m_useDb)return ;
	char stmp[256];
	char stmp1[256];
	char sstmp[512];
	//
	////memset(stmp,0,sizeof(stmp));
	////strcpy(stmp,"1234567890,����,101,����һ\r\n");
	////memset(sstmp,0,sizeof(sstmp));
	////memcpy(sstmp,snode,3);
	////GBK2UTF8(stmp,sstmp+3,strlen(stmp));
	////m_pQuerySendQueue->InsertQueue((BYTE*)sstmp,strlen(sstmp));
	//////
	////memset(stmp,0,sizeof(stmp));
	////strcpy(stmp,"1234567891,����,101,����һ\r\n");
	////memset(sstmp,0,sizeof(sstmp));
	////memcpy(sstmp,snode,3);
	////GBK2UTF8(stmp,sstmp+3,strlen(stmp));
	////m_pQuerySendQueue->InsertQueue((BYTE*)sstmp,strlen(sstmp));
	//////
	////memset(stmp,0,sizeof(stmp));
	////strcpy(stmp,"1234567892,����,102,���Ҷ�\r\n");
	////memset(sstmp,0,sizeof(sstmp));
	////memcpy(sstmp,snode,3);
	////GBK2UTF8(stmp,sstmp+3,strlen(stmp));
	////m_pQuerySendQueue->InsertQueue((BYTE*)sstmp,strlen(sstmp));
	//////
	////memset(stmp,0,sizeof(stmp));
	////strcpy(stmp,"1234567893,����,103,������\r\n");
	////memset(sstmp,0,sizeof(sstmp));
	////memcpy(sstmp,snode,3);
	////GBK2UTF8(stmp,sstmp+3,strlen(stmp));
	////m_pQuerySendQueue->InsertQueue((BYTE*)sstmp,strlen(sstmp));
    try
	{
		_variant_t vCardNo,vCardName,vDevNo,vDateTime,vFlg,vRoomNo,vRoomName;
		_RecordsetPtr m_pRecordset;
		m_pRecordset.CreateInstance(__uuidof(Recordset)); 
		m_pRecordset->CursorLocation = adUseClient;
		m_pRecordset->Open(_bstr_t("SELECT t1.azdd as room_no,t2.Wzmc as room_name,t1.cardid as card_no,t1.yqmc as card_name,t1.yqbh as dev_no,CONVERT(varchar(30), t1.gxrq, 120) as rq,t1.rkbz as bz FROM DB_gdzcb t1 left join DB_StorageRack t2 on t2.sbh = t1.azdd where t1.azdd <> 0 and t1.gdwz = 0 or  t1.gdwz = 1"),_variant_t((IDispatch *)m_dbConnection,true),adOpenStatic,adLockReadOnly,adCmdText);
        while(!m_pRecordset->adoEOF)
		{
			vCardNo =  m_pRecordset->GetCollect("card_no");//
			vCardName =  m_pRecordset->GetCollect("card_name");//
			vDevNo =  m_pRecordset->GetCollect("dev_no");//
			vRoomNo =  m_pRecordset->GetCollect("room_no");//
			vRoomName =  m_pRecordset->GetCollect("room_name");//
			vDateTime =  m_pRecordset->GetCollect("rq");
			vFlg =  m_pRecordset->GetCollect("bz");
			//
			memset(stmp1,0,sizeof(stmp1));
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vCardNo);
			strcpy(stmp1,stmp);
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vCardName);
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			//
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vDevNo);
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			//
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vRoomNo.vt)
			{
				strcpy(stmp,"000");
			}
			else
			{
				strcpy(stmp,(LPCTSTR)(_bstr_t)vRoomNo);
			}
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vRoomName.vt)
			{
				strcpy(stmp,"δ֪");
			}
			else
			{
				strcpy(stmp,(LPCTSTR)(_bstr_t)vRoomName);
			}
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			memset(stmp,0,sizeof(vDateTime));
			if(VT_NULL == vDateTime.vt)
			{
				
			}
			else
			{
			    
			}
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vFlg.vt)
			{
				 strcpy(stmp,"0");
			}
			else
			{
			    strcpy(stmp,(LPCTSTR)(_bstr_t)vFlg);
			}
			strcat(stmp1,",");
			strcat(stmp1,stmp);
			strcat(stmp1,"\r\n");
			memset(sstmp,0,sizeof(sstmp));
			memcpy(sstmp,snode,3);
			GBK2UTF8(stmp1,sstmp+3,strlen(stmp1));
			//memcpy(sstmp+3,stmp1,strlen(stmp1));
			m_pQuerySendQueue->InsertQueue((BYTE*)sstmp,strlen(sstmp));
			//
			m_pRecordset->MoveNext();///�Ƶ���һ����¼
		}
		m_pRecordset->Close();
		//m_pRecordset->Release();
	}
	catch(_com_error e)
	{
		//
		;
	}
	//
	memset(stmp,0,sizeof(stmp));
	memcpy(stmp,snode,3);
	strcat(stmp,"000000000000\r\n");
	m_pQuerySendQueue->InsertQueue((BYTE*)stmp,strlen(stmp));
}
//վ�����ݴ����߳�
BOOL CSafServerAppDlg::StartStationPro()
{	
    //���ݴ���
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
	unsigned char sndBuf[DATA_FRAME_LEN+3];
	while(m_bolStation)
	{
		Sleep(1);
		while(!m_pStationRecvQueue->EmptyQueue()) 
		{
			dataSize = m_pStationRecvQueue->GetDataRecordSum();
			if(dataSize <= 0) 
				break;
			//
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
			m_pStationRecvQueue->ReadQueue(tmpBuf, &dataSize);
			//ת���նˣ�������ת��
			if(0x01 != tmpBuf[3])
			{
				//ת�������нڵ�
				memset(sndBuf,0,sizeof(sndBuf));
				memcpy(sndBuf,"000",3);
				memcpy(sndBuf+3,tmpBuf,dataSize);
				m_pClientSendQueue->InsertQueue(sndBuf,dataSize+3);

			}
		}
	}
    //
	return TRUE;				
}
UINT StationProcess(LPVOID Ptr)
{
	CSafServerAppDlg *pp = (CSafServerAppDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartStationPro();
	return 1;
}
//�ն����ݴ����߳�
BOOL CSafServerAppDlg::StartClientPro()
{	
    //���ݴ���
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
	while(m_bolStation)
	{
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
		}
	}
    //
	return TRUE;				
}
UINT ClientProcess(LPVOID Ptr)
{
	CSafServerAppDlg *pp = (CSafServerAppDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartClientPro();
	return 1;
}
//��ѯ���ݴ����߳�
BOOL CSafServerAppDlg::StartQueryPro()
{	
    //���ݴ���
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
	char snode_id[16];
	while(m_bolQuery)
	{
		Sleep(3);
		while(!m_pQueryRecvQueue->EmptyQueue()) 
		{
			dataSize = m_pQueryRecvQueue->GetDataRecordSum();
			if(dataSize <= 0) 
				break;
			//
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
			m_pQueryRecvQueue->ReadQueue(tmpBuf, &dataSize);
			if(0x02 == tmpBuf[3])
			{
				memset(snode_id,0,sizeof(snode_id));
				memcpy(snode_id,tmpBuf+8,3);
				GetAndSndPhoneQryData(snode_id);
			}
		}
	}
    //
	return TRUE;				
}
UINT QueryProcess(LPVOID Ptr)
{
	CSafServerAppDlg *pp = (CSafServerAppDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartQueryPro();
	return 1;
}
//�����߳�
BOOL CSafServerAppDlg::StartHelpPro()
{	
    //���ݴ���
	char stmp[256];
	int iCount = 0;
	int nIndex = 0;
    while(m_bolHelp)
	{
		Sleep(500);
		//�������½ڵ�״̬
		int lstCount = m_lstPoint.GetItemCount();
		for(int i = 0; i < lstCount; i++)
	    {
		    CString strNodeId = m_lstPoint.GetItemText(i,0);
			CString strNodeType = m_lstPoint.GetItemText(i,1);
			string tmpNodeId = strNodeId.GetBuffer();
			bool bStatus = false;
			if("վ��" == strNodeType)
			{
				bStatus = m_stationSvr.GetNodeConnStatus(tmpNodeId);
				SendNodeStatus(tmpNodeId,bStatus);
			}
			else if("�ն�" == strNodeType)
			{
				bStatus = m_clientSvr.GetNodeConnStatus(tmpNodeId);
			}
			else
			{
				bStatus = m_querySvr.GetNodeConnStatus(tmpNodeId);
			}
			if(false == bStatus)
			{
				m_lstPoint.SetItemText(i, 4,"�Ͽ�");
			}
			else
			{
				m_lstPoint.SetItemText(i, 4,"����");
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
		sprintf(stmp,"[%d]%d-%02d-%02d %d:%02d:%02d",m_pClientSendQueue->GetDataRecordSum(),now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
        m_lstLog.SetItemText(0, 3, _T(stmp)); 		
	}
    //
	return TRUE;				
}
UINT HelpProcess(LPVOID Ptr)
{
	CSafServerAppDlg *pp = (CSafServerAppDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartHelpPro();
	return 1;
}
/////////////
void CSafServerAppDlg::LoadNodeFromIni(string stitle,map<string,stuNodeInfo> &mapNodeInfo,int type)
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
			mapNodeInfo[vStr[0]].sock = 0;
			mapNodeInfo[vStr[0]].conn_status = false;
		}
	}
}
bool CSafServerAppDlg::SetNodeInfo()
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
		m_lstPoint.InsertItem(iCount-1, strTmp);
		//����
		strTmp = "վ��";
		m_lstPoint.SetItemText(iCount-1, 1,strTmp);
		//����
		strTmp = iter->second.node_name.c_str();
		m_lstPoint.SetItemText(iCount-1, 2,strTmp);
		//ip
		strTmp = iter->second.node_ip.c_str();
		m_lstPoint.SetItemText(iCount-1, 3,strTmp);
		//״̬
		strTmp = "δ����";
		m_lstPoint.SetItemText(iCount-1, 4,strTmp);
		//m_lstPoint.SetItemData(iCount-1, COLOR_RED);
	}
	for(iter = m_mapClientNodeInfo.begin();iter != m_mapClientNodeInfo.end();iter++)
	{
		iCount++;
		//id
		strTmp = iter->second.node_id.c_str();
		m_lstPoint.InsertItem(iCount-1, strTmp);
		//����
		strTmp = "�ն�";
		m_lstPoint.SetItemText(iCount-1, 1,strTmp);
		//����
		strTmp = iter->second.node_name.c_str();
		m_lstPoint.SetItemText(iCount-1, 2,strTmp);
		//ip
		strTmp = iter->second.node_ip.c_str();
		m_lstPoint.SetItemText(iCount-1, 3,strTmp);
		//״̬
		strTmp = "δ����";
		m_lstPoint.SetItemText(iCount-1, 4,strTmp);
		//m_lstPoint.SetItemData(iCount-1, COLOR_RED);
	}
	for(iter = m_mapQueryNodeInfo.begin();iter != m_mapQueryNodeInfo.end();iter++)
	{
		iCount++;
		//id
		strTmp = iter->second.node_id.c_str();
		m_lstPoint.InsertItem(iCount-1, strTmp);
		//����
		strTmp = "��ѯ";
		m_lstPoint.SetItemText(iCount-1, 1,strTmp);
		//����
		strTmp = iter->second.node_name.c_str();
		m_lstPoint.SetItemText(iCount-1, 2,strTmp);
		//ip
		strTmp = iter->second.node_ip.c_str();
		m_lstPoint.SetItemText(iCount-1, 3,strTmp);
		//״̬
		strTmp = "δ����";
		m_lstPoint.SetItemText(iCount-1, 4,strTmp);
		//m_lstPoint.SetItemData(iCount-1, COLOR_RED);
	}
	return true;
}
//
bool CSafServerAppDlg::SendNodeStatus(string nod_id,bool status)
{
	short len = 0;
	char tmpBuf[256];
	memset(tmpBuf,0,sizeof(tmpBuf));
	strcpy(tmpBuf,"000");
	len += 3;
	tmpBuf[len] = 0x0B;
	len += 1;
	len += 2;
	//֡����,0x01����,0x02��Ƭ����,0x03��������,0x04����,0x05�������״̬
	tmpBuf[len] = 0x05;
	len += 1;
	//����
	tmpBuf[len] = 0x01;
	len += 1;
	//client_id
	//memcpy(&tmpBuf[len],m_clientId.c_str(),3);
	len += 3;
	//nod_id
	memcpy(&tmpBuf[len],nod_id.c_str(),3);
	len += 3;
	//status
	if(status)
	{
		tmpBuf[len] = 0x01;
	}
	else
	{
	    tmpBuf[len] = 0x00;
	}
	len += 1;
	//У�飬�������ݲ�У��
	tmpBuf[len] = 0x00;
	len += 1;
	tmpBuf[len] = 0x0E;
	len += 1;
	//
	short lenTmp = len - 3 - 3;
	memcpy(&tmpBuf[4],&lenTmp,2);
	//
	m_pClientSendQueue->InsertQueue((BYTE *)tmpBuf,len);
	//
	return true;
}
//
BOOL CSafServerAppDlg::OnInitDialog()
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
	dwStyle = m_lstPoint.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_lstPoint.SetExtendedStyle(dwStyle);

	m_lstPoint.GetClientRect(&rect); 
    m_lstPoint.InsertColumn(0, _T("�ڵ�ID"), LVCFMT_LEFT, rect.Width()/6);
    m_lstPoint.InsertColumn(1, _T("�ڵ�����"), LVCFMT_LEFT, rect.Width()/6);
	m_lstPoint.InsertColumn(2, _T("�ڵ�����"), LVCFMT_LEFT, rect.Width()/4);
	m_lstPoint.InsertColumn(3, _T("�ڵ�IP"), LVCFMT_LEFT, rect.Width()/4);
	m_lstPoint.InsertColumn(4, _T("״̬"), LVCFMT_LEFT, rect.Width()/6);
	//������ʼ��
	char stmp[256];
	CString strPath(".\\cfg\\syscfg.ini");
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("��������", "db","",stmp,256,strPath.GetBuffer());
	m_dbStr = stmp;
	m_useDb = GetPrivateProfileInt("��������", "use_db",0,strPath.GetBuffer());
	m_svrStationPort = GetPrivateProfileInt("�������˿�", "station_port",0,strPath.GetBuffer());
	m_svrClientPort = GetPrivateProfileInt("�������˿�", "client_port",0,strPath.GetBuffer());
	m_svrQueryPort = GetPrivateProfileInt("�������˿�", "query_port",0,strPath.GetBuffer());
	m_logGrade = GetPrivateProfileInt("��������", "loggrade",0,strPath.GetBuffer());
	m_useDb = GetPrivateProfileInt("��������", "use_db",0,strPath.GetBuffer());
	if(m_useDb <= 0)m_useDb = 1;
    //
	memset(&m_mySqlCfg,0,sizeof(stuMySqlCfg));
	m_mySqlCfg.my_port = GetPrivateProfileInt("mysql����", "my_port",0,strPath.GetBuffer());
	if(m_mySqlCfg.my_port <= 0)m_mySqlCfg.my_port = 3306;
	GetPrivateProfileString("mysql����", "my_host","",m_mySqlCfg.my_host,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_db","",m_mySqlCfg.my_db,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_user","",m_mySqlCfg.my_user,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql����", "my_password","",m_mySqlCfg.my_password,256,strPath.GetBuffer());
	//
	LoadNodeFromIni("���վ��",m_mapStationNodeInfo,NODE_STATION);
	LoadNodeFromIni("����ն�",m_mapClientNodeInfo,NODE_STATION);
	LoadNodeFromIni("��ѯ�ն�",m_mapQueryNodeInfo,NODE_STATION);
	//
	SetNodeInfo();
	//
   ::CoInitialize(NULL);    
    //provider=Data Source=.;User ID=sa;Password=111111;database=FixedAssetsSystem
    m_dbConnection.CreateInstance(__uuidof(Connection));   
      
	_bstr_t strConnect = m_dbStr.GetBuffer(); 
	//_bstr_t strConnect = "provider=Data Source=.;User ID=sa;Password=casco_123;database=formal";

	////�������ݿ�
	if(2 == m_useDb)
	{
		try
		{
			HRESULT hr = NULL;
			hr = m_dbConnection->Open(strConnect,"","",adModeUnknown);
			if(S_OK != hr)
			{
				//���ݿ�����ʧ��
				MessageBox("sqlserver���ݿ�����ʧ��1��");
				SendMessage(WM_CLOSE);
				return FALSE;
			}
		}
		catch(_com_error e)
		{
			//���ݿ�����ʧ��
			MessageBox("sqlserver���ݿ�����ʧ��2��");
			SendMessage(WM_CLOSE);
			return FALSE;
		}
	}
	else if(3 == m_useDb)
	{
		////MYSQL *tmpRet = NULL;
		////tmpRet = mysql_init(&m_myCont);
		////if(NULL == tmpRet)
		////{
		////	MessageBox("mysql���ݿ�����ʧ��1��");
		////	SendMessage(WM_CLOSE);
		////}
		////tmpRet = mysql_real_connect(&m_myCont, m_mySqlCfg.my_host, m_mySqlCfg.my_user, m_mySqlCfg.my_password, m_mySqlCfg.my_db, m_mySqlCfg.my_port, NULL, CLIENT_MULTI_STATEMENTS);
		////if(NULL == tmpRet)
		////{
		////	MessageBox("mysql���ݿ�����ʧ��2��");
		////	SendMessage(WM_CLOSE);
		////}
	}
	//
	m_pStationRecvQueue = new CDataQueue;
	m_pStationSendQueue = new CDataQueue;
	m_pClientRecvQueue = new CDataQueue;
	m_pClientSendQueue = new CDataQueue;
	m_pQueryRecvQueue = new CDataQueue;
	m_pQuerySendQueue = new CDataQueue;
	m_pLogMsg    = new CLogMessage;
	//����վ��������
	m_stationSvr.SetNodeInfo(m_mapStationNodeInfo);
	m_stationSvr.StartNodeServer("127.0.0.1",m_svrStationPort,m_pStationRecvQueue,m_pStationSendQueue,m_pLogMsg,SVR_STATION);
	//�����ն˷�����
	m_clientSvr.StartNodeServer("127.0.0.1",m_svrClientPort,m_pClientRecvQueue,m_pClientSendQueue,m_pLogMsg,SVR_CLIENT);
	m_clientSvr.SetNodeInfo(m_mapClientNodeInfo);
	//������ѯ������
	m_querySvr.StartNodeServer("127.0.0.1",m_svrQueryPort,m_pQueryRecvQueue,m_pQuerySendQueue,m_pLogMsg,SVR_QUERY);
	m_querySvr.SetNodeInfo(m_mapQueryNodeInfo);
	//����վ�����ݴ����߳�
	m_bolStation = true;
	m_stationThread = AfxBeginThread(StationProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_stationThread->m_bAutoDelete = TRUE;
	m_stationThread->ResumeThread();
	//�����ն����ݴ����߳�
	m_bolClient = true;
	m_clientThread = AfxBeginThread(ClientProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_clientThread->m_bAutoDelete = TRUE;
	m_clientThread->ResumeThread();
	//������ѯ�����߳�
	m_bolQuery = true;
	m_queryThread = AfxBeginThread(QueryProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_queryThread->m_bAutoDelete = TRUE;
	m_queryThread->ResumeThread();
	//���������߳�
	m_bolHelp = true;
	m_helpThread = AfxBeginThread(HelpProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_helpThread->m_bAutoDelete = TRUE;
	m_helpThread->ResumeThread();
	//
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSafServerAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSafServerAppDlg::OnPaint()
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
HCURSOR CSafServerAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSafServerAppDlg::OnDestroy()
{
	m_bolStation = false;
	m_bolClient = false;
	m_bolQuery = false;
	m_bolHelp = false;
	m_stationSvr.StopTcpServer();
	m_clientSvr.StopTcpServer();
	m_querySvr.StopTcpServer();
	Sleep(2000);
	try
	{
		//
		if(m_pStationRecvQueue != NULL)
		{
			m_pStationRecvQueue->RemoveAll();
			delete m_pStationRecvQueue;
			m_pStationRecvQueue = NULL;
		}
		if(m_pStationSendQueue != NULL)
		{
			m_pStationSendQueue->RemoveAll();
			delete m_pStationSendQueue;
			m_pStationSendQueue = NULL;
		}
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
		if(m_pQueryRecvQueue != NULL)
		{
			m_pQueryRecvQueue->RemoveAll();
			delete m_pQueryRecvQueue;
			m_pQueryRecvQueue = NULL;
		}
		if(m_pQuerySendQueue != NULL)
		{
			m_pQuerySendQueue->RemoveAll();
			delete m_pQuerySendQueue;
			m_pQuerySendQueue = NULL;
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
    //
	try
	{
		if(2 == m_useDb)
		{
			m_dbConnection->Close();
			::CoUninitialize();
		}
		else if(3 == m_useDb)
		{
			////mysql_close (&m_myCont);
		}
	}
	catch(_com_error e)
	{
	}
	//
	WSACleanup();
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}
