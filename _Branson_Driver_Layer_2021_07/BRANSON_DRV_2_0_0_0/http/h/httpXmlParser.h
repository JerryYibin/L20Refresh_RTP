/* httpXmlParser.h - definitions for httpXmlParser.c */

/* Copyright 2004,2010 Wind River Systems, Inc. */

/*
modification history
-------------------- 
01c,2feb10,h_y   support for LP64  
01b,20feb04,jws  added copyright
01a,17jan02,jc   imported, renamed and modified from RCX codebase
*/


#ifndef __INChttpXmlParserh
#define __INChttpXmlParserh


#define CHAR_NULL           '\0'
#define CHAR_SPACE          ' '
#define CHAR_TAB            '\t'
#define CHAR_NEWLINE        '\n'
#define CHAR_RETURN         '\r'
#define CHAR_LT             '<'
#define CHAR_GT             '>'
#define CHAR_EXCLAMATION    '!'
#define CHAR_SLASH          '/'
#define CHAR_EQ             '='
#define CHAR_QUOTE          '\"'
#define CHAR_OPENBR         '['


#define STRING_XML              "<?xml"
#define STRING_ENDXML           "?>"
#define STRING_LTQM             "<?"
#define STRING_QMGT             "?>"
#define STRING_LTGT             "<>"
#define STRING_LTEX             "<!"
#define STRING_EXGT             "!>"
#define STRING_LTSL             "</"
#define STRING_SLGT             "/>"
#define STRING_BEGINCOMMENT     "<!--"
#define STRING_ENDCOMMENT       "--!>"
#define STRING_NLRET            "\n\r"
#define STRING_WHITESPACE       " \t\n\r"
#define STRING_XMLSYMBOLS       "<>&\"'"
#define STRING_DOCTYPE          "<!DOCTYPE"
#define STRING_SYSTEM           "SYSTEM"
#define STRING_PUBLIC           "PUBLIC"
#define STRING_GTNL             ">\n"


extern void xmlParseTransformToXMLString(sbyte **srcstr);
extern void xmlParseGetAttribute(XML_DOC_ENV *pXmlEnv, ubyte2 pos,
                                 sbyte **ppName, sbyte **ppValue);
extern void xmlParseGetAttributeCount(XML_DOC_ENV *pXmlEnv, sbyte *pElement,
                                      ubyte2 *pNumAttr);
extern STATUS xmlParseDocument(XML_DOC_ENV *pXmlEnv, sbyte *pBuffer,
                               saxCallBack    pCallback);

#endif /* __INChttpXmlParserh */
