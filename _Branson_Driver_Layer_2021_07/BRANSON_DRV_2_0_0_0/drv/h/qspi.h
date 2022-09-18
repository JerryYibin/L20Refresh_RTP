/**
 *  \ingroup CSL_IP_MODULE
 *  \defgroup CSL_QSPI QSPI
 *
 *  @{
 */
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
 *
 * \details    Programming sequence of QSPI is as follows:
 *             -# Enable the functional clock for required QSPI instance.
 *             -# Perform the pin multiplexing for required QSPI instance.
 *             -# Set the QSPI idle mode using the API #QSPIconfigIdleMode.
 *             -# Set Device Control register - clock polarity, phase, CS polarity and data
 *                 delay depending on silicon Revision #QSPIsetDeviceControl
 *             -# sets Memory Map Setup register #QSPISetMemoryMapSetup.
 *             -# selects the memory mapped mode in SPI switch register and
 *                configures mapped address for different chip select in
 *                control module using #QSPISetMAddrSpace
 *             -# Read data from QSPI data registers using the API
 *                #QSPIreadData.
 *             -# Write data into QSPI data registers by using the API
 *                #QSPIwriteData.
 *             -# returns the status of the interrupts #QSPIintStatus
 *
 **/

#ifndef QSPI_H_
#define QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
//#include <ti/csl/cslr_qspi.h>
#include <vxworks.h>
#include <types.h>
#include <hw_am572xqspi.h>

/* ========================================================================== */
/*                                 Macros & Typedefs                          */
/* ========================================================================== */

/* ========================================================================== */
/*                         Deprecated Macros                                  */
/* ========================================================================== */
/* Instead of using these Macros, we can use HW Macros. So these Macros are
 * Deprecated and also Undefined the Macro QSPI_H_BACKWARD_COMPATIBLE_CHG which
 * fixes MISRAC Issue: 67 X: MISRAC_2012_D.4.5: Identifier is typographically
 * ambiguous*/
#ifdef QSPI_H_BACKWARD_COMPATIBLE_CHG
/* \name Values that can be used by the application for configuration via APIs
 *  @{
 */
#define QSPI__SPI_STATUS_REG__BUSY__BUSY    (QSPI_SPI_STATUS_REG_BUSY_BUSY)
#define QSPI__SPI_STATUS_REG__BUSY__IDLE    (QSPI_SPI_STATUS_REG_BUSY_IDLE)

#define QSPI__SPI_CLOCK_CNTRL_REG__CLKEN__DCLOCK_OFF \
		(QSPI_SPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_OFF)
#define QSPI__SPI_CLOCK_CNTRL_REG__CLKEN__DCLOCK_ON \
		(QSPI_SPI_CLOCK_CNTRL_REG_CLKEN_DCLOCK_ON)

#define QSPI__SPI_DC_REG__CKP__DATA_ACTIVE     (QSPI_SPI_DC_REG_CKP_DATA_ACTIVE)
#define QSPI__SPI_DC_REG__CKP__DATA_INACTIVE   \
		(QSPI_SPI_DC_REG_CKP_DATA_INACTIVE)

#define QSPI__SPI_DC_REG__CSP__ACTIVE_LOW      (QSPI_SPI_DC_REG_CSP_ACTIVE_LOW)
#define QSPI__SPI_DC_REG__CSP__ACTIVE_HIGH     (QSPI_SPI_DC_REG_CSP_ACTIVE_HIGH)

#define QSPI__SPI_DC_REG__CKPH__CKP_1_SHIFT_OUT_FALLING_EDGE \
		(QSPI_SPI_DC_REG_CKPH_CKP_1_SHIFT_OUT_FALLING_EDGE)
#define QSPI__SPI_DC_REG__CKPH__CKP_1_SHIFT_OUT_RISING_EDGE \
		(QSPI_SPI_DC_REG_CKPH_CKP_1_SHIFT_OUT_RISING_EDGE)
#define QSPI__SPI_DC_REG__CKPH__CKP_0_SHIFT_OUT_FALLING_EDGE \
		(QSPI_SPI_DC_REG_CKPH_CKP_0_SHIFT_OUT_FALLING_EDGE)
#define QSPI__SPI_DC_REG__CKPH__CKP_0_SHIFT_OUT_RISING_EDGE \
		(QSPI_SPI_DC_REG_CKPH_CKP_0_SHIFT_OUT_RISING_EDGE)

#define QSPI__SPI_DC_REG__DD__CS_TO_DATA_DELAY_0 \
		(QSPI_SPI_DC_REG_DD_CS_TO_DATA_DELAY_0)
#define QSPI__SPI_DC_REG__DD__CS_TO_DATA_DELAY_1 \
		(QSPI_SPI_DC_REG_DD_CS_TO_DATA_DELAY_1)
#define QSPI__SPI_DC_REG__DD__CS_TO_DATA_DELAY_2 \
		(QSPI_SPI_DC_REG_DD_CS_TO_DATA_DELAY_2)
#define QSPI__SPI_DC_REG__DD__CS_TO_DATA_DELAY_3 \
		(QSPI_SPI_DC_REG_DD_CS_TO_DATA_DELAY_3)

#define QSPI__SPI_CMD_REG__CSNUM__CS_0      (QSPI_SPI_CMD_REG_CSNUM_CS_0)
#define QSPI__SPI_CMD_REG__CSNUM__CS_1      (QSPI_SPI_CMD_REG_CSNUM_CS_1)
#define QSPI__SPI_CMD_REG__CSNUM__CS_2      (QSPI_SPI_CMD_REG_CSNUM_CS_2)
#define QSPI__SPI_CMD_REG__CSNUM__CS_3      (QSPI_SPI_CMD_REG_CSNUM_CS_3)

#define QSPI__SPI_CMD_REG__FIRQ__FRAME_COUNT_IRQ_DISABLE \
		(QSPI_SPI_CMD_REG_FIRQ_FRAME_COUNT_IRQ_DISABLE)
#define QSPI__SPI_CMD_REG__FIRQ__FRAME_COUNT_IRQ_ENABLE \
		(QSPI_SPI_CMD_REG_FIRQ_FRAME_COUNT_IRQ_ENABLE)

#define QSPI__SPI_CMD_REG__WIRQ__WORD_COUNT_IRQ_DISABLE \
		(QSPI_SPI_CMD_REG_WIRQ_WORD_COUNT_IRQ_DISABLE)
#define QSPI__SPI_CMD_REG__WIRQ__WORD_COUNT_IRQ_ENABLE \
		(QSPI_SPI_CMD_REG_WIRQ_WORD_COUNT_IRQ_ENABLE)

#define QSPI__SPI_SETUP0_REG__NUM_A_BYTES__THREE_BYTES   \
		(QSPI_SPI_SETUP0_REG_NUM_A_BYTES_THREE_BYTES)
#define QSPI__SPI_SETUP0_REG__NUM_A_BYTES__TWO_BYTES     \
		(QSPI_SPI_SETUP0_REG_NUM_A_BYTES_TWO_BYTES)
#define QSPI__SPI_SETUP0_REG__NUM_A_BYTES__ONE_BYTES     \
		(QSPI_SPI_SETUP0_REG_NUM_A_BYTES_ONE_BYTE)

#define QSPI__SPI_SETUP0_REG__NUM_D_BYTES__USE_8_BITS    \
		(QSPI_SPI_SETUP0_REG_NUM_D_BYTES_USE_8_BITS)
#define QSPI__SPI_SETUP0_REG__NUM_D_BYTES__USE_16_BITS   \
		(QSPI_SPI_SETUP0_REG_NUM_D_BYTES_USE_16_BITS)
#define QSPI__SPI_SETUP0_REG__NUM_D_BYTES__USE_24_BITS   \
		(QSPI_SPI_SETUP0_REG_NUM_D_BYTES_USE_24_BITS)
/* @} */
#endif

/* \brief Values of QSPI flash clock frequency is combination of input clock
 *  select value and Clock divider value (QSPI_CLKSWITCH + QSPI_CLKDIVIDER)*/
/**
 * \brief Mask value to get input clock select value
 *      0 - 128 MHz input clock
 *      1 - 192 MHz input clock
 *      2 - 76.8 MHz input clock
 */
#define QSPI_CLKSWITCH_MASK  (0xF0000U)

/**
 * \brief Shift value to get input clock select value
 */
#define QSPI_CLKSWITCH_SHIFT (16U)

/**
 * \brief Mask value to get input clock divider value to be set in QSPI IP
 *      internal divider
 */
#define QSPI_CLKDIVIDER_MASK  (0x0FFFFU)

/**
 * \brief Shift value to get input clock divider value
 */
#define QSPI_CLKDIVIDER_SHIFT (0U)

/* ========================================================================== */
/*                             Enum Definitions                               */
/* ========================================================================== */
/**
 * \brief Enumerates the values used to represent QSPI device type
 */
typedef enum {
	DEVICE_TYPE_QSPI4 = 0x01U,
	/**< 4 Bit Quad Fast read at 48 MHz.This is used for flash auto detect */
	DEVICE_TYPE_QSPI1 = 0x02U,
	/**< 1 Bit Normal read at 12 MHz. This is used for flash auto detect */
	DEVICE_TYPE_MICRON_QSPI4 = 0x11U,
	/**< 4 bit (Quad) read from Micron Flash*/
	DEVICE_TYPE_MICRON_QSPI1 = 0x12U,
	/**< 1 bit read from Micron Flash*/
	DEVICE_TYPE_MICRON_1GBIT_QSPI4 = 0x13U,
	/**< 4 bit read from 1 GigaBit Micron Flash*/
	DEVICE_TYPE_MICRON_1GBIT_QSPI1 = 0x14U,
	/**< 1 bit read from 1 GigaBit Micron Flash*/
	DEVICE_TYPE_WINBOND_QSPI4 = 0x21U,
	/**< 4 bit (Quad) read from Winbond Flash*/
	DEVICE_TYPE_WINBOND_QSPI1 = 0x22U,
	/**< 1 bit read from Winbond Flash*/
	DEVICE_TYPE_ISSI_QSPI4 = 0x31U,
	/**< 4 bit (Quad) read from ISSI Flash*/
	DEVICE_TYPE_ISSI_QSPI1 = 0x32U,
	/**< 1 bit read from ISSI Flash*/
	DEVICE_TYPE_SPANSION_QSPI4 = 0x41U,
	/**< 4 bit (Quad) read from Spansion Flash*/
	DEVICE_TYPE_SPANSION_QSPI1 = 0x42U,
	/**< 1 bit read from Spansion Flash*/
	DEVICE_TYPE_CUSTOM = 0xF0U
	/**< Custom Flash Mode*/
}qspi_DeviceType_e;

/**
 * \brief Enumerates the values used to represent QSPI flash clock frequency
 */
typedef enum {
	QSPI_SCLK_FREQ_64MHz = 0x00001U,
	/**< Selects QSPI SCLK Freq as 64 MHz clock*/
	QSPI_SCLK_FREQ_32MHz = 0x00003U,
	/**< Selects QSPI SCLK Freq as 32 MHz clock*/
	QSPI_SCLK_FREQ_16MHz = 0x00007U,
	/**< Selects QSPI SCLK Freq as 16 MHz clock*/
	QSPI_SCLK_FREQ_96MHz = 0x10001U,
	/**< Selects QSPI SCLK Freq as 96 MHz clock*/
	QSPI_SCLK_FREQ_48MHz = 0x10003U,
	/**< Selects QSPI SCLK Freq as 48 MHz clock*/
	QSPI_SCLK_FREQ_24MHz = 0x10007U,
	/**< Selects QSPI SCLK Freq as 24 MHz clock*/
	QSPI_SCLK_FREQ_12MHz = 0x1000FU,
	/**< Selects QSPI SCLK Freq as 12 MHz clock*/
	QSPI_SCLK_FREQ_76_8MHz = 0x20000U
	/**< Selects QSPI SCLK Freq as 76.8 MHz clock*/
}qspi_ClockFreq_e;

/**
 * \brief Enumerates the values used to represent Chip select value
 */
typedef enum {
	QSPI_MMR = 0,
	/**< Configuration port registers*/
	QSPI_CS0 = 1,
	/**< select chip select 0*/
	QSPI_CS1 = 2,
	/**< select chip select 1*/
	QSPI_CS2 = 3,
	/**< select chip select 2*/
	QSPI_CS3 = 4
	/**< select chip select 3*/
}qspi_ChipSelect_e;

/**
 * \brief Enumerates the values used to represent QSPI commands
 */
typedef enum {
	QSPI_CMD_READ_24 = 0x03U,
	/**< Read command for 3 or 4 Byte Address*/
	QSPI_CMD_READ = 0x13U,
	/**< Normal Read command for 4 Byte Address*/
	QSPI_CMD_RDSR = 0x05U,
	/**< Command to Read Status Register-1*/
	QSPI_CMD_RDSR2 = 0x07U,
	/**< Command to Read Status Register-2*/
	QSPI_CMD_RDCR = 0x35U,
	/**< Command to Read Configuration Register-1*/
	QSPI_CMD_WREN = 0x06U,
	/**< Write enable command*/
	QSPI_CMD_WRREG = 0x01U,
	/**< Write register command*/
	QSPI_CMD_WRITE_24 = 0x02U,
	/**< Write command for 3 or 4 Byte Address*/
	QSPI_CMD_WRITE = 0x12U,
	/**< Normal write command for 4 Byte Address*/
	QSPI_CMD_FAST_READ_24 = 0x0BU,
	/**< Fast read command for 3 or 4 Byte Address*/
	QSPI_CMD_FAST_READ = 0x0CU,
	/**< Fast read command for 4 Byte Address*/
	QSPI_CMD_FAST_DREAD_24 = 0x3BU,
	/**< Fast dual read command for 3 or 4 Byte Address*/
	QSPI_CMD_FAST_DREAD = 0x3CU,
	/**< Fast dual read command for 4 Byte Address*/
	QSPI_CMD_FAST_QREAD_24 = 0x6BU,
	/**< Fast quad read command for 3 or 4 Byte Address*/
	QSPI_CMD_FAST_QREAD = 0x6CU,
	/**< Fast quad read command for 4 Byte Address*/
	QSPI_CMD_MFGID = 0x90U,
	/**< Read manufacturer Id command*/
	QSPI_CMD_BLKERASE_24 = 0xD8U,
	/**< Block Erase(64 kB or 256 kB) command for 3 or 4 Byte Address*/
	QSPI_CMD_BLKERASE = 0xDCU,
	/**< Block Erase(64 kB or 256 kB) command for 4 Byte Address*/
	QSPI_CMD_PAGEERASE = 0x21U,
	/**< 4 kB Sector Erase command for 4 Byte Address*/
	QSPI_CMD_PAGEERASE_24 = 0x20U,
	/**< 4 kB Sector Erase command for 3 or 4 Byte Address*/
	QSPI_CMD_FULLERASE = 0x60U,
	/**< Full Flash Erase command*/

	/* Spansion flash commands */
	QSPI_CMD_SPANSION_RESET = 0xF0U,
	/**< Spansion Flash: Software Reset command*/

	/* Micron flash commands */
	QSPI_CMD_MICRON_READ_24 = 0x03U,
	/**< Micron Flash: Read command for 3 or 4 Byte Address*/
	QSPI_CMD_MICRON_RDSR = 0x05U,
	/**< Micron Flash: Command to Read Status Register-1*/
	QSPI_CMD_MICRON_QREAD_24 = 0x6BU,
	/**< Micron Flash: Fast quad read command for 3 or 4 Byte Address*/
	QSPI_CMD_MICRON_WRITE_24 = 0x02U,
	/**< Micron Flash: Write command for 4 Byte Address*/
	QSPI_CMD_MICRON_MFGID = 0x9EU,
	/**< Micron Flash: Read manufacturer Id command*/
	QSPI_CMD_MICRON_WREN = 0x06U,
	/**< Micron Flash: Write enable command*/
	QSPI_CMD_MICRON_BLKERASE_24 = 0xD8U,
	/**< Micron Flash: Block Erase(64 kB or 256 kB) command for 3 or 4 Byte
	 *   Address*/
	QSPI_CMD_MICRON_FULLERASE = 0xC7U,
	/**< Micron Flash: Full Flash Erase command*/
	QSPI_CMD_MICRON_RDNVCR = 0xB5U,
	/**< Micron Flash: Command to Read Non Volatile Configuration Register*/
	QSPI_CMD_MICRON_WRNVCR = 0xB1U,
	/**< Micron Flash: Command to Write Non Volatile Configuration Register*/
	QSPI_CMD_MICRON_RDEVCR = 0x65U,
	/**< Micron Flash: Command to Read Enhanced Volatile Configuration Register
	 */
	QSPI_CMD_MICRON_WREVCR = 0x61U,
	/**< Micron Flash: Command to Write Enhanced Volatile Configuration Register
	 */
	/* Commands specific to micron 1G bit flash */
	QSPI_CMD_MICRON_1GBIT_FULLERASE = 0xC4U,
	/**< Micron 1G Bit Flash: Full Flash Erase command*/

	/* Winbond specific commands */
	QSPI_CMD_WINBOND_RDSR2 = 0x35U,
	/**< Winbond Flash: Command to Read Status Register-2*/
	QSPI_CMD_WINBOND_WRSR2 = 0x31U,
	/**< Winbond Flash: Command to Write Status Register-2*/

	/* Reset Commands common for Micron, Winbond and ISSI Flashes */
	QSPI_CMD_RESET_ENABLE = 0x66U,
	/**< Reset Enable command*/
	QSPI_CMD_RESET_DEVICE = 0x99U,
	/**< Reset Memory command*/

	QSPI_CMD_JDEC_DEVICE_ID = 0x9FU
	/**< Command to detect flash Device Id*/
}qspi_ReadCommand_e;

/**
 * \brief  This enumerates the flags for
 *         word or frame interrupt for QSPI module
 */
typedef enum qspiIntrMask
{
	QSPI_INTR_MASK_FRAME = QSPI_INTR_ENABLE_SET_REG_FIRQ_ENA_SET_MASK,
	/**< Interrupt mask for frame interrupt. */
	QSPI_INTR_MASK_WORD = QSPI_INTR_ENABLE_SET_REG_WIRQ_ENA_SET_MASK
	/**< Interrupt mask for word interrupt . */
}qspiIntrMask_t;

/**
 * \brief  Enumerates the four possible clock modes for QSPI.
 *         Each mode has different values of clock phase(CKPH)
 *         and clock polarity(CKP)
 */
typedef enum qspiClkMode
{
	QSPI_CLK_MODE_0 = (QSPI_SPI_DC_REG_CKPH0_CKP_0_SHIFT_OUT_FALLING_EDGE
			<< QSPI_SPI_DC_REG_CKPH0_SHIFT) |
			(QSPI_SPI_DC_REG_CKP0_DATA_INACTIVE <<
					QSPI_SPI_DC_REG_CKP0_SHIFT),
					/**< Mode 0 - Clock polarity = 0, clock phase = 0*/
					QSPI_CLK_MODE_1 = (QSPI_SPI_DC_REG_CKPH0_CKP_0_SHIFT_OUT_RISING_EDGE
							<< QSPI_SPI_DC_REG_CKPH0_SHIFT) |
							(QSPI_SPI_DC_REG_CKP0_DATA_INACTIVE <<
									QSPI_SPI_DC_REG_CKP0_SHIFT),
									/**< Mode 1 - clock polarity = 0, clock phase = 1*/
									QSPI_CLK_MODE_2 = (QSPI_SPI_DC_REG_CKPH0_CKP_1_SHIFT_OUT_RISING_EDGE
											<< QSPI_SPI_DC_REG_CKPH0_SHIFT) |
											(QSPI_SPI_DC_REG_CKP0_DATA_ACTIVE <<
													QSPI_SPI_DC_REG_CKP0_SHIFT),
													/**< Mode 2 - clock polarity = 1, clock phase = 0*/
													QSPI_CLK_MODE_3 = (QSPI_SPI_DC_REG_CKPH0_CKP_1_SHIFT_OUT_FALLING_EDGE
															<< QSPI_SPI_DC_REG_CKPH0_SHIFT) |
															(QSPI_SPI_DC_REG_CKP0_DATA_ACTIVE <<
																	QSPI_SPI_DC_REG_CKP0_SHIFT)
																	/**<Mode 3 - clock polarity = 1, clock phase = 1*/
}qspiClkMode_t;

/**
 * \brief   Enumerates the different chip selects available
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
	QSPI_CHIP_SELECT_CS3 = 0x3U
	/** Chip select 3 */
}qspiChipSelect_t;

/**
 * /brief  Enumerates the polarities for different chip select
 *         available. (Chip Select 0 to Chip Select 3)
 */
typedef enum qspiCsPol
{
	QSPI_CS_POL_ACTIVE_LOW = QSPI_SPI_DC_REG_CSP0_ACTIVE_LOW,
	/**< Chip select polarity active low */
	QSPI_CS_POL_ACTIVE_HIGH = QSPI_SPI_DC_REG_CSP0_ACTIVE_HIGH
	/**< Chip select polarity active high */
}qspiCsPol_t;

/**
 *  \brief   Enumerates the value of Data delay value for
 *           each chip select. Data is output on the same cycle as
 *           CS_N goes active, or 1, 2 or 3 DCLK cycles after the
 *           CS_N goes active depending on the data delay provided.
 */
typedef enum qspiDataDelay
{
	QSPI_DATA_DELAY_0 = QSPI_SPI_DC_REG_DD0_CS_TO_DATA_DELAY_0,
	/**<Data is output on the same cycle as the CS N goes active. */
	QSPI_DATA_DELAY_1 = QSPI_SPI_DC_REG_DD0_CS_TO_DATA_DELAY_1,
	/**<Data is output 1 DCLK cycle after the CS N goes active. */
	QSPI_DATA_DELAY_2 = QSPI_SPI_DC_REG_DD0_CS_TO_DATA_DELAY_2,
	/**<Data is output 2 DCLK cycle after the CS N goes active. */
	QSPI_DATA_DELAY_3 = QSPI_SPI_DC_REG_DD0_CS_TO_DATA_DELAY_3
	/**<Data is output 3 DCLK cycle after the CS N goes active. */
}qspiDataDelay_t;

/**
 *  \brief  Enumerates the number of Address bytes to be sent before
 *          transfer in memory mapped mode for each chip select
 */
typedef enum qspiMemMapNumAddrBytes
{
	QSPI_MEM_MAP_NUM_ADDR_BYTES_ONE =
			QSPI_SPI_SETUP0_REG_NUM_A_BYTES_ONE_BYTE,
			/**< One address byte for Chip Select N*/
			QSPI_MEM_MAP_NUM_ADDR_BYTES_TWO =
					QSPI_SPI_SETUP0_REG_NUM_A_BYTES_TWO_BYTES,
					/**< Two address bytes for chip select N */
					QSPI_MEM_MAP_NUM_ADDR_BYTES_THREE =
							QSPI_SPI_SETUP0_REG_NUM_A_BYTES_THREE_BYTES,
							/**< Three address bytes for chip select N */
							QSPI_MEM_MAP_NUM_ADDR_BYTES_FOUR =
									QSPI_SPI_SETUP0_REG_NUM_A_BYTES_FOUR_BYTES
									/**< Four address bytes for Chip select N */
}qspiMemMapNumAddrBytes_t;

/**
 *  \brief   This enumerates the values for switch to be set in the
 *           QSPI switch register for selection between config port
 *           and memory mapped port.
 *           If 0 (default) config port has is selected to control
 *           config of core SPI module module.
 *           If 1, Memory Mapped Protocol Translator is selected to
 *           control config port of core SPI module.
 */
typedef enum qspiMemMapPortSel
{
	QSPI_MEM_MAP_PORT_SEL_CFG_PORT =
			QSPI_SPI_SWITCH_REG_MMPT_S_SEL_CFG_PORT,
			/** < config port is selected to control config of core
	 SPI module. */
			QSPI_MEM_MAP_PORT_SEL_MEM_MAP_PORT =
					QSPI_SPI_SWITCH_REG_MMPT_S_SEL_MM_PORT
					/** < Memory Mapped Protocol Translator is selected to
	 control config port of core SPI module.*/
}qspiMemMapPortSel_t;

/**
 * \brief  This enum Enumerates the different read types for each chip
 *         select for transfer in memory mapped mode.
 */
typedef enum qspiMemMapReadType
{
	QSPI_MEM_MAP_READ_TYPE_NORMAL =
			QSPI_SPI_SETUP0_REG_READ_TYPE_NORMAL_READ,
			/**< Normal read type for Chip select 0
			 *   Normal means single read mode.
			 */
			QSPI_MEM_MAP_READ_TYPE_DUAL =
					QSPI_SPI_SETUP0_REG_READ_TYPE_DUAL_READ,
					/**< Dual read type for chip select 0 */
					QSPI_MEM_MAP_READ_TYPE_NORMAL_READTYPE =
							QSPI_SPI_SETUP0_REG_READ_TYPE_NORMAL_READ_TYPE,
							/**< Normal read type for Chip select 0 */
							QSPI_MEM_MAP_READ_TYPE_QUAD =
									QSPI_SPI_SETUP0_REG_READ_TYPE_QUAD_READ
									/**< Quad read type for chip select 1 */
}qspiMemMapReadType_t;

/**
 *
 * \brief   Enumerates the various transfer commands to be set in the
 *          command register for transfer using configuration registers.
 */
typedef enum qspiCfgModeTxCmd
{
	QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_SINGLE =
			QSPI_SPI_CMD_REG_CMD_FOUR_PIN_READ_SINGLE,
			/**< Four Pin single read command .*/
			QSPI_CFG_MODE_TX_CMD_FOUR_PIN_WRITE_SINGLE =
					QSPI_SPI_CMD_REG_CMD_FOUR_PIN_WRITE_SINGLE,
					/**<Four pin single write command .*/
					QSPI_CFG_MODE_TX_CMD_FOUR_PIN_READ_DUAL =
							QSPI_SPI_CMD_REG_CMD_FOUR_PIN_READ_DUAL,
							/**< Four pin dual read command.*/
							QSPI_CFG_MODE_TX_CMD_THREE_PIN_READ_SINGLE =
									QSPI_SPI_CMD_REG_CMD_THREE_PIN_READ_SINGLE,
									/**< Three pin single read command.*/
									QSPI_CFG_MODE_TX_CMD_THREE_PIN_WRITE_SINGLE =
											QSPI_SPI_CMD_REG_CMD_THREE_PIN_WRITE_SINGLE,
											/**< Three pin single write command.*/
											QSPI_CFG_MODE_TX_CMD_SIX_PIN_READ_QUAD =
													QSPI_SPI_CMD_REG_CMD_SIX_PIN_READ_QUAD
													/**< Six pin quad read command.*/
}qspiCfgModeTxCmd_t;
/* ========================================================================== */
/*                             Structure Definitions                          */
/* ========================================================================== */
/* MISRAC doesn't allow Unions so Undefined the Macro
 * QSPI_H_BACKWARD_COMPATIBLE_CHG which fixes MISRAC Issue: 74 S:
 * MISRAC_2012_R.19.2: Union declared" */
#ifdef QSPI_H_BACKWARD_COMPATIBLE_CHG
/*
 *  \brief Register bitfield definition for Command register.
 *
 */
typedef struct
{
	uint32_t frameLength : 12;
	/**< Frame Length. */
	uint32_t res1 : 2;
	uint32_t wirq : 1;
	/**< Word Completion Interrupt. */
	uint32_t firq : 1;
	/**< Frame Completion Interrupt. */
	uint32_t trCmd : 3;
	/**< Transfer command. */
	uint32_t wordLength : 7;
	/**< Word Length. */
	uint32_t res2 : 2;
	uint32_t cs : 2;
	/**< chip Select. */
}HAL_QSPI_Command_t;

