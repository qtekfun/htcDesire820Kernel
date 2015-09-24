/*
 * Summary: interface for the libxslt security framework
 * Description: the libxslt security framework allow to restrict
 *              the access to new resources (file or URL) from
 *              the stylesheet at runtime.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLT_SECURITY_H__
#define __XML_XSLT_SECURITY_H__

#include <libxml/tree.h>
#include "xsltexports.h"
#include "xsltInternals.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xsltSecurityPrefs xsltSecurityPrefs;
typedef xsltSecurityPrefs *xsltSecurityPrefsPtr;

typedef enum {
    XSLT_SECPREF_READ_FILE = 1,
    XSLT_SECPREF_WRITE_FILE,
    XSLT_SECPREF_CREATE_DIRECTORY,
    XSLT_SECPREF_READ_NETWORK,
    XSLT_SECPREF_WRITE_NETWORK
} xsltSecurityOption;

typedef int (*xsltSecurityCheck)	(xsltSecurityPrefsPtr sec,
					 xsltTransformContextPtr ctxt,
					 const char *value);

XSLTPUBFUN xsltSecurityPrefsPtr XSLTCALL	
		    xsltNewSecurityPrefs	(void);
XSLTPUBFUN void XSLTCALL			
		    xsltFreeSecurityPrefs	(xsltSecurityPrefsPtr sec);
XSLTPUBFUN int XSLTCALL			
		    xsltSetSecurityPrefs	(xsltSecurityPrefsPtr sec,
						 xsltSecurityOption option,
						 xsltSecurityCheck func);
XSLTPUBFUN xsltSecurityCheck XSLTCALL	
		    xsltGetSecurityPrefs	(xsltSecurityPrefsPtr sec,
						 xsltSecurityOption option);

XSLTPUBFUN void XSLTCALL			
		    xsltSetDefaultSecurityPrefs	(xsltSecurityPrefsPtr sec);
XSLTPUBFUN xsltSecurityPrefsPtr XSLTCALL	
		    xsltGetDefaultSecurityPrefs	(void);

XSLTPUBFUN int XSLTCALL			
		    xsltSetCtxtSecurityPrefs	(xsltSecurityPrefsPtr sec,
						 xsltTransformContextPtr ctxt);

XSLTPUBFUN int XSLTCALL			
		    xsltSecurityAllow		(xsltSecurityPrefsPtr sec,
						 xsltTransformContextPtr ctxt,
						 const char *value);
XSLTPUBFUN int XSLTCALL	
		    xsltSecurityForbid		(xsltSecurityPrefsPtr sec,
						 xsltTransformContextPtr ctxt,
						 const char *value);
XSLTPUBFUN int XSLTCALL			
		    xsltCheckWrite		(xsltSecurityPrefsPtr sec,
						 xsltTransformContextPtr ctxt,
						 const xmlChar *URL);
XSLTPUBFUN int XSLTCALL			
		    xsltCheckRead		(xsltSecurityPrefsPtr sec,
						 xsltTransformContextPtr ctxt,
						 const xmlChar *URL);

#ifdef __cplusplus
}
#endif

#endif 

