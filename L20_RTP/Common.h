/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

/* VxWorks API support header files */
#include <sys/types.h>
#include <taskLib.h>
#include <msgQLib.h>
#include <eventLib.h>
#include <timerLib.h>
#include <sysLib.h>
#include <tickLib.h>
#include <semLib.h>
#include <sys/time.h>
#include <syscall.h>

/* Language specific header files */
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include "Logger.h"
#include "ScreenIndexUI.h"
#include "RequestCommandUI.h"

/* Macro defined for VxWorks message queue length */
#define MAX_SIZE_OF_MSG_LENGTH				768
#define MSG_SIZE							512
#define	MAX_SIZE_OF_MSG_BUFF 				(MAX_SIZE_OF_MSG_LENGTH - 4 * sizeof(UINT32) /* sizeof(ID) + sizeof(len) + sizeof(rspCode) + sizeof(crc))*/)
#define READ_USB_RESP_MSG_MAX_SIZE          2048
#define READ_USB_RESP_MSG_BUFFER_MAX_SIZE   READ_USB_RESP_MSG_MAX_SIZE - 8
#define TIMESTAMP							100

/* for RTC */
#define MIN_YEAR                100
#define MAX_YEAR                199
#define DAYS_IN_YEAR             365
#define DAY_IN_WEEK              7
#define JANUARY                  1
#define FEBRUARY                 2
#define DECEMBER                 12
#define IS_LEAP_YEAR(year)      (((year % 4 == 0) && (year % 100 != 0)) \
                                || year % 400 == 0)

/* Delay in ticks - seconds */
#define HALF_SEC_DELAY						(0.5 * sysClkRateGet())
#define ONE_SEC_DELAY						(1 * sysClkRateGet())
#define TWO_SEC_DELAY						(2 * sysClkRateGet())
#define FIVE_SEC_DELAY						(5 * sysClkRateGet())
#define FIFTY_MS_DELAY						((50 * sysClkRateGet()) / 1000)
#define FORTY_MS_DELAY						((40 * sysClkRateGet()) / 1000)
#define DEFAULT_TIMEINTERVAL_MS				1000

/* Event registers */
#define CTRL_1MS							VXEV01
#define DIG_HEARTBEAT_EVENT					VXEV02
#define	CON_EVENT							0x02
#define	DATA_TASK_EVENT						0x02
#define	ACT_TASK_TX_EVENT					0x10
#define	ACT_TASK_RX_EVENT					0x20
#define ACT_TASK_1MS_EVENT					0x40
#define ACT_TASK_QEVENT						0x80
#define PS_TASK_TX_EVENT					0x10
#define PS_TASK_RX_EVENT					0x20
#define PS_TASK_1MS_EVENT					0x40
#define PS_TASK_QEVENT						0x80
#define DGT_INPUT_QEVENT					0x80
#define SHUTDOWN_EVENT						VXEV08

struct MESSAGE
{
	UINT32	msgID;
	char	Buffer[MAX_SIZE_OF_MSG_LENGTH - sizeof(msgID)];	
};

/* Macro defined to UIC Response*/
enum UIC_IDENTITIES
{
	REQ_HEART_BEAT_IDX					= SCBL_HEART_BEAT_REQ,
	REQ_ACTIVE_RECIPE_INFO_IDX 			= SCBL_ACTIVE_RECIPE_BASIC_INFO_READ_REQ,
	
	REQ_SETUP_WELD_PARAM_IDX			= SCBL_SETUP_WELD_PARAM_WRITE_REQ,
	REQ_SETUP_QUALITY_PARAM_IDX			= SCBL_SETUP_QUALITY_PARAM_WRITE_REQ,
	REQ_SETUP_ADVANCED_PARAM_IDX 		= SCBL_SETUP_ADVANCED_PARAM_WRITE_REQ,
	
	REQ_SYSTEM_INFO_IDX					= SCBL_SYSINFO_READ_REQ,
	REQ_SYSTEM_CONGIF_READ_IDX			= SCBL_SYSCONFIG_READ_REQ,
	REQ_SYSTEM_CONGIF_WRITE_IDX			= SCBL_SYSCONFIG_WRITE_REQ,
	REQ_INITIALIZATION_IDX				= SCBL_INITIALIZATION_REQ,
	REQ_HEIGHT_CALIBRATE_CHECK_IDX		= SCBL_HEIGHT_CALIBRATE_CHECK_REQ,
	REQ_HEIGHT_CALIBRATE_START_IDX		= SCBL_HEIGHT_CALIBRATE_START_REQ,
	REQ_HEIGHT_CALIBRATE_CORRECT_IDX	= SCBL_HEIGHT_CALIBRATE_CORRECT_REQ,
	REQ_HEIGHT_CALIBRATE_ACCEPT_IDX		= SCBL_HEIGHT_CALIBRATE_ACCEPT_REQ,
	
	REQ_RECIPE_LIBRARY_READ_IDX			= SCBL_RECIPE_LIBRARY_READ_REQ,
	REQ_RECIPE_LIBRARY_NEXT_PAGE_IDX	= SCBL_RECIPE_LIBRARY_NEXT_PAGE_READ_REQ,
	REQ_GET_SELECTED_RECIPE_INFO_REQ	= SCBL_GET_SELECTED_RECIPE_INFO_REQ,
	REQ_DEFAULT_RECIPE_IDX				= SCBL_DEFAULT_RECIPE_REQ,
	REQ_INSERT_RECIPE_WRITE_IDX			= SCBL_INSERT_RECIPE_WRITE_REQ,
	REQ_UPDATE_RECIPE_IDX				= SCBL_UPDATE_RECIPE_REQ,
	REQ_DELETE_RECIPE_IDX				= SCBL_DELETE_RECIPE_REQ,
	REQ_RECIPE_ERROR_CODE_IDX			= SCBL_RECIPE_ERROR_CODE_REQ,
	
	REQ_USER_IO_INPUT_IDX				= SCBL_USER_IO_INPUT_REQ,
};

#endif /* COMMON_H_ */
