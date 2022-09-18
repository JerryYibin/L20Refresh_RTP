/* httpUtil.c - HTTP server utility functions header */

/* Copyright 1997-1999,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,2feb10,h_y   support for LP64  
01b,09sep99,csr  merging for zlib
01b,09jun99,km   merge of Wind Web Server 2.0 code drop.
01a,15may97,mb   written.
*/

#ifndef __INChttpUtilh
#define __INChttpUtilh
 
#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */

char *       httpOsalStringToken (char * string,char * separators,char ** ppLast);
HTTP_STATUS  httpOsalStringError (long errcode,char * buffer);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpUtilh */

