/*
 * vxbTiehrpwm.c
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
#include <vxbEpwm.h>
#include <ehrpwm.h>
#include <vxbEhrPwmLib.h>

extern void ehrpwmRegister
    (
    VXB_DEV_ID pDev,
    VXB_EHRPWM_FUNC *ehrpwm,
    char *name
    );

static const VXB_FDT_DEV_MATCH_ENTRY am570xehrpwmMatch[] =
    {
    {
    "ti,am570x-ehrpwm", /* compatible */
    (void *)NULL
    },
    {} /* Empty terminated list */
    };

/*************************************************************************
 * 
 * am570xehrpwmProbe : This function finds and compare the compatible 
 * field from the dts file.
 * 
 * RETURN : if found then success , else fail.
 */
static STATUS am570xehrpwmProbe
    (
    struct vxbDev *pDev
    )
    {
    return vxbFdtDevMatch(pDev, am570xehrpwmMatch, NULL);
    }

/*******************************************************************************
* ehrpwmGetTbSts - print the time base status register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetTbSts
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Time Base Status: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_TBSTS));
    return OK;
    }

/*******************************************************************************
* ehrpwmTbSetEmulationMode - set the emulation mode
*
* These bits select the behavior of the ePWM time-base counter
* during emulation suspend events.
* Emulation debug events can be set up in the Debug Subsystem.
* 0h (R/W) = Stop after the next time-base counter increment or
* decrement
* 1h (R/W) = Stop when counter completes a whole cycle. (a) Upcount
* mode: stop when the time-base counter = period (TBCNT =
* TBPRD). (b) Down-count mode: stop when the time-base counter =
* 0000 (TBCNT = 0000h). (c) Up-down-count mode: stop when the
* time-base counter = 0000 (TBCNT = 0000h).
* 2h (R/W) = Free run
* 3h (R/W) = Free run
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmTbSetEmulationMode(VXB_DEV_ID pDev)
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    regVal &= (~PWMSS_EPWM_TBCTL_FREE_SOFT_MASK);
    regVal |= (PWMSS_EPWM_TBCTL_FREE_SOFT_STOP_AFTER_CYCLE << PWMSS_EPWM_TBCTL_FREE_SOFT_SHIFT) & PWMSS_EPWM_TBCTL_FREE_SOFT_MASK;
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmSetTBCTL - Set the Time-Base Control Register register bits
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmSetTBCTL
    (
    VXB_DEV_ID pDev,
    UINT32 action
    )
    {
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)action);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetTBCTL - print the Time-Base Control Register register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetTBCTL
    (
    VXB_DEV_ID pDev
    )
    {
    printf("TBCTL: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_TBCTL));
    return OK;
    }

/*******************************************************************************
* ehrpwmTbPwmFreqCfg - set the value of the current PWM Frequency
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
static STATUS ehrpwmTbPwmFreqCfg
    (
    VXB_DEV_ID pDev,
    UINT32     tbPeriodCount,
    UINT32     dir
    )
    {
    /* Configure PWM time base counter frequency 
     * NOTE: Only to be used when directio is up or down.Cannot be used for both
     * For Up-Down both mode use function ehrpwmTbPwmCfg
     */
    
    UINT32 regVal;
    
    if(dir == EPWM_TB_COUNTER_DIR_UP_DOWN)
        regVal = (UINT16)(tbPeriodCount - 1U) / 2U;
    else
        regVal = (UINT16)(tbPeriodCount - 1U);
    
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBPRD, (UINT16)regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetPRDCounter - print the TBPRD
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetPRDCounter
    (
    VXB_DEV_ID pDev
    )
    {
    printf("PWM PRD: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_TBPRD));
    return OK;
    }

/*******************************************************************************
* ehrpwmTbPwmDirCfg - set the value of the current PWM Counter Direction
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmTbPwmDirCfg
    (
    VXB_DEV_ID pDev,
    UINT32     counterDir
    )
    {
    /* Configure PWM time base counter direction */
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_PRDLD, EPWM_SHADOW_REG_CTRL_ENABLE);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE, counterDir);
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetCounterTbCTL - print the COUTER MODE(direction of counter)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetCounterTbCTL
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    printf("Direction of Counter : %x \n", CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE));
    return OK;
    }

/*******************************************************************************
* ehrpwmSetCounter - set the TBCNT
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmSetCounter
    (
    VXB_DEV_ID pDev,
    UINT32 counter
    )
    {
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCNT, (UINT16)counter);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetCounter - print the TBCNT
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetCounter
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Current Counter: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_TBCNT));
    return OK;
    }

/*******************************************************************************
* ehrpwmSetTbTimeBaseClkCfg - set the value of the current time base clock
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
static STATUS ehrpwmSetTbTimeBaseClkCfg
    (
    VXB_DEV_ID pDev,
    UINT32     clkDiv
    )
    {
    EHRPWMGEN *pDrvCtrl = vxbDevSoftcGet(pDev);
    /* Configure Time base clock */
    UINT32 lspClkDivSetting = 0U;
    UINT32 hspClkDiv;
    UINT32 DivVal;
    UINT32 regVal;
    
    if(clkDiv > EPWM_TBCTL_HSPCLKDIV_14)
        {
        UINT32 lspClkDiv;

        /* High speed Time-base clock pre scale value */
        hspClkDiv = PWMSS_EPWM_TBCTL_HSPCLKDIV_DIV_14;

        /* Time-base clock pre scale value */
        lspClkDiv = clkDiv/EPWM_TBCTL_HSPCLKDIV_14;
        
        while(lspClkDiv > 1U)
            {
            lspClkDiv = lspClkDiv >> 1U;
            lspClkDivSetting++;
            }
        DivVal = hspClkDiv * lspClkDivSetting;
        if(DivVal != 0)
            {
            pDrvCtrl->tbclk = SOC_EHRPWM_2_MODULE_FREQ / DivVal;
            }
        else
            {
            pDrvCtrl->tbclk = SOC_EHRPWM_2_MODULE_FREQ;
            }
        }
    else
        {
        /* High speed Time-base clock pre scale value */
        hspClkDiv = clkDiv / 2U;
        /* divide by 1 */
        lspClkDivSetting = PWMSS_EPWM_TBCTL_HSPCLKDIV_DIV_1;
        DivVal = hspClkDiv * lspClkDivSetting;
            if(DivVal != 0)
            {
            pDrvCtrl->tbclk = (SOC_EHRPWM_2_MODULE_FREQ / DivVal);
            }
            else
            {
            pDrvCtrl->tbclk = SOC_EHRPWM_2_MODULE_FREQ;
            }
        }
    
    regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CLKDIV, lspClkDivSetting);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_HSPCLKDIV, hspClkDiv);
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetTbTimeBaseClkCfg - get the value of the current time base clock
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
static STATUS ehrpwmGetTbTimeBaseClkCfg
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    printf("Time-base Clock Prescale : %x\n", CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CLKDIV));
    printf("High-Speed Time-base Clock Prescale : %x\n", CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_HSPCLKDIV));
    return OK;
    }

