#include "StdAfx.h"
#include "NodeSvr.h"

CNodeSvr::CNodeSvr(void)
{
}

CNodeSvr::~CNodeSvr(void)
{
}
//接收一个完整的数据包
int CNodeSvr::RevceiveData(char *recvBuf,SOCKET sscoket) 
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
bool CNodeSvr::CheckFrame(const unsigned char *dataframe, //待校验的帧
				int framelen)                    //帧长度
{
	return true;
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
        return true;
	}
	else
	{
		return false;
	}
}
bool CNodeSvr::StartNodeRecvSel()
{	
    //FD_CLR(s,*set);从set集合中删除s套接字。
    //FD_ISSET(s,*set);检查s是否为set集合的成员。
    //FD_SET(s,*set);将套接字加入到set集合中。
    //FD_ZERO(*set);将set集合初始化为空集合。
	stuSockInfo sockInfo;
	CString strTmp;
	unsigned char recvData[DATA_FRAME_LEN+1];
	//
	int uClientPort = 0;
	//
	char node_id[10];
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
	iAddrSize = sizeof(addr);

	while(m_bRecv)
	{
		//accept客户
		if(!m_isStart)break;
		if(NULL == m_sServer)break;

        FD_ZERO(&m_readSet);
		m_readSet=m_socketSet;

		//同时检查套接字的可读可写性。
		//&m_readSet
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
				sockInfo.sock = acceptSocket;
				sockInfo.ip = clientIp;
				sockInfo.port = uClientPort;
				sockInfo.node_id = "";
				sockInfo.recv_time = time(NULL);
				sockInfo.snd_time = time(NULL);
				sockInfo.is_update_node = false;
				AddSockInfo(sockInfo);
				//
				strTmp.Format("[%s]客户已连接[%d]",clientIp,uClientPort);
				AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
				//
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
			vecIndex = FindSockIndex(sSocket);
			//判断是否超时未收到数据，如果超时，则认为客户端已断开
			if(vecIndex >= 0)
			{
				if((time(NULL) - m_vecSockInfo[vecIndex].recv_time) > CONN_TIMEOUT)
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

				if(framelen < BASE_FRAME_LEN)
				{
					//未知错误
					strTmp.Format("[%s]超时或其它错误[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
					AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					FD_CLR(sSocket,&m_socketSet);
					DelSockInfo(sSocket);
					continue;
				}
				//更新最后收到数据的时间
				m_vecSockInfo[vecIndex].recv_time = time(NULL);
				//
				if(0x0B == recvData[0])
				{
					BOOL bol = CheckFrame(recvData,framelen);
					if(FALSE == bol)
					{
						strTmp.Format("[0B]校验帧失败[%d]编号[%d]",framelen,m_vecSockInfo[vecIndex].node_id);
						AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
						continue;
					}
					//更新结点编号
					memset(node_id,0,sizeof(node_id));
					if(!m_vecSockInfo[vecIndex].is_update_node)
					{
						memcpy(node_id,&recvData[HEAD_LEN+5],3);
						m_vecSockInfo[vecIndex].node_id = node_id;
						m_vecSockInfo[vecIndex].is_update_node = true;
						map<string,stuNodeInfo>::iterator iter;
						iter = m_mapNodeInfo.find(m_vecSockInfo[vecIndex].node_id);
						if(iter != m_mapNodeInfo.end())
						{
							if(true == iter->second.conn_status)
							{
								//已连接节点
								strTmp.Format("[%s]节点已连接[%s]",node_id,m_vecSockInfo[vecIndex].ip.c_str());
								//AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
								//FD_CLR(sSocket,&m_socketSet);
								//DelSockInfo(sSocket);
							}
							else
							{
								iter->second.conn_status = true;
								//iter->second.node_ip = m_vecSockInfo[vecIndex].ip;
								iter->second.sock = sSocket;
							}
						}
						else
						{
							//未定义节点
							strTmp.Format("[%s]节点未定义[%s]",node_id,m_vecSockInfo[vecIndex].ip.c_str());
							AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
							FD_CLR(sSocket,&m_socketSet);
							DelSockInfo(sSocket);
							continue;
						}
					}
					//
				}
				//将正常的数据帧加入待处理队列
				m_pRecvQueue->InsertQueue(recvData,framelen);
			}
			//
		}
		Sleep(10);
	}
	//
	RealseTcp();
    //
	return true;				
}
UINT nodeRecvMain(LPVOID Ptr)
{
	CNodeSvr *pp = (CNodeSvr *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartNodeRecvSel();
	return 1;
}
bool CNodeSvr::StartNodeSendSel()
{	
	//FD_CLR(s,*set);从set集合中删除s套接字。
    //FD_ISSET(s,*set);检查s是否为set集合的成员。
    //FD_SET(s,*set);将套接字加入到set集合中。
    //FD_ZERO(*set);将set集合初始化为空集合。
	SOCKET sSocket;
	int dataSize = 0;
	char snode_id[16];
	unsigned char tmpBuf[DATA_FRAME_LEN+3];
	unsigned char sndBuf[DATA_FRAME_LEN];
	////unsigned char sendData[DATA_FRAME_LEN+1];
	////memset(sendData,0,sizeof(sendData));
	CString strTmp;
    //
	while(m_bSend)
	{
		//
		if(!m_isStart)break;
		if(NULL == m_sServer)break;

		////FD_ZERO(&m_writeSet);
		////m_writeSet=m_socketSet;

		//////同时检查套接字的可读可写性。
		//////&m_writeSet
		////int ret=select(0,NULL,&m_writeSet,NULL,NULL);//&timeout);//最后一个参数为等待时间，传入NULL，则永久等待。传入0立即返回。不要勿用。
		////if(!m_isStart)break;
		////if(ret==SOCKET_ERROR)
		////{
		////	//需相应处理
		////	strTmp.Format("select出错[%d][%d]",ret,errno);
		////	AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		////	//
		////	break;
		////}

		//////是否存在客户端的连接请求。
		////if(FD_ISSET(m_sServer,&m_writeSet))//在readset中会返回已经调用过listen的套接字。
		////{
		////	continue;
		////}
  ////      //
		if(!m_pSendQueue->EmptyQueue()) 
		{
			dataSize = m_pSendQueue->GetDataRecordSum();
			if(dataSize <= 0) 
			{
				memset(tmpBuf,0,sizeof(tmpBuf));
				dataSize = 0;
			}
			else
			{
				//
				memset(tmpBuf,0,sizeof(tmpBuf));
				dataSize = 0;
				m_pSendQueue->ReadQueue(tmpBuf, &dataSize);
				//
			}
		}
		else
		{
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
		}
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
			m_del_sock_cs.Lock();
			vecIndex = FindSockIndex(sSocket);
			//判断是否超时未收到数据，如果超时，则认为客户端已断开
			if(vecIndex >= 0)
			{
				////if((time(NULL) - m_vecSockInfo[vecIndex].last_time) > CONN_TIMEOUT)
				////{
				////	//超时未收到数据
				////	strTmp.Format("[%s]timeout关闭连接[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
				////	AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
				////	FD_CLR(sSocket,&m_socketSet);
				////	DelSockInfo(sSocket);
				////	continue;
				////}
			}
			else
			{
				m_del_sock_cs.Unlock();
				continue;
			}	
			//if(FD_ISSET(sSocket,&m_writeSet))
			{
				//调用send，发送数据。
				if(dataSize > 0)
				{
					memset(snode_id,0,sizeof(snode_id));
					memset(sndBuf,0,sizeof(sndBuf));
					memcpy(snode_id,tmpBuf,3);
					memcpy(sndBuf,tmpBuf+3,dataSize-3);
					if(0 == strcmp(snode_id,"000"))
					{
						//发给所有节点
						send(sSocket,(char*)sndBuf,dataSize-3,0);
						m_vecSockInfo[vecIndex].snd_time = time(NULL);
					}
					else
					{
						//发给指定的节点
						if(0 == strcmp(snode_id,m_vecSockInfo[vecIndex].node_id.c_str()))
						{
							send(sSocket,(char*)sndBuf,dataSize-3,0);
							m_vecSockInfo[vecIndex].snd_time = time(NULL);
						}
					}
				}
				//心跳
				time_t time_now = time(NULL);
				if(time_now - m_vecSockInfo[vecIndex].snd_time > HEART_BEAT_TIME)
				{
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
					memcpy(&m_heartBeatBuf[5],m_vecSockInfo[vecIndex].node_id.c_str(),3);
					//station_id
					memcpy(&m_heartBeatBuf[8],m_svrId.c_str(),3);
					//校验，保留，暂不校验
					m_heartBeatBuf[11] = 0x00;
					m_heartBeatBuf[12] = 0x0E;
					//
					send(sSocket,m_heartBeatBuf,sizeof(m_heartBeatBuf),0);
					m_vecSockInfo[vecIndex].snd_time = time_now;
				}
			}
			m_del_sock_cs.Unlock();
		}
		Sleep(1);
		
	}
    //
	return true;				
}
UINT nodeSendMain(LPVOID Ptr)
{
	CNodeSvr *pp = (CNodeSvr *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartNodeSendSel();
	return 1;
}
//
bool CNodeSvr::StartNodeServer(const char *netIp,u_short netPort,CDataQueue *pRecvQueue,CDataQueue *pSendQueue,CLogMessage *plogMsg,int svr_type)
{
	if((NULL == netIp) || (NULL == pRecvQueue) || NULL == pSendQueue || NULL == plogMsg)
	{
		return FALSE;
	}
	////////////
	CString strTmp;
	m_nServerPort = netPort;
	m_pRecvQueue = pRecvQueue;
	m_pSendQueue = pSendQueue;
	m_pLogMsg = plogMsg;
	m_svrType = svr_type;
	m_svrId = "999";
	memset(m_recvData,0,sizeof(m_recvData));
	m_bRecv = true;
	m_bSend = true;
	//
    SOCKADDR_IN local;
	//创建监听socket
    m_sServer = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sServer == SOCKET_ERROR)
    {
		AddLogMsg(GRADE_LOG_ERROR,"初始化失败");
        err = WSAGetLastError();
        return false;
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
		strTmp.Format("bind套接字失败[%d]",netPort);
		AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		//
		err = WSAGetLastError();
		closesocket(m_sServer);
        return true;
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
        return true;
    }
	//
	FD_ZERO(&m_socketSet);
	m_socketSet.fd_count = 0;
	FD_SET(m_sServer,&m_socketSet);
	m_isStart = true;
	strTmp.Format("服务器启动，端口[%d]",netPort);
	AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
	//
	m_bRecv = true;
	m_recvThread = AfxBeginThread(nodeRecvMain,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_recvThread->m_bAutoDelete = TRUE;
	m_recvThread->ResumeThread();
	//
	m_bSend = true;
	m_sendThread = AfxBeginThread(nodeSendMain,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_sendThread->m_bAutoDelete = TRUE;
	m_sendThread->ResumeThread();
	return true;
}
//
bool CNodeSvr::AddSockInfo(stuSockInfo sockinfo)
{
	//如果多线程要加锁
	m_vecSockInfo.push_back(sockinfo);
	return TRUE;
}
//
bool CNodeSvr::GetAllSockInfo(vector<stuSockInfo> &vec_sockinfo)
{
	std::copy(m_vecSockInfo.begin(),m_vecSockInfo.end(),std::back_inserter(vec_sockinfo));
	//vec_sockinfo = m_vecSockInfo;
	return TRUE;
}
//返回索引
int CNodeSvr::FindSockIndex(SOCKET sSocket)
{
	size_t vecIndex = -1;
	size_t iSize = m_vecSockInfo.size();
	for(size_t i = 0; i < iSize; i++)
	{
		if(sSocket == m_vecSockInfo[i].sock)
		{
            vecIndex = i;
		}
	}
	return (int)vecIndex;
}
//根据结点编号，返回SockInfo
bool CNodeSvr::FindSockInfoByNodeId(string node_id,stuSockInfo &sockInfo)
{
	bool bFind = false;
	size_t iSize = m_vecSockInfo.size();
	for(size_t i = 0; i < iSize; i++)
	{
		if(node_id == m_vecSockInfo[i].node_id)
		{
			sockInfo.sock = m_vecSockInfo[i].sock;
			bFind = true;
		}
	}
	return bFind;
}
//
bool CNodeSvr::DelSockInfo(SOCKET sSocket)
{
	m_del_sock_cs.Lock();
	bool bFind = false;
	vector<stuSockInfo>::iterator iter;
	for(iter = m_vecSockInfo.begin(); iter != m_vecSockInfo.end(); iter++)
	{
		if(sSocket == iter->sock)
		{
			bFind = true;
			//SetEvent(*m_phConnFreshEvent);
			map<string,stuNodeInfo>::iterator iter_n;
			iter_n = m_mapNodeInfo.find(iter->node_id);
			if(iter_n != m_mapNodeInfo.end())
			{
				iter_n->second.conn_status = false;
				iter_n->second.sock = 0;
			}
            closesocket(sSocket);
			m_vecSockInfo.erase(iter);
			break;
		}
	}
	m_del_sock_cs.Unlock();
	return bFind;
}
//
bool CNodeSvr::AddLogMsg(int grade, const char *sbody)
{
	stuLogContent msg;
    msg.item           = 0;
	msg.type           = TYPE_LOG_TCP;
	msg.grade          = grade;
	if(SVR_STATION == m_svrType)
	{
		msg.body = "站点";
	}
	else if(SVR_CLIENT == m_svrType)
	{
		msg.body = "终端";
	}
	else if(SVR_QUERY == m_svrType)
	{
		msg.body = "查询";
	}
	msg.body           += sbody;
	msg.title          = "TCP";
	msg.datetime       = time(NULL);
	m_pLogMsg->AddLogMessage(msg);
	return true;
}
//
bool CNodeSvr::StopTcpServer()
{
	//RealseTcp();
	m_bRecv = false;
	m_bSend = false;
	m_isStart = false;
	shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	return true;
}
//
bool CNodeSvr::GetTcpStatus()
{
	return m_isStart;
}
//
int CNodeSvr::GetSocketNum()
{
	if(m_socketSet.fd_count <= 0)return 0;
	return (m_socketSet.fd_count - 1);
}
//
bool CNodeSvr::RealseTcp()
{    
    CString strTmp;
	strTmp.Format("服务器已关闭，端口[%d]",m_nServerPort);
	AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
	//
	m_recvThread = false;
	m_sendThread = false;
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
	m_isStart = false;
	//SetEvent(*m_phConnFreshEvent);
	//
	if( m_recvThread != NULL )
	{
		if (WaitForSingleObject(m_recvThread->m_hThread, 
			THREAD_SHUTDOWN_TIME) == WAIT_TIMEOUT)
		{
		    TerminateThread(m_recvThread->m_hThread,0);
		}
		
		delete m_recvThread;
		m_recvThread = NULL;
	}
	//
	if( m_sendThread != NULL )
	{
		if (WaitForSingleObject(m_sendThread->m_hThread, 
			THREAD_SHUTDOWN_TIME) == WAIT_TIMEOUT)
		{
		    TerminateThread(m_sendThread->m_hThread,0);
		}
		
		delete m_sendThread;
		m_sendThread = NULL;
	}
	//关闭服务器socket
	if(NULL == m_sServer) 
	{
		return true;
	}
	shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	return true;
}

bool CNodeSvr::GetNodeConnStatus(string node_id)
{
	map<string,stuNodeInfo>::iterator iter;
	iter = m_mapNodeInfo.find(node_id);
	if(iter != m_mapNodeInfo.end())
	{
		return iter->second.conn_status;		
	}
	return false;
}

SOCKET CNodeSvr::GetNodeSocket(string node_id)
{

	map<string,stuNodeInfo>::iterator iter;
	iter = m_mapNodeInfo.find(node_id);
	if(iter != m_mapNodeInfo.end())
	{
		return iter->second.sock;		
	}
	return -1;
}