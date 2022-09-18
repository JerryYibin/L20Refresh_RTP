/* vxbEqepLib.h - VxBus EQEP interfaces header file */

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
05may22,ykm  created.
*/
/*
DESCRIPTION

This library provides the EQEP specific interfaces

*/

#ifndef __INCvxbEqepLibh
#define __INCvxbEqepLibh 

#include <vxWorks.h>

#ifdef __cplusplus
extern "C" {
#endif
extern UINT32 eqepDevNumGet();
extern STATUS vxbEqepShow(UINT32);
extern STATUS vxbEqepGetMaxCount(UINT32);
extern STATUS vxbEqepSetMaxCount(UINT32, UINT32);
extern STATUS vxbEqepGetInitCount(UINT32);
extern STATUS vxbEqepSetInitCount(UINT32, UINT32);
extern STATUS vxbEqepGetPosCount(UINT32);
extern STATUS vxbEqepGetDirection(UINT32);
#ifdef __cplusplus
}
#endif

#endif /* __INCvxbEqepLibh */


