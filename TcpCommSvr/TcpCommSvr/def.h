#if !defined(DEF)
#define DEF

#include <tchar.h>
#include <wchar.h>
#include <afx.h>
#include <malloc.h>

#define IN
#define OUT

#ifndef OPTIONAL
#define OPTIONAL
#endif

//  Attempted to open a comm port but it was already opened by another task
#define IO_ALREADY_OPEN                  ((DWORD)0xC1010123L)

//  Operation completed successfully
#define IO_SUCCESS                       ((DWORD)0x010101FFL)

//  Driver failed to open port
#define IO_COMM_PORT                     ((DWORD)0xC1010110L)

//  One or more parameters are invalid
#define IO_INVALID_PARAMETER             ((DWORD)0xC1010113L)

//  Driver failed to allocate an internal buffer
#define IO_ALLOC_BUFFER                  ((DWORD)0xC1010108L)

//  Driver failed to setup queue sizes on port %1. Error reported: %2
#define IO_SETUPCOMM_QUEUE               ((DWORD)0xC1010101L)

//  Driver failed to setup timeout values on port %1. Error reported: %2
#define IO_SETCOMM_TIMEOUTS              ((DWORD)0xC1010102L)

//  Driver failed to set comm port's state on port %1. Error reported: %2
#define IO_SETCOMM_STATE                 ((DWORD)0xC1010103L)

//  Driver failed to get the current configuration for port %1. Error reported: %2
#define IO_GETCOMM_STATE                 ((DWORD)0xC1010105L)

//  Driver failed to build a DCB structure for port %1. Error reported: %2
#define IO_BUILD_DCB                     ((DWORD)0xC1010104L)

//  Driver failed to set the comm port characteristics
#define IO_SET_MODE                      ((DWORD)0xC1010111L)

//  Driver failed to create the asynchronous read thread
#define IO_SETUP_IO                      ((DWORD)0xC1010112L)

//  An error occurred while attempting to read from the serial port
#define IO_READPORT_ERROR                ((DWORD)0xC101011BL)

//  An error occurred while attempting to write to the serial port
#define IO_WRITEPORT_ERROR               ((DWORD)0xC1010122L)

//  An unknown error occured while attempting to write to port %1. Error reported: %2
#define IO_WRITE_ERROR                   ((DWORD)0xC101010BL)

//  An unknown error occured while attempting to read from port %1. Error reported: %2
#define IO_READ_ERROR                    ((DWORD)0xC101010CL)

//  An error occurred while attempting to notify caller of pending data
#define IO_NOTIFY_ERROR                  ((DWORD)0xC101011CL)

//  Failed to cancel outstanding I/O
#define IO_CANCEL_ERROR                  ((DWORD)0xC1010121L)

#define EXPORT32 __declspec(dllexport)

#endif
