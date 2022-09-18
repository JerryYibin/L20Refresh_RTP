/**************************************************************************************

Copyright (c) Hilscher Gesellschaft fuer Systemautomation mbH. All Rights Reserved.

***************************************************************************************

  $Id: vxbCifXVxWorks.h 6954 2015-07-10 13:10:34Z Robert $:

  Description:
    Definitions for the VxWorks driver initialization and handling functions

  Changes:
    Date        Description
    -----------------------------------------------------------------------------------
    2015-06-08  created from VxWorks driver V6.x revision 3

**************************************************************************************/
#ifndef __VXBCIFXVXWORKS__H
#define __VXBCIFXVXWORKS__H

/*****************************************************************************/
/*! \file vxbCifXVxWorks.h
*   Special definitions for VxWorks                                          */
/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*! Trace level definitions                                                  */
/*****************************************************************************/
#ifndef TRACE_LEVEL_DEBUG
#define TRACE_LEVEL_DEBUG   0x00000001
#endif
#ifndef TRACE_LEVEL_INFO
#define TRACE_LEVEL_INFO    0x00000002
#endif
#ifndef TRACE_LEVEL_WARNING
#define TRACE_LEVEL_WARNING 0x00000004
#endif
#ifndef TRACE_LEVEL_ERROR
#define TRACE_LEVEL_ERROR   0x00000008
#endif


#ifdef __cplusplus
}
#endif

#endif /* __VXBCIFXVXWORKS__H */

