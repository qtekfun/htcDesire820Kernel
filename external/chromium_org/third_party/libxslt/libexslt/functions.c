#define IN_LIBEXSLT
#include "libexslt/libexslt.h"

#if defined(WIN32) && !defined (__CYGWIN__) && (!__MINGW32__)
#include <win32config.h>
#else
#include "config.h"
#endif

#include <string.h>

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/hash.h>
#include <libxml/debugXML.h>

#include <libxslt/xsltutils.h>
#include <libxslt/variables.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>
#include <libxslt/transform.h>
#include <libxslt/imports.h>

#include "exslt.h"

typedef struct _exsltFuncFunctionData exsltFuncFunctionData;
struct _exsltFuncFunctionData {
    int nargs;			
    xmlNodePtr content;		
};

typedef struct _exsltFuncData exsltFuncData;
struct _exsltFuncData {
    xmlHashTablePtr funcs;	
    xmlXPathObjectPtr result;	
    int error;			
    xmlDocPtr RVT;   
};

typedef struct _exsltFuncResultPreComp exsltFuncResultPreComp;
struct _exsltFuncResultPreComp {
    xsltElemPreComp comp;
    xmlXPathCompExprPtr select;
    xmlNsPtr *nsList;
    int nsNr;
};

typedef struct _exsltFuncImportRegData exsltFuncImportRegData;
struct _exsltFuncImportRegData {
    xsltTransformContextPtr ctxt;
    xmlHashTablePtr hash;
};

static void exsltFuncFunctionFunction (xmlXPathParserContextPtr ctxt,
				       int nargs);
static exsltFuncFunctionData *exsltFuncNewFunctionData(void);

#define MAX_FUNC_RECURSION 1000


static void
exsltFuncRegisterFunc (exsltFuncFunctionData *data,
		       xsltTransformContextPtr ctxt,
		       const xmlChar *URI, const xmlChar *name,
		       ATTRIBUTE_UNUSED const xmlChar *ignored) {
    if ((data == NULL) || (ctxt == NULL) || (URI == NULL) || (name == NULL))
	return;

    xsltGenericDebug(xsltGenericDebugContext,
		     "exsltFuncRegisterFunc: register {%s}%s\n",
		     URI, name);
    xsltRegisterExtFunction(ctxt, name, URI,
			    exsltFuncFunctionFunction);
}

static void
exsltFuncRegisterImportFunc (exsltFuncFunctionData *data,
			     exsltFuncImportRegData *ch,
			     const xmlChar *URI, const xmlChar *name,
			     ATTRIBUTE_UNUSED const xmlChar *ignored) {
    exsltFuncFunctionData *func=NULL;

    if ((data == NULL) || (ch == NULL) || (URI == NULL) || (name == NULL))
            return;

    if (ch->ctxt == NULL || ch->hash == NULL)
    	return;

    
    func = (exsltFuncFunctionData*)xmlHashLookup2(ch->hash, URI, name);
    if (func == NULL) {		
    	func = exsltFuncNewFunctionData();
	memcpy(func, data, sizeof(exsltFuncFunctionData));
	if (xmlHashAddEntry2(ch->hash, URI, name, func) < 0) {
	    xsltGenericError(xsltGenericErrorContext,
	    	    "Failed to register function {%s}%s\n",
		    URI, name);
	} else {		
	    xsltGenericDebug(xsltGenericDebugContext,
	            "exsltFuncRegisterImportFunc: register {%s}%s\n",
		    URI, name);
	    xsltRegisterExtFunction(ch->ctxt, name, URI,
		    exsltFuncFunctionFunction);
	}
    }
}

static exsltFuncData *
exsltFuncInit (xsltTransformContextPtr ctxt, const xmlChar *URI) {
    exsltFuncData *ret;
    xsltStylesheetPtr tmp;
    exsltFuncImportRegData ch;
    xmlHashTablePtr hash;
    
    ret = (exsltFuncData *) xmlMalloc (sizeof(exsltFuncData));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncInit: not enough memory\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(exsltFuncData));

    ret->result = NULL;
    ret->error = 0;

    ch.hash = (xmlHashTablePtr) xsltStyleGetExtData(ctxt->style, URI);
    ret->funcs = ch.hash;
    xmlHashScanFull(ch.hash, (xmlHashScannerFull) exsltFuncRegisterFunc, ctxt);
    tmp = ctxt->style;
    ch.ctxt = ctxt;
    while ((tmp=xsltNextImport(tmp))!=NULL) {
	hash = xsltGetExtInfo(tmp, URI);
	if (hash != NULL) {
	    xmlHashScanFull(hash, 
		    (xmlHashScannerFull) exsltFuncRegisterImportFunc, &ch);
	}
    }

    return(ret);
}

