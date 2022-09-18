/* gfxTiDss.h - TI DSS header file */

/*
 * Copyright (c) 2013-2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
24jan14,mgc  Modified for VxWorks 7 release
25mar13,af   created
*/

#ifndef __INCgfxTiDssH
#define __INCgfxTiDssH

/* defines */

/* register read and write macros */

#define GFX_READ_REG32(_addr)         \
    (*(volatile UINT32 *)(_addr))
#define GFX_WRITE_REG32(_addr, _data) \
    *((volatile UINT32 *)(_addr)) = (_data)

#define GFX_READ_REG16(_addr)         \
    (*(volatile UINT16 *)(_addr))
#define GFX_WRITE_REG16(_addr, _data) \
    *((volatile UINT16 *)(_addr)) = (_data)

#define GFX_READ_BIT32(_addr, _offset) \
    (GFX_READ_REG32(_addr) & (0x1 << (_offset)))
#define GFX_WRITE_BIT32(_addr, _offset, _data)        \
    GFX_WRITE_REG32(_addr,                            \
    (GFX_READ_REG32(_addr) & (~(0x1 << (_offset)))) | \
    ((_data & 0x1) << (_offset)))

#define GFX_CLR_BIT32(_addr, _mask) \
    GFX_WRITE_REG32(_addr, GFX_READ_REG32(_addr) & (~_mask))
#define GFX_SET_BIT32(_addr, _mask) \
    GFX_WRITE_REG32(_addr, GFX_READ_REG32(_addr) | (_mask))

/* address map configuration */

#define GFX_TI_REG_STATE_MASK                (VM_STATE_MASK_VALID \
                                            | VM_STATE_MASK_WRITABLE \
                                            | VM_STATE_MASK_CACHEABLE)

#define GFX_TI_REG_STATE                     (VM_STATE_VALID \
                                            | VM_STATE_WRITABLE \
                                            | VM_STATE_CACHEABLE_NOT)

/* registers address */

/* display subsystem CM registers */

#define GFX_TI_CM_REG_SIZE                      (0x2000)
#if 0   /* AM57XX change */
#define GFX_TI_CM                               (0x48004000)
#else
#define GFX_TI_CM                               (0x58004000)
#endif
#define GFX_TI_CM_FCLKEN_DSS                    ((unsigned int)virtDssCmAddr + 0xE00)
#define GFX_TI_CM_ICLKEN_DSS                    ((unsigned int)virtDssCmAddr + 0xE10)
#define GFX_TI_CM_CLKSEL_DSS                    ((unsigned int)virtDssCmAddr + 0xE40)
#define GFX_TI_CM_CLKSEL2_PLL                   ((unsigned int)virtDssCmAddr + 0xD44)
#define GFX_TI_CM_CLKSEL3_PLL                   ((unsigned int)virtDssCmAddr + 0xD48)

/* display subsystem registers */

#define GFX_TI_DSS_REG_SIZE                     (0x1000)
#if 0   /* AM57XX change */
#define GFX_TI_DSS                              (0x48050000)
#else
#define GFX_TI_DSS                              (0x58000000)
#endif
#define GFX_TI_DSS_SYSCONFIG                    ((unsigned int)virtDssAddr + 0x010)
#define GFX_TI_DSS_SYSSTATUS                    ((unsigned int)virtDssAddr + 0x014)
#define GFX_TI_DSS_CONTROL                      ((unsigned int)virtDssAddr + 0x040)

/* display controller registers */

