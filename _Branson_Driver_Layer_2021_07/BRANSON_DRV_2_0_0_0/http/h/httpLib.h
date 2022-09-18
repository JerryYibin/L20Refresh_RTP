/* httpLib.h - HTTP server library header */

/*
 * Copyright (c) 1996-2011, 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06nov14,r_w  fix static analysis issues
26mar13,r_w  chang unsigned long to int for defect WIND00409393
01nov11,r_w  HTTP header timeout support(WIND00307889)
20feb13,r_w  change char * to unsigned char *
             defect: WIND00403091 and WIND00403090
07aug12,r_w  fix compile errors when including C++ header files
             defect WIND00367448
07jun11,m_z  add etag/if-modified-since support 
             CQID: WIND00203773
28mar11,r_w  fix defect WIND00257014
             to make some declares of functions comply with definitions 
17dec10,m_z  part of APIs changed
             CQID: Fix WIND00247096
12jul10,m_z  change location auth. to avoid sub location being shielded 
             (WIND00206095)
26jan10,h_y  support for 2010 
04mar09,m_z  change interface of GID functions 
             (WIND00157654)
28aug08,q_z  change the some function return declared  (WIND00033655)
28aug08,q_z  removed the some const declared  (WIND00032048)
13apr04,adb  added httpRequestPortfolioRead and httpTrustedPortfolioRead
25feb04,adb  added httpMudTasksDelete and httpMudInfrastructureDelete
10feb04,adb  added httpMudCreate prototype
30jan04,adb  added httpPortAdd prototype
22jan04,adb  merging in (dkg) SPR 92564 fix, httpAbsPathGet() declaration
12jan04,gsr  added cleanup routine for SPR 93009
09jan04,jws  add note about ALLOW_PREDEFINES
01dec03,adb  added httpOutputScreen () and httpOutputValidate ()
24nov03,adb  Cross Site Scripting Mitigation
18aug03,jws  removed HTTP_BOOL and friends
07aug03,jws  change httpMimeHdrGet() type to be ptr to const char
17jan00,csr  adding httpSpecialCharEscapePrint() declaration
14dec99,va   adding POST related calls
14sep99,csr  httpSystemMemFsCompressMaxFiles -> httpSystemMemFsMaxFiles
09sep99,csr  merging for zlib
24aug99,csr  added extern for httpSystemMemFsMaxFiles.
09jun99,km   merge of Wind Web Server 2.0 code.
02apr98,ck   new variables for extended HTTP1.1 support.
26may97,mb   templates renamed to SSI.
13may97,mb   osal introduced.
20mar97,mb   httpOutputEnable() and httpOutputDisable added.
02oct96,mb   RPM CGI and RPM Template config structures added.
28sep96,mb   all system RPM config structures are now part of the API.
11sep96,mb   descriptions reworked.
02sep96,pk   beta test.
02aug96,pk   MIME encapsulation added.
10jul96,mb   prototypes reworked (const...).
05jul96,ck   httpPrintf and output/header generators prototypes added.
25jun96,mb   status, error and string prototypes added.
15jun96,ck   written.
*/

#ifndef __INChttpLibh
#define __INChttpLibh


/* includes */

#include <stdio.h>

#ifdef _WRS_KERNEL
  #include "logLib.h"
#endif

#include "wm.h"
#include "commonCfg.h"
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"
#include "httpWmb.h"
#include "httpWmbMacro.h"
#endif
#include "memPoolLib.h"
#include "mudLib.h"
#include "httpFileMgr.h"
#include "httpType.h"
#include "httpSem.h"
#include "httpList.h"
#include "httpUtil.h"
#include "httpFileRegistry.h"
#include "httpSaxC.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DEBUG_XXXX flags enables the debug messages.
 * To debug a specific module, compile it with the
 * -DDEBUG_INFO or -DDEBUG_ERROR option.
 * To enable debug messages ONLY on initialization
 * routine define HTTP_DEBUG in httpconf.h.
 */

