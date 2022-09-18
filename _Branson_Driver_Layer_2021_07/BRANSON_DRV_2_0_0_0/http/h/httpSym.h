/* httpSymLib.h - HTTP server symbol table package header */

/* Copyright (c) 1998-1999,2010,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------- 
01d,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01c,26jan10,h_y  support for 2010 
01b,21dec99,csr  reverting to old API of httpSymTabAdd()
01a,03may99,mb   written.
*/

#ifndef __INChttpSymLibh
#define __INChttpSymLibh

/* includes */

#include "httpType.h"
#include "memPoolLib.h"
#include "httpTcp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef HTTP_GEN_LIST_ENTRY_ID     HTTP_SYM_TYPE_ID;
typedef HTTP_SYM_TYPE_ID           HTTP_SYM_TYPE;
typedef struct  http_sym  HTTP_SYM;
typedef HTTP_SYM *        HTTP_SYM_ID;
typedef unsigned short    HTTP_SYM_ACCESS_ID;
typedef HTTP_STATUS (* HTTP_SYM_FUNC)(HTTP_SYM_ID symId, void * pFunc);

/*
struct http_live     subject to change
    {
      HTTP_TIME maxUpdateFreq;
    };

typedef struct http_live HTTP_LIVE;
typedef HTTP_LIVE *      HTTP_LIVE_ID;
*/

struct http_sym     /* subject to change */ 
    {
    void *             pAddr;
    char *             szName;
    HTTP_SYM_TYPE_ID   typeId;
    unsigned long      ulNumItems;
    unsigned long      ulMaxSize;
    HTTP_SYM_FUNC      pGetFunc;
    HTTP_SYM_FUNC      pSetFunc;
    void *             pGetUserArg;
    void *             pSetUserArg;
    HTTP_SYM_ACCESS_ID accessId;
    HTTP_SEM_ID        semId;
    char *             szDesc;
    /* HTTP_LIVE_ID       liveId; */
    } ;

typedef struct http_sym_tab HTTP_SYM_TAB;
typedef HTTP_SYM_TAB *      HTTP_SYM_TAB_ID;

/* generic function to determine a symbol types storage size in bytes */
typedef unsigned long (* HTTP_SYM_VAL_SIZE_HANDLER_FUNC)(HTTP_SYM_ID symId);

/* generic function to determine a symbol types string size in bytes */
typedef unsigned long (* HTTP_SYM_VAL_STRING_SIZE_HANDLER_FUNC)(HTTP_SYM_ID symId);

/* generic function to transform symbol value to string */
typedef char * (* HTTP_SYM_VAL_TO_STRING_HANDLER_FUNC)(HTTP_SYM_ID symId,unsigned long ulItem,char * szBuffer);

/* generic function to set the symbol value using strings */
typedef HTTP_BOOL (* HTTP_SYM_VAL_SET_BY_STRING_HANDLER_FUNC)(HTTP_SYM_ID symId,unsigned long ulItem,char * szValue);

/* generic function to print the symbol value using efficient encoding */
typedef HTTP_STATUS (* HTTP_SYM_VAL_TO_NET_PRINT_HANDLER_FUNC)(HTTP_OSAL_IO_HNDL_ID ioHandleId,HTTP_SYM_ID symId);

/* generic function to determine a symbol types network encoding size in bytes */
typedef unsigned long (* HTTP_SYM_VAL_NET_SIZE_HANDLER_FUNC)(HTTP_SYM_ID symId);

/* generic function to set the symbol value using strings */
typedef HTTP_BOOL (* HTTP_SYM_VAL_SET_BY_NET_HANDLER_FUNC)(HTTP_SYM_ID symId,char * szValue);

struct http_sym_type_handler
    {      /* subject to changes */
    char *                                   szSymType;
    HTTP_SYM_VAL_SIZE_HANDLER_FUNC           pSymValSize;
    HTTP_SYM_VAL_STRING_SIZE_HANDLER_FUNC    pSymValStringSize;
    HTTP_SYM_VAL_TO_STRING_HANDLER_FUNC      pSymValToString;
    HTTP_SYM_VAL_SET_BY_STRING_HANDLER_FUNC  pSymValSetByString;
    HTTP_SYM_VAL_TO_NET_PRINT_HANDLER_FUNC   pSymValToNetPrint;
    HTTP_SYM_VAL_NET_SIZE_HANDLER_FUNC       pSymValNetSize;
    HTTP_SYM_VAL_SET_BY_NET_HANDLER_FUNC     pSymValSetByNet;
    };
  
typedef struct http_sym_type_handler HTTP_SYM_TYPE_HANDLER;
typedef HTTP_SYM_TYPE_HANDLER      * HTTP_SYM_TYPE_HANDLER_ID;
typedef HTTP_STATUS (*HTTP_SYM_NOTIFY_FUNC) (HTTP_SYM_ID symId);

