/*
 * Summary: minimal FTP implementation
 * Description: minimal FTP implementation allowing to fetch resources
 *              like external subset.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */
 
#ifndef __NANO_FTP_H__
#define __NANO_FTP_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_FTP_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ftpListCallback) (void *userData,
	                         const char *filename, const char *attrib,
	                         const char *owner, const char *group,
				 unsigned long size, int links, int year,
				 const char *month, int day, int hour,
				 int minute);
typedef void (*ftpDataCallback) (void *userData,
				 const char *data,
				 int len);

XMLPUBFUN void XMLCALL
	xmlNanoFTPInit		(void);
XMLPUBFUN void XMLCALL	
	xmlNanoFTPCleanup	(void);

XMLPUBFUN void * XMLCALL	
	xmlNanoFTPNewCtxt	(const char *URL);
XMLPUBFUN void XMLCALL	
	xmlNanoFTPFreeCtxt	(void * ctx);
XMLPUBFUN void * XMLCALL 	
	xmlNanoFTPConnectTo	(const char *server,
				 int port);
XMLPUBFUN void * XMLCALL 	
	xmlNanoFTPOpen		(const char *URL);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPConnect	(void *ctx);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPClose		(void *ctx);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPQuit		(void *ctx);
XMLPUBFUN void XMLCALL	
	xmlNanoFTPScanProxy	(const char *URL);
XMLPUBFUN void XMLCALL	
	xmlNanoFTPProxy		(const char *host,
				 int port,
				 const char *user,
				 const char *passwd,
				 int type);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPUpdateURL	(void *ctx,
				 const char *URL);

XMLPUBFUN int XMLCALL	
	xmlNanoFTPGetResponse	(void *ctx);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPCheckResponse	(void *ctx);

XMLPUBFUN int XMLCALL	
	xmlNanoFTPCwd		(void *ctx,
				 const char *directory);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPDele		(void *ctx,
				 const char *file);

XMLPUBFUN int XMLCALL	
	xmlNanoFTPGetConnection	(void *ctx);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPCloseConnection(void *ctx);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPList		(void *ctx,
				 ftpListCallback callback,
				 void *userData,
				 const char *filename);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPGetSocket	(void *ctx,
				 const char *filename);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPGet		(void *ctx,
				 ftpDataCallback callback,
				 void *userData,
				 const char *filename);
XMLPUBFUN int XMLCALL	
	xmlNanoFTPRead		(void *ctx,
				 void *dest,
				 int len);

#ifdef __cplusplus
}
#endif
#endif 
#endif 
