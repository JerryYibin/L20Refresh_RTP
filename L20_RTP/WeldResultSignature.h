/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#ifndef WELDRESULTSIGNATURE_H_
#define WELDRESULTSIGNATURE_H_

struct WELD_SIGNATURE
{
	unsigned int Frquency;
	unsigned int Power;
	unsigned int Height;
	unsigned int Amplitude;
};

struct WELD_SIGNATURE_GROUP
{
	unsigned int Frquency[200];
	unsigned int Power[200];
	unsigned int Height[200];
	unsigned int length; //<=200
};

#endif /* WELDRESULTSIGNATURE_H_ */
