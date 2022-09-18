/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: cifXVxWorks.h 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    Definitions for the VxWorks driver initialization and handling functions

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  created from VxWorks driver V6.x revision 3

**************************************************************************************/
#ifndef __CIFXVXWORKS__H
#define __CIFXVXWORKS__H

/*****************************************************************************/
/*! \file cifXVxWorks.h
*   Definitions for the VxWorks driver initialization and handling functions */
/*****************************************************************************/
#include <vxworks.h>
#include <stdint.h>

#include "cifXToolkit/cifXErrors.h"
#include "cifXToolkit/cifXToolkit.h"
#include "cifXToolkit/HilPCIDefs.h"
#include "cifXToolkit/netx51_romloader_dpm.h"

#ifdef __cplusplus
  extern "C" {
#endif  /* _cplusplus */

/*****************************************************************************/
/*! Notification events                                                      */
/*****************************************************************************/
typedef enum CIFX_NOTIFY_Etag
{
  eCIFX_EVENT_PRERESET        = 0, /*!< Event signalled, before device is reset (HW Reset) */
  eCIFX_EVENT_POSTRESET,           /*!< Called after HW reset has been executed            */
  eCIFX_EVENT_PRE_BOOTLOADER,      /*!< Called before bootloader is downloaded             */
  eCIFX_EVENT_POST_BOOTLOADER      /*!< Called after bootloader was downloaded and started */

} CIFX_NOTIFY_E;

typedef void(*PFN_CIFX_NOTIFY_EVENT)(void* pvDeviceEntry, CIFX_NOTIFY_E eEvent);

/*****************************************************************************/
/*!  Global driver functions                                                 */
/*****************************************************************************/
int32_t cifXInitializeDriver  ( BOOL           fSingleDir,           /*!< !=0 will use a single firmware directory */
                                unsigned long  ulTraceLevel,         /*!< Trace level at startup */
                                unsigned long  ulPollInterval,       /*!< Intervall to poll for COS Flag changes */
                                char*          szDriverBaseDir);     /*!< Base device directory to search for settings/configuration/firmware files */
int32_t cifXDeinitializeDriver( void );


#ifdef __cplusplus
}
#endif

/*****************************************************************************/
/*! \}                                                                       */
/*****************************************************************************/

#endif /* __CIFXVXWORKS__H */
