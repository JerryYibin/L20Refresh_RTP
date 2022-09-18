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
---------------------------- MODULE DESCRIPTION ----------------------------

    LoggerModule implemented to capture logs for debug with varies level.  
 
****************************************************************************/
#include <ioLib.h>
#include <sysLib.h>
#include <string.h>
#include "loggerModule.h"

static int      maxCount;            /* max number of line logged in active file */
static int      logCount     = 0;    /* Number of line logged in active file */
static UINT8    toggleFlag   = 0;    /* Used to decide active file to be logged */
static MSG_Q_ID loggerMsgRecvQId = MSG_Q_ID_NULL;
static char logFileName[2][LOGFILE_NAME_LEN];

/**************************************************************************//**
* \brief   - Counts number line present in logger file.
*
* \param   - None.
*
* \return  - UINT16 lineCount.
*
******************************************************************************/
static INT32 GetLineCount(const char *fileName)
    {
    FILE *fp;
    int   value;
    int   lineCount = 0;

    fp = fopen(fileName, "r");
    if(NULL == fp)
        {
        kprintf("Logger Module: File %s open failed\r\n",
                (_Vx_usr_arg_t)fileName,0,0,0,0,0);
        }
    else
        {
        for(value = fgetc(fp); value != EOF; value = fgetc(fp))
            {
            if(value == '\n')
                {
                lineCount++;
                }
            }
        fclose(fp);
        }
    return lineCount;
    }

/**************************************************************************//**
* \brief   - Updates the logCount on every power up with latest line count.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
static void UpdateLogCounter()
    {
    int numOfLine = GetLineCount(logFileName[0]);
    if(numOfLine >= maxCount)
        {
        numOfLine = GetLineCount(logFileName[1]);
        if(numOfLine < maxCount)
            {
            toggleFlag ^= 1; /* Decides which file to log(logFile1 or logFile2) */
            }
        }
    logCount = numOfLine;
    }

/**************************************************************************//**
* \brief   - Writes received logs into logFile1.txt or logFile2.txt based on
*     maximum number of lines configuration.
*
* \param   - char *buf - information to be logged into file.
*
* \return  - STATUS (OK or ERROR)
*
******************************************************************************/
static STATUS loggerFile(char *buf)
    {
    INT32 fd;
    STATUS ret;

    if(logCount >= maxCount) /* When Maximum line count reached, log to another file */
        {
        toggleFlag ^= 1; /* Decides which file to log(logFile1 or logFile2) */
        fd = open(logFileName[toggleFlag], O_TRUNC, 0);
        if(fd < 0)
            {
            kprintf("Logger Module: Error in truncating file Files_%d\r\n",toggleFlag+1,0,0,0,0,0);
            ret = ERROR;
            }
        else
            {
            logCount = 1; /* reset file count */ 
            close(fd);
            }
        }

    fd = open(logFileName[toggleFlag], O_RDWR | O_APPEND, 0);
    if(fd < 0)
        {
        kprintf("Logger Module: Files_%d does not exist with Error code : %d\r\n",toggleFlag+1, errno,0,0,0,0);
        ret = ERROR;
        }
    else
        {
        ret = write(fd, buf, strlen(buf));
        logCount++;
        close(fd);
        }
    return ret;
    }

/**************************************************************************//**
* \brief   - Logger module task function routine. Identifies log level and 
*     Based on log level takes action.
*     
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
static void loggerTask(void)
    {
    LogMessage logMsgBuffer;

    UpdateLogCounter();

    for(;;)
        {
        msgQReceive(loggerMsgRecvQId, (char *)&logMsgBuffer, MAX_SIZE_OF_RECVMSG_LENGTH, WAIT_FOREVER);
        switch(logMsgBuffer.LogLevel)
            {
            case LOG_ERROR:
                if(ENABLE_LOGFILE == logMsgBuffer.FileFlag)
                    {
                    if(loggerFile(logMsgBuffer.Buffer) < 0)
                        {
                        kprintf("Logger Module: Error in logging into a file\r\n",0,0,0,0,0,0);
                        }
                    }
                logMsgNoWait("%s%s%s", (_Vx_usr_arg_t)KRED, (_Vx_usr_arg_t)logMsgBuffer.Buffer, (_Vx_usr_arg_t)KNRM, 0, 0, 0);
                break;

            case LOG_WARNING:
                logMsg("%s%s%s", (_Vx_usr_arg_t)KYEL, (_Vx_usr_arg_t)logMsgBuffer.Buffer, (_Vx_usr_arg_t)KNRM, 0, 0, 0);
                break;

            case LOG_DEBUG:
                logMsg(logMsgBuffer.Buffer, 0, 0, 0, 0, 0, 0);
                break;
            }
        }
    }

/**************************************************************************//**
* \brief   - Initializes prerequisite for logger module.
*     
* \param   - None.
*
* \return  - STATUS (OK or ERROR)
*
******************************************************************************/
static void loggerInitInternal(char *logFile1, char *logFile2, int maxLineCount, int priority)
    {
    TASK_ID loggerTaskId;
    INT32 fd;
    int i;

    if((logFile1 == NULL)||(strlen(logFile1)>=LOGFILE_NAME_LEN))
        {
        sprintf(logFileName[0], "%s", LOGFILE1);
        }
    else
        {
        sprintf(logFileName[0], "%s", logFile1);
        }
    if((logFile2 == NULL)||(strlen(logFile2)>=LOGFILE_NAME_LEN))
        {
        sprintf(logFileName[1], "%s", LOGFILE2);
        }
    else
        {
        sprintf(logFileName[1], "%s", logFile2);
        }
    if(maxLineCount <= 0)
        maxCount = MAX_LOGS_COUNT;
    else
        maxCount = maxLineCount;

    for(i=0; i<2; i++)
        {
        fd = open(logFileName[i], O_RDWR | O_CREAT | O_EXCL, 0644);
        if(fd < 0)
            {
            if(EEXIST != errno)
                {
                kprintf("Logger Module: errno %d, waiting for storage device %s\r\n",
                        errno,(_Vx_usr_arg_t)logFileName[i],0,0,0,0);
                i--;
                taskDelay(sysClkRateGet()*2);
                }
            }
        else
            {
            close(fd);
            }
        }

    if(MSG_Q_ID_NULL == loggerMsgRecvQId)
        {
        loggerMsgRecvQId = msgQOpen("/LOG_Task_Q", MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO, OM_CREATE, 0);
        if(MSG_Q_ID_NULL == loggerMsgRecvQId)
            {
            kprintf("Logger Module: MessageQueue creation failed\r\n", 0,0,0,0,0,0);
            return;
            }
        }

    if((UINT32)priority > 255)
        {
        priority = LOGGER_TASK_PRIORITY;
        }
    loggerTaskId = taskSpawn(LOGGER_TASK_NAME, priority, 0, LOGGER_T_STACK_SIZE, (FUNCPTR)loggerTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if(TASK_ID_ERROR == loggerTaskId)
        {
        msgQDelete(loggerMsgRecvQId);
        kprintf("Logger Module: TaskSpwaning creation failed\r\n", 0,0,0,0,0,0);
        return;
        }
    }
void loggerInit(char *logFile1, char *logFile2, int maxLineCount, int priority)
    {
    taskSpawn(NULL, priority, 0, LOGGER_T_STACK_SIZE, (FUNCPTR)loggerInitInternal,
              (_Vx_usr_arg_t)logFile1, (_Vx_usr_arg_t)logFile2,
              (_Vx_usr_arg_t)maxLineCount, (_Vx_usr_arg_t)priority, 0, 0, 0, 0, 0, 0);
    }

