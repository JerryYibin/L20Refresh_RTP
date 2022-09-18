/* httpWmbHandlers.h - WindManage for Web Backplane handlers */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01b,,2feb10,h_y   support for LP64  
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INChttpWmbHandlersh
#define __INChttpWmbHandlersh

STATUS selHandler (WMB_TID_T tid, WMB_OBJ_T selObj, COMMAND_T cmd);
STATUS checkedHandler (WMB_TID_T tid, WMB_OBJ_T chkObj, COMMAND_T cmd);
STATUS jsManglerHandler (WMB_TID_T tid, WMB_OBJ_T jsObj, COMMAND_T cmd);

#endif  /* __INChttpWmbHandlersh */
