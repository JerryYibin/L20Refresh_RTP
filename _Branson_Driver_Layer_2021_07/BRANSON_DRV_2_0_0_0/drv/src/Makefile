## Makefile for kernel src directory for BRANSON_DRV 

LIB_BASE_NAME = branson_drv

include $(WIND_KRNL_MK)/defs.library.mk

SUBDIRS = spi core i2c resource pll pmic net storage mcspi cifx emmc wdt ehrpwm eqep loggerModule dcan


include $(WIND_KRNL_MK)/rules.library.mk

CC_OPTIM=$(CC_OPTIM_NORMAL)
include $(WIND_KRNL_MK)/defs.drivers.mk