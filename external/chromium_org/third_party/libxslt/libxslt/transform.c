/*
 * transform.c: Implementation of the XSL Transformation 1.0 engine
 *              transform part, i.e. applying a Stylesheet to a document
 *
 * References:
 *   http://www.w3.org/TR/1999/REC-xslt-19991116
 *
 *   Michael Kay "XSLT Programmer's Reference" pp 637-643
 *   Writing Multiple Output Files
 *
 *   XSLT-1.1 Working Draft
 *   http://www.w3.org/TR/xslt11#multiple-output
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXSLT
#include "libxslt.h"

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/valid.h>
#include <libxml/hash.h>
#include <libxml/encoding.h>
#include <libxml/xmlerror.h>
#include <libxml/xpath.h>
#include <libxml/parserInternals.h>
#include <libxml/xpathInternals.h>
#include <libxml/HTMLtree.h>
#include <libxml/debugXML.h>
#include <libxml/uri.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "pattern.h"
#include "transform.h"
#include "variables.h"
#include "numbersInternals.h"
#include "namespaces.h"
#include "attributes.h"
#include "templates.h"
#include "imports.h"
#include "keys.h"
#include "documents.h"
#include "extensions.h"
#include "extra.h"
#include "preproc.h"
#include "security.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_EXTRA
#define WITH_XSLT_DEBUG_PROCESS
#endif

#define XSLT_GENERATE_HTML_DOCTYPE
#ifdef XSLT_GENERATE_HTML_DOCTYPE
static int xsltGetHTMLIDs(const xmlChar *version, const xmlChar **publicID,
			  const xmlChar **systemID);
#endif

int xsltMaxDepth = 3000;


#ifndef FALSE
# define FALSE (0 == 1)
# define TRUE (!FALSE)
#endif

#define IS_BLANK_NODE(n)						\
    (((n)->type == XML_TEXT_NODE) && (xsltIsBlank((n)->content)))



static xmlNsPtr
xsltCopyNamespaceListInternal(xmlNodePtr node, xmlNsPtr cur);

static xmlNodePtr
xsltCopyTreeInternal(xsltTransformContextPtr ctxt,
		     xmlNodePtr invocNode,
		     xmlNodePtr node,
		     xmlNodePtr insert, int isLRE, int topElemVisited);

static void
xsltApplySequenceConstructor(xsltTransformContextPtr ctxt,
			     xmlNodePtr contextNode, xmlNodePtr list,
			     xsltTemplatePtr templ);

static void
xsltApplyXSLTTemplate(xsltTransformContextPtr ctxt,
		      xmlNodePtr contextNode,
		      xmlNodePtr list,
		      xsltTemplatePtr templ,
		      xsltStackElemPtr withParams);

static int
templPush(xsltTransformContextPtr ctxt, xsltTemplatePtr value)
{
    if (ctxt->templMax == 0) {
        ctxt->templMax = 4;
        ctxt->templTab =
            (xsltTemplatePtr *) xmlMalloc(ctxt->templMax *
                                          sizeof(ctxt->templTab[0]));
        if (ctxt->templTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "malloc failed !\n");
            return (0);
        }
    }
    if (ctxt->templNr >= ctxt->templMax) {
        ctxt->templMax *= 2;
        ctxt->templTab =
            (xsltTemplatePtr *) xmlRealloc(ctxt->templTab,
                                           ctxt->templMax *
                                           sizeof(ctxt->templTab[0]));
        if (ctxt->templTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
            return (0);
        }
    }
    ctxt->templTab[ctxt->templNr] = value;
    ctxt->templ = value;
    return (ctxt->templNr++);
}
static xsltTemplatePtr
templPop(xsltTransformContextPtr ctxt)
{
    xsltTemplatePtr ret;

    if (ctxt->templNr <= 0)
        return (0);
    ctxt->templNr--;
    if (ctxt->templNr > 0)
        ctxt->templ = ctxt->templTab[ctxt->templNr - 1];
    else
        ctxt->templ = (xsltTemplatePtr) 0;
    ret = ctxt->templTab[ctxt->templNr];
    ctxt->templTab[ctxt->templNr] = 0;
    return (ret);
}

void
xsltLocalVariablePop(xsltTransformContextPtr ctxt, int limitNr, int level)
{
    xsltStackElemPtr variable;

    if (ctxt->varsNr <= 0)
        return;

    do {
	if (ctxt->varsNr <= limitNr)
	    break;
	variable = ctxt->varsTab[ctxt->varsNr - 1];
	if (variable->level <= level)
	    break;
	if (variable->level >= 0)
	    xsltFreeStackElemList(variable);
	ctxt->varsNr--;
    } while (ctxt->varsNr != 0);
    if (ctxt->varsNr > 0)
        ctxt->vars = ctxt->varsTab[ctxt->varsNr - 1];
    else
        ctxt->vars = NULL;
}

static void
xsltTemplateParamsCleanup(xsltTransformContextPtr ctxt)
{
    xsltStackElemPtr param;

    for (; ctxt->varsNr > ctxt->varsBase; ctxt->varsNr--) {
	param = ctxt->varsTab[ctxt->varsNr -1];
	if (param->level >= 0) {
	    xsltFreeStackElemList(param);
	}
    }
    if (ctxt->varsNr > 0)
        ctxt->vars = ctxt->varsTab[ctxt->varsNr - 1];
    else
        ctxt->vars = NULL;
}

static int
profPush(xsltTransformContextPtr ctxt, long value)
{
    if (ctxt->profMax == 0) {
        ctxt->profMax = 4;
        ctxt->profTab =
            (long *) xmlMalloc(ctxt->profMax * sizeof(ctxt->profTab[0]));
        if (ctxt->profTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "malloc failed !\n");
            return (0);
        }
    }
    if (ctxt->profNr >= ctxt->profMax) {
        ctxt->profMax *= 2;
        ctxt->profTab =
            (long *) xmlRealloc(ctxt->profTab,
                                ctxt->profMax * sizeof(ctxt->profTab[0]));
        if (ctxt->profTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
            return (0);
        }
    }
    ctxt->profTab[ctxt->profNr] = value;
    ctxt->prof = value;
    return (ctxt->profNr++);
}
static long
profPop(xsltTransformContextPtr ctxt)
{
    long ret;

    if (ctxt->profNr <= 0)
        return (0);
    ctxt->profNr--;
    if (ctxt->profNr > 0)
        ctxt->prof = ctxt->profTab[ctxt->profNr - 1];
    else
        ctxt->prof = (long) 0;
    ret = ctxt->profTab[ctxt->profNr];
    ctxt->profTab[ctxt->profNr] = 0;
    return (ret);
}


static int xsltDoXIncludeDefault = 0;

void
xsltSetXIncludeDefault(int xinclude) {
    xsltDoXIncludeDefault = (xinclude != 0);
}

int
xsltGetXIncludeDefault(void) {
    return(xsltDoXIncludeDefault);
}

unsigned long xsltDefaultTrace = (unsigned long) XSLT_TRACE_ALL;

void xsltDebugSetDefaultTrace(xsltDebugTraceCodes val) {
	xsltDefaultTrace = val;
}

xsltDebugTraceCodes xsltDebugGetDefaultTrace() {
	return xsltDefaultTrace;
}


static xsltTransformCachePtr
xsltTransformCacheCreate(void)
{
    xsltTransformCachePtr ret;

    ret = (xsltTransformCachePtr) xmlMalloc(sizeof(xsltTransformCache));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
	    "xsltTransformCacheCreate : malloc failed\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltTransformCache));
    return(ret);
}

static void
xsltTransformCacheFree(xsltTransformCachePtr cache)
{
    if (cache == NULL)
	return;
    if (cache->RVT) {
	xmlDocPtr tmp, cur = cache->RVT;
	while (cur) {
	    tmp = cur;
	    cur = (xmlDocPtr) cur->next;
	    if (tmp->_private != NULL) {
		xsltFreeDocumentKeys((xsltDocumentPtr) tmp->_private);
		xmlFree(tmp->_private);
	    }
	    xmlFreeDoc(tmp);
	}
    }
    if (cache->stackItems) {
	xsltStackElemPtr tmp, cur = cache->stackItems;
	while (cur) {
	    tmp = cur;
	    cur = cur->next;
	    xmlFree(tmp);
	}
    }
    xmlFree(cache);
}

xsltTransformContextPtr
xsltNewTransformContext(xsltStylesheetPtr style, xmlDocPtr doc) {
    xsltTransformContextPtr cur;
    xsltDocumentPtr docu;
    int i;

    xsltInitGlobals();

    cur = (xsltTransformContextPtr) xmlMalloc(sizeof(xsltTransformContext));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr)doc,
		"xsltNewTransformContext : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltTransformContext));

    cur->cache = xsltTransformCacheCreate();
    if (cur->cache == NULL)
	goto internal_err;
    cur->dict = xmlDictCreateSub(style->dict);
    cur->internalized = ((style->internalized) && (cur->dict != NULL));
#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
	     "Creating sub-dictionary from stylesheet for transformation\n");
#endif

    cur->templTab = (xsltTemplatePtr *)
	        xmlMalloc(10 * sizeof(xsltTemplatePtr));
    if (cur->templTab == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr) doc,
		"xsltNewTransformContext: out of memory\n");
	goto internal_err;
    }
    cur->templNr = 0;
    cur->templMax = 5;
    cur->templ = NULL;

    cur->varsTab = (xsltStackElemPtr *)
	        xmlMalloc(10 * sizeof(xsltStackElemPtr));
    if (cur->varsTab == NULL) {
        xmlGenericError(xmlGenericErrorContext,
		"xsltNewTransformContext: out of memory\n");
	goto internal_err;
    }
    cur->varsNr = 0;
    cur->varsMax = 10;
    cur->vars = NULL;
    cur->varsBase = 0;

    cur->profTab = NULL;
    cur->profNr = 0;
    cur->profMax = 0;
    cur->prof = 0;

    cur->style = style;
    xmlXPathInit();
    cur->xpathCtxt = xmlXPathNewContext(doc);
    if (cur->xpathCtxt == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr) doc,
		"xsltNewTransformContext : xmlXPathNewContext failed\n");
	goto internal_err;
    }
    if (xmlXPathContextSetCache(cur->xpathCtxt, 1, -1, 0) == -1)
	goto internal_err;
    if (style->extrasNr != 0) {
	cur->extrasMax = style->extrasNr + 20;
	cur->extras = (xsltRuntimeExtraPtr)
	    xmlMalloc(cur->extrasMax * sizeof(xsltRuntimeExtra));
	if (cur->extras == NULL) {
	    xmlGenericError(xmlGenericErrorContext,
		    "xsltNewTransformContext: out of memory\n");
	    goto internal_err;
	}
	cur->extrasNr = style->extrasNr;
	for (i = 0;i < cur->extrasMax;i++) {
	    cur->extras[i].info = NULL;
	    cur->extras[i].deallocate = NULL;
	    cur->extras[i].val.ptr = NULL;
	}
    } else {
	cur->extras = NULL;
	cur->extrasNr = 0;
	cur->extrasMax = 0;
    }

    XSLT_REGISTER_VARIABLE_LOOKUP(cur);
    XSLT_REGISTER_FUNCTION_LOOKUP(cur);
    cur->xpathCtxt->nsHash = style->nsHash;
    xsltInitCtxtExts(cur);
    if (xslDebugStatus == XSLT_DEBUG_NONE)
        xmlXPathOrderDocElems(doc);
    cur->parserOptions = XSLT_PARSE_OPTIONS;
    docu = xsltNewDocument(cur, doc);
    if (docu == NULL) {
	xsltTransformError(cur, NULL, (xmlNodePtr)doc,
		"xsltNewTransformContext : xsltNewDocument failed\n");
	goto internal_err;
    }
    docu->main = 1;
    cur->document = docu;
    cur->inst = NULL;
    cur->outputFile = NULL;
    cur->sec = xsltGetDefaultSecurityPrefs();
    cur->debugStatus = xslDebugStatus;
    cur->traceCode = (unsigned long*) &xsltDefaultTrace;
    cur->xinclude = xsltGetXIncludeDefault();
    cur->keyInitLevel = 0;

    return(cur);

internal_err:
    if (cur != NULL)
	xsltFreeTransformContext(cur);
    return(NULL);
}

void
xsltFreeTransformContext(xsltTransformContextPtr ctxt) {
    if (ctxt == NULL)
	return;

    xsltShutdownCtxtExts(ctxt);

    if (ctxt->xpathCtxt != NULL) {
	ctxt->xpathCtxt->nsHash = NULL;
	xmlXPathFreeContext(ctxt->xpathCtxt);
    }
    if (ctxt->templTab != NULL)
	xmlFree(ctxt->templTab);
    if (ctxt->varsTab != NULL)
	xmlFree(ctxt->varsTab);
    if (ctxt->profTab != NULL)
	xmlFree(ctxt->profTab);
    if ((ctxt->extrasNr > 0) && (ctxt->extras != NULL)) {
	int i;

	for (i = 0;i < ctxt->extrasNr;i++) {
	    if ((ctxt->extras[i].deallocate != NULL) &&
		(ctxt->extras[i].info != NULL))
		ctxt->extras[i].deallocate(ctxt->extras[i].info);
	}
	xmlFree(ctxt->extras);
    }
    xsltFreeGlobalVariables(ctxt);
    xsltFreeDocuments(ctxt);
    xsltFreeCtxtExts(ctxt);
    xsltFreeRVTs(ctxt);
    xsltTransformCacheFree(ctxt->cache);
    xmlDictFree(ctxt->dict);
#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
                     "freeing transformation dictionary\n");
#endif
    memset(ctxt, -1, sizeof(xsltTransformContext));
    xmlFree(ctxt);
}


xmlNodePtr xsltCopyTree(xsltTransformContextPtr ctxt,
                        xmlNodePtr node, xmlNodePtr insert, int literal);

static xmlNodePtr
xsltAddChild(xmlNodePtr parent, xmlNodePtr cur) {
   xmlNodePtr ret;

   if ((cur == NULL) || (parent == NULL))
       return(NULL);
   if (parent == NULL) {
       xmlFreeNode(cur);
       return(NULL);
   }
   ret = xmlAddChild(parent, cur);

   return(ret);
}

static xmlNodePtr
xsltAddTextString(xsltTransformContextPtr ctxt, xmlNodePtr target,
		  const xmlChar *string, int len) {
    if ((len <= 0) || (string == NULL) || (target == NULL))
        return(target);

    if (ctxt->lasttext == target->content) {

	if (ctxt->lasttuse + len >= ctxt->lasttsize) {
	    xmlChar *newbuf;
	    int size;

	    size = ctxt->lasttsize + len + 100;
	    size *= 2;
	    newbuf = (xmlChar *) xmlRealloc(target->content,size);
	    if (newbuf == NULL) {
		xsltTransformError(ctxt, NULL, target,
		 "xsltCopyText: text allocation failed\n");
		return(NULL);
	    }
	    ctxt->lasttsize = size;
	    ctxt->lasttext = newbuf;
	    target->content = newbuf;
	}
	memcpy(&(target->content[ctxt->lasttuse]), string, len);
	ctxt->lasttuse += len;
	target->content[ctxt->lasttuse] = 0;
    } else {
	xmlNodeAddContent(target, string);
	ctxt->lasttext = target->content;
	len = xmlStrlen(target->content);
	ctxt->lasttsize = len;
	ctxt->lasttuse = len;
    }
    return(target);
}

xmlNodePtr
xsltCopyTextString(xsltTransformContextPtr ctxt, xmlNodePtr target,
	           const xmlChar *string, int noescape)
{
    xmlNodePtr copy;
    int len;

    if (string == NULL)
	return(NULL);

#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_COPY_TEXT,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltCopyTextString: copy text %s\n",
		     string));
#endif

    if ((target == NULL) || (target->children == NULL)) {
	ctxt->lasttext = NULL;
    }

    
    len = xmlStrlen(string);
    if ((ctxt->type == XSLT_OUTPUT_XML) &&
	(ctxt->style->cdataSection != NULL) &&
	(target != NULL) &&
	(target->type == XML_ELEMENT_NODE) &&
	(((target->ns == NULL) &&
	  (xmlHashLookup2(ctxt->style->cdataSection,
		          target->name, NULL) != NULL)) ||
	 ((target->ns != NULL) &&
	  (xmlHashLookup2(ctxt->style->cdataSection,
	                  target->name, target->ns->href) != NULL))))
    {
	if ((target->last != NULL) &&
	    (target->last->type == XML_CDATA_SECTION_NODE))
	{
	    return(xsltAddTextString(ctxt, target->last, string, len));
	}
	copy = xmlNewCDataBlock(ctxt->output, string, len);
    } else if (noescape) {
	if ((target != NULL) && (target->last != NULL) &&
	    (target->last->type == XML_TEXT_NODE) &&
	    (target->last->name == xmlStringTextNoenc))
	{
	    return(xsltAddTextString(ctxt, target->last, string, len));
	}
	copy = xmlNewTextLen(string, len);
	if (copy != NULL)
	    copy->name = xmlStringTextNoenc;
    } else {
	if ((target != NULL) && (target->last != NULL) &&
	    (target->last->type == XML_TEXT_NODE) &&
	    (target->last->name == xmlStringText)) {
	    return(xsltAddTextString(ctxt, target->last, string, len));
	}
	copy = xmlNewTextLen(string, len);
    }
    if (copy != NULL) {
	if (target != NULL)
	    copy = xsltAddChild(target, copy);
	ctxt->lasttext = copy->content;
	ctxt->lasttsize = len;
	ctxt->lasttuse = len;
    } else {
	xsltTransformError(ctxt, NULL, target,
			 "xsltCopyTextString: text copy failed\n");
	ctxt->lasttext = NULL;
    }
    return(copy);
}

static xmlNodePtr
xsltCopyText(xsltTransformContextPtr ctxt, xmlNodePtr target,
	     xmlNodePtr cur, int interned)
{
    xmlNodePtr copy;

    if ((cur->type != XML_TEXT_NODE) &&
	(cur->type != XML_CDATA_SECTION_NODE))
	return(NULL);
    if (cur->content == NULL)
	return(NULL);

#ifdef WITH_XSLT_DEBUG_PROCESS
    if (cur->type == XML_CDATA_SECTION_NODE) {
	XSLT_TRACE(ctxt,XSLT_TRACE_COPY_TEXT,xsltGenericDebug(xsltGenericDebugContext,
			 "xsltCopyText: copy CDATA text %s\n",
			 cur->content));
    } else if (cur->name == xmlStringTextNoenc) {
	XSLT_TRACE(ctxt,XSLT_TRACE_COPY_TEXT,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltCopyText: copy unescaped text %s\n",
			 cur->content));
    } else {
	XSLT_TRACE(ctxt,XSLT_TRACE_COPY_TEXT,xsltGenericDebug(xsltGenericDebugContext,
			 "xsltCopyText: copy text %s\n",
			 cur->content));
    }
#endif

    if ((target == NULL) || (target->children == NULL)) {
	ctxt->lasttext = NULL;
    }

    if ((ctxt->style->cdataSection != NULL) &&
	(ctxt->type == XSLT_OUTPUT_XML) &&
	(target != NULL) &&
	(target->type == XML_ELEMENT_NODE) &&
	(((target->ns == NULL) &&
	  (xmlHashLookup2(ctxt->style->cdataSection,
		          target->name, NULL) != NULL)) ||
	 ((target->ns != NULL) &&
	  (xmlHashLookup2(ctxt->style->cdataSection,
	                  target->name, target->ns->href) != NULL))))
    {
	if ((target->last != NULL) &&
	     (target->last->type == XML_CDATA_SECTION_NODE))
	{
	    copy = xsltAddTextString(ctxt, target->last, cur->content,
		xmlStrlen(cur->content));
	    goto exit;
	} else {
	    unsigned int len;

	    len = xmlStrlen(cur->content);
	    copy = xmlNewCDataBlock(ctxt->output, cur->content, len);
	    if (copy == NULL)
		goto exit;
	    ctxt->lasttext = copy->content;
	    ctxt->lasttsize = len;
	    ctxt->lasttuse = len;
	}
    } else if ((target != NULL) &&
	(target->last != NULL) &&
	
	(((target->last->type == XML_TEXT_NODE) &&
	(target->last->name == cur->name)) ||
        
	(((target->last->type == XML_CDATA_SECTION_NODE) &&
	(cur->name == xmlStringTextNoenc)))))
    {
	copy = xsltAddTextString(ctxt, target->last, cur->content,
	    xmlStrlen(cur->content));
	goto exit;
    } else if ((interned) && (target != NULL) &&
	(target->doc != NULL) &&
	(target->doc->dict == ctxt->dict))
    {
        copy = xmlNewTextLen(NULL, 0);
	if (copy == NULL)
	    goto exit;
	if (cur->name == xmlStringTextNoenc)
	    copy->name = xmlStringTextNoenc;

	if (xmlDictOwns(ctxt->dict, cur->content))
	    copy->content = cur->content;
	else {
	    if ((copy->content = xmlStrdup(cur->content)) == NULL)
		return NULL;
	}
    } else {
        unsigned int len;

	len = xmlStrlen(cur->content);
	copy = xmlNewTextLen(cur->content, len);
	if (copy == NULL)
	    goto exit;
	if (cur->name == xmlStringTextNoenc)
	    copy->name = xmlStringTextNoenc;
	ctxt->lasttext = copy->content;
	ctxt->lasttsize = len;
	ctxt->lasttuse = len;
    }
    if (copy != NULL) {
	if (target != NULL) {
	    copy->doc = target->doc;
	    copy = xsltAddChild(target, copy);
	}
    } else {
	xsltTransformError(ctxt, NULL, target,
			 "xsltCopyText: text copy failed\n");
    }

exit:
    if ((copy == NULL) || (copy->content == NULL)) {
	xsltTransformError(ctxt, NULL, target,
	    "Internal error in xsltCopyText(): "
	    "Failed to copy the string.\n");
	ctxt->state = XSLT_STATE_STOPPED;
    }
    return(copy);
}

static xmlAttrPtr
xsltShallowCopyAttr(xsltTransformContextPtr ctxt, xmlNodePtr invocNode,
	     xmlNodePtr target, xmlAttrPtr attr)
{
    xmlAttrPtr copy;
    xmlChar *value;

    if (attr == NULL)
	return(NULL);

    if (target->type != XML_ELEMENT_NODE) {
	xsltTransformError(ctxt, NULL, invocNode,
	    "Cannot add an attribute node to a non-element node.\n");
	return(NULL);
    }

    if (target->children != NULL) {
	xsltTransformError(ctxt, NULL, invocNode,
	    "Attribute nodes must be added before "
	    "any child nodes to an element.\n");
	return(NULL);
    }

    value = xmlNodeListGetString(attr->doc, attr->children, 1);
    if (attr->ns != NULL) {
	xmlNsPtr ns;

	ns = xsltGetSpecialNamespace(ctxt, invocNode,
	    attr->ns->href, attr->ns->prefix, target);
	if (ns == NULL) {
	    xsltTransformError(ctxt, NULL, invocNode,
		"Namespace fixup error: Failed to acquire an in-scope "
		"namespace binding of the copied attribute '{%s}%s'.\n",
		attr->ns->href, attr->name);
	}
	copy = xmlSetNsProp(target, ns, attr->name, value);
    } else {
	copy = xmlSetNsProp(target, NULL, attr->name, value);
    }
    if (value != NULL)
	xmlFree(value);

    if (copy == NULL)
	return(NULL);

#if 0
    value = xmlNodeListGetString(attr->doc, attr->children, 1);
    if (value != NULL) {
	txtNode = xmlNewDocText(target->doc, NULL);
	if (txtNode == NULL)
	    return(NULL);
	if ((target->doc != NULL) &&
	    (target->doc->dict != NULL))
	{
	    txtNode->content =
		(xmlChar *) xmlDictLookup(target->doc->dict,
		    BAD_CAST value, -1);
	    xmlFree(value);
	} else
	    txtNode->content = value;
	copy->children = txtNode;
    }
#endif

    return(copy);
}

static int
xsltCopyAttrListNoOverwrite(xsltTransformContextPtr ctxt,
			    xmlNodePtr invocNode,
			    xmlNodePtr target, xmlAttrPtr attr)
{
    xmlAttrPtr copy;
    xmlNsPtr origNs = NULL, copyNs = NULL;
    xmlChar *value;

    while (attr != NULL) {
	if (attr->ns != origNs) {
	    origNs = attr->ns;
	    if (attr->ns != NULL) {
		copyNs = xsltGetSpecialNamespace(ctxt, invocNode,
		    attr->ns->href, attr->ns->prefix, target);
		if (copyNs == NULL)
		    return(-1);
	    } else
		copyNs = NULL;
	}
	if ((attr->children) && (attr->children->type == XML_TEXT_NODE) &&
            (attr->children->next == NULL)) {
            copy = xmlNewNsProp(target, copyNs, attr->name,
                                attr->children->content);
        } else if (attr->children != NULL) {
	    value = xmlNodeListGetString(attr->doc, attr->children, 1);
            copy = xmlNewNsProp(target, copyNs, attr->name, BAD_CAST value);
	    xmlFree(value);
        } else {
            copy = xmlNewNsProp(target, copyNs, attr->name, NULL);
        }

	if (copy == NULL)
	    return(-1);

	attr = attr->next;
    }
    return(0);
}

static xmlNodePtr
xsltShallowCopyElem(xsltTransformContextPtr ctxt, xmlNodePtr node,
		    xmlNodePtr insert, int isLRE)
{
    xmlNodePtr copy;

    if ((node->type == XML_DTD_NODE) || (insert == NULL))
	return(NULL);
    if ((node->type == XML_TEXT_NODE) ||
	(node->type == XML_CDATA_SECTION_NODE))
	return(xsltCopyText(ctxt, insert, node, 0));

    copy = xmlDocCopyNode(node, insert->doc, 0);
    if (copy != NULL) {
	copy->doc = ctxt->output;
	copy = xsltAddChild(insert, copy);

	if (node->type == XML_ELEMENT_NODE) {
	    if (node->nsDef != NULL) {
		if (isLRE)
		    xsltCopyNamespaceList(ctxt, copy, node->nsDef);
		else
		    xsltCopyNamespaceListInternal(copy, node->nsDef);
	    }

	    if (node->ns != NULL) {
		if (isLRE) {
		    copy->ns = xsltGetNamespace(ctxt, node, node->ns, copy);
		} else {
		    copy->ns = xsltGetSpecialNamespace(ctxt,
			node, node->ns->href, node->ns->prefix, copy);

		}
	    } else if ((insert->type == XML_ELEMENT_NODE) &&
		       (insert->ns != NULL))
	    {
		xsltGetSpecialNamespace(ctxt, node, NULL, NULL, copy);
	    }
	}
    } else {
	xsltTransformError(ctxt, NULL, node,
		"xsltShallowCopyElem: copy %s failed\n", node->name);
    }
    return(copy);
}

static xmlNodePtr
xsltCopyTreeList(xsltTransformContextPtr ctxt, xmlNodePtr invocNode,
		 xmlNodePtr list,
		 xmlNodePtr insert, int isLRE, int topElemVisited)
{
    xmlNodePtr copy, ret = NULL;

    while (list != NULL) {
	copy = xsltCopyTreeInternal(ctxt, invocNode,
	    list, insert, isLRE, topElemVisited);
	if (copy != NULL) {
	    if (ret == NULL) {
		ret = copy;
	    }
	}
	list = list->next;
    }
    return(ret);
}

static xmlNsPtr
xsltCopyNamespaceListInternal(xmlNodePtr elem, xmlNsPtr ns) {
    xmlNsPtr ret = NULL;
    xmlNsPtr p = NULL, q, luNs;

    if (ns == NULL)
	return(NULL);
    if ((elem != NULL) && (elem->type != XML_ELEMENT_NODE))
	elem = NULL;

    do {
	if (ns->type != XML_NAMESPACE_DECL)
	    break;
	if (elem != NULL) {
	    if ((elem->ns != NULL) &&
		xmlStrEqual(elem->ns->prefix, ns->prefix) &&
		xmlStrEqual(elem->ns->href, ns->href))
	    {
		ns = ns->next;
		continue;
	    }
	    luNs = xmlSearchNs(elem->doc, elem, ns->prefix);
	    if ((luNs != NULL) && (xmlStrEqual(luNs->href, ns->href)))
	    {
		ns = ns->next;
		continue;
	    }
	}
	q = xmlNewNs(elem, ns->href, ns->prefix);
	if (p == NULL) {
	    ret = p = q;
	} else if (q != NULL) {
	    p->next = q;
	    p = q;
	}
	ns = ns->next;
    } while (ns != NULL);
    return(ret);
}

static xmlNsPtr
xsltShallowCopyNsNode(xsltTransformContextPtr ctxt,
		      xmlNodePtr invocNode,
		      xmlNodePtr insert,
		      xmlNsPtr ns)
{
    xmlNsPtr tmpns;

    if ((insert == NULL) || (insert->type != XML_ELEMENT_NODE))
	return(NULL);

    if (insert->children != NULL) {
	xsltTransformError(ctxt, NULL, invocNode,
	    "Namespace nodes must be added before "
	    "any child nodes are added to an element.\n");
	return(NULL);
    }
    if (ns->prefix == NULL) {
	if (insert->ns == NULL)
	    goto occupied;
    } else if ((ns->prefix[0] == 'x') &&
	xmlStrEqual(ns->prefix, BAD_CAST "xml"))
    {
	return(NULL);
    }

    if (insert->nsDef != NULL) {
	tmpns = insert->nsDef;
	do {
	    if ((tmpns->prefix == NULL) == (ns->prefix == NULL)) {
		if ((tmpns->prefix == ns->prefix) ||
		    xmlStrEqual(tmpns->prefix, ns->prefix))
		{
		    if (xmlStrEqual(tmpns->href, ns->href))
			return(NULL);
		    goto occupied;
		}
	    }
	    tmpns = tmpns->next;
	} while (tmpns != NULL);
    }
    tmpns = xmlSearchNs(insert->doc, insert, ns->prefix);
    if ((tmpns != NULL) && xmlStrEqual(tmpns->href, ns->href))
	return(NULL);
    return(xmlNewNs(insert, ns->href, ns->prefix));

occupied:
    return(NULL);
}

static xmlNodePtr
xsltCopyTreeInternal(xsltTransformContextPtr ctxt,
		     xmlNodePtr invocNode,
		     xmlNodePtr node,
		     xmlNodePtr insert, int isLRE, int topElemVisited)
{
    xmlNodePtr copy;

    if (node == NULL)
	return(NULL);
    switch (node->type) {
        case XML_ELEMENT_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
        case XML_DOCB_DOCUMENT_NODE:
#endif
	    break;
        case XML_TEXT_NODE: {
	    int noenc = (node->name == xmlStringTextNoenc);
	    return(xsltCopyTextString(ctxt, insert, node->content, noenc));
	    }
        case XML_CDATA_SECTION_NODE:
	    return(xsltCopyTextString(ctxt, insert, node->content, 0));
        case XML_ATTRIBUTE_NODE:
	    return((xmlNodePtr)
		xsltShallowCopyAttr(ctxt, invocNode, insert, (xmlAttrPtr) node));
        case XML_NAMESPACE_DECL:
	    return((xmlNodePtr) xsltShallowCopyNsNode(ctxt, invocNode,
		insert, (xmlNsPtr) node));

        case XML_DOCUMENT_TYPE_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
            return(NULL);
    }
    if (XSLT_IS_RES_TREE_FRAG(node)) {
	if (node->children != NULL)
	    copy = xsltCopyTreeList(ctxt, invocNode,
		node->children, insert, 0, 0);
	else
	    copy = NULL;
	return(copy);
    }
    copy = xmlDocCopyNode(node, insert->doc, 0);
    if (copy != NULL) {
	copy->doc = ctxt->output;
	copy = xsltAddChild(insert, copy);
	if (insert->last != copy)
	    return(insert->last);
	copy->next = NULL;

	if (node->type == XML_ELEMENT_NODE) {
	    if ((topElemVisited == 0) &&
		(node->parent != NULL) &&
		(node->parent->type != XML_DOCUMENT_NODE) &&
		(node->parent->type != XML_HTML_DOCUMENT_NODE))
	    {
		xmlNsPtr *nsList, *curns, ns;


		nsList = xmlGetNsList(node->doc, node);
		if (nsList != NULL) {
		    curns = nsList;
		    do {
			ns = xmlSearchNs(insert->doc, insert,
			    (*curns)->prefix);

			if ((ns == NULL) ||
			    (! xmlStrEqual(ns->href, (*curns)->href)))
			{
			    ns = NULL;
#if 0
			    ns = xmlSearchNsByHref(insert->doc,
				insert, (*curns)->href);
#endif
			}
			if (ns == NULL) {
			    ns = xmlNewNs(copy, (*curns)->href,
				(*curns)->prefix);
			    
			}
			if (node->ns == *curns) {
			    copy->ns = ns;
			}
			curns++;
		    } while (*curns != NULL);
		    xmlFree(nsList);
		}
	    } else if (node->nsDef != NULL) {
		if (node->nsDef != NULL) {
		    if (isLRE)
			xsltCopyNamespaceList(ctxt, copy, node->nsDef);
		    else
			xsltCopyNamespaceListInternal(copy, node->nsDef);
		}
	    }
	    if (node->ns != NULL) {
		if (copy->ns == NULL) {
		    copy->ns = xsltGetSpecialNamespace(ctxt, invocNode,
			node->ns->href, node->ns->prefix, copy);
		}
	    } else if ((insert->type == XML_ELEMENT_NODE) &&
		(insert->ns != NULL))
	    {
		xsltGetSpecialNamespace(ctxt, invocNode, NULL, NULL, copy);
	    }
	    if (node->properties != NULL) {
		xsltCopyAttrListNoOverwrite(ctxt, invocNode,
		    copy, node->properties);
	    }
	    if (topElemVisited == 0)
		topElemVisited = 1;
	}
	if (node->children != NULL) {
	    xsltCopyTreeList(ctxt, invocNode,
		node->children, copy, isLRE, topElemVisited);
	}
    } else {
	xsltTransformError(ctxt, NULL, invocNode,
	    "xsltCopyTreeInternal: Copying of '%s' failed.\n", node->name);
    }
    return(copy);
}

xmlNodePtr
xsltCopyTree(xsltTransformContextPtr ctxt, xmlNodePtr node,
	     xmlNodePtr insert, int literal)
{
    return(xsltCopyTreeInternal(ctxt, node, node, insert, literal, 0));

}


static int
xsltApplyFallbacks(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst) {

    xmlNodePtr child;
    int ret = 0;

    if ((ctxt == NULL) || (node == NULL) || (inst == NULL) ||
	(inst->children == NULL))
	return(0);

    child = inst->children;
    while (child != NULL) {
        if ((IS_XSLT_ELEM(child)) &&
            (xmlStrEqual(child->name, BAD_CAST "fallback"))) {
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
			     "applying xsl:fallback\n");
#endif
	    ret++;
	    xsltApplySequenceConstructor(ctxt, node, child->children,
		NULL);
	}
	child = child->next;
    }
    return(ret);
}


static void
xsltDefaultProcessOneNode(xsltTransformContextPtr ctxt, xmlNodePtr node,
			  xsltStackElemPtr params) {
    xmlNodePtr copy;
    xmlNodePtr delete = NULL, cur;
    int nbchild = 0, oldSize;
    int childno = 0, oldPos;
    xsltTemplatePtr template;

    CHECK_STOPPED;
    switch (node->type) {
	case XML_DOCUMENT_NODE:
	case XML_HTML_DOCUMENT_NODE:
	case XML_ELEMENT_NODE:
	    break;
	case XML_CDATA_SECTION_NODE:
#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltDefaultProcessOneNode: copy CDATA %s\n",
		node->content));
#endif
	    copy = xsltCopyText(ctxt, ctxt->insert, node, 0);
	    if (copy == NULL) {
		xsltTransformError(ctxt, NULL, node,
		 "xsltDefaultProcessOneNode: cdata copy failed\n");
	    }
	    return;
	case XML_TEXT_NODE:
#ifdef WITH_XSLT_DEBUG_PROCESS
	    if (node->content == NULL) {
		XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltDefaultProcessOneNode: copy empty text\n"));
		return;
	    } else {
		XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltDefaultProcessOneNode: copy text %s\n",
			node->content));
            }
#endif
	    copy = xsltCopyText(ctxt, ctxt->insert, node, 0);
	    if (copy == NULL) {
		xsltTransformError(ctxt, NULL, node,
		 "xsltDefaultProcessOneNode: text copy failed\n");
	    }
	    return;
	case XML_ATTRIBUTE_NODE:
	    cur = node->children;
	    while ((cur != NULL) && (cur->type != XML_TEXT_NODE))
		cur = cur->next;
	    if (cur == NULL) {
		xsltTransformError(ctxt, NULL, node,
		 "xsltDefaultProcessOneNode: no text for attribute\n");
	    } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
		if (cur->content == NULL) {
		    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltDefaultProcessOneNode: copy empty text\n"));
		} else {
		    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltDefaultProcessOneNode: copy text %s\n",
			cur->content));
                }
#endif
		copy = xsltCopyText(ctxt, ctxt->insert, cur, 0);
		if (copy == NULL) {
		    xsltTransformError(ctxt, NULL, node,
		     "xsltDefaultProcessOneNode: text copy failed\n");
		}
	    }
	    return;
	default:
	    return;
    }
    cur = node->children;
    while (cur != NULL) {
	switch (cur->type) {
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
	    case XML_DOCUMENT_NODE:
	    case XML_HTML_DOCUMENT_NODE:
	    case XML_ELEMENT_NODE:
	    case XML_PI_NODE:
	    case XML_COMMENT_NODE:
		nbchild++;
		break;
            case XML_DTD_NODE:
		
		if (cur->next != NULL)
		    cur->next->prev = cur->prev;
		if (cur->prev != NULL)
		    cur->prev->next = cur->next;
		break;
	    default:
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltDefaultProcessOneNode: skipping node type %d\n",
		                 cur->type));
#endif
		delete = cur;
	}
	cur = cur->next;
	if (delete != NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltDefaultProcessOneNode: removing ignorable blank node\n"));
#endif
	    xmlUnlinkNode(delete);
	    xmlFreeNode(delete);
	    delete = NULL;
	}
    }
    if (delete != NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltDefaultProcessOneNode: removing ignorable blank node\n"));
#endif
	xmlUnlinkNode(delete);
	xmlFreeNode(delete);
	delete = NULL;
    }

    oldSize = ctxt->xpathCtxt->contextSize;
    oldPos = ctxt->xpathCtxt->proximityPosition;
    cur = node->children;
    while (cur != NULL) {
	childno++;
	switch (cur->type) {
	    case XML_DOCUMENT_NODE:
	    case XML_HTML_DOCUMENT_NODE:
	    case XML_ELEMENT_NODE:
		ctxt->xpathCtxt->contextSize = nbchild;
		ctxt->xpathCtxt->proximityPosition = childno;
		xsltProcessOneNode(ctxt, cur, params);
		break;
	    case XML_CDATA_SECTION_NODE:
		template = xsltGetTemplate(ctxt, cur, NULL);
		if (template) {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltDefaultProcessOneNode: applying template for CDATA %s\n",
				     cur->content));
#endif
		    xsltApplyXSLTTemplate(ctxt, cur, template->content,
			template, params);
		} else  {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltDefaultProcessOneNode: copy CDATA %s\n",
				     cur->content));
#endif
		    copy = xsltCopyText(ctxt, ctxt->insert, cur, 0);
		    if (copy == NULL) {
			xsltTransformError(ctxt, NULL, cur,
			    "xsltDefaultProcessOneNode: cdata copy failed\n");
		    }
		}
		break;
	    case XML_TEXT_NODE:
		template = xsltGetTemplate(ctxt, cur, NULL);
		if (template) {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltDefaultProcessOneNode: applying template for text %s\n",
				     cur->content));
#endif
		    ctxt->xpathCtxt->contextSize = nbchild;
		    ctxt->xpathCtxt->proximityPosition = childno;
		    xsltApplyXSLTTemplate(ctxt, cur, template->content,
			template, params);
		} else  {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    if (cur->content == NULL) {
			XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
			 "xsltDefaultProcessOneNode: copy empty text\n"));
		    } else {
			XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltDefaultProcessOneNode: copy text %s\n",
					 cur->content));
                    }
#endif
		    copy = xsltCopyText(ctxt, ctxt->insert, cur, 0);
		    if (copy == NULL) {
			xsltTransformError(ctxt, NULL, cur,
			    "xsltDefaultProcessOneNode: text copy failed\n");
		    }
		}
		break;
	    case XML_PI_NODE:
	    case XML_COMMENT_NODE:
		template = xsltGetTemplate(ctxt, cur, NULL);
		if (template) {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    if (cur->type == XML_PI_NODE) {
			XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltDefaultProcessOneNode: template found for PI %s\n",
			                 cur->name));
		    } else if (cur->type == XML_COMMENT_NODE) {
			XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltDefaultProcessOneNode: template found for comment\n"));
                    }
#endif
		    ctxt->xpathCtxt->contextSize = nbchild;
		    ctxt->xpathCtxt->proximityPosition = childno;
		    xsltApplyXSLTTemplate(ctxt, cur, template->content,
			template, params);
		}
		break;
	    default:
		break;
	}
	cur = cur->next;
    }
    ctxt->xpathCtxt->contextSize = oldSize;
    ctxt->xpathCtxt->proximityPosition = oldPos;
}

void
xsltProcessOneNode(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	           xsltStackElemPtr withParams)
{
    xsltTemplatePtr templ;
    xmlNodePtr oldNode;

    templ = xsltGetTemplate(ctxt, contextNode, NULL);
    if (templ == NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	if (contextNode->type == XML_DOCUMENT_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for /\n"));
	} else if (contextNode->type == XML_CDATA_SECTION_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for CDATA\n"));
	} else if (contextNode->type == XML_ATTRIBUTE_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for attribute %s\n",
	                     ((xmlAttrPtr) contextNode)->name));
	} else  {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: no template found for %s\n", contextNode->name));
        }
#endif
	oldNode = ctxt->node;
	ctxt->node = contextNode;
	xsltDefaultProcessOneNode(ctxt, contextNode, withParams);
	ctxt->node = oldNode;
	return;
    }

    if (contextNode->type == XML_ATTRIBUTE_NODE) {
	xsltTemplatePtr oldCurTempRule = ctxt->currentTemplateRule;
	ctxt->currentTemplateRule = templ;

#ifdef WITH_XSLT_DEBUG_PROCESS
	XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: applying template '%s' for attribute %s\n",
	                 templ->match, contextNode->name));
#endif
	xsltApplyXSLTTemplate(ctxt, contextNode, templ->content, templ, withParams);

	ctxt->currentTemplateRule = oldCurTempRule;
    } else {
	xsltTemplatePtr oldCurTempRule = ctxt->currentTemplateRule;
	ctxt->currentTemplateRule = templ;

#ifdef WITH_XSLT_DEBUG_PROCESS
	if (contextNode->type == XML_DOCUMENT_NODE) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: applying template '%s' for /\n",
	                     templ->match));
	} else {
	    XSLT_TRACE(ctxt,XSLT_TRACE_PROCESS_NODE,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessOneNode: applying template '%s' for %s\n",
	                     templ->match, contextNode->name));
        }
#endif
	xsltApplyXSLTTemplate(ctxt, contextNode, templ->content, templ, withParams);

	ctxt->currentTemplateRule = oldCurTempRule;
    }
}

static xmlNodePtr
xsltDebuggerStartSequenceConstructor(xsltTransformContextPtr ctxt,
				     xmlNodePtr contextNode,
				     xmlNodePtr list,
				     xsltTemplatePtr templ,
				     int *addCallResult)
{
    xmlNodePtr debugedNode = NULL;

    if (ctxt->debugStatus != XSLT_DEBUG_NONE) {
        if (templ) {
            *addCallResult = xslAddCall(templ, templ->elem);
        } else {
            *addCallResult = xslAddCall(NULL, list);
        }
        switch (ctxt->debugStatus) {
            case XSLT_DEBUG_RUN_RESTART:
            case XSLT_DEBUG_QUIT:
                if (*addCallResult)
                    xslDropCall();
                return(NULL);
        }
        if (templ) {
            xslHandleDebugger(templ->elem, contextNode, templ, ctxt);
            debugedNode = templ->elem;
        } else if (list) {
            xslHandleDebugger(list, contextNode, templ, ctxt);
            debugedNode = list;
        } else if (ctxt->inst) {
            xslHandleDebugger(ctxt->inst, contextNode, templ, ctxt);
            debugedNode = ctxt->inst;
        }
    }
    return(debugedNode);
}

int
xsltLocalVariablePush(xsltTransformContextPtr ctxt,
		      xsltStackElemPtr variable,
		      int level)
{
    if (ctxt->varsMax == 0) {
	ctxt->varsMax = 10;
	ctxt->varsTab =
	    (xsltStackElemPtr *) xmlMalloc(ctxt->varsMax *
	    sizeof(ctxt->varsTab[0]));
	if (ctxt->varsTab == NULL) {
	    xmlGenericError(xmlGenericErrorContext, "malloc failed !\n");
	    return (-1);
	}
    }
    if (ctxt->varsNr >= ctxt->varsMax) {
	ctxt->varsMax *= 2;
	ctxt->varsTab =
	    (xsltStackElemPtr *) xmlRealloc(ctxt->varsTab,
	    ctxt->varsMax *
	    sizeof(ctxt->varsTab[0]));
	if (ctxt->varsTab == NULL) {
	    xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
	    return (-1);
	}
    }
    ctxt->varsTab[ctxt->varsNr++] = variable;
    ctxt->vars = variable;
    variable->level = level;
    return(0);
}

static void
xsltReleaseLocalRVTs(xsltTransformContextPtr ctxt, xmlDocPtr base)
{
    xmlDocPtr cur = ctxt->localRVT, tmp;

    while ((cur != NULL) && (cur != base)) {
	if (cur->psvi == (void *) ((long) 1)) {
	    cur = (xmlDocPtr) cur->next;
	} else {
	    tmp = cur;
	    cur = (xmlDocPtr) cur->next;

	    if (tmp == ctxt->localRVT)
		ctxt->localRVT = cur;

	    if (tmp == ctxt->localRVTBase)
		ctxt->localRVTBase = cur;

	    if (tmp->prev)
		tmp->prev->next = (xmlNodePtr) cur;
	    if (cur)
		cur->prev = tmp->prev;
	    xsltReleaseRVT(ctxt, tmp);
	}
    }
}

static void
xsltApplySequenceConstructor(xsltTransformContextPtr ctxt,
			     xmlNodePtr contextNode, xmlNodePtr list,
			     xsltTemplatePtr templ)
{
    xmlNodePtr oldInsert, oldInst, oldCurInst, oldContextNode;
    xmlNodePtr cur, insert, copy = NULL;
    int level = 0, oldVarsNr;
    xmlDocPtr oldLocalFragmentTop, oldLocalFragmentBase;

#ifdef XSLT_REFACTORED
    xsltStylePreCompPtr info;
#endif

#ifdef WITH_DEBUGGER
    int addCallResult = 0;
    xmlNodePtr debuggedNode = NULL;
#endif

    if (ctxt == NULL)
	return;

#ifdef WITH_DEBUGGER
    if (ctxt->debugStatus != XSLT_DEBUG_NONE) {
	debuggedNode =
	    xsltDebuggerStartSequenceConstructor(ctxt, contextNode,
		list, templ, &addCallResult);
	if (debuggedNode == NULL)
	    return;
    }
#endif

    if (list == NULL)
        return;
    CHECK_STOPPED;

    oldLocalFragmentTop = ctxt->localRVT;
    oldInsert = insert = ctxt->insert;
    oldInst = oldCurInst = ctxt->inst;
    oldContextNode = ctxt->node;
    oldVarsNr = ctxt->varsNr;
    cur = list;
    while (cur != NULL) {
        ctxt->inst = cur;

#ifdef WITH_DEBUGGER
        switch (ctxt->debugStatus) {
            case XSLT_DEBUG_RUN_RESTART:
            case XSLT_DEBUG_QUIT:
                break;

        }
#endif
        if (insert == NULL) {

#ifdef WITH_XSLT_DEBUG_PROCESS
            XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
		"xsltApplySequenceConstructor: insert == NULL !\n"));
#endif
            goto error;
        }

#ifdef WITH_DEBUGGER
        if ((ctxt->debugStatus != XSLT_DEBUG_NONE) && (debuggedNode != cur))
            xslHandleDebugger(cur, contextNode, templ, ctxt);
#endif

#ifdef XSLT_REFACTORED
	if (cur->type == XML_ELEMENT_NODE) {
	    info = (xsltStylePreCompPtr) cur->psvi;
	    if (info == NULL) {
		if (IS_XSLT_ELEM_FAST(cur) && IS_XSLT_NAME(cur, "message")) {
		    xsltMessage(ctxt, contextNode, cur);
		    goto skip_children;
		}
		xsltTransformError(ctxt, NULL, cur,
		    "Internal error in xsltApplySequenceConstructor(): "
		    "The element '%s' in the stylesheet has no compiled "
		    "representation.\n",
		    cur->name);
                goto skip_children;
            }

	    if (info->type == XSLT_FUNC_LITERAL_RESULT_ELEMENT) {
		xsltStyleItemLRElementInfoPtr lrInfo =
		    (xsltStyleItemLRElementInfoPtr) info;
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt, XSLT_TRACE_APPLY_TEMPLATE,
		    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltApplySequenceConstructor: copy literal result "
		    "element '%s'\n", cur->name));
#endif
		copy = xmlDocCopyNode(cur, insert->doc, 0);
		if (copy == NULL) {
		    xsltTransformError(ctxt, NULL, cur,
			"Internal error in xsltApplySequenceConstructor(): "
			"Failed to copy literal result element '%s'.\n",
			cur->name);
		    goto error;
		} else {
		    copy->doc = ctxt->output;
		    copy = xsltAddChild(insert, copy);
		    if (lrInfo->effectiveNs != NULL) {
			xsltEffectiveNsPtr effNs = lrInfo->effectiveNs;
			xmlNsPtr ns, lastns = NULL;

			while (effNs != NULL) {
			    ns = xmlSearchNs(copy->doc, copy, effNs->prefix);
			    if ((ns != NULL) &&
				(xmlStrEqual(ns->href, effNs->nsName)))
			    {
				effNs = effNs->next;
				continue;
			    }
			    ns = xmlNewNs(copy, effNs->nsName, effNs->prefix);
			    if (ns == NULL) {
				xsltTransformError(ctxt, NULL, cur,
				    "Internal error in "
				    "xsltApplySequenceConstructor(): "
				    "Failed to copy a namespace "
				    "declaration.\n");
				goto error;
			    }

			    if (lastns == NULL)
				copy->nsDef = ns;
			    else
				lastns->next =ns;
			    lastns = ns;

			    effNs = effNs->next;
			}

		    }
		    if (cur->ns != NULL) {
			copy->ns = xsltGetSpecialNamespace(ctxt, cur,
			    cur->ns->href, cur->ns->prefix, copy);
		    } else {
			if (copy->nsDef ||
			    ((insert != NULL) &&
			     (insert->type == XML_ELEMENT_NODE) &&
			     (insert->ns != NULL)))
			{
			    xsltGetSpecialNamespace(ctxt, cur,
				NULL, NULL, copy);
			}
		    }
		}
		if (cur->properties != NULL) {
		    xsltAttrListTemplateProcess(ctxt, copy, cur->properties);
		}
	    } else if (IS_XSLT_ELEM_FAST(cur)) {
		if (info->type == XSLT_FUNC_UNKOWN_FORWARDS_COMPAT) {
		    ctxt->insert = insert;
		    if (!xsltApplyFallbacks(ctxt, contextNode, cur)) {
			xsltTransformError(ctxt, NULL, cur,
			    "The is no fallback behaviour defined for "
			    "the unknown XSLT element '%s'.\n",
			    cur->name);
		    }
		    ctxt->insert = oldInsert;
		} else if (info->func != NULL) {
		    ctxt->insert = insert;

		    info->func(ctxt, contextNode, cur,
			(xsltElemPreCompPtr) info);

		    if (oldLocalFragmentTop != ctxt->localRVT)
			xsltReleaseLocalRVTs(ctxt, oldLocalFragmentTop);

		    ctxt->insert = oldInsert;
		} else if (info->type == XSLT_FUNC_VARIABLE) {
		    xsltStackElemPtr tmpvar = ctxt->vars;

		    xsltParseStylesheetVariable(ctxt, cur);

		    if (tmpvar != ctxt->vars) {
			ctxt->vars->level = level;
		    }
		} else if (info->type == XSLT_FUNC_MESSAGE) {
		    xsltMessage(ctxt, contextNode, cur);
		} else {
		    xsltTransformError(ctxt, NULL, cur,
			"Unexpected XSLT element '%s'.\n", cur->name);
		}
		goto skip_children;

	    } else {
		xsltTransformFunction func;
		if (cur->psvi == xsltExtMarker) {
		    func = (xsltTransformFunction)
			xsltExtElementLookup(ctxt, cur->name, cur->ns->href);
		} else
		    func = ((xsltElemPreCompPtr) cur->psvi)->func;

		if (func == NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt, XSLT_TRACE_APPLY_TEMPLATE,
			xsltGenericDebug(xsltGenericDebugContext,
			    "xsltApplySequenceConstructor: unknown extension %s\n",
			    cur->name));
#endif
		    ctxt->insert = insert;
		    if (!xsltApplyFallbacks(ctxt, contextNode, cur)) {
			xsltTransformError(ctxt, NULL, cur,
			    "Unknown extension instruction '{%s}%s'.\n",
			    cur->ns->href, cur->name);
		    }
		    ctxt->insert = oldInsert;
		} else {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
			"xsltApplySequenceConstructor: extension construct %s\n",
			cur->name));
#endif
		    ctxt->insert = insert;
		    oldLocalFragmentBase = ctxt->localRVTBase;
		    ctxt->localRVTBase = NULL;

		    func(ctxt, contextNode, cur, cur->psvi);

		    ctxt->localRVTBase = oldLocalFragmentBase;
		    if (oldLocalFragmentTop != ctxt->localRVT)
			xsltReleaseLocalRVTs(ctxt, oldLocalFragmentTop);

		    ctxt->insert = oldInsert;
		}
		goto skip_children;
	    }

	} else if (XSLT_IS_TEXT_NODE(cur)) {
#ifdef WITH_XSLT_DEBUG_PROCESS
            if (cur->name == xmlStringTextNoenc) {
                XSLT_TRACE(ctxt, XSLT_TRACE_APPLY_TEMPLATE,
		    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltApplySequenceConstructor: copy unescaped text '%s'\n",
		    cur->content));
            } else {
                XSLT_TRACE(ctxt, XSLT_TRACE_APPLY_TEMPLATE,
		    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltApplySequenceConstructor: copy text '%s'\n",
		    cur->content));
            }
#endif
            if (xsltCopyText(ctxt, insert, cur, ctxt->internalized) == NULL)
		goto error;
	}

#else 

        if (IS_XSLT_ELEM(cur)) {
            xsltStylePreCompPtr info = (xsltStylePreCompPtr) cur->psvi;

            if (info == NULL) {
                if (IS_XSLT_NAME(cur, "message")) {
                    xsltMessage(ctxt, contextNode, cur);
                } else {
                    ctxt->insert = insert;
                    if (!xsltApplyFallbacks(ctxt, contextNode, cur)) {
                        xsltGenericError(xsltGenericErrorContext,
			    "xsltApplySequenceConstructor: %s was not compiled\n",
			    cur->name);
                    }
                    ctxt->insert = oldInsert;
                }
                goto skip_children;
            }

            if (info->func != NULL) {
		oldCurInst = ctxt->inst;
		ctxt->inst = cur;
                ctxt->insert = insert;
		oldLocalFragmentBase = ctxt->localRVTBase;
		ctxt->localRVTBase = NULL;

                info->func(ctxt, contextNode, cur, (xsltElemPreCompPtr) info);

		ctxt->localRVTBase = oldLocalFragmentBase;
		if (oldLocalFragmentTop != ctxt->localRVT)
		    xsltReleaseLocalRVTs(ctxt, oldLocalFragmentTop);

                ctxt->insert = oldInsert;
		ctxt->inst = oldCurInst;
                goto skip_children;
            }

            if (IS_XSLT_NAME(cur, "variable")) {
		xsltStackElemPtr tmpvar = ctxt->vars;

		oldCurInst = ctxt->inst;
		ctxt->inst = cur;

		xsltParseStylesheetVariable(ctxt, cur);

		ctxt->inst = oldCurInst;

		if (tmpvar != ctxt->vars) {
		    ctxt->vars->level = level;
		}
            } else if (IS_XSLT_NAME(cur, "message")) {
                xsltMessage(ctxt, contextNode, cur);
            } else {
		xsltTransformError(ctxt, NULL, cur,
		    "Unexpected XSLT element '%s'.\n", cur->name);
            }
            goto skip_children;
        } else if ((cur->type == XML_TEXT_NODE) ||
                   (cur->type == XML_CDATA_SECTION_NODE)) {

#ifdef WITH_XSLT_DEBUG_PROCESS
            if (cur->type == XML_CDATA_SECTION_NODE) {
                XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
                                 "xsltApplySequenceConstructor: copy CDATA text %s\n",
                                 cur->content));
            } else if (cur->name == xmlStringTextNoenc) {
                XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
                                 "xsltApplySequenceConstructor: copy unescaped text %s\n",
                                 cur->content));
            } else {
                XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
                                 "xsltApplySequenceConstructor: copy text %s\n",
                                 cur->content));
            }
#endif
            if (xsltCopyText(ctxt, insert, cur, ctxt->internalized) == NULL)
		goto error;
        } else if ((cur->type == XML_ELEMENT_NODE) &&
                   (cur->ns != NULL) && (cur->psvi != NULL)) {
            xsltTransformFunction function;

	    oldCurInst = ctxt->inst;
	    ctxt->inst = cur;
            if (cur->psvi == xsltExtMarker)
                function = (xsltTransformFunction)
                    xsltExtElementLookup(ctxt, cur->name, cur->ns->href);
            else
                function = ((xsltElemPreCompPtr) cur->psvi)->func;

            if (function == NULL) {
                xmlNodePtr child;
                int found = 0;

#ifdef WITH_XSLT_DEBUG_PROCESS
                XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
		    "xsltApplySequenceConstructor: unknown extension %s\n",
                    cur->name));
#endif
                child = cur->children;
                while (child != NULL) {
                    if ((IS_XSLT_ELEM(child)) &&
                        (IS_XSLT_NAME(child, "fallback")))
		    {
                        found = 1;
                        xsltApplySequenceConstructor(ctxt, contextNode,
			    child->children, NULL);
                    }
                    child = child->next;
                }

                if (!found) {
                    xsltTransformError(ctxt, NULL, cur,
			"xsltApplySequenceConstructor: failed to find extension %s\n",
			cur->name);
                }
            } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
                XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
		    "xsltApplySequenceConstructor: extension construct %s\n",
                    cur->name));
#endif

                ctxt->insert = insert;
		oldLocalFragmentBase = ctxt->localRVTBase;
		ctxt->localRVTBase = NULL;

                function(ctxt, contextNode, cur, cur->psvi);
		if (oldLocalFragmentTop != ctxt->localRVT)
		    xsltReleaseLocalRVTs(ctxt, oldLocalFragmentTop);

		ctxt->localRVTBase = oldLocalFragmentBase;
                ctxt->insert = oldInsert;

            }
	    ctxt->inst = oldCurInst;
            goto skip_children;
        } else if (cur->type == XML_ELEMENT_NODE) {
#ifdef WITH_XSLT_DEBUG_PROCESS
            XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
		"xsltApplySequenceConstructor: copy node %s\n",
                cur->name));
#endif
	    oldCurInst = ctxt->inst;
	    ctxt->inst = cur;

            if ((copy = xsltShallowCopyElem(ctxt, cur, insert, 1)) == NULL)
		goto error;
            if ((templ != NULL) && (oldInsert == insert) &&
                (ctxt->templ != NULL) && (ctxt->templ->inheritedNs != NULL)) {
                int i;
                xmlNsPtr ns, ret;

                for (i = 0; i < ctxt->templ->inheritedNsNr; i++) {
		    const xmlChar *URI = NULL;
		    xsltStylesheetPtr style;
                    ns = ctxt->templ->inheritedNs[i];

#if 0
		    if (xmlStrEqual(ns->href, XSLT_NAMESPACE))
			continue;
#endif
		    style = ctxt->style;
		    while (style != NULL) {
			if (style->nsAliases != NULL)
			    URI = (const xmlChar *)
				xmlHashLookup(style->nsAliases, ns->href);
			if (URI != NULL)
			    break;

			style = xsltNextImport(style);
		    }
		    if (URI == UNDEFINED_DEFAULT_NS)
			continue;
		    if (URI == NULL)
			URI = ns->href;
		    ret = xmlSearchNs(copy->doc, copy, ns->prefix);
		    if ((ret == NULL) || (!xmlStrEqual(ret->href, URI)))
		    {
			xmlNewNs(copy, URI, ns->prefix);
		    }
                }
		if (copy->ns != NULL) {
		    copy->ns = xsltGetNamespace(ctxt, cur, copy->ns, copy);
		}
            }
            if (cur->properties != NULL) {
                xsltAttrListTemplateProcess(ctxt, copy, cur->properties);
            }
	    ctxt->inst = oldCurInst;
        }
#endif 

        if (cur->children != NULL) {
            if (cur->children->type != XML_ENTITY_DECL) {
                cur = cur->children;
		level++;
                if (copy != NULL)
                    insert = copy;
                continue;
            }
        }

skip_children:
	if (ctxt->state == XSLT_STATE_STOPPED)
	    break;
        if (cur->next != NULL) {
            cur = cur->next;
            continue;
        }

        do {
            cur = cur->parent;
	    level--;
	    if ((ctxt->varsNr > oldVarsNr) && (ctxt->vars->level > level)) {
		xsltLocalVariablePop(ctxt, oldVarsNr, level);
	    }

            insert = insert->parent;
            if (cur == NULL)
                break;
            if (cur == list->parent) {
                cur = NULL;
                break;
            }
            if (cur->next != NULL) {
                cur = cur->next;
                break;
            }
        } while (cur != NULL);
    }

error:
    if (ctxt->varsNr > oldVarsNr)
	xsltLocalVariablePop(ctxt, oldVarsNr, -1);

    ctxt->node = oldContextNode;
    ctxt->inst = oldInst;
    ctxt->insert = oldInsert;

#ifdef WITH_DEBUGGER
    if ((ctxt->debugStatus != XSLT_DEBUG_NONE) && (addCallResult)) {
        xslDropCall();
    }
#endif
}

static void
xsltApplyXSLTTemplate(xsltTransformContextPtr ctxt,
		      xmlNodePtr contextNode,
		      xmlNodePtr list,
		      xsltTemplatePtr templ,
		      xsltStackElemPtr withParams)
{
    int oldVarsBase = 0;
    long start = 0;
    xmlNodePtr cur;
    xsltStackElemPtr tmpParam = NULL;
    xmlDocPtr oldUserFragmentTop, oldLocalFragmentTop;

#ifdef XSLT_REFACTORED
    xsltStyleItemParamPtr iparam;
#else
    xsltStylePreCompPtr iparam;
#endif

#ifdef WITH_DEBUGGER
    int addCallResult = 0;
#endif

    if (ctxt == NULL)
	return;
    if (templ == NULL) {
	xsltTransformError(ctxt, NULL, list,
	    "xsltApplyXSLTTemplate: Bad arguments; @templ is mandatory.\n");
	return;
    }

#ifdef WITH_DEBUGGER
    if (ctxt->debugStatus != XSLT_DEBUG_NONE) {
	if (xsltDebuggerStartSequenceConstructor(ctxt, contextNode,
		list, templ, &addCallResult) == NULL)
	    return;
    }
#endif

    if (list == NULL)
        return;
    CHECK_STOPPED;

    if (((ctxt->templNr >= xsltMaxDepth) ||
        (ctxt->varsNr >= 5 * xsltMaxDepth)))
    {
        xsltTransformError(ctxt, NULL, list,
	    "xsltApplyXSLTTemplate: A potential infinite template recursion "
	    "was detected.\n"
	    "You can adjust xsltMaxDepth (--maxdepth) in order to "
	    "raise the maximum number of nested template calls and "
	    "variables/params (currently set to %d).\n",
	    xsltMaxDepth);
        xsltDebug(ctxt, contextNode, list, NULL);
        return;
    }

    oldUserFragmentTop = ctxt->tmpRVT;
    ctxt->tmpRVT = NULL;
    oldLocalFragmentTop = ctxt->localRVT;

    oldVarsBase = ctxt->varsBase;
    ctxt->varsBase = ctxt->varsNr;

    ctxt->node = contextNode;
    if (ctxt->profile) {
	templ->nbCalls++;
	start = xsltTimestamp();
	profPush(ctxt, 0);
    }
    templPush(ctxt, templ);

#ifdef WITH_XSLT_DEBUG_PROCESS
    if (templ->name != NULL)
	XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
	"applying xsl:template '%s'\n", templ->name));
#endif
    cur = list;
    do {
	if (cur->type == XML_TEXT_NODE) {
	    cur = cur->next;
	    continue;
	}
	if ((cur->type != XML_ELEMENT_NODE) ||
	    (cur->name[0] != 'p') ||
	    (cur->psvi == NULL) ||
	    (! xmlStrEqual(cur->name, BAD_CAST "param")) ||
	    (! IS_XSLT_ELEM(cur)))
	{
	    break;
	}

	list = cur->next;

#ifdef XSLT_REFACTORED
	iparam = (xsltStyleItemParamPtr) cur->psvi;
#else
	iparam = (xsltStylePreCompPtr) cur->psvi;
#endif

	tmpParam = NULL;
	if (withParams) {
	    tmpParam = withParams;
	    do {
		if ((tmpParam->name == (iparam->name)) &&
		    (tmpParam->nameURI == (iparam->ns)))
		{
		    xsltLocalVariablePush(ctxt, tmpParam, -1);
		    break;
		}
		tmpParam = tmpParam->next;
	    } while (tmpParam != NULL);
	}
	if (tmpParam == NULL) {
	    xsltParseStylesheetParam(ctxt, cur);
	}
	cur = cur->next;
    } while (cur != NULL);
    xsltApplySequenceConstructor(ctxt, contextNode, list, templ);

    if (ctxt->varsNr > ctxt->varsBase)
	xsltTemplateParamsCleanup(ctxt);
    ctxt->varsBase = oldVarsBase;

    if (oldLocalFragmentTop != ctxt->localRVT) {
	xmlDocPtr curdoc = ctxt->localRVT, tmp;

	do {
	    tmp = curdoc;
	    curdoc = (xmlDocPtr) curdoc->next;
	    
	    if (tmp == ctxt->localRVTBase)
	        ctxt->localRVTBase = curdoc;
	    if (tmp->prev)
		tmp->prev->next = (xmlNodePtr) curdoc;
	    if (curdoc)
		curdoc->prev = tmp->prev;
	    xsltReleaseRVT(ctxt, tmp);
	} while (curdoc != oldLocalFragmentTop);
    }
    ctxt->localRVT = oldLocalFragmentTop;

    if (ctxt->tmpRVT) {
	xmlDocPtr curdoc = ctxt->tmpRVT, tmp;

	while (curdoc != NULL) {
	    tmp = curdoc;
	    curdoc = (xmlDocPtr) curdoc->next;
	    xsltReleaseRVT(ctxt, tmp);
	}
    }
    ctxt->tmpRVT = oldUserFragmentTop;

    templPop(ctxt);
    if (ctxt->profile) {
	long spent, child, total, end;

	end = xsltTimestamp();
	child = profPop(ctxt);
	total = end - start;
	spent = total - child;
	if (spent <= 0) {
	    xsltCalibrateAdjust(spent);
	    spent = 0;
	}

	templ->time += spent;
	if (ctxt->profNr > 0)
	    ctxt->profTab[ctxt->profNr - 1] += total;
    }

#ifdef WITH_DEBUGGER
    if ((ctxt->debugStatus != XSLT_DEBUG_NONE) && (addCallResult)) {
        xslDropCall();
    }
#endif
}


void
xsltApplyOneTemplate(xsltTransformContextPtr ctxt,
		     xmlNodePtr contextNode,
                     xmlNodePtr list,
		     xsltTemplatePtr templ ATTRIBUTE_UNUSED,
                     xsltStackElemPtr params)
{
    if ((ctxt == NULL) || (list == NULL))
	return;
    CHECK_STOPPED;

    if (params) {
	int oldVarsNr = ctxt->varsNr;

	while (params != NULL) {
	    xsltLocalVariablePush(ctxt, params, -1);
	    params = params->next;
	}
	xsltApplySequenceConstructor(ctxt, contextNode, list, templ);
	xsltLocalVariablePop(ctxt, oldVarsNr, -2);
    } else
	xsltApplySequenceConstructor(ctxt, contextNode, list, templ);
}


void
xsltDocumentElem(xsltTransformContextPtr ctxt, xmlNodePtr node,
                 xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemDocumentPtr comp = (xsltStyleItemDocumentPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xsltStylesheetPtr style = NULL;
    int ret;
    xmlChar *filename = NULL, *prop, *elements;
    xmlChar *element, *end;
    xmlDocPtr res = NULL;
    xmlDocPtr oldOutput;
    xmlNodePtr oldInsert, root;
    const char *oldOutputFile;
    xsltOutputType oldType;
    xmlChar *URL = NULL;
    const xmlChar *method;
    const xmlChar *doctypePublic;
    const xmlChar *doctypeSystem;
    const xmlChar *version;
    const xmlChar *encoding;

    if ((ctxt == NULL) || (node == NULL) || (inst == NULL) || (comp == NULL))
        return;

    if (comp->filename == NULL) {

        if (xmlStrEqual(inst->name, (const xmlChar *) "output")) {
#ifdef WITH_XSLT_DEBUG_EXTRA
            xsltGenericDebug(xsltGenericDebugContext,
                             "Found saxon:output extension\n");
#endif
            URL = xsltEvalAttrValueTemplate(ctxt, inst,
                                                 (const xmlChar *) "file",
                                                 XSLT_SAXON_NAMESPACE);

	    if (URL == NULL)
		URL = xsltEvalAttrValueTemplate(ctxt, inst,
                                                 (const xmlChar *) "href",
                                                 XSLT_SAXON_NAMESPACE);
        } else if (xmlStrEqual(inst->name, (const xmlChar *) "write")) {
#ifdef WITH_XSLT_DEBUG_EXTRA
            xsltGenericDebug(xsltGenericDebugContext,
                             "Found xalan:write extension\n");
#endif
            URL = xsltEvalAttrValueTemplate(ctxt, inst,
                                                 (const xmlChar *)
                                                 "select",
                                                 XSLT_XALAN_NAMESPACE);
	    if (URL != NULL) {
		xmlXPathCompExprPtr cmp;
		xmlChar *val;

		cmp = xmlXPathCompile(URL);
                val = xsltEvalXPathString(ctxt, cmp);
		xmlXPathFreeCompExpr(cmp);
		xmlFree(URL);
		URL = val;
	    }
	    if (URL == NULL)
		URL = xsltEvalAttrValueTemplate(ctxt, inst,
						     (const xmlChar *)
						     "file",
						     XSLT_XALAN_NAMESPACE);
	    if (URL == NULL)
		URL = xsltEvalAttrValueTemplate(ctxt, inst,
						     (const xmlChar *)
						     "href",
						     XSLT_XALAN_NAMESPACE);
        } else if (xmlStrEqual(inst->name, (const xmlChar *) "document")) {
            URL = xsltEvalAttrValueTemplate(ctxt, inst,
                                                 (const xmlChar *) "href",
                                                 NULL);
        }

    } else {
        URL = xmlStrdup(comp->filename);
    }

    if (URL == NULL) {
	xsltTransformError(ctxt, NULL, inst,
		         "xsltDocumentElem: href/URI-Reference not found\n");
	return;
    }

    filename = xmlBuildURI(URL, (const xmlChar *) ctxt->outputFile);
    if (filename == NULL) {
	xmlChar *escURL;

	escURL=xmlURIEscapeStr(URL, BAD_CAST ":/.?,");
	if (escURL != NULL) {
	    filename = xmlBuildURI(escURL, (const xmlChar *) ctxt->outputFile);
	    xmlFree(escURL);
	}
    }

    if (filename == NULL) {
	xsltTransformError(ctxt, NULL, inst,
		         "xsltDocumentElem: URL computation failed for %s\n",
			 URL);
	xmlFree(URL);
	return;
    }

    if (ctxt->sec != NULL) {
	ret = xsltCheckWrite(ctxt->sec, ctxt, filename);
	if (ret == 0) {
	    xsltTransformError(ctxt, NULL, inst,
		 "xsltDocumentElem: write rights for %s denied\n",
			     filename);
	    xmlFree(URL);
	    xmlFree(filename);
	    return;
	}
    }

    oldOutputFile = ctxt->outputFile;
    oldOutput = ctxt->output;
    oldInsert = ctxt->insert;
    oldType = ctxt->type;
    ctxt->outputFile = (const char *) filename;

    style = xsltNewStylesheet();
    if (style == NULL) {
	xsltTransformError(ctxt, NULL, inst,
                         "xsltDocumentElem: out of memory\n");
        goto error;
    }

    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *) "version",
				     NULL);
    if (prop != NULL) {
	if (style->version != NULL)
	    xmlFree(style->version);
	style->version = prop;
    }
    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *) "encoding",
				     NULL);
    if (prop != NULL) {
	if (style->encoding != NULL)
	    xmlFree(style->encoding);
	style->encoding = prop;
    }
    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *) "method",
				     NULL);
    if (prop != NULL) {
	const xmlChar *URI;

	if (style->method != NULL)
	    xmlFree(style->method);
	style->method = NULL;
	if (style->methodURI != NULL)
	    xmlFree(style->methodURI);
	style->methodURI = NULL;

	URI = xsltGetQNameURI(inst, &prop);
	if (prop == NULL) {
	    if (style != NULL) style->errors++;
	} else if (URI == NULL) {
	    if ((xmlStrEqual(prop, (const xmlChar *) "xml")) ||
		(xmlStrEqual(prop, (const xmlChar *) "html")) ||
		(xmlStrEqual(prop, (const xmlChar *) "text"))) {
		style->method = prop;
	    } else {
		xsltTransformError(ctxt, NULL, inst,
				 "invalid value for method: %s\n", prop);
		if (style != NULL) style->warnings++;
	    }
	} else {
	    style->method = prop;
	    style->methodURI = xmlStrdup(URI);
	}
    }
    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *)
				     "doctype-system", NULL);
    if (prop != NULL) {
	if (style->doctypeSystem != NULL)
	    xmlFree(style->doctypeSystem);
	style->doctypeSystem = prop;
    }
    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *)
				     "doctype-public", NULL);
    if (prop != NULL) {
	if (style->doctypePublic != NULL)
	    xmlFree(style->doctypePublic);
	style->doctypePublic = prop;
    }
    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *) "standalone",
				     NULL);
    if (prop != NULL) {
	if (xmlStrEqual(prop, (const xmlChar *) "yes")) {
	    style->standalone = 1;
	} else if (xmlStrEqual(prop, (const xmlChar *) "no")) {
	    style->standalone = 0;
	} else {
	    xsltTransformError(ctxt, NULL, inst,
			     "invalid value for standalone: %s\n",
			     prop);
	    if (style != NULL) style->warnings++;
	}
	xmlFree(prop);
    }

    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *) "indent",
				     NULL);
    if (prop != NULL) {
	if (xmlStrEqual(prop, (const xmlChar *) "yes")) {
	    style->indent = 1;
	} else if (xmlStrEqual(prop, (const xmlChar *) "no")) {
	    style->indent = 0;
	} else {
	    xsltTransformError(ctxt, NULL, inst,
			     "invalid value for indent: %s\n", prop);
	    if (style != NULL) style->warnings++;
	}
	xmlFree(prop);
    }

    prop = xsltEvalAttrValueTemplate(ctxt, inst,
				     (const xmlChar *)
				     "omit-xml-declaration",
				     NULL);
    if (prop != NULL) {
	if (xmlStrEqual(prop, (const xmlChar *) "yes")) {
	    style->omitXmlDeclaration = 1;
	} else if (xmlStrEqual(prop, (const xmlChar *) "no")) {
	    style->omitXmlDeclaration = 0;
	} else {
	    xsltTransformError(ctxt, NULL, inst,
			     "invalid value for omit-xml-declaration: %s\n",
			     prop);
	    if (style != NULL) style->warnings++;
	}
	xmlFree(prop);
    }

    elements = xsltEvalAttrValueTemplate(ctxt, inst,
					 (const xmlChar *)
					 "cdata-section-elements",
					 NULL);
    if (elements != NULL) {
	if (style->stripSpaces == NULL)
	    style->stripSpaces = xmlHashCreate(10);
	if (style->stripSpaces == NULL)
	    return;

	element = elements;
	while (*element != 0) {
	    while (IS_BLANK_CH(*element))
		element++;
	    if (*element == 0)
		break;
	    end = element;
	    while ((*end != 0) && (!IS_BLANK_CH(*end)))
		end++;
	    element = xmlStrndup(element, end - element);
	    if (element) {
		const xmlChar *URI;

#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
				 "add cdata section output element %s\n",
				 element);
#endif
                URI = xsltGetQNameURI(inst, &element);

		xmlHashAddEntry2(style->stripSpaces, element, URI,
			        (xmlChar *) "cdata");
		xmlFree(element);
	    }
	    element = end;
	}
	xmlFree(elements);
    }

    XSLT_GET_IMPORT_PTR(method, style, method)
    XSLT_GET_IMPORT_PTR(doctypePublic, style, doctypePublic)
    XSLT_GET_IMPORT_PTR(doctypeSystem, style, doctypeSystem)
    XSLT_GET_IMPORT_PTR(version, style, version)
    XSLT_GET_IMPORT_PTR(encoding, style, encoding)

    if ((method != NULL) &&
	(!xmlStrEqual(method, (const xmlChar *) "xml"))) {
	if (xmlStrEqual(method, (const xmlChar *) "html")) {
	    ctxt->type = XSLT_OUTPUT_HTML;
	    if (((doctypePublic != NULL) || (doctypeSystem != NULL)))
		res = htmlNewDoc(doctypeSystem, doctypePublic);
	    else {
		if (version != NULL) {
#ifdef XSLT_GENERATE_HTML_DOCTYPE
		    xsltGetHTMLIDs(version, &doctypePublic, &doctypeSystem);
#endif
                }
		res = htmlNewDocNoDtD(doctypeSystem, doctypePublic);
	    }
	    if (res == NULL)
		goto error;
	    res->dict = ctxt->dict;
	    xmlDictReference(res->dict);
	} else if (xmlStrEqual(method, (const xmlChar *) "xhtml")) {
	    xsltTransformError(ctxt, NULL, inst,
	     "xsltDocumentElem: unsupported method xhtml\n",
		             style->method);
	    ctxt->type = XSLT_OUTPUT_HTML;
	    res = htmlNewDocNoDtD(doctypeSystem, doctypePublic);
	    if (res == NULL)
		goto error;
	    res->dict = ctxt->dict;
	    xmlDictReference(res->dict);
	} else if (xmlStrEqual(method, (const xmlChar *) "text")) {
	    ctxt->type = XSLT_OUTPUT_TEXT;
	    res = xmlNewDoc(style->version);
	    if (res == NULL)
		goto error;
	    res->dict = ctxt->dict;
	    xmlDictReference(res->dict);
#ifdef WITH_XSLT_DEBUG
	    xsltGenericDebug(xsltGenericDebugContext,
                     "reusing transformation dict for output\n");
#endif
	} else {
	    xsltTransformError(ctxt, NULL, inst,
			     "xsltDocumentElem: unsupported method %s\n",
		             style->method);
	    goto error;
	}
    } else {
	ctxt->type = XSLT_OUTPUT_XML;
	res = xmlNewDoc(style->version);
	if (res == NULL)
	    goto error;
	res->dict = ctxt->dict;
	xmlDictReference(res->dict);
#ifdef WITH_XSLT_DEBUG
	xsltGenericDebug(xsltGenericDebugContext,
                     "reusing transformation dict for output\n");
#endif
    }
    res->charset = XML_CHAR_ENCODING_UTF8;
    if (encoding != NULL)
	res->encoding = xmlStrdup(encoding);
    ctxt->output = res;
    ctxt->insert = (xmlNodePtr) res;
    xsltApplySequenceConstructor(ctxt, node, inst->children, NULL);

    root = xmlDocGetRootElement(res);
    if (root != NULL) {
        const xmlChar *doctype = NULL;

        if ((root->ns != NULL) && (root->ns->prefix != NULL))
	    doctype = xmlDictQLookup(ctxt->dict, root->ns->prefix, root->name);
	if (doctype == NULL)
	    doctype = root->name;

        if ((method == NULL) &&
            (root->ns == NULL) &&
            (!xmlStrcasecmp(root->name, (const xmlChar *) "html"))) {
            xmlNodePtr tmp;

            tmp = res->children;
            while ((tmp != NULL) && (tmp != root)) {
                if (tmp->type == XML_ELEMENT_NODE)
                    break;
                if ((tmp->type == XML_TEXT_NODE) && (!xmlIsBlankNode(tmp)))
                    break;
		tmp = tmp->next;
            }
            if (tmp == root) {
                ctxt->type = XSLT_OUTPUT_HTML;
                res->type = XML_HTML_DOCUMENT_NODE;
                if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
                    res->intSubset = xmlCreateIntSubset(res, doctype,
                                                        doctypePublic,
                                                        doctypeSystem);
#ifdef XSLT_GENERATE_HTML_DOCTYPE
		} else if (version != NULL) {
                    xsltGetHTMLIDs(version, &doctypePublic,
                                   &doctypeSystem);
                    if (((doctypePublic != NULL) || (doctypeSystem != NULL)))
                        res->intSubset =
                            xmlCreateIntSubset(res, doctype,
                                               doctypePublic,
                                               doctypeSystem);
#endif
                }
            }

        }
        if (ctxt->type == XSLT_OUTPUT_XML) {
            XSLT_GET_IMPORT_PTR(doctypePublic, style, doctypePublic)
                XSLT_GET_IMPORT_PTR(doctypeSystem, style, doctypeSystem)
                if (((doctypePublic != NULL) || (doctypeSystem != NULL)))
                res->intSubset = xmlCreateIntSubset(res, doctype,
                                                    doctypePublic,
                                                    doctypeSystem);
        }
    }

    ret = xsltSaveResultToFilename((const char *) filename,
                                   res, style, 0);
    if (ret < 0) {
	xsltTransformError(ctxt, NULL, inst,
                         "xsltDocumentElem: unable to save to %s\n",
                         filename);
	ctxt->state = XSLT_STATE_ERROR;
#ifdef WITH_XSLT_DEBUG_EXTRA
    } else {
        xsltGenericDebug(xsltGenericDebugContext,
                         "Wrote %d bytes to %s\n", ret, filename);
#endif
    }

  error:
    ctxt->output = oldOutput;
    ctxt->insert = oldInsert;
    ctxt->type = oldType;
    ctxt->outputFile = oldOutputFile;
    if (URL != NULL)
        xmlFree(URL);
    if (filename != NULL)
        xmlFree(filename);
    if (style != NULL)
        xsltFreeStylesheet(style);
    if (res != NULL)
        xmlFreeDoc(res);
}


void
xsltSort(xsltTransformContextPtr ctxt,
	xmlNodePtr node ATTRIBUTE_UNUSED, xmlNodePtr inst,
	xsltStylePreCompPtr comp) {
    if (comp == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	     "xsl:sort : compilation failed\n");
	return;
    }
    xsltTransformError(ctxt, NULL, inst,
	 "xsl:sort : improper use this should not be reached\n");
}

void
xsltCopy(xsltTransformContextPtr ctxt, xmlNodePtr node,
	 xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemCopyPtr comp = (xsltStyleItemCopyPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlNodePtr copy, oldInsert;

    oldInsert = ctxt->insert;
    if (ctxt->insert != NULL) {
	switch (node->type) {
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
#ifdef WITH_XSLT_DEBUG_PROCESS
		if (node->type == XML_CDATA_SECTION_NODE) {
		    XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
			 "xsltCopy: CDATA text %s\n", node->content));
		} else {
		    XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
			 "xsltCopy: text %s\n", node->content));
                }
#endif
		xsltCopyText(ctxt, ctxt->insert, node, 0);
		break;
	    case XML_DOCUMENT_NODE:
	    case XML_HTML_DOCUMENT_NODE:
		break;
	    case XML_ELEMENT_NODE:

#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
				 "xsltCopy: node %s\n", node->name));
#endif
		copy = xsltShallowCopyElem(ctxt, node, ctxt->insert, 0);
		ctxt->insert = copy;
		if (comp->use != NULL) {
		    xsltApplyAttributeSet(ctxt, node, inst, comp->use);
		}
		break;
	    case XML_ATTRIBUTE_NODE: {
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
				 "xsltCopy: attribute %s\n", node->name));
#endif
		xsltShallowCopyAttr(ctxt, inst, ctxt->insert, (xmlAttrPtr) node);
		break;
	    }
	    case XML_PI_NODE:
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
				 "xsltCopy: PI %s\n", node->name));
#endif
		copy = xmlNewDocPI(ctxt->insert->doc, node->name,
		                   node->content);
		copy = xsltAddChild(ctxt->insert, copy);
		break;
	    case XML_COMMENT_NODE:
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
				 "xsltCopy: comment\n"));
#endif
		copy = xmlNewComment(node->content);
		copy = xsltAddChild(ctxt->insert, copy);
		break;
	    case XML_NAMESPACE_DECL:
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_COPY,xsltGenericDebug(xsltGenericDebugContext,
				 "xsltCopy: namespace declaration\n"));
#endif
		xsltShallowCopyNsNode(ctxt, inst, ctxt->insert, (xmlNsPtr)node);
		break;
	    default:
		break;

	}
    }

    switch (node->type) {
	case XML_DOCUMENT_NODE:
	case XML_HTML_DOCUMENT_NODE:
	case XML_ELEMENT_NODE:
	    xsltApplySequenceConstructor(ctxt, ctxt->node, inst->children,
		NULL);
	    break;
	default:
	    break;
    }
    ctxt->insert = oldInsert;
}

void
xsltText(xsltTransformContextPtr ctxt, xmlNodePtr node ATTRIBUTE_UNUSED,
	    xmlNodePtr inst, xsltStylePreCompPtr comp ATTRIBUTE_UNUSED) {
    if ((inst->children != NULL) && (comp != NULL)) {
	xmlNodePtr text = inst->children;
	xmlNodePtr copy;

	while (text != NULL) {
	    if ((text->type != XML_TEXT_NODE) &&
	         (text->type != XML_CDATA_SECTION_NODE)) {
		xsltTransformError(ctxt, NULL, inst,
				 "xsl:text content problem\n");
		break;
	    }
	    copy = xmlNewDocText(ctxt->output, text->content);
	    if (text->type != XML_CDATA_SECTION_NODE) {
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
		     "Disable escaping: %s\n", text->content);
#endif
		copy->name = xmlStringTextNoenc;
	    }
	    copy = xsltAddChild(ctxt->insert, copy);
	    text = text->next;
	}
    }
}

void
xsltElement(xsltTransformContextPtr ctxt, xmlNodePtr node,
	    xmlNodePtr inst, xsltStylePreCompPtr castedComp) {
#ifdef XSLT_REFACTORED
    xsltStyleItemElementPtr comp = (xsltStyleItemElementPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlChar *prop = NULL;
    const xmlChar *name, *prefix = NULL, *nsName = NULL;
    xmlNodePtr copy;
    xmlNodePtr oldInsert;

    if (ctxt->insert == NULL)
	return;

    if (!comp->has_name)
        return;

    oldInsert = ctxt->insert;

    if (comp->name == NULL) {
	
        prop = xsltEvalAttrValueTemplate(ctxt, inst,
	    (const xmlChar *) "name", XSLT_NAMESPACE);
        if (prop == NULL) {
            xsltTransformError(ctxt, NULL, inst,
		"xsl:element: The attribute 'name' is missing.\n");
            goto error;
        }
	if (xmlValidateQName(prop, 0)) {
	    xsltTransformError(ctxt, NULL, inst,
		"xsl:element: The effective name '%s' is not a "
		"valid QName.\n", prop);
	    
	}
	name = xsltSplitQName(ctxt->dict, prop, &prefix);
	xmlFree(prop);
	if ((prefix != NULL) &&
	    (!xmlStrncasecmp(prefix, (xmlChar *)"xml", 3)))
	{
	    goto error;
	}
    } else {
#ifdef XSLT_REFACTORED
	prefix = comp->nsPrefix;
	name = comp->name;
#else
	name = xsltSplitQName(ctxt->dict, comp->name, &prefix);
#endif
    }

    if (ctxt->output->dict == ctxt->dict) {
	copy = xmlNewDocNodeEatName(ctxt->output, NULL, (xmlChar *)name, NULL);
    } else {
	copy = xmlNewDocNode(ctxt->output, NULL, (xmlChar *)name, NULL);
    }
    if (copy == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "xsl:element : creation of %s failed\n", name);
	return;
    }
    copy = xsltAddChild(ctxt->insert, copy);

    if (comp->has_ns) {
	if (comp->ns != NULL) {
	    if (comp->ns[0] != 0)
		nsName = comp->ns;
	} else {
	    xmlChar *tmpNsName;
	    
	    tmpNsName = xsltEvalAttrValueTemplate(ctxt, inst,
		(const xmlChar *) "namespace", XSLT_NAMESPACE);
	    if ((tmpNsName != NULL) && (tmpNsName[0] != 0))
		nsName = xmlDictLookup(ctxt->dict, BAD_CAST tmpNsName, -1);
	    xmlFree(tmpNsName);
	};
    } else {
	xmlNsPtr ns;
	ns = xmlSearchNs(inst->doc, inst, prefix);
	if (ns == NULL) {
	    if (prefix != NULL) {
		xsltTransformError(ctxt, NULL, inst,
		    "xsl:element: The QName '%s:%s' has no "
		    "namespace binding in scope in the stylesheet; "
		    "this is an error, since the namespace was not "
		    "specified by the instruction itself.\n", prefix, name);
	    }
	} else
	    nsName = ns->href;
    }
    if (nsName != NULL) {
	copy->ns = xsltGetSpecialNamespace(ctxt, inst, nsName, prefix, copy);
    } else if ((copy->parent != NULL) &&
	(copy->parent->type == XML_ELEMENT_NODE) &&
	(copy->parent->ns != NULL))
    {
	xsltGetSpecialNamespace(ctxt, inst, NULL, NULL, copy);
    }

    ctxt->insert = copy;

    if (comp->has_use) {
	if (comp->use != NULL) {
	    xsltApplyAttributeSet(ctxt, node, inst, comp->use);
	} else {
	    xmlChar *attrSets = NULL;
	    attrSets = xsltEvalAttrValueTemplate(ctxt, inst,
		(const xmlChar *)"use-attribute-sets", NULL);
	    if (attrSets != NULL) {
		xsltApplyAttributeSet(ctxt, node, inst, attrSets);
		xmlFree(attrSets);
	    }
	}
    }
    if (inst->children != NULL)
	xsltApplySequenceConstructor(ctxt, ctxt->node, inst->children,
	    NULL);

error:
    ctxt->insert = oldInsert;
    return;
}


void
xsltComment(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst, xsltStylePreCompPtr comp ATTRIBUTE_UNUSED) {
    xmlChar *value = NULL;
    xmlNodePtr commentNode;
    int len;

    value = xsltEvalTemplateString(ctxt, node, inst);
    
    len = xmlStrlen(value);
    if (len > 0) {
        if ((value[len-1] == '-') ||
	    (xmlStrstr(value, BAD_CAST "--"))) {
	    xsltTransformError(ctxt, NULL, inst,
		    "xsl:comment : '--' or ending '-' not allowed in comment\n");
	    
	}
    }
#ifdef WITH_XSLT_DEBUG_PROCESS
    if (value == NULL) {
	XSLT_TRACE(ctxt,XSLT_TRACE_COMMENT,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltComment: empty\n"));
    } else {
	XSLT_TRACE(ctxt,XSLT_TRACE_COMMENT,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltComment: content %s\n", value));
    }
#endif

    commentNode = xmlNewComment(value);
    commentNode = xsltAddChild(ctxt->insert, commentNode);

    if (value != NULL)
	xmlFree(value);
}

void
xsltProcessingInstruction(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst, xsltStylePreCompPtr castedComp) {
#ifdef XSLT_REFACTORED
    xsltStyleItemPIPtr comp = (xsltStyleItemPIPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    const xmlChar *name;
    xmlChar *value = NULL;
    xmlNodePtr pi;


    if (ctxt->insert == NULL)
	return;
    if (comp->has_name == 0)
	return;
    if (comp->name == NULL) {
	name = xsltEvalAttrValueTemplate(ctxt, inst,
			    (const xmlChar *)"name", NULL);
	if (name == NULL) {
	    xsltTransformError(ctxt, NULL, inst,
		 "xsl:processing-instruction : name is missing\n");
	    goto error;
	}
    } else {
	name = comp->name;
    }
    


    value = xsltEvalTemplateString(ctxt, node, inst);
    if (xmlStrstr(value, BAD_CAST "?>") != NULL) {
	xsltTransformError(ctxt, NULL, inst,
	     "xsl:processing-instruction: '?>' not allowed within PI content\n");
	goto error;
    }
#ifdef WITH_XSLT_DEBUG_PROCESS
    if (value == NULL) {
	XSLT_TRACE(ctxt,XSLT_TRACE_PI,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessingInstruction: %s empty\n", name));
    } else {
	XSLT_TRACE(ctxt,XSLT_TRACE_PI,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltProcessingInstruction: %s content %s\n", name, value));
    }
#endif

    pi = xmlNewDocPI(ctxt->insert->doc, name, value);
    pi = xsltAddChild(ctxt->insert, pi);

error:
    if ((name != NULL) && (name != comp->name))
        xmlFree((xmlChar *) name);
    if (value != NULL)
	xmlFree(value);
}

void
xsltCopyOf(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst, xsltStylePreCompPtr castedComp) {
#ifdef XSLT_REFACTORED
    xsltStyleItemCopyOfPtr comp = (xsltStyleItemCopyOfPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlXPathObjectPtr res = NULL;
    xmlNodeSetPtr list = NULL;
    int i;
    xmlDocPtr oldXPContextDoc;
    xmlNsPtr *oldXPNamespaces;
    xmlNodePtr oldXPContextNode;
    int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
    xmlXPathContextPtr xpctxt;

    if ((ctxt == NULL) || (node == NULL) || (inst == NULL))
	return;
    if ((comp == NULL) || (comp->select == NULL) || (comp->comp == NULL)) {
	xsltTransformError(ctxt, NULL, inst,
	     "xsl:copy-of : compilation failed\n");
	return;
    }


#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_COPY_OF,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltCopyOf: select %s\n", comp->select));
#endif

    xpctxt = ctxt->xpathCtxt;
    oldXPContextDoc = xpctxt->doc;
    oldXPContextNode = xpctxt->node;
    oldXPProximityPosition = xpctxt->proximityPosition;
    oldXPContextSize = xpctxt->contextSize;
    oldXPNsNr = xpctxt->nsNr;
    oldXPNamespaces = xpctxt->namespaces;

    xpctxt->node = node;
    if (comp != NULL) {

#ifdef XSLT_REFACTORED
	if (comp->inScopeNs != NULL) {
	    xpctxt->namespaces = comp->inScopeNs->list;
	    xpctxt->nsNr = comp->inScopeNs->xpathNumber;
	} else {
	    xpctxt->namespaces = NULL;
	    xpctxt->nsNr = 0;
	}
#else
	xpctxt->namespaces = comp->nsList;
	xpctxt->nsNr = comp->nsNr;
#endif
    } else {
	xpctxt->namespaces = NULL;
	xpctxt->nsNr = 0;
    }

    res = xmlXPathCompiledEval(comp->comp, xpctxt);

    xpctxt->doc = oldXPContextDoc;
    xpctxt->node = oldXPContextNode;
    xpctxt->contextSize = oldXPContextSize;
    xpctxt->proximityPosition = oldXPProximityPosition;
    xpctxt->nsNr = oldXPNsNr;
    xpctxt->namespaces = oldXPNamespaces;

    if (res != NULL) {
	if (res->type == XPATH_NODESET) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_COPY_OF,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltCopyOf: result is a node set\n"));
#endif
	    list = res->nodesetval;
	    if (list != NULL) {
		xmlNodePtr cur;
		for (i = 0;i < list->nodeNr;i++) {
		    cur = list->nodeTab[i];
		    if (cur == NULL)
			continue;
		    if ((cur->type == XML_DOCUMENT_NODE) ||
			(cur->type == XML_HTML_DOCUMENT_NODE))
		    {
			xsltCopyTreeList(ctxt, inst,
			    cur->children, ctxt->insert, 0, 0);
		    } else if (cur->type == XML_ATTRIBUTE_NODE) {
			xsltShallowCopyAttr(ctxt, inst,
			    ctxt->insert, (xmlAttrPtr) cur);
		    } else {
			xsltCopyTreeInternal(ctxt, inst,
			    cur, ctxt->insert, 0, 0);
		    }
		}
	    }
	} else if (res->type == XPATH_XSLT_TREE) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_COPY_OF,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltCopyOf: result is a result tree fragment\n"));
#endif
	    list = res->nodesetval;
	    if ((list != NULL) && (list->nodeTab != NULL) &&
		(list->nodeTab[0] != NULL) &&
		(IS_XSLT_REAL_NODE(list->nodeTab[0])))
	    {
		xsltCopyTreeList(ctxt, inst,
		    list->nodeTab[0]->children, ctxt->insert, 0, 0);
	    }
	} else {
	    xmlChar *value = NULL;
	    value = xmlXPathCastToString(res);
	    if (value == NULL) {
		xsltTransformError(ctxt, NULL, inst,
		    "Internal error in xsltCopyOf(): "
		    "failed to cast an XPath object to string.\n");
		ctxt->state = XSLT_STATE_STOPPED;
	    } else {
		if (value[0] != 0) {
		    xsltCopyTextString(ctxt, ctxt->insert, value, 0);
		}
		xmlFree(value);

#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_COPY_OF,xsltGenericDebug(xsltGenericDebugContext,
		    "xsltCopyOf: result %s\n", res->stringval));
#endif
	    }
	}
    } else {
	ctxt->state = XSLT_STATE_STOPPED;
    }

    if (res != NULL)
	xmlXPathFreeObject(res);
}

void
xsltValueOf(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemValueOfPtr comp = (xsltStyleItemValueOfPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlXPathObjectPtr res = NULL;
    xmlNodePtr copy = NULL;
    xmlChar *value = NULL;
    xmlDocPtr oldXPContextDoc;
    xmlNsPtr *oldXPNamespaces;
    xmlNodePtr oldXPContextNode;
    int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
    xmlXPathContextPtr xpctxt;

    if ((ctxt == NULL) || (node == NULL) || (inst == NULL))
	return;

    if ((comp == NULL) || (comp->select == NULL) || (comp->comp == NULL)) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltValueOf(): "
	    "The XSLT 'value-of' instruction was not compiled.\n");
	return;
    }

#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_VALUE_OF,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltValueOf: select %s\n", comp->select));
#endif

    xpctxt = ctxt->xpathCtxt;
    oldXPContextDoc = xpctxt->doc;
    oldXPContextNode = xpctxt->node;
    oldXPProximityPosition = xpctxt->proximityPosition;
    oldXPContextSize = xpctxt->contextSize;
    oldXPNsNr = xpctxt->nsNr;
    oldXPNamespaces = xpctxt->namespaces;

    xpctxt->node = node;
    if (comp != NULL) {

#ifdef XSLT_REFACTORED
	if (comp->inScopeNs != NULL) {
	    xpctxt->namespaces = comp->inScopeNs->list;
	    xpctxt->nsNr = comp->inScopeNs->xpathNumber;
	} else {
	    xpctxt->namespaces = NULL;
	    xpctxt->nsNr = 0;
	}
#else
	xpctxt->namespaces = comp->nsList;
	xpctxt->nsNr = comp->nsNr;
#endif
    } else {
	xpctxt->namespaces = NULL;
	xpctxt->nsNr = 0;
    }

    res = xmlXPathCompiledEval(comp->comp, xpctxt);

    xpctxt->doc = oldXPContextDoc;
    xpctxt->node = oldXPContextNode;
    xpctxt->contextSize = oldXPContextSize;
    xpctxt->proximityPosition = oldXPProximityPosition;
    xpctxt->nsNr = oldXPNsNr;
    xpctxt->namespaces = oldXPNamespaces;

    if (res != NULL) {
	value = xmlXPathCastToString(res);
	if (value == NULL) {
	    xsltTransformError(ctxt, NULL, inst,
		"Internal error in xsltValueOf(): "
		"failed to cast an XPath object to string.\n");
	    ctxt->state = XSLT_STATE_STOPPED;
	    goto error;
	}
	if (value[0] != 0) {
	    copy = xsltCopyTextString(ctxt,
		ctxt->insert, value, comp->noescape);
	}
    } else {
	xsltTransformError(ctxt, NULL, inst,
	    "XPath evaluation returned no result.\n");
	ctxt->state = XSLT_STATE_STOPPED;
	goto error;
    }

#ifdef WITH_XSLT_DEBUG_PROCESS
    if (value) {
	XSLT_TRACE(ctxt,XSLT_TRACE_VALUE_OF,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltValueOf: result '%s'\n", value));
    }
#endif

error:
    if (value != NULL)
	xmlFree(value);
    if (res != NULL)
	xmlXPathFreeObject(res);
}

void
xsltNumber(xsltTransformContextPtr ctxt, xmlNodePtr node,
	   xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemNumberPtr comp = (xsltStyleItemNumberPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    if (comp == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	     "xsl:number : compilation failed\n");
	return;
    }

    if ((ctxt == NULL) || (node == NULL) || (inst == NULL) || (comp == NULL))
	return;

    comp->numdata.doc = inst->doc;
    comp->numdata.node = inst;

    xsltNumberFormat(ctxt, &comp->numdata, node);
}

void
xsltApplyImports(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	         xmlNodePtr inst,
		 xsltStylePreCompPtr comp ATTRIBUTE_UNUSED)
{
    xsltTemplatePtr templ;

    if ((ctxt == NULL) || (inst == NULL))
	return;

    if (comp == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltApplyImports(): "
	    "The XSLT 'apply-imports' instruction was not compiled.\n");
	return;
    }
    if (ctxt->currentTemplateRule == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	     "It is an error to call 'apply-imports' "
	     "when there's no current template rule.\n");
	return;
    }
    templ = xsltGetTemplate(ctxt, contextNode,
	ctxt->currentTemplateRule->style);

    if (templ != NULL) {
	xsltTemplatePtr oldCurTemplRule = ctxt->currentTemplateRule;
	ctxt->currentTemplateRule = templ;
	xsltApplyXSLTTemplate(ctxt, contextNode, templ->content,
	    templ, NULL);

	ctxt->currentTemplateRule = oldCurTemplRule;
    }
}

void
xsltCallTemplate(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemCallTemplatePtr comp =
	(xsltStyleItemCallTemplatePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xsltStackElemPtr withParams = NULL;

    if (ctxt->insert == NULL)
	return;
    if (comp == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	     "The XSLT 'call-template' instruction was not compiled.\n");
	return;
    }

    if (comp->templ == NULL) {
	comp->templ = xsltFindTemplate(ctxt, comp->name, comp->ns);
	if (comp->templ == NULL) {
	    if (comp->ns != NULL) {
	        xsltTransformError(ctxt, NULL, inst,
			"The called template '{%s}%s' was not found.\n",
			comp->ns, comp->name);
	    } else {
	        xsltTransformError(ctxt, NULL, inst,
			"The called template '%s' was not found.\n",
			comp->name);
	    }
	    return;
	}
    }

#ifdef WITH_XSLT_DEBUG_PROCESS
    if ((comp != NULL) && (comp->name != NULL))
	XSLT_TRACE(ctxt,XSLT_TRACE_CALL_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
			 "call-template: name %s\n", comp->name));
#endif

    if (inst->children) {
	xmlNodePtr cur;
	xsltStackElemPtr param;

	cur = inst->children;
	while (cur != NULL) {
#ifdef WITH_DEBUGGER
	    if (ctxt->debugStatus != XSLT_DEBUG_NONE)
		xslHandleDebugger(cur, node, comp->templ, ctxt);
#endif
	    if (ctxt->state == XSLT_STATE_STOPPED) break;
	    if (IS_XSLT_ELEM(cur)) {
		if (IS_XSLT_NAME(cur, "with-param")) {
		    param = xsltParseStylesheetCallerParam(ctxt, cur);
		    if (param != NULL) {
			param->next = withParams;
			withParams = param;
		    }
		} else {
		    xsltGenericError(xsltGenericErrorContext,
			"xsl:call-template: misplaced xsl:%s\n", cur->name);
		}
	    } else {
		xsltGenericError(xsltGenericErrorContext,
		    "xsl:call-template: misplaced %s element\n", cur->name);
	    }
	    cur = cur->next;
	}
    }
    xsltApplyXSLTTemplate(ctxt, node, comp->templ->content, comp->templ,
	withParams);
    if (withParams != NULL)
	xsltFreeStackElemList(withParams);

#ifdef WITH_XSLT_DEBUG_PROCESS
    if ((comp != NULL) && (comp->name != NULL))
	XSLT_TRACE(ctxt,XSLT_TRACE_CALL_TEMPLATE,xsltGenericDebug(xsltGenericDebugContext,
			 "call-template returned: name %s\n", comp->name));
#endif
}

void
xsltApplyTemplates(xsltTransformContextPtr ctxt, xmlNodePtr node,
	           xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemApplyTemplatesPtr comp =
	(xsltStyleItemApplyTemplatesPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    int i;
    xmlNodePtr cur, delNode = NULL, oldContextNode;
    xmlNodeSetPtr list = NULL, oldList;
    xsltStackElemPtr withParams = NULL;
    int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
    const xmlChar *oldMode, *oldModeURI;
    xmlDocPtr oldXPDoc;
    xsltDocumentPtr oldDocInfo;
    xmlXPathContextPtr xpctxt;
    xmlNsPtr *oldXPNamespaces;

    if (comp == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	     "xsl:apply-templates : compilation failed\n");
	return;
    }
    if ((ctxt == NULL) || (node == NULL) || (inst == NULL) || (comp == NULL))
	return;

#ifdef WITH_XSLT_DEBUG_PROCESS
    if ((node != NULL) && (node->name != NULL))
	XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltApplyTemplates: node: '%s'\n", node->name));
#endif

    xpctxt = ctxt->xpathCtxt;
    oldContextNode = ctxt->node;
    oldMode = ctxt->mode;
    oldModeURI = ctxt->modeURI;
    oldDocInfo = ctxt->document;
    oldList = ctxt->nodeList;

    oldXPContextSize = xpctxt->contextSize;
    oldXPProximityPosition = xpctxt->proximityPosition;
    oldXPDoc = xpctxt->doc;
    oldXPNsNr = xpctxt->nsNr;
    oldXPNamespaces = xpctxt->namespaces;

    ctxt->mode = comp->mode;
    ctxt->modeURI = comp->modeURI;

    if (comp->select != NULL) {
	xmlXPathObjectPtr res = NULL;

	if (comp->comp == NULL) {
	    xsltTransformError(ctxt, NULL, inst,
		 "xsl:apply-templates : compilation failed\n");
	    goto error;
	}
#ifdef WITH_XSLT_DEBUG_PROCESS
	XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltApplyTemplates: select %s\n", comp->select));
#endif

	xpctxt->node = node; 
#ifdef XSLT_REFACTORED
	if (comp->inScopeNs != NULL) {
	    xpctxt->namespaces = comp->inScopeNs->list;
	    xpctxt->nsNr = comp->inScopeNs->xpathNumber;
	} else {
	    xpctxt->namespaces = NULL;
	    xpctxt->nsNr = 0;
	}
#else
	xpctxt->namespaces = comp->nsList;
	xpctxt->nsNr = comp->nsNr;
#endif
	res = xmlXPathCompiledEval(comp->comp, xpctxt);

	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	if (res != NULL) {
	    if (res->type == XPATH_NODESET) {
		list = res->nodesetval; 
		res->nodesetval = NULL;
	    } else {
		xsltTransformError(ctxt, NULL, inst,
		    "The 'select' expression did not evaluate to a "
		    "node set.\n");
		ctxt->state = XSLT_STATE_STOPPED;
		xmlXPathFreeObject(res);
		goto error;
	    }
	    xmlXPathFreeObject(res);
	} else {
	    xsltTransformError(ctxt, NULL, inst,
		"Failed to evaluate the 'select' expression.\n");
	    ctxt->state = XSLT_STATE_STOPPED;
	    goto error;
	}
	if (list == NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
		"xsltApplyTemplates: select didn't evaluate to a node list\n"));
#endif
	    goto exit;
	}
#if 0
	if ((ctxt->nbKeys > 0) &&
	    (list->nodeNr != 0) &&
	    (list->nodeTab[0]->doc != NULL) &&
	    XSLT_IS_RES_TREE_FRAG(list->nodeTab[0]->doc))
	{
	    isRTF = 1;
	    effectiveDocInfo = list->nodeTab[0]->doc->_private;
	}
#endif
    } else {
	list = xmlXPathNodeSetCreate(NULL);
	if (list == NULL)
	    goto error;
	cur = node->children;
	while (cur != NULL) {
	    switch (cur->type) {
		case XML_TEXT_NODE:
		    if ((IS_BLANK_NODE(cur)) &&
			(cur->parent != NULL) &&
			(cur->parent->type == XML_ELEMENT_NODE) &&
			(ctxt->style->stripSpaces != NULL)) {
			const xmlChar *val;

			if (cur->parent->ns != NULL) {
			    val = (const xmlChar *)
				  xmlHashLookup2(ctxt->style->stripSpaces,
						 cur->parent->name,
						 cur->parent->ns->href);
			    if (val == NULL) {
				val = (const xmlChar *)
				  xmlHashLookup2(ctxt->style->stripSpaces,
						 BAD_CAST "*",
						 cur->parent->ns->href);
			    }
			} else {
			    val = (const xmlChar *)
				  xmlHashLookup2(ctxt->style->stripSpaces,
						 cur->parent->name, NULL);
			}
			if ((val != NULL) &&
			    (xmlStrEqual(val, (xmlChar *) "strip"))) {
			    delNode = cur;
			    break;
			}
		    }
		    
		case XML_ELEMENT_NODE:
		case XML_DOCUMENT_NODE:
		case XML_HTML_DOCUMENT_NODE:
		case XML_CDATA_SECTION_NODE:
		case XML_PI_NODE:
		case XML_COMMENT_NODE:
		    xmlXPathNodeSetAddUnique(list, cur);
		    break;
		case XML_DTD_NODE:
		    if (cur->next != NULL)
			cur->next->prev = cur->prev;
		    if (cur->prev != NULL)
			cur->prev->next = cur->next;
		    break;
		default:
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltApplyTemplates: skipping cur type %d\n",
				     cur->type));
#endif
		    delNode = cur;
	    }
	    cur = cur->next;
	    if (delNode != NULL) {
#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltApplyTemplates: removing ignorable blank cur\n"));
#endif
		xmlUnlinkNode(delNode);
		xmlFreeNode(delNode);
		delNode = NULL;
	    }
	}
    }

#ifdef WITH_XSLT_DEBUG_PROCESS
    if (list != NULL)
    XSLT_TRACE(ctxt,XSLT_TRACE_APPLY_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	"xsltApplyTemplates: list of %d nodes\n", list->nodeNr));
#endif

    if ((list == NULL) || (list->nodeNr == 0))
	goto exit;

    ctxt->nodeList = list;
    if (inst->children) {
	xsltStackElemPtr param;

	cur = inst->children;
	while (cur) {

#ifdef WITH_DEBUGGER
	    if (ctxt->debugStatus != XSLT_DEBUG_NONE)
		xslHandleDebugger(cur, node, NULL, ctxt);
#endif
	    if (ctxt->state == XSLT_STATE_STOPPED)
		break;
	    if (cur->type == XML_TEXT_NODE) {
		cur = cur->next;
		continue;
	    }
	    if (! IS_XSLT_ELEM(cur))
		break;
	    if (IS_XSLT_NAME(cur, "with-param")) {
		param = xsltParseStylesheetCallerParam(ctxt, cur);
		if (param != NULL) {
		    param->next = withParams;
		    withParams = param;
		}
	    }
	    if (IS_XSLT_NAME(cur, "sort")) {
		xsltTemplatePtr oldCurTempRule =
		    ctxt->currentTemplateRule;
		int nbsorts = 0;
		xmlNodePtr sorts[XSLT_MAX_SORT];

		sorts[nbsorts++] = cur;

		while (cur) {

#ifdef WITH_DEBUGGER
		    if (ctxt->debugStatus != XSLT_DEBUG_NONE)
			xslHandleDebugger(cur, node, NULL, ctxt);
#endif
		    if (ctxt->state == XSLT_STATE_STOPPED)
			break;

		    if (cur->type == XML_TEXT_NODE) {
			cur = cur->next;
			continue;
		    }

		    if (! IS_XSLT_ELEM(cur))
			break;
		    if (IS_XSLT_NAME(cur, "with-param")) {
			param = xsltParseStylesheetCallerParam(ctxt, cur);
			if (param != NULL) {
			    param->next = withParams;
			    withParams = param;
			}
		    }
		    if (IS_XSLT_NAME(cur, "sort")) {
			if (nbsorts >= XSLT_MAX_SORT) {
			    xsltTransformError(ctxt, NULL, cur,
				"The number (%d) of xsl:sort instructions exceeds the "
				"maximum allowed by this processor's settings.\n",
				nbsorts);
			    ctxt->state = XSLT_STATE_STOPPED;
			    break;
			} else {
			    sorts[nbsorts++] = cur;
			}
		    }
		    cur = cur->next;
		}
		ctxt->currentTemplateRule = NULL;
		xsltDoSortFunction(ctxt, sorts, nbsorts);
		ctxt->currentTemplateRule = oldCurTempRule;
		break;
	    }
	    cur = cur->next;
	}
    }
    xpctxt->contextSize = list->nodeNr;
    for (i = 0; i < list->nodeNr; i++) {
	cur = list->nodeTab[i];
	ctxt->node = cur;
	if ((cur->type != XML_NAMESPACE_DECL) && (cur->doc != NULL))
	    xpctxt->doc = cur->doc;

	xpctxt->proximityPosition = i + 1;
	xsltProcessOneNode(ctxt, cur, withParams);
    }

exit:
error:
    if (withParams != NULL)
	xsltFreeStackElemList(withParams);
    if (list != NULL)
	xmlXPathFreeNodeSet(list);
    xpctxt->nsNr = oldXPNsNr;
    xpctxt->namespaces = oldXPNamespaces;
    xpctxt->doc = oldXPDoc;
    xpctxt->contextSize = oldXPContextSize;
    xpctxt->proximityPosition = oldXPProximityPosition;

    ctxt->document = oldDocInfo;
    ctxt->nodeList = oldList;
    ctxt->node = oldContextNode;
    ctxt->mode = oldMode;
    ctxt->modeURI = oldModeURI;
}


void
xsltChoose(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	   xmlNodePtr inst, xsltStylePreCompPtr comp ATTRIBUTE_UNUSED)
{
    xmlNodePtr cur;

    if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL))
	return;

    cur = inst->children;
    if (cur == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "xsl:choose: The instruction has no content.\n");
	return;
    }

#ifdef XSLT_REFACTORED
#else
    if ((! IS_XSLT_ELEM(cur)) || (! IS_XSLT_NAME(cur, "when"))) {
	xsltTransformError(ctxt, NULL, inst,
	     "xsl:choose: xsl:when expected first\n");
	return;
    }
#endif

    {
	int testRes = 0, res = 0;
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	xmlDocPtr oldXPContextDoc = xpctxt->doc;
	int oldXPProximityPosition = xpctxt->proximityPosition;
	int oldXPContextSize = xpctxt->contextSize;
	xmlNsPtr *oldXPNamespaces = xpctxt->namespaces;
	int oldXPNsNr = xpctxt->nsNr;

#ifdef XSLT_REFACTORED
	xsltStyleItemWhenPtr wcomp = NULL;
#else
	xsltStylePreCompPtr wcomp = NULL;
#endif

	while (IS_XSLT_ELEM(cur) && IS_XSLT_NAME(cur, "when")) {
	    wcomp = cur->psvi;

	    if ((wcomp == NULL) || (wcomp->test == NULL) ||
		(wcomp->comp == NULL))
	    {
		xsltTransformError(ctxt, NULL, cur,
		    "Internal error in xsltChoose(): "
		    "The XSLT 'when' instruction was not compiled.\n");
		goto error;
	    }


#ifdef WITH_DEBUGGER
	    if (xslDebugStatus != XSLT_DEBUG_NONE) {
		xslHandleDebugger(cur, contextNode, NULL, ctxt);
	    }
#endif
#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_CHOOSE,xsltGenericDebug(xsltGenericDebugContext,
		"xsltChoose: test %s\n", wcomp->test));
#endif

	    xpctxt->node = contextNode;
	    xpctxt->doc = oldXPContextDoc;
	    xpctxt->proximityPosition = oldXPProximityPosition;
	    xpctxt->contextSize = oldXPContextSize;

#ifdef XSLT_REFACTORED
	    if (wcomp->inScopeNs != NULL) {
		xpctxt->namespaces = wcomp->inScopeNs->list;
		xpctxt->nsNr = wcomp->inScopeNs->xpathNumber;
	    } else {
		xpctxt->namespaces = NULL;
		xpctxt->nsNr = 0;
	    }
#else
	    xpctxt->namespaces = wcomp->nsList;
	    xpctxt->nsNr = wcomp->nsNr;
#endif


#ifdef XSLT_FAST_IF
	    res = xmlXPathCompiledEvalToBoolean(wcomp->comp, xpctxt);

	    if (res == -1) {
		ctxt->state = XSLT_STATE_STOPPED;
		goto error;
	    }
	    testRes = (res == 1) ? 1 : 0;

#else 

	    res = xmlXPathCompiledEval(wcomp->comp, xpctxt);

	    if (res != NULL) {
		if (res->type != XPATH_BOOLEAN)
		    res = xmlXPathConvertBoolean(res);
		if (res->type == XPATH_BOOLEAN)
		    testRes = res->boolval;
		else {
#ifdef WITH_XSLT_DEBUG_PROCESS
		    XSLT_TRACE(ctxt,XSLT_TRACE_CHOOSE,xsltGenericDebug(xsltGenericDebugContext,
			"xsltChoose: test didn't evaluate to a boolean\n"));
#endif
		    goto error;
		}
		xmlXPathFreeObject(res);
		res = NULL;
	    } else {
		ctxt->state = XSLT_STATE_STOPPED;
		goto error;
	    }

#endif 

#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_CHOOSE,xsltGenericDebug(xsltGenericDebugContext,
		"xsltChoose: test evaluate to %d\n", testRes));
#endif
	    if (testRes)
		goto test_is_true;

	    cur = cur->next;
	}

	if (IS_XSLT_ELEM(cur) && IS_XSLT_NAME(cur, "otherwise")) {

#ifdef WITH_DEBUGGER
	    if (xslDebugStatus != XSLT_DEBUG_NONE)
		xslHandleDebugger(cur, contextNode, NULL, ctxt);
#endif

#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_CHOOSE,xsltGenericDebug(xsltGenericDebugContext,
		"evaluating xsl:otherwise\n"));
#endif
	    goto test_is_true;
	}
	xpctxt->node = contextNode;
	xpctxt->doc = oldXPContextDoc;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->namespaces = oldXPNamespaces;
	xpctxt->nsNr = oldXPNsNr;
	goto exit;

test_is_true:

	xpctxt->node = contextNode;
	xpctxt->doc = oldXPContextDoc;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->namespaces = oldXPNamespaces;
	xpctxt->nsNr = oldXPNsNr;
	goto process_sequence;
    }

process_sequence:

    xsltApplySequenceConstructor(ctxt, ctxt->node, cur->children,
	NULL);

exit:
error:
    return;
}

void
xsltIf(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	           xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
    int res = 0;

#ifdef XSLT_REFACTORED
    xsltStyleItemIfPtr comp = (xsltStyleItemIfPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif

    if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL))
	return;
    if ((comp == NULL) || (comp->test == NULL) || (comp->comp == NULL)) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltIf(): "
	    "The XSLT 'if' instruction was not compiled.\n");
	return;
    }

#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_IF,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltIf: test %s\n", comp->test));
#endif

#ifdef XSLT_FAST_IF
    {
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	xmlDocPtr oldXPContextDoc = xpctxt->doc;
	xmlNsPtr *oldXPNamespaces = xpctxt->namespaces;
	xmlNodePtr oldXPContextNode = xpctxt->node;
	int oldXPProximityPosition = xpctxt->proximityPosition;
	int oldXPContextSize = xpctxt->contextSize;
	int oldXPNsNr = xpctxt->nsNr;
	xmlDocPtr oldLocalFragmentTop = ctxt->localRVT;

	xpctxt->node = contextNode;
	if (comp != NULL) {

#ifdef XSLT_REFACTORED
	    if (comp->inScopeNs != NULL) {
		xpctxt->namespaces = comp->inScopeNs->list;
		xpctxt->nsNr = comp->inScopeNs->xpathNumber;
	    } else {
		xpctxt->namespaces = NULL;
		xpctxt->nsNr = 0;
	    }
#else
	    xpctxt->namespaces = comp->nsList;
	    xpctxt->nsNr = comp->nsNr;
#endif
	} else {
	    xpctxt->namespaces = NULL;
	    xpctxt->nsNr = 0;
	}
	res = xmlXPathCompiledEvalToBoolean(comp->comp, xpctxt);

	if (oldLocalFragmentTop != ctxt->localRVT)
	    xsltReleaseLocalRVTs(ctxt, oldLocalFragmentTop);

	xpctxt->doc = oldXPContextDoc;
	xpctxt->node = oldXPContextNode;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->nsNr = oldXPNsNr;
	xpctxt->namespaces = oldXPNamespaces;
    }

#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_IF,xsltGenericDebug(xsltGenericDebugContext,
	"xsltIf: test evaluate to %d\n", res));
#endif

    if (res == -1) {
	ctxt->state = XSLT_STATE_STOPPED;
	goto error;
    }
    if (res == 1) {
	xsltApplySequenceConstructor(ctxt,
	    contextNode, inst->children, NULL);
    }

#else 
    {
	xmlXPathObjectPtr xpobj = NULL;
	{
	    xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	    xmlDocPtr oldXPContextDoc = xpctxt->doc;
	    xmlNsPtr *oldXPNamespaces = xpctxt->namespaces;
	    xmlNodePtr oldXPContextNode = xpctxt->node;
	    int oldXPProximityPosition = xpctxt->proximityPosition;
	    int oldXPContextSize = xpctxt->contextSize;
	    int oldXPNsNr = xpctxt->nsNr;

	    xpctxt->node = contextNode;
	    if (comp != NULL) {

#ifdef XSLT_REFACTORED
		if (comp->inScopeNs != NULL) {
		    xpctxt->namespaces = comp->inScopeNs->list;
		    xpctxt->nsNr = comp->inScopeNs->xpathNumber;
		} else {
		    xpctxt->namespaces = NULL;
		    xpctxt->nsNr = 0;
		}
#else
		xpctxt->namespaces = comp->nsList;
		xpctxt->nsNr = comp->nsNr;
#endif
	    } else {
		xpctxt->namespaces = NULL;
		xpctxt->nsNr = 0;
	    }

	    xpobj = xmlXPathCompiledEval(comp->comp, xpctxt);

	    xpctxt->doc = oldXPContextDoc;
	    xpctxt->node = oldXPContextNode;
	    xpctxt->contextSize = oldXPContextSize;
	    xpctxt->proximityPosition = oldXPProximityPosition;
	    xpctxt->nsNr = oldXPNsNr;
	    xpctxt->namespaces = oldXPNamespaces;
	}
	if (xpobj != NULL) {
	    if (xpobj->type != XPATH_BOOLEAN)
		xpobj = xmlXPathConvertBoolean(xpobj);
	    if (xpobj->type == XPATH_BOOLEAN) {
		res = xpobj->boolval;

#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt,XSLT_TRACE_IF,xsltGenericDebug(xsltGenericDebugContext,
		    "xsltIf: test evaluate to %d\n", res));
#endif
		if (res) {
		    xsltApplySequenceConstructor(ctxt,
			contextNode, inst->children, NULL);
		}
	    } else {

#ifdef WITH_XSLT_DEBUG_PROCESS
		XSLT_TRACE(ctxt, XSLT_TRACE_IF,
		    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltIf: test didn't evaluate to a boolean\n"));
#endif
		ctxt->state = XSLT_STATE_STOPPED;
	    }
	    xmlXPathFreeObject(xpobj);
	} else {
	    ctxt->state = XSLT_STATE_STOPPED;
	}
    }
#endif 

error:
    return;
}

void
xsltForEach(xsltTransformContextPtr ctxt, xmlNodePtr contextNode,
	    xmlNodePtr inst, xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemForEachPtr comp = (xsltStyleItemForEachPtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    int i;
    xmlXPathObjectPtr res = NULL;
    xmlNodePtr cur, curInst;
    xmlNodeSetPtr list = NULL;
    xmlNodeSetPtr oldList;
    int oldXPProximityPosition, oldXPContextSize;
    xmlNodePtr oldContextNode;
    xsltTemplatePtr oldCurTemplRule;
    xmlDocPtr oldXPDoc;
    xsltDocumentPtr oldDocInfo;
    xmlXPathContextPtr xpctxt;

    if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL)) {
	xsltGenericError(xsltGenericErrorContext,
	    "xsltForEach(): Bad arguments.\n");
	return;
    }

    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltForEach(): "
	    "The XSLT 'for-each' instruction was not compiled.\n");
        return;
    }
    if ((comp->select == NULL) || (comp->comp == NULL)) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltForEach(): "
	    "The selecting expression of the XSLT 'for-each' "
	    "instruction was not compiled correctly.\n");
	return;
    }
    xpctxt = ctxt->xpathCtxt;

#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_FOR_EACH,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltForEach: select %s\n", comp->select));
#endif

    oldDocInfo = ctxt->document;
    oldList = ctxt->nodeList;
    oldContextNode = ctxt->node;
    oldCurTemplRule = ctxt->currentTemplateRule;
    ctxt->currentTemplateRule = NULL;

    oldXPDoc = xpctxt->doc;
    oldXPProximityPosition = xpctxt->proximityPosition;
    oldXPContextSize = xpctxt->contextSize;
    xpctxt->node = contextNode;
#ifdef XSLT_REFACTORED
    if (comp->inScopeNs != NULL) {
	xpctxt->namespaces = comp->inScopeNs->list;
	xpctxt->nsNr = comp->inScopeNs->xpathNumber;
    } else {
	xpctxt->namespaces = NULL;
	xpctxt->nsNr = 0;
    }
#else
    xpctxt->namespaces = comp->nsList;
    xpctxt->nsNr = comp->nsNr;
#endif

    res = xmlXPathCompiledEval(comp->comp, ctxt->xpathCtxt);

    if (res != NULL) {
	if (res->type == XPATH_NODESET)
	    list = res->nodesetval;
	else {
	    xsltTransformError(ctxt, NULL, inst,
		"The 'select' expression does not evaluate to a node set.\n");

#ifdef WITH_XSLT_DEBUG_PROCESS
	    XSLT_TRACE(ctxt,XSLT_TRACE_FOR_EACH,xsltGenericDebug(xsltGenericDebugContext,
		"xsltForEach: select didn't evaluate to a node list\n"));
#endif
	    goto error;
	}
    } else {
	xsltTransformError(ctxt, NULL, inst,
	    "Failed to evaluate the 'select' expression.\n");
	ctxt->state = XSLT_STATE_STOPPED;
	goto error;
    }

    if ((list == NULL) || (list->nodeNr <= 0))
	goto exit;

#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_FOR_EACH,xsltGenericDebug(xsltGenericDebugContext,
	"xsltForEach: select evaluates to %d nodes\n", list->nodeNr));
#endif

    xpctxt->contextSize = oldXPContextSize;
    xpctxt->proximityPosition = oldXPProximityPosition;
    xpctxt->node = contextNode;

    ctxt->nodeList = list;
    curInst = inst->children;
    if (IS_XSLT_ELEM(curInst) && IS_XSLT_NAME(curInst, "sort")) {
	int nbsorts = 0;
	xmlNodePtr sorts[XSLT_MAX_SORT];

	sorts[nbsorts++] = curInst;

#ifdef WITH_DEBUGGER
	if (xslDebugStatus != XSLT_DEBUG_NONE)
	    xslHandleDebugger(curInst, contextNode, NULL, ctxt);
#endif

	curInst = curInst->next;
	while (IS_XSLT_ELEM(curInst) && IS_XSLT_NAME(curInst, "sort")) {
	    if (nbsorts >= XSLT_MAX_SORT) {
		xsltTransformError(ctxt, NULL, curInst,
		    "The number of xsl:sort instructions exceeds the "
		    "maximum (%d) allowed by this processor.\n",
		    XSLT_MAX_SORT);
		goto error;
	    } else {
		sorts[nbsorts++] = curInst;
	    }

#ifdef WITH_DEBUGGER
	    if (xslDebugStatus != XSLT_DEBUG_NONE)
		xslHandleDebugger(curInst, contextNode, NULL, ctxt);
#endif
	    curInst = curInst->next;
	}
	xsltDoSortFunction(ctxt, sorts, nbsorts);
    }
    xpctxt->contextSize = list->nodeNr;
    for (i = 0; i < list->nodeNr; i++) {
	cur = list->nodeTab[i];
	ctxt->node = cur;
	if ((cur->type != XML_NAMESPACE_DECL) && (cur->doc != NULL))
	    xpctxt->doc = cur->doc;

	xpctxt->proximityPosition = i + 1;

	xsltApplySequenceConstructor(ctxt, cur, curInst, NULL);
    }

exit:
error:
    if (res != NULL)
	xmlXPathFreeObject(res);
    ctxt->document = oldDocInfo;
    ctxt->nodeList = oldList;
    ctxt->node = oldContextNode;
    ctxt->currentTemplateRule = oldCurTemplRule;

    xpctxt->doc = oldXPDoc;
    xpctxt->contextSize = oldXPContextSize;
    xpctxt->proximityPosition = oldXPProximityPosition;
}


#ifdef XSLT_GENERATE_HTML_DOCTYPE
typedef struct xsltHTMLVersion {
    const char *version;
    const char *public;
    const char *system;
} xsltHTMLVersion;

static xsltHTMLVersion xsltHTMLVersions[] = {
    { "4.01frame", "-//W3C//DTD HTML 4.01 Frameset//EN",
      "http://www.w3.org/TR/1999/REC-html401-19991224/frameset.dtd"},
    { "4.01strict", "-//W3C//DTD HTML 4.01//EN",
      "http://www.w3.org/TR/1999/REC-html401-19991224/strict.dtd"},
    { "4.01trans", "-//W3C//DTD HTML 4.01 Transitional//EN",
      "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd"},
    { "4.01", "-//W3C//DTD HTML 4.01 Transitional//EN",
      "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd"},
    { "4.0strict", "-//W3C//DTD HTML 4.01//EN",
      "http://www.w3.org/TR/html4/strict.dtd"},
    { "4.0trans", "-//W3C//DTD HTML 4.01 Transitional//EN",
      "http://www.w3.org/TR/html4/loose.dtd"},
    { "4.0frame", "-//W3C//DTD HTML 4.01 Frameset//EN",
      "http://www.w3.org/TR/html4/frameset.dtd"},
    { "4.0", "-//W3C//DTD HTML 4.01 Transitional//EN",
      "http://www.w3.org/TR/html4/loose.dtd"},
    { "3.2", "-//W3C//DTD HTML 3.2//EN", NULL }
};

static int
xsltGetHTMLIDs(const xmlChar *version, const xmlChar **publicID,
	            const xmlChar **systemID) {
    unsigned int i;
    if (version == NULL)
	return(-1);
    for (i = 0;i < (sizeof(xsltHTMLVersions)/sizeof(xsltHTMLVersions[1]));
	 i++) {
	if (!xmlStrcasecmp(version,
		           (const xmlChar *) xsltHTMLVersions[i].version)) {
	    if (publicID != NULL)
		*publicID = (const xmlChar *) xsltHTMLVersions[i].public;
	    if (systemID != NULL)
		*systemID = (const xmlChar *) xsltHTMLVersions[i].system;
	    return(0);
	}
    }
    return(-1);
}
#endif

void
xsltApplyStripSpaces(xsltTransformContextPtr ctxt, xmlNodePtr node) {
    xmlNodePtr current;
#ifdef WITH_XSLT_DEBUG_PROCESS
    int nb = 0;
#endif


    current = node;
    while (current != NULL) {
	if ((IS_XSLT_REAL_NODE(current)) &&
	    (current->children != NULL) &&
	    (xsltFindElemSpaceHandling(ctxt, current))) {
	    xmlNodePtr delete = NULL, cur = current->children;

	    while (cur != NULL) {
		if (IS_BLANK_NODE(cur))
		    delete = cur;

		cur = cur->next;
		if (delete != NULL) {
		    xmlUnlinkNode(delete);
		    xmlFreeNode(delete);
		    delete = NULL;
#ifdef WITH_XSLT_DEBUG_PROCESS
		    nb++;
#endif
		}
	    }
	}

	if (node->type == XML_ENTITY_REF_NODE) {
	    
	    xsltApplyStripSpaces(ctxt, node->children);
	}
	if ((current->children != NULL) &&
            (current->type != XML_ENTITY_REF_NODE)) {
	    current = current->children;
	} else if (current->next != NULL) {
	    current = current->next;
	} else {
	    do {
		current = current->parent;
		if (current == NULL)
		    break;
		if (current == node)
		    goto done;
		if (current->next != NULL) {
		    current = current->next;
		    break;
		}
	    } while (current != NULL);
	}
    }

done:
#ifdef WITH_XSLT_DEBUG_PROCESS
    XSLT_TRACE(ctxt,XSLT_TRACE_STRIP_SPACES,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltApplyStripSpaces: removed %d ignorable blank node\n", nb));
#endif
    return;
}

static int
xsltCountKeys(xsltTransformContextPtr ctxt)
{
    xsltStylesheetPtr style;
    xsltKeyDefPtr keyd;

    if (ctxt == NULL)
	return(-1);

    ctxt->hasTemplKeyPatterns = 0;
    style = ctxt->style;
    while (style != NULL) {
	if (style->keyMatch != NULL) {
	    ctxt->hasTemplKeyPatterns = 1;
	    break;
	}
	style = xsltNextImport(style);
    }
    ctxt->nbKeys = 0;
    style = ctxt->style;
    while (style != NULL) {
	keyd = style->keys;
	while (keyd) {
	    ctxt->nbKeys++;
	    keyd = keyd->next;
	}
	style = xsltNextImport(style);
    }
    return(ctxt->nbKeys);
}

static xmlDocPtr
xsltApplyStylesheetInternal(xsltStylesheetPtr style, xmlDocPtr doc,
                            const char **params, const char *output,
                            FILE * profile, xsltTransformContextPtr userCtxt)
{
    xmlDocPtr res = NULL;
    xsltTransformContextPtr ctxt = NULL;
    xmlNodePtr root, node;
    const xmlChar *method;
    const xmlChar *doctypePublic;
    const xmlChar *doctypeSystem;
    const xmlChar *version;
    const xmlChar *encoding;
    xsltStackElemPtr variables;
    xsltStackElemPtr vptr;

    xsltInitGlobals();

    if ((style == NULL) || (doc == NULL))
        return (NULL);

    if (style->internalized == 0) {
#ifdef WITH_XSLT_DEBUG
	xsltGenericDebug(xsltGenericDebugContext,
			 "Stylesheet was not fully internalized !\n");
#endif
    }
    if (doc->intSubset != NULL) {
	xmlNodePtr cur = (xmlNodePtr) doc->intSubset;
	if (cur->next != NULL)
	    cur->next->prev = cur->prev;
	if (cur->prev != NULL)
	    cur->prev->next = cur->next;
	if (doc->children == cur)
	    doc->children = cur->next;
	if (doc->last == cur)
	    doc->last = cur->prev;
	cur->prev = cur->next = NULL;
    }

    root = xmlDocGetRootElement(doc);
    if (root != NULL) {
	if (((long) root->content) >= 0 && (xslDebugStatus == XSLT_DEBUG_NONE))
	    xmlXPathOrderDocElems(doc);
    }

    if (userCtxt != NULL)
	ctxt = userCtxt;
    else
	ctxt = xsltNewTransformContext(style, doc);

    if (ctxt == NULL)
        return (NULL);

    ctxt->initialContextDoc = doc;
    ctxt->initialContextNode = (xmlNodePtr) doc;

    if (profile != NULL)
        ctxt->profile = 1;

    if (output != NULL)
        ctxt->outputFile = output;
    else
        ctxt->outputFile = NULL;

    if (ctxt->dict != NULL) {
        if (ctxt->mode != NULL)
	    ctxt->mode = xmlDictLookup(ctxt->dict, ctxt->mode, -1);
        if (ctxt->modeURI != NULL)
	    ctxt->modeURI = xmlDictLookup(ctxt->dict, ctxt->modeURI, -1);
    }

    XSLT_GET_IMPORT_PTR(method, style, method)
    XSLT_GET_IMPORT_PTR(doctypePublic, style, doctypePublic)
    XSLT_GET_IMPORT_PTR(doctypeSystem, style, doctypeSystem)
    XSLT_GET_IMPORT_PTR(version, style, version)
    XSLT_GET_IMPORT_PTR(encoding, style, encoding)

    if ((method != NULL) &&
	(!xmlStrEqual(method, (const xmlChar *) "xml")))
    {
        if (xmlStrEqual(method, (const xmlChar *) "html")) {
            ctxt->type = XSLT_OUTPUT_HTML;
            if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
                res = htmlNewDoc(doctypeSystem, doctypePublic);
	    } else {
                if (version == NULL) {
		    xmlDtdPtr dtd;

		    res = htmlNewDoc(NULL, NULL);
		    if (res != NULL) {
			dtd = xmlGetIntSubset(res);
			if (dtd != NULL) {
			    xmlUnlinkNode((xmlNodePtr) dtd);
			    xmlFreeDtd(dtd);
			}
			res->intSubset = NULL;
			res->extSubset = NULL;
		    }
		} else {

#ifdef XSLT_GENERATE_HTML_DOCTYPE
		    xsltGetHTMLIDs(version, &doctypePublic, &doctypeSystem);
#endif
		    res = htmlNewDoc(doctypeSystem, doctypePublic);
		}
            }
            if (res == NULL)
                goto error;
	    res->dict = ctxt->dict;
	    xmlDictReference(res->dict);

#ifdef WITH_XSLT_DEBUG
	    xsltGenericDebug(xsltGenericDebugContext,
		"reusing transformation dict for output\n");
#endif
        } else if (xmlStrEqual(method, (const xmlChar *) "xhtml")) {
	    xsltTransformError(ctxt, NULL, (xmlNodePtr) doc,
		"xsltApplyStylesheetInternal: unsupported method xhtml, using html\n",
		style->method);
            ctxt->type = XSLT_OUTPUT_HTML;
            res = htmlNewDoc(doctypeSystem, doctypePublic);
            if (res == NULL)
                goto error;
	    res->dict = ctxt->dict;
	    xmlDictReference(res->dict);

#ifdef WITH_XSLT_DEBUG
	    xsltGenericDebug(xsltGenericDebugContext,
		"reusing transformation dict for output\n");
#endif
        } else if (xmlStrEqual(method, (const xmlChar *) "text")) {
            ctxt->type = XSLT_OUTPUT_TEXT;
            res = xmlNewDoc(style->version);
            if (res == NULL)
                goto error;
	    res->dict = ctxt->dict;
	    xmlDictReference(res->dict);

#ifdef WITH_XSLT_DEBUG
	    xsltGenericDebug(xsltGenericDebugContext,
		"reusing transformation dict for output\n");
#endif
        } else {
	    xsltTransformError(ctxt, NULL, (xmlNodePtr) doc,
		"xsltApplyStylesheetInternal: unsupported method %s\n",
		style->method);
            goto error;
        }
    } else {
        ctxt->type = XSLT_OUTPUT_XML;
        res = xmlNewDoc(style->version);
        if (res == NULL)
            goto error;
	res->dict = ctxt->dict;
	xmlDictReference(ctxt->dict);
#ifdef WITH_XSLT_DEBUG
	xsltGenericDebug(xsltGenericDebugContext,
			 "reusing transformation dict for output\n");
#endif
    }
    res->charset = XML_CHAR_ENCODING_UTF8;
    if (encoding != NULL)
        res->encoding = xmlStrdup(encoding);
    variables = style->variables;

    if (xsltNeedElemSpaceHandling(ctxt))
	xsltApplyStripSpaces(ctxt, xmlDocGetRootElement(doc));
    ctxt->node = (xmlNodePtr) doc;
    if (ctxt->globalVars == NULL)
	ctxt->globalVars = xmlHashCreate(20);
    if (params != NULL) {
        xsltEvalUserParams(ctxt, params);
    }

    
    xsltCountKeys(ctxt);

    xsltEvalGlobalVariables(ctxt);

    ctxt->node = (xmlNodePtr) doc;
    ctxt->output = res;
    ctxt->insert = (xmlNodePtr) res;
    ctxt->varsBase = ctxt->varsNr - 1;

    ctxt->xpathCtxt->contextSize = 1;
    ctxt->xpathCtxt->proximityPosition = 1;
    ctxt->xpathCtxt->node = NULL; 
    xsltProcessOneNode(ctxt, ctxt->node, NULL);
    xsltLocalVariablePop(ctxt, 0, -2);
    xsltShutdownCtxtExts(ctxt);

    xsltCleanupTemplates(style); 

    if (style->variables != variables) {
        vptr = style->variables;
        while (vptr->next != variables)
            vptr = vptr->next;
        vptr->next = NULL;
        xsltFreeStackElemList(style->variables);
        style->variables = variables;
    }
    vptr = style->variables;
    while (vptr != NULL) {
        if (vptr->computed) {
            if (vptr->value != NULL) {
                xmlXPathFreeObject(vptr->value);
                vptr->value = NULL;
                vptr->computed = 0;
            }
        }
        vptr = vptr->next;
    }
#if 0
    xsltFreeRVTs(ctxt);
#endif
    root = xmlDocGetRootElement(res);
    if (root != NULL) {
        const xmlChar *doctype = NULL;

        if ((root->ns != NULL) && (root->ns->prefix != NULL))
	    doctype = xmlDictQLookup(ctxt->dict, root->ns->prefix, root->name);
	if (doctype == NULL)
	    doctype = root->name;

        if ((method == NULL) &&
            (root->ns == NULL) &&
            (!xmlStrcasecmp(root->name, (const xmlChar *) "html"))) {
            xmlNodePtr tmp;

            tmp = res->children;
            while ((tmp != NULL) && (tmp != root)) {
                if (tmp->type == XML_ELEMENT_NODE)
                    break;
                if ((tmp->type == XML_TEXT_NODE) && (!xmlIsBlankNode(tmp)))
                    break;
		tmp = tmp->next;
            }
            if (tmp == root) {
                ctxt->type = XSLT_OUTPUT_HTML;
                res->type = XML_HTML_DOCUMENT_NODE;
                if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
                    res->intSubset = xmlCreateIntSubset(res, doctype,
                                                        doctypePublic,
                                                        doctypeSystem);
#ifdef XSLT_GENERATE_HTML_DOCTYPE
		} else if (version != NULL) {
                    xsltGetHTMLIDs(version, &doctypePublic,
                                   &doctypeSystem);
                    if (((doctypePublic != NULL) || (doctypeSystem != NULL)))
                        res->intSubset =
                            xmlCreateIntSubset(res, doctype,
                                               doctypePublic,
                                               doctypeSystem);
#endif
                }
            }

        }
        if (ctxt->type == XSLT_OUTPUT_XML) {
            XSLT_GET_IMPORT_PTR(doctypePublic, style, doctypePublic)
            XSLT_GET_IMPORT_PTR(doctypeSystem, style, doctypeSystem)
            if (((doctypePublic != NULL) || (doctypeSystem != NULL))) {
	        xmlNodePtr last;
		node = res->children;
		last = res->last;
		res->children = NULL;
		res->last = NULL;
                res->intSubset = xmlCreateIntSubset(res, doctype,
                                                    doctypePublic,
                                                    doctypeSystem);
		if (res->children != NULL) {
		    res->children->next = node;
		    node->prev = res->children;
		    res->last = last;
		} else {
		    res->children = node;
		    res->last = last;
		}
	    }
        }
    }
    xmlXPathFreeNodeSet(ctxt->nodeList);
    if (profile != NULL) {
        xsltSaveProfiling(ctxt, profile);
    }

    if ((ctxt != NULL) && (ctxt->state == XSLT_STATE_ERROR)) {
	xmlFreeDoc(res);
	res = NULL;
    }
    if ((res != NULL) && (ctxt != NULL) && (output != NULL)) {
	int ret;

	ret = xsltCheckWrite(ctxt->sec, ctxt, (const xmlChar *) output);
	if (ret == 0) {
	    xsltTransformError(ctxt, NULL, NULL,
		     "xsltApplyStylesheet: forbidden to save to %s\n",
			       output);
	} else if (ret < 0) {
	    xsltTransformError(ctxt, NULL, NULL,
		     "xsltApplyStylesheet: saving to %s may not be possible\n",
			       output);
	}
    }

#ifdef XSLT_DEBUG_PROFILE_CACHE
    printf("# Cache:\n");
    printf("# Reused tree fragments: %d\n", ctxt->cache->dbgReusedRVTs);
    printf("# Reused variables     : %d\n", ctxt->cache->dbgReusedVars);
#endif

    if ((ctxt != NULL) && (userCtxt == NULL))
	xsltFreeTransformContext(ctxt);

    return (res);

error:
    if (res != NULL)
        xmlFreeDoc(res);

#ifdef XSLT_DEBUG_PROFILE_CACHE
    printf("# Cache:\n");
    printf("# Reused tree fragments: %d\n", ctxt->cache->dbgReusedRVTs);
    printf("# Reused variables     : %d\n", ctxt->cache->dbgReusedVars);
#endif

    if ((ctxt != NULL) && (userCtxt == NULL))
        xsltFreeTransformContext(ctxt);
    return (NULL);
}

xmlDocPtr
xsltApplyStylesheet(xsltStylesheetPtr style, xmlDocPtr doc,
                    const char **params)
{
    return (xsltApplyStylesheetInternal(style, doc, params, NULL, NULL, NULL));
}

xmlDocPtr
xsltProfileStylesheet(xsltStylesheetPtr style, xmlDocPtr doc,
                      const char **params, FILE * output)
{
    xmlDocPtr res;

    res = xsltApplyStylesheetInternal(style, doc, params, NULL, output, NULL);
    return (res);
}

xmlDocPtr
xsltApplyStylesheetUser(xsltStylesheetPtr style, xmlDocPtr doc,
                            const char **params, const char *output,
                            FILE * profile, xsltTransformContextPtr userCtxt)
{
    xmlDocPtr res;

    res = xsltApplyStylesheetInternal(style, doc, params, output,
	                              profile, userCtxt);
    return (res);
}

/**
 * xsltRunStylesheetUser:
 * @style:  a parsed XSLT stylesheet
 * @doc:  a parsed XML document
 * @params:  a NULL terminated array of parameters names/values tuples
 * @output:  the URL/filename ot the generated resource if available
 * @SAX:  a SAX handler for progressive callback output (not implemented yet)
 * @IObuf:  an output buffer for progressive output (not implemented yet)
 * @profile:  profile FILE * output or NULL
 * @userCtxt:  user provided transform context
 *
 * Apply the stylesheet to the document and generate the output according
 * to @output @SAX and @IObuf. It's an error to specify both @SAX and @IObuf.
 *
 * NOTE: This may lead to a non-wellformed output XML wise !
 * NOTE: This may also result in multiple files being generated
 * NOTE: using IObuf, the result encoding used will be the one used for
 *       creating the output buffer, use the following macro to read it
 *       from the stylesheet
 *       XSLT_GET_IMPORT_PTR(encoding, style, encoding)
 * NOTE: using SAX, any encoding specified in the stylesheet will be lost
 *       since the interface uses only UTF8
 *
 * Returns the number of by written to the main resource or -1 in case of
 *         error.
 */
