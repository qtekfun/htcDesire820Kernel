/*
 * templates.c: Implementation of the template processing
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

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/globals.h>
#include <libxml/xmlerror.h>
#include <libxml/tree.h>
#include <libxml/dict.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "variables.h"
#include "functions.h"
#include "templates.h"
#include "transform.h"
#include "namespaces.h"
#include "attributes.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_TEMPLATES
#endif

 
int
xsltEvalXPathPredicate(xsltTransformContextPtr ctxt, xmlXPathCompExprPtr comp,
		       xmlNsPtr *nsList, int nsNr) {
    int ret;
    xmlXPathObjectPtr res;
    int oldNsNr;
    xmlNsPtr *oldNamespaces;
    xmlNodePtr oldInst;
    int oldProximityPosition, oldContextSize;

    oldContextSize = ctxt->xpathCtxt->contextSize;
    oldProximityPosition = ctxt->xpathCtxt->proximityPosition;
    oldNsNr = ctxt->xpathCtxt->nsNr;
    oldNamespaces = ctxt->xpathCtxt->namespaces;
    oldInst = ctxt->inst;

    ctxt->xpathCtxt->node = ctxt->node;
    ctxt->xpathCtxt->namespaces = nsList;
    ctxt->xpathCtxt->nsNr = nsNr;

    res = xmlXPathCompiledEval(comp, ctxt->xpathCtxt);

    if (res != NULL) {
	ret = xmlXPathEvalPredicate(ctxt->xpathCtxt, res);
	xmlXPathFreeObject(res);
#ifdef WITH_XSLT_DEBUG_TEMPLATES
	XSLT_TRACE(ctxt,XSLT_TRACE_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltEvalXPathPredicate: returns %d\n", ret));
#endif
    } else {
#ifdef WITH_XSLT_DEBUG_TEMPLATES
	XSLT_TRACE(ctxt,XSLT_TRACE_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltEvalXPathPredicate: failed\n"));
#endif
	ctxt->state = XSLT_STATE_STOPPED;
	ret = 0;
    }
    ctxt->xpathCtxt->nsNr = oldNsNr;

    ctxt->xpathCtxt->namespaces = oldNamespaces;
    ctxt->inst = oldInst;
    ctxt->xpathCtxt->contextSize = oldContextSize;
    ctxt->xpathCtxt->proximityPosition = oldProximityPosition;

    return(ret);
}

xmlChar *
xsltEvalXPathStringNs(xsltTransformContextPtr ctxt, xmlXPathCompExprPtr comp,
	              int nsNr, xmlNsPtr *nsList) {
    xmlChar *ret = NULL;
    xmlXPathObjectPtr res;
    xmlNodePtr oldInst;
    xmlNodePtr oldNode;
    int	oldPos, oldSize;
    int oldNsNr;
    xmlNsPtr *oldNamespaces;

    oldInst = ctxt->inst;
    oldNode = ctxt->node;
    oldPos = ctxt->xpathCtxt->proximityPosition;
    oldSize = ctxt->xpathCtxt->contextSize;
    oldNsNr = ctxt->xpathCtxt->nsNr;
    oldNamespaces = ctxt->xpathCtxt->namespaces;

    ctxt->xpathCtxt->node = ctxt->node;
    
    ctxt->xpathCtxt->namespaces = nsList;
    ctxt->xpathCtxt->nsNr = nsNr;
    res = xmlXPathCompiledEval(comp, ctxt->xpathCtxt);
    if (res != NULL) {
	if (res->type != XPATH_STRING)
	    res = xmlXPathConvertString(res);
	if (res->type == XPATH_STRING) {
            ret = res->stringval;
	    res->stringval = NULL;
	} else {
	    xsltTransformError(ctxt, NULL, NULL,
		 "xpath : string() function didn't return a String\n");
	}
	xmlXPathFreeObject(res);
    } else {
	ctxt->state = XSLT_STATE_STOPPED;
    }
#ifdef WITH_XSLT_DEBUG_TEMPLATES
    XSLT_TRACE(ctxt,XSLT_TRACE_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltEvalXPathString: returns %s\n", ret));
#endif
    ctxt->inst = oldInst;
    ctxt->node = oldNode;
    ctxt->xpathCtxt->contextSize = oldSize;
    ctxt->xpathCtxt->proximityPosition = oldPos;
    ctxt->xpathCtxt->nsNr = oldNsNr;
    ctxt->xpathCtxt->namespaces = oldNamespaces;
    return(ret);
}

xmlChar *
xsltEvalXPathString(xsltTransformContextPtr ctxt, xmlXPathCompExprPtr comp) {
    return(xsltEvalXPathStringNs(ctxt, comp, 0, NULL));
}

xmlChar *
xsltEvalTemplateString(xsltTransformContextPtr ctxt,
		       xmlNodePtr contextNode,
	               xmlNodePtr inst)
{
    xmlNodePtr oldInsert, insert = NULL;
    xmlChar *ret;

    if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL))
	return(NULL);

    if (inst->children == NULL)
	return(NULL);

    insert = xmlNewDocNode(ctxt->output, NULL,
	                   (const xmlChar *)"fake", NULL);
    if (insert == NULL) {
	xsltTransformError(ctxt, NULL, contextNode,
		"Failed to create temporary node\n");
	return(NULL);
    }
    oldInsert = ctxt->insert;
    ctxt->insert = insert;
    xsltApplyOneTemplate(ctxt, contextNode, inst->children, NULL, NULL);

    ctxt->insert = oldInsert;

    ret = xmlNodeGetContent(insert);
    if (insert != NULL)
	xmlFreeNode(insert);
    return(ret);
}

xmlChar *
xsltAttrTemplateValueProcessNode(xsltTransformContextPtr ctxt,
	  const xmlChar *str, xmlNodePtr inst)
{
    xmlChar *ret = NULL;
    const xmlChar *cur;
    xmlChar *expr, *val;
    xmlNsPtr *nsList = NULL;
    int nsNr = 0;

    if (str == NULL) return(NULL);
    if (*str == 0)
	return(xmlStrndup((xmlChar *)"", 0));

    cur = str;
    while (*cur != 0) {
	if (*cur == '{') {
	    if (*(cur+1) == '{') {	
	        cur++;
		ret = xmlStrncat(ret, str, cur - str);
		cur++;
		str = cur;
		continue;
	    }
	    ret = xmlStrncat(ret, str, cur - str);
	    str = cur;
	    cur++;
	    while ((*cur != 0) && (*cur != '}')) cur++;
	    if (*cur == 0) {
	        xsltTransformError(ctxt, NULL, inst,
			"xsltAttrTemplateValueProcessNode: unmatched '{'\n");
		ret = xmlStrncat(ret, str, cur - str);
		return(ret);
	    }
	    str++;
	    expr = xmlStrndup(str, cur - str);
	    if (expr == NULL)
		return(ret);
	    else if (*expr == '{') {
		ret = xmlStrcat(ret, expr);
		xmlFree(expr);
	    } else {
		xmlXPathCompExprPtr comp;
		if ((nsList == NULL) && (inst != NULL)) {
		    int i = 0;

		    nsList = xmlGetNsList(inst->doc, inst);
		    if (nsList != NULL) {
			while (nsList[i] != NULL)
			    i++;
			nsNr = i;
		    }
		}
		comp = xmlXPathCompile(expr);
                val = xsltEvalXPathStringNs(ctxt, comp, nsNr, nsList);
		xmlXPathFreeCompExpr(comp);
		xmlFree(expr);
		if (val != NULL) {
		    ret = xmlStrcat(ret, val);
		    xmlFree(val);
		}
	    }
	    cur++;
	    str = cur;
	} else if (*cur == '}') {
	    cur++;
	    if (*cur == '}') {	
		ret = xmlStrncat(ret, str, cur - str);
		cur++;
		str = cur;
		continue;
	    } else {
	        xsltTransformError(ctxt, NULL, inst,
		     "xsltAttrTemplateValueProcessNode: unmatched '}'\n");
	    }
	} else
	    cur++;
    }
    if (cur != str) {
	ret = xmlStrncat(ret, str, cur - str);
    }

    if (nsList != NULL)
	xmlFree(nsList);

    return(ret);
}

xmlChar *
xsltAttrTemplateValueProcess(xsltTransformContextPtr ctxt, const xmlChar *str) {
    return(xsltAttrTemplateValueProcessNode(ctxt, str, NULL));
}

xmlChar *
xsltEvalAttrValueTemplate(xsltTransformContextPtr ctxt, xmlNodePtr inst,
	                  const xmlChar *name, const xmlChar *ns)
{
    xmlChar *ret;
    xmlChar *expr;

    if ((ctxt == NULL) || (inst == NULL) || (name == NULL))
	return(NULL);

    expr = xsltGetNsProp(inst, name, ns);
    if (expr == NULL)
	return(NULL);


    ret = xsltAttrTemplateValueProcessNode(ctxt, expr, inst);
#ifdef WITH_XSLT_DEBUG_TEMPLATES
    XSLT_TRACE(ctxt,XSLT_TRACE_TEMPLATES,xsltGenericDebug(xsltGenericDebugContext,
	 "xsltEvalAttrValueTemplate: %s returns %s\n", expr, ret));
#endif
    if (expr != NULL)
	xmlFree(expr);
    return(ret);
}

const xmlChar *
xsltEvalStaticAttrValueTemplate(xsltStylesheetPtr style, xmlNodePtr inst,
			const xmlChar *name, const xmlChar *ns, int *found) {
    const xmlChar *ret;
    xmlChar *expr;

    if ((style == NULL) || (inst == NULL) || (name == NULL))
	return(NULL);

    expr = xsltGetNsProp(inst, name, ns);
    if (expr == NULL) {
	*found = 0;
	return(NULL);
    }
    *found = 1;

    ret = xmlStrchr(expr, '{');
    if (ret != NULL) {
	xmlFree(expr);
	return(NULL);
    }
    ret = xmlDictLookup(style->dict, expr, -1);
    xmlFree(expr);
    return(ret);
}

xmlAttrPtr
xsltAttrTemplateProcess(xsltTransformContextPtr ctxt, xmlNodePtr target,
	                xmlAttrPtr attr)
{
    const xmlChar *value;
    xmlAttrPtr ret;

    if ((ctxt == NULL) || (attr == NULL) || (target == NULL))
	return(NULL);
    
    if (attr->type != XML_ATTRIBUTE_NODE)
	return(NULL);

#ifdef XSLT_REFACTORED    
    if (attr->psvi == xsltXSLTAttrMarker)
	return(NULL);
#else
    if ((attr->ns != NULL) && xmlStrEqual(attr->ns->href, XSLT_NAMESPACE))
	return(NULL);
#endif
    if (attr->children != NULL) {
	if ((attr->children->type != XML_TEXT_NODE) ||
	    (attr->children->next != NULL))
	{
	    xsltTransformError(ctxt, NULL, attr->parent,
		"Internal error: The children of an attribute node of a "
		"literal result element are not in the expected form.\n");
	    return(NULL);
	}
	value = attr->children->content;
	if (value == NULL)
	    value = xmlDictLookup(ctxt->dict, BAD_CAST "", 0);
    } else
	value = xmlDictLookup(ctxt->dict, BAD_CAST "", 0);
    ret = target->properties;
    while (ret != NULL) {
        if (((attr->ns != NULL) == (ret->ns != NULL)) &&
	    xmlStrEqual(ret->name, attr->name) &&
	    ((attr->ns == NULL) || xmlStrEqual(ret->ns->href, attr->ns->href)))
	{
	    break;
	}
        ret = ret->next;
    }
    if (ret != NULL) {	
        
	xmlFreeNodeList(ret->children);
	ret->children = ret->last = NULL;
	if ((ret->ns != NULL) &&
	    (! xmlStrEqual(ret->ns->prefix, attr->ns->prefix)))
	{
	    ret->ns = xsltGetNamespace(ctxt, attr->parent, attr->ns, target);
	}
    } else {
        
	if (attr->ns != NULL)
	    ret = xmlNewNsProp(target,
		xsltGetNamespace(ctxt, attr->parent, attr->ns, target),
		    attr->name, NULL);
	else
	    ret = xmlNewNsProp(target, NULL, attr->name, NULL);	
    }
    if (ret != NULL) {
        xmlNodePtr text;

        text = xmlNewText(NULL);
	if (text != NULL) {
	    ret->last = ret->children = text;
	    text->parent = (xmlNodePtr) ret;
	    text->doc = ret->doc;

	    if (attr->psvi != NULL) {
		xmlChar *val;
		val = xsltEvalAVT(ctxt, attr->psvi, attr->parent);
		if (val == NULL) {
		    if (attr->ns) {
			xsltTransformError(ctxt, NULL, attr->parent,
			    "Internal error: Failed to evaluate the AVT "
			    "of attribute '{%s}%s'.\n",
			    attr->ns->href, attr->name);
		    } else {
			xsltTransformError(ctxt, NULL, attr->parent,
			    "Internal error: Failed to evaluate the AVT "
			    "of attribute '%s'.\n",
			    attr->name);
		    }
		    text->content = xmlStrdup(BAD_CAST "");
		} else {
		    text->content = val;
		}
	    } else if ((ctxt->internalized) && (target != NULL) &&
	               (target->doc != NULL) &&
		       (target->doc->dict == ctxt->dict) &&
		       xmlDictOwns(ctxt->dict, value)) {
		text->content = (xmlChar *) value;
	    } else {
		text->content = xmlStrdup(value);
	    }
	}
    } else {
	if (attr->ns) {
	    xsltTransformError(ctxt, NULL, attr->parent,
	    	"Internal error: Failed to create attribute '{%s}%s'.\n",
		attr->ns->href, attr->name);
	} else {
	    xsltTransformError(ctxt, NULL, attr->parent,
	    	"Internal error: Failed to create attribute '%s'.\n",
		attr->name);
	}
    }
    return(ret);
}


xmlAttrPtr
xsltAttrListTemplateProcess(xsltTransformContextPtr ctxt, 
	                    xmlNodePtr target, xmlAttrPtr attrs)
{
    xmlAttrPtr attr, copy, last;
    xmlNodePtr oldInsert, text;
    xmlNsPtr origNs = NULL, copyNs = NULL;
    const xmlChar *value;
    xmlChar *valueAVT;

    if ((ctxt == NULL) || (target == NULL) || (attrs == NULL))
	return(NULL);

    oldInsert = ctxt->insert;
    ctxt->insert = target;        

    if (target->properties) {
	last = target->properties;
	while (last->next != NULL)
	    last = last->next;
    } else {
	last = NULL;
    }
    attr = attrs;
    do {
#ifdef XSLT_REFACTORED
	if (attr->psvi == xsltXSLTAttrMarker) {
	    goto next_attribute;
	}
#else
	if ((attr->ns != NULL) &&
	    xmlStrEqual(attr->ns->href, XSLT_NAMESPACE))
	{
	    goto next_attribute;
	}
#endif
	if (attr->children != NULL) {
	    if ((attr->children->type != XML_TEXT_NODE) ||
		(attr->children->next != NULL))
	    {
		xsltTransformError(ctxt, NULL, attr->parent,
		    "Internal error: The children of an attribute node of a "
		    "literal result element are not in the expected form.\n");
		goto error;
	    }
	    value = attr->children->content;
	    if (value == NULL)
		value = xmlDictLookup(ctxt->dict, BAD_CAST "", 0);
	} else
	    value = xmlDictLookup(ctxt->dict, BAD_CAST "", 0);

	copy = xmlNewDocProp(target->doc, attr->name, NULL);
	if (copy == NULL) {
	    if (attr->ns) {
		xsltTransformError(ctxt, NULL, attr->parent,
		    "Internal error: Failed to create attribute '{%s}%s'.\n",
		    attr->ns->href, attr->name);
	    } else {
		xsltTransformError(ctxt, NULL, attr->parent,
		    "Internal error: Failed to create attribute '%s'.\n",
		    attr->name);
	    }
	    goto error;
	}
	copy->parent = target;
	if (last == NULL) {
	    target->properties = copy;
	    last = copy;
	} else {
	    last->next = copy;
	    copy->prev = last;
	    last = copy;
	}
	if (attr->ns != origNs) {
	    origNs = attr->ns;
	    if (attr->ns != NULL) {
#ifdef XSLT_REFACTORED
		copyNs = xsltGetSpecialNamespace(ctxt, attr->parent,
		    attr->ns->href, attr->ns->prefix, target);
#else
		copyNs = xsltGetNamespace(ctxt, attr->parent,
		    attr->ns, target);
#endif
		if (copyNs == NULL)
		    goto error;
	    } else
		copyNs = NULL;
	}
	copy->ns = copyNs;

	text = xmlNewText(NULL);
	if (text != NULL) {
	    copy->last = copy->children = text;
	    text->parent = (xmlNodePtr) copy;
	    text->doc = copy->doc;

	    if (attr->psvi != NULL) {
		valueAVT = xsltEvalAVT(ctxt, attr->psvi, attr->parent);
		if (valueAVT == NULL) {
		    if (attr->ns) {
			xsltTransformError(ctxt, NULL, attr->parent,
			    "Internal error: Failed to evaluate the AVT "
			    "of attribute '{%s}%s'.\n",
			    attr->ns->href, attr->name);
		    } else {
			xsltTransformError(ctxt, NULL, attr->parent,
			    "Internal error: Failed to evaluate the AVT "
			    "of attribute '%s'.\n",
			    attr->name);
		    }
		    text->content = xmlStrdup(BAD_CAST "");
		    goto error;
		} else {
		    text->content = valueAVT;
		}
	    } else if ((ctxt->internalized) &&
		(target->doc != NULL) &&
		(target->doc->dict == ctxt->dict) &&
		xmlDictOwns(ctxt->dict, value))
	    {
		text->content = (xmlChar *) value;
	    } else {
		text->content = xmlStrdup(value);
	    }
            if ((copy != NULL) && (text != NULL) &&
                (xmlIsID(copy->doc, copy->parent, copy)))
                xmlAddID(NULL, copy->doc, text->content, copy);
	}

next_attribute:
	attr = attr->next;
    } while (attr != NULL);

    attr = attrs;
    do {
#ifdef XSLT_REFACTORED
	if ((attr->psvi == xsltXSLTAttrMarker) &&
	    xmlStrEqual(attr->name, (const xmlChar *)"use-attribute-sets"))
	{
	    xsltApplyAttributeSet(ctxt, ctxt->node, (xmlNodePtr) attr, NULL);
	}
#else
	if ((attr->ns != NULL) &&
	    xmlStrEqual(attr->name, (const xmlChar *)"use-attribute-sets") &&
	    xmlStrEqual(attr->ns->href, XSLT_NAMESPACE))
	{
	    xsltApplyAttributeSet(ctxt, ctxt->node, (xmlNodePtr) attr, NULL);
	}
#endif
	attr = attr->next;
    } while (attr != NULL);

    ctxt->insert = oldInsert;
    return(target->properties);

error:
    ctxt->insert = oldInsert;
    return(NULL);
}


xmlNodePtr *
xsltTemplateProcess(xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED, xmlNodePtr node) {
    if (node == NULL)
	return(NULL);
    
    return(0);
}


