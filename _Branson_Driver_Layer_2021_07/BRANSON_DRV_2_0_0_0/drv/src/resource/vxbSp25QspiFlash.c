/*
 * vxbSp25QspiFlash.c
 *
 *  Created on: Mar 22, 2018
 *      Author: emerson
 */

#include <vxworks.h>
#include <vxbSp25QspiFlash.h>
#include <vxbQspiInterface.h>
//#include <ti/drv/spi/soc/QSPI_v1.h>
#define CSL_MPU_QSPI_ADDRSP0_REGS                                               (0x4b300000U)
#define CSL_MPU_QSPI_ADDRSP0_SIZE                                               (0x80U)
#define CSL_MPU_QSPI_ADDRSP1_REGS                                               (0x5c000000U)
#define CSL_MPU_QSPI_ADDRSP1_SIZE                                               (0x4000000U)
/* SPI configuration structure */
QSPI_HwAttrs qspiInitCfg[1] = { {
		CSL_MPU_QSPI_ADDRSP0_REGS, /* register baseAddr */
		CSL_MPU_QSPI_ADDRSP1_REGS, /* memMappedBaseAddr */
		48000000, /*Input frequency */
#ifdef _TMS320C6X
		15U,
#elif defined(__ARM_ARCH_7A__)
		67U, /* 35 + 32 */
#else
		62U,
#endif
#ifdef _TMS320C6X
		38U, /*343U,*/
#elif defined(__ARM_ARCH_7A__)
		35U, /*343U,*/
#else
		0U, /*343U,*/
#endif
		QSPI_OPER_MODE_MMAP, /* operMode */
		0U, /* chipSelect */
		QSPI_CS_POL_ACTIVE_LOW, /* csPol */
		QSPI_CLK_MODE_3, /* frmFmt */
		QSPI_DATA_DELAY_0, /* dataDelay */
		QSPI_RX_LINES_QUAD, /* rxLines */
		TRUE, } };

const SPI_Params SPI_defaultParams = { SPI_MODE_BLOCKING, /* transferMode */
		0U, /* transferTimeout */
		NULL, /* transferCallbackFxn */
		SPI_MASTER, /* mode */
		1000000, /* bitRate */
		8, /* dataSize */
		SPI_POL0_PHA0, /* frameFormat */
		NULL /* custom */
};

/*!
 *  @brief Flash object containing flash attributes.
 */
S25FL_Object s25flObject = { NULL, 4,
		S25FL_FLASH_SECTOR_SIZE,
		S25FL_FLASH_BLOCK_SIZE,
		S25FL_FLASH_DEVICE_SIZE,
		S25FL_FLASH_DEVICE_ID };

static BOOL SF25FL_ConfigMode_Write(uint32_t dstOffstAddr,
		unsigned char* srcAddr, uint32_t length, S25FL_Handle flashHandle);

SPI_config_list SPI_config;

/*******************************************************************************
 *
 * qspiRegister : This API stores the QSPI Controller Driver instance &
 *        Instantiate Flash Library struct as well .
 *
 */
void qspiRegister(VXB_DEV_ID pDev, SPI_FxnTable * pQspiFunc) {
	SPI_config[0].pInst = pDev;
	SPI_config[0].fxnTablePtr = pQspiFunc;
}

/*
 *  This function returns the Handle for the Corresponding QSPI object.
 */
SPI_Handle SPI_open(uint32_t index, SPI_Params *params) {
	SPI_Handle handle;

	/* Get handle for this driver instance */
	handle = (SPI_Handle) &(SPI_config[0]);

	handle->hwAttrs = qspiInitCfg;
	handle = handle->fxnTablePtr->openFxn(SPI_config[0].pInst, handle,
			&SPI_defaultParams);

	return (handle);
}

/*!
 *  @brief  Opens the S25FL object. This will internally opens the corresponding
 *          QSPI instance and gets the QSPI instance handle.
 *
 *  @param  index    Flash index.
 *  @param  params   Pointer to the SPI parameters structure.
 */
S25FL_Handle vxbQspiLibSF25FLOpen() {
	S25FL_Handle s25flHandle;

	s25flHandle = &s25flObject;

	/* Open SPI driver for SF25FL */
	s25flHandle->spiHandle = SPI_open(0, NULL);

	return s25flHandle;
}

/*
 *  This function in turn call the Transfer function of SPI.  
 */
BOOL SPI_transfer(SPI_Handle handle, SPI_Transaction *transaction) {

	return (handle->fxnTablePtr->transferFxn(handle, transaction));
}

/*
 * This function cancels the transfer to SPI.
 */
void SPI_transferCancel(SPI_Handle handle) {
	handle->fxnTablePtr->transferCancelFxn(handle);
}

/*
 *  This function closes the Handle for the Corresponding QSPI object.
 */
void SPI_close(SPI_Handle handle) {

	handle->fxnTablePtr->closeFxn(handle);

}

/*
 *  This function in turn calls the control function of the SPI.
 */
int32_t SPI_control(SPI_Handle handle, uint32_t cmd, void *arg) {

	return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*!
 *  @brief  Closes the S25FL object.
 *
 *  @param  flashHandle   A S25FL_Handle
 */
void vxbQspiLibSF25FLClose(S25FL_Handle handle) {
	SPI_close(handle->spiHandle);
}

/*!
 *  @brief  This function writes data to the flash using QSPI interface.
 *
 *  @param  flashHandle       A S25FL_Handle
 *  @param  transaction       A S25FL_Transaction
 *  @return Status            true if successful; else false
 *
 */
BOOL vxbQspiLibSF25FLBufferWrite(S25FL_Handle flashHandle,
		S25FL_Transaction* flashTransaction) {
	BOOL retVal = FALSE; /* return value */
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	SPI_Transaction transaction; /* SPI transaction structure */
	unsigned int idx; /* index */
	uint32_t dstOffstAddr; /* Flash offset address */
	unsigned char *srcAddr; /* Address of data buffer */
	uint32_t length; /* data length in bytes */
	unsigned int transferType;
	unsigned char transferCmd;
	QSPI_v1_Object *object;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* Copy flash transaction parameters to local variables */
	dstOffstAddr = flashTransaction->address;
	srcAddr = flashTransaction->data;
	length = flashTransaction->dataSize;

	if (QSPI_OPER_MODE_CFG == object->qspiMode) {
		/*
		 * Config mode write supports 256 bytes at a time. So if the length is
		 * greater than 256, then multiple transactions have to be performed till
		 * all the bytes are written.
		 */
		while (length > 256) {
			SF25FL_ConfigMode_Write(dstOffstAddr, srcAddr, 256, flashHandle);
			dstOffstAddr = dstOffstAddr + (256 / 4);
			srcAddr += 256;
			length -= 256;
		}
		if (length > 0) {
			SF25FL_ConfigMode_Write(dstOffstAddr, srcAddr, length, flashHandle);
		}
	}

	if (QSPI_OPER_MODE_MMAP == object->qspiMode) {
		for (idx = 0; idx < length; idx++) {
			/* Write enable */
			vxbQspiLibS25FLFlashWriteEnable(flashHandle);

			/* Perform the transfer */
			transaction.txBuf = (unsigned char *) dstOffstAddr;
			transaction.rxBuf = srcAddr;
			transaction.count = 1;

			transferType = SPI_TRANSACTION_TYPE_WRITE;

			if (dstOffstAddr > 0xFFFFFFU)
			{
				transferCmd  = QSPI_LIB_CMD_PAGE_PRG_4B;
			}
			else
			{
				transferCmd  = QSPI_LIB_CMD_PAGE_PRG;
			}
			
			SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW,
					(void *) &transferType);
			SPI_control(handle, SPI_V1_CMD_MMAP_TRANSFER_CMD,
					(void *) &transferCmd);
			retVal = SPI_transfer(handle, &transaction);

			/* Check flash status for completion */
			while ((vxbQspiLibS25FLFlashStatus(flashHandle) & 0x1U))
				;

			dstOffstAddr += 1;
			srcAddr += 1;
		}
	}

	return retVal;
}

/*!
 *  @brief  This function reads specific number of bytes from the flash using
 *          QSPI interface.
 *
 *  @param  flashHandle       A S25FL_Handle
 *  @param  transaction       A S25FL_Transaction
 *  @return Status            true if successful; else false
 *
 */
