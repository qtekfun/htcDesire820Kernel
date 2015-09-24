/*
 * Summary: interfaces for tree manipulation
 * Description: this module describes the structures found in an tree resulting
 *              from an XML or HTML parsing, as well as the API provided for
 *              various processing on that tree
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_TREE_H__
#define __XML_TREE_H__

#include <stdio.h>
#include <libxml/xmlversion.h>
#include <libxml/xmlstring.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xmlParserInputBuffer xmlParserInputBuffer;
typedef xmlParserInputBuffer *xmlParserInputBufferPtr;

typedef struct _xmlOutputBuffer xmlOutputBuffer;
typedef xmlOutputBuffer *xmlOutputBufferPtr;

typedef struct _xmlParserInput xmlParserInput;
typedef xmlParserInput *xmlParserInputPtr;

typedef struct _xmlParserCtxt xmlParserCtxt;
typedef xmlParserCtxt *xmlParserCtxtPtr;

typedef struct _xmlSAXLocator xmlSAXLocator;
typedef xmlSAXLocator *xmlSAXLocatorPtr;

typedef struct _xmlSAXHandler xmlSAXHandler;
typedef xmlSAXHandler *xmlSAXHandlerPtr;

typedef struct _xmlEntity xmlEntity;
typedef xmlEntity *xmlEntityPtr;

#define BASE_BUFFER_SIZE 4096



typedef enum {
    XML_BUFFER_ALLOC_DOUBLEIT,	
    XML_BUFFER_ALLOC_EXACT,	
    XML_BUFFER_ALLOC_IMMUTABLE, 
    XML_BUFFER_ALLOC_IO		
} xmlBufferAllocationScheme;

typedef struct _xmlBuffer xmlBuffer;
typedef xmlBuffer *xmlBufferPtr;
struct _xmlBuffer {
    xmlChar *content;		
    unsigned int use;		
    unsigned int size;		
    xmlBufferAllocationScheme alloc; 
    xmlChar *contentIO;		
};

#define XML_XML_NAMESPACE \
    (const xmlChar *) "http://www.w3.org/XML/1998/namespace"

#define XML_XML_ID (const xmlChar *) "xml:id"

typedef enum {
    XML_ELEMENT_NODE=		1,
    XML_ATTRIBUTE_NODE=		2,
    XML_TEXT_NODE=		3,
    XML_CDATA_SECTION_NODE=	4,
    XML_ENTITY_REF_NODE=	5,
    XML_ENTITY_NODE=		6,
    XML_PI_NODE=		7,
    XML_COMMENT_NODE=		8,
    XML_DOCUMENT_NODE=		9,
    XML_DOCUMENT_TYPE_NODE=	10,
    XML_DOCUMENT_FRAG_NODE=	11,
    XML_NOTATION_NODE=		12,
    XML_HTML_DOCUMENT_NODE=	13,
    XML_DTD_NODE=		14,
    XML_ELEMENT_DECL=		15,
    XML_ATTRIBUTE_DECL=		16,
    XML_ENTITY_DECL=		17,
    XML_NAMESPACE_DECL=		18,
    XML_XINCLUDE_START=		19,
    XML_XINCLUDE_END=		20
#ifdef LIBXML_DOCB_ENABLED
   ,XML_DOCB_DOCUMENT_NODE=	21
#endif
} xmlElementType;



typedef struct _xmlNotation xmlNotation;
typedef xmlNotation *xmlNotationPtr;
struct _xmlNotation {
    const xmlChar               *name;	        
    const xmlChar               *PublicID;	
    const xmlChar               *SystemID;	
};


typedef enum {
    XML_ATTRIBUTE_CDATA = 1,
    XML_ATTRIBUTE_ID,
    XML_ATTRIBUTE_IDREF	,
    XML_ATTRIBUTE_IDREFS,
    XML_ATTRIBUTE_ENTITY,
    XML_ATTRIBUTE_ENTITIES,
    XML_ATTRIBUTE_NMTOKEN,
    XML_ATTRIBUTE_NMTOKENS,
    XML_ATTRIBUTE_ENUMERATION,
    XML_ATTRIBUTE_NOTATION
} xmlAttributeType;


typedef enum {
    XML_ATTRIBUTE_NONE = 1,
    XML_ATTRIBUTE_REQUIRED,
    XML_ATTRIBUTE_IMPLIED,
    XML_ATTRIBUTE_FIXED
} xmlAttributeDefault;


typedef struct _xmlEnumeration xmlEnumeration;
typedef xmlEnumeration *xmlEnumerationPtr;
struct _xmlEnumeration {
    struct _xmlEnumeration    *next;	
    const xmlChar            *name;	
};


typedef struct _xmlAttribute xmlAttribute;
typedef xmlAttribute *xmlAttributePtr;
struct _xmlAttribute {
    void           *_private;	        
    xmlElementType          type;       
    const xmlChar          *name;	
    struct _xmlNode    *children;	
    struct _xmlNode        *last;	
    struct _xmlDtd       *parent;	
    struct _xmlNode        *next;	
    struct _xmlNode        *prev;	
    struct _xmlDoc          *doc;       

    struct _xmlAttribute  *nexth;	
    xmlAttributeType       atype;	
    xmlAttributeDefault      def;	
    const xmlChar  *defaultValue;	
    xmlEnumerationPtr       tree;       
    const xmlChar        *prefix;	
    const xmlChar          *elem;	
};

typedef enum {
    XML_ELEMENT_CONTENT_PCDATA = 1,
    XML_ELEMENT_CONTENT_ELEMENT,
    XML_ELEMENT_CONTENT_SEQ,
    XML_ELEMENT_CONTENT_OR
} xmlElementContentType;

typedef enum {
    XML_ELEMENT_CONTENT_ONCE = 1,
    XML_ELEMENT_CONTENT_OPT,
    XML_ELEMENT_CONTENT_MULT,
    XML_ELEMENT_CONTENT_PLUS
} xmlElementContentOccur;


typedef struct _xmlElementContent xmlElementContent;
typedef xmlElementContent *xmlElementContentPtr;
struct _xmlElementContent {
    xmlElementContentType     type;	
    xmlElementContentOccur    ocur;	
    const xmlChar             *name;	
    struct _xmlElementContent *c1;	
    struct _xmlElementContent *c2;	
    struct _xmlElementContent *parent;	
    const xmlChar             *prefix;	
};


typedef enum {
    XML_ELEMENT_TYPE_UNDEFINED = 0,
    XML_ELEMENT_TYPE_EMPTY = 1,
    XML_ELEMENT_TYPE_ANY,
    XML_ELEMENT_TYPE_MIXED,
    XML_ELEMENT_TYPE_ELEMENT
} xmlElementTypeVal;

#ifdef __cplusplus
}
#endif
#include <libxml/xmlregexp.h>
#ifdef __cplusplus
extern "C" {
#endif


typedef struct _xmlElement xmlElement;
typedef xmlElement *xmlElementPtr;
struct _xmlElement {
    void           *_private;	        
    xmlElementType          type;       
    const xmlChar          *name;	
    struct _xmlNode    *children;	
    struct _xmlNode        *last;	
    struct _xmlDtd       *parent;	
    struct _xmlNode        *next;	
    struct _xmlNode        *prev;	
    struct _xmlDoc          *doc;       

    xmlElementTypeVal      etype;	
    xmlElementContentPtr content;	
    xmlAttributePtr   attributes;	
    const xmlChar        *prefix;	
#ifdef LIBXML_REGEXP_ENABLED
    xmlRegexpPtr       contModel;	
#else
    void	      *contModel;
#endif
};


#define XML_LOCAL_NAMESPACE XML_NAMESPACE_DECL
typedef xmlElementType xmlNsType;


typedef struct _xmlNs xmlNs;
typedef xmlNs *xmlNsPtr;
struct _xmlNs {
    struct _xmlNs  *next;	
    xmlNsType      type;	
    const xmlChar *href;	
    const char* dummy_children; 
    const xmlChar *prefix;	
    void           *_private;   
    struct _xmlDoc *context;		
};

typedef struct _xmlDtd xmlDtd;
typedef xmlDtd *xmlDtdPtr;
struct _xmlDtd {
    void           *_private;	
    xmlElementType  type;       
    const xmlChar *name;	
    struct _xmlNode *children;	
    struct _xmlNode *last;	
    struct _xmlDoc  *parent;	
    struct _xmlNode *next;	
    struct _xmlNode *prev;	
    struct _xmlDoc  *doc;	

    
    void          *notations;   
    void          *elements;    
    void          *attributes;  
    void          *entities;    
    const xmlChar *ExternalID;	
    const xmlChar *SystemID;	
    void          *pentities;   
};

typedef struct _xmlAttr xmlAttr;
typedef xmlAttr *xmlAttrPtr;
struct _xmlAttr {
    void           *_private;	
    xmlElementType   type;      
    const xmlChar   *name;      
    struct _xmlNode *children;	
    struct _xmlNode *last;	
    struct _xmlNode *parent;	
    struct _xmlAttr *next;	
    struct _xmlAttr *prev;	
    struct _xmlDoc  *doc;	
    xmlNs           *ns;        
    xmlAttributeType atype;     
    void            *psvi;	
};


typedef struct _xmlID xmlID;
typedef xmlID *xmlIDPtr;
struct _xmlID {
    struct _xmlID    *next;	
    const xmlChar    *value;	
    xmlAttrPtr        attr;	
    const xmlChar    *name;	
    int               lineno;	
    struct _xmlDoc   *doc;	
};


typedef struct _xmlRef xmlRef;
typedef xmlRef *xmlRefPtr;
struct _xmlRef {
    struct _xmlRef    *next;	
    const xmlChar     *value;	
    xmlAttrPtr        attr;	
    const xmlChar    *name;	
    int               lineno;	
};

typedef struct _xmlNode xmlNode;
typedef xmlNode *xmlNodePtr;
struct _xmlNode {
    void           *_private;	
    xmlElementType   type;	
    const xmlChar   *name;      
    struct _xmlNode *children;	
    struct _xmlNode *last;	
    struct _xmlNode *parent;	
    struct _xmlNode *next;	
    struct _xmlNode *prev;	
    struct _xmlDoc  *doc;	

    
    xmlNs           *ns;        
    xmlChar         *content;   
    struct _xmlAttr *properties;
    xmlNs           *nsDef;     
    void            *psvi;	
    unsigned short   line;	
    unsigned short   extra;	
};

#define XML_GET_CONTENT(n)					\
    ((n)->type == XML_ELEMENT_NODE ? NULL : (n)->content)

#define XML_GET_LINE(n)						\
    (xmlGetLineNo(n))

typedef enum {
    XML_DOC_WELLFORMED		= 1<<0, 
    XML_DOC_NSVALID		= 1<<1, 
    XML_DOC_OLD10		= 1<<2, 
    XML_DOC_DTDVALID		= 1<<3, 
    XML_DOC_XINCLUDE		= 1<<4, 
    XML_DOC_USERBUILT		= 1<<5, 
    XML_DOC_INTERNAL		= 1<<6, 
    XML_DOC_HTML		= 1<<7  
} xmlDocProperties;

typedef struct _xmlDoc xmlDoc;
typedef xmlDoc *xmlDocPtr;
struct _xmlDoc {
    void           *_private;	
    xmlElementType  type;       
    char           *name;	
    struct _xmlNode *children;	
    struct _xmlNode *last;	
    struct _xmlNode *parent;	
    struct _xmlNode *next;	
    struct _xmlNode *prev;	
    struct _xmlDoc  *doc;	

    
    int             compression;
    int             standalone; 
    struct _xmlDtd  *intSubset;	
    struct _xmlDtd  *extSubset;	
    struct _xmlNs   *oldNs;	
    const xmlChar  *version;	
    const xmlChar  *encoding;   
    void           *ids;        
    void           *refs;       
    const xmlChar  *URL;	
    int             charset;    
    struct _xmlDict *dict;      
    void           *psvi;	
    int             parseFlags;	
    int             properties;	
};


typedef struct _xmlDOMWrapCtxt xmlDOMWrapCtxt;
typedef xmlDOMWrapCtxt *xmlDOMWrapCtxtPtr;

typedef xmlNsPtr (*xmlDOMWrapAcquireNsFunction) (xmlDOMWrapCtxtPtr ctxt,
						 xmlNodePtr node,
						 const xmlChar *nsName,
						 const xmlChar *nsPrefix);

struct _xmlDOMWrapCtxt {
    void * _private;
    int type;
    void * namespaceMap;
    xmlDOMWrapAcquireNsFunction getNsForNodeFunc;
};

#ifndef xmlChildrenNode
#define xmlChildrenNode children
#endif

#ifndef xmlRootNode
#define xmlRootNode children
#endif


#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED) || defined(LIBXML_DEBUG_ENABLED) || defined (LIBXML_HTML_ENABLED) || defined(LIBXML_SAX1_ENABLED) || defined(LIBXML_HTML_ENABLED) || defined(LIBXML_WRITER_ENABLED) || defined(LIBXML_DOCB_ENABLED)
XMLPUBFUN int XMLCALL
		xmlValidateNCName	(const xmlChar *value,
					 int space);
#endif

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN int XMLCALL		
		xmlValidateQName	(const xmlChar *value,
					 int space);
XMLPUBFUN int XMLCALL		
		xmlValidateName		(const xmlChar *value,
					 int space);
XMLPUBFUN int XMLCALL		
		xmlValidateNMToken	(const xmlChar *value,
					 int space);
#endif

XMLPUBFUN xmlChar * XMLCALL	
		xmlBuildQName		(const xmlChar *ncname,
					 const xmlChar *prefix,
					 xmlChar *memory,
					 int len);
XMLPUBFUN xmlChar * XMLCALL	
		xmlSplitQName2		(const xmlChar *name,
					 xmlChar **prefix);
XMLPUBFUN const xmlChar * XMLCALL	
		xmlSplitQName3		(const xmlChar *name,
					 int *len);


XMLPUBFUN void XMLCALL		
		xmlSetBufferAllocationScheme(xmlBufferAllocationScheme scheme);
XMLPUBFUN xmlBufferAllocationScheme XMLCALL	 
		xmlGetBufferAllocationScheme(void);

XMLPUBFUN xmlBufferPtr XMLCALL	
		xmlBufferCreate		(void);
XMLPUBFUN xmlBufferPtr XMLCALL	
		xmlBufferCreateSize	(size_t size);
XMLPUBFUN xmlBufferPtr XMLCALL	
		xmlBufferCreateStatic	(void *mem,
					 size_t size);
XMLPUBFUN int XMLCALL		
		xmlBufferResize		(xmlBufferPtr buf,
					 unsigned int size);
XMLPUBFUN void XMLCALL		
		xmlBufferFree		(xmlBufferPtr buf);
XMLPUBFUN int XMLCALL		
		xmlBufferDump		(FILE *file,
					 xmlBufferPtr buf);
XMLPUBFUN int XMLCALL		
		xmlBufferAdd		(xmlBufferPtr buf,
					 const xmlChar *str,
					 int len);
XMLPUBFUN int XMLCALL		
		xmlBufferAddHead	(xmlBufferPtr buf,
					 const xmlChar *str,
					 int len);
XMLPUBFUN int XMLCALL		
		xmlBufferCat		(xmlBufferPtr buf,
					 const xmlChar *str);
XMLPUBFUN int XMLCALL	
		xmlBufferCCat		(xmlBufferPtr buf,
					 const char *str);
XMLPUBFUN int XMLCALL		
		xmlBufferShrink		(xmlBufferPtr buf,
					 unsigned int len);
XMLPUBFUN int XMLCALL		
		xmlBufferGrow		(xmlBufferPtr buf,
					 unsigned int len);
XMLPUBFUN void XMLCALL		
		xmlBufferEmpty		(xmlBufferPtr buf);
XMLPUBFUN const xmlChar* XMLCALL	
		xmlBufferContent	(const xmlBufferPtr buf);
XMLPUBFUN void XMLCALL		
		xmlBufferSetAllocationScheme(xmlBufferPtr buf,
					 xmlBufferAllocationScheme scheme);
XMLPUBFUN int XMLCALL		
		xmlBufferLength		(const xmlBufferPtr buf);

XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlCreateIntSubset	(xmlDocPtr doc,
					 const xmlChar *name,
					 const xmlChar *ExternalID,
					 const xmlChar *SystemID);
XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlNewDtd		(xmlDocPtr doc,
					 const xmlChar *name,
					 const xmlChar *ExternalID,
					 const xmlChar *SystemID);
XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlGetIntSubset		(xmlDocPtr doc);
XMLPUBFUN void XMLCALL		
		xmlFreeDtd		(xmlDtdPtr cur);
#ifdef LIBXML_LEGACY_ENABLED
XMLPUBFUN xmlNsPtr XMLCALL	
		xmlNewGlobalNs		(xmlDocPtr doc,
					 const xmlChar *href,
					 const xmlChar *prefix);
#endif 
XMLPUBFUN xmlNsPtr XMLCALL	
		xmlNewNs		(xmlNodePtr node,
					 const xmlChar *href,
					 const xmlChar *prefix);
XMLPUBFUN void XMLCALL		
		xmlFreeNs		(xmlNsPtr cur);
XMLPUBFUN void XMLCALL		
		xmlFreeNsList		(xmlNsPtr cur);
XMLPUBFUN xmlDocPtr XMLCALL 	
		xmlNewDoc		(const xmlChar *version);
XMLPUBFUN void XMLCALL		
		xmlFreeDoc		(xmlDocPtr cur);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlNewDocProp		(xmlDocPtr doc,
					 const xmlChar *name,
					 const xmlChar *value);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_HTML_ENABLED) || \
    defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlNewProp		(xmlNodePtr node,
					 const xmlChar *name,
					 const xmlChar *value);
#endif
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlNewNsProp		(xmlNodePtr node,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 const xmlChar *value);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlNewNsPropEatName	(xmlNodePtr node,
					 xmlNsPtr ns,
					 xmlChar *name,
					 const xmlChar *value);
XMLPUBFUN void XMLCALL		
		xmlFreePropList		(xmlAttrPtr cur);
XMLPUBFUN void XMLCALL		
		xmlFreeProp		(xmlAttrPtr cur);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlCopyProp		(xmlNodePtr target,
					 xmlAttrPtr cur);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlCopyPropList		(xmlNodePtr target,
					 xmlAttrPtr cur);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlCopyDtd		(xmlDtdPtr dtd);
#endif 
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlCopyDoc		(xmlDocPtr doc,
					 int recursive);
#endif 
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocNode		(xmlDocPtr doc,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocNodeEatName	(xmlDocPtr doc,
					 xmlNsPtr ns,
					 xmlChar *name,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewNode		(xmlNsPtr ns,
					 const xmlChar *name);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewNodeEatName	(xmlNsPtr ns,
					 xmlChar *name);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewChild		(xmlNodePtr parent,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 const xmlChar *content);
#endif
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocText		(xmlDocPtr doc,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewText		(const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocPI		(xmlDocPtr doc,
					 const xmlChar *name,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewPI		(const xmlChar *name,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocTextLen	(xmlDocPtr doc,
					 const xmlChar *content,
					 int len);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewTextLen		(const xmlChar *content,
					 int len);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocComment	(xmlDocPtr doc,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewComment		(const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewCDataBlock	(xmlDocPtr doc,
					 const xmlChar *content,
					 int len);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewCharRef		(xmlDocPtr doc,
					 const xmlChar *name);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewReference		(xmlDocPtr doc,
					 const xmlChar *name);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlCopyNode		(const xmlNodePtr node,
					 int recursive);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlDocCopyNode		(const xmlNodePtr node,
					 xmlDocPtr doc,
					 int recursive);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlDocCopyNodeList	(xmlDocPtr doc,
					 const xmlNodePtr node);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlCopyNodeList		(const xmlNodePtr node);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewTextChild		(xmlNodePtr parent,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocRawNode	(xmlDocPtr doc,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 const xmlChar *content);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlNewDocFragment	(xmlDocPtr doc);
#endif 

XMLPUBFUN long XMLCALL		
		xmlGetLineNo		(xmlNodePtr node);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_DEBUG_ENABLED)
XMLPUBFUN xmlChar * XMLCALL	
		xmlGetNodePath		(xmlNodePtr node);
#endif 
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlDocGetRootElement	(xmlDocPtr doc);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlGetLastChild		(xmlNodePtr parent);
XMLPUBFUN int XMLCALL		
		xmlNodeIsText		(xmlNodePtr node);
XMLPUBFUN int XMLCALL		
		xmlIsBlankNode		(xmlNodePtr node);

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_WRITER_ENABLED)
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlDocSetRootElement	(xmlDocPtr doc,
					 xmlNodePtr root);
#endif 
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN void XMLCALL		
		xmlNodeSetName		(xmlNodePtr cur,
					 const xmlChar *name);
#endif 
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlAddChild		(xmlNodePtr parent,
					 xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlAddChildList		(xmlNodePtr parent,
					 xmlNodePtr cur);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_WRITER_ENABLED)
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlReplaceNode		(xmlNodePtr old,
					 xmlNodePtr cur);
#endif 
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_HTML_ENABLED) || \
    defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlAddPrevSibling	(xmlNodePtr cur,
					 xmlNodePtr elem);
#endif 
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlAddSibling		(xmlNodePtr cur,
					 xmlNodePtr elem);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlAddNextSibling	(xmlNodePtr cur,
					 xmlNodePtr elem);
XMLPUBFUN void XMLCALL		
		xmlUnlinkNode		(xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlTextMerge		(xmlNodePtr first,
					 xmlNodePtr second);
XMLPUBFUN int XMLCALL		
		xmlTextConcat		(xmlNodePtr node,
					 const xmlChar *content,
					 int len);
XMLPUBFUN void XMLCALL		
		xmlFreeNodeList		(xmlNodePtr cur);
XMLPUBFUN void XMLCALL		
		xmlFreeNode		(xmlNodePtr cur);
XMLPUBFUN void XMLCALL		
		xmlSetTreeDoc		(xmlNodePtr tree,
					 xmlDocPtr doc);
XMLPUBFUN void XMLCALL		
		xmlSetListDoc		(xmlNodePtr list,
					 xmlDocPtr doc);
XMLPUBFUN xmlNsPtr XMLCALL	
		xmlSearchNs		(xmlDocPtr doc,
					 xmlNodePtr node,
					 const xmlChar *nameSpace);
XMLPUBFUN xmlNsPtr XMLCALL	
		xmlSearchNsByHref	(xmlDocPtr doc,
					 xmlNodePtr node,
					 const xmlChar *href);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN xmlNsPtr * XMLCALL	
		xmlGetNsList		(xmlDocPtr doc,
					 xmlNodePtr node);
#endif 

XMLPUBFUN void XMLCALL		
		xmlSetNs		(xmlNodePtr node,
					 xmlNsPtr ns);
XMLPUBFUN xmlNsPtr XMLCALL	
		xmlCopyNamespace	(xmlNsPtr cur);
XMLPUBFUN xmlNsPtr XMLCALL	
		xmlCopyNamespaceList	(xmlNsPtr cur);

#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XINCLUDE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED) || defined(LIBXML_HTML_ENABLED)
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlSetProp		(xmlNodePtr node,
					 const xmlChar *name,
					 const xmlChar *value);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlSetNsProp		(xmlNodePtr node,
					 xmlNsPtr ns,
					 const xmlChar *name,
					 const xmlChar *value);
#endif 
XMLPUBFUN xmlChar * XMLCALL	
		xmlGetNoNsProp		(xmlNodePtr node,
					 const xmlChar *name);
XMLPUBFUN xmlChar * XMLCALL	
		xmlGetProp		(xmlNodePtr node,
					 const xmlChar *name);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlHasProp		(xmlNodePtr node,
					 const xmlChar *name);
XMLPUBFUN xmlAttrPtr XMLCALL	
		xmlHasNsProp		(xmlNodePtr node,
					 const xmlChar *name,
					 const xmlChar *nameSpace);
XMLPUBFUN xmlChar * XMLCALL	
		xmlGetNsProp		(xmlNodePtr node,
					 const xmlChar *name,
					 const xmlChar *nameSpace);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlStringGetNodeList	(xmlDocPtr doc,
					 const xmlChar *value);
XMLPUBFUN xmlNodePtr XMLCALL	
		xmlStringLenGetNodeList	(xmlDocPtr doc,
					 const xmlChar *value,
					 int len);
XMLPUBFUN xmlChar * XMLCALL	
		xmlNodeListGetString	(xmlDocPtr doc,
					 xmlNodePtr list,
					 int inLine);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN xmlChar * XMLCALL	
		xmlNodeListGetRawString	(xmlDocPtr doc,
					 xmlNodePtr list,
					 int inLine);
#endif 
XMLPUBFUN void XMLCALL		
		xmlNodeSetContent	(xmlNodePtr cur,
					 const xmlChar *content);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN void XMLCALL		
		xmlNodeSetContentLen	(xmlNodePtr cur,
					 const xmlChar *content,
					 int len);
#endif 
XMLPUBFUN void XMLCALL		
		xmlNodeAddContent	(xmlNodePtr cur,
					 const xmlChar *content);
XMLPUBFUN void XMLCALL		
		xmlNodeAddContentLen	(xmlNodePtr cur,
					 const xmlChar *content,
					 int len);
XMLPUBFUN xmlChar * XMLCALL	
		xmlNodeGetContent	(xmlNodePtr cur);
XMLPUBFUN int XMLCALL
		xmlNodeBufGetContent	(xmlBufferPtr buffer,
					 xmlNodePtr cur);
XMLPUBFUN xmlChar * XMLCALL	
		xmlNodeGetLang		(xmlNodePtr cur);
XMLPUBFUN int XMLCALL		
		xmlNodeGetSpacePreserve	(xmlNodePtr cur);
#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN void XMLCALL		
		xmlNodeSetLang		(xmlNodePtr cur,
					 const xmlChar *lang);
XMLPUBFUN void XMLCALL		
		xmlNodeSetSpacePreserve (xmlNodePtr cur,
					 int val);
#endif 
XMLPUBFUN xmlChar * XMLCALL	
		xmlNodeGetBase		(xmlDocPtr doc,
					 xmlNodePtr cur);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_XINCLUDE_ENABLED)
XMLPUBFUN void XMLCALL		
		xmlNodeSetBase		(xmlNodePtr cur,
					 const xmlChar *uri);
#endif

XMLPUBFUN int XMLCALL		
		xmlRemoveProp		(xmlAttrPtr cur);
#if defined(LIBXML_TREE_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN int XMLCALL		
		xmlUnsetNsProp		(xmlNodePtr node,
					 xmlNsPtr ns,
					 const xmlChar *name);
XMLPUBFUN int XMLCALL		
		xmlUnsetProp		(xmlNodePtr node,
					 const xmlChar *name);
#endif 

XMLPUBFUN void XMLCALL		
		xmlBufferWriteCHAR	(xmlBufferPtr buf,
					 const xmlChar *string);
XMLPUBFUN void XMLCALL		
		xmlBufferWriteChar	(xmlBufferPtr buf,
					 const char *string);
XMLPUBFUN void XMLCALL		
		xmlBufferWriteQuotedString(xmlBufferPtr buf,
					 const xmlChar *string);

#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void xmlAttrSerializeTxtContent(xmlBufferPtr buf,
					 xmlDocPtr doc,
					 xmlAttrPtr attr,
					 const xmlChar *string);
#endif 

#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN int XMLCALL		
		xmlReconciliateNs	(xmlDocPtr doc,
					 xmlNodePtr tree);
#endif

#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL		
		xmlDocDumpFormatMemory	(xmlDocPtr cur,
					 xmlChar **mem,
					 int *size,
					 int format);
XMLPUBFUN void XMLCALL		
		xmlDocDumpMemory	(xmlDocPtr cur,
					 xmlChar **mem,
					 int *size);
XMLPUBFUN void XMLCALL		
		xmlDocDumpMemoryEnc	(xmlDocPtr out_doc,
					 xmlChar **doc_txt_ptr,
					 int * doc_txt_len,
					 const char *txt_encoding);
XMLPUBFUN void XMLCALL		
		xmlDocDumpFormatMemoryEnc(xmlDocPtr out_doc,
					 xmlChar **doc_txt_ptr,
					 int * doc_txt_len,
					 const char *txt_encoding,
					 int format);
XMLPUBFUN int XMLCALL		
		xmlDocFormatDump	(FILE *f,
					 xmlDocPtr cur,
					 int format);
XMLPUBFUN int XMLCALL	
		xmlDocDump		(FILE *f,
					 xmlDocPtr cur);
XMLPUBFUN void XMLCALL		
		xmlElemDump		(FILE *f,
					 xmlDocPtr doc,
					 xmlNodePtr cur);
XMLPUBFUN int XMLCALL		
		xmlSaveFile		(const char *filename,
					 xmlDocPtr cur);
XMLPUBFUN int XMLCALL		
		xmlSaveFormatFile	(const char *filename,
					 xmlDocPtr cur,
					 int format);
XMLPUBFUN int XMLCALL		
		xmlNodeDump		(xmlBufferPtr buf,
					 xmlDocPtr doc,
					 xmlNodePtr cur,
					 int level,
					 int format);

XMLPUBFUN int XMLCALL		
		xmlSaveFileTo		(xmlOutputBufferPtr buf,
					 xmlDocPtr cur,
					 const char *encoding);
XMLPUBFUN int XMLCALL             
		xmlSaveFormatFileTo     (xmlOutputBufferPtr buf,
					 xmlDocPtr cur,
				         const char *encoding,
				         int format);
XMLPUBFUN void XMLCALL		
		xmlNodeDumpOutput	(xmlOutputBufferPtr buf,
					 xmlDocPtr doc,
					 xmlNodePtr cur,
					 int level,
					 int format,
					 const char *encoding);

XMLPUBFUN int XMLCALL		
		xmlSaveFormatFileEnc    (const char *filename,
					 xmlDocPtr cur,
					 const char *encoding,
					 int format);

XMLPUBFUN int XMLCALL		
		xmlSaveFileEnc		(const char *filename,
					 xmlDocPtr cur,
					 const char *encoding);

#endif 
XMLPUBFUN int XMLCALL		
		xmlIsXHTML		(const xmlChar *systemID,
					 const xmlChar *publicID);

XMLPUBFUN int XMLCALL		
		xmlGetDocCompressMode	(xmlDocPtr doc);
XMLPUBFUN void XMLCALL		
		xmlSetDocCompressMode	(xmlDocPtr doc,
					 int mode);
XMLPUBFUN int XMLCALL		
		xmlGetCompressMode	(void);
XMLPUBFUN void XMLCALL		
		xmlSetCompressMode	(int mode);

XMLPUBFUN xmlDOMWrapCtxtPtr XMLCALL
		xmlDOMWrapNewCtxt	(void);
XMLPUBFUN void XMLCALL
		xmlDOMWrapFreeCtxt	(xmlDOMWrapCtxtPtr ctxt);
XMLPUBFUN int XMLCALL
	    xmlDOMWrapReconcileNamespaces(xmlDOMWrapCtxtPtr ctxt,
					 xmlNodePtr elem,
					 int options);
XMLPUBFUN int XMLCALL
	    xmlDOMWrapAdoptNode		(xmlDOMWrapCtxtPtr ctxt,
					 xmlDocPtr sourceDoc,
					 xmlNodePtr node,
					 xmlDocPtr destDoc,		    
					 xmlNodePtr destParent,
					 int options);
XMLPUBFUN int XMLCALL
	    xmlDOMWrapRemoveNode	(xmlDOMWrapCtxtPtr ctxt,
					 xmlDocPtr doc,
					 xmlNodePtr node,
					 int options);
XMLPUBFUN int XMLCALL
	    xmlDOMWrapCloneNode		(xmlDOMWrapCtxtPtr ctxt,
					 xmlDocPtr sourceDoc,
					 xmlNodePtr node,
					 xmlNodePtr *clonedNode,
					 xmlDocPtr destDoc,
					 xmlNodePtr destParent,
					 int deep,
					 int options);

#ifdef LIBXML_TREE_ENABLED
XMLPUBFUN unsigned long XMLCALL
            xmlChildElementCount        (xmlNodePtr parent);
XMLPUBFUN xmlNodePtr XMLCALL
            xmlNextElementSibling       (xmlNodePtr node);
XMLPUBFUN xmlNodePtr XMLCALL
            xmlFirstElementChild        (xmlNodePtr parent);
XMLPUBFUN xmlNodePtr XMLCALL
            xmlLastElementChild         (xmlNodePtr parent);
XMLPUBFUN xmlNodePtr XMLCALL
            xmlPreviousElementSibling   (xmlNodePtr node);
#endif
#ifdef __cplusplus
}
#endif
#ifndef __XML_PARSER_H__
#include <libxml/xmlmemory.h>
#endif

#endif 

