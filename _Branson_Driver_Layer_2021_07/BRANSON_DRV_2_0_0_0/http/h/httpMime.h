/* httpMime.h - HTTP server MIME header */

/* Copyright 1996-1999,2010-2011 Wind River Systems, Inc. */

/*
modification history
-------------------- 
02a,07jun11,m_z add header functions for WIND00203773
                CQID: WIND00203773
01e,026jan10,h_y  support for 2010 
01d,12oct05,wdz      '#ifdef __cplusplus' statement changed
                 for SPR#98444
01c,09sep99,csr  merging for zlib
01d,26aug99,va   adding httpMimeAcceptEncoding function
01c,09jun99,km   merge of Wind Web Server 2.0 code.
01b,08oct96,wk   adapted for vxWorks 5.2 on UNIX host and m68k target.
01a,03sep96,ck   written.
*/

#ifndef __INChttpMimeh
#define __INChttpMimeh

#ifdef __cplusplus
/* includes */
#include "httpType.h"
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#include "httpType.h"
#endif /* __cplusplus */

#include "httpCfg.h"

/* function declarations */

HTTP_STATUS httpMimeAuthorization  (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeContentLength  (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeContentType    (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeAcceptEncoding (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeConnection     (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeReadCookie     (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeKeepAlive      (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimePersist        (HTTP_REQ_ID reqId,char * szLine);
#ifdef INCLUDE_HTTP_LANGUAGE_NEGOTIATION
HTTP_STATUS httpMimeAcceptLanguage (HTTP_REQ_ID reqId,char * szLine);
#endif
HTTP_STATUS httpMimeAccept         (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeUserAgent      (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeHost           (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeIfNoneMatch    (HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeIfModifiedSince(HTTP_REQ_ID reqId,char * szLine);
HTTP_STATUS httpMimeInEtag         (HTTP_REQ_ID reqId, char * szLine);
   
#ifdef __cplusplus
}
#endif

#endif /* __INCMimeh */

