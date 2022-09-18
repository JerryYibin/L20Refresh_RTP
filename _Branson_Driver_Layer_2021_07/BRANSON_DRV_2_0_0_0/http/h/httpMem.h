/* httpMemPartLib.h -  HTTP server memory pool management header */

/* Copyright (c) 1997-1999,2010,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------- 
01f,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01e,026jan10,h_y  support for 2010 
01d,09sep99,csr  merging for zlib
01c,09jun99,km   merge of Wind Web Server 2.0 code.
01b,15may97,mb   osal introduced.
01a,03mar97,ck   written.
*/

#ifndef __INChttpMemPartLibh
#define __INChttpMemPartLibh

/* includes */

#include "httpType.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

#ifndef DEFINED_HTTP_MEM_PART_DESC
#define DEFINED_HTTP_MEM_PART_DESC

/*
 * Default (opaque) definition of MEM_POOL_ID; include
 * http/private/httpMemP.h for the full definition.
 */

typedef struct http_mem_part_desc  HTTP_MEM_PART_DESC;
typedef HTTP_MEM_PART_DESC *       MEM_POOL_ID;
#endif /* DEFINED_HTTP_MEM_PART_DESC */

/* function declarations */

MEM_POOL_ID memPoolCreate  (char * pPool, unsigned long ulPoolSize);

HTTP_STATUS      memPoolCleanup (MEM_POOL_ID partId);

HTTP_STATUS      httpMemPartCheck   (MEM_POOL_ID partId);

char *           httpMemPartDelete  (MEM_POOL_ID partId);

void *           memPoolAlloc   (MEM_POOL_ID partId, unsigned long ulBytes);
     
HTTP_STATUS      memPoolFree    (MEM_POOL_ID partId, void * pBlock);

unsigned long    memPoolAvail   (MEM_POOL_ID partId, short sMode);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpMemPartLibh */

