/* bsp.cdf - Board definition description file */

/*
 * Copyright (c) 2015-2016, 2018-2019 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
21aug19,jff  added generic component dependencies (VXWPG-248)
14mar19,mpc  added CORTEX series CPU type definitions (F11131)
03aug18,swu  change DRV_ARMV7_GEN_TIMER to DRV_ARM_GEN_TIMER (F10748)
17jul18,mpc  BSP rename (F10748)
28jun18,mze  specify MP_OPTIONS properly
24apr18,cfm  renamed FP type vector to hard (F8170)
04may16,m_w  update clock system, replace DRV_CLK_FDT_AM5
             with DRV_CLK_FDT_AM4. (F7158)
26sep15,yjw  created (US64529)
*/

Bsp ti_sitara_ctxa15_branson {
    NAME                am572x board support package
    CPU                 ARMARCH7 CORTEX_A15 
    ENDIAN              little
    MP_OPTIONS          SMP UP
    DATA_MODEL          ILP32
    BUILD_SPECS         default
    BUILD_TARGETS       uVxWorks vxWorks vxWorks.bin
    FP                  hard soft
    REQUIRES            INCLUDE_KERNEL                  \
                        INCLUDE_CPU_INIT                \
                        INCLUDE_MEM_DESC                \
                        BOARD_SELECTION                 \
                        INCLUDE_BOARD_LIB_INIT          \
                        DRV_BUS_FDT_SIMPLE              \
                        INCLUDE_EARLY_MMU               \
                        INCLUDE_DEVCLK_SYS              \
                        DRV_INTCTLR_FDT_ARM_GIC         \
                        INCLUDE_TIMER_SYS               \
                        INCLUDE_TIMESTAMP64             \
                        INCLUDE_AUX_CLK                 \
                        INCLUDE_CACHE_SUPPORT           \
                        INCLUDE_PINMUX_SYS              \
                        INCLUDE_SIO                     \
                        INCLUDE_USB                     \
                        INCLUDE_SD_BUS                  \
                        DRV_AM572x_QSPI                 \
                        INCLUDE_TI_DCAN                 \
                        BRANSON_DRV_TI_WDT              \
                        BRANSON_DRV_TI_EQEP             \
                        BRANSON_DRV_TI_EHRPWM           \
                        BRANSON_DRV_TI_MCSPI            \
                        BRANSON_DRV_LOGGER_MODULE       \
                        INCLUDE_MICREL_PHY              \
                        INCLUDE_EEPROMDRV               \
                        INCLUDE_RTP                     \
                        INCLUDE_SHELL                   \
                        INCLUDE_IFCONFIG                \
                        INCLUDE_DISK_UTIL
}
