/* socketCAN.c - CAN socket library */

/*
 * Copyright (c) 2013-2014, 2016, 2018-2020 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
20jan21,sun  Corrected buffer overrun defects in canSoRecv(,From)
31mar20,dlk  Adapt for struct socket core extraction (V7NET-2699)
19mar20,sun  CAN FD supprot (VXWPG-451)
20feb20,dlk  canSoIoctl() should not call canDevIoctl() with a NULL device
             support FIOSHUTDOWN ioctl
20aug19,gqn  socketCAN internal memory freeSoList exhausted. (V7CON-724)
14aug18,hma  clean up build warnings. (F9305)
22dec16,hma  remove the print info (V7NET-1216)
02dec14,hcl  add SMP support (US46188)
20aug14,hcl  fix static analysis defects (US15981)
31may14,hcl  fix static analysis defects (US15762)
28may14,hcl  remove VXBUS_LEGACY support (V7CON-133)
17may14,hcl  CAN BCM support (US15988)
15may14,hcl  update for US32965
07mar13,hcl  written
*/

/*
DESCRIPTION

This library provides CAN socket calls. These calls may be used to open,
close, read, and write sockets.

ADDRESS FAMILY
CAN sockets support only the PF_CAN domain address family. Use PF_CAN for
the <domain> argument in subroutines that require it.
*/

#include <vxWorks.h>
#include <stdio.h>
#include <tickLib.h>
#include <ioLib.h>
#include <lstLib.h>
#include <string.h>
#include <semLib.h>
#include <selectLib.h>
#include <errno.h>
#include <errnoLib.h>
#include <sockLib.h>
#include <net/socketvar.h>
#include <private/iosLibP.h>
#include <canDevLib.h>
#include <socketCAN.h>

/* defines */

#define MSG_POOL_SIZE       1024
#define MAX_CAN_DEV_NUM     (10)

#define BOMB(code) do {(void)errnoSet(code); return (ERROR);} while ((0))
#define DEV_INDEX(pDev) (((CAN_DEV*)pDev)->ifindex)
#define MEMBER_TO_OBJECT(pMember, type, memberName) \
        ((type *)((char *)(pMember) - offsetof (type, memberName)))

#ifdef _WRS_CONFIG_SERVICE_SOCKET__SOCKETCORE
/* struct socket_bsd is defined by socketvar.h */
#define SOCKBSD_TO_SOCK(x) (&((x)->so_so))
#define SOCK_TO_SOCKBSD(x) ((struct socket_bsd *)(x))
#else
#define socket_bsd socket
#define SOCKBSD_TO_SOCK(x) (x)
#define SOCK_TO_SOCKBSD(x) (x)
#endif /* _WRS_CONFIG_SERVICE_SOCKET__SOCKETCORE */

/* local */

static CAN_LIST   canSoList;
static CAN_LIST * freeSoListArray[MAX_CAN_DEV_NUM] = {NULL};
static BOOL recvEnabled = TRUE;

/* imports */

IMPORT CAN_DEV *    canDevBind();
IMPORT STATUS       canDevSend();
IMPORT CAN_DEV *    canDevGetByIndex();
IMPORT CAN_DEV *    canDevGetByName();
IMPORT unsigned int canDevIoctl();

/* locals*/

LOCAL SOCK_FUNC * canSoInit(void);
LOCAL int canSoSocket(int, int, int, struct socket **);
LOCAL int canSoBind(struct socket *, struct sockaddr *, socklen_t);
LOCAL int canSoSend(struct socket *, const void *, size_t, int);
LOCAL int canSoSendTo(struct socket *, const void *, size_t, int,
                                const struct sockaddr *, socklen_t);
LOCAL int canSoRecv(struct socket *, void *, size_t, int);
LOCAL int canSoRecvFrom(struct socket *, void *, size_t, int,
                                    struct sockaddr *, socklen_t *);
LOCAL int canSoRead(struct socket *, char *, size_t);
LOCAL int canSoWrite(struct socket *, const char *, size_t);
LOCAL int canSoOptSet(struct socket *, int, int, const void *,
                                socklen_t);
LOCAL int canSoOptGet(struct socket *, int, int, void *, socklen_t *);
LOCAL int canSoIoctl(struct socket *, int, _Vx_usr_arg_t, void *);
LOCAL int canSoClose(struct socket *);
LOCAL BOOL canSoIfRcv(void *, int, void *, void *);
LOCAL STATUS bcmConnect(struct socket *, struct sockaddr *, socklen_t);
LOCAL int bcmSendMsg(struct socket *, struct msghdr *, int);
LOCAL int bcmRecvMsg(struct socket *, struct msghdr *, int);

/* define CAN socket interface function table */

SOCK_FUNC canSoFunc =
    {
    (SOCK_FUNC_INIT)        canSoInit,
    (SOCK_FUNC_ACCEPT)      NULL,
    (SOCK_FUNC_BIND)        canSoBind,
    (SOCK_FUNC_CONNECT)     bcmConnect,
    (SOCK_FUNC_CONN_TIMEO)  NULL,
    (SOCK_FUNC_GETPEERNAME) NULL,
    (SOCK_FUNC_GETSOCKNAME) NULL,
    (SOCK_FUNC_LISTEN)      NULL,
    (SOCK_FUNC_RECV)        canSoRecv,
    (SOCK_FUNC_RECVFROM)    canSoRecvFrom,
    (SOCK_FUNC_RECVMSG)     bcmRecvMsg,
    (SOCK_FUNC_SEND)        canSoSend,
    (SOCK_FUNC_SENDTO)      canSoSendTo,
    (SOCK_FUNC_SENDMSG)     bcmSendMsg,
    (SOCK_FUNC_SHUTDOWN)    NULL,
    (SOCK_FUNC_SOCKET)      canSoSocket,
    (SOCK_FUNC_GETSOCKOPT)  canSoOptGet,
    (SOCK_FUNC_SETSOCKOPT)  canSoOptSet,
    (SOCK_FUNC_ZBUF)        NULL,
    (SOCK_FUNC_CLOSE)       canSoClose,
    (SOCK_FUNC_READ)        canSoRead,
    (SOCK_FUNC_WRITE)       canSoWrite,
    (SOCK_FUNC_IOCTL)       canSoIoctl,
    };

