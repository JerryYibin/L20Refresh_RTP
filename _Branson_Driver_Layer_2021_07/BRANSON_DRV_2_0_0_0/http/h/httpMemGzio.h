/* memgzio.h -  header file to memgzio.c */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,026jan10,h_y  support for 2010 
01b,12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INCmemgzioh
#define __INCmemgzioh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef struct memGzFile_t_tag {
    z_stream stream;
    ubyte4   crc;      /* crc32 of uncompressed data */
    sbyte *  startpos; /* start of compressed data in file (header skipped) */
    ubyte4   zstrsize; /* size of the zstream (compressed file w/out header)*/
    ubyte4   cmpSize;  /* compressed size                                   */
    ubyte4   size;     /* uncompressed size                                 */
    ubyte4   savedCrc; /* crc value stored in the gzip file                 */
} memGzFile_t;

STATUS memGzOpen
    (
    sbyte * filename,
    memGzFile_t **  pFile,
    ubyte4 * size
    );

STATUS memGzRead
    (
    memGzFile_t * pFile,
    sbyte * pDecompBuf,
    ubyte4 * pDecompSize
    );

STATUS memGzClose (memGzFile_t * pFile);

STATUS memGzSeek (memGzFile_t * pFile, ubyte4 offset);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INCmemgzioh */
