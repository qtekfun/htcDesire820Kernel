/*
 * xsltutils.c: Utilities for the XSL Transformation 1.0 engine
 *
 * Reference:
 *   http://www.w3.org/TR/1999/REC-xslt-19991116
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXSLT
#include "libxslt.h"

#ifndef	XSLT_NEED_TRIO
#include <stdio.h>
#else
#include <trio.h>
#endif

#include <string.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <stdarg.h>

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlIO.h>
#include "xsltutils.h"
#include "templates.h"
#include "xsltInternals.h"
#include "imports.h"
#include "transform.h"

#if defined(WIN32) && !defined(__CYGWIN__)
#ifdef _MSC_VER
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define gettimeofday(p1,p2)
#define HAVE_GETTIMEOFDAY
#define XSLT_WIN32_PERFORMANCE_COUNTER
#endif 
#endif 


const xmlChar *
xsltGetCNsProp(xsltStylesheetPtr style, xmlNodePtr node,
              const xmlChar *name, const xmlChar *nameSpace) {
    xmlAttrPtr prop;
    xmlDocPtr doc;
    xmlNsPtr ns;
    xmlChar *tmp;
    const xmlChar *ret;

    if ((node == NULL) || (style == NULL) || (style->dict == NULL))
	return(NULL);

    prop = node->properties;
    if (nameSpace == NULL) {
        return xmlGetProp(node, name);
    }
    while (prop != NULL) {
        if ((xmlStrEqual(prop->name, name)) &&
	    (((prop->ns == NULL) && (node->ns != NULL) &&
	      (xmlStrEqual(node->ns->href, nameSpace))) ||
	     ((prop->ns != NULL) &&
	      (xmlStrEqual(prop->ns->href, nameSpace))))) {

	    tmp = xmlNodeListGetString(node->doc, prop->children, 1);
	    if (tmp == NULL)
	        ret = xmlDictLookup(style->dict, BAD_CAST "", 0);
	    else {
	        ret = xmlDictLookup(style->dict, tmp, -1);
		xmlFree(tmp);
	    }
	    return ret;
        }
	prop = prop->next;
    }
    tmp = NULL;
    doc =  node->doc;
    if (doc != NULL) {
        if (doc->intSubset != NULL) {
	    xmlAttributePtr attrDecl;

	    attrDecl = xmlGetDtdAttrDesc(doc->intSubset, node->name, name);
	    if ((attrDecl == NULL) && (doc->extSubset != NULL))
		attrDecl = xmlGetDtdAttrDesc(doc->extSubset, node->name, name);
		
	    if ((attrDecl != NULL) && (attrDecl->prefix != NULL)) {
		ns = xmlSearchNs(doc, node, attrDecl->prefix);
		if ((ns != NULL) && (xmlStrEqual(ns->href, nameSpace)))
		    return(xmlDictLookup(style->dict,
		                         attrDecl->defaultValue, -1));
	    }
	}
    }
    return(NULL);
}
xmlChar *
xsltGetNsProp(xmlNodePtr node, const xmlChar *name, const xmlChar *nameSpace) {
    xmlAttrPtr prop;
    xmlDocPtr doc;
    xmlNsPtr ns;

    if (node == NULL)
	return(NULL);

    prop = node->properties;
    if (nameSpace == NULL)
	return(xmlGetProp(node, name));
    while (prop != NULL) {
        if ((xmlStrEqual(prop->name, name)) &&
	    (((prop->ns == NULL) && (node->ns != NULL) &&
	      (xmlStrEqual(node->ns->href, nameSpace))) ||
	     ((prop->ns != NULL) &&
	      (xmlStrEqual(prop->ns->href, nameSpace))))) {
	    xmlChar *ret;

	    ret = xmlNodeListGetString(node->doc, prop->children, 1);
	    if (ret == NULL) return(xmlStrdup((xmlChar *)""));
	    return(ret);
        }
	prop = prop->next;
    }

    doc =  node->doc;
    if (doc != NULL) {
        if (doc->intSubset != NULL) {
	    xmlAttributePtr attrDecl;

	    attrDecl = xmlGetDtdAttrDesc(doc->intSubset, node->name, name);
	    if ((attrDecl == NULL) && (doc->extSubset != NULL))
		attrDecl = xmlGetDtdAttrDesc(doc->extSubset, node->name, name);
		
	    if ((attrDecl != NULL) && (attrDecl->prefix != NULL)) {
		ns = xmlSearchNs(doc, node, attrDecl->prefix);
		if ((ns != NULL) && (xmlStrEqual(ns->href, nameSpace)))
		    return(xmlStrdup(attrDecl->defaultValue));
	    }
	}
    }
    return(NULL);
}

int
xsltGetUTF8Char(const unsigned char *utf, int *len) {
    unsigned int c;

    if (utf == NULL)
	goto error;
    if (len == NULL)
	goto error;
    if (*len < 1)
	goto error;

    c = utf[0];
    if (c & 0x80) {
	if (*len < 2)
	    goto error;
	if ((utf[1] & 0xc0) != 0x80)
	    goto error;
	if ((c & 0xe0) == 0xe0) {
	    if (*len < 3)
		goto error;
	    if ((utf[2] & 0xc0) != 0x80)
		goto error;
	    if ((c & 0xf0) == 0xf0) {
		if (*len < 4)
		    goto error;
		if ((c & 0xf8) != 0xf0 || (utf[3] & 0xc0) != 0x80)
		    goto error;
		*len = 4;
		
		c = (utf[0] & 0x7) << 18;
		c |= (utf[1] & 0x3f) << 12;
		c |= (utf[2] & 0x3f) << 6;
		c |= utf[3] & 0x3f;
	    } else {
	      
		*len = 3;
		c = (utf[0] & 0xf) << 12;
		c |= (utf[1] & 0x3f) << 6;
		c |= utf[2] & 0x3f;
	    }
	} else {
	  
	    *len = 2;
	    c = (utf[0] & 0x1f) << 6;
	    c |= utf[1] & 0x3f;
	}
    } else {
	
	*len = 1;
    }
    return(c);

error:
    if (len != NULL)
	*len = 0;
    return(-1);
}

#ifdef XSLT_REFACTORED

int
xsltPointerListAddSize(xsltPointerListPtr list,		       
		       void *item,
		       int initialSize)
{
    if (list->items == NULL) {
	if (initialSize <= 0)
	    initialSize = 1;
	list->items = (void **) xmlMalloc(
	    initialSize * sizeof(void *));
	if (list->items == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
	     "xsltPointerListAddSize: memory allocation failure.\n");
	    return(-1);
	}
	list->number = 0;
	list->size = initialSize;
    } else if (list->size <= list->number) {
	list->size *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->size * sizeof(void *));
	if (list->items == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
	     "xsltPointerListAddSize: memory re-allocation failure.\n");
	    list->size = 0;
	    return(-1);
	}
    }
    list->items[list->number++] = item;
    return(0);
}

xsltPointerListPtr
xsltPointerListCreate(int initialSize)
{
    xsltPointerListPtr ret;

    ret = xmlMalloc(sizeof(xsltPointerList));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
	     "xsltPointerListCreate: memory allocation failure.\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(xsltPointerList));
    if (initialSize > 0) {
	xsltPointerListAddSize(ret, NULL, initialSize);
	ret->number = 0;
    }
    return (ret);
}

void
xsltPointerListFree(xsltPointerListPtr list)
{
    if (list == NULL)
	return;
    if (list->items != NULL)
	xmlFree(list->items);
    xmlFree(list);
}

void
xsltPointerListClear(xsltPointerListPtr list)
{
    if (list->items != NULL) {
	xmlFree(list->items);
	list->items = NULL;
    }
    list->number = 0;
    list->size = 0;
}

#endif 


void
xsltMessage(xsltTransformContextPtr ctxt, xmlNodePtr node, xmlNodePtr inst) {
    xmlGenericErrorFunc error = xsltGenericError;
    void *errctx = xsltGenericErrorContext;
    xmlChar *prop, *message;
    int terminate = 0;

    if ((ctxt == NULL) || (inst == NULL))
	return;

    if (ctxt->error != NULL) {
	error = ctxt->error;
	errctx = ctxt->errctx;
    }

    prop = xmlGetNsProp(inst, (const xmlChar *)"terminate", NULL);
    if (prop != NULL) {
	if (xmlStrEqual(prop, (const xmlChar *)"yes")) {
	    terminate = 1;
	} else if (xmlStrEqual(prop, (const xmlChar *)"no")) {
	    terminate = 0;
	} else {
	    error(errctx,
		"xsl:message : terminate expecting 'yes' or 'no'\n");
	    ctxt->state = XSLT_STATE_ERROR;
	}
	xmlFree(prop);
    }
    message = xsltEvalTemplateString(ctxt, node, inst);
    if (message != NULL) {
	int len = xmlStrlen(message);

	error(errctx, "%s", (const char *)message);
	if ((len > 0) && (message[len - 1] != '\n'))
	    error(errctx, "\n");
	xmlFree(message);
    }
    if (terminate)
	ctxt->state = XSLT_STATE_STOPPED;
}


#define XSLT_GET_VAR_STR(msg, str) {				\
    int       size;						\
    int       chars;						\
    char      *larger;						\
    va_list   ap;						\
								\
    str = (char *) xmlMalloc(150);				\
    if (str == NULL) 						\
	return;							\
								\
    size = 150;							\
								\
    while (size < 64000) {					\
	va_start(ap, msg);					\
  	chars = vsnprintf(str, size, msg, ap);			\
	va_end(ap);						\
	if ((chars > -1) && (chars < size))			\
	    break;						\
	if (chars > -1)						\
	    size += chars + 1;					\
	else							\
	    size += 100;					\
	if ((larger = (char *) xmlRealloc(str, size)) == NULL) {\
	    xmlFree(str);					\
	    return;						\
	}							\
	str = larger;						\
    }								\
}
static void
xsltGenericErrorDefaultFunc(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {
    va_list args;

    if (xsltGenericErrorContext == NULL)
	xsltGenericErrorContext = (void *) stderr;

    va_start(args, msg);
    vfprintf((FILE *)xsltGenericErrorContext, msg, args);
    va_end(args);
}

xmlGenericErrorFunc xsltGenericError = xsltGenericErrorDefaultFunc;
void *xsltGenericErrorContext = NULL;


void
xsltSetGenericErrorFunc(void *ctx, xmlGenericErrorFunc handler) {
    xsltGenericErrorContext = ctx;
    if (handler != NULL)
	xsltGenericError = handler;
    else
	xsltGenericError = xsltGenericErrorDefaultFunc;
}

static void
xsltGenericDebugDefaultFunc(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...) {
    va_list args;

    if (xsltGenericDebugContext == NULL)
	return;

    va_start(args, msg);
    vfprintf((FILE *)xsltGenericDebugContext, msg, args);
    va_end(args);
}

xmlGenericErrorFunc xsltGenericDebug = xsltGenericDebugDefaultFunc;
void *xsltGenericDebugContext = NULL;


void
xsltSetGenericDebugFunc(void *ctx, xmlGenericErrorFunc handler) {
    xsltGenericDebugContext = ctx;
    if (handler != NULL)
	xsltGenericDebug = handler;
    else
	xsltGenericDebug = xsltGenericDebugDefaultFunc;
}

void
xsltPrintErrorContext(xsltTransformContextPtr ctxt,
	              xsltStylesheetPtr style, xmlNodePtr node) {
    int line = 0;
    const xmlChar *file = NULL;
    const xmlChar *name = NULL;
    const char *type = "error";
    xmlGenericErrorFunc error = xsltGenericError;
    void *errctx = xsltGenericErrorContext;

    if (ctxt != NULL) {
	ctxt->state = XSLT_STATE_ERROR;
	if (ctxt->error != NULL) {
	    error = ctxt->error;
	    errctx = ctxt->errctx;
	}
    }
    if ((node == NULL) && (ctxt != NULL))
	node = ctxt->inst;

    if (node != NULL)  {
	if ((node->type == XML_DOCUMENT_NODE) ||
	    (node->type == XML_HTML_DOCUMENT_NODE)) {
	    xmlDocPtr doc = (xmlDocPtr) node;

	    file = doc->URL;
	} else {
	    line = xmlGetLineNo(node);
	    if ((node->doc != NULL) && (node->doc->URL != NULL))
		file = node->doc->URL;
	    if (node->name != NULL)
		name = node->name;
	}
    } 
    
    if (ctxt != NULL)
	type = "runtime error";
    else if (style != NULL) {
#ifdef XSLT_REFACTORED
	if (XSLT_CCTXT(style)->errSeverity == XSLT_ERROR_SEVERITY_WARNING)
	    type = "compilation warning";
	else
	    type = "compilation error";
#else
	type = "compilation error";
#endif
    }

    if ((file != NULL) && (line != 0) && (name != NULL))
	error(errctx, "%s: file %s line %d element %s\n",
	      type, file, line, name);
    else if ((file != NULL) && (name != NULL))
	error(errctx, "%s: file %s element %s\n", type, file, name);
    else if ((file != NULL) && (line != 0))
	error(errctx, "%s: file %s line %d\n", type, file, line);
    else if (file != NULL)
	error(errctx, "%s: file %s\n", type, file);
    else if (name != NULL)
	error(errctx, "%s: element %s\n", type, name);
    else
	error(errctx, "%s\n", type);
}

void
xsltSetTransformErrorFunc(xsltTransformContextPtr ctxt,
                          void *ctx, xmlGenericErrorFunc handler)
{
    ctxt->error = handler;
    ctxt->errctx = ctx;
}

void
xsltTransformError(xsltTransformContextPtr ctxt,
		   xsltStylesheetPtr style,
		   xmlNodePtr node,
		   const char *msg, ...) {
    xmlGenericErrorFunc error = xsltGenericError;
    void *errctx = xsltGenericErrorContext;
    char * str;

    if (ctxt != NULL) {
	ctxt->state = XSLT_STATE_ERROR;
	if (ctxt->error != NULL) {
	    error = ctxt->error;
	    errctx = ctxt->errctx;
	}
    }
    if ((node == NULL) && (ctxt != NULL))
	node = ctxt->inst;
    xsltPrintErrorContext(ctxt, style, node);
    XSLT_GET_VAR_STR(msg, str);
    error(errctx, "%s", str);
    if (str != NULL)
	xmlFree(str);
}


const xmlChar *
xsltSplitQName(xmlDictPtr dict, const xmlChar *name, const xmlChar **prefix) {
    int len = 0;
    const xmlChar *ret = NULL;

    *prefix = NULL;
    if ((name == NULL) || (dict == NULL)) return(NULL);
    if (name[0] == ':')
        return(xmlDictLookup(dict, name, -1));
    while ((name[len] != 0) && (name[len] != ':')) len++;
    if (name[len] == 0) return(xmlDictLookup(dict, name, -1));
    *prefix = xmlDictLookup(dict, name, len);
    ret = xmlDictLookup(dict, &name[len + 1], -1);
    return(ret);
}

const xmlChar *
xsltGetQNameURI(xmlNodePtr node, xmlChar ** name)
{
    int len = 0;
    xmlChar *qname;
    xmlNsPtr ns;

    if (name == NULL)
	return(NULL);
    qname = *name;
    if ((qname == NULL) || (*qname == 0))
	return(NULL);
    if (node == NULL) {
	xsltGenericError(xsltGenericErrorContext,
		         "QName: no element for namespace lookup %s\n",
			 qname);
	xmlFree(qname);
	*name = NULL;
	return(NULL);
    }

    
    if (qname[0] == ':')
	return(NULL);

    while ((qname[len] != 0) && (qname[len] != ':')) 
	len++;
    
    if (qname[len] == 0)
	return(NULL);

    if ((qname[0] == 'x') && (qname[1] == 'm') &&
        (qname[2] == 'l') && (qname[3] == ':')) {
	if (qname[4] == 0)
	    return(NULL);
        *name = xmlStrdup(&qname[4]);
	xmlFree(qname);
	return(XML_XML_NAMESPACE);
    }

    qname[len] = 0;
    ns = xmlSearchNs(node->doc, node, qname);
    if (ns == NULL) {
	xsltGenericError(xsltGenericErrorContext,
		"%s:%s : no namespace bound to prefix %s\n",
		         qname, &qname[len + 1], qname);
	*name = NULL;
	xmlFree(qname);
	return(NULL);
    }
    *name = xmlStrdup(&qname[len + 1]);
    xmlFree(qname);
    return(ns->href);
}

const xmlChar *
xsltGetQNameURI2(xsltStylesheetPtr style, xmlNodePtr node,
		 const xmlChar **name) {
    int len = 0;
    xmlChar *qname;
    xmlNsPtr ns;

    if (name == NULL)
        return(NULL);
    qname = (xmlChar *)*name;
    if ((qname == NULL) || (*qname == 0))
        return(NULL);
    if (node == NULL) {
        xsltGenericError(xsltGenericErrorContext,
                         "QName: no element for namespace lookup %s\n",
                          qname);
	*name = NULL;
	return(NULL);
    }

    while ((qname[len] != 0) && (qname[len] != ':'))
        len++;

    if (qname[len] == 0)
        return(NULL);

    if ((qname[0] == 'x') && (qname[1] == 'm') &&
        (qname[2] == 'l') && (qname[3] == ':')) {
        if (qname[4] == 0)
            return(NULL);
        *name = xmlDictLookup(style->dict, &qname[4], -1);
        return(XML_XML_NAMESPACE);
    }

    qname = xmlStrndup(*name, len);
    ns = xmlSearchNs(node->doc, node, qname);
    if (ns == NULL) {
	if (style) {
	    xsltTransformError(NULL, style, node,
		"No namespace bound to prefix '%s'.\n",
		qname);
	    style->errors++;
	} else {
	    xsltGenericError(xsltGenericErrorContext,
                "%s : no namespace bound to prefix %s\n",
		*name, qname);
	}
        *name = NULL;
        xmlFree(qname);
        return(NULL);
    }
    *name = xmlDictLookup(style->dict, (*name)+len+1, -1);
    xmlFree(qname);
    return(ns->href);
}
										      

void
xsltDocumentSortFunction(xmlNodeSetPtr list) {
    int i, j;
    int len, tst;
    xmlNodePtr node;

    if (list == NULL)
	return;
    len = list->nodeNr;
    if (len <= 1)
	return;
    
    for (i = 0;i < len -1;i++) {
	for (j = i + 1; j < len; j++) {
	    tst = xmlXPathCmpNodes(list->nodeTab[i], list->nodeTab[j]);
	    if (tst == -1) {
		node = list->nodeTab[i];
		list->nodeTab[i] = list->nodeTab[j];
		list->nodeTab[j] = node;
	    }
	}
    }
}

xmlXPathObjectPtr *
xsltComputeSortResult(xsltTransformContextPtr ctxt, xmlNodePtr sort) {
#ifdef XSLT_REFACTORED
    xsltStyleItemSortPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    xmlXPathObjectPtr *results = NULL;
    xmlNodeSetPtr list = NULL;
    xmlXPathObjectPtr res;
    int len = 0;
    int i;    
    xmlNodePtr oldNode;
    xmlNodePtr oldInst;
    int	oldPos, oldSize ;
    int oldNsNr;
    xmlNsPtr *oldNamespaces;

    comp = sort->psvi;
    if (comp == NULL) {
	xsltGenericError(xsltGenericErrorContext,
	     "xsl:sort : compilation failed\n");
	return(NULL);
    }

    if ((comp->select == NULL) || (comp->comp == NULL))
	return(NULL);

    list = ctxt->nodeList;
    if ((list == NULL) || (list->nodeNr <= 1))
	return(NULL);

    len = list->nodeNr;

    
    


    results = xmlMalloc(len * sizeof(xmlXPathObjectPtr));
    if (results == NULL) {
	xsltGenericError(xsltGenericErrorContext,
	     "xsltComputeSortResult: memory allocation failure\n");
	return(NULL);
    }

    oldNode = ctxt->node;
    oldInst = ctxt->inst;
    oldPos = ctxt->xpathCtxt->proximityPosition;
    oldSize = ctxt->xpathCtxt->contextSize;
    oldNsNr = ctxt->xpathCtxt->nsNr;
    oldNamespaces = ctxt->xpathCtxt->namespaces;
    for (i = 0;i < len;i++) {
	ctxt->inst = sort;
	ctxt->xpathCtxt->contextSize = len;
	ctxt->xpathCtxt->proximityPosition = i + 1;
	ctxt->node = list->nodeTab[i];
	ctxt->xpathCtxt->node = ctxt->node;
#ifdef XSLT_REFACTORED
	if (comp->inScopeNs != NULL) {
	    ctxt->xpathCtxt->namespaces = comp->inScopeNs->list;
	    ctxt->xpathCtxt->nsNr = comp->inScopeNs->xpathNumber;
	} else {
	    ctxt->xpathCtxt->namespaces = NULL;
	    ctxt->xpathCtxt->nsNr = 0;
	}
#else
	ctxt->xpathCtxt->namespaces = comp->nsList;
	ctxt->xpathCtxt->nsNr = comp->nsNr;
#endif
	res = xmlXPathCompiledEval(comp->comp, ctxt->xpathCtxt);
	if (res != NULL) {
	    if (res->type != XPATH_STRING)
		res = xmlXPathConvertString(res);
	    if (comp->number)
		res = xmlXPathConvertNumber(res);
	    res->index = i;	
	    if (comp->number) {
		if (res->type == XPATH_NUMBER) {
		    results[i] = res;
		} else {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    xsltGenericDebug(xsltGenericDebugContext,
			"xsltComputeSortResult: select didn't evaluate to a number\n");
#endif
		    results[i] = NULL;
		}
	    } else {
		if (res->type == XPATH_STRING) {
		    if (comp->locale != (xsltLocale)0) {
			xmlChar *str = res->stringval;
			res->stringval = (xmlChar *) xsltStrxfrm(comp->locale, str);
			xmlFree(str);
		    }

		    results[i] = res;
		} else {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    xsltGenericDebug(xsltGenericDebugContext,
			"xsltComputeSortResult: select didn't evaluate to a string\n");
#endif
		    results[i] = NULL;
		}
	    }
	} else {
	    ctxt->state = XSLT_STATE_STOPPED;
	    results[i] = NULL;
	}
    }
    ctxt->node = oldNode;
    ctxt->inst = oldInst;
    ctxt->xpathCtxt->contextSize = oldSize;
    ctxt->xpathCtxt->proximityPosition = oldPos;
    ctxt->xpathCtxt->nsNr = oldNsNr;
    ctxt->xpathCtxt->namespaces = oldNamespaces;

    return(results);
}

void	
xsltDefaultSortFunction(xsltTransformContextPtr ctxt, xmlNodePtr *sorts,
	           int nbsorts) {
#ifdef XSLT_REFACTORED
    xsltStyleItemSortPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    xmlXPathObjectPtr *resultsTab[XSLT_MAX_SORT];
    xmlXPathObjectPtr *results = NULL, *res;
    xmlNodeSetPtr list = NULL;
    int descending, number, desc, numb;
    int len = 0;
    int i, j, incr;
    int tst;
    int depth;
    xmlNodePtr node;
    xmlXPathObjectPtr tmp;    
    int tempstype[XSLT_MAX_SORT], temporder[XSLT_MAX_SORT];

    if ((ctxt == NULL) || (sorts == NULL) || (nbsorts <= 0) ||
	(nbsorts >= XSLT_MAX_SORT))
	return;
    if (sorts[0] == NULL)
	return;
    comp = sorts[0]->psvi;
    if (comp == NULL)
	return;

    list = ctxt->nodeList;
    if ((list == NULL) || (list->nodeNr <= 1))
	return; 

    for (j = 0; j < nbsorts; j++) {
	comp = sorts[j]->psvi;
	tempstype[j] = 0;
	if ((comp->stype == NULL) && (comp->has_stype != 0)) {
	    comp->stype =
		xsltEvalAttrValueTemplate(ctxt, sorts[j],
					  (const xmlChar *) "data-type",
					  XSLT_NAMESPACE);
	    if (comp->stype != NULL) {
		tempstype[j] = 1;
		if (xmlStrEqual(comp->stype, (const xmlChar *) "text"))
		    comp->number = 0;
		else if (xmlStrEqual(comp->stype, (const xmlChar *) "number"))
		    comp->number = 1;
		else {
		    xsltTransformError(ctxt, NULL, sorts[j],
			  "xsltDoSortFunction: no support for data-type = %s\n",
				     comp->stype);
		    comp->number = 0; 
		}
	    }
	}
	temporder[j] = 0;
	if ((comp->order == NULL) && (comp->has_order != 0)) {
	    comp->order = xsltEvalAttrValueTemplate(ctxt, sorts[j],
						    (const xmlChar *) "order",
						    XSLT_NAMESPACE);
	    if (comp->order != NULL) {
		temporder[j] = 1;
		if (xmlStrEqual(comp->order, (const xmlChar *) "ascending"))
		    comp->descending = 0;
		else if (xmlStrEqual(comp->order,
				     (const xmlChar *) "descending"))
		    comp->descending = 1;
		else {
		    xsltTransformError(ctxt, NULL, sorts[j],
			     "xsltDoSortFunction: invalid value %s for order\n",
				     comp->order);
		    comp->descending = 0; 
		}
	    }
	}
    }

    len = list->nodeNr;

    resultsTab[0] = xsltComputeSortResult(ctxt, sorts[0]);
    for (i = 1;i < XSLT_MAX_SORT;i++)
	resultsTab[i] = NULL;

    results = resultsTab[0];

    comp = sorts[0]->psvi;
    descending = comp->descending;
    number = comp->number;
    if (results == NULL)
	return;

    
    for (incr = len / 2; incr > 0; incr /= 2) {
	for (i = incr; i < len; i++) {
	    j = i - incr;
	    if (results[i] == NULL)
		continue;
	    
	    while (j >= 0) {
		if (results[j] == NULL)
		    tst = 1;
		else {
		    if (number) {
			if (xmlXPathIsNaN(results[j]->floatval)) {
			    if (xmlXPathIsNaN(results[j + incr]->floatval))
				tst = 0;
			    else
				tst = -1;
			} else if (xmlXPathIsNaN(results[j + incr]->floatval))
			    tst = 1;
			else if (results[j]->floatval ==
				results[j + incr]->floatval)
			    tst = 0;
			else if (results[j]->floatval > 
				results[j + incr]->floatval)
			    tst = 1;
			else tst = -1;
		    } else if(comp->locale != (xsltLocale)0) {
			tst = xsltLocaleStrcmp(
			    comp->locale,
			    (xsltLocaleChar *) results[j]->stringval,
			    (xsltLocaleChar *) results[j + incr]->stringval); 
		    } else {
			tst = xmlStrcmp(results[j]->stringval,
				     results[j + incr]->stringval); 
		    }
		    if (descending)
			tst = -tst;
		}
		if (tst == 0) {
		    depth = 1;
		    while (depth < nbsorts) {
			if (sorts[depth] == NULL)
			    break;
			comp = sorts[depth]->psvi;
			if (comp == NULL)
			    break;
			desc = comp->descending;
			numb = comp->number;

			if (resultsTab[depth] == NULL) 
			    resultsTab[depth] = xsltComputeSortResult(ctxt,
				                        sorts[depth]);
			res = resultsTab[depth];
			if (res == NULL) 
			    break;
			if (res[j] == NULL) {
			    if (res[j+incr] != NULL)
				tst = 1;
			} else {
			    if (numb) {
				if (xmlXPathIsNaN(res[j]->floatval)) {
				    if (xmlXPathIsNaN(res[j +
				    		incr]->floatval))
					tst = 0;
				    else
				        tst = -1;
				} else if (xmlXPathIsNaN(res[j + incr]->
						floatval))
				    tst = 1;
				else if (res[j]->floatval == res[j + incr]->
						floatval)
				    tst = 0;
				else if (res[j]->floatval > 
					res[j + incr]->floatval)
				    tst = 1;
				else tst = -1;
			    } else if(comp->locale != (xsltLocale)0) {
				tst = xsltLocaleStrcmp(
				    comp->locale,
				    (xsltLocaleChar *) res[j]->stringval,
				    (xsltLocaleChar *) res[j + incr]->stringval); 
			    } else {
				tst = xmlStrcmp(res[j]->stringval,
					     res[j + incr]->stringval); 
			    }
			    if (desc)
				tst = -tst;
			}

			if (tst != 0)
			    break;
			depth++;
		    }
		}
		if (tst == 0) {
		    tst = results[j]->index > results[j + incr]->index;
		}
		if (tst > 0) {
		    tmp = results[j];
		    results[j] = results[j + incr];
		    results[j + incr] = tmp;
		    node = list->nodeTab[j];
		    list->nodeTab[j] = list->nodeTab[j + incr];
		    list->nodeTab[j + incr] = node;
		    depth = 1;
		    while (depth < nbsorts) {
			if (sorts[depth] == NULL)
			    break;
			if (resultsTab[depth] == NULL)
			    break;
			res = resultsTab[depth];
			tmp = res[j];
			res[j] = res[j + incr];
			res[j + incr] = tmp;
			depth++;
		    }
		    j -= incr;
		} else
		    break;
	    }
	}
    }

    for (j = 0; j < nbsorts; j++) {
	comp = sorts[j]->psvi;
	if (tempstype[j] == 1) {
	    
	    xmlFree((void *)(comp->stype));
	    comp->stype = NULL;
	}
	if (temporder[j] == 1) {
	    
	    xmlFree((void *)(comp->order));
	    comp->order = NULL;
	}
	if (resultsTab[j] != NULL) {
	    for (i = 0;i < len;i++)
		xmlXPathFreeObject(resultsTab[j][i]);
	    xmlFree(resultsTab[j]);
	}
    }
}


static xsltSortFunc xsltSortFunction = xsltDefaultSortFunction;

void
xsltDoSortFunction(xsltTransformContextPtr ctxt, xmlNodePtr * sorts,
                   int nbsorts)
{
    if (ctxt->sortfunc != NULL)
	(ctxt->sortfunc)(ctxt, sorts, nbsorts);
    else if (xsltSortFunction != NULL)
        xsltSortFunction(ctxt, sorts, nbsorts);
}

void
xsltSetSortFunc(xsltSortFunc handler) {
    if (handler != NULL)
	xsltSortFunction = handler;
    else
	xsltSortFunction = xsltDefaultSortFunction;
}

void 
xsltSetCtxtSortFunc(xsltTransformContextPtr ctxt, xsltSortFunc handler) {
    ctxt->sortfunc = handler;
}


int 
xsltSetCtxtParseOptions(xsltTransformContextPtr ctxt, int options)
{
    int oldopts;

    if (ctxt == NULL)
        return(-1);
    oldopts = ctxt->parserOptions;
    if (ctxt->xinclude)
        oldopts |= XML_PARSE_XINCLUDE;
    ctxt->parserOptions = options;
    if (options & XML_PARSE_XINCLUDE)
        ctxt->xinclude = 1;
    else
        ctxt->xinclude = 0;
    return(oldopts);
}


/**
 * xsltSaveResultTo:
 * @buf:  an output buffer
 * @result:  the result xmlDocPtr
 * @style:  the stylesheet
 *
 * Save the result @result obtained by applying the @style stylesheet
 * to an I/O output channel @buf
 *
 * Returns the number of byte written or -1 in case of failure.
 */
int
xsltSaveResultTo(xmlOutputBufferPtr buf, xmlDocPtr result,
	       xsltStylesheetPtr style) {
    const xmlChar *encoding;
    int base;
    const xmlChar *method;
    int indent;

    if ((buf == NULL) || (result == NULL) || (style == NULL))
	return(-1);
    if ((result->children == NULL) ||
	((result->children->type == XML_DTD_NODE) &&
	 (result->children->next == NULL)))
	return(0);

    if ((style->methodURI != NULL) &&
	((style->method == NULL) ||
	 (!xmlStrEqual(style->method, (const xmlChar *) "xhtml")))) {
        xsltGenericError(xsltGenericErrorContext,
		"xsltSaveResultTo : unknown ouput method\n");
        return(-1);
    }

    base = buf->written;

    XSLT_GET_IMPORT_PTR(method, style, method)
    XSLT_GET_IMPORT_PTR(encoding, style, encoding)
    XSLT_GET_IMPORT_INT(indent, style, indent);

    if ((method == NULL) && (result->type == XML_HTML_DOCUMENT_NODE))
	method = (const xmlChar *) "html";

    if ((method != NULL) &&
	(xmlStrEqual(method, (const xmlChar *) "html"))) {
	if (encoding != NULL) {
	    htmlSetMetaEncoding(result, (const xmlChar *) encoding);
	} else {
	    htmlSetMetaEncoding(result, (const xmlChar *) "UTF-8");
	}
	if (indent == -1)
	    indent = 1;
	htmlDocContentDumpFormatOutput(buf, result, (const char *) encoding,
		                       indent);
	xmlOutputBufferFlush(buf);
    } else if ((method != NULL) &&
	(xmlStrEqual(method, (const xmlChar *) "xhtml"))) {
	if (encoding != NULL) {
	    htmlSetMetaEncoding(result, (const xmlChar *) encoding);
	} else {
	    htmlSetMetaEncoding(result, (const xmlChar *) "UTF-8");
	}
	htmlDocContentDumpOutput(buf, result, (const char *) encoding);
	xmlOutputBufferFlush(buf);
    } else if ((method != NULL) &&
	       (xmlStrEqual(method, (const xmlChar *) "text"))) {
	xmlNodePtr cur;

	cur = result->children;
	while (cur != NULL) {
	    if (cur->type == XML_TEXT_NODE)
		xmlOutputBufferWriteString(buf, (const char *) cur->content);

	    if (cur->children != NULL) {
		if ((cur->children->type != XML_ENTITY_DECL) &&
		    (cur->children->type != XML_ENTITY_REF_NODE) &&
		    (cur->children->type != XML_ENTITY_NODE)) {
		    cur = cur->children;
		    continue;
		}
	    }
	    if (cur->next != NULL) {
		cur = cur->next;
		continue;
	    }
	    
	    do {
		cur = cur->parent;
		if (cur == NULL)
		    break;
		if (cur == (xmlNodePtr) style->doc) {
		    cur = NULL;
		    break;
		}
		if (cur->next != NULL) {
		    cur = cur->next;
		    break;
		}
	    } while (cur != NULL);
	}
	xmlOutputBufferFlush(buf);
    } else {
	int omitXmlDecl;
	int standalone;

	XSLT_GET_IMPORT_INT(omitXmlDecl, style, omitXmlDeclaration);
	XSLT_GET_IMPORT_INT(standalone, style, standalone);

	if (omitXmlDecl != 1) {
	    xmlOutputBufferWriteString(buf, "<?xml version=");
	    if (result->version != NULL) 
		xmlBufferWriteQuotedString(buf->buffer, result->version);
	    else
		xmlOutputBufferWriteString(buf, "\"1.0\"");
	    if (encoding == NULL) {
		if (result->encoding != NULL)
		    encoding = result->encoding;
		else if (result->charset != XML_CHAR_ENCODING_UTF8)
		    encoding = (const xmlChar *)
			       xmlGetCharEncodingName((xmlCharEncoding)
			                              result->charset);
	    }
	    if (encoding != NULL) {
		xmlOutputBufferWriteString(buf, " encoding=");
		xmlBufferWriteQuotedString(buf->buffer, (xmlChar *) encoding);
	    }
	    switch (standalone) {
		case 0:
		    xmlOutputBufferWriteString(buf, " standalone=\"no\"");
		    break;
		case 1:
		    xmlOutputBufferWriteString(buf, " standalone=\"yes\"");
		    break;
		default:
		    break;
	    }
	    xmlOutputBufferWriteString(buf, "?>\n");
	}
	if (result->children != NULL) {
	    xmlNodePtr child = result->children;

	    while (child != NULL) {
		xmlNodeDumpOutput(buf, result, child, 0, (indent == 1),
			          (const char *) encoding);
		if ((child->type == XML_DTD_NODE) ||
		    ((child->type == XML_COMMENT_NODE) &&
		     (child->next != NULL)))
		    xmlOutputBufferWriteString(buf, "\n");
		child = child->next;
	    }
	    xmlOutputBufferWriteString(buf, "\n");
	}
	xmlOutputBufferFlush(buf);
    }
    return(buf->written - base);
}

/**
 * xsltSaveResultToFilename:
 * @URL:  a filename or URL
 * @result:  the result xmlDocPtr
 * @style:  the stylesheet
 * @compression:  the compression factor (0 - 9 included)
 *
 * Save the result @result obtained by applying the @style stylesheet
 * to a file or @URL
 *
 * Returns the number of byte written or -1 in case of failure.
 */
int
xsltSaveResultToFilename(const char *URL, xmlDocPtr result,
			 xsltStylesheetPtr style, int compression) {
    xmlOutputBufferPtr buf;
    const xmlChar *encoding;
    int ret;

    if ((URL == NULL) || (result == NULL) || (style == NULL))
	return(-1);
    if (result->children == NULL)
	return(0);

    XSLT_GET_IMPORT_PTR(encoding, style, encoding)
    if (encoding != NULL) {
	xmlCharEncodingHandlerPtr encoder;

	encoder = xmlFindCharEncodingHandler((char *)encoding);
	if ((encoder != NULL) &&
	    (xmlStrEqual((const xmlChar *)encoder->name,
			 (const xmlChar *) "UTF-8")))
	    encoder = NULL;
	buf = xmlOutputBufferCreateFilename(URL, encoder, compression);
    } else {
	buf = xmlOutputBufferCreateFilename(URL, NULL, compression);
    }
    if (buf == NULL)
	return(-1);
    xsltSaveResultTo(buf, result, style);
    ret = xmlOutputBufferClose(buf);
    return(ret);
}

/**
 * xsltSaveResultToFile:
 * @file:  a FILE * I/O
 * @result:  the result xmlDocPtr
 * @style:  the stylesheet
 *
 * Save the result @result obtained by applying the @style stylesheet
 * to an open FILE * I/O.
 * This does not close the FILE @file
 *
 * Returns the number of bytes written or -1 in case of failure.
 */
int
xsltSaveResultToFile(FILE *file, xmlDocPtr result, xsltStylesheetPtr style) {
    xmlOutputBufferPtr buf;
    const xmlChar *encoding;
    int ret;

    if ((file == NULL) || (result == NULL) || (style == NULL))
	return(-1);
    if (result->children == NULL)
	return(0);

    XSLT_GET_IMPORT_PTR(encoding, style, encoding)
    if (encoding != NULL) {
	xmlCharEncodingHandlerPtr encoder;

	encoder = xmlFindCharEncodingHandler((char *)encoding);
	if ((encoder != NULL) &&
	    (xmlStrEqual((const xmlChar *)encoder->name,
			 (const xmlChar *) "UTF-8")))
	    encoder = NULL;
	buf = xmlOutputBufferCreateFile(file, encoder);
    } else {
	buf = xmlOutputBufferCreateFile(file, NULL);
    }

    if (buf == NULL)
	return(-1);
    xsltSaveResultTo(buf, result, style);
    ret = xmlOutputBufferClose(buf);
    return(ret);
}

