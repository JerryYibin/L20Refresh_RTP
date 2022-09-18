/* gfxTiDssDrv.h - TI DSS frame buffer driver header file */

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
22dec14,yat  Rework pmap of frame buffer for RTP restart (US46449)
24jan14,mgc  Modified for VxWorks 7 release
26mar13,a_f Written
*/

#ifndef __INCgfxTiDssDrvH
#define __INCgfxTiDssDrvH

/* includes */

#include <vxWorks.h>
#include <ioLib.h>
#include <semLib.h>
#include <fbdev.h>
#include <hwif/vxBus.h>

#include <gfxTiDss.h>

#define GFX_USE_PMAP
#define GFX_VXBUS_GEN2

/* defines */

/* Vsync interrupt */
#if 0
#define GFX_VSYNC_IRQ               25
#endif

/* typedefs */

typedef enum gfxDisp
    {
    GFX_DISP_TIAM35XX_HDMI,
    GFX_DISP_TIAM35XX_FLAT_PANEL,
    GFX_DISP_TIAM37XX_DVI,
    GFX_DISP_TIAM37XX_FLAT_PANEL,
    GFX_DISP_TIOMAP35XX_DVI,
    GFX_DISP_TIOMAP35XX_FLAT_PANEL,
    GFX_DISP_TIAM57XX_HDMI,
    } GFX_DISP;

/* Frame buffer display device */
typedef struct
    {
    DEV_HDR                         header;         /* I/O subsystem header */
    int                             enabled;
    /* Video mode */
    GFX_DISP                        disp;           /* display */
    FB_VIDEO_MODE                   fbMode;         /* video mode */
    FB_VIDEO_MODE*                  fbModesDb;
    ULONG                           fbModesCount;
    ULONG                           fbSize;
#if !defined(GFX_USE_PMAP)
    BOOL                            fbRtpFreeMem;
#endif
    GFX_TIDSS_INFO                  info;
    /* Internal data */
    void*                           firstPhysAddr;  /* first buffer address */
    void*                           firstVirtAddr;  /* first buffer address */
    void*                           frontPhysAddr;  /* front buffer address */
    void*                           frontVirtAddr;  /* front buffer address */
#if defined(GFX_VSYNC_IRQ)
    BOOL                            needSwap;
    BOOL                            whenSwap;
    VX_BINARY_SEMAPHORE             (vsync);        /* VBL sync semaphore */
    BOOL                            needVsync;
    VOIDFUNCPTR                     intHandlerVsyncFuncPtr;
#endif
    FUNCPTR                         setVideoModeExFuncPtr;
    FUNCPTR                         setFbAddrFuncPtr;
    char                            deviceName[FB_MAX_STR_CHARS];
    char                            displayName[FB_MAX_STR_CHARS];
    char                            modeStr[FB_MAX_VIDEO_MODE_LEN];
#if defined(GFX_VXBUS_GEN2)
    VXB_DEV_ID                      vxbDev;
#endif
    } GFX_FBDEV;

#endif /* __INCgfxTiDssDrvH */
