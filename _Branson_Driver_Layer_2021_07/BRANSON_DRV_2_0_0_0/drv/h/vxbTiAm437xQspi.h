/**
 * \file       qspi.h
 *
 * \brief      This file contains the function prototypes for the device
 *             abstraction layer for QSPI. It also contains some
 *             related macro definitions and enum definitions.
 *
 *             The QSPI module support single, dual and quad read mode
 *             in interrupt and polling mode using QSPI data registers
 *             and also support memory mapped mode.
 */
#ifndef QSPI_H_
#define QSPI_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#include <hw_am437xqspi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                          Macros and typedefs                               */
/* ========================================================================== */

#define QSPI_CORE_START 		0x47900000
#define QSPI_CORE_END 			0x479FFFFF
#define QSPI_FLASH_MMAP_START 	0x30000000
#define QSPI_FLASH_MMAP_END 	0x33FFFFFF

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/**
 * QSPI Device Structure.
 */
typedef struct ti_am437x_spi_drv_ctrl
    {
    VXB_DEV_ID      pDev;
    void *          regBase;
    void *          regHandle;
    void *          regFlashBase;
    void *          regFlashHandle;
    UINT32          clkFrequency;
    UINT8 *         txBuf;
    UINT32          txLen;
    UINT32          txLeft;
    UINT8 *         rxBuf;
    UINT32          rxLen;
    UINT32          rxLeft;
    UINT32			cmd;
    UINT32			dc;
    UINT32          curWorkingFrq;
    SEM_ID          semSync;
    VXB_RESOURCE *  intRes;
    BOOL			mmap_enabled;
    } TI_AM437X_SPI_CTRL;
    
/**
 * Enumerates the different IDLE modes available for
 *            QSPI module.
 */
typedef enum qspiSysConfigIdleMode
{
    QSPI_SYS_CONFIG_IDLE_MODE_FORCE = QSPI_SYSCONFIG_IDLE_MODE_FORCE,
    /**< Force IDLE Mode.*/
    QSPI_SYS_CONFIG_IDLE_MODE_NO = QSPI_SYSCONFIG_IDLE_MODE_NO,
    /**< No IDLE mode. QSPI_FCLK is never gated by QSPI.*/
    QSPI_SYS_CONFIG_IDLE_MODE_SMART = QSPI_SYSCONFIG_IDLE_MODE_SMART,
    /**< Smart IDLE Mode.*/
    QSPI_SYS_CONFIG_IDLE_MODE_SMART_WAKEUP_CAPABLE =
        QSPI_SYSCONFIG_IDLE_MODE_SMART_WAKEUP_CAPABLE,
    /**< Smart Wakeup Capable Idle Mode.*/
} qspiSysConfigIdleMode_t;

/**
 * Enumerates the four possible clock modes for QSPI.
 *         Each mode has different values of clock phase(CKPH)
 *         and clock polarity(CKP)
 */
typedef enum qspiClkMode
{
    QSPI_CLK_MODE_0 = (QSPI_DC_REG_CKPH0_CKP_0_SHIFT_OUT_FALLING_EDGE <<
                       QSPI_DC_REG_CKPH0_SHIFT) |
                      (QSPI_DC_REG_CKP0_DATA_INACTIVE <<
                       QSPI_DC_REG_CKP0_SHIFT),
    /**< Mode 0 - Clock polarity = 0, clock phase = 0*/
    QSPI_CLK_MODE_1 = (QSPI_DC_REG_CKPH0_CKP_0_SHIFT_OUT_RISING_EDGE <<
                       QSPI_DC_REG_CKPH0_SHIFT) |
                      (QSPI_DC_REG_CKP0_DATA_INACTIVE <<
                       QSPI_DC_REG_CKP0_SHIFT),
    /**< Mode 1 - clock polarity = 0, clock phase = 1*/
    QSPI_CLK_MODE_2 = (QSPI_DC_REG_CKPH0_CKP_1_SHIFT_OUT_RISING_EDGE <<
                       QSPI_DC_REG_CKPH0_SHIFT) |
                      (QSPI_DC_REG_CKP0_DATA_ACTIVE <<
                       QSPI_DC_REG_CKP0_SHIFT),
    /**< Mode 2 - clock polarity = 1, clock phase = 0*/
    QSPI_CLK_MODE_3 = (QSPI_DC_REG_CKPH0_CKP_1_SHIFT_OUT_FALLING_EDGE <<
                       QSPI_DC_REG_CKPH0_SHIFT) |
                      (QSPI_DC_REG_CKP0_DATA_ACTIVE <<
                       QSPI_DC_REG_CKP0_SHIFT),
    /**<Mode 3 - clock polarity = 1, clock phase = 1*/
}qspiClkMode_t;

/**
 *
 *  Enumerates the various transfer commands to be set in the
 *          command register for transfer using configuration registers.
 */
