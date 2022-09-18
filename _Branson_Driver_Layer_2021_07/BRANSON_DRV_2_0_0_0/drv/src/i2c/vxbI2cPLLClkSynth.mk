# Makefile - makefile for vxbPLLClkSynth.c

ifdef _WRS_CONFIG_FDT

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

DOC_FILES	+= vxbI2cPLLClkSynth.c
OBJS_COMMON += vxbI2cPLLClkSynth.o
endif