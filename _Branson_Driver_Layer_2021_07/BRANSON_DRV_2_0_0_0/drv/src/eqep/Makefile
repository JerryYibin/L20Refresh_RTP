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

ifeq ($(TOOL_FAMILY), gnu)
CC_OPTIM = -std=c99
endif

include $(WIND_KRNL_MK)/defs.drivers.mk

