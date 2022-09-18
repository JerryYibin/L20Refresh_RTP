/* httpRpms.h - HTTP server request processing module (RPM) header */

/*
 * Copyright (c) 1997-2008,2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02e,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
02d,29sep11,r_w  add way to delete WR Web server aliases.
                 defect:WIND00197521 
02c,28mar11,r_w fix defect WIND00257014
            to make some declares of functions comply with definitions
02b,17dec10,m_z part of APIs changed
                CQID: Fix WIND00247096
02a,26jan10,h_y  support for 2010 
01p,30jun08,q_z  fix using REPEAT macro in pages sent via POST (WIND00039488)
01o,25jun08,q_z  implementation of config file save and load(WIND00042066)
01n,09nov04,dlr  SPR #102705 : allow upload to be last RPM.
01m,05aug04,adb  added httpCtrlConfRemove and httpCtrlGroupRemove
01l,03aug04,adb  added httpPwdGroupRemove and httpPwdConfUserRemove
01k,27apr04,adb  replaced all buffer size constants with kMagicMarkupBufferSize
01j,15mar04,adb  introduced httpSecureUploadConfAdd
01i,12mar04,adb  introduced httpFileSystemRootGet
01h,05mar04,adb  introduced httpSecureFsConfAdd
01g,06feb04,dkg  Added declaration of httpWindMarkParse() and
                 #define TMP_BUF_SIZE with reference to SPR 91304
01f,14jan04,gsr  cleanup routine for SPR 93009
01e,31jul03,jws  Additions for SPR 89657
01d,09sep99,csr  merging for zlib
01c,01sep99,va   adding download rpm
01b,09jun99,km   merge of Wind Web Server 2.0 code.
01a,15may97,mb   written.
*/

#ifndef __INChttpRpmsh
#define __INChttpRpmsh

/* includes */

#include "httpFileMgr.h"
#include "httpCfg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef short (* HTTP_RPM)(HTTP_REQ_ID reqId); /* generic type of RPMs */
#ifdef INCLUDE_HTTP_CGI
typedef short (* HTTP_CGI_FUNC)(long argc,char **argv,HTTP_REQ_ID reqId); /* generic type of CGI functions */
#endif
typedef HTTP_STATUS (* HTTP_MULTIPLEX_FUNC)(char * szArg);

/* defines */

#define RPM_ALL_OR_NONE_SET     1
#define RPM_BEST_EFFORT_SET     2
#define RPM_CHANGE_ONLY_SET     4


/* RPM return codes */
#define RPM_ERROR   -1 /* an error occured, stop processing by RPMs  */
#define RPM_OK       0 /* RPM processing successful, no output done, */
                       /* continue with the next RPM in the list     */
#define RPM_RESTART  1 /* RPM processing successful, no output done, */
                       /* continue with the first RPM in the list    */
#define RPM_DONE     2 /* RPM processing successful, all output was  */
                       /* handled by the RPM */

/* externals */

extern       HTTP_RPM     httpSystemRpms[];
extern const char * const httpDefGidString;
extern const char * const httpDefLocationString;

/* function declarations */

HTTP_GEN_LIST_ID       httpRpmConfInit (unsigned short usMethods,unsigned short usDispatchTabSize);
HTTP_STATUS            httpRpmConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpRpmConfAdd (short sMethod, char * szUrl, HTTP_RPM pRpm);
HTTP_GEN_LIST_ENTRY_ID httpSecureRpmConfAdd (short sMethod, char * szUrl, HTTP_RPM pRpm);

HTTP_STATUS httpSysRpmConfInit (short sHttpSysRpmDispTabMax);
HTTP_STATUS httpSysRpmConfAdd (HTTP_RPM pRpm);

/* RPMs */
short httpRpmHello               (HTTP_REQ_ID  reqId);
short httpRpmMime                (HTTP_REQ_ID  reqId);
short httpRpmFs                  (HTTP_REQ_ID  reqId);
short httpRpmMemory              (HTTP_REQ_ID  reqId);
#ifdef INCLUDE_HTTP_IP_CHECKING
short httpRpmIPchk               (HTTP_REQ_ID  reqId);
#endif
#ifdef INCLUDE_HTTP_ACCESS_CTRL
short httpRpmCtrl                (HTTP_REQ_ID  reqId);
#endif
short httpRpmPull                (HTTP_REQ_ID  reqId);
short httpRpmShell               (HTTP_REQ_ID  reqId);
#ifdef INCLUDE_HTTP_PWD_CHECKING
short httpRpmPass                (HTTP_REQ_ID  reqId);
#endif

