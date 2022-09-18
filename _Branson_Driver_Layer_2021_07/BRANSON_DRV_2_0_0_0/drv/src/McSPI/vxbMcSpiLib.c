/* vxbMcSpiLib.c - vxBus McSpi function interfaces file */
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
#include <semLib.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <vxbMcSpiLib.h>

#define MCSPI_NUM  4
    
struct vxbMcSpiHandle
    {
    VXB_DEV_ID       pInit;
    VXB_MC_SPI_FUNC *pFunc;
    SEM_ID           semId;
    char             name[FDT_NODE_NAME_LEN+1];
    };

/* locals */
static UINT32 count = 0;
static struct vxbMcSpiHandle devHandle[MCSPI_NUM] = {0};

void mcspiRegister
    (
    VXB_DEV_ID pDev,
    VXB_MC_SPI_FUNC *mcspi,
    char *name
    )
    {
    if(count<MCSPI_NUM)
        {
        devHandle[count].pInit = pDev;
        devHandle[count].pFunc = mcspi;
        devHandle[count].semId = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
        bcopy(name, devHandle[count].name, FDT_NODE_NAME_LEN);
        count++;
        }
    }
UINT32 mcspiDevNumGet()
    {
    UINT32 i;
    for(i=0; i<count; i++)
        {
        printf("McSpi Dev%d: %s\r\n", i+1, devHandle[i].name);
        }
    return count;
    }

/*******************************************************************************
* vxbMcSpiTrans - Spi Transfer Function
*
* This routine is used to perform one transmission. 
* It is the interface which can be called by SPI device driver 
* to send and receive data via the SPI controller.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/
STATUS vxbMcSpiTrans
    (
    UINT32 channel,
    SPI_HARDWARE *devInfo,
    SPI_TRANSFER *pPkg
    )
    {
    STATUS status;

    channel--;
    if((UINT32)channel >= count)
        {
        return ERROR;
        }

    semTake(devHandle[channel].semId, WAIT_FOREVER);
    status = devHandle[channel].pFunc->mcspiTrans(devHandle[channel].pInit, devInfo, pPkg);
    semGive(devHandle[channel].semId);
    return status;
    }

