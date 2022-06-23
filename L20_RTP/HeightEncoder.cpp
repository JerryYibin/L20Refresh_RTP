/*
 * HeightEncoder.cpp
 *
 *  Created on: May 27, 2022
 *      Author: JerryW.Wang
 */

#include "HeightEncoder.h"
extern "C"
{
	#include <vxbEqepLib.h>
}
HeightEncoder::HeightEncoder() {
	// TODO Auto-generated constructor stub

}

HeightEncoder::~HeightEncoder() {
	// TODO Auto-generated destructor stub
}

int HeightEncoder::SetInitCount(const unsigned int count)
{
	return vxbEqepSetInitCount(1, count);
}


int HeightEncoder::GetInitCount()
{
	return vxbEqepGetInitCount(1);
}

int HeightEncoder::SetMaxCount(const unsigned int count)
{
	return vxbEqepSetMaxCount(1, count);
}

int HeightEncoder::GetMaxCount()
{
	return vxbEqepGetMaxCount(1);
}

int HeightEncoder::GetPositionCount()
{
	return vxbEqepGetPosCount(1);
}

int HeightEncoder::GetDirection()
{
	return vxbEqepGetDirection(1);
}
