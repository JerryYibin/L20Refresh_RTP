/* vxbMcSpiLib.h - VxBus Mc Spi interfaces header file */


/*
DESCRIPTION

This library provides the McSPI specific interfaces

*/

#ifndef __INCvxbMcSpiLibh
#define __INCvxbMcSpiLibh 

#ifdef __cplusplus
extern "C" {
#endif

#include <hwif/buslib/vxbSpiLib.h>

typedef struct vxbMcSpiFunctionality
    {
    STATUS (*mcspiTrans)(VXB_DEV_ID, SPI_HARDWARE *,SPI_TRANSFER *);   
    } VXB_MC_SPI_FUNC;

extern UINT32 mcspiDevNumGet();
extern STATUS vxbMcSpiTrans(UINT32, SPI_HARDWARE *, SPI_TRANSFER *);
    
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbMcSpiLibh */

