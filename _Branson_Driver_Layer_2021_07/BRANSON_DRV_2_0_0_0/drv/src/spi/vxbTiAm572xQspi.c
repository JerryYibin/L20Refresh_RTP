/*
 * vxbTiAm572xQspi.c
 */

/*
 * 		qspi_flash@4b300000
 {
 #address-cells = <1>;
 #size-cells = <1>;
 compatible = "ti,am437x-spi";
 reg = <0x4b300000 0x100000> , <0x5c000000 0x4000000> , <0x4A002558 0x20>;
 pinmux-0 = <&qspi_pads>;
 clock-frequency = <48000000>;
 spiflash@0
 {
 #address-cells = <1>;
 #size-cells = <0>;
 compatible = "spiflash";
 reg = <0>;
 spi-max-frequency = <48000000>;
 };
 }
 */
#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbFdtSpiLib.h>
#include <subsys/int/vxbIntLib.h>
#include <subsys/pinmux/vxbPinMuxLib.h>
#include <subsys/clk/vxbClkLib.h>
#include <subsys/timer/vxbTimerLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <tickLib.h>
#include <types.h>

#include "vxbTiAm572xQspi.h"
#include <vxbQspiInterface.h>
#include <vxbSp25QspiFlash.h>
#include <hw_am572xqspi.h>

/*#define  QSPI_DBG_ON*//* turn on/off debug messages */
#ifdef  QSPI_DBG_ON

#define DEBUG_PRINT 1
/* turning local symbols into global symbols */

#define QSPI_DBG_OFF            0x00000000
#define QSPI_DBG_ISR            0x00000001
#define QSPI_DBG_RW             0x00000002
#define QSPI_DBG_ERR            0x00000004
#define QSPI_DBG_RTN            0x00000008
#define QSPI_DBG_INFO		   0x0000000C	
#define QSPI_DBG_ALL            0xffffffff

LOCAL UINT32 qspiDbgMask = QSPI_DBG_ALL;

#define QSPI_DBG(mask, ...)                                              \
		do                                                                  \
		{                                                               \
			if ((qspiDbgMask & mask) || (mask == QSPI_DBG_ALL))               \
			{                                                           \
				if (_func_kprintf != NULL)                                  \
				{                                                       \
					(* _func_kprintf)(__VA_ARGS__);                         \
				}                                                       \
			}                                                           \
		}                                                               \
		while ((FALSE))
#else
#undef QSPI_DBG
#define QSPI_DBG(...)
#endif  /* QSPI_DBG_ON */

#define TI_AM572X_SPI_NAME	"ti,Am572xSpi"

LOCAL STATUS vxbTiAm572xQSPIProbe( VXB_DEV_ID);
LOCAL STATUS vxbTiAm572xQSPIAttach( VXB_DEV_ID);
/* QSPI AM57x functions */
LOCAL VOID vxbQspiClose(SPI_Handle handle);
LOCAL STATUS vxbQspiControl(SPI_Handle handle, uint32_t cmd, const void *arg);
LOCAL VOID vxbQspiInit(SPI_Handle handle);
LOCAL SPI_Handle vxbQspiOpen(VXB_DEV_ID pDev, SPI_Handle handle,
		const SPI_Params *params);
LOCAL BOOL vxbQspiTransfer(SPI_Handle handle, SPI_Transaction *transaction);
LOCAL void vxbQspiTransferCancel(SPI_Handle handle);

static void vxbQspiPrimeTransfer(SPI_Handle handle,
		const SPI_Transaction *transaction);
/* static void vxbQspiTransferCallback(SPI_Handle handle, SPI_Transaction *msg);
static void vxbQspiHwiFxn(uintptr_t arg); */
static void vxbQspiRead(SPI_Handle handle, const SPI_Transaction *transaction);
static void vxbQspiWrite(SPI_Handle handle, const SPI_Transaction *transaction);
static void vxbQspiMmapModeWrite(SPI_Handle handle,
		const SPI_Transaction *transaction);
static void vxbQspiCmdModeWrite(SPI_Handle handle,
		const SPI_Transaction *transaction);
static void vxbQspiCmdModeRead(SPI_Handle handle,
		const SPI_Transaction *transaction);
static void vxbQspiMmapModeRead(SPI_Handle handle,
		const SPI_Transaction *transaction);
static void vxbQspiMmapCsEnable(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect);

LOCAL VXB_DRV_METHOD vxbTiQSPIMethods[] = { { VXB_DEVMETHOD_CALL(vxbDevProbe),
		(FUNCPTR) vxbTiAm572xQSPIProbe }, { VXB_DEVMETHOD_CALL(vxbDevAttach),
				(FUNCPTR) vxbTiAm572xQSPIAttach }, VXB_DEVMETHOD_END };

VXB_DRV vxbOfAm572xSpiDrv = { { NULL },
		TI_AM572X_SPI_NAME, /* Name */
		"Branson TI AM572x SPI controller", /* Description */
		VXB_BUSID_FDT, /* Class */
		0, /* Flags */
		0, /* Reference count */
		(VXB_DRV_METHOD*) &vxbTiQSPIMethods /* Method table */
};

VXB_DRV_DEF( vxbOfAm572xSpiDrv)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY vxbTiAm572xQSPIMatch[] = { {
		"ti,am572x-spi", /* compatible */
		(void *) NULL }, { } /* Empty terminated list */
};

/*******************************************************************************
 *
 * vxbTiAm572xQSPIProbe : probe for device in dts
 * 
 * This function finds and compare the compatible field from the dts file.
 * if found then success , else fail.
 */

LOCAL STATUS vxbTiAm572xQSPIProbe(struct vxbDev * pDev) {
	return vxbFdtDevMatch(pDev, vxbTiAm572xQSPIMatch, NULL);
}

/*******************************************************************************
 *
 * vxbTiAm572xQSPIAttach : initialize QSPI controller
 * 
 * This function allocates driver structure, registers and map it to
 * system.It always does the basic hardware and software initialization 
 * needed for driver and hardware functioning. 
 */
LOCAL STATUS vxbTiAm572xQSPIAttach(VXB_DEV_ID pDev) {
	TI_AM572X_SPI_CTRL * pDrvCtrl;
	VXB_RESOURCE_ADR * pResAdr = NULL;
	VXB_RESOURCE * pRes = NULL;
	VXB_RESOURCE_ADR * pFlashResAdr = NULL;
	VXB_RESOURCE * pFlashRes = NULL;
	VXB_RESOURCE_ADR * pMMControlIOResAdr = NULL;
	VXB_RESOURCE * pMMControlIORes = NULL;
	VXB_RESOURCE_ADR * pClkResAdr = NULL;
	VXB_RESOURCE * pClkRes = NULL;
	SPI_FxnTable * pQspiFunc = NULL;
	/* STATUS ret = ERROR; */
	VXB_FDT_DEV * pFdtDev;
	/* int offset; */

	/* int result, len; */

	if (pDev == NULL) {
		return ERROR;
	}

	pFdtDev = vxbFdtDevGet(pDev);
	if (pFdtDev == NULL) {
		return ERROR;
	}
	/* create controller driver context structure for core */
	/* result = vxFdtPropGet(pFdtDev->offset, "reg", &len); */
	pDrvCtrl = (TI_AM572X_SPI_CTRL *) vxbMemAlloc(sizeof(TI_AM572X_SPI_CTRL));
	if (pDrvCtrl == NULL) {
		return ERROR;
	}

	if (vxbPinMuxEnable(pDev) != OK) {
		goto attach_error;
	}
	(void) vxbClkEnableAll(pDev);
	//pDrvCtrl->clkFrequency = 48000000;
	/* save pDrvCtrl in VXB_DEVICE structure */

	vxbDevSoftcSet(pDev, pDrvCtrl);
	/* save instance ID */

	pDrvCtrl->pDev = pDev;
	pRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);

	if (pRes == NULL) {
		goto attach_error;
	}
	pResAdr = (VXB_RESOURCE_ADR *) pRes->pRes;

	if (pResAdr == NULL) {
		goto attach_error;
	}
	pDrvCtrl->regHandle = pResAdr->pHandle;
	pDrvCtrl->regBase = (void *) pResAdr->virtual;

	pFlashRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 1);

	if (pFlashRes == NULL) {
		goto attach_error;
	}
	pFlashResAdr = (VXB_RESOURCE_ADR *) pFlashRes->pRes;

	if (pFlashResAdr == NULL) {
		goto attach_error;
	}
	pDrvCtrl->regFlashHandle = pFlashResAdr->pHandle;
	pDrvCtrl->regFlashBase = (void *) pFlashResAdr->virtual;

	pMMControlIORes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 2);

	if (pMMControlIORes == NULL) {
		goto attach_error;
	}
	pMMControlIOResAdr = (VXB_RESOURCE_ADR *) pMMControlIORes->pRes;

	if (pMMControlIOResAdr == NULL) {
		goto attach_error;
	}

	pDrvCtrl->regMMControlIOHandle = pMMControlIOResAdr->pHandle;
	pDrvCtrl->regMMControlIOBase = (void *) pMMControlIOResAdr->virtual;

	/* void *clockHandle;
	void *clockBase; */

	pClkRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 3);

	if (pClkRes == NULL) {
		goto attach_error;
	}
	pClkResAdr = (VXB_RESOURCE_ADR *) pClkRes->pRes;

	if (pClkResAdr == NULL) {
		goto attach_error;
	}

	vxbWrite32(pClkResAdr->pHandle, (UINT32 *)pClkResAdr->virtAddr,
			(vxbRead32(pClkResAdr->pHandle, (UINT32 *)pClkResAdr->virtAddr) | (0x1 << 24)
					| (0x2 << 25)));

	pQspiFunc = (SPI_FxnTable *) vxbMemAlloc(sizeof(SPI_FxnTable));
	if (pQspiFunc == NULL) {
		goto attach_error;
	}

	pQspiFunc->closeFxn = vxbQspiClose;
	pQspiFunc->controlFxn = vxbQspiControl;
	pQspiFunc->initFxn = vxbQspiInit;
	pQspiFunc->openFxn = vxbQspiOpen;
	pQspiFunc->transferFxn = vxbQspiTransfer;
	pQspiFunc->transferCancelFxn = vxbQspiTransferCancel;
	pDrvCtrl->handle->fxnTablePtr = pQspiFunc;
	pDrvCtrl->handle->pInst = pDev;

	/* SPI Library Register */
	qspiRegister(pDev, pQspiFunc);

	return OK;

attach_error:

	if (pQspiFunc != NULL) {
		vxbMemFree(pQspiFunc);
	}
	if (pDrvCtrl->semSync != SEM_ID_NULL) {
		(void) semDelete(pDrvCtrl->semSync);
	}

	if (pDrvCtrl->intRes != NULL) {
		(void) vxbResourceFree(pDev, pDrvCtrl->intRes);
	}

	if (pRes != NULL) {
		(void) vxbResourceFree(pDev, pRes);
	}

	(void) vxbClkDisableAll(pDev);
	(void) vxbPinMuxDisable(pDev);
	vxbMemFree(pDrvCtrl);
	return ERROR;
}

/*
 *  This function performs the necessary initializations and returns the SPI handle.
 *  @params pDev   VXB_DEV_ID
 *  @params handle  SPI_Handle
 *  @params params  SPI_Params
 *  @return SPI_HANDLE
 */
static SPI_Handle vxbQspiOpen(VXB_DEV_ID pDev, SPI_Handle handle,
		const SPI_Params *params) {
	QSPI_v1_Object *object = NULL;
	QSPI_HwAttrs const *hwAttrs = NULL;
	uint8_t ret_flag = 0u;

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;

	object->isOpen = (BOOL) TRUE;

	/* Copy the params contents */
	object->qspiParams = *params;

	/* Copy the controller mode from hardware attributes to object */
	object->qspiMode = hwAttrs->operMode;
	object->rxLines = hwAttrs->rxLines;

	object->intrPollMode = SPI_OPER_MODE_POLLING;

	if (ret_flag == 0u) {

		vxbQspiConfigIdleMode(pDev, hwAttrs->baseAddr,
				QSPI_SYSCONFIG_IDLE_MODE_NO_IDLE);

		/* Set clock mode in mode 3 */
		vxbQSPISetClkMode(pDev, hwAttrs->baseAddr, hwAttrs->chipSelect,
				hwAttrs->frmFmt);
		vxbQSPISetCsPol(pDev, hwAttrs->baseAddr, hwAttrs->chipSelect,
				hwAttrs->csPol);
		vxbQSPISetDataDelay(pDev, hwAttrs->baseAddr, hwAttrs->chipSelect,
				hwAttrs->dataDelay);

		/* Enable clock and set divider value */
		vxbQSPISetPreScaler(pDev, hwAttrs->baseAddr, 0x0U);

		/* Clear the interrupts and interrupt status */
		vxbQspiIntDisable(pDev, hwAttrs->baseAddr,
				(QSPI_INTR_MASK_FRAME | QSPI_INTR_MASK_WORD));
		vxbQspiIntClear(pDev, hwAttrs->baseAddr,
				(QSPI_INTR_MASK_FRAME | QSPI_INTR_MASK_WORD));

		vxbQSPISetMemMapNumAddrBytes(pDev, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);

		/* Enable memory mapped port by default */
		vxbQSPISetMemAddrSpace(pDev, hwAttrs->baseAddr, hwAttrs->chipSelect,
				QSPI_MEM_MAP_PORT_SEL_MEM_MAP_PORT);

		vxbQspiMmapCsEnable(pDev, hwAttrs->baseAddr, hwAttrs->chipSelect);

		/* Return the address of the spiObjectArray[i] configuration structure */
	}
	return (handle);
}

/**
 *  \brief This function configures the idle mode of the QSPI
 *
 * \param     baseAdd   QSPI base address.
 * \param     idleMode  Idle mode to be configured. Possible values are
 *                      0x0: Force-idle. An idle request is acknowledged
 *                           unconditionally
 *                      0x1: No-idle. An idle request is never acknowledged
 *                      0x2: Smart-idle. Acknowledgement to an idle request is
 *                           given based on the internal activity of the module
 * \return    None
 */
void vxbQspiConfigIdleMode(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t idleMode) {
	/* Set the idle mode value */
	CS_WRITE_FIELD32(pDev, baseAdd + QSPI_SYSCONFIG, QSPI_SYSCONFIG_IDLE_MODE,
			idleMode);
}

/**
 *  \brief This function sets DCLK_DIV feild in SPICC register and enables clock.
 *
 * \param     baseAdd           QSPI base address.
 * \param     in_ClockSettings  Clock Settings based on enum qspi_ClockFreq_e.
 *
 * \return    None
 */
void vxbQspiSetClockControl(VXB_DEV_ID pDev, uint32_t baseAdd,
		qspi_ClockFreq_e in_ClockSettings) {
	uint32_t lv_clkDivider;

	/* get qspi CC register divider value */
	lv_clkDivider = (uint32_t) in_ClockSettings & QSPI_CLKDIVIDER_MASK;

	/* wait for the QSPI to be idle */
	vxbQspiWaitIdle(pDev, baseAdd);

	/* turn off QSPI data clock */
	CS_WRITE_FIELD32(pDev, baseAdd + QSPI_SPI_CLOCK_CNTRL_REG,
			QSPI_SPI_CLOCK_CNTRL_REG_CLKEN,
			QSPI_SPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_OFF);

	/* clear QSPI clock divider*/
	CS_WRITE_FIELD32(pDev, baseAdd + QSPI_SPI_CLOCK_CNTRL_REG,
			QSPI_SPI_CLOCK_CNTRL_REG_DCLK_DIV, 0U);

	/* update QSPI clock divider*/
	CS_WRITE_FIELD32(pDev, baseAdd + QSPI_SPI_CLOCK_CNTRL_REG,
			QSPI_SPI_CLOCK_CNTRL_REG_DCLK_DIV, lv_clkDivider);

	/* enable QSPI data clock */
	CS_WRITE_FIELD32(pDev, baseAdd + QSPI_SPI_CLOCK_CNTRL_REG,
			QSPI_SPI_CLOCK_CNTRL_REG_CLKEN,
			QSPI_SPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_ON);
}

/**
 *  \brief This function sets Device Control register.
 *
 * \param     baseAdd   QSPI base address.
 * \param     value     Value to be written to Device Control register
 *
 * \return    None
 */
void vxbQspiSetDeviceControl(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t value) {
	/* wait for the QSPI to be idle */
	vxbQspiWaitIdle(pDev, baseAdd);
	CS_WRITE_REG32(pDev, baseAdd + QSPI_SPI_DC_REG, value);
}

/**
 *  \brief This function returns value of device control register.
 *
 * \param     baseAdd   QSPI base address.
 *
 * \return    Deivce Control register value
 */
uint32_t vxbQspiGetDeviceControl(VXB_DEV_ID pDev, uint32_t baseAdd) {
	uint32_t devCtrlVal;

	devCtrlVal = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_DC_REG);

	return devCtrlVal;
}

/**
 *  \brief This function sets Command register.
 *
 * \param     baseAdd   QSPI base address.
 * \param     cmd       Value to be written to Command register
 *
 * \return    None
 */
void vxbQspiSetCommandReg(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t cmd) {
	/* wait for the QSPI to be idle */
	vxbQspiWaitIdle(pDev, baseAdd);
	CS_WRITE_REG32(pDev, baseAdd + QSPI_SPI_CMD_REG, cmd);
}

/**
 *  \brief This function returns value of Command register.
 *
 * \param     baseAdd   QSPI base address.
 *
 * \return    Command register value
 */
uint32_t vxbQspiGetCommandReg(VXB_DEV_ID pDev, uint32_t baseAdd) {
	uint32_t cmdRegVal;

	cmdRegVal = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_CMD_REG);

	return cmdRegVal;
}

/**
 *  \brief This function returns value of Status register.
 *
 * \param     baseAdd   QSPI base address.
 *
 * \return    Status register value
 */
uint32_t vxbQspiGetStatus(VXB_DEV_ID pDev, uint32_t baseAdd) {
	uint32_t cmdRegStatus;

	cmdRegStatus = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_STATUS_REG);

	return cmdRegStatus;
}

/**
 *  \brief This function returns Word count value of Status register.
 *
 * \param     baseAdd   QSPI base address.
 *
 * \return    Word count field in Status register
 */
uint32_t vxbQspiGetStatusWordCount(VXB_DEV_ID pDev, uint32_t baseAdd) {
	return ((vxbQspiGetStatus(pDev, baseAdd) &
			QSPI_SPI_STATUS_REG_WDCNT_MASK) >> QSPI_SPI_STATUS_REG_WDCNT_SHIFT);
}

/**
 *  \brief This function is blocking and waits till busy bit in status register is
 *  reset.
 *
 * \param     baseAdd   QSPI base address.
 *
 * \return    None
 */
void vxbQspiWaitIdle(VXB_DEV_ID pDev, uint32_t baseAdd) {
	/* wait while QSPI is busy */
	while ((vxbQspiGetStatus(pDev, baseAdd) & QSPI_SPI_STATUS_REG_BUSY_MASK) ==
			QSPI_SPI_STATUS_REG_BUSY_BUSY) {
		/*Do nothing */
	}
}

/**
 *  \brief This function writes to data register: data, data1, data2, data3
 *
 * \param     baseAdd   QSPI base address.
 * \param     data      pointer to source address.
 * \param     length    number of words to be written - accepted values from 1
 *                      to 4.
 *
 * \return    None
 */
void vxbQspiWriteData(VXB_DEV_ID pDev, uint32_t baseAdd, const uint32_t *data,
		int32_t length) {
	const uint32_t *pData;
	pData = data;

	if (pData != ((void *) NULL)) {
		CS_WRITE_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG, *pData);
		if (length > 1) {
			/* "Reason - Pointer arithmetic needed here" */
			pData++;
			CS_WRITE_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG_1, *pData);
		}
		if (length > 2) {
			/* "Reason - Pointer arithmetic needed here" */
			pData++;
			CS_WRITE_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG_2, *pData);
		}
		if (length > 3) {
			/* "Reason - Pointer arithmetic needed here" */
			pData++;
			CS_WRITE_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG_3, *pData);
		}
	}
}

/**
 *  \brief This function reads value of data register: data, data1, data2, data3
 *
 * \param     baseAdd   QSPI base address.
 * \param     data      pointer to destination address.
 * \param     length    number of words to be read - accepted values from 1
 *                      to 4.
 *
 * \return    None
 */
void vxbQSPIReadData(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t *data,
		int32_t length) {
	uint32_t *pData;
	pData = data;

	if (pData != ((void *) NULL)) {
		*pData = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG);
		if (length > 1) {
			/* "Reason - Pointer arithmetic needed here" */
			pData++;
			/*TI_INSPECTED 8 D: MISRAC_2012_R.2.2
			 * "Reason - LDRA tool issue not able to understand Pointer
			 *  arithmetic" */
			*pData = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG_1);
		}
		if (length > 2) {
			/* "Reason - Pointer arithmetic needed here" */
			pData++;
			*pData = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG_2);
		}
		if (length > 3) {
			/* "Reason - Pointer arithmetic needed here" */
			pData++;
			*pData = CS_READ_REG32(pDev, baseAdd + QSPI_SPI_DATA_REG_3);
		}
	}
}

/**
 * \brief This Function disables only specified QSPI interrupts
 *
 * \param   baseAdd    QSPI base address.
 * \param   intFlag    mask value for the interrupts to be enabled.
 *
 * \returns None.
 *
 */
void vxbQspiIntDisable(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t intFlag) {
	uint32_t regVal;

	regVal = CS_READ_REG32(pDev, baseAdd + QSPI_INTR_ENABLE_CLEAR_REG);

	regVal |= intFlag;

	CS_WRITE_REG32(pDev, baseAdd + QSPI_INTR_ENABLE_CLEAR_REG, regVal);
}

/**
 * \brief This Function clears the interrupts.
 *
 * \param   baseAdd    QSPI base address.
 * \param   intFlag    mask value for the interrupts to be cleared.
 *
 * \returns None.
 *
 */
void vxbQspiIntClear(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t intFlag) {
	CS_WRITE_REG32(pDev, baseAdd + QSPI_INTR_STATUS_ENABLED_CLEAR, intFlag);
}

/**
 * \brief   This function configures the QSPI to work in different clock modes
 *          for each output chip select.
 *
 * \details The combination of Clock phase
 *          and clock polarity creates the SPI mode. Most serial flash
 *          devices support only mode 0 and mode 3. Changing the clock polarity
 *          also swaps the transmit/receive clock edge relationship.
 *          If a slave device states that it receives data on the
 *          rising edge and transmits on the falling edge of the clock,
 *          then it can only support mode 0 or 3.
 *          This can only be done if QSPI module is not busy.
 *
 *          The 4 possible modes are :
 *
 *          CKP             CKPH         Description
 *          0               0            Data input captured on rising edge of
 *                                       qspi1_sclk clock. Data output generated
 *                                       on falling edge of qspi1_sclk clock
 *          0               1            Data input captured on falling edge of
 *                                       qspi1_sclk clock. Data output generate
 *                                       on rising edge of qspi1_sclk clock
 *          1               0            Data input captured on falling edge of
 *                                       qspi1_sclk clock. Data output generated
 *                                       on rising edge of qspi1_sclk clock
 *          1               1            Data input captured on rising edge of
 *                                       qspi1_sclk clock. Data output generated
 *                                       on falling edge of qspi1_sclk clock
 *
 * \param  baseAddr        Base Address of QSPI module registers.
 * \param  chipSelect      chipSelect for which the mode is to be configured.
 *                         Supported values according to enum
 *                         #qspiChipSelect_t.
 * \param  clkMode       Mode required to be selected.
 *                         Supported values given by enum #qspiClkMode_t
 */
void vxbQSPISetClkMode(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t chipSelect,
		uint32_t clkMode) {
	uint32_t regVal;

	/* read value of the device control register */
	regVal = vxbQspiGetDeviceControl(pDev, baseAddr);

	/* Set the values of clock phase and clock polarity in the device
	 * Control register for chip Select passed as the parameter to function.
	 */
	/* The clock mode for different chip select are at a separation of
	 * 8 bits in the device control register.
	 * So 8U has been multiplied to the chip select value.
	 */
	regVal &= (uint32_t)(~((QSPI_CLK_MODE_3) << (8U * chipSelect)));
	regVal |= (clkMode << (8U * chipSelect));

	vxbQspiSetDeviceControl(pDev, baseAddr, regVal);
}

/**
 * \brief   This function configures the chip select polarity for
 *          a selected chip select.
 *          This can only be done if QSPI module is not busy.
 *
 * \param   baseAddr            Base Address for QSPI module registers.
 * \param   chipSelect          chip select for which polarity is to be
 *                              configured. Supported values are
 *                              according to enum
 *                              #qspiChipSelect_t.
 * \param   csPol               Polarity value of the chip select
 *                              selected.
 *                              Supported values are given by the enum
 *                              #qspiCsPol_t
 */
void vxbQSPISetCsPol(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t chipSelect,
		uint32_t csPol) {
	/* Read value of device control register */
	uint32_t regVal;

	regVal = vxbQspiGetDeviceControl(pDev, baseAddr);

	/* Set the chip select polarity for the required chip Select */
	/* The polarity for different chip select are at a separation of
	 * 8 bits in the device control register.
	 * So 8U has been multiplied to the chip select value.
	 */
	regVal &= (uint32_t)(
			~((QSPI_CS_POL_ACTIVE_HIGH)
					<< (QSPI_SPI_DC_REG_CSP0_SHIFT + (8U * chipSelect))));
	regVal |= (csPol << (QSPI_SPI_DC_REG_CSP0_SHIFT + (8U * chipSelect)));
	/* Set the value of the device control register to configure chip
	 * polarity
	 */
	vxbQspiSetDeviceControl(pDev, baseAddr, regVal);
}

/**
 * \brief   This function configures the Data delay for each chip select.
 *          This can only be done if QSPI module is not busy.
 *
 * \param   baseAddr     Base Address for QSPI module registers.
 * \param   chipSelect   chip select for which polarity is to be configured.
 *                       Supported values are according to enum
 *                       #qspiChipSelect_t.
 * \param   dataDelay    Value of Data Delay.
 *                       Supported values are
 *                       given by enum #qspiDataDelay_t
 */
void vxbQSPISetDataDelay(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t dataDelay) {
	uint32_t regVal;

	/* Read the value of device control register */
	regVal = vxbQspiGetDeviceControl(pDev, baseAddr);

	/** Set the Data Delay value for the required chip Select */
	/* The Data delay for different chip select are at a separation of
	 * 8 bits in the device control register.
	 * So 8U has been multiplied to the chip select value.
	 */
	regVal &= (uint32_t)(
			~((QSPI_DATA_DELAY_3)
					<< (QSPI_SPI_DC_REG_DD0_SHIFT + (8U * chipSelect))));
	regVal |= (dataDelay << (QSPI_SPI_DC_REG_DD0_SHIFT + (8U * chipSelect)));

	/* Set the value of the device control register to configure data
	 delay */
	vxbQspiSetDeviceControl(pDev, baseAddr, regVal);
}

/**
 * \brief   Set the QSPI clock register divider value.
 *
 * \details This function sets the QSPI clock control register
 *          with serial data clock divider ratio (DCLK_DIV)
 *          according to the input clock provided and the output clock
 *          required.
 *          DCLK_DIV = ((input clock) / (output clock)) - 1.
 *          This function also enables the clock for QSPI module.
 *          This can only be done if QSPI module is not busy.
 *
 * \param   baseAddr      Base Address for QSPI module registers.
 * \param   clkDividerVal clock divider value to be set.
 */
void vxbQSPISetPreScaler(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t clkDividerVal) {
	/* Read the value of Clock control register */
	uint32_t regVal = CS_READ_REG32(pDev, baseAddr + QSPI_SPI_CLOCK_CNTRL_REG);
	/* wait for QSPI to be idle */
	vxbQspiWaitIdle(pDev, baseAddr);

	/* turn off QSPI data clock */
	CS_SET_FIELD(regVal, QSPI_SPI_CLOCK_CNTRL_REG_CLKEN,
			QSPI_SPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_OFF);
	/* Set the value of QSPI clock control register */
	CS_WRITE_REG32(pDev, baseAddr + QSPI_SPI_CLOCK_CNTRL_REG, regVal);

	/* Set the QSPI clock divider bit field value*/
	CS_SET_FIELD(regVal, QSPI_SPI_CLOCK_CNTRL_REG_DCLK_DIV, clkDividerVal);
	/* Set the value of QSPI clock control register */
	CS_WRITE_REG32(pDev, baseAddr + QSPI_SPI_CLOCK_CNTRL_REG, regVal);

	/* Enable the QSPI data clock */
	CS_SET_FIELD(regVal, QSPI_SPI_CLOCK_CNTRL_REG_CLKEN,
			QSPI_SPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_ON);
	/* Set the value of QSPI clock control register */
	CS_WRITE_REG32(pDev, baseAddr + QSPI_SPI_CLOCK_CNTRL_REG, regVal);
}

/**
 * \brief   Set number of address bytes for memory mapped mode.
 *
 * \details This function configures the number af Address Bytes to be
 *          sent for memory mapped protocol translator for selected chip
 *          select.By default, the device uses 3 address bytes.
 *          This can only be done if QSPI module is not busy.
 *
 * \param   baseAddr           Base Address for QSPI module registers.
 * \param   chipSelect         chip select for which polarity is to be
 *                             configured.
 *                             Supported values are according to enum
 *                             #qspiChipSelect_t.
 * \param   numAddrBytes    Number of Address bytes to be sent for
 *                          memory mapped protocol translator.
 *                          Supported values are :
 *                          for each chip select given by enum
 *                          #qspiMemMapNumAddrBytes_t.
 */
