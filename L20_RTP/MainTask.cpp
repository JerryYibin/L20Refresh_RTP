/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#include <stdio.h>
#include "MainTask.h"
#include "ControlTask.h"
#include "UserInterface.h"
#include "DataTask.h"
#include "DataInterface.h"
#include "SocketReceiverHMI.h"
#include "AuxClockTask.h"
#include "GPIO.h"
extern "C"
{
	#include "timerDev.h"	
	#include "customSystemCall.h"
}
using namespace std;
/**************************************************************************//**
* \brief   - Initialize the task info structure
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
MainTask::MainTask()
{
	UINT32 t_index = 0;
	char* taskName[TOTAL_NUM_OF_TASK] = {
										(char *) "/Ctrl_Task", 
										(char *) "Actuator_Process_Task",
										(char *) "DSP_Task", 
										(char *) "Actuator_System_Task", 
										(char *) "UI_Task", 
										(char *) "Data_Task", 
										(char *) "Data_Interface_Task",
										(char *) "Alarm_Task", 
										(char *) "BarcodeReader_Task", 
										(char *) "ScDgtInput_Task",
										(char *) "ScDgtOutput_Task",
										(char *) "Maintenance_Task", 
										(char *) "ConsoleApp_Task",
										(char *) "RtpMain_Task", 
										(char *) "HMI_Socket_Task", 
										(char *) "ACT_Socket_Task", 
										(char *) "DIG_Socket_Task",
										(char *) "FW_Upgrade_Task"};
	
	for(t_index = 0; t_index < TOTAL_NUM_OF_TASK; t_index++)
	{
		CommonProperty::cTaskName[t_index] = taskName[t_index];
	}
	
	/* Enabling the flag allows business logic tasks to run continuously   */
	CP->m_bTaskRun = true;
	
	UINT32	g_Priority[NUM_OF_BL_TASK] 			= {CTRL_T_PRIORITY, ACT_PROCESS_T_PRIORITY, DSP_T_PRIORITY, ACT_SYSTEM_T_PRIORITY, UI_T_PRIORITY, DATA_T_PRIORITY, 
													INTERFACE_T_PRIORITY, ALARM_T_PRIORITY, BARCODE_READER_T_PRIORITY, DGTIN_T_PRIORITY, DGTOUT_T_PRIORITY,
													MAINTENANCE_T_PRIORITY};
	
	FUNC*	g_TaskFunc[NUM_OF_BL_TASK] 			= {ControlTask::Control_Task, Actuator_Process_Task, DSP_Task, Actuator_System_Task, UserInterface::UserInterface_Task, DataTask::Data_Task, DataInterface::DataInterface_Task, Alarm_Task,
													BarcodeReader_Task, ScDgtInput_Task, ScDgtOutput_Task, Maintenance_Task};
	
	UINT32	g_StackSize[NUM_OF_BL_TASK]			= {CTRL_T_STACK_SIZE, ACT_PROCESS_T_STACK_SIZE, DSP_T_STACK_SIZE, ACT_SYSTEM_T_STACK_SIZE, UI_T_STACK_SIZE,
													DATA_T_STACK_SIZE, INTERFACE_T_STACK_SIZE, ALARM_T_STACK_SIZE, BARCODE_READER_T_STACK_SIZE, DGTIN_T_STACK_SIZE, DGTOUT_T_STACK_SIZE,
													MAINTENANCE_T_STACK_SIZE};
	
	for(t_index=0; t_index < NUM_OF_BL_TASK; t_index++)
	{			
		taskinfo[t_index].TaskName 		= CommonProperty::cTaskName[t_index];
		taskinfo[t_index].Priority 		= g_Priority[t_index];
		taskinfo[t_index].TaskFunc	 	= g_TaskFunc[t_index];
		taskinfo[t_index].StackSize 	= g_StackSize[t_index];		
	}
}

/**************************************************************************//**
* \brief   - Creates the message queue for all the task spawned
*
* \param   - None
*
* \return  - bool success
*
******************************************************************************/
bool MainTask::CreateMsgQ()
{
	bool bSuccess = false;
	MSG_Q_ID qID = MSG_Q_ID_NULL, dbqID[3];
	UINT8 q_index;
	
	for(UINT32 t_index = 0; t_index < NUM_OF_BL_TASK; t_index++)
	{

		if(t_index == CommonProperty::DATA_T)
		{
			// multiple queues for this task
			dbqID[0] = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
			dbqID[1] = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
			dbqID[2] = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
			
			if(MSG_Q_ID_NULL != dbqID[0] && MSG_Q_ID_NULL != dbqID[1] && MSG_Q_ID_NULL != dbqID[2])
			{
				CP->setMsgQId(taskinfo[t_index].TaskName + "/Control", 	dbqID[0]);
				CP->setMsgQId(taskinfo[t_index].TaskName + "/Data", 	dbqID[1]);
				CP->setMsgQId(taskinfo[t_index].TaskName + "/Request",	dbqID[2]);
				bSuccess = true;
			}
			else
				bSuccess = false;
			
			continue;
		}
		else
		{
			// the rest of the tasks
			qID = msgQCreate(MAX_MSG, MAX_MSG_LEN, MSG_Q_FIFO);
		}
		
		// register queue ID
		if(MSG_Q_ID_NULL != qID)
		{
			CP->setMsgQId(taskinfo[t_index].TaskName, qID);
			bSuccess = true;
		}
		else
		{
			bSuccess = false;
		}
	}
	return bSuccess;
}

