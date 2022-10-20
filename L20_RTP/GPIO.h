/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_
#include "commons.h"
#if INCLUDE_TI_AM5708_JN
// GPIO pin value definitions
#define GPIO1_22 0x16
#define GPIO1_24 0x18
#define GPIO1_25 0x19

#define GPIO3_28 0x5c
#define GPIO3_29 0x5d
#define GPIO3_30 0x5e
#define GPIO3_31 0x5f

#define GPIO4_0 0x60

#define GPIO4_2 0x62
#define GPIO4_3 0x63
#define GPIO4_4 0x64
#define GPIO4_5 0x65
#define GPIO4_6 0x66
#define GPIO4_7 0x67                             
#define GPIO4_8 0x68
                           
#define GPIO4_13 0x6d
#define GPIO4_26 0x7a
#define GPIO4_28 0x7c
#define GPIO4_29 0x7d
#define GPIO4_30 0x7e

#define GPIO6_10 0xaa
#define GPIO6_11 0xab
#define GPIO6_17 0xb1
#define GPIO6_18 0xb2
#define GPIO6_19 0xb3
#define GPIO6_20 0xb4

#define GPIO6_29 0xbd

#define GPIO7_22 0xd6
#define GPIO7_23 0xd7

#define INPUT_SIZE 12
#define OUTPUT_SIZE 16
#elif INCLUDE_TI_AM5708_BRANSON
#define GPIO1_3		0x03
#define GPIO1_4 	0x04
#define GPIO1_5 	0x05
#define GPIO1_16 	0x10
#define GPIO1_17 	0x11

#define GPIO2_2		0x22
#define GPIO2_19	0x33

#define GPIO2_22	0x36
#define GPIO2_24	0x38
#define GPIO2_25	0x39
#define GPIO2_26	0x3A
#define GPIO2_27	0x3B
#define GPIO2_28	0x3C
#define GPIO2_29	0x3D

#define GPIO3_30	0x5E
#define GPIO3_31	0x5F

#define GPIO4_2		0x62
#define GPIO4_3		0x63
#define GPIO4_4		0x64
#define GPIO4_5		0x65
#define GPIO4_8		0x68
#define GPIO4_9		0x69

#define GPIO5_0		0x80
#define GPIO5_1		0x81
#define GPIO5_4		0x84
#define GPIO5_5		0x85
#define GPIO5_6		0x86
#define GPIO5_7		0x87
#define GPIO5_8		0x88
#define GPIO5_9		0x89
#define GPIO5_15	0x8F
#define GPIO5_16	0x90

#define GPIO6_7		0xA7
#define GPIO6_19	0xB3

#define GPIO7_22	0xD6
#define GPIO7_23	0xD7
#define GPIO7_24	0xD8
#define GPIO7_25	0xD9

#define INPUT_SIZE 17
#define OUTPUT_SIZE 21
#endif
class GPIO {
private:
	static int m_InputPin[INPUT_SIZE];
	static int m_OutputPin[OUTPUT_SIZE];
public:
	enum OUTPUT_PIN
	{
#if INCLUDE_TI_AM5708_JN
		O_READY 		= GPIO3_28,
		O_GATHER 		= GPIO3_29,
		O_ANVIL			= GPIO3_30,
		O_CLAMP			= GPIO3_30,
		O_SPARE 		= GPIO3_31,
		O_SONICS_ON 	= GPIO3_31,
		
		O_HORN 			= GPIO4_0,
		O_OUT0 			= GPIO4_2,
		O_SAFETY 		= GPIO4_3,
		O_RUN_PSI 		= GPIO4_4,
		O_COOLAIR 		= GPIO4_5,
		O_EXT_SEEK_PSI 	= GPIO4_6,
		O_SEEK 			= GPIO4_7,
		O_BUZZ 			= GPIO4_8,
		O_OL_RST_PSI	= GPIO4_13,
		O_OUT1 			= GPIO4_26,
		O_TEST_PSI 		= GPIO4_28,
		O_ALARM 		= GPIO4_30,
#elif INCLUDE_TI_AM5708_BRANSON
		O_RUN_PSI		= GPIO1_4,	// Sonics on digital IO for AUPS only
		O_OL_RST_PSI	= GPIO1_5,	// Sonics Overload indicator from AUPS only
		O_TEST_PSI		= GPIO1_16,	// Sonics test digital IO for AUPS only 
		O_EXT_SEEK_PSI	= GPIO1_17, // Sonics seek digital IO for AUPS only
		
