/*
 * Summary: interface for the encoding conversion functions
 * Description: interface for the encoding conversion functions needed for
 *              XML basic encoding and iconv() support.
 *
 * Related specs are
 * rfc2044        (UTF-8 and UTF-16) F. Yergeau Alis Technologies
 * [ISO-10646]    UTF-8 and UTF-16 in Annexes
 * [ISO-8859-1]   ISO Latin-1 characters codes.
 * [UNICODE]      The Unicode Consortium, "The Unicode Standard --
 *                Worldwide Character Encoding -- Version 1.0", Addison-
 *                Wesley, Volume 1, 1991, Volume 2, 1992.  UTF-8 is
 *                described in Unicode Technical Report #4.
 * [US-ASCII]     Coded Character Set--7-bit American Standard Code for
 *                Information Interchange, ANSI X3.4-1986.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_CHAR_ENCODING_H__
#define __XML_CHAR_ENCODING_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_ICONV_ENABLED
#include <iconv.h>
#else 
#ifdef LIBXML_ICU_ENABLED
#include <unicode/ucnv.h>
#if 0
#include <stdint.h>
struct UConverter;
typedef struct UConverter UConverter;
#ifdef _MSC_VER
typedef wchar_t UChar;
#else
typedef uint16_t UChar;
#endif
#endif
#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    XML_CHAR_ENCODING_ERROR=   -1, 
    XML_CHAR_ENCODING_NONE=	0, 
    XML_CHAR_ENCODING_UTF8=	1, 
    XML_CHAR_ENCODING_UTF16LE=	2, 
    XML_CHAR_ENCODING_UTF16BE=	3, 
    XML_CHAR_ENCODING_UCS4LE=	4, 
    XML_CHAR_ENCODING_UCS4BE=	5, 
    XML_CHAR_ENCODING_EBCDIC=	6, 
    XML_CHAR_ENCODING_UCS4_2143=7, 
    XML_CHAR_ENCODING_UCS4_3412=8, 
    XML_CHAR_ENCODING_UCS2=	9, 
    XML_CHAR_ENCODING_8859_1=	10,
    XML_CHAR_ENCODING_8859_2=	11,
    XML_CHAR_ENCODING_8859_3=	12,
    XML_CHAR_ENCODING_8859_4=	13,
    XML_CHAR_ENCODING_8859_5=	14,
    XML_CHAR_ENCODING_8859_6=	15,
    XML_CHAR_ENCODING_8859_7=	16,
    XML_CHAR_ENCODING_8859_8=	17,
    XML_CHAR_ENCODING_8859_9=	18,
    XML_CHAR_ENCODING_2022_JP=  19,
    XML_CHAR_ENCODING_SHIFT_JIS=20,
    XML_CHAR_ENCODING_EUC_JP=   21,
    XML_CHAR_ENCODING_ASCII=    22 
} xmlCharEncoding;

/**
 * xmlCharEncodingInputFunc:
 * @out:  a pointer to an array of bytes to store the UTF-8 result
 * @outlen:  the length of @out
 * @in:  a pointer to an array of chars in the original encoding
 * @inlen:  the length of @in
 *
 * Take a block of chars in the original encoding and try to convert
 * it to an UTF-8 block of chars out.
 *
 * Returns the number of bytes written, -1 if lack of space, or -2
 *     if the transcoding failed.
 * The value of @inlen after return is the number of octets consumed
 *     if the return value is positive, else unpredictiable.
 * The value of @outlen after return is the number of octets consumed.
 */
typedef int (* xmlCharEncodingInputFunc)(unsigned char *out, int *outlen,
                                         const unsigned char *in, int *inlen);


/**
 * xmlCharEncodingOutputFunc:
 * @out:  a pointer to an array of bytes to store the result
 * @outlen:  the length of @out
 * @in:  a pointer to an array of UTF-8 chars
 * @inlen:  the length of @in
 *
 * Take a block of UTF-8 chars in and try to convert it to another
 * encoding.
 * Note: a first call designed to produce heading info is called with
 * in = NULL. If stateful this should also initialize the encoder state.
 *
 * Returns the number of bytes written, -1 if lack of space, or -2
 *     if the transcoding failed.
 * The value of @inlen after return is the number of octets consumed
 *     if the return value is positive, else unpredictiable.
 * The value of @outlen after return is the number of octets produced.
 */