/**************************************************************************//**
* \brief   - Spawns the required tasks
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool MainTask::CreateTasks()
{
	bool bSuccess = true;
	TASK_ID tID;
	
	tID = taskSpawn((char *)CommonProperty::cTaskName[CommonProperty::HMI_SOCKET_T], HMI_SOCKET_T_PRIORITY, VX_FP_TASK, HMI_SOCKET_T_STACK_SIZE, (FUNCPTR)SocketReceiver_HMI::Socket_HMI_Task, 0,0,0,0,0,0,0,0,0,0);
	if (tID == TASK_ID_ERROR)
		bSuccess = false;
	else
		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::HMI_SOCKET_T], tID);


//	tID = taskSpawn((char *)CommonProperty::cTaskName[CommonProperty::DIG_SOCKET_T], DIG_SOCKET_T_PRIORITY, VX_FP_TASK, DIG_SOCKET_T_STACK_SIZE, (FUNCPTR)SocketSenderDIG::Socket_DIG_Task,0,0,0,0,0,0,0,0,0,0);
//	if (tID == TASK_ID_ERROR)
//		bSuccess = false;
//	else
//		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DIG_SOCKET_T], tID);

//	tID = taskSpawn((char *)CommonProperty::cTaskName[ACT_SOCKET_T], ACT_SOCKET_T_PRIORITY, VX_FP_TASK, ACT_SOCKET_T_STACK_SIZE, (FUNCPTR)Socket_ACT_Task,0,0,0,0,0,0,0,0,0,0);
//	if (tID == TASK_ID_ERROR)
//		bSuccess = false;
//	else
//		CP->SetTaskId(CommonProperty::cTaskName[ACT_SOCKET_T], tID);
	
	tID = taskOpen((char *)taskinfo[CommonProperty::CTRL_T].TaskName.c_str(),taskinfo[CommonProperty::CTRL_T].Priority,VX_FP_TASK,OM_CREATE,NULL,taskinfo[CommonProperty::CTRL_T].StackSize,0,(FUNCPTR)taskinfo[CommonProperty::CTRL_T].TaskFunc,0,0,0,0,0,0,0,0,0,0);
	if (tID == TASK_ID_ERROR)
		bSuccess = false;
	else
	{
		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::CTRL_T], tID);
		LOG("\nMain_T : CTRL_T task Id: %d\n",tID);
	}
	
	//TO_BE_DONE - Added ENABLE_DIG_CLIENT #define so that 
	//other developers using this stream can enable/disable
	//the DIG  client functionality easily. This #define will 
	//be removed once the DIG client implementation is complete.
	//For now by default, DIG client functionality is disabled.
	
//	tID = taskSpawn((char *)CommonProperty::cTaskName[CommonProperty::DIG_SOCKET_T], DIG_SOCKET_T_PRIORITY, VX_FP_TASK, DIG_SOCKET_T_STACK_SIZE, (FUNCPTR)Socket_DIG_Task, 0,0,0,0,0,0,0,0,0,0);
//	if (tID == TASK_ID_ERROR)
//		bSuccess = false;
//	else
//		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DIG_SOCKET_T], tID);
//	tID = taskSpawn((char *)CommonProperty::cTaskName[ACTUATOR_PROCESS_T], ACT_PROCESS_T_PRIORITY, VX_FP_TASK,ACT_PROCESS_T_STACK_SIZE, (FUNCPTR)Actuator_Process_Task,0,0,0,0,0,0,0,0,0,0);
//	if (tID == TASK_ID_ERROR)
//		bSuccess = false;
//	else
//		CP->SetTaskId(CommonProperty::cTaskName[ACTUATOR_PROCESS_T], tID);
	
//	tID = taskSpawn((char *)CommonProperty::cTaskName[ACTUATOR_SYSTEM_T], ACT_SYSTEM_T_PRIORITY, VX_FP_TASK,ACT_SYSTEM_T_STACK_SIZE, (FUNCPTR)Actuator_System_Task,0,0,0,0,0,0,0,0,0,0);
//	if (tID == TASK_ID_ERROR)
//		bSuccess = false;
//	else
//		CP->SetTaskId(CommonProperty::cTaskName[ACTUATOR_SYSTEM_T], tID);
	
	tID = taskSpawn((char *)CommonProperty::cTaskName[CommonProperty::UI_T], UI_T_PRIORITY, VX_FP_TASK,UI_T_STACK_SIZE, (FUNCPTR)UserInterface::UserInterface_Task, 0,0,0,0,0,0,0,0,0,0);
	if (tID == TASK_ID_ERROR)
		bSuccess = false;
	else
		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::UI_T], tID);
	
	tID = taskSpawn((char*)CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T], INTERFACE_T_PRIORITY, VX_FP_TASK, INTERFACE_T_STACK_SIZE, (FUNCPTR)DataInterface::DataInterface_Task, 0,0,0,0,0,0,0,0,0,0);
	if (tID == TASK_ID_ERROR)
		bSuccess = false;
	else
		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DATA_INTERFACE_T], tID);
	
	tID = taskSpawn((char*)CommonProperty::cTaskName[CommonProperty::DATA_T], DATA_T_PRIORITY, VX_FP_TASK, DATA_T_STACK_SIZE, (FUNCPTR)DataTask::Data_Task, 0,0,0,0,0,0,0,0,0,0);
	if (tID == TASK_ID_ERROR)
		bSuccess = false;
	else
		CP->setTaskId(CommonProperty::cTaskName[CommonProperty::DATA_T], tID);
	
//	for(UINT32 t_index=0; t_index < NUM_OF_BL_TASK; t_index++)
//	{
//		if(t_index == CTRL_T)
//		{ 
//			tid[t_index] = taskOpen((char *)taskinfo[t_index].TaskName.c_str(),taskinfo[t_index].Priority,VX_FP_TASK,OM_CREATE,NULL,taskinfo[t_index].StackSize,0,(FUNCPTR)taskinfo[t_index].TaskFunc,0,0,0,0,0,0,0,0,0,0);
//		}
//		else
//		{
//			tid[t_index] = taskSpawn((char *)taskinfo[t_index].TaskName.c_str(),taskinfo[t_index].Priority,VX_FP_TASK,taskinfo[t_index].StackSize,(FUNCPTR)taskinfo[t_index].TaskFunc,0,0,0,0,0,0,0,0,0,0);
//		}
//		
//		if(TASK_ID_NULL != tid[t_index] && TASK_ID_ERROR != tid[t_index])
//		{
//			CP->SetTaskId(taskinfo[t_index].TaskName,tid[t_index]);
//		}
//		else
//		{
//			bIsSuccess = false;
//		}
//	}

	return bSuccess;
}

/**************************************************************************//**
* \brief   - Release memory and exits from spawned tasks
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::TerminateTasks()
{
	/* Disabling the flag allows business logic tasks to exit from continuous loop */
	CP->m_bTaskRun = false;
}

/**************************************************************************//**
* \brief   - Delete the created message queue.
*
* \param   - None
*
* \return  - bool
*
******************************************************************************/
bool MainTask::DestroyAllMsgQ()
{
	for(INT32 t_index = 0; t_index < NUM_OF_BL_TASK; t_index++)
	{
		if(msgQDelete(CP->getMsgQId(taskinfo[t_index].TaskName)) != OK)
		{
			LOGERR((char *) "MainTask::DestroyAllMsgQ: MessageQ deletion failed", 0, 0, 0);
		}
	}		
	return true;
}

/**************************************************************************//**
* \brief   - De-initialize the allocated stack memory for CommonProperty. 
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::ReleaseMemory()
{
	CommonProperty::deleteInstance();
}

/**************************************************************************//**
* \brief   - Assign Console_Task id with DS. 
*
* \param   - string Task name, TASK_ID TID
*
* \return  - None
*
******************************************************************************/
void MainTask::SetTaskId(string T_Name, TASK_ID TID)
{
	CP->setTaskId(T_Name, TID);
}

/**************************************************************************//**
* \brief   - Clean up unwanted data available in secondary storage
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
void MainTask::CleanUp()
{
	
}

/**************************************************************************//**
* \brief   - Deletes all tasks
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
 
void MainTask::DeleteTasks()
{
	UINT8 taskIndex=0;
	
	for(taskIndex = 0; taskIndex < (TOTAL_NUM_OF_TASK - 1); taskIndex++)
	{
		taskDelete(CP->getTaskId(CommonProperty::cTaskName[taskIndex]));
	}
	
}

/**************************************************************************//**
* \brief   - De-initialize the allocated stack memory for MainTask Class
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
MainTask::~MainTask()
{
	#ifdef DEBUG
		/*	need to decide on calling DestroyTask function from here.	*/
		cout << "-----------Main task destructor is called------------" << endl;
	#endif
	CP = NULL;
}

