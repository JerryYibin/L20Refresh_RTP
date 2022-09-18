
#ifndef CS_TYPES_H_
#define CS_TYPES_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <vxWorks.h>
#include <vxbTiAm437xQspi.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define MEM_BARRIER_DISABLE

/**
 *  Macro to extract Register Base Address. This macro extracts the base address
 *         from a driver id (which contains the driver structure ,which further contains the register base).
 *
 *  driver id (VXB_DEV_ID)
 */
#define MC_SPI_BAR(p)       (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regBase)
#define MMAP_SPI_BAR(p)       (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regFlashBase)

/**
 *  Macro to extract Register Handle. This macro extracts the register handle
 *         from a driver id (which contains the driver structure ,which further contains the register handle).
 *
 * p         driver id (VXB_DEV_ID)
 */
#define MC_SPI_HANDLE(p)    (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regHandle)
#define MMAP_SPI_HANDLE(p)    (((TI_AM437X_SPI_CTRL *) vxbDevSoftcGet(p))->regFlashHandle)

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
static inline void CS_WRITE_REG32(VXB_DEV_ID pDev, uint32_t addr, uint32_t value);

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
static inline void CS_WRITE_REG16(VXB_DEV_ID pDev, uint32_t addr, uint16_t value);

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
static inline void CS_WRITE_FIELD32_RAW(VXB_DEV_ID pDev,
									 uint32_t addr,
                                     uint32_t mask,
                                     uint32_t shift,
                                     uint32_t value);

/**
 * This function reads a 16 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 uint32_t addr,
                                     uint16_t mask,
                                     uint32_t shift,
                                     uint16_t value);

/**
 * This function reads a 8 bit register, modifies specific set of
 *           bits and writes back to the register.
 *
 * addr    Address of the memory mapped hardware register.
 * mask    Mask for the bit field.
 * shift   Bit field shift from LSB.
 * value   Value to be written to bit-field.
 */
static inline void CS_WRITE_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	uint32_t addr,
                                    uint8_t mask,
                                    uint32_t shift,
                                    uint8_t value);

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
static inline uint32_t CS_READ_FIELD32_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint32_t mask,
                                         uint32_t shift);

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
static inline uint16_t CS_READ_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint16_t mask,
                                         uint32_t shift);

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
static inline uint8_t CS_READ_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	   uint32_t addr,
                                       uint8_t mask,
                                       uint32_t shift);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

static inline uint32_t CS_READ_REG32(VXB_DEV_ID pDev, uint32_t addr)
{
	uint32_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead32 (MMAP_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));
	
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline void CS_WRITE_REG32(VXB_DEV_ID pDev, uint32_t addr, uint32_t value)
{
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite32 (MMAP_SPI_HANDLE(pDev),               \
						(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), value);
	
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline uint16_t CS_READ_REG16(VXB_DEV_ID pDev, uint32_t addr)
{
    uint16_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline void CS_WRITE_REG16(VXB_DEV_ID pDev, uint32_t addr, uint16_t value)
{
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), value);
	
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline uint8_t CS_READ_REG8(VXB_DEV_ID pDev, uint32_t addr)
{
    uint8_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline void CS_WRITE_REG8(VXB_DEV_ID pDev, uint32_t addr, uint8_t value)
{
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), value);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), value);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline void CS_WRITE_FIELD32_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 uint32_t addr,
                                     uint32_t mask,
                                     uint32_t shift,
                                     uint32_t value)
{
    uint32_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead32 (MMAP_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite32 (MMAP_SPI_HANDLE(pDev),               \
						(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), regVal);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline void CS_WRITE_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 uint32_t addr,
                                     uint16_t mask,
                                     uint32_t shift,
                                     uint16_t value)
{
    uint16_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), regVal);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline void CS_WRITE_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	uint32_t addr,
                                    uint8_t mask,
                                    uint32_t shift,
                                    uint8_t value)
{
    uint8_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal &= (~mask);
    regVal |= (value << shift) & mask;
    if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		vxbWrite8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)), regVal);
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		vxbWrite8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)), regVal);

#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return;
}

static inline uint32_t CS_READ_FIELD32_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint32_t mask,
                                         uint32_t shift)
{
    uint32_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead32 (MC_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead32 (MMAP_SPI_HANDLE(pDev),               \
					(UINT32 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline uint16_t CS_READ_FIELD16_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	 	 uint32_t addr,
                                         uint16_t mask,
                                         uint32_t shift)
{
    uint16_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead16 (MC_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead16 (MMAP_SPI_HANDLE(pDev),               \
					(UINT16 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

static inline uint8_t CS_READ_FIELD8_RAW(VXB_DEV_ID pDev,
		 	 	 	 	 	 	 	   uint32_t addr,
                                       uint8_t mask,
                                       uint32_t shift)
{
    uint8_t regVal;
	if (addr >= QSPI_CORE_START && addr <= QSPI_CORE_END)
		 regVal = vxbRead8 (MC_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MC_SPI_BAR(pDev) + (addr - QSPI_CORE_START)));
	else if (addr >= QSPI_FLASH_MMAP_START && addr <= QSPI_FLASH_MMAP_END)
		 regVal = vxbRead8 (MMAP_SPI_HANDLE(pDev),               \
					(UINT8 *)((char *)MMAP_SPI_BAR(pDev) + (addr - QSPI_FLASH_MMAP_START)));

    regVal = (regVal & mask) >> shift;
#ifndef MEM_BARRIER_DISABLE
    asm("    dsb");
#endif
    return (regVal);
}

#ifdef __cplusplus
}
#endif

#endif 
