/* httpType.h - HTTP server types header */

/* Copyright (c) 1998-2003,2010,2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
/*
modification history
02b,17dec12,r_w  Move "#ifdef __cplusplus "C" {" after all the #includes. defect WIND00391948
02a,2feb10,h_y   support for LP64 
01f,10oct07,ten  Need to include wm_options.h before WEBCLI_ON_LINUX is
                 evaluated
01e,16feb06,ps  changes for webcli on linux
01d,08aug03,jws  HTTP_BOOL -> BOOL
01c,09sep99,csr  merging for zlib
01d,09jun99,km   merge of Wind Web Server 2.0 code.
01c,02jun98,mb   bugfix: GNU PPC compiler handles type char like unsigned
                 char -> negative values should be avoided for type char.
01b,13may97,mb   osal introduced.
01a,13may97,mb   written.
*/

#ifndef __INChttpTypeh
#define __INChttpTypeh

/* includes */
#include "commonCfg.h"
#include <webcli_osconf.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* typedefs */

typedef short         HTTP_STATUS;
typedef BOOL          HTTP_BOOL;
typedef va_list       HTTP_VA_LIST;
typedef long          (*HTTP_FUNCPTR)(); /* ptr to function returning long */

/* defines */

/* valid values for HTTP_STATUS */

#define HTTP_OK      0
#define HTTP_ERROR  -1

/* valid values for HTTP_BOOL */

#define HTTP_FALSE  FALSE
#define HTTP_TRUE   TRUE


#ifdef __cplusplus
}
#endif

#endif /* INChttpTypeh */

