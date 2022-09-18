/* httpMimeEtag.h - HTTP Etag functionality header */

/* Copyright 2011 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01a,01jun11,m_z written.
                CQID: WIND00203773
*/

#ifndef __INChttpEtagh
#define __INChttpEtagh

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */
typedef struct http_etag_binary
    {
    unsigned long file;     /* file id for etag */
    unsigned long size;     /* file size */
    unsigned long time;     /* file modification time */
    } HTTP_ETAG_BINARY;     /* consisting of file+time */


/* function declarations */
#define HTTP_ETAG_MAX_LEN  80 

STATUS httpEtagInit (unsigned short usMax);
STATUS httpEtagDestroy ();
STATUS httpEtagFileTestAndSet (char* fileName, unsigned long* pEntryId);
STATUS httpEtagFileRemove (char* fileName);
STATUS httpEtagValueSet (HTTP_ETAG_BINARY * src, char* dst);
STATUS httpEtagValueGet (const char * src, HTTP_ETAG_BINARY *dst);
    
#ifdef __cplusplus
}
#endif

#endif /* __INChttpEtagh */
