// SerialTrans.cpp: implementation of the CSerialTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialTrans.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialTrans::CSerialTrans()
{
	m_serialThread = NULL;
	m_hIOEvent = NULL;
	m_hShutdownEvent = NULL;
    m_serial = NULL;
	m_serialIsOpen = false;
	m_bactiveThread = true;
	memset(&m_stuRecvBufErr,0,sizeof(m_stuRecvBufErr));
	m_connType = 0;
}

CSerialTrans::~CSerialTrans()
{
	RealseSerial();
}
//触发退出串口接收事件
void CSerialTrans::SetThreadStatus(bool bol) 
{
	m_bactiveThread = bol;
	SetEvent(m_hIOEvent);
}
//接收一个完整的数据包
int CSerialTrans::RevceiveData(unsigned char *recvBuf) 
{
	if(NULL == recvBuf)
	{
		return -1;
	}
	//
    int bol = 0;
    DWORD totalsize = 0;
    unsigned char tmpBuf[DATA_FRAME_LEN];
    int tmplen = 0;
	int datalen = 0;
	int framelen = 0;
	int icount = 0;
	unsigned char *pp = recvBuf;
	CString strTmp;
	int iNum = 0;
	//开始接收数据
    //接帧头0x0B
	iNum = 0;
	////////memset(&m_stuRecvBufErr,0,sizeof(m_stuRecvBufErr));
	////////while(true)
	////////{
	////////	memset(tmpBuf,0,sizeof(tmpBuf));
	////////	bol = m_serial->Receive(tmpBuf,1,&totalsize);//接收数据
	////////	if(totalsize < 0)
	////////	{
	////////		return -1;
	////////	}
	////////	else
	////////	{
	////////		//
	////////	    if(0 == totalsize)
	////////		{
	////////			Sleep(100);
	////////			continue;
	////////		}
	////////		//
	////////		if(0x0b != tmpBuf[0])
	////////		{
	////////			m_stuRecvBufErr.recvBuf[iNum] = tmpBuf[0];
	////////			iNum++;
 ////////               m_stuRecvBufErr.len = iNum;
	////////			continue;
	////////		}
	////////		//
	////////		memcpy(pp,tmpBuf,totalsize);
	////////		pp += totalsize;
	////////		tmplen += totalsize;
	////////		break;
	////////	}
	////////}
	////////if(0 != iNum)
	////////{
	////////	strTmp.Format("err_head[%d]",iNum);
	////////	//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
	////////	//CTestLog::LogDebugInfo(".\\data\\","testdebug_err.txt",m_stuRecvBufErr.recvBuf,m_stuRecvBufErr.len);
	////////}
	//////////接收前四个字节
	////////icount = 0;
	////////int tmp_head_len = 0;
	////////if(IS_NET == m_connType)
	////////{
	////////	tmp_head_len = HEAD_LEN + 1;
	////////}
	////////else
	////////{
	////////	tmp_head_len = HEAD_LEN;
	////////}
	////////while(tmplen < tmp_head_len)
	////////{
	////////	memset(tmpBuf,0,sizeof(tmpBuf));
	////////	bol = m_serial->Receive(tmpBuf,tmp_head_len - tmplen,&totalsize);//接收数据
	////////	if(totalsize < 0) return -3;
	////////	if(0 == totalsize)
	////////	{
	////////		Sleep(100);
	////////		continue;
	////////	}
	////////	memcpy(pp,tmpBuf,totalsize);
	////////	pp += totalsize;
	////////	tmplen += totalsize;
	////////	//
	////////	//CTestLog::LogDebugInfo(".\\data\\","testdebug1.txt",recvBuf,tmplen);
	////////}
 ////////   //数据长度
	////////datalen = (unsigned char)recvBuf[tmp_head_len-1];
 ////////   //接收数据体
	////////framelen = tmp_head_len + datalen;
	//////////
	////////icount = 0;
	////////while(tmplen < framelen)
	////////{
	////////	memset(tmpBuf,0,sizeof(tmpBuf));
	////////	bol = m_serial->Receive(tmpBuf,framelen - tmplen,&totalsize);//接收数据
	////////	if(totalsize < 0) return -6;
	////////	if(0 == totalsize)
	////////	{
	////////		Sleep(100);
	////////		continue;
	////////	}
	////////	memcpy(pp,tmpBuf,totalsize);
	////////	pp += totalsize;
	////////	tmplen += totalsize;
	////////	//
	////////	//CTestLog::LogDebugInfo(".\\data\\","testdebug2.txt",recvBuf,tmplen);
	////////}
	///////////////////调试
	/*TRACE0("收到的数据为:");
	for(i=0;i<packagelen;i++)
	{
	    TRACE1("%x",receivest[i]);
	}
	TRACE0("\n");*/
	//CTestLog::LogDebugInfo(".\\data\\","testdebug.txt",recvBuf,framelen);
	////////////////////////
    return framelen;	
}
//校验
BOOL CSerialTrans::CheckFrame(const unsigned char *dataframe, //待校验的帧
				int framelen)                    //帧长度
{
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

BOOL CSerialTrans::StartSerialCom()
{	
	//监听串口，接收数据
	CString strTmp;
	////////while(m_bactiveThread)
	////////{
	////////	//Sleep(100);//
	////////	//if(WAIT_OBJECT_0==WaitForSingleObject(m_hIOEvent,INFINITE))//等待m_hIOEvent事件的发生
	////////	{   
 ////////           memset(m_recvData,0,sizeof(m_recvData));
	////////		int framelen = RevceiveData(m_recvData);//接收一个完整的帧
	////////		if(framelen <= 0)
	////////		{
	////////			Sleep(100);
	////////			strTmp.Format("RevceiveData failed[%d]",framelen);
	////////			//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
	////////			AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
	////////			continue;
	////////		}
	////////		BOOL bol = CheckFrame(m_recvData,framelen);
	////////		if(FALSE == bol)
	////////		{
	////////			strTmp.Format("CheckFrame failed[%d]",framelen);
	////////			//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_COMM,0);
	////////			//CTestLog::LogDebugInfo(".\\data\\","testdebug_chk.txt",m_recvData,framelen);
	////////			AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
	////////			continue;
	////////		}
	////////		//将正常的数据帧加入待处理队列
	////////		m_pDataQueue->InsertQueue(m_recvData,framelen);	
	////////	}       
	////////}
	////////if(m_serialIsOpen)
	////////{
	////////    m_serial->Close();
	////////	m_serialIsOpen = FALSE; 
	////////}
	return TRUE;				
}

UINT SerialTransMain(LPVOID Ptr)
{
	CSerialTrans *pp = (CSerialTrans *)Ptr;//
	if(pp==NULL)
	{
		return FALSE;
	}
	pp->StartSerialCom();
	return 1;
}

BOOL CSerialTrans::initSerial(const char *serialPara,CDataQueue *pdataQueue,CLogMessage *plogMsg,int conn_type)
{
	CString strTmp;
	if((NULL == serialPara) || (NULL == pdataQueue) || (NULL == plogMsg))
	{
		strTmp = "参数错误";
		//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_MAIN,0);
		AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
		return FALSE;
	}
	////////////
	m_connType = conn_type;
	m_pDataQueue = pdataQueue;
	m_pLogMsg = plogMsg;
	memset(m_recvData,0,sizeof(m_recvData));
	m_hIOEvent = CreateEvent(NULL,FALSE,FALSE,NULL);//创建事件
	m_hShutdownEvent = CreateEvent(NULL,FALSE,FALSE,NULL);//
    ////////////m_serial = new CSerial("");//构造对象
	////////////////////////////////
	////////////int bol = m_serial->Open(_T(serialPara),
	////////////	                       _T("baud=9600 parity=N data=8 stop=1"),m_hIOEvent);
	////////////if(bol < 0)
	////////////{
	////////////    RealseSerial();
	////////////	strTmp.Format("串口打开失败[%s]",serialPara);
	////////////	//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_MAIN,0);
	////////////	AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
	////////////}
	////////////else
	////////////{
	////////////	m_serial->TransmitClear();
	////////////	m_serial->ReceiveClear();
	////////////	//m_serial->SetBufferSize(SERIAL_BUFFER_SIZE);
	////////////    m_serialIsOpen = TRUE;
	////////////	HANDLE tmpHandle = m_serial->GetCommHandle();
	////////////	if(!SetupComm(tmpHandle,SERIAL_BUFFER_SIZE,2048))
	////////////	{
	////////////		RealseSerial();
	////////////		strTmp.Format("设置缓冲区失败[%s]",serialPara);
	////////////	    //CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_MAIN,0);
	////////////		AddLogMsg(GRADE_LOG_ERROR,strTmp.GetBuffer());
	////////////		return FALSE;
	////////////	}

	////////////	//
	////////////	CString strTmp;
	////////////	strTmp.Format("串口打开成功[%s]",serialPara);
	////////////	//CTestLog::LogRunInfo(strTmp.GetBuffer(0),LOG_MAIN,0);
	////////////	AddLogMsg(GRADE_LOG_COMM,strTmp.GetBuffer());
	////////////	//
	////////////	m_serialThread = AfxBeginThread(SerialTransMain,(LPVOID)this,
	////////////		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	////////////	m_serialThread->m_bAutoDelete = FALSE;
	////////////	m_serialThread->ResumeThread();
	////////////}

	return m_serialIsOpen;
}
//
BOOL CSerialTrans::RealseSerial()
{
 //////////   if(m_serialIsOpen)
	//////////{
	//////////    m_serial->Close();
	//////////	m_serialIsOpen = FALSE;
	//////////}
 //////////   
	//////////SetEvent(m_hShutdownEvent);

	//////////if( m_serialThread != NULL )
	//////////{
	//////////	if (WaitForSingleObject(m_serialThread->m_hThread, 
	//////////		THREAD_SHUTDOWN_TIME) == WAIT_TIMEOUT)
	//////////	{
	//////////	    TerminateThread(m_serialThread->m_hThread,0);
	//////////	}
	//////////	
	//////////	delete m_serialThread;
	//////////	m_serialThread = NULL;
	//////////}


	//////////if(m_hIOEvent != NULL)
	//////////{
	//////////    CloseHandle(m_hIOEvent);
	//////////	m_hIOEvent = NULL;
	//////////}

	//////////if(m_serial != NULL)
	//////////{
	//////////    delete m_serial;
	//////////	m_serial = NULL;
	//////////}
	return TRUE;
}
//
BOOL CSerialTrans::AddLogMsg(int grade, const char *sbody)
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