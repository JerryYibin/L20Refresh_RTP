/* 40vxbbransonWdt.cdf - Component Definition file for WDT */

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
01a,26jan22,ykm  created
*/

Component       BRANSON_DRV_TI_WDT {
    NAME        Branson Wdt Driver
    SYNOPSIS    vxBus Driver for Ti Wdt
    MODULES     vxbTiWdt.o
    LINK_SYMS   vxbTiFdtWdtDrv
    REQUIRES    INCLUDE_VXBUS INCLUDE_WATCHDOGDRV
    _CHILDREN   FOLDER_DRIVERS
}
