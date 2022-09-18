/**
 * qspi_lib.h
 *
 * This file contains the function prototypes, structures
 * and enums for QSPI flash required to communicate with
 * QSPI flash device.
 */


#ifndef QSPI_LIB_H_
#define QSPI_LIB_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <qspi_flash.h>
#include <vxbTiQspi.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This enumerates the commands of qspi flash
 * for communication with the flash device.
 */
typedef enum qspiLibCmd
{
    QSPI_LIB_CMD_READ_SINGLE = 0x3U,
    /**< Single read from qspi flash device */
    QSPI_LIB_CMD_READ_DUAL = 0x3BU,
    /**< Dual Read from QSPI flash device */
    QSPI_LIB_CMD_READ_QUAD = 0x6B,
    /**< Quad read commnad from QSPI flash */
    QSPI_LIB_CMD_WRITE_ENABLE = 0x6U,
    /**< QSPI flash write enable command */
    QSPI_LIB_CMD_READ_STATUS_REG = 0x5U,
    /**< Read status register */
    QSPI_LIB_CMD_READ_STATUS_REG2 = 0x7U,
    /**< Read status register 2 */
    QSPI_LIB_CMD_READ_CONF_REG = 0x35U,
    /**< Read configuration register */
    QSPI_LIB_CMD_WRITE_STATUS_REG = 0x1U,
    /**< Write status register */
    QSPI_LIB_CMD_READ_SFDP = 0x5AU,
    /**< Command to read SFDP header and parameter table */
    QSPI_LIB_CMD_PAGE_PRG = 0x2U,
    /**< Command to write to serial nor flash */
    QSPI_LIB_CMD_FAST_READ = 0x0BU,
    /**< Fast single read command */
    QSPI_LIB_CMD_READ_MFG_ID = 0x90U,
    /**< Read manufacturer and device ID */
    QSPI_LIB_CMD_BLOCK_ERASE = 0xD8U,
    /**< QSPI flash block erase command */
    QSPI_LIB_CMD_FULL_ERASE = 0x60U,
    /**< Completely erase flash data */
    QSPI_LIB_CMD_ENTER_4_BYTE_ADDR = 0xB7U,
    /**< Command to enter 4 byte addressing mode */
    QSPI_LIB_CMD_EXIT_4_BYTE_ADDR = 0xE9U
    /**< Command to exit 4 byte addressing mode */
}qspiLibCmd_t;

/**
 * This enumerates the data transfer mode for qspi flash device i.e. config mode
 * using configuration registers or memory mapped mode
 */
typedef enum qspiLibTxMode
{
    QSPI_LIB_TX_MODE_CONFIG_MODE = 0x0U,
    /**< QSPI data transfer in config mode */
    QSPI_LIB_TX_MODE_MEMORY_MAPPED = 0x1U
    /**< QSPI data transfer in memory mapped mode */
}qspiLibTxMode_t;


/**
 * This enumnerates the read type for a qspi flash device 
 * i.e. single, dual or quad read.
 */
typedef enum qspiLibReadType
{
    QSPI_LIB_READ_TYPE_SINGLE = 0x0U,
    /**< Single read mode*/
    QSPI_LIB_READ_TYPE_DUAL = 0x1U,
    /**< Dual read mode*/
    QSPI_LIB_READ_TYPE_QUAD = 0x2U,
    /**< Quad read mode*/
}qspiLibReadType_t;

/**
 *  This structure defined the QSPI controller information.
 *  This information has to be entered by the user from
 *  the application.
 */
typedef struct qspiLibCtrlInfo
{
	VXB_DEV_ID pDev;
	/**< Driver ID */
    uint32_t chipSelect;
    /**< Chip select on which the device is interfaced */
    uint32_t memMapBaseAddr;
    /**< QSPI memory mapped base address */
    uint32_t cfgBaseAddr;
    /**< QSPI configuration registers base address */
    qspiLibReadType_t qspiLibReadType;
    /**< Read type for read from flash device i.e. single, dual or quad */
    qspiLibTxMode_t qspiLibTxMode;
    /**< Transfer mode i.e. memory mapped mode or config mode */
} qspiLibCtrlInfo_t;

/**
 * This structure defines the QSPI flash related information.
 */
typedef struct qspiLibFlashInfo
{
    uint32_t deviceSize;
    /**< Size of the flash device */
    uint32_t flashDevId;
    /**< Device ID of the QSPI flash device */
    uint32_t flashMfgId;
    /**< Manufacturer ID of the flash device */
    uint32_t sectorSize;
    /**< Size of the each sector of flash */
    uint32_t blockSize;
    /**< Size of each block of flash */
}qspiLibFlashInfo_t;

/**
 * This structure defines the QSPI controller and flash device info.
 */
typedef struct qspiLibInfo
{
    qspiLibCtrlInfo_t qspiLibCtrlInfo;
    /**< This contains details about QSPI controller */
    qspiLibFlashInfo_t qspiLibFlashInfo;
    /**< this contains details about QSPI flash device */
}qspiLibInfo_t;

/* Structure holding the QSPI instance related data for nor flash */
typedef struct qspiObj
{
    uint32_t chipSelect;
    /**< QSPI chip select number */
    uint32_t instNum;
    /**< QSPI instance number */
    uint32_t instAddr;
    /**< QSPI instance address */
    uint32_t devId;
    /**< QSPI device ID*/
    uint32_t length;
    /**< Length of data to be transferred */
    qspiLibInfo_t qspiLibInfo;
    /**< configurable parameters for the qspi flash device and controller */
}qspiObj_t;

void QSPIRegister   (	TI_AM437X_SPI_CTRL *    pDrvCtrl  );

void QSPILibInit(void);

void QSPILibBlockErase(uint32_t blockNumber);

void QSPILibChipErase(void);

void QSPILibPrintId(void);

int32_t QSPILibRead(
                 uint32_t srcOffsetAddr,
                 uint32_t dstAddr,
                 uint32_t length);

int32_t QSPILibWrite(
                  uint32_t dstOffsetAddr,
                  uint32_t srcAddr,
                  uint32_t length);

void QSPILibQuadEnable(qspiFlashDeviceData_t *qspiFlashDeviceData);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef QSPI_LIB_H */
