/* vxbEhrPwmLib.c - vxBus ehrpwm function interfaces file */
/*
  PWM    - Pulse-Width Modulation
  PWMSS  - Pulse Width Modulation Subsystem
  eHRPWM - Enhanced High Resolution Pulse Width Modulator
  TZ     - Trip-Zone
  ET     - Event-Trigger
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
/* includes */
#include <stdio.h>
#include <string.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <vxbEhrPwmLib.h>

#define EPWM_NUM  3
    
struct vxbEhrPwmHandle
    {
    VXB_DEV_ID       pInst;
    VXB_EHRPWM_FUNC *pFunc;
    char             name[FDT_NODE_NAME_LEN+1];
    };

/* locals */
static UINT32 count = 0;
static struct vxbEhrPwmHandle devHandle[EPWM_NUM] = {0};

/*****************************************************************************
* ehrpwmRegister - register ehrpwm set/get routine to VxBus ehrpwm subsystem.
*
* This function register ehrpwm set/get routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void ehrpwmRegister
    (
    VXB_DEV_ID pDev,
    VXB_EHRPWM_FUNC *ehrpwm,
    char *name
    )
    {
    if(count<EPWM_NUM)
        {
        devHandle[count].pInst = pDev;
        devHandle[count].pFunc = ehrpwm;
        bcopy(name, devHandle[count].name, FDT_NODE_NAME_LEN);
        count++;
        }
    }
/*******************************************************************************
*
* ehrpwmDevNumGet - get the count of the current ehrpwm 
*
* RETURNS: ehrpwm attached current.
*
* ERRNO: N/A
*/
UINT32 ehrpwmDevNumGet()
    {
    UINT32 i;
    for(i=0; i<count; i++)
        {
        printf("ehrPWM Dev%d: %s\r\n", i+1, devHandle[i].name);
        }
    return count;
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbCounter - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTbStatus 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbStatus(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetEmulation - set the These bits select the behavior of the ePWM 
* time-base counter during emulation suspend events.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetEmulation 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;    
    return devHandle[ehrpwm].pFunc->ehrpwmSetEmulation(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetTBCTL - set Time-Base Control Register register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTBCTL 
    (
    UINT32 ehrpwm,
    UINT32 val
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTBCTL(devHandle[ehrpwm].pInst,val);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTBCTL - get Time-Base Control Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTBCTL 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTBCTL(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
* vxbEhrPwmSetTbPWMFreq - set the value of both PWM Counter Direction[CTRMODE]
*                         & Maximum Period Counter Value(PWM Frequency[TBPRD]).
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTbPWMFreq
    (
    UINT32 ehrpwm,
    UINT32 pwm_freq,
    UINT32 dir
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbPWMFreq(devHandle[ehrpwm].pInst, 
            pwm_freq, dir);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbPRD - get the value of the  Maximum Period Counter 
*                           Value(PWM Frequency[TBPRD])
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTbPRD 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetPRD(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
* vxbEhrPwmSetTbPWMDir - set the value of the current PWM Counter Direction[CTRMODE]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTbPWMDir
    (
    UINT32  ehrpwm,
    UINT32 dir
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbPWMDir(devHandle[ehrpwm].pInst, dir);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbPWMDir - get the value of the current PWM Counter Direction[CTRMODE]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTbPWMDir
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbPWMDir(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetTbCounter - set the value of the Time Base Counter [TBCNT]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTbCounter 
    (
    UINT32 ehrpwm,
    UINT32 counter
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetCounter(devHandle[ehrpwm].pInst, counter);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbCounter - get the value of the Time Base Counter [TBCNT]
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTbCounter 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetCounter(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetTbClk - set the divisor of the time base clock
*
* This routine takes the divisor for System Clock and finally sets 
* Time Base Clock Frequency(TBCLK)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
STATUS vxbEhrPwmSetTbClk
    (
    UINT32 ehrpwm,
    UINT32 tb_freq
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbClk(devHandle[ehrpwm].pInst, tb_freq);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbClk - get the value of the Time Base Clock Frequency(TBCLK)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
STATUS vxbEhrPwmGetTbClk
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbClk(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetTbPWMCfg - set the value of the current PWM Counter Direction [CTRMODE] 
*                         & Maximum Period Counter Value(PWM Frequency[TBPRD]).
*                         
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTbPWMCfg
    (
    UINT32 ehrpwm,
    UINT32 freq,        
    UINT32 dir
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbPWMCfg(devHandle[ehrpwm].pInst, freq, dir);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbPWMCfg - get the value of both PWM Counter Direction[CTRMODE]
*                         & Maximum Period Counter Value(PWM Frequency[TBPRD]).
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTbPWMCfg
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbPWMCfg(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetTbAQConfA - set the actions to take when TBCNT reach 0, TBPRD, CMPA
*                         and CMPB for eHRPWM0A
*
* Set at what point it will take action and what will be the action 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTbAQConfA
    (
    UINT32 ehrpwm,
    UINT32 action
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbAQConfA(devHandle[ehrpwm].pInst, action);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbAQConfA - get the actions to take when TBCNT reach 0, TBPRD, CMPA
*                         and CMPB for eHRPWM0A
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/

/* Set at what point it will take action and what will be the action */
STATUS vxbEhrPwmGetTbAQConfA
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbAQConfA(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetTbAQConfB - set the actions to take when TBCNT reach 0, TBPRD, CMPA
*                         and CMPB for eHRPWM0B
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetTbAQConfB
    (
    UINT32 ehrpwm,
    UINT32 action
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbAQConfB(devHandle[ehrpwm].pInst, action);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbAQConfB - get the actions to take when TBCNT reach 0, TBPRD, CMPA
*                         and CMPB for eHRPWM0B
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetTbAQConfB
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbAQConfB(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetCMPA - set the value for Comparator A (CMPA)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetCMPA
    (
    UINT32 ehrpwm,
    UINT32 ehrpwmTime
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbCOMPAConf(devHandle[ehrpwm].pInst, ehrpwmTime);
    }

/*******************************************************************************
*
* vxbEhrPwmGetCMPA - get the value for Comparator A (CMPA)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetCMPA
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbCOMPAConf(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetCMPB - set the value for Comparator B (CMPB)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetCMPB
    (
    UINT32 ehrpwm,
    UINT32 ehrpwmTime
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetTbCOMPBConf(devHandle[ehrpwm].pInst, ehrpwmTime);
    }

/*******************************************************************************
*
* vxbEhrPwmGetCMPB - get the value for Comparator B (CMPB)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetCMPB
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetTbCOMPBConf(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetAQSFRC - set Software Force & also set Action when One-Time 
* Software Force A & B Is invoked
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetAQSFRC 
    (
    UINT32 ehrpwm,
    UINT32 val
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetAQSFRC(devHandle[ehrpwm].pInst,val);
    }

/*******************************************************************************
*
* vxbEhrPwmGetAQSFRC - get AQSFRC register 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetAQSFRC 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetAQSFRC(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetIntEvt - ePWM Interrupt (EPWMx_INT) Selection Options
*
* This routine set at what event the interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetIntEvt
    (
    UINT32 ehrpwm,
    UINT32 mode
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetIntEvtTBCTL(devHandle[ehrpwm].pInst,mode);
    }

/*******************************************************************************
*
* vxbEhrPwmGetIntEvt - get ePWM Interrupt (EPWMx_INT) Selection Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetIntEvt
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetIntEvtTBCTL(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmSetIntPrd - ePWM Interrupt (EPWMx_INT) Period Options
*
* These bits determine how many selected ETSEL[INTSEL] events
* need to occur before an interrupt is generated.
* To be generated, the interrupt must be enabled (ETSEL[INT] = 1).
* If the interrupt status flag is set from a previous interrupt
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmSetIntPrd
    (
    UINT32 ehrpwm,
    UINT32 mode
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmSetIntPrdTBCTL(devHandle[ehrpwm].pInst,mode);
    }

/*******************************************************************************
*
* vxbEhrPwmGetIntPrd - get ePWM Interrupt (EPWMx_INT) Period Register bits 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetIntPrd
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetIntPrdTBCTL(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmGetTbCounter - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmRegisterISR 
    (
    UINT32 ehrpwm,
    VOIDFUNCPTR pIsr,
    void  *pArg,
    UINT32 period,
    UINT32 mode
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmPWMRegisterISR(devHandle[ehrpwm].pInst,
                    pIsr, pArg, period,mode);
    }

/*******************************************************************************
*
* vxbEhrPwmGetIntStatus - get the value of the Time base Status Register [TBSTS]
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmGetIntStatus 
    (
    UINT32 ehrpwm
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmGetIntStatus(devHandle[ehrpwm].pInst);
    }

/*******************************************************************************
*
* vxbEhrPwmEnableInt - Enable interrupt generateion from PWM
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmEnableInt 
    (
    UINT32 ehrpwm,
    UINT32 period, 
    UINT32 mode
    )
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;
    return devHandle[ehrpwm].pFunc->ehrpwmEnableInt(devHandle[ehrpwm].pInst,period,mode);
    }

/*******************************************************************************
*
* vxbEhrPwmDisableInt - disable ePWM Interrupt 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
STATUS vxbEhrPwmDisableInt(UINT32 ehrpwm)
    {
    ehrpwm--;
    if(ehrpwm >= count)
        return ERROR;

    return devHandle[ehrpwm].pFunc->ehrpwmDisableInt(devHandle[ehrpwm].pInst);
    }

