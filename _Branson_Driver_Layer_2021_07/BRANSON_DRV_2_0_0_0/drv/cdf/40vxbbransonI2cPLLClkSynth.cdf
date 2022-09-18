/* 40vxbI2cPLLClkSynth.cdf - Component Definition file for I2C Clock Synthesizer */

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

Component       BRANSON_DRV_I2C_PLL_CLK_SYNTH {
    NAME        I2c PLL Clock Synthesizer Driver
    SYNOPSIS    vxBus Driver for I2c clk synthesizer
    MODULES     vxbI2cPLLClkSynth.o
    LINK_SYMS   vxbFdtI2cPLLClkSynthDrv
    REQUIRES    INCLUDE_VXBUS
    _CHILDREN   FOLDER_DRIVERS
}