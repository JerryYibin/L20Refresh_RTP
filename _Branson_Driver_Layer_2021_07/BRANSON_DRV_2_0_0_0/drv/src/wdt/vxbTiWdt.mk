# Makefile - makefile for vxbTiWdt.c
#
# Copyright (c) 2022 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,26Jan22,ykm  written
#
# DESCRIPTION
# This file contains the makefile macro values for the WDT driver
#

ifdef _WRS_CONFIG_FDT

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

DOC_FILES	+= vxbTiWdt.c
OBJS_COMMON += vxbTiWdt.o
endif
