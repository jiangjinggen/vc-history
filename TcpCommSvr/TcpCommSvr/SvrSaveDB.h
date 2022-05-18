#pragma once
#include "./dal/SqlHelper.h"
class CSvrSaveDB
{
public:
	CSvrSaveDB(void);
public:
	~CSvrSaveDB(void);
public:
	void DestroyDBInfo();
	BOOL    InitDBInfo();
public:
	CSqlObject* m_sqlobj_store;		// 存储用Sql对象
	CSqlObject* m_sqlObj_inquire;	// 查询用Sql对象
	CSqlObject* m_sqlObj_assist;	// 辅助Sql对象，需要检测数据库时使用
	bool			m_db_conn_state;	// 数据库连接状态
	time_t			m_db_disc_time;		// 数据库前次中断发生的时间
};