/*******************************************************************************
* ehrpwmTbPwmCfg - Set the value of the current PWM Counter Direction & Period
*
* Configure PWM time base counter direction and period 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmTbPwmCfg
    (
    VXB_DEV_ID pDev,
    UINT32     tbPeriodCount,
    UINT32     counterDir
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_PRDLD, EPWM_SHADOW_REG_CTRL_ENABLE);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE, counterDir);
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCTL, (UINT16)regVal);
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBPRD, (UINT16)tbPeriodCount);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetTbPwmCfg - get the value of the current PWM Counter Direction & Frequency
*
* Configure PWM time base counter direction and frequency 
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetTbPwmCfg
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_TBCTL);
    printf("Direction : %x\n", CSR_GET_FIELD(regVal, PWMSS_EPWM_TBCTL_CTRMODE));
    printf("PWM Period: %x\n", CSR_READ_2(pDev, PWMSS_EPWM_TBPRD));
    return OK;
    }

/*******************************************************************************
* ehrpwmActionQualifierConfA - set the action qualifier for ehrpwm0A
* 
* This function sets the actions when the counter reaches 0, TBPRD, CMPA when 
* counter is incrementing, CMPA when counter is decrementing , CMPB when 
* counter is incrementing & CMPB when counter is decrementing for EHRPWM0A.
* They each occupy 2 bits and following values can be set for each of them.  
* Possible values:0h (R/W) = Do nothing (action disabled)
*    1h (R/W) = Clear - force EPWMxA output low.
*    2h (R/W) = Set - force EPWMxA output high.
*    3h (R/W) = Toggle EPWMxA output - low output signal will be forced
* It is a 16 bits register . The order is 
* x x CBD  CBU CAD CAU PRD ZRO    
*    Ex. To toggle when counter reaches CMPB when counter is decrementing,
*    the value to set is  0x0c00
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmActionQualifierConfA
    (
    VXB_DEV_ID pDev,
    UINT32     action
    )
    {
    /*TODO: Configure Action Qualifier - toggle if any case happens*/
    CSR_WRITE_2(pDev, PWMSS_EPWM_AQCTLA, (UINT16)action);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetActionQualifierConfA - print the action qualifier for ehrpwm0A
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetActionQualifierConfA
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Action Qualifier A: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_AQCTLA));
    return OK;
    }

/*******************************************************************************
* ehrpwmActionQualifierConfB - set the action qualifier for ehrpwm0B
* 
* This function sets the actions when the counter reaches 0, TBPRD, CMPA when 
* counter is incrementing, CMPA when counter is decrementing , CMPB when 
* counter is incrementing & CMPB when counter is decrementing for EHRPWM0B.
* They each occupy 2 bits and following values can be set for each of them.  
* Possible values:0h (R/W) = Do nothing (action disabled)
*    1h (R/W) = Clear - force EPWMxB output low.
*    2h (R/W) = Set - force EPWMxB output high.
*    3h (R/W) = Toggle EPWMxB output - low output signal will be forced
* It is a 16 bits register . The order is 
* x x CBD  CBU CAD CAU PRD ZRO    
*    Ex. To toggle when counter reaches CMPA when counter is decrementing,
*    the value to set is  0x00c0
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmActionQualifierConfB
    (
    VXB_DEV_ID pDev,
    UINT32     action
    )
    {
    /*TODO: Configure Action Qualifier - toggle if any case happens */
    CSR_WRITE_2(pDev, PWMSS_EPWM_AQCTLB, (UINT16)action);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetActionQualifierConfB - print the action qualifier for ehrpwm0B
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetActionQualifierConfB
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Action Qualifier B: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_AQCTLB));
    return OK;
    }

/******************************************************************************
* ehrpwmCtrCompAConf - set the CMPA value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmCtrCompAConf
    (
    VXB_DEV_ID pDev,
    UINT32     cmpa
    )
    {
    /* Counter Comparator A configuration */
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPCTL);
    
    /*  LOADAMODE:
     *  Active Counter-Compare A (CMPA) Load From Shadow Select
        Mode.
        This bit has no effect in immediate mode (CMPCTL[SHDWAMODE]
        = EPWM_SHADOW_REG_CTRL_DISABLE).
     *  0h (R/W) = Load on CTR = 0: Time-base counter equal to zero
        (TBCNT = 0000h)
        1h (R/W) = Load on CTR = PRD: Time-base counter equal to period
        (TBCNT = TBPRD)
        2h (R/W) = Load on either CTR = 0 or CTR = PRD
        3h (R/W) = Freeze (no loads possible)
     */
    /*  SHADOWMODE:
     *  Counter-compare A (CMPA) Register Operating Mode.
        0h (R/W) = Shadow mode. Operates as a double buffer. All writes
        via the CPU access the shadow register.
        1h (R/W) = Immediate mode. Only the active compare register is
        used. All writes and reads directly access the active register for
        immediate compare action
     */
    
    CSR_SET_FIELD(regVal, PWMSS_EPWM_CMPCTL_SHDWAMODE, EPWM_SHADOW_REG_CTRL_ENABLE);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_CMPCTL_LOADAMODE, EPWM_CC_CMP_LOAD_MODE_CNT_EQ_PRD);
    CSR_WRITE_2(pDev,PWMSS_EPWM_CMPCTL, (UINT16)regVal);
    
    regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPA);
    regVal &= (~PWMSS_EPWM_CMPA_MASK);
    regVal |= (cmpa << PWMSS_EPWM_CMPA_SHIFT) & PWMSS_EPWM_CMPA_MASK;
    
    CSR_WRITE_2(pDev, PWMSS_EPWM_CMPA, (UINT16)regVal);
    return OK;
    }

