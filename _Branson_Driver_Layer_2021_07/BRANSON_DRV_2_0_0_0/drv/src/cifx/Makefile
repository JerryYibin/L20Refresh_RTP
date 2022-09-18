# Makefile - makefile for src/hwif/cifx
#
# Copyright (c) 2013 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 2017-10-12  FL, fixed build error
# 05Sept17,fl  add clifx
# 01a,09jun15,r_m  created
#
# DESCRIPTION
# This file contains the makefile rules for building the vxBus cifx library
#
#*/
LIB_BASE_NAME=BRANSON_DRV

# Including defs.library to see if _WRS_CONFIG_xxx is defined.
# It must be after LIB_BASE_NAME

#include $(WIND_KRNL_MK)/defs.library.mk


#OBJS_COMMON	=

#include $(wildcard $(sort *.mk))

#OBJS= $(OBJS_COMMON) $(OBJS_$(CPU))

#include $(WIND_KRNL_MK)/rules.library.mk

CC_OPTIM=$(CC_OPTIM_NORMAL)
ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

#include $(WIND_KRNL_MK)/defs.drivers.mk


#TGT_DIR = $(WIND_BASE)/pkgs/os/drv/vxbus/drv-1.1.9.1/
#TGT_DIR = $(WIND_BASE)/pkgs/os/drv/vxbus/drv-1.2.5.0/

DOC_FILES = cifXVxWorks.c vxbCifXVxWorks.c vxbPciCifX.c OS_VxWorks.c User_VxWorks.c

#LIB_BASE_NAME=vxbus
# Including defs.library to see if _WRS_CONFIG_xxx is defined.
# It must be after LIB_BASE_NAME

SUBDIRS = cifXToolkit

#EXTRA_INCLUDE = -I$(TGT_DIR)src/cifx -I$(TGT_DIR)src/cifx/cifXToolkit
EXTRA_INCLUDE = -I$(LIB_BASE_NAME)/src/cifx -I$(LIB_BASE_NAME)/src/cifx/cifXToolkit

EXTRA_DEFINE = -DCIFX_TOOLKIT_DMA -DCIFX_TOOLKIT_ENABLE_DRS_LOCK

include $(WIND_KRNL_MK)/defs.library.mk

# OBJS_COMMON	= cifXVxWorks.o vxbCifXVxWorks.o vxbPciCifX.o OS_VxWorks.o User_VxWorks.o 

#include $(wildcard $(sort *.mk))

OBJS= $(OBJS_COMMON) $(OBJS_$(CPU))

METHODM = $(sort $(wildcard *.m))

OBJS= $(METHODM:.m=.o) $(OBJS_COMMON) $(OBJS_$(CPU))

BUILD_ALL_OBJS=TRUE
include $(WIND_KRNL_MK)/rules.library.mk

CC_OPTIM=$(CC_OPTIM_NORMAL)
include $(WIND_KRNL_MK)/defs.drivers.mk

