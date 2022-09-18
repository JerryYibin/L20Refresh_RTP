/* vxbMdioLib.h - VxBus Mdio interfaces header file */

/*
DESCRIPTION

This library provides the Mdio specific interfaces

*/

#ifndef __INCvxbMdioLibh
#define __INCvxbMdioLibh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/vxBus.h>
#include <hwif/util/vxbIsrDeferLib.h>


/* structure to be used and updated RTC time and Alarm */

typedef struct vxbMdioFunctionality
    {
    STATUS (*mdioDisable)(VXB_DEV_ID pInst);
    STATUS (*mdioEnable)(VXB_DEV_ID pInst);
    STATUS (*mdioPrintReg)(VXB_DEV_ID pInst);
    } VXB_MDIO_FUNC;

struct vxbMdioHandle
    {
	struct vxbMdioFunctionality 	*pMdioFunc;
    VXB_DEV_ID       pInst;
    };

/* IMPORT */

IMPORT STATUS Mdio_Disable_Logs (void);
IMPORT STATUS Mdio_Enable_Log (void);
IMPORT STATUS Mdio_Print_Reg (void);


IMPORT void mdio_Register
    (
    VXB_DEV_ID pDev,
    VXB_MDIO_FUNC * mdio
    );
    
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbRtcLibh */


