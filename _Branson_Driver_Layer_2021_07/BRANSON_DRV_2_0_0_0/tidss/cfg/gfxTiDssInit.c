/* gfxTiDssInit.c - TI DSS frame buffer driver initialization */

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
22mar13,af  Written
*/

#ifndef __INCgfxTiDssInitc
#define __INCgfxTiDssInitc

#include <vxWorks.h>

#ifndef SYS_MODEL
#define SYS_MODEL                   "TI AM57xx"
#endif

#ifndef FBDEV_MEMORY_SIZE
#define FBDEV_MEMORY_SIZE           (16*1024*1024)
#endif
#ifndef FBDEV_PREEMPTIVE_RTP_MEMORY_FREE
#define FBDEV_PREEMPTIVE_RTP_MEMORY_FREE       TRUE
#endif

#ifdef INCLUDE_FBDEV_TIDSS_0
#ifndef TIDSS_FBDEV_NAME_0
#define TIDSS_FBDEV_NAME_0          "/dev/fb0"
#endif
#ifndef TIDSS_FBDEV_DISPLAY_0
#define TIAM35XX_FBDEV_DISPLAY_0    "HDMI"
#define TIAM37XX_FBDEV_DISPLAY_0    "DVI"
#define TIOMAP35XX_FBDEV_DISPLAY_0  "DVI"
#define TIAM57XX_FBDEV_DISPLAY_0  "HDMI"
#else
#define TIAM35XX_FBDEV_DISPLAY_0    TIDSS_FBDEV_DISPLAY_0
#define TIAM37XX_FBDEV_DISPLAY_0    TIDSS_FBDEV_DISPLAY_0
#define TIOMAP35XX_FBDEV_DISPLAY_0  TIDSS_FBDEV_DISPLAY_0
#define TIAM57XX_FBDEV_DISPLAY_0    TIDSS_FBDEV_DISPLAY_0
#endif
#ifndef TIDSS_FBDEV_RESOLUTION_0
#define TIAM35XX_FBDEV_RESOLUTION_0 "default"
#define TIAM37XX_FBDEV_RESOLUTION_0 "default"
#define TIOMAP35XX_FBDEV_RESOLUTION_0 "default"
#define TIAM57XX_FBDEV_RESOLUTION_0 "default"
#else
#define TIAM35XX_FBDEV_RESOLUTION_0 TIDSS_FBDEV_RESOLUTION_0
#define TIAM37XX_FBDEV_RESOLUTION_0 TIDSS_FBDEV_RESOLUTION_0
#define TIOMAP35XX_FBDEV_RESOLUTION_0 TIDSS_FBDEV_RESOLUTION_0
#define TIAM57XX_FBDEV_RESOLUTION_0 TIDSS_FBDEV_RESOLUTION_0
#endif
#ifndef TIDSS_FBDEV_BUFFERS_0
#define TIAM35XX_FBDEV_BUFFERS_0    2
#define TIAM37XX_FBDEV_BUFFERS_0    2
#define TIOMAP35XX_FBDEV_BUFFERS_0  2
#define TIAM57XX_FBDEV_BUFFERS_0    2
#else
#define TIAM35XX_FBDEV_BUFFERS_0    TIDSS_FBDEV_BUFFERS_0
#define TIAM37XX_FBDEV_BUFFERS_0    TIDSS_FBDEV_BUFFERS_0
#define TIOMAP35XX_FBDEV_BUFFERS_0  TIDSS_FBDEV_BUFFERS_0
#define TIAM57XX_FBDEV_BUFFERS_0    TIDSS_FBDEV_BUFFERS_0
#endif
#ifndef TIDSS_FBDEV_VSYNC_0
#define TIAM35XX_FBDEV_VSYNC_0      TRUE
#define TIAM37XX_FBDEV_VSYNC_0      TRUE
#define TIOMAP35XX_FBDEV_VSYNC_0    TRUE
#define TIAM57XX_FBDEV_VSYNC_0      TRUE
#else
#define TIAM35XX_FBDEV_VSYNC_0      TIDSS_FBDEV_VSYNC_0
#define TIAM37XX_FBDEV_VSYNC_0      TIDSS_FBDEV_VSYNC_0
#define TIOMAP35XX_FBDEV_VSYNC_0    TIDSS_FBDEV_VSYNC_0
#define TIAM57XX_FBDEV_VSYNC_0      TIDSS_FBDEV_VSYNC_0
#endif
#endif

#ifdef INCLUDE_FBDEV_TIDSS_1
#ifndef TIDSS_FBDEV_NAME_1
#define TIDSS_FBDEV_NAME_1          "/dev/fb1"
#endif
#ifndef TIDSS_FBDEV_DISPLAY_1
#define TIAM35XX_FBDEV_DISPLAY_1    "Flat panel"
#define TIAM37XX_FBDEV_DISPLAY_1    "Flat panel"
#define TIOMAP35XX_FBDEV_DISPLAY_1  "Flat panel"
#else
#define TIAM35XX_FBDEV_DISPLAY_1    TIDSS_FBDEV_DISPLAY_1
#define TIAM37XX_FBDEV_DISPLAY_1    TIDSS_FBDEV_DISPLAY_1
#define TIOMAP35XX_FBDEV_DISPLAY_1  TIDSS_FBDEV_DISPLAY_1
#endif
#ifndef TIDSS_FBDEV_RESOLUTION_1
#define TIAM35XX_FBDEV_RESOLUTION_1 "default"
#define TIAM37XX_FBDEV_RESOLUTION_1 "default"
#define TIOMAP35XX_FBDEV_RESOLUTION_1 "default"
#else
#define TIAM35XX_FBDEV_RESOLUTION_1 TIDSS_FBDEV_RESOLUTION_1
#define TIAM37XX_FBDEV_RESOLUTION_1 TIDSS_FBDEV_RESOLUTION_1
#define TIOMAP35XX_FBDEV_RESOLUTION_1 TIDSS_FBDEV_RESOLUTION_1
#endif
#ifndef TIDSS_FBDEV_BUFFERS_1
#define TIAM35XX_FBDEV_BUFFERS_1    2
#define TIAM37XX_FBDEV_BUFFERS_1    2
#define TIOMAP35XX_FBDEV_BUFFERS_1  2
#else
#define TIAM35XX_FBDEV_BUFFERS_1    TIDSS_FBDEV_BUFFERS_1
#define TIAM37XX_FBDEV_BUFFERS_1    TIDSS_FBDEV_BUFFERS_1
#define TIOMAP35XX_FBDEV_BUFFERS_1  TIDSS_FBDEV_BUFFERS_1
#endif
#ifndef TIDSS_FBDEV_VSYNC_1
#define TIAM35XX_FBDEV_VSYNC_1      TRUE
#define TIAM37XX_FBDEV_VSYNC_1      TRUE
#define TIOMAP35XX_FBDEV_VSYNC_1    TRUE
#else
#define TIAM35XX_FBDEV_VSYNC_1      TIDSS_FBDEV_VSYNC_1
#define TIAM37XX_FBDEV_VSYNC_1      TIDSS_FBDEV_VSYNC_1
#define TIOMAP35XX_FBDEV_VSYNC_1    TIDSS_FBDEV_VSYNC_1
#endif
#endif

#ifdef INCLUDE_FBDEV_SPLASH
#ifndef FB_SPLASH_BLIT
#define FB_SPLASH_BLIT              gfxFbSplashBlit
#endif
IMPORT STATUS FB_SPLASH_BLIT (void*, int, int, int, int);
#endif

IMPORT void gfxTiDssInit (const char*, FUNCPTR);

#ifdef INCLUDE_FBDEV_TIDSS_0
/*******************************************************************************
 *
 * gfxTiDssGetCfg0 - Get frame buffer configuration
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssGetCfg0
    (
    size_t*             pVidMemSize,
    unsigned int*       pRtpFreeMem,
    char**              pDeviceName,
    char**              pDisplayName,
    char**              pResolution,
    unsigned int*       pBuffers,
    unsigned int*       pVsync,
    FUNCPTR*            pConsoleFuncPtr,
    FUNCPTR*            pSplashFuncPtr
    )
    {
    *pVidMemSize = FBDEV_MEMORY_SIZE;
    *pRtpFreeMem = FBDEV_PREEMPTIVE_RTP_MEMORY_FREE;
    *pDeviceName = TIDSS_FBDEV_NAME_0;
    if (strstr (SYS_MODEL, "TI AM35xx") != NULL)
        {
        *pDisplayName = TIAM35XX_FBDEV_DISPLAY_0;
        *pResolution = TIAM35XX_FBDEV_RESOLUTION_0;
        *pBuffers = TIAM35XX_FBDEV_BUFFERS_0;
        *pVsync = TIAM35XX_FBDEV_VSYNC_0;
        }
    else if (strstr (SYS_MODEL, "Mistral DM37x") != NULL)
        {
        *pDisplayName = TIAM37XX_FBDEV_DISPLAY_0;
        *pResolution = TIAM37XX_FBDEV_RESOLUTION_0;
        *pBuffers = TIAM37XX_FBDEV_BUFFERS_0;
        *pVsync = TIAM37XX_FBDEV_VSYNC_0;
        }
    else if (strstr (SYS_MODEL, "Mistral OMAP35xx") != NULL)
        {
        *pDisplayName = TIOMAP35XX_FBDEV_DISPLAY_0;
        *pResolution = TIOMAP35XX_FBDEV_RESOLUTION_0;
        *pBuffers = TIOMAP35XX_FBDEV_BUFFERS_0;
        *pVsync = TIOMAP35XX_FBDEV_VSYNC_0;
        }
    else
        {
        *pDisplayName = TIAM57XX_FBDEV_DISPLAY_0;
        *pResolution = TIAM57XX_FBDEV_RESOLUTION_0;
        *pBuffers = TIAM57XX_FBDEV_BUFFERS_0;
        *pVsync = TIAM57XX_FBDEV_VSYNC_0;
        }
#if defined(INCLUDE_FBDEV_CONSOLE)
    *pConsoleFuncPtr = FB_CONSOLE_WRITE;
#else
    *pConsoleFuncPtr = NULL;
#endif
#if defined(INCLUDE_FBDEV_SPLASH)
    *pSplashFuncPtr = FB_SPLASH_BLIT;
#else
    *pSplashFuncPtr = NULL;
#endif
    return OK;
    }

/*******************************************************************************
*
* gfxTiDssInit0 - Frame buffer driver initialization
*
* RETURNS: N/A
*
*/
void gfxTiDssInit0
    (
    void
    )
    {
    gfxTiDssInit (SYS_MODEL, gfxTiDssGetCfg0);
#if defined (INCLUDE_FBDEV_CONSOLE)
    gfxFbConsoleInit ();
#endif
    }
#endif

#ifdef INCLUDE_FBDEV_TIDSS_1
/*******************************************************************************
 *
 * gfxTiDssGetCfg1 - Get frame buffer configuration
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
LOCAL STATUS gfxTiDssGetCfg1
    (
    size_t*             pVidMemSize,
    unsigned int*       pRtpFreeMem,
    char**              pDeviceName,
    char**              pDisplayName,
    char**              pResolution,
    unsigned int*       pBuffers,
    unsigned int*       pVsync,
    FUNCPTR*            pConsoleFuncPtr,
    FUNCPTR*            pSplashFuncPtr
    )
    {
    *pVidMemSize = FBDEV_MEMORY_SIZE;
    *pRtpFreeMem = FBDEV_PREEMPTIVE_RTP_MEMORY_FREE;
    *pDeviceName = TIDSS_FBDEV_NAME_1;
    if (strstr (SYS_MODEL, "TI AM35xx") != NULL)
        {
        *pDisplayName = TIAM35XX_FBDEV_DISPLAY_1;
        *pResolution = TIAM35XX_FBDEV_RESOLUTION_1;
        *pBuffers = TIAM35XX_FBDEV_BUFFERS_1;
        *pVsync = TIAM35XX_FBDEV_VSYNC_1;
        }
    else if (strstr (SYS_MODEL, "Mistral DM37x") != NULL)
        {
        *pDisplayName = TIAM37XX_FBDEV_DISPLAY_1;
        *pResolution = TIAM37XX_FBDEV_RESOLUTION_1;
        *pBuffers = TIAM37XX_FBDEV_BUFFERS_1;
        *pVsync = TIAM37XX_FBDEV_VSYNC_1;
        }
    else if (strstr (SYS_MODEL, "Mistral OMAP35xx") != NULL)
        {
        *pDisplayName = TIOMAP35XX_FBDEV_DISPLAY_1;
        *pResolution = TIOMAP35XX_FBDEV_RESOLUTION_1;
        *pBuffers = TIOMAP35XX_FBDEV_BUFFERS_1;
        *pVsync = TIOMAP35XX_FBDEV_VSYNC_1;
        }
    else
        {
        *pDisplayName = TIAM57XX_FBDEV_DISPLAY_0;
        *pResolution = TIAM57XX_FBDEV_RESOLUTION_0;
        *pBuffers = TIAM57XX_FBDEV_BUFFERS_0;
        *pVsync = TIAM57XX_FBDEV_VSYNC_0;
        }
#if defined(INCLUDE_FBDEV_CONSOLE)
    *pConsoleFuncPtr = FB_CONSOLE_WRITE;
#else
    *pConsoleFuncPtr = NULL;
#endif
#if defined(INCLUDE_FBDEV_SPLASH)
    *pSplashFuncPtr = FB_SPLASH_BLIT;
#else
    *pSplashFuncPtr = NULL;
#endif
    return OK;
    }

/*******************************************************************************
*
* gfxTiDssInit1 - Frame buffer driver initialization
*
* RETURNS: N/A
*
*/
void gfxTiDssInit1
    (
    void
    )
    {
    gfxTiDssInit (SYS_MODEL, gfxTiDssGetCfg1);
#if defined (INCLUDE_FBDEV_CONSOLE)
    gfxFbConsoleInit ();
#endif
    }
#endif

#if defined(INCLUDE_FBDEV_TIDSS_0) || defined(INCLUDE_FBDEV_TIDSS_1)

/*******************************************************************************
 *
 * gfxTiDssI2cRead - I2C read
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
STATUS gfxTiDssI2cRead
    (
    UINT8 a,
    UINT8 b,
    char* c
    )
    {
    return ERROR;
    }

/*******************************************************************************
 *
 * gfxTiDssI2cWrite - I2C write
 *
 * RETURNS: OK on success, ERROR otherwise
 *
 */
STATUS gfxTiDssI2cWrite
    (
    UINT8 a,
    char* b
    )
    {
    return ERROR;
    }
#endif

#endif /* __INCgfxTiDssInitc */
