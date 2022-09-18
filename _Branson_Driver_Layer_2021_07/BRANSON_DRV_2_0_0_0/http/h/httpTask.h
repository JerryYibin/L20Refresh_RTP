/* httpTask.h - HTTP server task functionality header */

/* Copyright 1997-1999,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,026jan10,h_y  support for 2010 
01b,09sep99,csr  merging for zlib
01b,09jun99,km   merge of Wind Web Server 2.0 code.
01a,14may97,mb   written.
*/

#ifndef __INChttpTaskh
#define __INChttpTaskh

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef long HTTP_OSAL_TASK_ID;

/* function declarations */

HTTP_STATUS  httpOsalTaskInit   (void); 
HTTP_OSAL_TASK_ID httpOsalTaskCreate (char *name,long priority,long stacksize,HTTP_FUNCPTR func,long arg1);
HTTP_STATUS  httpOsalTaskPrioritySet ( long priority,HTTP_OSAL_TASK_ID tid);
long httpOsalTaskIdSelf (void);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpTaskh */

