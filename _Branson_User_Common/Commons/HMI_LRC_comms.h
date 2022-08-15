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
	SCBL_USER_DETAILS_READ_REQ,
	SCBL_PASSCODE_VALIDATE_REQ,
	SCBL_ACTIVE_RECIPE_BASIC_INFO_READ_REQ,
	SCBL_WELD_RECIPE_LIST_READ_REQ,
	SCBL_SYSINFO_READ_REQ,
	SCBL_WELDRECIPE_REQ,
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
    MENU_OPTIONS_SCREEN,
    WELD_SETTING_SCREEN,
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
    EVENT_LOG_SCREEN,
    SETUP_SCREEN,
    NEW_RECIPE_SCREEN,
    LANGUAGE_SET_SCREEN,
    SYSTEMINFO_UPGRADE,
    SCREEN_END
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
#endif
