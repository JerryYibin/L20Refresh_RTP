/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: OS_Includes.h 7069 2015-07-21 12:23:45Z LuisContreras $:

  Description:
    Headerfile for specific operating system includes

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2017-10-12  FL, fixed build error
    2015-06-08  created from VxWorks 6x driver version 4


**************************************************************************************/

/*****************************************************************************/
/*! \file OS_Includes.h
*   Headerfile for specific operating system includes                        */
/*****************************************************************************/

#ifndef __OS_INCLUDES__H
#define __OS_INCLUDES__H

/* System includes */
#include <vxWorks.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <sysLib.h>
#include <taskLib.h>
#include <tickLib.h>
#include <pmapLib.h>
#include <vmLib.h>
#include <hwif/vxBus.h>
#include <hwif/buslib/vxbPciLib.h>
#include <hwif/buslib/pciDefines.h>
#include <hwif/util/vxbDmaBufLib.h>

/* OS specific definitions */

#define APIENTRY

#ifndef NULL
  #define NULL 0
#endif

#ifndef UNREFERENCED_PARAMETER
  #define UNREFERENCED_PARAMETER(a) (a=a)
#endif

//#ifndef _DIAB_TOOL
//  #define _DIAB_TOOL
//#endif

/* support for DIAB compiler */
#ifdef _DIAB_TOOL
	#define __CIFX_PACKED_PRE
	#define __CIFX_PACKED_POST __attribute__((packed)) 
#endif

#endif /* __OS_INCLUDES__H */
