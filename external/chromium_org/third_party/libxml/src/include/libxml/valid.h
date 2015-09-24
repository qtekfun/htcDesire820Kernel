/*
 * Summary: The DTD validation
 * Description: API for the DTD handling and the validity checking
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */


#ifndef __XML_VALID_H__
#define __XML_VALID_H__

#include <libxml/xmlversion.h>
#include <libxml/xmlerror.h>
#include <libxml/tree.h>
#include <libxml/list.h>
#include <libxml/xmlautomata.h>
#include <libxml/xmlregexp.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xmlValidState xmlValidState;
typedef xmlValidState *xmlValidStatePtr;

typedef void (XMLCDECL *xmlValidityErrorFunc) (void *ctx,
			     const char *msg,
			     ...) LIBXML_ATTR_FORMAT(2,3);

typedef void (XMLCDECL *xmlValidityWarningFunc) (void *ctx,
			       const char *msg,
			       ...) LIBXML_ATTR_FORMAT(2,3);

#ifdef IN_LIBXML
#define XML_CTXT_FINISH_DTD_0 0xabcd1234
#define XML_CTXT_FINISH_DTD_1 0xabcd1235
#endif

typedef struct _xmlValidCtxt xmlValidCtxt;
typedef xmlValidCtxt *xmlValidCtxtPtr;
struct _xmlValidCtxt {
    void *userData;			
    xmlValidityErrorFunc error;		
    xmlValidityWarningFunc warning;	

    
    xmlNodePtr         node;          
    int                nodeNr;        
    int                nodeMax;       
    xmlNodePtr        *nodeTab;       

    unsigned int     finishDtd;       
    xmlDocPtr              doc;       
    int                  valid;       

    
    xmlValidState     *vstate;        
    int                vstateNr;      
    int                vstateMax;     
    xmlValidState     *vstateTab;     

#ifdef LIBXML_REGEXP_ENABLED
    xmlAutomataPtr            am;     
    xmlAutomataStatePtr    state;     
#else
    void                     *am;
    void                  *state;
#endif
};


typedef struct _xmlHashTable xmlNotationTable;
typedef xmlNotationTable *xmlNotationTablePtr;


typedef struct _xmlHashTable xmlElementTable;
typedef xmlElementTable *xmlElementTablePtr;


typedef struct _xmlHashTable xmlAttributeTable;
typedef xmlAttributeTable *xmlAttributeTablePtr;


typedef struct _xmlHashTable xmlIDTable;
typedef xmlIDTable *xmlIDTablePtr;


typedef struct _xmlHashTable xmlRefTable;
typedef xmlRefTable *xmlRefTablePtr;

XMLPUBFUN xmlNotationPtr XMLCALL	    
		xmlAddNotationDecl	(xmlValidCtxtPtr ctxt,
					 xmlDtdPtr dtd,
					 const xmlChar *name,
					 const xmlChar *PublicID,
					 const xmlChar *SystemID);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlNotationTablePtr XMLCALL 
		xmlCopyNotationTable	(xmlNotationTablePtr table);
#endif 
XMLPUBFUN void XMLCALL		    
		xmlFreeNotationTable	(xmlNotationTablePtr table);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL		    
		xmlDumpNotationDecl	(xmlBufferPtr buf,
					 xmlNotationPtr nota);
XMLPUBFUN void XMLCALL		    
		xmlDumpNotationTable	(xmlBufferPtr buf,
					 xmlNotationTablePtr table);
#endif 

XMLPUBFUN xmlElementContentPtr XMLCALL 
		xmlNewElementContent	(const xmlChar *name,
					 xmlElementContentType type);
XMLPUBFUN xmlElementContentPtr XMLCALL 
		xmlCopyElementContent	(xmlElementContentPtr content);
XMLPUBFUN void XMLCALL		     
		xmlFreeElementContent	(xmlElementContentPtr cur);
XMLPUBFUN xmlElementContentPtr XMLCALL 
		xmlNewDocElementContent	(xmlDocPtr doc,
					 const xmlChar *name,
					 xmlElementContentType type);
XMLPUBFUN xmlElementContentPtr XMLCALL 
		xmlCopyDocElementContent(xmlDocPtr doc,
					 xmlElementContentPtr content);
XMLPUBFUN void XMLCALL		     
		xmlFreeDocElementContent(xmlDocPtr doc,
					 xmlElementContentPtr cur);
XMLPUBFUN void XMLCALL		     
		xmlSnprintfElementContent(char *buf,
					 int size,
	                                 xmlElementContentPtr content,
					 int englob);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL		     
		xmlSprintfElementContent(char *buf,
	                                 xmlElementContentPtr content,
					 int englob);
#endif 

XMLPUBFUN xmlElementPtr XMLCALL	   
		xmlAddElementDecl	(xmlValidCtxtPtr ctxt,
					 xmlDtdPtr dtd,
					 const xmlChar *name,
					 xmlElementTypeVal type,
					 xmlElementContentPtr content);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlElementTablePtr XMLCALL 
		xmlCopyElementTable	(xmlElementTablePtr table);
#endif 
XMLPUBFUN void XMLCALL		   
		xmlFreeElementTable	(xmlElementTablePtr table);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL		   
		xmlDumpElementTable	(xmlBufferPtr buf,
					 xmlElementTablePtr table);
XMLPUBFUN void XMLCALL		   
		xmlDumpElementDecl	(xmlBufferPtr buf,
					 xmlElementPtr elem);
#endif 

XMLPUBFUN xmlEnumerationPtr XMLCALL 
		xmlCreateEnumeration	(const xmlChar *name);
XMLPUBFUN void XMLCALL		   
		xmlFreeEnumeration	(xmlEnumerationPtr cur);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlEnumerationPtr XMLCALL  
		xmlCopyEnumeration	(xmlEnumerationPtr cur);
#endif 

XMLPUBFUN xmlAttributePtr XMLCALL	    
		xmlAddAttributeDecl	(xmlValidCtxtPtr ctxt,
					 xmlDtdPtr dtd,
					 const xmlChar *elem,
					 const xmlChar *name,
					 const xmlChar *ns,
					 xmlAttributeType type,
					 xmlAttributeDefault def,
					 const xmlChar *defaultValue,
					 xmlEnumerationPtr tree);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlAttributeTablePtr XMLCALL 
		xmlCopyAttributeTable  (xmlAttributeTablePtr table);
#endif 
XMLPUBFUN void XMLCALL		     
		xmlFreeAttributeTable  (xmlAttributeTablePtr table);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL		     
		xmlDumpAttributeTable  (xmlBufferPtr buf,
					xmlAttributeTablePtr table);
XMLPUBFUN void XMLCALL		     
		xmlDumpAttributeDecl   (xmlBufferPtr buf,
					xmlAttributePtr attr);
#endif 

XMLPUBFUN xmlIDPtr XMLCALL	
		xmlAddID	       (xmlValidCtxtPtr ctxt,
					xmlDocPtr doc,
					const xmlChar *value,
					xmlAttrPtr attr);
XMLPUBFUN void XMLCALL		
		xmlFreeIDTable	       (xmlIDTablePtr table);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlGetID	       (xmlDocPtr doc,
					const xmlChar *ID);
XMLPUBFUN int XMLCALL		
		xmlIsID		       (xmlDocPtr doc,
					xmlNodePtr elem,
					xmlAttrPtr attr);
XMLPUBFUN int XMLCALL		
		xmlRemoveID	       (xmlDocPtr doc, 
					xmlAttrPtr attr);

XMLPUBFUN xmlRefPtr XMLCALL	
		xmlAddRef	       (xmlValidCtxtPtr ctxt,
					xmlDocPtr doc,
					const xmlChar *value,
					xmlAttrPtr attr);
XMLPUBFUN void XMLCALL		
		xmlFreeRefTable	       (xmlRefTablePtr table);
XMLPUBFUN int XMLCALL		
		xmlIsRef	       (xmlDocPtr doc,
					xmlNodePtr elem,
					xmlAttrPtr attr);
XMLPUBFUN int XMLCALL		
		xmlRemoveRef	       (xmlDocPtr doc, 
					xmlAttrPtr attr);
XMLPUBFUN xmlListPtr XMLCALL	
		xmlGetRefs	       (xmlDocPtr doc,
					const xmlChar *ID);

#ifdef LIBXML_VALID_ENABLED
XMLPUBFUN xmlValidCtxtPtr XMLCALL	    
		xmlNewValidCtxt(void);
XMLPUBFUN void XMLCALL		    
		xmlFreeValidCtxt(xmlValidCtxtPtr);

