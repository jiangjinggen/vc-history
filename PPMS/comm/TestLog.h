// TestLog.h: interface for the CTestLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTLOG_H__FDEF56A4_35F1_47E8_9CCD_A65FE599AD9F__INCLUDED_)
#define AFX_TESTLOG_H__FDEF56A4_35F1_47E8_9CCD_A65FE599AD9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <io.h>
#include <stdio.h>
#include <afxmt.h>
#include <ctime>
#include <string>
using namespace std;

#define LOG_MAIN    "_MAIN"
#define LOG_COMM    "_COMM"
#define LOG_DATA    "_DATA"
#define LOG_FRESH   "_FRESH"

class CTestLog  
{
public:
	CTestLog();
	virtual ~CTestLog();
public:
	/**
    * @brief �ֽڴ���ʮ������������ļ�����ͬʱʱ���ʱ����ֽڴ�����
	* @param srcPath����־�ļ�·��  
	* @param srcName����־�ļ���
	* @param dataBuf���ֽڴ�
	* @param dataSize���ֽڴ�����
	* @return �ɹ�����0�����򷵻�-1
	*/
	static int LogDebugInfo(const string srcPath,const string srcName,const unsigned char *dataBuf,int dataSize)
	{
		if(dataSize > 1024) return -1;
		CTime curTime = CTime::GetCurrentTime();
		string sdate = curTime.Format("%Y-%m-%d");
		string spath = srcPath + sdate;
		spath = spath + "_";
		spath = spath + srcName;
		FILE *fp = fopen(spath.c_str(),"a");
		if(NULL != fp)
		{
			char stmp[4096];
			CTime curTime = CTime::GetCurrentTime();
			string stime = curTime.Format("%Y-%m-%d %H:%M:%S");
			fputs("time:",fp);
			fputs(stime.c_str(),fp);
			fputs("\n",fp);

			fputs("length:",fp);			
			memset(stmp,0,sizeof(stmp));
			sprintf(stmp,"%d",dataSize);
			fputs(stmp,fp);
			fputs("\n",fp);
			fputs("value:\n",fp);
			memset(stmp,0,sizeof(stmp));
			char *ps = stmp;
			for(int i = 0;i < dataSize; i++)
			{
				sprintf(ps,"%02x ",unsigned char(dataBuf[i]));
				ps += 3;
			}
			fputs(stmp,fp);
			fputs("\n",fp);
			fclose(fp);
		}
		return 0;
	}
    /**
    * @brief �ֽڴ���ʮ������������ļ�����ͬʱʱ���ʱ����ֽڴ�����
	* @param srcPath����־�ļ�·��  
	* @param srcName����־�ļ���
	* @param dataBuf���ֽڴ�
	* @param dataSize���ֽڴ�����
	* @return �ɹ�����0�����򷵻�-1
	*/
	static int LogRunInfo(const char *logdata,const char *logtype,int loglevel)
	{
		if((NULL == logdata)||(NULL == logtype))return -1;
		if(-1 == loglevel) return -1;
		DWORD pid = GetCurrentThreadId(); 
		char stmp[20];
		memset(stmp,0,sizeof(stmp));
		sprintf(stmp,"_%d",pid);
		CTime curTime = CTime::GetCurrentTime();
		string sdate = curTime.Format("%Y-%m-%d");
		string stime = curTime.Format("%Y-%m-%d %H:%M:%S");
		string spath = ".\\log\\" + sdate + logtype + stmp + ".log";
		FILE *fp = fopen(spath.c_str(),"a");
		if(NULL != fp)
		{
			fputs("ʱ��:",fp);
			fputs(stime.c_str(),fp);
			fputs("\n",fp);

			fputs(logdata,fp);
			fputs("\n",fp);
			fclose(fp);
		}
		return 0;
	}
	/**
    * @brief �ֽڴ���ʮ������������ļ�����ͬʱʱ���ʱ����ֽڴ�����
	* @param srcPath����־�ļ�·��  
	* @param srcName����־�ļ���
	* @param dataBuf���ֽڴ�
	* @param dataSize���ֽڴ�����
	* @return �ɹ�����0�����򷵻�-1
	*/
	static int LogRunInfo1(const char *logdata,const char *logtype,int loglevel)
	{
		if((NULL == logdata)||(NULL == logtype))return -1;
		CTime curTime = CTime::GetCurrentTime();
		string sdate = curTime.Format("%Y-%m-%d");
		string stime = curTime.Format("%Y-%m-%d %H:%M:%S");
		string spath = ".\\log\\" + sdate + logtype + ".log";
		FILE *fp = fopen(spath.c_str(),"a");
		if(NULL != fp)
		{
			fputs("ʱ��:",fp);
			fputs(stime.c_str(),fp);
			fputs("\n",fp);

			fputs(logdata,fp);
			fputs("\n",fp);
			fclose(fp);
		}
		return 0;
	}
};

#endif // !defined(AFX_TESTLOG_H__FDEF56A4_35F1_47E8_9CCD_A65FE599AD9F__INCLUDED_)
