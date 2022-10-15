/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
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

#define SOCKET_DBG 0
#define LOGIN_DBG 0
#define HEIGHT_CALIBRATE_DBG 1
#define INCLUDE_TI_AM5708_BRANSON 1
#define INCLUDE_TI_AM5708_JN 0

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

#if 0 // redefinition in alarm_code_common.h
// alarms IDs
enum
{
	ALARM_NONE,
	
	ALARM_PHASE_OVERLOAD_OVA = 0x001,
	ALARM_CURR_OVERLOAD_OVA = 0x002,
	ALARM_FREQ_OVERLOAD_OVA = 0x003,
	ALARM_POWER_OVERLOAD_OVA = 0x004,
	ALARM_VOLT_OVERLOAD_OVA = 0x005,
	ALARM_TEMP_OVERLOAD_OVA = 0x006,
	
	ALARM_GND_DETECT_CMA = 0x303,
	ALARM_SONICS_MAX_CMA = 0x304,
	ALARM_NO_FORCE_STEP_CMA = 0x306,

	ALARM_TRIGGER_LOST_WRA = 0x401,
	ALARM_ACTUATOR_CLEAR_WRA = 0x417,
	ALARM_PEAK_POWER_CUTOFF_WRA = 0x41B,
	ALARM_ABSOLUTE_CUTOFF_WRA = 0x41C,
	ALARM_ACTUATOR_CAL_WRA = 0x41E,
	ALARM_COLLAPSE_CUTOFF_WRA = 0x41F,

	ALARM_SONICS_DISABLE_WRA = 0x421,
	ALARM_USB_MEM_FULL_WRA = 0x422,
	ALARM_DISK_MEM_FULL_WRA = 0x423,
	ALARM_CUSTOME1_CUTOFF_WRA = 0x424,
	ALARM_CUSTOME2_CUTOFF_WRA = 0x425,
	ALARM_FREQUENCY_LOW_CUTOFF_WRA = 0x426,
	ALARM_FREQUENCY_HIGH_CUTOFF_WRA = 0x427,
	ALARM_CUSTOME_DIGITAL_CUTOFF_WRA = 0x428,
    ALARM_ENERGY_CUTOFF_WRA =0x429,
	ALARM_GND_DETECT_CUTOFF_WRA = 0x42A,
	ALARM_TIME_CUTOFF_WRA = 0x42B,  
	ALARM_RTC_LOW_BATT_WRA = 0x444,
	ALARM_ALARM_LOG_FULL_WRA = 0x445,
	ALARM_EVENT_LOG_FULL_WRA = 0x446,
	ALARM_WELD_RESULT_FULL_WRA = 0x447,
	ALARM_WELD_GRAPH_FULL_WRA = 0x448,
	ALARM_HORN_SCAN_FULL_WRA = 0x449,
	ALARM_EEPROM_SYSCONFIG_WRA =0x450,
	
	ALARM_PEAKPOWER_LIMIT_MLR_PRA = 0x503,
	ALARM_PEAKPOWER_LIMIT_PLR_PRA = 0x504,
	ALARM_TIME_LIMIT_MLR_PRA = 0x505,
	ALARM_TIME_LIMIT_PLR_PRA = 0x506,
	ALARM_ENERGY_LIMIT_MLR_PRA = 0x507,
	ALARM_ENERGY_LIMIT_PLR_PRA = 0x508,
	ALARM_COLLAPSEDIST_LIMIT_MLR_PRA = 0x509,
	ALARM_COLLAPSEDIST_LIMIT_PLR_PRA = 0x50A,
	ALARM_ABSOLUTEDIST_LIMIT_MLR_PRA = 0x50B,
	ALARM_ABSOLUTEDIST_LIMIT_PLR_PRA = 0x50C,
	ALARM_TRIGGERDIST_LIMIT_MLR_PRA = 0x50D,
	ALARM_TIRGGERDIST_LIMIT_PLR_PRA = 0x50E,
	ALARM_WELDFORCE_LIMIT_MLR_PRA = 0x50F,
	ALARM_WELDFORCE_LIMIT_PLR_PRA = 0x510,
	ALARM_FREQUENCY_LIMIT_MLR_PRA = 0x512,
	ALARM_FREQUENCY_LIMIT_PLR_PRA = 0x513,

	ALARM_ENERGY_LIMIT_MLS_PSA =0x551,
	ALARM_ENERGY_LIMIT_PLS_PSA =0x552,
	ALARM_PEAKPOWER_LIMIT_MLS_PSA =0x553,
	ALARM_PEAKPOWER_LIMIT_PLS_PSA =0x554,
	ALARM_COLLAPSEDIST_LIMIT_MLS_PSA =0x555,
	ALARM_COLLAPSEDIST_LIMIT_PLS_PSA =0x556,
	ALARM_ABSOLUTEDIST_LIMIT_MLS_PSA =0x557,
	ALARM_ABSOLUTEDIST_LIMIT_PLS_PSA =0x558,
	ALARM_TRIGGERDIST_LIMIT_MLS_PSA =0x559,
	ALARM_TIRGGERDIST_LIMIT_PLS_PSA =0x55A,
	ALARM_WELDFORCE_LIMIT_MLS_PSA =0x55B,
	ALARM_WELDFORCE_LIMIT_PLS_PSA =0x55C,
	ALARM_TIME_LIMIT_MLS_PSA =0x55D,
	ALARM_TIME_LIMIT_PLS_PSA =0x55E,
	ALARM_FREQUENCY_LIMIT_MLS_PSA =0x563,
	ALARM_FREQUENCY_LIMIT_PLS_PSA =0x562,
	
	ALARM_START_SWITCH_CLOSED_EFA = 0x601,
	ALARM_GND_DETECT_EFA = 0x605,
	ALARM_START_SWITCH_LOST_EFA = 0x609,
	ALARM_ALARM_LOG_FULL_EFA = 0x611,
	ALARM_EVENT_LOG_FULL_EFA = 0x612,
	ALARM_WELD_RESULT_FULL_EFA = 0x613,
	ALARM_WELD_GRAPH_FULL_EFA = 0x614,
	ALARM_HORN_SCAN_FULL_EFA = 0x615,
	ALARM_START_SWITCH_TIME_EFA = 0x628,
	ALARM_DISK_MEM_FULL_EFA = 0x629,
	ALARM_INTERNAL_STORAGE_FAILURE_EFA = 0x62A,
	ALARM_RECALIBRATE_ACTUATOR_EFA = 0x62F,
	ALARM_EXT_DISK_NA_EFA = 0x631,
	ALARM_PART_PRESENT_ACTIVE_EFA = 0x634,
	ALARM_INTERNAL_COMM_USB_EFA = 0x635,
	ALARM_INTERNAL_COMM_ENET = 0x638,
	ALARM_INTERNAL_COMM_CABLE_EFA = 0x63A,
	ALARM_SONICS_SOURCE_LOST_EFA = 0x63C,
	ALARM_BUS_SCAN_FAILURE_EFA	= 0x63E, //TODO: NO DUPLICATES!!!!
	ALARM_INTERNAL_COMM_EFA = 0x63E,
 
	ALARM_INTERNAL_COMM_ECAT_EFA = 0x63F,
	
	ALARM_ESTOP_NCA = 0x700,
	ALARM_SONICS_DELAY_TIMEOUT_NCA = 0x703,
	ALARM_PART_WINDOW_ABORT_NCA = 0x706,
	ALARM_PART_CONTACT_PRETRIGGER_NCA = 0x708,
	ALARM_LLS_ABORT_BEFORE_TRS = 0x709,

	ALARM_CYCLE_ABORT_NCA = 0x70A,
	ALARM_ABSOLUTE_DISTANCE_NCA = 0x70C,
	ALARM_EXTERNAL_TOOLING_LOST_NCA = 0x714,
	ALARM_EXTERNAL_TOOLING_TIMEOUT_NCA = 0x715,
	ALARM_PART_PRESENT_LOST_NCA = 0x716,
	ALARM_SERVO_MOTOR_ERROR_NCA = 0x717,
	ALARM_READY_POSITION_TIMEOUT_NCA = 0x718,
	ALARM_RECIPE_NOT_VALID_NCA = 0x719,
	ALARM_BATCH_COUNT_NCA = 0x71A,
	ALARM_RECIPE_NOT_VALIDATED_NCA = 0x71B,
	
	ALARM_ELC_UNKNOWN = 0xEF0,
	ALARM_ELC_PB = 0xEF1,
	ALARM_ELC_24V = 0xEF2,
	ALARM_ELC_ESTOP = 0xEF3,
	ALARM_ELC_LE = 0xEF4,
	ALARM_ELC_SBEAM = 0xEF5,
	ALARM_ELC_TRS = 0xEF6,
	ALARM_ELC_DRIVE = 0xEF7,
	ALARM_ELC_CM = 0xEF8,
	ALARM_ELC_LU = 0xEF9,
	ALARM_ELC_SONICS_EN = 0xEFA
	
};
#endif
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
