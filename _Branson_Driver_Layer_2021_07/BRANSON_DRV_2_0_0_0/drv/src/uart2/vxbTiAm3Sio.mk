# Makefile - makefile for vxbTiAm3Sio.c

# Copyright (c) 2014-2015 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 28sep15,yjw  added Cortex-M support (US64520)
# 25jun14,c_t  created 

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbTiAm3Sio.c
OBJS_ARMARCH7 += vxbTiAm3Sio.o
OBJS_ARMARCH7M += vxbTiAm3Sio.o
endif
