/* httpWmbMacro.h - header file for WMB macros */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01b,25feb04,dkg  Added struct stack_entry_tag and struct macro_stack_tag
                 to fix SPR 91625 
01a,20feb04,jws  added copyright and mod history
*/

#ifndef __INChttpWmbMacroh
#define __INChttpWmbMacroh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* typedefs (opaque)*/

typedef struct macro_stack_tag * MACRO_STACK_ID;

typedef struct  stack_entry_tag
    {
    ubyte4  startAddr;
    ubyte4  remaining;
    } STACK_ENTRY;

typedef struct macro_stack_tag
    {
    STACK_ENTRY  *  entry;
    ubyte4          size;
    ubyte4          count;
    } MACRO_STACK;

/* prototypes */
sbyte4 httpIsMacro   (sbyte * pMark);

STATUS httpMacroInit (WMB_SESSION_T * pSession, ubyte4 size);

STATUS httpMacroCleanup (WMB_SESSION_T * pSession);

STATUS httpMacroExecute (WMB_SESSION_T * pSession, sbyte * pMacro,
                         ubyte4 * pFilePos);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __INChttpWmbMacroh */
