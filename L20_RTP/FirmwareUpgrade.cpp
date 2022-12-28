/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file is used as a software upgrade source file
 
**********************************************************************************************************/

#include "FirmwareUpgrade.h"
#include "CommunicationInterface.h"
#include "UserInterface.h"
#include "CommunicationInterfaceHMI.h"
#include "FirmwareUpgrdeUI.h"
#include <dirent.h>
#include <fcntl.h>
#include <usrFsLib.h>
extern "C"
{
	#include "vxbSp25QspiFlash.h"
	#include "timerDev.h"
}
FirmwareUpgrade::USB_PATH 	FirmwareUpgrade::CurrentPath 	= PRIMARY_PATH;
string 						FirmwareUpgrade::USBDirectory 	= "/bd0a/";
/******************************************************************************
 * \brief   - Initialization of class members and memory for object.
 *
 * \param   - None.
 *
 * \return  - None
 *
 ******************************************************************************/
FirmwareUpgrade::FirmwareUpgrade()
{
	UI_MSG_Q_ID = CP->getMsgQId(CommonProperty::cTaskName[CommonProperty::UI_T]);
}

/******************************************************************************
 * \brief   - De-Initialize allocated stack memory of EcSDOTask
 *
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
FirmwareUpgrade::~FirmwareUpgrade() 
{
	;
}

/******************************************************************************
 * \brief   - Check whether USB is connected or not
 * 
 * \param   - void
 *
 * \return  - int
 *
 ******************************************************************************/
int FirmwareUpgrade::USBDetect(void) 
{
	int fd = 0;
	int iResult = USB_DETECT_FAIL;
	switch(CurrentPath)
	{
	case PRIMARY_PATH:
		USBDirectory = "/bd0a/";
		/* open the USB */
		fd = open(USBDirectory.c_str(), RDONLY);
		/* check Whether it is connected or not */
		if (fd < 0)
		{
			CurrentPath = ADDITIONAL_PATH;
			iResult = USB_DETECT_FAIL;
		}
		else
		{
			CurrentPath = PRIMARY_PATH;
			iResult = USB_DETECT_SUCCESS;
		}
		close(fd); 
		break;
	case ADDITIONAL_PATH:
		USBDirectory = "/bd0/";
		/* open the USB */
		fd = open(USBDirectory.c_str(), RDONLY);
		/* check Whether it is connected or not */
		if (fd < 0)
		{
			CurrentPath = PRIMARY_PATH;
			iResult = USB_DETECT_FAIL;
		}
		else
		{
			CurrentPath = ADDITIONAL_PATH;
			iResult = USB_DETECT_SUCCESS;
		}
		close(fd); 
		break;
	default:
		CurrentPath = PRIMARY_PATH;
		iResult = USB_DETECT_FAIL;
		break;
	}
	return iResult;
}

/******************************************************************************
 * \brief   - Excute firmware upgrade task entrance
 * 
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::FirmwareUpgradeTaskHandler(void) 
{
	int iCheckSCres = 0;
	int iUpgradeTask = 0;

	iCheckSCres = checkFirmwareUpgradeFiles();
	if(iCheckSCres != 0)
	{
		// to response UI error message
		responseFirmwareUpgradeErrorMessage(iCheckSCres);
	}
	else
	{
		// excute software upgrge
		iUpgradeTask = ExcuteFirmwareUpgradeTask();
		if(iUpgradeTask != 0)
		{
			responseFirmwareUpgradeErrorMessage(iUpgradeTask);
		}
		
	}
}

/******************************************************************************
 * \brief   - Forward SC upgrade error information to userInterface
 * 
 * \param   - int iCherckErrorCode
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::responseFirmwareUpgradeErrorMessage(int iCherckErrorCode)
{
	MESSAGE	message;
	
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	memcpy(message.Buffer, &iCherckErrorCode, sizeof(iCherckErrorCode));
	message.msgID = UserInterface::TO_UI_TASK_FIRMWARE_UPGRADE_ERROR_MESSADE;
	SendToMsgQ(message, UI_MSG_Q_ID);
}

/******************************************************************************
 * \brief   - Verify that the SC file is correct
 * 
 * \param   - void
 *
 * \return  - int
 *
 ******************************************************************************/
int FirmwareUpgrade::checkFirmwareUpgradeFiles(void)
{
	int fd = 0;
	int iUSBreadStatus = 0;
	FILE *fp;
	int iFwFileSize = 0;
	string strSCfilePath;
	
	//1.Check whether the USB can be opened normally
	iUSBreadStatus = chdir(USBDirectory.c_str());
	if(iUSBreadStatus != 0)
	{
		LOGERR("FW_T : FAILED TO OPEN USB\n", 0, 0, 0);
		return FAILED_TO_OPEN_USB;
	}
	
	//2. read USB dir files
	getUpgradeSCfile();
	//3.check if vxworks file exists
	if (m_iSCupgradeFileSum == 0)
	{
		LOGERR("FW_T : UNABLE TO READ VXWORKS FILES FROM USB\n", 0, 0, 0);
		return FAILED_TO_READ_VXWORKS_FILES_FROM_USB;
	}

	//4. check if vxworks file open
	strSCfilePath = USBDirectory + m_strSCupgradeFileName;
	fp = fopen((char *) strSCfilePath.c_str(), READ_BINARY);
	if (fp == NULL) 
	{
		LOGERR("FW_T : UNABLE TO OPEN FILE %s\n",
				(_Vx_usr_arg_t )strSCfilePath.c_str(), 0, 0);
		return FAILED_TO_OPEN_VXWORKS_FILE;
	}
	
	//5. check vxworks size
	fseek(fp, ZERO_BYTES, SEEK_END);
	iFwFileSize = ftell(fp);

	fseek(fp, ZERO_BYTES, SEEK_SET);
	fclose(fp);
	if (iFwFileSize < SC_FINE_MIN_SIZE || iFwFileSize > SC_FINE_MAX_SIZE)
	{
		LOGERR("FW_T : FILE SIZE IS INCORRECT, SIZE = %d\n", iFwFileSize, 0, 0);
		return SC_FILE_SIZE_IS_INCORRECT;
	}
	else
	{
		m_iSCupgradefileSize = iFwFileSize;
	}
	
	LOG("FW_T: upgrade sc file check success, SC_NAME = %s, Version = %d, fileSize = %d\n",
			m_strSCupgradeFileName.c_str(), m_iSCupgradeVersion, m_iSCupgradefileSize);
	
	return 0;
}

/******************************************************************************
 * \brief   - Excute firmware upgrade task:
 * 			remove the old version from the QSPI and write the new version to the QSPI
 *
 * \param   - void
 *
 * \return  - int
 *
 ******************************************************************************/
int FirmwareUpgrade::ExcuteFirmwareUpgradeTask(void)
{
	int i;
	int iFwFileSize = 0;
	int iByteRead = 0;
	FILE  *fp;
	UINT8 *pBuf;
	UINT8 *pCheckBuf;
	string strSCfilePath = USBDirectory + m_strSCupgradeFileName;
	
	S25FL_Handle flashHandle;
	S25FL_Transaction flashTransaction;
    UINT32 blkAddr;
    UINT8  *wrData;
    UINT8  *rdData;
    UINT8  *origData;
    
	/* Delete tasks affecting SC upgrades */
	/* This is to ensure that no other tasks interrupt firmware upgrade task*/
	deleteTasks();

	/* Send the Response to UIC */
	progressResponseToUI(SC_PROGRESS_ONE);
	/* Get sc file size */
	fp = fopen((char *) strSCfilePath.c_str(), READ_BINARY);
	fseek(fp, ZERO_BYTES, SEEK_END);
	iFwFileSize = ftell(fp);
	fseek(fp, ZERO_BYTES, SEEK_SET);
	
	/* Check SC file size */
	if(iFwFileSize <= SC_FINE_MIN_SIZE && iFwFileSize >=SC_FINE_MAX_SIZE)
	{
		LOGERR("FW_T : SC FILE SIZE IS INCORRECT, SIZE = %d\n", iFwFileSize, 0, 0);
		return SC_FILE_SIZE_IS_INCORRECT;
	}
	
	/* Allocate memory */
	pBuf = (UINT8 *) malloc(iFwFileSize);
	pCheckBuf = (UINT8 *) malloc(iFwFileSize);
	if (pBuf == NULL || pCheckBuf == NULL)
	{
		fclose(fp);
		LOGERR("FW_T : pBuf MEMMORY ALLOCATION FAILED\n", 0, 0, 0);
		return MOLLOC_SC_BUFFER_FAILED;
	}
	
	/* Read file contents into pBuf */
	iByteRead = fread(pBuf, iFwFileSize, READ_BYTE, fp);

	if ((iByteRead == ZERO_BYTES) && (ferror(fp)))
	{
		LOGERR("FW_T : UANBLE TO READ SC FIRMWARE UPGRADATION FILE\n", 0, 0, 0);
		/* Free the allocated memory */
		if (pBuf != NULL)
		{
			free(pBuf);
			pBuf = NULL;
		}
		fclose(fp);
		return SC_FILE_SIZE_IS_INCORRECT;
	}

	fclose(fp);
	/* Send the Response to UIC */
	progressResponseToUI(SC_PROGRESS_TWO);
	
	/* Malloc memery */
    wrData = (UINT8 *)calloc(3, QSPI_FLASH_DATA_SIZE);
    if(wrData == NULL)
    {
    	LOGERR("FW_T : wrData MEMMORY ALLOCATION FAILED\n", 0, 0, 0);
        return MOLLOC_SC_BUFFER_FAILED;
    }
    rdData      = wrData + QSPI_FLASH_DATA_SIZE;
    origData    = rdData + QSPI_FLASH_DATA_SIZE;
    
    /* Open qspi*/
    flashHandle = vxbQspiLibSF25FLOpen();
    vxbQspiLibS25FLFlashQuadModeEnable(flashHandle);
    /* Read data from qspi */
    flashTransaction.address    = SC_QSPI_ADDR;
    flashTransaction.dataSize   = QSPI_FLASH_DATA_SIZE;
    flashTransaction.data = origData;
    vxbQspiLibSF25FLBufferRead(flashHandle, &flashTransaction);

    /* Send the Response to UIC */
	progressResponseToUI(SC_PROGRESS_THREE);
	
	/* Erase data */
    blkAddr = SC_QSPI_ADDR / QSPI_FLASH_DATA_BLK_SIZE;
    for(i = 0; i < QSPI_FLASH_DATA_BLK_CNT; i++)
    {
       vxbQspiLibS25FLFlashBlockErase(flashHandle, blkAddr + i);
    }

	/* Send the flash writing response to UIC */
	progressResponseToUI(SC_PROGRESS_FOUR);

	/* TODO: Write file image to QSPI flash */
	flashTransaction.data = pBuf;
	vxbQspiLibSF25FLBufferWrite(flashHandle, &flashTransaction);
#if(0)
	/* Check if SC file write successfully */
	flashTransaction.dataSize = iFwFileSize;
	flashTransaction.data = pCheckBuf;
	vxbQspiLibSF25FLBufferRead(flashHandle, &flashTransaction);
	if(memcmp(pBuf, pCheckBuf, iFwFileSize) != 0)
	{
		free(pCheckBuf);
		pCheckBuf = NULL;
    	LOGERR("FW_T : WRITE SC BINARY TO QSPI FAILED\n", 0, 0, 0);
        return WRITE_SC_BINARY_TO_QSPI_FAIL;
	}
#endif
	/* Send the Upgrade success response to UIC */
	progressResponseToUI(SC_PROGRESS_FIVE);
	taskDelay(10);
	
	/* Close qspi and free memery*/
    vxbQspiLibSF25FLClose(flashHandle);
    free(wrData);
    wrData = NULL;
    free(pBuf);
    pBuf = NULL;
	free(pCheckBuf);
	pCheckBuf = NULL;
    
	return 0;
	
}

/******************************************************************************
 * \brief   - Send the Progress to UIC
 *
 * \param   - UINT32 pResponseVal
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::progressResponseToUI(UINT32 uiResponseVal) 
{
	MESSAGE message;
	/* Send the Progress bar response to UIC */
	message.msgID = UserInterface::TO_UI_TASK_FIRMWARE_UPGRADE_PROGRESS_MESSADE;
	memset(message.Buffer, 0x00, sizeof(message.Buffer));
	memcpy(message.Buffer, &uiResponseVal, sizeof(uiResponseVal));
	SendToMsgQ(message, UI_MSG_Q_ID);
}

/******************************************************************************
 * \brief   - Delete all other tasks except SDO , Socket receiver and Firmware
 * 			  upgrade task 
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::deleteTasks(void)
{
	TASK_ID DB_TID, UI_DB_TID, ALARM_TID, BARCODE_TID, DGTIN_TID, DGTOUT_TID, ACTUATOR_TID, POWER_TID;
	
	sysAuxClkDisable();
	
	/* Get DB task ID */
	DB_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::DATA_T]);
	if (TASK_ID_NULL != DB_TID)
	{
		/* Delete DB task */
		if(!taskDelete(DB_TID))
		{
			DB_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DATA_T], DB_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete DB Task",0,0,0);
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get DB Task",0,0,0);
	}

	/* Get ALARM task ID */
	ALARM_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::ALARM_T]);
	if (TASK_ID_NULL != ALARM_TID) 
	{
		/* Delete ALARM task */
		if(!taskDelete(ALARM_TID))
		{
			ALARM_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::ALARM_T], ALARM_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete ALARM Task",0,0,0);
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get ALARM Task",0,0,0);
	}
	
	/* Get BARCODE READER task ID */
	BARCODE_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::BARCODE_READER_T]);
	if (TASK_ID_NULL != BARCODE_TID) 
	{
		/* Delete BARCODE READER task */
		if(!taskDelete(BARCODE_TID))
		{
			BARCODE_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::BARCODE_READER_T], BARCODE_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete BARCODE Task",0,0,0);
		}
		
	}
	else
	{
		LOGERR("FW_T:Fail to get BARCODE Task",0,0,0);
	}
	
	/* Get DIGITAL INPUT task ID */
	DGTIN_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::DGTIN_T]);
	if (TASK_ID_NULL != DGTIN_TID) 
	{
		/* Delete DIGITAL INPUT task */
		if(!taskDelete(DGTIN_TID))
		{
			DGTIN_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DGTIN_T], DGTIN_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete DIGITAL input Task",0,0,0);
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get DIGITAL input Task",0,0,0);
	}
	
	/* Get DIGITAL OUTPUT task ID */
	DGTOUT_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T]);
	if (TASK_ID_NULL != DGTOUT_TID) 
	{
	    /* Delete DIGITAL OUTPUT task */
		if(!taskDelete(DGTOUT_TID))
		{
			DGTOUT_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T], DGTOUT_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete DIGITAL output Task",0,0,0);
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get DIGITAL output Task",0,0,0);
	}
	
	/* Get ACTUATOR process task ID */
	ACTUATOR_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::ACTUATOR_PROCESS_T]);
	if (TASK_ID_NULL != ACTUATOR_TID) 
	{
	    /* Delete DIGITAL OUTPUT task */
		if(!taskDelete(ACTUATOR_TID))
		{
			ACTUATOR_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T], ACTUATOR_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete ACTUATOR process Task",0,0,0);
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get ACTUATOR process Task",0,0,0);
	}
	
	/* Get POWER supply task ID */
	POWER_TID = CP->getTaskId(CommonProperty::cTaskName[CommonProperty::POWER_SUPPLY_T]);
	if (TASK_ID_NULL != POWER_TID) 
	{
	    /* Delete DIGITAL OUTPUT task */
		if(!taskDelete(POWER_TID))
		{
			POWER_TID = TASK_ID_NULL;
			CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DGTOUT_T], POWER_TID);
		}
		else
		{
			LOGERR("FW_T:Fail to delete POWER supply Task",0,0,0);
		}
	}
	else
	{
		LOGERR("FW_T:Fail to get  POWER supply Task",0,0,0);
	}
}

/******************************************************************************
 * \brief   - Get all filenames from USB folder 
 *
 * \param   - vector<string>& vtFiles
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::getUSBdirFiles(vector<string>& vtFiles)
{
	struct dirent* entryDir;
	DIR* usbDir = opendir(USBDirectory.c_str());
	if(usbDir != NULL)
	{
		while( (entryDir=readdir(usbDir)) != NULL)
		{
			vtFiles.push_back(entryDir->d_name);
		}
	}
	closedir(usbDir);
}

/******************************************************************************
 * \brief   - Split a string according to specified rules 
 *
 * \param   - string strSplitOrigin
 * 			  string strPattern 
 * 			  vector<string>& strSplitOriginRes
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::splitString(string strSplitOrigin, string strPattern, vector<string>& strSplitOriginRes)
{
	int i = 0;
	string::size_type pos;
	strSplitOrigin += strPattern;
	int iStrSize = strSplitOrigin.size();
	
	for(int i=0; i<iStrSize; i++)
	{
		pos=strSplitOrigin.find(strPattern,i);
	    if(pos<iStrSize)
	    {
	      string strSplit = strSplitOrigin.substr(i,pos-i);
	      strSplitOriginRes.push_back(strSplit);
	      i = pos + strPattern.size() - 1;
	    }
	 }
}

/******************************************************************************
 * \brief   - Check if a string is all numbers
 *
 * \param   - string strCheck
 *
 * \return  - bool
 *
 ******************************************************************************/
bool FirmwareUpgrade::allIsNum(string strCheck)  
{  
	int i = 0;
	
    for (i = 0; i < strCheck.size(); i++)
    {
        int iTmp = (int)strCheck[i];
        if (iTmp >= 48 && iTmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    } 
    return true;
} 

/******************************************************************************
 * \brief   - Get SC file for firmware upgrade 
 *
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::getUpgradeSCfile(void)
{
	vector<string> vtAllFileNmaes;
	vector<string> vtSCfileNmaes;
	int iSCupgradeFileSum = 0;
	getUSBdirFiles(vtAllFileNmaes);
	
	//1. get all sc file names
	if(!vtAllFileNmaes.empty())
	{
		for(vector<string>::iterator iter = vtAllFileNmaes.begin(); iter != vtAllFileNmaes.end(); iter++)
		{
			string strFileNameTmp = *iter; 
			int strFileNameTmpLen = strFileNameTmp.size();
			if(strFileNameTmpLen >= SC_FILE_NAME_SUB_STR_LENGTH)
			{
				if(strFileNameTmp.substr(0, SC_FILE_NAME_SUB_STR_LENGTH) == string(SC_FILE_NAME_SUB_STR))
				{
					vtSCfileNmaes.push_back(strFileNameTmp);
				}
			}
		}
	}
	//2. get the latest SC file with version number
	if(!vtSCfileNmaes.empty())
	{
		// 2.1 update sc upgrade files
		for(vector<string>::iterator iter = vtSCfileNmaes.begin(); iter != vtSCfileNmaes.end(); iter++)
		{
			string strSCfileNameTmp =  *iter;
			vector<string> vtSplitStr;
			splitString(strSCfileNameTmp, "_", vtSplitStr);
			if(vtSplitStr.size() > SC_FILE_NAME_INDEX_FIVE && vtSplitStr[SC_FILE_NAME_INDEX_ONE] == string(SC_FILE_NAME_L20REFRESH) 
					&& allIsNum(vtSplitStr[SC_FILE_NAME_INDEX_TWO]) && allIsNum(vtSplitStr[SC_FILE_NAME_INDEX_THREE]) && allIsNum(vtSplitStr[SC_FILE_NAME_INDEX_FOUR]))
			{
				vector<string> vtSplitSuffix;
				splitString(vtSplitStr[SC_FILE_NAME_INDEX_FIVE], ".", vtSplitSuffix);
				if(vtSplitSuffix.size() > SC_FILE_NAME_INDEX_ONE && allIsNum(vtSplitSuffix[SC_FILE_NAME_INDEX_ZERO]) 
					&& vtSplitSuffix[SC_FILE_NAME_INDEX_ONE] == string(SC_FILE_NAME_SUFFIX))
				{
					iSCupgradeFileSum++;
					string strSCversion = vtSplitStr[SC_FILE_NAME_INDEX_TWO] + vtSplitStr[SC_FILE_NAME_INDEX_THREE] 
											+ vtSplitStr[SC_FILE_NAME_INDEX_FOUR] + vtSplitSuffix[SC_FILE_NAME_INDEX_ZERO];
					int iSCversion = atoi(strSCversion.c_str());
					if(iSCversion >= m_iSCupgradeVersion)
					{
						m_iSCupgradeVersion = iSCversion;
						m_strSCupgradeFileName = strSCfileNameTmp;
					}
				}
			}
		}
		
		// 2.2 updage sc file number
		m_iSCupgradeFileSum = iSCupgradeFileSum;
	}
}

/******************************************************************************
 * \brief   - Rewrite ProcessTaskMessage function
 * 
 * \param   - MESSAGE
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::ProcessTaskMessage(MESSAGE & message)
{
	;
}

/**************************************************************************//**
 * \brief   - Task Function Routine called when task spawned
 *
 * \param   - void
 *
 * \return  - void
 *
 ******************************************************************************/
void FirmwareUpgrade::FW_Upgrade_Task(void)
{
	/* Call the message handler function to process the Message received from UIC */
	FirmwareUpgrade *FwUpgradeObj = new FirmwareUpgrade();
	FwUpgradeObj->FirmwareUpgradeTaskHandler();
}