#if 0   /* AM57XX change */
#define GFX_TI_DISPC                            (0x48050400)
#else
#define GFX_TI_DISPC                            (0x58001000)
#endif
#define GFX_TI_DISPC_SYSCONFIG                  ((unsigned int)virtDispcAddr + 0x010)
#define GFX_TI_DISPC_IRQSTATUS                  ((unsigned int)virtDispcAddr + 0x018)
#define GFX_TI_DISPC_IRQENABLE                  ((unsigned int)virtDispcAddr + 0x01C)
#define GFX_TI_DISPC_CONTROL                    ((unsigned int)virtDispcAddr + 0x040)
#define GFX_TI_DISPC_CONFIG                     ((unsigned int)virtDispcAddr + 0x044)
#define GFX_TI_DISPC_TRANS_COLOR                ((unsigned int)virtDispcAddr + 0x054)
#define GFX_TI_DISPC_TIMING_H                   ((unsigned int)virtDispcAddr + 0x064)
#define GFX_TI_DISPC_TIMING_V                   ((unsigned int)virtDispcAddr + 0x068)
#define GFX_TI_DISPC_POL_FREQ                   ((unsigned int)virtDispcAddr + 0x06C)
#define GFX_TI_DISPC_DIVISOR                    ((unsigned int)virtDispcAddr + 0x070)
#define GFX_TI_DISPC_GLOBAL_ALPHA               ((unsigned int)virtDispcAddr + 0x074)
#define GFX_TI_DISPC_SIZE_LCD                   ((unsigned int)virtDispcAddr + 0x07C)

/* display subsystem config register */

#define GFX_TI_DSS_SYSCONFIG_SOFTRESET          1

/* display subsystem status register */

#define GFX_TI_DSS_SYSSTATUS_RESETDONE          0

/* display controller sysconfig register */

#define GFX_TI_DISPC_SYSCONFIG_MIDLEMODE        12
#define GFX_TI_DISPC_SYSCONFIG_SIDLEMODE        3

/* display controller IRQ status register */

#define GFX_TI_DISPC_IRQSTATUS_VSYNC            1
#define GFX_TI_DISPC_IRQSTATUS_FRAMEDONE        0

/* display controller register */

#define GFX_TI_DISPC_CONTROL_GPOUT1             16
#define GFX_TI_DISPC_CONTROL_GPOUT0             15
#define GFX_TI_DISPC_CONTROL_TFTDATALINES       8
#define GFX_TI_DISPC_CONTROL_GOTV               6
#define GFX_TI_DISPC_CONTROL_GOLCD              5
#define GFX_TI_DISPC_CONTROL_STNTFT             3
#define GFX_TI_DISPC_CONTROL_TVENABLE           1
#define GFX_TI_DISPC_CONTROL_LCDENABLE          0

/* display config register */

#define GFX_TI_DISPC_CONFIG_LCDALPHABLENDEREN   18
#define GFX_TI_DISPC_CONFIG_TCKLCDSELECTION     11
#define GFX_TI_DISPC_CONFIG_TCKLCDENABLE        10
#define GFX_TI_DISPC_CONFIG_LOADMODE            1

/* timing logic for the HSYNC signal register */

#define GFX_TI_DISPC_TIMING_H_HBP               20
#define GFX_TI_DISPC_TIMING_H_HFP               8
#define GFX_TI_DISPC_TIMING_H_HSW               0

/* timing logic for the VSYNC signal register */

#define GFX_TI_DISPC_TIMING_V_VBP               20
#define GFX_TI_DISPC_TIMING_V_VFP               8
#define GFX_TI_DISPC_TIMING_V_VSW               0

/* signal configuration register */

#define GFX_TI_DISPC_POL_FREQ_ONOFF             17
#define GFX_TI_DISPC_POL_FREQ_RF                16
#define GFX_TI_DISPC_POL_FREQ_IPC               14
#define GFX_TI_DISPC_POL_FREQ_IHS               13
#define GFX_TI_DISPC_POL_FREQ_IVS               12

/* divisors register */

#define GFX_TI_DISPC_DIVISOR_LCD                16
#define GFX_TI_DISPC_DIVISOR_PCD                0

/* global alpha register */

#define GFX_TI_DISPC_GLOBAL_VID                 16
#define GFX_TI_DISPC_GLOBAL_GFX                 0

/* panel size register */

#define GFX_TI_DISPC_SIZE_LCD_LPP               16
#define GFX_TI_DISPC_SIZE_LCD_PPL               0

/* FCLKEN register */
#define GFX_TI_CM_FCLKEN_DSS_EN_TV              2
#define GFX_TI_CM_FCLKEN_DSS_EN_DSS2            1
#define GFX_TI_CM_FCLKEN_DSS_EN_DSS1            0

/* ICLKEN register */

#define GFX_TI_CM_ICLKEN_DSS_EN_DSS             0

/* DISPC functional clock register */

#define GFX_TI_DISPC_CLK_SWITCH                 (0x1 << 0)
#define GFX_TI_DSI_CLK_SWITCH                   (0x1 << 1)

#define GFX_TI_DSS_SYSCONFIG_AUTOIDEL           (0x1 << 0) 
#define GFX_TI_CM_CLKSEL2_PLL_MULT_MASK         (0x7FF << 8)
#define GFX_TI_CM_CLKSEL2_PLL_MULT_SHIFT        8
#define GFX_TI_CM_CLKSEL2_PLL_MULT              (432 << 8)
#define GFX_TI_CM_CLKSEL2_PLL_DIV_MASK          (0x7F << 0)
#define GFX_TI_CM_CLKSEL2_PLL_DIV               (12 << 0)

#define GFX_TI_CM_CLKSEL3_PLL_DIV_96M_MASK      (0x1F << 0)
#define GFX_TI_CM_CLKSEL3_PLL_DIV_96M           (0x9)

/* DISPC configration */

#define GFX_DISPC_LOAD_MODE                     (0x2)    /* frame data only */
#define GFX_CM_DSS_DIV                          (0x7)
#define GFX_CM_DSS_MASK                         (0xFFFFFFE0)

/* alpha configration */

#define GFX_TI_ALPHA_FULLY_OPAQUE               (0xFF)
#define GFX_TI_ALPHA_FULLY_TRANSPARENT          (0x0)

#define GFX_TI_DSS_GFX_BA0                      ((unsigned int)virtDispcAddr + 0x080)
#define GFX_TI_DSS_GFX_POSITION                 ((unsigned int)virtDispcAddr + 0x088)
#define GFX_TI_DSS_GFX_SIZE                     ((unsigned int)virtDispcAddr + 0x08C)
#define GFX_TI_DSS_GFX_ATTRIBUTES               ((unsigned int)virtDispcAddr + 0x0A0)
#define GFX_TI_DSS_GFX_FIFO_THRESHOLD           ((unsigned int)virtDispcAddr + 0x0A4)
#define GFX_TI_DSS_GFX_ROW_INC                  ((unsigned int)virtDispcAddr + 0x0AC)
#define GFX_TI_DSS_GFX_PIXEL_INC                ((unsigned int)virtDispcAddr + 0x0B0)

/* position of the GFX window register */

#define GFX_TI_DSS_GFX_POSITION_Y               16
#define GFX_TI_DSS_GFX_POSITION_X               0

/* size of the GFX window register */

#define GFX_TI_DSS_GFX_SIZE_GFXSIZEY            16
#define GFX_TI_DSS_GFX_SIZE_GFXSIZEX            0

/* GFX attributes register */

#define GFX_TI_DSS_GFX_ATTR_GFXBURSTSIZE        6
#define GFX_TI_DSS_GFX_ATTR_REPLICATION         5
#define GFX_TI_DSS_GFX_ATTR_GFXFORMAT           1
#define GFX_TI_DSS_GFX_ATTR_GFXENABLE           0


/* GFX FIFO register */

#define GFX_TI_DSS_GFX_FIFO_HIGH                16
#define GFX_TI_DSS_GFX_FIFO_LOW                 0

/* GFX configration */

#define GFX_FIFO_THRESHOLD_HIGH             (0xFC0)
#define GFX_FIFO_THRESHOLD_LOW              (0x800)
#define GFX_BURST_SIZE                      (0x2)
#define GFX_POSITION_Y_MAX                  (0x7FF)
#define GFX_POSITION_X_MAX                  (0x7FF)

/* typedefs */

/* GFX color format */

typedef enum gfxTiDispcGfxColorFormat
    {
    GFX_TI_DSS_GFX_ARGB16    = 0x5,
    GFX_TI_DSS_GFX_RGB16     = 0x6,
    GFX_TI_DSS_GFX_RGB24UP   = 0x8,
    GFX_TI_DSS_GFX_RGB24P    = 0x9,
    GFX_TI_DSS_GFX_ARGB32    = 0xC,
    GFX_TI_DSS_GFX_RGBA32    = 0xD
    } GFX_TI_GFX_COLOR_FORMAT;

/* VID1 color format */

typedef enum gfxTiDispcVid1ColorFormat
    {
    GFX_TI_DSS_VID1_RGB16    = 0x6,
    GFX_TI_DSS_VID1_RGB24UP  = 0x8,
    GFX_TI_DSS_VID1_RGB24P   = 0x9,
    GFX_TI_DSS_VID1_YUV2     = 0xA,
    GFX_TI_DSS_VID1_UYVY     = 0xB
    } GFX_TI_VID1_COLOR_FORMAT;

/* LCD data lines */

typedef enum gfxTiLcdDataLines
    {
    GFX_TIDSS_DATA_LINES_12 = 0,
    GFX_TIDSS_DATA_LINES_16,
    GFX_TIDSS_DATA_LINES_18,
    GFX_TIDSS_DATA_LINES_24
    } GFX_TIDSS_DATA_LINES;

/* graphics pipeline */

typedef enum gfxTiDispcPipe
    {
    GFX_TI_DISPC_PIPE_GFX = 0,
    GFX_TI_DISPC_PIPE_VID1,
    GFX_TI_DISPC_PIPE_VID2
    } GFX_TI_DISPC_PIPE;

typedef struct
    {
    UINT8   lcdAlphaEn;        /* LCD alpha blender enabled */
    UINT8   lcdTckEn;          /* LCD transparency color key enabled */
    UINT8   lcdTckSel;         /* LCD transparency color key selection */
    } GFX_TI_DISPC_OVERLAY_INFO;

typedef struct
    {
    UINT8   rf;               /* HSYNC/VSYNC Rise or Fall */
    UINT8   ipc;              /* invert pixel clock */
    UINT8   ihs;              /* invert HSYNC */
    UINT8   ivs;              /* invert VSYNC */
    UINT32  hfp;              /* horizontal front porch */
    UINT32  hbp;              /* horizontal back porch */
    UINT32  hsw;              /* horizontal synchronization pulse width */
    UINT32  vfp;              /* vertical front porch */
    UINT32  vbp;              /* vertical back porch */
    UINT32  vsw;              /* vertical synchronization pulse width */
    UINT32  pcd;              /* pixel clock divisor */
    UINT32  dcd;              /* dss1  clock divisor */
    UINT32  p4m;              /* pll4 multiplier */
    UINT32  p4d;              /* pll4 divisor */
    UINT32  xres;
    UINT32  yres;
    } GFX_TIDSS_TIMING;

typedef struct
    {
    GFX_TIDSS_TIMING*        pTiming;   /* timing */
    GFX_TIDSS_DATA_LINES     dataLines; /* data lines */
    BOOL                     isTft;     /* TFT or STN */
    } GFX_TI_DISPC_LCD_INFO;

typedef struct
    {
    UINT32              fbAddress;
    UINT32              rowInc;
    UINT32              pixelInc;
    UINT32              xres;
    UINT32              yres;
    UINT32              format;
    } GFX_TI_DISPC_PIPE_INFO;

typedef struct
    {
    GFX_TI_DISPC_LCD_INFO           lcdInfo;
    GFX_TI_DISPC_PIPE_INFO          pipeInfo;
    GFX_TI_DISPC_PIPE               pipe;
    } GFX_TIDSS_INFO;

#define GFX_TI_SMS_BASE                         (0x6C000000)
#define GFX_TI_SMS_ROT_CH0_CONTROL              (GFX_TI_SMS_BASE + 0x180)
#define GFX_TI_SMS_ROT_CH0_SIZE                 (GFX_TI_SMS_BASE + 0x184)
#define GFX_TI_SMS_ROT_CH0_PHYSICAL_BA          (GFX_TI_SMS_BASE + 0x188)
#define GFX_TI_SMS_ROT_CH0_0_DEG                (0x70000000)
#define GFX_TI_SMS_ROT_CH0_90_DEG               (0x71000000)
#define GFX_TI_SMS_ROT_CH0_270_DEG              (0x73000000)

/* SMS rotation control register */

#define GFX_TI_SMS_ROT_CONTROL_PH               8
#define GFX_TI_SMS_ROT_CONTROL_PW               4
#define GFX_TI_SMS_ROT_CONTROL_PS               0

/* SMS rotation size register */

#define GFX_TI_SMS_ROT_SIZE_HEIGHT              16
#define GFX_TI_SMS_ROT_SIZE_WIDTH               0

#define GFX_SMS_REG_SIZE                        (0x10000)
#define GFX_SMS_ROT_WIDTH                       (2048)
#define GFX_SMS_ROT_HEIGHT                      (2048)

#define GFX_TI_DSS_VID1_BA0                     ((unsigned int)virtDispcAddr + 0x0BC)
#define GFX_TI_DSS_VID1_POSITION                ((unsigned int)virtDispcAddr + 0x0C4)
#define GFX_TI_DSS_VID1_SIZE                    ((unsigned int)virtDispcAddr + 0x0C8)
#define GFX_TI_DSS_VID1_ATTRIBUTES              ((unsigned int)virtDispcAddr + 0x0CC)
#define GFX_TI_DSS_VID1_FIFO_THRESHOLD          ((unsigned int)virtDispcAddr + 0x0D0)
#define GFX_TI_DSS_VID1_ROW_INC                 ((unsigned int)virtDispcAddr + 0x0D8)
#define GFX_TI_DSS_VID1_PIXEL_INC               ((unsigned int)virtDispcAddr + 0x0DC)
#define GFX_TI_DSS_VID1_PICTURE_SIZE            ((unsigned int)virtDispcAddr + 0x0E4)

/* position of the VID1 window register */

#define GFX_TI_DSS_VID1_POSITION_Y              16
#define GFX_TI_DSS_VID1_POSITION_X              0

/* size of the VID1 window register */

#define GFX_TI_DSS_VID1_SIZE_Y                  16
#define GFX_TI_DSS_VID1_SIZE_X                  0

/* VID1 attributes register */

#define GFX_TI_DSS_VID1_ATTR_BURSTSIZE          14
#define GFX_TI_DSS_VID1_ATTR_REPLICATION        10
#define GFX_TI_DSS_VID1_ATTR_FORMAT             1
#define GFX_TI_DSS_VID1_ATTR_ENABLE             0

/* VID1 FIFO register */

#define GFX_TI_DSS_VID1_FIFO_HIGH               16
#define GFX_TI_DSS_VID1_FIFO_LOW                0

/* VID1 configration */

#define VID1_FIFO_THRESHOLD_HIGH            (0xFC0)
#define VID1_FIFO_THRESHOLD_LOW             (0x800)
#define VID1_BURST_SIZE                     (0x2)
#define VID1_POSITION_Y_MAX                 (0x7FF)
#define VID1_POSITION_X_MAX                 (0x7FF)

#endif /* __INCgfxTiDssH */
