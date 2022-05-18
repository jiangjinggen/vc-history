#include "StdAfx.h"
#include "NodeSvr.h"

CNodeSvr::CNodeSvr(void)
{
}

CNodeSvr::~CNodeSvr(void)
{
}
//����һ�����������ݰ�
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
bool CNodeSvr::CheckFrame(const unsigned char *dataframe, //��У���֡
				int framelen)                    //֡����
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
    //FD_CLR(s,*set);��set������ɾ��s�׽��֡�
    //FD_ISSET(s,*set);���s�Ƿ�Ϊset���ϵĳ�Ա��
    //FD_SET(s,*set);���׽��ּ��뵽set�����С�
    //FD_ZERO(*set);��set���ϳ�ʼ��Ϊ�ռ��ϡ�
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
		//accept�ͻ�
		if(!m_isStart)break;
		if(NULL == m_sServer)break;

        FD_ZERO(&m_readSet);
		m_readSet=m_socketSet;

		//ͬʱ����׽��ֵĿɶ���д�ԡ�
		//&m_readSet
		int ret=select(0,&m_readSet,NULL,NULL,NULL);//&timeout);//���һ������Ϊ�ȴ�ʱ�䣬����NULL�������õȴ�������0�������ء���Ҫ���á�
		if(!m_isStart)break;
		if(ret==SOCKET_ERROR)
		{
			//����Ӧ����
			strTmp.Format("select����[%d][%d]",ret,errno);
			AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
			//
			break;
		}

		//�Ƿ���ڿͻ��˵���������
		if(FD_ISSET(m_sServer,&m_readSet))//��readset�л᷵���Ѿ����ù�listen���׽��֡�
		{
			if(m_socketSet.fd_count >= FD_SETSIZE)continue;
			acceptSocket=accept(m_sServer,(struct sockaddr *)&addr, &iAddrSize);
			if(acceptSocket==INVALID_SOCKET)
			{
				//����Ӧ����
				strTmp.Format("accept����[%d][%d]sel",acceptSocket,errno);
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
				strTmp.Format("[%s]�ͻ�������[%d]",clientIp,uClientPort);
				AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
				//
				int time_recv = SOCKET_TIMEOUT; //��ʱ��
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
			//�ж��Ƿ�ʱδ�յ����ݣ������ʱ������Ϊ�ͻ����ѶϿ�
			if(vecIndex >= 0)
			{
				if((time(NULL) - m_vecSockInfo[vecIndex].recv_time) > CONN_TIMEOUT)
				{
					//��ʱδ�յ�����
					strTmp.Format("[%s]timeout�ر�����[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
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
				//����recv���������ݡ�
				memset(recvData,0,sizeof(recvData));          
				int framelen = RevceiveData((char *)recvData,sSocket);//����һ��������֡
				if(-3 == framelen)
				{
					//�Է��ر�����
					strTmp.Format("[%s]�ͻ��ر�����[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
					AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					FD_CLR(sSocket,&m_socketSet);
					DelSockInfo(sSocket);
					continue;
				}

				if(framelen < BASE_FRAME_LEN)
				{
					//δ֪����
					strTmp.Format("[%s]��ʱ����������[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
					AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
					FD_CLR(sSocket,&m_socketSet);
					DelSockInfo(sSocket);
					continue;
				}
				//��������յ����ݵ�ʱ��
				m_vecSockInfo[vecIndex].recv_time = time(NULL);
				//
				if(0x0B == recvData[0])
				{
					BOOL bol = CheckFrame(recvData,framelen);
					if(FALSE == bol)
					{
						strTmp.Format("[0B]У��֡ʧ��[%d]���[%d]",framelen,m_vecSockInfo[vecIndex].node_id);
						AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
						continue;
					}
					//���½����
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
								//�����ӽڵ�
								strTmp.Format("[%s]�ڵ�������[%s]",node_id,m_vecSockInfo[vecIndex].ip.c_str());
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
							//δ����ڵ�
							strTmp.Format("[%s]�ڵ�δ����[%s]",node_id,m_vecSockInfo[vecIndex].ip.c_str());
							AddLogMsg(GRADE_LOG_WARN,strTmp.GetBuffer());
							FD_CLR(sSocket,&m_socketSet);
							DelSockInfo(sSocket);
							continue;
						}
					}
					//
				}
				//������������֡������������
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
	//FD_CLR(s,*set);��set������ɾ��s�׽��֡�
    //FD_ISSET(s,*set);���s�Ƿ�Ϊset���ϵĳ�Ա��
    //FD_SET(s,*set);���׽��ּ��뵽set�����С�
    //FD_ZERO(*set);��set���ϳ�ʼ��Ϊ�ռ��ϡ�
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

		//////ͬʱ����׽��ֵĿɶ���д�ԡ�
		//////&m_writeSet
		////int ret=select(0,NULL,&m_writeSet,NULL,NULL);//&timeout);//���һ������Ϊ�ȴ�ʱ�䣬����NULL�������õȴ�������0�������ء���Ҫ���á�
		////if(!m_isStart)break;
		////if(ret==SOCKET_ERROR)
		////{
		////	//����Ӧ����
		////	strTmp.Format("select����[%d][%d]",ret,errno);
		////	AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		////	//
		////	break;
		////}

		//////�Ƿ���ڿͻ��˵���������
		////if(FD_ISSET(m_sServer,&m_writeSet))//��readset�л᷵���Ѿ����ù�listen���׽��֡�
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
			//�ж��Ƿ�ʱδ�յ����ݣ������ʱ������Ϊ�ͻ����ѶϿ�
			if(vecIndex >= 0)
			{
				////if((time(NULL) - m_vecSockInfo[vecIndex].last_time) > CONN_TIMEOUT)
				////{
				////	//��ʱδ�յ�����
				////	strTmp.Format("[%s]timeout�ر�����[%d]",m_vecSockInfo[vecIndex].ip.c_str(),m_vecSockInfo[vecIndex].port);
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
				//����send���������ݡ�
				if(dataSize > 0)
				{
					memset(snode_id,0,sizeof(snode_id));
					memset(sndBuf,0,sizeof(sndBuf));
					memcpy(snode_id,tmpBuf,3);
					memcpy(sndBuf,tmpBuf+3,dataSize-3);
					if(0 == strcmp(snode_id,"000"))
					{
						//�������нڵ�
						send(sSocket,(char*)sndBuf,dataSize-3,0);
						m_vecSockInfo[vecIndex].snd_time = time(NULL);
					}
					else
					{
						//����ָ���Ľڵ�
						if(0 == strcmp(snode_id,m_vecSockInfo[vecIndex].node_id.c_str()))
						{
							send(sSocket,(char*)sndBuf,dataSize-3,0);
							m_vecSockInfo[vecIndex].snd_time = time(NULL);
						}
					}
				}
				//����
				time_t time_now = time(NULL);
				if(time_now - m_vecSockInfo[vecIndex].snd_time > HEART_BEAT_TIME)
				{
					// m_heartBeatBuf[13];//
					m_heartBeatBuf[0] = 0x0B;
					//����
					m_heartBeatBuf[1] = 0x0A;
					m_heartBeatBuf[2] = 0x00;
					//����
					m_heartBeatBuf[3] = 0x01;
					//����
					m_heartBeatBuf[4] = 0x01;
					//svr_id
					memcpy(&m_heartBeatBuf[5],m_vecSockInfo[vecIndex].node_id.c_str(),3);
					//station_id
					memcpy(&m_heartBeatBuf[8],m_svrId.c_str(),3);
					//У�飬�������ݲ�У��
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
	//��������socket
    m_sServer = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sServer == SOCKET_ERROR)
    {
		AddLogMsg(GRADE_LOG_ERROR,"��ʼ��ʧ��");
        err = WSAGetLastError();
        return false;
    }
	
	int value;
	int optlen  = sizeof(value);
	value = 1;
	//�����	
	memset(&local,0,sizeof(SOCKADDR_IN));
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	//local.sin_addr.s_addr=inet_addr("192.100.88.220");
    local.sin_family = AF_INET;
    local.sin_port = htons(netPort);
	local.sin_addr.S_un.S_addr = INADDR_ANY; //����

	if(bind(m_sServer, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
		//
		strTmp.Format("bind�׽���ʧ��[%d]",netPort);
		AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		//
		err = WSAGetLastError();
		closesocket(m_sServer);
        return true;
    }
	//��ʼ����
    if(listen(m_sServer, 100) != 0)
	{
		//
		//CString strTmp;
		strTmp.Format("listen�׽���ʧ��[%d]",netPort);
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
	strTmp.Format("�������������˿�[%d]",netPort);
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
	//������߳�Ҫ����
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
//��������
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
//���ݽ���ţ�����SockInfo
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
		msg.body = "վ��";
	}
	else if(SVR_CLIENT == m_svrType)
	{
		msg.body = "�ն�";
	}
	else if(SVR_QUERY == m_svrType)
	{
		msg.body = "��ѯ";
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
	strTmp.Format("�������ѹرգ��˿�[%d]",m_nServerPort);
	AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
	//
	m_recvThread = false;
	m_sendThread = false;
	//�ر����пͻ�����
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
	//�رշ�����socket
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