/**
 * xsltSaveResultToFd:
 * @fd:  a file descriptor
 * @result:  the result xmlDocPtr
 * @style:  the stylesheet
 *
 * Save the result @result obtained by applying the @style stylesheet
 * to an open file descriptor
 * This does not close the descriptor.
 *
 * Returns the number of bytes written or -1 in case of failure.
 */
int
xsltSaveResultToFd(int fd, xmlDocPtr result, xsltStylesheetPtr style) {
    xmlOutputBufferPtr buf;
    const xmlChar *encoding;
    int ret;

    if ((fd < 0) || (result == NULL) || (style == NULL))
	return(-1);
    if (result->children == NULL)
	return(0);

    XSLT_GET_IMPORT_PTR(encoding, style, encoding)
    if (encoding != NULL) {
	xmlCharEncodingHandlerPtr encoder;

	encoder = xmlFindCharEncodingHandler((char *)encoding);
	if ((encoder != NULL) &&
	    (xmlStrEqual((const xmlChar *)encoder->name,
			 (const xmlChar *) "UTF-8")))
	    encoder = NULL;
	buf = xmlOutputBufferCreateFd(fd, encoder);
    } else {
	buf = xmlOutputBufferCreateFd(fd, NULL);
    }
    if (buf == NULL)
	return(-1);
    xsltSaveResultTo(buf, result, style);
    ret = xmlOutputBufferClose(buf);
    return(ret);
}

