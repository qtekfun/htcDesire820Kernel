/*
 * Summary: unfinished XLink detection module
 * Description: unfinished XLink detection module
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XLINK_H__
#define __XML_XLINK_H__

#include <libxml/xmlversion.h>
#include <libxml/tree.h>

#ifdef LIBXML_XPTR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

typedef xmlChar *xlinkHRef;
typedef xmlChar *xlinkRole;
typedef xmlChar *xlinkTitle;

typedef enum {
    XLINK_TYPE_NONE = 0,
    XLINK_TYPE_SIMPLE,
    XLINK_TYPE_EXTENDED,
    XLINK_TYPE_EXTENDED_SET
} xlinkType;

typedef enum {
    XLINK_SHOW_NONE = 0,
    XLINK_SHOW_NEW,
    XLINK_SHOW_EMBED,
    XLINK_SHOW_REPLACE
} xlinkShow;

typedef enum {
    XLINK_ACTUATE_NONE = 0,
    XLINK_ACTUATE_AUTO,
    XLINK_ACTUATE_ONREQUEST
} xlinkActuate;

typedef void (*xlinkNodeDetectFunc) (void *ctx, xmlNodePtr node);


typedef void
(*xlinkSimpleLinkFunk)	(void *ctx,
			 xmlNodePtr node,
			 const xlinkHRef href,
			 const xlinkRole role,
			 const xlinkTitle title);

typedef void
(*xlinkExtendedLinkFunk)(void *ctx,
			 xmlNodePtr node,
			 int nbLocators,
			 const xlinkHRef *hrefs,
			 const xlinkRole *roles,
			 int nbArcs,
			 const xlinkRole *from,
			 const xlinkRole *to,
			 xlinkShow *show,
			 xlinkActuate *actuate,
			 int nbTitles,
			 const xlinkTitle *titles,
			 const xmlChar **langs);

typedef void
(*xlinkExtendedLinkSetFunk)	(void *ctx,
				 xmlNodePtr node,
				 int nbLocators,
				 const xlinkHRef *hrefs,
				 const xlinkRole *roles,
				 int nbTitles,
				 const xlinkTitle *titles,
				 const xmlChar **langs);

typedef struct _xlinkHandler xlinkHandler;
typedef xlinkHandler *xlinkHandlerPtr;
struct _xlinkHandler {
    xlinkSimpleLinkFunk simple;
    xlinkExtendedLinkFunk extended;
    xlinkExtendedLinkSetFunk set;
};


XMLPUBFUN xlinkNodeDetectFunc XMLCALL	
		xlinkGetDefaultDetect	(void);
XMLPUBFUN void XMLCALL			
		xlinkSetDefaultDetect	(xlinkNodeDetectFunc func);

XMLPUBFUN xlinkHandlerPtr XMLCALL	
		xlinkGetDefaultHandler	(void);
XMLPUBFUN void XMLCALL		
		xlinkSetDefaultHandler	(xlinkHandlerPtr handler);

XMLPUBFUN xlinkType XMLCALL	 
		xlinkIsLink		(xmlDocPtr doc,
					 xmlNodePtr node);

#ifdef __cplusplus
}
#endif

#endif 

#endif 
