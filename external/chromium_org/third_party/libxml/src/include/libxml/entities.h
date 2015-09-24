/*
 * Summary: interface for the XML entities handling
 * Description: this module provides some of the entity API needed
 *              for the parser and applications.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_ENTITIES_H__
#define __XML_ENTITIES_H__

#include <libxml/xmlversion.h>
#include <libxml/tree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    XML_INTERNAL_GENERAL_ENTITY = 1,
    XML_EXTERNAL_GENERAL_PARSED_ENTITY = 2,
    XML_EXTERNAL_GENERAL_UNPARSED_ENTITY = 3,
    XML_INTERNAL_PARAMETER_ENTITY = 4,
    XML_EXTERNAL_PARAMETER_ENTITY = 5,
    XML_INTERNAL_PREDEFINED_ENTITY = 6
} xmlEntityType;


struct _xmlEntity {
    void           *_private;	        
    xmlElementType          type;       
    const xmlChar          *name;	
    struct _xmlNode    *children;	
    struct _xmlNode        *last;	
    struct _xmlDtd       *parent;	
    struct _xmlNode        *next;	
    struct _xmlNode        *prev;	
    struct _xmlDoc          *doc;       

    xmlChar                *orig;	
    xmlChar             *content;	
    int                   length;	
    xmlEntityType          etype;	
    const xmlChar    *ExternalID;	
    const xmlChar      *SystemID;	

    struct _xmlEntity     *nexte;	
    const xmlChar           *URI;	
    int                    owner;	
    int			 checked;	
};


typedef struct _xmlHashTable xmlEntitiesTable;
typedef xmlEntitiesTable *xmlEntitiesTablePtr;


#ifdef LIBXML_LEGACY_ENABLED
XMLPUBFUN void XMLCALL
		xmlInitializePredefinedEntities	(void);
#endif 

XMLPUBFUN xmlEntityPtr XMLCALL
			xmlNewEntity		(xmlDocPtr doc,
						 const xmlChar *name,
						 int type,
						 const xmlChar *ExternalID,
						 const xmlChar *SystemID,
						 const xmlChar *content);
XMLPUBFUN xmlEntityPtr XMLCALL
			xmlAddDocEntity		(xmlDocPtr doc,
						 const xmlChar *name,
						 int type,
						 const xmlChar *ExternalID,
						 const xmlChar *SystemID,
						 const xmlChar *content);
XMLPUBFUN xmlEntityPtr XMLCALL
			xmlAddDtdEntity		(xmlDocPtr doc,
						 const xmlChar *name,
						 int type,
						 const xmlChar *ExternalID,
						 const xmlChar *SystemID,
						 const xmlChar *content);
XMLPUBFUN xmlEntityPtr XMLCALL
			xmlGetPredefinedEntity	(const xmlChar *name);
XMLPUBFUN xmlEntityPtr XMLCALL
			xmlGetDocEntity		(xmlDocPtr doc,
						 const xmlChar *name);
XMLPUBFUN xmlEntityPtr XMLCALL
			xmlGetDtdEntity		(xmlDocPtr doc,
						 const xmlChar *name);
XMLPUBFUN xmlEntityPtr XMLCALL
			xmlGetParameterEntity	(xmlDocPtr doc,
						 const xmlChar *name);
#ifdef LIBXML_LEGACY_ENABLED
XMLPUBFUN const xmlChar * XMLCALL
			xmlEncodeEntities	(xmlDocPtr doc,
						 const xmlChar *input);
#endif 
XMLPUBFUN xmlChar * XMLCALL
			xmlEncodeEntitiesReentrant(xmlDocPtr doc,
						 const xmlChar *input);
XMLPUBFUN xmlChar * XMLCALL
			xmlEncodeSpecialChars	(xmlDocPtr doc,
						 const xmlChar *input);
XMLPUBFUN xmlEntitiesTablePtr XMLCALL
			xmlCreateEntitiesTable	(void);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlEntitiesTablePtr XMLCALL
			xmlCopyEntitiesTable	(xmlEntitiesTablePtr table);
#endif 
XMLPUBFUN void XMLCALL
			xmlFreeEntitiesTable	(xmlEntitiesTablePtr table);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL
			xmlDumpEntitiesTable	(xmlBufferPtr buf,
						 xmlEntitiesTablePtr table);
XMLPUBFUN void XMLCALL
			xmlDumpEntityDecl	(xmlBufferPtr buf,
						 xmlEntityPtr ent);
#endif 
#ifdef LIBXML_LEGACY_ENABLED
XMLPUBFUN void XMLCALL
			xmlCleanupPredefinedEntities(void);
#endif 


#ifdef __cplusplus
}
#endif

# endif 
