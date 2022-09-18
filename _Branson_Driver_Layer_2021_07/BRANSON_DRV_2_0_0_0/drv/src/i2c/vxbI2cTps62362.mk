# Makefile - makefile for vxbI2cTps62362.c

ifdef _WRS_CONFIG_FDT

ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

DOC_FILES	+= vxbI2cTps62362.c
OBJS_COMMON += vxbI2cTps62362.o
endif
