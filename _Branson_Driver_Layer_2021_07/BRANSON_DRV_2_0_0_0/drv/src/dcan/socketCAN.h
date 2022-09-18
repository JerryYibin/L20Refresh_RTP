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
19mar20,sun  CAN FD supprot (VXWPG-451)
17may14,hcl  CAN BCM support (US15988)
15may14,hcl  update for US32965
07mar13,hcl  created
*/

#ifndef __INCsocketCANh
#define __INCsocketCANh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AF_CAN              34
#define PF_CAN              AF_CAN

#define CAN_RAW             1
#define CAN_BCM             2

#define CSS_READY           1   /* socket is connectionless */
#define CSS_DISCONNECED     2   /* socket is no longer connected */

#define CAN_INV_FILTER      0x20000000U /* to be set in can_filter.can_id */

#define PACKET_HOST         0
#define PACKET_LOOPBACK     1

/* RAW */

#define SOL_CAN_RAW         101

enum {
    CAN_RAW_FILTER = 1,      /* set 0 .. n can_filter(s) */
    CAN_RAW_ERR_FILTER,      /* set filter for error frames */
    CAN_RAW_LOOPBACK,        /* local loopback (default:on) */
    CAN_RAW_RECV_OWN_MSGS,   /* receive my own msgs (default:off) */
    CAN_RAW_FD_FRAMES,       /* allow CAN FD frames (default:off) */
    };

/* Broadcast Manager (BCM) */

struct bcm_msg_head
    {
    unsigned int        opcode;
    unsigned int        flags;
    unsigned int        count;
    struct timeval      ival1, ival2;
    canid_t             can_id;
    unsigned int        nframes;
    struct can_frame    frames[0];
    };

enum {
    TX_SETUP = 1,
    TX_DELETE,
    TX_READ,
    TX_SEND,
    RX_SETUP,
    RX_DELETE,
    RX_READ,
    TX_STATUS,
    TX_EXPIRED,
    RX_STATUS,
    RX_TIMEOUT,
    RX_CHANGED
    };

#define SETTIMER            0x0001
#define STARTTIMER          0x0002
#define TX_COUNTEVT         0x0004
#define TX_ANNOUNCE         0x0008
#define TX_CP_CAN_ID        0x0010
#define RX_FILTER_ID        0x0020
#define RX_CHECK_DLC        0x0040
#define RX_NO_AUTOTIMER     0x0080
#define RX_ANNOUNCE_RESUME  0x0100
#define TX_RESET_MULTI_IDX  0x0200
#define RX_RTR_FRAME        0x0400

/* CAN list */

typedef struct can_list
    {
    LIST            lst;
    SEM_ID          sem;
    } CAN_LIST;

/* CAN filter */

typedef struct can_filter
    {
    unsigned int    can_id;
    unsigned int    can_mask;
    } CAN_FILTER;

typedef struct can_filter_node
    {
    NODE            node;
    CAN_FILTER      filter;
    } CAN_FILTER_NODE;

/* socket CAN */

typedef struct socket_can
    {
    NODE            node;       /* node */
    void          * so;         /* socket */
    void          * dev;        /* CAN device */
    int             devState;   /* CAN device state */
    int             timeout;    /* receive timeout */
    volatile int    loopback;   /* loop */
    CAN_LIST        lstMsg;     /* package list */
    SEM_ID          semRcv;     /* receive sem */
    SEM_ID          semPolicy;  /* policy sem */
    volatile BOOL   rcvOwnMsg;  /* receive own message */
    unsigned int    errMask;    /* error frame mask */
    LIST            lstFilter;  /* filter list */
    volatile UINT8  rawFdFrame; /* whether CAN FD frame */
    } SOCKET_CAN;

/* socket CAN address */

typedef struct sockaddr_can
    {
    sa_family_t     can_family;
    int             can_ifindex;

    union
        {
        /* transport protocol class address info (e.g. ISOTP) */

        struct {unsigned int rx_id, tx_id;} tp;

        /* reserved for future CAN protocols address information */

        } can_addr;
    } SOCKADDR_CAN;

/* CAN message node */

typedef struct can_msg_node
    {
    NODE               node;   /* node */
    int                type;   /* type */
    void             * soc;    /* socket CAN */
    void             * dev;    /* device */
    CAN_FD_MSG         msg;    /* CAN message */
    } CAN_MSG_NODE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__INCsocketCANh */
