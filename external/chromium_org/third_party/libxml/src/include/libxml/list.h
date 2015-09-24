/*
 * Summary: lists interfaces
 * Description: this module implement the list support used in 
 * various place in the library.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Gary Pennington <Gary.Pennington@uk.sun.com>
 */

#ifndef __XML_LINK_INCLUDE__
#define __XML_LINK_INCLUDE__

#include <libxml/xmlversion.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xmlLink xmlLink;
typedef xmlLink *xmlLinkPtr;

typedef struct _xmlList xmlList;
typedef xmlList *xmlListPtr;

typedef void (*xmlListDeallocator) (xmlLinkPtr lk);
typedef int  (*xmlListDataCompare) (const void *data0, const void *data1);
typedef int (*xmlListWalker) (const void *data, const void *user);

XMLPUBFUN xmlListPtr XMLCALL
		xmlListCreate		(xmlListDeallocator deallocator,
	                                 xmlListDataCompare compare);
XMLPUBFUN void XMLCALL		
		xmlListDelete		(xmlListPtr l);

XMLPUBFUN void * XMLCALL		
		xmlListSearch		(xmlListPtr l,
					 void *data);
XMLPUBFUN void * XMLCALL		
		xmlListReverseSearch	(xmlListPtr l,
					 void *data);
XMLPUBFUN int XMLCALL		
		xmlListInsert		(xmlListPtr l,
					 void *data) ;
XMLPUBFUN int XMLCALL		
		xmlListAppend		(xmlListPtr l,
					 void *data) ;
XMLPUBFUN int XMLCALL		
		xmlListRemoveFirst	(xmlListPtr l,
					 void *data);
XMLPUBFUN int XMLCALL		
		xmlListRemoveLast	(xmlListPtr l,
					 void *data);
XMLPUBFUN int XMLCALL		
		xmlListRemoveAll	(xmlListPtr l,
					 void *data);
XMLPUBFUN void XMLCALL		
		xmlListClear		(xmlListPtr l);
XMLPUBFUN int XMLCALL		
		xmlListEmpty		(xmlListPtr l);
XMLPUBFUN xmlLinkPtr XMLCALL	
		xmlListFront		(xmlListPtr l);
XMLPUBFUN xmlLinkPtr XMLCALL	
		xmlListEnd		(xmlListPtr l);
XMLPUBFUN int XMLCALL		
		xmlListSize		(xmlListPtr l);

XMLPUBFUN void XMLCALL		
		xmlListPopFront		(xmlListPtr l);
XMLPUBFUN void XMLCALL		
		xmlListPopBack		(xmlListPtr l);
XMLPUBFUN int XMLCALL		
		xmlListPushFront	(xmlListPtr l,
					 void *data);
XMLPUBFUN int XMLCALL		
		xmlListPushBack		(xmlListPtr l,
					 void *data);

XMLPUBFUN void XMLCALL		
		xmlListReverse		(xmlListPtr l);
XMLPUBFUN void XMLCALL		
		xmlListSort		(xmlListPtr l);
XMLPUBFUN void XMLCALL		
		xmlListWalk		(xmlListPtr l,
					 xmlListWalker walker,
					 const void *user);
XMLPUBFUN void XMLCALL		
		xmlListReverseWalk	(xmlListPtr l,
					 xmlListWalker walker,
					 const void *user);
XMLPUBFUN void XMLCALL		
		xmlListMerge		(xmlListPtr l1,
					 xmlListPtr l2);
XMLPUBFUN xmlListPtr XMLCALL	
		xmlListDup		(const xmlListPtr old);
XMLPUBFUN int XMLCALL		
		xmlListCopy		(xmlListPtr cur,
					 const xmlListPtr old);
XMLPUBFUN void * XMLCALL          
		xmlLinkGetData          (xmlLinkPtr lk);


#ifdef __cplusplus
}
#endif

#endif 
