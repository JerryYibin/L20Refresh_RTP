
#include <stdio.h>
#ifdef _WRS_KERNEL
#include <logLib.h>
#else
#include <customSystemCall.h>
#endif
#include <taskLib.h>
#include <sysLib.h>

static void auxIsr(char *s)
    {
#ifdef _WRS_KERNEL
    logMsg("%s\n",(_Vx_usr_arg_t)s,0,0,0,0,0);
#else
    printf("%s\n", s);
#endif
    }

void testAux()
    {
    printf("### %s ###\n", __FUNCTION__);

    sysAuxClkRateSet(100);

    printf("sysAuxClkRateGet = %d\n", sysAuxClkRateGet());

    sysAuxClkConnect((FUNCPTR)auxIsr, (_Vx_usr_arg_t)"hello\n");
    sysAuxClkEnable();

    taskDelay(sysClkRateGet()/5);

    sysAuxClkDisable();
    }