/******************************************************************************
* ehrpwmGetCtrCompAConf - print the value for CMPA
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetCtrCompAConf
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Comparator A: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_CMPA));
    return OK;
    }

/******************************************************************************
*
* ehrpwmCtrCompBConf - set the CMPB value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmCtrCompBConf
    (
    VXB_DEV_ID pDev,
    UINT32     cmpb
    )
    {
    /* Counter Comparator B configuration */
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPCTL);
    
    /*  LOADAMODE:
        Active Counter-Compare B (CMPB) Load From Shadow Select
        Mode.
        This bit has no effect in immediate mode (CMPCTL[SHDWBMODE]
        = 1).
        0h (R/W) = Load on CTR = 0: Time-base counter equal to zero
        (TBCNT = 0000h)
        1h (R/W) = Load on CTR = PRD: Time-base counter equal to period
        (TBCNT = TBPRD)
        2h (R/W) = Load on either CTR = 0 or CTR = PRD
        3h (R/W) = Freeze (no loads possible)
     */
    /*  SHADOWMODE:
     *  Counter-compare B (CMPB) Register Operating Mode.
        0h (R/W) = Shadow mode. Operates as a double buffer. All writes
        via the CPU access the shadow register.
        1h (R/W) = Immediate mode. Only the active compare B register is
        used. All writes and reads directly access the active register for
        immediate compare action.
     */
    
    CSR_SET_FIELD(regVal, PWMSS_EPWM_CMPCTL_SHDWBMODE, EPWM_SHADOW_REG_CTRL_ENABLE);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_CMPCTL_LOADBMODE, EPWM_CC_CMP_LOAD_MODE_CNT_EQ_PRD);
    CSR_WRITE_2(pDev,PWMSS_EPWM_CMPCTL, (UINT16)regVal);
    
    regVal = CSR_READ_2(pDev, PWMSS_EPWM_CMPB);
    regVal &= (~PWMSS_EPWM_CMPB_MASK);
    regVal |= ( cmpb << PWMSS_EPWM_CMPB_SHIFT) & PWMSS_EPWM_CMPB_MASK;
    
    CSR_WRITE_2(pDev, PWMSS_EPWM_CMPB, (UINT16)regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetCtrCompBConf - print the value for CMPB
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetCtrCompBConf
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Comparator B: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_CMPB));
    return OK;
    }

/*******************************************************************************
* ehrpwmSetAQSFRC - set Software Force & also set Action when One-Time 
* Software Force A & B Is invoked
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmSetAQSFRC
    (
    VXB_DEV_ID pDev,
    UINT32 action
    )
    {
    /*TODO: Configure Action Qualifier - toggle if any case happens*/
    CSR_WRITE_2(pDev, PWMSS_EPWM_AQSFRC, (UINT16)action);
    return OK;
    }

/*******************************************************************************
* ehrpwmGetAQSFRC - print the AQSFRC register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmGetAQSFRC
    (
    VXB_DEV_ID pDev
    )
    {
    printf("AQSFRC A: %x \n", CSR_READ_2(pDev, PWMSS_EPWM_AQSFRC));
    return OK;
    }

/*******************************************************************************
* ehrpwmPWMIntEventSelect - ePWM Interrupt (EPWMx_INT) Selection Options
*
* This routine set at what event the interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmPWMIntEventSelect
    (
    VXB_DEV_ID pDev,
    UINT32 mode
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
    switch (mode)
        {
        case TBCNT_EQ_ZERO:
            // Enable event time-base counter equal to zero
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_TBPRD:
            // Enable event time-base counter equal to TIME PERIOD
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_PERIOD);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_INC_CMPA:
            // Enable event time-base counter equal to cmpa while incrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_INCR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_DEC_CMPA:
            // Enable event time-base counter equal to cmpa while decrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_DECR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_INC_CMPB:
            // Enable event time-base counter equal to cmpb while incrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_INCR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_DEC_CMPB:
            // Enable event time-base counter equal to cmpb while decrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_DECR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
        }
    return OK;
    }

/*******************************************************************************
* ehrpwmPWMGetIntEventSelect - print ePWM Interrupt (EPWMx_INT) Selection Register
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmPWMGetIntEventSelect
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
    regVal = CSR_GET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL);
    printf("PWMSS_EPWM_ETSEL_INTSEL: %x \n", regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmPWMIntPeriodSelect - ePWM Interrupt (EPWMx_INT) Selection Options
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
static STATUS ehrpwmPWMIntPeriodSelect
    (
    VXB_DEV_ID pDev,
    UINT32 mode
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);
    
    switch(mode)
        {
        case ETPS_INTPRD_DISABLE:
            // Disable the interrupt event counter
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_DISABLE);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
            
        case ETPS_INTPRD_GEN_FIRST_EVT:
            // Generate an interrupt on the first event INTCNT = 01 (first event)
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_FIRST_EVT);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
            
        case ETPS_INTPRD_GEN_SECOND_EVT:
            // Generate interrupt on ETPS[INTCNT] = 1,0 (second    event)
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_SECOND_EVT);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
            
        case ETPS_INTPRD_GEN_THIRD_EVT:
            // Generate interrupt on ETPS[INTCNT] = 1,1 (third event)
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_THIRD_EVT);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
        }
    return OK;
    }

/*******************************************************************************
* ehrpwmPWMGetIntPeriodSelect - print ePWM Interrupt (EPWMx_INT) Selection Reg
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmPWMGetIntPeriodSelect
    (
    VXB_DEV_ID pDev
    )
    {
    UINT32 regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);
    regVal = CSR_GET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD);
    printf("PWMSS_EPWM_ETPS_INTPRD: %x \n", regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmTbEnablePWMInt - Enables PWM Interrupt generation and set at which event 
*                         to generate interrupt.
*
* This routine enables the PWM interrupt generation and set at what event the 
* interrupt will be generated.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmEnablePWMInt
    (
    VXB_DEV_ID pDev,
    UINT32 period,
    UINT32 mode
    )
    {
    UINT32 regVal;
    EHRPWMGEN *pDrvCtrl;
    int ret;
    
    pDrvCtrl = vxbDevSoftcGet(pDev);
    
    ret = vxbIntEnable(pDev, pDrvCtrl->intRes);
    if(ret != OK)
        {
        vxbIntDisconnect(pDev, pDrvCtrl->intRes);
        return ERROR;
        }
    
    /* Generate an interrupt on the first event INTCNT  */
    regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETPS);
    switch(period)
        {
        case ETPS_INTPRD_DISABLE :
            // Disable the interrupt event counter
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_DISABLE);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
            
        case ETPS_INTPRD_GEN_FIRST_EVT:
            // Generate an interrupt on the first event INTCNT = 01 (first event)
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_FIRST_EVT);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
            
        case ETPS_INTPRD_GEN_SECOND_EVT:
            // Generate interrupt on ETPS[INTCNT] = 1,0 (second    event)
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_SECOND_EVT);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
            
        case ETPS_INTPRD_GEN_THIRD_EVT:
            // Generate interrupt on ETPS[INTCNT] = 1,1 (third event)
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETPS_INTPRD, PWMSS_EPWM_ETPS_INTPRD_GEN_THIRD_EVT);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
            break;
        
        }
    CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, (UINT16)regVal);
    
    /* Enable ePWM Interrupt (EPWMx_INT) Generation
     * & Enable event time-base counter equal to zero
     */
    regVal = CSR_READ_2(pDev, PWMSS_EPWM_ETSEL);
    CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTEN, PWMSS_EPWM_ETSEL_INTEN_ENABLE);
    switch (mode)
        {
        case TBCNT_EQ_ZERO:
            // Enable event time-base counter equal to zero
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_TBPRD:
            // Enable event time-base counter equal to TIME PERIOD
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_PERIOD);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_INC_CMPA:
            // Enable event time-base counter equal to cmpa while incrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_INCR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_DEC_CMPA:
            // Enable event time-base counter equal to cmpa while decrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_DECR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_INC_CMPB:
            // Enable event time-base counter equal to cmpb while incrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPB_INCR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
            
        case TBCNT_EQ_DEC_CMPB:
            // Enable event time-base counter equal to cmpb while decrementing
            CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_CMPA_DECR);
            CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
            break;
        }
    //CSR_SET_FIELD(regVal, PWMSS_EPWM_ETSEL_INTSEL, PWMSS_EPWM_ETSEL_INTSEL_CTR_0);
    CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, (UINT16)regVal);
    return OK;
    }

