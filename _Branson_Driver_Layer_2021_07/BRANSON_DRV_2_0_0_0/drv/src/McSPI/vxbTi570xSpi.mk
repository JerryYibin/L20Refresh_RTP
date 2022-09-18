# Makefile - makefile for mcspi

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbTi570xSpi driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES   += vxbTi570xSpi.o vxbMcSpiLib.c
OBJS_COMMON += vxbTi570xSpi.o vxbMcSpiLib.o
endif

