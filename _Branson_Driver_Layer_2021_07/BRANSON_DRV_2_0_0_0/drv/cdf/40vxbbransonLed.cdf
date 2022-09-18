/* 05eeprom.cdf - eeprom device component description */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
22dec15,y_y  move it from vxbus core layer cdf folder to here. (V7PRO-2620)
07feb12,y_y  written.
*/

Component BRANSON_INCLUDE_LEDDRV {
    NAME        LED device driver
    SYNOPSIS    Attach the LED device to standard I/O 
    PROTOTYPE   STATUS ledDrv (void);
    INIT_RTN    ledDrv ();
    _INIT_ORDER usrIosCoreInit
    INIT_AFTER  INCLUDE_TTY_DEV
    _CHILDREN   FOLDER_PERIPHERALS
}
