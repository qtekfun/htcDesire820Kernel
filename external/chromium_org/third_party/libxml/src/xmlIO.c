/*
 * xmlIO.c : implementation of the I/O interfaces used by the parser
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 *
 * 14 Nov 2000 ht - for VMS, truncated name of long functions to under 32 char
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif


#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_ZLIB_H
#include <zlib.h>
#endif

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif

#if defined(_WIN32_WCE)
#include <winnls.h> 
#endif

#ifndef HAVE_STAT
#  ifdef HAVE__STAT
#    ifndef _MSC_VER
#      define stat(x,y) _stat(x,y)
#    endif
#    define HAVE_STAT
#  endif
#else
#  ifdef HAVE__STAT
#    if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
#      define stat _stat
#    endif
#  endif
#endif
#ifdef HAVE_STAT
#  ifndef S_ISDIR
#    ifdef _S_ISDIR
#      define S_ISDIR(x) _S_ISDIR(x)
#    else
#      ifdef S_IFDIR
#        ifndef S_IFMT
#          ifdef _S_IFMT
#            define S_IFMT _S_IFMT
#          endif
#        endif
#        ifdef S_IFMT
#          define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#        endif
#      endif
#    endif
#  endif
#endif

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlIO.h>
#include <libxml/uri.h>
#include <libxml/nanohttp.h>
#include <libxml/nanoftp.h>
#include <libxml/xmlerror.h>
#ifdef LIBXML_CATALOG_ENABLED
#include <libxml/catalog.h>
#endif
#include <libxml/globals.h>


#ifdef DEBUG_INPUT
#define MINLEN 40
#else
#define MINLEN 4000
#endif

typedef struct _xmlInputCallback {
    xmlInputMatchCallback matchcallback;
    xmlInputOpenCallback opencallback;
    xmlInputReadCallback readcallback;
    xmlInputCloseCallback closecallback;
} xmlInputCallback;

#define MAX_INPUT_CALLBACK 15

static xmlInputCallback xmlInputCallbackTable[MAX_INPUT_CALLBACK];
static int xmlInputCallbackNr = 0;
static int xmlInputCallbackInitialized = 0;

#ifdef LIBXML_OUTPUT_ENABLED
typedef struct _xmlOutputCallback {
    xmlOutputMatchCallback matchcallback;
    xmlOutputOpenCallback opencallback;
    xmlOutputWriteCallback writecallback;
    xmlOutputCloseCallback closecallback;
} xmlOutputCallback;

#define MAX_OUTPUT_CALLBACK 15

static xmlOutputCallback xmlOutputCallbackTable[MAX_OUTPUT_CALLBACK];
static int xmlOutputCallbackNr = 0;
static int xmlOutputCallbackInitialized = 0;

xmlOutputBufferPtr
xmlAllocOutputBufferInternal(xmlCharEncodingHandlerPtr encoder);
#endif 


static const char *IOerr[] = {
    "Unknown IO error",         
    "Permission denied",	
    "Resource temporarily unavailable",
    "Bad file descriptor",	
    "Bad message",		
    "Resource busy",		
    "Operation canceled",	
    "No child processes",	
    "Resource deadlock avoided",
    "Domain error",		
    "File exists",		
    "Bad address",		
    "File too large",		
    "Operation in progress",	
    "Interrupted function call",
    "Invalid argument",		
    "Input/output error",	
    "Is a directory",		
    "Too many open files",	
    "Too many links",		
    "Inappropriate message buffer length",
    "Filename too long",	
    "Too many open files in system",
    "No such device",		
    "No such file or directory",
    "Exec format error",	
    "No locks available",	
    "Not enough space",		
    "No space left on device",	
    "Function not implemented",	
    "Not a directory",		
    "Directory not empty",	
    "Not supported",		
    "Inappropriate I/O control operation",
    "No such device or address",
    "Operation not permitted",	
    "Broken pipe",		
    "Result too large",		
    "Read-only file system",	
    "Invalid seek",		
    "No such process",		
    "Operation timed out",	
    "Improper link",		
    "Attempt to load network entity %s", 
    "encoder error",		
    "flush error",
    "write error",
    "no input",
    "buffer full",
    "loading error",
    "not a socket",		
    "already connected",	
    "connection refused",	
    "unreachable network",	
    "adddress in use",		
    "already in use",		
    "unknown address familly",	
};

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
static wchar_t *
__xmlIOWin32UTF8ToWChar(const char *u8String)
{
    wchar_t *wString = NULL;

    if (u8String) {
        int wLen =
            MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, u8String,
                                -1, NULL, 0);
        if (wLen) {
            wString = xmlMalloc(wLen * sizeof(wchar_t));
            if (wString) {
                if (MultiByteToWideChar
                    (CP_UTF8, 0, u8String, -1, wString, wLen) == 0) {
                    xmlFree(wString);
                    wString = NULL;
                }
            }
        }
    }

    return wString;
}
#endif

static void
xmlIOErrMemory(const char *extra)
{
    __xmlSimpleError(XML_FROM_IO, XML_ERR_NO_MEMORY, NULL, NULL, extra);
}

void
__xmlIOErr(int domain, int code, const char *extra)
{
    unsigned int idx;

    if (code == 0) {
#ifdef HAVE_ERRNO_H
	if (errno == 0) code = 0;
#ifdef EACCES
        else if (errno == EACCES) code = XML_IO_EACCES;
#endif
#ifdef EAGAIN
        else if (errno == EAGAIN) code = XML_IO_EAGAIN;
#endif
#ifdef EBADF
        else if (errno == EBADF) code = XML_IO_EBADF;
#endif
#ifdef EBADMSG
        else if (errno == EBADMSG) code = XML_IO_EBADMSG;
#endif
#ifdef EBUSY
        else if (errno == EBUSY) code = XML_IO_EBUSY;
#endif
#ifdef ECANCELED
        else if (errno == ECANCELED) code = XML_IO_ECANCELED;
#endif
#ifdef ECHILD
        else if (errno == ECHILD) code = XML_IO_ECHILD;
#endif
#ifdef EDEADLK
        else if (errno == EDEADLK) code = XML_IO_EDEADLK;
#endif
#ifdef EDOM
        else if (errno == EDOM) code = XML_IO_EDOM;
#endif
#ifdef EEXIST
        else if (errno == EEXIST) code = XML_IO_EEXIST;
#endif
#ifdef EFAULT
        else if (errno == EFAULT) code = XML_IO_EFAULT;
#endif
#ifdef EFBIG
        else if (errno == EFBIG) code = XML_IO_EFBIG;
#endif
#ifdef EINPROGRESS
        else if (errno == EINPROGRESS) code = XML_IO_EINPROGRESS;
#endif
#ifdef EINTR
        else if (errno == EINTR) code = XML_IO_EINTR;
#endif
#ifdef EINVAL
        else if (errno == EINVAL) code = XML_IO_EINVAL;
#endif
#ifdef EIO
        else if (errno == EIO) code = XML_IO_EIO;
#endif
#ifdef EISDIR
        else if (errno == EISDIR) code = XML_IO_EISDIR;
#endif
#ifdef EMFILE
        else if (errno == EMFILE) code = XML_IO_EMFILE;
#endif
#ifdef EMLINK
        else if (errno == EMLINK) code = XML_IO_EMLINK;
#endif
#ifdef EMSGSIZE
        else if (errno == EMSGSIZE) code = XML_IO_EMSGSIZE;
#endif
#ifdef ENAMETOOLONG
        else if (errno == ENAMETOOLONG) code = XML_IO_ENAMETOOLONG;
#endif
#ifdef ENFILE
        else if (errno == ENFILE) code = XML_IO_ENFILE;
#endif
#ifdef ENODEV
        else if (errno == ENODEV) code = XML_IO_ENODEV;
#endif
#ifdef ENOENT
        else if (errno == ENOENT) code = XML_IO_ENOENT;
#endif
#ifdef ENOEXEC
        else if (errno == ENOEXEC) code = XML_IO_ENOEXEC;
#endif
#ifdef ENOLCK
        else if (errno == ENOLCK) code = XML_IO_ENOLCK;
#endif
#ifdef ENOMEM
        else if (errno == ENOMEM) code = XML_IO_ENOMEM;
#endif
#ifdef ENOSPC
        else if (errno == ENOSPC) code = XML_IO_ENOSPC;
#endif
#ifdef ENOSYS
        else if (errno == ENOSYS) code = XML_IO_ENOSYS;
#endif
#ifdef ENOTDIR
        else if (errno == ENOTDIR) code = XML_IO_ENOTDIR;
#endif
#ifdef ENOTEMPTY
        else if (errno == ENOTEMPTY) code = XML_IO_ENOTEMPTY;
#endif
#ifdef ENOTSUP
        else if (errno == ENOTSUP) code = XML_IO_ENOTSUP;
#endif
#ifdef ENOTTY
        else if (errno == ENOTTY) code = XML_IO_ENOTTY;
#endif
#ifdef ENXIO
        else if (errno == ENXIO) code = XML_IO_ENXIO;
#endif
#ifdef EPERM
        else if (errno == EPERM) code = XML_IO_EPERM;
#endif
#ifdef EPIPE
        else if (errno == EPIPE) code = XML_IO_EPIPE;
#endif
#ifdef ERANGE
        else if (errno == ERANGE) code = XML_IO_ERANGE;
#endif
#ifdef EROFS
        else if (errno == EROFS) code = XML_IO_EROFS;
#endif
#ifdef ESPIPE
        else if (errno == ESPIPE) code = XML_IO_ESPIPE;
#endif
#ifdef ESRCH
        else if (errno == ESRCH) code = XML_IO_ESRCH;
#endif
#ifdef ETIMEDOUT
        else if (errno == ETIMEDOUT) code = XML_IO_ETIMEDOUT;
#endif
#ifdef EXDEV
        else if (errno == EXDEV) code = XML_IO_EXDEV;
#endif
#ifdef ENOTSOCK
        else if (errno == ENOTSOCK) code = XML_IO_ENOTSOCK;
#endif
#ifdef EISCONN
        else if (errno == EISCONN) code = XML_IO_EISCONN;
#endif
#ifdef ECONNREFUSED
        else if (errno == ECONNREFUSED) code = XML_IO_ECONNREFUSED;
#endif
#ifdef ETIMEDOUT
        else if (errno == ETIMEDOUT) code = XML_IO_ETIMEDOUT;
#endif
#ifdef ENETUNREACH
        else if (errno == ENETUNREACH) code = XML_IO_ENETUNREACH;
#endif
#ifdef EADDRINUSE
        else if (errno == EADDRINUSE) code = XML_IO_EADDRINUSE;
#endif
#ifdef EINPROGRESS
        else if (errno == EINPROGRESS) code = XML_IO_EINPROGRESS;
#endif
#ifdef EALREADY
        else if (errno == EALREADY) code = XML_IO_EALREADY;
#endif
#ifdef EAFNOSUPPORT
        else if (errno == EAFNOSUPPORT) code = XML_IO_EAFNOSUPPORT;
#endif
        else code = XML_IO_UNKNOWN;
#endif 
    }
    idx = 0;
    if (code >= XML_IO_UNKNOWN) idx = code - XML_IO_UNKNOWN;
    if (idx >= (sizeof(IOerr) / sizeof(IOerr[0]))) idx = 0;

    __xmlSimpleError(domain, code, NULL, IOerr[idx], extra);
}

static void
xmlIOErr(int code, const char *extra)
{
    __xmlIOErr(XML_FROM_IO, code, extra);
}

void
__xmlLoaderErr(void *ctx, const char *msg, const char *filename)
{
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    xmlStructuredErrorFunc schannel = NULL;
    xmlGenericErrorFunc channel = NULL;
    void *data = NULL;
    xmlErrorLevel level = XML_ERR_ERROR;

    if ((ctxt != NULL) && (ctxt->disableSAX != 0) &&
        (ctxt->instate == XML_PARSER_EOF))
	return;
    if ((ctxt != NULL) && (ctxt->sax != NULL)) {
        if (ctxt->validate) {
	    channel = ctxt->sax->error;
	    level = XML_ERR_ERROR;
	} else {
	    channel = ctxt->sax->warning;
	    level = XML_ERR_WARNING;
	}
	if (ctxt->sax->initialized == XML_SAX2_MAGIC)
	    schannel = ctxt->sax->serror;
	data = ctxt->userData;
    }
    __xmlRaiseError(schannel, channel, data, ctxt, NULL, XML_FROM_IO,
                    XML_IO_LOAD_ERROR, level, NULL, 0,
		    filename, NULL, NULL, 0, 0,
		    msg, filename);

}

xmlChar *
xmlNormalizeWindowsPath(const xmlChar *path)
{
    return xmlCanonicPath(path);
}

void
xmlCleanupInputCallbacks(void)
{
    int i;

    if (!xmlInputCallbackInitialized)
        return;

    for (i = xmlInputCallbackNr - 1; i >= 0; i--) {
        xmlInputCallbackTable[i].matchcallback = NULL;
        xmlInputCallbackTable[i].opencallback = NULL;
        xmlInputCallbackTable[i].readcallback = NULL;
        xmlInputCallbackTable[i].closecallback = NULL;
    }

    xmlInputCallbackNr = 0;
    xmlInputCallbackInitialized = 0;
}

int
xmlPopInputCallbacks(void)
{
    if (!xmlInputCallbackInitialized)
        return(-1);

    if (xmlInputCallbackNr <= 0)
        return(-1);

    xmlInputCallbackNr--;
    xmlInputCallbackTable[xmlInputCallbackNr].matchcallback = NULL;
    xmlInputCallbackTable[xmlInputCallbackNr].opencallback = NULL;
    xmlInputCallbackTable[xmlInputCallbackNr].readcallback = NULL;
    xmlInputCallbackTable[xmlInputCallbackNr].closecallback = NULL;

    return(xmlInputCallbackNr);
}

#ifdef LIBXML_OUTPUT_ENABLED
void
xmlCleanupOutputCallbacks(void)
{
    int i;

    if (!xmlOutputCallbackInitialized)
        return;

    for (i = xmlOutputCallbackNr - 1; i >= 0; i--) {
        xmlOutputCallbackTable[i].matchcallback = NULL;
        xmlOutputCallbackTable[i].opencallback = NULL;
        xmlOutputCallbackTable[i].writecallback = NULL;
        xmlOutputCallbackTable[i].closecallback = NULL;
    }

    xmlOutputCallbackNr = 0;
    xmlOutputCallbackInitialized = 0;
}
#endif 


#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)

static FILE*
xmlWrapOpenUtf8(const char *path,int mode)
{
    FILE *fd = NULL;
    wchar_t *wPath;

    wPath = __xmlIOWin32UTF8ToWChar(path);
    if(wPath)
    {
       fd = _wfopen(wPath, mode ? L"wb" : L"rb");
       xmlFree(wPath);
    }
    
    if(fd == NULL)
       fd = fopen(path, mode ? "wb" : "rb");

    return fd;
}

#ifdef HAVE_ZLIB_H
static gzFile
xmlWrapGzOpenUtf8(const char *path, const char *mode)
{
    gzFile fd;
    wchar_t *wPath;

    fd = gzopen (path, mode);
    if (fd)
        return fd;

    wPath = __xmlIOWin32UTF8ToWChar(path);
    if(wPath)
    {
	int d, m = (strstr(mode, "r") ? O_RDONLY : O_RDWR);
#ifdef _O_BINARY
        m |= (strstr(mode, "b") ? _O_BINARY : 0);
#endif
	d = _wopen(wPath, m);
	if (d >= 0)
	    fd = gzdopen(d, mode);
        xmlFree(wPath);
    }

    return fd;
}
#endif

static int
xmlWrapStatUtf8(const char *path,struct stat *info)
{
#ifdef HAVE_STAT
    int retval = -1;
    wchar_t *wPath;

    wPath = __xmlIOWin32UTF8ToWChar(path);
    if (wPath)
    {
       retval = _wstat(wPath,info);
       xmlFree(wPath);
    }
    
    if(retval < 0)
       retval = stat(path,info);
    return retval;
#else
    return -1;
#endif
}

static FILE*
xmlWrapOpenNative(const char *path,int mode)
{
    return fopen(path,mode ? "wb" : "rb");
}

static int
xmlWrapStatNative(const char *path,struct stat *info)
{
#ifdef HAVE_STAT
    return stat(path,info);
#else
    return -1;
#endif
}

typedef int (* xmlWrapStatFunc) (const char *f, struct stat *s);
static xmlWrapStatFunc xmlWrapStat = xmlWrapStatNative;
typedef FILE* (* xmlWrapOpenFunc)(const char *f,int mode);
static xmlWrapOpenFunc xmlWrapOpen = xmlWrapOpenNative;
#ifdef HAVE_ZLIB_H
typedef gzFile (* xmlWrapGzOpenFunc) (const char *f, const char *mode);
static xmlWrapGzOpenFunc xmlWrapGzOpen = gzopen;
#endif
static void
xmlInitPlatformSpecificIo(void)
{
    static int xmlPlatformIoInitialized = 0;
    OSVERSIONINFO osvi;

    if(xmlPlatformIoInitialized)
      return;

    osvi.dwOSVersionInfoSize = sizeof(osvi);

    if(GetVersionEx(&osvi) && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)) {
      xmlWrapStat = xmlWrapStatUtf8;
      xmlWrapOpen = xmlWrapOpenUtf8;
#ifdef HAVE_ZLIB_H
      xmlWrapGzOpen = xmlWrapGzOpenUtf8;
#endif
    } else {
      xmlWrapStat = xmlWrapStatNative;
      xmlWrapOpen = xmlWrapOpenNative;
#ifdef HAVE_ZLIB_H
      xmlWrapGzOpen = gzopen;
#endif
    }

    xmlPlatformIoInitialized = 1;
    return;
}

#endif


int
xmlCheckFilename (const char *path)
{
#ifdef HAVE_STAT
	struct stat stat_buffer;
#endif
	if (path == NULL)
		return(0);

#ifdef HAVE_STAT
#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    if (xmlWrapStat(path, &stat_buffer) == -1)
        return 0;
#else
    if (stat(path, &stat_buffer) == -1)
        return 0;
#endif
#ifdef S_ISDIR
    if (S_ISDIR(stat_buffer.st_mode))
        return 2;
#endif
#endif 
    return 1;
}

static int
xmlNop(void) {
    return(0);
}

/**
 * xmlFdRead:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to read
 *
 * Read @len bytes to @buffer from the I/O channel.
 *
 * Returns the number of bytes written
 */
static int
xmlFdRead (void * context, char * buffer, int len) {
    int ret;

    ret = read((int) (long) context, &buffer[0], len);
    if (ret < 0) xmlIOErr(0, "read()");
    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlFdWrite:
 * @context:  the I/O context
 * @buffer:  where to get data
 * @len:  number of bytes to write
 *
 * Write @len bytes from @buffer to the I/O channel.
 *
 * Returns the number of bytes written
 */
static int
xmlFdWrite (void * context, const char * buffer, int len) {
    int ret = 0;

    if (len > 0) {
	ret = write((int) (long) context, &buffer[0], len);
	if (ret < 0) xmlIOErr(0, "write()");
    }
    return(ret);
}
#endif 

static int
xmlFdClose (void * context) {
    int ret;
    ret = close((int) (long) context);
    if (ret < 0) xmlIOErr(0, "close()");
    return(ret);
}

int
xmlFileMatch (const char *filename ATTRIBUTE_UNUSED) {
    return(1);
}

static void *
xmlFileOpen_real (const char *filename) {
    const char *path = NULL;
    FILE *fd;

    if (filename == NULL)
        return(NULL);

    if (!strcmp(filename, "-")) {
	fd = stdin;
	return((void *) fd);
    }

    if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file://localhost/", 17)) {
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[17];
#else
	path = &filename[16];
#endif
    } else if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file:///", 8)) {
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[8];
#else
	path = &filename[7];
#endif
    } else if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file:/", 6)) {
        
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[6];
#else
	path = &filename[5];
#endif
    } else
	path = filename;

    if (path == NULL)
	return(NULL);
    if (!xmlCheckFilename(path))
        return(NULL);

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    fd = xmlWrapOpen(path, 0);
#else
    fd = fopen(path, "r");
#endif 
    if (fd == NULL) xmlIOErr(0, path);
    return((void *) fd);
}

void *
xmlFileOpen (const char *filename) {
    char *unescaped;
    void *retval;

    retval = xmlFileOpen_real(filename);
    if (retval == NULL) {
	unescaped = xmlURIUnescapeString(filename, 0, NULL);
	if (unescaped != NULL) {
	    retval = xmlFileOpen_real(unescaped);
	    xmlFree(unescaped);
	}
    }

    return retval;
}

#ifdef LIBXML_OUTPUT_ENABLED
static void *
xmlFileOpenW (const char *filename) {
    const char *path = NULL;
    FILE *fd;

    if (!strcmp(filename, "-")) {
	fd = stdout;
	return((void *) fd);
    }

    if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file://localhost/", 17))
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[17];
#else
	path = &filename[16];
#endif
    else if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file:///", 8)) {
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[8];
#else
	path = &filename[7];
#endif
    } else
	path = filename;

    if (path == NULL)
	return(NULL);

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    fd = xmlWrapOpen(path, 1);
#else
 	   fd = fopen(path, "wb");
#endif 

	 if (fd == NULL) xmlIOErr(0, path);
    return((void *) fd);
}
#endif 

/**
 * xmlFileRead:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to write
 *
 * Read @len bytes to @buffer from the I/O channel.
 *
 * Returns the number of bytes written or < 0 in case of failure
 */
int
xmlFileRead (void * context, char * buffer, int len) {
    int ret;
    if ((context == NULL) || (buffer == NULL))
        return(-1);
    ret = fread(&buffer[0], 1,  len, (FILE *) context);
    if (ret < 0) xmlIOErr(0, "fread()");
    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlFileWrite:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to write
 *
 * Write @len bytes from @buffer to the I/O channel.
 *
 * Returns the number of bytes written
 */
static int
xmlFileWrite (void * context, const char * buffer, int len) {
    int items;

    if ((context == NULL) || (buffer == NULL))
        return(-1);
    items = fwrite(&buffer[0], len, 1, (FILE *) context);
    if ((items == 0) && (ferror((FILE *) context))) {
        xmlIOErr(0, "fwrite()");
	return(-1);
    }
    return(items * len);
}
#endif 

int
xmlFileClose (void * context) {
    FILE *fil;
    int ret;

    if (context == NULL)
        return(-1);
    fil = (FILE *) context;
    if ((fil == stdout) || (fil == stderr)) {
        ret = fflush(fil);
	if (ret < 0)
	    xmlIOErr(0, "fflush()");
	return(0);
    }
    if (fil == stdin)
	return(0);
    ret = ( fclose((FILE *) context) == EOF ) ? -1 : 0;
    if (ret < 0)
        xmlIOErr(0, "fclose()");
    return(ret);
}

static int
xmlFileFlush (void * context) {
    int ret;

    if (context == NULL)
        return(-1);
    ret = ( fflush((FILE *) context) == EOF ) ? -1 : 0;
    if (ret < 0)
        xmlIOErr(0, "fflush()");
    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlBufferWrite:
 * @context:  the xmlBuffer
 * @buffer:  the data to write
 * @len:  number of bytes to write
 *
 * Write @len bytes from @buffer to the xml buffer
 *
 * Returns the number of bytes written
 */
static int
xmlBufferWrite (void * context, const char * buffer, int len) {
    int ret;

    ret = xmlBufferAdd((xmlBufferPtr) context, (const xmlChar *) buffer, len);
    if (ret != 0)
        return(-1);
    return(len);
}
#endif

#ifdef HAVE_ZLIB_H
static int
xmlGzfileMatch (const char *filename ATTRIBUTE_UNUSED) {
    return(1);
}

static void *
xmlGzfileOpen_real (const char *filename) {
    const char *path = NULL;
    gzFile fd;

    if (!strcmp(filename, "-")) {
        fd = gzdopen(dup(0), "rb");
	return((void *) fd);
    }

    if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file://localhost/", 17))
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[17];
#else
	path = &filename[16];
#endif
    else if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file:///", 8)) {
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[8];
#else
	path = &filename[7];
#endif
    } else
	path = filename;

    if (path == NULL)
	return(NULL);
    if (!xmlCheckFilename(path))
        return(NULL);

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    fd = xmlWrapGzOpen(path, "rb");
#else
    fd = gzopen(path, "rb");
#endif
    return((void *) fd);
}

static void *
xmlGzfileOpen (const char *filename) {
    char *unescaped;
    void *retval;

    retval = xmlGzfileOpen_real(filename);
    if (retval == NULL) {
	unescaped = xmlURIUnescapeString(filename, 0, NULL);
	if (unescaped != NULL) {
	    retval = xmlGzfileOpen_real(unescaped);
	}
	xmlFree(unescaped);
    }
    return retval;
}

#ifdef LIBXML_OUTPUT_ENABLED
static void *
xmlGzfileOpenW (const char *filename, int compression) {
    const char *path = NULL;
    char mode[15];
    gzFile fd;

    snprintf(mode, sizeof(mode), "wb%d", compression);
    if (!strcmp(filename, "-")) {
        fd = gzdopen(dup(1), mode);
	return((void *) fd);
    }

    if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file://localhost/", 17))
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[17];
#else
	path = &filename[16];
#endif
    else if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "file:///", 8)) {
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &filename[8];
#else
	path = &filename[7];
#endif
    } else
	path = filename;

    if (path == NULL)
	return(NULL);

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    fd = xmlWrapGzOpen(path, mode);
#else
    fd = gzopen(path, mode);
#endif
    return((void *) fd);
}
#endif 

/**
 * xmlGzfileRead:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to write
 *
 * Read @len bytes to @buffer from the compressed I/O channel.
 *
 * Returns the number of bytes written
 */
static int
xmlGzfileRead (void * context, char * buffer, int len) {
    int ret;

    ret = gzread((gzFile) context, &buffer[0], len);
    if (ret < 0) xmlIOErr(0, "gzread()");
    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlGzfileWrite:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to write
 *
 * Write @len bytes from @buffer to the compressed I/O channel.
 *
 * Returns the number of bytes written
 */
static int
xmlGzfileWrite (void * context, const char * buffer, int len) {
    int ret;

    ret = gzwrite((gzFile) context, (char *) &buffer[0], len);
    if (ret < 0) xmlIOErr(0, "gzwrite()");
    return(ret);
}
#endif 

static int
xmlGzfileClose (void * context) {
    int ret;

    ret =  (gzclose((gzFile) context) == Z_OK ) ? 0 : -1;
    if (ret < 0) xmlIOErr(0, "gzclose()");
    return(ret);
}
#endif 

#ifdef LIBXML_HTTP_ENABLED

#ifdef LIBXML_OUTPUT_ENABLED
typedef struct xmlIOHTTPWriteCtxt_
{
    int			compression;

    char *		uri;

    void *		doc_buff;

} xmlIOHTTPWriteCtxt, *xmlIOHTTPWriteCtxtPtr;

#ifdef HAVE_ZLIB_H

#define DFLT_WBITS		( -15 )
#define DFLT_MEM_LVL		( 8 )
#define GZ_MAGIC1		( 0x1f )
#define GZ_MAGIC2		( 0x8b )
#define LXML_ZLIB_OS_CODE	( 0x03 )
#define INIT_HTTP_BUFF_SIZE	( 32768 )
#define DFLT_ZLIB_RATIO		( 5 )


typedef struct xmlZMemBuff_
{
   unsigned long	size;
   unsigned long	crc;

   unsigned char *	zbuff;
   z_stream		zctrl;

} xmlZMemBuff, *xmlZMemBuffPtr;

static void
append_reverse_ulong( xmlZMemBuff * buff, unsigned long data ) {

    int		idx;

    if ( buff == NULL )
	return;


    for ( idx = 0; idx < 4; idx++ ) {
	*buff->zctrl.next_out = ( data & 0xff );
	data >>= 8;
	buff->zctrl.next_out++;
    }

    return;
}

static void
xmlFreeZMemBuff( xmlZMemBuffPtr buff ) {

#ifdef DEBUG_HTTP
    int z_err;
#endif

    if ( buff == NULL )
	return;

    xmlFree( buff->zbuff );
#ifdef DEBUG_HTTP
    z_err = deflateEnd( &buff->zctrl );
    if ( z_err != Z_OK )
	xmlGenericError( xmlGenericErrorContext,
			"xmlFreeZMemBuff:  Error releasing zlib context:  %d\n",
			z_err );
#else
    deflateEnd( &buff->zctrl );
#endif

    xmlFree( buff );
    return;
}

static void *
xmlCreateZMemBuff( int compression ) {

    int			z_err;
    int			hdr_lgth;
    xmlZMemBuffPtr	buff = NULL;

    if ( ( compression < 1 ) || ( compression > 9 ) )
	return ( NULL );

    

    buff = xmlMalloc( sizeof( xmlZMemBuff ) );
    if ( buff == NULL ) {
	xmlIOErrMemory("creating buffer context");
	return ( NULL );
    }

    (void)memset( buff, 0, sizeof( xmlZMemBuff ) );
    buff->size = INIT_HTTP_BUFF_SIZE;
    buff->zbuff = xmlMalloc( buff->size );
    if ( buff->zbuff == NULL ) {
	xmlFreeZMemBuff( buff );
	xmlIOErrMemory("creating buffer");
	return ( NULL );
    }

    z_err = deflateInit2( &buff->zctrl, compression, Z_DEFLATED,
			    DFLT_WBITS, DFLT_MEM_LVL, Z_DEFAULT_STRATEGY );
    if ( z_err != Z_OK ) {
	xmlChar msg[500];
	xmlFreeZMemBuff( buff );
	buff = NULL;
	xmlStrPrintf(msg, 500,
		    (const xmlChar *) "xmlCreateZMemBuff:  %s %d\n",
		    "Error initializing compression context.  ZLIB error:",
		    z_err );
	xmlIOErr(XML_IO_WRITE, (const char *) msg);
	return ( NULL );
    }

    
    buff->crc = crc32( 0L, NULL, 0 );
    hdr_lgth = snprintf( (char *)buff->zbuff, buff->size,
			"%c%c%c%c%c%c%c%c%c%c",
			GZ_MAGIC1, GZ_MAGIC2, Z_DEFLATED,
			0, 0, 0, 0, 0, 0, LXML_ZLIB_OS_CODE );
    buff->zctrl.next_out  = buff->zbuff + hdr_lgth;
    buff->zctrl.avail_out = buff->size - hdr_lgth;

    return ( buff );
}

static int
xmlZMemBuffExtend( xmlZMemBuffPtr buff, size_t ext_amt ) {

    int			rc = -1;
    size_t		new_size;
    size_t		cur_used;

    unsigned char *	tmp_ptr = NULL;

    if ( buff == NULL )
	return ( -1 );

    else if ( ext_amt == 0 )
	return ( 0 );

    cur_used = buff->zctrl.next_out - buff->zbuff;
    new_size = buff->size + ext_amt;

#ifdef DEBUG_HTTP
    if ( cur_used > new_size )
	xmlGenericError( xmlGenericErrorContext,
			"xmlZMemBuffExtend:  %s\n%s %d bytes.\n",
			"Buffer overwrite detected during compressed memory",
			"buffer extension.  Overflowed by",
			(cur_used - new_size ) );
#endif

    tmp_ptr = xmlRealloc( buff->zbuff, new_size );
    if ( tmp_ptr != NULL ) {
	rc = 0;
	buff->size  = new_size;
	buff->zbuff = tmp_ptr;
	buff->zctrl.next_out  = tmp_ptr + cur_used;
	buff->zctrl.avail_out = new_size - cur_used;
    }
    else {
	xmlChar msg[500];
	xmlStrPrintf(msg, 500,
		    (const xmlChar *) "xmlZMemBuffExtend:  %s %lu bytes.\n",
		    "Allocation failure extending output buffer to",
		    new_size );
	xmlIOErr(XML_IO_WRITE, (const char *) msg);
    }

    return ( rc );
}

static int
xmlZMemBuffAppend( xmlZMemBuffPtr buff, const char * src, int len ) {

    int		z_err;
    size_t	min_accept;

    if ( ( buff == NULL ) || ( src == NULL ) )
	return ( -1 );

    buff->zctrl.avail_in = len;
    buff->zctrl.next_in  = (unsigned char *)src;
    while ( buff->zctrl.avail_in > 0 ) {
	min_accept = buff->zctrl.avail_in / DFLT_ZLIB_RATIO;
	if ( buff->zctrl.avail_out <= min_accept ) {
	    if ( xmlZMemBuffExtend( buff, buff->size ) == -1 )
		return ( -1 );
	}

	z_err = deflate( &buff->zctrl, Z_NO_FLUSH );
	if ( z_err != Z_OK ) {
	    xmlChar msg[500];
	    xmlStrPrintf(msg, 500,
			(const xmlChar *) "xmlZMemBuffAppend:  %s %d %s - %d",
			"Compression error while appending",
			len, "bytes to buffer.  ZLIB error", z_err );
	    xmlIOErr(XML_IO_WRITE, (const char *) msg);
	    return ( -1 );
	}
    }

    buff->crc = crc32( buff->crc, (unsigned char *)src, len );

    return ( len );
}

static int
xmlZMemBuffGetContent( xmlZMemBuffPtr buff, char ** data_ref ) {

    int		zlgth = -1;
    int		z_err;

    if ( ( buff == NULL ) || ( data_ref == NULL ) )
	return ( -1 );

    

    do
    {
	z_err = deflate( &buff->zctrl, Z_FINISH );
	if ( z_err == Z_OK ) {
	    

	    if ( xmlZMemBuffExtend( buff, buff->size ) == -1 )
		return ( -1 );
	}
    }
    while ( z_err == Z_OK );

    

    if ( z_err == Z_STREAM_END ) {

	

	if ( buff->zctrl.avail_out < ( 2 * sizeof( unsigned long ) ) ) {
	    if ( xmlZMemBuffExtend(buff, (2 * sizeof(unsigned long))) == -1 )
		return ( -1 );
	}


	append_reverse_ulong( buff, buff->crc );
	append_reverse_ulong( buff, buff->zctrl.total_in );

	zlgth = buff->zctrl.next_out - buff->zbuff;
	*data_ref = (char *)buff->zbuff;
    }

    else {
	xmlChar msg[500];
	xmlStrPrintf(msg, 500,
		    (const xmlChar *) "xmlZMemBuffGetContent:  %s - %d\n",
		    "Error flushing zlib buffers.  Error code", z_err );
	xmlIOErr(XML_IO_WRITE, (const char *) msg);
    }

    return ( zlgth );
}
#endif 
#endif  

#ifdef LIBXML_OUTPUT_ENABLED
static void
xmlFreeHTTPWriteCtxt( xmlIOHTTPWriteCtxtPtr ctxt )
{
    if ( ctxt->uri != NULL )
	xmlFree( ctxt->uri );

    if ( ctxt->doc_buff != NULL ) {

#ifdef HAVE_ZLIB_H
	if ( ctxt->compression > 0 ) {
	    xmlFreeZMemBuff( ctxt->doc_buff );
	}
	else
#endif
	{
	    xmlOutputBufferClose( ctxt->doc_buff );
	}
    }

    xmlFree( ctxt );
    return;
}
#endif 


int
xmlIOHTTPMatch (const char *filename) {
    if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "http://", 7))
	return(1);
    return(0);
}

void *
xmlIOHTTPOpen (const char *filename) {
    return(xmlNanoHTTPOpen(filename, NULL));
}

#ifdef LIBXML_OUTPUT_ENABLED

void *
xmlIOHTTPOpenW(const char *post_uri, int compression)
{

    xmlIOHTTPWriteCtxtPtr ctxt = NULL;

    if (post_uri == NULL)
        return (NULL);

    ctxt = xmlMalloc(sizeof(xmlIOHTTPWriteCtxt));
    if (ctxt == NULL) {
	xmlIOErrMemory("creating HTTP output context");
        return (NULL);
    }

    (void) memset(ctxt, 0, sizeof(xmlIOHTTPWriteCtxt));

    ctxt->uri = (char *) xmlStrdup((const xmlChar *)post_uri);
    if (ctxt->uri == NULL) {
	xmlIOErrMemory("copying URI");
        xmlFreeHTTPWriteCtxt(ctxt);
        return (NULL);
    }


#ifdef HAVE_ZLIB_H
    if ((compression > 0) && (compression <= 9)) {

        ctxt->compression = compression;
        ctxt->doc_buff = xmlCreateZMemBuff(compression);
    } else
#endif
    {
        

        ctxt->doc_buff = xmlAllocOutputBufferInternal(NULL);
    }

    if (ctxt->doc_buff == NULL) {
        xmlFreeHTTPWriteCtxt(ctxt);
        ctxt = NULL;
    }

    return (ctxt);
}
#endif 

#ifdef LIBXML_OUTPUT_ENABLED
static void *
xmlIOHTTPDfltOpenW( const char * post_uri ) {
    return ( xmlIOHTTPOpenW( post_uri, 0 ) );
}
#endif 

/**
 * xmlIOHTTPRead:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to write
 *
 * Read @len bytes to @buffer from the I/O channel.
 *
 * Returns the number of bytes written
 */