static void
exsltFuncShutdown (xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		   const xmlChar *URI ATTRIBUTE_UNUSED,
		   exsltFuncData *data) {
    if (data->result != NULL)
	xmlXPathFreeObject(data->result);
    xmlFree(data);
}

static xmlHashTablePtr
exsltFuncStyleInit (xsltStylesheetPtr style ATTRIBUTE_UNUSED,
		    const xmlChar *URI ATTRIBUTE_UNUSED) {
    return xmlHashCreate(1);
}

static void
exsltFuncStyleShutdown (xsltStylesheetPtr style ATTRIBUTE_UNUSED,
			const xmlChar *URI ATTRIBUTE_UNUSED,
			xmlHashTablePtr data) {
    xmlHashFree(data, (xmlHashDeallocator) xmlFree);
}

static exsltFuncFunctionData *
exsltFuncNewFunctionData (void) {
    exsltFuncFunctionData *ret;

    ret = (exsltFuncFunctionData *) xmlMalloc (sizeof(exsltFuncFunctionData));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncNewFunctionData: not enough memory\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(exsltFuncFunctionData));

    ret->nargs = 0;
    ret->content = NULL;

    return(ret);
}

static void
exsltFreeFuncResultPreComp (exsltFuncResultPreComp *comp) {
    if (comp == NULL)
	return;

    if (comp->select != NULL)
	xmlXPathFreeCompExpr (comp->select);
    if (comp->nsList != NULL)
        xmlFree(comp->nsList);
    xmlFree(comp);
}

static void
exsltFuncFunctionFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr oldResult, ret;
    exsltFuncData *data;
    exsltFuncFunctionData *func;
    xmlNodePtr paramNode, oldInsert, fake;
    int oldBase;
    xsltStackElemPtr params = NULL, param;
    xsltTransformContextPtr tctxt = xsltXPathGetTransformContext(ctxt);
    int i, notSet;
    struct objChain {
	struct objChain *next;
	xmlXPathObjectPtr obj;
    };
    struct objChain	*savedObjChain = NULL, *savedObj;

    data = (exsltFuncData *) xsltGetExtData (tctxt,
					     EXSLT_FUNCTIONS_NAMESPACE);
    oldResult = data->result;
    data->result = NULL;

    func = (exsltFuncFunctionData*) xmlHashLookup2 (data->funcs,
						    ctxt->context->functionURI,
						    ctxt->context->function);

    if (nargs > func->nargs) {
	xsltGenericError(xsltGenericErrorContext,
			 "{%s}%s: called with too many arguments\n",
			 ctxt->context->functionURI, ctxt->context->function);
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }
    if (func->content != NULL) {
	paramNode = func->content->prev;
    }
    else
	paramNode = NULL;
    if ((paramNode == NULL) && (func->nargs != 0)) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncFunctionFunction: nargs != 0 and "
			 "param == NULL\n");
	return;
    }
    if (tctxt->funcLevel > MAX_FUNC_RECURSION) {
	xsltGenericError(xsltGenericErrorContext,
			 "{%s}%s: detected a recursion\n",
			 ctxt->context->functionURI, ctxt->context->function);
	ctxt->error = XPATH_MEMORY_ERROR;
	return;
    }
    tctxt->funcLevel++;

    oldBase = tctxt->varsBase;
    tctxt->varsBase = tctxt->varsNr;
    
    if (paramNode != NULL) {
        
	for (i = 0; i < nargs; i++) {
	    savedObj = xmlMalloc(sizeof(struct objChain));
	    savedObj->next = savedObjChain;
	    savedObj->obj = valuePop(ctxt);
	    savedObjChain = savedObj;
	}

	for (i = 1; i <= func->nargs; i++) {
	    if (paramNode->prev == NULL)
	        break;
	    paramNode = paramNode->prev;
	}
	notSet = func->nargs - nargs;
	for (; i > 0; i--) {
	    param = xsltParseStylesheetCallerParam (tctxt, paramNode);
	    if (i > notSet) {	
		param->computed = 1;
		if (param->value != NULL)
		    xmlXPathFreeObject(param->value);
		savedObj = savedObjChain;	
		param->value = savedObj->obj;
		savedObjChain = savedObjChain->next;
		xmlFree(savedObj);
	    }
	    xsltLocalVariablePush(tctxt, param, -1);
	    param->next = params;
	    params = param;
	    paramNode = paramNode->next;
	}
    }
    fake = xmlNewDocNode(tctxt->output, NULL,
			 (const xmlChar *)"fake", NULL);
    oldInsert = tctxt->insert;
    tctxt->insert = fake;
    xsltApplyOneTemplate (tctxt, xmlXPathGetContextNode(ctxt),
			  func->content, NULL, NULL);
    xsltLocalVariablePop(tctxt, tctxt->varsBase, -2);
    tctxt->insert = oldInsert;
    tctxt->varsBase = oldBase;	
    if (params != NULL)
	xsltFreeStackElemList(params);    

    if (data->error != 0)
	goto error;

    if (data->result != NULL) {
	ret = data->result;
    } else
	ret = xmlXPathNewCString("");

    data->result = oldResult;

    if (fake->children != NULL) {
#ifdef LIBXML_DEBUG_ENABLED
	xmlDebugDumpNode (stderr, fake, 1);
#endif
	xsltGenericError(xsltGenericErrorContext,
			 "{%s}%s: cannot write to result tree while "
			 "executing a function\n",
			 ctxt->context->functionURI, ctxt->context->function);
	xmlFreeNode(fake);
	goto error;
    }
    xmlFreeNode(fake);
    valuePush(ctxt, ret);

error:
    xsltExtensionInstructionResultFinalize(tctxt);
    tctxt->funcLevel--;
}


static void
exsltFuncFunctionComp (xsltStylesheetPtr style, xmlNodePtr inst) {
    xmlChar *name, *prefix;
    xmlNsPtr ns;
    xmlHashTablePtr data;
    exsltFuncFunctionData *func;

    if ((style == NULL) || (inst == NULL))
	return;


    {
	xmlChar *qname;

	qname = xmlGetProp(inst, (const xmlChar *) "name");
	name = xmlSplitQName2 (qname, &prefix);
	xmlFree(qname);
    }
    if ((name == NULL) || (prefix == NULL)) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:function: not a QName\n");
	if (name != NULL)
	    xmlFree(name);
	return;
    }
    
    ns = xmlSearchNs (inst->doc, inst, prefix);
    if (ns == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:function: undeclared prefix %s\n",
			 prefix);
	xmlFree(name);
	xmlFree(prefix);
	return;
    }
    xmlFree(prefix);

    func = exsltFuncNewFunctionData();
    func->content = inst->children;
    while (IS_XSLT_ELEM(func->content) &&
	   IS_XSLT_NAME(func->content, "param")) {
	func->content = func->content->next;
	func->nargs++;
    }

    xsltParseTemplateContent(style, inst);

#ifdef XSLT_REFACTORED
    data = (xmlHashTablePtr)
	xsltStyleStylesheetLevelGetExtData(style,
	    EXSLT_FUNCTIONS_NAMESPACE);
#else
    data = (xmlHashTablePtr)
	xsltStyleGetExtData (style, EXSLT_FUNCTIONS_NAMESPACE);
#endif
    if (data == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncFunctionComp: no stylesheet data\n");
	xmlFree(name);
	return;
    }

    if (xmlHashAddEntry2 (data, ns->href, name, func) < 0) {
	xsltTransformError(NULL, style, inst,
	    "Failed to register function {%s}%s\n",
			 ns->href, name);
	style->errors++;
    } else {
	xsltGenericDebug(xsltGenericDebugContext,
			 "exsltFuncFunctionComp: register {%s}%s\n",
			 ns->href, name);
    }
    xmlFree(name);
}

static xsltElemPreCompPtr
exsltFuncResultComp (xsltStylesheetPtr style, xmlNodePtr inst,
		     xsltTransformFunction function) {
    xmlNodePtr test;
    xmlChar *sel;
    exsltFuncResultPreComp *ret;

    for (test = inst->next; test != NULL; test = test->next) {
	if (test->type != XML_ELEMENT_NODE)
	    continue;
	if (IS_XSLT_ELEM(test) && IS_XSLT_NAME(test, "fallback"))
	    continue;
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncResultElem: only xsl:fallback is "
			 "allowed to follow func:result\n");
	return (NULL);
    }
    for (test = inst->parent; test != NULL; test = test->parent) {
	if (IS_XSLT_ELEM(test) &&
	    IS_XSLT_NAME(test, "stylesheet")) {
	    xsltGenericError(xsltGenericErrorContext,
			     "func:result element not a descendant "
			     "of a func:function\n");
	    return (NULL);
	}
	if ((test->ns != NULL) &&
	    (xmlStrEqual(test->ns->href, EXSLT_FUNCTIONS_NAMESPACE))) {
	    if (xmlStrEqual(test->name, (const xmlChar *) "function")) {
		break;
	    }
	    if (xmlStrEqual(test->name, (const xmlChar *) "result")) {
		xsltGenericError(xsltGenericErrorContext,
				 "func:result element not allowed within"
				 " another func:result element\n");
		return (NULL);
	    }
	}
	if (IS_XSLT_ELEM(test) &&
	    (IS_XSLT_NAME(test, "variable") ||
	     IS_XSLT_NAME(test, "param"))) {
	    xsltGenericError(xsltGenericErrorContext,
			     "func:result element not allowed within"
			     " a variable binding element\n");
	    return (NULL);
	}
    }

    ret = (exsltFuncResultPreComp *)
	xmlMalloc (sizeof(exsltFuncResultPreComp));
    if (ret == NULL) {
	xsltPrintErrorContext(NULL, NULL, NULL);
        xsltGenericError(xsltGenericErrorContext,
                         "exsltFuncResultComp : malloc failed\n");
        return (NULL);
    }
    memset(ret, 0, sizeof(exsltFuncResultPreComp));

    xsltInitElemPreComp ((xsltElemPreCompPtr) ret, style, inst, function,
		 (xsltElemPreCompDeallocator) exsltFreeFuncResultPreComp);
    ret->select = NULL;

    sel = xmlGetNsProp(inst, (const xmlChar *) "select", NULL);
    if (sel != NULL) {
	ret->select = xmlXPathCompile (sel);
	xmlFree(sel);
    }
    ret->nsList = xmlGetNsList(inst->doc, inst);
    if (ret->nsList != NULL) {
        int i = 0;
        while (ret->nsList[i] != NULL)
	    i++;
	ret->nsNr = i;
    }
    return ((xsltElemPreCompPtr) ret);
}

