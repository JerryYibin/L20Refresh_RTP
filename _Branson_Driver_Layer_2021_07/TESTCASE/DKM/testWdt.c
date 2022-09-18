/*
read
 WDIOC_GETTIMELEFT

write
 WDIOC_KEEPALIVE

ioctl
 WDIOC_KEEPALIVE
 WDIOC_GETTIMELEFT
 WDIOC_SETTIMEOUT
 WDIOC_GETTIMEOUT
 WDIOC_SETOPTIONS
  WDIOS_DISABLECARD
  WDIOS_ENABLECARD
  WDIOS_FORCERESET
*/

#include <stdio.h>
#include <ioLib.h>
#include <taskLib.h>
#include <sysLib.h>
#include <hwif/drv/watchdog/watchdog.h>

#define WDT_NAME "/watchdog/0"

void testWdt()
    {
    int fd;
    int left;

    printf("### %s ###\n", __FUNCTION__);

    fd = open(WDT_NAME,2,0);
    if(fd>0)
        {
        printf("!!! WDT enabled !!!\n");
        ioctl(fd, WDIOC_SETOPTIONS, WDIOS_ENABLECARD);

        /*
         * read(fd, buffer, maxBytes)
         * return the remaining time when (buffer != NULL) and (maxBytes > 0)
         */
        left = read(fd, &left, 1);
        printf("!!! WDT remaining time: %d !!!\n", left);

        taskDelay(3*sysClkRateGet());
        left = read(fd, &left, 1);
        printf("!!! WDT remaining time: %d !!!\n", left);

        /*
         * write(fd, buffer, nBytes)
         * reset the WDT when (buffer != NULL)
         *
         * if buffer includes character 'V',
         * close(fd) will disable the WDT
         */
        printf("!!! WDT reset !!!\n");
        write(fd, &left, 0);
        left = read(fd, &left, 1);
        printf("!!! WDT remaining time: %d !!!\n", left);

        printf("!!! WDT disabled !!!\n");
        ioctl(fd, WDIOC_SETOPTIONS, WDIOS_DISABLECARD);

        close(fd);
        }
    }

