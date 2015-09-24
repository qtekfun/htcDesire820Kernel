/*
 * Summary: Chained hash tables
 * Description: This module implements the hash table support used in 
 * 		various places in the library.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Bjorn Reese <bjorn.reese@systematic.dk>
 */

#ifndef __XML_HASH_H__
#define __XML_HASH_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xmlHashTable xmlHashTable;
typedef xmlHashTable *xmlHashTablePtr;

#ifdef __cplusplus
}
#endif

#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/dict.h>

#ifdef __cplusplus
extern "C" {
#endif


#define XML_CAST_FPTR(fptr) fptr


typedef void (*xmlHashDeallocator)(void *payload, xmlChar *name);
typedef void *(*xmlHashCopier)(void *payload, xmlChar *name);
typedef void (*xmlHashScanner)(void *payload, void *data, xmlChar *name);
typedef void (*xmlHashScannerFull)(void *payload, void *data,
				   const xmlChar *name, const xmlChar *name2,
				   const xmlChar *name3);

XMLPUBFUN xmlHashTablePtr XMLCALL
			xmlHashCreate	(int size);
XMLPUBFUN xmlHashTablePtr XMLCALL
			xmlHashCreateDict(int size,
					 xmlDictPtr dict);
XMLPUBFUN void XMLCALL			
			xmlHashFree	(xmlHashTablePtr table,
					 xmlHashDeallocator f);

XMLPUBFUN int XMLCALL			
			xmlHashAddEntry	(xmlHashTablePtr table,
		                         const xmlChar *name,
		                         void *userdata);
XMLPUBFUN int XMLCALL			
			xmlHashUpdateEntry(xmlHashTablePtr table,
		                         const xmlChar *name,
		                         void *userdata,
					 xmlHashDeallocator f);
XMLPUBFUN int XMLCALL		    
			xmlHashAddEntry2(xmlHashTablePtr table,
		                         const xmlChar *name,
		                         const xmlChar *name2,
		                         void *userdata);
XMLPUBFUN int XMLCALL			
			xmlHashUpdateEntry2(xmlHashTablePtr table,
		                         const xmlChar *name,
		                         const xmlChar *name2,
		                         void *userdata,
					 xmlHashDeallocator f);
XMLPUBFUN int XMLCALL			
			xmlHashAddEntry3(xmlHashTablePtr table,
		                         const xmlChar *name,
		                         const xmlChar *name2,
		                         const xmlChar *name3,
		                         void *userdata);
XMLPUBFUN int XMLCALL			
			xmlHashUpdateEntry3(xmlHashTablePtr table,
		                         const xmlChar *name,
		                         const xmlChar *name2,
		                         const xmlChar *name3,
		                         void *userdata,
					 xmlHashDeallocator f);

XMLPUBFUN int XMLCALL     
			xmlHashRemoveEntry(xmlHashTablePtr table, const xmlChar *name,
                           xmlHashDeallocator f);
XMLPUBFUN int XMLCALL     
			xmlHashRemoveEntry2(xmlHashTablePtr table, const xmlChar *name,
                            const xmlChar *name2, xmlHashDeallocator f);
XMLPUBFUN int  XMLCALL    
			xmlHashRemoveEntry3(xmlHashTablePtr table, const xmlChar *name,
                            const xmlChar *name2, const xmlChar *name3,
                            xmlHashDeallocator f);

XMLPUBFUN void * XMLCALL			
			xmlHashLookup	(xmlHashTablePtr table,
					 const xmlChar *name);
XMLPUBFUN void * XMLCALL			
			xmlHashLookup2	(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *name2);
XMLPUBFUN void * XMLCALL			
			xmlHashLookup3	(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *name2,
					 const xmlChar *name3);
XMLPUBFUN void * XMLCALL			
			xmlHashQLookup	(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *prefix);
XMLPUBFUN void * XMLCALL			
			xmlHashQLookup2	(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *prefix,
					 const xmlChar *name2,
					 const xmlChar *prefix2);
XMLPUBFUN void * XMLCALL			
			xmlHashQLookup3	(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *prefix,
					 const xmlChar *name2,
					 const xmlChar *prefix2,
					 const xmlChar *name3,
					 const xmlChar *prefix3);

XMLPUBFUN xmlHashTablePtr XMLCALL		
			xmlHashCopy	(xmlHashTablePtr table,
					 xmlHashCopier f);
XMLPUBFUN int XMLCALL			
			xmlHashSize	(xmlHashTablePtr table);
XMLPUBFUN void XMLCALL			
			xmlHashScan	(xmlHashTablePtr table,
					 xmlHashScanner f,
					 void *data);
XMLPUBFUN void XMLCALL			
			xmlHashScan3	(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *name2,
					 const xmlChar *name3,
					 xmlHashScanner f,
					 void *data);
XMLPUBFUN void XMLCALL			
			xmlHashScanFull	(xmlHashTablePtr table,
					 xmlHashScannerFull f,
					 void *data);
XMLPUBFUN void XMLCALL			
			xmlHashScanFull3(xmlHashTablePtr table,
					 const xmlChar *name,
					 const xmlChar *name2,
					 const xmlChar *name3,
					 xmlHashScannerFull f,
					 void *data);
#ifdef __cplusplus
}
#endif
#endif 
