/* httpWmb.h - header file for httpRrb,c */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,2feb10,h_y   support for LP64 
01b,12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01a,20feb04,jws  added copyright and mod history
*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

WMB_COMPONENT_T * httpGetWmbComp(void);
STATUS httpWmbInit (void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
