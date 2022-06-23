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

#define MIN_TRIGGER_FORCE 3  // Newtons

#define HMI_SOCKET_PORT   1600
#define AC_SOCKET_PORT    1601

// ACInputs bits of TxPDO_AC
#define SS1MASK 					0x01
#define SS2MASK 					0x02
#define GRDDETMASK 					0x04 
#define ULSMASK 					0x08
#define TRSMASK 					0x10 
#define BOTHSTARTSWITCHMASK 		0x03  	

/*
 * defines event bits in the master events byte in the PC RX PDO
 * Note: use these bit definitions with shifting 1 to the left
 */
typedef struct RxPDO_PC
{
	UINT32	MasterState;
	UINT32	TargetAmplitude;
	UINT32	MasterEvents;
	UINT32  StartFrequency;
	UINT32  AmplitudeRampTime;
	INT32   AmpProportionalGain;
	INT32   AmpIntegralGain;
	INT32   AmpDerivativeGain;
	INT32   PhaseProportionalGain;
	INT32   PhaseIntegralGain;
	INT32   PhaseDerivativeGain;
}RxPDO_PC;

typedef struct RxPDO_AC
{
	UINT32	SCState;
	UINT32	ACControlEvent;
	UINT32	ACOutputs;
}RxPDO_AC;

typedef struct TxPDO_PC
{
	UINT32  Amplitude;
	INT32  	Phase;
	UINT32  Power;
	UINT32  Frequency;
	UINT32  Current;
	UINT32  PC_StatusEvent;
	UINT32	PCState;
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
	SONICS_ON_OFF_STATUS,
	POWER_OVERLOAD,    
	VOLTAGE_OVERLOAD,
	CURRENT_OVERLOAD,
	TEMPERATURE_OVERLOAD
}DSP_TX_STATUS;

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


typedef enum
{
	CONVERTER_COOLING = 7
} AC_OUTPUT_BIT;

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
	PC_WELD_RUN,
	PC_SEEK_RUN,
	PC_WELD_HD_RUN,
	PC_ALARM, 
	PC_SCAN_RUN,
	PC_TEST_RUN,
	PC_ESTOP
} PCSTATES;

/* Macros and variables */
extern TxPDO_PC *PC_TX;
extern TxPDO_AC *AC_TX;
extern RxPDO_PC *PC_RX;
extern RxPDO_AC *AC_RX;

#endif
