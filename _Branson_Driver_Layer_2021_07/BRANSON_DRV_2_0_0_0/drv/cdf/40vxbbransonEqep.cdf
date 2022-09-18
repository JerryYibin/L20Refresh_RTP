/* 40vxbEqep.cdf - Component Definition file for PWMSS */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Component       BRANSON_DRV_TI_EQEP {
    NAME        Branson Eqep vxBus driver
    SYNOPSIS    EQEP vxBus driver
    MODULES     vxbTiEqep.o vxbEqepLib.o
    LINK_SYMS   vxbOfAm570xeqepDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
