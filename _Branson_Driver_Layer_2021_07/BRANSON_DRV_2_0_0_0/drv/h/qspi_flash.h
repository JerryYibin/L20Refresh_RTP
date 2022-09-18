/**
 * \file       qspi_flash.h
 *
 * \brief      This file contains the device data for QSPI flash
 *             devices. It contains a structure which captures the
 *             data which is different for different flash devices.
 */


#ifndef QSPI_FLASH_H_
#define QSPI_FLASH_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vxWorks.h>
#include <vsbConfig.h>
#include <semLib.h>
#include <hwif/vxBus.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * This structure contains the device data for different flash devices
 *        which can be connected to the QSPI controller.
 *        Only the flash device specific data is captured here
 */
typedef struct qspiFlashDeviceData
{
    uint32_t devId;
    /* Device ID of the QSPI flash */
    uint32_t quadReadCmd;
    /* The command to read the register where the quad enable bit has
     *   to be set */
    uint32_t quadWriteCmd;
    /* The command to write to the register where the quad enable bit has
     *   to be set */
    uint32_t quadEnableBit;
    /* The quad enable bit which needs to be set/reset in the register to enable
     *   the quad mode of the QSPI flash device */
    uint32_t quadValue;
    /* The value to be written to enable quad functionality i.e. 0 or 1 */
}qspiFlashDeviceData_t;


/**
 * This API returns the QSPI flash related data according to the
 *        flash device connected to the board.
 *
 * devId The device id of the device that is connected to QSPI
 *                   controller.
 *
 * retval QSPI_Device_Data Pointer to the QSPI device data.
 */
qspiFlashDeviceData_t *QSPIFlashGetDeviceData(uint32_t devId);

/**
 * This API returns the list of QSPI flash devices available
 *
 * retval QSPI_Flash_Device_Id_List Pointer to the QSPI flash device ID list
 */
uint32_t * QSPIFlashGetDeviceIdList(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef QSPI_FLASH_H_ */