BOOL vxbQspiLibSF25FLBufferRead(S25FL_Handle flashHandle,
		S25FL_Transaction* flashTransaction) {
	BOOL retVal = FALSE; /* return value */
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	unsigned char writeVal[4]; /* data to be written */
	SPI_Transaction transaction; /* SPI transaction structure */
	uint32_t addrLengthInBytes = 3U; /* Flash address length in bytes */
	uint32_t readCmd = QSPI_LIB_CMD_READ_SINGLE; /* Read command */
	uint32_t numDummyBits = 0U; /* Number of dummy bits */
	uint32_t frameLength = 0U; /* Frame length */
	uint32_t dstOffstAddr; /* Flash offset address */
	unsigned char *srcAddr; /* Address of data buffer */
	uint32_t length; /* data length in bytes */
	unsigned int *tempPtr = (unsigned int *) &writeVal[0]; /* temp pointer */
	unsigned int frmLength;
	unsigned int transferType;
	unsigned char transferCmd;
	QSPI_v1_Object *object;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* Copy flash transaction parameters to local variables */
	dstOffstAddr = (uint32_t) flashTransaction->data;
	srcAddr = (unsigned char *) flashTransaction->address;
	length = flashTransaction->dataSize;

	if (QSPI_OPER_MODE_CFG == object->qspiMode) {
		addrLengthInBytes = 3U;

		switch (object->rxLines) {
		case QSPI_RX_LINES_SINGLE: {
			
			if (flashTransaction->address > 0xFFFFFF) 
			{
				readCmd = QSPI_LIB_CMD_READ_SINGLE_4B;
			} 
			else 
			{
				readCmd = QSPI_LIB_CMD_READ_SINGLE;
			}
			
			numDummyBits = 0U;

			/* total transaction frame length in number of words (bytes)*/
			frameLength = length + 1 + addrLengthInBytes;
			break;
		}

		case QSPI_RX_LINES_DUAL: {
			
			if (flashTransaction->address > 0xFFFFFF) 
			{
				readCmd = QSPI_LIB_CMD_READ_DUAL_4B;
			} 
			else 
			{
				readCmd = QSPI_LIB_CMD_READ_DUAL;
			}
			
			numDummyBits = 8U;

			/* total transaction frame length in number of words (bytes)*/
			frameLength = length + 1 + 1 + addrLengthInBytes;
			break;
		}

		case QSPI_RX_LINES_QUAD: {
			
			if (flashTransaction->address > 0xFFFFFF) 
			{
				readCmd = QSPI_LIB_CMD_READ_QUAD_4B;
			}
			else 
			{
				readCmd = QSPI_LIB_CMD_READ_QUAD;
			}
			
			numDummyBits = 8U;

			/* total transaction frame length in number of words (bytes)*/
			frameLength = length + 1 + 1 + addrLengthInBytes;
			break;
		}

		default:
			break;
		}

		/* total transaction frame length */
		frmLength = frameLength;
		SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *) &frmLength);

		/* Write read command */
		writeVal[0] = (UINT8)readCmd;
		transaction.txBuf = (unsigned char *) &writeVal[0];
		transaction.rxBuf = NULL;
		transaction.count = 1;

		transferType = SPI_TRANSACTION_TYPE_WRITE;
		SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
		retVal = SPI_transfer(handle, &transaction);

		/* Write dummy bits for fast read if required */
		if (0 != numDummyBits) {
			writeVal[0] = 0U;
			transaction.txBuf = (unsigned char *) &writeVal[0];
			transaction.rxBuf = NULL;
			transaction.count = (numDummyBits >> 3); /* In bytes */

			transferType = SPI_TRANSACTION_TYPE_WRITE;
			SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW,
					(void *) &transferType);
			retVal = SPI_transfer(handle, &transaction);
		}

		/* Write Address Bytes */
		*tempPtr = (unsigned int) srcAddr;
		transaction.txBuf = (unsigned char *) tempPtr;
		transaction.rxBuf = NULL;
		transaction.count = addrLengthInBytes;

		transferType = SPI_TRANSACTION_TYPE_WRITE;
		SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
		retVal = SPI_transfer(handle, &transaction);

		/* Read the actual flash data */
		transaction.txBuf = NULL;
		transaction.rxBuf = (unsigned char *) dstOffstAddr;
		transaction.count = length;

		transferType = SPI_TRANSACTION_TYPE_READ;
		SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
		retVal = SPI_transfer(handle, &transaction);
	}

	if (QSPI_OPER_MODE_MMAP == object->qspiMode) {

		switch (object->rxLines) {
		case QSPI_RX_LINES_SINGLE: {
			
			if (flashTransaction->address > 0xFFFFFF) 
			{
				readCmd = QSPI_LIB_CMD_READ_SINGLE_4B;
			} 
			else 
			{
				readCmd = QSPI_LIB_CMD_READ_SINGLE;
			}
			
			break;
		}

		case QSPI_RX_LINES_DUAL: {
			
			if (flashTransaction->address > 0xFFFFFF) 
			{
				readCmd = QSPI_LIB_CMD_READ_DUAL_4B;
			} 
			else 
			{
				readCmd = QSPI_LIB_CMD_READ_DUAL;
			}
			
			break;
		}

		case QSPI_RX_LINES_QUAD: {
			
			if (flashTransaction->address > 0xFFFFFF) 
			{
				readCmd = QSPI_LIB_CMD_READ_QUAD_4B;
			} 
			else 
			{
				readCmd = QSPI_LIB_CMD_READ_QUAD;
			}
			
			break;
		}

		default:
			break;
		}

		/* Perform the transfer */
		transaction.txBuf = srcAddr;
		transaction.rxBuf = (uint8_t *) dstOffstAddr;
		transaction.count = length;

		transferType = SPI_TRANSACTION_TYPE_READ;
		transferCmd = (unsigned char) readCmd;
		SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
		SPI_control(handle, SPI_V1_CMD_MMAP_TRANSFER_CMD,
				(void *) &transferCmd);
		retVal = SPI_transfer(handle, &transaction);

	}

	return retVal;
}

/*!
 *  @brief  This function sends write enable command to the flash device. This
 *          function has to be called first before performing any write to
 *          flash.
 *
 *  @param  flashHandle  A S25FL_Handle
 *  @return status       status of the operation
 *
 */
BOOL vxbQspiLibS25FLFlashWriteEnable(S25FL_Handle flashHandle) {
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	BOOL retVal = FALSE; /* return value */
	unsigned char writeVal; /* data to be written */
	SPI_Transaction transaction; /* SPI transaction structure */
	unsigned int operMode; /* temp variable to hold mode */
	unsigned int rxLines; /* temp variable to hold rx lines */
	unsigned int frmLength;
	unsigned int transferType;
	QSPI_v1_Object *object;
	unsigned int rxLinesArg;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* These operations require the qspi to be configured in the following mode
	 only: tx/rx single line and config mode. */

	/* Save the current mode and rxLine configurations */
	operMode = object->qspiMode;
	rxLines = object->rxLines;

	/* Update the mode and rxLines with the required values */
	SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, NULL);

	rxLinesArg = QSPI_RX_LINES_SINGLE;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLinesArg);

	/* transaction frame length in words (bytes) */
	frmLength = 1;
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *) &frmLength);

	/* Write enable command */
	writeVal = QSPI_LIB_CMD_WRITE_ENABLE;

	/* Update transaction parameters */
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Restore operating mode and rx Lines */
	object->qspiMode = operMode;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLines);

	return retVal;
}

/*!
 *  @brief  This function controls the enable/disable of the 4 byte addressing
 *          mode. If the destination flash offset address is greater than
 *          0xFFFFFFU then this mode should be enabled else not required.
 *
 *  @param  flashHandle  A S25FL_Handle
 *  @param  enableFlag   Flag controlling enable/disable of 4 byte address mode.
 *                       TRUE- enable 4 Byte mode; FALSE- disable 4 Byte mode.
 *
 */
BOOL vxbQspiLibS25FLFlashEnable4ByteAddrMode(S25FL_Handle flashHandle,
		BOOL enable4ByteMode) {
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	BOOL retVal = FALSE; /* return value */
	unsigned char writeVal; /* data to be written */
	SPI_Transaction transaction; /* SPI transaction structure */
	unsigned int operMode; /* temp variable to hold mode */
	unsigned int rxLines; /* temp variable to hold rx lines */
	unsigned int transferType;
	QSPI_v1_Object *object;
	unsigned int rxLinesArg;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* These operations require the qspi to be configured in the following mode
	 only: tx/rx single line and config mode. */

	/* Save the current mode and rxLine configurations */
	operMode = object->qspiMode;
	rxLines = object->rxLines;

	/* Update the mode and rxLines with the required values */
	SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, NULL);
	rxLinesArg = QSPI_RX_LINES_SINGLE;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLinesArg);

	/* Command to control the 4 byte mode */
	if (TRUE == enable4ByteMode) {
		writeVal = QSPI_LIB_CMD_ENTER_4_BYTE_ADDR;
	} else {
		writeVal = QSPI_LIB_CMD_EXIT_4_BYTE_ADDR;
	}

	/* Write 4 byte enable/disable command to flash */
	transaction.count = 1U; /* Frame length in bytes */
	transaction.txBuf = (unsigned char *) &writeVal; /* Value to be written */
	transaction.rxBuf = NULL; /* Nothing to read */
	transaction.arg = NULL; /* Not applicable */

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Restore operating mode and rx Lines */
	object->qspiMode = operMode;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLines);

	return retVal;
}