void vxbQSPISetMemMapNumAddrBytes(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t numAddrBytes) {
	uint32_t addr = baseAddr + QSPI_SPI_SETUP_REG(chipSelect);
	CS_WRITE_FIELD32(pDev, addr, QSPI_SPI_SETUP0_REG_NUM_A_BYTES, numAddrBytes);
}

/**
 * \brief    Set Memory address space
 *
 * \details  This function selects the memory mapped mode in
 *           SPI switch register and configures mapped address
 *           for different chip select in control module.
 *           It provides a functionality for
 *           remapping the memory mapped address space to one of
 *           the four supported chip selects or to the
 *           configuration registers.
 *
 * \param    baseAddr            Base Address for QSPI module registers
 * \param    memMappedPortSwitch Switch for selection of memory mapped port
 *                               and configuration port.
 *                               Supported values are given by enum
 *                               qspiMemMapPortSel_t.
 * \param    chipSelect    chip select for which polarity is to be configured.
 *                         Supported values given by #qspiChipSelect_t.
 */
void vxbQSPISetMemAddrSpace(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t memMappedPortSwitch) {
	CS_WRITE_FIELD32(pDev, baseAddr + QSPI_SPI_SWITCH_REG,
			QSPI_SPI_SWITCH_REG_MMPT_S, memMappedPortSwitch);
}

/**
 * \brief    Set Read Command in memory mapped mode.
 *
 * \details  This function configures the read command for memory
 *           mapped protocol translator in memory mapped SPI setup register.
 *           The command send is according to the slave device connected.
 *           By default, the device uses a read command of 0x3.
 *           This can only be done if QSPI module is not busy.
 *
 * \param    baseAddr      Base Address for QSPI module registers.
 * \param    chipSelect    chip select for which polarity is to be configured.
 *                         Supported values are according to enum
 *                         #qspiChipSelect_t.
 * \param    readCmd       Read Command to be sent to the flash device.
 *                         This command is specific to the flash device being
 *                         used.
 */
void vxbQSPISetMemMapReadCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t readCmd) {
	uint32_t addr = baseAddr + QSPI_SPI_SETUP_REG(chipSelect);
	CS_WRITE_FIELD32(pDev, addr, QSPI_SPI_SETUP0_REG_RCMD, readCmd);
}

/**
 * \brief   Set read type in memory mapped mode.
 *
 * \details This function configures the read type for QSPI in memory mapped
 *          mode for selected chip select.Determines if the read command
 *          is a single, dual or quad read mode.
 *          This can only be done if QSPI module is not busy.
 *
 * \param   baseAddr       Base Address for QSPI module registers.
 * \param   chipSelect     chip select for which polarity is to be configured.
 *                         Supported values are according to enum
 *                         #qspiChipSelect_t.
 * \param   readType       Read type to be chosen.
 *                         Supported values are given by enum
 *                         #qspiMemMapReadType_t.
 */
void vxbQSPISetMemMapReadType(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t readType) {
	uint32_t addr = baseAddr + QSPI_SPI_SETUP_REG(chipSelect);
	CS_WRITE_FIELD32(pDev, addr, QSPI_SPI_SETUP0_REG_READ_TYPE, readType);
}

/**
 * \brief   Set Number of dummy bits for memory mapped mode.
 *
 * \details This function configures the number of Dummy Bits to be
 *          sent for memory mapped protocol translator for selected chip
 *          select for fast read mode.
 *          This can only be done if QSPI module is not busy.
 *
 * \param   baseAddr        Base Address for QSPI module registers.
 * \param   chipSelect      chip select for which polarity is to be configured.
 *                          Supported values are according to enum
 *                          #qspiChipSelect_t.
 * \param   numDummyBits    Number of Dummy Bits to be sent for
 *                          memory mapped protocol translator.
 *                          User can pass any value from 0 to 32 bits.
 */
void vxbQSPISetMemMapNumDummyBits(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t numDummyBits) {
	uint32_t addr = baseAddr + QSPI_SPI_SETUP_REG(chipSelect);
	/* Read the value of setup register for given chip select */
	uint32_t regVal = CS_READ_REG32(pDev, addr);
	uint32_t numDummyBytes;
	/* set number of dummy bits or bytes in setup register for transfer of
	 * data in memory mapped mode
	 */
	numDummyBytes = numDummyBits / 8U;
	numDummyBits = numDummyBits % 8U;
	CS_SET_FIELD(regVal, QSPI_SPI_SETUP0_REG_NUM_D_BITS, numDummyBits);
	CS_SET_FIELD(regVal, QSPI_SPI_SETUP0_REG_NUM_D_BYTES, numDummyBytes);
	CS_WRITE_REG32(pDev, addr, regVal);
}

/**
 * \brief    Set write command in memory mapped mode.
 *
 * \details  This function configures the write command for memory
 *           mapped protocol translator in memory mapped SPI setup register.
 *           The command send is according to the slave device connected.
 *           By default, the device uses a write command of 0x2.
 *           This can only be done if QSPI module is not busy.
 *
 * \param    baseAddr      Base Address for QSPI module registers.
 * \param    chipSelect    chip select for which polarity is to be configured.
 *                         Supported values are according to enum
 *                         #qspiChipSelect_t.
 * \param    writeCmd      Write Command to be sent to the flash device.
 *                         This command is specific to the flash device being
 *                         used.
 */
void vxbQSPISetMemMapWriteCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t writeCmd) {
	uint32_t addr = baseAddr + QSPI_SPI_SETUP_REG(chipSelect);
	CS_WRITE_FIELD32(pDev, addr, QSPI_SPI_SETUP0_REG_WCMD, writeCmd);
}

/*
 *  This functions Disables the interrupts and marke the object as closed.
 *  @params handle SPI_Handle
 */
static void vxbQspiClose(SPI_Handle handle) {
	QSPI_v1_Object *object = NULL;
	QSPI_HwAttrs const *hwAttrs = NULL;

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;

	/* Mask I2C interrupts */
	vxbQspiIntDisable(handle->pInst, hwAttrs->baseAddr,
			(QSPI_INTR_MASK_FRAME | QSPI_INTR_MASK_WORD));

	/* Open flag is set FALSE */
	object->isOpen = (BOOL) FALSE;

	return;
}

/*
 *  This is QSPI control function function, Based on the cmd It performs certain actions.
 *  @params handle SPI_Handle
 *  @params cmd    uint32_t
 *  @params arg    const void *
 */
static int32_t vxbQspiControl(SPI_Handle handle, uint32_t cmd, const void *arg) {
	QSPI_v1_Object *object; /* QSPI object */
	int32_t retVal;

	/* Get the pointer to the object */
	object = (QSPI_v1_Object*) handle->object;

	switch (cmd) {
	case SPI_V1_CMD_SETFRAMELENGTH: {
		object->frmLength = *(uint32_t *) arg;
		retVal = SPI_STATUS_SUCCESS;
		break;
	}

	case SPI_V1_CMD_MMAP_TRANSFER_CMD: {
		object->transferCmd = *(uint8_t *) arg;
		retVal = SPI_STATUS_SUCCESS;
		break;
	}

	case SPI_V1_CMD_TRANSFERMODE_RW: {
		object->transactionType = *(uint32_t *) arg;
		retVal = SPI_STATUS_SUCCESS;
		break;
	}

	case SPI_V1_CMD_SETCONFIGMODE: {
		object->qspiMode = QSPI_OPER_MODE_CFG;
		retVal = SPI_STATUS_SUCCESS;
		break;
	}

	case SPI_V1_CMD_SETRXLINES: {
		object->rxLines = *(uint32_t *) arg;
		retVal = SPI_STATUS_SUCCESS;
		break;
	}

	case SPI_V1_CMD_SETMEMMORYMAPMODE: {
		object->qspiMode = QSPI_OPER_MODE_MMAP;
		retVal = SPI_STATUS_SUCCESS;
		break;
	}

	default:
		retVal = SPI_STATUS_UNDEFINEDCMD;
		break;
	}

	return retVal;
}

