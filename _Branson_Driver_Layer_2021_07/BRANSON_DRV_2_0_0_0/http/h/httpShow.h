/* httpShow.h - header file for HTTP show routines */

/* Copyright (c) 2003-2004,2010,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
-------------------- 
01h,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
01g,26jan10,h_y  support for 2010
01f,15mar04,adb  added httpSecureUploadConfigurationShow
01e,15mar04,adb  added httpSecureFileSystemsShow
01d,08mar04,adb  added httpWmbParserConfigurationShow 
01c,20jan04,jws  fix compiler warning
01b,07nov03,adb  WRS coding conventions modifications
01a,18aug03,adb  added to source control
*/

#ifndef __INChttpShowh
#define __INChttpShowh

/* includes */

#include "httpLib.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef HTTP_SHOW
void httpListShow
    (
    HTTP_GEN_LIST_ID listId,
    char *           pListName,
    BOOL             idIsaPointer,
    BOOL             dataIsaPointer,
    char *           pLeftMargin
    );

void httpAliasesShow
    (
    void
    );

void httpFileSystemsShow
    (
    void
    );

void httpSecureFileSystemsShow
    (
    void
    );

void httpUploadConfigurationShow
    (
    void
    );

void httpSecureUploadConfigurationShow
    (
    void
    );

void httpCgiConfigurationShow
    (
    void
    );

void httpMimeParserConfigurationShow
    (
    void
    );

void httpWmbParserConfigurationShow
    (
    void
    );

void httpIpCheckListShow
    (
    void
    );

void httpPasswordListShow
    (
    void
    );

void httpAccessControlListShow
    (
    void
    );

void httpSystemRpmsShow
    (
    void
    );

void httpRpmDispatchListShow
    (
    void
    );

void httpFirstMethodsShow
    (
    unsigned int numberOfHttpMethods
    );

void httpFirstMethodRpmsShow
    (
    unsigned int numberOfHttpMethods
    );

#endif /* HTTP_SHOW */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ___INChttpShowh */

