/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#include "GPIO.h"
#include "Logger.h"
extern "C"
{
	#include "subsys/gpio/vxbGpioLib.h"	
}
#if JN_BOARD
int GPIO::m_InputPin[INPUT_SIZE] = {GPIO1_22, GPIO1_24, GPIO1_25, GPIO6_10, GPIO6_11, 
		GPIO6_17, GPIO6_18, GPIO6_19, GPIO6_20, GPIO6_29, GPIO7_22, GPIO7_23};
int GPIO::m_OutputPin[OUTPUT_SIZE] = {GPIO3_28, GPIO3_29, GPIO3_30, GPIO3_31, GPIO4_0, 
		GPIO4_2, GPIO4_3, GPIO4_4, GPIO4_5, GPIO4_6, 
		GPIO4_7, GPIO4_8, GPIO4_13, GPIO4_26, GPIO4_28, GPIO4_30};
#elif LEVEL_ONE
int GPIO::m_InputPin[INPUT_SIZE] = {GPIO1_3, 	GPIO2_2, 	GPIO2_19, 	GPIO2_22, 	GPIO2_24, 
									GPIO2_25, 	GPIO2_26, 	GPIO2_27, 	GPIO2_28, 	GPIO2_29, 
									GPIO3_30, 	GPIO3_31, 	GPIO4_2, 	GPIO5_15, 	GPIO7_22, 
									GPIO7_24, 	GPIO7_25};
int GPIO::m_OutputPin[OUTPUT_SIZE] = {GPIO1_4, 	GPIO1_5, 	GPIO1_16, 	GPIO1_17, 	GPIO4_3, 
									  GPIO4_4, 	GPIO4_5, 	GPIO4_8, 	GPIO4_9, 	GPIO5_0, 
									  GPIO5_1, 	GPIO5_4, 	GPIO5_5, 	GPIO5_6,	GPIO5_7, 
									  GPIO5_8, 	GPIO5_9, 	GPIO5_16, 	GPIO6_7, 	GPIO6_19, 
									  GPIO7_23};
#endif

GPIO::GPIO() {
	// TODO Auto-generated constructor stub

}

GPIO::~GPIO() {
	// TODO Auto-generated destructor stub
}

int GPIO::InitGPIO()
{
	for(int i = 0; i < INPUT_SIZE; i++)
	{
		if(vxbGpioAlloc(m_InputPin[i]) != OK)
		{
			LOGERR((char *)"failed to allocate input pin = %d , try to free it first\n", i, 0, 0);
			return ERROR;
		}
		
		if(vxbGpioSetDir(m_InputPin[i], GPIO_DIR_INPUT) != OK)
		{
			LOGERR((char *)"failed to switch input pin = %d to input direction\n", i, 0, 0);
			return ERROR;
		}
	}
			
	for(int i = 0; i < OUTPUT_SIZE; i++)
	{
		if(vxbGpioAlloc(m_OutputPin[i]) != OK)
		{
			LOGERR((char *)"failed to allocate output pin = %d, try to free it first\n", i, 0, 0);
			return ERROR;
		}
		
		if(vxbGpioSetDir(m_OutputPin[i], GPIO_DIR_OUTPUT) != OK)
		{
			LOGERR((char *)"failed to switch output pin = %d to output direction\n", 1, 0, 0);
			return ERROR;
		}

		if(vxbGpioSetValue(m_OutputPin[i], GPIO_VALUE_LOW) != OK)
		{
			LOGERR((char *)"failed to output pin = %d output value\n", i, 0, 0);
			return ERROR;
		}
	}
	return OK;
}

int GPIO::FreeGPIO()
{
	for(int i = 0; i < INPUT_SIZE; i++)
	{
		if(vxbGpioFree(m_InputPin[i]) != OK)
		{
			LOGERR((char *)"failed to free input pin = %d\n", i, 0, 0);
			return ERROR;
		}
	}
	
	for(int i = 0; i < OUTPUT_SIZE; i++)
	{
		if(vxbGpioFree(m_OutputPin[i]) != OK)
		{
			LOGERR((char *)"failed to free output pin = %d\n", i, 0, 0);
			return ERROR;
		}
	}
	return OK;
}
