/*
 * preproc.c: Preprocessing of style operations
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
#include <libxml/uri.h>
#include <libxml/encoding.h>
#include <libxml/xmlerror.h>
#include "xslt.h"
#include "xsltutils.h"
#include "xsltInternals.h"
#include "transform.h"
#include "templates.h"
#include "variables.h"
#include "numbersInternals.h"
#include "preproc.h"
#include "extra.h"
#include "imports.h"
#include "extensions.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_PREPROC
#endif

const xmlChar *xsltExtMarker = (const xmlChar *) "Extension Element";


#ifdef XSLT_REFACTORED
#else
static int
xsltCheckTopLevelElement(xsltStylesheetPtr style, xmlNodePtr inst, int err) {
    xmlNodePtr parent;
    if ((style == NULL) || (inst == NULL) || (inst->ns == NULL))
        return(-1);
    
    parent = inst->parent;
    if (parent == NULL) {
        if (err) {
	    xsltTransformError(NULL, style, inst,
		    "internal problem: element has no parent\n");
	    style->errors++;
	}
	return(0);
    }
    if ((parent->ns == NULL) || (parent->type != XML_ELEMENT_NODE) ||
        ((parent->ns != inst->ns) &&
	 (!xmlStrEqual(parent->ns->href, inst->ns->href))) ||
	((!xmlStrEqual(parent->name, BAD_CAST "stylesheet")) &&
	 (!xmlStrEqual(parent->name, BAD_CAST "transform")))) {
	if (err) {
	    xsltTransformError(NULL, style, inst,
		    "element %s only allowed as child of stylesheet\n",
			       inst->name);
	    style->errors++;
	}
	return(0);
    }
    return(1);
}

static void
xsltCheckInstructionElement(xsltStylesheetPtr style, xmlNodePtr inst) {
    xmlNodePtr parent;
    int has_ext;

    if ((style == NULL) || (inst == NULL) || (inst->ns == NULL) ||
        (style->literal_result))
        return;

    has_ext = (style->extInfos != NULL);
    
    parent = inst->parent;
    if (parent == NULL) {
	xsltTransformError(NULL, style, inst,
		"internal problem: element has no parent\n");
	style->errors++;
	return;
    }
    while ((parent != NULL) && (parent->type != XML_DOCUMENT_NODE)) {
        if (((parent->ns == inst->ns) ||
	     ((parent->ns != NULL) &&
	      (xmlStrEqual(parent->ns->href, inst->ns->href)))) &&
	    ((xmlStrEqual(parent->name, BAD_CAST "template")) ||
	     (xmlStrEqual(parent->name, BAD_CAST "param")) ||
	     (xmlStrEqual(parent->name, BAD_CAST "attribute")) ||
	     (xmlStrEqual(parent->name, BAD_CAST "variable")))) {
	    return;
	}

	if ((has_ext) && (parent->ns != NULL) &&
	    (xmlHashLookup(style->extInfos, parent->ns->href) != NULL))
	    return;
	
        parent = parent->parent;
    }
    xsltTransformError(NULL, style, inst,
	    "element %s only allowed within a template, variable or param\n",
		           inst->name);
    style->errors++;
}

static void
xsltCheckParentElement(xsltStylesheetPtr style, xmlNodePtr inst,
                       const xmlChar *allow1, const xmlChar *allow2) {
    xmlNodePtr parent;

    if ((style == NULL) || (inst == NULL) || (inst->ns == NULL) ||
        (style->literal_result))
        return;

    parent = inst->parent;
    if (parent == NULL) {
	xsltTransformError(NULL, style, inst,
		"internal problem: element has no parent\n");
	style->errors++;
	return;
    }
    if (((parent->ns == inst->ns) ||
	 ((parent->ns != NULL) &&
	  (xmlStrEqual(parent->ns->href, inst->ns->href)))) &&
	((xmlStrEqual(parent->name, allow1)) ||
	 (xmlStrEqual(parent->name, allow2)))) {
	return;
    }

    if (style->extInfos != NULL) {
	while ((parent != NULL) && (parent->type != XML_DOCUMENT_NODE)) {
	    if ((parent->ns != NULL) &&
		(xmlHashLookup(style->extInfos, parent->ns->href) != NULL))
		return;
	    
	    parent = parent->parent;
	}
    }
    xsltTransformError(NULL, style, inst,
		       "element %s is not allowed within that context\n",
		       inst->name);
    style->errors++;
}
#endif


static xsltStylePreCompPtr
xsltNewStylePreComp(xsltStylesheetPtr style, xsltStyleType type) {
    xsltStylePreCompPtr cur;
#ifdef XSLT_REFACTORED
    size_t size;
#endif

    if (style == NULL)
        return(NULL);
   
#ifdef XSLT_REFACTORED
    switch (type) {
        case XSLT_FUNC_COPY:
            size = sizeof(xsltStyleItemCopy); break;
        case XSLT_FUNC_SORT:
            size = sizeof(xsltStyleItemSort); break;
        case XSLT_FUNC_TEXT:
            size = sizeof(xsltStyleItemText); break;
        case XSLT_FUNC_ELEMENT:
            size = sizeof(xsltStyleItemElement); break;
        case XSLT_FUNC_ATTRIBUTE:
            size = sizeof(xsltStyleItemAttribute); break;
        case XSLT_FUNC_COMMENT:
            size = sizeof(xsltStyleItemComment); break;
        case XSLT_FUNC_PI:
            size = sizeof(xsltStyleItemPI); break;
        case XSLT_FUNC_COPYOF:
            size = sizeof(xsltStyleItemCopyOf); break;
        case XSLT_FUNC_VALUEOF:
            size = sizeof(xsltStyleItemValueOf); break;;
        case XSLT_FUNC_NUMBER:
            size = sizeof(xsltStyleItemNumber); break;
        case XSLT_FUNC_APPLYIMPORTS:
            size = sizeof(xsltStyleItemApplyImports); break;
        case XSLT_FUNC_CALLTEMPLATE:
            size = sizeof(xsltStyleItemCallTemplate); break;
        case XSLT_FUNC_APPLYTEMPLATES:
            size = sizeof(xsltStyleItemApplyTemplates); break;
        case XSLT_FUNC_CHOOSE:
            size = sizeof(xsltStyleItemChoose); break;
        case XSLT_FUNC_IF:
            size = sizeof(xsltStyleItemIf); break;
        case XSLT_FUNC_FOREACH:
            size = sizeof(xsltStyleItemForEach); break;
        case XSLT_FUNC_DOCUMENT:
            size = sizeof(xsltStyleItemDocument); break;
	case XSLT_FUNC_WITHPARAM:
	    size = sizeof(xsltStyleItemWithParam); break;
	case XSLT_FUNC_PARAM:
	    size = sizeof(xsltStyleItemParam); break;
	case XSLT_FUNC_VARIABLE:
	    size = sizeof(xsltStyleItemVariable); break;
	case XSLT_FUNC_WHEN:
	    size = sizeof(xsltStyleItemWhen); break;
	case XSLT_FUNC_OTHERWISE:
	    size = sizeof(xsltStyleItemOtherwise); break;
	default:	
	    xsltTransformError(NULL, style, NULL,
		    "xsltNewStylePreComp : invalid type %d\n", type);
	    style->errors++;
	    return(NULL);
    }
    cur = (xsltStylePreCompPtr) xmlMalloc(size);
    if (cur == NULL) {
	xsltTransformError(NULL, style, NULL,
		"xsltNewStylePreComp : malloc failed\n");
	style->errors++;
	return(NULL);
    }
    memset(cur, 0, size);

#else 
    cur = (xsltStylePreCompPtr) xmlMalloc(sizeof(xsltStylePreComp));
    if (cur == NULL) {
	xsltTransformError(NULL, style, NULL,
		"xsltNewStylePreComp : malloc failed\n");
	style->errors++;
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltStylePreComp));
#endif 

    cur->type = type;
    switch (cur->type) {
        case XSLT_FUNC_COPY:
            cur->func = (xsltTransformFunction) xsltCopy;break;
        case XSLT_FUNC_SORT:
            cur->func = (xsltTransformFunction) xsltSort;break;
        case XSLT_FUNC_TEXT:
            cur->func = (xsltTransformFunction) xsltText;break;
        case XSLT_FUNC_ELEMENT:
            cur->func = (xsltTransformFunction) xsltElement;break;
        case XSLT_FUNC_ATTRIBUTE:
            cur->func = (xsltTransformFunction) xsltAttribute;break;
        case XSLT_FUNC_COMMENT:
            cur->func = (xsltTransformFunction) xsltComment;break;
        case XSLT_FUNC_PI:
            cur->func = (xsltTransformFunction) xsltProcessingInstruction;
	    break;
        case XSLT_FUNC_COPYOF:
            cur->func = (xsltTransformFunction) xsltCopyOf;break;
        case XSLT_FUNC_VALUEOF:
            cur->func = (xsltTransformFunction) xsltValueOf;break;
        case XSLT_FUNC_NUMBER:
            cur->func = (xsltTransformFunction) xsltNumber;break;
        case XSLT_FUNC_APPLYIMPORTS:
            cur->func = (xsltTransformFunction) xsltApplyImports;break;
        case XSLT_FUNC_CALLTEMPLATE:
            cur->func = (xsltTransformFunction) xsltCallTemplate;break;
        case XSLT_FUNC_APPLYTEMPLATES:
            cur->func = (xsltTransformFunction) xsltApplyTemplates;break;
        case XSLT_FUNC_CHOOSE:
            cur->func = (xsltTransformFunction) xsltChoose;break;
        case XSLT_FUNC_IF:
            cur->func = (xsltTransformFunction) xsltIf;break;
        case XSLT_FUNC_FOREACH:
            cur->func = (xsltTransformFunction) xsltForEach;break;
        case XSLT_FUNC_DOCUMENT:
            cur->func = (xsltTransformFunction) xsltDocumentElem;break;
	case XSLT_FUNC_WITHPARAM:
	case XSLT_FUNC_PARAM:	    
	case XSLT_FUNC_VARIABLE:	    
	case XSLT_FUNC_WHEN:
	    break;
	default:
	if (cur->func == NULL) {
	    xsltTransformError(NULL, style, NULL,
		    "xsltNewStylePreComp : no function for type %d\n", type);
	    style->errors++;
	}
    }
    cur->next = style->preComps;
    style->preComps = (xsltElemPreCompPtr) cur;

    return(cur);
}

static void
xsltFreeStylePreComp(xsltStylePreCompPtr comp) {
    if (comp == NULL)
	return;
#ifdef XSLT_REFACTORED
    switch (comp->type) {
	case XSLT_FUNC_LITERAL_RESULT_ELEMENT:
	    break;
	case XSLT_FUNC_COPY:
            break;
        case XSLT_FUNC_SORT: {
		xsltStyleItemSortPtr item = (xsltStyleItemSortPtr) comp;
		if (item->locale != (xsltLocale)0)
		    xsltFreeLocale(item->locale);
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
            break;
        case XSLT_FUNC_TEXT:
            break;
        case XSLT_FUNC_ELEMENT:
            break;
        case XSLT_FUNC_ATTRIBUTE:
            break;
        case XSLT_FUNC_COMMENT:
            break;
        case XSLT_FUNC_PI:
	    break;
        case XSLT_FUNC_COPYOF: {
		xsltStyleItemCopyOfPtr item = (xsltStyleItemCopyOfPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
            break;
        case XSLT_FUNC_VALUEOF: {
		xsltStyleItemValueOfPtr item = (xsltStyleItemValueOfPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
            break;
        case XSLT_FUNC_NUMBER:
            break;
        case XSLT_FUNC_APPLYIMPORTS:
            break;
        case XSLT_FUNC_CALLTEMPLATE:
            break;
        case XSLT_FUNC_APPLYTEMPLATES: {
		xsltStyleItemApplyTemplatesPtr item =
		    (xsltStyleItemApplyTemplatesPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
            break;
        case XSLT_FUNC_CHOOSE:
            break;
        case XSLT_FUNC_IF: {
		xsltStyleItemIfPtr item = (xsltStyleItemIfPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
            break;
        case XSLT_FUNC_FOREACH: {
		xsltStyleItemForEachPtr item =
		    (xsltStyleItemForEachPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
            break;
        case XSLT_FUNC_DOCUMENT:
            break;
	case XSLT_FUNC_WITHPARAM: {
		xsltStyleItemWithParamPtr item =
		    (xsltStyleItemWithParamPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
	    break;
	case XSLT_FUNC_PARAM: {
		xsltStyleItemParamPtr item =
		    (xsltStyleItemParamPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
	    break;
	case XSLT_FUNC_VARIABLE: {
		xsltStyleItemVariablePtr item =
		    (xsltStyleItemVariablePtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
	    break;
	case XSLT_FUNC_WHEN: {
		xsltStyleItemWhenPtr item =
		    (xsltStyleItemWhenPtr) comp;
		if (item->comp != NULL)
		    xmlXPathFreeCompExpr(item->comp);
	    }
	    break;
	case XSLT_FUNC_OTHERWISE:	    
	case XSLT_FUNC_FALLBACK:
	case XSLT_FUNC_MESSAGE:
	case XSLT_FUNC_INCLUDE:
	case XSLT_FUNC_ATTRSET:
	
	    break;
	default:
	    
	    break;
    }
#else    
    if (comp->locale != (xsltLocale)0)
	xsltFreeLocale(comp->locale);
    if (comp->comp != NULL)
	xmlXPathFreeCompExpr(comp->comp);
    if (comp->nsList != NULL)
	xmlFree(comp->nsList);
#endif

    xmlFree(comp);
}



xsltElemPreCompPtr
xsltDocumentComp(xsltStylesheetPtr style, xmlNodePtr inst,
		 xsltTransformFunction function ATTRIBUTE_UNUSED) {
#ifdef XSLT_REFACTORED
    xsltStyleItemDocumentPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    const xmlChar *filename = NULL;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemDocumentPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_DOCUMENT);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_DOCUMENT);
#endif
    
    if (comp == NULL)
	return (NULL);
    comp->inst = inst;
    comp->ver11 = 0;

    if (xmlStrEqual(inst->name, (const xmlChar *) "output")) {
#ifdef WITH_XSLT_DEBUG_EXTRA
	xsltGenericDebug(xsltGenericDebugContext,
	    "Found saxon:output extension\n");
#endif
	filename = xsltEvalStaticAttrValueTemplate(style, inst,
			 (const xmlChar *)"file",
			 NULL, &comp->has_filename);
    } else if (xmlStrEqual(inst->name, (const xmlChar *) "write")) {
#ifdef WITH_XSLT_DEBUG_EXTRA
	xsltGenericDebug(xsltGenericDebugContext,
	    "Found xalan:write extension\n");
#endif
	
    } else if (xmlStrEqual(inst->name, (const xmlChar *) "document")) {
	if (inst->ns != NULL) {
	    if (xmlStrEqual(inst->ns->href, XSLT_NAMESPACE)) {
		comp->ver11 = 1;
#ifdef WITH_XSLT_DEBUG_EXTRA
		xsltGenericDebug(xsltGenericDebugContext,
		    "Found xslt11:document construct\n");
#endif	    		
	    } else {		
		if (xmlStrEqual(inst->ns->href,
		    (const xmlChar *)"http://exslt.org/common")) {
		    
#ifdef WITH_XSLT_DEBUG_EXTRA
		    xsltGenericDebug(xsltGenericDebugContext,
			"Found exslt:document extension\n");
#endif
		} else if (xmlStrEqual(inst->ns->href, XSLT_XT_NAMESPACE)) {
		    
#ifdef WITH_XSLT_DEBUG_EXTRA
		    xsltGenericDebug(xsltGenericDebugContext,
			"Found xt:document extension\n");
#endif
		}
	    }
	}
	filename = xsltEvalStaticAttrValueTemplate(style, inst,
	    (const xmlChar *)"href", NULL, &comp->has_filename);
    }		
    if (!comp->has_filename) {
	goto error;
    }
    comp->filename = filename;

error:
    return ((xsltElemPreCompPtr) comp);
}


static void
xsltSortComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemSortPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemSortPtr) xsltNewStylePreComp(style, XSLT_FUNC_SORT);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_SORT);
#endif
    
    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->stype = xsltEvalStaticAttrValueTemplate(style, inst,
			 (const xmlChar *)"data-type",
			 NULL, &comp->has_stype);
    if (comp->stype != NULL) {
	if (xmlStrEqual(comp->stype, (const xmlChar *) "text"))
	    comp->number = 0;
	else if (xmlStrEqual(comp->stype, (const xmlChar *) "number"))
	    comp->number = 1;
	else {
	    xsltTransformError(NULL, style, inst,
		 "xsltSortComp: no support for data-type = %s\n", comp->stype);
	    comp->number = 0; 
	    if (style != NULL) style->warnings++;
	}
    }
    comp->order = xsltEvalStaticAttrValueTemplate(style, inst,
			      (const xmlChar *)"order",
			      NULL, &comp->has_order);
    if (comp->order != NULL) {
	if (xmlStrEqual(comp->order, (const xmlChar *) "ascending"))
	    comp->descending = 0;
	else if (xmlStrEqual(comp->order, (const xmlChar *) "descending"))
	    comp->descending = 1;
	else {
	    xsltTransformError(NULL, style, inst,
		 "xsltSortComp: invalid value %s for order\n", comp->order);
	    comp->descending = 0; 
	    if (style != NULL) style->warnings++;
	}
    }
    comp->case_order = xsltEvalStaticAttrValueTemplate(style, inst,
			      (const xmlChar *)"case-order",
			      NULL, &comp->has_use);
    if (comp->case_order != NULL) {
	if (xmlStrEqual(comp->case_order, (const xmlChar *) "upper-first"))
	    comp->lower_first = 0;
	else if (xmlStrEqual(comp->case_order, (const xmlChar *) "lower-first"))
	    comp->lower_first = 1;
	else {
	    xsltTransformError(NULL, style, inst,
		 "xsltSortComp: invalid value %s for order\n", comp->order);
	    comp->lower_first = 0; 
	    if (style != NULL) style->warnings++;
	}
    }

    comp->lang = xsltEvalStaticAttrValueTemplate(style, inst,
				 (const xmlChar *)"lang",
				 NULL, &comp->has_lang);
    if (comp->lang != NULL) {
	comp->locale = xsltNewLocale(comp->lang);
    }
    else {
        comp->locale = (xsltLocale)0;
    }

    comp->select = xsltGetCNsProp(style, inst,(const xmlChar *)"select", XSLT_NAMESPACE);
    if (comp->select == NULL) {
	comp->select = xmlDictLookup(style->dict, BAD_CAST ".", 1);
    }
    comp->comp = xsltXPathCompile(style, comp->select);
    if (comp->comp == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsltSortComp: could not compile select expression '%s'\n",
	                 comp->select);
	if (style != NULL) style->errors++;
    }
    if (inst->children != NULL) {
	xsltTransformError(NULL, style, inst,
	"xsl:sort : is not empty\n");
	if (style != NULL) style->errors++;
    }
}

static void
xsltCopyComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemCopyPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;
#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemCopyPtr) xsltNewStylePreComp(style, XSLT_FUNC_COPY);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_COPY);
#endif
    
    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;


    comp->use = xsltGetCNsProp(style, inst, (const xmlChar *)"use-attribute-sets",
				    XSLT_NAMESPACE);
    if (comp->use == NULL)
	comp->has_use = 0;
    else
	comp->has_use = 1;
}

#ifdef XSLT_REFACTORED
    
#else
static void
xsltTextComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemTextPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    const xmlChar *prop;

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemTextPtr) xsltNewStylePreComp(style, XSLT_FUNC_TEXT);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_TEXT);
#endif    
    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;
    comp->noescape = 0;

    prop = xsltGetCNsProp(style, inst,
	    (const xmlChar *)"disable-output-escaping",
			XSLT_NAMESPACE);
    if (prop != NULL) {
	if (xmlStrEqual(prop, (const xmlChar *)"yes")) {
	    comp->noescape = 1;
	} else if (!xmlStrEqual(prop,
	    (const xmlChar *)"no")){
	    xsltTransformError(NULL, style, inst,
		"xsl:text: disable-output-escaping allows only yes or no\n");
	    if (style != NULL) style->warnings++;
	}
    }
}
#endif 

static void
xsltElementComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemElementPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemElementPtr) xsltNewStylePreComp(style, XSLT_FUNC_ELEMENT);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_ELEMENT);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->name = xsltEvalStaticAttrValueTemplate(style, inst,
	(const xmlChar *)"name", NULL, &comp->has_name);
    if (! comp->has_name) {
	xsltTransformError(NULL, style, inst,
	    "xsl:element: The attribute 'name' is missing.\n");
	style->errors++;
	goto error;
    }
    comp->ns = xsltEvalStaticAttrValueTemplate(style, inst,
	(const xmlChar *)"namespace", NULL, &comp->has_ns);
    
    if (comp->name != NULL) {	
	if (xmlValidateQName(comp->name, 0)) {
	    xsltTransformError(NULL, style, inst,
		"xsl:element: The value '%s' of the attribute 'name' is "
		"not a valid QName.\n", comp->name);
	    style->errors++;
	} else {
	    const xmlChar *prefix = NULL, *name;

	    name = xsltSplitQName(style->dict, comp->name, &prefix);
	    if (comp->has_ns == 0) {	    
		xmlNsPtr ns;

		
		ns = xmlSearchNs(inst->doc, inst, prefix);
		if (ns != NULL) {
		    comp->ns = xmlDictLookup(style->dict, ns->href, -1);
		    comp->has_ns = 1;
#ifdef XSLT_REFACTORED
		    comp->nsPrefix = prefix;
		    comp->name = name;
#endif
		} else if (prefix != NULL) {
		    xsltTransformError(NULL, style, inst,
			"xsl:element: The prefixed QName '%s' "
			"has no namespace binding in scope in the "
			"stylesheet; this is an error, since the namespace was "
			"not specified by the instruction itself.\n", comp->name);
		    style->errors++;
		}
	    }	    
	    if ((prefix != NULL) &&
		(!xmlStrncasecmp(prefix, (xmlChar *)"xml", 3)))
	    {
		comp->has_name = 0;		
	    }
	}
    }    
    comp->use = xsltEvalStaticAttrValueTemplate(style, inst,
		       (const xmlChar *)"use-attribute-sets",
		       NULL, &comp->has_use);

error:    
    return;
}

static void
xsltAttributeComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemAttributePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemAttributePtr) xsltNewStylePreComp(style,
	XSLT_FUNC_ATTRIBUTE);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_ATTRIBUTE);
#endif
    
    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->name = xsltEvalStaticAttrValueTemplate(style, inst,
				 (const xmlChar *)"name",
				 NULL, &comp->has_name);
    if (! comp->has_name) {
	xsltTransformError(NULL, style, inst,
	    "XSLT-attribute: The attribute 'name' is missing.\n");
	style->errors++;
	return;
    }    
    comp->ns = xsltEvalStaticAttrValueTemplate(style, inst,
	(const xmlChar *)"namespace",
	NULL, &comp->has_ns);

    if (comp->name != NULL) {
	if (xmlValidateQName(comp->name, 0)) {
	    xsltTransformError(NULL, style, inst,
		"xsl:attribute: The value '%s' of the attribute 'name' is "
		"not a valid QName.\n", comp->name);
	    style->errors++;
	} else {
	    const xmlChar *prefix = NULL, *name;

	    name = xsltSplitQName(style->dict, comp->name, &prefix);
	    if (prefix != NULL) {
		if (comp->has_ns == 0) {
		    xmlNsPtr ns;

	    	    
		    ns = xmlSearchNs(inst->doc, inst, prefix);
		    if (ns != NULL) {
			comp->ns = xmlDictLookup(style->dict, ns->href, -1);
			comp->has_ns = 1;
#ifdef XSLT_REFACTORED
			comp->nsPrefix = prefix;
			comp->name = name;
#endif
		    } else {
			xsltTransformError(NULL, style, inst,
			    "xsl:attribute: The prefixed QName '%s' "
			    "has no namespace binding in scope in the "
			    "stylesheet; this is an error, since the "
			    "namespace was not specified by the instruction "
			    "itself.\n", comp->name);
			style->errors++;
		    }
		}
		if (!xmlStrncasecmp(prefix, (xmlChar *) "xmlns", 5)) {
		    comp->has_name = 0;
		    
#ifdef WITH_XSLT_DEBUG_PARSING
		    xsltGenericDebug(xsltGenericDebugContext,
			"xsltAttribute: xmlns prefix forbidden\n");
#endif		    
		    return;
		}
		
	    }
	}	
    }
}

static void
xsltCommentComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemCommentPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemCommentPtr) xsltNewStylePreComp(style, XSLT_FUNC_COMMENT);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_COMMENT);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;
}

static void
xsltProcessingInstructionComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemPIPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemPIPtr) xsltNewStylePreComp(style, XSLT_FUNC_PI);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_PI);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->name = xsltEvalStaticAttrValueTemplate(style, inst,
				 (const xmlChar *)"name",
				 XSLT_NAMESPACE, &comp->has_name);
}

static void
xsltCopyOfComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemCopyOfPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemCopyOfPtr) xsltNewStylePreComp(style, XSLT_FUNC_COPYOF);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_COPYOF);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->select = xsltGetCNsProp(style, inst, (const xmlChar *)"select",
	                        XSLT_NAMESPACE);
    if (comp->select == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:copy-of : select is missing\n");
	if (style != NULL) style->errors++;
	return;
    }
    comp->comp = xsltXPathCompile(style, comp->select);
    if (comp->comp == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:copy-of : could not compile select expression '%s'\n",
	                 comp->select);
	if (style != NULL) style->errors++;
    }
}

static void
xsltValueOfComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemValueOfPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    const xmlChar *prop;

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemValueOfPtr) xsltNewStylePreComp(style, XSLT_FUNC_VALUEOF);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_VALUEOF);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    prop = xsltGetCNsProp(style, inst,
	    (const xmlChar *)"disable-output-escaping",
			XSLT_NAMESPACE);
    if (prop != NULL) {
	if (xmlStrEqual(prop, (const xmlChar *)"yes")) {
	    comp->noescape = 1;
	} else if (!xmlStrEqual(prop,
				(const xmlChar *)"no")){
	    xsltTransformError(NULL, style, inst,
"xsl:value-of : disable-output-escaping allows only yes or no\n");
	    if (style != NULL) style->warnings++;
	}
    }
    comp->select = xsltGetCNsProp(style, inst, (const xmlChar *)"select",
	                        XSLT_NAMESPACE);
    if (comp->select == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:value-of : select is missing\n");
	if (style != NULL) style->errors++;
	return;
    }
    comp->comp = xsltXPathCompile(style, comp->select);
    if (comp->comp == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:value-of : could not compile select expression '%s'\n",
	                 comp->select);
	if (style != NULL) style->errors++;
    }
}

static void
xsltGetQNameProperty(xsltStylesheetPtr style, xmlNodePtr inst,
		     const xmlChar *propName,
		     int mandatory,
		     int *hasProp, const xmlChar **nsName,
		     const xmlChar** localName)
{
    const xmlChar *prop;

    if (nsName)
	*nsName = NULL;
    if (localName)
	*localName = NULL;
    if (hasProp)
	*hasProp = 0;

    prop = xsltGetCNsProp(style, inst, propName, XSLT_NAMESPACE);
    if (prop == NULL) {
	if (mandatory) {
	    xsltTransformError(NULL, style, inst,
		"The attribute '%s' is missing.\n", propName);
	    style->errors++;
	    return;
	}
    } else {
        const xmlChar *URI;

	if (xmlValidateQName(prop, 0)) {
	    xsltTransformError(NULL, style, inst,
		"The value '%s' of the attribute "
		"'%s' is not a valid QName.\n", prop, propName);
	    style->errors++;
	    return;
	} else {
	    URI = xsltGetQNameURI2(style, inst, &prop);
	    if (prop == NULL) {
		style->errors++;
	    } else {
		*localName = prop;
		if (hasProp)
		    *hasProp = 1;
		if (URI != NULL) {
		    if (nsName)
			*nsName = xmlDictLookup(style->dict, URI, -1);
		    
		}
	    }
	}
    }
    return;
}

static void
xsltWithParamComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemWithParamPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemWithParamPtr) xsltNewStylePreComp(style, XSLT_FUNC_WITHPARAM);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_WITHPARAM);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    xsltGetQNameProperty(style, inst, BAD_CAST "name",
	1, &(comp->has_name), &(comp->ns), &(comp->name));
    if (comp->ns)
	comp->has_ns = 1;
    comp->select = xsltGetCNsProp(style, inst, (const xmlChar *)"select",
	                        XSLT_NAMESPACE);
    if (comp->select != NULL) {
	comp->comp = xsltXPathCompile(style, comp->select);
	if (comp->comp == NULL) {
	    xsltTransformError(NULL, style, inst,
		 "XSLT-with-param: Failed to compile select "
		 "expression '%s'\n", comp->select);
	    style->errors++;
	}
	if (inst->children != NULL) {
	    xsltTransformError(NULL, style, inst,
		"XSLT-with-param: The content should be empty since "
		"the attribute select is present.\n");
	    style->warnings++;
	}
    }
}

static void
xsltNumberComp(xsltStylesheetPtr style, xmlNodePtr cur) {
#ifdef XSLT_REFACTORED
    xsltStyleItemNumberPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    const xmlChar *prop;

    if ((style == NULL) || (cur == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemNumberPtr) xsltNewStylePreComp(style, XSLT_FUNC_NUMBER);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_NUMBER);
#endif

    if (comp == NULL)
	return;
    cur->psvi = comp;

    if ((style == NULL) || (cur == NULL))
	return;

    comp->numdata.doc = cur->doc;
    comp->numdata.node = cur;
    comp->numdata.value = xsltGetCNsProp(style, cur, (const xmlChar *)"value",
	                                XSLT_NAMESPACE);
    
    prop = xsltEvalStaticAttrValueTemplate(style, cur,
			 (const xmlChar *)"format",
			 XSLT_NAMESPACE, &comp->numdata.has_format);
    if (comp->numdata.has_format == 0) {
	comp->numdata.format = xmlDictLookup(style->dict, BAD_CAST "" , 0);
    } else {
	comp->numdata.format = prop;
    }

    comp->numdata.count = xsltGetCNsProp(style, cur, (const xmlChar *)"count",
					XSLT_NAMESPACE);
    comp->numdata.from = xsltGetCNsProp(style, cur, (const xmlChar *)"from",
					XSLT_NAMESPACE);
    
    prop = xsltGetCNsProp(style, cur, (const xmlChar *)"level", XSLT_NAMESPACE);
    if (prop != NULL) {
	if (xmlStrEqual(prop, BAD_CAST("single")) ||
	    xmlStrEqual(prop, BAD_CAST("multiple")) ||
	    xmlStrEqual(prop, BAD_CAST("any"))) {
	    comp->numdata.level = prop;
	} else {
	    xsltTransformError(NULL, style, cur,
			 "xsl:number : invalid value %s for level\n", prop);
	    if (style != NULL) style->warnings++;
	}
    }
    
    prop = xsltGetCNsProp(style, cur, (const xmlChar *)"lang", XSLT_NAMESPACE);
    if (prop != NULL) {
	    xsltTransformError(NULL, style, cur,
		 "xsl:number : lang attribute not implemented\n");
	XSLT_TODO; 
    }
    
    prop = xsltGetCNsProp(style, cur, (const xmlChar *)"letter-value", XSLT_NAMESPACE);
    if (prop != NULL) {
	if (xmlStrEqual(prop, BAD_CAST("alphabetic"))) {
	    xsltTransformError(NULL, style, cur,
		 "xsl:number : letter-value 'alphabetic' not implemented\n");
	    if (style != NULL) style->warnings++;
	    XSLT_TODO; 
	} else if (xmlStrEqual(prop, BAD_CAST("traditional"))) {
	    xsltTransformError(NULL, style, cur,
		 "xsl:number : letter-value 'traditional' not implemented\n");
	    if (style != NULL) style->warnings++;
	    XSLT_TODO; 
	} else {
	    xsltTransformError(NULL, style, cur,
		     "xsl:number : invalid value %s for letter-value\n", prop);
	    if (style != NULL) style->warnings++;
	}
    }
    
    prop = xsltGetCNsProp(style, cur, (const xmlChar *)"grouping-separator",
	                XSLT_NAMESPACE);
    if (prop != NULL) {
        comp->numdata.groupingCharacterLen = xmlStrlen(prop);
	comp->numdata.groupingCharacter =
	    xsltGetUTF8Char(prop, &(comp->numdata.groupingCharacterLen));
    }
    
    prop = xsltGetCNsProp(style, cur, (const xmlChar *)"grouping-size", XSLT_NAMESPACE);
    if (prop != NULL) {
	sscanf((char *)prop, "%d", &comp->numdata.digitsPerGroup);
    } else {
	comp->numdata.groupingCharacter = 0;
    }

    
    if (comp->numdata.value == NULL) {
	if (comp->numdata.level == NULL) {
	    comp->numdata.level = xmlDictLookup(style->dict,
	                                        BAD_CAST"single", 6);
	}
    }
    
}

static void
xsltApplyImportsComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemApplyImportsPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemApplyImportsPtr) xsltNewStylePreComp(style, XSLT_FUNC_APPLYIMPORTS);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_APPLYIMPORTS);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;
}

static void
xsltCallTemplateComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemCallTemplatePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemCallTemplatePtr)
	xsltNewStylePreComp(style, XSLT_FUNC_CALLTEMPLATE);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_CALLTEMPLATE);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    xsltGetQNameProperty(style, inst, BAD_CAST "name",
	1, &(comp->has_name), &(comp->ns), &(comp->name));
    if (comp->ns)
	comp->has_ns = 1;
}

static void
xsltApplyTemplatesComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemApplyTemplatesPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemApplyTemplatesPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_APPLYTEMPLATES);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_APPLYTEMPLATES);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    xsltGetQNameProperty(style, inst, BAD_CAST "mode",
	0, NULL, &(comp->modeURI), &(comp->mode));
    comp->select = xsltGetCNsProp(style, inst, BAD_CAST "select",
	XSLT_NAMESPACE);
    if (comp->select != NULL) {
	comp->comp = xsltXPathCompile(style, comp->select);
	if (comp->comp == NULL) {
	    xsltTransformError(NULL, style, inst,
		"XSLT-apply-templates: could not compile select "
		"expression '%s'\n", comp->select);
	     style->errors++;
	}
    }
    
}

static void
xsltChooseComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemChoosePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemChoosePtr)
	xsltNewStylePreComp(style, XSLT_FUNC_CHOOSE);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_CHOOSE);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;
}

static void
xsltIfComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemIfPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemIfPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_IF);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_IF);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->test = xsltGetCNsProp(style, inst, (const xmlChar *)"test", XSLT_NAMESPACE);
    if (comp->test == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:if : test is not defined\n");
	if (style != NULL) style->errors++;
	return;
    }
    comp->comp = xsltXPathCompile(style, comp->test);
    if (comp->comp == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:if : could not compile test expression '%s'\n",
	                 comp->test);
	if (style != NULL) style->errors++;
    }
}

static void
xsltWhenComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemWhenPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemWhenPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_WHEN);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_WHEN);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->test = xsltGetCNsProp(style, inst, (const xmlChar *)"test", XSLT_NAMESPACE);
    if (comp->test == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:when : test is not defined\n");
	if (style != NULL) style->errors++;
	return;
    }
    comp->comp = xsltXPathCompile(style, comp->test);
    if (comp->comp == NULL) {
	xsltTransformError(NULL, style, inst,
	     "xsl:when : could not compile test expression '%s'\n",
	                 comp->test);
	if (style != NULL) style->errors++;
    }
}

static void
xsltForEachComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemForEachPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemForEachPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_FOREACH);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_FOREACH);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    comp->select = xsltGetCNsProp(style, inst, (const xmlChar *)"select",
	                        XSLT_NAMESPACE);
    if (comp->select == NULL) {
	xsltTransformError(NULL, style, inst,
		"xsl:for-each : select is missing\n");
	if (style != NULL) style->errors++;
    } else {
	comp->comp = xsltXPathCompile(style, comp->select);
	if (comp->comp == NULL) {
	    xsltTransformError(NULL, style, inst,
     "xsl:for-each : could not compile select expression '%s'\n",
			     comp->select);
	    if (style != NULL) style->errors++;
	}
    }
    
}

static void
xsltVariableComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemVariablePtr)
	xsltNewStylePreComp(style, XSLT_FUNC_VARIABLE);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_VARIABLE);
#endif

    if (comp == NULL)
	return;

    inst->psvi = comp;
    comp->inst = inst;

    xsltGetQNameProperty(style, inst, BAD_CAST "name",
	1, &(comp->has_name), &(comp->ns), &(comp->name));
    if (comp->ns)
	comp->has_ns = 1;    
    comp->select = xsltGetCNsProp(style, inst, (const xmlChar *)"select",
	                        XSLT_NAMESPACE);
    if (comp->select != NULL) {
	comp->comp = xsltXPathCompile(style, comp->select);
	if (comp->comp == NULL) {
	    xsltTransformError(NULL, style, inst,
		"XSLT-variable: Failed to compile the XPath expression '%s'.\n",
		comp->select);
	    style->errors++;
	}
	if (inst->children != NULL) {
	    xsltTransformError(NULL, style, inst,
		"XSLT-variable: The must be no child nodes, since the "
		"attribute 'select' was specified.\n");
	    style->errors++;
	}
    }
}

static void
xsltParamComp(xsltStylesheetPtr style, xmlNodePtr inst) {
#ifdef XSLT_REFACTORED
    xsltStyleItemParamPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((style == NULL) || (inst == NULL))
	return;

#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemParamPtr)
	xsltNewStylePreComp(style, XSLT_FUNC_PARAM);
#else
    comp = xsltNewStylePreComp(style, XSLT_FUNC_PARAM);
#endif

    if (comp == NULL)
	return;
    inst->psvi = comp;
    comp->inst = inst;

    xsltGetQNameProperty(style, inst, BAD_CAST "name",
	1, &(comp->has_name), &(comp->ns), &(comp->name));
    if (comp->ns)
	comp->has_ns = 1;
    comp->select = xsltGetCNsProp(style, inst, (const xmlChar *)"select",
	                        XSLT_NAMESPACE);
    if (comp->select != NULL) {
	comp->comp = xsltXPathCompile(style, comp->select);
	if (comp->comp == NULL) {
	    xsltTransformError(NULL, style, inst,
		"XSLT-param: could not compile select expression '%s'.\n",
		comp->select);
	    style->errors++;
	}
	if (inst->children != NULL) {
	    xsltTransformError(NULL, style, inst,
		"XSLT-param: The content should be empty since the "
		"attribute 'select' is present.\n");
	    style->warnings++;
	}
    }
}


void
xsltFreeStylePreComps(xsltStylesheetPtr style) {
    xsltElemPreCompPtr cur, next;

    if (style == NULL)
	return;        
    
    cur = style->preComps;
    while (cur != NULL) {
	next = cur->next;		
	if (cur->type == XSLT_FUNC_EXTENSION)
	    cur->free(cur);
	else
	    xsltFreeStylePreComp((xsltStylePreCompPtr) cur);
	cur = next;
    }
}

#ifdef XSLT_REFACTORED

void
xsltStylePreCompute(xsltStylesheetPtr style, xmlNodePtr node) {
    if (style == NULL) {
	if (node != NULL)
	    node->psvi = NULL;
	return;
    }
    if (node == NULL)
	return;
    if (! IS_XSLT_ELEM_FAST(node))
	return;

    node->psvi = NULL;
    if (XSLT_CCTXT(style)->inode->type != 0) {
	switch (XSLT_CCTXT(style)->inode->type) {
	    case XSLT_FUNC_APPLYTEMPLATES:
		xsltApplyTemplatesComp(style, node);
		break;
	    case XSLT_FUNC_WITHPARAM:			   
		xsltWithParamComp(style, node);
		break;
	    case XSLT_FUNC_VALUEOF:	    
		xsltValueOfComp(style, node);
		break;
	    case XSLT_FUNC_COPY:	    
		xsltCopyComp(style, node);
		break;
	    case XSLT_FUNC_COPYOF:
		xsltCopyOfComp(style, node);
		break;
	    case XSLT_FUNC_IF:	    
		xsltIfComp(style, node);
		break;
	    case XSLT_FUNC_CHOOSE:	    
		xsltChooseComp(style, node);
		break;
	    case XSLT_FUNC_WHEN:	    
		xsltWhenComp(style, node);
		break;
	    case XSLT_FUNC_OTHERWISE:	    
		
		return;
	    case XSLT_FUNC_FOREACH:	    
		xsltForEachComp(style, node);
		break;
	    case XSLT_FUNC_APPLYIMPORTS:	    
		xsltApplyImportsComp(style, node);
		break;
	    case XSLT_FUNC_ATTRIBUTE:	    
		xsltAttributeComp(style, node);
		break;
	    case XSLT_FUNC_ELEMENT:	    
		xsltElementComp(style, node);
		break;
	    case XSLT_FUNC_SORT:	    
		xsltSortComp(style, node);
		break;
	    case XSLT_FUNC_COMMENT:	    
		xsltCommentComp(style, node);
		break;
	    case XSLT_FUNC_NUMBER:	    
		xsltNumberComp(style, node);
		break;
	    case XSLT_FUNC_PI:	    
		xsltProcessingInstructionComp(style, node);
		break;
	    case XSLT_FUNC_CALLTEMPLATE:	    
		xsltCallTemplateComp(style, node);
		break;
	    case XSLT_FUNC_PARAM:	    
		xsltParamComp(style, node);
		break;
	    case XSLT_FUNC_VARIABLE:	    
		xsltVariableComp(style, node);
		break;
	    case XSLT_FUNC_FALLBACK:	    
		
		return;
	    case XSLT_FUNC_DOCUMENT:	    
		
		node->psvi = (void *) xsltDocumentComp(style, node,
		    (xsltTransformFunction) xsltDocumentElem);
		break;
	    case XSLT_FUNC_MESSAGE:
		
		return;
	    default:
		xsltTransformError(NULL, style, node,
		    "Internal error: (xsltStylePreCompute) cannot handle "
		    "the XSLT element '%s'.\n", node->name);
		style->errors++;
		return;
	}
    } else {
	
	if (IS_XSLT_NAME(node, "apply-templates")) {
	    xsltApplyTemplatesComp(style, node);
	} else if (IS_XSLT_NAME(node, "with-param")) {
	    xsltWithParamComp(style, node);
	} else if (IS_XSLT_NAME(node, "value-of")) {
	    xsltValueOfComp(style, node);
	} else if (IS_XSLT_NAME(node, "copy")) {
	    xsltCopyComp(style, node);
	} else if (IS_XSLT_NAME(node, "copy-of")) {
	    xsltCopyOfComp(style, node);
	} else if (IS_XSLT_NAME(node, "if")) {
	    xsltIfComp(style, node);
	} else if (IS_XSLT_NAME(node, "choose")) {
	    xsltChooseComp(style, node);
	} else if (IS_XSLT_NAME(node, "when")) {
	    xsltWhenComp(style, node);	
	} else if (IS_XSLT_NAME(node, "otherwise")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "for-each")) {
	    xsltForEachComp(style, node);
	} else if (IS_XSLT_NAME(node, "apply-imports")) {
	    xsltApplyImportsComp(style, node);
	} else if (IS_XSLT_NAME(node, "attribute")) {
	    xsltAttributeComp(style, node);
	} else if (IS_XSLT_NAME(node, "element")) {
	    xsltElementComp(style, node);
	} else if (IS_XSLT_NAME(node, "sort")) {
	    xsltSortComp(style, node);
	} else if (IS_XSLT_NAME(node, "comment")) {
	    xsltCommentComp(style, node);
	} else if (IS_XSLT_NAME(node, "number")) {
	    xsltNumberComp(style, node);
	} else if (IS_XSLT_NAME(node, "processing-instruction")) {
	    xsltProcessingInstructionComp(style, node);
	} else if (IS_XSLT_NAME(node, "call-template")) {
	    xsltCallTemplateComp(style, node);
	} else if (IS_XSLT_NAME(node, "param")) {
	    xsltParamComp(style, node);
	} else if (IS_XSLT_NAME(node, "variable")) {
	    xsltVariableComp(style, node);
	} else if (IS_XSLT_NAME(node, "fallback")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "document")) {
	    
	    node->psvi = (void *) xsltDocumentComp(style, node,
		(xsltTransformFunction) xsltDocumentElem);	
	} else if (IS_XSLT_NAME(node, "output")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "preserve-space")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "strip-space")) {
	    
	    return;	
	} else if (IS_XSLT_NAME(node, "key")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "message")) {
	    return;
	} else if (IS_XSLT_NAME(node, "attribute-set")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "namespace-alias")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "decimal-format")) {
	    
	    return;
	} else if (IS_XSLT_NAME(node, "include")) {
	    	    	    
	} else {
	    xsltTransformError(NULL, style, node,
		"Internal error: (xsltStylePreCompute) cannot handle "
		"the XSLT element '%s'.\n", node->name);
		style->errors++;
	    return;
	}	
    }
    if (node->psvi != NULL) {
	((xsltStylePreCompPtr) node->psvi)->inScopeNs =
	    XSLT_CCTXT(style)->inode->inScopeNs;
    }
}

#else

void
xsltStylePreCompute(xsltStylesheetPtr style, xmlNodePtr inst) {
    if (inst->psvi != NULL)
	return;

    if (IS_XSLT_ELEM(inst)) {
	xsltStylePreCompPtr cur;

	if (IS_XSLT_NAME(inst, "apply-templates")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltApplyTemplatesComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "with-param")) {
	    xsltCheckParentElement(style, inst, BAD_CAST "apply-templates",
	                           BAD_CAST "call-template");
	    xsltWithParamComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "value-of")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltValueOfComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "copy")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltCopyComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "copy-of")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltCopyOfComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "if")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltIfComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "when")) {
	    xsltCheckParentElement(style, inst, BAD_CAST "choose", NULL);
	    xsltWhenComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "choose")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltChooseComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "for-each")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltForEachComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "apply-imports")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltApplyImportsComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "attribute")) {
	    xmlNodePtr parent = inst->parent;

	    if ((parent == NULL) || (parent->ns == NULL) ||
		((parent->ns != inst->ns) &&
		 (!xmlStrEqual(parent->ns->href, inst->ns->href))) ||
		(!xmlStrEqual(parent->name, BAD_CAST "attribute-set"))) {
		xsltCheckInstructionElement(style, inst);
	    }
	    xsltAttributeComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "element")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltElementComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "text")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltTextComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "sort")) {
	    xsltCheckParentElement(style, inst, BAD_CAST "apply-templates",
	                           BAD_CAST "for-each");
	    xsltSortComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "comment")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltCommentComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "number")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltNumberComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "processing-instruction")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltProcessingInstructionComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "call-template")) {
	    xsltCheckInstructionElement(style, inst);
	    xsltCallTemplateComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "param")) {	   
	    if (xsltCheckTopLevelElement(style, inst, 0) == 0)
	        xsltCheckInstructionElement(style, inst);
	    xsltParamComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "variable")) {
	    if (xsltCheckTopLevelElement(style, inst, 0) == 0)
	        xsltCheckInstructionElement(style, inst);
	    xsltVariableComp(style, inst);
	} else if (IS_XSLT_NAME(inst, "otherwise")) {
	    xsltCheckParentElement(style, inst, BAD_CAST "choose", NULL);
	    xsltCheckInstructionElement(style, inst);
	    return;
	} else if (IS_XSLT_NAME(inst, "template")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "output")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "preserve-space")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "strip-space")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if ((IS_XSLT_NAME(inst, "stylesheet")) ||
	           (IS_XSLT_NAME(inst, "transform"))) {
	    xmlNodePtr parent = inst->parent;

	    if ((parent == NULL) || (parent->type != XML_DOCUMENT_NODE)) {
		xsltTransformError(NULL, style, inst,
		    "element %s only allowed only as root element\n",
				   inst->name);
		style->errors++;
	    }
	    return;
	} else if (IS_XSLT_NAME(inst, "key")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "message")) {
	    xsltCheckInstructionElement(style, inst);
	    return;
	} else if (IS_XSLT_NAME(inst, "attribute-set")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "namespace-alias")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "include")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "import")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "decimal-format")) {
	    xsltCheckTopLevelElement(style, inst, 1);
	    return;
	} else if (IS_XSLT_NAME(inst, "fallback")) {
	    xsltCheckInstructionElement(style, inst);
	    return;
	} else if (IS_XSLT_NAME(inst, "document")) {
	    xsltCheckInstructionElement(style, inst);
	    inst->psvi = (void *) xsltDocumentComp(style, inst,
				(xsltTransformFunction) xsltDocumentElem);
	} else {
	    xsltTransformError(NULL, style, inst,
		 "xsltStylePreCompute: unknown xsl:%s\n", inst->name);
	    if (style != NULL) style->warnings++;
	}
	
	cur = (xsltStylePreCompPtr) inst->psvi;
	if (cur != NULL) {
	    int i = 0;

	    cur->nsList = xmlGetNsList(inst->doc, inst);
            if (cur->nsList != NULL) {
		while (cur->nsList[i] != NULL)
		    i++;
	    }
	    cur->nsNr = i;
	}
    } else {
	inst->psvi =
	    (void *) xsltPreComputeExtModuleElement(style, inst);

	if (inst->psvi == NULL)
	    inst->psvi = (void *) xsltExtMarker;
    }
}
#endif 
