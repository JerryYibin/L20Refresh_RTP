/* httpFileRegistry.h - header file for the file registry */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01d,026jan10,h_y  support for 2010 
01c,02sep04,dkg  added read, write access level field to RegDes structure
                 and new API's to set & get file access level to support
                 JavaScript Authentication feature 
01b,28apr04,vmk  added customTimeGet funtion pointer (SPR#92408)
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __REGISTRY_HEADER__
#define __REGISTRY_HEADER__

#define     kREGISTRY_Disabled  0
#define     kREGISTRY_RawData   1
#define     kREGISTRY_HtmlDoc   2

/* datatypes & structures */

typedef struct  RegDes                      /* (basename:  reg) */
{
    sbyte*             pFileName;
    enum fs_e          fs;
    sbyte*             contentType;
    void*              (*customOpen) (void * , void *);
    unsigned long      (*customRead) (void * , unsigned long, unsigned long, void *);
    unsigned long      (*customWrite) (void * , unsigned long, unsigned long, void *);
    unsigned long      (*customSeek)  (void *,  long , long);
    unsigned long      (*customTimeGet)  (void *,  sbyte* , void *);
    unsigned long      (*customClose) (void *);
    unsigned long      (*customSize)  (void *);
    Access             readAccess;    /* read access */
    Access             writeAccess;   /* write access */

} RegDes;                                   /* aka Registered File */

/* prototypes */

extern  STATUS  httpFileRegInit                 (ubyte2 maxEntries);
extern  STATUS  httpFileRegCleanup              (void);
extern  STATUS  httpFileRegister
    (
    sbyte              *pFileName,
    enum fs_e          fs,
    sbyte *            contentType,
    void *             (*customOpen)  (void * , void *),
    unsigned long      (*customRead)  (void * , unsigned long, unsigned long, void *),
    unsigned long      (*customWrite) (void * , unsigned long, unsigned long, void *),
    unsigned long      (*customSeek)  (void *,  long , long),
    unsigned long      (*customTimeGet)  (void *,  sbyte* , void *),
    unsigned long      (*customClose) (void *),
    unsigned long      (*customSize)  (void *)

    );


extern  RegDes* httpFileRegHandle (sbyte *pFileName);
extern  STATUS  httpFileRegGetFd  (RegDes *ph_regObject,generic_fd_t * fd);
extern  STATUS  httpFileRegRemove (sbyte *pFileName);

extern  STATUS  httpFileAccessSet
    (
    char    *fileName,    /* name of file */
    Access  readAccess,   /* read access */
    Access  writeAccess   /* write access */
    );
extern  STATUS  httpFileAccessGet
    (
    char    *fileName,     /* name of file */
    Access  *readAccess,   /* pointer to get read access */
    Access  *writeAccess   /* pointer to get write access */
    );
extern void REGISTRY_ReaderEnter();
extern void REGISTRY_ReaderExit();
extern Counter REGISTRY_PrivGetNumReaders
	(
	void
	);

#endif /* __REGISTRY_HEADER__ */
