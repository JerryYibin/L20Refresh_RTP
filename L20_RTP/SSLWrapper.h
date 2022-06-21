/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef SSLWRAPPER_H_
#define SSLWRAPPER_H_
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define SSL_PEM_PATH		"/romfs/server.pem"
#define SSL_RETRY_ATTEMPT	2
class SSLWrapper {
public:
	SSLWrapper			();
	virtual ~SSLWrapper	();
	bool 	InitSSL		();
	bool 	CreateSSL	();
	bool 	BindSSL		(int socketDescriptor);
	bool 	AcceptSSL	();
	bool 	ResetSSL	();
	int 	SendSSL		(char* buff, const int len);
	int 	RecvSSL		(char* buff, const int len);
private:
	SSL_CTX*	m_ptrSSL_CTX;
	SSL*		m_ptrSSL;
private:
	void 		initOpenSSL		();
	void 		cleanUpOpenSSL	();
	SSL_CTX* 	createContext	();
	bool 		configContext	(SSL_CTX* ptrCTX);
	
};

#endif /* SSLWRAPPER_H_ */
