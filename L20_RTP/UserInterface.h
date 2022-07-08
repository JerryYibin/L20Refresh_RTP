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
		TO_UI_TASK_LAST_WELD_RESULT,
	};
	UserInterface();
	~UserInterface();
	
	static  void UserInterface_Task			(void);
protected:
	virtual void ProcessTaskMessage			(MESSAGE& message);	
private:
	//Just list out all the external Message ID, don't include itself.
	MSG_Q_ID CTRL_MSG_Q_ID;
	MSG_Q_ID DATA_MSG_Q_ID_CTRL;
	MSG_Q_ID DATA_MSG_Q_ID_DATA;
	MSG_Q_ID DATA_MSG_Q_ID_REQ;
	MSG_Q_ID INTERFACE_MSG_Q_ID;
private:
	void			responseHeartbeat					();
	void			getActiveRecipe						();
	void			updateOperationMode					(char* messagebuf);
	void 			updateActiveRecipeGeneralParam		(char* recipebuf);
	void			updateActiveRecipeQualityParam		(char* recipebuf);
	void			updateActiveRecipeAdvancedParam		(char* recipebuf);
};

#endif /* USERINTERFACE_H_ */
