# Makefile - makefile for vxbI2cEeprom.c
#
# Copyright (c) 2011, 2013 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01b,10oct13,x_z  added C99 flags and dependency with FDT.
# 01a,22oct11,y_y  written
#
# DESCRIPTION
# This file contains the makefile macro values for the
# I2C EEPROM driver
#

ifdef _WRS_CONFIG_FDT

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

DOC_FILES	+= vxbI2cTpicLed.c
OBJS_COMMON += vxbI2cTpicLed.o
endif
