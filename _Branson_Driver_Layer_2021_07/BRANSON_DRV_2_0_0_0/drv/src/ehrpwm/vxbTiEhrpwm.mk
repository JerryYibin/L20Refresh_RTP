# Makefile - makefile for Epwm

# DESCRIPTION
#
# This file contains the makefile macro values for the
# vxbTiEpwm driver
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += vxbTiehrpwm.c
OBJS_ARMARCH7M += vxbTiehrpwm.o vxbEhrPwmLib.o
OBJS_ARMARCH7 += vxbTiehrpwm.o vxbEhrPwmLib.o
endif
