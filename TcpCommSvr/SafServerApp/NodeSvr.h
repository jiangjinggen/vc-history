#pragma once
#include "SafServerApp.h"
////接收超时时间5秒
#define SOCKET_TIMEOUT 5000
//1分钟收不到数据，则认为连接断开
#define CONN_TIMEOUT 60   
//
#define FD_SETSIZE 256
#define FD_NUM 1
#define FD_ALL_SIZE FD_NUM*FD_SETSIZE
#define HEART_BEAT_TIME 10
//
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")
//
struct stuSockInfo
{
    SOCKET sock;
	string ip;
	int    port;
	string node_id;
	time_t recv_time;   //最后一次收到数据的时间
	time_t snd_time;
	bool   is_update_node;
};
//
struct stuNodeInfo
{
	int node_type;
	string node_id;
	string node_code;
	string node_name;
	string node_ip;
	SOCKET sock;
	bool conn_status;
};
//
enum NODE_TYPE
{
	NODE_SERVER = 0,
	NODE_STATION = 1,
	NODE_CLIENT = 2,
	NODE_QUERY = 3,
};
//
enum SVR_TYPE
{
	SVR_STATION = 1,
	SVR_CLIENT = 2,
	SVR_QUERY = 3,
};
//
class CNodeSvr
{
public:
	CNodeSvr(void);
public:
	~CNodeSvr(void);
public:
	CCriticalSection m_del_sock_cs;
	map<string,stuNodeInfo> m_mapNodeInfo;
	int     m_nodeType;
	int     m_svrType;
	int		err;
	string  m_svrId;
	CWinThread *m_recvThread;
	CWinThread *m_sendThread;
	CDataQueue *m_pRecvQueue;
	CDataQueue *m_pSendQueue;
	CLogMessage *m_pLogMsg;
	SOCKET  m_sServer;
	stuRecvBufError m_stuRecvBufErr;
	unsigned char m_recvData[DATA_FRAME_LEN+1];
	int m_nServerPort;
	bool m_bRecv;
	bool m_bSend;
	bool m_isStart;
	//
	fd_set m_socketSet;
	fd_set m_writeSet;
	fd_set m_readSet;
	//链接列表
	vector<stuSockInfo> m_vecSockInfo;
	//
	char m_heartBeatBuf[13];// 
public:
	//接收一个完整的数据包
    int  RevceiveData(char *recvBuf,SOCKET sscoket);
	//校验
	bool CheckFrame(const unsigned char *dataframe, //待校验的帧
					int framelen);                    //帧长度
	bool StartNodeRecvSel();
	bool StartNodeSendSel();
	bool StartNodeServer(const char *netIp,u_short netPort,CDataQueue *pRecvQueue,CDataQueue *pSendQueue,CLogMessage *plogMsg,int svr_type);
	bool AddSockInfo(stuSockInfo sockinfo);
	bool GetAllSockInfo(vector<stuSockInfo> &vec_sockinfo);
	int  FindSockIndex(SOCKET sSocket);
	bool FindSockInfoByNodeId(string node_id,stuSockInfo &sockInfo);
	bool DelSockInfo(SOCKET sSocket);
	bool AddLogMsg(int grade, const char *sbody);
	bool StopTcpServer();
	bool GetTcpStatus();
	int  GetSocketNum();
	bool RealseTcp();
	//
	void SetNodeInfo(map<string,stuNodeInfo> &nodeInfo){m_mapNodeInfo = nodeInfo;}
	bool SetNodeType(int type){m_nodeType = type;}
	//
	bool GetNodeConnStatus(string node_id);
	SOCKET GetNodeSocket(string node_id);
};
