/* vxbEqepLib.c - vxBus EQEP function interfaces file */
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

This file implements the vxbus EQEP functionalities.
All this functions are used to operate the EQEP device.
*/

/* includes */
#include <stdio.h>
#include <string.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <vxbEqepLib.h>

#define VXB_EQEP_DEBUG

#define EQEP_NUM  3

struct vxbEqepHandle
    {
    VXB_DEV_ID           pInst;
    VXB_PWMSS_EQEP_FUNC *pFunc;
    char                 name[FDT_NODE_NAME_LEN+1];
    };

/* locals */
static UINT32 count = 0;
static struct vxbEqepHandle devHandle[EQEP_NUM] = {0};

/*****************************************************************************
*
* eqepRegister - register eqep set/get routine to VxBus eqep subsystem.
*
* This function register eqep set/get routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void eqepRegister
    (
    VXB_DEV_ID pDev,
    VXB_PWMSS_EQEP_FUNC *eqep,
    char *name
    )
    {
    if(count<EQEP_NUM)
        {
        devHandle[count].pInst = pDev;
        devHandle[count].pFunc = eqep;
        bcopy(name, devHandle[count].name, FDT_NODE_NAME_LEN);
        count++;
        }
    }

/*******************************************************************************
*
* eqepCountGet - get the count of the current EQEP 
*
* RETURNS: eQEP attached current.
*
* ERRNO: N/A
*/
UINT32 eqepDevNumGet()
    {
    UINT32 i;
    for(i=0; i<count; i++)
        {
        printf("eQEP Dev%d: %s\r\n", i+1, devHandle[i].name);
        }
    return count;
    }

/*******************************************************************************
*
* vxbEqepShow - display the value of the current EQEP 
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
STATUS vxbEqepShow
    (
    UINT32 eqep
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepGet(devHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepGetMaxCount - get the maximum counter value
*
* RETURNS: maximum counter value.
*
* ERRNO: N/A
*/
STATUS vxbEqepGetMaxCount
    (
    UINT32 eqep
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepGetMaxCount(devHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepSetMaxCount - set the maximum counter value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
STATUS vxbEqepSetMaxCount
    (
    UINT32 eqep,
    UINT32 maxCount
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepSetMaxCount(devHandle[eqep].pInst, maxCount);
    }

/*******************************************************************************
*
* vxbEqepGetPosCount - get the current position counter value
*
* RETURNS: get current counter.
*
* ERRNO: N/A
*/
STATUS vxbEqepGetPosCount
    (
    UINT32 eqep
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepGetPosCount(devHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepGetInitCount - get the initial counter value
*
* RETURNS: initial counter value.
*
* ERRNO: N/A
*/
STATUS vxbEqepGetInitCount
    (
    UINT32 eqep
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepGetInitCount(devHandle[eqep].pInst);
    }

/*******************************************************************************
*
* vxbEqepSetInitCount - set the initial counter value
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
STATUS vxbEqepSetInitCount
    (
    UINT32 eqep,
    UINT32 initCount
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepSetInitCount(devHandle[eqep].pInst, initCount);
    }

/*******************************************************************************
*
* vxbEqepGetDirection - get the initial counter value
*
* RETURNS: direction value
*
* ERRNO: N/A
*/
STATUS vxbEqepGetDirection
    (
    UINT32 eqep
    )
    {
    eqep--;
    if(eqep >= count)
        return ERROR;
   
    return devHandle[eqep].pFunc->eqepGetDirection(devHandle[eqep].pInst);
    }

