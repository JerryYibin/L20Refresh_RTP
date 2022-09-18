/* httpTcp.h - HTTP server system dependent socket header */

/* Copyright (c) 1996-1999, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
04d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
04c,09sep99,csr  merging for zlib
04b,09jun99,km   merge of Wind Web Server 2.0 code.
04a,02apr98,ck   API changes for HTTP1.1 support.
03a,13may97,ck   adaped for OSAL interface.
02a,02sep96,ck   beta.
01a,01sep96,ck   written.
*/

#ifndef __INCosalTcph
#define __INCosalTcph

/* includes */

#include "memPoolLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

#ifndef DEFINED_HTTP_OSAL_IO_HNDL_ID
#define DEFINED_HTTP_OSAL_IO_HNDL_ID

typedef void * HTTP_OSAL_IO_HNDL_ID;

#endif /* DEFINED_HTTP_OSAL_IO_HNDL_ID */

#ifndef DEFINED_HTTP_OSAL_IO_CALLBACK_FUNC
#define DEFINED_HTTP_OSAL_IO_CALLBACK_FUNC

typedef void (* OSAL_IO_CALLBACK_FUNC) (HTTP_OSAL_IO_HNDL_ID ioHndlConnectSockId);

#endif

/* function declarations */

HTTP_OSAL_IO_HNDL_ID httpOsalSocketOpen       (void);
HTTP_STATUS          httpOsalSocketSleep      (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId);
HTTP_STATUS          httpOsalSocketClose      (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId);
HTTP_BOOL            httpOsalSocketIsNew      (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId);
HTTP_STATUS          httpOsalSocketCloseAddNotify (HTTP_OSAL_IO_HNDL_ID ioHndlConnectSockId, OSAL_IO_CALLBACK_FUNC pfuncNofiyHandler);
HTTP_STATUS          httpOsalSocketShutDown   (void);
long                 httpOsalSocketReadLine   (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId, char * szBuffer, unsigned long ulMaxLen);
long                 httpOsalSocketReadBlock  (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId, char * chBuffer, unsigned long ulBlockSize);
long                 httpOsalSocketWriteBlock (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId, char * chBuffer, unsigned long ulBlockSize);
HTTP_STATUS          httpOsalVaPrintf         (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId, char * szFormat, HTTP_VA_LIST va_list);
HTTP_STATUS          httpOsalPrintf           (HTTP_OSAL_IO_HNDL_ID ioConnectHandleId, char * szFormat, ...);
HTTP_STATUS          httpOsalSocketGetClientIP(HTTP_OSAL_IO_HNDL_ID ioConnectSocketId, char * szIPString);
HTTP_STATUS          httpOsalSocketTimeoutSet (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId, long lTimeout);
HTTP_STATUS          httpOsalStdIoStoreDefault(HTTP_OSAL_IO_HNDL_ID ioConnectSocketId);
HTTP_STATUS          httpOsalStdIoSetToDefault(HTTP_OSAL_IO_HNDL_ID ioConnectSocketId);
HTTP_STATUS          httpOsalStdIoSetToSocket (HTTP_OSAL_IO_HNDL_ID ioConnectSocketId);

unsigned long        httpOsalHTONL (unsigned long hostlong);
unsigned long        httpOsalNTOHL (unsigned long netlong);


#ifdef __cplusplus
}
#endif

#endif   /* __INCosalTcph */

