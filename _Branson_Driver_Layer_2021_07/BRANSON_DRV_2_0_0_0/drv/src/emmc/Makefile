LIB_BASE_NAME=BRANSON_DRV

# Including defs.library to see if _WRS_CONFIG_xxx is defined.
# It must be after LIB_BASE_NAME

include $(WIND_KRNL_MK)/defs.library.mk


OBJS_COMMON	=

include $(wildcard $(sort *.mk))

OBJS= $(OBJS_COMMON) $(OBJS_$(CPU))

include $(WIND_KRNL_MK)/rules.library.mk

CC_OPTIM=$(CC_OPTIM_NORMAL)
ifeq ($(TOOL_FAMILY), diab)
CC_OPTIM = -Xdialect-c99
endif

SUBDIRS = core host storage
EXTRA_INCLUDE = -I$(LIB_BASE_NAME)/src/emmc -I$(LIB_BASE_NAME)/src/emmc/core
EXTRA_INCLUDE = -I$(LIB_BASE_NAME)/src/emmc -I$(LIB_BASE_NAME)/src/emmc/host
EXTRA_INCLUDE = -I$(LIB_BASE_NAME)/src/emmc -I$(LIB_BASE_NAME)/src/emmc/storage

include $(WIND_KRNL_MK)/defs.library.mk

#include $(wildcard $(sort *.mk))

OBJS= $(OBJS_COMMON) $(OBJS_$(CPU))

METHODM = $(sort $(wildcard *.m))

OBJS= $(METHODM:.m=.o) $(OBJS_COMMON) $(OBJS_$(CPU))

BUILD_ALL_OBJS=TRUE
include $(WIND_KRNL_MK)/rules.library.mk

CC_OPTIM=$(CC_OPTIM_NORMAL)
include $(WIND_KRNL_MK)/defs.drivers.mk

