# Makefile - Custom system call definition layer Makefile
#
# Copyright 2013 Wind River Systems, Inc.
#      
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# -------------------- 
# 01a,01oct13,pad   created

LIB_BASE_NAME   = MyOwnSc

include $(WIND_KRNL_MK)/defs.library.mk

OBJS_COMMON=	customSystemCallHdlr.o

OBJS		= $(OBJS_COMMON) 

include $(WIND_KRNL_MK)/rules.library.mk