int
xsltSaveResultToString(xmlChar **doc_txt_ptr, int * doc_txt_len, 
		       xmlDocPtr result, xsltStylesheetPtr style) {
    xmlOutputBufferPtr buf;
    const xmlChar *encoding;

    *doc_txt_ptr = NULL;
    *doc_txt_len = 0;
    if (result->children == NULL)
	return(0);

    XSLT_GET_IMPORT_PTR(encoding, style, encoding)
    if (encoding != NULL) {
	xmlCharEncodingHandlerPtr encoder;

	encoder = xmlFindCharEncodingHandler((char *)encoding);
	if ((encoder != NULL) &&
	    (xmlStrEqual((const xmlChar *)encoder->name,
			 (const xmlChar *) "UTF-8")))
	    encoder = NULL;
	buf = xmlAllocOutputBuffer(encoder);
    } else {
	buf = xmlAllocOutputBuffer(NULL);
    }
    if (buf == NULL)
	return(-1);
    xsltSaveResultTo(buf, result, style);
    if (buf->conv != NULL) {
	*doc_txt_len = buf->conv->use;
	*doc_txt_ptr = xmlStrndup(buf->conv->content, *doc_txt_len);
    } else {
	*doc_txt_len = buf->buffer->use;
	*doc_txt_ptr = xmlStrndup(buf->buffer->content, *doc_txt_len);
    }
    (void)xmlOutputBufferClose(buf);
    return 0;
}


static long calibration = -1;

static long
xsltCalibrateTimestamps(void) {
    register int i;

    for (i = 0;i < 999;i++)
	xsltTimestamp();
    return(xsltTimestamp() / 1000);
}

void
xsltCalibrateAdjust(long delta) {
    calibration += delta;
}

long
xsltTimestamp(void)
{
#ifdef XSLT_WIN32_PERFORMANCE_COUNTER
    BOOL ok;
    LARGE_INTEGER performanceCount;
    LARGE_INTEGER performanceFrequency;
    LONGLONG quadCount;
    double seconds;
    static LONGLONG startupQuadCount = 0;
    static LONGLONG startupQuadFreq = 0;

    ok = QueryPerformanceCounter(&performanceCount);
    if (!ok)
        return 0;
    quadCount = performanceCount.QuadPart;
    if (calibration < 0) {
        calibration = 0;
        ok = QueryPerformanceFrequency(&performanceFrequency);
        if (!ok)
            return 0;
        startupQuadFreq = performanceFrequency.QuadPart;
        startupQuadCount = quadCount;
        return (0);
    }
    if (startupQuadFreq == 0)
        return 0;
    seconds = (quadCount - startupQuadCount) / (double) startupQuadFreq;
    return (long) (seconds * XSLT_TIMESTAMP_TICS_PER_SEC);

#else 
#ifdef HAVE_GETTIMEOFDAY
    static struct timeval startup;
    struct timeval cur;
    long tics;

    if (calibration < 0) {
        gettimeofday(&startup, NULL);
        calibration = 0;
        calibration = xsltCalibrateTimestamps();
        gettimeofday(&startup, NULL);
        return (0);
    }

    gettimeofday(&cur, NULL);
    tics = (cur.tv_sec - startup.tv_sec) * XSLT_TIMESTAMP_TICS_PER_SEC;
    tics += (cur.tv_usec - startup.tv_usec) /
                          (1000000l / XSLT_TIMESTAMP_TICS_PER_SEC);
    
    tics -= calibration;
    return(tics);
#else

    

    return (0);

#endif 
#endif 
}

#define MAX_TEMPLATES 10000

void
xsltSaveProfiling(xsltTransformContextPtr ctxt, FILE *output) {
    int nb, i,j;
    int max;
    int total;
    long totalt;
    xsltTemplatePtr *templates;
    xsltStylesheetPtr style;
    xsltTemplatePtr template;

    if ((output == NULL) || (ctxt == NULL))
	return;
    if (ctxt->profile == 0)
	return;

    nb = 0;
    max = MAX_TEMPLATES;
    templates = xmlMalloc(max * sizeof(xsltTemplatePtr));
    if (templates == NULL)
	return;

    style = ctxt->style;
    while (style != NULL) {
	template = style->templates;
	while (template != NULL) {
	    if (nb >= max)
		break;

	    if (template->nbCalls > 0)
		templates[nb++] = template;
	    template = template->next;
	}

	style = xsltNextImport(style);
    }

    for (i = 0;i < nb -1;i++) {
	for (j = i + 1; j < nb; j++) {
	    if ((templates[i]->time <= templates[j]->time) ||
		((templates[i]->time == templates[j]->time) &&
	         (templates[i]->nbCalls <= templates[j]->nbCalls))) {
		template = templates[j];
		templates[j] = templates[i];
		templates[i] = template;
	    }
	}
    }

    fprintf(output, "%6s%20s%20s%10s  Calls Tot 100us Avg\n\n",
	    "number", "match", "name", "mode");
    total = 0;
    totalt = 0;
    for (i = 0;i < nb;i++) {
	fprintf(output, "%5d ", i);
	if (templates[i]->match != NULL) {
	    if (xmlStrlen(templates[i]->match) > 20)
		fprintf(output, "%s\n%26s", templates[i]->match, "");
	    else
		fprintf(output, "%20s", templates[i]->match);
	} else {
	    fprintf(output, "%20s", "");
	}
	if (templates[i]->name != NULL) {
	    if (xmlStrlen(templates[i]->name) > 20)
		fprintf(output, "%s\n%46s", templates[i]->name, "");
	    else
		fprintf(output, "%20s", templates[i]->name);
	} else {
	    fprintf(output, "%20s", "");
	}
	if (templates[i]->mode != NULL) {
	    if (xmlStrlen(templates[i]->mode) > 10)
		fprintf(output, "%s\n%56s", templates[i]->mode, "");
	    else
		fprintf(output, "%10s", templates[i]->mode);
	} else {
	    fprintf(output, "%10s", "");
	}
	fprintf(output, " %6d", templates[i]->nbCalls);
	fprintf(output, " %6ld %6ld\n", templates[i]->time,
		templates[i]->time / templates[i]->nbCalls);
	total += templates[i]->nbCalls;
	totalt += templates[i]->time;
    }
    fprintf(output, "\n%30s%26s %6d %6ld\n", "Total", "", total, totalt);

    xmlFree(templates);
}



