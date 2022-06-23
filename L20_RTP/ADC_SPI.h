/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The ADC_AD7689 is the spi adc chip for the analog signal reading.   
 
***************************************************************************/

#ifndef ADC_SPI_H_
#define ADC_SPI_H_
extern "C"
{
	#include <vxbMcSpiLib.h>
}
#define COUNTER	10
class ADC_AD7689 
{
public:
	enum CFG
	{
		KeepCurrentConfig  = 0,
		Overwrite = 1,
	};
	
	enum CHANNEL_CONFIG
	{
		BiDiffPair = 0, 
		Bipolar = 2, 
		Temperature = 3,
		UniDiffPair = 4, 
		UniRef2COM = 6, 
		UniRef2GND = 7
	};
	
	enum CHANNEL
	{
		CH_0 = 0, 
		CH_1, 
		CH_2, 
		CH_3, 
		CH_4, 
		CH_5, 
		CH_6, 
		CH_7,
		CH_MAX
	};
	
	enum ANALOG_CHANNEL
	{
		FREQ_OUT		= CH_0,
		AMPLITUDE_OUT	= CH_1,
		POWER_OUT		= CH_2,
		MEMORY			= CH_3,
		VOL10REF		= CH_4,
		RESERVED1		= CH_5,
		RESERVED2		= CH_6,
		RESERVED3		= CH_7
	};
	
	enum BAND_WIDTH
	{
		Quarter = 0, 
		Full = 1
	};
	
	enum REFERENCE
	{
		IntRef2_5V = 0, 
		IntRef4_096V = 1, 
		ExtRef_Temp = 2,
		ExtRef_IntBuf_Temp = 3, 
		ExtRef = 6, 
		ExtRef_IntBuf = 7
	};

	enum SEQUENCER
	{
		DisableSeq = 0, 
		UpdateConf = 1, 
		ScanChannel_Temp = 2, 
		ScanChannel = 3
	};
	
	enum READ_BACK
	{
		ReadBack = 0, 
		NoRead = 1
	};
	
	union ADC
	{
		unsigned short ADC_Cfg;
		struct ADC_REGISTER
		{
			unsigned short Reserved : 2;
			unsigned short RB 		: 1;
			unsigned short SEQ 		: 2;
			unsigned short REF 		: 3;
			unsigned short BW 		: 1;
			unsigned short INx 		: 3;
			unsigned short INCC 	: 3;
			unsigned short CFG 		: 1;
		}ADC_Reg;
	};
private:
	static SPI_HARDWARE 	m_Descriptor;
	static SPI_TRANSFER 	m_Transfer;
	static ADC			 	m_Register;
	static unsigned int		m_ADPowerAvg	[COUNTER];
	static unsigned int		m_ADFreqAvg		[COUNTER];
	static unsigned int		m_ADAmpAvg		[COUNTER];
	static unsigned int		m_ADMemoryAvg	[COUNTER];
	static unsigned int		m_ADRefAvg		[COUNTER];
	static unsigned int		m_Counter;
private:
	static unsigned int		getAverageFilter(const unsigned int* pointer);
public:
	ADC_AD7689();
	virtual ~ADC_AD7689();
	static int InitADConverter						();
	static unsigned short	GetAmplitude			();
	static unsigned short	GetPower				();
	static unsigned short	GetFrequency			();
	static unsigned short	GetMemory				();
	static unsigned short	GetReference			();
	static int 				RunADCSample			();
};

#endif /* ADC_SPI_H_ */
