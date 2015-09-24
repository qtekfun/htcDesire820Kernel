/*
 * attributes.c: Implementation of the XSLT attributes handling
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

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/uri.h>
#include <libxml/parserInternals.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "attributes.h"
#include "namespaces.h"
#include "templates.h"
#include "imports.h"
#include "transform.h"
#include "preproc.h"

#define WITH_XSLT_DEBUG_ATTRIBUTES
#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_ATTRIBUTES
#endif


#ifdef IS_BLANK
#undef IS_BLANK
#endif

#define IS_BLANK(c) (((c) == 0x20) || ((c) == 0x09) || ((c) == 0xA) ||	\
                     ((c) == 0x0D))

#define IS_BLANK_NODE(n)						\
    (((n)->type == XML_TEXT_NODE) && (xsltIsBlank((n)->content)))




typedef struct _xsltAttrElem xsltAttrElem;
typedef xsltAttrElem *xsltAttrElemPtr;
struct _xsltAttrElem {
    struct _xsltAttrElem *next;
    xmlNodePtr attr;	
    const xmlChar *set; 
    const xmlChar *ns;  
};


static xsltAttrElemPtr
xsltNewAttrElem(xmlNodePtr attr) {
    xsltAttrElemPtr cur;

    cur = (xsltAttrElemPtr) xmlMalloc(sizeof(xsltAttrElem));
    if (cur == NULL) {
        xsltGenericError(xsltGenericErrorContext,
		"xsltNewAttrElem : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltAttrElem));
    cur->attr = attr;
    return(cur);
}

static void
xsltFreeAttrElem(xsltAttrElemPtr attr) {
    xmlFree(attr);
}

static void
xsltFreeAttrElemList(xsltAttrElemPtr list) {
    xsltAttrElemPtr next;
    
    while (list != NULL) {
	next = list->next;
	xsltFreeAttrElem(list);
	list = next;
    }
}

#ifdef XSLT_REFACTORED
#else
static xsltAttrElemPtr
xsltAddAttrElemList(xsltAttrElemPtr list, xmlNodePtr attr) {
    xsltAttrElemPtr next, cur;

    if (attr == NULL)
	return(list);
    if (list == NULL)
	return(xsltNewAttrElem(attr));
    cur = list;
    while (cur != NULL) {	
	next = cur->next;
	if (cur->attr == attr)
	    return(cur);
	if (cur->next == NULL) {
	    cur->next = xsltNewAttrElem(attr);
	    return(list);
	}
	cur = next;
    }
    return(list);
}
#endif 

static xsltAttrElemPtr
xsltMergeAttrElemList(xsltStylesheetPtr style,
		      xsltAttrElemPtr list, xsltAttrElemPtr old) {
    xsltAttrElemPtr cur;
    int add;

    while (old != NULL) {
	if ((old->attr == NULL) && (old->set == NULL)) {
	    old = old->next;
	    continue;
	}
	cur = list;
	add = 1;
	while (cur != NULL) {
	    if ((cur->attr == NULL) && (cur->set == NULL)) {
		if (cur->next == NULL)
		    break;
		cur = cur->next;
		continue;
	    }
	    if ((cur->set != NULL) && (cur->set == old->set)) {
		add = 0;
		break;
	    }
	    if (cur->set != NULL) {
		if (cur->next == NULL)
		    break;
		cur = cur->next;
		continue;
	    }
	    if (old->set != NULL) {
		if (cur->next == NULL)
		    break;
		cur = cur->next;
		continue;
	    }
	    if (cur->attr == old->attr) {
		xsltGenericError(xsltGenericErrorContext,
	     "xsl:attribute-set : use-attribute-sets recursion detected\n");
		return(list);
	    }
	    if (cur->next == NULL)
		break;
            cur = cur->next;
	}

	if (add == 1) {
	    if (cur == NULL) {
		list = xsltNewAttrElem(old->attr);
		if (old->set != NULL) {
		    list->set = xmlDictLookup(style->dict, old->set, -1);
		    if (old->ns != NULL)
			list->ns = xmlDictLookup(style->dict, old->ns, -1);
		}
	    } else if (add) {
		cur->next = xsltNewAttrElem(old->attr);
		if (old->set != NULL) {
		    cur->next->set = xmlDictLookup(style->dict, old->set, -1);
		    if (old->ns != NULL)
			cur->next->ns = xmlDictLookup(style->dict, old->ns, -1);
		}
	    }
	}

	old = old->next;
    }
    return(list);
}



void
xsltParseStylesheetAttributeSet(xsltStylesheetPtr style, xmlNodePtr cur) {
    const xmlChar *ncname;
    const xmlChar *prefix;
    xmlChar *value;
    xmlNodePtr child;
    xsltAttrElemPtr attrItems;

    if ((cur == NULL) || (style == NULL))
	return;

    value = xmlGetNsProp(cur, (const xmlChar *)"name", NULL);
    if (value == NULL) {
	xsltGenericError(xsltGenericErrorContext,
	     "xsl:attribute-set : name is missing\n");
	return;
    }

    ncname = xsltSplitQName(style->dict, value, &prefix);
    xmlFree(value);
    value = NULL;

    if (style->attributeSets == NULL) {
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
	xsltGenericDebug(xsltGenericDebugContext,
	    "creating attribute set table\n");
#endif
	style->attributeSets = xmlHashCreate(10);
    }
    if (style->attributeSets == NULL)
	return;

    attrItems = xmlHashLookup2(style->attributeSets, ncname, prefix);

    child = cur->children;
    while (child != NULL) {
	if ((child->type != XML_ELEMENT_NODE) ||
	    (child->ns == NULL) ||
	    (! IS_XSLT_ELEM(child)))
	{
	    if (child->type == XML_ELEMENT_NODE)
		xsltTransformError(NULL, style, child,
			"xsl:attribute-set : unexpected child %s\n",
		                 child->name);
	    else
		xsltTransformError(NULL, style, child,
			"xsl:attribute-set : child of unexpected type\n");
	} else if (!IS_XSLT_NAME(child, "attribute")) {
	    xsltTransformError(NULL, style, child,
		"xsl:attribute-set : unexpected child xsl:%s\n",
		child->name);
	} else {
#ifdef XSLT_REFACTORED
	    xsltAttrElemPtr nextAttr, curAttr;


#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
	    xsltGenericDebug(xsltGenericDebugContext,
		"add attribute to list %s\n", ncname);
#endif
	    if (attrItems == NULL) {
		attrItems = xsltNewAttrElem(child);
	    } else {
		curAttr = attrItems;
		while (curAttr != NULL) {
		    nextAttr = curAttr->next;
		    if (curAttr->attr == child) {
			attrItems = curAttr;
			goto next_child;
		    }
		    if (curAttr->next == NULL)			
			curAttr->next = xsltNewAttrElem(child);
		    curAttr = nextAttr;
		}
	    }
	    xsltParseAnyXSLTElem(XSLT_CCTXT(style), child);
#else
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
	    xsltGenericDebug(xsltGenericDebugContext,
		"add attribute to list %s\n", ncname);
#endif
	    attrItems = xsltAddAttrElemList(attrItems, child);
#endif
	}

#ifdef XSLT_REFACTORED
next_child:
#endif
	child = child->next;
    }

        
    value = xmlGetNsProp(cur, (const xmlChar *)"use-attribute-sets",
	NULL);
    if (value != NULL) {
	const xmlChar *curval, *endval;
	curval = value;
	while (*curval != 0) {
	    while (IS_BLANK(*curval)) curval++;
	    if (*curval == 0)
		break;
	    endval = curval;
	    while ((*endval != 0) && (!IS_BLANK(*endval))) endval++;
	    curval = xmlDictLookup(style->dict, curval, endval - curval);
	    if (curval) {
		const xmlChar *ncname2 = NULL;
		const xmlChar *prefix2 = NULL;
		xsltAttrElemPtr refAttrItems;
		
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
		xsltGenericDebug(xsltGenericDebugContext,
		    "xsl:attribute-set : %s adds use %s\n", ncname, curval);
#endif
		ncname2 = xsltSplitQName(style->dict, curval, &prefix2);
		refAttrItems = xsltNewAttrElem(NULL);
		if (refAttrItems != NULL) {
		    refAttrItems->set = ncname2;
		    refAttrItems->ns = prefix2;
		    attrItems = xsltMergeAttrElemList(style,
			attrItems, refAttrItems);
		    xsltFreeAttrElem(refAttrItems);
		}
	    }
	    curval = endval;
	}
	xmlFree(value);
	value = NULL;
    }

    if (attrItems == NULL)
	attrItems = xsltNewAttrElem(NULL);
    xmlHashUpdateEntry2(style->attributeSets, ncname, prefix, attrItems, NULL);
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
    xsltGenericDebug(xsltGenericDebugContext,
	"updated attribute list %s\n", ncname);
#endif
}

static xsltAttrElemPtr
xsltGetSAS(xsltStylesheetPtr style, const xmlChar *name, const xmlChar *ns) {
    xsltAttrElemPtr values;

    while (style != NULL) {
	values = xmlHashLookup2(style->attributeSets, name, ns);
	if (values != NULL)
	    return(values);
	style = xsltNextImport(style);
    }
    return(NULL);
}

static void
xsltResolveSASCallback(xsltAttrElemPtr values, xsltStylesheetPtr style,
	               const xmlChar *name, const xmlChar *ns,
		       ATTRIBUTE_UNUSED const xmlChar *ignored) {
    xsltAttrElemPtr tmp;
    xsltAttrElemPtr refs;

    tmp = values;
    while (tmp != NULL) {
	if (tmp->set != NULL) {
	    if ((xmlStrEqual(name, tmp->set)) && (xmlStrEqual(ns, tmp->ns))) {
		xsltGenericError(xsltGenericErrorContext,
     "xsl:attribute-set : use-attribute-sets recursion detected on %s\n",
                                 name);
	    } else {
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
		xsltGenericDebug(xsltGenericDebugContext,
			"Importing attribute list %s\n", tmp->set);
#endif

		refs = xsltGetSAS(style, tmp->set, tmp->ns);
		if (refs == NULL) {
		    xsltGenericError(xsltGenericErrorContext,
     "xsl:attribute-set : use-attribute-sets %s reference missing %s\n",
				     name, tmp->set);
		} else {
		    xsltResolveSASCallback(refs, style, name, ns, NULL);
		    xsltMergeAttrElemList(style, values, refs);
		    tmp->set = NULL;
		    tmp->ns = NULL;
		}
	    }
	}
	tmp = tmp->next;
    }
}

static void
xsltMergeSASCallback(xsltAttrElemPtr values, xsltStylesheetPtr style,
	               const xmlChar *name, const xmlChar *ns,
		       ATTRIBUTE_UNUSED const xmlChar *ignored) {
    int ret;
    xsltAttrElemPtr topSet;

    ret = xmlHashAddEntry2(style->attributeSets, name, ns, values);
    if (ret < 0) {
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
	xsltGenericDebug(xsltGenericDebugContext,
		"attribute set %s present already in top stylesheet"
		" - merging\n", name);
#endif
	topSet = xmlHashLookup2(style->attributeSets, name, ns);
	if (topSet==NULL) {
	    xsltGenericError(xsltGenericErrorContext,
	        "xsl:attribute-set : logic error merging from imports for"
		" attribute-set %s\n", name);
	} else {
	    topSet = xsltMergeAttrElemList(style, topSet, values);
	    xmlHashUpdateEntry2(style->attributeSets, name, ns, topSet, NULL);
	}
	xsltFreeAttrElemList(values);
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
    } else {
	xsltGenericDebug(xsltGenericDebugContext,
		"attribute set %s moved to top stylesheet\n",
		         name);
#endif
    }
}

void
xsltResolveStylesheetAttributeSet(xsltStylesheetPtr style) {
    xsltStylesheetPtr cur;

#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
    xsltGenericDebug(xsltGenericDebugContext,
	    "Resolving attribute sets references\n");
#endif
    cur = xsltNextImport(style);
    while (cur != NULL) {
	if (cur->attributeSets != NULL) {
	    if (style->attributeSets == NULL) {
#ifdef WITH_XSLT_DEBUG_ATTRIBUTES
		xsltGenericDebug(xsltGenericDebugContext,
		    "creating attribute set table\n");
#endif
		style->attributeSets = xmlHashCreate(10);
	    }
	    xmlHashScanFull(cur->attributeSets, 
		(xmlHashScannerFull) xsltMergeSASCallback, style);
	    xmlHashFree(cur->attributeSets, NULL);
	    cur->attributeSets = NULL;
	}
	cur = xsltNextImport(cur);
    }

    if (style->attributeSets != NULL) {
	xmlHashScanFull(style->attributeSets, 
		(xmlHashScannerFull) xsltResolveSASCallback, style);
    }
}

static void
xsltAttributeInternal(xsltTransformContextPtr ctxt,
		      xmlNodePtr contextNode,
                      xmlNodePtr inst,
		      xsltStylePreCompPtr castedComp,
                      int fromAttributeSet)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemAttributePtr comp =
	(xsltStyleItemAttributePtr) castedComp;   
#else
    xsltStylePreCompPtr comp = castedComp;
#endif
    xmlNodePtr targetElem;
    xmlChar *prop = NULL;    
    const xmlChar *name = NULL, *prefix = NULL, *nsName = NULL;
    xmlChar *value = NULL;
    xmlNsPtr ns = NULL;
    xmlAttrPtr attr;    

    if ((ctxt == NULL) || (contextNode == NULL) || (inst == NULL))
        return;

    if (!comp->has_name)
        return;

    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltAttributeInternal(): "
	    "The XSLT 'attribute' instruction was not compiled.\n");
        return;
    }
    if (ctxt->insert == NULL)
        return;    
    targetElem = ctxt->insert;
    if (targetElem->type != XML_ELEMENT_NODE)
	return;
    
    if (targetElem->children != NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "xsl:attribute: Cannot add attributes to an "
	    "element if children have been already added "
	    "to the element.\n");
        return;
    }

    

#ifdef WITH_DEBUGGER
    if (ctxt->debugStatus != XSLT_DEBUG_NONE)
        xslHandleDebugger(inst, contextNode, NULL, ctxt);
#endif

    if (comp->name == NULL) {
	
        prop = xsltEvalAttrValueTemplate(ctxt, inst,
	    (const xmlChar *) "name", XSLT_NAMESPACE);
        if (prop == NULL) {
            xsltTransformError(ctxt, NULL, inst,
		"xsl:attribute: The attribute 'name' is missing.\n");
            goto error;
        }
	if (xmlValidateQName(prop, 0)) {
	    xsltTransformError(ctxt, NULL, inst,
		"xsl:attribute: The effective name '%s' is not a "
		"valid QName.\n", prop);
	    
	}
	name = xsltSplitQName(ctxt->dict, prop, &prefix);
	xmlFree(prop);

	if ((prefix != NULL) &&
	    (!xmlStrncasecmp(prefix, (xmlChar *) "xmlns", 5)))
	{
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"xsltAttribute: xmlns prefix forbidden\n");
#endif
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
    } else if (prefix != NULL) {
	
	ns = xmlSearchNs(inst->doc, inst, prefix);
	if (ns == NULL) {
	    xsltTransformError(ctxt, NULL, inst,
		"xsl:attribute: The QName '%s:%s' has no "
		"namespace binding in scope in the stylesheet; "
		"this is an error, since the namespace was not "
		"specified by the instruction itself.\n", prefix, name);
	} else
	    nsName = ns->href;	
    }

    if (fromAttributeSet) {
	attr = xmlHasNsProp(targetElem, name, nsName);
	if (attr != NULL)
	    return;
    }

    ns = NULL;
    
#if 0
    if (0) {	
	if (nsName != NULL) {
	    ns = xsltTreeAcquireStoredNs(some doc, nsName, prefix);
	}
    }
#endif

    if (nsName != NULL) {	
	if (prefix == NULL) {
	    xmlChar *pref = xmlStrdup(BAD_CAST "ns_1");

	    ns = xsltGetSpecialNamespace(ctxt, inst, nsName, BAD_CAST pref,
		targetElem);

	    xmlFree(pref);
	} else {
	    ns = xsltGetSpecialNamespace(ctxt, inst, nsName, prefix,
		targetElem);
	}
	if (ns == NULL) {
	    xsltTransformError(ctxt, NULL, inst,
		"Namespace fixup error: Failed to acquire an in-scope "
		"namespace binding for the generated attribute '{%s}%s'.\n",
		nsName, name);
	    goto error;
	}
    }
    if (inst->children == NULL) {
	attr = xmlSetNsProp(ctxt->insert, ns, name, (const xmlChar *) "");
    } else if ((inst->children->next == NULL) && 
	    ((inst->children->type == XML_TEXT_NODE) ||
	     (inst->children->type == XML_CDATA_SECTION_NODE)))
    {
	xmlNodePtr copyTxt;
	
	attr = xmlSetNsProp(ctxt->insert, ns, name, NULL);
	if (attr == NULL) 
	    goto error;
	if (ctxt->internalized &&
	    (ctxt->insert->doc != NULL) &&
	    (ctxt->insert->doc->dict == ctxt->dict))
	{
	    copyTxt = xmlNewText(NULL);
	    if (copyTxt == NULL) 
		goto error;
	    copyTxt->content = inst->children->content;
	    if (inst->children->name == xmlStringTextNoenc)
		copyTxt->name = xmlStringTextNoenc;
	} else {
	    copyTxt = xmlNewText(inst->children->content);
	    if (copyTxt == NULL) 
		goto error;	    	    
	}
	attr->children = attr->last = copyTxt;
	copyTxt->parent = (xmlNodePtr) attr;
	copyTxt->doc = attr->doc;
	if (inst->children->name == xmlStringTextNoenc)
	    copyTxt->name = xmlStringTextNoenc;	

        if ((copyTxt->content != NULL) &&
            (xmlIsID(attr->doc, attr->parent, attr)))
            xmlAddID(NULL, attr->doc, copyTxt->content, attr);
    } else {
	value = xsltEvalTemplateString(ctxt, contextNode, inst);
	if (value != NULL) {
	    attr = xmlSetNsProp(ctxt->insert, ns, name, value);
	    xmlFree(value);
	} else {
	    attr = xmlSetNsProp(ctxt->insert, ns, name,
		(const xmlChar *) "");
	}
    }

error:
    return;    
}

void
xsltAttribute(xsltTransformContextPtr ctxt, xmlNodePtr node,
	      xmlNodePtr inst, xsltStylePreCompPtr comp) {
    xsltAttributeInternal(ctxt, node, inst, comp, 0);
}

void
xsltApplyAttributeSet(xsltTransformContextPtr ctxt, xmlNodePtr node,
                      xmlNodePtr inst,
                      const xmlChar *attrSets)
{
    const xmlChar *ncname = NULL;
    const xmlChar *prefix = NULL;    
    const xmlChar *curstr, *endstr;
    xsltAttrElemPtr attrs;
    xsltStylesheetPtr style;    

    if (attrSets == NULL) {
	if (inst == NULL)
	    return;
	else {
	    if (inst->type == XML_ATTRIBUTE_NODE) {
		if ( ((xmlAttrPtr) inst)->children != NULL)
		    attrSets = ((xmlAttrPtr) inst)->children->content;
		
	    }
	    if (attrSets == NULL) {
		return;
	    }
	}
    }
    curstr = attrSets;
    while (*curstr != 0) {
        while (IS_BLANK(*curstr))
            curstr++;
        if (*curstr == 0)
            break;
        endstr = curstr;
        while ((*endstr != 0) && (!IS_BLANK(*endstr)))
            endstr++;
        curstr = xmlDictLookup(ctxt->dict, curstr, endstr - curstr);
        if (curstr) {

#ifdef WITH_XSLT_DEBUG_curstrUTES
            xsltGenericDebug(xsltGenericDebugContext,
                             "apply curstrute set %s\n", curstr);
#endif
            ncname = xsltSplitQName(ctxt->dict, curstr, &prefix);

            style = ctxt->style;

#ifdef WITH_DEBUGGER
            if ((style != NULL) &&
		(style->attributeSets != NULL) &&
		(ctxt->debugStatus != XSLT_DEBUG_NONE))
	    {
                attrs =
                    xmlHashLookup2(style->attributeSets, ncname, prefix);
                if ((attrs != NULL) && (attrs->attr != NULL))
                    xslHandleDebugger(attrs->attr->parent, node, NULL,
			ctxt);
            }
#endif
            while (style != NULL) {
                attrs =
                    xmlHashLookup2(style->attributeSets, ncname, prefix);
                while (attrs != NULL) {
                    if (attrs->attr != NULL) {
                        xsltAttributeInternal(ctxt, node, attrs->attr,
			    attrs->attr->psvi, 1);
                    }
                    attrs = attrs->next;
                }
                style = xsltNextImport(style);
            }
        }
        curstr = endstr;
    }
}

void
xsltFreeAttributeSetsHashes(xsltStylesheetPtr style) {
    if (style->attributeSets != NULL)
	xmlHashFree((xmlHashTablePtr) style->attributeSets,
		    (xmlHashDeallocator) xsltFreeAttrElemList);
    style->attributeSets = NULL;
}
