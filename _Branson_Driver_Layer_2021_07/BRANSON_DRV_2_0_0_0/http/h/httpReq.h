/* httpReq - HTTP server request definition header */

/* Copyright (c) 1996-2003,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
02d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
02c,12jul10,m_z  change location auth. to avoid sub location being shielded
                 (WIND00206095)
02b,04mar09,m_z  change GID content in struct http_req_tag
                 (WIND00157654)
02a,15dec03,adb  ioHandle type modification
01e,09sep99,csr  merging for zlib
02b,09jun99,km   merge of Wind Web Server 2.0 code.
02a,03apr98,ck   added support for persistent connections.
01c,08sep97,ics  added Date and Expires fields.
01b,15may97,mb   osal introduced.
01a,03sep96,mb   written.
*/

#ifndef __INChttpReqh
#define __INChttpReqh

#include "httpCfg.h"
#include "httpType.h"
#include "httpTcp.h"
#include "httpSem.h"
#include "httpList.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "httpWmbMacro.h"
#endif
#include "mudLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

struct http_req_tag
    {
    HTTP_GEN_LIST_ID     MimeHdrListId[2]; /* MIME header of incoming/outgoing HTTP requests */
    HTTP_BOOL            fHdrWritten;      /* is MIME header already written to network ? */
    HTTP_BOOL            fProduceOutput;   /* whether a body has to / can be generated */
    HTTP_BOOL            fPersistent;      /* whether the connection is intended to be persistent */
    HTTP_BOOL            fChunked;         /* whether transfer encoding "chunked" is enabled */
    HTTP_BOOL            fWmbTable;        /* whether we are currently parsing a WMB table or not */
    short                wmbErrorCount;    /* Number of WindMark access errors */
    short                sMethod;          /* URL method ( GET , POST , ...) */
    short                sProtocol;        /* protocol version, eg 1001=HTTP1.1 or 1000=HTTP1.0 */
    char *               szAbsPath;        /* absolute path component of the URL */
    char *               szRpmIdent;       /* URL fragment that identified an invoked RPM */
    char *               szRpmData;        /* URL fragment that follows the identifier */
    HTTP_GEN_LIST_ID     argListId;        /* environment variables containing request arguments and info */
    HTTP_GEN_LIST_ID     recCookieListId;  /* List of received cookies with this request */
    HTTP_GEN_LIST_ID     sendCookieListId; /* List of cookies to send with this request */
    HTTP_GEN_LIST_ID     wmbPostErrListId; /* List of WMB errors that occurred during a POST */
    short                sStatus;          /* HTTP return status */
    MUD_CONN_ID          ioHandle;         /* 
                                            * handle for encapsulated network I/O; HTTP is
                                            * a MUD trusted i.e. not guarded application
                                            */
    HTTP_BOOL            fHdrOnly;         /* if TRUE transmit http header only */
    HTTP_GEN_LIST_ENTRY_ID gidEntryList;   /* GID entry list for autheorization */
    unsigned short       usGidEntryCount;   /* GID entry list for autheorization */
    HTTP_GEN_LIST_ENTRY_ID locCandEntryList; /* List of possible locations */
    unsigned short       usLocCandEntryCount;   /* possible location entry list for autheorization */
    char *               szLocation;       /* Pointer to a location identification string (best match)*/
    char *               szBoundary;       /* boundary string for multipart documents */
    char *               szBuffer;         /* temporary used buffer for all functions */
#ifndef WEBCLI_STANDALONE_WEBSERVER 
    MACRO_STACK_ID       repeatStack;      /* stack used to track nested REPEATs */
    MACRO_STACK_ID       ifStack;          /* stack used to track nested IFs */
#endif    
    MEM_POOL_ID          memPool;          /* memory pool handle */   
#ifndef WEBCLI_STANDALONE_WEBSERVER 
    WMB_SESSION_T *      pSession;         /* current WMB session if any */
#endif
    };

#ifdef __cplusplus
}
#endif

#endif /* __INChttpReqh */

