## Makefile for kernel src directory for BRANSON_FLASH 

LIB_BASE_NAME = BRANSON_DRV

# the following is an example of using a wildcard to select the src files
SRC_C = $(wildcard *.c)
SRC_ASM = $(wildcard *.s)

# If this layer is to be used to generate binary layers then
# the OBJS list must be an explicit list of objects 
# as there are no source files shipped in a binary layer
OBJS = $(SRC_C:.c=.o) $(SRC_ASM:.s=.o)
OBJ += qspi_lib.o
include $(WIND_KRNL_MK)/rules.library.mk
