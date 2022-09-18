/* sysALib.s - TI Cortex-A15 Sitara system entry */

/*
 * Copyright (c) 2015, 2018, 2020 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
13may20,lau  removed Diab macros (V7COR-7376)
17apr18,mpc  added Thumb2 support (F10138)
12oct15,yjw  created (US64529)
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly language.  
It contains the entry code, sysInit(), for VxWorks images that start running 
from RAM, such as 'vxWorks'.  These images are loaded into memory by some 
external program (e.g., a boot ROM) and then started.  The routine sysInit() 
must come first in the text segment. Its job is to perform the minimal setup 
needed to call the generic C routine usrInit().

sysInit() masks interrupts in the processor and the interrupt controller, also 
sets the initial stack pointer. Other hardware and device initialization is 
performed later in the usrInit().

NOTE
The routines in this module don't use the "C" frame pointer %r11@ ! or establish
a stack frame.

INCLUDE FILES:
*/

#define _ASMLANGUAGE
#include <vxWorks.h>
#include <vsbConfig.h>
#include <asm.h>
#include <regs.h>
#include <arch/arm/arm.h>
#include "prjParams.h"

        /* defines */

#define GIC_DIST                        (0x48211000)
#define GIC_CPU                         (0x48212000)
#define ARM_GIC_CPU_CONTROL             (GIC_CPU)
#define ARM_GIC_DIST_CONTROL            (GIC_DIST)
#define ARM_GIC_CPU_ACK                 (GIC_CPU + 0xC)
#define ARM_GIC_INT_SPURIOUS            (0x3FF)
#define ARM_GIC_CPU_END_INTR            (GIC_CPU + 0x10)
#define GIC_IntPendClr                  (GIC_DIST + 0x280)
#define ALL_PPI_INT_MASK                0xFFFF0000 
#define ALL_SGI_INT_MASK                0x0000FFFF
#define GIC_CPU_PriMask                 (GIC_CPU + 0x04)
#define GIC_INT_ALL_ENABLED             0xFF 
#define GIC_CPU_BinPoint                (GIC_CPU + 0x08)
#define GIC_CPU_BINP_DEFAULT            0x07
#define GIC_CPU_Control                 (GIC_CPU + 0x00)
#define GIC_CONTROL_ENABLE              0x01
#define GIC_IntEnClr                    (GIC_DIST + 0x180)
#define GIC_Prio                        (GIC_DIST + 0x400)

        /* exports */

        FUNC_EXPORT (sysInit)
        DATA_IMPORT (gpDtbInit)

        /* externals */

        FUNC_IMPORT (usrInit)

#ifdef INCLUDE_STANDALONE_DTB

        .data

#define TO_STRING(exp)                  #exp
#define DTS_ASM_FILE(file)              TO_STRING (file.s)

#include DTS_ASM_FILE (DTS_FILE)

#endif /* INCLUDE_STANDALONE_DTB */

#if defined (__GNUC__)
        .section .text.entry, "ax"
#else
#error "TOOL not supported!"
#endif

#ifndef INCLUDE_TI_AM5708_BRANSON
#ifndef INCLUDE_TI_AM5708_JN
        .code   32
#endif /* !INCLUDE_TI_AM5708_JN */
#endif /* !INCLUDE_TI_AM5708_BRANSON */

/*******************************************************************************
*
* sysInit - start after boot
*
* This routine is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up
* the stack, and jumps to the C routine usrInit().
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* NOTE: This routine should not be called by the user.
*
* SYNOPSIS
* \ss
* VOID sysInit
*     (
*     void * pDtb,  /@ physical pointer for device-tree in RAM  @/
*     )
* \se
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

FUNC_LABEL (sysInit)
#ifdef INCLUDE_TI_AM5708_BRANSON
        .thumb

        ADR     r3, arm_entry
        BX      r3

        .code   32

arm_entry:
#else
#ifdef INCLUDE_TI_AM5708_JN
        .thumb

        ADR     r3, arm_entry
        BX      r3

        .code   32

arm_entry:
#endif /* INCLUDE_TI_AM5708_JN */
#endif /* INCLUDE_TI_AM5708_BRANSON */

        MRS     r1, cpsr
#ifdef INCLUDE_TI_AM5708_BRANSON

        /* disable interrupts in CPU and switch to SVC32 mode */

        BIC     r1, r1, #MASK_MODE
        ORR     r1, r1, #MODE_SVC32 | I_BIT | F_BIT
#else /* !INCLUDE_TI_AM5708_BRANSON */
#ifdef INCLUDE_TI_AM5708_JN
        /* disable interrupts in CPU and switch to SVC32 mode */

        BIC     r1, r1, #MASK_MODE
        ORR     r1, r1, #MODE_SVC32 | I_BIT | F_BIT
#else /* !INCLUDE_TI_AM5708_JN */
        /* disable interrupts in CPU */

        ORR     r1, r1, #(I_BIT | F_BIT)
#endif /* INCLUDE_TI_AM5708_JN */
#endif /* INCLUDE_TI_AM5708_BRANSON */
        MSR     cpsr, r1

        /* set initial stack pointer so stack grows down from start of code */

        ADR     sp, sysInit
        MOV     fp, #0     /* initialize frame pointer */

        LDR     r1, =sysInit
        ADR     r6, sysInit
        SUB     r6, r6, r1

#ifdef _WRS_CONFIG_SMP
        MRC     p15, 0, r1, c0, c0, 5
        ANDS    r1, r1, #0xf
        BEQ     1f          /* if core0 */
	    BL	    sysArmGicDevInit
	    B	    2f
#endif

1:
#ifdef INCLUDE_STANDALONE_DTB
        LDR     r0, =dt_blob_start
#endif /* INCLUDE_STANDALONE_DTB */

        LDR     r1, =gpDtbInit
        ADD     r1, r1, r6
        STR     r0, [r1]

        /* disable interrupts for distributor */

        LDR     r0, =ARM_GIC_DIST_CONTROL
        MOV     r1, #0
        STR     r1, [r0]
        
       /* disable interrupts to CPU */

        LDR     r0, =ARM_GIC_CPU_CONTROL
        MOV     r1, #0
        STR     r1, [r0]

2:
        LDR     r1, =usrInit
        ADD     r1, r1, r6
        MOV     r0, #2      /* legacy BOOT_COLD */
        BX      r1

FUNC_END (sysInit)

#ifdef _WRS_CONFIG_SMP
sysArmGicDevInit:

	/* flush all pending local interrupts */

3:
	LDR	r0, =ARM_GIC_CPU_ACK
    LDR	r1, [r0]
    LDR r2, =ARM_GIC_CPU_END_INTR
	STR	r1, [r2]
	LDR	r3, =ARM_GIC_INT_SPURIOUS
	CMP	r1, r3
	BNE	3b
    LDR r3, =GIC_IntPendClr
	LDR	r0, =0xffff
	STR	r0, [r3]

	/* clear all pending PPI and SGI interrupts in the distributor */

	LDR	r0, =GIC_IntPendClr
	LDR	r1, =(ALL_PPI_INT_MASK | ALL_SGI_INT_MASK)
	STR	r1, [r0]

	/* set priority */

    MOV	r2, #4
	LDR	r0, =GIC_Prio
4:
	MOV	r1, #0
	STR	r1, [r0]
	ADD	r0, r0, #4
	SUBS	r2, r2, #1
	BNE	4b

    /* enable all interrupt priorities */

	LDR	r0, =GIC_CPU_PriMask
	LDR	r1, =GIC_INT_ALL_ENABLED
	STR	r1, [r0]

    /* enable preemption of all interrupts */

	LDR	r0, =GIC_CPU_BinPoint
	LDR 	r1, =GIC_CPU_BINP_DEFAULT
	STR	r1, [r0]

	/* enable this processor's CPU interface */

	LDR	r0, =GIC_CPU_Control
	LDR	r1, =GIC_CONTROL_ENABLE
	STR	r1, [r0]		

	MOV	pc, lr
#endif /* _WRS_CONFIG_SMP */
