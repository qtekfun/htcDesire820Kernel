/*
 * Summary: interface for the I/O interfaces used by the parser
 * Description: interface for the I/O interfaces used by the parser
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_IO_H__
#define __XML_IO_H__

#include <stdio.h>
#include <libxml/xmlversion.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef int (XMLCALL *xmlInputMatchCallback) (char const *filename);
typedef void * (XMLCALL *xmlInputOpenCallback) (char const *filename);
typedef int (XMLCALL *xmlInputReadCallback) (void * context, char * buffer, int len);
typedef int (XMLCALL *xmlInputCloseCallback) (void * context);

#ifdef LIBXML_OUTPUT_ENABLED

typedef int (XMLCALL *xmlOutputMatchCallback) (char const *filename);
typedef void * (XMLCALL *xmlOutputOpenCallback) (char const *filename);
/**
 * xmlOutputWriteCallback:
 * @context:  an Output context
 * @buffer:  the buffer of data to write
 * @len:  the length of the buffer in bytes
 *
 * Callback used in the I/O Output API to write to the resource
 *
 * Returns the number of bytes written or -1 in case of error
 */
typedef int (XMLCALL *xmlOutputWriteCallback) (void * context, const char * buffer,
                                       int len);
typedef int (XMLCALL *xmlOutputCloseCallback) (void * context);
#endif 

#ifdef __cplusplus
}
#endif

#include <libxml/globals.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/encoding.h>

