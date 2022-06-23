/*
 * Seek.h
 *
 *  Created on: May 26, 2022
 *      Author: JerryW.Wang
 */

#ifndef SEEK_H_
#define SEEK_H_

#include "SCState.h"
#define DELAY25MSEC 25
class Seek: public SCState
{
public:
	Seek();
	~Seek();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* SEEK_H_ */
