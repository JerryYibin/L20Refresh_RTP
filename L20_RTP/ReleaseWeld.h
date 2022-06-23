/*
 * ReleaseWeld.h
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#ifndef RELEASEWELD_H_
#define RELEASEWELD_H_

#include "SCState.h"

class ReleaseWeld: public SCState {
public:
	ReleaseWeld();
	virtual ~ReleaseWeld();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
};

#endif /* RELEASEWELD_H_ */