typedef enum qspiCfgModeTxCmd
{
    QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE =
        QSPI_CMD_REG_FOUR_PIN_READ_SINGLE,
    /**< Four Pin single read command .*/
    QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE =
        QSPI_CMD_REG_FOUR_PIN_WRITE_SINGLE,
    /**<Four pin single write command .*/
    QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_DUAL =
        QSPI_CMD_REG_FOUR_PIN_READ_DUAL,
    /**< Four pin dual read command.*/
    QSPI_CFG_MODE_TX_CMD_THREE_PIN_READ_SINGLE =
        QSPI_CMD_REG_THREE_PIN_READ_SINGLE,
    /**< Three pin single read command.*/
    QSPI_CFG_MODE_TX_CMD_THREE_PIN_WRITE_SINGLE =
        QSPI_CMD_REG_THREE_PIN_WRITE_SINGLE,
    /**< Three pin single write command.*/
    QSPI_CFG_MODE_TX_CMD_SIX_PIN_READ_QUAD =
        QSPI_CMD_REG_SIX_PIN_READ_QUAD,
    /**< Six pin quad read command.*/
} qspiCfgModeTxCmd_t;

/**
 *  Enumerates the differenct chip selects available
 *          to QSPI. QSPI module have four chip selects to
 *          connect to four external devices.
 */
typedef enum qspiChipSelect
{
    QSPI_CHIP_SELECT_CS0 = 0x0U,
    /**< Chip select 0 */
    QSPI_CHIP_SELECT_CS1 = 0x1U,
    /**< Chip select 1 */
    QSPI_CHIP_SELECT_CS2 = 0x2U,
    /**< Chip select 2 */
    QSPI_CHIP_SELECT_CS3 = 0x3U,
    /** Chip select 3 */
} qspiChipSelect_t;

/**
 * Enumerates the polarities for different chip select
 *         available. (Chip Select 0 to Chip Select 3)
 */
typedef enum qspiCsPol
{
    QSPI_CS_POL_ACTIVE_LOW = QSPI_DC_REG_CSP0_ACTIVE_LOW,
    /**< Chip select polarity active low */
    QSPI_CS_POL_ACTIVE_HIGH = QSPI_DC_REG_CSP0_ACTIVE_HIGH,
    /**< Chip select polarity active high */
} qspiCsPol_t;

/**
 * Enumerates the value of Data delay value for
 *           each chip select. Data is output on the same cycle as
 *           CS_N goes active, or 1, 2 or 3 DCLK cycles after the
 *           CS_N goes active depending on the data delay provided.
 */
typedef enum qspiDataDelay
{
    QSPI_DATA_DELAY_0 = QSPI_DC_REG_DD0_CS_TO_DATA_DELAY_0,
    /**<Data is output on the same cycle as the CS N goes active. */
    QSPI_DATA_DELAY_1 = QSPI_DC_REG_DD0_CS_TO_DATA_DELAY_1,
    /**<Data is output 1 DCLK cycle after the CS N goes active. */
    QSPI_DATA_DELAY_2 = QSPI_DC_REG_DD0_CS_TO_DATA_DELAY_2,
    /**<Data is output 2 DCLK cycle after the CS N goes active. */
    QSPI_DATA_DELAY_3 = QSPI_DC_REG_DD0_CS_TO_DATA_DELAY_3,
    /**<Data is output 3 DCLK cycle after the CS N goes active. */
} qspiDataDelay_t;

/**
 *  Enumerates the number of Address bytes to be sent before
 *          transfer in memory mapped mode for each chip select
 */
typedef enum qspiMemMapNumAddrBytes
{
    QSPI_MEM_MAP_NUM_ADDR_BYTES_ONE = QSPI_SETUP_REG_NUM_A_BYTES_ONE_BYTE,
    /**< One address byte for Chip Select N*/
    QSPI_MEM_MAP_NUM_ADDR_BYTES_TWO = QSPI_SETUP_REG_NUM_A_BYTES_TWO,
    /**< Two address bytes for chip select N */
    QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE = QSPI_SETUP_REG_NUM_A_BYTES_THREE,
    /**< Three address bytes for chip select N */
    QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR = QSPI_SETUP_REG_NUM_A_BYTES_FOUR,
    /**< Four address bytes for Chip select N */
} qspiMemMapNumAddrBytes_t;

/**
 * This enum Enumerates the different read types for each chip
 *         select for transfer in memory mapped mode.
 */
typedef enum qspiMemMapReadType
{
    QSPI_MEM_MAP_READ_TYPE_NORMAL = QSPI_SETUP_REG_READ_TYPE_NORMAL,
    /**< Normal read type for Chip select 0
     *   Normal means single read mode.
     */
    QSPI_MEM_MAP_READ_TYPE_DUAL = QSPI_SETUP_REG_READ_TYPE_DUAL,
    /**< Dual read type for chip select 0 */
    QSPI_MEM_MAP_READ_TYPE_NORMAL_READTYPE =
        QSPI_SETUP_REG_READ_TYPE_NORMAL_READTYPE,
    /**< Normal read type for Chip select 0 */
    QSPI_MEM_MAP_READ_TYPE_QUAD = QSPI_SETUP_REG_READ_TYPE_QUAD,
    /**< Quad read type for chip select 1 */
} qspiMemMapReadType_t;

