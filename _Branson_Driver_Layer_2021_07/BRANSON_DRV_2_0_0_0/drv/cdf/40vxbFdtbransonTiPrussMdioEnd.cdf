/* vxbFdtTiPrussMdioEnd.cdf - Component configuration file */

/*
 * Copyright (c) To be done.
 *
 */

/*
modification history
--------------------
01mar17,RR  Create
*/

/*
modification history
--------------------
05jan16,m_w  create. (V7PRO-2729)
*/

Component   BRANSON_DRV_MDIO_END_FDT_TI_PRUSS {
    NAME        PRUSS MDIOSubSystem
    SYNOPSIS    PRUSS MDIOSubSystem
    _CHILDREN   FOLDER_NET_END_DRV
    MODULES     vxbFdtTiPrussMdioEnd.o
    LINK_SYMS   prussMdioDrv
    REQUIRES    INCLUDE_VXBUS INCLUDE_END
}