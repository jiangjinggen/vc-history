#if !defined(SERIAL_H)
#define SERIAL_H

#include "def.h"
class CSerial  
{
public:
	EXPORT32 CSerial(IN const TCHAR *,
					 IN OPTIONAL void (*lpfnMsgDisplay)(TCHAR *) = NULL);
	EXPORT32 ~CSerial();
	EXPORT32 DWORD Open(IN const TCHAR *szPortName, 
						IN const TCHAR *szCharacteristics, 
						IN OPTIONAL HANDLE hNotifyEvent = NULL, 
						IN OPTIONAL BOOL bModem = FALSE,
						IN OPTIONAL BOOL bAsyncRead = TRUE, 
						IN OPTIONAL BOOL bPortShare = FALSE);
	EXPORT32 DWORD SetBufferSize(IN DWORD);
	EXPORT32 void  Close(void);
	EXPORT32 DWORD Send(IN unsigned char *pBuffer, 
						IN DWORD dwSize, 
						IN OPTIONAL const TCHAR *szPhoneNumber = NULL,
						IN OPTIONAL DWORD dwKeyUpTime = 0, 
						IN OPTIONAL DWORD dwKeyDownTime = 0, 
						IN OPTIONAL LPOVERLAPPED_COMPLETION_ROUTINE pfnCallBack = NULL);
	EXPORT32 DWORD TransmitClear();
	EXPORT32 DWORD Receive(OUT unsigned char *, IN DWORD, OUT DWORD *);
	EXPORT32 DWORD ReceiveCount(void);
	EXPORT32 DWORD ReceiveClear(void);
	EXPORT32 HANDLE GetCommHandle() const { return m_hCommPort;}
private:
	CRITICAL_SECTION    m_Lock;

	BOOL                m_bTimeToShutdown;

	HANDLE              m_hCommPort,
						m_hReadThread,
						m_hDisconnectThread,
						m_hNotifyEvent,
						m_hConnectTimer;
						
	DCB                 m_OldDCB,
						m_CurrentDCB;

	DWORD               m_dwTransmitCount,
						m_dwReceiveCount,
						m_dwTotalTransmitSize,
						m_dwTotalReceiveSize,
						m_dwReadThreadId,
						m_dwDisconnectThreadId,
						m_dwOpenPortRetry,
						m_dwCharacterTimeout,
						m_dwReadThreadPriority;
						
	unsigned char       *m_pLocalBuffer,
						*m_pGlobalBuffer,
						*m_pGlobalTail,
						*m_pGlobalHead,
						*m_pGlobalEnd;

	TCHAR               m_szPortName[256],
						m_szPortCharacteristics[256],
						m_szAppName[256];

	BOOL                m_bAsyncRead,
						m_bPortOpen,
						m_bDisconnectThreadRunning,
						m_bPortShare;

	void                (*m_lpfnMsgDisplay)(TCHAR *);

	OVERLAPPED          m_WriteOverlap,
						m_ReadOverlap;

	DWORD WINAPI OpenPort(IN const TCHAR *, IN BOOL);
	DWORD WINAPI SetMode(IN const TCHAR *);
	DWORD WINAPI SetupIO(IN BOOL);
	void  WINAPI PutData(IN DWORD);
	DWORD WINAPI ReadPort(void);

	static BOOL ReadHandler(IN LPVOID);
	inline void DisplayMessage(IN TCHAR *szStr)
	{
		if (m_lpfnMsgDisplay)
			(*m_lpfnMsgDisplay)(szStr);
	}
};

#define INPUT_QUEUE_SIZE        2048
#define OUTPUT_QUEUE_SIZE       2048


#define ONE_TENTH_SECOND        100
#define ONE_SECOND              1000
#define TWO_SECONDS             2000
#define FIVE_SECONDS            5000
#define TEN_SECONDS				10000
#define THIRTY_SECONDS          30000

#endif
