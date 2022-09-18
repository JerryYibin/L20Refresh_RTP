/* httpAuth.h */

/* Copyright(c) 2005-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01d,18apr12,r_w  add some routines
                 Requirement: WIND00299859
01c,29jan10,m_z  VxWorks 64-bit support
                 (WIND00193649)
01b,25apr05,???  Get #define for JavaScript Authentication from wm_options.h,
                 since these files are in the common folder.
01a,17sep04,dkg  ported from RC3.3 to WM4.4 for JavaScript Authentication
*/


#ifndef __AUTH_H__
#define __AUTH_H__


#include "wmMD5.h"
#include "httpFileMgr.h"
#include "httpFileRegistry.h"


#define kMaxNonceLen        32

#ifdef __cplusplus
extern "C" {
#endif

extern STATUS wmAUTHConstruct ( void );
extern WM_ACCESS_T * wmAUTHCheckAuthentication ( WMB_SESSION_T *p_envVar);
extern sbyte *wmAUTHGetSentNonce ( sbyte4 clientIndex );
extern sbyte *wmAUTHGetClientCookie ( sbyte4 clientIndex );
extern void wmAUTHLogout(WMB_SESSION_T *p_envVar);
extern void wmAUTHLogoutAll(int flag);
extern STATUS CgiLookup(WMB_SESSION_T *p_envVar, char *SearchArgument, char *pRetBuf, unsigned long retBufLen);
extern sbyte* ENVIRONMENT_GetEnvironment(WMB_SESSION_T *p_envVar,	Counter index);	

/* following is moved from retrofit.h */
#if 0
#ifdef __HTTP_ENABLED__


IMPORT STATUS CgiLookup
	(
	environment *p_envVar,	/* The WMB session == RC Environment structure */
	char *SearchArgument,	/* name to look for */
	char *pRetBuf,			/* buffer to place value */
	unsigned long retBufLen	/* size of the passed value buffer */
	);

IMPORT STATUS WCTOOLS_ParseQueryString
	(
	environment *pEnv, 		/* The WMB session == RC Environment structure */
	char *SearchArgument,	/* name to look for */
    char *pReturnBuffer, 	/* buffer to place value */
    unsigned long ReturnBufferLen		/* size of the passed value buffer */
    );


IMPORT sbyte* ENVIRONMENT_GetEnvironment
	(
	environment *p_envVar,	/* The WMB session == RC Environment structure */
	Counter      index		/* Index of requested HTTP variable */
	);

IMPORT RLSTATUS WCTOOLS_RetrieveArgument(
	char *pArgString,		/* source argument string */
	long    Arg,				/* index of argument requested */
    char *pArgRetBuf,		/* buffer for argument value */
    long    ArgRetBufLen		/* size of passed argument value buffer */
    );

IMPORT RLSTATUS RESPONSE_SendResource
	(
	OS_SPECIFIC_SOCKET_HANDLE sock, /* IGNORED - WM gets the socket from the session */
    environment *p_envVar,			/* The WMB session == RC Environment structure */
    char *Resource					/* File name to be sent */
    );

IMPORT RLSTATUS RESPONSE_SendMessage
	(
	OS_SPECIFIC_SOCKET_HANDLE sock,	/* IGNORED - WM gets the socket from the session */
    environment *p_envVar,			/* The WMB session == RC Environment structure */
    char *pMessage,					/* message to be sent */
    #ifdef _WRS_CONFIG_LP64
    sbyte8 messageLen				/* length of message */
    #else
    sbyte4 messageLen				/* length of message */
    #endif
    );

IMPORT RLSTATUS RESPONSE_RedirectReply
    (
    environment *p_envVar,	/* The WMB session == RC Environment structure */
    char *Resource			/* File name to be sent */
    );

/* RC Indexing support API */

IMPORT STATUS RC_MACRO_INIT
	(
    WMB_SESSION_T * pSession	/* The WMB session == RC Environment structure */
    );

IMPORT STATUS RC_MACRO_CLEANUP
	(
    WMB_SESSION_T * pSession	/* The WMB session == RC Environment structure */
    );

IMPORT STATUS RC_MACRO_PUSH
	(
    WMB_SESSION_T * pSession,	/* The WMB session == RC Environment structure */
	int bPushStatus
	);

IMPORT STATUS RC_MACRO_DONE_REPEAT
	(
    WMB_SESSION_T * pSession,	/* The WMB session == RC Environment structure */
	int bPushStatus				/* indicates whether REPEAT mechanism hit end of table */
	);

IMPORT int MACRO_GetIndexEx
	(
	WMB_SESSION_T *pSession,	/* The WMB session == RC Environment structure */
	int nest_level,				/* Nest level for the requested index */
	sbyte *pArgs				/* Name to search against for index nest level */
	);

IMPORT int MACRO_GetIndex
	(
	WMB_SESSION_T *pSession	/* The WMB session == RC Environment structure */
	);

#endif /* __HTTP_ENABLED__ */
#endif

#ifdef __cplusplus
}
#endif

#endif
