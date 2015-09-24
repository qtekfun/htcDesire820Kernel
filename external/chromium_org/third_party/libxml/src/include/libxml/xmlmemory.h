/*
 * Summary: interface for the memory allocator
 * Description: provides interfaces for the memory allocator,
 *              including debugging capabilities.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */


#ifndef __DEBUG_MEMORY_ALLOC__
#define __DEBUG_MEMORY_ALLOC__

#include <stdio.h>
#include <libxml/xmlversion.h>


#ifdef DEBUG
#ifndef DEBUG_MEMORY
#define DEBUG_MEMORY
#endif
#endif

#ifdef DEBUG_MEMORY_LOCATION
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (XMLCALL *xmlFreeFunc)(void *mem);
typedef void *(LIBXML_ATTR_ALLOC_SIZE(1) XMLCALL *xmlMallocFunc)(size_t size);

typedef void *(XMLCALL *xmlReallocFunc)(void *mem, size_t size);

typedef char *(XMLCALL *xmlStrdupFunc)(const char *str);


XMLPUBFUN int XMLCALL
	xmlMemSetup	(xmlFreeFunc freeFunc,
			 xmlMallocFunc mallocFunc,
			 xmlReallocFunc reallocFunc,
			 xmlStrdupFunc strdupFunc);
XMLPUBFUN int XMLCALL
	xmlMemGet	(xmlFreeFunc *freeFunc,
			 xmlMallocFunc *mallocFunc,
			 xmlReallocFunc *reallocFunc,
			 xmlStrdupFunc *strdupFunc);
XMLPUBFUN int XMLCALL
	xmlGcMemSetup	(xmlFreeFunc freeFunc,
			 xmlMallocFunc mallocFunc,
			 xmlMallocFunc mallocAtomicFunc,
			 xmlReallocFunc reallocFunc,
			 xmlStrdupFunc strdupFunc);
XMLPUBFUN int XMLCALL
	xmlGcMemGet	(xmlFreeFunc *freeFunc,
			 xmlMallocFunc *mallocFunc,
			 xmlMallocFunc *mallocAtomicFunc,
			 xmlReallocFunc *reallocFunc,
			 xmlStrdupFunc *strdupFunc);

XMLPUBFUN int XMLCALL
	xmlInitMemory	(void);

XMLPUBFUN void XMLCALL
                xmlCleanupMemory        (void);
XMLPUBFUN int XMLCALL
	xmlMemUsed	(void);
XMLPUBFUN int XMLCALL
	xmlMemBlocks	(void);
XMLPUBFUN void XMLCALL
	xmlMemDisplay	(FILE *fp);
XMLPUBFUN void XMLCALL
	xmlMemDisplayLast(FILE *fp, long nbBytes);
XMLPUBFUN void XMLCALL
	xmlMemShow	(FILE *fp, int nr);
XMLPUBFUN void XMLCALL
	xmlMemoryDump	(void);
XMLPUBFUN void * XMLCALL
	xmlMemMalloc	(size_t size) LIBXML_ATTR_ALLOC_SIZE(1);
XMLPUBFUN void * XMLCALL
	xmlMemRealloc	(void *ptr,size_t size);
XMLPUBFUN void XMLCALL
	xmlMemFree	(void *ptr);
XMLPUBFUN char * XMLCALL
	xmlMemoryStrdup	(const char *str);
XMLPUBFUN void * XMLCALL
	xmlMallocLoc	(size_t size, const char *file, int line) LIBXML_ATTR_ALLOC_SIZE(1);
XMLPUBFUN void * XMLCALL
	xmlReallocLoc	(void *ptr, size_t size, const char *file, int line);
XMLPUBFUN void * XMLCALL
	xmlMallocAtomicLoc (size_t size, const char *file, int line) LIBXML_ATTR_ALLOC_SIZE(1);
XMLPUBFUN char * XMLCALL
	xmlMemStrdupLoc	(const char *str, const char *file, int line);


#ifdef DEBUG_MEMORY_LOCATION
#define xmlMalloc(size) xmlMallocLoc((size), __FILE__, __LINE__)
#define xmlMallocAtomic(size) xmlMallocAtomicLoc((size), __FILE__, __LINE__)
#define xmlRealloc(ptr, size) xmlReallocLoc((ptr), (size), __FILE__, __LINE__)
#define xmlMemStrdup(str) xmlMemStrdupLoc((str), __FILE__, __LINE__)

#endif 

#ifdef __cplusplus
}
#endif 

#ifndef __XML_GLOBALS_H
#ifndef __XML_THREADS_H__
#include <libxml/threads.h>
#include <libxml/globals.h>
#endif
#endif

#endif  

