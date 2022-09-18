#include <syscall.h>
#include <taskLib.h>

#ifdef _WRS_KERNEL
#error this file should be built in RTP
#endif

extern STATUS sysAuxClkConnectInternal();

#define TASK_NAME "/auxIsr"

static void isrTask(FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    while(eventReceive(VXEV01, EVENTS_WAIT_ALL, WAIT_FOREVER, NULL) == OK)
        {
        routine(arg);
        }
    }

STATUS sysAuxClkConnect(FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    TASK_ID taskId = taskOpen(TASK_NAME, 0, 0, 0, NULL, 0, NULL, (FUNCPTR)NULL,0,0,0,0,0,0,0,0,0,0);
    if(taskId != NULL)
        {
        taskDelete(taskId);
        }

    if(routine != NULL)
        {
        taskId = taskOpen(TASK_NAME, 0, VX_FP_TASK, OM_CREATE|OM_EXCL, NULL, 0x2000, NULL,
                (FUNCPTR)isrTask, (_Vx_usr_arg_t)routine, arg, 0,0,0,0,0,0,0,0);
        }
    return sysAuxClkConnectInternal();
    }

