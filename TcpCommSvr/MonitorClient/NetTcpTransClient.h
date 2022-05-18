#pragma once
//
#include "MonitorClient.h"
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")
//
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
#define CLIENT_SOCKET_TIMEOUT 20000
#define HEART_BEAT_TIME 10
//
class CNetTcpTransClient
{
public:
	CNetTcpTransClient(void);
public:
	~CNetTcpTransClient(void);
public:
	int SetSendData(stuCardInOutStatus &stuCIOS);
	int RevceiveData(char *recvBuf,SOCKET sscoket);
	BOOL CheckFrame(const unsigned char *dataframe,
				int framelen) ;
	//
	BOOL StartTcpClientConn();
	bool StartClientRecvSel();
	bool StartClientSendSel();
	BOOL StartTcpClient(const char *svrId,const char *clientId,const char *netIp,u_short netPort,CLogMessage *plogMsg,CDataQueue *pRecvDataQueue);
	//触发退出网络接收线程
    void SetThreadStatus(BOOL bol) ;
	BOOL AddLogMsg(int grade, const char *sbody);
    BOOL StopTcpClient();
private:
	HANDLE *m_phConnFreshEvent;
	int		err;
	//
	CWinThread *m_netThread;
	CWinThread *m_recvThread;
	CWinThread *m_sendThread;
	SOCKET  m_sSocket;
	bool m_bConn;
	int		m_nRemotePort;
	string  m_sRemoteIp;
	string  m_svrId;
	string  m_clientId;
    //数据
	unsigned char m_recvData[DATA_FRAME_LEN+1];
	stuRecvBufError m_stuRecvBufErr;
	CDataQueue *m_pSendDataQueue;
	CDataQueue *m_pRecvDataQueue;
	CLogMessage *m_pLogMsg;
	BOOL m_bWSAStartup;
	BOOL m_bactiveThread;
	BOOL m_isStart;
	bool m_bRecv;
	bool m_bSend;
	time_t m_recvTime;
	time_t m_sendTime;
	char m_heartBeatBuf[13];// 
};
