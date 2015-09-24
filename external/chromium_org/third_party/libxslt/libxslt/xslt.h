/*
 * Summary: Interfaces, constants and types related to the XSLT engine
 * Description: Interfaces, constants and types related to the XSLT engine
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLT_H__
#define __XML_XSLT_H__

#include <libxml/tree.h>
#include "xsltexports.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XSLT_DEFAULT_VERSION     "1.0"

#define XSLT_DEFAULT_VENDOR      "libxslt"

#define XSLT_DEFAULT_URL         "http://xmlsoft.org/XSLT/"

#define XSLT_NAMESPACE ((xmlChar *) "http://www.w3.org/1999/XSL/Transform")

#define XSLT_PARSE_OPTIONS \
 XML_PARSE_NOENT | XML_PARSE_DTDLOAD | XML_PARSE_DTDATTR | XML_PARSE_NOCDATA

XSLTPUBVAR int xsltMaxDepth;

XSLTPUBVAR const char *xsltEngineVersion;

XSLTPUBVAR const int xsltLibxsltVersion;

XSLTPUBVAR const int xsltLibxmlVersion;


XSLTPUBFUN void XSLTCALL
		xsltInit		(void);

XSLTPUBFUN void XSLTCALL	
		xsltCleanupGlobals	(void);

#ifdef __cplusplus
}
#endif

#endif 

