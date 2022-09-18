
#include <stdio.h>    /* printf() */
#include <sockLib.h>  /* socket() */
#include <lstLib.h>   /* LIST */
#include <canDevLib.h>/* can_frame */
#include <socketCAN.h>/* PF_CAN */

/*
 * index: 1 for DCAN1, 2 for DCAN2
 * loop : 0 disabling socket loop, 1 for enabling socket loop
 */
void testCanSend(int index, int loop)
    {
    struct sockaddr_can addr;
    struct can_frame frame;
    int nbytes;
    int fd;
    int i;

    printf("### %s ###\n", __FUNCTION__);

    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if((index<1) || (index>canDevCount()))
        {
        index=1;
        }
    addr.can_ifindex = index;
    addr.can_family = PF_CAN;
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    setsockopt(fd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loop, sizeof(int));

    frame.can_id = 0x123;
    frame.can_dlc = 8;
    for(i = 0; i < 8; i++)
        frame.data[i] = (unsigned char)i;
    nbytes = send(fd, &frame, sizeof(struct can_frame), 0);
    if(nbytes != sizeof(struct can_frame))
        printf("send fail\n");
    else
        printf("send succeed\n");

    close(fd);
    return;
    }

/*
 * index: 1 for DCAN1, 2 for DCAN2
 */
void testCanRecv(int index)
    {
    struct sockaddr_can addr;
    struct can_frame frame;
    int nbytes;
    int fd;
    int i;

    printf("### %s ###\n", __FUNCTION__);

    fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if((index<1) || (index>canDevCount()))
        {
        index=1;
        }
    addr.can_ifindex = index;
    addr.can_family = PF_CAN;
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    nbytes = recv(fd, &frame, sizeof(struct can_frame), 0);
    if(nbytes != sizeof(struct can_frame))
        {
        printf("recv failed");
        close(fd);
        return;
        }

    printf("CAN id = 0x%x\n", frame.can_id);
    printf("CAN dlc = 0x%x\n", frame.can_dlc);
    if(!(frame.can_id & CAN_RTR_FLAG))
        {
        for(i = 0; i<frame.can_dlc; i++)
            printf("data[%d] = 0x%x\n", i, frame.data[i]);
        }
    close(fd);
    return;
    }

