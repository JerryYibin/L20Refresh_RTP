/* sysLib.c - TI AM57XX with Cortex-A15 system-dependent library */

/*
 * Copyright (c) 2015, 2018 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
19jan18,wch  updated to support endMacGet() (F6878)
10oct15,yjw  created (US64529)
*/

/*
DESCRIPTION
This library provides board-specific routines for TI AM57XX 
with cortex-A15 processor.

INCLUDE FILES: vxFdtLib.h

SEE ALSO:
\tb VxWorks Programmer's Guide: Configuration
*/

/* includes */

#include <vxWorks.h>
#include <boardLib.h>
#include "prjParams.h"
#include <am5xxx.h>

#ifdef INCLUDE_SHOW_ROUTINES
#include <stdio.h>
#include <stdlib.h>
#include <vxFdtLib.h>

/* defines */

LOCAL void boardInfo (void);
IMPORT void cpuArmVerShow (void);

#endif /* INCLUDE_SHOW_ROUTINES */

LOCAL BOARD_FUNC_TBL sitaraFuncTbl = {
    /* .earlyInit  = */ am5xxxEarlyInit,
    /* .init       = */ am5xxxInit,
    /* .reset      = */ am5xxxReset,
    /* .model      = */ am5xxxModel,
    /* .usDelay    = */ am5xxxUsDelay,
#ifdef _WRS_CONFIG_SMP
    /* .cpuEn      = */ am5xxxCpuEnable,
    /* .cpuAvail   = */ am5xxxCpuAvail,
    /* .cpuDis     = */ NULL,
#endif /*_WRS_CONFIG_SMP*/ 
#ifdef INCLUDE_SHOW_ROUTINES
    /* .infoShow   = */ boardInfo,
#else
    /* .infoShow   = */ NULL,
#endif /* INCLUDE_SHOW_ROUTINES */
    /* .endMacGet  = */ NULL
};

LOCAL BOARD_DESC ti_sitara_a15 =
    {
    /* .uVer     = */ BOARD_DESC_VER_3_0,
    /* .pCompat  = */ "ti,sitara-ctxa15",
#ifdef INCLUDE_DEBUG_KPRINTF
    /* .uFlag    = */ BOARD_DESC_FLAG (BOARD_DESC_FLAG_DBG, 0),
#else
    /* .uFlag    = */ BOARD_DESC_FLAG (0, 0),
#endif
    /* .probe    = */ am5xxxProbe,
    /* .pFuncTbl = */ &sitaraFuncTbl
    };

BOARD_DEF (ti_sitara_a15)

/* Only for 19.2M crystal oscillator */
#include <private/pmapLibP.h>
#include <pmapLib.h>

void clkSet192(BOOL use192)
    {
    UINT32 addr  = 0x4a0052b4; /* CM_CLKSEL_DPLL_GMAC */
    UINT32 value = 0x0002710b; /* DPLL_MULT[18:8] DPLL_DIV[6:0] */
    void  *vAddr;

    if(use192 == TRUE)
        {
        vAddr = pmapGlobalMap(addr,0x100,MMU_ATTR_CACHE_DEFAULT | MMU_ATTR_VALID | MMU_ATTR_SUP_RW);
        if(vAddr != PMAP_FAILED)
            {
            *(UINT32 *)(vAddr) = value;
            pmapGlobalUnmap(vAddr,0x100);
            }
        }
    }
/*******************************************************/
#ifdef INCLUDE_SHOW_ROUTINES

/*******************************************************************************
*
* socInfoShow - print SOC information
*
* This routine prints SOC information.
*
* RETURNS: N/A
* ERRNO: N/A
*/

LOCAL void socInfoShow(void)
    {
    }

/*******************************************************************************
*
* boardInfo - print board information
*
* This routine prints board information.
*
* RETURNS: N/A
* 
* ERRNO: N/A
*/

LOCAL void boardInfo (void)
    {
    socInfoShow();

    cpuArmVerShow();

    return;
    }
#endif /* INCLUDE_SHOW_ROUTINES */
