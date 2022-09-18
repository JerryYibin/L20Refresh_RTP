/* vxbEhrPwmLib.h - VxBus EhrPwm interfaces header file */

/*
 * Copyright - To be Done
 */

/*
DESCRIPTION

This library provides the EhrPwm specific interfaces
*/

#ifndef __INCvxbEhrPwmLibh
#define __INCvxbEhrPwmLibh 

#include <vxWorks.h>
#include <hwif/vxBus.h>
#ifdef __cplusplus
extern "C" {
#endif

#define TBCNT_EQ_ZERO                 0
#define TBCNT_EQ_TBPRD                1
#define TBCNT_EQ_INC_CMPA             2
#define TBCNT_EQ_DEC_CMPA             3
#define TBCNT_EQ_INC_CMPB             4     
#define TBCNT_EQ_DEC_CMPB             5

#define ETPS_INTPRD_DISABLE           0
#define ETPS_INTPRD_GEN_FIRST_EVT     1
#define ETPS_INTPRD_GEN_SECOND_EVT    2
#define ETPS_INTPRD_GEN_THIRD_EVT     3

typedef struct vxbEhrPwmFunctionality
    {
    STATUS (*ehrpwmGetTbClk)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbClk)(VXB_DEV_ID pInst, UINT32 tb);
    STATUS (*ehrpwmGetTbPWMFreq)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbPWMFreq)(VXB_DEV_ID pInst,UINT32 tb, UINT32 dir);
    STATUS (*ehrpwmGetTbPWMDir)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbPWMDir)(VXB_DEV_ID pInst, UINT32 tb);
    STATUS (*ehrpwmGetTbPWMCfg)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbPWMCfg)(VXB_DEV_ID pInst,UINT32 tb, UINT32 dir);
    STATUS (*ehrpwmGetTbCOMPAConf)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbCOMPAConf)(VXB_DEV_ID pInst, UINT32 tb);
    STATUS (*ehrpwmGetTbCOMPBConf)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbCOMPBConf)(VXB_DEV_ID pInst, UINT32 tb);
    STATUS (*ehrpwmGetTbAQConfA)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbAQConfA)(VXB_DEV_ID pInst, UINT32 tb);
    STATUS (*ehrpwmGetTbAQConfB)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTbAQConfB)(VXB_DEV_ID pInst, UINT32 tb);
    STATUS (*ehrpwmGetCounter)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetCounter)(VXB_DEV_ID pInst, UINT32 counter);
    STATUS (*ehrpwmGetPRD)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmGetTbStatus)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmPWMRegisterISR)(VXB_DEV_ID pInst, VOIDFUNCPTR pIsr, void *pArg, UINT32 period, UINT32 mode);
    STATUS (*ehrpwmGetIntStatus)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmEnableInt)(VXB_DEV_ID pInst, UINT32 period, UINT32 mode);
    STATUS (*ehrpwmDisableInt)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetEmulation)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetAQSFRC)(VXB_DEV_ID pInst, UINT32 val);
    STATUS (*ehrpwmGetAQSFRC)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetTBCTL)(VXB_DEV_ID pInst, UINT32 val);
    STATUS (*ehrpwmGetTBCTL)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmSetIntEvtTBCTL)(VXB_DEV_ID pInst, UINT32 mode);
    STATUS (*ehrpwmSetIntPrdTBCTL)(VXB_DEV_ID pInst, UINT32 mode);
    STATUS (*ehrpwmGetIntEvtTBCTL)(VXB_DEV_ID pInst);
    STATUS (*ehrpwmGetIntPrdTBCTL)(VXB_DEV_ID pInst);
    } VXB_EHRPWM_FUNC;

/* IMPORT */
extern STATUS vxbEhrPwmGetTbStatus(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetEmulation(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTBCTL(UINT32 ehrpwm, UINT32 val);
extern STATUS vxbEhrPwmGetTBCTL(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbPWMFreq(UINT32 ehrpwm, UINT32 tb, UINT32 dir);
extern STATUS vxbEhrPwmGetTbPRD(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbPWMDir(UINT32 ehrpwm, UINT32 tb);
extern STATUS vxbEhrPwmGetTbPWMDir(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbCounter(UINT32 ehrpwm, UINT32 counter);
extern STATUS vxbEhrPwmGetTbCounter(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbClk(UINT32 ehrpwm, UINT32 tb);
extern STATUS vxbEhrPwmGetTbClk(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbPWMCfg(UINT32 ehrpwm, UINT32 freq, UINT32 dir);
extern STATUS vxbEhrPwmGetTbPWMCfg(UINT32 ehrpwm);
extern STATUS vxbEhrPwmGetTbAQConfA(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbAQConfA(UINT32 ehrpwm, UINT32 tb);
extern STATUS vxbEhrPwmGetTbAQConfB(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetTbAQConfB(UINT32 ehrpwm, UINT32 tb);
extern STATUS vxbEhrPwmGetCMPA(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetCMPA(UINT32 ehrpwm, UINT32 tb);
extern STATUS vxbEhrPwmGetCMPB(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetCMPB(UINT32 ehrpwm, UINT32 tb);
extern STATUS vxbEhrPwmSetAQSFRC(UINT32 ehrpwm, UINT32 val);
extern STATUS vxbEhrPwmGetAQSFRC(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetIntEvt(UINT32 ehrpwm, UINT32 mode);
extern STATUS vxbEhrPwmGetIntEvt(UINT32 ehrpwm);
extern STATUS vxbEhrPwmSetIntPrd(UINT32 ehrpwm, UINT32 mode);
extern STATUS vxbEhrPwmGetIntPrd(UINT32 ehrpwm);
extern STATUS vxbEhrPwmEnableInt(UINT32 ehrpwm, UINT32 period, UINT32 mode);
extern STATUS vxbEhrPwmDisableInt(UINT32 ehrpwm);
extern STATUS vxbEhrPwmRegisterISR(UINT32 ehrpwm, VOIDFUNCPTR pIsr, void *pArg, UINT32 period, UINT32 mode);
extern STATUS vxbEhrPwmGetIntStatus(UINT32 ehrpwm);
extern UINT32 ehrpwmDevNumGet();

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbEhrPwmLibh */

