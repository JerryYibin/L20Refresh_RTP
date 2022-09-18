/* vxbCdce913Lib.c - vxBus cdce913 function interfaces file */
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
#include <vxbCdce913Lib.h>
#ifdef _WRS_CONFIG_FS_DOSFS
#include <dosFsLib.h>
#endif /* _WRS_CONFIG_FS_DOSFS */

/* locals */

LOCAL struct vxbCdce913Handle cdce913Handle;


/*****************************************************************************
*
* cdce913Register - register cdce913 set/get routine to VxBus cdce913 subsystem.
*
* This function register cdce913 set/get routine to VxBus interrupt subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void cdce913Register
    (
    VXB_DEV_ID pDev,
    VXB_CDCE913_FUNC * cdce913
    )
    {
	cdce913Handle.pCdce913Inst = pDev;
	cdce913Handle.pCdce913Func = cdce913;
	}

/*******************************************************************************
*
* vxbCdce913SetTbClk - set the divisor of the time base clock
*
* This routine takes the divisor for System Clock and finally sets 
* Time Base Clock Frequency(TBCLK)
*
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbCdce913SetTbClk
    (
    	    UINT8              buf,
    	    UINT8               offset,
    	    UINT8               length
	)
	{
	if (cdce913Handle.pCdce913Inst == NULL)
		return ERROR;
	
	return cdce913Handle.pCdce913Func->cdce913SetTbClk(cdce913Handle.pCdce913Inst, &buf,offset,length);
	}


/*******************************************************************************
*
* vxbCdce913GetTbClk - get the value of the Time Base Clock Frequency(TBCLK)
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

INT32 vxbCdce913GetTbClk
    (
    	    UINT8               offset,
    	    UINT8               length
    )

    {
	UINT8 *				data;
    if (cdce913Handle.pCdce913Inst == NULL)
        return ERROR;
    
  
   data = (UINT8 *)vxbMemAlloc (sizeof (UINT8));
   cdce913Handle.pCdce913Func->cdce913GetTbClk(cdce913Handle.pCdce913Inst, data,offset,length);
    return (CDC_MAX_FREQ/(*data));
    }


/*******************************************************************************
*
* vxbCdce913Freq - Set the Frequency and give the divisor value to Set Function
*
* RETURNS: OK, or ERROR.
*
* ERRNO: N/A
*/

STATUS vxbCdce913Freq
    (
    		UINT8				freq /* in MhZ */
			
   )
    {
	UINT8				divs;
	 
    if (cdce913Handle.pCdce913Inst == NULL)
        return ERROR;
    
    vxbCdce913SetTbClk(0x90,0x5,0x1);
    vxbCdce913SetTbClk(0x6d,0x14,0x1);
    
/*
 * calculation of 7-bit output-divider
 * 0 Reset and stand-by
 * 1 to 127 Divider value
 * */
    
    divs= CDC_MAX_FREQ/freq;
   if(divs >127 || divs < 1)
   {
	   PLL_CLK_SYNTH_MSG(PLL_CLK_SYNTH_DBG_ERR,"Divs out of range(Range :- 1 -127)..\n");
   	   return ERROR;
    }
   else
   {
	vxbCdce913SetTbClk(divs,0x16,0x1);
	vxbCdce913SetTbClk(divs,0x17,0x1);
    }
   return OK;
    }

