#pragma once
//
#include "TcpCommSvr.h"
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")
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
	int SetSendCardData(const stuCardInOutStatus *stuCIOS);
	int SetSendAttData(const stuAttendanceData *stuAtt);
	int RevceiveData(char *recvBuf,SOCKET sscoket);
	BOOL CheckFrame(const unsigned char *dataframe,
				int framelen) ;
	//
	BOOL StartTcpClientConn();
	bool StartClientRecvSel();
	bool StartClientSendSel();
	BOOL StartTcpClient(const char *svrId,const char *stationId,const char *netIp,u_short netPort,CLogMessage *plogMsg);
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
	string  m_stationId;
    //数据
	unsigned char m_recvData[DATA_FRAME_LEN+1];
	stuRecvBufError m_stuRecvBufErr;
	CDataQueue *m_pSendDataQueue;
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