#ifdef DEBUG_ERROR
#define logError(msg)        httpLogErrorFunction (msg)
#define logErrorMsg(msg,arg) httpLogErrorMsgFunction (msg, arg);
#else
#define logError(msg)
#define logErrorMsg(msg,arg)
#endif /* DEBUG_ERROR */

#ifdef DEBUG_INFO
#define logInfo(msg)         httpLogInfoFunction (msg)
#define logInfoMsg(msg,arg)  httpLogInfoMsgFunction (msg, arg)
#else
#define logInfo(msg)
#define logInfoMsg(msg,arg)
#endif /* DEBUG_INFO */


/* ALLOW_PREDEFINES is never defined! */

#ifdef ALLOW_PREDEFINES

#define HTTP_PREDEFINE(value)    = value
/* #define HTTP_POSTDEFINE(expression) */

#else

#define HTTP_PREDEFINE(value)
/* #define HTTP_POSTDEFINE(expression) expression */

#endif

/* defines */

/* default language supported */

/* #define HTTP_DEFAULT_LANGUAGE "en" */

/* HTTP methods */

/* the following numbers correspond with httpRequestTranslation.c! */

#define HTTP_M_UNKNOWN 0
#define HTTP_M_OPTIONS 1
#define HTTP_M_GET     2
#define HTTP_M_HEAD    3
#define HTTP_M_POST    4
#define HTTP_M_PUT     5
#define HTTP_M_DELETE  6
#define HTTP_M_TRACE   7
#define HTTP_M_MAX     8

/* HTTP protocol versions */

#define HTTP_1_0       0
#define HTTP_1_1       1

/* access selector for httpMime*() functions  */

#define HDR_IN         0
#define HDR_OUT        1

/* selector for httpStdIoRedirect() functions */

#define STD_IN         0
#define STD_OUT        1

/* HTTP status codes */

#define HTTP_CONTINUE         100
#define HTTP_SWITCH_PROT      101
#define HTTP_REQ_OK           200
#define HTTP_CREATED          201
#define HTTP_ACCEPTED         202
#define HTTP_NON_AUTH_INF     203
#define HTTP_NO_CONTENT       204
#define HTTP_RESET_CONTENT    205
#define HTTP_PART_CONTENT     206
#define HTTP_MULT_CHOICE      300
#define HTTP_MOVED_PERM       301
#define HTTP_MOVED_TEMP       302
#define HTTP_SEE_OTHER        303
#define HTTP_NOT_MODIFIED     304
#define HTTP_USE_PROXY        305
#define HTTP_BAD_REQUEST      400
#define HTTP_UNAUTHORIZED     401
#define HTTP_PAYMENT_REQ      402
#define HTTP_FORBIDDEN        403
#define HTTP_NOT_FOUND        404
#define HTTP_METHOD_NA        405
#define HTTP_NOT_ACCEPTED     406
#define HTTP_PROXY_AUTH_REQ   407
#define HTTP_REQ_TIME_OUT     408
#define HTTP_CONFLICT         409
#define HTTP_GONE             410
#define HTTP_LENGTH_REQIRED   411
#define HTTP_PRECOND_FAIL     412
#define HTTP_ENTITY_TOO_LARGE 413
#define HTTP_URI_TOO_LARGE    414
#define HTTP_UNSUP_MEDIA_TYPE 415
#define HTTP_INTERNAL_ERROR   500
#define HTTP_NOT_IMPLEMENTED  501
#define HTTP_BAD_GATEWAY      502
#define HTTP_SERV_UNAVAILABLE 503
#define HTTP_GATEWAY_TIMEOUT  504
#define HTTP_VERSION_NOT_SUPP 505

/* standard system error messages: */

/* nobody _SHOULD_ ever get this error */
#define HTTP_ERROR_INTERNAL             0
/* no RPM configured for this URL/method */
#define HTTP_ERROR_NO_RPM               1
/* something is wrong with the URL */
#define HTTP_ERROR_WRONG_URL            2
/* cannot find specified file object */
#define HTTP_ERROR_FILE_NOT_FOUND       3
/* error occurred during download (NFS)*/
#define HTTP_ERROR_TRANSFER_INTERRUPTED 4
#define HTTP_ACCESS_DENIED              5
#define HTTP_ERROR_CONFIGURATION        6
#define HTTP_ERROR_ENTITY_TOO_LARGE     7

