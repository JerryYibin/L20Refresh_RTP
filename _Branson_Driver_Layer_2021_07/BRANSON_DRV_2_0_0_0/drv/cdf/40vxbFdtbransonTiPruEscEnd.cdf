/* 40vxbFdtTiPruEscEnd.cdf - Component configuration file */

/*
 * Copyright (c) To be done.
 *
 */

/*
modification history
--------------------
01mar17,RR  Create
*/

Component   BRANSON_DRV_END_FDT_TI_PRU_ESC {
    NAME        PRU ESC Driver
    SYNOPSIS    PRU ESC Driver for AM437x Device
    _CHILDREN   FOLDER_NET_END_DRV
    MODULES     vxbFdtTiPruEscEnd.o 
    LINK_SYMS   pruEscDrv
    REQUIRES    INCLUDE_END INCLUDE_VXBUS
}