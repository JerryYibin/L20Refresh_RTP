
#include <stdio.h>                        /* snprintf() */
#include <hwif/buslib/vxbFdtLib.h>        /* vxFdtPropGet() */
#include <hwif/drv/watchdog/vxbWatchdog.h>/* watchdogDevRemove() */

#undef  OMAP3_WATCHDOG_DBG_ON
#ifdef  OMAP3_WATCHDOG_DBG_ON
#include <private/kwriteLibP.h>         /* _func_kprintf */
#define OMAP3_WATCHDOG_DBG(...)                     \
    do                                                    \
        {                                                 \
        if(_func_kprintf != NULL)                \
            {                                         \
            (* _func_kprintf)(__VA_ARGS__);      \
            }                                         \
        }                                                 \
    while(FALSE)
#else
#define OMAP3_WATCHDOG_DBG(...)
#endif  /* OMAP3_WATCHDOG_DBG_ON */

#define OMAP_WATCHDOG_REV        (0x00)
#define OMAP_WATCHDOG_SYS_CONFIG (0x10)
#define OMAP_WATCHDOG_STATUS     (0x14)
#define OMAP_WATCHDOG_CNTRL      (0x24)
#define OMAP_WATCHDOG_CRR        (0x28)
#define OMAP_WATCHDOG_LDR        (0x2c)
#define OMAP_WATCHDOG_TGR        (0x30)
#define OMAP_WATCHDOG_WPS        (0x34)
#define OMAP_WATCHDOG_SPR        (0x48)

/* Using the prescaler, the OMAP watchdog could go for many months before firing.
 * These limits work without scaling,
 * with the 180 second default assumed by HW.
 */
#define TIMER_MARGIN_MAX       (24*60*60)/* 1 day */
#define TIMER_MARGIN_DEFAULT   180       /* 180 secs */
#define TIMER_MARGIN_MIN         1

#define GET_WLDR_VAL(secs)     (0xffffffff - ((secs) * 32768) + 1)
#define GET_WCCR_SECS(val)    ((0xffffffff - (val) + 1) / 32768)

#undef OMAP3_READ_4
#define OMAP3_READ_4(pDrvCtrl, addr)                                 \
    vxbRead32(pDrvCtrl->regHandle, (UINT32 *)((char *)(pDrvCtrl->regBase) + addr))

#undef OMAP3_WRITE_4
#define OMAP3_WRITE_4(pDrvCtrl, addr, data)                          \
    vxbWrite32(pDrvCtrl->regHandle,(UINT32 *)((char *)(pDrvCtrl->regBase) + addr), data)

LOCAL const VXB_FDT_DEV_MATCH_ENTRY tiWatchdogMatch[] =
    {
    {
    "ti,omap3-wdt",          /* compatible */
    NULL                     /* no configuration */
    },
    {}                       /* Empty terminated list */
    };

typedef struct tiWatchdogDrvCtrl
    {
    VXB_DEV_ID    pDev;         /* VxBus device handle */
    spinlockIsr_t spinLock;
    UINT32        unit;
    VIRT_ADDR     regBase;      /* WDT register base address */
    void         *regHandle;    /* WDT register access handle */
    VXB_RESOURCE *pMemRes;      /* WDT memory resource */
    UINT32        timeoutSec;
    BOOL          tiWdtRcvClose;
    UINT32        wdt_trgr_pattern;
    } OMAP3_WATCHDOG_DRVCTRL;

LOCAL void tiWdtReload(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    /* wait for posted write to complete */
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x08)
        ;

    pDrvCtrl->wdt_trgr_pattern = ~pDrvCtrl->wdt_trgr_pattern;
    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_TGR, pDrvCtrl->wdt_trgr_pattern);

    /* wait for posted write to complete */
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x08)
        ;
    }

LOCAL void tiWdtEnable(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    /* Sequence to enable the watchdog */
    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_SPR, 0xBBBB);
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x10)
        ;

    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_SPR, 0x4444);
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x10)
        ;
    }

LOCAL void tiWdtDisable(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    /* sequence required to disable watchdog */
    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_SPR, 0xAAAA);    /* TIMER_MODE */
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x10)
        ;

    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_SPR, 0x5555);    /* TIMER_MODE */
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x10)
        ;
    }

LOCAL void tiWdtSetTimer(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl, unsigned int timeout)
    {
    UINT32 pre_margin = GET_WLDR_VAL(timeout);

    /* just count up at 32 KHz */
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x04)
        ;
    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_LDR, pre_margin);
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x04)
        ;
    }

LOCAL void tiWdtStart(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    SPIN_LOCK_ISR_TAKE(&pDrvCtrl->spinLock);

    tiWdtDisable(pDrvCtrl);

    /* initialize prescaler */
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x01)
        ;

    OMAP3_WRITE_4(pDrvCtrl, OMAP_WATCHDOG_CNTRL, (1 << 5));
    while(OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_WPS) & 0x01)
        ;

    tiWdtSetTimer(pDrvCtrl, pDrvCtrl->timeoutSec);
    tiWdtReload(pDrvCtrl); /* trigger loading of new timeout value */
    tiWdtEnable(pDrvCtrl);
    SPIN_LOCK_ISR_GIVE(&pDrvCtrl->spinLock);
    }

LOCAL void tiWdtStop(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    SPIN_LOCK_ISR_TAKE(&pDrvCtrl->spinLock);
    tiWdtDisable(pDrvCtrl);
    SPIN_LOCK_ISR_GIVE(&pDrvCtrl->spinLock);
    }

LOCAL void tiWdtKeepalive(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    SPIN_LOCK_ISR_TAKE(&pDrvCtrl->spinLock);
    tiWdtReload(pDrvCtrl);
    SPIN_LOCK_ISR_GIVE(&pDrvCtrl->spinLock);
    }

LOCAL int tiWdtSetTimeout(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl, UINT32 timeout)
    {
    SPIN_LOCK_ISR_TAKE(&pDrvCtrl->spinLock);
    pDrvCtrl->timeoutSec = timeout;
    SPIN_LOCK_ISR_GIVE(&pDrvCtrl->spinLock);
    return 0;
    }
LOCAL unsigned int tiWdtGetTimeLeft(OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl)
    {
    UINT32 value = OMAP3_READ_4(pDrvCtrl, OMAP_WATCHDOG_CRR);
    return GET_WCCR_SECS(value);
    }

/*******************************************************************************
*
* tiWatchdogShutdown - shutdown Watchdog device
*
* This routine turns off the hardware and puts it into a quiescent state.
*
* RETURNS: ERROR if the device is invalid, OK otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogShutdown
    (
    VXB_DEV_ID pDev
    )
    {
    OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl;   /* driver control */
    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = (OMAP3_WATCHDOG_DRVCTRL *)vxbDevSoftcGet(pDev);
    if(pDrvCtrl == NULL)
        {
        return ERROR;
        }

    tiWdtStop(pDrvCtrl);
    return OK;
    }

/*******************************************************************************
*
* tiWatchdogDetach - detach Watchdog device
*
* This routine implements the VxBus driver detach method.
*
* RETURNS: OK if driver detached, ERROR if device is invalid.
*
* ERRNO: N/A
*/

LOCAL STATUS tiWatchdogDetach
    (
    VXB_DEV_ID pDev
    )
    {
    OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl;   /* driver control */
    char                    name[MAX_DRV_NAME_LEN];

    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = (OMAP3_WATCHDOG_DRVCTRL *)vxbDevSoftcGet(pDev);
    if(pDrvCtrl == NULL)
        {
        return ERROR;
        }

    /* Remove the device from the I/O systems device list */
    snprintf(name, MAX_DRV_NAME_LEN, "/%s/%d", WATCHDOG_DEVICE, pDrvCtrl->unit);
    if(ERROR == watchdogDevRemove(name))
        {
        return ERROR;
        }

    vxbDevSoftcSet(pDev, NULL);
    vxbResourceFree(pDev, pDrvCtrl->pMemRes);
    vxbMemFree(pDrvCtrl);
    return OK;
    }

/*******************************************************************************
* tiWatchdogOpen - called by IOS watchdog driver when device is opened
*
* This routine implements the IOS device open method.
*
* RETURNS: OK if success , ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogOpen
    (
    VXB_DEV_ID pDev
    )
    {
    OMAP3_WATCHDOG_DRVCTRL * pDrvCtrl;   /* driver control */

    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = vxbDevSoftcGet(pDev);
    if(pDrvCtrl == NULL)
        {
        return ERROR;
        }
#if 0
    tiWdtStart(pDrvCtrl);
#endif
    return OK;
    }

/*******************************************************************************
*
* tiWatchdogClose - called by IOS watchdog driver when device is closed
*
* This routine implements the IOS device close method.
*
* RETURNS: OK if success, ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogClose
    (
    VXB_DEV_ID pDev
    )
    {
    OMAP3_WATCHDOG_DRVCTRL * pDrvCtrl;   /* driver control */

    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = vxbDevSoftcGet(pDev);
    if(pDrvCtrl == NULL)
        {
        return ERROR;
        }

    if(pDrvCtrl->tiWdtRcvClose)
        {
        tiWdtStop(pDrvCtrl);
        }
    else
        {
        OMAP3_WATCHDOG_DBG("WATCHDOG: Unexpected close, not stopping watchdog!\n");
        }

    pDrvCtrl->tiWdtRcvClose = FALSE;

    return(OK);
    }

/*******************************************************************************
* tiWatchdogRead - called by IOS watchdog driver when read from device
*
* This routine implements the IOS device read method.
*
* RETURNS: OK if success, ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogRead
    (
    VXB_DEV_ID pDev,
    UINT8     *buf,
    UINT32     len,
    UINT32    *realByte
    )
    {
    OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl;

    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = vxbDevSoftcGet(pDev);
    *realByte = tiWdtGetTimeLeft(pDrvCtrl);
#if 0
    tiWdtKeepalive(pDrvCtrl);
#endif
    OMAP3_WATCHDOG_DBG("!!!%s left = %d !!!\n",__FUNCTION__,*realByte);
    return(OK);
    }

/*******************************************************************************
* tiWatchdogWrite - called by IOS watchdog driver when write to device
*
* This routine implements the IOS device write method.
*
* RETURNS: OK if success, ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogWrite
    (
    VXB_DEV_ID pDev,
    UINT8     *buf,
    UINT32     len,
    UINT32    *realByte
    )
    {
    OMAP3_WATCHDOG_DRVCTRL * pDrvCtrl;   /* driver control */
    UINT32 i;

    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = vxbDevSoftcGet(pDev);
    tiWdtKeepalive(pDrvCtrl);

    if((buf != NULL)&&(len > 0))
        {
        /* In case it was previously set */
        pDrvCtrl->tiWdtRcvClose = FALSE;

        for(i = 0; i != len; i++)
            {
            if((char)*(buf+i) == 'V')
                {
                pDrvCtrl->tiWdtRcvClose = TRUE;
                OMAP3_WATCHDOG_DBG("WATCHDOG: Received close command.\n");
                }
            }
        *realByte = len;
        }
    return(OK);
    }

/*******************************************************************************
*
* tiWatchdogIoctl - called by IOS watchdog driver ioctl function
*
* This routine implements the IOS device ioctl method.
*
* RETURNS: OK if success
*          -EINVAL if parameter out of range
*          ERROR if other failed
*
* ERRNO: N/A
*/

LOCAL STATUS tiWatchdogIoctl
    (
    VXB_DEV_ID       pDev,
    int              request,
    _Vx_ioctl_arg_t  arg
    )
    {
    OMAP3_WATCHDOG_DRVCTRL *pDrvCtrl;   /* driver control */

    if(pDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = vxbDevSoftcGet(pDev);

    switch(request)
        {
        case WDIOC_KEEPALIVE:              /* 0x40047705 */
            tiWdtKeepalive(pDrvCtrl);
            break;
        case WDIOC_GETTIMELEFT:            /* 0x4004770a */
            *(UINT32 *)arg = tiWdtGetTimeLeft(pDrvCtrl);
            OMAP3_WATCHDOG_DBG("!!!%s left = %d !!!\n",__FUNCTION__,*(UINT32 *)arg);
            break;
        case WDIOC_SETOPTIONS:             /* 0x40047704 */
            if(*(UINT32 *)arg & WDIOS_DISABLECARD)     /* 0x0001 */
                {
                tiWdtStop(pDrvCtrl);
                }
            else if(*(UINT32 *)arg & WDIOS_ENABLECARD) /* 0x0002 */
                {
                tiWdtStart(pDrvCtrl);
                }
            else if(*(UINT32 *)arg & WDIOS_FORCERESET) /* 0x0004 */
                {
                tiWdtStart(pDrvCtrl);
                }
            else
                {
                return(ERROR);
                }
            break;
        case WDIOC_SETTIMEOUT:             /* 0xc0047706 */
            tiWdtSetTimeout(pDrvCtrl, *(UINT32 *)arg);
            tiWdtStart(pDrvCtrl);
            break;
        case WDIOC_GETTIMEOUT:             /* 0x40047707 */
            *(UINT32 *)arg = pDrvCtrl->timeoutSec;
            break;
        default:
            return ERROR;
        }
    return(OK);
    }

/*******************************************************************************
* tiWatchdogProbe - probe for device presence at specific address
*
* Check for ARM OMAP3 watchdog device at the specified base address.
*
* RETURNS: OK if probe passes and assumed a valid (or compatible) device,
* ERROR otherwise.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogProbe
    (
    struct vxbDev *pDev    /* device information */
    )
    {
    return vxbFdtDevMatch(pDev, &tiWatchdogMatch[0], NULL);
    }

