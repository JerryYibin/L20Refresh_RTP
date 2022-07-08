/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "HeightEncoder.h"
extern "C"
{
	#include <vxbEqepLib.h>
}
/**************************************************************************//**
* 
* \brief   - Constructor
*
* \param   - None.
*
* \return  - None.
******************************************************************************/
HeightEncoder::HeightEncoder() {
	// TODO Auto-generated constructor stub

}

/**************************************************************************//**
* 
* \brief   - Destructor.
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
HeightEncoder::~HeightEncoder() {
	// TODO Auto-generated destructor stub
}

/**************************************************************************//**
* 
* \brief   - Set Initial eqep current counter number.
*
* \param   - count.
*
* \return  - If the setting is successful, it returns OK, otherwise it will return ERROR; .
*
******************************************************************************/
int HeightEncoder::SetInitCount(const unsigned int count)
{
	return vxbEqepSetInitCount(1, count);
}

/**************************************************************************//**
* 
* \brief   - Get Initial eqep current counter number.
*
* \param   - None.
*
* \return  - If the getting is successful, it returns initial count number, 
* 			otherwise it will return ERROR; .
*
******************************************************************************/
int HeightEncoder::GetInitCount()
{
	return vxbEqepGetInitCount(1);
}

/**************************************************************************//**
* 
* \brief   - Set eqep counter maximum limit value.
*
* \param   - count.
*
* \return  - If the setting is successful, it returns OK, 
* 			otherwise it will return ERROR; .
*
******************************************************************************/
int HeightEncoder::SetMaxCount(const unsigned int count)
{
	return vxbEqepSetMaxCount(1, count);
}

/**************************************************************************//**
* 
* \brief   - Get eqep counter maximum limit value.
*
* \param   - None.
*
* \return  - If the getting is successful, it returns maximum count number, 
* 			otherwise it will return ERROR; .
*
******************************************************************************/
int HeightEncoder::GetMaxCount()
{
	return vxbEqepGetMaxCount(1);
}

/**************************************************************************//**
* 
* \brief   - Get eqep current count value.
*
* \param   - None.
*
* \return  - If the getting is successful, it returns current count number, 
* 			otherwise it will return ERROR; .
*
******************************************************************************/
int HeightEncoder::GetPositionCount()
{
	return vxbEqepGetPosCount(1);
}

/**************************************************************************//**
* 
* \brief   - Get eqep current moving direction.
*
* \param   - None.
*
* \return  - moving direction 0/1.
*            If there is any issue on the getting, it returns ERROR.
*
******************************************************************************/
int HeightEncoder::GetDirection()
{
	return vxbEqepGetDirection(1);
}
