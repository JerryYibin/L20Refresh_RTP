/* vxbGpioLib.h - vxBus GPIO interfaces module */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05may22,ykm  created.
*/

/*
DESCRIPTION

This library provides the GPIO specific interfaces

*/

#ifndef _INC_vxbGpioLibH
#define _INC_vxbGpioLibH

/* includes */
#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif
#define GPIO_DIR_INPUT          (0)
#define GPIO_DIR_OUTPUT         (1)
#define GPIO_VALUE_LOW          (0)
#define GPIO_VALUE_HIGH         (1)
#define GPIO_VALUE_INVALID      (0xff)
typedef enum intr_trigger
    {
    INTR_TRIGGER_CONFORM = 0,
    INTR_TRIGGER_EDGE    = 1,
    INTR_TRIGGER_LEVEL   = 2
    }INTR_TRIGER;
typedef enum intr_polarity
    {
    INTR_POLARITY_CONFORM = 0,
    INTR_POLARITY_HIGH    = 1,
    INTR_POLARITY_LOW     = 2,
    INTR_POLARITY_BOTH
    }INTR_POLARITY;
extern STATUS vxbGpioAlloc(UINT32);
extern STATUS vxbGpioFree(UINT32);
extern BOOL   vxbGpioIsFree(UINT32);
extern STATUS vxbGpioSetDir(UINT32, UINT32);
extern UINT32 vxbGpioGetDir(UINT32);
extern UINT32 vxbGpioGetValue(UINT32);
extern STATUS vxbGpioSetValue(UINT32, UINT32);
extern STATUS vxbGpioSetDebounce(UINT32, UINT32);
extern STATUS vxbGpioIntConfig(UINT32, INTR_TRIGER, INTR_POLARITY);
extern STATUS vxbGpioIntConnect(UINT32, VOIDFUNCPTR, void *);
extern STATUS vxbGpioIntDisconnect(UINT32, VOIDFUNCPTR, void *);
extern STATUS vxbGpioIntEnable(UINT32, VOIDFUNCPTR, void *);
extern STATUS vxbGpioIntDisable(UINT32, VOIDFUNCPTR, void *);
#ifdef __cplusplus
}
#endif

#endif /* _INC_vxbGpioLibH */