XMLPUBFUN int XMLCALL		
		xmlValidateRoot		(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc);
XMLPUBFUN int XMLCALL		
		xmlValidateElementDecl	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
		                         xmlElementPtr elem);
XMLPUBFUN xmlChar * XMLCALL	
		xmlValidNormalizeAttributeValue(xmlDocPtr doc,
					 xmlNodePtr elem,
					 const xmlChar *name,
					 const xmlChar *value);
XMLPUBFUN xmlChar * XMLCALL	
		xmlValidCtxtNormalizeAttributeValue(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr elem,
					 const xmlChar *name,
					 const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateAttributeDecl(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
		                         xmlAttributePtr attr);
XMLPUBFUN int XMLCALL		
		xmlValidateAttributeValue(xmlAttributeType type,
					 const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateNotationDecl	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
		                         xmlNotationPtr nota);
XMLPUBFUN int XMLCALL		
		xmlValidateDtd		(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlDtdPtr dtd);
XMLPUBFUN int XMLCALL		
		xmlValidateDtdFinal	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc);
XMLPUBFUN int XMLCALL		
		xmlValidateDocument	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc);
XMLPUBFUN int XMLCALL		
		xmlValidateElement	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr elem);
XMLPUBFUN int XMLCALL		
		xmlValidateOneElement	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
		                         xmlNodePtr elem);
XMLPUBFUN int XMLCALL	
		xmlValidateOneAttribute	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr	elem,
					 xmlAttrPtr attr,
					 const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateOneNamespace	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr elem,
					 const xmlChar *prefix,
					 xmlNsPtr ns,
					 const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateDocumentFinal(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc);
#endif 

#if defined(LIBXML_VALID_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN int XMLCALL		
		xmlValidateNotationUse	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 const xmlChar *notationName);
#endif 

XMLPUBFUN int XMLCALL		
		xmlIsMixedElement	(xmlDocPtr doc,
					 const xmlChar *name);
XMLPUBFUN xmlAttributePtr XMLCALL	
		xmlGetDtdAttrDesc	(xmlDtdPtr dtd,
					 const xmlChar *elem,
					 const xmlChar *name);
XMLPUBFUN xmlAttributePtr XMLCALL	
		xmlGetDtdQAttrDesc	(xmlDtdPtr dtd,
					 const xmlChar *elem,
					 const xmlChar *name,
					 const xmlChar *prefix);
XMLPUBFUN xmlNotationPtr XMLCALL	
		xmlGetDtdNotationDesc	(xmlDtdPtr dtd,
					 const xmlChar *name);
XMLPUBFUN xmlElementPtr XMLCALL	
		xmlGetDtdQElementDesc	(xmlDtdPtr dtd,
					 const xmlChar *name,
					 const xmlChar *prefix);
XMLPUBFUN xmlElementPtr XMLCALL	
		xmlGetDtdElementDesc	(xmlDtdPtr dtd,
					 const xmlChar *name);

#ifdef LIBXML_VALID_ENABLED

XMLPUBFUN int XMLCALL		
		xmlValidGetPotentialChildren(xmlElementContent *ctree,
					 const xmlChar **names,
					 int *len,
					 int max);

XMLPUBFUN int XMLCALL		
		xmlValidGetValidElements(xmlNode *prev,
					 xmlNode *next,
					 const xmlChar **names,
					 int max);
XMLPUBFUN int XMLCALL		
		xmlValidateNameValue	(const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateNamesValue	(const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateNmtokenValue	(const xmlChar *value);
XMLPUBFUN int XMLCALL		
		xmlValidateNmtokensValue(const xmlChar *value);

#ifdef LIBXML_REGEXP_ENABLED
XMLPUBFUN int XMLCALL		
		xmlValidBuildContentModel(xmlValidCtxtPtr ctxt,
					 xmlElementPtr elem);

XMLPUBFUN int XMLCALL		
		xmlValidatePushElement	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr elem,
					 const xmlChar *qname);
XMLPUBFUN int XMLCALL		
		xmlValidatePushCData	(xmlValidCtxtPtr ctxt,
					 const xmlChar *data,
					 int len);
XMLPUBFUN int XMLCALL		
		xmlValidatePopElement	(xmlValidCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr elem,
					 const xmlChar *qname);
#endif 
#endif 
#ifdef __cplusplus
}
#endif
#endif 
