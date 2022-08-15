/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef HEIGHTENCODER_H_
#define HEIGHTENCODER_H_
#define RESOLUTION 5 //5um
#define INIT_COUNT 30000
#define HEIGHT_UNIT 1000 //convert mm to um
class HeightEncoder 
{
public:
	HeightEncoder();
	virtual ~HeightEncoder();
public:
	static int SetInitCount(const unsigned int count);
	static int GetInitCount();
	static int SetMaxCount(const unsigned int count);
	static int GetMaxCount();
	static int GetPositionCount();
	static int GetDirection();
	static int GetActualHeight();
};

#endif /* HEIGHTENCODER_H_ */
