/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include "EEPROM.h"
#include "SystemConfiguration.h"
#include "QuickCRC32Algorithm.h"
#include "Logger.h"
extern "C"
{
	#include "customSystemCall.h"
}
EEPROM::SYSTEM_CONFIG* 	EEPROM::_System 		= nullptr;
EEPROM* 				EEPROM::_EepromObj 		= nullptr;
string 					EEPROM::EEPROM_DEV_NAME = "/eeprom/0";
/**************************************************************************//**
* \brief   - Constructor - Initialize _System data structure
*
* \param   - None.
*
* \return  - None
*
******************************************************************************/
EEPROM::EEPROM() {
	init();

}

/**************************************************************************//**
* 
* \brief   - Destructor. Release _System
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
EEPROM::~EEPROM() {
	delete _System;
	_System = nullptr;
}

EEPROM * EEPROM::GetInstance()
{
	return (_EepromObj != nullptr) ? _EepromObj : (_EepromObj = new EEPROM());
}

/**************************************************************************//**
* 
* \brief   - Read. Read _System information from Eeprom. If there is any error, 
* 			 the initial data will be write to Eeprom
*
* \param   - None.
*
* \return  - None.
*
******************************************************************************/
int EEPROM::Read()
{
	unsigned int crcCode = 0;
    if(eepromIoRead(EEPROM_DEV_NAME.c_str(), EEPROM_DATA_START_ADDR, sizeof(SYSTEM_CONFIG), (void *)_System) != OK)
	{
        LOGERR((char*)"EEPROM Read: -------- Failed to read original data -----------\n", 0, 0, 0);
        return ERROR;
	}
    LOG("_System->Power: %d\n", _System->Power);
    LOG("_System->Frequency: %d\n", _System->Frequency);
    LOG("_System->Type: %d\n", _System->Type);
    crcCode = QuickCRC32Algorithm::crc32_ccitt(_System, sizeof(SYSTEM_CONFIG) - sizeof(int));
    if(crcCode != _System->CRC)
    {
    	init();
    	eepromIoWrite(EEPROM_DEV_NAME.c_str(), EEPROM_DATA_START_ADDR, sizeof(SYSTEM_CONFIG), (void *)_System);
    	LOGERR((char*)"EEPROM Read: -------- CRC ERROR -----------\n", 0, 0, 0);
    	return ERROR;
    }
    
	return OK;
}

int EEPROM::Write()
{
	_System->CRC = QuickCRC32Algorithm::crc32_ccitt(_System, sizeof(SYSTEM_CONFIG) - sizeof(int));
	if(eepromIoWrite(EEPROM_DEV_NAME.c_str(), EEPROM_DATA_START_ADDR, sizeof(SYSTEM_CONFIG), (void *)_System) != OK) 
	{
		LOGERR((char*)"EEPROM Write: -------- Failed to Write new data -----------\n", 0, 0, 0);
		return ERROR;
	}
	return ERROR;
}

/**************************************************************************//**
* \brief   - int - Initialize _System
* 
* \param   - None
* 
* \return  - None
*
******************************************************************************/
void EEPROM::init()
{
	_System = new SYSTEM_CONFIG();
	_System->Type = L20;
	_System->Frequency = SYSTEMCONFIG::FREQ_20KHZ;
	_System->Power = SYSTEMCONFIG::POWER_5500W;
	memcpy(_System->SeriesNumber, "FFFFFFFFFFFF", SER_LEN);
	_System->TunePoint = 0;
	_System->FrequencyOffset = 0;
	_System->CRC = QuickCRC32Algorithm::crc32_ccitt(_System, sizeof(SYSTEM_CONFIG) - sizeof(int));
}

/**************************************************************************//**
* \brief   - eepromIoRead - read EEPROM
* 
* \param   - The function reads 'length' bytes into 'pRdBuf' from 'devName', starting from 'startAddr'.
* 
* \return  - OK or ERROR
*
******************************************************************************/
int EEPROM::eepromIoRead(const char *devName, unsigned int startAddr, unsigned int length, void *pRdBuf)
{
    int fd;
    unsigned int curAddr;

    /* open the EEPROM */
    if((fd = open(devName, O_RDONLY, 0)) == ERROR)
	{
    	LOGERR((char*)"EEPROM Read: -------- Open Error -----------\n", 0, 0, 0);
        return ERROR;
	}

    /* redirection */
    if(ioctl(fd, FIOSEEK, startAddr) == ERROR)
	{
        LOGERR((char*)"EEPROM Read: -------- FIOSEEK Error -----------\n", 0, 0, 0);
        close(fd);
        return ERROR;
	}

    /*get current position before read*/
    curAddr = ioctl(fd, FIOWHERE, 0);
//    LOG("location before read: 0x%x\n", curAddr);

    /* read the data */
    if(read(fd, pRdBuf, length) == ERROR)
	{
        LOGERR((char*)"EEPROM Read: -------- READ Error -----------\n", 0, 0, 0);
        close(fd);
        return ERROR;
	}

    curAddr = ioctl(fd, FIOWHERE, 0);
//    LOG("location after read: 0x%x\n", curAddr);

    close(fd);
    return OK;
}

/*******************************************************************************
* \brief   - eepromIoWrite - write EEPROM
*
* \param   - The function writes 'length' bytes, saved in 'pWrBuf', into 'devName', starting from 'startAddr'.
*
* \return  - OK or ERROR
*
******************************************************************************/
int EEPROM::eepromIoWrite(const char *devName, const UINT32 startAddr, const UINT32 length, const void *pWrBuf)
{
    int fd;
    UINT32 curAddr;

    /* open the EEPROM */
    if((fd = open(devName, O_WRONLY, 0)) == ERROR)
	{
        LOGERR((char*)"EEPROM Write: -------- Open Error -----------\n", 0, 0, 0);
        return ERROR;
	}

    /* redirection */
    if(ioctl(fd, FIOSEEK, startAddr) == ERROR)
	{
        LOGERR((char*)"EEPROM Write: -------- FIOSEEK Error -----------\n", 0, 0, 0);
        close(fd);
        return ERROR;
	}

    curAddr = ioctl(fd, FIOWHERE, 0);
//    LOG("location before write: 0x%x\n", curAddr);

    /* write data */
    if(write(fd, pWrBuf, length) == ERROR)
	{
        LOGERR((char*)"EEPROM Write: -------- WRITE Error -----------\n", 0, 0, 0);
        close(fd);
        return ERROR;
	}

    /*get current position after write*/
    curAddr = ioctl(fd, FIOWHERE, 0);
//    LOG("location after write: 0x%x\n", curAddr);

    close(fd);
    return OK;
}

