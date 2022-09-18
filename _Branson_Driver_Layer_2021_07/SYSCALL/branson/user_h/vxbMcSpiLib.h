/* vxbMcSpiLib.h - VxBus Mc Spi interfaces header file */
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
/*
DESCRIPTION
This library provides the McSPI specific interfaces
*/

#ifndef __INCvxbMcSpiLibh
#define __INCvxbMcSpiLibh 

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_CKPHA           0x01  /* clock phase */
#define SPI_CKPOL           0x02  /* clock polarity */
#define SPI_MODE_0          (0 | 0)
#define SPI_MODE_1          (0 | SPI_CKPHA)
#define SPI_MODE_2          (SPI_CKPOL | 0)
#define SPI_MODE_3          (SPI_CKPOL | SPI_CKPHA)
#define SPI_CSPOL_HIGH      0x04  /* CS active high: active-low; normal */
#define SPI_LSB_FIRST       0x08
#define SPI_LOOP            0x10
#define SPI_FULL_DUPLEX     0x20
#define SPI_POLLING         0x40  /* SPI controller works at polling mode */
#define SPI_PARALLEL        0x80  /* two or more SPI devices are connected in parallel */
#define SPI_READ_11X        0x100 /* address for readm command is single bit transfer */
#define SPI_DATA_STRIPE     0x200 /* data stripe for dual parallel flash data transfer */

enum SPI_SLAVE
{
	SPI_SLAVE_0 = 0,
	SPI_SLAVE_1 = 1,
	SPI_SLAVE_2 = 2,
	SPI_SLAVE_3 = 3
};
enum SPI_DEVICE
{
	SPI_PORT_1 = 1,
	SPI_PORT_2 = 2,
	SPI_PORT_3 = 3,
	SPI_PORT_4 = 4
};
enum SPI_CLOCK
{
	SPI_CLK_MODE0 = SPI_MODE_0,     // POL = 0, PHA = 0
	SPI_CLK_MODE1 = SPI_MODE_1,     // POL = 0, PHA = 1
	SPI_CLK_MODE2 = SPI_MODE_2,     // POL = 1, PHA = 0
	SPI_CLK_MODE3 = SPI_MODE_3      // POL = 1, PHA = 1
};

enum BIT_WIDTH
{
	NUM_BITS_PERTRANSFER_8 	= 8,
	NUM_BITS_PERTRANSFER_16 = 16,
	NUM_BITS_PERTRANSFER_32 = 32,
};

typedef struct vxbSpiDevInfo
    {
    char  *deviceName;
    UINT8  chipSelect;
    UINT8  bitWidth;
    UINT32 devFreq;
    UINT32 mode;
    UINT32 chipNum;
    UINT32 dataLines;
    }SPI_HARDWARE;

typedef struct spiTransfer 
    {
    UINT8 *txBuf;
    UINT8 *rxBuf;
    UINT32 txLen;
    UINT32 rxLen;
    UINT32 rxNbits;
    UINT32 addrLen;
    UINT32 dummy;
    UINT32 usDelay;
    UINT32 base;
    }SPI_TRANSFER;
extern UINT32 mcspiDevNumGet();
extern STATUS vxbMcSpiTrans(UINT32, SPI_HARDWARE *, SPI_TRANSFER *);
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbMcSpiLibh */


