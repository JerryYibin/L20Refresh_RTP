/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Functions for SSL-Socket communication
 
**********************************************************************************************************/

#include "SSLWrapper.h"
#include "Logger.h"

/**************************************************************************//**
* \brief    - SSLWrapper Class constructor
*
* \param    - None
*
* \return   - None
*
******************************************************************************/
SSLWrapper::SSLWrapper()
	:m_ptrSSL_CTX(nullptr)
	,m_ptrSSL(nullptr)
{
	// TODO Auto-generated constructor stub

}

/**************************************************************************//**
* 
* \brief   - SSLSocket class destructor
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
SSLWrapper::~SSLWrapper() 
{
	SSL_CTX_free(m_ptrSSL_CTX);
	cleanUpOpenSSL();
}

/**************************************************************************//**
* 
* \brief   - SSL initialization for SSL CTX configuration
*
* \param   - None
*
* \return  - If there is any issue in the initialization, it will return false
*
******************************************************************************/
bool SSLWrapper::InitSSL()
{
	bool bResult = false;
	initOpenSSL();
	m_ptrSSL_CTX = createContext();
	if(m_ptrSSL_CTX == NULL)
	{
		LOGERR((char *)"SSL Socket: Error on Initialize SSL\n", 0, 0, 0);
		return bResult;
	}
	bResult = configContext(m_ptrSSL_CTX);
	return bResult;
}

/**************************************************************************//**
* 
* \brief   - SSL creating
*
* \param   - None
*
* \return  - If there is any issue in the initialization, it will return false
*
******************************************************************************/
bool SSLWrapper::CreateSSL()
{
	bool bResult = false;

	if(m_ptrSSL_CTX == nullptr)
		return false;
	m_ptrSSL = SSL_new(m_ptrSSL_CTX);
	if(m_ptrSSL == nullptr)
	{
		LOGERR((char *)"SSL Socket: Error on Create SSL\n", 0, 0, 0);
		bResult = false;
	}
	else
	{
		bResult = true;
	}
	return bResult;
}

/**************************************************************************//**
* 
* \brief   - SSL binding to create a socket BIO between ssl and fd.
*
* \param   - TCP/IP socket connected descriptor 
*
* \return  - If there is any issue during the binding, it will return false
*
******************************************************************************/
bool SSLWrapper::BindSSL(int socketDescriptor)
{
	bool bResult = false;
	if(m_ptrSSL == nullptr)
		return false;
	if(SSL_set_fd(m_ptrSSL, socketDescriptor) == 0)
	{
		LOGERR((char*)"SSL Socket: Error on Bind SSL to Socket\n", 0, 0, 0);
		bResult = false;
	}
	else
	{
		bResult = true;
	}
	return bResult;
}

/**************************************************************************//**s
* 
* \brief   - SSL accept function to wait for a SSL client to initiate the SSL handshake.
*
* \param   - None
*
* \return  - If there is any issue during the accepting, it will return false
*
******************************************************************************/
bool SSLWrapper::AcceptSSL()
{
	bool bResult = false;
	if(m_ptrSSL == NULL)
		return false;
	if(SSL_accept(m_ptrSSL) > 0)
	{
		bResult = true;
	}
	else
	{
		LOGERR((char *)"SSL Socket: Error on ssl_socket accept", 0, 0, 0);
//		ERR_print_errors_fp(stderr);
		bResult = false;
	}
	return bResult;
}

/**************************************************************************//**
* 
* \brief   - To reset the SSL object to allow for another connection.s
*
* \param   - None
*
* \return  - If there is any issue during the accepting, it will return false
*
******************************************************************************/
bool SSLWrapper::ResetSSL()
{
	bool bResult = false;
	if(m_ptrSSL == NULL)
		return false;
	if(SSL_clear(m_ptrSSL) == 0)
	{
		LOGERR((char *)"SSL Socket: Error on ssl_socket clear", 0, 0, 0);
//		ERR_print_errors_fp(stderr);
		bResult = false;
	}
	else
		bResult = true;
	return bResult;
}

/**************************************************************************//**
* 
* \brief   - To send bufflen bytes from the buffer buff into the specified SSL connection.
*
* \param   - buffer and buffer length
*
* \return  - >0 is the write operation was successful, 
* 			 the return value is the number of bytes actually written to the SSL connection.
* 			 <=0 is the write operation was not successful.  
*
******************************************************************************/
int SSLWrapper::SendSSL(char *buff, const int len)
{
	if(m_ptrSSL == NULL)
		return ERROR;
	return SSL_write(m_ptrSSL, reinterpret_cast<void*>(buff), len);	
}

/**************************************************************************//**
* 
* \brief   - Try To read bufflen bytes from the specified SSL connection into the buffer buff.
*
* \param   - buffer and buffer length
*
* \return  - >0 is the read operation was successful, 
* 			 the return value is the number of bytes actually read from the SSL connection.
* 			 <=0 is the read operation was not successful.  
*
******************************************************************************/
int SSLWrapper::RecvSSL(char *buff, const int len)
{
	int iActualBufLen = 0, iTriedCount = 0;
	if(m_ptrSSL == nullptr)
		return ERROR;
	
	do
	{
		iActualBufLen = SSL_read(m_ptrSSL, reinterpret_cast<void*>(buff), len);
		if(iActualBufLen == 0)
		{
			iTriedCount++;
			if(iTriedCount > SSL_RETRY_ATTEMPT)
				iActualBufLen = ERROR;
		}
		
	}while(iActualBufLen == 0);
	
	return iActualBufLen;
}

/**************************************************************************//**
* 
* \brief   - Initialize the OpenSSL lib before using
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SSLWrapper::initOpenSSL()
{
	SSL_load_error_strings();
	OpenSSL_add_ssl_algorithms();
}

/**************************************************************************//**
* 
* \brief   - Cleanup the OpenSSL
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void SSLWrapper::cleanUpOpenSSL()
{
	EVP_cleanup();
}

/**************************************************************************//**
* 
* \brief   - Create the OpenSSL Context
*
* \param   - None
*
* \return  - SSL_CTX *
*
******************************************************************************/
SSL_CTX* SSLWrapper::createContext()
{
	const SSL_METHOD *ptrSSLMethod;
	SSL_CTX* ptrCTX;

	ptrSSLMethod = SSLv23_server_method();
	ptrCTX = SSL_CTX_new(ptrSSLMethod);
	if (!ptrCTX) 
	{
		LOGERR((char *)"SSLSocket: Unable to create SSL context", 0, 0, 0);
//		ERR_print_errors_fp(stderr);
//		exit(EXIT_FAILURE);
		ptrCTX = NULL;
	}
	return ptrCTX;
}

/**************************************************************************//**
* 
* \brief   - Configure the OpenSSL Context
*
* \param   - None
*
* \return  - SSL_CTX *
*
******************************************************************************/
bool SSLWrapper::configContext(SSL_CTX *ptrCTX)
{	
	bool bResult = false;
	SSL_CTX_set_ecdh_auto(ptrCTX, 1);
	
	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(ptrCTX, SSL_PEM_PATH, SSL_FILETYPE_PEM) <= 0)
	{
		LOGERR((char *)"SSLSocket: Unable to use Certificate file",0,0,0);
//		ERR_print_errors_fp(stderr);
//		exit(EXIT_FAILURE);
		return bResult;
	}

	if (SSL_CTX_use_PrivateKey_file(ptrCTX, SSL_PEM_PATH, SSL_FILETYPE_PEM) <= 0 )
	{
		LOGERR((char *)"SSLSocket: Unable to use PrivateKey file",0,0,0);
//		ERR_print_errors_fp(stderr);
//		exit(EXIT_FAILURE);
		return bResult;
	}
	bResult = true;
	return bResult;
}
