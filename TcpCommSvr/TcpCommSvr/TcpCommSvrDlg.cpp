// TcpCommSvrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpCommSvr.h"
#include "TcpCommSvrDlg.h"
#include "NetTcpTrans.h"
#include "direct.h"
//#include "LoginDlg.h"
#include "DlgNewDataQry.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern int g_userGrade;
CTcpCommSvrDlg *g_pMainDlg;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTcpCommSvrDlg 对话框
CTcpCommSvrDlg::CTcpCommSvrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpCommSvrDlg::IDD, pParent)
	, m_isRecordData(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_isRecordData = FALSE;
	m_hConnFreshEvent = NULL;
    m_pDataQueue = NULL;
	m_pLogMsg    = NULL;
}

void CTcpCommSvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_isRecordData);
	DDX_Control(pDX, IDC_LIST1, m_connInfo);
	DDX_Control(pDX, IDC_LIST2, m_commInfo);
}

BEGIN_MESSAGE_MAP(CTcpCommSvrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CTcpCommSvrDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTcpCommSvrDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PARA, &CTcpCommSvrDlg::OnBnClickedBtnPara)
	ON_BN_CLICKED(IDC_BTN_READER_QRY, &CTcpCommSvrDlg::OnBnClickedBtnReaderQry)
	ON_BN_CLICKED(IDC_BTN_CARD_QRY, &CTcpCommSvrDlg::OnBnClickedBtnCardQry)
	ON_BN_CLICKED(IDC_BTN_TMD_START, &CTcpCommSvrDlg::OnBnClickedBtnTmdStart)
	ON_BN_CLICKED(IDC_BTN_PARA_MNG, &CTcpCommSvrDlg::OnBnClickedBtnParaMng)
	ON_BN_CLICKED(IDC_BTN_INFO_QRY, &CTcpCommSvrDlg::OnBnClickedBtnInfoQry)
	ON_BN_CLICKED(IDC_BTN_START_CONNECT, &CTcpCommSvrDlg::OnBnClickedBtnStartConnect)
	ON_BN_CLICKED(IDC_BTN_STOP_CONNECT, &CTcpCommSvrDlg::OnBnClickedBtnStopConnect)
	ON_BN_CLICKED(IDC_BTN_START_COM, &CTcpCommSvrDlg::OnBnClickedBtnStartCom)
	ON_BN_CLICKED(IDC_BTN_STOP_COM, &CTcpCommSvrDlg::OnBnClickedBtnStopCom)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CTcpCommSvrDlg::StartLogPro()
{
	char stmp[256];
	int iCount = 0;
	int nIndex = 0;
    while(m_bolLog)
	{
	    stuLogContent msg;
	    if(!m_pLogMsg->GetLogMessage(&msg))
		{
			Sleep(1000);
			continue;
		}
		if(iCount > 300)
		{
			m_commInfo.DeleteAllItems();//清空
            iCount = 0;
		}
		iCount++;
		//序号
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"%03d",iCount);
		m_commInfo.InsertItem(0, _T(stmp));
		//等级
		memset(stmp,0,sizeof(stmp));
		if(GRADE_LOG_COMM == msg.grade)
		{
			strcpy(stmp,"普通");
		}
		else if(GRADE_LOG_WARN == msg.grade)
		{
			strcpy(stmp,"警告");
		}
		else if(GRADE_LOG_ERROR == msg.grade)
		{
			strcpy(stmp,"错误");
		}
		else
		{
			strcpy(stmp,"未知");
		}
		m_commInfo.SetItemText(0, 1, _T(stmp));
		//内容
		m_commInfo.SetItemText(0, 2, _T(msg.body.c_str()));
		//时间
		struct tm *now = NULL;
		now = localtime(&msg.datetime);
		sprintf(stmp,"%d-%02d-%02d %d:%02d:%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
        m_commInfo.SetItemText(0, 3, _T(stmp)); 		
	}
    //
	return TRUE;				
}
UINT logProcess(LPVOID Ptr)
{
	CTcpCommSvrDlg *pp = (CTcpCommSvrDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartLogPro();
	return 1;
}
//
BOOL CTcpCommSvrDlg::StartDataPro()
{	
    //数据处理
	time_t nowtime;
	time_t fsttime = 0;
	time_t lsttime = 0;
	int frameLen = 0;
	int dataSize = 0;
	unsigned char tmpBuf[DATA_FRAME_LEN];
    CString strTmp;
	int ireaderNo = 0;
	unsigned int icardNo = 0;
	char stmp[256];
	char scontent[1024];
	int cardnum = 0;
	CStatic *pwndMsg = NULL;
	pwndMsg=(CStatic*)GetDlgItem(IDC_STATIC_Q_COUNT); //提取静态文本框句柄
	CStatic *pwndInOut = NULL;
	pwndInOut=(CStatic*)GetDlgItem(IDC_STATIC_IN_OUT_SIZE); //提取静态文本框句柄
	while(m_bolData)
	{
		Sleep(3);
		time(&nowtime);
		while(!m_pDataQueue->EmptyQueue()) 
		{
			dataSize = m_pDataQueue->GetDataRecordSum();
			if(dataSize <= 0) 
				break;
			//
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%d",dataSize);
			if(NULL != pwndMsg)
			{
				pwndMsg->SetWindowText(stmp); //输出
			}
			//
			memset(tmpBuf,0,sizeof(tmpBuf));
			dataSize = 0;
			m_pDataQueue->ReadQueue(tmpBuf, &dataSize);
			//正常读卡信息
			if(0x0B == tmpBuf[0])
			{
				//解析数据帧
				stuCardInfo cardInfo;
				memset(&cardInfo,0,sizeof(stuCardInfo));
				//
				unsigned char *pp = tmpBuf;
				if(IS_NET == m_connType)
				{
				    pp += 2;
				}
				else
				{
				    pp += 1;
				}
				ireaderNo = pp[0];
				pp += 3;
				cardnum = pp[0];
				pp += 1;
				//对cardnum检核，增加健壮性
				int dataLen_1 = 0;
				int dataLen_2 = 0;
				if(IS_NET == m_connType)
				{
					dataLen_1 = 6+cardnum*8+1;
					dataLen_2 = 6+cardnum*14+1;
				}
				else
				{
					dataLen_1 = 5+cardnum*8+1;
					dataLen_2 = 5+cardnum*14+1;
				}
				if((dataLen_1 == dataSize)||(dataLen_2 == dataSize))
				{}
				else
				{
				    //数据长度有误
					continue;
				}
				int card_data_len = 8;
				if(dataLen_2 == dataSize)card_data_len = 13;
				//
				unsigned char ustmp[256];
				CString strCardNo;
				for(int i = 0; i < cardnum; i++)
				{
					//
					memset(ustmp,0,sizeof(ustmp));
					memcpy(ustmp,pp,4);
					icardNo = ustmp[0]*256*256*256 + ustmp[1]*256*256 + ustmp[2]*256 + ustmp[3];
					strCardNo.Format("%010u",icardNo);
					pp += 4;
					if(2 == m_protocolType)
					{
						//map<unsigned int, stuCardInOutStatus> m_mapCardInOutStatus;
						map<unsigned int, stuCardInOutStatus>::iterator iter_ios;
						iter_ios = m_mapCardInOutStatus.find(icardNo);
						if(iter_ios == m_mapCardInOutStatus.end())
						{
							pp += 4;
							continue;
						}
						if(1 == iter_ios->second.gdwzd)
						{
							pp += 4;
							continue;
						}
						//新协议解包
						//轴
						unsigned char uch = pp[0];
						unsigned char axis_flg = (uch & 0x02)>>1;
						pp += 1;
						//激活器编号
						unsigned char uch_no = pp[0];
						pp += 1;
						//天线编号，信号强度
						uch = pp[0];
						//天线编号
						unsigned char uch_ano = (uch>>5) & 0x07;
						//信号强度
						unsigned char uch_value = uch & 0x19;
						pp += 1;
						//校验
						pp += 1;
						//
						unsigned int iNo = uch_no * 256 + uch_ano;
						//先判断是否进出map<unsigned int,stuInOutPointInfo> m_mapPointInOutInfo;
						map<unsigned int,stuInOutPointInfo>::iterator iter_io;
						iter_io = m_mapPointInOutInfo.find(iNo);
						if(iter_io != m_mapPointInOutInfo.end())
						{
							stuInOutInfo inOutInfo;
							if(iter_ios->second.io_flg != iter_io->second.io_flg)
							{
								//有进出动作
								//卡号
								memset(inOutInfo.card_no,0,sizeof(inOutInfo.card_no));
								sprintf(inOutInfo.card_no,"%010u",icardNo);
								//时间20170911165010
								time_t time_now = time(NULL);
								struct tm *now = NULL;
								now = localtime(&time_now);
								memset(stmp,0,sizeof(stmp));
								sprintf(stmp,"%04d%02d%02d%02d%02d%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
								memset(inOutInfo.io_time,0,sizeof(inOutInfo.io_time));
								memcpy(inOutInfo.io_time,stmp+2,12);
								//进出库状态
								memset(inOutInfo.sflag,0,sizeof(inOutInfo.sflag));
								memset(stmp,0,sizeof(stmp));
								sprintf(stmp,"%d",iter_io->second.io_flg); 
								memcpy(inOutInfo.sflag,stmp,1);
                                //
								PutInOutInfo(inOutInfo);
								//出入库信息信息
								memset(scontent,0,sizeof(scontent));
								sprintf(scontent,"出入库信息[%s][%s][%s]",inOutInfo.sflag,inOutInfo.card_no,inOutInfo.io_time);
								AddLogMsg(GRADE_LOG_WARN,scontent);
								//
								iter_ios->second.io_flg = iter_io->second.io_flg;
								//
							}
							continue;
						}
						//
						//map<unsigned int,stuPointInfo>::iterator iter_a;
						//iter_a = m_mapCardPointInfo[icardNo].find(iNo);
						uch_value += 1;
						unsigned char ucx = 0;
						unsigned char ucy = 0;
						unsigned char ucz = 0;
						//m_mapPoindCardBindInfo
						map<unsigned int,stuPointCardBind>::iterator iter_pcbi;
						iter_pcbi = m_mapPoindCardBindInfo.find(iNo);
						if(iter_pcbi != m_mapPoindCardBindInfo.end())
						{
							if(icardNo == iter_pcbi->second.card_no)
							{
								ucx = iter_pcbi->second.x;
								ucy = iter_pcbi->second.y;
								ucz = iter_pcbi->second.z;
							}
						}
						if(0 == axis_flg)
						{
						    m_mapCardPointInfo[icardNo][iNo].point_signal_x = uch_value + ucx;
						}
						else
						{
							m_mapCardPointInfo[icardNo][iNo].point_signal_y = uch_value + ucy;
						}
						m_mapCardPointInfo[icardNo][iNo].lst_time = time(NULL);
						//
						if(0 == axis_flg)
						{
						    m_mapPointCardInfo[iNo][icardNo].point_signal_x = uch_value + ucx;
						}
						else
						{
							m_mapPointCardInfo[iNo][icardNo].point_signal_y = uch_value + ucy;
						}
						m_mapPointCardInfo[iNo][icardNo].lst_time = time(NULL);
						/////////////////////////////////////
						time(&nowtime);
						//读卡器卡片对应关系
						fsttime = m_mapReaderCardInfo[ireaderNo][icardNo].fstTime;
						lsttime = m_mapReaderCardInfo[ireaderNo][icardNo].lstTime;
						if(0 == fsttime)
						{
							m_mapReaderCardInfo[ireaderNo][icardNo].fstTime = nowtime;
							m_mapReaderCardInfo[ireaderNo][icardNo].lstTime = nowtime;
							m_mapReaderCardInfo[ireaderNo][icardNo].readCount += 1;
						}
						else
						{
							if((lsttime-fsttime) > m_freshTime)
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].fstTime = nowtime;
								m_mapReaderCardInfo[ireaderNo][icardNo].lstTime = nowtime;
								m_mapReaderCardInfo[ireaderNo][icardNo].readCount = 1;
							}
							else
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].lstTime = nowtime;
								m_mapReaderCardInfo[ireaderNo][icardNo].readCount += 1;
							}
						}
						//m_mapCardTimeInfo
						m_mapCardTimeInfo[icardNo] = nowtime;
						//m_mapCardReaderInfoVec
						/////////*size_t vecSize = m_mapCardReaderInfoVec[icardNo].size();
						////////if(0 == vecSize)
						////////{
						////////	stuReaderInfo readInfo;
						////////	memset(&readInfo,0,sizeof(stuReaderInfo));
						////////	readInfo.fstTime = nowtime;
						////////	readInfo.lstTime = nowtime;
						////////	readInfo.readNo = ireaderNo;
						////////	readInfo.readCount = 1;
						////////	m_mapCardReaderInfoVec[icardNo].push_back(readInfo);
						////////}
						////////else
						////////{
						////////	BOOL bFind = FALSE;
						////////	fsttime = m_mapCardReaderInfoVec[icardNo][0].fstTime;
						////////	lsttime = m_mapCardReaderInfoVec[icardNo][0].lstTime;
						////////	if((lsttime-fsttime) > 2*m_freshTime)
						////////	{
						////////		for(int i = 0; i < vecSize; i++)
						////////		{
						////////			m_mapCardReaderInfoVec[icardNo][i].fstTime = nowtime;
						////////			m_mapCardReaderInfoVec[icardNo][i].lstTime = nowtime;
						////////			m_mapCardReaderInfoVec[icardNo][i].readCount = 0;
						////////			if(m_mapCardReaderInfoVec[icardNo][i].readNo == ireaderNo)
						////////			{
						////////				m_mapCardReaderInfoVec[icardNo][i].readCount = 1;
						////////				bFind = TRUE;
						////////			}
						////////		}
						////////		if(FALSE == bFind)
						////////		{
						////////			stuReaderInfo readInfo;
						////////			memset(&readInfo,0,sizeof(stuReaderInfo));
						////////			readInfo.fstTime = nowtime;
						////////			readInfo.lstTime = nowtime;
						////////			readInfo.readNo = ireaderNo;
						////////			readInfo.readCount = 1;
						////////			m_mapCardReaderInfoVec[icardNo].push_back(readInfo);
						////////		}
						////////	}
						////////	else
						////////	{
						////////		for(int i = 0; i < vecSize; i++)
						////////		{
						////////			m_mapCardReaderInfoVec[icardNo][i].lstTime = nowtime;
						////////			if(m_mapCardReaderInfoVec[icardNo][i].readNo == ireaderNo)
						////////			{
						////////				m_mapCardReaderInfoVec[icardNo][i].readCount += 1;
						////////				bFind = TRUE;
						////////			}
						////////		}
						////////		if(FALSE == bFind)
						////////		{
						////////			stuReaderInfo readInfo;
						////////			memset(&readInfo,0,sizeof(stuReaderInfo));
						////////			readInfo.fstTime = fsttime;
						////////			readInfo.lstTime = nowtime;
						////////			readInfo.readNo = ireaderNo;
						////////			readInfo.readCount = 1;
						////////			m_mapCardReaderInfoVec[icardNo].push_back(readInfo);
						////////		}
						////////	}
						////////}*/
						/////////////////////////////////////////
					}
					else
					{
						//map<unsigned int, stuCardInOutStatus> m_mapCardInOutStatus;
						map<unsigned int, stuCardInOutStatus>::iterator iter_ios;
						iter_ios = m_mapCardInOutStatus.find(icardNo);
						if(iter_ios == m_mapCardInOutStatus.end())
						{
						    if(13 == card_data_len)
						    {
								pp += 10;
							}
							else
							{
							    pp += 4;
							}
							continue;
						}
						//先判断是否进出map<unsigned int,stuInOutDevInfo> m_mapPointInOutInfo;
						map<unsigned int,stuInOutDevInfo>::iterator iter_io;
						iter_io = m_mapDevInOutInfo.find(ireaderNo);
						if(iter_io != m_mapDevInOutInfo.end())
						{
							stuInOutInfo inOutInfo;
							if(iter_ios->second.io_flg != iter_io->second.io_flg)
							{
								//有进出动作
								//卡号
								memset(inOutInfo.card_no,0,sizeof(inOutInfo.card_no));
								sprintf(inOutInfo.card_no,"%010u",icardNo);
								//时间20170911165010
								time_t time_now = time(NULL);
								struct tm *now = NULL;
								now = localtime(&time_now);
								memset(stmp,0,sizeof(stmp));
								sprintf(stmp,"%04d%02d%02d%02d%02d%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
								memset(inOutInfo.io_time,0,sizeof(inOutInfo.io_time));
								memcpy(inOutInfo.io_time,stmp+2,12);
								//进出库状态
								memset(inOutInfo.sflag,0,sizeof(inOutInfo.sflag));
								memset(stmp,0,sizeof(stmp));
								sprintf(stmp,"%d",iter_io->second.io_flg); 
								memcpy(inOutInfo.sflag,stmp,1);
                                //
								PutInOutInfo(inOutInfo);
								//出入库信息信息
								memset(scontent,0,sizeof(scontent));
								sprintf(scontent,"出入库信息[%s][%s][%s]",inOutInfo.sflag,inOutInfo.card_no,inOutInfo.io_time);
								AddLogMsg(GRADE_LOG_WARN,scontent);
								//
								iter_ios->second.io_flg = iter_io->second.io_flg;
								//
								if(1 == m_stuStationPara.up_type)
								{
									//上传
									m_netTcpClient.SetSendCardData(&iter_ios->second);
								}
							}
						    if(13 == card_data_len)
						    {
								pp += 10;
							}
							else
							{
							    pp += 4;
							}
							continue;
						}
						//
						//插入或更新卡片、读卡器信息
						//map<int,map<int,stuCardInfo>> m_mapReaderCardInfo;
						//map<int,map<int,stuCardInfo>> m_mapCardReaderInfo;
						////////////
						map<unsigned int,stuCardInfo>::iterator iter;
						iter = m_mapReaderCardInfo[ireaderNo].find(icardNo);
						if(m_mapReaderCardInfo[ireaderNo].end() == iter)
						{
							m_mapReaderCardInfo[ireaderNo][icardNo].voltage = 0;
							m_mapReaderCardInfo[ireaderNo][icardNo].current = 0;
							m_mapReaderCardInfo[ireaderNo][icardNo].temperature = 0;
						}
						else
						{
							//
							memset(ustmp,0,sizeof(ustmp));
							memcpy(ustmp,pp,4);
							int iTmp1 = ustmp[0]*256 + ustmp[1];
							int iTmp2 = ustmp[0]*256 + ustmp[2];
							memset(stmp,0,sizeof(stmp));
							sprintf(stmp,"%d.%02d",iTmp1,iTmp2);
							//电压
							if(0xA0 == ustmp[3])
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].voltage = atof(stmp);
							}
							//电流
							if(0xA1 == ustmp[3])
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].current = atof(stmp);
							}
							//温度
							if(0xA6 == ustmp[3])
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].temperature = atof(stmp);
							}
							//
						}
						//腕带
						if(13 == card_data_len)
						{
			                m_mapReaderCardInfo[ireaderNo][icardNo].data_len = 9;
							memcpy(m_mapReaderCardInfo[ireaderNo][icardNo].data,pp,9);
							pp += 10;
						}
						else
						{
							m_mapReaderCardInfo[ireaderNo][icardNo].data_len = 0;
							//memset(m_mapReaderCardInfo[ireaderNo][icardNo].data,0,sizeof(m_mapReaderCardInfo[ireaderNo][icardNo].data));
							pp += 4;
						}
						time(&nowtime);
						//读卡器卡片对应关系
						fsttime = m_mapReaderCardInfo[ireaderNo][icardNo].fstTime;
						lsttime = m_mapReaderCardInfo[ireaderNo][icardNo].lstTime;
						if(0 == fsttime)
						{
							m_mapReaderCardInfo[ireaderNo][icardNo].fstTime = nowtime;
							m_mapReaderCardInfo[ireaderNo][icardNo].lstTime = nowtime;
							m_mapReaderCardInfo[ireaderNo][icardNo].readCount += 1;
						}
						else
						{
							if((lsttime-fsttime) > m_freshTime)
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].fstTime = nowtime;
								m_mapReaderCardInfo[ireaderNo][icardNo].lstTime = nowtime;
								m_mapReaderCardInfo[ireaderNo][icardNo].readCount = 1;
							}
							else
							{
								m_mapReaderCardInfo[ireaderNo][icardNo].lstTime = nowtime;
								m_mapReaderCardInfo[ireaderNo][icardNo].readCount += 1;
							}
						}
						//m_mapCardReaderInfoVec
						size_t vecSize = m_mapCardReaderInfoVec[icardNo].size();
						if(0 == vecSize)
						{
							stuReaderInfo readInfo;
							memset(&readInfo,0,sizeof(stuReaderInfo));
							readInfo.fstTime = nowtime;
							readInfo.lstTime = nowtime;
							readInfo.readNo = ireaderNo;
							readInfo.readCount = 1;
							m_mapCardReaderInfoVec[icardNo].push_back(readInfo);
						}
						else
						{
							BOOL bFind = FALSE;
							fsttime = m_mapCardReaderInfoVec[icardNo][0].fstTime;
							lsttime = m_mapCardReaderInfoVec[icardNo][0].lstTime;
							if((lsttime-fsttime) > 2*m_freshTime)
							{
								for(int i = 0; i < vecSize; i++)
								{
									m_mapCardReaderInfoVec[icardNo][i].fstTime = nowtime;
									m_mapCardReaderInfoVec[icardNo][i].lstTime = nowtime;
									m_mapCardReaderInfoVec[icardNo][i].readCount = 0;
									if(m_mapCardReaderInfoVec[icardNo][i].readNo == ireaderNo)
									{
										m_mapCardReaderInfoVec[icardNo][i].readCount = 1;
										bFind = TRUE;
									}
								}
								if(FALSE == bFind)
								{
									stuReaderInfo readInfo;
									memset(&readInfo,0,sizeof(stuReaderInfo));
									readInfo.fstTime = nowtime;
									readInfo.lstTime = nowtime;
									readInfo.readNo = ireaderNo;
									readInfo.readCount = 1;
									m_mapCardReaderInfoVec[icardNo].push_back(readInfo);
								}
							}
							else
							{
								for(int i = 0; i < vecSize; i++)
								{
									m_mapCardReaderInfoVec[icardNo][i].lstTime = nowtime;
									if(m_mapCardReaderInfoVec[icardNo][i].readNo == ireaderNo)
									{
										m_mapCardReaderInfoVec[icardNo][i].readCount += 1;
										bFind = TRUE;
									}
								}
								if(FALSE == bFind)
								{
									stuReaderInfo readInfo;
									memset(&readInfo,0,sizeof(stuReaderInfo));
									readInfo.fstTime = fsttime;
									readInfo.lstTime = nowtime;
									readInfo.readNo = ireaderNo;
									readInfo.readCount = 1;
									m_mapCardReaderInfoVec[icardNo].push_back(readInfo);
								}
							}
						}
						//
					    //pp += 4;
					}//end protocol type
				}//end for
			}
			//出入库信息
			if(0x2A == tmpBuf[0])
			{
				stuInOutInfo inOutInfo;
				memset(&inOutInfo,0,sizeof(stuInOutInfo));
				//
				unsigned char *pp = tmpBuf;
				pp += 1;
				//记录条数
				memset(stmp,0,sizeof(stmp));
				memcpy(stmp,pp,2);
				pp += 2;
				cardnum = atoi(stmp);
				//对cardnum检核，增加健壮性
				////if(6+cardnum*8+1 != dataSize)
				////{
				////    //数据长度有误
				////	continue;
				////}
				//序号
				memcpy(inOutInfo.sitem,pp,3);
				pp += 3;
				//
				for(int i = 0; i < cardnum; i++)
				{
					//卡号
					memset(inOutInfo.card_no,0,sizeof(inOutInfo.card_no));
					memcpy(inOutInfo.card_no,pp,10);
					pp += 10;
                    //时间
					memset(inOutInfo.io_time,0,sizeof(inOutInfo.io_time));
					memcpy(inOutInfo.io_time,pp,12);
					pp += 12;
					//进出库状态
					memset(inOutInfo.sflag,0,sizeof(inOutInfo.sflag));
					memcpy(inOutInfo.sflag,pp,1);
					pp += 1;
					//电池状态
					pp += 1;
					//
					PutInOutInfo(inOutInfo);
					//出入库信息信息
					memset(scontent,0,sizeof(scontent));
					sprintf(scontent,"出处库信息[%s][%s][%s]",inOutInfo.sflag,inOutInfo.card_no,inOutInfo.io_time);
					AddLogMsg(GRADE_LOG_WARN,scontent);
				}
				if(cardnum > 0)
				{
					memset(stmp,0,sizeof(stmp));
					sprintf(stmp,"%d",m_vecInOutInfo.size());
					if(NULL != pwndInOut)
					{
						pwndInOut->SetWindowText(stmp); //输出
					}
					//
				}
			}
		}
	}
    //
	return TRUE;				
}
UINT dataProcess(LPVOID Ptr)
{
	CTcpCommSvrDlg *pp = (CTcpCommSvrDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartDataPro();
	return 1;
}
//辅助线程
BOOL CTcpCommSvrDlg::StartHelper()
{	
	int iCount = 0;
	int iNum = 0;
	char stmp[256];
    while(m_bolHelper)
	{
		if(WAIT_OBJECT_0==WaitForSingleObject(m_hConnFreshEvent,INFINITE))
		{
			if(FALSE == m_bolHelper)break;
			iCount = 0;
			m_connInfo.DeleteAllItems();//清空
			vector<stuSockInfo> vecSockInfo;
			m_netTcpSvr.GetAllSockInfo(vecSockInfo);
			vector<stuSockInfo>::iterator iter;
			for(iter = vecSockInfo.begin(); iter != vecSockInfo.end(); iter++)
			{
				iCount++;
				//序号
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%03d",iCount);
				m_connInfo.InsertItem(iCount-1, _T(stmp));
				//设备编号
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%03d",iter->reader_no);
				m_connInfo.SetItemText(iCount-1, 1, _T(stmp));
				//IP:[端口]
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%s:[%d]",iter->ip.c_str(),iter->port);
				m_connInfo.SetItemText(iCount-1, 2, _T(stmp));
			}
			iNum = m_netTcpSvr.GetSocketNum();
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%d",iNum);
			CStatic *pwnd = NULL;
			pwnd=(CStatic*)GetDlgItem(IDC_STATIC_CONN_NUM); //提取静态文本框句柄
			if(NULL != pwnd)
			{
				pwnd->SetWindowText(stmp); //输出
			}
		}
	}    
    //
	return TRUE;				
}
UINT helperThread(LPVOID Ptr)
{
	CTcpCommSvrDlg *pp = (CTcpCommSvrDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartHelper();
	return 1;
}
//
BOOL CTcpCommSvrDlg::FreshCardInfoDB()
{
	/////////////
	///*stuReaderInfo stuTmp;
	//stuTmp.fstTime = time(NULL) - m_freshTime - 1;
	//stuTmp.lstTime = time(NULL) - 2*m_freshTime + 3;
	//stuTmp.readNo = 1;
	//stuTmp.readCount = 10;
	//m_mapCardReaderInfoVec[1755605117].clear();
	//m_mapCardReaderInfoVec[1755605117].push_back(stuTmp);
	//m_mapCardReaderInfoVec[2387869110].clear();
	//m_mapCardReaderInfoVec[2387869110].push_back(stuTmp);
	//m_mapCardReaderInfoVec[2387869111].clear();
	//m_mapCardReaderInfoVec[2387869111].push_back(stuTmp);
	//m_mapCardReaderInfoVec[2387869112].clear();
	//m_mapCardReaderInfoVec[2387869112].push_back(stuTmp);*/
	/////////////////
	time_t nowtime = time(NULL);
	size_t vecSize = 0;
	time_t fsttime = 0;
	time_t lsttime = 0;
	map<unsigned int,vector<stuReaderInfo>>::iterator iter;
	for(iter = m_mapCardReaderInfoVec.begin(); iter != m_mapCardReaderInfoVec.end(); iter++)
	{
		//vector<stuReaderInfo>
		vecSize = iter->second.size();
		BOOL bFind = FALSE;
		int iReaderNo = 0;
		int iCount = 0;
		if(vecSize > 0)
		{
			fsttime = iter->second[0].fstTime;
			lsttime = iter->second[0].lstTime;
			nowtime = time(NULL);
			if((nowtime-lsttime) < 2*m_freshTime)
			{
				if((nowtime-fsttime) > m_freshTime)
				{
					for(int i = 0; i < vecSize; i++)
					{
						if(iter->second[i].readCount > iCount)
						{
							iReaderNo = iter->second[i].readNo;
							iCount = iter->second[i].readCount;
						}
					}
					if(iReaderNo > 0)
					{
					    bFind = TRUE;
					}
				}
			}
			if(TRUE == bFind)
			{
				//
				//m_dlgTblTmd.SetCardToDisplayData(iter->first,iReaderNo,lsttime,m_mapReaderCardInfo[iReaderNo][iter->first].data_len,m_mapReaderCardInfo[iReaderNo][iter->first].data);
				//显示
				if(NULL != m_hzpSetData)
				{
					m_hzpSetData(iter->first,iReaderNo,lsttime,0,NULL);
				}
				SetAttendanceData(iter->first,lsttime);
				//
				unsigned int icardNo = iter->first;
				m_mapCardInOutStatus[icardNo].card_no = icardNo;
				m_mapCardInOutStatus[icardNo].room_no = iReaderNo;
				//m_mapCardInOutStatus[icardNo].room_name = m_mapRoomNoName[dstNo];
				m_mapCardInOutStatus[icardNo].lst_time = lsttime;
			}
		}	
	}	
    //
	UpdateCardInfo_DB();
	//
	return TRUE;
}
BOOL CTcpCommSvrDlg::FreshInOutInfoDB()
{
	UpdateInOutInfo_DB();
	//
	return TRUE;
}
BOOL CTcpCommSvrDlg::StartDbFresh()
{	
	//
    while(m_bolDb)
	{
		//更新卡片信息入库
		Sleep(1000);
        FreshCardInfoDB();
		//更新出入库信息
		//Sleep(3000);
		//FreshInOutInfoDB();
	}    
    //
	return TRUE;				
}
UINT dbFresh(LPVOID Ptr)
{
	CTcpCommSvrDlg *pp = (CTcpCommSvrDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartDbFresh();
	
	return 1;
}
//
BOOL CTcpCommSvrDlg::FreshCardInfoDB_New()
{
	
	//计算卡是属于哪个房间
	struct rs
	{
		int sin;
		time_t tim;
	};
	unsigned int icardNo = 0;
	map < unsigned int, map<unsigned int, stuPointInfo>>::iterator iter;
	for (iter = m_mapCardPointInfo.begin(); iter != m_mapCardPointInfo.end(); iter++)
	{
		map<int, rs> mapRoomSingal;   //记录房间信号强度
		mapRoomSingal.clear();
		icardNo = iter->first;
		map<unsigned int, stuPointInfo>::iterator iter_card;
		for (iter_card = iter->second.begin(); iter_card != iter->second.end(); iter_card++)
		{
			time_t timenow = time(NULL);
			if ((timenow - iter_card->second.lst_time) > m_freshRatio*m_freshTime)
			{
				iter_card->second.point_signal_x = 0;
				iter_card->second.point_signal_y = 0;
				iter_card->second.point_signal_z = 0;
				continue;
			}
			unsigned int pointNo = iter_card->first;
			map <unsigned int, stuPointRoomInfo>::iterator iterRoom;
			iterRoom = m_mapPointRoomInfo.find(pointNo);
			int roomNo = 0;
			if (iterRoom != m_mapPointRoomInfo.end())
			{
				roomNo = m_mapPointRoomInfo[pointNo].room_no;
				int point_signal = iter_card->second.point_signal_x + iter_card->second.point_signal_y + iter_card->second.point_signal_z;
				mapRoomSingal[roomNo].sin += point_signal;
				mapRoomSingal[roomNo].tim = iter_card->second.lst_time;
			}
		}
		map<int, rs>::iterator iter_singal;
		int dstNo = 0;
		int tmpSingal = 0;
		time_t tmpTime = 0;
		for (iter_singal = mapRoomSingal.begin(); iter_singal != mapRoomSingal.end(); iter_singal++)
		{
			if (tmpSingal < iter_singal->second.sin)
			{
				tmpSingal = iter_singal->second.sin;
				tmpTime = iter_singal->second.tim;
				dstNo = iter_singal->first;
			}

		}
		if(mapRoomSingal.size() > 0)
		{
			//
			m_mapCardInOutStatus[icardNo].room_no = dstNo;
			//m_mapCardInOutStatus[icardNo].room_name = m_mapRoomNoName[dstNo];
			m_mapCardInOutStatus[icardNo].lst_time = tmpTime;
		}
		else
		{
			//////m_mapCardInOutStatus[icardNo].room_no = 0;
			m_mapCardInOutStatus[icardNo].lst_time = time(NULL);
		}
	}
	//计算进出20190119
	char stmp[256];
	char scontent1[1024];
	map<unsigned int, stuCardInOutStatus>::iterator iter_cr;
	for(iter_cr = m_mapCardInOutStatus.begin(); iter_cr != m_mapCardInOutStatus.end(); iter_cr++)
	{
		int io_tmp = 0;
		map<unsigned int,time_t>::iterator iter_ct;
		iter_ct = m_mapCardTimeInfo.find(iter_cr->first);
		if(m_mapCardTimeInfo.end() != iter_ct)
		{
			time_t t_inv = time(NULL) - iter_ct->second;
			if(t_inv > 2*60*1000)
			{
				//大于2分钟，且之前是入，则出
				if(1 == iter_cr->second.io_flg)
				{
					iter_cr->second.io_flg = 2;
					//
					io_tmp = 1;
					//发送给门口读卡器
					SendCmdInOut(21);
					SendCmdInOut(22);
				}
			}
			else
			{
				if(1 != iter_cr->second.io_flg)
				{
					iter_cr->second.io_flg = 1;
					//
					io_tmp = 1;
				}
			}
			if(1 == io_tmp)
			{
				stuInOutInfo inOutInfo;
				//有进出动作
				//卡号
				memset(inOutInfo.card_no,0,sizeof(inOutInfo.card_no));
				sprintf(inOutInfo.card_no,"%010u",iter_cr->first);
				//时间20170911165010
				time_t time_now = time(NULL);
				struct tm *now = NULL;
				now = localtime(&time_now);
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%04d%02d%02d%02d%02d%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec); 
				memset(inOutInfo.io_time,0,sizeof(inOutInfo.io_time));
				memcpy(inOutInfo.io_time,stmp+2,12);
				//进出库状态
				memset(inOutInfo.sflag,0,sizeof(inOutInfo.sflag));
				memset(stmp,0,sizeof(stmp));
				sprintf(stmp,"%d",iter_cr->second.io_flg); 
				memcpy(inOutInfo.sflag,stmp,1);
				//
				PutInOutInfo(inOutInfo);
				//出入库信息信息
				memset(scontent1,0,sizeof(scontent1));
				sprintf(scontent1,"出入库信息[%s][%s][%s]",inOutInfo.sflag,inOutInfo.card_no,inOutInfo.io_time);
				AddLogMsg(GRADE_LOG_WARN,scontent1);
				//
			}
		}
	}
	//
	UpdateCardInfo_DB();
	//
	return TRUE;
}
BOOL CTcpCommSvrDlg::FreshInOutInfoDB_New()
{
    UpdateInOutInfo_DB();
	return TRUE;
}
BOOL CTcpCommSvrDlg::StartDbFresh_New()
{	
	//
    while(m_bolDb)
	{				
		//更新卡片信息入库
		Sleep(1000);
        FreshCardInfoDB_New();
		//更新出入库信息
		Sleep(1000);
		FreshInOutInfoDB_New();
	}    
    //
	return TRUE;				
}
UINT dbFresh_New(LPVOID Ptr)
{
	CTcpCommSvrDlg *pp = (CTcpCommSvrDlg *)Ptr;//
	if(pp==NULL)
	{
		return TRUE;
	}
	pp->StartDbFresh_New();
	
	return 1;
}
//
BOOL CTcpCommSvrDlg::UpdateCardInfo_DB()
{
	//写数据库
	int cardCount = 0;
	char cardBuf[PRO_CARD_NUM*11+10];  //保存卡片列表传给存储过程
	char timeBuf[PRO_CARD_NUM*20+10];  //
	char preroomBuf[PRO_CARD_NUM*4+10]; //保存房间列表传给存储过程
	char roomBuf[PRO_CARD_NUM*4+10]; //保存房间列表传给存储过程
	char cardNo[20];
	char preroomNo[10];
	char roomNo[10];
	char stime[30];
	char scontent[1024];
	time_t nowtime = time(NULL);
	//遍历卡片列表
	memset(cardBuf,0,sizeof(cardBuf));
	memset(timeBuf,0,sizeof(timeBuf));
	memset(preroomBuf,0,sizeof(preroomBuf));
	memset(roomBuf,0,sizeof(roomBuf));
	map<unsigned int, stuCardInOutStatus>::iterator iter_cr;
	for(iter_cr = m_mapCardInOutStatus.begin(); iter_cr != m_mapCardInOutStatus.end(); iter_cr++)
	{
		if(iter_cr->second.pre_room_no == iter_cr->second.room_no)
		{
			continue;
		}
		else
		{
			if(0 == iter_cr->second.room_no)continue;
			if(iter_cr->second.io_flg > 2) iter_cr->second.io_flg = 0;
			if(iter_cr->second.pre_room_no > 65535) iter_cr->second.pre_room_no = 0;
			//
			memset(cardNo,0,sizeof(cardNo));
			memset(preroomNo,0,sizeof(preroomNo));
		    memset(roomNo,0,sizeof(roomNo));
			memset(stime,0,sizeof(stime));
			sprintf(cardNo,"%010u",iter_cr->first);
			sprintf(preroomNo,"%03d",iter_cr->second.pre_room_no);
			sprintf(roomNo,"%03d",iter_cr->second.room_no);
			TimeToStr(stime,iter_cr->second.lst_time);
			memset(scontent,0,sizeof(scontent));
			sprintf(scontent,"%d,%03d,%03d,%s",iter_cr->second.io_flg,iter_cr->second.pre_room_no,iter_cr->second.room_no,stime);
			WritePrivateProfileString("卡片信息",cardNo,scontent,".\\cfg\\card.ini");
			if(1 == m_useDb)
			{
				iter_cr->second.pre_room_no = iter_cr->second.room_no;
				continue;
			}
			strcat(cardBuf,cardNo);
			strcat(cardBuf,",");
			strcat(timeBuf,stime);
			strcat(timeBuf,",");
			strcat(preroomBuf,preroomNo);
			strcat(preroomBuf,",");
			strcat(roomBuf,roomNo);
			strcat(roomBuf,",");
			cardCount++;
			iter_cr->second.pre_room_no = iter_cr->second.room_no;
		}

		if(cardCount == PRO_CARD_NUM)
		{
			//更新卡片到数据库
			int len = strlen(cardBuf);
			if(len > 0)cardBuf[len-1] = 0;
			len = strlen(timeBuf);
			if(len > 0)timeBuf[len-1] = 0;
			len = strlen(preroomBuf);
			if(len > 0)preroomBuf[len-1] = 0;
			len = strlen(roomBuf);
			if(len > 0)roomBuf[len-1] = 0;
			BOOL bol = TRUE;
			if(SS_DB == m_useDb)
			{
				bol = UpdateCardInfo_SSDB(cardCount,cardBuf,preroomBuf,roomBuf);
			}
			else if(MS_DB == m_useDb)
			{
				bol = UpdateCardInfo_MSDB(cardCount,cardBuf,timeBuf,preroomBuf,roomBuf);
			}
			if(FALSE == bol)
			{
				memset(scontent,0,sizeof(scontent));
				strcpy(scontent,"执行CARD_INFO_P_UPDATE失败1");
				AddLogMsg(GRADE_LOG_WARN,scontent);
			}
			//清空列表
			cardCount = 0;
			memset(cardBuf,0,sizeof(cardBuf));
			memset(preroomBuf,0,sizeof(preroomBuf));
			memset(roomBuf,0,sizeof(roomBuf));
		}
	}
	if(cardCount > 0)
	{
		//更新剩余卡片到数据库
		int len = strlen(cardBuf);
		if(len > 0)cardBuf[len-1] = 0;
		len = strlen(timeBuf);
		if(len > 0)timeBuf[len-1] = 0;
		len = strlen(preroomBuf);
		if(len > 0)preroomBuf[len-1] = 0;
		len = strlen(roomBuf);
		if(len > 0)roomBuf[len-1] = 0;
		BOOL bol = TRUE;
		if(SS_DB == m_useDb)
		{
		    bol = UpdateCardInfo_SSDB(cardCount,cardBuf,preroomBuf,roomBuf);
		}
		else if(MS_DB == m_useDb)
		{
			bol = UpdateCardInfo_MSDB(cardCount,cardBuf,timeBuf,preroomBuf,roomBuf);
		}
		if(FALSE == bol)
		{
			memset(scontent,0,sizeof(scontent));
			strcpy(scontent,"执行CARD_INFO_P_UPDATE失败2");
			AddLogMsg(GRADE_LOG_WARN,scontent);
		}
		//清空列表
		cardCount = 0;
		memset(preroomBuf,0,sizeof(preroomBuf));
		memset(roomBuf,0,sizeof(roomBuf));
	}
	//
	return TRUE;
}
BOOL CTcpCommSvrDlg::UpdateInOutInfo_DB()
{
	//
	vector<stuInOutInfo> vecInOutInfo;
	vecInOutInfo.clear();
	GetAllInOutInfo(vecInOutInfo);
	//
	char cardBuf[PRO_CARD_NUM*11+10];
	char timeBuf[PRO_CARD_NUM*20+10];
	char flagBuf[PRO_CARD_NUM*1+10];
	int cardCount = 0;
	char scontent[1024];
	char stime[30];
	//
	size_t vecSize = 0;
	vecSize = vecInOutInfo.size();
	memset(cardBuf,0,sizeof(cardBuf));
	memset(timeBuf,0,sizeof(timeBuf));
	memset(flagBuf,0,sizeof(flagBuf));
	for(int i = 0; i < vecSize; i++)
	{
		//
		unsigned int cardNo = strtoul(vecInOutInfo[i].card_no,NULL,10);
		map<unsigned int, stuCardInOutStatus>::iterator iter_cr;
		iter_cr = m_mapCardInOutStatus.find(cardNo);
		memset(stime,0,sizeof(stime));
		TimeToStr(stime,iter_cr->second.lst_time);
		memset(scontent,0,sizeof(scontent));
		sprintf(scontent,"%d,%03d,%03d,%s",iter_cr->second.io_flg,iter_cr->second.pre_room_no,iter_cr->second.room_no,stime);
		WritePrivateProfileString("卡片信息",vecInOutInfo[i].card_no,scontent,".\\cfg\\card.ini");
		//不用数据库
		if(1 == m_useDb)continue;
		//
        cardCount++;
		strcat(cardBuf,vecInOutInfo[i].card_no);
		strcat(cardBuf,",");
		strcat(timeBuf,vecInOutInfo[i].io_time);
		strcat(timeBuf,",");
		strcat(flagBuf,vecInOutInfo[i].sflag);
		strcat(flagBuf,",");
		if(cardCount == PRO_CARD_NUM)
		{
			//处理
			int len = strlen(cardBuf);
			if(len > 0)cardBuf[len-1] = 0;
			len = strlen(timeBuf);
			if(len > 0)timeBuf[len-1] = 0;
			len = strlen(flagBuf);
			if(len > 0)flagBuf[len-1] = 0;
			BOOL bol = TRUE;
			if(SS_DB == m_useDb)
			{
			    bol = UpdateInOutInfo_SSDB(cardCount,cardBuf,timeBuf,flagBuf);
			}
			else if(MS_DB == m_useDb)
			{
				bol = UpdateInOutInfo_MSDB(cardCount,cardBuf,timeBuf,flagBuf);
			}
			if(FALSE == bol)
			{
				memset(scontent,0,sizeof(scontent));
				strcpy(scontent,"执行INOUT_INFO_P_UPDATE失败1");
				AddLogMsg(GRADE_LOG_WARN,scontent);
			}
			//清空列表
			cardCount = 0;
			memset(cardBuf,0,sizeof(cardBuf));
			memset(timeBuf,0,sizeof(timeBuf));
			memset(flagBuf,0,sizeof(flagBuf));
		}
	}
	//
    if(cardCount > 0)
	{
		//处理
		int len = strlen(cardBuf);
		if(len > 0)cardBuf[len-1] = 0;
		len = strlen(timeBuf);
		if(len > 0)timeBuf[len-1] = 0;
		len = strlen(flagBuf);
		if(len > 0)flagBuf[len-1] = 0;
		BOOL bol = TRUE;
		if(SS_DB == m_useDb)
		{
		    bol = UpdateInOutInfo_SSDB(cardCount,cardBuf,timeBuf,flagBuf);
		}
		else if(MS_DB == m_useDb)
		{
			bol = UpdateInOutInfo_MSDB(cardCount,cardBuf,timeBuf,flagBuf);
		}
		if(FALSE == bol)
		{
			memset(scontent,0,sizeof(scontent));
			strcpy(scontent,"执行INOUT_INFO_P_UPDATE失败2");
			AddLogMsg(GRADE_LOG_WARN,scontent);
		}
		//清空列表
		cardCount = 0;
		memset(cardBuf,0,sizeof(cardBuf));
		memset(timeBuf,0,sizeof(timeBuf));
		memset(flagBuf,0,sizeof(flagBuf));
	}
	//
	return TRUE;
}
//更新数据库,sqlserver
//更新位置变化信息
BOOL CTcpCommSvrDlg::UpdateCardInfo_SSDB(int cardCount,const char *cardBuf,const char *preroomBuf,const char *roomBuf)
{
	char scontent[4096];
	char stmp[256];
	time_t time_now = time(NULL);
	struct tm tmTmp;
	tmTmp = *localtime(&time_now);
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,".\\log\\%d-%02d-%02d_card.txt",tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
	memset(scontent,0,sizeof(scontent));
	sprintf(scontent,"写位置变更信息[%02d:%02d:%02d][%d][%s][%s][%s]\n",tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec,cardCount,cardBuf,preroomBuf,roomBuf);
	//AddLogMsg(GRADE_LOG_COMM,scontent);
	FILE *fp = fopen(stmp,"a");
	if(NULL != fp)
	{
		fputs(scontent,fp);
		fclose(fp);
	}
	try
	{
		m_dbConnection->BeginTrans();
		_CommandPtr cmmd;
		HRESULT hr = cmmd.CreateInstance(__uuidof(Command));

		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("Return"),adInteger,adParamReturnValue,4));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("intCardNum"),adInteger,adParamInput,sizeof(int),cardCount));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strCardLst"),adVarChar,adParamInput,strlen(cardBuf),cardBuf));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strPreRoomLst"),adVarChar,adParamInput,strlen(preroomBuf),preroomBuf));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strRoomLst"),adVarChar,adParamInput,strlen(roomBuf),roomBuf));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strMsg"),adVarChar,adParamOutput,50));
        
		cmmd->CommandText = _bstr_t("CARD_INFO_P_UPDATE");
		cmmd->ActiveConnection = m_dbConnection;
		cmmd->CommandType = adCmdStoredProc;
		cmmd->Execute(NULL,NULL,adCmdStoredProc);
		string strRet = (const char*)(_bstr_t)cmmd->Parameters->GetItem("strMsg")->GetValue();
		int nRet = cmmd->Parameters->GetItem("Return")->GetValue();
		//cout<<strRet<<endl;
		m_dbConnection->CommitTrans();
	}
	catch(_com_error e)
	{
		char scontent[1024];
		memset(scontent,0,sizeof(scontent));
		sprintf(scontent,"写位置变更信息失败[%d]",cardCount);
		AddLogMsg(GRADE_LOG_WARN,scontent);
	   try
	   {
			//操作失败,回退
			m_dbConnection->RollbackTrans();
		}
		catch(_com_error e)
		{
		}
		return FALSE;
	}
	return TRUE;
}
//更新进出库变化信息
BOOL CTcpCommSvrDlg::UpdateInOutInfo_SSDB(int cardCount,const char *cardBuf,const char *timeBuf,const char *flagBuf)
{
	char scontent[4096];
	char stmp[256];
	time_t time_now = time(NULL);
	struct tm tmTmp;
	tmTmp = *localtime(&time_now);
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,".\\log\\%d-%02d-%02d.txt",tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
	memset(scontent,0,sizeof(scontent));
	sprintf(scontent,"写进出库变化信息[%02d:%02d:%02d][%d][%s][%s][%s]\n",tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec,cardCount,cardBuf,timeBuf,flagBuf);
	//AddLogMsg(GRADE_LOG_COMM,scontent);
	FILE *fp = fopen(stmp,"a");
	if(NULL != fp)
	{
		fputs(scontent,fp);
		fclose(fp);
	}
	try
	{
		m_dbConnection->BeginTrans();
		_CommandPtr cmmd;
		HRESULT hr = cmmd.CreateInstance(__uuidof(Command));

		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("Return"),adInteger,adParamReturnValue,4));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("intCardNum"),adInteger,adParamInput,sizeof(int),cardCount));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strCardLst"),adVarChar,adParamInput,strlen(cardBuf),cardBuf));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strTimerLst"),adVarChar,adParamInput,strlen(timeBuf),timeBuf));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strFlagLst"),adVarChar,adParamInput,strlen(flagBuf),flagBuf));
		cmmd->Parameters->Append(cmmd->CreateParameter(_bstr_t("strMsg"),adVarChar,adParamOutput,50));
        
		cmmd->CommandText = _bstr_t("INOUT_INFO_P_UPDATE");
		cmmd->ActiveConnection = m_dbConnection;
		cmmd->CommandType = adCmdStoredProc;
		cmmd->Execute(NULL,NULL,adCmdStoredProc);
		string strRet = (const char*)(_bstr_t)cmmd->Parameters->GetItem("strMsg")->GetValue();
		int nRet = cmmd->Parameters->GetItem("Return")->GetValue();
		//cout<<strRet<<endl;
		m_dbConnection->CommitTrans();
	}
	catch(_com_error e)
	{
		char scontent[1024];
		memset(scontent,0,sizeof(scontent));
		sprintf(scontent,"写出入库信息失败[%d]",cardCount);
		AddLogMsg(GRADE_LOG_WARN,scontent);
	   try
	   {
			//操作失败,回退
			m_dbConnection->RollbackTrans();
		}
		catch(_com_error e)
		{
		}
		return FALSE;
	}
	return TRUE;
}
//更新数据库,mysql
//更新位置变化信息
BOOL CTcpCommSvrDlg::UpdateCardInfo_MSDB(int cardCount,const char *cardBuf,const char *timeBuf,const char *preroomBuf,const char *roomBuf)
{
	////MYSQL_RES *res_ptr;
 ////   MYSQL_ROW sqlrow;
	////char tmpbuf[PRO_CARD_NUM*80];
	////memset(tmpbuf,0,sizeof(tmpbuf));
	////sprintf(tmpbuf,"call CARD_INFO_P_UPDATE(%d,'%s','%s','%s','%s',@t)",cardCount,cardBuf,timeBuf,preroomBuf,roomBuf);
	//////mysql_query(&m_myCont,"start transaction;");
	////int ret = -1;
	////ret = mysql_query(&m_myCont,tmpbuf);
	////if(0 == ret)
	////{
	////	//成功
 ////       //获得返回参数@t,@t是传出参数
 ////       mysql_query(&m_myCont, "select @t");
 ////       res_ptr = mysql_store_result(&m_myCont);
 ////       if (res_ptr)
 ////       {
 ////          sqlrow = mysql_fetch_row (res_ptr);
 ////       }
 ////       mysql_free_result (res_ptr);
	////	//mysql_commit(&m_myCont);
	////}
	////else
	////{
	////	//mysql_rollback(&m_myCont);
	////	//失败
	////}
	return TRUE;
}
//更新进出库变化信息
BOOL CTcpCommSvrDlg::UpdateInOutInfo_MSDB(int cardCount,const char *cardBuf,const char *timeBuf,const char *flagBuf)
{
	////MYSQL_RES *res_ptr;
 ////   MYSQL_ROW sqlrow;
	////char tmpbuf[PRO_CARD_NUM*80];
	////memset(tmpbuf,0,sizeof(tmpbuf));
	////sprintf(tmpbuf,"call INOUT_INFO_P_UPDATE(%d,'%s','%s','%s',@t)",cardCount,cardBuf,timeBuf,flagBuf);
	//////mysql_query(&m_myCont,"start transaction;");
	////int ret = -1;
	////ret = mysql_query(&m_myCont,tmpbuf);
	////if(0 == ret)
	////{
	////	//成功
 ////       //获得返回参数@t,@t是传出参数
 ////       mysql_query(&m_myCont, "select @t");
 ////       res_ptr = mysql_store_result(&m_myCont);
 ////       if (res_ptr)
 ////       {
 ////          sqlrow = mysql_fetch_row (res_ptr);
 ////       }
 ////       mysql_free_result (res_ptr);
	////	//mysql_commit(&m_myCont);
	////}
	////else
	////{
	////	//mysql_rollback(&m_myCont);
	////	//失败
	////}
	return TRUE;
}
//
void CTcpCommSvrDlg::PutInOutInfo(stuInOutInfo inOutInfo)
{
	m_csInOut.Lock();
	m_vecInOutInfo.push_back(inOutInfo);
	m_csInOut.Unlock();
}
void CTcpCommSvrDlg::GetAllInOutInfo(vector<stuInOutInfo> &vecInOutInfo)
{
	m_csInOut.Lock();
    std::copy(m_vecInOutInfo.begin(),m_vecInOutInfo.end(),std::back_inserter(vecInOutInfo));
	m_vecInOutInfo.clear();
	m_csInOut.Unlock();
}
//
void CTcpCommSvrDlg::GetCardInfo(int readerNo,map<unsigned int,stuCardInfo> &mapCardInfo)
{
    //map<int,map<int,stuCardInfo>> m_mapReaderCardInfo;
	//m_mapReaderCardInfo[123456][1].fstTime = time(NULL);
	//m_mapReaderCardInfo[123456][1].lstTime = time(NULL);
	//m_mapReaderCardInfo[123456][1].readCount = 7;
	//m_mapReaderCardInfo[123456][2].fstTime = time(NULL);
	//m_mapReaderCardInfo[123456][2].lstTime = time(NULL);
	//m_mapReaderCardInfo[123456][2].readCount = 3;
	//m_mapReaderCardInfo[123456][10].fstTime = time(NULL);
	//m_mapReaderCardInfo[123456][10].lstTime = time(NULL);
	//m_mapReaderCardInfo[123456][10].readCount = 19;
    //
	map<unsigned int,map<unsigned int,stuCardInfo>>::iterator iter;
	iter = m_mapReaderCardInfo.find(readerNo);
	if(iter != m_mapReaderCardInfo.end())
	{
		mapCardInfo = iter->second;
	}
	//std::copy(iter->second.begin(),iter->second.end(),std::back_inserter(mapCardInfo));
}
//
void CTcpCommSvrDlg::GetCardInfoVec(int readerNo,vector<stuCardInfo> &vecCardInfo)
{
    //map<int,vector<stuCardInfo>> m_mapReaderCardInfoVec;
	map<unsigned int,vector<stuCardInfo>>::iterator iter;
	iter = m_mapReaderCardInfoVec.find(readerNo);
	if(iter != m_mapReaderCardInfoVec.end())
	{
		vecCardInfo = iter->second;
	}	

}
//
void CTcpCommSvrDlg::GetReaderInfo(unsigned int cardNo,map<unsigned int,stuCardInfo> &mapReaderInfo)
{
    //map<int,map<int,stuCardInfo>> m_mapCardReaderInfo;
	//m_mapCardReaderInfo[123456][1].fstTime = time(NULL);
	//m_mapCardReaderInfo[123456][1].lstTime = time(NULL);
	//m_mapCardReaderInfo[123456][1].readCount = 7;
	//m_mapCardReaderInfo[123456][2].fstTime = time(NULL);
	//m_mapCardReaderInfo[123456][2].lstTime = time(NULL);
	//m_mapCardReaderInfo[123456][2].readCount = 3;
	//m_mapCardReaderInfo[123456][10].fstTime = time(NULL);
	//m_mapCardReaderInfo[123456][10].lstTime = time(NULL);
	//m_mapCardReaderInfo[123456][10].readCount = 19;
	//
    map<unsigned int,map<unsigned int,stuCardInfo>>::iterator iter;
	iter = m_mapCardReaderInfo.find(cardNo);
	if(iter != m_mapCardReaderInfo.end())
	{
	    mapReaderInfo = iter->second;
	}
}
//
void CTcpCommSvrDlg::GetReaderInfoVec(unsigned int cardNo,vector<stuReaderInfo> &vecReaderInfo)
{
	//map<unsigned int,vector<stuReaderInfo>> m_mapCardReaderInfoVec;
	map<unsigned int,vector<stuReaderInfo>>::iterator iter;
	iter = m_mapCardReaderInfoVec.find(cardNo);
	if(iter != m_mapCardReaderInfoVec.end())
	{
	    vecReaderInfo = iter->second;
	}
}
//
BOOL CTcpCommSvrDlg::FindSockInfoByReaderNo(int readerNo,stuSockInfo &sockInfo)
{
	return (m_netTcpSvr.FindSockInfoByReaderNo(readerNo,sockInfo));
}
//
void CTcpCommSvrDlg::SendCmdInOut(int readerNo)
{
	char sndBuf[256];
	char checkNum = 0;
	memset(sndBuf,0,sizeof(sndBuf));
	sndBuf[0] = 0x0A;
	sndBuf[1] = readerNo;
	sndBuf[2] = 0x04;
	sndBuf[3] = 0x23;
	sndBuf[4] = 0xF3;
	sndBuf[5] = 0x05;
	for(int i = 0; i < 6; i++)
	{
        checkNum += sndBuf[i];
	}
    sndBuf[6] = 0x00 - checkNum;
	//
	stuSockInfo sockinfo;
	BOOL bol = m_netTcpSvr.FindSockInfoByReaderNo(readerNo,sockinfo);
    if(TRUE == bol)
	{
		int ret = send(sockinfo.sock,sndBuf,7,0);
		if(ret == 7)
		{
			//MessageBox("发送成功！");
		}
	}
	else
	{
		//MessageBox("未找到该读卡器有效连接！");
	}
}
//
BOOL CTcpCommSvrDlg::AddLogMsg(int grade, const char *sbody)
{
	stuLogContent msg;
    msg.item           = 0;
	msg.type           = TYPE_LOG_DAT;
	msg.grade          = grade;
	msg.body           = sbody;
	msg.title          = "DAT";
	msg.datetime       = time(NULL);
	m_pLogMsg->AddLogMessage(msg);
	return TRUE;
}
//
void CTcpCommSvrDlg::DelMapTest(int key)
{

}
/////////////
//新协议相关函数
/******************************************************************************
* Purpose : ascii码转16进制
* Argument: source:ascii码输入
*           result:16进制输出
*           len:result输出的长度
* Author  : jjg 2011/08/30
*****************************************************************************/
void CTcpCommSvrDlg::StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	char h1, h2;
	char s1, s2;
	int i;

	for (i = 0; i<nLen; i++)
	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		pbDest[i] = s1 * 16 + s2;
	}
}
//
//加载
BOOL CTcpCommSvrDlg::LoadPointRoomMapping()
{
	m_mapPointRoomInfo.clear();
	m_mapRoomNoName.clear();
	char stmp[256];
	char sdest[256];
	string filePath = ".\\cfg\\point.ini";
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("激活器列表" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "总行数" || str_first == "总路数")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 6)
			{
				continue;
			}
			memset(stmp, 0, sizeof(stmp));
			memset(sdest, 0, sizeof(sdest));
			memcpy(stmp, str_id.c_str(), 4);
			StrToHex(sdest, stmp, 2);
			int itmpNo = (unsigned char)sdest[0] * 256 + (unsigned char)sdest[1];
			int roomNo = atoi(vStr[1].c_str());
			m_mapPointRoomInfo[itmpNo].point_name = vStr[0];
			m_mapPointRoomInfo[itmpNo].room_no = roomNo;
			m_mapPointRoomInfo[itmpNo].room_name = vStr[2];
			m_mapPointRoomInfo[itmpNo].x = atof(vStr[3].c_str());
			m_mapPointRoomInfo[itmpNo].y = atof(vStr[4].c_str());
			m_mapPointRoomInfo[itmpNo].z = atof(vStr[5].c_str());
			//
			m_mapRoomNoName[roomNo] = vStr[2];
		}
	}
	return TRUE;
}
//加载
BOOL CTcpCommSvrDlg::LoadPointInOutMapping()
{
	m_mapPointInOutInfo.clear();
	char stmp[256];
	char sdest[256];
	string filePath = ".\\cfg\\point.ini";
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("出入库激活器列表" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "总行数" || str_first == "总路数")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			memset(stmp, 0, sizeof(stmp));
			memset(sdest, 0, sizeof(sdest));
			memcpy(stmp, str_id.c_str(), 4);
			StrToHex(sdest, stmp, 2);
			int itmpNo = (unsigned char)sdest[0] * 256 + (unsigned char)sdest[1];
			m_mapPointInOutInfo[itmpNo].point_no = itmpNo;
			m_mapPointInOutInfo[itmpNo].io_flg = atoi(vStr[0].c_str());;
			m_mapPointInOutInfo[itmpNo].point_name = vStr[1];
		}
	}
	return TRUE;
}
BOOL CTcpCommSvrDlg::LoadDevInOutMapping()
{
	m_mapDevInOutInfo.clear();
	char stmp[256];
	char sdest[256];
	string filePath = ".\\cfg\\dev.ini";
	if (access(filePath.c_str(), 0) == -1)
	{
		return false;
	}
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("出入库读卡器列表" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "总行数" || str_first == "总路数")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			int itmpNo = atoi(str_id.c_str());
			m_mapDevInOutInfo[itmpNo].dev_no = itmpNo;
			m_mapDevInOutInfo[itmpNo].io_flg = atoi(vStr[0].c_str());;
			m_mapDevInOutInfo[itmpNo].dev_name = vStr[1];
		}
	}
	return TRUE;
}
//加载
BOOL CTcpCommSvrDlg::LoadPointCardBindMapping()
{
	m_mapPoindCardBindInfo.clear();
	char stmp[256];
	char sdest[256];
	string filePath = ".\\cfg\\point_card_bind.ini";
	if (access(filePath.c_str(), 0) == -1)
	{
		return false;
	}
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return FALSE;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("激活器卡片绑定列表" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "总行数" || str_first == "总路数")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 4)
			{
				continue;
			}
			memset(stmp, 0, sizeof(stmp));
			memset(sdest, 0, sizeof(sdest));
			memcpy(stmp, str_id.c_str(), 4);
			StrToHex(sdest, stmp, 2);
			int itmpNo = (unsigned char)sdest[0] * 256 + (unsigned char)sdest[1];
			unsigned int card_id = strtoul(vStr[0].c_str(),NULL,10);
			m_mapPoindCardBindInfo[itmpNo].card_no = itmpNo;
			m_mapPoindCardBindInfo[itmpNo].x = atoi(vStr[1].c_str());
			m_mapPoindCardBindInfo[itmpNo].y = atoi(vStr[2].c_str());
			m_mapPoindCardBindInfo[itmpNo].z = atoi(vStr[3].c_str());
		}
	}
	return TRUE;
}
//通过卡号查该卡下的激活器信息
BOOL CTcpCommSvrDlg::GetCardPointInfo(unsigned int cardNo, map<unsigned int, stuPointInfo> &mapPointInfo)
{
	//m_mapCardPointInfo
	map<unsigned int, map<unsigned int, stuPointInfo>>::iterator iter;
	iter = m_mapCardPointInfo.find(cardNo);
	if (iter != m_mapCardPointInfo.end())
	{
		mapPointInfo = iter->second;
	}
	return TRUE;
}
//查卡、房间信息
BOOL CTcpCommSvrDlg::GetCardRoomInfo(map< unsigned int, stuRoomInfo> &mapCardRoomInfo)
{
	//m_mapCardRoomInfo
	map< unsigned int, stuRoomInfo>::iterator iter;
	for (iter = m_mapCardRoomInfo.begin(); iter != m_mapCardRoomInfo.end(); iter++)
	{
		unsigned int cardNo = iter->first;
		mapCardRoomInfo[cardNo].room_no = iter->second.room_no;
		mapCardRoomInfo[cardNo].room_name = iter->second.room_name;
		mapCardRoomInfo[cardNo].lst_time = iter->second.lst_time;
	}
	return TRUE;
}
//通过房间号查该房间下的卡信息
BOOL CTcpCommSvrDlg::GetRoomCardInfo(unsigned int roomNo, map< unsigned int, stuRoomInfo> &mapCardRoomInfo)
{
	//m_mapCardRoomInfo
	map<unsigned int, stuCardInOutStatus>::iterator iter;
	for (iter = m_mapCardInOutStatus.begin(); iter != m_mapCardInOutStatus.end(); iter++)
	{
		unsigned int cardNo = iter->first;
		if (roomNo != iter->second.room_no) continue;
		mapCardRoomInfo[cardNo].room_no = iter->second.room_no;
		mapCardRoomInfo[cardNo].room_name = iter->second.room_name;
		mapCardRoomInfo[cardNo].lst_time = iter->second.lst_time;
	}
	return TRUE;
}
//通过激活器查该激活器下的卡信息
BOOL CTcpCommSvrDlg::GetPointCardInfo(unsigned int pointNo, map<unsigned int, stuPointInfo> &mapCardInfo)
{
	//m_mapPointCardInfo
	map < unsigned int, map<unsigned int, stuPointInfo>>::iterator iter;
	iter = m_mapPointCardInfo.find(pointNo);
	if (iter != m_mapPointCardInfo.end())
	{
		mapCardInfo = iter->second;
	}
	return TRUE;
}
/////////////
void CTcpCommSvrDlg::LoadCardFromIni()
{
    m_mapCardInOutStatus.clear();
	char stmp[256];
	char sdest[256];
	string filePath = ".\\cfg\\card.ini";
	TIniFile iniFile(filePath.c_str());
	TIniFile::SectionIterator pos_ini;
	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	size_t section_count = v_section.size();
	if (section_count <= 0)
	{
		return ;
	}
	for (size_t sec_idx = 0; sec_idx < section_count; sec_idx++)
	{
		std::string& str_mapping_type = v_section[sec_idx];
		if ("卡片信息" != str_mapping_type)continue;
		if (!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for (pos_ini; !pos_ini.IsDone(); pos_ini.Next())
		{
			string str_first = pos_ini.Data().first.c_str();
			if (str_first == "总行数" || str_first == "总路数")
				continue;

			string str_id = pos_ini.Data().first.c_str();
			string str_content = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_content.c_str(), ",", vStr);
			if (vStr.size() != 4)
			{
				continue;
			}
			unsigned int card_id = strtoul(str_id.c_str(),NULL,10);
			m_mapCardInOutStatus[card_id].card_no = card_id;
			m_mapCardInOutStatus[card_id].io_flg  = atoi(vStr[0].c_str());
			if(m_mapCardInOutStatus[card_id].io_flg > 2) m_mapCardInOutStatus[card_id].io_flg = 0;
			strcpy(stmp,vStr[2].c_str());
			if('-' == stmp[0])strcpy(stmp,"0");
			m_mapCardInOutStatus[card_id].pre_room_no = atoi(stmp);
			m_mapCardInOutStatus[card_id].room_no = atoi(stmp);
			m_mapCardInOutStatus[card_id].lst_time = StrToTime(vStr[3].c_str());
			m_mapCardInOutStatus[card_id].gdwzd = 0;
		}
	}
}
BOOL CTcpCommSvrDlg::LoadCardFromSSDb()
{
	//初始化卡片在库状态
	m_mapCardInOutStatus.clear();
    try
	{
		_variant_t vCardNo,vDateTime,vFlg,vRoom,vGdwzd;
		_RecordsetPtr m_pRecordset;
		m_pRecordset.CreateInstance(__uuidof(Recordset)); 
		m_pRecordset->CursorLocation = adUseClient;
		//m_pRecordset->Open(_bstr_t("select cardid,CONVERT(varchar(30), gxrq, 120) as rq,rkbz,gdwzd from DB_gdzcb"),_variant_t((IDispatch *)m_dbConnection,true),adOpenStatic,adLockReadOnly,adCmdText);
		m_pRecordset->Open(_bstr_t("select cardid,CONVERT(varchar(30), gxrq, 120) as rq,rkbz,azdd,gdwzd from DB_gdzcb"),_variant_t((IDispatch *)m_dbConnection,true),adOpenStatic,adLockReadOnly,adCmdText);
		while(!m_pRecordset->adoEOF)
		{
			vCardNo =  m_pRecordset->GetCollect("cardid");//
			vDateTime =  m_pRecordset->GetCollect("rq");
			vFlg =  m_pRecordset->GetCollect("rkbz");
			vRoom =  m_pRecordset->GetCollect("azdd");
			vGdwzd =  m_pRecordset->GetCollect("gdwzd");
			//
			char stmp[256];
			memset(stmp,0,sizeof(stmp));
			strcpy(stmp,(LPCTSTR)(_bstr_t)vCardNo);
			unsigned int iCardNo = strtoul(stmp,NULL,10);
			m_mapCardInOutStatus[iCardNo].card_no = iCardNo;
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vFlg.vt)
			{
				 m_mapCardInOutStatus[iCardNo].io_flg = 0;
			}
			else
			{
			    strcpy(stmp,(LPCTSTR)(_bstr_t)vFlg);
				m_mapCardInOutStatus[iCardNo].io_flg = atoi(stmp);
			}
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vRoom.vt)
			{
				m_mapCardInOutStatus[iCardNo].room_no = 0;
				m_mapCardInOutStatus[iCardNo].pre_room_no = 0;
			}
			else
			{
				strcpy(stmp,(LPCTSTR)(_bstr_t)vRoom);
				m_mapCardInOutStatus[iCardNo].room_no = atoi(stmp);
				m_mapCardInOutStatus[iCardNo].pre_room_no = atoi(stmp);
			}
			//m_mapCardInOutStatus[iCardNo].room_name = m_mapRoomNoName[dstNo];
			memset(stmp,0,sizeof(vDateTime));
			if(VT_NULL == vDateTime.vt)
			{
				m_mapCardInOutStatus[iCardNo].lst_time = time(NULL);
			}
			else
			{
			    strcpy(stmp,(LPCTSTR)(_bstr_t)vDateTime);
				m_mapCardInOutStatus[iCardNo].lst_time = StrToTime(stmp);
			}
			memset(stmp,0,sizeof(stmp));
			if(VT_NULL == vGdwzd.vt)
			{
				 m_mapCardInOutStatus[iCardNo].gdwzd = 0;
			}
			else
			{
			    strcpy(stmp,(LPCTSTR)(_bstr_t)vGdwzd);
				m_mapCardInOutStatus[iCardNo].gdwzd = atoi(stmp);
			}
			//
			m_pRecordset->MoveNext();///移到下一条记录
		}
		m_pRecordset->Close();
		//m_pRecordset->Release();
	}
	catch(_com_error e)
	{
		//
		MessageBox("sqlserver初始化卡片失败1！");
		SendMessage(WM_CLOSE);
		return FALSE;
	}
	return TRUE;
}
//
BOOL CTcpCommSvrDlg::LoadCardFromMSDb()
{
	//初始化卡片在库状态
	m_mapCardInOutStatus.clear();
 ////   try
	////{
	////	MYSQL_RES *m_myResult;
	////	MYSQL_ROW m_sqlRow;
	////	mysql_query(&m_myCont, "SET NAMES GBK"); //设置编码格式
 ////       int ret = mysql_query(&m_myCont, "select CARD_ID,IO_FLG,NOW_POS,CHG_DATE from hm_db.card_inf");//查询
 ////       if (0 == ret)
 ////       {
 ////           m_myResult = mysql_store_result(&m_myCont);
 ////           if (m_myResult)
 ////           {
 ////               while (m_sqlRow = mysql_fetch_row(m_myResult))//获取具体的数据
 ////               {
	////				unsigned int iCardNo = strtoul(m_sqlRow[0],NULL,10);
	////				m_mapCardInOutStatus[iCardNo].card_no = iCardNo;
	////				m_mapCardInOutStatus[iCardNo].io_flg = atoi(m_sqlRow[1]);
	////				m_mapCardInOutStatus[iCardNo].room_no = atoi(m_sqlRow[2]);
	////				m_mapCardInOutStatus[iCardNo].pre_room_no = atoi(m_sqlRow[2]);
	////				m_mapCardInOutStatus[iCardNo].lst_time = StrToTime(m_sqlRow[1]);
 ////               }
	////			mysql_free_result(m_myResult);
 ////           }
 ////       }
 ////       else
 ////       {
	////		MessageBox("mysql初始化卡片失败1！");
	////		SendMessage(WM_CLOSE);
	////		return FALSE;
 ////       }
	////}
	////catch(_com_error e)
	////{
	////	//
	////	MessageBox("mysql初始化卡片失败2！");
	////	SendMessage(WM_CLOSE);
	////	return FALSE;
	////}
	return TRUE;
}
//
bool CTcpCommSvrDlg::LoadAttendanceData()
{
	m_mapAttData.clear();
	char stmp[256];
	time_t time_now = time(NULL);
	struct tm tmTmp;
	tmTmp = *localtime(&time_now);
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,".\\data\\%d-%02d-%02d_att.ini",tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
    if (access(stmp, 0) == -1)
	{
		return false;
	}
	CString filePath = _T(stmp);
	TIniFile iniFile(filePath);
	TIniFile::SectionIterator pos_ini;

	std::vector<std::string> v_section;
	iniFile.GetIniSections(v_section);
	int section_count = v_section.size();
	if(section_count <= 0)
	{
		return FALSE;
	}
	for(int sec_idx = 0; sec_idx < section_count; sec_idx++)
	{	
		std::string& str_mapping_type = v_section[sec_idx];
		if("考勤数据" != str_mapping_type)continue; 
		if(!iniFile.GetIniSection(str_mapping_type.c_str(), pos_ini))
		{
			continue;
		}
		for(pos_ini;!pos_ini.IsDone();pos_ini.Next())
		{
			CString str_first = pos_ini.Data().first.c_str();
			if(str_first == "总行数" || str_first == "总路数" )
				continue;

			CString str_id = pos_ini.Data().first.c_str();
			CString str_dcqk = pos_ini.Data().second.c_str();
			std::vector<std::string> vStr;
			SpliteStr(str_dcqk, ",", vStr);
			if (vStr.size() != 2)
			{
				continue;
			}
			//
			unsigned int card_id = strtoul(str_id.GetBuffer(), NULL, 10);
			m_mapAttData[card_id].card_no = card_id;
			m_mapAttData[card_id].fst_time = strtoul(vStr[0].c_str(), NULL, 10);
			m_mapAttData[card_id].lst_time = strtoul(vStr[1].c_str(), NULL, 10);
		}
	}
	return true;
}
void CTcpCommSvrDlg::SetAttendanceData(unsigned int card_no,time_t lst_time)
{
	//map<unsigned int,stuAttendanceData> m_mapAttData;
	map<unsigned int,stuAttendanceData>::iterator iter_att;
	iter_att = m_mapAttData.find(card_no);
	if(m_mapAttData.end() == iter_att)
	{
		m_mapAttData[card_no].card_no = card_no;
		m_mapAttData[card_no].fst_time = lst_time;
        m_mapAttData[card_no].lst_time = lst_time;
	}
	else
	{
		iter_att->second.lst_time = lst_time;
	}
	//
	char tmpno[256];
	char tmpdat[256];
	memset(tmpno,0,sizeof(tmpno));
	memset(tmpdat,0,sizeof(tmpdat));
	sprintf(tmpno,"%010u",card_no);
	sprintf(tmpdat,"%u,%u",m_mapAttData[card_no].fst_time,m_mapAttData[card_no].lst_time);
	//
	char stmp[256];
	time_t time_now = time(NULL);
	struct tm tmTmp;
	tmTmp = *localtime(&time_now);
	memset(stmp,0,sizeof(stmp));
	sprintf(stmp,".\\data\\%d-%02d-%02d_att.ini",tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);
	WritePrivateProfileString("考勤数据",tmpno,tmpdat,stmp);
}
////
BOOL CTcpCommSvrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(TRUE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, TRUE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	m_pmDlgOk = false;
	//参数初始化
    m_hzDll = NULL;
	m_hzpShowDlg = NULL;
	m_hzpSetData = NULL;
	//
	char stmp[256];
	CString strPath(".\\cfg\\syscfg.ini");
	//m_stuStationPara
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("站点参数", "station_id","",stmp,256,strPath.GetBuffer());
	m_stuStationPara.id = stmp;
	GetPrivateProfileString("站点参数", "svr_id","",stmp,256,strPath.GetBuffer());
	m_stuStationPara.svr_id = stmp;
	GetPrivateProfileString("站点参数", "svr_ip","",stmp,256,strPath.GetBuffer());
	m_stuStationPara.svr_ip = stmp;
	m_stuStationPara.svr_port = GetPrivateProfileInt("站点参数", "svr_port",0,strPath.GetBuffer());
	m_stuStationPara.up_load = GetPrivateProfileInt("站点参数", "up_load",0,strPath.GetBuffer());
	m_stuStationPara.up_type = GetPrivateProfileInt("站点参数", "up_type",0,strPath.GetBuffer());
	//
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("参数配置", "db","",stmp,256,strPath.GetBuffer());
	m_dbStr = stmp;
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("参数配置", "ip","",stmp,256,strPath.GetBuffer());
	m_svrIp = stmp;
	m_svrPort = GetPrivateProfileInt("参数配置", "port",0,strPath.GetBuffer());
	m_freshTime = GetPrivateProfileInt("参数配置", "freshtime",0,strPath.GetBuffer());
	if(m_freshTime <= 0)
	{
		//3秒
        m_freshTime = 3;
	}
	else if(1 == m_freshTime)
	{
		m_freshTime = 5;
	}
	else if(2 == m_freshTime)
	{
	    m_freshTime = 10;
	}
	else if(3 == m_freshTime)
	{
	    m_freshTime = 30;
	}
	else
	{
        m_freshTime = (m_freshTime - 2)*60;
	}
	m_logGrade = GetPrivateProfileInt("参数配置", "loggrade",0,strPath.GetBuffer());
	m_protocolType = GetPrivateProfileInt("参数配置", "protocol_type",0,strPath.GetBuffer());
	if(m_protocolType <= 0)m_protocolType = 1;
	m_useDb = GetPrivateProfileInt("参数配置", "use_db",0,strPath.GetBuffer());
	if(m_useDb <= 0)m_useDb = 1;
	//
	m_runMode = GetPrivateProfileInt("参数配置", "run_mode",0,strPath.GetBuffer());
	if(m_runMode <= 0)m_runMode = 1;
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("参数配置", "remote_ip","",stmp,256,strPath.GetBuffer());
	m_remoteIp = stmp;
	m_remotePort = GetPrivateProfileInt("参数配置", "remote_port",0,strPath.GetBuffer());
	//
	memset(&m_mySqlCfg,0,sizeof(stuMySqlCfg));
	m_mySqlCfg.my_port = GetPrivateProfileInt("mysql配置", "my_port",0,strPath.GetBuffer());
	if(m_mySqlCfg.my_port <= 0)m_mySqlCfg.my_port = 3306;
	GetPrivateProfileString("mysql配置", "my_host","",m_mySqlCfg.my_host,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql配置", "my_db","",m_mySqlCfg.my_db,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql配置", "my_user","",m_mySqlCfg.my_user,256,strPath.GetBuffer());
	GetPrivateProfileString("mysql配置", "my_password","",m_mySqlCfg.my_password,256,strPath.GetBuffer());
	m_connType = GetPrivateProfileInt("连接类型配置", "conn_type",0,strPath.GetBuffer());
	//
	m_freshRatio = 2;
	memset(stmp,0,sizeof(stmp));
	GetPrivateProfileString("参数配置", "fresh_ratio","",stmp,256,strPath.GetBuffer());
	double dTmp = atof(stmp);
	if(dTmp > 1) m_freshRatio = dTmp;
	//
	m_bolLog = TRUE;
	m_bolData = TRUE;
	m_bolDb = TRUE;
	m_bolHelper = TRUE;
	m_mapReaderCardInfo.clear();
	m_vecInOutInfo.clear();
	//
	m_mapCardPointInfo.clear();
	m_mapPointCardInfo.clear();
	//
	LoadPointRoomMapping();
	LoadPointInOutMapping();
	LoadDevInOutMapping();
	LoadPointCardBindMapping();
	//
	LoadAttendanceData();
	//
	//m_svrIp = "127.0.0.1";
    //m_svrPort = 8888;
	//m_freshTime = 60;
	//
   ::CoInitialize(NULL);    
    //provider=Data Source=.;User ID=sa;Password=111111;database=FixedAssetsSystem
    m_dbConnection.CreateInstance(__uuidof(Connection));   
      
	_bstr_t strConnect = m_dbStr.GetBuffer(); 
	//_bstr_t strConnect = "provider=Data Source=.;User ID=sa;Password=casco_123;database=formal";

	////连接数据库
	if(2 == m_useDb)
	{
		try
		{
			HRESULT hr = NULL;
			hr = m_dbConnection->Open(strConnect,"","",adModeUnknown);
			if(S_OK != hr)
			{
				//数据库连接失败
				MessageBox("sqlserver数据库连接失败1！");
				SendMessage(WM_CLOSE);
				return FALSE;
			}
		}
		catch(_com_error e)
		{
			//数据库连接失败
			MessageBox("sqlserver数据库连接失败2！");
			SendMessage(WM_CLOSE);
			return FALSE;
		}
	}
	else if(3 == m_useDb)
	{
		////MYSQL *tmpRet = NULL;
		////tmpRet = mysql_init(&m_myCont);
		////if(NULL == tmpRet)
		////{
		////	MessageBox("mysql数据库连接失败1！");
		////	SendMessage(WM_CLOSE);
		////}
		////tmpRet = mysql_real_connect(&m_myCont, m_mySqlCfg.my_host, m_mySqlCfg.my_user, m_mySqlCfg.my_password, m_mySqlCfg.my_db, m_mySqlCfg.my_port, NULL, CLIENT_MULTI_STATEMENTS);
		////if(NULL == tmpRet)
		////{
		////	MessageBox("mysql数据库连接失败2！");
		////	SendMessage(WM_CLOSE);
		////}
	}
	
    DWORD dwStyle = m_commInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_commInfo.SetExtendedStyle(dwStyle);
	
	CRect rect;
	m_commInfo.GetClientRect(&rect); //获得当前客户区信息 
    m_commInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/12);
    m_commInfo.InsertColumn(1, _T("级别"), LVCFMT_LEFT, rect.Width()/12);
    m_commInfo.InsertColumn(2, _T("内容"), LVCFMT_LEFT, rect.Width()/2);
    m_commInfo.InsertColumn(3, _T("时间"), LVCFMT_LEFT, rect.Width()/3); 
	//m_commInfo.SetRedraw(FALSE);//防止重绘
    ////
	dwStyle = m_connInfo.GetExtendedStyle();     
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_connInfo.SetExtendedStyle(dwStyle);

	m_connInfo.GetClientRect(&rect); 
    m_connInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, rect.Width()/6);
    m_connInfo.InsertColumn(1, _T("设备编号"), LVCFMT_LEFT, rect.Width()/4);
	m_connInfo.InsertColumn(2, _T("IP地址"), LVCFMT_LEFT, rect.Width()*7/12);
	//m_connInfo.SetRedraw(FALSE);//
	//
	m_pDataQueue = new CDataQueue;
	m_pLogMsg    = new CLogMessage;
	//
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);//
	GetDlgItem(IDC_BTN_STOP_CONNECT)->EnableWindow(FALSE);//
	GetDlgItem(IDC_BTN_STOP_COM)->EnableWindow(FALSE);//
	if(1 == m_runMode)
	{
		GetDlgItem(IDC_BTN_START_CONNECT)->EnableWindow(FALSE);//
	}
	else if(2 == m_runMode)
	{
		GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);//
	}
	//
	if(1 == m_useDb)
	{
		LoadCardFromIni();
	}
	else if(2 == m_useDb)
	{
		BOOL bol = LoadCardFromSSDb();
		if(FALSE == bol)return FALSE;
	}
	else if(3 == m_useDb)
	{
		BOOL bol = LoadCardFromMSDb();
		if(FALSE == bol)return FALSE;
	}
	if(2 == m_protocolType)
	{
		//
		GetDlgItem(IDC_BTN_TMD_START)->EnableWindow(FALSE);//
		//GetDlgItem(IDC_BTN_READER_QRY)->EnableWindow(FALSE);//
		GetDlgItem(IDC_BTN_CARD_QRY)->EnableWindow(FALSE);//
		GetDlgItem(IDC_BTN_PARA_MNG)->EnableWindow(FALSE);//
	}
	else
	{
	    GetDlgItem(IDC_BTN_INFO_QRY)->EnableWindow(FALSE);//
	}
	//map<unsigned int,vector<stuReaderInfo>> m_mapCardReaderInfoVec;
		//测试数据
	////SetCardToDisplayData(1755605117,1,time(NULL));
	////stuReaderInfo stuTmp;
	////stuTmp.fstTime = time(NULL);
	////stuTmp.lstTime = time(NULL) + m_freshTime;
	////stuTmp.readNo = 1;
	////stuTmp.readCount = 10;
	////m_mapCardReaderInfoVec[1755605117].push_back(stuTmp);
	////m_mapCardReaderInfoVec[2387869110].push_back(stuTmp);
	////m_mapCardReaderInfoVec[2387869111].push_back(stuTmp);
	////m_mapCardReaderInfoVec[2387869112].push_back(stuTmp);
	////SetCardToDisplayData(19260306,2,time(NULL));
	////SetCardToDisplayData(3254563195,3,time(NULL));
	////SetCardToDisplayData(2387869110,1,time(NULL));
	//////
	////SetCardToDisplayData(2387869111,1,time(NULL));
	////SetCardToDisplayData(2387869112,1,time(NULL));
	////SetCardToDisplayData(2387869113,1,time(NULL));
	////SetCardToDisplayData(2387869114,1,time(NULL));
	////SetCardToDisplayData(2387869115,1,time(NULL));
	////SetCardToDisplayData(2387869116,1,time(NULL));
	////SetCardToDisplayData(2387869117,1,time(NULL));
	////SetCardToDisplayData(2387869118,1,time(NULL));
	////SetCardToDisplayData(2387869119,1,time(NULL));
	////SetCardToDisplayData(2387869120,1,time(NULL));
 ////   //
	////SetCardToDisplayData(1755605127,4,time(NULL));
	////SetCardToDisplayData(19260316,5,time(NULL));
	////SetCardToDisplayData(3254563185,6,time(NULL));
	////SetCardToDisplayData(2387869120,4,time(NULL));
	//////
	////SetCardToDisplayData(2387869121,4,time(NULL));
	////SetCardToDisplayData(2387869122,4,time(NULL));
	////SetCardToDisplayData(2387869123,4,time(NULL));
	////SetCardToDisplayData(2387869124,4,time(NULL));
	////SetCardToDisplayData(2387869125,4,time(NULL));
	////SetCardToDisplayData(2387869126,4,time(NULL));
	////SetCardToDisplayData(2387869127,4,time(NULL));
	////SetCardToDisplayData(2387869128,4,time(NULL));
	////SetCardToDisplayData(2387869129,4,time(NULL));
	////SetCardToDisplayData(2387869130,4,time(NULL));
	//////
	m_hConnFreshEvent = CreateEvent(NULL,FALSE,FALSE,NULL);//
	//日志处理线程
	m_logThread = AfxBeginThread(logProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_logThread->m_bAutoDelete = TRUE;
	m_logThread->ResumeThread();
	//数据处理线程
	m_dataThread = AfxBeginThread(dataProcess,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_dataThread->m_bAutoDelete = TRUE;
	m_dataThread->ResumeThread();
	//更新数据库线程
	if(2 == m_protocolType)
	{
		m_dbThread = AfxBeginThread(dbFresh_New,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
		m_dbThread->m_bAutoDelete = TRUE;
		m_dbThread->ResumeThread();
	}
	else
	{
		m_dbThread = AfxBeginThread(dbFresh,(LPVOID)this,
		THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
		m_dbThread->m_bAutoDelete = TRUE;
		m_dbThread->ResumeThread();
	}
	//辅助线程
	m_helperThread = AfxBeginThread(helperThread,(LPVOID)this,
	THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_helperThread->m_bAutoDelete = TRUE;
	m_helperThread->ResumeThread();
	//
	if(1 == m_stuStationPara.up_load)
	{
		BOOL bol = m_netTcpClient.StartTcpClient(m_stuStationPara.svr_id.c_str(),m_stuStationPara.id.c_str(),m_stuStationPara.svr_ip.c_str(),m_stuStationPara.svr_port,m_pLogMsg);
		if(FALSE == bol)
		{
			MessageBox("TCP客户端启动失败！");
		}
		if(0 ==  m_stuStationPara.up_type)
		{
		    SetTimer(1, 1000, NULL);
		}
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTcpCommSvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTcpCommSvrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
BOOL CTcpCommSvrDlg::DestroyWindow() 
{
	try
	{
		m_bolLog = FALSE;
		m_bolData = FALSE;
		m_bolDb = FALSE;
		m_bolHelper = FALSE;
		SetEvent(m_hConnFreshEvent);
		//
		m_netTcpSvr.StopTcpServer();
		m_netTcpClient.StopTcpClient();
		//
		Sleep(3000);
		//
		if(m_pDataQueue != NULL)
		{
			m_pDataQueue->RemoveAll();
			delete m_pDataQueue;
			m_pDataQueue = NULL;
		}
		//
		if(m_pLogMsg != NULL)
		{
			delete m_pLogMsg;
			m_pLogMsg = NULL;
		}
	}
	catch(_com_error e)
	{
	}
	//
	try
	{
		if(2 == m_useDb)
		{
			m_dbConnection->Close();
			::CoUninitialize();
		}
		else if(3 == m_useDb)
		{
			////mysql_close (&m_myCont);
		}
	}
	catch(_com_error e)
	{
	}
	//
	return CDialog::DestroyWindow();
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTcpCommSvrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTcpCommSvrDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bol = m_netTcpSvr.StartTcpServer(m_svrIp.GetBuffer(),m_svrPort,m_pDataQueue,m_pLogMsg,&m_hConnFreshEvent,m_connType);
	if(FALSE == bol)
	{
		MessageBox("TCP服务器启动失败！");
		return ;
	}
	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);//
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);//
	//
}

void CTcpCommSvrDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_netTcpSvr.StopTcpServer();
    GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);//
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);//
}

void CTcpCommSvrDlg::OnBnClickedBtnPara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlgPara.DoModal();
	m_svrIp = m_dlgPara.m_svrIp;
	m_svrPort = m_dlgPara.m_svrPort;
	m_freshTime = m_dlgPara.m_freshTime;
	if(m_freshTime <= 0)
	{
		//5秒
        m_freshTime = 5;
	}
	else if(1 == m_freshTime)
	{
		m_freshTime = 10;
	}
	else if(2 == m_freshTime)
	{
	    m_freshTime = 30;
	}
	else
	{
        m_freshTime = (m_freshTime - 2)*60;
	}
	m_logGrade = m_dlgPara.m_logGrade;
}

