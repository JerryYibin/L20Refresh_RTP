/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef MAINTASK_H_
#define MAINTASK_H_
#include "commons.h"
#include "SCTask.h"

/* Message queue OFFSETS */
#define MAX_MSG					1000 
#define MAX_MSG_LEN				1024 
#define MSG_Q_FIFO				0x00

/* No of task going to create in Supervisory Controller */
#define NUM_OF_BL_TASK			14
#define SHUTDOWN_STATUS			1

/* Priority of tasks in Supervisory Controller */

enum TASK_PRIORITY
{
	CONSOLE_T_PRIORITY			= 10,
	CTRL_T_PRIORITY				= 220,
	ACT_SOCKET_T_PRIORITY,
	ACT_PROCESS_T_PRIORITY,
	PS_T_PRIORITY 				= 222,
	ACT_SYSTEM_T_PRIORITY,
	UI_T_PRIORITY,
	DATA_T_PRIORITY,
	FW_UPGRADE_T_PRIORITY       = 225,
	INTERFACE_T_PRIORITY,
	HMI_SOCKET_T_PRIORITY,
	CAN_SOCKET_T_PRIORITY,
	ALARM_T_PRIORITY,
	DGTIN_T_PRIORITY,
	DGTOUT_T_PRIORITY,
//	WEBSERVICE_T_PRIORITY,
	DIG_SOCKET_T_PRIORITY,
	BARCODE_READER_T_PRIORITY,
	MAINTENANCE_T_PRIORITY
};

/* Stack size of tasks in Supervisory Controller */
#define CONSOLE_T_STACK_SIZE			(1024 * 6)
#define CTRL_T_STACK_SIZE				(1024 * 6)
#define ACT_PROCESS_T_STACK_SIZE		(1024 * 6)
#define PS_T_STACK_SIZE					(1024 * 6)
#define ACT_SYSTEM_T_STACK_SIZE			(1024 * 6)
#define SDO_T_STACK_SIZE				(1024 * 6)
#define ALARM_T_STACK_SIZE				(1024 * 6)
#define UI_T_STACK_SIZE					(1024 * 10)
#define INTERFACE_T_STACK_SIZE			(1024 * 20)
#define DATA_T_STACK_SIZE				(1024 * 20)
#define HMI_SOCKET_T_STACK_SIZE			(1024 * 10)
#define CAN_SOCKET_T_STACK_SIZE			(1024 * 4)
#define ACT_SOCKET_T_STACK_SIZE			(1024 * 8)
#define DIG_SOCKET_T_STACK_SIZE			(1024 * 10)
#define FW_UPGRADE_T_STACK_SIZE			(1024 * 8)
#define DGTIN_T_STACK_SIZE				(1024 * 4)
#define DGTOUT_T_STACK_SIZE				(1024 * 4)
#define BARCODE_READER_T_STACK_SIZE		(1024 * 2)
#define MAINTENANCE_T_STACK_SIZE		(1024 * 8)
#define GATEWAY_SOCKET_T_STACK_SIZE		(1024 * 8)

/* Shared data object size for PDO data */
#define SD_SIZE					512

/* Shared data public object name */
#define RX_DATA_PC				"/RxPDO_PC"
#define RX_DATA_AC				"/RxPDO_AC"
#define TX_DATA_PC				"/TxPDO_PC"
#define TX_DATA_AC				"/TxPDO_AC"

/* Global structure object for PDO DATA */
extern TxPDO_AC *AC_TX;
extern RxPDO_AC *AC_RX;

/* List of business logic tasks function routine declaration*/

void Socket_ACT_Task		(void);
//void Socket_DIG_Task		(void);
void ConsoleApp_Task		(void);
void FirmwareUpgrade_Task	(void);

void Alarm_Task				(void);
void BarcodeReader_Task		(void);
void Maintenance_Task		(void);
void Actuator_Process_Task	(void);

void ScDgtOutput_Task 		(void);
void ScDgtInput_Task		(void);
void Actuator_System_Task	(void);

//void Socket_Gateway_Task	(void);

/* FUNCPTR for available business logic tasks */
typedef void FUNC(void);
FUNC /*Control_Task, UserInterface_Task, Data_Task, DataInterface_Task,*/ Alarm_task,FirmwareUpgrade_Task,ScDgtInput_Task,ScDgtOutput_Task,BarcodeReader_Task/*, Socket_Gateway_Task */;

class MainTask : public SCTask
{
public:
	MainTask();
	~MainTask();

	bool 			CreateSD				();
	bool			CreateMsgQ				();
	bool			CreateTasks				();

	void			ReleaseMemory			();
	bool			DestroyAllMsgQ			();
	void			TerminateTasks			();
	void			SetTaskId				(string T_Name, TASK_ID TID);
	void 			DeleteTasks				();
		
	void 			CleanUp					();
	void 			InitAuxClk				();
protected:
	virtual void 	ProcessTaskMessage		(MESSAGE& message){};
private:
	struct TaskInfo
	{
		string		TaskName;
		UINT32 		Priority;
		FUNC*   	TaskFunc;
		UINT32 		StackSize;	
	};
	TaskInfo	taskinfo[NUM_OF_BL_TASK];
};

#endif /* MAINTASK_H_ */
