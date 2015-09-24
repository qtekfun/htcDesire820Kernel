/*
 * entities.c : implementation for the XML entities handling
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <libxml/xmlmemory.h>
#include <libxml/hash.h>
#include <libxml/entities.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlerror.h>
#include <libxml/globals.h>
#include <libxml/dict.h>


static xmlEntity xmlEntityLt = {
    NULL, XML_ENTITY_DECL, BAD_CAST "lt",
    NULL, NULL, NULL, NULL, NULL, NULL, 
    BAD_CAST "<", BAD_CAST "<", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    NULL, NULL, NULL, NULL, 0, 1
};
static xmlEntity xmlEntityGt = {
    NULL, XML_ENTITY_DECL, BAD_CAST "gt",
    NULL, NULL, NULL, NULL, NULL, NULL, 
    BAD_CAST ">", BAD_CAST ">", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    NULL, NULL, NULL, NULL, 0, 1
};
static xmlEntity xmlEntityAmp = {
    NULL, XML_ENTITY_DECL, BAD_CAST "amp",
    NULL, NULL, NULL, NULL, NULL, NULL, 
    BAD_CAST "&", BAD_CAST "&", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    NULL, NULL, NULL, NULL, 0, 1
};
static xmlEntity xmlEntityQuot = {
    NULL, XML_ENTITY_DECL, BAD_CAST "quot",
    NULL, NULL, NULL, NULL, NULL, NULL, 
    BAD_CAST "\"", BAD_CAST "\"", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    NULL, NULL, NULL, NULL, 0, 1
};
static xmlEntity xmlEntityApos = {
    NULL, XML_ENTITY_DECL, BAD_CAST "apos",
    NULL, NULL, NULL, NULL, NULL, NULL, 
    BAD_CAST "'", BAD_CAST "'", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    NULL, NULL, NULL, NULL, 0, 1
};

static void
xmlEntitiesErrMemory(const char *extra)
{
    __xmlSimpleError(XML_FROM_TREE, XML_ERR_NO_MEMORY, NULL, NULL, extra);
}

static void
xmlEntitiesErr(xmlParserErrors code, const char *msg)
{
    __xmlSimpleError(XML_FROM_TREE, code, NULL, msg, NULL);
}

static void
xmlFreeEntity(xmlEntityPtr entity)
{
    xmlDictPtr dict = NULL;

    if (entity == NULL)
        return;

    if (entity->doc != NULL)
        dict = entity->doc->dict;


    if ((entity->children) && (entity->owner == 1) &&
        (entity == (xmlEntityPtr) entity->children->parent))
        xmlFreeNodeList(entity->children);
    if (dict != NULL) {
        if ((entity->name != NULL) && (!xmlDictOwns(dict, entity->name)))
            xmlFree((char *) entity->name);
        if ((entity->ExternalID != NULL) &&
	    (!xmlDictOwns(dict, entity->ExternalID)))
            xmlFree((char *) entity->ExternalID);
        if ((entity->SystemID != NULL) &&
	    (!xmlDictOwns(dict, entity->SystemID)))
            xmlFree((char *) entity->SystemID);
        if ((entity->URI != NULL) && (!xmlDictOwns(dict, entity->URI)))
            xmlFree((char *) entity->URI);
        if ((entity->content != NULL)
            && (!xmlDictOwns(dict, entity->content)))
            xmlFree((char *) entity->content);
        if ((entity->orig != NULL) && (!xmlDictOwns(dict, entity->orig)))
            xmlFree((char *) entity->orig);
    } else {
        if (entity->name != NULL)
            xmlFree((char *) entity->name);
        if (entity->ExternalID != NULL)
            xmlFree((char *) entity->ExternalID);
        if (entity->SystemID != NULL)
            xmlFree((char *) entity->SystemID);
        if (entity->URI != NULL)
            xmlFree((char *) entity->URI);
        if (entity->content != NULL)
            xmlFree((char *) entity->content);
        if (entity->orig != NULL)
            xmlFree((char *) entity->orig);
    }
    xmlFree(entity);
}

static xmlEntityPtr
xmlCreateEntity(xmlDictPtr dict, const xmlChar *name, int type,
	        const xmlChar *ExternalID, const xmlChar *SystemID,
	        const xmlChar *content) {
    xmlEntityPtr ret;

    ret = (xmlEntityPtr) xmlMalloc(sizeof(xmlEntity));
    if (ret == NULL) {
        xmlEntitiesErrMemory("xmlCreateEntity: malloc failed");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlEntity));
    ret->type = XML_ENTITY_DECL;
    ret->checked = 0;

    ret->etype = (xmlEntityType) type;
    if (dict == NULL) {
	ret->name = xmlStrdup(name);
	if (ExternalID != NULL)
	    ret->ExternalID = xmlStrdup(ExternalID);
	if (SystemID != NULL)
	    ret->SystemID = xmlStrdup(SystemID);
    } else {
        ret->name = xmlDictLookup(dict, name, -1);
	if (ExternalID != NULL)
	    ret->ExternalID = xmlDictLookup(dict, ExternalID, -1);
	if (SystemID != NULL)
	    ret->SystemID = xmlDictLookup(dict, SystemID, -1);
    }
    if (content != NULL) {
        ret->length = xmlStrlen(content);
	if ((dict != NULL) && (ret->length < 5))
	    ret->content = (xmlChar *)
	                   xmlDictLookup(dict, content, ret->length);
	else
	    ret->content = xmlStrndup(content, ret->length);
     } else {
        ret->length = 0;
        ret->content = NULL;
    }
    ret->URI = NULL; 
    ret->orig = NULL;
    ret->owner = 0;

    return(ret);
}

static xmlEntityPtr
xmlAddEntity(xmlDtdPtr dtd, const xmlChar *name, int type,
	  const xmlChar *ExternalID, const xmlChar *SystemID,
	  const xmlChar *content) {
    xmlDictPtr dict = NULL;
    xmlEntitiesTablePtr table = NULL;
    xmlEntityPtr ret;

    if (name == NULL)
	return(NULL);
    if (dtd == NULL)
	return(NULL);
    if (dtd->doc != NULL)
        dict = dtd->doc->dict;

    switch (type) {
        case XML_INTERNAL_GENERAL_ENTITY:
        case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
        case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
	    if (dtd->entities == NULL)
		dtd->entities = xmlHashCreateDict(0, dict);
	    table = dtd->entities;
	    break;
        case XML_INTERNAL_PARAMETER_ENTITY:
        case XML_EXTERNAL_PARAMETER_ENTITY:
	    if (dtd->pentities == NULL)
		dtd->pentities = xmlHashCreateDict(0, dict);
	    table = dtd->pentities;
	    break;
        case XML_INTERNAL_PREDEFINED_ENTITY:
	    return(NULL);
    }
    if (table == NULL)
	return(NULL);
    ret = xmlCreateEntity(dict, name, type, ExternalID, SystemID, content);
    if (ret == NULL)
        return(NULL);
    ret->doc = dtd->doc;

    if (xmlHashAddEntry(table, name, ret)) {
        xmlFreeEntity(ret);
	return(NULL);
    }
    return(ret);
}

xmlEntityPtr
xmlGetPredefinedEntity(const xmlChar *name) {
    if (name == NULL) return(NULL);
    switch (name[0]) {
        case 'l':
	    if (xmlStrEqual(name, BAD_CAST "lt"))
	        return(&xmlEntityLt);
	    break;
        case 'g':
	    if (xmlStrEqual(name, BAD_CAST "gt"))
	        return(&xmlEntityGt);
	    break;
        case 'a':
	    if (xmlStrEqual(name, BAD_CAST "amp"))
	        return(&xmlEntityAmp);
	    if (xmlStrEqual(name, BAD_CAST "apos"))
	        return(&xmlEntityApos);
	    break;
        case 'q':
	    if (xmlStrEqual(name, BAD_CAST "quot"))
	        return(&xmlEntityQuot);
	    break;
	default:
	    break;
    }
    return(NULL);
}

xmlEntityPtr
xmlAddDtdEntity(xmlDocPtr doc, const xmlChar *name, int type,
	        const xmlChar *ExternalID, const xmlChar *SystemID,
		const xmlChar *content) {
    xmlEntityPtr ret;
    xmlDtdPtr dtd;

    if (doc == NULL) {
	xmlEntitiesErr(XML_DTD_NO_DOC,
	        "xmlAddDtdEntity: document is NULL");
	return(NULL);
    }
    if (doc->extSubset == NULL) {
	xmlEntitiesErr(XML_DTD_NO_DTD,
	        "xmlAddDtdEntity: document without external subset");
	return(NULL);
    }
    dtd = doc->extSubset;
    ret = xmlAddEntity(dtd, name, type, ExternalID, SystemID, content);
    if (ret == NULL) return(NULL);

    ret->parent = dtd;
    ret->doc = dtd->doc;
    if (dtd->last == NULL) {
	dtd->children = dtd->last = (xmlNodePtr) ret;
    } else {
        dtd->last->next = (xmlNodePtr) ret;
	ret->prev = dtd->last;
	dtd->last = (xmlNodePtr) ret;
    }
    return(ret);
}

xmlEntityPtr
xmlAddDocEntity(xmlDocPtr doc, const xmlChar *name, int type,
	        const xmlChar *ExternalID, const xmlChar *SystemID,
	        const xmlChar *content) {
    xmlEntityPtr ret;
    xmlDtdPtr dtd;

    if (doc == NULL) {
	xmlEntitiesErr(XML_DTD_NO_DOC,
	        "xmlAddDocEntity: document is NULL");
	return(NULL);
    }
    if (doc->intSubset == NULL) {
	xmlEntitiesErr(XML_DTD_NO_DTD,
	        "xmlAddDocEntity: document without internal subset");
	return(NULL);
    }
    dtd = doc->intSubset;
    ret = xmlAddEntity(dtd, name, type, ExternalID, SystemID, content);
    if (ret == NULL) return(NULL);

    ret->parent = dtd;
    ret->doc = dtd->doc;
    if (dtd->last == NULL) {
	dtd->children = dtd->last = (xmlNodePtr) ret;
    } else {
	dtd->last->next = (xmlNodePtr) ret;
	ret->prev = dtd->last;
	dtd->last = (xmlNodePtr) ret;
    }
    return(ret);
}

xmlEntityPtr
xmlNewEntity(xmlDocPtr doc, const xmlChar *name, int type,
	     const xmlChar *ExternalID, const xmlChar *SystemID,
	     const xmlChar *content) {
    xmlEntityPtr ret;
    xmlDictPtr dict;

    if ((doc != NULL) && (doc->intSubset != NULL)) {
	return(xmlAddDocEntity(doc, name, type, ExternalID, SystemID, content));
    }
    if (doc != NULL)
        dict = doc->dict;
    else
        dict = NULL;
    ret = xmlCreateEntity(dict, name, type, ExternalID, SystemID, content);
    if (ret == NULL)
        return(NULL);
    ret->doc = doc;
    return(ret);
}

static xmlEntityPtr
xmlGetEntityFromTable(xmlEntitiesTablePtr table, const xmlChar *name) {
    return((xmlEntityPtr) xmlHashLookup(table, name));
}

xmlEntityPtr
xmlGetParameterEntity(xmlDocPtr doc, const xmlChar *name) {
    xmlEntitiesTablePtr table;
    xmlEntityPtr ret;

    if (doc == NULL)
	return(NULL);
    if ((doc->intSubset != NULL) && (doc->intSubset->pentities != NULL)) {
	table = (xmlEntitiesTablePtr) doc->intSubset->pentities;
	ret = xmlGetEntityFromTable(table, name);
	if (ret != NULL)
	    return(ret);
    }
    if ((doc->extSubset != NULL) && (doc->extSubset->pentities != NULL)) {
	table = (xmlEntitiesTablePtr) doc->extSubset->pentities;
	return(xmlGetEntityFromTable(table, name));
    }
    return(NULL);
}

xmlEntityPtr
xmlGetDtdEntity(xmlDocPtr doc, const xmlChar *name) {
    xmlEntitiesTablePtr table;

    if (doc == NULL)
	return(NULL);
    if ((doc->extSubset != NULL) && (doc->extSubset->entities != NULL)) {
	table = (xmlEntitiesTablePtr) doc->extSubset->entities;
	return(xmlGetEntityFromTable(table, name));
    }
    return(NULL);
}

xmlEntityPtr
xmlGetDocEntity(xmlDocPtr doc, const xmlChar *name) {
    xmlEntityPtr cur;
    xmlEntitiesTablePtr table;

    if (doc != NULL) {
	if ((doc->intSubset != NULL) && (doc->intSubset->entities != NULL)) {
	    table = (xmlEntitiesTablePtr) doc->intSubset->entities;
	    cur = xmlGetEntityFromTable(table, name);
	    if (cur != NULL)
		return(cur);
	}
	if (doc->standalone != 1) {
	    if ((doc->extSubset != NULL) &&
		(doc->extSubset->entities != NULL)) {
		table = (xmlEntitiesTablePtr) doc->extSubset->entities;
		cur = xmlGetEntityFromTable(table, name);
		if (cur != NULL)
		    return(cur);
	    }
	}
    }
    return(xmlGetPredefinedEntity(name));
}

#define growBufferReentrant() {						\
    buffer_size *= 2;							\
    buffer = (xmlChar *)						\
    		xmlRealloc(buffer, buffer_size * sizeof(xmlChar));	\
    if (buffer == NULL) {						\
        xmlEntitiesErrMemory("xmlEncodeEntitiesReentrant: realloc failed");\
	return(NULL);							\
    }									\
}


xmlChar *
xmlEncodeEntitiesReentrant(xmlDocPtr doc, const xmlChar *input) {
    const xmlChar *cur = input;
    xmlChar *buffer = NULL;
    xmlChar *out = NULL;
    int buffer_size = 0;
    int html = 0;

    if (input == NULL) return(NULL);
    if (doc != NULL)
        html = (doc->type == XML_HTML_DOCUMENT_NODE);

    buffer_size = 1000;
    buffer = (xmlChar *) xmlMalloc(buffer_size * sizeof(xmlChar));
    if (buffer == NULL) {
        xmlEntitiesErrMemory("xmlEncodeEntitiesReentrant: malloc failed");
	return(NULL);
    }
    out = buffer;

    while (*cur != '\0') {
        if (out - buffer > buffer_size - 100) {
	    int indx = out - buffer;

	    growBufferReentrant();
	    out = &buffer[indx];
	}

	if (*cur == '<') {
	    *out++ = '&';
	    *out++ = 'l';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '>') {
	    *out++ = '&';
	    *out++ = 'g';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '&') {
	    *out++ = '&';
	    *out++ = 'a';
	    *out++ = 'm';
	    *out++ = 'p';
	    *out++ = ';';
	} else if (((*cur >= 0x20) && (*cur < 0x80)) ||
	    (*cur == '\n') || (*cur == '\t') || ((html) && (*cur == '\r'))) {
	    *out++ = *cur;
	} else if (*cur >= 0x80) {
	    if (((doc != NULL) && (doc->encoding != NULL)) || (html)) {
		*out++ = *cur;
	    } else {
		char buf[11], *ptr;
		int val = 0, l = 1;

		if (*cur < 0xC0) {
		    xmlEntitiesErr(XML_CHECK_NOT_UTF8,
			    "xmlEncodeEntitiesReentrant : input not UTF-8");
		    if (doc != NULL)
			doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
		    snprintf(buf, sizeof(buf), "&#%d;", *cur);
		    buf[sizeof(buf) - 1] = 0;
		    ptr = buf;
		    while (*ptr != 0) *out++ = *ptr++;
		    cur++;
		    continue;
		} else if (*cur < 0xE0) {
                    val = (cur[0]) & 0x1F;
		    val <<= 6;
		    val |= (cur[1]) & 0x3F;
		    l = 2;
		} else if (*cur < 0xF0) {
                    val = (cur[0]) & 0x0F;
		    val <<= 6;
		    val |= (cur[1]) & 0x3F;
		    val <<= 6;
		    val |= (cur[2]) & 0x3F;
		    l = 3;
		} else if (*cur < 0xF8) {
                    val = (cur[0]) & 0x07;
		    val <<= 6;
		    val |= (cur[1]) & 0x3F;
		    val <<= 6;
		    val |= (cur[2]) & 0x3F;
		    val <<= 6;
		    val |= (cur[3]) & 0x3F;
		    l = 4;
		}
		if ((l == 1) || (!IS_CHAR(val))) {
		    xmlEntitiesErr(XML_ERR_INVALID_CHAR,
			"xmlEncodeEntitiesReentrant : char out of range\n");
		    if (doc != NULL)
			doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
		    snprintf(buf, sizeof(buf), "&#%d;", *cur);
		    buf[sizeof(buf) - 1] = 0;
		    ptr = buf;
		    while (*ptr != 0) *out++ = *ptr++;
		    cur++;
		    continue;
		}
		snprintf(buf, sizeof(buf), "&#x%X;", val);
		buf[sizeof(buf) - 1] = 0;
		ptr = buf;
		while (*ptr != 0) *out++ = *ptr++;
		cur += l;
		continue;
	    }
	} else if (IS_BYTE_CHAR(*cur)) {
	    char buf[11], *ptr;

	    snprintf(buf, sizeof(buf), "&#%d;", *cur);
	    buf[sizeof(buf) - 1] = 0;
            ptr = buf;
	    while (*ptr != 0) *out++ = *ptr++;
	}
	cur++;
    }
    *out = 0;
    return(buffer);
}

xmlChar *
xmlEncodeSpecialChars(xmlDocPtr doc ATTRIBUTE_UNUSED, const xmlChar *input) {
    const xmlChar *cur = input;
    xmlChar *buffer = NULL;
    xmlChar *out = NULL;
    int buffer_size = 0;
    if (input == NULL) return(NULL);

    buffer_size = 1000;
    buffer = (xmlChar *) xmlMalloc(buffer_size * sizeof(xmlChar));
    if (buffer == NULL) {
        xmlEntitiesErrMemory("xmlEncodeSpecialChars: malloc failed");
	return(NULL);
    }
    out = buffer;

    while (*cur != '\0') {
        if (out - buffer > buffer_size - 10) {
	    int indx = out - buffer;

	    growBufferReentrant();
	    out = &buffer[indx];
	}

	if (*cur == '<') {
	    *out++ = '&';
	    *out++ = 'l';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '>') {
	    *out++ = '&';
	    *out++ = 'g';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '&') {
	    *out++ = '&';
	    *out++ = 'a';
	    *out++ = 'm';
	    *out++ = 'p';
	    *out++ = ';';
	} else if (*cur == '"') {
	    *out++ = '&';
	    *out++ = 'q';
	    *out++ = 'u';
	    *out++ = 'o';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '\r') {
	    *out++ = '&';
	    *out++ = '#';
	    *out++ = '1';
	    *out++ = '3';
	    *out++ = ';';
	} else {
	    *out++ = *cur;
	}
	cur++;
    }
    *out = 0;
    return(buffer);
}

xmlEntitiesTablePtr
xmlCreateEntitiesTable(void) {
    return((xmlEntitiesTablePtr) xmlHashCreate(0));
}

static void
xmlFreeEntityWrapper(xmlEntityPtr entity,
	               const xmlChar *name ATTRIBUTE_UNUSED) {
    if (entity != NULL)
	xmlFreeEntity(entity);
}

void
xmlFreeEntitiesTable(xmlEntitiesTablePtr table) {
    xmlHashFree(table, (xmlHashDeallocator) xmlFreeEntityWrapper);
}

#ifdef LIBXML_TREE_ENABLED
static xmlEntityPtr
xmlCopyEntity(xmlEntityPtr ent) {
    xmlEntityPtr cur;

    cur = (xmlEntityPtr) xmlMalloc(sizeof(xmlEntity));
    if (cur == NULL) {
        xmlEntitiesErrMemory("xmlCopyEntity:: malloc failed");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlEntity));
    cur->type = XML_ENTITY_DECL;

    cur->etype = ent->etype;
    if (ent->name != NULL)
	cur->name = xmlStrdup(ent->name);
    if (ent->ExternalID != NULL)
	cur->ExternalID = xmlStrdup(ent->ExternalID);
    if (ent->SystemID != NULL)
	cur->SystemID = xmlStrdup(ent->SystemID);
    if (ent->content != NULL)
	cur->content = xmlStrdup(ent->content);
    if (ent->orig != NULL)
	cur->orig = xmlStrdup(ent->orig);
    if (ent->URI != NULL)
	cur->URI = xmlStrdup(ent->URI);
    return(cur);
}

xmlEntitiesTablePtr
xmlCopyEntitiesTable(xmlEntitiesTablePtr table) {
    return(xmlHashCopy(table, (xmlHashCopier) xmlCopyEntity));
}
#endif 

#ifdef LIBXML_OUTPUT_ENABLED

static void
xmlDumpEntityContent(xmlBufferPtr buf, const xmlChar *content) {
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return;
    if (xmlStrchr(content, '%')) {
        const xmlChar * base, *cur;

	xmlBufferCCat(buf, "\"");
	base = cur = content;
	while (*cur != 0) {
	    if (*cur == '"') {
		if (base != cur)
		    xmlBufferAdd(buf, base, cur - base);
		xmlBufferAdd(buf, BAD_CAST "&quot;", 6);
		cur++;
		base = cur;
	    } else if (*cur == '%') {
		if (base != cur)
		    xmlBufferAdd(buf, base, cur - base);
		xmlBufferAdd(buf, BAD_CAST "&#x25;", 6);
		cur++;
		base = cur;
	    } else {
		cur++;
	    }
	}
	if (base != cur)
	    xmlBufferAdd(buf, base, cur - base);
	xmlBufferCCat(buf, "\"");
    } else {
        xmlBufferWriteQuotedString(buf, content);
    }
}

void
xmlDumpEntityDecl(xmlBufferPtr buf, xmlEntityPtr ent) {
    if ((buf == NULL) || (ent == NULL)) return;
    switch (ent->etype) {
	case XML_INTERNAL_GENERAL_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    xmlBufferWriteChar(buf, " ");
	    if (ent->orig != NULL)
		xmlBufferWriteQuotedString(buf, ent->orig);
	    else
		xmlDumpEntityContent(buf, ent->content);
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    if (ent->ExternalID != NULL) {
		 xmlBufferWriteChar(buf, " PUBLIC ");
		 xmlBufferWriteQuotedString(buf, ent->ExternalID);
		 xmlBufferWriteChar(buf, " ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    } else {
		 xmlBufferWriteChar(buf, " SYSTEM ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    }
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    if (ent->ExternalID != NULL) {
		 xmlBufferWriteChar(buf, " PUBLIC ");
		 xmlBufferWriteQuotedString(buf, ent->ExternalID);
		 xmlBufferWriteChar(buf, " ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    } else {
		 xmlBufferWriteChar(buf, " SYSTEM ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    }
	    if (ent->content != NULL) { 
		xmlBufferWriteChar(buf, " NDATA ");
		if (ent->orig != NULL)
		    xmlBufferWriteCHAR(buf, ent->orig);
		else
		    xmlBufferWriteCHAR(buf, ent->content);
	    }
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_INTERNAL_PARAMETER_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY % ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    xmlBufferWriteChar(buf, " ");
	    if (ent->orig == NULL)
		xmlDumpEntityContent(buf, ent->content);
	    else
		xmlBufferWriteQuotedString(buf, ent->orig);
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_EXTERNAL_PARAMETER_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY % ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    if (ent->ExternalID != NULL) {
		 xmlBufferWriteChar(buf, " PUBLIC ");
		 xmlBufferWriteQuotedString(buf, ent->ExternalID);
		 xmlBufferWriteChar(buf, " ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    } else {
		 xmlBufferWriteChar(buf, " SYSTEM ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    }
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	default:
	    xmlEntitiesErr(XML_DTD_UNKNOWN_ENTITY,
		"xmlDumpEntitiesDecl: internal: unknown type entity type");
    }
}

static void
xmlDumpEntityDeclScan(xmlEntityPtr ent, xmlBufferPtr buf) {
    xmlDumpEntityDecl(buf, ent);
}
      
void
xmlDumpEntitiesTable(xmlBufferPtr buf, xmlEntitiesTablePtr table) {
    xmlHashScan(table, (xmlHashScanner)xmlDumpEntityDeclScan, buf);
}
#endif 
#define bottom_entities
#include "elfgcchack.h"
