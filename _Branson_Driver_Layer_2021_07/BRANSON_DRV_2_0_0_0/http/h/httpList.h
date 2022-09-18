/* httpList.h - HTTP server generic list header */

/* Copyright (c) 1997-1999,2010,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------- 
01b,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01a,26jan10,h_y  support for 2010 
02d,09sep99,csr  merging for zlib
02c,09jun99,km   merge of Wind Web Server 2.0 code.
02b,03dec98,mb   httpGenListCaseFind() added.
02a,13may97,mb   osal introduced.
01a,25feb97,mb   written.
*/

#ifndef __INChttpListh
#define __INChttpListh

/* includes */

#include <stdio.h>
#include "memPoolLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef union http_gen_list_data
    {
	HTTP_FUNCPTR pFunc;
	char *       szData;
	void *       pData;
    } HTTP_GEN_LIST_DATA;

typedef struct http_gen_list_entry
    {
	char *                       szId;
	struct http_gen_list_entry * nextId;
	HTTP_GEN_LIST_DATA           listItem;
    } HTTP_GEN_LIST_ENTRY;

typedef HTTP_GEN_LIST_ENTRY * HTTP_GEN_LIST_ENTRY_ID;

typedef struct http_gen_list
    {
	unsigned short         ulMax;
	unsigned short         ulUsed;
	HTTP_GEN_LIST_ENTRY_ID firstId;
	MEM_POOL_ID       memPartId;
	HTTP_SEM_ID            semLock;
	HTTP_GEN_LIST_ENTRY *  pData;
    } HTTP_GEN_LIST;

typedef HTTP_GEN_LIST * HTTP_GEN_LIST_ID;

/* functions */

HTTP_STATUS            httpGenListLock ( HTTP_GEN_LIST_ID listId );
HTTP_STATUS            httpGenListUnlock ( HTTP_GEN_LIST_ID listId );

HTTP_GEN_LIST_ID       httpGenListCreate (MEM_POOL_ID memPartId,  unsigned short ulMaxItems , HTTP_BOOL fUseSemaphore);
HTTP_GEN_LIST_ENTRY_ID httpGenListFind ( HTTP_GEN_LIST_ID listId , char * szKey );
HTTP_GEN_LIST_ENTRY_ID httpGenListCaseFind ( HTTP_GEN_LIST_ID listId , char * szKey );
HTTP_GEN_LIST_ENTRY_ID httpGenListFuncAppend ( HTTP_GEN_LIST_ID listId , char * szId , HTTP_FUNCPTR pFunc );
HTTP_FUNCPTR           httpGenListFuncGet ( HTTP_GEN_LIST_ENTRY_ID entryId );
HTTP_GEN_LIST_ENTRY_ID httpGenListDataAppend ( HTTP_GEN_LIST_ID listId , char * szId , void * pData );
void *                 httpGenListDataGet ( HTTP_GEN_LIST_ENTRY_ID entryId );
char *                 httpGenListIdGet ( HTTP_GEN_LIST_ENTRY_ID entryId );
void                   httpGenListRemove ( HTTP_GEN_LIST_ID listId , HTTP_GEN_LIST_ENTRY_ID entryId );
void                   httpGenListDestroy ( HTTP_GEN_LIST_ID listId );
HTTP_GEN_LIST_ENTRY_ID httpGenListIterator ( HTTP_GEN_LIST_ID listId , HTTP_GEN_LIST_ENTRY_ID * pEntryId );
HTTP_GEN_LIST_ENTRY_ID httpGenListIndexIterator (HTTP_GEN_LIST_ID listId,unsigned short * pusIndex);
unsigned short         httpGenListItemsGet ( HTTP_GEN_LIST_ID listId );
MEM_POOL_ID       httpGenListMemPartIdGet (HTTP_GEN_LIST_ID listId);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpListh */

