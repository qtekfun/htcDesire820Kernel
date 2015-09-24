/*
 * Summary: interface for an HTML 4.0 non-verifying parser
 * Description: this module implements an HTML 4.0 non-verifying parser
 *              with API compatible with the XML parser ones. It should
 *              be able to parse "real world" HTML, even if severely
 *              broken from a specification point of view.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __HTML_PARSER_H__
#define __HTML_PARSER_H__
#include <libxml/xmlversion.h>
#include <libxml/parser.h>

#ifdef LIBXML_HTML_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

typedef xmlParserCtxt htmlParserCtxt;
typedef xmlParserCtxtPtr htmlParserCtxtPtr;
typedef xmlParserNodeInfo htmlParserNodeInfo;
typedef xmlSAXHandler htmlSAXHandler;
typedef xmlSAXHandlerPtr htmlSAXHandlerPtr;
typedef xmlParserInput htmlParserInput;
typedef xmlParserInputPtr htmlParserInputPtr;
typedef xmlDocPtr htmlDocPtr;
typedef xmlNodePtr htmlNodePtr;

typedef struct _htmlElemDesc htmlElemDesc;
typedef htmlElemDesc *htmlElemDescPtr;
struct _htmlElemDesc {
    const char *name;	
    char startTag;      
    char endTag;        
    char saveEndTag;    
    char empty;         
    char depr;          
    char dtd;           
    char isinline;      
    const char *desc;   

    const char** subelts;		
    const char* defaultsubelt;	
    const char** attrs_opt;		
    const char** attrs_depr;		
    const char** attrs_req;		
};

typedef struct _htmlEntityDesc htmlEntityDesc;
typedef htmlEntityDesc *htmlEntityDescPtr;
struct _htmlEntityDesc {
    unsigned int value;	
    const char *name;	
    const char *desc;   
};

XMLPUBFUN const htmlElemDesc * XMLCALL 	
			htmlTagLookup	(const xmlChar *tag);
XMLPUBFUN const htmlEntityDesc * XMLCALL 	
			htmlEntityLookup(const xmlChar *name);
XMLPUBFUN const htmlEntityDesc * XMLCALL 	
			htmlEntityValueLookup(unsigned int value);

XMLPUBFUN int XMLCALL			
			htmlIsAutoClosed(htmlDocPtr doc,
					 htmlNodePtr elem);
XMLPUBFUN int XMLCALL			
			htmlAutoCloseTag(htmlDocPtr doc,
					 const xmlChar *name,
					 htmlNodePtr elem);
XMLPUBFUN const htmlEntityDesc * XMLCALL	
			htmlParseEntityRef(htmlParserCtxtPtr ctxt,
					 const xmlChar **str);
XMLPUBFUN int XMLCALL			
			htmlParseCharRef(htmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			htmlParseElement(htmlParserCtxtPtr ctxt);

XMLPUBFUN htmlParserCtxtPtr XMLCALL	
			htmlNewParserCtxt(void);

XMLPUBFUN htmlParserCtxtPtr XMLCALL	
			htmlCreateMemoryParserCtxt(const char *buffer,
						   int size);

XMLPUBFUN int XMLCALL			
			htmlParseDocument(htmlParserCtxtPtr ctxt);
XMLPUBFUN htmlDocPtr XMLCALL		
			htmlSAXParseDoc	(xmlChar *cur,
					 const char *encoding,
					 htmlSAXHandlerPtr sax,
					 void *userData);
XMLPUBFUN htmlDocPtr XMLCALL		
			htmlParseDoc	(xmlChar *cur,
					 const char *encoding);
XMLPUBFUN htmlDocPtr XMLCALL		
			htmlSAXParseFile(const char *filename,
					 const char *encoding,
					 htmlSAXHandlerPtr sax,
					 void *userData);
XMLPUBFUN htmlDocPtr XMLCALL		
			htmlParseFile	(const char *filename,
					 const char *encoding);
XMLPUBFUN int XMLCALL			
			UTF8ToHtml	(unsigned char *out,
					 int *outlen,
					 const unsigned char *in,
					 int *inlen);
XMLPUBFUN int XMLCALL			
			htmlEncodeEntities(unsigned char *out,
					 int *outlen,
					 const unsigned char *in,
					 int *inlen, int quoteChar);
XMLPUBFUN int XMLCALL			
			htmlIsScriptAttribute(const xmlChar *name);
XMLPUBFUN int XMLCALL			
			htmlHandleOmittedElem(int val);

#ifdef LIBXML_PUSH_ENABLED
XMLPUBFUN htmlParserCtxtPtr XMLCALL	
			htmlCreatePushParserCtxt(htmlSAXHandlerPtr sax,
						 void *user_data,
						 const char *chunk,
						 int size,
						 const char *filename,
						 xmlCharEncoding enc);
XMLPUBFUN int XMLCALL			
			htmlParseChunk		(htmlParserCtxtPtr ctxt,
						 const char *chunk,
						 int size,
						 int terminate);
#endif 

XMLPUBFUN void XMLCALL			
			htmlFreeParserCtxt	(htmlParserCtxtPtr ctxt);

typedef enum {
    HTML_PARSE_RECOVER  = 1<<0, 
    HTML_PARSE_NOERROR	= 1<<5,	
    HTML_PARSE_NOWARNING= 1<<6,	
    HTML_PARSE_PEDANTIC	= 1<<7,	
    HTML_PARSE_NOBLANKS	= 1<<8,	
    HTML_PARSE_NONET	= 1<<11,
    HTML_PARSE_NOIMPLIED= 1<<13,
    HTML_PARSE_COMPACT  = 1<<16 
} htmlParserOption;

XMLPUBFUN void XMLCALL
		htmlCtxtReset		(htmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL
		htmlCtxtUseOptions	(htmlParserCtxtPtr ctxt,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlReadDoc		(const xmlChar *cur,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlReadFile		(const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlReadMemory		(const char *buffer,
					 int size,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlReadFd		(int fd,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlReadIO		(xmlInputReadCallback ioread,
					 xmlInputCloseCallback ioclose,
					 void *ioctx,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlCtxtReadDoc		(xmlParserCtxtPtr ctxt,
					 const xmlChar *cur,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlCtxtReadFile		(xmlParserCtxtPtr ctxt,
					 const char *filename,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlCtxtReadMemory		(xmlParserCtxtPtr ctxt,
					 const char *buffer,
					 int size,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlCtxtReadFd		(xmlParserCtxtPtr ctxt,
					 int fd,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN htmlDocPtr XMLCALL
		htmlCtxtReadIO		(xmlParserCtxtPtr ctxt,
					 xmlInputReadCallback ioread,
					 xmlInputCloseCallback ioclose,
					 void *ioctx,
					 const char *URL,
					 const char *encoding,
					 int options);

typedef enum {
  HTML_NA = 0 ,		
  HTML_INVALID = 0x1 ,
  HTML_DEPRECATED = 0x2 ,
  HTML_VALID = 0x4 ,
  HTML_REQUIRED = 0xc 
} htmlStatus ;

XMLPUBFUN htmlStatus XMLCALL htmlAttrAllowed(const htmlElemDesc*, const xmlChar*, int) ;
XMLPUBFUN int XMLCALL htmlElementAllowedHere(const htmlElemDesc*, const xmlChar*) ;
XMLPUBFUN htmlStatus XMLCALL htmlElementStatusHere(const htmlElemDesc*, const htmlElemDesc*) ;
XMLPUBFUN htmlStatus XMLCALL htmlNodeStatus(const htmlNodePtr, int) ;
#define htmlDefaultSubelement(elt) elt->defaultsubelt
#define htmlElementAllowedHereDesc(parent,elt) \
	htmlElementAllowedHere((parent), (elt)->name)
#define htmlRequiredAttrs(elt) (elt)->attrs_req


#ifdef __cplusplus
}
#endif

#endif 
#endif 
