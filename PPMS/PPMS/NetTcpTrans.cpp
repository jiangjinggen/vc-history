// NetTcpTrans.cpp: implementation of the CNetTcpTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetTcpTrans.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetTcpTrans::CNetTcpTrans()
{
	WSADATA wsaData;
    //
	m_sServer = NULL;
	m_nServerPort = -1;
	m_netThread = NULL;
	//
	m_bWSAStartup = TRUE;
	m_bactiveThread = TRUE;
	m_isStart = FALSE;
	//
	m_vecSockInfo.clear();
	//
	FD_ZERO(&m_socketSet);
	m_socketSet.fd_count = 0;
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
	m_connType = 0;
}

CNetTcpTrans::~CNetTcpTrans()
{
    //RealseTcp();
	//WSACleanup();
}

//触发退出串口接收事件
void CNetTcpTrans::SetThreadStatus(BOOL bol) 
{
	m_bactiveThread = bol;
}
//接收一个完整的数据包
int CNetTcpTrans::RevceiveData(char *recvBuf,SOCKET sscoket) 
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
	char stmp[10];
	//开始接收数据
    //接帧头0x0B
	iNum = 0;
	memset(&m_stuRecvBufErr,0,sizeof(m_stuRecvBufErr));
	while(TRUE)
	{
		memset(tmpBuf,0,sizeof(tmpBuf));
		//bol = m_serial->Receive(tmpBuf,1,&totalsize);//接收数据
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
			if((0x0B != tmpBuf[0])&&(0x2A != tmpBuf[0]))
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
	int tmp_head_len = 0;
	if(IS_NET == m_connType)
	{
		tmp_head_len = HEAD_LEN + 1;
	}
	else
	{
		tmp_head_len = HEAD_LEN;
	}
	if(0x0B == recvBuf[0])
	{
		//接收前四个字节
		icount = 0;
		while(tmplen < tmp_head_len)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			//bol = m_serial->Receive(tmpBuf,HEAD_LEN - tmplen,&totalsize);//接收数据
			totalsize = recv(sscoket, tmpBuf, tmp_head_len - tmplen, 0);
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
		datalen = (unsigned char)recvBuf[tmp_head_len-1];
		//接收数据体
		framelen = tmp_head_len + datalen;
		//
		icount = 0;
		while(tmplen < framelen)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			//bol = m_serial->Receive(tmpBuf,framelen - tmplen,&totalsize);//接收数据
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
	if(0x2A == recvBuf[0])
	{
		//接收前四个字节
		icount = 0;
		while(tmplen < HEAD_LEN_2A)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			//bol = m_serial->Receive(tmpBuf,HEAD_LEN - tmplen,&totalsize);//接收数据
			totalsize = recv(sscoket, tmpBuf, HEAD_LEN_2A - tmplen, 0);
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
		//卡片张数最多十张
		memset(stmp,0,sizeof(stmp));
		memcpy(stmp,&recvBuf[1],2);
		//
		if((stmp[0] < 0x30)||(stmp[0] > 0x31)||(stmp[0] < 0x30)||(stmp[0] > 0x39))
		{
			return -4;
		}
		datalen = atoi(stmp);
		datalen = datalen*24 + 4;
		//接收数据体
		framelen = HEAD_LEN_2A + datalen;
		//
		icount = 0;
		while(tmplen < framelen)
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			//bol = m_serial->Receive(tmpBuf,framelen - tmplen,&totalsize);//接收数据
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
BOOL CNetTcpTrans::CheckFrame(const unsigned char *dataframe, //待校验的帧
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
//校验，出入库报文，2A
BOOL CNetTcpTrans::CheckFrame2A(const unsigned char *dataframe, //待校验的帧
				int framelen)                    //帧长度
{
	unsigned short chcc = 0;
	unsigned short chres = dataframe[framelen-4] + dataframe[framelen-3] * 256;
	for(int i = 1; i < framelen - 4; i++)
	{
		chcc += dataframe[i];
	}
	//chcc = ~chcc;
	//chcc += 1;
	if(chres == chcc)
	{
        return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//
UINT tcpTransRecv(LPVOID Ptr)
{
	stuThreadInfo *threadInfo = (stuThreadInfo *)Ptr;
	CNetTcpTrans *pp = (CNetTcpTrans *)(threadInfo->ths);//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartTcpComRecv(threadInfo->sock);
	return 1;
}
BOOL CNetTcpTrans::StartTcpComRecv(SOCKET sSocket)
{	
	//可在此设置SOCKET为非阻塞
	int timeout = SOCKET_TIMEOUT; //超时间
	setsockopt(sSocket,SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int));
	//接收数据
	unsigned char recvData[DATA_FRAME_LEN+1];
	CString strTmp;
	while(TRUE)
	{
		memset(recvData,0,sizeof(recvData));
		int framelen = RevceiveData((char *)recvData,sSocket);//接收一个完整的帧
		if(-3 == framelen)
		{
			//对方关闭连接
			strTmp.Format("对方关闭连接[%d][%d]",framelen,errno);
			CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
			break;
		}

		if(framelen < 0)
		{
			//未知错误
			strTmp.Format("超时或其它错误[%d][%d]",framelen,errno);
			CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
			break;
		}
		BOOL bol = CheckFrame(recvData,framelen);
		if(TRUE == bol)
		{
			strTmp.Format("CheckFrame failed[%d]",framelen);
			CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
			//CTestLog::LogDebugInfo(".\\data\\","testdebug_chk.txt",m_recvData,framelen);
			continue;
		}
		//将正常的数据帧加入待处理队列
		m_pDataQueue->InsertQueue(recvData,framelen);
	}
	//关闭连接
	closesocket(sSocket);
	//
	return TRUE;
}
BOOL CNetTcpTrans::StartTcpComThread()
{	
	
	CString strTmp;
	while(m_bactiveThread)
	{
		//accept客户
		if(!m_isStart) break;
		if(NULL == m_sServer) break;

		SOCKET	sClient;	
		int		iAddrSize;
		struct	sockaddr_in addr;
		char clientIp[256];

		iAddrSize = sizeof(addr);

		sClient = accept(m_sServer, (struct sockaddr *)&addr, &iAddrSize);
		
		if(sClient == SOCKET_ERROR)
		{
			strTmp.Format("accept failed[%d]",1);
			CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
			err = WSAGetLastError();
			break;
		}

		memset(clientIp,0,sizeof(clientIp));
		sprintf(clientIp, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, 
										 addr.sin_addr.S_un.S_un_b.s_b2,
										 addr.sin_addr.S_un.S_un_b.s_b3,
										 addr.sin_addr.S_un.S_un_b.s_b4);
		//
		strTmp.Format("accept success[%s]",clientIp);
		CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
		//开启接收线程
		CWinThread *recvThread;
		stuThreadInfo threadInfo;
		threadInfo.ths = (LPVOID)this;
		threadInfo.sock = sClient;
		recvThread = AfxBeginThread(tcpTransRecv,(LPVOID)&threadInfo,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
		recvThread->m_bAutoDelete = TRUE;
		recvThread->ResumeThread();
        Sleep(100);
	}
	//
	RealseTcp();
    //
	return TRUE;				
}
BOOL CNetTcpTrans::StartTcpComSel()
{	
    //FD_CLR(s,*set);从set集合中删除s套接字。
    //FD_ISSET(s,*set);检查s是否为set集合的成员。
    //FD_SET(s,*set);将套接字加入到set集合中。
    //FD_ZERO(*set);将set集合初始化为空集合。
	stuSockInfo sockInfo;
	CString strTmp;
	unsigned char recvData[DATA_FRAME_LEN+1];
	unsigned char sendData[DATA_FRAME_LEN+1];
	//
	int uClientPort = 0;
	//
	int readerNo = 0;
	//
	timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_sec = 10;
	//
	SOCKET	acceptSocket;
	SOCKET  sSocket;
	int		iAddrSize;
	struct	sockaddr_in addr;
	char clientIp[256];
	char stmp[256];

	iAddrSize = sizeof(addr);

	while(m_bactiveThread)
	{
		//accept客户
		if(!m_isStart)break;
		if(NULL == m_sServer)break;

        FD_ZERO(&m_readSet);
		FD_ZERO(&m_writeSet);
		m_readSet=m_socketSet;
		m_writeSet=m_socketSet;

		//同时检查套接字的可读可写性。
		//&m_writeSet
		int ret=select(0,&m_readSet,NULL,NULL,NULL);//&timeout);//最后一个参数为等待时间，传入NULL，则永久等待。传入0立即返回。不要勿用。
		if(!m_isStart)break;
		if(ret==SOCKET_ERROR)
		{
			//需相应处理
			strTmp.Format("select出错[%d][%d]",ret,errno);
			AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
			//
			break;
		}

		//是否存在客户端的连接请求。
		if(FD_ISSET(m_sServer,&m_readSet))//在readset中会返回已经调用过listen的套接字。
		{
			if(m_socketSet.fd_count >= FD_SETSIZE)continue;
			acceptSocket=accept(m_sServer,(struct sockaddr *)&addr, &iAddrSize);
			if(acceptSocket==INVALID_SOCKET)
			{
				//需相应处理
				strTmp.Format("accept出错[%d][%d]sel",acceptSocket,errno);
				AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
				break;
			}
			else
			{
				FD_SET(acceptSocket,&m_socketSet);
				memset(clientIp,0,sizeof(clientIp));
				strcpy(clientIp,inet_ntoa(addr.sin_addr));//IP 
				uClientPort = addr.sin_port;
				memset(&sockInfo,0,sizeof(stuSockInfo));
				sockInfo.sockThread = NULL;
				sockInfo.sock = acceptSocket;
				sockInfo.ip = clientIp;
				sockInfo.port = uClientPort;
				sockInfo.reader_no = 0;
				sockInfo.last_time = time(NULL);
				sockInfo.isUpdateReader = TRUE;
				AddSockInfo(sockInfo);
				//
				strTmp.Format("[%s]客户已连接[%d]",clientIp,uClientPort);
				AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
				//
				SetEvent(*m_phConnFreshEvent);
				int time_recv = SOCKET_TIMEOUT; //超时间
	            setsockopt(acceptSocket,SOL_SOCKET, SO_RCVTIMEO, (char*)&time_recv, sizeof(int));
			}
		}
        //
		int vecIndex = 0;
		for(u_int i = 0;i < m_socketSet.fd_count;i++)
		{
			//
			sSocket = m_socketSet.fd_array[i];
            //	
			if(m_sServer == sSocket)
			{
				continue;
			}
			//
			vecIndex = FindSockInfo(sSocket);
			//判断是否超时未收到数据，如果超时，则认为客户端已断开
			if(vecIndex >= 0)
			{
				if((time(NULL) - m_vecSockInfo[vecIndex].last_time) > CONN_TIMEOUT)
				{
					//超时未收到数据
					strTmp.Format("[%s]timeout关闭连接[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
					AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					FD_CLR(sSocket,&m_socketSet);
					DelSockInfo(sSocket);
					continue;
				}
			}
			else
			{
				continue;
			}
			//
			if(FD_ISSET(sSocket,&m_readSet))
			{
				//调用recv，接收数据。
				memset(recvData,0,sizeof(recvData));          
				int framelen = RevceiveData((char *)recvData,sSocket);//接收一个完整的帧
				if(-3 == framelen)
				{
					//对方关闭连接
					strTmp.Format("[%s]客户关闭连接[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
					AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					FD_CLR(sSocket,&m_socketSet);
					DelSockInfo(sSocket);
					continue;
				}

				if(framelen < 0)
				{
					//未知错误
					strTmp.Format("[%s]超时或其它错误[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
					AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					FD_CLR(sSocket,&m_socketSet);
					DelSockInfo(sSocket);
					continue;
				}
				//更新最后收到数据的时间
				m_vecSockInfo[vecIndex].last_time = time(NULL);
				//
				if(0x0B == recvData[0])
				{
					BOOL bol = CheckFrame(recvData,framelen);
					if(FALSE == bol)
					{
						strTmp.Format("[0B]校验帧失败[%d]编号[%d]",framelen,m_vecSockInfo[vecIndex].reader_no);
						AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
						continue;
					}
					//更新读卡器编号
					//mod by jjg 20171101 多一个字节，少一个字节问题
					if(IS_NET == m_connType)
	                {
					    readerNo = recvData[2];
					}
					else
					{
					    readerNo = recvData[1];
					}
					//
					if(m_vecSockInfo[vecIndex].isUpdateReader)
					{
						SetSockReaderNo(sSocket,readerNo);
						m_vecSockInfo[vecIndex].isUpdateReader = FALSE;
						SetEvent(*m_phConnFreshEvent);
					}
					//
					memset(sendData,0,sizeof(sendData));
					memcpy(sendData,"OK",2);
					send(sSocket,(char *)sendData,2,0);
				}
				if(0x2A == recvData[0])
				{
					//BOOL bol = CheckFrame2A(recvData,framelen);
					//if(FALSE == bol)
					//{
					//	strTmp.Format("[2A]校验帧失败[%d]编号[%d]",framelen,m_vecSockInfo[vecIndex].reader_no);
					//	AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					//	continue;
					//}
					//发送确认,123OK20150108123456
					memset(sendData,0,sizeof(sendData));
					time_t nowtime = time(NULL);
					struct tm *now = NULL;
					now = localtime(&nowtime);
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"OK%d%02d%02d%d%02d%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
                    memcpy(sendData,&recvData[3],3);
					memcpy(&sendData[3],stmp,16);
                    sendData[19] = 0x0D;
					sendData[20] = 0x0A;
					send(sSocket,(char *)sendData,21,0);

				}
				//将正常的数据帧加入待处理队列
				m_pDataQueue->InsertQueue(recvData,framelen);
			}
			//
			//if(FD_ISSET(sSocket,&m_writeSet))
			//{
			//	//调用send，发送数据。
			//}
		}

	}
	//
	RealseTcp();
    //
	return TRUE;				
}
UINT tcpTransMain(LPVOID Ptr)
{
	CNetTcpTrans *pp = (CNetTcpTrans *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	//pp->StartTcpComThread();
	pp->StartTcpComSel();
	return 1;
}

BOOL CNetTcpTrans::StartTcpServer(const char *netIp,u_short netPort,CDataQueue *pdataQueue,CLogMessage *plogMsg,HANDLE *phConnFreshEvent,int conn_type)
{
	if((NULL == netIp) || (NULL == pdataQueue) || NULL == plogMsg || NULL == phConnFreshEvent)
	{
		return FALSE;
	}
	m_connType = conn_type;
	////////////
	CString strTmp;
	m_nServerPort = netPort;
	m_pDataQueue = pdataQueue;
	m_pLogMsg = plogMsg;
	m_phConnFreshEvent = phConnFreshEvent;
	memset(m_recvData,0,sizeof(m_recvData));
	m_bactiveThread = TRUE;
	//
    SOCKADDR_IN local;
	//创建监听socket
    m_sServer = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sServer == SOCKET_ERROR)
    {
		AddLogMsg(GRADE_LOG_ERROR,"初始化失败");
        err = WSAGetLastError();
        return FALSE;
    }
	
	int value;
	int optlen  = sizeof(value);
	value = 1;
	//添参数	
	memset(&local,0,sizeof(SOCKADDR_IN));
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	//local.sin_addr.s_addr=inet_addr("192.100.88.220");
    local.sin_family = AF_INET;
    local.sin_port = htons(netPort);
	local.sin_addr.S_un.S_addr = INADDR_ANY; //本机

	if(bind(m_sServer, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
		//
		//CString strTmp;
		strTmp.Format("bind套接字失败[%d]",netPort);
		AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		//
		err = WSAGetLastError();
		closesocket(m_sServer);
        return TRUE;
    }

	//开始侦听
    if(listen(m_sServer, 100) != 0)
	{
		//
		//CString strTmp;
		strTmp.Format("listen套接字失败[%d]",netPort);
		AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		//
		err = WSAGetLastError();
		closesocket(m_sServer);
        return TRUE;
    }
	m_nServerPort = netPort;
	//
	FD_ZERO(&m_socketSet);
	m_socketSet.fd_count = 0;
	FD_SET(m_sServer,&m_socketSet);
	m_isStart = TRUE;
	//CString strTmp;
	strTmp.Format("服务器启动，端口[%d]",netPort);
	AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
	//
	m_netThread = AfxBeginThread(tcpTransMain,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_netThread->m_bAutoDelete = TRUE;
	m_netThread->ResumeThread();

	return TRUE;
}
//
BOOL CNetTcpTrans::AddSockInfo(stuSockInfo sockinfo)
{
	//如果多线程要加锁
	m_vecSockInfo.push_back(sockinfo);
	return TRUE;
}
//
BOOL CNetTcpTrans::GetAllSockInfo(vector<stuSockInfo> &vec_sockinfo)
{
	std::copy(m_vecSockInfo.begin(),m_vecSockInfo.end(),std::back_inserter(vec_sockinfo));
	//vec_sockinfo = m_vecSockInfo;
	return TRUE;
}
//返回索引
int CNetTcpTrans::FindSockInfo(SOCKET sSocket)
{
	int vecIndex = -1;
	for(int i = 0; i < m_vecSockInfo.size(); i++)
	{
		if(sSocket == m_vecSockInfo[i].sock)
		{
            vecIndex = i;
		}
	}
	return vecIndex;
}
//根据读卡器编号，返回SockInfo
BOOL CNetTcpTrans::FindSockInfoByReaderNo(int readerNo,stuSockInfo &sockInfo)
{
	BOOL bFind = FALSE;
	for(int i = 0; i < m_vecSockInfo.size(); i++)
	{
		if(readerNo == m_vecSockInfo[i].reader_no)
		{
			sockInfo.sock = m_vecSockInfo[i].sock;
			bFind = TRUE;
		}
	}
	return bFind;
}
//
BOOL CNetTcpTrans::DelSockInfo(SOCKET sSocket)
{
	BOOL bFind = FALSE;
	vector<stuSockInfo>::iterator iter;
	for(iter = m_vecSockInfo.begin(); iter != m_vecSockInfo.end(); iter++)
	{
		if(sSocket == iter->sock)
		{
            closesocket(sSocket);
			m_vecSockInfo.erase(iter);
			SetEvent(*m_phConnFreshEvent);
			break;
		}
	}
	return bFind;
}
//
BOOL CNetTcpTrans::SetSockReaderNo(SOCKET sSocket,int readNo)
{
	BOOL bFind = FALSE;
	vector<stuSockInfo>::iterator iter;
	for(iter = m_vecSockInfo.begin(); iter != m_vecSockInfo.end(); iter++)
	{
		if(sSocket == iter->sock)
		{
			iter->reader_no = readNo;
			break;
		}
	}
	return bFind;
}
//
BOOL CNetTcpTrans::AddLogMsg(int grade, const char *sbody)
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
BOOL CNetTcpTrans::StopTcpServer()
{
	//RealseTcp();
	m_bactiveThread = FALSE;
	m_isStart = FALSE;
	shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	return TRUE;
}
//
BOOL CNetTcpTrans::GetTcpStatus()
{
	return m_isStart;
}
//
int CNetTcpTrans::GetSocketNum()
{
	if(m_socketSet.fd_count <= 0)return 0;
	return (m_socketSet.fd_count - 1);
}
//
BOOL CNetTcpTrans::RealseTcp()
{    
    CString strTmp;
	strTmp.Format("服务器已关闭，端口[%d]",m_nServerPort);
	AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
	//
	m_bactiveThread = FALSE;

	//关闭所有客户连接
	SOCKET sSocket;
	for(u_int i = 0; i < m_socketSet.fd_count; i++)
	{
		//
		sSocket = m_socketSet.fd_array[i];
		FD_CLR(sSocket,&m_socketSet);
		closesocket(sSocket);
		//
	}
	FD_ZERO(&m_socketSet);
	m_socketSet.fd_count = 0;
	//
	m_vecSockInfo.clear();
	m_isStart = FALSE;
	SetEvent(*m_phConnFreshEvent);
	//
	if( m_netThread != NULL )
	{
		if (WaitForSingleObject(m_netThread->m_hThread, 
			THREAD_SHUTDOWN_TIME) == WAIT_TIMEOUT)
		{
		    TerminateThread(m_netThread->m_hThread,0);
		}
		
		delete m_netThread;
		m_netThread = NULL;
	}
	//关闭服务器socket
	if(NULL == m_sServer) 
	{
		return TRUE;
	}
	if(!m_isStart) 
	{
		m_sServer = NULL;
		return TRUE;
	}
	shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	return TRUE;
}