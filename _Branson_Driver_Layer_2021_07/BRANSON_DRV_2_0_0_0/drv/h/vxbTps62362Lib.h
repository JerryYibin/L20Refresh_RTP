/* vxbTps62362Lib.h - VxBus Tps62362 interfaces header file */

/*
 * Copyright - To be Done 
 */

/*
DESCRIPTION

This library provides the Tps62362 voltage regulator specific interfaces

*/

#ifndef __INCvxbTps62362Libh
#define __INCvxbTps62362Libh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>

/* voltage range in millivolts */
#define MIN_VOLTAGE 770
#define MAX_VOLTAGE 1400

/* Supported input voltages */
enum voltages {V1 = 1200, V2 = 1260, V3 = 1330  };

/* mode of operations */
#define FORCE_MODE	1	/* Forced PWM mode operation */
#define PSM_MODE	0	/* PFM / PWM mode operation */
#define CMD_LENGTH	1	/* Command length */

typedef struct vxbTps62362Functionality
    {
    STATUS (*tps62362GetVoltage)(
    		VXB_DEV_ID 			pInst,
    	    UINT8 *             buf,
    	    UINT8               offset,
    	    UINT8               length);
    STATUS (*tps62362SetVoltage)(
    		VXB_DEV_ID 			pInst, 
    	    UINT8 *             buf,
    	    UINT8               offset,
    	    UINT8               length);

	} VXB_TPS62362_FUNC;

	
typedef struct vxbTps62362Handle
    {
		VXB_TPS62362_FUNC *			pTps62362Func;
		VXB_DEV_ID                  pTps62362Inst;
    }VXB_TPS62362_HANDLE;


STATUS vxbTps62362ReadData
		(
		UINT8 	* 		buf,
		UINT8			offset,
		UINT8			length
		);
STATUS vxbTps62362WriteData(
		UINT8	 			data,
		UINT8               offset,
		UINT8               length);


UINT32 vxbTps62362GetVoltage (void);
STATUS vxbTps62362SetVoltage (UINT32	voltage);
UINT8 vxbI2cTps62362GetMode (void);
STATUS vxbI2cTps62362SetMode (UINT8		mode);

void tps62362Register
    (
    VXB_DEV_ID 			pDev,
	VXB_TPS62362_FUNC * tps62362
    );

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbTps62362Libh */

	
