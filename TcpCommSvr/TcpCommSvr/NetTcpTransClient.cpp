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

//触发退出接收事件
void CNetTcpTransClient::SetThreadStatus(BOOL bol) 
{
	m_bactiveThread = bol;
	m_bRecv = false;
	m_bSend = false;
}
//添加发送卡片数据
int CNetTcpTransClient::SetSendCardData(const stuCardInOutStatus *stuCIOS)
{
	char tmpBuf[DATA_FRAME_LEN];
	short len = 0;
	memset(tmpBuf,0,sizeof(tmpBuf));
	tmpBuf[len] = 0x0B;
	len += 1;
	len += 2;
	//帧类型,0x01心跳,0x02卡片数据,0x03考勤数据,0x04配置,0x05结点连接状态
	tmpBuf[len] = CARD_DATA;
	len += 1;
	//条数
	tmpBuf[len] = 0x01;
	len += 1;
	//svr_id
	memcpy(&tmpBuf[len],m_svrId.c_str(),3);
	len += 3;
	//station_id
	memcpy(&tmpBuf[len],m_stationId.c_str(),3);
	len += 3;
	//card_no
	unsigned int card_no = stuCIOS->card_no;
	memcpy(&tmpBuf[len],&card_no,sizeof(unsigned int));
	len += sizeof(unsigned int);
	//io_flg
	tmpBuf[len] = stuCIOS->io_flg;
	len += 1;
	//pre_room_no
	unsigned int pre_room_no = stuCIOS->pre_room_no;
	memcpy(&tmpBuf[len],&pre_room_no,sizeof(unsigned int));
	len += sizeof(unsigned int);
	//room_no
	unsigned int room_no = stuCIOS->room_no;
	memcpy(&tmpBuf[len],&room_no,sizeof(unsigned int));
	len += sizeof(unsigned int);
	//lst_time
	time_t lst_time = stuCIOS->lst_time;
	memcpy(&tmpBuf[len],&lst_time,sizeof(time_t));
	len += sizeof(time_t);
	//校验，保留，暂不校验
	tmpBuf[len] = 0x00;
	len += 1;
	tmpBuf[len] = 0x0E;
	len += 1;
	//
	short lenTmp = len - 3;
	memcpy(&tmpBuf[1],&lenTmp,2);
	//
	m_pSendDataQueue->InsertQueue((BYTE *)tmpBuf,len);
	return 0;
}
//添加发送考勤数据
int CNetTcpTransClient::SetSendAttData(const stuAttendanceData *stuAtt)
{
	char tmpBuf[DATA_FRAME_LEN];
	short len = 0;
	memset(tmpBuf,0,sizeof(tmpBuf));
	tmpBuf[len] = 0x0B;
	len += 1;
	len += 2;
	//帧类型,0x01心跳,0x02卡片数据,0x03考勤数据,0x04配置,0x05结点连接状态
	tmpBuf[len] = ATT_DATA;
	len += 1;
	//条数
	tmpBuf[len] = 0x01;
	len += 1;
	//svr_id
	memcpy(&tmpBuf[len],m_svrId.c_str(),3);
	len += 3;
	//station_id
	memcpy(&tmpBuf[len],m_stationId.c_str(),3);
	len += 3;
	//card_no
	unsigned int card_no = stuAtt->card_no;
	memcpy(&tmpBuf[len],&card_no,sizeof(unsigned int));
	len += sizeof(unsigned int);
	//fst_time
	time_t fst_time = stuAtt->fst_time;
	memcpy(&tmpBuf[len],&fst_time,sizeof(time_t));
	len += sizeof(time_t);
	//lst_time
	time_t lst_time = stuAtt->lst_time;
	memcpy(&tmpBuf[len],&lst_time,sizeof(time_t));
	len += sizeof(time_t);
	//校验，保留，暂不校验
	tmpBuf[len] = 0x00;
	len += 1;
	tmpBuf[len] = 0x0E;
	len += 1;
	//
	short lenTmp = len - 3;
	memcpy(&tmpBuf[1],&lenTmp,2);
	//
	m_pSendDataQueue->InsertQueue((BYTE *)tmpBuf,len);
	return 0;
}
//接收一个完整的数据包
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
	//开始接收数据
    //接帧头0x0B
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
			//超时或连接断开
			return totalsize;
		}
		else
		{
			//
		    if(0 == totalsize)
			{
				//对方正常关闭连接
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
		//接收前三个字节
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
				//对方正常关闭连接
				return -3;
			}
			memcpy(pp,tmpBuf,totalsize);
			pp += totalsize;
			tmplen += totalsize;
			//
			//CTestLog::LogDebugInfo(".\\data\\","testdebug1.txt",recvBuf,tmplen);
		}
		//数据长度
		datalen = (unsigned char)recvBuf[HEAD_LEN-2] + (unsigned char)recvBuf[HEAD_LEN-1] * 256;
		//接收数据体
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
				//对方正常关闭连接
				return -3;
			}
			memcpy(pp,tmpBuf,totalsize);
			pp += totalsize;
			tmplen += totalsize;
			//
			//CTestLog::LogDebugInfo(".\\data\\","testdebug2.txt",recvBuf,tmplen);
		}
	}
	///////////////////调试
	//CTestLog::LogDebugInfo(".\\data\\","testdebug.txt",recvBuf,framelen);
	////////////////////////
    return framelen;	
}
//校验，正常读卡报文，0B
BOOL CNetTcpTransClient::CheckFrame(const unsigned char *dataframe, //待校验的帧
				int framelen)                    //帧长度
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
//连接服务器
BOOL CNetTcpTransClient::StartTcpClientConn()
{
	CString strTmp;
	while(m_bactiveThread)
	{  
		if(m_sSocket <= 0 || false == m_bConn)
		{
			//连接服务器
			m_sSocket = socket(AF_INET,SOCK_STREAM,0);
			if(INVALID_SOCKET == m_sSocket)
			{}
			SOCKADDR_IN clientsock_in;
			clientsock_in.sin_family=AF_INET;
			clientsock_in.sin_port=htons(m_nRemotePort);
			clientsock_in.sin_addr.S_un.S_addr=inet_addr(m_sRemoteIp.c_str());
			
			int ret = connect(m_sSocket,(SOCKADDR*)&clientsock_in,sizeof(SOCKADDR));//开始连接
			if(ret >= 0)
			{
				strTmp.Format("连接服务器成功[%s][%d]",m_sRemoteIp.c_str(),m_nRemotePort);
				AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
				int time_recv = CLIENT_SOCKET_TIMEOUT; //超时间
	            setsockopt(m_sSocket,SOL_SOCKET, SO_RCVTIMEO, (char*)&time_recv, sizeof(int));
				m_recvTime = time(NULL);
				m_sendTime = time(NULL);
				m_bConn = true;
			}
			else
			{
				strTmp.Format("连接服务器失败[%s][%d]",m_sRemoteIp.c_str(),m_nRemotePort);
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
    //接收数据
	CString strTmp;
	unsigned char recvData[DATA_FRAME_LEN+1];
	while(m_bRecv)
	{
		if(m_sSocket <= 0 || false == m_bConn)
		{
			Sleep(1000);
			continue;
		}
		//调用recv，接收数据。
		memset(recvData,0,sizeof(recvData));          
		int framelen = RevceiveData((char *)recvData,m_sSocket);//接收一个完整的帧
		if(-3 == framelen)
		{
			//对方关闭连接
			strTmp.Format("服务器关闭连接[%d]",framelen);
			AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
		    closesocket(m_sSocket);
			m_sSocket = 0;
			m_bConn = false;
			continue;
		}

		if(framelen < 0)
		{
			//未知错误
			strTmp.Format("超时或其它错误[%d]",framelen);
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
				strTmp.Format("[0B]校验帧失败[%d]编号",framelen);
				AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
				continue;
			}
		}
		m_recvTime = time(NULL);
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
		if(NULL != m_pSendDataQueue)
		{
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
BOOL CNetTcpTransClient::StartTcpClient(const char *svrId,const char *stationId,const char *netIp,u_short netPort,CLogMessage *plogMsg)
{
	if((NULL == netIp) || NULL == plogMsg )
	{
		return FALSE;
	}
	////////////
	CString strTmp;
	m_sRemoteIp = netIp;
	m_nRemotePort = netPort;
	m_svrId = svrId;
	m_stationId = stationId;
	//m_pDataQueue = pdataQueue;
	m_pLogMsg = plogMsg;
	//m_phConnFreshEvent = phConnFreshEvent;
	m_pSendDataQueue = new CDataQueue;
	memset(m_recvData,0,sizeof(m_recvData));
	m_sSocket = 0;
	m_bConn = false;
	m_bactiveThread = TRUE;
	//
    // m_heartBeatBuf[13];//
	m_heartBeatBuf[0] = 0x0B;
	//长度
	m_heartBeatBuf[1] = 0x0A;
	m_heartBeatBuf[2] = 0x00;
	//类型
	m_heartBeatBuf[3] = 0x01;
	//条数
	m_heartBeatBuf[4] = 0x01;
	//svr_id
	memcpy(&m_heartBeatBuf[5],m_svrId.c_str(),3);
	//station_id
	memcpy(&m_heartBeatBuf[8],m_stationId.c_str(),3);
	//校验，保留，暂不校验
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
