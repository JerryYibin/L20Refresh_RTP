/* 40gfxFbTiDss.cdf - TI DSS frame buffer driver */

/*
 * Copyright (c) 2013-2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
27jul17,s_m  Modified for TI AM5728 FB implementation
24jan14,mgc  Modified for VxWorks 7 release
17may12,rfm  Written
*/

Component INCLUDE_FBDEV_TIDSS_0 {
    NAME            TI DSS frame buffer 0
    SYNOPSIS        TI DSS frame buffer 0
    _CHILDREN       INCLUDE_FBDEV
    _DEFAULTS       INCLUDE_FBDEV
    ARCHIVE         libgfxTiDssFb.a
    CONFIGLETTES    gfxTiDssInit.c
    PROTOTYPE       void gfxTiDssInit0(void);
    INIT_RTN        gfxTiDssInit0();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usrToolsInit
    INIT_BEFORE     INCLUDE_USER_APPL
    CFG_PARAMS      TIDSS_FBDEV_DISPLAY_0 \
                    TIDSS_FBDEV_RESOLUTION_0 \
                    TIDSS_FBDEV_BUFFERS_0 \
                    TIDSS_FBDEV_VSYNC_0
    REQUIRES        INCLUDE_FBDEV_MEMORY \
                    INCLUDE_IO_SYSTEM
}

Component INCLUDE_FBDEV_TIDSS_1 {
    NAME            TI DSS frame buffer 1
    SYNOPSIS        TI DSS frame buffer 1
    _CHILDREN       INCLUDE_FBDEV
    ARCHIVE         libgfxTiDssFb.a
    CONFIGLETTES    gfxTiDssInit.c
    PROTOTYPE       void gfxTiDssInit1(void);
    INIT_RTN        gfxTiDssInit1();
    _INIT_ORDER     usrRoot
    INIT_AFTER      usrToolsInit
    INIT_BEFORE     INCLUDE_USER_APPL
    CFG_PARAMS      TIDSS_FBDEV_DISPLAY_1 \
                    TIDSS_FBDEV_RESOLUTION_1 \
                    TIDSS_FBDEV_BUFFERS_1 \
                    TIDSS_FBDEV_VSYNC_1
    REQUIRES        INCLUDE_FBDEV_MEMORY \
                    INCLUDE_IO_SYSTEM
}

Parameter TIDSS_FBDEV_DISPLAY_0 {
    NAME            Display device
    SYNOPSIS        "HDMI" for HDMI, "Flat panel" for flat panel
    TYPE            string
    DEFAULT         "HDMI"
}

Parameter TIDSS_FBDEV_DISPLAY_1 {
    NAME            Display device
    SYNOPSIS        "HDMI" for HDMI, "Flat panel" for flat panel
    TYPE            string
    DEFAULT         "Flat panel"
}

Parameter TIDSS_FBDEV_RESOLUTION_0 {
    NAME            Screen resolution
    SYNOPSIS        Screen resolution of "1920x1080-32", "480x800-32", "1024x768-32" for HDMI or "480x272-32" for flat panel
    TYPE            string
    DEFAULT         "480x800-32"
}

Parameter TIDSS_FBDEV_RESOLUTION_1 {
    NAME            Screen resolution
    SYNOPSIS        Screen resolution of "1280x800-32", "1280x720-32", "1024x768-32" for HDMI or "480x272-32" for flat panel
    TYPE            string
    DEFAULT         "480x272-32"
}

Parameter TIDSS_FBDEV_BUFFERS_0 {
    NAME            Number of buffers
    SYNOPSIS        Number of buffers
    TYPE            uint
    DEFAULT         2
}

Parameter TIDSS_FBDEV_BUFFERS_1 {
    NAME            Number of buffers
    SYNOPSIS        Number of buffers
    TYPE            uint
    DEFAULT         2
}

Parameter TIDSS_FBDEV_VSYNC_0 {
    NAME            Vertical synchronization
    SYNOPSIS        Vertical synchronization
    TYPE            bool
    DEFAULT         TRUE
}

Parameter TIDSS_FBDEV_VSYNC_1 {
    NAME            Vertical synchronization
    SYNOPSIS        Vertical synchronization
    TYPE            bool
    DEFAULT         TRUE
}
