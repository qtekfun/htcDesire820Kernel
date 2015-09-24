/*
 * Summary: XML Schemastron implementation
 * Description: interface to the XML Schematron validity checking.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */


#ifndef __XML_SCHEMATRON_H__
#define __XML_SCHEMATRON_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_SCHEMATRON_ENABLED

#include <libxml/tree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    XML_SCHEMATRON_OUT_QUIET = 1 << 0,	
    XML_SCHEMATRON_OUT_TEXT = 1 << 1,	
    XML_SCHEMATRON_OUT_XML = 1 << 2,	
    XML_SCHEMATRON_OUT_ERROR = 1 << 3,  
    XML_SCHEMATRON_OUT_FILE = 1 << 8,	
    XML_SCHEMATRON_OUT_BUFFER = 1 << 9,	
    XML_SCHEMATRON_OUT_IO = 1 << 10	
} xmlSchematronValidOptions;

typedef struct _xmlSchematron xmlSchematron;
typedef xmlSchematron *xmlSchematronPtr;

typedef void (*xmlSchematronValidityErrorFunc) (void *ctx, const char *msg, ...);

typedef void (*xmlSchematronValidityWarningFunc) (void *ctx, const char *msg, ...);

typedef struct _xmlSchematronParserCtxt xmlSchematronParserCtxt;
typedef xmlSchematronParserCtxt *xmlSchematronParserCtxtPtr;

typedef struct _xmlSchematronValidCtxt xmlSchematronValidCtxt;
typedef xmlSchematronValidCtxt *xmlSchematronValidCtxtPtr;

XMLPUBFUN xmlSchematronParserCtxtPtr XMLCALL 
	    xmlSchematronNewParserCtxt	(const char *URL);
XMLPUBFUN xmlSchematronParserCtxtPtr XMLCALL 
	    xmlSchematronNewMemParserCtxt(const char *buffer,
					 int size);
XMLPUBFUN xmlSchematronParserCtxtPtr XMLCALL
	    xmlSchematronNewDocParserCtxt(xmlDocPtr doc);
XMLPUBFUN void XMLCALL		
	    xmlSchematronFreeParserCtxt	(xmlSchematronParserCtxtPtr ctxt);
XMLPUBFUN xmlSchematronPtr XMLCALL	
	    xmlSchematronParse		(xmlSchematronParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL		
	    xmlSchematronFree		(xmlSchematronPtr schema);
XMLPUBFUN void XMLCALL
	    xmlSchematronSetValidStructuredErrors(
	                                  xmlSchematronValidCtxtPtr ctxt,
					  xmlStructuredErrorFunc serror,
					  void *ctx);

XMLPUBFUN xmlSchematronValidCtxtPtr XMLCALL	
	    xmlSchematronNewValidCtxt	(xmlSchematronPtr schema,
	    				 int options);
XMLPUBFUN void XMLCALL			
	    xmlSchematronFreeValidCtxt	(xmlSchematronValidCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			
	    xmlSchematronValidateDoc	(xmlSchematronValidCtxtPtr ctxt,
					 xmlDocPtr instance);

#ifdef __cplusplus
}
#endif

#endif 
#endif 
