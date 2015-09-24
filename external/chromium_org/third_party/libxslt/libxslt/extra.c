/*
 * extra.c: Implementation of non-standard features
 *
 * Reference:
 *   Michael Kay "XSLT Programmer's Reference" pp 637-643
 *   The node-set() extension function
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXSLT
#include "libxslt.h"

#include <string.h>
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/parserInternals.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "extensions.h"
#include "variables.h"
#include "transform.h"
#include "extra.h"
#include "preproc.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_EXTRA
#endif


void
xsltDebug(xsltTransformContextPtr ctxt, xmlNodePtr node ATTRIBUTE_UNUSED,
          xmlNodePtr inst ATTRIBUTE_UNUSED,
          xsltStylePreCompPtr comp ATTRIBUTE_UNUSED)
{
    int i, j;

    xsltGenericError(xsltGenericErrorContext, "Templates:\n");
    for (i = 0, j = ctxt->templNr - 1; ((i < 15) && (j >= 0)); i++, j--) {
        xsltGenericError(xsltGenericErrorContext, "#%d ", i);
        if (ctxt->templTab[j]->name != NULL)
            xsltGenericError(xsltGenericErrorContext, "name %s ",
                             ctxt->templTab[j]->name);
        if (ctxt->templTab[j]->match != NULL)
            xsltGenericError(xsltGenericErrorContext, "name %s ",
                             ctxt->templTab[j]->match);
        if (ctxt->templTab[j]->mode != NULL)
            xsltGenericError(xsltGenericErrorContext, "name %s ",
                             ctxt->templTab[j]->mode);
        xsltGenericError(xsltGenericErrorContext, "\n");
    }
    xsltGenericError(xsltGenericErrorContext, "Variables:\n");
    for (i = 0, j = ctxt->varsNr - 1; ((i < 15) && (j >= 0)); i++, j--) {
        xsltStackElemPtr cur;

        if (ctxt->varsTab[j] == NULL)
            continue;
        xsltGenericError(xsltGenericErrorContext, "#%d\n", i);
        cur = ctxt->varsTab[j];
        while (cur != NULL) {
            if (cur->comp == NULL) {
                xsltGenericError(xsltGenericErrorContext,
                                 "corrupted !!!\n");
            } else if (cur->comp->type == XSLT_FUNC_PARAM) {
                xsltGenericError(xsltGenericErrorContext, "param ");
            } else if (cur->comp->type == XSLT_FUNC_VARIABLE) {
                xsltGenericError(xsltGenericErrorContext, "var ");
            }
            if (cur->name != NULL)
                xsltGenericError(xsltGenericErrorContext, "%s ",
                                 cur->name);
            else
                xsltGenericError(xsltGenericErrorContext, "noname !!!!");
#ifdef LIBXML_DEBUG_ENABLED
            if (cur->value != NULL) {
                xmlXPathDebugDumpObject(stdout, cur->value, 1);
            } else {
                xsltGenericError(xsltGenericErrorContext, "NULL !!!!");
            }
#endif
            xsltGenericError(xsltGenericErrorContext, "\n");
            cur = cur->next;
        }

    }
}


void
xsltFunctionNodeSet(xmlXPathParserContextPtr ctxt, int nargs){
    if (nargs != 1) {
	xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
		"node-set() : expects one result-tree arg\n");
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_XSLT_TREE) &&
	 (ctxt->value->type != XPATH_NODESET))) {
	xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
	    "node-set() invalid arg expecting a result tree\n");
	ctxt->error = XPATH_INVALID_TYPE;
	return;
    }
    if (ctxt->value->type == XPATH_XSLT_TREE) {
	ctxt->value->type = XPATH_NODESET;
    }
}


#if defined(linux) || defined(__sun)
#if defined(HAVE_MKTIME) && defined(HAVE_LOCALTIME) && defined(HAVE_ASCTIME)
#define WITH_LOCALTIME

static void
xsltFunctionLocalTime(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr obj;
    char *str;
    char digits[5];
    char result[29];
    long int field;
    time_t gmt, lmt;
    struct tm gmt_tm;
    struct tm *local_tm;
 
    if (nargs != 1) {
       xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                      "localTime() : invalid number of args %d\n", nargs);
       ctxt->error = XPATH_INVALID_ARITY;
       return;
    }
 
    obj = valuePop(ctxt);

    if (obj->type != XPATH_STRING) {
	obj = xmlXPathConvertString(obj);
    }
    if (obj == NULL) {
	valuePush(ctxt, xmlXPathNewString((const xmlChar *)""));
	return;
    }
    
    str = (char *) obj->stringval;

    
    memset(digits, 0, sizeof(digits));
    strncpy(digits, str+7, 4);
    field = strtol(digits, NULL, 10);
    gmt_tm.tm_year = field - 1900;

    memset(digits, 0, sizeof(digits));
    strncpy(digits, str+12, 2);
    field = strtol(digits, NULL, 10);
    gmt_tm.tm_mon = field - 1;

    memset(digits, 0, sizeof(digits));
    strncpy(digits, str+15, 2);
    field = strtol(digits, NULL, 10);
    gmt_tm.tm_mday = field;

    memset(digits, 0, sizeof(digits));
    strncpy(digits, str+18, 2);
    field = strtol(digits, NULL, 10);
    gmt_tm.tm_hour = field;

    memset(digits, 0, sizeof(digits));
    strncpy(digits, str+21, 2);
    field = strtol(digits, NULL, 10);
    gmt_tm.tm_min = field;

    memset(digits, 0, sizeof(digits));
    strncpy(digits, str+24, 2);
    field = strtol(digits, NULL, 10);
    gmt_tm.tm_sec = field;

    
    gmt = mktime(&gmt_tm);


    local_tm = localtime(&gmt);

    lmt = gmt - timezone;

    local_tm = localtime(&lmt);


    str = asctime(local_tm);           
                                       

    memset(result, 0, sizeof(result)); 
                                       

    strncpy(result, str, 20);
    strcpy(result+20, "???");          
    strncpy(result+23, str+19, 5);

    
    valuePush(ctxt, xmlXPathNewString((xmlChar *)result));
}
#endif
#endif 


void
xsltRegisterExtras(xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED) {
    xsltRegisterAllExtras();
}

void
xsltRegisterAllExtras (void) {
    xsltRegisterExtModuleFunction((const xmlChar *) "node-set",
				  XSLT_LIBXSLT_NAMESPACE,
				  xsltFunctionNodeSet);
    xsltRegisterExtModuleFunction((const xmlChar *) "node-set",
				  XSLT_SAXON_NAMESPACE,
				  xsltFunctionNodeSet);
    xsltRegisterExtModuleFunction((const xmlChar *) "node-set",
				  XSLT_XT_NAMESPACE,
				  xsltFunctionNodeSet);
#ifdef WITH_LOCALTIME
    xsltRegisterExtModuleFunction((const xmlChar *) "localTime",
				  XSLT_NORM_SAXON_NAMESPACE,
				  xsltFunctionLocalTime);
#endif
    xsltRegisterExtModuleElement((const xmlChar *) "debug",
				 XSLT_LIBXSLT_NAMESPACE,
				 NULL,
				 (xsltTransformFunction) xsltDebug);
    xsltRegisterExtModuleElement((const xmlChar *) "output",
				 XSLT_SAXON_NAMESPACE,
				 xsltDocumentComp,
				 (xsltTransformFunction) xsltDocumentElem);
    xsltRegisterExtModuleElement((const xmlChar *) "write",
				 XSLT_XALAN_NAMESPACE,
				 xsltDocumentComp,
				 (xsltTransformFunction) xsltDocumentElem);
    xsltRegisterExtModuleElement((const xmlChar *) "document",
				 XSLT_XT_NAMESPACE,
				 xsltDocumentComp,
				 (xsltTransformFunction) xsltDocumentElem);
    xsltRegisterExtModuleElement((const xmlChar *) "document",
				 XSLT_NAMESPACE,
				 xsltDocumentComp,
				 (xsltTransformFunction) xsltDocumentElem);
}
