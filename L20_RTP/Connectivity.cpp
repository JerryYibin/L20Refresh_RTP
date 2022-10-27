/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used to define all the Ethernet settings.

***************************************************************************/

#include "Connectivity.h"
struct ETHERNET Connectivity::EthernetConfig;
vector<struct GATEWAY_SERVER>* Connectivity::_DIGServersUI = nullptr;
map<int, struct GATEWAY_SERVER>* Connectivity::_DIGServersSC = nullptr;
Connectivity* Connectivity::_ConnectivityObj = nullptr;	
/**************************************************************************//**
* \brief   - Constructor - Initialize _DIGServersUI,_DIGServersSC and EthernetConfig.
* 			 The data should be from database in the future.
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
Connectivity::Connectivity() 
{
	_DIGServersUI = new vector<struct GATEWAY_SERVER>;
	_DIGServersSC = new map<int, GATEWAY_SERVER>;
	EthernetConfig.EthernetType = TCP_IP;
	EthernetConfig.TCP_RemoteSignature = true;
	EthernetConfig.TCP_ServerPort = 4000;
	memcpy(EthernetConfig.TCP_ServerIP, "192.168.0.100", IP_ADD_LEN);
	EthernetConfig.DIG_WeldResult = false;
	EthernetConfig.DIG_SysConfigure = false;
	EthernetConfig.DIG_WeldRecipe = false;
	EthernetConfig.DIG_Signature = false;
	EthernetConfig.DIG_ServerID = 0;
	struct GATEWAY_SERVER server[10] = {{"Machine 01", 65100, "150.150.150.10"},
										{"Machine 02", 65101, "150.150.150.11"},
										{"Machine 03", 65102, "150.150.150.12"},
										{"Machine 04", 65103, "150.150.150.13"},
										{"Machine 05", 65104, "150.150.150.14"},
										{"Machine 06", 65105, "150.150.150.15"},
										{"Machine 07", 65106, "150.150.150.16"},
										{"Machine 08", 65107, "150.150.150.17"},
										{"Machine 09", 65108, "150.150.150.18"},
										{"Machine 10", 65109, "150.150.150.19"}};
	_DIGServersUI->clear();
	_DIGServersSC->clear();
	for(int i = 0; i < sizeof(server); i++)
	{
		_DIGServersUI->push_back(server[i]);
		_DIGServersSC->insert(pair<int, struct GATEWAY_SERVER>(i, server[i]));
	}
	
	
}

/**************************************************************************//**
* 
* \brief   - Destructor. destruct _DIGServersUI,_DIGServersSC and EthernetConfig
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
Connectivity::~Connectivity() {
	_DIGServersUI->clear();
	_DIGServersSC->clear();
	if(_DIGServersUI != nullptr)
		delete _DIGServersUI;
	if(_DIGServersSC != nullptr)
		delete _DIGServersSC;
	delete _ConnectivityObj;
}

/**************************************************************************//**
* \brief   - Return the single instance of class.
*
* \param   - None.
*
* \return  - Connectivity* Object 
*
******************************************************************************/
Connectivity* Connectivity::GetInstance()
{
	return (_ConnectivityObj != nullptr) ? _ConnectivityObj : (_ConnectivityObj = new Connectivity());
}
