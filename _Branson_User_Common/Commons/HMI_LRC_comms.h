/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------
 
 All the definition that need to be used both on ASC and HMI software should be in here.  
 
***************************************************************************/

#ifndef SHARED_COMMON_H_
#define SHARED_COMMON_H_
enum RequestIdentities
{
	SCBL_HEART_BEAT_REQ							= 0,
	SCBL_USER_DETAILS_READ_REQ					= 3,
	SCBL_PASSCODE_VALIDATE_REQ					= 4,
	SCBL_ACTIVE_RECIPE_BASIC_INFO_READ_REQ		= 5,
	SCBL_WELD_RECIPE_LIST_READ_REQ				= 6,
	SCBL_SYSINFO_READ_REQ						= 7,
	SCBL_WELDRECIPE_REQ							= 8,
	SCBL_GRAPH_READ_REQ,
	SCBL_RECIPE_LIBRARY_READ_REQ,
	SCBL_GET_RECIPE_INFO_READ_REQ,
	SCBL_CREATE_NEW_RECIPE_READ_REQ,
	SCBL_INSERT_RECIPE_READ_REQ,
	SCBL_UPDATE_RECIPE_READ_REQ,
	SCBL_DELETE_RECIPE_READ_REQ,
	SCBL_SET_ACTIVE_RECIPE_READ_REQ,
	SCBL_SETUP_WELD_PARAM_WRITE_REQ,
	SCBL_SETUP_QUALITY_PARAM_WRITE_REQ,
	SCBL_SETUP_ADVANCED_PARAM_WRITE_REQ,
	SCBL_ACTIVE_RECIPE_VALIDATE_REQ,
	SCBL_TEACH_RECIPE_READ_REQ,
	SCBL_TEACH_RECIPE_INITIAL_REQ,
	SCBL_TEAHMODE_RECIPE_VALIDATE_REQ,
	SCBL_TEACH_RECIPE_ACCEPT_REQ,
	SCBL_ALARM_RESET_READ_REQ,
	SCBL_GET_PERMISSION_SCREEN_REQ,
	SCBL_SET_PERMISSION_SCREEN_REQ,
	SCBL_GET_PASSCODE_LIST_REQ,
	SCBL_UPDATE_PASSCODE_REQ,
	DUMMY
};

enum SCREENINDEX
{
    DASHBORD_SCREEN = 0,
    MENU_OPTIONS_SCREEN = 1,
    WELD_SETTING_SCREEN = 2,
    WELD_MODE_SCREEN,
    WELD_LIMITS_SCREEN,
    ADVANCED_WELD_SETTINGS_SCREEN,
    PRESET_MENU_SCREEN,
    PRESET_LIBRARY_SCREEN,
    MAINTENANCE_MENU_SCREEN,
    HEIGHT_CALIBRATION_SCREEN,
    SONICS_GENERATOR_SCREEN,
    SENDING_SCREEN,
    MAINTENACE_COUNTER_LIMITS_SCREEN,
    SEQUENCE_SCREEN,
    SEQUENCE_MENU_SCREEN,
    SEQUENCE_STEPS_SCREEN,
    SEQUENCE_LIBRARY_SCREEN,
    SEQUENCE_ERROR_SCREEN,
    HEIGHT_ADJUSTMENT_SCREEN,
    SYSTEM_CONFIGURATION_SCREEN,
    TEACH_MODE_SCREEN,
    TEACH_MODE_SETUP_SCREEN,
    PASSWORD_CHANGING_SCREEN,
    GATEWAY_FEATURE_SCREEN,
    DIAGNOSTIC_SCREEN,
    SYSTEM_INFORMATION_SCREEN,
    EVNET_LOG_SCREEN,
    SCREEN_END
};

