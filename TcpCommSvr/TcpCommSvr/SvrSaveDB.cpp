#include "StdAfx.h"
#include "SvrSaveDB.h"

CSvrSaveDB::CSvrSaveDB(void)
{
	m_sqlobj_store = NULL;
	m_sqlObj_assist = NULL;
	m_sqlObj_inquire = NULL;

	m_db_conn_state = true;
	m_db_disc_time  = -1;
}

CSvrSaveDB::~CSvrSaveDB(void)
{
	DestroyDBInfo();
}

void CSvrSaveDB::DestroyDBInfo()
{
	if(m_sqlobj_store != NULL)
	{
		delete m_sqlobj_store;
	}
	if(m_sqlObj_inquire != NULL)
	{
		delete m_sqlObj_inquire;
	}
	if(m_sqlObj_assist != NULL)
	{
		delete m_sqlObj_assist;
	}
}

BOOL CSvrSaveDB::InitDBInfo()
{

	CString strPrompt;

	strPrompt = "";
	CSqlHelper::DB_CFG dbCfg;
	dbCfg.m_bShuangJiReBei		= FALSE;
	dbCfg.m_strMasterIP			= "127.0.0.1";
	dbCfg.m_strMasterConnectStr = "";
	dbCfg.m_strSlaveIP			= "127.0.0.1";
	dbCfg.m_strSlaveConnectStr	= "";
	dbCfg.m_strCurConnectStr	= "";
	dbCfg.m_pfnDBChange			= FALSE;

	//
	//dbCfg.m_pfnConnStateCallBack	= DBConnStateCallBack;

	// �洢��Sql����
	if ((m_sqlobj_store = CSqlHelper::NewUser(&dbCfg)) == NULL)
	{
		//TRAP_HERE_AND_DISPLAY("CServerSaveDBģ������ʧ�ܣ����ܻ��m_sqlobj_kgl���������������У�", LOG_BOOT, TRUE);
		return FALSE;
	}

	// ��ѯ��Sql
	dbCfg.m_strCurConnectStr = "";
	if((m_sqlObj_inquire = CSqlHelper::NewUser(&dbCfg)) == NULL)
	{
		//TRAP_HERE_AND_DISPLAY("CServerSaveDBģ������ʧ�ܣ����ܻ��m_sqlObj_inquire���������������У�", LOG_BOOT, TRUE);
		return FALSE;
	}

	//����m_sqlObj_assist�������ڼ�����ݿ�״̬
	dbCfg.m_strCurConnectStr = "";
	if ((m_sqlObj_assist = CSqlHelper::NewUser(&dbCfg)) == NULL)
	{
		//TRAP_HERE_AND_DISPLAY("CServerSaveDBģ������ʧ�ܣ����ܻ��m_sqlObj_assist���������������У�", LOG_BOOT, TRUE);
		return FALSE;
	}

	return TRUE;
}