int
xsltRunStylesheetUser(xsltStylesheetPtr style, xmlDocPtr doc,
                  const char **params, const char *output,
                  xmlSAXHandlerPtr SAX, xmlOutputBufferPtr IObuf,
		  FILE * profile, xsltTransformContextPtr userCtxt)
{
    xmlDocPtr tmp;
    int ret;

    if ((output == NULL) && (SAX == NULL) && (IObuf == NULL))
        return (-1);
    if ((SAX != NULL) && (IObuf != NULL))
        return (-1);

    
    if (SAX != NULL) {
        XSLT_TODO   
	return (-1);
    }

    tmp = xsltApplyStylesheetInternal(style, doc, params, output, profile,
	                              userCtxt);
    if (tmp == NULL) {
	xsltTransformError(NULL, NULL, (xmlNodePtr) doc,
                         "xsltRunStylesheet : run failed\n");
        return (-1);
    }
    if (IObuf != NULL) {
        
        ret = xsltSaveResultTo(IObuf, tmp, style);
    } else {
        ret = xsltSaveResultToFilename(output, tmp, style, 0);
    }
    xmlFreeDoc(tmp);
    return (ret);
}

/**
 * xsltRunStylesheet:
 * @style:  a parsed XSLT stylesheet
 * @doc:  a parsed XML document
 * @params:  a NULL terminated array of parameters names/values tuples
 * @output:  the URL/filename ot the generated resource if available
 * @SAX:  a SAX handler for progressive callback output (not implemented yet)
 * @IObuf:  an output buffer for progressive output (not implemented yet)
 *
 * Apply the stylesheet to the document and generate the output according
 * to @output @SAX and @IObuf. It's an error to specify both @SAX and @IObuf.
 *
 * NOTE: This may lead to a non-wellformed output XML wise !
 * NOTE: This may also result in multiple files being generated
 * NOTE: using IObuf, the result encoding used will be the one used for
 *       creating the output buffer, use the following macro to read it
 *       from the stylesheet
 *       XSLT_GET_IMPORT_PTR(encoding, style, encoding)
 * NOTE: using SAX, any encoding specified in the stylesheet will be lost
 *       since the interface uses only UTF8
 *
 * Returns the number of bytes written to the main resource or -1 in case of
 *         error.
 */
