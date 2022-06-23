/*
 * Ready.h
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#ifndef READY_H_
#define READY_H_

#include "SCState.h"

class Ready: public SCState {
public:
	Ready();
	virtual ~Ready();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* READY_H_ */
