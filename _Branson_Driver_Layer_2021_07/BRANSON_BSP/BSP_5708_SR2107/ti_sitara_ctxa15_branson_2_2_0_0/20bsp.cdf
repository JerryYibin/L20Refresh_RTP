/* 20board.cdf - Board component description file */

/*
 * Copyright (c) 2015, 2017-2020 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
23mar20,k_s  removed parameters NUM_TTY and CONSOLE_TTY (V7COR-7004)
09feb20,p_x  added INCLUDE_VXBUS_RTC_LIB component (VXWPG-159)
12dec19,jnl  added usage restrictions on INCLUDE_MEM_LAYOUT_CHECK (V7PRO-5879)
21aug19,jff  added generic component dependencies (VXWPG-248)
26dec18,jnl  added INCLUDE_MEM_LAYOUT_CHECK to check the memory layout when
             build vip (V7PRO-5161)
24oct18,whu  set parameter XHCI_DWC3_TI_ENABLE
02aug17,d_l  update for SD UHS support (F9151)
24sep15,yjw  created (US64529)
*/

Selection BOARD_SELECTION {
    NAME        Board Selection
    SYNOPSIS    This BSP supports multiple boards. Choose your desired hardware \
                from this list.
    COUNT       1-1
    CHILDREN    INCLUDE_TI_AM5728_BRANSON       \
                INCLUDE_TI_AM5708_BRANSON 		\
                INCLUDE_TI_AM5708_JN
    DEFAULTS    INCLUDE_TI_AM5708_BRANSON
    _CHILDREN   FOLDER_BSP_CONFIG
}

Component INCLUDE_TI_AM5728_BRANSON {
    NAME        TI AM5728 Branson Board
    SYNOPSIS    This component is used to select the configuration of \
                TI AM5728 Branson Board.
}

Component INCLUDE_TI_AM5708_BRANSON {
    NAME        TI AM5708 Branson Board
    SYNOPSIS    This component is used to select the configuration of \
                TI AM5708 Branson Board.
}

Component INCLUDE_TI_AM5708_JN {
	NAME		TI AM5708 JN EV Board
	SYNOPSIS	This component is used to select the configuration of \
				TI AM5708 JN EV Board
	/* Add the code only for crystal oscillator */
    CFG_PARAMS  USE_192M_CLK
    INIT_RTN    clkSet192(USE_192M_CLK);
    PROTOTYPE   extern void clkSet192(BOOL);
    INIT_AFTER  INCLUDE_VXB_AUX_CLK
    _INIT_ORDER usrClkInit
}

/* Add the code only for crystal oscillator */
Parameter USE_192M_CLK {
    NAME        input clock of SYS_CLKSEL uses 19.2MHz or 20MHz
    TYPE        BOOL
    DEFAULT     TRUE
}

/********************************************/

Component DRV_ARM_GEN_TIMER_SYSTIMERDEV {
    REQUIRES        DRV_ARM_GEN_TIMER
    INCLUDE_WHEN    INCLUDE_TIMER_SYS
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Component DRV_ARM_GEN_TIMER_TIMESTAMPDEV {
    REQUIRES        DRV_ARM_GEN_TIMER
    INCLUDE_WHEN    INCLUDE_SYS_TIMESTAMP
    _CHILDREN       FOLDER_NOT_VISIBLE
}

Component DRV_TIMER_FDT_OMAP3 {
    INCLUDE_WHEN    INCLUDE_VXB_AUX_CLK
}

Component DRV_SIO_TI_AM3 {
    INCLUDE_WHEN    INCLUDE_SIO
}

Component DRV_PINMUX_FDT_AM3 {
    INCLUDE_WHEN    INCLUDE_PINMUX_SYS
}

Component DRV_CLK_FDT_AM4 {
    INCLUDE_WHEN    INCLUDE_DEVCLK_SYS
}

Component DRV_END_FDT_TI_CPSW {
    INCLUDE_WHEN    INCLUDE_END
}

Component INCLUDE_IPSSL {
    INCLUDE_WHEN    INCLUDE_END
}

Component DRV_TI_EDMA3 {
    INCLUDE_WHEN    INCLUDE_DMA_SYS
}

Component DRV_USB_DWC3_TI {
    INCLUDE_WHEN    INCLUDE_USB
}

Component INCLUDE_USB_PHY_TI_AM572X {
    INCLUDE_WHEN    INCLUDE_USB
}

Component INCLUDE_EXTPWR_GPIO_ENA {
        NAME            EXT PWR GPIO enable
        SYNOPSIS        This component enables the externel power GPIO.
        _INIT_ORDER     usrRoot
        INIT_RTN		\
                        {\
                            int status; \
                            printf("IMG1.. Enable EXT PWR GPIO\n"); \
                            status = vxbGpioAlloc(0x69); \
                            printf("IMG1.. GPIO allocation status: %d\n", status); \
                            status = vxbGpioSetDir(0x69,1); \
                            printf("IMG1.. GPIO direction set status: %d\n", status); \
                        }
        INIT_BEFORE		INCLUDE_USER_APPL
        INCLUDE_WHEN	INCLUDE_USB && INCLUDE_TI_AM5728_BRANSON
        _CHILDREN       FOLDER_NOT_VISIBLE
}

Component INCLUDE_USB_GEN2_STORAGE_INIT {
    INCLUDE_WHEN    INCLUDE_USB
}

Component INCLUDE_DOSFS {
    INCLUDE_WHEN    INCLUDE_USB_GEN2_STORAGE_INIT
}

Component DRV_GPIO_FDT_OMAP {
    INCLUDE_WHEN    INCLUDE_GPIO_SYS
}

Component DRV_TI_MMCHS_CTRL {
    INCLUDE_WHEN    INCLUDE_SD_BUS
}

Component DRV_SDSTORAGE_CARD {
    INCLUDE_WHEN    INCLUDE_SD_BUS
}

Component DRV_MMCSTORAGE_CARD {
    INCLUDE_WHEN    INCLUDE_SD_BUS
}

Component INCLUDE_DOSFS {
    INCLUDE_WHEN    DRV_SDSTORAGE_CARD
}

Component INCLUDE_HRFS {
    INCLUDE_WHEN    DRV_MMCSTORAGE_CARD
}

Component DRV_I2C_EEPROM {
    INCLUDE_WHEN    INCLUDE_EEPROMDRV
}

Component DRV_AM38XX_I2C {
    INCLUDE_WHEN    DRV_I2C_EEPROM
}

Component DRV_I2C_PCFRTC {
    INCLUDE_WHEN	DRV_AM38XX_I2C
}

Component INCLUDE_LEDDRV {
    REQUIRES 		DRV_I2C_LED
    INCLUDE_WHEN 	DRV_AM38XX_I2C
}

Component INCLUDE_STANDALONE_DTB {
    INCLUDE_WHEN    INCLUDE_VXBUS
}
/* TODO the value of the each board base address should be the same. 
*  The value depended the u-boot that how many memory will be cost when it booting. 
*  Once the Branson customized u-boot is ready, we can redefine the value using u-boot 
*  debugging to to redefine the same empty address as the vxworks start address.  
*  For the work process tracking, please see work item 8516.   
*/
Parameter LOCAL_MEM_PHYS_ADRS {
    NAME        local memory physical base address
    SYNOPSIS    Base physical address. For LPAE, alignment of address must be 2MB.
    DEFAULT     (INCLUDE_TI_AM5728_BRANSON)::(0x84000000)   \
                (INCLUDE_TI_AM5708_BRANSON)::(0x88000000)	\
                (INCLUDE_TI_AM5708_JN)::(0x88000000)
}

Parameter LOCAL_MEM_SIZE {
    NAME        local memory size
    SYNOPSIS    Fixed (static) memory size
    DEFAULT     (INCLUDE_TI_AM5728_BRANSON)::(0x7C000000)   \
                (INCLUDE_TI_AM5708_BRANSON)::(0x38000000)	\
                (INCLUDE_TI_AM5708_JN)::(0x38000000)
}

Parameter DTS_FILE {
    NAME        dts file name to be used
    DEFAULT     (INCLUDE_TI_AM5728_BRANSON)::(am572x_branson_ca15.dts)   \
                (INCLUDE_TI_AM5708_BRANSON)::(am5708_branson_ca15.dts)   \
                (INCLUDE_TI_AM5708_JN)::(am5708_jn_ca15.dts)
}

Parameter IMA_SIZE {
    NAME        IMA(Initial Mapped Area) size
    SYNOPSIS    Initial Mapped Area size.
    DEFAULT     0x10000000
}

Parameter RAM_LOW_ADRS {
    NAME        Runtime kernel entry address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x02000000) \
                0x00100000
}

Parameter KERNEL_LOAD_ADRS {
    NAME        Runtime kernel load address
    DEFAULT     (INCLUDE_BOOT_APP && INCLUDE_TI_AM5728_BRANSON)::(0x86000000) \
                (INCLUDE_BOOT_APP && INCLUDE_TI_AM5708_BRANSON)::(0x8a000000) \
                (INCLUDE_BOOT_APP && INCLUDE_TI_AM5708_JN)::(0x8a000000) \
                (INCLUDE_TI_AM5728_BRANSON)::(0x84100000) \
                (INCLUDE_TI_AM5708_BRANSON)::(0x88100000) \
                (INCLUDE_TI_AM5708_JN)::(0x88100000)
}

Parameter RAM_HIGH_ADRS {
    NAME        Runtime kernel high address
    DEFAULT     (INCLUDE_BOOT_APP)::(0x28000000) \
                0x01000000
}

Parameter ISR_STACK_SIZE {
    NAME        ISR stack size
    SYNOPSIS    ISR Stack size (bytes)
    DEFAULT     0x2000
}

Parameter DEFAULT_BOOT_LINE {
    NAME        default boot line
    SYNOPSIS    Default boot line string
    TYPE        string
    DEFAULT     "cpsw(0,0)host:vxWorks h=192.168.1.1 e=192.168.1.100 u=target pw=vxTarget f=0x0"
}

Parameter CONSOLE_TTY {
    NAME        console serial port
    DEFAULT     0
}

Parameter NUM_TTY {
     NAME       number of serial ports
     DEFAULT    1
}

Parameter CONSOLE_BAUD_RATE {
    NAME        baud rate of console port
    DEFAULT     115200
}

Profile BSP_DEFAULT {
    PROFILES    PROFILE_OS_DEFAULT
}

Component INCLUDE_MEM_LAYOUT_CHECK {
    INCLUDE_WHEN    INCLUDE_KERNEL
    REQUIRES        -= COMPONENT_NOT_SUPPORTED
}

Parameter SYSCLK_TIMER_NAME {
    NAME        System clock device name
    SYNOPSIS    This parameter sets the system clock device. If the value is \
                NULL, the device is auto-selected.
    TYPE        string
    DEFAULT     "armGenTimer"
}

Parameter SYSCLK_TIMER_NUM {
    NAME        System Clock Timer Number
    TYPE        int
    DEFAULT     0
}

Parameter AUXCLK_TIMER_NAME {
    NAME        Auxiliary Clock Device Name (NULL is auto-assign)
    TYPE        string
    DEFAULT     (DRV_TIMER_FDT_OMAP3)::("omap35xxTimer") \
                NULL
}

Parameter AUXCLK_TIMER_NUM {
    NAME        Auxiliary Clock Timer Number
    TYPE        int
    DEFAULT     0
}

Parameter TIMESTAMP_TIMER_NAME {
    NAME        Timestamp device name
    SYNOPSIS    This parameter sets the timestamp device. If the value is NULL,
                the device is auto-selected.
    TYPE        string
    DEFAULT     "armGenTimer"
}

Parameter TIMESTAMP_TIMER_NUM {
    NAME        Timestamp device number
    TYPE        int
    DEFAULT     0
}

Component INCLUDE_VXBUS_RTC_LIB {
    REQUIRES    += DRV_FDT_RTC_AM3
}

