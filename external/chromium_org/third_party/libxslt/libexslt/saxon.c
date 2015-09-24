#define IN_LIBEXSLT
#include "libexslt/libexslt.h"

#if defined(WIN32) && !defined (__CYGWIN__) && (!__MINGW32__)
#include <win32config.h>
#else
#include "config.h"
#endif

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parser.h>
#include <libxml/hash.h>

#include <libxslt/xsltconfig.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>

#include "exslt.h"

static xmlHashTablePtr
exsltSaxonInit (xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		const xmlChar *URI ATTRIBUTE_UNUSED) {
    return xmlHashCreate(1);
}

static void
exsltSaxonShutdown (xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		    const xmlChar *URI ATTRIBUTE_UNUSED,
		    xmlHashTablePtr data) {
    xmlHashFree(data, (xmlHashDeallocator) xmlXPathFreeCompExpr);
}


static void
exsltSaxonExpressionFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlChar *arg;
    xmlXPathCompExprPtr ret;
    xmlHashTablePtr hash;
    xsltTransformContextPtr tctxt = xsltXPathGetTransformContext(ctxt);

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    arg = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt) || (arg == NULL)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    hash = (xmlHashTablePtr) xsltGetExtData(tctxt,
					    ctxt->context->functionURI);

    ret = xmlHashLookup(hash, arg);

    if (ret == NULL) {
	 ret = xmlXPathCompile(arg);
	 if (ret == NULL) {
	      xmlFree(arg);
	      xsltGenericError(xsltGenericErrorContext,
			"{%s}:%s: argument is not an XPath expression\n",
			ctxt->context->functionURI, ctxt->context->function);
	      return;
	 }
	 xmlHashAddEntry(hash, arg, (void *) ret);
    }

    xmlFree(arg);

    xmlXPathReturnExternal(ctxt, ret);
}

static void
exsltSaxonEvalFunction (xmlXPathParserContextPtr ctxt, int nargs) {
     xmlXPathCompExprPtr expr;
     xmlXPathObjectPtr ret;

     if (nargs != 1) {
	  xmlXPathSetArityError(ctxt);
	  return;
     }

     if (!xmlXPathStackIsExternal(ctxt)) {
	  xmlXPathSetTypeError(ctxt);
	  return;
     }

     expr = (xmlXPathCompExprPtr) xmlXPathPopExternal(ctxt);

     ret = xmlXPathCompiledEval(expr, ctxt->context);

     valuePush(ctxt, ret);
}

/**
 * exsltSaxonEvaluateFunction:
 * @ctxt:  an XPath parser context
 * @nargs: number of arguments
 *
 * Implements the SAXON evaluate() function
 *     object saxon:evaluate (string)
 * The supplied string must contain an XPath expression. The result of
 * the function is the result of evaluating the XPath expression. This
 * is useful where an expression needs to be constructed at run-time or
 * passed to the stylesheet as a parameter, for example where the sort
 * key is determined dynamically. The context for the expression (e.g.
 * which variables and namespaces are available) is exactly the same as
 * if the expression were written explicitly at this point in the
 * stylesheet. The function saxon:evaluate(string) is shorthand for
 * saxon:eval(saxon:expression(string)).
 */
static void
exsltSaxonEvaluateFunction (xmlXPathParserContextPtr ctxt, int nargs) {
     if (nargs != 1) {
	  xmlXPathSetArityError(ctxt);
	  return;
     }

     exsltSaxonExpressionFunction(ctxt, 1);
     exsltSaxonEvalFunction(ctxt, 1);
}

static void
exsltSaxonLineNumberFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodePtr cur = NULL;

    if (nargs == 0) {
	cur = ctxt->context->node;
    } else if (nargs == 1) {
	xmlXPathObjectPtr obj;
	xmlNodeSetPtr nodelist;
	int i;

	if ((ctxt->value == NULL) || (ctxt->value->type != XPATH_NODESET)) {
	    xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
		"saxon:line-number() : invalid arg expecting a node-set\n");
	    ctxt->error = XPATH_INVALID_TYPE;
	    return;
	}

	obj = valuePop(ctxt);
	nodelist = obj->nodesetval;
	if ((nodelist == NULL) || (nodelist->nodeNr <= 0)) {
	    xmlXPathFreeObject(obj);
	    valuePush(ctxt, xmlXPathNewFloat(-1));
	    return;
	}
	cur = nodelist->nodeTab[0];
	for (i = 1;i < nodelist->nodeNr;i++) {
	    int ret = xmlXPathCmpNodes(cur, nodelist->nodeTab[i]);
	    if (ret == -1)
		cur = nodelist->nodeTab[i];
	}
	xmlXPathFreeObject(obj);
    } else {
	xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
		"saxon:line-number() : invalid number of args %d\n",
		nargs);
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }

    valuePush(ctxt, xmlXPathNewFloat(xmlGetLineNo(cur)));
    return;
}

void
exsltSaxonRegister (void) {
     xsltRegisterExtModule (SAXON_NAMESPACE,
			    (xsltExtInitFunction) exsltSaxonInit,
			    (xsltExtShutdownFunction) exsltSaxonShutdown);
     xsltRegisterExtModuleFunction((const xmlChar *) "expression",
				   SAXON_NAMESPACE,
				   exsltSaxonExpressionFunction);
     xsltRegisterExtModuleFunction((const xmlChar *) "eval",
				   SAXON_NAMESPACE,
				   exsltSaxonEvalFunction);
     xsltRegisterExtModuleFunction((const xmlChar *) "evaluate",
				   SAXON_NAMESPACE,
				   exsltSaxonEvaluateFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "line-number",
				   SAXON_NAMESPACE,
				   exsltSaxonLineNumberFunction);
}