/* users may define special errors starting with this number (included) */

#define HTTP_USER_ERROR                 8

/* typedefs */

/*
 * Default (opaque) definition of HTTP_REQ.
 * Cf. http/private/httpReq.h for the full definition.
 */

typedef struct http_req_tag HTTP_REQ;
typedef HTTP_REQ *      HTTP_REQ_ID;

typedef struct http_status_msg
    {
    short  sNumber;
    char * szMessage;
    } HTTP_STATUS_MSG;

typedef struct mimeSuffixGuess
    {
    char * szSuffix;
    char * szType;
    } HTTP_MIME_SUFFIX_GUESS;

typedef HTTP_STATUS (* HTTP_MIME_TOKEN_PARSER)(HTTP_REQ_ID, char *);

/* globals */

extern  unsigned short httpPort;
extern  unsigned short httpSSLPort;
extern  unsigned long  httpAcceptIP;
extern  unsigned short httpMaxPorts;
extern  unsigned short httpMaxConnections;
extern  int            httpMaxSockBuff;
extern  unsigned short httpSockTimeout;
extern  unsigned short httpHeaderTimeout;
extern  long           httpProWebSockTimeout;

extern  unsigned long  httpReqMemPartSize;
extern  unsigned long  httpGlobalMemPartSize;
extern  unsigned long  httpMaxUrlSize;
extern  unsigned long  httpMaxTempBufSize;
extern  unsigned short httpMaxArguments;
extern  unsigned short httpMaxMimeLines;
extern  unsigned short httpMaxRecCookies;
extern  unsigned short httpMaxSendCookies;
extern  unsigned long  httpSystemMemFsMaxFiles;

extern  unsigned short  httpPriority;
extern  unsigned short  httpMaxProcesses;
extern  long            httpdTaskStackSize;

extern  const char * const    httpSysErrorTab[];
extern  const char * const    httpErrorHeader;
extern  const char * const    httpErrorFooter;
extern  char *    httpServerName;
extern  char *    httpServerIdentification;
extern  char *    httpUri;
extern  char      httpPostFileUploadPath[];

extern const unsigned short httpMaxErrors;
extern unsigned short kMacroRepeatNestMax;

extern  char *              kUniqueSignature;
extern const long           kSectorSize;
extern const long           kNVRamSize;

extern OS_THREAD_T *        httpServerTaskIds;

#if 0                       /* never referenced */
extern HTTP_REQ_ID *        httpReqIdResource;
#endif

extern BOOL                 httpPersistentConnectionsAllowed;
/*extern BOOL               httpServerUncompress;*/
extern MUD_ID               httpMud;
#ifdef INCLUDE_HTTP_WMB_INTERFACE
extern sbyte                httpWmbName[64]; /* !! magic 64 !! */
#endif

#ifdef INCLUDE_OPENSSL_SUPPORT
extern  char           httpSSLCertificateFile[];
extern  char           httpSSLPrivateKeyFile[];
extern  char           httpSSLCipherList[];
extern  char           httpSSLCAFile[];
extern  char           httpSSLCAPath[];
#endif

/* function declarations */

/*
 * httpd () is defined by the user, as part of configuring the http server.
 * It is declared here so that it can be called from elsewhere; in particular,
 * it is called as part of VxWorks initialization.  However, it is the
 * responsibility of the user to provide this function, and to ensure that
 * it confoms to this prototype.
 */

HTTP_STATUS            httpd (void);

HTTP_STATUS            httpInit (void);
HTTP_STATUS            httpCleanup (void);
HTTP_STATUS            httpShutdown (void);

HTTP_STATUS            httpMudCreate (void);
HTTP_STATUS            httpPortAdd (unsigned short port,
                                    SSL_CONFIGURATION_ID pSSLConfiguration);
HTTP_STATUS            httpServerCreate (void);
HTTP_STATUS            httpMudTasksDelete (void);
HTTP_STATUS            httpMudInfrastructureDelete (void);

