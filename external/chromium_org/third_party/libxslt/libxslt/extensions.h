/*
 * Summary: interface for the extension support
 * Description: This provide the API needed for simple and module
 *              extension support.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLT_EXTENSION_H__
#define __XML_XSLT_EXTENSION_H__

#include <libxml/xpath.h>
#include "xsltexports.h"
#include "xsltInternals.h"

#ifdef __cplusplus
extern "C" {
#endif



XSLTPUBFUN void XSLTCALL
		xsltInitGlobals                 (void);

typedef void * (*xsltStyleExtInitFunction)	(xsltStylesheetPtr style,
						 const xmlChar *URI);

typedef void (*xsltStyleExtShutdownFunction)	(xsltStylesheetPtr style,
						 const xmlChar *URI,
						 void *data);

typedef void * (*xsltExtInitFunction)	(xsltTransformContextPtr ctxt,
					 const xmlChar *URI);

typedef void (*xsltExtShutdownFunction) (xsltTransformContextPtr ctxt,
					 const xmlChar *URI,
					 void *data);

XSLTPUBFUN int XSLTCALL
		xsltRegisterExtModule	(const xmlChar *URI,
					 xsltExtInitFunction initFunc,
					 xsltExtShutdownFunction shutdownFunc);
XSLTPUBFUN int XSLTCALL
		xsltRegisterExtModuleFull
					(const xmlChar * URI,
					 xsltExtInitFunction initFunc,
					 xsltExtShutdownFunction shutdownFunc,
					 xsltStyleExtInitFunction styleInitFunc,
					 xsltStyleExtShutdownFunction styleShutdownFunc);

XSLTPUBFUN int XSLTCALL
		xsltUnregisterExtModule	(const xmlChar * URI);

XSLTPUBFUN void * XSLTCALL
		xsltGetExtData		(xsltTransformContextPtr ctxt,
					 const xmlChar *URI);

XSLTPUBFUN void * XSLTCALL
		xsltStyleGetExtData	(xsltStylesheetPtr style,
					 const xmlChar *URI);
#ifdef XSLT_REFACTORED
XSLTPUBFUN void * XSLTCALL
		xsltStyleStylesheetLevelGetExtData(
					 xsltStylesheetPtr style,
					 const xmlChar * URI);
#endif
XSLTPUBFUN void XSLTCALL
		xsltShutdownCtxtExts	(xsltTransformContextPtr ctxt);

XSLTPUBFUN void XSLTCALL
		xsltShutdownExts	(xsltStylesheetPtr style);

XSLTPUBFUN xsltTransformContextPtr XSLTCALL
		xsltXPathGetTransformContext
					(xmlXPathParserContextPtr ctxt);

XSLTPUBFUN int XSLTCALL
		xsltRegisterExtModuleFunction
					(const xmlChar *name,
					 const xmlChar *URI,
					 xmlXPathFunction function);
XSLTPUBFUN xmlXPathFunction XSLTCALL
	xsltExtModuleFunctionLookup	(const xmlChar *name,
					 const xmlChar *URI);
XSLTPUBFUN int XSLTCALL
		xsltUnregisterExtModuleFunction
					(const xmlChar *name,
					 const xmlChar *URI);

typedef xsltElemPreCompPtr (*xsltPreComputeFunction)
					(xsltStylesheetPtr style,
					 xmlNodePtr inst,
					 xsltTransformFunction function);

XSLTPUBFUN xsltElemPreCompPtr XSLTCALL
		xsltNewElemPreComp	(xsltStylesheetPtr style,
					 xmlNodePtr inst,
					 xsltTransformFunction function);
XSLTPUBFUN void XSLTCALL
		xsltInitElemPreComp	(xsltElemPreCompPtr comp,
					 xsltStylesheetPtr style,
					 xmlNodePtr inst,
					 xsltTransformFunction function,
					 xsltElemPreCompDeallocator freeFunc);

XSLTPUBFUN int XSLTCALL
		xsltRegisterExtModuleElement
					(const xmlChar *name,
					 const xmlChar *URI,
					 xsltPreComputeFunction precomp,
					 xsltTransformFunction transform);
XSLTPUBFUN xsltTransformFunction XSLTCALL
		xsltExtElementLookup	(xsltTransformContextPtr ctxt,
					 const xmlChar *name,
					 const xmlChar *URI);
XSLTPUBFUN xsltTransformFunction XSLTCALL
		xsltExtModuleElementLookup
					(const xmlChar *name,
					 const xmlChar *URI);
XSLTPUBFUN xsltPreComputeFunction XSLTCALL
		xsltExtModuleElementPreComputeLookup
					(const xmlChar *name,
					 const xmlChar *URI);
XSLTPUBFUN int XSLTCALL
		xsltUnregisterExtModuleElement
					(const xmlChar *name,
					 const xmlChar *URI);

typedef void (*xsltTopLevelFunction)	(xsltStylesheetPtr style,
					 xmlNodePtr inst);

XSLTPUBFUN int XSLTCALL
		xsltRegisterExtModuleTopLevel
					(const xmlChar *name,
					 const xmlChar *URI,
					 xsltTopLevelFunction function);
XSLTPUBFUN xsltTopLevelFunction XSLTCALL
		xsltExtModuleTopLevelLookup
					(const xmlChar *name,
					 const xmlChar *URI);
XSLTPUBFUN int XSLTCALL
		xsltUnregisterExtModuleTopLevel
					(const xmlChar *name,
					 const xmlChar *URI);


XSLTPUBFUN int XSLTCALL
		xsltRegisterExtFunction	(xsltTransformContextPtr ctxt,
					 const xmlChar *name,
					 const xmlChar *URI,
					 xmlXPathFunction function);
XSLTPUBFUN int XSLTCALL
		xsltRegisterExtElement	(xsltTransformContextPtr ctxt,
					 const xmlChar *name,
					 const xmlChar *URI,
					 xsltTransformFunction function);


XSLTPUBFUN int XSLTCALL
		xsltRegisterExtPrefix	(xsltStylesheetPtr style,
					 const xmlChar *prefix,
					 const xmlChar *URI);
XSLTPUBFUN int XSLTCALL
		xsltCheckExtPrefix	(xsltStylesheetPtr style,
					 const xmlChar *URI);
XSLTPUBFUN int XSLTCALL
		xsltCheckExtURI		(xsltStylesheetPtr style,
					 const xmlChar *URI);
XSLTPUBFUN int XSLTCALL
		xsltInitCtxtExts	(xsltTransformContextPtr ctxt);
XSLTPUBFUN void XSLTCALL
		xsltFreeCtxtExts	(xsltTransformContextPtr ctxt);
XSLTPUBFUN void XSLTCALL
		xsltFreeExts		(xsltStylesheetPtr style);

XSLTPUBFUN xsltElemPreCompPtr XSLTCALL
		xsltPreComputeExtModuleElement
					(xsltStylesheetPtr style,
					 xmlNodePtr inst);

XSLTPUBFUN xmlHashTablePtr XSLTCALL
		xsltGetExtInfo		(xsltStylesheetPtr style,
					 const xmlChar *URI);

XSLTPUBFUN void XSLTCALL
		xsltRegisterTestModule	(void);
XSLTPUBFUN void XSLTCALL
		xsltDebugDumpExtensions	(FILE * output);


#ifdef __cplusplus
}
#endif

#endif 

