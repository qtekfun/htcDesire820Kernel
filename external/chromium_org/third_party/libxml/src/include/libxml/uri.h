/**
 * Summary: library of generic URI related routines
 * Description: library of generic URI related routines
 *              Implements RFC 2396
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_URI_H__
#define __XML_URI_H__

#include <libxml/xmlversion.h>
#include <libxml/tree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xmlURI xmlURI;
typedef xmlURI *xmlURIPtr;
struct _xmlURI {
    char *scheme;	
    char *opaque;	
    char *authority;	
    char *server;	
    char *user;		
    int port;		
    char *path;		
    char *query;	
    char *fragment;	
    int  cleanup;	
    char *query_raw;	
};

XMLPUBFUN xmlURIPtr XMLCALL
		xmlCreateURI		(void);
XMLPUBFUN xmlChar * XMLCALL
		xmlBuildURI		(const xmlChar *URI,
					 const xmlChar *base);
XMLPUBFUN xmlChar * XMLCALL
		xmlBuildRelativeURI	(const xmlChar *URI,
					 const xmlChar *base);
XMLPUBFUN xmlURIPtr XMLCALL
		xmlParseURI		(const char *str);
XMLPUBFUN xmlURIPtr XMLCALL
		xmlParseURIRaw		(const char *str,
					 int raw);
XMLPUBFUN int XMLCALL
		xmlParseURIReference	(xmlURIPtr uri,
					 const char *str);
XMLPUBFUN xmlChar * XMLCALL
		xmlSaveUri		(xmlURIPtr uri);
XMLPUBFUN void XMLCALL
		xmlPrintURI		(FILE *stream,
					 xmlURIPtr uri);
XMLPUBFUN xmlChar * XMLCALL
		xmlURIEscapeStr         (const xmlChar *str,
					 const xmlChar *list);
XMLPUBFUN char * XMLCALL
		xmlURIUnescapeString	(const char *str,
					 int len,
					 char *target);
XMLPUBFUN int XMLCALL
		xmlNormalizeURIPath	(char *path);
XMLPUBFUN xmlChar * XMLCALL
		xmlURIEscape		(const xmlChar *str);
XMLPUBFUN void XMLCALL
		xmlFreeURI		(xmlURIPtr uri);
XMLPUBFUN xmlChar* XMLCALL
		xmlCanonicPath		(const xmlChar *path);
XMLPUBFUN xmlChar* XMLCALL
		xmlPathToURI		(const xmlChar *path);

#ifdef __cplusplus
}
#endif
#endif 
