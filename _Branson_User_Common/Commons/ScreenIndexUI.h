/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
     
---------------------------- MODULE DESCRIPTION ----------------------------
 
 All the definition that need to be used both on ASC and HMI software should be in here.  
 
***************************************************************************/

#ifndef SHARED_COMMON_H_
#define SHARED_COMMON_H_

enum SCREENINDEX
{
    DASHBORD_SCREEN = 0,
    MENU_OPTIONS_SCREEN,
    RECIPE_MENU_SCREEN,
    RECIPE_LIBRARY_SCREEN,
    NEW_RECIPE_SCREEN,
    EDIT_RECIPE_SCREEN,
    SEQUENCE_LIBRARY_SCREEN,
    NEW_SEQUENCE_SCREEN,
    EDIT_SEQUENCE_SCREEN,
    MAINTAIN_SCREEN,
    MAINTAIN_ACTUATOR_SCREEN,
    MAINTAIN_SONICS_SCREEN,
    MAINTAIN_COUNTERS_SCREEN,
    SYSTEM_CONFIG_SCREEN,
    CONFIG_WELD_DEFAULT_SCREEN,
    CONFIG_LANGUAGE_SCREEN,
    CONFIG_UNIT_SCREEN,
    CONFIG_TEACH_MODE_SCREEN,
    CONFIG_INIT_SCREEN,
    CONFIG_POWER_SUPPLY_SCREEN,
    CONFIG_SYSTEM_TIME_SCREEN,
    ADMINISTRATOR_SCREEN,
    PASSCODE_LIST_SCREEN,
    PERMISSON_SETTING_SCREEN,
    DATA_LOG_SCREEN,
    WELD_RESULT_HISTORY_SCREEN,
    WELD_RESULT_GRAPH_SCREEN,
    ALARM_LOG_SCREEN,
    CONNECTIVITY_SCREEN,
    ETHERNET_SCREEN,
    GATEWAY_DATA_SCREEN,
    GATEWAY_MACHIEN_IP_SCREEN,
    USER_IO_SCREEN,
    DATA_EXPORT_SCREEN,
    CALIBRATION_SCREEN,
    HEIGHT_CALIBRATION_SCREEN,
    AMPLITUDE_CALIBRATINON_SCREEN,
    SYSTEM_INFORMATION_SCREEN,
    SYSTEMINFO_UPGRADE,
    SETUP_TEACH_SCREEN,
    SETUP_SCREEN,
    TEACH_MODE_SCREEN,
    KEYBOARD_SCREEN,
    PROMPT_MESSAGE_SCREEN,
    LIBRARY_GET,
    SETUP_SETUPSCREEN,
    SETUP_TEACH,
    LANGUAGE_SCREEN,
    SCREEN_END
};
#endif
