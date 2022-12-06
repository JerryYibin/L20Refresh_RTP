/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2022
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     This file is used as a software upgrade header file
 
**********************************************************************************************************/
#ifndef FIRMWAREUPGRADE_H_
#define FIRMWAREUPGRADE_H_

#include <string>
#include "SCTask.h"

#define   USB_PATH            		  "/bd0a/"
#define   RDONLY                      0x0000
#define   RDWR_CREAT         		  "w+"
//#define   SC_FILE_NAME_FORMAT         "SC_*.bin"
#define   READ_BINARY        		  "rb"
#define   ZERO_BYTES                   0
#define   SEEK_END                     2
#define   SEEK_SET                     0
#define	  READ_BYTE           		   1
#define   SC_QSPI_ADDR                 0x1E0000
#define   SC_FILE_NAME_SUB_STR         "SC"
#define   SC_FILE_NAME_SUB_STR_LENGTH  2
#define   SC_FINE_MIN_SIZE             0xA00000     /* minium size of SC file: 10M */
#define   SC_FINE_MAX_SIZE             0x1E00000	/* maxium size of SC file: 30M */
#define   SC_FILE_NAME_L20REFRESH      "L20Refresh"
#define   SC_FILE_NAME_SUFFIX          "bin"
#define   QSPI_FLASH_DATA_BLK_SIZE S25FL_FLASH_BLOCK_SIZE
#define   QSPI_FLASH_DATA_BLK_CNT 480
#define   QSPI_FLASH_DATA_SIZE (QSPI_FLASH_DATA_BLK_SIZE*QSPI_FLASH_DATA_BLK_CNT)

typedef enum
{
	SC_FILE_NAME_INDEX_ZERO,
	SC_FILE_NAME_INDEX_ONE,
	SC_FILE_NAME_INDEX_TWO,
	SC_FILE_NAME_INDEX_THREE,
	SC_FILE_NAME_INDEX_FOUR,
	SC_FILE_NAME_INDEX_FIVE
}SCnameIndex;

class FirmwareUpgrade : public SCTask 
{
public:
	FirmwareUpgrade();
	~FirmwareUpgrade();
	static int       USBDetect                            (void);
	void             FirmwareUpgradeTaskHandler           (void);
	int              ExcuteFirmwareUpgradeTask            (void);
	static void	     FW_Upgrade_Task		              (void);
	virtual	void     ProcessTaskMessage	                  (MESSAGE& message) override;
private:
	void             responseFirmwareUpgradeErrorMessage  (int iCherckErrorCode);
	void             progressResponseToUI                 (UINT32 uiResponseVal);
	void             getUpgradeSCfile                     (void);
	void             deleteTasks                          (void);
	int              checkFirmwareUpgradeFiles            (void);
	void             getUSBdirFiles                       (vector<string>& vtFiles);
	void 			 splitString                          (string strSplitOrigin, string strPattern, vector<string>& strSplitOriginRes);
	bool 			 allIsNum                             (string strCheck);
private:
	MSG_Q_ID                 UI_MSG_Q_ID;
	int                      m_iSCupgradeFileSum = 0;
	int                      m_iSCupgradeVersion = 0;   /* upgrade SC version   */
	int                      m_iSCupgradefileSize = 0;  /* upgrade SC size      */
	string                   m_strSCupgradeFileName;    /* upgrade SC file name */
	
};
#endif
