#include "StdAfx.h"
#include "NetTcpTransClient.h"

CNetTcpTransClient::CNetTcpTransClient(void)
{
	WSADATA wsaData;
    //
	m_sSocket = NULL;
	m_sRemoteIp = "";
	m_nRemotePort = -1;
	m_netThread = NULL;
	//
	m_bWSAStartup = TRUE;
	m_bactiveThread = TRUE;
	m_isStart = FALSE;
	//
	if(WSAStartup(0x202, &wsaData) == 0)
	{
		m_bWSAStartup = TRUE;
		err = 0;
	}
	else
	{
		m_bWSAStartup = TRUE;
		err = WSAGetLastError();
	}
	m_pSendDataQueue = NULL;
	////
}

CNetTcpTransClient::~CNetTcpTransClient(void)
{
}

//�����˳������¼�
void CNetTcpTransClient::SetThreadStatus(BOOL bol) 
{
	m_bactiveThread = bol;
	m_bRecv = false;
	m_bSend = false;
}
//��ӷ�������
int CNetTcpTransClient::SetSendData(stuCardInOutStatus &stuCIOS)
{
	////char tmpBuf[DATA_FRAME_LEN];
	////short len = 0;
	////memset(tmpBuf,0,sizeof(tmpBuf));
	////tmpBuf[len] = 0x0B;
	////len += 1;
	////len += 2;
	//////֡����,0x01����,0x02����,0x03����
	////tmpBuf[len] = 0x02;
	////len += 1;
	//////����
	////tmpBuf[len] = 0x01;
	////len += 1;
	//////svr_id
	////memcpy(&tmpBuf[len],m_svrId.c_str(),3);
	////len += 3;
	//////station_id
	////memcpy(&tmpBuf[len],m_clientId.c_str(),3);
	////len += 3;
	//////card_no
	////memcpy(&tmpBuf[len],&stuCIOS.card_no,sizeof(unsigned int));
	////len += sizeof(unsigned int);
	//////io_flg
	////tmpBuf[len] = stuCIOS.io_flg;
	////len += 1;
	//////pre_room_no
	////memcpy(&tmpBuf[len],&stuCIOS.pre_room_no,sizeof(unsigned int));
	////len += sizeof(unsigned int);
	//////room_no
	////memcpy(&tmpBuf[len],&stuCIOS.room_no,sizeof(unsigned int));
	////len += sizeof(unsigned int);
	//////lst_time
	////memcpy(&tmpBuf[len],&stuCIOS.lst_time,sizeof(time_t));
	////len += sizeof(time_t);
	//////У�飬�������ݲ�У��
	////tmpBuf[len] = 0x00;
	////len += 1;
	//////
	////m_pSendDataQueue->InsertQueue((BYTE *)tmpBuf,len);
	return 0;
}
//����һ�����������ݰ�
int CNetTcpTransClient::RevceiveData(char *recvBuf,SOCKET sscoket) 
{
	if(NULL == recvBuf)
	{
		return -1;
	}
	//
    int bol = 0;
    int totalsize = 0;
    char tmpBuf[DATA_FRAME_LEN];
    int tmplen = 0;
	int datalen = 0;
	int framelen = 0;
	int icount = 0;
	char *pp = recvBuf;
	CString strTmp;
	int iNum = 0;
	//��ʼ��������
    //��֡ͷ0x0B
	iNum = 0;
	memset(&m_stuRecvBufErr,0,sizeof(m_stuRecvBufErr));
	while(TRUE)
	{
		memset(tmpBuf,0,sizeof(tmpBuf));
		totalsize = recv(sscoket, tmpBuf, 1, 0);
		//(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		if(totalsize < 0)
		{
			//if(errno == EINTR || errno == EAGAIN)
			//{
			//	continue;
			//}
			//else
			//{
			//    return -2;
			//}
			//��ʱ�����ӶϿ�
			return totalsize;
		}
		else
		{
			//
		    if(0 == totalsize)
			{
				//�Է������ر�����
				return -3;
			}
			//
			if(0x0B != tmpBuf[0])
			{
				m_stuRecvBufErr.recvBuf[iNum] = tmpBuf[0];
				iNum++;
                m_stuRecvBufErr.len = iNum;
				continue;
			}
			//
			memcpy(pp,tmpBuf,totalsize);
			pp += totalsize;
			tmplen += totalsize;
			break;
		}
	}
    if(0 != iNum)
	{
		strTmp.Format("err_head[%d]",iNum);
		//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
		//CTestLog::LogDebugInfo(".\\data\\","testdebug_err.txt",m_stuRecvBufErr.recvBuf,m_stuRecvBufErr.len);
	}
	if(0x0B == recvBuf[0])
	{
		//����ǰ�����ֽ�
		icount = 0;
		while(tmplen < HEAD_LEN)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			totalsize = recv(sscoket, tmpBuf, HEAD_LEN - tmplen, 0);
			if(totalsize < 0) 
			{
				return totalsize;
			}
			if(0 == totalsize)
			{
				//�Է������ر�����
				return -3;
			}
			memcpy(pp,tmpBuf,totalsize);
			pp += totalsize;
			tmplen += totalsize;
			//
			//CTestLog::LogDebugInfo(".\\data\\","testdebug1.txt",recvBuf,tmplen);
		}
		//���ݳ���
		datalen = (unsigned char)recvBuf[HEAD_LEN-2] + (unsigned char)recvBuf[HEAD_LEN-1] * 256;
		//����������
		framelen = HEAD_LEN + datalen;
		//
		icount = 0;
		while(tmplen < framelen)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			totalsize = recv(sscoket, tmpBuf, framelen - tmplen, 0);
			if(totalsize < 0) 
			{
				return totalsize;
			}
			if(0 == totalsize)
			{
				//�Է������ر�����
				return -3;
			}
			memcpy(pp,tmpBuf,totalsize);
			pp += totalsize;
			tmplen += totalsize;
			//
			//CTestLog::LogDebugInfo(".\\data\\","testdebug2.txt",recvBuf,tmplen);
		}
	}
	///////////////////����
	//CTestLog::LogDebugInfo(".\\data\\","testdebug.txt",recvBuf,framelen);
	////////////////////////
    return framelen;	
}
//У�飬�����������ģ�0B
BOOL CNetTcpTransClient::CheckFrame(const unsigned char *dataframe, //��У���֡
				int framelen)                    //֡����
{
	return TRUE;
	unsigned char chcc = 0;
	unsigned char chres = dataframe[framelen-1];
	for(int i = 0; i < framelen - 1; i++)
	{
		chcc += dataframe[i];
	}
	chcc = ~chcc;
	chcc += 1;
	if(chres == chcc)
	{
        return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//���ӷ�����
BOOL CNetTcpTransClient::StartTcpClientConn()
{
	CString strTmp;
	while(m_bactiveThread)
	{
		if(m_sSocket <= 0 || false == m_bConn)
		{
			//���ӷ�����
			m_sSocket = socket(AF_INET,SOCK_STREAM,0);

			SOCKADDR_IN clientsock_in;
			clientsock_in.sin_addr.S_un.S_addr=inet_addr(m_sRemoteIp.c_str());
			clientsock_in.sin_family=AF_INET;
			clientsock_in.sin_port=htons(m_nRemotePort);

			int ret = connect(m_sSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//��ʼ����
			if(ret >= 0)
			{
				strTmp.Format("���ӷ������ɹ�[%s][%d]",m_sRemoteIp.c_str(),m_nRemotePort);
				AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
				int time_recv = CLIENT_SOCKET_TIMEOUT; //��ʱ��
	            setsockopt(m_sSocket,SOL_SOCKET, SO_RCVTIMEO, (char*)&time_recv, sizeof(int));
				m_recvTime = time(NULL);
				m_sendTime = time(NULL);
				m_bConn = true;
			}
			else
			{
				strTmp.Format("���ӷ�����ʧ��[%s][%d]",m_sRemoteIp.c_str(),m_nRemotePort);
				AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
				m_sSocket = 0;
				m_bConn = false;
			}
		}
		Sleep(3000);
	}
	return TRUE;
}
//
UINT tcpTransMainClient(LPVOID Ptr)
{
	CNetTcpTransClient *pp = (CNetTcpTransClient *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartTcpClientConn();
	return 1;
}
bool CNetTcpTransClient::StartClientRecvSel()
{	
    //��������
	CString strTmp;
	unsigned char recvData[DATA_FRAME_LEN+1];
	while(m_bRecv)
	{
		if(m_sSocket <= 0 || false == m_bConn)
		{
			Sleep(1000);
			continue;
		}
		//����recv���������ݡ�
		memset(recvData,0,sizeof(recvData));          
		int framelen = RevceiveData((char *)recvData,m_sSocket);//����һ��������֡
		if(-3 == framelen)
		{
			//�Է��ر�����
			strTmp.Format("�������ر�����[%d]",framelen);
			AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
		    closesocket(m_sSocket);
			m_sSocket = 0;
			m_bConn = false;
			continue;
		}

		if(framelen < 0)
		{
			//δ֪����
			strTmp.Format("��ʱ����������[%d]",framelen);
			AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
		    closesocket(m_sSocket);
			m_sSocket = 0;
			m_bConn = false;
			continue;
		}
		//
		if(0x0B == recvData[0])
		{
			BOOL bol = CheckFrame(recvData,framelen);
			if(FALSE == bol)
			{
				strTmp.Format("[0B]У��֡ʧ��[%d]���",framelen);
				AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
				continue;
			}
		}
		m_recvTime = time(NULL);
		//������������֡������������
		m_pRecvDataQueue->InsertQueue(recvData,framelen);		
	}
	//
	//
	return true;				
}
UINT clientRecvMain(LPVOID Ptr)
{
	CNetTcpTransClient *pp = (CNetTcpTransClient *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartClientRecvSel();
	return 1;
}
bool CNetTcpTransClient::StartClientSendSel()
{	
    //
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
	while(m_bSend)
	{
		if(m_sSocket <= 0 || false == m_bConn)
		{
			dataSize = m_pSendDataQueue->GetDataRecordSum();
			if(dataSize > 1000)m_pSendDataQueue->RemoveAll();
			Sleep(1000);
			continue;
		}
		//
		while(!m_pSendDataQueue->EmptyQueue()) 
		{
			dataSize = m_pSendDataQueue->GetDataRecordSum();
			if(dataSize <= 0) 
				break;
			//
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
			m_pSendDataQueue->ReadQueue(tmpBuf, &dataSize);
			send(m_sSocket,(char *)tmpBuf,dataSize,0);
			m_sendTime = time(NULL);
		}
		time_t time_now = time(NULL);
		if(time_now - m_sendTime > HEART_BEAT_TIME)
		{
			send(m_sSocket,m_heartBeatBuf,sizeof(m_heartBeatBuf),0);
			m_sendTime = time_now;
		}
		Sleep(10);
	}
    //
	return true;				
}
UINT clientSendMain(LPVOID Ptr)
{
	CNetTcpTransClient *pp = (CNetTcpTransClient *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartClientSendSel();
	return 1;
}
//
BOOL CNetTcpTransClient::StartTcpClient(const char *svrId,const char *clientId,const char *netIp,u_short netPort,CLogMessage *plogMsg,CDataQueue *pRecvDataQueue)
{
	if((NULL == netIp) || NULL == plogMsg || NULL == pRecvDataQueue)
	{
		return FALSE;
	}
	////////////
	CString strTmp;
	m_sRemoteIp = netIp;
	m_nRemotePort = netPort;
	m_svrId = svrId;
	m_clientId = clientId;
	//m_pDataQueue = pdataQueue;
	m_pLogMsg = plogMsg;
	m_pRecvDataQueue = pRecvDataQueue;
	//m_phConnFreshEvent = phConnFreshEvent;
	m_pSendDataQueue = new CDataQueue;
	memset(m_recvData,0,sizeof(m_recvData));
	m_sSocket = 0;
	m_bConn = false;
	m_bactiveThread = TRUE;
	//
	// m_heartBeatBuf[13];
	m_heartBeatBuf[0] = 0x0B;
	//����
	m_heartBeatBuf[1] = 0x0A;
	m_heartBeatBuf[2] = 0x00;
	//����
	m_heartBeatBuf[3] = 0x01;
	//����
	m_heartBeatBuf[4] = 0x01;
	//svr_id
	memcpy(&m_heartBeatBuf[5],m_svrId.c_str(),3);
	//client_id
	memcpy(&m_heartBeatBuf[8],m_clientId.c_str(),3);
	//У�飬�������ݲ�У��
	m_heartBeatBuf[11] = 0x00;
	m_heartBeatBuf[12] = 0x0E;
	//
	m_netThread = AfxBeginThread(tcpTransMainClient,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_netThread->m_bAutoDelete = TRUE;
	m_netThread->ResumeThread();
	//
	m_bRecv = true;
	m_recvThread = AfxBeginThread(clientRecvMain,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_recvThread->m_bAutoDelete = TRUE;
	m_recvThread->ResumeThread();
	//
	m_bSend = true;
	m_sendThread = AfxBeginThread(clientSendMain,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_sendThread->m_bAutoDelete = TRUE;
	m_sendThread->ResumeThread();
	//
	return TRUE;
}
//
BOOL CNetTcpTransClient::AddLogMsg(int grade, const char *sbody)
{
	stuLogContent msg;
    msg.item           = 0;
	msg.type           = TYPE_LOG_TCP;
	msg.grade          = grade;
	msg.body           = sbody;
	msg.title          = "TCP";
	msg.datetime       = time(NULL);
	m_pLogMsg->AddLogMessage(msg);
	return TRUE;
}
//
BOOL CNetTcpTransClient::StopTcpClient()
{
	m_bactiveThread = FALSE;
	m_isStart = FALSE;
	m_bRecv = false;
	m_bSend = false;
	if(m_sSocket > 0)
	{
		shutdown(m_sSocket, SD_RECEIVE);
		Sleep(50);
		closesocket(m_sSocket);
		m_sSocket = NULL;
	}
	Sleep(1000);
	//
	if(m_pSendDataQueue != NULL)
	{
		m_pSendDataQueue->RemoveAll();
		delete m_pSendDataQueue;
		m_pSendDataQueue = NULL;
	}
	return TRUE;
}
