/* gfxTiAm35xx.h - TI AM35XX graphics driver header file */

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
26mar13,a_f Written
*/

#ifndef __INCgfxTiAm35xxH
#define __INCgfxTiAm35xxH

/* defines */

#define AM35XX_BIT_ENABLE                    (0x1)
#define AM35XX_BIT_DISABLE                   (0x0)

#define AM35XX_DISPC                         (0x48050400)
#define AM35XX_PINMUX_BASE                   (0x48002000)

#define AM35XX_PIN_BITS_MASK                 (0x10)

#define AM35XX_PIN_DSS_D0_ADDR               (AM35XX_PINMUX_BASE + 0xDC)
#define AM35XX_PIN_DSS_D1_ADDR               (AM35XX_PINMUX_BASE + 0xDE)
#define AM35XX_PIN_DSS_D2_ADDR               (AM35XX_PINMUX_BASE + 0xE0)
#define AM35XX_PIN_DSS_D3_ADDR               (AM35XX_PINMUX_BASE + 0xE2)
#define AM35XX_PIN_DSS_D4_ADDR               (AM35XX_PINMUX_BASE + 0xE4)
#define AM35XX_PIN_DSS_D5_ADDR               (AM35XX_PINMUX_BASE + 0xE6)
#define AM35XX_PIN_DSS_D6_ADDR               (AM35XX_PINMUX_BASE + 0xE8)
#define AM35XX_PIN_DSS_D7_ADDR               (AM35XX_PINMUX_BASE + 0xEA)
#define AM35XX_PIN_DSS_D8_ADDR               (AM35XX_PINMUX_BASE + 0xEC)
#define AM35XX_PIN_DSS_D9_ADDR               (AM35XX_PINMUX_BASE + 0xEE)
#define AM35XX_PIN_DSS_D10_ADDR              (AM35XX_PINMUX_BASE + 0xF0)
#define AM35XX_PIN_DSS_D11_ADDR              (AM35XX_PINMUX_BASE + 0xF2)
#define AM35XX_PIN_DSS_D12_ADDR              (AM35XX_PINMUX_BASE + 0xF4)
#define AM35XX_PIN_DSS_D13_ADDR              (AM35XX_PINMUX_BASE + 0xF6)
#define AM35XX_PIN_DSS_D14_ADDR              (AM35XX_PINMUX_BASE + 0xF8)
#define AM35XX_PIN_DSS_D15_ADDR              (AM35XX_PINMUX_BASE + 0xFA)
#define AM35XX_PIN_DSS_D16_ADDR              (AM35XX_PINMUX_BASE + 0xFC)
#define AM35XX_PIN_DSS_D17_ADDR              (AM35XX_PINMUX_BASE + 0xFE)
#define AM35XX_PIN_DSS_D18_ADDR              (AM35XX_PINMUX_BASE + 0x100)
#define AM35XX_PIN_DSS_D19_ADDR              (AM35XX_PINMUX_BASE + 0x102)
#define AM35XX_PIN_DSS_D20_ADDR              (AM35XX_PINMUX_BASE + 0x104)
#define AM35XX_PIN_DSS_D21_ADDR              (AM35XX_PINMUX_BASE + 0x106)
#define AM35XX_PIN_DSS_D22_ADDR              (AM35XX_PINMUX_BASE + 0x108)
#define AM35XX_PIN_DSS_D23_ADDR              (AM35XX_PINMUX_BASE + 0x10A)
#define AM35XX_PIN_DSS_VSYNC_ADDR            (AM35XX_PINMUX_BASE + 0xD8)
#define AM35XX_PIN_DSS_AC_ADDR               (AM35XX_PINMUX_BASE + 0xDA)
#define AM35XX_PIN_DSS_PCLK_ADDR             (AM35XX_PINMUX_BASE + 0xD4)
#define AM35XX_PIN_DSS_HSYNC_ADDR            (AM35XX_PINMUX_BASE + 0xD6)
#define AM35XX_PIN_DSS_PWM_ADDR              (AM35XX_PINMUX_BASE + 0x1DC)
#define AM35XX_PIN_DSS_BL_PWR_ADDR           (AM35XX_PINMUX_BASE + 0x1DE)
#define AM35XX_PIN_DSS_PWR_ADDR              (AM35XX_PINMUX_BASE + 0x1D2)

