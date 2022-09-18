/* httpSaxC.h - This is the C-Language SAX like API header file */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,26jan10,h_y      support for LP64
01d,12oct05,wdz      '#ifdef __cplusplus' statements added
                 for SPR#98444
01c,13apr04,adb  added useSecureFS flag to the XML_DOC_ENV structure 
01b,20feb04,jws  added copyright
01a,17jan02,jc   imported, renamed and modified from RCX codebase
*/


#ifndef __INChttpSaxCh
#define __INChttpSaxCh

#ifdef __cplusplus
extern "C" {
#endif 

/*
 *
 *  SAX function definitions.
 *
 */

typedef enum SAX_FUNCTION_ID_tag
{
    kSAX_START_DOCUMENT = 1,
    kSAX_END_DOCUMENT,
    kSAX_START_ELEMENT,
    kSAX_END_ELEMENT,
    kSAX_CHARACTERS,
    kSAX_IGNORABLE_WHITESPACE,
    kSAX_PROCESSING_INSTRUCTION
}SAX_FUNCTION_ID;

typedef struct XML_DOC_ENV_tag
{
    sbyte * tag;
    ubyte2  numAttr;
    sbyte * attrib;
    sbyte * p_XmlDocument;
    sbyte * p_DTDUrl;
    BOOL    useSecureFS;
} XML_DOC_ENV;


typedef STATUS (*saxCallBack)(XML_DOC_ENV * pXmlEnv, SAX_FUNCTION_ID, sbyte*);

/*
 * Function Prototypes
 */
extern STATUS sax_startParser(XML_DOC_ENV * pXmlEnv, saxCallBack    pDocHandler,
                              sbyte *pBuffer,unsigned long bufSize);
extern STATUS sax_getAttribute(XML_DOC_ENV * pXmlEnv, ubyte2 index,
                               sbyte **ppAttrib, sbyte **ppValue);
extern STATUS sax_getAttributeCount(XML_DOC_ENV * pXmlEnv, sbyte *pElement,
                                    ubyte2 *pCount);

#ifdef __cplusplus
}
#endif
#endif /* __INChttpSaxCh */
