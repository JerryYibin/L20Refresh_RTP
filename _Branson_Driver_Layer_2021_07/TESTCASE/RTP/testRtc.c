
#include <stdio.h>
#ifdef _WRS_KERNEL
#include <time.h>
#include <hwif/drv/resource/vxbRtcLib.h>
#else
#include <customSystemCall.h>
#endif

void testRtc()
    {
    struct tm time;
    
    printf("### %s ###\n", __FUNCTION__);

    time.tm_year = 2022-1900;
    time.tm_mon  = 5-1;
    time.tm_mday = 1;
    time.tm_hour = 2;
    time.tm_min  = 3;
    time.tm_sec  = 4;

    printf("vxbRtcSet = %d\n", vxbRtcSet(&time));

    printf("vxbRtcGet = %d\n", vxbRtcGet(&time));
    printf("%d-%d-%d %d:%d:%d\n", time.tm_year+1900, time.tm_mon+1, time.tm_mday,
                                  time.tm_hour, time.tm_min, time.tm_sec);


    }

