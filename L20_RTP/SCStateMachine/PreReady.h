/*
 * PreReady.h
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#ifndef PREREADY_H_
#define PREREADY_H_

#include "SCState.h"
#define TOP_CHECK_TIMEOUT	1000
#define RDCHECK				300/5 
#define TWICE				2
#define MAX_HEIGHT			38005
class PreReady: public SCState {
public:
	PreReady();
	virtual ~PreReady();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* PREREADY_H_ */
