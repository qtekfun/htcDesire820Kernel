/*
 * namespaces.c: Implementation of the XSLT namespaces handling
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

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_MATH_H
#include <math.h>
#endif
#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
#ifdef HAVE_NAN_H
#include <nan.h>
#endif
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifndef	XSLT_NEED_TRIO
#include <stdio.h>
#else
#include <trio.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/uri.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "namespaces.h"
#include "imports.h"


#ifdef XSLT_REFACTORED  
static xsltNsAliasPtr
xsltNewNsAlias(xsltCompilerCtxtPtr cctxt)
{
    xsltNsAliasPtr ret;

    if (cctxt == NULL)
	return(NULL);

    ret = (xsltNsAliasPtr) xmlMalloc(sizeof(xsltNsAlias));
    if (ret == NULL) {
	xsltTransformError(NULL, cctxt->style, NULL,
	    "Internal error in xsltNewNsAlias(): Memory allocation failed.\n");
	cctxt->style->errors++;
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltNsAlias));    
    ret->next = cctxt->nsAliases;
    cctxt->nsAliases = ret;       

    return(ret);
}
#endif 
void
xsltNamespaceAlias(xsltStylesheetPtr style, xmlNodePtr node)
{
    xmlChar *resultPrefix = NULL;
    xmlChar *stylePrefix = NULL;
    xmlNsPtr literalNs = NULL;
    xmlNsPtr targetNs = NULL;
 
#ifdef XSLT_REFACTORED 
    xsltNsAliasPtr alias;

    if ((style == NULL) || (node == NULL))
	return;

    stylePrefix = xmlGetNsProp(node, (const xmlChar *)"stylesheet-prefix", NULL);
    if (stylePrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "The attribute 'stylesheet-prefix' is missing.\n");
	return;
    }
    if (xmlStrEqual(stylePrefix, (const xmlChar *)"#default"))
	literalNs = xmlSearchNs(node->doc, node, NULL);	
    else {
	literalNs = xmlSearchNs(node->doc, node, stylePrefix);
	if (literalNs == NULL) {
	    xsltTransformError(NULL, style, node,
	        "Attribute 'stylesheet-prefix': There's no namespace "
		"declaration in scope for the prefix '%s'.\n",
		    stylePrefix);
	    goto error;
	}
    }
    resultPrefix = xmlGetNsProp(node, (const xmlChar *)"result-prefix", NULL);
    if (resultPrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "The attribute 'result-prefix' is missing.\n");
	goto error;
    }        
    if (xmlStrEqual(resultPrefix, (const xmlChar *)"#default"))
	targetNs = xmlSearchNs(node->doc, node, NULL);
    else {
	targetNs = xmlSearchNs(node->doc, node, resultPrefix);

        if (targetNs == NULL) {
	   xsltTransformError(NULL, style, node,
	        "Attribute 'result-prefix': There's no namespace "
		"declaration in scope for the prefix '%s'.\n",
		    stylePrefix);
	    goto error;
	}
    }
    
	
    alias = xsltNewNsAlias(XSLT_CCTXT(style));
    if (alias == NULL)
	return;
    alias->literalNs = literalNs;
    alias->targetNs = targetNs;
    XSLT_CCTXT(style)->hasNsAliases = 1;


#else 
    const xmlChar *literalNsName;
    const xmlChar *targetNsName;
    

    if ((style == NULL) || (node == NULL))
	return;

    stylePrefix = xmlGetNsProp(node, (const xmlChar *)"stylesheet-prefix", NULL);
    if (stylePrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "namespace-alias: stylesheet-prefix attribute missing\n");
	return;
    }
    resultPrefix = xmlGetNsProp(node, (const xmlChar *)"result-prefix", NULL);
    if (resultPrefix == NULL) {
	xsltTransformError(NULL, style, node,
	    "namespace-alias: result-prefix attribute missing\n");
	goto error;
    }
    
    if (xmlStrEqual(stylePrefix, (const xmlChar *)"#default")) {
	literalNs = xmlSearchNs(node->doc, node, NULL);
	if (literalNs == NULL) {
	    literalNsName = NULL;
	} else
	    literalNsName = literalNs->href; 
    } else {
	literalNs = xmlSearchNs(node->doc, node, stylePrefix);
 
	if ((literalNs == NULL) || (literalNs->href == NULL)) {
	    xsltTransformError(NULL, style, node,
	        "namespace-alias: prefix %s not bound to any namespace\n",
					stylePrefix);
	    goto error;
	} else
	    literalNsName = literalNs->href;
    }

    if (xmlStrEqual(resultPrefix, (const xmlChar *)"#default")) {
	targetNs = xmlSearchNs(node->doc, node, NULL);
	if (targetNs == NULL) {
	    targetNsName = UNDEFINED_DEFAULT_NS;
	} else
	    targetNsName = targetNs->href;
    } else {
	targetNs = xmlSearchNs(node->doc, node, resultPrefix);

        if ((targetNs == NULL) || (targetNs->href == NULL)) {
	    xsltTransformError(NULL, style, node,
	        "namespace-alias: prefix %s not bound to any namespace\n",
					resultPrefix);
	    goto error;
	} else
	    targetNsName = targetNs->href;
    }
   
    if (literalNsName == NULL) {
        if (targetNs != NULL) {
            style->defaultAlias = targetNs->href;
	}
    } else {
        if (style->nsAliases == NULL)
	    style->nsAliases = xmlHashCreate(10);
        if (style->nsAliases == NULL) {
	    xsltTransformError(NULL, style, node,
	        "namespace-alias: cannot create hash table\n");
	    goto error;
        }
	xmlHashAddEntry((xmlHashTablePtr) style->nsAliases,
	    literalNsName, (void *) targetNsName);
    }
#endif 

error:
    if (stylePrefix != NULL)
	xmlFree(stylePrefix);
    if (resultPrefix != NULL)
	xmlFree(resultPrefix);
}

xmlNsPtr
xsltGetSpecialNamespace(xsltTransformContextPtr ctxt, xmlNodePtr invocNode,
		const xmlChar *nsName, const xmlChar *nsPrefix,
		xmlNodePtr target)
{
    xmlNsPtr ns;
    int prefixOccupied = 0;

    if ((ctxt == NULL) || (target == NULL) ||
	(target->type != XML_ELEMENT_NODE))
	return(NULL);

    
    if ((nsPrefix == NULL) && ((nsName == NULL) || (nsName[0] == 0))) {
	if (target->nsDef != NULL) {
	    ns = target->nsDef;
	    do {
		if (ns->prefix == NULL) {
		    if ((ns->href != NULL) && (ns->href[0] != 0)) {
			xsltTransformError(ctxt, NULL, invocNode,
			    "Namespace normalization error: Cannot undeclare "
			    "the default namespace, since the default namespace "
			    "'%s' is already declared on the result element "
			    "'%s'.\n", ns->href, target->name);
			return(NULL);
		    } else {
			return(NULL);
		    }
		    break;
		}
		ns = ns->next;
	    } while (ns != NULL);
	}	
	if ((target->parent != NULL) &&
	    (target->parent->type == XML_ELEMENT_NODE))
	{
	    if (target->parent->ns == NULL)
		return(NULL);
	    
	    ns = xmlSearchNs(target->doc, target->parent,
		NULL);
	    if ((ns == NULL) || (ns->href == NULL) || (ns->href[0] == 0))
		return(NULL);
	    
	    xmlNewNs(target, BAD_CAST "", NULL);
	    	
	    return(NULL);
	}
	return(NULL);
    }
    if ((nsPrefix != NULL) &&
	(nsPrefix[0] == 'x') && (nsPrefix[1] == 'm') &&
	(nsPrefix[2] == 'l') && (nsPrefix[3] == 0))
    {
	return(xmlSearchNs(target->doc, target, nsPrefix));
    }
    if (target->nsDef != NULL) {
	ns = target->nsDef;
	do {
	    if ((ns->prefix == NULL) == (nsPrefix == NULL)) {
		if (ns->prefix == nsPrefix) {
		    if (xmlStrEqual(ns->href, nsName))
			return(ns);
		    prefixOccupied = 1;
		    break;
		} else if (xmlStrEqual(ns->prefix, nsPrefix)) {
		    if (xmlStrEqual(ns->href, nsName))
			return(ns);
		    prefixOccupied = 1;
		    break;
		}
	    }
	    ns = ns->next;
	} while (ns != NULL);
    }
    if (prefixOccupied) {
	ns = xmlSearchNsByHref(target->doc, target, nsName);
	if (ns != NULL)
	    return(ns);

    
    } else if ((target->parent != NULL) &&
	(target->parent->type == XML_ELEMENT_NODE))
    {
     
	if ((target->parent->ns != NULL) &&
	    ((target->parent->ns->prefix != NULL) == (nsPrefix != NULL)))
	{
	    ns = target->parent->ns;
	    
	    if (nsPrefix == NULL) {
		if (xmlStrEqual(ns->href, nsName))
		    return(ns);
	    } else if (xmlStrEqual(ns->prefix, nsPrefix) &&
		xmlStrEqual(ns->href, nsName))
	    {
		return(ns);
	    }
	}
    
	ns = xmlSearchNs(target->doc, target->parent, nsPrefix);
	if (ns != NULL) {
	    if (xmlStrEqual(ns->href, nsName))
		return(ns);	    
	    if (target->properties) {
		xmlAttrPtr attr = target->properties;
		do {
		    if ((attr->ns) &&
			xmlStrEqual(attr->ns->prefix, nsPrefix))
		    {
			ns = xmlSearchNsByHref(target->doc, target, nsName);
			if (ns != NULL)
			    return(ns);
			goto declare_new_prefix;
		    }
		    attr = attr->next;
		} while (attr != NULL);
	    }
	} else {
#if 0
	    ns = xmlSearchNsByHref(target->doc, target, nsName);
	    if (ns != NULL)
		return(ns);
#endif
	}
	ns = xmlNewNs(target, nsName, nsPrefix);
	
	return(ns);
    } else {
	ns = xmlNewNs(target, nsName, nsPrefix);
	
	return(ns);
    }

declare_new_prefix:
    {
	xmlChar pref[30];
	int counter = 1;

	if (nsPrefix == NULL) {
	    nsPrefix = "ns";
	}

	do {
	    snprintf((char *) pref, 30, "%s_%d", nsPrefix, counter++);
	    ns = xmlSearchNs(target->doc, target, BAD_CAST pref);
	    if (counter > 1000) {
		xsltTransformError(ctxt, NULL, invocNode,
		    "Internal error in xsltAcquireResultInScopeNs(): "
		    "Failed to compute a unique ns-prefix for the "
		    "generated element");
		return(NULL);
	    }
	} while (ns != NULL);
	ns = xmlNewNs(target, nsName, BAD_CAST pref);
	
	return(ns);
    }
    return(NULL);
}

xmlNsPtr
xsltGetNamespace(xsltTransformContextPtr ctxt, xmlNodePtr cur, xmlNsPtr ns,
	         xmlNodePtr out)
{    
    
    if (ns == NULL)
	return(NULL);

#ifdef XSLT_REFACTORED
    return(xsltGetSpecialNamespace(ctxt, cur, ns->href, ns->prefix, out));
#else
    {
	xsltStylesheetPtr style;
	const xmlChar *URI = NULL; 

	if ((ctxt == NULL) || (cur == NULL) || (out == NULL))
	    return(NULL);

	style = ctxt->style;
	while (style != NULL) {
	    if (style->nsAliases != NULL)
		URI = (const xmlChar *) 
		xmlHashLookup(style->nsAliases, ns->href);
	    if (URI != NULL)
		break;
	    
	    style = xsltNextImport(style);
	}
	
	
	if (URI == UNDEFINED_DEFAULT_NS) {
	    return(xsltGetSpecialNamespace(ctxt, cur, NULL, NULL, out));
#if 0
	    xmlNsPtr dflt;	    
	    dflt = xmlSearchNs(cur->doc, cur, NULL);
	    if (dflt != NULL)
		URI = dflt->href;
	    else
		return NULL;
#endif
	} else if (URI == NULL)
	    URI = ns->href;

	return(xsltGetSpecialNamespace(ctxt, cur, URI, ns->prefix, out));
    }
#endif
}

xmlNsPtr
xsltGetPlainNamespace(xsltTransformContextPtr ctxt, xmlNodePtr cur,
                      xmlNsPtr ns, xmlNodePtr out)
{    
    return(xsltGetNamespace(ctxt, cur, ns, out));
}

xmlNsPtr
xsltCopyNamespaceList(xsltTransformContextPtr ctxt, xmlNodePtr node,
	              xmlNsPtr cur) {
    xmlNsPtr ret = NULL, tmp;
    xmlNsPtr p = NULL,q;    

    if (cur == NULL)
	return(NULL);
    if (cur->type != XML_NAMESPACE_DECL)
	return(NULL);

    if ((node != NULL) && (node->type != XML_ELEMENT_NODE))
	node = NULL;

    while (cur != NULL) {
	if (cur->type != XML_NAMESPACE_DECL)
	    break;

	if (node != NULL) {
	    if ((node->ns != NULL) &&
		(xmlStrEqual(node->ns->prefix, cur->prefix)) &&
        	(xmlStrEqual(node->ns->href, cur->href))) {
		cur = cur->next;
		continue;
	    }
	    tmp = xmlSearchNs(node->doc, node, cur->prefix);
	    if ((tmp != NULL) && (xmlStrEqual(tmp->href, cur->href))) {
		cur = cur->next;
		continue;
	    }
	}
#ifdef XSLT_REFACTORED
	q = xmlNewNs(node, cur->href, cur->prefix);
	if (p == NULL) {
	    ret = p = q;
	} else {
	    p->next = q;
	    p = q;
	}
#else
	if (!xmlStrEqual(cur->href, XSLT_NAMESPACE)) {
	    const xmlChar *URI;
	    
	    URI = (const xmlChar *) xmlHashLookup(ctxt->style->nsAliases,
		                                  cur->href);
	    if (URI == UNDEFINED_DEFAULT_NS)
	        continue;
	    if (URI != NULL) {
		q = xmlNewNs(node, URI, cur->prefix);
	    } else {
		q = xmlNewNs(node, cur->href, cur->prefix);
	    }
	    if (p == NULL) {
		ret = p = q;
	    } else {
		p->next = q;
		p = q;
	    }
	}
#endif
	cur = cur->next;
    }
    return(ret);
}

xmlNsPtr
xsltCopyNamespace(xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		  xmlNodePtr elem, xmlNsPtr ns)
{    
    if ((ns == NULL) || (ns->type != XML_NAMESPACE_DECL))
	return(NULL);
    if ((elem != NULL) && (elem->type != XML_ELEMENT_NODE))
	return(xmlNewNs(NULL, ns->href, ns->prefix));
    else
	return(xmlNewNs(elem, ns->href, ns->prefix));
}


void
xsltFreeNamespaceAliasHashes(xsltStylesheetPtr style) {
    if (style->nsAliases != NULL)
	xmlHashFree((xmlHashTablePtr) style->nsAliases, NULL);
    style->nsAliases = NULL;
}
