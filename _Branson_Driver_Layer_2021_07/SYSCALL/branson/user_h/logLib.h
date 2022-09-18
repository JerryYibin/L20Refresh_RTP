/* logLib.h - message logging library header */

/*
 * Copyright (c) 2022 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
05may22,ykm  created.
*/

#ifndef __INClogLibh
#define __INClogLibh

#ifndef _ASMLANGUAGE
#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int logMsg(char *fmt, _Vx_usr_arg_t arg1, _Vx_usr_arg_t arg2,
                    _Vx_usr_arg_t arg3, _Vx_usr_arg_t arg4,
                    _Vx_usr_arg_t arg5, _Vx_usr_arg_t arg6);
#ifdef __cplusplus
}
#endif

#endif /* !_ASMLANGUAGE */
#endif /* __INClogLibh */

