/* httpFileMgr.h - file manager header */

/*
 * Copyright (c) 2003-2008,2010-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
02jul14,r_w  support Etag on NVM file system US32703
07jun11,m_z  add support functions for etag/if-modified-since
             CQID: WIND00203773
2feb10,h_y   support for LP64 
12oct05,wdz  '#ifdef __cplusplus' statement added
             for SPR#98444
26apr04,vmk  Adding pfTime Get to retrieve modification time ( SPR#92408)
14jan04,gsr  cleanup routine for SPR 93009
15dec03,adb  merged in SPR 90310 fix
*/

#ifndef __INCfilemngrh
#define __INCfilemngrh

#include <stdio.h>

/* 'zlib.h' should either be the original file or the WindRiver modified */
#include "zlib.h"
#include "httpMemGzio.h"

enum fs_e
    {
    ALL_FS,
    MEM_FS,       /* regular file access, from MEM FS     */
    CMP_MEM_FS,   /* gzip/ungzip, from MEM FS             */
    ANSI_FS,      /* regular file access, from ANSI FS    */
    CMP_ANSI_FS,  /* gzip/ungzip, from ANSI FS            */
    CUSTOM_FS
    };

typedef union fd_u_tag
    {
    void *          anyFd;
    gzFile          cmpAnsiFd;
    FILE *          ansiFd;
    void *          memFd;
    memGzFile_t *   cmpMemFd;
    void *          customFd;
    } fd_u;

typedef void * (*custom_fopen_t) (void * , void *);

typedef ubyte4 (*custom_fread_t) (void * , ubyte4, ubyte4, void *);

typedef ubyte4 (*custom_fwrite_t) (void * , ubyte4, ubyte4, void *);

typedef ubyte4 (*custom_fseek_t) (void *,  sbyte4 , sbyte4);

typedef ubyte4 (*custom_ftimeget_t) ( void *, sbyte *, void * ); 

typedef ubyte4 (*custom_fclose_t)  (void *);

typedef ubyte4 (*custom_fsize_t) (void *);

typedef sbyte * (*custom_fgets_t) (sbyte *, ubyte4, void *);

typedef struct generic_fd_t_tag
    {
    fd_u                fd;       /* the specific filesystem file descriptor */
    enum fs_e           fs;       /* the filesystem                          */
    ubyte4              size;     /* the size of the (uncompressed) file     */
    ubyte4              cmpSize;  /* the size of the compressed file (or 0)  */
    sbyte *             mimeType; /* the MIME type for this file             */
    ubyte4              offset;   /* used to implement fseek in mem fs       */
    custom_fopen_t      pfOpen;   /* function ptr to custom fopen            */
    custom_fread_t      pfRead;   /* function ptr to custom fread            */
    custom_fwrite_t     pfWrite;  /* function ptr to custom fwrite           */
    custom_fgets_t      pfGets;   /* function ptr to custom fgets            */
    custom_fclose_t     pfClose;  /* function ptr to custom fclose           */
    custom_fseek_t      pfSeek;   /* function ptr to custom fseek            */
    custom_ftimeget_t   pfTimeGet; /* function ptr to custom get modification time */
    custom_fsize_t      pfSize;   /* function ptr to custom fsize            */
    } generic_fd_t;
    
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

STATUS httpFileMngrInit (void);
STATUS httpFileMngrCleanup (void);

STATUS httpFileOpen 
    (
    sbyte *         szName, 
    sbyte *         mode, 
    generic_fd_t *  f
    );

STATUS httpFileRead
    (
    generic_fd_t *  f,
    sbyte *         cpBuffer,
    ubyte4          lMaxbytes,
    ubyte4 *        pBytesRead
    );

STATUS httpFileWrite
    (
    generic_fd_t *  f,
    const sbyte *   cpBuffer,
    sbyte4          lNbytes,
    ubyte4 *        bytesWritten
    );

STATUS httpFileGets
    (
    generic_fd_t * f,
    sbyte *        cpBuffer,
    sbyte4         lNbytes
    );

STATUS httpFileSeek
    (
    generic_fd_t * f, 
    ubyte4 offset
    );

STATUS httpFileClose 
    (
    generic_fd_t * f
    );

STATUS httpFileDelete 
    (
    sbyte * szName, 
    enum fs_e fs
    );

ssize_t httpFileSize 
    (
    generic_fd_t * f
    );

sbyte * httpFileContentTypeGet 
    (
    generic_fd_t * f
    );

void httpFileSetDefaultFs 
    (
    enum fs_e defaultFs
    );

STATUS httpFileTimeGet
    (
    generic_fd_t * f,
    sbyte *        szName,
    sbyte *        szDateBuffer
    );

STATUS  httpFileBiTimeGet
    (
    generic_fd_t * f,
    sbyte *        szName,
    unsigned long * mtime
    );
 
enum fs_e httpFileGetDefaultFs (void);

extern void   (*pFILEMGR_Construct)(void);
extern void * (*pFILEMGR_RetrieveFile)(int *, void *);
extern void   (*pFILEMGR_ReleaseFile)(void *);
extern void   (*pFILEMGR_DeConstruct)(void);
extern STATUS (*pMemGzRead)(memGzFile_t *,sbyte *,ubyte4 *);
extern STATUS (*pMemGzOpen)(sbyte *,memGzFile_t **,ubyte4 *);
extern STATUS (*pMemGzClose)(memGzFile_t *);
extern STATUS (*pMemGzSeek)(memGzFile_t *,ubyte4);

#ifdef __cplusplus
/* extern "C" */
}
#endif /* __cplusplus */

#endif /* __INCfilemngrh */
