/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
/*
  * Commons.h
 *
 *  Created on: Feb 9, 2018
 *      Author: DShilonie
 *      
 *  Prototypes for various common code and macros that are used for all components, should be here.
 *  
 *  
 */


#ifndef COMMONS_H_
#define COMMONS_H_

#ifndef AC_BOARD
#include "vxWorks.h"
#endif

#define SOCKET_DBG
#define LOGIN_DBG
//MMC2/eMMC Storage
#define MMC_EMMC_DEV_NAME	"/mmc1"

#define PATH_SEPARATOR		"/"
//Database file names
#define COMMON_DB_FILE_NAME		"sample_common.db"
#define P1_BASE_DB_FILE_NAME	"sample_p1_base.db"
#define L20_DB_FILE_NAME		"sample_l20_base.db"

#define COMMON_DB_FILE_PATH_EMMC		MMC_EMMC_DEV_NAME PATH_SEPARATOR COMMON_DB_FILE_NAME
#define P1_BASE_DB_FILE_PATH_EMMC		MMC_EMMC_DEV_NAME PATH_SEPARATOR P1_BASE_DB_FILE_NAME
#define L20_DB_FILE_PATH_EMMC			MMC_EMMC_DEV_NAME PATH_SEPARATOR L20_DB_FILE_NAME

#define CONVERTER_COOLING 7
#define MIN_TRIGGER_FORCE 3  // Newtons

#define HMI_SOCKET_PORT   1600
#define AC_SOCKET_PORT    1601

/*
 * defines event bits in the master events byte in the PC RX PDO
 * Note: use these bit definitions with shifting 1 to the left
 */
typedef struct RxPDO_PC
{
	UINT16	MasterState;
	UINT16	Amplitudechange;
	UINT16	MasterEvents;
	UINT32  Frequency;
}RxPDO_PC;

typedef struct RxPDO_AC
{
	UINT32	SCState;
	UINT32	ACControlEvent;
	UINT32	ACOutputs;
}RxPDO_AC;

typedef struct TxPDO_PC
{
	UINT16  Amplitude;
	INT16  	Phase;
	UINT16  Power;
	UINT32  Frequency;
	UINT16  Current;
	UINT8   PC_StatusEvent;
	UINT8	PCState;
}TxPDO_PC;

typedef struct TxPDO_AC
{
	UINT32  ActualForce;
	UINT32 	ActualDistance;
	UINT32  ACStatusEvent;
	UINT32  ACState;
	UINT32  ACInputs;
}TxPDO_AC;


typedef enum{
	STATUS_TMR_INTERVAL,
	STATUS_POWER_UP_RECIPE,
	STATUS_PC_CLEAR_ALARMS,
	STATUS_PRETRIGGER_ENABLE,
	STATUS_AFTERBURST_ENABLE,
	STATUS_SOURCE_LOST,
}PC_STATUS_EVENTS;

typedef enum
{
	CTRL_PC_SONIC_DISABLE,
	CTRL_TMR_INTERVAL,
	CTRL_POWER_UP_RECIPE,
	CTRL_PC_CLEAR_ALARMS,
	CTRL_PRETRIGGER_ENABLE,
	CTRL_WELDTEST,
	CTRL_AFTERBURST_ENABLE,
	CTRL_ESTOP
} PC_CONTROL_EVENTS;

typedef enum
{
	EVENT_STEP_FORCE,
	EVENT_AC_CLEAR_ALARMS
} AC_CONTROL_EVENTS;

typedef enum {
	PART_CONTACT_FOUND,
	EN_FAULT,
	HOME_POS,
	READY_POS
} AC_STATUS_EVENTS;

// alarms process limits ID ranges
enum
{
	ALARM_PROCESS_LIMIT_PRA_START = 0x500,
	ALARM_PROCESS_LIMIT_PSA_START = 0x550,
};

enum ErrorTypePC {
  NO_ERROR = 					0,
  ERR_POWER_OVERLOAD = 			0x01,
  ERR_WELDERROR = 				0x01,
  ERR_CURRENT_OVERLOAD = 		0x02 ,
  ERR_VOLTAGE_OVERLOAD = 		0x04,
  ERR_TEMP_OVERLOAD = 			0x08,
  ERR_DDSFREQ_LOWSEEKLIMIT =	0x10,
  ERR_DDSFREQ_HIGHSEEKLIMIT = 	0x20,
  ERR_DDSFREQ_LOWWELDLIMIT = 	0x40,
  ERR_DDSFREQ_HIGHWELDLIMIT = 	0x80,
  ERR_PHASELIMIT_TIMEERROR = 	0x100,
  ERR_15V_LOST=					0x800,
  ERR_PWR_AVG_OVERLOAD = 		0x10000,
};

enum ErrorTypeAC {
  ERR_NONE = 				0,
  ERR_PART_WINDOW =			0x0001,
  ERR_RECALIBRATE =			0x0004,
};

typedef	enum 
{
	PREREADY, 
	SC_READY, 
	START_SWITCH, 
	WAIT_FOR_TRIGGER, 
	WELD_SONIC_ON, 
	HOLD, 
	WAIT_FOR_READY_POSITION, 
	SC_ALARM, 
	WAIT_FOR_PART_CONTACT,
	SEEK_SONIC_ON, 
	SCAN_SONIC_ON, 
	CALIBRATION_FIRST_POINT, 
	CALIBRATION_SECOND_POINT,
	TEST_SONICS_ON,
	HORN_DOWN,
	UNDEFINED_STATE,
	JOG_UP,
	JOG_DOWN,
	HORN_CLAMP_ALARM,
	TOOLING,
	POWER_UP,
	AC_TO_HOME,
	AC_TO_READY,
	SC_UNREACHABLE = 0xFF
 } SCSTATES;

typedef enum
{
	POWERUP,
	AC_READY,
	DOWN_STROKE,
// 	FORCE_CONTROL,
	RETURN_STROKE,
	AC_ALARM,
// 	FIND_PART_CONTACT,
	WELD_HOLD,
// 	HOMEING,
// 	FORCE_CALIBRATION,
// 	JOGGING_UP,
// 	JOGGING_DOWN,
	AC_ESTOP,
	GOING_TO_POSITION,
// 	SERVO_RESET
} ACSTATES;

typedef enum
{
	PC_READY,
	WELD_RUN,
	SEEK_RUN,
	WELD_HD_RUN,
	PC_ALARM, 
	SCAN_RUN,
	TEST_RUN
} PCSTATES;


/* Macros and variables */
extern TxPDO_PC *PC_TX;
extern TxPDO_AC *AC_TX;
extern RxPDO_PC *PC_RX;
extern RxPDO_AC *AC_RX;

#endif