/*!
 *  @brief  This function reads the value of flash status register. This status
 *          is required to know whether flash transaction is completed or still
 *          going on.
 *
 *  @param  flashHandle       A S25FL_Handle
 *  @return status            value of the flash status register.
 *
 */
uint32_t vxbQspiLibS25FLFlashStatus(S25FL_Handle flashHandle) {
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	BOOL retVal = FALSE; /* return value */
	unsigned char writeVal; /* data to be written */
	SPI_Transaction transaction; /* SPI transaction structure */
	uint32_t rxData = 0U; /* received data */
	unsigned int operMode; /* temp variable to hold mode */
	unsigned int rxLines; /* temp variable to hold rx lines */
	unsigned int frmLength;
	unsigned int transferType;
	QSPI_v1_Object *object;
	unsigned int rxLinesArg;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* These operations require the qspi to be configured in the following mode
	 only: tx/rx single line and config mode. */

	/* Save the current mode and rxLine configurations */
	operMode = object->qspiMode;
	rxLines = object->rxLines;

	/* Update the mode and rxLines with the required values */
	SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, NULL);

	rxLinesArg = QSPI_RX_LINES_SINGLE;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLinesArg);

	/* Total transaction frame length in words (bytes) */
	frmLength = 1 + 1;
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *) &frmLength);

	/* Write Address Bytes */
	writeVal = QSPI_LIB_CMD_READ_STATUS_REG;
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 1U;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	if (retVal == FALSE) {
		/* Error */
	}

	/* Read the status register */
	transaction.txBuf = NULL;
	transaction.rxBuf = (unsigned char *) &rxData;
	transaction.count = 1U;

	transferType = SPI_TRANSACTION_TYPE_READ;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	if (retVal == FALSE) {
		/* Error */
	}

	/* Restore operating mode and rx Lines */
	object->qspiMode = operMode;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLines);

	return (rxData & 0xFF);
}

/*!
 *  @brief  This API enables the quad enable bit in the flash device. This will
 *          called only if 4-Line(QUAD) transfers through QSPI interface.
 *
 *  @param  flashHandle  A S25FL_Handle
 *  @return status       status of the operation
 *
 */
BOOL vxbQspiLibS25FLFlashQuadModeEnable(S25FL_Handle flashHandle) {
	SPI_Transaction transaction; /* SPI transaction structure */
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	unsigned char writeVal = 0U; /* data to be written */
	uint32_t norStatus; /* flash status value */
	BOOL retVal = FALSE; /* return value */
	unsigned int operMode; /* temp variable to hold mode */
	unsigned int rxLines; /* temp variable to hold rx lines */
	unsigned int frmLength;
	unsigned int transferType;
	QSPI_v1_Object *object;
	unsigned int rxLinesArg;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* These operations require the qspi to be configured in the following mode
	 only: tx/rx single line and config mode. */

	/* Save the current mode and rxLine configurations */
	operMode = object->qspiMode;
	rxLines = object->rxLines;

	/* Update the mode and rxLines with the required values */
	SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, NULL);

	rxLinesArg = QSPI_RX_LINES_SINGLE;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLinesArg);

	/* Set transfer length in bytes: Reading status register */
	frmLength = 1 + 1;
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *) &frmLength);

	/* Read Status register */
	writeVal = 0x05U;   //QSPI_LIB_CMD_READ_STATUS_REG;  //Cmd for AM437x
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	transaction.txBuf = NULL;
	transaction.rxBuf = (unsigned char *) &norStatus;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_READ;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Flash write enable */
	vxbQspiLibS25FLFlashWriteEnable(flashHandle);

	/* Set transfer length in bytes: Reading status register */
	frmLength = 1 + 1;
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *) &frmLength);

	/* Read command register */
	writeVal = 0x01U;   //QSPI_LIB_CMD_QUAD_RD_CMD_REG;
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Set status register 6th bit to 1 for Quad enable
	 * Write this value to the status register.
	 */
	norStatus &= ~(1U << 0x6U);
	norStatus |= (0x1U << 0x6U);

	transaction.txBuf = (unsigned char *) &norStatus;
	transaction.rxBuf = NULL;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Wait till the status register is being written */
	while (1U == (vxbQspiLibS25FLFlashStatus(flashHandle) & 0x1U))
		;

	/* Restore operating mode and rx Lines */
	object->qspiMode = operMode;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLines);

	return retVal;
}

/*!
 *  @brief  This API erases a block of QSPI flash device by sending the command
 *          through QSPI controller.

 *  @param  flashHandle  A S25FL_Handle
 *  @return none
 *
 */
BOOL vxbQspiLibS25FLFlashBlockErase(S25FL_Handle flashHandle, unsigned int blockNumber) {
	uint32_t eraseAddr; /* Flash erase address */
	uint32_t writeVal; /* data to be written */
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	SPI_Transaction transaction; /* SPI transaction structure */
	unsigned int addrLenInBytes = 3; /* address length in bytes */
	BOOL retVal = FALSE; /* return value */
	unsigned int operMode; /* temp variable to hold mode */
	unsigned int rxLines; /* temp variable to hold rx lines */
	unsigned int frmLength;
	unsigned int transferType;
	QSPI_v1_Object *object;
	unsigned int rxLinesArg;
	uint32_t tempAddr;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* Compute flash erase address based on the block size */
	eraseAddr = blockNumber * flashHandle->blockSize;

	/* These operations require the qspi to be configured in the following mode
	 only: tx/rx single line and config mode. */

	/* Save the current mode and rxLine configurations */
	operMode = object->qspiMode;
	rxLines = object->rxLines;

	/* Update the mode and rxLines with the required values */
	SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, NULL);

	rxLinesArg = QSPI_RX_LINES_SINGLE;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLinesArg);

	if (eraseAddr > 0xFFFFFF) {
		/* Enter 4 byte addressing mode */
		addrLenInBytes = 4;
	}

	tempAddr = ((eraseAddr & 0xFF000000) >> 24)
					| ((eraseAddr & 0x00FF0000) >> 8) | ((eraseAddr & 0x0000FF00) << 8)
					| ((eraseAddr & 0x000000FF) << 24);
	

	if (addrLenInBytes == 3) {
		tempAddr = (tempAddr >> 8) & 0x00FFFFFF;
	}

	/* Flash write enable */
	vxbQspiLibS25FLFlashWriteEnable(flashHandle);

	/*total transaction frame length */
	frmLength = 1 + addrLenInBytes;
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, ((void *) &frmLength));

	/* Block erase command */
	 if(eraseAddr > 0xFFFFFF)
	    {
	        writeVal = QSPI_LIB_CMD_BLOCK_ERASE_4B;
	    }
	    else
	    {
	        writeVal = QSPI_LIB_CMD_BLOCK_ERASE;
	    }
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Send erase address */
	writeVal = tempAddr;
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = addrLenInBytes;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Check flash status for write completion */
	while (1U == (vxbQspiLibS25FLFlashStatus(flashHandle) & 0x1U))
		;

	/* Restore operating mode and rx Lines */
	object->qspiMode = operMode;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLines);

	return retVal;
}

/*!
 *  @brief  This API prints the flash id.

 *  @param  flashHandle  A S25FL_Handle
 *  @return none
 *
 */
