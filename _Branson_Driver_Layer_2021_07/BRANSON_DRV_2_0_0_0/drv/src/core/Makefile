# Makefile - makefile for core
#
# Copyright (c) 2013 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,4sep13,y_y  created
#
# DESCRIPTION
# This file contains the makefile rules for building the core library
#
#*/

LIB_BASE_NAME=BRANSON_DRV

# Including defs.library to see if _WRS_CONFIG_xxx is defined.
# It must be after LIB_BASE_NAME

include $(WIND_KRNL_MK)/defs.library.mk

OBJS_COMMON +=  vxbPru.o vxbPruIntcLib.o
DOC_FILES   +=  vxbPru.c vxbPruIntcLib.c

include $(wildcard $(sort *.mk))

OBJS= $(OBJS_COMMON) $(OBJS_$(CPU))

include $(WIND_KRNL_MK)/rules.library.mk

CC_OPTIM=$(CC_OPTIM_NORMAL)
include $(WIND_KRNL_MK)/defs.drivers.mk

