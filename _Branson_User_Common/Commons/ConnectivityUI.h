/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

The file is used both on ASC and HMI software and it is used to define all the Ethernet settings.

***************************************************************************/

#ifndef CONNECTIVITY_UI_H_
#define CONNECTIVITY_UI_H_
#define IP_ADD_LEN		16
#define SERVER_NAME_LEN	12
enum ETHERNET_TYPE
{
	INVALID_TYPE = -1,
	TCP_IP,
	GATEWAY
};

struct ETHERNET
{
	int 			EthernetType;
	bool 			TCP_RemoteSignature;
	unsigned int 	TCP_ServerPort;
	char			TCP_ServerIP[IP_ADD_LEN];
	bool			DIG_WeldResult;
	bool			DIG_SysConfigure;
	bool			DIG_WeldRecipe;
	bool			DIG_Signature;
	unsigned int	DIG_ServerID;
	
};

struct GATEWAY_SERVER
{
	char			DIG_ServerName[SERVER_NAME_LEN];
	unsigned int	DIG_ServerPort;
	char			DIG_ServerIP[IP_ADD_LEN];
};

#endif
