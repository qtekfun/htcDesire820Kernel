/*
 * xpointer.c : Code to handle XML Pointer
 *
 * Base implementation was made accordingly to
 * W3C Candidate Recommendation 7 June 2000
 * http://www.w3.org/TR/2000/CR-xptr-20000607
 *
 * Added support for the element() scheme described in:
 * W3C Proposed Recommendation 13 November 2002
 * http://www.w3.org/TR/2002/PR-xptr-element-20021113/  
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"


#include <string.h>
#include <libxml/xpointer.h>
#include <libxml/xmlmemory.h>
#include <libxml/parserInternals.h>
#include <libxml/uri.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlerror.h>
#include <libxml/globals.h>

#ifdef LIBXML_XPTR_ENABLED

#define XPTR_XMLNS_SCHEME

#ifdef DEBUG_RANGES
#ifdef LIBXML_DEBUG_ENABLED
#include <libxml/debugXML.h>
#endif
#endif

#define TODO 								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);

#define STRANGE 							\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Internal error at %s:%d\n",				\
            __FILE__, __LINE__);


static void
xmlXPtrErrMemory(const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_XPOINTER,
		    XML_ERR_NO_MEMORY, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Memory allocation failed : %s\n", extra);
}

static void
xmlXPtrErr(xmlXPathParserContextPtr ctxt, int error,
           const char * msg, const xmlChar *extra)
{
    if (ctxt != NULL)
        ctxt->error = error;
    if ((ctxt == NULL) || (ctxt->context == NULL)) {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, NULL, XML_FROM_XPOINTER, error,
			XML_ERR_ERROR, NULL, 0,
			(const char *) extra, NULL, NULL, 0, 0,
			msg, extra);
	return;
    }
    ctxt->context->lastError.domain = XML_FROM_XPOINTER;
    ctxt->context->lastError.code = error;
    ctxt->context->lastError.level = XML_ERR_ERROR;
    ctxt->context->lastError.str1 = (char *) xmlStrdup(ctxt->base);
    ctxt->context->lastError.int1 = ctxt->cur - ctxt->base;
    ctxt->context->lastError.node = ctxt->context->debugNode;
    if (ctxt->context->error != NULL) {
	ctxt->context->error(ctxt->context->userData,
	                     &ctxt->context->lastError);
    } else {
	__xmlRaiseError(NULL, NULL, NULL,
			NULL, ctxt->context->debugNode, XML_FROM_XPOINTER,
			error, XML_ERR_ERROR, NULL, 0,
			(const char *) extra, (const char *) ctxt->base, NULL,
			ctxt->cur - ctxt->base, 0,
			msg, extra);
    }
}

xmlNodePtr xmlXPtrAdvanceNode(xmlNodePtr cur, int *level);
static int
xmlXPtrGetArity(xmlNodePtr cur) {
    int i;
    if (cur == NULL) 
	return(-1);
    cur = cur->children;
    for (i = 0;cur != NULL;cur = cur->next) {
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_DOCUMENT_NODE) ||
	    (cur->type == XML_HTML_DOCUMENT_NODE)) {
	    i++;
	}
    }
    return(i);
}

static int
xmlXPtrGetIndex(xmlNodePtr cur) {
    int i;
    if (cur == NULL) 
	return(-1);
    for (i = 1;cur != NULL;cur = cur->prev) {
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_DOCUMENT_NODE) ||
	    (cur->type == XML_HTML_DOCUMENT_NODE)) {
	    i++;
	}
    }
    return(i);
}

static xmlNodePtr
xmlXPtrGetNthChild(xmlNodePtr cur, int no) {
    int i;
    if (cur == NULL) 
	return(cur);
    cur = cur->children;
    for (i = 0;i <= no;cur = cur->next) {
	if (cur == NULL) 
	    return(cur);
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_DOCUMENT_NODE) ||
	    (cur->type == XML_HTML_DOCUMENT_NODE)) {
	    i++;
	    if (i == no)
		break;
	}
    }
    return(cur);
}


static int
xmlXPtrCmpPoints(xmlNodePtr node1, int index1, xmlNodePtr node2, int index2) {
    if ((node1 == NULL) || (node2 == NULL))
	return(-2);
    if (node1 == node2) {
	if (index1 < index2)
	    return(1);
	if (index1 > index2)
	    return(-1);
	return(0);
    }
    return(xmlXPathCmpNodes(node1, node2));
}

static xmlXPathObjectPtr
xmlXPtrNewPoint(xmlNodePtr node, int indx) {
    xmlXPathObjectPtr ret;

    if (node == NULL)
	return(NULL);
    if (indx < 0)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating point");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_POINT;
    ret->user = (void *) node;
    ret->index = indx;
    return(ret);
}

static void
xmlXPtrRangeCheckOrder(xmlXPathObjectPtr range) {
    int tmp;
    xmlNodePtr tmp2;
    if (range == NULL)
	return;
    if (range->type != XPATH_RANGE)
	return;
    if (range->user2 == NULL)
	return;
    tmp = xmlXPtrCmpPoints(range->user, range->index,
	                     range->user2, range->index2);
    if (tmp == -1) {
	tmp2 = range->user;
	range->user = range->user2;
	range->user2 = tmp2;
	tmp = range->index;
	range->index = range->index2;
	range->index2 = tmp;
    }
}

static int
xmlXPtrRangesEqual(xmlXPathObjectPtr range1, xmlXPathObjectPtr range2) {
    if (range1 == range2)
	return(1);
    if ((range1 == NULL) || (range2 == NULL))
	return(0);
    if (range1->type != range2->type)
	return(0);
    if (range1->type != XPATH_RANGE)
	return(0);
    if (range1->user != range2->user)
	return(0);
    if (range1->index != range2->index)
	return(0);
    if (range1->user2 != range2->user2)
	return(0);
    if (range1->index2 != range2->index2)
	return(0);
    return(1);
}

xmlXPathObjectPtr
xmlXPtrNewRange(xmlNodePtr start, int startindex,
	        xmlNodePtr end, int endindex) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);
    if (end == NULL)
	return(NULL);
    if (startindex < 0)
	return(NULL);
    if (endindex < 0)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start;
    ret->index = startindex;
    ret->user2 = end;
    ret->index2 = endindex;
    xmlXPtrRangeCheckOrder(ret);
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewRangePoints(xmlXPathObjectPtr start, xmlXPathObjectPtr end) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);
    if (end == NULL)
	return(NULL);
    if (start->type != XPATH_POINT)
	return(NULL);
    if (end->type != XPATH_POINT)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start->user;
    ret->index = start->index;
    ret->user2 = end->user;
    ret->index2 = end->index;
    xmlXPtrRangeCheckOrder(ret);
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewRangePointNode(xmlXPathObjectPtr start, xmlNodePtr end) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);
    if (end == NULL)
	return(NULL);
    if (start->type != XPATH_POINT)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start->user;
    ret->index = start->index;
    ret->user2 = end;
    ret->index2 = -1;
    xmlXPtrRangeCheckOrder(ret);
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewRangeNodePoint(xmlNodePtr start, xmlXPathObjectPtr end) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);
    if (end == NULL)
	return(NULL);
    if (start->type != XPATH_POINT)
	return(NULL);
    if (end->type != XPATH_POINT)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start;
    ret->index = -1;
    ret->user2 = end->user;
    ret->index2 = end->index;
    xmlXPtrRangeCheckOrder(ret);
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewRangeNodes(xmlNodePtr start, xmlNodePtr end) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);
    if (end == NULL)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start;
    ret->index = -1;
    ret->user2 = end;
    ret->index2 = -1;
    xmlXPtrRangeCheckOrder(ret);
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewCollapsedRange(xmlNodePtr start) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start;
    ret->index = -1;
    ret->user2 = NULL;
    ret->index2 = -1;
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewRangeNodeObject(xmlNodePtr start, xmlXPathObjectPtr end) {
    xmlXPathObjectPtr ret;

    if (start == NULL)
	return(NULL);
    if (end == NULL)
	return(NULL);
    switch (end->type) {
	case XPATH_POINT:
	case XPATH_RANGE:
	    break;
	case XPATH_NODESET:
	    if (end->nodesetval->nodeNr <= 0)
		return(NULL);
	    break;
	default:
	    
	    return(NULL);
    }

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating range");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_RANGE;
    ret->user = start;
    ret->index = -1;
    switch (end->type) {
	case XPATH_POINT:
	    ret->user2 = end->user;
	    ret->index2 = end->index;
	    break;
	case XPATH_RANGE:
	    ret->user2 = end->user2;
	    ret->index2 = end->index2;
	    break;
	case XPATH_NODESET: {
	    ret->user2 = end->nodesetval->nodeTab[end->nodesetval->nodeNr - 1];
	    ret->index2 = -1;
	    break;
	}
	default:
	    STRANGE
	    return(NULL);
    }
    xmlXPtrRangeCheckOrder(ret);
    return(ret);
}

#define XML_RANGESET_DEFAULT	10

xmlLocationSetPtr
xmlXPtrLocationSetCreate(xmlXPathObjectPtr val) {
    xmlLocationSetPtr ret;

    ret = (xmlLocationSetPtr) xmlMalloc(sizeof(xmlLocationSet));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating locationset");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlLocationSet));
    if (val != NULL) {
        ret->locTab = (xmlXPathObjectPtr *) xmlMalloc(XML_RANGESET_DEFAULT *
					     sizeof(xmlXPathObjectPtr));
	if (ret->locTab == NULL) {
	    xmlXPtrErrMemory("allocating locationset");
	    xmlFree(ret);
	    return(NULL);
	}
	memset(ret->locTab, 0 ,
	       XML_RANGESET_DEFAULT * (size_t) sizeof(xmlXPathObjectPtr));
        ret->locMax = XML_RANGESET_DEFAULT;
	ret->locTab[ret->locNr++] = val;
    }
    return(ret);
}

void
xmlXPtrLocationSetAdd(xmlLocationSetPtr cur, xmlXPathObjectPtr val) {
    int i;

    if ((cur == NULL) || (val == NULL)) return;

    for (i = 0;i < cur->locNr;i++) {
	if (xmlXPtrRangesEqual(cur->locTab[i], val)) {
	    xmlXPathFreeObject(val);
	    return;
	}
    }

    if (cur->locMax == 0) {
        cur->locTab = (xmlXPathObjectPtr *) xmlMalloc(XML_RANGESET_DEFAULT *
					     sizeof(xmlXPathObjectPtr));
	if (cur->locTab == NULL) {
	    xmlXPtrErrMemory("adding location to set");
	    return;
	}
	memset(cur->locTab, 0 ,
	       XML_RANGESET_DEFAULT * (size_t) sizeof(xmlXPathObjectPtr));
        cur->locMax = XML_RANGESET_DEFAULT;
    } else if (cur->locNr == cur->locMax) {
        xmlXPathObjectPtr *temp;

        cur->locMax *= 2;
	temp = (xmlXPathObjectPtr *) xmlRealloc(cur->locTab, cur->locMax *
				      sizeof(xmlXPathObjectPtr));
	if (temp == NULL) {
	    xmlXPtrErrMemory("adding location to set");
	    return;
	}
	cur->locTab = temp;
    }
    cur->locTab[cur->locNr++] = val;
}

xmlLocationSetPtr
xmlXPtrLocationSetMerge(xmlLocationSetPtr val1, xmlLocationSetPtr val2) {
    int i;

    if (val1 == NULL) return(NULL);
    if (val2 == NULL) return(val1);


    for (i = 0;i < val2->locNr;i++)
        xmlXPtrLocationSetAdd(val1, val2->locTab[i]);

    return(val1);
}

void
xmlXPtrLocationSetDel(xmlLocationSetPtr cur, xmlXPathObjectPtr val) {
    int i;

    if (cur == NULL) return;
    if (val == NULL) return;

    for (i = 0;i < cur->locNr;i++)
        if (cur->locTab[i] == val) break;

    if (i >= cur->locNr) {
#ifdef DEBUG
        xmlGenericError(xmlGenericErrorContext, 
	        "xmlXPtrLocationSetDel: Range wasn't found in RangeList\n");
#endif
        return;
    }
    cur->locNr--;
    for (;i < cur->locNr;i++)
        cur->locTab[i] = cur->locTab[i + 1];
    cur->locTab[cur->locNr] = NULL;
}

void
xmlXPtrLocationSetRemove(xmlLocationSetPtr cur, int val) {
    if (cur == NULL) return;
    if (val >= cur->locNr) return;
    cur->locNr--;
    for (;val < cur->locNr;val++)
        cur->locTab[val] = cur->locTab[val + 1];
    cur->locTab[cur->locNr] = NULL;
}

void
xmlXPtrFreeLocationSet(xmlLocationSetPtr obj) {
    int i;

    if (obj == NULL) return;
    if (obj->locTab != NULL) {
	for (i = 0;i < obj->locNr; i++) {
            xmlXPathFreeObject(obj->locTab[i]);
	}
	xmlFree(obj->locTab);
    }
    xmlFree(obj);
}

xmlXPathObjectPtr
xmlXPtrNewLocationSetNodes(xmlNodePtr start, xmlNodePtr end) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating locationset");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_LOCATIONSET;
    if (end == NULL)
	ret->user = xmlXPtrLocationSetCreate(xmlXPtrNewCollapsedRange(start));
    else
	ret->user = xmlXPtrLocationSetCreate(xmlXPtrNewRangeNodes(start,end));
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrNewLocationSetNodeSet(xmlNodeSetPtr set) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating locationset");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_LOCATIONSET;
    if (set != NULL) {
	int i;
	xmlLocationSetPtr newset;

	newset = xmlXPtrLocationSetCreate(NULL);
	if (newset == NULL)
	    return(ret);

	for (i = 0;i < set->nodeNr;i++)
	    xmlXPtrLocationSetAdd(newset,
		        xmlXPtrNewCollapsedRange(set->nodeTab[i]));

	ret->user = (void *) newset;
    }
    return(ret);
}

xmlXPathObjectPtr
xmlXPtrWrapLocationSet(xmlLocationSetPtr val) {
    xmlXPathObjectPtr ret;

    ret = (xmlXPathObjectPtr) xmlMalloc(sizeof(xmlXPathObject));
    if (ret == NULL) {
        xmlXPtrErrMemory("allocating locationset");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlXPathObject));
    ret->type = XPATH_LOCATIONSET;
    ret->user = (void *) val;
    return(ret);
}


static void xmlXPtrEvalChildSeq(xmlXPathParserContextPtr ctxt, xmlChar *name);


#define CUR (*ctxt->cur)
#define SKIP(val) ctxt->cur += (val)
#define NXT(val) ctxt->cur[(val)]
#define CUR_PTR ctxt->cur

#define SKIP_BLANKS 							\
    while (IS_BLANK_CH(*(ctxt->cur))) NEXT

#define CURRENT (*ctxt->cur)
#define NEXT ((*ctxt->cur) ?  ctxt->cur++: ctxt->cur)

static void
xmlXPtrGetChildNo(xmlXPathParserContextPtr ctxt, int indx) {
    xmlNodePtr cur = NULL;
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr oldset;

    CHECK_TYPE(XPATH_NODESET);
    obj = valuePop(ctxt);
    oldset = obj->nodesetval;
    if ((indx <= 0) || (oldset == NULL) || (oldset->nodeNr != 1)) {
	xmlXPathFreeObject(obj);
	valuePush(ctxt, xmlXPathNewNodeSet(NULL));
	return;
    }
    cur = xmlXPtrGetNthChild(oldset->nodeTab[0], indx);
    if (cur == NULL) {
	xmlXPathFreeObject(obj);
	valuePush(ctxt, xmlXPathNewNodeSet(NULL));
	return;
    }
    oldset->nodeTab[0] = cur;
    valuePush(ctxt, obj);
}


static void
xmlXPtrEvalXPtrPart(xmlXPathParserContextPtr ctxt, xmlChar *name) {
    xmlChar *buffer, *cur;
    int len;
    int level;

    if (name == NULL)
    name = xmlXPathParseName(ctxt);
    if (name == NULL)
	XP_ERROR(XPATH_EXPR_ERROR);

    if (CUR != '(')
	XP_ERROR(XPATH_EXPR_ERROR);
    NEXT;
    level = 1;

    len = xmlStrlen(ctxt->cur);
    len++;
    buffer = (xmlChar *) xmlMallocAtomic(len * sizeof (xmlChar));
    if (buffer == NULL) {
        xmlXPtrErrMemory("allocating buffer");
	return;
    }

    cur = buffer;
    while (CUR != 0) {
	if (CUR == ')') {
	    level--;
	    if (level == 0) {
		NEXT;
		break;
	    }
	} else if (CUR == '(') {
	    level++;
	} else if (CUR == '^') {
	    if ((NXT(1) == ')') || (NXT(1) == '(') || (NXT(1) == '^')) {
	        NEXT;
	    }
	}
	*cur++ = CUR;
	NEXT;
    }
    *cur = 0;

    if ((level != 0) && (CUR == 0)) {
	xmlFree(buffer);
	XP_ERROR(XPTR_SYNTAX_ERROR);
    }

    if (xmlStrEqual(name, (xmlChar *) "xpointer")) {
	const xmlChar *left = CUR_PTR;

	CUR_PTR = buffer;
	ctxt->context->node = (xmlNodePtr)ctxt->context->doc;
	ctxt->context->proximityPosition = 1;
	ctxt->context->contextSize = 1;
	xmlXPathEvalExpr(ctxt);
	CUR_PTR=left;
    } else if (xmlStrEqual(name, (xmlChar *) "element")) {
	const xmlChar *left = CUR_PTR;
	xmlChar *name2;

	CUR_PTR = buffer;
	if (buffer[0] == '/') {
	    xmlXPathRoot(ctxt);
	    xmlXPtrEvalChildSeq(ctxt, NULL);
	} else {
	    name2 = xmlXPathParseName(ctxt);
	    if (name2 == NULL) {
		CUR_PTR = left;
		xmlFree(buffer);
		XP_ERROR(XPATH_EXPR_ERROR);
	    }
	    xmlXPtrEvalChildSeq(ctxt, name2);
	}
	CUR_PTR = left;
#ifdef XPTR_XMLNS_SCHEME
    } else if (xmlStrEqual(name, (xmlChar *) "xmlns")) {
	const xmlChar *left = CUR_PTR;
	xmlChar *prefix;
	xmlChar *URI;
	xmlURIPtr value;

	CUR_PTR = buffer;
        prefix = xmlXPathParseNCName(ctxt);
	if (prefix == NULL) {
	    xmlFree(buffer);
	    xmlFree(name);
	    XP_ERROR(XPTR_SYNTAX_ERROR);
	}
	SKIP_BLANKS;
	if (CUR != '=') {
	    xmlFree(prefix);
	    xmlFree(buffer);
	    xmlFree(name);
	    XP_ERROR(XPTR_SYNTAX_ERROR);
	}
	NEXT;
	SKIP_BLANKS;
	

	value = xmlParseURI((const char *)ctxt->cur);
	if (value == NULL) {
	    xmlFree(prefix);
	    xmlFree(buffer);
	    xmlFree(name);
	    XP_ERROR(XPTR_SYNTAX_ERROR);
	}
	URI = xmlSaveUri(value);
	xmlFreeURI(value);
	if (URI == NULL) {
	    xmlFree(prefix);
	    xmlFree(buffer);
	    xmlFree(name);
	    XP_ERROR(XPATH_MEMORY_ERROR);
	}
	
	xmlXPathRegisterNs(ctxt->context, prefix, URI);
	CUR_PTR = left;
	xmlFree(URI);
	xmlFree(prefix);
#endif 
    } else {
        xmlXPtrErr(ctxt, XML_XPTR_UNKNOWN_SCHEME,
		   "unsupported scheme '%s'\n", name);
    }
    xmlFree(buffer);
    xmlFree(name);
}

static void
xmlXPtrEvalFullXPtr(xmlXPathParserContextPtr ctxt, xmlChar *name) {
    if (name == NULL)
    name = xmlXPathParseName(ctxt);
    if (name == NULL)
	XP_ERROR(XPATH_EXPR_ERROR);
    while (name != NULL) {
	ctxt->error = XPATH_EXPRESSION_OK;
	xmlXPtrEvalXPtrPart(ctxt, name);

	
	if ((ctxt->error != XPATH_EXPRESSION_OK) &&
            (ctxt->error != XML_XPTR_UNKNOWN_SCHEME))
	    return;

	if (ctxt->value != NULL) {
	    xmlXPathObjectPtr obj = ctxt->value;

	    switch (obj->type) {
		case XPATH_LOCATIONSET: {
		    xmlLocationSetPtr loc = ctxt->value->user;
		    if ((loc != NULL) && (loc->locNr > 0))
			return;
		    break;
		}
		case XPATH_NODESET: {
		    xmlNodeSetPtr loc = ctxt->value->nodesetval;
		    if ((loc != NULL) && (loc->nodeNr > 0))
			return;
		    break;
		}
		default:
		    break;
	    }

	    do {
		obj = valuePop(ctxt);
		if (obj != NULL) {
		    xmlXPathFreeObject(obj);
		}
	    } while (obj != NULL);
	}

	SKIP_BLANKS;
	name = xmlXPathParseName(ctxt);
    }
}

static void
xmlXPtrEvalChildSeq(xmlXPathParserContextPtr ctxt, xmlChar *name) {
    if ((name == NULL) && (CUR == '/') && (NXT(1) != '1')) {
        xmlXPtrErr(ctxt, XML_XPTR_CHILDSEQ_START,
		   "warning: ChildSeq not starting by /1\n", NULL);
    }

    if (name != NULL) {
	valuePush(ctxt, xmlXPathNewString(name));
	xmlFree(name);
	xmlXPathIdFunction(ctxt, 1);
	CHECK_ERROR;
    }

    while (CUR == '/') {
	int child = 0;
	NEXT;
        
	while ((CUR >= '0') && (CUR <= '9')) {
	    child = child * 10 + (CUR - '0');
	    NEXT;
	}
	xmlXPtrGetChildNo(ctxt, child);
    }
}


static void
xmlXPtrEvalXPointer(xmlXPathParserContextPtr ctxt) {
    if (ctxt->valueTab == NULL) {
	
	ctxt->valueTab = (xmlXPathObjectPtr *) 
			 xmlMalloc(10 * sizeof(xmlXPathObjectPtr));
	if (ctxt->valueTab == NULL) {
	    xmlXPtrErrMemory("allocating evaluation context");
	    return;
	}
	ctxt->valueNr = 0;
	ctxt->valueMax = 10;
	ctxt->value = NULL;
    }
    SKIP_BLANKS;
    if (CUR == '/') {
	xmlXPathRoot(ctxt);
        xmlXPtrEvalChildSeq(ctxt, NULL);
    } else {
	xmlChar *name;

	name = xmlXPathParseName(ctxt);
	if (name == NULL)
	    XP_ERROR(XPATH_EXPR_ERROR);
	if (CUR == '(') {
	    xmlXPtrEvalFullXPtr(ctxt, name);
	    
	    return;
	} else {
	    
	    xmlXPtrEvalChildSeq(ctxt, name);
	}
    }
    SKIP_BLANKS;
    if (CUR != 0)
	XP_ERROR(XPATH_EXPR_ERROR);
}



static
void xmlXPtrStringRangeFunction(xmlXPathParserContextPtr ctxt, int nargs);
static
void xmlXPtrStartPointFunction(xmlXPathParserContextPtr ctxt, int nargs);
static
void xmlXPtrEndPointFunction(xmlXPathParserContextPtr ctxt, int nargs);
static
void xmlXPtrHereFunction(xmlXPathParserContextPtr ctxt, int nargs);
static
void xmlXPtrOriginFunction(xmlXPathParserContextPtr ctxt, int nargs);
static
void xmlXPtrRangeInsideFunction(xmlXPathParserContextPtr ctxt, int nargs);
static
void xmlXPtrRangeFunction(xmlXPathParserContextPtr ctxt, int nargs);

xmlXPathContextPtr
xmlXPtrNewContext(xmlDocPtr doc, xmlNodePtr here, xmlNodePtr origin) {
    xmlXPathContextPtr ret;

    ret = xmlXPathNewContext(doc);
    if (ret == NULL)
	return(ret);
    ret->xptr = 1;
    ret->here = here;
    ret->origin = origin;

    xmlXPathRegisterFunc(ret, (xmlChar *)"range-to",
	                 xmlXPtrRangeToFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)"range",
	                 xmlXPtrRangeFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)"range-inside",
	                 xmlXPtrRangeInsideFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)"string-range",
	                 xmlXPtrStringRangeFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)"start-point",
	                 xmlXPtrStartPointFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)"end-point",
	                 xmlXPtrEndPointFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)"here",
	                 xmlXPtrHereFunction);
    xmlXPathRegisterFunc(ret, (xmlChar *)" origin",
	                 xmlXPtrOriginFunction);

    return(ret);
}

xmlXPathObjectPtr
xmlXPtrEval(const xmlChar *str, xmlXPathContextPtr ctx) {
    xmlXPathParserContextPtr ctxt;
    xmlXPathObjectPtr res = NULL, tmp;
    xmlXPathObjectPtr init = NULL;
    int stack = 0;

    xmlXPathInit();

    if ((ctx == NULL) || (str == NULL))
	return(NULL);

    ctxt = xmlXPathNewParserContext(str, ctx);
    ctxt->xptr = 1;
    xmlXPtrEvalXPointer(ctxt);

    if ((ctxt->value != NULL) &&
	(ctxt->value->type != XPATH_NODESET) &&
	(ctxt->value->type != XPATH_LOCATIONSET)) {
        xmlXPtrErr(ctxt, XML_XPTR_EVAL_FAILED,
		"xmlXPtrEval: evaluation failed to return a node set\n",
		   NULL);
    } else {
	res = valuePop(ctxt);
    }

    do {
        tmp = valuePop(ctxt);
	if (tmp != NULL) {
	    if (tmp != init) {
		if (tmp->type == XPATH_NODESET) {
		    xmlNodeSetPtr set; 
		    set = tmp->nodesetval;
		    if ((set->nodeNr != 1) ||
			(set->nodeTab[0] != (xmlNodePtr) ctx->doc))
			stack++;
		} else
		    stack++;    
	    }
	    xmlXPathFreeObject(tmp);
        }
    } while (tmp != NULL);
    if (stack != 0) {
        xmlXPtrErr(ctxt, XML_XPTR_EXTRA_OBJECTS,
		   "xmlXPtrEval: object(s) left on the eval stack\n",
		   NULL);
    }
    if (ctxt->error != XPATH_EXPRESSION_OK) {
	xmlXPathFreeObject(res);
	res = NULL;
    }
        
    xmlXPathFreeParserContext(ctxt);
    return(res);
}

static xmlNodePtr
xmlXPtrBuildRangeNodeList(xmlXPathObjectPtr range) {
    
    xmlNodePtr list = NULL, last = NULL, parent = NULL, tmp;
    
    xmlNodePtr start, cur, end;
    int index1, index2;

    if (range == NULL)
	return(NULL);
    if (range->type != XPATH_RANGE)
	return(NULL);
    start = (xmlNodePtr) range->user;

    if (start == NULL)
	return(NULL);
    end = range->user2;
    if (end == NULL)
	return(xmlCopyNode(start, 1));

    cur = start;
    index1 = range->index;
    index2 = range->index2;
    while (cur != NULL) {
	if (cur == end) {
	    if (cur->type == XML_TEXT_NODE) {
		const xmlChar *content = cur->content;
		int len;

		if (content == NULL) {
		    tmp = xmlNewTextLen(NULL, 0);
		} else {
		    len = index2;
		    if ((cur == start) && (index1 > 1)) {
			content += (index1 - 1);
			len -= (index1 - 1);
			index1 = 0;
		    } else {
			len = index2;
		    }
		    tmp = xmlNewTextLen(content, len);
		}
		
		if (list == NULL)
		    return(tmp);
		
		if (last != NULL)
		    xmlAddNextSibling(last, tmp);
		else 
		    xmlAddChild(parent, tmp);
		return(list);
	    } else {
		tmp = xmlCopyNode(cur, 0);
		if (list == NULL)
		    list = tmp;
		else {
		    if (last != NULL)
			xmlAddNextSibling(last, tmp);
		    else
			xmlAddChild(parent, tmp);
		}
		last = NULL;
		parent = tmp;

		if (index2 > 1) {
		    end = xmlXPtrGetNthChild(cur, index2 - 1);
		    index2 = 0;
		}
		if ((cur == start) && (index1 > 1)) {
		    cur = xmlXPtrGetNthChild(cur, index1 - 1);
		    index1 = 0;
		} else {
		    cur = cur->children;
		}
		continue; 
	    }
	} else if ((cur == start) &&
		   (list == NULL)  ) {
	    if ((cur->type == XML_TEXT_NODE) ||
		(cur->type == XML_CDATA_SECTION_NODE)) {
		const xmlChar *content = cur->content;

		if (content == NULL) {
		    tmp = xmlNewTextLen(NULL, 0);
		} else {
		    if (index1 > 1) {
			content += (index1 - 1);
		    }
		    tmp = xmlNewText(content);
		}
		last = list = tmp;
	    } else {
		if ((cur == start) && (index1 > 1)) {
		    tmp = xmlCopyNode(cur, 0);
		    list = tmp;
		    parent = tmp;
		    last = NULL;
		    cur = xmlXPtrGetNthChild(cur, index1 - 1);
		    index1 = 0;
		    continue; 
		}
		tmp = xmlCopyNode(cur, 1);
		list = tmp;
		parent = NULL;
		last = tmp;
	    }
	} else {
	    tmp = NULL;
	    switch (cur->type) {
		case XML_DTD_NODE:
		case XML_ELEMENT_DECL:
		case XML_ATTRIBUTE_DECL:
		case XML_ENTITY_NODE:
		    
		    break;
		case XML_ENTITY_DECL:
		    TODO 
		    break;
		case XML_XINCLUDE_START:
		case XML_XINCLUDE_END:
		    
		    break;
		case XML_ATTRIBUTE_NODE:
		    
		    STRANGE
		    break;
		default:
		    tmp = xmlCopyNode(cur, 1);
		    break;
	    }
	    if (tmp != NULL) {
		if ((list == NULL) || ((last == NULL) && (parent == NULL)))  {
		    STRANGE
		    return(NULL);
		}
		if (last != NULL)
		    xmlAddNextSibling(last, tmp);
		else {
		    xmlAddChild(parent, tmp);
		    last = tmp;
		}
	    }
	}
	if ((list == NULL) || ((last == NULL) && (parent == NULL)))  {
	    STRANGE
	    return(NULL);
	}
	cur = xmlXPtrAdvanceNode(cur, NULL);
    }
    return(list);
}

xmlNodePtr
xmlXPtrBuildNodeList(xmlXPathObjectPtr obj) {
    xmlNodePtr list = NULL, last = NULL;
    int i;

    if (obj == NULL)
	return(NULL);
    switch (obj->type) {
        case XPATH_NODESET: {
	    xmlNodeSetPtr set = obj->nodesetval;
	    if (set == NULL)
		return(NULL);
	    for (i = 0;i < set->nodeNr;i++) {
		if (set->nodeTab[i] == NULL)
		    continue;
		switch (set->nodeTab[i]->type) {
		    case XML_TEXT_NODE:
		    case XML_CDATA_SECTION_NODE:
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
		    case XML_XINCLUDE_START:
		    case XML_XINCLUDE_END:
			break;
		    case XML_ATTRIBUTE_NODE:
		    case XML_NAMESPACE_DECL:
		    case XML_DOCUMENT_TYPE_NODE:
		    case XML_DOCUMENT_FRAG_NODE:
		    case XML_NOTATION_NODE:
		    case XML_DTD_NODE:
		    case XML_ELEMENT_DECL:
		    case XML_ATTRIBUTE_DECL:
		    case XML_ENTITY_DECL:
			continue; 
		}
		if (last == NULL)
		    list = last = xmlCopyNode(set->nodeTab[i], 1);
		else {
		    xmlAddNextSibling(last, xmlCopyNode(set->nodeTab[i], 1));
		    if (last->next != NULL)
			last = last->next;
		}
	    }
	    break;
	}
	case XPATH_LOCATIONSET: {
	    xmlLocationSetPtr set = (xmlLocationSetPtr) obj->user;
	    if (set == NULL)
		return(NULL);
	    for (i = 0;i < set->locNr;i++) {
		if (last == NULL)
		    list = last = xmlXPtrBuildNodeList(set->locTab[i]);
		else
		    xmlAddNextSibling(last,
			    xmlXPtrBuildNodeList(set->locTab[i]));
		if (last != NULL) {
		    while (last->next != NULL)
			last = last->next;
		}
	    }
	    break;
	}
	case XPATH_RANGE:
	    return(xmlXPtrBuildRangeNodeList(obj));
	case XPATH_POINT:
	    return(xmlCopyNode(obj->user, 0));
	default:
	    break;
    }
    return(list);
}


static int
xmlXPtrNbLocChildren(xmlNodePtr node) {
    int ret = 0;
    if (node == NULL)
	return(-1);
    switch (node->type) {
        case XML_HTML_DOCUMENT_NODE:
        case XML_DOCUMENT_NODE:
        case XML_ELEMENT_NODE:
	    node = node->children;
	    while (node != NULL) {
		if (node->type == XML_ELEMENT_NODE)
		    ret++;
		node = node->next;
	    }
	    break;
        case XML_ATTRIBUTE_NODE:
	    return(-1);

        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
	    ret = xmlStrlen(node->content);
	    break;
	default:
	    return(-1);
    }
    return(ret);
}

static void
xmlXPtrHereFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);

    if (ctxt->context->here == NULL)
	XP_ERROR(XPTR_SYNTAX_ERROR);
    
    valuePush(ctxt, xmlXPtrNewLocationSetNodes(ctxt->context->here, NULL));
}

static void
xmlXPtrOriginFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);

    if (ctxt->context->origin == NULL)
	XP_ERROR(XPTR_SYNTAX_ERROR);
    
    valuePush(ctxt, xmlXPtrNewLocationSetNodes(ctxt->context->origin, NULL));
}

static void
xmlXPtrStartPointFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr tmp, obj, point;
    xmlLocationSetPtr newset = NULL;
    xmlLocationSetPtr oldset = NULL;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    obj = valuePop(ctxt);
    if (obj->type == XPATH_NODESET) {
	tmp = xmlXPtrNewLocationSetNodeSet(obj->nodesetval);
	xmlXPathFreeObject(obj);
	obj = tmp;
    }

    newset = xmlXPtrLocationSetCreate(NULL);
    if (newset == NULL) {
	xmlXPathFreeObject(obj);
        XP_ERROR(XPATH_MEMORY_ERROR);
    }
    oldset = (xmlLocationSetPtr) obj->user;
    if (oldset != NULL) {
	int i;

	for (i = 0; i < oldset->locNr; i++) {
	    tmp = oldset->locTab[i];
	    if (tmp == NULL)
		continue;
	    point = NULL;
	    switch (tmp->type) {
		case XPATH_POINT:
		    point = xmlXPtrNewPoint(tmp->user, tmp->index);
		    break;
		case XPATH_RANGE: {
		    xmlNodePtr node = tmp->user;
		    if (node != NULL) {
			if (node->type == XML_ATTRIBUTE_NODE) {
			    
			    xmlXPathFreeObject(obj);
			    xmlXPtrFreeLocationSet(newset);
			    XP_ERROR(XPTR_SYNTAX_ERROR);
			}
			point = xmlXPtrNewPoint(node, tmp->index);
		    }
		    break;
	        }
		default:
		    break;
	    }
            if (point != NULL)
		xmlXPtrLocationSetAdd(newset, point);
	}
    }
    xmlXPathFreeObject(obj);
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
}

static void
xmlXPtrEndPointFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr tmp, obj, point;
    xmlLocationSetPtr newset = NULL;
    xmlLocationSetPtr oldset = NULL;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    obj = valuePop(ctxt);
    if (obj->type == XPATH_NODESET) {
	tmp = xmlXPtrNewLocationSetNodeSet(obj->nodesetval);
	xmlXPathFreeObject(obj);
	obj = tmp;
    }

    newset = xmlXPtrLocationSetCreate(NULL);
    oldset = (xmlLocationSetPtr) obj->user;
    if (oldset != NULL) {
	int i;

	for (i = 0; i < oldset->locNr; i++) {
	    tmp = oldset->locTab[i];
	    if (tmp == NULL)
		continue;
	    point = NULL;
	    switch (tmp->type) {
		case XPATH_POINT:
		    point = xmlXPtrNewPoint(tmp->user, tmp->index);
		    break;
		case XPATH_RANGE: {
		    xmlNodePtr node = tmp->user2;
		    if (node != NULL) {
			if (node->type == XML_ATTRIBUTE_NODE) {
			    
			    xmlXPathFreeObject(obj);
			    xmlXPtrFreeLocationSet(newset);
			    XP_ERROR(XPTR_SYNTAX_ERROR);
			}
			point = xmlXPtrNewPoint(node, tmp->index2);
		    } else if (tmp->user == NULL) {
			point = xmlXPtrNewPoint(node,
				       xmlXPtrNbLocChildren(node));
		    }
		    break;
	        }
		default:
		    break;
	    }
            if (point != NULL)
		xmlXPtrLocationSetAdd(newset, point);
	}
    }
    xmlXPathFreeObject(obj);
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
}


static xmlXPathObjectPtr
xmlXPtrCoveringRange(xmlXPathParserContextPtr ctxt, xmlXPathObjectPtr loc) {
    if (loc == NULL)
	return(NULL);
    if ((ctxt == NULL) || (ctxt->context == NULL) ||
	(ctxt->context->doc == NULL))
	return(NULL);
    switch (loc->type) {
        case XPATH_POINT:
	    return(xmlXPtrNewRange(loc->user, loc->index,
			           loc->user, loc->index));
        case XPATH_RANGE:
	    if (loc->user2 != NULL) {
		return(xmlXPtrNewRange(loc->user, loc->index,
			              loc->user2, loc->index2));
	    } else {
		xmlNodePtr node = (xmlNodePtr) loc->user;
		if (node == (xmlNodePtr) ctxt->context->doc) {
		    return(xmlXPtrNewRange(node, 0, node,
					   xmlXPtrGetArity(node)));
		} else {
		    switch (node->type) {
			case XML_ATTRIBUTE_NODE:
			
			    return(xmlXPtrNewRange(node, 0, node,
					           xmlXPtrGetArity(node)));
			case XML_ELEMENT_NODE:
			case XML_TEXT_NODE:
			case XML_CDATA_SECTION_NODE:
			case XML_ENTITY_REF_NODE:
			case XML_PI_NODE:
			case XML_COMMENT_NODE:
			case XML_DOCUMENT_NODE:
			case XML_NOTATION_NODE:
			case XML_HTML_DOCUMENT_NODE: {
			    int indx = xmlXPtrGetIndex(node);
			     
			    node = node->parent;
			    return(xmlXPtrNewRange(node, indx - 1,
					           node, indx + 1));
			}
			default:
			    return(NULL);
		    }
		}
	    }
	default:
	    TODO 
    }
    return(NULL);
}

static void
xmlXPtrRangeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    int i;
    xmlXPathObjectPtr set;
    xmlLocationSetPtr oldset;
    xmlLocationSetPtr newset;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    set = valuePop(ctxt);
    if (set->type == XPATH_NODESET) {
	xmlXPathObjectPtr tmp;

	tmp = xmlXPtrNewLocationSetNodeSet(set->nodesetval);
	xmlXPathFreeObject(set);
	set = tmp;
    }
    oldset = (xmlLocationSetPtr) set->user;

    newset = xmlXPtrLocationSetCreate(NULL);
    for (i = 0;i < oldset->locNr;i++) {
	xmlXPtrLocationSetAdd(newset,
		xmlXPtrCoveringRange(ctxt, oldset->locTab[i]));
    }

    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
    xmlXPathFreeObject(set);
}

static xmlXPathObjectPtr
xmlXPtrInsideRange(xmlXPathParserContextPtr ctxt, xmlXPathObjectPtr loc) {
    if (loc == NULL)
	return(NULL);
    if ((ctxt == NULL) || (ctxt->context == NULL) ||
	(ctxt->context->doc == NULL))
	return(NULL);
    switch (loc->type) {
        case XPATH_POINT: {
	    xmlNodePtr node = (xmlNodePtr) loc->user;
	    switch (node->type) {
		case XML_PI_NODE:
		case XML_COMMENT_NODE:
		case XML_TEXT_NODE:
		case XML_CDATA_SECTION_NODE: {
		    if (node->content == NULL) {
			return(xmlXPtrNewRange(node, 0, node, 0));
		    } else {
			return(xmlXPtrNewRange(node, 0, node,
					       xmlStrlen(node->content)));
		    }
		}
		case XML_ATTRIBUTE_NODE:
		case XML_ELEMENT_NODE:
		case XML_ENTITY_REF_NODE:
		case XML_DOCUMENT_NODE:
		case XML_NOTATION_NODE:
		case XML_HTML_DOCUMENT_NODE: {
		    return(xmlXPtrNewRange(node, 0, node,
					   xmlXPtrGetArity(node)));
		}
		default:
		    break;
	    }
	    return(NULL);
	}
        case XPATH_RANGE: {
	    xmlNodePtr node = (xmlNodePtr) loc->user;
	    if (loc->user2 != NULL) {
		return(xmlXPtrNewRange(node, loc->index,
			               loc->user2, loc->index2));
	    } else {
		switch (node->type) {
		    case XML_PI_NODE:
		    case XML_COMMENT_NODE:
		    case XML_TEXT_NODE:
		    case XML_CDATA_SECTION_NODE: {
			if (node->content == NULL) {
			    return(xmlXPtrNewRange(node, 0, node, 0));
			} else {
			    return(xmlXPtrNewRange(node, 0, node,
						   xmlStrlen(node->content)));
			}
		    }
		    case XML_ATTRIBUTE_NODE:
		    case XML_ELEMENT_NODE:
		    case XML_ENTITY_REF_NODE:
		    case XML_DOCUMENT_NODE:
		    case XML_NOTATION_NODE:
		    case XML_HTML_DOCUMENT_NODE: {
			return(xmlXPtrNewRange(node, 0, node,
					       xmlXPtrGetArity(node)));
		    }
		    default:
			break;
		}
		return(NULL);
	    }
        }
	default:
	    TODO 
    }
    return(NULL);
}

static void
xmlXPtrRangeInsideFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    int i;
    xmlXPathObjectPtr set;
    xmlLocationSetPtr oldset;
    xmlLocationSetPtr newset;

    CHECK_ARITY(1);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    set = valuePop(ctxt);
    if (set->type == XPATH_NODESET) {
	xmlXPathObjectPtr tmp;

	tmp = xmlXPtrNewLocationSetNodeSet(set->nodesetval);
	xmlXPathFreeObject(set);
	set = tmp;
    }
    oldset = (xmlLocationSetPtr) set->user;

    newset = xmlXPtrLocationSetCreate(NULL);
    for (i = 0;i < oldset->locNr;i++) {
	xmlXPtrLocationSetAdd(newset,
		xmlXPtrInsideRange(ctxt, oldset->locTab[i]));
    }

    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
    xmlXPathFreeObject(set);
}

void
xmlXPtrRangeToFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr range;
    const xmlChar *cur;
    xmlXPathObjectPtr res, obj;
    xmlXPathObjectPtr tmp;
    xmlLocationSetPtr newset = NULL;
    xmlNodeSetPtr oldset;
    int i;

    if (ctxt == NULL) return;
    CHECK_ARITY(1);
    CHECK_TYPE(XPATH_NODESET);
    obj = valuePop(ctxt);
    oldset = obj->nodesetval;
    ctxt->context->node = NULL;

    cur = ctxt->cur;
    newset = xmlXPtrLocationSetCreate(NULL);
    
    for (i = 0; i < oldset->nodeNr; i++) {
	ctxt->cur = cur;

	ctxt->context->node = oldset->nodeTab[i];
	tmp = xmlXPathNewNodeSet(ctxt->context->node);
	valuePush(ctxt, tmp);

	xmlXPathEvalExpr(ctxt);
	CHECK_ERROR;

	res = valuePop(ctxt);
	range = xmlXPtrNewRangeNodeObject(oldset->nodeTab[i], res);
	if (range != NULL) {
	    xmlXPtrLocationSetAdd(newset, range);
	}

	if (res != NULL)
	    xmlXPathFreeObject(res);
	if (ctxt->value == tmp) {
	    res = valuePop(ctxt);
	    xmlXPathFreeObject(res);
	}
	
	ctxt->context->node = NULL;
    }

    xmlXPathFreeObject(obj);
    ctxt->context->node = NULL;
    ctxt->context->contextSize = -1;
    ctxt->context->proximityPosition = -1;
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
}

xmlNodePtr
xmlXPtrAdvanceNode(xmlNodePtr cur, int *level) {
next:
    if (cur == NULL)
	return(NULL);
    if (cur->children != NULL) {
        cur = cur->children ;
	if (level != NULL)
	    (*level)++;
	goto found;
    }
skip:		
    if (cur->next != NULL) {
	cur = cur->next;
	goto found;
    }
    do {
        cur = cur->parent;
	if (level != NULL)
	    (*level)--;
        if (cur == NULL) return(NULL);
        if (cur->next != NULL) {
	    cur = cur->next;
	    goto found;
	}
    } while (cur != NULL);

found:
    if ((cur->type != XML_ELEMENT_NODE) &&
	(cur->type != XML_TEXT_NODE) &&
	(cur->type != XML_DOCUMENT_NODE) &&
	(cur->type != XML_HTML_DOCUMENT_NODE) &&
	(cur->type != XML_CDATA_SECTION_NODE)) {
	    if (cur->type == XML_ENTITY_REF_NODE) {	
		TODO
		goto skip;
	    }
	    goto next;
	}
    return(cur);
}

static int
xmlXPtrAdvanceChar(xmlNodePtr *node, int *indx, int bytes) {
    xmlNodePtr cur;
    int pos;
    int len;

    if ((node == NULL) || (indx == NULL))
	return(-1);
    cur = *node;
    if (cur == NULL)
	return(-1);
    pos = *indx;

    while (bytes >= 0) {
	while ((cur != NULL) &&
	       ((cur->type == XML_ELEMENT_NODE) ||
	        (cur->type == XML_DOCUMENT_NODE) ||
	        (cur->type == XML_HTML_DOCUMENT_NODE))) {
	    if (pos > 0) {
		cur = xmlXPtrGetNthChild(cur, pos);
		pos = 0;
	    } else {
		cur = xmlXPtrAdvanceNode(cur, NULL);
		pos = 0;
	    }
	}

	if (cur == NULL) {
	    *node = NULL;
	    *indx = 0;
	    return(-1);
	}

	if (pos == 0) pos = 1;
	if (bytes == 0) {
	    *node = cur;
	    *indx = pos;
	    return(0);
	}
	len = 0;
	if ((cur->type != XML_ELEMENT_NODE) &&
            (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	}
	if (pos > len) {
	    
	    STRANGE
	    pos = len;
	}
	if (pos + bytes >= len) {
	    bytes -= (len - pos);
	    cur = xmlXPtrAdvanceNode(cur, NULL);
	    pos = 0;
	} else if (pos + bytes < len) {
	    pos += bytes;
	    *node = cur;
	    *indx = pos;
	    return(0);
	}
    }
    return(-1);
}

static int
xmlXPtrMatchString(const xmlChar *string, xmlNodePtr start, int startindex,
	            xmlNodePtr *end, int *endindex) {
    xmlNodePtr cur;
    int pos; 
    int len; 
    int stringlen; 
    int match;

    if (string == NULL)
	return(-1);
    if (start == NULL)
	return(-1);
    if ((end == NULL) || (endindex == NULL))
	return(-1);
    cur = start;
    if (cur == NULL)
	return(-1);
    pos = startindex - 1;
    stringlen = xmlStrlen(string);

    while (stringlen > 0) {
	if ((cur == *end) && (pos + stringlen > *endindex))
	    return(0);

	if ((cur->type != XML_ELEMENT_NODE) && (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	    if (len >= pos + stringlen) {
		match = (!xmlStrncmp(&cur->content[pos], string, stringlen));
		if (match) {
#ifdef DEBUG_RANGES
		    xmlGenericError(xmlGenericErrorContext,
			    "found range %d bytes at index %d of ->",
			    stringlen, pos + 1);
		    xmlDebugDumpString(stdout, cur->content);
		    xmlGenericError(xmlGenericErrorContext, "\n");
#endif
		    *end = cur;
		    *endindex = pos + stringlen;
		    return(1);
		} else {
		    return(0);
		}
	    } else {
                int sub = len - pos;
		match = (!xmlStrncmp(&cur->content[pos], string, sub));
		if (match) {
#ifdef DEBUG_RANGES
		    xmlGenericError(xmlGenericErrorContext,
			    "found subrange %d bytes at index %d of ->",
			    sub, pos + 1);
		    xmlDebugDumpString(stdout, cur->content);
		    xmlGenericError(xmlGenericErrorContext, "\n");
#endif
                    string = &string[sub];
		    stringlen -= sub;
		} else {
		    return(0);
		}
	    }
	}
	cur = xmlXPtrAdvanceNode(cur, NULL);
	if (cur == NULL)
	    return(0);
	pos = 0;
    }
    return(1);
}

static int
xmlXPtrSearchString(const xmlChar *string, xmlNodePtr *start, int *startindex,
	            xmlNodePtr *end, int *endindex) {
    xmlNodePtr cur;
    const xmlChar *str;
    int pos; 
    int len; 
    xmlChar first;

    if (string == NULL)
	return(-1);
    if ((start == NULL) || (startindex == NULL))
	return(-1);
    if ((end == NULL) || (endindex == NULL))
	return(-1);
    cur = *start;
    if (cur == NULL)
	return(-1);
    pos = *startindex - 1;
    first = string[0];

    while (cur != NULL) {
	if ((cur->type != XML_ELEMENT_NODE) && (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	    while (pos <= len) {
		if (first != 0) {
		    str = xmlStrchr(&cur->content[pos], first);
		    if (str != NULL) {
			pos = (str - (xmlChar *)(cur->content));
#ifdef DEBUG_RANGES
			xmlGenericError(xmlGenericErrorContext,
				"found '%c' at index %d of ->",
				first, pos + 1);
			xmlDebugDumpString(stdout, cur->content);
			xmlGenericError(xmlGenericErrorContext, "\n");
#endif
			if (xmlXPtrMatchString(string, cur, pos + 1,
					       end, endindex)) {
			    *start = cur;
			    *startindex = pos + 1;
			    return(1);
			}
			pos++;
		    } else {
			pos = len + 1;
		    }
		} else {
#ifdef DEBUG_RANGES
		    xmlGenericError(xmlGenericErrorContext,
			    "found '' at index %d of ->",
			    pos + 1);
		    xmlDebugDumpString(stdout, cur->content);
		    xmlGenericError(xmlGenericErrorContext, "\n");
#endif
		    *start = cur;
		    *startindex = pos + 1;
		    *end = cur;
		    *endindex = pos + 1;
		    return(1);
		}
	    }
	}
	if ((cur == *end) && (pos >= *endindex))
	    return(0);
	cur = xmlXPtrAdvanceNode(cur, NULL);
	if (cur == NULL)
	    return(0);
	pos = 1;
    }
    return(0);
}

static int
xmlXPtrGetLastChar(xmlNodePtr *node, int *indx) {
    xmlNodePtr cur;
    int pos, len = 0;

    if ((node == NULL) || (indx == NULL))
	return(-1);
    cur = *node;
    pos = *indx;

    if (cur == NULL)
	return(-1);

    if ((cur->type == XML_ELEMENT_NODE) ||
	(cur->type == XML_DOCUMENT_NODE) ||
	(cur->type == XML_HTML_DOCUMENT_NODE)) {
	if (pos > 0) {
	    cur = xmlXPtrGetNthChild(cur, pos);
	}
    }
    while (cur != NULL) {
	if (cur->last != NULL)
	    cur = cur->last;
	else if ((cur->type != XML_ELEMENT_NODE) &&
	         (cur->content != NULL)) {
	    len = xmlStrlen(cur->content);
	    break;
	} else {
	    return(-1);
	}
    }
    if (cur == NULL)
	return(-1);
    *node = cur;
    *indx = len;
    return(0);
}

static int
xmlXPtrGetStartPoint(xmlXPathObjectPtr obj, xmlNodePtr *node, int *indx) {
    if ((obj == NULL) || (node == NULL) || (indx == NULL))
	return(-1);

    switch (obj->type) {
        case XPATH_POINT:
	    *node = obj->user;
	    if (obj->index <= 0)
		*indx = 0;
	    else
		*indx = obj->index;
	    return(0);
        case XPATH_RANGE:
	    *node = obj->user;
	    if (obj->index <= 0)
		*indx = 0;
	    else
		*indx = obj->index;
	    return(0);
	default:
	    break;
    }
    return(-1);
}

static int
xmlXPtrGetEndPoint(xmlXPathObjectPtr obj, xmlNodePtr *node, int *indx) {
    if ((obj == NULL) || (node == NULL) || (indx == NULL))
	return(-1);

    switch (obj->type) {
        case XPATH_POINT:
	    *node = obj->user;
	    if (obj->index <= 0)
		*indx = 0;
	    else
		*indx = obj->index;
	    return(0);
        case XPATH_RANGE:
	    *node = obj->user;
	    if (obj->index <= 0)
		*indx = 0;
	    else
		*indx = obj->index;
	    return(0);
	default:
	    break;
    }
    return(-1);
}

static void
xmlXPtrStringRangeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    int i, startindex, endindex = 0, fendindex;
    xmlNodePtr start, end = 0, fend;
    xmlXPathObjectPtr set;
    xmlLocationSetPtr oldset;
    xmlLocationSetPtr newset;
    xmlXPathObjectPtr string;
    xmlXPathObjectPtr position = NULL;
    xmlXPathObjectPtr number = NULL;
    int found, pos = 0, num = 0;

    if ((nargs < 2) || (nargs > 4))
	XP_ERROR(XPATH_INVALID_ARITY);

    if (nargs >= 4) {
	CHECK_TYPE(XPATH_NUMBER);
	number = valuePop(ctxt);
	if (number != NULL)
	    num = (int) number->floatval;
    }
    if (nargs >= 3) {
	CHECK_TYPE(XPATH_NUMBER);
	position = valuePop(ctxt);
	if (position != NULL)
	    pos = (int) position->floatval;
    }
    CHECK_TYPE(XPATH_STRING);
    string = valuePop(ctxt);
    if ((ctxt->value == NULL) ||
	((ctxt->value->type != XPATH_LOCATIONSET) &&
	 (ctxt->value->type != XPATH_NODESET)))
        XP_ERROR(XPATH_INVALID_TYPE)

    set = valuePop(ctxt);
    newset = xmlXPtrLocationSetCreate(NULL);
    if (set->nodesetval == NULL) {
        goto error;
    }
    if (set->type == XPATH_NODESET) {
	xmlXPathObjectPtr tmp;

	tmp = xmlXPtrNewLocationSetNodeSet(set->nodesetval);
	xmlXPathFreeObject(set);
	set = tmp;
    }
    oldset = (xmlLocationSetPtr) set->user;

    for (i = 0;i < oldset->locNr;i++) {
#ifdef DEBUG_RANGES
	xmlXPathDebugDumpObject(stdout, oldset->locTab[i], 0);
#endif

	xmlXPtrGetStartPoint(oldset->locTab[i], &start, &startindex);
	xmlXPtrGetEndPoint(oldset->locTab[i], &end, &endindex);
	xmlXPtrAdvanceChar(&start, &startindex, 0);
	xmlXPtrGetLastChar(&end, &endindex);

#ifdef DEBUG_RANGES
	xmlGenericError(xmlGenericErrorContext,
		"from index %d of ->", startindex);
	xmlDebugDumpString(stdout, start->content);
	xmlGenericError(xmlGenericErrorContext, "\n");
	xmlGenericError(xmlGenericErrorContext,
		"to index %d of ->", endindex);
	xmlDebugDumpString(stdout, end->content);
	xmlGenericError(xmlGenericErrorContext, "\n");
#endif
	do {
            fend = end;
            fendindex = endindex;
	    found = xmlXPtrSearchString(string->stringval, &start, &startindex,
		                        &fend, &fendindex);
	    if (found == 1) {
		if (position == NULL) {
		    xmlXPtrLocationSetAdd(newset,
			 xmlXPtrNewRange(start, startindex, fend, fendindex));
		} else if (xmlXPtrAdvanceChar(&start, &startindex,
			                      pos - 1) == 0) {
		    if ((number != NULL) && (num > 0)) {
			int rindx;
			xmlNodePtr rend;
			rend = start;
			rindx = startindex - 1;
			if (xmlXPtrAdvanceChar(&rend, &rindx,
				               num) == 0) {
			    xmlXPtrLocationSetAdd(newset,
					xmlXPtrNewRange(start, startindex,
							rend, rindx));
			}
		    } else if ((number != NULL) && (num <= 0)) {
			xmlXPtrLocationSetAdd(newset,
				    xmlXPtrNewRange(start, startindex,
						    start, startindex));
		    } else {
			xmlXPtrLocationSetAdd(newset,
				    xmlXPtrNewRange(start, startindex,
						    fend, fendindex));
		    }
		}
		start = fend;
		startindex = fendindex;
		if (string->stringval[0] == 0)
		    startindex++;
	    }
	} while (found == 1);
    }

error:
    valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
    xmlXPathFreeObject(set);
    xmlXPathFreeObject(string);
    if (position) xmlXPathFreeObject(position);
    if (number) xmlXPathFreeObject(number);
}

void
xmlXPtrEvalRangePredicate(xmlXPathParserContextPtr ctxt) {
    const xmlChar *cur;
    xmlXPathObjectPtr res;
    xmlXPathObjectPtr obj, tmp;
    xmlLocationSetPtr newset = NULL;
    xmlLocationSetPtr oldset;
    int i;

    if (ctxt == NULL) return;

    SKIP_BLANKS;
    if (CUR != '[') {
	XP_ERROR(XPATH_INVALID_PREDICATE_ERROR);
    }
    NEXT;
    SKIP_BLANKS;

    CHECK_TYPE(XPATH_LOCATIONSET);
    obj = valuePop(ctxt);
    oldset = obj->user;
    ctxt->context->node = NULL;

    if ((oldset == NULL) || (oldset->locNr == 0)) {
	ctxt->context->contextSize = 0;
	ctxt->context->proximityPosition = 0;
	xmlXPathEvalExpr(ctxt);
	res = valuePop(ctxt);
	if (res != NULL)
	    xmlXPathFreeObject(res);
	valuePush(ctxt, obj);
	CHECK_ERROR;
    } else {
        cur = ctxt->cur;
	newset = xmlXPtrLocationSetCreate(NULL);
	
        for (i = 0; i < oldset->locNr; i++) {
	    ctxt->cur = cur;

	    ctxt->context->node = oldset->locTab[i]->user;
	    tmp = xmlXPathNewNodeSet(ctxt->context->node);
	    valuePush(ctxt, tmp);
	    ctxt->context->contextSize = oldset->locNr;
	    ctxt->context->proximityPosition = i + 1;

	    xmlXPathEvalExpr(ctxt);
	    CHECK_ERROR;

	    res = valuePop(ctxt);
	    if (xmlXPathEvaluatePredicateResult(ctxt, res)) {
	        xmlXPtrLocationSetAdd(newset,
			xmlXPathObjectCopy(oldset->locTab[i]));
	    }

	    if (res != NULL)
		xmlXPathFreeObject(res);
	    if (ctxt->value == tmp) {
		res = valuePop(ctxt);
		xmlXPathFreeObject(res);
	    }
	    
	    ctxt->context->node = NULL;
	}

	xmlXPathFreeObject(obj);
	ctxt->context->node = NULL;
	ctxt->context->contextSize = -1;
	ctxt->context->proximityPosition = -1;
	valuePush(ctxt, xmlXPtrWrapLocationSet(newset));
    }
    if (CUR != ']') {
	XP_ERROR(XPATH_INVALID_PREDICATE_ERROR);
    }

    NEXT;
    SKIP_BLANKS;
}

#define bottom_xpointer
#include "elfgcchack.h"
#endif