#ifdef __cplusplus
extern "C" {
#endif
struct _xmlParserInputBuffer {
    void*                  context;
    xmlInputReadCallback   readcallback;
    xmlInputCloseCallback  closecallback;
    
    xmlCharEncodingHandlerPtr encoder; 
    
    xmlBufferPtr buffer;    
    xmlBufferPtr raw;       
    int	compressed;	    
    int error;
    unsigned long rawconsumed;
};


#ifdef LIBXML_OUTPUT_ENABLED
struct _xmlOutputBuffer {
    void*                   context;
    xmlOutputWriteCallback  writecallback;
    xmlOutputCloseCallback  closecallback;
    
    xmlCharEncodingHandlerPtr encoder; 
    
    xmlBufferPtr buffer;    
    xmlBufferPtr conv;      
    int written;            /* total number of byte written */
    int error;
};
#endif 

XMLPUBFUN void XMLCALL	
	xmlCleanupInputCallbacks		(void);

XMLPUBFUN int XMLCALL
	xmlPopInputCallbacks			(void);

XMLPUBFUN void XMLCALL	
	xmlRegisterDefaultInputCallbacks	(void);
XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlAllocParserInputBuffer		(xmlCharEncoding enc);

XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlParserInputBufferCreateFilename	(const char *URI,
                                                 xmlCharEncoding enc);
XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlParserInputBufferCreateFile		(FILE *file,
                                                 xmlCharEncoding enc);
XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlParserInputBufferCreateFd		(int fd,
	                                         xmlCharEncoding enc);
XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlParserInputBufferCreateMem		(const char *mem, int size,
	                                         xmlCharEncoding enc);
XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlParserInputBufferCreateStatic	(const char *mem, int size,
	                                         xmlCharEncoding enc);
XMLPUBFUN xmlParserInputBufferPtr XMLCALL
	xmlParserInputBufferCreateIO		(xmlInputReadCallback   ioread,
						 xmlInputCloseCallback  ioclose,
						 void *ioctx,
	                                         xmlCharEncoding enc);
XMLPUBFUN int XMLCALL	
	xmlParserInputBufferRead		(xmlParserInputBufferPtr in,
						 int len);
XMLPUBFUN int XMLCALL	
	xmlParserInputBufferGrow		(xmlParserInputBufferPtr in,
						 int len);
XMLPUBFUN int XMLCALL	
	xmlParserInputBufferPush		(xmlParserInputBufferPtr in,
						 int len,
						 const char *buf);
XMLPUBFUN void XMLCALL	
	xmlFreeParserInputBuffer		(xmlParserInputBufferPtr in);
XMLPUBFUN char * XMLCALL	
	xmlParserGetDirectory			(const char *filename);

XMLPUBFUN int XMLCALL     
	xmlRegisterInputCallbacks		(xmlInputMatchCallback matchFunc,
						 xmlInputOpenCallback openFunc,
						 xmlInputReadCallback readFunc,
						 xmlInputCloseCallback closeFunc);

xmlParserInputBufferPtr
	__xmlParserInputBufferCreateFilename(const char *URI,
										xmlCharEncoding enc);

#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL	
	xmlCleanupOutputCallbacks		(void);
XMLPUBFUN void XMLCALL	
	xmlRegisterDefaultOutputCallbacks(void);
XMLPUBFUN xmlOutputBufferPtr XMLCALL
	xmlAllocOutputBuffer		(xmlCharEncodingHandlerPtr encoder);

XMLPUBFUN xmlOutputBufferPtr XMLCALL
	xmlOutputBufferCreateFilename	(const char *URI,
					 xmlCharEncodingHandlerPtr encoder,
					 int compression);

XMLPUBFUN xmlOutputBufferPtr XMLCALL
	xmlOutputBufferCreateFile	(FILE *file,
					 xmlCharEncodingHandlerPtr encoder);

XMLPUBFUN xmlOutputBufferPtr XMLCALL
	xmlOutputBufferCreateBuffer	(xmlBufferPtr buffer,
					 xmlCharEncodingHandlerPtr encoder);

XMLPUBFUN xmlOutputBufferPtr XMLCALL
	xmlOutputBufferCreateFd		(int fd,
					 xmlCharEncodingHandlerPtr encoder);

XMLPUBFUN xmlOutputBufferPtr XMLCALL
	xmlOutputBufferCreateIO		(xmlOutputWriteCallback   iowrite,
					 xmlOutputCloseCallback  ioclose,
					 void *ioctx,
					 xmlCharEncodingHandlerPtr encoder);

XMLPUBFUN int XMLCALL	
	xmlOutputBufferWrite		(xmlOutputBufferPtr out,
					 int len,
					 const char *buf);
XMLPUBFUN int XMLCALL	
	xmlOutputBufferWriteString	(xmlOutputBufferPtr out,
					 const char *str);
XMLPUBFUN int XMLCALL	
	xmlOutputBufferWriteEscape	(xmlOutputBufferPtr out,
					 const xmlChar *str,
					 xmlCharEncodingOutputFunc escaping);

XMLPUBFUN int XMLCALL	
	xmlOutputBufferFlush		(xmlOutputBufferPtr out);
XMLPUBFUN int XMLCALL	
	xmlOutputBufferClose		(xmlOutputBufferPtr out);

XMLPUBFUN int XMLCALL     
	xmlRegisterOutputCallbacks	(xmlOutputMatchCallback matchFunc,
					 xmlOutputOpenCallback openFunc,
					 xmlOutputWriteCallback writeFunc,
					 xmlOutputCloseCallback closeFunc);

xmlOutputBufferPtr
	__xmlOutputBufferCreateFilename(const char *URI,
                              xmlCharEncodingHandlerPtr encoder,
                              int compression);

#ifdef LIBXML_HTTP_ENABLED
XMLPUBFUN void XMLCALL	
	xmlRegisterHTTPPostCallbacks	(void );
#endif 
	
#endif 

XMLPUBFUN xmlParserInputPtr XMLCALL
	xmlCheckHTTPInput		(xmlParserCtxtPtr ctxt,
					 xmlParserInputPtr ret);

XMLPUBFUN xmlParserInputPtr XMLCALL 
	xmlNoNetExternalEntityLoader	(const char *URL,
					 const char *ID,
					 xmlParserCtxtPtr ctxt);

XMLPUBFUN xmlChar * XMLCALL 
	xmlNormalizeWindowsPath		(const xmlChar *path);

XMLPUBFUN int XMLCALL	
	xmlCheckFilename		(const char *path);
XMLPUBFUN int XMLCALL	
	xmlFileMatch 			(const char *filename);
XMLPUBFUN void * XMLCALL	
	xmlFileOpen 			(const char *filename);
XMLPUBFUN int XMLCALL	
	xmlFileRead 			(void * context, 
					 char * buffer, 
					 int len);
XMLPUBFUN int XMLCALL	
	xmlFileClose 			(void * context);

#ifdef LIBXML_HTTP_ENABLED
XMLPUBFUN int XMLCALL	
	xmlIOHTTPMatch 			(const char *filename);
XMLPUBFUN void * XMLCALL	
	xmlIOHTTPOpen 			(const char *filename);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void * XMLCALL	
	xmlIOHTTPOpenW			(const char * post_uri,
					 int   compression );
#endif 
XMLPUBFUN int XMLCALL 	
	xmlIOHTTPRead			(void * context, 
					 char * buffer, 
					 int len);
XMLPUBFUN int XMLCALL	
	xmlIOHTTPClose 			(void * context);
#endif 

#ifdef LIBXML_FTP_ENABLED 
XMLPUBFUN int XMLCALL	
	xmlIOFTPMatch 			(const char *filename);
XMLPUBFUN void * XMLCALL	
	xmlIOFTPOpen 			(const char *filename);
XMLPUBFUN int XMLCALL 	
	xmlIOFTPRead			(void * context, 
					 char * buffer, 
					 int len);
XMLPUBFUN int XMLCALL 	
	xmlIOFTPClose 			(void * context);
#endif 

#ifdef __cplusplus
}
#endif

#endif 
