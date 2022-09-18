/* gfxTiAm37xx.h - TI OMAP35XX/AM37XX graphics driver header file */

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
*/

#ifndef __INCgfxTiAm37xxH
#define __INCgfxTiAm37xxH

/* defines */

/* regsiters address */

#define OMAP35XX_CM_WKUP_BASE                    (0x48004C00)
#define OMAP35XX_CLOCK_MANAGER_FCLKEN_WKUP       (OMAP35XX_CM_WKUP_BASE + 0x000)
#define OMAP35XX_CLOCK_MANAGER_ICLKEN_WKUP       (OMAP35XX_CM_WKUP_BASE + 0x010)

/* pad configurate registers*/

#define OMAP35XX_CONTROL_PADCONF_DSS_D0          (0x480020DC)
#define OMAP35XX_CONTROL_PADCONF_DSS_D1          (0x480020DE)
#define OMAP35XX_CONTROL_PADCONF_DSS_D2          (0x480020E0)
#define OMAP35XX_CONTROL_PADCONF_DSS_D3          (0x480020E2)
#define OMAP35XX_CONTROL_PADCONF_DSS_D4          (0x480020E4)
#define OMAP35XX_CONTROL_PADCONF_DSS_D5          (0x480020E6)
#define OMAP35XX_CONTROL_PADCONF_DSS_D6          (0x480020E8)
#define OMAP35XX_CONTROL_PADCONF_DSS_D7          (0x480020EA)
#define OMAP35XX_CONTROL_PADCONF_DSS_D8          (0x480020EC)
#define OMAP35XX_CONTROL_PADCONF_DSS_D9          (0x480020EE)
#define OMAP35XX_CONTROL_PADCONF_DSS_D10         (0x480020F0)
#define OMAP35XX_CONTROL_PADCONF_DSS_D11         (0x480020F2)
#define OMAP35XX_CONTROL_PADCONF_DSS_D12         (0x480020F4)
#define OMAP35XX_CONTROL_PADCONF_DSS_D13         (0x480020F6)
#define OMAP35XX_CONTROL_PADCONF_DSS_D14         (0x480020F8)
#define OMAP35XX_CONTROL_PADCONF_DSS_D15         (0x480020FA)
#define OMAP35XX_CONTROL_PADCONF_DSS_D16         (0x480020FC)
#define OMAP35XX_CONTROL_PADCONF_DSS_D17         (0x480020FE)
#define OMAP35XX_CONTROL_PADCONF_DSS_D18         (0x48002100)
#define OMAP35XX_CONTROL_PADCONF_DSS_D19         (0x48002102)
#define OMAP35XX_CONTROL_PADCONF_DSS_D20         (0x48002104)
#define OMAP35XX_CONTROL_PADCONF_DSS_D21         (0x48002106)
#define OMAP35XX_CONTROL_PADCONF_DSS_D22         (0x48002108)
#define OMAP35XX_CONTROL_PADCONF_DSS_D23         (0x4800210A)

#define OMAP35XX_CONTROL_PADCONF_DSS_D18_2       (0x48002A0A)
#define OMAP35XX_CONTROL_PADCONF_DSS_D19_2       (0x48002A0C)
#define OMAP35XX_CONTROL_PADCONF_DSS_D20_2       (0x48002A10)
#define OMAP35XX_CONTROL_PADCONF_DSS_D21_2       (0x48002A12)
#define OMAP35XX_CONTROL_PADCONF_DSS_D22_2       (0x48002A14)
#define OMAP35XX_CONTROL_PADCONF_DSS_D23_2       (0x48002A16)

#define OMAP35XX_CONTROL_PADCONF_DSS_PCLK        (0x480020D4)
#define OMAP35XX_CONTROL_PADCONF_DSS_VSYNC       (0x480020D8)
#define OMAP35XX_CONTROL_PADCONF_DSS_HSYNC       (0x480020D6)
#define OMAP35XX_CONTROL_PADCONF_DSS_ACBIAS      (0x480020DA)

#define OMAP35XX_CONTROL_PADCONF_LCD_INI         (0x48002184)
#define OMAP35XX_CONTROL_PADCONF_LCD_ENVDD       (0x48002186)
#define OMAP35XX_CONTROL_PADCONF_LCD_QVGA        (0x48002188)
#define OMAP35XX_CONTROL_PADCONF_LCD_RESB        (0x4800218A)
#define OMAP35XX_CONTROL_PADCONF_LCD_LR          (0x48002A0A)
#define OMAP35XX_CONTROL_PADCONF_LCD_UD          (0x48002A0C)

/* GPIO registers */

#define OMAP35XX_GPIO1                           (0x48310000)
#define OMAP35XX_GPIO5                           (0x49056000)

/* registers bits offset */

/* WKUP_CM register */

#define OMAP35XX_CM_FCLKEN_WKUP_EN_GPIO1         3
#define OMAP35XX_CM_ICLKEN_WKUP_EN_GPIO1         3

/* configuration */

/* hardware rotation configuration */

#define OMAP35XX_SMS_ROT_PH                      (0x3)
#define OMAP35XX_SMS_ROT_PW                      (0x3)

/* GPIO configuration */

#define OMAP35XX_LCD_INI_BIT                     24 /* PIN 152, GPIO5, bit 24 */
#define OMAP35XX_LCD_ENVDD_BIT                   25 /* PIN 153, GPIO5, bit 25 */
#define OMAP35XX_LCD_QVGA_NVGA_BIT               26 /* PIN 154, GPIO5, bit 26 */
#define OMAP35XX_LCD_RESET_BIT                   27 /* PIN 155, GPIO5, bit 27 */
#define OMAP35XX_LCD_LR_BIT                      2  /* PIN   2, GPIO1, bit  2 */
#define OMAP35XX_LCD_UD_BIT                      3  /* PIN   3, GPIO1, bit  3 */

#define OMAP35XX_GPIO_OE                         (0x34)
#define OMAP35XX_GPIO_DATAOUT                    (0x3C)

/* pin mux configuration */

#define OMAP35XX_CONTROL_PADCONF_PIN_MASK        (0xFFF8)
#define OMAP35XX_CONTROL_PADCONF_PIN_OFF         (0x0007)
#define OMAP35XX_CONTROL_PADCONF_GPIO_ON         (0x001C)
#define OMAP35XX_CONTROL_PADCONF_LCD_ON          (0x0018)

/* power IC address */

#define OMAP35XX_POWER_IC_IO_ADDR                (0x49)
#define OMAP35XX_POWER_IC_LED_ADDR               (0x4A)
#define OMAP35XX_POWER_IC_PM_ADDR                (0x4B)

#define OMAP35XX_POWER_IC_IO_DIR                 (0x9B)
#define OMAP35XX_POWER_IC_IO_DATA                (0x9E)

#define OMAP35XX_POWER_IC_LED_LEDEN              (0xEE)
#define OMAP35XX_POWER_IC_LED_PWMAON             (0xEF)
#define OMAP35XX_POWER_IC_LED_PWMAOFF            (0xF0)

#define OMAP35XX_POWER_IC_VPLL2_DEV_GRP          (0x8E)
#define OMAP35XX_POWER_IC_VPLL2_DEDICATED        (0x91)

#define OMAP35XX_LCD_QVGA_WIDTH                  240
#define OMAP35XX_LCD_QVGA_HEIGHT                 320

#endif /* __INCgfxTiAm37xxH */
