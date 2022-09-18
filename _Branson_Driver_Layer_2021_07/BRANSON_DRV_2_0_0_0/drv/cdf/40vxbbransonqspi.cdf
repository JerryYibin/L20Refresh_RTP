/* 40vxbbransonAm572xqspi.cdf - Component Definition file for QSPI Controller*/

/*
 * Copyright To be Done
 */
 
Component       DRV_AM572x_QSPI {
    NAME        Branson AM572x QSPI Controller Driver
    SYNOPSIS    Branson vxBus Driver for AM572x QSPI Controller
    MODULES     vxbTiAm572xQspi.o
    LINK_SYMS   vxbOfAm572xSpiDrv
    REQUIRES    INCLUDE_VXBUS 
    _CHILDREN   FOLDER_DRIVERS
}
