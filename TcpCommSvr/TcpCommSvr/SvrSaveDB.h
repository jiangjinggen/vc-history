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
	CSqlObject* m_sqlobj_store;		// �洢��Sql����
	CSqlObject* m_sqlObj_inquire;	// ��ѯ��Sql����
	CSqlObject* m_sqlObj_assist;	// ����Sql������Ҫ������ݿ�ʱʹ��
	bool			m_db_conn_state;	// ���ݿ�����״̬
	time_t			m_db_disc_time;		// ���ݿ�ǰ���жϷ�����ʱ��
};
