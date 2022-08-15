/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#ifndef SCACTION_H_
#define SCACTION_H_

class SCAction {
public:
	SCAction();
	virtual ~SCAction();
	virtual int Execute() = 0;
};

#endif /* ACTION_H_ */
