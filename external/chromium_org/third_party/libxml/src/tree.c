/*
 * tree.c : implementation of access function for an XML tree.
 *
 * References:
 *   XHTML 1.0 W3C REC: http://www.w3.org/TR/2002/REC-xhtml1-20020801/
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 *
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h> 
#include <limits.h>
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_ZLIB_H
#include <zlib.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>
#include <libxml/entities.h>
#include <libxml/valid.h>
#include <libxml/xmlerror.h>
#include <libxml/parserInternals.h>
#include <libxml/globals.h>
#ifdef LIBXML_HTML_ENABLED
#include <libxml/HTMLtree.h>
#endif
#ifdef LIBXML_DEBUG_ENABLED
#include <libxml/debugXML.h>
#endif

int __xmlRegisterCallbacks = 0;


static xmlNsPtr
xmlNewReconciliedNs(xmlDocPtr doc, xmlNodePtr tree, xmlNsPtr ns);

static xmlChar* xmlGetPropNodeValueInternal(xmlAttrPtr prop);

static void
xmlTreeErrMemory(const char *extra)
{
    __xmlSimpleError(XML_FROM_TREE, XML_ERR_NO_MEMORY, NULL, NULL, extra);
}

static void
xmlTreeErr(int code, xmlNodePtr node, const char *extra)
{
    const char *msg = NULL;

    switch(code) {
        case XML_TREE_INVALID_HEX:
	    msg = "invalid hexadecimal character value\n";
	    break;
	case XML_TREE_INVALID_DEC:
	    msg = "invalid decimal character value\n";
	    break;
	case XML_TREE_UNTERMINATED_ENTITY:
	    msg = "unterminated entity reference %15s\n";
	    break;
	case XML_TREE_NOT_UTF8:
	    msg = "string is not in UTF-8\n";
	    break;
	default:
	    msg = "unexpected error number\n";
    }
    __xmlSimpleError(XML_FROM_TREE, code, node, msg, extra);
}

const xmlChar xmlStringText[] = { 't', 'e', 'x', 't', 0 };
const xmlChar xmlStringTextNoenc[] =
              { 't', 'e', 'x', 't', 'n', 'o', 'e', 'n', 'c', 0 };
const xmlChar xmlStringComment[] = { 'c', 'o', 'm', 'm', 'e', 'n', 't', 0 };

static int xmlCompressMode = 0;
static int xmlCheckDTD = 1;

#define UPDATE_LAST_CHILD_AND_PARENT(n) if ((n) != NULL) {		\
    xmlNodePtr ulccur = (n)->children;					\
    if (ulccur == NULL) {						\
        (n)->last = NULL;						\
    } else {								\
        while (ulccur->next != NULL) {					\
		ulccur->parent = (n);					\
		ulccur = ulccur->next;					\
	}								\
	ulccur->parent = (n);						\
	(n)->last = ulccur;						\
}}

#define IS_STR_XML(str) ((str != NULL) && (str[0] == 'x') && \
  (str[1] == 'm') && (str[2] == 'l') && (str[3] == 0))


#include <libxml/hash.h>

#ifdef LIBXML_TREE_ENABLED
static xmlEntityPtr
xmlGetEntityFromDtd(xmlDtdPtr dtd, const xmlChar *name) {
    xmlEntitiesTablePtr table;

    if((dtd != NULL) && (dtd->entities != NULL)) {
	table = (xmlEntitiesTablePtr) dtd->entities;
	return((xmlEntityPtr) xmlHashLookup(table, name));
	
    }
    return(NULL);
}
static xmlEntityPtr
xmlGetParameterEntityFromDtd(xmlDtdPtr dtd, const xmlChar *name) {
    xmlEntitiesTablePtr table;

    if ((dtd != NULL) && (dtd->pentities != NULL)) {
	table = (xmlEntitiesTablePtr) dtd->pentities;
	return((xmlEntityPtr) xmlHashLookup(table, name));
	
    }
    return(NULL);
}
#endif 


xmlChar *
xmlBuildQName(const xmlChar *ncname, const xmlChar *prefix,
	      xmlChar *memory, int len) {
    int lenn, lenp;
    xmlChar *ret;

    if (ncname == NULL) return(NULL);
    if (prefix == NULL) return((xmlChar *) ncname);

    lenn = strlen((char *) ncname);
    lenp = strlen((char *) prefix);

    if ((memory == NULL) || (len < lenn + lenp + 2)) {
	ret = (xmlChar *) xmlMallocAtomic(lenn + lenp + 2);
	if (ret == NULL) {
	    xmlTreeErrMemory("building QName");
	    return(NULL);
	}
    } else {
	ret = memory;
    }
    memcpy(&ret[0], prefix, lenp);
    ret[lenp] = ':';
    memcpy(&ret[lenp + 1], ncname, lenn);
    ret[lenn + lenp + 1] = 0;
    return(ret);
}


xmlChar *
xmlSplitQName2(const xmlChar *name, xmlChar **prefix) {
    int len = 0;
    xmlChar *ret = NULL;

    if (prefix == NULL) return(NULL);
    *prefix = NULL;
    if (name == NULL) return(NULL);

#ifndef XML_XML_NAMESPACE
    
    if ((name[0] == 'x') && (name[1] == 'm') &&
        (name[2] == 'l') && (name[3] == ':'))
	return(NULL);
#endif

    
    if (name[0] == ':')
	return(NULL);

    while ((name[len] != 0) && (name[len] != ':'))
	len++;

    if (name[len] == 0)
	return(NULL);

    *prefix = xmlStrndup(name, len);
    if (*prefix == NULL) {
	xmlTreeErrMemory("QName split");
	return(NULL);
    }
    ret = xmlStrdup(&name[len + 1]);
    if (ret == NULL) {
	xmlTreeErrMemory("QName split");
	if (*prefix != NULL) {
	    xmlFree(*prefix);
	    *prefix = NULL;
	}
	return(NULL);
    }

    return(ret);
}


const xmlChar *
xmlSplitQName3(const xmlChar *name, int *len) {
    int l = 0;

    if (name == NULL) return(NULL);
    if (len == NULL) return(NULL);

    
    if (name[0] == ':')
	return(NULL);

    while ((name[l] != 0) && (name[l] != ':'))
	l++;

    if (name[l] == 0)
	return(NULL);

    *len = l;

    return(&name[l+1]);
}


#define CUR_SCHAR(s, l) xmlStringCurrentChar(NULL, s, &l)

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED) || defined(LIBXML_DEBUG_ENABLED) || defined (LIBXML_HTML_ENABLED) || defined(LIBXML_SAX1_ENABLED) || defined(LIBXML_HTML_ENABLED) || defined(LIBXML_WRITER_ENABLED) || defined(LIBXML_DOCB_ENABLED)
int
xmlValidateNCName(const xmlChar *value, int space) {
    const xmlChar *cur = value;
    int c,l;

    if (value == NULL)
        return(-1);

    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (((*cur >= 'a') && (*cur <= 'z')) || ((*cur >= 'A') && (*cur <= 'Z')) ||
	(*cur == '_'))
	cur++;
    else
	goto try_complex;
    while (((*cur >= 'a') && (*cur <= 'z')) ||
	   ((*cur >= 'A') && (*cur <= 'Z')) ||
	   ((*cur >= '0') && (*cur <= '9')) ||
	   (*cur == '_') || (*cur == '-') || (*cur == '.'))
	cur++;
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (*cur == 0)
	return(0);

try_complex:
    cur = value;
    c = CUR_SCHAR(cur, l);
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if ((!IS_LETTER(c)) && (c != '_'))
	return(1);
    cur += l;
    c = CUR_SCHAR(cur, l);
    while (IS_LETTER(c) || IS_DIGIT(c) || (c == '.') ||
	   (c == '-') || (c == '_') || IS_COMBINING(c) ||
	   IS_EXTENDER(c)) {
	cur += l;
	c = CUR_SCHAR(cur, l);
    }
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if (c != 0)
	return(1);

    return(0);
}
#endif

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
int
xmlValidateQName(const xmlChar *value, int space) {
    const xmlChar *cur = value;
    int c,l;

    if (value == NULL)
        return(-1);
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (((*cur >= 'a') && (*cur <= 'z')) || ((*cur >= 'A') && (*cur <= 'Z')) ||
	(*cur == '_'))
	cur++;
    else
	goto try_complex;
    while (((*cur >= 'a') && (*cur <= 'z')) ||
	   ((*cur >= 'A') && (*cur <= 'Z')) ||
	   ((*cur >= '0') && (*cur <= '9')) ||
	   (*cur == '_') || (*cur == '-') || (*cur == '.'))
	cur++;
    if (*cur == ':') {
	cur++;
	if (((*cur >= 'a') && (*cur <= 'z')) ||
	    ((*cur >= 'A') && (*cur <= 'Z')) ||
	    (*cur == '_'))
	    cur++;
	else
	    goto try_complex;
	while (((*cur >= 'a') && (*cur <= 'z')) ||
	       ((*cur >= 'A') && (*cur <= 'Z')) ||
	       ((*cur >= '0') && (*cur <= '9')) ||
	       (*cur == '_') || (*cur == '-') || (*cur == '.'))
	    cur++;
    }
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (*cur == 0)
	return(0);

try_complex:
    cur = value;
    c = CUR_SCHAR(cur, l);
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if ((!IS_LETTER(c)) && (c != '_'))
	return(1);
    cur += l;
    c = CUR_SCHAR(cur, l);
    while (IS_LETTER(c) || IS_DIGIT(c) || (c == '.') ||
	   (c == '-') || (c == '_') || IS_COMBINING(c) ||
	   IS_EXTENDER(c)) {
	cur += l;
	c = CUR_SCHAR(cur, l);
    }
    if (c == ':') {
	cur += l;
	c = CUR_SCHAR(cur, l);
	if ((!IS_LETTER(c)) && (c != '_'))
	    return(1);
	cur += l;
	c = CUR_SCHAR(cur, l);
	while (IS_LETTER(c) || IS_DIGIT(c) || (c == '.') ||
	       (c == '-') || (c == '_') || IS_COMBINING(c) ||
	       IS_EXTENDER(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if (c != 0)
	return(1);
    return(0);
}

int
xmlValidateName(const xmlChar *value, int space) {
    const xmlChar *cur = value;
    int c,l;

    if (value == NULL)
        return(-1);
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (((*cur >= 'a') && (*cur <= 'z')) || ((*cur >= 'A') && (*cur <= 'Z')) ||
	(*cur == '_') || (*cur == ':'))
	cur++;
    else
	goto try_complex;
    while (((*cur >= 'a') && (*cur <= 'z')) ||
	   ((*cur >= 'A') && (*cur <= 'Z')) ||
	   ((*cur >= '0') && (*cur <= '9')) ||
	   (*cur == '_') || (*cur == '-') || (*cur == '.') || (*cur == ':'))
	cur++;
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (*cur == 0)
	return(0);

try_complex:
    cur = value;
    c = CUR_SCHAR(cur, l);
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if ((!IS_LETTER(c)) && (c != '_') && (c != ':'))
	return(1);
    cur += l;
    c = CUR_SCHAR(cur, l);
    while (IS_LETTER(c) || IS_DIGIT(c) || (c == '.') || (c == ':') ||
	   (c == '-') || (c == '_') || IS_COMBINING(c) || IS_EXTENDER(c)) {
	cur += l;
	c = CUR_SCHAR(cur, l);
    }
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if (c != 0)
	return(1);
    return(0);
}

int
xmlValidateNMToken(const xmlChar *value, int space) {
    const xmlChar *cur = value;
    int c,l;

    if (value == NULL)
        return(-1);
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (((*cur >= 'a') && (*cur <= 'z')) ||
        ((*cur >= 'A') && (*cur <= 'Z')) ||
        ((*cur >= '0') && (*cur <= '9')) ||
        (*cur == '_') || (*cur == '-') || (*cur == '.') || (*cur == ':'))
	cur++;
    else
	goto try_complex;
    while (((*cur >= 'a') && (*cur <= 'z')) ||
	   ((*cur >= 'A') && (*cur <= 'Z')) ||
	   ((*cur >= '0') && (*cur <= '9')) ||
	   (*cur == '_') || (*cur == '-') || (*cur == '.') || (*cur == ':'))
	cur++;
    if (space)
	while (IS_BLANK_CH(*cur)) cur++;
    if (*cur == 0)
	return(0);

try_complex:
    cur = value;
    c = CUR_SCHAR(cur, l);
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if (!(IS_LETTER(c) || IS_DIGIT(c) || (c == '.') || (c == ':') ||
        (c == '-') || (c == '_') || IS_COMBINING(c) || IS_EXTENDER(c)))
	return(1);
    cur += l;
    c = CUR_SCHAR(cur, l);
    while (IS_LETTER(c) || IS_DIGIT(c) || (c == '.') || (c == ':') ||
	   (c == '-') || (c == '_') || IS_COMBINING(c) || IS_EXTENDER(c)) {
	cur += l;
	c = CUR_SCHAR(cur, l);
    }
    if (space) {
	while (IS_BLANK(c)) {
	    cur += l;
	    c = CUR_SCHAR(cur, l);
	}
    }
    if (c != 0)
	return(1);
    return(0);
}
#endif 


void
xmlSetBufferAllocationScheme(xmlBufferAllocationScheme scheme) {
    if ((scheme == XML_BUFFER_ALLOC_EXACT) ||
        (scheme == XML_BUFFER_ALLOC_DOUBLEIT))
	xmlBufferAllocScheme = scheme;
}

xmlBufferAllocationScheme
xmlGetBufferAllocationScheme(void) {
    return(xmlBufferAllocScheme);
}

xmlNsPtr
xmlNewNs(xmlNodePtr node, const xmlChar *href, const xmlChar *prefix) {
    xmlNsPtr cur;

    if ((node != NULL) && (node->type != XML_ELEMENT_NODE))
	return(NULL);

    if ((prefix != NULL) && (xmlStrEqual(prefix, BAD_CAST "xml")))
	return(NULL);

    cur = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
    if (cur == NULL) {
	xmlTreeErrMemory("building namespace");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNs));
    cur->type = XML_LOCAL_NAMESPACE;

    if (href != NULL)
	cur->href = xmlStrdup(href);
    if (prefix != NULL)
	cur->prefix = xmlStrdup(prefix);

    if (node != NULL) {
	if (node->nsDef == NULL) {
	    node->nsDef = cur;
	} else {
	    xmlNsPtr prev = node->nsDef;

	    if (((prev->prefix == NULL) && (cur->prefix == NULL)) ||
		(xmlStrEqual(prev->prefix, cur->prefix))) {
		xmlFreeNs(cur);
		return(NULL);
	    }
	    while (prev->next != NULL) {
	        prev = prev->next;
		if (((prev->prefix == NULL) && (cur->prefix == NULL)) ||
		    (xmlStrEqual(prev->prefix, cur->prefix))) {
		    xmlFreeNs(cur);
		    return(NULL);
		}
	    }
	    prev->next = cur;
	}
    }
    return(cur);
}

void
xmlSetNs(xmlNodePtr node, xmlNsPtr ns) {
    if (node == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlSetNs: node == NULL\n");
#endif
	return;
    }
    node->ns = ns;
}

void
xmlFreeNs(xmlNsPtr cur) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlFreeNs : ns == NULL\n");
#endif
	return;
    }
    if (cur->href != NULL) xmlFree((char *) cur->href);
    if (cur->prefix != NULL) xmlFree((char *) cur->prefix);
    xmlFree(cur);
}

void
xmlFreeNsList(xmlNsPtr cur) {
    xmlNsPtr next;
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlFreeNsList : ns == NULL\n");
#endif
	return;
    }
    while (cur != NULL) {
        next = cur->next;
        xmlFreeNs(cur);
	cur = next;
    }
}

xmlDtdPtr
xmlNewDtd(xmlDocPtr doc, const xmlChar *name,
                    const xmlChar *ExternalID, const xmlChar *SystemID) {
    xmlDtdPtr cur;

    if ((doc != NULL) && (doc->extSubset != NULL)) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewDtd(%s): document %s already have a DTD %s\n",
	     (char *) name, doc->name,
	     (char *)doc->extSubset->name);
#endif
	return(NULL);
    }

    cur = (xmlDtdPtr) xmlMalloc(sizeof(xmlDtd));
    if (cur == NULL) {
	xmlTreeErrMemory("building DTD");
	return(NULL);
    }
    memset(cur, 0 , sizeof(xmlDtd));
    cur->type = XML_DTD_NODE;

    if (name != NULL)
	cur->name = xmlStrdup(name);
    if (ExternalID != NULL)
	cur->ExternalID = xmlStrdup(ExternalID);
    if (SystemID != NULL)
	cur->SystemID = xmlStrdup(SystemID);
    if (doc != NULL)
	doc->extSubset = cur;
    cur->doc = doc;

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue((xmlNodePtr)cur);
    return(cur);
}


xmlDtdPtr
xmlGetIntSubset(xmlDocPtr doc) {
    xmlNodePtr cur;

    if (doc == NULL)
	return(NULL);
    cur = doc->children;
    while (cur != NULL) {
	if (cur->type == XML_DTD_NODE)
	    return((xmlDtdPtr) cur);
	cur = cur->next;
    }
    return((xmlDtdPtr) doc->intSubset);
}

xmlDtdPtr
xmlCreateIntSubset(xmlDocPtr doc, const xmlChar *name,
                   const xmlChar *ExternalID, const xmlChar *SystemID) {
    xmlDtdPtr cur;

    if ((doc != NULL) && (xmlGetIntSubset(doc) != NULL)) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,

     "xmlCreateIntSubset(): document %s already have an internal subset\n",
	    doc->name);
#endif
	return(NULL);
    }

    cur = (xmlDtdPtr) xmlMalloc(sizeof(xmlDtd));
    if (cur == NULL) {
	xmlTreeErrMemory("building internal subset");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlDtd));
    cur->type = XML_DTD_NODE;

    if (name != NULL) {
	cur->name = xmlStrdup(name);
	if (cur->name == NULL) {
	    xmlTreeErrMemory("building internal subset");
	    xmlFree(cur);
	    return(NULL);
	}
    }
    if (ExternalID != NULL) {
	cur->ExternalID = xmlStrdup(ExternalID);
	if (cur->ExternalID  == NULL) {
	    xmlTreeErrMemory("building internal subset");
	    if (cur->name != NULL)
	        xmlFree((char *)cur->name);
	    xmlFree(cur);
	    return(NULL);
	}
    }
    if (SystemID != NULL) {
	cur->SystemID = xmlStrdup(SystemID);
	if (cur->SystemID == NULL) {
	    xmlTreeErrMemory("building internal subset");
	    if (cur->name != NULL)
	        xmlFree((char *)cur->name);
	    if (cur->ExternalID != NULL)
	        xmlFree((char *)cur->ExternalID);
	    xmlFree(cur);
	    return(NULL);
	}
    }
    if (doc != NULL) {
	doc->intSubset = cur;
	cur->parent = doc;
	cur->doc = doc;
	if (doc->children == NULL) {
	    doc->children = (xmlNodePtr) cur;
	    doc->last = (xmlNodePtr) cur;
	} else {
	    if (doc->type == XML_HTML_DOCUMENT_NODE) {
		xmlNodePtr prev;

		prev = doc->children;
		prev->prev = (xmlNodePtr) cur;
		cur->next = prev;
		doc->children = (xmlNodePtr) cur;
	    } else {
		xmlNodePtr next;

		next = doc->children;
		while ((next != NULL) && (next->type != XML_ELEMENT_NODE))
		    next = next->next;
		if (next == NULL) {
		    cur->prev = doc->last;
		    cur->prev->next = (xmlNodePtr) cur;
		    cur->next = NULL;
		    doc->last = (xmlNodePtr) cur;
		} else {
		    cur->next = next;
		    cur->prev = next->prev;
		    if (cur->prev == NULL)
			doc->children = (xmlNodePtr) cur;
		    else
			cur->prev->next = (xmlNodePtr) cur;
		    next->prev = (xmlNodePtr) cur;
		}
	    }
	}
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue((xmlNodePtr)cur);
    return(cur);
}

#define DICT_FREE(str)						\
	if ((str) && ((!dict) ||				\
	    (xmlDictOwns(dict, (const xmlChar *)(str)) == 0)))	\
	    xmlFree((char *)(str));


#define DICT_COPY(str, cpy) \
    if (str) { \
	if (dict) { \
	    if (xmlDictOwns(dict, (const xmlChar *)(str))) \
		cpy = (xmlChar *) (str); \
	    else \
		cpy = (xmlChar *) xmlDictLookup((dict), (const xmlChar *)(str), -1); \
	} else \
	    cpy = xmlStrdup((const xmlChar *)(str)); }

#define DICT_CONST_COPY(str, cpy) \
    if (str) { \
	if (dict) { \
	    if (xmlDictOwns(dict, (const xmlChar *)(str))) \
		cpy = (const xmlChar *) (str); \
	    else \
		cpy = xmlDictLookup((dict), (const xmlChar *)(str), -1); \
	} else \
	    cpy = (const xmlChar *) xmlStrdup((const xmlChar *)(str)); }


void
xmlFreeDtd(xmlDtdPtr cur) {
    xmlDictPtr dict = NULL;

    if (cur == NULL) {
	return;
    }
    if (cur->doc != NULL) dict = cur->doc->dict;

    if ((__xmlRegisterCallbacks) && (xmlDeregisterNodeDefaultValue))
	xmlDeregisterNodeDefaultValue((xmlNodePtr)cur);

    if (cur->children != NULL) {
	xmlNodePtr next, c = cur->children;

        while (c != NULL) {
	    next = c->next;
	    if ((c->type != XML_NOTATION_NODE) &&
	        (c->type != XML_ELEMENT_DECL) &&
		(c->type != XML_ATTRIBUTE_DECL) &&
		(c->type != XML_ENTITY_DECL)) {
		xmlUnlinkNode(c);
		xmlFreeNode(c);
	    }
	    c = next;
	}
    }
    DICT_FREE(cur->name)
    DICT_FREE(cur->SystemID)
    DICT_FREE(cur->ExternalID)
    
    if (cur->notations != NULL)
        xmlFreeNotationTable((xmlNotationTablePtr) cur->notations);

    if (cur->elements != NULL)
        xmlFreeElementTable((xmlElementTablePtr) cur->elements);
    if (cur->attributes != NULL)
        xmlFreeAttributeTable((xmlAttributeTablePtr) cur->attributes);
    if (cur->entities != NULL)
        xmlFreeEntitiesTable((xmlEntitiesTablePtr) cur->entities);
    if (cur->pentities != NULL)
        xmlFreeEntitiesTable((xmlEntitiesTablePtr) cur->pentities);

    xmlFree(cur);
}

xmlDocPtr
xmlNewDoc(const xmlChar *version) {
    xmlDocPtr cur;

    if (version == NULL)
	version = (const xmlChar *) "1.0";

    cur = (xmlDocPtr) xmlMalloc(sizeof(xmlDoc));
    if (cur == NULL) {
	xmlTreeErrMemory("building doc");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlDoc));
    cur->type = XML_DOCUMENT_NODE;

    cur->version = xmlStrdup(version);
    if (cur->version == NULL) {
	xmlTreeErrMemory("building doc");
	xmlFree(cur);
	return(NULL);
    }
    cur->standalone = -1;
    cur->compression = -1; 
    cur->doc = cur;
    cur->parseFlags = 0;
    cur->properties = XML_DOC_USERBUILT;
    cur->charset = XML_CHAR_ENCODING_UTF8;

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue((xmlNodePtr)cur);
    return(cur);
}

void
xmlFreeDoc(xmlDocPtr cur) {
    xmlDtdPtr extSubset, intSubset;
    xmlDictPtr dict = NULL;

    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlFreeDoc : document == NULL\n");
#endif
	return;
    }
#ifdef LIBXML_DEBUG_RUNTIME
#ifdef LIBXML_DEBUG_ENABLED
    xmlDebugCheckDocument(stderr, cur);
#endif
#endif

    if (cur != NULL) dict = cur->dict;

    if ((__xmlRegisterCallbacks) && (xmlDeregisterNodeDefaultValue))
	xmlDeregisterNodeDefaultValue((xmlNodePtr)cur);

    if (cur->ids != NULL) xmlFreeIDTable((xmlIDTablePtr) cur->ids);
    cur->ids = NULL;
    if (cur->refs != NULL) xmlFreeRefTable((xmlRefTablePtr) cur->refs);
    cur->refs = NULL;
    extSubset = cur->extSubset;
    intSubset = cur->intSubset;
    if (intSubset == extSubset)
	extSubset = NULL;
    if (extSubset != NULL) {
	xmlUnlinkNode((xmlNodePtr) cur->extSubset);
	cur->extSubset = NULL;
	xmlFreeDtd(extSubset);
    }
    if (intSubset != NULL) {
	xmlUnlinkNode((xmlNodePtr) cur->intSubset);
	cur->intSubset = NULL;
	xmlFreeDtd(intSubset);
    }

    if (cur->children != NULL) xmlFreeNodeList(cur->children);
    if (cur->oldNs != NULL) xmlFreeNsList(cur->oldNs);

    DICT_FREE(cur->version)
    DICT_FREE(cur->name)
    DICT_FREE(cur->encoding)
    DICT_FREE(cur->URL)
    xmlFree(cur);
    if (dict) xmlDictFree(dict);
}

xmlNodePtr
xmlStringLenGetNodeList(xmlDocPtr doc, const xmlChar *value, int len) {
    xmlNodePtr ret = NULL, last = NULL;
    xmlNodePtr node;
    xmlChar *val;
    const xmlChar *cur = value, *end = cur + len;
    const xmlChar *q;
    xmlEntityPtr ent;

    if (value == NULL) return(NULL);

    q = cur;
    while ((cur < end) && (*cur != 0)) {
	if (cur[0] == '&') {
	    int charval = 0;
	    xmlChar tmp;

            if (cur != q) {
		if ((last != NULL) && (last->type == XML_TEXT_NODE)) {
		    xmlNodeAddContentLen(last, q, cur - q);
		} else {
		    node = xmlNewDocTextLen(doc, q, cur - q);
		    if (node == NULL) return(ret);
		    if (last == NULL)
			last = ret = node;
		    else {
			last->next = node;
			node->prev = last;
			last = node;
		    }
		}
	    }
	    q = cur;
	    if ((cur + 2 < end) && (cur[1] == '#') && (cur[2] == 'x')) {
		cur += 3;
		if (cur < end)
		    tmp = *cur;
		else
		    tmp = 0;
		while (tmp != ';') { 
		    if ((tmp >= '0') && (tmp <= '9'))
			charval = charval * 16 + (tmp - '0');
		    else if ((tmp >= 'a') && (tmp <= 'f'))
			charval = charval * 16 + (tmp - 'a') + 10;
		    else if ((tmp >= 'A') && (tmp <= 'F'))
			charval = charval * 16 + (tmp - 'A') + 10;
		    else {
			xmlTreeErr(XML_TREE_INVALID_HEX, (xmlNodePtr) doc,
			           NULL);
			charval = 0;
			break;
		    }
		    cur++;
		    if (cur < end)
			tmp = *cur;
		    else
			tmp = 0;
		}
		if (tmp == ';')
		    cur++;
		q = cur;
	    } else if ((cur + 1 < end) && (cur[1] == '#')) {
		cur += 2;
		if (cur < end)
		    tmp = *cur;
		else
		    tmp = 0;
		while (tmp != ';') { 
		    if ((tmp >= '0') && (tmp <= '9'))
			charval = charval * 10 + (tmp - '0');
		    else {
			xmlTreeErr(XML_TREE_INVALID_DEC, (xmlNodePtr) doc,
			           NULL);
			charval = 0;
			break;
		    }
		    cur++;
		    if (cur < end)
			tmp = *cur;
		    else
			tmp = 0;
		}
		if (tmp == ';')
		    cur++;
		q = cur;
	    } else {
		cur++;
		q = cur;
		while ((cur < end) && (*cur != 0) && (*cur != ';')) cur++;
		if ((cur >= end) || (*cur == 0)) {
		    xmlTreeErr(XML_TREE_UNTERMINATED_ENTITY, (xmlNodePtr) doc,
		               (const char *) q);
		    return(ret);
		}
		if (cur != q) {
		    val = xmlStrndup(q, cur - q);
		    ent = xmlGetDocEntity(doc, val);
		    if ((ent != NULL) &&
			(ent->etype == XML_INTERNAL_PREDEFINED_ENTITY)) {
			if (last == NULL) {
			    node = xmlNewDocText(doc, ent->content);
			    last = ret = node;
			} else if (last->type != XML_TEXT_NODE) {
			    node = xmlNewDocText(doc, ent->content);
			    last = xmlAddNextSibling(last, node);
			} else
			    xmlNodeAddContent(last, ent->content);

		    } else {
			node = xmlNewReference(doc, val);
			if (node == NULL) {
			    if (val != NULL) xmlFree(val);
			    return(ret);
			}
			else if ((ent != NULL) && (ent->children == NULL)) {
			    xmlNodePtr temp;

			    ent->children = xmlStringGetNodeList(doc,
				    (const xmlChar*)node->content);
			    ent->owner = 1;
			    temp = ent->children;
			    while (temp) {
				temp->parent = (xmlNodePtr)ent;
				ent->last = temp;
				temp = temp->next;
			    }
			}
			if (last == NULL) {
			    last = ret = node;
			} else {
			    last = xmlAddNextSibling(last, node);
			}
		    }
		    xmlFree(val);
		}
		cur++;
		q = cur;
	    }
	    if (charval != 0) {
		xmlChar buf[10];
		int l;

		l = xmlCopyCharMultiByte(buf, charval);
		buf[l] = 0;
		node = xmlNewDocText(doc, buf);
		if (node != NULL) {
		    if (last == NULL) {
			last = ret = node;
		    } else {
			last = xmlAddNextSibling(last, node);
		    }
		}
		charval = 0;
	    }
	} else
	    cur++;
    }
    if ((cur != q) || (ret == NULL)) {
	if ((last != NULL) && (last->type == XML_TEXT_NODE)) {
	    xmlNodeAddContentLen(last, q, cur - q);
	} else {
	    node = xmlNewDocTextLen(doc, q, cur - q);
	    if (node == NULL) return(ret);
	    if (last == NULL) {
		ret = node;
	    } else {
		xmlAddNextSibling(last, node);
	    }
	}
    }
    return(ret);
}

xmlNodePtr
xmlStringGetNodeList(xmlDocPtr doc, const xmlChar *value) {
    xmlNodePtr ret = NULL, last = NULL;
    xmlNodePtr node;
    xmlChar *val;
    const xmlChar *cur = value;
    const xmlChar *q;
    xmlEntityPtr ent;

    if (value == NULL) return(NULL);

    q = cur;
    while (*cur != 0) {
	if (cur[0] == '&') {
	    int charval = 0;
	    xmlChar tmp;

            if (cur != q) {
		if ((last != NULL) && (last->type == XML_TEXT_NODE)) {
		    xmlNodeAddContentLen(last, q, cur - q);
		} else {
		    node = xmlNewDocTextLen(doc, q, cur - q);
		    if (node == NULL) return(ret);
		    if (last == NULL)
			last = ret = node;
		    else {
			last->next = node;
			node->prev = last;
			last = node;
		    }
		}
	    }
	    q = cur;
	    if ((cur[1] == '#') && (cur[2] == 'x')) {
		cur += 3;
		tmp = *cur;
		while (tmp != ';') { 
		    if ((tmp >= '0') && (tmp <= '9'))
			charval = charval * 16 + (tmp - '0');
		    else if ((tmp >= 'a') && (tmp <= 'f'))
			charval = charval * 16 + (tmp - 'a') + 10;
		    else if ((tmp >= 'A') && (tmp <= 'F'))
			charval = charval * 16 + (tmp - 'A') + 10;
		    else {
			xmlTreeErr(XML_TREE_INVALID_HEX, (xmlNodePtr) doc,
			           NULL);
			charval = 0;
			break;
		    }
		    cur++;
		    tmp = *cur;
		}
		if (tmp == ';')
		    cur++;
		q = cur;
	    } else if  (cur[1] == '#') {
		cur += 2;
		tmp = *cur;
		while (tmp != ';') { 
		    if ((tmp >= '0') && (tmp <= '9'))
			charval = charval * 10 + (tmp - '0');
		    else {
			xmlTreeErr(XML_TREE_INVALID_DEC, (xmlNodePtr) doc,
			           NULL);
			charval = 0;
			break;
		    }
		    cur++;
		    tmp = *cur;
		}
		if (tmp == ';')
		    cur++;
		q = cur;
	    } else {
		cur++;
		q = cur;
		while ((*cur != 0) && (*cur != ';')) cur++;
		if (*cur == 0) {
		    xmlTreeErr(XML_TREE_UNTERMINATED_ENTITY,
		               (xmlNodePtr) doc, (const char *) q);
		    return(ret);
		}
		if (cur != q) {
		    val = xmlStrndup(q, cur - q);
		    ent = xmlGetDocEntity(doc, val);
		    if ((ent != NULL) &&
			(ent->etype == XML_INTERNAL_PREDEFINED_ENTITY)) {
			if (last == NULL) {
			    node = xmlNewDocText(doc, ent->content);
			    last = ret = node;
			} else if (last->type != XML_TEXT_NODE) {
			    node = xmlNewDocText(doc, ent->content);
			    last = xmlAddNextSibling(last, node);
			} else
			    xmlNodeAddContent(last, ent->content);

		    } else {
			node = xmlNewReference(doc, val);
			if (node == NULL) {
			    if (val != NULL) xmlFree(val);
			    return(ret);
			}
			else if ((ent != NULL) && (ent->children == NULL)) {
			    xmlNodePtr temp;

			    ent->children = xmlStringGetNodeList(doc,
				    (const xmlChar*)node->content);
			    ent->owner = 1;
			    temp = ent->children;
			    while (temp) {
				temp->parent = (xmlNodePtr)ent;
				temp = temp->next;
			    }
			}
			if (last == NULL) {
			    last = ret = node;
			} else {
			    last = xmlAddNextSibling(last, node);
			}
		    }
		    xmlFree(val);
		}
		cur++;
		q = cur;
	    }
	    if (charval != 0) {
		xmlChar buf[10];
		int len;

		len = xmlCopyCharMultiByte(buf, charval);
		buf[len] = 0;
		node = xmlNewDocText(doc, buf);
		if (node != NULL) {
		    if (last == NULL) {
			last = ret = node;
		    } else {
			last = xmlAddNextSibling(last, node);
		    }
		}
	    }
	} else
	    cur++;
    }
    if ((cur != q) || (ret == NULL)) {
	if ((last != NULL) && (last->type == XML_TEXT_NODE)) {
	    xmlNodeAddContentLen(last, q, cur - q);
	} else {
	    node = xmlNewDocTextLen(doc, q, cur - q);
	    if (node == NULL) return(ret);
	    if (last == NULL) {
		last = ret = node;
	    } else {
		last = xmlAddNextSibling(last, node);
	    }
	}
    }
    return(ret);
}

xmlChar *
xmlNodeListGetString(xmlDocPtr doc, xmlNodePtr list, int inLine)
{
    xmlNodePtr node = list;
    xmlChar *ret = NULL;
    xmlEntityPtr ent;

    if (list == NULL)
        return (NULL);

    while (node != NULL) {
        if ((node->type == XML_TEXT_NODE) ||
            (node->type == XML_CDATA_SECTION_NODE)) {
            if (inLine) {
                ret = xmlStrcat(ret, node->content);
            } else {
                xmlChar *buffer;

                buffer = xmlEncodeEntitiesReentrant(doc, node->content);
                if (buffer != NULL) {
                    ret = xmlStrcat(ret, buffer);
                    xmlFree(buffer);
                }
            }
        } else if (node->type == XML_ENTITY_REF_NODE) {
            if (inLine) {
                ent = xmlGetDocEntity(doc, node->name);
                if (ent != NULL) {
                    xmlChar *buffer;

                    buffer = xmlNodeListGetString(doc, ent->children, 1);
                    if (buffer != NULL) {
                        ret = xmlStrcat(ret, buffer);
                        xmlFree(buffer);
                    }
                } else {
                    ret = xmlStrcat(ret, node->content);
                }
            } else {
                xmlChar buf[2];

                buf[0] = '&';
                buf[1] = 0;
                ret = xmlStrncat(ret, buf, 1);
                ret = xmlStrcat(ret, node->name);
                buf[0] = ';';
                buf[1] = 0;
                ret = xmlStrncat(ret, buf, 1);
            }
        }
#if 0
        else {
            xmlGenericError(xmlGenericErrorContext,
                            "xmlGetNodeListString : invalid node type %d\n",
                            node->type);
        }
#endif
        node = node->next;
    }
    return (ret);
}

#ifdef LIBXML_TREE_ENABLED
xmlChar *
xmlNodeListGetRawString(xmlDocPtr doc, xmlNodePtr list, int inLine)
{
    xmlNodePtr node = list;
    xmlChar *ret = NULL;
    xmlEntityPtr ent;

    if (list == NULL)
        return (NULL);

    while (node != NULL) {
        if ((node->type == XML_TEXT_NODE) ||
            (node->type == XML_CDATA_SECTION_NODE)) {
            if (inLine) {
                ret = xmlStrcat(ret, node->content);
            } else {
                xmlChar *buffer;

                buffer = xmlEncodeSpecialChars(doc, node->content);
                if (buffer != NULL) {
                    ret = xmlStrcat(ret, buffer);
                    xmlFree(buffer);
                }
            }
        } else if (node->type == XML_ENTITY_REF_NODE) {
            if (inLine) {
                ent = xmlGetDocEntity(doc, node->name);
                if (ent != NULL) {
                    xmlChar *buffer;

                    buffer =
                        xmlNodeListGetRawString(doc, ent->children, 1);
                    if (buffer != NULL) {
                        ret = xmlStrcat(ret, buffer);
                        xmlFree(buffer);
                    }
                } else {
                    ret = xmlStrcat(ret, node->content);
                }
            } else {
                xmlChar buf[2];

                buf[0] = '&';
                buf[1] = 0;
                ret = xmlStrncat(ret, buf, 1);
                ret = xmlStrcat(ret, node->name);
                buf[0] = ';';
                buf[1] = 0;
                ret = xmlStrncat(ret, buf, 1);
            }
        }
#if 0
        else {
            xmlGenericError(xmlGenericErrorContext,
                            "xmlGetNodeListString : invalid node type %d\n",
                            node->type);
        }
#endif
        node = node->next;
    }
    return (ret);
}
#endif 

static xmlAttrPtr
xmlNewPropInternal(xmlNodePtr node, xmlNsPtr ns,
                   const xmlChar * name, const xmlChar * value,
                   int eatname)
{
    xmlAttrPtr cur;
    xmlDocPtr doc = NULL;

    if ((node != NULL) && (node->type != XML_ELEMENT_NODE)) {
        if ((eatname == 1) &&
	    ((node->doc == NULL) ||
	     (!(xmlDictOwns(node->doc->dict, name)))))
            xmlFree((xmlChar *) name);
        return (NULL);
    }

    cur = (xmlAttrPtr) xmlMalloc(sizeof(xmlAttr));
    if (cur == NULL) {
        if ((eatname == 1) &&
	    ((node == NULL) || (node->doc == NULL) ||
	     (!(xmlDictOwns(node->doc->dict, name)))))
            xmlFree((xmlChar *) name);
        xmlTreeErrMemory("building attribute");
        return (NULL);
    }
    memset(cur, 0, sizeof(xmlAttr));
    cur->type = XML_ATTRIBUTE_NODE;

    cur->parent = node;
    if (node != NULL) {
        doc = node->doc;
        cur->doc = doc;
    }
    cur->ns = ns;

    if (eatname == 0) {
        if ((doc != NULL) && (doc->dict != NULL))
            cur->name = (xmlChar *) xmlDictLookup(doc->dict, name, -1);
        else
            cur->name = xmlStrdup(name);
    } else
        cur->name = name;

    if (value != NULL) {
        xmlNodePtr tmp;

        if(!xmlCheckUTF8(value)) {
            xmlTreeErr(XML_TREE_NOT_UTF8, (xmlNodePtr) doc,
                       NULL);
            if (doc != NULL)
                doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
        }
        cur->children = xmlNewDocText(doc, value);
        cur->last = NULL;
        tmp = cur->children;
        while (tmp != NULL) {
            tmp->parent = (xmlNodePtr) cur;
            if (tmp->next == NULL)
                cur->last = tmp;
            tmp = tmp->next;
        }
    }

    if (node != NULL) {
        if (node->properties == NULL) {
            node->properties = cur;
        } else {
            xmlAttrPtr prev = node->properties;

            while (prev->next != NULL)
                prev = prev->next;
            prev->next = cur;
            cur->prev = prev;
        }
    }

    if ((value != NULL) && (node != NULL) &&
        (xmlIsID(node->doc, node, cur) == 1))
        xmlAddID(NULL, node->doc, value, cur);

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
        xmlRegisterNodeDefaultValue((xmlNodePtr) cur);
    return (cur);
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_HTML_ENABLED) || \
    defined(LIBXML_SCHEMAS_ENABLED)
xmlAttrPtr
xmlNewProp(xmlNodePtr node, const xmlChar *name, const xmlChar *value) {

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewProp : name == NULL\n");
#endif
	return(NULL);
    }

	return xmlNewPropInternal(node, NULL, name, value, 0);
}
#endif 

xmlAttrPtr
xmlNewNsProp(xmlNodePtr node, xmlNsPtr ns, const xmlChar *name,
           const xmlChar *value) {

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewNsProp : name == NULL\n");
#endif
	return(NULL);
    }

    return xmlNewPropInternal(node, ns, name, value, 0);
}

xmlAttrPtr
xmlNewNsPropEatName(xmlNodePtr node, xmlNsPtr ns, xmlChar *name,
           const xmlChar *value) {

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewNsPropEatName : name == NULL\n");
#endif
	return(NULL);
    }

    return xmlNewPropInternal(node, ns, name, value, 1);
}

xmlAttrPtr
xmlNewDocProp(xmlDocPtr doc, const xmlChar *name, const xmlChar *value) {
    xmlAttrPtr cur;

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewDocProp : name == NULL\n");
#endif
	return(NULL);
    }

    cur = (xmlAttrPtr) xmlMalloc(sizeof(xmlAttr));
    if (cur == NULL) {
	xmlTreeErrMemory("building attribute");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlAttr));
    cur->type = XML_ATTRIBUTE_NODE;

    if ((doc != NULL) && (doc->dict != NULL))
	cur->name = xmlDictLookup(doc->dict, name, -1);
    else
	cur->name = xmlStrdup(name);
    cur->doc = doc;
    if (value != NULL) {
	xmlNodePtr tmp;

	cur->children = xmlStringGetNodeList(doc, value);
	cur->last = NULL;

	tmp = cur->children;
	while (tmp != NULL) {
	    tmp->parent = (xmlNodePtr) cur;
	    if (tmp->next == NULL)
		cur->last = tmp;
	    tmp = tmp->next;
	}
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue((xmlNodePtr)cur);
    return(cur);
}

void
xmlFreePropList(xmlAttrPtr cur) {
    xmlAttrPtr next;
    if (cur == NULL) return;
    while (cur != NULL) {
        next = cur->next;
        xmlFreeProp(cur);
	cur = next;
    }
}

void
xmlFreeProp(xmlAttrPtr cur) {
    xmlDictPtr dict = NULL;
    if (cur == NULL) return;

    if (cur->doc != NULL) dict = cur->doc->dict;

    if ((__xmlRegisterCallbacks) && (xmlDeregisterNodeDefaultValue))
	xmlDeregisterNodeDefaultValue((xmlNodePtr)cur);

    
    if ((cur->doc != NULL) && (cur->atype == XML_ATTRIBUTE_ID)) {
	    xmlRemoveID(cur->doc, cur);
    }
    if (cur->children != NULL) xmlFreeNodeList(cur->children);
    DICT_FREE(cur->name)
    xmlFree(cur);
}

int
xmlRemoveProp(xmlAttrPtr cur) {
    xmlAttrPtr tmp;
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlRemoveProp : cur == NULL\n");
#endif
	return(-1);
    }
    if (cur->parent == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlRemoveProp : cur->parent == NULL\n");
#endif
	return(-1);
    }
    tmp = cur->parent->properties;
    if (tmp == cur) {
        cur->parent->properties = cur->next;
		if (cur->next != NULL)
			cur->next->prev = NULL;
	xmlFreeProp(cur);
	return(0);
    }
    while (tmp != NULL) {
	if (tmp->next == cur) {
	    tmp->next = cur->next;
	    if (tmp->next != NULL)
		tmp->next->prev = tmp;
	    xmlFreeProp(cur);
	    return(0);
	}
        tmp = tmp->next;
    }
#ifdef DEBUG_TREE
    xmlGenericError(xmlGenericErrorContext,
	    "xmlRemoveProp : attribute not owned by its node\n");
#endif
    return(-1);
}

xmlNodePtr
xmlNewDocPI(xmlDocPtr doc, const xmlChar *name, const xmlChar *content) {
    xmlNodePtr cur;

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewPI : name == NULL\n");
#endif
	return(NULL);
    }

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building PI");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_PI_NODE;

    if ((doc != NULL) && (doc->dict != NULL))
        cur->name = xmlDictLookup(doc->dict, name, -1);
    else
	cur->name = xmlStrdup(name);
    if (content != NULL) {
	cur->content = xmlStrdup(content);
    }
    cur->doc = doc;

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue((xmlNodePtr)cur);
    return(cur);
}

xmlNodePtr
xmlNewPI(const xmlChar *name, const xmlChar *content) {
    return(xmlNewDocPI(NULL, name, content));
}

xmlNodePtr
xmlNewNode(xmlNsPtr ns, const xmlChar *name) {
    xmlNodePtr cur;

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewNode : name == NULL\n");
#endif
	return(NULL);
    }

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building node");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_ELEMENT_NODE;

    cur->name = xmlStrdup(name);
    cur->ns = ns;

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

xmlNodePtr
xmlNewNodeEatName(xmlNsPtr ns, xmlChar *name) {
    xmlNodePtr cur;

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewNode : name == NULL\n");
#endif
	return(NULL);
    }

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building node");
	
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_ELEMENT_NODE;

    cur->name = name;
    cur->ns = ns;

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue((xmlNodePtr)cur);
    return(cur);
}

xmlNodePtr
xmlNewDocNode(xmlDocPtr doc, xmlNsPtr ns,
              const xmlChar *name, const xmlChar *content) {
    xmlNodePtr cur;

    if ((doc != NULL) && (doc->dict != NULL))
        cur = xmlNewNodeEatName(ns, (xmlChar *)
	                        xmlDictLookup(doc->dict, name, -1));
    else
	cur = xmlNewNode(ns, name);
    if (cur != NULL) {
        cur->doc = doc;
	if (content != NULL) {
	    cur->children = xmlStringGetNodeList(doc, content);
	    UPDATE_LAST_CHILD_AND_PARENT(cur)
	}
    }

    return(cur);
}

xmlNodePtr
xmlNewDocNodeEatName(xmlDocPtr doc, xmlNsPtr ns,
              xmlChar *name, const xmlChar *content) {
    xmlNodePtr cur;

    cur = xmlNewNodeEatName(ns, name);
    if (cur != NULL) {
        cur->doc = doc;
	if (content != NULL) {
	    cur->children = xmlStringGetNodeList(doc, content);
	    UPDATE_LAST_CHILD_AND_PARENT(cur)
	}
    } else {
        
        if ((name != NULL) && (doc != NULL) &&
	    (!(xmlDictOwns(doc->dict, name))))
	    xmlFree(name);
    }
    return(cur);
}

#ifdef LIBXML_TREE_ENABLED
xmlNodePtr
xmlNewDocRawNode(xmlDocPtr doc, xmlNsPtr ns,
                 const xmlChar *name, const xmlChar *content) {
    xmlNodePtr cur;

    cur = xmlNewDocNode(doc, ns, name, NULL);
    if (cur != NULL) {
        cur->doc = doc;
	if (content != NULL) {
	    cur->children = xmlNewDocText(doc, content);
	    UPDATE_LAST_CHILD_AND_PARENT(cur)
	}
    }
    return(cur);
}

xmlNodePtr
xmlNewDocFragment(xmlDocPtr doc) {
    xmlNodePtr cur;

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building fragment");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_DOCUMENT_FRAG_NODE;

    cur->doc = doc;

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}
#endif 

xmlNodePtr
xmlNewText(const xmlChar *content) {
    xmlNodePtr cur;

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building text");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_TEXT_NODE;

    cur->name = xmlStringText;
    if (content != NULL) {
	cur->content = xmlStrdup(content);
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

#ifdef LIBXML_TREE_ENABLED
xmlNodePtr
xmlNewTextChild(xmlNodePtr parent, xmlNsPtr ns,
            const xmlChar *name, const xmlChar *content) {
    xmlNodePtr cur, prev;

    if (parent == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewTextChild : parent == NULL\n");
#endif
	return(NULL);
    }

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewTextChild : name == NULL\n");
#endif
	return(NULL);
    }

    if (parent->type == XML_ELEMENT_NODE) {
	if (ns == NULL)
	    cur = xmlNewDocRawNode(parent->doc, parent->ns, name, content);
	else
	    cur = xmlNewDocRawNode(parent->doc, ns, name, content);
    } else if ((parent->type == XML_DOCUMENT_NODE) ||
	       (parent->type == XML_HTML_DOCUMENT_NODE)) {
	if (ns == NULL)
	    cur = xmlNewDocRawNode((xmlDocPtr) parent, NULL, name, content);
	else
	    cur = xmlNewDocRawNode((xmlDocPtr) parent, ns, name, content);
    } else if (parent->type == XML_DOCUMENT_FRAG_NODE) {
	    cur = xmlNewDocRawNode( parent->doc, ns, name, content);
    } else {
	return(NULL);
    }
    if (cur == NULL) return(NULL);

    cur->type = XML_ELEMENT_NODE;
    cur->parent = parent;
    cur->doc = parent->doc;
    if (parent->children == NULL) {
        parent->children = cur;
	parent->last = cur;
    } else {
        prev = parent->last;
	prev->next = cur;
	cur->prev = prev;
	parent->last = cur;
    }

    return(cur);
}
#endif 

xmlNodePtr
xmlNewCharRef(xmlDocPtr doc, const xmlChar *name) {
    xmlNodePtr cur;

    if (name == NULL)
        return(NULL);

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building character reference");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_ENTITY_REF_NODE;

    cur->doc = doc;
    if (name[0] == '&') {
        int len;
        name++;
	len = xmlStrlen(name);
	if (name[len - 1] == ';')
	    cur->name = xmlStrndup(name, len - 1);
	else
	    cur->name = xmlStrndup(name, len);
    } else
	cur->name = xmlStrdup(name);

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

xmlNodePtr
xmlNewReference(xmlDocPtr doc, const xmlChar *name) {
    xmlNodePtr cur;
    xmlEntityPtr ent;

    if (name == NULL)
        return(NULL);

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building reference");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_ENTITY_REF_NODE;

    cur->doc = doc;
    if (name[0] == '&') {
        int len;
        name++;
	len = xmlStrlen(name);
	if (name[len - 1] == ';')
	    cur->name = xmlStrndup(name, len - 1);
	else
	    cur->name = xmlStrndup(name, len);
    } else
	cur->name = xmlStrdup(name);

    ent = xmlGetDocEntity(doc, cur->name);
    if (ent != NULL) {
	cur->content = ent->content;
	cur->children = (xmlNodePtr) ent;
	cur->last = (xmlNodePtr) ent;
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

xmlNodePtr
xmlNewDocText(xmlDocPtr doc, const xmlChar *content) {
    xmlNodePtr cur;

    cur = xmlNewText(content);
    if (cur != NULL) cur->doc = doc;
    return(cur);
}

xmlNodePtr
xmlNewTextLen(const xmlChar *content, int len) {
    xmlNodePtr cur;

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building text");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_TEXT_NODE;

    cur->name = xmlStringText;
    if (content != NULL) {
	cur->content = xmlStrndup(content, len);
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

xmlNodePtr
xmlNewDocTextLen(xmlDocPtr doc, const xmlChar *content, int len) {
    xmlNodePtr cur;

    cur = xmlNewTextLen(content, len);
    if (cur != NULL) cur->doc = doc;
    return(cur);
}

xmlNodePtr
xmlNewComment(const xmlChar *content) {
    xmlNodePtr cur;

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building comment");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_COMMENT_NODE;

    cur->name = xmlStringComment;
    if (content != NULL) {
	cur->content = xmlStrdup(content);
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

xmlNodePtr
xmlNewCDataBlock(xmlDocPtr doc, const xmlChar *content, int len) {
    xmlNodePtr cur;

    cur = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (cur == NULL) {
	xmlTreeErrMemory("building CDATA");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlNode));
    cur->type = XML_CDATA_SECTION_NODE;
    cur->doc = doc;

    if (content != NULL) {
	cur->content = xmlStrndup(content, len);
    }

    if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	xmlRegisterNodeDefaultValue(cur);
    return(cur);
}

xmlNodePtr
xmlNewDocComment(xmlDocPtr doc, const xmlChar *content) {
    xmlNodePtr cur;

    cur = xmlNewComment(content);
    if (cur != NULL) cur->doc = doc;
    return(cur);
}

void
xmlSetTreeDoc(xmlNodePtr tree, xmlDocPtr doc) {
    xmlAttrPtr prop;

    if (tree == NULL)
	return;
    if (tree->doc != doc) {
	if(tree->type == XML_ELEMENT_NODE) {
	    prop = tree->properties;
	    while (prop != NULL) {
		prop->doc = doc;
		xmlSetListDoc(prop->children, doc);
		prop = prop->next;
	    }
	}
	if (tree->children != NULL)
	    xmlSetListDoc(tree->children, doc);
	tree->doc = doc;
    }
}

void
xmlSetListDoc(xmlNodePtr list, xmlDocPtr doc) {
    xmlNodePtr cur;

    if (list == NULL)
	return;
    cur = list;
    while (cur != NULL) {
	if (cur->doc != doc)
	    xmlSetTreeDoc(cur, doc);
	cur = cur->next;
    }
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
xmlNodePtr
xmlNewChild(xmlNodePtr parent, xmlNsPtr ns,
            const xmlChar *name, const xmlChar *content) {
    xmlNodePtr cur, prev;

    if (parent == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewChild : parent == NULL\n");
#endif
	return(NULL);
    }

    if (name == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewChild : name == NULL\n");
#endif
	return(NULL);
    }

    if (parent->type == XML_ELEMENT_NODE) {
	if (ns == NULL)
	    cur = xmlNewDocNode(parent->doc, parent->ns, name, content);
	else
	    cur = xmlNewDocNode(parent->doc, ns, name, content);
    } else if ((parent->type == XML_DOCUMENT_NODE) ||
	       (parent->type == XML_HTML_DOCUMENT_NODE)) {
	if (ns == NULL)
	    cur = xmlNewDocNode((xmlDocPtr) parent, NULL, name, content);
	else
	    cur = xmlNewDocNode((xmlDocPtr) parent, ns, name, content);
    } else if (parent->type == XML_DOCUMENT_FRAG_NODE) {
	    cur = xmlNewDocNode( parent->doc, ns, name, content);
    } else {
	return(NULL);
    }
    if (cur == NULL) return(NULL);

    cur->type = XML_ELEMENT_NODE;
    cur->parent = parent;
    cur->doc = parent->doc;
    if (parent->children == NULL) {
        parent->children = cur;
	parent->last = cur;
    } else {
        prev = parent->last;
	prev->next = cur;
	cur->prev = prev;
	parent->last = cur;
    }

    return(cur);
}
#endif 

static xmlNodePtr
xmlAddPropSibling(xmlNodePtr prev, xmlNodePtr cur, xmlNodePtr prop) {
	xmlAttrPtr attr;

	if (cur->type != XML_ATTRIBUTE_NODE)
		return(NULL);

	
	if (prop->ns == NULL)
		attr = xmlHasNsProp(cur->parent, prop->name, NULL);
	else
		attr = xmlHasNsProp(cur->parent, prop->name, prop->ns->href);

	if (prop->doc != cur->doc) {
		xmlSetTreeDoc(prop, cur->doc);
	}
	prop->parent = cur->parent;
	prop->prev = prev;
	if (prev != NULL) {
		prop->next = prev->next;
		prev->next = prop;
		if (prop->next)
			prop->next->prev = prop;
	} else {
		prop->next = cur;
		cur->prev = prop;
	}
	if (prop->prev == NULL && prop->parent != NULL)
		prop->parent->properties = (xmlAttrPtr) prop;
	if ((attr != NULL) && (attr->type != XML_ATTRIBUTE_DECL)) {
		
		xmlRemoveProp((xmlAttrPtr) attr);
	}
	return prop;
}

xmlNodePtr
xmlAddNextSibling(xmlNodePtr cur, xmlNodePtr elem) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddNextSibling : cur == NULL\n");
#endif
	return(NULL);
    }
    if (elem == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddNextSibling : elem == NULL\n");
#endif
	return(NULL);
    }

    if (cur == elem) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddNextSibling : cur == elem\n");
#endif
	return(NULL);
    }

    xmlUnlinkNode(elem);

    if (elem->type == XML_TEXT_NODE) {
	if (cur->type == XML_TEXT_NODE) {
	    xmlNodeAddContent(cur, elem->content);
	    xmlFreeNode(elem);
	    return(cur);
	}
	if ((cur->next != NULL) && (cur->next->type == XML_TEXT_NODE) &&
            (cur->name == cur->next->name)) {
	    xmlChar *tmp;

	    tmp = xmlStrdup(elem->content);
	    tmp = xmlStrcat(tmp, cur->next->content);
	    xmlNodeSetContent(cur->next, tmp);
	    xmlFree(tmp);
	    xmlFreeNode(elem);
	    return(cur->next);
	}
    } else if (elem->type == XML_ATTRIBUTE_NODE) {
		return xmlAddPropSibling(cur, cur, elem);
    }

    if (elem->doc != cur->doc) {
	xmlSetTreeDoc(elem, cur->doc);
    }
    elem->parent = cur->parent;
    elem->prev = cur;
    elem->next = cur->next;
    cur->next = elem;
    if (elem->next != NULL)
	elem->next->prev = elem;
    if ((elem->parent != NULL) && (elem->parent->last == cur))
	elem->parent->last = elem;
    return(elem);
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_HTML_ENABLED) || \
    defined(LIBXML_SCHEMAS_ENABLED)
xmlNodePtr
xmlAddPrevSibling(xmlNodePtr cur, xmlNodePtr elem) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddPrevSibling : cur == NULL\n");
#endif
	return(NULL);
    }
    if (elem == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddPrevSibling : elem == NULL\n");
#endif
	return(NULL);
    }

    if (cur == elem) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddPrevSibling : cur == elem\n");
#endif
	return(NULL);
    }

    xmlUnlinkNode(elem);

    if (elem->type == XML_TEXT_NODE) {
	if (cur->type == XML_TEXT_NODE) {
	    xmlChar *tmp;

	    tmp = xmlStrdup(elem->content);
	    tmp = xmlStrcat(tmp, cur->content);
	    xmlNodeSetContent(cur, tmp);
	    xmlFree(tmp);
	    xmlFreeNode(elem);
	    return(cur);
	}
	if ((cur->prev != NULL) && (cur->prev->type == XML_TEXT_NODE) &&
            (cur->name == cur->prev->name)) {
	    xmlNodeAddContent(cur->prev, elem->content);
	    xmlFreeNode(elem);
	    return(cur->prev);
	}
    } else if (elem->type == XML_ATTRIBUTE_NODE) {
		return xmlAddPropSibling(cur->prev, cur, elem);
    }

    if (elem->doc != cur->doc) {
	xmlSetTreeDoc(elem, cur->doc);
    }
    elem->parent = cur->parent;
    elem->next = cur;
    elem->prev = cur->prev;
    cur->prev = elem;
    if (elem->prev != NULL)
	elem->prev->next = elem;
    if ((elem->parent != NULL) && (elem->parent->children == cur)) {
		elem->parent->children = elem;
    }
    return(elem);
}
#endif 

xmlNodePtr
xmlAddSibling(xmlNodePtr cur, xmlNodePtr elem) {
    xmlNodePtr parent;

    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddSibling : cur == NULL\n");
#endif
	return(NULL);
    }

    if (elem == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddSibling : elem == NULL\n");
#endif
	return(NULL);
    }

    if (cur == elem) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddSibling : cur == elem\n");
#endif
	return(NULL);
    }

    if ((cur->type != XML_ATTRIBUTE_NODE) && (cur->parent != NULL) &&
	(cur->parent->children != NULL) &&
	(cur->parent->last != NULL) &&
	(cur->parent->last->next == NULL)) {
	cur = cur->parent->last;
    } else {
	while (cur->next != NULL) cur = cur->next;
    }

    xmlUnlinkNode(elem);

    if ((cur->type == XML_TEXT_NODE) && (elem->type == XML_TEXT_NODE) &&
        (cur->name == elem->name)) {
	xmlNodeAddContent(cur, elem->content);
	xmlFreeNode(elem);
	return(cur);
    } else if (elem->type == XML_ATTRIBUTE_NODE) {
		return xmlAddPropSibling(cur, cur, elem);
    }

    if (elem->doc != cur->doc) {
	xmlSetTreeDoc(elem, cur->doc);
    }
    parent = cur->parent;
    elem->prev = cur;
    elem->next = NULL;
    elem->parent = parent;
    cur->next = elem;
    if (parent != NULL)
	parent->last = elem;

    return(elem);
}

xmlNodePtr
xmlAddChildList(xmlNodePtr parent, xmlNodePtr cur) {
    xmlNodePtr prev;

    if (parent == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddChildList : parent == NULL\n");
#endif
	return(NULL);
    }

    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddChildList : child == NULL\n");
#endif
	return(NULL);
    }

    if ((cur->doc != NULL) && (parent->doc != NULL) &&
        (cur->doc != parent->doc)) {
#ifdef DEBUG_TREE
	xmlGenericError(xmlGenericErrorContext,
		"Elements moved to a different document\n");
#endif
    }


    if (parent->children == NULL) {
        parent->children = cur;
    } else {
	if ((cur->type == XML_TEXT_NODE) &&
	    (parent->last->type == XML_TEXT_NODE) &&
	    (cur->name == parent->last->name)) {
	    xmlNodeAddContent(parent->last, cur->content);
	    if (cur->next == NULL) {
		xmlFreeNode(cur);
		return(parent->last);
	    }
	    prev = cur;
	    cur = cur->next;
	    xmlFreeNode(prev);
	}
        prev = parent->last;
	prev->next = cur;
	cur->prev = prev;
    }
    while (cur->next != NULL) {
	cur->parent = parent;
	if (cur->doc != parent->doc) {
	    xmlSetTreeDoc(cur, parent->doc);
	}
        cur = cur->next;
    }
    cur->parent = parent;
    
    if (cur->doc != parent->doc) {
        xmlSetTreeDoc(cur, parent->doc);
    }
    parent->last = cur;

    return(cur);
}

xmlNodePtr
xmlAddChild(xmlNodePtr parent, xmlNodePtr cur) {
    xmlNodePtr prev;

    if (parent == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddChild : parent == NULL\n");
#endif
	return(NULL);
    }

    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddChild : child == NULL\n");
#endif
	return(NULL);
    }

    if (parent == cur) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlAddChild : parent == cur\n");
#endif
	return(NULL);
    }
    if (cur->type == XML_TEXT_NODE) {
	if ((parent->type == XML_TEXT_NODE) &&
	    (parent->content != NULL) &&
	    (parent->name == cur->name)) {
	    xmlNodeAddContent(parent, cur->content);
	    xmlFreeNode(cur);
	    return(parent);
	}
	if ((parent->last != NULL) && (parent->last->type == XML_TEXT_NODE) &&
	    (parent->last->name == cur->name) &&
	    (parent->last != cur)) {
	    xmlNodeAddContent(parent->last, cur->content);
	    xmlFreeNode(cur);
	    return(parent->last);
	}
    }

    prev = cur->parent;
    cur->parent = parent;
    if (cur->doc != parent->doc) {
	xmlSetTreeDoc(cur, parent->doc);
    }
    if (prev == parent)
	return(cur);

    if ((parent->type == XML_TEXT_NODE) &&
	(parent->content != NULL) &&
	(parent != cur)) {
	xmlNodeAddContent(parent, cur->content);
	xmlFreeNode(cur);
	return(parent);
    }
    if (cur->type == XML_ATTRIBUTE_NODE) {
		if (parent->type != XML_ELEMENT_NODE)
			return(NULL);
	if (parent->properties != NULL) {
	    
	    xmlAttrPtr lastattr;

	    if (cur->ns == NULL)
		lastattr = xmlHasNsProp(parent, cur->name, NULL);
	    else
		lastattr = xmlHasNsProp(parent, cur->name, cur->ns->href);
	    if ((lastattr != NULL) && (lastattr != (xmlAttrPtr) cur) && (lastattr->type != XML_ATTRIBUTE_DECL)) {
		
			xmlUnlinkNode((xmlNodePtr) lastattr);
		xmlFreeProp(lastattr);
	    }
		if (lastattr == (xmlAttrPtr) cur)
			return(cur);

	}
	if (parent->properties == NULL) {
	    parent->properties = (xmlAttrPtr) cur;
	} else {
	    
	    xmlAttrPtr lastattr = parent->properties;
	    while (lastattr->next != NULL) {
		lastattr = lastattr->next;
	    }
	    lastattr->next = (xmlAttrPtr) cur;
	    ((xmlAttrPtr) cur)->prev = lastattr;
	}
    } else {
	if (parent->children == NULL) {
	    parent->children = cur;
	    parent->last = cur;
	} else {
	    prev = parent->last;
	    prev->next = cur;
	    cur->prev = prev;
	    parent->last = cur;
	}
    }
    return(cur);
}

xmlNodePtr
xmlGetLastChild(xmlNodePtr parent) {
    if (parent == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlGetLastChild : parent == NULL\n");
#endif
	return(NULL);
    }
    return(parent->last);
}

#ifdef LIBXML_TREE_ENABLED

unsigned long
xmlChildElementCount(xmlNodePtr parent) {
    unsigned long ret = 0;
    xmlNodePtr cur = NULL;

    if (parent == NULL)
        return(0);
    switch (parent->type) {
        case XML_ELEMENT_NODE:
        case XML_ENTITY_NODE:
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
            cur = parent->children;
            break;
        default:
            return(0);
    }
    while (cur != NULL) {
        if (cur->type == XML_ELEMENT_NODE)
            ret++;
        cur = cur->next;
    }
    return(ret);
}

xmlNodePtr
xmlFirstElementChild(xmlNodePtr parent) {
    xmlNodePtr cur = NULL;

    if (parent == NULL)
        return(NULL);
    switch (parent->type) {
        case XML_ELEMENT_NODE:
        case XML_ENTITY_NODE:
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
            cur = parent->children;
            break;
        default:
            return(NULL);
    }
    while (cur != NULL) {
        if (cur->type == XML_ELEMENT_NODE)
            return(cur);
        cur = cur->next;
    }
    return(NULL);
}

xmlNodePtr
xmlLastElementChild(xmlNodePtr parent) {
    xmlNodePtr cur = NULL;

    if (parent == NULL)
        return(NULL);
    switch (parent->type) {
        case XML_ELEMENT_NODE:
        case XML_ENTITY_NODE:
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
            cur = parent->last;
            break;
        default:
            return(NULL);
    }
    while (cur != NULL) {
        if (cur->type == XML_ELEMENT_NODE)
            return(cur);
        cur = cur->prev;
    }
    return(NULL);
}

xmlNodePtr
xmlPreviousElementSibling(xmlNodePtr node) {
    if (node == NULL)
        return(NULL);
    switch (node->type) {
        case XML_ELEMENT_NODE:
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
            node = node->prev;
            break;
        default:
            return(NULL);
    }
    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE)
            return(node);
        node = node->prev;
    }
    return(NULL);
}

xmlNodePtr
xmlNextElementSibling(xmlNodePtr node) {
    if (node == NULL)
        return(NULL);
    switch (node->type) {
        case XML_ELEMENT_NODE:
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_DTD_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
            node = node->next;
            break;
        default:
            return(NULL);
    }
    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE)
            return(node);
        node = node->next;
    }
    return(NULL);
}

#endif 

void
xmlFreeNodeList(xmlNodePtr cur) {
    xmlNodePtr next;
    xmlDictPtr dict = NULL;

    if (cur == NULL) return;
    if (cur->type == XML_NAMESPACE_DECL) {
	xmlFreeNsList((xmlNsPtr) cur);
	return;
    }
    if ((cur->type == XML_DOCUMENT_NODE) ||
#ifdef LIBXML_DOCB_ENABLED
	(cur->type == XML_DOCB_DOCUMENT_NODE) ||
#endif
	(cur->type == XML_HTML_DOCUMENT_NODE)) {
	xmlFreeDoc((xmlDocPtr) cur);
	return;
    }
    if (cur->doc != NULL) dict = cur->doc->dict;
    while (cur != NULL) {
        next = cur->next;
	if (cur->type != XML_DTD_NODE) {

	    if ((__xmlRegisterCallbacks) && (xmlDeregisterNodeDefaultValue))
		xmlDeregisterNodeDefaultValue(cur);

	    if ((cur->children != NULL) &&
		(cur->type != XML_ENTITY_REF_NODE))
		xmlFreeNodeList(cur->children);
	    if (((cur->type == XML_ELEMENT_NODE) ||
		 (cur->type == XML_XINCLUDE_START) ||
		 (cur->type == XML_XINCLUDE_END)) &&
		(cur->properties != NULL))
		xmlFreePropList(cur->properties);
	    if ((cur->type != XML_ELEMENT_NODE) &&
		(cur->type != XML_XINCLUDE_START) &&
		(cur->type != XML_XINCLUDE_END) &&
		(cur->type != XML_ENTITY_REF_NODE) &&
		(cur->content != (xmlChar *) &(cur->properties))) {
		DICT_FREE(cur->content)
	    }
	    if (((cur->type == XML_ELEMENT_NODE) ||
	         (cur->type == XML_XINCLUDE_START) ||
		 (cur->type == XML_XINCLUDE_END)) &&
		(cur->nsDef != NULL))
		xmlFreeNsList(cur->nsDef);

	    if ((cur->name != NULL) &&
		(cur->type != XML_TEXT_NODE) &&
		(cur->type != XML_COMMENT_NODE))
		DICT_FREE(cur->name)
	    xmlFree(cur);
	}
	cur = next;
    }
}

void
xmlFreeNode(xmlNodePtr cur) {
    xmlDictPtr dict = NULL;

    if (cur == NULL) return;

    
    if (cur->type == XML_DTD_NODE) {
	xmlFreeDtd((xmlDtdPtr) cur);
	return;
    }
    if (cur->type == XML_NAMESPACE_DECL) {
	xmlFreeNs((xmlNsPtr) cur);
        return;
    }
    if (cur->type == XML_ATTRIBUTE_NODE) {
	xmlFreeProp((xmlAttrPtr) cur);
	return;
    }

    if ((__xmlRegisterCallbacks) && (xmlDeregisterNodeDefaultValue))
	xmlDeregisterNodeDefaultValue(cur);

    if (cur->doc != NULL) dict = cur->doc->dict;

    if (cur->type == XML_ENTITY_DECL) {
        xmlEntityPtr ent = (xmlEntityPtr) cur;
	DICT_FREE(ent->SystemID);
	DICT_FREE(ent->ExternalID);
    }
    if ((cur->children != NULL) &&
	(cur->type != XML_ENTITY_REF_NODE))
	xmlFreeNodeList(cur->children);
    if (((cur->type == XML_ELEMENT_NODE) ||
	 (cur->type == XML_XINCLUDE_START) ||
	 (cur->type == XML_XINCLUDE_END)) &&
	(cur->properties != NULL))
	xmlFreePropList(cur->properties);
    if ((cur->type != XML_ELEMENT_NODE) &&
	(cur->content != NULL) &&
	(cur->type != XML_ENTITY_REF_NODE) &&
	(cur->type != XML_XINCLUDE_END) &&
	(cur->type != XML_XINCLUDE_START) &&
	(cur->content != (xmlChar *) &(cur->properties))) {
	DICT_FREE(cur->content)
    }

    if ((cur->name != NULL) &&
        (cur->type != XML_TEXT_NODE) &&
        (cur->type != XML_COMMENT_NODE))
	DICT_FREE(cur->name)

    if (((cur->type == XML_ELEMENT_NODE) ||
	 (cur->type == XML_XINCLUDE_START) ||
	 (cur->type == XML_XINCLUDE_END)) &&
	(cur->nsDef != NULL))
	xmlFreeNsList(cur->nsDef);
    xmlFree(cur);
}

void
xmlUnlinkNode(xmlNodePtr cur) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlUnlinkNode : node == NULL\n");
#endif
	return;
    }
    if (cur->type == XML_DTD_NODE) {
	xmlDocPtr doc;
	doc = cur->doc;
	if (doc != NULL) {
	    if (doc->intSubset == (xmlDtdPtr) cur)
		doc->intSubset = NULL;
	    if (doc->extSubset == (xmlDtdPtr) cur)
		doc->extSubset = NULL;
	}
    }
    if (cur->type == XML_ENTITY_DECL) {
        xmlDocPtr doc;
	doc = cur->doc;
	if (doc != NULL) {
	    if (doc->intSubset != NULL) {
	        if (xmlHashLookup(doc->intSubset->entities, cur->name) == cur)
		    xmlHashRemoveEntry(doc->intSubset->entities, cur->name,
		                       NULL);
	        if (xmlHashLookup(doc->intSubset->pentities, cur->name) == cur)
		    xmlHashRemoveEntry(doc->intSubset->pentities, cur->name,
		                       NULL);
	    }
	    if (doc->extSubset != NULL) {
	        if (xmlHashLookup(doc->extSubset->entities, cur->name) == cur)
		    xmlHashRemoveEntry(doc->extSubset->entities, cur->name,
		                       NULL);
	        if (xmlHashLookup(doc->extSubset->pentities, cur->name) == cur)
		    xmlHashRemoveEntry(doc->extSubset->pentities, cur->name,
		                       NULL);
	    }
	}
    }
    if (cur->parent != NULL) {
	xmlNodePtr parent;
	parent = cur->parent;
	if (cur->type == XML_ATTRIBUTE_NODE) {
	    if (parent->properties == (xmlAttrPtr) cur)
		parent->properties = ((xmlAttrPtr) cur)->next;
	} else {
	    if (parent->children == cur)
		parent->children = cur->next;
	    if (parent->last == cur)
		parent->last = cur->prev;
	}
	cur->parent = NULL;
    }
    if (cur->next != NULL)
        cur->next->prev = cur->prev;
    if (cur->prev != NULL)
        cur->prev->next = cur->next;
    cur->next = cur->prev = NULL;
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_WRITER_ENABLED)
xmlNodePtr
xmlReplaceNode(xmlNodePtr old, xmlNodePtr cur) {
    if (old == cur) return(NULL);
    if ((old == NULL) || (old->parent == NULL)) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlReplaceNode : old == NULL or without parent\n");
#endif
	return(NULL);
    }
    if (cur == NULL) {
	xmlUnlinkNode(old);
	return(old);
    }
    if (cur == old) {
	return(old);
    }
    if ((old->type==XML_ATTRIBUTE_NODE) && (cur->type!=XML_ATTRIBUTE_NODE)) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlReplaceNode : Trying to replace attribute node with other node type\n");
#endif
	return(old);
    }
    if ((cur->type==XML_ATTRIBUTE_NODE) && (old->type!=XML_ATTRIBUTE_NODE)) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlReplaceNode : Trying to replace a non-attribute node with attribute node\n");
#endif
	return(old);
    }
    xmlUnlinkNode(cur);
    xmlSetTreeDoc(cur, old->doc);
    cur->parent = old->parent;
    cur->next = old->next;
    if (cur->next != NULL)
	cur->next->prev = cur;
    cur->prev = old->prev;
    if (cur->prev != NULL)
	cur->prev->next = cur;
    if (cur->parent != NULL) {
	if (cur->type == XML_ATTRIBUTE_NODE) {
	    if (cur->parent->properties == (xmlAttrPtr)old)
		cur->parent->properties = ((xmlAttrPtr) cur);
	} else {
	    if (cur->parent->children == old)
		cur->parent->children = cur;
	    if (cur->parent->last == old)
		cur->parent->last = cur;
	}
    }
    old->next = old->prev = NULL;
    old->parent = NULL;
    return(old);
}
#endif 


xmlNsPtr
xmlCopyNamespace(xmlNsPtr cur) {
    xmlNsPtr ret;

    if (cur == NULL) return(NULL);
    switch (cur->type) {
	case XML_LOCAL_NAMESPACE:
	    ret = xmlNewNs(NULL, cur->href, cur->prefix);
	    break;
	default:
#ifdef DEBUG_TREE
	    xmlGenericError(xmlGenericErrorContext,
		    "xmlCopyNamespace: invalid type %d\n", cur->type);
#endif
	    return(NULL);
    }
    return(ret);
}

xmlNsPtr
xmlCopyNamespaceList(xmlNsPtr cur) {
    xmlNsPtr ret = NULL;
    xmlNsPtr p = NULL,q;

    while (cur != NULL) {
        q = xmlCopyNamespace(cur);
	if (p == NULL) {
	    ret = p = q;
	} else {
	    p->next = q;
	    p = q;
	}
	cur = cur->next;
    }
    return(ret);
}

static xmlNodePtr
xmlStaticCopyNodeList(xmlNodePtr node, xmlDocPtr doc, xmlNodePtr parent);

static xmlAttrPtr
xmlCopyPropInternal(xmlDocPtr doc, xmlNodePtr target, xmlAttrPtr cur) {
    xmlAttrPtr ret;

    if (cur == NULL) return(NULL);
    if (target != NULL)
	ret = xmlNewDocProp(target->doc, cur->name, NULL);
    else if (doc != NULL)
	ret = xmlNewDocProp(doc, cur->name, NULL);
    else if (cur->parent != NULL)
	ret = xmlNewDocProp(cur->parent->doc, cur->name, NULL);
    else if (cur->children != NULL)
	ret = xmlNewDocProp(cur->children->doc, cur->name, NULL);
    else
	ret = xmlNewDocProp(NULL, cur->name, NULL);
    if (ret == NULL) return(NULL);
    ret->parent = target;

    if ((cur->ns != NULL) && (target != NULL)) {
      xmlNsPtr ns;

      ns = xmlSearchNs(target->doc, target, cur->ns->prefix);
      if (ns == NULL) {
        ns = xmlSearchNs(cur->doc, cur->parent, cur->ns->prefix);
        if (ns != NULL) {
          xmlNodePtr root = target;
          xmlNodePtr pred = NULL;

          while (root->parent != NULL) {
            pred = root;
            root = root->parent;
          }
          if (root == (xmlNodePtr) target->doc) {
            
            root = pred;
          }
          ret->ns = xmlNewNs(root, ns->href, ns->prefix);
        }
      } else {
        if (xmlStrEqual(ns->href, cur->ns->href)) {
          
          ret->ns = ns;
        } else {
          ret->ns = xmlNewReconciliedNs(target->doc, target, cur->ns);
        }
      }

    } else
        ret->ns = NULL;

    if (cur->children != NULL) {
	xmlNodePtr tmp;

	ret->children = xmlStaticCopyNodeList(cur->children, ret->doc, (xmlNodePtr) ret);
	ret->last = NULL;
	tmp = ret->children;
	while (tmp != NULL) {
	    
	    if (tmp->next == NULL)
	        ret->last = tmp;
	    tmp = tmp->next;
	}
    }
    if ((target!= NULL) && (cur!= NULL) &&
	(target->doc != NULL) && (cur->doc != NULL) &&
	(cur->doc->ids != NULL) && (cur->parent != NULL)) {
	if (xmlIsID(cur->doc, cur->parent, cur)) {
	    xmlChar *id;

	    id = xmlNodeListGetString(cur->doc, cur->children, 1);
	    if (id != NULL) {
		xmlAddID(NULL, target->doc, id, ret);
		xmlFree(id);
	    }
	}
    }
    return(ret);
}

xmlAttrPtr
xmlCopyProp(xmlNodePtr target, xmlAttrPtr cur) {
	return xmlCopyPropInternal(NULL, target, cur);
}

xmlAttrPtr
xmlCopyPropList(xmlNodePtr target, xmlAttrPtr cur) {
    xmlAttrPtr ret = NULL;
    xmlAttrPtr p = NULL,q;

    while (cur != NULL) {
        q = xmlCopyProp(target, cur);
	if (q == NULL)
	    return(NULL);
	if (p == NULL) {
	    ret = p = q;
	} else {
	    p->next = q;
	    q->prev = p;
	    p = q;
	}
	cur = cur->next;
    }
    return(ret);
}

/*
 * NOTE about the CopyNode operations !
 *
 * They are split into external and internal parts for one
 * tricky reason: namespaces. Doing a direct copy of a node
 * say RPM:Copyright without changing the namespace pointer to
 * something else can produce stale links. One way to do it is
 * to keep a reference counter but this doesn't work as soon
 * as one move the element or the subtree out of the scope of
 * the existing namespace. The actual solution seems to add
 * a copy of the namespace at the top of the copied tree if
 * not available in the subtree.
 * Hence two functions, the public front-end call the inner ones
 * The argument "recursive" normally indicates a recursive copy
 * of the node with values 0 (no) and 1 (yes).  For XInclude,
 * however, we allow a value of 2 to indicate copy properties and
 * namespace info, but don't recurse on children.
 */

static xmlNodePtr
xmlStaticCopyNode(const xmlNodePtr node, xmlDocPtr doc, xmlNodePtr parent,
                  int extended) {
    xmlNodePtr ret;

    if (node == NULL) return(NULL);
    switch (node->type) {
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ELEMENT_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
	    break;
        case XML_ATTRIBUTE_NODE:
		return((xmlNodePtr) xmlCopyPropInternal(doc, parent, (xmlAttrPtr) node));
        case XML_NAMESPACE_DECL:
	    return((xmlNodePtr) xmlCopyNamespaceList((xmlNsPtr) node));

        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
        case XML_DOCB_DOCUMENT_NODE:
#endif
#ifdef LIBXML_TREE_ENABLED
	    return((xmlNodePtr) xmlCopyDoc((xmlDocPtr) node, extended));
#endif 
        case XML_DOCUMENT_TYPE_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
            return(NULL);
    }

    ret = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
    if (ret == NULL) {
	xmlTreeErrMemory("copying node");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlNode));
    ret->type = node->type;

    ret->doc = doc;
    ret->parent = parent;
    if (node->name == xmlStringText)
	ret->name = xmlStringText;
    else if (node->name == xmlStringTextNoenc)
	ret->name = xmlStringTextNoenc;
    else if (node->name == xmlStringComment)
	ret->name = xmlStringComment;
    else if (node->name != NULL) {
        if ((doc != NULL) && (doc->dict != NULL))
	    ret->name = xmlDictLookup(doc->dict, node->name, -1);
	else
	    ret->name = xmlStrdup(node->name);
    }
    if ((node->type != XML_ELEMENT_NODE) &&
	(node->content != NULL) &&
	(node->type != XML_ENTITY_REF_NODE) &&
	(node->type != XML_XINCLUDE_END) &&
	(node->type != XML_XINCLUDE_START)) {
	ret->content = xmlStrdup(node->content);
    }else{
      if (node->type == XML_ELEMENT_NODE)
        ret->line = node->line;
    }
    if (parent != NULL) {
	xmlNodePtr tmp;

	if ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue))
	    xmlRegisterNodeDefaultValue((xmlNodePtr)ret);

        tmp = xmlAddChild(parent, ret);
	
	if (tmp != ret)
	    return(tmp);
    }

    if (!extended)
	goto out;
    if (((node->type == XML_ELEMENT_NODE) ||
         (node->type == XML_XINCLUDE_START)) && (node->nsDef != NULL))
        ret->nsDef = xmlCopyNamespaceList(node->nsDef);

    if (node->ns != NULL) {
        xmlNsPtr ns;

	ns = xmlSearchNs(doc, ret, node->ns->prefix);
	if (ns == NULL) {
	    ns = xmlSearchNs(node->doc, node, node->ns->prefix);
	    if (ns != NULL) {
	        xmlNodePtr root = ret;

		while (root->parent != NULL) root = root->parent;
		ret->ns = xmlNewNs(root, ns->href, ns->prefix);
		} else {
			ret->ns = xmlNewReconciliedNs(doc, ret, node->ns);
	    }
	} else {
	    ret->ns = ns;
	}
    }
    if (((node->type == XML_ELEMENT_NODE) ||
         (node->type == XML_XINCLUDE_START)) && (node->properties != NULL))
        ret->properties = xmlCopyPropList(ret, node->properties);
    if (node->type == XML_ENTITY_REF_NODE) {
	if ((doc == NULL) || (node->doc != doc)) {
	    ret->children = (xmlNodePtr) xmlGetDocEntity(doc, ret->name);
	} else {
            ret->children = node->children;
	}
	ret->last = ret->children;
    } else if ((node->children != NULL) && (extended != 2)) {
        ret->children = xmlStaticCopyNodeList(node->children, doc, ret);
	UPDATE_LAST_CHILD_AND_PARENT(ret)
    }

