// DataQueue.h: interface for the CDataQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAQUEUE_H__7FA26676_C452_432B_A223_2E377660BF06__INCLUDED_)
#define AFX_DATAQUEUE_H__7FA26676_C452_432B_A223_2E377660BF06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxmt.h"

class CDataQueue {
public:
	CPtrList DataPtrList;
	BYTE *   QueueBuf;
	int      QueueNum;
	long     DataLen;
	//HANDLE   HMutex;
	CCriticalSection m_cs;// by zsm 20111101 for ÐÔÄÜ
	
public: 
	CDataQueue();		  
	~CDataQueue();		  
	BOOL InsertQueue( BYTE * DataBuf, int DataSize );		   
	BOOL ReadQueue( BYTE * DataBuf, int * DataSize );
	void RemoveAll();
	          
	inline BOOL EmptyQueue() {return DataPtrList.IsEmpty();}
	inline long GetDataSizeSum(){return DataLen;}
	inline long GetDataRecordSum(){return QueueNum;}
};

#endif // !defined(AFX_DATAQUEUE_H__7FA26676_C452_432B_A223_2E377660BF06__INCLUDED_)
