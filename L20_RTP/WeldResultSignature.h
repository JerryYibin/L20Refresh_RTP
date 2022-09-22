/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef WELDRESULTSIGNATURE_H_
#define WELDRESULTSIGNATURE_H_

#define HMI_SIGNA_POINT_MAX 200

enum SIGNATURE_DATA_TYPE
{
	FRQUENCY = 0,
	POWER,
	HEIGHT,
	AMPLITUDE,
	TOTAL
};

struct WELD_SIGNATURE
{
	unsigned int Frquency;
	unsigned int Power;
	unsigned int Height;
	unsigned int Amplitude;
};

struct HMI_WELD_SIGNATURE_POINTS
{
	unsigned int Frquency[HMI_SIGNA_POINT_MAX];
	unsigned int Power[HMI_SIGNA_POINT_MAX];
	unsigned int Height[HMI_SIGNA_POINT_MAX];
	unsigned int Time[HMI_SIGNA_POINT_MAX];
	unsigned int length; //<=200
};

#endif /* WELDRESULTSIGNATURE_H_ */
