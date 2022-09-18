/* smtpLib.h - SMTP headers file */

/* Copyright (c) 2003, 2012 Wind River Systems, Inc. */

#include "copyright_wrs.h"

/*
modification history
--------------------
01b,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01a,14aug03,adb  added to source control
*/

#ifndef __INCsmtpLibh
#define __INCsmtpLibh

/*
 *  smtpLib.h
 *
 *  This is a part of the OpenControl SDK source code library.
 *
 *  Copyright (C) 1999 Rapid Logic, Inc.
 *  All rights reserved.
 *
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

#include <webcli_osconf.h>

#include "wm.h"
#include "wmos.h"
#include "wmb.h"
#include "wmbLockMgrLib.h"
#include "wmbDBLib.h"
#include "wmbSessionLib.h"
#include "httpFileMgr.h"
#include "httpFileRegistry.h"
#include "httpReq.h"
#include "httpLib.h"

#ifndef WEBCLI_ON_LINUX
#include "pipeDrv.h"
#include "ioLib.h"
#include "memLib.h"
#endif

#include "stdlib.h"
#include "memPoolLib.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* defines */

/* Max. bytes to read at a time from file */

#define kNumDomainChars     64    /* max size of domain name */
#define kNumMailChars       80    /* max size of email address */

#define kNumMaxRcpts        5     /* max no. of email recepients */
#define kNumResourceChars   256   /* max size of resource path length */

/* Structure used to describe a Mail session */

typedef struct Smtp_MailData
    {
    sbyte   mailServer[kNumDomainChars];
    sbyte   domainName[kNumDomainChars];
    sbyte   mailFrom[kNumMailChars];

    sbyte   numRecipients;
    sbyte   rcptTo[kNumMaxRcpts][kNumMailChars];

    sbyte   ResourceToMail[kNumResourceChars];

    } Smtp_MailData;


/* Function Prototypes */

/* SMTP_Init - Initializes SMTP mail control block structure */

STATUS SMTP_Init 
    (
    void
    );

/* SMTP_SendMail - Basic API to send the email through SMTP */

STATUS SMTP_SendMail 
    (
    Smtp_MailData * pMailData
    );

/* smtpSendAlert - Collect email data from WindMarks and invoke SMTP_SendMail */

STATUS smtpSendAlert
    (
    char *  resource
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCsmtpLibh */

