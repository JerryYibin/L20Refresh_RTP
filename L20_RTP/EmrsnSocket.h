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
#define TCP_INFO		11	/* Information about this connection. */
#define TCP_KEEPIDLE	4	/* Start keeplives after this period */
#define TCP_KEEPINTVL	5	/* Interval between keepalives */
#define TCP_KEEPCNT		6	/* Number of keepalives before death */
#define SOL_TCP IPPROTO_TCP
struct tcp_info {
	UINT8	tcpi_state;
	UINT8	tcpi_ca_state;
	UINT8	tcpi_retransmits;
	UINT8	tcpi_probes;
	UINT8	tcpi_backoff;
	UINT8	tcpi_options;
	UINT8	tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;
	UINT8	tcpi_delivery_rate_app_limited:1;

	UINT32	tcpi_rto;
	UINT32	tcpi_ato;
	UINT32	tcpi_snd_mss;
	UINT32	tcpi_rcv_mss;

	UINT32	tcpi_unacked;
	UINT32	tcpi_sacked;
	UINT32	tcpi_lost;
	UINT32	tcpi_retrans;
	UINT32	tcpi_fackets;

	/* Times. */
	UINT32	tcpi_last_data_sent;
	UINT32	tcpi_last_ack_sent;     /* Not remembered, sorry. */
	UINT32	tcpi_last_data_recv;
	UINT32	tcpi_last_ack_recv;

	/* Metrics. */
	UINT32	tcpi_pmtu;
	UINT32	tcpi_rcv_ssthresh;
	UINT32	tcpi_rtt;
	UINT32	tcpi_rttvar;
	UINT32	tcpi_snd_ssthresh;
	UINT32	tcpi_snd_cwnd;
	UINT32	tcpi_advmss;
	UINT32	tcpi_reordering;

	UINT32	tcpi_rcv_rtt;
	UINT32	tcpi_rcv_space;

	UINT32	tcpi_total_retrans;

	UINT64	tcpi_pacing_rate;
	UINT64	tcpi_max_pacing_rate;
	UINT64	tcpi_bytes_acked;    /* RFC4898 tcpEStatsAppHCThruOctetsAcked */
	UINT64	tcpi_bytes_received; /* RFC4898 tcpEStatsAppHCThruOctetsReceived */
	UINT32	tcpi_segs_out;	     /* RFC4898 tcpEStatsPerfSegsOut */
	UINT32	tcpi_segs_in;	     /* RFC4898 tcpEStatsPerfSegsIn */

	UINT32	tcpi_notsent_bytes;
	UINT32	tcpi_min_rtt;
	UINT32	tcpi_data_segs_in;	/* RFC4898 tcpEStatsDataSegsIn */
	UINT32	tcpi_data_segs_out;	/* RFC4898 tcpEStatsDataSegsOut */

	UINT64   tcpi_delivery_rate;

	UINT64	tcpi_busy_time;      /* Time (usec) busy sending data */
	UINT64	tcpi_rwnd_limited;   /* Time (usec) limited by receive window */
	UINT64	tcpi_sndbuf_limited; /* Time (usec) limited by send buffer */

	UINT32	tcpi_delivered;
	UINT32	tcpi_delivered_ce;

	UINT64	tcpi_bytes_sent;     /* RFC4898 tcpEStatsPerfHCDataOctetsOut */
	UINT64	tcpi_bytes_retrans;  /* RFC4898 tcpEStatsPerfOctetsRetrans */
	UINT32	tcpi_dsack_dups;     /* RFC4898 tcpEStatsStackDSACKDups */
	UINT32	tcpi_reord_seen;     /* reordering events seen */
};
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
	int drsock; // New socket returns from the Accept() of TCP/IP Server
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
	 **  This method closes the new socket created from the Accept() function
	 */
	bool CloseServer();

	/*
	 **  This method checks whether the port no is free or not
	 */
	bool IsFree(const unsigned int port); 

	/*
	 ** This method binds the socket to the specified port no.
	 */
	bool Bind(const unsigned int port);

	/*
	 ** This method binds the socket to the specified IP address and port no.
	 */
	bool Bind(const char * address,const unsigned int port);
	
	/*
	 ** This method is used to connect with timeout to the specified address and port no.
	 ** If dwTimeOutInSec is not specified then default timeout is 60 seconds.
	 */
	bool Connect(const char * address, const unsigned int port,const int dwTimeOutInSec = 60);
	
	/*
	 ** This method is for the Non-Block Connect from TCP/IP Client.
	 ** If dwTimeOutInSec is not specified then default timeout is 60 seconds.
	*/
	bool NBConnect(const char * address, const unsigned int port,const int dwTimeOutInSec = 60);
	/*
	 **  This method listens for connections at the binded port no.
	 */
	bool Listen();

	/*
	 **  This method accepts the incoming connections. This is the method where
	 **	the process blocks after listen() for incoming connections.
	 */
	int Accept(SOCKET_CLIENT_TYPE clientType);
	
	/*
	 **  This method use the select function to realize the NON-Block Accept(2s timeout)
	 */
	int NBAccept(const char * address);

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
	 **  This method is the Non-Block Read function, if newsocket = true means tcp/ip server read messages from client
	 */
	int NBRead(char *buff, const unsigned int bufflen, bool newsocket = false);

	/*
	 **  This method exactly reads the data from the connected socket.
	 */
	int ReadExact(char *buff, const unsigned int bufflen);
	/*
	 **  This method Enable the socket lose connection check 
	 */
	int AliveCheck(bool newsocket = false);
	
	void GetIPAddress(char *pszIPAddress);
	
	/*
	 **  This method check the IP Address setting of Ethernet with specified name 
	 */
	int GetIPAddress(char *ethname, char *Ipaddr);
	/*
	 **  This method Set the IP Address of Ethernet with specified name 
	 */
	int SetIpConfig(char *ethname, char *Ipaddr, char *mask);
	/*
	 **  This method check the SubMask Address setting of Ethernet with specified name 
	 */
	int GetMaskAddress(char *ethname, char *mask);
};

#endif /* EMRSNSOCKET_H_ */
