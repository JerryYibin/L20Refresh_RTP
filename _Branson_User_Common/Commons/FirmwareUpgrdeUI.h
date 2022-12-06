/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

---------------------------- MODULE DESCRIPTION ----------------------------

This file is to define all the commands that is used to response
the firmware upgrade message data to UI .

***************************************************************************/

#ifndef FIRMWAREUPGRADEUI_H_
#define FIRMWAREUPGRADEUI_H_

enum USBdetectStatus
{
	USB_DETECT_FAIL = 0,
	USB_DETECT_SUCCESS,
	STSTUS_DUMMY
};

enum FirmwareUpgradeErrorCode
{
	FAILED_TO_OPEN_USB = -7,                               
	FAILED_TO_OPEN_FIRMWARE_FILE,                         
	FAILED_TO_READ_VXWORKS_FILES_FROM_USB,                
	FAILED_TO_OPEN_VXWORKS_FILE,                         
	SC_FILE_SIZE_IS_INCORRECT,
	MOLLOC_SC_BUFFER_FAILED,
	WRITE_SC_BINARY_TO_QSPI_FAIL,
	ERROR_DUMMY
};

enum SCProgressResponseToUI
{
	SC_PROGRESS_ONE = 21,
	SC_PROGRESS_TWO,
	SC_PROGRESS_THREE,
	SC_PROGRESS_FOUR,
	SC_PROGRESS_FIVE,
	PROGESS_DUMMY
};

#endif