/*
 *  This function cancels the transfer the command.
 *  @params handle SPI_Handle
 */
static void vxbQspiTransferCancel(SPI_Handle handle) {

	return;
}

/*
 * This is a QSPI transfer function.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 *  return  return TRUE if transfer is completed or else FALSE
 */
static BOOL vxbQspiTransfer(SPI_Handle handle, SPI_Transaction *transaction) {
	BOOL ret = FALSE; /* return value */
	QSPI_v1_Object *object; /* QSPI object */
	QSPI_HwAttrs const *hwAttrs; /* QSPI hardware attributes */

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;

	/* Check if anything needs to be written or read */
	if (0 != transaction->count) {
		transaction->status = SPI_TRANSFER_STARTED;

		/* Book keeping of transmit and receive buffers. */
		object->writeBufIdx = (uint8_t *) transaction->txBuf;
		object->writeCountIdx = transaction->count;
		object->readBufIdx = (uint8_t *) transaction->rxBuf;
		object->readCountIdx = transaction->count;

		vxbQspiPrimeTransfer(handle, transaction);

		if (object->intrPollMode == SPI_OPER_MODE_BLOCKING) {
#ifdef WORD_INTERRUPT
			/* transfer is completed and semaphore is posted. */
			ret = (BOOL)TRUE;
#else
			/* If the word count is zero then wait for semaphore */
			if (object->frmLength
					== vxbQspiGetStatusWordCount(handle->pInst,
							hwAttrs->baseAddr)) {

			}

			/* transfer is completed and semaphore is posted. */
			transaction->status = SPI_TRANSFER_COMPLETED;
			ret = (BOOL) TRUE;
#endif
		} else {
			/* Always return true if in Asynchronous mode */
			ret = (BOOL) TRUE;
		}

	} else {
		transaction->status = SPI_TRANSFER_CANCELED;
	}

	/* Return the number of bytes transferred by the I2C */
	return (ret);
}

/*
 *  This is a QSPI Initialization function.
 *  @params handle         SPI_Handle
 */
static void vxbQspiInit(SPI_Handle handle) {

	/* Mark the object as available */
	((QSPI_v1_Object *) (handle->object))->isOpen = (BOOL) FALSE;
}

/*
 *  This function is to enable Chip select.
 *  @params pDev        QSPI Node
 *  @params baseAddr    QSPI module configuration registers base address.
 *  @params chipselect  chip select value
 */
static void vxbQspiMmapCsEnable(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect) {

	uint32_t regVal;

	regVal = CS_READ_REG32(pDev, CTRL_CORE_CONTROL_IO_2);
	regVal |= ((chipSelect + 1U) << QSPI_MMAP_CS_SHIFT);
	CS_WRITE_REG32(pDev, CTRL_CORE_CONTROL_IO_2, regVal);

}

/*
 *  This function calls the QSPI Read and Write based on the TransactionType
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 */
static void vxbQspiPrimeTransfer(SPI_Handle handle,
		const SPI_Transaction *transaction) {
	QSPI_v1_Object *object = NULL;
	QSPI_HwAttrs const *hwAttrs = NULL;

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;

	/* Store the new internal counters and pointers */
	object->writeBufIdx = (uint8_t*) transaction->txBuf;
	object->writeCountIdx = transaction->count;

	object->readBufIdx = (uint8_t*) transaction->rxBuf;
	object->readCountIdx = transaction->count;

	/* Polling Mode */
	if (object->intrPollMode == SPI_OPER_MODE_POLLING) {
		/* No Specific implementation is needed here. */
	}

	/* Identify the direction of transfer (whether read/write) */
	if (SPI_TRANSACTION_TYPE_READ == object->transactionType) {
		vxbQspiRead(handle, transaction);
	}

	if (SPI_TRANSACTION_TYPE_WRITE == object->transactionType) {
		vxbQspiWrite(handle, transaction);
	}
}

/*
 *  This function performs the mmap mode read.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 */

static void vxbQspiMmapModeRead(SPI_Handle handle,
		const SPI_Transaction *transaction) {
	QSPI_HwAttrs const *hwAttrs = NULL;
	uint32_t *pDst = NULL;
	uint8_t *pSrc = NULL;
	uint32_t count;
	uint32_t mmapReadCmd = 0U;
	QSPI_v1_Object *object;
	uint32_t offset;

	pDst = (uint32_t *) transaction->rxBuf;
	pSrc = (uint8_t *) transaction->txBuf;
	count = transaction->count;

	/* Get the pointer to the object and hwAttrs */
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;
	object = (QSPI_v1_Object*) handle->object;

	TI_AM572X_SPI_CTRL * pDrvCtrl;

	pDrvCtrl = (TI_AM572X_SPI_CTRL *) vxbMemAlloc(sizeof(TI_AM572X_SPI_CTRL));
	if (pDrvCtrl == NULL) {
		return;
	}

	uint32_t temp_addr = ((uint32_t) pDrvCtrl->regFlashBase
			+ (uint32_t) transaction->txBuf);
	pSrc = ((uint8_t *) (temp_addr));

	offset = (uint32_t) transaction->txBuf;
	/* Extract memory map mode read command */
	mmapReadCmd = (uint32_t) object->transferCmd;

	/* Set the number of address bytes to three */
	vxbQSPISetMemMapNumAddrBytes(handle->pInst, hwAttrs->baseAddr,
			hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);

	/* 4 byte addressing mode */
	if (offset > (uint32_t) 0xFFFFFF) {
		/* Enable 4 byte addressing mode */
		vxbQSPISetMemMapNumAddrBytes(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR);
	}

	switch (object->rxLines) {
	case QSPI_RX_LINES_SINGLE: {
		vxbQSPISetMemMapReadCmd(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, mmapReadCmd);
		vxbQSPISetMemMapReadType(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_READ_TYPE_NORMAL);
		vxbQSPISetMemMapNumDummyBits(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, 0x0U);
		break;
	}

	case QSPI_RX_LINES_DUAL: {
		vxbQSPISetMemMapReadCmd(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, mmapReadCmd);
		vxbQSPISetMemMapReadType(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_READ_TYPE_DUAL);
		vxbQSPISetMemMapNumDummyBits(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, 0x8U);
		break;
	}

	case QSPI_RX_LINES_QUAD: {
		vxbQSPISetMemMapReadCmd(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, mmapReadCmd);
		vxbQSPISetMemMapReadType(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_READ_TYPE_QUAD);
		vxbQSPISetMemMapNumDummyBits(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, 0x8U);
		break;
	}

	default:
		break;
	}

	uint32_t addr = ((uint32_t) hwAttrs->memMappedBaseAddr + (uint32_t) offset);
	if (count != 0) {
		if ((count % 4) == 0)
			count = (count / 4);
		else {
			count = ((count / 4) + 1);
		}
	}

	while (count) {
		*pDst = CS_READ_REG32(handle->pInst, addr);
		pDst++;
		pSrc++;
		addr = addr + 4;
		count--;
	}

	/* 4 byte addressing mode */
	if (offset > (uint32_t) 0xFFFFFF) {
		/*
		 * After the transfer switch back to 3 byte addressing mode.
		 * This is the default mode.
		 */
		vxbQSPISetMemMapNumAddrBytes(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);
	}
}

