/* httpFileProcess.h - parse file for WindMarks and send output to a socket */

/* Copyright (c) 2003-2004,2010,2012 Wind River Systems, Inc. */

#include "copyright_wrs.h"

/*
modification history
-------------------- 
02b,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
02a,02feb10,h_y  support for LP64 
01d,16feb06,ps   changes for webcli on linux
01c,27apr04,adb  replaced all buffer size constants with kMagicMarkupBufferSize
01b,06feb04,dkg  Removed declaration of fileSendChunkProcess() and 
                 #define TMP_BUF_SIZE with reference to SPR 91304
01a,14aug03,adb  added to source control
*/

#ifndef __INChttpFileProcessh
#define __INChttpFileProcessh

#include "wm.h"
#include "wmos.h"
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"
#include "wmbLockMgrLib.h"
#include "wmbDBLib.h"
#include "wmbSessionLib.h"
#endif
#include "httpFileMgr.h"
#include "httpFileRegistry.h"
#include "httpReq.h"
#include "httpLib.h"

#ifndef WEBCLI_ON_LINUX
# include "pipeDrv.h"
# include "ioLib.h"
# include "memLib.h"
#endif

#include "stdlib.h"
#include "memPoolLib.h"
#include "httpRpms.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

#define MAX_READ_BYTES         kMagicMarkupBufferSize 
                               /* maximum windMark substitution buffer */
#define MAX_FILE_INCLUDE_LEVEL 8                  
                               /* maximum number of recursive inclusions */

/* Function Prototypes */

/* fileSend - computes file size, allocates storage and calls fileSendProcess */
#ifndef WEBCLI_STANDALONE_WEBSERVER
STATUS fileSend 
    (
    WMB_SESSION_T * pSession, 
    generic_fd_t *  pFileDescriptor
    );

/* fileSendProcess - reads file and passes the data to fileSendChunkProcess */

STATUS fileSendProcess 
    (
    STATUS (*writeFctPtr)(void *, sbyte *, ubyte4), 
    void *          writeArg, 
    WMB_SESSION_T * pSession, 
    generic_fd_t  * pFileDescriptor, 
    size_t          fileSize,
    sbyte *         auxBuffer, 
    size_t          auxBufferSize
    );
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INChttpFileProcessh */
