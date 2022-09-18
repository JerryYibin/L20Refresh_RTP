/* 40vxbTps62362.cdf - Component Definition file for I2C VOLTAGE REGULATOR */

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
01a,01JUNE17  created
*/
	
Component       BRANSON_DRV_I2C_TPS62362_VOLTAGE_REGULATOR {
    NAME        I2c voltage regulator Driver
    SYNOPSIS    vxBus Driver for I2c voltage regulator
    MODULES     vxbI2cTps62362.o
    LINK_SYMS   vxbFdtI2cTps62362Drv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}
