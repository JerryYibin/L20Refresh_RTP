/* vxbFdtTiPrussIntcEnd.cdf - Component configuration file */

/*
 * Copyright (c) To be done.
 *
 */

/*
modification history
--------------------
01mar17,RR  Create
*/


Component   BRANSON_DRV_INTC_END_FDT_TI_PRUSS {
    NAME        PRUSS INTCSubSystem
    SYNOPSIS    PRUSS INTCSubSystem
    _CHILDREN   FOLDER_NET_END_DRV
    MODULES     vxbFdtTiPrussIntcEnd.o
    LINK_SYMS   prussIntcDrv
    REQUIRES    INCLUDE_VXBUS INCLUDE_END
}