#ifdef INCLUDE_HTTP_ALIASING
short httpRpmAlias               (HTTP_REQ_ID  reqId);
#endif

#ifdef INCLUDE_HTTP_LOG
short httpRpmLog                 (HTTP_REQ_ID  reqId);
#endif

#ifdef INCLUDE_HTTP_UPLOAD
short httpRpmUpload              (HTTP_REQ_ID  reqId);
short httpRpmUploadLast          (HTTP_REQ_ID  reqId);
short httpRpmDownload            (HTTP_REQ_ID  reqId);
#endif

short httpProcessPostData         (HTTP_REQ_ID  reqId);
short httpSendResponse        	  (HTTP_REQ_ID  reqId);

short httpProcessFOEPostData      (HTTP_REQ_ID  reqId);
short httpSendFOEResponse         (HTTP_REQ_ID  reqId);

#ifdef INCLUDE_HTTP_CGI
short httpRpmCgi                 (HTTP_REQ_ID  reqId);
#endif

#ifdef INCLUDE_HTTP_SHOWAUTH
short httpRpmAuth                (HTTP_REQ_ID  reqId);
#endif
short httpRpmDump                (HTTP_REQ_ID  reqId);
short httpRpmFormInput           (HTTP_REQ_ID  reqId);
short httpRpmSystemCall          (HTTP_REQ_ID  reqId);
short httpRpmStatusPage          (HTTP_REQ_ID  reqId);
short httpRpmPost                (HTTP_REQ_ID  reqId);
short httpRpmWmbParse            (HTTP_REQ_ID  reqId);

short httpRpmWmbPost             (HTTP_REQ_ID  reqId);
short httpRpmWmbBestEffortPost   (HTTP_REQ_ID  reqId);
short httpRpmWmbPostExec         (HTTP_REQ_ID reqId,int);
short httpRpmWmbPostExecUser     (HTTP_REQ_ID reqId,int);
STATUS SendResponse
    (
    HTTP_REQ_ID     reqId,         /* http request */
    WMB_SESSION_T * pSession,      /* session for setting */
    WMB_TID_T       tid,           /* transaction in pSession */
    WMB_SESSION_T * pGetSession    /* (optional) session for getting */
    );
short httpRpmJsPost              (HTTP_REQ_ID  reqId);


/* RPM configuration functions */
#ifdef INCLUDE_HTTP_LOG
HTTP_STATUS            httpLogConfInit (char * szFile);
#endif

#ifdef INCLUDE_HTTP_ALIASING
HTTP_GEN_LIST_ID       httpAliasConfInit (unsigned short sTabMax);
HTTP_STATUS            httpAliasConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpAliasConfAdd (char * szSearch, char * szReplace);
HTTP_STATUS            httpAliasConfDel (char * szSearch);
#endif

#ifdef INCLUDE_HTTP_UPLOAD
HTTP_STATUS            httpUploadConfInit (unsigned short sTabMax);
HTTP_STATUS            httpUploadConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpUploadConfAdd (char * szUrl, char * szFsRoot);
HTTP_GEN_LIST_ENTRY_ID httpSecureUploadConfAdd (char * szUrl, char * szFsRoot);
#endif

HTTP_STATUS            httpFsConfInit (unsigned short sTabMax);
HTTP_STATUS            httpFsConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpFsConfAdd (char * szUrl, char * szFsRoot);
HTTP_GEN_LIST_ENTRY_ID httpSecureFsConfAdd (char * szUrl, char * szFsRoot);
HTTP_STATUS            httpFsRootGet (HTTP_REQ_ID reqId,
                                      char * pRoot,
                                      size_t bufferSize);
    
#ifdef INCLUDE_HTTP_CGI
HTTP_GEN_LIST_ID       httpCgiFnConfInit (unsigned short usTabMax);
HTTP_STATUS            httpCgiFnConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpCgiFnConfAdd (char * szName, HTTP_CGI_FUNC pCgi);
#endif