out:
    
    if ((parent == NULL) &&
        ((__xmlRegisterCallbacks) && (xmlRegisterNodeDefaultValue)))
	xmlRegisterNodeDefaultValue((xmlNodePtr)ret);
    return(ret);
}

static xmlNodePtr
xmlStaticCopyNodeList(xmlNodePtr node, xmlDocPtr doc, xmlNodePtr parent) {
    xmlNodePtr ret = NULL;
    xmlNodePtr p = NULL,q;

    while (node != NULL) {
#ifdef LIBXML_TREE_ENABLED
	if (node->type == XML_DTD_NODE ) {
	    if (doc == NULL) {
		node = node->next;
		continue;
	    }
	    if (doc->intSubset == NULL) {
		q = (xmlNodePtr) xmlCopyDtd( (xmlDtdPtr) node );
		q->doc = doc;
		q->parent = parent;
		doc->intSubset = (xmlDtdPtr) q;
		xmlAddChild(parent, q);
	    } else {
		q = (xmlNodePtr) doc->intSubset;
		xmlAddChild(parent, q);
	    }
	} else
#endif 
	    q = xmlStaticCopyNode(node, doc, parent, 1);
	if (ret == NULL) {
	    q->prev = NULL;
	    ret = p = q;
	} else if (p != q) {
	
	    p->next = q;
	    q->prev = p;
	    p = q;
	}
	node = node->next;
    }
    return(ret);
}

