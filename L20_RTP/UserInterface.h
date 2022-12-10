/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "SCTask.h"
#include "HeartBeatUI.h"
#include "FirmwareUpgrdeUI.h"
#include "MainTask.h"

/*
 *
 */
class UserInterface: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to UIC */
		TO_UI_TASK_HEART_BEAT,
		TO_UI_TASK_ACTIVE_RECIPE_INFO,
		TO_UI_SET_ACTIVE_RECIPE,
		TO_UI_RECIPE_LIBRARY_LATEST_PAGE_READ,
		TO_UI_RECIPE_LIBRARY_NEXT_PAGE_READ,
		TO_UI_DEFAULT_RECIPE_GET,
		TO_UI_INSERT_RECIPE_WRITE,
		TO_UI_UPDATE_RECIPE,
		TO_UI_DELETE_RECIPE,
		TO_UI_RECIPE_DETAILS_GET,
		TO_UI_TASK_SETUP_WELD_PARAM,
		TO_UI_TASK_SETUP_QUALITY_PARAM,
		TO_UI_TASK_SETUP_ADVANCED_PARAM,
		TO_UI_ACTIVE_RECIPE_VALIDATE,
		TO_UI_TASK_SYSINFO_READ,
		TO_UI_TASK_SYSCONFIG_READ,
		TO_UI_TASK_SYSCONFIG_WRITE,
		TO_UI_TASK_INITIALIZATION,
	    TO_UI_TASK_HEIGHT_CHECK,
	    TO_UI_TASK_HEIGHT_CALIBRATE_START,
	    TO_UI_TASK_HEIGHT_CORRECT,
	    TO_UI_TASK_HEIGHT_CALIBRATE_ACCEPT,
		TO_UI_TASK_HEIGHT_CALIBRATE_EXIT,
	    TO_UI_TASK_USERIO_INPUT_READ,
		TO_UI_TASK_AUX_MOTION_REQ,
		TO_UI_TASK_SONICS_TEST_REQ,
		TO_UI_TASK_SONICS_100TEST_REQ,
		
		TO_UI_TASK_SETUP_CLICKET_PARAM,
		TO_UI_TASK_ACTIVE_RECIPE_INFO_RESPONSE,
		TO_UI_TASK_SYSCONFIG_READ_RESPONSE,
		TO_UI_TASK_LAST_WELD_RESULT,
		TO_UI_TASK_HEIGHT_CALIBRATE_RESPONSE,
		TO_UI_TASK_HEIGHT_CHECK_RESPONSE,
		TO_UI_TASK_USER_IO_RESPONSE,
		TO_UI_RECIPE_LIBRARY_DATA_RESPONSE,
		TO_UI_TASK_SONICS_TEST_RESPONSE,
		TO_UI_TASK_WELD_RECIPE_QUERY,
		TO_UI_RECIPE_ERRCODE,
		TO_UI_TASK_DETECT_USB_DEVICE,
		TO_UI_TASK_FIRMWARE_UPGRADE_ERROR_MESSADE,
		TO_UI_TASK_FIRMWARE_UPGRADE_PROGRESS_MESSADE,
		TO_UI_TASK_FIRMWARE_UPGRADE_RESPONSE,
		TO_UI_TASK_ETHERNET_CONFIG_SET,
		TO_UI_TASK_ETHERNET_CONFIG_GET,
		TO_UI_TASK_GATEWAY_SERVER_GET,
		TO_UI_TASK_RECIPE_TOTAL_NUMBER_GET,
		TO_UI_TASK_RECIPE_TOTAL_NUMBER_RESPONSE,
		TO_UI_TASK_RESULT_LIBRARY_DATA_RESPONSE,
		TO_UI_TASK_WELD_RESULT_HISTORY_LOG,
		TO_UI_TASK_WELD_RESULT_HISTORY_NEXT_PAGE,
		TO_UI_TASK_WELD_RECIPE_RENAME,
		TO_UI_TASK_READ_POWER_GRAPH_RESPONSE,
		TO_UI_TASK_READ_HEIGHT_GRAPH_RESPONSE,
		TO_UI_TASK_READ_FREQUENCY_GRAPH_RESPONSE,
		TO_UI_TASK_GET_CURRENT_ALARM_IDX,
		TO_UI_TASK_RESET_CURRENT_ALARM_IDX,
		TO_UI_TASK_ALARM_LOG_LATEST_PAGE_IDX,
		TO_UI_TASK_ALARM_LOG_NEXT_PAGE_IDX,
		TO_UI_TASK_ALARM_LOG_LIBRARY_RESPONSE,
		TO_UI_TASK_CLEAR_WELD_COUNTER_IDX,
		TO_UI_TASK_WELD_DATA_EXTERNAL_REQ,
		TO_UI_TASK_POWER_CURVE_EXTERNAL_REQ,
		TO_UI_TASK_HEIGHT_CURVE_EXTERNAL_REQ,
		TO_UI_TASK_FREQUENCY_CURVE_EXTERNAL_REQ
	};
	UserInterface();
	~UserInterface();
	
	static  void UserInterface_Task			(void);
protected:
	virtual void ProcessTaskMessage			(MESSAGE& message) override;	
private:
	//Just list out all the external Message ID, don't include itself.
	MSG_Q_ID 	CTRL_MSG_Q_ID;
	MSG_Q_ID 	DATA_MSG_Q_ID_CTRL;
	MSG_Q_ID 	DATA_MSG_Q_ID_DATA;
	MSG_Q_ID 	DATA_MSG_Q_ID_REQ;
	MSG_Q_ID 	INTERFACE_MSG_Q_ID;
	MSG_Q_ID	ACT_MSG_Q_ID;
	MSG_Q_ID	OUTPUT_MSG_Q_ID;
	MSG_Q_ID	INPUT_MSG_Q_ID;
	MSG_Q_ID	PS_MSG_Q_ID;
	HEARTBEAT 	m_stHeartbeat;
	bool        bIsFWTask = false;
private:
	void			responseHeartbeat					();
	void			getActiveRecipe						();
	void 			responseSystemInfo					();
	void 			getDateAndTime						(char* timebuf);
	void			updateOperationMode					(char* messagebuf);
	void 			updateActiveRecipeGeneralParam		(char* recipebuf);
	void			updateActiveRecipeQualityParam		(char* recipebuf);
	void			updateActiveRecipeAdvancedParam		(char* recipebuf);
	void			updateLastWeldResult				();
	void 			responseWeldRecipeData				();
	void 			responseWeldRecipeDetails			();
	void			responseWeldRecipeLib				();
	void			responseWeldResultLib				();
	void			responseAlarmLogLib					();
	void			sendWeldRecipeErrCode				(char* recipebuf);
	void 			responseInitializationData 			();
	void 			responseSystemConfig				();
	void			updateSystemConfigData				(char* messagebuf);
	void			responseHeightCalibration			();
	void			responseHeightCheck					();
	void			responseUserIORequest				(char* messagebuf);
	void 			responseSonicsTest					(char* messagebuf);
	
	void            responseDetectUSBdevice             ();
	void            firmWareUpgradeHandler              ();
	void 			responseFirmwareUPgradeHandler        ();
	void 			responseFirmwareUpgradeProgress     (char* messagebuf);
	void 			setEthernetConfigData               (char* messagebuf);
	void 			responseEthernetConfigData			();
	void 			responseGatewayServerData			();
	void			responseWeldRecipeTotalNumber		(char*);
	void 			responseReadPowerGraphRequest       ();
	void 			responseReadHeightGraphRequest	    ();
	void 			responseReadFrquencyGraphRequest    ();
	void			responseCurrentAlarmEventRequest	();
	void			responseCurrentAlarmEventReset		();
};

#endif /* USERINTERFACE_H_ */
