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
#include <subsys/gpio/vxbGpioLib.h>
#include <time.h>
#include <private/timeP.h>
#include <vxbI2cTpicLed.h> /* Driver Header FIle */

/* defines */
#define PACC_BOARD

#ifdef PACC_BOARD
#define GPIO_BANK1_PIN7 0x27
#endif

#undef DEBUG_I2C_TPICLED
#ifdef DEBUG_I2C_TPICLED
#undef LOCAL
#define LOCAL
#include <private/kwriteLibP.h>     /* _func_kprintf */

#define LED_DBG_ERR         0x00000001
#define LED_DBG_INFO        0x00000002
#define LED_DBG_ALL         0xffffffff
#define LED_DBG_OFF         0x00000000

LOCAL UINT32 ledDbgMask = LED_DBG_ALL;

#undef TPICLED_MSG
#define TPICLED_MSG(mask, ...)          	                    \
    do   	        	                                        \
	    {       	                                            \
	    if ((ledDbgMask & (mask)) || ((mask) == LED_DBG_ALL))   \
            {							     					\
		    if (_func_kprintf != NULL)                     	    \
		        {                                          	    \
         	    (* _func_kprintf)(__VA_ARGS__);        	        \
	            }                                          	    \
            }                                               	\
	    }														\
    while ((FALSE))
#else
#undef TPICLED_MSG
#define TPICLED_MSG(...)
#endif  /* TPICLED_MSG */

/* typedef */

typedef struct i2cLedCtrl
    {
    VXB_DEV_ID  pDev;
    UINT16      i2cAddr;
    }I2C_LED_CTRL;

/* VxBus methods */

LOCAL STATUS vxbI2cLedRead (VXB_DEV_ID, UINT32, UINT8*, UINT32, UINT32*);
LOCAL STATUS vxbI2cLedWrite (VXB_DEV_ID, UINT32, UINT8*, UINT32, UINT32*);

LOCAL STATUS vxbI2cTpicLedProbe (struct vxbDev * pDev);
LOCAL STATUS vxbI2cTpicLedAttach (struct vxbDev * pDev);

LOCAL VXB_DRV_METHOD vxbI2cTpicLed_methods[] = {
    { VXB_DEVMETHOD_CALL(vxbDevProbe), vxbI2cTpicLedProbe },
    { VXB_DEVMETHOD_CALL(vxbDevAttach), vxbI2cTpicLedAttach },
    VXB_DEVMETHOD_END
};

VXB_DRV vxbFdtI2cTpicLedDrv = 
    {
    { NULL },
    TPIC_LED_NAME,         /* Name */
    "TI TPIC2810 LED",    /* Description */
    VXB_BUSID_FDT,        /* Class */
    0,                    /* Flags */
    0,                    /* Reference count */
    vxbI2cTpicLed_methods  /* Method table */
    };

/* supported devices list */

LOCAL const VXB_FDT_DEV_MATCH_ENTRY i2cLedMatch[] =
    {
        {
	"ti,tpic2810",  /* compatible */
	NULL            /* no configuration */
	},
	{} /* Empty terminated list */
    };

VXB_DRV_DEF(vxbFdtI2cTpicLedDrv)

/******************************************************************************
 *
 * vxbI2cTpicLedProbe - probe for device presence at specific address
 *
 * Check for TPIC controller (or compatible) device at the specified
 * base address. We assume one is present at that address, but we need to verify.
 *
 * RETURNS: OK if probe passes and assumed a valid TPIC2810
 * (or compatible) device. ERROR otherwise.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTpicLedProbe
    (
    struct vxbDev * pDev /* Device information */
    )
    {
    return vxbFdtDevMatch (pDev, i2cLedMatch, NULL);
    }



