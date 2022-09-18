## Makefile for Project BRANSON_DRV 

LIB_BASE_NAMES = bransondrv

include $(WIND_KRNL_MK)/defs.layers.mk

# Modify the following settings to change the header directories that get copied into the VSB
# KERNEL_PUBLIC_H_DIRS Files in these directories are copied into the VSB public kernel header directory; preferred directory name is h
KERNEL_PUBLIC_H_DIRS = h

# Modify the following settings to change the header files that get copied into the VSB
# KERNEL_PUBLIC_H_FILES These files are copied into the VSB public kernel header directory
# KERNEL_PUBLIC_H_FILES =

# Modify the following settings if your layer is being used to generate kernel libraries in the VSB
# BUILD_DIRS Directories for Kernel library source; preferred directory name is src
BUILD_DIRS = src

# BUILD_TOOLS Kernel compile tools
# BUILD_TOOLS =                             -primary compiler this is the default 
# BUILD_TOOLS = $(_WRS_CONFIG_ALL_TOOLS)    -Both VSB compilers LIB_COMMON = false must be set in src Makefile
# BUILD_TOOLS = $(_WRS_CONFIG_diab_VERSION) -diab only, add VSB_REQUIRES diab_SUPPORTED to layer.vsbl
# BUILD_TOOLS = $(_WRS_CONFIG_gnu_VERSION)  -gnu only, add VSB_REQUIRES gnu_SUPPORTED to layer.vsbl
# BUILD_TOOLS = $(_WRS_CONFIG_icc_VERSION)  -icc only, add VSB_REQUIRES icc_SUPPORTED to layer.vsbl
# BUILD_TOOLS = $(_WRS_CONFIG_llvm_VERSION)  -llvm only, add VSB_REQUIRES llvm_SUPPORTED to layer.vsbl
# BUILD_TOOLS =


# Modify the following settings if your layer has cdf files to be delivered in the VSB
# POST_NOBUILD_CDFDIRS Post-no-build CDF directories; preferred directory name is cdf
POST_NOBUILD_CDFDIRS = cdf

CC_WARNINGS_BUILD_DIRS = CC_WARNINGS_HIGH_SYS
CC_WARNINGS_BUILD_USER_DIRS = CC_WARNINGS_HIGH_SYS
include $(WIND_KRNL_MK)/rules.layers.mk
