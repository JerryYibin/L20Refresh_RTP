/* httpWmbExpandInstance.h - header file for httpWmbExpandInstance.c */

/* Copyright 2001-2004,2010-2011 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01d,03mar11,r_w  fix defect WIND00255073 and WIND00255074
01c,2feb10,h_y   support for LP64  
01b,03may04,dkg  Modified prototype of httpParseInlineExpression() and 
                 httpProcessMarkup() to fix SPR 96709
01a,16jan04,gsr  Created for SPR#91008
*/

/*
DESCRIPTION
This file declares routines for the expansion of wind instances

INCLUDE FILES: wm.h wmb.h
*/

#ifndef __INChttpWmbExpandInstanceh
#define __INChttpWmbExpandInstanceh

#include "wm.h"
#include "memPoolLib.h"
#include <stdlib.h>
#include <string.h>
#include "httpCfg.h"
#ifndef WEBCLI_STANDALONE_WEBSERVER
#include "wmb.h"
#endif
/* ported defines */
#define kInlineStartDelim       '{'
#define kInlineEndDelim         '}'

/* inner expression delims */
#define kInlineExpStartDelim    '['
#define kInlineExpEndDelim      ']'
#define kInlineExpStartBrace    '('
#define kInlineExpEndBrace      ')'
#define kInlineSep              ','


#define kMaxExpResultLen        255
#define kMaxMibLength           64

#define kMaxInstanceLen         128

#ifndef WEBCLI_STANDALONE_WEBSERVER
void     httpExpandInstance ( WMB_SESSION_T *   pSession, char*, char *);

void     httpParseInlineExpression ( WMB_SESSION_T *   pSession, char *p_inlineExpString, char *pOutputString, sbyte4 OutputLen,sbyte, sbyte);

STATUS   httpProcessMarkup (  WMB_SESSION_T *   pSession, sbyte *pReadObjectString, sbyte *pOutputBuf, sbyte4 OutputLen);
#endif

STATUS   httpInsertMarkup (sbyte **pStart, sbyte *pOBracket, sbyte *pCBracket, sbyte* outputBuf, sbyte outputLen);

BOOL     ISALPHANUMERIC (sbyte4 c);


#endif /* __INChttpWmbExpandInstanceh */
