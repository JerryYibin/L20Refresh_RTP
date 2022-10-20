/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------
 
 This file is to define the HEIGHT_PROPERTY of height calibration.

***************************************************************************/
#ifndef HEIGHT_CALIBRATION_H_
#define HEIGHT_CALIBRATION_H_

struct HEIGHT_ENCODER
{
	unsigned int ZeroCount;
	unsigned int TopCount;
	bool Calibrated;
};

struct HEIGHT_PROPERTY
{
	int 			ActualHeight;
	unsigned int 	Pressure;
	unsigned int 	Stroke;
	unsigned int 	HornUpTime;
	unsigned int 	HornDownTime;
	unsigned int 	TotalTime;
};
#endif