/******************************************************************************
* tiWatchdogAttach - attach ARM OMAP3 Watchdog device
*
* This is the ARM OMAP3 Watchdog device initialisation routine.
*
* RETURNS: OK if driver attached, or ERROR if the device is invalid, clocks
* cannot be enabled or resources cannot be allocated.
*
* ERRNO: N/A
*/
LOCAL STATUS tiWatchdogAttach
    (
    VXB_DEV_ID pDev        /* Device information */
    )
    {
    VXB_FDT_DEV             *pFdtDev;
    OMAP3_WATCHDOG_DRVCTRL  *pDrvCtrl;
    char                     name[MAX_DRV_NAME_LEN];
    const void              *prop;
    VXB_WATCHDOG_INFO       *pCandidate;
    VXB_RESOURCE_ADR        *pResAdr;

    pFdtDev = vxbFdtDevGet(pDev);
    if(pFdtDev == NULL)
        {
        return ERROR;
        }

    pDrvCtrl = vxbMemAlloc(sizeof(OMAP3_WATCHDOG_DRVCTRL));
    if(pDrvCtrl == NULL)
        {
        return ERROR;
        }

    /* save instance ID */
    pDrvCtrl->pDev = pDev;

    /* WDT memory map */
    pDrvCtrl->pMemRes = vxbResourceAlloc(pDev, VXB_RES_MEMORY, 0);
    if(pDrvCtrl->pMemRes == NULL)
        {
        OMAP3_WATCHDOG_DBG("%s: memory resource allocation failed\n", __FUNCTION__);
        vxbMemFree(pDrvCtrl);
        return ERROR;
        }

    pResAdr = (VXB_RESOURCE_ADR *)pDrvCtrl->pMemRes->pRes;
    pDrvCtrl->regHandle = pResAdr->pHandle;
    pDrvCtrl->regBase = (VIRT_ADDR)pResAdr->virtual;

    /* set default time out value */
    prop = vxFdtPropGet(pFdtDev->offset, "timeout-sec", NULL);
    if((prop == NULL) || ((*(UINT32 *) prop) == 0))
        {
        pDrvCtrl->timeoutSec = min(TIMER_MARGIN_MAX, TIMER_MARGIN_DEFAULT);
        }
    else
        {
        pDrvCtrl->timeoutSec = min(TIMER_MARGIN_MAX, vxFdt32ToCpu(*(UINT32 *)prop));
        }
    OMAP3_WATCHDOG_DBG("!!! Ti Watchdog timeout - %d Sec !!!\n", pDrvCtrl->timeoutSec);

    pDrvCtrl->tiWdtRcvClose = FALSE;

    /* initialize the spinlock */
    SPIN_LOCK_ISR_INIT(&pDrvCtrl->spinLock, 0);

    /* set device context */
    vxbDevSoftcSet(pDev, (void *)pDrvCtrl);

    pDrvCtrl->unit = watchdogDrvNextUnit();
    pDrvCtrl->wdt_trgr_pattern = 0x1234;

    /* Add the device to the I/O systems device list */
    snprintf(name, MAX_DRV_NAME_LEN, "/%s/%d", WATCHDOG_DEVICE, pDrvCtrl->unit);

    pCandidate = (VXB_WATCHDOG_INFO *)vxbMemAlloc(sizeof(VXB_WATCHDOG_INFO));
    if(pCandidate == NULL)
        {
        vxbResourceFree(pDev, pDrvCtrl->pMemRes);
        vxbMemFree(pDrvCtrl);
        return ERROR;
        }

    pCandidate->isVirtual = FALSE;
    pCandidate->wdOpen    = tiWatchdogOpen;
    pCandidate->wdClose   = tiWatchdogClose;
    pCandidate->wdRead    = tiWatchdogRead;
    pCandidate->wdWrite   = tiWatchdogWrite;
    pCandidate->wdIoctl   = tiWatchdogIoctl;

    if(watchdogDevCreate(name, pDev, pCandidate) != OK)
        {
        vxbMemFree(pCandidate);
        vxbResourceFree(pDev, pDrvCtrl->pMemRes);
        vxbMemFree(pDrvCtrl);
        return ERROR;
        }
    vxbMemFree(pCandidate);

    prop = vxFdtPropGet(pFdtDev->offset, "enable", NULL);
    if((prop != NULL) && (vxFdt32ToCpu(*(UINT32 *)prop) == 1))
        {
        tiWdtStart(pDrvCtrl);
        OMAP3_WATCHDOG_DBG("!!! Ti Watchdog Start - %s %d !!!\n", vxbDevNameGet(pDev), vxbDevUnitGet(pDev));
        }
    else
        {
        OMAP3_WATCHDOG_DBG("!!! Ti Watchdog Attach - %s %d !!!\n", vxbDevNameGet(pDev), vxbDevUnitGet(pDev));
        }
    return OK;
    }

LOCAL VXB_DRV_METHOD vxbFdtOMAP3WatchdogMethods[] =
    {
    {VXB_DEVMETHOD_CALL(vxbDevProbe),    tiWatchdogProbe},
    {VXB_DEVMETHOD_CALL(vxbDevAttach),   tiWatchdogAttach},
    {VXB_DEVMETHOD_CALL(vxbDevShutdown), tiWatchdogShutdown},
    {VXB_DEVMETHOD_CALL(vxbDevDetach),   tiWatchdogDetach},
    VXB_DEVMETHOD_END
    };

VXB_DRV vxbTiFdtWdtDrv =
    {
    { NULL } ,
    "Ti_wdt",                        /* Name */
    "ARM Ti Watchdog",               /* Description */
    VXB_BUSID_FDT,                   /* Class */
    0,                               /* Flags */
    0,                               /* Reference count */
    &vxbFdtOMAP3WatchdogMethods[0]   /* Method table */
    };
VXB_DRV_DEF(vxbTiFdtWdtDrv)

