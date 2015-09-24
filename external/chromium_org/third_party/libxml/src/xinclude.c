/*
 * xinclude.c : Code to implement XInclude processing
 *
 * World Wide Web Consortium W3C Last Call Working Draft 10 November 2003
 * http://www.w3.org/TR/2003/WD-xinclude-20031110
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>
#include <libxml/xpointer.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlerror.h>
#include <libxml/encoding.h>
#include <libxml/globals.h>

#ifdef LIBXML_XINCLUDE_ENABLED
#include <libxml/xinclude.h>


#define XINCLUDE_MAX_DEPTH 40

#ifdef DEBUG_XINCLUDE
#ifdef LIBXML_DEBUG_ENABLED
#include <libxml/debugXML.h>
#endif
#endif


typedef xmlChar *xmlURL;

typedef struct _xmlXIncludeRef xmlXIncludeRef;
typedef xmlXIncludeRef *xmlXIncludeRefPtr;
struct _xmlXIncludeRef {
    xmlChar              *URI; 
    xmlChar         *fragment; 
    xmlDocPtr		  doc; 
    xmlNodePtr            ref; 
    xmlNodePtr            inc; 
    int                   xml; 
    int                 count; 
    xmlXPathObjectPtr    xptr; 
    int		      emptyFb; 
};

struct _xmlXIncludeCtxt {
    xmlDocPtr             doc; 
    int               incBase; 
    int                 incNr; 
    int                incMax; 
    xmlXIncludeRefPtr *incTab; 

    int                 txtNr; 
    int                txtMax; 
    xmlNodePtr        *txtTab; 
    xmlURL         *txturlTab; 

    xmlChar *             url; 
    int                 urlNr; 
    int                urlMax; 
    xmlChar *         *urlTab; 

    int              nbErrors; 
    int                legacy; 
    int            parseFlags; 
    xmlChar *		 base; 

    void            *_private; 
};

static int
xmlXIncludeDoProcess(xmlXIncludeCtxtPtr ctxt, xmlDocPtr doc, xmlNodePtr tree);



static void
xmlXIncludeErrMemory(xmlXIncludeCtxtPtr ctxt, xmlNodePtr node,
                     const char *extra)
{
    if (ctxt != NULL)
	ctxt->nbErrors++;
    __xmlRaiseError(NULL, NULL, NULL, ctxt, node, XML_FROM_XINCLUDE,
                    XML_ERR_NO_MEMORY, XML_ERR_ERROR, NULL, 0,
		    extra, NULL, NULL, 0, 0,
		    "Memory allocation failed : %s\n", extra);
}

static void
xmlXIncludeErr(xmlXIncludeCtxtPtr ctxt, xmlNodePtr node, int error,
               const char *msg, const xmlChar *extra)
{
    if (ctxt != NULL)
	ctxt->nbErrors++;
    __xmlRaiseError(NULL, NULL, NULL, ctxt, node, XML_FROM_XINCLUDE,
                    error, XML_ERR_ERROR, NULL, 0,
		    (const char *) extra, NULL, NULL, 0, 0,
		    msg, (const char *) extra);
}

#if 0
static void
xmlXIncludeWarn(xmlXIncludeCtxtPtr ctxt, xmlNodePtr node, int error,
               const char *msg, const xmlChar *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, ctxt, node, XML_FROM_XINCLUDE,
                    error, XML_ERR_WARNING, NULL, 0,
		    (const char *) extra, NULL, NULL, 0, 0,
		    msg, (const char *) extra);
}
#endif

static xmlChar *
xmlXIncludeGetProp(xmlXIncludeCtxtPtr ctxt, xmlNodePtr cur,
                   const xmlChar *name) {
    xmlChar *ret;

    ret = xmlGetNsProp(cur, XINCLUDE_NS, name);
    if (ret != NULL)
        return(ret);
    if (ctxt->legacy != 0) {
	ret = xmlGetNsProp(cur, XINCLUDE_OLD_NS, name);
	if (ret != NULL)
	    return(ret);
    }
    ret = xmlGetProp(cur, name);
    return(ret);
}
static void
xmlXIncludeFreeRef(xmlXIncludeRefPtr ref) {
    if (ref == NULL)
	return;
#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Freeing ref\n");
#endif
    if (ref->doc != NULL) {
#ifdef DEBUG_XINCLUDE
	xmlGenericError(xmlGenericErrorContext, "Freeing doc %s\n", ref->URI);
#endif
	xmlFreeDoc(ref->doc);
    }
    if (ref->URI != NULL)
	xmlFree(ref->URI);
    if (ref->fragment != NULL)
	xmlFree(ref->fragment);
    if (ref->xptr != NULL)
	xmlXPathFreeObject(ref->xptr);
    xmlFree(ref);
}

static xmlXIncludeRefPtr
xmlXIncludeNewRef(xmlXIncludeCtxtPtr ctxt, const xmlChar *URI,
	          xmlNodePtr ref) {
    xmlXIncludeRefPtr ret;

#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "New ref %s\n", URI);
#endif
    ret = (xmlXIncludeRefPtr) xmlMalloc(sizeof(xmlXIncludeRef));
    if (ret == NULL) {
        xmlXIncludeErrMemory(ctxt, ref, "growing XInclude context");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlXIncludeRef));
    if (URI == NULL)
	ret->URI = NULL;
    else
	ret->URI = xmlStrdup(URI);
    ret->fragment = NULL;
    ret->ref = ref;
    ret->doc = NULL;
    ret->count = 0;
    ret->xml = 0;
    ret->inc = NULL;
    if (ctxt->incMax == 0) {
	ctxt->incMax = 4;
        ctxt->incTab = (xmlXIncludeRefPtr *) xmlMalloc(ctxt->incMax *
					      sizeof(ctxt->incTab[0]));
        if (ctxt->incTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, ref, "growing XInclude context");
	    xmlXIncludeFreeRef(ret);
	    return(NULL);
	}
    }
    if (ctxt->incNr >= ctxt->incMax) {
	ctxt->incMax *= 2;
        ctxt->incTab = (xmlXIncludeRefPtr *) xmlRealloc(ctxt->incTab,
	             ctxt->incMax * sizeof(ctxt->incTab[0]));
        if (ctxt->incTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, ref, "growing XInclude context");
	    xmlXIncludeFreeRef(ret);
	    return(NULL);
	}
    }
    ctxt->incTab[ctxt->incNr++] = ret;
    return(ret);
}

xmlXIncludeCtxtPtr
xmlXIncludeNewContext(xmlDocPtr doc) {
    xmlXIncludeCtxtPtr ret;

#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "New context\n");
#endif
    if (doc == NULL)
	return(NULL);
    ret = (xmlXIncludeCtxtPtr) xmlMalloc(sizeof(xmlXIncludeCtxt));
    if (ret == NULL) {
	xmlXIncludeErrMemory(NULL, (xmlNodePtr) doc,
	                     "creating XInclude context");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlXIncludeCtxt));
    ret->doc = doc;
    ret->incNr = 0;
    ret->incBase = 0;
    ret->incMax = 0;
    ret->incTab = NULL;
    ret->nbErrors = 0;
    return(ret);
}

static int
xmlXIncludeURLPush(xmlXIncludeCtxtPtr ctxt,
	           const xmlChar *value)
{
    if (ctxt->urlNr > XINCLUDE_MAX_DEPTH) {
	xmlXIncludeErr(ctxt, NULL, XML_XINCLUDE_RECURSION,
	               "detected a recursion in %s\n", value);
	return(-1);
    }
    if (ctxt->urlTab == NULL) {
	ctxt->urlMax = 4;
	ctxt->urlNr = 0;
	ctxt->urlTab = (xmlChar * *) xmlMalloc(
		        ctxt->urlMax * sizeof(ctxt->urlTab[0]));
        if (ctxt->urlTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, NULL, "adding URL");
            return (-1);
        }
    }
    if (ctxt->urlNr >= ctxt->urlMax) {
        ctxt->urlMax *= 2;
        ctxt->urlTab =
            (xmlChar * *) xmlRealloc(ctxt->urlTab,
                                      ctxt->urlMax *
                                      sizeof(ctxt->urlTab[0]));
        if (ctxt->urlTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, NULL, "adding URL");
            return (-1);
        }
    }
    ctxt->url = ctxt->urlTab[ctxt->urlNr] = xmlStrdup(value);
    return (ctxt->urlNr++);
}

static void
xmlXIncludeURLPop(xmlXIncludeCtxtPtr ctxt)
{
    xmlChar * ret;

    if (ctxt->urlNr <= 0)
        return;
    ctxt->urlNr--;
    if (ctxt->urlNr > 0)
        ctxt->url = ctxt->urlTab[ctxt->urlNr - 1];
    else
        ctxt->url = NULL;
    ret = ctxt->urlTab[ctxt->urlNr];
    ctxt->urlTab[ctxt->urlNr] = NULL;
    if (ret != NULL)
	xmlFree(ret);
}

void
xmlXIncludeFreeContext(xmlXIncludeCtxtPtr ctxt) {
    int i;

#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Freeing context\n");
#endif
    if (ctxt == NULL)
	return;
    while (ctxt->urlNr > 0)
	xmlXIncludeURLPop(ctxt);
    if (ctxt->urlTab != NULL)
	xmlFree(ctxt->urlTab);
    for (i = 0;i < ctxt->incNr;i++) {
	if (ctxt->incTab[i] != NULL)
	    xmlXIncludeFreeRef(ctxt->incTab[i]);
    }
    if (ctxt->txturlTab != NULL) {
	for (i = 0;i < ctxt->txtNr;i++) {
	    if (ctxt->txturlTab[i] != NULL)
		xmlFree(ctxt->txturlTab[i]);
	}
    }
    if (ctxt->incTab != NULL)
	xmlFree(ctxt->incTab);
    if (ctxt->txtTab != NULL)
	xmlFree(ctxt->txtTab);
    if (ctxt->txturlTab != NULL)
	xmlFree(ctxt->txturlTab);
    if (ctxt->base != NULL) {
        xmlFree(ctxt->base);
    }
    xmlFree(ctxt);
}

static xmlDocPtr
xmlXIncludeParseFile(xmlXIncludeCtxtPtr ctxt, const char *URL) {
    xmlDocPtr ret;
    xmlParserCtxtPtr pctxt;
    xmlParserInputPtr inputStream;

    xmlInitParser();

    pctxt = xmlNewParserCtxt();
    if (pctxt == NULL) {
	xmlXIncludeErrMemory(ctxt, NULL, "cannot allocate parser context");
	return(NULL);
    }

    pctxt->_private = ctxt->_private;
    
    if ((ctxt->doc != NULL) && (ctxt->doc->dict != NULL)) {
       if (pctxt->dict != NULL)
            xmlDictFree(pctxt->dict);
	pctxt->dict = ctxt->doc->dict;
	xmlDictReference(pctxt->dict);
    }

    xmlCtxtUseOptions(pctxt, ctxt->parseFlags | XML_PARSE_DTDLOAD);
    
    inputStream = xmlLoadExternalEntity(URL, NULL, pctxt);
    if (inputStream == NULL) {
	xmlFreeParserCtxt(pctxt);
	return(NULL);
    }

    inputPush(pctxt, inputStream);

    if (pctxt->directory == NULL)
        pctxt->directory = xmlParserGetDirectory(URL);

    pctxt->loadsubset |= XML_DETECT_IDS;

    xmlParseDocument(pctxt);

    if (pctxt->wellFormed) {
        ret = pctxt->myDoc;
    }
    else {
        ret = NULL;
	if (pctxt->myDoc != NULL)
	    xmlFreeDoc(pctxt->myDoc);
        pctxt->myDoc = NULL;
    }
    xmlFreeParserCtxt(pctxt);
    
    return(ret);
}

static int
xmlXIncludeAddNode(xmlXIncludeCtxtPtr ctxt, xmlNodePtr cur) {
    xmlXIncludeRefPtr ref;
    xmlURIPtr uri;
    xmlChar *URL;
    xmlChar *fragment = NULL;
    xmlChar *href;
    xmlChar *parse;
    xmlChar *base;
    xmlChar *URI;
    int xml = 1, i; 
    int local = 0;


    if (ctxt == NULL)
	return(-1);
    if (cur == NULL)
	return(-1);

#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Add node\n");
#endif
    href = xmlXIncludeGetProp(ctxt, cur, XINCLUDE_HREF);
    if (href == NULL) {
	href = xmlStrdup(BAD_CAST ""); 
	if (href == NULL) 
	    return(-1);
    }
    if ((href[0] == '#') || (href[0] == 0))
	local = 1;
    parse = xmlXIncludeGetProp(ctxt, cur, XINCLUDE_PARSE);
    if (parse != NULL) {
	if (xmlStrEqual(parse, XINCLUDE_PARSE_XML))
	    xml = 1;
	else if (xmlStrEqual(parse, XINCLUDE_PARSE_TEXT))
	    xml = 0;
	else {
	    xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_PARSE_VALUE,
	                   "invalid value %s for 'parse'\n", parse);
	    if (href != NULL)
		xmlFree(href);
	    if (parse != NULL)
		xmlFree(parse);
	    return(-1);
	}
    }

    base = xmlNodeGetBase(ctxt->doc, cur);
    if (base == NULL) {
	URI = xmlBuildURI(href, ctxt->doc->URL);
    } else {
	URI = xmlBuildURI(href, base);
    }
    if (URI == NULL) {
	xmlChar *escbase;
	xmlChar *eschref;
	escbase = xmlURIEscape(base);
	eschref = xmlURIEscape(href);
	URI = xmlBuildURI(eschref, escbase);
	if (escbase != NULL)
	    xmlFree(escbase);
	if (eschref != NULL)
	    xmlFree(eschref);
    }
    if (parse != NULL)
	xmlFree(parse);
    if (href != NULL)
	xmlFree(href);
    if (base != NULL)
	xmlFree(base);
    if (URI == NULL) {
	xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_HREF_URI,
	               "failed build URL\n", NULL);
	return(-1);
    }
    fragment = xmlXIncludeGetProp(ctxt, cur, XINCLUDE_PARSE_XPOINTER);

    uri = xmlParseURI((const char *)URI);
    if (uri == NULL) {
	xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_HREF_URI,
	               "invalid value URI %s\n", URI);
	if (fragment != NULL)
	    xmlFree(fragment);
	xmlFree(URI);
	return(-1);
    }

    if (uri->fragment != NULL) {
        if (ctxt->legacy != 0) {
	    if (fragment == NULL) {
		fragment = (xmlChar *) uri->fragment;
	    } else {
		xmlFree(uri->fragment);
	    }
	} else {
	    xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_FRAGMENT_ID,
       "Invalid fragment identifier in URI %s use the xpointer attribute\n",
                           URI);
	    if (fragment != NULL)
	        xmlFree(fragment);
	    xmlFreeURI(uri);
	    xmlFree(URI);
	    return(-1);
	}
	uri->fragment = NULL;
    }
    URL = xmlSaveUri(uri);
    xmlFreeURI(uri);
    xmlFree(URI);
    if (URL == NULL) {
	xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_HREF_URI,
	               "invalid value URI %s\n", URI);
	if (fragment != NULL)
	    xmlFree(fragment);
	return(-1);
    }

    if ((local == 1) && (xml == 1) &&
        ((fragment == NULL) || (fragment[0] == 0))) {
	xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_RECURSION,
	               "detected a local recursion with no xpointer in %s\n",
		       URL);
	if (fragment != NULL)
	    xmlFree(fragment);
	return(-1);
    }

    if ((!local) && (xml == 1)) {
	for (i = 0;i < ctxt->urlNr;i++) {
	    if (xmlStrEqual(URL, ctxt->urlTab[i])) {
		xmlXIncludeErr(ctxt, cur, XML_XINCLUDE_RECURSION,
		               "detected a recursion in %s\n", URL);
		return(-1);
	    }
	}
    }

    ref = xmlXIncludeNewRef(ctxt, URL, cur);
    if (ref == NULL) {
	return(-1);
    }
    ref->fragment = fragment;
    ref->doc = NULL;
    ref->xml = xml;
    ref->count = 1;
    xmlFree(URL);
    return(0);
}

static void
xmlXIncludeRecurseDoc(xmlXIncludeCtxtPtr ctxt, xmlDocPtr doc,
	              const xmlURL url ATTRIBUTE_UNUSED) {
    xmlXIncludeCtxtPtr newctxt;
    int i;


#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Recursing in doc %s\n", doc->URL);
#endif

    newctxt = xmlXIncludeNewContext(doc);
    if (newctxt != NULL) {
	newctxt->_private = ctxt->_private;	
	newctxt->incMax = ctxt->incMax;
	newctxt->incNr = ctxt->incNr;
        newctxt->incTab = (xmlXIncludeRefPtr *) xmlMalloc(newctxt->incMax *
		                          sizeof(newctxt->incTab[0]));
        if (newctxt->incTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, (xmlNodePtr) doc, "processing doc");
	    xmlFree(newctxt);
	    return;
	}
	newctxt->urlMax = ctxt->urlMax;
	newctxt->urlNr = ctxt->urlNr;
	newctxt->urlTab = ctxt->urlTab;

	newctxt->base = xmlStrdup(ctxt->base);

	newctxt->incBase = ctxt->incNr;
	for (i = 0;i < ctxt->incNr;i++) {
	    newctxt->incTab[i] = ctxt->incTab[i];
	    newctxt->incTab[i]->count++; 
	}
	newctxt->parseFlags = ctxt->parseFlags;
	xmlXIncludeDoProcess(newctxt, doc, xmlDocGetRootElement(doc));
	for (i = 0;i < ctxt->incNr;i++) {
	    newctxt->incTab[i]->count--;
	    newctxt->incTab[i] = NULL;
	}

	
	ctxt->urlTab = newctxt->urlTab;
	ctxt->urlMax = newctxt->urlMax;

	newctxt->urlMax = 0;
	newctxt->urlNr = 0;
	newctxt->urlTab = NULL;

	xmlXIncludeFreeContext(newctxt);
    }
#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Done recursing in doc %s\n", url);
#endif
}

static void
xmlXIncludeAddTxt(xmlXIncludeCtxtPtr ctxt, xmlNodePtr txt, const xmlURL url) {
#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Adding text %s\n", url);
#endif
    if (ctxt->txtMax == 0) {
	ctxt->txtMax = 4;
        ctxt->txtTab = (xmlNodePtr *) xmlMalloc(ctxt->txtMax *
		                          sizeof(ctxt->txtTab[0]));
        if (ctxt->txtTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, NULL, "processing text");
	    return;
	}
        ctxt->txturlTab = (xmlURL *) xmlMalloc(ctxt->txtMax *
		                          sizeof(ctxt->txturlTab[0]));
        if (ctxt->txturlTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, NULL, "processing text");
	    return;
	}
    }
    if (ctxt->txtNr >= ctxt->txtMax) {
	ctxt->txtMax *= 2;
        ctxt->txtTab = (xmlNodePtr *) xmlRealloc(ctxt->txtTab,
	             ctxt->txtMax * sizeof(ctxt->txtTab[0]));
        if (ctxt->txtTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, NULL, "processing text");
	    return;
	}
        ctxt->txturlTab = (xmlURL *) xmlRealloc(ctxt->txturlTab,
	             ctxt->txtMax * sizeof(ctxt->txturlTab[0]));
        if (ctxt->txturlTab == NULL) {
	    xmlXIncludeErrMemory(ctxt, NULL, "processing text");
	    return;
	}
    }
    ctxt->txtTab[ctxt->txtNr] = txt;
    ctxt->txturlTab[ctxt->txtNr] = xmlStrdup(url);
    ctxt->txtNr++;
}


static xmlNodePtr
xmlXIncludeCopyNodeList(xmlXIncludeCtxtPtr ctxt, xmlDocPtr target,
	                xmlDocPtr source, xmlNodePtr elem);

static xmlNodePtr
xmlXIncludeCopyNode(xmlXIncludeCtxtPtr ctxt, xmlDocPtr target,
	            xmlDocPtr source, xmlNodePtr elem) {
    xmlNodePtr result = NULL;

    if ((ctxt == NULL) || (target == NULL) || (source == NULL) ||
	(elem == NULL))
	return(NULL);
    if (elem->type == XML_DTD_NODE)
	return(NULL);
    if (elem->type == XML_DOCUMENT_NODE)
	result = xmlXIncludeCopyNodeList(ctxt, target, source, elem->children);
    else
        result = xmlDocCopyNode(elem, target, 1);
    return(result);
}

static xmlNodePtr
xmlXIncludeCopyNodeList(xmlXIncludeCtxtPtr ctxt, xmlDocPtr target,
	                xmlDocPtr source, xmlNodePtr elem) {
    xmlNodePtr cur, res, result = NULL, last = NULL;

    if ((ctxt == NULL) || (target == NULL) || (source == NULL) ||
	(elem == NULL))
	return(NULL);
    cur = elem;
    while (cur != NULL) {
	res = xmlXIncludeCopyNode(ctxt, target, source, cur);
	if (res != NULL) {
	    if (result == NULL) {
		result = last = res;
	    } else {
		last->next = res;
		res->prev = last;
		last = res;
	    }
	}
	cur = cur->next;
    }
    return(result);
}

static xmlNodePtr
xmlXIncludeGetNthChild(xmlNodePtr cur, int no) {
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

xmlNodePtr xmlXPtrAdvanceNode(xmlNodePtr cur, int *level); 
static xmlNodePtr
xmlXIncludeCopyRange(xmlXIncludeCtxtPtr ctxt, xmlDocPtr target,
	                xmlDocPtr source, xmlXPathObjectPtr range) {
    
    xmlNodePtr list = NULL, last = NULL, listParent = NULL;
    xmlNodePtr tmp, tmp2;
    
    xmlNodePtr start, cur, end;
    int index1, index2;
    int level = 0, lastLevel = 0, endLevel = 0, endFlag = 0;

    if ((ctxt == NULL) || (target == NULL) || (source == NULL) ||
	(range == NULL))
	return(NULL);
    if (range->type != XPATH_RANGE)
	return(NULL);
    start = (xmlNodePtr) range->user;

    if (start == NULL)
	return(NULL);
    end = range->user2;
    if (end == NULL)
	return(xmlDocCopyNode(start, target, 1));

    cur = start;
    index1 = range->index;
    index2 = range->index2;
    while (cur != NULL) {
	if (level < 0) {
	    while (level < 0) {
	        
	        tmp2 = xmlDocCopyNode(listParent, target, 2);
	        xmlAddChild(tmp2, list);
	        list = tmp2;
	        listParent = listParent->parent;
	        level++;
	    }
	    last = list;
	    lastLevel = 0;
	}
	while (level < lastLevel) {
	    last = last->parent;
	    lastLevel --;
	}
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
		    } else {
			len = index2;
		    }
		    tmp = xmlNewTextLen(content, len);
		}
		
		if (list == NULL)
		    return(tmp);
		
		if (level == lastLevel)
		    xmlAddNextSibling(last, tmp);
		else 
		    xmlAddChild(last, tmp);
		return(list);
	    } else {	
	        endLevel = level;	
		endFlag = 1;
		
		tmp = xmlDocCopyNode(cur, target, 2);
		if (list == NULL) {
		    list = tmp;
		    listParent = cur->parent;
		} else {
		    if (level == lastLevel)
			xmlAddNextSibling(last, tmp);
		    else {
			xmlAddChild(last, tmp);
			lastLevel = level;
		    }
		}
		last = tmp;

		if (index2 > 1) {
		    end = xmlXIncludeGetNthChild(cur, index2 - 1);
		    index2 = 0;
		}
		if ((cur == start) && (index1 > 1)) {
		    cur = xmlXIncludeGetNthChild(cur, index1 - 1);
		    index1 = 0;
		}  else {
		    cur = cur->children;
		}
		level++;	
		continue;	
	    }
	} else if (cur == start) {	
	    if ((cur->type == XML_TEXT_NODE) ||
		(cur->type == XML_CDATA_SECTION_NODE)) {
		const xmlChar *content = cur->content;

		if (content == NULL) {
		    tmp = xmlNewTextLen(NULL, 0);
		} else {
		    if (index1 > 1) {
			content += (index1 - 1);
			index1 = 0;
		    }
		    tmp = xmlNewText(content);
		}
		last = list = tmp;
		listParent = cur->parent;
	    } else {		
		tmp = xmlDocCopyNode(cur, target, 2);
		list = last = tmp;
		listParent = cur->parent;
		if (index1 > 1) {	
		    cur = xmlXIncludeGetNthChild(cur, index1 - 1);
		    level = lastLevel = 1;
		    index1 = 0;
		    continue; 
		}
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
		    
		    break;
		case XML_XINCLUDE_START:
		case XML_XINCLUDE_END:
		    
		    break;
		case XML_ATTRIBUTE_NODE:
		    
		    break;
		default:
		    tmp = xmlDocCopyNode(cur, target, 2);
		    break;
	    }
	    if (tmp != NULL) {
		if (level == lastLevel)
		    xmlAddNextSibling(last, tmp);
		else {
		    xmlAddChild(last, tmp);
		    lastLevel = level;
		}
		last = tmp;
	    }
	}
	cur = xmlXPtrAdvanceNode(cur, &level);
	if (endFlag && (level >= endLevel))
	    break;
    }
    return(list);
}

static xmlNodePtr
xmlXIncludeCopyXPointer(xmlXIncludeCtxtPtr ctxt, xmlDocPtr target,
	                xmlDocPtr source, xmlXPathObjectPtr obj) {
    xmlNodePtr list = NULL, last = NULL;
    int i;

    if (source == NULL)
	source = ctxt->doc;
    if ((ctxt == NULL) || (target == NULL) || (source == NULL) ||
	(obj == NULL))
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
		    case XML_XINCLUDE_END:
			break;
		    case XML_XINCLUDE_START: {
	                xmlNodePtr tmp, cur = set->nodeTab[i];

			cur = cur->next;
			while (cur != NULL) {
			    switch(cur->type) {
				case XML_TEXT_NODE:
				case XML_CDATA_SECTION_NODE:
				case XML_ELEMENT_NODE:
				case XML_ENTITY_REF_NODE:
				case XML_ENTITY_NODE:
				case XML_PI_NODE:
				case XML_COMMENT_NODE:
				    tmp = xmlXIncludeCopyNode(ctxt, target,
							      source, cur);
				    if (last == NULL) {
					list = last = tmp;
				    } else {
					xmlAddNextSibling(last, tmp);
					last = tmp;
				    }
				    cur = cur->next;
				    continue;
				default:
				    break;
			    }
			    break;
			}
			continue;
		    }
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
		    list = last = xmlXIncludeCopyNode(ctxt, target, source,
			                              set->nodeTab[i]);
		else {
		    xmlAddNextSibling(last,
			    xmlXIncludeCopyNode(ctxt, target, source,
				                set->nodeTab[i]));
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
		    list = last = xmlXIncludeCopyXPointer(ctxt, target, source,
			                                  set->locTab[i]);
		else
		    xmlAddNextSibling(last,
			    xmlXIncludeCopyXPointer(ctxt, target, source,
				                    set->locTab[i]));
		if (last != NULL) {
		    while (last->next != NULL)
			last = last->next;
		}
	    }
	    break;
	}
#ifdef LIBXML_XPTR_ENABLED
	case XPATH_RANGE:
	    return(xmlXIncludeCopyRange(ctxt, target, source, obj));
#endif
	case XPATH_POINT:
	    
	    break;
	default:
	    break;
    }
    return(list);
}

typedef struct _xmlXIncludeMergeData xmlXIncludeMergeData;
typedef xmlXIncludeMergeData *xmlXIncludeMergeDataPtr;
struct _xmlXIncludeMergeData {
    xmlDocPtr doc;
    xmlXIncludeCtxtPtr ctxt;
};

static void
xmlXIncludeMergeEntity(xmlEntityPtr ent, xmlXIncludeMergeDataPtr data,
	               xmlChar *name ATTRIBUTE_UNUSED) {
    xmlEntityPtr ret, prev;
    xmlDocPtr doc;
    xmlXIncludeCtxtPtr ctxt;

    if ((ent == NULL) || (data == NULL))
	return;
    ctxt = data->ctxt;
    doc = data->doc;
    if ((ctxt == NULL) || (doc == NULL))
	return;
    switch (ent->etype) {
        case XML_INTERNAL_PARAMETER_ENTITY:
        case XML_EXTERNAL_PARAMETER_ENTITY:
        case XML_INTERNAL_PREDEFINED_ENTITY:
	    return;
        case XML_INTERNAL_GENERAL_ENTITY:
        case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
        case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
	    break;
    }
    ret = xmlAddDocEntity(doc, ent->name, ent->etype, ent->ExternalID,
			  ent->SystemID, ent->content);
    if (ret != NULL) {
	if (ent->URI != NULL)
	    ret->URI = xmlStrdup(ent->URI);
    } else {
	prev = xmlGetDocEntity(doc, ent->name);
	if (prev != NULL) {
	    if (ent->etype != prev->etype)
		goto error;
	
	    if ((ent->SystemID != NULL) && (prev->SystemID != NULL)) {
		if (!xmlStrEqual(ent->SystemID, prev->SystemID))
		    goto error;
	    } else if ((ent->ExternalID != NULL) &&
		       (prev->ExternalID != NULL)) {
		if (!xmlStrEqual(ent->ExternalID, prev->ExternalID))
		    goto error;
	    } else if ((ent->content != NULL) && (prev->content != NULL)) {
		if (!xmlStrEqual(ent->content, prev->content))
		    goto error;
	    } else {
		goto error;
	    }

	}
    }
    return;
error:
    switch (ent->etype) {
        case XML_INTERNAL_PARAMETER_ENTITY:
        case XML_EXTERNAL_PARAMETER_ENTITY:
        case XML_INTERNAL_PREDEFINED_ENTITY:
        case XML_INTERNAL_GENERAL_ENTITY:
        case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
	    return;
        case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
	    break;
    }
    xmlXIncludeErr(ctxt, (xmlNodePtr) ent, XML_XINCLUDE_ENTITY_DEF_MISMATCH,
                   "mismatch in redefinition of entity %s\n",
		   ent->name);
}

static int
xmlXIncludeMergeEntities(xmlXIncludeCtxtPtr ctxt, xmlDocPtr doc,
	                 xmlDocPtr from) {
    xmlNodePtr cur;
    xmlDtdPtr target, source;

    if (ctxt == NULL)
	return(-1);

    if ((from == NULL) || (from->intSubset == NULL))
	return(0);

    target = doc->intSubset;
    if (target == NULL) {
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL)
	    return(-1);
        target = xmlCreateIntSubset(doc, cur->name, NULL, NULL);
	if (target == NULL)
	    return(-1);
    }

    source = from->intSubset;
    if ((source != NULL) && (source->entities != NULL)) {
	xmlXIncludeMergeData data;

	data.ctxt = ctxt;
	data.doc = doc;

	xmlHashScan((xmlHashTablePtr) source->entities,
		    (xmlHashScanner) xmlXIncludeMergeEntity, &data);
    }
    source = from->extSubset;
    if ((source != NULL) && (source->entities != NULL)) {
	xmlXIncludeMergeData data;

	data.ctxt = ctxt;
	data.doc = doc;

	if ((!xmlStrEqual(target->ExternalID, source->ExternalID)) &&
	    (!xmlStrEqual(target->SystemID, source->SystemID))) {
	    xmlHashScan((xmlHashTablePtr) source->entities,
			(xmlHashScanner) xmlXIncludeMergeEntity, &data);
	}
    }
    return(0);
}

static int
xmlXIncludeLoadDoc(xmlXIncludeCtxtPtr ctxt, const xmlChar *url, int nr) {
    xmlDocPtr doc;
    xmlURIPtr uri;
    xmlChar *URL;
    xmlChar *fragment = NULL;
    int i = 0;
#ifdef LIBXML_XPTR_ENABLED
    int saveFlags;
#endif

#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "Loading doc %s:%d\n", url, nr);
#endif
    uri = xmlParseURI((const char *)url);
    if (uri == NULL) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	               XML_XINCLUDE_HREF_URI,
		       "invalid value URI %s\n", url);
	return(-1);
    }
    if (uri->fragment != NULL) {
	fragment = (xmlChar *) uri->fragment;
	uri->fragment = NULL;
    }
    if ((ctxt->incTab != NULL) && (ctxt->incTab[nr] != NULL) &&
        (ctxt->incTab[nr]->fragment != NULL)) {
	if (fragment != NULL) xmlFree(fragment);
	fragment = xmlStrdup(ctxt->incTab[nr]->fragment);
    }
    URL = xmlSaveUri(uri);
    xmlFreeURI(uri);
    if (URL == NULL) {
        if (ctxt->incTab != NULL)
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
			   XML_XINCLUDE_HREF_URI,
			   "invalid value URI %s\n", url);
	else
	    xmlXIncludeErr(ctxt, NULL,
			   XML_XINCLUDE_HREF_URI,
			   "invalid value URI %s\n", url);
	if (fragment != NULL)
	    xmlFree(fragment);
	return(-1);
    }

    if ((URL[0] == 0) || (URL[0] == '#') ||
	((ctxt->doc != NULL) && (xmlStrEqual(URL, ctxt->doc->URL)))) {
	doc = NULL;
        goto loaded;
    }

    for (i = 0; i < ctxt->incNr; i++) {
	if ((xmlStrEqual(URL, ctxt->incTab[i]->URI)) &&
	    (ctxt->incTab[i]->doc != NULL)) {
	    doc = ctxt->incTab[i]->doc;
#ifdef DEBUG_XINCLUDE
	    printf("Already loaded %s\n", URL);
#endif
	    goto loaded;
	}
    }

#ifdef DEBUG_XINCLUDE
    printf("loading %s\n", URL);
#endif
#ifdef LIBXML_XPTR_ENABLED
    saveFlags = ctxt->parseFlags;
    if (fragment != NULL) {	
	ctxt->parseFlags |= XML_PARSE_NOENT;
    }
#endif

    doc = xmlXIncludeParseFile(ctxt, (const char *)URL);
#ifdef LIBXML_XPTR_ENABLED
    ctxt->parseFlags = saveFlags;
#endif
    if (doc == NULL) {
	xmlFree(URL);
	if (fragment != NULL)
	    xmlFree(fragment);
	return(-1);
    }
    ctxt->incTab[nr]->doc = doc;
   if (!xmlStrEqual(URL, doc->URL)) {
       xmlFree(URL);
       URL = xmlStrdup(doc->URL);
   }
    for (i = nr + 1; i < ctxt->incNr; i++) {
	if (xmlStrEqual(URL, ctxt->incTab[i]->URI)) {
	    ctxt->incTab[nr]->count++;
#ifdef DEBUG_XINCLUDE
	    printf("Increasing %s count since reused\n", URL);
#endif
            break;
	}
    }

    xmlXIncludeMergeEntities(ctxt, ctxt->doc, doc);

    xmlXIncludeRecurseDoc(ctxt, doc, URL);

loaded:
    if (fragment == NULL) {
	if (doc == NULL)
	{
	    ctxt->incTab[nr]->inc = xmlCopyNodeList(ctxt->doc->children);
	} else {
	    ctxt->incTab[nr]->inc = xmlXIncludeCopyNodeList(ctxt, ctxt->doc,
		                                       doc, doc->children);
	}
    } 
#ifdef LIBXML_XPTR_ENABLED
    else {
	xmlXPathObjectPtr xptr;
	xmlXPathContextPtr xptrctxt;
	xmlNodeSetPtr set;

	if (doc == NULL) {
	    xptrctxt = xmlXPtrNewContext(ctxt->doc, ctxt->incTab[nr]->ref,
		                         NULL);
	} else {
	    xptrctxt = xmlXPtrNewContext(doc, NULL, NULL);
	}
	if (xptrctxt == NULL) {
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	                   XML_XINCLUDE_XPTR_FAILED,
			   "could not create XPointer context\n", NULL);
	    xmlFree(URL);
	    xmlFree(fragment);
	    return(-1);
	}
	xptr = xmlXPtrEval(fragment, xptrctxt);
	if (xptr == NULL) {
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref,
	                   XML_XINCLUDE_XPTR_FAILED,
			   "XPointer evaluation failed: #%s\n",
			   fragment);
	    xmlXPathFreeContext(xptrctxt);
	    xmlFree(URL);
	    xmlFree(fragment);
	    return(-1);
	}
	switch (xptr->type) {
	    case XPATH_UNDEFINED:
	    case XPATH_BOOLEAN:
	    case XPATH_NUMBER:
	    case XPATH_STRING:
	    case XPATH_POINT:
	    case XPATH_USERS:
	    case XPATH_XSLT_TREE:
		xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
		               XML_XINCLUDE_XPTR_RESULT,
			       "XPointer is not a range: #%s\n",
			       fragment);
		xmlXPathFreeContext(xptrctxt);
		xmlFree(URL);
		xmlFree(fragment);
		return(-1);
	    case XPATH_NODESET:
	        if ((xptr->nodesetval == NULL) ||
		    (xptr->nodesetval->nodeNr <= 0)) {
		    xmlXPathFreeContext(xptrctxt);
		    xmlFree(URL);
		    xmlFree(fragment);
		    return(-1);
		}

	    case XPATH_RANGE:
	    case XPATH_LOCATIONSET:
		break;
	}
	set = xptr->nodesetval;
	if (set != NULL) {
	    for (i = 0;i < set->nodeNr;i++) {
		if (set->nodeTab[i] == NULL)
		    continue;
		switch (set->nodeTab[i]->type) {
		    case XML_ELEMENT_NODE:
		    case XML_TEXT_NODE:
		    case XML_CDATA_SECTION_NODE:
		    case XML_ENTITY_REF_NODE:
		    case XML_ENTITY_NODE:
		    case XML_PI_NODE:
		    case XML_COMMENT_NODE:
		    case XML_DOCUMENT_NODE:
		    case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
		    case XML_DOCB_DOCUMENT_NODE:
#endif
			continue;

		    case XML_ATTRIBUTE_NODE:
			xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
			               XML_XINCLUDE_XPTR_RESULT,
				       "XPointer selects an attribute: #%s\n",
				       fragment);
			set->nodeTab[i] = NULL;
			continue;
		    case XML_NAMESPACE_DECL:
			xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
			               XML_XINCLUDE_XPTR_RESULT,
				       "XPointer selects a namespace: #%s\n",
				       fragment);
			set->nodeTab[i] = NULL;
			continue;
		    case XML_DOCUMENT_TYPE_NODE:
		    case XML_DOCUMENT_FRAG_NODE:
		    case XML_NOTATION_NODE:
		    case XML_DTD_NODE:
		    case XML_ELEMENT_DECL:
		    case XML_ATTRIBUTE_DECL:
		    case XML_ENTITY_DECL:
		    case XML_XINCLUDE_START:
		    case XML_XINCLUDE_END:
			xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
			               XML_XINCLUDE_XPTR_RESULT,
				   "XPointer selects unexpected nodes: #%s\n",
				       fragment);
			set->nodeTab[i] = NULL;
			set->nodeTab[i] = NULL;
			continue; 
		}
	    }
	}
	if (doc == NULL) {
	    ctxt->incTab[nr]->xptr = xptr;
	    ctxt->incTab[nr]->inc = NULL;
	} else {
	    ctxt->incTab[nr]->inc =
		xmlXIncludeCopyXPointer(ctxt, ctxt->doc, doc, xptr);
	    xmlXPathFreeObject(xptr);
	}
	xmlXPathFreeContext(xptrctxt);
	xmlFree(fragment);
    }
#endif

    if ((doc != NULL) && (URL != NULL) && (xmlStrchr(URL, (xmlChar) '/')) &&
        (!(ctxt->parseFlags & XML_PARSE_NOBASEFIX)) &&
	(!(doc->parseFlags & XML_PARSE_NOBASEFIX))) {
	xmlNodePtr node;
	xmlChar *base;
	xmlChar *curBase;

	base = xmlGetNsProp(ctxt->incTab[nr]->ref, BAD_CAST "base",
			XML_XML_NAMESPACE);
	if (base == NULL) {
	    curBase = xmlBuildRelativeURI(URL, ctxt->base);
	    if (curBase == NULL) {	
	        xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	               XML_XINCLUDE_HREF_URI,
		       "trying to build relative URI from %s\n", URL);
	    } else {
		
	        if (!xmlStrchr(curBase, (xmlChar) '/'))
		    xmlFree(curBase);
		else
		    base = curBase;
	    }
	}
	if (base != NULL) {	
	    node = ctxt->incTab[nr]->inc;
	    while (node != NULL) {
		
		if (node->type == XML_ELEMENT_NODE) {
		    curBase = xmlNodeGetBase(node->doc, node);
		    
		    if (curBase == NULL) {
			xmlNodeSetBase(node, base);
		    } else {
			if (xmlStrEqual(curBase, node->doc->URL)) {
			    xmlNodeSetBase(node, base);
			} else {
			    xmlChar *xmlBase;
			    xmlBase = xmlGetNsProp(node,
					    BAD_CAST "base",
					    XML_XML_NAMESPACE);
			    if (xmlBase != NULL) {
				xmlChar *relBase;
				relBase = xmlBuildURI(xmlBase, base);
				if (relBase == NULL) { 
				    xmlXIncludeErr(ctxt, 
						ctxt->incTab[nr]->ref,
						XML_XINCLUDE_HREF_URI,
					"trying to rebuild base from %s\n",
						xmlBase);
				} else {
				    xmlNodeSetBase(node, relBase);
				    xmlFree(relBase);
				}
				xmlFree(xmlBase);
			    }
			}
			xmlFree(curBase);
		    }
		}
	        node = node->next;
	    }
	    xmlFree(base);
	}
    }
    if ((nr < ctxt->incNr) && (ctxt->incTab[nr]->doc != NULL) &&
	(ctxt->incTab[nr]->count <= 1)) {
#ifdef DEBUG_XINCLUDE
        printf("freeing %s\n", ctxt->incTab[nr]->doc->URL);
#endif
	xmlFreeDoc(ctxt->incTab[nr]->doc);
	ctxt->incTab[nr]->doc = NULL;
    }
    xmlFree(URL);
    return(0);
}

static int
xmlXIncludeLoadTxt(xmlXIncludeCtxtPtr ctxt, const xmlChar *url, int nr) {
    xmlParserInputBufferPtr buf;
    xmlNodePtr node;
    xmlURIPtr uri;
    xmlChar *URL;
    int i;
    xmlChar *encoding = NULL;
    xmlCharEncoding enc = (xmlCharEncoding) 0;

    uri = xmlParseURI((const char *)url);
    if (uri == NULL) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, XML_XINCLUDE_HREF_URI,
	               "invalid value URI %s\n", url);
	return(-1);
    }
    if (uri->fragment != NULL) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, XML_XINCLUDE_TEXT_FRAGMENT,
	               "fragment identifier forbidden for text: %s\n",
		       (const xmlChar *) uri->fragment);
	xmlFreeURI(uri);
	return(-1);
    }
    URL = xmlSaveUri(uri);
    xmlFreeURI(uri);
    if (URL == NULL) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, XML_XINCLUDE_HREF_URI,
	               "invalid value URI %s\n", url);
	return(-1);
    }

    if (URL[0] == 0) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	               XML_XINCLUDE_TEXT_DOCUMENT,
		       "text serialization of document not available\n", NULL);
	xmlFree(URL);
	return(-1);
    }

    for (i = 0; i < ctxt->txtNr; i++) {
	if (xmlStrEqual(URL, ctxt->txturlTab[i])) {
	    node = xmlCopyNode(ctxt->txtTab[i], 1);
	    goto loaded;
	}
    }
    if ((ctxt->incTab[nr] != NULL) && (ctxt->incTab[nr]->ref != NULL)) {
	encoding = xmlGetProp(ctxt->incTab[nr]->ref, XINCLUDE_PARSE_ENCODING);
    }
    if (encoding != NULL) {
        enc = xmlParseCharEncoding((const char *) encoding);
	if (enc == XML_CHAR_ENCODING_ERROR) {
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref,
	                   XML_XINCLUDE_UNKNOWN_ENCODING,
			   "encoding %s not supported\n", encoding);
	    xmlFree(encoding);
	    xmlFree(URL);
	    return(-1);
	}
	xmlFree(encoding);
    }

    buf = xmlParserInputBufferCreateFilename((const char *)URL, enc);
    if (buf == NULL) {
	xmlFree(URL);
	return(-1);
    }
    node = xmlNewText(NULL);

    while (xmlParserInputBufferRead(buf, 128) > 0) {
	int len;
	const xmlChar *content;

	content = xmlBufferContent(buf->buffer);
	len = xmlBufferLength(buf->buffer);
	for (i = 0;i < len;) {
	    int cur;
	    int l;

	    cur = xmlStringCurrentChar(NULL, &content[i], &l);
	    if (!IS_CHAR(cur)) {
		xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref,
		               XML_XINCLUDE_INVALID_CHAR,
			       "%s contains invalid char\n", URL);
		xmlFreeParserInputBuffer(buf);
		xmlFree(URL);
		return(-1);
	    } else {
		xmlNodeAddContentLen(node, &content[i], l);
	    }
	    i += l;
	}
	xmlBufferShrink(buf->buffer, len);
    }
    xmlFreeParserInputBuffer(buf);
    xmlXIncludeAddTxt(ctxt, node, URL);

loaded:
    ctxt->incTab[nr]->inc = node;
    xmlFree(URL);
    return(0);
}

static int
xmlXIncludeLoadFallback(xmlXIncludeCtxtPtr ctxt, xmlNodePtr fallback, int nr) {
    xmlXIncludeCtxtPtr newctxt;
    int ret = 0;
    
    if ((fallback == NULL) || (ctxt == NULL))
	return(-1);
    if (fallback->children != NULL) {
	newctxt = xmlXIncludeNewContext(ctxt->doc);
	if (newctxt == NULL)
	    return (-1);
	newctxt->_private = ctxt->_private;
	newctxt->base = xmlStrdup(ctxt->base);	
	xmlXIncludeSetFlags(newctxt, ctxt->parseFlags);
	ret = xmlXIncludeDoProcess(newctxt, ctxt->doc, fallback->children);
	if (ctxt->nbErrors > 0)
	    ret = -1;
	else if (ret > 0)
	    ret = 0;	
	xmlXIncludeFreeContext(newctxt);

	ctxt->incTab[nr]->inc = xmlDocCopyNodeList(ctxt->doc,
	                                           fallback->children);
    } else {
        ctxt->incTab[nr]->inc = NULL;
	ctxt->incTab[nr]->emptyFb = 1;	
    }
    return(ret);
}


static xmlNodePtr
xmlXIncludePreProcessNode(xmlXIncludeCtxtPtr ctxt, xmlNodePtr node) {
    xmlXIncludeAddNode(ctxt, node);
    return(NULL);
}

static int
xmlXIncludeLoadNode(xmlXIncludeCtxtPtr ctxt, int nr) {
    xmlNodePtr cur;
    xmlChar *href;
    xmlChar *parse;
    xmlChar *base;
    xmlChar *oldBase;
    xmlChar *URI;
    int xml = 1; 
    int ret;

    if (ctxt == NULL)
	return(-1);
    if ((nr < 0) || (nr >= ctxt->incNr))
	return(-1);
    cur = ctxt->incTab[nr]->ref;
    if (cur == NULL)
	return(-1);

    href = xmlXIncludeGetProp(ctxt, cur, XINCLUDE_HREF);
    if (href == NULL) {
	href = xmlStrdup(BAD_CAST ""); 
	if (href == NULL) 
	    return(-1);
    }
    parse = xmlXIncludeGetProp(ctxt, cur, XINCLUDE_PARSE);
    if (parse != NULL) {
	if (xmlStrEqual(parse, XINCLUDE_PARSE_XML))
	    xml = 1;
	else if (xmlStrEqual(parse, XINCLUDE_PARSE_TEXT))
	    xml = 0;
	else {
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref,
	                   XML_XINCLUDE_PARSE_VALUE,
			   "invalid value %s for 'parse'\n", parse);
	    if (href != NULL)
		xmlFree(href);
	    if (parse != NULL)
		xmlFree(parse);
	    return(-1);
	}
    }

    base = xmlNodeGetBase(ctxt->doc, cur);
    if (base == NULL) {
	URI = xmlBuildURI(href, ctxt->doc->URL);
    } else {
	URI = xmlBuildURI(href, base);
    }
    if (URI == NULL) {
	xmlChar *escbase;
	xmlChar *eschref;
	escbase = xmlURIEscape(base);
	eschref = xmlURIEscape(href);
	URI = xmlBuildURI(eschref, escbase);
	if (escbase != NULL)
	    xmlFree(escbase);
	if (eschref != NULL)
	    xmlFree(eschref);
    }
    if (URI == NULL) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	               XML_XINCLUDE_HREF_URI, "failed build URL\n", NULL);
	if (parse != NULL)
	    xmlFree(parse);
	if (href != NULL)
	    xmlFree(href);
	if (base != NULL)
	    xmlFree(base);
	return(-1);
    }
#ifdef DEBUG_XINCLUDE
    xmlGenericError(xmlGenericErrorContext, "parse: %s\n",
	    xml ? "xml": "text");
    xmlGenericError(xmlGenericErrorContext, "URI: %s\n", URI);
#endif

    oldBase = ctxt->base;
    ctxt->base = base;

    if (xml) {
	ret = xmlXIncludeLoadDoc(ctxt, URI, nr);
	
    } else {
	ret = xmlXIncludeLoadTxt(ctxt, URI, nr);
    }

    ctxt->base = oldBase;
    
    if (ret < 0) {
	xmlNodePtr children;

#ifdef DEBUG_XINCLUDE
	xmlGenericError(xmlGenericErrorContext, "error looking for fallback\n");
#endif
	children = cur->children;
	while (children != NULL) {
	    if ((children->type == XML_ELEMENT_NODE) &&
		(children->ns != NULL) &&
		(xmlStrEqual(children->name, XINCLUDE_FALLBACK)) &&
		((xmlStrEqual(children->ns->href, XINCLUDE_NS)) ||
		 (xmlStrEqual(children->ns->href, XINCLUDE_OLD_NS)))) {
		ret = xmlXIncludeLoadFallback(ctxt, children, nr);
		if (ret == 0) 
		    break;
	    }
	    children = children->next;
	}
    }
    if (ret < 0) {
	xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	               XML_XINCLUDE_NO_FALLBACK,
		       "could not load %s, and no fallback was found\n",
		       URI);
    }

    if (URI != NULL)
	xmlFree(URI);
    if (parse != NULL)
	xmlFree(parse);
    if (href != NULL)
	xmlFree(href);
    if (base != NULL)
	xmlFree(base);
    return(0);
}

static int
xmlXIncludeIncludeNode(xmlXIncludeCtxtPtr ctxt, int nr) {
    xmlNodePtr cur, end, list, tmp;

    if (ctxt == NULL)
	return(-1);
    if ((nr < 0) || (nr >= ctxt->incNr))
	return(-1);
    cur = ctxt->incTab[nr]->ref;
    if (cur == NULL)
	return(-1);

    if ((ctxt->incTab[nr]->inc == NULL) &&
	(ctxt->incTab[nr]->xptr != NULL)) {
	ctxt->incTab[nr]->inc =
	    xmlXIncludeCopyXPointer(ctxt, ctxt->doc, ctxt->doc,
		                    ctxt->incTab[nr]->xptr);
	xmlXPathFreeObject(ctxt->incTab[nr]->xptr);
	ctxt->incTab[nr]->xptr = NULL;
    }
    list = ctxt->incTab[nr]->inc;
    ctxt->incTab[nr]->inc = NULL;

    if ((cur->parent != NULL) &&
	(cur->parent->type != XML_ELEMENT_NODE)) {
	int nb_elem = 0;

	tmp = list;
	while (tmp != NULL) {
	    if (tmp->type == XML_ELEMENT_NODE)
		nb_elem++;
	    tmp = tmp->next;
	}
	if (nb_elem > 1) {
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref, 
	                   XML_XINCLUDE_MULTIPLE_ROOT,
		       "XInclude error: would result in multiple root nodes\n",
			   NULL);
	    return(-1);
	}
    }

    if (ctxt->parseFlags & XML_PARSE_NOXINCNODE) {
	while (list != NULL) {
	    end = list;
	    list = list->next;

	    xmlAddPrevSibling(cur, end);
	}
	xmlUnlinkNode(cur);
	xmlFreeNode(cur);
    } else {
	cur->type = XML_XINCLUDE_START;
	end = xmlNewDocNode(cur->doc, cur->ns, cur->name, NULL);
	if (end == NULL) {
	    xmlXIncludeErr(ctxt, ctxt->incTab[nr]->ref,
	                   XML_XINCLUDE_BUILD_FAILED,
			   "failed to build node\n", NULL);
	    return(-1);
	}
	end->type = XML_XINCLUDE_END;
	xmlAddNextSibling(cur, end);

	while (list != NULL) {
	    cur = list;
	    list = list->next;

	    xmlAddPrevSibling(end, cur);
	}
    }

    
    return(0);
}

static int
xmlXIncludeTestNode(xmlXIncludeCtxtPtr ctxt, xmlNodePtr node) {
    if (node == NULL)
	return(0);
    if (node->type != XML_ELEMENT_NODE)
	return(0);
    if (node->ns == NULL)
	return(0);
    if ((xmlStrEqual(node->ns->href, XINCLUDE_NS)) ||
        (xmlStrEqual(node->ns->href, XINCLUDE_OLD_NS))) {
	if (xmlStrEqual(node->ns->href, XINCLUDE_OLD_NS)) {
	    if (ctxt->legacy == 0) {
#if 0 
		xmlXIncludeWarn(ctxt, node, XML_XINCLUDE_DEPRECATED_NS,
	               "Deprecated XInclude namespace found, use %s",
		                XINCLUDE_NS);
#endif
	        ctxt->legacy = 1;
	    }
	}
	if (xmlStrEqual(node->name, XINCLUDE_NODE)) {
	    xmlNodePtr child = node->children;
	    int nb_fallback = 0;

	    while (child != NULL) {
		if ((child->type == XML_ELEMENT_NODE) &&
		    (child->ns != NULL) &&
		    ((xmlStrEqual(child->ns->href, XINCLUDE_NS)) ||
		     (xmlStrEqual(child->ns->href, XINCLUDE_OLD_NS)))) {
		    if (xmlStrEqual(child->name, XINCLUDE_NODE)) {
			xmlXIncludeErr(ctxt, node,
			               XML_XINCLUDE_INCLUDE_IN_INCLUDE,
				       "%s has an 'include' child\n",
				       XINCLUDE_NODE);
			return(0);
		    }
		    if (xmlStrEqual(child->name, XINCLUDE_FALLBACK)) {
			nb_fallback++;
		    }
		}
		child = child->next;
	    }
	    if (nb_fallback > 1) {
		xmlXIncludeErr(ctxt, node, XML_XINCLUDE_FALLBACKS_IN_INCLUDE,
			       "%s has multiple fallback children\n",
		               XINCLUDE_NODE);
		return(0);
	    }
	    return(1);
	}
	if (xmlStrEqual(node->name, XINCLUDE_FALLBACK)) {
	    if ((node->parent == NULL) ||
		(node->parent->type != XML_ELEMENT_NODE) ||
		(node->parent->ns == NULL) ||
		((!xmlStrEqual(node->parent->ns->href, XINCLUDE_NS)) &&
		 (!xmlStrEqual(node->parent->ns->href, XINCLUDE_OLD_NS))) ||
		(!xmlStrEqual(node->parent->name, XINCLUDE_NODE))) {
		xmlXIncludeErr(ctxt, node,
		               XML_XINCLUDE_FALLBACK_NOT_IN_INCLUDE,
			       "%s is not the child of an 'include'\n",
			       XINCLUDE_FALLBACK);
	    }
	}
    }
    return(0);
}

static int
xmlXIncludeDoProcess(xmlXIncludeCtxtPtr ctxt, xmlDocPtr doc, xmlNodePtr tree) {
    xmlNodePtr cur;
    int ret = 0;
    int i, start;

    if ((doc == NULL) || (tree == NULL))
	return(-1);
    if (ctxt == NULL)
	return(-1);

    if (doc->URL != NULL) {
	ret = xmlXIncludeURLPush(ctxt, doc->URL);
	if (ret < 0)
	    return(-1);
    }
    start = ctxt->incNr;

    cur = tree;
    if (xmlXIncludeTestNode(ctxt, cur) == 1)
	xmlXIncludePreProcessNode(ctxt, cur);
    while ((cur != NULL) && (cur != tree->parent)) {
	
	if ((cur->children != NULL) &&
	    (cur->children->type != XML_ENTITY_DECL) &&
	    (cur->children->type != XML_XINCLUDE_START) &&
	    (cur->children->type != XML_XINCLUDE_END)) {
	    cur = cur->children;
	    if (xmlXIncludeTestNode(ctxt, cur))
		xmlXIncludePreProcessNode(ctxt, cur);
	} else if (cur->next != NULL) {
	    cur = cur->next;
	    if (xmlXIncludeTestNode(ctxt, cur))
		xmlXIncludePreProcessNode(ctxt, cur);
	} else {
	    if (cur == tree)
	        break;
	    do {
		cur = cur->parent;
		if ((cur == NULL) || (cur == tree->parent))
		    break; 
		if (cur->next != NULL) {
		    cur = cur->next;
		    if (xmlXIncludeTestNode(ctxt, cur))
			xmlXIncludePreProcessNode(ctxt, cur);
		    break; 
		}
	    } while (cur != NULL);
	}
    }

    for (i = start;i < ctxt->incNr; i++) {
        xmlXIncludeLoadNode(ctxt, i);
	ret++;
    }

    for (i = ctxt->incBase;i < ctxt->incNr; i++) {
	if ((ctxt->incTab[i]->inc != NULL) ||
		(ctxt->incTab[i]->xptr != NULL) ||
		(ctxt->incTab[i]->emptyFb != 0))	
	    xmlXIncludeIncludeNode(ctxt, i);
    }

    if (doc->URL != NULL)
	xmlXIncludeURLPop(ctxt);
    return(ret);
}

int
xmlXIncludeSetFlags(xmlXIncludeCtxtPtr ctxt, int flags) {
    if (ctxt == NULL)
        return(-1);
    ctxt->parseFlags = flags;
    return(0);
}


int
xmlXIncludeProcessTreeFlagsData(xmlNodePtr tree, int flags, void *data) {
    xmlXIncludeCtxtPtr ctxt;
    int ret = 0;

    if ((tree == NULL) || (tree->doc == NULL))
        return(-1);

    ctxt = xmlXIncludeNewContext(tree->doc);
    if (ctxt == NULL)
        return(-1);
    ctxt->_private = data;
    ctxt->base = xmlStrdup((xmlChar *)tree->doc->URL);
    xmlXIncludeSetFlags(ctxt, flags);
    ret = xmlXIncludeDoProcess(ctxt, tree->doc, tree);
    if ((ret >= 0) && (ctxt->nbErrors > 0))
        ret = -1;

    xmlXIncludeFreeContext(ctxt);
    return(ret);
}

int
xmlXIncludeProcessFlagsData(xmlDocPtr doc, int flags, void *data) {
    xmlNodePtr tree;

    if (doc == NULL)
	return(-1);
    tree = xmlDocGetRootElement(doc);
    if (tree == NULL)
	return(-1);
    return(xmlXIncludeProcessTreeFlagsData(tree, flags, data));
}

int
xmlXIncludeProcessFlags(xmlDocPtr doc, int flags) {
    return xmlXIncludeProcessFlagsData(doc, flags, NULL);
}

int
xmlXIncludeProcess(xmlDocPtr doc) {
    return(xmlXIncludeProcessFlags(doc, 0));
}

int
xmlXIncludeProcessTreeFlags(xmlNodePtr tree, int flags) {
    xmlXIncludeCtxtPtr ctxt;
    int ret = 0;

    if ((tree == NULL) || (tree->doc == NULL))
	return(-1);
    ctxt = xmlXIncludeNewContext(tree->doc);
    if (ctxt == NULL)
	return(-1);
    ctxt->base = xmlNodeGetBase(tree->doc, tree);
    xmlXIncludeSetFlags(ctxt, flags);
    ret = xmlXIncludeDoProcess(ctxt, tree->doc, tree);
    if ((ret >= 0) && (ctxt->nbErrors > 0))
	ret = -1;

    xmlXIncludeFreeContext(ctxt);
    return(ret);
}

int
xmlXIncludeProcessTree(xmlNodePtr tree) {
    return(xmlXIncludeProcessTreeFlags(tree, 0));
}

int
xmlXIncludeProcessNode(xmlXIncludeCtxtPtr ctxt, xmlNodePtr node) {
    int ret = 0;

    if ((node == NULL) || (node->doc == NULL) || (ctxt == NULL))
	return(-1);
    ret = xmlXIncludeDoProcess(ctxt, node->doc, node);
    if ((ret >= 0) && (ctxt->nbErrors > 0))
	ret = -1;
    return(ret);
}

#else 
#endif
#define bottom_xinclude
#include "elfgcchack.h"
