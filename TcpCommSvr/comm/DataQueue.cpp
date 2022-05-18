// DataQueue.cpp: implementation of the CDataQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//以下dataqueue的实现部分
 CDataQueue::CDataQueue()
{
	QueueBuf = NULL;
	QueueNum = 0;
	DataLen  = 0;
}

 CDataQueue::~CDataQueue()
{              			  	
	RemoveAll();            
}	       
BOOL CDataQueue::InsertQueue( BYTE * DataBuf, int DataSize )
{
	if( DataBuf == NULL ) return TRUE;

	if(DataSize<0)
	{
		//ASSERT(0);  // 如果有这种情况进来，肯定是有问题 [9/10/2012 zsm]
		return TRUE;
	}
				
	m_cs.Lock();
				
	QueueBuf = new BYTE[DataSize+sizeof(int)];
	if( QueueBuf != NULL ) {
	memmove( &QueueBuf[0], &DataSize, sizeof(int));
	memmove( &QueueBuf[sizeof(int)], DataBuf, DataSize );
	DataPtrList.AddTail( QueueBuf );
	QueueNum++;
	DataLen += DataSize;
	}				
	m_cs.Unlock();			
	return TRUE;	
}

BOOL CDataQueue::ReadQueue( BYTE * DataBuf, int * DataSize )
{
	*DataSize = 0;
	if( DataBuf == NULL ) return TRUE;    
	
	m_cs.Lock();
	if( !DataPtrList.IsEmpty() ) {
		QueueBuf=NULL;
		QueueBuf = (BYTE *)DataPtrList.RemoveHead();
		if( QueueBuf != NULL ) {
			memmove( DataSize, &QueueBuf[0], sizeof(int));
			memmove( DataBuf,  &QueueBuf[sizeof(int)], *DataSize );
			QueueNum--;
			DataLen -= *DataSize;			
			delete []QueueBuf;
			QueueBuf=NULL;
		}		
		m_cs.Unlock();
		return TRUE;
	}
	m_cs.Unlock();
	return TRUE;
}

void CDataQueue::RemoveAll()
{
	int DataSize ;
	while(!DataPtrList.IsEmpty())
	{
		QueueBuf=NULL;
		m_cs.Lock();
		QueueBuf=(BYTE*)DataPtrList.RemoveHead();
		if(QueueBuf!=NULL)
		{
			memmove( &DataSize, &QueueBuf[0], sizeof(int));
			QueueNum--;
			DataLen -= DataSize;
			delete []QueueBuf;
            QueueBuf=NULL;
		}
		m_cs.Unlock();
	}			    
}

