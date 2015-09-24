/*
 * keys.c: Implemetation of the keys support
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
#include <libxml/parserInternals.h>
#include <libxml/xpathInternals.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "imports.h"
#include "templates.h"
#include "keys.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_KEYS
#endif

static int
xsltInitDocKeyTable(xsltTransformContextPtr ctxt, const xmlChar *name,
                    const xmlChar *nameURI);


static xsltKeyDefPtr
xsltNewKeyDef(const xmlChar *name, const xmlChar *nameURI) {
    xsltKeyDefPtr cur;

    cur = (xsltKeyDefPtr) xmlMalloc(sizeof(xsltKeyDef));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewKeyDef : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltKeyDef));
    if (name != NULL)
	cur->name = xmlStrdup(name);
    if (nameURI != NULL)
	cur->nameURI = xmlStrdup(nameURI);
    cur->nsList = NULL;
    return(cur);
}

static void
xsltFreeKeyDef(xsltKeyDefPtr keyd) {
    if (keyd == NULL)
	return;
    if (keyd->comp != NULL)
	xmlXPathFreeCompExpr(keyd->comp);
    if (keyd->usecomp != NULL)
	xmlXPathFreeCompExpr(keyd->usecomp);
    if (keyd->name != NULL)
	xmlFree(keyd->name);
    if (keyd->nameURI != NULL)
	xmlFree(keyd->nameURI);
    if (keyd->match != NULL)
	xmlFree(keyd->match);
    if (keyd->use != NULL)
	xmlFree(keyd->use);
    if (keyd->nsList != NULL)
        xmlFree(keyd->nsList);
    memset(keyd, -1, sizeof(xsltKeyDef));
    xmlFree(keyd);
}

static void
xsltFreeKeyDefList(xsltKeyDefPtr keyd) {
    xsltKeyDefPtr cur;

    while (keyd != NULL) {
	cur = keyd;
	keyd = keyd->next;
	xsltFreeKeyDef(cur);
    }
}

static xsltKeyTablePtr
xsltNewKeyTable(const xmlChar *name, const xmlChar *nameURI) {
    xsltKeyTablePtr cur;

    cur = (xsltKeyTablePtr) xmlMalloc(sizeof(xsltKeyTable));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewKeyTable : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltKeyTable));
    if (name != NULL)
	cur->name = xmlStrdup(name);
    if (nameURI != NULL)
	cur->nameURI = xmlStrdup(nameURI);
    cur->keys = xmlHashCreate(0);
    return(cur);
}

static void
xsltFreeKeyTable(xsltKeyTablePtr keyt) {
    if (keyt == NULL)
	return;
    if (keyt->name != NULL)
	xmlFree(keyt->name);
    if (keyt->nameURI != NULL)
	xmlFree(keyt->nameURI);
    if (keyt->keys != NULL)
	xmlHashFree(keyt->keys, 
		    (xmlHashDeallocator) xmlXPathFreeNodeSet);
    memset(keyt, -1, sizeof(xsltKeyTable));
    xmlFree(keyt);
}

static void
xsltFreeKeyTableList(xsltKeyTablePtr keyt) {
    xsltKeyTablePtr cur;

    while (keyt != NULL) {
	cur = keyt;
	keyt = keyt->next;
	xsltFreeKeyTable(cur);
    }
}



void
xsltFreeKeys(xsltStylesheetPtr style) {
    if (style->keys)
	xsltFreeKeyDefList((xsltKeyDefPtr) style->keys);
}

static int
skipString(const xmlChar *cur, int end) {
    xmlChar limit;

    if ((cur == NULL) || (end < 0)) return(-1);
    if ((cur[end] == '\'') || (cur[end] == '"')) limit = cur[end];
    else return(end);
    end++;
    while (cur[end] != 0) {
        if (cur[end] == limit)
	    return(end + 1);
	end++;
    }
    return(-1);
}

static int
skipPredicate(const xmlChar *cur, int end) {
    if ((cur == NULL) || (end < 0)) return(-1);
    if (cur[end] != '[') return(end);
    end++;
    while (cur[end] != 0) {
        if ((cur[end] == '\'') || (cur[end] == '"')) {
	    end = skipString(cur, end);
	    if (end <= 0)
	        return(-1);
	    continue;
	} else if (cur[end] == '[') {
	    end = skipPredicate(cur, end);
	    if (end <= 0)
	        return(-1);
	    continue;
	} else if (cur[end] == ']')
	    return(end + 1);
	end++;
    }
    return(-1);
}

int
xsltAddKey(xsltStylesheetPtr style, const xmlChar *name,
	   const xmlChar *nameURI, const xmlChar *match,
	   const xmlChar *use, xmlNodePtr inst) {
    xsltKeyDefPtr key;
    xmlChar *pattern = NULL;
    int current, end, start, i = 0;

    if ((style == NULL) || (name == NULL) || (match == NULL) || (use == NULL))
	return(-1);

#ifdef WITH_XSLT_DEBUG_KEYS
    xsltGenericDebug(xsltGenericDebugContext,
	"Add key %s, match %s, use %s\n", name, match, use);
#endif

    key = xsltNewKeyDef(name, nameURI);
    key->match = xmlStrdup(match);
    key->use = xmlStrdup(use);
    key->inst = inst;
    key->nsList = xmlGetNsList(inst->doc, inst);
    if (key->nsList != NULL) {
        while (key->nsList[i] != NULL)
	    i++;
    }
    key->nsNr = i;

    current = end = 0;
    while (match[current] != 0) {
	start = current;
	while (IS_BLANK_CH(match[current]))
	    current++;
	end = current;
	while ((match[end] != 0) && (match[end] != '|')) {
	    if (match[end] == '[') {
	        end = skipPredicate(match, end);
		if (end <= 0) {
		    xsltTransformError(NULL, style, inst,
		                       "key pattern is malformed: %s",
				       key->match);
		    if (style != NULL) style->errors++;
		    goto error;
		}
	    } else
		end++;
	}
	if (current == end) {
	    xsltTransformError(NULL, style, inst,
			       "key pattern is empty\n");
	    if (style != NULL) style->errors++;
	    goto error;
	}
	if (match[start] != '/') {
	    pattern = xmlStrcat(pattern, (xmlChar *)"//");
	    if (pattern == NULL) {
		if (style != NULL) style->errors++;
		goto error;
	    }
	}
	pattern = xmlStrncat(pattern, &match[start], end - start);
	if (pattern == NULL) {
	    if (style != NULL) style->errors++;
	    goto error;
	}

	if (match[end] == '|') {
	    pattern = xmlStrcat(pattern, (xmlChar *)"|");
	    end++;
	}
	current = end;
    }
#ifdef WITH_XSLT_DEBUG_KEYS
    xsltGenericDebug(xsltGenericDebugContext,
	"   resulting pattern %s\n", pattern);
#endif
    key->comp = xsltXPathCompile(style, pattern);
    if (key->comp == NULL) {
	xsltTransformError(NULL, style, inst,
		"xsl:key : XPath pattern compilation failed '%s'\n",
		         pattern);
	if (style != NULL) style->errors++;
    }
    key->usecomp = xsltXPathCompile(style, use);
    if (key->usecomp == NULL) {
	xsltTransformError(NULL, style, inst,
		"xsl:key : XPath pattern compilation failed '%s'\n",
		         use);
	if (style != NULL) style->errors++;
    }

    if (style->keys == NULL) {
	style->keys = key;
    } else {
        xsltKeyDefPtr prev = style->keys;

	while (prev->next != NULL)
	    prev = prev->next;

	prev->next = key;
    }
    key->next = NULL;

error:
    if (pattern != NULL)
	xmlFree(pattern);
    return(0);
}

xmlNodeSetPtr
xsltGetKey(xsltTransformContextPtr ctxt, const xmlChar *name,
	   const xmlChar *nameURI, const xmlChar *value) {
    xmlNodeSetPtr ret;
    xsltKeyTablePtr table;
    int init_table = 0;

    if ((ctxt == NULL) || (name == NULL) || (value == NULL) ||
	(ctxt->document == NULL))
	return(NULL);

#ifdef WITH_XSLT_DEBUG_KEYS
    xsltGenericDebug(xsltGenericDebugContext,
	"Get key %s, value %s\n", name, value);
#endif

    if ((ctxt->document->nbKeysComputed < ctxt->nbKeys) &&
        (ctxt->keyInitLevel == 0)) {
	if (xsltInitAllDocKeys(ctxt))
	    return(NULL);
    }

retry:
    table = (xsltKeyTablePtr) ctxt->document->keys;
    while (table != NULL) {
	if (((nameURI != NULL) == (table->nameURI != NULL)) &&
	    xmlStrEqual(table->name, name) &&
	    xmlStrEqual(table->nameURI, nameURI))
	{
	    ret = (xmlNodeSetPtr)xmlHashLookup(table->keys, value);
	    return(ret);
	}
	table = table->next;
    }

    if ((ctxt->keyInitLevel != 0) && (init_table == 0)) {
        xsltInitDocKeyTable(ctxt, name, nameURI);
	init_table = 1;
	goto retry;
    }

    return(NULL);
}


static int
xsltInitDocKeyTable(xsltTransformContextPtr ctxt, const xmlChar *name,
                    const xmlChar *nameURI)
{
    xsltStylesheetPtr style;
    xsltKeyDefPtr keyd = NULL;
    int found = 0;

#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitDocKeyTable %s\n", name);
#endif

    style = ctxt->style;
    while (style != NULL) {
	keyd = (xsltKeyDefPtr) style->keys;
	while (keyd != NULL) {
	    if (((keyd->nameURI != NULL) ==
		 (nameURI != NULL)) &&
		xmlStrEqual(keyd->name, name) &&
		xmlStrEqual(keyd->nameURI, nameURI))
	    {
		xsltInitCtxtKey(ctxt, ctxt->document, keyd);
		if (ctxt->document->nbKeysComputed == ctxt->nbKeys)
		    return(0);
		found = 1;
	    }
	    keyd = keyd->next;
	}
	style = xsltNextImport(style);
    }
    if (found == 0) {
#ifdef WITH_XSLT_DEBUG_KEYS
	XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltInitDocKeyTable: did not found %s\n", name));
#endif
	xsltTransformError(ctxt, NULL, keyd? keyd->inst : NULL,
	    "Failed to find key definition for %s\n", name);
	ctxt->state = XSLT_STATE_STOPPED;
        return(-1);
    }
#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitDocKeyTable %s done\n", name);
#endif
    return(0);
}

int
xsltInitAllDocKeys(xsltTransformContextPtr ctxt)
{
    xsltStylesheetPtr style;
    xsltKeyDefPtr keyd;
    xsltKeyTablePtr table;

    if (ctxt == NULL)
	return(-1);

#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitAllDocKeys %d %d\n",
        ctxt->document->nbKeysComputed, ctxt->nbKeys);
#endif

    if (ctxt->document->nbKeysComputed == ctxt->nbKeys)
	return(0);


    style = ctxt->style;
    while (style) {
	keyd = (xsltKeyDefPtr) style->keys;
	while (keyd != NULL) {
#ifdef KEY_INIT_DEBUG
fprintf(stderr, "Init key %s\n", keyd->name);
#endif
	    table = (xsltKeyTablePtr) ctxt->document->keys;
	    while (table) {
		if (((keyd->nameURI != NULL) == (table->nameURI != NULL)) &&
		    xmlStrEqual(keyd->name, table->name) &&
		    xmlStrEqual(keyd->nameURI, table->nameURI))
		{
		    break;
		}
		table = table->next;
	    }
	    if (table == NULL) {
		xsltInitDocKeyTable(ctxt, keyd->name, keyd->nameURI);
	    }
	    keyd = keyd->next;
	}
	style = xsltNextImport(style);
    }
#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitAllDocKeys: done\n");
#endif
    return(0);
}

int
xsltInitCtxtKey(xsltTransformContextPtr ctxt, xsltDocumentPtr idoc,
	        xsltKeyDefPtr keyDef)
{
    int i, len, k;
    xmlNodeSetPtr matchList = NULL, keylist;
    xmlXPathObjectPtr matchRes = NULL, useRes = NULL;
    xmlChar *str = NULL;
    xsltKeyTablePtr table;
    xmlNodePtr oldInst, cur;
    xmlNodePtr oldContextNode;
    xsltDocumentPtr oldDocInfo;
    int	oldXPPos, oldXPSize;
    xmlDocPtr oldXPDoc;
    int oldXPNsNr;
    xmlNsPtr *oldXPNamespaces;
    xmlXPathContextPtr xpctxt;

#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitCtxtKey %s : %d\n", keyDef->name, ctxt->keyInitLevel);
#endif

    if ((keyDef->comp == NULL) || (keyDef->usecomp == NULL))
	return(-1);

    if (ctxt->keyInitLevel > ctxt->nbKeys) {
#ifdef WITH_XSLT_DEBUG_KEYS
	XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,
	           xsltGenericDebug(xsltGenericDebugContext,
		       "xsltInitCtxtKey: key definition of %s is recursive\n",
		       keyDef->name));
#endif
	xsltTransformError(ctxt, NULL, keyDef->inst,
	    "Key definition for %s is recursive\n", keyDef->name);
	ctxt->state = XSLT_STATE_STOPPED;
        return(-1);
    }
    ctxt->keyInitLevel++;

    xpctxt = ctxt->xpathCtxt;
    idoc->nbKeysComputed++;
    oldInst = ctxt->inst;
    oldDocInfo = ctxt->document;
    oldContextNode = ctxt->node;

    oldXPDoc = xpctxt->doc;
    oldXPPos = xpctxt->proximityPosition;
    oldXPSize = xpctxt->contextSize;
    oldXPNsNr = xpctxt->nsNr;
    oldXPNamespaces = xpctxt->namespaces;

    ctxt->document = idoc;
    ctxt->node = (xmlNodePtr) idoc->doc;
    ctxt->inst = keyDef->inst;

    xpctxt->doc = idoc->doc;
    xpctxt->node = (xmlNodePtr) idoc->doc;
    
    xpctxt->namespaces = keyDef->nsList;
    xpctxt->nsNr = keyDef->nsNr;

    matchRes = xmlXPathCompiledEval(keyDef->comp, xpctxt);
    if (matchRes == NULL) {

#ifdef WITH_XSLT_DEBUG_KEYS
	XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,xsltGenericDebug(xsltGenericDebugContext,
	     "xsltInitCtxtKey: %s evaluation failed\n", keyDef->match));
#endif
	xsltTransformError(ctxt, NULL, keyDef->inst,
	    "Failed to evaluate the 'match' expression.\n");
	ctxt->state = XSLT_STATE_STOPPED;
	goto error;
    } else {
	if (matchRes->type == XPATH_NODESET) {
	    matchList = matchRes->nodesetval;

#ifdef WITH_XSLT_DEBUG_KEYS
	    if (matchList != NULL)
		XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,xsltGenericDebug(xsltGenericDebugContext,
		     "xsltInitCtxtKey: %s evaluates to %d nodes\n",
				 keyDef->match, matchList->nodeNr));
#endif
	} else {
#ifdef WITH_XSLT_DEBUG_KEYS
	    XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,xsltGenericDebug(xsltGenericDebugContext,
		 "xsltInitCtxtKey: %s is not a node set\n", keyDef->match));
#endif
	    xsltTransformError(ctxt, NULL, keyDef->inst,
		"The 'match' expression did not evaluate to a node set.\n");
	    ctxt->state = XSLT_STATE_STOPPED;
	    goto error;
	}
    }
    if ((matchList == NULL) || (matchList->nodeNr <= 0))
	goto exit;

    table = (xsltKeyTablePtr) idoc->keys;
    while (table != NULL) {
        if (xmlStrEqual(table->name, keyDef->name) &&
	    (((keyDef->nameURI == NULL) && (table->nameURI == NULL)) ||
	     ((keyDef->nameURI != NULL) && (table->nameURI != NULL) &&
	      (xmlStrEqual(table->nameURI, keyDef->nameURI)))))
	    break;
	table = table->next;
    }
    if (table == NULL) {
        table = xsltNewKeyTable(keyDef->name, keyDef->nameURI);
        if (table == NULL)
	    goto error;
        table->next = idoc->keys;
        idoc->keys = table;
    }

    xpctxt->contextSize = 1;
    xpctxt->proximityPosition = 1;

    for (i = 0; i < matchList->nodeNr; i++) {
	cur = matchList->nodeTab[i];
	if (! IS_XSLT_REAL_NODE(cur))
	    continue;
	xpctxt->node = cur;
	if (useRes != NULL)
	    xmlXPathFreeObject(useRes);
	useRes = xmlXPathCompiledEval(keyDef->usecomp, xpctxt);
	if (useRes == NULL) {
	    xsltTransformError(ctxt, NULL, keyDef->inst,
		"Failed to evaluate the 'use' expression.\n");
	    ctxt->state = XSLT_STATE_STOPPED;
	    break;
	}
	if (useRes->type == XPATH_NODESET) {
	    if ((useRes->nodesetval != NULL) &&
		(useRes->nodesetval->nodeNr != 0))
	    {
		len = useRes->nodesetval->nodeNr;
		str = xmlXPathCastNodeToString(useRes->nodesetval->nodeTab[0]);
	    } else {
		continue;
	    }
	} else {
	    len = 1;
	    if (useRes->type == XPATH_STRING) {
		str = useRes->stringval;
		useRes->stringval = NULL;
	    } else {
		str = xmlXPathCastToString(useRes);
	    }
	}
	k = 0;
	while (1) {
	    if (str == NULL)
		goto next_string;

#ifdef WITH_XSLT_DEBUG_KEYS
	    XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,xsltGenericDebug(xsltGenericDebugContext,
		"xsl:key : node associated to ('%s', '%s')\n", keyDef->name, str));
#endif

	    keylist = xmlHashLookup(table->keys, str);
	    if (keylist == NULL) {
		keylist = xmlXPathNodeSetCreate(cur);
		if (keylist == NULL)
		    goto error;
		xmlHashAddEntry(table->keys, str, keylist);
	    } else {
		xmlXPathNodeSetAdd(keylist, cur);
	    }
	    switch (cur->type) {
		case XML_ELEMENT_NODE:
		case XML_TEXT_NODE:
		case XML_CDATA_SECTION_NODE:
		case XML_PI_NODE:
		case XML_COMMENT_NODE:
		    cur->psvi = keyDef;
		    break;
		case XML_ATTRIBUTE_NODE:
		    ((xmlAttrPtr) cur)->psvi = keyDef;
		    break;
		case XML_DOCUMENT_NODE:
		case XML_HTML_DOCUMENT_NODE:
		    ((xmlDocPtr) cur)->psvi = keyDef;
		    break;
		default:
		    break;
	    }
	    xmlFree(str);
	    str = NULL;

next_string:
	    k++;
	    if (k >= len)
		break;
	    str = xmlXPathCastNodeToString(useRes->nodesetval->nodeTab[k]);
	}
    }

exit:
error:
    ctxt->keyInitLevel--;
    xpctxt->doc = oldXPDoc;
    xpctxt->nsNr = oldXPNsNr;
    xpctxt->namespaces = oldXPNamespaces;
    xpctxt->proximityPosition = oldXPPos;
    xpctxt->contextSize = oldXPSize;

    ctxt->node = oldContextNode;
    ctxt->document = oldDocInfo;
    ctxt->inst = oldInst;

    if (str)
	xmlFree(str);
    if (useRes != NULL)
	xmlXPathFreeObject(useRes);
    if (matchRes != NULL)
	xmlXPathFreeObject(matchRes);
    return(0);
}

void
xsltInitCtxtKeys(xsltTransformContextPtr ctxt, xsltDocumentPtr idoc) {
    xsltStylesheetPtr style;
    xsltKeyDefPtr keyDef;

    if ((ctxt == NULL) || (idoc == NULL))
	return;

#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitCtxtKeys on document\n");
#endif

#ifdef WITH_XSLT_DEBUG_KEYS
    if ((idoc->doc != NULL) && (idoc->doc->URL != NULL))
	XSLT_TRACE(ctxt,XSLT_TRACE_KEYS,xsltGenericDebug(xsltGenericDebugContext, "Initializing keys on %s\n",
		     idoc->doc->URL));
#endif
    style = ctxt->style;
    while (style != NULL) {
	keyDef = (xsltKeyDefPtr) style->keys;
	while (keyDef != NULL) {
	    xsltInitCtxtKey(ctxt, idoc, keyDef);

	    keyDef = keyDef->next;
	}

	style = xsltNextImport(style);
    }

#ifdef KEY_INIT_DEBUG
fprintf(stderr, "xsltInitCtxtKeys on document: done\n");
#endif

}

void
xsltFreeDocumentKeys(xsltDocumentPtr idoc) {
    if (idoc != NULL)
        xsltFreeKeyTableList(idoc->keys);
}