#define AM35XX_PAD_CONF_MODE_OFFSET          (0x0)
#define AM35XX_PAD_CONF_MODE_MASK            (0x7)
#define AM35XX_PAD_CONF_MODE0_VALUE          (0x0 << 0x0)
#define AM35XX_PAD_CONF_MODE4_VALUE          (0x4 << 0x0)

#define AM35XX_PAD_CONF_PULLUD_OFFSET        (0x3)
#define AM35XX_PAD_CONF_PULLUD_MASK          (0x1)
#define AM35XX_PAD_CONF_PULLUD_EN_VALUE      (0x1 << 0x3)
#define AM35XX_PAD_CONF_PULLUD_DIS_VALUE     (0x0 << 0x3)

#define AM35XX_PAD_CONF_PULLTYPE_OFFSET      (0x4)
#define AM35XX_PAD_CONF_PULLTYPE_MASK        (0x1)
#define AM35XX_PAD_CONF_PULLTYPE_UP_VALUE    (0x1 << 0x4)
#define AM35XX_PAD_CONF_PULLTYPE_DOWN_VALUE  (0x0 << 0x4)

#define AM35XX_PAD_CONF_INPUT_OFFSET         (0x8)
#define AM35XX_PAD_CONF_INPUT_MASK           (0x1)
#define AM35XX_PAD_CONF_INPUT_EN_VALUE       (0x1 << 0x8)
#define AM35XX_PAD_CONF_INPUT_DIS_VALUE      (0x0 << 0x3)

#define AM35XX_PIN_DSS_D0_VALUE      (AM35XX_PAD_CONF_MODE0_VALUE | \
                                     (AM35XX_PAD_CONF_PULLUD_EN_VALUE) | \
                                     (AM35XX_PAD_CONF_PULLTYPE_DOWN_VALUE) | \
                                     (AM35XX_PAD_CONF_INPUT_EN_VALUE))
#define AM35XX_PIN_DSS_D1_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D2_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D3_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D4_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D5_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D6_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D7_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D8_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D9_VALUE              AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D10_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D11_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D12_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D13_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D14_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D15_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D16_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D17_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D18_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D19_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D20_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D21_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D22_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_D23_VALUE             AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_VSYNC_VALUE           AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_HSYNC_VALUE           AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_PCLK_VALUE            AM35XX_PIN_DSS_D0_VALUE
#define AM35XX_PIN_DSS_AC_VALUE              AM35XX_PIN_DSS_D0_VALUE

#define AM35XX_PIN_DSS_PWR_VALUE    ((AM35XX_PAD_CONF_MODE4_VALUE) | \
                                    (AM35XX_PAD_CONF_PULLUD_EN_VALUE) | \
                                    (AM35XX_PAD_CONF_PULLTYPE_UP_VALUE) | \
                                    (AM35XX_PAD_CONF_INPUT_EN_VALUE))

#define AM35XX_PIN_DSS_PWM_VALUE    ((AM35XX_PAD_CONF_MODE4_VALUE) | \
                                    (AM35XX_PAD_CONF_PULLUD_EN_VALUE) | \
                                    (AM35XX_PAD_CONF_INPUT_EN_VALUE))
#define AM35XX_PIN_DSS_BL_PWR_VALUE         AM35XX_PIN_DSS_PWM_VALUE

#define AM35XX_GPIO6_ADDR                   (0x49058000)
#define AM35XX_GPIO6_OE_ADDR                (AM35XX_GPIO6_ADDR + 0x34)
#define AM35XX_GPIO6_DATAOUT_ADDR           (AM35XX_GPIO6_ADDR + 0x3c)

#define AM35XX_GPIO6_LCD_PWR_OFFSET         (0x10)    /* 176 */
#define AM35XX_GPIO6_LCD_BL_PWR_OFFSET      (0x16)    /* 182 */
#define AM35XX_GPIO6_LCD_PWM_OFFSET         (0x15)    /* 181 */

#define AM35XX_DISPC_CONTROL                     (AM35XX_DISPC + 0x40)
#define AM35XX_DISPC_CONTROL_LCDENABLE_VALUE     (0x1 << 0x0)

/* hardware rotation configuration */

#define AM35XX_SMS_ROT_PH                      (0x3)
#define AM35XX_SMS_ROT_PW                      (0x3)

/* get system clock */

#define AM35XX_PRM_CLKSEL                        (0x48306d40)
#define AM35XX_PRM_CLKSRC_CTRL                   (0x48307270)

#endif /* __INCgfxTiAm35xxH */
