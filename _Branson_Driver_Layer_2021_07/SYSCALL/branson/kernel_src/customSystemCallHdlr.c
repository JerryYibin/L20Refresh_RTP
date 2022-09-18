/*
This source file holds an example of custom system call handler
*/

/* Header files */
#include <stdio.h>
#include <taskLib.h>

typedef struct
    {
    char *fmt;
    _Vx_usr_arg_t arg1;
    _Vx_usr_arg_t arg2;
    _Vx_usr_arg_t arg3;
    _Vx_usr_arg_t arg4;
    _Vx_usr_arg_t arg5;
    _Vx_usr_arg_t arg6;
    }logMsgScArgs;
int logMsgSc(logMsgScArgs *pArgs)
    {
    return printf(pArgs->fmt, pArgs->arg1, pArgs->arg2, pArgs->arg3, 
                  pArgs->arg4, pArgs->arg5, pArgs->arg6);
    }

/* Aux begin */
#include <timerDev.h>

#define AUX_TASK_NAME "/auxIsr"

typedef struct
    {
    _Vx_freq_t ticksPerSecond;
    }AuxClkRateSetScArgs;

static void auxIsr(TASK_ID taskId)
    {
    eventSend(taskId, VXEV01);
    }
STATUS sysAuxClkConnectInternalSc()
    {
    TASK_ID taskId = taskOpen(AUX_TASK_NAME, 0, 0, 0, NULL, 0, NULL, (FUNCPTR)NULL,0,0,0,0,0,0,0,0,0,0);
    if(taskId == NULL)
        {
        return ERROR;
        }
    return sysAuxClkConnect((FUNCPTR)auxIsr, (_Vx_usr_arg_t)taskId);
    }
void sysAuxClkDisableSc()
    {
    sysAuxClkDisable();
    }
void sysAuxClkEnableSc()
    {
    sysAuxClkEnable();
    }
STATUS sysAuxClkRateSetSc(AuxClkRateSetScArgs *rate)
    {
    return sysAuxClkRateSet(rate->ticksPerSecond);
    }
/* Aux end */

/* gpio begin */
#include <subsys/gpio/vxbGpioLib.h>

#define GPIO_TASK_NAME "/gpioIsr"

typedef struct
    {
    UINT32 id;
    }vxbGpioScArgs;
typedef struct
    {
    UINT32 id;
    UINT32 value;
    }vxbGpioValueScArgs;
typedef struct
    {
    UINT32 id;
    VOIDFUNCPTR fun;
    void *arg;
    }vxbGpioFunScArgs;
typedef struct
    {
    UINT32 id;
    INTR_TRIGER triger;
    INTR_POLARITY pol;
    }vxbGpioConfigScArgs;

STATUS vxbGpioAllocSc(vxbGpioScArgs *pArgs)
    {
    return vxbGpioAlloc(pArgs->id);
    }
STATUS vxbGpioFreeSc(vxbGpioScArgs *pArgs)
    {
    return vxbGpioFree(pArgs->id);
    }
BOOL vxbGpioIsFreeSc(vxbGpioScArgs *pArgs)
    {
    return vxbGpioIsFree(pArgs->id);
    }
STATUS vxbGpioSetDirSc(vxbGpioValueScArgs *pArgs)
    {
    return vxbGpioSetDir(pArgs->id, pArgs->value);
    }
UINT32 vxbGpioGetDirSc(vxbGpioScArgs *pArgs)
    {
    return vxbGpioGetDir(pArgs->id);
    }
UINT32 vxbGpioGetValueSc(vxbGpioScArgs *pArgs)
    {
    return vxbGpioGetValue(pArgs->id);
    }
STATUS vxbGpioSetValueSc(vxbGpioValueScArgs *pArgs)
    {
    return vxbGpioSetValue(pArgs->id, pArgs->value);
    }
STATUS vxbGpioSetDebounceSc(vxbGpioValueScArgs *pArgs)
    {
    return vxbGpioSetDebounce(pArgs->id, pArgs->value);
    }
STATUS vxbGpioIntConfigSc(vxbGpioConfigScArgs *pArgs)
    {
    return vxbGpioIntConfig(pArgs->id, pArgs->triger, pArgs->pol);
    }
static void gpioIsr(TASK_ID taskId)
    {
    eventSend(taskId, VXEV01);
    }
STATUS vxbGpioIntConnectInternalSc(vxbGpioScArgs *pArgs)
    {
    char taskName[12];
    TASK_ID taskId;

    sprintf(taskName, "%s%02x", GPIO_TASK_NAME, pArgs->id&0xff);
    taskId = taskOpen(taskName, 0, 0, 0, NULL, 0, NULL, (FUNCPTR)NULL,0,0,0,0,0,0,0,0,0,0);
    if(taskId == NULL)
        {
        return ERROR;
        }
    return vxbGpioIntConnect(pArgs->id, (VOIDFUNCPTR)gpioIsr, (void *)taskId);
    }
STATUS vxbGpioIntDisconnectSc(vxbGpioFunScArgs *pArgs)
    {
    return vxbGpioIntDisconnect(pArgs->id, pArgs->fun, pArgs->arg);
    }
STATUS vxbGpioIntEnableSc(vxbGpioFunScArgs *pArgs)
    {
    return vxbGpioIntEnable(pArgs->id, pArgs->fun, pArgs->arg);
    }
STATUS vxbGpioIntDisableSc(vxbGpioFunScArgs *pArgs)
    {
    return vxbGpioIntDisable(pArgs->id, pArgs->fun, pArgs->arg);
    }
/* gpio end */

/* eQEP begin */
#include <vxbEqepLib.h>
typedef struct
    {
    UINT32 eqep;
    }vxbEqepScArgs;
typedef struct
    {
    UINT32 eqep;
    int count;
    }vxbEqepCountScArgs;

int eqepDevNumGetSc()
    {
    return eqepDevNumGet();
    }
STATUS vxbEqepShowSc(vxbEqepScArgs *pArgs)
    {
    return vxbEqepShow(pArgs->eqep);
    }
STATUS vxbEqepGetMaxCountSc(vxbEqepScArgs *pArgs)
    {
    return vxbEqepGetMaxCount(pArgs->eqep);
    }
STATUS vxbEqepSetMaxCountSc(vxbEqepCountScArgs *pArgs)
    {
    return vxbEqepSetMaxCount(pArgs->eqep, pArgs->count);
    }
STATUS vxbEqepGetInitCountSc(vxbEqepScArgs *pArgs)
    {
    return vxbEqepGetInitCount(pArgs->eqep);
    }

STATUS vxbEqepSetInitCountSc(vxbEqepCountScArgs *pArgs)
    {
    return vxbEqepSetInitCount(pArgs->eqep, pArgs->count);
    }
INT32  vxbEqepGetPosCountSc(vxbEqepScArgs *pArgs)
    {
    return vxbEqepGetPosCount(pArgs->eqep);
    }
STATUS vxbEqepGetDirectionSc(vxbEqepScArgs *pArgs)
    {
    return vxbEqepGetDirection(pArgs->eqep);
    }
/* eQEP end */

/* ehrPWM begin */
#include <vxbEhrpwmLib.h>

#define EHRPWM_TASK_NAME "/pwmIsr"

typedef struct
    {
    UINT8 ehrpwm;
    }vxbEhrpwmScArgs;
typedef struct
    {
    UINT8  ehrpwm;
    UINT32 val;
    }vxbEhrpwmValueScArgs;
typedef struct
    {
    UINT8  ehrpwm;
    UINT32 val1;
    UINT32 val2;
    }vxbEhrpwmVal2ueScArgs;

int ehrpwmDevNumGetSc()
    {
    return ehrpwmDevNumGet();
    }

STATUS vxbEhrPwmGetTbStatusSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbStatus(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetEmulationSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmSetEmulation(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTBCTLSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetTBCTL(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetTBCTLSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTBCTL(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbPWMFreqSc(vxbEhrpwmVal2ueScArgs *pArg)
    {
    return vxbEhrPwmSetTbPWMFreq(pArg->ehrpwm, pArg->val1, pArg->val2);
    }

STATUS vxbEhrPwmGetTbPRDSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbPRD(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbPWMDirSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetTbPWMDir(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetTbPWMDirSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbPWMDir(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbCounterSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetTbCounter(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetTbCounterSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbCounter(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbClkSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetTbClk(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetTbClkSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbClk(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbPWMCfgSc(vxbEhrpwmVal2ueScArgs *pArg)
    {
    return vxbEhrPwmSetTbPWMCfg(pArg->ehrpwm, pArg->val1, pArg->val2);
    }

STATUS vxbEhrPwmGetTbPWMCfgSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbPWMCfg(pArg->ehrpwm);
    }

STATUS vxbEhrPwmGetTbAQConfASc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbAQConfA(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbAQConfASc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetTbAQConfA(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetTbAQConfBSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetTbAQConfB(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetTbAQConfBSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetTbAQConfB(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetCMPASc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetCMPA(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetCMPASc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetCMPA(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetCMPBSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetCMPB(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetCMPBSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetCMPB(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmSetAQSFRCSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetAQSFRC(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetAQSFRCSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetAQSFRC(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetIntEvtSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetIntEvt(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetIntEvtSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetIntEvt(pArg->ehrpwm);
    }

STATUS vxbEhrPwmSetIntPrdSc(vxbEhrpwmValueScArgs *pArg)
    {
    return vxbEhrPwmSetIntPrd(pArg->ehrpwm, pArg->val);
    }

STATUS vxbEhrPwmGetIntPrdSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetIntPrd(pArg->ehrpwm);
    }

STATUS vxbEhrPwmEnableIntSc(vxbEhrpwmVal2ueScArgs *pArg)
    {
    return vxbEhrPwmEnableInt(pArg->ehrpwm, pArg->val1, pArg->val2);
    }

STATUS vxbEhrPwmDisableIntSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmDisableInt(pArg->ehrpwm);
    }
static void ehrpwmIsr(TASK_ID taskId)
    {
    eventSend(taskId, VXEV01);
    }
STATUS vxbEhrPwmRegISRInternalSc(vxbEhrpwmVal2ueScArgs *pArg)
    {
    char taskName[12];
    TASK_ID taskId;

    sprintf(taskName, "%s%x", EHRPWM_TASK_NAME, pArg->ehrpwm&0xff);
    taskId = taskOpen(taskName, 0, 0, 0, NULL, 0, NULL, (FUNCPTR)NULL,0,0,0,0,0,0,0,0,0,0);
    if(taskId == NULL)
        {
        return ERROR;
        }
    return vxbEhrPwmRegisterISR(pArg->ehrpwm, (VOIDFUNCPTR)ehrpwmIsr, (void *)taskId, pArg->val1, pArg->val2);
    }
STATUS vxbEhrPwmGetIntStatusSc(vxbEhrpwmScArgs *pArg)
    {
    return vxbEhrPwmGetIntStatus(pArg->ehrpwm);
    }
/* ehrPWM end */

/* McSpi begin */
#include <vxbMcSpiLib.h>
typedef struct
    {
    int channel;
    SPI_HARDWARE *devInfo;
    SPI_TRANSFER *pPkg;
    }vxbMcSpiTransScArgs;
UINT32 mcspiDevNumGetSc()
    {
    return mcspiDevNumGet();
    }
STATUS vxbMcSpiTransSc(vxbMcSpiTransScArgs *pArgs)
    {
    return vxbMcSpiTrans(pArgs->channel, pArgs->devInfo, pArgs->pPkg);
    }
/* McSpi end */

/* I2C RTC begin */
#include <time.h>
extern STATUS vxbRtcGet(struct tm *);
extern STATUS vxbRtcSet(struct tm *);
typedef struct
    {
    struct tm *time;
    }vxbRtcScArgs;
STATUS vxbRtcSetSc(vxbRtcScArgs *pArg)
    {
    return vxbRtcSet(pArg->time);
    }
STATUS vxbRtcGetSc(vxbRtcScArgs *pArg)
    {
    return vxbRtcGet(pArg->time);
    }
/* I2C RTC end */

/* DCAN begin */
extern int canDevCount();
int canDevCountSc()
    {
    return canDevCount();
    }
/* DCAN end */

/* custom begin */
extern void vxbMsDelay(int delayTime);
typedef struct
    {
    int delayTime;
    }MsDelayScArgs;
void vxbMsDelaySc(MsDelayScArgs *pArgs)
    {
    vxbMsDelay(pArgs->delayTime);
    }

extern void bcopy(const void *, void *, size_t);

#define LOGDATA_SIZE           512

typedef STATUS (*FIOFORMATV_OUTPUT_FUNCPTR)(char *, size_t, _Vx_usr_arg_t);
typedef enum
    {
    DISABLE_LOGS,
    LOG_ERROR,
    LOG_WARNING,
    LOG_DEBUG,
    MAX_LOG_LEVELS
    }LOG_LEVEL_LIST;
typedef struct 
    {
    UINT8 LogLevel;
    UINT8 FileFlag;
    char Buffer[LOGDATA_SIZE]; 
    }LogMessage;
typedef struct 
    {
    UINT8 *logData;
    UINT32 lineNo;
    const UINT8 *fileName;
    UINT32 logLevel;
    UINT32 logToFile;
    _Vx_usr_arg_t arg1;
    _Vx_usr_arg_t arg2;
    _Vx_usr_arg_t arg3;
    }logScArgs;

static char *logStatus[MAX_LOG_LEVELS] =
    {
    0,
    "< E >",
    "< W >",
    "< D >",
    };
static UINT16 globalLogCount;

/*This function writes formatted string output into the output buffer*/
static STATUS printBuf(char *inbuf, size_t nbytes, char **outbuf)
    {
    bcopy(inbuf, *outbuf, nbytes);
    *outbuf += nbytes;
    if(nbytes > LOGDATA_SIZE)
        return ERROR;
    return OK;
    }

/*This function converts unformatted string to formatted string*/
static STATUS bufPrintf(char *buff, const char *fmt, ...)
    {
    INT32 nChars;
    va_list vaList;
    va_start(vaList, fmt);
    nChars = fioFormatV(fmt, vaList, (FIOFORMATV_OUTPUT_FUNCPTR)printBuf, (_Vx_usr_arg_t)&buff);
    va_end(vaList);
    return (nChars);
    }
static STATUS logInternal(logScArgs *logInf, UINT8 LogLevel, UINT8 fileFlag, int pri)
    {
    MSG_Q_ID loggerMsgQId;
    char LogingDataBuf[LOGDATA_SIZE] = {0};
    LogMessage loggerMsg = {0};

    if(logInf->logLevel == DISABLE_LOGS)
        {
        printf("### error in %s %d ###\n",__FUNCTION__,__LINE__);
        return ERROR;
        }
    if((logInf->logLevel >= LogLevel) && (logInf->logLevel < MAX_LOG_LEVELS))
        {
        globalLogCount++;

        sprintf(LogingDataBuf, "<LC: %d> ~ %s < Line No: %d > < File: %s > : %s \n",
                globalLogCount, logStatus[LogLevel], logInf->lineNo, logInf->fileName, (char *)logInf->logData);

        if(bufPrintf(loggerMsg.Buffer, LogingDataBuf, logInf->arg1, logInf->arg2, logInf->arg3)<0)
            {
            kprintf("%s: Error in Formatting String\n", __FUNCTION__);
            return ERROR;
            }

        loggerMsg.LogLevel = LogLevel;
        loggerMsg.FileFlag = fileFlag;

        loggerMsgQId = msgQOpen("/LOG_Task_Q", 0, 0, 0, 0, 0);
        if(loggerMsgQId == MSG_Q_ID_NULL)
            {
            kprintf("%s: Error in opening Message Queue\n", __FUNCTION__);
            kprintf("%s: %s\n", __FUNCTION__, loggerMsg.Buffer);
            return ERROR;
            }

        if(msgQSend(loggerMsgQId, (char *)&loggerMsg, sizeof(loggerMsg), NO_WAIT, pri) == ERROR)
            {
            kprintf("%s: Error in sending Message\n", __FUNCTION__);
            msgQClose(loggerMsgQId);
            return ERROR;
            }
        msgQClose(loggerMsgQId);
        }
    return OK;
    }
/*This Custom SystemCall gives Information of Errors */
STATUS logERRSc(logScArgs *logInf)
    {
    return logInternal(logInf, (UINT8)LOG_ERROR, (UINT8)logInf->logToFile, MSG_PRI_URGENT);
    }

/*This Custom SystemCall shows all Warnings */
STATUS logWARNSc(logScArgs *logInf)
    {
    return logInternal(logInf, (UINT8)LOG_WARNING, 0, MSG_PRI_NORMAL);
    }

/*This Custom SystemCall gives Debugging Information */
STATUS logDBGSc(logScArgs *logInf)
    {
    return logInternal(logInf, (UINT8)LOG_DEBUG, 0, MSG_PRI_NORMAL);
    }
/* custom end */

/* timestamp begin */
UINT32 sysTimestampFreqSc()
    {
    return sysTimestampFreq();
    }
UINT32 sysTimestampLockSc()
    {
    return sysTimestampLock();
    }
/* timestamp end */

#if 0

/*  RUN time feature */
#define EEPROM_PATH    "/eeprom/0" /*  eeprom path */

/* Ip Configuration */
#define IFF_UP 0x1
#define INTERFACE "cpsw"
#define INTERFACE_NAME "cpsw1"
#define UNIT_NUMBER 1
#define SUCCESS 1
#define FAILURE 0
#define GATEWAY_CMD_BUFSIZE 100

#define GPIO_PC_15V_LOW_PIN 0x24 /* PC GPIO1_4 pin allocated for interrupt to Shutdown 15V */
#define GPIO_PC_5V_LOW_PIN  0x25 /* PC GPIO1_5 pin allocated for interrupt to Shutdown 5V */
#define GPIO_PC_24V_LOW_PIN 0x26 /* PC GPIO1_6 pin allocated for interrupt to Shutdown 24V */
#define GPIO_24V_LOW_PIN    0x61 /* GPIO4_1 pin allocated for Shutdown Activity */

/* Global Variables */
TASK_ID taskId, ctrlTaskId;
TASK_ID taskIdServo;
TASK_ID taskIdGpio;

UINT8 logFmtBuffer[MAX_LOG_FMT_BUFSIZE]; /* Buffer to hold formatted string */



/*  RUN time feature start */
INT32 EepromFd; /*  Eeprom file descriptor */
UINT8 featureNeptune[FEATUREFLAG_SIZE]            =    FEATURE_NEPTUNE;          /* Neptune model default values */
UINT8 featureP1Pneumatic[FEATUREFLAG_SIZE]        =    FEATURE_P1_PNEUMATIC;      /* p1pneumatic model default values */
UINT8 featureP2Pneumatic[FEATUREFLAG_SIZE]        =    FEATURE_P2_PNEUMATIC;      /* p2pneumatic model default values */
UINT8 featureIIW[FEATUREFLAG_SIZE]                =    FEATURE_I_IW;              /* i(iw)model default values */
UINT8 featureMercury[FEATUREFLAG_SIZE]            =    FEATURE_MERCURY;         /* mercury model default values */
UINT8 featureSaturnP1[FEATUREFLAG_SIZE]           =    FEATURE_SATURNP1;          /* saturnp1 model default values */
UINT8 featureSaturnP2[FEATUREFLAG_SIZE]           =    FEATURE_SATURNP2;          /* saturnp2 model default values */
UINT8 featureMars[FEATUREFLAG_SIZE]               =    FEATURE_MARS;            /* mars model default values */
UINT8 featureJuno[FEATUREFLAG_SIZE]               =    FEATURE_JUNO;              /* juno model default values */
UINT8 featureVenus[FEATUREFLAG_SIZE]              =    FEATURE_VENUS;            /* venus model default values */
UINT8 featureVenusHH[FEATUREFLAG_SIZE]            =    FEATURE_VENUSHH;          /* Venus HH model default values */
#endif
#if 0
/*
 * copied from os\drv\vxbus-2.1.3.0\drv-2.0.5.0\src\i2c\vxbI2cPcfRtc.c, 
 * need be updated with it.
 */
typedef struct i2cRtcCtrlInfo
    {
    UINT32 ctrlRegStart;
    UINT32 ctrlRegSize;
    UINT32 timeRegStart;
    UINT32 timeRegSize;
    UINT32 alarmRegStart;
    UINT32 alarmRegSize;
    UINT32 readWithStop;

    UINT8  rtcStopCtrlVal;

    BOOL (*vxbI2cPcfRtcLowBattery) (VXB_DEV_ID pDev, char * i2cBuffRead);
    } I2C_RTC_CTRL_INFO;

typedef struct i2cRtcCtrl
    {
    VXB_DEV_ID          pDev;
    I2C_RTC_CTRL_INFO   i2cRtcInfo;
    UINT16              i2cAddr;
    VXB_RESOURCE  *     intRes;
    ISR_DEFER_JOB       isrDef;
    ISR_DEFER_QUEUE_ID  queueId;
    RTC_ALARM_FUNC      rtcAlarmFunc;  /* alarm callback routine */
    void *              rtcAlarmArg;   /* parameter of alarm callback */
    VXB_I2C_RTC_FUNC *  i2cRtc;
    SEM_ID              devSem;
    UINT32              gpioIntPin;
    INTR_TRIGGER        gpioTrigger;
    BOOL                gpioInt;
    BOOL                legacyInt;
    }I2C_RTC_CTRL;

#define I2C_BUF_COMMON_SIZE     0x30
#define I2C_RTC_WRDELAY         2       /* tBuf 1.3 us */

#define I2C_REG_CTRL1(pDrvCtrl)   \
        ((UINT8)(pDrvCtrl->i2cRtcInfo.ctrlRegStart + 0))

LOCAL STATUS vxbI2cPcfRtcRead
    (
    VXB_DEV_ID          pDev,
    UINT8 *             buf,
    UINT8               offset,
    UINT8               length
    )
    {
    I2C_RTC_CTRL *      pDrvCtrl = vxbDevSoftcGet (pDev);
    I2C_MSG             msg[2] = {{0}};
    UINT8               msgbuf;
    STATUS              status;

    msgbuf       = offset;
    msg[0].addr  = pDrvCtrl->i2cAddr;
    msg[0].scl   = FAST_MODE;
    msg[0].flags = I2C_M_WR;
    msg[0].buf   = &msgbuf;
    msg[0].len   = 1;

    if (pDrvCtrl->i2cRtcInfo.readWithStop)
        {
        if (OK != (status = vxbI2cDevXfer (pDev, msg, 1)))
            {
            return status;
            }
        }

    msg[1].addr   = pDrvCtrl->i2cAddr;
    msg[1].scl    = FAST_MODE;
    msg[1].buf    = buf;
    msg[1].len    = length;
    msg[1].flags  = I2C_M_RD;
    msg[1].wrTime = I2C_RTC_WRDELAY;
    if (pDrvCtrl->i2cRtcInfo.readWithStop)
        {
        status = vxbI2cDevXfer (pDev, &msg[1], 1);
        }
    else
        {
        status = vxbI2cDevXfer (pDev, msg, 2);
        }

    return status;
    }

/*  Custom system call Get RTC Low Voltage bit */
INT32 GetRTClowPwrSc (struct GetRTClowPwrScArgs *pArgs)
    {
    VXB_DEV_ID          pRtcDev;
    int                 offset = vxFdtPathOffset("/soc/i2c1@48070000/pcfrtc@51");
    I2C_RTC_CTRL        *pDrvCtrl;
    UINT8               i2cBuff[I2C_BUF_COMMON_SIZE] = {0};
    INT32               tm_lowPwr = 0;

    if(offset <= 0)
        {
        kprintf("\nfailed to obtain RTC node offset while Detecting the RTC low Power!!!\n");
        return 0;
        }

    pRtcDev = vxbFdtDevAcquireByOffset(offset);
    if(pRtcDev == NULL)
        {
        kprintf("\nfailed to acquire RTC device while Detecting the RTC low Power!!!\n");
        return 0;
        }

    vxbDevAcquire(pRtcDev);
    pDrvCtrl = (I2C_RTC_CTRL *)vxbDevSoftcGet(pRtcDev);
    if(pDrvCtrl == NULL)
        {
        kprintf("\nfailed to obtain RTC driver information while Detecting the RTC low Power!!!\n");
        (void)vxbDevRelease(pRtcDev);
        return 0;
        }

    (void)semTake (pDrvCtrl->devSem, WAIT_FOREVER);
    if(vxbI2cPcfRtcRead(pRtcDev, i2cBuff, I2C_REG_CTRL1 (pDrvCtrl),
        (UINT8)(pDrvCtrl->i2cRtcInfo.ctrlRegSize + pDrvCtrl->i2cRtcInfo.timeRegSize)) != OK)
        {
        kprintf("\nfailed to read data while Detecting the RTC low Power!!!\n");
        (void)semGive (pDrvCtrl->devSem);
        (void)vxbDevRelease(pRtcDev);
        return 0;
        }
    (void)semGive (pDrvCtrl->devSem);

    if (NULL != pDrvCtrl->i2cRtcInfo.vxbI2cPcfRtcLowBattery)
        {
        if (TRUE == (*(pDrvCtrl->i2cRtcInfo.vxbI2cPcfRtcLowBattery))(pRtcDev, (char *)i2cBuff))
            {
            tm_lowPwr = 1;
            kprintf("\nRTC Power is low!!!\n");
            }
        else
            {
            tm_lowPwr = 0;
            kprintf("\nRTC Power is OK\n");
            }
        }
    (void)semGive (pDrvCtrl->devSem);
    (void)vxbDevRelease(pRtcDev);
    return tm_lowPwr;
    }
#endif
#if 0
/**************************************************LOGGING LEVELS*****************************************************************/

/*This function gets the time and convert this broken time into string format*/
STATUS getTimeStamp(UINT8 *timebuff)
{
    struct tm rtcTimePar;
    UINT8 timeBuf[sizeof(RTCtimerStr)+1]; //Buffer for date and time

    if( vxbRtcGet(&rtcTimePar) == ERROR)
    {
        kprintf("\nError in Getting the RTC Time!!!\n");
        return ERROR;
    }
    bzero(timeBuf, sizeof(timeBuf));

    if(asctime_r (&rtcTimePar, timeBuf)==NULL)
    {
        kprintf("Error in Converting broken time into String format\n");
        return ERROR;
    }

    memcpy(timebuff, (char *)timeBuf, strlen(timeBuf));
    timebuff[strlen(timeBuf)-1]='\0';
    return OK;
}

#endif

#if 0
/**************************************************RUNTIME FEATURE*****************************************************************/

/******************************************************************************
*
* CheckEepromModelEntry
*
* This routine  is for checking eeprom whether model
* is present or not.
*
* RETURNS: TRUE if model present, OK if not
*
* ERRNO: N/A
*/

STATUS CheckEepromModelEntry(INT32 Offset,INT32 length)
{
    char EepromBuf;
    int status=TRUE;

    if ((EepromFd = open(EEPROM_PATH, O_RDWR, 0)) == ERROR)
    {
        kprintf("CheckEepromModelEntry: Failed to open the fd..\n");
    }

    ioctl (EepromFd, FIOSEEK, Offset);
    read(EepromFd, (char *)&EepromBuf, BYTESIZE);

    if(EepromBuf==RUNTIMEDATA)
    {
        status=OK;
    }

    return status;

}

/******************************************************************************
*
* CheckModel
*
* This routine  is for checking the feature present in the eeprom
*
* RETURNS: Model present in eeprom
*
* ERRNO: N/A
*/

ModelID CheckModel(INT32 Offset)
{
    INT32    ReadStatus;
    UINT8 ModelBuffer,modelID;

    EepromRead((Offset+FEATUREFLAG_SIZE-1),sizeof(UINT8),&ModelBuffer);
    modelID= (ModelBuffer & MODELID_MASK);

    return modelID;
}

/************************************************
 * getDefaultModel
 *
 * This routine reads the default model
 *
 * RETURN :TRUE on finding default model
 *
 */

STATUS getDefaultModel(UINT8 modelID,UINT8 *FeatureArray)
{

    if(modelID==NEPTUNE_ID)
    {
        memcpy(FeatureArray,featureNeptune,FEATUREFLAG_SIZE);
    }
    else if(modelID==P1_PNEUMATIC_ID)
    {
        memcpy(FeatureArray,featureP1Pneumatic,FEATUREFLAG_SIZE);
    }
    else if(modelID==P2_PNEUMATIC_ID)
    {
        memcpy(FeatureArray,featureP2Pneumatic,FEATUREFLAG_SIZE);
    }
    else if(modelID==I_IW_ID)
    {
        memcpy(FeatureArray,featureIIW,FEATUREFLAG_SIZE);
    }
    else if(modelID==MERCURY_ID)
    {
        memcpy(FeatureArray,featureMercury,FEATUREFLAG_SIZE);
    }
    else if(modelID==SATURNP1_ID)
    {
        memcpy(FeatureArray,featureSaturnP1,FEATUREFLAG_SIZE);
    }
    else if(modelID==SATURNP2_ID)
    {
        memcpy(FeatureArray,featureSaturnP2,FEATUREFLAG_SIZE);
    }
    else if(modelID==MARS_ID)
    {
        memcpy(FeatureArray,featureMars,FEATUREFLAG_SIZE);
    }
    else if(modelID==JUNO_ID)
    {
        memcpy(FeatureArray,featureJuno,FEATUREFLAG_SIZE);
    }
    else if(modelID==VENUS_ID)
    {
        memcpy(FeatureArray,featureVenus,FEATUREFLAG_SIZE);
    }
    else if(modelID==VENUSHH_ID)
    {
        memcpy(FeatureArray,featureVenusHH,FEATUREFLAG_SIZE);
    }
    else
    {
        return ERROR;
    }

    return TRUE;

}

/******************************************************************************
*
* changeModelWrite
*
* This routine  is for  writing the modified Featureflag into eeprom
*
* RETURNS: TRUE on write
*
* ERRNO: ERROR
*/

STATUS changeModelWrite()
{
    INT32     index1,index2,statusValue,ArrayIndex,ReadStatus,DefaultValue,WriteStatus;
    UINT8      ModelFeatureList[FEATUREFLAG_SIZE],modelID,DefaultFeature[FEATUREFLAG_SIZE],featureStatus=0;

    if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModelFeatureList))==ERROR)
    {
        printf(" changeModelWrite : Error in eeprom read\n");
        return ERROR;
    }

    modelID=CheckModel(EEPROM_MODEL_ADDRESS);
    getDefaultModel(modelID,DefaultFeature);

    for(ArrayIndex=0;ArrayIndex<RTF_LIST1_SIZE;ArrayIndex++)
        {
            for(index1=0;index1<NUMFLAG_INBYTES;index1++)
            {
                index2=NUMBITS_PERFLAG*index1;
                statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
                DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;

                if(statusValue==RTF_NA && (DefaultValue==RTF_NA))
                {
                    statusValue=DISABLE;
                }
                else if(statusValue==RTF_BASE && (DefaultValue==RTF_BASE))
                {
                    statusValue=ENABLE;
                }
                else if(statusValue==RTF_LTD && (DefaultValue==RTF_LTD))
                {
                    statusValue=ENABLE;
                }
                else if((statusValue==RTF_OPT) || (DefaultValue==RTF_OPT))
                {
                    if(statusValue==RTF_OPT)
                    {
                        statusValue=DISABLE;
                    }
                }
                featureStatus= featureStatus | (statusValue<<index2);
            }
            ModelFeatureList[ArrayIndex]=featureStatus;
            featureStatus=0;
        }

        for(ArrayIndex=RTF_LIST1_SIZE;ArrayIndex<(FEATUREFLAG_SIZE-1);ArrayIndex++)
        {
            for(index1=0;index1<(FEATUREFLAG_SIZE/3);index1++)
            {
                index2=NUMBITS_PERFLAG*index1;
                statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
                DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;
                if(statusValue==RTF_NA && (DefaultValue==RTF_NA))
                {
                    statusValue=DISABLE;
                }
                else if(statusValue==RTF_BASE && (DefaultValue==RTF_BASE))
                {
                    statusValue=ENABLE;
                }
                else if(statusValue==RTF_LTD && (DefaultValue==RTF_LTD))
                {
                    statusValue=ENABLE;
                }
                else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
                {
                    if(statusValue==3)
                    {
                        statusValue=DISABLE;
                    }
                }
                featureStatus= featureStatus | (statusValue<<index2);
            }

            ModelFeatureList[ArrayIndex]=featureStatus;
            featureStatus=0;
        }

        for(index1=0;index1<(RTF_LIST2_SIZE/2);index1++)
        {
            index2=NUMBITS_PERFLAG*index1;
            statusValue=(ModelFeatureList[(FEATUREFLAG_SIZE-1)]>>index2) & FLAGMASK;
            DefaultValue=(DefaultFeature[FEATUREFLAG_SIZE-1]>>index2) & FLAGMASK;
            if(statusValue==RTF_NA && (DefaultValue==RTF_NA))
            {
                statusValue=DISABLE;
            }
            else if(statusValue==RTF_BASE && (DefaultValue==RTF_BASE))
            {
                statusValue=ENABLE;
            }
            else  if(statusValue==RTF_LTD && (DefaultValue==RTF_LTD))
            {
                statusValue=ENABLE;
            }
            else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
            {
                if(statusValue==3)
                {
                    statusValue=DISABLE;
                }
            }

            featureStatus= featureStatus | (statusValue<<index2);
        }

        ModelFeatureList[FEATUREFLAG_SIZE-1]=featureStatus;
        featureStatus=0;

        for(index1=NUMFLAG_INBYTES;index1<RTF_LIST1_SIZE;index1++)
        {
            statusValue=(DefaultFeature[FEATUREFLAG_SIZE-1]>>index1) & MODELBIT_MASK;
            featureStatus= featureStatus | (statusValue<<index1);
        }

        ModelFeatureList[FEATUREFLAG_SIZE-1]=(ModelFeatureList[FEATUREFLAG_SIZE-1]|featureStatus);

        if((WriteStatus=EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModelFeatureList))==ERROR)
        {
            kprintf(" changeModelWrite : ERROR in EEPROM Write \n");
        }

        return TRUE;
}

/******************************************************************************
*
* RunTimeFeatureDisplay
*
* This routine  is for  displaying the features of the particular model.
*
* RETURNS: TRUE on display
*
* ERRNO: ERROR
*/

STATUS  RunTimeFeatureDisplay(int Offset,int length )
{
    UINT8 Feature_Name [][30]={    "MULTIMODE           ", "TIME                ", "ENERGY              ", "PEAK POWER          ",
                                "COLLAPSE DISTANCE   ", "ABSOLUTE DISTANCE   ", "GROUND DETECT       ",    "CONTINOUS           ",
                                "PRETRIGGER          ",    "AFTERBURST          ",     "ENERGYBRAKE         ","AMPLITUDE STEP      ",
                                "FORCE STEP          ",    "ALARM LOG           ",     "EVENT LOG           ",  "WELD DATA LOG       ",
                                "REPORTING           ",        "CRACKED HORN        ",        "USB                 ",         "BINARY              ",
                                "WEB SERVICES        ",        "OPC UA              ",        "USER I/O            ",        "ACTUATOR I/O        ",
                                "CYCLE COUNTER       ",        "BATCH COUNTER       ",        "POWER MATCH CURVE   ",        "SMARTCONVERTER      ",
                                "CONTROL SPEED       ",        "SEQUENCING          ",        "AUTOCONVERTERCOOLING",        "FIELD BUS           ",
                                "DASHBOARD           ",        "PASSWORD            ",        "MOBILE              ",        "OVERLAY             ",
                                "TRENDS              ",        "OPTITUNE            ","LIMITS              ",    "HD MODE             ",
                                "TEACH MODE          ",        "ADV COMM            ",    "EXTRA DATA STORAGE  ",        "TROUBLE SHOOT       ",
                                "CFR                  ","HOUR COUNTER         "    };

    INT32 index1,index2,statusValue,ArrayIndex,FeatureIndex=0,ReadStatus,DefaultValue;
    UINT8  ModelFeatureList[FEATUREFLAG_SIZE],FeatureCategory[FEATURECATEGORY_SIZE],modelID,DefaultFeature[FEATUREFLAG_SIZE];

    if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModelFeatureList))==ERROR)
    {
        kprintf(" RunTimeFeatureDisplay : Error in eeprom read\n");
        return ERROR;
    }

    modelID=CheckModel(EEPROM_MODEL_ADDRESS);
    getDefaultModel(modelID,DefaultFeature);

    kprintf("---------------------------------------------------------------------------------\n");
    kprintf(" \t FEATURES \t\t |\tFeature Category   |  Feature Status\t| \n");
    kprintf("---------------------------------------------------------------------------------\n");

    for(ArrayIndex=0;ArrayIndex<RTF_LIST1_SIZE;ArrayIndex++)
    {
        for(index1=0;index1<NUMFLAG_INBYTES;index1++,FeatureIndex++)
        {
            index2=NUMBITS_PERFLAG*index1;
            statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
            DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;

            if(DefaultValue==RTF_NA)
            {
                strcpy(FeatureCategory,"NOT SUPPORTED");
            }
            else if(DefaultValue==RTF_BASE)
            {
                strcpy(FeatureCategory,"BASE         ");
            }
            else if(DefaultValue==RTF_LTD)
            {
                strcpy(FeatureCategory,"LIMITED");
                statusValue=ENABLE;
            }
            else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
            {
                strcpy(FeatureCategory,"OPTIONAL");
                if(statusValue==RTF_OPT)
                {
                    statusValue=DISABLE;
                }
            }
            kprintf("  %d )    %s\t |\t %s \t   |         %d  \t|\n",FeatureIndex,Feature_Name[FeatureIndex],FeatureCategory,statusValue);
        }
    }

    for(ArrayIndex=RTF_LIST1_SIZE;ArrayIndex<(FEATUREFLAG_SIZE-1);ArrayIndex++)
    {
        for(index1=0;index1<(FEATUREFLAG_SIZE/3);index1++,FeatureIndex++)
        {
            index2=NUMBITS_PERFLAG*index1;
            statusValue=(ModelFeatureList[ArrayIndex]>>index2) & FLAGMASK;
            DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;

            if (DefaultValue==RTF_NA)
            {
                strcpy(FeatureCategory,"NOT SUPPORTED");
            }
            else if(DefaultValue==RTF_BASE)
            {
                strcpy(FeatureCategory,"BASE         ");
            }
            else if(DefaultValue==RTF_LTD)
            {
                strcpy(FeatureCategory,"LIMITED");
                statusValue=ENABLE;
            }
            else if(DefaultValue==RTF_OPT)
            {
                strcpy(FeatureCategory,"OPTIONAL");
                if(statusValue==RTF_OPT)
                {
                    statusValue=DISABLE;
                }
            }
            kprintf("  %d )    %s\t |\t %s \t   |         %d  \t|\n",FeatureIndex,Feature_Name[FeatureIndex],FeatureCategory,statusValue);
        }
    }

    for(index1=0;index1<(RTF_LIST2_SIZE/2);index1++,FeatureIndex++)
    {
        index2=NUMBITS_PERFLAG*index1;
        statusValue=(ModelFeatureList[(FEATUREFLAG_SIZE-1)]>>index2) & FLAGMASK;
        DefaultValue=(DefaultFeature[ArrayIndex]>>index2) & FLAGMASK;

        if(DefaultValue==RTF_NA)
        {
            strcpy(FeatureCategory,"NOT SUPPORTED");
        }
        else if(DefaultValue==RTF_BASE)
        {
            strcpy(FeatureCategory,"BASE         ");
        }
        else if(DefaultValue==RTF_LTD)
        {
            strcpy(FeatureCategory,"LIMITED");
            statusValue=ENABLE;
        }
        else if(statusValue==RTF_OPT || (DefaultValue==RTF_OPT))
        {
            strcpy(FeatureCategory,"OPTIONAL");
            if(statusValue==RTF_OPT)
            {
                statusValue=DISABLE;
            }
        }
        kprintf("  %d )    %s\t |\t %s \t   |         %d  \t|\n",FeatureIndex,Feature_Name[FeatureIndex],FeatureCategory,statusValue);
    }

    kprintf("---------------------------------------------------------------------------------\n");
    kprintf("\n");
    return TRUE;
}

/******************************************************************************
*
* DisplayModel
*
* This routine  is for checking and displaying the model present in the eeprom.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

STATUS DisplayModel(int Offset)
{
    INT32 ReadStatus;
    UINT8 ModelBuffer,modelID;

    if((ReadStatus=EepromRead((Offset+FEATUREFLAG_SIZE-1),BYTESIZE,(UINT8 *)&ModelBuffer))==ERROR)
    {
    kprintf(" DisplayModel : Error in eeprom read\n");
    return ERROR;
    }

    modelID= (ModelBuffer & MODELID_MASK);

    if(modelID==NEPTUNE_ID)
    {
        kprintf("            ******* MODEL IS NEPTUNE ******* \n\n");
    }
    else if(modelID==P1_PNEUMATIC_ID)
    {
        kprintf("            ******* MODEL IS P1 PNEUMATIC ******* \n\n");
    }
    else if(modelID==P2_PNEUMATIC_ID)
    {
        kprintf("            ******* MODEL IS P2 PNEUMATIC ******* \n\n");
    }
    else if(modelID==I_IW_ID)
    {
        kprintf("            ******* MODEL IS I(IW) ******* \n\n");
    }
    else if(modelID==MERCURY_ID)
    {
        kprintf("            ******* MODEL IS MERCURY ******* \n\n");
    }
    else  if(modelID==SATURNP1_ID)
    {
        kprintf("            ******* MODEL IS SATURNP1 ******* \n\n ");
    }
    else  if(modelID==SATURNP2_ID)
    {
        kprintf("            ******* MODEL IS SATURN P2 ******* \n\n ");
    }
    else if(modelID==MARS_ID)
    {
        kprintf("            ******* MODEL IS MARS ******* \n\n ");
    }
    else if(modelID==JUNO_ID)
    {
        kprintf("            ******* MODEL IS JUNO ******* \n\n");
    }
    else  if(modelID==VENUS_ID)
    {
        kprintf("            ******* MODEL IS VENUS ******* \n\n ");
    }
    else  if(modelID==VENUSHH_ID)
    {
        kprintf("            ******* MODEL IS VENUS-HH ******* \n\n ");
    }
    else
    {
        kprintf("            ******* WRONG MODEL ******* \n \n");
    }
    return TRUE;
}

/******************************************************************************
*
* ModifyFeature -Modify the feature of model
*
* This routine  is for modifying  the feature flags for model present
* in EEPROM
*
* RETURNS: Modified FeaturesFlag
*
* ERRNO: N/A
*/

STATUS ModifyFeature(int Offset,int length)
{
    INT32 value,UserChoice,FeatureChoice,ReadStatus;
    UINT8  Feature[FEATUREFLAG_SIZE],ModelBuffer,modelID,FeatureArray[FEATUREFLAG_SIZE],ModelFeatureList[FEATUREFLAG_SIZE];
    FeaturesFlag ModifiedFeature={0};

    if((ReadStatus=EepromRead( (Offset+FEATUREFLAG_SIZE-1),BYTESIZE,(UINT8 *)&ModelBuffer))==ERROR)
    {
        kprintf(" ModifyFeature : EEPROM read in Modify Feature\n");
        return ERROR;
    }

    modelID= (ModelBuffer & MODELID_MASK);

    if((ReadStatus=EepromRead(Offset ,FEATUREFLAG_SIZE,(UINT8 *)&Feature))==ERROR)
    {
        kprintf(" ModifyFeature : EEPROM read in Modify Feature\n");
        return ERROR;
    }

    memcpy(&ModifiedFeature,&Feature,length);

    // Logic to Enable/Disable features
    do
    {
        kprintf(" press '1' to enable/disable feature else press '0'\n");
        scanf("%d",&UserChoice);

        if(UserChoice==1)
        {
            kprintf("enter feature number (0-45)  \n");
            scanf("%d",&FeatureChoice);

            if(modelID==NEPTUNE_ID)
            {
                if ((FeatureChoice == FIELDBUS) || (FeatureChoice == HD_MODE) || (FeatureChoice == ADV_COMM))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);
                    if((FeatureChoice == FIELDBUS))
                        {
                            ModifiedFeature.RtFeature1.FieldBus_Flag_B31=value;
                        }
                    else if((FeatureChoice == HD_MODE))
                        {
                            ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
                        }
                        else if((FeatureChoice == ADV_COMM))
                        {
                            ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
                        }
                }
                else if (FeatureChoice ==CONTINOUS)
                {
                    kprintf("FEATURE NOT SUPPORTED \n\n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
            }

            if(modelID==P1_PNEUMATIC_ID)
            {
                if ((FeatureChoice ==REPORTING) || (FeatureChoice == AUTOCONVERTERCOOLING) || (FeatureChoice == FIELDBUS) ||
                    (FeatureChoice == OVERLAY) ||(FeatureChoice == TRENDS) || (FeatureChoice == HD_MODE) ||
                    (FeatureChoice == TEACH_MODE) ||(FeatureChoice == ADV_COMM) || (FeatureChoice == EXTRADATASTORAGE))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);
                    if((FeatureChoice == REPORTING))
                    {
                        ModifiedFeature.RtFeature1.Reporting_Flag_B16=value;
                    }
                    else if((FeatureChoice ==AUTOCONVERTERCOOLING ))
                    {
                        ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
                    }
                    else if((FeatureChoice == FIELDBUS))
                    {
                        ModifiedFeature.RtFeature1.FieldBus_Flag_B31=value;
                    }
                    else if((FeatureChoice == OVERLAY))
                    {
                        ModifiedFeature.RtFeature2.Overlay_Flag_B35 =value;
                    }
                    else if((FeatureChoice == TRENDS))
                    {
                        ModifiedFeature.RtFeature2.Trends_Flag_B36 =value;
                    }
                    else if((FeatureChoice == HD_MODE))
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
                    }
                    else if((FeatureChoice == TEACH_MODE))
                    {
                        ModifiedFeature.RtFeature2.TeachMode_Flag_B40 =value;
                    }
                    else if(FeatureChoice == ADV_COMM)
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
                    }
                    else if((FeatureChoice == EXTRADATASTORAGE))
                    {
                        ModifiedFeature.RtFeature2.ExtraDataStorage_Flag_B42 =value;
                    }
                }
                else if ((FeatureChoice == CONTINOUS) ||  (FeatureChoice == FORCESTEP) ||  (FeatureChoice == POWERMATCHCURVE) ||
                        (FeatureChoice == SMART_CONVERTER)||  (FeatureChoice == CONTROLSPEED)||  (FeatureChoice == OPTITIUNE)||
                        (FeatureChoice == CFR))
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n \n");
                }
            }

            if(modelID==P2_PNEUMATIC_ID)
            {
                if ((FeatureChoice == AUTOCONVERTERCOOLING)||(FeatureChoice == FIELDBUS)||(FeatureChoice == HD_MODE)||(FeatureChoice ==ADV_COMM))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);

                    if(FeatureChoice == AUTOCONVERTERCOOLING)
                    {
                        ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
                    }
                    else if(FeatureChoice == FIELDBUS)
                    {
                        ModifiedFeature.RtFeature1.FieldBus_Flag_B31=value;
                    }
                    else if(FeatureChoice == HD_MODE)
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
                    }
                    else if(FeatureChoice == ADV_COMM)
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
                    }
                }
                else if (FeatureChoice ==CONTINOUS)
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
            }

            if(modelID==I_IW_ID)
            {
                if ((FeatureChoice == ENERGY) || (FeatureChoice == COLLLAPSEDIST) ||(FeatureChoice == ABSOLUTEDIST))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);

                    if((FeatureChoice == ENERGY))
                    {
                        ModifiedFeature.RtFeature1.Energy_Flag_B2=value;
                    }
                    else if(FeatureChoice == COLLLAPSEDIST)
                    {
                        ModifiedFeature.RtFeature1.CollapseDist_Flag_B4=value;
                    }
                    else if((FeatureChoice == ABSOLUTEDIST))
                    {
                        ModifiedFeature.RtFeature1.AbsoluteDist_Flag_B5 =value;
                    }
                }
                else if ((FeatureChoice == TIMEMODE) ||(FeatureChoice == GROUND_DETECT) || (FeatureChoice == PRETRIGGER) ||
                        (FeatureChoice == AFTERBURST) ||  (FeatureChoice == ALARMLOG) || (FeatureChoice == EVENTLOG)||
                        (FeatureChoice == WELDDATALOG)||(FeatureChoice == CRACKEDHORN)|| (FeatureChoice ==USER_IO)||
                        (FeatureChoice == ACTUATOR_IO)||  (FeatureChoice == CYCLECOUNTER)|| (FeatureChoice == PASSWORD)||
                        (FeatureChoice == LIMITS)|| (FeatureChoice == HD_MODE))
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
                else
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
            }

            if(modelID==MERCURY_ID)
            {
                if ((FeatureChoice == DASHBOARD)|| (FeatureChoice == HD_MODE)||(FeatureChoice == ADV_COMM))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);
                    if((FeatureChoice == DASHBOARD))
                    {
                        ModifiedFeature.RtFeature2.DashBoard_Flag_B32=value;
                    }
                    else if((FeatureChoice == HD_MODE))
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39 =value;
                    }
                    else if((FeatureChoice == ADV_COMM))
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41 =value;
                    }
                }
                else if ((FeatureChoice == 28) ||  (FeatureChoice == OPTITIUNE))
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
            }

            if(modelID==SATURNP1_ID)
            {
                if ((FeatureChoice == AUTOCONVERTERCOOLING)||(FeatureChoice == DASHBOARD)||(FeatureChoice == HD_MODE)||
                    (FeatureChoice == REPORTING)||(FeatureChoice == TEACH_MODE)||(FeatureChoice == ADV_COMM)||(FeatureChoice == EXTRADATASTORAGE))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);

                    if(FeatureChoice == REPORTING)
                    {
                        ModifiedFeature.RtFeature1.Reporting_Flag_B16=value;
                    }
                    else if((FeatureChoice == AUTOCONVERTERCOOLING))
                    {
                        ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
                    }
                    else if(FeatureChoice == DASHBOARD)
                    {
                        ModifiedFeature.RtFeature2.DashBoard_Flag_B32=value;
                    }
                    else if(FeatureChoice == HD_MODE)
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
                    }
                    else if(FeatureChoice == TEACH_MODE)
                    {
                        ModifiedFeature.RtFeature2.TeachMode_Flag_B40=value;
                    }
                    else if((FeatureChoice == ADV_COMM))
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
                    }
                    else if(FeatureChoice == EXTRADATASTORAGE)
                    {
                        ModifiedFeature.RtFeature2.ExtraDataStorage_Flag_B42=value;
                    }
                }
                else if ((FeatureChoice == CONTROLSPEED) || (FeatureChoice == OPTITIUNE)|| (FeatureChoice == CFR))
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
            }

            if(modelID==SATURNP2_ID)
            {
                if ((FeatureChoice == AUTOCONVERTERCOOLING)||(FeatureChoice == DASHBOARD)||(FeatureChoice == HD_MODE) ||(FeatureChoice == ADV_COMM))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);

                    if(FeatureChoice == AUTOCONVERTERCOOLING)
                    {
                        ModifiedFeature.RtFeature1.AutoConverterCooling_Flag_B30=value;
                    }
                    else if(FeatureChoice == DASHBOARD)
                    {
                        ModifiedFeature.RtFeature2.DashBoard_Flag_B32=value;
                    }
                    else if(FeatureChoice ==HD_MODE)
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
                    }
                    else if((FeatureChoice == ADV_COMM))
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
                    }
                }
                else if ((FeatureChoice == CONTROLSPEED) || (FeatureChoice == OPTITIUNE))
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
            }

            if(modelID==MARS_ID)
            {
                if ((FeatureChoice == COLLLAPSEDIST) || (FeatureChoice == ABSOLUTEDIST))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);
                    if(FeatureChoice == COLLLAPSEDIST)
                    {
                        ModifiedFeature.RtFeature1.CollapseDist_Flag_B4=value;
                    }
                    else if(FeatureChoice ==ABSOLUTEDIST)
                    {
                        ModifiedFeature.RtFeature1.AbsoluteDist_Flag_B5=value;
                    }
                }
                else if ((FeatureChoice == TIMEMODE) ||  (FeatureChoice == ENERGY) ||  (FeatureChoice == GROUND_DETECT) ||
                        (FeatureChoice == CONTINOUS) || (FeatureChoice == PRETRIGGER) ||  (FeatureChoice == AFTERBURST) ||
                        (FeatureChoice == ENERGYBRAKE) || (FeatureChoice == ALARMLOG)||(FeatureChoice == EVENTLOG) ||
                        (FeatureChoice == WELDDATALOG)|| (FeatureChoice == CRACKEDHORN)||(FeatureChoice ==USER_IO)||
                        (FeatureChoice == ACTUATOR_IO) || (FeatureChoice == CYCLECOUNTER) ||  (FeatureChoice == PASSWORD) ||(FeatureChoice == LIMITS))
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
                else
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
            }

            if(modelID==JUNO_ID)
            {
                if ((FeatureChoice == ADV_COMM)  ||  (FeatureChoice == HD_MODE))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);
                    if(FeatureChoice == ADV_COMM)
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
                    }
                    else if(FeatureChoice == HD_MODE)
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
                    }
                }
                else if ((FeatureChoice == COLLLAPSEDIST) ||  (FeatureChoice == ABSOLUTEDIST)||  (FeatureChoice == AFTERBURST)||
                        (FeatureChoice == ENERGYBRAKE)||(FeatureChoice == FORCESTEP)|| (FeatureChoice == ALARMLOG)||(FeatureChoice == WELDDATALOG)
                        ||(FeatureChoice ==BATCHCOUNTER)||(FeatureChoice == AUTOCONVERTERCOOLING)||  (FeatureChoice == TEACH_MODE)||
                        (FeatureChoice == CFR)||(FeatureChoice == HOURCOUNTER))
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
                else
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
            }

            if(modelID==VENUS_ID)
            {
                if ((FeatureChoice == REPORTING)  ||  (FeatureChoice == OVERLAY) ||  (FeatureChoice == TRENDS) ||
                        (FeatureChoice == HD_MODE)||  (FeatureChoice == ADV_COMM))
                {
                    kprintf(" 1. ENABLE\n");
                    kprintf(" 0. DISABLE\n");
                    scanf("%d",&value);
                    if(FeatureChoice == REPORTING)
                    {
                        ModifiedFeature.RtFeature1.Reporting_Flag_B16=value;
                    }
                    else if(FeatureChoice == OVERLAY)
                    {
                        ModifiedFeature.RtFeature2.Overlay_Flag_B35=value;
                    }
                    else if(FeatureChoice == TRENDS)
                    {
                        ModifiedFeature.RtFeature2.Trends_Flag_B36=value;
                    }
                    else if(FeatureChoice == HD_MODE)
                    {
                        ModifiedFeature.RtFeature2.HDMode_Flag_B39=value;
                    }
                    else if(FeatureChoice == ADV_COMM)
                    {
                        ModifiedFeature.RtFeature2.AdvComm_Flag_B41=value;
                    }
                }
                else if ((FeatureChoice == TIMEMODE) ||(FeatureChoice == ENERGY) ||(FeatureChoice == PEAKPOWER) ||(FeatureChoice == GROUND_DETECT)
                        ||(FeatureChoice == CONTINOUS)|| (FeatureChoice == EVENTLOG) ||  (FeatureChoice == CRACKEDHORN) ||
                        (FeatureChoice == USER_IO)||(FeatureChoice == CYCLECOUNTER) ||(FeatureChoice == MOBILE))
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
                else
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
            }

            if(modelID==VENUSHH_ID)
            {
                if ((FeatureChoice == TIMEMODE) || (FeatureChoice == ENERGY)||(FeatureChoice == PEAKPOWER)||(FeatureChoice == GROUND_DETECT)
                ||(FeatureChoice == CONTINOUS)||(FeatureChoice == USER_IO) ||(FeatureChoice == CYCLECOUNTER) ||(FeatureChoice ==PASSWORD) ||
                (FeatureChoice == LIMITS))
                {
                    kprintf("BASE FEATURE/LIMITED ENABLED BY DEFAULT & CANNOT BE MODIFIED \n\n");
                }
                else
                {
                    kprintf("FEATURE NOT SUPPORTED \n");
                }
            }
        }
    }while(UserChoice);

    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&ModifiedFeature);
    changeModelWrite();
    kprintf(" \n");

    return TRUE;
}

