#include <syscall.h>
#include <taskLib.h>

#ifdef _WRS_KERNEL
#error this file should be built in RTP
#endif

extern int sprintf(char *, const char *, ...);
extern STATUS vxbGpioIntConnectInternal(UINT32 id);
extern STATUS vxbGpioIntDisconnect(UINT32, VOIDFUNCPTR, void *);

#define TASK_NAME "/gpioIsr"
#define NAME_SIZE 12

static void isrTask(FUNCPTR routine, _Vx_usr_arg_t arg)
    {
    while(eventReceive(VXEV01, EVENTS_WAIT_ALL, WAIT_FOREVER, NULL) == OK)
        {
        routine(arg);
        }
    }
STATUS vxbGpioIntConnect(UINT32 id, VOIDFUNCPTR pIsr, void *pArg)
    {
    char taskName[NAME_SIZE];
    TASK_ID taskId;

    vxbGpioIntDisconnect(id, pIsr, pArg);

    sprintf(taskName, "%s%02x", TASK_NAME, id&0xff);
    taskId = taskOpen(taskName, 0, 0, 0, NULL, 0, NULL, (FUNCPTR)NULL,0,0,0,0,0,0,0,0,0,0);
    if(taskId != NULL)
        {
        taskDelete(taskId);
        }

    if(pIsr == NULL)
        {
        return OK;
        }

    taskId = taskOpen(taskName, 0, VX_FP_TASK, OM_CREATE|OM_EXCL, NULL, 0x2000, NULL,
                (FUNCPTR)isrTask, (_Vx_usr_arg_t)pIsr, (_Vx_usr_arg_t)pArg, 0,0,0,0,0,0,0,0);

    return vxbGpioIntConnectInternal(id);
    }

