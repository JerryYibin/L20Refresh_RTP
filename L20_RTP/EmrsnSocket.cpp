/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/
#include <string.h>
#include <stdio.h>
#include "EmrsnSocket.h"
#include "Logger.h"
#include "commons.h"
#include  <sys/ioctl.h>
#include  <sys/socket.h>
#include <net/ipioctl_var.h>
#include <errno.h>
#include "errnoLib.h"
// Set the flag value to false at first time calling
bool EmrsnSocket::bStaticWSAStartupSocket = false;  


/**************************************************************************//**
* \brief    - EmrsnSocket Class constructor
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
EmrsnSocket::EmrsnSocket()
{

	// Clear out the internal socket fields

	dwsock = -1;
	drsock = -1;
	bServer = false;
}


/**************************************************************************//**
* 
* \brief   - EmrsnSocket class destructor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
EmrsnSocket::~EmrsnSocket()
{
	// Close the socket
	Close();
}

/**************************************************************************//**
* 
* \brief   - This method creates a new Socket.It also sets the option to the 
*            socket such that the socket may listen at the same port no.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::Create()
{
	const int one = 1;
	// Check that the old socket was closed
	if (dwsock >= 0)
		Close();

	if ((dwsock = socket(AF_INET, SOCK_STREAM, 0))<0 )
	{
		LOGERR((char *) "EmrsnSocket::Create: Cannot create the socket ", 0, 0, 0);
		return false;
	}
		 
	if (setsockopt(dwsock, SOL_SOCKET, SO_REUSEPORT, (char *)&one, sizeof(one)))
    {
		return false;
    }
    
	return true;
}

/**************************************************************************//**
* 
* \brief   - This method closes the the socket. The shutdown() stops the 
* 			 communication 
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::Close()
{
	int dwResult;
	char buff[TEMP_READ_BUFF_LEN];	// To read data
	int currlen = TEMP_READ_BUFF_LEN;
	int dwRetBuffsize;

	if (dwsock >= 0)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		while(1)
		{
			if((dwResult = select(dwsock + 1,&ReadFdset,(fd_set *)0,(fd_set *)0,&timeout)) <= 0)
				break;
			
			// For Linux, it means 1 Second
			sleep(1);
			// If we will not read the buffer from socket before closing, it goes to
			// TIME_WAIT state. For that we are Reading here.
			dwRetBuffsize = Read(buff, currlen);
			
			if(dwRetBuffsize <= 0)
				break;
			
		}

		close(dwsock); //Unix method
		dwsock = -1;
	}
    return true;
}



/**************************************************************************//**
* 
* \brief   - This method closes the the Server socket. 
* 
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::CloseServer()
{
	int dwResult;
	char buff[TEMP_READ_BUFF_LEN];	// To read data
	int currlen = TEMP_READ_BUFF_LEN;
	int dwRetBuffsize;

	if (drsock >= 0)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		while(1)
		{
			if((dwResult = select(drsock + 1,&ReadFdset,(fd_set *)0,(fd_set *)0,&timeout)) <= 0)
				break;
			
			// For Linux, it means 1 Second
			sleep(1);
			// If we will not read the buffer from socket before closing, it goes to
			// TIME_WAIT state. For that we are Reading here.
			dwRetBuffsize = Read(buff, currlen);
			
			if(dwRetBuffsize <= 0)
				break;
			
		}

		close(drsock); //Unix method
		drsock = -1;
	}
    return true;
}


/**************************************************************************//**
* 
* \brief   - This method checks whether the port no is free or not
* 			 This is not a public method 
*
* \param   - const unsigned int port
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::IsFree(const unsigned int port)
{
	EmrsnSocket dummy;
	
	if (dummy.Create())
	{
		// If we're not able to connect then the port number is in use...
		if (dummy.Connect("localhost", port,2))
		{
			LOGERR((char *) "EmrsnSocket::IsFree: The port no is in use", 0, 0, 0);
		}
		else
		{
			return true;
		}
	}

	LOGERR((char *) "EmrsnSocket::IsFree: Socket can not be created", 0, 0, 0);
	return false;
}

/**************************************************************************//**
* 
* \brief   - This method binds the socket to the specified port no.
*
* \param   - const unsigned int port
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::Bind(const unsigned int port)
{
	bServer = true;
	struct sockaddr_in addr;

	// Check that the socket is open
	if (dwsock < 0)
	{
		LOGERR((char *) "EmrsnSocket::Bind: The socket is not yet created", 0, 0, 0);
		return false;
	}

	// If a specific port is being set then check it's not already used!
//	if (port != 0)
//	{
//	   if(IsFree(port) == false)
//		  return false;
//	}

	// Set up the address to bind the socket to
	addr.sin_family		 = AF_INET;
	addr.sin_port		 = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	if( (addr.sin_addr.s_addr = inet_addr(ip_addr)) == ERROR)
//	{
//		LOGERR((char *) "EmrsnSocket::Bind: Cannot parse the IP address", 0, 0, 0);
//		return false;
//	}
	
	if (bind(dwsock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		LOGERR((char *) "EmrsnSocket::Bind: Cannot bind to the port no ", 0, 0, 0);
		return false;
	}

	LOG("\nEmrsnSocket::Bind: Port ID: %x: %d\n", addr.sin_addr.s_addr, port);
	return true;
}

/**************************************************************************//**
*
* \brief   - This method binds the socket to the specified IP address, port number.
*
* \param   - char* ip address, unsigned int port
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::Bind(const char * address,const unsigned int port)
{
	bServer = true;
	struct sockaddr_in addr;

	// Check that the socket is open
	if (dwsock < 0)
	{
		LOGERR((char *) "EmrsnSocket::Bind: The socket is not yet created", 0, 0, 0);
		return false;
	}

	// If a specific port is being set then check it's not already used!
//	if (port != 0)
//	{
//	   if(IsFree(port) == false)
//		  return false;
//	}

	// Set up the address to bind the socket to
	addr.sin_family		 = AF_INET;
	addr.sin_port		 = htons(port);
	addr.sin_addr.s_addr = inet_addr(address);
//	if( (addr.sin_addr.s_addr = inet_addr(ip_addr)) == ERROR)
//	{
//		LOGERR((char *) "EmrsnSocket::Bind: Cannot parse the IP address", 0, 0, 0);
//		return false;
//	}

	if (bind(dwsock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		LOGERR((char *) "EmrsnSocket::BindSingle: Bind Fail ", 0, 0, 0);
		return false;
	}
	

	LOG("\nEmrsnSocket::Bind: IP Address %s, Port ID: %d\n", address, port);
	

	return true;
}



/**************************************************************************//**
* 
* \brief   - This method is used to connect with timeout to the specified 
* 			 address and port no.
*            If Timeout is not specified then the default timeout will be 
*            60 seconds. (Blocking)
*            
* \param   - const char * address, const unsigned int port,const int dwTimeOutInSec
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::Connect(const char * address, const unsigned int port,const int dwTimeOutInSec)
{
	long dwOrigFlag, dwFlag;
	fd_set		myset; 
	struct		timeval timeout; 
	int			valopt; 
	socklen_t	len; 

	// Check the socket
	if (dwsock < 0)
		return false;
	
	// Create an address structure and clear it
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	
	// Fill in the address if possible
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr  = inet_addr(address);

	// Set the port number in the correct format
	addr.sin_port = htons(port);

	// Read the file descriptor flags
	if( (dwOrigFlag = fcntl(dwsock, F_GETFL, NULL)) < 0) 
		return false;

	// Add blocking mode to the existing flags
	dwFlag = dwOrigFlag & ~O_NONBLOCK; 
	
	// Save the file descriptor flags
	if( fcntl(dwsock, F_SETFL, dwFlag) < 0) 
		return false; 

	// Connect to the server
	int dwRet = connect (dwsock, (struct sockaddr*)&addr, sizeof(struct sockaddr));

	if (dwRet < 0 )
	{
		// If the connection is still in progress
		if (errno == EINPROGRESS) 
		{ 
			// Specify the time to wait before getting timeout.
			timeout.tv_sec	= dwTimeOutInSec; 
			timeout.tv_usec = 0; 
			FD_ZERO(&myset); 
			FD_SET(dwsock, &myset); 

			// Blocked here until connection is established or time out occurs.
			if (select(dwsock + 1, NULL, &myset, NULL, &timeout) > 0) 
			{ 
				len = sizeof(int); 
				getsockopt(dwsock, SOL_SOCKET, SO_ERROR, &valopt, &len);
				// Get the Socket error status .
				if (valopt) 
					return false;
			} 
			else 
			{ 
				// Wait time expired.
				return false;
			} 
		} 
		else 
		{ 
			// Connect failed.
			return false;
		} 
	}

	if(fcntl(dwsock, F_SETFL, dwOrigFlag) < 0) 
	{
		Close();
		return false;
	}
	
    return true;
}



/**************************************************************************//**
* 
* \brief   - This method is used to connect with timeout to the specified 
* 			 address and port no.
*            If Timeout is not specified then the default timeout will be 
*            60 seconds. 
*            
* \param   - const char * address, const unsigned int port,const int dwTimeOutInSec
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::NBConnect(const char * address, const unsigned int port,const int dwTimeOutInSec)
{
	long dwOrigFlag, dwFlag;
	fd_set		myset; 
	struct		timeval timeout; 
	int			valopt; 
	socklen_t	len; 

	// Check the socket
	if (dwsock < 0)
		return false;
	
	// Create an address structure and clear it
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	
	// Fill in the address if possible
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr  = inet_addr(address);

	// Set the port number in the correct format
	addr.sin_port = htons(port);

	// Read the file descriptor flags
	if( (dwOrigFlag = fcntl(dwsock, F_GETFL, NULL)) < 0) 
		return false;

	// Add blocking mode to the existing flags
	dwFlag = dwOrigFlag | O_NONBLOCK; 
	
	// Save the file descriptor flags
	if( fcntl(dwsock, F_SETFL, dwFlag) < 0) 
		return false; 
	// Connect to the server
	int dwRet = connect (dwsock, (struct sockaddr*)&addr, sizeof(struct sockaddr));

	if (dwRet < 0 )
	{
		// If the connection is still in progress
		if (errno == EINPROGRESS) 
		{ 
			// Specify the time to wait before getting timeout.
			timeout.tv_sec	= dwTimeOutInSec; 
			timeout.tv_usec = 0; 
			FD_ZERO(&myset); 
			FD_SET(dwsock, &myset); 

			// Blocked here until connection is established or time out occurs.
			if (select(dwsock + 1, NULL, &myset, NULL, &timeout) > 0) 
			{ 
				len = sizeof(int); 
				getsockopt(dwsock, SOL_SOCKET, SO_ERROR, &valopt, &len);
				// Get the Socket error status .
				if (valopt) 
					return false;
			} 
			else 
			{ 
				// Wait time expired.
				return false;
			} 
		} 
		else 
		{ 
			// Connect failed.
			return false;
		} 
	}

	if(fcntl(dwsock, F_SETFL, dwOrigFlag) < 0) 
	{
		Close();
		return false;
	}
	
    return true;
}



/**************************************************************************//**
* 
* \brief   - This method listens for connections at the bound port no.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool EmrsnSocket::Listen()
{
	// Check socket
	if (dwsock < 0)
		return false;

	// The number is the size of the waiting queue
	if (listen(dwsock, 10) < 0)
		return false;

	return true;
}

/**************************************************************************//**
* 
* \brief   - This method accepts the incoming connections. This is the 
* 			 method where the process blocks after listen() for incoming
* 			 connections.(Blocking)
*
* \param   - SOCKET_CLIENT_TYPE clientType
*
* \return  - int
*
******************************************************************************/
int EmrsnSocket::Accept(SOCKET_CLIENT_TYPE clientType)
{
	int new_socket_id;
	int dwLen = sizeof(msockaddr_in);

	union {
		UINT32 	aslong;
		char	asInet[4];
	} addrLow, addrHi, sockAddr;
	
	/* set boundaries for HMi connections
	 * note: needs to be taken from the boot line
	 * will write boot line to eeprom and read from there
	 */
#if INCLUDE_TI_AM5708_BRANSON_LEVEL1
	addrLow.aslong  = inet_addr(clientType == HMI_SOCKET_CONNECTION ? "192.168.1.1" : "192.168.0.1");
	addrHi.aslong  = inet_addr(clientType == HMI_SOCKET_CONNECTION ? "192.168.1.255": "192.168.0.255");
#elif INCLUDE_TI_AM5708_BRANSON
	addrLow.aslong  = inet_addr(clientType == HMI_SOCKET_CONNECTION ? "192.168.1.1" : "192.168.0.1");
	addrHi.aslong  = inet_addr(clientType == HMI_SOCKET_CONNECTION ? "192.168.1.255": "192.168.0.255");
#else
	addrLow.aslong  = inet_addr(clientType == HMI_SOCKET_CONNECTION ? "192.168.1.1" : "192.168.1.129");
	addrHi.aslong  = inet_addr(clientType == HMI_SOCKET_CONNECTION ? "192.168.1.128": "192.168.1.255");
#endif
	// Check this socket
	if (dwsock < 0)
	{
		return ERROR;
	}
	
	if ((new_socket_id = accept(dwsock,(struct sockaddr *)&msockaddr_in,(socklen_t *)&dwLen)) < 0)
	{
		LOGERR((char *) "EmrsnSocket::Accept: accept Error", 0, 0, 0);
		return ERROR;
	}

	sockAddr.aslong = msockaddr_in.sin_addr.s_addr; 
	
#if SOCKET_DBG
	printf ("\n%sEmrsnSocket::Accept: ssRecipeValues: sockAddr.asInet[0] (msockaddr_in): %d %s\n", KCYN, sockAddr.asInet[0],KNRM);
	printf ("\n%sEmrsnSocket::Accept: ssRecipeValues: sockAddr.asInet[1] (msockaddr_in): %d %s\n", KCYN, sockAddr.asInet[1],KNRM);
	printf ("\n%sEmrsnSocket::Accept: ssRecipeValues: sockAddr.asInet[2] (msockaddr_in): %d %s\n", KCYN, sockAddr.asInet[2],KNRM);
	printf ("\n%sEmrsnSocket::Accept: ssRecipeValues: sockAddr.asInet[3] (msockaddr_in): %d %s\n", KCYN, sockAddr.asInet[3],KNRM);
#endif
	
	// check boundaries of incoming connection here...
	if (sockAddr.asInet[0] == addrLow.asInet[0] && sockAddr.asInet[1] == addrLow.asInet[1] && sockAddr.asInet[2] == addrLow.asInet[2]
									&& sockAddr.asInet[3] >= addrLow.asInet[3] && sockAddr.asInet[3] <= addrHi.asInet[3])
	{
		LOG("\nEmrsnSocket::Accept: %s: %d.%d.%d.%d\n", clientType == HMI_SOCKET_CONNECTION ? "HMI_SOCKET" : "ACT_SOCKET", sockAddr.asInet[0],
				sockAddr.asInet[1], sockAddr.asInet[2], sockAddr.asInet[3]);
		return new_socket_id;
	}
	else
	{
		LOGERR((char *) "EmrsnSocket::Accept: Connection rejected", 0, 0, 0);
		close (new_socket_id);
	}
	
	return 0;
}

/**************************************************************************//**
* 
* \brief   - This method accepts the incoming connections for the cpsw0 port with specified ip.(Non Blocking)
*			
* \param   - The ip address of cpsw0 configured by customer
*
* \return  - int
*
******************************************************************************/
int EmrsnSocket::NBAccept(const char * address)
{
	int dwLen = sizeof(msockaddr_in);
	int iresult = ERROR; 
	union {
		UINT32 	aslong;
		char	asInet[4];
	} addrRef, sockAddr;
	
	addrRef.aslong  = inet_addr(address);
	if (dwsock < 0)
	{
		return ERROR;
	}
	
	if(fcntl(dwsock,F_SETFL,O_NONBLOCK) < 0)
    {
		LOGERR((char *) "EmrsnSocket::NBAccept: Change to Non-Block Fail\n", 0, 0, 0);
        return ERROR;
    }
	/*Use Select to realize non-blocking Accept*/
	 struct fd_set fds;
	 struct timeval timeOut={2,0};
	 FD_ZERO(&fds);
	 FD_SET(dwsock,&fds);
	 if(select(dwsock + 1, &fds, NULL, NULL, &timeOut)>0)
	 {
		if ((drsock = accept(dwsock,(struct sockaddr *)&msockaddr_in,(socklen_t *)&dwLen)) < 0)
		{
			LOGERR((char *) "EmrsnSocket::Accept: accept Error", 0, 0, 0);
			return ERROR;
		}
			
		sockAddr.aslong = msockaddr_in.sin_addr.s_addr; 
			
		// check boundaries of incoming connection here...
		if (sockAddr.asInet[0] == addrRef.asInet[0] && sockAddr.asInet[1] == addrRef.asInet[1] && sockAddr.asInet[2] == addrRef.asInet[2]
											&& sockAddr.asInet[3]!=addrRef.asInet[3])
		{
			LOG("\nEmrsnSocket::Accept: %d.%d.%d.%d\n", sockAddr.asInet[0], sockAddr.asInet[1], sockAddr.asInet[2], sockAddr.asInet[3]);
			iresult = drsock;
		}
		else
		{
			LOGERR((char *) "EmrsnSocket::Accept: Connection rejected\n", 0, 0, 0);
			close (drsock);
		}
		 
	 }
	return iresult;
}

/**************************************************************************//**
* 
* \brief   - This method is used to send the information between the 
* 			 connected sockets 
*
* \param   - const char *buff, const unsigned int bufflen
*
* \return  - int dwReturnCode
*
******************************************************************************/
int EmrsnSocket::Send(const char *buff, const unsigned int bufflen)
{
	long	dwReturnCode;
	
#if SOCKET_DBG	
	printf("\n send buffer ->");
	for (int i=0;i<bufflen;i++)
		{
		printf("%x,",buff[i]);
		}
#endif
	
	dwReturnCode = send(dwsock, buff, bufflen, 0);
	if(dwReturnCode < 0)
	{
		LOGERR((char *) "EmrsnSocket::Send: errno: %d", errno,0,0);
		return -1;
	}

	return dwReturnCode;
}
/**************************************************************************//**
* 
* \brief   - This method is used to send the information between the 
* 			 connected sockets 
*
* \param   - const char *buff, const unsigned int bufflen
* 			bool newsocket: for the new socket created by Accept, input true
*
* \return  - int dwReturnCode
*
******************************************************************************/
int EmrsnSocket::Send(const char *buff, const unsigned int bufflen, bool newsocket)
{
	long dwReturnCode;
	int socketid;
	int flags;
	if(newsocket)
		socketid = drsock;
	else
		socketid = dwsock;
	flags = fcntl(socketid,F_GETFL,0);
	fcntl(socketid,F_SETFL,flags&~O_NONBLOCK);
#if SOCKET_DBG	
	printf("\n send buffer ->");
	for (int i=0;i<bufflen;i++)
		{
		printf("%x,",buff[i]);
		}
#endif
	printf("\nEmrsnSocket::Send -- length = %d\n",bufflen);
	dwReturnCode = send(socketid, buff, bufflen, 0);
	if(dwReturnCode < 0)
	{
		LOGERR((char *) "EmrsnSocket::Send: errno: %d", errno,0,0);
		return -1;
	}
	flags = fcntl(socketid,F_GETFL,0);
	fcntl(socketid,F_SETFL,flags|O_NONBLOCK);
	return dwReturnCode;
}
/**************************************************************************//**
* 
* \brief   - This method is used to get the information from the connected
* 			 sockets. 
* 			 recv() blocks till a message comes from the specified socket. 
*
* \param   - char *buff, const unsigned int bufflen
*
* \return  - int len
*
******************************************************************************/
int EmrsnSocket::Read(char *buff, const unsigned int bufflen)
{

	int  len=0;
	len =recv(dwsock, buff, bufflen, 0);

	if(len < 0)
	{
		LOGERR((char *) "EmrsnSocket::Read: The Error code = %d,%d\n",errno,len,0);
	}
	return len;
}

/**************************************************************************//**
* 
* \brief   - None-Blocking Read Function 
* 			
* \param   - char *buff, const unsigned int bufflen
* 			 bool newsocket: for the new socket created by Accept, input true
*
* \return  - int len
*
******************************************************************************/
int EmrsnSocket::NBRead(char *buff, const unsigned int bufflen, bool newsocket)
{

	int  len=0;
	int errid = 0;
	int socketid;
	unsigned long ul = 1;
	
	if(newsocket)
		socketid = drsock;
	else
		socketid = dwsock;
	
	if(fcntl(socketid,F_SETFL,O_NONBLOCK) < 0)
    {
		LOGERR((char *) "EmrsnSocket::NBRead: Change to Non-Block Fail\n", 0, 0, 0);
        return ERROR;
    }

	len =recv(socketid, buff, bufflen, 0);
	if(len == 0) //Detect socket connection lose
	{
		LOGERR((char *) "EmrsnSocket::NBRead: Connection may lose",0,0,0);
		return ERROR;
	}
	else if (len == ERROR)
	{
		errid = errnoGet();		
		if((errid != EAGAIN)&&(errid != EWOULDBLOCK)&&(errid != EINTR))
		{
			LOGERR((char *) "EmrsnSocket::NBRead: The Error code = %d, %d\n",errid,len,0);
			return ERROR;
		}
		else // Normal case because currently is non-blocking receive
			return OK;
	}

	return len;
}


/**************************************************************************//**
* 
* \brief   - This method exactly reads the data from the connected socket.
*
* \param   - char *buff, const unsigned int buffle
*
* \return  - int bufflen
*
******************************************************************************/
int EmrsnSocket::ReadExact(char *buff, const unsigned int bufflen)
{
	int n;
	unsigned int currlen = bufflen;
	int dwResult;
	char szError[TEMP_READ_BUFF_LEN];
	
	while (currlen > 0)
	{
		if(bServer)
		{
			timeout.tv_sec  = MAX_READ_WAIT_TIME;
		}
		else
		{
			timeout.tv_sec  = MAX_READ_WAIT_TIME_CLIENT;
		}

		timeout.tv_usec = 0;
		FD_ZERO(&ReadFdset);
		FD_SET(dwsock,&ReadFdset);

		if((dwResult = select(dwsock + 1,&ReadFdset,(fd_set *)0,(fd_set *)0,&timeout)) < 0)
		{
			LOGERR((char *) "EmrsnSocket::ReadExact: Error in Select\n", 0, 0, 0);
			return -1;
		}
			
		if(dwResult == 0)
		{
			LOGERR((char *) "EmrsnSocket::ReadExact:Time out.\n", 0, 0, 0);
			return 0;
		}

		// Try to read some data in
		n = Read(buff, currlen);

		if (n > 0)
		{
			// Adjust the buffer position and size
			buff += n;
			currlen -= n;
		} 
		else if (n == 0) 
		{
           if(dwResult)
                return -1; // If Select was success and Read was fail then the socket was Closed.
			return 0;
		} 
		else 
		{
		   if(dwResult)
				return -1; // If Select was success and Read was fail then the socket was Closed.

		   if(errno != EAGAIN)
			{
				sprintf(szError,"EmrsnSocket::ReadExact:Error %d In Read Exact", errno);

				return -2;
			}
		}
    }

	return bufflen;
}

