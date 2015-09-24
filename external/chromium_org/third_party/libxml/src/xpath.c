/*
 * xpath.c: XML Path Language implementation
 *          XPath is a language for addressing parts of an XML document,
 *          designed to be used by both XSLT and XPointer
 *f
 * Reference: W3C Recommendation 16 November 1999
 *     http://www.w3.org/TR/1999/REC-xpath-19991116
 * Public reference:
 *     http://www.w3.org/TR/xpath
 *
 * See Copyright for the status of this software
 *
 * Author: daniel@veillard.com
 *
 */

#define IN_LIBXML
#include "libxml.h"

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
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/valid.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>
#include <libxml/hash.h>
#ifdef LIBXML_XPTR_ENABLED
#include <libxml/xpointer.h>
#endif
#ifdef LIBXML_DEBUG_ENABLED
#include <libxml/debugXML.h>
#endif
#include <libxml/xmlerror.h>
#include <libxml/threads.h>
#include <libxml/globals.h>
#ifdef LIBXML_PATTERN_ENABLED
#include <libxml/pattern.h>
#endif

#ifdef LIBXML_PATTERN_ENABLED
#define XPATH_STREAMING
#endif

#define TODO								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);

#define XP_OPTIMIZED_NON_ELEM_COMPARISON

#define XP_OPTIMIZED_FILTER_FIRST



#if defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)


#ifndef TRIO_REPLACE_STDIO
#define TRIO_PUBLIC static
#endif
#include "trionan.c"

double xmlXPathNAN = 0;
double xmlXPathPINF = 1;
double xmlXPathNINF = -1;
static double xmlXPathNZERO = 0; 
static int xmlXPathInitialized = 0;

void
xmlXPathInit(void) {
    if (xmlXPathInitialized) return;

    xmlXPathPINF = trio_pinf();
    xmlXPathNINF = trio_ninf();
    xmlXPathNAN = trio_nan();
    xmlXPathNZERO = trio_nzero();

    xmlXPathInitialized = 1;
}

int
xmlXPathIsNaN(double val) {
    return(trio_isnan(val));
}

int
xmlXPathIsInf(double val) {
    return(trio_isinf(val));
}

#endif 
#ifdef LIBXML_XPATH_ENABLED
static int
xmlXPathGetSign(double val) {
    return(trio_signbit(val));
}


#ifdef DEBUG_XPATH_EXPRESSION
#define DEBUG_STEP
#define DEBUG_EXPR
#define DEBUG_EVAL_COUNTS
#endif

static xmlNs xmlXPathXMLNamespaceStruct = {
    NULL,
    XML_NAMESPACE_DECL,
    XML_XML_NAMESPACE,
    BAD_CAST "xml",
    NULL,
    NULL
};
static xmlNsPtr xmlXPathXMLNamespace = &xmlXPathXMLNamespaceStruct;
#ifndef LIBXML_THREAD_ENABLED
static int xmlXPathDisableOptimizer = 0;
#endif


#define XP_ERRORNULL(X)							\
    { xmlXPathErr(ctxt, X); return(NULL); }

static const char *xmlXPathErrorMessages[] = {
    "Ok\n",
    "Number encoding\n",
    "Unfinished literal\n",
    "Start of literal\n",
    "Expected $ for variable reference\n",
    "Undefined variable\n",
    "Invalid predicate\n",
    "Invalid expression\n",
    "Missing closing curly brace\n",
    "Unregistered function\n",
    "Invalid operand\n",
    "Invalid type\n",
    "Invalid number of arguments\n",
    "Invalid context size\n",
    "Invalid context position\n",
    "Memory allocation error\n",
    "Syntax error\n",
    "Resource error\n",
    "Sub resource error\n",
    "Undefined namespace prefix\n",
    "Encoding error\n",
    "Char out of XML range\n",
    "Invalid or incomplete context\n",
    "?? Unknown error ??\n"	
};
#define MAXERRNO ((int)(sizeof(xmlXPathErrorMessages) /	\
		   sizeof(xmlXPathErrorMessages[0])) - 1)
static void
xmlXPathErrMemory(xmlXPathContextPtr ctxt, const char *extra)
{
    if (ctxt != NULL) {
        if (extra) {
            xmlChar buf[200];

            xmlStrPrintf(buf, 200,
                         BAD_CAST "Memory allocation failed : %s\n",
                         extra);
            ctxt->lastError.message = (char *) xmlStrdup(buf);
        } else {
            ctxt->lastError.message = (char *)
	       xmlStrdup(BAD_CAST "Memory allocation failed\n");
        }
        ctxt->lastError.domain = XML_FROM_XPATH;
        ctxt->lastError.code = XML_ERR_NO_MEMORY;
	if (ctxt->error != NULL)
	    ctxt->error(ctxt->userData, &ctxt->lastError);
    } else {
        if (extra)
            __xmlRaiseError(NULL, NULL, NULL,
                            NULL, NULL, XML_FROM_XPATH,
                            XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0,
                            extra, NULL, NULL, 0, 0,
                            "Memory allocation failed : %s\n", extra);
        else
            __xmlRaiseError(NULL, NULL, NULL,
                            NULL, NULL, XML_FROM_XPATH,
                            XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0,
                            NULL, NULL, NULL, 0, 0,
                            "Memory allocation failed\n");
    }
}

static void
xmlXPathPErrMemory(xmlXPathParserContextPtr ctxt, const char *extra)
{
    if (ctxt == NULL)
	xmlXPathErrMemory(NULL, extra);
    else {
	ctxt->error = XPATH_MEMORY_ERROR;
	xmlXPathErrMemory(ctxt->context, extra);
    }
}

void
xmlXPathErr(xmlXPathParserContextPtr ctxt, int error)
{
    if ((error < 0) || (error > MAXERRNO))
	error = MAXERRNO;
    if (ctxt == NULL) {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, NULL, XML_FROM_XPATH,
			error + XML_XPATH_EXPRESSION_OK - XPATH_EXPRESSION_OK,
			XML_ERR_ERROR, NULL, 0,
			NULL, NULL, NULL, 0, 0,
			"%s", xmlXPathErrorMessages[error]);
	return;
    }
    ctxt->error = error;
    if (ctxt->context == NULL) {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, NULL, XML_FROM_XPATH,
			error + XML_XPATH_EXPRESSION_OK - XPATH_EXPRESSION_OK,
			XML_ERR_ERROR, NULL, 0,
			(const char *) ctxt->base, NULL, NULL,
			ctxt->cur - ctxt->base, 0,
			"%s", xmlXPathErrorMessages[error]);
	return;
    }

    
    xmlResetError(&ctxt->context->lastError);

    ctxt->context->lastError.domain = XML_FROM_XPATH;
    ctxt->context->lastError.code = error + XML_XPATH_EXPRESSION_OK -
                           XPATH_EXPRESSION_OK;
    ctxt->context->lastError.level = XML_ERR_ERROR;
    ctxt->context->lastError.str1 = (char *) xmlStrdup(ctxt->base);
    ctxt->context->lastError.int1 = ctxt->cur - ctxt->base;
    ctxt->context->lastError.node = ctxt->context->debugNode;
    if (ctxt->context->error != NULL) {
	ctxt->context->error(ctxt->context->userData,
	                     &ctxt->context->lastError);
    } else {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, ctxt->context->debugNode, XML_FROM_XPATH,
			error + XML_XPATH_EXPRESSION_OK - XPATH_EXPRESSION_OK,
			XML_ERR_ERROR, NULL, 0,
			(const char *) ctxt->base, NULL, NULL,
			ctxt->cur - ctxt->base, 0,
			"%s", xmlXPathErrorMessages[error]);
    }

}

void
xmlXPatherror(xmlXPathParserContextPtr ctxt, const char *file ATTRIBUTE_UNUSED,
              int line ATTRIBUTE_UNUSED, int no) {
    xmlXPathErr(ctxt, no);
}


typedef struct _xmlPointerList xmlPointerList;
typedef xmlPointerList *xmlPointerListPtr;
struct _xmlPointerList {
    void **items;
    int number;
    int size;
};
static int
xmlPointerListAddSize(xmlPointerListPtr list,
		       void *item,
		       int initialSize)
{
    if (list->items == NULL) {
	if (initialSize <= 0)
	    initialSize = 1;
	list->items = (void **) xmlMalloc(
	    initialSize * sizeof(void *));
	if (list->items == NULL) {
	    xmlXPathErrMemory(NULL,
		"xmlPointerListCreate: allocating item\n");
	    return(-1);
	}
	list->number = 0;
	list->size = initialSize;
    } else if (list->size <= list->number) {
	list->size *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->size * sizeof(void *));
	if (list->items == NULL) {
	    xmlXPathErrMemory(NULL,
		"xmlPointerListCreate: re-allocating item\n");
	    list->size = 0;
	    return(-1);
	}
    }
    list->items[list->number++] = item;
    return(0);
}

static xmlPointerListPtr
xmlPointerListCreate(int initialSize)
{
    xmlPointerListPtr ret;

    ret = xmlMalloc(sizeof(xmlPointerList));
    if (ret == NULL) {
	xmlXPathErrMemory(NULL,
	    "xmlPointerListCreate: allocating item\n");
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlPointerList));
    if (initialSize > 0) {
	xmlPointerListAddSize(ret, NULL, initialSize);
	ret->number = 0;
    }
    return (ret);
}

static void
xmlPointerListFree(xmlPointerListPtr list)
{
    if (list == NULL)
	return;
    if (list->items != NULL)
	xmlFree(list->items);
    xmlFree(list);
}



typedef enum {
    XPATH_OP_END=0,
    XPATH_OP_AND,
    XPATH_OP_OR,
    XPATH_OP_EQUAL,
    XPATH_OP_CMP,
    XPATH_OP_PLUS,
    XPATH_OP_MULT,
    XPATH_OP_UNION,
    XPATH_OP_ROOT,
    XPATH_OP_NODE,
    XPATH_OP_RESET, 
    XPATH_OP_COLLECT,
    XPATH_OP_VALUE, 
    XPATH_OP_VARIABLE,
    XPATH_OP_FUNCTION,
    XPATH_OP_ARG,
    XPATH_OP_PREDICATE,
    XPATH_OP_FILTER, 
    XPATH_OP_SORT 
#ifdef LIBXML_XPTR_ENABLED
    ,XPATH_OP_RANGETO
#endif
} xmlXPathOp;

typedef enum {
    AXIS_ANCESTOR = 1,
    AXIS_ANCESTOR_OR_SELF,
    AXIS_ATTRIBUTE,
    AXIS_CHILD,
    AXIS_DESCENDANT,
    AXIS_DESCENDANT_OR_SELF,
    AXIS_FOLLOWING,
    AXIS_FOLLOWING_SIBLING,
    AXIS_NAMESPACE,
    AXIS_PARENT,
    AXIS_PRECEDING,
    AXIS_PRECEDING_SIBLING,
    AXIS_SELF
} xmlXPathAxisVal;

typedef enum {
    NODE_TEST_NONE = 0,
    NODE_TEST_TYPE = 1,
    NODE_TEST_PI = 2,
    NODE_TEST_ALL = 3,
    NODE_TEST_NS = 4,
    NODE_TEST_NAME = 5
} xmlXPathTestVal;

typedef enum {
    NODE_TYPE_NODE = 0,
    NODE_TYPE_COMMENT = XML_COMMENT_NODE,
    NODE_TYPE_TEXT = XML_TEXT_NODE,
    NODE_TYPE_PI = XML_PI_NODE
} xmlXPathTypeVal;

#define XP_REWRITE_DOS_CHILD_ELEM 1

typedef struct _xmlXPathStepOp xmlXPathStepOp;
typedef xmlXPathStepOp *xmlXPathStepOpPtr;
struct _xmlXPathStepOp {
    xmlXPathOp op;		
    int ch1;			
    int ch2;			
    int value;
    int value2;
    int value3;
    void *value4;
    void *value5;
    void *cache;
    void *cacheURI;
    int rewriteType;
};

struct _xmlXPathCompExpr {
    int nbStep;			
    int maxStep;		
    xmlXPathStepOp *steps;	
    int last;			
    xmlChar *expr;		
    xmlDictPtr dict;		
#ifdef DEBUG_EVAL_COUNTS
    int nb;
    xmlChar *string;
#endif
#ifdef XPATH_STREAMING
    xmlPatternPtr stream;
#endif
};

static void
xmlXPathFreeValueTree(xmlNodeSetPtr obj);
static void
xmlXPathReleaseObject(xmlXPathContextPtr ctxt, xmlXPathObjectPtr obj);
static int
xmlXPathCompOpEvalFirst(xmlXPathParserContextPtr ctxt,
                        xmlXPathStepOpPtr op, xmlNodePtr *first);
static int
xmlXPathCompOpEvalToBoolean(xmlXPathParserContextPtr ctxt,
			    xmlXPathStepOpPtr op,
			    int isPredicate);


static xmlXPathCompExprPtr
xmlXPathNewCompExpr(void) {
    xmlXPathCompExprPtr cur;

    cur = (xmlXPathCompExprPtr) xmlMalloc(sizeof(xmlXPathCompExpr));
    if (cur == NULL) {
        xmlXPathErrMemory(NULL, "allocating component\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlXPathCompExpr));
    cur->maxStep = 10;
    cur->nbStep = 0;
    cur->steps = (xmlXPathStepOp *) xmlMalloc(cur->maxStep *
	                                   sizeof(xmlXPathStepOp));
    if (cur->steps == NULL) {
        xmlXPathErrMemory(NULL, "allocating steps\n");
	xmlFree(cur);
	return(NULL);
    }
    memset(cur->steps, 0, cur->maxStep * sizeof(xmlXPathStepOp));
    cur->last = -1;
#ifdef DEBUG_EVAL_COUNTS
    cur->nb = 0;
#endif
    return(cur);
}

void
xmlXPathFreeCompExpr(xmlXPathCompExprPtr comp)
{
    xmlXPathStepOpPtr op;
    int i;

    if (comp == NULL)
        return;
    if (comp->dict == NULL) {
	for (i = 0; i < comp->nbStep; i++) {
	    op = &comp->steps[i];
	    if (op->value4 != NULL) {
		if (op->op == XPATH_OP_VALUE)
		    xmlXPathFreeObject(op->value4);
		else
		    xmlFree(op->value4);
	    }
	    if (op->value5 != NULL)
		xmlFree(op->value5);
	}
    } else {
	for (i = 0; i < comp->nbStep; i++) {
	    op = &comp->steps[i];
	    if (op->value4 != NULL) {
		if (op->op == XPATH_OP_VALUE)
		    xmlXPathFreeObject(op->value4);
	    }
	}
        xmlDictFree(comp->dict);
    }
    if (comp->steps != NULL) {
        xmlFree(comp->steps);
    }
#ifdef DEBUG_EVAL_COUNTS
    if (comp->string != NULL) {
        xmlFree(comp->string);
    }
#endif
#ifdef XPATH_STREAMING
    if (comp->stream != NULL) {
        xmlFreePatternList(comp->stream);
    }
#endif
    if (comp->expr != NULL) {
        xmlFree(comp->expr);
    }

    xmlFree(comp);
}

static int
xmlXPathCompExprAdd(xmlXPathCompExprPtr comp, int ch1, int ch2,
   xmlXPathOp op, int value,
   int value2, int value3, void *value4, void *value5) {
    if (comp->nbStep >= comp->maxStep) {
	xmlXPathStepOp *real;

	comp->maxStep *= 2;
	real = (xmlXPathStepOp *) xmlRealloc(comp->steps,
		                      comp->maxStep * sizeof(xmlXPathStepOp));
	if (real == NULL) {
	    comp->maxStep /= 2;
	    xmlXPathErrMemory(NULL, "adding step\n");
	    return(-1);
	}
	comp->steps = real;
    }
    comp->last = comp->nbStep;
    comp->steps[comp->nbStep].rewriteType = 0;
    comp->steps[comp->nbStep].ch1 = ch1;
    comp->steps[comp->nbStep].ch2 = ch2;
    comp->steps[comp->nbStep].op = op;
    comp->steps[comp->nbStep].value = value;
    comp->steps[comp->nbStep].value2 = value2;
    comp->steps[comp->nbStep].value3 = value3;
    if ((comp->dict != NULL) &&
        ((op == XPATH_OP_FUNCTION) || (op == XPATH_OP_VARIABLE) ||
	 (op == XPATH_OP_COLLECT))) {
        if (value4 != NULL) {
	    comp->steps[comp->nbStep].value4 = (xmlChar *)
	        (void *)xmlDictLookup(comp->dict, value4, -1);
	    xmlFree(value4);
	} else
	    comp->steps[comp->nbStep].value4 = NULL;
        if (value5 != NULL) {
	    comp->steps[comp->nbStep].value5 = (xmlChar *)
	        (void *)xmlDictLookup(comp->dict, value5, -1);
	    xmlFree(value5);
	} else
	    comp->steps[comp->nbStep].value5 = NULL;
    } else {
	comp->steps[comp->nbStep].value4 = value4;
	comp->steps[comp->nbStep].value5 = value5;
    }
    comp->steps[comp->nbStep].cache = NULL;
    return(comp->nbStep++);
}

static void
xmlXPathCompSwap(xmlXPathStepOpPtr op) {
    int tmp;

#ifndef LIBXML_THREAD_ENABLED
    if (xmlXPathDisableOptimizer)
	return;
#endif

    tmp = op->ch1;
    op->ch1 = op->ch2;
    op->ch2 = tmp;
}

#define PUSH_FULL_EXPR(op, op1, op2, val, val2, val3, val4, val5)	\
    xmlXPathCompExprAdd(ctxt->comp, (op1), (op2),			\
	                (op), (val), (val2), (val3), (val4), (val5))
#define PUSH_LONG_EXPR(op, val, val2, val3, val4, val5)			\
    xmlXPathCompExprAdd(ctxt->comp, ctxt->comp->last, -1,		\
	                (op), (val), (val2), (val3), (val4), (val5))

#define PUSH_LEAVE_EXPR(op, val, val2)					\
xmlXPathCompExprAdd(ctxt->comp, -1, -1, (op), (val), (val2), 0 ,NULL ,NULL)

#define PUSH_UNARY_EXPR(op, ch, val, val2)				\
xmlXPathCompExprAdd(ctxt->comp, (ch), -1, (op), (val), (val2), 0 ,NULL ,NULL)

#define PUSH_BINARY_EXPR(op, ch1, ch2, val, val2)			\
xmlXPathCompExprAdd(ctxt->comp, (ch1), (ch2), (op),			\
			(val), (val2), 0 ,NULL ,NULL)



#define XP_HAS_CACHE(c) ((c != NULL) && ((c)->cache != NULL))

typedef struct _xmlXPathContextCache xmlXPathContextCache;
typedef xmlXPathContextCache *xmlXPathContextCachePtr;
struct _xmlXPathContextCache {
    xmlPointerListPtr nodesetObjs;  
    xmlPointerListPtr stringObjs;   
    xmlPointerListPtr booleanObjs;  
    xmlPointerListPtr numberObjs;   
    xmlPointerListPtr miscObjs;     
    int maxNodeset;
    int maxString;
    int maxBoolean;
    int maxNumber;
    int maxMisc;
#ifdef XP_DEBUG_OBJ_USAGE
    int dbgCachedAll;
    int dbgCachedNodeset;
    int dbgCachedString;
    int dbgCachedBool;
    int dbgCachedNumber;
    int dbgCachedPoint;
    int dbgCachedRange;
    int dbgCachedLocset;
    int dbgCachedUsers;
    int dbgCachedXSLTTree;
    int dbgCachedUndefined;


    int dbgReusedAll;
    int dbgReusedNodeset;
    int dbgReusedString;
    int dbgReusedBool;
    int dbgReusedNumber;
    int dbgReusedPoint;
    int dbgReusedRange;
    int dbgReusedLocset;
    int dbgReusedUsers;
    int dbgReusedXSLTTree;
    int dbgReusedUndefined;

#endif
};


#define STRANGE							\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Internal error at %s:%d\n",				\
            __FILE__, __LINE__);

#ifdef LIBXML_DEBUG_ENABLED
static void
xmlXPathDebugDumpNode(FILE *output, xmlNodePtr cur, int depth) {
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;
    if (cur == NULL) {
	fprintf(output, "%s", shift);
	fprintf(output, "Node is NULL !\n");
	return;

    }

    if ((cur->type == XML_DOCUMENT_NODE) ||
	     (cur->type == XML_HTML_DOCUMENT_NODE)) {
	fprintf(output, "%s", shift);
	fprintf(output, " /\n");
    } else if (cur->type == XML_ATTRIBUTE_NODE)
	xmlDebugDumpAttr(output, (xmlAttrPtr)cur, depth);
    else
	xmlDebugDumpOneNode(output, cur, depth);
}
static void
xmlXPathDebugDumpNodeList(FILE *output, xmlNodePtr cur, int depth) {
    xmlNodePtr tmp;
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;
    if (cur == NULL) {
	fprintf(output, "%s", shift);
	fprintf(output, "Node is NULL !\n");
	return;

    }

    while (cur != NULL) {
	tmp = cur;
	cur = cur->next;
	xmlDebugDumpOneNode(output, tmp, depth);
    }
}

static void
xmlXPathDebugDumpNodeSet(FILE *output, xmlNodeSetPtr cur, int depth) {
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;

    if (cur == NULL) {
	fprintf(output, "%s", shift);
	fprintf(output, "NodeSet is NULL !\n");
	return;

    }

    if (cur != NULL) {
	fprintf(output, "Set contains %d nodes:\n", cur->nodeNr);
	for (i = 0;i < cur->nodeNr;i++) {
	    fprintf(output, "%s", shift);
	    fprintf(output, "%d", i + 1);
	    xmlXPathDebugDumpNode(output, cur->nodeTab[i], depth + 1);
	}
    }
}

static void
xmlXPathDebugDumpValueTree(FILE *output, xmlNodeSetPtr cur, int depth) {
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;

    if ((cur == NULL) || (cur->nodeNr == 0) || (cur->nodeTab[0] == NULL)) {
	fprintf(output, "%s", shift);
	fprintf(output, "Value Tree is NULL !\n");
	return;

    }

    fprintf(output, "%s", shift);
    fprintf(output, "%d", i + 1);
    xmlXPathDebugDumpNodeList(output, cur->nodeTab[0]->children, depth + 1);
}
#if defined(LIBXML_XPTR_ENABLED)
static void
xmlXPathDebugDumpLocationSet(FILE *output, xmlLocationSetPtr cur, int depth) {
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;

    if (cur == NULL) {
	fprintf(output, "%s", shift);
	fprintf(output, "LocationSet is NULL !\n");
	return;

    }

    for (i = 0;i < cur->locNr;i++) {
	fprintf(output, "%s", shift);
        fprintf(output, "%d : ", i + 1);
	xmlXPathDebugDumpObject(output, cur->locTab[i], depth + 1);
    }
}
#endif 

void
xmlXPathDebugDumpObject(FILE *output, xmlXPathObjectPtr cur, int depth) {
    int i;
    char shift[100];

    if (output == NULL) return;

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;


    fprintf(output, "%s", shift);

    if (cur == NULL) {
        fprintf(output, "Object is empty (NULL)\n");
	return;
    }
    switch(cur->type) {
        case XPATH_UNDEFINED:
	    fprintf(output, "Object is uninitialized\n");
	    break;
        case XPATH_NODESET:
	    fprintf(output, "Object is a Node Set :\n");
	    xmlXPathDebugDumpNodeSet(output, cur->nodesetval, depth);
	    break;
	case XPATH_XSLT_TREE:
	    fprintf(output, "Object is an XSLT value tree :\n");
	    xmlXPathDebugDumpValueTree(output, cur->nodesetval, depth);
	    break;
        case XPATH_BOOLEAN:
	    fprintf(output, "Object is a Boolean : ");
	    if (cur->boolval) fprintf(output, "true\n");
	    else fprintf(output, "false\n");
	    break;
        case XPATH_NUMBER:
	    switch (xmlXPathIsInf(cur->floatval)) {
	    case 1:
		fprintf(output, "Object is a number : Infinity\n");
		break;
	    case -1:
		fprintf(output, "Object is a number : -Infinity\n");
		break;
	    default:
		if (xmlXPathIsNaN(cur->floatval)) {
		    fprintf(output, "Object is a number : NaN\n");
		} else if (cur->floatval == 0 && xmlXPathGetSign(cur->floatval) != 0) {
		    fprintf(output, "Object is a number : 0\n");
		} else {
		    fprintf(output, "Object is a number : %0g\n", cur->floatval);
		}
	    }
	    break;
        case XPATH_STRING:
	    fprintf(output, "Object is a string : ");
	    xmlDebugDumpString(output, cur->stringval);
	    fprintf(output, "\n");
	    break;
	case XPATH_POINT:
	    fprintf(output, "Object is a point : index %d in node", cur->index);
	    xmlXPathDebugDumpNode(output, (xmlNodePtr) cur->user, depth + 1);
	    fprintf(output, "\n");
	    break;
	case XPATH_RANGE:
	    if ((cur->user2 == NULL) ||
		((cur->user2 == cur->user) && (cur->index == cur->index2))) {
		fprintf(output, "Object is a collapsed range :\n");
		fprintf(output, "%s", shift);
		if (cur->index >= 0)
		    fprintf(output, "index %d in ", cur->index);
		fprintf(output, "node\n");
		xmlXPathDebugDumpNode(output, (xmlNodePtr) cur->user,
			              depth + 1);
	    } else  {
		fprintf(output, "Object is a range :\n");
		fprintf(output, "%s", shift);
		fprintf(output, "From ");
		if (cur->index >= 0)
		    fprintf(output, "index %d in ", cur->index);
		fprintf(output, "node\n");
		xmlXPathDebugDumpNode(output, (xmlNodePtr) cur->user,
			              depth + 1);
		fprintf(output, "%s", shift);
		fprintf(output, "To ");
		if (cur->index2 >= 0)
		    fprintf(output, "index %d in ", cur->index2);
		fprintf(output, "node\n");
		xmlXPathDebugDumpNode(output, (xmlNodePtr) cur->user2,
			              depth + 1);
		fprintf(output, "\n");
	    }
	    break;
	case XPATH_LOCATIONSET:
#if defined(LIBXML_XPTR_ENABLED)
	    fprintf(output, "Object is a Location Set:\n");
	    xmlXPathDebugDumpLocationSet(output,
		    (xmlLocationSetPtr) cur->user, depth);
#endif
	    break;
	case XPATH_USERS:
	    fprintf(output, "Object is user defined\n");
	    break;
    }
}

static void
xmlXPathDebugDumpStepOp(FILE *output, xmlXPathCompExprPtr comp,
	                     xmlXPathStepOpPtr op, int depth) {
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;

    fprintf(output, "%s", shift);
    if (op == NULL) {
	fprintf(output, "Step is NULL\n");
	return;
    }
    switch (op->op) {
        case XPATH_OP_END:
	    fprintf(output, "END"); break;
        case XPATH_OP_AND:
	    fprintf(output, "AND"); break;
        case XPATH_OP_OR:
	    fprintf(output, "OR"); break;
        case XPATH_OP_EQUAL:
	     if (op->value)
		 fprintf(output, "EQUAL =");
	     else
		 fprintf(output, "EQUAL !=");
	     break;
        case XPATH_OP_CMP:
	     if (op->value)
		 fprintf(output, "CMP <");
	     else
		 fprintf(output, "CMP >");
	     if (!op->value2)
		 fprintf(output, "=");
	     break;
        case XPATH_OP_PLUS:
	     if (op->value == 0)
		 fprintf(output, "PLUS -");
	     else if (op->value == 1)
		 fprintf(output, "PLUS +");
	     else if (op->value == 2)
		 fprintf(output, "PLUS unary -");
	     else if (op->value == 3)
		 fprintf(output, "PLUS unary - -");
	     break;
        case XPATH_OP_MULT:
	     if (op->value == 0)
		 fprintf(output, "MULT *");
	     else if (op->value == 1)
		 fprintf(output, "MULT div");
	     else
		 fprintf(output, "MULT mod");
	     break;
        case XPATH_OP_UNION:
	     fprintf(output, "UNION"); break;
        case XPATH_OP_ROOT:
	     fprintf(output, "ROOT"); break;
        case XPATH_OP_NODE:
	     fprintf(output, "NODE"); break;
        case XPATH_OP_RESET:
	     fprintf(output, "RESET"); break;
        case XPATH_OP_SORT:
	     fprintf(output, "SORT"); break;
        case XPATH_OP_COLLECT: {
	    xmlXPathAxisVal axis = (xmlXPathAxisVal)op->value;
	    xmlXPathTestVal test = (xmlXPathTestVal)op->value2;
	    xmlXPathTypeVal type = (xmlXPathTypeVal)op->value3;
	    const xmlChar *prefix = op->value4;
	    const xmlChar *name = op->value5;

	    fprintf(output, "COLLECT ");
	    switch (axis) {
		case AXIS_ANCESTOR:
		    fprintf(output, " 'ancestors' "); break;
		case AXIS_ANCESTOR_OR_SELF:
		    fprintf(output, " 'ancestors-or-self' "); break;
		case AXIS_ATTRIBUTE:
		    fprintf(output, " 'attributes' "); break;
		case AXIS_CHILD:
		    fprintf(output, " 'child' "); break;
		case AXIS_DESCENDANT:
		    fprintf(output, " 'descendant' "); break;
		case AXIS_DESCENDANT_OR_SELF:
		    fprintf(output, " 'descendant-or-self' "); break;
		case AXIS_FOLLOWING:
		    fprintf(output, " 'following' "); break;
		case AXIS_FOLLOWING_SIBLING:
		    fprintf(output, " 'following-siblings' "); break;
		case AXIS_NAMESPACE:
		    fprintf(output, " 'namespace' "); break;
		case AXIS_PARENT:
		    fprintf(output, " 'parent' "); break;
		case AXIS_PRECEDING:
		    fprintf(output, " 'preceding' "); break;
		case AXIS_PRECEDING_SIBLING:
		    fprintf(output, " 'preceding-sibling' "); break;
		case AXIS_SELF:
		    fprintf(output, " 'self' "); break;
	    }
	    switch (test) {
                case NODE_TEST_NONE:
		    fprintf(output, "'none' "); break;
                case NODE_TEST_TYPE:
		    fprintf(output, "'type' "); break;
                case NODE_TEST_PI:
		    fprintf(output, "'PI' "); break;
                case NODE_TEST_ALL:
		    fprintf(output, "'all' "); break;
                case NODE_TEST_NS:
		    fprintf(output, "'namespace' "); break;
                case NODE_TEST_NAME:
		    fprintf(output, "'name' "); break;
	    }
	    switch (type) {
                case NODE_TYPE_NODE:
		    fprintf(output, "'node' "); break;
                case NODE_TYPE_COMMENT:
		    fprintf(output, "'comment' "); break;
                case NODE_TYPE_TEXT:
		    fprintf(output, "'text' "); break;
                case NODE_TYPE_PI:
		    fprintf(output, "'PI' "); break;
	    }
	    if (prefix != NULL)
		fprintf(output, "%s:", prefix);
	    if (name != NULL)
		fprintf(output, "%s", (const char *) name);
	    break;

        }
	case XPATH_OP_VALUE: {
	    xmlXPathObjectPtr object = (xmlXPathObjectPtr) op->value4;

	    fprintf(output, "ELEM ");
	    xmlXPathDebugDumpObject(output, object, 0);
	    goto finish;
	}
	case XPATH_OP_VARIABLE: {
	    const xmlChar *prefix = op->value5;
	    const xmlChar *name = op->value4;

	    if (prefix != NULL)
		fprintf(output, "VARIABLE %s:%s", prefix, name);
	    else
		fprintf(output, "VARIABLE %s", name);
	    break;
	}
	case XPATH_OP_FUNCTION: {
	    int nbargs = op->value;
	    const xmlChar *prefix = op->value5;
	    const xmlChar *name = op->value4;

	    if (prefix != NULL)
		fprintf(output, "FUNCTION %s:%s(%d args)",
			prefix, name, nbargs);
	    else
		fprintf(output, "FUNCTION %s(%d args)", name, nbargs);
	    break;
	}
        case XPATH_OP_ARG: fprintf(output, "ARG"); break;
        case XPATH_OP_PREDICATE: fprintf(output, "PREDICATE"); break;
        case XPATH_OP_FILTER: fprintf(output, "FILTER"); break;
#ifdef LIBXML_XPTR_ENABLED
        case XPATH_OP_RANGETO: fprintf(output, "RANGETO"); break;
#endif
	default:
        fprintf(output, "UNKNOWN %d\n", op->op); return;
    }
    fprintf(output, "\n");
finish:
    if (op->ch1 >= 0)
	xmlXPathDebugDumpStepOp(output, comp, &comp->steps[op->ch1], depth + 1);
    if (op->ch2 >= 0)
	xmlXPathDebugDumpStepOp(output, comp, &comp->steps[op->ch2], depth + 1);
}

void
xmlXPathDebugDumpCompExpr(FILE *output, xmlXPathCompExprPtr comp,
	                  int depth) {
    int i;
    char shift[100];

    if ((output == NULL) || (comp == NULL)) return;

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;

    fprintf(output, "%s", shift);

    fprintf(output, "Compiled Expression : %d elements\n",
	    comp->nbStep);
    i = comp->last;
    xmlXPathDebugDumpStepOp(output, comp, &comp->steps[i], depth + 1);
}

#ifdef XP_DEBUG_OBJ_USAGE

static int xmlXPathDebugObjCounterUndefined = 0;
static int xmlXPathDebugObjCounterNodeset = 0;
static int xmlXPathDebugObjCounterBool = 0;
static int xmlXPathDebugObjCounterNumber = 0;
static int xmlXPathDebugObjCounterString = 0;
static int xmlXPathDebugObjCounterPoint = 0;
static int xmlXPathDebugObjCounterRange = 0;
static int xmlXPathDebugObjCounterLocset = 0;
static int xmlXPathDebugObjCounterUsers = 0;
static int xmlXPathDebugObjCounterXSLTTree = 0;
static int xmlXPathDebugObjCounterAll = 0;

static int xmlXPathDebugObjTotalUndefined = 0;
static int xmlXPathDebugObjTotalNodeset = 0;
static int xmlXPathDebugObjTotalBool = 0;
static int xmlXPathDebugObjTotalNumber = 0;
static int xmlXPathDebugObjTotalString = 0;
static int xmlXPathDebugObjTotalPoint = 0;
static int xmlXPathDebugObjTotalRange = 0;
static int xmlXPathDebugObjTotalLocset = 0;
static int xmlXPathDebugObjTotalUsers = 0;
static int xmlXPathDebugObjTotalXSLTTree = 0;
static int xmlXPathDebugObjTotalAll = 0;

static int xmlXPathDebugObjMaxUndefined = 0;
static int xmlXPathDebugObjMaxNodeset = 0;
static int xmlXPathDebugObjMaxBool = 0;
static int xmlXPathDebugObjMaxNumber = 0;
static int xmlXPathDebugObjMaxString = 0;
static int xmlXPathDebugObjMaxPoint = 0;
static int xmlXPathDebugObjMaxRange = 0;
static int xmlXPathDebugObjMaxLocset = 0;
static int xmlXPathDebugObjMaxUsers = 0;
static int xmlXPathDebugObjMaxXSLTTree = 0;
static int xmlXPathDebugObjMaxAll = 0;

static void
xmlXPathDebugObjUsageReset(xmlXPathContextPtr ctxt)
{
    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    cache->dbgCachedAll = 0;
	    cache->dbgCachedNodeset = 0;
	    cache->dbgCachedString = 0;
	    cache->dbgCachedBool = 0;
	    cache->dbgCachedNumber = 0;
	    cache->dbgCachedPoint = 0;
	    cache->dbgCachedRange = 0;
	    cache->dbgCachedLocset = 0;
	    cache->dbgCachedUsers = 0;
	    cache->dbgCachedXSLTTree = 0;
	    cache->dbgCachedUndefined = 0;

	    cache->dbgReusedAll = 0;
	    cache->dbgReusedNodeset = 0;
	    cache->dbgReusedString = 0;
	    cache->dbgReusedBool = 0;
	    cache->dbgReusedNumber = 0;
	    cache->dbgReusedPoint = 0;
	    cache->dbgReusedRange = 0;
	    cache->dbgReusedLocset = 0;
	    cache->dbgReusedUsers = 0;
	    cache->dbgReusedXSLTTree = 0;
	    cache->dbgReusedUndefined = 0;
	}
    }

    xmlXPathDebugObjCounterUndefined = 0;
    xmlXPathDebugObjCounterNodeset = 0;
    xmlXPathDebugObjCounterBool = 0;
    xmlXPathDebugObjCounterNumber = 0;
    xmlXPathDebugObjCounterString = 0;
    xmlXPathDebugObjCounterPoint = 0;
    xmlXPathDebugObjCounterRange = 0;
    xmlXPathDebugObjCounterLocset = 0;
    xmlXPathDebugObjCounterUsers = 0;
    xmlXPathDebugObjCounterXSLTTree = 0;
    xmlXPathDebugObjCounterAll = 0;

    xmlXPathDebugObjTotalUndefined = 0;
    xmlXPathDebugObjTotalNodeset = 0;
    xmlXPathDebugObjTotalBool = 0;
    xmlXPathDebugObjTotalNumber = 0;
    xmlXPathDebugObjTotalString = 0;
    xmlXPathDebugObjTotalPoint = 0;
    xmlXPathDebugObjTotalRange = 0;
    xmlXPathDebugObjTotalLocset = 0;
    xmlXPathDebugObjTotalUsers = 0;
    xmlXPathDebugObjTotalXSLTTree = 0;
    xmlXPathDebugObjTotalAll = 0;

    xmlXPathDebugObjMaxUndefined = 0;
    xmlXPathDebugObjMaxNodeset = 0;
    xmlXPathDebugObjMaxBool = 0;
    xmlXPathDebugObjMaxNumber = 0;
    xmlXPathDebugObjMaxString = 0;
    xmlXPathDebugObjMaxPoint = 0;
    xmlXPathDebugObjMaxRange = 0;
    xmlXPathDebugObjMaxLocset = 0;
    xmlXPathDebugObjMaxUsers = 0;
    xmlXPathDebugObjMaxXSLTTree = 0;
    xmlXPathDebugObjMaxAll = 0;

}

static void
xmlXPathDebugObjUsageRequested(xmlXPathContextPtr ctxt,
			      xmlXPathObjectType objType)
{
    int isCached = 0;

    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    isCached = 1;

	    cache->dbgReusedAll++;
	    switch (objType) {
		case XPATH_UNDEFINED:
		    cache->dbgReusedUndefined++;
		    break;
		case XPATH_NODESET:
		    cache->dbgReusedNodeset++;
		    break;
		case XPATH_BOOLEAN:
		    cache->dbgReusedBool++;
		    break;
		case XPATH_NUMBER:
		    cache->dbgReusedNumber++;
		    break;
		case XPATH_STRING:
		    cache->dbgReusedString++;
		    break;
		case XPATH_POINT:
		    cache->dbgReusedPoint++;
		    break;
		case XPATH_RANGE:
		    cache->dbgReusedRange++;
		    break;
		case XPATH_LOCATIONSET:
		    cache->dbgReusedLocset++;
		    break;
		case XPATH_USERS:
		    cache->dbgReusedUsers++;
		    break;
		case XPATH_XSLT_TREE:
		    cache->dbgReusedXSLTTree++;
		    break;
		default:
		    break;
	    }
	}
    }

    switch (objType) {
	case XPATH_UNDEFINED:
	    if (! isCached)
		xmlXPathDebugObjTotalUndefined++;
	    xmlXPathDebugObjCounterUndefined++;
	    if (xmlXPathDebugObjCounterUndefined >
		xmlXPathDebugObjMaxUndefined)
		xmlXPathDebugObjMaxUndefined =
		    xmlXPathDebugObjCounterUndefined;
	    break;
	case XPATH_NODESET:
	    if (! isCached)
		xmlXPathDebugObjTotalNodeset++;
	    xmlXPathDebugObjCounterNodeset++;
	    if (xmlXPathDebugObjCounterNodeset >
		xmlXPathDebugObjMaxNodeset)
		xmlXPathDebugObjMaxNodeset =
		    xmlXPathDebugObjCounterNodeset;
	    break;
	case XPATH_BOOLEAN:
	    if (! isCached)
		xmlXPathDebugObjTotalBool++;
	    xmlXPathDebugObjCounterBool++;
	    if (xmlXPathDebugObjCounterBool >
		xmlXPathDebugObjMaxBool)
		xmlXPathDebugObjMaxBool =
		    xmlXPathDebugObjCounterBool;
	    break;
	case XPATH_NUMBER:
	    if (! isCached)
		xmlXPathDebugObjTotalNumber++;
	    xmlXPathDebugObjCounterNumber++;
	    if (xmlXPathDebugObjCounterNumber >
		xmlXPathDebugObjMaxNumber)
		xmlXPathDebugObjMaxNumber =
		    xmlXPathDebugObjCounterNumber;
	    break;
	case XPATH_STRING:
	    if (! isCached)
		xmlXPathDebugObjTotalString++;
	    xmlXPathDebugObjCounterString++;
	    if (xmlXPathDebugObjCounterString >
		xmlXPathDebugObjMaxString)
		xmlXPathDebugObjMaxString =
		    xmlXPathDebugObjCounterString;
	    break;
	case XPATH_POINT:
	    if (! isCached)
		xmlXPathDebugObjTotalPoint++;
	    xmlXPathDebugObjCounterPoint++;
	    if (xmlXPathDebugObjCounterPoint >
		xmlXPathDebugObjMaxPoint)
		xmlXPathDebugObjMaxPoint =
		    xmlXPathDebugObjCounterPoint;
	    break;
	case XPATH_RANGE:
	    if (! isCached)
		xmlXPathDebugObjTotalRange++;
	    xmlXPathDebugObjCounterRange++;
	    if (xmlXPathDebugObjCounterRange >
		xmlXPathDebugObjMaxRange)
		xmlXPathDebugObjMaxRange =
		    xmlXPathDebugObjCounterRange;
	    break;
	case XPATH_LOCATIONSET:
	    if (! isCached)
		xmlXPathDebugObjTotalLocset++;
	    xmlXPathDebugObjCounterLocset++;
	    if (xmlXPathDebugObjCounterLocset >
		xmlXPathDebugObjMaxLocset)
		xmlXPathDebugObjMaxLocset =
		    xmlXPathDebugObjCounterLocset;
	    break;
	case XPATH_USERS:
	    if (! isCached)
		xmlXPathDebugObjTotalUsers++;
	    xmlXPathDebugObjCounterUsers++;
	    if (xmlXPathDebugObjCounterUsers >
		xmlXPathDebugObjMaxUsers)
		xmlXPathDebugObjMaxUsers =
		    xmlXPathDebugObjCounterUsers;
	    break;
	case XPATH_XSLT_TREE:
	    if (! isCached)
		xmlXPathDebugObjTotalXSLTTree++;
	    xmlXPathDebugObjCounterXSLTTree++;
	    if (xmlXPathDebugObjCounterXSLTTree >
		xmlXPathDebugObjMaxXSLTTree)
		xmlXPathDebugObjMaxXSLTTree =
		    xmlXPathDebugObjCounterXSLTTree;
	    break;
	default:
	    break;
    }
    if (! isCached)
	xmlXPathDebugObjTotalAll++;
    xmlXPathDebugObjCounterAll++;
    if (xmlXPathDebugObjCounterAll >
	xmlXPathDebugObjMaxAll)
	xmlXPathDebugObjMaxAll =
	    xmlXPathDebugObjCounterAll;
}

static void
xmlXPathDebugObjUsageReleased(xmlXPathContextPtr ctxt,
			      xmlXPathObjectType objType)
{
    int isCached = 0;

    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    isCached = 1;

	    cache->dbgCachedAll++;
	    switch (objType) {
		case XPATH_UNDEFINED:
		    cache->dbgCachedUndefined++;
		    break;
		case XPATH_NODESET:
		    cache->dbgCachedNodeset++;
		    break;
		case XPATH_BOOLEAN:
		    cache->dbgCachedBool++;
		    break;
		case XPATH_NUMBER:
		    cache->dbgCachedNumber++;
		    break;
		case XPATH_STRING:
		    cache->dbgCachedString++;
		    break;
		case XPATH_POINT:
		    cache->dbgCachedPoint++;
		    break;
		case XPATH_RANGE:
		    cache->dbgCachedRange++;
		    break;
		case XPATH_LOCATIONSET:
		    cache->dbgCachedLocset++;
		    break;
		case XPATH_USERS:
		    cache->dbgCachedUsers++;
		    break;
		case XPATH_XSLT_TREE:
		    cache->dbgCachedXSLTTree++;
		    break;
		default:
		    break;
	    }

	}
    }
    switch (objType) {
	case XPATH_UNDEFINED:
	    xmlXPathDebugObjCounterUndefined--;
	    break;
	case XPATH_NODESET:
	    xmlXPathDebugObjCounterNodeset--;
	    break;
	case XPATH_BOOLEAN:
	    xmlXPathDebugObjCounterBool--;
	    break;
	case XPATH_NUMBER:
	    xmlXPathDebugObjCounterNumber--;
	    break;
	case XPATH_STRING:
	    xmlXPathDebugObjCounterString--;
	    break;
	case XPATH_POINT:
	    xmlXPathDebugObjCounterPoint--;
	    break;
	case XPATH_RANGE:
	    xmlXPathDebugObjCounterRange--;
	    break;
	case XPATH_LOCATIONSET:
	    xmlXPathDebugObjCounterLocset--;
	    break;
	case XPATH_USERS:
	    xmlXPathDebugObjCounterUsers--;
	    break;
	case XPATH_XSLT_TREE:
	    xmlXPathDebugObjCounterXSLTTree--;
	    break;
	default:
	    break;
    }
    xmlXPathDebugObjCounterAll--;
}

static void
xmlXPathDebugObjUsageDisplay(xmlXPathContextPtr ctxt)
{
    int reqAll, reqNodeset, reqString, reqBool, reqNumber,
	reqXSLTTree, reqUndefined;
    int caAll = 0, caNodeset = 0, caString = 0, caBool = 0,
	caNumber = 0, caXSLTTree = 0, caUndefined = 0;
    int reAll = 0, reNodeset = 0, reString = 0, reBool = 0,
	reNumber = 0, reXSLTTree = 0, reUndefined = 0;
    int leftObjs = xmlXPathDebugObjCounterAll;

    reqAll = xmlXPathDebugObjTotalAll;
    reqNodeset = xmlXPathDebugObjTotalNodeset;
    reqString = xmlXPathDebugObjTotalString;
    reqBool = xmlXPathDebugObjTotalBool;
    reqNumber = xmlXPathDebugObjTotalNumber;
    reqXSLTTree = xmlXPathDebugObjTotalXSLTTree;
    reqUndefined = xmlXPathDebugObjTotalUndefined;

    printf("# XPath object usage:\n");

    if (ctxt != NULL) {
	if (ctxt->cache != NULL) {
	    xmlXPathContextCachePtr cache =
		(xmlXPathContextCachePtr) ctxt->cache;

	    reAll = cache->dbgReusedAll;
	    reqAll += reAll;
	    reNodeset = cache->dbgReusedNodeset;
	    reqNodeset += reNodeset;
	    reString = cache->dbgReusedString;
	    reqString += reString;
	    reBool = cache->dbgReusedBool;
	    reqBool += reBool;
	    reNumber = cache->dbgReusedNumber;
	    reqNumber += reNumber;
	    reXSLTTree = cache->dbgReusedXSLTTree;
	    reqXSLTTree += reXSLTTree;
	    reUndefined = cache->dbgReusedUndefined;
	    reqUndefined += reUndefined;

	    caAll = cache->dbgCachedAll;
	    caBool = cache->dbgCachedBool;
	    caNodeset = cache->dbgCachedNodeset;
	    caString = cache->dbgCachedString;
	    caNumber = cache->dbgCachedNumber;
	    caXSLTTree = cache->dbgCachedXSLTTree;
	    caUndefined = cache->dbgCachedUndefined;

	    if (cache->nodesetObjs)
		leftObjs -= cache->nodesetObjs->number;
	    if (cache->stringObjs)
		leftObjs -= cache->stringObjs->number;
	    if (cache->booleanObjs)
		leftObjs -= cache->booleanObjs->number;
	    if (cache->numberObjs)
		leftObjs -= cache->numberObjs->number;
	    if (cache->miscObjs)
		leftObjs -= cache->miscObjs->number;
	}
    }

    printf("# all\n");
    printf("#   total  : %d\n", reqAll);
    printf("#   left  : %d\n", leftObjs);
    printf("#   created: %d\n", xmlXPathDebugObjTotalAll);
    printf("#   reused : %d\n", reAll);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxAll);

    printf("# node-sets\n");
    printf("#   total  : %d\n", reqNodeset);
    printf("#   created: %d\n", xmlXPathDebugObjTotalNodeset);
    printf("#   reused : %d\n", reNodeset);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxNodeset);

    printf("# strings\n");
    printf("#   total  : %d\n", reqString);
    printf("#   created: %d\n", xmlXPathDebugObjTotalString);
    printf("#   reused : %d\n", reString);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxString);

    printf("# booleans\n");
    printf("#   total  : %d\n", reqBool);
    printf("#   created: %d\n", xmlXPathDebugObjTotalBool);
    printf("#   reused : %d\n", reBool);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxBool);

    printf("# numbers\n");
    printf("#   total  : %d\n", reqNumber);
    printf("#   created: %d\n", xmlXPathDebugObjTotalNumber);
    printf("#   reused : %d\n", reNumber);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxNumber);

    printf("# XSLT result tree fragments\n");
    printf("#   total  : %d\n", reqXSLTTree);
    printf("#   created: %d\n", xmlXPathDebugObjTotalXSLTTree);
    printf("#   reused : %d\n", reXSLTTree);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxXSLTTree);

    printf("# undefined\n");
    printf("#   total  : %d\n", reqUndefined);
    printf("#   created: %d\n", xmlXPathDebugObjTotalUndefined);
    printf("#   reused : %d\n", reUndefined);
    printf("#   max    : %d\n", xmlXPathDebugObjMaxUndefined);

}

#endif 

#endif 


static xmlXPathContextCachePtr
xmlXPathNewCache(void)
{
    xmlXPathContextCachePtr ret;

    ret = (xmlXPathContextCachePtr) xmlMalloc(sizeof(xmlXPathContextCache));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating object cache\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathContextCache));
    ret->maxNodeset = 100;
    ret->maxString = 100;
    ret->maxBoolean = 100;
    ret->maxNumber = 100;
    ret->maxMisc = 100;
    return(ret);
}

static void
xmlXPathCacheFreeObjectList(xmlPointerListPtr list)
{
    int i;
    xmlXPathObjectPtr obj;

    if (list == NULL)
	return;

    for (i = 0; i < list->number; i++) {
	obj = list->items[i];
	if (obj->nodesetval != NULL) {
	    if (obj->nodesetval->nodeTab != NULL)
		xmlFree(obj->nodesetval->nodeTab);
	    xmlFree(obj->nodesetval);
	}
	xmlFree(obj);
#ifdef XP_DEBUG_OBJ_USAGE
	xmlXPathDebugObjCounterAll--;
#endif
    }
    xmlPointerListFree(list);
}

static void
xmlXPathFreeCache(xmlXPathContextCachePtr cache)
{
    if (cache == NULL)
	return;
    if (cache->nodesetObjs)
	xmlXPathCacheFreeObjectList(cache->nodesetObjs);
    if (cache->stringObjs)
	xmlXPathCacheFreeObjectList(cache->stringObjs);
    if (cache->booleanObjs)
	xmlXPathCacheFreeObjectList(cache->booleanObjs);
    if (cache->numberObjs)
	xmlXPathCacheFreeObjectList(cache->numberObjs);
    if (cache->miscObjs)
	xmlXPathCacheFreeObjectList(cache->miscObjs);
    xmlFree(cache);
}

int
xmlXPathContextSetCache(xmlXPathContextPtr ctxt,
			int active,
			int value,
			int options)
{
    if (ctxt == NULL)
	return(-1);
    if (active) {
	xmlXPathContextCachePtr cache;

	if (ctxt->cache == NULL) {
	    ctxt->cache = xmlXPathNewCache();
	    if (ctxt->cache == NULL)
		return(-1);
	}
	cache = (xmlXPathContextCachePtr) ctxt->cache;
	if (options == 0) {
	    if (value < 0)
		value = 100;
	    cache->maxNodeset = value;
	    cache->maxString = value;
	    cache->maxNumber = value;
	    cache->maxBoolean = value;
	    cache->maxMisc = value;
	}
    } else if (ctxt->cache != NULL) {
	xmlXPathFreeCache((xmlXPathContextCachePtr) ctxt->cache);
	ctxt->cache = NULL;
    }
    return(0);
}

static xmlXPathObjectPtr
xmlXPathCacheWrapNodeSet(xmlXPathContextPtr ctxt, xmlNodeSetPtr val)
{
    if ((ctxt != NULL) && (ctxt->cache != NULL)) {
	xmlXPathContextCachePtr cache =
	    (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];
	    ret->type = XPATH_NODESET;
	    ret->nodesetval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_NODESET);
#endif
	    return(ret);
	}
    }

    return(xmlXPathWrapNodeSet(val));

}

static xmlXPathObjectPtr
xmlXPathCacheWrapString(xmlXPathContextPtr ctxt, xmlChar *val)
{
    if ((ctxt != NULL) && (ctxt->cache != NULL)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->stringObjs != NULL) &&
	    (cache->stringObjs->number != 0))
	{

	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->stringObjs->items[--cache->stringObjs->number];
	    ret->type = XPATH_STRING;
	    ret->stringval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;
	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_STRING;
	    ret->stringval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	}
    }
    return(xmlXPathWrapString(val));
}

static xmlXPathObjectPtr
xmlXPathCacheNewNodeSet(xmlXPathContextPtr ctxt, xmlNodePtr val)
{
    if ((ctxt != NULL) && (ctxt->cache)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->nodesetObjs != NULL) &&
	    (cache->nodesetObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;
	    ret = (xmlXPathObjectPtr)
		cache->nodesetObjs->items[--cache->nodesetObjs->number];
	    ret->type = XPATH_NODESET;
	    ret->boolval = 0;
	    if (val) {
		if ((ret->nodesetval->nodeMax == 0) ||
		    (val->type == XML_NAMESPACE_DECL))
		{
		    xmlXPathNodeSetAddUnique(ret->nodesetval, val);
		} else {
		    ret->nodesetval->nodeTab[0] = val;
		    ret->nodesetval->nodeNr = 1;
		}
	    }
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_NODESET);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_NODESET;
	    ret->boolval = 0;
	    ret->nodesetval = xmlXPathNodeSetCreate(val);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_NODESET);
#endif
	    return(ret);
	}
    }
    return(xmlXPathNewNodeSet(val));
}

static xmlXPathObjectPtr
xmlXPathCacheNewCString(xmlXPathContextPtr ctxt, const char *val)
{
    if ((ctxt != NULL) && (ctxt->cache)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->stringObjs != NULL) &&
	    (cache->stringObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->stringObjs->items[--cache->stringObjs->number];

	    ret->type = XPATH_STRING;
	    ret->stringval = xmlStrdup(BAD_CAST val);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_STRING;
	    ret->stringval = xmlStrdup(BAD_CAST val);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	}
    }
    return(xmlXPathNewCString(val));
}

static xmlXPathObjectPtr
xmlXPathCacheNewString(xmlXPathContextPtr ctxt, const xmlChar *val)
{
    if ((ctxt != NULL) && (ctxt->cache)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->stringObjs != NULL) &&
	    (cache->stringObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->stringObjs->items[--cache->stringObjs->number];
	    ret->type = XPATH_STRING;
	    if (val != NULL)
		ret->stringval = xmlStrdup(val);
	    else
		ret->stringval = xmlStrdup((const xmlChar *)"");
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_STRING;
	    if (val != NULL)
		ret->stringval = xmlStrdup(val);
	    else
		ret->stringval = xmlStrdup((const xmlChar *)"");
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_STRING);
#endif
	    return(ret);
	}
    }
    return(xmlXPathNewString(val));
}

static xmlXPathObjectPtr
xmlXPathCacheNewBoolean(xmlXPathContextPtr ctxt, int val)
{
    if ((ctxt != NULL) && (ctxt->cache)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->booleanObjs != NULL) &&
	    (cache->booleanObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->booleanObjs->items[--cache->booleanObjs->number];
	    ret->type = XPATH_BOOLEAN;
	    ret->boolval = (val != 0);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_BOOLEAN);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_BOOLEAN;
	    ret->boolval = (val != 0);
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_BOOLEAN);
#endif
	    return(ret);
	}
    }
    return(xmlXPathNewBoolean(val));
}

static xmlXPathObjectPtr
xmlXPathCacheNewFloat(xmlXPathContextPtr ctxt, double val)
{
     if ((ctxt != NULL) && (ctxt->cache)) {
	xmlXPathContextCachePtr cache = (xmlXPathContextCachePtr) ctxt->cache;

	if ((cache->numberObjs != NULL) &&
	    (cache->numberObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->numberObjs->items[--cache->numberObjs->number];
	    ret->type = XPATH_NUMBER;
	    ret->floatval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_NUMBER);
#endif
	    return(ret);
	} else if ((cache->miscObjs != NULL) &&
	    (cache->miscObjs->number != 0))
	{
	    xmlXPathObjectPtr ret;

	    ret = (xmlXPathObjectPtr)
		cache->miscObjs->items[--cache->miscObjs->number];

	    ret->type = XPATH_NUMBER;
	    ret->floatval = val;
#ifdef XP_DEBUG_OBJ_USAGE
	    xmlXPathDebugObjUsageRequested(ctxt, XPATH_NUMBER);
#endif
	    return(ret);
	}
    }
    return(xmlXPathNewFloat(val));
}


static xmlXPathObjectPtr
xmlXPathCacheConvertString(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val) {
    xmlChar *res = NULL;

    if (val == NULL)
	return(xmlXPathCacheNewCString(ctxt, ""));

    switch (val->type) {
    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext, "STRING: undefined\n");
#endif
	break;
    case XPATH_NODESET:
    case XPATH_XSLT_TREE:
	res = xmlXPathCastNodeSetToString(val->nodesetval);
	break;
    case XPATH_STRING:
	return(val);
    case XPATH_BOOLEAN:
	res = xmlXPathCastBooleanToString(val->boolval);
	break;
    case XPATH_NUMBER:
	res = xmlXPathCastNumberToString(val->floatval);
	break;
    case XPATH_USERS:
    case XPATH_POINT:
    case XPATH_RANGE:
    case XPATH_LOCATIONSET:
	TODO;
	break;
    }
    xmlXPathReleaseObject(ctxt, val);
    if (res == NULL)
	return(xmlXPathCacheNewCString(ctxt, ""));
    return(xmlXPathCacheWrapString(ctxt, res));
}

static xmlXPathObjectPtr
xmlXPathCacheObjectCopy(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val)
{
    if (val == NULL)
	return(NULL);

    if (XP_HAS_CACHE(ctxt)) {
	switch (val->type) {
	    case XPATH_NODESET:
		return(xmlXPathCacheWrapNodeSet(ctxt,
		    xmlXPathNodeSetMerge(NULL, val->nodesetval)));
	    case XPATH_STRING:
		return(xmlXPathCacheNewString(ctxt, val->stringval));
	    case XPATH_BOOLEAN:
		return(xmlXPathCacheNewBoolean(ctxt, val->boolval));
	    case XPATH_NUMBER:
		return(xmlXPathCacheNewFloat(ctxt, val->floatval));
	    default:
		break;
	}
    }
    return(xmlXPathObjectCopy(val));
}

static xmlXPathObjectPtr
xmlXPathCacheConvertBoolean(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(xmlXPathCacheNewBoolean(ctxt, 0));
    if (val->type == XPATH_BOOLEAN)
	return(val);
    ret = xmlXPathCacheNewBoolean(ctxt, xmlXPathCastToBoolean(val));
    xmlXPathReleaseObject(ctxt, val);
    return(ret);
}

static xmlXPathObjectPtr
xmlXPathCacheConvertNumber(xmlXPathContextPtr ctxt, xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(xmlXPathCacheNewFloat(ctxt, 0.0));
    if (val->type == XPATH_NUMBER)
	return(val);
    ret = xmlXPathCacheNewFloat(ctxt, xmlXPathCastToNumber(val));
    xmlXPathReleaseObject(ctxt, val);
    return(ret);
}


xmlXPathObjectPtr
valuePop(xmlXPathParserContextPtr ctxt)
{
    xmlXPathObjectPtr ret;

    if ((ctxt == NULL) || (ctxt->valueNr <= 0))
        return (NULL);
    ctxt->valueNr--;
    if (ctxt->valueNr > 0)
        ctxt->value = ctxt->valueTab[ctxt->valueNr - 1];
    else
        ctxt->value = NULL;
    ret = ctxt->valueTab[ctxt->valueNr];
    ctxt->valueTab[ctxt->valueNr] = NULL;
    return (ret);
}
int
valuePush(xmlXPathParserContextPtr ctxt, xmlXPathObjectPtr value)
{
    if ((ctxt == NULL) || (value == NULL)) return(-1);
    if (ctxt->valueNr >= ctxt->valueMax) {
        xmlXPathObjectPtr *tmp;

        tmp = (xmlXPathObjectPtr *) xmlRealloc(ctxt->valueTab,
                                             2 * ctxt->valueMax *
                                             sizeof(ctxt->valueTab[0]));
        if (tmp == NULL) {
            xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
            return (0);
        }
        ctxt->valueMax *= 2;
	ctxt->valueTab = tmp;
    }
    ctxt->valueTab[ctxt->valueNr] = value;
    ctxt->value = value;
    return (ctxt->valueNr++);
}

int
xmlXPathPopBoolean (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    int ret;

    obj = valuePop(ctxt);
    if (obj == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(0);
    }
    if (obj->type != XPATH_BOOLEAN)
	ret = xmlXPathCastToBoolean(obj);
    else
        ret = obj->boolval;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}

double
xmlXPathPopNumber (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    double ret;

    obj = valuePop(ctxt);
    if (obj == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(0);
    }
    if (obj->type != XPATH_NUMBER)
	ret = xmlXPathCastToNumber(obj);
    else
        ret = obj->floatval;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}

xmlChar *
xmlXPathPopString (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    xmlChar * ret;

    obj = valuePop(ctxt);
    if (obj == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(NULL);
    }
    ret = xmlXPathCastToString(obj);	
    
    if (obj->stringval == ret)
	obj->stringval = NULL;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}

xmlNodeSetPtr
xmlXPathPopNodeSet (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr ret;

    if (ctxt == NULL) return(NULL);
    if (ctxt->value == NULL) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(NULL);
    }
    if (!xmlXPathStackIsNodeSet(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return(NULL);
    }
    obj = valuePop(ctxt);
    ret = obj->nodesetval;
#if 0
    
    if (obj->boolval && obj->user != NULL)
        xmlFreeNodeList((xmlNodePtr) obj->user);
#endif
    obj->nodesetval = NULL;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}

void *
xmlXPathPopExternal (xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr obj;
    void * ret;

    if ((ctxt == NULL) || (ctxt->value == NULL)) {
	xmlXPathSetError(ctxt, XPATH_INVALID_OPERAND);
	return(NULL);
    }
    if (ctxt->value->type != XPATH_USERS) {
	xmlXPathSetTypeError(ctxt);
	return(NULL);
    }
    obj = valuePop(ctxt);
    ret = obj->user;
    obj->user = NULL;
    xmlXPathReleaseObject(ctxt->context, obj);
    return(ret);
}


#define CUR (*ctxt->cur)
#define SKIP(val) ctxt->cur += (val)
#define NXT(val) ctxt->cur[(val)]
#define CUR_PTR ctxt->cur
#define CUR_CHAR(l) xmlXPathCurrentChar(ctxt, &l)

#define COPY_BUF(l,b,i,v)                                              \
    if (l == 1) b[i++] = (xmlChar) v;                                  \
    else i += xmlCopyChar(l,&b[i],v)

#define NEXTL(l)  ctxt->cur += l

#define SKIP_BLANKS							\
    while (IS_BLANK_CH(*(ctxt->cur))) NEXT

#define CURRENT (*ctxt->cur)
#define NEXT ((*ctxt->cur) ?  ctxt->cur++: ctxt->cur)


#ifndef DBL_DIG
#define DBL_DIG 16
#endif
#ifndef DBL_EPSILON
#define DBL_EPSILON 1E-9
#endif

#define UPPER_DOUBLE 1E9
#define LOWER_DOUBLE 1E-5
#define	LOWER_DOUBLE_EXP 5

#define INTEGER_DIGITS DBL_DIG
#define FRACTION_DIGITS (DBL_DIG + 1 + (LOWER_DOUBLE_EXP))
#define EXPONENT_DIGITS (3 + 2)

static void
xmlXPathFormatNumber(double number, char buffer[], int buffersize)
{
    switch (xmlXPathIsInf(number)) {
    case 1:
	if (buffersize > (int)sizeof("Infinity"))
	    snprintf(buffer, buffersize, "Infinity");
	break;
    case -1:
	if (buffersize > (int)sizeof("-Infinity"))
	    snprintf(buffer, buffersize, "-Infinity");
	break;
    default:
	if (xmlXPathIsNaN(number)) {
	    if (buffersize > (int)sizeof("NaN"))
		snprintf(buffer, buffersize, "NaN");
	} else if (number == 0 && xmlXPathGetSign(number) != 0) {
	    snprintf(buffer, buffersize, "0");
	} else if (number == ((int) number)) {
	    char work[30];
	    char *ptr, *cur;
	    int value = (int) number;

            ptr = &buffer[0];
	    if (value == 0) {
		*ptr++ = '0';
	    } else {
		snprintf(work, 29, "%d", value);
		cur = &work[0];
		while ((*cur) && (ptr - buffer < buffersize)) {
		    *ptr++ = *cur++;
		}
	    }
	    if (ptr - buffer < buffersize) {
		*ptr = 0;
	    } else if (buffersize > 0) {
		ptr--;
		*ptr = 0;
	    }
	} else {
	    char work[DBL_DIG + EXPONENT_DIGITS + 3 + LOWER_DOUBLE_EXP];
	    int integer_place, fraction_place;
	    char *ptr;
	    char *after_fraction;
	    double absolute_value;
	    int size;

	    absolute_value = fabs(number);

	    if ( ((absolute_value > UPPER_DOUBLE) ||
		  (absolute_value < LOWER_DOUBLE)) &&
		 (absolute_value != 0.0) ) {
		
		integer_place = DBL_DIG + EXPONENT_DIGITS + 1;
		fraction_place = DBL_DIG - 1;
		size = snprintf(work, sizeof(work),"%*.*e",
			 integer_place, fraction_place, number);
		while ((size > 0) && (work[size] != 'e')) size--;

	    }
	    else {
		
		if (absolute_value > 0.0) {
		    integer_place = (int)log10(absolute_value);
		    if (integer_place > 0)
		        fraction_place = DBL_DIG - integer_place - 1;
		    else
		        fraction_place = DBL_DIG - integer_place;
		} else {
		    fraction_place = 1;
		}
		size = snprintf(work, sizeof(work), "%0.*f",
				fraction_place, number);
	    }

	    
	    after_fraction = work + size;
	    ptr = after_fraction;
	    while (*(--ptr) == '0')
		;
	    if (*ptr != '.')
	        ptr++;
	    while ((*ptr++ = *after_fraction++) != 0);

	    
	    size = strlen(work) + 1;
	    if (size > buffersize) {
		work[buffersize - 1] = 0;
		size = buffersize;
	    }
	    memmove(buffer, work, size);
	}
	break;
    }
}



long
xmlXPathOrderDocElems(xmlDocPtr doc) {
    long count = 0;
    xmlNodePtr cur;

    if (doc == NULL)
	return(-1);
    cur = doc->children;
    while (cur != NULL) {
	if (cur->type == XML_ELEMENT_NODE) {
	    cur->content = (void *) (-(++count));
	    if (cur->children != NULL) {
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
	    if (cur == (xmlNodePtr) doc) {
		cur = NULL;
		break;
	    }
	    if (cur->next != NULL) {
		cur = cur->next;
		break;
	    }
	} while (cur != NULL);
    }
    return(count);
}

int
xmlXPathCmpNodes(xmlNodePtr node1, xmlNodePtr node2) {
    int depth1, depth2;
    int attr1 = 0, attr2 = 0;
    xmlNodePtr attrNode1 = NULL, attrNode2 = NULL;
    xmlNodePtr cur, root;

    if ((node1 == NULL) || (node2 == NULL))
	return(-2);
    if (node1 == node2)		
	return(0);
    if (node1->type == XML_ATTRIBUTE_NODE) {
	attr1 = 1;
	attrNode1 = node1;
	node1 = node1->parent;
    }
    if (node2->type == XML_ATTRIBUTE_NODE) {
	attr2 = 1;
	attrNode2 = node2;
	node2 = node2->parent;
    }
    if (node1 == node2) {
	if (attr1 == attr2) {
	    
	    if (attr1 != 0) {
	        cur = attrNode2->prev;
		while (cur != NULL) {
		    if (cur == attrNode1)
		        return (1);
		    cur = cur->prev;
		}
		return (-1);
	    }
	    return(0);
	}
	if (attr2 == 1)
	    return(1);
	return(-1);
    }
    if ((node1->type == XML_NAMESPACE_DECL) ||
        (node2->type == XML_NAMESPACE_DECL))
	return(1);
    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);

    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {
	long l1, l2;

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

    for (depth2 = 0, cur = node2;cur->parent != NULL;cur = cur->parent) {
	if (cur == node1)
	    return(1);
	depth2++;
    }
    root = cur;
    for (depth1 = 0, cur = node1;cur->parent != NULL;cur = cur->parent) {
	if (cur == node2)
	    return(-1);
	depth1++;
    }
    if (root != cur) {
	return(-2);
    }
    while (depth1 > depth2) {
	depth1--;
	node1 = node1->parent;
    }
    while (depth2 > depth1) {
	depth2--;
	node2 = node2->parent;
    }
    while (node1->parent != node2->parent) {
	node1 = node1->parent;
	node2 = node2->parent;
	
	if ((node1 == NULL) || (node2 == NULL))
	    return(-2);
    }
    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);
    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {
	long l1, l2;

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

    for (cur = node1->next;cur != NULL;cur = cur->next)
	if (cur == node2)
	    return(1);
    return(-1); 
}

#ifdef XP_OPTIMIZED_NON_ELEM_COMPARISON
static int
xmlXPathCmpNodesExt(xmlNodePtr node1, xmlNodePtr node2) {
    int depth1, depth2;
    int misc = 0, precedence1 = 0, precedence2 = 0;
    xmlNodePtr miscNode1 = NULL, miscNode2 = NULL;
    xmlNodePtr cur, root;
    long l1, l2;

    if ((node1 == NULL) || (node2 == NULL))
	return(-2);

    if (node1 == node2)
	return(0);

    switch (node1->type) {
	case XML_ELEMENT_NODE:
	    if (node2->type == XML_ELEMENT_NODE) {
		if ((0 > (long) node1->content) && 
		    (0 > (long) node2->content) &&
		    (node1->doc == node2->doc))
		{
		    l1 = -((long) node1->content);
		    l2 = -((long) node2->content);
		    if (l1 < l2)
			return(1);
		    if (l1 > l2)
			return(-1);
		} else
		    goto turtle_comparison;
	    }
	    break;
	case XML_ATTRIBUTE_NODE:
	    precedence1 = 1; 
	    miscNode1 = node1;
	    node1 = node1->parent;
	    misc = 1;
	    break;
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_COMMENT_NODE:
	case XML_PI_NODE: {
	    miscNode1 = node1;
	    if (node1->prev != NULL) {
		do {
		    node1 = node1->prev;
		    if (node1->type == XML_ELEMENT_NODE) {
			precedence1 = 3; 
			break;
		    }
		    if (node1->prev == NULL) {
			precedence1 = 2; 
			node1 = node1->parent;
			break;
		    }
		} while (1);
	    } else {
		precedence1 = 2; 
		node1 = node1->parent;
	    }
	    if ((node1 == NULL) || (node1->type != XML_ELEMENT_NODE) ||
		(0 <= (long) node1->content)) {
		node1 = miscNode1;
		precedence1 = 0;
	    } else
		misc = 1;
	}
	    break;
	case XML_NAMESPACE_DECL:
	    return(1);
	default:
	    break;
    }
    switch (node2->type) {
	case XML_ELEMENT_NODE:
	    break;
	case XML_ATTRIBUTE_NODE:
	    precedence2 = 1; 
	    miscNode2 = node2;
	    node2 = node2->parent;
	    misc = 1;
	    break;
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_COMMENT_NODE:
	case XML_PI_NODE: {
	    miscNode2 = node2;
	    if (node2->prev != NULL) {
		do {
		    node2 = node2->prev;
		    if (node2->type == XML_ELEMENT_NODE) {
			precedence2 = 3; 
			break;
		    }
		    if (node2->prev == NULL) {
			precedence2 = 2; 
			node2 = node2->parent;
			break;
		    }
		} while (1);
	    } else {
		precedence2 = 2; 
		node2 = node2->parent;
	    }
	    if ((node2 == NULL) || (node2->type != XML_ELEMENT_NODE) ||
		(0 <= (long) node1->content))
	    {
		node2 = miscNode2;
		precedence2 = 0;
	    } else
		misc = 1;
	}
	    break;
	case XML_NAMESPACE_DECL:
	    return(1);
	default:
	    break;
    }
    if (misc) {
	if (node1 == node2) {
	    if (precedence1 == precedence2) {
		cur = miscNode2->prev;
		while (cur != NULL) {
		    if (cur == miscNode1)
			return(1);
		    if (cur->type == XML_ELEMENT_NODE)
			return(-1);
		    cur = cur->prev;
		}
		return (-1);
	    } else {
		if (precedence1 < precedence2)
		    return(1);
		else
		    return(-1);
	    }
	}
	if ((precedence2 == 3) && (precedence1 > 1)) {
	    cur = node1->parent;
	    while (cur) {
		if (cur == node2)
		    return(1);
		cur = cur->parent;
	    }
	}
	if ((precedence1 == 3) && (precedence2 > 1)) {
	    cur = node2->parent;
	    while (cur) {
		if (cur == node1)
		    return(-1);
		cur = cur->parent;
	    }
	}
    }

    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

turtle_comparison:

    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);
    for (depth2 = 0, cur = node2;cur->parent != NULL;cur = cur->parent) {
	if (cur == node1)
	    return(1);
	depth2++;
    }
    root = cur;
    for (depth1 = 0, cur = node1;cur->parent != NULL;cur = cur->parent) {
	if (cur == node2)
	    return(-1);
	depth1++;
    }
    if (root != cur) {
	return(-2);
    }
    while (depth1 > depth2) {
	depth1--;
	node1 = node1->parent;
    }
    while (depth2 > depth1) {
	depth2--;
	node2 = node2->parent;
    }
    while (node1->parent != node2->parent) {
	node1 = node1->parent;
	node2 = node2->parent;
	
	if ((node1 == NULL) || (node2 == NULL))
	    return(-2);
    }
    if (node1 == node2->prev)
	return(1);
    if (node1 == node2->next)
	return(-1);
    if ((node1->type == XML_ELEMENT_NODE) &&
	(node2->type == XML_ELEMENT_NODE) &&
	(0 > (long) node1->content) &&
	(0 > (long) node2->content) &&
	(node1->doc == node2->doc)) {

	l1 = -((long) node1->content);
	l2 = -((long) node2->content);
	if (l1 < l2)
	    return(1);
	if (l1 > l2)
	    return(-1);
    }

    for (cur = node1->next;cur != NULL;cur = cur->next)
	if (cur == node2)
	    return(1);
    return(-1); 
}
#endif 

void
xmlXPathNodeSetSort(xmlNodeSetPtr set) {
    int i, j, incr, len;
    xmlNodePtr tmp;

    if (set == NULL)
	return;

    
    len = set->nodeNr;
    for (incr = len / 2; incr > 0; incr /= 2) {
	for (i = incr; i < len; i++) {
	    j = i - incr;
	    while (j >= 0) {
#ifdef XP_OPTIMIZED_NON_ELEM_COMPARISON
		if (xmlXPathCmpNodesExt(set->nodeTab[j],
			set->nodeTab[j + incr]) == -1)
#else
		if (xmlXPathCmpNodes(set->nodeTab[j],
			set->nodeTab[j + incr]) == -1)
#endif
		{
		    tmp = set->nodeTab[j];
		    set->nodeTab[j] = set->nodeTab[j + incr];
		    set->nodeTab[j + incr] = tmp;
		    j -= incr;
		} else
		    break;
	    }
	}
    }
}

#define XML_NODESET_DEFAULT	10
static xmlNodePtr
xmlXPathNodeSetDupNs(xmlNodePtr node, xmlNsPtr ns) {
    xmlNsPtr cur;

    if ((ns == NULL) || (ns->type != XML_NAMESPACE_DECL))
	return(NULL);
    if ((node == NULL) || (node->type == XML_NAMESPACE_DECL))
	return((xmlNodePtr) ns);

    cur = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
    if (cur == NULL) {
        xmlXPathErrMemory(NULL, "duplicating namespace\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNs));
    cur->type = XML_NAMESPACE_DECL;
    if (ns->href != NULL)
	cur->href = xmlStrdup(ns->href);
    if (ns->prefix != NULL)
	cur->prefix = xmlStrdup(ns->prefix);
    cur->next = (xmlNsPtr) node;
    return((xmlNodePtr) cur);
}

void
xmlXPathNodeSetFreeNs(xmlNsPtr ns) {
    if ((ns == NULL) || (ns->type != XML_NAMESPACE_DECL))
	return;

    if ((ns->next != NULL) && (ns->next->type != XML_NAMESPACE_DECL)) {
	if (ns->href != NULL)
	    xmlFree((xmlChar *)ns->href);
	if (ns->prefix != NULL)
	    xmlFree((xmlChar *)ns->prefix);
	xmlFree(ns);
    }
}

xmlNodeSetPtr
xmlXPathNodeSetCreate(xmlNodePtr val) {
    xmlNodeSetPtr ret;

    ret = (xmlNodeSetPtr) xmlMalloc(sizeof(xmlNodeSet));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating nodeset\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlNodeSet));
    if (val != NULL) {
        ret->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
					     sizeof(xmlNodePtr));
	if (ret->nodeTab == NULL) {
	    xmlXPathErrMemory(NULL, "creating nodeset\n");
	    xmlFree(ret);
	    return(NULL);
	}
	memset(ret->nodeTab, 0 ,
	       XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
        ret->nodeMax = XML_NODESET_DEFAULT;
	if (val->type == XML_NAMESPACE_DECL) {
	    xmlNsPtr ns = (xmlNsPtr) val;

	    ret->nodeTab[ret->nodeNr++] =
		xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
	} else
	    ret->nodeTab[ret->nodeNr++] = val;
    }
    return(ret);
}

static xmlNodeSetPtr
xmlXPathNodeSetCreateSize(int size) {
    xmlNodeSetPtr ret;

    ret = (xmlNodeSetPtr) xmlMalloc(sizeof(xmlNodeSet));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating nodeset\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlNodeSet));
    if (size < XML_NODESET_DEFAULT)
	size = XML_NODESET_DEFAULT;
    ret->nodeTab = (xmlNodePtr *) xmlMalloc(size * sizeof(xmlNodePtr));
    if (ret->nodeTab == NULL) {
	xmlXPathErrMemory(NULL, "creating nodeset\n");
	xmlFree(ret);
	return(NULL);
    }
    memset(ret->nodeTab, 0 , size * (size_t) sizeof(xmlNodePtr));
    ret->nodeMax = size;
    return(ret);
}

int
xmlXPathNodeSetContains (xmlNodeSetPtr cur, xmlNodePtr val) {
    int i;

    if ((cur == NULL) || (val == NULL)) return(0);
    if (val->type == XML_NAMESPACE_DECL) {
	for (i = 0; i < cur->nodeNr; i++) {
	    if (cur->nodeTab[i]->type == XML_NAMESPACE_DECL) {
		xmlNsPtr ns1, ns2;

		ns1 = (xmlNsPtr) val;
		ns2 = (xmlNsPtr) cur->nodeTab[i];
		if (ns1 == ns2)
		    return(1);
		if ((ns1->next != NULL) && (ns2->next == ns1->next) &&
	            (xmlStrEqual(ns1->prefix, ns2->prefix)))
		    return(1);
	    }
	}
    } else {
	for (i = 0; i < cur->nodeNr; i++) {
	    if (cur->nodeTab[i] == val)
		return(1);
	}
    }
    return(0);
}

void
xmlXPathNodeSetAddNs(xmlNodeSetPtr cur, xmlNodePtr node, xmlNsPtr ns) {
    int i;


    if ((cur == NULL) || (ns == NULL) || (node == NULL) ||
        (ns->type != XML_NAMESPACE_DECL) ||
	(node->type != XML_ELEMENT_NODE))
	return;

    
    for (i = 0;i < cur->nodeNr;i++) {
        if ((cur->nodeTab[i] != NULL) &&
	    (cur->nodeTab[i]->type == XML_NAMESPACE_DECL) &&
	    (((xmlNsPtr)cur->nodeTab[i])->next == (xmlNsPtr) node) &&
	    (xmlStrEqual(ns->prefix, ((xmlNsPtr)cur->nodeTab[i])->prefix)))
	    return;
    }

    if (cur->nodeMax == 0) {
        cur->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
					     sizeof(xmlNodePtr));
	if (cur->nodeTab == NULL) {
	    xmlXPathErrMemory(NULL, "growing nodeset\n");
	    return;
	}
	memset(cur->nodeTab, 0 ,
	       XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
        cur->nodeMax = XML_NODESET_DEFAULT;
    } else if (cur->nodeNr == cur->nodeMax) {
        xmlNodePtr *temp;

        cur->nodeMax *= 2;
	temp = (xmlNodePtr *) xmlRealloc(cur->nodeTab, cur->nodeMax *
				      sizeof(xmlNodePtr));
	if (temp == NULL) {
	    xmlXPathErrMemory(NULL, "growing nodeset\n");
	    return;
	}
	cur->nodeTab = temp;
    }
    cur->nodeTab[cur->nodeNr++] = xmlXPathNodeSetDupNs(node, ns);
}

void
xmlXPathNodeSetAdd(xmlNodeSetPtr cur, xmlNodePtr val) {
    int i;

    if ((cur == NULL) || (val == NULL)) return;

#if 0
    if ((val->type == XML_ELEMENT_NODE) && (val->name[0] == ' '))
	return;	
#endif

    
    for (i = 0;i < cur->nodeNr;i++)
        if (cur->nodeTab[i] == val) return;

    if (cur->nodeMax == 0) {
        cur->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
					     sizeof(xmlNodePtr));
	if (cur->nodeTab == NULL) {
	    xmlXPathErrMemory(NULL, "growing nodeset\n");
	    return;
	}
	memset(cur->nodeTab, 0 ,
	       XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
        cur->nodeMax = XML_NODESET_DEFAULT;
    } else if (cur->nodeNr == cur->nodeMax) {
        xmlNodePtr *temp;

        cur->nodeMax *= 2;
	temp = (xmlNodePtr *) xmlRealloc(cur->nodeTab, cur->nodeMax *
				      sizeof(xmlNodePtr));
	if (temp == NULL) {
	    xmlXPathErrMemory(NULL, "growing nodeset\n");
	    return;
	}
	cur->nodeTab = temp;
    }
    if (val->type == XML_NAMESPACE_DECL) {
	xmlNsPtr ns = (xmlNsPtr) val;

	cur->nodeTab[cur->nodeNr++] =
	    xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
    } else
	cur->nodeTab[cur->nodeNr++] = val;
}

void
xmlXPathNodeSetAddUnique(xmlNodeSetPtr cur, xmlNodePtr val) {
    if ((cur == NULL) || (val == NULL)) return;

#if 0
    if ((val->type == XML_ELEMENT_NODE) && (val->name[0] == ' '))
	return;	
#endif

    
    if (cur->nodeMax == 0) {
        cur->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
					     sizeof(xmlNodePtr));
	if (cur->nodeTab == NULL) {
	    xmlXPathErrMemory(NULL, "growing nodeset\n");
	    return;
	}
	memset(cur->nodeTab, 0 ,
	       XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
        cur->nodeMax = XML_NODESET_DEFAULT;
    } else if (cur->nodeNr == cur->nodeMax) {
        xmlNodePtr *temp;

        cur->nodeMax *= 2;
	temp = (xmlNodePtr *) xmlRealloc(cur->nodeTab, cur->nodeMax *
				      sizeof(xmlNodePtr));
	if (temp == NULL) {
	    xmlXPathErrMemory(NULL, "growing nodeset\n");
	    return;
	}
	cur->nodeTab = temp;
    }
    if (val->type == XML_NAMESPACE_DECL) {
	xmlNsPtr ns = (xmlNsPtr) val;

	cur->nodeTab[cur->nodeNr++] =
	    xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
    } else
	cur->nodeTab[cur->nodeNr++] = val;
}

xmlNodeSetPtr
xmlXPathNodeSetMerge(xmlNodeSetPtr val1, xmlNodeSetPtr val2) {
    int i, j, initNr, skip;
    xmlNodePtr n1, n2;

    if (val2 == NULL) return(val1);
    if (val1 == NULL) {
	val1 = xmlXPathNodeSetCreate(NULL);
    if (val1 == NULL)
        return (NULL);
#if 0
	val1 = xmlXPathNodeSetCreateSize(val2->nodeNr);
	if (val1 == NULL)
	    return(NULL);
	if (val2->nodeNr != 0) {
	    if (val2->nodeNr == 1)
		*(val1->nodeTab) = *(val2->nodeTab);
	    else {
		memcpy(val1->nodeTab, val2->nodeTab,
		    val2->nodeNr * sizeof(xmlNodePtr));
	    }
	    val1->nodeNr = val2->nodeNr;
	}
	return(val1);
#endif
    }

    
    initNr = val1->nodeNr;

    for (i = 0;i < val2->nodeNr;i++) {
	n2 = val2->nodeTab[i];
	skip = 0;
	for (j = 0; j < initNr; j++) {
	    n1 = val1->nodeTab[j];
	    if (n1 == n2) {
		skip = 1;
		break;
	    } else if ((n1->type == XML_NAMESPACE_DECL) &&
		       (n2->type == XML_NAMESPACE_DECL)) {
		if ((((xmlNsPtr) n1)->next == ((xmlNsPtr) n2)->next) &&
		    (xmlStrEqual(((xmlNsPtr) n1)->prefix,
			((xmlNsPtr) n2)->prefix)))
		{
		    skip = 1;
		    break;
		}
	    }
	}
	if (skip)
	    continue;

	if (val1->nodeMax == 0) {
	    val1->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
						    sizeof(xmlNodePtr));
	    if (val1->nodeTab == NULL) {
	        xmlXPathErrMemory(NULL, "merging nodeset\n");
		return(NULL);
	    }
	    memset(val1->nodeTab, 0 ,
		   XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
	    val1->nodeMax = XML_NODESET_DEFAULT;
	} else if (val1->nodeNr == val1->nodeMax) {
	    xmlNodePtr *temp;

	    val1->nodeMax *= 2;
	    temp = (xmlNodePtr *) xmlRealloc(val1->nodeTab, val1->nodeMax *
					     sizeof(xmlNodePtr));
	    if (temp == NULL) {
	        xmlXPathErrMemory(NULL, "merging nodeset\n");
		return(NULL);
	    }
	    val1->nodeTab = temp;
	}
	if (n2->type == XML_NAMESPACE_DECL) {
	    xmlNsPtr ns = (xmlNsPtr) n2;

	    val1->nodeTab[val1->nodeNr++] =
		xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
	} else
	    val1->nodeTab[val1->nodeNr++] = n2;
    }

    return(val1);
}

#if 0 
static xmlNodeSetPtr
xmlXPathNodeSetMergeUnique(xmlNodeSetPtr val1, xmlNodeSetPtr val2) {
    int i;

    if (val2 == NULL) return(val1);
    if (val1 == NULL) {
	val1 = xmlXPathNodeSetCreate(NULL);
    }
    if (val1 == NULL)
        return (NULL);

    

    for (i = 0;i < val2->nodeNr;i++) {
	if (val1->nodeMax == 0) {
	    val1->nodeTab = (xmlNodePtr *) xmlMalloc(XML_NODESET_DEFAULT *
						    sizeof(xmlNodePtr));
	    if (val1->nodeTab == NULL) {
	        xmlXPathErrMemory(NULL, "merging nodeset\n");
		return(NULL);
	    }
	    memset(val1->nodeTab, 0 ,
		   XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
	    val1->nodeMax = XML_NODESET_DEFAULT;
	} else if (val1->nodeNr == val1->nodeMax) {
	    xmlNodePtr *temp;

	    val1->nodeMax *= 2;
	    temp = (xmlNodePtr *) xmlRealloc(val1->nodeTab, val1->nodeMax *
					     sizeof(xmlNodePtr));
	    if (temp == NULL) {
	        xmlXPathErrMemory(NULL, "merging nodeset\n");
		return(NULL);
	    }
	    val1->nodeTab = temp;
	}
	if (val2->nodeTab[i]->type == XML_NAMESPACE_DECL) {
	    xmlNsPtr ns = (xmlNsPtr) val2->nodeTab[i];

	    val1->nodeTab[val1->nodeNr++] =
		xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
	} else
	    val1->nodeTab[val1->nodeNr++] = val2->nodeTab[i];
    }

    return(val1);
}
#endif 

static xmlNodeSetPtr
xmlXPathNodeSetMergeAndClear(xmlNodeSetPtr set1, xmlNodeSetPtr set2,
			     int hasNullEntries)
{
    if ((set1 == NULL) && (hasNullEntries == 0)) {
	set1 = xmlXPathNodeSetCreateSize(set2->nodeNr);
	if (set1 == NULL)
	    return(NULL);
	if (set2->nodeNr != 0) {
	    memcpy(set1->nodeTab, set2->nodeTab,
		set2->nodeNr * sizeof(xmlNodePtr));
	    set1->nodeNr = set2->nodeNr;
	}
    } else {
	int i, j, initNbSet1;
	xmlNodePtr n1, n2;

	if (set1 == NULL)
            set1 = xmlXPathNodeSetCreate(NULL);
        if (set1 == NULL)
            return (NULL);

	initNbSet1 = set1->nodeNr;
	for (i = 0;i < set2->nodeNr;i++) {
	    n2 = set2->nodeTab[i];
	    if (n2 == NULL)
		continue;
	    for (j = 0; j < initNbSet1; j++) {
		n1 = set1->nodeTab[j];
		if (n1 == n2) {
		    goto skip_node;
		} else if ((n1->type == XML_NAMESPACE_DECL) &&
		    (n2->type == XML_NAMESPACE_DECL))
		{
		    if ((((xmlNsPtr) n1)->next == ((xmlNsPtr) n2)->next) &&
			(xmlStrEqual(((xmlNsPtr) n1)->prefix,
			((xmlNsPtr) n2)->prefix)))
		    {
			set2->nodeTab[i] = NULL;
			xmlXPathNodeSetFreeNs((xmlNsPtr) n2);
			goto skip_node;
		    }
		}
	    }
	    if (set1->nodeMax == 0) {
		set1->nodeTab = (xmlNodePtr *) xmlMalloc(
		    XML_NODESET_DEFAULT * sizeof(xmlNodePtr));
		if (set1->nodeTab == NULL) {
		    xmlXPathErrMemory(NULL, "merging nodeset\n");
		    return(NULL);
		}
		memset(set1->nodeTab, 0,
		    XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
		set1->nodeMax = XML_NODESET_DEFAULT;
	    } else if (set1->nodeNr >= set1->nodeMax) {
		xmlNodePtr *temp;

		set1->nodeMax *= 2;
		temp = (xmlNodePtr *) xmlRealloc(
		    set1->nodeTab, set1->nodeMax * sizeof(xmlNodePtr));
		if (temp == NULL) {
		    xmlXPathErrMemory(NULL, "merging nodeset\n");
		    return(NULL);
		}
		set1->nodeTab = temp;
	    }
	    if (n2->type == XML_NAMESPACE_DECL) {
		xmlNsPtr ns = (xmlNsPtr) n2;

		set1->nodeTab[set1->nodeNr++] =
		    xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
	    } else
		set1->nodeTab[set1->nodeNr++] = n2;
skip_node:
	    {}
	}
    }
    set2->nodeNr = 0;
    return(set1);
}

static xmlNodeSetPtr
xmlXPathNodeSetMergeAndClearNoDupls(xmlNodeSetPtr set1, xmlNodeSetPtr set2,
				    int hasNullEntries)
{
    if (set2 == NULL)
	return(set1);
    if ((set1 == NULL) && (hasNullEntries == 0)) {
	set1 = xmlXPathNodeSetCreateSize(set2->nodeNr);
	if (set1 == NULL)
	    return(NULL);
	if (set2->nodeNr != 0) {
	    memcpy(set1->nodeTab, set2->nodeTab,
		set2->nodeNr * sizeof(xmlNodePtr));
	    set1->nodeNr = set2->nodeNr;
	}
    } else {
	int i;
	xmlNodePtr n2;

	if (set1 == NULL)
	    set1 = xmlXPathNodeSetCreate(NULL);
        if (set1 == NULL)
            return (NULL);

	for (i = 0;i < set2->nodeNr;i++) {
	    n2 = set2->nodeTab[i];
	    if (n2 == NULL)
		continue;
	    if (set1->nodeMax == 0) {
		set1->nodeTab = (xmlNodePtr *) xmlMalloc(
		    XML_NODESET_DEFAULT * sizeof(xmlNodePtr));
		if (set1->nodeTab == NULL) {
		    xmlXPathErrMemory(NULL, "merging nodeset\n");
		    return(NULL);
		}
		memset(set1->nodeTab, 0,
		    XML_NODESET_DEFAULT * (size_t) sizeof(xmlNodePtr));
		set1->nodeMax = XML_NODESET_DEFAULT;
	    } else if (set1->nodeNr >= set1->nodeMax) {
		xmlNodePtr *temp;

		set1->nodeMax *= 2;
		temp = (xmlNodePtr *) xmlRealloc(
		    set1->nodeTab, set1->nodeMax * sizeof(xmlNodePtr));
		if (temp == NULL) {
		    xmlXPathErrMemory(NULL, "merging nodeset\n");
		    return(NULL);
		}
		set1->nodeTab = temp;
	    }
	    set1->nodeTab[set1->nodeNr++] = n2;
	}
    }
    set2->nodeNr = 0;
    return(set1);
}

void
xmlXPathNodeSetDel(xmlNodeSetPtr cur, xmlNodePtr val) {
    int i;

    if (cur == NULL) return;
    if (val == NULL) return;

    for (i = 0;i < cur->nodeNr;i++)
        if (cur->nodeTab[i] == val) break;

    if (i >= cur->nodeNr) {	
#ifdef DEBUG
        xmlGenericError(xmlGenericErrorContext,
	        "xmlXPathNodeSetDel: Node %s wasn't found in NodeList\n",
		val->name);
#endif
        return;
    }
    if ((cur->nodeTab[i] != NULL) &&
	(cur->nodeTab[i]->type == XML_NAMESPACE_DECL))
	xmlXPathNodeSetFreeNs((xmlNsPtr) cur->nodeTab[i]);
    cur->nodeNr--;
    for (;i < cur->nodeNr;i++)
        cur->nodeTab[i] = cur->nodeTab[i + 1];
    cur->nodeTab[cur->nodeNr] = NULL;
}

void
xmlXPathNodeSetRemove(xmlNodeSetPtr cur, int val) {
    if (cur == NULL) return;
    if (val >= cur->nodeNr) return;
    if ((cur->nodeTab[val] != NULL) &&
	(cur->nodeTab[val]->type == XML_NAMESPACE_DECL))
	xmlXPathNodeSetFreeNs((xmlNsPtr) cur->nodeTab[val]);
    cur->nodeNr--;
    for (;val < cur->nodeNr;val++)
        cur->nodeTab[val] = cur->nodeTab[val + 1];
    cur->nodeTab[cur->nodeNr] = NULL;
}

void
xmlXPathFreeNodeSet(xmlNodeSetPtr obj) {
    if (obj == NULL) return;
    if (obj->nodeTab != NULL) {
	int i;

	
	for (i = 0;i < obj->nodeNr;i++)
	    if ((obj->nodeTab[i] != NULL) &&
		(obj->nodeTab[i]->type == XML_NAMESPACE_DECL))
		xmlXPathNodeSetFreeNs((xmlNsPtr) obj->nodeTab[i]);
	xmlFree(obj->nodeTab);
    }
    xmlFree(obj);
}

static void
xmlXPathNodeSetClear(xmlNodeSetPtr set, int hasNsNodes)
{
    if ((set == NULL) || (set->nodeNr <= 0))
	return;
    else if (hasNsNodes) {
	int i;
	xmlNodePtr node;

	for (i = 0; i < set->nodeNr; i++) {
	    node = set->nodeTab[i];
	    if ((node != NULL) &&
		(node->type == XML_NAMESPACE_DECL))
		xmlXPathNodeSetFreeNs((xmlNsPtr) node);
	}
    }
    set->nodeNr = 0;
}

static void
xmlXPathNodeSetClearFromPos(xmlNodeSetPtr set, int pos, int hasNsNodes)
{
    if ((set == NULL) || (set->nodeNr <= 0) || (pos >= set->nodeNr))
	return;
    else if ((hasNsNodes)) {
	int i;
	xmlNodePtr node;

	for (i = pos; i < set->nodeNr; i++) {
	    node = set->nodeTab[i];
	    if ((node != NULL) &&
		(node->type == XML_NAMESPACE_DECL))
		xmlXPathNodeSetFreeNs((xmlNsPtr) node);
	}
    }
    set->nodeNr = pos;
}

static void
xmlXPathFreeValueTree(xmlNodeSetPtr obj) {
    int i;

    if (obj == NULL) return;

    if (obj->nodeTab != NULL) {
	for (i = 0;i < obj->nodeNr;i++) {
	    if (obj->nodeTab[i] != NULL) {
		if (obj->nodeTab[i]->type == XML_NAMESPACE_DECL) {
		    xmlXPathNodeSetFreeNs((xmlNsPtr) obj->nodeTab[i]);
		} else {
		    xmlFreeNodeList(obj->nodeTab[i]);
		}
	    }
	}
	xmlFree(obj->nodeTab);
    }
    xmlFree(obj);
}

#if defined(DEBUG) || defined(DEBUG_STEP)
void
xmlGenericErrorContextNodeSet(FILE *output, xmlNodeSetPtr obj) {
    int i;

    if (output == NULL) output = xmlGenericErrorContext;
    if (obj == NULL)  {
        fprintf(output, "NodeSet == NULL !\n");
	return;
    }
    if (obj->nodeNr == 0) {
        fprintf(output, "NodeSet is empty\n");
	return;
    }
    if (obj->nodeTab == NULL) {
	fprintf(output, " nodeTab == NULL !\n");
	return;
    }
    for (i = 0; i < obj->nodeNr; i++) {
        if (obj->nodeTab[i] == NULL) {
	    fprintf(output, " NULL !\n");
	    return;
        }
	if ((obj->nodeTab[i]->type == XML_DOCUMENT_NODE) ||
	    (obj->nodeTab[i]->type == XML_HTML_DOCUMENT_NODE))
	    fprintf(output, " /");
	else if (obj->nodeTab[i]->name == NULL)
	    fprintf(output, " noname!");
	else fprintf(output, " %s", obj->nodeTab[i]->name);
    }
    fprintf(output, "\n");
}
#endif

xmlXPathObjectPtr
xmlXPathNewNodeSet(xmlNodePtr val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating nodeset\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_NODESET;
    ret->boolval = 0;
    ret->nodesetval = xmlXPathNodeSetCreate(val);
    
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_NODESET);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathNewValueTree(xmlNodePtr val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating result value tree\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_XSLT_TREE;
    ret->boolval = 1;
    ret->user = (void *) val;
    ret->nodesetval = xmlXPathNodeSetCreate(val);
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_XSLT_TREE);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathNewNodeSetList(xmlNodeSetPtr val)
{
    xmlXPathObjectPtr ret;
    int i;

    if (val == NULL)
        ret = NULL;
    else if (val->nodeTab == NULL)
        ret = xmlXPathNewNodeSet(NULL);
    else {
        ret = xmlXPathNewNodeSet(val->nodeTab[0]);
        if (ret)
            for (i = 1; i < val->nodeNr; ++i)
                xmlXPathNodeSetAddUnique(ret->nodesetval, val->nodeTab[i]);
    }

    return (ret);
}

xmlXPathObjectPtr
xmlXPathWrapNodeSet(xmlNodeSetPtr val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating node set object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_NODESET;
    ret->nodesetval = val;
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_NODESET);
#endif
    return(ret);
}

void
xmlXPathFreeNodeSetList(xmlXPathObjectPtr obj) {
    if (obj == NULL) return;
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageReleased(NULL, obj->type);
#endif
    xmlFree(obj);
}

xmlNodeSetPtr
xmlXPathDifference (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    xmlNodeSetPtr ret;
    int i, l1;
    xmlNodePtr cur;

    if (xmlXPathNodeSetIsEmpty(nodes2))
	return(nodes1);

    ret = xmlXPathNodeSetCreate(NULL);
    if (xmlXPathNodeSetIsEmpty(nodes1))
	return(ret);

    l1 = xmlXPathNodeSetGetLength(nodes1);

    for (i = 0; i < l1; i++) {
	cur = xmlXPathNodeSetItem(nodes1, i);
	if (!xmlXPathNodeSetContains(nodes2, cur))
	    xmlXPathNodeSetAddUnique(ret, cur);
    }
    return(ret);
}

xmlNodeSetPtr
xmlXPathIntersection (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    xmlNodeSetPtr ret = xmlXPathNodeSetCreate(NULL);
    int i, l1;
    xmlNodePtr cur;

    if (ret == NULL)
        return(ret);
    if (xmlXPathNodeSetIsEmpty(nodes1))
	return(ret);
    if (xmlXPathNodeSetIsEmpty(nodes2))
	return(ret);

    l1 = xmlXPathNodeSetGetLength(nodes1);

    for (i = 0; i < l1; i++) {
	cur = xmlXPathNodeSetItem(nodes1, i);
	if (xmlXPathNodeSetContains(nodes2, cur))
	    xmlXPathNodeSetAddUnique(ret, cur);
    }
    return(ret);
}

xmlNodeSetPtr
xmlXPathDistinctSorted (xmlNodeSetPtr nodes) {
    xmlNodeSetPtr ret;
    xmlHashTablePtr hash;
    int i, l;
    xmlChar * strval;
    xmlNodePtr cur;

    if (xmlXPathNodeSetIsEmpty(nodes))
	return(nodes);

    ret = xmlXPathNodeSetCreate(NULL);
    if (ret == NULL)
        return(ret);
    l = xmlXPathNodeSetGetLength(nodes);
    hash = xmlHashCreate (l);
    for (i = 0; i < l; i++) {
	cur = xmlXPathNodeSetItem(nodes, i);
	strval = xmlXPathCastNodeToString(cur);
	if (xmlHashLookup(hash, strval) == NULL) {
	    xmlHashAddEntry(hash, strval, strval);
	    xmlXPathNodeSetAddUnique(ret, cur);
	} else {
	    xmlFree(strval);
	}
    }
    xmlHashFree(hash, (xmlHashDeallocator) xmlFree);
    return(ret);
}

xmlNodeSetPtr
xmlXPathDistinct (xmlNodeSetPtr nodes) {
    if (xmlXPathNodeSetIsEmpty(nodes))
	return(nodes);

    xmlXPathNodeSetSort(nodes);
    return(xmlXPathDistinctSorted(nodes));
}

int
xmlXPathHasSameNodes (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    int i, l;
    xmlNodePtr cur;

    if (xmlXPathNodeSetIsEmpty(nodes1) ||
	xmlXPathNodeSetIsEmpty(nodes2))
	return(0);

    l = xmlXPathNodeSetGetLength(nodes1);
    for (i = 0; i < l; i++) {
	cur = xmlXPathNodeSetItem(nodes1, i);
	if (xmlXPathNodeSetContains(nodes2, cur))
	    return(1);
    }
    return(0);
}

xmlNodeSetPtr
xmlXPathNodeLeadingSorted (xmlNodeSetPtr nodes, xmlNodePtr node) {
    int i, l;
    xmlNodePtr cur;
    xmlNodeSetPtr ret;

    if (node == NULL)
	return(nodes);

    ret = xmlXPathNodeSetCreate(NULL);
    if (ret == NULL)
        return(ret);
    if (xmlXPathNodeSetIsEmpty(nodes) ||
	(!xmlXPathNodeSetContains(nodes, node)))
	return(ret);

    l = xmlXPathNodeSetGetLength(nodes);
    for (i = 0; i < l; i++) {
	cur = xmlXPathNodeSetItem(nodes, i);
	if (cur == node)
	    break;
	xmlXPathNodeSetAddUnique(ret, cur);
    }
    return(ret);
}

xmlNodeSetPtr
xmlXPathNodeLeading (xmlNodeSetPtr nodes, xmlNodePtr node) {
    xmlXPathNodeSetSort(nodes);
    return(xmlXPathNodeLeadingSorted(nodes, node));
}

xmlNodeSetPtr
xmlXPathLeadingSorted (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    if (xmlXPathNodeSetIsEmpty(nodes2))
	return(nodes1);
    return(xmlXPathNodeLeadingSorted(nodes1,
				     xmlXPathNodeSetItem(nodes2, 1)));
}

xmlNodeSetPtr
xmlXPathLeading (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    if (xmlXPathNodeSetIsEmpty(nodes2))
	return(nodes1);
    if (xmlXPathNodeSetIsEmpty(nodes1))
	return(xmlXPathNodeSetCreate(NULL));
    xmlXPathNodeSetSort(nodes1);
    xmlXPathNodeSetSort(nodes2);
    return(xmlXPathNodeLeadingSorted(nodes1,
				     xmlXPathNodeSetItem(nodes2, 1)));
}

xmlNodeSetPtr
xmlXPathNodeTrailingSorted (xmlNodeSetPtr nodes, xmlNodePtr node) {
    int i, l;
    xmlNodePtr cur;
    xmlNodeSetPtr ret;

    if (node == NULL)
	return(nodes);

    ret = xmlXPathNodeSetCreate(NULL);
    if (ret == NULL)
        return(ret);
    if (xmlXPathNodeSetIsEmpty(nodes) ||
	(!xmlXPathNodeSetContains(nodes, node)))
	return(ret);

    l = xmlXPathNodeSetGetLength(nodes);
    for (i = l - 1; i >= 0; i--) {
	cur = xmlXPathNodeSetItem(nodes, i);
	if (cur == node)
	    break;
	xmlXPathNodeSetAddUnique(ret, cur);
    }
    xmlXPathNodeSetSort(ret);	
    return(ret);
}

xmlNodeSetPtr
xmlXPathNodeTrailing (xmlNodeSetPtr nodes, xmlNodePtr node) {
    xmlXPathNodeSetSort(nodes);
    return(xmlXPathNodeTrailingSorted(nodes, node));
}

xmlNodeSetPtr
xmlXPathTrailingSorted (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    if (xmlXPathNodeSetIsEmpty(nodes2))
	return(nodes1);
    return(xmlXPathNodeTrailingSorted(nodes1,
				      xmlXPathNodeSetItem(nodes2, 0)));
}

xmlNodeSetPtr
xmlXPathTrailing (xmlNodeSetPtr nodes1, xmlNodeSetPtr nodes2) {
    if (xmlXPathNodeSetIsEmpty(nodes2))
	return(nodes1);
    if (xmlXPathNodeSetIsEmpty(nodes1))
	return(xmlXPathNodeSetCreate(NULL));
    xmlXPathNodeSetSort(nodes1);
    xmlXPathNodeSetSort(nodes2);
    return(xmlXPathNodeTrailingSorted(nodes1,
				      xmlXPathNodeSetItem(nodes2, 0)));
}


int
xmlXPathRegisterFunc(xmlXPathContextPtr ctxt, const xmlChar *name,
		     xmlXPathFunction f) {
    return(xmlXPathRegisterFuncNS(ctxt, name, NULL, f));
}

int
xmlXPathRegisterFuncNS(xmlXPathContextPtr ctxt, const xmlChar *name,
		       const xmlChar *ns_uri, xmlXPathFunction f) {
    if (ctxt == NULL)
	return(-1);
    if (name == NULL)
	return(-1);

    if (ctxt->funcHash == NULL)
	ctxt->funcHash = xmlHashCreate(0);
    if (ctxt->funcHash == NULL)
	return(-1);
    if (f == NULL)
        return(xmlHashRemoveEntry2(ctxt->funcHash, name, ns_uri, NULL));
    return(xmlHashAddEntry2(ctxt->funcHash, name, ns_uri, XML_CAST_FPTR(f)));
}

void
xmlXPathRegisterFuncLookup (xmlXPathContextPtr ctxt,
			    xmlXPathFuncLookupFunc f,
			    void *funcCtxt) {
    if (ctxt == NULL)
	return;
    ctxt->funcLookupFunc = f;
    ctxt->funcLookupData = funcCtxt;
}

xmlXPathFunction
xmlXPathFunctionLookup(xmlXPathContextPtr ctxt, const xmlChar *name) {
    if (ctxt == NULL)
	return (NULL);

    if (ctxt->funcLookupFunc != NULL) {
	xmlXPathFunction ret;
	xmlXPathFuncLookupFunc f;

	f = ctxt->funcLookupFunc;
	ret = f(ctxt->funcLookupData, name, NULL);
	if (ret != NULL)
	    return(ret);
    }
    return(xmlXPathFunctionLookupNS(ctxt, name, NULL));
}

xmlXPathFunction
xmlXPathFunctionLookupNS(xmlXPathContextPtr ctxt, const xmlChar *name,
			 const xmlChar *ns_uri) {
    xmlXPathFunction ret;

    if (ctxt == NULL)
	return(NULL);
    if (name == NULL)
	return(NULL);

    if (ctxt->funcLookupFunc != NULL) {
	xmlXPathFuncLookupFunc f;

	f = ctxt->funcLookupFunc;
	ret = f(ctxt->funcLookupData, name, ns_uri);
	if (ret != NULL)
	    return(ret);
    }

    if (ctxt->funcHash == NULL)
	return(NULL);

    XML_CAST_FPTR(ret) = xmlHashLookup2(ctxt->funcHash, name, ns_uri);
    return(ret);
}

void
xmlXPathRegisteredFuncsCleanup(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL)
	return;

    xmlHashFree(ctxt->funcHash, NULL);
    ctxt->funcHash = NULL;
}


int
xmlXPathRegisterVariable(xmlXPathContextPtr ctxt, const xmlChar *name,
			 xmlXPathObjectPtr value) {
    return(xmlXPathRegisterVariableNS(ctxt, name, NULL, value));
}

int
xmlXPathRegisterVariableNS(xmlXPathContextPtr ctxt, const xmlChar *name,
			   const xmlChar *ns_uri,
			   xmlXPathObjectPtr value) {
    if (ctxt == NULL)
	return(-1);
    if (name == NULL)
	return(-1);

    if (ctxt->varHash == NULL)
	ctxt->varHash = xmlHashCreate(0);
    if (ctxt->varHash == NULL)
	return(-1);
    if (value == NULL)
        return(xmlHashRemoveEntry2(ctxt->varHash, name, ns_uri,
	                           (xmlHashDeallocator)xmlXPathFreeObject));
    return(xmlHashUpdateEntry2(ctxt->varHash, name, ns_uri,
			       (void *) value,
			       (xmlHashDeallocator)xmlXPathFreeObject));
}

void
xmlXPathRegisterVariableLookup(xmlXPathContextPtr ctxt,
	 xmlXPathVariableLookupFunc f, void *data) {
    if (ctxt == NULL)
	return;
    ctxt->varLookupFunc = f;
    ctxt->varLookupData = data;
}

xmlXPathObjectPtr
xmlXPathVariableLookup(xmlXPathContextPtr ctxt, const xmlChar *name) {
    if (ctxt == NULL)
	return(NULL);

    if (ctxt->varLookupFunc != NULL) {
	xmlXPathObjectPtr ret;

	ret = ((xmlXPathVariableLookupFunc)ctxt->varLookupFunc)
	        (ctxt->varLookupData, name, NULL);
	return(ret);
    }
    return(xmlXPathVariableLookupNS(ctxt, name, NULL));
}

xmlXPathObjectPtr
xmlXPathVariableLookupNS(xmlXPathContextPtr ctxt, const xmlChar *name,
			 const xmlChar *ns_uri) {
    if (ctxt == NULL)
	return(NULL);

    if (ctxt->varLookupFunc != NULL) {
	xmlXPathObjectPtr ret;

	ret = ((xmlXPathVariableLookupFunc)ctxt->varLookupFunc)
	        (ctxt->varLookupData, name, ns_uri);
	if (ret != NULL) return(ret);
    }

    if (ctxt->varHash == NULL)
	return(NULL);
    if (name == NULL)
	return(NULL);

    return(xmlXPathCacheObjectCopy(ctxt, (xmlXPathObjectPtr)
		xmlHashLookup2(ctxt->varHash, name, ns_uri)));
}

void
xmlXPathRegisteredVariablesCleanup(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL)
	return;

    xmlHashFree(ctxt->varHash, (xmlHashDeallocator)xmlXPathFreeObject);
    ctxt->varHash = NULL;
}

int
xmlXPathRegisterNs(xmlXPathContextPtr ctxt, const xmlChar *prefix,
			   const xmlChar *ns_uri) {
    if (ctxt == NULL)
	return(-1);
    if (prefix == NULL)
	return(-1);
    if (prefix[0] == 0)
	return(-1);

    if (ctxt->nsHash == NULL)
	ctxt->nsHash = xmlHashCreate(10);
    if (ctxt->nsHash == NULL)
	return(-1);
    if (ns_uri == NULL)
        return(xmlHashRemoveEntry(ctxt->nsHash, prefix,
	                          (xmlHashDeallocator)xmlFree));
    return(xmlHashUpdateEntry(ctxt->nsHash, prefix, (void *) xmlStrdup(ns_uri),
			      (xmlHashDeallocator)xmlFree));
}

const xmlChar *
xmlXPathNsLookup(xmlXPathContextPtr ctxt, const xmlChar *prefix) {
    if (ctxt == NULL)
	return(NULL);
    if (prefix == NULL)
	return(NULL);

#ifdef XML_XML_NAMESPACE
    if (xmlStrEqual(prefix, (const xmlChar *) "xml"))
	return(XML_XML_NAMESPACE);
#endif

    if (ctxt->namespaces != NULL) {
	int i;

	for (i = 0;i < ctxt->nsNr;i++) {
	    if ((ctxt->namespaces[i] != NULL) &&
		(xmlStrEqual(ctxt->namespaces[i]->prefix, prefix)))
		return(ctxt->namespaces[i]->href);
	}
    }

    return((const xmlChar *) xmlHashLookup(ctxt->nsHash, prefix));
}

void
xmlXPathRegisteredNsCleanup(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL)
	return;

    xmlHashFree(ctxt->nsHash, (xmlHashDeallocator)xmlFree);
    ctxt->nsHash = NULL;
}



xmlXPathObjectPtr
xmlXPathNewFloat(double val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating float object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_NUMBER;
    ret->floatval = val;
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_NUMBER);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathNewBoolean(int val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating boolean object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_BOOLEAN;
    ret->boolval = (val != 0);
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_BOOLEAN);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathNewString(const xmlChar *val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating string object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_STRING;
    if (val != NULL)
	ret->stringval = xmlStrdup(val);
    else
	ret->stringval = xmlStrdup((const xmlChar *)"");
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_STRING);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathWrapString (xmlChar *val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating string object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_STRING;
    ret->stringval = val;
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_STRING);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathNewCString(const char *val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating string object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_STRING;
    ret->stringval = xmlStrdup(BAD_CAST val);
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_STRING);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathWrapCString (char * val) {
    return(xmlXPathWrapString((xmlChar *)(val)));
}

xmlXPathObjectPtr
xmlXPathWrapExternal (void *val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating user object\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_USERS;
    ret->user = val;
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, XPATH_USERS);
#endif
    return(ret);
}

xmlXPathObjectPtr
xmlXPathObjectCopy(xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "copying object\n");
	return(NULL);
    }
    memcpy(ret, val , (size_t) sizeof(xmlXPathObject));
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageRequested(NULL, val->type);
#endif
    switch (val->type) {
	case XPATH_BOOLEAN:
	case XPATH_NUMBER:
	case XPATH_POINT:
	case XPATH_RANGE:
	    break;
	case XPATH_STRING:
	    ret->stringval = xmlStrdup(val->stringval);
	    break;
	case XPATH_XSLT_TREE:
#if 0
	    if ((val->nodesetval != NULL) &&
		(val->nodesetval->nodeTab != NULL)) {
		xmlNodePtr cur, tmp;
		xmlDocPtr top;

		ret->boolval = 1;
		top =  xmlNewDoc(NULL);
		top->name = (char *)
		    xmlStrdup(val->nodesetval->nodeTab[0]->name);
		ret->user = top;
		if (top != NULL) {
		    top->doc = top;
		    cur = val->nodesetval->nodeTab[0]->children;
		    while (cur != NULL) {
			tmp = xmlDocCopyNode(cur, top, 1);
			xmlAddChild((xmlNodePtr) top, tmp);
			cur = cur->next;
		    }
		}

		ret->nodesetval = xmlXPathNodeSetCreate((xmlNodePtr) top);
	    } else
		ret->nodesetval = xmlXPathNodeSetCreate(NULL);
	    
	    break;
#endif
	case XPATH_NODESET:
	    ret->nodesetval = xmlXPathNodeSetMerge(NULL, val->nodesetval);
	    
	    ret->boolval = 0;
	    break;
	case XPATH_LOCATIONSET:
#ifdef LIBXML_XPTR_ENABLED
	{
	    xmlLocationSetPtr loc = val->user;
	    ret->user = (void *) xmlXPtrLocationSetMerge(NULL, loc);
	    break;
	}
#endif
        case XPATH_USERS:
	    ret->user = val->user;
	    break;
        case XPATH_UNDEFINED:
	    xmlGenericError(xmlGenericErrorContext,
		    "xmlXPathObjectCopy: unsupported type %d\n",
		    val->type);
	    break;
    }
    return(ret);
}

void
xmlXPathFreeObject(xmlXPathObjectPtr obj) {
    if (obj == NULL) return;
    if ((obj->type == XPATH_NODESET) || (obj->type == XPATH_XSLT_TREE)) {
	if (obj->boolval) {
#if 0
	    if (obj->user != NULL) {
                xmlXPathFreeNodeSet(obj->nodesetval);
		xmlFreeNodeList((xmlNodePtr) obj->user);
	    } else
#endif
	    obj->type = XPATH_XSLT_TREE; 
	    if (obj->nodesetval != NULL)
		xmlXPathFreeValueTree(obj->nodesetval);
	} else {
	    if (obj->nodesetval != NULL)
		xmlXPathFreeNodeSet(obj->nodesetval);
	}
#ifdef LIBXML_XPTR_ENABLED
    } else if (obj->type == XPATH_LOCATIONSET) {
	if (obj->user != NULL)
	    xmlXPtrFreeLocationSet(obj->user);
#endif
    } else if (obj->type == XPATH_STRING) {
	if (obj->stringval != NULL)
	    xmlFree(obj->stringval);
    }
#ifdef XP_DEBUG_OBJ_USAGE
    xmlXPathDebugObjUsageReleased(NULL, obj->type);
#endif
    xmlFree(obj);
}

static void
xmlXPathReleaseObject(xmlXPathContextPtr ctxt, xmlXPathObjectPtr obj)
{
#define XP_CACHE_ADD(sl, o) if (sl == NULL) { \
	sl = xmlPointerListCreate(10); if (sl == NULL) goto free_obj; } \
    if (xmlPointerListAddSize(sl, obj, 0) == -1) goto free_obj;

#define XP_CACHE_WANTS(sl, n) ((sl == NULL) || ((sl)->number < n))

    if (obj == NULL)
	return;
    if ((ctxt == NULL) || (ctxt->cache == NULL)) {
	 xmlXPathFreeObject(obj);
    } else {
	xmlXPathContextCachePtr cache =
	    (xmlXPathContextCachePtr) ctxt->cache;

	switch (obj->type) {
	    case XPATH_NODESET:
	    case XPATH_XSLT_TREE:
		if (obj->nodesetval != NULL) {
		    if (obj->boolval) {
			obj->type = XPATH_XSLT_TREE; 
			xmlXPathFreeValueTree(obj->nodesetval);
			obj->nodesetval = NULL;
		    } else if ((obj->nodesetval->nodeMax <= 40) &&
			(XP_CACHE_WANTS(cache->nodesetObjs,
					cache->maxNodeset)))
		    {
			XP_CACHE_ADD(cache->nodesetObjs, obj);
			goto obj_cached;
		    } else {
			xmlXPathFreeNodeSet(obj->nodesetval);
			obj->nodesetval = NULL;
		    }
		}
		break;
	    case XPATH_STRING:
		if (obj->stringval != NULL)
		    xmlFree(obj->stringval);

		if (XP_CACHE_WANTS(cache->stringObjs, cache->maxString)) {
		    XP_CACHE_ADD(cache->stringObjs, obj);
		    goto obj_cached;
		}
		break;
	    case XPATH_BOOLEAN:
		if (XP_CACHE_WANTS(cache->booleanObjs, cache->maxBoolean)) {
		    XP_CACHE_ADD(cache->booleanObjs, obj);
		    goto obj_cached;
		}
		break;
	    case XPATH_NUMBER:
		if (XP_CACHE_WANTS(cache->numberObjs, cache->maxNumber)) {
		    XP_CACHE_ADD(cache->numberObjs, obj);
		    goto obj_cached;
		}
		break;
#ifdef LIBXML_XPTR_ENABLED
	    case XPATH_LOCATIONSET:
		if (obj->user != NULL) {
		    xmlXPtrFreeLocationSet(obj->user);
		}
		goto free_obj;
#endif
	    default:
		goto free_obj;
	}

	if (XP_CACHE_WANTS(cache->miscObjs, cache->maxMisc)) {
	    XP_CACHE_ADD(cache->miscObjs, obj);
	} else
	    goto free_obj;

obj_cached:

#ifdef XP_DEBUG_OBJ_USAGE
	xmlXPathDebugObjUsageReleased(ctxt, obj->type);
#endif

	if (obj->nodesetval != NULL) {
	    xmlNodeSetPtr tmpset = obj->nodesetval;

	    if (tmpset->nodeNr > 1) {
		int i;
		xmlNodePtr node;

		for (i = 0; i < tmpset->nodeNr; i++) {
		    node = tmpset->nodeTab[i];
		    if ((node != NULL) &&
			(node->type == XML_NAMESPACE_DECL))
		    {
			xmlXPathNodeSetFreeNs((xmlNsPtr) node);
		    }
		}
	    } else if (tmpset->nodeNr == 1) {
		if ((tmpset->nodeTab[0] != NULL) &&
		    (tmpset->nodeTab[0]->type == XML_NAMESPACE_DECL))
		    xmlXPathNodeSetFreeNs((xmlNsPtr) tmpset->nodeTab[0]);
	    }
	    tmpset->nodeNr = 0;
	    memset(obj, 0, sizeof(xmlXPathObject));
	    obj->nodesetval = tmpset;
	} else
	    memset(obj, 0, sizeof(xmlXPathObject));

	return;

free_obj:
	if (obj->nodesetval != NULL)
	    xmlXPathFreeNodeSet(obj->nodesetval);
#ifdef XP_DEBUG_OBJ_USAGE
	xmlXPathDebugObjUsageReleased(NULL, obj->type);
#endif
	xmlFree(obj);
    }
    return;
}



xmlChar *
xmlXPathCastBooleanToString (int val) {
    xmlChar *ret;
    if (val)
	ret = xmlStrdup((const xmlChar *) "true");
    else
	ret = xmlStrdup((const xmlChar *) "false");
    return(ret);
}

xmlChar *
xmlXPathCastNumberToString (double val) {
    xmlChar *ret;
    switch (xmlXPathIsInf(val)) {
    case 1:
	ret = xmlStrdup((const xmlChar *) "Infinity");
	break;
    case -1:
	ret = xmlStrdup((const xmlChar *) "-Infinity");
	break;
    default:
	if (xmlXPathIsNaN(val)) {
	    ret = xmlStrdup((const xmlChar *) "NaN");
	} else if (val == 0 && xmlXPathGetSign(val) != 0) {
	    ret = xmlStrdup((const xmlChar *) "0");
	} else {
	    
	    char buf[100];
	    xmlXPathFormatNumber(val, buf, 99);
	    buf[99] = 0;
	    ret = xmlStrdup((const xmlChar *) buf);
	}
    }
    return(ret);
}

xmlChar *
xmlXPathCastNodeToString (xmlNodePtr node) {
xmlChar *ret;
    if ((ret = xmlNodeGetContent(node)) == NULL)
	ret = xmlStrdup((const xmlChar *) "");
    return(ret);
}

xmlChar *
xmlXPathCastNodeSetToString (xmlNodeSetPtr ns) {
    if ((ns == NULL) || (ns->nodeNr == 0) || (ns->nodeTab == NULL))
	return(xmlStrdup((const xmlChar *) ""));

    if (ns->nodeNr > 1)
	xmlXPathNodeSetSort(ns);
    return(xmlXPathCastNodeToString(ns->nodeTab[0]));
}

xmlChar *
xmlXPathCastToString(xmlXPathObjectPtr val) {
    xmlChar *ret = NULL;

    if (val == NULL)
	return(xmlStrdup((const xmlChar *) ""));
    switch (val->type) {
	case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
	    xmlGenericError(xmlGenericErrorContext, "String: undefined\n");
#endif
	    ret = xmlStrdup((const xmlChar *) "");
	    break;
        case XPATH_NODESET:
        case XPATH_XSLT_TREE:
	    ret = xmlXPathCastNodeSetToString(val->nodesetval);
	    break;
	case XPATH_STRING:
	    return(xmlStrdup(val->stringval));
        case XPATH_BOOLEAN:
	    ret = xmlXPathCastBooleanToString(val->boolval);
	    break;
	case XPATH_NUMBER: {
	    ret = xmlXPathCastNumberToString(val->floatval);
	    break;
	}
	case XPATH_USERS:
	case XPATH_POINT:
	case XPATH_RANGE:
	case XPATH_LOCATIONSET:
	    TODO
	    ret = xmlStrdup((const xmlChar *) "");
	    break;
    }
    return(ret);
}

xmlXPathObjectPtr
xmlXPathConvertString(xmlXPathObjectPtr val) {
    xmlChar *res = NULL;

    if (val == NULL)
	return(xmlXPathNewCString(""));

    switch (val->type) {
    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext, "STRING: undefined\n");
#endif
	break;
    case XPATH_NODESET:
    case XPATH_XSLT_TREE:
	res = xmlXPathCastNodeSetToString(val->nodesetval);
	break;
    case XPATH_STRING:
	return(val);
    case XPATH_BOOLEAN:
	res = xmlXPathCastBooleanToString(val->boolval);
	break;
    case XPATH_NUMBER:
	res = xmlXPathCastNumberToString(val->floatval);
	break;
    case XPATH_USERS:
    case XPATH_POINT:
    case XPATH_RANGE:
    case XPATH_LOCATIONSET:
	TODO;
	break;
    }
    xmlXPathFreeObject(val);
    if (res == NULL)
	return(xmlXPathNewCString(""));
    return(xmlXPathWrapString(res));
}

double
xmlXPathCastBooleanToNumber(int val) {
    if (val)
	return(1.0);
    return(0.0);
}

double
xmlXPathCastStringToNumber(const xmlChar * val) {
    return(xmlXPathStringEvalNumber(val));
}

double
xmlXPathCastNodeToNumber (xmlNodePtr node) {
    xmlChar *strval;
    double ret;

    if (node == NULL)
	return(xmlXPathNAN);
    strval = xmlXPathCastNodeToString(node);
    if (strval == NULL)
	return(xmlXPathNAN);
    ret = xmlXPathCastStringToNumber(strval);
    xmlFree(strval);

    return(ret);
}

double
xmlXPathCastNodeSetToNumber (xmlNodeSetPtr ns) {
    xmlChar *str;
    double ret;

    if (ns == NULL)
	return(xmlXPathNAN);
    str = xmlXPathCastNodeSetToString(ns);
    ret = xmlXPathCastStringToNumber(str);
    xmlFree(str);
    return(ret);
}

double
xmlXPathCastToNumber(xmlXPathObjectPtr val) {
    double ret = 0.0;

    if (val == NULL)
	return(xmlXPathNAN);
    switch (val->type) {
    case XPATH_UNDEFINED:
#ifdef DEGUB_EXPR
	xmlGenericError(xmlGenericErrorContext, "NUMBER: undefined\n");
#endif
	ret = xmlXPathNAN;
	break;
    case XPATH_NODESET:
    case XPATH_XSLT_TREE:
	ret = xmlXPathCastNodeSetToNumber(val->nodesetval);
	break;
    case XPATH_STRING:
	ret = xmlXPathCastStringToNumber(val->stringval);
	break;
    case XPATH_NUMBER:
	ret = val->floatval;
	break;
    case XPATH_BOOLEAN:
	ret = xmlXPathCastBooleanToNumber(val->boolval);
	break;
    case XPATH_USERS:
    case XPATH_POINT:
    case XPATH_RANGE:
    case XPATH_LOCATIONSET:
	TODO;
	ret = xmlXPathNAN;
	break;
    }
    return(ret);
}

xmlXPathObjectPtr
xmlXPathConvertNumber(xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(xmlXPathNewFloat(0.0));
    if (val->type == XPATH_NUMBER)
	return(val);
    ret = xmlXPathNewFloat(xmlXPathCastToNumber(val));
    xmlXPathFreeObject(val);
    return(ret);
}

int
xmlXPathCastNumberToBoolean (double val) {
     if (xmlXPathIsNaN(val) || (val == 0.0))
	 return(0);
     return(1);
}

int
xmlXPathCastStringToBoolean (const xmlChar *val) {
    if ((val == NULL) || (xmlStrlen(val) == 0))
	return(0);
    return(1);
}

int
xmlXPathCastNodeSetToBoolean (xmlNodeSetPtr ns) {
    if ((ns == NULL) || (ns->nodeNr == 0))
	return(0);
    return(1);
}

int
xmlXPathCastToBoolean (xmlXPathObjectPtr val) {
    int ret = 0;

    if (val == NULL)
	return(0);
    switch (val->type) {
    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext, "BOOLEAN: undefined\n");
#endif
	ret = 0;
	break;
    case XPATH_NODESET:
    case XPATH_XSLT_TREE:
	ret = xmlXPathCastNodeSetToBoolean(val->nodesetval);
	break;
    case XPATH_STRING:
	ret = xmlXPathCastStringToBoolean(val->stringval);
	break;
    case XPATH_NUMBER:
	ret = xmlXPathCastNumberToBoolean(val->floatval);
	break;
    case XPATH_BOOLEAN:
	ret = val->boolval;
	break;
    case XPATH_USERS:
    case XPATH_POINT:
    case XPATH_RANGE:
    case XPATH_LOCATIONSET:
	TODO;
	ret = 0;
	break;
    }
    return(ret);
}


xmlXPathObjectPtr
xmlXPathConvertBoolean(xmlXPathObjectPtr val) {
    xmlXPathObjectPtr ret;

    if (val == NULL)
	return(xmlXPathNewBoolean(0));
    if (val->type == XPATH_BOOLEAN)
	return(val);
    ret = xmlXPathNewBoolean(xmlXPathCastToBoolean(val));
    xmlXPathFreeObject(val);
    return(ret);
}


xmlXPathContextPtr
xmlXPathNewContext(xmlDocPtr doc) {
    xmlXPathContextPtr ret;

    ret = (xmlXPathContextPtr) xmlMalloc(sizeof(xmlXPathContext));
    if (ret == NULL) {
        xmlXPathErrMemory(NULL, "creating context\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathContext));
    ret->doc = doc;
    ret->node = NULL;

    ret->varHash = NULL;

    ret->nb_types = 0;
    ret->max_types = 0;
    ret->types = NULL;

    ret->funcHash = xmlHashCreate(0);

    ret->nb_axis = 0;
    ret->max_axis = 0;
    ret->axis = NULL;

    ret->nsHash = NULL;
    ret->user = NULL;

    ret->contextSize = -1;
    ret->proximityPosition = -1;

#ifdef XP_DEFAULT_CACHE_ON
    if (xmlXPathContextSetCache(ret, 1, -1, 0) == -1) {
	xmlXPathFreeContext(ret);
	return(NULL);
    }
#endif

    xmlXPathRegisterAllFunctions(ret);

    return(ret);
}

void
xmlXPathFreeContext(xmlXPathContextPtr ctxt) {
    if (ctxt == NULL) return;

    if (ctxt->cache != NULL)
	xmlXPathFreeCache((xmlXPathContextCachePtr) ctxt->cache);
    xmlXPathRegisteredNsCleanup(ctxt);
    xmlXPathRegisteredFuncsCleanup(ctxt);
    xmlXPathRegisteredVariablesCleanup(ctxt);
    xmlResetError(&ctxt->lastError);
    xmlFree(ctxt);
}


#define CHECK_CTXT(ctxt)						\
    if (ctxt == NULL) {						\
	__xmlRaiseError(NULL, NULL, NULL,				\
		NULL, NULL, XML_FROM_XPATH,				\
		XML_ERR_INTERNAL_ERROR, XML_ERR_FATAL,			\
		__FILE__, __LINE__,					\
		NULL, NULL, NULL, 0, 0,					\
		"NULL context pointer\n");				\
	return(NULL);							\
    }									\

#define CHECK_CTXT_NEG(ctxt)						\
    if (ctxt == NULL) {						\
	__xmlRaiseError(NULL, NULL, NULL,				\
		NULL, NULL, XML_FROM_XPATH,				\
		XML_ERR_INTERNAL_ERROR, XML_ERR_FATAL,			\
		__FILE__, __LINE__,					\
		NULL, NULL, NULL, 0, 0,					\
		"NULL context pointer\n");				\
	return(-1);							\
    }									\


#define CHECK_CONTEXT(ctxt)						\
    if ((ctxt == NULL) || (ctxt->doc == NULL) ||			\
        (ctxt->doc->children == NULL)) {				\
	xmlXPatherror(ctxt, __FILE__, __LINE__, XPATH_INVALID_CTXT);	\
	return(NULL);							\
    }


xmlXPathParserContextPtr
xmlXPathNewParserContext(const xmlChar *str, xmlXPathContextPtr ctxt) {
    xmlXPathParserContextPtr ret;

    ret = (xmlXPathParserContextPtr) xmlMalloc(sizeof(xmlXPathParserContext));
    if (ret == NULL) {
        xmlXPathErrMemory(ctxt, "creating parser context\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathParserContext));
    ret->cur = ret->base = str;
    ret->context = ctxt;

    ret->comp = xmlXPathNewCompExpr();
    if (ret->comp == NULL) {
	xmlFree(ret->valueTab);
	xmlFree(ret);
	return(NULL);
    }
    if ((ctxt != NULL) && (ctxt->dict != NULL)) {
        ret->comp->dict = ctxt->dict;
	xmlDictReference(ret->comp->dict);
    }

    return(ret);
}

static xmlXPathParserContextPtr
xmlXPathCompParserContext(xmlXPathCompExprPtr comp, xmlXPathContextPtr ctxt) {
    xmlXPathParserContextPtr ret;

    ret = (xmlXPathParserContextPtr) xmlMalloc(sizeof(xmlXPathParserContext));
    if (ret == NULL) {
        xmlXPathErrMemory(ctxt, "creating evaluation context\n");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathParserContext));

    
    ret->valueTab = (xmlXPathObjectPtr *)
                     xmlMalloc(10 * sizeof(xmlXPathObjectPtr));
    if (ret->valueTab == NULL) {
	xmlFree(ret);
	xmlXPathErrMemory(ctxt, "creating evaluation context\n");
	return(NULL);
    }
    ret->valueNr = 0;
    ret->valueMax = 10;
    ret->value = NULL;

    ret->context = ctxt;
    ret->comp = comp;

    return(ret);
}

void
xmlXPathFreeParserContext(xmlXPathParserContextPtr ctxt) {
    if (ctxt->valueTab != NULL) {
        xmlFree(ctxt->valueTab);
    }
    if (ctxt->comp != NULL) {
#ifdef XPATH_STREAMING
	if (ctxt->comp->stream != NULL) {
	    xmlFreePatternList(ctxt->comp->stream);
	    ctxt->comp->stream = NULL;
	}
#endif
	xmlXPathFreeCompExpr(ctxt->comp);
    }
    xmlFree(ctxt);
}


static unsigned int
xmlXPathNodeValHash(xmlNodePtr node) {
    int len = 2;
    const xmlChar * string = NULL;
    xmlNodePtr tmp = NULL;
    unsigned int ret = 0;

    if (node == NULL)
	return(0);

    if (node->type == XML_DOCUMENT_NODE) {
	tmp = xmlDocGetRootElement((xmlDocPtr) node);
	if (tmp == NULL)
	    node = node->children;
	else
	    node = tmp;

	if (node == NULL)
	    return(0);
    }

    switch (node->type) {
	case XML_COMMENT_NODE:
	case XML_PI_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_TEXT_NODE:
	    string = node->content;
	    if (string == NULL)
		return(0);
	    if (string[0] == 0)
		return(0);
	    return(((unsigned int) string[0]) +
		   (((unsigned int) string[1]) << 8));
	case XML_NAMESPACE_DECL:
	    string = ((xmlNsPtr)node)->href;
	    if (string == NULL)
		return(0);
	    if (string[0] == 0)
		return(0);
	    return(((unsigned int) string[0]) +
		   (((unsigned int) string[1]) << 8));
	case XML_ATTRIBUTE_NODE:
	    tmp = ((xmlAttrPtr) node)->children;
	    break;
	case XML_ELEMENT_NODE:
	    tmp = node->children;
	    break;
	default:
	    return(0);
    }
    while (tmp != NULL) {
	switch (tmp->type) {
	    case XML_COMMENT_NODE:
	    case XML_PI_NODE:
	    case XML_CDATA_SECTION_NODE:
	    case XML_TEXT_NODE:
		string = tmp->content;
		break;
	    case XML_NAMESPACE_DECL:
		string = ((xmlNsPtr)tmp)->href;
		break;
	    default:
		break;
	}
	if ((string != NULL) && (string[0] != 0)) {
	    if (len == 1) {
		return(ret + (((unsigned int) string[0]) << 8));
	    }
	    if (string[1] == 0) {
		len = 1;
		ret = (unsigned int) string[0];
	    } else {
		return(((unsigned int) string[0]) +
		       (((unsigned int) string[1]) << 8));
	    }
	}
	if ((tmp->children != NULL) && (tmp->type != XML_DTD_NODE)) {
	    if (tmp->children->type != XML_ENTITY_DECL) {
		tmp = tmp->children;
		continue;
	    }
	}
	if (tmp == node)
	    break;

	if (tmp->next != NULL) {
	    tmp = tmp->next;
	    continue;
	}

	do {
	    tmp = tmp->parent;
	    if (tmp == NULL)
		break;
	    if (tmp == node) {
		tmp = NULL;
		break;
	    }
	    if (tmp->next != NULL) {
		tmp = tmp->next;
		break;
	    }
	} while (tmp != NULL);
    }
    return(ret);
}

static unsigned int
xmlXPathStringHash(const xmlChar * string) {
    if (string == NULL)
	return((unsigned int) 0);
    if (string[0] == 0)
	return(0);
    return(((unsigned int) string[0]) +
	   (((unsigned int) string[1]) << 8));
}

static int
xmlXPathCompareNodeSetFloat(xmlXPathParserContextPtr ctxt, int inf, int strict,
	                    xmlXPathObjectPtr arg, xmlXPathObjectPtr f) {
    int i, ret = 0;
    xmlNodeSetPtr ns;
    xmlChar *str2;

    if ((f == NULL) || (arg == NULL) ||
	((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE))) {
	xmlXPathReleaseObject(ctxt->context, arg);
	xmlXPathReleaseObject(ctxt->context, f);
        return(0);
    }
    ns = arg->nodesetval;
    if (ns != NULL) {
	for (i = 0;i < ns->nodeNr;i++) {
	     str2 = xmlXPathCastNodeToString(ns->nodeTab[i]);
	     if (str2 != NULL) {
		 valuePush(ctxt,
			   xmlXPathCacheNewString(ctxt->context, str2));
		 xmlFree(str2);
		 xmlXPathNumberFunction(ctxt, 1);
		 valuePush(ctxt, xmlXPathCacheObjectCopy(ctxt->context, f));
		 ret = xmlXPathCompareValues(ctxt, inf, strict);
		 if (ret)
		     break;
	     }
	}
    }
    xmlXPathReleaseObject(ctxt->context, arg);
    xmlXPathReleaseObject(ctxt->context, f);
    return(ret);
}

static int
xmlXPathCompareNodeSetString(xmlXPathParserContextPtr ctxt, int inf, int strict,
	                    xmlXPathObjectPtr arg, xmlXPathObjectPtr s) {
    int i, ret = 0;
    xmlNodeSetPtr ns;
    xmlChar *str2;

    if ((s == NULL) || (arg == NULL) ||
	((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE))) {
	xmlXPathReleaseObject(ctxt->context, arg);
	xmlXPathReleaseObject(ctxt->context, s);
        return(0);
    }
    ns = arg->nodesetval;
    if (ns != NULL) {
	for (i = 0;i < ns->nodeNr;i++) {
	     str2 = xmlXPathCastNodeToString(ns->nodeTab[i]);
	     if (str2 != NULL) {
		 valuePush(ctxt,
			   xmlXPathCacheNewString(ctxt->context, str2));
		 xmlFree(str2);
		 valuePush(ctxt, xmlXPathCacheObjectCopy(ctxt->context, s));
		 ret = xmlXPathCompareValues(ctxt, inf, strict);
		 if (ret)
		     break;
	     }
	}
    }
    xmlXPathReleaseObject(ctxt->context, arg);
    xmlXPathReleaseObject(ctxt->context, s);
    return(ret);
}

static int
xmlXPathCompareNodeSets(int inf, int strict,
	                xmlXPathObjectPtr arg1, xmlXPathObjectPtr arg2) {
    int i, j, init = 0;
    double val1;
    double *values2;
    int ret = 0;
    xmlNodeSetPtr ns1;
    xmlNodeSetPtr ns2;

    if ((arg1 == NULL) ||
	((arg1->type != XPATH_NODESET) && (arg1->type != XPATH_XSLT_TREE))) {
	xmlXPathFreeObject(arg2);
        return(0);
    }
    if ((arg2 == NULL) ||
	((arg2->type != XPATH_NODESET) && (arg2->type != XPATH_XSLT_TREE))) {
	xmlXPathFreeObject(arg1);
	xmlXPathFreeObject(arg2);
        return(0);
    }

    ns1 = arg1->nodesetval;
    ns2 = arg2->nodesetval;

    if ((ns1 == NULL) || (ns1->nodeNr <= 0)) {
	xmlXPathFreeObject(arg1);
	xmlXPathFreeObject(arg2);
	return(0);
    }
    if ((ns2 == NULL) || (ns2->nodeNr <= 0)) {
	xmlXPathFreeObject(arg1);
	xmlXPathFreeObject(arg2);
	return(0);
    }

    values2 = (double *) xmlMalloc(ns2->nodeNr * sizeof(double));
    if (values2 == NULL) {
        xmlXPathErrMemory(NULL, "comparing nodesets\n");
	xmlXPathFreeObject(arg1);
	xmlXPathFreeObject(arg2);
	return(0);
    }
    for (i = 0;i < ns1->nodeNr;i++) {
	val1 = xmlXPathCastNodeToNumber(ns1->nodeTab[i]);
	if (xmlXPathIsNaN(val1))
	    continue;
	for (j = 0;j < ns2->nodeNr;j++) {
	    if (init == 0) {
		values2[j] = xmlXPathCastNodeToNumber(ns2->nodeTab[j]);
	    }
	    if (xmlXPathIsNaN(values2[j]))
		continue;
	    if (inf && strict)
		ret = (val1 < values2[j]);
	    else if (inf && !strict)
		ret = (val1 <= values2[j]);
	    else if (!inf && strict)
		ret = (val1 > values2[j]);
	    else if (!inf && !strict)
		ret = (val1 >= values2[j]);
	    if (ret)
		break;
	}
	if (ret)
	    break;
	init = 1;
    }
    xmlFree(values2);
    xmlXPathFreeObject(arg1);
    xmlXPathFreeObject(arg2);
    return(ret);
}

static int
xmlXPathCompareNodeSetValue(xmlXPathParserContextPtr ctxt, int inf, int strict,
	                    xmlXPathObjectPtr arg, xmlXPathObjectPtr val) {
    if ((val == NULL) || (arg == NULL) ||
	((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE)))
        return(0);

    switch(val->type) {
        case XPATH_NUMBER:
	    return(xmlXPathCompareNodeSetFloat(ctxt, inf, strict, arg, val));
        case XPATH_NODESET:
        case XPATH_XSLT_TREE:
	    return(xmlXPathCompareNodeSets(inf, strict, arg, val));
        case XPATH_STRING:
	    return(xmlXPathCompareNodeSetString(ctxt, inf, strict, arg, val));
        case XPATH_BOOLEAN:
	    valuePush(ctxt, arg);
	    xmlXPathBooleanFunction(ctxt, 1);
	    valuePush(ctxt, val);
	    return(xmlXPathCompareValues(ctxt, inf, strict));
	default:
	    TODO
    }
    return(0);
}

static int
xmlXPathEqualNodeSetString(xmlXPathObjectPtr arg, const xmlChar * str, int neq)
{
    int i;
    xmlNodeSetPtr ns;
    xmlChar *str2;
    unsigned int hash;

    if ((str == NULL) || (arg == NULL) ||
        ((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE)))
        return (0);
    ns = arg->nodesetval;
    if ((ns == NULL) || (ns->nodeNr <= 0) )
        return (0);
    hash = xmlXPathStringHash(str);
    for (i = 0; i < ns->nodeNr; i++) {
        if (xmlXPathNodeValHash(ns->nodeTab[i]) == hash) {
            str2 = xmlNodeGetContent(ns->nodeTab[i]);
            if ((str2 != NULL) && (xmlStrEqual(str, str2))) {
                xmlFree(str2);
		if (neq)
		    continue;
                return (1);
	    } else if ((str2 == NULL) && (xmlStrEqual(str, BAD_CAST ""))) {
		if (neq)
		    continue;
                return (1);
            } else if (neq) {
		if (str2 != NULL)
		    xmlFree(str2);
		return (1);
	    }
            if (str2 != NULL)
                xmlFree(str2);
        } else if (neq)
	    return (1);
    }
    return (0);
}

static int
xmlXPathEqualNodeSetFloat(xmlXPathParserContextPtr ctxt,
    xmlXPathObjectPtr arg, double f, int neq) {
  int i, ret=0;
  xmlNodeSetPtr ns;
  xmlChar *str2;
  xmlXPathObjectPtr val;
  double v;

    if ((arg == NULL) ||
	((arg->type != XPATH_NODESET) && (arg->type != XPATH_XSLT_TREE)))
        return(0);

    ns = arg->nodesetval;
    if (ns != NULL) {
	for (i=0;i<ns->nodeNr;i++) {
	    str2 = xmlXPathCastNodeToString(ns->nodeTab[i]);
	    if (str2 != NULL) {
		valuePush(ctxt, xmlXPathCacheNewString(ctxt->context, str2));
		xmlFree(str2);
		xmlXPathNumberFunction(ctxt, 1);
		val = valuePop(ctxt);
		v = val->floatval;
		xmlXPathReleaseObject(ctxt->context, val);
		if (!xmlXPathIsNaN(v)) {
		    if ((!neq) && (v==f)) {
			ret = 1;
			break;
		    } else if ((neq) && (v!=f)) {
			ret = 1;
			break;
		    }
		} else {	
		    if (neq)
			ret = 1;
		}
	    }
	}
    }

    return(ret);
}


static int
xmlXPathEqualNodeSets(xmlXPathObjectPtr arg1, xmlXPathObjectPtr arg2, int neq) {
    int i, j;
    unsigned int *hashs1;
    unsigned int *hashs2;
    xmlChar **values1;
    xmlChar **values2;
    int ret = 0;
    xmlNodeSetPtr ns1;
    xmlNodeSetPtr ns2;

    if ((arg1 == NULL) ||
	((arg1->type != XPATH_NODESET) && (arg1->type != XPATH_XSLT_TREE)))
        return(0);
    if ((arg2 == NULL) ||
	((arg2->type != XPATH_NODESET) && (arg2->type != XPATH_XSLT_TREE)))
        return(0);

    ns1 = arg1->nodesetval;
    ns2 = arg2->nodesetval;

    if ((ns1 == NULL) || (ns1->nodeNr <= 0))
	return(0);
    if ((ns2 == NULL) || (ns2->nodeNr <= 0))
	return(0);

    if (neq == 0)
	for (i = 0;i < ns1->nodeNr;i++)
	    for (j = 0;j < ns2->nodeNr;j++)
		if (ns1->nodeTab[i] == ns2->nodeTab[j])
		    return(1);

    values1 = (xmlChar **) xmlMalloc(ns1->nodeNr * sizeof(xmlChar *));
    if (values1 == NULL) {
        xmlXPathErrMemory(NULL, "comparing nodesets\n");
	return(0);
    }
    hashs1 = (unsigned int *) xmlMalloc(ns1->nodeNr * sizeof(unsigned int));
    if (hashs1 == NULL) {
        xmlXPathErrMemory(NULL, "comparing nodesets\n");
	xmlFree(values1);
	return(0);
    }
    memset(values1, 0, ns1->nodeNr * sizeof(xmlChar *));
    values2 = (xmlChar **) xmlMalloc(ns2->nodeNr * sizeof(xmlChar *));
    if (values2 == NULL) {
        xmlXPathErrMemory(NULL, "comparing nodesets\n");
	xmlFree(hashs1);
	xmlFree(values1);
	return(0);
    }
    hashs2 = (unsigned int *) xmlMalloc(ns2->nodeNr * sizeof(unsigned int));
    if (hashs2 == NULL) {
        xmlXPathErrMemory(NULL, "comparing nodesets\n");
	xmlFree(hashs1);
	xmlFree(values1);
	xmlFree(values2);
	return(0);
    }
    memset(values2, 0, ns2->nodeNr * sizeof(xmlChar *));
    for (i = 0;i < ns1->nodeNr;i++) {
	hashs1[i] = xmlXPathNodeValHash(ns1->nodeTab[i]);
	for (j = 0;j < ns2->nodeNr;j++) {
	    if (i == 0)
		hashs2[j] = xmlXPathNodeValHash(ns2->nodeTab[j]);
	    if (hashs1[i] != hashs2[j]) {
		if (neq) {
		    ret = 1;
		    break;
		}
	    }
	    else {
		if (values1[i] == NULL)
		    values1[i] = xmlNodeGetContent(ns1->nodeTab[i]);
		if (values2[j] == NULL)
		    values2[j] = xmlNodeGetContent(ns2->nodeTab[j]);
		ret = xmlStrEqual(values1[i], values2[j]) ^ neq;
		if (ret)
		    break;
	    }
	}
	if (ret)
	    break;
    }
    for (i = 0;i < ns1->nodeNr;i++)
	if (values1[i] != NULL)
	    xmlFree(values1[i]);
    for (j = 0;j < ns2->nodeNr;j++)
	if (values2[j] != NULL)
	    xmlFree(values2[j]);
    xmlFree(values1);
    xmlFree(values2);
    xmlFree(hashs1);
    xmlFree(hashs2);
    return(ret);
}

static int
xmlXPathEqualValuesCommon(xmlXPathParserContextPtr ctxt,
  xmlXPathObjectPtr arg1, xmlXPathObjectPtr arg2) {
    int ret = 0;
    switch (arg1->type) {
        case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
	    xmlGenericError(xmlGenericErrorContext,
		    "Equal: undefined\n");
#endif
	    break;
        case XPATH_BOOLEAN:
	    switch (arg2->type) {
	        case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
		    xmlGenericError(xmlGenericErrorContext,
			    "Equal: undefined\n");
#endif
		    break;
		case XPATH_BOOLEAN:
#ifdef DEBUG_EXPR
		    xmlGenericError(xmlGenericErrorContext,
			    "Equal: %d boolean %d \n",
			    arg1->boolval, arg2->boolval);
#endif
		    ret = (arg1->boolval == arg2->boolval);
		    break;
		case XPATH_NUMBER:
		    ret = (arg1->boolval ==
			   xmlXPathCastNumberToBoolean(arg2->floatval));
		    break;
		case XPATH_STRING:
		    if ((arg2->stringval == NULL) ||
			(arg2->stringval[0] == 0)) ret = 0;
		    else
			ret = 1;
		    ret = (arg1->boolval == ret);
		    break;
		case XPATH_USERS:
		case XPATH_POINT:
		case XPATH_RANGE:
		case XPATH_LOCATIONSET:
		    TODO
		    break;
		case XPATH_NODESET:
		case XPATH_XSLT_TREE:
		    break;
	    }
	    break;
        case XPATH_NUMBER:
	    switch (arg2->type) {
	        case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
		    xmlGenericError(xmlGenericErrorContext,
			    "Equal: undefined\n");
#endif
		    break;
		case XPATH_BOOLEAN:
		    ret = (arg2->boolval==
			   xmlXPathCastNumberToBoolean(arg1->floatval));
		    break;
		case XPATH_STRING:
		    valuePush(ctxt, arg2);
		    xmlXPathNumberFunction(ctxt, 1);
		    arg2 = valuePop(ctxt);
		    
		case XPATH_NUMBER:
		    
		    if (xmlXPathIsNaN(arg1->floatval) ||
			    xmlXPathIsNaN(arg2->floatval)) {
		        ret = 0;
		    } else if (xmlXPathIsInf(arg1->floatval) == 1) {
		        if (xmlXPathIsInf(arg2->floatval) == 1)
			    ret = 1;
			else
			    ret = 0;
		    } else if (xmlXPathIsInf(arg1->floatval) == -1) {
			if (xmlXPathIsInf(arg2->floatval) == -1)
			    ret = 1;
			else
			    ret = 0;
		    } else if (xmlXPathIsInf(arg2->floatval) == 1) {
			if (xmlXPathIsInf(arg1->floatval) == 1)
			    ret = 1;
			else
			    ret = 0;
		    } else if (xmlXPathIsInf(arg2->floatval) == -1) {
			if (xmlXPathIsInf(arg1->floatval) == -1)
			    ret = 1;
			else
			    ret = 0;
		    } else {
		        ret = (arg1->floatval == arg2->floatval);
		    }
		    break;
		case XPATH_USERS:
		case XPATH_POINT:
		case XPATH_RANGE:
		case XPATH_LOCATIONSET:
		    TODO
		    break;
		case XPATH_NODESET:
		case XPATH_XSLT_TREE:
		    break;
	    }
	    break;
        case XPATH_STRING:
	    switch (arg2->type) {
	        case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
		    xmlGenericError(xmlGenericErrorContext,
			    "Equal: undefined\n");
#endif
		    break;
		case XPATH_BOOLEAN:
		    if ((arg1->stringval == NULL) ||
			(arg1->stringval[0] == 0)) ret = 0;
		    else
			ret = 1;
		    ret = (arg2->boolval == ret);
		    break;
		case XPATH_STRING:
		    ret = xmlStrEqual(arg1->stringval, arg2->stringval);
		    break;
		case XPATH_NUMBER:
		    valuePush(ctxt, arg1);
		    xmlXPathNumberFunction(ctxt, 1);
		    arg1 = valuePop(ctxt);
		    
		    if (xmlXPathIsNaN(arg1->floatval) ||
			    xmlXPathIsNaN(arg2->floatval)) {
		        ret = 0;
		    } else if (xmlXPathIsInf(arg1->floatval) == 1) {
			if (xmlXPathIsInf(arg2->floatval) == 1)
			    ret = 1;
			else
			    ret = 0;
		    } else if (xmlXPathIsInf(arg1->floatval) == -1) {
			if (xmlXPathIsInf(arg2->floatval) == -1)
			    ret = 1;
			else
			    ret = 0;
		    } else if (xmlXPathIsInf(arg2->floatval) == 1) {
			if (xmlXPathIsInf(arg1->floatval) == 1)
			    ret = 1;
			else
			    ret = 0;
		    } else if (xmlXPathIsInf(arg2->floatval) == -1) {
			if (xmlXPathIsInf(arg1->floatval) == -1)
			    ret = 1;
			else
			    ret = 0;
		    } else {
		        ret = (arg1->floatval == arg2->floatval);
		    }
		    break;
		case XPATH_USERS:
		case XPATH_POINT:
		case XPATH_RANGE:
		case XPATH_LOCATIONSET:
		    TODO
		    break;
		case XPATH_NODESET:
		case XPATH_XSLT_TREE:
		    break;
	    }
	    break;
        case XPATH_USERS:
	case XPATH_POINT:
	case XPATH_RANGE:
	case XPATH_LOCATIONSET:
	    TODO
	    break;
	case XPATH_NODESET:
	case XPATH_XSLT_TREE:
	    break;
    }
    xmlXPathReleaseObject(ctxt->context, arg1);
    xmlXPathReleaseObject(ctxt->context, arg2);
    return(ret);
}

int
xmlXPathEqualValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg1, arg2, argtmp;
    int ret = 0;

    if ((ctxt == NULL) || (ctxt->context == NULL)) return(0);
    arg2 = valuePop(ctxt);
    arg1 = valuePop(ctxt);
    if ((arg1 == NULL) || (arg2 == NULL)) {
	if (arg1 != NULL)
	    xmlXPathReleaseObject(ctxt->context, arg1);
	else
	    xmlXPathReleaseObject(ctxt->context, arg2);
	XP_ERROR0(XPATH_INVALID_OPERAND);
    }

    if (arg1 == arg2) {
#ifdef DEBUG_EXPR
        xmlGenericError(xmlGenericErrorContext,
		"Equal: by pointer\n");
#endif
	xmlXPathFreeObject(arg1);
        return(1);
    }

    if ((arg2->type == XPATH_NODESET) || (arg2->type == XPATH_XSLT_TREE) ||
      (arg1->type == XPATH_NODESET) || (arg1->type == XPATH_XSLT_TREE)) {
	if ((arg1->type != XPATH_NODESET) && (arg1->type != XPATH_XSLT_TREE)) {
		argtmp = arg2;
		arg2 = arg1;
		arg1 = argtmp;
	}
	switch (arg2->type) {
	    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
		xmlGenericError(xmlGenericErrorContext,
			"Equal: undefined\n");
#endif
		break;
	    case XPATH_NODESET:
	    case XPATH_XSLT_TREE:
		ret = xmlXPathEqualNodeSets(arg1, arg2, 0);
		break;
	    case XPATH_BOOLEAN:
		if ((arg1->nodesetval == NULL) ||
		  (arg1->nodesetval->nodeNr == 0)) ret = 0;
		else
		    ret = 1;
		ret = (ret == arg2->boolval);
		break;
	    case XPATH_NUMBER:
		ret = xmlXPathEqualNodeSetFloat(ctxt, arg1, arg2->floatval, 0);
		break;
	    case XPATH_STRING:
		ret = xmlXPathEqualNodeSetString(arg1, arg2->stringval, 0);
		break;
	    case XPATH_USERS:
	    case XPATH_POINT:
	    case XPATH_RANGE:
	    case XPATH_LOCATIONSET:
		TODO
		break;
	}
	xmlXPathReleaseObject(ctxt->context, arg1);
	xmlXPathReleaseObject(ctxt->context, arg2);
	return(ret);
    }

    return (xmlXPathEqualValuesCommon(ctxt, arg1, arg2));
}

int
xmlXPathNotEqualValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg1, arg2, argtmp;
    int ret = 0;

    if ((ctxt == NULL) || (ctxt->context == NULL)) return(0);
    arg2 = valuePop(ctxt);
    arg1 = valuePop(ctxt);
    if ((arg1 == NULL) || (arg2 == NULL)) {
	if (arg1 != NULL)
	    xmlXPathReleaseObject(ctxt->context, arg1);
	else
	    xmlXPathReleaseObject(ctxt->context, arg2);
	XP_ERROR0(XPATH_INVALID_OPERAND);
    }

    if (arg1 == arg2) {
#ifdef DEBUG_EXPR
        xmlGenericError(xmlGenericErrorContext,
		"NotEqual: by pointer\n");
#endif
	xmlXPathReleaseObject(ctxt->context, arg1);
        return(0);
    }

    if ((arg2->type == XPATH_NODESET) || (arg2->type == XPATH_XSLT_TREE) ||
      (arg1->type == XPATH_NODESET) || (arg1->type == XPATH_XSLT_TREE)) {
	if ((arg1->type != XPATH_NODESET) && (arg1->type != XPATH_XSLT_TREE)) {
		argtmp = arg2;
		arg2 = arg1;
		arg1 = argtmp;
	}
	switch (arg2->type) {
	    case XPATH_UNDEFINED:
#ifdef DEBUG_EXPR
		xmlGenericError(xmlGenericErrorContext,
			"NotEqual: undefined\n");
#endif
		break;
	    case XPATH_NODESET:
	    case XPATH_XSLT_TREE:
		ret = xmlXPathEqualNodeSets(arg1, arg2, 1);
		break;
	    case XPATH_BOOLEAN:
		if ((arg1->nodesetval == NULL) ||
		  (arg1->nodesetval->nodeNr == 0)) ret = 0;
		else
		    ret = 1;
		ret = (ret != arg2->boolval);
		break;
	    case XPATH_NUMBER:
		ret = xmlXPathEqualNodeSetFloat(ctxt, arg1, arg2->floatval, 1);
		break;
	    case XPATH_STRING:
		ret = xmlXPathEqualNodeSetString(arg1, arg2->stringval,1);
		break;
	    case XPATH_USERS:
	    case XPATH_POINT:
	    case XPATH_RANGE:
	    case XPATH_LOCATIONSET:
		TODO
		break;
	}
	xmlXPathReleaseObject(ctxt->context, arg1);
	xmlXPathReleaseObject(ctxt->context, arg2);
	return(ret);
    }

    return (!xmlXPathEqualValuesCommon(ctxt, arg1, arg2));
}

int
xmlXPathCompareValues(xmlXPathParserContextPtr ctxt, int inf, int strict) {
    int ret = 0, arg1i = 0, arg2i = 0;
    xmlXPathObjectPtr arg1, arg2;

    if ((ctxt == NULL) || (ctxt->context == NULL)) return(0);
    arg2 = valuePop(ctxt);
    arg1 = valuePop(ctxt);
    if ((arg1 == NULL) || (arg2 == NULL)) {
	if (arg1 != NULL)
	    xmlXPathReleaseObject(ctxt->context, arg1);
	else
	    xmlXPathReleaseObject(ctxt->context, arg2);
	XP_ERROR0(XPATH_INVALID_OPERAND);
    }

    if ((arg2->type == XPATH_NODESET) || (arg2->type == XPATH_XSLT_TREE) ||
      (arg1->type == XPATH_NODESET) || (arg1->type == XPATH_XSLT_TREE)) {
	if (((arg2->type == XPATH_NODESET) || (arg2->type == XPATH_XSLT_TREE)) &&
	  ((arg1->type == XPATH_NODESET) || (arg1->type == XPATH_XSLT_TREE))){
	    ret = xmlXPathCompareNodeSets(inf, strict, arg1, arg2);
	} else {
	    if ((arg1->type == XPATH_NODESET) || (arg1->type == XPATH_XSLT_TREE)) {
		ret = xmlXPathCompareNodeSetValue(ctxt, inf, strict,
			                          arg1, arg2);
	    } else {
		ret = xmlXPathCompareNodeSetValue(ctxt, !inf, strict,
			                          arg2, arg1);
	    }
	}
	return(ret);
    }

    if (arg1->type != XPATH_NUMBER) {
	valuePush(ctxt, arg1);
	xmlXPathNumberFunction(ctxt, 1);
	arg1 = valuePop(ctxt);
    }
    if (arg1->type != XPATH_NUMBER) {
	xmlXPathFreeObject(arg1);
	xmlXPathFreeObject(arg2);
	XP_ERROR0(XPATH_INVALID_OPERAND);
    }
    if (arg2->type != XPATH_NUMBER) {
	valuePush(ctxt, arg2);
	xmlXPathNumberFunction(ctxt, 1);
	arg2 = valuePop(ctxt);
    }
    if (arg2->type != XPATH_NUMBER) {
	xmlXPathReleaseObject(ctxt->context, arg1);
	xmlXPathReleaseObject(ctxt->context, arg2);
	XP_ERROR0(XPATH_INVALID_OPERAND);
    }
    
    if (xmlXPathIsNaN(arg1->floatval) || xmlXPathIsNaN(arg2->floatval)) {
	ret=0;
    } else {
	arg1i=xmlXPathIsInf(arg1->floatval);
	arg2i=xmlXPathIsInf(arg2->floatval);
	if (inf && strict) {
	    if ((arg1i == -1 && arg2i != -1) ||
		(arg2i == 1 && arg1i != 1)) {
		ret = 1;
	    } else if (arg1i == 0 && arg2i == 0) {
		ret = (arg1->floatval < arg2->floatval);
	    } else {
		ret = 0;
	    }
	}
	else if (inf && !strict) {
	    if (arg1i == -1 || arg2i == 1) {
		ret = 1;
	    } else if (arg1i == 0 && arg2i == 0) {
		ret = (arg1->floatval <= arg2->floatval);
	    } else {
		ret = 0;
	    }
	}
	else if (!inf && strict) {
	    if ((arg1i == 1 && arg2i != 1) ||
		(arg2i == -1 && arg1i != -1)) {
		ret = 1;
	    } else if (arg1i == 0 && arg2i == 0) {
		ret = (arg1->floatval > arg2->floatval);
	    } else {
		ret = 0;
	    }
	}
	else if (!inf && !strict) {
	    if (arg1i == 1 || arg2i == -1) {
		ret = 1;
	    } else if (arg1i == 0 && arg2i == 0) {
		ret = (arg1->floatval >= arg2->floatval);
	    } else {
		ret = 0;
	    }
	}
    }
    xmlXPathReleaseObject(ctxt->context, arg1);
    xmlXPathReleaseObject(ctxt->context, arg2);
    return(ret);
}

void
xmlXPathValueFlipSign(xmlXPathParserContextPtr ctxt) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return;
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    if (xmlXPathIsNaN(ctxt->value->floatval))
        ctxt->value->floatval=xmlXPathNAN;
    else if (xmlXPathIsInf(ctxt->value->floatval) == 1)
        ctxt->value->floatval=xmlXPathNINF;
    else if (xmlXPathIsInf(ctxt->value->floatval) == -1)
        ctxt->value->floatval=xmlXPathPINF;
    else if (ctxt->value->floatval == 0) {
        if (xmlXPathGetSign(ctxt->value->floatval) == 0)
	    ctxt->value->floatval = xmlXPathNZERO;
	else
	    ctxt->value->floatval = 0;
    }
    else
        ctxt->value->floatval = - ctxt->value->floatval;
}

void
xmlXPathAddValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg;
    double val;

    arg = valuePop(ctxt);
    if (arg == NULL)
	XP_ERROR(XPATH_INVALID_OPERAND);
    val = xmlXPathCastToNumber(arg);
    xmlXPathReleaseObject(ctxt->context, arg);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    ctxt->value->floatval += val;
}

void
xmlXPathSubValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg;
    double val;

    arg = valuePop(ctxt);
    if (arg == NULL)
	XP_ERROR(XPATH_INVALID_OPERAND);
    val = xmlXPathCastToNumber(arg);
    xmlXPathReleaseObject(ctxt->context, arg);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    ctxt->value->floatval -= val;
}

void
xmlXPathMultValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg;
    double val;

    arg = valuePop(ctxt);
    if (arg == NULL)
	XP_ERROR(XPATH_INVALID_OPERAND);
    val = xmlXPathCastToNumber(arg);
    xmlXPathReleaseObject(ctxt->context, arg);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    ctxt->value->floatval *= val;
}

void
xmlXPathDivValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg;
    double val;

    arg = valuePop(ctxt);
    if (arg == NULL)
	XP_ERROR(XPATH_INVALID_OPERAND);
    val = xmlXPathCastToNumber(arg);
    xmlXPathReleaseObject(ctxt->context, arg);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    if (xmlXPathIsNaN(val) || xmlXPathIsNaN(ctxt->value->floatval))
	ctxt->value->floatval = xmlXPathNAN;
    else if (val == 0 && xmlXPathGetSign(val) != 0) {
	if (ctxt->value->floatval == 0)
	    ctxt->value->floatval = xmlXPathNAN;
	else if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = xmlXPathNINF;
	else if (ctxt->value->floatval < 0)
	    ctxt->value->floatval = xmlXPathPINF;
    }
    else if (val == 0) {
	if (ctxt->value->floatval == 0)
	    ctxt->value->floatval = xmlXPathNAN;
	else if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = xmlXPathPINF;
	else if (ctxt->value->floatval < 0)
	    ctxt->value->floatval = xmlXPathNINF;
    } else
	ctxt->value->floatval /= val;
}

void
xmlXPathModValues(xmlXPathParserContextPtr ctxt) {
    xmlXPathObjectPtr arg;
    double arg1, arg2;

    arg = valuePop(ctxt);
    if (arg == NULL)
	XP_ERROR(XPATH_INVALID_OPERAND);
    arg2 = xmlXPathCastToNumber(arg);
    xmlXPathReleaseObject(ctxt->context, arg);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    arg1 = ctxt->value->floatval;
    if (arg2 == 0)
	ctxt->value->floatval = xmlXPathNAN;
    else {
	ctxt->value->floatval = fmod(arg1, arg2);
    }
}


typedef xmlNodePtr (*xmlXPathTraversalFunction)
                    (xmlXPathParserContextPtr ctxt, xmlNodePtr cur);

typedef xmlNodePtr (*xmlXPathTraversalFunctionExt)
                    (xmlNodePtr cur, xmlNodePtr contextNode);

typedef xmlNodeSetPtr (*xmlXPathNodeSetMergeFunction)
		    (xmlNodeSetPtr, xmlNodeSetPtr, int);


xmlNodePtr
xmlXPathNextSelf(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL)
        return(ctxt->context->node);
    return(NULL);
}

xmlNodePtr
xmlXPathNextChild(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	if (ctxt->context->node == NULL) return(NULL);
	switch (ctxt->context->node->type) {
            case XML_ELEMENT_NODE:
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
            case XML_ENTITY_REF_NODE:
            case XML_ENTITY_NODE:
            case XML_PI_NODE:
            case XML_COMMENT_NODE:
            case XML_NOTATION_NODE:
            case XML_DTD_NODE:
		return(ctxt->context->node->children);
            case XML_DOCUMENT_NODE:
            case XML_DOCUMENT_TYPE_NODE:
            case XML_DOCUMENT_FRAG_NODE:
            case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE:
#endif
		return(((xmlDocPtr) ctxt->context->node)->children);
	    case XML_ELEMENT_DECL:
	    case XML_ATTRIBUTE_DECL:
	    case XML_ENTITY_DECL:
            case XML_ATTRIBUTE_NODE:
	    case XML_NAMESPACE_DECL:
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		return(NULL);
	}
	return(NULL);
    }
    if ((cur->type == XML_DOCUMENT_NODE) ||
        (cur->type == XML_HTML_DOCUMENT_NODE))
	return(NULL);
    return(cur->next);
}

static xmlNodePtr
xmlXPathNextChildElement(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	cur = ctxt->context->node;
	if (cur == NULL) return(NULL);
	switch (cur->type) {
            case XML_ELEMENT_NODE:
	    case XML_DOCUMENT_FRAG_NODE:
	    case XML_ENTITY_REF_NODE: 
            case XML_ENTITY_NODE:
		cur = cur->children;
		if (cur != NULL) {
		    if (cur->type == XML_ELEMENT_NODE)
			return(cur);
		    do {
			cur = cur->next;
		    } while ((cur != NULL) &&
			(cur->type != XML_ELEMENT_NODE));
		    return(cur);
		}
		return(NULL);
            case XML_DOCUMENT_NODE:
            case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE:
#endif
		return(xmlDocGetRootElement((xmlDocPtr) cur));
	    default:
		return(NULL);
	}
	return(NULL);
    }
    switch (cur->type) {
	case XML_ELEMENT_NODE:
	case XML_TEXT_NODE:
	case XML_ENTITY_REF_NODE:
	case XML_ENTITY_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	case XML_XINCLUDE_END:
	    break;
	 
	default:
	    return(NULL);
    }
    if (cur->next != NULL) {
	if (cur->next->type == XML_ELEMENT_NODE)
	    return(cur->next);
	cur = cur->next;
	do {
	    cur = cur->next;
	} while ((cur != NULL) && (cur->type != XML_ELEMENT_NODE));
	return(cur);
    }
    return(NULL);
}

static xmlNodePtr
xmlXPathNextDescendantOrSelfElemParent(xmlNodePtr cur,
				       xmlNodePtr contextNode)
{
    if (cur == NULL) {
	if (contextNode == NULL)
	    return(NULL);
	switch (contextNode->type) {
	    case XML_ELEMENT_NODE:
	    case XML_XINCLUDE_START:
	    case XML_DOCUMENT_FRAG_NODE:
	    case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE:
#endif
	    case XML_HTML_DOCUMENT_NODE:	    
		return(contextNode);
	    default:
		return(NULL);
	}
	return(NULL);
    } else {
	xmlNodePtr start = cur;

	while (cur != NULL) {
	    switch (cur->type) {
		case XML_ELEMENT_NODE:
		
		case XML_XINCLUDE_START:
		case XML_DOCUMENT_FRAG_NODE:
		    if (cur != start)
			return(cur);
		    if (cur->children != NULL) {
			cur = cur->children;
			continue;
		    }
		    break;
		
		case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
		case XML_DOCB_DOCUMENT_NODE:
#endif
		case XML_HTML_DOCUMENT_NODE:
		    if (cur != start)
			return(cur);
		    return(xmlDocGetRootElement((xmlDocPtr) cur));
		default:
		    break;
	    }

next_sibling:
	    if ((cur == NULL) || (cur == contextNode))
		return(NULL);
	    if (cur->next != NULL) {
		cur = cur->next;
	    } else {
		cur = cur->parent;
		goto next_sibling;
	    }
	}
    }
    return(NULL);
}

xmlNodePtr
xmlXPathNextDescendant(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	if (ctxt->context->node == NULL)
	    return(NULL);
	if ((ctxt->context->node->type == XML_ATTRIBUTE_NODE) ||
	    (ctxt->context->node->type == XML_NAMESPACE_DECL))
	    return(NULL);

        if (ctxt->context->node == (xmlNodePtr) ctxt->context->doc)
	    return(ctxt->context->doc->children);
        return(ctxt->context->node->children);
    }

    if (cur->children != NULL) {
	if (cur->children->type != XML_ENTITY_DECL) {
	    cur = cur->children;
	    if (cur->type != XML_DTD_NODE)
		return(cur);
	}
    }

    if (cur == ctxt->context->node) return(NULL);

    while (cur->next != NULL) {
	cur = cur->next;
	if ((cur->type != XML_ENTITY_DECL) &&
	    (cur->type != XML_DTD_NODE))
	    return(cur);
    }

    do {
        cur = cur->parent;
	if (cur == NULL) break;
	if (cur == ctxt->context->node) return(NULL);
	if (cur->next != NULL) {
	    cur = cur->next;
	    return(cur);
	}
    } while (cur != NULL);
    return(cur);
}

xmlNodePtr
xmlXPathNextDescendantOrSelf(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	if (ctxt->context->node == NULL)
	    return(NULL);
	if ((ctxt->context->node->type == XML_ATTRIBUTE_NODE) ||
	    (ctxt->context->node->type == XML_NAMESPACE_DECL))
	    return(NULL);
        return(ctxt->context->node);
    }

    return(xmlXPathNextDescendant(ctxt, cur));
}

xmlNodePtr
xmlXPathNextParent(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	if (ctxt->context->node == NULL) return(NULL);
	switch (ctxt->context->node->type) {
            case XML_ELEMENT_NODE:
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
            case XML_ENTITY_REF_NODE:
            case XML_ENTITY_NODE:
            case XML_PI_NODE:
            case XML_COMMENT_NODE:
            case XML_NOTATION_NODE:
            case XML_DTD_NODE:
	    case XML_ELEMENT_DECL:
	    case XML_ATTRIBUTE_DECL:
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
	    case XML_ENTITY_DECL:
		if (ctxt->context->node->parent == NULL)
		    return((xmlNodePtr) ctxt->context->doc);
		if ((ctxt->context->node->parent->type == XML_ELEMENT_NODE) &&
		    ((ctxt->context->node->parent->name[0] == ' ') ||
		     (xmlStrEqual(ctxt->context->node->parent->name,
				 BAD_CAST "fake node libxslt"))))
		    return(NULL);
		return(ctxt->context->node->parent);
            case XML_ATTRIBUTE_NODE: {
		xmlAttrPtr att = (xmlAttrPtr) ctxt->context->node;

		return(att->parent);
	    }
            case XML_DOCUMENT_NODE:
            case XML_DOCUMENT_TYPE_NODE:
            case XML_DOCUMENT_FRAG_NODE:
            case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE:
#endif
                return(NULL);
	    case XML_NAMESPACE_DECL: {
		xmlNsPtr ns = (xmlNsPtr) ctxt->context->node;

		if ((ns->next != NULL) &&
		    (ns->next->type != XML_NAMESPACE_DECL))
		    return((xmlNodePtr) ns->next);
                return(NULL);
	    }
	}
    }
    return(NULL);
}

xmlNodePtr
xmlXPathNextAncestor(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
	if (ctxt->context->node == NULL) return(NULL);
	switch (ctxt->context->node->type) {
            case XML_ELEMENT_NODE:
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
            case XML_ENTITY_REF_NODE:
            case XML_ENTITY_NODE:
            case XML_PI_NODE:
            case XML_COMMENT_NODE:
	    case XML_DTD_NODE:
	    case XML_ELEMENT_DECL:
	    case XML_ATTRIBUTE_DECL:
	    case XML_ENTITY_DECL:
            case XML_NOTATION_NODE:
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		if (ctxt->context->node->parent == NULL)
		    return((xmlNodePtr) ctxt->context->doc);
		if ((ctxt->context->node->parent->type == XML_ELEMENT_NODE) &&
		    ((ctxt->context->node->parent->name[0] == ' ') ||
		     (xmlStrEqual(ctxt->context->node->parent->name,
				 BAD_CAST "fake node libxslt"))))
		    return(NULL);
		return(ctxt->context->node->parent);
            case XML_ATTRIBUTE_NODE: {
		xmlAttrPtr tmp = (xmlAttrPtr) ctxt->context->node;

		return(tmp->parent);
	    }
            case XML_DOCUMENT_NODE:
            case XML_DOCUMENT_TYPE_NODE:
            case XML_DOCUMENT_FRAG_NODE:
            case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	    case XML_DOCB_DOCUMENT_NODE:
#endif
                return(NULL);
	    case XML_NAMESPACE_DECL: {
		xmlNsPtr ns = (xmlNsPtr) ctxt->context->node;

		if ((ns->next != NULL) &&
		    (ns->next->type != XML_NAMESPACE_DECL))
		    return((xmlNodePtr) ns->next);
		
                return(NULL);
	    }
	}
	return(NULL);
    }
    if (cur == ctxt->context->doc->children)
	return((xmlNodePtr) ctxt->context->doc);
    if (cur == (xmlNodePtr) ctxt->context->doc)
	return(NULL);
    switch (cur->type) {
	case XML_ELEMENT_NODE:
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_ENTITY_REF_NODE:
	case XML_ENTITY_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	case XML_NOTATION_NODE:
	case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
	    if (cur->parent == NULL)
		return(NULL);
	    if ((cur->parent->type == XML_ELEMENT_NODE) &&
		((cur->parent->name[0] == ' ') ||
		 (xmlStrEqual(cur->parent->name,
			      BAD_CAST "fake node libxslt"))))
		return(NULL);
	    return(cur->parent);
	case XML_ATTRIBUTE_NODE: {
	    xmlAttrPtr att = (xmlAttrPtr) ctxt->context->node;

	    return(att->parent);
	}
	case XML_NAMESPACE_DECL: {
	    xmlNsPtr ns = (xmlNsPtr) ctxt->context->node;

	    if ((ns->next != NULL) &&
	        (ns->next->type != XML_NAMESPACE_DECL))
	        return((xmlNodePtr) ns->next);
	    
            return(NULL);
	}
	case XML_DOCUMENT_NODE:
	case XML_DOCUMENT_TYPE_NODE:
	case XML_DOCUMENT_FRAG_NODE:
	case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	    return(NULL);
    }
    return(NULL);
}

xmlNodePtr
xmlXPathNextAncestorOrSelf(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL)
        return(ctxt->context->node);
    return(xmlXPathNextAncestor(ctxt, cur));
}

xmlNodePtr
xmlXPathNextFollowingSibling(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if ((ctxt->context->node->type == XML_ATTRIBUTE_NODE) ||
	(ctxt->context->node->type == XML_NAMESPACE_DECL))
	return(NULL);
    if (cur == (xmlNodePtr) ctxt->context->doc)
        return(NULL);
    if (cur == NULL)
        return(ctxt->context->node->next);
    return(cur->next);
}

xmlNodePtr
xmlXPathNextPrecedingSibling(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if ((ctxt->context->node->type == XML_ATTRIBUTE_NODE) ||
	(ctxt->context->node->type == XML_NAMESPACE_DECL))
	return(NULL);
    if (cur == (xmlNodePtr) ctxt->context->doc)
        return(NULL);
    if (cur == NULL)
        return(ctxt->context->node->prev);
    if ((cur->prev != NULL) && (cur->prev->type == XML_DTD_NODE)) {
	cur = cur->prev;
	if (cur == NULL)
	    return(ctxt->context->node->prev);
    }
    return(cur->prev);
}

xmlNodePtr
xmlXPathNextFollowing(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if ((cur != NULL) && (cur->type  != XML_ATTRIBUTE_NODE) &&
        (cur->type != XML_NAMESPACE_DECL) && (cur->children != NULL))
        return(cur->children);

    if (cur == NULL) {
        cur = ctxt->context->node;
        if (cur->type == XML_NAMESPACE_DECL)
            return(NULL);
        if (cur->type == XML_ATTRIBUTE_NODE)
            cur = cur->parent;
    }
    if (cur == NULL) return(NULL) ; 
    if (cur->next != NULL) return(cur->next) ;
    do {
        cur = cur->parent;
        if (cur == NULL) break;
        if (cur == (xmlNodePtr) ctxt->context->doc) return(NULL);
        if (cur->next != NULL) return(cur->next);
    } while (cur != NULL);
    return(cur);
}

static int
xmlXPathIsAncestor(xmlNodePtr ancestor, xmlNodePtr node) {
    if ((ancestor == NULL) || (node == NULL)) return(0);
    
    if (ancestor->doc != node->doc) return(0);
    
    if (ancestor == (xmlNodePtr) node->doc) return(1);
    if (node == (xmlNodePtr) ancestor->doc) return(0);
    while (node->parent != NULL) {
        if (node->parent == ancestor)
            return(1);
	node = node->parent;
    }
    return(0);
}

xmlNodePtr
xmlXPathNextPreceding(xmlXPathParserContextPtr ctxt, xmlNodePtr cur)
{
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
        cur = ctxt->context->node;
        if (cur->type == XML_NAMESPACE_DECL)
            return(NULL);
        if (cur->type == XML_ATTRIBUTE_NODE)
            return(cur->parent);
    }
    if (cur == NULL)
	return (NULL);
    if ((cur->prev != NULL) && (cur->prev->type == XML_DTD_NODE))
	cur = cur->prev;
    do {
        if (cur->prev != NULL) {
            for (cur = cur->prev; cur->last != NULL; cur = cur->last) ;
            return (cur);
        }

        cur = cur->parent;
        if (cur == NULL)
            return (NULL);
        if (cur == ctxt->context->doc->children)
            return (NULL);
    } while (xmlXPathIsAncestor(cur, ctxt->context->node));
    return (cur);
}

static xmlNodePtr
xmlXPathNextPrecedingInternal(xmlXPathParserContextPtr ctxt,
                              xmlNodePtr cur)
{
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (cur == NULL) {
        cur = ctxt->context->node;
        if (cur == NULL)
            return (NULL);
        if (cur->type == XML_NAMESPACE_DECL)
            return (NULL);
        ctxt->ancestor = cur->parent;
    }
    if ((cur->prev != NULL) && (cur->prev->type == XML_DTD_NODE))
	cur = cur->prev;
    while (cur->prev == NULL) {
        cur = cur->parent;
        if (cur == NULL)
            return (NULL);
        if (cur == ctxt->context->doc->children)
            return (NULL);
        if (cur != ctxt->ancestor)
            return (cur);
        ctxt->ancestor = cur->parent;
    }
    cur = cur->prev;
    while (cur->last != NULL)
        cur = cur->last;
    return (cur);
}

xmlNodePtr
xmlXPathNextNamespace(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (ctxt->context->node->type != XML_ELEMENT_NODE) return(NULL);
    if (ctxt->context->tmpNsList == NULL && cur != (xmlNodePtr) xmlXPathXMLNamespace) {
        if (ctxt->context->tmpNsList != NULL)
	    xmlFree(ctxt->context->tmpNsList);
	ctxt->context->tmpNsList =
	    xmlGetNsList(ctxt->context->doc, ctxt->context->node);
	ctxt->context->tmpNsNr = 0;
	if (ctxt->context->tmpNsList != NULL) {
	    while (ctxt->context->tmpNsList[ctxt->context->tmpNsNr] != NULL) {
		ctxt->context->tmpNsNr++;
	    }
	}
	return((xmlNodePtr) xmlXPathXMLNamespace);
    }
    if (ctxt->context->tmpNsNr > 0) {
	return (xmlNodePtr)ctxt->context->tmpNsList[--ctxt->context->tmpNsNr];
    } else {
	if (ctxt->context->tmpNsList != NULL)
	    xmlFree(ctxt->context->tmpNsList);
	ctxt->context->tmpNsList = NULL;
	return(NULL);
    }
}

xmlNodePtr
xmlXPathNextAttribute(xmlXPathParserContextPtr ctxt, xmlNodePtr cur) {
    if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
    if (ctxt->context->node == NULL)
	return(NULL);
    if (ctxt->context->node->type != XML_ELEMENT_NODE)
	return(NULL);
    if (cur == NULL) {
        if (ctxt->context->node == (xmlNodePtr) ctxt->context->doc)
	    return(NULL);
        return((xmlNodePtr)ctxt->context->node->properties);
    }
    return((xmlNodePtr)cur->next);
}


#define IS_FUNCTION			200



void
xmlXPathRoot(xmlXPathParserContextPtr ctxt) {
    if ((ctxt == NULL) || (ctxt->context == NULL))
	return;
    ctxt->context->node = (xmlNodePtr) ctxt->context->doc;
    valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
	ctxt->context->node));
}



void
xmlXPathLastFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    if (ctxt->context->contextSize >= 0) {
	valuePush(ctxt,
	    xmlXPathCacheNewFloat(ctxt->context,
		(double) ctxt->context->contextSize));
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext,
		"last() : %d\n", ctxt->context->contextSize);
#endif
    } else {
	XP_ERROR(XPATH_INVALID_CTXT_SIZE);
    }
}

void
xmlXPathPositionFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    if (ctxt->context->proximityPosition >= 0) {
	valuePush(ctxt,
	      xmlXPathCacheNewFloat(ctxt->context,
		(double) ctxt->context->proximityPosition));
#ifdef DEBUG_EXPR
	xmlGenericError(xmlGenericErrorContext, "position() : %d\n",
		ctxt->context->proximityPosition);
#endif
    } else {
	XP_ERROR(XPATH_INVALID_CTXT_POSITION);
    }
}

void
xmlXPathCountFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_NODESET) &&
	 (ctxt->value->type != XPATH_XSLT_TREE)))
	XP_ERROR(XPATH_INVALID_TYPE);
    cur = valuePop(ctxt);

    if ((cur == NULL) || (cur->nodesetval == NULL))
	valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, (double) 0));
    else if ((cur->type == XPATH_NODESET) || (cur->type == XPATH_XSLT_TREE)) {
	valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context,
	    (double) cur->nodesetval->nodeNr));
    } else {
	if ((cur->nodesetval->nodeNr != 1) ||
	    (cur->nodesetval->nodeTab == NULL)) {
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, (double) 0));
	} else {
	    xmlNodePtr tmp;
	    int i = 0;

	    tmp = cur->nodesetval->nodeTab[0];
	    if (tmp != NULL) {
		tmp = tmp->children;
		while (tmp != NULL) {
		    tmp = tmp->next;
		    i++;
		}
	    }
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, (double) i));
	}
    }
    xmlXPathReleaseObject(ctxt->context, cur);
}

static xmlNodeSetPtr
xmlXPathGetElementsByIds (xmlDocPtr doc, const xmlChar *ids) {
    xmlNodeSetPtr ret;
    const xmlChar *cur = ids;
    xmlChar *ID;
    xmlAttrPtr attr;
    xmlNodePtr elem = NULL;

    if (ids == NULL) return(NULL);

    ret = xmlXPathNodeSetCreate(NULL);
    if (ret == NULL)
        return(ret);

    while (IS_BLANK_CH(*cur)) cur++;
    while (*cur != 0) {
	while ((!IS_BLANK_CH(*cur)) && (*cur != 0))
	    cur++;

        ID = xmlStrndup(ids, cur - ids);
	if (ID != NULL) {
	    attr = xmlGetID(doc, ID);
	    if (attr != NULL) {
		if (attr->type == XML_ATTRIBUTE_NODE)
		    elem = attr->parent;
		else if (attr->type == XML_ELEMENT_NODE)
		    elem = (xmlNodePtr) attr;
		else
		    elem = NULL;
		if (elem != NULL)
		    xmlXPathNodeSetAdd(ret, elem);
	    }
	    xmlFree(ID);
	}

	while (IS_BLANK_CH(*cur)) cur++;
	ids = cur;
    }
    return(ret);
}

void
xmlXPathIdFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlChar *tokens;
    xmlNodeSetPtr ret;
    xmlXPathObjectPtr obj;

    CHECK_ARITY(1);
    obj = valuePop(ctxt);
    if (obj == NULL) XP_ERROR(XPATH_INVALID_OPERAND);
    if ((obj->type == XPATH_NODESET) || (obj->type == XPATH_XSLT_TREE)) {
	xmlNodeSetPtr ns;
	int i;

	ret = xmlXPathNodeSetCreate(NULL);

	if (obj->nodesetval != NULL) {
	    for (i = 0; i < obj->nodesetval->nodeNr; i++) {
		tokens =
		    xmlXPathCastNodeToString(obj->nodesetval->nodeTab[i]);
		ns = xmlXPathGetElementsByIds(ctxt->context->doc, tokens);
		ret = xmlXPathNodeSetMerge(ret, ns);
		xmlXPathFreeNodeSet(ns);
		if (tokens != NULL)
		    xmlFree(tokens);
	    }
	}
	xmlXPathReleaseObject(ctxt->context, obj);
	valuePush(ctxt, xmlXPathCacheWrapNodeSet(ctxt->context, ret));
	return;
    }
    obj = xmlXPathCacheConvertString(ctxt->context, obj);
    ret = xmlXPathGetElementsByIds(ctxt->context->doc, obj->stringval);
    valuePush(ctxt, xmlXPathCacheWrapNodeSet(ctxt->context, ret));
    xmlXPathReleaseObject(ctxt->context, obj);
    return;
}

void
xmlXPathLocalNameFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (ctxt == NULL) return;

    if (nargs == 0) {
	valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
	    ctxt->context->node));
	nargs = 1;
    }

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_NODESET) &&
	 (ctxt->value->type != XPATH_XSLT_TREE)))
	XP_ERROR(XPATH_INVALID_TYPE);
    cur = valuePop(ctxt);

    if ((cur->nodesetval == NULL) || (cur->nodesetval->nodeNr == 0)) {
	valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
    } else {
	int i = 0; 
	switch (cur->nodesetval->nodeTab[i]->type) {
	case XML_ELEMENT_NODE:
	case XML_ATTRIBUTE_NODE:
	case XML_PI_NODE:
	    if (cur->nodesetval->nodeTab[i]->name[0] == ' ')
		valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	    else
		valuePush(ctxt,
		      xmlXPathCacheNewString(ctxt->context,
			cur->nodesetval->nodeTab[i]->name));
	    break;
	case XML_NAMESPACE_DECL:
	    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
			((xmlNsPtr)cur->nodesetval->nodeTab[i])->prefix));
	    break;
	default:
	    valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	}
    }
    xmlXPathReleaseObject(ctxt->context, cur);
}

void
xmlXPathNamespaceURIFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (ctxt == NULL) return;

    if (nargs == 0) {
	valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
	    ctxt->context->node));
	nargs = 1;
    }
    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_NODESET) &&
	 (ctxt->value->type != XPATH_XSLT_TREE)))
	XP_ERROR(XPATH_INVALID_TYPE);
    cur = valuePop(ctxt);

    if ((cur->nodesetval == NULL) || (cur->nodesetval->nodeNr == 0)) {
	valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
    } else {
	int i = 0; 
	switch (cur->nodesetval->nodeTab[i]->type) {
	case XML_ELEMENT_NODE:
	case XML_ATTRIBUTE_NODE:
	    if (cur->nodesetval->nodeTab[i]->ns == NULL)
		valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	    else
		valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
			  cur->nodesetval->nodeTab[i]->ns->href));
	    break;
	default:
	    valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
	}
    }
    xmlXPathReleaseObject(ctxt->context, cur);
}

static void
xmlXPathNameFunction(xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlXPathObjectPtr cur;

    if (nargs == 0) {
	valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
	    ctxt->context->node));
        nargs = 1;
    }

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
        ((ctxt->value->type != XPATH_NODESET) &&
         (ctxt->value->type != XPATH_XSLT_TREE)))
        XP_ERROR(XPATH_INVALID_TYPE);
    cur = valuePop(ctxt);

    if ((cur->nodesetval == NULL) || (cur->nodesetval->nodeNr == 0)) {
        valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
    } else {
        int i = 0;              

        switch (cur->nodesetval->nodeTab[i]->type) {
            case XML_ELEMENT_NODE:
            case XML_ATTRIBUTE_NODE:
		if (cur->nodesetval->nodeTab[i]->name[0] == ' ')
		    valuePush(ctxt,
			xmlXPathCacheNewCString(ctxt->context, ""));
		else if ((cur->nodesetval->nodeTab[i]->ns == NULL) ||
                         (cur->nodesetval->nodeTab[i]->ns->prefix == NULL)) {
		    valuePush(ctxt,
		        xmlXPathCacheNewString(ctxt->context,
			    cur->nodesetval->nodeTab[i]->name));
		} else {
		    xmlChar *fullname;

		    fullname = xmlBuildQName(cur->nodesetval->nodeTab[i]->name,
				     cur->nodesetval->nodeTab[i]->ns->prefix,
				     NULL, 0);
		    if (fullname == cur->nodesetval->nodeTab[i]->name)
			fullname = xmlStrdup(cur->nodesetval->nodeTab[i]->name);
		    if (fullname == NULL) {
			XP_ERROR(XPATH_MEMORY_ERROR);
		    }
		    valuePush(ctxt, xmlXPathCacheWrapString(
			ctxt->context, fullname));
                }
                break;
            default:
		valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
		    cur->nodesetval->nodeTab[i]));
                xmlXPathLocalNameFunction(ctxt, 1);
        }
    }
    xmlXPathReleaseObject(ctxt->context, cur);
}


void
xmlXPathStringFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (ctxt == NULL) return;
    if (nargs == 0) {
    valuePush(ctxt,
	xmlXPathCacheWrapString(ctxt->context,
	    xmlXPathCastNodeToString(ctxt->context->node)));
	return;
    }

    CHECK_ARITY(1);
    cur = valuePop(ctxt);
    if (cur == NULL) XP_ERROR(XPATH_INVALID_OPERAND);
    valuePush(ctxt, xmlXPathCacheConvertString(ctxt->context, cur));
}

void
xmlXPathStringLengthFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    if (nargs == 0) {
        if ((ctxt == NULL) || (ctxt->context == NULL))
	    return;
	if (ctxt->context->node == NULL) {
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, 0));
	} else {
	    xmlChar *content;

	    content = xmlXPathCastNodeToString(ctxt->context->node);
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context,
		xmlUTF8Strlen(content)));
	    xmlFree(content);
	}
	return;
    }
    CHECK_ARITY(1);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    cur = valuePop(ctxt);
    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context,
	xmlUTF8Strlen(cur->stringval)));
    xmlXPathReleaseObject(ctxt->context, cur);
}

void
xmlXPathConcatFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur, newobj;
    xmlChar *tmp;

    if (ctxt == NULL) return;
    if (nargs < 2) {
	CHECK_ARITY(2);
    }

    CAST_TO_STRING;
    cur = valuePop(ctxt);
    if ((cur == NULL) || (cur->type != XPATH_STRING)) {
	xmlXPathReleaseObject(ctxt->context, cur);
	return;
    }
    nargs--;

    while (nargs > 0) {
	CAST_TO_STRING;
	newobj = valuePop(ctxt);
	if ((newobj == NULL) || (newobj->type != XPATH_STRING)) {
	    xmlXPathReleaseObject(ctxt->context, newobj);
	    xmlXPathReleaseObject(ctxt->context, cur);
	    XP_ERROR(XPATH_INVALID_TYPE);
	}
	tmp = xmlStrcat(newobj->stringval, cur->stringval);
	newobj->stringval = cur->stringval;
	cur->stringval = tmp;
	xmlXPathReleaseObject(ctxt->context, newobj);
	nargs--;
    }
    valuePush(ctxt, cur);
}

void
xmlXPathContainsFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr hay, needle;

    CHECK_ARITY(2);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    needle = valuePop(ctxt);
    CAST_TO_STRING;
    hay = valuePop(ctxt);

    if ((hay == NULL) || (hay->type != XPATH_STRING)) {
	xmlXPathReleaseObject(ctxt->context, hay);
	xmlXPathReleaseObject(ctxt->context, needle);
	XP_ERROR(XPATH_INVALID_TYPE);
    }
    if (xmlStrstr(hay->stringval, needle->stringval))
	valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 1));
    else
	valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 0));
    xmlXPathReleaseObject(ctxt->context, hay);
    xmlXPathReleaseObject(ctxt->context, needle);
}

void
xmlXPathStartsWithFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr hay, needle;
    int n;

    CHECK_ARITY(2);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    needle = valuePop(ctxt);
    CAST_TO_STRING;
    hay = valuePop(ctxt);

    if ((hay == NULL) || (hay->type != XPATH_STRING)) {
	xmlXPathReleaseObject(ctxt->context, hay);
	xmlXPathReleaseObject(ctxt->context, needle);
	XP_ERROR(XPATH_INVALID_TYPE);
    }
    n = xmlStrlen(needle->stringval);
    if (xmlStrncmp(hay->stringval, needle->stringval, n))
        valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 0));
    else
        valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 1));
    xmlXPathReleaseObject(ctxt->context, hay);
    xmlXPathReleaseObject(ctxt->context, needle);
}

void
xmlXPathSubstringFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr str, start, len;
    double le=0, in;
    int i, l, m;
    xmlChar *ret;

    if (nargs < 2) {
	CHECK_ARITY(2);
    }
    if (nargs > 3) {
	CHECK_ARITY(3);
    }
    if (nargs == 3) {
	CAST_TO_NUMBER;
	CHECK_TYPE(XPATH_NUMBER);
	len = valuePop(ctxt);
	le = len->floatval;
	xmlXPathReleaseObject(ctxt->context, len);
    }

    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);
    start = valuePop(ctxt);
    in = start->floatval;
    xmlXPathReleaseObject(ctxt->context, start);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    str = valuePop(ctxt);
    m = xmlUTF8Strlen((const unsigned char *)str->stringval);

    if (nargs != 3) {
	le = (double)m;
	if (in < 1.0)
	    in = 1.0;
    }

    if (!xmlXPathIsInf(in) && !xmlXPathIsNaN(in + le)) {
        i = (int) in;
        if (((double)i)+0.5 <= in) i++;

	if (xmlXPathIsInf(le) == 1) {
	    l = m;
	    if (i < 1)
		i = 1;
	}
	else if (xmlXPathIsInf(le) == -1 || le < 0.0)
	    l = 0;
	else {
	    l = (int) le;
	    if (((double)l)+0.5 <= le) l++;
	}

	
        i -= 1;
        l += i;
        if (i < 0)
            i = 0;
        if (l > m)
            l = m;

        
        l -= i;

        ret = xmlUTF8Strsub(str->stringval, i, l);
    }
    else {
        ret = NULL;
    }
    if (ret == NULL)
	valuePush(ctxt, xmlXPathCacheNewCString(ctxt->context, ""));
    else {
	valuePush(ctxt, xmlXPathCacheNewString(ctxt->context, ret));
	xmlFree(ret);
    }
    xmlXPathReleaseObject(ctxt->context, str);
}

void
xmlXPathSubstringBeforeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
  xmlXPathObjectPtr str;
  xmlXPathObjectPtr find;
  xmlBufferPtr target;
  const xmlChar *point;
  int offset;

  CHECK_ARITY(2);
  CAST_TO_STRING;
  find = valuePop(ctxt);
  CAST_TO_STRING;
  str = valuePop(ctxt);

  target = xmlBufferCreate();
  if (target) {
    point = xmlStrstr(str->stringval, find->stringval);
    if (point) {
      offset = (int)(point - str->stringval);
      xmlBufferAdd(target, str->stringval, offset);
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
  }
  xmlXPathReleaseObject(ctxt->context, str);
  xmlXPathReleaseObject(ctxt->context, find);
}

void
xmlXPathSubstringAfterFunction(xmlXPathParserContextPtr ctxt, int nargs) {
  xmlXPathObjectPtr str;
  xmlXPathObjectPtr find;
  xmlBufferPtr target;
  const xmlChar *point;
  int offset;

  CHECK_ARITY(2);
  CAST_TO_STRING;
  find = valuePop(ctxt);
  CAST_TO_STRING;
  str = valuePop(ctxt);

  target = xmlBufferCreate();
  if (target) {
    point = xmlStrstr(str->stringval, find->stringval);
    if (point) {
      offset = (int)(point - str->stringval) + xmlStrlen(find->stringval);
      xmlBufferAdd(target, &str->stringval[offset],
		   xmlStrlen(str->stringval) - offset);
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
  }
  xmlXPathReleaseObject(ctxt->context, str);
  xmlXPathReleaseObject(ctxt->context, find);
}

void
xmlXPathNormalizeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
  xmlXPathObjectPtr obj = NULL;
  xmlChar *source = NULL;
  xmlBufferPtr target;
  xmlChar blank;

  if (ctxt == NULL) return;
  if (nargs == 0) {
    
      valuePush(ctxt,
	  xmlXPathCacheWrapString(ctxt->context,
	    xmlXPathCastNodeToString(ctxt->context->node)));
    nargs = 1;
  }

  CHECK_ARITY(1);
  CAST_TO_STRING;
  CHECK_TYPE(XPATH_STRING);
  obj = valuePop(ctxt);
  source = obj->stringval;

  target = xmlBufferCreate();
  if (target && source) {

    
    while (IS_BLANK_CH(*source))
      source++;

    
    blank = 0;
    while (*source) {
      if (IS_BLANK_CH(*source)) {
	blank = 0x20;
      } else {
	if (blank) {
	  xmlBufferAdd(target, &blank, 1);
	  blank = 0;
	}
	xmlBufferAdd(target, source, 1);
      }
      source++;
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
  }
  xmlXPathReleaseObject(ctxt->context, obj);
}

void
xmlXPathTranslateFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr str;
    xmlXPathObjectPtr from;
    xmlXPathObjectPtr to;
    xmlBufferPtr target;
    int offset, max;
    xmlChar ch;
    const xmlChar *point;
    xmlChar *cptr;

    CHECK_ARITY(3);

    CAST_TO_STRING;
    to = valuePop(ctxt);
    CAST_TO_STRING;
    from = valuePop(ctxt);
    CAST_TO_STRING;
    str = valuePop(ctxt);

    target = xmlBufferCreate();
    if (target) {
	max = xmlUTF8Strlen(to->stringval);
	for (cptr = str->stringval; (ch=*cptr); ) {
	    offset = xmlUTF8Strloc(from->stringval, cptr);
	    if (offset >= 0) {
		if (offset < max) {
		    point = xmlUTF8Strpos(to->stringval, offset);
		    if (point)
			xmlBufferAdd(target, point, xmlUTF8Strsize(point, 1));
		}
	    } else
		xmlBufferAdd(target, cptr, xmlUTF8Strsize(cptr, 1));

	    
	    cptr++;
	    if ( ch & 0x80 ) {
		
		if ( (ch & 0xc0) != 0xc0 ) {
		    xmlGenericError(xmlGenericErrorContext,
			"xmlXPathTranslateFunction: Invalid UTF8 string\n");
		    break;
		}
		
		while ( (ch <<= 1) & 0x80 )
		    if ( (*cptr++ & 0xc0) != 0x80 ) {
			xmlGenericError(xmlGenericErrorContext,
			    "xmlXPathTranslateFunction: Invalid UTF8 string\n");
			break;
		    }
		if (ch & 0x80) 
		    break;
	    }
	}
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
    xmlXPathReleaseObject(ctxt->context, str);
    xmlXPathReleaseObject(ctxt->context, from);
    xmlXPathReleaseObject(ctxt->context, to);
}

void
xmlXPathBooleanFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;

    CHECK_ARITY(1);
    cur = valuePop(ctxt);
    if (cur == NULL) XP_ERROR(XPATH_INVALID_OPERAND);
    cur = xmlXPathCacheConvertBoolean(ctxt->context, cur);
    valuePush(ctxt, cur);
}

void
xmlXPathNotFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(1);
    CAST_TO_BOOLEAN;
    CHECK_TYPE(XPATH_BOOLEAN);
    ctxt->value->boolval = ! ctxt->value->boolval;
}

void
xmlXPathTrueFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 1));
}

void
xmlXPathFalseFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, 0));
}

void
xmlXPathLangFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr val = NULL;
    const xmlChar *theLang = NULL;
    const xmlChar *lang;
    int ret = 0;
    int i;

    CHECK_ARITY(1);
    CAST_TO_STRING;
    CHECK_TYPE(XPATH_STRING);
    val = valuePop(ctxt);
    lang = val->stringval;
    theLang = xmlNodeGetLang(ctxt->context->node);
    if ((theLang != NULL) && (lang != NULL)) {
        for (i = 0;lang[i] != 0;i++)
	    if (toupper(lang[i]) != toupper(theLang[i]))
	        goto not_equal;
	if ((theLang[i] == 0) || (theLang[i] == '-'))
	    ret = 1;
    }
not_equal:
    if (theLang != NULL)
	xmlFree((void *)theLang);

    xmlXPathReleaseObject(ctxt->context, val);
    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, ret));
}

void
xmlXPathNumberFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;
    double res;

    if (ctxt == NULL) return;
    if (nargs == 0) {
	if (ctxt->context->node == NULL) {
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, 0.0));
	} else {
	    xmlChar* content = xmlNodeGetContent(ctxt->context->node);

	    res = xmlXPathStringEvalNumber(content);
	    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, res));
	    xmlFree(content);
	}
	return;
    }

    CHECK_ARITY(1);
    cur = valuePop(ctxt);
    valuePush(ctxt, xmlXPathCacheConvertNumber(ctxt->context, cur));
}

void
xmlXPathSumFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr cur;
    int i;
    double res = 0.0;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_NODESET) &&
	 (ctxt->value->type != XPATH_XSLT_TREE)))
	XP_ERROR(XPATH_INVALID_TYPE);
    cur = valuePop(ctxt);

    if ((cur->nodesetval != NULL) && (cur->nodesetval->nodeNr != 0)) {
	for (i = 0; i < cur->nodesetval->nodeNr; i++) {
	    res += xmlXPathCastNodeToNumber(cur->nodesetval->nodeTab[i]);
	}
    }
    valuePush(ctxt, xmlXPathCacheNewFloat(ctxt->context, res));
    xmlXPathReleaseObject(ctxt->context, cur);
}

#define XTRUNC(f, v)            \
    f = fmod((v), INT_MAX);     \
    f = (v) - (f) + (double)((int)(f));

void
xmlXPathFloorFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    double f;

    CHECK_ARITY(1);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);

    XTRUNC(f, ctxt->value->floatval);
    if (f != ctxt->value->floatval) {
	if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = f;
	else
	    ctxt->value->floatval = f - 1;
    }
}

void
xmlXPathCeilingFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    double f;

    CHECK_ARITY(1);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);

#if 0
    ctxt->value->floatval = ceil(ctxt->value->floatval);
#else
    XTRUNC(f, ctxt->value->floatval);
    if (f != ctxt->value->floatval) {
	if (ctxt->value->floatval > 0)
	    ctxt->value->floatval = f + 1;
	else {
	    if (ctxt->value->floatval < 0 && f == 0)
	        ctxt->value->floatval = xmlXPathNZERO;
	    else
	        ctxt->value->floatval = f;
	}

    }
#endif
}

void
xmlXPathRoundFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    double f;

    CHECK_ARITY(1);
    CAST_TO_NUMBER;
    CHECK_TYPE(XPATH_NUMBER);

    if ((xmlXPathIsNaN(ctxt->value->floatval)) ||
	(xmlXPathIsInf(ctxt->value->floatval) == 1) ||
	(xmlXPathIsInf(ctxt->value->floatval) == -1) ||
	(ctxt->value->floatval == 0.0))
	return;

    XTRUNC(f, ctxt->value->floatval);
    if (ctxt->value->floatval < 0) {
	if (ctxt->value->floatval < f - 0.5)
	    ctxt->value->floatval = f - 1;
	else
	    ctxt->value->floatval = f;
	if (ctxt->value->floatval == 0)
	    ctxt->value->floatval = xmlXPathNZERO;
    } else {
	if (ctxt->value->floatval < f + 0.5)
	    ctxt->value->floatval = f;
	else
	    ctxt->value->floatval = f + 1;
    }
}


static void xmlXPathCompileExpr(xmlXPathParserContextPtr ctxt, int sort);
static void xmlXPathCompPredicate(xmlXPathParserContextPtr ctxt, int filter);
static void xmlXPathCompLocationPath(xmlXPathParserContextPtr ctxt);
static void xmlXPathCompRelativeLocationPath(xmlXPathParserContextPtr ctxt);
static xmlChar * xmlXPathParseNameComplex(xmlXPathParserContextPtr ctxt,
	                                  int qualified);


static int
xmlXPathCurrentChar(xmlXPathParserContextPtr ctxt, int *len) {
    unsigned char c;
    unsigned int val;
    const xmlChar *cur;

    if (ctxt == NULL)
	return(0);
    cur = ctxt->cur;

    c = *cur;
    if (c & 0x80) {
	if ((cur[1] & 0xc0) != 0x80)
	    goto encoding_error;
	if ((c & 0xe0) == 0xe0) {

	    if ((cur[2] & 0xc0) != 0x80)
		goto encoding_error;
	    if ((c & 0xf0) == 0xf0) {
		if (((c & 0xf8) != 0xf0) ||
		    ((cur[3] & 0xc0) != 0x80))
		    goto encoding_error;
		
		*len = 4;
		val = (cur[0] & 0x7) << 18;
		val |= (cur[1] & 0x3f) << 12;
		val |= (cur[2] & 0x3f) << 6;
		val |= cur[3] & 0x3f;
	    } else {
	      
		*len = 3;
		val = (cur[0] & 0xf) << 12;
		val |= (cur[1] & 0x3f) << 6;
		val |= cur[2] & 0x3f;
	    }
	} else {
	  
	    *len = 2;
	    val = (cur[0] & 0x1f) << 6;
	    val |= cur[1] & 0x3f;
	}
	if (!IS_CHAR(val)) {
	    XP_ERROR0(XPATH_INVALID_CHAR_ERROR);
	}
	return(val);
    } else {
	
	*len = 1;
	return((int) *cur);
    }
encoding_error:
    *len = 0;
    XP_ERROR0(XPATH_ENCODING_ERROR);
}


xmlChar *
xmlXPathParseNCName(xmlXPathParserContextPtr ctxt) {
    const xmlChar *in;
    xmlChar *ret;
    int count = 0;

    if ((ctxt == NULL) || (ctxt->cur == NULL)) return(NULL);
    in = ctxt->cur;
    if (((*in >= 0x61) && (*in <= 0x7A)) ||
	((*in >= 0x41) && (*in <= 0x5A)) ||
	(*in == '_')) {
	in++;
	while (((*in >= 0x61) && (*in <= 0x7A)) ||
	       ((*in >= 0x41) && (*in <= 0x5A)) ||
	       ((*in >= 0x30) && (*in <= 0x39)) ||
	       (*in == '_') || (*in == '.') ||
	       (*in == '-'))
	    in++;
	if ((*in == ' ') || (*in == '>') || (*in == '/') ||
            (*in == '[') || (*in == ']') || (*in == ':') ||
            (*in == '@') || (*in == '*')) {
	    count = in - ctxt->cur;
	    if (count == 0)
		return(NULL);
	    ret = xmlStrndup(ctxt->cur, count);
	    ctxt->cur = in;
	    return(ret);
	}
    }
    return(xmlXPathParseNameComplex(ctxt, 0));
}



static xmlChar *
xmlXPathParseQName(xmlXPathParserContextPtr ctxt, xmlChar **prefix) {
    xmlChar *ret = NULL;

    *prefix = NULL;
    ret = xmlXPathParseNCName(ctxt);
    if (ret && CUR == ':') {
        *prefix = ret;
	NEXT;
	ret = xmlXPathParseNCName(ctxt);
    }
    return(ret);
}


xmlChar *
xmlXPathParseName(xmlXPathParserContextPtr ctxt) {
    const xmlChar *in;
    xmlChar *ret;
    int count = 0;

    if ((ctxt == NULL) || (ctxt->cur == NULL)) return(NULL);
    in = ctxt->cur;
    if (((*in >= 0x61) && (*in <= 0x7A)) ||
	((*in >= 0x41) && (*in <= 0x5A)) ||
	(*in == '_') || (*in == ':')) {
	in++;
	while (((*in >= 0x61) && (*in <= 0x7A)) ||
	       ((*in >= 0x41) && (*in <= 0x5A)) ||
	       ((*in >= 0x30) && (*in <= 0x39)) ||
	       (*in == '_') || (*in == '-') ||
	       (*in == ':') || (*in == '.'))
	    in++;
	if ((*in > 0) && (*in < 0x80)) {
	    count = in - ctxt->cur;
	    ret = xmlStrndup(ctxt->cur, count);
	    ctxt->cur = in;
	    return(ret);
	}
    }
    return(xmlXPathParseNameComplex(ctxt, 1));
}

static xmlChar *
xmlXPathParseNameComplex(xmlXPathParserContextPtr ctxt, int qualified) {
    xmlChar buf[XML_MAX_NAMELEN + 5];
    int len = 0, l;
    int c;

    c = CUR_CHAR(l);
    if ((c == ' ') || (c == '>') || (c == '/') || 
        (c == '[') || (c == ']') || (c == '@') || 
        (c == '*') || 
	(!IS_LETTER(c) && (c != '_') &&
         ((qualified) && (c != ':')))) {
	return(NULL);
    }

    while ((c != ' ') && (c != '>') && (c != '/') && 
	   ((IS_LETTER(c)) || (IS_DIGIT(c)) ||
            (c == '.') || (c == '-') ||
	    (c == '_') || ((qualified) && (c == ':')) ||
	    (IS_COMBINING(c)) ||
	    (IS_EXTENDER(c)))) {
	COPY_BUF(l,buf,len,c);
	NEXTL(l);
	c = CUR_CHAR(l);
	if (len >= XML_MAX_NAMELEN) {
	    xmlChar *buffer;
	    int max = len * 2;

	    buffer = (xmlChar *) xmlMallocAtomic(max * sizeof(xmlChar));
	    if (buffer == NULL) {
		XP_ERRORNULL(XPATH_MEMORY_ERROR);
	    }
	    memcpy(buffer, buf, len);
	    while ((IS_LETTER(c)) || (IS_DIGIT(c)) || 
		   (c == '.') || (c == '-') ||
		   (c == '_') || ((qualified) && (c == ':')) ||
		   (IS_COMBINING(c)) ||
		   (IS_EXTENDER(c))) {
		if (len + 10 > max) {
		    max *= 2;
		    buffer = (xmlChar *) xmlRealloc(buffer,
			                            max * sizeof(xmlChar));
		    if (buffer == NULL) {
			XP_ERRORNULL(XPATH_MEMORY_ERROR);
		    }
		}
		COPY_BUF(l,buffer,len,c);
		NEXTL(l);
		c = CUR_CHAR(l);
	    }
	    buffer[len] = 0;
	    return(buffer);
	}
    }
    if (len == 0)
	return(NULL);
    return(xmlStrndup(buf, len));
}

#define MAX_FRAC 20

static double my_pow10[MAX_FRAC+1] = {
    1.0, 10.0, 100.0, 1000.0, 10000.0,
    100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0,
    10000000000.0, 100000000000.0, 1000000000000.0, 10000000000000.0,
    100000000000000.0,
    1000000000000000.0, 10000000000000000.0, 100000000000000000.0,
    1000000000000000000.0, 10000000000000000000.0, 100000000000000000000.0
};

double
xmlXPathStringEvalNumber(const xmlChar *str) {
    const xmlChar *cur = str;
    double ret;
    int ok = 0;
    int isneg = 0;
    int exponent = 0;
    int is_exponent_negative = 0;
#ifdef __GNUC__
    unsigned long tmp = 0;
    double temp;
#endif
    if (cur == NULL) return(0);
    while (IS_BLANK_CH(*cur)) cur++;
    if ((*cur != '.') && ((*cur < '0') || (*cur > '9')) && (*cur != '-')) {
        return(xmlXPathNAN);
    }
    if (*cur == '-') {
	isneg = 1;
	cur++;
    }

#ifdef __GNUC__
    ret = 0;
    while ((*cur >= '0') && (*cur <= '9')) {
	ret = ret * 10;
	tmp = (*cur - '0');
	ok = 1;
	cur++;
	temp = (double) tmp;
	ret = ret + temp;
    }
#else
    ret = 0;
    while ((*cur >= '0') && (*cur <= '9')) {
	ret = ret * 10 + (*cur - '0');
	ok = 1;
	cur++;
    }
#endif

    if (*cur == '.') {
	int v, frac = 0;
	double fraction = 0;

        cur++;
	if (((*cur < '0') || (*cur > '9')) && (!ok)) {
	    return(xmlXPathNAN);
	}
	while (((*cur >= '0') && (*cur <= '9')) && (frac < MAX_FRAC)) {
	    v = (*cur - '0');
	    fraction = fraction * 10 + v;
	    frac = frac + 1;
	    cur++;
	}
	fraction /= my_pow10[frac];
	ret = ret + fraction;
	while ((*cur >= '0') && (*cur <= '9'))
	    cur++;
    }
    if ((*cur == 'e') || (*cur == 'E')) {
      cur++;
      if (*cur == '-') {
	is_exponent_negative = 1;
	cur++;
      } else if (*cur == '+') {
        cur++;
      }
      while ((*cur >= '0') && (*cur <= '9')) {
	exponent = exponent * 10 + (*cur - '0');
	cur++;
      }
    }
    while (IS_BLANK_CH(*cur)) cur++;
    if (*cur != 0) return(xmlXPathNAN);
    if (isneg) ret = -ret;
    if (is_exponent_negative) exponent = -exponent;
    ret *= pow(10.0, (double)exponent);
    return(ret);
}

static void
xmlXPathCompNumber(xmlXPathParserContextPtr ctxt)
{
    double ret = 0.0;
    double mult = 1;
    int ok = 0;
    int exponent = 0;
    int is_exponent_negative = 0;
#ifdef __GNUC__
    unsigned long tmp = 0;
    double temp;
#endif

    CHECK_ERROR;
    if ((CUR != '.') && ((CUR < '0') || (CUR > '9'))) {
        XP_ERROR(XPATH_NUMBER_ERROR);
    }
#ifdef __GNUC__
    ret = 0;
    while ((CUR >= '0') && (CUR <= '9')) {
	ret = ret * 10;
	tmp = (CUR - '0');
        ok = 1;
        NEXT;
	temp = (double) tmp;
	ret = ret + temp;
    }
#else
    ret = 0;
    while ((CUR >= '0') && (CUR <= '9')) {
	ret = ret * 10 + (CUR - '0');
	ok = 1;
	NEXT;
    }
#endif
    if (CUR == '.') {
        NEXT;
        if (((CUR < '0') || (CUR > '9')) && (!ok)) {
            XP_ERROR(XPATH_NUMBER_ERROR);
        }
        while ((CUR >= '0') && (CUR <= '9')) {
            mult /= 10;
            ret = ret + (CUR - '0') * mult;
            NEXT;
        }
    }
    if ((CUR == 'e') || (CUR == 'E')) {
        NEXT;
        if (CUR == '-') {
            is_exponent_negative = 1;
            NEXT;
        } else if (CUR == '+') {
	    NEXT;
	}
        while ((CUR >= '0') && (CUR <= '9')) {
            exponent = exponent * 10 + (CUR - '0');
            NEXT;
        }
        if (is_exponent_negative)
            exponent = -exponent;
        ret *= pow(10.0, (double) exponent);
    }
    PUSH_LONG_EXPR(XPATH_OP_VALUE, XPATH_NUMBER, 0, 0,
                   xmlXPathCacheNewFloat(ctxt->context, ret), NULL);
}

static xmlChar *
xmlXPathParseLiteral(xmlXPathParserContextPtr ctxt) {
    const xmlChar *q;
    xmlChar *ret = NULL;

    if (CUR == '"') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '"'))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    XP_ERRORNULL(XPATH_UNFINISHED_LITERAL_ERROR);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else if (CUR == '\'') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '\''))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    XP_ERRORNULL(XPATH_UNFINISHED_LITERAL_ERROR);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else {
	XP_ERRORNULL(XPATH_START_LITERAL_ERROR);
    }
    return(ret);
}

static void
xmlXPathCompLiteral(xmlXPathParserContextPtr ctxt) {
    const xmlChar *q;
    xmlChar *ret = NULL;

    if (CUR == '"') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '"'))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    XP_ERROR(XPATH_UNFINISHED_LITERAL_ERROR);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else if (CUR == '\'') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '\''))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    XP_ERROR(XPATH_UNFINISHED_LITERAL_ERROR);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else {
	XP_ERROR(XPATH_START_LITERAL_ERROR);
    }
    if (ret == NULL) return;
    PUSH_LONG_EXPR(XPATH_OP_VALUE, XPATH_STRING, 0, 0,
	           xmlXPathCacheNewString(ctxt->context, ret), NULL);
    xmlFree(ret);
}

static void
xmlXPathCompVariableReference(xmlXPathParserContextPtr ctxt) {
    xmlChar *name;
    xmlChar *prefix;

    SKIP_BLANKS;
    if (CUR != '$') {
	XP_ERROR(XPATH_VARIABLE_REF_ERROR);
    }
    NEXT;
    name = xmlXPathParseQName(ctxt, &prefix);
    if (name == NULL) {
	XP_ERROR(XPATH_VARIABLE_REF_ERROR);
    }
    ctxt->comp->last = -1;
    PUSH_LONG_EXPR(XPATH_OP_VARIABLE, 0, 0, 0,
	           name, prefix);
    SKIP_BLANKS;
    if ((ctxt->context != NULL) && (ctxt->context->flags & XML_XPATH_NOVAR)) {
	XP_ERROR(XPATH_UNDEF_VARIABLE_ERROR);
    }
}

int
xmlXPathIsNodeType(const xmlChar *name) {
    if (name == NULL)
	return(0);

    if (xmlStrEqual(name, BAD_CAST "node"))
	return(1);
    if (xmlStrEqual(name, BAD_CAST "text"))
	return(1);
    if (xmlStrEqual(name, BAD_CAST "comment"))
	return(1);
    if (xmlStrEqual(name, BAD_CAST "processing-instruction"))
	return(1);
    return(0);
}

static void
xmlXPathCompFunctionCall(xmlXPathParserContextPtr ctxt) {
    xmlChar *name;
    xmlChar *prefix;
    int nbargs = 0;
    int sort = 1;

    name = xmlXPathParseQName(ctxt, &prefix);
    if (name == NULL) {
	xmlFree(prefix);
	XP_ERROR(XPATH_EXPR_ERROR);
    }
    SKIP_BLANKS;
#ifdef DEBUG_EXPR
    if (prefix == NULL)
	xmlGenericError(xmlGenericErrorContext, "Calling function %s\n",
			name);
    else
	xmlGenericError(xmlGenericErrorContext, "Calling function %s:%s\n",
			prefix, name);
#endif

    if (CUR != '(') {
	XP_ERROR(XPATH_EXPR_ERROR);
    }
    NEXT;
    SKIP_BLANKS;

    if ((prefix == NULL) && (name[0] == 'c') &&
	xmlStrEqual(name, BAD_CAST "count"))
    {
	sort = 0;
    }
    ctxt->comp->last = -1;
    if (CUR != ')') {
	while (CUR != 0) {
	    int op1 = ctxt->comp->last;
	    ctxt->comp->last = -1;
	    xmlXPathCompileExpr(ctxt, sort);
	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		xmlFree(name);
		xmlFree(prefix);
		return;
	    }
	    PUSH_BINARY_EXPR(XPATH_OP_ARG, op1, ctxt->comp->last, 0, 0);
	    nbargs++;
	    if (CUR == ')') break;
	    if (CUR != ',') {
		XP_ERROR(XPATH_EXPR_ERROR);
	    }
	    NEXT;
	    SKIP_BLANKS;
	}
    }
    PUSH_LONG_EXPR(XPATH_OP_FUNCTION, nbargs, 0, 0,
	           name, prefix);
    NEXT;
    SKIP_BLANKS;
}

static void
xmlXPathCompPrimaryExpr(xmlXPathParserContextPtr ctxt) {
    SKIP_BLANKS;
    if (CUR == '$') xmlXPathCompVariableReference(ctxt);
    else if (CUR == '(') {
	NEXT;
	SKIP_BLANKS;
	xmlXPathCompileExpr(ctxt, 1);
	CHECK_ERROR;
	if (CUR != ')') {
	    XP_ERROR(XPATH_EXPR_ERROR);
	}
	NEXT;
	SKIP_BLANKS;
    } else if (IS_ASCII_DIGIT(CUR) || (CUR == '.' && IS_ASCII_DIGIT(NXT(1)))) {
	xmlXPathCompNumber(ctxt);
    } else if ((CUR == '\'') || (CUR == '"')) {
	xmlXPathCompLiteral(ctxt);
    } else {
	xmlXPathCompFunctionCall(ctxt);
    }
    SKIP_BLANKS;
}


static void
xmlXPathCompFilterExpr(xmlXPathParserContextPtr ctxt) {
    xmlXPathCompPrimaryExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;

    while (CUR == '[') {
	xmlXPathCompPredicate(ctxt, 1);
	SKIP_BLANKS;
    }


}


static xmlChar *
xmlXPathScanName(xmlXPathParserContextPtr ctxt) {
    int len = 0, l;
    int c;
    const xmlChar *cur;
    xmlChar *ret;

    cur = ctxt->cur;

    c = CUR_CHAR(l);
    if ((c == ' ') || (c == '>') || (c == '/') || 
	(!IS_LETTER(c) && (c != '_') &&
         (c != ':'))) {
	return(NULL);
    }

    while ((c != ' ') && (c != '>') && (c != '/') && 
	   ((IS_LETTER(c)) || (IS_DIGIT(c)) ||
            (c == '.') || (c == '-') ||
	    (c == '_') || (c == ':') ||
	    (IS_COMBINING(c)) ||
	    (IS_EXTENDER(c)))) {
	len += l;
	NEXTL(l);
	c = CUR_CHAR(l);
    }
    ret = xmlStrndup(cur, ctxt->cur - cur);
    ctxt->cur = cur;
    return(ret);
}


static void
xmlXPathCompPathExpr(xmlXPathParserContextPtr ctxt) {
    int lc = 1;           
    xmlChar *name = NULL; 

    SKIP_BLANKS;
    if ((CUR == '$') || (CUR == '(') ||
	(IS_ASCII_DIGIT(CUR)) ||
        (CUR == '\'') || (CUR == '"') ||
	(CUR == '.' && IS_ASCII_DIGIT(NXT(1)))) {
	lc = 0;
    } else if (CUR == '*') {
	
	lc = 1;
    } else if (CUR == '/') {
	
	lc = 1;
    } else if (CUR == '@') {
	
	lc = 1;
    } else if (CUR == '.') {
	
	lc = 1;
    } else {
	SKIP_BLANKS;
	name = xmlXPathScanName(ctxt);
	if ((name != NULL) && (xmlStrstr(name, (xmlChar *) "::") != NULL)) {
#ifdef DEBUG_STEP
	    xmlGenericError(xmlGenericErrorContext,
		    "PathExpr: Axis\n");
#endif
	    lc = 1;
	    xmlFree(name);
	} else if (name != NULL) {
	    int len =xmlStrlen(name);


	    while (NXT(len) != 0) {
		if (NXT(len) == '/') {
		    
#ifdef DEBUG_STEP
		    xmlGenericError(xmlGenericErrorContext,
			    "PathExpr: AbbrRelLocation\n");
#endif
		    lc = 1;
		    break;
		} else if (IS_BLANK_CH(NXT(len))) {
		    
		    ;
		} else if (NXT(len) == ':') {
#ifdef DEBUG_STEP
		    xmlGenericError(xmlGenericErrorContext,
			    "PathExpr: AbbrRelLocation\n");
#endif
		    lc = 1;
		    break;
		} else if ((NXT(len) == '(')) {
		    
		    if (xmlXPathIsNodeType(name)) {
#ifdef DEBUG_STEP
		        xmlGenericError(xmlGenericErrorContext,
				"PathExpr: Type search\n");
#endif
			lc = 1;
		    } else {
#ifdef DEBUG_STEP
		        xmlGenericError(xmlGenericErrorContext,
				"PathExpr: function call\n");
#endif
			lc = 0;
		    }
                    break;
		} else if ((NXT(len) == '[')) {
		    
#ifdef DEBUG_STEP
		    xmlGenericError(xmlGenericErrorContext,
			    "PathExpr: AbbrRelLocation\n");
#endif
		    lc = 1;
		    break;
		} else if ((NXT(len) == '<') || (NXT(len) == '>') ||
			   (NXT(len) == '=')) {
		    lc = 1;
		    break;
		} else {
		    lc = 1;
		    break;
		}
		len++;
	    }
	    if (NXT(len) == 0) {
#ifdef DEBUG_STEP
		xmlGenericError(xmlGenericErrorContext,
			"PathExpr: AbbrRelLocation\n");
#endif
		
		lc = 1;
	    }
	    xmlFree(name);
	} else {
	    
	    XP_ERROR(XPATH_EXPR_ERROR);
	}
    }

    if (lc) {
	if (CUR == '/') {
	    PUSH_LEAVE_EXPR(XPATH_OP_ROOT, 0, 0);
	} else {
	    PUSH_LEAVE_EXPR(XPATH_OP_NODE, 0, 0);
	}
	xmlXPathCompLocationPath(ctxt);
    } else {
	xmlXPathCompFilterExpr(ctxt);
	CHECK_ERROR;
	if ((CUR == '/') && (NXT(1) == '/')) {
	    SKIP(2);
	    SKIP_BLANKS;

	    PUSH_LONG_EXPR(XPATH_OP_COLLECT, AXIS_DESCENDANT_OR_SELF,
		    NODE_TEST_TYPE, NODE_TYPE_NODE, NULL, NULL);
	    PUSH_UNARY_EXPR(XPATH_OP_RESET, ctxt->comp->last, 1, 0);

	    xmlXPathCompRelativeLocationPath(ctxt);
	} else if (CUR == '/') {
	    xmlXPathCompRelativeLocationPath(ctxt);
	}
    }
    SKIP_BLANKS;
}


static void
xmlXPathCompUnionExpr(xmlXPathParserContextPtr ctxt) {
    xmlXPathCompPathExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while (CUR == '|') {
	int op1 = ctxt->comp->last;
	PUSH_LEAVE_EXPR(XPATH_OP_NODE, 0, 0);

	NEXT;
	SKIP_BLANKS;
	xmlXPathCompPathExpr(ctxt);

	PUSH_BINARY_EXPR(XPATH_OP_UNION, op1, ctxt->comp->last, 0, 0);

	SKIP_BLANKS;
    }
}


static void
xmlXPathCompUnaryExpr(xmlXPathParserContextPtr ctxt) {
    int minus = 0;
    int found = 0;

    SKIP_BLANKS;
    while (CUR == '-') {
        minus = 1 - minus;
	found = 1;
	NEXT;
	SKIP_BLANKS;
    }

    xmlXPathCompUnionExpr(ctxt);
    CHECK_ERROR;
    if (found) {
	if (minus)
	    PUSH_UNARY_EXPR(XPATH_OP_PLUS, ctxt->comp->last, 2, 0);
	else
	    PUSH_UNARY_EXPR(XPATH_OP_PLUS, ctxt->comp->last, 3, 0);
    }
}


static void
xmlXPathCompMultiplicativeExpr(xmlXPathParserContextPtr ctxt) {
    xmlXPathCompUnaryExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == '*') ||
           ((CUR == 'd') && (NXT(1) == 'i') && (NXT(2) == 'v')) ||
           ((CUR == 'm') && (NXT(1) == 'o') && (NXT(2) == 'd'))) {
	int op = -1;
	int op1 = ctxt->comp->last;

        if (CUR == '*') {
	    op = 0;
	    NEXT;
	} else if (CUR == 'd') {
	    op = 1;
	    SKIP(3);
	} else if (CUR == 'm') {
	    op = 2;
	    SKIP(3);
	}
	SKIP_BLANKS;
        xmlXPathCompUnaryExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_MULT, op1, ctxt->comp->last, op, 0);
	SKIP_BLANKS;
    }
}


static void
xmlXPathCompAdditiveExpr(xmlXPathParserContextPtr ctxt) {

    xmlXPathCompMultiplicativeExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == '+') || (CUR == '-')) {
	int plus;
	int op1 = ctxt->comp->last;

        if (CUR == '+') plus = 1;
	else plus = 0;
	NEXT;
	SKIP_BLANKS;
        xmlXPathCompMultiplicativeExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_PLUS, op1, ctxt->comp->last, plus, 0);
	SKIP_BLANKS;
    }
}


static void
xmlXPathCompRelationalExpr(xmlXPathParserContextPtr ctxt) {
    xmlXPathCompAdditiveExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == '<') ||
           (CUR == '>') ||
           ((CUR == '<') && (NXT(1) == '=')) ||
           ((CUR == '>') && (NXT(1) == '='))) {
	int inf, strict;
	int op1 = ctxt->comp->last;

        if (CUR == '<') inf = 1;
	else inf = 0;
	if (NXT(1) == '=') strict = 0;
	else strict = 1;
	NEXT;
	if (!strict) NEXT;
	SKIP_BLANKS;
        xmlXPathCompAdditiveExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_CMP, op1, ctxt->comp->last, inf, strict);
	SKIP_BLANKS;
    }
}

static void
xmlXPathCompEqualityExpr(xmlXPathParserContextPtr ctxt) {
    xmlXPathCompRelationalExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == '=') || ((CUR == '!') && (NXT(1) == '='))) {
	int eq;
	int op1 = ctxt->comp->last;

        if (CUR == '=') eq = 1;
	else eq = 0;
	NEXT;
	if (!eq) NEXT;
	SKIP_BLANKS;
        xmlXPathCompRelationalExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_EQUAL, op1, ctxt->comp->last, eq, 0);
	SKIP_BLANKS;
    }
}

static void
xmlXPathCompAndExpr(xmlXPathParserContextPtr ctxt) {
    xmlXPathCompEqualityExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == 'a') && (NXT(1) == 'n') && (NXT(2) == 'd')) {
	int op1 = ctxt->comp->last;
        SKIP(3);
	SKIP_BLANKS;
        xmlXPathCompEqualityExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_AND, op1, ctxt->comp->last, 0, 0);
	SKIP_BLANKS;
    }
}

static void
xmlXPathCompileExpr(xmlXPathParserContextPtr ctxt, int sort) {
    xmlXPathCompAndExpr(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while ((CUR == 'o') && (NXT(1) == 'r')) {
	int op1 = ctxt->comp->last;
        SKIP(2);
	SKIP_BLANKS;
        xmlXPathCompAndExpr(ctxt);
	CHECK_ERROR;
	PUSH_BINARY_EXPR(XPATH_OP_OR, op1, ctxt->comp->last, 0, 0);
	SKIP_BLANKS;
    }
    if ((sort) && (ctxt->comp->steps[ctxt->comp->last].op != XPATH_OP_VALUE)) {
	
	PUSH_UNARY_EXPR(XPATH_OP_SORT, ctxt->comp->last , 0, 0);
    }
}

static void
xmlXPathCompPredicate(xmlXPathParserContextPtr ctxt, int filter) {
    int op1 = ctxt->comp->last;

    SKIP_BLANKS;
    if (CUR != '[') {
	XP_ERROR(XPATH_INVALID_PREDICATE_ERROR);
    }
    NEXT;
    SKIP_BLANKS;

    ctxt->comp->last = -1;
    if (! filter)
	xmlXPathCompileExpr(ctxt, 0);
    else
	xmlXPathCompileExpr(ctxt, 1);
    CHECK_ERROR;

    if (CUR != ']') {
	XP_ERROR(XPATH_INVALID_PREDICATE_ERROR);
    }

    if (filter)
	PUSH_BINARY_EXPR(XPATH_OP_FILTER, op1, ctxt->comp->last, 0, 0);
    else
	PUSH_BINARY_EXPR(XPATH_OP_PREDICATE, op1, ctxt->comp->last, 0, 0);

    NEXT;
    SKIP_BLANKS;
}

static xmlChar *
xmlXPathCompNodeTest(xmlXPathParserContextPtr ctxt, xmlXPathTestVal *test,
	             xmlXPathTypeVal *type, const xmlChar **prefix,
		     xmlChar *name) {
    int blanks;

    if ((test == NULL) || (type == NULL) || (prefix == NULL)) {
	STRANGE;
	return(NULL);
    }
    *type = (xmlXPathTypeVal) 0;
    *test = (xmlXPathTestVal) 0;
    *prefix = NULL;
    SKIP_BLANKS;

    if ((name == NULL) && (CUR == '*')) {
	NEXT;
	*test = NODE_TEST_ALL;
	return(NULL);
    }

    if (name == NULL)
	name = xmlXPathParseNCName(ctxt);
    if (name == NULL) {
	XP_ERRORNULL(XPATH_EXPR_ERROR);
    }

    blanks = IS_BLANK_CH(CUR);
    SKIP_BLANKS;
    if (CUR == '(') {
	NEXT;
	if (xmlStrEqual(name, BAD_CAST "comment"))
	    *type = NODE_TYPE_COMMENT;
	else if (xmlStrEqual(name, BAD_CAST "node"))
	    *type = NODE_TYPE_NODE;
	else if (xmlStrEqual(name, BAD_CAST "processing-instruction"))
	    *type = NODE_TYPE_PI;
	else if (xmlStrEqual(name, BAD_CAST "text"))
	    *type = NODE_TYPE_TEXT;
	else {
	    if (name != NULL)
		xmlFree(name);
	    XP_ERRORNULL(XPATH_EXPR_ERROR);
	}

	*test = NODE_TEST_TYPE;

	SKIP_BLANKS;
	if (*type == NODE_TYPE_PI) {
	    if (name != NULL)
		xmlFree(name);
	    name = NULL;
	    if (CUR != ')') {
		name = xmlXPathParseLiteral(ctxt);
		CHECK_ERROR NULL;
		*test = NODE_TEST_PI;
		SKIP_BLANKS;
	    }
	}
	if (CUR != ')') {
	    if (name != NULL)
		xmlFree(name);
	    XP_ERRORNULL(XPATH_UNCLOSED_ERROR);
	}
	NEXT;
	return(name);
    }
    *test = NODE_TEST_NAME;
    if ((!blanks) && (CUR == ':')) {
	NEXT;

#if 0
	*prefix = xmlXPathNsLookup(ctxt->context, name);
	if (name != NULL)
	    xmlFree(name);
	if (*prefix == NULL) {
	    XP_ERROR0(XPATH_UNDEF_PREFIX_ERROR);
	}
#else
	*prefix = name;
#endif

	if (CUR == '*') {
	    NEXT;
	    *test = NODE_TEST_ALL;
	    return(NULL);
	}

	name = xmlXPathParseNCName(ctxt);
	if (name == NULL) {
	    XP_ERRORNULL(XPATH_EXPR_ERROR);
	}
    }
    return(name);
}

static xmlXPathAxisVal
xmlXPathIsAxisName(const xmlChar *name) {
    xmlXPathAxisVal ret = (xmlXPathAxisVal) 0;
    switch (name[0]) {
	case 'a':
	    if (xmlStrEqual(name, BAD_CAST "ancestor"))
		ret = AXIS_ANCESTOR;
	    if (xmlStrEqual(name, BAD_CAST "ancestor-or-self"))
		ret = AXIS_ANCESTOR_OR_SELF;
	    if (xmlStrEqual(name, BAD_CAST "attribute"))
		ret = AXIS_ATTRIBUTE;
	    break;
	case 'c':
	    if (xmlStrEqual(name, BAD_CAST "child"))
		ret = AXIS_CHILD;
	    break;
	case 'd':
	    if (xmlStrEqual(name, BAD_CAST "descendant"))
		ret = AXIS_DESCENDANT;
	    if (xmlStrEqual(name, BAD_CAST "descendant-or-self"))
		ret = AXIS_DESCENDANT_OR_SELF;
	    break;
	case 'f':
	    if (xmlStrEqual(name, BAD_CAST "following"))
		ret = AXIS_FOLLOWING;
	    if (xmlStrEqual(name, BAD_CAST "following-sibling"))
		ret = AXIS_FOLLOWING_SIBLING;
	    break;
	case 'n':
	    if (xmlStrEqual(name, BAD_CAST "namespace"))
		ret = AXIS_NAMESPACE;
	    break;
	case 'p':
	    if (xmlStrEqual(name, BAD_CAST "parent"))
		ret = AXIS_PARENT;
	    if (xmlStrEqual(name, BAD_CAST "preceding"))
		ret = AXIS_PRECEDING;
	    if (xmlStrEqual(name, BAD_CAST "preceding-sibling"))
		ret = AXIS_PRECEDING_SIBLING;
	    break;
	case 's':
	    if (xmlStrEqual(name, BAD_CAST "self"))
		ret = AXIS_SELF;
	    break;
    }
    return(ret);
}

static void
xmlXPathCompStep(xmlXPathParserContextPtr ctxt) {
#ifdef LIBXML_XPTR_ENABLED
    int rangeto = 0;
    int op2 = -1;
#endif

    SKIP_BLANKS;
    if ((CUR == '.') && (NXT(1) == '.')) {
	SKIP(2);
	SKIP_BLANKS;
	PUSH_LONG_EXPR(XPATH_OP_COLLECT, AXIS_PARENT,
		    NODE_TEST_TYPE, NODE_TYPE_NODE, NULL, NULL);
    } else if (CUR == '.') {
	NEXT;
	SKIP_BLANKS;
    } else {
	xmlChar *name = NULL;
	const xmlChar *prefix = NULL;
	xmlXPathTestVal test = (xmlXPathTestVal) 0;
	xmlXPathAxisVal axis = (xmlXPathAxisVal) 0;
	xmlXPathTypeVal type = (xmlXPathTypeVal) 0;
	int op1;

#ifdef LIBXML_XPTR_ENABLED
	if (ctxt->xptr) {
	    name = xmlXPathParseNCName(ctxt);
	    if ((name != NULL) && (xmlStrEqual(name, BAD_CAST "range-to"))) {
                op2 = ctxt->comp->last;
		xmlFree(name);
		SKIP_BLANKS;
		if (CUR != '(') {
		    XP_ERROR(XPATH_EXPR_ERROR);
		}
		NEXT;
		SKIP_BLANKS;

		xmlXPathCompileExpr(ctxt, 1);
		
		CHECK_ERROR;

		SKIP_BLANKS;
		if (CUR != ')') {
		    XP_ERROR(XPATH_EXPR_ERROR);
		}
		NEXT;
		rangeto = 1;
		goto eval_predicates;
	    }
	}
#endif
	if (CUR == '*') {
	    axis = AXIS_CHILD;
	} else {
	    if (name == NULL)
		name = xmlXPathParseNCName(ctxt);
	    if (name != NULL) {
		axis = xmlXPathIsAxisName(name);
		if (axis != 0) {
		    SKIP_BLANKS;
		    if ((CUR == ':') && (NXT(1) == ':')) {
			SKIP(2);
			xmlFree(name);
			name = NULL;
		    } else {
			
			axis = AXIS_CHILD;
		    }
		} else {
		    axis = AXIS_CHILD;
		}
	    } else if (CUR == '@') {
		NEXT;
		axis = AXIS_ATTRIBUTE;
	    } else {
		axis = AXIS_CHILD;
	    }
	}

	CHECK_ERROR;

	name = xmlXPathCompNodeTest(ctxt, &test, &type, &prefix, name);
	if (test == 0)
	    return;

        if ((prefix != NULL) && (ctxt->context != NULL) &&
	    (ctxt->context->flags & XML_XPATH_CHECKNS)) {
	    if (xmlXPathNsLookup(ctxt->context, prefix) == NULL) {
		xmlXPathErr(ctxt, XPATH_UNDEF_PREFIX_ERROR);
	    }
	}
#ifdef DEBUG_STEP
	xmlGenericError(xmlGenericErrorContext,
		"Basis : computing new set\n");
#endif

#ifdef DEBUG_STEP
	xmlGenericError(xmlGenericErrorContext, "Basis : ");
	if (ctxt->value == NULL)
	    xmlGenericError(xmlGenericErrorContext, "no value\n");
	else if (ctxt->value->nodesetval == NULL)
	    xmlGenericError(xmlGenericErrorContext, "Empty\n");
	else
	    xmlGenericErrorContextNodeSet(stdout, ctxt->value->nodesetval);
#endif

#ifdef LIBXML_XPTR_ENABLED
eval_predicates:
#endif
	op1 = ctxt->comp->last;
	ctxt->comp->last = -1;

	SKIP_BLANKS;
	while (CUR == '[') {
	    xmlXPathCompPredicate(ctxt, 0);
	}

#ifdef LIBXML_XPTR_ENABLED
	if (rangeto) {
	    PUSH_BINARY_EXPR(XPATH_OP_RANGETO, op2, op1, 0, 0);
	} else
#endif
	    PUSH_FULL_EXPR(XPATH_OP_COLLECT, op1, ctxt->comp->last, axis,
			   test, type, (void *)prefix, (void *)name);

    }
#ifdef DEBUG_STEP
    xmlGenericError(xmlGenericErrorContext, "Step : ");
    if (ctxt->value == NULL)
	xmlGenericError(xmlGenericErrorContext, "no value\n");
    else if (ctxt->value->nodesetval == NULL)
	xmlGenericError(xmlGenericErrorContext, "Empty\n");
    else
	xmlGenericErrorContextNodeSet(xmlGenericErrorContext,
		ctxt->value->nodesetval);
#endif
}

static void
xmlXPathCompRelativeLocationPath
(xmlXPathParserContextPtr ctxt) {
    SKIP_BLANKS;
    if ((CUR == '/') && (NXT(1) == '/')) {
	SKIP(2);
	SKIP_BLANKS;
	PUSH_LONG_EXPR(XPATH_OP_COLLECT, AXIS_DESCENDANT_OR_SELF,
		         NODE_TEST_TYPE, NODE_TYPE_NODE, NULL, NULL);
    } else if (CUR == '/') {
	    NEXT;
	SKIP_BLANKS;
    }
    xmlXPathCompStep(ctxt);
    CHECK_ERROR;
    SKIP_BLANKS;
    while (CUR == '/') {
	if ((CUR == '/') && (NXT(1) == '/')) {
	    SKIP(2);
	    SKIP_BLANKS;
	    PUSH_LONG_EXPR(XPATH_OP_COLLECT, AXIS_DESCENDANT_OR_SELF,
			     NODE_TEST_TYPE, NODE_TYPE_NODE, NULL, NULL);
	    xmlXPathCompStep(ctxt);
	} else if (CUR == '/') {
	    NEXT;
	    SKIP_BLANKS;
	    xmlXPathCompStep(ctxt);
	}
	SKIP_BLANKS;
    }
}

static void
xmlXPathCompLocationPath(xmlXPathParserContextPtr ctxt) {
    SKIP_BLANKS;
    if (CUR != '/') {
        xmlXPathCompRelativeLocationPath(ctxt);
    } else {
	while (CUR == '/') {
	    if ((CUR == '/') && (NXT(1) == '/')) {
		SKIP(2);
		SKIP_BLANKS;
		PUSH_LONG_EXPR(XPATH_OP_COLLECT, AXIS_DESCENDANT_OR_SELF,
			     NODE_TEST_TYPE, NODE_TYPE_NODE, NULL, NULL);
		xmlXPathCompRelativeLocationPath(ctxt);
	    } else if (CUR == '/') {
		NEXT;
		SKIP_BLANKS;
		if ((CUR != 0 ) &&
		    ((IS_ASCII_LETTER(CUR)) || (CUR == '_') || (CUR == '.') ||
		     (CUR == '@') || (CUR == '*')))
		    xmlXPathCompRelativeLocationPath(ctxt);
	    }
	    CHECK_ERROR;
	}
    }
}


static int
xmlXPathCompOpEval(xmlXPathParserContextPtr ctxt, xmlXPathStepOpPtr op);

#ifdef DEBUG_STEP
static void
xmlXPathDebugDumpStepAxis(xmlXPathStepOpPtr op,
			  int nbNodes)
{
    xmlGenericError(xmlGenericErrorContext, "new step : ");
    switch (op->value) {
        case AXIS_ANCESTOR:
            xmlGenericError(xmlGenericErrorContext, "axis 'ancestors' ");
            break;
        case AXIS_ANCESTOR_OR_SELF:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'ancestors-or-self' ");
            break;
        case AXIS_ATTRIBUTE:
            xmlGenericError(xmlGenericErrorContext, "axis 'attributes' ");
            break;
        case AXIS_CHILD:
            xmlGenericError(xmlGenericErrorContext, "axis 'child' ");
            break;
        case AXIS_DESCENDANT:
            xmlGenericError(xmlGenericErrorContext, "axis 'descendant' ");
            break;
        case AXIS_DESCENDANT_OR_SELF:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'descendant-or-self' ");
            break;
        case AXIS_FOLLOWING:
            xmlGenericError(xmlGenericErrorContext, "axis 'following' ");
            break;
        case AXIS_FOLLOWING_SIBLING:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'following-siblings' ");
            break;
        case AXIS_NAMESPACE:
            xmlGenericError(xmlGenericErrorContext, "axis 'namespace' ");
            break;
        case AXIS_PARENT:
            xmlGenericError(xmlGenericErrorContext, "axis 'parent' ");
            break;
        case AXIS_PRECEDING:
            xmlGenericError(xmlGenericErrorContext, "axis 'preceding' ");
            break;
        case AXIS_PRECEDING_SIBLING:
            xmlGenericError(xmlGenericErrorContext,
                            "axis 'preceding-sibling' ");
            break;
        case AXIS_SELF:
            xmlGenericError(xmlGenericErrorContext, "axis 'self' ");
            break;
    }
    xmlGenericError(xmlGenericErrorContext,
	" context contains %d nodes\n", nbNodes);
    switch (op->value2) {
        case NODE_TEST_NONE:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for none !!!\n");
            break;
        case NODE_TEST_TYPE:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for type %d\n", op->value3);
            break;
        case NODE_TEST_PI:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for PI !!!\n");
            break;
        case NODE_TEST_ALL:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for *\n");
            break;
        case NODE_TEST_NS:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for namespace %s\n",
                            op->value5);
            break;
        case NODE_TEST_NAME:
            xmlGenericError(xmlGenericErrorContext,
                            "           searching for name %s\n", op->value5);
            if (op->value4)
                xmlGenericError(xmlGenericErrorContext,
                                "           with namespace %s\n", op->value4);
            break;
    }
    xmlGenericError(xmlGenericErrorContext, "Testing : ");
}
#endif 

static int
xmlXPathCompOpEvalPredicate(xmlXPathParserContextPtr ctxt,
			    xmlXPathStepOpPtr op,
			    xmlNodeSetPtr set,
			    int contextSize,
			    int hasNsNodes)
{
    if (op->ch1 != -1) {
	xmlXPathCompExprPtr comp = ctxt->comp;
	if (comp->steps[op->ch1].op != XPATH_OP_PREDICATE) {
	}
	contextSize = xmlXPathCompOpEvalPredicate(ctxt,
	    &comp->steps[op->ch1], set, contextSize, hasNsNodes);
	CHECK_ERROR0;
	if (contextSize <= 0)
	    return(0);
    }
    if (op->ch2 != -1) {
	xmlXPathContextPtr xpctxt = ctxt->context;
	xmlNodePtr contextNode, oldContextNode;
	xmlDocPtr oldContextDoc;
	int i, res, contextPos = 0, newContextSize;
	xmlXPathStepOpPtr exprOp;
	xmlXPathObjectPtr contextObj = NULL, exprRes = NULL;

#ifdef LIBXML_XPTR_ENABLED
#endif
	oldContextNode = xpctxt->node;
	oldContextDoc = xpctxt->doc;
	exprOp = &ctxt->comp->steps[op->ch2];
	newContextSize = 0;
	for (i = 0; i < set->nodeNr; i++) {
	    if (set->nodeTab[i] == NULL)
		continue;

	    contextNode = set->nodeTab[i];
	    xpctxt->node = contextNode;
	    xpctxt->contextSize = contextSize;
	    xpctxt->proximityPosition = ++contextPos;

	    if ((contextNode->type != XML_NAMESPACE_DECL) &&
		(contextNode->doc != NULL))
		xpctxt->doc = contextNode->doc;
	    if (contextObj == NULL)
		contextObj = xmlXPathCacheNewNodeSet(xpctxt, contextNode);
	    else
		xmlXPathNodeSetAddUnique(contextObj->nodesetval,
		    contextNode);

	    valuePush(ctxt, contextObj);

	    res = xmlXPathCompOpEvalToBoolean(ctxt, exprOp, 1);

	    if ((ctxt->error != XPATH_EXPRESSION_OK) || (res == -1)) {
		xmlXPathNodeSetClear(set, hasNsNodes);
		newContextSize = 0;
		goto evaluation_exit;
	    }

	    if (res != 0) {
		newContextSize++;
	    } else {
		set->nodeTab[i] = NULL;
		if (contextNode->type == XML_NAMESPACE_DECL)
		    xmlXPathNodeSetFreeNs((xmlNsPtr) contextNode);
	    }
	    if (ctxt->value == contextObj) {
		valuePop(ctxt);
		xmlXPathNodeSetClear(contextObj->nodesetval, hasNsNodes);
	    } else {
		contextObj = NULL;
	    }
	}

	if (contextObj != NULL) {
	    if (ctxt->value == contextObj)
		valuePop(ctxt);
	    xmlXPathReleaseObject(xpctxt, contextObj);
	}
evaluation_exit:
	if (exprRes != NULL)
	    xmlXPathReleaseObject(ctxt->context, exprRes);
	xpctxt->node = oldContextNode;
	xpctxt->doc = oldContextDoc;
	xpctxt->contextSize = -1;
	xpctxt->proximityPosition = -1;
	return(newContextSize);
    }
    return(contextSize);
}

static int
xmlXPathCompOpEvalPositionalPredicate(xmlXPathParserContextPtr ctxt,
				      xmlXPathStepOpPtr op,
				      xmlNodeSetPtr set,
				      int contextSize,
				      int minPos,
				      int maxPos,
				      int hasNsNodes)
{
    if (op->ch1 != -1) {
	xmlXPathCompExprPtr comp = ctxt->comp;
	if (comp->steps[op->ch1].op != XPATH_OP_PREDICATE) {
	}
	contextSize = xmlXPathCompOpEvalPredicate(ctxt,
	    &comp->steps[op->ch1], set, contextSize, hasNsNodes);
	CHECK_ERROR0;
	if (contextSize <= 0)
	    return(0);
    }
    if (contextSize < minPos) {
	xmlXPathNodeSetClear(set, hasNsNodes);
	return(0);
    }
    if (op->ch2 == -1) {
	return (contextSize);
    } else {
	xmlDocPtr oldContextDoc;
	int i, pos = 0, newContextSize = 0, contextPos = 0, res;
	xmlXPathStepOpPtr exprOp;
	xmlXPathObjectPtr contextObj = NULL, exprRes = NULL;
	xmlNodePtr oldContextNode, contextNode = NULL;
	xmlXPathContextPtr xpctxt = ctxt->context;

#ifdef LIBXML_XPTR_ENABLED
#endif 

	oldContextNode = xpctxt->node;
	oldContextDoc = xpctxt->doc;
	exprOp = &ctxt->comp->steps[op->ch2];
	for (i = 0; i < set->nodeNr; i++) {
	    if (set->nodeTab[i] == NULL)
		continue;

	    contextNode = set->nodeTab[i];
	    xpctxt->node = contextNode;
	    xpctxt->contextSize = contextSize;
	    xpctxt->proximityPosition = ++contextPos;

	    if ((contextNode->type != XML_NAMESPACE_DECL) &&
		(contextNode->doc != NULL))
		xpctxt->doc = contextNode->doc;
	    if (contextObj == NULL)
		contextObj = xmlXPathCacheNewNodeSet(xpctxt, contextNode);
	    else
		xmlXPathNodeSetAddUnique(contextObj->nodesetval,
		    contextNode);

	    valuePush(ctxt, contextObj);
	    res = xmlXPathCompOpEvalToBoolean(ctxt, exprOp, 1);

	    if ((ctxt->error != XPATH_EXPRESSION_OK) || (res == -1)) {
	        xmlXPathObjectPtr tmp;
		
		tmp = valuePop(ctxt);
                while (tmp != contextObj) {
                    xmlXPathReleaseObject(xpctxt, tmp);
                    tmp = valuePop(ctxt);
                }
		goto evaluation_error;
	    }

	    if (res)
		pos++;

	    if (res && (pos >= minPos) && (pos <= maxPos)) {
		newContextSize++;
		if (minPos == maxPos) {
		    if (contextNode->type == XML_NAMESPACE_DECL) {
			set->nodeTab[i] = NULL;
		    }
		    xmlXPathNodeSetClear(set, hasNsNodes);
		    set->nodeNr = 1;
		    set->nodeTab[0] = contextNode;
		    goto evaluation_exit;
		}
		if (pos == maxPos) {
		    xmlXPathNodeSetClearFromPos(set, i +1, hasNsNodes);
		    goto evaluation_exit;
		}
	    } else {
		set->nodeTab[i] = NULL;
		if (contextNode->type == XML_NAMESPACE_DECL)
		    xmlXPathNodeSetFreeNs((xmlNsPtr) contextNode);
	    }
	    if (exprRes != NULL) {
		xmlXPathReleaseObject(ctxt->context, exprRes);
		exprRes = NULL;
	    }
	    if (ctxt->value == contextObj) {
		valuePop(ctxt);
		xmlXPathNodeSetClear(contextObj->nodesetval, hasNsNodes);
	    } else {
		contextObj = NULL;
	    }
	}
	goto evaluation_exit;

evaluation_error:
	xmlXPathNodeSetClear(set, hasNsNodes);
	newContextSize = 0;

evaluation_exit:
	if (contextObj != NULL) {
	    if (ctxt->value == contextObj)
		valuePop(ctxt);
	    xmlXPathReleaseObject(xpctxt, contextObj);
	}
	if (exprRes != NULL)
	    xmlXPathReleaseObject(ctxt->context, exprRes);
	xpctxt->node = oldContextNode;
	xpctxt->doc = oldContextDoc;
	xpctxt->contextSize = -1;
	xpctxt->proximityPosition = -1;
	return(newContextSize);
    }
    return(contextSize);
}

static int
xmlXPathIsPositionalPredicate(xmlXPathParserContextPtr ctxt,
			    xmlXPathStepOpPtr op,
			    int *maxPos)
{

    xmlXPathStepOpPtr exprOp;


    if ((op->op != XPATH_OP_PREDICATE) && (op->op != XPATH_OP_FILTER))
	return(0);

    if (op->ch2 != -1) {
	exprOp = &ctxt->comp->steps[op->ch2];
    } else
	return(0);

    if ((exprOp != NULL) &&
	(exprOp->op == XPATH_OP_VALUE) &&
	(exprOp->value4 != NULL) &&
	(((xmlXPathObjectPtr) exprOp->value4)->type == XPATH_NUMBER))
    {
	*maxPos = (int) ((xmlXPathObjectPtr) exprOp->value4)->floatval;

	if (((xmlXPathObjectPtr) exprOp->value4)->floatval ==
	    (float) *maxPos)
	{
	    return(1);
	}
    }
    return(0);
}

static int
xmlXPathNodeCollectAndTest(xmlXPathParserContextPtr ctxt,
                           xmlXPathStepOpPtr op,
			   xmlNodePtr * first, xmlNodePtr * last,
			   int toBool)
{

#define XP_TEST_HIT \
    if (hasAxisRange != 0) { \
	if (++pos == maxPos) { \
	    addNode(seq, cur); \
	goto axis_range_end; } \
    } else { \
	addNode(seq, cur); \
	if (breakOnFirstHit) goto first_hit; }

#define XP_TEST_HIT_NS \
    if (hasAxisRange != 0) { \
	if (++pos == maxPos) { \
	    hasNsNodes = 1; \
	    xmlXPathNodeSetAddNs(seq, xpctxt->node, (xmlNsPtr) cur); \
	goto axis_range_end; } \
    } else { \
	hasNsNodes = 1; \
	xmlXPathNodeSetAddNs(seq, \
	xpctxt->node, (xmlNsPtr) cur); \
	if (breakOnFirstHit) goto first_hit; }

    xmlXPathAxisVal axis = (xmlXPathAxisVal) op->value;
    xmlXPathTestVal test = (xmlXPathTestVal) op->value2;
    xmlXPathTypeVal type = (xmlXPathTypeVal) op->value3;
    const xmlChar *prefix = op->value4;
    const xmlChar *name = op->value5;
    const xmlChar *URI = NULL;

#ifdef DEBUG_STEP
    int nbMatches = 0, prevMatches = 0;
#endif
    int total = 0, hasNsNodes = 0;
    
    xmlXPathObjectPtr obj;
    
    xmlNodeSetPtr contextSeq;
    int contextIdx;
    xmlNodePtr contextNode;
    
    xmlNodePtr outerContextNode;
    
    xmlNodeSetPtr outSeq;
    xmlNodeSetPtr seq;
    xmlNodePtr cur;
    
    xmlXPathStepOpPtr predOp;
    int maxPos; 
    int hasPredicateRange, hasAxisRange, pos, size, newSize;
    int breakOnFirstHit;

    xmlXPathTraversalFunction next = NULL;
    
    xmlXPathTraversalFunctionExt outerNext = NULL;
    void (*addNode) (xmlNodeSetPtr, xmlNodePtr);
    xmlXPathNodeSetMergeFunction mergeAndClear;
    xmlNodePtr oldContextNode;
    xmlXPathContextPtr xpctxt = ctxt->context;


    CHECK_TYPE0(XPATH_NODESET);
    obj = valuePop(ctxt);
    if (prefix != NULL) {
        URI = xmlXPathNsLookup(xpctxt, prefix);
        if (URI == NULL) {
	    xmlXPathReleaseObject(xpctxt, obj);
            XP_ERROR0(XPATH_UNDEF_PREFIX_ERROR);
	}
    }
    mergeAndClear = xmlXPathNodeSetMergeAndClear;
    switch (axis) {
        case AXIS_ANCESTOR:
            first = NULL;
            next = xmlXPathNextAncestor;
            break;
        case AXIS_ANCESTOR_OR_SELF:
            first = NULL;
            next = xmlXPathNextAncestorOrSelf;
            break;
        case AXIS_ATTRIBUTE:
            first = NULL;
	    last = NULL;
            next = xmlXPathNextAttribute;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
        case AXIS_CHILD:
	    last = NULL;
	    if (op->rewriteType == XP_REWRITE_DOS_CHILD_ELEM) {
		outerNext = xmlXPathNextDescendantOrSelfElemParent;
	    }
	    if (((test == NODE_TEST_NAME) || (test == NODE_TEST_ALL)) &&
		(type == NODE_TYPE_NODE))
	    {
		next = xmlXPathNextChildElement;
	    } else
		next = xmlXPathNextChild;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
        case AXIS_DESCENDANT:
	    last = NULL;
            next = xmlXPathNextDescendant;
            break;
        case AXIS_DESCENDANT_OR_SELF:
	    last = NULL;
            next = xmlXPathNextDescendantOrSelf;
            break;
        case AXIS_FOLLOWING:
	    last = NULL;
            next = xmlXPathNextFollowing;
            break;
        case AXIS_FOLLOWING_SIBLING:
	    last = NULL;
            next = xmlXPathNextFollowingSibling;
            break;
        case AXIS_NAMESPACE:
            first = NULL;
	    last = NULL;
            next = (xmlXPathTraversalFunction) xmlXPathNextNamespace;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
        case AXIS_PARENT:
            first = NULL;
            next = xmlXPathNextParent;
            break;
        case AXIS_PRECEDING:
            first = NULL;
            next = xmlXPathNextPrecedingInternal;
            break;
        case AXIS_PRECEDING_SIBLING:
            first = NULL;
            next = xmlXPathNextPrecedingSibling;
            break;
        case AXIS_SELF:
            first = NULL;
	    last = NULL;
            next = xmlXPathNextSelf;
	    mergeAndClear = xmlXPathNodeSetMergeAndClearNoDupls;
            break;
    }

#ifdef DEBUG_STEP
    xmlXPathDebugDumpStepAxis(op,
	(obj->nodesetval != NULL) ? obj->nodesetval->nodeNr : 0);
#endif

    if (next == NULL) {
	xmlXPathReleaseObject(xpctxt, obj);
        return(0);
    }
    contextSeq = obj->nodesetval;
    if ((contextSeq == NULL) || (contextSeq->nodeNr <= 0)) {
	xmlXPathReleaseObject(xpctxt, obj);
        valuePush(ctxt, xmlXPathCacheWrapNodeSet(xpctxt, NULL));
        return(0);
    }
    maxPos = 0;
    predOp = NULL;
    hasPredicateRange = 0;
    hasAxisRange = 0;
    if (op->ch2 != -1) {
	predOp = &ctxt->comp->steps[op->ch2];
	if (xmlXPathIsPositionalPredicate(ctxt, predOp, &maxPos)) {
	    if (predOp->ch1 != -1) {
		predOp = &ctxt->comp->steps[predOp->ch1];
		hasPredicateRange = 1;
	    } else {
		predOp = NULL;
		hasAxisRange = 1;
	    }
	}
    }
    breakOnFirstHit = ((toBool) && (predOp == NULL)) ? 1 : 0;
    oldContextNode = xpctxt->node;
    addNode = xmlXPathNodeSetAddUnique;
    outSeq = NULL;
    seq = NULL;
    outerContextNode = NULL;
    contextNode = NULL;
    contextIdx = 0;


    while ((contextIdx < contextSeq->nodeNr) || (contextNode != NULL)) {
	if (outerNext != NULL) {
	    if (contextNode == NULL) {
		outerContextNode = contextSeq->nodeTab[contextIdx++];
		contextNode = outerNext(NULL, outerContextNode);
	    } else
		contextNode = outerNext(contextNode, outerContextNode);
	    if (contextNode == NULL)
		continue;
	    xpctxt->node = contextNode;
	} else
	    xpctxt->node = contextSeq->nodeTab[contextIdx++];

	if (seq == NULL) {
	    seq = xmlXPathNodeSetCreate(NULL);
	    if (seq == NULL) {
		total = 0;
		goto error;
	    }
	}
	pos = 0;
	cur = NULL;
	hasNsNodes = 0;
        do {
            cur = next(ctxt, cur);
            if (cur == NULL)
                break;

            if ((first != NULL) && (*first != NULL)) {
		if (*first == cur)
		    break;
		if (((total % 256) == 0) &&
#ifdef XP_OPTIMIZED_NON_ELEM_COMPARISON
		    (xmlXPathCmpNodesExt(*first, cur) >= 0))
#else
		    (xmlXPathCmpNodes(*first, cur) >= 0))
#endif
		{
		    break;
		}
	    }
	    if ((last != NULL) && (*last != NULL)) {
		if (*last == cur)
		    break;
		if (((total % 256) == 0) &&
#ifdef XP_OPTIMIZED_NON_ELEM_COMPARISON
		    (xmlXPathCmpNodesExt(cur, *last) >= 0))
#else
		    (xmlXPathCmpNodes(cur, *last) >= 0))
#endif
		{
		    break;
		}
	    }

            total++;

#ifdef DEBUG_STEP
            xmlGenericError(xmlGenericErrorContext, " %s", cur->name);
#endif

	    switch (test) {
                case NODE_TEST_NONE:
		    total = 0;
                    STRANGE
		    goto error;
                case NODE_TEST_TYPE:
		    if (type == NODE_TYPE_NODE) {
			switch (cur->type) {
			    case XML_DOCUMENT_NODE:
			    case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
			    case XML_DOCB_DOCUMENT_NODE:
#endif
			    case XML_ELEMENT_NODE:
			    case XML_ATTRIBUTE_NODE:
			    case XML_PI_NODE:
			    case XML_COMMENT_NODE:
			    case XML_CDATA_SECTION_NODE:
			    case XML_TEXT_NODE:
			    case XML_NAMESPACE_DECL:
				XP_TEST_HIT
				break;
			    default:
				break;
			}
		    } else if (cur->type == type) {
			if (cur->type == XML_NAMESPACE_DECL)
			    XP_TEST_HIT_NS
			else
			    XP_TEST_HIT
		    } else if ((type == NODE_TYPE_TEXT) &&
			 (cur->type == XML_CDATA_SECTION_NODE))
		    {
			XP_TEST_HIT
		    }
		    break;
                case NODE_TEST_PI:
                    if ((cur->type == XML_PI_NODE) &&
                        ((name == NULL) || xmlStrEqual(name, cur->name)))
		    {
			XP_TEST_HIT
                    }
                    break;
                case NODE_TEST_ALL:
                    if (axis == AXIS_ATTRIBUTE) {
                        if (cur->type == XML_ATTRIBUTE_NODE)
			{
			    XP_TEST_HIT
                        }
                    } else if (axis == AXIS_NAMESPACE) {
                        if (cur->type == XML_NAMESPACE_DECL)
			{
			    XP_TEST_HIT_NS
                        }
                    } else {
                        if (cur->type == XML_ELEMENT_NODE) {
                            if (prefix == NULL)
			    {
				XP_TEST_HIT

                            } else if ((cur->ns != NULL) &&
				(xmlStrEqual(URI, cur->ns->href)))
			    {
				XP_TEST_HIT
                            }
                        }
                    }
                    break;
                case NODE_TEST_NS:{
                        TODO;
                        break;
                    }
                case NODE_TEST_NAME:
                    if (axis == AXIS_ATTRIBUTE) {
                        if (cur->type != XML_ATTRIBUTE_NODE)
			    break;
		    } else if (axis == AXIS_NAMESPACE) {
                        if (cur->type != XML_NAMESPACE_DECL)
			    break;
		    } else {
		        if (cur->type != XML_ELEMENT_NODE)
			    break;
		    }
                    switch (cur->type) {
                        case XML_ELEMENT_NODE:
                            if (xmlStrEqual(name, cur->name)) {
                                if (prefix == NULL) {
                                    if (cur->ns == NULL)
				    {
					XP_TEST_HIT
                                    }
                                } else {
                                    if ((cur->ns != NULL) &&
                                        (xmlStrEqual(URI, cur->ns->href)))
				    {
					XP_TEST_HIT
                                    }
                                }
                            }
                            break;
                        case XML_ATTRIBUTE_NODE:{
                                xmlAttrPtr attr = (xmlAttrPtr) cur;

                                if (xmlStrEqual(name, attr->name)) {
                                    if (prefix == NULL) {
                                        if ((attr->ns == NULL) ||
                                            (attr->ns->prefix == NULL))
					{
					    XP_TEST_HIT
                                        }
                                    } else {
                                        if ((attr->ns != NULL) &&
                                            (xmlStrEqual(URI,
					      attr->ns->href)))
					{
					    XP_TEST_HIT
                                        }
                                    }
                                }
                                break;
                            }
                        case XML_NAMESPACE_DECL:
                            if (cur->type == XML_NAMESPACE_DECL) {
                                xmlNsPtr ns = (xmlNsPtr) cur;

                                if ((ns->prefix != NULL) && (name != NULL)
                                    && (xmlStrEqual(ns->prefix, name)))
				{
				    XP_TEST_HIT_NS
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    break;
	    } 
        } while (cur != NULL);

	goto apply_predicates;

axis_range_end: 
	if (outSeq == NULL) {
	    outSeq = seq;
	    seq = NULL;
	} else
	    outSeq = mergeAndClear(outSeq, seq, 0);
	if (toBool)
	    break;
	continue;

first_hit: 
	if (outSeq == NULL) {
	    outSeq = seq;
	    seq = NULL;
	} else
	    outSeq = mergeAndClear(outSeq, seq, 0);
	break;

#ifdef DEBUG_STEP
	if (seq != NULL)
	    nbMatches += seq->nodeNr;
#endif

apply_predicates: 
        if ((predOp != NULL) && (seq->nodeNr > 0)) {
	    
	    
	    size = seq->nodeNr;
	    if (hasPredicateRange != 0)
		newSize = xmlXPathCompOpEvalPositionalPredicate(ctxt,
		    predOp, seq, size, maxPos, maxPos, hasNsNodes);
	    else
		newSize = xmlXPathCompOpEvalPredicate(ctxt,
		    predOp, seq, size, hasNsNodes);

	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		total = 0;
		goto error;
	    }
	    if (newSize == 0) {
		xmlXPathNodeSetClear(seq, hasNsNodes);
	    } else if (seq->nodeNr > 0) {
		if (outSeq == NULL) {
		    if (size != newSize) {
			outSeq = mergeAndClear(NULL, seq, 1);
		    } else {
			outSeq = seq;
			seq = NULL;
		    }
		} else
		    outSeq = mergeAndClear(outSeq, seq,
			(size != newSize) ? 1: 0);
		if (toBool)
		    break;
	    }
        } else if (seq->nodeNr > 0) {
	    if (outSeq == NULL) {
		outSeq = seq;
		seq = NULL;
	    } else {
		outSeq = mergeAndClear(outSeq, seq, 0);
	    }
	}
    }

error:
    if ((obj->boolval) && (obj->user != NULL)) {
	ctxt->value->boolval = 1;
	ctxt->value->user = obj->user;
	obj->user = NULL;
	obj->boolval = 0;
    }
    xmlXPathReleaseObject(xpctxt, obj);

    if (outSeq == NULL) {
	if ((seq != NULL) && (seq->nodeNr == 0))
	    outSeq = seq;
	else
	    outSeq = xmlXPathNodeSetCreate(NULL);
        
    }
    if ((seq != NULL) && (seq != outSeq)) {
	 xmlXPathFreeNodeSet(seq);
    }
    valuePush(ctxt, xmlXPathCacheWrapNodeSet(xpctxt, outSeq));
    xpctxt->node = oldContextNode;

#ifdef DEBUG_STEP
    xmlGenericError(xmlGenericErrorContext,
	"\nExamined %d nodes, found %d nodes at that step\n",
	total, nbMatches);
#endif

    return(total);
}

static int
xmlXPathCompOpEvalFilterFirst(xmlXPathParserContextPtr ctxt,
			      xmlXPathStepOpPtr op, xmlNodePtr * first);

static int
xmlXPathCompOpEvalFirst(xmlXPathParserContextPtr ctxt,
                        xmlXPathStepOpPtr op, xmlNodePtr * first)
{
    int total = 0, cur;
    xmlXPathCompExprPtr comp;
    xmlXPathObjectPtr arg1, arg2;

    CHECK_ERROR0;
    comp = ctxt->comp;
    switch (op->op) {
        case XPATH_OP_END:
            return (0);
        case XPATH_OP_UNION:
            total =
                xmlXPathCompOpEvalFirst(ctxt, &comp->steps[op->ch1],
                                        first);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL)
                && (ctxt->value->type == XPATH_NODESET)
                && (ctxt->value->nodesetval != NULL)
                && (ctxt->value->nodesetval->nodeNr >= 1)) {
		if (ctxt->value->nodesetval->nodeNr > 1)
		    xmlXPathNodeSetSort(ctxt->value->nodesetval);
                *first = ctxt->value->nodesetval->nodeTab[0];
            }
            cur =
                xmlXPathCompOpEvalFirst(ctxt, &comp->steps[op->ch2],
                                        first);
	    CHECK_ERROR0;
            CHECK_TYPE0(XPATH_NODESET);
            arg2 = valuePop(ctxt);

            CHECK_TYPE0(XPATH_NODESET);
            arg1 = valuePop(ctxt);

            arg1->nodesetval = xmlXPathNodeSetMerge(arg1->nodesetval,
                                                    arg2->nodesetval);
            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            
	    if (total > cur)
		xmlXPathCompSwap(op);
            return (total + cur);
        case XPATH_OP_ROOT:
            xmlXPathRoot(ctxt);
            return (0);
        case XPATH_OP_NODE:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
	    valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
		ctxt->context->node));
            return (total);
        case XPATH_OP_RESET:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            ctxt->context->node = NULL;
            return (total);
        case XPATH_OP_COLLECT:{
                if (op->ch1 == -1)
                    return (total);

                total = xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		CHECK_ERROR0;

                total += xmlXPathNodeCollectAndTest(ctxt, op, first, NULL, 0);
                return (total);
            }
        case XPATH_OP_VALUE:
            valuePush(ctxt,
                      xmlXPathCacheObjectCopy(ctxt->context,
			(xmlXPathObjectPtr) op->value4));
            return (0);
        case XPATH_OP_SORT:
            if (op->ch1 != -1)
                total +=
                    xmlXPathCompOpEvalFirst(ctxt, &comp->steps[op->ch1],
                                            first);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL)
                && (ctxt->value->type == XPATH_NODESET)
                && (ctxt->value->nodesetval != NULL)
		&& (ctxt->value->nodesetval->nodeNr > 1))
                xmlXPathNodeSetSort(ctxt->value->nodesetval);
            return (total);
#ifdef XP_OPTIMIZED_FILTER_FIRST
	case XPATH_OP_FILTER:
                total += xmlXPathCompOpEvalFilterFirst(ctxt, op, first);
            return (total);
#endif
        default:
            return (xmlXPathCompOpEval(ctxt, op));
    }
}

static int
xmlXPathCompOpEvalLast(xmlXPathParserContextPtr ctxt, xmlXPathStepOpPtr op,
                       xmlNodePtr * last)
{
    int total = 0, cur;
    xmlXPathCompExprPtr comp;
    xmlXPathObjectPtr arg1, arg2;
    xmlNodePtr bak;
    xmlDocPtr bakd;
    int pp;
    int cs;

    CHECK_ERROR0;
    comp = ctxt->comp;
    switch (op->op) {
        case XPATH_OP_END:
            return (0);
        case XPATH_OP_UNION:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total =
                xmlXPathCompOpEvalLast(ctxt, &comp->steps[op->ch1], last);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL)
                && (ctxt->value->type == XPATH_NODESET)
                && (ctxt->value->nodesetval != NULL)
                && (ctxt->value->nodesetval->nodeNr >= 1)) {
		if (ctxt->value->nodesetval->nodeNr > 1)
		    xmlXPathNodeSetSort(ctxt->value->nodesetval);
                *last =
                    ctxt->value->nodesetval->nodeTab[ctxt->value->
                                                     nodesetval->nodeNr -
                                                     1];
            }
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            cur =
                xmlXPathCompOpEvalLast(ctxt, &comp->steps[op->ch2], last);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL)
                && (ctxt->value->type == XPATH_NODESET)
                && (ctxt->value->nodesetval != NULL)
                && (ctxt->value->nodesetval->nodeNr >= 1)) { 
            }
            CHECK_TYPE0(XPATH_NODESET);
            arg2 = valuePop(ctxt);

            CHECK_TYPE0(XPATH_NODESET);
            arg1 = valuePop(ctxt);

            arg1->nodesetval = xmlXPathNodeSetMerge(arg1->nodesetval,
                                                    arg2->nodesetval);
            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            
	    if (total > cur)
		xmlXPathCompSwap(op);
            return (total + cur);
        case XPATH_OP_ROOT:
            xmlXPathRoot(ctxt);
            return (0);
        case XPATH_OP_NODE:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
	    valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
		ctxt->context->node));
            return (total);
        case XPATH_OP_RESET:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            ctxt->context->node = NULL;
            return (total);
        case XPATH_OP_COLLECT:{
                if (op->ch1 == -1)
                    return (0);

                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		CHECK_ERROR0;

                total += xmlXPathNodeCollectAndTest(ctxt, op, NULL, last, 0);
                return (total);
            }
        case XPATH_OP_VALUE:
            valuePush(ctxt,
                      xmlXPathCacheObjectCopy(ctxt->context,
			(xmlXPathObjectPtr) op->value4));
            return (0);
        case XPATH_OP_SORT:
            if (op->ch1 != -1)
                total +=
                    xmlXPathCompOpEvalLast(ctxt, &comp->steps[op->ch1],
                                           last);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL)
                && (ctxt->value->type == XPATH_NODESET)
                && (ctxt->value->nodesetval != NULL)
		&& (ctxt->value->nodesetval->nodeNr > 1))
                xmlXPathNodeSetSort(ctxt->value->nodesetval);
            return (total);
        default:
            return (xmlXPathCompOpEval(ctxt, op));
    }
}

#ifdef XP_OPTIMIZED_FILTER_FIRST
static int
xmlXPathCompOpEvalFilterFirst(xmlXPathParserContextPtr ctxt,
			      xmlXPathStepOpPtr op, xmlNodePtr * first)
{
    int total = 0;
    xmlXPathCompExprPtr comp;
    xmlXPathObjectPtr res;
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr oldset;
    xmlNodePtr oldnode;
    xmlDocPtr oldDoc;
    int i;

    CHECK_ERROR0;
    comp = ctxt->comp;
    if ((op->ch1 != -1) && (op->ch2 != -1) &&
	(comp->steps[op->ch1].op == XPATH_OP_SORT) &&
	(comp->steps[op->ch2].op == XPATH_OP_SORT)) {
	int f = comp->steps[op->ch2].ch1;

	if ((f != -1) &&
	    (comp->steps[f].op == XPATH_OP_FUNCTION) &&
	    (comp->steps[f].value5 == NULL) &&
	    (comp->steps[f].value == 0) &&
	    (comp->steps[f].value4 != NULL) &&
	    (xmlStrEqual
	    (comp->steps[f].value4, BAD_CAST "last"))) {
	    xmlNodePtr last = NULL;

	    total +=
		xmlXPathCompOpEvalLast(ctxt,
		    &comp->steps[op->ch1],
		    &last);
	    CHECK_ERROR0;
	    if ((ctxt->value != NULL) &&
		(ctxt->value->type == XPATH_NODESET) &&
		(ctxt->value->nodesetval != NULL) &&
		(ctxt->value->nodesetval->nodeTab != NULL) &&
		(ctxt->value->nodesetval->nodeNr > 1)) {
		ctxt->value->nodesetval->nodeTab[0] =
		    ctxt->value->nodesetval->nodeTab[ctxt->
		    value->
		    nodesetval->
		    nodeNr -
		    1];
		ctxt->value->nodesetval->nodeNr = 1;
		*first = *(ctxt->value->nodesetval->nodeTab);
	    }
	    return (total);
	}
    }

    if (op->ch1 != -1)
	total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
    CHECK_ERROR0;
    if (op->ch2 == -1)
	return (total);
    if (ctxt->value == NULL)
	return (total);

#ifdef LIBXML_XPTR_ENABLED
    oldnode = ctxt->context->node;
    if (ctxt->value->type == XPATH_LOCATIONSET) {
	xmlXPathObjectPtr tmp = NULL;
	xmlLocationSetPtr newlocset = NULL;
	xmlLocationSetPtr oldlocset;

	CHECK_TYPE0(XPATH_LOCATIONSET);
	obj = valuePop(ctxt);
	oldlocset = obj->user;
	ctxt->context->node = NULL;

	if ((oldlocset == NULL) || (oldlocset->locNr == 0)) {
	    ctxt->context->contextSize = 0;
	    ctxt->context->proximityPosition = 0;
	    if (op->ch2 != -1)
		total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    res = valuePop(ctxt);
	    if (res != NULL) {
		xmlXPathReleaseObject(ctxt->context, res);
	    }
	    valuePush(ctxt, obj);
	    CHECK_ERROR0;
	    return (total);
	}
	newlocset = xmlXPtrLocationSetCreate(NULL);

	for (i = 0; i < oldlocset->locNr; i++) {
	    ctxt->context->node = oldlocset->locTab[i]->user;
	    ctxt->context->contextSize = oldlocset->locNr;
	    ctxt->context->proximityPosition = i + 1;
	    if (tmp == NULL) {
		tmp = xmlXPathCacheNewNodeSet(ctxt->context,
		    ctxt->context->node);
	    } else {
		xmlXPathNodeSetAddUnique(tmp->nodesetval,
		    ctxt->context->node);
	    }
	    valuePush(ctxt, tmp);
	    if (op->ch2 != -1)
		total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		xmlXPathFreeObject(obj);
		return(0);
	    }
	    res = valuePop(ctxt);
	    if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
		xmlXPtrLocationSetAdd(newlocset,
		    xmlXPathCacheObjectCopy(ctxt->context,
			oldlocset->locTab[i]));
	    }
	    if (res != NULL) {
		xmlXPathReleaseObject(ctxt->context, res);
	    }
	    if (ctxt->value == tmp) {
		valuePop(ctxt);
		xmlXPathNodeSetClear(tmp->nodesetval, 1);
		
	    } else
		tmp = NULL;
	    ctxt->context->node = NULL;
	    if (newlocset->locNr > 0) {
		*first = (xmlNodePtr) oldlocset->locTab[i]->user;
		break;
	    }
	}
	if (tmp != NULL) {
	    xmlXPathReleaseObject(ctxt->context, tmp);
	}
	xmlXPathReleaseObject(ctxt->context, obj);
	ctxt->context->node = NULL;
	ctxt->context->contextSize = -1;
	ctxt->context->proximityPosition = -1;
	valuePush(ctxt, xmlXPtrWrapLocationSet(newlocset));
	ctxt->context->node = oldnode;
	return (total);
    }
#endif 

    CHECK_TYPE0(XPATH_NODESET);
    obj = valuePop(ctxt);
    oldset = obj->nodesetval;

    oldnode = ctxt->context->node;
    oldDoc = ctxt->context->doc;
    ctxt->context->node = NULL;

    if ((oldset == NULL) || (oldset->nodeNr == 0)) {
	ctxt->context->contextSize = 0;
	ctxt->context->proximityPosition = 0;
	valuePush(ctxt, obj);
	ctxt->context->node = oldnode;
	CHECK_ERROR0;
    } else {
	xmlNodeSetPtr newset;
	xmlXPathObjectPtr tmp = NULL;
	newset = xmlXPathNodeSetCreate(NULL);
        

	for (i = 0; i < oldset->nodeNr; i++) {
	    ctxt->context->node = oldset->nodeTab[i];
	    if ((oldset->nodeTab[i]->type != XML_NAMESPACE_DECL) &&
		(oldset->nodeTab[i]->doc != NULL))
		ctxt->context->doc = oldset->nodeTab[i]->doc;
	    if (tmp == NULL) {
		tmp = xmlXPathCacheNewNodeSet(ctxt->context,
		    ctxt->context->node);
	    } else {
		xmlXPathNodeSetAddUnique(tmp->nodesetval,
		    ctxt->context->node);
	    }
	    valuePush(ctxt, tmp);
	    ctxt->context->contextSize = oldset->nodeNr;
	    ctxt->context->proximityPosition = i + 1;
	    if (op->ch2 != -1)
		total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error != XPATH_EXPRESSION_OK) {
		xmlXPathFreeNodeSet(newset);
		xmlXPathFreeObject(obj);
		return(0);
	    }
	    res = valuePop(ctxt);
	    if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
		xmlXPathNodeSetAdd(newset, oldset->nodeTab[i]);
	    }
	    if (res != NULL) {
		xmlXPathReleaseObject(ctxt->context, res);
	    }
	    if (ctxt->value == tmp) {
		valuePop(ctxt);
		xmlXPathNodeSetClear(tmp->nodesetval, 1);
	    } else
		tmp = NULL;
	    ctxt->context->node = NULL;
	    if (newset->nodeNr > 0) {
		*first = *(newset->nodeTab);
		break;
	    }
	}
	if (tmp != NULL) {
	    xmlXPathReleaseObject(ctxt->context, tmp);
	}
	xmlXPathReleaseObject(ctxt->context, obj);
	ctxt->context->node = NULL;
	ctxt->context->contextSize = -1;
	ctxt->context->proximityPosition = -1;
	
	ctxt->context->doc = oldDoc;
	valuePush(ctxt, xmlXPathCacheWrapNodeSet(ctxt->context, newset));
    }
    ctxt->context->node = oldnode;
    return(total);
}
#endif 

static int
xmlXPathCompOpEval(xmlXPathParserContextPtr ctxt, xmlXPathStepOpPtr op)
{
    int total = 0;
    int equal, ret;
    xmlXPathCompExprPtr comp;
    xmlXPathObjectPtr arg1, arg2;
    xmlNodePtr bak;
    xmlDocPtr bakd;
    int pp;
    int cs;

    CHECK_ERROR0;
    comp = ctxt->comp;
    switch (op->op) {
        case XPATH_OP_END:
            return (0);
        case XPATH_OP_AND:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            xmlXPathBooleanFunction(ctxt, 1);
            if ((ctxt->value == NULL) || (ctxt->value->boolval == 0))
                return (total);
            arg2 = valuePop(ctxt);
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error) {
		xmlXPathFreeObject(arg2);
		return(0);
	    }
            xmlXPathBooleanFunction(ctxt, 1);
            arg1 = valuePop(ctxt);
            arg1->boolval &= arg2->boolval;
            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            return (total);
        case XPATH_OP_OR:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            xmlXPathBooleanFunction(ctxt, 1);
            if ((ctxt->value == NULL) || (ctxt->value->boolval == 1))
                return (total);
            arg2 = valuePop(ctxt);
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    if (ctxt->error) {
		xmlXPathFreeObject(arg2);
		return(0);
	    }
            xmlXPathBooleanFunction(ctxt, 1);
            arg1 = valuePop(ctxt);
            arg1->boolval |= arg2->boolval;
            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            return (total);
        case XPATH_OP_EQUAL:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
	    if (op->value)
		equal = xmlXPathEqualValues(ctxt);
	    else
		equal = xmlXPathNotEqualValues(ctxt);
	    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, equal));
            return (total);
        case XPATH_OP_CMP:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            ret = xmlXPathCompareValues(ctxt, op->value, op->value2);
	    valuePush(ctxt, xmlXPathCacheNewBoolean(ctxt->context, ret));
            return (total);
        case XPATH_OP_PLUS:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1) {
		ctxt->context->doc = bakd;
		ctxt->context->node = bak;
		ctxt->context->proximityPosition = pp;
		ctxt->context->contextSize = cs;
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    }
	    CHECK_ERROR0;
            if (op->value == 0)
                xmlXPathSubValues(ctxt);
            else if (op->value == 1)
                xmlXPathAddValues(ctxt);
            else if (op->value == 2)
                xmlXPathValueFlipSign(ctxt);
            else if (op->value == 3) {
                CAST_TO_NUMBER;
                CHECK_TYPE0(XPATH_NUMBER);
            }
            return (total);
        case XPATH_OP_MULT:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            if (op->value == 0)
                xmlXPathMultValues(ctxt);
            else if (op->value == 1)
                xmlXPathDivValues(ctxt);
            else if (op->value == 2)
                xmlXPathModValues(ctxt);
            return (total);
        case XPATH_OP_UNION:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
	    ctxt->context->doc = bakd;
	    ctxt->context->node = bak;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->contextSize = cs;
            total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            CHECK_TYPE0(XPATH_NODESET);
            arg2 = valuePop(ctxt);

            CHECK_TYPE0(XPATH_NODESET);
            arg1 = valuePop(ctxt);

	    if ((arg1->nodesetval == NULL) ||
		((arg2->nodesetval != NULL) &&
		 (arg2->nodesetval->nodeNr != 0)))
	    {
		arg1->nodesetval = xmlXPathNodeSetMerge(arg1->nodesetval,
							arg2->nodesetval);
	    }

            valuePush(ctxt, arg1);
	    xmlXPathReleaseObject(ctxt->context, arg2);
            return (total);
        case XPATH_OP_ROOT:
            xmlXPathRoot(ctxt);
            return (total);
        case XPATH_OP_NODE:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
	    valuePush(ctxt, xmlXPathCacheNewNodeSet(ctxt->context,
		ctxt->context->node));
            return (total);
        case XPATH_OP_RESET:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if (op->ch2 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	    CHECK_ERROR0;
            ctxt->context->node = NULL;
            return (total);
        case XPATH_OP_COLLECT:{
                if (op->ch1 == -1)
                    return (total);

                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		CHECK_ERROR0;

                total += xmlXPathNodeCollectAndTest(ctxt, op, NULL, NULL, 0);
                return (total);
            }
        case XPATH_OP_VALUE:
            valuePush(ctxt,
                      xmlXPathCacheObjectCopy(ctxt->context,
			(xmlXPathObjectPtr) op->value4));
            return (total);
        case XPATH_OP_VARIABLE:{
		xmlXPathObjectPtr val;

                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
                if (op->value5 == NULL) {
		    val = xmlXPathVariableLookup(ctxt->context, op->value4);
		    if (val == NULL) {
			ctxt->error = XPATH_UNDEF_VARIABLE_ERROR;
			return(0);
		    }
                    valuePush(ctxt, val);
		} else {
                    const xmlChar *URI;

                    URI = xmlXPathNsLookup(ctxt->context, op->value5);
                    if (URI == NULL) {
                        xmlGenericError(xmlGenericErrorContext,
            "xmlXPathCompOpEval: variable %s bound to undefined prefix %s\n",
                                    (char *) op->value4, (char *)op->value5);
                        ctxt->error = XPATH_UNDEF_PREFIX_ERROR;
                        return (total);
                    }
		    val = xmlXPathVariableLookupNS(ctxt->context,
                                                       op->value4, URI);
		    if (val == NULL) {
			ctxt->error = XPATH_UNDEF_VARIABLE_ERROR;
			return(0);
		    }
                    valuePush(ctxt, val);
                }
                return (total);
            }
        case XPATH_OP_FUNCTION:{
                xmlXPathFunction func;
                const xmlChar *oldFunc, *oldFuncURI;
		int i;

                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		if (ctxt->valueNr < op->value) {
		    xmlGenericError(xmlGenericErrorContext,
			    "xmlXPathCompOpEval: parameter error\n");
		    ctxt->error = XPATH_INVALID_OPERAND;
		    return (total);
		}
		for (i = 0; i < op->value; i++)
		    if (ctxt->valueTab[(ctxt->valueNr - 1) - i] == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlXPathCompOpEval: parameter error\n");
			ctxt->error = XPATH_INVALID_OPERAND;
			return (total);
		    }
                if (op->cache != NULL)
                    XML_CAST_FPTR(func) = op->cache;
                else {
                    const xmlChar *URI = NULL;

                    if (op->value5 == NULL)
                        func =
                            xmlXPathFunctionLookup(ctxt->context,
                                                   op->value4);
                    else {
                        URI = xmlXPathNsLookup(ctxt->context, op->value5);
                        if (URI == NULL) {
                            xmlGenericError(xmlGenericErrorContext,
            "xmlXPathCompOpEval: function %s bound to undefined prefix %s\n",
                                    (char *)op->value4, (char *)op->value5);
                            ctxt->error = XPATH_UNDEF_PREFIX_ERROR;
                            return (total);
                        }
                        func = xmlXPathFunctionLookupNS(ctxt->context,
                                                        op->value4, URI);
                    }
                    if (func == NULL) {
                        xmlGenericError(xmlGenericErrorContext,
                                "xmlXPathCompOpEval: function %s not found\n",
                                        (char *)op->value4);
                        XP_ERROR0(XPATH_UNKNOWN_FUNC_ERROR);
                    }
                    op->cache = XML_CAST_FPTR(func);
                    op->cacheURI = (void *) URI;
                }
                oldFunc = ctxt->context->function;
                oldFuncURI = ctxt->context->functionURI;
                ctxt->context->function = op->value4;
                ctxt->context->functionURI = op->cacheURI;
                func(ctxt, op->value);
                ctxt->context->function = oldFunc;
                ctxt->context->functionURI = oldFuncURI;
                return (total);
            }
        case XPATH_OP_ARG:
	    bakd = ctxt->context->doc;
	    bak = ctxt->context->node;
	    pp = ctxt->context->proximityPosition;
	    cs = ctxt->context->contextSize;
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    ctxt->context->contextSize = cs;
	    ctxt->context->proximityPosition = pp;
	    ctxt->context->node = bak;
	    ctxt->context->doc = bakd;
	    CHECK_ERROR0;
            if (op->ch2 != -1) {
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
	        ctxt->context->doc = bakd;
	        ctxt->context->node = bak;
	        CHECK_ERROR0;
	    }
            return (total);
        case XPATH_OP_PREDICATE:
        case XPATH_OP_FILTER:{
                xmlXPathObjectPtr res;
                xmlXPathObjectPtr obj, tmp;
                xmlNodeSetPtr newset = NULL;
                xmlNodeSetPtr oldset;
                xmlNodePtr oldnode;
		xmlDocPtr oldDoc;
                int i;

                if ((op->ch1 != -1) && (op->ch2 != -1) &&
#ifdef XP_OPTIMIZED_FILTER_FIRST
                    ((comp->steps[op->ch1].op == XPATH_OP_SORT) || 
		     (comp->steps[op->ch1].op == XPATH_OP_FILTER)) && 
#else
		    (comp->steps[op->ch1].op == XPATH_OP_SORT) &&
#endif
                    (comp->steps[op->ch2].op == XPATH_OP_VALUE)) { 
                    xmlXPathObjectPtr val;

                    val = comp->steps[op->ch2].value4;
                    if ((val != NULL) && (val->type == XPATH_NUMBER) &&
                        (val->floatval == 1.0)) {
                        xmlNodePtr first = NULL;

                        total +=
                            xmlXPathCompOpEvalFirst(ctxt,
                                                    &comp->steps[op->ch1],
                                                    &first);
			CHECK_ERROR0;
                        if ((ctxt->value != NULL) &&
                            (ctxt->value->type == XPATH_NODESET) &&
                            (ctxt->value->nodesetval != NULL) &&
                            (ctxt->value->nodesetval->nodeNr > 1))
                            ctxt->value->nodesetval->nodeNr = 1;
                        return (total);
                    }
                }
                if ((op->ch1 != -1) && (op->ch2 != -1) &&
                    (comp->steps[op->ch1].op == XPATH_OP_SORT) &&
                    (comp->steps[op->ch2].op == XPATH_OP_SORT)) {
                    int f = comp->steps[op->ch2].ch1;

                    if ((f != -1) &&
                        (comp->steps[f].op == XPATH_OP_FUNCTION) &&
                        (comp->steps[f].value5 == NULL) &&
                        (comp->steps[f].value == 0) &&
                        (comp->steps[f].value4 != NULL) &&
                        (xmlStrEqual
                         (comp->steps[f].value4, BAD_CAST "last"))) {
                        xmlNodePtr last = NULL;

                        total +=
                            xmlXPathCompOpEvalLast(ctxt,
                                                   &comp->steps[op->ch1],
                                                   &last);
			CHECK_ERROR0;
                        if ((ctxt->value != NULL) &&
                            (ctxt->value->type == XPATH_NODESET) &&
                            (ctxt->value->nodesetval != NULL) &&
                            (ctxt->value->nodesetval->nodeTab != NULL) &&
                            (ctxt->value->nodesetval->nodeNr > 1)) {
                            ctxt->value->nodesetval->nodeTab[0] =
                                ctxt->value->nodesetval->nodeTab[ctxt->
                                                                 value->
                                                                 nodesetval->
                                                                 nodeNr -
                                                                 1];
                            ctxt->value->nodesetval->nodeNr = 1;
                        }
                        return (total);
                    }
                }
                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
		CHECK_ERROR0;
                if (op->ch2 == -1)
                    return (total);
                if (ctxt->value == NULL)
                    return (total);

                oldnode = ctxt->context->node;

#ifdef LIBXML_XPTR_ENABLED
                if (ctxt->value->type == XPATH_LOCATIONSET) {
                    xmlLocationSetPtr newlocset = NULL;
                    xmlLocationSetPtr oldlocset;

                    CHECK_TYPE0(XPATH_LOCATIONSET);
                    obj = valuePop(ctxt);
                    oldlocset = obj->user;
                    ctxt->context->node = NULL;

                    if ((oldlocset == NULL) || (oldlocset->locNr == 0)) {
                        ctxt->context->contextSize = 0;
                        ctxt->context->proximityPosition = 0;
                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
                        res = valuePop(ctxt);
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        valuePush(ctxt, obj);
                        CHECK_ERROR0;
                        return (total);
                    }
                    newlocset = xmlXPtrLocationSetCreate(NULL);

                    for (i = 0; i < oldlocset->locNr; i++) {
                        ctxt->context->node = oldlocset->locTab[i]->user;
                        ctxt->context->contextSize = oldlocset->locNr;
                        ctxt->context->proximityPosition = i + 1;
			tmp = xmlXPathCacheNewNodeSet(ctxt->context,
			    ctxt->context->node);
                        valuePush(ctxt, tmp);

                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			if (ctxt->error != XPATH_EXPRESSION_OK) {
			    xmlXPathFreeObject(obj);
			    return(0);
			}

                        res = valuePop(ctxt);
                        if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
                            xmlXPtrLocationSetAdd(newlocset,
                                                  xmlXPathObjectCopy
                                                  (oldlocset->locTab[i]));
                        }

                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        if (ctxt->value == tmp) {
                            res = valuePop(ctxt);
			    xmlXPathReleaseObject(ctxt->context, res);
                        }

                        ctxt->context->node = NULL;
                    }

		    xmlXPathReleaseObject(ctxt->context, obj);
                    ctxt->context->node = NULL;
                    ctxt->context->contextSize = -1;
                    ctxt->context->proximityPosition = -1;
                    valuePush(ctxt, xmlXPtrWrapLocationSet(newlocset));
                    ctxt->context->node = oldnode;
                    return (total);
                }
#endif 

                CHECK_TYPE0(XPATH_NODESET);
                obj = valuePop(ctxt);
                oldset = obj->nodesetval;

                oldnode = ctxt->context->node;
		oldDoc = ctxt->context->doc;
                ctxt->context->node = NULL;

                if ((oldset == NULL) || (oldset->nodeNr == 0)) {
                    ctxt->context->contextSize = 0;
                    ctxt->context->proximityPosition = 0;
                    valuePush(ctxt, obj);
                    ctxt->context->node = oldnode;
                    CHECK_ERROR0;
                } else {
		    tmp = NULL;
                    newset = xmlXPathNodeSetCreate(NULL);
                    for (i = 0; i < oldset->nodeNr; i++) {
                        ctxt->context->node = oldset->nodeTab[i];
			if ((oldset->nodeTab[i]->type != XML_NAMESPACE_DECL) &&
			    (oldset->nodeTab[i]->doc != NULL))
		            ctxt->context->doc = oldset->nodeTab[i]->doc;
			if (tmp == NULL) {
			    tmp = xmlXPathCacheNewNodeSet(ctxt->context,
				ctxt->context->node);
			} else {
			    xmlXPathNodeSetAddUnique(tmp->nodesetval,
				ctxt->context->node);
			}
                        valuePush(ctxt, tmp);
                        ctxt->context->contextSize = oldset->nodeNr;
                        ctxt->context->proximityPosition = i + 1;
                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			if (ctxt->error != XPATH_EXPRESSION_OK) {
			    xmlXPathFreeNodeSet(newset);
			    xmlXPathFreeObject(obj);
			    return(0);
			}

                        res = valuePop(ctxt);
                        if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
                            xmlXPathNodeSetAdd(newset, oldset->nodeTab[i]);
                        }

                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        if (ctxt->value == tmp) {
                            valuePop(ctxt);
			    xmlXPathNodeSetClear(tmp->nodesetval, 1);
                        } else
			    tmp = NULL;
                        ctxt->context->node = NULL;
                    }
		    if (tmp != NULL)
			xmlXPathReleaseObject(ctxt->context, tmp);
		    xmlXPathReleaseObject(ctxt->context, obj);
                    ctxt->context->node = NULL;
                    ctxt->context->contextSize = -1;
                    ctxt->context->proximityPosition = -1;
		    
		    ctxt->context->doc = oldDoc;
		    valuePush(ctxt,
			xmlXPathCacheWrapNodeSet(ctxt->context, newset));
                }
                ctxt->context->node = oldnode;
                return (total);
            }
        case XPATH_OP_SORT:
            if (op->ch1 != -1)
                total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
	    CHECK_ERROR0;
            if ((ctxt->value != NULL) &&
                (ctxt->value->type == XPATH_NODESET) &&
                (ctxt->value->nodesetval != NULL) &&
		(ctxt->value->nodesetval->nodeNr > 1))
	    {
                xmlXPathNodeSetSort(ctxt->value->nodesetval);
	    }
            return (total);
#ifdef LIBXML_XPTR_ENABLED
        case XPATH_OP_RANGETO:{
                xmlXPathObjectPtr range;
                xmlXPathObjectPtr res, obj;
                xmlXPathObjectPtr tmp;
                xmlLocationSetPtr newlocset = NULL;
		    xmlLocationSetPtr oldlocset;
                xmlNodeSetPtr oldset;
                int i, j;

                if (op->ch1 != -1)
                    total +=
                        xmlXPathCompOpEval(ctxt, &comp->steps[op->ch1]);
                if (op->ch2 == -1)
                    return (total);

                if (ctxt->value->type == XPATH_LOCATIONSET) {
                    CHECK_TYPE0(XPATH_LOCATIONSET);
                    obj = valuePop(ctxt);
                    oldlocset = obj->user;

                    if ((oldlocset == NULL) || (oldlocset->locNr == 0)) {
		        ctxt->context->node = NULL;
                        ctxt->context->contextSize = 0;
                        ctxt->context->proximityPosition = 0;
                        total += xmlXPathCompOpEval(ctxt,&comp->steps[op->ch2]);
                        res = valuePop(ctxt);
                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        valuePush(ctxt, obj);
                        CHECK_ERROR0;
                        return (total);
                    }
                    newlocset = xmlXPtrLocationSetCreate(NULL);

                    for (i = 0; i < oldlocset->locNr; i++) {
                        ctxt->context->node = oldlocset->locTab[i]->user;
                        ctxt->context->contextSize = oldlocset->locNr;
                        ctxt->context->proximityPosition = i + 1;
			tmp = xmlXPathCacheNewNodeSet(ctxt->context,
			    ctxt->context->node);
                        valuePush(ctxt, tmp);

                        if (op->ch2 != -1)
                            total +=
                                xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			if (ctxt->error != XPATH_EXPRESSION_OK) {
			    xmlXPathFreeObject(obj);
			    return(0);
			}

                        res = valuePop(ctxt);
			if (res->type == XPATH_LOCATIONSET) {
			    xmlLocationSetPtr rloc =
			        (xmlLocationSetPtr)res->user;
			    for (j=0; j<rloc->locNr; j++) {
			        range = xmlXPtrNewRange(
				  oldlocset->locTab[i]->user,
				  oldlocset->locTab[i]->index,
				  rloc->locTab[j]->user2,
				  rloc->locTab[j]->index2);
				if (range != NULL) {
				    xmlXPtrLocationSetAdd(newlocset, range);
				}
			    }
			} else {
			    range = xmlXPtrNewRangeNodeObject(
				(xmlNodePtr)oldlocset->locTab[i]->user, res);
                            if (range != NULL) {
                                xmlXPtrLocationSetAdd(newlocset,range);
			    }
                        }

                        if (res != NULL) {
			    xmlXPathReleaseObject(ctxt->context, res);
			}
                        if (ctxt->value == tmp) {
                            res = valuePop(ctxt);
			    xmlXPathReleaseObject(ctxt->context, res);
                        }

                        ctxt->context->node = NULL;
                    }
		} else {	
                    CHECK_TYPE0(XPATH_NODESET);
                    obj = valuePop(ctxt);
                    oldset = obj->nodesetval;
                    ctxt->context->node = NULL;

                    newlocset = xmlXPtrLocationSetCreate(NULL);

                    if (oldset != NULL) {
                        for (i = 0; i < oldset->nodeNr; i++) {
                            ctxt->context->node = oldset->nodeTab[i];
			    tmp = xmlXPathCacheNewNodeSet(ctxt->context,
				ctxt->context->node);
                            valuePush(ctxt, tmp);

                            if (op->ch2 != -1)
                                total +=
                                    xmlXPathCompOpEval(ctxt,
                                                   &comp->steps[op->ch2]);
			    if (ctxt->error != XPATH_EXPRESSION_OK) {
				xmlXPathFreeObject(obj);
				return(0);
			    }

                            res = valuePop(ctxt);
                            range =
                                xmlXPtrNewRangeNodeObject(oldset->nodeTab[i],
                                                      res);
                            if (range != NULL) {
                                xmlXPtrLocationSetAdd(newlocset, range);
                            }

                            if (res != NULL) {
				xmlXPathReleaseObject(ctxt->context, res);
			    }
                            if (ctxt->value == tmp) {
                                res = valuePop(ctxt);
				xmlXPathReleaseObject(ctxt->context, res);
                            }

                            ctxt->context->node = NULL;
                        }
                    }
                }

		xmlXPathReleaseObject(ctxt->context, obj);
                ctxt->context->node = NULL;
                ctxt->context->contextSize = -1;
                ctxt->context->proximityPosition = -1;
                valuePush(ctxt, xmlXPtrWrapLocationSet(newlocset));
                return (total);
            }
#endif 
    }
    xmlGenericError(xmlGenericErrorContext,
                    "XPath: unknown precompiled operation %d\n", op->op);
    return (total);
}

static int
xmlXPathCompOpEvalToBoolean(xmlXPathParserContextPtr ctxt,
			    xmlXPathStepOpPtr op,
			    int isPredicate)
{
    xmlXPathObjectPtr resObj = NULL;

start:
    
    switch (op->op) {
        case XPATH_OP_END:
            return (0);
	case XPATH_OP_VALUE:
	    resObj = (xmlXPathObjectPtr) op->value4;
	    if (isPredicate)
		return(xmlXPathEvaluatePredicateResult(ctxt, resObj));
	    return(xmlXPathCastToBoolean(resObj));
	case XPATH_OP_SORT:
            if (op->ch1 != -1) {
		op = &ctxt->comp->steps[op->ch1];
		goto start;
	    }
	    return(0);
	case XPATH_OP_COLLECT:
	    if (op->ch1 == -1)
		return(0);

            xmlXPathCompOpEval(ctxt, &ctxt->comp->steps[op->ch1]);
	    if (ctxt->error != XPATH_EXPRESSION_OK)
		return(-1);

            xmlXPathNodeCollectAndTest(ctxt, op, NULL, NULL, 1);
	    if (ctxt->error != XPATH_EXPRESSION_OK)
		return(-1);

	    resObj = valuePop(ctxt);
	    if (resObj == NULL)
		return(-1);
	    break;
	default:
	    xmlXPathCompOpEval(ctxt, op);
	    if (ctxt->error != XPATH_EXPRESSION_OK)
		return(-1);

	    resObj = valuePop(ctxt);
	    if (resObj == NULL)
		return(-1);
	    break;
    }

    if (resObj) {
	int res;

	if (resObj->type == XPATH_BOOLEAN) {
	    res = resObj->boolval;
	} else if (isPredicate) {
	    res = xmlXPathEvaluatePredicateResult(ctxt, resObj);
	} else {
	    res = xmlXPathCastToBoolean(resObj);
	}
	xmlXPathReleaseObject(ctxt->context, resObj);
	return(res);
    }

    return(0);
}

#ifdef XPATH_STREAMING
static int
xmlXPathRunStreamEval(xmlXPathContextPtr ctxt, xmlPatternPtr comp,
		      xmlXPathObjectPtr *resultSeq, int toBool)
{
    int max_depth, min_depth;
    int from_root;
    int ret, depth;
    int eval_all_nodes;
    xmlNodePtr cur = NULL, limit = NULL;
    xmlStreamCtxtPtr patstream = NULL;

    int nb_nodes = 0;

    if ((ctxt == NULL) || (comp == NULL))
        return(-1);
    max_depth = xmlPatternMaxDepth(comp);
    if (max_depth == -1)
        return(-1);
    if (max_depth == -2)
        max_depth = 10000;
    min_depth = xmlPatternMinDepth(comp);
    if (min_depth == -1)
        return(-1);
    from_root = xmlPatternFromRoot(comp);
    if (from_root < 0)
        return(-1);
#if 0
    printf("stream eval: depth %d from root %d\n", max_depth, from_root);
#endif

    if (! toBool) {
	if (resultSeq == NULL)
	    return(-1);
	*resultSeq = xmlXPathCacheNewNodeSet(ctxt, NULL);
	if (*resultSeq == NULL)
	    return(-1);
    }

    if (min_depth == 0) {
	if (from_root) {
	    
	    if (toBool)
		return(1);
	    xmlXPathNodeSetAddUnique((*resultSeq)->nodesetval,
		(xmlNodePtr) ctxt->doc);
	} else {
	    
	    if (toBool)
		return(1);
	    xmlXPathNodeSetAddUnique((*resultSeq)->nodesetval, ctxt->node);
	}
    }
    if (max_depth == 0) {
	return(0);
    }

    if (from_root) {
        cur = (xmlNodePtr)ctxt->doc;
    } else if (ctxt->node != NULL) {
        switch (ctxt->node->type) {
            case XML_ELEMENT_NODE:
            case XML_DOCUMENT_NODE:
            case XML_DOCUMENT_FRAG_NODE:
            case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
            case XML_DOCB_DOCUMENT_NODE:
#endif
	        cur = ctxt->node;
		break;
            case XML_ATTRIBUTE_NODE:
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
            case XML_ENTITY_REF_NODE:
            case XML_ENTITY_NODE:
            case XML_PI_NODE:
            case XML_COMMENT_NODE:
            case XML_NOTATION_NODE:
            case XML_DTD_NODE:
            case XML_DOCUMENT_TYPE_NODE:
            case XML_ELEMENT_DECL:
            case XML_ATTRIBUTE_DECL:
            case XML_ENTITY_DECL:
            case XML_NAMESPACE_DECL:
            case XML_XINCLUDE_START:
            case XML_XINCLUDE_END:
		break;
	}
	limit = cur;
    }
    if (cur == NULL) {
        return(0);
    }

    patstream = xmlPatternGetStreamCtxt(comp);
    if (patstream == NULL) {
	return(0);
    }

    eval_all_nodes = xmlStreamWantsAnyNode(patstream);

    if (from_root) {
	ret = xmlStreamPush(patstream, NULL, NULL);
	if (ret < 0) {
	} else if (ret == 1) {
	    if (toBool)
		goto return_1;
	    xmlXPathNodeSetAddUnique((*resultSeq)->nodesetval, cur);
	}
    }
    depth = 0;
    goto scan_children;
next_node:
    do {
        nb_nodes++;

	switch (cur->type) {
	    case XML_ELEMENT_NODE:
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
	    case XML_COMMENT_NODE:
	    case XML_PI_NODE:
		if (cur->type == XML_ELEMENT_NODE) {
		    ret = xmlStreamPush(patstream, cur->name,
				(cur->ns ? cur->ns->href : NULL));
		} else if (eval_all_nodes)
		    ret = xmlStreamPushNode(patstream, NULL, NULL, cur->type);
		else
		    break;

		if (ret < 0) {
		    
		} else if (ret == 1) {
		    if (toBool)
			goto return_1;
		    xmlXPathNodeSetAddUnique((*resultSeq)->nodesetval, cur);
		}
		if ((cur->children == NULL) || (depth >= max_depth)) {
		    ret = xmlStreamPop(patstream);
		    while (cur->next != NULL) {
			cur = cur->next;
			if ((cur->type != XML_ENTITY_DECL) &&
			    (cur->type != XML_DTD_NODE))
			    goto next_node;
		    }
		}
	    default:
		break;
	}

scan_children:
	if ((cur->children != NULL) && (depth < max_depth)) {
	    if (cur->children->type != XML_ENTITY_DECL) {
		cur = cur->children;
		depth++;
		if (cur->type != XML_DTD_NODE)
		    continue;
	    }
	}

	if (cur == limit)
	    break;

	while (cur->next != NULL) {
	    cur = cur->next;
	    if ((cur->type != XML_ENTITY_DECL) &&
		(cur->type != XML_DTD_NODE))
		goto next_node;
	}

	do {
	    cur = cur->parent;
	    depth--;
	    if ((cur == NULL) || (cur == limit))
	        goto done;
	    if (cur->type == XML_ELEMENT_NODE) {
		ret = xmlStreamPop(patstream);
	    } else if ((eval_all_nodes) &&
		((cur->type == XML_TEXT_NODE) ||
		 (cur->type == XML_CDATA_SECTION_NODE) ||
		 (cur->type == XML_COMMENT_NODE) ||
		 (cur->type == XML_PI_NODE)))
	    {
		ret = xmlStreamPop(patstream);
	    }
	    if (cur->next != NULL) {
		cur = cur->next;
		break;
	    }
	} while (cur != NULL);

    } while ((cur != NULL) && (depth >= 0));

done:

#if 0
    printf("stream eval: checked %d nodes selected %d\n",
           nb_nodes, retObj->nodesetval->nodeNr);
#endif

    if (patstream)
	xmlFreeStreamCtxt(patstream);
    return(0);

return_1:
    if (patstream)
	xmlFreeStreamCtxt(patstream);
    return(1);
}
#endif 

static int
xmlXPathRunEval(xmlXPathParserContextPtr ctxt, int toBool)
{
    xmlXPathCompExprPtr comp;

    if ((ctxt == NULL) || (ctxt->comp == NULL))
	return(-1);

    if (ctxt->valueTab == NULL) {
	
	ctxt->valueTab = (xmlXPathObjectPtr *)
			 xmlMalloc(10 * sizeof(xmlXPathObjectPtr));
	if (ctxt->valueTab == NULL) {
	    xmlXPathPErrMemory(ctxt, "creating evaluation context\n");
	    xmlFree(ctxt);
	}
	ctxt->valueNr = 0;
	ctxt->valueMax = 10;
	ctxt->value = NULL;
    }
#ifdef XPATH_STREAMING
    if (ctxt->comp->stream) {
	int res;

	if (toBool) {
	    res = xmlXPathRunStreamEval(ctxt->context,
		ctxt->comp->stream, NULL, 1);
	    if (res != -1)
		return(res);
	} else {
	    xmlXPathObjectPtr resObj = NULL;

	    res = xmlXPathRunStreamEval(ctxt->context,
		ctxt->comp->stream, &resObj, 0);

	    if ((res != -1) && (resObj != NULL)) {
		valuePush(ctxt, resObj);
		return(0);
	    }
	    if (resObj != NULL)
		xmlXPathReleaseObject(ctxt->context, resObj);
	}
    }
#endif
    comp = ctxt->comp;
    if (comp->last < 0) {
	xmlGenericError(xmlGenericErrorContext,
	    "xmlXPathRunEval: last is less than zero\n");
	return(-1);
    }
    if (toBool)
	return(xmlXPathCompOpEvalToBoolean(ctxt,
	    &comp->steps[comp->last], 0));
    else
	xmlXPathCompOpEval(ctxt, &comp->steps[comp->last]);

    return(0);
}


int
xmlXPathEvalPredicate(xmlXPathContextPtr ctxt, xmlXPathObjectPtr res) {
    if ((ctxt == NULL) || (res == NULL)) return(0);
    switch (res->type) {
        case XPATH_BOOLEAN:
	    return(res->boolval);
        case XPATH_NUMBER:
	    return(res->floatval == ctxt->proximityPosition);
        case XPATH_NODESET:
        case XPATH_XSLT_TREE:
	    if (res->nodesetval == NULL)
		return(0);
	    return(res->nodesetval->nodeNr != 0);
        case XPATH_STRING:
	    return((res->stringval != NULL) &&
	           (xmlStrlen(res->stringval) != 0));
        default:
	    STRANGE
    }
    return(0);
}

int
xmlXPathEvaluatePredicateResult(xmlXPathParserContextPtr ctxt,
                                xmlXPathObjectPtr res) {
    if ((ctxt == NULL) || (res == NULL)) return(0);
    switch (res->type) {
        case XPATH_BOOLEAN:
	    return(res->boolval);
        case XPATH_NUMBER:
#if defined(__BORLANDC__) || (defined(_MSC_VER) && (_MSC_VER == 1200))
	    return((res->floatval == ctxt->context->proximityPosition) &&
	           (!xmlXPathIsNaN(res->floatval))); 
#else
	    return(res->floatval == ctxt->context->proximityPosition);
#endif
        case XPATH_NODESET:
        case XPATH_XSLT_TREE:
	    if (res->nodesetval == NULL)
		return(0);
	    return(res->nodesetval->nodeNr != 0);
        case XPATH_STRING:
	    return((res->stringval != NULL) && (res->stringval[0] != 0));
#ifdef LIBXML_XPTR_ENABLED
	case XPATH_LOCATIONSET:{
	    xmlLocationSetPtr ptr = res->user;
	    if (ptr == NULL)
	        return(0);
	    return (ptr->locNr != 0);
	    }
#endif
        default:
	    STRANGE
    }
    return(0);
}

#ifdef XPATH_STREAMING
static xmlXPathCompExprPtr
xmlXPathTryStreamCompile(xmlXPathContextPtr ctxt, const xmlChar *str) {
    xmlPatternPtr stream;
    xmlXPathCompExprPtr comp;
    xmlDictPtr dict = NULL;
    const xmlChar **namespaces = NULL;
    xmlNsPtr ns;
    int i, j;

    if ((!xmlStrchr(str, '[')) && (!xmlStrchr(str, '(')) &&
        (!xmlStrchr(str, '@'))) {
	const xmlChar *tmp;

	tmp = xmlStrchr(str, ':');
	if ((tmp != NULL) &&
	    ((ctxt == NULL) || (ctxt->nsNr == 0) || (tmp[1] == ':')))
	    return(NULL);

	if (ctxt != NULL) {
	    dict = ctxt->dict;
	    if (ctxt->nsNr > 0) {
		namespaces = xmlMalloc(2 * (ctxt->nsNr + 1) * sizeof(xmlChar*));
		if (namespaces == NULL) {
		    xmlXPathErrMemory(ctxt, "allocating namespaces array\n");
		    return(NULL);
		}
		for (i = 0, j = 0; (j < ctxt->nsNr); j++) {
		    ns = ctxt->namespaces[j];
		    namespaces[i++] = ns->href;
		    namespaces[i++] = ns->prefix;
		}
		namespaces[i++] = NULL;
		namespaces[i] = NULL;
	    }
	}

	stream = xmlPatterncompile(str, dict, XML_PATTERN_XPATH,
			&namespaces[0]);
	if (namespaces != NULL) {
	    xmlFree((xmlChar **)namespaces);
	}
	if ((stream != NULL) && (xmlPatternStreamable(stream) == 1)) {
	    comp = xmlXPathNewCompExpr();
	    if (comp == NULL) {
		xmlXPathErrMemory(ctxt, "allocating streamable expression\n");
		return(NULL);
	    }
	    comp->stream = stream;
	    comp->dict = dict;
	    if (comp->dict)
		xmlDictReference(comp->dict);
	    return(comp);
	}
	xmlFreePattern(stream);
    }
    return(NULL);
}
#endif 

static int
xmlXPathCanRewriteDosExpression(xmlChar *expr)
{
    if (expr == NULL)
	return(0);
    do {
        if ((*expr == '/') && (*(++expr) == '/'))
	    return(1);
    } while (*expr++);
    return(0);
}
static void
xmlXPathRewriteDOSExpression(xmlXPathCompExprPtr comp, xmlXPathStepOpPtr op)
{
    if (op->ch1 != -1) {
	if ((op->op == XPATH_OP_COLLECT ) &&
	    ((xmlXPathAxisVal) op->value == AXIS_CHILD ) &&
	    ((xmlXPathTestVal) op->value2 == NODE_TEST_NAME ) &&
	    ((xmlXPathTypeVal) op->value3 == NODE_TYPE_NODE ))
	{
	    xmlXPathStepOpPtr prevop = &comp->steps[op->ch1];

	    if ((prevop->op == XPATH_OP_COLLECT ) &&
		(prevop->ch1 != -1) &&
		((xmlXPathAxisVal) prevop->value ==
		    AXIS_DESCENDANT_OR_SELF) &&
		(prevop->ch2 == -1) &&
		((xmlXPathTestVal) prevop->value2 == NODE_TEST_TYPE) &&
		((xmlXPathTypeVal) prevop->value3 == NODE_TYPE_NODE) &&
		(comp->steps[prevop->ch1].op == XPATH_OP_ROOT))
	    {
		op->ch1 = prevop->ch1;
		op->rewriteType = XP_REWRITE_DOS_CHILD_ELEM;
	    }
	}
	if (op->ch1 != -1)
	    xmlXPathRewriteDOSExpression(comp, &comp->steps[op->ch1]);
    }
    if (op->ch2 != -1)
	xmlXPathRewriteDOSExpression(comp, &comp->steps[op->ch2]);
}

xmlXPathCompExprPtr
xmlXPathCtxtCompile(xmlXPathContextPtr ctxt, const xmlChar *str) {
    xmlXPathParserContextPtr pctxt;
    xmlXPathCompExprPtr comp;

#ifdef XPATH_STREAMING
    comp = xmlXPathTryStreamCompile(ctxt, str);
    if (comp != NULL)
        return(comp);
#endif

    xmlXPathInit();

    pctxt = xmlXPathNewParserContext(str, ctxt);
    if (pctxt == NULL)
        return NULL;
    xmlXPathCompileExpr(pctxt, 1);

    if( pctxt->error != XPATH_EXPRESSION_OK )
    {
        xmlXPathFreeParserContext(pctxt);
        return(NULL);
    }

    if (*pctxt->cur != 0) {
	xmlXPatherror(pctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
	comp = NULL;
    } else {
	comp = pctxt->comp;
	pctxt->comp = NULL;
    }
    xmlXPathFreeParserContext(pctxt);

    if (comp != NULL) {
	comp->expr = xmlStrdup(str);
#ifdef DEBUG_EVAL_COUNTS
	comp->string = xmlStrdup(str);
	comp->nb = 0;
#endif
	if ((comp->expr != NULL) &&
	    (comp->nbStep > 2) &&
	    (comp->last >= 0) &&
	    (xmlXPathCanRewriteDosExpression(comp->expr) == 1))
	{
	    xmlXPathRewriteDOSExpression(comp, &comp->steps[comp->last]);
	}
    }
    return(comp);
}

xmlXPathCompExprPtr
xmlXPathCompile(const xmlChar *str) {
    return(xmlXPathCtxtCompile(NULL, str));
}

static int
xmlXPathCompiledEvalInternal(xmlXPathCompExprPtr comp,
			     xmlXPathContextPtr ctxt,
			     xmlXPathObjectPtr *resObj,
			     int toBool)
{
    xmlXPathParserContextPtr pctxt;
#ifndef LIBXML_THREAD_ENABLED
    static int reentance = 0;
#endif
    int res;

    CHECK_CTXT_NEG(ctxt)

    if (comp == NULL)
	return(-1);
    xmlXPathInit();

#ifndef LIBXML_THREAD_ENABLED
    reentance++;
    if (reentance > 1)
	xmlXPathDisableOptimizer = 1;
#endif

#ifdef DEBUG_EVAL_COUNTS
    comp->nb++;
    if ((comp->string != NULL) && (comp->nb > 100)) {
	fprintf(stderr, "100 x %s\n", comp->string);
	comp->nb = 0;
    }
#endif
    pctxt = xmlXPathCompParserContext(comp, ctxt);
    res = xmlXPathRunEval(pctxt, toBool);

    if (resObj) {
	if (pctxt->value == NULL) {
	    xmlGenericError(xmlGenericErrorContext,
		"xmlXPathCompiledEval: evaluation failed\n");
	    *resObj = NULL;
	} else {
	    *resObj = valuePop(pctxt);
	}
    }

    if (pctxt->valueNr > 0) {
	xmlXPathObjectPtr tmp;
	int stack = 0;

	do {
	    tmp = valuePop(pctxt);
	    if (tmp != NULL) {
		stack++;
		xmlXPathReleaseObject(ctxt, tmp);
	    }
	} while (tmp != NULL);
	if ((stack != 0) &&
	    ((toBool) || ((resObj) && (*resObj))))
	{
	    xmlGenericError(xmlGenericErrorContext,
		"xmlXPathCompiledEval: %d objects left on the stack.\n",
		stack);
	}
    }

    if ((pctxt->error != XPATH_EXPRESSION_OK) && (resObj) && (*resObj)) {
	xmlXPathFreeObject(*resObj);
	*resObj = NULL;
    }
    pctxt->comp = NULL;
    xmlXPathFreeParserContext(pctxt);
#ifndef LIBXML_THREAD_ENABLED
    reentance--;
#endif

    return(res);
}

xmlXPathObjectPtr
xmlXPathCompiledEval(xmlXPathCompExprPtr comp, xmlXPathContextPtr ctx)
{
    xmlXPathObjectPtr res = NULL;

    xmlXPathCompiledEvalInternal(comp, ctx, &res, 0);
    return(res);
}

int
xmlXPathCompiledEvalToBoolean(xmlXPathCompExprPtr comp,
			      xmlXPathContextPtr ctxt)
{
    return(xmlXPathCompiledEvalInternal(comp, ctxt, NULL, 1));
}

void
xmlXPathEvalExpr(xmlXPathParserContextPtr ctxt) {
#ifdef XPATH_STREAMING
    xmlXPathCompExprPtr comp;
#endif

    if (ctxt == NULL) return;

#ifdef XPATH_STREAMING
    comp = xmlXPathTryStreamCompile(ctxt->context, ctxt->base);
    if (comp != NULL) {
        if (ctxt->comp != NULL)
	    xmlXPathFreeCompExpr(ctxt->comp);
        ctxt->comp = comp;
	if (ctxt->cur != NULL)
	    while (*ctxt->cur != 0) ctxt->cur++;
    } else
#endif
    {
	xmlXPathCompileExpr(ctxt, 1);
	if ((ctxt->error == XPATH_EXPRESSION_OK) &&
	    (ctxt->comp != NULL) &&
	    (ctxt->base != NULL) &&
	    (ctxt->comp->nbStep > 2) &&
	    (ctxt->comp->last >= 0) &&
	    (xmlXPathCanRewriteDosExpression((xmlChar *) ctxt->base) == 1))
	{
	    xmlXPathRewriteDOSExpression(ctxt->comp,
		&ctxt->comp->steps[ctxt->comp->last]);
	}
    }
    CHECK_ERROR;
    xmlXPathRunEval(ctxt, 0);
}

xmlXPathObjectPtr
xmlXPathEval(const xmlChar *str, xmlXPathContextPtr ctx) {
    xmlXPathParserContextPtr ctxt;
    xmlXPathObjectPtr res, tmp, init = NULL;
    int stack = 0;

    CHECK_CTXT(ctx)

    xmlXPathInit();

    ctxt = xmlXPathNewParserContext(str, ctx);
    if (ctxt == NULL)
        return NULL;
    xmlXPathEvalExpr(ctxt);

    if (ctxt->value == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlXPathEval: evaluation failed\n");
	res = NULL;
    } else if ((*ctxt->cur != 0) && (ctxt->comp != NULL)
#ifdef XPATH_STREAMING
            && (ctxt->comp->stream == NULL)
#endif
	      ) {
	xmlXPatherror(ctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
	res = NULL;
    } else {
	res = valuePop(ctxt);
    }

    do {
        tmp = valuePop(ctxt);
	if (tmp != NULL) {
	    if (tmp != init)
		stack++;
	    xmlXPathReleaseObject(ctx, tmp);
        }
    } while (tmp != NULL);
    if ((stack != 0) && (res != NULL)) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlXPathEval: %d object left on the stack\n",
	        stack);
    }
    if (ctxt->error != XPATH_EXPRESSION_OK) {
	xmlXPathFreeObject(res);
	res = NULL;
    }

    xmlXPathFreeParserContext(ctxt);
    return(res);
}

xmlXPathObjectPtr
xmlXPathEvalExpression(const xmlChar *str, xmlXPathContextPtr ctxt) {
    xmlXPathParserContextPtr pctxt;
    xmlXPathObjectPtr res, tmp;
    int stack = 0;

    CHECK_CTXT(ctxt)

    xmlXPathInit();

    pctxt = xmlXPathNewParserContext(str, ctxt);
    if (pctxt == NULL)
        return NULL;
    xmlXPathEvalExpr(pctxt);

    if ((*pctxt->cur != 0) || (pctxt->error != XPATH_EXPRESSION_OK)) {
	xmlXPatherror(pctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
	res = NULL;
    } else {
	res = valuePop(pctxt);
    }
    do {
        tmp = valuePop(pctxt);
	if (tmp != NULL) {
	    xmlXPathReleaseObject(ctxt, tmp);
	    stack++;
	}
    } while (tmp != NULL);
    if ((stack != 0) && (res != NULL)) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlXPathEvalExpression: %d object left on the stack\n",
	        stack);
    }
    xmlXPathFreeParserContext(pctxt);
    return(res);
}

static void
xmlXPathEscapeUriFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr str;
    int escape_reserved;
    xmlBufferPtr target;
    xmlChar *cptr;
    xmlChar escape[4];

    CHECK_ARITY(2);

    escape_reserved = xmlXPathPopBoolean(ctxt);

    CAST_TO_STRING;
    str = valuePop(ctxt);

    target = xmlBufferCreate();

    escape[0] = '%';
    escape[3] = 0;

    if (target) {
	for (cptr = str->stringval; *cptr; cptr++) {
	    if ((*cptr >= 'A' && *cptr <= 'Z') ||
		(*cptr >= 'a' && *cptr <= 'z') ||
		(*cptr >= '0' && *cptr <= '9') ||
		*cptr == '-' || *cptr == '_' || *cptr == '.' ||
		*cptr == '!' || *cptr == '~' || *cptr == '*' ||
		*cptr == '\''|| *cptr == '(' || *cptr == ')' ||
		(*cptr == '%' &&
		 ((cptr[1] >= 'A' && cptr[1] <= 'F') ||
		  (cptr[1] >= 'a' && cptr[1] <= 'f') ||
		  (cptr[1] >= '0' && cptr[1] <= '9')) &&
		 ((cptr[2] >= 'A' && cptr[2] <= 'F') ||
		  (cptr[2] >= 'a' && cptr[2] <= 'f') ||
		  (cptr[2] >= '0' && cptr[2] <= '9'))) ||
		(!escape_reserved &&
		 (*cptr == ';' || *cptr == '/' || *cptr == '?' ||
		  *cptr == ':' || *cptr == '@' || *cptr == '&' ||
		  *cptr == '=' || *cptr == '+' || *cptr == '$' ||
		  *cptr == ','))) {
		xmlBufferAdd(target, cptr, 1);
	    } else {
		if ((*cptr >> 4) < 10)
		    escape[1] = '0' + (*cptr >> 4);
		else
		    escape[1] = 'A' - 10 + (*cptr >> 4);
		if ((*cptr & 0xF) < 10)
		    escape[2] = '0' + (*cptr & 0xF);
		else
		    escape[2] = 'A' - 10 + (*cptr & 0xF);

		xmlBufferAdd(target, &escape[0], 3);
	    }
	}
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
    xmlXPathReleaseObject(ctxt->context, str);
}

void
xmlXPathRegisterAllFunctions(xmlXPathContextPtr ctxt)
{
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"boolean",
                         xmlXPathBooleanFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"ceiling",
                         xmlXPathCeilingFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"count",
                         xmlXPathCountFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"concat",
                         xmlXPathConcatFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"contains",
                         xmlXPathContainsFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"id",
                         xmlXPathIdFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"false",
                         xmlXPathFalseFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"floor",
                         xmlXPathFloorFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"last",
                         xmlXPathLastFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"lang",
                         xmlXPathLangFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"local-name",
                         xmlXPathLocalNameFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"not",
                         xmlXPathNotFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"name",
                         xmlXPathNameFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"namespace-uri",
                         xmlXPathNamespaceURIFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"normalize-space",
                         xmlXPathNormalizeFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"number",
                         xmlXPathNumberFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"position",
                         xmlXPathPositionFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"round",
                         xmlXPathRoundFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"string",
                         xmlXPathStringFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"string-length",
                         xmlXPathStringLengthFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"starts-with",
                         xmlXPathStartsWithFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"substring",
                         xmlXPathSubstringFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"substring-before",
                         xmlXPathSubstringBeforeFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"substring-after",
                         xmlXPathSubstringAfterFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"sum",
                         xmlXPathSumFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"true",
                         xmlXPathTrueFunction);
    xmlXPathRegisterFunc(ctxt, (const xmlChar *)"translate",
                         xmlXPathTranslateFunction);

    xmlXPathRegisterFuncNS(ctxt, (const xmlChar *)"escape-uri",
	 (const xmlChar *)"http://www.w3.org/2002/08/xquery-functions",
                         xmlXPathEscapeUriFunction);
}

#endif 
#define bottom_xpath
#include "elfgcchack.h"
