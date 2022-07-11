/*
 * ACCalibrate.h
 *
 *  Created on: Jul 11, 2022
 *      Author: JerryW.Wang
 */

#ifndef ACSTATEMACHINE_ACCALIBRATE_H_
#define ACSTATEMACHINE_ACCALIBRATE_H_

#include "ACState.h"

class ACCalibrate: public ACState {
public:
	ACCalibrate();
	virtual ~ACCalibrate();
};

#endif /* ACSTATEMACHINE_ACCALIBRATE_H_ */
