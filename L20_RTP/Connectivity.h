/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used to define all the Ethernet settings.

***************************************************************************/

#ifndef CONNECTIVITY_H_
#define CONNECTIVITY_H_
#include "ConnectivityUI.h"
#include <vector>
#include <map>
using namespace std;
class Connectivity 
{
public:
	static struct ETHERNET EthernetConfig;
	static vector<struct GATEWAY_SERVER>* 		_DIGServersUI;
	static map<int, struct GATEWAY_SERVER>* 	_DIGServersSC;
	virtual 				~Connectivity	();
	static Connectivity*	GetInstance		();
private:
							Connectivity	();

private:
	static Connectivity* _ConnectivityObj;	
};

#endif /* CONNECTIVITY_H_ */