/**************************************************************************//**
* 
* \brief   - This Method is used to get IP address of the client who is 
* 			 connected to the server
*
* \param   - char *pszIPAddress
*
* \return  - None
*
******************************************************************************/
void EmrsnSocket::GetIPAddress(char *pszIPAddress)
{
	strcpy(pszIPAddress,inet_ntoa(msockaddr_in.sin_addr));
}

/**************************************************************************//**
* 
* \brief   - This Method is used to enable TCP/IP check alive function.
* Once check alive is enabled, the TCP/IP link will auto send probe heartbeat
* signal to check whether the Connection is still on, if detect connection lose,
* The socket send/recv function will return error
*
* \param   - bool newsocket: for the new socket created by Accept, input true
*
* \return  - OK or ERROR
*
******************************************************************************/
int EmrsnSocket::AliveCheck(bool newsocket)
{
	
	int keepAlive = 1; // Enable keep alive flage
	int keepIdle = 5; // If no data com within keepIdle, start probe 
	int keepInterval = 5; // Keep send package interval
	int keepCount = 3; //Probe count
	int socketid;
	
	if(newsocket)
		socketid = drsock;
	else
		socketid = dwsock;
	
	if(setsockopt(socketid,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == ERROR)
	{
		LOGERR((char *) "EmrsnSocket::AliveCheck: setsockopt SO_KEEPALIVE Select\n", 0, 0, 0);
	}
	if(setsockopt(socketid,SOL_TCP,TCP_KEEPIDLE,(void *)&keepIdle,sizeof(keepIdle)) == ERROR)
	{
		LOGERR((char *) "EmrsnSocket::AliveCheck: setsockopt TCP_KEEPIDLE Select\n", 0, 0, 0);
	}
	if(setsockopt(socketid,SOL_TCP,TCP_KEEPINTVL,(void *)&keepInterval,sizeof(keepInterval)) == ERROR)
	{
		LOGERR((char *) "EmrsnSocket::AliveCheck: setsockopt TCP_KEEPINTVL Select\n", 0, 0, 0);
	}
	if(setsockopt(socketid,SOL_TCP,TCP_KEEPCNT,(void *)&keepCount,sizeof(keepCount)) == ERROR)
	{
		LOGERR((char *) "EmrsnSocket::AliveCheck: setsockopt TCP_KEEPCNT Select\n", 0, 0, 0);
	}

	return OK; //Even Set Alive fail think won't affect the primary socket function
}


/**************************************************************************//**
* \brief   - Set the IP Address of the External Ethernet Port(cpsw0)
*
* \param   - string IP address, string net mask address, string gateway address
*
* \return  - true or false.
*
******************************************************************************/
int EmrsnSocket::SetIpConfig(char *ethname, char *Ipaddr, char *mask)
{
	 int dwsock;
	 struct ifreq ifr;
	 struct sockaddr_in *sin;
	if ((dwsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		LOGERR((char *) "EmrsnSocket::SetIfAddress Cannot create the socket\n", 0, 0, 0);
		return ERROR;
	}
	
	memset(&ifr,0,sizeof(ifr)); 
	strcpy(ifr.ifr_name,ethname); 
	sin = (struct sockaddr_in*)&ifr.ifr_addr; 
	//ipaddr Setting
	sin->sin_family = AF_INET;    
	inet_aton(Ipaddr,&(sin->sin_addr));
	
    if(ioctl(dwsock, SIOCSIFADDR, &ifr) < 0)
    {
		LOGERR((char *) "EmrsnSocket::SetIfAddress Set IPAddress Fail\n", 0, 0, 0);
        return ERROR;
    }
    else
    	LOG("EmrsnSocket::SetIfAddress Set IP Address to %s success!\n", Ipaddr);
    //netmask Setting
	inet_aton(mask,&(sin->sin_addr));
    if(ioctl(dwsock, SIOCSIFNETMASK, &ifr) < 0)
    {
		LOGERR((char *) "EmrsnSocket::SetIfAddress Set NetMask Fail\n", 0, 0, 0);
        return ERROR;
    }
    else
    	LOG("EmrsnSocket::SetIfAddress Set NetMask to %s success!\n", mask);
 
    close(dwsock);
    return OK;
}

/**************************************************************************//**
* \brief   - Get the IP Address of the Ethernet Port with name "ethname"
*
* \param   - get the string IP address 
*
* \return  - true or false.
*
******************************************************************************/
int EmrsnSocket::GetIPAddress(char *ethname, char *Ipaddr)
{
	int dwsock;	
	struct sockaddr_in *sin;
	struct ifreq ifr;

	if ((dwsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		LOGERR((char *) "EmrsnSocket::GetIPAddress Cannot create the socket\n ", 0, 0, 0);
		return ERROR;
	}
	

	memset(&ifr,0,sizeof(ifr)); 
    memcpy(ifr.ifr_name, ethname, strlen(ethname));
    
    if(ioctl(dwsock, SIOCGIFADDR, &ifr) <0)
    {
		LOGERR((char *) "EmrsnSocket::GetIPAddress Read ip fail\n ", 0, 0, 0);
		return ERROR;
    }
    
    sin = (struct sockaddr_in*)&ifr.ifr_addr; 
    strcpy(Ipaddr, inet_ntoa(sin->sin_addr));
	LOG("EmrsnSocket::GetIPAddress get IPAddress %s success!\n", Ipaddr);
	return OK;
}
/**************************************************************************//**
* \brief   - Get the Mask Address of the External Ethernet Port(cpsw0)
*
* \param   - get the mask IP address 
*
* \return  - true or false.
*
******************************************************************************/
int EmrsnSocket::GetMaskAddress(char *ethname, char *mask)
{
	int dwsock;	
	struct sockaddr_in *sin;
	struct ifreq ifr;

	if ((dwsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		LOGERR((char *) "\EmrsnSocket::GetMaskAddress Cannot create the socket ", 0, 0, 0);
		return ERROR;
	}
	

	memset(&ifr,0,sizeof(ifr)); 
    memcpy(ifr.ifr_name, ethname, strlen(ethname));
    
    if(ioctl(dwsock, SIOCGIFNETMASK, &ifr) <0)
    {
		LOGERR((char *) "\EmrsnSocket::GetMaskAddress Read ip fail ", 0, 0, 0);
		return ERROR;
    }
    
    sin = (struct sockaddr_in*)&ifr.ifr_addr; 
    strcpy(mask, inet_ntoa(sin->sin_addr));
	LOG("\EmrsnSocket::GetMaskAddress get mask %s success!", mask);
	 return OK;
}

