/*
 * WeldSonicOn.h
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#ifndef WELDSONICON_H_
#define WELDSONICON_H_
#include "SCState.h"

#define DELAY500MSEC	500
#define PWR_SIZE		5
#define GRAPHDATASIZE	5120
#define ABSMAXTIME		5000
#define JOULE500		500
#define DELAY5SEC		5000
using namespace std;
class WeldSonicOn: public SCState {
	public:
	enum WELD_STEP
	{
		SET_WPRESSURE,
		RUN_SONICS,
		RESULT_UPDATE,
		EXTEND_SAMPLE,
		RESULT_ANALYSIS,

	};
private:
	unsigned int m_WeldTime;
	unsigned int m_PeakPower;
	unsigned long m_EnergyAccumulator;
	unsigned long m_EnergyTarget;
	WELD_STEP m_WeldStep;
public:
	WeldSonicOn();
	virtual ~WeldSonicOn();
public:
	virtual void Init() override;
	virtual void Loop() override;
	virtual void Fail() override;
private:
	void CaptureWeldData();
	void ClearWeldData();
	unsigned int GetFilteredPower(bool clearPwr);
	void CoolAirControl(unsigned int delay, unsigned duration);
};

#endif /* WELDSONICON_H_ */
