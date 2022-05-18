// NetTcpTrans.h: interface for the CNetTcpTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETTCPTRANS_H__800B31CE_A49D_4C23_891D_25D05DAA5775__INCLUDED_)
#define AFX_NETTCPTRANS_H__800B31CE_A49D_4C23_891D_25D05DAA5775__INCLUDED_
#include "TcpCommSvr.h"
////接收超时时间5秒
#define SOCKET_TIMEOUT 5000
//1分钟收不到数据，则认为连接断开
#define CONN_TIMEOUT 60   
//
#define FD_SETSIZE 256
#define FD_NUM 1
#define FD_ALL_SIZE FD_NUM*FD_SETSIZE
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winsock2.h"
#pragma comment (lib, "Ws2_32.lib")
//
struct stuThreadInfo
{
	LPVOID ths;
	SOCKET sock;
};
//
struct stuSockInfo
{
	CWinThread *sockThread;
    SOCKET sock;
	string ip;
	int    port;
	int    reader_no;
	time_t last_time;   //最后一次收到数据的时间
	BOOL   isUpdateReader;
};
//
class CNetTcpTrans  
{
public:
	CNetTcpTrans();
	virtual ~CNetTcpTrans();
	int RevceiveData(char *recvBuf,SOCKET sscoket);
	BOOL CheckFrame(const unsigned char *dataframe,
				int framelen) ;
	BOOL CheckFrame2A(const unsigned char *dataframe,
				int framelen) ;
	//多线程模型
	BOOL StartTcpComThread();
	//select模型
	BOOL StartTcpComSel();
	//
	BOOL StartTcpComRecv(SOCKET sSocket);
	BOOL StartTcpServer(const char *netIp,u_short netPort,CDataQueue *pdataQueue,CLogMessage *plogMsg,HANDLE *phConnFreshEvent,int conn_type);
	//触发退出网络接收线程
    void SetThreadStatus(BOOL bol) ;
	//
	BOOL AddSockInfo(stuSockInfo sockinfo);
	BOOL GetAllSockInfo(vector<stuSockInfo> &vector_sockinfo);
	BOOL FindSockInfo(SOCKET sSocket);
	BOOL FindSockInfoByReaderNo(int readerNo,stuSockInfo &sockInfo);
	BOOL DelSockInfo(SOCKET sSocket);
	BOOL SetSockReaderNo(SOCKET sSocket,int readNo);
	BOOL AddLogMsg(int grade, const char *sbody);
	int  GetSocketNum();
    BOOL GetTcpStatus();
	BOOL StopTcpServer();
	BOOL RealseTcp();
private:
	HANDLE *m_phConnFreshEvent;
	int		err;
	//
	CWinThread *m_netThread;
	SOCKET  m_sServer;
	int		m_nServerPort;
    //数据
	unsigned char m_recvData[DATA_FRAME_LEN+1];
	CDataQueue *m_pDataQueue;
	CLogMessage *m_pLogMsg;
	stuRecvBufError m_stuRecvBufErr;
	//链接列表
	vector<stuSockInfo> m_vecSockInfo;
	//
	fd_set m_socketSet;
	fd_set m_writeSet;
	fd_set m_readSet;
	//
	BOOL m_bWSAStartup;
	BOOL m_bactiveThread;
	BOOL m_isStart;
	//
	int m_connType;
};

#endif // !defined(AFX_NETTCPTRANS_H__800B31CE_A49D_4C23_891D_25D05DAA5775__INCLUDED_)
