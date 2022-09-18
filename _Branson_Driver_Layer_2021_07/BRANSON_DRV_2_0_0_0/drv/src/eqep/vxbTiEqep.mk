# Makefile - makefile for Epwm

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbTiEpwm driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbTiEqep.c
OBJS_COMMON += vxbTiEqep.o vxbEqepLib.o
endif