void CTcpCommSvrDlg::OnBnClickedBtnReaderQry()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMainDlg = this;
	m_dlgReaderQry.DoModal();
}

void CTcpCommSvrDlg::OnBnClickedBtnCardQry()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMainDlg = this;
	m_dlgCardQry.DoModal();
}

void CTcpCommSvrDlg::OnBnClickedBtnTmdStart()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMainDlg = this;
	g_pMainDlg->ShowWindow(SW_HIDE);
	if(NULL == m_hzDll)
	{
		//1.加载动态库
		m_hzDll = LoadLibrary(_T("MonitorDisplay.dll"));
		if (NULL == m_hzDll)
		{
			MessageBox("加载 MyDLL.dll 失败");
		}
		//2.获取导出函数指针
		m_hzpShowDlg = (hzpDllShowDlg)GetProcAddress(m_hzDll,"ShowDialog");
		if (NULL==m_hzpShowDlg)
		{
			MessageBox("DLL中函数寻找失败1");
			//return;
		}
		m_hzpSetData = (hzpDllSetData)GetProcAddress(m_hzDll,"SetData");
		if (NULL==m_hzpSetData)
		{
			MessageBox("DLL中函数寻找失败2");
			//return;
		}
		//
	}
	if(m_hzpShowDlg != NULL)
	{
	    m_hzpShowDlg("000");
	}
	g_pMainDlg->ShowWindow(SW_SHOW);
	////m_dlgTblTmd.SetFreshTime(2*m_freshTime);
	////m_dlgTblTmd.DoModal();
	//if(false == m_pmDlgOk)
	//{
	//	m_dlgTblTmd.Create(IDD_DLG_DISPLAY_TBL);  
	//}
	//else
	//{
	//	m_dlgTblTmd.ShowWindow(SW_SHOW);
	//	
	//}
}