		O_SPARE			= GPIO4_3,  // Sonics indicator for external
		O_SONICS_ON		= GPIO4_3,	
		O_ALARM			= GPIO4_4,	// Alarm indicator for external
		O_COOLAIR		= GPIO4_5,	// Cooling digital IO
		O_GATHER		= GPIO4_8,	// Gather digital IO
		O_ANVIL			= GPIO4_9,  // Anvil digital IO
		O_CLAMP			= GPIO4_9,	// Clamp digital IO
		
		O_HORN			= GPIO5_0,	// Horn down/Up digital IO
		O_READY			= GPIO5_1,	// Ready signal indicator for external
		O_SEEK			= GPIO5_4,	// Sonics seek indicator for external
		O_SAFETY		= GPIO5_5,	// Safety digital IO
		O_OUT0			= GPIO5_6,	// Auxiliary indicator for external
		O_OUT1			= GPIO5_7,  // Auxiliary indicator for external
		O_NULL_IO_01	= GPIO5_8,	// IO validation indicator
		O_NULL_IO_02	= GPIO5_9,  // IO validation indicator
		O_BUZZ			= GPIO5_16,	// Buzzer digital for external

		O_MOTOR_IN1		= GPIO6_7,  // motor CW
		O_MOTOR_IN2		= GPIO6_19, // motor CCW
		
		O_SHUT_DOWN		= GPIO7_23  // Shut down control IO
#endif
		
	};
	enum INPUT_PIN
	{
#if INCLUDE_TI_AM5708_JN
		I_IN3 		= GPIO1_22,
		I_IN0 		= GPIO1_24,
		I_OPSIG 	= GPIO1_25,

		I_IN2 		= GPIO6_10,
		I_AIRMON 	= GPIO6_11,
		I_PB2 		= GPIO6_17,
		I_PB1 		= GPIO6_18,
		I_OL_PSOUI 	= GPIO6_19,
		I_IN4 		= GPIO6_20,
		I_COVCLSE 	= GPIO6_29,
		
		I_ESTOPNC 	= GPIO7_22,
		I_ESTOPNCR 	= GPIO7_23
#elif INCLUDE_TI_AM5708_BRANSON
		I_POWER_INT		= GPIO1_3,   // hardware interrupt from power management chip
		
		I_OL_PSOUI 		= GPIO2_2,   // Overload signal monitor from AUPS
		I_OPSIG 		= GPIO2_19,  // Remote reset signal 
		I_IN0 			= GPIO2_22,  // Remote IN0 signal
		I_COVCLSE 		= GPIO2_24,  // Cover close switch signal
		I_AIRMON 		= GPIO2_25,  // Alarm monitor sensor signal.	
		I_ESTOPNC 		= GPIO2_26,  // E-Stop signal 
		I_ESTOPNCR 		= GPIO2_27,  // E-Stop return signal
		I_PB1 			= GPIO2_28,  // Palm button 1
		I_PB2 			= GPIO2_29,  // Palm button 2
		
		I_IN2 			= GPIO3_30,	 // Remote IN2 signal
		I_IN3 			= GPIO3_31,  // Remote IN3 signal

		I_IN4 			= GPIO4_2,   // Remote IN4 signal

		I_SETUP_LOCK 	= GPIO5_15,  // Lock switch signal
		
		I_SWITCH_HOLD 	= GPIO7_22,  // Power off request signal 
		I_24V_LOW		= GPIO7_24,  // 24V lost signal
		I_USB3_FLT		= GPIO7_25   // USB fault signal
#endif
	};
	GPIO();
	virtual ~GPIO();
	static int InitGPIO();
	static int FreeGPIO();
};

#endif /* GPIO_H_ */
