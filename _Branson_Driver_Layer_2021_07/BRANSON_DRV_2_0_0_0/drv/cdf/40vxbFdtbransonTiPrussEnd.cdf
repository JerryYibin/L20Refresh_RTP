/* 40vxbFdtTiPrussEnd.cdf - Component configuration file */

/*
 * Copyright (c) To be done.
 *
 */

/*
modification history
--------------------
01mar17,RR  Create
*/


Component   BRANSON_DRV_END_FDT_TI_PRUSS {
    NAME        PRUSS SubSystem
    SYNOPSIS    PRUSS SubSystem
    _CHILDREN   FOLDER_NET_END_DRV
    MODULES     vxbFdtTiPrussEnd.o
    LINK_SYMS   prussDrv
    REQUIRES    INCLUDE_VXBUS INCLUDE_END
}