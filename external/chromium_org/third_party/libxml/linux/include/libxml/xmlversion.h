/*
 * Summary: compile-time version informations
 * Description: compile-time version informations for the XML library
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_VERSION_H__
#define __XML_VERSION_H__

#include <libxml/xmlexports.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBXML2_COMPILING_MSCCDEF
XMLPUBFUN void XMLCALL xmlCheckVersion(int version);
#endif 

#define LIBXML_DOTTED_VERSION "2.7.7"

#define LIBXML_VERSION 20707

#define LIBXML_VERSION_STRING "20707"

#define LIBXML_VERSION_EXTRA ""

#define LIBXML_TEST_VERSION xmlCheckVersion(20707);

#ifndef VMS
#if 0
#define WITH_TRIO
#else
#define WITHOUT_TRIO
#endif
#else 
#define WITH_TRIO 1
#endif 

#if 1
#if defined(_REENTRANT) || defined(__MT__) || \
    (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE - 0 >= 199506L))
#define LIBXML_THREAD_ENABLED
#endif
#endif

#if 1
#define LIBXML_TREE_ENABLED
#endif

#if 1
#define LIBXML_OUTPUT_ENABLED
#endif

#if 1
#define LIBXML_PUSH_ENABLED
#endif

#if 1
#define LIBXML_READER_ENABLED
#endif

#if 1
#define LIBXML_PATTERN_ENABLED
#endif

#if 1
#define LIBXML_WRITER_ENABLED
#endif

#if 1
#define LIBXML_SAX1_ENABLED
#endif

#if 0
#define LIBXML_FTP_ENABLED
#endif

#if 0
#define LIBXML_HTTP_ENABLED
#endif

#if 1
#define LIBXML_VALID_ENABLED
#endif

#if 1
#define LIBXML_HTML_ENABLED
#endif

#if 1
#define LIBXML_LEGACY_ENABLED
#endif

#if 1
#define LIBXML_C14N_ENABLED
#endif

#if 1
#define LIBXML_CATALOG_ENABLED
#endif

#if 1
#define LIBXML_DOCB_ENABLED
#endif

#if 1
#define LIBXML_XPATH_ENABLED
#endif

#if 1
#define LIBXML_XPTR_ENABLED
#endif

#if 1
#define LIBXML_XINCLUDE_ENABLED
#endif

#if 0
#define LIBXML_ICONV_ENABLED
#endif

#if 1
#define LIBXML_ICU_ENABLED
#endif

#if 1
#define LIBXML_ISO8859X_ENABLED
#endif

#if 1
#define LIBXML_DEBUG_ENABLED
#endif

#if 0
#define DEBUG_MEMORY_LOCATION
#endif

#if 0
#define LIBXML_DEBUG_RUNTIME
#endif

#if 1
#define LIBXML_UNICODE_ENABLED
#endif

#if 1
#define LIBXML_REGEXP_ENABLED
#endif

#if 1
#define LIBXML_AUTOMATA_ENABLED
#endif

#if 1
#define LIBXML_EXPR_ENABLED
#endif

#if 1
#define LIBXML_SCHEMAS_ENABLED
#endif

#if 1
#define LIBXML_SCHEMATRON_ENABLED
#endif

#if 1
#define LIBXML_MODULES_ENABLED
#define LIBXML_MODULE_EXTENSION ".so" 
#endif

#if 1
#define LIBXML_ZLIB_ENABLED
#endif

#ifdef __GNUC__
#ifdef HAVE_ANSIDECL_H
#include <ansidecl.h>
#endif


#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((unused))
#endif


#ifndef LIBXML_ATTR_ALLOC_SIZE
# if ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)))
#  define LIBXML_ATTR_ALLOC_SIZE(x) __attribute__((alloc_size(x)))
# else
#  define LIBXML_ATTR_ALLOC_SIZE(x)
# endif
#else
# define LIBXML_ATTR_ALLOC_SIZE(x)
#endif


#ifndef LIBXML_ATTR_FORMAT
# if ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)))
#  define LIBXML_ATTR_FORMAT(fmt,args) __attribute__((__format__(__printf__,fmt,args)))
# else
#  define LIBXML_ATTR_FORMAT(fmt,args)
# endif
#else
# define LIBXML_ATTR_FORMAT(fmt,args)
#endif

#else 
#define ATTRIBUTE_UNUSED
#define LIBXML_ATTR_ALLOC_SIZE(x)
#define LIBXML_ATTR_FORMAT(fmt,args)
#endif 

#ifdef __cplusplus
}
#endif 
#endif


