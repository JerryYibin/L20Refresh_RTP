/*
 * vxbTiEqep.c
 * Enhanced Quadrature Encoder Pulse
 */
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

#include <stdio.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <vxbEqepLib.h>
#include <vxbEqep.h>

/* defines */
#undef EQEP_DBG_ON

#ifdef  EQEP_DBG_ON
#define EQEP_DBG_IRQ         0x00000001
#define EQEP_DBG_INFO        0x00000002
#define EQEP_DBG_ERR         0x00000004
#define EQEP_DBG_ALL         0xffffffff
#define EQEP_DBG_OFF         0x00000000
LOCAL UINT32 eqepDbgMask = EQEP_DBG_ALL;
#define EQEP_DBG(mask, ...)                              \
     do                                                                  \
         {                                                               \
         if((eqepDbgMask & mask) || (mask == EQEP_DBG_ALL))               \
             {                                                           \
             if(_func_kprintf != NULL)                                  \
                 {                                                       \
                 (* _func_kprintf)("%s %s %d: ",__FUNCTION__,__FILE__,__LINE__);                         \
                 (* _func_kprintf)(__VA_ARGS__);                         \
                 }                                                       \
             }                                                           \
         }                                                               \
     while((FALSE))
#else
#define EQEP_DBG(...)
#endif  /* EQEP_DBG_ON */

extern void eqepRegister
    (
    VXB_DEV_ID pDev,
    VXB_PWMSS_EQEP_FUNC *eqep,
    char *name
    );

LOCAL const VXB_FDT_DEV_MATCH_ENTRY am570xeqepMatch[] =
    {
    {
    "ti,am570x-eqep", /* compatible */
    (void *)NULL
    },
    {} /* Empty terminated list */
    };

/*************************************************************************
 * 
 * am570xeqepProbe : This function finds and compare the compatible 
 * field from the dts file.
 * 
 * RETURN : if found then success , else fail.
 */
LOCAL STATUS am570xEqepProbe
    (
    struct vxbDev *pDev
    )
    {
    return vxbFdtDevMatch(pDev, am570xeqepMatch, NULL);
    }

/*******************************************************************************
* eqepIrqHandler - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL void eqepIrqHandler
    (
    void *arg
    )
    {
    EQEP_DBG(EQEP_DBG_INFO,"%s %s %d\n",__FUNCTION__,__FILE__,__LINE__);
    }

/*******************************************************************************
* eqepIrqHandler - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepInitialize
    (
    VXB_DEV_ID pDev
    )
    {
    UINT16 status;

    status = CSR_READ_2(pDev, QDECCTL);
    status &= ~QSRC0 & ~QSRC1;
    status &= ~QAP;
    status &= ~QBP;
    status &= ~QIP;
    status &= ~QSP;
    status |= SWAP;
    CSR_WRITE_2(pDev, QDECCTL,status);
    CSR_WRITE_4(pDev, QPOSINIT,0);
    CSR_WRITE_4(pDev, QPOSMAX, 0xffffffff);
    CSR_WRITE_4(pDev, PWMSS_QPOSCNT,0);
#if 0
    UINT64 period = 0;
    // period = 1000000000;
    // CSR_WRITE_2(pDev, QUPRD,(uint32_t)(period & 0x00000000FFFFFFFF));
    // 
    //    status = CSR_READ_2(pDev, QEPCTL);
    //    status = status | PHEN | IEL0 | SWI | UTE | QCLM;
#endif
    status = 0x1098;
    CSR_WRITE_2(pDev, QEPCTL, status);
    return OK;
    }

/*******************************************************************************
* eqepShow - interrupt handler
*
* display the information of eQEP device.
* 
* RETURNS: OK.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepShow(VXB_DEV_ID pDev)
    {
    UINT32 mode;

    mode = CSR_READ_4(pDev,PWMSS_QPOSCNT);
    printf("PWMSS_QPOSCNT %x\n", mode);

    mode = CSR_READ_2(pDev,PWMSS_QEPSTS);
    printf("PWMSS_QEPSTS %x\n", mode);

    mode = CSR_READ_2(pDev,PWMSS_QDECCTL);
    printf("PWMSS_QDECCTL %x\n", mode);

    mode = CSR_READ_4(pDev, PWMSS_QREVID);
    printf("PWMSS_QREVID  %x\n", mode);

    mode = CSR_READ_4(pDev, PWMSS_QPOSILAT);
    printf("PWMSS_QPOSILAT  %x\n", mode);

    mode = CSR_READ_2(pDev, PWMSS_QFLG);
    printf("PWMSS_QFLG  %x\n", mode);

    CSR_WRITE_2(pDev, PWMSS_QCLR, 0);

    mode = CSR_READ_4(pDev, PWMSS_QPOSMAX);
    printf("PWMSS_QPOSMAX  %x\n", mode);
    return OK;
    }

/*******************************************************************************
* eqepGetPosCount - read Position counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepGetPosCount
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 mode = CSR_READ_4(pDev, PWMSS_QPOSCNT);
    EQEP_DBG(EQEP_DBG_INFO,"PWMSS_QPOSCNT  %x\n", mode);
    return (STATUS)mode;
    }

/*******************************************************************************
* eqepSetMaxCount - set Maximum counter register
*
* RETURNS: OK.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepSetMaxCount
    (
    VXB_DEV_ID pDev, 
    UINT32 max
    )
    {
    CSR_WRITE_4(pDev, PWMSS_QPOSMAX, max);
    return OK;
    }

/*******************************************************************************
* eqepGetMaxCount - get Maximum counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepGetMaxCount
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 mode = CSR_READ_4(pDev, PWMSS_QPOSMAX);
    EQEP_DBG(EQEP_DBG_INFO, "PWMSS_QPOSMAX  %x\n", mode);
    return (STATUS)mode;
    }

/*******************************************************************************
* eqepSetInitCount - set Initial counter register
*
* RETURNS: OK.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepSetInitCount
    (
    VXB_DEV_ID pDev, 
    UINT32 val
    )
    {
    CSR_WRITE_4(pDev, QPOSINIT, val);
    CSR_WRITE_4(pDev, PWMSS_QPOSCNT, val);
    return OK;
    }

/*******************************************************************************
* eqepGetInitCount - get Initial counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepGetInitCount
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 mode = CSR_READ_4(pDev, QPOSINIT);
    EQEP_DBG(EQEP_DBG_INFO,"QPOSINIT  %x\n", mode);
    return (STATUS)mode;
    }

/*******************************************************************************
* eqepGetDirection - get Initial counter register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
LOCAL STATUS eqepGetDirection
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 mode = CSR_READ_2(pDev, PWMSS_QEPSTS);
    mode = mode >> 5;
    mode &=0x1;
    EQEP_DBG(EQEP_DBG_INFO, "Direction %s \n", mode ? "clockwise" : "anti-clockwise");
    return (STATUS)mode;
    }

/***************************************************************************
 * am570xeqepAttach : This function allocates driver structure, 
 * registers and map it to  system.It always does the basic hardware and 
 * software initialization needed for driver and hardware functioning.
 * 
 * RETURN :  attach success , else fail.
 */