void CTcpCommSvrDlg::OnBnClickedBtnParaMng()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pmDlg.Create(IDD_DLG_PARA_MNG);
	if(g_userGrade > 0)
	{
		CString strTmp("只有管理员才有权限设置参数！");
		MessageBox(strTmp);
		return;
	}
	m_pmDlg.DoModal();
}

BOOL CTcpCommSvrDlg::PreTranslateMessage(MSG* pMsg)
{
	return FALSE;
	//return CDialog::PreTranslateMessage(pMsg);
}
void CTcpCommSvrDlg::OnBnClickedBtnInfoQry()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMainDlg = this;
	CDlgNewDataQry dlg;
	dlg.DoModal();
}

void CTcpCommSvrDlg::OnBnClickedBtnStartConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	//BOOL bol = m_netTcpClient.StartTcpClient(m_remoteIp.GetBuffer(),m_remotePort,m_pLogMsg);
	//if(FALSE == bol)
	//{
	//	MessageBox("TCP客户端启动失败！");
	//	return ;
	//}
	GetDlgItem(IDC_BTN_START_CONNECT)->EnableWindow(FALSE);//
	GetDlgItem(IDC_BTN_STOP_CONNECT)->EnableWindow(TRUE);//
}

void CTcpCommSvrDlg::OnBnClickedBtnStopConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_netTcpClient.StopTcpClient();
	GetDlgItem(IDC_BTN_START_CONNECT)->EnableWindow(TRUE);//
	GetDlgItem(IDC_BTN_STOP_CONNECT)->EnableWindow(FALSE);//
}

