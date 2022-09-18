# Makefile - makefile for DCAN

# DESCRIPTION
#
# This file contains the makefile macro values for the
# CAN Socket library
#

ifdef _WRS_CONFIG_FDT
DOC_FILES     += socketCAN.c
OBJS_ARMARCH7M += socketCAN.o
OBJS_ARMARCH7 += socketCAN.o
endif