/*******************************************************************************
* ehrpwmTbDisablePWMInt - Disable PWM interrupt
*
* This routine disables the PWM interrupt generation
*  
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmDisablePWMInt
    (
    VXB_DEV_ID pDev
    )
    {
    EHRPWMGEN *pDrvCtrl = vxbDevSoftcGet(pDev);
    
    /* Generate an interrupt on the first event INTCNT  */
    CSR_WRITE_2(pDev,PWMSS_EPWM_ETPS, 0);
    
    /* Enable ePWM Interrupt (EPWMx_INT) Generation
     * & Enable event time-base counter equal to zero
     */
    CSR_WRITE_2(pDev,PWMSS_EPWM_ETSEL, 0);
    
    CSR_WRITE_2(pDev,PWMSS_EPWM_TBCTL, 0);
    CSR_WRITE_2(pDev, PWMSS_EPWM_TBCNT,0);
    
    vxbIntDisable(pDev, pDrvCtrl->intRes);
    return OK;
    }

/*******************************************************************************
* ehrpwmIntConnect - Registers the user application ISR
*
* This routine stores the function pointer from the user application to the 
* global function pointer object in the driver and gets called when interrupt
* is triggered.
* 
* RETURNS: OK.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmIntConnect
    (
    VXB_DEV_ID pDev,
    VOIDFUNCPTR pIsr,
    void  *pArg,
    UINT32 period,
    UINT32 mode
    )
    {
    EHRPWMGEN *pDrvCtrl = vxbDevSoftcGet(pDev);

    /* Assign the user ISR */
    pDrvCtrl->pIsr = pIsr;
    pDrvCtrl->pArg = pArg;

    /* Enable PWM Interrupts */
    ehrpwmEnablePWMInt(pDev, period, mode);
    return OK;
    }

/*******************************************************************************
* ehrpwmIntStatus - print the Interrupt status register
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static STATUS ehrpwmIntStatus
    (
    VXB_DEV_ID pDev
    )
    {
    printf("Enable ePWM Interrupt (EPWMx_INT) Generation %x\n",
            CSR_READ_2(pDev, PWMSS_EPWM_ETSEL));
    printf("Event-Trigger Prescale Register %x\n",
            CSR_READ_2(pDev, PWMSS_EPWM_ETPS));
    printf("Event-Trigger Flag Register %x\n",
            CSR_READ_2(pDev, PWMSS_EPWM_ETFLG));
    printf("Event-Trigger Clear Register %x\n",
            CSR_READ_2(pDev, PWMSS_EPWM_ETCLR));
    printf("Event-Trigger Force Register %x\n",
            CSR_READ_2(pDev, PWMSS_EPWM_ETFRC));
    return OK;
    }

/*******************************************************************************
* ehrpwmIrqHandler - interrupt handler
*
* This interrupt handler is called when interrupt is triggered and executes 
* the global function pointer object.
* 
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A.
*/
static void ehrpwmIrqHandler
    (
    void *arg
    )
    {
    VXB_DEV_ID pDev = arg;
    EHRPWMGEN *pDrvCtrl = vxbDevSoftcGet(pDev);

    CSR_WRITE_2(pDev, PWMSS_EPWM_ETCLR, 1);
    if(pDrvCtrl->pIsr != NULL)
        (*pDrvCtrl->pIsr)(pDrvCtrl->pArg);
    }

