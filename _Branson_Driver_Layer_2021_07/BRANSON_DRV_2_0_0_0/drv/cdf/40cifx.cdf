/**************************************************************************************
 
   Copyright (c) Hilscher GmbH. All Rights Reserved.
 
 **************************************************************************************
 
   Filename:
    $Workfile: $
   Last Modification:
    $Author: $
    $Modtime: $
    $Revision: $
   
   Targets:
     VxWorks   : yes
 
   Description:
     Component Description File (CDF) for cifX device driver
       
   Changes:
 
     Version   Date        Author   Description
     ----------------------------------------------------------------------------------
     1         15.06.2009  SS       initial version
     
**************************************************************************************/
Component DRV_HILSCHER_CIFX {
  NAME         Hilscher CIFX Device Driver
  SYNOPSIS     Driver for Hilscher CIFX cards
  CFG_PARAMS   SINGLE_DIR \
               TRACE_LEVEL \
               POLL_INTERVAL \
               BASE_DIR 
  MODULES      vxbCifXVxWorks.o vxbPciCifX.o cifXVxWorks.o OS_VxWorks.o User_VxWorks.o \
               cifXInit.o cifXFunctions.o cifXHWFunctions.o cifXEndianess.o cifXDownload.o cifXInterrupt.o _md5_.o
  LINK_SYMS    registerCIFXDrv
  PROTOTYPE    STATUS registerCIFXDrv \
               ( \
               BOOL   fSingleDir, \
               UINT32 ulTraceLevel, \
               UINT32 ulPollInterval, \
               char * szDriverBaseDir \
               );
  INIT_RTN     registerCIFXDrv(SINGLE_DIR, TRACE_LEVEL, POLL_INTERVAL, BASE_DIR);  
/* HDR_FILES    hwif/drv/cifx/vxbCifXVxWorks.h */
  HDR_FILES    vxbCifXVxWorks.h
  _INIT_ORDER  usrRoot
  REQUIRES     INCLUDE_VXBUS \
  			       INCLUDE_ISR_DEFER
  _CHILDREN    FOLDER_DRIVERS
}

/*
Component INCLUDE_CIFX {
  NAME         cifX device driver
  SYNOPSIS     cifX device driver for VxWorks
  CFG_PARAMS   TRACE_LEVEL \
               POLL_INTERVAL \
               BASE_DIR \
               SINGLE_DIR \
               DEFER_INIT
  MODULES      cifxvxworks.o \
               cifxdownload.o \
               cifxfunctions.o \
               cifxhwfunctions.o \
               cifxinit.o \
               cifxinterrupt.o \
               _md5_.o \
               os_vxworks.o \
               user_vxworks.o
  CONFIGLETTES cifxdrv.c
  INIT_RTN     cifXInit(TRACE_LEVEL, POLL_INTERVAL, BASE_DIR, SINGLE_DIR, DEFER_INIT);
  _INIT_ORDER  usrRoot
  INIT_BEFORE  INCLUDE_USER_APPL
  HDR_FILES    ../config/hilscher/cifxdrv/cifxvxworks.h
  _CHILDREN    FOLDER_DRIVERS
}

Component INCLUDE_CIFX_DPM {
  NAME         cifX DPM
  SYNOPSIS     Adds a DPM based cifX device
  CFG_PARAMS   DPM_ADDRESS \
               DPM_IRQ \
               DPM_SIZE \
               DPM_MMU
  REQUIRES     INCLUDE_CIFX
  INIT_RTN     cifXDevDPM(DPM_ADDRESS, DPM_IRQ, DPM_SIZE, DPM_MMU);
  _INIT_ORDER  usrRoot
  INIT_BEFORE  INCLUDE_MMU_BASIC
  _CHILDREN    FOLDER_DRIVERS
}
*/
Parameter TRACE_LEVEL {
  NAME Set the trace level of the driver
  SYNOPSIS Options are: TRACE_LEVEL_ERROR, TRACE_LEVEL_WARNING, TRACE_LEVEL_INFO, TRACE_LEVEL_DEBUG
  TYPE UINT32
  DEFAULT TRACE_LEVEL_ERROR|TRACE_LEVEL_WARNING
}

Parameter POLL_INTERVAL {
  NAME Poll interval in milliseconds for  non-interrupt cards
  TYPE UINT32
  DEFAULT 500
}

Parameter BASE_DIR {
  NAME Set the base directory of the driver
  TYPE string
  DEFAULT "/hd0/cifX"
}

Parameter SINGLE_DIR {
  NAME Use a single firmware directory
  TYPE BOOL
  DEFAULT FALSE
}

