/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
    
 
***************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_
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

class GPIO {
private:
	static int m_InputPin[INPUT_SIZE];
	static int m_OutputPin[OUTPUT_SIZE];
public:
	enum OUTPUT_PIN
	{
		O_SPARE 		= GPIO3_31,
		O_SONICS_ON 	= GPIO3_31,
		O_ALARM 		= GPIO4_30,
		O_COOLAIR 		= GPIO4_5,
		O_GATHER 		= GPIO3_29,
		O_ANVIL			= GPIO3_30,
		O_CLAMP			= GPIO3_30,
		O_HORN 			= GPIO4_0,
		O_READY 		= GPIO3_28,
		O_SEEK 			= GPIO4_7,
		O_SAFETY 		= GPIO4_3,
		O_OUT0 			= GPIO4_2,
		O_OUT1 			= GPIO4_26,
		O_BUZZ 			= GPIO4_8,
		O_RUN_PSI 		= GPIO4_4,
		O_OL_RST_PSI	= GPIO4_13,
		O_TEST_PSI 		= GPIO4_28,
		O_EXT_SEEK_PSI 	= GPIO4_6,
	};
	enum INPUT_PIN
	{
		I_OPSIG 	= GPIO1_25,
		I_IN0 		= GPIO1_24,
		I_COVCLSE 	= GPIO6_29,
		I_AIRMON 	= GPIO6_11,
		I_IN2 		= GPIO6_10,
		I_IN3 		= GPIO1_22,
		I_PB1 		= GPIO6_18,
		I_PB2 		= GPIO6_17,
		I_IN4 		= GPIO6_20,
		I_OL_PSOUI 	= GPIO6_19,
		I_ESTOPNC 	= GPIO7_22,
		I_ESTOPNCR 	= GPIO7_23
	};
	GPIO();
	virtual ~GPIO();
	static int InitGPIO();
	static int FreeGPIO();
};

#endif /* GPIO_H_ */
