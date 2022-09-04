/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "SCTask.h"

/*
 *
 */
class UserInterface: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		/* Macro defined to UIC */
		TO_UI_TASK_HEART_BEAT 				= SCBL_HEART_BEAT_REQ,
		TO_UI_TASK_ACTIVE_RECIPE_INFO 		= SCBL_ACTIVE_RECIPE_BASIC_INFO_READ_REQ,
		TO_UI_TASK_SETUP_WELD_PARAM			= SCBL_SETUP_WELD_PARAM_WRITE_REQ,
		TO_UI_TASK_SETUP_QUALITY_PARAM		= SCBL_SETUP_QUALITY_PARAM_WRITE_REQ,
		TO_UI_TASK_SETUP_ADVANCED_PARAM 	= SCBL_SETUP_ADVANCED_PARAM_WRITE_REQ,
		TO_UI_TASK_SYSINFO_READ				= SCBL_SYSINFO_READ_REQ,
		TO_UI_TASK_SYSCONFIG_READ			= SCBL_SYSCONFIG_READ_REQ,
		TO_UI_TASK_SYSCONFIG_WRITE			= SCBL_SYSCONFIG_WRITE_REQ,
		TO_UI_TASK_INITIALIZATION			= SCBL_INITIALIZATION_REQ,
	    TO_UI_TASK_HEIGHT_CALIBRATE_START	= SCBL_HEIGHT_CALIBRATE_START_REQ,
	    TO_UI_TASK_HEIGHT_CHECK				= SCBL_HEIGHT_CALIBRATE_CHECK_REQ,
	    TO_UI_TASK_HEIGHT_CORRECT			= SCBL_HEIGHT_CALIBRATE_CORRECT_REQ,
	    TO_UI_TASK_HEIGHT_CALIBRATE_ACCEPT	= SCBL_HEIGHT_CALIBRATE_ACCEPT_REQ,
		
		TO_UI_TASK_LAST_WELD_RESULT,
		TO_UI_TASK_HEIGHT_CALIBRATE_RESPONSE,
		TO_UI_TASK_HEIGHT_CHECK_RESPONSE,
	};
	UserInterface();
	~UserInterface();
	
	static  void UserInterface_Task			(void);
protected:
	virtual void ProcessTaskMessage			(MESSAGE& message);	
private:
	//Just list out all the external Message ID, don't include itself.
	MSG_Q_ID 	CTRL_MSG_Q_ID;
	MSG_Q_ID 	DATA_MSG_Q_ID_CTRL;
	MSG_Q_ID 	DATA_MSG_Q_ID_DATA;
	MSG_Q_ID 	DATA_MSG_Q_ID_REQ;
	MSG_Q_ID 	INTERFACE_MSG_Q_ID;
	HEARTBEAT 	m_stHeartbeat;
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
	void 			responseInitializationData 			();
	void 			responseSystemConfig				();
	void			updateSystemConfigData				(char* messagebuf);
	void			responseHeightCalibration			();
	void			responseHeightCheck					();
};

#endif /* USERINTERFACE_H_ */
