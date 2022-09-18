/* socketCAN.h - CAN socket library header file */

/*
 * Copyright (c) 2013-2014, 2020 Wind River Systems, Inc.
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

#ifndef __INCsocketCANh
#define __INCsocketCANh

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef unsigned char sa_family_t;

#define AF_CAN              34
#define PF_CAN              AF_CAN
#define CAN_RAW             1
#define CAN_BCM             2
#define CSS_READY           1
#define CSS_DISCONNECED     2
#define CAN_INV_FILTER      0x20000000U
#define PACKET_HOST         0
#define PACKET_LOOPBACK     1
#define SOL_CAN_RAW         101
enum
    {
    CAN_RAW_FILTER = 1,
    CAN_RAW_ERR_FILTER,
    CAN_RAW_LOOPBACK,
    CAN_RAW_RECV_OWN_MSGS,
    CAN_RAW_FD_FRAMES,
    };
typedef struct sockaddr_can
    {
    sa_family_t can_family;
    int        can_ifindex;
    union
        {
        struct
            {
            UINT32 rx_id;
            UINT32 tx_id;
            }tp;
        } can_addr;
    }SOCKADDR_CAN;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__INCsocketCANh */