//system information structure for the HMI response parsing 
#define SYSINFO_SIZE						16
#define MAC_ADDR_SIZE      					18
struct SYSTEM_INFO
{
	INT32	psLifeCounter;
	INT32   actuatorlifecounter;
	INT32   generalAlarmCounter;
	INT32   overloadAlarmCounter;
	INT32	actuatorOverloads;
	INT32	actuatorStrokeLength;
	UINT16  psFrequency;
	INT32   psWatt;
	UINT8   calibrationStatus;
	UINT8   psType;
	UINT8   actuatorType;	
	char    modelName[SYSINFO_SIZE];
	char    version_SC[SYSINFO_SIZE];
	char    version_PC[SYSINFO_SIZE];
	char    version_AC[SYSINFO_SIZE];
//		char 	pc_FPGA_Versoin[SYSINFO_SIZE];
	char    psAssemblyNumber[SYSINFO_SIZE];
	char    actuatorAssembly[SYSINFO_SIZE];
	char    stackAssembly[SYSINFO_SIZE];
	char    psMacID[MAC_ADDR_SIZE];
	char    psIP[SYSINFO_SIZE];
	char    dateTime[2 * SYSINFO_SIZE];
	UINT16  crc_SC;
//		UINT16  crc_PC;
	UINT16  crc_AC;
};
enum COOLING
{
	DISABLE = -1,
	SECOND_PER_100JOULE,
	STANDARD
};
struct SYSTEM_CONFIG
{
	bool HeightEncoder;
	bool Seek;
	bool FootPedalAbort;
	bool LockOnAlarm;
	bool GraphData;
	bool DoubleHitMode;
	COOLING CoolingType;
	unsigned int CoolingDuration;
	unsigned int CoolingDelay;
	unsigned int PowerSupply;
	unsigned int VerifiedAmplitude;
	unsigned int SystemFrequency;
	unsigned int FrequencyOffset;
	unsigned int TunePoint;
	unsigned int HomePosition;
};

struct HEARTBEAT
{
	unsigned int 	CycleCounter;
	unsigned int 	RecipeNumber;
	unsigned int	AlarmCode;
	unsigned int 	TotalEnergy;
	unsigned int 	TriggerPress;
	unsigned int 	WeldPress;
	unsigned int 	Amplitude;
	unsigned int 	WeldTime;
	unsigned int 	PeakPower;
	unsigned int 	PreHeight;
	unsigned int	PostHeight;
};
#if 0
// alarms IDs
enum ALARM_CODE
{
	ALARM_NONE					= 0x000,
	
	ALARM_PHASE_OVERLOAD_OVA 	= 0x001,
	ALARM_CURR_OVERLOAD_OVA 	= 0x002,
	ALARM_FREQ_OVERLOAD_OVA 	= 0x003,
	ALARM_POWER_OVERLOAD_OVA 	= 0x004,
	ALARM_VOLT_OVERLOAD_OVA 	= 0x005,
	ALARM_TEMP_OVERLOAD_OVA 	= 0x006,
	
	ALARM_GND_DETECT_CMA 		= 0x303,
	ALARM_SONICS_MAX_CMA 		= 0x304,
	ALARM_NO_FORCE_STEP_CMA 	= 0x306,

	ALARM_TRIGGER_LOST_WRA 		= 0x401,
	ALARM_ACTUATOR_CLEAR_WRA 	= 0x417,
	ALARM_PEAK_POWER_CUTOFF_WRA = 0x41B,
	ALARM_ABSOLUTE_CUTOFF_WRA 	= 0x41C,
	ALARM_ACTUATOR_CAL_WRA 		= 0x41E,
	ALARM_COLLAPSE_CUTOFF_WRA 	= 0x41F,

	ALARM_SONICS_DISABLE_WRA 	= 0x421,
	ALARM_USB_MEM_FULL_WRA 		= 0x422,
	ALARM_DISK_MEM_FULL_WRA 	= 0x423,
	ALARM_CUSTOME1_CUTOFF_WRA 	= 0x424,
	ALARM_CUSTOME2_CUTOFF_WRA 			= 0x425,
	ALARM_FREQUENCY_LOW_CUTOFF_WRA 		= 0x426,
	ALARM_FREQUENCY_HIGH_CUTOFF_WRA 	= 0x427,
	ALARM_CUSTOME_DIGITAL_CUTOFF_WRA 	= 0x428,
    ALARM_ENERGY_CUTOFF_WRA 			= 0x429,
	ALARM_GND_DETECT_CUTOFF_WRA 		= 0x42A,
	ALARM_TIME_CUTOFF_WRA 				= 0x42B,  
	ALARM_RTC_LOW_BATT_WRA 				= 0x444,
	ALARM_ALARM_LOG_FULL_WRA 			= 0x445,
	ALARM_EVENT_LOG_FULL_WRA 			= 0x446,
	ALARM_WELD_RESULT_FULL_WRA 			= 0x447,
	ALARM_WELD_GRAPH_FULL_WRA 			= 0x448,
	ALARM_HORN_SCAN_FULL_WRA 			= 0x449,
	ALARM_EEPROM_SYSCONFIG_WRA 			= 0x450,
	ALARM_RECIPE_WELD_CORRUPTED_WRA 	= 0x451,
	ALARM_RECIPE_LIMITS_CORRUPTED_WRA 	= 0x452,
	ALARM_RECIPE_SETUP_CORRUPTED_WRA 	= 0x453,
	ALARM_RECIPE_STACK_CORRUPTED_WRA 	= 0x454,
	
	ALARM_PEAKPOWER_LIMIT_MLR_PRA 		= 0x503,
	ALARM_PEAKPOWER_LIMIT_PLR_PRA 		= 0x504,
	ALARM_TIME_LIMIT_MLR_PRA 			= 0x505,
	ALARM_TIME_LIMIT_PLR_PRA 			= 0x506,
	ALARM_ENERGY_LIMIT_MLR_PRA 			= 0x507,
	ALARM_ENERGY_LIMIT_PLR_PRA 			= 0x508,
	ALARM_COLLAPSEDIST_LIMIT_MLR_PRA 	= 0x509,
	ALARM_COLLAPSEDIST_LIMIT_PLR_PRA 	= 0x50A,
	ALARM_ABSOLUTEDIST_LIMIT_MLR_PRA 	= 0x50B,
	ALARM_ABSOLUTEDIST_LIMIT_PLR_PRA 	= 0x50C,
	ALARM_TRIGGERDIST_LIMIT_MLR_PRA 	= 0x50D,
	ALARM_TIRGGERDIST_LIMIT_PLR_PRA 	= 0x50E,
	ALARM_WELDFORCE_LIMIT_MLR_PRA 		= 0x50F,
	ALARM_WELDFORCE_LIMIT_PLR_PRA 		= 0x510,
	ALARM_FREQUENCY_LIMIT_MLR_PRA 		= 0x512,
	ALARM_FREQUENCY_LIMIT_PLR_PRA 		= 0x513,
	ALARM_PREHEIGHT_LIMIT_MLR_PRA		= 0x514,
	ALARM_PREHEIGHT_LIMIT_PLR_PRA		= 0x515,
	ALARM_POSTHEIGHT_LIMIT_MLR_PRA		= 0x516,
	ALARM_POSTHEIGHT_LIMIT_PLR_PRA		= 0x517,

	ALARM_ENERGY_LIMIT_MLS_PSA 			= 0x551,
	ALARM_ENERGY_LIMIT_PLS_PSA 			= 0x552,
	ALARM_PEAKPOWER_LIMIT_MLS_PSA 		= 0x553,
	ALARM_PEAKPOWER_LIMIT_PLS_PSA 		= 0x554,
	ALARM_COLLAPSEDIST_LIMIT_MLS_PSA 	= 0x555,
	ALARM_COLLAPSEDIST_LIMIT_PLS_PSA 	= 0x556,
	ALARM_ABSOLUTEDIST_LIMIT_MLS_PSA 	= 0x557,
	ALARM_ABSOLUTEDIST_LIMIT_PLS_PSA 	= 0x558,
	ALARM_TRIGGERDIST_LIMIT_MLS_PSA 	= 0x559,
	ALARM_TIRGGERDIST_LIMIT_PLS_PSA 	= 0x55A,
	ALARM_WELDFORCE_LIMIT_MLS_PSA 		= 0x55B,
	ALARM_WELDFORCE_LIMIT_PLS_PSA 		= 0x55C,
	ALARM_TIME_LIMIT_MLS_PSA 			= 0x55D,
	ALARM_TIME_LIMIT_PLS_PSA 			= 0x55E,
	ALARM_FREQUENCY_LIMIT_MLS_PSA 		= 0x563,
	ALARM_FREQUENCY_LIMIT_PLS_PSA 		= 0x562,
	
	ALARM_START_SWITCH_CLOSED_EFA 		= 0x601,
	ALARM_GND_DETECT_EFA 				= 0x605,
	ALARM_START_SWITCH_LOST_EFA 		= 0x609,
	ALARM_ALARM_LOG_FULL_EFA 			= 0x611,
	ALARM_EVENT_LOG_FULL_EFA 			= 0x612,
	ALARM_WELD_RESULT_FULL_EFA 			= 0x613,
	ALARM_WELD_GRAPH_FULL_EFA 			= 0x614,
	ALARM_HORN_SCAN_FULL_EFA 			= 0x615,
	ALARM_START_SWITCH_TIME_EFA 		= 0x628,
	ALARM_DISK_MEM_FULL_EFA 			= 0x629,
	ALARM_INTERNAL_STORAGE_FAILURE_EFA 	= 0x62A,
	ALARM_RECALIBRATE_ACTUATOR_EFA 		= 0x62F,
	ALARM_EXT_DISK_NA_EFA 				= 0x631,
	ALARM_PART_PRESENT_ACTIVE_EFA 		= 0x634,
	ALARM_INTERNAL_COMM_USB_EFA 		= 0x635,
	ALARM_INTERNAL_COMM_ENET 			= 0x638,
	ALARM_BUS_SCAN_FAILURE_EFA			= 0x639,
	ALARM_INTERNAL_COMM_CABLE_EFA 		= 0x63A,
	ALARM_SONICS_SOURCE_LOST_EFA 		= 0x63C,
	ALARM_INTERNAL_COMM_EFA 			= 0x63E,
 
	ALARM_INTERNAL_COMM_ECAT_EFA 		= 0x63F,
	
	ALARM_ESTOP_NCA 					= 0x700,
	ALARM_SONICS_DELAY_TIMEOUT_NCA 		= 0x703,
	ALARM_PART_WINDOW_ABORT_NCA 		= 0x706,
	ALARM_PART_CONTACT_PRETRIGGER_NCA 	= 0x708,
	ALARM_LLS_ABORT_BEFORE_TRS 			= 0x709,

	ALARM_CYCLE_ABORT_NCA 				= 0x70A,
	ALARM_ABSOLUTE_DISTANCE_NCA 		= 0x70C,
	ALARM_EXTERNAL_TOOLING_LOST_NCA 	= 0x714,
	ALARM_EXTERNAL_TOOLING_TIMEOUT_NCA 	= 0x715,
	ALARM_PART_PRESENT_LOST_NCA 		= 0x716,
	ALARM_SERVO_MOTOR_ERROR_NCA 		= 0x717,
	ALARM_READY_POSITION_TIMEOUT_NCA 	= 0x718,
	ALARM_RECIPE_NOT_VALID_NCA 			= 0x719,
	ALARM_BATCH_COUNT_NCA 				= 0x71A,
	ALARM_RECIPE_NOT_VALIDATED_NCA 		= 0x71B,
	
	ALARM_ELC_UNKNOWN 					= 0xEF0,
	ALARM_ELC_PB 						= 0xEF1,
	ALARM_ELC_24V 						= 0xEF2,
	ALARM_ELC_ESTOP 					= 0xEF3,
	ALARM_ELC_LE 						= 0xEF4,
	ALARM_ELC_SBEAM 					= 0xEF5,
	ALARM_ELC_TRS 						= 0xEF6,
	ALARM_ELC_DRIVE 					= 0xEF7,
	ALARM_ELC_CM 						= 0xEF8,
	ALARM_ELC_LU 						= 0xEF9,
	ALARM_ELC_SONICS_EN 				= 0xEFA
	
};
#endif
#endif
