/*
 * Summary: interface for the document handling
 * Description: implements document loading and cache (multiple
 *              document() reference for the same resources must
 *              be equal.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLT_DOCUMENTS_H__
#define __XML_XSLT_DOCUMENTS_H__

#include <libxml/tree.h>
#include "xsltexports.h"
#include "xsltInternals.h"

#ifdef __cplusplus
extern "C" {
#endif

XSLTPUBFUN xsltDocumentPtr XSLTCALL	
		xsltNewDocument		(xsltTransformContextPtr ctxt,
					 xmlDocPtr doc);
XSLTPUBFUN xsltDocumentPtr XSLTCALL	
    		xsltLoadDocument	(xsltTransformContextPtr ctxt,
					 const xmlChar *URI);
XSLTPUBFUN xsltDocumentPtr XSLTCALL	
    		xsltFindDocument	(xsltTransformContextPtr ctxt,
					 xmlDocPtr doc);
XSLTPUBFUN void XSLTCALL		
    		xsltFreeDocuments	(xsltTransformContextPtr ctxt);

XSLTPUBFUN xsltDocumentPtr XSLTCALL	
    		xsltLoadStyleDocument	(xsltStylesheetPtr style,
					 const xmlChar *URI);
XSLTPUBFUN xsltDocumentPtr XSLTCALL	
    		xsltNewStyleDocument	(xsltStylesheetPtr style,
					 xmlDocPtr doc);
XSLTPUBFUN void XSLTCALL		
    		xsltFreeStyleDocuments	(xsltStylesheetPtr style);


typedef enum {
    XSLT_LOAD_START = 0,	
    XSLT_LOAD_STYLESHEET = 1,	
    XSLT_LOAD_DOCUMENT = 2	
} xsltLoadType;

typedef xmlDocPtr (*xsltDocLoaderFunc)		(const xmlChar *URI,
						 xmlDictPtr dict,
						 int options,
						 void *ctxt,
						 xsltLoadType type);

XSLTPUBFUN void XSLTCALL
		xsltSetLoaderFunc		(xsltDocLoaderFunc f);

XSLTPUBVAR xsltDocLoaderFunc xsltDocDefaultLoader;

#ifdef __cplusplus
}
#endif

#endif 

