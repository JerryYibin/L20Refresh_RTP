/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef EMRSNSOCKET_H_
#define EMRSNSOCKET_H_
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "CommunicationImplementer.h"

#define MAX_READ_WAIT_TIME_CLIENT 	2 // Seconds.  2 Seconds required if network is too busy.
#define TEMP_READ_BUFF_LEN          1000
#define MAX_READ_WAIT_TIME			3 /* N - No.of seconds */
/* While blocking on Read() if there is no response
 from the other side for more than 300 secs the connection terminates. */

extern "C"
{
	#include <customSystemCall.h>
}

class EmrsnSocket : public CommunicationImplementer
{
public:
	enum SOCKET_CLIENT_TYPE
	{
		HMI_SOCKET_CONNECTION,
		ACT_SOCKET_CONNECTION,
		CUSTOM_SOCKET_CONNECTION,
		GATEWAY_SOCKET_CONNECTION
	};
public:
	
	/*
	 ** It determines, whether the current object acts as Server/Client.
	 */
	bool bServer;

	/*
	 ** It determines, whether WSAStartup function called once or not
	 */
	static bool bStaticWSAStartupSocket;

	/*
	 ** The Socket identifier.The Accept() returns the new socket id. So the new
	 ** socket identifier must be set after creating a new socket object.
	 ** So this is made public.
	 */
	int dwsock;
	
	timeval timeout;

	sockaddr_in   msockaddr_in;

	fd_set ReadFdset;

public:
	/*
	 ** The socket identifier is initialized in this method.
	 */
	EmrsnSocket();

	/*
	 ** Here the socket will be closed.
	 */
	virtual ~EmrsnSocket();
public:
	/*
	 **  This method creates a new Socket.It also sets the option to the socket
	 **  such that the socket may listen at the same port no.
	 */
	bool Create();


	/*
	 **  This method closes the the socket. The shutdown() stops the communication.
	 */
	bool Close();

	/*
	 **  This method checks whether the port no is free or not
	 */
	bool IsFree(const unsigned int port); 

	/*
	 ** This method binds the socket to the specified port no.
	 */
	bool Bind(const unsigned int port);

	/*
	 ** This method is used to connect with timeout to the specified address and port no.
	 ** If dwTimeOutInSec is not specified then default timeout is 60 seconds.
	 */
	bool Connect(const char * address, const unsigned int port,const int dwTimeOutInSec = 60);
	/*
	 **  This method listens for connections at the binded port no.
	 */
	bool Listen();

	/*
	 **  This method accepts the incoming connections. This is the method where
	 **	the process blocks after listen() for incoming connections.
	 */
	int Accept(SOCKET_CLIENT_TYPE clientType);

	// I/O routines

	/*
	 ** This method is used to send the information between the connected sockets
	 */
	int Send(const char *buff, const unsigned int bufflen);

	/*
	 ** This method is used to get the information from the connected sockets.
	 ** recv() blocks till a message comes from the specified socket.
	 ** But generally this should be private method and should be used by
	 ** ReadExact.  But as a general case, this is made as public
	 */
	int Read(char *buff, const unsigned int bufflen);


	/*
	 **  This method exactly reads the data from the connected socket.
	 */
	int ReadExact(char *buff, const unsigned int bufflen);


	void GetIPAddress(char *pszIPAddress);

};

#endif /* EMRSNSOCKET_H_ */
