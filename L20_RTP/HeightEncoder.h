/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef HEIGHTENCODER_H_
#define HEIGHTENCODER_H_
#include <map>
#include "HeightCalibrationUI.h"
#define RESOLUTION 5 //5um
#define HEIGHT_BASIC_COUNT		30000
#define HEIGHT_HOME_COUNT		38000
#define HEIGHT_MAXIMUM_COUNT	0xffff
#define PRESSURE_NUM 				81
#define PRESSURE_FACTOR 			1000
using namespace std;
class HeightEncoder 
{
private:
	static HeightEncoder	*_HeightEncoder;
private:
	HeightEncoder();
	HeightEncoder(const HeightEncoder & srcObj) = delete;
	HeightEncoder & operator=	(const HeightEncoder& srcObj) = delete;
public:
	static map<int, HEIGHT_ENCODER>		HeightCalibratedMap;
	static HEIGHT_PROPERTY				HeightProperty;
	static HEIGHT_ENCODER				RawHeight;
public:
	
	virtual ~HeightEncoder();
public:
	static HeightEncoder* 	GetInstance			();
	int 					SetInitCount		(const unsigned int count);
	int 					GetInitCount		();
	int 					SetMaxCount			(const unsigned int count);
	int 					GetMaxCount			();
	int 					GetPositionCount	();
	int 					GetDirection		();
	int 					GetActualHeight		(unsigned int pressure);
};

#endif /* HEIGHTENCODER_H_ */
