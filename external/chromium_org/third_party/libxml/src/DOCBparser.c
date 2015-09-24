/*
 * DOCBparser.c : an attempt to parse SGML Docbook documents
 *
 * This is deprecated !!!
 * Code removed with release 2.6.0 it was broken.
 * The doc are expect to be migrated to XML DocBook
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"
#ifdef LIBXML_DOCB_ENABLED

#include <libxml/xmlerror.h>
#include <libxml/DOCBparser.h>

int
docbEncodeEntities(unsigned char *out ATTRIBUTE_UNUSED,
                   int *outlen ATTRIBUTE_UNUSED,
                   const unsigned char *in ATTRIBUTE_UNUSED,
                   int *inlen ATTRIBUTE_UNUSED,
                   int quoteChar ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbEncodeEntities() deprecated function reached\n");
        deprecated = 1;
    }
    return(-1);
}


int
docbParseDocument(docbParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbParseDocument() deprecated function reached\n");
        deprecated = 1;
    }
    return (xmlParseDocument(ctxt));
}


void
docbFreeParserCtxt(docbParserCtxtPtr ctxt ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbFreeParserCtxt() deprecated function reached\n");
        deprecated = 1;
    }
    xmlFreeParserCtxt(ctxt);
}

int
docbParseChunk(docbParserCtxtPtr ctxt ATTRIBUTE_UNUSED,
               const char *chunk ATTRIBUTE_UNUSED,
	       int size ATTRIBUTE_UNUSED,
               int terminate ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbParseChunk() deprecated function reached\n");
        deprecated = 1;
    }

    return (xmlParseChunk(ctxt, chunk, size, terminate));
}

docbParserCtxtPtr
docbCreatePushParserCtxt(docbSAXHandlerPtr sax ATTRIBUTE_UNUSED,
                         void *user_data ATTRIBUTE_UNUSED,
                         const char *chunk ATTRIBUTE_UNUSED,
			 int size ATTRIBUTE_UNUSED,
			 const char *filename ATTRIBUTE_UNUSED,
                         xmlCharEncoding enc ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbParseChunk() deprecated function reached\n");
        deprecated = 1;
    }

    return(xmlCreatePushParserCtxt(sax, user_data, chunk, size, filename));
}


docbDocPtr
docbSAXParseDoc(xmlChar * cur ATTRIBUTE_UNUSED,
                const char *encoding ATTRIBUTE_UNUSED,
		docbSAXHandlerPtr sax ATTRIBUTE_UNUSED,
                void *userData ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbParseChunk() deprecated function reached\n");
        deprecated = 1;
    }

    return (xmlSAXParseMemoryWithData(sax, (const char *)cur,
			  xmlStrlen((const xmlChar *) cur), 0,  userData));
}


docbDocPtr
docbParseDoc(xmlChar * cur ATTRIBUTE_UNUSED,
             const char *encoding ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbParseChunk() deprecated function reached\n");
        deprecated = 1;
    }

    return (xmlParseDoc(cur));
}


docbParserCtxtPtr
docbCreateFileParserCtxt(const char *filename ATTRIBUTE_UNUSED,
                         const char *encoding ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbCreateFileParserCtxt() deprecated function reached\n");
        deprecated = 1;
    }

    return (xmlCreateFileParserCtxt(filename));
}


docbDocPtr
docbSAXParseFile(const char *filename ATTRIBUTE_UNUSED,
                 const char *encoding ATTRIBUTE_UNUSED,
                 docbSAXHandlerPtr sax ATTRIBUTE_UNUSED,
		 void *userData ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbSAXParseFile() deprecated function reached\n");
        deprecated = 1;
    }

    return (xmlSAXParseFileWithData(sax, filename, 0, userData));
}


docbDocPtr
docbParseFile(const char *filename ATTRIBUTE_UNUSED,
              const char *encoding ATTRIBUTE_UNUSED)
{
    static int deprecated = 0;

    if (!deprecated) {
        xmlGenericError(xmlGenericErrorContext,
                        "docbParseFile() deprecated function reached\n");
        deprecated = 1;
    }

    return (xmlParseFile(filename));
}
#define bottom_DOCBparser
#include "elfgcchack.h"
#endif 
