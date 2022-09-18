/*
 * customSystemCallHdlr.c
 *
 */
/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
06may22,ykm  created.
*/
#ifndef __INCcustomSystemCallh
#define __INCcustomSystemCallh

/* Header files */
#include <vxWorks.h>
#ifdef __cplusplus
extern "C" {
#endif

/* function declarations */
#ifndef _ASMLANGUAGE

#define LOGERR(str,arg1, arg2, arg3)  logERR ((UINT8 *)str, __LINE__, (UINT8 *)__FILE__, LOG_ERROR, 1, arg1, arg2, arg3)
#define LOGWARN(str,arg1, arg2, arg3) logWARN((UINT8 *)str, __LINE__, (UINT8 *)__FILE__, LOG_WARNING, 0, arg1, arg2, arg3)
#define LOGINFO(str,arg1, arg2, arg3) logDBG ((UINT8 *)str, __LINE__, (UINT8 *)__FILE__, LOG_DEBUG, 0, arg1, arg2, arg3)

typedef enum
    {
    DISABLE_LOGS,
    LOG_ERROR,
    LOG_WARNING,
    LOG_DEBUG,
    MAX_LOG_LEVELS
    }LOG_LEVEL_LIST;
extern void vxbMsDelay(int);
extern STATUS logERR(UINT8 *logData, UINT32 lineNo, const UINT8 *fileName, UINT32 logLevel, UINT32 logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3);
extern STATUS logWARN(UINT8 *logData, UINT32 lineNo, const UINT8 *fileName, UINT32 logLevel, UINT32 logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3);
extern STATUS logDBG(UINT8 *logData, UINT32 lineNo, const UINT8 *fileName, UINT32 logLevel, UINT32 logToFile, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2, _Vx_usr_arg_t arg3);
#endif /* _ASMLANGUAGE */
#ifdef __cplusplus
}
#endif
#endif/* __INCcustomSystemCallh */