HTTP_STATUS            httpMimeConfInit (HTTP_MIME_SUFFIX_GUESS * mimeSuffixTab);
HTTP_GEN_LIST_ID       httpMimeParseFnConfInit (unsigned short sMaxEntries);
HTTP_STATUS            httpMimeParseFnConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpMimeParseFnConfAdd (char * szMimeType, HTTP_MIME_TOKEN_PARSER pMimeParser);

#ifdef INCLUDE_HTTP_ACCESS_CTRL
HTTP_GEN_LIST_ID       httpCtrlConfInit (unsigned short sCtrls,unsigned short sMembersPerCtrl,unsigned short sCtrlUrlSize,unsigned short sGidNameSize,unsigned short sLocNameSize);
HTTP_STATUS            httpCtrlConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpCtrlConfAdd (char * szUrl, char * szUserGroup, char * szLocGroup);
void                   httpCtrlConfDelete (HTTP_GEN_LIST_ENTRY_ID urlidEntryId, HTTP_GEN_LIST_ENTRY_ID ctrlEntryId);
HTTP_STATUS            httpCtrlConfRemove (char * szUrl, char * szUserGroup, char * szLocGroup);
void                   httpCtrlUrlDelete (HTTP_GEN_LIST_ENTRY_ID urlidEntryId);
HTTP_STATUS            httpCtrlUrlRemove (char * szUrl);
char *                 httpCtrlUrlStringGet (HTTP_GEN_LIST_ENTRY_ID urlidEntryId);
HTTP_STATUS            httpCtrlConfLock (void);
HTTP_STATUS            httpCtrlConfUnlock (void);
HTTP_GEN_LIST_ID       httpCtrlConfGet(void);
HTTP_GEN_LIST_ENTRY_ID httpCtrlUrlIterator (HTTP_GEN_LIST_ENTRY_ID * currentId);
HTTP_GEN_LIST_ENTRY_ID httpCtrlUrlIndexIterator (unsigned short * pusIndex);
HTTP_GEN_LIST_ENTRY_ID httpCtrlAllowIterator (HTTP_GEN_LIST_ENTRY_ID ctrlEntryId, HTTP_GEN_LIST_ENTRY_ID * allowEntryId);
HTTP_GEN_LIST_ENTRY_ID httpCtrlAllowIndexIterator (HTTP_GEN_LIST_ENTRY_ID ctrlEntryId, unsigned short * pusIndex);
#endif

#ifdef INCLUDE_HTTP_IP_CHECKING
HTTP_GEN_LIST_ID       httpIpConfInit (unsigned short sLocs,unsigned short sIpPerLoc, unsigned short sIpPatSize, unsigned short sLocatNameSize);
HTTP_STATUS            httpIpConfCleanup (void);
HTTP_GEN_LIST_ID       httpIpConfGet(void);
HTTP_GEN_LIST_ENTRY_ID httpIpConfAdd (char * szGroup, char * szIpPattern);
HTTP_GEN_LIST_ENTRY_ID httpIpLocationIterator (HTTP_GEN_LIST_ENTRY_ID * locationId);
HTTP_GEN_LIST_ENTRY_ID httpIpPatternIterator (HTTP_GEN_LIST_ENTRY_ID locEntryId,HTTP_GEN_LIST_ENTRY_ID * ipEntryId);
HTTP_GEN_LIST_ENTRY_ID httpIpLocationIndexIterator (unsigned short *pusIndex);
HTTP_GEN_LIST_ENTRY_ID httpIpPatternIndexIterator (HTTP_GEN_LIST_ENTRY_ID locEntryId,unsigned short * pusIndex);
void                   httpIpConfDelete (HTTP_GEN_LIST_ENTRY_ID locEntryId,HTTP_GEN_LIST_ENTRY_ID ipEntryId);
void                   httpIpGroupDelete (HTTP_GEN_LIST_ENTRY_ID locEntryId);
HTTP_STATUS            httpIpConfLoad (generic_fd_t * inFile);
HTTP_STATUS            httpIpConfSave (generic_fd_t * outFile);
HTTP_STATUS            httpIpConfLock (void);
HTTP_STATUS            httpIpConfUnlock (void);
#endif

