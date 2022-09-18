/* vxbEhrPwmLib.h - VxBus EhrPwm interfaces header file */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05may22,ykm  created.
*/

/*
DESCRIPTION

This library provides the EhrPwm specific interfaces
*/

#ifndef __INCvxbEhrPwmLibh
#define __INCvxbEhrPwmLibh 

#include <vxWorks.h>
#ifdef __cplusplus
extern "C" {
#endif
extern UINT32 ehrpwmDevNumGet();
extern STATUS vxbEhrPwmGetTbStatus(UINT32);
extern STATUS vxbEhrPwmSetEmulation(UINT32);
extern STATUS vxbEhrPwmSetTBCTL(UINT32, UINT32);
extern STATUS vxbEhrPwmGetTBCTL(UINT32);
extern STATUS vxbEhrPwmSetTbPWMFreq(UINT32, UINT32, UINT32);
extern STATUS vxbEhrPwmGetTbPRD(UINT32);
extern STATUS vxbEhrPwmSetTbPWMDir(UINT32, UINT32);
extern STATUS vxbEhrPwmGetTbPWMDir(UINT32);
extern STATUS vxbEhrPwmSetTbCounter(UINT32, UINT32);
extern STATUS vxbEhrPwmGetTbCounter(UINT32);
extern STATUS vxbEhrPwmSetTbClk(UINT32, UINT32);
extern STATUS vxbEhrPwmGetTbClk(UINT32);
extern STATUS vxbEhrPwmSetTbPWMCfg(UINT32, UINT32, UINT32);
extern STATUS vxbEhrPwmGetTbPWMCfg(UINT32);
extern STATUS vxbEhrPwmGetTbAQConfA(UINT32);
extern STATUS vxbEhrPwmSetTbAQConfA(UINT32, UINT32);
extern STATUS vxbEhrPwmGetTbAQConfB(UINT32);
extern STATUS vxbEhrPwmSetTbAQConfB(UINT32, UINT32);
extern STATUS vxbEhrPwmGetCMPA(UINT32);
extern STATUS vxbEhrPwmSetCMPA(UINT32, UINT32);
extern STATUS vxbEhrPwmGetCMPB(UINT32);
extern STATUS vxbEhrPwmSetCMPB(UINT32, UINT32);
extern STATUS vxbEhrPwmSetAQSFRC(UINT32, UINT32 val);
extern STATUS vxbEhrPwmGetAQSFRC(UINT32);
extern STATUS vxbEhrPwmSetIntEvt(UINT32, UINT32);
extern STATUS vxbEhrPwmGetIntEvt(UINT32);
extern STATUS vxbEhrPwmSetIntPrd(UINT32, UINT32);
extern STATUS vxbEhrPwmGetIntPrd(UINT32);
extern STATUS vxbEhrPwmEnableInt(UINT32, UINT32, UINT32);
extern STATUS vxbEhrPwmDisableInt(UINT32);
extern STATUS vxbEhrPwmRegisterISR(UINT32, VOIDFUNCPTR, void*, UINT32, UINT32);
extern STATUS vxbEhrPwmGetIntStatus(UINT32);
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbEhrPwmLibh */


