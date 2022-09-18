/*
 * vxbI2cTps62362.c
 *
 *  Created on: Jun 13, 2017
 *      Author: ee209834
 */
/* Copyright - To be done */

/* includes */	
#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lstLib.h>
#include <iosLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtLib.h>
#include <hwif/buslib/vxbI2cLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <time.h>
#include <private/timeP.h>

#include <vxbTps62362Lib.h>


#define DEFAULT_WRTIME  		2
#define TPS62362_NAME			"tps62362"
#define TPS62362_DESCRIPTION	"TI tps62362 Flexible voltage regulator"

/* typedef */
typedef struct i2cTps62362Ctrl
    {
    VXB_DEV_ID  pDev;
    UINT16      i2cAddr;
    }I2C_TPS_CTRL;

LOCAL STATUS vxbI2cTps62362Read (VXB_DEV_ID, UINT8*, UINT8, UINT8);
LOCAL STATUS vxbI2cTps62362Write (VXB_DEV_ID, UINT8*, UINT8, UINT8);

LOCAL STATUS vxbI2cTps62362Probe (struct vxbDev * pDev);
LOCAL STATUS vxbI2cTps62362Attach (struct vxbDev * pDev);

LOCAL VXB_DRV_METHOD vxbI2cTps62362_methods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe), vxbI2cTps62362Probe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), vxbI2cTps62362Attach },
    VXB_DEVMETHOD_END
	};

VXB_DRV vxbFdtI2cTps62362Drv = 
    {
    { NULL },
	TPS62362_NAME,         		/* Name */
	TPS62362_DESCRIPTION,    	/* Description */
    VXB_BUSID_FDT,        		/* Class */
    0,                    		/* Flags */
    0,                    		/* Reference count */
    vxbI2cTps62362_methods  	/* Method table */
    };

VXB_DRV_DEF(vxbFdtI2cTps62362Drv)

/* supported devices list */

LOCAL const VXB_FDT_DEV_MATCH_ENTRY i2cTps62362Match[] =
    {
    {
	"ti,tps62362",  /* compatible */
	NULL            /* no configuration */
	},
	{} 				/* Empty terminated list */
    };


/************************************************************************
 *
 * vxbI2cTps62362Probe - probe for device presence at specific address
 *
 * Check for TPS62362 controller (or compatible) device at the specified
 * base address. We assume one is present at that address, but we need to 
 * verify.
 *
 * RETURNS: OK if probe passes and assumed a valid TPS62362
 * (or compatible) device. ERROR otherwise.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTps62362Probe
    (
    struct vxbDev * pDev 	/* Device information */
    )
    {
	return vxbFdtDevMatch (pDev, i2cTps62362Match, NULL);
    }


/**********************************************************************
 *
 * vxbI2cTps62362Attach - attach the TPS62362 device
 *
 * This is the TPS62362 Voltage Regulator initialization routine. This 
 * routines attaches i2c tps62362 device with vxbFdtI2cTps62362Drv, 
 * it creates child device and assign resource for it.
 * 
 * RETURNS: OK, or ERROR if the initialization failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTps62362Attach
    (
    VXB_DEV_ID          pDev
    )
    {
    VXB_RESOURCE_ADR *  	pResAdr = NULL;
    VXB_RESOURCE *      	pRes;
    char                	name[MAX_DRV_NAME_LEN];
    I2C_TPS_CTRL *   		pDrvCtrl;
    VXB_TPS62362_FUNC * 	tps62362;
    
	pDrvCtrl = (I2C_TPS_CTRL *) vxbMemAlloc (sizeof (I2C_TPS_CTRL));
    if (pDrvCtrl == NULL)
    	return ERROR;

    /* retrieve the i2cAddress */

    pRes = vxbResourceAlloc (pDev, VXB_RES_MEMORY, 0);
    if (pRes == NULL)
 	goto Exit;

    pResAdr = (VXB_RESOURCE_ADR *) pRes->pRes;
    if (pResAdr == NULL)
	goto Exit;

    pDrvCtrl->i2cAddr = (UINT16)pResAdr->virtual;
        
    /* save instance ID */

    pDrvCtrl->pDev = pDev;

    /* save pDrvCtrl in VXB_DEVICE structure */

    vxbDevSoftcSet(pDev, pDrvCtrl);

    tps62362 = (VXB_TPS62362_FUNC *)vxbMemAlloc (sizeof (VXB_TPS62362_FUNC));
    tps62362->tps62362GetVoltage = vxbI2cTps62362Read;
    tps62362->tps62362SetVoltage = vxbI2cTps62362Write;
    tps62362Register(pDev,tps62362);
    return OK;

Exit:
	if (pRes != NULL)
		(void)vxbResourceFree (pDev, pRes);
	vxbMemFree (pDrvCtrl);
	return ERROR;
     }

/**********************************************************************
 *
 * vxbI2cTps62362Read - read from TPS62362
 *
 * This routine performs the I2C read operation for LED TPIC2810.
 *
 * RETURNS: OK, or ERROR if read failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTps62362Read
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
	
	I2C_TPS_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg[2];
    UINT8               msgbuf;
    
    if (pDev == NULL)
        return ERROR;
    
    memset (msg, 0, sizeof (I2C_MSG) * 2);
    
    msgbuf      = offset;
    msg[0].addr = pDrvCtrl->i2cAddr;
    msg[0].scl  = FAST_MODE;
    msg[0].flags  = I2C_M_WR;
    msg[0].buf  = &offset;
    msg[0].len  = CMD_LENGTH;
    
    msg[1].addr = pDrvCtrl->i2cAddr;
    msg[1].scl  = FAST_MODE;
    msg[1].buf  = buf;
    msg[1].len  = length;
    msg[1].flags  = I2C_M_RD;
    

    return (vxbI2cDevXfer (pDev, msg, 2));
    }

/*********************************************************************
 *
 * vxbI2cTps62362Write - write into TPS62362
 *
 * This routine performs the I2C write operation for TPS62362.
 *
 * RETURNS: OK, or ERROR if write failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTps62362Write
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
	I2C_TPS_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg;
    
    if (pDev == NULL)
        return ERROR;
    
    memset (&msg, 0, sizeof (I2C_MSG));
    msg.addr    = pDrvCtrl->i2cAddr;
    msg.scl     = FAST_MODE;
    msg.flags   = I2C_M_WR;
    msg.buf[0]  = offset;
    msg.len     = length + CMD_LENGTH; /* data length + cmd */
    msg.wrTime  = DEFAULT_WRTIME;   
    memcpy (&msg.buf[1], buf, length);
    
    return (vxbI2cDevXfer (pDev, &msg, 1));
    }


