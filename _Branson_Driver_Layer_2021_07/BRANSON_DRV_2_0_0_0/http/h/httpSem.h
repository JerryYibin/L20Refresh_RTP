/* httpSem.h - HTTP server semaphores header */

/* Copyright 1997-1999,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,026jan10,h_y  support for 2010 
01b,09sep99,csr  merging for zlib
01b,09jun99,km   merge of Wind Web Server 2.0 code.
01a,14may97,mb   written.
*/

#ifndef __INChttpSemh
#define __INChttpSemh

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef OS_SEM_T  HTTP_SEM_ID;

/* defines */

#define HTTP_SEM_Q_PRIORITY   1

/* function declarations */
HTTP_STATUS httpOsalSemInit    (void);
HTTP_STATUS httpOsalSemTake    (HTTP_SEM_ID semId);
HTTP_STATUS httpOsalSemGive    (HTTP_SEM_ID semId);
HTTP_SEM_ID httpOsalSemMCreate (long options);
HTTP_STATUS httpOsalSemDelete  (HTTP_SEM_ID semId);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpSemh */

