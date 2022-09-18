/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXDrv_ioctl.h 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    Definition of the CIFx driver IO control structures and commands

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2017-10-12  FL, fixed build error
    2015-06-08  created

**************************************************************************************/

/*****************************************************************************/
/**! \addtogroup CIFX_DRIVER_API cifX Driver API implementation              */
/**! \{                                                                      */
/*****************************************************************************/

/* prevent multiple inclusion */
#ifndef __CIFxIOCtrl__h_
#define __CIFxIOCtrl__h_

//#include "cifXUser.h"
#include "cifXToolkit/cifXUser.h"

/*****************************************************************************/
/*! Definition for named events                                              */
/*****************************************************************************/
#define NOTIFICATION_STRING_LENGTH 16 /* max Event name length */

/*****************************************************************************/
/*! IO CONTROL DATA STRUCTURES                                               */
/*****************************************************************************/

/* Get Driver Information command */
typedef struct DEVxIO_BASEtag
{
  int32_t*  plError;

} DEVxIO_BASE;

/* Driver: Get Driver Information command */
typedef struct DEVxIO_DRIVER_INFOtag
{
  int32_t*               plError;          /* Driver error */
  uint32_t               ulSize;           /* Size of the driver information structure */
  DRIVER_INFORMATION*    ptDriverInfo;     /* Pointer to store the driver information */

} DEVxIO_DRIVER_INFO;

/* Driver: Board enumeration command */
typedef struct DEVxIO_DRIVER_ENUMBOARDtag
{
  int32_t*               plError;          /* Driver error */
  uint32_t               ulBoardNumber;    /* Board number */
  uint32_t               ulSize;           /* Size of the board info structure */
  BOARD_INFORMATION*     ptBoardInfo;      /* Pointer to store the board information */

} DEVxIO_DRIVER_ENUMBOARD;

/* Driver: Channel enumeration command */
typedef struct DEVxIO_DRIVER_ENUMCHANNELtag
{
  int32_t*                plError;         /* Driver error */
  uint32_t                ulBoardNumber;   /* Board number */
  uint32_t                ulChannelNumber; /* Channel number */
  uint32_t                ulSize;          /* Size of the channel information structure */
  CHANNEL_INFORMATION*    ptChannelInfo;   /* Pointer to store the channel information */

} DEVxIO_DRIVER_ENUMCHANNEL;

/* Driver: Memory mapping function */
typedef struct DEVxIO_DRIVER_MEMPTRtag
{
  int32_t*                plError;         /* Driver error */
  uint32_t                ulBoardNumber;   /* Board number */
  uint32_t                ulCmd;           /* Command */
  MEMORY_INFORMATION*     ptMemory;

} DEVxIO_DRIVER_MEMPTR;

/* Driver: Restart device */
typedef struct DEVxIO_DRIVER_RESTARTDEVICEtag
{
  int32_t*                plError;         /* Driver error */
  char*                   szBoardName;     /* Board name */
  void*                   pvData;          /* Additional information (not used) */

} DEVxIO_DRIVER_RESTARTDEVICE;

/* Driver: Progress callback */
typedef struct IOCTL_PROGRESS_CALLBACK_Ttag
{
  uint32_t  ulStep;
  uint32_t  ulMaxStep;
  char      bFinished;
  int32_t   lError;
  char      szEventIN[NOTIFICATION_STRING_LENGTH];
  char      szEventOUT[NOTIFICATION_STRING_LENGTH];

} IOCTL_PROGRESS_CALLBACK_T;

/* Driver: Receive callback */
typedef struct IOCTL_RECV_CALLBACK_Ttag
{
  void*     pvBuffer;
  uint32_t  ulBufferLen;
  char      szEventIN[NOTIFICATION_STRING_LENGTH];
  char      szEventOUT[NOTIFICATION_STRING_LENGTH];

} IOCTL_RECV_CALLBACK_T;

/*============================================================================================ */
/* Generic
* Download                                                                                     */
/*============================================================================================ */
typedef struct DEVxIO_DOWNLOADtag
{
  int32_t*                  plError;            /* Driver error */
  uint32_t                  ulChannel;          /* Channel number */
  uint32_t                  ulMode;             /* Download mode */
  char*                     pszFileName;        /* File name */
  uint32_t                  ulFileSize;         /* File size in byte */
  void*                     pvFileData;         /* File data pinter */

  IOCTL_PROGRESS_CALLBACK_T* ptProgressData;
  IOCTL_RECV_CALLBACK_T*     ptRecvData;

} DEVxIO_DOWNLOAD;

/*============================================================================================ */
/* Generic
* Upload                                                                                       */
/*============================================================================================ */
typedef struct DEVxIO_UPLOADtag
{
  int32_t*                  plError;            /* Driver error */
  uint32_t                  ulChannel;          /* Channel number */
  uint32_t                  ulMode;             /* Download mode */
  char*                     pszFileName;        /* File name */
  uint32_t*                 pulFileSize;        /* File size in byte */
  void*                     pvFileData;         /* File data pinter */

  IOCTL_PROGRESS_CALLBACK_T* ptProgressData;
  IOCTL_RECV_CALLBACK_T*     ptRecvData;

} DEVxIO_UPLOAD;

/*============================================================================================ */
/* Generic
* Find First / Next                                                                            */
/*============================================================================================ */
typedef struct DEVxIO_FIND_FILEtag
{
  int32_t*               plError;               /* Driver error */
  uint32_t               ulChannel;             /* Channel number */
  CIFX_DIRECTORYENTRY*   ptDirectoryInfo;

  IOCTL_RECV_CALLBACK_T* ptRecvData;

} DEVxIO_FIND_FILE;

/*============================================================================================ */
/* SYSTEM device dependent                                                                     */
/*============================================================================================ */
/* Driver: System channel information */
typedef struct DEVxIO_SYSDEVICE_INFOtag
{
  int32_t*               plError;               /* Driver error */
  uint32_t               ulCmd;                 /* Command */
  uint32_t               ulSize;                /* Size of user buffer */
  void*                  pvInfo;                /* Pointer to the user buffer */
} DEVxIO_SYSDEVICE_INFO;

/* Driver: System channel extended memory information */
typedef struct INTERNAL_EXTENDED_MEMORY_INFORMATIONtag
{
  uint64_t               ullMemoryID;           /* Identification of the memory area       */
  void*                  pvMemoryPtr;           /* Memory pointer                          */
  uint32_t               ulMemorySize;          /* Memory size of the Extended memory area */
  uint32_t               ulMemoryType;          /* Memory type information                 */
} INTERNAL_EXTENDED_MEMORY_INFORMATION;

/* Driver: Memory mapping function */
typedef struct DEVxIO_SYSDEVICE_EXTMEMPTRtag
{
  int32_t*                              plError;  /* Driver error */
  uint32_t                              ulCmd;    /* Command */
  INTERNAL_EXTENDED_MEMORY_INFORMATION* ptMemory; /* Memory information structure */

} DEVxIO_SYSDEVICE_EXTMEMPTR;

/*============================================================================================ */
/* CHANNEL dependent                                                                           */
/*============================================================================================ */
/* Channel: Open command */
typedef struct DEVxIO_CHANNEL_OPENtag
{
  int32_t*             plError;            /* Driver error */
  uint32_t             ulChannel;          /* Channel number */

} DEVxIO_CHANNEL_OPEN;

/* Channel: Close command */
typedef struct DEVxIO_CHANNEL_CLOSEtag
{
  int32_t*             plError;            /* Driver error */
  uint32_t             ulChannel;          /* Channel number */

} DEVxIO_CHANNEL_CLOSE;

/* Channel: Download command */
typedef struct DEVxIO_DOWNLOAD_COMPLETEtag
{
  int32_t*             plError;            /* Driver error */
  uint8_t*             pabFileData;        /* File buffer */
  uint32_t             ulFileDataLen;      /* File buffer length */
  char*                szFileName;         /* File name */
  uint32_t             ulChannel;          /* Channel nummer */

} DEVxIO_DOWNLOAD_COMPLETE;

