/* timerDev.h - Generic Timer header */

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
This file contains header information for generic timer routines.
*/

#ifndef __INCtimerDevh
#define __INCtimerDevh

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */
#ifndef _ASMLANGUAGE
extern STATUS     sysAuxClkConnect(FUNCPTR, _Vx_usr_arg_t);
extern void       sysAuxClkDisable();
extern void       sysAuxClkEnable();
extern _Vx_freq_t sysAuxClkRateGet();
extern STATUS     sysAuxClkRateSet(_Vx_freq_t);

extern UINT32 sysTimestampFreq();
extern UINT32 sysTimestampLock();
#endif /* _ASMLANGUAGE */
#ifdef __cplusplus
}
#endif
#endif /* __INCtimerDevh */

