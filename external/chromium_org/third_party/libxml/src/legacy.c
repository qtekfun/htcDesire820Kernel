/*
 * legacy.c: set of deprecated routines, not to be used anymore but
 *           kept purely for ABI compatibility
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#ifdef LIBXML_LEGACY_ENABLED
#include <string.h>

#include <libxml/tree.h>
#include <libxml/entities.h>
#include <libxml/SAX.h>
#include <libxml/parserInternals.h>
#include <libxml/HTMLparser.h>

void xmlUpgradeOldNs(xmlDocPtr doc);


#ifdef LIBXML_HTML_ENABLED
xmlChar *htmlDecodeEntities(htmlParserCtxtPtr ctxt, int len, xmlChar end,
                            xmlChar end2, xmlChar end3);

xmlChar *
htmlDecodeEntities(htmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED,
                   int len ATTRIBUTE_UNUSED, xmlChar end ATTRIBUTE_UNUSED,
                   xmlChar end2 ATTRIBUTE_UNUSED,
                   xmlChar end3 ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "htmlDecodeEntities() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}
#endif

void
xmlInitializePredefinedEntities(void)
{
}

void
xmlCleanupPredefinedEntities(void)
{
}

static const char *xmlFeaturesList[] = {
    "validate",
    "load subset",
    "keep blanks",
    "disable SAX",
    "fetch external entities",
    "substitute entities",
    "gather line info",
    "user data",
    "is html",
    "is standalone",
    "stop parser",
    "document",
    "is well formed",
    "is valid",
    "SAX block",
    "SAX function internalSubset",
    "SAX function isStandalone",
    "SAX function hasInternalSubset",
    "SAX function hasExternalSubset",
    "SAX function resolveEntity",
    "SAX function getEntity",
    "SAX function entityDecl",
    "SAX function notationDecl",
    "SAX function attributeDecl",
    "SAX function elementDecl",
    "SAX function unparsedEntityDecl",
    "SAX function setDocumentLocator",
    "SAX function startDocument",
    "SAX function endDocument",
    "SAX function startElement",
    "SAX function endElement",
    "SAX function reference",
    "SAX function characters",
    "SAX function ignorableWhitespace",
    "SAX function processingInstruction",
    "SAX function comment",
    "SAX function warning",
    "SAX function error",
    "SAX function fatalError",
    "SAX function getParameterEntity",
    "SAX function cdataBlock",
    "SAX function externalSubset",
};

int
xmlGetFeaturesList(int *len, const char **result)
{
    int ret, i;

    ret = sizeof(xmlFeaturesList) / sizeof(xmlFeaturesList[0]);
    if ((len == NULL) || (result == NULL))
        return (ret);
    if ((*len < 0) || (*len >= 1000))
        return (-1);
    if (*len > ret)
        *len = ret;
    for (i = 0; i < *len; i++)
        result[i] = xmlFeaturesList[i];
    return (ret);
}

int
xmlGetFeature(xmlParserCtxtPtr ctxt, const char *name, void *result)
{
    if ((ctxt == NULL) || (name == NULL) || (result == NULL))
        return (-1);

    if (!strcmp(name, "validate")) {
        *((int *) result) = ctxt->validate;
    } else if (!strcmp(name, "keep blanks")) {
        *((int *) result) = ctxt->keepBlanks;
    } else if (!strcmp(name, "disable SAX")) {
        *((int *) result) = ctxt->disableSAX;
    } else if (!strcmp(name, "fetch external entities")) {
        *((int *) result) = ctxt->loadsubset;
    } else if (!strcmp(name, "substitute entities")) {
        *((int *) result) = ctxt->replaceEntities;
    } else if (!strcmp(name, "gather line info")) {
        *((int *) result) = ctxt->record_info;
    } else if (!strcmp(name, "user data")) {
        *((void **) result) = ctxt->userData;
    } else if (!strcmp(name, "is html")) {
        *((int *) result) = ctxt->html;
    } else if (!strcmp(name, "is standalone")) {
        *((int *) result) = ctxt->standalone;
    } else if (!strcmp(name, "document")) {
        *((xmlDocPtr *) result) = ctxt->myDoc;
    } else if (!strcmp(name, "is well formed")) {
        *((int *) result) = ctxt->wellFormed;
    } else if (!strcmp(name, "is valid")) {
        *((int *) result) = ctxt->valid;
    } else if (!strcmp(name, "SAX block")) {
        *((xmlSAXHandlerPtr *) result) = ctxt->sax;
    } else if (!strcmp(name, "SAX function internalSubset")) {
        *((internalSubsetSAXFunc *) result) = ctxt->sax->internalSubset;
    } else if (!strcmp(name, "SAX function isStandalone")) {
        *((isStandaloneSAXFunc *) result) = ctxt->sax->isStandalone;
    } else if (!strcmp(name, "SAX function hasInternalSubset")) {
        *((hasInternalSubsetSAXFunc *) result) =
            ctxt->sax->hasInternalSubset;
    } else if (!strcmp(name, "SAX function hasExternalSubset")) {
        *((hasExternalSubsetSAXFunc *) result) =
            ctxt->sax->hasExternalSubset;
    } else if (!strcmp(name, "SAX function resolveEntity")) {
        *((resolveEntitySAXFunc *) result) = ctxt->sax->resolveEntity;
    } else if (!strcmp(name, "SAX function getEntity")) {
        *((getEntitySAXFunc *) result) = ctxt->sax->getEntity;
    } else if (!strcmp(name, "SAX function entityDecl")) {
        *((entityDeclSAXFunc *) result) = ctxt->sax->entityDecl;
    } else if (!strcmp(name, "SAX function notationDecl")) {
        *((notationDeclSAXFunc *) result) = ctxt->sax->notationDecl;
    } else if (!strcmp(name, "SAX function attributeDecl")) {
        *((attributeDeclSAXFunc *) result) = ctxt->sax->attributeDecl;
    } else if (!strcmp(name, "SAX function elementDecl")) {
        *((elementDeclSAXFunc *) result) = ctxt->sax->elementDecl;
    } else if (!strcmp(name, "SAX function unparsedEntityDecl")) {
        *((unparsedEntityDeclSAXFunc *) result) =
            ctxt->sax->unparsedEntityDecl;
    } else if (!strcmp(name, "SAX function setDocumentLocator")) {
        *((setDocumentLocatorSAXFunc *) result) =
            ctxt->sax->setDocumentLocator;
    } else if (!strcmp(name, "SAX function startDocument")) {
        *((startDocumentSAXFunc *) result) = ctxt->sax->startDocument;
    } else if (!strcmp(name, "SAX function endDocument")) {
        *((endDocumentSAXFunc *) result) = ctxt->sax->endDocument;
    } else if (!strcmp(name, "SAX function startElement")) {
        *((startElementSAXFunc *) result) = ctxt->sax->startElement;
    } else if (!strcmp(name, "SAX function endElement")) {
        *((endElementSAXFunc *) result) = ctxt->sax->endElement;
    } else if (!strcmp(name, "SAX function reference")) {
        *((referenceSAXFunc *) result) = ctxt->sax->reference;
    } else if (!strcmp(name, "SAX function characters")) {
        *((charactersSAXFunc *) result) = ctxt->sax->characters;
    } else if (!strcmp(name, "SAX function ignorableWhitespace")) {
        *((ignorableWhitespaceSAXFunc *) result) =
            ctxt->sax->ignorableWhitespace;
    } else if (!strcmp(name, "SAX function processingInstruction")) {
        *((processingInstructionSAXFunc *) result) =
            ctxt->sax->processingInstruction;
    } else if (!strcmp(name, "SAX function comment")) {
        *((commentSAXFunc *) result) = ctxt->sax->comment;
    } else if (!strcmp(name, "SAX function warning")) {
        *((warningSAXFunc *) result) = ctxt->sax->warning;
    } else if (!strcmp(name, "SAX function error")) {
        *((errorSAXFunc *) result) = ctxt->sax->error;
    } else if (!strcmp(name, "SAX function fatalError")) {
        *((fatalErrorSAXFunc *) result) = ctxt->sax->fatalError;
    } else if (!strcmp(name, "SAX function getParameterEntity")) {
        *((getParameterEntitySAXFunc *) result) =
            ctxt->sax->getParameterEntity;
    } else if (!strcmp(name, "SAX function cdataBlock")) {
        *((cdataBlockSAXFunc *) result) = ctxt->sax->cdataBlock;
    } else if (!strcmp(name, "SAX function externalSubset")) {
        *((externalSubsetSAXFunc *) result) = ctxt->sax->externalSubset;
    } else {
        return (-1);
    }
    return (0);
}

int
xmlSetFeature(xmlParserCtxtPtr ctxt, const char *name, void *value)
{
    if ((ctxt == NULL) || (name == NULL) || (value == NULL))
        return (-1);

    if (!strcmp(name, "validate")) {
        int newvalidate = *((int *) value);

        if ((!ctxt->validate) && (newvalidate != 0)) {
            if (ctxt->vctxt.warning == NULL)
                ctxt->vctxt.warning = xmlParserValidityWarning;
            if (ctxt->vctxt.error == NULL)
                ctxt->vctxt.error = xmlParserValidityError;
            ctxt->vctxt.nodeMax = 0;
        }
        ctxt->validate = newvalidate;
    } else if (!strcmp(name, "keep blanks")) {
        ctxt->keepBlanks = *((int *) value);
    } else if (!strcmp(name, "disable SAX")) {
        ctxt->disableSAX = *((int *) value);
    } else if (!strcmp(name, "fetch external entities")) {
        ctxt->loadsubset = *((int *) value);
    } else if (!strcmp(name, "substitute entities")) {
        ctxt->replaceEntities = *((int *) value);
    } else if (!strcmp(name, "gather line info")) {
        ctxt->record_info = *((int *) value);
    } else if (!strcmp(name, "user data")) {
        ctxt->userData = *((void **) value);
    } else if (!strcmp(name, "is html")) {
        ctxt->html = *((int *) value);
    } else if (!strcmp(name, "is standalone")) {
        ctxt->standalone = *((int *) value);
    } else if (!strcmp(name, "document")) {
        ctxt->myDoc = *((xmlDocPtr *) value);
    } else if (!strcmp(name, "is well formed")) {
        ctxt->wellFormed = *((int *) value);
    } else if (!strcmp(name, "is valid")) {
        ctxt->valid = *((int *) value);
    } else if (!strcmp(name, "SAX block")) {
        ctxt->sax = *((xmlSAXHandlerPtr *) value);
    } else if (!strcmp(name, "SAX function internalSubset")) {
        ctxt->sax->internalSubset = *((internalSubsetSAXFunc *) value);
    } else if (!strcmp(name, "SAX function isStandalone")) {
        ctxt->sax->isStandalone = *((isStandaloneSAXFunc *) value);
    } else if (!strcmp(name, "SAX function hasInternalSubset")) {
        ctxt->sax->hasInternalSubset =
            *((hasInternalSubsetSAXFunc *) value);
    } else if (!strcmp(name, "SAX function hasExternalSubset")) {
        ctxt->sax->hasExternalSubset =
            *((hasExternalSubsetSAXFunc *) value);
    } else if (!strcmp(name, "SAX function resolveEntity")) {
        ctxt->sax->resolveEntity = *((resolveEntitySAXFunc *) value);
    } else if (!strcmp(name, "SAX function getEntity")) {
        ctxt->sax->getEntity = *((getEntitySAXFunc *) value);
    } else if (!strcmp(name, "SAX function entityDecl")) {
        ctxt->sax->entityDecl = *((entityDeclSAXFunc *) value);
    } else if (!strcmp(name, "SAX function notationDecl")) {
        ctxt->sax->notationDecl = *((notationDeclSAXFunc *) value);
    } else if (!strcmp(name, "SAX function attributeDecl")) {
        ctxt->sax->attributeDecl = *((attributeDeclSAXFunc *) value);
    } else if (!strcmp(name, "SAX function elementDecl")) {
        ctxt->sax->elementDecl = *((elementDeclSAXFunc *) value);
    } else if (!strcmp(name, "SAX function unparsedEntityDecl")) {
        ctxt->sax->unparsedEntityDecl =
            *((unparsedEntityDeclSAXFunc *) value);
    } else if (!strcmp(name, "SAX function setDocumentLocator")) {
        ctxt->sax->setDocumentLocator =
            *((setDocumentLocatorSAXFunc *) value);
    } else if (!strcmp(name, "SAX function startDocument")) {
        ctxt->sax->startDocument = *((startDocumentSAXFunc *) value);
    } else if (!strcmp(name, "SAX function endDocument")) {
        ctxt->sax->endDocument = *((endDocumentSAXFunc *) value);
    } else if (!strcmp(name, "SAX function startElement")) {
        ctxt->sax->startElement = *((startElementSAXFunc *) value);
    } else if (!strcmp(name, "SAX function endElement")) {
        ctxt->sax->endElement = *((endElementSAXFunc *) value);
    } else if (!strcmp(name, "SAX function reference")) {
        ctxt->sax->reference = *((referenceSAXFunc *) value);
    } else if (!strcmp(name, "SAX function characters")) {
        ctxt->sax->characters = *((charactersSAXFunc *) value);
    } else if (!strcmp(name, "SAX function ignorableWhitespace")) {
        ctxt->sax->ignorableWhitespace =
            *((ignorableWhitespaceSAXFunc *) value);
    } else if (!strcmp(name, "SAX function processingInstruction")) {
        ctxt->sax->processingInstruction =
            *((processingInstructionSAXFunc *) value);
    } else if (!strcmp(name, "SAX function comment")) {
        ctxt->sax->comment = *((commentSAXFunc *) value);
    } else if (!strcmp(name, "SAX function warning")) {
        ctxt->sax->warning = *((warningSAXFunc *) value);
    } else if (!strcmp(name, "SAX function error")) {
        ctxt->sax->error = *((errorSAXFunc *) value);
    } else if (!strcmp(name, "SAX function fatalError")) {
        ctxt->sax->fatalError = *((fatalErrorSAXFunc *) value);
    } else if (!strcmp(name, "SAX function getParameterEntity")) {
        ctxt->sax->getParameterEntity =
            *((getParameterEntitySAXFunc *) value);
    } else if (!strcmp(name, "SAX function cdataBlock")) {
        ctxt->sax->cdataBlock = *((cdataBlockSAXFunc *) value);
    } else if (!strcmp(name, "SAX function externalSubset")) {
        ctxt->sax->externalSubset = *((externalSubsetSAXFunc *) value);
    } else {
        return (-1);
    }
    return (0);
}

xmlChar *
xmlDecodeEntities(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED,
                  int len ATTRIBUTE_UNUSED, int what ATTRIBUTE_UNUSED,
                  xmlChar end ATTRIBUTE_UNUSED,
                  xmlChar end2 ATTRIBUTE_UNUSED,
                  xmlChar end3 ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlDecodeEntities() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}


xmlChar *
xmlNamespaceParseNCName(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlNamespaceParseNCName() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}


xmlChar *
xmlNamespaceParseQName(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED,
                       xmlChar ** prefix ATTRIBUTE_UNUSED)
{

    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlNamespaceParseQName() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}


xmlChar *
xmlNamespaceParseNSDef(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlNamespaceParseNSDef() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}

xmlChar *
xmlParseQuotedString(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlParseQuotedString() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}


void
xmlParseNamespace(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlParseNamespace() deprecated function reached\n");
        deprecated = 1;
    }
}


xmlChar *
xmlScanName(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlScanName() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}

void
xmlParserHandleReference(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlParserHandleReference() deprecated function reached\n");
        deprecated = 1;
    }

    return;
}


void
xmlHandleEntity(xmlParserCtxtPtr ctxt ATTRIBUTE_UNUSED,
                xmlEntityPtr entity ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlHandleEntity() deprecated function reached\n");
        deprecated = 1;
    }
}

xmlNsPtr
xmlNewGlobalNs(xmlDocPtr doc ATTRIBUTE_UNUSED,
               const xmlChar * href ATTRIBUTE_UNUSED,
               const xmlChar * prefix ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlNewGlobalNs() deprecated function reached\n");
        deprecated = 1;
    }
    return (NULL);
}

void
xmlUpgradeOldNs(xmlDocPtr doc ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "xmlUpgradeOldNs() deprecated function reached\n");
        deprecated = 1;
    }
}

const xmlChar *
xmlEncodeEntities(xmlDocPtr doc ATTRIBUTE_UNUSED,
                  const xmlChar * input ATTRIBUTE_UNUSED)
{
    static int warning = 1;

    if (warning) {
        xmlGenericError(xmlGenericErrorContext,
                        "Deprecated API xmlEncodeEntities() used\n");
        xmlGenericError(xmlGenericErrorContext,
                        "   change code to use xmlEncodeEntitiesReentrant()\n");
        warning = 0;
    }
    return (NULL);
}

static int deprecated_v1_msg = 0;

#define DEPRECATED(n)						\
    if (deprecated_v1_msg == 0)					\
	xmlGenericError(xmlGenericErrorContext,			\
	  "Use of deprecated SAXv1 function %s\n", n);		\
    deprecated_v1_msg++;

const xmlChar *
getPublicId(void *ctx)
{
    DEPRECATED("getPublicId")
        return (xmlSAX2GetPublicId(ctx));
}

const xmlChar *
getSystemId(void *ctx)
{
    DEPRECATED("getSystemId")
        return (xmlSAX2GetSystemId(ctx));
}

int
getLineNumber(void *ctx)
{
    DEPRECATED("getLineNumber")
        return (xmlSAX2GetLineNumber(ctx));
}

int
getColumnNumber(void *ctx)
{
    DEPRECATED("getColumnNumber")
        return (xmlSAX2GetColumnNumber(ctx));
}

int
isStandalone(void *ctx)
{
    DEPRECATED("isStandalone")
        return (xmlSAX2IsStandalone(ctx));
}

int
hasInternalSubset(void *ctx)
{
    DEPRECATED("hasInternalSubset")
        return (xmlSAX2HasInternalSubset(ctx));
}

int
hasExternalSubset(void *ctx)
{
    DEPRECATED("hasExternalSubset")
        return (xmlSAX2HasExternalSubset(ctx));
}

void
internalSubset(void *ctx, const xmlChar * name,
               const xmlChar * ExternalID, const xmlChar * SystemID)
{
    DEPRECATED("internalSubset")
        xmlSAX2InternalSubset(ctx, name, ExternalID, SystemID);
}

void
externalSubset(void *ctx, const xmlChar * name,
               const xmlChar * ExternalID, const xmlChar * SystemID)
{
    DEPRECATED("externalSubset")
        xmlSAX2ExternalSubset(ctx, name, ExternalID, SystemID);
}

xmlParserInputPtr
resolveEntity(void *ctx, const xmlChar * publicId,
              const xmlChar * systemId)
{
    DEPRECATED("resolveEntity")
        return (xmlSAX2ResolveEntity(ctx, publicId, systemId));
}

xmlEntityPtr
getEntity(void *ctx, const xmlChar * name)
{
    DEPRECATED("getEntity")
        return (xmlSAX2GetEntity(ctx, name));
}

xmlEntityPtr
getParameterEntity(void *ctx, const xmlChar * name)
{
    DEPRECATED("getParameterEntity")
        return (xmlSAX2GetParameterEntity(ctx, name));
}


void
entityDecl(void *ctx, const xmlChar * name, int type,
           const xmlChar * publicId, const xmlChar * systemId,
           xmlChar * content)
{
    DEPRECATED("entityDecl")
        xmlSAX2EntityDecl(ctx, name, type, publicId, systemId, content);
}

void
attributeDecl(void *ctx, const xmlChar * elem, const xmlChar * fullname,
              int type, int def, const xmlChar * defaultValue,
              xmlEnumerationPtr tree)
{
    DEPRECATED("attributeDecl")
        xmlSAX2AttributeDecl(ctx, elem, fullname, type, def, defaultValue,
                             tree);
}

void
elementDecl(void *ctx, const xmlChar * name, int type,
            xmlElementContentPtr content)
{
    DEPRECATED("elementDecl")
        xmlSAX2ElementDecl(ctx, name, type, content);
}

void
notationDecl(void *ctx, const xmlChar * name,
             const xmlChar * publicId, const xmlChar * systemId)
{
    DEPRECATED("notationDecl")
        xmlSAX2NotationDecl(ctx, name, publicId, systemId);
}

void
unparsedEntityDecl(void *ctx, const xmlChar * name,
                   const xmlChar * publicId, const xmlChar * systemId,
                   const xmlChar * notationName)
{
    DEPRECATED("unparsedEntityDecl")
        xmlSAX2UnparsedEntityDecl(ctx, name, publicId, systemId,
                                  notationName);
}

void
setDocumentLocator(void *ctx ATTRIBUTE_UNUSED,
                   xmlSAXLocatorPtr loc ATTRIBUTE_UNUSED)
{
    DEPRECATED("setDocumentLocator")
}

void
startDocument(void *ctx)
{
   
   
        xmlSAX2StartDocument(ctx);
}

void
endDocument(void *ctx)
{
    DEPRECATED("endDocument")
        xmlSAX2EndDocument(ctx);
}

void
attribute(void *ctx ATTRIBUTE_UNUSED,
          const xmlChar * fullname ATTRIBUTE_UNUSED,
          const xmlChar * value ATTRIBUTE_UNUSED)
{
    DEPRECATED("attribute")
}

void
startElement(void *ctx, const xmlChar * fullname, const xmlChar ** atts)
{
    xmlSAX2StartElement(ctx, fullname, atts);
}

void
endElement(void *ctx, const xmlChar * name ATTRIBUTE_UNUSED)
{
    DEPRECATED("endElement")
        xmlSAX2EndElement(ctx, name);
}

void
reference(void *ctx, const xmlChar * name)
{
    DEPRECATED("reference")
        xmlSAX2Reference(ctx, name);
}

void
characters(void *ctx, const xmlChar * ch, int len)
{
    DEPRECATED("characters")
        xmlSAX2Characters(ctx, ch, len);
}

void
ignorableWhitespace(void *ctx ATTRIBUTE_UNUSED,
                    const xmlChar * ch ATTRIBUTE_UNUSED,
                    int len ATTRIBUTE_UNUSED)
{
    DEPRECATED("ignorableWhitespace")
}

void
processingInstruction(void *ctx, const xmlChar * target,
                      const xmlChar * data)
{
    DEPRECATED("processingInstruction")
        xmlSAX2ProcessingInstruction(ctx, target, data);
}

void
globalNamespace(void *ctx ATTRIBUTE_UNUSED,
                const xmlChar * href ATTRIBUTE_UNUSED,
                const xmlChar * prefix ATTRIBUTE_UNUSED)
{
    DEPRECATED("globalNamespace")
}


void
setNamespace(void *ctx ATTRIBUTE_UNUSED,
             const xmlChar * name ATTRIBUTE_UNUSED)
{
    DEPRECATED("setNamespace")
}


xmlNsPtr
getNamespace(void *ctx ATTRIBUTE_UNUSED)
{
    DEPRECATED("getNamespace")
        return (NULL);
}


int
checkNamespace(void *ctx ATTRIBUTE_UNUSED,
               xmlChar * namespace ATTRIBUTE_UNUSED)
{
    DEPRECATED("checkNamespace")
        return (0);
}

void
namespaceDecl(void *ctx ATTRIBUTE_UNUSED,
              const xmlChar * href ATTRIBUTE_UNUSED,
              const xmlChar * prefix ATTRIBUTE_UNUSED)
{
    DEPRECATED("namespaceDecl")
}

void
comment(void *ctx, const xmlChar * value)
{
    DEPRECATED("comment")
        xmlSAX2Comment(ctx, value);
}

void
cdataBlock(void *ctx, const xmlChar * value, int len)
{
    DEPRECATED("cdataBlock")
        xmlSAX2CDataBlock(ctx, value, len);
}
#define bottom_legacy
#include "elfgcchack.h"
#endif 