/* Channel: Reset command */
typedef struct DEVxIO_CHANNEL_RESETtag
{
  int32_t*             plError;            /* Driver error */
  uint32_t             ulChannel;          /* Channel number */
  uint32_t             ulMode;             /* Reset mode */
  uint32_t             ulTimeout;          /* Reset timeout */

} DEVxIO_CHANNEL_RESET;

/* Channel: Information command */
typedef struct DEVxIO_CHANNEL_INFOtag
{
  int32_t*             plError;            /* Driver error */
  uint32_t             ulChannel;          /* Channel number */
  uint32_t             ulSize;             /* User buffer size */
  CHANNEL_INFORMATION* ptChannelInfo;      /* User buffer */

} DEVxIO_CHANNEL_INFO;

/* Channel: Host state command */
typedef struct DEVxIO_CHANNEL_HOSTSTATEtag
{
  int32_t*   plError;            /* Driver error */
  uint32_t   ulChannel;          /* Channel number */
  uint32_t   ulCmd;              /* User command */
  uint32_t*  pulState;           /* Actual/Last state */
  uint32_t   ulTimeout;          /* Command timeout */

} DEVxIO_CHANNEL_HOSTSTATE;

/* Channel: Mailbox state command */
typedef struct DEVxIO_CHANNEL_MBXSTATEtag
{
  int32_t*   plError;            /* Driver error */
  uint32_t   ulChannel;          /* Channel number */
  uint32_t*  pulReceiveCount;    /* Receive packet count */
  uint32_t*  pulSendState;       /* Send packet state */

} DEVxIO_CHANNEL_MBXSTATE;

/* Channel: Packet command */
typedef struct DEVxIO_CHANNEL_PACKETtag
{
  int32_t*   plError;            /* Driver error */
  uint32_t   ulChannel;          /* Channel number */
  void*      pvPacket;           /* Packet buffer pointer */
  uint32_t   ulBufferSize;       /* Buffer size */
  uint32_t   ulTimeout;          /* Timeout */

} DEVxIO_CHANNEL_PACKET;

/* Channel: Watchdog command */
typedef struct DEVxIO_CHANNEL_WATCHDOGtag
{
  int32_t*   plError;            /* Driver error */
  uint32_t   ulChannel;          /* Channel number */
  uint32_t   ulCmd;              /* Watchdog command */
  uint32_t*  pulWatchdog;        /* Actual watchdog value */

} DEVxIO_CHANNEL_WATCHDOG;

/* Channel: Configuration Lock */
typedef struct DEVxIO_CHANNEL_CONFIG_LOCKtag
{
  int32_t*   plError;            /* Driver error */
  uint32_t   ulChannel;          /* Channel number */
  uint32_t   ulCmd;              /* Watchdog command */
  uint32_t*  pulState;           /* Actual state */
  uint32_t   ulTimeout;          /* Timeout */

} DEVxIO_CHANNEL_CONFIG_LOCK;

/* Channel: Bus state */
typedef struct DEVxIO_CHANNEL_BUSSTATEtag
{
  int32_t*   plError;            /* Driver error */
  uint32_t   ulChannel;          /* Channel number */
  uint32_t   ulCmd;              /* User command */
  uint32_t*  pulState;           /* Actual/Last state */
  uint32_t   ulTimeout;          /* Command timeout */

} DEVxIO_CHANNEL_BUSSTATE;

/* Channel: DMA state */
typedef struct DEVxIO_CHANNEL_DMASTATEtag
{
  int32_t*    plError;            /* Driver error */
  uint32_t    ulChannel;          /* Channel number */
  uint32_t    ulCmd;              /* User command */
  uint32_t*   pulState;           /* Actual/Last state */

} DEVxIO_CHANNEL_DMASTATE;

/* Channel: I/O functions, I/O information */
typedef struct DEVxIO_CHANNEL_IOINFOtag
{
  int32_t*                plError;            /* Driver error */
  uint32_t                ulChannel;          /* Channel number */
  uint32_t                ulCmd;              /* Channel number */
  uint32_t                ulAreaNumber;       /* I/O area number */
  uint32_t                ulSize;             /* User buffer size */
  CHANNEL_IO_INFORMATION* ptChannelIOInfo;    /* User buffer */
} DEVxIO_CHANNEL_IOINFO;

/* Channel: I/O functions, I/O read */
typedef struct DEVxIO_CHANNEL_IOREADtag
{
  int32_t*  plError;            /* Driver error */
  uint32_t  ulChannel;          /* Channel number */
  uint32_t  ulAreaNumber;       /* I/O area number */
  uint32_t  ulOffset;           /* Data start offset */
  uint32_t  ulSize;             /* Data size */
  void*     pvBuffer;           /* Data buffer */
  uint32_t  ulTimeout;          /* Timeout */

} DEVxIO_CHANNEL_IOREAD;

/* Channel: I/O functions, I/O write */
typedef struct DEVxIO_CHANNEL_IOWRITEtag
{
  int32_t*   plError;            /* Driver Error */
  uint32_t   ulChannel;          /* Channel number */
  uint32_t   ulAreaNumber;       /* I/O area number */
  uint32_t   ulOffset;           /* Data start offset */
  uint32_t   ulSize;             /* Data size */
  void*      pvBuffer;           /* Data buffer */
  uint32_t   ulTimeout;          /* Timeout */

} DEVxIO_CHANNEL_IOWRITE;

/*****************************************************************************/
/*! Internal PLC Memory Information structure                                */
/*****************************************************************************/
/* Channel: I/O Memory mapping function */
typedef struct DEVxIO_PLC_IOMEMPTRtag
{
  int32_t*                plError;          /* Driver error */
  uint32_t                ulBoardNumber;    /* Board number */
  uint32_t                ulChannel;        /* Channel number */
  uint32_t                ulCmd;            /* Command */
  PLC_MEMORY_INFORMATION* ptMemory;

} DEVxIO_CHANNEL_IOMEMPTR;

/* Channel: I/O functions, I/O is read ready */
typedef struct DEVxIO_CHANNEL_IOREADRDYtag
{
  int32_t*    plError;              /* Driver Error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulAreaNumber;         /* I/O area number */
  uint32_t*   pulState;             /* Actual I/O state */

} DEVxIO_CHANNEL_IOREADRDY;

/* Channel: I/O functions, I/O is write ready */
typedef struct DEVxIO_CHANNEL_IOWRITERDYtag
{
  int32_t*    plError;              /* Driver Error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulAreaNumber;         /* I/O area number */
  uint32_t*   pulState;             /* Actual I/O state */

} DEVxIO_CHANNEL_IOWRITERDY;

/* Channel: I/O functions, I/O do read/write */
typedef struct DEVxIO_CHANNEL_IODORWtag
{
  int32_t*    plError;              /* Driver Error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulAreaNumber;         /* I/O area number */

} DEVxIO_CHANNEL_IODORW;

/* Channel Information command */
typedef struct DEVxIO_CHANNEL_STATEtag
{
  int32_t*    plError;              /* Driver Error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulCmd;                /* Command */
  uint32_t    ulOffset;             /* Offset */
  uint32_t    ulSize;               /* Buffer size */
  void*       pvBuffer;             /* User buffer */

} DEVxIO_CHANNEL_STATE;

/* Channel Information command */
typedef struct DEVxIO_CHANNEL_CTRLtag
{
  int32_t*    plError;              /* Driver Error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulCmd;                /* Command */
  uint32_t    ulOffset;             /* Offset */
  uint32_t    ulSize;               /* Buffer size */
  void*       pvBuffer;             /* User buffer */

} DEVxIO_CHANNEL_CTRL;

/* Channel Information command */
typedef struct DEVxIO_CHANNEL_USERtag
{
  int32_t*    plError;              /* Driver Error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulAreaNumber;         /* Area number */
  uint32_t    ulCmd;                /* Command */
  uint32_t    ulOffset;             /* Offset */
  uint32_t    ulSize;               /* Buffer size */
  void*       pvBuffer;             /* User buffer */

} DEVxIO_CHANNEL_USER;

/* Channel: Notification command */
typedef struct DEVxIO_CHANNEL_NOTIFICATIONtag
{
  int32_t*    plError;                                  /* Driver error */
  uint32_t    ulChannel;                                /* Channel number */
  uint32_t    ulNotification;                           /* Notification */
  char        szEventName[NOTIFICATION_STRING_LENGTH];  /* Notify event */

} DEVxIO_CHANNEL_NOTIFICATION;

/* Channel: Notification command */
#define DEVIOCTL_DATA_BUFFER_SIZE 128
typedef struct DEVxIO_CHANNEL_NOTIFY_DATAtag
{
  int32_t*    plError;              /* Driver error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulNotification;       /* Notification index */
  uint32_t    ulDataLen;
  uint8_t     abData[DEVIOCTL_DATA_BUFFER_SIZE];

} DEVxIO_CHANNEL_NOTIFY_DATA;

/* Channel: Sync state */
typedef struct DEVxIO_CHANNEL_SYNCSTATEtag
{
  int32_t*    plError;              /* Driver error */
  uint32_t    ulChannel;            /* Channel number */
  uint32_t    ulCmd;                /* User command */
  uint32_t    ulTimeout;            /* Timeout in ms */
  uint32_t*   pulSyncCount;         /* Sync counter */

} DEVxIO_CHANNEL_SYNCSTATE;

/*============================================================================================ */
/* NXSB-PCA dependent functions                                                                */
/*============================================================================================ */
/* Driver: Memory mapping fuction */
typedef struct DEVxIO_NXSB_MEMPTRtag
{
  int32_t*    plError;              /* Driver error */
  void*       pvHandle;             /* Handle of mapping */
  void**      ppvDPMPointer;        /* Memory pointer */
  uint32_t    ulDataLength;         /* Complete size of the mapped memory */
} DEVxIO_NXSB_MEMPTR;

/*----------------------------------------- */
/* IO CONTROL CODE DEFINITIONS              */
/*----------------------------------------- */
/* IOCTRL function definitions */
#define IOCTLNOFUNCTION                 0

#define IOCTL_DRV_DEVICENAME        0x0700

#define IOCTL_DRV_INFO              0x0800
#define IOCTL_DRV_ENUMBOARD         0x0801
#define IOCTL_DRV_ENUMCHANNEL       0x0802
#define IOCTL_DRV_MEMPTR            0x0803
#define IOCTL_DRV_DOWNLOADCOMPLETE  0x0804
#define IOCTL_DRV_RESTARTDEVICE     0x0805

#define IOCTL_SYSDEVICE_INFO        0x0820
#define IOCTL_SYSDEVICE_OPEN        0x0821
#define IOCTL_SYSDEVICE_CLOSE       0x0822
#define IOCTL_SYSDEVICE_MBXSTATE    0x0823
#define IOCTL_SYSDEVICE_PUTPACKET   0x0824
#define IOCTL_SYSDEVICE_GETPACKET   0x0825
#define IOCTL_SYSDEVICE_RESET       0x0826
#define IOCTL_SYSDEVICE_DOWNLOAD    0x0827
#define IOCTL_SYSDEVICE_UPLOAD      0x0828
#define IOCTL_SYSDEVICE_FINDFIRST   0x0829
#define IOCTL_SYSDEVICE_FINDNEXT    0x082A
#define IOCTL_SYSDEVICE_BOOTSTART   0x082B
#define IOCTL_SYSDEVICE_EXTMEM      0x082C

#define IOCTL_CH_OPEN               0x0840
#define IOCTL_CH_CLOSE              0x0841
#define IOCTL_CH_DOWNLOAD           0x0842
#define IOCTL_CH_UPLOAD             0x0843
#define IOCTL_CH_FINDFIRST          0x0844
#define IOCTL_CH_FINDNEXT           0x0845

#define IOCTL_CH_INFO               0x0850
#define IOCTL_CH_HOSTSTATE          0x0851
#define IOCTL_CH_MBXSTATE           0x0852
#define IOCTL_CH_PUTPACKET          0x0853
#define IOCTL_CH_GETPACKET          0x0854
#define IOCTL_CH_READSENDPACKET     0x0855
#define IOCTL_CH_RESET              0x0856
#define IOCTL_CH_WATCHDOG           0x0857
#define IOCTL_CH_CONFIGLOCK         0x0858
#define IOCTL_CH_BUSSTATE           0x0859
#define IOCTL_CH_DMASTATE           0x085A

#define IOCTL_CH_IOINFO             0x085F
#define IOCTL_CH_IOREAD             0x0860
#define IOCTL_CH_IOWRITE            0x0861
#define IOCTL_CH_IOREADSEND         0x0862
#define IOCTL_CH_IODOWRITE          0x0863
#define IOCTL_CH_IODOREAD           0x0864
#define IOCTL_CH_IOWRITERDY         0x0865
#define IOCTL_CH_IOREADRDY          0x0866
#define IOCTL_CH_IOMEMPTR           0x0868

#define IOCTL_CH_CTRL               0x0870
#define IOCTL_CH_COMMON_STATE       0x0871
#define IOCTL_CH_EXTENDED_STATE     0x0872
#define IOCTL_CH_USER               0x0873

#define IOCTL_CH_NOTIFY_REG         0x0880
#define IOCTL_CH_NOTIFY_UNREG       0x0881
#define IOCTL_CH_NOTIFY_DATA        0x0882
#define IOCTL_CH_SYNC_STATE         0x0883

/* NXSB-PCA test functions */
#define IOCTL_NXSB_DPMINFO          0x1000
#define IOCTL_NXSB_MAP              0x1003
#define IOCTL_NXSB_UNMAP            0x1004

/* IOCTRL codes */
#define CIFx_IOCTL_DRV_INFO             IOCTL_DRV_INFO
#define CIFx_IOCTL_DRV_ENUMBOARD        IOCTL_DRV_ENUMBOARD
#define CIFx_IOCTL_DRV_ENUMCHANNEL      IOCTL_DRV_ENUMCHANNEL
#define CIFx_IOCTL_DRV_MEMPTR           IOCTL_DRV_MEMPTR
#define CIFx_IOCTL_DRV_DOWNLOADCOMPLETE IOCTL_DRV_DOWNLOADCOMPLETE
#define CIFx_IOCTL_DRV_RESTARTDEVICE    IOCTL_DRV_RESTARTDEVICE


#define CIFx_IOCTL_SYSDEVICE_INFO       IOCTL_SYSDEVICE_INFO
#define CIFx_IOCTL_SYSDEVICE_OPEN       IOCTL_SYSDEVICE_OPEN
#define CIFx_IOCTL_SYSDEVICE_CLOSE      IOCTL_SYSDEVICE_CLOSE
#define CIFx_IOCTL_SYSDEVICE_DOWNLOAD   IOCTL_SYSDEVICE_DOWNLOAD
#define CIFx_IOCTL_SYSDEVICE_UPLOAD     IOCTL_SYSDEVICE_UPLOAD
#define CIFx_IOCTL_SYSDEVICE_FINDFIRST  IOCTL_SYSDEVICE_FINDFIRST
#define CIFx_IOCTL_SYSDEVICE_FINDNEXT   IOCTL_SYSDEVICE_FINDNEXT
#define CIFx_IOCTL_SYSDEVICE_MBXSTATE   IOCTL_SYSDEVICE_MBXSTATE
#define CIFx_IOCTL_SYSDEVICE_PUTPACKET  IOCTL_SYSDEVICE_PUTPACKET
#define CIFx_IOCTL_SYSDEVICE_GETPACKET  IOCTL_SYSDEVICE_GETPACKET

#define CIFx_IOCTL_SYSDEVICE_RESET      IOCTL_SYSDEVICE_RESET
#define CIFx_IOCTL_SYSDEVICE_BOOTSTART  IOCTL_SYSDEVICE_BOOTSTART
#define CIFx_IOCTL_SYSDEVICE_EXTMEM     IOCTL_SYSDEVICE_EXTMEM