xmlNodePtr
xmlCopyNode(const xmlNodePtr node, int extended) {
    xmlNodePtr ret;

    ret = xmlStaticCopyNode(node, NULL, NULL, extended);
    return(ret);
}

xmlNodePtr
xmlDocCopyNode(const xmlNodePtr node, xmlDocPtr doc, int extended) {
    xmlNodePtr ret;

    ret = xmlStaticCopyNode(node, doc, NULL, extended);
    return(ret);
}

xmlNodePtr xmlDocCopyNodeList(xmlDocPtr doc, const xmlNodePtr node) {
    xmlNodePtr ret = xmlStaticCopyNodeList(node, doc, NULL);
    return(ret);
}

xmlNodePtr xmlCopyNodeList(const xmlNodePtr node) {
    xmlNodePtr ret = xmlStaticCopyNodeList(node, NULL, NULL);
    return(ret);
}

#if defined(LIBXML_TREE_ENABLED)
xmlDtdPtr
xmlCopyDtd(xmlDtdPtr dtd) {
    xmlDtdPtr ret;
    xmlNodePtr cur, p = NULL, q;

    if (dtd == NULL) return(NULL);
    ret = xmlNewDtd(NULL, dtd->name, dtd->ExternalID, dtd->SystemID);
    if (ret == NULL) return(NULL);
    if (dtd->entities != NULL)
        ret->entities = (void *) xmlCopyEntitiesTable(
	                    (xmlEntitiesTablePtr) dtd->entities);
    if (dtd->notations != NULL)
        ret->notations = (void *) xmlCopyNotationTable(
	                    (xmlNotationTablePtr) dtd->notations);
    if (dtd->elements != NULL)
        ret->elements = (void *) xmlCopyElementTable(
	                    (xmlElementTablePtr) dtd->elements);
    if (dtd->attributes != NULL)
        ret->attributes = (void *) xmlCopyAttributeTable(
	                    (xmlAttributeTablePtr) dtd->attributes);
    if (dtd->pentities != NULL)
	ret->pentities = (void *) xmlCopyEntitiesTable(
			    (xmlEntitiesTablePtr) dtd->pentities);

    cur = dtd->children;
    while (cur != NULL) {
	q = NULL;

	if (cur->type == XML_ENTITY_DECL) {
	    xmlEntityPtr tmp = (xmlEntityPtr) cur;
	    switch (tmp->etype) {
		case XML_INTERNAL_GENERAL_ENTITY:
		case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
		case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
		    q = (xmlNodePtr) xmlGetEntityFromDtd(ret, tmp->name);
		    break;
		case XML_INTERNAL_PARAMETER_ENTITY:
		case XML_EXTERNAL_PARAMETER_ENTITY:
		    q = (xmlNodePtr)
			xmlGetParameterEntityFromDtd(ret, tmp->name);
		    break;
		case XML_INTERNAL_PREDEFINED_ENTITY:
		    break;
	    }
	} else if (cur->type == XML_ELEMENT_DECL) {
	    xmlElementPtr tmp = (xmlElementPtr) cur;
	    q = (xmlNodePtr)
		xmlGetDtdQElementDesc(ret, tmp->name, tmp->prefix);
	} else if (cur->type == XML_ATTRIBUTE_DECL) {
	    xmlAttributePtr tmp = (xmlAttributePtr) cur;
	    q = (xmlNodePtr)
		xmlGetDtdQAttrDesc(ret, tmp->elem, tmp->name, tmp->prefix);
	} else if (cur->type == XML_COMMENT_NODE) {
	    q = xmlCopyNode(cur, 0);
	}

	if (q == NULL) {
	    cur = cur->next;
	    continue;
	}

	if (p == NULL)
	    ret->children = q;
	else
	    p->next = q;

	q->prev = p;
	q->parent = (xmlNodePtr) ret;
	q->next = NULL;
	ret->last = q;
	p = q;
	cur = cur->next;
    }

    return(ret);
}
#endif

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
xmlDocPtr
xmlCopyDoc(xmlDocPtr doc, int recursive) {
    xmlDocPtr ret;

    if (doc == NULL) return(NULL);
    ret = xmlNewDoc(doc->version);
    if (ret == NULL) return(NULL);
    if (doc->name != NULL)
        ret->name = xmlMemStrdup(doc->name);
    if (doc->encoding != NULL)
        ret->encoding = xmlStrdup(doc->encoding);
    if (doc->URL != NULL)
        ret->URL = xmlStrdup(doc->URL);
    ret->charset = doc->charset;
    ret->compression = doc->compression;
    ret->standalone = doc->standalone;
    if (!recursive) return(ret);

    ret->last = NULL;
    ret->children = NULL;
#ifdef LIBXML_TREE_ENABLED
    if (doc->intSubset != NULL) {
        ret->intSubset = xmlCopyDtd(doc->intSubset);
	xmlSetTreeDoc((xmlNodePtr)ret->intSubset, ret);
	ret->intSubset->parent = ret;
    }
#endif
    if (doc->oldNs != NULL)
        ret->oldNs = xmlCopyNamespaceList(doc->oldNs);
    if (doc->children != NULL) {
	xmlNodePtr tmp;

	ret->children = xmlStaticCopyNodeList(doc->children, ret,
		                               (xmlNodePtr)ret);
	ret->last = NULL;
	tmp = ret->children;
	while (tmp != NULL) {
	    if (tmp->next == NULL)
	        ret->last = tmp;
	    tmp = tmp->next;
	}
    }
    return(ret);
}
#endif 


long
xmlGetLineNo(xmlNodePtr node)
{
    long result = -1;

    if (!node)
        return result;
    if ((node->type == XML_ELEMENT_NODE) ||
        (node->type == XML_TEXT_NODE) ||
	(node->type == XML_COMMENT_NODE) ||
	(node->type == XML_PI_NODE))
        result = (long) node->line;
    else if ((node->prev != NULL) &&
             ((node->prev->type == XML_ELEMENT_NODE) ||
	      (node->prev->type == XML_TEXT_NODE) ||
	      (node->prev->type == XML_COMMENT_NODE) ||
	      (node->prev->type == XML_PI_NODE)))
        result = xmlGetLineNo(node->prev);
    else if ((node->parent != NULL) &&
             (node->parent->type == XML_ELEMENT_NODE))
        result = xmlGetLineNo(node->parent);

    return result;
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_DEBUG_ENABLED)
xmlChar *
xmlGetNodePath(xmlNodePtr node)
{
    xmlNodePtr cur, tmp, next;
    xmlChar *buffer = NULL, *temp;
    size_t buf_len;
    xmlChar *buf;
    const char *sep;
    const char *name;
    char nametemp[100];
    int occur = 0, generic;

    if (node == NULL)
        return (NULL);

    buf_len = 500;
    buffer = (xmlChar *) xmlMallocAtomic(buf_len * sizeof(xmlChar));
    if (buffer == NULL) {
	xmlTreeErrMemory("getting node path");
        return (NULL);
    }
    buf = (xmlChar *) xmlMallocAtomic(buf_len * sizeof(xmlChar));
    if (buf == NULL) {
	xmlTreeErrMemory("getting node path");
        xmlFree(buffer);
        return (NULL);
    }

    buffer[0] = 0;
    cur = node;
    do {
        name = "";
        sep = "?";
        occur = 0;
        if ((cur->type == XML_DOCUMENT_NODE) ||
            (cur->type == XML_HTML_DOCUMENT_NODE)) {
            if (buffer[0] == '/')
                break;
            sep = "/";
            next = NULL;
        } else if (cur->type == XML_ELEMENT_NODE) {
	    generic = 0;
            sep = "/";
            name = (const char *) cur->name;
            if (cur->ns) {
		if (cur->ns->prefix != NULL) {
                    snprintf(nametemp, sizeof(nametemp) - 1, "%s:%s",
			(char *)cur->ns->prefix, (char *)cur->name);
		    nametemp[sizeof(nametemp) - 1] = 0;
		    name = nametemp;
		} else {
		    generic = 1;
		    name = "*";
		}
            }
            next = cur->parent;

            tmp = cur->prev;
            while (tmp != NULL) {
                if ((tmp->type == XML_ELEMENT_NODE) &&
		    (generic ||
		     (xmlStrEqual(cur->name, tmp->name) &&
		     ((tmp->ns == cur->ns) ||
		      ((tmp->ns != NULL) && (cur->ns != NULL) &&
		       (xmlStrEqual(cur->ns->prefix, tmp->ns->prefix)))))))
                    occur++;
                tmp = tmp->prev;
            }
            if (occur == 0) {
                tmp = cur->next;
                while (tmp != NULL && occur == 0) {
                    if ((tmp->type == XML_ELEMENT_NODE) &&
			(generic ||
			 (xmlStrEqual(cur->name, tmp->name) &&
			 ((tmp->ns == cur->ns) ||
			  ((tmp->ns != NULL) && (cur->ns != NULL) &&
			   (xmlStrEqual(cur->ns->prefix, tmp->ns->prefix)))))))
                        occur++;
                    tmp = tmp->next;
                }
                if (occur != 0)
                    occur = 1;
            } else
                occur++;
        } else if (cur->type == XML_COMMENT_NODE) {
            sep = "/";
	    name = "comment()";
            next = cur->parent;

            tmp = cur->prev;
            while (tmp != NULL) {
                if (tmp->type == XML_COMMENT_NODE)
		    occur++;
                tmp = tmp->prev;
            }
            if (occur == 0) {
                tmp = cur->next;
                while (tmp != NULL && occur == 0) {
		  if (tmp->type == XML_COMMENT_NODE)
		    occur++;
                    tmp = tmp->next;
                }
                if (occur != 0)
                    occur = 1;
            } else
                occur++;
        } else if ((cur->type == XML_TEXT_NODE) ||
                   (cur->type == XML_CDATA_SECTION_NODE)) {
            sep = "/";
	    name = "text()";
            next = cur->parent;

            tmp = cur->prev;
            while (tmp != NULL) {
                if ((tmp->type == XML_TEXT_NODE) ||
		    (tmp->type == XML_CDATA_SECTION_NODE))
		    occur++;
                tmp = tmp->prev;
            }
            if (occur == 0) {
                tmp = cur->next;
                while (tmp != NULL) {
		    if ((tmp->type == XML_TEXT_NODE) ||
			(tmp->type == XML_CDATA_SECTION_NODE))
		    {
			occur = 1;
			break;
		    }
		    tmp = tmp->next;
		}
            } else
                occur++;
        } else if (cur->type == XML_PI_NODE) {
            sep = "/";
	    snprintf(nametemp, sizeof(nametemp) - 1,
		     "processing-instruction('%s')", (char *)cur->name);
            nametemp[sizeof(nametemp) - 1] = 0;
            name = nametemp;

	    next = cur->parent;

            tmp = cur->prev;
            while (tmp != NULL) {
                if ((tmp->type == XML_PI_NODE) &&
		    (xmlStrEqual(cur->name, tmp->name)))
                    occur++;
                tmp = tmp->prev;
            }
            if (occur == 0) {
                tmp = cur->next;
                while (tmp != NULL && occur == 0) {
                    if ((tmp->type == XML_PI_NODE) &&
			(xmlStrEqual(cur->name, tmp->name)))
                        occur++;
                    tmp = tmp->next;
                }
                if (occur != 0)
                    occur = 1;
            } else
                occur++;

        } else if (cur->type == XML_ATTRIBUTE_NODE) {
            sep = "/@";
            name = (const char *) (((xmlAttrPtr) cur)->name);
            if (cur->ns) {
	        if (cur->ns->prefix != NULL)
                    snprintf(nametemp, sizeof(nametemp) - 1, "%s:%s",
			(char *)cur->ns->prefix, (char *)cur->name);
		else
		    snprintf(nametemp, sizeof(nametemp) - 1, "%s",
			(char *)cur->name);
                nametemp[sizeof(nametemp) - 1] = 0;
                name = nametemp;
            }
            next = ((xmlAttrPtr) cur)->parent;
        } else {
            next = cur->parent;
        }

        if (xmlStrlen(buffer) + sizeof(nametemp) + 20 > buf_len) {
            buf_len =
                2 * buf_len + xmlStrlen(buffer) + sizeof(nametemp) + 20;
            temp = (xmlChar *) xmlRealloc(buffer, buf_len);
            if (temp == NULL) {
		xmlTreeErrMemory("getting node path");
                xmlFree(buf);
                xmlFree(buffer);
                return (NULL);
            }
            buffer = temp;
            temp = (xmlChar *) xmlRealloc(buf, buf_len);
            if (temp == NULL) {
		xmlTreeErrMemory("getting node path");
                xmlFree(buf);
                xmlFree(buffer);
                return (NULL);
            }
            buf = temp;
        }
        if (occur == 0)
            snprintf((char *) buf, buf_len, "%s%s%s",
                     sep, name, (char *) buffer);
        else
            snprintf((char *) buf, buf_len, "%s%s[%d]%s",
                     sep, name, occur, (char *) buffer);
        snprintf((char *) buffer, buf_len, "%s", (char *)buf);
        cur = next;
    } while (cur != NULL);
    xmlFree(buf);
    return (buffer);
}
#endif 

xmlNodePtr
xmlDocGetRootElement(xmlDocPtr doc) {
    xmlNodePtr ret;

    if (doc == NULL) return(NULL);
    ret = doc->children;
    while (ret != NULL) {
	if (ret->type == XML_ELEMENT_NODE)
	    return(ret);
        ret = ret->next;
    }
    return(ret);
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_WRITER_ENABLED)
xmlNodePtr
xmlDocSetRootElement(xmlDocPtr doc, xmlNodePtr root) {
    xmlNodePtr old = NULL;

    if (doc == NULL) return(NULL);
    if (root == NULL)
	return(NULL);
    xmlUnlinkNode(root);
    xmlSetTreeDoc(root, doc);
    root->parent = (xmlNodePtr) doc;
    old = doc->children;
    while (old != NULL) {
	if (old->type == XML_ELEMENT_NODE)
	    break;
        old = old->next;
    }
    if (old == NULL) {
	if (doc->children == NULL) {
	    doc->children = root;
	    doc->last = root;
	} else {
	    xmlAddSibling(doc->children, root);
	}
    } else {
	xmlReplaceNode(old, root);
    }
    return(old);
}
#endif

#if defined(LIBXML_TREE_ENABLED)
void
xmlNodeSetLang(xmlNodePtr cur, const xmlChar *lang) {
    xmlNsPtr ns;

    if (cur == NULL) return;
    switch(cur->type) {
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_COMMENT_NODE:
        case XML_DOCUMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_NOTATION_NODE:
        case XML_HTML_DOCUMENT_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
        case XML_PI_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
	case XML_NAMESPACE_DECL:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
	    return;
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
	    break;
    }
    ns = xmlSearchNsByHref(cur->doc, cur, XML_XML_NAMESPACE);
    if (ns == NULL)
	return;
    xmlSetNsProp(cur, ns, BAD_CAST "lang", lang);
}
#endif 

xmlChar *
xmlNodeGetLang(xmlNodePtr cur) {
    xmlChar *lang;

    while (cur != NULL) {
        lang = xmlGetNsProp(cur, BAD_CAST "lang", XML_XML_NAMESPACE);
	if (lang != NULL)
	    return(lang);
	cur = cur->parent;
    }
    return(NULL);
}


#ifdef LIBXML_TREE_ENABLED
void
xmlNodeSetSpacePreserve(xmlNodePtr cur, int val) {
    xmlNsPtr ns;

    if (cur == NULL) return;
    switch(cur->type) {
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_COMMENT_NODE:
        case XML_DOCUMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_NOTATION_NODE:
        case XML_HTML_DOCUMENT_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
        case XML_PI_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
	case XML_NAMESPACE_DECL:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	    return;
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
	    break;
    }
    ns = xmlSearchNsByHref(cur->doc, cur, XML_XML_NAMESPACE);
    if (ns == NULL)
	return;
    switch (val) {
    case 0:
	xmlSetNsProp(cur, ns, BAD_CAST "space", BAD_CAST "default");
	break;
    case 1:
	xmlSetNsProp(cur, ns, BAD_CAST "space", BAD_CAST "preserve");
	break;
    }
}
#endif 

int
xmlNodeGetSpacePreserve(xmlNodePtr cur) {
    xmlChar *space;

    while (cur != NULL) {
	space = xmlGetNsProp(cur, BAD_CAST "space", XML_XML_NAMESPACE);
	if (space != NULL) {
	    if (xmlStrEqual(space, BAD_CAST "preserve")) {
		xmlFree(space);
		return(1);
	    }
	    if (xmlStrEqual(space, BAD_CAST "default")) {
		xmlFree(space);
		return(0);
	    }
	    xmlFree(space);
	}
	cur = cur->parent;
    }
    return(-1);
}

#ifdef LIBXML_TREE_ENABLED
void
xmlNodeSetName(xmlNodePtr cur, const xmlChar *name) {
    xmlDocPtr doc;
    xmlDictPtr dict;

    if (cur == NULL) return;
    if (name == NULL) return;
    switch(cur->type) {
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_COMMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_NOTATION_NODE:
        case XML_HTML_DOCUMENT_NODE:
	case XML_NAMESPACE_DECL:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	    return;
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
        case XML_PI_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_DTD_NODE:
        case XML_DOCUMENT_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
	    break;
    }
    doc = cur->doc;
    if (doc != NULL)
	dict = doc->dict;
    else
        dict = NULL;
    if (dict != NULL) {
        if ((cur->name != NULL) && (!xmlDictOwns(dict, cur->name)))
	    xmlFree((xmlChar *) cur->name);
	cur->name = xmlDictLookup(dict, name, -1);
    } else {
	if (cur->name != NULL) xmlFree((xmlChar *) cur->name);
	cur->name = xmlStrdup(name);
    }
}
#endif

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XINCLUDE_ENABLED)
void
xmlNodeSetBase(xmlNodePtr cur, const xmlChar* uri) {
    xmlNsPtr ns;
    xmlChar* fixed;

    if (cur == NULL) return;
    switch(cur->type) {
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_COMMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_DOCUMENT_FRAG_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
        case XML_PI_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
	case XML_NAMESPACE_DECL:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
	    return;
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
	    break;
        case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
        case XML_HTML_DOCUMENT_NODE: {
	    xmlDocPtr doc = (xmlDocPtr) cur;

	    if (doc->URL != NULL)
		xmlFree((xmlChar *) doc->URL);
	    if (uri == NULL)
		doc->URL = NULL;
	    else
		doc->URL = xmlPathToURI(uri);
	    return;
	}
    }

    ns = xmlSearchNsByHref(cur->doc, cur, XML_XML_NAMESPACE);
    if (ns == NULL)
	return;
    fixed = xmlPathToURI(uri);
    if (fixed != NULL) {
	xmlSetNsProp(cur, ns, BAD_CAST "base", fixed);
	xmlFree(fixed);
    } else {
	xmlSetNsProp(cur, ns, BAD_CAST "base", uri);
    }
}
#endif 

xmlChar *
xmlNodeGetBase(xmlDocPtr doc, xmlNodePtr cur) {
    xmlChar *oldbase = NULL;
    xmlChar *base, *newbase;

    if ((cur == NULL) && (doc == NULL))
        return(NULL);
    if (doc == NULL) doc = cur->doc;
    if ((doc != NULL) && (doc->type == XML_HTML_DOCUMENT_NODE)) {
        cur = doc->children;
	while ((cur != NULL) && (cur->name != NULL)) {
	    if (cur->type != XML_ELEMENT_NODE) {
	        cur = cur->next;
		continue;
	    }
	    if (!xmlStrcasecmp(cur->name, BAD_CAST "html")) {
	        cur = cur->children;
		continue;
	    }
	    if (!xmlStrcasecmp(cur->name, BAD_CAST "head")) {
	        cur = cur->children;
		continue;
	    }
	    if (!xmlStrcasecmp(cur->name, BAD_CAST "base")) {
                return(xmlGetProp(cur, BAD_CAST "href"));
	    }
	    cur = cur->next;
	}
	return(NULL);
    }
    while (cur != NULL) {
	if (cur->type == XML_ENTITY_DECL) {
	    xmlEntityPtr ent = (xmlEntityPtr) cur;
	    return(xmlStrdup(ent->URI));
	}
	if (cur->type == XML_ELEMENT_NODE) {
	    base = xmlGetNsProp(cur, BAD_CAST "base", XML_XML_NAMESPACE);
	    if (base != NULL) {
		if (oldbase != NULL) {
		    newbase = xmlBuildURI(oldbase, base);
		    if (newbase != NULL) {
			xmlFree(oldbase);
			xmlFree(base);
			oldbase = newbase;
		    } else {
			xmlFree(oldbase);
			xmlFree(base);
			return(NULL);
		    }
		} else {
		    oldbase = base;
		}
		if ((!xmlStrncmp(oldbase, BAD_CAST "http://", 7)) ||
		    (!xmlStrncmp(oldbase, BAD_CAST "ftp://", 6)) ||
		    (!xmlStrncmp(oldbase, BAD_CAST "urn:", 4)))
		    return(oldbase);
	    }
	}
	cur = cur->parent;
    }
    if ((doc != NULL) && (doc->URL != NULL)) {
	if (oldbase == NULL)
	    return(xmlStrdup(doc->URL));
	newbase = xmlBuildURI(oldbase, doc->URL);
	xmlFree(oldbase);
	return(newbase);
    }
    return(oldbase);
}

int
xmlNodeBufGetContent(xmlBufferPtr buffer, xmlNodePtr cur)
{
    if ((cur == NULL) || (buffer == NULL)) return(-1);
    switch (cur->type) {
        case XML_CDATA_SECTION_NODE:
        case XML_TEXT_NODE:
	    xmlBufferCat(buffer, cur->content);
            break;
        case XML_DOCUMENT_FRAG_NODE:
        case XML_ELEMENT_NODE:{
                xmlNodePtr tmp = cur;

                while (tmp != NULL) {
                    switch (tmp->type) {
                        case XML_CDATA_SECTION_NODE:
                        case XML_TEXT_NODE:
                            if (tmp->content != NULL)
                                xmlBufferCat(buffer, tmp->content);
                            break;
                        case XML_ENTITY_REF_NODE:
                            xmlNodeBufGetContent(buffer, tmp);
                            break;
                        default:
                            break;
                    }
                    if (tmp->children != NULL) {
                        if (tmp->children->type != XML_ENTITY_DECL) {
                            tmp = tmp->children;
                            continue;
                        }
                    }
                    if (tmp == cur)
                        break;

                    if (tmp->next != NULL) {
                        tmp = tmp->next;
                        continue;
                    }

                    do {
                        tmp = tmp->parent;
                        if (tmp == NULL)
                            break;
                        if (tmp == cur) {
                            tmp = NULL;
                            break;
                        }
                        if (tmp->next != NULL) {
                            tmp = tmp->next;
                            break;
                        }
                    } while (tmp != NULL);
                }
		break;
            }
        case XML_ATTRIBUTE_NODE:{
                xmlAttrPtr attr = (xmlAttrPtr) cur;
		xmlNodePtr tmp = attr->children;

		while (tmp != NULL) {
		    if (tmp->type == XML_TEXT_NODE)
		        xmlBufferCat(buffer, tmp->content);
		    else
		        xmlNodeBufGetContent(buffer, tmp);
		    tmp = tmp->next;
		}
                break;
            }
        case XML_COMMENT_NODE:
        case XML_PI_NODE:
	    xmlBufferCat(buffer, cur->content);
            break;
        case XML_ENTITY_REF_NODE:{
                xmlEntityPtr ent;
                xmlNodePtr tmp;

                
                ent = xmlGetDocEntity(cur->doc, cur->name);
                if (ent == NULL)
                    return(-1);

                tmp = ent->children;
                while (tmp) {
		    xmlNodeBufGetContent(buffer, tmp);
                    tmp = tmp->next;
                }
		break;
            }
        case XML_ENTITY_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
            break;
        case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
        case XML_DOCB_DOCUMENT_NODE:
#endif
        case XML_HTML_DOCUMENT_NODE:
	    cur = cur->children;
	    while (cur!= NULL) {
		if ((cur->type == XML_ELEMENT_NODE) ||
		    (cur->type == XML_TEXT_NODE) ||
		    (cur->type == XML_CDATA_SECTION_NODE)) {
		    xmlNodeBufGetContent(buffer, cur);
		}
		cur = cur->next;
	    }
	    break;
        case XML_NAMESPACE_DECL:
	    xmlBufferCat(buffer, ((xmlNsPtr) cur)->href);
	    break;
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
            break;
    }
    return(0);
}
xmlChar *
xmlNodeGetContent(xmlNodePtr cur)
{
    if (cur == NULL)
        return (NULL);
    switch (cur->type) {
        case XML_DOCUMENT_FRAG_NODE:
        case XML_ELEMENT_NODE:{
                xmlBufferPtr buffer;
                xmlChar *ret;

                buffer = xmlBufferCreateSize(64);
                if (buffer == NULL)
                    return (NULL);
		xmlNodeBufGetContent(buffer, cur);
                ret = buffer->content;
                buffer->content = NULL;
                xmlBufferFree(buffer);
                return (ret);
            }
        case XML_ATTRIBUTE_NODE:
	    return(xmlGetPropNodeValueInternal((xmlAttrPtr) cur));
        case XML_COMMENT_NODE:
        case XML_PI_NODE:
            if (cur->content != NULL)
                return (xmlStrdup(cur->content));
            return (NULL);
        case XML_ENTITY_REF_NODE:{
                xmlEntityPtr ent;
                xmlBufferPtr buffer;
                xmlChar *ret;

                
                ent = xmlGetDocEntity(cur->doc, cur->name);
                if (ent == NULL)
                    return (NULL);

                buffer = xmlBufferCreate();
                if (buffer == NULL)
                    return (NULL);

                xmlNodeBufGetContent(buffer, cur);

                ret = buffer->content;
                buffer->content = NULL;
                xmlBufferFree(buffer);
                return (ret);
            }
        case XML_ENTITY_NODE:
        case XML_DOCUMENT_TYPE_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
            return (NULL);
        case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
        case XML_DOCB_DOCUMENT_NODE:
#endif
        case XML_HTML_DOCUMENT_NODE: {
	    xmlBufferPtr buffer;
	    xmlChar *ret;

	    buffer = xmlBufferCreate();
	    if (buffer == NULL)
		return (NULL);

	    xmlNodeBufGetContent(buffer, (xmlNodePtr) cur);

	    ret = buffer->content;
	    buffer->content = NULL;
	    xmlBufferFree(buffer);
	    return (ret);
	}
        case XML_NAMESPACE_DECL: {
	    xmlChar *tmp;

	    tmp = xmlStrdup(((xmlNsPtr) cur)->href);
            return (tmp);
	}
        case XML_ELEMENT_DECL:
            
            return (NULL);
        case XML_ATTRIBUTE_DECL:
            
            return (NULL);
        case XML_ENTITY_DECL:
            
            return (NULL);
        case XML_CDATA_SECTION_NODE:
        case XML_TEXT_NODE:
            if (cur->content != NULL)
                return (xmlStrdup(cur->content));
            return (NULL);
    }
    return (NULL);
}

void
xmlNodeSetContent(xmlNodePtr cur, const xmlChar *content) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNodeSetContent : node == NULL\n");
#endif
	return;
    }
    switch (cur->type) {
        case XML_DOCUMENT_FRAG_NODE:
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
	    if (cur->children != NULL) xmlFreeNodeList(cur->children);
	    cur->children = xmlStringGetNodeList(cur->doc, content);
	    UPDATE_LAST_CHILD_AND_PARENT(cur)
	    break;
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
	    if ((cur->content != NULL) &&
	        (cur->content != (xmlChar *) &(cur->properties))) {
	        if (!((cur->doc != NULL) && (cur->doc->dict != NULL) &&
		    (xmlDictOwns(cur->doc->dict, cur->content))))
		    xmlFree(cur->content);
	    }
	    if (cur->children != NULL) xmlFreeNodeList(cur->children);
	    cur->last = cur->children = NULL;
	    if (content != NULL) {
		cur->content = xmlStrdup(content);
	    } else
		cur->content = NULL;
	    cur->properties = NULL;
	    cur->nsDef = NULL;
	    break;
        case XML_DOCUMENT_NODE:
        case XML_HTML_DOCUMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	    break;
        case XML_NOTATION_NODE:
	    break;
        case XML_DTD_NODE:
	    break;
	case XML_NAMESPACE_DECL:
	    break;
        case XML_ELEMENT_DECL:
	    
	    break;
        case XML_ATTRIBUTE_DECL:
	    
	    break;
        case XML_ENTITY_DECL:
	    
	    break;
    }
}

#ifdef LIBXML_TREE_ENABLED
void
xmlNodeSetContentLen(xmlNodePtr cur, const xmlChar *content, int len) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNodeSetContentLen : node == NULL\n");
#endif
	return;
    }
    switch (cur->type) {
        case XML_DOCUMENT_FRAG_NODE:
        case XML_ELEMENT_NODE:
        case XML_ATTRIBUTE_NODE:
	    if (cur->children != NULL) xmlFreeNodeList(cur->children);
	    cur->children = xmlStringLenGetNodeList(cur->doc, content, len);
	    UPDATE_LAST_CHILD_AND_PARENT(cur)
	    break;
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_NOTATION_NODE:
	    if ((cur->content != NULL) &&
	        (cur->content != (xmlChar *) &(cur->properties))) {
	        if (!((cur->doc != NULL) && (cur->doc->dict != NULL) &&
		    (xmlDictOwns(cur->doc->dict, cur->content))))
		    xmlFree(cur->content);
	    }
	    if (cur->children != NULL) xmlFreeNodeList(cur->children);
	    cur->children = cur->last = NULL;
	    if (content != NULL) {
		cur->content = xmlStrndup(content, len);
	    } else
		cur->content = NULL;
	    cur->properties = NULL;
	    cur->nsDef = NULL;
	    break;
        case XML_DOCUMENT_NODE:
        case XML_DTD_NODE:
        case XML_HTML_DOCUMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
	case XML_NAMESPACE_DECL:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	    break;
        case XML_ELEMENT_DECL:
	    
	    break;
        case XML_ATTRIBUTE_DECL:
	    
	    break;
        case XML_ENTITY_DECL:
	    
	    break;
    }
}
#endif 

void
xmlNodeAddContentLen(xmlNodePtr cur, const xmlChar *content, int len) {
    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNodeAddContentLen : node == NULL\n");
#endif
	return;
    }
    if (len <= 0) return;
    switch (cur->type) {
        case XML_DOCUMENT_FRAG_NODE:
        case XML_ELEMENT_NODE: {
	    xmlNodePtr last, newNode, tmp;

	    last = cur->last;
	    newNode = xmlNewTextLen(content, len);
	    if (newNode != NULL) {
		tmp = xmlAddChild(cur, newNode);
		if (tmp != newNode)
		    return;
	        if ((last != NULL) && (last->next == newNode)) {
		    xmlTextMerge(last, newNode);
		}
	    }
	    break;
	}
        case XML_ATTRIBUTE_NODE:
	    break;
        case XML_TEXT_NODE:
        case XML_CDATA_SECTION_NODE:
        case XML_ENTITY_REF_NODE:
        case XML_ENTITY_NODE:
        case XML_PI_NODE:
        case XML_COMMENT_NODE:
        case XML_NOTATION_NODE:
	    if (content != NULL) {
	        if ((cur->content == (xmlChar *) &(cur->properties)) ||
		    ((cur->doc != NULL) && (cur->doc->dict != NULL) &&
			    xmlDictOwns(cur->doc->dict, cur->content))) {
		    cur->content = xmlStrncatNew(cur->content, content, len);
		    cur->properties = NULL;
		    cur->nsDef = NULL;
		    break;
		}
		cur->content = xmlStrncat(cur->content, content, len);
            }
        case XML_DOCUMENT_NODE:
        case XML_DTD_NODE:
        case XML_HTML_DOCUMENT_NODE:
        case XML_DOCUMENT_TYPE_NODE:
	case XML_NAMESPACE_DECL:
	case XML_XINCLUDE_START:
	case XML_XINCLUDE_END:
#ifdef LIBXML_DOCB_ENABLED
	case XML_DOCB_DOCUMENT_NODE:
#endif
	    break;
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
	    break;
    }
}

void
xmlNodeAddContent(xmlNodePtr cur, const xmlChar *content) {
    int len;

    if (cur == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNodeAddContent : node == NULL\n");
#endif
	return;
    }
    if (content == NULL) return;
    len = xmlStrlen(content);
    xmlNodeAddContentLen(cur, content, len);
}

xmlNodePtr
xmlTextMerge(xmlNodePtr first, xmlNodePtr second) {
    if (first == NULL) return(second);
    if (second == NULL) return(first);
    if (first->type != XML_TEXT_NODE) return(first);
    if (second->type != XML_TEXT_NODE) return(first);
    if (second->name != first->name)
	return(first);
    xmlNodeAddContent(first, second->content);
    xmlUnlinkNode(second);
    xmlFreeNode(second);
    return(first);
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
xmlNsPtr *
xmlGetNsList(xmlDocPtr doc ATTRIBUTE_UNUSED, xmlNodePtr node)
{
    xmlNsPtr cur;
    xmlNsPtr *ret = NULL;
    int nbns = 0;
    int maxns = 10;
    int i;

    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE) {
            cur = node->nsDef;
            while (cur != NULL) {
                if (ret == NULL) {
                    ret =
                        (xmlNsPtr *) xmlMalloc((maxns + 1) *
                                               sizeof(xmlNsPtr));
                    if (ret == NULL) {
			xmlTreeErrMemory("getting namespace list");
                        return (NULL);
                    }
                    ret[nbns] = NULL;
                }
                for (i = 0; i < nbns; i++) {
                    if ((cur->prefix == ret[i]->prefix) ||
                        (xmlStrEqual(cur->prefix, ret[i]->prefix)))
                        break;
                }
                if (i >= nbns) {
                    if (nbns >= maxns) {
                        maxns *= 2;
                        ret = (xmlNsPtr *) xmlRealloc(ret,
                                                      (maxns +
                                                       1) *
                                                      sizeof(xmlNsPtr));
                        if (ret == NULL) {
			    xmlTreeErrMemory("getting namespace list");
                            return (NULL);
                        }
                    }
                    ret[nbns++] = cur;
                    ret[nbns] = NULL;
                }

                cur = cur->next;
            }
        }
        node = node->parent;
    }
    return (ret);
}
#endif 

static xmlNsPtr
xmlTreeEnsureXMLDecl(xmlDocPtr doc)
{
    if (doc == NULL)
	return (NULL);
    if (doc->oldNs != NULL)
	return (doc->oldNs);
    {
	xmlNsPtr ns;
	ns = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
	if (ns == NULL) {
	    xmlTreeErrMemory(
		"allocating the XML namespace");
	    return (NULL);
	}
	memset(ns, 0, sizeof(xmlNs));
	ns->type = XML_LOCAL_NAMESPACE;
	ns->href = xmlStrdup(XML_XML_NAMESPACE);
	ns->prefix = xmlStrdup((const xmlChar *)"xml");
	doc->oldNs = ns;
	return (ns);
    }
}

xmlNsPtr
xmlSearchNs(xmlDocPtr doc, xmlNodePtr node, const xmlChar *nameSpace) {

    xmlNsPtr cur;
    xmlNodePtr orig = node;

    if (node == NULL) return(NULL);
    if ((nameSpace != NULL) &&
	(xmlStrEqual(nameSpace, (const xmlChar *)"xml"))) {
	if ((doc == NULL) && (node->type == XML_ELEMENT_NODE)) {
	    cur = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
	    if (cur == NULL) {
		xmlTreeErrMemory("searching namespace");
		return(NULL);
	    }
	    memset(cur, 0, sizeof(xmlNs));
	    cur->type = XML_LOCAL_NAMESPACE;
	    cur->href = xmlStrdup(XML_XML_NAMESPACE);
	    cur->prefix = xmlStrdup((const xmlChar *)"xml");
	    cur->next = node->nsDef;
	    node->nsDef = cur;
	    return(cur);
	}
	if (doc == NULL) {
	    doc = node->doc;
	    if (doc == NULL)
		return(NULL);
	}
	if (doc->oldNs == NULL)
	    return(xmlTreeEnsureXMLDecl(doc));
	else
	    return(doc->oldNs);
    }
    while (node != NULL) {
	if ((node->type == XML_ENTITY_REF_NODE) ||
	    (node->type == XML_ENTITY_NODE) ||
	    (node->type == XML_ENTITY_DECL))
	    return(NULL);
	if (node->type == XML_ELEMENT_NODE) {
	    cur = node->nsDef;
	    while (cur != NULL) {
		if ((cur->prefix == NULL) && (nameSpace == NULL) &&
		    (cur->href != NULL))
		    return(cur);
		if ((cur->prefix != NULL) && (nameSpace != NULL) &&
		    (cur->href != NULL) &&
		    (xmlStrEqual(cur->prefix, nameSpace)))
		    return(cur);
		cur = cur->next;
	    }
	    if (orig != node) {
	        cur = node->ns;
	        if (cur != NULL) {
		    if ((cur->prefix == NULL) && (nameSpace == NULL) &&
		        (cur->href != NULL))
		        return(cur);
		    if ((cur->prefix != NULL) && (nameSpace != NULL) &&
		        (cur->href != NULL) &&
		        (xmlStrEqual(cur->prefix, nameSpace)))
		        return(cur);
	        }
	    }
	}
	node = node->parent;
    }
    return(NULL);
}

static int
xmlNsInScope(xmlDocPtr doc ATTRIBUTE_UNUSED, xmlNodePtr node,
             xmlNodePtr ancestor, const xmlChar * prefix)
{
    xmlNsPtr tst;

    while ((node != NULL) && (node != ancestor)) {
        if ((node->type == XML_ENTITY_REF_NODE) ||
            (node->type == XML_ENTITY_NODE) ||
            (node->type == XML_ENTITY_DECL))
            return (-1);
        if (node->type == XML_ELEMENT_NODE) {
            tst = node->nsDef;
            while (tst != NULL) {
                if ((tst->prefix == NULL)
                    && (prefix == NULL))
                    return (0);
                if ((tst->prefix != NULL)
                    && (prefix != NULL)
                    && (xmlStrEqual(tst->prefix, prefix)))
                    return (0);
                tst = tst->next;
            }
        }
        node = node->parent;
    }
    if (node != ancestor)
        return (-1);
    return (1);
}

xmlNsPtr
xmlSearchNsByHref(xmlDocPtr doc, xmlNodePtr node, const xmlChar * href)
{
    xmlNsPtr cur;
    xmlNodePtr orig = node;
    int is_attr;

    if ((node == NULL) || (href == NULL))
        return (NULL);
    if (xmlStrEqual(href, XML_XML_NAMESPACE)) {
        if ((doc == NULL) && (node->type == XML_ELEMENT_NODE)) {
            cur = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
            if (cur == NULL) {
		xmlTreeErrMemory("searching namespace");
                return (NULL);
            }
            memset(cur, 0, sizeof(xmlNs));
            cur->type = XML_LOCAL_NAMESPACE;
            cur->href = xmlStrdup(XML_XML_NAMESPACE);
            cur->prefix = xmlStrdup((const xmlChar *) "xml");
            cur->next = node->nsDef;
            node->nsDef = cur;
            return (cur);
        }
	if (doc == NULL) {
	    doc = node->doc;
	    if (doc == NULL)
		return(NULL);
	}
	if (doc->oldNs == NULL)
	    return(xmlTreeEnsureXMLDecl(doc));
	else
	    return(doc->oldNs);
    }
    is_attr = (node->type == XML_ATTRIBUTE_NODE);
    while (node != NULL) {
        if ((node->type == XML_ENTITY_REF_NODE) ||
            (node->type == XML_ENTITY_NODE) ||
            (node->type == XML_ENTITY_DECL))
            return (NULL);
        if (node->type == XML_ELEMENT_NODE) {
            cur = node->nsDef;
            while (cur != NULL) {
                if ((cur->href != NULL) && (href != NULL) &&
                    (xmlStrEqual(cur->href, href))) {
		    if (((!is_attr) || (cur->prefix != NULL)) &&
		        (xmlNsInScope(doc, orig, node, cur->prefix) == 1))
			return (cur);
                }
                cur = cur->next;
            }
            if (orig != node) {
                cur = node->ns;
                if (cur != NULL) {
                    if ((cur->href != NULL) && (href != NULL) &&
                        (xmlStrEqual(cur->href, href))) {
			if (((!is_attr) || (cur->prefix != NULL)) &&
		            (xmlNsInScope(doc, orig, node, cur->prefix) == 1))
			    return (cur);
                    }
                }
            }
        }
        node = node->parent;
    }
    return (NULL);
}

static xmlNsPtr
xmlNewReconciliedNs(xmlDocPtr doc, xmlNodePtr tree, xmlNsPtr ns) {
    xmlNsPtr def;
    xmlChar prefix[50];
    int counter = 1;

    if (tree == NULL) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewReconciliedNs : tree == NULL\n");
#endif
	return(NULL);
    }
    if ((ns == NULL) || (ns->type != XML_NAMESPACE_DECL)) {
#ifdef DEBUG_TREE
        xmlGenericError(xmlGenericErrorContext,
		"xmlNewReconciliedNs : ns == NULL\n");
#endif
	return(NULL);
    }
    def = xmlSearchNsByHref(doc, tree, ns->href);
    if (def != NULL)
        return(def);

    if (ns->prefix == NULL)
	snprintf((char *) prefix, sizeof(prefix), "default");
    else
	snprintf((char *) prefix, sizeof(prefix), "%.20s", (char *)ns->prefix);

    def = xmlSearchNs(doc, tree, prefix);
    while (def != NULL) {
        if (counter > 1000) return(NULL);
	if (ns->prefix == NULL)
	    snprintf((char *) prefix, sizeof(prefix), "default%d", counter++);
	else
	    snprintf((char *) prefix, sizeof(prefix), "%.20s%d",
		(char *)ns->prefix, counter++);
	def = xmlSearchNs(doc, tree, prefix);
    }

    def = xmlNewNs(tree, ns->href, prefix);
    return(def);
}

#ifdef LIBXML_TREE_ENABLED
int
xmlReconciliateNs(xmlDocPtr doc, xmlNodePtr tree) {
    xmlNsPtr *oldNs = NULL;
    xmlNsPtr *newNs = NULL;
    int sizeCache = 0;
    int nbCache = 0;

    xmlNsPtr n;
    xmlNodePtr node = tree;
    xmlAttrPtr attr;
    int ret = 0, i;

    if ((node == NULL) || (node->type != XML_ELEMENT_NODE)) return(-1);
    if ((doc == NULL) || (doc->type != XML_DOCUMENT_NODE)) return(-1);
    if (node->doc != doc) return(-1);
    while (node != NULL) {
	if (node->ns != NULL) {
	    if (sizeCache == 0) {
		sizeCache = 10;
		oldNs = (xmlNsPtr *) xmlMalloc(sizeCache *
					       sizeof(xmlNsPtr));
		if (oldNs == NULL) {
		    xmlTreeErrMemory("fixing namespaces");
		    return(-1);
		}
		newNs = (xmlNsPtr *) xmlMalloc(sizeCache *
					       sizeof(xmlNsPtr));
		if (newNs == NULL) {
		    xmlTreeErrMemory("fixing namespaces");
		    xmlFree(oldNs);
		    return(-1);
		}
	    }
	    for (i = 0;i < nbCache;i++) {
	        if (oldNs[i] == node->ns) {
		    node->ns = newNs[i];
		    break;
		}
	    }
	    if (i == nbCache) {
		n = xmlNewReconciliedNs(doc, tree, node->ns);
		if (n != NULL) { 
		    if (sizeCache <= nbCache) {
		        sizeCache *= 2;
			oldNs = (xmlNsPtr *) xmlRealloc(oldNs, sizeCache *
			                               sizeof(xmlNsPtr));
		        if (oldNs == NULL) {
			    xmlTreeErrMemory("fixing namespaces");
			    xmlFree(newNs);
			    return(-1);
			}
			newNs = (xmlNsPtr *) xmlRealloc(newNs, sizeCache *
			                               sizeof(xmlNsPtr));
		        if (newNs == NULL) {
			    xmlTreeErrMemory("fixing namespaces");
			    xmlFree(oldNs);
			    return(-1);
			}
		    }
		    newNs[nbCache] = n;
		    oldNs[nbCache++] = node->ns;
		    node->ns = n;
                }
	    }
	}
	if (node->type == XML_ELEMENT_NODE) {
	    attr = node->properties;
	    while (attr != NULL) {
		if (attr->ns != NULL) {
		    if (sizeCache == 0) {
			sizeCache = 10;
			oldNs = (xmlNsPtr *) xmlMalloc(sizeCache *
						       sizeof(xmlNsPtr));
			if (oldNs == NULL) {
			    xmlTreeErrMemory("fixing namespaces");
			    return(-1);
			}
			newNs = (xmlNsPtr *) xmlMalloc(sizeCache *
						       sizeof(xmlNsPtr));
			if (newNs == NULL) {
			    xmlTreeErrMemory("fixing namespaces");
			    xmlFree(oldNs);
			    return(-1);
			}
		    }
		    for (i = 0;i < nbCache;i++) {
			if (oldNs[i] == attr->ns) {
			    attr->ns = newNs[i];
			    break;
			}
		    }
		    if (i == nbCache) {
			n = xmlNewReconciliedNs(doc, tree, attr->ns);
			if (n != NULL) { 
			    if (sizeCache <= nbCache) {
				sizeCache *= 2;
				oldNs = (xmlNsPtr *) xmlRealloc(oldNs,
				           sizeCache * sizeof(xmlNsPtr));
				if (oldNs == NULL) {
				    xmlTreeErrMemory("fixing namespaces");
				    xmlFree(newNs);
				    return(-1);
				}
				newNs = (xmlNsPtr *) xmlRealloc(newNs,
				           sizeCache * sizeof(xmlNsPtr));
				if (newNs == NULL) {
				    xmlTreeErrMemory("fixing namespaces");
				    xmlFree(oldNs);
				    return(-1);
				}
			    }
			    newNs[nbCache] = n;
			    oldNs[nbCache++] = attr->ns;
			    attr->ns = n;
			}
		    }
		}
		attr = attr->next;
	    }
	}

        if ((node->children != NULL) && (node->type != XML_ENTITY_REF_NODE)) {
	    
	    node = node->children;
	} else if ((node != tree) && (node->next != NULL)) {
	    
	    node = node->next;
	} else if (node != tree) {
	    
	    while (node != tree) {
	        if (node->parent != NULL)
		    node = node->parent;
		if ((node != tree) && (node->next != NULL)) {
		    node = node->next;
		    break;
		}
		if (node->parent == NULL) {
		    node = NULL;
		    break;
		}
	    }
	    
	    if (node == tree)
	        node = NULL;
	} else
	    break;
    }
    if (oldNs != NULL)
	xmlFree(oldNs);
    if (newNs != NULL)
	xmlFree(newNs);
    return(ret);
}
#endif 

static xmlAttrPtr
xmlGetPropNodeInternal(xmlNodePtr node, const xmlChar *name,
		       const xmlChar *nsName, int useDTD)
{
    xmlAttrPtr prop;

    if ((node == NULL) || (node->type != XML_ELEMENT_NODE) || (name == NULL))
	return(NULL);

    if (node->properties != NULL) {
	prop = node->properties;
	if (nsName == NULL) {
	    do {
		if ((prop->ns == NULL) && xmlStrEqual(prop->name, name)) {
		    return(prop);
		}
		prop = prop->next;
	    } while (prop != NULL);
	} else {
	    do {
		if ((prop->ns != NULL) && xmlStrEqual(prop->name, name) &&
		    ((prop->ns->href == nsName) ||
		     xmlStrEqual(prop->ns->href, nsName)))
		{
		    return(prop);
		}
		prop = prop->next;
	    } while (prop != NULL);
	}
    }

#ifdef LIBXML_TREE_ENABLED
    if (! useDTD)
	return(NULL);
    if ((node->doc != NULL) && (node->doc->intSubset != NULL)) {
	xmlDocPtr doc = node->doc;
	xmlAttributePtr attrDecl = NULL;
	xmlChar *elemQName, *tmpstr = NULL;

	if ((node->ns != NULL) && (node->ns->prefix != NULL)) {
	    tmpstr = xmlStrdup(node->ns->prefix);
	    tmpstr = xmlStrcat(tmpstr, BAD_CAST ":");
	    tmpstr = xmlStrcat(tmpstr, node->name);
	    if (tmpstr == NULL)
		return(NULL);
	    elemQName = tmpstr;
	} else
	    elemQName = (xmlChar *) node->name;
	if (nsName == NULL) {
	    attrDecl = xmlGetDtdQAttrDesc(doc->intSubset,
		elemQName, name, NULL);
	    if ((attrDecl == NULL) && (doc->extSubset != NULL)) {
		attrDecl = xmlGetDtdQAttrDesc(doc->extSubset,
		    elemQName, name, NULL);
	    }
	} else {
	    xmlNsPtr *nsList, *cur;

	    nsList = xmlGetNsList(node->doc, node);
	    if (nsList == NULL) {
		if (tmpstr != NULL)
		    xmlFree(tmpstr);
		return(NULL);
	    }
	    cur = nsList;
	    while (*cur != NULL) {
		if (xmlStrEqual((*cur)->href, nsName)) {
		    attrDecl = xmlGetDtdQAttrDesc(doc->intSubset, elemQName,
			name, (*cur)->prefix);
		    if (attrDecl)
			break;
		    if (doc->extSubset != NULL) {
			attrDecl = xmlGetDtdQAttrDesc(doc->extSubset, elemQName,
			    name, (*cur)->prefix);
			if (attrDecl)
			    break;
		    }
		}
		cur++;
	    }
	    xmlFree(nsList);
	}
	if (tmpstr != NULL)
	    xmlFree(tmpstr);
	if ((attrDecl != NULL) && (attrDecl->defaultValue != NULL))
	    return((xmlAttrPtr) attrDecl);
    }
#endif 
    return(NULL);
}

static xmlChar*
xmlGetPropNodeValueInternal(xmlAttrPtr prop)
{
    if (prop == NULL)
	return(NULL);
    if (prop->type == XML_ATTRIBUTE_NODE) {
	if (prop->children != NULL) {
	    if ((prop->children->next == NULL) &&
		((prop->children->type == XML_TEXT_NODE) ||
		(prop->children->type == XML_CDATA_SECTION_NODE)))
	    {
		return(xmlStrdup(prop->children->content));
	    } else {
		xmlChar *ret;

		ret = xmlNodeListGetString(prop->doc, prop->children, 1);
		if (ret != NULL)
		    return(ret);
	    }
	}
	return(xmlStrdup((xmlChar *)""));
    } else if (prop->type == XML_ATTRIBUTE_DECL) {
	return(xmlStrdup(((xmlAttributePtr)prop)->defaultValue));
    }
    return(NULL);
}

xmlAttrPtr
xmlHasProp(xmlNodePtr node, const xmlChar *name) {
    xmlAttrPtr prop;
    xmlDocPtr doc;

    if ((node == NULL) || (node->type != XML_ELEMENT_NODE) || (name == NULL))
        return(NULL);
    prop = node->properties;
    while (prop != NULL) {
        if (xmlStrEqual(prop->name, name))  {
	    return(prop);
        }
	prop = prop->next;
    }
    if (!xmlCheckDTD) return(NULL);

    doc =  node->doc;
    if (doc != NULL) {
        xmlAttributePtr attrDecl;
        if (doc->intSubset != NULL) {
	    attrDecl = xmlGetDtdAttrDesc(doc->intSubset, node->name, name);
	    if ((attrDecl == NULL) && (doc->extSubset != NULL))
		attrDecl = xmlGetDtdAttrDesc(doc->extSubset, node->name, name);
            if ((attrDecl != NULL) && (attrDecl->defaultValue != NULL))
		return((xmlAttrPtr) attrDecl);
	}
    }
    return(NULL);
}

xmlAttrPtr
xmlHasNsProp(xmlNodePtr node, const xmlChar *name, const xmlChar *nameSpace) {

    return(xmlGetPropNodeInternal(node, name, nameSpace, xmlCheckDTD));
}

xmlChar *
xmlGetProp(xmlNodePtr node, const xmlChar *name) {
    xmlAttrPtr prop;

    prop = xmlHasProp(node, name);
    if (prop == NULL)
	return(NULL);
    return(xmlGetPropNodeValueInternal(prop));
}

xmlChar *
xmlGetNoNsProp(xmlNodePtr node, const xmlChar *name) {
    xmlAttrPtr prop;

    prop = xmlGetPropNodeInternal(node, name, NULL, xmlCheckDTD);
    if (prop == NULL)
	return(NULL);
    return(xmlGetPropNodeValueInternal(prop));
}

xmlChar *
xmlGetNsProp(xmlNodePtr node, const xmlChar *name, const xmlChar *nameSpace) {
    xmlAttrPtr prop;

    prop = xmlGetPropNodeInternal(node, name, nameSpace, xmlCheckDTD);
    if (prop == NULL)
	return(NULL);
    return(xmlGetPropNodeValueInternal(prop));
}

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
int
xmlUnsetProp(xmlNodePtr node, const xmlChar *name) {
    xmlAttrPtr prop;

    prop = xmlGetPropNodeInternal(node, name, NULL, 0);
    if (prop == NULL)
	return(-1);
    xmlUnlinkNode((xmlNodePtr) prop);
    xmlFreeProp(prop);
    return(0);
}

int
xmlUnsetNsProp(xmlNodePtr node, xmlNsPtr ns, const xmlChar *name) {
    xmlAttrPtr prop;

    prop = xmlGetPropNodeInternal(node, name, (ns != NULL) ? ns->href : NULL, 0);
    if (prop == NULL)
	return(-1);
    xmlUnlinkNode((xmlNodePtr) prop);
    xmlFreeProp(prop);
    return(0);
}
#endif

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XINCLUDE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED) || defined(LIBXML_HTML_ENABLED)
xmlAttrPtr
xmlSetProp(xmlNodePtr node, const xmlChar *name, const xmlChar *value) {
    int len;
    const xmlChar *nqname;

    if ((node == NULL) || (name == NULL) || (node->type != XML_ELEMENT_NODE))
	return(NULL);

    nqname = xmlSplitQName3(name, &len);
    if (nqname != NULL) {
        xmlNsPtr ns;
	xmlChar *prefix = xmlStrndup(name, len);
	ns = xmlSearchNs(node->doc, node, prefix);
	if (prefix != NULL)
	    xmlFree(prefix);
	if (ns != NULL)
	    return(xmlSetNsProp(node, ns, nqname, value));
    }
    return(xmlSetNsProp(node, NULL, name, value));
}

xmlAttrPtr
xmlSetNsProp(xmlNodePtr node, xmlNsPtr ns, const xmlChar *name,
	     const xmlChar *value)
{
    xmlAttrPtr prop;

    if (ns && (ns->href == NULL))
	return(NULL);
    prop = xmlGetPropNodeInternal(node, name, (ns != NULL) ? ns->href : NULL, 0);
    if (prop != NULL) {
	if (prop->atype == XML_ATTRIBUTE_ID) {
	    xmlRemoveID(node->doc, prop);
	    prop->atype = XML_ATTRIBUTE_ID;
	}
	if (prop->children != NULL)
	    xmlFreeNodeList(prop->children);
	prop->children = NULL;
	prop->last = NULL;
	prop->ns = ns;
	if (value != NULL) {
	    xmlNodePtr tmp;

	    if(!xmlCheckUTF8(value)) {
	        xmlTreeErr(XML_TREE_NOT_UTF8, (xmlNodePtr) node->doc,
	                   NULL);
                if (node->doc != NULL)
                    node->doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
	    }
	    prop->children = xmlNewDocText(node->doc, value);
	    prop->last = NULL;
	    tmp = prop->children;
	    while (tmp != NULL) {
		tmp->parent = (xmlNodePtr) prop;
		if (tmp->next == NULL)
		    prop->last = tmp;
		tmp = tmp->next;
	    }
	}
	if (prop->atype == XML_ATTRIBUTE_ID)
	    xmlAddID(NULL, node->doc, value, prop);
	return(prop);
    }
    return(xmlNewPropInternal(node, ns, name, value, 0));
}

#endif 

int
xmlNodeIsText(xmlNodePtr node) {
    if (node == NULL) return(0);

    if (node->type == XML_TEXT_NODE) return(1);
    return(0);
}

int
xmlIsBlankNode(xmlNodePtr node) {
    const xmlChar *cur;
    if (node == NULL) return(0);

    if ((node->type != XML_TEXT_NODE) &&
        (node->type != XML_CDATA_SECTION_NODE))
	return(0);
    if (node->content == NULL) return(1);
    cur = node->content;
    while (*cur != 0) {
	if (!IS_BLANK_CH(*cur)) return(0);
	cur++;
    }

    return(1);
}


int
xmlTextConcat(xmlNodePtr node, const xmlChar *content, int len) {
    if (node == NULL) return(-1);

    if ((node->type != XML_TEXT_NODE) &&
        (node->type != XML_CDATA_SECTION_NODE) &&
	(node->type != XML_COMMENT_NODE) &&
	(node->type != XML_PI_NODE)) {
#ifdef DEBUG_TREE
	xmlGenericError(xmlGenericErrorContext,
		"xmlTextConcat: node is not text nor CDATA\n");
#endif
        return(-1);
    }
    
    if ((node->content == (xmlChar *) &(node->properties)) ||
        ((node->doc != NULL) && (node->doc->dict != NULL) &&
		xmlDictOwns(node->doc->dict, node->content))) {
	node->content = xmlStrncatNew(node->content, content, len);
    } else {
        node->content = xmlStrncat(node->content, content, len);
    }
    node->properties = NULL;
    if (node->content == NULL)
        return(-1);
    return(0);
}


xmlBufferPtr
xmlBufferCreate(void) {
    xmlBufferPtr ret;

    ret = (xmlBufferPtr) xmlMalloc(sizeof(xmlBuffer));
    if (ret == NULL) {
	xmlTreeErrMemory("creating buffer");
        return(NULL);
    }
    ret->use = 0;
    ret->size = xmlDefaultBufferSize;
    ret->alloc = xmlBufferAllocScheme;
    ret->content = (xmlChar *) xmlMallocAtomic(ret->size * sizeof(xmlChar));
    if (ret->content == NULL) {
	xmlTreeErrMemory("creating buffer");
	xmlFree(ret);
        return(NULL);
    }
    ret->content[0] = 0;
    ret->contentIO = NULL;
    return(ret);
}

xmlBufferPtr
xmlBufferCreateSize(size_t size) {
    xmlBufferPtr ret;

    ret = (xmlBufferPtr) xmlMalloc(sizeof(xmlBuffer));
    if (ret == NULL) {
	xmlTreeErrMemory("creating buffer");
        return(NULL);
    }
    ret->use = 0;
    ret->alloc = xmlBufferAllocScheme;
    ret->size = (size ? size+2 : 0);         
    if (ret->size){
        ret->content = (xmlChar *) xmlMallocAtomic(ret->size * sizeof(xmlChar));
        if (ret->content == NULL) {
	    xmlTreeErrMemory("creating buffer");
            xmlFree(ret);
            return(NULL);
        }
        ret->content[0] = 0;
    } else
	ret->content = NULL;
    ret->contentIO = NULL;
    return(ret);
}

xmlBufferPtr
xmlBufferCreateStatic(void *mem, size_t size) {
    xmlBufferPtr ret;

    if ((mem == NULL) || (size == 0))
        return(NULL);

    ret = (xmlBufferPtr) xmlMalloc(sizeof(xmlBuffer));
    if (ret == NULL) {
	xmlTreeErrMemory("creating buffer");
        return(NULL);
    }
    ret->use = size;
    ret->size = size;
    ret->alloc = XML_BUFFER_ALLOC_IMMUTABLE;
    ret->content = (xmlChar *) mem;
    return(ret);
}

void
xmlBufferSetAllocationScheme(xmlBufferPtr buf,
                             xmlBufferAllocationScheme scheme) {
    if (buf == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferSetAllocationScheme: buf == NULL\n");
#endif
        return;
    }
    if ((buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) ||
        (buf->alloc == XML_BUFFER_ALLOC_IO)) return;
    if ((scheme == XML_BUFFER_ALLOC_DOUBLEIT) ||
        (scheme == XML_BUFFER_ALLOC_EXACT) ||
        (scheme == XML_BUFFER_ALLOC_IMMUTABLE))
	buf->alloc = scheme;
}

void
xmlBufferFree(xmlBufferPtr buf) {
    if (buf == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferFree: buf == NULL\n");
#endif
	return;
    }

    if ((buf->alloc == XML_BUFFER_ALLOC_IO) &&
        (buf->contentIO != NULL)) {
        xmlFree(buf->contentIO);
    } else if ((buf->content != NULL) &&
        (buf->alloc != XML_BUFFER_ALLOC_IMMUTABLE)) {
        xmlFree(buf->content);
    }
    xmlFree(buf);
}

void
xmlBufferEmpty(xmlBufferPtr buf) {
    if (buf == NULL) return;
    if (buf->content == NULL) return;
    buf->use = 0;
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) {
        buf->content = BAD_CAST "";
    } else if ((buf->alloc == XML_BUFFER_ALLOC_IO) &&
               (buf->contentIO != NULL)) {
        size_t start_buf = buf->content - buf->contentIO;

	buf->size += start_buf;
        buf->content = buf->contentIO;
        buf->content[0] = 0;
    } else {
        buf->content[0] = 0;
    }
}

int
xmlBufferShrink(xmlBufferPtr buf, unsigned int len) {
    if (buf == NULL) return(-1);
    if (len == 0) return(0);
    if (len > buf->use) return(-1);

    buf->use -= len;
    if ((buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) ||
        ((buf->alloc == XML_BUFFER_ALLOC_IO) && (buf->contentIO != NULL))) {
        buf->content += len;
	buf->size -= len;

	if ((buf->alloc == XML_BUFFER_ALLOC_IO) && (buf->contentIO != NULL)) {
	    size_t start_buf = buf->content - buf->contentIO;
	    if (start_buf >= buf->size) {
		memmove(buf->contentIO, &buf->content[0], buf->use);
		buf->content = buf->contentIO;
		buf->content[buf->use] = 0;
		buf->size += start_buf;
	    }
	}
    } else {
	memmove(buf->content, &buf->content[len], buf->use);
	buf->content[buf->use] = 0;
    }
    return(len);
}

int
xmlBufferGrow(xmlBufferPtr buf, unsigned int len) {
    int size;
    xmlChar *newbuf;

    if (buf == NULL) return(-1);

    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return(0);
    if (len + buf->use < buf->size) return(0);

#if 1
    if (buf->size > len)
        size = buf->size * 2;
    else
        size = buf->use + len + 100;
#else
    size = buf->use + len + 100;
#endif

    if ((buf->alloc == XML_BUFFER_ALLOC_IO) && (buf->contentIO != NULL)) {
        size_t start_buf = buf->content - buf->contentIO;

	newbuf = (xmlChar *) xmlRealloc(buf->contentIO, start_buf + size);
	if (newbuf == NULL) {
	    xmlTreeErrMemory("growing buffer");
	    return(-1);
	}
	buf->contentIO = newbuf;
	buf->content = newbuf + start_buf;
    } else {
	newbuf = (xmlChar *) xmlRealloc(buf->content, size);
	if (newbuf == NULL) {
	    xmlTreeErrMemory("growing buffer");
	    return(-1);
	}
	buf->content = newbuf;
    }
    buf->size = size;
    return(buf->size - buf->use);
}

/**
 * xmlBufferDump:
 * @file:  the file output
 * @buf:  the buffer to dump
 *
 * Dumps an XML buffer to  a FILE *.
 * Returns the number of #xmlChar written
 */
int
xmlBufferDump(FILE *file, xmlBufferPtr buf) {
    int ret;

    if (buf == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferDump: buf == NULL\n");
#endif
	return(0);
    }
    if (buf->content == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferDump: buf->content == NULL\n");
#endif
	return(0);
    }
    if (file == NULL)
	file = stdout;
    ret = fwrite(buf->content, sizeof(xmlChar), buf->use, file);
    return(ret);
}


const xmlChar *
xmlBufferContent(const xmlBufferPtr buf)
{
    if(!buf)
        return NULL;

    return buf->content;
}


int
xmlBufferLength(const xmlBufferPtr buf)
{
    if(!buf)
        return 0;

    return buf->use;
}

int
xmlBufferResize(xmlBufferPtr buf, unsigned int size)
{
    unsigned int newSize;
    xmlChar* rebuf = NULL;
    size_t start_buf;

    if (buf == NULL)
        return(0);

    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return(0);

    
    if (size < buf->size)
        return 1;

    
    switch (buf->alloc){
	case XML_BUFFER_ALLOC_IO:
	case XML_BUFFER_ALLOC_DOUBLEIT:
	    
	    newSize = (buf->size ? buf->size*2 : size + 10);
	    while (size > newSize) {
	        if (newSize > UINT_MAX / 2) {
	            xmlTreeErrMemory("growing buffer");
	            return 0;
	        }
	        newSize *= 2;
	    }
	    break;
	case XML_BUFFER_ALLOC_EXACT:
	    newSize = size+10;
	    break;
	default:
	    newSize = size+10;
	    break;
    }

    if ((buf->alloc == XML_BUFFER_ALLOC_IO) && (buf->contentIO != NULL)) {
        start_buf = buf->content - buf->contentIO;

        if (start_buf > newSize) {
	    
	    memmove(buf->contentIO, buf->content, buf->use);
	    buf->content = buf->contentIO;
	    buf->content[buf->use] = 0;
	    buf->size += start_buf;
	} else {
	    rebuf = (xmlChar *) xmlRealloc(buf->contentIO, start_buf + newSize);
	    if (rebuf == NULL) {
		xmlTreeErrMemory("growing buffer");
		return 0;
	    }
	    buf->contentIO = rebuf;
	    buf->content = rebuf + start_buf;
	}
    } else {
	if (buf->content == NULL) {
	    rebuf = (xmlChar *) xmlMallocAtomic(newSize);
	} else if (buf->size - buf->use < 100) {
	    rebuf = (xmlChar *) xmlRealloc(buf->content, newSize);
        } else {
	    rebuf = (xmlChar *) xmlMallocAtomic(newSize);
	    if (rebuf != NULL) {
		memcpy(rebuf, buf->content, buf->use);
		xmlFree(buf->content);
		rebuf[buf->use] = 0;
	    }
	}
	if (rebuf == NULL) {
	    xmlTreeErrMemory("growing buffer");
	    return 0;
	}
	buf->content = rebuf;
    }
    buf->size = newSize;

    return 1;
}

int
xmlBufferAdd(xmlBufferPtr buf, const xmlChar *str, int len) {
    unsigned int needSize;

    if ((str == NULL) || (buf == NULL)) {
	return -1;
    }
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return -1;
    if (len < -1) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferAdd: len < 0\n");
#endif
	return -1;
    }
    if (len == 0) return 0;

    if (len < 0)
        len = xmlStrlen(str);

    if (len < 0) return -1;
    if (len == 0) return 0;

    needSize = buf->use + len + 2;
    if (needSize > buf->size){
        if (!xmlBufferResize(buf, needSize)){
	    xmlTreeErrMemory("growing buffer");
            return XML_ERR_NO_MEMORY;
        }
    }

    memmove(&buf->content[buf->use], str, len*sizeof(xmlChar));
    buf->use += len;
    buf->content[buf->use] = 0;
    return 0;
}

int
xmlBufferAddHead(xmlBufferPtr buf, const xmlChar *str, int len) {
    unsigned int needSize;

    if (buf == NULL)
        return(-1);
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return -1;
    if (str == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferAddHead: str == NULL\n");
#endif
	return -1;
    }
    if (len < -1) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferAddHead: len < 0\n");
#endif
	return -1;
    }
    if (len == 0) return 0;

    if (len < 0)
        len = xmlStrlen(str);

    if (len <= 0) return -1;

    if ((buf->alloc == XML_BUFFER_ALLOC_IO) && (buf->contentIO != NULL)) {
        size_t start_buf = buf->content - buf->contentIO;

	if (start_buf > (unsigned int) len) {
	    buf->content -= len;
            memmove(&buf->content[0], str, len);
	    buf->use += len;
	    buf->size += len;
	    return(0);
	}
    }
    needSize = buf->use + len + 2;
    if (needSize > buf->size){
        if (!xmlBufferResize(buf, needSize)){
	    xmlTreeErrMemory("growing buffer");
            return XML_ERR_NO_MEMORY;
        }
    }

    memmove(&buf->content[len], &buf->content[0], buf->use);
    memmove(&buf->content[0], str, len);
    buf->use += len;
    buf->content[buf->use] = 0;
    return 0;
}

int
xmlBufferCat(xmlBufferPtr buf, const xmlChar *str) {
    if (buf == NULL)
        return(-1);
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return -1;
    if (str == NULL) return -1;
    return xmlBufferAdd(buf, str, -1);
}

int
xmlBufferCCat(xmlBufferPtr buf, const char *str) {
    const char *cur;

    if (buf == NULL)
        return(-1);
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return -1;
    if (str == NULL) {
#ifdef DEBUG_BUFFER
        xmlGenericError(xmlGenericErrorContext,
		"xmlBufferCCat: str == NULL\n");
#endif
	return -1;
    }
    for (cur = str;*cur != 0;cur++) {
        if (buf->use  + 10 >= buf->size) {
            if (!xmlBufferResize(buf, buf->use+10)){
		xmlTreeErrMemory("growing buffer");
                return XML_ERR_NO_MEMORY;
            }
        }
        buf->content[buf->use++] = *cur;
    }
    buf->content[buf->use] = 0;
    return 0;
}

void
xmlBufferWriteCHAR(xmlBufferPtr buf, const xmlChar *string) {
    if (buf == NULL)
        return;
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return;
    xmlBufferCat(buf, string);
}

void
xmlBufferWriteChar(xmlBufferPtr buf, const char *string) {
    if (buf == NULL)
        return;
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return;
    xmlBufferCCat(buf, string);
}


void
xmlBufferWriteQuotedString(xmlBufferPtr buf, const xmlChar *string) {
    const xmlChar *cur, *base;
    if (buf == NULL)
        return;
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return;
    if (xmlStrchr(string, '\"')) {
        if (xmlStrchr(string, '\'')) {
#ifdef DEBUG_BUFFER
	    xmlGenericError(xmlGenericErrorContext,
 "xmlBufferWriteQuotedString: string contains quote and double-quotes !\n");
#endif
	    xmlBufferCCat(buf, "\"");
            base = cur = string;
            while(*cur != 0){
                if(*cur == '"'){
                    if (base != cur)
                        xmlBufferAdd(buf, base, cur - base);
                    xmlBufferAdd(buf, BAD_CAST "&quot;", 6);
                    cur++;
                    base = cur;
                }
                else {
                    cur++;
                }
            }
            if (base != cur)
                xmlBufferAdd(buf, base, cur - base);
	    xmlBufferCCat(buf, "\"");
	}
        else{
	    xmlBufferCCat(buf, "\'");
            xmlBufferCat(buf, string);
	    xmlBufferCCat(buf, "\'");
        }
    } else {
        xmlBufferCCat(buf, "\"");
        xmlBufferCat(buf, string);
        xmlBufferCCat(buf, "\"");
    }
}


int
xmlGetDocCompressMode (xmlDocPtr doc) {
    if (doc == NULL) return(-1);
    return(doc->compression);
}

void
xmlSetDocCompressMode (xmlDocPtr doc, int mode) {
    if (doc == NULL) return;
    if (mode < 0) doc->compression = 0;
    else if (mode > 9) doc->compression = 9;
    else doc->compression = mode;
}

int
xmlGetCompressMode(void)
{
    return (xmlCompressMode);
}

void
xmlSetCompressMode(int mode) {
    if (mode < 0) xmlCompressMode = 0;
    else if (mode > 9) xmlCompressMode = 9;
    else xmlCompressMode = mode;
}

#define XML_TREE_NSMAP_PARENT -1
#define XML_TREE_NSMAP_XML -2
#define XML_TREE_NSMAP_DOC -3
#define XML_TREE_NSMAP_CUSTOM -4

typedef struct xmlNsMapItem *xmlNsMapItemPtr;
struct xmlNsMapItem {
    xmlNsMapItemPtr next;
    xmlNsMapItemPtr prev;
    xmlNsPtr oldNs; 
    xmlNsPtr newNs; 
    int shadowDepth; 
    int depth;
};

typedef struct xmlNsMap *xmlNsMapPtr;
struct xmlNsMap {
    xmlNsMapItemPtr first;
    xmlNsMapItemPtr last;
    xmlNsMapItemPtr pool;
};

#define XML_NSMAP_NOTEMPTY(m) (((m) != NULL) && ((m)->first != NULL))
#define XML_NSMAP_FOREACH(m, i) for (i = (m)->first; i != NULL; i = (i)->next)
#define XML_NSMAP_POP(m, i) \
    i = (m)->last; \
    (m)->last = (i)->prev; \
    if ((m)->last == NULL) \
	(m)->first = NULL; \
    else \
	(m)->last->next = NULL; \
    (i)->next = (m)->pool; \
    (m)->pool = i;

static void
xmlDOMWrapNsMapFree(xmlNsMapPtr nsmap)
{
    xmlNsMapItemPtr cur, tmp;

    if (nsmap == NULL)
	return;
    cur = nsmap->pool;
    while (cur != NULL) {
	tmp = cur;
	cur = cur->next;
	xmlFree(tmp);
    }
    cur = nsmap->first;
    while (cur != NULL) {
	tmp = cur;
	cur = cur->next;
	xmlFree(tmp);
    }
    xmlFree(nsmap);
}

static xmlNsMapItemPtr
xmlDOMWrapNsMapAddItem(xmlNsMapPtr *nsmap, int position,
		       xmlNsPtr oldNs, xmlNsPtr newNs, int depth)
{
    xmlNsMapItemPtr ret;
    xmlNsMapPtr map;

    if (nsmap == NULL)
	return(NULL);
    if ((position != -1) && (position != 0))
	return(NULL);
    map = *nsmap;

    if (map == NULL) {
	map = (xmlNsMapPtr) xmlMalloc(sizeof(struct xmlNsMap));
	if (map == NULL) {
	    xmlTreeErrMemory("allocating namespace map");
	    return (NULL);
	}
	memset(map, 0, sizeof(struct xmlNsMap));
	*nsmap = map;
    }

    if (map->pool != NULL) {
	ret = map->pool;
	map->pool = ret->next;
	memset(ret, 0, sizeof(struct xmlNsMapItem));
    } else {
	ret = (xmlNsMapItemPtr) xmlMalloc(sizeof(struct xmlNsMapItem));
	if (ret == NULL) {
	    xmlTreeErrMemory("allocating namespace map item");
	    return (NULL);
	}
	memset(ret, 0, sizeof(struct xmlNsMapItem));
    }

    if (map->first == NULL) {
	map->first = ret;
	map->last = ret;
    } else if (position == -1) {
	ret->prev = map->last;
	map->last->next = ret;
	map->last = ret;
    } else if (position == 0) {
	map->first->prev = ret;
	ret->next = map->first;
	map->first = ret;
    } else
	return(NULL);

    ret->oldNs = oldNs;
    ret->newNs = newNs;
    ret->shadowDepth = -1;
    ret->depth = depth;
    return (ret);
}

static xmlNsPtr
xmlDOMWrapStoreNs(xmlDocPtr doc,
		   const xmlChar *nsName,
		   const xmlChar *prefix)
{
    xmlNsPtr ns;

    if (doc == NULL)
	return (NULL);
    ns = xmlTreeEnsureXMLDecl(doc);
    if (ns == NULL)
	return (NULL);
    if (ns->next != NULL) {
	
	ns = ns->next;
	while (ns != NULL) {
	    if (((ns->prefix == prefix) ||
		xmlStrEqual(ns->prefix, prefix)) &&
		xmlStrEqual(ns->href, nsName)) {
		return (ns);
	    }
	    if (ns->next == NULL)
		break;
	    ns = ns->next;
	}
    }
    
    if (ns != NULL) {
        ns->next = xmlNewNs(NULL, nsName, prefix);
        return (ns->next);
    }
    return(NULL);
}

xmlDOMWrapCtxtPtr
xmlDOMWrapNewCtxt(void)
{
    xmlDOMWrapCtxtPtr ret;

    ret = xmlMalloc(sizeof(xmlDOMWrapCtxt));
    if (ret == NULL) {
	xmlTreeErrMemory("allocating DOM-wrapper context");
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlDOMWrapCtxt));
    return (ret);
}

void
xmlDOMWrapFreeCtxt(xmlDOMWrapCtxtPtr ctxt)
{
    if (ctxt == NULL)
	return;
    if (ctxt->namespaceMap != NULL)
	xmlDOMWrapNsMapFree((xmlNsMapPtr) ctxt->namespaceMap);
    xmlFree(ctxt);
}

static xmlNsPtr
xmlTreeNSListLookupByPrefix(xmlNsPtr nsList, const xmlChar *prefix)
{
    if (nsList == NULL)
	return (NULL);
    {
	xmlNsPtr ns;
	ns = nsList;
	do {
	    if ((prefix == ns->prefix) ||
		xmlStrEqual(prefix, ns->prefix)) {
		return (ns);
	    }
	    ns = ns->next;
	} while (ns != NULL);
    }
    return (NULL);
}

static int
xmlDOMWrapNSNormGatherInScopeNs(xmlNsMapPtr *map,
				xmlNodePtr node)
{
    xmlNodePtr cur;
    xmlNsPtr ns;
    xmlNsMapItemPtr mi;
    int shadowed;

    if ((map == NULL) || (*map != NULL))
	return (-1);
    cur = node;
    while ((cur != NULL) && (cur != (xmlNodePtr) cur->doc)) {
	if (cur->type == XML_ELEMENT_NODE) {
	    if (cur->nsDef != NULL) {
		ns = cur->nsDef;
		do {
		    shadowed = 0;
		    if (XML_NSMAP_NOTEMPTY(*map)) {
			XML_NSMAP_FOREACH(*map, mi) {
			    if ((ns->prefix == mi->newNs->prefix) ||
				xmlStrEqual(ns->prefix, mi->newNs->prefix)) {
				shadowed = 1;
				break;
			    }
			}
		    }
		    mi = xmlDOMWrapNsMapAddItem(map, 0, NULL,
			ns, XML_TREE_NSMAP_PARENT);
		    if (mi == NULL)
			return (-1);
		    if (shadowed)
			mi->shadowDepth = 0;
		    ns = ns->next;
		} while (ns != NULL);
	    }
	}
	cur = cur->parent;
    }
    return (0);
}

#define XML_TREE_ADOPT_STR(str) \
    if (adoptStr && (str != NULL)) { \
	if (destDoc->dict) { \
	    const xmlChar *old = str;	\
	    str = xmlDictLookup(destDoc->dict, str, -1); \
	    if ((sourceDoc == NULL) || (sourceDoc->dict == NULL) || \
	        (!xmlDictOwns(sourceDoc->dict, old))) \
		xmlFree((char *)old); \
	} else if ((sourceDoc) && (sourceDoc->dict) && \
	    xmlDictOwns(sourceDoc->dict, str)) { \
	    str = BAD_CAST xmlStrdup(str); \
	} \
    }

#define XML_TREE_ADOPT_STR_2(str) \
    if (adoptStr && (str != NULL) && (sourceDoc != NULL) && \
	(sourceDoc->dict != NULL) && \
	xmlDictOwns(sourceDoc->dict, cur->content)) { \
	if (destDoc->dict) \
	    cur->content = (xmlChar *) \
		xmlDictLookup(destDoc->dict, cur->content, -1); \
	else \
	    cur->content = xmlStrdup(BAD_CAST cur->content); \
    }

static int
xmlDOMWrapNSNormAddNsMapItem2(xmlNsPtr **list, int *size, int *number,
			xmlNsPtr oldNs, xmlNsPtr newNs)
{
    if (*list == NULL) {
	*list = (xmlNsPtr *) xmlMalloc(6 * sizeof(xmlNsPtr));
	if (*list == NULL) {
	    xmlTreeErrMemory("alloc ns map item");
	    return(-1);
	}
	*size = 3;
	*number = 0;
    } else if ((*number) >= (*size)) {
	*size *= 2;
	*list = (xmlNsPtr *) xmlRealloc(*list,
	    (*size) * 2 * sizeof(xmlNsPtr));
	if (*list == NULL) {
	    xmlTreeErrMemory("realloc ns map item");
	    return(-1);
	}
    }
    (*list)[2 * (*number)] = oldNs;
    (*list)[2 * (*number) +1] = newNs;
    (*number)++;
    return (0);
}

int
xmlDOMWrapRemoveNode(xmlDOMWrapCtxtPtr ctxt, xmlDocPtr doc,
		     xmlNodePtr node, int options ATTRIBUTE_UNUSED)
{
    xmlNsPtr *list = NULL;
    int sizeList, nbList, i, j;
    xmlNsPtr ns;

    if ((node == NULL) || (doc == NULL) || (node->doc != doc))
	return (-1);

    
    if (node->parent == NULL)
	return (0);

    switch (node->type) {
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_ENTITY_REF_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	    xmlUnlinkNode(node);
	    return (0);
	case XML_ELEMENT_NODE:
	case XML_ATTRIBUTE_NODE:
	    break;
	default:
	    return (1);
    }
    xmlUnlinkNode(node);
    do {
	switch (node->type) {
	    case XML_ELEMENT_NODE:
		if ((ctxt == NULL) && (node->nsDef != NULL)) {
		    ns = node->nsDef;
		    do {
			if (xmlDOMWrapNSNormAddNsMapItem2(&list, &sizeList,
			    &nbList, ns, ns) == -1)
			    goto internal_error;
			ns = ns->next;
		    } while (ns != NULL);
		}
		
	    case XML_ATTRIBUTE_NODE:
		if (node->ns != NULL) {
		    if (list != NULL) {
			for (i = 0, j = 0; i < nbList; i++, j += 2) {
			    if (node->ns == list[j]) {
				node->ns = list[++j];
				goto next_node;
			    }
			}
		    }
		    ns = NULL;
		    if (ctxt != NULL) {
		    } else {
			ns = xmlDOMWrapStoreNs(doc, node->ns->href,
			    node->ns->prefix);
			if (ns == NULL)
			    goto internal_error;
		    }
		    if (ns != NULL) {
			if (xmlDOMWrapNSNormAddNsMapItem2(&list, &sizeList,
			    &nbList, node->ns, ns) == -1)
			    goto internal_error;
		    }
		    node->ns = ns;
		}
		if ((node->type == XML_ELEMENT_NODE) &&
		    (node->properties != NULL)) {
		    node = (xmlNodePtr) node->properties;
		    continue;
		}
		break;
	    default:
		goto next_sibling;
	}
next_node:
	if ((node->type == XML_ELEMENT_NODE) &&
	    (node->children != NULL)) {
	    node = node->children;
	    continue;
	}
next_sibling:
	if (node == NULL)
	    break;
	if (node->next != NULL)
	    node = node->next;
	else {
	    node = node->parent;
	    goto next_sibling;
	}
    } while (node != NULL);

    if (list != NULL)
	xmlFree(list);
    return (0);

internal_error:
    if (list != NULL)
	xmlFree(list);
    return (-1);
}

static int
xmlSearchNsByNamespaceStrict(xmlDocPtr doc, xmlNodePtr node,
			     const xmlChar* nsName,
			     xmlNsPtr *retNs, int prefixed)
{
    xmlNodePtr cur, prev = NULL, out = NULL;
    xmlNsPtr ns, prevns;

    if ((doc == NULL) || (nsName == NULL) || (retNs == NULL))
	return (-1);

    *retNs = NULL;
    if (xmlStrEqual(nsName, XML_XML_NAMESPACE)) {
	*retNs = xmlTreeEnsureXMLDecl(doc);
	if (*retNs == NULL)
	    return (-1);
	return (1);
    }
    cur = node;
    do {
	if (cur->type == XML_ELEMENT_NODE) {
	    if (cur->nsDef != NULL) {
		for (ns = cur->nsDef; ns != NULL; ns = ns->next) {
		    if (prefixed && (ns->prefix == NULL))
			continue;
		    if (prev != NULL) {
			prevns = prev->nsDef;
			do {
			    if ((prevns->prefix == ns->prefix) ||
				((prevns->prefix != NULL) &&
				(ns->prefix != NULL) &&
				xmlStrEqual(prevns->prefix, ns->prefix))) {
				break;
			    }
			    prevns = prevns->next;
			} while (prevns != NULL);
			if (prevns != NULL)
			    continue;
		    }
		    if ((nsName == ns->href) ||
			xmlStrEqual(nsName, ns->href)) {
			if (out) {
			    int ret;

			    ret = xmlNsInScope(doc, node, prev, ns->prefix);
			    if (ret < 0)
				return (-1);
			    if (! ret)
				continue;
			}
			*retNs = ns;
			return (1);
		    }
		}
		out = prev;
		prev = cur;
	    }
	} else if ((cur->type == XML_ENTITY_NODE) ||
            (cur->type == XML_ENTITY_DECL))
	    return (0);
	cur = cur->parent;
    } while ((cur != NULL) && (cur->doc != (xmlDocPtr) cur));
    return (0);
}

static int
xmlSearchNsByPrefixStrict(xmlDocPtr doc, xmlNodePtr node,
			  const xmlChar* prefix,
			  xmlNsPtr *retNs)
{
    xmlNodePtr cur;
    xmlNsPtr ns;

    if ((doc == NULL) || (node == NULL))
	return (-1);

    if (retNs)
	*retNs = NULL;
    if (IS_STR_XML(prefix)) {
	if (retNs) {
	    *retNs = xmlTreeEnsureXMLDecl(doc);
	    if (*retNs == NULL)
		return (-1);
	}
	return (1);
    }
    cur = node;
    do {
	if (cur->type == XML_ELEMENT_NODE) {
	    if (cur->nsDef != NULL) {
		ns = cur->nsDef;
		do {
		    if ((prefix == ns->prefix) ||
			xmlStrEqual(prefix, ns->prefix))
		    {
			if (ns->href == NULL)
			    return(0);
			if (retNs)
			    *retNs = ns;
			return (1);
		    }
		    ns = ns->next;
		} while (ns != NULL);
	    }
	} else if ((cur->type == XML_ENTITY_NODE) ||
            (cur->type == XML_ENTITY_DECL))
	    return (0);
	cur = cur->parent;
    } while ((cur != NULL) && (cur->doc != (xmlDocPtr) cur));
    return (0);
}

static xmlNsPtr
xmlDOMWrapNSNormDeclareNsForced(xmlDocPtr doc,
				xmlNodePtr elem,
				const xmlChar *nsName,
				const xmlChar *prefix,
				int checkShadow)
{

    xmlNsPtr ret;
    char buf[50];
    const xmlChar *pref;
    int counter = 0;
    pref = prefix;
    while (1) {
	if ((elem->nsDef != NULL) &&
	    (xmlTreeNSListLookupByPrefix(elem->nsDef, pref) != NULL))
	    goto ns_next_prefix;
	if (checkShadow && elem->parent &&
	    ((xmlNodePtr) elem->parent->doc != elem->parent)) {
	    if (xmlSearchNsByPrefixStrict(doc, elem->parent, pref, NULL) == 1)
		goto ns_next_prefix;
	}
	ret = xmlNewNs(NULL, nsName, pref);
	if (ret == NULL)
	    return (NULL);
	if (elem->nsDef == NULL)
	    elem->nsDef = ret;
	else {
	    xmlNsPtr ns2 = elem->nsDef;
	    while (ns2->next != NULL)
		ns2 = ns2->next;
	    ns2->next = ret;
	}
	return (ret);
ns_next_prefix:
	counter++;
	if (counter > 1000)
	    return (NULL);
	if (prefix == NULL) {
	    snprintf((char *) buf, sizeof(buf),
		"ns_%d", counter);
	} else
	    snprintf((char *) buf, sizeof(buf),
	    "%.30s_%d", (char *)prefix, counter);
	pref = BAD_CAST buf;
    }
}

static int
xmlDOMWrapNSNormAquireNormalizedNs(xmlDocPtr doc,
				   xmlNodePtr elem,
				   xmlNsPtr ns,
				   xmlNsPtr *retNs,
				   xmlNsMapPtr *nsMap,

				   int depth,
				   int ancestorsOnly,
				   int prefixed)
{
    xmlNsMapItemPtr mi;

    if ((doc == NULL) || (ns == NULL) || (retNs == NULL) ||
	(nsMap == NULL))
	return (-1);

    *retNs = NULL;
    if (IS_STR_XML(ns->prefix)) {
	*retNs = xmlTreeEnsureXMLDecl(doc);
	if (*retNs == NULL)
	    return (-1);
	return (0);
    }
    if ((XML_NSMAP_NOTEMPTY(*nsMap)) &&
	(! (ancestorsOnly && (elem == NULL))))
    {
	XML_NSMAP_FOREACH(*nsMap, mi) {
	    if ((mi->depth >= XML_TREE_NSMAP_PARENT) &&
		((! ancestorsOnly) || (mi->depth == XML_TREE_NSMAP_PARENT)) &&
		
		(mi->shadowDepth == -1) &&
		
		((mi->newNs->href != NULL) &&
		(mi->newNs->href[0] != 0)) &&
		
		((! prefixed) || (mi->newNs->prefix != NULL)) &&
		
		((mi->newNs->href == ns->href) ||
		xmlStrEqual(mi->newNs->href, ns->href))) {
		
		mi->oldNs = ns;
		*retNs = mi->newNs;
		return (0);
	    }
	}
    }
    if (elem == NULL) {
	xmlNsPtr tmpns;

	tmpns = xmlDOMWrapStoreNs(doc, ns->href, ns->prefix);
	if (tmpns == NULL)
	    return (-1);
	if (xmlDOMWrapNsMapAddItem(nsMap, -1, ns,
		tmpns, XML_TREE_NSMAP_DOC) == NULL) {
	    xmlFreeNs(tmpns);
	    return (-1);
	}
	*retNs = tmpns;
    } else {
	xmlNsPtr tmpns;

	tmpns = xmlDOMWrapNSNormDeclareNsForced(doc, elem, ns->href,
	    ns->prefix, 0);
	if (tmpns == NULL)
	    return (-1);

	if (*nsMap != NULL) {
	    XML_NSMAP_FOREACH(*nsMap, mi) {
		if ((mi->depth < depth) &&
		    (mi->shadowDepth == -1) &&
		    ((ns->prefix == mi->newNs->prefix) ||
		    xmlStrEqual(ns->prefix, mi->newNs->prefix))) {
		    mi->shadowDepth = depth;
		    break;
		}
	    }
	}
	if (xmlDOMWrapNsMapAddItem(nsMap, -1, ns, tmpns, depth) == NULL) {
	    xmlFreeNs(tmpns);
	    return (-1);
	}
	*retNs = tmpns;
    }
    return (0);
}

typedef enum {
    XML_DOM_RECONNS_REMOVEREDUND = 1<<0
} xmlDOMReconcileNSOptions;


int
xmlDOMWrapReconcileNamespaces(xmlDOMWrapCtxtPtr ctxt ATTRIBUTE_UNUSED,
			      xmlNodePtr elem,
			      int options)
{
    int depth = -1, adoptns = 0, parnsdone = 0;
    xmlNsPtr ns, prevns;
    xmlDocPtr doc;
    xmlNodePtr cur, curElem = NULL;
    xmlNsMapPtr nsMap = NULL;
    xmlNsMapItemPtr  mi;
    
    int ancestorsOnly = 0;
    int optRemoveRedundantNS =
	((xmlDOMReconcileNSOptions) options & XML_DOM_RECONNS_REMOVEREDUND) ? 1 : 0;
    xmlNsPtr *listRedund = NULL;
    int sizeRedund = 0, nbRedund = 0, ret, i, j;

    if ((elem == NULL) || (elem->doc == NULL) ||
	(elem->type != XML_ELEMENT_NODE))
	return (-1);

    doc = elem->doc;
    cur = elem;
    do {
	switch (cur->type) {
	    case XML_ELEMENT_NODE:
		adoptns = 1;
		curElem = cur;
		depth++;
		if (cur->nsDef != NULL) {
		    prevns = NULL;
		    ns = cur->nsDef;
		    while (ns != NULL) {
			if (! parnsdone) {
			    if ((elem->parent) &&
				((xmlNodePtr) elem->parent->doc != elem->parent)) {
				if (xmlDOMWrapNSNormGatherInScopeNs(&nsMap,
				    elem->parent) == -1)
				    goto internal_error;
			    }
			    parnsdone = 1;
			}

			if (optRemoveRedundantNS && XML_NSMAP_NOTEMPTY(nsMap)) {
			    XML_NSMAP_FOREACH(nsMap, mi) {
				if ((mi->depth >= XML_TREE_NSMAP_PARENT) &&
				    (mi->shadowDepth == -1) &&
				    ((ns->prefix == mi->newNs->prefix) ||
				      xmlStrEqual(ns->prefix, mi->newNs->prefix)) &&
				    ((ns->href == mi->newNs->href) ||
				      xmlStrEqual(ns->href, mi->newNs->href)))
				{
				    if (xmlDOMWrapNSNormAddNsMapItem2(&listRedund,
					&sizeRedund, &nbRedund, ns, mi->newNs) == -1)
					goto internal_error;
				    if (prevns)
					prevns->next = ns->next;
				    else
					cur->nsDef = ns->next;
				    goto next_ns_decl;
				}
			    }
			}

			if ((cur->ns != NULL) && adoptns && (cur->ns == ns))
			    adoptns = 0;
			if (XML_NSMAP_NOTEMPTY(nsMap)) {
			    XML_NSMAP_FOREACH(nsMap, mi) {
				if ((mi->depth >= XML_TREE_NSMAP_PARENT) &&
				    (mi->shadowDepth == -1) &&
				    ((ns->prefix == mi->newNs->prefix) ||
				    xmlStrEqual(ns->prefix, mi->newNs->prefix))) {

				    mi->shadowDepth = depth;
				}
			    }
			}
			if (xmlDOMWrapNsMapAddItem(&nsMap, -1, ns, ns,
			    depth) == NULL)
			    goto internal_error;

			prevns = ns;
next_ns_decl:
			ns = ns->next;
		    }
		}
		if (! adoptns)
		    goto ns_end;
		
	    case XML_ATTRIBUTE_NODE:
		
		if (cur->ns == NULL)
		    goto ns_end;

		if (! parnsdone) {
		    if ((elem->parent) &&
			((xmlNodePtr) elem->parent->doc != elem->parent)) {
			if (xmlDOMWrapNSNormGatherInScopeNs(&nsMap,
				elem->parent) == -1)
			    goto internal_error;
		    }
		    parnsdone = 1;
		}
		if (listRedund) {
		   for (i = 0, j = 0; i < nbRedund; i++, j += 2) {
		       if (cur->ns == listRedund[j]) {
			   cur->ns = listRedund[++j];
			   break;
		       }
		   }
		}
		if (XML_NSMAP_NOTEMPTY(nsMap)) {
		    XML_NSMAP_FOREACH(nsMap, mi) {
			if ((mi->shadowDepth == -1) &&
			    (cur->ns == mi->oldNs)) {

			    cur->ns = mi->newNs;
			    goto ns_end;
			}
		    }
		}
		if (xmlDOMWrapNSNormAquireNormalizedNs(doc, curElem,
			cur->ns, &ns,
			&nsMap, depth,
			ancestorsOnly,
			(cur->type == XML_ATTRIBUTE_NODE) ? 1 : 0) == -1)
		    goto internal_error;
		cur->ns = ns;

ns_end:
		if ((cur->type == XML_ELEMENT_NODE) &&
		    (cur->properties != NULL)) {
		    cur = (xmlNodePtr) cur->properties;
		    continue;
		}
		break;
	    default:
		goto next_sibling;
	}
into_content:
	if ((cur->type == XML_ELEMENT_NODE) &&
	    (cur->children != NULL)) {
	    cur = cur->children;
	    continue;
	}
next_sibling:
	if (cur == elem)
	    break;
	if (cur->type == XML_ELEMENT_NODE) {
	    if (XML_NSMAP_NOTEMPTY(nsMap)) {
		while ((nsMap->last != NULL) &&
		    (nsMap->last->depth >= depth))
		{
		    XML_NSMAP_POP(nsMap, mi)
		}
		XML_NSMAP_FOREACH(nsMap, mi) {
		    if (mi->shadowDepth >= depth)
			mi->shadowDepth = -1;
		}
	    }
	    depth--;
	}
	if (cur->next != NULL)
	    cur = cur->next;
	else {
	    if (cur->type == XML_ATTRIBUTE_NODE) {
		cur = cur->parent;
		goto into_content;
	    }
	    cur = cur->parent;
	    goto next_sibling;
	}
    } while (cur != NULL);

    ret = 0;
    goto exit;
internal_error:
    ret = -1;
exit:
    if (listRedund) {
	for (i = 0, j = 0; i < nbRedund; i++, j += 2) {
	    xmlFreeNs(listRedund[j]);
	}
	xmlFree(listRedund);
    }
    if (nsMap != NULL)
	xmlDOMWrapNsMapFree(nsMap);
    return (ret);
}

