/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The DAC_TLV5604 is the spi dac chip for the analog signal setting.   
 
***************************************************************************/

#ifndef DAC_SPI_H_
#define DAC_SPI_H_
extern "C"
{
	#include <vxbMcSpiLib.h>
}
class DAC_TLV5604 
{
private:
	enum SPEED_CONTROL
	{
		SlowMode = 0,
		FastMode = 1
	};
	enum POWER_CONTROL
	{
		NormalOperation = 0,
		PowerDown		= 1
	};
	enum CHANNEL
	{
		CH_0 = 0,
		CH_1 = 1,
		CH_2 = 2,
		CH_3 = 3
	};
	enum ANALOG_CHANNEL
	{
		FREQ_OFFSET		= CH_0,
		AMPLITUDE_IN	= CH_1,
		DIGPOT_WIPER	= CH_2,
		PRESSURE_SET    = CH_3
	};
	union DAC
	{
		unsigned short DAC_Cfg;
		struct DAC_REGISTER
		{
			unsigned short Reserved	: 2;
			unsigned short Value	: 10;
			unsigned short Speed	: 1;
			unsigned short Power	: 1;
			unsigned short Channel	: 2;
		}DAC_Reg;
	};
private:
	static SPI_HARDWARE 	m_Descriptor;
	static SPI_TRANSFER 	m_Transfer;
	static DAC			 	m_Register;
private:
	static int setDACValue(const int channel, const unsigned short data);
public:
	DAC_TLV5604();
	virtual ~DAC_TLV5604();
	static int InitDAConverter		();
	static int SetAmplitude			(const unsigned short data);
	static int SetTunePoint			(const unsigned short data);
	static int SetFrequencyOffset	(const unsigned short data);
	static int SetPressure			(const unsigned short data);
	
};

#endif /* DAC_SPI_H_ */
