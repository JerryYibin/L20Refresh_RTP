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
#define DIG_SERVERS 10
using namespace std;
class Connectivity 
{
private:
	static Connectivity* 						_ConnectivityObj;
private:
							Connectivity	();
public:
	static struct ETHERNET 						EthernetConfig;
	static vector<struct GATEWAY_SERVER>* 		_DIGServersUI;
	static map<int, struct GATEWAY_SERVER>* 	_DIGServersSC;
	virtual 				~Connectivity	();
	static Connectivity*	GetInstance		();
	
							Connectivity	(const Connectivity&) 			= delete;
							Connectivity& operator=(const Connectivity&) 	= delete;
	


};

#endif /* CONNECTIVITY_H_ */
