/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------   
 
***************************************************************************/

#include "SCState.h"
SCState::SCState() {
	// TODO Auto-generated constructor stub
	_objDCan = CommunicationInterface_CAN::GetInstance();

}

SCState::~SCState() {
	// TODO Auto-generated destructor stub
}

void SCState::abortWeld(void)
{
	///* Clear outputs and panel data */
	vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
	///* Clear SONICSRUN, SONICSRESET and SONICSTEST */
	vxbGpioSetValue(GPIO::O_RUN_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_OL_RST_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_TEST_PSI, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SONICS_ON, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_ALARM, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_COOLAIR, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_GATHER, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_CLAMP, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_HORN, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SEEK, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_SAFETY, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_OUT0, GPIO_VALUE_LOW);
	vxbGpioSetValue(GPIO::O_OUT1, GPIO_VALUE_LOW);
	
	//SetPressure(ActivePreset.m_WeldParam.TPressure);
}

bool SCState::DefeatWeldAbortHandler()
{
	bool isAbort = false;
//	if (SysConfig.m_SystemInfo.FootPedalAbort == true)
//	{
		if(vxbGpioGetValue(GPIO::I_PB1) == GPIO_VALUE_HIGH || vxbGpioGetValue(GPIO::I_PB2) == GPIO_VALUE_HIGH)
		{
			abortWeld();
			CommonProperty::WeldResult.ALARMS.AlarmFlags.WeldAborted = 1;
			CommonProperty::WeldResult.ALARMS.AlarmFlags.FootPedalAbort = 1;
			isAbort = true;
		}
//	}
	return isAbort;
}

bool SCState::ProcessAlarmHandler(void)
{
	bool isReset = false;
	if(CommonProperty::WeldResult.ALARMS.AlarmFlags.WeldAborted != 0)
	{
		if(m_Timeout < ALARMBEEPDELAY)
		{
			m_Timeout++;
		}
		else
		{
			if(vxbGpioGetValue(GPIO::O_BUZZ) == GPIO_VALUE_HIGH)
				vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_LOW);
			else
				vxbGpioSetValue(GPIO::O_BUZZ, GPIO_VALUE_HIGH);
			m_Timeout = 0;
		}
	}
	else
		isReset = true;
	return isReset;
}

/*bool SCState::SendToMsgQ(MESSAGE & msgBuffer, const int & msgQID)
{
	return false;
}*/
