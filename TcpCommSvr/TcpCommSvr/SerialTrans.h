// SerialTrans.h: interface for the CSerialTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALTRANS_H__12AE47E5_A33A_40B1_AB35_660785A64AEB__INCLUDED_)
#define AFX_SERIALTRANS_H__12AE47E5_A33A_40B1_AB35_660785A64AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TcpCommSvr.h"
#include "serial.h"
#pragma  comment (lib,"Serial")

class CSerialTrans  
{
public:
	CSerialTrans();
	virtual ~CSerialTrans();
	int RevceiveData(unsigned char *recvBuf);
	BOOL CheckFrame(const unsigned char *dataframe,
				int framelen) ;                   
	BOOL StartSerialCom();
	BOOL initSerial(const char *serialPara,CDataQueue *pdataQueue,CLogMessage *plogMsg,int conn_type);
	//触发退出串口接收事件
    void SetThreadStatus(bool bol) ;
	//
	BOOL RealseSerial();
	BOOL AddLogMsg(int grade, const char *sbody);
private:
	//
	CWinThread *m_serialThread;
	HANDLE m_hIOEvent;
	HANDLE m_hShutdownEvent;
	CSerial *m_serial;
	BOOL m_serialIsOpen;
	unsigned char m_recvData[DATA_FRAME_LEN+1];
	CDataQueue* m_pDataQueue;
	CLogMessage *m_pLogMsg;
	bool m_bactiveThread;
	stuRecvBufError m_stuRecvBufErr;
	int m_connType;
};

#endif // !defined(AFX_SERIALTRANS_H__12AE47E5_A33A_40B1_AB35_660785A64AEB__INCLUDED_)