int
xsltRunStylesheet(xsltStylesheetPtr style, xmlDocPtr doc,
                  const char **params, const char *output,
                  xmlSAXHandlerPtr SAX, xmlOutputBufferPtr IObuf)
{
    return(xsltRunStylesheetUser(style, doc, params, output, SAX, IObuf,
		                 NULL, NULL));
}

void
xsltRegisterAllElement(xsltTransformContextPtr ctxt)
{
    xsltRegisterExtElement(ctxt, (const xmlChar *) "apply-templates",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltApplyTemplates);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "apply-imports",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltApplyImports);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "call-template",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltCallTemplate);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "element",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltElement);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "attribute",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltAttribute);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "text",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltText);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "processing-instruction",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltProcessingInstruction);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "comment",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltComment);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "copy",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltCopy);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "value-of",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltValueOf);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "number",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltNumber);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "for-each",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltForEach);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "if",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltIf);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "choose",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltChoose);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "sort",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltSort);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "copy-of",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltCopyOf);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "message",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltMessage);

    xsltRegisterExtElement(ctxt, (const xmlChar *) "variable",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "param",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "with-param",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "decimal-format",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "when",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "otherwise",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);
    xsltRegisterExtElement(ctxt, (const xmlChar *) "fallback",
                           XSLT_NAMESPACE,
			   (xsltTransformFunction) xsltDebug);

}
