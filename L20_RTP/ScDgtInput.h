/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SCDGTINPUT_H_
#define SCDGTINPUT_H_

#include "SCTask.h"
#include "UserIO.h"
class ScDgtInputTask: public SCTask 
{
public:
	enum MESSAGE_IDENTIFY
	{
		TO_DGT_INPUT_TASK_IO_GET,
	};
	
							ScDgtInputTask		();
							~ScDgtInputTask		();
	
	static ScDgtInputTask*	GetInstance			(void);
	static void				ScDgtInput_Task		(void);
protected:
	unsigned int 			m_UserIO;
	virtual void 			ProcessTaskMessage		(MESSAGE& message) override;
	virtual void			GetDgtInputBits			() = 0;
private:
    
    MSG_Q_ID 			SELF_MSG_Q_ID;
    MSG_Q_ID			UI_MSG_Q_ID;
    
    static ScDgtInputTask* _DgtInputObj;

};

#endif /* SCDGTINPUT_H_ */