/*******************************************************************************
*
* canSoCallback - socket callback function
*
* This is the socket callback function.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

LOCAL void canSoCallback
    (
    void  * pCanDev,
    BOOL    state
    )
    {
    NODE * node = NULL;
    SOCKET_CAN * soc = NULL;

    (void)semTake (canSoList.sem, WAIT_FOREVER);

    for (node = lstFirst (&canSoList.lst); node != NULL; node = lstNext (node))
        {
        soc = (SOCKET_CAN *)node;
        if (soc->dev != NULL)
            {
            if (soc->dev == pCanDev)
                soc->devState = state;
            }
        }

    (void)semGive (canSoList.sem);
    }

/*******************************************************************************
*
* canSoWait - socket wait function
*
* This function waits data ready or timeout.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
*/

LOCAL int canSoWait
    (
    SOCKET_CAN * soc
    )
    {
    BOOL flag = FALSE;
    int ret = OK;

    if ((soc->timeout == 0) || (soc->timeout == -1))
        flag = TRUE;

Again:
    if (flag == TRUE)
        {
        ret = semTake (soc->semRcv, WAIT_FOREVER);
        if (ret == OK)
            {
            (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);

            if (lstCount (&soc->lstMsg.lst) == 0)
                {
                (void)semGive (soc->lstMsg.sem);
                goto Again;
                }

            (void)semGive (soc->lstMsg.sem);
            }
        }
    else
        {
        ret = semTake (soc->semRcv, (_Vx_ticks_t)soc->timeout);
        if (ret == OK)
            {
            (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);

            if (lstCount (&soc->lstMsg.lst) == 0)
                {
                (void)semGive (soc->lstMsg.sem);
                goto Again;
                }

            (void)semGive (soc->lstMsg.sem);
            }
        }

    return ret;
    }

/*******************************************************************************
*
* canSoPktListsInit -initialize the packet list
*
* This function initializes the packet list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

LOCAL void canSoPktListsInit( int canDevCount)
    {
    CAN_MSG_NODE *    pkt = NULL;
    CAN_LIST * freeSoList = NULL;
    int                 i = 0;

    for (int ix = 0; ix < canDevCount; ix++)
        {
        freeSoList = malloc (sizeof (CAN_LIST));
        if (freeSoList == NULL)
            {
            return;
            }

        lstInit (&freeSoList->lst);

        freeSoList->sem = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | \
                                      SEM_INVERSION_SAFE);
        for (i = 0; i < MSG_POOL_SIZE; i++)
            {
            pkt = malloc (sizeof (CAN_MSG_NODE));
            if (pkt != NULL)
                {
                lstAdd (&freeSoList->lst, &pkt->node);
                }
            }
        freeSoListArray[ix] = freeSoList;
        }

     lstInit (&canSoList.lst);

     canSoList.sem = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | \
                                 SEM_INVERSION_SAFE);
    }

/*******************************************************************************
*
* canSoPktMalloc - get a packet node from the free packet list
*
* This function gets a packet node from the free packet list.
*
* RETURNS: a pointer to the packet node structure if the free list is not empty,
* or NULL if the free packet list is empty.
*
* RETURNS: pointer to CAN_MSG_NODE or NULL if fail
*
* ERRNO: N/A
*
*/

LOCAL CAN_MSG_NODE * canSoPktMalloc
    (
    void * pDev
    )
    {
    CAN_MSG_NODE *    pkt = NULL;
    CAN_LIST * freeSoList = NULL;
    int          devIndex = 0;

    if (pDev == NULL)
        {
        kprintf( "canSoPktMalloc pDev null \n");
        return NULL;
        }

    devIndex = DEV_INDEX (pDev);

    /* device's ifindex start from 1, freeSoListArray[devIndex from 0] */

    devIndex -= 1;

    assert (devIndex < MAX_CAN_DEV_NUM);

    freeSoList = freeSoListArray[devIndex];
    if (freeSoList == NULL)
        {
        return NULL;
        }

    (void)semTake (freeSoList->sem, WAIT_FOREVER);

    if (lstCount (&freeSoList->lst) == 0)
        {
        (void)semGive (freeSoList->sem);
        return NULL;
        }

    pkt = (CAN_MSG_NODE *)lstGet (&freeSoList->lst);

    (void)semGive (freeSoList->sem);

    return pkt;
    }

/*******************************************************************************
*
* canSoPktPut - put a packet into the socket list
*
* This function puts a packet into the socket list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

LOCAL void canSoPktPut
    (
    SOCKET_CAN    * soc,
    NODE          * node
    )
    {
    struct socket_bsd * so = NULL;

    (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);
    lstAdd (&soc->lstMsg.lst, node);
    (void)semGive (soc->lstMsg.sem);

    /* wakeup read() */

    (void)semGive (soc->semRcv);

    /* wakeup select() */

    so = (struct socket_bsd *)soc->so;

    selWakeupAll (&so->so_selWakeupList, SELREAD);
    }

/*******************************************************************************
*
* canSoPktFree - put a packet to the free packet list
*
* This function puts a packet to the free packet list.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

static void canSoPktFree
    (
    CAN_MSG_NODE * pkt
    )
    {
    int devIndex = 0;
    NODE  * node = NULL;
    CAN_LIST * freeSoList = NULL;

    if (pkt == NULL)
        {
        return;
        }

    devIndex = DEV_INDEX (pkt->dev) - 1;
    node = &(pkt->node);

    assert (devIndex < MAX_CAN_DEV_NUM);

    freeSoList = freeSoListArray[devIndex];
    if (freeSoList == NULL)
        {
        return ;
        }

    (void)semTake (freeSoList->sem, WAIT_FOREVER);
    lstAdd (&freeSoList->lst, node);
    (void)semGive (freeSoList->sem);
    }

/*******************************************************************************
*
* canSoAlloc - allocate socket structure
*
* Allocates socket memory and initializes some fields.
*
* RETURNS: pointer to socket, or NULL if unable to allocate one
*
* ERRNO: N/A
*
*/

LOCAL struct socket * canSoAlloc
    (
    int     type,
    int     protocol
    )
    {
    struct socket_bsd * so = NULL;
    SOCKET_CAN * soc = NULL;

    so = malloc (sizeof(struct socket_bsd));
    if (so == NULL)
        return NULL;

    so->so_state = CSS_DISCONNECED;
    so->so_pcb = NULL;
    selWakeupListInit (&so->so_selWakeupList);

    so->so_pcb = malloc (sizeof(SOCKET_CAN));
    if (so->so_pcb == NULL)
        return NULL;

    memset (so->so_pcb, 0x0, sizeof(SOCKET_CAN));

    soc = (SOCKET_CAN *)(so->so_pcb);
    soc->so = so;
    soc->loopback = 1;      /* enable loopback */
    soc->rcvOwnMsg = 0;     /* don't receive own msgs */

    soc->semRcv = semBCreate (SEM_Q_FIFO, SEM_EMPTY);

    lstInit (&soc->lstMsg.lst);
    soc->lstMsg.sem = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | \
                                SEM_INVERSION_SAFE);

    soc->errMask = 0x0;

    lstInit (&soc->lstFilter);

    soc->semPolicy = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE | \
                                SEM_INVERSION_SAFE);

    soc->timeout = 0;
    soc->rawFdFrame = 0;

    return SOCKBSD_TO_SOCK (so);
    }

/*******************************************************************************
*
* canSoSocket - allocate socket structure
*
* This routine opens a socket and returns the corresponding socket structure.
* The socket structure is passed to the other socket routines to identify the
* socket.
 *
* RETURNS: OK if successful, otherwise errno
*
* ERRNO:
*
*/

LOCAL int canSoSocket
    (
    int             domain,
    int             type,
    int             protocol,
    struct socket **pso
    )
    {
    struct socket * so = NULL;

    if ((domain != AF_CAN) || (domain != PF_CAN))
        BOMB (EPFNOSUPPORT);

    so = canSoAlloc (type, protocol);
    if (so == NULL)
        BOMB (ENOBUFS);

    *pso = so;

    return OK;
    }

/*******************************************************************************
*
* canSoBind - bind the socket with a address
*
* This routine associates a network address (also referred to as its "name")
* with a specified socket so that other processes can connect or send to it.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO:
*
*/

LOCAL int canSoBind
    (
    struct socket     * so,     /* pointer to socket */
    struct sockaddr   * name,   /* name to be bound */
    socklen_t           namelen /* length of name */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    SOCKET_CAN * soc = NULL;
    CAN_DEV * pCanDev = NULL;
    SOCKADDR_CAN * addr = (SOCKADDR_CAN *)name;

    if (addr->can_family != PF_CAN)
        {
        (void)printf ("CAN only support PF_CAN\n");
        BOMB (EPFNOSUPPORT);
        }

    if ((namelen == 0) || (namelen < sizeof(struct sockaddr_can)))
        {
        (void)printf ("name length wrong\n");
        BOMB (EINVAL);
        }

    if (sob->so_pcb == NULL)
        {
        (void)printf ("so->so_pcb is NULL\n");
        BOMB (EINVAL);
        }

    soc = (SOCKET_CAN *)sob->so_pcb;

    if (addr->can_ifindex != 0)
        {
        if(recvEnabled)
            {
            pCanDev = canDevBind (addr->can_ifindex, canSoIfRcv, canSoCallback);
            }
        else
            {
            pCanDev = canDevBind (addr->can_ifindex, NULL, canSoCallback);
            }
        if (pCanDev == NULL)
            {
            (void)printf ("canDevBind failed\n");
            BOMB (EINVAL);
            }

        soc->devState = TRUE;
        }
    else
        {
        /* bind to all of CAN devices */
        if(recvEnabled)
            {
            pCanDev = canDevBind (addr->can_ifindex, canSoIfRcv, canSoCallback);
            }
        else
            {
            pCanDev = canDevBind (addr->can_ifindex, NULL, canSoCallback);
            }
        if (pCanDev != NULL)
            {
            (void)printf ("canDevBind to all devices failed\n");
            BOMB (EINVAL);
            }
        }

    soc->dev = pCanDev;

    (void)semTake (canSoList.sem, WAIT_FOREVER);
    lstAdd (&canSoList.lst, &soc->node);
    (void)semGive (canSoList.sem);

    sob->so_state = CSS_READY;

    return OK;
    }

/*******************************************************************************
*
* canSoRecv - socket receive function
*
* This routine receives data using the specified socket.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO:
*
* \NOMANUAL
*/

