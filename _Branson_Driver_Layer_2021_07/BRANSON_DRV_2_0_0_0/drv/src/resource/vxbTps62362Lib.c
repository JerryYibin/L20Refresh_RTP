/*
 * vxbTps62362Lib.c
 *
 *  Created on: Jun 13, 2017
 *      Author: ee209834
 */
/* vxbTps62362Lib.c - vxBus tps62362 function interfaces file */
/*
 * Copyright - To be Done
 */

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hwif/vxBus.h>
#include <vxbTps62362Lib.h>
#ifdef _WRS_CONFIG_FS_DOSFS
#include <dosFsLib.h>
#endif /* _WRS_CONFIG_FS_DOSFS */

#define MODE_BIT 7

UINT8 vxbI2cGetOutRegister(void);
/* locals */
LOCAL VXB_TPS62362_HANDLE tps62362Handle;


/*****************************************************************************
*
* tps62362Register - register tps62362 set/get routine to I2c Bus tps62362 subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void tps62362Register
    (
    VXB_DEV_ID				pDev,
	VXB_TPS62362_FUNC	*	tps62362
    )
    {
	tps62362Handle.pTps62362Inst = pDev;
	tps62362Handle.pTps62362Func = tps62362;
	}



UINT8 vxbI2cGetOutRegister
	(
	void
	)
	{
	UINT8               offset = 0x04; /* Control register */
	UINT8 				curReg = 0;
	UINT8 * 			data;
	UINT8               length = 0x01;
	
	if (tps62362Handle.pTps62362Inst == NULL)
		return 0;
	
	data = (UINT8 *)vxbMemAlloc (sizeof (UINT8));
	tps62362Handle.pTps62362Func->tps62362GetVoltage(tps62362Handle.pTps62362Inst, data,offset,length);
	return (*data & 0x60 ) >> 5;
	}

/*******************************************************************************
 * vxbTps62362ReadData	-	Read the data from the registers
 * 
 * RETURNS: OK, or ERROR.
 *  
 * ERRNO: N/A
 */

STATUS vxbTps62362ReadData
	(
	UINT8 *				data,
	UINT8               offset,
	UINT8               length
	)
	{
	if (tps62362Handle.pTps62362Inst == NULL)
		return 0;
	
	data = (UINT8 *)vxbMemAlloc (sizeof (UINT8));	
	return tps62362Handle.pTps62362Func->tps62362GetVoltage(tps62362Handle.pTps62362Inst, data,offset,length);
}

/***************************************************************************
 * vxbTps62362WriteData	-	Write the data on to the registers
 * 
 * RETURNS: OK, or ERROR.
 *  
 * ERRNO: N/A
 */

STATUS vxbTps62362WriteData
	(
	UINT8	 			data,
	UINT8               offset,
	UINT8               length
	){
		
	if (tps62362Handle.pTps62362Inst == NULL)
		return 0;
		
	return tps62362Handle.pTps62362Func->tps62362SetVoltage(tps62362Handle.pTps62362Inst,&data, offset, length);
}

	
/*******************************************************************************
*
* vxbTps62362SetVoltage - set the output voltage of the TPS62362 registers.
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbTps62362SetVoltage
    (
    UINT32              voltage
	)
	{
	 UINT8               offset = vxbI2cGetOutRegister();
	 UINT8               length = 0x01;
	 UINT8				 vol;
	 if(voltage < MIN_VOLTAGE || voltage > MAX_VOLTAGE)
		 return ERROR;
	 if((voltage == V1) || (voltage == V2) || (voltage == V3))
		 voltage = (voltage - MIN_VOLTAGE)/10;
	 else
		 return ERROR;
			 
	 vol = (UINT8)voltage;
	 
	 if(vxbI2cTps62362GetMode())
		 vol = (vol | (FORCE_MODE << MODE_BIT));
	 
	 if (tps62362Handle.pTps62362Inst == NULL)
		 return ERROR;
	 
	 return tps62362Handle.pTps62362Func->tps62362SetVoltage(tps62362Handle.pTps62362Inst,&vol, offset, length);
	}


/*******************************************************************************
*
* vxbTps62362GetVoltage - get the value at the output voltage of the TPS62362
* 						 from the registers.
*
* RETURNS: voltage or -1.
*
* ERRNO: N/A
*/
	
UINT32 vxbTps62362GetVoltage
    (
    void
    )
    {
	UINT8               offset = vxbI2cGetOutRegister();
	UINT8               length = 0x01;
	UINT8 * 			data;
	
	
    if (tps62362Handle.pTps62362Inst == NULL)
        return 0;
    data = (UINT8 *)vxbMemAlloc (sizeof (UINT8));
    tps62362Handle.pTps62362Func->tps62362GetVoltage(tps62362Handle.pTps62362Inst, data,offset,length);
    return (((*data & 0x7f) *10) + MIN_VOLTAGE);
    }

/* ******************************************************************************
 *  vxbI2cTps62362SetMode - set the mode of operation.
 *  mode's:	FORCE_MODE	 Forced PWM mode operation
 *  		PSM_MODE	 PFM / PWM mode operation
 *  RETURN OK, or ERROR
 *  ERRNO: N/A
 */

STATUS vxbI2cTps62362SetMode(UINT8 mode) {
	
	UINT8 				curVal = 0;
	UINT8               offset = vxbI2cGetOutRegister();
	UINT8               length = 0x01;
	UINT8 * 			data;
	
	if (tps62362Handle.pTps62362Inst == NULL)
		return 0;
	
	data = (UINT8 *)vxbMemAlloc (sizeof (UINT8));
	tps62362Handle.pTps62362Func->tps62362GetVoltage(tps62362Handle.pTps62362Inst, data,offset,length);
	
	if(mode)
		curVal = ((*data) | (FORCE_MODE << MODE_BIT));
	else 
		curVal = ((*data) & (~(FORCE_MODE << MODE_BIT)));
	
	return tps62362Handle.pTps62362Func->tps62362SetVoltage(tps62362Handle.pTps62362Inst,&curVal, offset, length);
}

/* ******************************************************************************
 *  vxbI2cTps62362GetMode - get/read the mode of operation.
 *  mode's:	FORCE_MODE	 Forced PWM mode operation
 *  		PSM_MODE	 PFM / PWM mode operation
 *  RETURN : Mode of operation 1/0 ( FORCE_MODE / PSM_MODE).
 *  ERRNO: N/A
 */

UINT8 vxbI2cTps62362GetMode(void) {
	UINT8               offset = vxbI2cGetOutRegister();
	UINT8               length = 0x01;
	UINT8 * 			data;
	
	if (tps62362Handle.pTps62362Inst == NULL)
		return 0;
	data = (UINT8 *)vxbMemAlloc (sizeof (UINT8));
	tps62362Handle.pTps62362Func->tps62362GetVoltage(tps62362Handle.pTps62362Inst, data,offset,length);
		
	if( *data & (FORCE_MODE << MODE_BIT))
		return FORCE_MODE;
	return PSM_MODE;
}


