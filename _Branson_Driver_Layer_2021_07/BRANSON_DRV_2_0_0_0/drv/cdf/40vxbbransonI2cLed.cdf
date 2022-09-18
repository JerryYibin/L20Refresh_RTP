/* 40vxbI2cEeprom.cdf - Component Definition file for I2C EEPROM */

/*
 * Copyright (c) 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,23may13,y_y  created
*/

Component       BRANSON_DRV_I2C_LED {
    NAME        I2c led Driver
    SYNOPSIS    vxBus Driver for I2c led
    MODULES     vxbI2cTpicLed.o
    LINK_SYMS   vxbFdtI2cTpicLedDrv
    REQUIRES    INCLUDE_VXBUS BRANSON_INCLUDE_LEDDRV
    _CHILDREN   FOLDER_DRIVERS
}
