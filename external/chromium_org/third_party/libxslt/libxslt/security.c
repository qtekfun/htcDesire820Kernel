/*
 * security.c: Implementation of the XSLT security framework
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXSLT
#include "libxslt.h"

#include <string.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_MATH_H
#include <math.h>
#endif
#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
#ifdef HAVE_NAN_H
#include <nan.h>
#endif
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#if defined(WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif
#endif

#ifndef HAVE_STAT
#  ifdef HAVE__STAT
#    ifndef _MSC_VER
#      define stat(x,y) _stat(x,y)
#    endif
#    define HAVE_STAT
#  endif
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/uri.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "extensions.h"
#include "security.h"


struct _xsltSecurityPrefs {
    xsltSecurityCheck readFile;
    xsltSecurityCheck createFile;
    xsltSecurityCheck createDir;
    xsltSecurityCheck readNet;
    xsltSecurityCheck writeNet;
};

static xsltSecurityPrefsPtr xsltDefaultSecurityPrefs = NULL;


xsltSecurityPrefsPtr
xsltNewSecurityPrefs(void) {
    xsltSecurityPrefsPtr ret;

    xsltInitGlobals();

    ret = (xsltSecurityPrefsPtr) xmlMalloc(sizeof(xsltSecurityPrefs));
    if (ret == NULL) {
	xsltTransformError(NULL, NULL, NULL,
		"xsltNewSecurityPrefs : malloc failed\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xsltSecurityPrefs));
    return(ret);
}

void
xsltFreeSecurityPrefs(xsltSecurityPrefsPtr sec) {
    if (sec == NULL)
	return;
    xmlFree(sec);
}

int
xsltSetSecurityPrefs(xsltSecurityPrefsPtr sec, xsltSecurityOption option,
                     xsltSecurityCheck func) {
    xsltInitGlobals();
    if (sec == NULL)
	return(-1);
    switch (option) {
        case XSLT_SECPREF_READ_FILE:
            sec->readFile = func; return(0);
        case XSLT_SECPREF_WRITE_FILE:
            sec->createFile = func; return(0);
        case XSLT_SECPREF_CREATE_DIRECTORY:
            sec->createDir = func; return(0);
        case XSLT_SECPREF_READ_NETWORK:
            sec->readNet = func; return(0);
        case XSLT_SECPREF_WRITE_NETWORK:
            sec->writeNet = func; return(0);
    }
    return(-1);
}

xsltSecurityCheck
xsltGetSecurityPrefs(xsltSecurityPrefsPtr sec, xsltSecurityOption option) {
    if (sec == NULL)
	return(NULL);
    switch (option) {
        case XSLT_SECPREF_READ_FILE:
            return(sec->readFile);
        case XSLT_SECPREF_WRITE_FILE:
            return(sec->createFile);
        case XSLT_SECPREF_CREATE_DIRECTORY:
            return(sec->createDir);
        case XSLT_SECPREF_READ_NETWORK:
            return(sec->readNet);
        case XSLT_SECPREF_WRITE_NETWORK:
            return(sec->writeNet);
    }
    return(NULL);
}

void
xsltSetDefaultSecurityPrefs(xsltSecurityPrefsPtr sec) {
    
    xsltDefaultSecurityPrefs = sec;
}

xsltSecurityPrefsPtr
xsltGetDefaultSecurityPrefs(void) {
    return(xsltDefaultSecurityPrefs);
}

int                    
xsltSetCtxtSecurityPrefs(xsltSecurityPrefsPtr sec,
	                 xsltTransformContextPtr ctxt) {
    if (ctxt == NULL)
	return(-1);
    ctxt->sec = (void *) sec;
    return(0);
}


int
xsltSecurityAllow(xsltSecurityPrefsPtr sec ATTRIBUTE_UNUSED,
	          xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		  const char *value ATTRIBUTE_UNUSED) {
    return(1);
}

int
xsltSecurityForbid(xsltSecurityPrefsPtr sec ATTRIBUTE_UNUSED,
	          xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED,
		  const char *value ATTRIBUTE_UNUSED) {
    return(0);
}



static int
xsltCheckFilename (const char *path)
{
#ifdef HAVE_STAT
    struct stat stat_buffer;
#if defined(WIN32) && !defined(__CYGWIN__)
    DWORD dwAttrs;

    dwAttrs = GetFileAttributesA(path); 
    if (dwAttrs != INVALID_FILE_ATTRIBUTES) {
        if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY) {
            return 2;
		}
    }
#endif

    if (stat(path, &stat_buffer) == -1)
        return 0;

#ifdef S_ISDIR
    if (S_ISDIR(stat_buffer.st_mode)) {
        return 2;
    }
#endif
#endif
    return 1;
}

static int
xsltCheckWritePath(xsltSecurityPrefsPtr sec,
		   xsltTransformContextPtr ctxt,
		   const char *path)
{
    int ret;
    xsltSecurityCheck check;
    char *directory;

    check = xsltGetSecurityPrefs(sec, XSLT_SECPREF_WRITE_FILE);
    if (check != NULL) {
	ret = check(sec, ctxt, path);
	if (ret == 0) {
	    xsltTransformError(ctxt, NULL, NULL,
			       "File write for %s refused\n", path);
	    return(0);
	}
    }

    directory = xmlParserGetDirectory (path);

    if (directory != NULL) {
	ret = xsltCheckFilename(directory);
	if (ret == 0) {
	    check = xsltGetSecurityPrefs(sec,
					 XSLT_SECPREF_CREATE_DIRECTORY);
	    if (check != NULL) {
		ret = check(sec, ctxt, directory);
		if (ret == 0) {
		    xsltTransformError(ctxt, NULL, NULL,
				       "Directory creation for %s refused\n",
				       path);
		    xmlFree(directory);
		    return(0);
		}
	    }
	    ret = xsltCheckWritePath(sec, ctxt, directory);
	    if (ret == 1)
		ret = mkdir(directory, 0755);
	}
	xmlFree(directory);
	if (ret < 0)
	    return(ret);
    }

    return(1);
}

/**
 * xsltCheckWrite:
 * @sec:  the security options
 * @ctxt:  an XSLT transformation context
 * @URL:  the resource to be written
 *
 * Check if the resource is allowed to be written, if necessary makes
 * some preliminary work like creating directories
 *
 * Return 1 if write is allowed, 0 if not and -1 in case or error.
 */
int
xsltCheckWrite(xsltSecurityPrefsPtr sec,
	       xsltTransformContextPtr ctxt, const xmlChar *URL) {
    int ret;
    xmlURIPtr uri;
    xsltSecurityCheck check;

    uri = xmlParseURI((const char *)URL);
    if (uri == NULL) {
        uri = xmlCreateURI();
	if (uri == NULL) {
	    xsltTransformError(ctxt, NULL, NULL,
	     "xsltCheckWrite: out of memory for %s\n", URL);
	    return(-1);
	}
	uri->path = (char *)xmlStrdup(URL);
    }
    if ((uri->scheme == NULL) ||
	(xmlStrEqual(BAD_CAST uri->scheme, BAD_CAST "file"))) {

#if defined(WIN32) && !defined(__CYGWIN__)
    if ((uri->path)&&(uri->path[0]=='/')&&
        (uri->path[1]!='\0')&&(uri->path[2]==':'))
    ret = xsltCheckWritePath(sec, ctxt, uri->path+1);
    else
#endif

	ret = xsltCheckWritePath(sec, ctxt, uri->path);
	if (ret <= 0) {
	    xmlFreeURI(uri);
	    return(ret);
	}
    } else {
	check = xsltGetSecurityPrefs(sec, XSLT_SECPREF_WRITE_NETWORK);
	if (check != NULL) {
	    ret = check(sec, ctxt, (const char *)URL);
	    if (ret == 0) {
		xsltTransformError(ctxt, NULL, NULL,
			     "File write for %s refused\n", URL);
		xmlFreeURI(uri);
		return(0);
	    }
	}
    }
    xmlFreeURI(uri);
    return(1);
}


int
xsltCheckRead(xsltSecurityPrefsPtr sec,
	      xsltTransformContextPtr ctxt, const xmlChar *URL) {
    int ret;
    xmlURIPtr uri;
    xsltSecurityCheck check;

    uri = xmlParseURI((const char *)URL);
    if (uri == NULL) {
	xsltTransformError(ctxt, NULL, NULL,
	 "xsltCheckRead: URL parsing failed for %s\n",
			 URL);
	return(-1);
    }
    if ((uri->scheme == NULL) ||
	(xmlStrEqual(BAD_CAST uri->scheme, BAD_CAST "file"))) {

	check = xsltGetSecurityPrefs(sec, XSLT_SECPREF_READ_FILE);
	if (check != NULL) {
	    ret = check(sec, ctxt, uri->path);
	    if (ret == 0) {
		xsltTransformError(ctxt, NULL, NULL,
			     "Local file read for %s refused\n", URL);
		xmlFreeURI(uri);
		return(0);
	    }
	}
    } else {
	check = xsltGetSecurityPrefs(sec, XSLT_SECPREF_READ_NETWORK);
	if (check != NULL) {
	    ret = check(sec, ctxt, (const char *)URL);
	    if (ret == 0) {
		xsltTransformError(ctxt, NULL, NULL,
			     "Network file read for %s refused\n", URL);
		xmlFreeURI(uri);
		return(0);
	    }
	}
    }
    xmlFreeURI(uri);
    return(1);
}

