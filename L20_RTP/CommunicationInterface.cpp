/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "CommunicationInterface.h"
#include "EmrsnSocket.h"
#include "EmrsnDCan.h"

/**************************************************************************//**
* 
* \brief   - Constructor Instantiate the specific implementor object 
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterface::CommunicationInterface(int type) 
{
	m_objImplementor = nullptr;
	m_typeCommunication = (COMMUNICATION_TYPE)type;
	switch(m_typeCommunication)
	{
	case CommunicationInterface::HMI_SOCKET_CONNECTION:
		m_objImplementor = new EmrsnSocket();
		break;
	case CommunicationInterface::ACT_SOCKET_CONNECTION:
		break;
	case CommunicationInterface::EXT_SOCKET_CONNECTION:
		m_objImplementor = new EmrsnSocket();
		break;
	case CommunicationInterface::DIG_SOCKET_CONNECTION:
		m_objImplementor = new EmrsnSocket();
		break;
	case CommunicationInterface::SENSOR_CAN_CONNECTION:
		m_objImplementor = new EmrsnDCan();
		break;
	default:
		m_objImplementor = nullptr;
		break;
	}
}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationInterface::~CommunicationInterface() 
{
	if (m_objImplementor != nullptr)
	{
		delete m_objImplementor;
		m_objImplementor = nullptr;
	}
}

/**************************************************************************//**
* 
* \brief   - Get the specific implementor object pointer 
*
* \param   - None.
*
* \return  - The specific implementor object pointer.
******************************************************************************/
CommunicationImplementer* CommunicationInterface::GetCommunicationImp()
{
	if(m_objImplementor != nullptr)
		return m_objImplementor;
	else
		return nullptr;
}
