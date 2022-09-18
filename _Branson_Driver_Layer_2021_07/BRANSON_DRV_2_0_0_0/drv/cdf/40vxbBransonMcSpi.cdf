/* 40vxbBransonMcSpi.cdf - Component Definition file for McSpi */

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
01a,30Apr22,ykm  created
*/

Component       BRANSON_DRV_TI_MCSPI {
    NAME        Branson McSpi vxBus driver
    SYNOPSIS    Ti570x McSpi vxBus driver
    MODULES     vxbTi570xSpi.o vxbMcSpiLib.o
    LINK_SYMS   vxbOfTi570xSpiDrv
    REQUIRES    INCLUDE_VXBUS           \
                INCLUDE_POWERDOMAIN_SYS \
                INCLUDE_SPI_BUS
    _CHILDREN   FOLDER_DRIVERS
}
