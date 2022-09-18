/* 40vxbehrpwm.cdf - Component Definition file for PWMSS */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

Component       BRANSON_DRV_TI_EHRPWM {
    NAME        Branson EhrPwm vxBus driver
    SYNOPSIS    EHRPWM vxBus driver
    MODULES     vxbTiehrpwm.o vxbEhrPwmLib.o
    LINK_SYMS   vxbOfAm570xehrpwmDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
