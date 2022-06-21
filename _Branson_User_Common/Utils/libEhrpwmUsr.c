#include <syscall.h>
#include <taskLib.h>

#ifdef _WRS_KERNEL
#error this file should be built in RTP
#endif

extern int sprintf(char *, const char *, ...);
extern STATUS vxbEhrPwmRegISRInternal(UINT8, UINT32, UINT32);

#define TASK_NAME "/pwmIsr"

static void isrTask(FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    while(eventReceive(VXEV01, EVENTS_WAIT_ALL, WAIT_FOREVER, NULL) == OK)
        {
        routine(arg);
        }
    }

STATUS vxbEhrPwmRegisterISR
    (
    UINT8  ehrpwm,
    VOIDFUNCPTR pIsr,
    void  *pArg,
    UINT32 period,
    UINT32 mode
    )
    {
    char taskName[12];
    TASK_ID taskId;

    sprintf(taskName, "%s%x", TASK_NAME, ehrpwm&0xff);
    taskId = taskOpen(taskName, 0, 0, 0, NULL, 0, NULL, (FUNCPTR)NULL,0,0,0,0,0,0,0,0,0,0);
    if(taskId != NULL)
        {
        taskDelete(taskId);
        }

    if(pIsr != NULL)
        {
        taskId = taskOpen(taskName, 0, VX_FP_TASK, OM_CREATE|OM_EXCL, NULL, 0x2000, NULL,
                (FUNCPTR)isrTask, (_Vx_usr_arg_t)pIsr, (_Vx_usr_arg_t)pArg, 0,0,0,0,0,0,0,0);
        }
    return vxbEhrPwmRegISRInternal(ehrpwm, period, mode);
    }