void CTcpCommSvrDlg::OnBnClickedBtnStartCom()
{
	// TODO: 在此添加控件通知处理程序代码
    //启动串口通讯
	char tmpSerial[16];
	memset(tmpSerial,0,sizeof(tmpSerial));
	CString strPath(".\\cfg\\syscfg.ini");
	std::string strValue;
	if (access(strPath, 0) == -1)
	{
		memcpy(tmpSerial,"COM1",4);
	}
	else
	{
		TIniFile iniFile(strPath);
		if (!iniFile.GetIniSetting("串口参数配置", "serial", strValue))
		{
			memcpy(tmpSerial,"COM1",4);
		}
		else
		{
			memcpy(tmpSerial,strValue.c_str(),strValue.length());
		}
	}
    bool bol = m_serialTrans.initSerial(tmpSerial,m_pDataQueue,m_pLogMsg,m_connType);
	if(!bol)
	{
		CString strTmp;
		strTmp.Format("串口通讯模块启动失败[%s]",tmpSerial);
		//MessageBox(strTmp);
		return;
	}
	GetDlgItem(IDC_BTN_START_COM)->EnableWindow(FALSE);//
	GetDlgItem(IDC_BTN_STOP_COM)->EnableWindow(TRUE);//
}

void CTcpCommSvrDlg::OnBnClickedBtnStopCom()
{
	// TODO: 在此添加控件通知处理程序代码
	m_serialTrans.SetThreadStatus(false);
	GetDlgItem(IDC_BTN_START_COM)->EnableWindow(TRUE);//
	GetDlgItem(IDC_BTN_STOP_COM)->EnableWindow(FALSE);//
}

void CTcpCommSvrDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
  	if(1 == nIDEvent)
	{
		//上传卡片数据
		//map<unsigned int, stuCardInOutStatus> m_mapCardInOutStatus;
		map<unsigned int, stuCardInOutStatus>::iterator iter_c;
		for(iter_c = m_mapCardInOutStatus.begin(); iter_c != m_mapCardInOutStatus.end(); iter_c++)
		{
			m_netTcpClient.SetSendCardData(&iter_c->second);
		}
		//上传考勤数据
		//map<unsigned int,stuAttendanceData> m_mapAttData;
		map<unsigned int,stuAttendanceData>::iterator iter_a;
		for(iter_a = m_mapAttData.begin(); iter_a != m_mapAttData.end(); iter_a++)
		{
			m_netTcpClient.SetSendAttData(&iter_a->second);
		}
		
	}  
	CDialog::OnTimer(nIDEvent);
}
