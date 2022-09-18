/* httpHash.h - HTTP server hash table package header */

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
01e,26jan10,h_y  support for 2010 
01d,09sep99,csr  merging for zlib
01c,09jun99,km   merge of Wind Web Server 2.0 code.
01b,14may97,mb   osal introduced.
01a,23apr97,ics  merged 3Soft source into vxWorks.
*/

#ifndef __INChttpHashh
#define __INChttpHashh

/* includes */

#include "memPoolLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

/* forward-declare hash_table... */
struct hash_table;
typedef struct hash_table * HTTP_HASH_TABLE_ID;

/* ... so we can declare HASH_FUNC: the type of a hashing function. */
typedef unsigned long (*HTTP_HASH_FUNC)(HTTP_HASH_TABLE_ID hashTabId , char *);

/* hash table entry structure */
typedef struct hash
    {
    char *         szKey;
    void *         pData;
    } HTTP_HASH;


/* hash table structure */
typedef struct hash_table
    {
    unsigned long    ulMax;
    unsigned long    sHashOffset;
    HTTP_HASH_FUNC   pHashFunc;
    MEM_POOL_ID 	 memPartId;
    HTTP_HASH *      pTable;
    } HTTP_HASH_TABLE;

/* function declarations */

HTTP_STATUS        httpHashInit   (void);
HTTP_HASH_TABLE_ID httpHashCreate (MEM_POOL_ID memPartId, long ulEntries , HTTP_HASH_FUNC pHashFunc , unsigned long sHashOffset);
HTTP_STATUS        httpHashAdd    (HTTP_HASH_TABLE_ID hashTabId, char * szKey, void * pData);
void *             httpHashGet    (HTTP_HASH_TABLE_ID hashTabId , char * szKey );
char *             httpHashIterator (HTTP_HASH_TABLE_ID hashTabId , unsigned long * pulIndex);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpHashh */

