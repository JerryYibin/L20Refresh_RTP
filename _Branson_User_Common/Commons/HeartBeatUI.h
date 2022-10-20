/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

This file is to define the struct HEARTBEAT
 
***************************************************************************/
#ifndef HEARTBEATUI_H_
#define HEARTBEATUI_H_


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
