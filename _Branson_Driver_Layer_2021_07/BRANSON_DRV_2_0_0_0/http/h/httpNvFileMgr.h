/* rc_filemgr.h */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,026jan10,h_y  support for 2010 
01b,12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history
*/

/*----------------------------------------------------------------------
 *
 * NAME CHANGE NOTICE:
 *
 * On May 11th, 1999, Rapid Logic changed its corporate naming scheme.
 * The changes are as follows:
 *
 *      OLD NAME                        NEW NAME
 *
 *      OpenControl                     RapidControl
 *      WebControl                      RapidControl for Web
 *      JavaControl                     RapidControl for Applets
 *      MIBway                          MIBway for RapidControl
 *
 *      OpenControl Backplane (OCB)     RapidControl Backplane (RCB)
 *      OpenControl Protocol (OCP)      RapidControl Protocol (RCP)
 *      MagicMarkup                     RapidMark
 *
 * The source code portion of our product family -- of which this file
 * is a member -- will fully reflect this new naming scheme in an upcoming
 * release.
 *
 *
 * RapidControl, RapidControl for Web, RapidControl Backplane,
 * RapidControl for Applets, MIBway, RapidControl Protocol, and
 * RapidMark are trademarks of Rapid Logic, Inc.  All rights reserved.
 *
 */




#ifndef __FILEMGR_HEADER__
#define __FILEMGR_HEADER__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* prototypes */
extern void     FILEMGR_Construct();
#ifdef __cplusplus
extern void * FILEMGR_RetrieveFile(int *, sbyte *);
#else
extern void*    FILEMGR_RetrieveFile(int *pFileLen, void * fileName);
#endif /* __cplusplus */

extern void     FILEMGR_ReleaseFile(void *pDummy);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
