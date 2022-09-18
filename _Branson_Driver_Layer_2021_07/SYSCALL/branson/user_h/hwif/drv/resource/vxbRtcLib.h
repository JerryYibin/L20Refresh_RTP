/* vxbRtcLib.h - VxBus RTC interfaces header file */

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

/*
DESCRIPTION

This library provides the RTC specific interfaces

*/

#ifndef __INCvxbRtcLibh
#define __INCvxbRtcLibh

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif
struct tm
    {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
    };
extern STATUS vxbRtcGet(struct tm *rtcTime);
extern STATUS vxbRtcSet(struct tm *rtcTime);

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbRtcLibh */

