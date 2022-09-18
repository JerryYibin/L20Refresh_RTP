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
#include <time.h>
#include <private/timeP.h>

#include <vxbCdce913Lib.h>

LOCAL STATUS vxbI2cReadCdce (VXB_DEV_ID, UINT8*, UINT8, UINT8);
LOCAL STATUS vxbI2cWriteCdce (VXB_DEV_ID, UINT8*, UINT8, UINT8);

LOCAL STATUS vxbI2cPLLClkSynthProbe (struct vxbDev * pDev);
LOCAL STATUS vxbI2cPLLClkSynthAttach (struct vxbDev * pDev);

LOCAL VXB_DRV_METHOD vxbI2cPLLClkSynth_methods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe), vxbI2cPLLClkSynthProbe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), vxbI2cPLLClkSynthAttach },
    VXB_DEVMETHOD_END
};

VXB_DRV vxbFdtI2cPLLClkSynthDrv = 
    {
    { NULL },
	PLL_CLK_SYNTH_NAME,         /* Name */
    "TI CDCE913 "
    "Flexible Low Power "
    "LVCMOS Clock Generator",    /* Description */
    VXB_BUSID_FDT,       		 /* Class */
    0,                    		/* Flags */
    0,                    		/* Reference count */
    vxbI2cPLLClkSynth_methods  /* Method table */
    };


LOCAL const VXB_FDT_DEV_MATCH_ENTRY i2cPLLClkSynthMatch[] =
    {
        {
        "ti,cdce913",  /* compatible */
		NULL            /* no configuration */
        },
		{} /* Empty terminated list */
    };

VXB_DRV_DEF(vxbFdtI2cPLLClkSynthDrv)

/******************************************************************************
 *
 * vxbI2cPLLClkSynthProbe - probe for device presence at specific address
 *
 * Check for PLL controller (or compatible) device at the specified
 * base address. We assume one is present at that address, but we need to verify.
 *
 * RETURNS: OK if probe passes and assumed a valid PLL Clk Synth
 * (or compatible) device. ERROR otherwise.
 *
 * ERRNO: N/A
 */
LOCAL STATUS vxbI2cPLLClkSynthProbe
    (
    struct vxbDev * pDev /* Device information */
    )
    {
    return vxbFdtDevMatch (pDev, i2cPLLClkSynthMatch, NULL);
    }

/******************************************************************************
 *
 * vxbI2cPLLClkSynthAttach - attach the PLL Clk Synth device
 *
 * This is the PLL initialization routine. This routines attaches PLL 
 * device with vxbFdtI2cPLLClkSynthDrv, it creates child device and assign resource 
 * for it.
 *
 * RETURNS: OK, or ERROR if the initialization failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cPLLClkSynthAttach
    (
    VXB_DEV_ID          pDev
    )
    {
    VXB_RESOURCE_ADR *  		pResAdr = NULL;
    VXB_RESOURCE *      		pRes;
    char                		name[MAX_DRV_NAME_LEN];
    I2C_PLL_CLK_SYNTH_CTRL *   	pDrvCtrl;
    VXB_CDCE913_FUNC * 			cdce913;

    pDrvCtrl = (I2C_PLL_CLK_SYNTH_CTRL *) vxbMemAlloc (sizeof (I2C_PLL_CLK_SYNTH_CTRL));
    if (pDrvCtrl == NULL)
    	return ERROR;
    

    /* retrive the i2cAddress */

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

    /* Registering Library */
    
    cdce913 = (VXB_CDCE913_FUNC *)vxbMemAlloc (sizeof (VXB_CDCE913_FUNC));
    
    cdce913->cdce913GetTbClk = vxbI2cReadCdce;
    cdce913->cdce913SetTbClk = vxbI2cWriteCdce;
    
    cdce913Register(pDev,cdce913);

    return OK;

Exit:

	if (pRes != NULL)
		(void)vxbResourceFree (pDev, pRes);
	vxbMemFree (pDrvCtrl);
	return ERROR;
     }

/*******************************************************************************
 *
 * vxbI2cReadCdce - read from PLL Clk Synth
 *
 * This routine performs the PLL read operation for PLL CDCE913.
 *
 * RETURNS: OK, or ERROR if read failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cReadCdce
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
	I2C_PLL_CLK_SYNTH_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg[2];
    UINT8               msgbuf;
   
    if (pDev == NULL)
        return ERROR;
    
    memset (msg, 0, sizeof (I2C_MSG) * 2);
    
    offset |= CLK_SYNTH_BYTE_MODE;
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

/******************************************************************************
 *
 * vxbI2cWriteCdce - write into PLL Clk Synth
 *
 * This routine performs the I2C write operation for PLL CDCE913.
 *
 * RETURNS: OK, or ERROR if write failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cWriteCdce
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
	I2C_PLL_CLK_SYNTH_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg;
    
    if (pDev == NULL)
        return ERROR;
    
    offset |= CLK_SYNTH_BYTE_MODE;
    
    memset (&msg, 0, sizeof (I2C_MSG));
    msg.addr    = pDrvCtrl->i2cAddr;
    msg.scl     = FAST_MODE;
    msg.flags   = I2C_M_WR;
    msg.buf[0] = offset;
    msg.len     = length + CMD_LENGTH; /* data length + cmd */
    msg.wrTime  = DEFAULT_WRTIME;   
    memcpy (&msg.buf[1], buf, length);

    return (vxbI2cDevXfer (pDev, &msg, 1));
    }
