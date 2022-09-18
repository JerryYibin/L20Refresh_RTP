/* 40vxbBransonTiDcan.cdf - TI DCAN controller driver description file */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This file contains descriptions for the TI DCAN components.
*/
Component INCLUDE_SOCKETCAN_RECV {
    NAME            Socket CAN Library including receiving
    SYNOPSIS        Socket CAN Library including receiving
    MODULES         socketCAN.o
    REQUIRES        INCLUDE_SOCKETCAN
    _CHILDREN       FOLDER_CAN_CORE
    CFG_PARAMS      CAN_RECV_ENABLE
    PROTOTYPE       extern void canSoRecvEnable(BOOL);
    INIT_RTN        canSoRecvEnable(CAN_RECV_ENABLE);
    _INIT_ORDER     usrRoot
    INIT_AFTER      INCLUDE_SOCKLIB
    INIT_BEFORE     INCLUDE_SOCKETCAN
}
Parameter CAN_RECV_ENABLE {
    NAME        Branson Can Socket Receiving
    TYPE        BOOL
    SYNOPSIS    enable the package receiving from CAN controller
    DEFAULT     TRUE
}