/*
 * \brief Union to access Command register value and bit fields
 *
 */
typedef union
{
	uint32_t reg;
	/**< 32 bit command register. */
	HAL_QSPI_Command_t regFields;
	/**< structure to access register bit fields. */
}HAL_QSPI_Command_reg;

/*
 * \brief Register bitfield definition for Device Control register.
 *
 */
typedef struct
{
	uint32_t CKP0 : 1;
	/**<  Clock polarity for chip select 0. */
	uint32_t CSP0 : 1;
	/**<  Chip select polarity for chip select 0. */
	uint32_t CKPH0 : 1;
	/**<  Clock phase for chip select 0. */
	uint32_t DD0 : 2;
	/**<  Data Delay For chip select 0. */
	uint32_t res0 : 3;
	/**<  Reserved bits. */
	uint32_t CKP1 : 1;
	/**<  Clock polarity for chip select 1. */
	uint32_t CSP1 : 1;
	/**<  Chip select polarity for chip select 1. */
	uint32_t CKPH1 : 1;
	/**<  Clock phase for chip select 1. */
	uint32_t DD1 : 2;
	/**<  Data Delay For chip select 1. */
	uint32_t res1 : 3;
	/**<  Reserved bits. */
	uint32_t CKP2 : 1;
	/**<  Clock polarity for chip select 2. */
	uint32_t CSP2 : 1;
	/**<  Chip select polarity for chip select 2. */
	uint32_t CKPH2 : 1;
	/**<  Clock phase for chip select 2. */
	uint32_t DD2 : 2;
	/**<  Data Delay For chip select 2. */
	uint32_t res2 : 3;
	/**<  Reserved bits. */
	uint32_t CKP3 : 1;
	/**<  Clock polarity for chip select 3. */
	uint32_t CSP3 : 1;
	/**<  Chip select polarity for chip select 3. */
	uint32_t CKPH3 : 1;
	/**<  Clock phase for chip select 3. */
	uint32_t DD3 : 2;
	/**<  Data Delay For chip select 3. */
	uint32_t res3 : 3;
	/**<  Reserved bits. */
}HAL_QSPI_DeviceControl_t;

/*
 * \brief Union to access Device Control register value and bit fields
 *
 */
typedef union
{
	uint32_t reg;
	/**<  32 bit Device Control register. */
	HAL_QSPI_DeviceControl_t regFields;
	/**<  structure to access register bit fields. */
}HAL_QSPI_DeviceControl_reg;

/*
 * \brief Register bitfield definition for Memory Map Setup register.
 *
 */
typedef struct
{
	uint32_t readCmd : 8;
	/**<  Read command. */
	uint32_t numAddBytes : 2;
	/**<  Number of Address Bytes (0 = 1 Byte; 1 = 2 Bytes; 2 = 3 Bytes; 3 = 4
	 *Bytes). */
	uint32_t numDummyBytes : 2;
	/**<  Number of Dummy bytes for Fast Read ( 1 = use 8 Bits; 2 = Use 16 Bits;
	 *3 = Use 24 Bits). */
	uint32_t readType : 2;
	/**<  Read Type Single Dual or Quad Read Mode. */
	uint32_t res1 : 2;
	uint32_t writeCmd : 8;
	/**<  Write command. */
	uint32_t numDummyBits : 6;
	/**<  Number of Dummy bits to use if Dummy Bytes is 0. */
	uint32_t res2 : 2;
}HAL_QSPI_MemMapSetup_t;

/*
 * \brief Union to access Memory Map Setup register value and bit fields
 *
 */
typedef union
{
	uint32_t reg;
	/**<  32 bit Memory Map Setup register. */
	HAL_QSPI_MemMapSetup_t regFields;
	/**<  structure to access register bit fields. */
}HAL_QSPI_MemMapSetup_reg;
#endif
#ifdef __cplusplus
}
#endif

#endif
/** @} */
/********************************* End of file ******************************/

