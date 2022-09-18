/* httpCfg.h */

/* Copyright(c) 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
06nov14,r_w  add code control based on __JAVASCRIPT_DIGEST_ENABLED__
22may14,r_w  written
*/

#ifndef __HTTP_CFG_H_HEADER__
#define __HTTP_CFG_H_HEADER__

#include <vsbConfig.h>

#undef HTTP_LOG_FILE
#undef HTTP_LOG_FILE_SIZE_MAX_BYTES
#undef WEBCLI_STANDALONE_WEBSERVER
#undef __NO_ROMBUILD_IMAGE__
#undef HTTP_SECTOR_SIZE
#undef HTTP_NVRAM_SIZE
#undef HTTP_NVRAM_START
#undef INCLUDE_HTTP_LIVE_CONTROL
#undef HTTP_LC_POLL_MED
#undef HTTP_LC_POLL_HGH
#undef __JAVASCRIPT_DIGEST_ENABLED__
#undef kHwIdleTimeoutSecs
#undef kHwMaxSimultaneousClients
#undef kJavaScriptLogonPage
#undef WEBCLI_INCLUDE_SMTP
#undef HTTP_ETAG_ENABLE
#undef  INCLUDE_HTTP_MIME_HDR_PROC
#undef  INCLUDE_HTTP_MIME_GUESSTYPE
#undef  INCLUDE_HTTP_LANGUAGE_NEGOTIATION
#undef  INCLUDE_HTTP_LOG
#undef  INCLUDE_HTTP_FS
#undef  INCLUDE_HTTP_ALIASING
#undef  INCLUDE_HTTP_ACCESS_CTRL
#undef  INCLUDE_HTTP_IP_CHECKING
#undef  INCLUDE_HTTP_PWD_CHECKING
#undef  INCLUDE_HTTP_SHOWAUTH
#undef  INCLUDE_HTTP_CGI
#undef  INCLUDE_HTTP_UPLOAD
#undef  INCLUDE_HTTP_WMB_INTERFACE
#undef  INCLUDE_HTTP_POST

/* where to store your logfile (if using logging RPM) */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_LOG_FILE
#define  HTTP_LOG_FILE "/logfile.txt"
#else
#define HTTP_LOG_FILE _WRS_CONFIG_WEBCLI_HTTP_LOG_FILE
#endif

#ifndef _WRS_CONFIG_WEBCLI_HTTP_LOG_FILE_SIZE_MAX_BYTES
#define HTTP_LOG_FILE_SIZE_MAX_BYTES 100000
#else
#define HTTP_LOG_FILE_SIZE_MAX_BYTES _WRS_CONFIG_WEBCLI_HTTP_LOG_FILE_SIZE_MAX_BYTES
#endif

#ifndef _WRS_CONFIG_WEBCLI_STANDALONE_WEBSERVER
#undef WEBCLI_STANDALONE_WEBSERVER
#else
#define WEBCLI_STANDALONE_WEBSERVER
#endif

/* Build FileSystem Flag */
#ifndef _WRS_CONFIG_WEBCLI_ROMBUILD_IMAGE
#define  __NO_ROMBUILD_IMAGE__
#else
#undef  __NO_ROMBUILD_IMAGE__
#endif

#ifndef _WRS_CONFIG_WEBCLI_HTTP_SECTOR_SIZE
#define  HTTP_SECTOR_SIZE 0x00010000
#else
#define  HTTP_SECTOR_SIZE _WRS_CONFIG_WEBCLI_HTTP_SECTOR_SIZE
#endif

#ifndef _WRS_CONFIG_WEBCLI_HTTP_NVRAM_SIZE
#define  HTTP_NVRAM_SIZE 0x00080000
#else
#define  HTTP_NVRAM_SIZE _WRS_CONFIG_WEBCLI_HTTP_NVRAM_SIZE
#endif

#ifndef _WRS_CONFIG_WEBCLI_HTTP_NVRAM_START
#define  HTTP_NVRAM_START 0x00000000
#else
#define  HTTP_NVRAM_START _WRS_CONFIG_WEBCLI_HTTP_NVRAM_START
#endif

/*
 * scalability/feature configuration
 * comment out some or all of the following statements to scale
 * functionality and code size of the Web Server
 */


/* 
 * Live Control configuration
 */

/* include Live Control Java processing */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_LIVE_CONTROL
#undef  INCLUDE_HTTP_LIVE_CONTROL
#else
#define  INCLUDE_HTTP_LIVE_CONTROL
#endif

#ifndef _WRS_CONFIG_WEBCLI_HTTP_LC_POLL_MED
#define  HTTP_LC_POLL_MED 500
#else
#define  HTTP_LC_POLL_MED _WRS_CONFIG_WEBCLI_HTTP_LC_POLL_MED
#endif

