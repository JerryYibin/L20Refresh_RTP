/* vxbEqepLib.h - VxBus EQEP interfaces header file */

/*
 * Copyright : To Be Done
 */
/*
DESCRIPTION

This library provides the EQEP specific interfaces

*/

#ifndef __INCvxbEqepLibh
#define __INCvxbEqepLibh 

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* structure to be used and updated EQEP time and Alarm */

typedef struct vxbEqepFunctionality
    {
    STATUS (*eqepGet)(VXB_DEV_ID pInst);
    STATUS (*eqepGetPosCount)(VXB_DEV_ID pInst);
    STATUS (*eqepGetMaxCount)(VXB_DEV_ID pInst);
    STATUS (*eqepSetMaxCount)(VXB_DEV_ID pInst, UINT32 val);
    STATUS (*eqepSetInitCount)(VXB_DEV_ID pInst,UINT32 val);
    STATUS (*eqepGetInitCount)(VXB_DEV_ID pInst);
    STATUS (*eqepGetDirection)(VXB_DEV_ID pInst);
    } VXB_PWMSS_EQEP_FUNC;

extern UINT32 eqepDevNumGet();
extern STATUS vxbEqepShow(UINT32);
extern STATUS vxbEqepGetMaxCount(UINT32);
extern STATUS vxbEqepSetMaxCount(UINT32, UINT32);
extern STATUS vxbEqepGetInitCount(UINT32);
extern STATUS vxbEqepSetInitCount(UINT32, UINT32);
extern STATUS vxbEqepGetPosCount(UINT32);
extern STATUS vxbEqepGetDirection(UINT32);

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbEqepLibh */

