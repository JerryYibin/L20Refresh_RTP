/*
 * vxbSp25QspiFlash.h
 *
 *  Created on: Mar 22, 2018
 *      Author: emerson
 */

/** ============================================================================
 *  @file       QSPI_v1.h
 *
 *  @brief      QSPI driver implementation for a AM572x QSPIcontroller
 *
 *  The SPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drv/spi/SPI.h>
 *  #include <ti/drv/spi/soc/QSPI_v1.h>
 *  @endcode
 *
 *  This QSPI driver implementation is designed to operate on a AM572x QSPI
 *
 *
 *  ============================================================================
 */

#ifndef QSPI_V1_H
#define QSPI_V1_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vxworks.h>
#include <stdint.h>
#include <vxbQspiInterface.h>
#include <qspi.h>

//#include <ti/osal/osal.h>
/*
 *  \brief  Set SPI controller into CONFIG Mode
 *
 *  Command code used with SPI_control()
 */
#define SPI_V1_CMD_SETCONFIGMODE        (SPI_CMD_RESERVED + 0U)

/*
 *  \brief  Set SPI controller into MEMORY MAP Mode
 *
 *  Command code used with SPI_control()
 */
#define SPI_V1_CMD_SETMEMMORYMAPMODE    (SPI_CMD_RESERVED + 1U)

/*
 *  \brief  Set "FRAMELENTH" when in memory mapped mode.
 *
 *  Conventional SPI_transfer() calls no not require the frame length to be set.
 *  This function is only used in combination with the series of specific SPI
 *  payload (e.g.flash control cmd + internal flash addressing + flash data)
 *
 *  Command code used with SPI_control(). arg is a pointer to an uint32_t
 *  that contains the frame length value to be placed into the SPI register.
 */
#define SPI_V1_CMD_SETFRAMELENGTH       (SPI_CMD_RESERVED + 2U)

/*
 *  \brief  Set direction of data flow when in memory mapped mode.
 *
 *  Conventional SPI_transfer() calls no not require a read or write direction
 *  as SPI operates a in full duplex mode. This function is only used in
 *  combination with memory mapped mode where a read or write direction is
 *  required with SPI flash memory.
 */
#define SPI_V1_CMD_TRANSFERMODE_RW          (SPI_CMD_RESERVED + 3U)

/*
 *  \brief  Set flash specific command when in memory mapped mode.
 *
 */
#define SPI_V1_CMD_MMAP_TRANSFER_CMD        (SPI_CMD_RESERVED + 4U)

/*
 *  \brief  Set number of lines configured for receive operation.
 *
 */
#define SPI_V1_CMD_SETRXLINES               (SPI_CMD_RESERVED + 5U)

/* SPI function table pointer */
extern const SPI_FxnTable QSPI_FxnTable_v1;

/*!
 *  @brief      Type of the SPI transaction.
 */
typedef enum SPI_TransactionType_s {
	SPI_TRANSACTION_TYPE_READ = 0, /*!< Read transaction */
	SPI_TRANSACTION_TYPE_WRITE /*!< Write transaction */
}SPI_TransactionType;

/*!
 *  @brief  QSPI Am572x controller modes.
 */
typedef enum QSPI_OperMode_s {
	QSPI_OPER_MODE_CFG = 0U, /*! QSPI controller is in config mode */
	QSPI_OPER_MODE_MMAP /*! QSPI controller is in memory map mode */
}QSPI_OperMode;

/*!
 *  @brief  Number of lines used for QSPI read transaction
 */
typedef enum QSPI_rxLines_s {
	QSPI_RX_LINES_SINGLE = 0U, /*! Single line is used */
	QSPI_RX_LINES_DUAL, /*! Two lines are used */
	QSPI_RX_LINES_QUAD /*! Four lines are used */
}QSPI_rxLines;

/*!
 *  @brief  QSPI Am572x Hardware attributes
 */
typedef struct QSPI_HwAttrs_s {
	/*! QSPI Am572x Peripheral base address. */
	uint32_t baseAddr;
	/*! QSPI Am572x memory mapped mode base address */
	uint32_t memMappedBaseAddr;
	/*! QSPI Am572x functional clock */
	uint32_t funcClk;
	/*! QSPI Am572x interrupt number */
	uint32_t intrNum;
	/*! QSPI Am572x interrupt id */
	uint32_t eventId;
	/*! QSPI controller mode */
	QSPI_OperMode operMode;
	/*! QSPI chip select number */
	uint32_t chipSelect;
	/*! QSPI CS polarity */
	qspiCsPol_t csPol;
	/*! QSPI frame format */
	qspiClkMode_t frmFmt;
	/*! QSPI data delay */
	qspiDataDelay_t dataDelay;
	/*! Number of rx Lines used for QSPI reading . */
	QSPI_rxLines rxLines;
	/*! Enable interrupts. */
	BOOL intrEnable;
}QSPI_HwAttrs;

/*!
 *  @brief  QSPI Am572x driver object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct QSPI_v1_Object_s {
	void * mutex; /* instance lock */
	void * transferComplete; /* Transfer complete lock */
	void * hwi; /* Hardware interrupt object */
	uint32_t waitTimeout; /* Wait time out */

	SPI_Params qspiParams; /* input parameters */
	uint32_t txCmd; /* config mode command */

	uint32_t frmLength; /* Frame length of total transfer */
	uint32_t transactionType; /* Read or Write Transaction */
	uint8_t transferCmd; /* Transfer command */
	uint32_t qspiMode; /* Config or Memory map mode */
	uint32_t rxLines; /* Number of receive Lines */
	uint32_t intrPollMode; /* Interrupt or polling mode */

	SPI_TransferMode transferMode; /* Transfer mode */
	SPI_Transaction transaction; /* Transaction structure */

	uint8_t *writeBufIdx; /* Internal inc. writeBuf index */
	uint32_t writeCountIdx; /* Internal dec. writeCounter */
	uint8_t *readBufIdx; /* Internal inc. readBuf index */
	uint32_t readCountIdx; /* Internal dec. readCounter */

	BOOL isOpen; /* flag to indicate module is open */
}QSPI_v1_Object;

void qspiRegister
(
		VXB_DEV_ID pDev,
		SPI_FxnTable * pQspiFunc
);

/*!
 *  @brief Flash device size in bytes.
 */
#define S25FL_FLASH_DEVICE_SIZE        (64 * 1024 *1024)

/*!
 *  @brief Flash block size in bytes.
 */
#define S25FL_FLASH_BLOCK_SIZE         (64 * 1024)

/*!
 *  @brief Flash sector size in bytes.
 */
#define S25FL_FLASH_SECTOR_SIZE        (4 * 1024)

/*!
 *  @brief Flash device ID.
 */
#define S25FL_FLASH_DEVICE_ID          (0x1a)

/*!
 *  @brief Macro to enable quad mode.
 */
#define QSPI_FLASH_QUAD_ENABLE_VALUE (0x1)

/*!
 *  @brief Macro indicating the bit position of quad enable bit.
 */
#define QSPI_FLASH_QUAD_ENABLE_BIT   (0x01U)

/**
 * \brief  This enumerates the commands of qspi flash
 *         for communication with the flash device.
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
	QSPI_LIB_CMD_EXIT_4_BYTE_ADDR = 0xE9U,
	/**< Command to exit 4 byte addressing mode */
	QSPI_LIB_CMD_QUAD_RD_CMD_REG = 0x35U,
	/**< command to read the register where the quad enable bit has to be set */
	QSPI_LIB_CMD_QUAD_WR_CMD_REG = 0x01U,
	/**< command to read the register where the quad enable bit has to be set */
	QSPI_LIB_CMD_READ_SINGLE_4B = 0x13U,
	/**< Single read from qspi flash device in 4 byte addressing mode */
	QSPI_LIB_CMD_READ_DUAL_4B = 0x3CU,
	/**< Dual Read from QSPI flash device in 4 byte addressing mode */
	QSPI_LIB_CMD_READ_QUAD_4B = 0x6C,
	/**< Quad read commnad from QSPI flash in 4 byte addressing mode */
	QSPI_LIB_CMD_PAGE_PRG_4B = 0x12U,
	/**< Command to write to serial nor flash in 4 byte addressing mode */
	QSPI_LIB_CMD_BLOCK_ERASE_4B = 0xDCU
	/**< QSPI flash block erase command */
}qspiLibCmd_t;

/*!
 *  @brief Flash object containing flash attributes.
 */
typedef struct S25FL_Object {
	SPI_Handle spiHandle; /* Handle for SPI object */
	unsigned int boardSPI; /* Board SPI in Board.h */
	unsigned int sectorSize; /* Indicates sector size of the flash */
	unsigned int blockSize; /* block size of the flash in bytes */
	unsigned int deviceSize; /* Size of the flash device */
	unsigned int flashDeviceId; /* Flash device id */
}S25FL_Object;

/*!
 *  @brief Flash transaction data structure.
 */
typedef struct S25FL_Transaction {
	uint8_t *data; /* Transaction data buffer  */
	uint32_t address; /* Flash address object for transaction */
	uint32_t dataSize; /* size of transaction data in bytes */
}S25FL_Transaction;

/*!
 *  @brief A handle that is returned from a S25FL_open() call.
 */
typedef S25FL_Object *S25FL_Handle;


extern S25FL_Handle vxbQspiLibSF25FLOpen();


extern void vxbQspiLibSF25FLClose(S25FL_Handle flashHandle);


extern BOOL vxbQspiLibSF25FLBufferWrite(S25FL_Handle flashHandle,
		S25FL_Transaction* flashTransaction);


extern BOOL vxbQspiLibSF25FLBufferRead(S25FL_Handle flashHandle,
		S25FL_Transaction* flashTransaction);


extern uint32_t vxbQspiLibS25FLFlashStatus(S25FL_Handle flashHandle);


extern BOOL vxbQspiLibS25FLFlashEnable4ByteAddrMode(S25FL_Handle flashHandle,
		BOOL enable4ByteMode);


extern BOOL vxbQspiLibS25FLFlashWriteEnable(S25FL_Handle flashHandle);


extern BOOL vxbQspiLibS25FLFlashQuadModeEnable(S25FL_Handle flashHandle);


extern BOOL vxbQspiLibS25FLFlashBlockErase(S25FL_Handle flashHandle,
		unsigned int blockNumber);


extern void vxbQspiLibS25FLFlashPrintId(S25FL_Handle flashHandle);

#ifdef __cplusplus
}
#endif

#endif /* SOURCES_PKGS_STORAGE_BDM_1_0_1_3_FLASH_MTD_1_1_1_0_H_SPIFLASH_VXBSP25QSPIFLASH_H_ */
