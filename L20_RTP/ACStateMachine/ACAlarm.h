/*
 * ACAlarm.h
 *
 *  Created on: Jul 11, 2022
 *      Author: JerryW.Wang
 */

#ifndef ACSTATEMACHINE_ACALARM_H_
#define ACSTATEMACHINE_ACALARM_H_

#include "ACState.h"

class ACAlarm: public ACState {
public:
	ACAlarm();
	virtual ~ACAlarm();
};

#endif /* ACSTATEMACHINE_ACALARM_H_ */
