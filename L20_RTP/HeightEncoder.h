/*
 * HeightEncoder.h
 *
 *  Created on: May 27, 2022
 *      Author: JerryW.Wang
 */

#ifndef HEIGHTENCODER_H_
#define HEIGHTENCODER_H_

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
};

#endif /* HEIGHTENCODER_H_ */