/**
 *  This enumerates the values for switch to be set in the
 *           QSPI switch register for selection between config port
 *           and memory mapped port.
 *           If 0 (default) config port has is selected to control
 *           config of core SPI module module.
 *           If 1, Memory Mapped Protocol Translator is selected to
 *           control config port of core SPI module.
 */
typedef enum qspiMemMapPortSel
{
    QSPI_MEM_MAP_PORT_SEL_CFG_PORT = QSPI_SWITCH_REG_MMPT_S_SEL_CFG_PORT,
    /** < config port is selected to control config of core
          SPI module. */
    QSPI_MEM_MAP_PORT_SEL_MEM_MAP_PORT = QSPI_SWITCH_REG_MMPT_S_SEL_MM_PORT,
    /** < Memory Mapped Protocol Translator is selected to
          control config port of core SPI module.*/
} qspiMemMapPortSel_t;

/**
 * This enumerates the flags for word or frame interrupt for QSPI module
 */
typedef enum qspiIntrMask
{
    QSPI_INTR_MASK_FRAME = QSPI_INTR_EN_SET_REG_FIRQ_ENA_MASK,
    /**< Interrupt mask for frame interrupt. */
    QSPI_INTR_MASK_WORD = QSPI_INTR_EN_SET_REG_WIRQ_ENA_MASK
    /**< Interrupt mask for word interrupt . */
} qspiIntrMask_t;

/**
 * This structure defines the bitfields for transfer command to be set for transfers in
 *         configuration mode.
 */
typedef struct qspiTxCmd
{
    uint32_t frameLength : 12U;
    /**< Frame length to be transferred
     * This can take any value from 1 to 4096
     */
    uint32_t res1        :  2U;
    /**< Reserved bits */
    uint32_t wordCntIntrVal : 1U;
    /**< Word count interrupt value,
     *  Values can be TRUE or FALSE
     */
    uint32_t frameCntIntrVal : 1U;
    /**< Frame count interrupt value,
     *  Values can be TRUE or FALSE
     */
    uint32_t txCmd : 3U;
    /**< Transfer command to be set.
     * Values given by enum #qspiCfgModeTxCmd_t
     */
    uint32_t wordLength : 7U;
    /**< wordLength to be transferred.
     * This can take values from 1 to 128
     */
    uint32_t res2 : 2U;
    /**< Reserved bit fields */
    uint32_t chipSelect : 1U;
    /**< Chip select to be used.
     * Values given by enum #qspiChipSelect_t
     */
} qspiTxCmd_t;


void QSPISetIdleMode(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t idleMode);


void QSPISetPreScaler(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t clkDividerVal);

void QSPISetClkMode(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t chipSelect,
                    uint32_t clkMode);

void QSPISetCsPol(VXB_DEV_ID pDev, uint32_t baseAddr,
                  uint32_t chipSelect,
                  uint32_t csPol);

void QSPISetDataDelay(VXB_DEV_ID pDev, uint32_t baseAddr,
                      uint32_t chipSelect,
                      uint32_t dataDelay);

void QSPISetMemMapNumAddrBytes(VXB_DEV_ID pDev, uint32_t baseAddr,
                               uint32_t chipSelect,
                               uint32_t numAddrBytes);

void QSPISetMemMapNumDummyBits(VXB_DEV_ID pDev, uint32_t baseAddr,
                               uint32_t chipSelect,
                               uint32_t numDummyBits);

void QSPISetMemMapReadType(VXB_DEV_ID pDev, uint32_t baseAddr,
                           uint32_t chipSelect,
                           uint32_t readType);

void QSPISetMemMapWriteCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
                           uint32_t chipSelect,
                           uint32_t writeCmd);

void QSPISetMemMapReadCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
                          uint32_t chipSelect,
                          uint32_t readCmd);

void QSPIReadData(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t *pData, uint32_t length);

void QSPIWriteData(VXB_DEV_ID pDev, uint32_t baseAddr, const uint32_t *pData, uint32_t length);

uint32_t QSPIIsBusy(VXB_DEV_ID pDev, uint32_t baseAddr);

uint32_t QSPIGetStatusWordCount(VXB_DEV_ID pDev, uint32_t baseAddr);

void QSPIIntrEnable(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t intrMask);

void QSPIIntrDisable(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t intrMask);

uint32_t QSPIIntrStatus(VXB_DEV_ID pDev, uint32_t baseAddr);

void QSPIIntrClear(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t intrMask);

uint32_t QSPIIntrRawStatus(VXB_DEV_ID pDev, uint32_t baseAddr);

void QSPISetMemAddrSpace(VXB_DEV_ID pDev, uint32_t baseAddr,
                         uint32_t chipSelect,
                         uint32_t memMapPortSwitch);


void QSPISetCfgModeTxCmd(VXB_DEV_ID pDev, uint32_t baseAddr, qspiTxCmd_t qspiTxCmd);

#ifdef __cplusplus
}
#endif

#endif