static int
xmlDOMWrapAdoptBranch(xmlDOMWrapCtxtPtr ctxt,
		      xmlDocPtr sourceDoc,
		      xmlNodePtr node,
		      xmlDocPtr destDoc,
		      xmlNodePtr destParent,
		      int options ATTRIBUTE_UNUSED)
{
    int ret = 0;
    xmlNodePtr cur, curElem = NULL;
    xmlNsMapPtr nsMap = NULL;
    xmlNsMapItemPtr mi;
    xmlNsPtr ns = NULL;
    int depth = -1, adoptStr = 1;
    
    int parnsdone;
    
    int ancestorsOnly = 0;

    if ((sourceDoc != NULL) &&
	(sourceDoc->dict == destDoc->dict))
	adoptStr = 0;
    else
	adoptStr = 1;

    if (ctxt)
	nsMap = (xmlNsMapPtr) ctxt->namespaceMap;
    if ((destParent == NULL) ||
	(ctxt && ctxt->getNsForNodeFunc))
    {
	parnsdone = 1;
    } else
	parnsdone = 0;

    cur = node;
    while (cur != NULL) {
	if (cur->doc != sourceDoc) {
	    if (cur->next == NULL)
		goto leave_node;
	    do {
		cur = cur->next;
		if ((cur->type == XML_XINCLUDE_END) ||
		    (cur->doc == node->doc))
		    break;
	    } while (cur->next != NULL);

	    if (cur->doc != node->doc)
		goto leave_node;
	}
	cur->doc = destDoc;
	switch (cur->type) {
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		return (-1);
	    case XML_ELEMENT_NODE:
		curElem = cur;
		depth++;
		if ((cur->nsDef) &&
		    ((ctxt == NULL) || (ctxt->getNsForNodeFunc == NULL)))
		{
		    if (! parnsdone) {
			if (xmlDOMWrapNSNormGatherInScopeNs(&nsMap,
			    destParent) == -1)
			    goto internal_error;
			parnsdone = 1;
		    }
		    for (ns = cur->nsDef; ns != NULL; ns = ns->next) {
			if (XML_NSMAP_NOTEMPTY(nsMap)) {
			    XML_NSMAP_FOREACH(nsMap, mi) {
				if ((mi->depth >= XML_TREE_NSMAP_PARENT) &&
				    (mi->shadowDepth == -1) &&
				    ((ns->prefix == mi->newNs->prefix) ||
				    xmlStrEqual(ns->prefix,
				    mi->newNs->prefix))) {

				    mi->shadowDepth = depth;
				}
			    }
			}
			if (xmlDOMWrapNsMapAddItem(&nsMap, -1,
			    ns, ns, depth) == NULL)
			    goto internal_error;
		    }
		}
		
	    case XML_ATTRIBUTE_NODE:
		
		if (cur->ns == NULL)
		    goto ns_end;

		if (! parnsdone) {
		    if (xmlDOMWrapNSNormGatherInScopeNs(&nsMap,
			destParent) == -1)
			goto internal_error;
		    parnsdone = 1;
		}
		if (XML_NSMAP_NOTEMPTY(nsMap)) {
		    XML_NSMAP_FOREACH(nsMap, mi) {
			if ((mi->shadowDepth == -1) &&
			    (cur->ns == mi->oldNs)) {

			    cur->ns = mi->newNs;
			    goto ns_end;
			}
		    }
		}
		if ((ctxt) && (ctxt->getNsForNodeFunc)) {
		    ns = ctxt->getNsForNodeFunc(ctxt, cur,
			cur->ns->href, cur->ns->prefix);
		    if (xmlDOMWrapNsMapAddItem(&nsMap, -1,
			    cur->ns, ns, XML_TREE_NSMAP_CUSTOM) == NULL)
			goto internal_error;
		    cur->ns = ns;
		} else {
		    if (xmlDOMWrapNSNormAquireNormalizedNs(destDoc,
			
			destParent ? curElem : NULL,
			cur->ns, &ns,
			&nsMap, depth,
			ancestorsOnly,
			
			(cur->type == XML_ATTRIBUTE_NODE) ? 1 : 0) == -1)
			goto internal_error;
		    cur->ns = ns;
		}
ns_end:
		XML_TREE_ADOPT_STR(cur->name)
		if (cur->type == XML_ELEMENT_NODE) {
		    cur->psvi = NULL;
		    cur->line = 0;
		    cur->extra = 0;
		    if (cur->properties != NULL) {
			cur = (xmlNodePtr) cur->properties;
			continue;
		    }
		} else {
		    if ((sourceDoc != NULL) &&
			(((xmlAttrPtr) cur)->atype == XML_ATTRIBUTE_ID))
		    {
			xmlRemoveID(sourceDoc, (xmlAttrPtr) cur);
		    }
		    ((xmlAttrPtr) cur)->atype = 0;
		    ((xmlAttrPtr) cur)->psvi = NULL;
		}
		break;
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
		XML_TREE_ADOPT_STR_2(cur->content)
		goto leave_node;
	    case XML_ENTITY_REF_NODE:
		cur->content = NULL;
		cur->children = NULL;
		cur->last = NULL;
		if ((destDoc->intSubset) || (destDoc->extSubset)) {
		    xmlEntityPtr ent;
		    ent = xmlGetDocEntity(destDoc, cur->name);
		    if (ent != NULL) {
			cur->content = ent->content;
			cur->children = (xmlNodePtr) ent;
			cur->last = (xmlNodePtr) ent;
		    }
		}
		goto leave_node;
	    case XML_PI_NODE:
		XML_TREE_ADOPT_STR(cur->name)
		XML_TREE_ADOPT_STR_2(cur->content)
		break;
	    case XML_COMMENT_NODE:
		break;
	    default:
		goto internal_error;
	}
	if (cur->children != NULL) {
	    cur = cur->children;
	    continue;
	}

leave_node:
	if (cur == node)
	    break;
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_XINCLUDE_START) ||
	    (cur->type == XML_XINCLUDE_END))
	{
	    if (XML_NSMAP_NOTEMPTY(nsMap)) {
		while ((nsMap->last != NULL) &&
		    (nsMap->last->depth >= depth))
		{
		    XML_NSMAP_POP(nsMap, mi)
		}
		XML_NSMAP_FOREACH(nsMap, mi) {
		    if (mi->shadowDepth >= depth)
			mi->shadowDepth = -1;
		}
	    }
	    depth--;
	}
	if (cur->next != NULL)
	    cur = cur->next;
	else if ((cur->type == XML_ATTRIBUTE_NODE) &&
	    (cur->parent->children != NULL))
	{
	    cur = cur->parent->children;
	} else {
	    cur = cur->parent;
	    goto leave_node;
	}
    }

    goto exit;

internal_error:
    ret = -1;

exit:
    if (nsMap != NULL) {
	if ((ctxt) && (ctxt->namespaceMap == nsMap)) {
	    if (nsMap->first) {
		if (nsMap->pool)
		    nsMap->last->next = nsMap->pool;
		nsMap->pool = nsMap->first;
		nsMap->first = NULL;
	    }
	} else
	    xmlDOMWrapNsMapFree(nsMap);
    }
    return(ret);
}


int
xmlDOMWrapCloneNode(xmlDOMWrapCtxtPtr ctxt,
		      xmlDocPtr sourceDoc,
		      xmlNodePtr node,
		      xmlNodePtr *resNode,
		      xmlDocPtr destDoc,
		      xmlNodePtr destParent,
		      int deep,
		      int options ATTRIBUTE_UNUSED)
{
    int ret = 0;
    xmlNodePtr cur, curElem = NULL;
    xmlNsMapPtr nsMap = NULL;
    xmlNsMapItemPtr mi;
    xmlNsPtr ns;
    int depth = -1;
    
    
    int parnsdone = 0;
    int ancestorsOnly = 0;
    xmlNodePtr resultClone = NULL, clone = NULL, parentClone = NULL, prevClone = NULL;
    xmlNsPtr cloneNs = NULL, *cloneNsDefSlot = NULL;
    xmlDictPtr dict; 

    if ((node == NULL) || (resNode == NULL) || (destDoc == NULL))
	return(-1);
    if (node->type != XML_ELEMENT_NODE)
	return(1);
    if ((node->doc != NULL) && (sourceDoc != NULL) &&
	(node->doc != sourceDoc)) {
	return (-1);
    }
    if (sourceDoc == NULL)
	sourceDoc = node->doc;
    if (sourceDoc == NULL)
        return (-1);

    dict = destDoc->dict;
    if (ctxt)
	nsMap = (xmlNsMapPtr) ctxt->namespaceMap;

    *resNode = NULL;

    cur = node;
    while (cur != NULL) {
	if (cur->doc != sourceDoc) {
	    goto internal_error;
	}
	switch (cur->type) {
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		goto internal_error;
		break;
	    case XML_ELEMENT_NODE:
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
	    case XML_COMMENT_NODE:
	    case XML_PI_NODE:
	    case XML_DOCUMENT_FRAG_NODE:
	    case XML_ENTITY_REF_NODE:
	    case XML_ENTITY_NODE:
		clone = (xmlNodePtr) xmlMalloc(sizeof(xmlNode));
		if (clone == NULL) {
		    xmlTreeErrMemory("xmlDOMWrapCloneNode(): allocating a node");
		    goto internal_error;
		}
		memset(clone, 0, sizeof(xmlNode));
		if (resultClone != NULL) {
		    clone->parent = parentClone;
		    if (prevClone) {
			prevClone->next = clone;
			clone->prev = prevClone;
		    } else
			parentClone->children = clone;
		} else
		    resultClone = clone;

		break;
	    case XML_ATTRIBUTE_NODE:
		clone = (xmlNodePtr) xmlMalloc(sizeof(xmlAttr));
		if (clone == NULL) {
		    xmlTreeErrMemory("xmlDOMWrapCloneNode(): allocating an attr-node");
		    goto internal_error;
		}
		memset(clone, 0, sizeof(xmlAttr));
		if (resultClone != NULL) {
		    clone->parent = parentClone;
		    if (prevClone) {
			prevClone->next = clone;
			clone->prev = prevClone;
		    } else
			parentClone->properties = (xmlAttrPtr) clone;
		} else
		    resultClone = clone;
		break;
	    default:
		goto internal_error;
	}

	clone->type = cur->type;
	clone->doc = destDoc;

	if (cur->name == xmlStringText)
	    clone->name = xmlStringText;
	else if (cur->name == xmlStringTextNoenc)
	    clone->name = xmlStringTextNoenc;
	else if (cur->name == xmlStringComment)
	    clone->name = xmlStringComment;
	else if (cur->name != NULL) {
	    DICT_CONST_COPY(cur->name, clone->name);
	}

	switch (cur->type) {
	    case XML_XINCLUDE_START:
	    case XML_XINCLUDE_END:
		return (-1);
	    case XML_ELEMENT_NODE:
		curElem = cur;
		depth++;
		if (cur->nsDef != NULL) {
		    if (! parnsdone) {
			if (destParent && (ctxt == NULL)) {
			    if (xmlDOMWrapNSNormGatherInScopeNs(&nsMap,
				destParent) == -1)
				goto internal_error;
			}
			parnsdone = 1;
		    }
		    cloneNsDefSlot = &(clone->nsDef);
		    for (ns = cur->nsDef; ns != NULL; ns = ns->next) {
			cloneNs = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
			if (cloneNs == NULL) {
			    xmlTreeErrMemory("xmlDOMWrapCloneNode(): "
				"allocating namespace");
			    return(-1);
			}
			memset(cloneNs, 0, sizeof(xmlNs));
			cloneNs->type = XML_LOCAL_NAMESPACE;

			if (ns->href != NULL)
			    cloneNs->href = xmlStrdup(ns->href);
			if (ns->prefix != NULL)
			    cloneNs->prefix = xmlStrdup(ns->prefix);

			*cloneNsDefSlot = cloneNs;
			cloneNsDefSlot = &(cloneNs->next);

			if ((ctxt == NULL) ||
			    (ctxt->getNsForNodeFunc == NULL))
			{
			    if (XML_NSMAP_NOTEMPTY(nsMap)) {
				XML_NSMAP_FOREACH(nsMap, mi) {
				    if ((mi->depth >= XML_TREE_NSMAP_PARENT) &&
					(mi->shadowDepth == -1) &&
					((ns->prefix == mi->newNs->prefix) ||
					xmlStrEqual(ns->prefix,
					mi->newNs->prefix))) {
					mi->shadowDepth = depth;
				    }
				}
			    }
			    if (xmlDOMWrapNsMapAddItem(&nsMap, -1,
				ns, cloneNs, depth) == NULL)
				goto internal_error;
			}
		    }
		}
		
		break;
	    case XML_ATTRIBUTE_NODE:
		
		
		break;
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
		DICT_COPY(cur->content, clone->content);
		goto leave_node;
	    case XML_ENTITY_NODE:
		
		goto leave_node;
	    case XML_ENTITY_REF_NODE:
		if (sourceDoc != destDoc) {
		    if ((destDoc->intSubset) || (destDoc->extSubset)) {
			xmlEntityPtr ent;
			ent = xmlGetDocEntity(destDoc, cur->name);
			if (ent != NULL) {
			    clone->content = ent->content;
			    clone->children = (xmlNodePtr) ent;
			    clone->last = (xmlNodePtr) ent;
			}
		    }
		} else {
		    clone->content = cur->content;
		    clone->children = cur->children;
		    clone->last = cur->last;
		}
		goto leave_node;
	    case XML_PI_NODE:
		DICT_COPY(cur->content, clone->content);
		goto leave_node;
	    case XML_COMMENT_NODE:
		DICT_COPY(cur->content, clone->content);
		goto leave_node;
	    default:
		goto internal_error;
	}

	if (cur->ns == NULL)
	    goto end_ns_reference;

	if (! parnsdone) {
	    if (destParent && (ctxt == NULL)) {
		if (xmlDOMWrapNSNormGatherInScopeNs(&nsMap, destParent) == -1)
		    goto internal_error;
	    }
	    parnsdone = 1;
	}
	if (XML_NSMAP_NOTEMPTY(nsMap)) {
	    XML_NSMAP_FOREACH(nsMap, mi) {
		if ((mi->shadowDepth == -1) &&
		    (cur->ns == mi->oldNs)) {
		    clone->ns = mi->newNs;
		    goto end_ns_reference;
		}
	    }
	}
	if ((ctxt != NULL) && (ctxt->getNsForNodeFunc != NULL)) {
	    ns = ctxt->getNsForNodeFunc(ctxt, cur,
		cur->ns->href, cur->ns->prefix);
	    if (xmlDOMWrapNsMapAddItem(&nsMap, -1,
		cur->ns, ns, XML_TREE_NSMAP_CUSTOM) == NULL)
		goto internal_error;
	    clone->ns = ns;
	} else {
	    if (xmlDOMWrapNSNormAquireNormalizedNs(destDoc,
		
		destParent ? curElem : NULL,
		cur->ns, &ns,
		&nsMap, depth,
		
		ancestorsOnly,
		
		(cur->type == XML_ATTRIBUTE_NODE) ? 1 : 0) == -1)
		goto internal_error;
	    clone->ns = ns;
	}

end_ns_reference:

	if ((clone->type == XML_ATTRIBUTE_NODE) &&
	    (clone->parent != NULL))
	{
	    if (xmlIsID(destDoc, clone->parent, (xmlAttrPtr) clone)) {

		xmlChar *idVal;

		idVal = xmlNodeListGetString(cur->doc, cur->children, 1);
		if (idVal != NULL) {
		    if (xmlAddID(NULL, destDoc, idVal, (xmlAttrPtr) cur) == NULL) {
			
			xmlFree(idVal);
			goto internal_error;
		    }
		    xmlFree(idVal);
		}
	    }
	}
	if ((cur->type == XML_ELEMENT_NODE) && (cur->properties != NULL)) {
	    prevClone = NULL;
	    parentClone = clone;
	    cur = (xmlNodePtr) cur->properties;
	    continue;
	}
into_content:
	if (cur->children != NULL) {
	    if (deep || (cur->type == XML_ATTRIBUTE_NODE)) {
		prevClone = NULL;
		parentClone = clone;
		cur = cur->children;
		continue;
	    }
	}

leave_node:
	if (cur == node)
	    break;
	if ((cur->type == XML_ELEMENT_NODE) ||
	    (cur->type == XML_XINCLUDE_START) ||
	    (cur->type == XML_XINCLUDE_END)) {
	    if (XML_NSMAP_NOTEMPTY(nsMap)) {
		while ((nsMap->last != NULL) &&
		    (nsMap->last->depth >= depth))
		{
		    XML_NSMAP_POP(nsMap, mi)
		}
		XML_NSMAP_FOREACH(nsMap, mi) {
		    if (mi->shadowDepth >= depth)
			mi->shadowDepth = -1;
		}
	    }
	    depth--;
	}
	if (cur->next != NULL) {
	    prevClone = clone;
	    cur = cur->next;
	} else if (cur->type != XML_ATTRIBUTE_NODE) {
	    if (clone->parent != NULL)
		clone->parent->last = clone;
	    clone = clone->parent;
	    parentClone = clone->parent;
	    cur = cur->parent;
	    goto leave_node;
	} else {
	    
	    clone = clone->parent;
	    parentClone = clone->parent;
	    cur = cur->parent;
	    goto into_content;
	}
    }
    goto exit;

internal_error:
    ret = -1;

exit:
    if (nsMap != NULL) {
	if ((ctxt) && (ctxt->namespaceMap == nsMap)) {
	    if (nsMap->first) {
		if (nsMap->pool)
		    nsMap->last->next = nsMap->pool;
		nsMap->pool = nsMap->first;
		nsMap->first = NULL;
	    }
	} else
	    xmlDOMWrapNsMapFree(nsMap);
    }
    *resNode = resultClone;
    return (ret);
}

static int
xmlDOMWrapAdoptAttr(xmlDOMWrapCtxtPtr ctxt,
		    xmlDocPtr sourceDoc,
		    xmlAttrPtr attr,
		    xmlDocPtr destDoc,
		    xmlNodePtr destParent,
		    int options ATTRIBUTE_UNUSED)
{
    xmlNodePtr cur;
    int adoptStr = 1;

    if ((attr == NULL) || (destDoc == NULL))
	return (-1);

    attr->doc = destDoc;
    if (attr->ns != NULL) {
	xmlNsPtr ns = NULL;

	if (ctxt != NULL) {
	    
	}
	
	if (IS_STR_XML(attr->ns->prefix)) {
	    ns = xmlTreeEnsureXMLDecl(destDoc);
	} else if (destParent == NULL) {
	    ns = xmlDOMWrapStoreNs(destDoc, attr->ns->href, attr->ns->prefix);
	} else {
	    if (xmlSearchNsByNamespaceStrict(destDoc, destParent, attr->ns->href,
		&ns, 1) == -1)
		goto internal_error;
	    if (ns == NULL) {
		ns = xmlDOMWrapNSNormDeclareNsForced(destDoc, destParent,
		    attr->ns->href, attr->ns->prefix, 1);
	    }
	}
	if (ns == NULL)
	    goto internal_error;
	attr->ns = ns;
    }

    XML_TREE_ADOPT_STR(attr->name);
    attr->atype = 0;
    attr->psvi = NULL;
    if (attr->children == NULL)
	return (0);
    cur = attr->children;
    while (cur != NULL) {
	cur->doc = destDoc;
	switch (cur->type) {
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
		XML_TREE_ADOPT_STR_2(cur->content)
		break;
	    case XML_ENTITY_REF_NODE:
		cur->content = NULL;
		cur->children = NULL;
		cur->last = NULL;
		if ((destDoc->intSubset) || (destDoc->extSubset)) {
		    xmlEntityPtr ent;
		    ent = xmlGetDocEntity(destDoc, cur->name);
		    if (ent != NULL) {
			cur->content = ent->content;
			cur->children = (xmlNodePtr) ent;
			cur->last = (xmlNodePtr) ent;
		    }
		}
		break;
	    default:
		break;
	}
	if (cur->children != NULL) {
	    cur = cur->children;
	    continue;
	}
next_sibling:
	if (cur == (xmlNodePtr) attr)
	    break;
	if (cur->next != NULL)
	    cur = cur->next;
	else {
	    cur = cur->parent;
	    goto next_sibling;
	}
    }
    return (0);
internal_error:
    return (-1);
}

int
xmlDOMWrapAdoptNode(xmlDOMWrapCtxtPtr ctxt,
		    xmlDocPtr sourceDoc,
		    xmlNodePtr node,
		    xmlDocPtr destDoc,
		    xmlNodePtr destParent,
		    int options)
{
    if ((node == NULL) || (destDoc == NULL) ||
	((destParent != NULL) && (destParent->doc != destDoc)))
	return(-1);
    if ((node->doc != NULL) && (sourceDoc != NULL) &&
	(node->doc != sourceDoc)) {
	return (-1);
    }
    if (sourceDoc == NULL)
	sourceDoc = node->doc;
    if (sourceDoc == destDoc)
	return (-1);
    switch (node->type) {
	case XML_ELEMENT_NODE:
	case XML_ATTRIBUTE_NODE:
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_ENTITY_REF_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	    break;
	case XML_DOCUMENT_FRAG_NODE:
	    
	    return (2);
	default:
	    return (1);
    }
    if ((node->parent != NULL) && (destParent != node->parent))
	xmlUnlinkNode(node);

    if (node->type == XML_ELEMENT_NODE) {
	    return (xmlDOMWrapAdoptBranch(ctxt, sourceDoc, node,
		    destDoc, destParent, options));
    } else if (node->type == XML_ATTRIBUTE_NODE) {
	    return (xmlDOMWrapAdoptAttr(ctxt, sourceDoc,
		(xmlAttrPtr) node, destDoc, destParent, options));
    } else {
	xmlNodePtr cur = node;
	int adoptStr = 1;

	cur->doc = destDoc;
	if ((sourceDoc != NULL) &&
	    (sourceDoc->dict == destDoc->dict))
		adoptStr = 0;
	switch (node->type) {
	    case XML_TEXT_NODE:
	    case XML_CDATA_SECTION_NODE:
		XML_TREE_ADOPT_STR_2(node->content)
		    break;
	    case XML_ENTITY_REF_NODE:
		node->content = NULL;
		node->children = NULL;
		node->last = NULL;
		if ((destDoc->intSubset) || (destDoc->extSubset)) {
		    xmlEntityPtr ent;
		    ent = xmlGetDocEntity(destDoc, node->name);
		    if (ent != NULL) {
			node->content = ent->content;
			node->children = (xmlNodePtr) ent;
			node->last = (xmlNodePtr) ent;
		    }
		}
		XML_TREE_ADOPT_STR(node->name)
		break;
	    case XML_PI_NODE: {
		XML_TREE_ADOPT_STR(node->name)
		XML_TREE_ADOPT_STR_2(node->content)
		break;
	    }
	    default:
		break;
	}
    }
    return (0);
}

#define bottom_tree
#include "elfgcchack.h"
