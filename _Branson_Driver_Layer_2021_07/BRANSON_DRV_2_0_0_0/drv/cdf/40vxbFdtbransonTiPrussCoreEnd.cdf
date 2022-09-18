/* 40vxbFdtTiPrussCoreEnd.cdf - Component configuration file */

/*
 * Copyright (c) To be done.
 *
 */

/*
modification history
--------------------
01mar17,RR  Create
*/


Component   BRANSON_DRV_CORE_END_FDT_TI_PRUSS {
    NAME        PRUSS CoreSubSystem
    SYNOPSIS    PRUSS CoreSubSystem
    _CHILDREN   FOLDER_NET_END_DRV
    MODULES     vxbFdtTiPrussCoreEnd.o
    LINK_SYMS   pruCoreDrv
    REQUIRES    INCLUDE_VXBUS INCLUDE_END
}