xmlDocPtr
xsltGetProfileInformation(xsltTransformContextPtr ctxt)
{
    xmlDocPtr ret = NULL;
    xmlNodePtr root, child;
    char buf[100];

    xsltStylesheetPtr style;
    xsltTemplatePtr *templates;
    xsltTemplatePtr templ;
    int nb = 0, max = 0, i, j;

    if (!ctxt)
        return NULL;

    if (!ctxt->profile)
        return NULL;

    nb = 0;
    max = 10000;
    templates =
        (xsltTemplatePtr *) xmlMalloc(max * sizeof(xsltTemplatePtr));
    if (templates == NULL)
        return NULL;

    style = ctxt->style;
    while (style != NULL) {
        templ = style->templates;
        while (templ != NULL) {
            if (nb >= max)
                break;

            if (templ->nbCalls > 0)
                templates[nb++] = templ;
            templ = templ->next;
        }

        style = (xsltStylesheetPtr) xsltNextImport(style);
    }

    for (i = 0; i < nb - 1; i++) {
        for (j = i + 1; j < nb; j++) {
            if ((templates[i]->time <= templates[j]->time) ||
                ((templates[i]->time == templates[j]->time) &&
                 (templates[i]->nbCalls <= templates[j]->nbCalls))) {
                templ = templates[j];
                templates[j] = templates[i];
                templates[i] = templ;
            }
        }
    }

    ret = xmlNewDoc(BAD_CAST "1.0");
    root = xmlNewDocNode(ret, NULL, BAD_CAST "profile", NULL);
    xmlDocSetRootElement(ret, root);

    for (i = 0; i < nb; i++) {
        child = xmlNewChild(root, NULL, BAD_CAST "template", NULL);
        sprintf(buf, "%d", i + 1);
        xmlSetProp(child, BAD_CAST "rank", BAD_CAST buf);
        xmlSetProp(child, BAD_CAST "match", BAD_CAST templates[i]->match);
        xmlSetProp(child, BAD_CAST "name", BAD_CAST templates[i]->name);
        xmlSetProp(child, BAD_CAST "mode", BAD_CAST templates[i]->mode);

        sprintf(buf, "%d", templates[i]->nbCalls);
        xmlSetProp(child, BAD_CAST "calls", BAD_CAST buf);

        sprintf(buf, "%ld", templates[i]->time);
        xmlSetProp(child, BAD_CAST "time", BAD_CAST buf);

        sprintf(buf, "%ld", templates[i]->time / templates[i]->nbCalls);
        xmlSetProp(child, BAD_CAST "average", BAD_CAST buf);
    };

    xmlFree(templates);

    return ret;
}


xmlXPathCompExprPtr
xsltXPathCompile(xsltStylesheetPtr style, const xmlChar *str) {
    xmlXPathContextPtr xpathCtxt;
    xmlXPathCompExprPtr ret;

    if (style != NULL) {
#ifdef XSLT_REFACTORED_XPATHCOMP
	if (XSLT_CCTXT(style)) {
	    xpathCtxt = XSLT_CCTXT(style)->xpathCtxt;
	    xpathCtxt->doc = style->doc;
	} else
	    xpathCtxt = xmlXPathNewContext(style->doc);
#else
	xpathCtxt = xmlXPathNewContext(style->doc);
#endif
	if (xpathCtxt == NULL)
	    return NULL;
	xpathCtxt->dict = style->dict;
    } else {
	xpathCtxt = xmlXPathNewContext(NULL);
	if (xpathCtxt == NULL)
	    return NULL;
    }
    ret = xmlXPathCtxtCompile(xpathCtxt, str);

#ifdef XSLT_REFACTORED_XPATHCOMP
    if ((style == NULL) || (! XSLT_CCTXT(style))) {
	xmlXPathFreeContext(xpathCtxt);
    }
#else
    xmlXPathFreeContext(xpathCtxt);
#endif

    return(ret);
}


#define XSLT_CALLBACK_NUMBER 3

typedef struct _xsltDebuggerCallbacks xsltDebuggerCallbacks;
typedef xsltDebuggerCallbacks *xsltDebuggerCallbacksPtr;
struct _xsltDebuggerCallbacks {
    xsltHandleDebuggerCallback handler;
    xsltAddCallCallback add;
    xsltDropCallCallback drop;
};

static xsltDebuggerCallbacks xsltDebuggerCurrentCallbacks = {
    NULL, 
    NULL, 
    NULL  
};

int xslDebugStatus;

void
xsltSetDebuggerStatus(int value)
{
    xslDebugStatus = value;	
}

int
xsltGetDebuggerStatus(void)
{
    return(xslDebugStatus);	
}

int
xsltSetDebuggerCallbacks(int no, void *block)
{
    xsltDebuggerCallbacksPtr callbacks;

    if ((block == NULL) || (no != XSLT_CALLBACK_NUMBER))
	return(-1);

    callbacks = (xsltDebuggerCallbacksPtr) block;
    xsltDebuggerCurrentCallbacks.handler = callbacks->handler;
    xsltDebuggerCurrentCallbacks.add  = callbacks->add;
    xsltDebuggerCurrentCallbacks.drop  = callbacks->drop;
    return(0);
}

void
xslHandleDebugger(xmlNodePtr cur, xmlNodePtr node, xsltTemplatePtr templ,
	          xsltTransformContextPtr ctxt)
{
    if (xsltDebuggerCurrentCallbacks.handler != NULL)
	xsltDebuggerCurrentCallbacks.handler(cur, node, templ, ctxt);
}

int
xslAddCall(xsltTemplatePtr templ, xmlNodePtr source)
{
    if (xsltDebuggerCurrentCallbacks.add != NULL)
	return(xsltDebuggerCurrentCallbacks.add(templ, source));
    return(0);
}

void
xslDropCall(void)
{
    if (xsltDebuggerCurrentCallbacks.drop != NULL)
	xsltDebuggerCurrentCallbacks.drop();
}

