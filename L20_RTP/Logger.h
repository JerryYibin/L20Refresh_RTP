/************************************************************************** 

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.
 
***************************************************************************/

#ifndef LOGGER_H_
#define LOGGER_H_

extern "C"
{
	#include <customSystemCall.h>
}

/* Define the Color for LogMsg Display */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define LOG_LEVEL  	3
#define LOGFILE 	1

//#define LOGERR(str,arg1, arg2, arg3)  logERR(str, __LINE__,  __FILE__, LOG_LEVEL, LOGFILE,  arg1, arg2, arg3)
//#define LOGWARN(str,arg1, arg2, arg3) logWARN(str, __LINE__,  __FILE__, LOG_LEVEL, LOGFILE, arg1, arg2, arg3)
//#define LOGINFO(str,arg1, arg2, arg3)  logDBG(str, __LINE__,  __FILE__, LOG_LEVEL, LOGFILE,  arg1, arg2, arg3)

#define LOG printf

#endif /* LOGGER_H_ */
