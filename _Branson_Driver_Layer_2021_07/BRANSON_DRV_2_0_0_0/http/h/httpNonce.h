/* wmNonce.h */

/* Copyright 2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,15mar10,m_z  VxWorks 64bit support (WIND00193649)
01b,25apr05,???  Get #define for JavaScript Authentication from wm_options.h,
                 since these files are in the common folder.
01a,17sep04,dkg  ported from RC3.3 to WM4.4 for JavaScript Authentication
*/


#include "httpAuth.h"

STATUS wmbGetNonceHandler
    (
    WMB_TID_T   tid,        /* transaction id */
    WMB_OBJ_T   obj,        /* the WindMark object */
    COMMAND_T   cmd         /* handler command */
    );

