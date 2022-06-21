/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 ---------------------------- MODULE DESCRIPTION ----------------------------
 
 The CommunicationImp is the encapsulated interface named implementer interface 
 for all the communication code that from the third part to try to separate 
 the relationship between third part code and Branson standard communication interface. 
 Please refer to the code in constructor function of CommunicationInterface. 
 There is the polymorphism implemented in the CommunicationInterface and 
 keep the specific object in the CommunicationInterface using CommunicationImp* m_objImplementor.     
 
***************************************************************************/

#include "CommunicationImplementer.h"

/**************************************************************************//**
* 
* \brief   - Constructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationImplementer::CommunicationImplementer() {
	// TODO Auto-generated constructor stub

}

/**************************************************************************//**
* 
* \brief   - Destructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
CommunicationImplementer::~CommunicationImplementer() {
	// TODO Auto-generated destructor stub
}

