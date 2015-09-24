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

#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>

#include "exslt.h"

static void
exsltSetsDifferenceFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr arg1, arg2, ret;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    arg2 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    arg1 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    ret = xmlXPathDifference(arg1, arg2);

    if (ret != arg1)
	xmlXPathFreeNodeSet(arg1);
    xmlXPathFreeNodeSet(arg2);

    xmlXPathReturnNodeSet(ctxt, ret);
}

static void
exsltSetsIntersectionFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr arg1, arg2, ret;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    arg2 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    arg1 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    ret = xmlXPathIntersection(arg1, arg2);

    xmlXPathFreeNodeSet(arg1);
    xmlXPathFreeNodeSet(arg2);

    xmlXPathReturnNodeSet(ctxt, ret);
}

static void
exsltSetsDistinctFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr ns, ret;
    int boolval = 0;
    void *user = NULL;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (ctxt->value != NULL) {
        boolval = ctxt->value->boolval;
	user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = NULL;
    }
    ns = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    
    ret = xmlXPathDistinctSorted(ns);

	if (ret != ns)
		xmlXPathFreeNodeSet(ns);

    obj = xmlXPathWrapNodeSet(ret);
    obj->user = user;
    obj->boolval = boolval;
    valuePush((ctxt), obj);
}

static void
exsltSetsHasSameNodesFunction (xmlXPathParserContextPtr ctxt,
			      int nargs) {
    xmlNodeSetPtr arg1, arg2;
    int ret;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    arg2 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    arg1 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    ret = xmlXPathHasSameNodes(arg1, arg2);

    xmlXPathFreeNodeSet(arg1);
    xmlXPathFreeNodeSet(arg2);

    xmlXPathReturnBoolean(ctxt, ret);
}

static void
exsltSetsLeadingFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr arg1, arg2, ret;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    arg2 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    arg1 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    if (xmlXPathNodeSetIsEmpty(arg2)) {
	xmlXPathReturnNodeSet(ctxt, arg1);

	xmlXPathFreeNodeSet(arg2);

	return;
    }
    
    ret = xmlXPathNodeLeadingSorted(arg1, xmlXPathNodeSetItem(arg2, 0));

    xmlXPathFreeNodeSet(arg1);
    xmlXPathFreeNodeSet(arg2);

    xmlXPathReturnNodeSet(ctxt, ret);
}

static void
exsltSetsTrailingFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr arg1, arg2, ret;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    arg2 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    arg1 = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    if (xmlXPathNodeSetIsEmpty(arg2)) {
	xmlXPathReturnNodeSet(ctxt, arg1);

	xmlXPathFreeNodeSet(arg2);

	return;
    }
    
    ret = xmlXPathNodeTrailingSorted(arg1, xmlXPathNodeSetItem(arg2, 0));

    xmlXPathFreeNodeSet(arg1);
    xmlXPathFreeNodeSet(arg2);

    xmlXPathReturnNodeSet(ctxt, ret);
}


void
exsltSetsRegister (void) {
    xsltRegisterExtModuleFunction ((const xmlChar *) "difference",
				   EXSLT_SETS_NAMESPACE,
				   exsltSetsDifferenceFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "intersection",
				   EXSLT_SETS_NAMESPACE,
				   exsltSetsIntersectionFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "distinct",
				   EXSLT_SETS_NAMESPACE,
				   exsltSetsDistinctFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "has-same-node",
				   EXSLT_SETS_NAMESPACE,
				   exsltSetsHasSameNodesFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "leading",
				   EXSLT_SETS_NAMESPACE,
				   exsltSetsLeadingFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "trailing",
				   EXSLT_SETS_NAMESPACE,
				   exsltSetsTrailingFunction);
}

int
exsltSetsXpathCtxtRegister (xmlXPathContextPtr ctxt, const xmlChar *prefix)
{
    if (ctxt
        && prefix
        && !xmlXPathRegisterNs(ctxt,
                               prefix,
                               (const xmlChar *) EXSLT_SETS_NAMESPACE)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "difference",
                                   (const xmlChar *) EXSLT_SETS_NAMESPACE,
                                   exsltSetsDifferenceFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "intersection",
                                   (const xmlChar *) EXSLT_SETS_NAMESPACE,
                                   exsltSetsIntersectionFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "distinct",
                                   (const xmlChar *) EXSLT_SETS_NAMESPACE,
                                   exsltSetsDistinctFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "has-same-node",
                                   (const xmlChar *) EXSLT_SETS_NAMESPACE,
                                   exsltSetsHasSameNodesFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "leading",
                                   (const xmlChar *) EXSLT_SETS_NAMESPACE,
                                   exsltSetsLeadingFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "trailing",
                                   (const xmlChar *) EXSLT_SETS_NAMESPACE,
                                   exsltSetsTrailingFunction)) {
        return 0;
    }
    return -1;
}