#ifdef INCLUDE_HTTP_PWD_CHECKING
HTTP_GEN_LIST_ID       httpPwdConfInit (unsigned short sGids,unsigned short sUsersPerGid, unsigned short sGidNameSize, unsigned short sCredentialSize,char *szRealm);
HTTP_STATUS            httpPwdConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpPwdConfAdd (char * szGroup, char * szName, char * szPwd);
HTTP_STATUS            httpPwdConfUserRemove (char * szGroup, char * szName);
HTTP_GEN_LIST_ENTRY_ID httpPwdAuthAdd (char * szGroup, char * szAuth);
HTTP_GEN_LIST_ENTRY_ID httpPwdNameFind (HTTP_GEN_LIST_ENTRY_ID gidEntryId,char *szUser,char *szBuffer);
void                   httpPwdConfDelete (HTTP_GEN_LIST_ENTRY_ID gidEntryId,HTTP_GEN_LIST_ENTRY_ID authEntryId);
void                   httpPwdGroupDelete (HTTP_GEN_LIST_ENTRY_ID gidEntryId);
HTTP_STATUS            httpPwdCroupRemove (char * szGroup);
HTTP_STATUS            httpPwdConfLoad (generic_fd_t * inFile);
HTTP_STATUS            httpPwdConfSave (generic_fd_t * outFile);
char *                 httpPwdGroupStringGet (HTTP_GEN_LIST_ENTRY_ID gidEntryId);
char *                 httpPwdAuthStringGet (HTTP_GEN_LIST_ENTRY_ID authEntryId);
HTTP_GEN_LIST_ENTRY_ID httpPwdAuthIterator (HTTP_GEN_LIST_ENTRY_ID gidEntryId,HTTP_GEN_LIST_ENTRY_ID * authEntryId);
HTTP_GEN_LIST_ENTRY_ID httpPwdGroupIterator (HTTP_GEN_LIST_ENTRY_ID * gidEntryId);
HTTP_GEN_LIST_ENTRY_ID httpPwdAuthIndexIterator (HTTP_GEN_LIST_ENTRY_ID gidEntryId,unsigned short * pusIndex);
HTTP_GEN_LIST_ENTRY_ID httpPwdGroupIndexIterator (unsigned short * pusIndex);
HTTP_GEN_LIST_ID       httpPwdConfGet(void);
HTTP_STATUS            httpPwdConfLock (void);
HTTP_STATUS            httpPwdConfUnlock (void);
HTTP_STATUS            httpPwdDecode (char * szAuth,char * szName,unsigned short sSize);
#endif

HTTP_STATUS            httpRpmSymbolsInit (char * szURL);

HTTP_GEN_LIST_ID       httpWmbParserConfInit (unsigned short maxPaths);
HTTP_STATUS            httpWmbParserConfCleanup (void);
HTTP_GEN_LIST_ENTRY_ID httpWmbParserConfAdd (char * pUrl, char * pFsPath);

HTTP_STATUS            httpWmbPostConfInit (void);

HTTP_STATUS            httpPostConfInit (void);

#ifdef INCLUDE_HTTP_UPLOAD
HTTP_STATUS            httpUploadRpmNotLast (void);
#endif

HTTP_STATUS            httpRpmConfLock (void);
HTTP_STATUS            httpRpmConfUnlock (void);
void                   httpRpmConfDelete (HTTP_GEN_LIST_ENTRY_ID methodEntryId,HTTP_GEN_LIST_ENTRY_ID rpmEntryId);
void                   httpRpmMethodDelete (HTTP_GEN_LIST_ENTRY_ID methodEntryId);
char *                 httpRpmMethodStringGet (HTTP_GEN_LIST_ENTRY_ID methodEntryId);

#ifndef WEBCLI_STANDALONE_WEBSERVER
STATUS                 httpWindMarkParse (STATUS (*writeFctPtr)(void *, sbyte *, ubyte4),
                                          void *            writeArg,
                                          WMB_SESSION_T *   pSession,
                                          sbyte *           pSrc,
                                          unsigned long     srcLen,
                                          unsigned long *   pUsed,
                                          sbyte **          ppExitMark
                                          );
#endif

STATUS                 markParserDrvr (STATUS (*writeFctPtr)(void *, sbyte *, ubyte4),
                                       void *        writeArg,
                                       HTTP_REQ_ID   reqId,
                                       generic_fd_t* pF,
                                       unsigned long fileSize,
                                       sbyte *       workBuff,
                                       unsigned long bufSize
                                       );

#ifdef __cplusplus
}
#endif

#endif /* __INChttpRpmsh */

