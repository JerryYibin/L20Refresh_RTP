/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXVxWorks_internal.h 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    VxWorks driver internal definitions

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  initial version

**************************************************************************************/
#ifndef __CIFXVXWORKS_INTERNAL__H
#define __CIFXVXWORKS_INTERNAL__H

/*****************************************************************************/
/*! \file cifXVxWorks_internal.h
*   VxWorks driver internal definitions                                      */
/*****************************************************************************/

#include "cifXVxWorks.h"
#include "cifxDrv_ioctl.h"
#include "cifXToolkit/cifXToolkit.h"

/*****************************************************************************/
/*!  Actual driver version                                                   */
/*****************************************************************************/
#define VXWCIFXDRV_VERSION        "VxWCIFXDrv V2.0.0.0"

/*****************************************************************************/
/*!  Default driver parameters                                               */
/*****************************************************************************/
#define CIFX_DRIVER_DEFAULT_BASEDIR       "/hd0/cifX"
#define CIFX_DRIVER_DEFAULT_POLL_INTERVAL 500

/*****************************************************************************/
/*! Bit modes for PLX chip on NXPCA-PCI card                                 */
/*****************************************************************************/
#define PLX_MODE_8BIT     0
#define PLX_MODE_16BIT    1
#define PLX_MODE_32BIT    2
#define PLX_MODE_INVALID  0xFFFFFFFF

/*****************************************************************************/
/*! OFFSET of PLX register                                                   */
/*****************************************************************************/
#define PLX_GPIO_OFFSET_CHAR     0x54
#define PLX_GPIO_OFFSET_DW       0x15
#define PLX_TIMING_OFFSET_CHAR   0x28
#define PLX_TIMING_OFFSET_DW     0x0A

/*****************************************************************************/
/*! PLX GPIO Register                                                        */
/*****************************************************************************/
#define MSK_PLX_GPIO_DATA_0 0x00000004
#define SRT_PLX_GPIO_DATA_0 2
#define MSK_PLX_GPIO_DATA_1 0x00000020
#define SRT_PLX_GPIO_DATA_1 5

/*****************************************************************************/
/*! Local Control Register Offsets                                           */
/*****************************************************************************/
#define PLX_PCI_INTCTRLSTS_REG      0x4C

#define PLX_HW_INTERRUPT_DISABLE    0x00
#define PLX_HW_INTERRUPT_ENABLE     0x01
#define PLX_HW_INTERRUPT_STATE      0x04

/*****************************************************************************/
/*! Timing values for PLX chip on NXPCA-PCI card                             */
/*****************************************************************************/
static ULONG s_aulDefaultPLXTimings[] =
{
  0x5431F962,   /*!< 8  Bit Timing value */
  0x4073F8E2,   /*!< 16 Bit Timing value */
  0x40824122    /*!< 32 Bit Timing value */
};

/*****************************************************************************/
/*! Structure describing a single device                                     */
/*****************************************************************************/
typedef struct cifXDevData
{
  VXB_RESOURCE *  pResDPM;
  VXB_RESOURCE *  pResIRQ;
  VXB_RESOURCE *  pResEXTMEM;
  VXB_RESOURCE *  pResPLXRegDPM;
} CIFX_DEV_RESOURCE_DATA;

typedef struct VXW_CIFXDRV_DEVICEENTRY_Ttag
{
    VXB_DEV_ID                  pDev;               /*!< Device ID                */
    CIFX_DEV_RESOURCE_DATA      tCifXPciData;       /*!< Resource information     */
    unsigned short              vendorId;           /*!< PCI device information   */
    unsigned short              deviceId;           /*!< PCI device information   */
    unsigned short              subSystemId;        /*!< PCI device information   */
    unsigned short              subVendorId;        /*!< PCI device information   */
    unsigned long               ulPhysicalAddress;  /*!< Physical DPM address     */
    unsigned char               bIrqNumber;         /*!< Interrupt number         */
    void*                       pvDPMAddress;       /*!< Virtual pointer to DPM   */
    unsigned long               ulDPMSize;          /*!< Size of the DPM in bytes */
    BOOL                        fPCICard;           /*!< TRUE if device is a PCI Card */
} VXW_CIFXDRV_DEVICEENTRY_T, *PVXW_CIFXDRV_DEVICEENTRY_T;

/*****************************************************************************/
/*! PCI Device information                                                   */
/*****************************************************************************/
typedef struct VXW_CIFXDRV_PCIDEVICEINFO_Ttag
{
  PCI_HEADER_DEVICE tPCIConfig;        /*!< PCI configuration */
  BOOL              fPLXBridge;        /*!< TRUE if PLX DPM Bridge is available */
  volatile uint8_t* pbPLXRegisters;    /*!< Pointer to PLX Registers            */
  uint32_t          ulPLXRegistersLen; /*!< Length of mapped PLX Registers      */
  uint32_t          ulPLXMode;         /*!< Bitwidth of NXPCA-PCI board         */

} VXW_CIFXDRV_PCIDEVICEINFO_T, *PVXW_CIFXDRV_PCIDEVICEINFO_T;

/*****************************************************************************/
/*! Callback registration data                                               */
/*****************************************************************************/
typedef struct CALLBACK_REGISTRATION_Ttag
{
  SEM_ID        hUserEvent;                               /*!< Event to signal to user               */
  /* TODO: Define how large the callback data can get */
  char          abNotifyData[DEVIOCTL_DATA_BUFFER_SIZE];  /*!< Notification data stored from toolkit */
  unsigned long ulNotifyDataLen;                          /*!< Length of stored notification data    */

} CALLBACK_REGISTRATION_T;


/*****************************************************************************/
/*! Internal device instance structure                                       */
/*****************************************************************************/
#define CIFX_NOTIFY_MAX  9
typedef struct VXW_CIFXDRV_DEVINSTANCE_Ttag
{
  DEVICEINSTANCE              tDeviceInst;
  VXW_CIFXDRV_DEVICEENTRY_T   tDeviceEntry;
  VXW_CIFXDRV_PCIDEVICEINFO_T tPciInfo;

  TASK_ID                     iDeferredServiceThread;
  int                         fDeferredServiceThreadRunning;
  int                         fDeferredServiceThreadSuspended;
  void*                       hDeferredServiceEvent;

#ifdef CIFX_TOOLKIT_ENABLE_DSR_LOCK
  int                         ftIsrLockInitialized;
  spinlockIsr_t               tIsrLock;
#endif

#ifdef CIFX_TOOLKIT_DMA
  VXB_DMA_TAG_ID              ptVxbDmaTagId;
  VXB_DMA_MAP_ID              ptVxbDmaMapId[CIFX_DMA_BUFFER_COUNT];
#endif

  CIFXHANDLE                  hDriver;                                          /*!< Handle to the driver returned from xOpenDriver */
  unsigned long               ulOpenCount;                                      /*!< Number of times the driver was opened          */

  CIFXHANDLE                  hSysdevice;                                       /*!< Handle to system device returned from xSysdeviceOpen */
  unsigned long               ulSysdeviceOpenCnt;                               /*!< Number of times the system device was opened         */

  CIFXHANDLE                  ahChannels[CIFX_MAX_NUMBER_OF_CHANNELS];          /*!< Handles to channels returned from xChannelOpen */
  unsigned long               aulChannelOpenCnt[CIFX_MAX_NUMBER_OF_CHANNELS];   /*!< Number of times the channel was opened         */

  CALLBACK_REGISTRATION_T     aatNotifyCallbacks[CIFX_MAX_NUMBER_OF_CHANNELS][CIFX_NOTIFY_MAX];

} VXW_CIFXDRV_DEVINSTANCE_T, *PVXW_CIFXDRV_DEVINSTANCE_T;

/*****************************************************************************/
/*! Progress callback data                                                   */
/*****************************************************************************/
typedef struct PROGRESS_CALLBACK_DATA_Ttag
{
  uint32_t*  pulStep;       /*!< Pointer to current step  */
  uint32_t*  pulMaxStep;    /*!< Pointer to maximum steps */
  char*      pbFinished;
  int32_t*   plError;

} PROGRESS_CALLBACK_DATA_T;

/*****************************************************************************/
/*! Receive packet callback data                                             */
/*****************************************************************************/
typedef struct RECV_CALLBACK_DATA_Ttag
{
  void*      pvBuffer;      /*!< Buffer for packet            */
  uint32_t   ulBufferLen;   /*!< Length of the packet buffer  */

} RECV_CALLBACK_DATA_T;

/*****************************************************************************/
/*! Structure for storing callback data                                      */
/*****************************************************************************/
typedef struct CALLBACK_DATA_Ttag
{
  PROGRESS_CALLBACK_DATA_T    tProgressCallback;
  void*                       hProgressIn;
  void*                       hProgressOut;
  int                         fSuppressProgress;

  RECV_CALLBACK_DATA_T        tRecvCallback;
  void*                       hRecvIn;
  void*                       hRecvOut;
  int                         fSuppressRecv;
  PVXW_CIFXDRV_DEVINSTANCE_T  ptDevExt;

} CALLBACK_DATA_T, *PCALLBACK_DATA_T;

#endif /* __CIFXVXWORKS_INTERNAL__H */

