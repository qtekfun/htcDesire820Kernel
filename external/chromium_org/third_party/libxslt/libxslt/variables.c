/*
 * variables.c: Implementation of the variable storage and lookup
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
#include <libxml/tree.h>
#include <libxml/valid.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>
#include <libxml/dict.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "variables.h"
#include "transform.h"
#include "imports.h"
#include "preproc.h"
#include "keys.h"

#ifdef WITH_XSLT_DEBUG
 #define WITH_XSLT_DEBUG_VARIABLE
#endif

#ifdef XSLT_REFACTORED
const xmlChar *xsltDocFragFake = (const xmlChar *) " fake node libxslt";
#endif

const xmlChar *xsltComputingGlobalVarMarker =
 (const xmlChar *) " var/param being computed";

#define XSLT_VAR_GLOBAL 1<<0
#define XSLT_VAR_IN_SELECT 1<<1
#define XSLT_TCTXT_VARIABLE(c) ((xsltStackElemPtr) (c)->contextVariable)

xmlDocPtr
xsltCreateRVT(xsltTransformContextPtr ctxt)
{
    xmlDocPtr container;

    
    if (ctxt == NULL)
	return(NULL);

    if (ctxt->cache->RVT) {
	container = ctxt->cache->RVT;
	ctxt->cache->RVT = (xmlDocPtr) container->next;
	
	container->next = NULL;
	container->prev = NULL;
	if (ctxt->cache->nbRVT > 0)
	    ctxt->cache->nbRVT--;
#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgReusedRVTs++;
#endif
	return(container);
    }

    container = xmlNewDoc(NULL);
    if (container == NULL)
	return(NULL);
    container->dict = ctxt->dict;
    xmlDictReference(container->dict);
    XSLT_MARK_RES_TREE_FRAG(container);
    container->doc = container;
    container->parent = NULL;
    return(container);
}

int
xsltRegisterTmpRVT(xsltTransformContextPtr ctxt, xmlDocPtr RVT)
{
    if ((ctxt == NULL) || (RVT == NULL))
	return(-1);

    if (ctxt->contextVariable != NULL) {
	RVT->next = (xmlNodePtr) XSLT_TCTXT_VARIABLE(ctxt)->fragment;
	XSLT_TCTXT_VARIABLE(ctxt)->fragment = RVT;
	return(0);
    }

    RVT->next = (xmlNodePtr) ctxt->tmpRVT;
    if (ctxt->tmpRVT != NULL)
	ctxt->tmpRVT->prev = (xmlNodePtr) RVT;
    ctxt->tmpRVT = RVT;
    return(0);
}

int
xsltRegisterLocalRVT(xsltTransformContextPtr ctxt,
		     xmlDocPtr RVT)
{
    if ((ctxt == NULL) || (RVT == NULL))
	return(-1);
    
    if ((ctxt->contextVariable != NULL) &&
	(XSLT_TCTXT_VARIABLE(ctxt)->flags & XSLT_VAR_IN_SELECT))
    {
	RVT->next = (xmlNodePtr) XSLT_TCTXT_VARIABLE(ctxt)->fragment;
	XSLT_TCTXT_VARIABLE(ctxt)->fragment = RVT;
	return(0);
    }
    RVT->next = (xmlNodePtr) ctxt->localRVT;
    if (ctxt->localRVT != NULL)
	ctxt->localRVT->prev = (xmlNodePtr) RVT;
    ctxt->localRVT = RVT;
    if (ctxt->localRVTBase == NULL)
	ctxt->localRVTBase = RVT;
    return(0);
}

int
xsltExtensionInstructionResultFinalize(xsltTransformContextPtr ctxt)
{
    xmlDocPtr cur;

    if (ctxt == NULL)
	return(-1);
    if (ctxt->localRVTBase == NULL)
	return(0);
    cur = ctxt->localRVTBase;
    do {
	cur->psvi = NULL;
	cur = (xmlDocPtr) cur->next;
    } while (cur != NULL);
    return(0);
}

int
xsltExtensionInstructionResultRegister(xsltTransformContextPtr ctxt,
				       xmlXPathObjectPtr obj)
{
    int i;
    xmlNodePtr cur;
    xmlDocPtr doc;

    if ((ctxt == NULL) || (obj == NULL))
	return(-1);


    if ((obj->type != XPATH_NODESET) && (obj->type != XPATH_XSLT_TREE))
	return(0);
    if ((obj->nodesetval == NULL) || (obj->nodesetval->nodeNr == 0))
	return(0);

    for (i = 0; i < obj->nodesetval->nodeNr; i++) {
	cur = obj->nodesetval->nodeTab[i];
	if (cur->type == XML_NAMESPACE_DECL) {
	    if ((((xmlNsPtr) cur)->next != NULL) &&
		(((xmlNsPtr) cur)->next->type == XML_ELEMENT_NODE))
	    {
		cur = (xmlNodePtr) ((xmlNsPtr) cur)->next;
		doc = cur->doc;	
	    } else {
		xsltTransformError(ctxt, NULL, ctxt->inst,
		    "Internal error in "
		    "xsltExtensionInstructionResultRegister(): "
		    "Cannot retrieve the doc of a namespace node.\n");
		goto error;
	    }
	} else {
	    doc = cur->doc;
	}
	if (doc == NULL) {
	    xsltTransformError(ctxt, NULL, ctxt->inst,
		"Internal error in "
		"xsltExtensionInstructionResultRegister(): "
		"Cannot retrieve the doc of a node.\n");
	    goto error;
	}
	if (doc->name && (doc->name[0] == ' ')) {
	    doc->psvi = (void *) ((long) 1);
	}
    }

    return(0);
error:
    return(-1);
}

void
xsltReleaseRVT(xsltTransformContextPtr ctxt, xmlDocPtr RVT)
{
    if (RVT == NULL)
	return;

    if (ctxt && (ctxt->cache->nbRVT < 40)) {
	if (RVT->_private != NULL) {
	    xsltFreeDocumentKeys((xsltDocumentPtr) RVT->_private);
	    xmlFree(RVT->_private);
	    RVT->_private = NULL;
	}
	if (RVT->children != NULL) {
	    xmlFreeNodeList(RVT->children);
	    RVT->children = NULL;
	    RVT->last = NULL;
	}
	if (RVT->ids != NULL) {
	    xmlFreeIDTable((xmlIDTablePtr) RVT->ids);
	    RVT->ids = NULL;
	}
	if (RVT->refs != NULL) {
	    xmlFreeRefTable((xmlRefTablePtr) RVT->refs);
	    RVT->refs = NULL;
	}

	RVT->psvi = 0;

	RVT->next = (xmlNodePtr) ctxt->cache->RVT;
	ctxt->cache->RVT = RVT;

	ctxt->cache->nbRVT++;

#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgCachedRVTs++;
#endif
	return;
    }
    if (RVT->_private != NULL) {
	xsltFreeDocumentKeys((xsltDocumentPtr) RVT->_private);
	xmlFree(RVT->_private);
    }
    xmlFreeDoc(RVT);
}

int
xsltRegisterPersistRVT(xsltTransformContextPtr ctxt, xmlDocPtr RVT)
{
    if ((ctxt == NULL) || (RVT == NULL)) return(-1);

    RVT->next = (xmlNodePtr) ctxt->persistRVT;
    if (ctxt->persistRVT != NULL)
	ctxt->persistRVT->prev = (xmlNodePtr) RVT;
    ctxt->persistRVT = RVT;
    return(0);
}

void
xsltFreeRVTs(xsltTransformContextPtr ctxt)
{
    xmlDocPtr cur, next;

    if (ctxt == NULL)
	return;
    cur = ctxt->localRVT;
    while (cur != NULL) {
        next = (xmlDocPtr) cur->next;
	if (cur->_private != NULL) {
	    xsltFreeDocumentKeys(cur->_private);
	    xmlFree(cur->_private);
	}
	xmlFreeDoc(cur);
	cur = next;
    }
    ctxt->localRVT = NULL;
    cur = ctxt->tmpRVT;
    while (cur != NULL) {
        next = (xmlDocPtr) cur->next;
	if (cur->_private != NULL) {
	    xsltFreeDocumentKeys(cur->_private);
	    xmlFree(cur->_private);
	}
	xmlFreeDoc(cur);
	cur = next;
    }
    ctxt->tmpRVT = NULL;
    cur = ctxt->persistRVT;
    while (cur != NULL) {
        next = (xmlDocPtr) cur->next;
	if (cur->_private != NULL) {
	    xsltFreeDocumentKeys(cur->_private);
	    xmlFree(cur->_private);
	}
	xmlFreeDoc(cur);
	cur = next;
    }
    ctxt->persistRVT = NULL;
}


static xsltStackElemPtr
xsltNewStackElem(xsltTransformContextPtr ctxt)
{
    xsltStackElemPtr ret;
    if (ctxt && ctxt->cache->stackItems) {
	ret = ctxt->cache->stackItems;
	ctxt->cache->stackItems = ret->next;
	ret->next = NULL;
	ctxt->cache->nbStackItems--;
#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgReusedVars++;
#endif
	return(ret);
    }
    ret = (xsltStackElemPtr) xmlMalloc(sizeof(xsltStackElem));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewStackElem : malloc failed\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltStackElem));
    ret->context = ctxt;
    return(ret);
}

static xsltStackElemPtr
xsltCopyStackElem(xsltStackElemPtr elem) {
    xsltStackElemPtr cur;

    cur = (xsltStackElemPtr) xmlMalloc(sizeof(xsltStackElem));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltCopyStackElem : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltStackElem));
    cur->context = elem->context;
    cur->name = elem->name;
    cur->nameURI = elem->nameURI;
    cur->select = elem->select;
    cur->tree = elem->tree;
    cur->comp = elem->comp;    
    return(cur);
}

static void
xsltFreeStackElem(xsltStackElemPtr elem) {
    if (elem == NULL)
	return;
    if (elem->value != NULL)
	xmlXPathFreeObject(elem->value);
    if (elem->fragment) {
	xmlDocPtr cur;

	while (elem->fragment != NULL) {
	    cur = elem->fragment;
	    elem->fragment = (xmlDocPtr) cur->next;

	    if (elem->context &&
		(cur->psvi == (void *) ((long) 1)))
	    {
		xsltRegisterLocalRVT(elem->context, cur);
	    } else {
		xsltReleaseRVT((xsltTransformContextPtr) elem->context,
		    cur);
	    }	    
	}
    }
    if (elem->context && (elem->context->cache->nbStackItems < 50)) {
	xsltTransformContextPtr ctxt = elem->context;
	memset(elem, 0, sizeof(xsltStackElem));
	elem->context = ctxt;
	elem->next = ctxt->cache->stackItems;
	ctxt->cache->stackItems = elem;	
	ctxt->cache->nbStackItems++;
#ifdef XSLT_DEBUG_PROFILE_CACHE
	ctxt->cache->dbgCachedVars++;
#endif
	return;
    }
    xmlFree(elem);
}

void
xsltFreeStackElemList(xsltStackElemPtr elem) {
    xsltStackElemPtr next;
    
    while (elem != NULL) {
	next = elem->next;
	xsltFreeStackElem(elem);
	elem = next;
    }
}

#if 0 
static int stack_addr = 0;
static int stack_cmp = 0;
#endif

static xsltStackElemPtr
xsltStackLookup(xsltTransformContextPtr ctxt, const xmlChar *name,
	        const xmlChar *nameURI) {
    int i;
    xsltStackElemPtr cur;

    if ((ctxt == NULL) || (name == NULL) || (ctxt->varsNr == 0))
	return(NULL);

    for (i = ctxt->varsNr; i > ctxt->varsBase; i--) {
	cur = ctxt->varsTab[i-1];
	while (cur != NULL) {
	    if ((cur->name == name) && (cur->nameURI == nameURI)) {
#if 0
		stack_addr++;
#endif
		return(cur);
	    }
	    cur = cur->next;
	}
    }

    name = xmlDictLookup(ctxt->dict, name, -1);
    if (nameURI != NULL)
        nameURI = xmlDictLookup(ctxt->dict, nameURI, -1);

    for (i = ctxt->varsNr; i > ctxt->varsBase; i--) {
	cur = ctxt->varsTab[i-1];
	while (cur != NULL) {
	    if ((cur->name == name) && (cur->nameURI == nameURI)) {
#if 0
		stack_cmp++;
#endif
		return(cur);
	    }
	    cur = cur->next;
	}
    }

    return(NULL);
}

static int
xsltCheckStackElem(xsltTransformContextPtr ctxt, const xmlChar *name,
	           const xmlChar *nameURI) {
    xsltStackElemPtr cur;

    if ((ctxt == NULL) || (name == NULL))
	return(-1);

    cur = xsltStackLookup(ctxt, name, nameURI);
    if (cur == NULL)
        return(0);
    if (cur->comp != NULL) {
        if (cur->comp->type == XSLT_FUNC_WITHPARAM)
	    return(3);
	else if (cur->comp->type == XSLT_FUNC_PARAM)
	    return(2);
    }
    
    return(1);
}

static int
xsltAddStackElem(xsltTransformContextPtr ctxt, xsltStackElemPtr elem)
{
    if ((ctxt == NULL) || (elem == NULL))
	return(-1);

    do {
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
	ctxt->varsTab[ctxt->varsNr++] = elem;
	ctxt->vars = elem;
	
	elem = elem->next;
    } while (elem != NULL);
    
    return(0);
}

int
xsltAddStackElemList(xsltTransformContextPtr ctxt, xsltStackElemPtr elems)
{
    return(xsltAddStackElem(ctxt, elems));
}


static xmlXPathObjectPtr
xsltEvalVariable(xsltTransformContextPtr ctxt, xsltStackElemPtr variable,
	         xsltStylePreCompPtr castedComp)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp =
	(xsltStyleItemVariablePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif   
    xmlXPathObjectPtr result = NULL;
    xmlNodePtr oldInst;

    if ((ctxt == NULL) || (variable == NULL))
	return(NULL);

    oldInst = ctxt->inst;

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Evaluating variable '%s'\n", variable->name));
#endif
    if (variable->select != NULL) {
	xmlXPathCompExprPtr xpExpr = NULL;
	xmlDocPtr oldXPDoc;
	xmlNodePtr oldXPContextNode;
	int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
	xmlNsPtr *oldXPNamespaces;
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	xsltStackElemPtr oldVar = ctxt->contextVariable;

	if ((comp != NULL) && (comp->comp != NULL)) {
	    xpExpr = comp->comp;
	} else {
	    xpExpr = xmlXPathCompile(variable->select);
	}
	if (xpExpr == NULL)
	    return(NULL);
	oldXPDoc = xpctxt->doc;
	oldXPContextNode = xpctxt->node;
	oldXPProximityPosition = xpctxt->proximityPosition;
	oldXPContextSize = xpctxt->contextSize;
	oldXPNamespaces = xpctxt->namespaces;
	oldXPNsNr = xpctxt->nsNr;
		
	xpctxt->node = ctxt->node;
	if ((ctxt->node->type != XML_NAMESPACE_DECL) &&
	    ctxt->node->doc)
	    xpctxt->doc = ctxt->node->doc;
	
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

	ctxt->contextVariable = variable;	
	variable->flags |= XSLT_VAR_IN_SELECT;	
	
	result = xmlXPathCompiledEval(xpExpr, xpctxt);

	variable->flags ^= XSLT_VAR_IN_SELECT;
	ctxt->contextVariable = oldVar;

	xpctxt->doc = oldXPDoc;
	xpctxt->node = oldXPContextNode;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->namespaces = oldXPNamespaces;
	xpctxt->nsNr = oldXPNsNr;

	if ((comp == NULL) || (comp->comp == NULL))
	    xmlXPathFreeCompExpr(xpExpr);
	if (result == NULL) {
	    xsltTransformError(ctxt, NULL,
		(comp != NULL) ? comp->inst : NULL,
		"Failed to evaluate the expression of variable '%s'.\n",
		variable->name);
	    ctxt->state = XSLT_STATE_STOPPED;

#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED
	} else {
	    if ((xsltGenericDebugContext == stdout) ||
		(xsltGenericDebugContext == stderr))
		xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
					result, 0);
#endif
#endif
	}
    } else {
	if (variable->tree == NULL) {
	    result = xmlXPathNewCString("");
	} else {	    
	    if (variable->tree) {
		xmlDocPtr container;
		xmlNodePtr oldInsert;
		xmlDocPtr  oldOutput;
		xsltStackElemPtr oldVar = ctxt->contextVariable;

		container = xsltCreateRVT(ctxt);
		if (container == NULL)
		    goto error;
		variable->fragment = container;
		
		oldOutput = ctxt->output;
		oldInsert = ctxt->insert;		
		
		ctxt->output = container;
		ctxt->insert = (xmlNodePtr) container;
		ctxt->contextVariable = variable;
		xsltApplyOneTemplate(ctxt, ctxt->node, variable->tree,
		    NULL, NULL);

		ctxt->contextVariable = oldVar;		
		ctxt->insert = oldInsert;
		ctxt->output = oldOutput;
		
		result = xmlXPathNewValueTree((xmlNodePtr) container);
	    }
	    if (result == NULL) {
		result = xmlXPathNewCString("");
	    } else {
	        result->boolval = 0; 
	    }
#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED

	    if ((xsltGenericDebugContext == stdout) ||
		(xsltGenericDebugContext == stderr))
		xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
					result, 0);
#endif
#endif
	}
    }

error:
    ctxt->inst = oldInst;
    return(result);
}

static xmlXPathObjectPtr
xsltEvalGlobalVariable(xsltStackElemPtr elem, xsltTransformContextPtr ctxt)
{
    xmlXPathObjectPtr result = NULL;
    xmlNodePtr oldInst;
    const xmlChar* oldVarName;

#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((ctxt == NULL) || (elem == NULL))
	return(NULL);
    if (elem->computed)
	return(elem->value);


#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Evaluating global variable %s\n", elem->name));
#endif

#ifdef WITH_DEBUGGER
    if ((ctxt->debugStatus != XSLT_DEBUG_NONE) &&
        elem->comp && elem->comp->inst)
        xslHandleDebugger(elem->comp->inst, NULL, NULL, ctxt);
#endif

    oldInst = ctxt->inst;
    comp = elem->comp;
    oldVarName = elem->name;
    elem->name = xsltComputingGlobalVarMarker;
    if (elem->select != NULL) {		
	xmlXPathCompExprPtr xpExpr = NULL;
	xmlDocPtr oldXPDoc;
	xmlNodePtr oldXPContextNode;
	int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
	xmlNsPtr *oldXPNamespaces;
	xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;

	if ((comp != NULL) && (comp->comp != NULL)) {
	    xpExpr = comp->comp;
	} else {
	    xpExpr = xmlXPathCompile(elem->select);
	}
	if (xpExpr == NULL)
	    goto error;
	
	
	if (comp != NULL)
	    ctxt->inst = comp->inst;
	else
	    ctxt->inst = NULL;
	oldXPDoc = xpctxt->doc;
	oldXPContextNode = xpctxt->node;
	oldXPProximityPosition = xpctxt->proximityPosition;
	oldXPContextSize = xpctxt->contextSize;
	oldXPNamespaces = xpctxt->namespaces;
	oldXPNsNr = xpctxt->nsNr;
		
	xpctxt->node = ctxt->initialContextNode;
	xpctxt->doc = ctxt->initialContextDoc;
	xpctxt->contextSize = 1;
	xpctxt->proximityPosition = 1;
		
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
	
	result = xmlXPathCompiledEval(xpExpr, xpctxt);

	xpctxt->doc = oldXPDoc;
	xpctxt->node = oldXPContextNode;
	xpctxt->contextSize = oldXPContextSize;
	xpctxt->proximityPosition = oldXPProximityPosition;
	xpctxt->namespaces = oldXPNamespaces;
	xpctxt->nsNr = oldXPNsNr;

	if ((comp == NULL) || (comp->comp == NULL))
	    xmlXPathFreeCompExpr(xpExpr);
	if (result == NULL) {
	    if (comp == NULL)
		xsltTransformError(ctxt, NULL, NULL,
		    "Evaluating global variable %s failed\n", elem->name);
	    else
		xsltTransformError(ctxt, NULL, comp->inst,
		    "Evaluating global variable %s failed\n", elem->name);
	    ctxt->state = XSLT_STATE_STOPPED;
#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED
	} else {
	    if ((xsltGenericDebugContext == stdout) ||
		(xsltGenericDebugContext == stderr))
		xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
					result, 0);
#endif
#endif
	}
    } else {
	if (elem->tree == NULL) {
	    result = xmlXPathNewCString("");
	} else {
	    xmlDocPtr container;
	    xmlNodePtr oldInsert;
	    xmlDocPtr  oldOutput, oldXPDoc;	    
	    container = xsltCreateRVT(ctxt);
	    if (container == NULL)
		goto error;
	    xsltRegisterPersistRVT(ctxt, container);	    

	    oldOutput = ctxt->output;
	    oldInsert = ctxt->insert;

	    oldXPDoc = ctxt->xpathCtxt->doc;
	    
	    ctxt->output = container;	    
	    ctxt->insert = (xmlNodePtr) container;

	    ctxt->xpathCtxt->doc = ctxt->initialContextDoc;
	    xsltApplyOneTemplate(ctxt, ctxt->node, elem->tree, NULL, NULL);

	    ctxt->xpathCtxt->doc = oldXPDoc;

	    ctxt->insert = oldInsert;
	    ctxt->output = oldOutput;
	    
	    result = xmlXPathNewValueTree((xmlNodePtr) container);
	    if (result == NULL) {
		result = xmlXPathNewCString("");
	    } else {
	        result->boolval = 0; 
	    }
#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED
	    if ((xsltGenericDebugContext == stdout) ||
		(xsltGenericDebugContext == stderr))
		xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
					result, 0);
#endif
#endif
	}
    }

error:
    elem->name = oldVarName;
    ctxt->inst = oldInst;
    if (result != NULL) {
	elem->value = result;
	elem->computed = 1;
    }
    return(result);
}

int
xsltEvalGlobalVariables(xsltTransformContextPtr ctxt) {
    xsltStackElemPtr elem;
    xsltStylesheetPtr style;    

    if ((ctxt == NULL) || (ctxt->document == NULL))
	return(-1);
 
#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Registering global variables\n"));
#endif
    style = ctxt->style;
    while (style != NULL) {
	elem = style->variables;
	
#ifdef WITH_XSLT_DEBUG_VARIABLE
	if ((style->doc != NULL) && (style->doc->URL != NULL)) {
	    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
			     "Registering global variables from %s\n",
		             style->doc->URL));
	}
#endif

	while (elem != NULL) {
	    xsltStackElemPtr def;

	    def = (xsltStackElemPtr) 
		    xmlHashLookup2(ctxt->globalVars,
		                 elem->name, elem->nameURI);
	    if (def == NULL) {

		def = xsltCopyStackElem(elem);
		xmlHashAddEntry2(ctxt->globalVars,
				 elem->name, elem->nameURI, def);
	    } else if ((elem->comp != NULL) &&
		       (elem->comp->type == XSLT_FUNC_VARIABLE)) {
		if ((elem->comp->inst != NULL) &&
		    (def->comp != NULL) && (def->comp->inst != NULL) &&
		    (elem->comp->inst->doc == def->comp->inst->doc))
		{
		    xsltTransformError(ctxt, style, elem->comp->inst,
			"Global variable %s already defined\n", elem->name);
		    if (style != NULL) style->errors++;
		}
	    }
	    elem = elem->next;
	}

	style = xsltNextImport(style);
    }

    
    xmlHashScan(ctxt->globalVars,
	        (xmlHashScanner) xsltEvalGlobalVariable, ctxt);

    return(0);
}

static int
xsltRegisterGlobalVariable(xsltStylesheetPtr style, const xmlChar *name,
		     const xmlChar *ns_uri, const xmlChar *sel,
		     xmlNodePtr tree, xsltStylePreCompPtr comp,
		     const xmlChar *value) {
    xsltStackElemPtr elem, tmp;
    if (style == NULL)
	return(-1);
    if (name == NULL)
	return(-1);
    if (comp == NULL)
	return(-1);

#ifdef WITH_XSLT_DEBUG_VARIABLE
    if (comp->type == XSLT_FUNC_PARAM)
	xsltGenericDebug(xsltGenericDebugContext,
			 "Defining global param %s\n", name);
    else
	xsltGenericDebug(xsltGenericDebugContext,
			 "Defining global variable %s\n", name);
#endif

    elem = xsltNewStackElem(NULL);
    if (elem == NULL)
	return(-1);
    elem->comp = comp;
    elem->name = xmlDictLookup(style->dict, name, -1);
    elem->select = xmlDictLookup(style->dict, sel, -1);
    if (ns_uri)
	elem->nameURI = xmlDictLookup(style->dict, ns_uri, -1);
    elem->tree = tree;
    tmp = style->variables;
    if (tmp == NULL) {
	elem->next = NULL;
	style->variables = elem;
    } else {
	while (tmp != NULL) {
	    if ((elem->comp->type == XSLT_FUNC_VARIABLE) &&
		(tmp->comp->type == XSLT_FUNC_VARIABLE) &&
		(xmlStrEqual(elem->name, tmp->name)) &&
		((elem->nameURI == tmp->nameURI) ||
		 (xmlStrEqual(elem->nameURI, tmp->nameURI))))
	    {
		xsltTransformError(NULL, style, comp->inst,
		"redefinition of global variable %s\n", elem->name);
		style->errors++;
	    }
	    if (tmp->next == NULL)
	        break;
	    tmp = tmp->next;
	}
	elem->next = NULL;
	tmp->next = elem;
    }
    if (value != NULL) {
	elem->computed = 1;
	elem->value = xmlXPathNewString(value);
    }
    return(0);
}


static
int
xsltProcessUserParamInternal(xsltTransformContextPtr ctxt,
		             const xmlChar * name,
			     const xmlChar * value,
			     int eval) {

    xsltStylesheetPtr style;
    const xmlChar *prefix;
    const xmlChar *href;
    xmlXPathCompExprPtr xpExpr;
    xmlXPathObjectPtr result;
    
    xsltStackElemPtr elem;
    int res;
    void *res_ptr;

    if (ctxt == NULL)
	return(-1);
    if (name == NULL)
	return(0);
    if (value == NULL)
	return(0);

    style = ctxt->style;

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "Evaluating user parameter %s=%s\n", name, value));
#endif


    name = xsltSplitQName(ctxt->dict, name, &prefix);
    href = NULL;
    if (prefix != NULL) {
	xmlNsPtr ns;

	ns = xmlSearchNs(style->doc, xmlDocGetRootElement(style->doc),
			 prefix);
	if (ns == NULL) {
	    xsltTransformError(ctxt, style, NULL,
	    "user param : no namespace bound to prefix %s\n", prefix);
	    href = NULL;
	} else {
	    href = ns->href;
	}
    }

    if (name == NULL)
	return (-1);

    res_ptr = xmlHashLookup2(ctxt->globalVars, name, href);
    if (res_ptr != 0) {
	xsltTransformError(ctxt, style, NULL,
	    "Global parameter %s already defined\n", name);
    }
    if (ctxt->globalVars == NULL)
	ctxt->globalVars = xmlHashCreate(20);

    while (style != NULL) {
        elem = ctxt->style->variables;
	while (elem != NULL) {
	    if ((elem->comp != NULL) &&
	        (elem->comp->type == XSLT_FUNC_VARIABLE) &&
		(xmlStrEqual(elem->name, name)) &&
		(xmlStrEqual(elem->nameURI, href))) {
		return(0);
	    }
            elem = elem->next;
	}
        style = xsltNextImport(style);
    }
    style = ctxt->style;
    elem = NULL;


    result = NULL;
    if (eval != 0) {
        xpExpr = xmlXPathCompile(value);
	if (xpExpr != NULL) {
	    xmlDocPtr oldXPDoc;
	    xmlNodePtr oldXPContextNode;
	    int oldXPProximityPosition, oldXPContextSize, oldXPNsNr;
	    xmlNsPtr *oldXPNamespaces;
	    xmlXPathContextPtr xpctxt = ctxt->xpathCtxt;
	   
	    oldXPDoc = xpctxt->doc;
	    oldXPContextNode = xpctxt->node;
	    oldXPProximityPosition = xpctxt->proximityPosition;
	    oldXPContextSize = xpctxt->contextSize;
	    oldXPNamespaces = xpctxt->namespaces;
	    oldXPNsNr = xpctxt->nsNr;

	    xpctxt->doc = ctxt->initialContextDoc;	    
	    xpctxt->node = ctxt->initialContextNode;	    
	    xpctxt->contextSize = 1;
	    xpctxt->proximityPosition = 1;
	    xpctxt->namespaces = NULL;
	    xpctxt->nsNr = 0;	   
	    
	    result = xmlXPathCompiledEval(xpExpr, xpctxt);
	    
	    xpctxt->doc = oldXPDoc;
	    xpctxt->node = oldXPContextNode;
	    xpctxt->contextSize = oldXPContextSize;
	    xpctxt->proximityPosition = oldXPProximityPosition;
	    xpctxt->namespaces = oldXPNamespaces;
	    xpctxt->nsNr = oldXPNsNr;
	    
	    xmlXPathFreeCompExpr(xpExpr);
	}
	if (result == NULL) {
	    xsltTransformError(ctxt, style, NULL,
		"Evaluating user parameter %s failed\n", name);
	    ctxt->state = XSLT_STATE_STOPPED;
	    return(-1);
	}
    }


#ifdef WITH_XSLT_DEBUG_VARIABLE
#ifdef LIBXML_DEBUG_ENABLED
    if ((xsltGenericDebugContext == stdout) ||
        (xsltGenericDebugContext == stderr))
	    xmlXPathDebugDumpObject((FILE *)xsltGenericDebugContext,
				    result, 0);
#endif
#endif

    elem = xsltNewStackElem(NULL);
    if (elem != NULL) {
	elem->name = name;
	elem->select = xmlDictLookup(ctxt->dict, value, -1);
	if (href != NULL)
	    elem->nameURI = xmlDictLookup(ctxt->dict, href, -1);
	elem->tree = NULL;
	elem->computed = 1;
	if (eval == 0) {
	    elem->value = xmlXPathNewString(value);
	} 
	else {
	    elem->value = result;
	}
    }


    res = xmlHashAddEntry2(ctxt->globalVars, name, href, elem);
    if (res != 0) {
	xsltFreeStackElem(elem);
	xsltTransformError(ctxt, style, NULL,
	    "Global parameter %s already defined\n", name);
    }
    return(0);
}

 
int
xsltEvalUserParams(xsltTransformContextPtr ctxt, const char **params) {
    int indx = 0;
    const xmlChar *name;
    const xmlChar *value;

    if (params == NULL)
	return(0);
    while (params[indx] != NULL) {
	name = (const xmlChar *) params[indx++];
	value = (const xmlChar *) params[indx++];
    	if (xsltEvalOneUserParam(ctxt, name, value) != 0) 
	    return(-1);
    }
    return 0;
}

 
int
xsltQuoteUserParams(xsltTransformContextPtr ctxt, const char **params) {
    int indx = 0;
    const xmlChar *name;
    const xmlChar *value;

    if (params == NULL)
	return(0);
    while (params[indx] != NULL) {
	name = (const xmlChar *) params[indx++];
	value = (const xmlChar *) params[indx++];
    	if (xsltQuoteOneUserParam(ctxt, name, value) != 0) 
	    return(-1);
    }
    return 0;
}


int
xsltEvalOneUserParam(xsltTransformContextPtr ctxt,
    		     const xmlChar * name,
		     const xmlChar * value) {
    return xsltProcessUserParamInternal(ctxt, name, value,
		                        1 );
}


int
xsltQuoteOneUserParam(xsltTransformContextPtr ctxt,
			 const xmlChar * name,
			 const xmlChar * value) {
    return xsltProcessUserParamInternal(ctxt, name, value,
					0 );
}

static xsltStackElemPtr
xsltBuildVariable(xsltTransformContextPtr ctxt,
		  xsltStylePreCompPtr castedComp,
		  xmlNodePtr tree)
{
#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp =
	(xsltStyleBasicItemVariablePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
#endif 
    xsltStackElemPtr elem;

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		     "Building variable %s", comp->name));
    if (comp->select != NULL)
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
			 " select %s", comp->select));
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext, "\n"));
#endif

    elem = xsltNewStackElem(ctxt);
    if (elem == NULL)
	return(NULL);
    elem->comp = (xsltStylePreCompPtr) comp;
    elem->name = comp->name;
    elem->select = comp->select;
    elem->nameURI = comp->ns;
    elem->tree = tree;
    elem->value = xsltEvalVariable(ctxt, elem,
	(xsltStylePreCompPtr) comp);
    if (elem->value != NULL)
	elem->computed = 1;
    return(elem);
}

static int
xsltRegisterVariable(xsltTransformContextPtr ctxt,
		     xsltStylePreCompPtr castedComp,
		     xmlNodePtr tree, int isParam)
{
#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp =
	(xsltStyleBasicItemVariablePtr) castedComp;
#else
    xsltStylePreCompPtr comp = castedComp;
    int present;
#endif
    xsltStackElemPtr variable;    
    
#ifdef XSLT_REFACTORED
#else
    present = xsltCheckStackElem(ctxt, comp->name, comp->ns);
    if (isParam == 0) {	
	if ((present != 0) && (present != 3)) {
	    
	    xsltTransformError(ctxt, NULL, comp->inst,
		"XSLT-variable: Redefinition of variable '%s'.\n", comp->name);
	    return(0);
	}
    } else if (present != 0) {
	if ((present == 1) || (present == 2)) {
	    
	    xsltTransformError(ctxt, NULL, comp->inst,
		"XSLT-param: Redefinition of parameter '%s'.\n", comp->name);
	    return(0);
	}
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		 "param %s defined by caller\n", comp->name));
#endif
	return(0);
    }
#endif 

    variable = xsltBuildVariable(ctxt, (xsltStylePreCompPtr) comp, tree);
    xsltAddStackElem(ctxt, variable);
    return(0);
}

static xmlXPathObjectPtr
xsltGlobalVariableLookup(xsltTransformContextPtr ctxt, const xmlChar *name,
		         const xmlChar *ns_uri) {
    xsltStackElemPtr elem;
    xmlXPathObjectPtr ret = NULL;

    if ((ctxt->xpathCtxt == NULL) || (ctxt->globalVars == NULL))
	return(NULL);
    elem = (xsltStackElemPtr)
	    xmlHashLookup2(ctxt->globalVars, name, ns_uri);
    if (elem == NULL) {
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
			 "global variable not found %s\n", name));
#endif
	return(NULL);
    }
    if (elem->computed == 0) {
	if (elem->name == xsltComputingGlobalVarMarker) {
	    xsltTransformError(ctxt, NULL, elem->comp->inst,
		"Recursive definition of %s\n", name);
	    return(NULL);
	}
	ret = xsltEvalGlobalVariable(elem, ctxt);
    } else
	ret = elem->value;
    return(xmlXPathObjectCopy(ret));
}

xmlXPathObjectPtr
xsltVariableLookup(xsltTransformContextPtr ctxt, const xmlChar *name,
		   const xmlChar *ns_uri) {
    xsltStackElemPtr elem;

    if (ctxt == NULL)
	return(NULL);

    elem = xsltStackLookup(ctxt, name, ns_uri);
    if (elem == NULL) {
	return(xsltGlobalVariableLookup(ctxt, name, ns_uri));
    }
    if (elem->computed == 0) {
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		         "uncomputed variable %s\n", name));
#endif
        elem->value = xsltEvalVariable(ctxt, elem, NULL);
	elem->computed = 1;
    }
    if (elem->value != NULL)
	return(xmlXPathObjectCopy(elem->value));
#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		     "variable not found %s\n", name));
#endif
    return(NULL);
}


xsltStackElemPtr
xsltParseStylesheetCallerParam(xsltTransformContextPtr ctxt, xmlNodePtr inst)
{
#ifdef XSLT_REFACTORED
    xsltStyleBasicItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif
    xmlNodePtr tree = NULL; 
    xsltStackElemPtr param = NULL;
    
    if ((ctxt == NULL) || (inst == NULL))
	return(NULL);

#ifdef XSLT_REFACTORED
    comp = (xsltStyleBasicItemVariablePtr) inst->psvi;
#else
    comp = (xsltStylePreCompPtr) inst->psvi;
#endif
    
    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetCallerParam(): "
	    "The XSLT 'with-param' instruction was not compiled.\n");
        return(NULL);
    }
    if (comp->name == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetCallerParam(): "
	    "XSLT 'with-param': The attribute 'name' was not compiled.\n");
	return(NULL);
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "Handling xsl:with-param %s\n", comp->name));
#endif

    if (comp->select == NULL) {
	tree = inst->children;
    } else {
#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "        select %s\n", comp->select));
#endif
	tree = inst;
    }

    param = xsltBuildVariable(ctxt, (xsltStylePreCompPtr) comp, tree);

    return(param);
}

void
xsltParseGlobalVariable(xsltStylesheetPtr style, xmlNodePtr cur)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((cur == NULL) || (style == NULL))
	return;
    
#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemVariablePtr) cur->psvi;
#else
    xsltStylePreCompute(style, cur);
    comp = (xsltStylePreCompPtr) cur->psvi;
#endif
    if (comp == NULL) {
	xsltTransformError(NULL, style, cur,
	     "xsl:variable : compilation failed\n");
	return;
    }

    if (comp->name == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsl:variable : missing name attribute\n");
	return;
    }

    if (cur->children != NULL) {
#ifdef XSLT_REFACTORED	
        xsltParseSequenceConstructor(XSLT_CCTXT(style), cur->children);
#else
        xsltParseTemplateContent(style, cur);
#endif
    }
#ifdef WITH_XSLT_DEBUG_VARIABLE
    xsltGenericDebug(xsltGenericDebugContext,
	"Registering global variable %s\n", comp->name);
#endif

    xsltRegisterGlobalVariable(style, comp->name, comp->ns,
	comp->select, cur->children, (xsltStylePreCompPtr) comp,
	NULL);
}


void
xsltParseGlobalParam(xsltStylesheetPtr style, xmlNodePtr cur) {
#ifdef XSLT_REFACTORED
    xsltStyleItemParamPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((cur == NULL) || (style == NULL))
	return;
    
#ifdef XSLT_REFACTORED
    comp = (xsltStyleItemParamPtr) cur->psvi;
#else
    xsltStylePreCompute(style, cur);
    comp = (xsltStylePreCompPtr) cur->psvi;
#endif    
    if (comp == NULL) {
	xsltTransformError(NULL, style, cur,
	     "xsl:param : compilation failed\n");
	return;
    }

    if (comp->name == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsl:param : missing name attribute\n");
	return;
    }

    if (cur->children != NULL) {
#ifdef XSLT_REFACTORED	
        xsltParseSequenceConstructor(XSLT_CCTXT(style), cur->children);
#else
        xsltParseTemplateContent(style, cur);
#endif
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    xsltGenericDebug(xsltGenericDebugContext,
	"Registering global param %s\n", comp->name);
#endif

    xsltRegisterGlobalVariable(style, comp->name, comp->ns,
	comp->select, cur->children, (xsltStylePreCompPtr) comp,
	NULL);
}

void
xsltParseStylesheetVariable(xsltTransformContextPtr ctxt, xmlNodePtr inst)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemVariablePtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((inst == NULL) || (ctxt == NULL))
	return;

    comp = inst->psvi;
    if (comp == NULL) {
        xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetVariable(): "
	    "The XSLT 'variable' instruction was not compiled.\n");
        return;
    }
    if (comp->name == NULL) {
	xsltTransformError(ctxt, NULL, inst,
	    "Internal error in xsltParseStylesheetVariable(): "
	    "The attribute 'name' was not compiled.\n");
	return;
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Registering variable '%s'\n", comp->name));
#endif

    xsltRegisterVariable(ctxt, (xsltStylePreCompPtr) comp, inst->children, 0);
}

void
xsltParseStylesheetParam(xsltTransformContextPtr ctxt, xmlNodePtr cur)
{
#ifdef XSLT_REFACTORED
    xsltStyleItemParamPtr comp;
#else
    xsltStylePreCompPtr comp;
#endif

    if ((cur == NULL) || (ctxt == NULL))
	return;

    comp = cur->psvi;
    if ((comp == NULL) || (comp->name == NULL)) {
	xsltTransformError(ctxt, NULL, cur,
	    "Internal error in xsltParseStylesheetParam(): "
	    "The XSLT 'param' declaration was not compiled correctly.\n");
	return;
    }

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(ctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	"Registering param %s\n", comp->name));
#endif

    xsltRegisterVariable(ctxt, (xsltStylePreCompPtr) comp, cur->children, 1);
}


void
xsltFreeGlobalVariables(xsltTransformContextPtr ctxt) {
    xmlHashFree(ctxt->globalVars, (xmlHashDeallocator) xsltFreeStackElem);
}

xmlXPathObjectPtr
xsltXPathVariableLookup(void *ctxt, const xmlChar *name,
	                const xmlChar *ns_uri) {
    xsltTransformContextPtr tctxt;
    xmlXPathObjectPtr valueObj = NULL;

    if ((ctxt == NULL) || (name == NULL))
	return(NULL);

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(((xsltTransformContextPtr)ctxt),XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "Lookup variable '%s'\n", name));
#endif
    
    tctxt = (xsltTransformContextPtr) ctxt;
    if (tctxt->varsNr != 0) {
	int i;
	xsltStackElemPtr variable = NULL, cur;

	for (i = tctxt->varsNr; i > tctxt->varsBase; i--) {
	    cur = tctxt->varsTab[i-1];
	    if ((cur->name == name) && (cur->nameURI == ns_uri)) {
#if 0
		stack_addr++;
#endif
		variable = cur;
		goto local_variable_found;
	    }
	    cur = cur->next;
	}	
	{
	    const xmlChar *tmpName = name, *tmpNsName = ns_uri;

	    name = xmlDictLookup(tctxt->dict, name, -1);
	    if (ns_uri)
		ns_uri = xmlDictLookup(tctxt->dict, ns_uri, -1);
	    if ((tmpName != name) || (tmpNsName != ns_uri)) {		
		for (i = tctxt->varsNr; i > tctxt->varsBase; i--) {
		    cur = tctxt->varsTab[i-1];
		    if ((cur->name == name) && (cur->nameURI == ns_uri)) {
#if 0
			stack_cmp++;
#endif
			variable = cur;
			goto local_variable_found;
		    }
		}
	    }
	}

local_variable_found:

	if (variable) {
	    if (variable->computed == 0) {
		
#ifdef WITH_XSLT_DEBUG_VARIABLE
		XSLT_TRACE(tctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		    "uncomputed variable '%s'\n", name));
#endif
		variable->value = xsltEvalVariable(tctxt, variable, NULL);
		variable->computed = 1;
	    }
	    if (variable->value != NULL) {
		valueObj = xmlXPathObjectCopy(variable->value); 
	    }
	    return(valueObj);
	}
    }
    
    if (tctxt->globalVars) {
	valueObj = xsltGlobalVariableLookup(tctxt, name, ns_uri);
    }

    if (valueObj == NULL) {

#ifdef WITH_XSLT_DEBUG_VARIABLE
    XSLT_TRACE(tctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
		     "variable not found '%s'\n", name));
#endif

	if (ns_uri) {
	    xsltTransformError(tctxt, NULL, tctxt->inst,
		"Variable '{%s}%s' has not been declared.\n", ns_uri, name);
	} else {
	    xsltTransformError(tctxt, NULL, tctxt->inst,
		"Variable '%s' has not been declared.\n", name);
	}
    } else {

#ifdef WITH_XSLT_DEBUG_VARIABLE
	XSLT_TRACE(tctxt,XSLT_TRACE_VARIABLES,xsltGenericDebug(xsltGenericDebugContext,
	    "found variable '%s'\n", name));
#endif
    }

    return(valueObj);
}


