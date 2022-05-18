#ifndef LOG_MESSAGE_H_
#define LOG_MESSAGE_H_
#include <string>
#include <list>
using namespace std;
//日志类型
#define TYPE_LOG_TCP    1    //TCP服务器模块日志
#define TYPE_LOG_DAT    2    //数据处理模块日志
#define TYPE_LOG_DB     3    //数据库操作模块日志
//日志等级
#define GRADE_LOG_COMM   1    //普通
#define GRADE_LOG_WARN   2    //警告
#define GRADE_LOG_ERROR  3    //错误
//日志结构
typedef struct log_stu
{
	int item;
	int type;
	int grade;
	time_t datetime;
	string title;	
	string body;
}stuLogContent;
//
class CLogMessage  
{
private:
    std::list<stuLogContent> m_logmessage;    //日志内容 
    CRITICAL_SECTION m_csloglist;                  //
public:
	CLogMessage();
	virtual ~CLogMessage();
	bool AddLogMessage(stuLogContent msg);
	bool GetLogMessage(stuLogContent *pmsg);
};
#endif