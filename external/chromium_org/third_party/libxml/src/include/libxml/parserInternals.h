/*
 * Summary: internals routines exported by the parser.
 * Description: this module exports a number of internal parsing routines
 *              they are not really all intended for applications but
 *              can prove useful doing low level processing.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_PARSER_INTERNALS_H__
#define __XML_PARSER_INTERNALS_H__

#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <libxml/chvalid.h>

#ifdef __cplusplus
extern "C" {
#endif

XMLPUBVAR unsigned int xmlParserMaxDepth;

#define XML_MAX_TEXT_LENGTH 10000000

#define XML_MAX_NAMELEN 100

#define INPUT_CHUNK	250

#define IS_BYTE_CHAR(c)	 xmlIsChar_ch(c)

#define IS_CHAR(c)   xmlIsCharQ(c)

#define IS_CHAR_CH(c)  xmlIsChar_ch(c)

#define IS_BLANK(c)  xmlIsBlankQ(c)

#define IS_BLANK_CH(c)  xmlIsBlank_ch(c)

#define IS_BASECHAR(c) xmlIsBaseCharQ(c)

#define IS_DIGIT(c) xmlIsDigitQ(c)

#define IS_DIGIT_CH(c)  xmlIsDigit_ch(c)

#define IS_COMBINING(c) xmlIsCombiningQ(c)

#define IS_COMBINING_CH(c) 0 

#define IS_EXTENDER(c) xmlIsExtenderQ(c)

#define IS_EXTENDER_CH(c)  xmlIsExtender_ch(c)

#define IS_IDEOGRAPHIC(c) xmlIsIdeographicQ(c)

#define IS_LETTER(c) (IS_BASECHAR(c) || IS_IDEOGRAPHIC(c))

#define IS_LETTER_CH(c) xmlIsBaseChar_ch(c)

#define IS_ASCII_LETTER(c)	(((0x41 <= (c)) && ((c) <= 0x5a)) || \
				 ((0x61 <= (c)) && ((c) <= 0x7a)))

#define IS_ASCII_DIGIT(c)	((0x30 <= (c)) && ((c) <= 0x39))

#define IS_PUBIDCHAR(c)	xmlIsPubidCharQ(c)

#define IS_PUBIDCHAR_CH(c) xmlIsPubidChar_ch(c)

#define SKIP_EOL(p) 							\
    if (*(p) == 0x13) { p++ ; if (*(p) == 0x10) p++; }			\
    if (*(p) == 0x10) { p++ ; if (*(p) == 0x13) p++; }

#define MOVETO_ENDTAG(p)						\
    while ((*p) && (*(p) != '>')) (p)++

#define MOVETO_STARTTAG(p)						\
    while ((*p) && (*(p) != '<')) (p)++

XMLPUBVAR const xmlChar xmlStringText[];
XMLPUBVAR const xmlChar xmlStringTextNoenc[];
XMLPUBVAR const xmlChar xmlStringComment[];

XMLPUBFUN int XMLCALL                   xmlIsLetter     (int c);

XMLPUBFUN xmlParserCtxtPtr XMLCALL	
			xmlCreateFileParserCtxt	(const char *filename);
XMLPUBFUN xmlParserCtxtPtr XMLCALL	
			xmlCreateURLParserCtxt	(const char *filename,
						 int options);
XMLPUBFUN xmlParserCtxtPtr XMLCALL	
			xmlCreateMemoryParserCtxt(const char *buffer,
						 int size);
XMLPUBFUN xmlParserCtxtPtr XMLCALL	
			xmlCreateEntityParserCtxt(const xmlChar *URL,
						 const xmlChar *ID,
						 const xmlChar *base);
XMLPUBFUN int XMLCALL			
			xmlSwitchEncoding	(xmlParserCtxtPtr ctxt,
						 xmlCharEncoding enc);
XMLPUBFUN int XMLCALL			
			xmlSwitchToEncoding	(xmlParserCtxtPtr ctxt,
					 xmlCharEncodingHandlerPtr handler);
XMLPUBFUN int XMLCALL			
			xmlSwitchInputEncoding	(xmlParserCtxtPtr ctxt,
						 xmlParserInputPtr input,
					 xmlCharEncodingHandlerPtr handler);

#ifdef IN_LIBXML
XMLPUBFUN void XMLCALL
			__xmlErrEncoding	(xmlParserCtxtPtr ctxt,
						 xmlParserErrors xmlerr,
						 const char *msg,
						 const xmlChar * str1,
						 const xmlChar * str2);
#endif

XMLPUBFUN xmlParserInputPtr XMLCALL	
			xmlNewStringInputStream	(xmlParserCtxtPtr ctxt,
						 const xmlChar *buffer);
XMLPUBFUN xmlParserInputPtr XMLCALL	
			xmlNewEntityInputStream	(xmlParserCtxtPtr ctxt,
						 xmlEntityPtr entity);
XMLPUBFUN int XMLCALL			
			xmlPushInput		(xmlParserCtxtPtr ctxt,
						 xmlParserInputPtr input);
XMLPUBFUN xmlChar XMLCALL			
			xmlPopInput		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlFreeInputStream	(xmlParserInputPtr input);
XMLPUBFUN xmlParserInputPtr XMLCALL	
			xmlNewInputFromFile	(xmlParserCtxtPtr ctxt,
						 const char *filename);
XMLPUBFUN xmlParserInputPtr XMLCALL	
			xmlNewInputStream	(xmlParserCtxtPtr ctxt);

XMLPUBFUN xmlChar * XMLCALL		
			xmlSplitQName		(xmlParserCtxtPtr ctxt,
						 const xmlChar *name,
						 xmlChar **prefix);

XMLPUBFUN const xmlChar * XMLCALL		
			xmlParseName		(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseNmtoken		(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseEntityValue	(xmlParserCtxtPtr ctxt,
						 xmlChar **orig);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseAttValue	(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseSystemLiteral	(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParsePubidLiteral	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseCharData	(xmlParserCtxtPtr ctxt,
						 int cdata);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseExternalID	(xmlParserCtxtPtr ctxt,
						 xmlChar **publicID,
						 int strict);
XMLPUBFUN void XMLCALL			
			xmlParseComment		(xmlParserCtxtPtr ctxt);
XMLPUBFUN const xmlChar * XMLCALL		
			xmlParsePITarget	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParsePI		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseNotationDecl	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseEntityDecl	(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			
			xmlParseDefaultDecl	(xmlParserCtxtPtr ctxt,
						 xmlChar **value);
XMLPUBFUN xmlEnumerationPtr XMLCALL	
			xmlParseNotationType	(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlEnumerationPtr XMLCALL	
			xmlParseEnumerationType	(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			
			xmlParseEnumeratedType	(xmlParserCtxtPtr ctxt,
						 xmlEnumerationPtr *tree);
XMLPUBFUN int XMLCALL			
			xmlParseAttributeType	(xmlParserCtxtPtr ctxt,
						 xmlEnumerationPtr *tree);
XMLPUBFUN void XMLCALL			
			xmlParseAttributeListDecl(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlElementContentPtr XMLCALL	
			xmlParseElementMixedContentDecl
						(xmlParserCtxtPtr ctxt,
						 int inputchk);
XMLPUBFUN xmlElementContentPtr XMLCALL	
			xmlParseElementChildrenContentDecl
						(xmlParserCtxtPtr ctxt,
						 int inputchk);
XMLPUBFUN int XMLCALL			
			xmlParseElementContentDecl(xmlParserCtxtPtr ctxt,
						 const xmlChar *name,
						 xmlElementContentPtr *result);
XMLPUBFUN int XMLCALL			
			xmlParseElementDecl	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseMarkupDecl	(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			
			xmlParseCharRef		(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlEntityPtr XMLCALL		
			xmlParseEntityRef	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseReference	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParsePEReference	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseDocTypeDecl	(xmlParserCtxtPtr ctxt);
#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN const xmlChar * XMLCALL		
			xmlParseAttribute	(xmlParserCtxtPtr ctxt,
						 xmlChar **value);
XMLPUBFUN const xmlChar * XMLCALL		
			xmlParseStartTag	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseEndTag		(xmlParserCtxtPtr ctxt);
#endif 
XMLPUBFUN void XMLCALL			
			xmlParseCDSect		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseContent		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseElement		(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseVersionNum	(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseVersionInfo	(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlParseEncName		(xmlParserCtxtPtr ctxt);
XMLPUBFUN const xmlChar * XMLCALL		
			xmlParseEncodingDecl	(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			
			xmlParseSDDecl		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseXMLDecl		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseTextDecl	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseMisc		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			
			xmlParseExternalSubset	(xmlParserCtxtPtr ctxt,
						 const xmlChar *ExternalID,
						 const xmlChar *SystemID); 
#define XML_SUBSTITUTE_NONE	0
#define XML_SUBSTITUTE_REF	1
#define XML_SUBSTITUTE_PEREF	2
#define XML_SUBSTITUTE_BOTH 	3

XMLPUBFUN xmlChar * XMLCALL
		xmlStringDecodeEntities		(xmlParserCtxtPtr ctxt,
						 const xmlChar *str,
						 int what,
						 xmlChar end,
						 xmlChar  end2,
						 xmlChar end3);
XMLPUBFUN xmlChar * XMLCALL
		xmlStringLenDecodeEntities	(xmlParserCtxtPtr ctxt,
						 const xmlChar *str,
						 int len,
						 int what,
						 xmlChar end,
						 xmlChar  end2,
						 xmlChar end3);

XMLPUBFUN int XMLCALL			nodePush		(xmlParserCtxtPtr ctxt,
						 xmlNodePtr value);
XMLPUBFUN xmlNodePtr XMLCALL		nodePop			(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			inputPush		(xmlParserCtxtPtr ctxt,
						 xmlParserInputPtr value);
XMLPUBFUN xmlParserInputPtr XMLCALL	inputPop		(xmlParserCtxtPtr ctxt);
XMLPUBFUN const xmlChar * XMLCALL	namePop			(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			namePush		(xmlParserCtxtPtr ctxt,
						 const xmlChar *value);

XMLPUBFUN int XMLCALL			xmlSkipBlankChars	(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			xmlStringCurrentChar	(xmlParserCtxtPtr ctxt,
						 const xmlChar *cur,
						 int *len);
XMLPUBFUN void XMLCALL			xmlParserHandlePEReference(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL			xmlCheckLanguageID	(const xmlChar *lang);

XMLPUBFUN int XMLCALL			xmlCurrentChar		(xmlParserCtxtPtr ctxt,
						 int *len);
XMLPUBFUN int XMLCALL		xmlCopyCharMultiByte	(xmlChar *out,
						 int val);
XMLPUBFUN int XMLCALL			xmlCopyChar		(int len,
						 xmlChar *out,
						 int val);
XMLPUBFUN void XMLCALL			xmlNextChar		(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL			xmlParserInputShrink	(xmlParserInputPtr in);

#ifdef LIBXML_HTML_ENABLED
XMLPUBFUN void XMLCALL			htmlInitAutoClose	(void);
XMLPUBFUN htmlParserCtxtPtr XMLCALL	htmlCreateFileParserCtxt(const char *filename,
	                                         const char *encoding);
#endif

#ifdef LIBXML_LEGACY_ENABLED
typedef	void	(*xmlEntityReferenceFunc)	(xmlEntityPtr ent,
						 xmlNodePtr firstNode,
						 xmlNodePtr lastNode);
  
XMLPUBFUN void XMLCALL		xmlSetEntityReferenceFunc	(xmlEntityReferenceFunc func);

XMLPUBFUN xmlChar * XMLCALL		
			xmlParseQuotedString	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL
                        xmlParseNamespace       (xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlNamespaceParseNSDef	(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlScanName		(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlNamespaceParseNCName	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL	xmlParserHandleReference(xmlParserCtxtPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL		
			xmlNamespaceParseQName	(xmlParserCtxtPtr ctxt,
						 xmlChar **prefix);
XMLPUBFUN xmlChar * XMLCALL
		xmlDecodeEntities		(xmlParserCtxtPtr ctxt,
						 int len,
						 int what,
						 xmlChar end,
						 xmlChar  end2,
						 xmlChar end3);
XMLPUBFUN void XMLCALL			
			xmlHandleEntity		(xmlParserCtxtPtr ctxt,
						 xmlEntityPtr entity);

#endif 

#ifdef IN_LIBXML
XMLPUBFUN void XMLCALL
	xmlErrMemory		(xmlParserCtxtPtr ctxt,
				 const char *extra);
#endif

#ifdef __cplusplus
}
#endif
#endif 