/******************************************************************************
 *
 * vxbI2cTpicLedAttach - attach the TPIC LED device
 *
 * This is the TPIC LED initialization routine. This routines attaches i2c led 
 * device with vxbFdtI2cTpicLedDrv, it creates child device and assign resource 
 * for it.
 *
 * RETURNS: OK, or ERROR if the initialization failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTpicLedAttach
    (
    VXB_DEV_ID          pDev
    )
    {
    VXB_RESOURCE_ADR *  	pResAdr = NULL;
    VXB_RESOURCE *      	pRes;
    char                	name[MAX_DRV_NAME_LEN];
    I2C_LED_CTRL *   		pDrvCtrl;
    VXB_I2C_LED_FUNC *   	pi2cLed;

    pDrvCtrl = (I2C_LED_CTRL *) vxbMemAlloc (sizeof (I2C_LED_CTRL));
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

#ifdef PACC_BOARD
    
    vxbGpioAlloc(GPIO_BANK1_PIN7);
    vxbGpioSetDir(GPIO_BANK1_PIN7,1);
    vxbGpioSetValue(GPIO_BANK1_PIN7,1);
    vxbUsDelay(300);
    vxbGpioSetValue(GPIO_BANK1_PIN7,0);
#endif
    /* Add the device to the I/O systems device list */

    (void) snprintf (name, MAX_DRV_NAME_LEN, "/%s/%d",
			TPIC_LED_NAME, 0);

    pi2cLed = (VXB_I2C_LED_FUNC *)vxbMemAlloc (sizeof (VXB_I2C_LED_FUNC));
    if (pi2cLed == NULL)
 	goto Exit;

    pi2cLed->ledRead   = vxbI2cLedRead;
    pi2cLed->ledWrite  = vxbI2cLedWrite;
	
    if (ledDevCreate (name, pDev, pi2cLed) != OK)
        {
		vxbMemFree (pi2cLed);
		goto Exit;
	}

    vxbMemFree (pi2cLed);
    return OK;

Exit:
	if (pi2cLed != NULL)
		vxbMemFree (pi2cLed);
	if (pRes != NULL)
		(void)vxbResourceFree (pDev, pRes);
	vxbMemFree (pDrvCtrl);
	return ERROR;
     }

/*******************************************************************************
 *
 * vxbI2cTpicLedRead - read from TPIC2810 LED
 *
 * This routine performs the I2C read operation for LED TPIC2810.
 *
 * RETURNS: OK, or ERROR if read failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTpicLedRead
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
    I2C_LED_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
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

/******************************************************************************
 *
 * vxbI2cTpicLedWrite - write into TPIC2810 LED
 *
 * This routine performs the I2C write operation for LED TPIC2810.
 *
 * RETURNS: OK, or ERROR if write failed.
 *
 * ERRNO: N/A
 */

LOCAL STATUS vxbI2cTpicLedWrite
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
    I2C_LED_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg;
    
    if (pDev == NULL)
        return ERROR;

#ifdef PACC_BOARD
    /* Make  gpmc_ad7.gpio1_7 low */
    vxbGpioSetDir(GPIO_BANK1_PIN7,1);
    vxbGpioSetValue(GPIO_BANK1_PIN7,0);
#endif
    
    memset (&msg, 0, sizeof (I2C_MSG));
    msg.addr    = pDrvCtrl->i2cAddr;
    msg.scl     = FAST_MODE;
    msg.flags   = 0;
    msg.buf[0] = offset;
    msg.len     = length + CMD_LENGTH; /* data length + cmd */
    msg.wrTime  = DEFAULT_WRTIME;   
    memcpy (&msg.buf[1], buf, length);
    
    return (vxbI2cDevXfer (pDev, &msg, 1));
    }


/*******************************************************************************
*
* vxbI2cLedRead - read from an LED device
*
* This function reads the specified size from the LED. 
*
* RETURNS: OK, or ERROR if read failed.
*
* ERRNO : N/A
*/

LOCAL STATUS vxbI2cLedRead
    (
    VXB_DEV_ID          pDev,
    UINT32              offset,
    UINT8 *             buf,
    UINT32              length,
    UINT32 *            realByte
    )
    {

    if (pDev == NULL)
        return ERROR;

    *realByte = length;

    return vxbI2cTpicLedRead (pDev, buf, LED_READ_CMD, 1);
	
    }


/*******************************************************************************
*
* vxbI2cLedWrite - write to an LED device
*
* This function writes the specified data to LED device. 
*
* RETURNS: OK/ERROR
*
* ERRNO : N/A
*/

LOCAL STATUS vxbI2cLedWrite
    (
    VXB_DEV_ID pDev,
    UINT32     addr,
    UINT8 *    pDataBuf,
    UINT32     length,
    UINT32*    realByte
    )
    {
    
    if (pDev == NULL)
        return ERROR;

    if (vxbI2cTpicLedWrite(pDev, pDataBuf, LED_WRITE_CMD, length) != OK)
            return ERROR;

    *realByte = length;
    return OK;
    
    }