typedef int (* xmlCharEncodingOutputFunc)(unsigned char *out, int *outlen,
                                          const unsigned char *in, int *inlen);


#ifdef LIBXML_ICU_ENABLED
struct _uconv_t {
  UConverter *uconv; 
  UConverter *utf8; 
};
typedef struct _uconv_t uconv_t;
#endif

typedef struct _xmlCharEncodingHandler xmlCharEncodingHandler;
typedef xmlCharEncodingHandler *xmlCharEncodingHandlerPtr;
struct _xmlCharEncodingHandler {
    char                       *name;
    xmlCharEncodingInputFunc   input;
    xmlCharEncodingOutputFunc  output;
#ifdef LIBXML_ICONV_ENABLED
    iconv_t                    iconv_in;
    iconv_t                    iconv_out;
#endif 
#ifdef LIBXML_ICU_ENABLED
    uconv_t                    *uconv_in;
    uconv_t                    *uconv_out;
#endif 
};

#ifdef __cplusplus
}
#endif
#include <libxml/tree.h>
#ifdef __cplusplus
extern "C" {
#endif

XMLPUBFUN void XMLCALL	
	xmlInitCharEncodingHandlers	(void);
XMLPUBFUN void XMLCALL	
	xmlCleanupCharEncodingHandlers	(void);
XMLPUBFUN void XMLCALL	
	xmlRegisterCharEncodingHandler	(xmlCharEncodingHandlerPtr handler);
XMLPUBFUN xmlCharEncodingHandlerPtr XMLCALL
	xmlGetCharEncodingHandler	(xmlCharEncoding enc);
XMLPUBFUN xmlCharEncodingHandlerPtr XMLCALL
	xmlFindCharEncodingHandler	(const char *name);
XMLPUBFUN xmlCharEncodingHandlerPtr XMLCALL
	xmlNewCharEncodingHandler	(const char *name, 
                          		 xmlCharEncodingInputFunc input,
                          		 xmlCharEncodingOutputFunc output);

XMLPUBFUN int XMLCALL	
	xmlAddEncodingAlias		(const char *name,
					 const char *alias);
XMLPUBFUN int XMLCALL	
	xmlDelEncodingAlias		(const char *alias);
XMLPUBFUN const char * XMLCALL
	xmlGetEncodingAlias		(const char *alias);
XMLPUBFUN void XMLCALL	
	xmlCleanupEncodingAliases	(void);
XMLPUBFUN xmlCharEncoding XMLCALL
	xmlParseCharEncoding		(const char *name);
XMLPUBFUN const char * XMLCALL
	xmlGetCharEncodingName		(xmlCharEncoding enc);

XMLPUBFUN xmlCharEncoding XMLCALL
	xmlDetectCharEncoding		(const unsigned char *in,
					 int len);

XMLPUBFUN int XMLCALL	
	xmlCharEncOutFunc		(xmlCharEncodingHandler *handler,
					 xmlBufferPtr out,
					 xmlBufferPtr in);

XMLPUBFUN int XMLCALL	
	xmlCharEncInFunc		(xmlCharEncodingHandler *handler,
					 xmlBufferPtr out,
					 xmlBufferPtr in);
XMLPUBFUN int XMLCALL
	xmlCharEncFirstLine		(xmlCharEncodingHandler *handler,
					 xmlBufferPtr out,
					 xmlBufferPtr in);
XMLPUBFUN int XMLCALL	
	xmlCharEncCloseFunc		(xmlCharEncodingHandler *handler);

#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN int XMLCALL	
	UTF8Toisolat1			(unsigned char *out,
					 int *outlen,
					 const unsigned char *in,
					 int *inlen);
#endif 
XMLPUBFUN int XMLCALL	
	isolat1ToUTF8			(unsigned char *out,
					 int *outlen,
					 const unsigned char *in,
					 int *inlen);
#ifdef __cplusplus
}
#endif

#endif 
