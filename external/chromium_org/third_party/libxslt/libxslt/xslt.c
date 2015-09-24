/*
 * xslt.c: Implemetation of an XSL Transformation 1.0 engine
 *
 * Reference:
 *   XSLT specification
 *   http://www.w3.org/TR/1999/REC-xslt-19991116
 *
 *   Associating Style Sheets with XML documents
 *   http://www.w3.org/1999/06/REC-xml-stylesheet-19990629
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
#include <libxml/xmlerror.h>
#include <libxml/parserInternals.h>
#include <libxml/xpathInternals.h>
#include <libxml/xpath.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "pattern.h"
#include "variables.h"
#include "namespaces.h"
#include "attributes.h"
#include "xsltutils.h"
#include "imports.h"
#include "keys.h"
#include "documents.h"
#include "extensions.h"
#include "preproc.h"
#include "extra.h"
#include "security.h"

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_PARSING
#endif

const char *xsltEngineVersion = LIBXSLT_VERSION_STRING LIBXSLT_VERSION_EXTRA;
const int xsltLibxsltVersion = LIBXSLT_VERSION;
const int xsltLibxmlVersion = LIBXML_VERSION;

#ifdef XSLT_REFACTORED

const xmlChar *xsltConstNamespaceNameXSLT = (const xmlChar *) XSLT_NAMESPACE;

#define XSLT_ELEMENT_CATEGORY_XSLT 0
#define XSLT_ELEMENT_CATEGORY_EXTENSION 1
#define XSLT_ELEMENT_CATEGORY_LRE 2


const xmlChar *xsltXSLTTextMarker = (const xmlChar *) "XSLT Text Element";

const xmlChar *xsltXSLTAttrMarker = (const xmlChar *) "LRE XSLT Attr";

#endif

#ifdef XSLT_LOCALE_WINAPI
extern xmlRMutexPtr xsltLocaleMutex;
#endif
#ifndef LIBXML_DEBUG_ENABLED
double xmlXPathStringEvalNumber(const xmlChar *str);
#endif

#ifdef  IS_BLANK
#undef	IS_BLANK
#endif
#define IS_BLANK(c) (((c) == 0x20) || ((c) == 0x09) || ((c) == 0xA) ||	\
                     ((c) == 0x0D))

#ifdef	IS_BLANK_NODE
#undef	IS_BLANK_NODE
#endif
#define IS_BLANK_NODE(n)						\
    (((n)->type == XML_TEXT_NODE) && (xsltIsBlank((n)->content)))

static void
xsltParseContentError(xsltStylesheetPtr style,
		       xmlNodePtr node)
{
    if ((style == NULL) || (node == NULL))
	return;

    if (IS_XSLT_ELEM(node))
	xsltTransformError(NULL, style, node,
	    "The XSLT-element '%s' is not allowed at this position.\n",
	    node->name);
    else
	xsltTransformError(NULL, style, node,
	    "The element '%s' is not allowed at this position.\n",
	    node->name);
    style->errors++;
}

#ifdef XSLT_REFACTORED
#else
static int
exclPrefixPush(xsltStylesheetPtr style, xmlChar * value)
{
    int i;

    if (style->exclPrefixMax == 0) {
        style->exclPrefixMax = 4;
        style->exclPrefixTab =
            (xmlChar * *)xmlMalloc(style->exclPrefixMax *
                                   sizeof(style->exclPrefixTab[0]));
        if (style->exclPrefixTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "malloc failed !\n");
            return (-1);
        }
    }
    
    for (i = 0;i < style->exclPrefixNr;i++) {
        if (xmlStrEqual(style->exclPrefixTab[i], value))
	    return(-1);
    }
    if (style->exclPrefixNr >= style->exclPrefixMax) {
        style->exclPrefixMax *= 2;
        style->exclPrefixTab =
            (xmlChar * *)xmlRealloc(style->exclPrefixTab,
                                    style->exclPrefixMax *
                                    sizeof(style->exclPrefixTab[0]));
        if (style->exclPrefixTab == NULL) {
            xmlGenericError(xmlGenericErrorContext, "realloc failed !\n");
            return (-1);
        }
    }
    style->exclPrefixTab[style->exclPrefixNr] = value;
    style->exclPrefix = value;
    return (style->exclPrefixNr++);
}
static xmlChar *
exclPrefixPop(xsltStylesheetPtr style)
{
    xmlChar *ret;

    if (style->exclPrefixNr <= 0)
        return (0);
    style->exclPrefixNr--;
    if (style->exclPrefixNr > 0)
        style->exclPrefix = style->exclPrefixTab[style->exclPrefixNr - 1];
    else
        style->exclPrefix = NULL;
    ret = style->exclPrefixTab[style->exclPrefixNr];
    style->exclPrefixTab[style->exclPrefixNr] = 0;
    return (ret);
}
#endif


static int initialized = 0;
void
xsltInit (void) {
    if (initialized == 0) {
	initialized = 1;
#ifdef XSLT_LOCALE_WINAPI
	xsltLocaleMutex = xmlNewRMutex();
#endif
        xsltRegisterAllExtras();
    }
}

void
xsltUninit (void) {
    initialized = 0;
}

int
xsltIsBlank(xmlChar *str) {
    if (str == NULL)
	return(1);
    while (*str != 0) {
	if (!(IS_BLANK(*str))) return(0);
	str++;
    }
    return(1);
}

static xsltDecimalFormatPtr
xsltNewDecimalFormat(xmlChar *name)
{
    xsltDecimalFormatPtr self;
    
    static const xmlChar permille[4] = {0xe2, 0x80, 0xb0, 0};

    self = xmlMalloc(sizeof(xsltDecimalFormat));
    if (self != NULL) {
	self->next = NULL;
	self->name = name;
	
	
	self->digit = xmlStrdup(BAD_CAST("#"));
	self->patternSeparator = xmlStrdup(BAD_CAST(";"));
	self->decimalPoint = xmlStrdup(BAD_CAST("."));
	self->grouping = xmlStrdup(BAD_CAST(","));
	self->percent = xmlStrdup(BAD_CAST("%"));
	self->permille = xmlStrdup(BAD_CAST(permille));
	self->zeroDigit = xmlStrdup(BAD_CAST("0"));
	self->minusSign = xmlStrdup(BAD_CAST("-"));
	self->infinity = xmlStrdup(BAD_CAST("Infinity"));
	self->noNumber = xmlStrdup(BAD_CAST("NaN"));
    }
    return self;
}

static void
xsltFreeDecimalFormat(xsltDecimalFormatPtr self)
{
    if (self != NULL) {
	if (self->digit)
	    xmlFree(self->digit);
	if (self->patternSeparator)
	    xmlFree(self->patternSeparator);
	if (self->decimalPoint)
	    xmlFree(self->decimalPoint);
	if (self->grouping)
	    xmlFree(self->grouping);
	if (self->percent)
	    xmlFree(self->percent);
	if (self->permille)
	    xmlFree(self->permille);
	if (self->zeroDigit)
	    xmlFree(self->zeroDigit);
	if (self->minusSign)
	    xmlFree(self->minusSign);
	if (self->infinity)
	    xmlFree(self->infinity);
	if (self->noNumber)
	    xmlFree(self->noNumber);
	if (self->name)
	    xmlFree(self->name);
	xmlFree(self);
    }
}

static void
xsltFreeDecimalFormatList(xsltStylesheetPtr self)
{
    xsltDecimalFormatPtr iter;
    xsltDecimalFormatPtr tmp;

    if (self == NULL)
	return;
    
    iter = self->decimalFormat;
    while (iter != NULL) {
	tmp = iter->next;
	xsltFreeDecimalFormat(iter);
	iter = tmp;
    }
}

xsltDecimalFormatPtr
xsltDecimalFormatGetByName(xsltStylesheetPtr style, xmlChar *name)
{
    xsltDecimalFormatPtr result = NULL;

    if (name == NULL)
	return style->decimalFormat;

    while (style != NULL) {
	for (result = style->decimalFormat->next;
	     result != NULL;
	     result = result->next) {
	    if (xmlStrEqual(name, result->name))
		return result;
	}
	style = xsltNextImport(style);
    }
    return result;
}


static xsltTemplatePtr
xsltNewTemplate(void) {
    xsltTemplatePtr cur;

    cur = (xsltTemplatePtr) xmlMalloc(sizeof(xsltTemplate));
    if (cur == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewTemplate : malloc failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xsltTemplate));
    cur->priority = XSLT_PAT_NO_PRIORITY;
    return(cur);
}

static void
xsltFreeTemplate(xsltTemplatePtr template) {
    if (template == NULL)
	return;
    if (template->match) xmlFree(template->match);
    if (template->inheritedNs) xmlFree(template->inheritedNs);
    memset(template, -1, sizeof(xsltTemplate));
    xmlFree(template);
}

static void
xsltFreeTemplateList(xsltTemplatePtr template) {
    xsltTemplatePtr cur;

    while (template != NULL) {
	cur = template;
	template = template->next;
	xsltFreeTemplate(cur);
    }
}

#ifdef XSLT_REFACTORED

static void
xsltFreeNsAliasList(xsltNsAliasPtr item)
{
    xsltNsAliasPtr tmp;
    
    while (item) {
	tmp = item;
	item = item->next;
	xmlFree(tmp);
    } 
    return;
}

#ifdef XSLT_REFACTORED_XSLT_NSCOMP
static void
xsltFreeNamespaceMap(xsltNsMapPtr item)
{
    xsltNsMapPtr tmp;
    
    while (item) {
	tmp = item;
	item = item->next;
	xmlFree(tmp);
    } 
    return;
}

static xsltNsMapPtr
xsltNewNamespaceMapItem(xsltCompilerCtxtPtr cctxt,
			xmlDocPtr doc,
			xmlNsPtr ns,
			xmlNodePtr elem)
{
    xsltNsMapPtr ret;

    if ((cctxt == NULL) || (doc == NULL) || (ns == NULL))
	return(NULL);

    ret = (xsltNsMapPtr) xmlMalloc(sizeof(xsltNsMap));
    if (ret == NULL) {
	xsltTransformError(NULL, cctxt->style, elem,
	    "Internal error: (xsltNewNamespaceMapItem) "
	    "memory allocation failed.\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltNsMap));
    ret->doc = doc;
    ret->ns = ns;
    ret->origNsName = ns->href;
    if (cctxt->psData->nsMap != NULL)
	ret->next = cctxt->psData->nsMap;
    cctxt->psData->nsMap = ret;

    return(ret);
}
#endif 

static void
xsltCompilerVarInfoFree(xsltCompilerCtxtPtr cctxt)
{
    xsltVarInfoPtr ivar = cctxt->ivars, ivartmp;    

    while (ivar) {
	ivartmp = ivar;
	ivar = ivar->next;
	xmlFree(ivartmp);
    }
}

static void
xsltCompilationCtxtFree(xsltCompilerCtxtPtr cctxt)
{    
    if (cctxt == NULL)
	return;
#ifdef WITH_XSLT_DEBUG_PARSING
    xsltGenericDebug(xsltGenericDebugContext,
	"Freeing compilation context\n");
    xsltGenericDebug(xsltGenericDebugContext,
	"### Max inodes: %d\n", cctxt->maxNodeInfos);
    xsltGenericDebug(xsltGenericDebugContext,
	"### Max LREs  : %d\n", cctxt->maxLREs);
#endif
    if (cctxt->inodeList != NULL) {
	xsltCompilerNodeInfoPtr tmp, cur = cctxt->inodeList;
	while (cur != NULL) {
	    tmp = cur;
	    cur = cur->next;
	    xmlFree(tmp);
	}
    }
    if (cctxt->tmpList != NULL)
	xsltPointerListFree(cctxt->tmpList);
#ifdef XSLT_REFACTORED_XPATHCOMP
    if (cctxt->xpathCtxt != NULL)
	xmlXPathFreeContext(cctxt->xpathCtxt);
#endif
    if (cctxt->nsAliases != NULL)
	xsltFreeNsAliasList(cctxt->nsAliases);

    if (cctxt->ivars)
	xsltCompilerVarInfoFree(cctxt);

    xmlFree(cctxt);
}

static xsltCompilerCtxtPtr
xsltCompilationCtxtCreate(xsltStylesheetPtr style) {
    xsltCompilerCtxtPtr ret;

    ret = (xsltCompilerCtxtPtr) xmlMalloc(sizeof(xsltCompilerCtxt));
    if (ret == NULL) {
	xsltTransformError(NULL, style, NULL,
	    "xsltCompilerCreate: allocation of compiler "
	    "context failed.\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltCompilerCtxt));

    ret->errSeverity = XSLT_ERROR_SEVERITY_ERROR;
    ret->tmpList = xsltPointerListCreate(20);
    if (ret->tmpList == NULL) {
	goto internal_err;
    }
#ifdef XSLT_REFACTORED_XPATHCOMP
    ret->xpathCtxt = xmlXPathNewContext(NULL);
    if (ret->xpathCtxt == NULL)
	goto internal_err;
#endif
    
    return(ret);

internal_err:
    xsltCompilationCtxtFree(ret);
    return(NULL);
}

static void
xsltLREEffectiveNsNodesFree(xsltEffectiveNsPtr first)
{
    xsltEffectiveNsPtr tmp;

    while (first != NULL) {
	tmp = first;
	first = first->nextInStore;
	xmlFree(tmp);
    }
}

static void
xsltFreePrincipalStylesheetData(xsltPrincipalStylesheetDataPtr data)
{
    if (data == NULL)
	return;

    if (data->inScopeNamespaces != NULL) {
	int i;
	xsltNsListContainerPtr nsi;
	xsltPointerListPtr list =
	    (xsltPointerListPtr) data->inScopeNamespaces;

	for (i = 0; i < list->number; i++) {
	    nsi = (xsltNsListContainerPtr) list->items[i];
	    if (nsi->list != NULL)
		xmlFree(nsi->list);
	    xmlFree(nsi);
	}
	xsltPointerListFree(list);
	data->inScopeNamespaces = NULL;
    }

    if (data->exclResultNamespaces != NULL) {
	int i;
	xsltPointerListPtr list = (xsltPointerListPtr)
	    data->exclResultNamespaces;	
	
	for (i = 0; i < list->number; i++)
	    xsltPointerListFree((xsltPointerListPtr) list->items[i]);
	
	xsltPointerListFree(list);
	data->exclResultNamespaces = NULL;
    }

    if (data->extElemNamespaces != NULL) {
	xsltPointerListPtr list = (xsltPointerListPtr)
	    data->extElemNamespaces;
	int i;

	for (i = 0; i < list->number; i++)
	    xsltPointerListFree((xsltPointerListPtr) list->items[i]);

	xsltPointerListFree(list);
	data->extElemNamespaces = NULL;
    }
    if (data->effectiveNs) {
	xsltLREEffectiveNsNodesFree(data->effectiveNs);
	data->effectiveNs = NULL;
    }
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
    xsltFreeNamespaceMap(data->nsMap);
#endif
    xmlFree(data);
}

static xsltPrincipalStylesheetDataPtr
xsltNewPrincipalStylesheetData(void)
{
    xsltPrincipalStylesheetDataPtr ret;

    ret = (xsltPrincipalStylesheetDataPtr)
	xmlMalloc(sizeof(xsltPrincipalStylesheetData));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
	    "xsltNewPrincipalStylesheetData: memory allocation failed.\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltPrincipalStylesheetData));
    
    
    ret->inScopeNamespaces = xsltPointerListCreate(-1);
    if (ret->inScopeNamespaces == NULL)
	goto internal_err;
    ret->exclResultNamespaces = xsltPointerListCreate(-1);
    if (ret->exclResultNamespaces == NULL)
	goto internal_err;
    
    ret->extElemNamespaces = xsltPointerListCreate(-1);
    if (ret->extElemNamespaces == NULL)
	goto internal_err;

    return(ret);

internal_err:

    return(NULL);
}

#endif

xsltStylesheetPtr
xsltNewStylesheet(void) {
    xsltStylesheetPtr ret = NULL;    

    ret = (xsltStylesheetPtr) xmlMalloc(sizeof(xsltStylesheet));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewStylesheet : malloc failed\n");
	goto internal_err;
    }
    memset(ret, 0, sizeof(xsltStylesheet));

    ret->omitXmlDeclaration = -1;
    ret->standalone = -1;
    ret->decimalFormat = xsltNewDecimalFormat(NULL);
    ret->indent = -1;
    ret->errors = 0;
    ret->warnings = 0;
    ret->exclPrefixNr = 0;
    ret->exclPrefixMax = 0;
    ret->exclPrefixTab = NULL;
    ret->extInfos = NULL;
    ret->extrasNr = 0;
    ret->internalized = 1;
    ret->literal_result = 0;
    ret->dict = xmlDictCreate();
#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
	"creating dictionary for stylesheet\n");
#endif

    xsltInit();

    return(ret);

internal_err:
    if (ret != NULL)
	xsltFreeStylesheet(ret);
    return(NULL);
}

int
xsltAllocateExtra(xsltStylesheetPtr style)
{
    return(style->extrasNr++);
}

int
xsltAllocateExtraCtxt(xsltTransformContextPtr ctxt)
{
    if (ctxt->extrasNr >= ctxt->extrasMax) {
	int i;
	if (ctxt->extrasNr == 0) {
	    ctxt->extrasMax = 20;
	    ctxt->extras = (xsltRuntimeExtraPtr)
		xmlMalloc(ctxt->extrasMax * sizeof(xsltRuntimeExtra));
	    if (ctxt->extras == NULL) {
		xmlGenericError(xmlGenericErrorContext,
			"xsltAllocateExtraCtxt: out of memory\n");
		ctxt->state = XSLT_STATE_ERROR;
		return(0);
	    }
	    for (i = 0;i < ctxt->extrasMax;i++) {
		ctxt->extras[i].info = NULL;
		ctxt->extras[i].deallocate = NULL;
		ctxt->extras[i].val.ptr = NULL;
	    }

	} else {
	    xsltRuntimeExtraPtr tmp;

	    ctxt->extrasMax += 100;
	    tmp = (xsltRuntimeExtraPtr) xmlRealloc(ctxt->extras,
		            ctxt->extrasMax * sizeof(xsltRuntimeExtra));
	    if (tmp == NULL) {
		xmlGenericError(xmlGenericErrorContext,
			"xsltAllocateExtraCtxt: out of memory\n");
		ctxt->state = XSLT_STATE_ERROR;
		return(0);
	    }
	    ctxt->extras = tmp;
	    for (i = ctxt->extrasNr;i < ctxt->extrasMax;i++) {
		ctxt->extras[i].info = NULL;
		ctxt->extras[i].deallocate = NULL;
		ctxt->extras[i].val.ptr = NULL;
	    }
	}
    }
    return(ctxt->extrasNr++);
}

static void
xsltFreeStylesheetList(xsltStylesheetPtr style) {
    xsltStylesheetPtr next;

    while (style != NULL) {
	next = style->next;
	xsltFreeStylesheet(style);
	style = next;
    }
}

static int
xsltCleanupStylesheetTree(xmlDocPtr doc ATTRIBUTE_UNUSED,
			  xmlNodePtr rootElem ATTRIBUTE_UNUSED)
{    
#if 0 
    xmlNodePtr cur;

    if ((doc == NULL) || (rootElem == NULL) ||
	(rootElem->type != XML_ELEMENT_NODE) ||
	(doc != rootElem->doc))
	return(-1);


    cur = rootElem;
    while (cur != NULL) {
	if (cur->type == XML_ELEMENT_NODE) {
	    cur->psvi = NULL;
	    if (cur->children) {
		cur = cur->children;
		continue;
	    }
	}

leave_node:
	if (cur == rootElem)
	    break;
	if (cur->next != NULL)
	    cur = cur->next;
	else {
	    cur = cur->parent;
	    if (cur == NULL)
		break;
	    goto leave_node;
	}
    }
#endif 
    return(0);
}

void
xsltFreeStylesheet(xsltStylesheetPtr style)
{
    if (style == NULL)
        return;
    
#ifdef XSLT_REFACTORED
    if ((style->principal == style) && (style->doc))
	xsltCleanupStylesheetTree(style->doc,
	    xmlDocGetRootElement(style->doc));
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
    if ((style->doc != NULL) &&
	XSLT_HAS_INTERNAL_NSMAP(style))
    {
	xsltRestoreDocumentNamespaces(XSLT_GET_INTERNAL_NSMAP(style),
	    style->doc);	
    }
#endif 
#else
    if ((style->parent == NULL) && (style->doc))
	xsltCleanupStylesheetTree(style->doc,
	    xmlDocGetRootElement(style->doc));
#endif 

    xsltFreeKeys(style);
    xsltFreeExts(style);
    xsltFreeTemplateHashes(style);
    xsltFreeDecimalFormatList(style);
    xsltFreeTemplateList(style->templates);
    xsltFreeAttributeSetsHashes(style);
    xsltFreeNamespaceAliasHashes(style);
    xsltFreeStylePreComps(style);
    xsltFreeStyleDocuments(style);
    xsltShutdownExts(style);
       
    if (style->variables != NULL)
        xsltFreeStackElemList(style->variables);
    if (style->cdataSection != NULL)
        xmlHashFree(style->cdataSection, NULL);
    if (style->stripSpaces != NULL)
        xmlHashFree(style->stripSpaces, NULL);
    if (style->nsHash != NULL)
        xmlHashFree(style->nsHash, NULL);
    if (style->exclPrefixTab != NULL)
        xmlFree(style->exclPrefixTab);
    if (style->method != NULL)
        xmlFree(style->method);
    if (style->methodURI != NULL)
        xmlFree(style->methodURI);
    if (style->version != NULL)
        xmlFree(style->version);
    if (style->encoding != NULL)
        xmlFree(style->encoding);
    if (style->doctypePublic != NULL)
        xmlFree(style->doctypePublic);
    if (style->doctypeSystem != NULL)
        xmlFree(style->doctypeSystem);
    if (style->mediaType != NULL)
        xmlFree(style->mediaType);
    if (style->attVTs)
        xsltFreeAVTList(style->attVTs);
    if (style->imports != NULL)
        xsltFreeStylesheetList(style->imports);

#ifdef XSLT_REFACTORED
    if (style->principal == style) {
	if (style->principalData) {
	    xsltFreePrincipalStylesheetData(style->principalData);
	    style->principalData = NULL;
	}
    }    
#endif
    if (style->doc != NULL) {	
        xmlFreeDoc(style->doc);
    }

#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
                     "freeing dictionary from stylesheet\n");
#endif
    xmlDictFree(style->dict);

    memset(style, -1, sizeof(xsltStylesheet));
    xmlFree(style);
}


#ifdef XSLT_REFACTORED
#else
static int
xsltGetInheritedNsList(xsltStylesheetPtr style,
	               xsltTemplatePtr template,
	               xmlNodePtr node)
{
    xmlNsPtr cur;
    xmlNsPtr *ret = NULL;
    int nbns = 0;
    int maxns = 10;
    int i;    

    if ((style == NULL) || (template == NULL) || (node == NULL) ||
	(template->inheritedNsNr != 0) || (template->inheritedNs != NULL))
	return(0);
    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE) {
            cur = node->nsDef;
            while (cur != NULL) {
		if (xmlStrEqual(cur->href, XSLT_NAMESPACE))
		    goto skip_ns;

		if ((cur->prefix != NULL) &&
		    (xsltCheckExtPrefix(style, cur->prefix)))
		    goto skip_ns;
		for (i = 0;i < style->exclPrefixNr;i++) {
		    if (xmlStrEqual(cur->href, style->exclPrefixTab[i]))
			goto skip_ns;
		}
                if (ret == NULL) {
                    ret =
                        (xmlNsPtr *) xmlMalloc((maxns + 1) *
                                               sizeof(xmlNsPtr));
                    if (ret == NULL) {
                        xmlGenericError(xmlGenericErrorContext,
                                        "xsltGetInheritedNsList : out of memory!\n");
                        return(0);
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
                            xmlGenericError(xmlGenericErrorContext,
                                            "xsltGetInheritedNsList : realloc failed!\n");
                            return(0);
                        }
                    }
                    ret[nbns++] = cur;
                    ret[nbns] = NULL;
                }
skip_ns:
                cur = cur->next;
            }
        }
        node = node->parent;
    }
    if (nbns != 0) {
#ifdef WITH_XSLT_DEBUG_PARSING
        xsltGenericDebug(xsltGenericDebugContext,
                         "template has %d inherited namespaces\n", nbns);
#endif
	template->inheritedNsNr = nbns;
	template->inheritedNs = ret;
    }
    return (nbns);
}
#endif 


void
xsltParseStylesheetOutput(xsltStylesheetPtr style, xmlNodePtr cur)
{
    xmlChar *elements,
     *prop;
    xmlChar *element,
     *end;

    if ((cur == NULL) || (style == NULL))
        return;
   
    prop = xmlGetNsProp(cur, (const xmlChar *) "version", NULL);
    if (prop != NULL) {
        if (style->version != NULL)
            xmlFree(style->version);
        style->version = prop;
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "encoding", NULL);
    if (prop != NULL) {
        if (style->encoding != NULL)
            xmlFree(style->encoding);
        style->encoding = prop;
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "method", NULL);
    if (prop != NULL) {
        const xmlChar *URI;

        if (style->method != NULL)
            xmlFree(style->method);
        style->method = NULL;
        if (style->methodURI != NULL)
            xmlFree(style->methodURI);
        style->methodURI = NULL;

	URI = xsltGetQNameURI(cur, &prop);
	if (prop == NULL) {
	    if (style != NULL) style->errors++;
	} else if (URI == NULL) {
            if ((xmlStrEqual(prop, (const xmlChar *) "xml")) ||
                (xmlStrEqual(prop, (const xmlChar *) "html")) ||
                (xmlStrEqual(prop, (const xmlChar *) "text"))) {
                style->method = prop;
            } else {
		xsltTransformError(NULL, style, cur,
                                 "invalid value for method: %s\n", prop);
                if (style != NULL) style->warnings++;
            }
	} else {
	    style->method = prop;
	    style->methodURI = xmlStrdup(URI);
	}
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "doctype-system", NULL);
    if (prop != NULL) {
        if (style->doctypeSystem != NULL)
            xmlFree(style->doctypeSystem);
        style->doctypeSystem = prop;
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "doctype-public", NULL);
    if (prop != NULL) {
        if (style->doctypePublic != NULL)
            xmlFree(style->doctypePublic);
        style->doctypePublic = prop;
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "standalone", NULL);
    if (prop != NULL) {
        if (xmlStrEqual(prop, (const xmlChar *) "yes")) {
            style->standalone = 1;
        } else if (xmlStrEqual(prop, (const xmlChar *) "no")) {
            style->standalone = 0;
        } else {
	    xsltTransformError(NULL, style, cur,
                             "invalid value for standalone: %s\n", prop);
            style->errors++;
        }
        xmlFree(prop);
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "indent", NULL);
    if (prop != NULL) {
        if (xmlStrEqual(prop, (const xmlChar *) "yes")) {
            style->indent = 1;
        } else if (xmlStrEqual(prop, (const xmlChar *) "no")) {
            style->indent = 0;
        } else {
	    xsltTransformError(NULL, style, cur,
                             "invalid value for indent: %s\n", prop);
            style->errors++;
        }
        xmlFree(prop);
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "omit-xml-declaration", NULL);
    if (prop != NULL) {
        if (xmlStrEqual(prop, (const xmlChar *) "yes")) {
            style->omitXmlDeclaration = 1;
        } else if (xmlStrEqual(prop, (const xmlChar *) "no")) {
            style->omitXmlDeclaration = 0;
        } else {
	    xsltTransformError(NULL, style, cur,
                             "invalid value for omit-xml-declaration: %s\n",
                             prop);
            style->errors++;
        }
        xmlFree(prop);
    }

    elements = xmlGetNsProp(cur, (const xmlChar *) "cdata-section-elements",
	NULL);
    if (elements != NULL) {
        if (style->cdataSection == NULL)
            style->cdataSection = xmlHashCreate(10);
        if (style->cdataSection == NULL)
            return;

        element = elements;
        while (*element != 0) {
            while (IS_BLANK(*element))
                element++;
            if (*element == 0)
                break;
            end = element;
            while ((*end != 0) && (!IS_BLANK(*end)))
                end++;
            element = xmlStrndup(element, end - element);
            if (element) {		
#ifdef WITH_XSLT_DEBUG_PARSING
                xsltGenericDebug(xsltGenericDebugContext,
                                 "add cdata section output element %s\n",
                                 element);
#endif
		if (xmlValidateQName(BAD_CAST element, 0) != 0) {
		    xsltTransformError(NULL, style, cur,
			"Attribute 'cdata-section-elements': The value "
			"'%s' is not a valid QName.\n", element);
		    xmlFree(element);
		    style->errors++;
		} else {
		    const xmlChar *URI;

		    URI = xsltGetQNameURI(cur, &element);
		    if (element == NULL) {
			xsltTransformError(NULL, style, cur,
			    "Attribute 'cdata-section-elements': The value "
			    "'%s' is not a valid QName.\n", element);
			style->errors++;
		    } else {
			xmlNsPtr ns;
			
			if (URI == NULL) {
			    ns = xmlSearchNs(style->doc, cur, NULL);
			    if (ns != NULL)
				URI = ns->href;
			}		   
			xmlHashAddEntry2(style->cdataSection, element, URI,
			    (void *) "cdata");
			xmlFree(element);
		    }
		}
            }
            element = end;
        }
        xmlFree(elements);
    }

    prop = xmlGetNsProp(cur, (const xmlChar *) "media-type", NULL);
    if (prop != NULL) {
	if (style->mediaType)
	    xmlFree(style->mediaType);
	style->mediaType = prop;
    }
    if (cur->children != NULL) {
	xsltParseContentError(style, cur->children);
    }
}

static void
xsltParseStylesheetDecimalFormat(xsltStylesheetPtr style, xmlNodePtr cur)
{
    xmlChar *prop;
    xsltDecimalFormatPtr format;
    xsltDecimalFormatPtr iter;
    
    if ((cur == NULL) || (style == NULL))
	return;

    format = style->decimalFormat;
    
    prop = xmlGetNsProp(cur, BAD_CAST("name"), NULL);
    if (prop != NULL) {
	format = xsltDecimalFormatGetByName(style, prop);
	if (format != NULL) {
	    xsltTransformError(NULL, style, cur,
	 "xsltParseStylestyleDecimalFormat: %s already exists\n", prop);
	    if (style != NULL) style->warnings++;
	    return;
	}
	format = xsltNewDecimalFormat(prop);
	if (format == NULL) {
	    xsltTransformError(NULL, style, cur,
     "xsltParseStylestyleDecimalFormat: failed creating new decimal-format\n");
	    if (style != NULL) style->errors++;
	    return;
	}
	
	for (iter = style->decimalFormat; iter->next; iter = iter->next)
	    ;
	if (iter)
	    iter->next = format;
    }

    prop = xmlGetNsProp(cur, (const xmlChar *)"decimal-separator", NULL);
    if (prop != NULL) {
	if (format->decimalPoint != NULL) xmlFree(format->decimalPoint);
	format->decimalPoint  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"grouping-separator", NULL);
    if (prop != NULL) {
	if (format->grouping != NULL) xmlFree(format->grouping);
	format->grouping  = prop;
    }

    prop = xmlGetNsProp(cur, (const xmlChar *)"infinity", NULL);
    if (prop != NULL) {
	if (format->infinity != NULL) xmlFree(format->infinity);
	format->infinity  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"minus-sign", NULL);
    if (prop != NULL) {
	if (format->minusSign != NULL) xmlFree(format->minusSign);
	format->minusSign  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"NaN", NULL);
    if (prop != NULL) {
	if (format->noNumber != NULL) xmlFree(format->noNumber);
	format->noNumber  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"percent", NULL);
    if (prop != NULL) {
	if (format->percent != NULL) xmlFree(format->percent);
	format->percent  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"per-mille", NULL);
    if (prop != NULL) {
	if (format->permille != NULL) xmlFree(format->permille);
	format->permille  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"zero-digit", NULL);
    if (prop != NULL) {
	if (format->zeroDigit != NULL) xmlFree(format->zeroDigit);
	format->zeroDigit  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"digit", NULL);
    if (prop != NULL) {
	if (format->digit != NULL) xmlFree(format->digit);
	format->digit  = prop;
    }
    
    prop = xmlGetNsProp(cur, (const xmlChar *)"pattern-separator", NULL);
    if (prop != NULL) {
	if (format->patternSeparator != NULL) xmlFree(format->patternSeparator);
	format->patternSeparator  = prop;
    }
    if (cur->children != NULL) {
	xsltParseContentError(style, cur->children);
    }
}


static void
xsltParseStylesheetPreserveSpace(xsltStylesheetPtr style, xmlNodePtr cur) {
    xmlChar *elements;
    xmlChar *element, *end;

    if ((cur == NULL) || (style == NULL))
	return;

    elements = xmlGetNsProp(cur, (const xmlChar *)"elements", NULL);
    if (elements == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsltParseStylesheetPreserveSpace: missing elements attribute\n");
	if (style != NULL) style->warnings++;
	return;
    }

    if (style->stripSpaces == NULL)
	style->stripSpaces = xmlHashCreate(10);
    if (style->stripSpaces == NULL)
	return;

    element = elements;
    while (*element != 0) {
	while (IS_BLANK(*element)) element++;
	if (*element == 0)
	    break;
        end = element;
	while ((*end != 0) && (!IS_BLANK(*end))) end++;
	element = xmlStrndup(element, end - element);
	if (element) {
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"add preserved space element %s\n", element);
#endif
	    if (xmlStrEqual(element, (const xmlChar *)"*")) {
		style->stripAll = -1;
	    } else {
		const xmlChar *URI;

                URI = xsltGetQNameURI(cur, &element);

		xmlHashAddEntry2(style->stripSpaces, element, URI,
				(xmlChar *) "preserve");
	    }
	    xmlFree(element);
	}
	element = end;
    }
    xmlFree(elements);
    if (cur->children != NULL) {
	xsltParseContentError(style, cur->children);
    }
}

#ifdef XSLT_REFACTORED
#else
static void
xsltParseStylesheetExtPrefix(xsltStylesheetPtr style, xmlNodePtr cur,
			     int isXsltElem) {
    xmlChar *prefixes;
    xmlChar *prefix, *end;

    if ((cur == NULL) || (style == NULL))
	return;

    if (isXsltElem) {
	
	prefixes = xmlGetNsProp(cur,
	    (const xmlChar *)"extension-element-prefixes", NULL);
    } else {
	
	prefixes = xmlGetNsProp(cur,
	    (const xmlChar *)"extension-element-prefixes", XSLT_NAMESPACE);
    }
    if (prefixes == NULL) {
	return;
    }

    prefix = prefixes;
    while (*prefix != 0) {
	while (IS_BLANK(*prefix)) prefix++;
	if (*prefix == 0)
	    break;
        end = prefix;
	while ((*end != 0) && (!IS_BLANK(*end))) end++;
	prefix = xmlStrndup(prefix, end - prefix);
	if (prefix) {
	    xmlNsPtr ns;

	    if (xmlStrEqual(prefix, (const xmlChar *)"#default"))
		ns = xmlSearchNs(style->doc, cur, NULL);
	    else
		ns = xmlSearchNs(style->doc, cur, prefix);
	    if (ns == NULL) {
		xsltTransformError(NULL, style, cur,
	    "xsl:extension-element-prefix : undefined namespace %s\n",
	                         prefix);
		if (style != NULL) style->warnings++;
	    } else {
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
		    "add extension prefix %s\n", prefix);
#endif
		xsltRegisterExtPrefix(style, prefix, ns->href);
	    }
	    xmlFree(prefix);
	}
	prefix = end;
    }
    xmlFree(prefixes);
}
#endif 


static void
xsltParseStylesheetStripSpace(xsltStylesheetPtr style, xmlNodePtr cur) {
    xmlChar *elements;
    xmlChar *element, *end;

    if ((cur == NULL) || (style == NULL))
	return;

    elements = xmlGetNsProp(cur, (const xmlChar *)"elements", NULL);
    if (elements == NULL) {
	xsltTransformError(NULL, style, cur,
	    "xsltParseStylesheetStripSpace: missing elements attribute\n");
	if (style != NULL) style->warnings++;
	return;
    }

    if (style->stripSpaces == NULL)
	style->stripSpaces = xmlHashCreate(10);
    if (style->stripSpaces == NULL)
	return;

    element = elements;
    while (*element != 0) {
	while (IS_BLANK(*element)) element++;
	if (*element == 0)
	    break;
        end = element;
	while ((*end != 0) && (!IS_BLANK(*end))) end++;
	element = xmlStrndup(element, end - element);
	if (element) {
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"add stripped space element %s\n", element);
#endif
	    if (xmlStrEqual(element, (const xmlChar *)"*")) {
		style->stripAll = 1;
	    } else {
		const xmlChar *URI;

                URI = xsltGetQNameURI(cur, &element);

		xmlHashAddEntry2(style->stripSpaces, element, URI,
			        (xmlChar *) "strip");
	    }
	    xmlFree(element);
	}
	element = end;
    }
    xmlFree(elements);
    if (cur->children != NULL) {
	xsltParseContentError(style, cur->children);
    }
}

#ifdef XSLT_REFACTORED
#else

static int
xsltParseStylesheetExcludePrefix(xsltStylesheetPtr style, xmlNodePtr cur,
				 int isXsltElem)
{
    int nb = 0;
    xmlChar *prefixes;
    xmlChar *prefix, *end;

    if ((cur == NULL) || (style == NULL))
	return(0);

    if (isXsltElem)
	prefixes = xmlGetNsProp(cur,
	    (const xmlChar *)"exclude-result-prefixes", NULL);
    else
	prefixes = xmlGetNsProp(cur,
	    (const xmlChar *)"exclude-result-prefixes", XSLT_NAMESPACE);

    if (prefixes == NULL) {
	return(0);
    }

    prefix = prefixes;
    while (*prefix != 0) {
	while (IS_BLANK(*prefix)) prefix++;
	if (*prefix == 0)
	    break;
        end = prefix;
	while ((*end != 0) && (!IS_BLANK(*end))) end++;
	prefix = xmlStrndup(prefix, end - prefix);
	if (prefix) {
	    xmlNsPtr ns;

	    if (xmlStrEqual(prefix, (const xmlChar *)"#default"))
		ns = xmlSearchNs(style->doc, cur, NULL);
	    else
		ns = xmlSearchNs(style->doc, cur, prefix);
	    if (ns == NULL) {
		xsltTransformError(NULL, style, cur,
	    "xsl:exclude-result-prefixes : undefined namespace %s\n",
	                         prefix);
		if (style != NULL) style->warnings++;
	    } else {
		if (exclPrefixPush(style, (xmlChar *) ns->href) >= 0) {
#ifdef WITH_XSLT_DEBUG_PARSING
		    xsltGenericDebug(xsltGenericDebugContext,
			"exclude result prefix %s\n", prefix);
#endif
		    nb++;
		}
	    }
	    xmlFree(prefix);
	}
	prefix = end;
    }
    xmlFree(prefixes);
    return(nb);
}
#endif 

#ifdef XSLT_REFACTORED

static xmlNsPtr
xsltTreeEnsureXMLDecl(xmlDocPtr doc)
{
    if (doc == NULL)
	return (NULL);
    if (doc->oldNs != NULL)
	return (doc->oldNs);
    {
	xmlNsPtr ns;
	ns = (xmlNsPtr) xmlMalloc(sizeof(xmlNs));
	if (ns == NULL) {
	    xmlGenericError(xmlGenericErrorContext,
		"xsltTreeEnsureXMLDecl: Failed to allocate "
		"the XML namespace.\n");	
	    return (NULL);
	}
	memset(ns, 0, sizeof(xmlNs));
	ns->type = XML_LOCAL_NAMESPACE;
#ifdef LIBXML_NAMESPACE_DICT
	if (doc->dict)
	    ns->href = xmlDictLookup(doc->dict, XML_XML_NAMESPACE, -1);
	else
	    ns->href = xmlStrdup(XML_XML_NAMESPACE);
#else
	ns->href = xmlStrdup(XML_XML_NAMESPACE); 
#endif
	ns->prefix = xmlStrdup((const xmlChar *)"xml");
	doc->oldNs = ns;
	return (ns);
    }
}

static xmlNsPtr
xsltTreeAcquireStoredNs(xmlDocPtr doc,
			const xmlChar *nsName,
			const xmlChar *prefix)
{
    xmlNsPtr ns;

    if (doc == NULL)
	return (NULL);
    if (doc->oldNs != NULL)
	ns = doc->oldNs;
    else
	ns = xsltTreeEnsureXMLDecl(doc);
    if (ns == NULL)
	return (NULL);
    if (ns->next != NULL) {
	
	ns = ns->next;
	while (ns != NULL) {
	    if ((ns->prefix == NULL) != (prefix == NULL)) {
		
	    } else if (prefix == NULL) {
		if (xmlStrEqual(ns->href, nsName))
		    return (ns);
	    } else {
		if ((ns->prefix[0] == prefix[0]) &&
		     xmlStrEqual(ns->prefix, prefix) &&
		     xmlStrEqual(ns->href, nsName))
		    return (ns);
		
	    }
	    if (ns->next == NULL)
		break;
	    ns = ns->next;
	}
    }
    
    ns->next = xmlNewNs(NULL, nsName, prefix);
    return (ns->next);
}

static int
xsltLREBuildEffectiveNs(xsltCompilerCtxtPtr cctxt,
			xmlNodePtr elem)
{
    xmlNsPtr ns;
    xsltNsAliasPtr alias;

    if ((cctxt == NULL) || (elem == NULL))
	return(-1);
    if ((cctxt->nsAliases == NULL) || (! cctxt->hasNsAliases))
	return(0);

    alias = cctxt->nsAliases;    		
    while (alias != NULL) {
	if ( 
	    ( (elem->ns == NULL) &&
	    ((alias->literalNs == NULL) ||
	    (alias->literalNs->href == NULL)) ) ||
	    
	    ( (elem->ns != NULL) &&
	    (alias->literalNs != NULL) &&
	    xmlStrEqual(elem->ns->href, alias->literalNs->href) ) )
	{
	    if ((alias->targetNs != NULL) &&
		(alias->targetNs->href != NULL))
	    {
		if (elem->doc == alias->docOfTargetNs) {
		    elem->ns = alias->targetNs;
		} else {
		    ns = xmlSearchNs(elem->doc, elem,
			alias->targetNs->prefix);		    
		    if ((ns == NULL) ||
			(! xmlStrEqual(ns->href, alias->targetNs->href)))
		    {
			ns = xsltTreeAcquireStoredNs(elem->doc,
			    alias->targetNs->href,
			    alias->targetNs->prefix);
			
			if (ns == NULL) {
			    xsltTransformError(NULL, cctxt->style, elem,
				"Internal error in "
				"xsltLREBuildEffectiveNs(): "
				"failed to acquire a stored "
				"ns-declaration.\n");
			    cctxt->style->errors++;
			    return(-1);
			    
			}
		    }
		    elem->ns = ns;
		}		   
	    } else {
		elem->ns = NULL;
	    }
	    break;
	}
	alias = alias->next;
    }
    if (elem->properties != NULL) {
	xmlAttrPtr attr = elem->properties;
	
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		attr = attr->next;
		continue;
	    }
	    alias = cctxt->nsAliases;
	    while (alias != NULL) {
		if ( 
		    ( (elem->ns == NULL) &&
		    ((alias->literalNs == NULL) ||
		    (alias->literalNs->href == NULL)) ) ||
		    
		    ( (elem->ns != NULL) &&
		    (alias->literalNs != NULL) &&
		    xmlStrEqual(elem->ns->href, alias->literalNs->href) ) )
		{
		    if ((alias->targetNs != NULL) &&
			(alias->targetNs->href != NULL))
		    {		    
			if (elem->doc == alias->docOfTargetNs) {
			    elem->ns = alias->targetNs;
			} else {
			    ns = xmlSearchNs(elem->doc, elem,
				alias->targetNs->prefix);
			    if ((ns == NULL) ||
				(! xmlStrEqual(ns->href, alias->targetNs->href)))
			    {
				ns = xsltTreeAcquireStoredNs(elem->doc,
				    alias->targetNs->href,
				    alias->targetNs->prefix);
				
				if (ns == NULL) {
				    xsltTransformError(NULL, cctxt->style, elem,
					"Internal error in "
					"xsltLREBuildEffectiveNs(): "
					"failed to acquire a stored "
					"ns-declaration.\n");
				    cctxt->style->errors++;
				    return(-1);
				    
				}
			    }
			    elem->ns = ns;
			}
		    } else {
			elem->ns = NULL;
		    }
		    break;
		}
		alias = alias->next;
	    }
	    
	    attr = attr->next;
	}
    }
    return(0);
}

static int
xsltLREBuildEffectiveNsNodes(xsltCompilerCtxtPtr cctxt,
			     xsltStyleItemLRElementInfoPtr item,
			     xmlNodePtr elem,
			     int isLRE)
{
    xmlNsPtr ns, tmpns;
    xsltEffectiveNsPtr effNs, lastEffNs = NULL;
    int i, j, holdByElem;
    xsltPointerListPtr extElemNs = cctxt->inode->extElemNs;
    xsltPointerListPtr exclResultNs = cctxt->inode->exclResultNs;

    if ((cctxt == NULL) || (cctxt->inode == NULL) || (elem == NULL) ||
	(item == NULL) || (item->effectiveNs != NULL))
	return(-1);

    if (item->inScopeNs == NULL)    
	return(0);

    extElemNs = cctxt->inode->extElemNs;
    exclResultNs = cctxt->inode->exclResultNs;

    for (i = 0; i < item->inScopeNs->totalNumber; i++) {
	ns = item->inScopeNs->list[i];
	if (xmlStrEqual(ns->href, XSLT_NAMESPACE))
	    goto skip_ns;

	if (cctxt->hasNsAliases) {
	    xsltNsAliasPtr alias;
	    alias = cctxt->nsAliases;
	    do {
		if ((alias->targetNs != NULL) &&
		    (xmlStrEqual(alias->targetNs->href, ns->href)))
		{
		    goto add_effective_ns;
		}
		alias = alias->next;
	    } while (alias != NULL);

	    alias = cctxt->nsAliases;
	    do {
		if ((alias->literalNs != NULL) &&
		    (xmlStrEqual(alias->literalNs->href, ns->href)))
		{
		    goto skip_ns;
		}
		alias = alias->next;
	    } while (alias != NULL);
	}
	
	if (exclResultNs) {
	    for (j = 0; j < exclResultNs->number; j++)
		if (xmlStrEqual(ns->href, BAD_CAST exclResultNs->items[j]))
		    goto skip_ns;
	}
	if (extElemNs) {
	    for (j = 0; j < extElemNs->number; j++)
		if (xmlStrEqual(ns->href, BAD_CAST extElemNs->items[j]))
		    goto skip_ns;
	}

add_effective_ns:
	if (isLRE && (elem->nsDef != NULL)) {
	    holdByElem = 0;
	    tmpns = elem->nsDef;
	    do {
		if (tmpns == ns) {
		    holdByElem = 1;
		    break;
		}
		tmpns = tmpns->next;
	    } while (tmpns != NULL);	    
	} else
	    holdByElem = 0;
	
	
	effNs = (xsltEffectiveNsPtr) xmlMalloc(sizeof(xsltEffectiveNs));
	if (effNs == NULL) {
	    xsltTransformError(NULL, cctxt->style, elem,
		"Internal error in xsltLREBuildEffectiveNs(): "
		"failed to allocate memory.\n");
	    cctxt->style->errors++;
	    return(-1);
	}
	if (cctxt->psData->effectiveNs == NULL) {
	    cctxt->psData->effectiveNs = effNs;
	    effNs->nextInStore = NULL;	 
	} else {
	    effNs->nextInStore = cctxt->psData->effectiveNs;
	    cctxt->psData->effectiveNs = effNs;
	}

	effNs->next = NULL;
	effNs->prefix = ns->prefix;
	effNs->nsName = ns->href;
	effNs->holdByElem = holdByElem;
	
	if (lastEffNs == NULL)
	    item->effectiveNs = effNs;
	else
	    lastEffNs->next = effNs;
	lastEffNs = effNs;
	
skip_ns:
	{}
    }
    return(0);
}


static int
xsltLREInfoCreate(xsltCompilerCtxtPtr cctxt,
		  xmlNodePtr elem,
		  int isLRE)
{
    xsltStyleItemLRElementInfoPtr item;

    if ((cctxt == NULL) || (cctxt->inode == NULL))
	return(-1);

    item = (xsltStyleItemLRElementInfoPtr)
	xmlMalloc(sizeof(xsltStyleItemLRElementInfo));
    if (item == NULL) {
	xsltTransformError(NULL, cctxt->style, NULL,
	    "Internal error in xsltLREInfoCreate(): "
	    "memory allocation failed.\n");
	cctxt->style->errors++;
	return(-1);
    }
    memset(item, 0, sizeof(xsltStyleItemLRElementInfo));
    item->type = XSLT_FUNC_LITERAL_RESULT_ELEMENT;
    item->next = cctxt->style->preComps;
    cctxt->style->preComps = (xsltElemPreCompPtr) item;
    item->inScopeNs = cctxt->inode->inScopeNs;
    
    if (elem)
	xsltLREBuildEffectiveNsNodes(cctxt, item, elem, isLRE);

    cctxt->inode->litResElemInfo = item;
    cctxt->inode->nsChanged = 0;
    cctxt->maxLREs++;
    return(0);
}

 
static xsltVarInfoPtr
xsltCompilerVarInfoPush(xsltCompilerCtxtPtr cctxt,
				  xmlNodePtr inst,
				  const xmlChar *name,
				  const xmlChar *nsName)
{
    xsltVarInfoPtr ivar;

    if ((cctxt->ivar != NULL) && (cctxt->ivar->next != NULL)) {
	ivar = cctxt->ivar->next;
    } else if ((cctxt->ivar == NULL) && (cctxt->ivars != NULL)) {
	ivar = cctxt->ivars;
    } else {
	ivar = (xsltVarInfoPtr) xmlMalloc(sizeof(xsltVarInfo));
	if (ivar == NULL) {
	    xsltTransformError(NULL, cctxt->style, inst,
		"xsltParseInScopeVarPush: xmlMalloc() failed!\n");
	    cctxt->style->errors++;
	    return(NULL);
	}
	
	if (cctxt->ivars == NULL) {
	    cctxt->ivars = ivar;
	    ivar->prev = NULL;
	} else {
	    cctxt->ivar->next = ivar;
	    ivar->prev = cctxt->ivar;
	}
	cctxt->ivar = ivar;
	ivar->next = NULL;
    }
    ivar->depth = cctxt->depth;
    ivar->name = name;
    ivar->nsName = nsName;
    return(ivar);
}

 
static void
xsltCompilerVarInfoPop(xsltCompilerCtxtPtr cctxt)
{

    while ((cctxt->ivar != NULL) &&
	(cctxt->ivar->depth > cctxt->depth))
    {
	cctxt->ivar = cctxt->ivar->prev;
    }
}

static xsltCompilerNodeInfoPtr
xsltCompilerNodePush(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{    
    xsltCompilerNodeInfoPtr inode, iprev;

    if ((cctxt->inode != NULL) && (cctxt->inode->next != NULL)) {	
	inode = cctxt->inode->next;
    } else if ((cctxt->inode == NULL) && (cctxt->inodeList != NULL)) {
	inode = cctxt->inodeList;	
    } else {
	inode = (xsltCompilerNodeInfoPtr)
	    xmlMalloc(sizeof(xsltCompilerNodeInfo));
	if (inode == NULL) {
	    xsltTransformError(NULL, cctxt->style, NULL,
		"xsltCompilerNodePush: malloc failed.\n");
	    return(NULL);
	}
	memset(inode, 0, sizeof(xsltCompilerNodeInfo));
	if (cctxt->inodeList == NULL)
	    cctxt->inodeList = inode;
	else {
	    cctxt->inodeLast->next = inode;
	    inode->prev = cctxt->inodeLast;
	}
	cctxt->inodeLast = inode;
	cctxt->maxNodeInfos++;	
	if (cctxt->inode == NULL) {
	    cctxt->inode = inode;
	    xsltLREInfoCreate(cctxt, NULL, 0);
	} 
    }       
    cctxt->depth++;
    cctxt->inode = inode;
    inode->node = node;
    inode->depth = cctxt->depth;
    inode->templ = NULL;
    inode->category = XSLT_ELEMENT_CATEGORY_XSLT;
    inode->type = 0;
    inode->item = NULL;
    inode->curChildType = 0;
    inode->extContentHandled = 0;
    inode->isRoot = 0;
    
    if (inode->prev != NULL) {
	iprev = inode->prev;
	inode->inScopeNs = iprev->inScopeNs;
	inode->litResElemInfo = iprev->litResElemInfo;
	inode->nsChanged = iprev->nsChanged;
	inode->exclResultNs = iprev->exclResultNs;
	inode->extElemNs = iprev->extElemNs;
	inode->preserveWhitespace = iprev->preserveWhitespace;
	inode->forwardsCompat = iprev->forwardsCompat;	
    } else {
	inode->inScopeNs = NULL;
	inode->exclResultNs = NULL;
	inode->extElemNs = NULL;
	inode->preserveWhitespace = 0;
	inode->forwardsCompat = 0;
    }
    
    return(inode);
}

static void
xsltCompilerNodePop(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{    
    if (cctxt->inode == NULL) {
	xmlGenericError(xmlGenericErrorContext,
	    "xsltCompilerNodePop: Top-node mismatch.\n");
	return;
    }
    if (cctxt->inode->node != node) {
	xmlGenericError(xmlGenericErrorContext,
	"xsltCompilerNodePop: Node mismatch.\n");
	goto mismatch;
    }
    if (cctxt->inode->depth != cctxt->depth) {
	xmlGenericError(xmlGenericErrorContext,
	"xsltCompilerNodePop: Depth mismatch.\n");
	goto mismatch;
    }
    if ((cctxt->ivar) && (cctxt->ivar->depth > cctxt->depth))
	xsltCompilerVarInfoPop(cctxt);

    cctxt->depth--;
    cctxt->inode = cctxt->inode->prev;
    if (cctxt->inode != NULL)
	cctxt->inode->curChildType = 0;
    return;

mismatch:
    {
	const xmlChar *nsName = NULL, *name = NULL;
	const xmlChar *infnsName = NULL, *infname = NULL;
	
	if (node) {
	    if (node->type == XML_ELEMENT_NODE) {
		name = node->name;
		if (node->ns != NULL)
		    nsName = node->ns->href;
		else
		    nsName = BAD_CAST "";
	    } else {
		name = BAD_CAST "#document";
		nsName = BAD_CAST "";
	    }
	} else
	    name = BAD_CAST "Not given";

	if (cctxt->inode->node) {
	    if (node->type == XML_ELEMENT_NODE) {
		infname = cctxt->inode->node->name;
		if (cctxt->inode->node->ns != NULL)
		    infnsName = cctxt->inode->node->ns->href;
		else
		    infnsName = BAD_CAST "";
	    } else {
		infname = BAD_CAST "#document";
		infnsName = BAD_CAST "";
	    }
	} else
	    infname = BAD_CAST "Not given";

	
	xmlGenericError(xmlGenericErrorContext,
	    "xsltCompilerNodePop: Given   : '%s' URI '%s'\n",
	    name, nsName);
	xmlGenericError(xmlGenericErrorContext,
	    "xsltCompilerNodePop: Expected: '%s' URI '%s'\n",
	    infname, infnsName);
    }
}

static xsltNsListContainerPtr
xsltCompilerBuildInScopeNsList(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{
    xsltNsListContainerPtr nsi = NULL;
    xmlNsPtr *list = NULL, ns;
    int i, maxns = 5;
    while (node != NULL) {
        if (node->type == XML_ELEMENT_NODE) {
            ns = node->nsDef;
            while (ns != NULL) {
                if (nsi == NULL) {
		    nsi = (xsltNsListContainerPtr)
			xmlMalloc(sizeof(xsltNsListContainer));
		    if (nsi == NULL) {
			xsltTransformError(NULL, cctxt->style, NULL,
			    "xsltCompilerBuildInScopeNsList: "
			    "malloc failed!\n");
			goto internal_err;
		    }
		    memset(nsi, 0, sizeof(xsltNsListContainer));
                    nsi->list =
                        (xmlNsPtr *) xmlMalloc(maxns * sizeof(xmlNsPtr));
                    if (nsi->list == NULL) {
			xsltTransformError(NULL, cctxt->style, NULL,
			    "xsltCompilerBuildInScopeNsList: "
			    "malloc failed!\n");
			goto internal_err;
                    }
                    nsi->list[0] = NULL;
                }
                for (i = 0; i < nsi->totalNumber; i++) {
                    if ((ns->prefix == nsi->list[i]->prefix) ||
                        (xmlStrEqual(ns->prefix, nsi->list[i]->prefix)))
		    break;
                }
                if (i >= nsi->totalNumber) {
                    if (nsi->totalNumber +1 >= maxns) {
                        maxns *= 2;
			nsi->list =
			    (xmlNsPtr *) xmlRealloc(nsi->list,
				maxns * sizeof(xmlNsPtr));
                        if (nsi->list == NULL) {
                            xsltTransformError(NULL, cctxt->style, NULL,
				"xsltCompilerBuildInScopeNsList: "
				"realloc failed!\n");
				goto internal_err;
                        }
                    }
                    nsi->list[nsi->totalNumber++] = ns;
                    nsi->list[nsi->totalNumber] = NULL;
                }

                ns = ns->next;
            }
        }
        node = node->parent;
    }
    if (nsi == NULL)
	return(NULL);
    nsi->xpathNumber = nsi->totalNumber;
    for (i = 0; i < nsi->totalNumber; i++) {
	if (nsi->list[i]->prefix == NULL) {
	    ns = nsi->list[i];
	    nsi->list[i] = nsi->list[nsi->totalNumber-1];
	    nsi->list[nsi->totalNumber-1] = ns;
	    nsi->xpathNumber--;
	    break;
	}
    }
    if (xsltPointerListAddSize(
	(xsltPointerListPtr)cctxt->psData->inScopeNamespaces,
	(void *) nsi, 5) == -1)
    {	
	xmlFree(nsi);
	nsi = NULL;
	xsltTransformError(NULL, cctxt->style, NULL,
	    "xsltCompilerBuildInScopeNsList: failed to add ns-info.\n");
	goto internal_err;
    }
    if (cctxt->inode != NULL)
	cctxt->inode->nsChanged = 1;

    return(nsi);

internal_err:
    if (list != NULL)
	xmlFree(list);    
    cctxt->style->errors++;
    return(NULL);
}

static int
xsltParseNsPrefixList(xsltCompilerCtxtPtr cctxt,
		      xsltPointerListPtr list,
		      xmlNodePtr node,
		      const xmlChar *value)
{
    xmlChar *cur, *end;
    xmlNsPtr ns;
    
    if ((cctxt == NULL) || (value == NULL) || (list == NULL))
	return(-1);

    list->number = 0;

    cur = (xmlChar *) value;
    while (*cur != 0) {
	while (IS_BLANK(*cur)) cur++;
	if (*cur == 0)
	    break;
	end = cur;
	while ((*end != 0) && (!IS_BLANK(*end))) end++;
	cur = xmlStrndup(cur, end - cur);
	if (cur == NULL) {
	    cur = end;
	    continue;
	}		
	if ((cur[0] == '#') &&
	    xmlStrEqual(cur, (const xmlChar *)"#default"))
	    ns = xmlSearchNs(cctxt->style->doc, node, NULL);
	else
	    ns = xmlSearchNs(cctxt->style->doc, node, cur);	    

	if (ns == NULL) {
	    xsltTransformError(NULL, cctxt->style, node,
		"No namespace binding in scope for prefix '%s'.\n", cur);
	    cctxt->style->errors++;
	} else {
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"resolved prefix '%s'\n", cur);
#endif
	    if (xsltPointerListAddSize(list,
		(void *) xmlDictLookup(cctxt->style->dict,
		ns->href, -1), 5) == -1)
	    {
		xmlFree(cur);
		goto internal_err;
	    }
	}
	xmlFree(cur);
		
	cur = end;
    }
    return(0);

internal_err:
    cctxt->style->errors++;
    return(-1);
}

static xsltPointerListPtr
xsltCompilerUtilsCreateMergedList(xsltPointerListPtr first,
			    xsltPointerListPtr second)
{
    xsltPointerListPtr ret;
    size_t num;

    if (first)
	num = first->number;
    else
	num = 0;
    if (second)
	num += second->number;    
    if (num == 0)
	return(NULL);
    ret = xsltPointerListCreate(num);
    if (ret == NULL)
	return(NULL);
    if ((first != NULL) &&  (first->number != 0)) {
	memcpy(ret->items, first->items,
	    first->number * sizeof(void *));
	if ((second != NULL) && (second->number != 0))
	    memcpy(ret->items + first->number, second->items,
		second->number * sizeof(void *));
    } else if ((second != NULL) && (second->number != 0))
	memcpy(ret->items, (void *) second->items,
	    second->number * sizeof(void *));
    ret->number = num;
    return(ret);
}

static xsltPointerListPtr
xsltParseExclResultPrefixes(xsltCompilerCtxtPtr cctxt, xmlNodePtr node,
			    xsltPointerListPtr def,
			    int instrCategory)
{    
    xsltPointerListPtr list = NULL;
    xmlChar *value;
    xmlAttrPtr attr;

    if ((cctxt == NULL) || (node == NULL))
	return(NULL);

    if (instrCategory == XSLT_ELEMENT_CATEGORY_XSLT)
	attr = xmlHasNsProp(node, BAD_CAST "exclude-result-prefixes", NULL);
    else
	attr = xmlHasNsProp(node, BAD_CAST "exclude-result-prefixes",
	    XSLT_NAMESPACE);
    if (attr == NULL)	
	return(def);

    if (attr && (instrCategory == XSLT_ELEMENT_CATEGORY_LRE)) {
	attr->psvi = (void *) xsltXSLTAttrMarker;
    }

    if ((attr->children != NULL) &&	
	(attr->children->content != NULL))
	value = attr->children->content;
    else {
	xsltTransformError(NULL, cctxt->style, node,
	    "Attribute 'exclude-result-prefixes': Invalid value.\n");
	cctxt->style->errors++;
	return(def);
    }        

    if (xsltParseNsPrefixList(cctxt, cctxt->tmpList, node,
	BAD_CAST value) != 0)
	goto exit;
    if (cctxt->tmpList->number == 0)	
	goto exit;    
    list = xsltCompilerUtilsCreateMergedList(def, cctxt->tmpList);
    if (list == NULL)
	goto exit;    
    if (xsltPointerListAddSize(
	cctxt->psData->exclResultNamespaces, list, 5) == -1)
    {
	xsltPointerListFree(list);
	list = NULL;
	goto exit;
    }
    if (cctxt->inode != NULL)
	cctxt->inode->nsChanged = 1;

exit:    
    if (list != NULL)
	return(list);
    else
	return(def);
}

static xsltPointerListPtr
xsltParseExtElemPrefixes(xsltCompilerCtxtPtr cctxt, xmlNodePtr node,
			 xsltPointerListPtr def,
			 int instrCategory)
{    
    xsltPointerListPtr list = NULL;
    xmlAttrPtr attr;
    xmlChar *value;
    int i;

    if ((cctxt == NULL) || (node == NULL))
	return(NULL);

    if (instrCategory == XSLT_ELEMENT_CATEGORY_XSLT)
	attr = xmlHasNsProp(node, BAD_CAST "extension-element-prefixes", NULL);
    else
	attr = xmlHasNsProp(node, BAD_CAST "extension-element-prefixes",
	    XSLT_NAMESPACE);
    if (attr == NULL)	
	return(def);

    if (attr && (instrCategory == XSLT_ELEMENT_CATEGORY_LRE)) {
	attr->psvi = (void *) xsltXSLTAttrMarker;
    }

    if ((attr->children != NULL) &&	
	(attr->children->content != NULL))
	value = attr->children->content;
    else {
	xsltTransformError(NULL, cctxt->style, node,
	    "Attribute 'extension-element-prefixes': Invalid value.\n");
	cctxt->style->errors++;
	return(def);
    }


    if (xsltParseNsPrefixList(cctxt, cctxt->tmpList, node,
	BAD_CAST value) != 0)
	goto exit;

    if (cctxt->tmpList->number == 0)
	goto exit;    
    for (i = 0; i < cctxt->tmpList->number; i++)
	xsltRegisterExtPrefix(cctxt->style, NULL,
	BAD_CAST cctxt->tmpList->items[i]);
    list = xsltCompilerUtilsCreateMergedList(def, cctxt->tmpList);
    if (list == NULL)
	goto exit;
    if (xsltPointerListAddSize(
	cctxt->psData->extElemNamespaces, list, 5) == -1)
    {
	xsltPointerListFree(list);
	list = NULL;
	goto exit;
    }
    if (cctxt->inode != NULL)
	cctxt->inode->nsChanged = 1;

exit:    
    if (list != NULL)
	return(list);
    else
	return(def);
}

static int
xsltParseAttrXSLTVersion(xsltCompilerCtxtPtr cctxt, xmlNodePtr node,			 
			 int instrCategory)
{
    xmlChar *value;
    xmlAttrPtr attr;

    if ((cctxt == NULL) || (node == NULL))
	return(-1);

    if (instrCategory == XSLT_ELEMENT_CATEGORY_XSLT)
	attr = xmlHasNsProp(node, BAD_CAST "version", NULL);
    else
	attr = xmlHasNsProp(node, BAD_CAST "version", XSLT_NAMESPACE);

    if (attr == NULL)	
	return(0);

    attr->psvi = (void *) xsltXSLTAttrMarker;

    if ((attr->children != NULL) &&	
	(attr->children->content != NULL))
	value = attr->children->content;
    else {
	xsltTransformError(NULL, cctxt->style, node,
	    "Attribute 'version': Invalid value.\n");
	cctxt->style->errors++;
	return(1);
    }
    
    if (! xmlStrEqual(value, (const xmlChar *)"1.0")) {
	cctxt->inode->forwardsCompat = 1;
	if (! cctxt->hasForwardsCompat) {
	    cctxt->hasForwardsCompat = 1;
	    cctxt->errSeverity = XSLT_ERROR_SEVERITY_WARNING;
	    xsltTransformError(NULL, cctxt->style, node,
		"Warning: the attribute xsl:version specifies a value "
		"different from '1.0'. Switching to forwards-compatible "
		"mode. Only features of XSLT 1.0 are supported by this "
		"processor.\n");
	    cctxt->style->warnings++;
	    cctxt->errSeverity = XSLT_ERROR_SEVERITY_ERROR;
	}	
    } else {
	cctxt->inode->forwardsCompat = 0;
    }

    if (attr && (instrCategory == XSLT_ELEMENT_CATEGORY_LRE)) {
	attr->psvi = (void *) xsltXSLTAttrMarker;
    }
    return(1);
}

static int
xsltParsePreprocessStylesheetTree(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{
    xmlNodePtr deleteNode, cur, txt, textNode = NULL;
    xmlDocPtr doc;
    xsltStylesheetPtr style;
    int internalize = 0, findSpaceAttr;
    int xsltStylesheetElemDepth;
    xmlAttrPtr attr;
    xmlChar *value;
    const xmlChar *name, *nsNameXSLT = NULL;
    int strictWhitespace, inXSLText = 0;
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
    xsltNsMapPtr nsMapItem;
#endif

    if ((cctxt == NULL) || (cctxt->style == NULL) ||
	(node == NULL) || (node->type != XML_ELEMENT_NODE))
        return(-1);

    doc = node->doc;
    if (doc == NULL)
	goto internal_err;

    style = cctxt->style;
    if ((style->dict != NULL) && (doc->dict == style->dict))
	internalize = 1;
    else
        style->internalized = 0;

    if (! cctxt->simplified)
	xsltStylesheetElemDepth = cctxt->depth +1;
    else
	xsltStylesheetElemDepth = 0;

    if (xmlNodeGetSpacePreserve(node) != 1)
	cctxt->inode->preserveWhitespace = 0;
    else
	cctxt->inode->preserveWhitespace = 1; 
    
    strictWhitespace = (cctxt->strict != 0) ? 1 : 0;

    nsNameXSLT = xsltConstNamespaceNameXSLT;

    deleteNode = NULL;
    cur = node;
    while (cur != NULL) {
	if (deleteNode != NULL)	{

#ifdef WITH_XSLT_DEBUG_BLANKS
	    xsltGenericDebug(xsltGenericDebugContext,
	     "xsltParsePreprocessStylesheetTree: removing node\n");
#endif
	    xmlUnlinkNode(deleteNode);
	    xmlFreeNode(deleteNode);
	    deleteNode = NULL;
	}
	if (cur->type == XML_ELEMENT_NODE) {
	    
	    cur->psvi = NULL;

	    xsltCompilerNodePush(cctxt, cur);

	    inXSLText = 0;
	    textNode = NULL;	    
	    findSpaceAttr = 1;	    
	    cctxt->inode->stripWhitespace = 0;
	    if (IS_XSLT_ELEM(cur)) {
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
		if (cur->ns->href != nsNameXSLT) {
		    nsMapItem = xsltNewNamespaceMapItem(cctxt,
			doc, cur->ns, cur);
		    if (nsMapItem == NULL)
			goto internal_err;
		    cur->ns->href = nsNameXSLT;
		}
#endif

		if (cur->name == NULL)
		    goto process_attributes;
		

		
		name = cur->name;
		switch (*name) {
		    case 't':
			if ((name[0] == 't') && (name[1] == 'e') &&
			    (name[2] == 'x') && (name[3] == 't') &&
			    (name[4] == 0))
			{
			    cur->psvi = (void *) xsltXSLTTextMarker;
			    findSpaceAttr = 0;
			    cctxt->inode->preserveWhitespace = 1;
			    inXSLText = 1;
			}			    
			break;
		    case 'c':
			if (xmlStrEqual(name, BAD_CAST "choose") ||
			    xmlStrEqual(name, BAD_CAST "call-template"))
			    cctxt->inode->stripWhitespace = 1;
			break;
		    case 'a':
			if (xmlStrEqual(name, BAD_CAST "apply-templates") ||
			    xmlStrEqual(name, BAD_CAST "apply-imports") ||
			    xmlStrEqual(name, BAD_CAST "attribute-set"))

			    cctxt->inode->stripWhitespace = 1;
			break;
		    default:
			if (xsltStylesheetElemDepth == cctxt->depth) {
			    cctxt->inode->stripWhitespace = 1;
			    break;
			}

			if ((cur->prev != NULL) &&
			    (cur->prev->type == XML_TEXT_NODE))
			{
			    if (((*name == 'p') || (*name == 's')) &&
				(xmlStrEqual(name, BAD_CAST "param") ||
				 xmlStrEqual(name, BAD_CAST "sort")))
			    {
				do {
				    if (IS_BLANK_NODE(cur->prev)) {
					txt = cur->prev;
					xmlUnlinkNode(txt);
					xmlFreeNode(txt);
				    } else {
					break;
				    }
				} while (cur->prev);				    
			    }
			}
			break;
		}
	    }

process_attributes:
	    if (cur->properties != NULL) {
		if (cur->children == NULL)
		    findSpaceAttr = 0;
		attr = cur->properties;
		do {
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
		    if ((attr->ns) && (attr->ns->href != nsNameXSLT) &&
			xmlStrEqual(attr->ns->href, nsNameXSLT))
		    {			
			nsMapItem = xsltNewNamespaceMapItem(cctxt,
			    doc, attr->ns, cur);
			if (nsMapItem == NULL)
			    goto internal_err;
			attr->ns->href = nsNameXSLT;
		    }		    
#endif
		    if (internalize) {
			txt = attr->children;
			if ((txt != NULL) && (txt->content != NULL) &&
			    (!xmlDictOwns(style->dict, txt->content)))
			{
			    value = (xmlChar *) xmlDictLookup(style->dict,
				txt->content, -1);
			    xmlNodeSetContent(txt, NULL);
			    txt->content = value;
			}
		    }
		    if ((findSpaceAttr != 0) &&
			(attr->ns != NULL) &&
			(attr->name != NULL) &&
			(attr->name[0] == 's') &&			
			(attr->ns->prefix != NULL) &&
			(attr->ns->prefix[0] == 'x') &&
			(attr->ns->prefix[1] == 'm') &&
			(attr->ns->prefix[2] == 'l') &&
			(attr->ns->prefix[3] == 0))
		    {
			value = xmlGetNsProp(cur, BAD_CAST "space",
			    XML_XML_NAMESPACE);
			if (value != NULL) {
			    if (xmlStrEqual(value, BAD_CAST "preserve")) {
				cctxt->inode->preserveWhitespace = 1;				
			    } else if (xmlStrEqual(value, BAD_CAST "default")) {
				cctxt->inode->preserveWhitespace = 0;
			    } else {
				
				xsltTransformError(NULL, style, cur,
				    "Attribute xml:space: Invalid value.\n");
				cctxt->style->warnings++;
			    }
			    findSpaceAttr = 0;
			    xmlFree(value);
			}
			
		    }
		    attr = attr->next;
		} while (attr != NULL);
	    }
	    if (cur->children != NULL) {
		cur = cur->children;
		continue;
	    }
	} else if ((cur->type == XML_TEXT_NODE) ||
		(cur->type == XML_CDATA_SECTION_NODE))
	{
	    cur->type = XML_TEXT_NODE;
	    if ((strictWhitespace != 0) || (inXSLText != 0)) {
		if (textNode == NULL)
		    textNode = cur;
		else {
		    if (cur->content != NULL)
			xmlNodeAddContent(textNode, cur->content);
		    deleteNode = cur;
		}
		if ((cur->next == NULL) ||
		    (cur->next->type == XML_ELEMENT_NODE))
		    goto end_of_text;
		else
		    goto next_sibling;
	    } else {
		if (textNode == NULL)
		    textNode = cur;
		goto end_of_text;
	    }	    	   
	} else if ((cur->type == XML_COMMENT_NODE) ||
	    (cur->type == XML_PI_NODE))
	{	    
	    deleteNode = cur;
	    if ((cur->next == NULL) ||
		(cur->next->type == XML_ELEMENT_NODE))
		goto end_of_text;
	    else
		goto next_sibling;
	} else {
	    textNode = NULL;
	    xsltTransformError(NULL, style, cur,
		"Invalid type of node for the XSLT data model.\n");
	    cctxt->style->errors++;
	    goto next_sibling;
	}

end_of_text:
	if (textNode) {
	    value = textNode->content;
	    if ((value == NULL) || (*value == 0) ||
		(((cctxt->inode->stripWhitespace) ||
		  (! cctxt->inode->preserveWhitespace)) &&
		 IS_BLANK(*value) &&
		 xsltIsBlank(value)))
	    {		
		if (textNode != cur) {
		    xmlUnlinkNode(textNode);
		    xmlFreeNode(textNode);
		} else
		    deleteNode = textNode;
		textNode = NULL;
		goto next_sibling;
	    }
	    if (textNode->type != XML_TEXT_NODE) {
		textNode->type = XML_TEXT_NODE;
		textNode->name = xmlStringText;
	    }
	    if (internalize &&
		(textNode->content != NULL) &&
		(!xmlDictOwns(style->dict, textNode->content)))
	    {
		value = (xmlChar *) xmlDictLookup(style->dict,
		    textNode->content, -1);
		xmlNodeSetContent(textNode, NULL);
		textNode->content = value;
	    }
	    textNode = NULL;
	}

next_sibling:
	if (cur->type == XML_ELEMENT_NODE) {
	    xsltCompilerNodePop(cctxt, cur);
	}
	if (cur == node)
	    break;
	if (cur->next != NULL) {
	    cur = cur->next;
	} else {
	    cur = cur->parent;
	    inXSLText = 0;
	    goto next_sibling;
	};
    }
    if (deleteNode != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
	 "xsltParsePreprocessStylesheetTree: removing node\n");
#endif
	xmlUnlinkNode(deleteNode);
	xmlFreeNode(deleteNode);
    }
    return(0);

internal_err:
    return(-1);
}

#endif 

#ifdef XSLT_REFACTORED
#else
static void
xsltPrecomputeStylesheet(xsltStylesheetPtr style, xmlNodePtr cur)
{
    xmlNodePtr deleteNode, styleelem;
    int internalize = 0;

    if ((style == NULL) || (cur == NULL))
        return;

    if ((cur->doc != NULL) && (style->dict != NULL) &&
        (cur->doc->dict == style->dict))
	internalize = 1;
    else
        style->internalized = 0;

    if ((cur != NULL) && (IS_XSLT_ELEM(cur)) &&
        (IS_XSLT_NAME(cur, "stylesheet"))) {
	styleelem = cur;
    } else {
        styleelem = NULL;
    }

    deleteNode = NULL;
    while (cur != NULL) {
	if (deleteNode != NULL) {
#ifdef WITH_XSLT_DEBUG_BLANKS
	    xsltGenericDebug(xsltGenericDebugContext,
	     "xsltPrecomputeStylesheet: removing ignorable blank node\n");
#endif
	    xmlUnlinkNode(deleteNode);
	    xmlFreeNode(deleteNode);
	    deleteNode = NULL;
	}
	if (cur->type == XML_ELEMENT_NODE) {
	    int exclPrefixes;
	    if ((internalize) && (cur->properties != NULL)) {
	        xmlAttrPtr attr = cur->properties;
		xmlNodePtr txt;

		while (attr != NULL) {
		    txt = attr->children;
		    if ((txt != NULL) && (txt->type == XML_TEXT_NODE) &&
		        (txt->content != NULL) &&
			(!xmlDictOwns(style->dict, txt->content)))
		    {
			xmlChar *tmp;

			tmp = (xmlChar *) xmlDictLookup(style->dict,
			                                txt->content, -1);
			if (tmp != txt->content) {
			    xmlNodeSetContent(txt, NULL);
			    txt->content = tmp;
			}
		    }
		    attr = attr->next;
		}
	    }
	    if (IS_XSLT_ELEM(cur)) {
		exclPrefixes = 0;
		xsltStylePreCompute(style, cur);
		if (IS_XSLT_NAME(cur, "text")) {
		    for (;exclPrefixes > 0;exclPrefixes--)
			exclPrefixPop(style);
		    goto skip_children;
		}
	    } else {
		exclPrefixes = xsltParseStylesheetExcludePrefix(style, cur, 0);
	    }
	    	     
	    if ((cur->nsDef != NULL) && (style->exclPrefixNr > 0)) {
		xmlNsPtr ns = cur->nsDef, prev = NULL, next;
		xmlNodePtr root = NULL;
		int i, moved;

		root = xmlDocGetRootElement(cur->doc);
		if ((root != NULL) && (root != cur)) {
		    while (ns != NULL) {
			moved = 0;
			next = ns->next;
			for (i = 0;i < style->exclPrefixNr;i++) {
			    if ((ns->prefix != NULL) && 
			        (xmlStrEqual(ns->href,
					     style->exclPrefixTab[i]))) {
				if (prev == NULL) {
				    cur->nsDef = ns->next;
				} else {
				    prev->next = ns->next;
				}
				ns->next = root->nsDef;
				root->nsDef = ns;
				moved = 1;
				break;
			    }
			}
			if (moved == 0)
			    prev = ns;
			ns = next;
		    }
		}
	    }
	    if (exclPrefixes > 0) {
		xsltPrecomputeStylesheet(style, cur->children);
		for (;exclPrefixes > 0;exclPrefixes--)
		    exclPrefixPop(style);
		goto skip_children;
	    }
	} else if (cur->type == XML_TEXT_NODE) {
	    if (IS_BLANK_NODE(cur)) {
		if (xmlNodeGetSpacePreserve(cur) != 1) {
		    deleteNode = cur;
		}
	    } else if ((cur->content != NULL) && (internalize) &&
	               (!xmlDictOwns(style->dict, cur->content))) {
		xmlChar *tmp;

		tmp = (xmlChar *) xmlDictLookup(style->dict, cur->content, -1);
		xmlNodeSetContent(cur, NULL);
		cur->content = tmp;
	    }
	} else if ((cur->type != XML_ELEMENT_NODE) &&
		   (cur->type != XML_CDATA_SECTION_NODE)) {
	    deleteNode = cur;
	    goto skip_children;
	}

	if ((cur->type == XML_ELEMENT_NODE) && (cur->ns != NULL) &&
	    (styleelem != NULL) && (cur->parent == styleelem) &&
	    (!xmlStrEqual(cur->ns->href, XSLT_NAMESPACE)) &&
	    (!xsltCheckExtURI(style, cur->ns->href))) {
	    goto skip_children;
	} else if (cur->children != NULL) {
	    if ((cur->children->type != XML_ENTITY_DECL) &&
		(cur->children->type != XML_ENTITY_REF_NODE) &&
		(cur->children->type != XML_ENTITY_NODE)) {
		cur = cur->children;
		continue;
	    }
	}

skip_children:
	if (cur->next != NULL) {
	    cur = cur->next;
	    continue;
	}
	do {

	    cur = cur->parent;
	    if (cur == NULL)
		break;
	    if (cur == (xmlNodePtr) style->doc) {
		cur = NULL;
		break;
	    }
	    if (cur->next != NULL) {
		cur = cur->next;
		break;
	    }
	} while (cur != NULL);
    }
    if (deleteNode != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
	 "xsltPrecomputeStylesheet: removing ignorable blank node\n");
#endif
	xmlUnlinkNode(deleteNode);
	xmlFreeNode(deleteNode);
    }
}
#endif 


static void
xsltGatherNamespaces(xsltStylesheetPtr style) {
    xmlNodePtr cur;
    const xmlChar *URI;

    if (style == NULL)
        return;
    cur = xmlDocGetRootElement(style->doc);
    while (cur != NULL) {
	if (cur->type == XML_ELEMENT_NODE) {
	    xmlNsPtr ns = cur->nsDef;
	    while (ns != NULL) {
		if (ns->prefix != NULL) {
		    if (style->nsHash == NULL) {
			style->nsHash = xmlHashCreate(10);
			if (style->nsHash == NULL) {
			    xsltTransformError(NULL, style, cur,
		 "xsltGatherNamespaces: failed to create hash table\n");
			    style->errors++;
			    return;
			}
		    }
		    URI = xmlHashLookup(style->nsHash, ns->prefix);
		    if ((URI != NULL) && (!xmlStrEqual(URI, ns->href))) {
			xsltTransformError(NULL, style, cur,
	     "Namespaces prefix %s used for multiple namespaces\n",ns->prefix);
			style->warnings++;
		    } else if (URI == NULL) {
			xmlHashUpdateEntry(style->nsHash, ns->prefix,
			    (void *) ns->href, (xmlHashDeallocator)xmlFree);

#ifdef WITH_XSLT_DEBUG_PARSING
			xsltGenericDebug(xsltGenericDebugContext,
		 "Added namespace: %s mapped to %s\n", ns->prefix, ns->href);
#endif
		    }
		}
		ns = ns->next;
	    }
	}

	if (cur->children != NULL) {
	    if (cur->children->type != XML_ENTITY_DECL) {
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
	    if (cur == (xmlNodePtr) style->doc) {
		cur = NULL;
		break;
	    }
	    if (cur->next != NULL) {
		cur = cur->next;
		break;
	    }
	} while (cur != NULL);
    }
}

#ifdef XSLT_REFACTORED

static xsltStyleType
xsltGetXSLTElementTypeByNode(xsltCompilerCtxtPtr cctxt,
			     xmlNodePtr node)
{
    if ((node == NULL) || (node->type != XML_ELEMENT_NODE) ||
	(node->name == NULL))
	return(0);

    if (node->name[0] == 'a') {
	if (IS_XSLT_NAME(node, "apply-templates"))
	    return(XSLT_FUNC_APPLYTEMPLATES);
	else if (IS_XSLT_NAME(node, "attribute"))
	    return(XSLT_FUNC_ATTRIBUTE);
	else if (IS_XSLT_NAME(node, "apply-imports"))
	    return(XSLT_FUNC_APPLYIMPORTS);
	else if (IS_XSLT_NAME(node, "attribute-set"))
	    return(0);

    } else if (node->name[0] == 'c') {
	if (IS_XSLT_NAME(node, "choose"))
	    return(XSLT_FUNC_CHOOSE);
	else if (IS_XSLT_NAME(node, "copy"))
	    return(XSLT_FUNC_COPY);
	else if (IS_XSLT_NAME(node, "copy-of"))
	    return(XSLT_FUNC_COPYOF);
	else if (IS_XSLT_NAME(node, "call-template"))
	    return(XSLT_FUNC_CALLTEMPLATE);
	else if (IS_XSLT_NAME(node, "comment"))
	    return(XSLT_FUNC_COMMENT);

    } else if (node->name[0] == 'd') {
	if (IS_XSLT_NAME(node, "document"))
	    return(XSLT_FUNC_DOCUMENT);
	else if (IS_XSLT_NAME(node, "decimal-format"))
	    return(0);

    } else if (node->name[0] == 'e') {
	if (IS_XSLT_NAME(node, "element"))
	    return(XSLT_FUNC_ELEMENT);

    } else if (node->name[0] == 'f') {
	if (IS_XSLT_NAME(node, "for-each"))
	    return(XSLT_FUNC_FOREACH);
	else if (IS_XSLT_NAME(node, "fallback"))
	    return(XSLT_FUNC_FALLBACK);

    } else if (*(node->name) == 'i') {
	if (IS_XSLT_NAME(node, "if"))
	    return(XSLT_FUNC_IF);
	else if (IS_XSLT_NAME(node, "include"))
	    return(0);
	else if (IS_XSLT_NAME(node, "import"))
	    return(0);

    } else if (*(node->name) == 'k') {
	if (IS_XSLT_NAME(node, "key"))
	    return(0);

    } else if (*(node->name) == 'm') {
	if (IS_XSLT_NAME(node, "message"))
	    return(XSLT_FUNC_MESSAGE);

    } else if (*(node->name) == 'n') {
	if (IS_XSLT_NAME(node, "number"))
	    return(XSLT_FUNC_NUMBER);
	else if (IS_XSLT_NAME(node, "namespace-alias"))
	    return(0);

    } else if (*(node->name) == 'o') {
	if (IS_XSLT_NAME(node, "otherwise"))
	    return(XSLT_FUNC_OTHERWISE);
	else if (IS_XSLT_NAME(node, "output"))
	    return(0);

    } else if (*(node->name) == 'p') {
	if (IS_XSLT_NAME(node, "param"))
	    return(XSLT_FUNC_PARAM);
	else if (IS_XSLT_NAME(node, "processing-instruction"))
	    return(XSLT_FUNC_PI);
	else if (IS_XSLT_NAME(node, "preserve-space"))
	    return(0);

    } else if (*(node->name) == 's') {
	if (IS_XSLT_NAME(node, "sort"))
	    return(XSLT_FUNC_SORT);
	else if (IS_XSLT_NAME(node, "strip-space"))
	    return(0);
	else if (IS_XSLT_NAME(node, "stylesheet"))
	    return(0);

    } else if (node->name[0] == 't') {
	if (IS_XSLT_NAME(node, "text"))
	    return(XSLT_FUNC_TEXT);
	else if (IS_XSLT_NAME(node, "template"))
	    return(0);
	else if (IS_XSLT_NAME(node, "transform"))
	    return(0);

    } else if (*(node->name) == 'v') {
	if (IS_XSLT_NAME(node, "value-of"))
	    return(XSLT_FUNC_VALUEOF);
	else if (IS_XSLT_NAME(node, "variable"))
	    return(XSLT_FUNC_VARIABLE);

    } else if (*(node->name) == 'w') {
	if (IS_XSLT_NAME(node, "when"))
	    return(XSLT_FUNC_WHEN);
	if (IS_XSLT_NAME(node, "with-param"))
	    return(XSLT_FUNC_WITHPARAM);
    }
    return(0);
}

 
int
xsltParseAnyXSLTElem(xsltCompilerCtxtPtr cctxt, xmlNodePtr elem)
{
    if ((cctxt == NULL) || (elem == NULL) ||
	(elem->type != XML_ELEMENT_NODE))
	return(-1);

    elem->psvi = NULL;

    if (! (IS_XSLT_ELEM_FAST(elem)))
	return(-1);
    if (cctxt->inode->category == XSLT_ELEMENT_CATEGORY_EXTENSION) {
	cctxt->inode->extContentHandled = 1;
    }
    
    xsltCompilerNodePush(cctxt, elem);
    if (cctxt->inode->prev->curChildType != 0)
	cctxt->inode->type = cctxt->inode->prev->curChildType;
    else
	cctxt->inode->type = xsltGetXSLTElementTypeByNode(cctxt, elem);    
    if (elem->nsDef != NULL)
	cctxt->inode->inScopeNs =
	    xsltCompilerBuildInScopeNsList(cctxt, elem);
    xsltStylePreCompute(cctxt->style, elem);

    switch (cctxt->inode->type) {	
	case XSLT_FUNC_APPLYIMPORTS:
	    
	    goto empty_content;
	case XSLT_FUNC_APPLYTEMPLATES:
	    
	    goto apply_templates;	    
	case XSLT_FUNC_ATTRIBUTE:	    
	    
	    goto sequence_constructor;
	case XSLT_FUNC_CALLTEMPLATE:
	    
	    goto call_template;
	case XSLT_FUNC_CHOOSE:	    
	    
	    goto choose;
	case XSLT_FUNC_COMMENT:
	    
	    goto sequence_constructor;	    
	case XSLT_FUNC_COPY:
	    
	    goto sequence_constructor;	    
	case XSLT_FUNC_COPYOF:
	    
	    goto empty_content;	   
	case XSLT_FUNC_DOCUMENT: 
	    
	    goto sequence_constructor;
	case XSLT_FUNC_ELEMENT:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_FALLBACK:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_FOREACH:
	    
	    goto for_each;
	case XSLT_FUNC_IF:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_OTHERWISE:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_MESSAGE:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_NUMBER:
	    
	    goto empty_content;
	case XSLT_FUNC_PARAM:
	    if ((elem->psvi != NULL) && (cctxt->ivar != NULL)) {
		xsltVarInfoPtr ivar = cctxt->ivar;

		do {
		    if ((ivar->name ==
			 ((xsltStyleItemParamPtr) elem->psvi)->name) &&
			(ivar->nsName ==
			 ((xsltStyleItemParamPtr) elem->psvi)->ns))
		    {
			elem->psvi = NULL;
			xsltTransformError(NULL, cctxt->style, elem,
			    "Redefinition of variable or parameter '%s'.\n",
			    ivar->name);
			cctxt->style->errors++;
			goto error;
		    }
		    ivar = ivar->prev;
		} while (ivar != NULL);
	    }
	    
	    goto sequence_constructor;
	case XSLT_FUNC_PI:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_SORT:
	    
	    goto empty_content;
	case XSLT_FUNC_TEXT:
	    
	    goto text;
	case XSLT_FUNC_VALUEOF:
	    
	    goto empty_content;
	case XSLT_FUNC_VARIABLE:
	    if ((elem->psvi != NULL) && (cctxt->ivar != NULL)) {
		xsltVarInfoPtr ivar = cctxt->ivar;		

		do {
		    if ((ivar->name ==
			 ((xsltStyleItemVariablePtr) elem->psvi)->name) &&
			(ivar->nsName ==
			 ((xsltStyleItemVariablePtr) elem->psvi)->ns))
		    {
			elem->psvi = NULL;
			xsltTransformError(NULL, cctxt->style, elem,
			    "Redefinition of variable or parameter '%s'.\n",
			    ivar->name);
			cctxt->style->errors++;
			goto error;
		    }
		    ivar = ivar->prev;
		} while (ivar != NULL);
	    }
	    
	    goto sequence_constructor;
	case XSLT_FUNC_WHEN:
	    
	    goto sequence_constructor;
	case XSLT_FUNC_WITHPARAM:
	    
	    goto sequence_constructor;
	default:
#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		"xsltParseXSLTNode: Unhandled XSLT element '%s'.\n",
		elem->name);	    
#endif
	    xsltTransformError(NULL, cctxt->style, elem,
		"xsltParseXSLTNode: Internal error; "
		"unhandled XSLT element '%s'.\n", elem->name);
	    cctxt->style->errors++;
	    goto internal_err;
    }

apply_templates:
    
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (IS_XSLT_ELEM_FAST(child)) {
		    if (xmlStrEqual(child->name, BAD_CAST "with-param")) {
			cctxt->inode->curChildType = XSLT_FUNC_WITHPARAM;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else if (xmlStrEqual(child->name, BAD_CAST "sort")) {
			cctxt->inode->curChildType = XSLT_FUNC_SORT;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else
			xsltParseContentError(cctxt->style, child);
		} else
		    xsltParseContentError(cctxt->style, child);
	    }
	    child = child->next;
	} while (child != NULL);
    }    
    goto exit;

call_template:
    
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (IS_XSLT_ELEM_FAST(child)) {
		    xsltStyleType type;

		    type = xsltGetXSLTElementTypeByNode(cctxt, child);
		    if (type == XSLT_FUNC_WITHPARAM) {
			cctxt->inode->curChildType = XSLT_FUNC_WITHPARAM;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else {
			xsltParseContentError(cctxt->style, child);
		    }
		} else
		    xsltParseContentError(cctxt->style, child);
	    }
	    child = child->next;
	} while (child != NULL);
    }    
    goto exit;

text:
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if ((child->type != XML_TEXT_NODE) &&
		(child->type != XML_CDATA_SECTION_NODE))
	    {
		xsltTransformError(NULL, cctxt->style, elem,
		    "The XSLT 'text' element must have only character "
		    "data as content.\n");
	    }
	    child = child->next;
	} while (child != NULL);
    }
    goto exit;

empty_content:
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {
	    if (((child->type != XML_TEXT_NODE) &&
		 (child->type != XML_CDATA_SECTION_NODE)) ||
		(! IS_BLANK_NODE(child)))
	    {
		xsltTransformError(NULL, cctxt->style, elem,
		    "This XSLT element must have no content.\n");
		cctxt->style->errors++;
		break;
	    }
	    child = child->next;
	} while (child != NULL);		
    }
    goto exit;

choose:
    
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	int nbWhen = 0, nbOtherwise = 0, err = 0;
	do {
	    if (child->type == XML_ELEMENT_NODE) {
		if (IS_XSLT_ELEM_FAST(child)) {
		    xsltStyleType type;
		
		    type = xsltGetXSLTElementTypeByNode(cctxt, child);
		    if (type == XSLT_FUNC_WHEN) {
			nbWhen++;
			if (nbOtherwise) {
			    xsltParseContentError(cctxt->style, child);
			    err = 1;
			    break;
			}
			cctxt->inode->curChildType = XSLT_FUNC_WHEN;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else if (type == XSLT_FUNC_OTHERWISE) {
			if (! nbWhen) {
			    xsltParseContentError(cctxt->style, child);
			    err = 1;
			    break;
			}			
			if (nbOtherwise) {
			    xsltTransformError(NULL, cctxt->style, elem,
				"The XSLT 'choose' element must not contain "
				"more than one XSLT 'otherwise' element.\n");
			    cctxt->style->errors++;
			    err = 1;
			    break;
			}
			nbOtherwise++;
			cctxt->inode->curChildType = XSLT_FUNC_OTHERWISE;
			xsltParseAnyXSLTElem(cctxt, child);
		    } else
			xsltParseContentError(cctxt->style, child);
		} else
		    xsltParseContentError(cctxt->style, child);
	    } 
	    child = child->next;
	} while (child != NULL);
	if ((! err) && (! nbWhen)) {
	    xsltTransformError(NULL, cctxt->style, elem,
		"The XSLT element 'choose' must contain at least one "
		"XSLT element 'when'.\n");
		cctxt->style->errors++;
	}	
    }    
    goto exit;

for_each:
    
    if (elem->children != NULL) {
	xmlNodePtr child = elem->children;
	do {	    
	    if ((child->type == XML_ELEMENT_NODE) &&
		IS_XSLT_ELEM_FAST(child))
	    {		
		if (xsltGetXSLTElementTypeByNode(cctxt, child) ==
		    XSLT_FUNC_SORT)
		{		
		    cctxt->inode->curChildType = XSLT_FUNC_SORT;
		    xsltParseAnyXSLTElem(cctxt, child);
		} else
		    break;
	    } else
		break;
	    child = child->next;
	} while (child != NULL);
	if (child != NULL)
	    xsltParseSequenceConstructor(cctxt, child);
    }    
    goto exit;

sequence_constructor:
    if (elem->children != NULL)
	xsltParseSequenceConstructor(cctxt, elem->children);

    if ((elem->next != NULL) &&
	((cctxt->inode->type == XSLT_FUNC_VARIABLE) ||
	 (cctxt->inode->type == XSLT_FUNC_PARAM)))
    {	
	if ((elem->psvi != NULL) &&
	    (((xsltStyleBasicItemVariablePtr) elem->psvi)->name))
	{	
	    xsltCompilerVarInfoPush(cctxt, elem,
		((xsltStyleBasicItemVariablePtr) elem->psvi)->name,
		((xsltStyleBasicItemVariablePtr) elem->psvi)->ns);
	}
    }

error:
exit:
    xsltCompilerNodePop(cctxt, elem);
    return(0);

internal_err:
    xsltCompilerNodePop(cctxt, elem);
    return(-1);
}

 
static xsltStyleItemUknownPtr
xsltForwardsCompatUnkownItemCreate(xsltCompilerCtxtPtr cctxt)
{
    xsltStyleItemUknownPtr item;

    item = (xsltStyleItemUknownPtr) xmlMalloc(sizeof(xsltStyleItemUknown));
    if (item == NULL) {
	xsltTransformError(NULL, cctxt->style, NULL,
	    "Internal error in xsltForwardsCompatUnkownItemCreate(): "
	    "Failed to allocate memory.\n");
	cctxt->style->errors++;
	return(NULL);
    }
    memset(item, 0, sizeof(xsltStyleItemUknown));
    item->type = XSLT_FUNC_UNKOWN_FORWARDS_COMPAT;
    item->next = cctxt->style->preComps;
    cctxt->style->preComps = (xsltElemPreCompPtr) item;
    return(item);
}

 
static int
xsltParseUnknownXSLTElem(xsltCompilerCtxtPtr cctxt,
			    xmlNodePtr node)
{
    if ((cctxt == NULL) || (node == NULL))
	return(-1);

    if (cctxt->inode->category == XSLT_ELEMENT_CATEGORY_EXTENSION) {
	cctxt->inode->extContentHandled = 1;
    }    
    if (cctxt->inode->forwardsCompat == 0) {	
	xsltTransformError(NULL, cctxt->style, node,
	    "Unknown XSLT element '%s'.\n", node->name);
	cctxt->style->errors++;
	return(1);
    }
    if (cctxt->unknownItem == NULL) {
	cctxt->unknownItem = xsltForwardsCompatUnkownItemCreate(cctxt);
	if (cctxt->unknownItem == NULL) {
	    node->psvi = NULL;
	    return(-1);
	}
    }
    node->psvi = cctxt->unknownItem;
    if (node->children == NULL)
	return(0);
    else {
	xmlNodePtr child = node->children;

	xsltCompilerNodePush(cctxt, node);
	if (node->nsDef != NULL)
	    cctxt->inode->inScopeNs =
		xsltCompilerBuildInScopeNsList(cctxt, node);
	do {
	    if ((child->type == XML_ELEMENT_NODE) &&
		IS_XSLT_ELEM_FAST(child) &&
		IS_XSLT_NAME(child, "fallback"))
	    {
		cctxt->inode->curChildType = XSLT_FUNC_FALLBACK;
		xsltParseAnyXSLTElem(cctxt, child);
	    }
	    child = child->next;
	} while (child != NULL);
	
	xsltCompilerNodePop(cctxt, node);
    }
    return(0);
}

 
void
xsltParseSequenceConstructor(xsltCompilerCtxtPtr cctxt, xmlNodePtr cur)
{
    xsltStyleType type;
    xmlNodePtr deleteNode = NULL;

    if (cctxt == NULL) {
	xmlGenericError(xmlGenericErrorContext,
	    "xsltParseSequenceConstructor: Bad arguments\n");
	cctxt->style->errors++;
	return;
    }
    if (cctxt->inode->category == XSLT_ELEMENT_CATEGORY_EXTENSION) {
	cctxt->inode->extContentHandled = 1;
    }
    if (cur == NULL)
	return;
    
    while (cur != NULL) {
	if (deleteNode != NULL)	{
#ifdef WITH_XSLT_DEBUG_BLANKS
	    xsltGenericDebug(xsltGenericDebugContext,
	     "xsltParseSequenceConstructor: removing xsl:text element\n");
#endif
	    xmlUnlinkNode(deleteNode);
	    xmlFreeNode(deleteNode);
	    deleteNode = NULL;
	}
	if (cur->type == XML_ELEMENT_NODE) {	    
	    
	    if (cur->psvi == xsltXSLTTextMarker) {
		xmlNodePtr tmp;

		cur->psvi = NULL;
		deleteNode = cur;
		tmp = cur->children;
		if (tmp) {
		    if ((tmp->type == XML_TEXT_NODE) ||
			(tmp->next == NULL))
		    {
			xmlUnlinkNode(tmp);
			xmlAddPrevSibling(cur, tmp);
		    } else {
			tmp = NULL;
			xsltTransformError(NULL, cctxt->style, cur,
			    "Element 'xsl:text': Invalid type "
			    "of node found in content.\n");
			cctxt->style->errors++;
		    } 
		}
		if (cur->properties) {
		    xmlAttrPtr attr;
		    attr = cur->properties;
		    do {
			if ((attr->ns == NULL) &&
			    (attr->name != NULL) &&
			    (attr->name[0] == 'd') &&
			    xmlStrEqual(attr->name,
			    BAD_CAST "disable-output-escaping"))
			{
			    if ((attr->children != NULL) &&
				xmlStrEqual(attr->children->content,
				BAD_CAST "yes"))
			    {
				if (tmp)
				    tmp->name = xmlStringTextNoenc;
			    } else if ((attr->children == NULL) ||
				(attr->children->content == NULL) ||
				(!xmlStrEqual(attr->children->content,
				BAD_CAST "no")))
			    {
				xsltTransformError(NULL, cctxt->style,
				    cur,
				    "Attribute 'disable-output-escaping': "
				    "Invalid value. Expected is "
				    "'yes' or 'no'.\n");
				cctxt->style->errors++;
			    }
			    break;
			}
			attr = attr->next;
		    } while (attr != NULL);
		}
	    } else if (IS_XSLT_ELEM_FAST(cur)) {
			    
		cur->psvi = NULL;
		type = xsltGetXSLTElementTypeByNode(cctxt, cur);
		switch (type) {
		    case XSLT_FUNC_APPLYIMPORTS:
		    case XSLT_FUNC_APPLYTEMPLATES:
		    case XSLT_FUNC_ATTRIBUTE:
		    case XSLT_FUNC_CALLTEMPLATE:
		    case XSLT_FUNC_CHOOSE:
		    case XSLT_FUNC_COMMENT:
		    case XSLT_FUNC_COPY:
		    case XSLT_FUNC_COPYOF:
		    case XSLT_FUNC_DOCUMENT: 
		    case XSLT_FUNC_ELEMENT:
		    case XSLT_FUNC_FALLBACK:
		    case XSLT_FUNC_FOREACH:
		    case XSLT_FUNC_IF:
		    case XSLT_FUNC_MESSAGE:
		    case XSLT_FUNC_NUMBER:
		    case XSLT_FUNC_PI:
		    case XSLT_FUNC_TEXT:
		    case XSLT_FUNC_VALUEOF:
		    case XSLT_FUNC_VARIABLE:
			cctxt->inode->curChildType = type;
			xsltParseAnyXSLTElem(cctxt, cur);
			break;
		    default:
			xsltParseUnknownXSLTElem(cctxt, cur);			
			cur = cur->next;
			continue;
		}
	    } else {
		xsltCompilerNodePush(cctxt, cur);
		if (cur->nsDef != NULL)
		    cctxt->inode->inScopeNs =
			xsltCompilerBuildInScopeNsList(cctxt, cur);
		if (cur->properties)
		    cctxt->inode->extElemNs =
			xsltParseExtElemPrefixes(cctxt,
			    cur, cctxt->inode->extElemNs,
			    XSLT_ELEMENT_CATEGORY_LRE);
		if ((cur->ns != NULL) &&
		    (cctxt->inode->extElemNs != NULL) &&
		    (xsltCheckExtPrefix(cctxt->style, cur->ns->href) == 1))
		{
		    cctxt->inode->category = XSLT_ELEMENT_CATEGORY_EXTENSION;
		    cctxt->inode->extContentHandled = 0;
		    if (cur->psvi != NULL) {
			cur->psvi = NULL;
			xsltTransformError(NULL, cctxt->style, cur,
			    "Internal error in xsltParseSequenceConstructor(): "
			    "Occupied PSVI field.\n");
			cctxt->style->errors++;
			cur = cur->next;
			continue;
		    }
		    cur->psvi = (void *)
			xsltPreComputeExtModuleElement(cctxt->style, cur);
		    
		    if (cur->psvi == NULL) {
			cur->psvi = (void *) xsltExtMarker;
		    }
		    if ((cur->children != NULL) &&
			(cctxt->inode->extContentHandled == 0))
		    {
			xsltParseSequenceConstructor(cctxt, cur->children);
		    }
		} else {
		    cur->psvi = NULL;
		    cctxt->inode->category = XSLT_ELEMENT_CATEGORY_LRE;
		    if (cur->properties != NULL) {
			xmlAttrPtr attr = cur->properties;
			cctxt->inode->exclResultNs =
			    xsltParseExclResultPrefixes(cctxt, cur,
				cctxt->inode->exclResultNs,
				XSLT_ELEMENT_CATEGORY_LRE);
			xsltParseAttrXSLTVersion(cctxt, cur,
			    XSLT_ELEMENT_CATEGORY_LRE);
			do {
			    if ((attr->psvi != xsltXSLTAttrMarker) &&
				IS_XSLT_ATTR_FAST(attr))
			    {				    
				if (! xmlStrEqual(attr->name,
				    BAD_CAST "use-attribute-sets"))
				{				
				    xsltTransformError(NULL, cctxt->style,
					cur,
					"Unknown XSLT attribute '%s'.\n",
					attr->name);
				    cctxt->style->errors++;
				} else {
				    attr->psvi = (void *) xsltXSLTAttrMarker;
				}
			    }
			    attr = attr->next;
			} while (attr != NULL);
		    }
		    if (cctxt->inode->nsChanged)
			xsltLREInfoCreate(cctxt, cur, 1);
		    cur->psvi = cctxt->inode->litResElemInfo;
		    if (cctxt->hasNsAliases)
			xsltLREBuildEffectiveNs(cctxt, cur);
		    if (cur->properties) {
			xmlAttrPtr attr = cur->properties;
			
			while (attr != NULL) {
			    xsltCompileAttr(cctxt->style, attr);
			    attr = attr->next;
			}
		    }
		    if (cur->children != NULL) {
			xsltParseSequenceConstructor(cctxt, cur->children);
		    }
		}
		xsltCompilerNodePop(cctxt, cur);
	    }
	}
	cur = cur->next;
    }
    if (deleteNode != NULL) {
#ifdef WITH_XSLT_DEBUG_BLANKS
	xsltGenericDebug(xsltGenericDebugContext,
	    "xsltParseSequenceConstructor: removing xsl:text element\n");
#endif
	xmlUnlinkNode(deleteNode);
	xmlFreeNode(deleteNode);
	deleteNode = NULL;
    }
}

void
xsltParseTemplateContent(xsltStylesheetPtr style, xmlNodePtr templ) {
    if ((style == NULL) || (templ == NULL))
	return;

    if (XSLT_CCTXT(style)->inode->category == XSLT_ELEMENT_CATEGORY_EXTENSION) {
	XSLT_CCTXT(style)->inode->extContentHandled = 1;
    }

    if (templ->children != NULL) {	
	xmlNodePtr child = templ->children;
	
	do {
	    if ((child->type == XML_ELEMENT_NODE) &&
		IS_XSLT_ELEM_FAST(child) &&
		IS_XSLT_NAME(child, "param"))
	    {
		XSLT_CCTXT(style)->inode->curChildType = XSLT_FUNC_PARAM;
		xsltParseAnyXSLTElem(XSLT_CCTXT(style), child);
	    } else
		break;
	    child = child->next;
	} while (child != NULL);
	xsltParseSequenceConstructor(XSLT_CCTXT(style), child);
    }
}

#else 

void
xsltParseTemplateContent(xsltStylesheetPtr style, xmlNodePtr templ) {
    xmlNodePtr cur, delete;
    cur = templ->children;
    delete = NULL;
    while (cur != NULL) {
	if (delete != NULL) {
#ifdef WITH_XSLT_DEBUG_BLANKS
	    xsltGenericDebug(xsltGenericDebugContext,
	     "xsltParseTemplateContent: removing text\n");
#endif
	    xmlUnlinkNode(delete);
	    xmlFreeNode(delete);
	    delete = NULL;
	}
	if (IS_XSLT_ELEM(cur)) {
	    if (IS_XSLT_NAME(cur, "text")) {
		if (cur->children != NULL) {
		    xmlChar *prop;
		    xmlNodePtr text = cur->children, next;
		    int noesc = 0;
			
		    prop = xmlGetNsProp(cur,
			(const xmlChar *)"disable-output-escaping",
			NULL);
		    if (prop != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
			xsltGenericDebug(xsltGenericDebugContext,
			     "Disable escaping: %s\n", text->content);
#endif
			if (xmlStrEqual(prop, (const xmlChar *)"yes")) {
			    noesc = 1;
			} else if (!xmlStrEqual(prop,
						(const xmlChar *)"no")){
			    xsltTransformError(NULL, style, cur,
	     "xsl:text: disable-output-escaping allows only yes or no\n");
			    style->warnings++;

			}
			xmlFree(prop);
		    }

		    while (text != NULL) {
			if (text->type == XML_COMMENT_NODE) {
			    text = text->next;
			    continue;
			}
			if ((text->type != XML_TEXT_NODE) &&
			     (text->type != XML_CDATA_SECTION_NODE)) {
			    xsltTransformError(NULL, style, cur,
		 "xsltParseTemplateContent: xslt:text content problem\n");
			    style->errors++;
			    break;
			}
			if ((noesc) && (text->type != XML_CDATA_SECTION_NODE))
			    text->name = xmlStringTextNoenc;
			text = text->next;
		    }

		    if (text == NULL) {
			text = cur->children;
			while (text != NULL) {
			    if ((style->internalized) &&
			        (text->content != NULL) &&
			        (!xmlDictOwns(style->dict, text->content))) {

				if (text->doc->dict != NULL) {
				    const xmlChar *tmp;
				    
				    tmp = xmlDictLookup(text->doc->dict,
				                        text->content, -1);
				    if (tmp != text->content) {
				        xmlNodeSetContent(text, NULL);
					text->content = (xmlChar *) tmp;
				    }
				}
			    }

			    next = text->next;
			    xmlUnlinkNode(text);
			    xmlAddPrevSibling(cur, text);
			    text = next;
			}
		    }
		}
		delete = cur;
		goto skip_children;
	    }
	}
	else if ((cur->ns != NULL) && (style->nsDefs != NULL) &&
	    (xsltCheckExtPrefix(style, cur->ns->prefix)))
	{
	    xsltStylePreCompute(style, cur);
	}
	else if (cur->type == XML_ELEMENT_NODE)
	{
	    if ((cur->ns == NULL) && (style->defaultAlias != NULL)) {
		cur->ns = xmlSearchNsByHref(cur->doc, cur,
			style->defaultAlias);
	    }
	    if (cur->properties != NULL) {
	        xmlAttrPtr attr = cur->properties;

		while (attr != NULL) {
		    xsltCompileAttr(style, attr);
		    attr = attr->next;
		}
	    }
	}
	if (cur->children != NULL) {
	    if (cur->children->type != XML_ENTITY_DECL) {
		cur = cur->children;
		continue;
	    }
	}
skip_children:
	if (cur->next != NULL) {
	    cur = cur->next;
	    continue;
	}
	
	do {
	    cur = cur->parent;
	    if (cur == NULL)
		break;
	    if (cur == templ) {
		cur = NULL;
		break;
	    }
	    if (cur->next != NULL) {
		cur = cur->next;
		break;
	    }
	} while (cur != NULL);
    }
    if (delete != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
	 "xsltParseTemplateContent: removing text\n");
#endif
	xmlUnlinkNode(delete);
	xmlFreeNode(delete);
	delete = NULL;
    }

    cur = templ->children;
    while (cur != NULL) {
	if ((IS_XSLT_ELEM(cur)) && (!(IS_XSLT_NAME(cur, "param"))))
	    break;
	cur = cur->next;
    }

    while (cur != NULL) {
	if ((IS_XSLT_ELEM(cur)) && (IS_XSLT_NAME(cur, "param"))) {
	    xmlNodePtr param = cur;

	    xsltTransformError(NULL, style, cur,
		"xsltParseTemplateContent: ignoring misplaced param element\n");
	    if (style != NULL) style->warnings++;
            cur = cur->next;
	    xmlUnlinkNode(param);
	    xmlFreeNode(param);
	} else
	    break;
    }
}

#endif 


static void
xsltParseStylesheetKey(xsltStylesheetPtr style, xmlNodePtr key) {
    xmlChar *prop = NULL;
    xmlChar *use = NULL;
    xmlChar *match = NULL;
    xmlChar *name = NULL;
    xmlChar *nameURI = NULL;

    if ((style == NULL) || (key == NULL))
	return;

    prop = xmlGetNsProp(key, (const xmlChar *)"name", NULL);
    if (prop != NULL) {
        const xmlChar *URI;

	URI = xsltGetQNameURI(key, &prop);
	if (prop == NULL) {
	    if (style != NULL) style->errors++;
	    goto error;
	} else {
	    name = prop;
	    if (URI != NULL)
		nameURI = xmlStrdup(URI);
	}
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
	     "xsltParseStylesheetKey: name %s\n", name);
#endif
    } else {
	xsltTransformError(NULL, style, key,
	    "xsl:key : error missing name\n");
	if (style != NULL) style->errors++;
	goto error;
    }

    match = xmlGetNsProp(key, (const xmlChar *)"match", NULL);
    if (match == NULL) {
	xsltTransformError(NULL, style, key,
	    "xsl:key : error missing match\n");
	if (style != NULL) style->errors++;
	goto error;
    }

    use = xmlGetNsProp(key, (const xmlChar *)"use", NULL);
    if (use == NULL) {
	xsltTransformError(NULL, style, key,
	    "xsl:key : error missing use\n");
	if (style != NULL) style->errors++;
	goto error;
    }

    xsltAddKey(style, name, nameURI, match, use, key);


error:
    if (use != NULL)
	xmlFree(use);
    if (match != NULL)
	xmlFree(match);
    if (name != NULL)
	xmlFree(name);
    if (nameURI != NULL)
	xmlFree(nameURI);

    if (key->children != NULL) {
	xsltParseContentError(style, key->children);
    }
}

#ifdef XSLT_REFACTORED

static void
xsltParseXSLTTemplate(xsltCompilerCtxtPtr cctxt, xmlNodePtr templNode) {
    xsltTemplatePtr templ;
    xmlChar *prop;    
    double  priority;    

    if ((cctxt == NULL) || (templNode == NULL))
	return;

    templ = xsltNewTemplate();
    if (templ == NULL)
	return;

    xsltCompilerNodePush(cctxt, templNode);
    if (templNode->nsDef != NULL)
	cctxt->inode->inScopeNs =
	    xsltCompilerBuildInScopeNsList(cctxt, templNode);

    templ->next = cctxt->style->templates;
    cctxt->style->templates = templ;
    templ->style = cctxt->style;  

    prop = xmlGetNsProp(templNode, (const xmlChar *)"mode", NULL);
    if (prop != NULL) {	
        const xmlChar *modeURI;

	modeURI = xsltGetQNameURI(templNode, &prop);
	if (prop == NULL) {
	    cctxt->style->errors++;
	    goto error;
	}
	templ->mode = xmlDictLookup(cctxt->style->dict, prop, -1);
	xmlFree(prop);
	prop = NULL;
	if (xmlValidateNCName(templ->mode, 0)) {
	    xsltTransformError(NULL, cctxt->style, templNode,
		"xsl:template: Attribute 'mode': The local part '%s' "
		"of the value is not a valid NCName.\n", templ->name);
	    cctxt->style->errors++;
	    goto error;
	}
	if (modeURI != NULL)
	    templ->modeURI = xmlDictLookup(cctxt->style->dict, modeURI, -1);
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
	     "xsltParseXSLTTemplate: mode %s\n", templ->mode);
#endif
    }
    prop = xmlGetNsProp(templNode, (const xmlChar *)"match", NULL);
    if (prop != NULL) {
	templ->match  = prop;
	prop = NULL;
    }
    prop = xmlGetNsProp(templNode, (const xmlChar *)"priority", NULL);
    if (prop != NULL) {
	priority = xmlXPathStringEvalNumber(prop);
	templ->priority = (float) priority;
	xmlFree(prop);
	prop = NULL;
    }
    prop = xmlGetNsProp(templNode, (const xmlChar *)"name", NULL);
    if (prop != NULL) {
        const xmlChar *nameURI;
	xsltTemplatePtr curTempl;
	
	nameURI = xsltGetQNameURI(templNode, &prop);
	if (prop == NULL) {
	    cctxt->style->errors++;
	    goto error;
	}
	templ->name = xmlDictLookup(cctxt->style->dict, prop, -1);
	xmlFree(prop);
	prop = NULL;
	if (xmlValidateNCName(templ->name, 0)) {
	    xsltTransformError(NULL, cctxt->style, templNode,
		"xsl:template: Attribute 'name': The local part '%s' of "
		"the value is not a valid NCName.\n", templ->name);
	    cctxt->style->errors++;
	    goto error;
	} 	
	if (nameURI != NULL)
	    templ->nameURI = xmlDictLookup(cctxt->style->dict, nameURI, -1);
	curTempl = templ->next;
	while (curTempl != NULL) {
	    if ((nameURI != NULL && xmlStrEqual(curTempl->name, templ->name) &&
		xmlStrEqual(curTempl->nameURI, nameURI) ) ||
		(nameURI == NULL && curTempl->nameURI == NULL &&
		xmlStrEqual(curTempl->name, templ->name)))
	    {
		xsltTransformError(NULL, cctxt->style, templNode,
		    "xsl:template: error duplicate name '%s'\n", templ->name);
		cctxt->style->errors++;
		goto error;
	    }
	    curTempl = curTempl->next;
	}
    }
    if (templNode->children != NULL) {
	xsltParseTemplateContent(cctxt->style, templNode);	
		
    }    
    
    templ->elem = templNode;
    templ->content = templNode->children;
    xsltAddTemplate(cctxt->style, templ, templ->mode, templ->modeURI);

error:
    xsltCompilerNodePop(cctxt, templNode);
    return;
}

#else 


static void
xsltParseStylesheetTemplate(xsltStylesheetPtr style, xmlNodePtr template) {
    xsltTemplatePtr ret;
    xmlChar *prop;
    xmlChar *mode = NULL;
    xmlChar *modeURI = NULL;
    double  priority;

    if (template == NULL)
	return;

    ret = xsltNewTemplate();
    if (ret == NULL)
	return;
    ret->next = style->templates;
    style->templates = ret;
    ret->style = style;
   
    xsltGetInheritedNsList(style, ret, template);

    prop = xmlGetNsProp(template, (const xmlChar *)"mode", NULL);
    if (prop != NULL) {
        const xmlChar *URI;

	URI = xsltGetQNameURI(template, &prop);
	if (prop == NULL) {
	    if (style != NULL) style->errors++;
	    goto error;
	} else {
	    mode = prop;
	    if (URI != NULL)
		modeURI = xmlStrdup(URI);
	}
	ret->mode = xmlDictLookup(style->dict, mode, -1);
	ret->modeURI = xmlDictLookup(style->dict, modeURI, -1);
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
	     "xsltParseStylesheetTemplate: mode %s\n", mode);
#endif
        if (mode != NULL) xmlFree(mode);
	if (modeURI != NULL) xmlFree(modeURI);
    }
    prop = xmlGetNsProp(template, (const xmlChar *)"match", NULL);
    if (prop != NULL) {
	if (ret->match != NULL) xmlFree(ret->match);
	ret->match  = prop;
    }

    prop = xmlGetNsProp(template, (const xmlChar *)"priority", NULL);
    if (prop != NULL) {
	priority = xmlXPathStringEvalNumber(prop);
	ret->priority = (float) priority;
	xmlFree(prop);
    }

    prop = xmlGetNsProp(template, (const xmlChar *)"name", NULL);
    if (prop != NULL) {
        const xmlChar *URI;
	xsltTemplatePtr cur;
	
	URI = xsltGetQNameURI(template, &prop);
	if (prop == NULL) {
	    if (style != NULL) style->errors++;
	    goto error;
	} else {
	    if (xmlValidateNCName(prop,0)) {
	        xsltTransformError(NULL, style, template,
	            "xsl:template : error invalid name '%s'\n", prop);
		if (style != NULL) style->errors++;
		goto error;
	    }
	    ret->name = xmlDictLookup(style->dict, BAD_CAST prop, -1);
	    xmlFree(prop);
	    prop = NULL;
	    if (URI != NULL)
		ret->nameURI = xmlDictLookup(style->dict, BAD_CAST URI, -1);
	    else
		ret->nameURI = NULL;
	    cur = ret->next;
	    while (cur != NULL) {
	        if ((URI != NULL && xmlStrEqual(cur->name, ret->name) &&
				xmlStrEqual(cur->nameURI, URI) ) ||
		    (URI == NULL && cur->nameURI == NULL &&
				xmlStrEqual(cur->name, ret->name))) {
		    xsltTransformError(NULL, style, template,
		        "xsl:template: error duplicate name '%s'\n", ret->name);
		    style->errors++;
		    goto error;
		}
		cur = cur->next;
	    }
	}
    }

    xsltParseTemplateContent(style, template);
    ret->elem = template;
    ret->content = template->children;
    xsltAddTemplate(style, ret, ret->mode, ret->modeURI);

error:
    return;
}

#endif 

#ifdef XSLT_REFACTORED

static xsltStyleItemIncludePtr
xsltCompileXSLTIncludeElem(xsltCompilerCtxtPtr cctxt, xmlNodePtr node) {
    xsltStyleItemIncludePtr item;

    if ((cctxt == NULL) || (node == NULL))
	return(NULL);

    node->psvi = NULL;
    item = (xsltStyleItemIncludePtr) xmlMalloc(sizeof(xsltStyleItemInclude));
    if (item == NULL) {
	xsltTransformError(NULL, cctxt->style, node,
		"xsltIncludeComp : malloc failed\n");
	cctxt->style->errors++;
	return(NULL);
    }
    memset(item, 0, sizeof(xsltStyleItemInclude));

    node->psvi = item;
    item->inst = node;
    item->type = XSLT_FUNC_INCLUDE;

    item->next = cctxt->style->preComps;
    cctxt->style->preComps = (xsltElemPreCompPtr) item;

    return(item);
}

static int
xsltParseFindTopLevelElem(xsltCompilerCtxtPtr cctxt,
			      xmlNodePtr cur,
			      const xmlChar *name,
			      const xmlChar *namespaceURI,
			      int breakOnOtherElem,			      
			      xmlNodePtr *resultNode)
{
    if (name == NULL)
	return(-1);

    *resultNode = NULL;
    while (cur != NULL) {
	if (cur->type == XML_ELEMENT_NODE) {
	    if ((cur->ns != NULL) && (cur->name != NULL)) {
		if ((*(cur->name) == *name) &&
		    xmlStrEqual(cur->name, name) &&
		    xmlStrEqual(cur->ns->href, namespaceURI))		    
		{
		    *resultNode = cur;
		    return(1);
		}
	    }
	    if (breakOnOtherElem)
		break;
	}
	cur = cur->next;
    }
    *resultNode = cur;
    return(0);
}

static int
xsltParseTopLevelXSLTElem(xsltCompilerCtxtPtr cctxt,
			  xmlNodePtr node,
			  xsltStyleType type)
{
    int ret = 0;

    xsltCompilerNodePush(cctxt, node);
    if (node->nsDef != NULL)
	cctxt->inode->inScopeNs =
	    xsltCompilerBuildInScopeNsList(cctxt, node);
    cctxt->inode->type = type;

    switch (type) {
	case XSLT_FUNC_INCLUDE:
	    {
		int oldIsInclude;

		if (xsltCompileXSLTIncludeElem(cctxt, node) == NULL)
		    goto exit;		
		oldIsInclude = cctxt->isInclude;
		cctxt->isInclude = 1;
						
		if (xsltParseStylesheetInclude(cctxt->style, node) != 0) {
		    cctxt->style->errors++;
		}
		cctxt->isInclude = oldIsInclude;
	    }
	    break;
	case XSLT_FUNC_PARAM:
	    xsltStylePreCompute(cctxt->style, node);
	    xsltParseGlobalParam(cctxt->style, node);
	    break;
	case XSLT_FUNC_VARIABLE:
	    xsltStylePreCompute(cctxt->style, node);
	    xsltParseGlobalVariable(cctxt->style, node);
	    break;
	case XSLT_FUNC_ATTRSET:
	    xsltParseStylesheetAttributeSet(cctxt->style, node);
	    break;
	default:
	    xsltTransformError(NULL, cctxt->style, node,
		"Internal error: (xsltParseTopLevelXSLTElem) "
		"Cannot handle this top-level declaration.\n");
	    cctxt->style->errors++;
	    ret = -1;
    }

exit:
    xsltCompilerNodePop(cctxt, node);

    return(ret);
}

#if 0
static int
xsltParseRemoveWhitespace(xmlNodePtr node)
{
    if ((node == NULL) || (node->children == NULL))
	return(0);
    else {
	xmlNodePtr delNode = NULL, child = node->children;

	do {
	    if (delNode) {
		xmlUnlinkNode(delNode);
		xmlFreeNode(delNode);
		delNode = NULL;
	    }
	    if (((child->type == XML_TEXT_NODE) ||
		 (child->type == XML_CDATA_SECTION_NODE)) &&
		(IS_BLANK_NODE(child)))
		delNode = child;	    
	    child = child->next;
	} while (child != NULL);
	if (delNode) {
	    xmlUnlinkNode(delNode);
	    xmlFreeNode(delNode);
	    delNode = NULL;
	}
    }
    return(0);
}
#endif

static int
xsltParseXSLTStylesheetElemCore(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{
#ifdef WITH_XSLT_DEBUG_PARSING
    int templates = 0;
#endif
    xmlNodePtr cur, start = NULL;
    xsltStylesheetPtr style;

    if ((cctxt == NULL) || (node == NULL) ||
	(node->type != XML_ELEMENT_NODE))
	return(-1);    

    style = cctxt->style;    
    if (IS_XSLT_ELEM_FAST(node) && IS_XSLT_NAME(node, "include"))
    {
	xsltDocumentPtr include;
	include = ((xsltStyleItemIncludePtr) node->psvi)->include;
	if (include == NULL) {
	    
	    return(-1);
	}
	node = xmlDocGetRootElement(include->doc);
	if (node == NULL) {
	    return(-1);
	}
    }    
    
    if (node->children == NULL)
	return(0);
  
    xsltCompilerNodePush(cctxt, node);
    cctxt->inode->isRoot = 1;
    cctxt->inode->nsChanged = 0;
    cctxt->inode->litResElemInfo = cctxt->inodeList->litResElemInfo;

		
    cctxt->inode->inScopeNs =
	xsltCompilerBuildInScopeNsList(cctxt, node);

    if (xsltParseAttrXSLTVersion(cctxt, node,
	XSLT_ELEMENT_CATEGORY_XSLT) == 0)
    {    
#ifdef XSLT_REFACTORED_MANDATORY_VERSION
	if (isXsltElem)
	    xsltTransformError(NULL, cctxt->style, node,
		"The attribute 'version' is missing.\n");
	cctxt->style->errors++;	
#else
	
	xsltTransformError(NULL, cctxt->style, node,
	    "xsl:version is missing: document may not be a stylesheet\n");
	cctxt->style->warnings++;
#endif
    }    
    cctxt->inode->extElemNs =
	xsltParseExtElemPrefixes(cctxt, node, NULL,
	    XSLT_ELEMENT_CATEGORY_XSLT);
    cctxt->inode->exclResultNs =
	xsltParseExclResultPrefixes(cctxt, node, NULL,
	    XSLT_ELEMENT_CATEGORY_XSLT);
    if (cctxt->inode->nsChanged)
	xsltLREInfoCreate(cctxt, node, 0);
    cur = node->children;
    while (cur != NULL) {
	if (cur->type == XML_TEXT_NODE) {
	    xsltTransformError(NULL, style, cur,
		"Misplaced text node (content: '%s').\n",
		(cur->content != NULL) ? cur->content : BAD_CAST "");
	    style->errors++;
	} else if (cur->type != XML_ELEMENT_NODE) {
	    xsltTransformError(NULL, style, cur, "Misplaced node.\n");
	    style->errors++;
	}
	cur = cur->next;
    }
    cur = node->children;
    while ((cur != NULL) && xsltParseFindTopLevelElem(cctxt, cur,
	    BAD_CAST "import", XSLT_NAMESPACE, 1, &cur) == 1)
	cur = cur->next;
    if (cur == NULL)
	goto exit;

    start = cur;
    while ((cur != NULL) &&
	xsltParseFindTopLevelElem(cctxt, cur,
	BAD_CAST "param", XSLT_NAMESPACE, 0, &cur) == 1)
    {
	xsltParseTopLevelXSLTElem(cctxt, cur, XSLT_FUNC_PARAM);	
	cur = cur->next;
    }  
    cur = start;
    while ((cur != NULL) &&
	xsltParseFindTopLevelElem(cctxt, cur,
	BAD_CAST "variable", XSLT_NAMESPACE, 0, &cur) == 1)
    {
	xsltParseTopLevelXSLTElem(cctxt, cur, XSLT_FUNC_VARIABLE);
	cur = cur->next;
    }   
    cur = start;
    while (cur != NULL) {	
	if (cur->type == XML_ELEMENT_NODE) {	    
	    if (cur->ns == NULL) {
		xsltTransformError(NULL, style, cur,
		    "Unexpected top-level element in no namespace.\n");
		style->errors++;
		cur = cur->next;
		continue;
	    }
	    if (IS_XSLT_ELEM_FAST(cur)) {
		if (IS_XSLT_NAME(cur, "import")) {
		    xsltTransformError(NULL, style, cur,
			"Misplaced xsl:import element.\n");
		    style->errors++;
		    cur = cur->next;
		    continue;
		}
		if (IS_XSLT_NAME(cur, "template")) {
#ifdef WITH_XSLT_DEBUG_PARSING
		    templates++;
#endif
		    xsltParseXSLTTemplate(cctxt, cur);
		} else if (IS_XSLT_NAME(cur, "variable")) {
		    
		} else if (IS_XSLT_NAME(cur, "param")) {
		    
		} else if (IS_XSLT_NAME(cur, "include")) {		    
		    if (cur->psvi != NULL)		    
			xsltParseXSLTStylesheetElemCore(cctxt, cur);
		    else {
			xsltTransformError(NULL, style, cur,
			    "Internal error: "
			    "(xsltParseXSLTStylesheetElemCore) "
			    "The xsl:include element was not compiled.\n");
			style->errors++;
		    }
		} else if (IS_XSLT_NAME(cur, "strip-space")) {
		    
		    xsltParseStylesheetStripSpace(style, cur);
		} else if (IS_XSLT_NAME(cur, "preserve-space")) {
		    
		    xsltParseStylesheetPreserveSpace(style, cur);
		} else if (IS_XSLT_NAME(cur, "output")) {
		    
		    xsltParseStylesheetOutput(style, cur);
		} else if (IS_XSLT_NAME(cur, "key")) {
		    
		    xsltParseStylesheetKey(style, cur);
		} else if (IS_XSLT_NAME(cur, "decimal-format")) {
		    		     
		    xsltParseStylesheetDecimalFormat(style, cur);
		} else if (IS_XSLT_NAME(cur, "attribute-set")) {		    
		    xsltParseTopLevelXSLTElem(cctxt, cur,
			XSLT_FUNC_ATTRSET);		
		} else if (IS_XSLT_NAME(cur, "namespace-alias")) {
		    		    
		} else {
		    if (cctxt->inode->forwardsCompat) {
			xsltTransformError(NULL, style, cur,
			    "Forwards-compatible mode: Ignoring unknown XSLT "
			    "element '%s'.\n", cur->name);
			style->warnings++;
		    } else {
			xsltTransformError(NULL, style, cur,
			    "Unknown XSLT element '%s'.\n", cur->name);
			style->errors++;
		    }
		}
	    } else {
		xsltTopLevelFunction function;

		function = xsltExtModuleTopLevelLookup(cur->name,
		    cur->ns->href);
		if (function != NULL)
		    function(style, cur);
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
		    "xsltParseXSLTStylesheetElemCore : User-defined "
		    "data element '%s'.\n", cur->name);
#endif
	    }
	}
	cur = cur->next;
    }

exit:

#ifdef WITH_XSLT_DEBUG_PARSING
    xsltGenericDebug(xsltGenericDebugContext,
	"### END of parsing top-level elements of doc '%s'.\n",
	node->doc->URL);
    xsltGenericDebug(xsltGenericDebugContext,
	"### Templates: %d\n", templates);
#ifdef XSLT_REFACTORED
    xsltGenericDebug(xsltGenericDebugContext,
	"### Max inodes: %d\n", cctxt->maxNodeInfos);
    xsltGenericDebug(xsltGenericDebugContext,
	"### Max LREs  : %d\n", cctxt->maxLREs);
#endif 
#endif 

    xsltCompilerNodePop(cctxt, node);
    return(0);
}

static int
xsltParseXSLTStylesheetElem(xsltCompilerCtxtPtr cctxt, xmlNodePtr node)
{
    xmlNodePtr cur, start;

    if ((cctxt == NULL) || (node == NULL))
	return(-1);
    
    if (node->children == NULL)
	goto exit;

    cur = node->children;
    
    while ((cur != NULL) &&
	xsltParseFindTopLevelElem(cctxt, cur,
	    BAD_CAST "import", XSLT_NAMESPACE, 1, &cur) == 1)
    {
	if (xsltParseStylesheetImport(cctxt->style, cur) != 0) {
	    cctxt->style->errors++;
	}
	cur = cur->next;
    }
    if (cur == NULL)
	goto exit;
    start = cur;
    cur = start;
    while ((cur != NULL) &&
	xsltParseFindTopLevelElem(cctxt, cur,
	    BAD_CAST "include", XSLT_NAMESPACE, 0, &cur) == 1)
    {
	xsltParseTopLevelXSLTElem(cctxt, cur, XSLT_FUNC_INCLUDE);
	cur = cur->next;
    }
    cur = start;
    while ((cur != NULL) &&
	xsltParseFindTopLevelElem(cctxt, cur,
	    BAD_CAST "namespace-alias", XSLT_NAMESPACE, 0, &cur) == 1)
    {
	xsltNamespaceAlias(cctxt->style, cur);
	cur = cur->next;
    }

    if (cctxt->isInclude) {
	goto exit;
    } 
    xsltParseXSLTStylesheetElemCore(cctxt, node); 	
exit:

    return(0);
}

#else 

static void
xsltParseStylesheetTop(xsltStylesheetPtr style, xmlNodePtr top) {
    xmlNodePtr cur;
    xmlChar *prop;
#ifdef WITH_XSLT_DEBUG_PARSING
    int templates = 0;
#endif

    if (top == NULL)
	return;

    prop = xmlGetNsProp(top, (const xmlChar *)"version", NULL);
    if (prop == NULL) {
	xsltTransformError(NULL, style, top,
	    "xsl:version is missing: document may not be a stylesheet\n");
	if (style != NULL) style->warnings++;
    } else {
	if ((!xmlStrEqual(prop, (const xmlChar *)"1.0")) &&
            (!xmlStrEqual(prop, (const xmlChar *)"1.1"))) {
	    xsltTransformError(NULL, style, top,
		"xsl:version: only 1.0 features are supported\n");
	     
	    if (style != NULL) style->warnings++;
	}
	xmlFree(prop);
    }

    cur = top->children;
    while (cur != NULL) {
	    if (IS_BLANK_NODE(cur)) {
		    cur = cur->next;
		    continue;
	    }
	    if (IS_XSLT_ELEM(cur) && IS_XSLT_NAME(cur, "import")) {
		    if (xsltParseStylesheetImport(style, cur) != 0)
			    if (style != NULL) style->errors++;
	    } else
		    break;
	    cur = cur->next;
    }

    while (cur != NULL) {
	if (IS_BLANK_NODE(cur)) {
	    cur = cur->next;
	    continue;
	}
	if (cur->type == XML_TEXT_NODE) {
	    if (cur->content != NULL) {
		xsltTransformError(NULL, style, cur,
		    "misplaced text node: '%s'\n", cur->content);
	    }
	    if (style != NULL) style->errors++;
            cur = cur->next;
	    continue;
	}
	if ((cur->type == XML_ELEMENT_NODE) && (cur->ns == NULL)) {
	    xsltGenericError(xsltGenericErrorContext,
		     "Found a top-level element %s with null namespace URI\n",
		     cur->name);
	    if (style != NULL) style->errors++;
	    cur = cur->next;
	    continue;
	}
	if ((cur->type == XML_ELEMENT_NODE) && (!(IS_XSLT_ELEM(cur)))) {
	    xsltTopLevelFunction function;

	    function = xsltExtModuleTopLevelLookup(cur->name,
						   cur->ns->href);
	    if (function != NULL)
		function(style, cur);

#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltParseStylesheetTop : found foreign element %s\n",
		    cur->name);
#endif
            cur = cur->next;
	    continue;
	}
	if (IS_XSLT_NAME(cur, "import")) {
	    xsltTransformError(NULL, style, cur,
			"xsltParseStylesheetTop: ignoring misplaced import element\n");
	    if (style != NULL) style->errors++;
    } else if (IS_XSLT_NAME(cur, "include")) {
	    if (xsltParseStylesheetInclude(style, cur) != 0)
		if (style != NULL) style->errors++;
    } else if (IS_XSLT_NAME(cur, "strip-space")) {
	    xsltParseStylesheetStripSpace(style, cur);
    } else if (IS_XSLT_NAME(cur, "preserve-space")) {
	    xsltParseStylesheetPreserveSpace(style, cur);
    } else if (IS_XSLT_NAME(cur, "output")) {
	    xsltParseStylesheetOutput(style, cur);
    } else if (IS_XSLT_NAME(cur, "key")) {
	    xsltParseStylesheetKey(style, cur);
    } else if (IS_XSLT_NAME(cur, "decimal-format")) {
	    xsltParseStylesheetDecimalFormat(style, cur);
    } else if (IS_XSLT_NAME(cur, "attribute-set")) {
	    xsltParseStylesheetAttributeSet(style, cur);
    } else if (IS_XSLT_NAME(cur, "variable")) {
	    xsltParseGlobalVariable(style, cur);
    } else if (IS_XSLT_NAME(cur, "param")) {
	    xsltParseGlobalParam(style, cur);
    } else if (IS_XSLT_NAME(cur, "template")) {
#ifdef WITH_XSLT_DEBUG_PARSING
	    templates++;
#endif
	    xsltParseStylesheetTemplate(style, cur);
    } else if (IS_XSLT_NAME(cur, "namespace-alias")) {
	    xsltNamespaceAlias(style, cur);
	} else {
            if ((style != NULL) && (style->doc->version != NULL) &&
	        (!strncmp((const char *) style->doc->version, "1.0", 3))) {
	        xsltTransformError(NULL, style, cur,
			"xsltParseStylesheetTop: unknown %s element\n",
			cur->name);
	        if (style != NULL) style->errors++;
	    }
	    else {
                
	        xsltTransformError(NULL, style, cur,
			"xsltParseStylesheetTop: ignoring unknown %s element\n",
			cur->name);
	        if (style != NULL) style->warnings++;
            }
	}
	cur = cur->next;
    }
#ifdef WITH_XSLT_DEBUG_PARSING
    xsltGenericDebug(xsltGenericDebugContext,
		    "parsed %d templates\n", templates);
#endif
}

#endif 

#ifdef XSLT_REFACTORED
static int
xsltParseSimplifiedStylesheetTree(xsltCompilerCtxtPtr cctxt,
				  xmlDocPtr doc,
				  xmlNodePtr node)
{
    xsltTemplatePtr templ;
    
    if ((cctxt == NULL) || (node == NULL))
	return(-1);

    if (xsltParseAttrXSLTVersion(cctxt, node, 0) == XSLT_ELEMENT_CATEGORY_LRE)
    {
	xsltTransformError(NULL, cctxt->style, node,
	    "The attribute 'xsl:version' is missing; cannot identify "
	    "this document as an XSLT stylesheet document.\n");
	cctxt->style->errors++;
	return(1);
    }
    
#ifdef WITH_XSLT_DEBUG_PARSING
    xsltGenericDebug(xsltGenericDebugContext,
	"xsltParseSimplifiedStylesheetTree: document is stylesheet\n");
#endif        
    
    templ = xsltNewTemplate();
    if (templ == NULL) {
	return(-1);
    }
    templ->next = cctxt->style->templates;
    cctxt->style->templates = templ;
    templ->match = xmlStrdup(BAD_CAST "/");

    xsltCompilerNodePush(cctxt, (xmlNodePtr) doc);
    cctxt->inode->inScopeNs =
	xsltCompilerBuildInScopeNsList(cctxt, node);
    xsltParseSequenceConstructor(cctxt, node);
    xsltCompilerNodePop(cctxt, (xmlNodePtr) doc);

    templ->elem = (xmlNodePtr) doc;
    templ->content = node;
    xsltAddTemplate(cctxt->style, templ, NULL, NULL);
    cctxt->style->literal_result = 1;
    return(0);
}

#ifdef XSLT_REFACTORED_XSLT_NSCOMP
int
xsltRestoreDocumentNamespaces(xsltNsMapPtr ns, xmlDocPtr doc)
{
    if (doc == NULL)
	return(-1);
    
    while (ns != NULL) {
	if ((ns->doc == doc) && (ns->ns != NULL)) {
	    ns->ns->href = ns->origNsName;
	    ns->origNsName = NULL;
	    ns->ns = NULL;	    
	}
	ns = ns->next;
    }
    return(0);
}
#endif 

xsltStylesheetPtr
xsltParseStylesheetProcess(xsltStylesheetPtr style, xmlDocPtr doc)
{
    xsltCompilerCtxtPtr cctxt;
    xmlNodePtr cur;
    int oldIsSimplifiedStylesheet;

    xsltInitGlobals();

    if ((style == NULL) || (doc == NULL))
	return(NULL);

    cctxt = XSLT_CCTXT(style);

    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
	xsltTransformError(NULL, style, (xmlNodePtr) doc,
		"xsltParseStylesheetProcess : empty stylesheet\n");
	return(NULL);
    }
    oldIsSimplifiedStylesheet = cctxt->simplified;

    if ((IS_XSLT_ELEM(cur)) && 
	((IS_XSLT_NAME(cur, "stylesheet")) ||
	 (IS_XSLT_NAME(cur, "transform")))) {	
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
		"xsltParseStylesheetProcess : found stylesheet\n");
#endif
	cctxt->simplified = 0;
	style->literal_result = 0;
    } else {
	cctxt->simplified = 1;
	style->literal_result = 1;
    }
    if (! style->nopreproc)
	xsltParsePreprocessStylesheetTree(cctxt, cur);
    if (style->literal_result == 0) {
	if (xsltParseXSLTStylesheetElem(cctxt, cur) != 0)
	    return(NULL);
    } else {
	if (xsltParseSimplifiedStylesheetTree(cctxt, doc, cur) != 0)
	    return(NULL);
    }    

    cctxt->simplified = oldIsSimplifiedStylesheet;

    return(style);
}

#else 

xsltStylesheetPtr
xsltParseStylesheetProcess(xsltStylesheetPtr ret, xmlDocPtr doc) {
    xmlNodePtr cur;

    xsltInitGlobals();

    if (doc == NULL)
	return(NULL);
    if (ret == NULL)
	return(ret);
    
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
	xsltTransformError(NULL, ret, (xmlNodePtr) doc,
		"xsltParseStylesheetProcess : empty stylesheet\n");
	return(NULL);
    }

    if ((IS_XSLT_ELEM(cur)) && 
	((IS_XSLT_NAME(cur, "stylesheet")) ||
	 (IS_XSLT_NAME(cur, "transform")))) {	
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
		"xsltParseStylesheetProcess : found stylesheet\n");
#endif
	ret->literal_result = 0;
	xsltParseStylesheetExcludePrefix(ret, cur, 1);
	xsltParseStylesheetExtPrefix(ret, cur, 1);
    } else {
	xsltParseStylesheetExcludePrefix(ret, cur, 0);
	xsltParseStylesheetExtPrefix(ret, cur, 0);
	ret->literal_result = 1;
    }
    if (!ret->nopreproc) {
	xsltPrecomputeStylesheet(ret, cur);
    }
    if (ret->literal_result == 0) {
	xsltParseStylesheetTop(ret, cur);
    } else {
	xmlChar *prop;
	xsltTemplatePtr template;

	prop = xmlGetNsProp(cur, (const xmlChar *)"version", XSLT_NAMESPACE);
	if (prop == NULL) {
	    xsltTransformError(NULL, ret, cur,
		"xsltParseStylesheetProcess : document is not a stylesheet\n");
	    return(NULL);
	}

#ifdef WITH_XSLT_DEBUG_PARSING
        xsltGenericDebug(xsltGenericDebugContext,
		"xsltParseStylesheetProcess : document is stylesheet\n");
#endif
	
	if (!xmlStrEqual(prop, (const xmlChar *)"1.0")) {
	    xsltTransformError(NULL, ret, cur,
		"xsl:version: only 1.0 features are supported\n");
	     
	    ret->warnings++;
	}
	xmlFree(prop);

	template = xsltNewTemplate();
	if (template == NULL) {
	    return(NULL);
	}
	template->next = ret->templates;
	ret->templates = template;
	template->match = xmlStrdup((const xmlChar *)"/");

	xsltParseTemplateContent(ret, (xmlNodePtr) doc);
	template->elem = (xmlNodePtr) doc;
	template->content = doc->children;
	xsltAddTemplate(ret, template, NULL, NULL);
	ret->literal_result = 1;	
    }

    return(ret);
}

#endif 


xsltStylesheetPtr
xsltParseStylesheetImportedDoc(xmlDocPtr doc,
			       xsltStylesheetPtr parentStyle) {
    xsltStylesheetPtr retStyle;

    if (doc == NULL)
	return(NULL);

    retStyle = xsltNewStylesheet();
    if (retStyle == NULL)
	return(NULL);
    retStyle->parent = parentStyle;
    if (doc->dict != NULL) {
        xmlDictFree(retStyle->dict);
	retStyle->dict = doc->dict;
#ifdef WITH_XSLT_DEBUG
        xsltGenericDebug(xsltGenericDebugContext,
	    "reusing dictionary from %s for stylesheet\n",
	    doc->URL);
#endif
	xmlDictReference(retStyle->dict);
    }    	
    
    xsltGatherNamespaces(retStyle);

#ifdef XSLT_REFACTORED
    {
	xsltCompilerCtxtPtr cctxt;
	xsltStylesheetPtr oldCurSheet;
	    
	if (parentStyle == NULL) {
	    xsltPrincipalStylesheetDataPtr principalData;
	    retStyle->principal = retStyle;
	    principalData = xsltNewPrincipalStylesheetData();
	    if (principalData == NULL) {
		xsltFreeStylesheet(retStyle);
		return(NULL);
	    }
	    retStyle->principalData = principalData;
	    cctxt = xsltCompilationCtxtCreate(retStyle);
	    if (cctxt == NULL) {
		xsltFreeStylesheet(retStyle);
		return(NULL);
	    }	    	    
	    retStyle->compCtxt = (void *) cctxt;
	    cctxt->style = retStyle;
	    cctxt->dict = retStyle->dict;
	    cctxt->psData = principalData;
	    cctxt->depth = -1;
	    xsltCompilerNodePush(cctxt, (xmlNodePtr) doc);
	} else {
	    retStyle->principal = parentStyle->principal;
	    cctxt = parentStyle->compCtxt;
	    retStyle->compCtxt = cctxt;
	}
	oldCurSheet = cctxt->style;
	cctxt->style = retStyle;
	
	retStyle->doc = doc;
	xsltParseStylesheetProcess(retStyle, doc);
	
	cctxt->style = oldCurSheet;
	if (parentStyle == NULL) {
	    xsltCompilerNodePop(cctxt, (xmlNodePtr) doc);
	} else {
	    
	}
	if (retStyle != NULL) {
	    if (retStyle->errors != 0) {
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
		if (cctxt->psData->nsMap)		
		    xsltRestoreDocumentNamespaces(cctxt->psData->nsMap, doc);
#endif
		retStyle->doc = NULL;
		if (parentStyle == NULL) {
		    xsltCleanupStylesheetTree(doc, xmlDocGetRootElement(doc));
		    if (retStyle->compCtxt != NULL) {			
			xsltCompilationCtxtFree(retStyle->compCtxt);
			retStyle->compCtxt = NULL;
		    }
		}

		xsltFreeStylesheet(retStyle);
		retStyle = NULL;
	    }
	}
    }
        
#else 
    retStyle->doc = doc;
    if (xsltParseStylesheetProcess(retStyle, doc) == NULL) {
		retStyle->doc = NULL;
		xsltFreeStylesheet(retStyle);
		retStyle = NULL;
    }
    if (retStyle != NULL) {
	if (retStyle->errors != 0) {
	    retStyle->doc = NULL;
	    if (parentStyle == NULL)
		xsltCleanupStylesheetTree(doc,
		    xmlDocGetRootElement(doc));
	    xsltFreeStylesheet(retStyle);
	    retStyle = NULL;
	}
    }
#endif 
        
    return(retStyle);
}


xsltStylesheetPtr
xsltParseStylesheetDoc(xmlDocPtr doc) {
    xsltStylesheetPtr ret;

    xsltInitGlobals();

    ret = xsltParseStylesheetImportedDoc(doc, NULL);
    if (ret == NULL)
	return(NULL);

    xsltResolveStylesheetAttributeSet(ret);
#ifdef XSLT_REFACTORED
    if (ret->compCtxt != NULL) {
	xsltCompilationCtxtFree(XSLT_CCTXT(ret));
	ret->compCtxt = NULL;
    }
#endif
    return(ret);
}


xsltStylesheetPtr
xsltParseStylesheetFile(const xmlChar* filename) {
    xsltSecurityPrefsPtr sec;
    xsltStylesheetPtr ret;
    xmlDocPtr doc;
    
    xsltInitGlobals();

    if (filename == NULL)
	return(NULL);

#ifdef WITH_XSLT_DEBUG_PARSING
    xsltGenericDebug(xsltGenericDebugContext,
	    "xsltParseStylesheetFile : parse %s\n", filename);
#endif

    sec = xsltGetDefaultSecurityPrefs();
    if (sec != NULL) {
	int res;

	res = xsltCheckRead(sec, NULL, filename);
	if (res == 0) {
	    xsltTransformError(NULL, NULL, NULL,
		 "xsltParseStylesheetFile: read rights for %s denied\n",
			     filename);
	    return(NULL);
	}
    }

    doc = xsltDocDefaultLoader(filename, NULL, XSLT_PARSE_OPTIONS,
                               NULL, XSLT_LOAD_START);
    if (doc == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltParseStylesheetFile : cannot parse %s\n", filename);
	return(NULL);
    }
    ret = xsltParseStylesheetDoc(doc);
    if (ret == NULL) {
	xmlFreeDoc(doc);
	return(NULL);
    }

    return(ret);
}


#define CUR (*cur)
#define SKIP(val) cur += (val)
#define NXT(val) cur[(val)]
#define SKIP_BLANKS						\
    while (IS_BLANK(CUR)) NEXT
#define NEXT ((*cur) ?  cur++ : cur)

static xmlChar *
xsltParseStylesheetPI(const xmlChar *value) {
    const xmlChar *cur;
    const xmlChar *start;
    xmlChar *val;
    xmlChar tmp;
    xmlChar *href = NULL;
    int isXml = 0;

    if (value == NULL)
	return(NULL);

    cur = value;
    while (CUR != 0) {
	SKIP_BLANKS;
	if ((CUR == 't') && (NXT(1) == 'y') && (NXT(2) == 'p') &&
	    (NXT(3) == 'e')) {
	    SKIP(4);
	    SKIP_BLANKS;
	    if (CUR != '=')
		continue;
	    NEXT;
	    if ((CUR != '\'') && (CUR != '"'))
		continue;
	    tmp = CUR;
	    NEXT;
	    start = cur;
	    while ((CUR != 0) && (CUR != tmp))
		NEXT;
	    if (CUR != tmp)
		continue;
	    val = xmlStrndup(start, cur - start);
	    NEXT;
	    if (val == NULL) 
		return(NULL);
	    if ((xmlStrcasecmp(val, BAD_CAST "text/xml")) &&
		(xmlStrcasecmp(val, BAD_CAST "text/xsl"))) {
                xmlFree(val);
		break;
	    }
	    isXml = 1;
	    xmlFree(val);
	} else if ((CUR == 'h') && (NXT(1) == 'r') && (NXT(2) == 'e') &&
	    (NXT(3) == 'f')) {
	    SKIP(4);
	    SKIP_BLANKS;
	    if (CUR != '=')
		continue;
	    NEXT;
	    if ((CUR != '\'') && (CUR != '"'))
		continue;
	    tmp = CUR;
	    NEXT;
	    start = cur;
	    while ((CUR != 0) && (CUR != tmp))
		NEXT;
	    if (CUR != tmp)
		continue;
	    if (href == NULL)
		href = xmlStrndup(start, cur - start);
	    NEXT;
	} else {
	    while ((CUR != 0) && (!IS_BLANK(CUR)))
		NEXT;
	}
            
    }

    if (!isXml) {
	if (href != NULL)
	    xmlFree(href);
	href = NULL;
    }
    return(href);
}

xsltStylesheetPtr
xsltLoadStylesheetPI(xmlDocPtr doc) {
    xmlNodePtr child;
    xsltStylesheetPtr ret = NULL;
    xmlChar *href = NULL;
    xmlURIPtr URI;

    xsltInitGlobals();

    if (doc == NULL)
	return(NULL);

    child = doc->children;
    while ((child != NULL) && (child->type != XML_ELEMENT_NODE)) {
	if ((child->type == XML_PI_NODE) &&
	    (xmlStrEqual(child->name, BAD_CAST "xml-stylesheet"))) {
	    href = xsltParseStylesheetPI(child->content);
	    if (href != NULL)
		break;
	}
	child = child->next;
    }

    if (href != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
	xsltGenericDebug(xsltGenericDebugContext,
		"xsltLoadStylesheetPI : found PI href=%s\n", href);
#endif
	URI = xmlParseURI((const char *) href);
	if (URI == NULL) {
	    xsltTransformError(NULL, NULL, child,
		    "xml-stylesheet : href %s is not valid\n", href);
	    xmlFree(href);
	    return(NULL);
	}
	if ((URI->fragment != NULL) && (URI->scheme == NULL) &&
            (URI->opaque == NULL) && (URI->authority == NULL) &&
            (URI->server == NULL) && (URI->user == NULL) &&
            (URI->path == NULL) && (URI->query == NULL)) {
	    xmlAttrPtr ID;

#ifdef WITH_XSLT_DEBUG_PARSING
	    xsltGenericDebug(xsltGenericDebugContext,
		    "xsltLoadStylesheetPI : Reference to ID %s\n", href);
#endif
	    if (URI->fragment[0] == '#')
		ID = xmlGetID(doc, (const xmlChar *) &(URI->fragment[1]));
	    else
		ID = xmlGetID(doc, (const xmlChar *) URI->fragment);
	    if (ID == NULL) {
		xsltTransformError(NULL, NULL, child,
		    "xml-stylesheet : no ID %s found\n", URI->fragment);
	    } else {
		xmlDocPtr fake;
		xmlNodePtr subtree, newtree;
		xmlNsPtr ns;

#ifdef WITH_XSLT_DEBUG
		xsltGenericDebug(xsltGenericDebugContext,
		    "creating new document from %s for embedded stylesheet\n",
		    doc->URL);
#endif
		subtree = ID->parent;
		fake = xmlNewDoc(NULL);
		if (fake != NULL) {
		    fake->dict = doc->dict;
		    xmlDictReference(doc->dict);
#ifdef WITH_XSLT_DEBUG
		    xsltGenericDebug(xsltGenericDebugContext,
			"reusing dictionary from %s for embedded stylesheet\n",
			doc->URL);
#endif

		    newtree = xmlDocCopyNode(subtree, fake, 1);

		    fake->URL = xmlNodeGetBase(doc, subtree->parent);
#ifdef WITH_XSLT_DEBUG
		    xsltGenericDebug(xsltGenericDebugContext,
			"set base URI for embedded stylesheet as %s\n",
			fake->URL);
#endif

		    while ((subtree = subtree->parent) != (xmlNodePtr)doc) {
			for (ns = subtree->ns; ns; ns = ns->next) {
			    xmlNewNs(newtree,  ns->href, ns->prefix);
			}
		    }

		    xmlAddChild((xmlNodePtr)fake, newtree);
		    ret = xsltParseStylesheetDoc(fake);
		    if (ret == NULL)
			xmlFreeDoc(fake);
		}
	    }
	} else {
	    xmlChar *URL, *base;


	    base = xmlNodeGetBase(doc, (xmlNodePtr) doc);
	    URL = xmlBuildURI(href, base);
	    if (URL != NULL) {
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
			"xsltLoadStylesheetPI : fetching %s\n", URL);
#endif
		ret = xsltParseStylesheetFile(URL);
		xmlFree(URL);
	    } else {
#ifdef WITH_XSLT_DEBUG_PARSING
		xsltGenericDebug(xsltGenericDebugContext,
			"xsltLoadStylesheetPI : fetching %s\n", href);
#endif
		ret = xsltParseStylesheetFile(href);
	    }
	    if (base != NULL)
		xmlFree(base);
	}
	xmlFreeURI(URI);
	xmlFree(href);
    }
    return(ret);
}