LOCAL int canSoRecv
    (
    struct socket * so,     /* socket to receive data from */
    void          * buf,    /* buffer to write data to */
    size_t          bufLen, /* length of buffer */
    int             flags   /* flags to underlying protocols */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;
    size_t msgLen = 0;
    SOCKET_CAN * soc = NULL;
    CAN_MSG_NODE * pkt = NULL;

    if (sob == NULL)
        {
        (void)printf ("so is NULL\n");
        BOMB (ENOTSOCK);
        }

    if ((sob->so_state != CSS_READY) || (sob->so_pcb == NULL))
        {
        (void)printf ("sob->so_state != CSS_READY || sob->so_pcb == NULL\n");
        BOMB (ENOTCONN);
        }

    soc = (SOCKET_CAN *)sob->so_pcb;

    if (lstCount (&soc->lstMsg.lst)== 0)
        {
        if (flags & MSG_DONTWAIT)
            {
            BOMB (EWOULDBLOCK);
            }
        else
            {
            /* wait data or timeout */

            ret = canSoWait (soc);
            }
        }

    if (ret == OK)
        {  
        (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);
        pkt = (CAN_MSG_NODE *)lstGet (&soc->lstMsg.lst);
        (void)semGive (soc->lstMsg.sem);

        if (pkt != NULL)
            {
            if (pkt->msg.res0 == 1)
                {
                if (pkt->msg.can_dlc > CANFD_MAX_DLEN)
                    {
                    BOMB(ENOBUFS);
                    }
                msgLen = CAN_FD_MTU;
                }
            else if (pkt->msg.res0 == 0)
                {
                if (pkt->msg.can_dlc > CAN_MAX_DLEN)
                    {
                    BOMB(ENOBUFS);
                    }
                msgLen = CAN_MTU;
                }
            else
                {
                BOMB(ENOBUFS);
                }

            msgLen = MIN (msgLen, bufLen);
            
            if (pkt->type == PACKET_LOOPBACK)
                {
                if ((soc->dev == NULL) || (soc == pkt->soc))
                    {
                    memcpy ((char *)buf, (char *)(&pkt->msg), msgLen);
                    ret = (int)msgLen;
                    canSoPktFree (pkt);

                    goto Out;
                    }
                }

            memcpy ((char *)buf, (char *)(&pkt->msg), msgLen);
            ret = (int) msgLen;
            canSoPktFree (pkt);
            }
        else
            {
            ret = 0;
            }
        }

Out:
    return ret;
    }

/*******************************************************************************
*
* canSoRecvFrom - socket receive function
*
* This routine receives data from the socket "so". The optional
* "from" argument captures the address of the data's sender.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO:
*
*/

LOCAL int canSoRecvFrom
    (
    struct socket     * so,         /* socket to receive from */
    void              * buf,        /* pointer to data buffer */
    size_t              bufLen,     /* length of buffer */
    int                 flags,      /* flags to underlying protocols */
    struct sockaddr   * from,       /* where to copy sender's addr */
    socklen_t         * pFromLen    /* value/result length of <from> */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;
    SOCKET_CAN * soc = NULL;
    CAN_MSG_NODE * pkt = NULL;
    SOCKADDR_CAN * addr = (SOCKADDR_CAN *)from;

    if (sob == NULL)
        {
        (void)printf ("so is NULL\n");
        BOMB (ENOTSOCK);
        }

    if ((sob->so_state != CSS_READY) || (sob->so_pcb == NULL))
        {
        (void)printf ("sob->so_state != CSS_READY || sob->so_pcb == NULL\n");
        BOMB (ENOTCONN);
        }

    soc = (SOCKET_CAN *)sob->so_pcb;
    if (soc->dev != NULL)
        BOMB (EINVAL);

Again:
    if (lstCount (&soc->lstMsg.lst)== 0)
        {
        if (flags & MSG_DONTWAIT)
            {
            BOMB (EWOULDBLOCK);
            }
        else
            {
            /* wait data or timeout */

            ret = canSoWait (soc);
            }
        }

    if (ret == OK)
        {
        (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);
        pkt = (CAN_MSG_NODE *)lstGet (&soc->lstMsg.lst);
        (void)semGive (soc->lstMsg.sem);

        if (pkt != NULL)
            {
            if (pkt->type == PACKET_LOOPBACK)
                {
                if (soc->rcvOwnMsg != 1)
                    {
                    canSoPktFree (pkt);
                    goto Again;
                    }
                }

            if (pkt->msg.can_dlc > CANFD_MAX_DLEN)
                BOMB (ENOBUFS);

            size_t const length = MIN (CAN_FD_MTU, bufLen);
            
            memcpy ((char *)buf, (char *)&pkt->msg, length);
            addr->can_ifindex = ((CAN_DEV *)pkt->dev)->ifindex;
            ret = (int) length;
            canSoPktFree (pkt);
            }
        else
            {
            ret = 0;
            }
        }

    return ret;
    }

/*******************************************************************************
*
* canSoSend - socket data sending funtion
*
* This routine sends data to a previously established socket.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO:
*
*/

LOCAL int canSoSend
    (
    struct socket * so,     /* socket to receive data from */
    const void    * buf,    /* buffer to write data to */
    size_t          bufLen, /* length of buffer */
    int             flags   /* flags to underlying protocols */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int          ret = OK;
    SOCKET_CAN * soc = NULL;
    CAN_FD_MSG * pMsg;

    pMsg = (CAN_FD_MSG *) buf;

    if (sob == NULL)
        {
        (void)printf ("so == NULL\n");
        BOMB (EINVAL);
        }

    if (buf == NULL)
        {
        (void)printf ("buf == NULL\n");
        BOMB (EINVAL);
        }

    if (((soc = (SOCKET_CAN *)sob->so_pcb) == NULL) ||
        (sob->so_state != CSS_READY))
        {
        (void)printf ("No connection\n");
        BOMB (ENOTCONN);
        }
    if (soc->rawFdFrame == 1)
        {
        if ((bufLen != CAN_FD_MTU) || (pMsg->can_dlc > CANFD_MAX_DLEN))
            {
            (void) printf ("CAN FD bufLen is not right\n");
            BOMB (EMSGSIZE);
            }

        pMsg->res0 = 1;  /* use res0 to indicate fd frame*/
        }
    else if (soc->rawFdFrame == 0)
        {
        if ((bufLen != CAN_MTU) || (pMsg->can_dlc > CAN_MAX_DLEN))
            {
            (void) printf ("bufLen is not right\n");
            BOMB (EMSGSIZE);
            }

        pMsg->res0 = 0;
        }

    if (soc->dev)
        {

        /*
         * The sent CAN frames are looped back to the open CAN sockets that
         * registered for the CAN frames' CAN-ID on this given interface.
         */   

        (void)canSoIfRcv ((void *)buf, PACKET_LOOPBACK, soc->dev, soc);

        if (soc->devState == TRUE)
            {
            if (canDevSend (soc->dev, buf) == OK)
                {
                ret = (int)bufLen;
                }
            else
                {
                (void)printf ("canDevSend failed\n");
                BOMB (EINVAL);
                }
            }
        else
            {
            (void)printf ("soc->devState == FALSE");
            BOMB (EINVAL);
            }
        }
    else
        {
        (void)printf ("No device\n");
        BOMB (EINVAL);
        }

    return ret;
    }

/*******************************************************************************
*
* canSoSendTo - socket data sending funtion
*
* This routine sends data to the socket "so". The optional "name" argument
* specifies the destination of the message.

* RETURNS: OK if successful, otherwise errno
*
* ERRNO:
*
*/

LOCAL int canSoSendTo
    (
    struct socket         * so,     /* socket to send data to */
    const void            * buf,    /* pointer to data buffer */
    size_t                  bufLen, /* length of buffer */
    int                     flags,  /* flags to underlying protocols */
    const struct sockaddr * name,   /* recipient's address */
    socklen_t               namelen /* length of <to> sockaddr */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;
    CAN_DEV * pCanDev = NULL;
    SOCKET_CAN * soc = NULL;
    SOCKADDR_CAN * addr = (SOCKADDR_CAN *)name;

    if (sob == NULL)
        BOMB (EINVAL);

    if (bufLen > CAN_FD_MTU)
        BOMB (EMSGSIZE);

    if (((soc = (SOCKET_CAN *)sob->so_pcb) == NULL) ||
        (sob->so_state != CSS_READY))
        BOMB (ENOTCONN);

    /*
     * soc->dev == NULL means the socket is bound to all CAN devices
     */

    if (soc->dev == NULL)
        {
        pCanDev = canDevGetByIndex (addr->can_ifindex);
        if (pCanDev != NULL)
            {
            (void)canSoIfRcv ((void *)buf, PACKET_LOOPBACK, pCanDev, NULL);

            if (canDevSend (pCanDev, buf) == OK)
                {
                ret = (int)bufLen;
                }
            else
                {
                (void)printf ("canDevSend failed\n");
                BOMB (EINVAL);
                }
            }
        else
            {
            BOMB (EINVAL);
            }
        }
    else
        {
        BOMB (EINVAL);
        }

    return ret;
    }

/*******************************************************************************
*
* canSoOptSet - set the socket option
*
* This routine sets the options associated with a socket.
*
* RETURNS: OK if successful, otherwise -1, and set errno
*
* ERRNO:
*
*/

LOCAL int canSoOptSet
    (
    struct socket * so,         /* target socket */
    int             level,      /* protocol level of option */
    int             optname,    /* option name */
    const void    * optval,     /* pointer to option value */
    socklen_t       optlen      /* option length */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;
    SOCKET_CAN * soc = (SOCKET_CAN *)sob->so_pcb;

    if (level != SOL_CAN_RAW)
        BOMB (EINVAL);

    if (semTake (soc->semPolicy, WAIT_FOREVER) != OK) /* socket has been terminated */
        {
        BOMB (EBADF);
        }

    switch (optname)
        {
        case CAN_RAW_FILTER:
            {
            int i;
            CAN_FILTER_NODE * node;
            CAN_FILTER * filter = (CAN_FILTER *)optval;

            lstFree (&soc->lstFilter);
            for (i = 0; i < (optlen / sizeof(CAN_FILTER)); i++)
                {
                node = malloc (sizeof(CAN_FILTER_NODE));

                if (node != NULL)
                    {
                    node->filter.can_id = filter->can_id;
                    node->filter.can_mask = filter->can_mask;
                    lstAdd (&soc->lstFilter, (NODE *)node);
                    filter++;
                    }
                }

            break;
            }

        case CAN_RAW_ERR_FILTER:
            {
            if (optlen > sizeof(soc->errMask))
                {
                ret = EINVAL;
                break;
                }

            soc->errMask = *(int *)optval;

            break;
            }

        case CAN_RAW_LOOPBACK:
            {
            if (optlen > sizeof(soc->loopback))
                {
                ret = EINVAL;
                break;
                }

            soc->loopback = *(int *)optval;

            break;
            }

        case CAN_RAW_RECV_OWN_MSGS:
            {
            if (optlen > sizeof(soc->rcvOwnMsg))
                {
                ret = EINVAL;
                break;
                }

            soc->rcvOwnMsg= *(int *)optval;

            break;
            }

        case CAN_RAW_FD_FRAMES:
            {
            if (optlen > sizeof (soc->rawFdFrame))
                {
                ret = EINVAL;
                break;
                }

            soc->rawFdFrame = 1;

            kprintf ("option set ok,soc->rawFdFrame = %d\n", soc->rawFdFrame);
            break;
            }

        default:
            ret = EINVAL;

            break;
        }

    (void)semGive (soc->semPolicy);

    if (ret == 0)
        return 0;

    (void) errnoSet (ret);
    return -1;
    }

/*******************************************************************************
*
* canSoOptGet - get the socket option
*
* This routine returns relevant option values associated with a socket.
*
* RETURNS: 0
*
* ERRNO: N/A
*
*/

LOCAL int canSoOptGet
    (
    struct socket * so,         /* socket */
    int             level,      /* protocol level for options */
    int             optname,    /* name of option */
    void          * optval,     /* where to put option */
    socklen_t     * optlen      /* where to put option length */
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;
    SOCKET_CAN * soc = (SOCKET_CAN *)sob->so_pcb;


    if ((optval == NULL) || (optlen == NULL))
        {
        BOMB (EINVAL);
        }

    if (level != SOL_CAN_RAW)
        BOMB (EINVAL);

    if (semTake (soc->semPolicy, WAIT_FOREVER) != OK) /* socket has been terminated */
        {
        BOMB (EBADF);
        }

    switch (optname)
        {
        case CAN_RAW_FILTER:
            {
            CAN_FILTER_NODE * node;
            CAN_FILTER *      filter;

            filter = (CAN_FILTER *) optval;

            *((socklen_t *) optlen) = lstCount (&soc->lstFilter);

            node = (CAN_FILTER_NODE *) lstFirst (&soc->lstFilter);

            while ((node != NULL) && (filter != NULL))
                {
                filter->can_id = node->filter.can_id;
                filter->can_mask = node->filter.can_mask;
                node = (CAN_FILTER_NODE *) lstNext ((NODE *) node);
                filter ++;
                }

            break;
            }

        case CAN_RAW_ERR_FILTER:
            {
            *((socklen_t *) optlen) = (sizeof(soc->errMask));
            *((unsigned int *) optval) = soc->errMask;

            break;
            }

        case CAN_RAW_LOOPBACK:
            {
            *((socklen_t *) optlen) = (sizeof(soc->loopback));
            *((volatile int *) optval) = soc->loopback;

            break;
            }

        case CAN_RAW_RECV_OWN_MSGS:
            {
            *((socklen_t *) optlen) = (sizeof(soc->rcvOwnMsg));
            *((volatile BOOL *) optval) = soc->rcvOwnMsg;

            break;
            }
        case CAN_RAW_FD_FRAMES:
            {
            *((socklen_t *) optlen) = (sizeof (soc->rawFdFrame));
            *((UINT8 *) optval) = soc->rawFdFrame;
            break;
            }

        default:
            ret = EINVAL;

            break;
        }
    (void)semGive (soc->semPolicy);
    return ret;
    }

/*******************************************************************************
*
* canSoTerminate - terminate the socket
*
* At the time the socket is closed, this function is called via the
* FIOSHUTDOWN ioctl to terminate the socket, waking up any tasks that might
* be blocked on it.  The actual freeing of the socket is deferred
* until canSoClose(), which the I/O system only calls once all active
* references to the FD_ENTRY for the socket are released.
*
* RETURNS: N/A
*
* ERRNO: N/A
*
*/

LOCAL void canSoTerminate
    (
    struct socket_bsd * sob
    )
    {
    NODE *         pNode = NULL;
    CAN_MSG_NODE * pkt = NULL;
    SOCKET_CAN *   soc = (SOCKET_CAN *)sob->so_pcb;

    sob->so_state = CSS_DISCONNECED;

    selWakeupListTerm (&sob->so_selWakeupList);

    if (soc != NULL)
        {
        (void)semTake (canSoList.sem, WAIT_FOREVER);
        lstDelete (&canSoList.lst, (NODE *)soc);
        (void)semGive (canSoList.sem);

        /* free rcv list */

        (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);
        for (pNode = lstGet (&soc->lstMsg.lst); pNode != NULL;
            pNode = lstNext (pNode))
            {
            pkt =  MEMBER_TO_OBJECT (pNode, CAN_MSG_NODE, node);
            if (pkt != NULL)
                {
                canSoPktFree (pkt);
                }
            }
        (void)semTerminate (soc->lstMsg.sem);

        /*free policy */

        (void)semTake (soc->semPolicy, WAIT_FOREVER);
        lstFree (&soc->lstFilter);
        (void)semTerminate (soc->semPolicy);

        (void)semTerminate (soc->semRcv);
        }

    return;
    }

/*******************************************************************************
*
* canSoClose - close the socket
*
* This routine is called by the I/O system to close a socket. This routine
* assumes that the I/O system only closes the socket once there are no more
* active references to it.
*
* RETURNS: OK
*
* ERRNO: N/A
*
*/

LOCAL int canSoClose
    (
    struct socket * so
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    SOCKET_CAN *   soc = (SOCKET_CAN *)sob->so_pcb;

#ifndef FIOSHUTDOWN
    /*
     * If FIOSHUTDOWN is defined, canSoTerminate() is called for the
     * FIOSHUTDOWN ioctl at the time the socket is closed, even if
     * some active references remain to the socket.
     */

    canSoTerminate (sob);
#endif

    if (soc != NULL)
        {
        free (soc);
        }

    free (sob);

    return OK;
    }

/*******************************************************************************
*
* canSoSelect - socket select function
*
* This is the socket select function.
*
* RETURNS: OK or -EINVAL
*
* ERRNO: N/A
*
*/