LOCAL STATUS am570xEqepAttach
    (
    VXB_DEV_ID  pDev
    )
    {
    VXB_PWMSS_EQEP_FUNC *eqep = NULL;
    VXB_RESOURCE_ADR *pResAdr;
    VXB_RESOURCE *pRes = NULL;
    VXB_FDT_DEV *pFdtDev;
    EQEPGEN *pEqepObj;

    pFdtDev = vxbFdtDevGet(pDev);
    if(pFdtDev == NULL)
        {
        return ERROR;
        }

    /* Allocate Driver Memory */
    pEqepObj = (EQEPGEN *)vxbMemAlloc(sizeof(EQEPGEN));
    if(pEqepObj == NULL)
        goto attach_error;

    vxbDevSoftcSet(pDev, pEqepObj);

    /* save instance ID */
    pEqepObj->pDev = pDev;

    /* Allocate resource */
    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
        goto attach_error;

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if(pResAdr == NULL)
        goto attach_error;

    pEqepObj->regHandle = pResAdr->pHandle;
    pEqepObj->regBase = (void *)pResAdr->virtAddr;
    pEqepObj->intRes = vxbResourceAlloc(pDev, VXB_RES_IRQ, 0);

    if(vxbPinMuxEnable(pDev) != OK)
        {
        goto attach_error;
        }

    if(vxbClkEnableAll(pDev) != OK)
        {
        goto attach_error;
        }

    /* Allocate and register library function for i/o subsytem */
    eqep = (VXB_PWMSS_EQEP_FUNC *)vxbMemAlloc(sizeof(VXB_PWMSS_EQEP_FUNC));
    if(eqep == NULL)
        goto attach_error;

    eqep->eqepGet = eqepShow;
    eqep->eqepGetMaxCount = eqepGetMaxCount;
    eqep->eqepSetMaxCount = eqepSetMaxCount;
    eqep->eqepGetInitCount = eqepGetInitCount;
    eqep->eqepSetInitCount = eqepSetInitCount;
    eqep->eqepGetPosCount = eqepGetPosCount;
    eqep->eqepGetDirection = eqepGetDirection;
    eqepRegister(pDev, eqep, pFdtDev->name);

    /* enable and connect ISR */
    if(vxbIntConnect(pDev, pEqepObj->intRes, (VOIDFUNCPTR)eqepIrqHandler, NULL) != OK)
        {
        goto attach_error;
        }

    if(vxbIntEnable(pDev, pEqepObj->intRes) != OK)
        {
        vxbIntDisconnect(pDev, pEqepObj->intRes);
        goto attach_error;
        }

    eqepInitialize(pDev);
    return OK;

    attach_error:
    if(pEqepObj->intRes != NULL)
        vxbResourceFree(pDev, pEqepObj->intRes);

    if(pRes != NULL)
        vxbResourceFree(pDev, pRes);

    if(pEqepObj != NULL)
        vxbMemFree(pEqepObj);

    if(eqep != NULL)
        vxbMemFree(eqep);
    return ERROR;
    }

LOCAL VXB_DRV_METHOD am570xeqepMethods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe),  (FUNCPTR)am570xEqepProbe},
    { VXB_DEVMETHOD_CALL(vxbDevAttach), (FUNCPTR)am570xEqepAttach},
    VXB_DEVMETHOD_END
    };
VXB_DRV vxbOfAm570xeqepDrv =
    {
    { NULL } ,
    "ti,am570x-eqep",                   /* Name */
    "TI AM570x EQEP Driver",            /* Description */
    VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
    (VXB_DRV_METHOD*)&am570xeqepMethods /* Method table */
    };
VXB_DRV_DEF(vxbOfAm570xeqepDrv)