/******************************************************************************
*
* ChangeModel - Change model
*
* This routine  is for changing the required model
*
* RETURNS: OK on successful change
*
* ERRNO: N/A
*/

STATUS ChangeModel(FeaturesFlag Flag)
{
    INT32    iChoice,ReadStatus;
    UINT8 modelID,FeatureArray[FEATUREFLAG_SIZE];
    FeaturesFlag CommonFeature;

    if((ReadStatus=EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&Flag))==ERROR)
    {
        kprintf(" ChangeModel : Error in eeprom read\n");
        return ERROR;
    }

    do
    {
        kprintf(" 1. To Change Model \n");
        kprintf(" 0. EXIT \n ");
        scanf(" %d",&iChoice);

        if(iChoice==1)
        {
            kprintf("    1.  NEPTUNE             \n ");
            kprintf("    2.  P1_PNEUMATIC     \n ");
            kprintf("    3.  P2_PNEUMATIC     \n ");
            kprintf("    4.  I(IW)             \n ");
            kprintf("    5.  MERCURY         \n ");
            kprintf("    6.  SATURN P1        \n ");
            kprintf("    7.  SATURN P2        \n ");
            kprintf("    8.  MARS             \n ");
            kprintf("    9.  JUNO             \n ");
            kprintf("    10. VENUS             \n ");
            kprintf("    11. VENUS HH         \n ");
            kprintf(" enter choice\n");
            scanf(" %d",&iChoice);

            if(iChoice==NEPTUNE)
            {
                kprintf("            ******* MODEL IS NEPTUNE ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==NEPTUNE_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureNeptune,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==P1_PNEUMATIC)
            {
                kprintf("            ******* MODEL IS P1 PNEUMATIC ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==P1_PNEUMATIC_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureP1Pneumatic,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==P2_PNEUMATIC)
            {
                kprintf("            ******* MODEL IS P2 PNEUMATIC ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==P2_PNEUMATIC_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureP2Pneumatic,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==I_IW)
            {
                kprintf("            ******* MODEL IS I(IW)****** \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==I_IW_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureIIW,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==MERCURY)
            {
                kprintf("            ******* MODEL IS MERCURY ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==MERCURY_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureMercury,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==SATURNP1)
            {
                kprintf("            ******* MODEL IS SATURN P1 ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==SATURNP1_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureSaturnP1,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==SATURNP2)
            {
                kprintf("            ******* MODEL IS SATURN P2 ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==SATURNP2_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureSaturnP2,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }

            }
            else if(iChoice==MARS)
            {
                kprintf("            ******* MODEL IS MARS ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==MARS_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureMars,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==JUNO)
            {
                kprintf("            ******* MODEL IS JUNO ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==JUNO_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureJuno,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
            else if(iChoice==VENUS)
            {
                kprintf("            ******* MODEL IS VENUS ******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==VENUS_ID)
                    {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    }
                else
                    {
                    memcpy(&CommonFeature,featureVenus,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    }
            }
            else if(iChoice==VENUSHH)
            {
                kprintf("            ******* MODEL IS VENUS HH******* \n\n");
                modelID=CheckModel(EEPROM_MODEL_ADDRESS);

                if(modelID==VENUSHH_ID)
                {
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
                else
                {
                    memcpy(&CommonFeature,featureVenusHH,FEATUREFLAG_SIZE);
                    EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&CommonFeature);
                    changeModelWrite();
                    RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                    ModifyFeature(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);
                }
            }
        }
    }while(iChoice);

    return TRUE;
}

/******************************************************************************
*
* RunTimeFeatureSc - Custom System Call for RunTime feature configuration
*
* This custom system call is for RunTime feature configuration.
*
* RETURNS: TRUE
*
* ERRNO: N/A
*/

STATUS RunTimeFeatureSc(void)
{

    INT32    EepromStatus,changeStatus,ReadStatus,WriteStatus;
    char Flag[FEATUREFLAG_SIZE],RunTimeData=RUNTIMEDATA;
    FeaturesFlag FeatureBits;

    EepromStatus=CheckEepromModelEntry(EEPROM_RUNTIMEENTRY_ADDRESS,BYTESIZE);

    if(EepromStatus==TRUE)  //default write when eeprom has no model
    {
        memcpy(&FeatureBits,&featureNeptune,FEATUREFLAG_SIZE);
        kprintf("                    EEPROM IS EMPTY \n\n");
        kprintf("            ******* DEFAULT MODEL WRITTEN IS NEPTUNE ******* \n\n");

        EepromWrite(EEPROM_RUNTIMEENTRY_ADDRESS,BYTESIZE,(char *)&RunTimeData);
        EepromWrite(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&FeatureBits);
        changeModelWrite();
           RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);

        if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&Flag))==ERROR)
        {
            kprintf(" RunTimeFeatureSc : ERROR in EEPROM Read \n");
        }

        memcpy(&FeatureBits,&Flag,FEATUREFLAG_SIZE);

        if((changeStatus=ChangeModel(FeatureBits))!=TRUE)
        {
            kprintf(" RunTimeFeatureSc : Error in Changing Model\n");
        }
    }
    else
    {
        DisplayModel(EEPROM_MODEL_ADDRESS);
        RunTimeFeatureDisplay(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE);

        if((ReadStatus=EepromRead(EEPROM_MODEL_ADDRESS,FEATUREFLAG_SIZE,(UINT8 *)&Flag))==ERROR)
        {
            kprintf(" RunTimeFeatureSc : ERROR in EEPROM Read \n");
        }

        memcpy(&FeatureBits,&Flag,FEATUREFLAG_SIZE);

        if((changeStatus=ChangeModel(FeatureBits))!=TRUE)
        {
            kprintf(" RunTimeFeatureSc : Error in Changing Model\n");
        }
    }
    return TRUE;
}

/*-----------------------IpConfiguration----------------------*/

/*
*ipConfigSc - Custom System Call for IP configuration
*
* This custom system call is for Internet protocol configuration
* for the second Interface of SC.
*
* RETURNS: SUCCESS, or FAILURE if not configured
*
* ERRNO: N/A
*/
STATUS IPConfigurationSc(struct IPConfigurationScArgs *ipArgs)
{
    uint8_t status = SUCCESS;
    char gatewayCmd[100]={0x00};

    sprintf(gatewayCmd, "add -net -netmask %s %s %s",ipArgs->subnetAddr,ipArgs->ipAddr,ipArgs->gatewayAddr);

    if(routec(gatewayCmd)==ERROR)
    {
        kprintf("IPConfigurationSc : Error in setting Gateway\n");
        status=FAILURE;
    }

    return status;
}

/*-----------------------QSPI Custom system Calls For AM5728 MASTER----------------------*/

/******************************************************************************
*QSPIFlashEraseSc - Custom System Call for QSPI erase
*
* This custom system call is for block by block erase of QSPI
*
* RETURNS: N/A
*/
void QSPIFlashEraseSc(struct QSPIFlashEraseScArgs * pArgs)
    {
    S25FL_Handle flashHandle;
    uint32_t blockCount;
    uint32_t startBlockNumber, endBlockNumber;
    flashHandle = vxbQspiLibSF25FLOpen();
    startBlockNumber = (pArgs->offset / QSPI_DEVICE_BLOCK_SIZE);
    endBlockNumber = ( (pArgs->offset + pArgs->length) / QSPI_DEVICE_BLOCK_SIZE);
    for (blockCount = startBlockNumber; blockCount <= endBlockNumber; blockCount++)
        {
        vxbQspiLibS25FLFlashBlockErase(flashHandle, blockCount);
        }
    vxbQspiLibSF25FLClose(flashHandle);
    return;
    }

/******************************************************************************
*QSPIFlashWriteSc - Custom System Call for QSPI Write.
*
* This custom system call is to write into the QSPI flash.
*
* RETURNS: Returns SUCCESS on Write success and FAIL on write fail.
*/
STATUS QSPIFlashWriteSc(struct QSPIFlashWriteScArgs * pArgs)
    {
    int retval=0;
    S25FL_Transaction flashTransaction;
    S25FL_Handle flashHandle;

    /* Update transaction parameters */
    flashTransaction.data       = (void *)pArgs->src;
    flashTransaction.address    = pArgs->offset;
    flashTransaction.dataSize   = pArgs->length;  /* In bytes */

    flashHandle = vxbQspiLibSF25FLOpen();
    /* Write buffer to flash */
    retval = vxbQspiLibSF25FLBufferWrite(flashHandle, &flashTransaction);
    vxbQspiLibSF25FLClose(flashHandle);

    return retval;
    }

/******************************************************************************
*QSPIFlashReadSc - Custom System Call for QSPI read.
*
* This custom system call is to read from the QSPI flash.
*
* RETURNS: Returns SUCCESS on read success and FAIL on read fail.
*/
STATUS QSPIFlashReadSc(struct QSPIFlashReadScArgs * pArgs)
    {
    S25FL_Handle flashHandle;
    int retval=0;
    S25FL_Transaction flashTransaction;

    /* Update transaction parameters */
    flashTransaction.data       = (void *)pArgs->dest;
    flashTransaction.address    = pArgs->offset;
    flashTransaction.dataSize   = pArgs->length;  /* In bytes */

    flashHandle = vxbQspiLibSF25FLOpen();

    /* Write buffer to flash */
    retval = vxbQspiLibSF25FLBufferRead(flashHandle, &flashTransaction);
    vxbQspiLibSF25FLClose(flashHandle);

    return retval;
    }
#endif
#if 0
/*-----------------------QSPI Custom system Calls For AM437x Slaves----------------------*/

/******************************************************************************
* QspiLibInitSc - This function calls the QSPILibInit() to initialize the QSPI
*
* RETURNS: N/A
*/
void QspiLibInitSc()
{
    QSPILibInit();
}

/******************************************************************************
* QspiLibPrintIdSc - This function calls the QSPILibPrintId()
*
* RETURNS: N/A
*/
void QspiLibPrintIdSc()
{
    QSPILibPrintId();
}

/******************************************************************************
* QspiLibBlockEraseSc - This function calls the erases the particular block of
*                          flash memory
*
* RETURNS: N/A
*/
void QspiLibBlockEraseSc(struct QspiLibBlockEraseScArgs *pArgs)
{
    QSPILibBlockErase(pArgs->block);
}

/******************************************************************************
* QspiLibWriteSc - This function calls the QSPI flash library to write
*                  into the flash memory
*
* RETURNS: int
*/
int QspiLibWriteSc(struct QspiLibWriteScArgs *pArgs)
{
    printf("\nIn Custom system calls write \n pArgs->dstOffsetAddr = %x \n pArgs->srcAddr = %x \np Args->length = %x ",pArgs->dstOffsetAddr,pArgs->srcAddr,pArgs->length);

    return(QSPILibWrite(pArgs->dstOffsetAddr,pArgs->srcAddr,pArgs->length));
}

/******************************************************************************
* QspiLibReadSc - This function calls the QSPI flash library to read
*                 from the flash memory
*
* RETURNS: int
*/
int QspiLibReadSc(struct QspiLibReadScArgs *pArgs)
{
    printf("\nIn Custom system calls read \n pArgs->srcOffsetAddr = %x \n pArgs->srcAddr = %x \np Args->length = %x ",pArgs->srcOffsetAddr,pArgs->dstAddr,pArgs->length);
    return(QSPILibRead(pArgs->srcOffsetAddr,pArgs->dstAddr,pArgs->length));
}
#endif
#if 0
/******************************************************************************
* TimeStampSc -   This function calls the system time stamp and returns ticks.
*
* RETURNS: float
*/
INT64 TimeStampSc()
    {
    return ((UINT64)sysTimestamp() * 1000)/sysTimestampFreq();
    }

/*
*Gpio24vLowShutdownIsr - Shutdown Interrupt Service Routine
*
* This ISR triggers when power goes off.
**
* ERRNO: N/A
*/
void Gpio24vLowShutdownIsr()
    {
    if(eventSend(ctrlTaskId, VXEV08) == ERROR)
        {
        kprintf("Error in sending an GPIO24VLow event\n");
        }
    }

/*
*Gpio24VLowShutdownSc - Custom System Call for Shutdown event
*
* This Custom System call triggers event to RTP Application to save the critical data when power goes off.
**
** RETURNS: SUCCESS, or FAILURE if not configured
*
* ERRNO: N/A
*/
STATUS Gpio24VLowShutdownSc()
    {
    int8_t status=SUCCESS;

    ctrlTaskId = taskOpen((char *)"/Ctrl_Task", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);

    /*Setting GPIO4_1 pin as input direction*/
    if (ERROR == vxbGpioSetDir (GPIO_24V_LOW_PIN, GPIO_DIR_INPUT))
        {
        (void)vxbGpioFree (GPIO_24V_LOW_PIN);
        kprintf("Gpio24VLowSc : Error in setting GPIO4_1 Input Direction\n");
        status=FAILURE;
        }

    /*Configuring GPIO4_1 pin as interrupt pin for poweroff*/
    if (ERROR == vxbGpioIntConfig (GPIO_24V_LOW_PIN, INTR_TRIGGER_EDGE, INTR_POLARITY_LOW))
        {
        (void)vxbGpioFree (GPIO_24V_LOW_PIN);
        kprintf("Gpio24VLowSc : Error in Configuring GPIO4_1 pin as Interrupt\n");
        status=FAILURE;
        }

    /*Registering ISR for GPIO4_1 pin*/
    if (ERROR == vxbGpioIntConnect (GPIO_24V_LOW_PIN, (VOIDFUNCPTR)Gpio24vLowShutdownIsr, (void *)NULL))
        {
        kprintf("Gpio24VLowSc : Error in Registering GPIO4_1 ISR\n");
        status=FAILURE;
        }

    /*Enabling GPIO4_1 interrupt pin*/
    if (ERROR == vxbGpioIntEnable (GPIO_24V_LOW_PIN, (VOIDFUNCPTR)Gpio24vLowShutdownIsr, (void *)NULL))
        {
        (void)vxbGpioIntDisconnect (GPIO_24V_LOW_PIN, (VOIDFUNCPTR)Gpio24vLowShutdownIsr, (void *)NULL);
        kprintf("Gpio24VLowSc : Error in Enabling GPIO4_1 Interrupt\n");
        status=FAILURE;
        }

    return status;
    }
#endif
#if 0
void PC24VIsr(INT32 tid)
{
    if(eventSend(taskId, VXEV05) == ERROR)
        kprintf("Error in sending an PC 24V Event\n");
}
//For PC 15V Monitor
void PcGpio15vLowIsr(void)
{
    if(eventSend(ctrlTaskId, VXEV06) == ERROR)
        kprintf("Error in sending an PC GPIO15VLow event\n");
}

STATUS PcGpio15VLowSc(void)
{
    int8_t status=SUCCESS;

    ctrlTaskId = taskOpen((char *)"/ControlTask", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);

    /*Setting GPIO1_4 pin as input direction*/
    if (ERROR == vxbGpioSetDir (GPIO_PC_15V_LOW_PIN, GPIO_DIR_INPUT))
    {
        (void)vxbGpioFree (GPIO_PC_15V_LOW_PIN);
        kprintf("PcGpio15VLowSc : Error in setting GPIO1_4 Input Direction\n");
        status=FAILURE;
    }

    /*Configuring GPIO1_4 pin as interrupt pin for poweroff*/
    if (ERROR == vxbGpioIntConfig (GPIO_PC_15V_LOW_PIN, INTR_TRIGGER_EDGE, INTR_POLARITY_LOW))
    {
        (void)vxbGpioFree (GPIO_PC_15V_LOW_PIN);
        kprintf("PcGpio15VLowSc : Error in Configuring GPIO1_4 pin as Interrupt\n");
        status=FAILURE;
    }

    /*Registering ISR for GPIO1_4 pin*/
    if (ERROR == vxbGpioIntConnect (GPIO_PC_15V_LOW_PIN, (VOIDFUNCPTR)PcGpio15vLowIsr, (void *)NULL))
    {
        kprintf("PcGpio15VLowSc : Error in Registering GPIO4_1 ISR\n");
        status=FAILURE;
    }

    /*Enabling GPIO1_4 interrupt pin*/
    if (ERROR == vxbGpioIntEnable (GPIO_PC_15V_LOW_PIN, (VOIDFUNCPTR)PcGpio15vLowIsr, (void *)NULL))
    {
        (void)vxbGpioIntDisconnect (GPIO_PC_15V_LOW_PIN, (VOIDFUNCPTR)PcGpio15vLowIsr, (void *)NULL);
        kprintf("PcGpio15VLowSc : Error in Enabling GPIO1_4 Interrupt\n");
        status=FAILURE;
    }

    return status;
}

//For PC 5V Monitor
void PcGpio5vLowIsr(void)
{
    if(eventSend(ctrlTaskId, VXEV07) == ERROR)
        kprintf("Error in sending an PC GPIO5VLow event\n");
}

STATUS PcGpio5VLowSc(void)
{
    int8_t status=SUCCESS;

    ctrlTaskId = taskOpen((char *)"/ControlTask", 0, 0, 0, 0, 0, 0 ,0,0,0,0,0,0,0,0,0,0,0);

    /*Setting GPIO1_5 pin as input direction*/
    if (ERROR == vxbGpioSetDir (GPIO_PC_5V_LOW_PIN, GPIO_DIR_INPUT))
    {
        (void)vxbGpioFree (GPIO_PC_5V_LOW_PIN);
        kprintf("PcGpio5VLowSc : Error in setting GPIO1_5 Input Direction\n");
        status=FAILURE;
    }

    /*Configuring GPIO1_5 pin as interrupt pin for poweroff*/
    if (ERROR == vxbGpioIntConfig (GPIO_PC_5V_LOW_PIN, INTR_TRIGGER_EDGE, INTR_POLARITY_LOW))
    {
        (void)vxbGpioFree (GPIO_PC_5V_LOW_PIN);
        kprintf("PcGpio5VLowSc : Error in Configuring GPIO1_5 pin as Interrupt\n");
        status=FAILURE;
    }

    /*Registering ISR for GPIO1_5 pin*/
    if (ERROR == vxbGpioIntConnect (GPIO_PC_5V_LOW_PIN, (VOIDFUNCPTR)PcGpio5vLowIsr, (void *)NULL))
    {
        kprintf("PcGpio5VLowSc : Error in Registering GPIO5_1 ISR\n");
        status=FAILURE;
    }

    /*Enabling GPIO1_5 interrupt pin*/
    if (ERROR == vxbGpioIntEnable (GPIO_PC_5V_LOW_PIN, (VOIDFUNCPTR)PcGpio5vLowIsr, (void *)NULL))
    {
        (void)vxbGpioIntDisconnect (GPIO_PC_5V_LOW_PIN, (VOIDFUNCPTR)PcGpio5vLowIsr, (void *)NULL);
        kprintf("PcGpio5VLowSc : Error in Enabling GPIO1_5 Interrupt\n");
        status=FAILURE;
    }

    return status;
}
#endif
#if 0
STATUS EmmcExtCsdDecodeSc(struct EmmcExtCsdDecodeScArgs *pArgs)
    {
    VXB_DEV_ID        pCtrlDev;
    VXB_DEV_ID        pMmcDev;
    int             offset = vxFdtPathOffset("/soc/sdmmc2@480b4000");
    MMC_CARD_CTRL   *pDrvCtrl;
    MMC_INFO         *pMmcInfo;

    if(offset <= 0)
        {
        return FAILURE;
        }

    pCtrlDev = vxbFdtDevAcquireByOffset(offset);
    if(pCtrlDev == NULL)
        {
        return FAILURE;
        }

    pMmcDev = vxbDevFirstChildGet(pCtrlDev);
    if(pMmcDev == NULL)
        {
        (void)vxbDevRelease(pCtrlDev);
        return FAILURE;
        }

    vxbDevAcquire(pMmcDev);
    pDrvCtrl = (MMC_CARD_CTRL *)vxbDevSoftcGet(pMmcDev);
    if(pDrvCtrl == NULL)
        {
        (void)vxbDevRelease(pMmcDev);
        (void)vxbDevRelease(pCtrlDev);
        return FAILURE;
        }

    pMmcInfo = (MMC_INFO *)(pDrvCtrl->pInfo);
    if(pMmcInfo == NULL)
        {
        (void)vxbDevRelease(pMmcDev);
        (void)vxbDevRelease(pCtrlDev);
        return FAILURE;
        }

    pArgs->emmc_est_life->LifeTimeEstA = pMmcInfo->extCsd[268];
    pArgs->emmc_est_life->LifeTimeEstB = pMmcInfo->extCsd[269];
    pArgs->emmc_est_life->PreEOLInfo =   pMmcInfo->extCsd[267];

    (void)vxbDevRelease(pMmcDev);
    (void)vxbDevRelease(pCtrlDev);
    return SUCCESS;
    }
#endif

