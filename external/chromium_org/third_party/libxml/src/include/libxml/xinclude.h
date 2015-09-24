/*
 * Summary: implementation of XInclude
 * Description: API to handle XInclude processing,
 * implements the
 * World Wide Web Consortium Last Call Working Draft 10 November 2003
 * http://www.w3.org/TR/2003/WD-xinclude-20031110
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XINCLUDE_H__
#define __XML_XINCLUDE_H__

#include <libxml/xmlversion.h>
#include <libxml/tree.h>

#ifdef LIBXML_XINCLUDE_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#define XINCLUDE_NS (const xmlChar *) "http://www.w3.org/2003/XInclude"
#define XINCLUDE_OLD_NS (const xmlChar *) "http://www.w3.org/2001/XInclude"
#define XINCLUDE_NODE (const xmlChar *) "include"
#define XINCLUDE_FALLBACK (const xmlChar *) "fallback"
#define XINCLUDE_HREF (const xmlChar *) "href"
#define XINCLUDE_PARSE (const xmlChar *) "parse"
#define XINCLUDE_PARSE_XML (const xmlChar *) "xml"
#define XINCLUDE_PARSE_TEXT (const xmlChar *) "text"
#define XINCLUDE_PARSE_ENCODING (const xmlChar *) "encoding"
#define XINCLUDE_PARSE_XPOINTER (const xmlChar *) "xpointer"

typedef struct _xmlXIncludeCtxt xmlXIncludeCtxt;
typedef xmlXIncludeCtxt *xmlXIncludeCtxtPtr;

XMLPUBFUN int XMLCALL
		xmlXIncludeProcess	(xmlDocPtr doc);
XMLPUBFUN int XMLCALL
		xmlXIncludeProcessFlags	(xmlDocPtr doc,
					 int flags);
XMLPUBFUN int XMLCALL
		xmlXIncludeProcessFlagsData(xmlDocPtr doc,
					 int flags,
					 void *data);
XMLPUBFUN int XMLCALL
                xmlXIncludeProcessTreeFlagsData(xmlNodePtr tree,
                                         int flags,
                                         void *data);
XMLPUBFUN int XMLCALL
		xmlXIncludeProcessTree	(xmlNodePtr tree);
XMLPUBFUN int XMLCALL
		xmlXIncludeProcessTreeFlags(xmlNodePtr tree,
					 int flags);
XMLPUBFUN xmlXIncludeCtxtPtr XMLCALL
		xmlXIncludeNewContext	(xmlDocPtr doc);
XMLPUBFUN int XMLCALL
		xmlXIncludeSetFlags	(xmlXIncludeCtxtPtr ctxt,
					 int flags);
XMLPUBFUN void XMLCALL
		xmlXIncludeFreeContext	(xmlXIncludeCtxtPtr ctxt);
XMLPUBFUN int XMLCALL
		xmlXIncludeProcessNode	(xmlXIncludeCtxtPtr ctxt,
					 xmlNodePtr tree);
#ifdef __cplusplus
}
#endif

#endif 

#endif 