HTTP_STATUS            httpServerStart (void); /* deprecated */
HTTP_STATUS            httpServerKill (void);  /* deprecated */

short                  httpBase64decode (unsigned char * szSource, unsigned char * szDestination);
HTTP_STATUS            httpBase64encode (unsigned char * cpSource,
                                         unsigned short sSourceLength,
                                         unsigned char * szDestination,
                                         unsigned short usDestLength);

void                   httpLogInfoFunction (char * szMessage);
void                   httpLogInfoMsgFunction (char * szMessage, ...);
void                   httpLogErrorFunction (char * szMessage);
void                   httpLogErrorMsgFunction (char * szMessage, ...);

HTTP_STATUS            httpStatusLine (HTTP_REQ_ID reqId);
HTTP_STATUS            httpError (HTTP_REQ_ID reqId, char *  szMsg);
char *                 httpErrStringGet (unsigned short usIndex);

char *                 httpGetEnv (HTTP_REQ_ID reqId, char * szName);
HTTP_GEN_LIST_ENTRY_ID httpSetEnv (HTTP_REQ_ID reqId,
                                   char * szName,
                                   char * szValue);

HTTP_STATUS            httpStdIoStoreDefault (HTTP_REQ * pReq);
HTTP_STATUS            httpStdIoSetToDefault (HTTP_REQ * pReq);
HTTP_STATUS            httpStdIoSetToSocket  (HTTP_REQ * pReq);

HTTP_STATUS            httpTimeoutSet (HTTP_REQ *  pReq, long  lTimeout);
HTTP_STATUS            httpClientIPGet (HTTP_REQ * pReq, char * szIPString);
HTTP_STATUS            httpStringPut (HTTP_REQ * pReq, char * szString);
HTTP_STATUS            httpBlockPut (HTTP_REQ * pReq,
                                     char * pchBuffer,
                                     unsigned long ulSize);
long                   httpLineRead (HTTP_REQ * pReq,
                                     char * szBuffer,
                                     size_t bufSize);                                     
long                   httpBlockRead (HTTP_REQ * pReq,
                                      char * pchBuffer,
                                      size_t ulMaxSize);
sbyte4                 httpPrintf (HTTP_REQ * pReq, char * szFormat, ...);

HTTP_STATUS            httpSpecialCharEscapePrint (HTTP_REQ_ID reqId,
                                                   char * szArg);
HTTP_STATUS            httpTagValueGet (char *  szSource,
                                        const char * const szToken,
                                        char *szBuffer,
                                        long sBufferSize);
char *                 httpTagValueIterate (char *szSource,char **pszNext);

char *                 httpBufferGet (HTTP_REQ_ID reqId);
char *                 httpRpmIdentGet (HTTP_REQ_ID reqId);
char *                 httpRpmDataGet (HTTP_REQ_ID reqId);
void                   httpRpmIdentAndDataSet (HTTP_REQ_ID reqId,
                                               char * szNewIdent,
                                               char * szNewData);
void                   httpStatusSet (HTTP_REQ_ID reqId, short sStatus);
BOOL                   httpWmbErrorsExist (HTTP_REQ_ID reqId);
HTTP_GEN_LIST_ENTRY_ID httpWmbErrorIterator (HTTP_REQ_ID reqId,
                                             HTTP_GEN_LIST_ENTRY_ID lstEntry,
                                             sbyte ** pszFailedWindMark,
                                             sbyte ** pszErrorMsg);
HTTP_GEN_LIST_ENTRY_ID httpWmbErrorStore (HTTP_REQ_ID reqId,
                                          sbyte * szFailedWM,
                                          sbyte * szErrorMsg);

char *                 httpLocationGet (HTTP_REQ_ID reqId);
HTTP_STATUS            httpLocationSet (HTTP_REQ_ID reqId, char * szLocation);
HTTP_STATUS            httpLocCandEntryAdd(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID gidEntryId);
HTTP_GEN_LIST_ENTRY_ID httpLocCandEntryGet(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID * gidEntryId);
char *                 httpLocCandEntryDataGet(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID gidEntryId);
HTTP_GEN_LIST_ENTRY_ID httpLocCandEntryRemove(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID * gidEntryId);
HTTP_STATUS            httpGidEntryInsert(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID gidEntryId);
HTTP_GEN_LIST_ENTRY_ID httpGidEntryGet(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID * gidEntryId);
char *                 httpGidEntryDataGet(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID gidEntryId);
HTTP_GEN_LIST_ENTRY_ID httpGidEntryRemove(HTTP_REQ_ID reqId, HTTP_GEN_LIST_ENTRY_ID * gidEntryId);

BOOL                   httpHeaderGenerate (HTTP_REQ_ID reqId);
void                   httpOutputGenerator (HTTP_REQ_ID reqId);
void                   httpOutputEnable (HTTP_REQ_ID reqId);
void                   httpOutputDisable (HTTP_REQ_ID reqId);
void                   httpChunkedEncodingOn (HTTP_REQ_ID reqId);
void                   httpChunkedEncodingOff (HTTP_REQ_ID reqId);
BOOL                   httpNotModifiedResp (HTTP_REQ_ID  reqId);
HTTP_STATUS            httpIfModifiedSinceValueGet (const char * src, struct tm* dst );

BOOL                   httpMimePersistentConnectionGet (HTTP_REQ_ID);
HTTP_STATUS            httpMimePersistentConnectionSet (HTTP_REQ_ID, BOOL);



/*
 * These generic functions obsolete all other Mime header access functions
 * (introduced with version 2.0)
 */

const char *           httpMimeHdrGet (HTTP_REQ_ID reqId,
                                       unsigned short usMimeHdr,
                                       const char * const szName);
HTTP_STATUS            httpMimeHdrSet (HTTP_REQ_ID reqId,
                                       unsigned short usMimeHdr,
                                       const char * const szName,
                                       const char * const szValue);

/* The use of the following functions is DEPRECATED. */

char *                 httpMimeDateGet (HTTP_REQ_ID reqId,
                                        unsigned short usSelect);
HTTP_STATUS            httpMimeDateSet (HTTP_REQ_ID reqId,
                                        unsigned short usSelect,
                                        char * szDate);

char *                 httpMimeExpiresGet (HTTP_REQ_ID reqId,
                                           unsigned short usSelect);
HTTP_STATUS            httpMimeExpiresSet (HTTP_REQ_ID reqId,
                                           unsigned short usSelect,
                                           char * szExpires);
HTTP_STATUS            httpMimeExpiresSetImmediate (HTTP_REQ_ID reqId);

char *                 httpMimeContentTypeGet (HTTP_REQ_ID reqId,
                                               unsigned short usSelect);
HTTP_STATUS            httpMimeContentTypeSet (HTTP_REQ_ID reqId,
                                               unsigned short usSelect,
                                               char * szType);
char *                 httpMimeContentEncodingGet (HTTP_REQ_ID reqId,
                                                   unsigned short usSelect);
HTTP_STATUS            httpMimeContentEncodingSet (HTTP_REQ_ID reqId,
                                                   unsigned short usSelect,
                                                   char * szType);
long                   httpMimeContentLengthGet (HTTP_REQ_ID reqId,
                                                 unsigned short usSelect);
HTTP_STATUS            httpMimeContentLengthSet (HTTP_REQ_ID reqId,
                                                 unsigned short usSelect,
                                                 long lLength);

HTTP_STATUS            httpMimeOutCookieSet (HTTP_REQ_ID reqId,
                                             char * szName,
                                             char * szValue,
                                             char * szPath,
                                             char * szDomain,
                                             char * szExpire,
                                             long lMaxAge,
                                             BOOL fSecure);
char *                 httpMimeInCookieGet (HTTP_REQ_ID reqId, char * szName);
char *                 httpMimeInCookieIterate (HTTP_REQ_ID reqId,
                                                unsigned short * pusIndex);

/* End of the list of deprecated functions */

char *                 httpAuthorizationGet (HTTP_REQ_ID reqId,
                                             unsigned short usSelect);
char *                 httpAuthenticationGet (HTTP_REQ_ID reqId,
                                              unsigned short usSelect);
HTTP_STATUS            httpAuthenticationSet (HTTP_REQ_ID reqId,
                                              unsigned short usSelect, char *  szAuth);

MEM_POOL_ID            httpReqMemPartIdGet (HTTP_REQ_ID reqId);
MEM_POOL_ID            httpGlobalMemPartIdGet (void);
HTTP_STATUS            httpGlobalMemPartCreate (unsigned long ulSize);
sbyte *                httpConservativeAlloc (MEM_POOL_ID memPool,
                                              size_t * pSizeInBytes);

HTTP_STATUS            httpDateGet (char * const szDate);
HTTP_STATUS            httpDateToString (char * const szDate,
                                         unsigned short usMaxlen,
                                         unsigned long ulSeconds);
BOOL                   httpStringToUpper (char * szString);
short                  httpStrNCaseCmp (const char * const szString1,
                                        const char * const szString2,
                                        unsigned long sStrLen);
char *                 httpStrdup (MEM_POOL_ID memPartID,
                                   const char * const szString);
char *                 httpStrCpy (char * szTo, char * szFrom);

BOOL                   httpIsMatching (MEM_POOL_ID memPartID,
                                       const char * const szPattern,
                                       const char * const szExamenee);
long                   httpSignificantChars (const char * const szPattern);

HTTP_STATUS            httpLineUnescape (char * szString);
HTTP_STATUS            httpLineEscape (char *  szString,
                                       short sMaxStringLength);

long                   httpMultipartFileSize (HTTP_REQ_ID reqId,
                                              long lBytesRemaining,
                                              short sCharsPerCRLF);
HTTP_STATUS            httpMultipartHeaderDiscard (HTTP_REQ_ID reqId,
                                                   long * lBytesRemaining,
                                                   short * sCharsPerCRLF);
HTTP_STATUS            httpMultipartFooterDiscard (HTTP_REQ_ID reqId,
                                                   long lBytesRemaining);
char *                 httpBoundaryGet (HTTP_REQ_ID reqId);
HTTP_STATUS            httpBoundarySet (HTTP_REQ_ID reqId, char * szBoundary);

BOOL                   httpAppErrorInit (char * szFile);
void                   httpAppErrorDefaultFileSet (char * szFile);
HTTP_STATUS            httpAppErrorSet (char * szCode,
                                        char * szShortDesc,
                                        char * szFullDesc,
                                        char * szCancelLink,
                                        char * szCancelText,
                                        char * szOkLink,
                                        char * szOkText);
HTTP_STATUS            httpAppError (HTTP_REQ_ID reqId,
                                     char * szCode,
                                     char * szShortDesc,
                                     char * szFullDesc,
                                     char * szCancelLink,
                                     char * szCancelText,
                                     char * szOkLink,
                                     char * szOkText);
HTTP_STATUS            httpPostReqParse (HTTP_REQ_ID reqId);
HTTP_STATUS            httpPostMultipartHeaderParse (HTTP_REQ_ID reqId,
                                                     long * lBytesRemaining,
                                                     short * sCharsPerCRLF,
                                                     int * tagFlag,
                                                     char * tagValue);

STATUS                 httpPortfolioRead (sbyte * fileName);

STATUS                 httpRequestPortfolioRead (HTTP_REQ_ID reqId,
                                                 sbyte * fileName);

STATUS                 httpTrustedPortfolioRead (sbyte * fileName,
                                                 BOOL    useSecureFS);
#ifndef WEBCLI_STANDALONE_WEBSERVER
WMB_SESSION_T *        httpWmbSessionGet (HTTP_REQ_ID reqId);
#endif

/* auxiliary functions */

void                   httpXSSFilterTableCreate (void);
BOOL                   httpOutputFilter (char * pOutput);
BOOL                   httpOutputScreen (char * pOutput);
BOOL                   httpOutputValidate (char * pOutput,
                                           unsigned int maxOutputSize);
char *                 httpAbsPathGet (HTTP_REQ_ID  reqId);

#ifdef __cplusplus
}
#endif

#endif /* __INChttpLibh */

