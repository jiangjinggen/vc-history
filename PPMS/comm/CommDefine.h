#ifndef __COMM_DEFINE_H__
#define __COMM_DEFINE_H__
#include "IniFile.h"
#include "mystring.h"
#include "DataQueue.h"
#include "LogMessage.h"
#include "TestLog.h"
#include <vector>
#include <list>
#include <map>
using namespace std;
//
#pragma warning(disable:4996) //ȫ���ص�
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
	time_t last_time;   //���һ���յ����ݵ�ʱ��
	BOOL   isUpdateReader;
};
//
////
#define THREAD_SHUTDOWN_TIME	    500
#define DATA_FRAME_LEN    2048
//#define HEAD_LEN   4
#define HEAD_LEN   3
#define HEAD_LEN_2A   6
#define RECV_0_COUNT  10
#define READ_MAX_LEN  1024
#define SERIAL_BUFFER_SIZE 256*256*10
#define COMM_TIME_OUT 1000
#define PRO_CARD_NUM  100    //�������ݿ�ʱ��һ�δ���100�ſ�
#define BASE_FRAME_LEN  13    //1+2++1+1+6+1+1
//
enum NET_TYPE
{
	IS_NET = 1,
	IS_OTH = 2,
};
//
enum DIS_TYPE
{
	IS_COMM = 1,
	IS_HZSD = 2,
};
//
enum DB_TYPE
{
	NO_DB = 1,
	SS_DB = 2,
	MS_DB = 3,
	ORA_DB = 4,
};
//
enum DATE_TYPE
{
	HEART_BEAT = 0x01,
	CARD_DATA = 0x02,
	ATT_DATA = 0x03,
	CFG_DATA = 0x04,
	NET_STATUS = 0x05,
};
//
enum STATUS_IN_OUT
{
	STATUS_IN = 1,
	STATUS_OUT = 2,
};
//
struct stuRecvBufError
{
	int len;
	unsigned char recvBuf[READ_MAX_LEN+1];
};
//������Ϣ
struct stuCardInfo
{
	//int           cardNo;
	int           readCount;       //�������Ĵ���
	time_t        fstTime;         //��һ�ζ�������ʱ��(��ԣ�
	time_t        lstTime;         //���һ�ζ�������ʱ��
	double        temperature;
	double        voltage;
	double        current;
	unsigned char data_len;
	unsigned char data[32];
};
//
struct stuReaderInfo
{
	int           readNo;
	int           readCount;       //�������Ĵ���
	time_t        fstTime;         //��һ�ζ�������ʱ��(��ԣ�
	time_t        lstTime;         //���һ�ζ�������ʱ��
};
//
struct stuInOutInfo
{
	char sitem[10];
	char card_no[20];
	char io_time[20];
	char sflag[3];
};
//Э�����ͣ�1����Э�飬2����Э��
const int g_protocol_type  = 2;
//��Э��洢���ݵ���������
//
struct PosInfo
{
	float x;
	float y;
	float z;
	float d;
};
//��������Ϣ
struct stuPointInfo
{
	unsigned int point_signal_x;   //�ź�ǿ��x
	unsigned int point_signal_y;   //�ź�ǿ��y
    unsigned int point_signal_z;   //�ź�ǿ��z
	unsigned int status;         //���״̬
	time_t lst_time;    //���һ���յ���Ϣ��ʱ��
};
//������������Ϣ
struct stuPointRoomInfo
{
	string point_name;
	unsigned int room_no;
	string room_name;
	float  x;
	float  y;
	float  z;
};
//
struct stuRoomInfo
{
	unsigned int pre_room_no;
	unsigned int room_no;
	string room_name;
	time_t lst_time;
};
//
struct stuCardInOutStatus
{
	unsigned int card_no;
	unsigned char io_flg;
	unsigned char gdwzd;
	unsigned int pre_room_no;
	unsigned int room_no;
	string room_name;
	time_t lst_time;
	time_t snd_time;
};
//��������Ϣ
struct stuInOutPointInfo
{
	unsigned int point_no;
	string point_name;
	unsigned char io_flg;
};
//��������Ϣ
struct stuInOutDevInfo
{
	unsigned int dev_no;
	string dev_name;
	unsigned char io_flg;
};
//mysql���ò���
struct stuMySqlCfg
{
	unsigned int my_port;
	char my_host[32];
	char my_db[32];
	char my_user[32];
	char my_password[32];
};
//HTTP����������
struct stuHttpSvrCfg
{
	unsigned int http_port;
	char http_host[32];
	char http_url[256];
	char http_method[16];
};
struct stuHttpsSvrCfg
{
	unsigned int https_port;
	char https_host[32];
	char https_url[256];
	char https_method[16];
	char login_account[32];
	char login_pass[64];
};
//�����Ϣ
struct stuWatchInfo
{
	double d_x;
	double d_y;
	double d_z;
	double d_temperature;
	int i_rate;
	char status[32];
};
//
struct stuStationPara
{
	string id;
	string svr_id;
	string svr_ip;
	int svr_port;
	int up_load;
	int up_type;
};
//
struct stuAttendanceData
{
	unsigned int card_no;
	time_t fst_time;
	time_t lst_time;
	time_t snd_time;
};
//
struct stuPointCardBind
{
	unsigned int card_no;
	unsigned char x;
	unsigned char y;
	unsigned char z;
};
//������Ϣ
struct stuElderInfo
{
	char org_id[19];
	char elder_id[19];
	char dev_id[11];
	char card_id[19];
	char lbl_id[19];
	double x;
	double y;
	double z;
	char gesture[4];
	double temperature;
	int heart_rate;
    int battery;
	int hand;
	double longitude;
	double latitude;
	double altitude;
	char  up_flg[4];
	char  check_time[21];
    char  detail_info[256];
};
//����λ����Ϣ
struct stuElderPosInfo
{
	char m_name[19];
	char org_id[19];
	char elder_id[19];
	char dev_id[11];
	char card_id[19];
	double longitude;
	double latitude;
	double rate;
	double angle;
	char  up_flg[4];
	char  check_time[21];
};
#endif