LOCAL int canSoSelect
    (
    struct socket     * so,
    SEL_WAKEUP_NODE   * node
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;
    SOCKET_CAN * soc = NULL;

    switch (selWakeupType (node))
        {
        case SELREAD:
            {
            if (selNodeAdd (&sob->so_selWakeupList, node) == ERROR)
                {
                ret = -EINVAL;
                break;
                }

            soc = (SOCKET_CAN *)sob->so_pcb;

            if ((sob->so_state != CSS_READY) || (soc == NULL))
                {
                ret = -EINVAL;
                break;
                }

            (void)semTake (soc->lstMsg.sem, WAIT_FOREVER);

            if (lstCount (&soc->lstMsg.lst))
                {
                selWakeupAll (&sob->so_selWakeupList, SELREAD);
                }

            (void)semGive (soc->lstMsg.sem);

            break;
            }

        case SELWRITE:
            break;

        default:
            ret = -EINVAL;
            break;
        }

    return ret;
    }

/*******************************************************************************
*
* canSoUnselect - socket unselect function
*
* This is the socket unselect function.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
*/

LOCAL int canSoUnselect
    (
    struct socket     * so,
    SEL_WAKEUP_NODE   * node
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = OK;

    switch ((selWakeupType (node)))
        {
        case SELREAD:
            ret = selNodeDelete (&sob->so_selWakeupList, node);
            break;

        case SELWRITE:
            break;

        default:
            ret = -EINVAL;
            break;
        }

    return ret;
    }

/*******************************************************************************
*
* canSoRead -socket read function
*
* This routine is called by the I/O system when a read is done on a socket.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
*/

LOCAL int canSoRead
    (
    struct socket * so,
    char          * buf,
    size_t          maxBytes
    )
    {
    return canSoRecv (so, buf, maxBytes, 0);
    }

/*******************************************************************************
*
* canSoWrite - socket write function
*
* This routine is called by the I/O system when a write is done on a socket.
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
*/

LOCAL int canSoWrite
    (
    struct socket * so,
    const char    * buf,
    size_t          maxBytes
    )
    {
    return canSoSend (so, buf, maxBytes, 0);
    }

/*******************************************************************************
*
* canSoIoctl - socket Ioctl function
*
* This is the ioctl support function for CAN sockets
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
*/

LOCAL int canSoIoctl
    (
    struct socket * so,
    int             code,
    _Vx_usr_arg_t   arg,
    void          * unused
    )
    {
    struct socket_bsd * sob = SOCK_TO_SOCKBSD (so);
    int ret = ERROR;
    CAN_DEV * pCanDev = NULL;
    SOCKET_CAN * soc = (SOCKET_CAN *)(sob->so_pcb);

    switch (code)
        {
        case FIOSELECT:
            ret = canSoSelect (so, (SEL_WAKEUP_NODE *)arg);
            break;

        case FIOUNSELECT:
            ret = canSoUnselect (so, (SEL_WAKEUP_NODE *)arg);
            break;

        case (int)SIOCGIFNAME:
            pCanDev = canDevGetByIndex (((struct ifreq *)arg)->ifr_ifindex);
            if (pCanDev != NULL)
                {
                strncpy(((struct ifreq *)arg)->ifr_name, pCanDev->name,
                        IFNAMSIZ);
                ((struct ifreq *)arg)->ifr_name[IFNAMSIZ-1] = 0;
                ret = OK;
                }
            else
                {
                ret = ERROR;
                }
            break;

        case (int)SIOCGIFINDEX:
            pCanDev = canDevGetByName (((struct ifreq *)arg)->ifr_name);
            if (pCanDev != NULL)
                {
                ((struct ifreq *)arg)->ifr_ifindex = pCanDev->ifindex;
                ret = OK;
                }
            else
                {
                ret = ERROR;
                }
            break;

#ifdef FIOSHUTDOWN
        case FIOSHUTDOWN:
            canSoTerminate (sob);
            ret = OK;
            break;
#endif /* FIOSHUTDOWN */

        default:
            if (soc->dev != NULL)
                {
                ret = canDevIoctl (soc->dev, code, arg);
                }
            else
                {
                BOMB (EINVAL);
                }
            break;
        }

    return ret;
    }

/*******************************************************************************
*
* canSoInit - socket intialization function
*
* canSoInit must be called once at configuration time before any CAN socket
* operations are performed.
*
* RETURNS: pointer to SOCK_FUNC
*
* ERRNO: N/A
*
*/

LOCAL SOCK_FUNC * canSoInit(void)
    {
    return (&canSoFunc);
    }

/*******************************************************************************
*
* canSoPktClone - clone a package
*
* This function clone a package.
*
* RETURNS: pointer to CAN_MSG_NODE if successful, otherwise NULL
*
* ERRNO: N/A
*
*/

LOCAL CAN_MSG_NODE * canSoPktClone
    (
    char  * buf,
    int     flag,
    void  * soc,
    void  * dev
    )
    {
    CAN_MSG_NODE * pkt = NULL;   

    pkt = canSoPktMalloc (dev);
    if (pkt == NULL)
        return NULL;

    pkt->soc = soc;
    pkt->type = flag;
    pkt->dev = dev;
    memcpy ((char *)(&pkt->msg), buf, sizeof(CAN_FD_MSG));

    return pkt;
    }

/*******************************************************************************
*
* canSoFilter - set the socket filter
*
* This function sets the socket filter.
*
* RETURNS: TRUE or FALSE
*
* ERRNO: N/A
*
*/

LOCAL BOOL canSoFilter
    (
    char          * buf,
    SOCKET_CAN    * soc
    )
    {
    BOOL match = FALSE;
    NODE * node = NULL;
    CAN_FILTER_NODE * fp = NULL;
    unsigned int can_id = ((CAN_MSG *)buf)->can_id;

    if (semTake (soc->semPolicy, WAIT_FOREVER) != OK)
        return FALSE;

    if (can_id & CAN_ERR_FLAG)
        {
        /* error frame */

        if (can_id & soc->errMask)
            match = TRUE;

        (void)semGive (soc->semPolicy);

        return match;
        }

    for (node = lstFirst (&soc->lstFilter); node != NULL; node = lstNext (node))
        {
        fp = (CAN_FILTER_NODE *)node;

        if (fp->filter.can_id & CAN_INV_FILTER)
            {
            /* invert filter */

            int id = fp->filter.can_id;
            id &= ~CAN_INV_FILTER;
            if ((can_id & fp->filter.can_mask) != (id & fp->filter.can_mask))
                {
                match = TRUE;
                (void)semGive (soc->semPolicy);

                return match;
                }
            }
        else
            {

            if ((can_id & fp->filter.can_mask) ==
                (fp->filter.can_id & fp->filter.can_mask))
                {
                match = TRUE;
                (void)semGive (soc->semPolicy);

                return match;
                }
            }
        }

    if (lstCount (&soc->lstFilter) == 0)
        match = TRUE;

    (void)semGive (soc->semPolicy);

    return match;
    }

/*******************************************************************************
*
* canSoIfRcv - receive package from CAN controller
*
* This function receives package from CAN controller.
*
* RETURNS: TRUE
*
* ERRNO: N/A
*
*/

LOCAL BOOL canSoIfRcv
    (
    void  * pBuf,
    int     flag,
    void  * pCanDev,
    void  * pExt
    )
    {
    NODE * node = NULL;
    SOCKET_CAN * soc = NULL;
    SOCKET_CAN * self = (SOCKET_CAN *)pExt;
    CAN_MSG_NODE * pkt = NULL;

    if ((self != NULL) && self->loopback != 1)
        return TRUE;

    (void)semTake (canSoList.sem, WAIT_FOREVER);

    for (node = lstFirst (&canSoList.lst); node != NULL; node = lstNext (node))
        {
        soc = (SOCKET_CAN *)node;

        if (soc == self)
            {
            /*
             * if the package is sent by ourself and we don't want to
             * receive it, just ignore the package.
             */

            if (self->rcvOwnMsg != 1)
                continue;
            }

        if ((soc->dev == NULL) || (soc->dev == pCanDev))
            {
            /*
             * if soc->dev is NULL, copy package to all binded sockets.
             * if soc->dev == pCanDev, copy package to the binded socket.
             */

            if ((flag == PACKET_LOOPBACK) && (soc->loopback != 1))
                continue;
            
            if (canSoFilter (pBuf, soc) == TRUE)
                {
                /* clone the package and put it into the socket's list */
                pkt = canSoPktClone (pBuf, flag, soc, pCanDev);                

                if (pkt != NULL)
                    {
                    canSoPktPut (soc, &pkt->node);
                    }
                else
                    {
                    (void)kprintf("canSoPktClone failed\n");
                    }    
                }
            }
        }
    
    (void)semGive (canSoList.sem);

    return TRUE;
    }

/*******************************************************************************
*
* bcmConnect - BCM connect function
*
* This is the BCM connect function.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO: N/A
*
*/

LOCAL STATUS bcmConnect
    (
    struct socket     * so,
    struct sockaddr   * name,
    socklen_t           namelen
    )
    {
    return canSoBind (so, name, namelen);
    }

/*******************************************************************************
*
* bcmSendMsg - BCM send function
*
* This is the BCM send function.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO: N/A
*
*/

LOCAL int bcmSendMsg
    (
    struct socket * so,
    struct msghdr * msg,
    int             flags
    )
    {
    return canSoSend (so, msg->msg_iov->iov_base, msg->msg_iov->iov_len, 0);
    }

/*******************************************************************************
*
* bcmRecvMsg - BCM receive function
*
* This is the BCM receive function.
*
* RETURNS: OK if successful, otherwise errno
*
* ERRNO: N/A
*
*/

LOCAL int bcmRecvMsg
    (
    struct socket * so,
    struct msghdr * msg,
    int             flags
    )
    {
    return canSoRecv (so, msg->msg_iov->iov_base, msg->msg_iov->iov_len, 0);
    }

/*******************************************************************************
*
* canSoLibInit - socket library intialization function
*
* This is the socket library intialization function. This routine is
* called during system initialization
*
* RETURNS: OK or ERROR
*
* ERRNO: N/A
*
* \NOMANUAL
*/

int canSoLibInit()
    {
    if (sockLibAdd (canSoInit, PF_CAN, PF_CAN) == ERROR)
        {
        (void)printf ("[canSoLibInit] failed sockLibAdd for PF_CAN\n");
        return (ERROR);
        }

    canSoPktListsInit (canDevCount ());

    return OK;
    }
void canSoRecvEnable(BOOL enabled)
    {
    recvEnabled = enabled;
    }

/*******************************************************************************
*
* showFreeSoListArray - show free list nodes of each device
*
* This is a debug helpful function, it shows free nodes for each device.
*
* RETURNS: void
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void showFreeSoListArray
    (
    void
    )
    {
    for (int ix = 0; ix < MAX_CAN_DEV_NUM; ix++)
        {
        if (freeSoListArray[ix] != NULL)
            {
            printf ("freeSoListArray[%2d] count : %2d \n", ix,
                    lstCount(&(freeSoListArray[ix]->lst)));
            }
        }
    }
