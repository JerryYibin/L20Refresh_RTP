/**
 * QSPI Device Structure.
 */
#include <vxbQspiInterface.h>
#include <qspi.h>

typedef struct ti_am572x_spi_drv_ctrl {
	VXB_DEV_ID pDev;
	void * regBase;
	void * regHandle;
	void * regFlashBase;
	void * regFlashHandle;
	void * regMMControlIOBase;
	void * regMMControlIOHandle;
	UINT32 clkFrequency;
	UINT8 * txBuf;
	UINT32 txLen;
	UINT32 txLeft;
	UINT8 * rxBuf;
	UINT32 rxLen;
	UINT32 rxLeft;
	UINT32 cmd;
	UINT32 dc;
	UINT32 curWorkingFrq;
	SEM_ID semSync;
	VXB_RESOURCE * intRes;
	BOOL mmap_enabled;
	SPI_Handle handle;
} TI_AM572X_SPI_CTRL;

typedef enum QSPI_intrPollMode_s {
	SPI_OPER_MODE_BLOCKING = 0U, /*! Interrupt based blocking mode */
	SPI_OPER_MODE_POLLING, /*! Non interrupt based blocking mode */
	SPI_OPER_MODE_CALLBACK /*! Interrupt based call back mode */
} QSPI_intrPollMode;


void vxbQspiConfigIdleMode(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t idleMode);


void vxbQspiSetClockControl(VXB_DEV_ID pDev, uint32_t baseAdd,
		qspi_ClockFreq_e in_ClockSettings);


void vxbQspiSetDeviceControl(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t value);


uint32_t vxbQspiGetDeviceControl(VXB_DEV_ID pDev, uint32_t baseAdd);


void vxbQspiSetCommandReg(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t cmd);


uint32_t vxbQspiGetCommandReg(VXB_DEV_ID pDev, uint32_t baseAdd);


uint32_t vxbQspiGetStatus(VXB_DEV_ID pDev, uint32_t baseAdd);


uint32_t vxbQspiGetStatusWordCount(VXB_DEV_ID pDev, uint32_t baseAdd);

void vxbQspiWaitIdle(VXB_DEV_ID pDev, uint32_t baseAdd);

void vxbQspiWriteData(VXB_DEV_ID pDev, uint32_t baseAdd, const uint32_t *data,
		int32_t length);

void vxbQSPIReadData(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t *data,
		int32_t length);

void vxbQspiIntDisable(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t intFlag);

void vxbQspiIntClear(VXB_DEV_ID pDev, uint32_t baseAdd, uint32_t intFlag);


void vxbQSPISetClkMode(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t chipSelect,
		uint32_t clkMode);


void vxbQSPISetCsPol(VXB_DEV_ID pDev, uint32_t baseAddr, uint32_t chipSelect,
		uint32_t csPol);


void vxbQSPISetDataDelay(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t dataDelay);


void vxbQSPISetPreScaler(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t clkDividerVal);


void vxbQSPISetMemMapNumAddrBytes(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t numAddrBytes);


void vxbQSPISetMemAddrSpace(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t memMappedPortSwitch);


void vxbQSPISetMemMapReadCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t readCmd);



void vxbQSPISetMemMapReadType(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t readType);


void vxbQSPISetMemMapNumDummyBits(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t numDummyBits);


void vxbQSPISetMemMapWriteCmd(VXB_DEV_ID pDev, uint32_t baseAddr,
		uint32_t chipSelect, uint32_t writeCmd);

#define QSPI_SPI_SETUP_REG(n)   (QSPI_SPI_SETUP0_REG + ((n) * 0x4U))

#define MEM_BARRIER_DISABLE

/**
 *  Macro to extract Register Base Address. This macro extracts the base address
 *         from a driver id (which contains the driver structure ,which further contains the register base).
 *
 *  driver id (VXB_DEV_ID)
 */
#define MC_SPI_BAR(p)       (((TI_AM572X_SPI_CTRL *) vxbDevSoftcGet(p))->regBase)
#define MMAP_SPI_BAR(p)       (((TI_AM572X_SPI_CTRL *) vxbDevSoftcGet(p))->regFlashBase)
#define CONTROLIO_SPI_BAR(p)       (((TI_AM572X_SPI_CTRL *) vxbDevSoftcGet(p))->regMMControlIOBase)

/**
 *  Macro to extract Register Handle. This macro extracts the register handle
 *         from a driver id (which contains the driver structure ,which further contains the register handle).
 *
 * p         driver id (VXB_DEV_ID)
 */
#define MC_SPI_HANDLE(p)    (((TI_AM572X_SPI_CTRL *) vxbDevSoftcGet(p))->regHandle)
#define MMAP_SPI_HANDLE(p)    (((TI_AM572X_SPI_CTRL *) vxbDevSoftcGet(p))->regFlashHandle)
#define CONTROLIO_SPI_HANDLE(p)    (((TI_AM572X_SPI_CTRL *) vxbDevSoftcGet(p))->regMMControlIOHandle)

/**
 * Macro to extract a field value. This macro extracts the field value
 *         from a 32-bit variable (which contains the register value).
 *         This macro does not read from actual register address, and only
 *         extracts the field from a variable.
 *
 * regVal         32-bit variable containing the register value.
 * REG_FIELD      Peripheral register bit field name, whose value has to
 *                        be extracted.
 */
#define CS_GET_FIELD(regVal, REG_FIELD)                                        \
		(((regVal) & (uint32_t) REG_FIELD##_MASK) >> (uint32_t) REG_FIELD##_SHIFT)

/**
 * Macro to write a specific field value. This macro first clears the
 *         specified field value and then performs "OR" of the field value which
 *         is shifted and masked. This will set the field value at its
 *         desired position.
 *
 * regVal         32-bit variable containing the register value.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_SET_FIELD(regVal, REG_FIELD, fieldVal)                              \
		((regVal) = ((regVal) & (uint32_t) (~(uint32_t) REG_FIELD##_MASK)) |       \
				((((uint32_t) fieldVal) << (uint32_t) REG_FIELD##_SHIFT) & \
						(uint32_t) REG_FIELD##_MASK))

/**
 * This macro calls read-modify-write API for 32 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_WRITE_FIELD32(pDev, regAddr, REG_FIELD, fieldVal)                            \
		(CS_WRITE_FIELD32_RAW(pDev, regAddr, (uint32_t) REG_FIELD##_MASK,                   \
				(uint32_t) REG_FIELD##_SHIFT, (uint32_t) fieldVal))

/**
 * This macro calls read-modify-write API for 16 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_WRITE_FIELD16(pDev, regAddr, REG_FIELD, fieldVal)                            \
		(CS_WRITE_FIELD16_RAW(pDev, regAddr, (uint16_t) REG_FIELD##_MASK,                   \
				(uint32_t) REG_FIELD##_SHIFT, (uint16_t) fieldVal))

/**
 * This macro calls read-modify-write API for 8 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, to which specified
 *                        value has to be set.
 * fieldVal       Value of the field which has to be set.
 */
#define CS_WRITE_FIELD8(pDev, regAddr, REG_FIELD, fieldVal)                             \
		(CS_WRITE_FIELD8_RAW(pDev, regAddr, (uint8_t) REG_FIELD##_MASK,                     \
				(uint32_t) REG_FIELD##_SHIFT, (uint8_t) fieldVal))

/**
 * This macro calls read field API for 32 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, from which specified
 *                        bit-field value has to be read.
 *  retval Value of the bit-field
 */
#define CS_READ_FIELD32(pDev, regAddr, REG_FIELD)                                      \
		(CS_READ_FIELD32_RAW(pDev, regAddr, (uint32_t) REG_FIELD##_MASK,                   \
				(uint32_t) REG_FIELD##_SHIFT))

/**
 *  This macro calls read field API for 16 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 * retval Value of the bit-field
 */
#define CS_READ_FIELD16(pDev, regAddr, REG_FIELD)                                      \
		(CS_READ_FIELD16_RAW(pDev, regAddr, (uint16_t) REG_FIELD##_MASK,                   \
				(uint32_t) REG_FIELD##_SHIFT))

/**
 * This macro calls read field API for 8 bit register. It also
 *         frames the mask and shift from register field macro.
 *
 * regAddr        Register Address.
 * REG_FIELD      Peripheral register bit field name, from which
 *                        specified bit-field value has to be read.
 * retval Value of the bit-field
 */
#define CS_READ_FIELD8(pDev, regAddr, REG_FIELD)                                       \
		(CS_READ_FIELD8_RAW(pDev, regAddr, (uint8_t) REG_FIELD##_MASK,                     \
				(uint32_t) REG_FIELD##_SHIFT))

/**
 * This function reads a 32-bit value from a hardware register
 *           and returns the value.
 *
 * addr    Address of the memory mapped hardware register.
 *
 * Unsigned 32-bit value read from a register.
 */
static inline uint32_t CS_READ_REG32(VXB_DEV_ID pDev, uint32_t addr);

/**
 * This function writes a 32-bit value to a hardware register.
 *
 * addr    Address of the memory mapped hardware register.
 * value   unsigned 32-bit value which has to be written to the
 *                   register.
 */
static inline void CS_WRITE_REG32(VXB_DEV_ID pDev, uint32_t addr,
		uint32_t value);

/**
 * This function reads a 16-bit value from a hardware register
 *           and returns the value.
 *
 * addr    Address of the memory mapped hardware register.
 *
 * Unsigned 16-bit value read from a register.
 */
static inline uint16_t CS_READ_REG16(VXB_DEV_ID pDev, uint32_t addr);

/**
 * This function writes a 16-bit value to a hardware register.
 *
 * addr    Address of the memory mapped hardware register.
 * value   unsigned 16-bit value which has to be written to the
 *                   register.
 */
static inline void CS_WRITE_REG16(VXB_DEV_ID pDev, uint32_t addr,
		uint16_t value);

/**
 * This function reads a 8-bit value from a hardware register
 *           and returns the value.
 *
 * addr    Address of the memory mapped hardware register.
 *
 * Unsigned 8-bit value read from a register.
 */
static inline uint8_t CS_READ_REG8(VXB_DEV_ID pDev, uint32_t addr);

/**
 * This function writes a 8-bit value to a hardware
 *           register.
 *
 * addr    Address of the memory mapped hardware register.
 * value   unsigned 8-bit value which has to be written to the
 *                   register.
 */
static inline void CS_WRITE_REG8(VXB_DEV_ID pDev, uint32_t addr, uint8_t value);

/**
 * This function reads a 32 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD32_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint32_t mask, uint32_t shift, uint32_t value);

/**
 * This function reads a 16 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD16_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint16_t mask, uint32_t shift, uint16_t value);

/**
 * This function reads a 8 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD8_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint8_t mask, uint32_t shift, uint8_t value);

/**
 * This function reads a 32 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 *
 * retval  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint32_t CS_READ_FIELD32_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint32_t mask, uint32_t shift);

/**
 * This function reads a 16 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 *
 * retval  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint16_t CS_READ_FIELD16_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint16_t mask, uint32_t shift);

/**
 * This function reads a 8 bit register, masks specific set of bits
 *           and the left shifted value.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 *
 * retval  Bit-field value (absolute value - shifted to LSB position)
 */
static inline uint8_t CS_READ_FIELD8_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint8_t mask, uint32_t shift);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */
#define QSPI_CORE_START					0x4b300000U
#define QSPI_CORE_END 					0x4B3FFFFFU
#define QSPI_FLASH_MMAP_START 			0x5c000000U
#define QSPI_FLASH_MMAP_END 			0x5FFFFFFF
#define   CTRL_CORE_CONTROL_IO_2       (0x4A002558U)
#define   QSPI_MMAP_CS_SHIFT           (0x8U)

static inline uint32_t CS_READ_REG32(VXB_DEV_ID pDev, uint32_t addr) {
	uint32_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead32(MC_SPI_HANDLE(pDev),
						(UINT32 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead32(MMAP_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead32(CONTROLIO_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) CONTROLIO_SPI_BAR(pDev)));
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return (regVal);
}

static inline void CS_WRITE_REG32(VXB_DEV_ID pDev, uint32_t addr,
		uint32_t value) {
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite32(MC_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)),
				value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite32(MMAP_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)), value);
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		vxbWrite32(CONTROLIO_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) CONTROLIO_SPI_BAR(pDev)), value);
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return;
}

static inline uint16_t CS_READ_REG16(VXB_DEV_ID pDev, uint32_t addr) {
	uint16_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead16(MC_SPI_HANDLE(pDev),
						(UINT16 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead16(MMAP_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead16(CONTROLIO_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) CONTROLIO_SPI_BAR(pDev)));
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return (regVal);
}

static inline void CS_WRITE_REG16(VXB_DEV_ID pDev, uint32_t addr,
		uint16_t value) {
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite16(MC_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)),
				value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite16(MMAP_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)), value);
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		vxbWrite16(CONTROLIO_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) CONTROLIO_SPI_BAR(pDev)), value);
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return;
}

static inline uint8_t CS_READ_REG8(VXB_DEV_ID pDev, uint32_t addr) {
	uint8_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead8(MC_SPI_HANDLE(pDev),
						(UINT8 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead8(MMAP_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead8(CONTROLIO_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) CONTROLIO_SPI_BAR(pDev)));
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return (regVal);
}

static inline void CS_WRITE_REG8(VXB_DEV_ID pDev, uint32_t addr,
		uint8_t value) {
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite8(MC_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)),
				value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite8(MMAP_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)), value);
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		vxbWrite8(CONTROLIO_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) CONTROLIO_SPI_BAR(pDev)), value);
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return;
}

static inline void CS_WRITE_FIELD32_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint32_t mask, uint32_t shift, uint32_t value) {
	uint32_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead32(MC_SPI_HANDLE(pDev),
						(UINT32 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead32(MMAP_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead32(CONTROLIO_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) CONTROLIO_SPI_BAR(pDev)));

	regVal &= (~mask);
	regVal |= (value << shift) & mask;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite32(MC_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)),
				regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite32(MMAP_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)), regVal);
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		vxbWrite32(CONTROLIO_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) CONTROLIO_SPI_BAR(pDev)), regVal);

#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return;
}

static inline void CS_WRITE_FIELD16_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint16_t mask, uint32_t shift, uint16_t value) {
	uint16_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead16(MC_SPI_HANDLE(pDev),
						(UINT16 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead16(MMAP_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead16(CONTROLIO_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) CONTROLIO_SPI_BAR(pDev)));

	regVal &= (~mask);
	regVal |= (value << shift) & mask;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite16(MC_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)),
				regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite16(MMAP_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)), regVal);
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		vxbWrite16(CONTROLIO_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) CONTROLIO_SPI_BAR(pDev)), regVal);

#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return;
}

static inline void CS_WRITE_FIELD8_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint8_t mask, uint32_t shift, uint8_t value) {
	uint8_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead8(MC_SPI_HANDLE(pDev),
						(UINT8 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead8(MMAP_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead8(CONTROLIO_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) CONTROLIO_SPI_BAR(pDev)));

	regVal &= (~mask);
	regVal |= (value << shift) & mask;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite8(MC_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)),
				regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite8(MMAP_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)), regVal);
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		vxbWrite8(CONTROLIO_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) CONTROLIO_SPI_BAR(pDev)), regVal);

#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return;
}

static inline uint32_t CS_READ_FIELD32_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint32_t mask, uint32_t shift) {
	uint32_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead32(MC_SPI_HANDLE(pDev),
						(UINT32 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead32(MMAP_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead32(CONTROLIO_SPI_HANDLE(pDev),
				(UINT32 *) ((char *) CONTROLIO_SPI_BAR(pDev)));

	regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return (regVal);
}

static inline uint16_t CS_READ_FIELD16_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint16_t mask, uint32_t shift) {
	uint16_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead16(MC_SPI_HANDLE(pDev),
						(UINT16 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead16(MMAP_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead16(CONTROLIO_SPI_HANDLE(pDev),
				(UINT16 *) ((char *) CONTROLIO_SPI_BAR(pDev)));

	regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return (regVal);
}

static inline uint8_t CS_READ_FIELD8_RAW(VXB_DEV_ID pDev, uint32_t addr,
		uint8_t mask, uint32_t shift) {
	uint8_t regVal = 0;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		regVal =
				vxbRead8(MC_SPI_HANDLE(pDev),
						(UINT8 *) ((char *) MC_SPI_BAR(pDev)
								+ (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		regVal = vxbRead8(MMAP_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) MMAP_SPI_BAR(pDev)
						+ (addr - QSPI_FLASH_MMAP_START)));
	else if (addr == CTRL_CORE_CONTROL_IO_2)
		regVal = vxbRead8(CONTROLIO_SPI_HANDLE(pDev),
				(UINT8 *) ((char *) CONTROLIO_SPI_BAR(pDev)));

	regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
	asm("    dsb");
#endif
	return (regVal);
}