/***************************************************************************
 * am570xehrpwmAttach : This function allocates driver structure, 
 * registers and map it to  system.It always does the basic hardware and 
 * software initialization needed for driver and hardware functioning.
 * 
 * RETURN :  attach success , else fail.
 */
static STATUS am570xehrpwmAttach
    (
    VXB_DEV_ID pDev
    )
    {
    VXB_FDT_DEV *pFdtDev;
    VXB_RESOURCE_ADR *pResAdr;
    VXB_RESOURCE *pRes = NULL;
    VXB_EHRPWM_FUNC *ehrpwm = NULL;
    EHRPWMGEN *pEhrpwmObj;

    if(pDev == NULL)
        {
        return ERROR;
        }
    
    pFdtDev = vxbFdtDevGet(pDev);
    if(pFdtDev == NULL)
        {
        return ERROR;
        }

    /* Allocate Driver Memory */
    pEhrpwmObj = (EHRPWMGEN *)vxbMemAlloc(sizeof(EHRPWMGEN));
    if(pEhrpwmObj == NULL)
        goto attach_error;

    vxbDevSoftcSet(pDev, pEhrpwmObj);
    pEhrpwmObj->pDev = pDev;

    /* Allocate resource */
    pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    if(pRes == NULL)
        goto attach_error;

    pResAdr = (VXB_RESOURCE_ADR *)pRes->pRes;
    if(pResAdr == NULL)
        goto attach_error;

    pEhrpwmObj->regHandle = pResAdr->pHandle;
    pEhrpwmObj->regBase = (void *)pResAdr->virtAddr;

    if(vxbPinMuxEnable(pDev) != OK)
        {
        goto attach_error;
        }

    if(vxbClkEnableAll(pDev) != OK)
        {
        goto attach_error;
        }

    /* Allocate and register library function for i/o subsytem */
    ehrpwm = (VXB_EHRPWM_FUNC *)vxbMemAlloc(sizeof(VXB_EHRPWM_FUNC));
    if(ehrpwm == NULL)
        goto attach_error;

    ehrpwm->ehrpwmGetTbStatus = ehrpwmGetTbSts;

    ehrpwm->ehrpwmSetEmulation = ehrpwmTbSetEmulationMode;
    ehrpwm->ehrpwmSetTBCTL = ehrpwmSetTBCTL;
    ehrpwm->ehrpwmGetTBCTL = ehrpwmGetTBCTL;

    ehrpwm->ehrpwmSetTbPWMFreq = ehrpwmTbPwmFreqCfg;
    ehrpwm->ehrpwmGetPRD = ehrpwmGetPRDCounter;

    ehrpwm->ehrpwmSetTbPWMDir = ehrpwmTbPwmDirCfg;
    ehrpwm->ehrpwmGetTbPWMDir = ehrpwmGetCounterTbCTL;

    ehrpwm->ehrpwmSetCounter = ehrpwmSetCounter;
    ehrpwm->ehrpwmGetCounter = ehrpwmGetCounter;

    ehrpwm->ehrpwmSetTbClk = ehrpwmSetTbTimeBaseClkCfg;
    ehrpwm->ehrpwmGetTbClk = ehrpwmGetTbTimeBaseClkCfg;

    ehrpwm->ehrpwmSetTbPWMCfg = ehrpwmTbPwmCfg;
    ehrpwm->ehrpwmGetTbPWMCfg = ehrpwmGetTbPwmCfg;

    ehrpwm->ehrpwmSetTbAQConfA = ehrpwmActionQualifierConfA;
    ehrpwm->ehrpwmGetTbAQConfA = ehrpwmGetActionQualifierConfA;

    ehrpwm->ehrpwmSetTbAQConfB = ehrpwmActionQualifierConfB;
    ehrpwm->ehrpwmGetTbAQConfB = ehrpwmGetActionQualifierConfB;

    ehrpwm->ehrpwmSetTbCOMPAConf = ehrpwmCtrCompAConf;
    ehrpwm->ehrpwmGetTbCOMPAConf = ehrpwmGetCtrCompAConf;

    ehrpwm->ehrpwmSetTbCOMPBConf = ehrpwmCtrCompBConf;
    ehrpwm->ehrpwmGetTbCOMPBConf = ehrpwmGetCtrCompBConf;

    ehrpwm->ehrpwmSetAQSFRC = ehrpwmSetAQSFRC;
    ehrpwm->ehrpwmGetAQSFRC = ehrpwmGetAQSFRC;

    ehrpwm->ehrpwmSetIntEvtTBCTL = ehrpwmPWMIntEventSelect;
    ehrpwm->ehrpwmGetIntEvtTBCTL = ehrpwmPWMGetIntEventSelect;

    ehrpwm->ehrpwmSetIntPrdTBCTL = ehrpwmPWMIntPeriodSelect;
    ehrpwm->ehrpwmGetIntPrdTBCTL = ehrpwmPWMGetIntPeriodSelect;

    ehrpwm->ehrpwmPWMRegisterISR = ehrpwmIntConnect;
    ehrpwm->ehrpwmGetIntStatus = ehrpwmIntStatus;

    ehrpwm->ehrpwmEnableInt = ehrpwmEnablePWMInt;
    ehrpwm->ehrpwmDisableInt = ehrpwmDisablePWMInt;

    ehrpwmRegister(pDev, ehrpwm, pFdtDev->name);
    
    /* enable and connect ISR */
    pEhrpwmObj->intRes = vxbResourceAlloc(pDev, VXB_RES_IRQ, 0);
    if(pEhrpwmObj->intRes != NULL)
        {
        if(vxbIntConnect(pDev, pEhrpwmObj->intRes, (VOIDFUNCPTR)ehrpwmIrqHandler, pDev) != OK)
            {
            }
        
        if(vxbIntEnable(pDev, pEhrpwmObj->intRes) != OK)
            {
            vxbIntDisconnect(pDev, pEhrpwmObj->intRes);
            goto attach_error;
            }
        }
    return OK;
    
attach_error:
    if(pRes != NULL)
        vxbResourceFree(pDev, pRes);
    
    if(pEhrpwmObj->intRes != NULL)
        vxbResourceFree(pDev, pEhrpwmObj->intRes);
    
    if(pEhrpwmObj != NULL)
        vxbMemFree(pEhrpwmObj);

    if(ehrpwm != NULL)
        vxbMemFree(ehrpwm);
    return ERROR;
    }

static VXB_DRV_METHOD am570xehrpwmMethods[] =
    {
    {VXB_DEVMETHOD_CALL(vxbDevProbe), (FUNCPTR)am570xehrpwmProbe},
    {VXB_DEVMETHOD_CALL(vxbDevAttach),(FUNCPTR)am570xehrpwmAttach},
    VXB_DEVMETHOD_END
    };

VXB_DRV vxbOfAm570xehrpwmDrv =
    {
    { NULL } ,
    "ti,am5707-ehrpwm",                 /* Name */
    "TI AM570x EPWM Driver",         /* Description */
    VXB_BUSID_FDT,                      /* Class */
    0,                                  /* Flags */
    0,                                  /* Reference count */
    (VXB_DRV_METHOD*)&am570xehrpwmMethods  /* Method table */
    };

VXB_DRV_DEF(vxbOfAm570xehrpwmDrv)

