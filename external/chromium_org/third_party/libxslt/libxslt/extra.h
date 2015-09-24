/*
 * Summary: interface for the non-standard features
 * Description: implement some extension outside the XSLT namespace
 *              but not EXSLT with is in a different library.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLT_EXTRA_H__
#define __XML_XSLT_EXTRA_H__

#include <libxml/xpath.h>
#include "xsltexports.h"
#include "xsltInternals.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XSLT_LIBXSLT_NAMESPACE ((xmlChar *) "http://xmlsoft.org/XSLT/namespace")

#define XSLT_SAXON_NAMESPACE ((xmlChar *) "http://icl.com/saxon")

#define XSLT_XT_NAMESPACE ((xmlChar *) "http://www.jclark.com/xt")

#define XSLT_XALAN_NAMESPACE ((xmlChar *)	\
	                        "org.apache.xalan.xslt.extensions.Redirect")

#define XSLT_NORM_SAXON_NAMESPACE ((xmlChar *)	\
	"http://nwalsh.com/xslt/ext/com.nwalsh.saxon.CVS")


XSLTPUBFUN void XSLTCALL
		xsltFunctionNodeSet	(xmlXPathParserContextPtr ctxt,
					 int nargs);
XSLTPUBFUN void XSLTCALL		
		xsltDebug		(xsltTransformContextPtr ctxt,
					 xmlNodePtr node,
					 xmlNodePtr inst,
					 xsltStylePreCompPtr comp);


XSLTPUBFUN void XSLTCALL		
		xsltRegisterExtras	(xsltTransformContextPtr ctxt);
XSLTPUBFUN void XSLTCALL		
		xsltRegisterAllExtras	(void);

#ifdef __cplusplus
}
#endif

#endif 