void vxbQspiLibS25FLFlashPrintId(S25FL_Handle flashHandle) {
	uint32_t writeVal; /* data to be written */
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	SPI_Transaction transaction; /* SPI transaction structure */
	BOOL retVal = FALSE; /* return value */
	unsigned char mfgId = 0; /* manufacture ID value */
	unsigned char deviceId = 0; /* device ID value */
	unsigned int operMode; /* temp variable to hold mode */
	unsigned int rxLines; /* temp variable to hold rx lines */
	unsigned int frmLength;
	unsigned int transferType;
	QSPI_v1_Object *object;
	unsigned int rxLinesArg;

	/* Get the pointer to the object and hwAttrs */
	object = handle->object;

	/* These operations require the qspi to be configured in the following mode
	 only: tx/rx single line and config mode. */

	/* Save the current mode and rxLine configurations */
	operMode = object->qspiMode;
	rxLines = object->rxLines;

	/* Update the mode and rxLines with the required values */
	SPI_control(handle, SPI_V1_CMD_SETCONFIGMODE, NULL);

	rxLinesArg = QSPI_RX_LINES_SINGLE;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLinesArg);

	/* Total transaction frame length in bytes */
	frmLength = 1 + 3 + 2; /* cmd + address + read data */
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, (void *) &frmLength);

	/* Write Command */
	writeVal = QSPI_LIB_CMD_READ_MFG_ID;
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	if (retVal == FALSE) {
	}

	/* Write Address Bytes */
	writeVal = 0x0U;
	transaction.txBuf = (unsigned char *) &writeVal;
	transaction.rxBuf = NULL;
	transaction.count = 3;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Read Manufacturer ID
	 * The manufacturer ID is of 1 byte(8 bits)
	 */
	transaction.txBuf = NULL;
	transaction.rxBuf = &mfgId;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_READ;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Read Device ID */
	transaction.txBuf = NULL;
	transaction.rxBuf = &deviceId;
	transaction.count = 1;

	transferType = SPI_TRANSACTION_TYPE_READ;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);

	retVal = SPI_transfer(handle, &transaction);

	/* Note - This ID is the device ID of the flash device.
	 * This ID is read from the flash
	 */

	/* Restore operating mode and rx Lines */
	object->qspiMode = operMode;
	SPI_control(handle, SPI_V1_CMD_SETRXLINES, (void *) &rxLines);
}

/*!
 *  @brief  This function performs the ConfigMode_Write

 *  @param  dstOffstAddr
 *  @param  srcAddr
 *  @return BOOL
 *
 */
static BOOL SF25FL_ConfigMode_Write(uint32_t dstOffstAddr,
		unsigned char* srcAddr, uint32_t length, S25FL_Handle flashHandle) {
	BOOL retVal = FALSE; /* return value */
	SPI_Handle handle = flashHandle->spiHandle; /* SPI handle */
	uint32_t addrLengthInBytes = 3U; /* Flash addr length in bytes */
	unsigned int frmLength;
	unsigned char writeVal[4]; /* data to be written to QSPI */
	SPI_Transaction transaction; /* SPI transaction structure */
	unsigned int transferType;

	addrLengthInBytes = 3U; /* Flash address length in bytes */

	/* Extract address word length from the flash's destination offst addr*/
	if (dstOffstAddr > 0xFFFFFF) {
		/* Enter 32 bit addressing mode */
		addrLengthInBytes = 4;
	}

	/* Write Enable */
	vxbQspiLibS25FLFlashWriteEnable(flashHandle);

	/* total transaction frame length in number of words (bytes)*/
	frmLength = 1 + addrLengthInBytes + length;
	SPI_control(handle, SPI_V1_CMD_SETFRAMELENGTH, ((void *) &frmLength));

	/* Send Flash write command */
	writeVal[0] = QSPI_LIB_CMD_PAGE_PRG; /* Flash write command */
	transaction.txBuf = (unsigned char *) &writeVal[0];
	transaction.rxBuf = NULL;
	transaction.count = 1U;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
	retVal = SPI_transfer(handle, &transaction);

	/* Send flash address offset to which data has to be written */
	transaction.txBuf = (unsigned char *) dstOffstAddr;
	transaction.rxBuf = NULL;
	transaction.count = addrLengthInBytes;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
	retVal = SPI_transfer(handle, &transaction);

	/* Write buffer data to flash */
	transaction.txBuf = (unsigned char *) srcAddr;
	transaction.rxBuf = NULL;
	transaction.count = length;

	transferType = SPI_TRANSACTION_TYPE_WRITE;
	SPI_control(handle, SPI_V1_CMD_TRANSFERMODE_RW, (void *) &transferType);
	retVal = SPI_transfer(handle, &transaction);

	/* Check flash status for write completion */
	while ((vxbQspiLibS25FLFlashStatus(flashHandle) & 0x1U));

	return retVal;
}

