/*
 * Summary: the core parser module
 * Description: Interfaces, constants and types related to the XML parser
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include <stdarg.h>

#include <libxml/xmlversion.h>
#include <libxml/tree.h>
#include <libxml/dict.h>
#include <libxml/hash.h>
#include <libxml/valid.h>
#include <libxml/entities.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlstring.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XML_DEFAULT_VERSION	"1.0"


typedef void (* xmlParserInputDeallocate)(xmlChar *str);

struct _xmlParserInput {
    
    xmlParserInputBufferPtr buf;      

    const char *filename;             
    const char *directory;            
    const xmlChar *base;              
    const xmlChar *cur;               
    const xmlChar *end;               
    int length;                       
    int line;                         
    int col;                          
    unsigned long consumed;           
    xmlParserInputDeallocate free;    
    const xmlChar *encoding;          
    const xmlChar *version;           
    int standalone;                   
    int id;                           
};

typedef struct _xmlParserNodeInfo xmlParserNodeInfo;
typedef xmlParserNodeInfo *xmlParserNodeInfoPtr;

struct _xmlParserNodeInfo {
  const struct _xmlNode* node;
  
  unsigned long begin_pos;
  unsigned long begin_line;
  unsigned long end_pos;
  unsigned long end_line;
};

typedef struct _xmlParserNodeInfoSeq xmlParserNodeInfoSeq;
typedef xmlParserNodeInfoSeq *xmlParserNodeInfoSeqPtr;
struct _xmlParserNodeInfoSeq {
  unsigned long maximum;
  unsigned long length;
  xmlParserNodeInfo* buffer;
};

typedef enum {
    XML_PARSER_EOF = -1,	
    XML_PARSER_START = 0,	
    XML_PARSER_MISC,		
    XML_PARSER_PI,		
    XML_PARSER_DTD,		
    XML_PARSER_PROLOG,		
    XML_PARSER_COMMENT,		
    XML_PARSER_START_TAG,	
    XML_PARSER_CONTENT,		
    XML_PARSER_CDATA_SECTION,	
    XML_PARSER_END_TAG,		
    XML_PARSER_ENTITY_DECL,	
    XML_PARSER_ENTITY_VALUE,	
    XML_PARSER_ATTRIBUTE_VALUE,	
    XML_PARSER_SYSTEM_LITERAL,	
    XML_PARSER_EPILOG, 		
    XML_PARSER_IGNORE,		
    XML_PARSER_PUBLIC_LITERAL 	
} xmlParserInputState;

#define XML_DETECT_IDS		2

#define XML_COMPLETE_ATTRS	4

#define XML_SKIP_IDS		8

typedef enum {
    XML_PARSE_UNKNOWN = 0,
    XML_PARSE_DOM = 1,
    XML_PARSE_SAX = 2,
    XML_PARSE_PUSH_DOM = 3,
    XML_PARSE_PUSH_SAX = 4,
    XML_PARSE_READER = 5
} xmlParserMode;

struct _xmlParserCtxt {
    struct _xmlSAXHandler *sax;       
    void            *userData;        
    xmlDocPtr           myDoc;        
    int            wellFormed;        
    int       replaceEntities;        
    const xmlChar    *version;        
    const xmlChar   *encoding;        
    int            standalone;        
    int                  html;        

    
    xmlParserInputPtr  input;         
    int                inputNr;       
    int                inputMax;      
    xmlParserInputPtr *inputTab;      

    
    xmlNodePtr         node;          
    int                nodeNr;        
    int                nodeMax;       
    xmlNodePtr        *nodeTab;       

    int record_info;                  
    xmlParserNodeInfoSeq node_seq;    

    int errNo;                        

    int     hasExternalSubset;        
    int             hasPErefs;        
    int              external;        

    int                 valid;        
    int              validate;        
    xmlValidCtxt        vctxt;        

    xmlParserInputState instate;      
    int                 token;            

    char           *directory;        

    
    const xmlChar     *name;          
    int                nameNr;        
    int                nameMax;       
    const xmlChar *   *nameTab;       

    long               nbChars;       
    long            checkIndex;       
    int             keepBlanks;       
    int             disableSAX;       
    int               inSubset;       
    const xmlChar *    intSubName;    
    xmlChar *          extSubURI;     
    xmlChar *          extSubSystem;  

    
    int *              space;         
    int                spaceNr;       
    int                spaceMax;      
    int *              spaceTab;      

    int                depth;         
    xmlParserInputPtr  entity;        
    int                charset;       
    int                nodelen;       
    int                nodemem;       
    int                pedantic;      
    void              *_private;      

    int                loadsubset;    
    int                linenumbers;   
    void              *catalogs;      
    int                recovery;      
    int                progressive;   
    xmlDictPtr         dict;          
    const xmlChar *   *atts;          
    int                maxatts;       
    int                docdict;       

    const xmlChar *str_xml;
    const xmlChar *str_xmlns;
    const xmlChar *str_xml_ns;

    int                sax2;          
    int                nsNr;          
    int                nsMax;         
    const xmlChar *   *nsTab;         
    int               *attallocs;     
    void *            *pushTab;       
    xmlHashTablePtr    attsDefault;   
    xmlHashTablePtr    attsSpecial;   
    int                nsWellFormed;  
    int                options;       

    int               dictNames;    
    int               freeElemsNr;  
    xmlNodePtr        freeElems;    
    int               freeAttrsNr;  
    xmlAttrPtr        freeAttrs;    

    xmlError          lastError;
    xmlParserMode     parseMode;    
    unsigned long    nbentities;    
    unsigned long  sizeentities;    

    
    xmlParserNodeInfo *nodeInfo;      
    int                nodeInfoNr;    
    int                nodeInfoMax;   
    xmlParserNodeInfo *nodeInfoTab;   
};

struct _xmlSAXLocator {
    const xmlChar *(*getPublicId)(void *ctx);
    const xmlChar *(*getSystemId)(void *ctx);
    int (*getLineNumber)(void *ctx);
    int (*getColumnNumber)(void *ctx);
};


typedef xmlParserInputPtr (*resolveEntitySAXFunc) (void *ctx,
				const xmlChar *publicId,
				const xmlChar *systemId);
typedef void (*internalSubsetSAXFunc) (void *ctx,
				const xmlChar *name,
				const xmlChar *ExternalID,
				const xmlChar *SystemID);
typedef void (*externalSubsetSAXFunc) (void *ctx,
				const xmlChar *name,
				const xmlChar *ExternalID,
				const xmlChar *SystemID);
typedef xmlEntityPtr (*getEntitySAXFunc) (void *ctx,
				const xmlChar *name);
typedef xmlEntityPtr (*getParameterEntitySAXFunc) (void *ctx,
				const xmlChar *name);
typedef void (*entityDeclSAXFunc) (void *ctx,
				const xmlChar *name,
				int type,
				const xmlChar *publicId,
				const xmlChar *systemId,
				xmlChar *content);
typedef void (*notationDeclSAXFunc)(void *ctx,
				const xmlChar *name,
				const xmlChar *publicId,
				const xmlChar *systemId);
typedef void (*attributeDeclSAXFunc)(void *ctx,
				const xmlChar *elem,
				const xmlChar *fullname,
				int type,
				int def,
				const xmlChar *defaultValue,
				xmlEnumerationPtr tree);
typedef void (*elementDeclSAXFunc)(void *ctx,
				const xmlChar *name,
				int type,
				xmlElementContentPtr content);
typedef void (*unparsedEntityDeclSAXFunc)(void *ctx,
				const xmlChar *name,
				const xmlChar *publicId,
				const xmlChar *systemId,
				const xmlChar *notationName);
typedef void (*setDocumentLocatorSAXFunc) (void *ctx,
				xmlSAXLocatorPtr loc);
typedef void (*startDocumentSAXFunc) (void *ctx);
typedef void (*endDocumentSAXFunc) (void *ctx);
typedef void (*startElementSAXFunc) (void *ctx,
				const xmlChar *name,
				const xmlChar **atts);
typedef void (*endElementSAXFunc) (void *ctx,
				const xmlChar *name);
typedef void (*attributeSAXFunc) (void *ctx,
				const xmlChar *name,
				const xmlChar *value);
typedef void (*referenceSAXFunc) (void *ctx,
				const xmlChar *name);
typedef void (*charactersSAXFunc) (void *ctx,
				const xmlChar *ch,
				int len);
typedef void (*ignorableWhitespaceSAXFunc) (void *ctx,
				const xmlChar *ch,
				int len);
typedef void (*processingInstructionSAXFunc) (void *ctx,
				const xmlChar *target,
				const xmlChar *data);
typedef void (*commentSAXFunc) (void *ctx,
				const xmlChar *value);
typedef void (*cdataBlockSAXFunc) (
	                        void *ctx,
				const xmlChar *value,
				int len);
typedef void (XMLCDECL *warningSAXFunc) (void *ctx,
				const char *msg, ...) LIBXML_ATTR_FORMAT(2,3);
typedef void (XMLCDECL *errorSAXFunc) (void *ctx,
				const char *msg, ...) LIBXML_ATTR_FORMAT(2,3);
typedef void (XMLCDECL *fatalErrorSAXFunc) (void *ctx,
				const char *msg, ...) LIBXML_ATTR_FORMAT(2,3);
typedef int (*isStandaloneSAXFunc) (void *ctx);
typedef int (*hasInternalSubsetSAXFunc) (void *ctx);

typedef int (*hasExternalSubsetSAXFunc) (void *ctx);

#define XML_SAX2_MAGIC 0xDEEDBEAF


typedef void (*startElementNsSAX2Func) (void *ctx,
					const xmlChar *localname,
					const xmlChar *prefix,
					const xmlChar *URI,
					int nb_namespaces,
					const xmlChar **namespaces,
					int nb_attributes,
					int nb_defaulted,
					const xmlChar **attributes);
 

typedef void (*endElementNsSAX2Func)   (void *ctx,
					const xmlChar *localname,
					const xmlChar *prefix,
					const xmlChar *URI);


struct _xmlSAXHandler {
    internalSubsetSAXFunc internalSubset;
    isStandaloneSAXFunc isStandalone;
    hasInternalSubsetSAXFunc hasInternalSubset;
    hasExternalSubsetSAXFunc hasExternalSubset;
    resolveEntitySAXFunc resolveEntity;
    getEntitySAXFunc getEntity;
    entityDeclSAXFunc entityDecl;
    notationDeclSAXFunc notationDecl;
    attributeDeclSAXFunc attributeDecl;
    elementDeclSAXFunc elementDecl;
    unparsedEntityDeclSAXFunc unparsedEntityDecl;
    setDocumentLocatorSAXFunc setDocumentLocator;
    startDocumentSAXFunc startDocument;
    endDocumentSAXFunc endDocument;
    startElementSAXFunc startElement;
    endElementSAXFunc endElement;
    referenceSAXFunc reference;
    charactersSAXFunc characters;
    ignorableWhitespaceSAXFunc ignorableWhitespace;
    processingInstructionSAXFunc processingInstruction;
    commentSAXFunc comment;
    warningSAXFunc warning;
    errorSAXFunc error;
    fatalErrorSAXFunc fatalError; 
    getParameterEntitySAXFunc getParameterEntity;
    cdataBlockSAXFunc cdataBlock;
    externalSubsetSAXFunc externalSubset;
    unsigned int initialized;
    
    void *_private;
    startElementNsSAX2Func startElementNs;
    endElementNsSAX2Func endElementNs;
    xmlStructuredErrorFunc serror;
};

typedef struct _xmlSAXHandlerV1 xmlSAXHandlerV1;
typedef xmlSAXHandlerV1 *xmlSAXHandlerV1Ptr;
struct _xmlSAXHandlerV1 {
    internalSubsetSAXFunc internalSubset;
    isStandaloneSAXFunc isStandalone;
    hasInternalSubsetSAXFunc hasInternalSubset;
    hasExternalSubsetSAXFunc hasExternalSubset;
    resolveEntitySAXFunc resolveEntity;
    getEntitySAXFunc getEntity;
    entityDeclSAXFunc entityDecl;
    notationDeclSAXFunc notationDecl;
    attributeDeclSAXFunc attributeDecl;
    elementDeclSAXFunc elementDecl;
    unparsedEntityDeclSAXFunc unparsedEntityDecl;
    setDocumentLocatorSAXFunc setDocumentLocator;
    startDocumentSAXFunc startDocument;
    endDocumentSAXFunc endDocument;
    startElementSAXFunc startElement;
    endElementSAXFunc endElement;
    referenceSAXFunc reference;
    charactersSAXFunc characters;
    ignorableWhitespaceSAXFunc ignorableWhitespace;
    processingInstructionSAXFunc processingInstruction;
    commentSAXFunc comment;
    warningSAXFunc warning;
    errorSAXFunc error;
    fatalErrorSAXFunc fatalError; 
    getParameterEntitySAXFunc getParameterEntity;
    cdataBlockSAXFunc cdataBlock;
    externalSubsetSAXFunc externalSubset;
    unsigned int initialized;
};


typedef xmlParserInputPtr (*xmlExternalEntityLoader) (const char *URL,
					 const char *ID,
					 xmlParserCtxtPtr context);

#ifdef __cplusplus
}
#endif

#include <libxml/encoding.h>
#include <libxml/xmlIO.h>
#include <libxml/globals.h>

#ifdef __cplusplus
extern "C" {
#endif


XMLPUBFUN void XMLCALL		
		xmlInitParser		(void);
XMLPUBFUN void XMLCALL		
		xmlCleanupParser	(void);

XMLPUBFUN int XMLCALL		
		xmlParserInputRead	(xmlParserInputPtr in,
					 int len);
XMLPUBFUN int XMLCALL		
		xmlParserInputGrow	(xmlParserInputPtr in,
					 int len);

#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlParseDoc		(const xmlChar *cur);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlParseFile		(const char *filename);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlParseMemory		(const char *buffer,
					 int size);
#endif 
XMLPUBFUN int XMLCALL		
		xmlSubstituteEntitiesDefault(int val);
XMLPUBFUN int XMLCALL		
		xmlKeepBlanksDefault	(int val);
XMLPUBFUN void XMLCALL		
		xmlStopParser		(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL		
		xmlPedanticParserDefault(int val);
XMLPUBFUN int XMLCALL		
		xmlLineNumbersDefault	(int val);

#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlRecoverDoc		(const xmlChar *cur);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlRecoverMemory	(const char *buffer,
					 int size);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlRecoverFile		(const char *filename);
#endif 

XMLPUBFUN int XMLCALL		
		xmlParseDocument	(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL		
		xmlParseExtParsedEnt	(xmlParserCtxtPtr ctxt);
#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN int XMLCALL		
		xmlSAXUserParseFile	(xmlSAXHandlerPtr sax,
					 void *user_data,
					 const char *filename);
XMLPUBFUN int XMLCALL		
		xmlSAXUserParseMemory	(xmlSAXHandlerPtr sax,
					 void *user_data,
					 const char *buffer,
					 int size);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlSAXParseDoc		(xmlSAXHandlerPtr sax,
					 const xmlChar *cur,
					 int recovery);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlSAXParseMemory	(xmlSAXHandlerPtr sax,
					 const char *buffer,
                                   	 int size,
					 int recovery);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlSAXParseMemoryWithData (xmlSAXHandlerPtr sax,
					 const char *buffer,
                                   	 int size,
					 int recovery,
					 void *data);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlSAXParseFile		(xmlSAXHandlerPtr sax,
					 const char *filename,
					 int recovery);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlSAXParseFileWithData	(xmlSAXHandlerPtr sax,
					 const char *filename,
					 int recovery,
					 void *data);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlSAXParseEntity	(xmlSAXHandlerPtr sax,
					 const char *filename);
XMLPUBFUN xmlDocPtr XMLCALL	
		xmlParseEntity		(const char *filename);
#endif 

#ifdef LIBXML_VALID_ENABLED
XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlSAXParseDTD		(xmlSAXHandlerPtr sax,
					 const xmlChar *ExternalID,
					 const xmlChar *SystemID);
XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlParseDTD		(const xmlChar *ExternalID,
					 const xmlChar *SystemID);
XMLPUBFUN xmlDtdPtr XMLCALL	
		xmlIOParseDTD		(xmlSAXHandlerPtr sax,
					 xmlParserInputBufferPtr input,
					 xmlCharEncoding enc);
#endif 
#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN int XMLCALL	
		xmlParseBalancedChunkMemory(xmlDocPtr doc,
					 xmlSAXHandlerPtr sax,
					 void *user_data,
					 int depth,
					 const xmlChar *string,
					 xmlNodePtr *lst);
#endif 
XMLPUBFUN xmlParserErrors XMLCALL
		xmlParseInNodeContext	(xmlNodePtr node,
					 const char *data,
					 int datalen,
					 int options,
					 xmlNodePtr *lst);
#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN int XMLCALL          
		xmlParseBalancedChunkMemoryRecover(xmlDocPtr doc,
                     xmlSAXHandlerPtr sax,
                     void *user_data,
                     int depth,
                     const xmlChar *string,
                     xmlNodePtr *lst,
                     int recover);
XMLPUBFUN int XMLCALL		
		xmlParseExternalEntity	(xmlDocPtr doc,
					 xmlSAXHandlerPtr sax,
					 void *user_data,
					 int depth,
					 const xmlChar *URL,
					 const xmlChar *ID,
					 xmlNodePtr *lst);
#endif 
XMLPUBFUN int XMLCALL		
		xmlParseCtxtExternalEntity(xmlParserCtxtPtr ctx,
					 const xmlChar *URL,
					 const xmlChar *ID,
					 xmlNodePtr *lst);

XMLPUBFUN xmlParserCtxtPtr XMLCALL	
		xmlNewParserCtxt	(void);
XMLPUBFUN int XMLCALL		
		xmlInitParserCtxt	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL		
		xmlClearParserCtxt	(xmlParserCtxtPtr ctxt);
XMLPUBFUN void XMLCALL		
		xmlFreeParserCtxt	(xmlParserCtxtPtr ctxt);
#ifdef LIBXML_SAX1_ENABLED
XMLPUBFUN void XMLCALL		
		xmlSetupParserForBuffer	(xmlParserCtxtPtr ctxt,
					 const xmlChar* buffer,
					 const char *filename);
#endif 
XMLPUBFUN xmlParserCtxtPtr XMLCALL 
		xmlCreateDocParserCtxt	(const xmlChar *cur);

#ifdef LIBXML_LEGACY_ENABLED
XMLPUBFUN int XMLCALL		
		xmlGetFeaturesList	(int *len,
					 const char **result);
XMLPUBFUN int XMLCALL		
		xmlGetFeature		(xmlParserCtxtPtr ctxt,
					 const char *name,
					 void *result);
XMLPUBFUN int XMLCALL		
		xmlSetFeature		(xmlParserCtxtPtr ctxt,
					 const char *name,
					 void *value);
#endif 

#ifdef LIBXML_PUSH_ENABLED
XMLPUBFUN xmlParserCtxtPtr XMLCALL 
		xmlCreatePushParserCtxt(xmlSAXHandlerPtr sax,
					 void *user_data,
					 const char *chunk,
					 int size,
					 const char *filename);
XMLPUBFUN int XMLCALL		 
		xmlParseChunk		(xmlParserCtxtPtr ctxt,
					 const char *chunk,
					 int size,
					 int terminate);
#endif 


XMLPUBFUN xmlParserCtxtPtr XMLCALL 
		xmlCreateIOParserCtxt	(xmlSAXHandlerPtr sax,
					 void *user_data,
					 xmlInputReadCallback   ioread,
					 xmlInputCloseCallback  ioclose,
					 void *ioctx,
					 xmlCharEncoding enc);

XMLPUBFUN xmlParserInputPtr XMLCALL 
		xmlNewIOInputStream	(xmlParserCtxtPtr ctxt,
					 xmlParserInputBufferPtr input,
					 xmlCharEncoding enc);

XMLPUBFUN const xmlParserNodeInfo* XMLCALL
		xmlParserFindNodeInfo	(const xmlParserCtxtPtr ctxt,
				         const xmlNodePtr node);
XMLPUBFUN void XMLCALL		
		xmlInitNodeInfoSeq	(xmlParserNodeInfoSeqPtr seq);
XMLPUBFUN void XMLCALL		
		xmlClearNodeInfoSeq	(xmlParserNodeInfoSeqPtr seq);
XMLPUBFUN unsigned long XMLCALL 
		xmlParserFindNodeInfoIndex(const xmlParserNodeInfoSeqPtr seq,
                                         const xmlNodePtr node);
XMLPUBFUN void XMLCALL		
		xmlParserAddNodeInfo	(xmlParserCtxtPtr ctxt,
					 const xmlParserNodeInfoPtr info);


XMLPUBFUN void XMLCALL		
		xmlSetExternalEntityLoader(xmlExternalEntityLoader f);
XMLPUBFUN xmlExternalEntityLoader XMLCALL
		xmlGetExternalEntityLoader(void);
XMLPUBFUN xmlParserInputPtr XMLCALL
		xmlLoadExternalEntity	(const char *URL,
					 const char *ID,
					 xmlParserCtxtPtr ctxt);

XMLPUBFUN long XMLCALL
		xmlByteConsumed		(xmlParserCtxtPtr ctxt);

typedef enum {
    XML_PARSE_RECOVER	= 1<<0,	
    XML_PARSE_NOENT	= 1<<1,	
    XML_PARSE_DTDLOAD	= 1<<2,	
    XML_PARSE_DTDATTR	= 1<<3,	
    XML_PARSE_DTDVALID	= 1<<4,	
    XML_PARSE_NOERROR	= 1<<5,	
    XML_PARSE_NOWARNING	= 1<<6,	
    XML_PARSE_PEDANTIC	= 1<<7,	
    XML_PARSE_NOBLANKS	= 1<<8,	
    XML_PARSE_SAX1	= 1<<9,	
    XML_PARSE_XINCLUDE	= 1<<10,
    XML_PARSE_NONET	= 1<<11,
    XML_PARSE_NODICT	= 1<<12,
    XML_PARSE_NSCLEAN	= 1<<13,
    XML_PARSE_NOCDATA	= 1<<14,
    XML_PARSE_NOXINCNODE= 1<<15,
    XML_PARSE_COMPACT   = 1<<16,
    XML_PARSE_OLD10	= 1<<17,
    XML_PARSE_NOBASEFIX = 1<<18,
    XML_PARSE_HUGE      = 1<<19, 
    XML_PARSE_OLDSAX    = 1<<20 
} xmlParserOption;

XMLPUBFUN void XMLCALL
		xmlCtxtReset		(xmlParserCtxtPtr ctxt);
XMLPUBFUN int XMLCALL
		xmlCtxtResetPush	(xmlParserCtxtPtr ctxt,
					 const char *chunk,
					 int size,
					 const char *filename,
					 const char *encoding);
XMLPUBFUN int XMLCALL
		xmlCtxtUseOptions	(xmlParserCtxtPtr ctxt,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlReadDoc		(const xmlChar *cur,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlReadFile		(const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlReadMemory		(const char *buffer,
					 int size,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlReadFd		(int fd,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlReadIO		(xmlInputReadCallback ioread,
					 xmlInputCloseCallback ioclose,
					 void *ioctx,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlCtxtReadDoc		(xmlParserCtxtPtr ctxt,
					 const xmlChar *cur,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlCtxtReadFile		(xmlParserCtxtPtr ctxt,
					 const char *filename,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlCtxtReadMemory		(xmlParserCtxtPtr ctxt,
					 const char *buffer,
					 int size,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlCtxtReadFd		(xmlParserCtxtPtr ctxt,
					 int fd,
					 const char *URL,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlDocPtr XMLCALL
		xmlCtxtReadIO		(xmlParserCtxtPtr ctxt,
					 xmlInputReadCallback ioread,
					 xmlInputCloseCallback ioclose,
					 void *ioctx,
					 const char *URL,
					 const char *encoding,
					 int options);

typedef enum {
    XML_WITH_THREAD = 1,
    XML_WITH_TREE = 2,
    XML_WITH_OUTPUT = 3,
    XML_WITH_PUSH = 4,
    XML_WITH_READER = 5,
    XML_WITH_PATTERN = 6,
    XML_WITH_WRITER = 7,
    XML_WITH_SAX1 = 8,
    XML_WITH_FTP = 9,
    XML_WITH_HTTP = 10,
    XML_WITH_VALID = 11,
    XML_WITH_HTML = 12,
    XML_WITH_LEGACY = 13,
    XML_WITH_C14N = 14,
    XML_WITH_CATALOG = 15,
    XML_WITH_XPATH = 16,
    XML_WITH_XPTR = 17,
    XML_WITH_XINCLUDE = 18,
    XML_WITH_ICONV = 19,
    XML_WITH_ISO8859X = 20,
    XML_WITH_UNICODE = 21,
    XML_WITH_REGEXP = 22,
    XML_WITH_AUTOMATA = 23,
    XML_WITH_EXPR = 24,
    XML_WITH_SCHEMAS = 25,
    XML_WITH_SCHEMATRON = 26,
    XML_WITH_MODULES = 27,
    XML_WITH_DEBUG = 28,
    XML_WITH_DEBUG_MEM = 29,
    XML_WITH_DEBUG_RUN = 30,
    XML_WITH_ZLIB = 31,
    XML_WITH_ICU = 32,
    XML_WITH_NONE = 99999 
} xmlFeature;

XMLPUBFUN int XMLCALL
		xmlHasFeature		(xmlFeature feature);

#ifdef __cplusplus
}
#endif
#endif 

