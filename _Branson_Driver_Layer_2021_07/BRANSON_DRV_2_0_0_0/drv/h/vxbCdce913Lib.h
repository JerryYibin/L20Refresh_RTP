/* vxbCdce913Lib.h - VxBus Cdce913 interfaces header file */

/*
 * Copyright - To be Done
 */

/*
DESCRIPTION

This library provides the Cdce913 specific interfaces

*/

#ifndef __INCvxbCdce913Libh
#define __INCvxbCdce913Libh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>

#define CLK_SYNTH_BYTE_MODE 0x80

#undef DEBUG_I2C_PLL_CLK_SYNTH
#ifdef DEBUG_I2C_PLL_CLK_SYNTH
#undef LOCAL
#define LOCAL
#include <private/kwriteLibP.h>     /* _func_kprintf */

#define PLL_CLK_SYNTH_DBG_ERR         0x00000001
#define PLL_CLK_SYNTH_DBG_INFO        0x00000002
#define PLL_CLK_SYNTH_DBG_ALL         0xffffffff
#define PLL_CLK_SYNTH_DBG_OFF         0x00000000

LOCAL UINT32 pllClkSynthDbgMask = PLL_CLK_SYNTH_DBG_ALL;

#undef PLL_CLK_SYNTH_MSG
#define PLL_CLK_SYNTH_MSG(mask, ...)          	                    \
    do   	        	                                        \
	    {       	                                            \
	    if ((pllClkSynthDbgMask & (mask)) || ((mask) == PLL_CLK_SYNTH_DBG_ALL))   \
            {							     					\
		    if (_func_kprintf != NULL)                     	    \
		        {                                          	    \
         	    (* _func_kprintf)(__VA_ARGS__);        	        \
	            }                                          	    \
            }                                               	\
	    }														\
    while ((FALSE))
#else
#undef PLL_CLK_SYNTH_MSG
#define PLL_CLK_SYNTH_MSG(...)
#endif  /* PLL_CLK_SYNTH_MSG */
#if 1
#define CMD_LENGTH	1
#define DEFAULT_WRTIME  2
#define PLL_CLK_SYNTH_NAME            "cdce913"
#endif


#define CLK_SYNTH_BYTE_MODE 0x80
#define CDC_MAX_FREQ	100000000

#undef DEBUG_I2C_PLL_CLK_SYNTH
#ifdef DEBUG_I2C_PLL_CLK_SYNTH
#undef LOCAL
#define LOCAL
#include <private/kwriteLibP.h>     /* _func_kprintf */

#define PLL_CLK_SYNTH_DBG_ERR         0x00000001
#define PLL_CLK_SYNTH_DBG_INFO        0x00000002
#define PLL_CLK_SYNTH_DBG_ALL         0xffffffff
#define PLL_CLK_SYNTH_DBG_OFF         0x00000000

LOCAL UINT32 pllClkSynthDbgMask = PLL_CLK_SYNTH_DBG_ALL;

#undef PLL_CLK_SYNTH_MSG
#define PLL_CLK_SYNTH_MSG(mask, ...)          	                    \
    do   	        	                                        \
	    {       	                                            \
	    if ((pllClkSynthDbgMask & (mask)) || ((mask) == PLL_CLK_SYNTH_DBG_ALL))   \
            {							     					\
		    if (_func_kprintf != NULL)                     	    \
		        {                                          	    \
         	    (* _func_kprintf)(__VA_ARGS__);        	        \
	            }                                          	    \
            }                                               	\
	    }														\
    while ((FALSE))
#else
#undef PLL_CLK_SYNTH_MSG
#define PLL_CLK_SYNTH_MSG(...)
#endif  /* PLL_CLK_SYNTH_MSG */
#if 1
#define CMD_LENGTH	1
#define DEFAULT_WRTIME  2
#define PLL_CLK_SYNTH_NAME            "cdce913"
#endif



typedef struct vxbCdce913Functionality
    {
    STATUS (*cdce913GetTbClk)(VXB_DEV_ID pInst,
    	    UINT8 *             buf,
    	    UINT8               offset,
    	    UINT8               length);
    STATUS (*cdce913SetTbClk)(VXB_DEV_ID pInst, 
    	    UINT8 *             buf,
    	    UINT8               offset,
    	    UINT8               length);

	} VXB_CDCE913_FUNC;

	
struct vxbCdce913Handle
    {
    struct vxbCdce913Functionality *pCdce913Func;
    VXB_DEV_ID                  pCdce913Inst;
    };


typedef struct i2cPLLClkSynthCtrl
    {
    VXB_DEV_ID  pDev;
    UINT16      i2cAddr;
    }I2C_PLL_CLK_SYNTH_CTRL;


/* IMPORT */

IMPORT INT32 vxbCdce913GetTbClk (
	    UINT8               offset,
	    UINT8               length);
IMPORT STATUS vxbCdce913SetTbClk ( 
		UINT8              buf,
	    UINT8               offset,
	    UINT8               length);

IMPORT STATUS vxbCdce913Freq
    (
    		UINT8				freq /* in MhZ */
    );
IMPORT void cdce913Register
    (
    VXB_DEV_ID pDev,
	VXB_CDCE913_FUNC * cdce913
    );




#ifdef __cplusplus
}
#endif

#endif 
