/* canDevLib.h - CAN device library header file */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05may22,ykm  created.
*/

#ifndef __INCcanDevLibh
#define __INCcanDevLibh

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CAN_EFF_FLAG    0x80000000
#define CAN_RTR_FLAG    0x40000000
#define CAN_ERR_FLAG    0x20000000
#define CAN_SFF_MASK    0x000007FF
#define CAN_EFF_MASK    0x1fffffff
#define CAN_ERR_MASK    0x1fffffff

typedef struct can_frame
    {
    UINT32 can_id;
    UINT8  can_dlc;
    UINT8  pad;
    UINT8  res0;
    UINT8  res1;
    UINT8  data[8];
    }CAN_MSG;
extern int canDevCount();
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INCcanDevLibh */