/*
 *  This function performs the cmd mode read.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 */
static void vxbQspiCmdModeRead(SPI_Handle handle,
		const SPI_Transaction *transaction) {
	QSPI_v1_Object *object; /* QSPI object */
	QSPI_HwAttrs const *hwAttrs; /* QSPI hardware attributes */
	uint32_t count = 0U; /* transfer length in bytes */
	uint32_t wordLenBytes = 0U; /* Word length in number of bytes */
	uint32_t cmd = 0; /* transmit command */
	uint32_t dataVal[4] = { 0U, 0U, 0U, 0U }; /* data to be written */
	uint32_t idx, idx1, idx2, idx3; /* indexes */
	uint32_t numWords = 0U; /* number of words */
	uint8_t *dstAddr; /* destination address */

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;

	dstAddr = object->readBufIdx;

	/*formulate the command */
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_FLEN, (object->frmLength - 1));
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_WLEN, (object->qspiParams.dataSize - 1));
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_CSNUM, hwAttrs->chipSelect);

	switch (object->rxLines) {
	case QSPI_RX_LINES_SINGLE: {
		CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_CMD,
				QSPI_SPI_CMD_REG_CMD_FOUR_PIN_READ_SINGLE);
		break;
	}
	case QSPI_RX_LINES_DUAL: {
		CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_CMD,
				QSPI_SPI_CMD_REG_CMD_FOUR_PIN_READ_DUAL);
		break;
	}
	case QSPI_RX_LINES_QUAD: {
		CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_CMD,
				QSPI_SPI_CMD_REG_CMD_SIX_PIN_READ_QUAD);
		break;
	}

	default:
		break;
	}

	/* Disable word count interrupt */
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_WIRQ, FALSE);
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_FIRQ, FALSE);

	/* Extract frame length in bytes */
	count = transaction->count;

	/* Extract word length in number of bytes */
	wordLenBytes = (object->qspiParams.dataSize >> 3U);

	/* Write the data into shift registers */
	while (count) {
		/* Write tx command to command register */
		vxbQspiSetCommandReg(handle->pInst, hwAttrs->baseAddr, cmd);

#ifdef WORD_INTERRUPT
		/* interrupt mode */
		if(SPI_OPER_MODE_BLOCKING == object->intrPollMode)
		{
			/* wait for the lock posted form the word completion interrupt */
			SPI_osalPendLock(object->transferComplete, SemaphoreP_WAIT_FOREVER);
		}

		if(SPI_OPER_MODE_POLLING == object->intrPollMode)
		{
			/* Wait for the QSPI busy status */
			vxbQspiWaitIdle(hwAttrs->baseAddr);
		}
#else

		/* Wait for the QSPI busy status */
		vxbQspiWaitIdle(handle->pInst, hwAttrs->baseAddr);
#endif

		if (wordLenBytes <= 4U) {
			numWords = 1U;

			/* Read data from the data registers */
			vxbQSPIReadData(handle->pInst, hwAttrs->baseAddr, &dataVal[0],
					(int32_t) numWords);

			/* Write the extracted data into receive buffer */
			for (idx = 0U; idx < wordLenBytes; idx++) {
				*dstAddr = (uint8_t)(
						(dataVal[0] >> (8U - (8U * (idx + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}
		} else if ((wordLenBytes > 4U) && (wordLenBytes <= 8U)) {
			numWords = 2U;

			/* Read data from the data registers */
			vxbQSPIReadData(handle->pInst, hwAttrs->baseAddr, &dataVal[0],
					(int32_t) numWords);

			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < 4U; idx++) {
				*dstAddr = (uint8_t)(
						(dataVal[0] >> (8U - (8U * (idx + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}

			for (idx1 = 4U; idx1 < wordLenBytes; idx1++) {
				*dstAddr = (uint8_t)(
						(dataVal[1] >> (8U - (8U * (idx1 + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}
		} else if ((wordLenBytes > 8U) && (wordLenBytes <= 12U)) {
			numWords = 3U;

			/* Read data from the data registers */
			vxbQSPIReadData(handle->pInst, hwAttrs->baseAddr, &dataVal[0],
					(int32_t) numWords);

			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < 4U; idx++) {
				*dstAddr = (uint8_t)(
						(dataVal[0] >> (8U - (8U * (idx + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}

			for (idx1 = 4U; idx1 < 8U; idx1++) {
				*dstAddr = (uint8_t)(
						(dataVal[1] >> (8U - (8U * (idx1 + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}

			for (idx2 = 8U; idx2 < 12U; idx2++) {
				*dstAddr = (uint8_t)(
						(dataVal[2] >> (8U - (8U * (idx2 + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}
		} else if ((wordLenBytes > 12U) && (wordLenBytes <= 16U)) {
			numWords = 4U;

			/* Read data from the data registers */
			vxbQSPIReadData(handle->pInst, hwAttrs->baseAddr, &dataVal[0],
					(int32_t) numWords);

			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < 4U; idx++) {
				*dstAddr = (uint8_t)(
						(dataVal[0] >> (8U - (8U * (idx + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}

			for (idx1 = 4U; idx1 < 8U; idx1++) {
				*dstAddr = (uint8_t)(
						(dataVal[1] >> (8U - (8U * (idx1 + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}

			for (idx2 = 8U; idx2 < 12U; idx2++) {
				*dstAddr = (uint8_t)(
						(dataVal[2] >> (8U - (8U * (idx2 + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}

			for (idx3 = 12U; idx3 < 16U; idx3++) {
				*dstAddr = (uint8_t)(
						(dataVal[3] >> (8U - (8U * (idx3 + 1U))))
						& (0x000000FFU));
				dstAddr++;
			}
		} else {
			/* Not supported */
		}

		/* Update the number of bytes to be transmitted */
		count -= wordLenBytes;
	}

	/* Update read buffer index in the object */
	object->readBufIdx = dstAddr;
}

/*
 *  This function calls the specific type of read based on the qspiMode.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 */
static void vxbQspiRead(SPI_Handle handle, const SPI_Transaction *transaction) {
	QSPI_v1_Object *object = NULL;

	object = (QSPI_v1_Object*) handle->object;

	if (QSPI_OPER_MODE_MMAP == object->qspiMode) {
		vxbQspiMmapModeRead(handle, transaction);
	}

	if (QSPI_OPER_MODE_CFG == object->qspiMode) {
		vxbQspiCmdModeRead(handle, transaction);
	}
}

/*
 *  This function performs the memory map mode write.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 */
static void vxbQspiMmapModeWrite(SPI_Handle handle,
		const SPI_Transaction *transaction) {
	QSPI_HwAttrs const *hwAttrs; /* QSPI hardware attributes */
	uint32_t mmapWriteCmd = 0U; /* Mmap command */
	uint32_t count = 0U; /* transaction length */
	uint8_t *pSrc; /* Source address */
	uint8_t *pDst; /* Destination address */
	QSPI_v1_Object *object; /* QSPI object */
	uint32_t offset;

	/* Copy flash transaction parameters to local variables */
	pSrc = (uint8_t*) transaction->rxBuf;
	pDst = (uint8_t*) transaction->txBuf;
	count = transaction->count;

	/* Get the pointer to the object and hwAttrs */
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;
	object = (QSPI_v1_Object*) handle->object;

	TI_AM572X_SPI_CTRL * pDrvCtrl;

//	pDrvCtrl = (TI_AM572X_SPI_CTRL *) vxbMemAlloc(sizeof(TI_AM572X_SPI_CTRL));
//	if (pDrvCtrl == NULL) {
//		return ERROR;
//	}

	/* Compute the flash destination address */
	pDrvCtrl = vxbDevSoftcGet(handle->pInst);
	uint32_t temp_addr = ((uint32_t) pDrvCtrl->regFlashBase
			+ (uint32_t) transaction->txBuf);
	pDst = ((uint8_t *) (temp_addr));

	offset = (uint32_t) transaction->txBuf;

	/* Extract memory map mode command */
	mmapWriteCmd = (uint32_t) object->transferCmd;

	vxbQSPISetMemMapWriteCmd(handle->pInst, hwAttrs->baseAddr,
			hwAttrs->chipSelect, mmapWriteCmd);
	vxbQSPISetMemMapNumAddrBytes(handle->pInst, hwAttrs->baseAddr,
			hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE);
	vxbQSPISetMemAddrSpace(handle->pInst, hwAttrs->baseAddr,
			hwAttrs->chipSelect, QSPI_MEM_MAP_PORT_SEL_MEM_MAP_PORT);
	vxbQspiMmapCsEnable(handle->pInst, hwAttrs->baseAddr, hwAttrs->chipSelect);
	/* 4 byte addressing mode. */
	if (offset > (uint32_t) 0xFFFFFF) {
		/* Switch to four byte addressing mode */
		vxbQSPISetMemMapNumAddrBytes(handle->pInst, hwAttrs->baseAddr,
				hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR);
	}
	while (count) {
		*pDst = *pSrc;
		pDst++;
		pSrc++;
		count--;
	}
	//Duplicate code
//	/* 4 byte addressing mode. */
//	if (offset > (uint32_t) 0xFFFFFF) {
//		/* Switch back to three byte addressing mode. */
//		vxbQSPISetMemMapNumAddrBytes(handle->pInst, hwAttrs->baseAddr,
//				hwAttrs->chipSelect, QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR);
//	}
//	if(pDrvCtrl)
//	{
//		vxbMemFree(pDrvCtrl);
//	}
}

/*
 *  This function performs the QSPI cmd mode write.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer
 */
static void vxbQspiCmdModeWrite(SPI_Handle handle,
		const SPI_Transaction *transaction) {
	QSPI_v1_Object *object; /* QSPI object */
	QSPI_HwAttrs const *hwAttrs; /* QSPI hardware attributes */
	uint32_t count = 0U; /* transfer length in bytes */
	uint32_t wordLenBytes = 0U; /* Word length in number of bytes */
	uint32_t cmd = 0; /* transmit command */
	uint32_t dataVal[4] = { 0U, 0U, 0U, 0U }; /* data to be written */
	uint32_t idx, idx1, idx2, idx3; /* indexes */
	uint32_t numWords = 0U; /* number of words */
	uint8_t *srcAddr; /* Source address */

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;
	hwAttrs = (const QSPI_HwAttrs *) handle->hwAttrs;

	srcAddr = object->writeBufIdx;

	/* formulate the command */
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_FLEN, (object->frmLength - 1));
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_WLEN, (object->qspiParams.dataSize - 1));
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_CSNUM, hwAttrs->chipSelect);
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_CMD,
			QSPI_SPI_CMD_REG_CMD_FOUR_PIN_WRITE_SINGLE);

	/* Disable word count interrupt */
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_WIRQ, FALSE);
	CS_SET_FIELD(cmd, QSPI_SPI_CMD_REG_FIRQ, FALSE);
	//    }

	/* Extract frame length in bytes */
	count = transaction->count;

	/* Extract word length in number of bytes */
	wordLenBytes = (object->qspiParams.dataSize >> 3U);

	/* Write the data into shift registers */
	while (count) {
		dataVal[0] = 0;
		dataVal[1] = 0;
		dataVal[2] = 0;
		dataVal[3] = 0;

		if (wordLenBytes <= 4U) {
			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < wordLenBytes; idx++) {
				dataVal[0] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			numWords = 1U;
		} else if ((wordLenBytes > 4U) && (wordLenBytes <= 8U)) {
			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < 4U; idx++) {
				dataVal[0] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			for (idx1 = 4U; idx1 < wordLenBytes; idx1++) {
				dataVal[1] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			numWords = 2U;
		} else if ((wordLenBytes > 8U) && (wordLenBytes <= 12U)) {
			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < 4U; idx++) {
				dataVal[0] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			for (idx1 = 4U; idx1 < 8U; idx1++) {
				dataVal[1] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			for (idx2 = 8U; idx2 < 12U; idx2++) {
				dataVal[2] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			numWords = 3U;
		} else if ((wordLenBytes > 12U) && (wordLenBytes <= 16U)) {
			/* Formulate the 32 bit word to write to data register */
			for (idx = 0U; idx < 4U; idx++) {
				dataVal[0] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			for (idx1 = 4U; idx1 < 8U; idx1++) {
				dataVal[1] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			for (idx2 = 8U; idx2 < 12U; idx2++) {
				dataVal[2] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			for (idx3 = 12U; idx3 < 16U; idx3++) {
				dataVal[3] |=
						(((uint32_t)(*srcAddr) << (8u - (8u * (idx + 1u)))));
				srcAddr++;
			}

			numWords = 4U;
		} else {
			/* Not supported */
		}

		/* Write data to data registers */
		vxbQspiWriteData(handle->pInst, hwAttrs->baseAddr, &dataVal[0],
				(int32_t) numWords);

		/* Write tx command to command register */
		vxbQspiSetCommandReg(handle->pInst, hwAttrs->baseAddr, cmd);

#ifdef WORD_INTERRUPT
		/* interrupt mode */
		if(SPI_OPER_MODE_BLOCKING == object->intrPollMode)
		{
			/* wait for the lock posted form the word completion interrupt */
			SPI_osalPendLock(object->transferComplete, SemaphoreP_WAIT_FOREVER);
		}

		if(SPI_OPER_MODE_POLLING == object->intrPollMode)
		{
			/* Wait for the QSPI busy status */
			vxbQspiWaitIdle(hwAttrs->baseAddr);
		}
#else

		/* Wait for the QSPI busy status */
		vxbQspiWaitIdle(handle->pInst, hwAttrs->baseAddr);
#endif

		/* Update the number of bytes to be transmitted */
		count -= wordLenBytes;
	}

	/* Update write buffer index in the object */
	object->writeBufIdx = srcAddr;
}

/*
 *  This function calls the specific QSPI write based on the qspiMode.
 *  @params handle         SPI_Handle
 *  @params transaction    SPI_Transaction pointer 
 */
static void vxbQspiWrite(SPI_Handle handle, const SPI_Transaction *transaction) {
	QSPI_v1_Object *object = NULL; /* QSPI object */

	/* Get the pointer to the object and hwAttrs */
	object = (QSPI_v1_Object*) handle->object;

	if (QSPI_OPER_MODE_MMAP == object->qspiMode) {
		vxbQspiMmapModeWrite(handle, transaction);
	}

	if (QSPI_OPER_MODE_CFG == object->qspiMode) {
		vxbQspiCmdModeWrite(handle, transaction);
	}
}
