/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------

There are some common definitions for the whole project, such as feature debug flag,
database directory and alarm index. 
 
***************************************************************************/


#ifndef COMMONS_H_
#define COMMONS_H_

#ifndef AC_BOARD
#include "vxWorks.h"
#endif

#define SOCKET_DBG					0
#define LOGIN_DBG					0
#define HEIGHT_CALIBRATE_DBG		1
#define ADMIN_DBG					1
#define INCLUDE_TI_AM5708_BRANSON_LEVEL1	0
#define INCLUDE_TI_AM5708_BRANSON	1
#define INCLUDE_TI_AM5708_JN		0
#define UNITTEST_DATABASE			0

#define DEFAULT_20K_FREQUENCY			20000
#define DEFAULT_5_5KW_POWER				5500
#define DEFAULT_72UM_AMPLITUDE			72

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


/*
 * defines event bits in the master events byte in the PC RX PDO
 * Note: use these bit definitions with shifting 1 to the left
 */

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

/* Macros and variables */

#endif