/* externals */

extern HTTP_SYM_TAB_ID  httpGlobalSymTabId;

/* defines */

#define HTTP_SYM_MAX_KEY_LENGTH 31 /* symbol names may have a size up to this */

/* permission codes for symbols */
#define HTTP_SYM_ACCESS_NONE    0                    /* do not grant any accesses to the symbol value */
#define HTTP_SYM_ACCESS_R       1                    /* symbol value may be read */
#define HTTP_SYM_ACCESS_W       2                    /* symbol value may be changed (written) */
#define HTTP_SYM_ACCESS_RW      (HTTP_SYM_ACCESS_R|HTTP_SYM_ACCESS_W)
#define HTTP_SYM_ACCESS_LIVE    4                    /* symbol may be used for LiveControl connections */
#define HTTP_SYM_ACCESS_DEF     HTTP_SYM_ACCESS_RW   /* global default permissions when symbol table is initialized */

/* options for httpSymTabAdd() */
#define HTTP_SYM_OPTION_USELOCKS  1     /* enable separate locking with own semaphore for each symbol */


#define HTTP_SYM_ASYNC          1  /* functions attached to a symbol should be executed asynchronously */
#define HTTP_SYM_SYNC           0  /* execute functions attached to a symbol synchronously */

/* symbol type handler ids */
#define HTTP_SYM_TYPE_CHAR_ID       httpSymTypeHandlerFind("char")
#define HTTP_SYM_TYPE_SHORT_ID      httpSymTypeHandlerFind("short")
#define HTTP_SYM_TYPE_LONG_ID       httpSymTypeHandlerFind("long")
#define HTTP_SYM_TYPE_INT_ID        httpSymTypeHandlerFind("long")

#define HTTP_SYM_TYPE_UCHAR_ID      httpSymTypeHandlerFind("uchar")
#define HTTP_SYM_TYPE_USHORT_ID     httpSymTypeHandlerFind("ushort")
#define HTTP_SYM_TYPE_ULONG_ID      httpSymTypeHandlerFind("ulong")
#define HTTP_SYM_TYPE_UINT_ID       httpSymTypeHandlerFind("ulong")

#define HTTP_SYM_TYPE_BOOL_ID       httpSymTypeHandlerFind("bool")
#define HTTP_SYM_TYPE_FLOAT_ID      httpSymTypeHandlerFind("float")
#define HTTP_SYM_TYPE_DOUBLE_ID     httpSymTypeHandlerFind("double")

#define HTTP_SYM_TYPE_STRING_ID     httpSymTypeHandlerFind("string")
#define HTTP_SYM_TYPE_GENLIST_ID    httpSymTypeHandlerFind("genlist")


/* #define HTTP_MAX_SYM_TYPE_SIZE 60 */       /* max size of a symbol type string */


/* prototypes */
HTTP_STATUS      httpSymTabInit (unsigned long ulMaxTypes);
HTTP_SYM_TAB_ID  httpSymTabCreate (MEM_POOL_ID  partId, unsigned long ulNumEntries, unsigned long ulOptions);
HTTP_SYM_ID      httpSymTabAdd ( HTTP_SYM_TAB_ID symTabId,char * szName, char * szDesc, HTTP_SYM_TYPE_ID typeId,unsigned long ulNumItems,void * pValue );
HTTP_SYM_ID      httpSymTabFindByNameAndType (HTTP_SYM_TAB_ID symTabId,char * szName,HTTP_SYM_TYPE_ID typeId);
HTTP_SYM_ID      httpSymTabFindByName (HTTP_SYM_TAB_ID symTabId,char * szName);
HTTP_SYM_ID      httpSymTabIterator ( HTTP_SYM_TAB_ID symTabId, unsigned long * ulIndex );
HTTP_BOOL        httpSymDescriptionSet (HTTP_SYM_ID symId,char *szDescription);
char *           httpSymDescriptionGet (HTTP_SYM_ID symId);
char *           httpSymNameGet ( HTTP_SYM_ID symId );
void *           httpSymValGet ( HTTP_SYM_ID symId );
HTTP_STATUS      httpSymValSet ( HTTP_SYM_ID symId , void * pValue );
void *           httpSymValSetByString ( HTTP_SYM_ID symId , char * szValue , unsigned long ulItem );
HTTP_SYM_TYPE_ID httpSymTypeGet ( HTTP_SYM_ID symId );
unsigned long    httpSymNumItemsGet ( HTTP_SYM_ID symId );
void             httpSymNumItemsSet ( HTTP_SYM_ID symId , unsigned long ulNumItems );
unsigned long    httpSymValSize ( HTTP_SYM_ID symId );
unsigned long    httpSymValStringSizeGet ( HTTP_SYM_ID symId );
void             httpSymValMaxSizeSet (HTTP_SYM_ID symId, unsigned long ulMaxSize);
unsigned long    httpSymValMaxSizeGet (HTTP_SYM_ID symId);
HTTP_BOOL        httpSymValCpy ( HTTP_SYM_ID symId , void * pCopy );
short            httpSymValCmp ( HTTP_SYM_ID symId , void * pLastValue );
char *           httpSymValToString (  HTTP_SYM_ID symId , char * szBuffer , unsigned long ulItem );
char *           httpSymValToStringCopy (HTTP_REQ_ID reqId, HTTP_SYM_ID symId, unsigned long ulItem);
char *           httpSymTypeToString ( HTTP_SYM_ID symId , char * szBuffer );
HTTP_SYM_TYPE_ID httpSymTypeHandlerFind (char * szType);
HTTP_SYM_TYPE_ID httpSymTypeHandlerAdd (char * szSymType, HTTP_SYM_VAL_SIZE_HANDLER_FUNC pSymValSize, HTTP_SYM_VAL_STRING_SIZE_HANDLER_FUNC pSymValStringSize, HTTP_SYM_VAL_TO_STRING_HANDLER_FUNC pSymValToString, HTTP_SYM_VAL_SET_BY_STRING_HANDLER_FUNC pSymValSetByString,HTTP_SYM_VAL_NET_SIZE_HANDLER_FUNC pSymValNetSize, HTTP_SYM_VAL_TO_NET_PRINT_HANDLER_FUNC pSymValToNetPrint,HTTP_SYM_VAL_SET_BY_NET_HANDLER_FUNC pSymValSetByNet);
HTTP_SYM_FUNC	 httpSymGetFuncGet (HTTP_SYM_ID symId);
void *		 httpSymGetArgGet (HTTP_SYM_ID symId);
void             httpSymGetFuncSet ( HTTP_SYM_ID symId , HTTP_SYM_FUNC symFunc , void * pUserArg , HTTP_BOOL fOwnTask );
HTTP_SYM_FUNC	 httpSymSetFuncGet (HTTP_SYM_ID symId);
void *		 httpSymSetArgGet (HTTP_SYM_ID symId);
void             httpSymSetFuncSet ( HTTP_SYM_ID symId , HTTP_SYM_FUNC symFunc , void * pUserArg , HTTP_BOOL fOwnTask );
void             httpSymFuncUnify ( HTTP_SYM_ID symId , HTTP_BOOL fSet );
void             httpSymAccessSet (HTTP_SYM_ID symId, HTTP_SYM_ACCESS_ID accessId);
HTTP_SYM_ACCESS_ID httpSymAccessGet (HTTP_SYM_ID symId);
HTTP_SYM_ACCESS_ID httpSymAccessDefGet(void);
void               httpSymAccessDefSet (HTTP_SYM_ACCESS_ID accessId);
HTTP_BOOL          httpSymAccessRead (HTTP_SYM_ID symId);
HTTP_BOOL          httpSymAccessWrite (HTTP_SYM_ID symId);
HTTP_BOOL          httpSymAccessLive (HTTP_SYM_ID symId);
void               httpSymNotifyFuncSet (HTTP_SYM_NOTIFY_FUNC pFunc);

HTTP_STATUS      httpSymLock (HTTP_SYM_ID symId);
HTTP_STATUS      httpSymUnlock (HTTP_SYM_ID symId);

HTTP_STATUS      httpSymTabLock (HTTP_SYM_TAB_ID symTabId);
HTTP_STATUS      httpSymTabUnlock (HTTP_SYM_TAB_ID symTabId);

HTTP_STATUS      httpEvaluate (HTTP_REQ_ID  reqId,HTTP_SYM_TAB_ID symTabId);
HTTP_GEN_LIST_ID httpEvaluateConfInit (unsigned short usTabMax);
HTTP_STATUS      httpEvaluateConfAdd (char * szName);


/* init functions of built-in symbol type handlers */
HTTP_STATUS httpSymTypeCharInit(void);
HTTP_STATUS httpSymTypeBoolInit(void);
HTTP_STATUS httpSymTypeFloatInit(void);
HTTP_STATUS httpSymTypeStringInit(void);
HTTP_STATUS httpSymTypeShortInit(void);
HTTP_STATUS httpSymTypeUShortInit(void);
HTTP_STATUS httpSymTypeLongInit(void);
HTTP_STATUS httpSymTypeULongInit(void);
HTTP_STATUS httpSymTypeGenListInit(void);


#ifdef __cplusplus
}
#endif

#endif /* __INChttpSymLibh */