#ifndef _WRS_CONFIG_WEBCLI_HTTP_LC_POLL_HGH
#define  HTTP_LC_POLL_HGH 300
#else
#define  HTTP_LC_POLL_HGH _WRS_CONFIG_WEBCLI_HTTP_LC_POLL_HGH
#endif
/* 
 * Web Authentication configuration
 */

/* Enable Javascript authentication */
#ifndef _WRS_CONFIG_WEBCLI_JAVASCRIPT_DIGEST_ENABLED
#undef  __JAVASCRIPT_DIGEST_ENABLED__
#else
#define  __JAVASCRIPT_DIGEST_ENABLED__
#endif

/* secure session timeout */
#ifndef _WRS_CONFIG_WEBCLI_kHwIdleTimeoutSecs
#define  kHwIdleTimeoutSecs 120
#else
#define  kHwIdleTimeoutSecs _WRS_CONFIG_WEBCLI_kHwIdleTimeoutSecs
#endif

/*                */
#ifndef _WRS_CONFIG_WEBCLI_kHwMaxSimultaneousClients
#define  kHwMaxSimultaneousClients 10
#else
#define  kHwMaxSimultaneousClients _WRS_CONFIG_WEBCLI_kHwMaxSimultaneousClients
#endif

#ifndef _WRS_CONFIG_WEBCLI_kJavaScriptLogonPage
#define  kJavaScriptLogonPage "logon.html"
#else
#define  kJavaScriptLogonPage _WRS_CONFIG_WEBCLI_kJavaScriptLogonPage
#endif

/* Enable SMTP */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_SMTP_ENABLE
#undef  WEBCLI_INCLUDE_SMTP
#else
#define  WEBCLI_INCLUDE_SMTP
#endif

/* Enable ETAG */
#ifndef _WRS_CONFIG_WEBCLI_HTTP_ETAG_ENABLE
#undef  HTTP_ETAG_ENABLE
#else
#define  HTTP_ETAG_ENABLE
#endif

/* 
 * RPM configuration
 */

/* include MIME header processing */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_MIME_HDR_PROC
#undef  INCLUDE_HTTP_MIME_HDR_PROC
#else
#define  INCLUDE_HTTP_MIME_HDR_PROC
#endif

/* include MIME type guessing support */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_MIME_GUESSTYPE
#undef  INCLUDE_HTTP_MIME_GUESSTYPE
#else
#define  INCLUDE_HTTP_MIME_GUESSTYPE
#endif

/* support pages with equal content in several languages */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_LANGUAGE_NEGOTIATION
#undef  INCLUDE_HTTP_LANGUAGE_NEGOTIATION
#else
#define  INCLUDE_HTTP_LANGUAGE_NEGOTIATION
#endif

/* logfile support RPM */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_LOG
#undef  INCLUDE_HTTP_LOG
#else
#define  INCLUDE_HTTP_LOG
#endif

/* include file system RPM */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_FS
#undef  INCLUDE_HTTP_FS
#else
#define  INCLUDE_HTTP_FS
#endif

/* include RPM alias */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_ALIASING
#undef  INCLUDE_HTTP_ALIASING
#else
#define  INCLUDE_HTTP_ALIASING
#endif

/* include access control RPM */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_ACCESS_CTRL
#undef  INCLUDE_HTTP_ACCESS_CTRL
#else
#define  INCLUDE_HTTP_ACCESS_CTRL
#endif

/* include and enable IP checking */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_IP_CHECKING
#undef  INCLUDE_HTTP_IP_CHECKING
#else
#define  INCLUDE_HTTP_IP_CHECKING
#endif

/* include password checking - needs INCLUDE_HTTP_MIME_HDR_PROC */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_PWD_CHECKING
#undef  INCLUDE_HTTP_PWD_CHECKING
#else
#define  INCLUDE_HTTP_PWD_CHECKING
#endif

/* show authentication information */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_SHOWAUTH
#undef  INCLUDE_HTTP_SHOWAUTH
#else
#define  INCLUDE_HTTP_SHOWAUTH
#endif

/* include CGI support */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_CGI
#undef  INCLUDE_HTTP_CGI
#else
#define  INCLUDE_HTTP_CGI
#endif

/* include file upload RPM */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_UPLOAD
#undef  INCLUDE_HTTP_UPLOAD
#else
#define  INCLUDE_HTTP_UPLOAD
#endif

/* include WMB RPM */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_WMB_INTERFACE
#undef  INCLUDE_HTTP_WMB_INTERFACE
#else
#define  INCLUDE_HTTP_WMB_INTERFACE
#endif

/* include form post RPM */
#ifndef _WRS_CONFIG_WEBCLI_INCLUDE_HTTP_POST
#undef  INCLUDE_HTTP_POST
#else
#define  INCLUDE_HTTP_POST
#endif

extern char * httpServerName;                   /* name of your www server */
extern char * global_HTTP_BASIC_REALM;          /* realm for basic authentication */
extern char * httpServerIdentification;         /* server identification string */
extern char * httpUri;                          /* HTTP URI selection */

/* socket configuration */
extern unsigned short httpPort;                        /* TCP port for HTTP requests */
extern int httpMaxSockBuff;                            /* Size of socket send/receive buffer */
extern unsigned short httpMaxPorts;
extern unsigned short httpMaxConnections;
extern unsigned short httpSockTimeout;                 /* timeout (secs) for HTTP socket */
extern unsigned short httpHeaderTimeout;               /* timeout (secs) for HTTP header */

/* memory configuration */
extern unsigned long  httpGlobalMemPartSize;       /* # bytes of the global memory pool */
extern unsigned long  httpReqMemPartSize;          /* # bytes of the request memory pool */
extern unsigned long  httpMaxUrlSize;
extern unsigned long  httpMaxTempBufSize;           /* # bytes of a request line (pool) */
extern unsigned short httpMaxArguments;            /* # of arguments in a get request */
extern unsigned short httpMaxMimeLines;            /* Maximum lines of incoming/outgoing HTTP header */
extern unsigned short httpMaxSendCookies;          /* # of cookies to send per request */
extern unsigned short httpMaxRecCookies;           /* # of cookies to receive per request */

/* task configuration */
extern unsigned short httpPriority;               /* Priority of the daemon process */
extern unsigned short httpMaxProcesses;           /* Number of daemon processes */
extern long           httpdTaskStackSize;         /* Size of the stack of every process */

/*
 * list configuration
 * specify the max number of entries in several configuration tables
 */
extern short global_HTTP_SYSRPM_CONF_MAX;          /* max # of system RPMs */
extern unsigned short global_HTTP_RPM_CONF_MAX;             /* max # of user RPMs per HTTP method */
extern unsigned short global_HTTP_RPM_DISP_MAX;             /* max # of supported HTTP methods */
extern unsigned short global_HTTP_MIMEPARSE_CONF_MAX;       /* max # of MIME parsing functions */
extern unsigned short global_HTTP_ALIAS_CONF_MAX;           /* max # of alias definitions */
extern unsigned short global_HTTP_LOCATION_CONF_MAX;        /* max # of location groups */
extern unsigned short global_HTTP_LOCATION_NAME_SIZE;       /* max size of a location group string */
extern unsigned short global_HTTP_IP_PER_LOCATION_MAX;      /* max # of IP patterns per location group */
extern unsigned short global_HTTP_IP_PATTERN_SIZE;          /* max size of an IP pattern */
extern unsigned short global_HTTP_GID_CONF_MAX;             /* max # of user groups */
extern unsigned short global_HTTP_GID_NAME_SIZE;            /* max size of a user group string */
extern unsigned short global_HTTP_USER_PER_GID;             /* max # of users per user group */
extern unsigned short global_HTTP_USERNAME_SIZE;            /* max size of a user name */
extern unsigned short global_HTTP_PASSWORD_SIZE;            /* max size of a user password */
extern unsigned short global_HTTP_CTRL_CONF_MAX;            /* max # of URL paths for access control */
extern unsigned short global_HTTP_MEMBERS_PER_CTRL;         /* max # of access control clauses per URL path */
extern unsigned short global_HTTP_CTRL_URL_SIZE;            /* max size of an access control statement */
extern unsigned short global_HTTP_CGI_FN_CONF_MAX;          /* max # of CGI routines */
extern unsigned short global_HTTP_UPLOAD_CONF_MAX;          /* max # of path definitions for file upload */
extern unsigned short global_HTTP_FS_CONF_MAX;              /* max # of file system mounting points */
extern unsigned short global_HTTP_WMB_PATHS_MAX;            /* max # of WMB file system mounting points */
extern unsigned short global_HTTP_FILE_REGISTRY_MAX;        /* max # of special files registered with the server */
extern unsigned short  kMacroRepeatNestMax;               /* max # of nested depth for Repeat Macro */
extern char       * kUniqueSignature;
extern unsigned short globle_HTTP_DEBUG;

/* user implemented functions */
extern void   (*phttpWindMarkSetFailed) (void * reqId,char * windMark, unsigned short errnoVal);
extern char * (*phttpWindMarkGetFailed) (void * reqId, char * windMark,unsigned short errnoVal);
extern void   (*phttpPostReply) (void * reqId, short status);
#ifdef __JAVASCRIPT_DIGEST_ENABLED__ 
extern void* (*pwmGetAuthenticationLevelPassword) (void *envVar,char *pUserName,char *pPassword);
extern void  (*pwmStoreUserData)(char *chBuffer);
#endif

#endif /* __HTTP_CFG_H_HEADER__ */
