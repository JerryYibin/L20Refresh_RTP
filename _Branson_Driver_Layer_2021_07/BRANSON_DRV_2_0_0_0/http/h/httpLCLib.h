/* httpLCLib.h - HTTP server LiveControl protocol API header */

/*
 * Copyright (c) 1996-2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
02d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
02c,26jan10,h_y  support for 2010 
02b,12jun08,q_z  clear LC Poll Threads when server shutdown(WIND00106722)
02a,20oct03,dlr  Port to WM Web
01c,22jun99,km   merge of Wind Web Server 2.0 code.
01b,07apr98,mb   product status.
01a,05feb96,mb   written.
*/

#ifndef __INChttpLCLibh
#define __INChttpLCLibh

/* includes */

#include "httpLib.h"
#include "httpCfg.h"

#ifndef WEBCLI_STANDALONE_WEBSERVER

#ifdef __cplusplus
extern "C" {
#endif

/* Define for local debugging */
/* #define HTTP_LC_DEBUG */

/* defines */

/* Live Control packet IDs */
#define HTTP_LC_PACKET_PING           1
#define HTTP_LC_PACKET_QUIT           2
#define HTTP_LC_PACKET_VERSION        3
#define HTTP_LC_PACKET_SUBSCRIBE      4
#define HTTP_LC_PACKET_UNSUBSCRIBE    5
#define HTTP_LC_PACKET_VALUE          6
#define HTTP_LC_PACKET_INTERVAL       7
#define HTTP_LC_PACKET_ERROR          8
#define HTTP_LC_PACKET_DESCRIPTION    9
#define HTTP_LC_PACKET_SYMBOLTABLE    10
/* These added WMW 4.3 */
#define HTTP_LC_PACKET_SUBSCRIBE_NONE 11
#define HTTP_LC_PACKET_SUBSCRIBE_MED  HTTP_LC_PACKET_SUBSCRIBE
#define HTTP_LC_PACKET_SUBSCRIBE_HIGH 13
#define HTTP_LC_PACKET_GET            14
#define HTTP_LC_PACKET_GETNEXT        15

#define HTTP_LC_PACKET_FINISHED       127

/* Live Control packet ID acknowledges */
#define HTTP_LC_PACKET_PING_ACK       (- HTTP_LC_PACKET_PING )
#define HTTP_LC_PACKET_VERSION_ACK    (- HTTP_LC_PACKET_VERSION )
#define HTTP_LC_PACKET_SUBSCRIBE_ACK  (- HTTP_LC_PACKET_SUBSCRIBE )
#define HTTP_LC_PACKET_GET_ACK        (- HTTP_LC_PACKET_GET )
#define HTTP_LC_PACKET_GETNEXT_ACK    (- HTTP_LC_PACKET_GETNEXT )
#define HTTP_LC_PACKET_INTERVAL_ACK   (- HTTP_LC_PACKET_INTERVAL )

extern  WMB_SESSION_T *   pLCSession;

extern  ubyte4 httpRpmLCHighPoll  ;
extern  ubyte4 httpRpmLCMediumPoll;


#define kPollPushNone       0x00
#define kPollPushMedium     0x01
#define kPollPushHigh       0x02

#define LC_SUBSCRIBE_OK     0x00
#define LC_SUBSCRIBE_ERROR  0x01
#define LC_SUBSCRIBE_DUP    0x02

#define HTTP_SYM_TYPE_CHAR_ID        0x0001
#define HTTP_SYM_TYPE_SHORT_ID       0x0002
#define HTTP_SYM_TYPE_LONG_ID        0x0004
#define HTTP_SYM_TYPE_INT_ID         0x0008
#define HTTP_SYM_TYPE_UCHAR_ID       0x0010
#define HTTP_SYM_TYPE_USHORT_ID      0x0020
#define HTTP_SYM_TYPE_ULONG_ID       0x0040
#define HTTP_SYM_TYPE_UINT_ID        0x0080
#define HTTP_SYM_TYPE_BOOL_ID        0x0100
#define HTTP_SYM_TYPE_FLOAT_ID       0x0200
#define HTTP_SYM_TYPE_DOUBLE_ID      0x0400
#define HTTP_SYM_TYPE_STRING_ID      0x0800
#define HTTP_SYM_TYPE_GENLIST_ID     0x1000

/* External API - Asserts LC Lock */
extern HTTP_STATUS     httpLCInit (ubyte4 usClients,ubyte4 ulSymbols);
extern HTTP_STATUS     httpLCNotifyChange (char * szName, void * szValue, ubyte4 nValueSize);
extern HTTP_STATUS     httpLCSocketClosed (MUD_CONN_ID ioHandle);
extern HTTP_STATUS     httpRpmLiveControlInit ( );
extern short           httpRpmLiveControl (HTTP_REQ_ID const reqId);

extern HTTP_STATUS     httpLCDisplay(void);
extern void            httpLCPollPushDisplay(ubyte4 nDisplayFlags);

/* Internal API for Live Control */
HTTP_STATUS            httpLCLock (void);
HTTP_STATUS            httpLCUnlock (void);
BOOL                   httpLCSubscribeAccess (HTTP_REQ_ID const reqId,char *szName);
BOOL                   httpLCSubscribe (MUD_CONN_ID ioHandle,char *szName,ubyte4 nPriority,HTTP_GEN_LIST_ENTRY_ID clientId,ubyte4 *status);
BOOL                   httpLCUnsubscribe (char *szName,HTTP_GEN_LIST_ENTRY_ID clientId);
HTTP_GEN_LIST_ENTRY_ID httpLCClientAdd (char * szClient,HTTP_REQ_ID reqId);
void                   httpLCClientRemove (MUD_CONN_ID ioHandle);
HTTP_GEN_LIST_ENTRY_ID httpLCClientFindByCookie(char * szCookie);
HTTP_STATUS            httpDoLCSocketClosed (MUD_CONN_ID ioHandle);

HTTP_STATUS            httpLCClientNotify (MUD_CONN_ID ioHandle,char * szName, void * szValue, unsigned long nValueSize);
HTTP_STATUS            httpLCNotify (char * szName, void * szValue, size_t nValueSize,ubyte4 nCmnd);

HTTP_STATUS            httpLCPacketHeaderWrite (MUD_CONN_ID ioHandle, ubyte4 ulSize);
HTTP_STATUS            httpLCPacketWrite (MUD_CONN_ID ioHandle,char cCommand,char * szPacket,unsigned long ulPacketSize);

HTTP_STATUS            httpLCDescriptionWrite (MUD_CONN_ID ioHandle,char *szName, DATATYPE_T wmType);
int                    httpWMtoLCType(DATATYPE_T wmType);
HTTP_STATUS            httpLCWindMarkType(char *szName, DATATYPE_T *nType);
void                   httpLCPollPushShutdown(void);


#ifdef HTTP_LC_DEBUG
extern int httpLCReceiveEnable;
extern int httpLCSendEnable;
#endif

HTTP_STATUS httpLCBlockPut
    (
    MUD_CONN_ID   ioHandle, /* Handle of the active connection    */
    char * pchBuffer,       /* Buffer with data to send to socket */
    unsigned long ulSize         /* Number of bytes to write to socket */
    );

long httpLCBlockRead
    (
    MUD_CONN_ID   ioHandle, /* Handle of the active connection    */
    char * pchBuffer,       /* Buffer to store the read block     */
    unsigned long ulMaxSize        /* Maximum size of block              */
    );

#ifdef __cplusplus
}
#endif

#endif

#endif /* __INChttpLCLibh */