static void
exsltFuncResultElem (xsltTransformContextPtr ctxt,
	             xmlNodePtr node ATTRIBUTE_UNUSED, xmlNodePtr inst,
		     exsltFuncResultPreComp *comp) {
    exsltFuncData *data;
    xmlXPathObjectPtr ret;
    

    data = (exsltFuncData *) xsltGetExtData (ctxt, EXSLT_FUNCTIONS_NAMESPACE);
    if (data == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltFuncReturnElem: data == NULL\n");
	return;
    }
    if (data->result != NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "func:result already instanciated\n");
	data->error = 1;
	return;
    }
    if (comp->select != NULL) {
	xmlNsPtr *oldXPNsList;
	int oldXPNsNr;
	xmlNodePtr oldXPContextNode;
	if (inst->children != NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "func:result content must be empty if"
			     " the function has a select attribute\n");
	    data->error = 1;
	    return;
	}
	oldXPNsList = ctxt->xpathCtxt->namespaces;
	oldXPNsNr = ctxt->xpathCtxt->nsNr;
	oldXPContextNode = ctxt->xpathCtxt->node;

	ctxt->xpathCtxt->namespaces = comp->nsList;
	ctxt->xpathCtxt->nsNr = comp->nsNr;

	ret = xmlXPathCompiledEval(comp->select, ctxt->xpathCtxt);

	ctxt->xpathCtxt->node = oldXPContextNode;
	ctxt->xpathCtxt->nsNr = oldXPNsNr;
	ctxt->xpathCtxt->namespaces = oldXPNsList;

	if (ret == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: ret == NULL\n");
	    return;
	}
	xsltExtensionInstructionResultRegister(ctxt, ret);
    } else if (inst->children != NULL) {
	xmlNodePtr oldInsert;
	xmlDocPtr container;

	container = xsltCreateRVT(ctxt);
	if (container == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: out of memory\n");
	    data->error = 1;
	    return;
	}
	xsltRegisterLocalRVT(ctxt, container);	

	oldInsert = ctxt->insert;
	ctxt->insert = (xmlNodePtr) container;
	xsltApplyOneTemplate (ctxt, ctxt->xpathCtxt->node,
			      inst->children, NULL, NULL);
	ctxt->insert = oldInsert;

	ret = xmlXPathNewValueTree((xmlNodePtr) container);
	if (ret == NULL) {
	    xsltGenericError(xsltGenericErrorContext,
			     "exsltFuncResultElem: ret == NULL\n");
	    data->error = 1;
	} else {
	    ret->boolval = 0; 
	    xsltExtensionInstructionResultRegister(ctxt, ret);
	}
    } else {
	ret = xmlXPathNewCString("");
    }
    data->result = ret;
}

void
exsltFuncRegister (void) {
    xsltRegisterExtModuleFull (EXSLT_FUNCTIONS_NAMESPACE,
		       (xsltExtInitFunction) exsltFuncInit,
		       (xsltExtShutdownFunction) exsltFuncShutdown,
		       (xsltStyleExtInitFunction) exsltFuncStyleInit,
		       (xsltStyleExtShutdownFunction) exsltFuncStyleShutdown);

    xsltRegisterExtModuleTopLevel ((const xmlChar *) "function",
				   EXSLT_FUNCTIONS_NAMESPACE,
				   exsltFuncFunctionComp);
    xsltRegisterExtModuleElement ((const xmlChar *) "result",
			  EXSLT_FUNCTIONS_NAMESPACE,
			  (xsltPreComputeFunction)exsltFuncResultComp,
			  (xsltTransformFunction) exsltFuncResultElem);
}