int
xmlIOHTTPRead(void * context, char * buffer, int len) {
    if ((buffer == NULL) || (len < 0)) return(-1);
    return(xmlNanoHTTPRead(context, &buffer[0], len));
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlIOHTTPWrite
 * @context:  previously opened writing context
 * @buffer:   data to output to temporary buffer
 * @len:      bytes to output
 *
 * Collect data from memory buffer into a temporary file for later
 * processing.
 *
 * Returns number of bytes written.
 */

static int
xmlIOHTTPWrite( void * context, const char * buffer, int len ) {

    xmlIOHTTPWriteCtxtPtr	ctxt = context;

    if ( ( ctxt == NULL ) || ( ctxt->doc_buff == NULL ) || ( buffer == NULL ) )
	return ( -1 );

    if ( len > 0 ) {

	

#ifdef HAVE_ZLIB_H
	if ( ctxt->compression > 0 )
	    len = xmlZMemBuffAppend( ctxt->doc_buff, buffer, len );

	else
#endif
	    len = xmlOutputBufferWrite( ctxt->doc_buff, len, buffer );

	if ( len < 0 ) {
	    xmlChar msg[500];
	    xmlStrPrintf(msg, 500,
			(const xmlChar *) "xmlIOHTTPWrite:  %s\n%s '%s'.\n",
			"Error appending to internal buffer.",
			"Error sending document to URI",
			ctxt->uri );
	    xmlIOErr(XML_IO_WRITE, (const char *) msg);
	}
    }

    return ( len );
}
#endif 


int
xmlIOHTTPClose (void * context) {
    xmlNanoHTTPClose(context);
    return 0;
}

#ifdef LIBXML_OUTPUT_ENABLED
static int
xmlIOHTTPCloseWrite( void * context, const char * http_mthd ) {

    int				close_rc = -1;
    int				http_rtn = 0;
    int				content_lgth = 0;
    xmlIOHTTPWriteCtxtPtr	ctxt = context;

    char *			http_content = NULL;
    char *			content_encoding = NULL;
    char *			content_type = (char *) "text/xml";
    void *			http_ctxt = NULL;

    if ( ( ctxt == NULL ) || ( http_mthd == NULL ) )
	return ( -1 );

    

#ifdef HAVE_ZLIB_H

    if ( ctxt->compression > 0 ) {
	content_lgth = xmlZMemBuffGetContent( ctxt->doc_buff, &http_content );
	content_encoding = (char *) "Content-Encoding: gzip";
    }
    else
#endif
    {
	

	xmlOutputBufferPtr	dctxt = ctxt->doc_buff;
	http_content = (char *)dctxt->buffer->content;
	content_lgth = dctxt->buffer->use;
    }

    if ( http_content == NULL ) {
	xmlChar msg[500];
	xmlStrPrintf(msg, 500,
		     (const xmlChar *) "xmlIOHTTPCloseWrite:  %s '%s' %s '%s'.\n",
		     "Error retrieving content.\nUnable to",
		     http_mthd, "data to URI", ctxt->uri );
	xmlIOErr(XML_IO_WRITE, (const char *) msg);
    }

    else {

	http_ctxt = xmlNanoHTTPMethod( ctxt->uri, http_mthd, http_content,
					&content_type, content_encoding,
					content_lgth );

	if ( http_ctxt != NULL ) {
#ifdef DEBUG_HTTP
	    

	    FILE *	tst_file = NULL;
	    char	buffer[ 4096 ];
	    char *	dump_name = NULL;
	    int		avail;

	    xmlGenericError( xmlGenericErrorContext,
			"xmlNanoHTTPCloseWrite:  HTTP %s to\n%s returned %d.\n",
			http_mthd, ctxt->uri,
			xmlNanoHTTPReturnCode( http_ctxt ) );


	    dump_name = tempnam( NULL, "lxml" );
	    if ( dump_name != NULL ) {
		(void)snprintf( buffer, sizeof(buffer), "%s.content", dump_name );

		tst_file = fopen( buffer, "wb" );
		if ( tst_file != NULL ) {
		    xmlGenericError( xmlGenericErrorContext,
			"Transmitted content saved in file:  %s\n", buffer );

		    fwrite( http_content, sizeof( char ),
					content_lgth, tst_file );
		    fclose( tst_file );
		}

		(void)snprintf( buffer, sizeof(buffer), "%s.reply", dump_name );
		tst_file = fopen( buffer, "wb" );
		if ( tst_file != NULL ) {
		    xmlGenericError( xmlGenericErrorContext,
			"Reply content saved in file:  %s\n", buffer );


		    while ( (avail = xmlNanoHTTPRead( http_ctxt,
					buffer, sizeof( buffer ) )) > 0 ) {

			fwrite( buffer, sizeof( char ), avail, tst_file );
		    }

		    fclose( tst_file );
		}

		free( dump_name );
	    }
#endif  

	    http_rtn = xmlNanoHTTPReturnCode( http_ctxt );
	    if ( ( http_rtn >= 200 ) && ( http_rtn < 300 ) )
		close_rc = 0;
	    else {
                xmlChar msg[500];
                xmlStrPrintf(msg, 500,
    (const xmlChar *) "xmlIOHTTPCloseWrite: HTTP '%s' of %d %s\n'%s' %s %d\n",
			    http_mthd, content_lgth,
			    "bytes to URI", ctxt->uri,
			    "failed.  HTTP return code:", http_rtn );
		xmlIOErr(XML_IO_WRITE, (const char *) msg);
            }

	    xmlNanoHTTPClose( http_ctxt );
	    xmlFree( content_type );
	}
    }

    

    xmlFreeHTTPWriteCtxt( ctxt );

    return ( close_rc );
}

static int
xmlIOHTTPClosePut( void * ctxt ) {
    return ( xmlIOHTTPCloseWrite( ctxt, "PUT" ) );
}


static int
xmlIOHTTPClosePost( void * ctxt ) {
    return ( xmlIOHTTPCloseWrite( ctxt, "POST" ) );
}
#endif 

#endif 

#ifdef LIBXML_FTP_ENABLED
int
xmlIOFTPMatch (const char *filename) {
    if (!xmlStrncasecmp(BAD_CAST filename, BAD_CAST "ftp://", 6))
	return(1);
    return(0);
}

void *
xmlIOFTPOpen (const char *filename) {
    return(xmlNanoFTPOpen(filename));
}

/**
 * xmlIOFTPRead:
 * @context:  the I/O context
 * @buffer:  where to drop data
 * @len:  number of bytes to write
 *
 * Read @len bytes to @buffer from the I/O channel.
 *
 * Returns the number of bytes written
 */
int
xmlIOFTPRead(void * context, char * buffer, int len) {
    if ((buffer == NULL) || (len < 0)) return(-1);
    return(xmlNanoFTPRead(context, &buffer[0], len));
}

int
xmlIOFTPClose (void * context) {
    return ( xmlNanoFTPClose(context) );
}
#endif 


int
xmlRegisterInputCallbacks(xmlInputMatchCallback matchFunc,
	xmlInputOpenCallback openFunc, xmlInputReadCallback readFunc,
	xmlInputCloseCallback closeFunc) {
    if (xmlInputCallbackNr >= MAX_INPUT_CALLBACK) {
	return(-1);
    }
    xmlInputCallbackTable[xmlInputCallbackNr].matchcallback = matchFunc;
    xmlInputCallbackTable[xmlInputCallbackNr].opencallback = openFunc;
    xmlInputCallbackTable[xmlInputCallbackNr].readcallback = readFunc;
    xmlInputCallbackTable[xmlInputCallbackNr].closecallback = closeFunc;
    xmlInputCallbackInitialized = 1;
    return(xmlInputCallbackNr++);
}

#ifdef LIBXML_OUTPUT_ENABLED
int
xmlRegisterOutputCallbacks(xmlOutputMatchCallback matchFunc,
	xmlOutputOpenCallback openFunc, xmlOutputWriteCallback writeFunc,
	xmlOutputCloseCallback closeFunc) {
    if (xmlOutputCallbackNr >= MAX_OUTPUT_CALLBACK) {
	return(-1);
    }
    xmlOutputCallbackTable[xmlOutputCallbackNr].matchcallback = matchFunc;
    xmlOutputCallbackTable[xmlOutputCallbackNr].opencallback = openFunc;
    xmlOutputCallbackTable[xmlOutputCallbackNr].writecallback = writeFunc;
    xmlOutputCallbackTable[xmlOutputCallbackNr].closecallback = closeFunc;
    xmlOutputCallbackInitialized = 1;
    return(xmlOutputCallbackNr++);
}
#endif 

void
xmlRegisterDefaultInputCallbacks(void) {
    if (xmlInputCallbackInitialized)
	return;

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    xmlInitPlatformSpecificIo();
#endif

    xmlRegisterInputCallbacks(xmlFileMatch, xmlFileOpen,
	                      xmlFileRead, xmlFileClose);
#ifdef HAVE_ZLIB_H
    xmlRegisterInputCallbacks(xmlGzfileMatch, xmlGzfileOpen,
	                      xmlGzfileRead, xmlGzfileClose);
#endif 

#ifdef LIBXML_HTTP_ENABLED
    xmlRegisterInputCallbacks(xmlIOHTTPMatch, xmlIOHTTPOpen,
	                      xmlIOHTTPRead, xmlIOHTTPClose);
#endif 

#ifdef LIBXML_FTP_ENABLED
    xmlRegisterInputCallbacks(xmlIOFTPMatch, xmlIOFTPOpen,
	                      xmlIOFTPRead, xmlIOFTPClose);
#endif 
    xmlInputCallbackInitialized = 1;
}

#ifdef LIBXML_OUTPUT_ENABLED
void
xmlRegisterDefaultOutputCallbacks (void) {
    if (xmlOutputCallbackInitialized)
	return;

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
    xmlInitPlatformSpecificIo();
#endif

    xmlRegisterOutputCallbacks(xmlFileMatch, xmlFileOpenW,
	                      xmlFileWrite, xmlFileClose);

#ifdef LIBXML_HTTP_ENABLED
    xmlRegisterOutputCallbacks(xmlIOHTTPMatch, xmlIOHTTPDfltOpenW,
	                       xmlIOHTTPWrite, xmlIOHTTPClosePut);
#endif

    xmlOutputCallbackInitialized = 1;
}

#ifdef LIBXML_HTTP_ENABLED
void
xmlRegisterHTTPPostCallbacks( void ) {

    

    if ( xmlOutputCallbackInitialized == 0 )
	xmlRegisterDefaultOutputCallbacks( );

    xmlRegisterOutputCallbacks(xmlIOHTTPMatch, xmlIOHTTPDfltOpenW,
	                       xmlIOHTTPWrite, xmlIOHTTPClosePost);
    return;
}
#endif
#endif 

xmlParserInputBufferPtr
xmlAllocParserInputBuffer(xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;

    ret = (xmlParserInputBufferPtr) xmlMalloc(sizeof(xmlParserInputBuffer));
    if (ret == NULL) {
	xmlIOErrMemory("creating input buffer");
	return(NULL);
    }
    memset(ret, 0, (size_t) sizeof(xmlParserInputBuffer));
    ret->buffer = xmlBufferCreateSize(2 * xmlDefaultBufferSize);
    if (ret->buffer == NULL) {
        xmlFree(ret);
	return(NULL);
    }
    ret->buffer->alloc = XML_BUFFER_ALLOC_DOUBLEIT;
    ret->encoder = xmlGetCharEncodingHandler(enc);
    if (ret->encoder != NULL)
        ret->raw = xmlBufferCreateSize(2 * xmlDefaultBufferSize);
    else
        ret->raw = NULL;
    ret->readcallback = NULL;
    ret->closecallback = NULL;
    ret->context = NULL;
    ret->compressed = -1;
    ret->rawconsumed = 0;

    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
xmlOutputBufferPtr
xmlAllocOutputBuffer(xmlCharEncodingHandlerPtr encoder) {
    xmlOutputBufferPtr ret;

    ret = (xmlOutputBufferPtr) xmlMalloc(sizeof(xmlOutputBuffer));
    if (ret == NULL) {
	xmlIOErrMemory("creating output buffer");
	return(NULL);
    }
    memset(ret, 0, (size_t) sizeof(xmlOutputBuffer));
    ret->buffer = xmlBufferCreate();
    if (ret->buffer == NULL) {
        xmlFree(ret);
	return(NULL);
    }

    
    if (ret->buffer->alloc == XML_BUFFER_ALLOC_EXACT)
        ret->buffer->alloc = XML_BUFFER_ALLOC_DOUBLEIT;

    ret->encoder = encoder;
    if (encoder != NULL) {
        ret->conv = xmlBufferCreateSize(4000);
	if (ret->conv == NULL) {
	    xmlFree(ret);
	    return(NULL);
	}

	xmlCharEncOutFunc(encoder, ret->conv, NULL);
    } else
        ret->conv = NULL;
    ret->writecallback = NULL;
    ret->closecallback = NULL;
    ret->context = NULL;
    ret->written = 0;

    return(ret);
}

xmlOutputBufferPtr
xmlAllocOutputBufferInternal(xmlCharEncodingHandlerPtr encoder) {
    xmlOutputBufferPtr ret;

    ret = (xmlOutputBufferPtr) xmlMalloc(sizeof(xmlOutputBuffer));
    if (ret == NULL) {
	xmlIOErrMemory("creating output buffer");
	return(NULL);
    }
    memset(ret, 0, (size_t) sizeof(xmlOutputBuffer));
    ret->buffer = xmlBufferCreate();
    if (ret->buffer == NULL) {
        xmlFree(ret);
	return(NULL);
    }


    ret->buffer->alloc = XML_BUFFER_ALLOC_IO;
    ret->buffer->contentIO = ret->buffer->content;

    ret->encoder = encoder;
    if (encoder != NULL) {
        ret->conv = xmlBufferCreateSize(4000);
	if (ret->conv == NULL) {
	    xmlFree(ret);
	    return(NULL);
	}

	xmlCharEncOutFunc(encoder, ret->conv, NULL);
    } else
        ret->conv = NULL;
    ret->writecallback = NULL;
    ret->closecallback = NULL;
    ret->context = NULL;
    ret->written = 0;

    return(ret);
}

#endif 

void
xmlFreeParserInputBuffer(xmlParserInputBufferPtr in) {
    if (in == NULL) return;

    if (in->raw) {
        xmlBufferFree(in->raw);
	in->raw = NULL;
    }
    if (in->encoder != NULL) {
        xmlCharEncCloseFunc(in->encoder);
    }
    if (in->closecallback != NULL) {
	in->closecallback(in->context);
    }
    if (in->buffer != NULL) {
        xmlBufferFree(in->buffer);
	in->buffer = NULL;
    }

    xmlFree(in);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlOutputBufferClose:
 * @out:  a buffered output
 *
 * flushes and close the output I/O channel
 * and free up all the associated resources
 *
 * Returns the number of byte written or -1 in case of error.
 */
int
xmlOutputBufferClose(xmlOutputBufferPtr out)
{
    int written;
    int err_rc = 0;

    if (out == NULL)
        return (-1);
    if (out->writecallback != NULL)
        xmlOutputBufferFlush(out);
    if (out->closecallback != NULL) {
        err_rc = out->closecallback(out->context);
    }
    written = out->written;
    if (out->conv) {
        xmlBufferFree(out->conv);
        out->conv = NULL;
    }
    if (out->encoder != NULL) {
        xmlCharEncCloseFunc(out->encoder);
    }
    if (out->buffer != NULL) {
        xmlBufferFree(out->buffer);
        out->buffer = NULL;
    }

    if (out->error)
        err_rc = -1;
    xmlFree(out);
    return ((err_rc == 0) ? written : err_rc);
}
#endif 

xmlParserInputBufferPtr
__xmlParserInputBufferCreateFilename(const char *URI, xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;
    int i = 0;
    void *context = NULL;

    if (xmlInputCallbackInitialized == 0)
	xmlRegisterDefaultInputCallbacks();

    if (URI == NULL) return(NULL);

    if (context == NULL) {
	for (i = xmlInputCallbackNr - 1;i >= 0;i--) {
	    if ((xmlInputCallbackTable[i].matchcallback != NULL) &&
		(xmlInputCallbackTable[i].matchcallback(URI) != 0)) {
		context = xmlInputCallbackTable[i].opencallback(URI);
		if (context != NULL) {
		    break;
		}
	    }
	}
    }
    if (context == NULL) {
	return(NULL);
    }

    ret = xmlAllocParserInputBuffer(enc);
    if (ret != NULL) {
	ret->context = context;
	ret->readcallback = xmlInputCallbackTable[i].readcallback;
	ret->closecallback = xmlInputCallbackTable[i].closecallback;
#ifdef HAVE_ZLIB_H
	if ((xmlInputCallbackTable[i].opencallback == xmlGzfileOpen) &&
		(strcmp(URI, "-") != 0)) {
#if defined(ZLIB_VERNUM) && ZLIB_VERNUM >= 0x1230
            ret->compressed = !gzdirect(context);
#else
	    if (((z_stream *)context)->avail_in > 4) {
	        char *cptr, buff4[4];
		cptr = (char *) ((z_stream *)context)->next_in;
		if (gzread(context, buff4, 4) == 4) {
		    if (strncmp(buff4, cptr, 4) == 0)
		        ret->compressed = 0;
		    else
		        ret->compressed = 1;
		    gzrewind(context);
		}
	    }
#endif
	}
#endif
    }
    else
      xmlInputCallbackTable[i].closecallback (context);

    return(ret);
}

xmlParserInputBufferPtr
xmlParserInputBufferCreateFilename(const char *URI, xmlCharEncoding enc) {
    if ((xmlParserInputBufferCreateFilenameValue)) {
		return xmlParserInputBufferCreateFilenameValue(URI, enc);
	}
	return __xmlParserInputBufferCreateFilename(URI, enc);
}

#ifdef LIBXML_OUTPUT_ENABLED
xmlOutputBufferPtr
__xmlOutputBufferCreateFilename(const char *URI,
                              xmlCharEncodingHandlerPtr encoder,
                              int compression ATTRIBUTE_UNUSED) {
    xmlOutputBufferPtr ret;
    xmlURIPtr puri;
    int i = 0;
    void *context = NULL;
    char *unescaped = NULL;
#ifdef HAVE_ZLIB_H
    int is_file_uri = 1;
#endif

    if (xmlOutputCallbackInitialized == 0)
	xmlRegisterDefaultOutputCallbacks();

    if (URI == NULL) return(NULL);

    puri = xmlParseURI(URI);
    if (puri != NULL) {
#ifdef HAVE_ZLIB_H
        if ((puri->scheme != NULL) &&
	    (!xmlStrEqual(BAD_CAST puri->scheme, BAD_CAST "file")))
	    is_file_uri = 0;
#endif
	if ((puri->scheme == NULL) ||
	    (xmlStrEqual(BAD_CAST puri->scheme, BAD_CAST "file")))
	    unescaped = xmlURIUnescapeString(URI, 0, NULL);
	xmlFreeURI(puri);
    }

    if (unescaped != NULL) {
#ifdef HAVE_ZLIB_H
	if ((compression > 0) && (compression <= 9) && (is_file_uri == 1)) {
	    context = xmlGzfileOpenW(unescaped, compression);
	    if (context != NULL) {
		ret = xmlAllocOutputBufferInternal(encoder);
		if (ret != NULL) {
		    ret->context = context;
		    ret->writecallback = xmlGzfileWrite;
		    ret->closecallback = xmlGzfileClose;
		}
		xmlFree(unescaped);
		return(ret);
	    }
	}
#endif
	for (i = xmlOutputCallbackNr - 1;i >= 0;i--) {
	    if ((xmlOutputCallbackTable[i].matchcallback != NULL) &&
		(xmlOutputCallbackTable[i].matchcallback(unescaped) != 0)) {
#if defined(LIBXML_HTTP_ENABLED) && defined(HAVE_ZLIB_H)
		
		if (xmlOutputCallbackTable[i].matchcallback == xmlIOHTTPMatch)
		    context = xmlIOHTTPOpenW(unescaped, compression);
		else
#endif
		    context = xmlOutputCallbackTable[i].opencallback(unescaped);
		if (context != NULL)
		    break;
	    }
	}
	xmlFree(unescaped);
    }

    if (context == NULL) {
#ifdef HAVE_ZLIB_H
	if ((compression > 0) && (compression <= 9) && (is_file_uri == 1)) {
	    context = xmlGzfileOpenW(URI, compression);
	    if (context != NULL) {
		ret = xmlAllocOutputBufferInternal(encoder);
		if (ret != NULL) {
		    ret->context = context;
		    ret->writecallback = xmlGzfileWrite;
		    ret->closecallback = xmlGzfileClose;
		}
		return(ret);
	    }
	}
#endif
	for (i = xmlOutputCallbackNr - 1;i >= 0;i--) {
	    if ((xmlOutputCallbackTable[i].matchcallback != NULL) &&
		(xmlOutputCallbackTable[i].matchcallback(URI) != 0)) {
#if defined(LIBXML_HTTP_ENABLED) && defined(HAVE_ZLIB_H)
		
		if (xmlOutputCallbackTable[i].matchcallback == xmlIOHTTPMatch)
		    context = xmlIOHTTPOpenW(URI, compression);
		else
#endif
		    context = xmlOutputCallbackTable[i].opencallback(URI);
		if (context != NULL)
		    break;
	    }
	}
    }

    if (context == NULL) {
	return(NULL);
    }

    ret = xmlAllocOutputBufferInternal(encoder);
    if (ret != NULL) {
	ret->context = context;
	ret->writecallback = xmlOutputCallbackTable[i].writecallback;
	ret->closecallback = xmlOutputCallbackTable[i].closecallback;
    }
    return(ret);
}

xmlOutputBufferPtr
xmlOutputBufferCreateFilename(const char *URI,
                              xmlCharEncodingHandlerPtr encoder,
                              int compression ATTRIBUTE_UNUSED) {
    if ((xmlOutputBufferCreateFilenameValue)) {
		return xmlOutputBufferCreateFilenameValue(URI, encoder, compression);
	}
	return __xmlOutputBufferCreateFilename(URI, encoder, compression);
}
#endif 

xmlParserInputBufferPtr
xmlParserInputBufferCreateFile(FILE *file, xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;

    if (xmlInputCallbackInitialized == 0)
	xmlRegisterDefaultInputCallbacks();

    if (file == NULL) return(NULL);

    ret = xmlAllocParserInputBuffer(enc);
    if (ret != NULL) {
        ret->context = file;
	ret->readcallback = xmlFileRead;
	ret->closecallback = xmlFileFlush;
    }

    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
xmlOutputBufferPtr
xmlOutputBufferCreateFile(FILE *file, xmlCharEncodingHandlerPtr encoder) {
    xmlOutputBufferPtr ret;

    if (xmlOutputCallbackInitialized == 0)
	xmlRegisterDefaultOutputCallbacks();

    if (file == NULL) return(NULL);

    ret = xmlAllocOutputBufferInternal(encoder);
    if (ret != NULL) {
        ret->context = file;
	ret->writecallback = xmlFileWrite;
	ret->closecallback = xmlFileFlush;
    }

    return(ret);
}

xmlOutputBufferPtr
xmlOutputBufferCreateBuffer(xmlBufferPtr buffer,
                            xmlCharEncodingHandlerPtr encoder) {
    xmlOutputBufferPtr ret;

    if (buffer == NULL) return(NULL);

    ret = xmlOutputBufferCreateIO((xmlOutputWriteCallback)
                                  xmlBufferWrite,
                                  (xmlOutputCloseCallback)
                                  NULL, (void *) buffer, encoder);

    return(ret);
}

#endif 

xmlParserInputBufferPtr
xmlParserInputBufferCreateFd(int fd, xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;

    if (fd < 0) return(NULL);

    ret = xmlAllocParserInputBuffer(enc);
    if (ret != NULL) {
        ret->context = (void *) (long) fd;
	ret->readcallback = xmlFdRead;
	ret->closecallback = xmlFdClose;
    }

    return(ret);
}

xmlParserInputBufferPtr
xmlParserInputBufferCreateMem(const char *mem, int size, xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;
    int errcode;

    if (size <= 0) return(NULL);
    if (mem == NULL) return(NULL);

    ret = xmlAllocParserInputBuffer(enc);
    if (ret != NULL) {
        ret->context = (void *) mem;
	ret->readcallback = (xmlInputReadCallback) xmlNop;
	ret->closecallback = NULL;
	errcode = xmlBufferAdd(ret->buffer, (const xmlChar *) mem, size);
	if (errcode != 0) {
	    xmlFree(ret);
	    return(NULL);
	}
    }

    return(ret);
}

xmlParserInputBufferPtr
xmlParserInputBufferCreateStatic(const char *mem, int size,
                                 xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;

    if (size <= 0) return(NULL);
    if (mem == NULL) return(NULL);

    ret = (xmlParserInputBufferPtr) xmlMalloc(sizeof(xmlParserInputBuffer));
    if (ret == NULL) {
	xmlIOErrMemory("creating input buffer");
	return(NULL);
    }
    memset(ret, 0, (size_t) sizeof(xmlParserInputBuffer));
    ret->buffer = xmlBufferCreateStatic((void *)mem, (size_t) size);
    if (ret->buffer == NULL) {
        xmlFree(ret);
	return(NULL);
    }
    ret->encoder = xmlGetCharEncodingHandler(enc);
    if (ret->encoder != NULL)
        ret->raw = xmlBufferCreateSize(2 * xmlDefaultBufferSize);
    else
        ret->raw = NULL;
    ret->compressed = -1;
    ret->context = (void *) mem;
    ret->readcallback = NULL;
    ret->closecallback = NULL;

    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
xmlOutputBufferPtr
xmlOutputBufferCreateFd(int fd, xmlCharEncodingHandlerPtr encoder) {
    xmlOutputBufferPtr ret;

    if (fd < 0) return(NULL);

    ret = xmlAllocOutputBufferInternal(encoder);
    if (ret != NULL) {
        ret->context = (void *) (long) fd;
	ret->writecallback = xmlFdWrite;
	ret->closecallback = NULL;
    }

    return(ret);
}
#endif 

xmlParserInputBufferPtr
xmlParserInputBufferCreateIO(xmlInputReadCallback   ioread,
	 xmlInputCloseCallback  ioclose, void *ioctx, xmlCharEncoding enc) {
    xmlParserInputBufferPtr ret;

    if (ioread == NULL) return(NULL);

    ret = xmlAllocParserInputBuffer(enc);
    if (ret != NULL) {
        ret->context = (void *) ioctx;
	ret->readcallback = ioread;
	ret->closecallback = ioclose;
    }

    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
xmlOutputBufferPtr
xmlOutputBufferCreateIO(xmlOutputWriteCallback   iowrite,
	 xmlOutputCloseCallback  ioclose, void *ioctx,
	 xmlCharEncodingHandlerPtr encoder) {
    xmlOutputBufferPtr ret;

    if (iowrite == NULL) return(NULL);

    ret = xmlAllocOutputBufferInternal(encoder);
    if (ret != NULL) {
        ret->context = (void *) ioctx;
	ret->writecallback = iowrite;
	ret->closecallback = ioclose;
    }

    return(ret);
}
#endif 

xmlParserInputBufferCreateFilenameFunc
xmlParserInputBufferCreateFilenameDefault(xmlParserInputBufferCreateFilenameFunc func)
{
    xmlParserInputBufferCreateFilenameFunc old = xmlParserInputBufferCreateFilenameValue;
    if (old == NULL) {
		old = __xmlParserInputBufferCreateFilename;
	}

    xmlParserInputBufferCreateFilenameValue = func;
    return(old);
}

xmlOutputBufferCreateFilenameFunc
xmlOutputBufferCreateFilenameDefault(xmlOutputBufferCreateFilenameFunc func)
{
    xmlOutputBufferCreateFilenameFunc old = xmlOutputBufferCreateFilenameValue;
#ifdef LIBXML_OUTPUT_ENABLED
    if (old == NULL) {
		old = __xmlOutputBufferCreateFilename;
	}
#endif
    xmlOutputBufferCreateFilenameValue = func;
    return(old);
}

int
xmlParserInputBufferPush(xmlParserInputBufferPtr in,
	                 int len, const char *buf) {
    int nbchars = 0;
    int ret;

    if (len < 0) return(0);
    if ((in == NULL) || (in->error)) return(-1);
    if (in->encoder != NULL) {
        unsigned int use;

        if (in->raw == NULL) {
	    in->raw = xmlBufferCreate();
	}
	ret = xmlBufferAdd(in->raw, (const xmlChar *) buf, len);
	if (ret != 0)
	    return(-1);

	use = in->raw->use;
	nbchars = xmlCharEncInFunc(in->encoder, in->buffer, in->raw);
	if (nbchars < 0) {
	    xmlIOErr(XML_IO_ENCODER, NULL);
	    in->error = XML_IO_ENCODER;
	    return(-1);
	}
	in->rawconsumed += (use - in->raw->use);
    } else {
	nbchars = len;
        ret = xmlBufferAdd(in->buffer, (xmlChar *) buf, nbchars);
	if (ret != 0)
	    return(-1);
    }
#ifdef DEBUG_INPUT
    xmlGenericError(xmlGenericErrorContext,
	    "I/O: pushed %d chars, buffer %d/%d\n",
            nbchars, in->buffer->use, in->buffer->size);
#endif
    return(nbchars);
}

static int
endOfInput (void * context ATTRIBUTE_UNUSED,
	    char * buffer ATTRIBUTE_UNUSED,
	    int len ATTRIBUTE_UNUSED) {
    return(0);
}

int
xmlParserInputBufferGrow(xmlParserInputBufferPtr in, int len) {
    char *buffer = NULL;
    int res = 0;
    int nbchars = 0;
    int buffree;
    unsigned int needSize;

    if ((in == NULL) || (in->error)) return(-1);
    if ((len <= MINLEN) && (len != 4))
        len = MINLEN;

    buffree = in->buffer->size - in->buffer->use;
    if (buffree <= 0) {
	xmlIOErr(XML_IO_BUFFER_FULL, NULL);
	in->error = XML_IO_BUFFER_FULL;
	return(-1);
    }

    needSize = in->buffer->use + len + 1;
    if (needSize > in->buffer->size){
        if (!xmlBufferResize(in->buffer, needSize)){
	    xmlIOErrMemory("growing input buffer");
	    in->error = XML_ERR_NO_MEMORY;
            return(-1);
        }
    }
    buffer = (char *)&in->buffer->content[in->buffer->use];

    if (in->readcallback != NULL) {
	res = in->readcallback(in->context, &buffer[0], len);
	if (res <= 0)
	    in->readcallback = endOfInput;
    } else {
	xmlIOErr(XML_IO_NO_INPUT, NULL);
	in->error = XML_IO_NO_INPUT;
	return(-1);
    }
    if (res < 0) {
	return(-1);
    }
    len = res;
    if (in->encoder != NULL) {
        unsigned int use;

        if (in->raw == NULL) {
	    in->raw = xmlBufferCreate();
	}
	res = xmlBufferAdd(in->raw, (const xmlChar *) buffer, len);
	if (res != 0)
	    return(-1);

	use = in->raw->use;
	nbchars = xmlCharEncInFunc(in->encoder, in->buffer, in->raw);
	if (nbchars < 0) {
	    xmlIOErr(XML_IO_ENCODER, NULL);
	    in->error = XML_IO_ENCODER;
	    return(-1);
	}
	in->rawconsumed += (use - in->raw->use);
    } else {
	nbchars = len;
   	in->buffer->use += nbchars;
	buffer[nbchars] = 0;
    }
#ifdef DEBUG_INPUT
    xmlGenericError(xmlGenericErrorContext,
	    "I/O: read %d chars, buffer %d/%d\n",
            nbchars, in->buffer->use, in->buffer->size);
#endif
    return(nbchars);
}

int
xmlParserInputBufferRead(xmlParserInputBufferPtr in, int len) {
    if ((in == NULL) || (in->error)) return(-1);
    if (in->readcallback != NULL)
	return(xmlParserInputBufferGrow(in, len));
    else if ((in->buffer != NULL) &&
             (in->buffer->alloc == XML_BUFFER_ALLOC_IMMUTABLE))
	return(0);
    else
        return(-1);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlOutputBufferWrite:
 * @out:  a buffered parser output
 * @len:  the size in bytes of the array.
 * @buf:  an char array
 *
 * Write the content of the array in the output I/O buffer
 * This routine handle the I18N transcoding from internal UTF-8
 * The buffer is lossless, i.e. will store in case of partial
 * or delayed writes.
 *
 * Returns the number of chars immediately written, or -1
 *         in case of error.
 */
int
xmlOutputBufferWrite(xmlOutputBufferPtr out, int len, const char *buf) {
    int nbchars = 0; 
    int ret;         
    int written = 0; /* number of char written to I/O so far */
    int chunk;       

    if ((out == NULL) || (out->error)) return(-1);
    if (len < 0) return(0);
    if (out->error) return(-1);

    do {
	chunk = len;
	if (chunk > 4 * MINLEN)
	    chunk = 4 * MINLEN;

	if (out->encoder != NULL) {
	    if (out->conv == NULL) {
		out->conv = xmlBufferCreate();
	    }
	    ret = xmlBufferAdd(out->buffer, (const xmlChar *) buf, chunk);
	    if (ret != 0)
	        return(-1);

	    if ((out->buffer->use < MINLEN) && (chunk == len))
		goto done;

	    ret = xmlCharEncOutFunc(out->encoder, out->conv, out->buffer);
	    if ((ret < 0) && (ret != -3)) {
		xmlIOErr(XML_IO_ENCODER, NULL);
		out->error = XML_IO_ENCODER;
		return(-1);
	    }
	    nbchars = out->conv->use;
	} else {
	    ret = xmlBufferAdd(out->buffer, (const xmlChar *) buf, chunk);
	    if (ret != 0)
	        return(-1);
	    nbchars = out->buffer->use;
	}
	buf += chunk;
	len -= chunk;

	if ((nbchars < MINLEN) && (len <= 0))
	    goto done;

	if (out->writecallback) {
	    if (out->encoder != NULL) {
		ret = out->writecallback(out->context,
				 (const char *)out->conv->content, nbchars);
		if (ret >= 0)
		    xmlBufferShrink(out->conv, ret);
	    } else {
		ret = out->writecallback(out->context,
				 (const char *)out->buffer->content, nbchars);
		if (ret >= 0)
		    xmlBufferShrink(out->buffer, ret);
	    }
	    if (ret < 0) {
		xmlIOErr(XML_IO_WRITE, NULL);
		out->error = XML_IO_WRITE;
		return(ret);
	    }
	    out->written += ret;
	}
	written += nbchars;
    } while (len > 0);

done:
#ifdef DEBUG_INPUT
    xmlGenericError(xmlGenericErrorContext,
	    "I/O: wrote %d chars\n", written);
#endif
    return(written);
}

static int
xmlEscapeContent(unsigned char* out, int *outlen,
                 const xmlChar* in, int *inlen) {
    unsigned char* outstart = out;
    const unsigned char* base = in;
    unsigned char* outend = out + *outlen;
    const unsigned char* inend;

    inend = in + (*inlen);

    while ((in < inend) && (out < outend)) {
   	if (*in == '<') {
	    if (outend - out < 4) break;
	    *out++ = '&';
	    *out++ = 'l';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*in == '>') {
	    if (outend - out < 4) break;
	    *out++ = '&';
	    *out++ = 'g';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*in == '&') {
	    if (outend - out < 5) break;
	    *out++ = '&';
	    *out++ = 'a';
	    *out++ = 'm';
	    *out++ = 'p';
	    *out++ = ';';
	} else if (*in == '\r') {
	    if (outend - out < 5) break;
	    *out++ = '&';
	    *out++ = '#';
	    *out++ = '1';
	    *out++ = '3';
	    *out++ = ';';
	} else {
	    *out++ = (unsigned char) *in;
	}
	++in;
    }
    *outlen = out - outstart;
    *inlen = in - base;
    return(0);
}

/**
 * xmlOutputBufferWriteEscape:
 * @out:  a buffered parser output
 * @str:  a zero terminated UTF-8 string
 * @escaping:  an optional escaping function (or NULL)
 *
 * Write the content of the string in the output I/O buffer
 * This routine escapes the caracters and then handle the I18N
 * transcoding from internal UTF-8
 * The buffer is lossless, i.e. will store in case of partial
 * or delayed writes.
 *
 * Returns the number of chars immediately written, or -1
 *         in case of error.
 */
int
xmlOutputBufferWriteEscape(xmlOutputBufferPtr out, const xmlChar *str,
                           xmlCharEncodingOutputFunc escaping) {
    int nbchars = 0; 
    int ret;         
    int written = 0; /* number of char written to I/O so far */
    int oldwritten=0;
    int chunk;       
    int len;         
    int cons;        

    if ((out == NULL) || (out->error) || (str == NULL) ||
        (out->buffer == NULL) ||
	(out->buffer->alloc == XML_BUFFER_ALLOC_IMMUTABLE)) return(-1);
    len = strlen((const char *)str);
    if (len < 0) return(0);
    if (out->error) return(-1);
    if (escaping == NULL) escaping = xmlEscapeContent;

    do {
        oldwritten = written;

	cons = len;
	chunk = (out->buffer->size - out->buffer->use) - 1;

	if (chunk < 40) {
	    if (xmlBufferGrow(out->buffer, out->buffer->size + 100) < 0)
	        return(-1);
            oldwritten = -1;
	    continue;
	}

	if (out->encoder != NULL) {
	    if (out->conv == NULL) {
		out->conv = xmlBufferCreate();
	    }
	    ret = escaping(out->buffer->content + out->buffer->use ,
	                   &chunk, str, &cons);
	    if ((ret < 0) || (chunk == 0)) 
	        return(-1);
	    out->buffer->use += chunk;
	    out->buffer->content[out->buffer->use] = 0;

	    if ((out->buffer->use < MINLEN) && (cons == len))
		goto done;

	    ret = xmlCharEncOutFunc(out->encoder, out->conv, out->buffer);
	    if ((ret < 0) && (ret != -3)) {
		xmlIOErr(XML_IO_ENCODER, NULL);
		out->error = XML_IO_ENCODER;
		return(-1);
	    }
	    nbchars = out->conv->use;
	} else {
	    ret = escaping(out->buffer->content + out->buffer->use ,
	                   &chunk, str, &cons);
	    if ((ret < 0) || (chunk == 0)) 
	        return(-1);
	    out->buffer->use += chunk;
	    out->buffer->content[out->buffer->use] = 0;
	    nbchars = out->buffer->use;
	}
	str += cons;
	len -= cons;

	if ((nbchars < MINLEN) && (len <= 0))
	    goto done;

	if (out->writecallback) {
	    if (out->encoder != NULL) {
		ret = out->writecallback(out->context,
				 (const char *)out->conv->content, nbchars);
		if (ret >= 0)
		    xmlBufferShrink(out->conv, ret);
	    } else {
		ret = out->writecallback(out->context,
				 (const char *)out->buffer->content, nbchars);
		if (ret >= 0)
		    xmlBufferShrink(out->buffer, ret);
	    }
	    if (ret < 0) {
		xmlIOErr(XML_IO_WRITE, NULL);
		out->error = XML_IO_WRITE;
		return(ret);
	    }
	    out->written += ret;
	} else if (out->buffer->size - out->buffer->use < MINLEN) {
	    xmlBufferResize(out->buffer, out->buffer->size + MINLEN);
	}
	written += nbchars;
    } while ((len > 0) && (oldwritten != written));

done:
#ifdef DEBUG_INPUT
    xmlGenericError(xmlGenericErrorContext,
	    "I/O: wrote %d chars\n", written);
#endif
    return(written);
}

/**
 * xmlOutputBufferWriteString:
 * @out:  a buffered parser output
 * @str:  a zero terminated C string
 *
 * Write the content of the string in the output I/O buffer
 * This routine handle the I18N transcoding from internal UTF-8
 * The buffer is lossless, i.e. will store in case of partial
 * or delayed writes.
 *
 * Returns the number of chars immediately written, or -1
 *         in case of error.
 */
int
xmlOutputBufferWriteString(xmlOutputBufferPtr out, const char *str) {
    int len;

    if ((out == NULL) || (out->error)) return(-1);
    if (str == NULL)
        return(-1);
    len = strlen(str);

    if (len > 0)
	return(xmlOutputBufferWrite(out, len, str));
    return(len);
}

/**
 * xmlOutputBufferFlush:
 * @out:  a buffered output
 *
 * flushes the output I/O channel
 *
 * Returns the number of byte written or -1 in case of error.
 */
int
xmlOutputBufferFlush(xmlOutputBufferPtr out) {
    int nbchars = 0, ret = 0;

    if ((out == NULL) || (out->error)) return(-1);
    if ((out->conv != NULL) && (out->encoder != NULL)) {
	nbchars = xmlCharEncOutFunc(out->encoder, out->conv, out->buffer);
	if (nbchars < 0) {
	    xmlIOErr(XML_IO_ENCODER, NULL);
	    out->error = XML_IO_ENCODER;
	    return(-1);
	}
    }

    if ((out->conv != NULL) && (out->encoder != NULL) &&
	(out->writecallback != NULL)) {
	ret = out->writecallback(out->context,
	           (const char *)out->conv->content, out->conv->use);
	if (ret >= 0)
	    xmlBufferShrink(out->conv, ret);
    } else if (out->writecallback != NULL) {
	ret = out->writecallback(out->context,
	           (const char *)out->buffer->content, out->buffer->use);
	if (ret >= 0)
	    xmlBufferShrink(out->buffer, ret);
    }
    if (ret < 0) {
	xmlIOErr(XML_IO_FLUSH, NULL);
	out->error = XML_IO_FLUSH;
	return(ret);
    }
    out->written += ret;

#ifdef DEBUG_INPUT
    xmlGenericError(xmlGenericErrorContext,
	    "I/O: flushed %d chars\n", ret);
#endif
    return(ret);
}
#endif 

char *
xmlParserGetDirectory(const char *filename) {
    char *ret = NULL;
    char dir[1024];
    char *cur;

#ifdef _WIN32_WCE  
    return NULL;
#endif

    if (xmlInputCallbackInitialized == 0)
	xmlRegisterDefaultInputCallbacks();

    if (filename == NULL) return(NULL);

#if defined(WIN32) && !defined(__CYGWIN__)
#   define IS_XMLPGD_SEP(ch) ((ch=='/')||(ch=='\\'))
#else
#   define IS_XMLPGD_SEP(ch) (ch=='/')
#endif

    strncpy(dir, filename, 1023);
    dir[1023] = 0;
    cur = &dir[strlen(dir)];
    while (cur > dir) {
         if (IS_XMLPGD_SEP(*cur)) break;
	 cur --;
    }
    if (IS_XMLPGD_SEP(*cur)) {
        if (cur == dir) dir[1] = 0;
	else *cur = 0;
	ret = xmlMemStrdup(dir);
    } else {
        if (getcwd(dir, 1024) != NULL) {
	    dir[1023] = 0;
	    ret = xmlMemStrdup(dir);
	}
    }
    return(ret);
#undef IS_XMLPGD_SEP
}


xmlParserInputPtr
xmlCheckHTTPInput(xmlParserCtxtPtr ctxt, xmlParserInputPtr ret) {
#ifdef LIBXML_HTTP_ENABLED
    if ((ret != NULL) && (ret->buf != NULL) &&
        (ret->buf->readcallback == xmlIOHTTPRead) &&
        (ret->buf->context != NULL)) {
        const char *encoding;
        const char *redir;
        const char *mime;
        int code;

        code = xmlNanoHTTPReturnCode(ret->buf->context);
        if (code >= 400) {
            
	    if (ret->filename != NULL)
		__xmlLoaderErr(ctxt, "failed to load HTTP resource \"%s\"\n",
                         (const char *) ret->filename);
	    else
		__xmlLoaderErr(ctxt, "failed to load HTTP resource\n", NULL);
            xmlFreeInputStream(ret);
            ret = NULL;
        } else {

            mime = xmlNanoHTTPMimeType(ret->buf->context);
            if ((xmlStrstr(BAD_CAST mime, BAD_CAST "/xml")) ||
                (xmlStrstr(BAD_CAST mime, BAD_CAST "+xml"))) {
                encoding = xmlNanoHTTPEncoding(ret->buf->context);
                if (encoding != NULL) {
                    xmlCharEncodingHandlerPtr handler;

                    handler = xmlFindCharEncodingHandler(encoding);
                    if (handler != NULL) {
                        xmlSwitchInputEncoding(ctxt, ret, handler);
                    } else {
                        __xmlErrEncoding(ctxt, XML_ERR_UNKNOWN_ENCODING,
                                         "Unknown encoding %s",
                                         BAD_CAST encoding, NULL);
                    }
                    if (ret->encoding == NULL)
                        ret->encoding = xmlStrdup(BAD_CAST encoding);
                }
#if 0
            } else if (xmlStrstr(BAD_CAST mime, BAD_CAST "html")) {
#endif
            }
            redir = xmlNanoHTTPRedir(ret->buf->context);
            if (redir != NULL) {
                if (ret->filename != NULL)
                    xmlFree((xmlChar *) ret->filename);
                if (ret->directory != NULL) {
                    xmlFree((xmlChar *) ret->directory);
                    ret->directory = NULL;
                }
                ret->filename =
                    (char *) xmlStrdup((const xmlChar *) redir);
            }
        }
    }
#endif
    return(ret);
}

static int xmlNoNetExists(const char *URL) {
    const char *path;

    if (URL == NULL)
	return(0);

    if (!xmlStrncasecmp(BAD_CAST URL, BAD_CAST "file://localhost/", 17))
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &URL[17];
#else
	path = &URL[16];
#endif
    else if (!xmlStrncasecmp(BAD_CAST URL, BAD_CAST "file:///", 8)) {
#if defined (_WIN32) || defined (__DJGPP__) && !defined(__CYGWIN__)
	path = &URL[8];
#else
	path = &URL[7];
#endif
    } else
	path = URL;

    return xmlCheckFilename(path);
}

#ifdef LIBXML_CATALOG_ENABLED

static xmlChar *
xmlResolveResourceFromCatalog(const char *URL, const char *ID,
                              xmlParserCtxtPtr ctxt) {
    xmlChar *resource = NULL;
    xmlCatalogAllow pref;

    pref = xmlCatalogGetDefaults();

    if ((pref != XML_CATA_ALLOW_NONE) && (!xmlNoNetExists(URL))) {
	if ((ctxt != NULL) && (ctxt->catalogs != NULL) &&
	    ((pref == XML_CATA_ALLOW_ALL) ||
	     (pref == XML_CATA_ALLOW_DOCUMENT))) {
	    resource = xmlCatalogLocalResolve(ctxt->catalogs,
					      (const xmlChar *)ID,
					      (const xmlChar *)URL);
        }
	if ((resource == NULL) &&
	    ((pref == XML_CATA_ALLOW_ALL) ||
	     (pref == XML_CATA_ALLOW_GLOBAL))) {
	    resource = xmlCatalogResolve((const xmlChar *)ID,
					 (const xmlChar *)URL);
	}
	if ((resource == NULL) && (URL != NULL))
	    resource = xmlStrdup((const xmlChar *) URL);

	if ((resource != NULL) && (!xmlNoNetExists((const char *)resource))) {
	    xmlChar *tmp = NULL;

	    if ((ctxt != NULL) && (ctxt->catalogs != NULL) &&
		((pref == XML_CATA_ALLOW_ALL) ||
		 (pref == XML_CATA_ALLOW_DOCUMENT))) {
		tmp = xmlCatalogLocalResolveURI(ctxt->catalogs, resource);
	    }
	    if ((tmp == NULL) &&
		((pref == XML_CATA_ALLOW_ALL) ||
	         (pref == XML_CATA_ALLOW_GLOBAL))) {
		tmp = xmlCatalogResolveURI(resource);
	    }

	    if (tmp != NULL) {
		xmlFree(resource);
		resource = tmp;
	    }
	}
    }

    return resource;
}

#endif

static xmlParserInputPtr
xmlDefaultExternalEntityLoader(const char *URL, const char *ID,
                               xmlParserCtxtPtr ctxt)
{
    xmlParserInputPtr ret = NULL;
    xmlChar *resource = NULL;

#ifdef DEBUG_EXTERNAL_ENTITIES
    xmlGenericError(xmlGenericErrorContext,
                    "xmlDefaultExternalEntityLoader(%s, xxx)\n", URL);
#endif
    if ((ctxt != NULL) && (ctxt->options & XML_PARSE_NONET)) {
        int options = ctxt->options;

	ctxt->options -= XML_PARSE_NONET;
        ret = xmlNoNetExternalEntityLoader(URL, ID, ctxt);
	ctxt->options = options;
	return(ret);
    }
#ifdef LIBXML_CATALOG_ENABLED
    resource = xmlResolveResourceFromCatalog(URL, ID, ctxt);
#endif

    if (resource == NULL)
        resource = (xmlChar *) URL;

    if (resource == NULL) {
        if (ID == NULL)
            ID = "NULL";
        __xmlLoaderErr(ctxt, "failed to load external entity \"%s\"\n", ID);
        return (NULL);
    }
    ret = xmlNewInputFromFile(ctxt, (const char *) resource);
    if ((resource != NULL) && (resource != (xmlChar *) URL))
        xmlFree(resource);
    return (ret);
}

static xmlExternalEntityLoader xmlCurrentExternalEntityLoader =
       xmlDefaultExternalEntityLoader;

void
xmlSetExternalEntityLoader(xmlExternalEntityLoader f) {
    xmlCurrentExternalEntityLoader = f;
}

xmlExternalEntityLoader
xmlGetExternalEntityLoader(void) {
    return(xmlCurrentExternalEntityLoader);
}

xmlParserInputPtr
xmlLoadExternalEntity(const char *URL, const char *ID,
                      xmlParserCtxtPtr ctxt) {
    if ((URL != NULL) && (xmlNoNetExists(URL) == 0)) {
	char *canonicFilename;
	xmlParserInputPtr ret;

	canonicFilename = (char *) xmlCanonicPath((const xmlChar *) URL);
	if (canonicFilename == NULL) {
            xmlIOErrMemory("building canonical path\n");
	    return(NULL);
	}

	ret = xmlCurrentExternalEntityLoader(canonicFilename, ID, ctxt);
	xmlFree(canonicFilename);
	return(ret);
    }
    return(xmlCurrentExternalEntityLoader(URL, ID, ctxt));
}


xmlParserInputPtr
xmlNoNetExternalEntityLoader(const char *URL, const char *ID,
                             xmlParserCtxtPtr ctxt) {
    xmlParserInputPtr input = NULL;
    xmlChar *resource = NULL;

#ifdef LIBXML_CATALOG_ENABLED
    resource = xmlResolveResourceFromCatalog(URL, ID, ctxt);
#endif

    if (resource == NULL)
	resource = (xmlChar *) URL;

    if (resource != NULL) {
        if ((!xmlStrncasecmp(BAD_CAST resource, BAD_CAST "ftp://", 6)) ||
            (!xmlStrncasecmp(BAD_CAST resource, BAD_CAST "http://", 7))) {
            xmlIOErr(XML_IO_NETWORK_ATTEMPT, (const char *) resource);
	    if (resource != (xmlChar *) URL)
		xmlFree(resource);
	    return(NULL);
	}
    }
    input = xmlDefaultExternalEntityLoader((const char *) resource, ID, ctxt);
    if (resource != (xmlChar *) URL)
	xmlFree(resource);
    return(input);
}

#define bottom_xmlIO
#include "elfgcchack.h"