#define CIFx_IOCTL_CH_OPEN              IOCTL_CH_OPEN
#define CIFx_IOCTL_CH_CLOSE             IOCTL_CH_CLOSE
#define CIFx_IOCTL_CH_DOWNLOAD          IOCTL_CH_DOWNLOAD
#define CIFx_IOCTL_CH_UPLOAD            IOCTL_CH_UPLOAD
#define CIFx_IOCTL_CH_FINDFIRST         IOCTL_CH_FINDFIRST
#define CIFx_IOCTL_CH_FINDNEXT          IOCTL_CH_FINDNEXT

#define CIFx_IOCTL_CH_INFO              IOCTL_CH_INFO
#define CIFx_IOCTL_CH_HOSTSTATE         IOCTL_CH_HOSTSTATE
#define CIFx_IOCTL_CH_MBXSTATE          IOCTL_CH_MBXSTATE
#define CIFx_IOCTL_CH_PUTPACKET         IOCTL_CH_PUTPACKET
#define CIFx_IOCTL_CH_GETPACKET         IOCTL_CH_GETPACKET
#define CIFx_IOCTL_CH_READSENDPACKET    IOCTL_CH_READSENDPACKET
#define CIFx_IOCTL_CH_RESET             IOCTL_CH_RESET
#define CIFx_IOCTL_CH_WATCHDOG          IOCTL_CH_WATCHDOG
#define CIFx_IOCTL_CH_CONFIGLOCK        IOCTL_CH_CONFIGLOCK
#define CIFx_IOCTL_CH_BUSSTATE          IOCTL_CH_BUSSTATE
#define CIFx_IOCTL_CH_DMASTATE          IOCTL_CH_DMASTATE

#define CIFx_IOCTL_CH_IOINFO            IOCTL_CH_IOINFO
#define CIFx_IOCTL_CH_IOREAD            IOCTL_CH_IOREAD
#define CIFx_IOCTL_CH_IOWRITE           IOCTL_CH_IOWRITE
#define CIFx_IOCTL_CH_IOREADSEND        IOCTL_CH_IOREADSEND

#define CIFx_IOCTL_CH_IOMEMPTR          IOCTL_CH_IOMEMPTR
#define CIFx_IOCTL_CH_IODOWRITE         IOCTL_CH_IODOWRITE
#define CIFx_IOCTL_CH_IODOREAD          IOCTL_CH_IODOREAD
#define CIFx_IOCTL_CH_IOWRITERDY        IOCTL_CH_IOWRITERDY
#define CIFx_IOCTL_CH_IOREADRDY         IOCTL_CH_IOREADRDY

#define CIFx_IOCTL_CH_CTRL              IOCTL_CH_CTRL
#define CIFx_IOCTL_CH_COMMON_STATE      IOCTL_CH_COMMON_STATE
#define CIFx_IOCTL_CH_EXTENDED_STATE    IOCTL_CH_EXTENDED_STATE
#define CIFx_IOCTL_CH_USER              IOCTL_CH_USER

#define CIFx_IOCTL_CH_NOTIFY_REG        IOCTL_CH_NOTIFY_REG
#define CIFx_IOCTL_CH_NOTIFY_UNREG      IOCTL_CH_NOTIFY_UNREG
#define CIFx_IOCTL_CH_NOTIFY_DATA       IOCTL_CH_NOTIFY_DATA
#define CIFx_IOCTL_CH_SYNC_STATE        IOCTL_CH_SYNC_STATE

/* NXSB-PCA test functions */
#define CIFx_IOCTL_NXSB_DPMINFO         IOCTL_NXSB_DPMINFO
#define CIFx_IOCTL_NXSB_MAP             IOCTL_NXSB_MAP
#define CIFx_IOCTL_NXSB_UNMAP           IOCTL_NXSB_UNMAP

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

#endif  /* __CIFxIOCtrl__h_ */
