#ifndef LOG_H
#define LOG_H

// [LiuXiaofeng 2013-08-08]:修改DBLogger机制，日志输出由外部实现，保持DAL层独立性

class CDBLogger 
{
public:
	CDBLogger()
	{

	};

	~CDBLogger()
	{
		m_v_outputer.clear();
	};

	class InfoLogger
	{
	public:
		virtual ~InfoLogger(){};
		virtual void OutputLog(const char* lpzsLogContent) = 0;
	};

	static CDBLogger* Ins();
	void AppendLogger(InfoLogger* pLogger);
	void Log(const char* pszInfoFormat, ...);

private:
	std::vector<InfoLogger*>	m_v_outputer;
};

/************************************************************************/
/* 函数实现 
/************************************************************************/
inline CDBLogger* CDBLogger::Ins()
{
	static CDBLogger instance;
	return &instance;
}

inline void CDBLogger::AppendLogger(InfoLogger* pLogger)
{
	ASSERT(pLogger!=NULL);
	if(pLogger==NULL)
		return;

	m_v_outputer.push_back(pLogger);
}

inline void CDBLogger::Log(const char* pszInfoFormat, ...)
{
	static char log_buffer[2048];
	va_list	args = NULL;
	va_start(args, pszInfoFormat);	
	log_buffer[0] = '\0';
	_vsnprintf_s(log_buffer,2048, pszInfoFormat, args);
	va_end(args);

	std::vector<InfoLogger*>::iterator pos;
	for(pos = m_v_outputer.begin(); pos != m_v_outputer.end(); ++pos)
	{
		InfoLogger* pLogger = *pos;
		ASSERT(pLogger != NULL);
		pLogger->OutputLog(log_buffer);
	}
}

#endif