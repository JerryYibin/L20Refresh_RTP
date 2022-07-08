/*
 * Ready.cpp
 *
 *  Created on: May 5, 2022
 *      Author: JerryW.Wang
 */

#include "Ready.h"

Ready::Ready() {
	m_Actions = SCState::INIT;
	m_State = SCState::READY;
	m_Timeout = 0;
}

Ready::~Ready() {
	m_Actions = SCState::INIT;
	m_State = NO_STATE;
}

void Ready::Init()
{
	vxbGpioSetValue(GPIO::O_READY, GPIO_VALUE_HIGH);
	m_Actions = SCState::LOOP;
}

void Ready::Loop()
{
	m_Actions = SCState::JUMP;
}

void Ready::Fail()
{
	m_Actions = SCState::ABORT;
	LOG("Ready Alarm happened!\n");
}