/**************************************************************************//**
* \brief   - Process Begin
*
* \param   - None
*
* \return  - None
*
******************************************************************************/
int main()
{
	UINT32 iEvent 		= 0;
	bool bDataStruct  	= true;
	TASK_ID tid			= taskIdSelf();
	GPIO::InitGPIO();
	MainTask *MT 		= new(nothrow) MainTask();
	if(NULL != MT)
	{
		MT->SetTaskId(CommonProperty::cTaskName[CommonProperty::MAIN_T], tid);
		
		bDataStruct = MT->CreateMsgQ();

		MT->CleanUp();

		if(bDataStruct)
		{
			LOG("\nMAIN_T: Create MsgQ: OK\n");
			bDataStruct = MT->CreateTasks();
		}
		
		MT->InitAuxClk();
		
#ifdef SHUTDOWN_EVENT_REGISTER
		
		/*Calling this custom system call to register the shutdown event*/
//		status=Gpio24VLowShutdown();
//		
//		if(SHUTDOWN_STATUS != status)
//		{
//			LOGERR("Main_T : Error in Registering Shutdown Event",0,0,0);
//		}
		
#endif
		
//		SendMsg.msgID = TO_DATA_TASK_ALARM_CONFIG_RW_REQ;		
//		memset(SendMsg.Buffer, 0x00, sizeof(SendMsg.Buffer));	
//		MT->PostMessageToTask(SendMsg);
		LOG("System Clock Value = %d\n", sysClkRateGet());
		if(bDataStruct)
		{
			while(MT->bIsTaskRunStatus())
			{
				if(eventReceive(SHUTDOWN_EVENT, EVENTS_WAIT_ANY, WAIT_FOREVER, &iEvent) != ERROR)
				{
					MT->TerminateTasks();
					taskDelay(FORTY_MS_DELAY);		/* Delay for exit all the business logic tasks */
					MT->DeleteTasks();				/* Delete all tasks */
					MT->DestroyAllMsgQ();
				}
			}
		}
		else
		{
			LOGERR((char *) "Main_T : Data structure creation failed",0,0,0);
			MT->DestroyAllMsgQ();
			MT->TerminateTasks();
		}

		MT->ReleaseMemory();
		delete MT;
	}
	else
	{
		LOGERR((char *) "Main_T : --------Memory allocation failed------------",0,0,0);
	}
	
	MT = NULL;
	GPIO::FreeGPIO();
	LOGINFO((char *) "Main_T : EXIT FROM RTP PROCESS",0,0,0);
	return 0;
}

/**************************************************************************//**
 * \brief   - Initialise Auxclk timer for 1ms and connect to control task
 * 			  Partition
 * \param   - None
 *
 * \return  - None
 *
 ******************************************************************************/
void MainTask::InitAuxClk()
{
	INT32 status = 0;
	UINT32 SampleRate;
	AuxClockTask* auxClockObj = AuxClockTask::getInstance();
	SampleRate = (1000*1000)/DEFAULT_TIMEINTERVAL_MS;
	status = sysAuxClkRateSet(SampleRate);

	if(status == OK)
		LOG("\nMain_T : AuxClk set at Rate = %d\n", sysAuxClkRateGet());
	else
		LOGERR((char *) "Main_T : AuxClk set Rate failed status: %d", status, 0, 0);
	

	status = sysAuxClkConnect((FUNCPTR)AuxClockTask::AuxClock_Task, (_Vx_usr_arg_t)auxClockObj);
	
	if(status == OK)
		LOG("\nMain_T : Connected Aux Clock ISR successful \n");
	else
		LOGERR((char *) "Main_T : Failed to connect Aux Clock ISR!",0,0,0);

	sysAuxClkEnable();
	vxbMsDelay(1000);
}

/*
 * Place holders for yet to be added tasks: remove as you go along
 * with the real task entry point
 */
void DataBase_Task(void)
{
	
}

void Alarm_Task(void)
{
	
}

void BarcodeReader_Task(void)
{
	
}

void Maintenance_Task(void)
{
	
}

void Socket_Gateway_Task(void)
{
	
}

void Actuator_Process_Task(void)
{
	
}

void DSP_Task(void)
{
	
}

void Actuator_System_Task(void)
{
	
}

void ScDgtInput_Task(void)
{
	
}

void ScDgtOutput_Task(void)
{
	
}

void FirmwareUpgrade_Task(void)
{
	
}



