/*
 * schemas.c : implementation of the XML Schema handling and
 *             schema validity checking
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <veillard@redhat.com>
 */

#define IN_LIBXML
#include "libxml.h"

#ifdef LIBXML_SCHEMAS_ENABLED

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/hash.h>
#include <libxml/uri.h>
#include <libxml/xmlschemas.h>
#include <libxml/schemasInternals.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlautomata.h>
#include <libxml/xmlregexp.h>
#include <libxml/dict.h>
#include <libxml/encoding.h>
#include <libxml/xmlIO.h>
#ifdef LIBXML_PATTERN_ENABLED
#include <libxml/pattern.h>
#endif
#ifdef LIBXML_READER_ENABLED
#include <libxml/xmlreader.h>
#endif









#ifdef DEBUG_IDC
 #ifndef DEBUG_IDC_NODE_TABLE
  #define DEBUG_IDC_NODE_TABLE
 #endif
#endif


#define ENABLE_REDEFINE



#define DUMP_CONTENT_MODEL

#ifdef LIBXML_READER_ENABLED
#endif

#define UNBOUNDED (1 << 30)
#define TODO 								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);

#define XML_SCHEMAS_NO_NAMESPACE (const xmlChar *) "##"

static const xmlChar *xmlSchemaNs = (const xmlChar *)
    "http://www.w3.org/2001/XMLSchema";

static const xmlChar *xmlSchemaInstanceNs = (const xmlChar *)
    "http://www.w3.org/2001/XMLSchema-instance";

static const xmlChar *xmlNamespaceNs = (const xmlChar *)
    "http://www.w3.org/2000/xmlns/";

#define ACTXT_CAST (xmlSchemaAbstractCtxtPtr)
#define PCTXT_CAST (xmlSchemaParserCtxtPtr)
#define VCTXT_CAST (xmlSchemaValidCtxtPtr)
#define WXS_BASIC_CAST (xmlSchemaBasicItemPtr)
#define WXS_TREE_CAST (xmlSchemaTreeItemPtr)
#define WXS_PTC_CAST (xmlSchemaParticlePtr)
#define WXS_TYPE_CAST (xmlSchemaTypePtr)
#define WXS_ELEM_CAST (xmlSchemaElementPtr)
#define WXS_ATTR_GROUP_CAST (xmlSchemaAttributeGroupPtr)
#define WXS_ATTR_CAST (xmlSchemaAttributePtr)
#define WXS_ATTR_USE_CAST (xmlSchemaAttributeUsePtr)
#define WXS_ATTR_PROHIB_CAST (xmlSchemaAttributeUseProhibPtr)
#define WXS_MODEL_GROUPDEF_CAST (xmlSchemaModelGroupDefPtr)
#define WXS_MODEL_GROUP_CAST (xmlSchemaModelGroupPtr)
#define WXS_IDC_CAST (xmlSchemaIDCPtr)
#define WXS_QNAME_CAST (xmlSchemaQNameRefPtr)
#define WXS_LIST_CAST (xmlSchemaItemListPtr)

#define WXS_ITEM_NODE(i) xmlSchemaGetComponentNode(WXS_BASIC_CAST (i))

#define WXS_ITEM_TYPE_NAME(i) xmlSchemaGetComponentTypeStr(WXS_BASIC_CAST (i))
#define WXS_ELEM_TYPEDEF(e) (e)->subtypes

#define WXS_SUBST_HEAD(item) (item)->refDecl
#define WXS_ATTR_TYPEDEF(a) (a)->subtypes
#define WXS_ATTRUSE_DECL(au) WXS_ATTR_CAST (WXS_ATTR_USE_CAST (au))->attrDecl

#define WXS_ATTRUSE_TYPEDEF(au) WXS_ATTR_TYPEDEF(WXS_ATTRUSE_DECL( WXS_ATTR_USE_CAST au))

#define WXS_ATTRUSE_DECL_NAME(au) (WXS_ATTRUSE_DECL(au))->name

#define WXS_ATTRUSE_DECL_TNS(au) (WXS_ATTRUSE_DECL(au))->targetNamespace
#define WXS_ATTR_GROUP_HAS_REFS(ag) ((WXS_ATTR_GROUP_CAST (ag))->flags & XML_SCHEMAS_ATTRGROUP_HAS_REFS)
#define WXS_ATTR_GROUP_EXPANDED(ag) ((WXS_ATTR_GROUP_CAST (ag))->flags & XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED)
#define WXS_PARTICLE(p) WXS_PTC_CAST (p)

#define WXS_PARTICLE_TERM(p) (WXS_PARTICLE(p))->children

#define WXS_PARTICLE_TERM_AS_ELEM(p) (WXS_ELEM_CAST WXS_PARTICLE_TERM(p))

#define WXS_PARTICLE_MODEL(p) WXS_MODEL_GROUP_CAST WXS_PARTICLE(p)->children
#define WXS_MODELGROUPDEF_MODEL(mgd) (WXS_MODEL_GROUP_CAST (mgd))->children
#define WXS_IS_MODEL_GROUP(i) \
    (((i)->type == XML_SCHEMA_TYPE_SEQUENCE) || \
     ((i)->type == XML_SCHEMA_TYPE_CHOICE) || \
     ((i)->type == XML_SCHEMA_TYPE_ALL))

#define WXS_MODELGROUP_PARTICLE(mg) WXS_PTC_CAST (mg)->children
#define WXS_IS_BUCKET_INCREDEF(t) (((t) == XML_SCHEMA_SCHEMA_INCLUDE) || \
    ((t) == XML_SCHEMA_SCHEMA_REDEFINE))

#define WXS_IS_BUCKET_IMPMAIN(t) (((t) == XML_SCHEMA_SCHEMA_MAIN) || \
    ((t) == XML_SCHEMA_SCHEMA_IMPORT))

#define WXS_IMPBUCKET(b) ((xmlSchemaImportPtr) (b))

#define WXS_INCBUCKET(b) ((xmlSchemaIncludePtr) (b))
#define WXS_IS_ANYTYPE(i) \
     (( (i)->type == XML_SCHEMA_TYPE_BASIC) && \
      ( (WXS_TYPE_CAST (i))->builtInType == XML_SCHEMAS_ANYTYPE))

#define WXS_IS_COMPLEX(i) \
    (((i)->type == XML_SCHEMA_TYPE_COMPLEX) || \
     ((i)->builtInType == XML_SCHEMAS_ANYTYPE))

#define WXS_IS_SIMPLE(item) \
    ((item->type == XML_SCHEMA_TYPE_SIMPLE) || \
     ((item->type == XML_SCHEMA_TYPE_BASIC) && \
      (item->builtInType != XML_SCHEMAS_ANYTYPE)))

#define WXS_IS_ANY_SIMPLE_TYPE(i) \
    (((i)->type == XML_SCHEMA_TYPE_BASIC) && \
      ((i)->builtInType == XML_SCHEMAS_ANYSIMPLETYPE))

#define WXS_IS_RESTRICTION(t) \
    ((t)->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION)

#define WXS_IS_EXTENSION(t) \
    ((t)->flags & XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION)

#define WXS_IS_TYPE_NOT_FIXED(i) \
    (((i)->type != XML_SCHEMA_TYPE_BASIC) && \
     (((i)->flags & XML_SCHEMAS_TYPE_INTERNAL_RESOLVED) == 0))

#define WXS_IS_TYPE_NOT_FIXED_1(item) \
    (((item)->type != XML_SCHEMA_TYPE_BASIC) && \
     (((item)->flags & XML_SCHEMAS_TYPE_FIXUP_1) == 0))

#define WXS_TYPE_IS_GLOBAL(t) ((t)->flags & XML_SCHEMAS_TYPE_GLOBAL)

#define WXS_TYPE_IS_LOCAL(t) (((t)->flags & XML_SCHEMAS_TYPE_GLOBAL) == 0)
#define WXS_HAS_COMPLEX_CONTENT(item) \
    ((item->contentType == XML_SCHEMA_CONTENT_MIXED) || \
     (item->contentType == XML_SCHEMA_CONTENT_EMPTY) || \
     (item->contentType == XML_SCHEMA_CONTENT_ELEMENTS))

#define WXS_HAS_SIMPLE_CONTENT(item) \
    ((item->contentType == XML_SCHEMA_CONTENT_SIMPLE) || \
     (item->contentType == XML_SCHEMA_CONTENT_BASIC))

#define WXS_HAS_MIXED_CONTENT(item) \
    (item->contentType == XML_SCHEMA_CONTENT_MIXED)

#define WXS_EMPTIABLE(t) \
    (xmlSchemaIsParticleEmptiable(WXS_PTC_CAST (t)->subtypes))

#define WXS_TYPE_CONTENTTYPE(t) (t)->subtypes

#define WXS_TYPE_PARTICLE(t) WXS_PTC_CAST (t)->subtypes

#define WXS_TYPE_PARTICLE_TERM(t) WXS_PARTICLE_TERM(WXS_TYPE_PARTICLE(t))
#define WXS_LIST_ITEMTYPE(t) (t)->subtypes

#define WXS_IS_ATOMIC(t) (t->flags & XML_SCHEMAS_TYPE_VARIETY_ATOMIC)

#define WXS_IS_LIST(t) (t->flags & XML_SCHEMAS_TYPE_VARIETY_LIST)

#define WXS_IS_UNION(t) (t->flags & XML_SCHEMAS_TYPE_VARIETY_UNION)
#define WXS_CONSTRUCTOR(ctx) (ctx)->constructor

#define WXS_HAS_BUCKETS(ctx) \
( (WXS_CONSTRUCTOR((ctx))->buckets != NULL) && \
(WXS_CONSTRUCTOR((ctx))->buckets->nbItems > 0) )

#define WXS_SUBST_GROUPS(ctx) WXS_CONSTRUCTOR((ctx))->substGroups

#define WXS_BUCKET(ctx) WXS_CONSTRUCTOR((ctx))->bucket

#define WXS_SCHEMA(ctx) (ctx)->schema

#define WXS_ADD_LOCAL(ctx, item) \
    xmlSchemaAddItemSize(&(WXS_BUCKET(ctx)->locals), 10, item)

#define WXS_ADD_GLOBAL(ctx, item) \
    xmlSchemaAddItemSize(&(WXS_BUCKET(ctx)->globals), 5, item)

#define WXS_ADD_PENDING(ctx, item) \
    xmlSchemaAddItemSize(&((ctx)->constructor->pending), 10, item)
#define WXS_ILIST_IS_EMPTY(l) ((l == NULL) || ((l)->nbItems == 0))
#define IS_SCHEMA(node, type) \
   ((node != NULL) && (node->ns != NULL) && \
    (xmlStrEqual(node->name, (const xmlChar *) type)) && \
    (xmlStrEqual(node->ns->href, xmlSchemaNs)))

#define FREE_AND_NULL(str) if ((str) != NULL) { xmlFree((xmlChar *) (str)); str = NULL; }

#define WXS_ARE_DEFAULT_STR_EQUAL(v1, v2) ((v1) == (v2))

#define INODE_NILLED(item) (item->flags & XML_SCHEMA_ELEM_INFO_NILLED)

#define CAN_PARSE_SCHEMA(b) (((b)->doc != NULL) && ((b)->parsed == 0))

#define HFAILURE if (res == -1) goto exit_failure;

#define HERROR if (res != 0) goto exit_error;

#define HSTOP(ctx) if ((ctx)->stop) goto exit;
#define SUBSET_RESTRICTION  1<<0
#define SUBSET_EXTENSION    1<<1
#define SUBSET_SUBSTITUTION 1<<2
#define SUBSET_LIST         1<<3
#define SUBSET_UNION        1<<4

typedef struct _xmlSchemaNodeInfo xmlSchemaNodeInfo;
typedef xmlSchemaNodeInfo *xmlSchemaNodeInfoPtr;

typedef struct _xmlSchemaItemList xmlSchemaItemList;
typedef xmlSchemaItemList *xmlSchemaItemListPtr;
struct _xmlSchemaItemList {
    void **items;  
    int nbItems; 
    int sizeItems; 
};

#define XML_SCHEMA_CTXT_PARSER 1
#define XML_SCHEMA_CTXT_VALIDATOR 2

typedef struct _xmlSchemaAbstractCtxt xmlSchemaAbstractCtxt;
typedef xmlSchemaAbstractCtxt *xmlSchemaAbstractCtxtPtr;
struct _xmlSchemaAbstractCtxt {
    int type; 
};

typedef struct _xmlSchemaBucket xmlSchemaBucket;
typedef xmlSchemaBucket *xmlSchemaBucketPtr;

#define XML_SCHEMA_SCHEMA_MAIN 0
#define XML_SCHEMA_SCHEMA_IMPORT 1
#define XML_SCHEMA_SCHEMA_INCLUDE 2
#define XML_SCHEMA_SCHEMA_REDEFINE 3

typedef struct _xmlSchemaSchemaRelation xmlSchemaSchemaRelation;
typedef xmlSchemaSchemaRelation *xmlSchemaSchemaRelationPtr;
struct _xmlSchemaSchemaRelation {
    xmlSchemaSchemaRelationPtr next;
    int type; 
    const xmlChar *importNamespace;
    xmlSchemaBucketPtr bucket;
};

#define XML_SCHEMA_BUCKET_MARKED 1<<0
#define XML_SCHEMA_BUCKET_COMPS_ADDED 1<<1

struct _xmlSchemaBucket {
    int type;
    int flags;
    const xmlChar *schemaLocation;
    const xmlChar *origTargetNamespace;
    const xmlChar *targetNamespace;
    xmlDocPtr doc;
    xmlSchemaSchemaRelationPtr relations;
    int located;
    int parsed;
    int imported;
    int preserveDoc;
    xmlSchemaItemListPtr globals; 
    xmlSchemaItemListPtr locals; 
};

typedef struct _xmlSchemaImport xmlSchemaImport;
typedef xmlSchemaImport *xmlSchemaImportPtr;
struct _xmlSchemaImport {
    int type; 
    int flags;
    const xmlChar *schemaLocation; 
    
    const xmlChar *origTargetNamespace;
    const xmlChar *targetNamespace;
    xmlDocPtr doc; 
    
    xmlSchemaSchemaRelationPtr relations;
    int located;
    int parsed;
    int imported;
    int preserveDoc;
    xmlSchemaItemListPtr globals;
    xmlSchemaItemListPtr locals;
    
    xmlSchemaPtr schema;
};

typedef struct _xmlSchemaInclude xmlSchemaInclude;
typedef xmlSchemaInclude *xmlSchemaIncludePtr;
struct _xmlSchemaInclude {
    int type;
    int flags;
    const xmlChar *schemaLocation;
    const xmlChar *origTargetNamespace;
    const xmlChar *targetNamespace;
    xmlDocPtr doc;
    xmlSchemaSchemaRelationPtr relations;
    int located;
    int parsed;
    int imported;
    int preserveDoc;
    xmlSchemaItemListPtr globals; 
    xmlSchemaItemListPtr locals; 

    
    xmlSchemaImportPtr ownerImport;
};

typedef struct _xmlSchemaBasicItem xmlSchemaBasicItem;
typedef xmlSchemaBasicItem *xmlSchemaBasicItemPtr;
struct _xmlSchemaBasicItem {
    xmlSchemaTypeType type;
};

typedef struct _xmlSchemaAnnotItem xmlSchemaAnnotItem;
typedef xmlSchemaAnnotItem *xmlSchemaAnnotItemPtr;
struct _xmlSchemaAnnotItem {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
};

typedef struct _xmlSchemaTreeItem xmlSchemaTreeItem;
typedef xmlSchemaTreeItem *xmlSchemaTreeItemPtr;
struct _xmlSchemaTreeItem {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next;
    xmlSchemaTreeItemPtr children;
};


#define XML_SCHEMA_ATTR_USE_FIXED 1<<0
typedef struct _xmlSchemaAttributeUse xmlSchemaAttributeUse;
typedef xmlSchemaAttributeUse *xmlSchemaAttributeUsePtr;
struct _xmlSchemaAttributeUse {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaAttributeUsePtr next; 
    xmlSchemaAttributePtr attrDecl;

    int flags;
    xmlNodePtr node;
    int occurs; 
    const xmlChar * defValue;
    xmlSchemaValPtr defVal;
};

typedef struct _xmlSchemaAttributeUseProhib xmlSchemaAttributeUseProhib;
typedef xmlSchemaAttributeUseProhib *xmlSchemaAttributeUseProhibPtr;
struct _xmlSchemaAttributeUseProhib {
    xmlSchemaTypeType type; 
    xmlNodePtr node;
    const xmlChar *name;
    const xmlChar *targetNamespace;
    int isRef;
};

typedef struct _xmlSchemaRedef xmlSchemaRedef;
typedef xmlSchemaRedef *xmlSchemaRedefPtr;
struct _xmlSchemaRedef {
    xmlSchemaRedefPtr next;
    xmlSchemaBasicItemPtr item; 
    xmlSchemaBasicItemPtr reference; 
    xmlSchemaBasicItemPtr target; 
    const xmlChar *refName; 
    const xmlChar *refTargetNs; 
    xmlSchemaBucketPtr targetBucket; 
};

typedef struct _xmlSchemaConstructionCtxt xmlSchemaConstructionCtxt;
typedef xmlSchemaConstructionCtxt *xmlSchemaConstructionCtxtPtr;
struct _xmlSchemaConstructionCtxt {
    xmlSchemaPtr mainSchema; 
    xmlSchemaBucketPtr mainBucket; 
    xmlDictPtr dict;
    xmlSchemaItemListPtr buckets; 
     
    xmlSchemaBucketPtr bucket; 
    xmlSchemaItemListPtr pending; 
    xmlHashTablePtr substGroups;
    xmlSchemaRedefPtr redefs;
    xmlSchemaRedefPtr lastRedef;
};

#define XML_SCHEMAS_PARSE_ERROR		1
#define SCHEMAS_PARSE_OPTIONS XML_PARSE_NOENT

struct _xmlSchemaParserCtxt {
    int type;
    void *errCtxt;             
    xmlSchemaValidityErrorFunc error;   
    xmlSchemaValidityWarningFunc warning;       
    int err;
    int nberrors;
    xmlStructuredErrorFunc serror;

    xmlSchemaConstructionCtxtPtr constructor;
    int ownsConstructor; 

    
    

    xmlSchemaPtr schema;        
    int counter;

    const xmlChar *URL;
    xmlDocPtr doc;
    int preserve;		

    const char *buffer;
    int size;

    xmlAutomataPtr am;
    xmlAutomataStatePtr start;
    xmlAutomataStatePtr end;
    xmlAutomataStatePtr state;

    xmlDictPtr dict;		
    xmlSchemaTypePtr ctxtType; 
    int options;
    xmlSchemaValidCtxtPtr vctxt;
    int isS4S;
    int isRedefine;
    int xsiAssemble;
    int stop; 
    const xmlChar *targetNamespace;
    xmlSchemaBucketPtr redefined; 

    xmlSchemaRedefPtr redef; 
    int redefCounter; 
    xmlSchemaItemListPtr attrProhibs;
};

typedef struct _xmlSchemaQNameRef xmlSchemaQNameRef;
typedef xmlSchemaQNameRef *xmlSchemaQNameRefPtr;
struct _xmlSchemaQNameRef {
    xmlSchemaTypeType type;
    xmlSchemaBasicItemPtr item; 
    xmlSchemaTypeType itemType;
    const xmlChar *name;
    const xmlChar *targetNamespace;
    xmlNodePtr node;
};

typedef struct _xmlSchemaParticle xmlSchemaParticle;
typedef xmlSchemaParticle *xmlSchemaParticlePtr;
struct _xmlSchemaParticle {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next; 
    xmlSchemaTreeItemPtr children; 
    int minOccurs;
    int maxOccurs;
    xmlNodePtr node;
};

typedef struct _xmlSchemaModelGroup xmlSchemaModelGroup;
typedef xmlSchemaModelGroup *xmlSchemaModelGroupPtr;
struct _xmlSchemaModelGroup {
    xmlSchemaTypeType type; 
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next; 
    xmlSchemaTreeItemPtr children; 
    xmlNodePtr node;
};

#define XML_SCHEMA_MODEL_GROUP_DEF_MARKED 1<<0
#define XML_SCHEMA_MODEL_GROUP_DEF_REDEFINED 1<<1
typedef struct _xmlSchemaModelGroupDef xmlSchemaModelGroupDef;
typedef xmlSchemaModelGroupDef *xmlSchemaModelGroupDefPtr;
struct _xmlSchemaModelGroupDef {
    xmlSchemaTypeType type; 
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next; 
    xmlSchemaTreeItemPtr children; 
    const xmlChar *name;
    const xmlChar *targetNamespace;
    xmlNodePtr node;
    int flags;
};

typedef struct _xmlSchemaIDC xmlSchemaIDC;
typedef xmlSchemaIDC *xmlSchemaIDCPtr;

typedef struct _xmlSchemaIDCSelect xmlSchemaIDCSelect;
typedef xmlSchemaIDCSelect *xmlSchemaIDCSelectPtr;
struct _xmlSchemaIDCSelect {
    xmlSchemaIDCSelectPtr next;
    xmlSchemaIDCPtr idc;
    int index; 
    const xmlChar *xpath; 
    void *xpathComp; 
};


struct _xmlSchemaIDC {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaIDCPtr next;
    xmlNodePtr node;
    const xmlChar *name;
    const xmlChar *targetNamespace;
    xmlSchemaIDCSelectPtr selector;
    xmlSchemaIDCSelectPtr fields;
    int nbFields;
    xmlSchemaQNameRefPtr ref;
};

typedef struct _xmlSchemaIDCAug xmlSchemaIDCAug;
typedef xmlSchemaIDCAug *xmlSchemaIDCAugPtr;
struct _xmlSchemaIDCAug {
    xmlSchemaIDCAugPtr next; 
    xmlSchemaIDCPtr def; 
    int keyrefDepth; 
};

typedef struct _xmlSchemaPSVIIDCKey xmlSchemaPSVIIDCKey;
typedef xmlSchemaPSVIIDCKey *xmlSchemaPSVIIDCKeyPtr;
struct _xmlSchemaPSVIIDCKey {
    xmlSchemaTypePtr type;
    xmlSchemaValPtr val;
};

typedef struct _xmlSchemaPSVIIDCNode xmlSchemaPSVIIDCNode;
typedef xmlSchemaPSVIIDCNode *xmlSchemaPSVIIDCNodePtr;
struct _xmlSchemaPSVIIDCNode {
    xmlNodePtr node;
    xmlSchemaPSVIIDCKeyPtr *keys;
    int nodeLine;
    int nodeQNameID;

};

typedef struct _xmlSchemaPSVIIDCBinding xmlSchemaPSVIIDCBinding;
typedef xmlSchemaPSVIIDCBinding *xmlSchemaPSVIIDCBindingPtr;
struct _xmlSchemaPSVIIDCBinding {
    xmlSchemaPSVIIDCBindingPtr next; 
    xmlSchemaIDCPtr definition; 
    xmlSchemaPSVIIDCNodePtr *nodeTable; 
    int nbNodes; 
    int sizeNodes; 
    xmlSchemaItemListPtr dupls;
};


#define XPATH_STATE_OBJ_TYPE_IDC_SELECTOR 1
#define XPATH_STATE_OBJ_TYPE_IDC_FIELD 2

#define XPATH_STATE_OBJ_MATCHES -2
#define XPATH_STATE_OBJ_BLOCKED -3

typedef struct _xmlSchemaIDCMatcher xmlSchemaIDCMatcher;
typedef xmlSchemaIDCMatcher *xmlSchemaIDCMatcherPtr;

typedef struct _xmlSchemaIDCStateObj xmlSchemaIDCStateObj;
typedef xmlSchemaIDCStateObj *xmlSchemaIDCStateObjPtr;
struct _xmlSchemaIDCStateObj {
    int type;
    xmlSchemaIDCStateObjPtr next; 
    int depth; 
    int *history; 
    int nbHistory;
    int sizeHistory;
    xmlSchemaIDCMatcherPtr matcher; 
    xmlSchemaIDCSelectPtr sel;
    void *xpathCtxt;
};

#define IDC_MATCHER 0

struct _xmlSchemaIDCMatcher {
    int type;
    int depth; 
    xmlSchemaIDCMatcherPtr next; 
    xmlSchemaIDCMatcherPtr nextCached; 
    xmlSchemaIDCAugPtr aidc; 
    int idcType;
    xmlSchemaPSVIIDCKeyPtr **keySeqs; 
    int sizeKeySeqs;
    xmlSchemaItemListPtr targets; 
};

#define XML_SCHEMA_NODE_INFO_FLAG_OWNED_NAMES  1<<0
#define XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES 1<<1
#define XML_SCHEMA_ELEM_INFO_NILLED	       1<<2
#define XML_SCHEMA_ELEM_INFO_LOCAL_TYPE	       1<<3

#define XML_SCHEMA_NODE_INFO_VALUE_NEEDED      1<<4
#define XML_SCHEMA_ELEM_INFO_EMPTY             1<<5
#define XML_SCHEMA_ELEM_INFO_HAS_CONTENT       1<<6

#define XML_SCHEMA_ELEM_INFO_HAS_ELEM_CONTENT  1<<7
#define XML_SCHEMA_ELEM_INFO_ERR_BAD_CONTENT  1<<8
#define XML_SCHEMA_NODE_INFO_ERR_NOT_EXPECTED  1<<9
#define XML_SCHEMA_NODE_INFO_ERR_BAD_TYPE  1<<10

struct _xmlSchemaNodeInfo {
    int nodeType;
    xmlNodePtr node;
    int nodeLine;
    const xmlChar *localName;
    const xmlChar *nsName;
    const xmlChar *value;
    xmlSchemaValPtr val; 
    xmlSchemaTypePtr typeDef; 

    int flags; 

    int valNeeded;
    int normVal;

    xmlSchemaElementPtr decl; 
    int depth;
    xmlSchemaPSVIIDCBindingPtr idcTable; 
    xmlSchemaIDCMatcherPtr idcMatchers; 
    xmlRegExecCtxtPtr regexCtxt;

    const xmlChar **nsBindings; 
    int nbNsBindings;
    int sizeNsBindings;

    int hasKeyrefs;
    int appliedXPath; 
};

#define XML_SCHEMAS_ATTR_UNKNOWN 1
#define XML_SCHEMAS_ATTR_ASSESSED 2
#define XML_SCHEMAS_ATTR_PROHIBITED 3
#define XML_SCHEMAS_ATTR_ERR_MISSING 4
#define XML_SCHEMAS_ATTR_INVALID_VALUE 5
#define XML_SCHEMAS_ATTR_ERR_NO_TYPE 6
#define XML_SCHEMAS_ATTR_ERR_FIXED_VALUE 7
#define XML_SCHEMAS_ATTR_DEFAULT 8
#define XML_SCHEMAS_ATTR_VALIDATE_VALUE 9
#define XML_SCHEMAS_ATTR_ERR_WILD_STRICT_NO_DECL 10
#define XML_SCHEMAS_ATTR_HAS_ATTR_USE 11
#define XML_SCHEMAS_ATTR_HAS_ATTR_DECL 12
#define XML_SCHEMAS_ATTR_WILD_SKIP 13
#define XML_SCHEMAS_ATTR_WILD_LAX_NO_DECL 14
#define XML_SCHEMAS_ATTR_ERR_WILD_DUPLICATE_ID 15
#define XML_SCHEMAS_ATTR_ERR_WILD_AND_USE_ID 16
#define XML_SCHEMAS_ATTR_META 17
#define XML_SCHEMA_ATTR_INFO_META_XSI_TYPE 1
#define XML_SCHEMA_ATTR_INFO_META_XSI_NIL 2
#define XML_SCHEMA_ATTR_INFO_META_XSI_SCHEMA_LOC 3
#define XML_SCHEMA_ATTR_INFO_META_XSI_NO_NS_SCHEMA_LOC 4
#define XML_SCHEMA_ATTR_INFO_META_XMLNS 5

typedef struct _xmlSchemaAttrInfo xmlSchemaAttrInfo;
typedef xmlSchemaAttrInfo *xmlSchemaAttrInfoPtr;
struct _xmlSchemaAttrInfo {
    int nodeType;
    xmlNodePtr node;
    int nodeLine;
    const xmlChar *localName;
    const xmlChar *nsName;
    const xmlChar *value;
    xmlSchemaValPtr val; 
    xmlSchemaTypePtr typeDef; 
    int flags; 

    xmlSchemaAttributePtr decl; 
    xmlSchemaAttributeUsePtr use;  
    int state;
    int metaType;
    const xmlChar *vcValue; 
    xmlSchemaNodeInfoPtr parent;
};


#define XML_SCHEMA_VALID_CTXT_FLAG_STREAM 1
struct _xmlSchemaValidCtxt {
    int type;
    void *errCtxt;             
    xmlSchemaValidityErrorFunc error;   
    xmlSchemaValidityWarningFunc warning; 
    xmlStructuredErrorFunc serror;

    xmlSchemaPtr schema;        
    xmlDocPtr doc;
    xmlParserInputBufferPtr input;
    xmlCharEncoding enc;
    xmlSAXHandlerPtr sax;
    xmlParserCtxtPtr parserCtxt;
    void *user_data; 

    int err;
    int nberrors;

    xmlNodePtr node;
    xmlNodePtr cur;
    

    xmlRegExecCtxtPtr regexp;
    xmlSchemaValPtr value;

    int valueWS;
    int options;
    xmlNodePtr validationRoot;
    xmlSchemaParserCtxtPtr pctxt;
    int xsiAssemble;

    int depth;
    xmlSchemaNodeInfoPtr *elemInfos; 
    int sizeElemInfos;
    xmlSchemaNodeInfoPtr inode; 

    xmlSchemaIDCAugPtr aidcs; 

    xmlSchemaIDCStateObjPtr xpathStates; 
    xmlSchemaIDCStateObjPtr xpathStatePool; 
    xmlSchemaIDCMatcherPtr idcMatcherCache; 

    xmlSchemaPSVIIDCNodePtr *idcNodes; 
    int nbIdcNodes;
    int sizeIdcNodes;

    xmlSchemaPSVIIDCKeyPtr *idcKeys; 
    int nbIdcKeys;
    int sizeIdcKeys;

    int flags;

    xmlDictPtr dict;

#ifdef LIBXML_READER_ENABLED
    xmlTextReaderPtr reader;
#endif

    xmlSchemaAttrInfoPtr *attrInfos;
    int nbAttrInfos;
    int sizeAttrInfos;

    int skipDepth;
    xmlSchemaItemListPtr nodeQNames;
    int hasKeyrefs;
    int createIDCNodeTables;
    int psviExposeIDCNodeTables;
};

typedef struct _xmlSchemaSubstGroup xmlSchemaSubstGroup;
typedef xmlSchemaSubstGroup *xmlSchemaSubstGroupPtr;
struct _xmlSchemaSubstGroup {
    xmlSchemaElementPtr head;
    xmlSchemaItemListPtr members;
};


static int xmlSchemaParseInclude(xmlSchemaParserCtxtPtr ctxt,
                                 xmlSchemaPtr schema,
                                 xmlNodePtr node);
static int xmlSchemaParseRedefine(xmlSchemaParserCtxtPtr ctxt,
                                 xmlSchemaPtr schema,
                                 xmlNodePtr node);
static int
xmlSchemaTypeFixup(xmlSchemaTypePtr type,
                   xmlSchemaAbstractCtxtPtr ctxt);
static const xmlChar *
xmlSchemaFacetTypeToString(xmlSchemaTypeType type);
static int
xmlSchemaParseImport(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                     xmlNodePtr node);
static int
xmlSchemaCheckFacetValues(xmlSchemaTypePtr typeDecl,
                       xmlSchemaParserCtxtPtr ctxt);
static void
xmlSchemaClearValidCtxt(xmlSchemaValidCtxtPtr vctxt);
static xmlSchemaWhitespaceValueType
xmlSchemaGetWhiteSpaceFacetValue(xmlSchemaTypePtr type);
static xmlSchemaTreeItemPtr
xmlSchemaParseModelGroup(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
			 xmlNodePtr node, xmlSchemaTypeType type,
			 int withParticle);
static const xmlChar *
xmlSchemaGetComponentTypeStr(xmlSchemaBasicItemPtr item);
static xmlSchemaTypeLinkPtr
xmlSchemaGetUnionSimpleTypeMemberTypes(xmlSchemaTypePtr type);
static void
xmlSchemaInternalErr(xmlSchemaAbstractCtxtPtr actxt,
		     const char *funcName,
		     const char *message);
static int
xmlSchemaCheckCOSSTDerivedOK(xmlSchemaAbstractCtxtPtr ctxt,
			     xmlSchemaTypePtr type,
			     xmlSchemaTypePtr baseType,
			     int subset);
static void
xmlSchemaCheckElementDeclComponent(xmlSchemaElementPtr elemDecl,
				   xmlSchemaParserCtxtPtr ctxt);
static void
xmlSchemaComponentListFree(xmlSchemaItemListPtr list);
static xmlSchemaQNameRefPtr
xmlSchemaParseAttributeGroupRef(xmlSchemaParserCtxtPtr pctxt,
				xmlSchemaPtr schema,
				xmlNodePtr node);


static const xmlChar *
xmlSchemaItemTypeToStr(xmlSchemaTypeType type)
{
    switch (type) {
	case XML_SCHEMA_TYPE_BASIC:
	    return(BAD_CAST "simple type definition");
	case XML_SCHEMA_TYPE_SIMPLE:
	    return(BAD_CAST "simple type definition");
	case XML_SCHEMA_TYPE_COMPLEX:
	    return(BAD_CAST "complex type definition");
	case XML_SCHEMA_TYPE_ELEMENT:
	    return(BAD_CAST "element declaration");
	case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
	    return(BAD_CAST "attribute use");
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return(BAD_CAST "attribute declaration");
	case XML_SCHEMA_TYPE_GROUP:
	    return(BAD_CAST "model group definition");
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return(BAD_CAST "attribute group definition");
	case XML_SCHEMA_TYPE_NOTATION:
	    return(BAD_CAST "notation declaration");
	case XML_SCHEMA_TYPE_SEQUENCE:
	    return(BAD_CAST "model group (sequence)");
	case XML_SCHEMA_TYPE_CHOICE:
	    return(BAD_CAST "model group (choice)");
	case XML_SCHEMA_TYPE_ALL:
	    return(BAD_CAST "model group (all)");
	case XML_SCHEMA_TYPE_PARTICLE:
	    return(BAD_CAST "particle");
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	    return(BAD_CAST "unique identity-constraint");
	    
	case XML_SCHEMA_TYPE_IDC_KEY:
	    return(BAD_CAST "key identity-constraint");
	    
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return(BAD_CAST "keyref identity-constraint");
	    
	case XML_SCHEMA_TYPE_ANY:
	    return(BAD_CAST "wildcard (any)");
	case XML_SCHEMA_EXTRA_QNAMEREF:
	    return(BAD_CAST "[helper component] QName reference");
	case XML_SCHEMA_EXTRA_ATTR_USE_PROHIB:
	    return(BAD_CAST "[helper component] attribute use prohibition");
	default:
	    return(BAD_CAST "Not a schema component");
    }
}

static const xmlChar *
xmlSchemaGetComponentTypeStr(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_BASIC:
	    if (WXS_IS_COMPLEX(WXS_TYPE_CAST item))
		return(BAD_CAST "complex type definition");
	    else
		return(BAD_CAST "simple type definition");
	default:
	    return(xmlSchemaItemTypeToStr(item->type));
    }
}

static xmlNodePtr
xmlSchemaGetComponentNode(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return (((xmlSchemaElementPtr) item)->node);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return (((xmlSchemaAttributePtr) item)->node);
	case XML_SCHEMA_TYPE_COMPLEX:
	case XML_SCHEMA_TYPE_SIMPLE:
	    return (((xmlSchemaTypePtr) item)->node);
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    return (((xmlSchemaWildcardPtr) item)->node);
	case XML_SCHEMA_TYPE_PARTICLE:
	    return (((xmlSchemaParticlePtr) item)->node);
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	    return (((xmlSchemaModelGroupPtr) item)->node);
	case XML_SCHEMA_TYPE_GROUP:
	    return (((xmlSchemaModelGroupDefPtr) item)->node);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return (((xmlSchemaAttributeGroupPtr) item)->node);
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return (((xmlSchemaIDCPtr) item)->node);
	case XML_SCHEMA_EXTRA_QNAMEREF:
	    return(((xmlSchemaQNameRefPtr) item)->node);
	case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
	    return (((xmlSchemaAttributeUsePtr) item)->node);
	default:
	    return (NULL);
    }
}

#if 0
static xmlSchemaBasicItemPtr
xmlSchemaGetNextComponent(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaElementPtr) item)->next);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaAttributePtr) item)->next);
	case XML_SCHEMA_TYPE_COMPLEX:
	case XML_SCHEMA_TYPE_SIMPLE:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaTypePtr) item)->next);
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    return (NULL);
	case XML_SCHEMA_TYPE_PARTICLE:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaParticlePtr) item)->next);
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	    return (NULL);
	case XML_SCHEMA_TYPE_GROUP:
	    return (NULL);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaAttributeGroupPtr) item)->next);
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return ((xmlSchemaBasicItemPtr) ((xmlSchemaIDCPtr) item)->next);
	default:
	    return (NULL);
    }
}
#endif


static const xmlChar*
xmlSchemaFormatQName(xmlChar **buf,
		     const xmlChar *namespaceName,
		     const xmlChar *localName)
{
    FREE_AND_NULL(*buf)
    if (namespaceName != NULL) {
	*buf = xmlStrdup(BAD_CAST "{");
	*buf = xmlStrcat(*buf, namespaceName);
	*buf = xmlStrcat(*buf, BAD_CAST "}");
    }
    if (localName != NULL) {
	if (namespaceName == NULL)
	    return(localName);
	*buf = xmlStrcat(*buf, localName);
    } else {
	*buf = xmlStrcat(*buf, BAD_CAST "(NULL)");
    }
    return ((const xmlChar *) *buf);
}

static const xmlChar*
xmlSchemaFormatQNameNs(xmlChar **buf, xmlNsPtr ns, const xmlChar *localName)
{
    if (ns != NULL)
	return (xmlSchemaFormatQName(buf, ns->href, localName));
    else
	return (xmlSchemaFormatQName(buf, NULL, localName));
}

static const xmlChar *
xmlSchemaGetComponentName(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return (((xmlSchemaElementPtr) item)->name);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return (((xmlSchemaAttributePtr) item)->name);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return (((xmlSchemaAttributeGroupPtr) item)->name);
	case XML_SCHEMA_TYPE_BASIC:
	case XML_SCHEMA_TYPE_SIMPLE:
	case XML_SCHEMA_TYPE_COMPLEX:
	    return (((xmlSchemaTypePtr) item)->name);
	case XML_SCHEMA_TYPE_GROUP:
	    return (((xmlSchemaModelGroupDefPtr) item)->name);
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return (((xmlSchemaIDCPtr) item)->name);
	case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
	    if (WXS_ATTRUSE_DECL(item) != NULL) {
		return(xmlSchemaGetComponentName(
		    WXS_BASIC_CAST WXS_ATTRUSE_DECL(item)));
	    } else
		return(NULL);
	case XML_SCHEMA_EXTRA_QNAMEREF:
	    return (((xmlSchemaQNameRefPtr) item)->name);
	case XML_SCHEMA_TYPE_NOTATION:
	    return (((xmlSchemaNotationPtr) item)->name);
	default:
	    break;
    }
    return (NULL);
}

#define xmlSchemaGetQNameRefName(r) (WXS_QNAME_CAST (r))->name
#define xmlSchemaGetQNameRefTargetNs(r) (WXS_QNAME_CAST (r))->targetNamespace

static const xmlChar *
xmlSchemaGetComponentTargetNs(xmlSchemaBasicItemPtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_ELEMENT:
	    return (((xmlSchemaElementPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    return (((xmlSchemaAttributePtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    return (((xmlSchemaAttributeGroupPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_BASIC:
	    return (BAD_CAST "http://www.w3.org/2001/XMLSchema");
	case XML_SCHEMA_TYPE_SIMPLE:
	case XML_SCHEMA_TYPE_COMPLEX:
	    return (((xmlSchemaTypePtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_GROUP:
	    return (((xmlSchemaModelGroupDefPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    return (((xmlSchemaIDCPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
	    if (WXS_ATTRUSE_DECL(item) != NULL) {
		return(xmlSchemaGetComponentTargetNs(
		    WXS_BASIC_CAST WXS_ATTRUSE_DECL(item)));
	    }
	    
	    break;
	case XML_SCHEMA_EXTRA_QNAMEREF:
	    return (((xmlSchemaQNameRefPtr) item)->targetNamespace);
	case XML_SCHEMA_TYPE_NOTATION:
	    return (((xmlSchemaNotationPtr) item)->targetNamespace);
	default:
	    break;
    }
    return (NULL);
}

static const xmlChar*
xmlSchemaGetComponentQName(xmlChar **buf,
			   void *item)
{
    return (xmlSchemaFormatQName(buf,
	xmlSchemaGetComponentTargetNs((xmlSchemaBasicItemPtr) item),
	xmlSchemaGetComponentName((xmlSchemaBasicItemPtr) item)));
}

static const xmlChar*
xmlSchemaGetComponentDesignation(xmlChar **buf, void *item)
{
    xmlChar *str = NULL;

    *buf = xmlStrcat(*buf, WXS_ITEM_TYPE_NAME(item));
    *buf = xmlStrcat(*buf, BAD_CAST " '");
    *buf = xmlStrcat(*buf, xmlSchemaGetComponentQName(&str,
	(xmlSchemaBasicItemPtr) item));
    *buf = xmlStrcat(*buf, BAD_CAST "'");
    FREE_AND_NULL(str);
    return(*buf);
}

static const xmlChar*
xmlSchemaGetIDCDesignation(xmlChar **buf, xmlSchemaIDCPtr idc)
{
    return(xmlSchemaGetComponentDesignation(buf, idc));
}

static const xmlChar *
xmlSchemaWildcardPCToString(int pc)
{
    switch (pc) {
	case XML_SCHEMAS_ANY_SKIP:
	    return (BAD_CAST "skip");
	case XML_SCHEMAS_ANY_LAX:
	    return (BAD_CAST "lax");
	case XML_SCHEMAS_ANY_STRICT:
	    return (BAD_CAST "strict");
	default:
	    return (BAD_CAST "invalid process contents");
    }
}

static int
xmlSchemaGetCanonValueWhtspExt(xmlSchemaValPtr val,
			       xmlSchemaWhitespaceValueType ws,
			       xmlChar **retValue)
{
    int list;
    xmlSchemaValType valType;
    const xmlChar *value, *value2 = NULL;


    if ((retValue == NULL) || (val == NULL))
	return (-1);
    list = xmlSchemaValueGetNext(val) ? 1 : 0;
    *retValue = NULL;
    do {
	value = NULL;
	valType = xmlSchemaGetValType(val);
	switch (valType) {
	    case XML_SCHEMAS_STRING:
	    case XML_SCHEMAS_NORMSTRING:
	    case XML_SCHEMAS_ANYSIMPLETYPE:
		value = xmlSchemaValueGetAsString(val);
		if (value != NULL) {
		    if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			value2 = xmlSchemaCollapseString(value);
		    else if (ws == XML_SCHEMA_WHITESPACE_REPLACE)
			value2 = xmlSchemaWhiteSpaceReplace(value);
		    if (value2 != NULL)
			value = value2;
		}
		break;
	    default:
		if (xmlSchemaGetCanonValue(val, &value2) == -1) {
		    if (value2 != NULL)
			xmlFree((xmlChar *) value2);
		    goto internal_error;
		}
		value = value2;
	}
	if (*retValue == NULL)
	    if (value == NULL) {
		if (! list)
		    *retValue = xmlStrdup(BAD_CAST "");
	    } else
		*retValue = xmlStrdup(value);
	else if (value != NULL) {
	    
	    *retValue = xmlStrcat((xmlChar *) *retValue, BAD_CAST " ");
	    *retValue = xmlStrcat((xmlChar *) *retValue, value);
	}
	FREE_AND_NULL(value2)
	val = xmlSchemaValueGetNext(val);
    } while (val != NULL);

    return (0);
internal_error:
    if (*retValue != NULL)
	xmlFree((xmlChar *) (*retValue));
    if (value2 != NULL)
	xmlFree((xmlChar *) value2);
    return (-1);
}

static xmlChar*
xmlSchemaFormatItemForReport(xmlChar **buf,
		     const xmlChar *itemDes,
		     xmlSchemaBasicItemPtr item,
		     xmlNodePtr itemNode)
{
    xmlChar *str = NULL;
    int named = 1;

    if (*buf != NULL) {
	xmlFree(*buf);
	*buf = NULL;
    }

    if (itemDes != NULL) {
	*buf = xmlStrdup(itemDes);
    } else if (item != NULL) {
	switch (item->type) {
	case XML_SCHEMA_TYPE_BASIC: {
	    xmlSchemaTypePtr type = WXS_TYPE_CAST item;

	    if (WXS_IS_ATOMIC(type))
		*buf = xmlStrdup(BAD_CAST "atomic type 'xs:");
	    else if (WXS_IS_LIST(type))
		*buf = xmlStrdup(BAD_CAST "list type 'xs:");
	    else if (WXS_IS_UNION(type))
		*buf = xmlStrdup(BAD_CAST "union type 'xs:");
	    else
		*buf = xmlStrdup(BAD_CAST "simple type 'xs:");
	    *buf = xmlStrcat(*buf, type->name);
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    break;
	case XML_SCHEMA_TYPE_SIMPLE: {
	    xmlSchemaTypePtr type = WXS_TYPE_CAST item;

	    if (type->flags & XML_SCHEMAS_TYPE_GLOBAL) {
		*buf = xmlStrdup(BAD_CAST"");
	    } else {
		*buf = xmlStrdup(BAD_CAST "local ");
	    }
	    if (WXS_IS_ATOMIC(type))
		*buf = xmlStrcat(*buf, BAD_CAST "atomic type");
	    else if (WXS_IS_LIST(type))
		*buf = xmlStrcat(*buf, BAD_CAST "list type");
	    else if (WXS_IS_UNION(type))
		*buf = xmlStrcat(*buf, BAD_CAST "union type");
	    else
		*buf = xmlStrcat(*buf, BAD_CAST "simple type");
	    if (type->flags & XML_SCHEMAS_TYPE_GLOBAL) {
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, type->name);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    }
	    break;
	case XML_SCHEMA_TYPE_COMPLEX: {
	    xmlSchemaTypePtr type = WXS_TYPE_CAST item;

	    if (type->flags & XML_SCHEMAS_TYPE_GLOBAL)
		*buf = xmlStrdup(BAD_CAST "");
	    else
		*buf = xmlStrdup(BAD_CAST "local ");
	    *buf = xmlStrcat(*buf, BAD_CAST "complex type");
	    if (type->flags & XML_SCHEMAS_TYPE_GLOBAL) {
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, type->name);
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE_USE: {
		xmlSchemaAttributeUsePtr ause;

		ause = WXS_ATTR_USE_CAST item;
		*buf = xmlStrdup(BAD_CAST "attribute use ");
		if (WXS_ATTRUSE_DECL(ause) != NULL) {
		    *buf = xmlStrcat(*buf, BAD_CAST "'");
		    *buf = xmlStrcat(*buf,
			xmlSchemaGetComponentQName(&str, WXS_ATTRUSE_DECL(ause)));
		    FREE_AND_NULL(str)
			*buf = xmlStrcat(*buf, BAD_CAST "'");
		} else {
		    *buf = xmlStrcat(*buf, BAD_CAST "(unknown)");
		}
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE: {
		xmlSchemaAttributePtr attr;

		attr = (xmlSchemaAttributePtr) item;
		*buf = xmlStrdup(BAD_CAST "attribute decl.");
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, xmlSchemaFormatQName(&str,
		    attr->targetNamespace, attr->name));
		FREE_AND_NULL(str)
		    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    xmlSchemaGetComponentDesignation(buf, item);
	    break;
	case XML_SCHEMA_TYPE_ELEMENT: {
		xmlSchemaElementPtr elem;

		elem = (xmlSchemaElementPtr) item;
		*buf = xmlStrdup(BAD_CAST "element decl.");
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, xmlSchemaFormatQName(&str,
		    elem->targetNamespace, elem->name));
		*buf = xmlStrcat(*buf, BAD_CAST "'");
	    }
	    break;
	case XML_SCHEMA_TYPE_IDC_UNIQUE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	    if (item->type == XML_SCHEMA_TYPE_IDC_UNIQUE)
		*buf = xmlStrdup(BAD_CAST "unique '");
	    else if (item->type == XML_SCHEMA_TYPE_IDC_KEY)
		*buf = xmlStrdup(BAD_CAST "key '");
	    else
		*buf = xmlStrdup(BAD_CAST "keyRef '");
	    *buf = xmlStrcat(*buf, ((xmlSchemaIDCPtr) item)->name);
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    break;
	case XML_SCHEMA_TYPE_ANY:
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	    *buf = xmlStrdup(xmlSchemaWildcardPCToString(
		    ((xmlSchemaWildcardPtr) item)->processContents));
	    *buf = xmlStrcat(*buf, BAD_CAST " wildcard");
	    break;
	case XML_SCHEMA_FACET_MININCLUSIVE:
	case XML_SCHEMA_FACET_MINEXCLUSIVE:
	case XML_SCHEMA_FACET_MAXINCLUSIVE:
	case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	case XML_SCHEMA_FACET_TOTALDIGITS:
	case XML_SCHEMA_FACET_FRACTIONDIGITS:
	case XML_SCHEMA_FACET_PATTERN:
	case XML_SCHEMA_FACET_ENUMERATION:
	case XML_SCHEMA_FACET_WHITESPACE:
	case XML_SCHEMA_FACET_LENGTH:
	case XML_SCHEMA_FACET_MAXLENGTH:
	case XML_SCHEMA_FACET_MINLENGTH:
	    *buf = xmlStrdup(BAD_CAST "facet '");
	    *buf = xmlStrcat(*buf, xmlSchemaFacetTypeToString(item->type));
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    break;
	case XML_SCHEMA_TYPE_GROUP: {
		*buf = xmlStrdup(BAD_CAST "model group def.");
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, xmlSchemaGetComponentQName(&str, item));
		*buf = xmlStrcat(*buf, BAD_CAST "'");
		FREE_AND_NULL(str)
	    }
	    break;
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL:
	case XML_SCHEMA_TYPE_PARTICLE:
	    *buf = xmlStrdup(WXS_ITEM_TYPE_NAME(item));
	    break;
	case XML_SCHEMA_TYPE_NOTATION: {
		*buf = xmlStrdup(WXS_ITEM_TYPE_NAME(item));
		*buf = xmlStrcat(*buf, BAD_CAST " '");
		*buf = xmlStrcat(*buf, xmlSchemaGetComponentQName(&str, item));
		*buf = xmlStrcat(*buf, BAD_CAST "'");
		FREE_AND_NULL(str);
	    }
	default:
	    named = 0;
	}
    } else
	named = 0;

    if ((named == 0) && (itemNode != NULL)) {
	xmlNodePtr elem;

	if (itemNode->type == XML_ATTRIBUTE_NODE)
	    elem = itemNode->parent;
	else
	    elem = itemNode;
	*buf = xmlStrdup(BAD_CAST "Element '");
	if (elem->ns != NULL) {
	    *buf = xmlStrcat(*buf,
		xmlSchemaFormatQName(&str, elem->ns->href, elem->name));
	    FREE_AND_NULL(str)
	} else
	    *buf = xmlStrcat(*buf, elem->name);
	*buf = xmlStrcat(*buf, BAD_CAST "'");

    }
    if ((itemNode != NULL) && (itemNode->type == XML_ATTRIBUTE_NODE)) {
	*buf = xmlStrcat(*buf, BAD_CAST ", attribute '");
	if (itemNode->ns != NULL) {
	    *buf = xmlStrcat(*buf, xmlSchemaFormatQName(&str,
		itemNode->ns->href, itemNode->name));
	    FREE_AND_NULL(str)
	} else
	    *buf = xmlStrcat(*buf, itemNode->name);
	*buf = xmlStrcat(*buf, BAD_CAST "'");
    }
    FREE_AND_NULL(str)

    return (*buf);
}

static const xmlChar *
xmlSchemaFormatFacetEnumSet(xmlSchemaAbstractCtxtPtr actxt,
			    xmlChar **buf, xmlSchemaTypePtr type)
{
    xmlSchemaFacetPtr facet;
    xmlSchemaWhitespaceValueType ws;
    xmlChar *value = NULL;
    int res, found = 0;

    if (*buf != NULL)
	xmlFree(*buf);
    *buf = NULL;

    do {
	ws = xmlSchemaGetWhiteSpaceFacetValue(type->baseType);
	for (facet = type->facets; facet != NULL; facet = facet->next) {
	    if (facet->type != XML_SCHEMA_FACET_ENUMERATION)
		continue;
	    found = 1;
	    res = xmlSchemaGetCanonValueWhtspExt(facet->val,
		ws, &value);
	    if (res == -1) {
		xmlSchemaInternalErr(actxt,
		    "xmlSchemaFormatFacetEnumSet",
		    "compute the canonical lexical representation");
		if (*buf != NULL)
		    xmlFree(*buf);
		*buf = NULL;
		return (NULL);
	    }
	    if (*buf == NULL)
		*buf = xmlStrdup(BAD_CAST "'");
	    else
		*buf = xmlStrcat(*buf, BAD_CAST ", '");
	    *buf = xmlStrcat(*buf, BAD_CAST value);
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	    if (value != NULL) {
		xmlFree((xmlChar *)value);
		value = NULL;
	    }
	}
	if (found)
	    break;
	type = type->baseType;
    } while ((type != NULL) && (type->type != XML_SCHEMA_TYPE_BASIC));

    return ((const xmlChar *) *buf);
}


#if 0
static void
xmlSchemaErrMemory(const char *msg)
{
    __xmlSimpleError(XML_FROM_SCHEMASP, XML_ERR_NO_MEMORY, NULL, NULL,
                     msg);
}
#endif

static void
xmlSchemaPSimpleErr(const char *msg)
{
    __xmlSimpleError(XML_FROM_SCHEMASP, XML_ERR_NO_MEMORY, NULL, NULL,
                     msg);
}

static void
xmlSchemaPErrMemory(xmlSchemaParserCtxtPtr ctxt,
                    const char *extra, xmlNodePtr node)
{
    if (ctxt != NULL)
        ctxt->nberrors++;
    __xmlSimpleError(XML_FROM_SCHEMASP, XML_ERR_NO_MEMORY, node, NULL,
                     extra);
}

static void
xmlSchemaPErr(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node, int error,
              const char *msg, const xmlChar * str1, const xmlChar * str2)
{
    xmlGenericErrorFunc channel = NULL;
    xmlStructuredErrorFunc schannel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
	ctxt->err = error;
        channel = ctxt->error;
        data = ctxt->errCtxt;
	schannel = ctxt->serror;
    }
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASP,
                    error, XML_ERR_ERROR, NULL, 0,
                    (const char *) str1, (const char *) str2, NULL, 0, 0,
                    msg, str1, str2);
}

static void
xmlSchemaPErr2(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
               xmlNodePtr child, int error,
               const char *msg, const xmlChar * str1, const xmlChar * str2)
{
    if (child != NULL)
        xmlSchemaPErr(ctxt, child, error, msg, str1, str2);
    else
        xmlSchemaPErr(ctxt, node, error, msg, str1, str2);
}


static void
xmlSchemaPErrExt(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node, int error,
		const xmlChar * strData1, const xmlChar * strData2,
		const xmlChar * strData3, const char *msg, const xmlChar * str1,
		const xmlChar * str2, const xmlChar * str3, const xmlChar * str4,
		const xmlChar * str5)
{

    xmlGenericErrorFunc channel = NULL;
    xmlStructuredErrorFunc schannel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
        ctxt->nberrors++;
	ctxt->err = error;
        channel = ctxt->error;
        data = ctxt->errCtxt;
	schannel = ctxt->serror;
    }
    __xmlRaiseError(schannel, channel, data, ctxt, node, XML_FROM_SCHEMASP,
                    error, XML_ERR_ERROR, NULL, 0,
                    (const char *) strData1, (const char *) strData2,
		    (const char *) strData3, 0, 0, msg, str1, str2,
		    str3, str4, str5);
}


static void
xmlSchemaVErrMemory(xmlSchemaValidCtxtPtr ctxt,
                    const char *extra, xmlNodePtr node)
{
    if (ctxt != NULL) {
        ctxt->nberrors++;
        ctxt->err = XML_SCHEMAV_INTERNAL;
    }
    __xmlSimpleError(XML_FROM_SCHEMASV, XML_ERR_NO_MEMORY, node, NULL,
                     extra);
}

static void
xmlSchemaPSimpleInternalErr(xmlNodePtr node,
			    const char *msg, const xmlChar *str)
{
     __xmlSimpleError(XML_FROM_SCHEMASP, XML_SCHEMAP_INTERNAL, node,
	 msg, (const char *) str);
}

#define WXS_ERROR_TYPE_ERROR 1
#define WXS_ERROR_TYPE_WARNING 2
static void
xmlSchemaErr4Line(xmlSchemaAbstractCtxtPtr ctxt,
		  xmlErrorLevel errorLevel,
		  int error, xmlNodePtr node, int line, const char *msg,
		  const xmlChar *str1, const xmlChar *str2,
		  const xmlChar *str3, const xmlChar *str4)
{
    xmlStructuredErrorFunc schannel = NULL;
    xmlGenericErrorFunc channel = NULL;
    void *data = NULL;

    if (ctxt != NULL) {
	if (ctxt->type == XML_SCHEMA_CTXT_VALIDATOR) {
	    xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) ctxt;
	    const char *file = NULL;
	    if (errorLevel != XML_ERR_WARNING) {
		vctxt->nberrors++;
		vctxt->err = error;
		channel = vctxt->error;
	    } else {
		channel = vctxt->warning;
	    }
	    schannel = vctxt->serror;
	    data = vctxt->errCtxt;

	    if (line == 0) {
		if ((node == NULL) &&
		    (vctxt->depth >= 0) &&
		    (vctxt->inode != NULL)) {
		    node = vctxt->inode->node;
		}
		if ((node == NULL) &&
		    (vctxt->parserCtxt != NULL) &&
		    (vctxt->parserCtxt->input != NULL)) {
		    file = vctxt->parserCtxt->input->filename;
		    line = vctxt->parserCtxt->input->line;
		}
	    } else {
		node = NULL;
		if (vctxt->doc != NULL)
		    file = (const char *) vctxt->doc->URL;
		else if ((vctxt->parserCtxt != NULL) &&
		    (vctxt->parserCtxt->input != NULL))
		    file = vctxt->parserCtxt->input->filename;
	    }
	    __xmlRaiseError(schannel, channel, data, ctxt,
		node, XML_FROM_SCHEMASV,
		error, errorLevel, file, line,
		(const char *) str1, (const char *) str2,
		(const char *) str3, 0, 0, msg, str1, str2, str3, str4);

	} else if (ctxt->type == XML_SCHEMA_CTXT_PARSER) {
	    xmlSchemaParserCtxtPtr pctxt = (xmlSchemaParserCtxtPtr) ctxt;
	    if (errorLevel != XML_ERR_WARNING) {
		pctxt->nberrors++;
		pctxt->err = error;
		channel = pctxt->error;
	    } else {
		channel = pctxt->warning;
	    }
	    schannel = pctxt->serror;
	    data = pctxt->errCtxt;
	    __xmlRaiseError(schannel, channel, data, ctxt,
		node, XML_FROM_SCHEMASP, error,
		errorLevel, NULL, 0,
		(const char *) str1, (const char *) str2,
		(const char *) str3, 0, 0, msg, str1, str2, str3, str4);
	} else {
	    TODO
	}
    }
}

static void
xmlSchemaErr3(xmlSchemaAbstractCtxtPtr actxt,
	      int error, xmlNodePtr node, const char *msg,
	      const xmlChar *str1, const xmlChar *str2, const xmlChar *str3)
{
    xmlSchemaErr4Line(actxt, XML_ERR_ERROR, error, node, 0,
	msg, str1, str2, str3, NULL);
}

static void
xmlSchemaErr4(xmlSchemaAbstractCtxtPtr actxt,
	      int error, xmlNodePtr node, const char *msg,
	      const xmlChar *str1, const xmlChar *str2,
	      const xmlChar *str3, const xmlChar *str4)
{
    xmlSchemaErr4Line(actxt, XML_ERR_ERROR, error, node, 0,
	msg, str1, str2, str3, str4);
}

static void
xmlSchemaErr(xmlSchemaAbstractCtxtPtr actxt,
	     int error, xmlNodePtr node, const char *msg,
	     const xmlChar *str1, const xmlChar *str2)
{
    xmlSchemaErr4(actxt, error, node, msg, str1, str2, NULL, NULL);
}

static xmlChar *
xmlSchemaFormatNodeForError(xmlChar ** msg,
			    xmlSchemaAbstractCtxtPtr actxt,
			    xmlNodePtr node)
{
    xmlChar *str = NULL;

    *msg = NULL;
    if ((node != NULL) &&
	(node->type != XML_ELEMENT_NODE) &&
	(node->type != XML_ATTRIBUTE_NODE))
    {
	*msg = xmlStrdup(BAD_CAST "");
	return(*msg);
    }
    if (node != NULL) {
	if (node->type == XML_ATTRIBUTE_NODE) {
	    xmlNodePtr elem = node->parent;

	    *msg = xmlStrdup(BAD_CAST "Element '");
	    if (elem->ns != NULL)
		*msg = xmlStrcat(*msg, xmlSchemaFormatQName(&str,
		    elem->ns->href, elem->name));
	    else
		*msg = xmlStrcat(*msg, xmlSchemaFormatQName(&str,
		    NULL, elem->name));
	    FREE_AND_NULL(str);
	    *msg = xmlStrcat(*msg, BAD_CAST "', ");
	    *msg = xmlStrcat(*msg, BAD_CAST "attribute '");
	} else {
	    *msg = xmlStrdup(BAD_CAST "Element '");
	}
	if (node->ns != NULL)
	    *msg = xmlStrcat(*msg, xmlSchemaFormatQName(&str,
	    node->ns->href, node->name));
	else
	    *msg = xmlStrcat(*msg, xmlSchemaFormatQName(&str,
	    NULL, node->name));
	FREE_AND_NULL(str);
	*msg = xmlStrcat(*msg, BAD_CAST "': ");
    } else if (actxt->type == XML_SCHEMA_CTXT_VALIDATOR) {
	xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) actxt;
	if (vctxt->inode->nodeType == XML_ATTRIBUTE_NODE) {
	    xmlSchemaNodeInfoPtr ielem =
		vctxt->elemInfos[vctxt->depth];

	    *msg = xmlStrdup(BAD_CAST "Element '");
	    *msg = xmlStrcat(*msg, xmlSchemaFormatQName(&str,
		ielem->nsName, ielem->localName));
	    FREE_AND_NULL(str);
	    *msg = xmlStrcat(*msg, BAD_CAST "', ");
	    *msg = xmlStrcat(*msg, BAD_CAST "attribute '");
	} else {
	    *msg = xmlStrdup(BAD_CAST "Element '");
	}
	*msg = xmlStrcat(*msg, xmlSchemaFormatQName(&str,
	    vctxt->inode->nsName, vctxt->inode->localName));
	FREE_AND_NULL(str);
	*msg = xmlStrcat(*msg, BAD_CAST "': ");
    } else if (actxt->type == XML_SCHEMA_CTXT_PARSER) {
	*msg = xmlStrdup(BAD_CAST "");
    } else {
	TODO
	return (NULL);
    }
#if 0
    if ((type != NULL) && (xmlSchemaIsGlobalItem(type))) {
	*msg = xmlStrcat(*msg, BAD_CAST " [");
	*msg = xmlStrcat(*msg, xmlSchemaFormatItemForReport(&str,
	    NULL, type, NULL, 0));
	FREE_AND_NULL(str)
	*msg = xmlStrcat(*msg, BAD_CAST "]");
    }
#endif
    return (*msg);
}

static void
xmlSchemaInternalErr2(xmlSchemaAbstractCtxtPtr actxt,
		     const char *funcName,
		     const char *message,
		     const xmlChar *str1,
		     const xmlChar *str2)
{
    xmlChar *msg = NULL;

    if (actxt == NULL)
        return;
    msg = xmlStrdup(BAD_CAST "Internal error: ");
    msg = xmlStrcat(msg, BAD_CAST funcName);
    msg = xmlStrcat(msg, BAD_CAST ", ");
    msg = xmlStrcat(msg, BAD_CAST message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");

    if (actxt->type == XML_SCHEMA_CTXT_VALIDATOR)
	xmlSchemaErr(actxt, XML_SCHEMAV_INTERNAL, NULL,
	    (const char *) msg, str1, str2);

    else if (actxt->type == XML_SCHEMA_CTXT_PARSER)
	xmlSchemaErr(actxt, XML_SCHEMAP_INTERNAL, NULL,
	    (const char *) msg, str1, str2);

    FREE_AND_NULL(msg)
}

static void
xmlSchemaInternalErr(xmlSchemaAbstractCtxtPtr actxt,
		     const char *funcName,
		     const char *message)
{
    xmlSchemaInternalErr2(actxt, funcName, message, NULL, NULL);
}

#if 0
static void
xmlSchemaPInternalErr(xmlSchemaParserCtxtPtr pctxt,
		     const char *funcName,
		     const char *message,
		     const xmlChar *str1,
		     const xmlChar *str2)
{
    xmlSchemaInternalErr2(ACTXT_CAST pctxt, funcName, message,
	str1, str2);
}
#endif

static void
xmlSchemaCustomErr4(xmlSchemaAbstractCtxtPtr actxt,
		   xmlParserErrors error,
		   xmlNodePtr node,
		   xmlSchemaBasicItemPtr item,
		   const char *message,
		   const xmlChar *str1, const xmlChar *str2,
		   const xmlChar *str3, const xmlChar *str4)
{
    xmlChar *msg = NULL;

    if ((node == NULL) && (item != NULL) &&
	(actxt->type == XML_SCHEMA_CTXT_PARSER)) {
	node = WXS_ITEM_NODE(item);
	xmlSchemaFormatItemForReport(&msg, NULL, item, NULL);
	msg = xmlStrcat(msg, BAD_CAST ": ");
    } else
	xmlSchemaFormatNodeForError(&msg, actxt, node);
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    xmlSchemaErr4(actxt, error, node,
	(const char *) msg, str1, str2, str3, str4);
    FREE_AND_NULL(msg)
}

static void
xmlSchemaCustomErr(xmlSchemaAbstractCtxtPtr actxt,
		   xmlParserErrors error,
		   xmlNodePtr node,
		   xmlSchemaBasicItemPtr item,
		   const char *message,
		   const xmlChar *str1,
		   const xmlChar *str2)
{
    xmlSchemaCustomErr4(actxt, error, node, item,
	message, str1, str2, NULL, NULL);
}



static void
xmlSchemaCustomWarning(xmlSchemaAbstractCtxtPtr actxt,
		   xmlParserErrors error,
		   xmlNodePtr node,
		   xmlSchemaTypePtr type ATTRIBUTE_UNUSED,
		   const char *message,
		   const xmlChar *str1,
		   const xmlChar *str2,
		   const xmlChar *str3)
{
    xmlChar *msg = NULL;

    xmlSchemaFormatNodeForError(&msg, actxt, node);
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");

    
    xmlSchemaErr4Line(actxt, XML_ERR_WARNING, error, node, 0,
	(const char *) msg, str1, str2, str3, NULL);

    FREE_AND_NULL(msg)
}



static void
xmlSchemaKeyrefErr(xmlSchemaValidCtxtPtr vctxt,
		   xmlParserErrors error,
		   xmlSchemaPSVIIDCNodePtr idcNode,
		   xmlSchemaTypePtr type ATTRIBUTE_UNUSED,
		   const char *message,
		   const xmlChar *str1,
		   const xmlChar *str2)
{
    xmlChar *msg = NULL, *qname = NULL;

    msg = xmlStrdup(BAD_CAST "Element '%s': ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    xmlSchemaErr4Line(ACTXT_CAST vctxt, XML_ERR_ERROR,
	error, NULL, idcNode->nodeLine, (const char *) msg,
	xmlSchemaFormatQName(&qname,
	    vctxt->nodeQNames->items[idcNode->nodeQNameID +1],
	    vctxt->nodeQNames->items[idcNode->nodeQNameID]),
	str1, str2, NULL);
    FREE_AND_NULL(qname);
    FREE_AND_NULL(msg);
}

static int
xmlSchemaEvalErrorNodeType(xmlSchemaAbstractCtxtPtr actxt,
			   xmlNodePtr node)
{
    if (node != NULL)
	return (node->type);
    if ((actxt->type == XML_SCHEMA_CTXT_VALIDATOR) &&
	(((xmlSchemaValidCtxtPtr) actxt)->inode != NULL))
	return ( ((xmlSchemaValidCtxtPtr) actxt)->inode->nodeType);
    return (-1);
}

static int
xmlSchemaIsGlobalItem(xmlSchemaTypePtr item)
{
    switch (item->type) {
	case XML_SCHEMA_TYPE_COMPLEX:
	case XML_SCHEMA_TYPE_SIMPLE:
	    if (item->flags & XML_SCHEMAS_TYPE_GLOBAL)
		return(1);
	    break;
	case XML_SCHEMA_TYPE_GROUP:
	    return (1);
	case XML_SCHEMA_TYPE_ELEMENT:
	    if ( ((xmlSchemaElementPtr) item)->flags &
		XML_SCHEMAS_ELEM_GLOBAL)
		return(1);
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE:
	    if ( ((xmlSchemaAttributePtr) item)->flags &
		XML_SCHEMAS_ATTR_GLOBAL)
		return(1);
	    break;
	
	default:
	    return(1);
    }
    return (0);
}

static void
xmlSchemaSimpleTypeErr(xmlSchemaAbstractCtxtPtr actxt,
		       xmlParserErrors error,
		       xmlNodePtr node,
		       const xmlChar *value,
		       xmlSchemaTypePtr type,
		       int displayValue)
{
    xmlChar *msg = NULL;

    xmlSchemaFormatNodeForError(&msg, actxt, node);

    if (displayValue || (xmlSchemaEvalErrorNodeType(actxt, node) ==
	    XML_ATTRIBUTE_NODE))
	msg = xmlStrcat(msg, BAD_CAST "'%s' is not a valid value of ");
    else
	msg = xmlStrcat(msg, BAD_CAST "The character content is not a valid "
	    "value of ");

    if (! xmlSchemaIsGlobalItem(type))
	msg = xmlStrcat(msg, BAD_CAST "the local ");
    else
	msg = xmlStrcat(msg, BAD_CAST "the ");

    if (WXS_IS_ATOMIC(type))
	msg = xmlStrcat(msg, BAD_CAST "atomic type");
    else if (WXS_IS_LIST(type))
	msg = xmlStrcat(msg, BAD_CAST "list type");
    else if (WXS_IS_UNION(type))
	msg = xmlStrcat(msg, BAD_CAST "union type");

    if (xmlSchemaIsGlobalItem(type)) {
	xmlChar *str = NULL;
	msg = xmlStrcat(msg, BAD_CAST " '");
	if (type->builtInType != 0) {
	    msg = xmlStrcat(msg, BAD_CAST "xs:");
	    msg = xmlStrcat(msg, type->name);
	} else
	    msg = xmlStrcat(msg,
		xmlSchemaFormatQName(&str,
		    type->targetNamespace, type->name));
	msg = xmlStrcat(msg, BAD_CAST "'");
	FREE_AND_NULL(str);
    }
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    if (displayValue || (xmlSchemaEvalErrorNodeType(actxt, node) ==
	    XML_ATTRIBUTE_NODE))
	xmlSchemaErr(actxt, error, node, (const char *) msg, value, NULL);
    else
	xmlSchemaErr(actxt, error, node, (const char *) msg, NULL, NULL);
    FREE_AND_NULL(msg)
}

static const xmlChar *
xmlSchemaFormatErrorNodeQName(xmlChar ** str,
			      xmlSchemaNodeInfoPtr ni,
			      xmlNodePtr node)
{
    if (node != NULL) {
	if (node->ns != NULL)
	    return (xmlSchemaFormatQName(str, node->ns->href, node->name));
	else
	    return (xmlSchemaFormatQName(str, NULL, node->name));
    } else if (ni != NULL)
	return (xmlSchemaFormatQName(str, ni->nsName, ni->localName));
    return (NULL);
}

static void
xmlSchemaIllegalAttrErr(xmlSchemaAbstractCtxtPtr actxt,
			xmlParserErrors error,
			xmlSchemaAttrInfoPtr ni,
			xmlNodePtr node)
{
    xmlChar *msg = NULL, *str = NULL;

    xmlSchemaFormatNodeForError(&msg, actxt, node);
    msg = xmlStrcat(msg, BAD_CAST "The attribute '%s' is not allowed.\n");
    xmlSchemaErr(actxt, error, node, (const char *) msg,
	xmlSchemaFormatErrorNodeQName(&str, (xmlSchemaNodeInfoPtr) ni, node),
	NULL);
    FREE_AND_NULL(str)
    FREE_AND_NULL(msg)
}

static void
xmlSchemaComplexTypeErr(xmlSchemaAbstractCtxtPtr actxt,
		        xmlParserErrors error,
		        xmlNodePtr node,
			xmlSchemaTypePtr type ATTRIBUTE_UNUSED,
			const char *message,
			int nbval,
			int nbneg,
			xmlChar **values)
{
    xmlChar *str = NULL, *msg = NULL;
    xmlChar *localName, *nsName;
    const xmlChar *cur, *end;
    int i;

    xmlSchemaFormatNodeForError(&msg, actxt, node);
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".");
    if (nbval + nbneg > 0) {
	if (nbval + nbneg > 1) {
	    str = xmlStrdup(BAD_CAST " Expected is one of ( ");
	} else
	    str = xmlStrdup(BAD_CAST " Expected is ( ");
	nsName = NULL;

	for (i = 0; i < nbval + nbneg; i++) {
	    cur = values[i];
	    if (cur == NULL)
	        continue;
	    if ((cur[0] == 'n') && (cur[1] == 'o') && (cur[2] == 't') &&
	        (cur[3] == ' ')) {
		cur += 4;
		str = xmlStrcat(str, BAD_CAST "##other");
	    }
	    localName = NULL;

	    end = cur;
	    if (*end == '*') {
		localName = xmlStrdup(BAD_CAST "*");
		end++;
	    } else {
		while ((*end != 0) && (*end != '|'))
		    end++;
		localName = xmlStrncat(localName, BAD_CAST cur, end - cur);
	    }
	    if (*end != 0) {
		end++;
		if ((nbneg == 0) || (*end != '*') || (*localName != '*')) {
		    cur = end;
		    if (*end == '*') {
			nsName = xmlStrdup(BAD_CAST "{*}");
		    } else {
			while (*end != 0)
			    end++;

			if (i >= nbval)
			    nsName = xmlStrdup(BAD_CAST "{##other:");
			else
			    nsName = xmlStrdup(BAD_CAST "{");

			nsName = xmlStrncat(nsName, BAD_CAST cur, end - cur);
			nsName = xmlStrcat(nsName, BAD_CAST "}");
		    }
		    str = xmlStrcat(str, BAD_CAST nsName);
		    FREE_AND_NULL(nsName)
		} else {
		    FREE_AND_NULL(localName);
		    continue;
		}
	    }
	    str = xmlStrcat(str, BAD_CAST localName);
	    FREE_AND_NULL(localName);

	    if (i < nbval + nbneg -1)
		str = xmlStrcat(str, BAD_CAST ", ");
	}
	str = xmlStrcat(str, BAD_CAST " ).\n");
	msg = xmlStrcat(msg, BAD_CAST str);
	FREE_AND_NULL(str)
    } else
      msg = xmlStrcat(msg, BAD_CAST "\n");
    xmlSchemaErr(actxt, error, node, (const char *) msg, NULL, NULL);
    xmlFree(msg);
}

static void
xmlSchemaFacetErr(xmlSchemaAbstractCtxtPtr actxt,
		  xmlParserErrors error,
		  xmlNodePtr node,
		  const xmlChar *value,
		  unsigned long length,
		  xmlSchemaTypePtr type,
		  xmlSchemaFacetPtr facet,
		  const char *message,
		  const xmlChar *str1,
		  const xmlChar *str2)
{
    xmlChar *str = NULL, *msg = NULL;
    xmlSchemaTypeType facetType;
    int nodeType = xmlSchemaEvalErrorNodeType(actxt, node);

    xmlSchemaFormatNodeForError(&msg, actxt, node);
    if (error == XML_SCHEMAV_CVC_ENUMERATION_VALID) {
	facetType = XML_SCHEMA_FACET_ENUMERATION;
    } else
	facetType = facet->type;
    msg = xmlStrcat(msg, BAD_CAST "[");
    msg = xmlStrcat(msg, BAD_CAST "facet '");
    msg = xmlStrcat(msg, xmlSchemaFacetTypeToString(facetType));
    msg = xmlStrcat(msg, BAD_CAST "'] ");
    if (message == NULL) {
	if ((facetType == XML_SCHEMA_FACET_LENGTH) ||
	    (facetType == XML_SCHEMA_FACET_MINLENGTH) ||
	    (facetType == XML_SCHEMA_FACET_MAXLENGTH)) {

	    char len[25], actLen[25];

	    if (nodeType == XML_ATTRIBUTE_NODE)
		msg = xmlStrcat(msg, BAD_CAST "The value '%s' has a length of '%s'; ");
	    else
		msg = xmlStrcat(msg, BAD_CAST "The value has a length of '%s'; ");

	    snprintf(len, 24, "%lu", xmlSchemaGetFacetValueAsULong(facet));
	    snprintf(actLen, 24, "%lu", length);

	    if (facetType == XML_SCHEMA_FACET_LENGTH)
		msg = xmlStrcat(msg,
		BAD_CAST "this differs from the allowed length of '%s'.\n");
	    else if (facetType == XML_SCHEMA_FACET_MAXLENGTH)
		msg = xmlStrcat(msg,
		BAD_CAST "this exceeds the allowed maximum length of '%s'.\n");
	    else if (facetType == XML_SCHEMA_FACET_MINLENGTH)
		msg = xmlStrcat(msg,
		BAD_CAST "this underruns the allowed minimum length of '%s'.\n");

	    if (nodeType == XML_ATTRIBUTE_NODE)
		xmlSchemaErr3(actxt, error, node, (const char *) msg,
		    value, (const xmlChar *) actLen, (const xmlChar *) len);
	    else
		xmlSchemaErr(actxt, error, node, (const char *) msg,
		    (const xmlChar *) actLen, (const xmlChar *) len);

	} else if (facetType == XML_SCHEMA_FACET_ENUMERATION) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not an element "
		"of the set {%s}.\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value,
		xmlSchemaFormatFacetEnumSet(actxt, &str, type));
	} else if (facetType == XML_SCHEMA_FACET_PATTERN) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not accepted "
		"by the pattern '%s'.\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MININCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is less than the "
		"minimum value allowed ('%s').\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MAXINCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is greater than the "
		"maximum value allowed ('%s').\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MINEXCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' must be greater than "
		"'%s'.\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_MAXEXCLUSIVE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' must be less than "
		"'%s'.\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_TOTALDIGITS) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' has more "
		"digits than are allowed ('%s').\n");
	    xmlSchemaErr(actxt, error, node, (const char*) msg, value,
		facet->value);
	} else if (facetType == XML_SCHEMA_FACET_FRACTIONDIGITS) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' has more fractional "
		"digits than are allowed ('%s').\n");
	    xmlSchemaErr(actxt, error, node, (const char*) msg, value,
		facet->value);
	} else if (nodeType == XML_ATTRIBUTE_NODE) {
	    msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not facet-valid.\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, value, NULL);
	} else {
	    msg = xmlStrcat(msg, BAD_CAST "The value is not facet-valid.\n");
	    xmlSchemaErr(actxt, error, node, (const char *) msg, NULL, NULL);
	}
    } else {
	msg = xmlStrcat(msg, (const xmlChar *) message);
	msg = xmlStrcat(msg, BAD_CAST ".\n");
	xmlSchemaErr(actxt, error, node, (const char *) msg, str1, str2);
    }
    FREE_AND_NULL(str)
    xmlFree(msg);
}

#define VERROR(err, type, msg) \
    xmlSchemaCustomErr(ACTXT_CAST vctxt, err, NULL, type, msg, NULL, NULL);

#define VERROR_INT(func, msg) xmlSchemaInternalErr(ACTXT_CAST vctxt, func, msg);

#define PERROR_INT(func, msg) xmlSchemaInternalErr(ACTXT_CAST pctxt, func, msg);
#define PERROR_INT2(func, msg) xmlSchemaInternalErr(ACTXT_CAST ctxt, func, msg);

#define AERROR_INT(func, msg) xmlSchemaInternalErr(actxt, func, msg);


static void
xmlSchemaPMissingAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,
			 xmlSchemaBasicItemPtr ownerItem,
			 xmlNodePtr ownerElem,
			 const char *name,
			 const char *message)
{
    xmlChar *des = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, ownerItem, ownerElem);

    if (message != NULL)
	xmlSchemaPErr(ctxt, ownerElem, error, "%s: %s.\n", BAD_CAST des, BAD_CAST message);
    else
	xmlSchemaPErr(ctxt, ownerElem, error,
	    "%s: The attribute '%s' is required but missing.\n",
	    BAD_CAST des, BAD_CAST name);
    FREE_AND_NULL(des);
}


static void
xmlSchemaPResCompAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,
			 xmlSchemaBasicItemPtr ownerItem,
			 xmlNodePtr ownerElem,
			 const char *name,
			 const xmlChar *refName,
			 const xmlChar *refURI,
			 xmlSchemaTypeType refType,
			 const char *refTypeStr)
{
    xmlChar *des = NULL, *strA = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, ownerItem, ownerElem);
    if (refTypeStr == NULL)
	refTypeStr = (const char *) xmlSchemaItemTypeToStr(refType);
	xmlSchemaPErrExt(ctxt, ownerElem, error,
	    NULL, NULL, NULL,
	    "%s, attribute '%s': The QName value '%s' does not resolve to a(n) "
	    "%s.\n", BAD_CAST des, BAD_CAST name,
	    xmlSchemaFormatQName(&strA, refURI, refName),
	    BAD_CAST refTypeStr, NULL);
    FREE_AND_NULL(des)
    FREE_AND_NULL(strA)
}

static void
xmlSchemaPCustomAttrErr(xmlSchemaParserCtxtPtr ctxt,
			xmlParserErrors error,
			xmlChar **ownerDes,
			xmlSchemaBasicItemPtr ownerItem,
			xmlAttrPtr attr,
			const char *msg)
{
    xmlChar *des = NULL;

    if (ownerDes == NULL)
	xmlSchemaFormatItemForReport(&des, NULL, ownerItem, attr->parent);
    else if (*ownerDes == NULL) {
	xmlSchemaFormatItemForReport(ownerDes, NULL, ownerItem, attr->parent);
	des = *ownerDes;
    } else
	des = *ownerDes;
    if (attr == NULL) {
	xmlSchemaPErrExt(ctxt, NULL, error, NULL, NULL, NULL,
	    "%s, attribute '%s': %s.\n",
	    BAD_CAST des, (const xmlChar *) "Unknown",
	    (const xmlChar *) msg, NULL, NULL);
    } else {
	xmlSchemaPErrExt(ctxt, (xmlNodePtr) attr, error, NULL, NULL, NULL,
	    "%s, attribute '%s': %s.\n",
	    BAD_CAST des, attr->name, (const xmlChar *) msg, NULL, NULL);
    }
    if (ownerDes == NULL)
	FREE_AND_NULL(des);
}

static void
xmlSchemaPIllegalAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,
			 xmlSchemaBasicItemPtr ownerComp ATTRIBUTE_UNUSED,
			 xmlAttrPtr attr)
{
    xmlChar *strA = NULL, *strB = NULL;

    xmlSchemaFormatNodeForError(&strA, ACTXT_CAST ctxt, attr->parent);
    xmlSchemaErr4(ACTXT_CAST ctxt, error, (xmlNodePtr) attr,
	"%sThe attribute '%s' is not allowed.\n", BAD_CAST strA,
	xmlSchemaFormatQNameNs(&strB, attr->ns, attr->name),
	NULL, NULL);
    FREE_AND_NULL(strA);
    FREE_AND_NULL(strB);
}

static void
xmlSchemaPCustomErrExt(xmlSchemaParserCtxtPtr ctxt,
		    xmlParserErrors error,
		    xmlSchemaBasicItemPtr item,
		    xmlNodePtr itemElem,
		    const char *message,
		    const xmlChar *str1,
		    const xmlChar *str2,
		    const xmlChar *str3)
{
    xmlChar *des = NULL, *msg = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, item, itemElem);
    msg = xmlStrdup(BAD_CAST "%s: ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    if ((itemElem == NULL) && (item != NULL))
	itemElem = WXS_ITEM_NODE(item);
    xmlSchemaPErrExt(ctxt, itemElem, error, NULL, NULL, NULL,
	(const char *) msg, BAD_CAST des, str1, str2, str3, NULL);
    FREE_AND_NULL(des);
    FREE_AND_NULL(msg);
}

static void
xmlSchemaPCustomErr(xmlSchemaParserCtxtPtr ctxt,
		    xmlParserErrors error,
		    xmlSchemaBasicItemPtr item,
		    xmlNodePtr itemElem,
		    const char *message,
		    const xmlChar *str1)
{
    xmlSchemaPCustomErrExt(ctxt, error, item, itemElem, message,
	str1, NULL, NULL);
}

static void
xmlSchemaPAttrUseErr4(xmlSchemaParserCtxtPtr ctxt,
		    xmlParserErrors error,
		    xmlNodePtr node,
		    xmlSchemaBasicItemPtr ownerItem,
		    const xmlSchemaAttributeUsePtr attruse,
		    const char *message,
		    const xmlChar *str1, const xmlChar *str2,
		    const xmlChar *str3,const xmlChar *str4)
{
    xmlChar *str = NULL, *msg = NULL;

    xmlSchemaFormatItemForReport(&msg, NULL, ownerItem, NULL);
    msg = xmlStrcat(msg, BAD_CAST ", ");
    msg = xmlStrcat(msg,
	BAD_CAST xmlSchemaFormatItemForReport(&str, NULL,
	WXS_BASIC_CAST attruse, NULL));
    FREE_AND_NULL(str);
    msg = xmlStrcat(msg, BAD_CAST ": ");
    msg = xmlStrcat(msg, (const xmlChar *) message);
    msg = xmlStrcat(msg, BAD_CAST ".\n");
    xmlSchemaErr4(ACTXT_CAST ctxt, error, node,
	(const char *) msg, str1, str2, str3, str4);
    xmlFree(msg);
}

static void
xmlSchemaPIllegalFacetAtomicErr(xmlSchemaParserCtxtPtr ctxt,
			  xmlParserErrors error,
			  xmlSchemaTypePtr type,
			  xmlSchemaTypePtr baseType,
			  xmlSchemaFacetPtr facet)
{
    xmlChar *des = NULL, *strT = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, WXS_BASIC_CAST type, type->node);
    xmlSchemaPErrExt(ctxt, type->node, error, NULL, NULL, NULL,
	"%s: The facet '%s' is not allowed on types derived from the "
	"type %s.\n",
	BAD_CAST des, xmlSchemaFacetTypeToString(facet->type),
	xmlSchemaFormatItemForReport(&strT, NULL, WXS_BASIC_CAST baseType, NULL),
	NULL, NULL);
    FREE_AND_NULL(des);
    FREE_AND_NULL(strT);
}

static void
xmlSchemaPIllegalFacetListUnionErr(xmlSchemaParserCtxtPtr ctxt,
			  xmlParserErrors error,
			  xmlSchemaTypePtr type,
			  xmlSchemaFacetPtr facet)
{
    xmlChar *des = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, WXS_BASIC_CAST type,
	type->node);
    xmlSchemaPErr(ctxt, type->node, error,
	"%s: The facet '%s' is not allowed.\n",
	BAD_CAST des, xmlSchemaFacetTypeToString(facet->type));
    FREE_AND_NULL(des);
}

static void
xmlSchemaPMutualExclAttrErr(xmlSchemaParserCtxtPtr ctxt,
			 xmlParserErrors error,
			 xmlSchemaBasicItemPtr ownerItem,
			 xmlAttrPtr attr,
			 const char *name1,
			 const char *name2)
{
    xmlChar *des = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, WXS_BASIC_CAST ownerItem, attr->parent);
    xmlSchemaPErrExt(ctxt, (xmlNodePtr) attr, error, NULL, NULL, NULL,
	"%s: The attributes '%s' and '%s' are mutually exclusive.\n",
	BAD_CAST des, BAD_CAST name1, BAD_CAST name2, NULL, NULL);
    FREE_AND_NULL(des);
}

static void
xmlSchemaPSimpleTypeErr(xmlSchemaParserCtxtPtr ctxt,
			xmlParserErrors error,
			xmlSchemaBasicItemPtr ownerItem ATTRIBUTE_UNUSED,
			xmlNodePtr node,
			xmlSchemaTypePtr type,
			const char *expected,
			const xmlChar *value,
			const char *message,
			const xmlChar *str1,
			const xmlChar *str2)
{
    xmlChar *msg = NULL;

    xmlSchemaFormatNodeForError(&msg, ACTXT_CAST ctxt, node);
    if (message == NULL) {
	if (type != NULL) {
	    if (node->type == XML_ATTRIBUTE_NODE)
		msg = xmlStrcat(msg, BAD_CAST "'%s' is not a valid value of ");
	    else
		msg = xmlStrcat(msg, BAD_CAST "The character content is not a "
		"valid value of ");
	    if (! xmlSchemaIsGlobalItem(type))
		msg = xmlStrcat(msg, BAD_CAST "the local ");
	    else
		msg = xmlStrcat(msg, BAD_CAST "the ");

	    if (WXS_IS_ATOMIC(type))
		msg = xmlStrcat(msg, BAD_CAST "atomic type");
	    else if (WXS_IS_LIST(type))
		msg = xmlStrcat(msg, BAD_CAST "list type");
	    else if (WXS_IS_UNION(type))
		msg = xmlStrcat(msg, BAD_CAST "union type");

	    if (xmlSchemaIsGlobalItem(type)) {
		xmlChar *str = NULL;
		msg = xmlStrcat(msg, BAD_CAST " '");
		if (type->builtInType != 0) {
		    msg = xmlStrcat(msg, BAD_CAST "xs:");
		    msg = xmlStrcat(msg, type->name);
		} else
		    msg = xmlStrcat(msg,
			xmlSchemaFormatQName(&str,
			    type->targetNamespace, type->name));
		msg = xmlStrcat(msg, BAD_CAST "'.");
		FREE_AND_NULL(str);
	    }
	} else {
	    if (node->type == XML_ATTRIBUTE_NODE)
		msg = xmlStrcat(msg, BAD_CAST "The value '%s' is not valid.");
	    else
		msg = xmlStrcat(msg, BAD_CAST "The character content is not "
		"valid.");
	}
	if (expected) {
	    msg = xmlStrcat(msg, BAD_CAST " Expected is '");
	    msg = xmlStrcat(msg, BAD_CAST expected);
	    msg = xmlStrcat(msg, BAD_CAST "'.\n");
	} else
	    msg = xmlStrcat(msg, BAD_CAST "\n");
	if (node->type == XML_ATTRIBUTE_NODE)
	    xmlSchemaPErr(ctxt, node, error, (const char *) msg, value, NULL);
	else
	    xmlSchemaPErr(ctxt, node, error, (const char *) msg, NULL, NULL);
    } else {
	msg = xmlStrcat(msg, BAD_CAST message);
	msg = xmlStrcat(msg, BAD_CAST ".\n");
	xmlSchemaPErrExt(ctxt, node, error, NULL, NULL, NULL,
	     (const char*) msg, str1, str2, NULL, NULL, NULL);
    }
    
    FREE_AND_NULL(msg)
}

static void
xmlSchemaPContentErr(xmlSchemaParserCtxtPtr ctxt,
		     xmlParserErrors error,
		     xmlSchemaBasicItemPtr ownerItem,
		     xmlNodePtr ownerElem,
		     xmlNodePtr child,
		     const char *message,
		     const char *content)
{
    xmlChar *des = NULL;

    xmlSchemaFormatItemForReport(&des, NULL, ownerItem, ownerElem);
    if (message != NULL)
	xmlSchemaPErr2(ctxt, ownerElem, child, error,
	    "%s: %s.\n",
	    BAD_CAST des, BAD_CAST message);
    else {
	if (content != NULL) {
	    xmlSchemaPErr2(ctxt, ownerElem, child, error,
		"%s: The content is not valid. Expected is %s.\n",
		BAD_CAST des, BAD_CAST content);
	} else {
	    xmlSchemaPErr2(ctxt, ownerElem, child, error,
		"%s: The content is not valid.\n",
		BAD_CAST des, NULL);
	}
    }
    FREE_AND_NULL(des)
}








static xmlSchemaPtr
xmlSchemaNewSchema(xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaPtr ret;

    ret = (xmlSchemaPtr) xmlMalloc(sizeof(xmlSchema));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating schema", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchema));
    ret->dict = ctxt->dict;
    xmlDictReference(ret->dict);

    return (ret);
}

xmlSchemaFacetPtr
xmlSchemaNewFacet(void)
{
    xmlSchemaFacetPtr ret;

    ret = (xmlSchemaFacetPtr) xmlMalloc(sizeof(xmlSchemaFacet));
    if (ret == NULL) {
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaFacet));

    return (ret);
}

static xmlSchemaAnnotPtr
xmlSchemaNewAnnot(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node)
{
    xmlSchemaAnnotPtr ret;

    ret = (xmlSchemaAnnotPtr) xmlMalloc(sizeof(xmlSchemaAnnot));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating annotation", node);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAnnot));
    ret->content = node;
    return (ret);
}

static xmlSchemaItemListPtr
xmlSchemaItemListCreate(void)
{
    xmlSchemaItemListPtr ret;

    ret = xmlMalloc(sizeof(xmlSchemaItemList));
    if (ret == NULL) {
	xmlSchemaPErrMemory(NULL,
	    "allocating an item list structure", NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaItemList));
    return (ret);
}

static void
xmlSchemaItemListClear(xmlSchemaItemListPtr list)
{
    if (list->items != NULL) {
	xmlFree(list->items);
	list->items = NULL;
    }
    list->nbItems = 0;
    list->sizeItems = 0;
}

static int
xmlSchemaItemListAdd(xmlSchemaItemListPtr list, void *item)
{
    if (list->items == NULL) {
	list->items = (void **) xmlMalloc(
	    20 * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "allocating new item list", NULL);
	    return(-1);
	}
	list->sizeItems = 20;
    } else if (list->sizeItems <= list->nbItems) {
	list->sizeItems *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->sizeItems * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "growing item list", NULL);
	    list->sizeItems = 0;
	    return(-1);
	}
    }
    list->items[list->nbItems++] = item;
    return(0);
}

static int
xmlSchemaItemListAddSize(xmlSchemaItemListPtr list,
			 int initialSize,
			 void *item)
{
    if (list->items == NULL) {
	if (initialSize <= 0)
	    initialSize = 1;
	list->items = (void **) xmlMalloc(
	    initialSize * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "allocating new item list", NULL);
	    return(-1);
	}
	list->sizeItems = initialSize;
    } else if (list->sizeItems <= list->nbItems) {
	list->sizeItems *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->sizeItems * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "growing item list", NULL);
	    list->sizeItems = 0;
	    return(-1);
	}
    }
    list->items[list->nbItems++] = item;
    return(0);
}

static int
xmlSchemaItemListInsert(xmlSchemaItemListPtr list, void *item, int idx)
{
    if (list->items == NULL) {
	list->items = (void **) xmlMalloc(
	    20 * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "allocating new item list", NULL);
	    return(-1);
	}
	list->sizeItems = 20;
    } else if (list->sizeItems <= list->nbItems) {
	list->sizeItems *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->sizeItems * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "growing item list", NULL);
	    list->sizeItems = 0;
	    return(-1);
	}
    }
    if (idx >= list->nbItems) {
	list->items[list->nbItems++] = item;
    } else {
	int i;
	for (i = list->nbItems; i > idx; i--)
	    list->items[i] = list->items[i-1];
	list->items[idx] = item;
	list->nbItems++;
    }
    return(0);
}

#if 0 
static int
xmlSchemaItemListInsertSize(xmlSchemaItemListPtr list,
			    int initialSize,
			    void *item,
			    int idx)
{
    if (list->items == NULL) {
	if (initialSize <= 0)
	    initialSize = 1;
	list->items = (void **) xmlMalloc(
	    initialSize * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "allocating new item list", NULL);
	    return(-1);
	}
	list->sizeItems = initialSize;
    } else if (list->sizeItems <= list->nbItems) {
	list->sizeItems *= 2;
	list->items = (void **) xmlRealloc(list->items,
	    list->sizeItems * sizeof(void *));
	if (list->items == NULL) {
	    xmlSchemaPErrMemory(NULL, "growing item list", NULL);
	    list->sizeItems = 0;
	    return(-1);
	}
    }
    if (idx >= list->nbItems) {
	list->items[list->nbItems++] = item;
    } else {
	int i;
	for (i = list->nbItems; i > idx; i--)
	    list->items[i] = list->items[i-1];
	list->items[idx] = item;
	list->nbItems++;
    }
    return(0);
}
#endif

static int
xmlSchemaItemListRemove(xmlSchemaItemListPtr list, int idx)
{
    int i;
    if ((list->items == NULL) || (idx >= list->nbItems)) {
	xmlSchemaPSimpleErr("Internal error: xmlSchemaItemListRemove, "
	    "index error.\n");
	return(-1);
    }

    if (list->nbItems == 1) {
	
	xmlFree(list->items);
	list->items = NULL;
	list->nbItems = 0;
	list->sizeItems = 0;
    } else if (list->nbItems -1 == idx) {
	list->nbItems--;
    } else {
	for (i = idx; i < list->nbItems -1; i++)
	    list->items[i] = list->items[i+1];
	list->nbItems--;
    }
    return(0);
}

static void
xmlSchemaItemListFree(xmlSchemaItemListPtr list)
{
    if (list == NULL)
	return;
    if (list->items != NULL)
	xmlFree(list->items);
    xmlFree(list);
}

static void
xmlSchemaBucketFree(xmlSchemaBucketPtr bucket)
{
    if (bucket == NULL)
	return;
    if (bucket->globals != NULL) {
	xmlSchemaComponentListFree(bucket->globals);
	xmlSchemaItemListFree(bucket->globals);
    }
    if (bucket->locals != NULL) {
	xmlSchemaComponentListFree(bucket->locals);
	xmlSchemaItemListFree(bucket->locals);
    }
    if (bucket->relations != NULL) {
	xmlSchemaSchemaRelationPtr prev, cur = bucket->relations;
	do {
	    prev = cur;
	    cur = cur->next;
	    xmlFree(prev);
	} while (cur != NULL);
    }
    if ((! bucket->preserveDoc) && (bucket->doc != NULL)) {
	xmlFreeDoc(bucket->doc);
    }
    if (bucket->type == XML_SCHEMA_SCHEMA_IMPORT) {
	if (WXS_IMPBUCKET(bucket)->schema != NULL)
	    xmlSchemaFree(WXS_IMPBUCKET(bucket)->schema);
    }
    xmlFree(bucket);
}

static xmlSchemaBucketPtr
xmlSchemaBucketCreate(xmlSchemaParserCtxtPtr pctxt,
			 int type, const xmlChar *targetNamespace)
{
    xmlSchemaBucketPtr ret;
    int size;
    xmlSchemaPtr mainSchema;

    if (WXS_CONSTRUCTOR(pctxt)->mainSchema == NULL) {
	PERROR_INT("xmlSchemaBucketCreate",
	    "no main schema on constructor");
	return(NULL);
    }
    mainSchema = WXS_CONSTRUCTOR(pctxt)->mainSchema;
    
    if (WXS_IS_BUCKET_INCREDEF(type))
	size = sizeof(xmlSchemaInclude);
    else
	size = sizeof(xmlSchemaImport);
    ret = (xmlSchemaBucketPtr) xmlMalloc(size);
    if (ret == NULL) {
	xmlSchemaPErrMemory(NULL, "allocating schema bucket", NULL);
	return(NULL);
    }
    memset(ret, 0, size);
    ret->targetNamespace = targetNamespace;
    ret->type = type;
    ret->globals = xmlSchemaItemListCreate();
    if (ret->globals == NULL) {
	xmlFree(ret);
	return(NULL);
    }
    ret->locals = xmlSchemaItemListCreate();
    if (ret->locals == NULL) {
	xmlFree(ret);
	return(NULL);
    }
    if (! WXS_HAS_BUCKETS(pctxt)) {
	if (WXS_IS_BUCKET_INCREDEF(type)) {
	    PERROR_INT("xmlSchemaBucketCreate",
		"first bucket but it's an include or redefine");
	    xmlSchemaBucketFree(ret);
	    return(NULL);
	}
	
	ret->type = XML_SCHEMA_SCHEMA_MAIN;
	
	WXS_CONSTRUCTOR(pctxt)->mainBucket = ret;
	WXS_IMPBUCKET(ret)->schema = mainSchema;
	mainSchema->targetNamespace = targetNamespace;
    } else {
	if (type == XML_SCHEMA_SCHEMA_MAIN) {
	    PERROR_INT("xmlSchemaBucketCreate",
		"main bucket but it's not the first one");
	    xmlSchemaBucketFree(ret);
	    return(NULL);
	} else if (type == XML_SCHEMA_SCHEMA_IMPORT) {
	    WXS_IMPBUCKET(ret)->schema = xmlSchemaNewSchema(pctxt);
	    if (WXS_IMPBUCKET(ret)->schema == NULL) {
		xmlSchemaBucketFree(ret);
		return(NULL);
	    }
	    WXS_IMPBUCKET(ret)->schema->targetNamespace = targetNamespace;
	}
    }
    if (WXS_IS_BUCKET_IMPMAIN(type)) {
	int res;
	if (mainSchema->schemasImports == NULL) {
	    mainSchema->schemasImports = xmlHashCreateDict(5,
		WXS_CONSTRUCTOR(pctxt)->dict);
	    if (mainSchema->schemasImports == NULL) {
		xmlSchemaBucketFree(ret);
		return(NULL);
	    }
	}
	if (targetNamespace == NULL)
	    res = xmlHashAddEntry(mainSchema->schemasImports,
		XML_SCHEMAS_NO_NAMESPACE, ret);
	else
	    res = xmlHashAddEntry(mainSchema->schemasImports,
		targetNamespace, ret);
	if (res != 0) {
	    PERROR_INT("xmlSchemaBucketCreate",
		"failed to add the schema bucket to the hash");
	    xmlSchemaBucketFree(ret);
	    return(NULL);
	}
    } else {
	
	if (WXS_IS_BUCKET_IMPMAIN(WXS_CONSTRUCTOR(pctxt)->bucket->type))
	    WXS_INCBUCKET(ret)->ownerImport =
		WXS_IMPBUCKET(WXS_CONSTRUCTOR(pctxt)->bucket);
	else
	    WXS_INCBUCKET(ret)->ownerImport =
		WXS_INCBUCKET(WXS_CONSTRUCTOR(pctxt)->bucket)->ownerImport;

	
	if (mainSchema->includes == NULL) {
	    mainSchema->includes = xmlSchemaItemListCreate();
	    if (mainSchema->includes == NULL) {
		xmlSchemaBucketFree(ret);
		return(NULL);
	    }
	}
	xmlSchemaItemListAdd(mainSchema->includes, ret);
    }
    if (xmlSchemaItemListAdd(WXS_CONSTRUCTOR(pctxt)->buckets, ret) == -1)
	return(NULL);
    return(ret);
}

static int
xmlSchemaAddItemSize(xmlSchemaItemListPtr *list, int initialSize, void *item)
{
    if (*list == NULL) {
	*list = xmlSchemaItemListCreate();
	if (*list == NULL)
	    return(-1);
    }
    xmlSchemaItemListAddSize(*list, initialSize, item);
    return(0);
}

static void
xmlSchemaFreeAnnot(xmlSchemaAnnotPtr annot)
{
    if (annot == NULL)
        return;
    if (annot->next == NULL) {
	xmlFree(annot);
    } else {
	xmlSchemaAnnotPtr prev;

	do {
	    prev = annot;
	    annot = annot->next;
	    xmlFree(prev);
	} while (annot != NULL);
    }
}

static void
xmlSchemaFreeNotation(xmlSchemaNotationPtr nota)
{
    if (nota == NULL)
        return;
    xmlFree(nota);
}

static void
xmlSchemaFreeAttribute(xmlSchemaAttributePtr attr)
{
    if (attr == NULL)
        return;
    if (attr->annot != NULL)
	xmlSchemaFreeAnnot(attr->annot);
    if (attr->defVal != NULL)
	xmlSchemaFreeValue(attr->defVal);
    xmlFree(attr);
}

static void
xmlSchemaFreeAttributeUse(xmlSchemaAttributeUsePtr use)
{
    if (use == NULL)
        return;
    if (use->annot != NULL)
	xmlSchemaFreeAnnot(use->annot);
    if (use->defVal != NULL)
	xmlSchemaFreeValue(use->defVal);
    xmlFree(use);
}

static void
xmlSchemaFreeAttributeUseProhib(xmlSchemaAttributeUseProhibPtr prohib)
{
    if (prohib == NULL)
        return;
    xmlFree(prohib);
}

static void
xmlSchemaFreeWildcardNsSet(xmlSchemaWildcardNsPtr set)
{
    xmlSchemaWildcardNsPtr next;

    while (set != NULL) {
	next = set->next;
	xmlFree(set);
	set = next;
    }
}

void
xmlSchemaFreeWildcard(xmlSchemaWildcardPtr wildcard)
{
    if (wildcard == NULL)
        return;
    if (wildcard->annot != NULL)
        xmlSchemaFreeAnnot(wildcard->annot);
    if (wildcard->nsSet != NULL)
	xmlSchemaFreeWildcardNsSet(wildcard->nsSet);
    if (wildcard->negNsSet != NULL)
	xmlFree(wildcard->negNsSet);
    xmlFree(wildcard);
}

static void
xmlSchemaFreeAttributeGroup(xmlSchemaAttributeGroupPtr attrGr)
{
    if (attrGr == NULL)
        return;
    if (attrGr->annot != NULL)
        xmlSchemaFreeAnnot(attrGr->annot);
    if (attrGr->attrUses != NULL)
	xmlSchemaItemListFree(WXS_LIST_CAST attrGr->attrUses);
    xmlFree(attrGr);
}

static void
xmlSchemaFreeQNameRef(xmlSchemaQNameRefPtr item)
{
    xmlFree(item);
}

static void
xmlSchemaFreeTypeLinkList(xmlSchemaTypeLinkPtr link)
{
    xmlSchemaTypeLinkPtr next;

    while (link != NULL) {
	next = link->next;
	xmlFree(link);
	link = next;
    }
}

static void
xmlSchemaFreeIDCStateObjList(xmlSchemaIDCStateObjPtr sto)
{
    xmlSchemaIDCStateObjPtr next;
    while (sto != NULL) {
	next = sto->next;
	if (sto->history != NULL)
	    xmlFree(sto->history);
	if (sto->xpathCtxt != NULL)
	    xmlFreeStreamCtxt((xmlStreamCtxtPtr) sto->xpathCtxt);
	xmlFree(sto);
	sto = next;
    }
}

static void
xmlSchemaFreeIDC(xmlSchemaIDCPtr idcDef)
{
    xmlSchemaIDCSelectPtr cur, prev;

    if (idcDef == NULL)
	return;
    if (idcDef->annot != NULL)
        xmlSchemaFreeAnnot(idcDef->annot);
    
    if (idcDef->selector != NULL) {
	if (idcDef->selector->xpathComp != NULL)
	    xmlFreePattern((xmlPatternPtr) idcDef->selector->xpathComp);
	xmlFree(idcDef->selector);
    }
    
    if (idcDef->fields != NULL) {
	cur = idcDef->fields;
	do {
	    prev = cur;
	    cur = cur->next;
	    if (prev->xpathComp != NULL)
		xmlFreePattern((xmlPatternPtr) prev->xpathComp);
	    xmlFree(prev);
	} while (cur != NULL);
    }
    xmlFree(idcDef);
}

static void
xmlSchemaFreeElement(xmlSchemaElementPtr elem)
{
    if (elem == NULL)
        return;
    if (elem->annot != NULL)
        xmlSchemaFreeAnnot(elem->annot);
    if (elem->contModel != NULL)
        xmlRegFreeRegexp(elem->contModel);
    if (elem->defVal != NULL)
	xmlSchemaFreeValue(elem->defVal);
    xmlFree(elem);
}

void
xmlSchemaFreeFacet(xmlSchemaFacetPtr facet)
{
    if (facet == NULL)
        return;
    if (facet->val != NULL)
        xmlSchemaFreeValue(facet->val);
    if (facet->regexp != NULL)
        xmlRegFreeRegexp(facet->regexp);
    if (facet->annot != NULL)
        xmlSchemaFreeAnnot(facet->annot);
    xmlFree(facet);
}

void
xmlSchemaFreeType(xmlSchemaTypePtr type)
{
    if (type == NULL)
        return;
    if (type->annot != NULL)
        xmlSchemaFreeAnnot(type->annot);
    if (type->facets != NULL) {
        xmlSchemaFacetPtr facet, next;

        facet = type->facets;
        while (facet != NULL) {
            next = facet->next;
            xmlSchemaFreeFacet(facet);
            facet = next;
        }
    }
    if (type->attrUses != NULL)
	xmlSchemaItemListFree((xmlSchemaItemListPtr) type->attrUses);
    if (type->memberTypes != NULL)
	xmlSchemaFreeTypeLinkList(type->memberTypes);
    if (type->facetSet != NULL) {
	xmlSchemaFacetLinkPtr next, link;

	link = type->facetSet;
	do {
	    next = link->next;
	    xmlFree(link);
	    link = next;
	} while (link != NULL);
    }
    if (type->contModel != NULL)
        xmlRegFreeRegexp(type->contModel);
    xmlFree(type);
}

static void
xmlSchemaFreeModelGroupDef(xmlSchemaModelGroupDefPtr item)
{
    if (item->annot != NULL)
	xmlSchemaFreeAnnot(item->annot);
    xmlFree(item);
}

static void
xmlSchemaFreeModelGroup(xmlSchemaModelGroupPtr item)
{
    if (item->annot != NULL)
	xmlSchemaFreeAnnot(item->annot);
    xmlFree(item);
}

static void
xmlSchemaComponentListFree(xmlSchemaItemListPtr list)
{
    if ((list == NULL) || (list->nbItems == 0))
	return;
    {
	xmlSchemaTreeItemPtr item;
	xmlSchemaTreeItemPtr *items = (xmlSchemaTreeItemPtr *) list->items;
	int i;

	for (i = 0; i < list->nbItems; i++) {
	    item = items[i];
	    if (item == NULL)
		continue;
	    switch (item->type) {
		case XML_SCHEMA_TYPE_SIMPLE:
		case XML_SCHEMA_TYPE_COMPLEX:
		    xmlSchemaFreeType((xmlSchemaTypePtr) item);
		    break;
		case XML_SCHEMA_TYPE_ATTRIBUTE:
		    xmlSchemaFreeAttribute((xmlSchemaAttributePtr) item);
		    break;
		case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
		    xmlSchemaFreeAttributeUse((xmlSchemaAttributeUsePtr) item);
		    break;
		case XML_SCHEMA_EXTRA_ATTR_USE_PROHIB:
		    xmlSchemaFreeAttributeUseProhib(
			(xmlSchemaAttributeUseProhibPtr) item);
		    break;
		case XML_SCHEMA_TYPE_ELEMENT:
		    xmlSchemaFreeElement((xmlSchemaElementPtr) item);
		    break;
		case XML_SCHEMA_TYPE_PARTICLE:
		    if (item->annot != NULL)
			xmlSchemaFreeAnnot(item->annot);
		    xmlFree(item);
		    break;
		case XML_SCHEMA_TYPE_SEQUENCE:
		case XML_SCHEMA_TYPE_CHOICE:
		case XML_SCHEMA_TYPE_ALL:
		    xmlSchemaFreeModelGroup((xmlSchemaModelGroupPtr) item);
		    break;
		case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		    xmlSchemaFreeAttributeGroup(
			(xmlSchemaAttributeGroupPtr) item);
		    break;
		case XML_SCHEMA_TYPE_GROUP:
		    xmlSchemaFreeModelGroupDef(
			(xmlSchemaModelGroupDefPtr) item);
		    break;
		case XML_SCHEMA_TYPE_ANY:
		case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
		    xmlSchemaFreeWildcard((xmlSchemaWildcardPtr) item);
		    break;
		case XML_SCHEMA_TYPE_IDC_KEY:
		case XML_SCHEMA_TYPE_IDC_UNIQUE:
		case XML_SCHEMA_TYPE_IDC_KEYREF:
		    xmlSchemaFreeIDC((xmlSchemaIDCPtr) item);
		    break;
		case XML_SCHEMA_TYPE_NOTATION:
		    xmlSchemaFreeNotation((xmlSchemaNotationPtr) item);
		    break;
		case XML_SCHEMA_EXTRA_QNAMEREF:
		    xmlSchemaFreeQNameRef((xmlSchemaQNameRefPtr) item);
		    break;
		default: {
		    
		    xmlSchemaPSimpleInternalErr(NULL,
			"Internal error: xmlSchemaComponentListFree, "
			"unexpected component type '%s'\n",
			(const xmlChar *) WXS_ITEM_TYPE_NAME(item));
			 }
		    break;
	    }
	}
	list->nbItems = 0;
    }
}

void
xmlSchemaFree(xmlSchemaPtr schema)
{
    if (schema == NULL)
        return;
    
    if (schema->volatiles != NULL)
	TODO
    if (schema->notaDecl != NULL)
        xmlHashFree(schema->notaDecl, NULL);
    if (schema->attrDecl != NULL)
        xmlHashFree(schema->attrDecl, NULL);
    if (schema->attrgrpDecl != NULL)
        xmlHashFree(schema->attrgrpDecl, NULL);
    if (schema->elemDecl != NULL)
        xmlHashFree(schema->elemDecl, NULL);
    if (schema->typeDecl != NULL)
        xmlHashFree(schema->typeDecl, NULL);
    if (schema->groupDecl != NULL)
        xmlHashFree(schema->groupDecl, NULL);
    if (schema->idcDef != NULL)
        xmlHashFree(schema->idcDef, NULL);

    if (schema->schemasImports != NULL)
	xmlHashFree(schema->schemasImports,
		    (xmlHashDeallocator) xmlSchemaBucketFree);
    if (schema->includes != NULL) {
	xmlSchemaItemListPtr list = (xmlSchemaItemListPtr) schema->includes;
	int i;
	for (i = 0; i < list->nbItems; i++) {
	    xmlSchemaBucketFree((xmlSchemaBucketPtr) list->items[i]);
	}
	xmlSchemaItemListFree(list);
    }
    if (schema->annot != NULL)
        xmlSchemaFreeAnnot(schema->annot);
    

    xmlDictFree(schema->dict);
    xmlFree(schema);
}


#ifdef LIBXML_OUTPUT_ENABLED

static void
xmlSchemaTypeDump(xmlSchemaTypePtr type, FILE * output); 

static void
xmlSchemaElementDump(xmlSchemaElementPtr elem, FILE * output,
                     const xmlChar * name ATTRIBUTE_UNUSED,
		     const xmlChar * namespace ATTRIBUTE_UNUSED,
                     const xmlChar * context ATTRIBUTE_UNUSED)
{
    if (elem == NULL)
        return;


    fprintf(output, "Element");
    if (elem->flags & XML_SCHEMAS_ELEM_GLOBAL)
	fprintf(output, " (global)");
    fprintf(output, ": '%s' ", elem->name);
    if (namespace != NULL)
	fprintf(output, "ns '%s'", namespace);
    fprintf(output, "\n");
#if 0
    if ((elem->minOccurs != 1) || (elem->maxOccurs != 1)) {
	fprintf(output, "  min %d ", elem->minOccurs);
        if (elem->maxOccurs >= UNBOUNDED)
            fprintf(output, "max: unbounded\n");
        else if (elem->maxOccurs != 1)
            fprintf(output, "max: %d\n", elem->maxOccurs);
        else
            fprintf(output, "\n");
    }
#endif
    if ((elem->flags & XML_SCHEMAS_ELEM_NILLABLE) ||
	(elem->flags & XML_SCHEMAS_ELEM_ABSTRACT) ||
	(elem->flags & XML_SCHEMAS_ELEM_FIXED) ||
	(elem->flags & XML_SCHEMAS_ELEM_DEFAULT)) {
	fprintf(output, "  props: ");
	if (elem->flags & XML_SCHEMAS_ELEM_FIXED)
	    fprintf(output, "[fixed] ");
	if (elem->flags & XML_SCHEMAS_ELEM_DEFAULT)
	    fprintf(output, "[default] ");
	if (elem->flags & XML_SCHEMAS_ELEM_ABSTRACT)
	    fprintf(output, "[abstract] ");
	if (elem->flags & XML_SCHEMAS_ELEM_NILLABLE)
	    fprintf(output, "[nillable] ");
	fprintf(output, "\n");
    }
    if (elem->value != NULL)
	fprintf(output, "  value: '%s'\n", elem->value);
    if (elem->namedType != NULL) {
	fprintf(output, "  type: '%s' ", elem->namedType);
	if (elem->namedTypeNs != NULL)
	    fprintf(output, "ns '%s'\n", elem->namedTypeNs);
	else
	    fprintf(output, "\n");
    } else if (elem->subtypes != NULL) {
	xmlSchemaTypeDump(elem->subtypes, output);
    }
    if (elem->substGroup != NULL) {
	fprintf(output, "  substitutionGroup: '%s' ", elem->substGroup);
	if (elem->substGroupNs != NULL)
	    fprintf(output, "ns '%s'\n", elem->substGroupNs);
	else
	    fprintf(output, "\n");
    }
}

static void
xmlSchemaAnnotDump(FILE * output, xmlSchemaAnnotPtr annot)
{
    xmlChar *content;

    if (annot == NULL)
        return;

    content = xmlNodeGetContent(annot->content);
    if (content != NULL) {
        fprintf(output, "  Annot: %s\n", content);
        xmlFree(content);
    } else
        fprintf(output, "  Annot: empty\n");
}

static void
xmlSchemaContentModelDump(xmlSchemaParticlePtr particle, FILE * output, int depth)
{
    xmlChar *str = NULL;
    xmlSchemaTreeItemPtr term;
    char shift[100];
    int i;

    if (particle == NULL)
	return;
    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;
    fprintf(output, "%s", shift);
    if (particle->children == NULL) {
	fprintf(output, "MISSING particle term\n");
	return;
    }
    term = particle->children;
    if (term == NULL) {
	fprintf(output, "(NULL)");
    } else {
	switch (term->type) {
	    case XML_SCHEMA_TYPE_ELEMENT:
		fprintf(output, "ELEM '%s'", xmlSchemaFormatQName(&str,
		    ((xmlSchemaElementPtr)term)->targetNamespace,
		    ((xmlSchemaElementPtr)term)->name));
		FREE_AND_NULL(str);
		break;
	    case XML_SCHEMA_TYPE_SEQUENCE:
		fprintf(output, "SEQUENCE");
		break;
	    case XML_SCHEMA_TYPE_CHOICE:
		fprintf(output, "CHOICE");
		break;
	    case XML_SCHEMA_TYPE_ALL:
		fprintf(output, "ALL");
		break;
	    case XML_SCHEMA_TYPE_ANY:
		fprintf(output, "ANY");
		break;
	    default:
		fprintf(output, "UNKNOWN\n");
		return;
	}
    }
    if (particle->minOccurs != 1)
	fprintf(output, " min: %d", particle->minOccurs);
    if (particle->maxOccurs >= UNBOUNDED)
	fprintf(output, " max: unbounded");
    else if (particle->maxOccurs != 1)
	fprintf(output, " max: %d", particle->maxOccurs);
    fprintf(output, "\n");
    if (term &&
	((term->type == XML_SCHEMA_TYPE_SEQUENCE) ||
	 (term->type == XML_SCHEMA_TYPE_CHOICE) ||
	 (term->type == XML_SCHEMA_TYPE_ALL)) &&
	 (term->children != NULL)) {
	xmlSchemaContentModelDump((xmlSchemaParticlePtr) term->children,
	    output, depth +1);
    }
    if (particle->next != NULL)
	xmlSchemaContentModelDump((xmlSchemaParticlePtr) particle->next,
		output, depth);
}

static void
xmlSchemaAttrUsesDump(xmlSchemaItemListPtr uses, FILE * output)
{
    xmlSchemaAttributeUsePtr use;
    xmlSchemaAttributeUseProhibPtr prohib;
    xmlSchemaQNameRefPtr ref;
    const xmlChar *name, *tns;
    xmlChar *str = NULL;
    int i;

    if ((uses == NULL) || (uses->nbItems == 0))
        return;

    fprintf(output, "  attributes:\n");
    for (i = 0; i < uses->nbItems; i++) {
	use = uses->items[i];
	if (use->type == XML_SCHEMA_EXTRA_ATTR_USE_PROHIB) {
	    fprintf(output, "  [prohibition] ");
	    prohib = (xmlSchemaAttributeUseProhibPtr) use;
	    name = prohib->name;
	    tns = prohib->targetNamespace;
	} else if (use->type == XML_SCHEMA_EXTRA_QNAMEREF) {
	    fprintf(output, "  [reference] ");
	    ref = (xmlSchemaQNameRefPtr) use;
	    name = ref->name;
	    tns = ref->targetNamespace;
	} else {
	    fprintf(output, "  [use] ");
	    name = WXS_ATTRUSE_DECL_NAME(use);
	    tns = WXS_ATTRUSE_DECL_TNS(use);
	}
	fprintf(output, "'%s'\n",
	    (const char *) xmlSchemaFormatQName(&str, tns, name));
	FREE_AND_NULL(str);
    }
}

static void
xmlSchemaTypeDump(xmlSchemaTypePtr type, FILE * output)
{
    if (type == NULL) {
        fprintf(output, "Type: NULL\n");
        return;
    }
    fprintf(output, "Type: ");
    if (type->name != NULL)
        fprintf(output, "'%s' ", type->name);
    else
        fprintf(output, "(no name) ");
    if (type->targetNamespace != NULL)
	fprintf(output, "ns '%s' ", type->targetNamespace);
    switch (type->type) {
        case XML_SCHEMA_TYPE_BASIC:
            fprintf(output, "[basic] ");
            break;
        case XML_SCHEMA_TYPE_SIMPLE:
            fprintf(output, "[simple] ");
            break;
        case XML_SCHEMA_TYPE_COMPLEX:
            fprintf(output, "[complex] ");
            break;
        case XML_SCHEMA_TYPE_SEQUENCE:
            fprintf(output, "[sequence] ");
            break;
        case XML_SCHEMA_TYPE_CHOICE:
            fprintf(output, "[choice] ");
            break;
        case XML_SCHEMA_TYPE_ALL:
            fprintf(output, "[all] ");
            break;
        case XML_SCHEMA_TYPE_UR:
            fprintf(output, "[ur] ");
            break;
        case XML_SCHEMA_TYPE_RESTRICTION:
            fprintf(output, "[restriction] ");
            break;
        case XML_SCHEMA_TYPE_EXTENSION:
            fprintf(output, "[extension] ");
            break;
        default:
            fprintf(output, "[unknown type %d] ", type->type);
            break;
    }
    fprintf(output, "content: ");
    switch (type->contentType) {
        case XML_SCHEMA_CONTENT_UNKNOWN:
            fprintf(output, "[unknown] ");
            break;
        case XML_SCHEMA_CONTENT_EMPTY:
            fprintf(output, "[empty] ");
            break;
        case XML_SCHEMA_CONTENT_ELEMENTS:
            fprintf(output, "[element] ");
            break;
        case XML_SCHEMA_CONTENT_MIXED:
            fprintf(output, "[mixed] ");
            break;
        case XML_SCHEMA_CONTENT_MIXED_OR_ELEMENTS:
	
            break;
        case XML_SCHEMA_CONTENT_BASIC:
            fprintf(output, "[basic] ");
            break;
        case XML_SCHEMA_CONTENT_SIMPLE:
            fprintf(output, "[simple] ");
            break;
        case XML_SCHEMA_CONTENT_ANY:
            fprintf(output, "[any] ");
            break;
    }
    fprintf(output, "\n");
    if (type->base != NULL) {
        fprintf(output, "  base type: '%s'", type->base);
	if (type->baseNs != NULL)
	    fprintf(output, " ns '%s'\n", type->baseNs);
	else
	    fprintf(output, "\n");
    }
    if (type->attrUses != NULL)
	xmlSchemaAttrUsesDump(type->attrUses, output);
    if (type->annot != NULL)
        xmlSchemaAnnotDump(output, type->annot);
#ifdef DUMP_CONTENT_MODEL
    if ((type->type == XML_SCHEMA_TYPE_COMPLEX) &&
	(type->subtypes != NULL)) {
	xmlSchemaContentModelDump((xmlSchemaParticlePtr) type->subtypes,
	    output, 1);
    }
#endif
}

void
xmlSchemaDump(FILE * output, xmlSchemaPtr schema)
{
    if (output == NULL)
        return;
    if (schema == NULL) {
        fprintf(output, "Schemas: NULL\n");
        return;
    }
    fprintf(output, "Schemas: ");
    if (schema->name != NULL)
        fprintf(output, "%s, ", schema->name);
    else
        fprintf(output, "no name, ");
    if (schema->targetNamespace != NULL)
        fprintf(output, "%s", (const char *) schema->targetNamespace);
    else
        fprintf(output, "no target namespace");
    fprintf(output, "\n");
    if (schema->annot != NULL)
        xmlSchemaAnnotDump(output, schema->annot);
    xmlHashScan(schema->typeDecl, (xmlHashScanner) xmlSchemaTypeDump,
                output);
    xmlHashScanFull(schema->elemDecl,
                    (xmlHashScannerFull) xmlSchemaElementDump, output);
}

#ifdef DEBUG_IDC_NODE_TABLE
static void
xmlSchemaDebugDumpIDCTable(FILE * output,
			   const xmlChar *namespaceName,
			   const xmlChar *localName,
			   xmlSchemaPSVIIDCBindingPtr bind)
{
    xmlChar *str = NULL;
    const xmlChar *value;
    xmlSchemaPSVIIDCNodePtr tab;
    xmlSchemaPSVIIDCKeyPtr key;
    int i, j, res;

    fprintf(output, "IDC: TABLES on '%s'\n",
	xmlSchemaFormatQName(&str, namespaceName, localName));
    FREE_AND_NULL(str)

    if (bind == NULL)
	return;
    do {
	fprintf(output, "IDC:   BINDING '%s' (%d)\n",
	    xmlSchemaGetComponentQName(&str,
		bind->definition), bind->nbNodes);
	FREE_AND_NULL(str)
	for (i = 0; i < bind->nbNodes; i++) {
	    tab = bind->nodeTable[i];
	    fprintf(output, "         ( ");
	    for (j = 0; j < bind->definition->nbFields; j++) {
		key = tab->keys[j];
		if ((key != NULL) && (key->val != NULL)) {
		    res = xmlSchemaGetCanonValue(key->val, &value);
		    if (res >= 0)
			fprintf(output, "'%s' ", value);
		    else
			fprintf(output, "CANON-VALUE-FAILED ");
		    if (res == 0)
			FREE_AND_NULL(value)
		} else if (key != NULL)
		    fprintf(output, "(no val), ");
		else
		    fprintf(output, "(key missing), ");
	    }
	    fprintf(output, ")\n");
	}
	if (bind->dupls && bind->dupls->nbItems) {
	    fprintf(output, "IDC:     dupls (%d):\n", bind->dupls->nbItems);
	    for (i = 0; i < bind->dupls->nbItems; i++) {
		tab = bind->dupls->items[i];
		fprintf(output, "         ( ");
		for (j = 0; j < bind->definition->nbFields; j++) {
		    key = tab->keys[j];
		    if ((key != NULL) && (key->val != NULL)) {
			res = xmlSchemaGetCanonValue(key->val, &value);
			if (res >= 0)
			    fprintf(output, "'%s' ", value);
			else
			    fprintf(output, "CANON-VALUE-FAILED ");
			if (res == 0)
			    FREE_AND_NULL(value)
		    } else if (key != NULL)
		    fprintf(output, "(no val), ");
			else
			    fprintf(output, "(key missing), ");
		}
		fprintf(output, ")\n");
	    }
	}
	bind = bind->next;
    } while (bind != NULL);
}
#endif 
#endif 


static xmlAttrPtr
xmlSchemaGetPropNode(xmlNodePtr node, const char *name)
{
    xmlAttrPtr prop;

    if ((node == NULL) || (name == NULL))
	return(NULL);
    prop = node->properties;
    while (prop != NULL) {
        if ((prop->ns == NULL) && xmlStrEqual(prop->name, BAD_CAST name))
	    return(prop);
	prop = prop->next;
    }
    return (NULL);
}

static xmlAttrPtr
xmlSchemaGetPropNodeNs(xmlNodePtr node, const char *uri, const char *name)
{
    xmlAttrPtr prop;

    if ((node == NULL) || (name == NULL))
	return(NULL);
    prop = node->properties;
    while (prop != NULL) {
	if ((prop->ns != NULL) &&
	    xmlStrEqual(prop->name, BAD_CAST name) &&
	    xmlStrEqual(prop->ns->href, BAD_CAST uri))
	    return(prop);
	prop = prop->next;
    }
    return (NULL);
}

static const xmlChar *
xmlSchemaGetNodeContent(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node)
{
    xmlChar *val;
    const xmlChar *ret;

    val = xmlNodeGetContent(node);
    if (val == NULL)
	val = xmlStrdup((xmlChar *)"");
    ret = xmlDictLookup(ctxt->dict, val, -1);
    xmlFree(val);
    return(ret);
}

static const xmlChar *
xmlSchemaGetNodeContentNoDict(xmlNodePtr node)
{
    return((const xmlChar*) xmlNodeGetContent(node));
}

static const xmlChar *
xmlSchemaGetProp(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
                 const char *name)
{
    xmlChar *val;
    const xmlChar *ret;

    val = xmlGetNoNsProp(node, BAD_CAST name);
    if (val == NULL)
        return(NULL);
    ret = xmlDictLookup(ctxt->dict, val, -1);
    xmlFree(val);
    return(ret);
}


#define WXS_FIND_GLOBAL_ITEM(slot)			\
    if (xmlStrEqual(nsName, schema->targetNamespace)) { \
	ret = xmlHashLookup(schema->slot, name); \
	if (ret != NULL) goto exit; \
    } \
    if (xmlHashSize(schema->schemasImports) > 1) { \
	xmlSchemaImportPtr import; \
	if (nsName == NULL) \
	    import = xmlHashLookup(schema->schemasImports, \
		XML_SCHEMAS_NO_NAMESPACE); \
	else \
	    import = xmlHashLookup(schema->schemasImports, nsName); \
	if (import == NULL) \
	    goto exit; \
	ret = xmlHashLookup(import->schema->slot, name); \
    }

static xmlSchemaElementPtr
xmlSchemaGetElem(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * nsName)
{
    xmlSchemaElementPtr ret = NULL;

    if ((name == NULL) || (schema == NULL))
        return(NULL);
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(elemDecl)
    }
exit:
#ifdef DEBUG
    if (ret == NULL) {
        if (nsName == NULL)
            fprintf(stderr, "Unable to lookup element decl. %s", name);
        else
            fprintf(stderr, "Unable to lookup element decl. %s:%s", name,
                    nsName);
    }
#endif
    return (ret);
}

static xmlSchemaTypePtr
xmlSchemaGetType(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * nsName)
{
    xmlSchemaTypePtr ret = NULL;

    if (name == NULL)
        return (NULL);
    
    if ((nsName != NULL) && xmlStrEqual(nsName, xmlSchemaNs)) {
	ret = xmlSchemaGetPredefinedType(name, nsName);
	if (ret != NULL)
	    goto exit;
    }
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(typeDecl)
    }
exit:

#ifdef DEBUG
    if (ret == NULL) {
        if (nsName == NULL)
            fprintf(stderr, "Unable to lookup type %s", name);
        else
            fprintf(stderr, "Unable to lookup type %s:%s", name,
                    nsName);
    }
#endif
    return (ret);
}

static xmlSchemaAttributePtr
xmlSchemaGetAttributeDecl(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * nsName)
{
    xmlSchemaAttributePtr ret = NULL;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(attrDecl)
    }
exit:
#ifdef DEBUG
    if (ret == NULL) {
        if (nsName == NULL)
            fprintf(stderr, "Unable to lookup attribute %s", name);
        else
            fprintf(stderr, "Unable to lookup attribute %s:%s", name,
                    nsName);
    }
#endif
    return (ret);
}

static xmlSchemaAttributeGroupPtr
xmlSchemaGetAttributeGroup(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * nsName)
{
    xmlSchemaAttributeGroupPtr ret = NULL;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(attrgrpDecl)
    }
exit:
#ifdef DEBUG
    if (ret == NULL) {
        if (nsName == NULL)
            fprintf(stderr, "Unable to lookup attribute group %s", name);
        else
            fprintf(stderr, "Unable to lookup attribute group %s:%s", name,
                    nsName);
    }
#endif
    return (ret);
}

static xmlSchemaModelGroupDefPtr
xmlSchemaGetGroup(xmlSchemaPtr schema, const xmlChar * name,
                 const xmlChar * nsName)
{
    xmlSchemaModelGroupDefPtr ret = NULL;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(groupDecl)
    }
exit:

#ifdef DEBUG
    if (ret == NULL) {
        if (nsName == NULL)
            fprintf(stderr, "Unable to lookup group %s", name);
        else
            fprintf(stderr, "Unable to lookup group %s:%s", name,
                    nsName);
    }
#endif
    return (ret);
}

static xmlSchemaNotationPtr
xmlSchemaGetNotation(xmlSchemaPtr schema,
		     const xmlChar *name,
		     const xmlChar *nsName)
{
    xmlSchemaNotationPtr ret = NULL;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(notaDecl)
    }
exit:
    return (ret);
}

static xmlSchemaIDCPtr
xmlSchemaGetIDC(xmlSchemaPtr schema,
		const xmlChar *name,
		const xmlChar *nsName)
{
    xmlSchemaIDCPtr ret = NULL;

    if ((name == NULL) || (schema == NULL))
        return (NULL);
    if (schema != NULL) {
	WXS_FIND_GLOBAL_ITEM(idcDef)
    }
exit:
    return (ret);
}

static xmlSchemaBasicItemPtr
xmlSchemaGetNamedComponent(xmlSchemaPtr schema,
			   xmlSchemaTypeType itemType,
			   const xmlChar *name,
			   const xmlChar *targetNs)
{
    switch (itemType) {
	case XML_SCHEMA_TYPE_GROUP:
	    return ((xmlSchemaBasicItemPtr) xmlSchemaGetGroup(schema,
		name, targetNs));
	case XML_SCHEMA_TYPE_ELEMENT:
	    return ((xmlSchemaBasicItemPtr) xmlSchemaGetElem(schema,
		name, targetNs));
	default:
	    TODO
	    return (NULL);
    }
}


#define IS_BLANK_NODE(n)						\
    (((n)->type == XML_TEXT_NODE) && (xmlSchemaIsBlank((n)->content, -1)))

static int
xmlSchemaIsBlank(xmlChar * str, int len)
{
    if (str == NULL)
        return (1);
    if (len < 0) {
	while (*str != 0) {
	    if (!(IS_BLANK_CH(*str)))
		return (0);
	    str++;
	}
    } else while ((*str != 0) && (len != 0)) {
	if (!(IS_BLANK_CH(*str)))
	    return (0);
	str++;
	len--;
    }

    return (1);
}

#define WXS_COMP_NAME(c, t) ((t) (c))->name
#define WXS_COMP_TNS(c, t) ((t) (c))->targetNamespace
static xmlSchemaBasicItemPtr
xmlSchemaFindRedefCompInGraph(xmlSchemaBucketPtr bucket,
			      xmlSchemaTypeType type,
			      const xmlChar *name,
			      const xmlChar *nsName)
{
    xmlSchemaBasicItemPtr ret;
    int i;

    if ((bucket == NULL) || (name == NULL))
	return(NULL);
    if ((bucket->globals == NULL) ||
	(bucket->globals->nbItems == 0))
	goto subschemas;
    for (i = 0; i < bucket->globals->nbItems; i++) {
	ret = bucket->globals->items[i];
	if (ret->type == type) {
	    switch (type) {
		case XML_SCHEMA_TYPE_COMPLEX:
		case XML_SCHEMA_TYPE_SIMPLE:
		    if ((WXS_COMP_NAME(ret, xmlSchemaTypePtr) == name) &&
			(WXS_COMP_TNS(ret, xmlSchemaTypePtr) ==
			nsName))
		    {
			return(ret);
		    }
		    break;
		case XML_SCHEMA_TYPE_GROUP:
		    if ((WXS_COMP_NAME(ret,
			    xmlSchemaModelGroupDefPtr) == name) &&
			(WXS_COMP_TNS(ret,
			    xmlSchemaModelGroupDefPtr) == nsName))
		    {
			return(ret);
		    }
		    break;
		case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		    if ((WXS_COMP_NAME(ret,
			    xmlSchemaAttributeGroupPtr) == name) &&
			(WXS_COMP_TNS(ret,
			    xmlSchemaAttributeGroupPtr) == nsName))
		    {
			return(ret);
		    }
		    break;
		default:
		    
		    return(NULL);
	    }
	}
    }
subschemas:
    if (bucket->relations != NULL) {
	xmlSchemaSchemaRelationPtr rel = bucket->relations;

	bucket->flags |= XML_SCHEMA_BUCKET_MARKED;
	do {
	    if ((rel->bucket != NULL) &&
		((rel->bucket->flags & XML_SCHEMA_BUCKET_MARKED) == 0)) {
		ret = xmlSchemaFindRedefCompInGraph(rel->bucket,
		    type, name, nsName);
		if (ret != NULL)
		    return(ret);
	    }
	    rel = rel->next;
	} while (rel != NULL);
	 bucket->flags ^= XML_SCHEMA_BUCKET_MARKED;
    }
    return(NULL);
}

static xmlSchemaNotationPtr
xmlSchemaAddNotation(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                     const xmlChar *name, const xmlChar *nsName,
		     xmlNodePtr node ATTRIBUTE_UNUSED)
{
    xmlSchemaNotationPtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

    ret = (xmlSchemaNotationPtr) xmlMalloc(sizeof(xmlSchemaNotation));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "add annotation", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaNotation));
    ret->type = XML_SCHEMA_TYPE_NOTATION;
    ret->name = name;
    ret->targetNamespace = nsName;
    WXS_ADD_GLOBAL(ctxt, ret);
    return (ret);
}

static xmlSchemaAttributePtr
xmlSchemaAddAttribute(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                      const xmlChar * name, const xmlChar * nsName,
		      xmlNodePtr node, int topLevel)
{
    xmlSchemaAttributePtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

    ret = (xmlSchemaAttributePtr) xmlMalloc(sizeof(xmlSchemaAttribute));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating attribute", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAttribute));
    ret->type = XML_SCHEMA_TYPE_ATTRIBUTE;
    ret->node = node;
    ret->name = name;
    ret->targetNamespace = nsName;

    if (topLevel)
	WXS_ADD_GLOBAL(ctxt, ret);
    else
	WXS_ADD_LOCAL(ctxt, ret);
    WXS_ADD_PENDING(ctxt, ret);
    return (ret);
}

static xmlSchemaAttributeUsePtr
xmlSchemaAddAttributeUse(xmlSchemaParserCtxtPtr pctxt,
			 xmlNodePtr node)
{
    xmlSchemaAttributeUsePtr ret = NULL;

    if (pctxt == NULL)
        return (NULL);

    ret = (xmlSchemaAttributeUsePtr) xmlMalloc(sizeof(xmlSchemaAttributeUse));
    if (ret == NULL) {
        xmlSchemaPErrMemory(pctxt, "allocating attribute", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAttributeUse));
    ret->type = XML_SCHEMA_TYPE_ATTRIBUTE_USE;
    ret->node = node;

    WXS_ADD_LOCAL(pctxt, ret);
    return (ret);
}

static xmlSchemaRedefPtr
xmlSchemaAddRedef(xmlSchemaParserCtxtPtr pctxt,
		  xmlSchemaBucketPtr targetBucket,
		  void *item,
		  const xmlChar *refName,
		  const xmlChar *refTargetNs)
{
    xmlSchemaRedefPtr ret;

    ret = (xmlSchemaRedefPtr)
	xmlMalloc(sizeof(xmlSchemaRedef));
    if (ret == NULL) {
	xmlSchemaPErrMemory(pctxt,
	    "allocating redefinition info", NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaRedef));
    ret->item = item;
    ret->targetBucket = targetBucket;
    ret->refName = refName;
    ret->refTargetNs = refTargetNs;
    if (WXS_CONSTRUCTOR(pctxt)->redefs == NULL)
	WXS_CONSTRUCTOR(pctxt)->redefs = ret;
    else
	WXS_CONSTRUCTOR(pctxt)->lastRedef->next = ret;
    WXS_CONSTRUCTOR(pctxt)->lastRedef = ret;

    return (ret);
}

static xmlSchemaAttributeGroupPtr
xmlSchemaAddAttributeGroupDefinition(xmlSchemaParserCtxtPtr pctxt,
                           xmlSchemaPtr schema ATTRIBUTE_UNUSED,
			   const xmlChar *name,
			   const xmlChar *nsName,
			   xmlNodePtr node)
{
    xmlSchemaAttributeGroupPtr ret = NULL;

    if ((pctxt == NULL) || (name == NULL))
        return (NULL);

    ret = (xmlSchemaAttributeGroupPtr)
        xmlMalloc(sizeof(xmlSchemaAttributeGroup));
    if (ret == NULL) {
	xmlSchemaPErrMemory(pctxt, "allocating attribute group", NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAttributeGroup));
    ret->type = XML_SCHEMA_TYPE_ATTRIBUTEGROUP;
    ret->name = name;
    ret->targetNamespace = nsName;
    ret->node = node;

    
    ret->flags |= XML_SCHEMAS_ATTRGROUP_GLOBAL;
    if (pctxt->isRedefine) {
	pctxt->redef = xmlSchemaAddRedef(pctxt, pctxt->redefined,
	    ret, name, nsName);
	if (pctxt->redef == NULL) {
	    xmlFree(ret);
	    return(NULL);
	}
	pctxt->redefCounter = 0;
    }
    WXS_ADD_GLOBAL(pctxt, ret);
    WXS_ADD_PENDING(pctxt, ret);
    return (ret);
}

static xmlSchemaElementPtr
xmlSchemaAddElement(xmlSchemaParserCtxtPtr ctxt,
                    const xmlChar * name, const xmlChar * nsName,
		    xmlNodePtr node, int topLevel)
{
    xmlSchemaElementPtr ret = NULL;

    if ((ctxt == NULL) || (name == NULL))
        return (NULL);

    ret = (xmlSchemaElementPtr) xmlMalloc(sizeof(xmlSchemaElement));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating element", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaElement));
    ret->type = XML_SCHEMA_TYPE_ELEMENT;
    ret->name = name;
    ret->targetNamespace = nsName;
    ret->node = node;

    if (topLevel)
	WXS_ADD_GLOBAL(ctxt, ret);
    else
	WXS_ADD_LOCAL(ctxt, ret);
    WXS_ADD_PENDING(ctxt, ret);
    return (ret);
}

static xmlSchemaTypePtr
xmlSchemaAddType(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
		 xmlSchemaTypeType type,
                 const xmlChar * name, const xmlChar * nsName,
		 xmlNodePtr node, int topLevel)
{
    xmlSchemaTypePtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

    ret = (xmlSchemaTypePtr) xmlMalloc(sizeof(xmlSchemaType));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating type", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaType));
    ret->type = type;
    ret->name = name;
    ret->targetNamespace = nsName;
    ret->node = node;
    if (topLevel) {
	if (ctxt->isRedefine) {
	    ctxt->redef = xmlSchemaAddRedef(ctxt, ctxt->redefined,
		ret, name, nsName);
	    if (ctxt->redef == NULL) {
		xmlFree(ret);
		return(NULL);
	    }
	    ctxt->redefCounter = 0;
	}
	WXS_ADD_GLOBAL(ctxt, ret);
    } else
	WXS_ADD_LOCAL(ctxt, ret);
    WXS_ADD_PENDING(ctxt, ret);
    return (ret);
}

static xmlSchemaQNameRefPtr
xmlSchemaNewQNameRef(xmlSchemaParserCtxtPtr pctxt,
		     xmlSchemaTypeType refType,
		     const xmlChar *refName,
		     const xmlChar *refNs)
{
    xmlSchemaQNameRefPtr ret;

    ret = (xmlSchemaQNameRefPtr)
	xmlMalloc(sizeof(xmlSchemaQNameRef));
    if (ret == NULL) {
	xmlSchemaPErrMemory(pctxt,
	    "allocating QName reference item", NULL);
	return (NULL);
    }
    ret->node = NULL;
    ret->type = XML_SCHEMA_EXTRA_QNAMEREF;
    ret->name = refName;
    ret->targetNamespace = refNs;
    ret->item = NULL;
    ret->itemType = refType;
    WXS_ADD_LOCAL(pctxt, ret);
    return (ret);
}

static xmlSchemaAttributeUseProhibPtr
xmlSchemaAddAttributeUseProhib(xmlSchemaParserCtxtPtr pctxt)
{
    xmlSchemaAttributeUseProhibPtr ret;

    ret = (xmlSchemaAttributeUseProhibPtr)
	xmlMalloc(sizeof(xmlSchemaAttributeUseProhib));
    if (ret == NULL) {
	xmlSchemaPErrMemory(pctxt,
	    "allocating attribute use prohibition", NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaAttributeUseProhib));
    ret->type = XML_SCHEMA_EXTRA_ATTR_USE_PROHIB;
    WXS_ADD_LOCAL(pctxt, ret);
    return (ret);
}


static xmlSchemaModelGroupPtr
xmlSchemaAddModelGroup(xmlSchemaParserCtxtPtr ctxt,
		       xmlSchemaPtr schema,
		       xmlSchemaTypeType type,
		       xmlNodePtr node)
{
    xmlSchemaModelGroupPtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

    ret = (xmlSchemaModelGroupPtr)
	xmlMalloc(sizeof(xmlSchemaModelGroup));
    if (ret == NULL) {
	xmlSchemaPErrMemory(ctxt, "allocating model group component",
	    NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaModelGroup));
    ret->type = type;
    ret->node = node;
    WXS_ADD_LOCAL(ctxt, ret);
    if ((type == XML_SCHEMA_TYPE_SEQUENCE) ||
	(type == XML_SCHEMA_TYPE_CHOICE))
	WXS_ADD_PENDING(ctxt, ret);
    return (ret);
}


static xmlSchemaParticlePtr
xmlSchemaAddParticle(xmlSchemaParserCtxtPtr ctxt,
		     xmlNodePtr node, int min, int max)
{
    xmlSchemaParticlePtr ret = NULL;
    if (ctxt == NULL)
        return (NULL);

#ifdef DEBUG
    fprintf(stderr, "Adding particle component\n");
#endif
    ret = (xmlSchemaParticlePtr)
	xmlMalloc(sizeof(xmlSchemaParticle));
    if (ret == NULL) {
	xmlSchemaPErrMemory(ctxt, "allocating particle component",
	    NULL);
	return (NULL);
    }
    ret->type = XML_SCHEMA_TYPE_PARTICLE;
    ret->annot = NULL;
    ret->node = node;
    ret->minOccurs = min;
    ret->maxOccurs = max;
    ret->next = NULL;
    ret->children = NULL;

    WXS_ADD_LOCAL(ctxt, ret);
    return (ret);
}

static xmlSchemaModelGroupDefPtr
xmlSchemaAddModelGroupDefinition(xmlSchemaParserCtxtPtr ctxt,
				 xmlSchemaPtr schema,
				 const xmlChar *name,
				 const xmlChar *nsName,
				 xmlNodePtr node)
{
    xmlSchemaModelGroupDefPtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

    ret = (xmlSchemaModelGroupDefPtr)
	xmlMalloc(sizeof(xmlSchemaModelGroupDef));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "adding group", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaModelGroupDef));
    ret->name = name;
    ret->type = XML_SCHEMA_TYPE_GROUP;
    ret->node = node;
    ret->targetNamespace = nsName;

    if (ctxt->isRedefine) {
	ctxt->redef = xmlSchemaAddRedef(ctxt, ctxt->redefined,
	    ret, name, nsName);
	if (ctxt->redef == NULL) {
	    xmlFree(ret);
	    return(NULL);
	}
	ctxt->redefCounter = 0;
    }
    WXS_ADD_GLOBAL(ctxt, ret);
    WXS_ADD_PENDING(ctxt, ret);
    return (ret);
}

static xmlSchemaWildcardNsPtr
xmlSchemaNewWildcardNsConstraint(xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaWildcardNsPtr ret;

    ret = (xmlSchemaWildcardNsPtr)
	xmlMalloc(sizeof(xmlSchemaWildcardNs));
    if (ret == NULL) {
	xmlSchemaPErrMemory(ctxt, "creating wildcard namespace constraint", NULL);
	return (NULL);
    }
    ret->value = NULL;
    ret->next = NULL;
    return (ret);
}

static xmlSchemaIDCPtr
xmlSchemaAddIDC(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                  const xmlChar *name, const xmlChar *nsName,
		  int category, xmlNodePtr node)
{
    xmlSchemaIDCPtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (name == NULL))
        return (NULL);

    ret = (xmlSchemaIDCPtr) xmlMalloc(sizeof(xmlSchemaIDC));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt,
	    "allocating an identity-constraint definition", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaIDC));
    
    ret->targetNamespace = nsName;
    ret->name = name;
    ret->type = category;
    ret->node = node;

    WXS_ADD_GLOBAL(ctxt, ret);
    if (category == XML_SCHEMA_TYPE_IDC_KEYREF)
	WXS_ADD_PENDING(ctxt, ret);
    return (ret);
}

static xmlSchemaWildcardPtr
xmlSchemaAddWildcard(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
		     xmlSchemaTypeType type, xmlNodePtr node)
{
    xmlSchemaWildcardPtr ret = NULL;

    if ((ctxt == NULL) || (schema == NULL))
        return (NULL);

    ret = (xmlSchemaWildcardPtr) xmlMalloc(sizeof(xmlSchemaWildcard));
    if (ret == NULL) {
        xmlSchemaPErrMemory(ctxt, "adding wildcard", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaWildcard));
    ret->type = type;
    ret->node = node;
    WXS_ADD_LOCAL(ctxt, ret);
    return (ret);
}

static void
xmlSchemaSubstGroupFree(xmlSchemaSubstGroupPtr group)
{
    if (group == NULL)
	return;
    if (group->members != NULL)
	xmlSchemaItemListFree(group->members);
    xmlFree(group);
}

static xmlSchemaSubstGroupPtr
xmlSchemaSubstGroupAdd(xmlSchemaParserCtxtPtr pctxt,
		       xmlSchemaElementPtr head)
{
    xmlSchemaSubstGroupPtr ret;

    
    if (WXS_SUBST_GROUPS(pctxt) == NULL) {
	WXS_SUBST_GROUPS(pctxt) = xmlHashCreateDict(10, pctxt->dict);
	if (WXS_SUBST_GROUPS(pctxt) == NULL)
	    return(NULL);
    }
    
    ret = (xmlSchemaSubstGroupPtr) xmlMalloc(sizeof(xmlSchemaSubstGroup));
    if (ret == NULL) {
	xmlSchemaPErrMemory(NULL,
	    "allocating a substitution group container", NULL);
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaSubstGroup));
    ret->head = head;
    
    ret->members = xmlSchemaItemListCreate();
    if (ret->members == NULL) {
	xmlSchemaSubstGroupFree(ret);
	return(NULL);
    }
    
    if (xmlHashAddEntry2(WXS_SUBST_GROUPS(pctxt),
	head->name, head->targetNamespace, ret) != 0) {
	PERROR_INT("xmlSchemaSubstGroupAdd",
	    "failed to add a new substitution container");
	xmlSchemaSubstGroupFree(ret);
	return(NULL);
    }
    return(ret);
}

static xmlSchemaSubstGroupPtr
xmlSchemaSubstGroupGet(xmlSchemaParserCtxtPtr pctxt,
		       xmlSchemaElementPtr head)
{
    if (WXS_SUBST_GROUPS(pctxt) == NULL)
	return(NULL);
    return(xmlHashLookup2(WXS_SUBST_GROUPS(pctxt),
	head->name, head->targetNamespace));

}

static int
xmlSchemaAddElementSubstitutionMember(xmlSchemaParserCtxtPtr pctxt,
				      xmlSchemaElementPtr head,
				      xmlSchemaElementPtr member)
{
    xmlSchemaSubstGroupPtr substGroup = NULL;

    if ((pctxt == NULL) || (head == NULL) || (member == NULL))
	return (-1);

    substGroup = xmlSchemaSubstGroupGet(pctxt, head);
    if (substGroup == NULL)
	substGroup = xmlSchemaSubstGroupAdd(pctxt, head);
    if (substGroup == NULL)
	return(-1);
    if (xmlSchemaItemListAdd(substGroup->members, member) == -1)
	return(-1);
    return(0);
}


static int
xmlSchemaPValAttrNodeQNameValue(xmlSchemaParserCtxtPtr ctxt,
				       xmlSchemaPtr schema,
				       xmlSchemaBasicItemPtr ownerItem,
				       xmlAttrPtr attr,
				       const xmlChar *value,
				       const xmlChar **uri,
				       const xmlChar **local)
{
    const xmlChar *pref;
    xmlNsPtr ns;
    int len, ret;

    *uri = NULL;
    *local = NULL;
    ret = xmlValidateQName(value, 1);
    if (ret > 0) {
	xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    ownerItem, (xmlNodePtr) attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME),
	    NULL, value, NULL, NULL, NULL);
	*local = value;
	return (ctxt->err);
    } else if (ret < 0)
	return (-1);

    if (!strchr((char *) value, ':')) {
	ns = xmlSearchNs(attr->doc, attr->parent, NULL);
	if (ns)
	    *uri = xmlDictLookup(ctxt->dict, ns->href, -1);
	else if (schema->flags & XML_SCHEMAS_INCLUDING_CONVERT_NS) {
	    *uri = ctxt->targetNamespace;
	}
	*local = xmlDictLookup(ctxt->dict, value, -1);
	return (0);
    }
    *local = xmlSplitQName3(value, &len);
    *local = xmlDictLookup(ctxt->dict, *local, -1);
    pref = xmlDictLookup(ctxt->dict, value, len);
    ns = xmlSearchNs(attr->doc, attr->parent, pref);
    if (ns == NULL) {
	xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    ownerItem, (xmlNodePtr) attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME), NULL, value,
	    "The value '%s' of simple type 'xs:QName' has no "
	    "corresponding namespace declaration in scope", value, NULL);
	return (ctxt->err);
    } else {
        *uri = xmlDictLookup(ctxt->dict, ns->href, -1);
    }
    return (0);
}

static int
xmlSchemaPValAttrNodeQName(xmlSchemaParserCtxtPtr ctxt,
				       xmlSchemaPtr schema,
				       xmlSchemaBasicItemPtr ownerItem,
				       xmlAttrPtr attr,
				       const xmlChar **uri,
				       const xmlChar **local)
{
    const xmlChar *value;

    value = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    return (xmlSchemaPValAttrNodeQNameValue(ctxt, schema,
	ownerItem, attr, value, uri, local));
}

static int
xmlSchemaPValAttrQName(xmlSchemaParserCtxtPtr ctxt,
				   xmlSchemaPtr schema,
				   xmlSchemaBasicItemPtr ownerItem,
				   xmlNodePtr ownerElem,
				   const char *name,
				   const xmlChar **uri,
				   const xmlChar **local)
{
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(ownerElem, name);
    if (attr == NULL) {
	*local = NULL;
	*uri = NULL;
	return (0);
    }
    return (xmlSchemaPValAttrNodeQName(ctxt, schema,
	ownerItem, attr, uri, local));
}

static int
xmlSchemaPValAttrNodeID(xmlSchemaParserCtxtPtr ctxt, xmlAttrPtr attr)
{
    int ret;
    const xmlChar *value;

    if (attr == NULL)
	return(0);
    value = xmlSchemaGetNodeContentNoDict((xmlNodePtr) attr);
    ret = xmlValidateNCName(value, 1);
    if (ret == 0) {
	if (attr->atype != XML_ATTRIBUTE_ID) {
	    xmlIDPtr res;
	    xmlChar *strip;

	    strip = xmlSchemaCollapseString(value);
	    if (strip != NULL) {
		xmlFree((xmlChar *) value);
		value = strip;
	    }
    	    res = xmlAddID(NULL, attr->doc, value, attr);
	    if (res == NULL) {
		ret = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
		xmlSchemaPSimpleTypeErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		    NULL, (xmlNodePtr) attr,
		    xmlSchemaGetBuiltInType(XML_SCHEMAS_ID),
		    NULL, NULL, "Duplicate value '%s' of simple "
		    "type 'xs:ID'", value, NULL);
	    } else
		attr->atype = XML_ATTRIBUTE_ID;
	}
    } else if (ret > 0) {
	ret = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
	xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    NULL, (xmlNodePtr) attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ID),
	    NULL, NULL, "The value '%s' of simple type 'xs:ID' is "
	    "not a valid 'xs:NCName'",
	    value, NULL);
    }
    if (value != NULL)
	xmlFree((xmlChar *)value);

    return (ret);
}

static int
xmlSchemaPValAttrID(xmlSchemaParserCtxtPtr ctxt,
		    xmlNodePtr ownerElem,
		    const xmlChar *name)
{
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(ownerElem, (const char *) name);
    if (attr == NULL)
	return(0);
    return(xmlSchemaPValAttrNodeID(ctxt, attr));

}

static int
xmlGetMaxOccurs(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
		int min, int max, int def, const char *expected)
{
    const xmlChar *val, *cur;
    int ret = 0;
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(node, "maxOccurs");
    if (attr == NULL)
	return (def);
    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);

    if (xmlStrEqual(val, (const xmlChar *) "unbounded")) {
	if (max != UNBOUNDED) {
	    xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		
		NULL, (xmlNodePtr) attr, NULL, expected,
		val, NULL, NULL, NULL);
	    return (def);
	} else
	    return (UNBOUNDED);  
    }

    cur = val;
    while (IS_BLANK_CH(*cur))
        cur++;
    if (*cur == 0) {
        xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    
	    NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
	return (def);
    }
    while ((*cur >= '0') && (*cur <= '9')) {
        ret = ret * 10 + (*cur - '0');
        cur++;
    }
    while (IS_BLANK_CH(*cur))
        cur++;
    if ((*cur != 0) || (ret < min) || ((max != -1) && (ret > max))) {
	xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    
	    NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
        return (def);
    }
    return (ret);
}

static int
xmlGetMinOccurs(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node,
		int min, int max, int def, const char *expected)
{
    const xmlChar *val, *cur;
    int ret = 0;
    xmlAttrPtr attr;

    attr = xmlSchemaGetPropNode(node, "minOccurs");
    if (attr == NULL)
	return (def);
    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    cur = val;
    while (IS_BLANK_CH(*cur))
        cur++;
    if (*cur == 0) {
        xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    
	    NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
        return (def);
    }
    while ((*cur >= '0') && (*cur <= '9')) {
        ret = ret * 10 + (*cur - '0');
        cur++;
    }
    while (IS_BLANK_CH(*cur))
        cur++;
    if ((*cur != 0) || (ret < min) || ((max != -1) && (ret > max))) {
	xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    
	    NULL, (xmlNodePtr) attr, NULL, expected,
	    val, NULL, NULL, NULL);
        return (def);
    }
    return (ret);
}

static int
xmlSchemaPGetBoolNodeValue(xmlSchemaParserCtxtPtr ctxt,
			   xmlSchemaBasicItemPtr ownerItem,
			   xmlNodePtr node)
{
    xmlChar *value = NULL;
    int res = 0;

    value = xmlNodeGetContent(node);
    if (xmlStrEqual(BAD_CAST value, BAD_CAST "true"))
        res = 1;
    else if (xmlStrEqual(BAD_CAST value, BAD_CAST "false"))
        res = 0;
    else if (xmlStrEqual(BAD_CAST value, BAD_CAST "1"))
	res = 1;
    else if (xmlStrEqual(BAD_CAST value, BAD_CAST "0"))
        res = 0;
    else {
        xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_INVALID_BOOLEAN,
	    ownerItem, node,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_BOOLEAN),
	    NULL, BAD_CAST value,
	    NULL, NULL, NULL);
    }
    if (value != NULL)
	xmlFree(value);
    return (res);
}

static int
xmlGetBooleanProp(xmlSchemaParserCtxtPtr ctxt,
		  xmlNodePtr node,
                  const char *name, int def)
{
    const xmlChar *val;

    val = xmlSchemaGetProp(ctxt, node, name);
    if (val == NULL)
        return (def);
    if (xmlStrEqual(val, BAD_CAST "true"))
        def = 1;
    else if (xmlStrEqual(val, BAD_CAST "false"))
        def = 0;
    else if (xmlStrEqual(val, BAD_CAST "1"))
	def = 1;
    else if (xmlStrEqual(val, BAD_CAST "0"))
        def = 0;
    else {
        xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_INVALID_BOOLEAN,
	    NULL,
	    (xmlNodePtr) xmlSchemaGetPropNode(node, name),
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_BOOLEAN),
	    NULL, val, NULL, NULL, NULL);
    }
    return (def);
}

static xmlSchemaTypePtr xmlSchemaParseSimpleType(xmlSchemaParserCtxtPtr
                                                 ctxt, xmlSchemaPtr schema,
                                                 xmlNodePtr node,
						 int topLevel);
static xmlSchemaTypePtr xmlSchemaParseComplexType(xmlSchemaParserCtxtPtr
                                                  ctxt,
                                                  xmlSchemaPtr schema,
                                                  xmlNodePtr node,
						  int topLevel);
static xmlSchemaTypePtr xmlSchemaParseRestriction(xmlSchemaParserCtxtPtr
                                                  ctxt,
                                                  xmlSchemaPtr schema,
                                                  xmlNodePtr node,
						  xmlSchemaTypeType parentType);
static xmlSchemaBasicItemPtr
xmlSchemaParseLocalAttribute(xmlSchemaParserCtxtPtr pctxt,
			     xmlSchemaPtr schema,
			     xmlNodePtr node,
			     xmlSchemaItemListPtr uses,
			     int parentType);
static xmlSchemaTypePtr xmlSchemaParseList(xmlSchemaParserCtxtPtr ctxt,
                                           xmlSchemaPtr schema,
                                           xmlNodePtr node);
static xmlSchemaWildcardPtr
xmlSchemaParseAnyAttribute(xmlSchemaParserCtxtPtr ctxt,
                           xmlSchemaPtr schema, xmlNodePtr node);

static int
xmlSchemaPValAttrNodeValue(xmlSchemaParserCtxtPtr pctxt,
			   xmlSchemaBasicItemPtr ownerItem,
			   xmlAttrPtr attr,
			   const xmlChar *value,
			   xmlSchemaTypePtr type)
{

    int ret = 0;

    if ((pctxt == NULL) || (type == NULL) || (attr == NULL))
	return (-1);
    if (type->type != XML_SCHEMA_TYPE_BASIC) {
	PERROR_INT("xmlSchemaPValAttrNodeValue",
	    "the given type is not a built-in type");
	return (-1);
    }
    switch (type->builtInType) {
	case XML_SCHEMAS_NCNAME:
	case XML_SCHEMAS_QNAME:
	case XML_SCHEMAS_ANYURI:
	case XML_SCHEMAS_TOKEN:
	case XML_SCHEMAS_LANGUAGE:
	    ret = xmlSchemaValPredefTypeNode(type, value, NULL,
		(xmlNodePtr) attr);
	    break;
	default: {
	    PERROR_INT("xmlSchemaPValAttrNodeValue",
		"validation using the given type is not supported while "
		"parsing a schema");
	    return (-1);
	}
    }
    if (ret < 0) {
	PERROR_INT("xmlSchemaPValAttrNodeValue",
	    "failed to validate a schema attribute value");
	return (-1);
    } else if (ret > 0) {
	if (WXS_IS_LIST(type))
	    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
	else
	    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
	xmlSchemaPSimpleTypeErr(pctxt,
	    ret, ownerItem, (xmlNodePtr) attr,
	    type, NULL, value, NULL, NULL, NULL);
    }
    return (ret);
}

static int
xmlSchemaPValAttrNode(xmlSchemaParserCtxtPtr ctxt,
			   xmlSchemaBasicItemPtr ownerItem,
			   xmlAttrPtr attr,
			   xmlSchemaTypePtr type,
			   const xmlChar **value)
{
    const xmlChar *val;

    if ((ctxt == NULL) || (type == NULL) || (attr == NULL))
	return (-1);

    val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    if (value != NULL)
	*value = val;

    return (xmlSchemaPValAttrNodeValue(ctxt, ownerItem, attr,
	val, type));
}

static int
xmlSchemaPValAttr(xmlSchemaParserCtxtPtr ctxt,
		       xmlSchemaBasicItemPtr ownerItem,
		       xmlNodePtr ownerElem,
		       const char *name,
		       xmlSchemaTypePtr type,
		       const xmlChar **value)
{
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (type == NULL)) {
	if (value != NULL)
	    *value = NULL;
	return (-1);
    }
    if (type->type != XML_SCHEMA_TYPE_BASIC) {
	if (value != NULL)
	    *value = NULL;
	xmlSchemaPErr(ctxt, ownerElem,
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaPValAttr, the given "
	    "type '%s' is not a built-in type.\n",
	    type->name, NULL);
	return (-1);
    }
    attr = xmlSchemaGetPropNode(ownerElem, name);
    if (attr == NULL) {
	if (value != NULL)
	    *value = NULL;
	return (0);
    }
    return (xmlSchemaPValAttrNode(ctxt, ownerItem, attr,
	type, value));
}

static int
xmlSchemaCheckReference(xmlSchemaParserCtxtPtr pctxt,
		  xmlSchemaPtr schema ATTRIBUTE_UNUSED,
		  xmlNodePtr node,
		  xmlAttrPtr attr,
		  const xmlChar *namespaceName)
{
    
    if (xmlStrEqual(pctxt->targetNamespace, namespaceName))
	return (0);
    if (xmlStrEqual(xmlSchemaNs, namespaceName))
	return (0);
    if (WXS_BUCKET(pctxt)->relations != NULL) {
	xmlSchemaSchemaRelationPtr rel;

	rel = WXS_BUCKET(pctxt)->relations;
	do {
	    if (WXS_IS_BUCKET_IMPMAIN(rel->type) &&
		xmlStrEqual(namespaceName, rel->importNamespace))
		return (0);
	    rel = rel->next;
	} while (rel != NULL);
    }
    {
	xmlNodePtr n = (attr != NULL) ? (xmlNodePtr) attr : node;

	if (namespaceName == NULL)
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_SRC_RESOLVE, n, NULL,
		"References from this schema to components in no "
		"namespace are not allowed, since not indicated by an "
		"import statement", NULL, NULL);
	else
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_SRC_RESOLVE, n, NULL,
		"References from this schema to components in the "
		"namespace '%s' are not allowed, since not indicated by an "
		"import statement", namespaceName, NULL);
    }
    return (XML_SCHEMAP_SRC_RESOLVE);
}

static int
xmlSchemaParseLocalAttributes(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                        xmlNodePtr *child, xmlSchemaItemListPtr *list,
			int parentType, int *hasRefs)
{
    void *item;

    while ((IS_SCHEMA((*child), "attribute")) ||
           (IS_SCHEMA((*child), "attributeGroup"))) {
        if (IS_SCHEMA((*child), "attribute")) {
	    item = xmlSchemaParseLocalAttribute(ctxt, schema, *child,
		*list, parentType);
        } else {
            item = xmlSchemaParseAttributeGroupRef(ctxt, schema, *child);
	    if ((item != NULL) && (hasRefs != NULL))
		*hasRefs = 1;
        }
	if (item != NULL) {
	    if (*list == NULL) {
		
		*list = xmlSchemaItemListCreate();
		if (*list == NULL)
		    return(-1);
	    }
	    if (xmlSchemaItemListAddSize(*list, 2, item) == -1)
		return(-1);
	}
        *child = (*child)->next;
    }
    return (0);
}

static xmlSchemaAnnotPtr
xmlSchemaParseAnnotation(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr node, int needed)
{
    xmlSchemaAnnotPtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    int barked = 0;

    if ((ctxt == NULL) || (node == NULL))
        return (NULL);
    if (needed)
	ret = xmlSchemaNewAnnot(ctxt, node);
    else
	ret = NULL;
    attr = node->properties;
    while (attr != NULL) {
	if (((attr->ns == NULL) &&
	    (!xmlStrEqual(attr->name, BAD_CAST "id"))) ||
	    ((attr->ns != NULL) &&
	    xmlStrEqual(attr->ns->href, xmlSchemaNs))) {

	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    child = node->children;
    while (child != NULL) {
	if (IS_SCHEMA(child, "appinfo")) {
	    
	    attr = child->properties;
	    while (attr != NULL) {
		if (((attr->ns == NULL) &&
		     (!xmlStrEqual(attr->name, BAD_CAST "source"))) ||
		     ((attr->ns != NULL) &&
		      xmlStrEqual(attr->ns->href, xmlSchemaNs))) {

		    xmlSchemaPIllegalAttrErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		}
		attr = attr->next;
	    }
	    xmlSchemaPValAttr(ctxt, NULL, child, "source",
		xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), NULL);
	    child = child->next;
	} else if (IS_SCHEMA(child, "documentation")) {
	    
	    attr = child->properties;
	    while (attr != NULL) {
		if (attr->ns == NULL) {
		    if (!xmlStrEqual(attr->name, BAD_CAST "source")) {
			xmlSchemaPIllegalAttrErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		    }
		} else {
		    if (xmlStrEqual(attr->ns->href, xmlSchemaNs) ||
			(xmlStrEqual(attr->name, BAD_CAST "lang") &&
			(!xmlStrEqual(attr->ns->href, XML_XML_NAMESPACE)))) {

			xmlSchemaPIllegalAttrErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		    }
		}
		attr = attr->next;
	    }
	    attr = xmlSchemaGetPropNodeNs(child, (const char *) XML_XML_NAMESPACE, "lang");
	    if (attr != NULL)
		xmlSchemaPValAttrNode(ctxt, NULL, attr,
		xmlSchemaGetBuiltInType(XML_SCHEMAS_LANGUAGE), NULL);
	    child = child->next;
	} else {
	    if (!barked)
		xmlSchemaPContentErr(ctxt,
		    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		    NULL, node, child, NULL, "(appinfo | documentation)*");
	    barked = 1;
	    child = child->next;
	}
    }

    return (ret);
}

static xmlSchemaFacetPtr
xmlSchemaParseFacet(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                    xmlNodePtr node)
{
    xmlSchemaFacetPtr facet;
    xmlNodePtr child = NULL;
    const xmlChar *value;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    facet = xmlSchemaNewFacet();
    if (facet == NULL) {
        xmlSchemaPErrMemory(ctxt, "allocating facet", node);
        return (NULL);
    }
    facet->node = node;
    value = xmlSchemaGetProp(ctxt, node, "value");
    if (value == NULL) {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_FACET_NO_VALUE,
                       "Facet %s has no value\n", node->name, NULL);
        xmlSchemaFreeFacet(facet);
        return (NULL);
    }
    if (IS_SCHEMA(node, "minInclusive")) {
        facet->type = XML_SCHEMA_FACET_MININCLUSIVE;
    } else if (IS_SCHEMA(node, "minExclusive")) {
        facet->type = XML_SCHEMA_FACET_MINEXCLUSIVE;
    } else if (IS_SCHEMA(node, "maxInclusive")) {
        facet->type = XML_SCHEMA_FACET_MAXINCLUSIVE;
    } else if (IS_SCHEMA(node, "maxExclusive")) {
        facet->type = XML_SCHEMA_FACET_MAXEXCLUSIVE;
    } else if (IS_SCHEMA(node, "totalDigits")) {
        facet->type = XML_SCHEMA_FACET_TOTALDIGITS;
    } else if (IS_SCHEMA(node, "fractionDigits")) {
        facet->type = XML_SCHEMA_FACET_FRACTIONDIGITS;
    } else if (IS_SCHEMA(node, "pattern")) {
        facet->type = XML_SCHEMA_FACET_PATTERN;
    } else if (IS_SCHEMA(node, "enumeration")) {
        facet->type = XML_SCHEMA_FACET_ENUMERATION;
    } else if (IS_SCHEMA(node, "whiteSpace")) {
        facet->type = XML_SCHEMA_FACET_WHITESPACE;
    } else if (IS_SCHEMA(node, "length")) {
        facet->type = XML_SCHEMA_FACET_LENGTH;
    } else if (IS_SCHEMA(node, "maxLength")) {
        facet->type = XML_SCHEMA_FACET_MAXLENGTH;
    } else if (IS_SCHEMA(node, "minLength")) {
        facet->type = XML_SCHEMA_FACET_MINLENGTH;
    } else {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_UNKNOWN_FACET_TYPE,
                       "Unknown facet type %s\n", node->name, NULL);
        xmlSchemaFreeFacet(facet);
        return (NULL);
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    facet->value = value;
    if ((facet->type != XML_SCHEMA_FACET_PATTERN) &&
	(facet->type != XML_SCHEMA_FACET_ENUMERATION)) {
	const xmlChar *fixed;

	fixed = xmlSchemaGetProp(ctxt, node, "fixed");
	if (fixed != NULL) {
	    if (xmlStrEqual(fixed, BAD_CAST "true"))
		facet->fixed = 1;
	}
    }
    child = node->children;

    if (IS_SCHEMA(child, "annotation")) {
        facet->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    if (child != NULL) {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_UNKNOWN_FACET_CHILD,
                       "Facet %s has unexpected child content\n",
                       node->name, NULL);
    }
    return (facet);
}

static int
xmlSchemaParseWildcardNs(xmlSchemaParserCtxtPtr ctxt,
			 xmlSchemaPtr schema ATTRIBUTE_UNUSED,
			 xmlSchemaWildcardPtr wildc,
			 xmlNodePtr node)
{
    const xmlChar *pc, *ns, *dictnsItem;
    int ret = 0;
    xmlChar *nsItem;
    xmlSchemaWildcardNsPtr tmp, lastNs = NULL;
    xmlAttrPtr attr;

    pc = xmlSchemaGetProp(ctxt, node, "processContents");
    if ((pc == NULL)
        || (xmlStrEqual(pc, (const xmlChar *) "strict"))) {
        wildc->processContents = XML_SCHEMAS_ANY_STRICT;
    } else if (xmlStrEqual(pc, (const xmlChar *) "skip")) {
        wildc->processContents = XML_SCHEMAS_ANY_SKIP;
    } else if (xmlStrEqual(pc, (const xmlChar *) "lax")) {
        wildc->processContents = XML_SCHEMAS_ANY_LAX;
    } else {
        xmlSchemaPSimpleTypeErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    NULL, node,
	    NULL, "(strict | skip | lax)", pc,
	    NULL, NULL, NULL);
        wildc->processContents = XML_SCHEMAS_ANY_STRICT;
	ret = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
    }
    attr = xmlSchemaGetPropNode(node, "namespace");
    ns = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
    if ((attr == NULL) || (xmlStrEqual(ns, BAD_CAST "##any")))
	wildc->any = 1;
    else if (xmlStrEqual(ns, BAD_CAST "##other")) {
	wildc->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
	if (wildc->negNsSet == NULL) {
	    return (-1);
	}
	wildc->negNsSet->value = ctxt->targetNamespace;
    } else {
	const xmlChar *end, *cur;

	cur = ns;
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    nsItem = xmlStrndup(cur, end - cur);
	    if ((xmlStrEqual(nsItem, BAD_CAST "##other")) ||
		    (xmlStrEqual(nsItem, BAD_CAST "##any"))) {
		xmlSchemaPSimpleTypeErr(ctxt,
		    XML_SCHEMAP_WILDCARD_INVALID_NS_MEMBER,
		    NULL, (xmlNodePtr) attr,
		    NULL,
		    "((##any | ##other) | List of (xs:anyURI | "
		    "(##targetNamespace | ##local)))",
		    nsItem, NULL, NULL, NULL);
		ret = XML_SCHEMAP_WILDCARD_INVALID_NS_MEMBER;
	    } else {
		if (xmlStrEqual(nsItem, BAD_CAST "##targetNamespace")) {
		    dictnsItem = ctxt->targetNamespace;
		} else if (xmlStrEqual(nsItem, BAD_CAST "##local")) {
		    dictnsItem = NULL;
		} else {
		    xmlSchemaPValAttrNodeValue(ctxt, NULL, attr,
			nsItem, xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI));
		    dictnsItem = xmlDictLookup(ctxt->dict, nsItem, -1);
		}
		tmp = wildc->nsSet;
		while (tmp != NULL) {
		    if (dictnsItem == tmp->value)
			break;
		    tmp = tmp->next;
		}
		if (tmp == NULL) {
		    tmp = xmlSchemaNewWildcardNsConstraint(ctxt);
		    if (tmp == NULL) {
			xmlFree(nsItem);
			return (-1);
		    }
		    tmp->value = dictnsItem;
		    tmp->next = NULL;
		    if (wildc->nsSet == NULL)
			wildc->nsSet = tmp;
		    else if (lastNs != NULL)
			lastNs->next = tmp;
		    lastNs = tmp;
		}

	    }
	    xmlFree(nsItem);
	    cur = end;
	} while (*cur != 0);
    }
    return (ret);
}

static int
xmlSchemaPCheckParticleCorrect_2(xmlSchemaParserCtxtPtr ctxt,
				 xmlSchemaParticlePtr item ATTRIBUTE_UNUSED,
				 xmlNodePtr node,
				 int minOccurs,
				 int maxOccurs) {

    if ((maxOccurs == 0) && ( minOccurs == 0))
	return (0);
    if (maxOccurs != UNBOUNDED) {
	if (maxOccurs < 1) {
	    xmlSchemaPCustomAttrErr(ctxt,
		XML_SCHEMAP_P_PROPS_CORRECT_2_2,
		NULL, NULL,
		xmlSchemaGetPropNode(node, "maxOccurs"),
		"The value must be greater than or equal to 1");
	    return (XML_SCHEMAP_P_PROPS_CORRECT_2_2);
	} else if (minOccurs > maxOccurs) {
	    xmlSchemaPCustomAttrErr(ctxt,
		XML_SCHEMAP_P_PROPS_CORRECT_2_1,
		NULL, NULL,
		xmlSchemaGetPropNode(node, "minOccurs"),
		"The value must not be greater than the value of 'maxOccurs'");
	    return (XML_SCHEMAP_P_PROPS_CORRECT_2_1);
	}
    }
    return (0);
}

static xmlSchemaParticlePtr
xmlSchemaParseAny(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                  xmlNodePtr node)
{
    xmlSchemaParticlePtr particle;
    xmlNodePtr child = NULL;
    xmlSchemaWildcardPtr wild;
    int min, max;
    xmlAttrPtr attr;
    xmlSchemaAnnotPtr annot = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "minOccurs")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "maxOccurs")) &&
	        (!xmlStrEqual(attr->name, BAD_CAST "namespace")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "processContents"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1,
	"(xs:nonNegativeInteger | unbounded)");
    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1,
	"xs:nonNegativeInteger");
    xmlSchemaPCheckParticleCorrect_2(ctxt, NULL, node, min, max);
    wild = xmlSchemaAddWildcard(ctxt, schema, XML_SCHEMA_TYPE_ANY, node);
    if (wild == NULL)
	return (NULL);
    xmlSchemaParseWildcardNs(ctxt, schema, wild, node);
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?)");
    }
    if ((min == 0) && (max == 0)) {
	
	return (NULL);
    }
    particle = xmlSchemaAddParticle(ctxt, node, min, max);
    if (particle == NULL)
        return (NULL);
    particle->annot = annot;
    particle->children = (xmlSchemaTreeItemPtr) wild;

    return (particle);
}

static xmlSchemaNotationPtr
xmlSchemaParseNotation(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                       xmlNodePtr node)
{
    const xmlChar *name;
    xmlSchemaNotationPtr ret;
    xmlNodePtr child = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    name = xmlSchemaGetProp(ctxt, node, "name");
    if (name == NULL) {
        xmlSchemaPErr2(ctxt, node, child, XML_SCHEMAP_NOTATION_NO_NAME,
                       "Notation has no name\n", NULL, NULL);
        return (NULL);
    }
    ret = xmlSchemaAddNotation(ctxt, schema, name,
	ctxt->targetNamespace, node);
    if (ret == NULL)
        return (NULL);
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");

    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?)");
    }

    return (ret);
}

static xmlSchemaWildcardPtr
xmlSchemaParseAnyAttribute(xmlSchemaParserCtxtPtr ctxt,
                           xmlSchemaPtr schema, xmlNodePtr node)
{
    xmlSchemaWildcardPtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    ret = xmlSchemaAddWildcard(ctxt, schema, XML_SCHEMA_TYPE_ANY_ATTRIBUTE,
	node);
    if (ret == NULL) {
        return (NULL);
    }
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
	        (!xmlStrEqual(attr->name, BAD_CAST "namespace")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "processContents"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    if (xmlSchemaParseWildcardNs(ctxt, schema, ret, node) != 0)
	return (NULL);
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?)");
    }

    return (ret);
}


static xmlSchemaBasicItemPtr
xmlSchemaParseLocalAttribute(xmlSchemaParserCtxtPtr pctxt,
			     xmlSchemaPtr schema,
			     xmlNodePtr node,
			     xmlSchemaItemListPtr uses,
			     int parentType)
{
    const xmlChar *attrValue, *name = NULL, *ns = NULL;
    xmlSchemaAttributeUsePtr use = NULL;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *tmpNs = NULL, *tmpName = NULL, *defValue = NULL;
    int isRef = 0, occurs = XML_SCHEMAS_ATTR_USE_OPTIONAL;
    int	nberrors, hasForm = 0, defValueType = 0;

#define WXS_ATTR_DEF_VAL_DEFAULT 1
#define WXS_ATTR_DEF_VAL_FIXED 2


    if ((pctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    attr = xmlSchemaGetPropNode(node, "ref");
    if (attr != NULL) {
	if (xmlSchemaPValAttrNodeQName(pctxt, schema,
	    NULL, attr, &tmpNs, &tmpName) != 0) {
	    return (NULL);
	}
	if (xmlSchemaCheckReference(pctxt, schema, node, attr, tmpNs) != 0)
	    return(NULL);
	isRef = 1;
    }
    nberrors = pctxt->nberrors;
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if (isRef) {
		if (xmlStrEqual(attr->name, BAD_CAST "id")) {
		    xmlSchemaPValAttrNodeID(pctxt, attr);
		    goto attr_next;
		} else if (xmlStrEqual(attr->name, BAD_CAST "ref")) {
		    goto attr_next;
		}
	    } else {
		if (xmlStrEqual(attr->name, BAD_CAST "name")) {
		    goto attr_next;
		} else if (xmlStrEqual(attr->name, BAD_CAST "id")) {
		    xmlSchemaPValAttrNodeID(pctxt, attr);
		    goto attr_next;
		} else if (xmlStrEqual(attr->name, BAD_CAST "type")) {
		    xmlSchemaPValAttrNodeQName(pctxt, schema, NULL,
			attr, &tmpNs, &tmpName);
		    goto attr_next;
		} else if (xmlStrEqual(attr->name, BAD_CAST "form")) {
		    hasForm = 1;
		    attrValue = xmlSchemaGetNodeContent(pctxt,
			(xmlNodePtr) attr);
		    if (xmlStrEqual(attrValue, BAD_CAST "qualified")) {
			ns = pctxt->targetNamespace;
		    } else if (!xmlStrEqual(attrValue, BAD_CAST "unqualified"))
		    {
			xmlSchemaPSimpleTypeErr(pctxt,
			    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			    NULL, (xmlNodePtr) attr,
			    NULL, "(qualified | unqualified)",
			    attrValue, NULL, NULL, NULL);
		    }
		    goto attr_next;
		}
	    }
	    if (xmlStrEqual(attr->name, BAD_CAST "use")) {

		attrValue = xmlSchemaGetNodeContent(pctxt, (xmlNodePtr) attr);
		
		if (xmlStrEqual(attrValue, BAD_CAST "optional"))
		    occurs = XML_SCHEMAS_ATTR_USE_OPTIONAL;
		else if (xmlStrEqual(attrValue, BAD_CAST "prohibited"))
		    occurs = XML_SCHEMAS_ATTR_USE_PROHIBITED;
		else if (xmlStrEqual(attrValue, BAD_CAST "required"))
		    occurs = XML_SCHEMAS_ATTR_USE_REQUIRED;
		else {
		    xmlSchemaPSimpleTypeErr(pctxt,
			XML_SCHEMAP_INVALID_ATTR_USE,
			NULL, (xmlNodePtr) attr,
			NULL, "(optional | prohibited | required)",
			attrValue, NULL, NULL, NULL);
		}
		goto attr_next;
	    } else if (xmlStrEqual(attr->name, BAD_CAST "default")) {
		if (defValue) {
		    xmlSchemaPMutualExclAttrErr(pctxt,
			XML_SCHEMAP_SRC_ATTRIBUTE_1,
			NULL, attr, "default", "fixed");
		} else {
		    defValue = xmlSchemaGetNodeContent(pctxt, (xmlNodePtr) attr);
		    defValueType = WXS_ATTR_DEF_VAL_DEFAULT;
		}
		goto attr_next;
	    } else if (xmlStrEqual(attr->name, BAD_CAST "fixed")) {
		if (defValue) {
		    xmlSchemaPMutualExclAttrErr(pctxt,
			XML_SCHEMAP_SRC_ATTRIBUTE_1,
			NULL, attr, "default", "fixed");
		} else {
		    defValue = xmlSchemaGetNodeContent(pctxt, (xmlNodePtr) attr);
		    defValueType = WXS_ATTR_DEF_VAL_FIXED;
		}
		goto attr_next;
	    }
	} else if (! xmlStrEqual(attr->ns->href, xmlSchemaNs))
	    goto attr_next;

	xmlSchemaPIllegalAttrErr(pctxt,
	    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);

attr_next:
	attr = attr->next;
    }
    if ((defValueType == WXS_ATTR_DEF_VAL_DEFAULT) &&
	(occurs != XML_SCHEMAS_ATTR_USE_OPTIONAL)) {
	xmlSchemaPSimpleTypeErr(pctxt,
	    XML_SCHEMAP_SRC_ATTRIBUTE_2,
	    NULL, node, NULL,
	    "(optional | prohibited | required)", NULL,
	    "The value of the attribute 'use' must be 'optional' "
	    "if the attribute 'default' is present",
	    NULL, NULL);
    }
    if (nberrors != pctxt->nberrors)
	return(NULL);
    if (! isRef) {
	xmlSchemaAttributePtr attrDecl;

	
	if ((! hasForm) && (schema->flags & XML_SCHEMAS_QUALIF_ATTR))
	    ns = pctxt->targetNamespace;
	if (xmlStrEqual(ns, xmlSchemaInstanceNs)) {
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_NO_XSI,
		node, NULL,
		"The target namespace must not match '%s'",
		xmlSchemaInstanceNs, NULL);
	}
	attr = xmlSchemaGetPropNode(node, "name");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(pctxt, XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, node, "name", NULL);
	    return (NULL);
	}
	if (xmlSchemaPValAttrNode(pctxt, NULL, attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	    return (NULL);
	}
	if (xmlStrEqual(name, BAD_CAST "xmlns")) {
	    xmlSchemaPSimpleTypeErr(pctxt,
		XML_SCHEMAP_NO_XMLNS,
		NULL, (xmlNodePtr) attr,
		xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), NULL, NULL,
		"The value of the attribute must not match 'xmlns'",
		NULL, NULL);
	    return (NULL);
	}
	if (occurs == XML_SCHEMAS_ATTR_USE_PROHIBITED)
	    goto check_children;
	use = xmlSchemaAddAttributeUse(pctxt, node);
	if (use == NULL)
	    return(NULL);
	use->occurs = occurs;
	attrDecl = xmlSchemaAddAttribute(pctxt, schema, name, ns, node, 0);
	if (attrDecl == NULL)
	    return (NULL);
	if (tmpName != NULL) {
	    attrDecl->typeName = tmpName;
	    attrDecl->typeNs = tmpNs;
	}
	use->attrDecl = attrDecl;
	if (defValue != NULL) {
	    attrDecl->defValue = defValue;
	    if (defValueType == WXS_ATTR_DEF_VAL_FIXED)
		attrDecl->flags |= XML_SCHEMAS_ATTR_FIXED;
	}
    } else if (occurs != XML_SCHEMAS_ATTR_USE_PROHIBITED) {
	xmlSchemaQNameRefPtr ref;

	use = xmlSchemaAddAttributeUse(pctxt, node);
	if (use == NULL)
	    return(NULL);
	WXS_ADD_PENDING(pctxt, use);
	use->occurs = occurs;
	ref = xmlSchemaNewQNameRef(pctxt, XML_SCHEMA_TYPE_ATTRIBUTE,
	    tmpName, tmpNs);
	if (ref == NULL)
	    return(NULL);
	use->attrDecl = WXS_ATTR_CAST ref;
	if (defValue != NULL)
	    use->defValue = defValue;
	    if (defValueType == WXS_ATTR_DEF_VAL_FIXED)
		use->flags |= XML_SCHEMA_ATTR_USE_FIXED;
    }

check_children:
    child = node->children;
    if (occurs == XML_SCHEMAS_ATTR_USE_PROHIBITED) {
	xmlSchemaAttributeUseProhibPtr prohib;

	if (IS_SCHEMA(child, "annotation")) {
	    xmlSchemaParseAnnotation(pctxt, child, 0);
	    child = child->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(pctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?)");
	}
	if (parentType == XML_SCHEMA_TYPE_ATTRIBUTEGROUP) {
	    xmlSchemaCustomWarning(ACTXT_CAST pctxt,
		XML_SCHEMAP_WARN_ATTR_POINTLESS_PROH,
		node, NULL,
		"Skipping attribute use prohibition, since it is "
		"pointless inside an <attributeGroup>",
		NULL, NULL, NULL);
	    return(NULL);
	} else if (parentType == XML_SCHEMA_TYPE_EXTENSION) {
	    xmlSchemaCustomWarning(ACTXT_CAST pctxt,
		XML_SCHEMAP_WARN_ATTR_POINTLESS_PROH,
		node, NULL,
		"Skipping attribute use prohibition, since it is "
		"pointless when extending a type",
		NULL, NULL, NULL);
	    return(NULL);
	}
	if (! isRef) {
	    tmpName = name;
	    tmpNs = ns;
	}
	if (uses) {
	    int i;

	    for (i = 0; i < uses->nbItems; i++) {
		use = uses->items[i];
		if ((use->type == XML_SCHEMA_EXTRA_ATTR_USE_PROHIB) &&
		    (tmpName == (WXS_ATTR_PROHIB_CAST use)->name) &&
		    (tmpNs == (WXS_ATTR_PROHIB_CAST use)->targetNamespace))
		{
		    xmlChar *str = NULL;

		    xmlSchemaCustomWarning(ACTXT_CAST pctxt,
			XML_SCHEMAP_WARN_ATTR_POINTLESS_PROH,
			node, NULL,
			"Skipping duplicate attribute use prohibition '%s'",
			xmlSchemaFormatQName(&str, tmpNs, tmpName),
			NULL, NULL);
		    FREE_AND_NULL(str)
		    return(NULL);
		}
	    }
	}
	prohib = xmlSchemaAddAttributeUseProhib(pctxt);
	if (prohib == NULL)
	    return(NULL);
	prohib->node = node;
	prohib->name = tmpName;
	prohib->targetNamespace = tmpNs;
	if (isRef) {
	    WXS_ADD_PENDING(pctxt, prohib);
	}
	return(WXS_BASIC_CAST prohib);
    } else {
	if (IS_SCHEMA(child, "annotation")) {
	    use->annot = xmlSchemaParseAnnotation(pctxt, child, 1);
	    child = child->next;
	}
	if (isRef) {
	    if (child != NULL) {
		if (IS_SCHEMA(child, "simpleType"))
		    xmlSchemaPContentErr(pctxt,
			XML_SCHEMAP_SRC_ATTRIBUTE_3_2,
			NULL, node, child, NULL,
			"(annotation?)");
		else
		    xmlSchemaPContentErr(pctxt,
			XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
			NULL, node, child, NULL,
			"(annotation?)");
	    }
	} else {
	    if (IS_SCHEMA(child, "simpleType")) {
		if (WXS_ATTRUSE_DECL(use)->typeName != NULL) {
		    xmlSchemaPContentErr(pctxt, XML_SCHEMAP_SRC_ATTRIBUTE_4,
			NULL, node, child,
			"The attribute 'type' and the <simpleType> child "
			"are mutually exclusive", NULL);
		} else
		    WXS_ATTRUSE_TYPEDEF(use) =
			xmlSchemaParseSimpleType(pctxt, schema, child, 0);
		child = child->next;
	    }
	    if (child != NULL)
		xmlSchemaPContentErr(pctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, simpleType?)");
	}
    }
    return (WXS_BASIC_CAST use);
}


static xmlSchemaAttributePtr
xmlSchemaParseGlobalAttribute(xmlSchemaParserCtxtPtr pctxt,
			      xmlSchemaPtr schema,
			      xmlNodePtr node)
{
    const xmlChar *attrValue;
    xmlSchemaAttributePtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((pctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    attr = xmlSchemaGetPropNode(node, "name");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(pctxt, XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node, "name", NULL);
	return (NULL);
    }
    if (xmlSchemaPValAttrNode(pctxt, NULL, attr,
	xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &attrValue) != 0) {
	return (NULL);
    }
    if (xmlStrEqual(attrValue, BAD_CAST "xmlns")) {
	xmlSchemaPSimpleTypeErr(pctxt,
	    XML_SCHEMAP_NO_XMLNS,
	    NULL, (xmlNodePtr) attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), NULL, NULL,
	    "The value of the attribute must not match 'xmlns'",
	    NULL, NULL);
	return (NULL);
    }
    if (xmlStrEqual(pctxt->targetNamespace, xmlSchemaInstanceNs)) {
	xmlSchemaCustomErr(ACTXT_CAST pctxt,
	    XML_SCHEMAP_NO_XSI, node, NULL,
	    "The target namespace must not match '%s'",
	    xmlSchemaInstanceNs, NULL);
    }

    ret = xmlSchemaAddAttribute(pctxt, schema, attrValue,
	pctxt->targetNamespace, node, 1);
    if (ret == NULL)
	return (NULL);
    ret->flags |= XML_SCHEMAS_ATTR_GLOBAL;

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "default")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "fixed")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "type")))
	    {
		xmlSchemaPIllegalAttrErr(pctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(pctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrQName(pctxt, schema, NULL,
	node, "type", &ret->typeNs, &ret->typeName);

    xmlSchemaPValAttrID(pctxt, node, BAD_CAST "id");
    ret->defValue = xmlSchemaGetProp(pctxt, node, "fixed");
    if (ret->defValue != NULL)
	ret->flags |= XML_SCHEMAS_ATTR_FIXED;
    attr = xmlSchemaGetPropNode(node, "default");
    if (attr != NULL) {
	if (ret->flags & XML_SCHEMAS_ATTR_FIXED) {
	    xmlSchemaPMutualExclAttrErr(pctxt, XML_SCHEMAP_SRC_ATTRIBUTE_1,
		WXS_BASIC_CAST ret, attr, "default", "fixed");
	} else
	    ret->defValue = xmlSchemaGetNodeContent(pctxt, (xmlNodePtr) attr);
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(pctxt, child, 1);
        child = child->next;
    }
    if (IS_SCHEMA(child, "simpleType")) {
	if (ret->typeName != NULL) {
	    xmlSchemaPContentErr(pctxt, XML_SCHEMAP_SRC_ATTRIBUTE_4,
		NULL, node, child,
		"The attribute 'type' and the <simpleType> child "
		"are mutually exclusive", NULL);
	} else
	    ret->subtypes = xmlSchemaParseSimpleType(pctxt, schema, child, 0);
	child = child->next;
    }
    if (child != NULL)
	xmlSchemaPContentErr(pctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?, simpleType?)");

    return (ret);
}

static xmlSchemaQNameRefPtr
xmlSchemaParseAttributeGroupRef(xmlSchemaParserCtxtPtr pctxt,
				xmlSchemaPtr schema,
				xmlNodePtr node)
{
    xmlSchemaQNameRefPtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *refNs = NULL, *ref = NULL;

    if ((pctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    attr = xmlSchemaGetPropNode(node, "ref");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(pctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node, "ref", NULL);
	return (NULL);
    }
    xmlSchemaPValAttrNodeQName(pctxt, schema,
	NULL, attr, &refNs, &ref);
    if (xmlSchemaCheckReference(pctxt, schema, node, attr, refNs) != 0)
	return(NULL);

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "ref")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "id")))
	    {
		xmlSchemaPIllegalAttrErr(pctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(pctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    
    xmlSchemaPValAttrID(pctxt, node, BAD_CAST "id");

    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        xmlSchemaParseAnnotation(pctxt, child, 0);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(pctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?)");
    }

    if (pctxt->isRedefine && pctxt->redef &&
	(pctxt->redef->item->type ==
	    XML_SCHEMA_TYPE_ATTRIBUTEGROUP) &&
	(ref == pctxt->redef->refName) &&
	(refNs == pctxt->redef->refTargetNs))
    {
	if (pctxt->redefCounter != 0) {
	    xmlChar *str = NULL;

	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_SRC_REDEFINE, node, NULL,
		"The redefining attribute group definition "
		"'%s' must not contain more than one "
		"reference to the redefined definition",
		xmlSchemaFormatQName(&str, refNs, ref), NULL);
	    FREE_AND_NULL(str);
	    return(NULL);
	}
	pctxt->redefCounter++;
	ret = xmlSchemaNewQNameRef(pctxt,
	    XML_SCHEMA_TYPE_ATTRIBUTEGROUP, ref, refNs);
	if (ret == NULL)
	    return(NULL);
	ret->node = node;
	pctxt->redef->reference = WXS_BASIC_CAST ret;
    } else {
	ret = xmlSchemaNewQNameRef(pctxt,
	    XML_SCHEMA_TYPE_ATTRIBUTEGROUP, ref, refNs);
	if (ret == NULL)
	    return(NULL);
	ret->node = node;
	
	WXS_ADD_PENDING(pctxt, ret);
    }
    return (ret);
}

static xmlSchemaAttributeGroupPtr
xmlSchemaParseAttributeGroupDefinition(xmlSchemaParserCtxtPtr pctxt,
				       xmlSchemaPtr schema,
				       xmlNodePtr node)
{
    const xmlChar *name;
    xmlSchemaAttributeGroupPtr ret;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    int hasRefs = 0;

    if ((pctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    attr = xmlSchemaGetPropNode(node, "name");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(pctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node, "name", NULL);
	return (NULL);
    }
    if (xmlSchemaPValAttrNode(pctxt,
	NULL, attr,
	xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	return (NULL);
    }
    ret = xmlSchemaAddAttributeGroupDefinition(pctxt, schema,
	name, pctxt->targetNamespace, node);
    if (ret == NULL)
	return (NULL);
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "id")))
	    {
		xmlSchemaPIllegalAttrErr(pctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(pctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    
    xmlSchemaPValAttrID(pctxt, node, BAD_CAST "id");
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        ret->annot = xmlSchemaParseAnnotation(pctxt, child, 1);
        child = child->next;
    }
    if (xmlSchemaParseLocalAttributes(pctxt, schema, &child,
	(xmlSchemaItemListPtr *) &(ret->attrUses),
	XML_SCHEMA_TYPE_ATTRIBUTEGROUP, &hasRefs) == -1)
	return(NULL);
    if (hasRefs)
	ret->flags |= XML_SCHEMAS_ATTRGROUP_HAS_REFS;
    if (IS_SCHEMA(child, "anyAttribute")) {
	ret->attributeWildcard = xmlSchemaParseAnyAttribute(pctxt,
	    schema, child);
	child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(pctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?, ((attribute | attributeGroup)*, anyAttribute?))");
    }
    return (ret);
}

static int
xmlSchemaPValAttrFormDefault(const xmlChar *value,
			     int *flags,
			     int flagQualified)
{
    if (xmlStrEqual(value, BAD_CAST "qualified")) {
	if  ((*flags & flagQualified) == 0)
	    *flags |= flagQualified;
    } else if (!xmlStrEqual(value, BAD_CAST "unqualified"))
	return (1);

    return (0);
}


static int
xmlSchemaPValAttrBlockFinal(const xmlChar *value,
			    int *flags,
			    int flagAll,
			    int flagExtension,
			    int flagRestriction,
			    int flagSubstitution,
			    int flagList,
			    int flagUnion)
{
    int ret = 0;

    if ((flags == NULL) || (value == NULL))
	return (-1);
    if (value[0] == 0)
	return (0);
    if (xmlStrEqual(value, BAD_CAST "#all")) {
	if (flagAll != -1)
	    *flags |= flagAll;
	else {
	    if (flagExtension != -1)
		*flags |= flagExtension;
	    if (flagRestriction != -1)
		*flags |= flagRestriction;
	    if (flagSubstitution != -1)
		*flags |= flagSubstitution;
	    if (flagList != -1)
		*flags |= flagList;
	    if (flagUnion != -1)
		*flags |= flagUnion;
	}
    } else {
	const xmlChar *end, *cur = value;
	xmlChar *item;

	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    item = xmlStrndup(cur, end - cur);
	    if (xmlStrEqual(item, BAD_CAST "extension")) {
		if (flagExtension != -1) {
		    if ((*flags & flagExtension) == 0)
			*flags |= flagExtension;
		} else
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "restriction")) {
		if (flagRestriction != -1) {
		    if ((*flags & flagRestriction) == 0)
			*flags |= flagRestriction;
		} else
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "substitution")) {
		if (flagSubstitution != -1) {
		    if ((*flags & flagSubstitution) == 0)
			*flags |= flagSubstitution;
		} else
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "list")) {
		if (flagList != -1) {
		    if ((*flags & flagList) == 0)
			*flags |= flagList;
		} else
		    ret = 1;
	    } else if (xmlStrEqual(item, BAD_CAST "union")) {
		if (flagUnion != -1) {
		    if ((*flags & flagUnion) == 0)
			*flags |= flagUnion;
		} else
		    ret = 1;
	    } else
		ret = 1;
	    if (item != NULL)
		xmlFree(item);
	    cur = end;
	} while ((ret == 0) && (*cur != 0));
    }

    return (ret);
}

static int
xmlSchemaCheckCSelectorXPath(xmlSchemaParserCtxtPtr ctxt,
			     xmlSchemaIDCPtr idc,
			     xmlSchemaIDCSelectPtr selector,
			     xmlAttrPtr attr,
			     int isField)
{
    xmlNodePtr node;

    if (selector == NULL) {
	xmlSchemaPErr(ctxt, idc->node,
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaCheckCSelectorXPath, "
	    "the selector is not specified.\n", NULL, NULL);
	return (-1);
    }
    if (attr == NULL)
	node = idc->node;
    else
	node = (xmlNodePtr) attr;
    if (selector->xpath == NULL) {
	xmlSchemaPCustomErr(ctxt,
	    
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    NULL, node,
	    "The XPath expression of the selector is not valid", NULL);
	return (XML_SCHEMAP_S4S_ATTR_INVALID_VALUE);
    } else {
	const xmlChar **nsArray = NULL;
	xmlNsPtr *nsList = NULL;
	if (attr == NULL)
	    nsList = NULL;
	else
	    nsList = xmlGetNsList(attr->doc, attr->parent);
	if (nsList != NULL) {
	    int i, count = 0;

	    for (i = 0; nsList[i] != NULL; i++)
		count++;

	    nsArray = (const xmlChar **) xmlMalloc(
		(count * 2 + 1) * sizeof(const xmlChar *));
	    if (nsArray == NULL) {
		xmlSchemaPErrMemory(ctxt, "allocating a namespace array",
		    NULL);
		xmlFree(nsList);
		return (-1);
	    }
	    for (i = 0; i < count; i++) {
		nsArray[2 * i] = nsList[i]->href;
		nsArray[2 * i + 1] = nsList[i]->prefix;
	    }
	    nsArray[count * 2] = NULL;
	    xmlFree(nsList);
	}
	if (isField)
	    selector->xpathComp = (void *) xmlPatterncompile(selector->xpath,
		NULL, XML_PATTERN_XSFIELD, nsArray);
	else
	    selector->xpathComp = (void *) xmlPatterncompile(selector->xpath,
		NULL, XML_PATTERN_XSSEL, nsArray);
	if (nsArray != NULL)
	    xmlFree((xmlChar **) nsArray);

	if (selector->xpathComp == NULL) {
	    xmlSchemaPCustomErr(ctxt,
		
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		NULL, node,
		"The XPath expression '%s' could not be "
		"compiled", selector->xpath);
	    return (XML_SCHEMAP_S4S_ATTR_INVALID_VALUE);
	}
    }
    return (0);
}

#define ADD_ANNOTATION(annot)   \
    xmlSchemaAnnotPtr cur = item->annot; \
    if (item->annot == NULL) {  \
	item->annot = annot;    \
	return (annot);         \
    }                           \
    cur = item->annot;          \
    if (cur->next != NULL) {    \
	cur = cur->next;	\
    }                           \
    cur->next = annot;

static xmlSchemaAnnotPtr
xmlSchemaAddAnnotation(xmlSchemaAnnotItemPtr annItem,
		       xmlSchemaAnnotPtr annot)
{
    if ((annItem == NULL) || (annot == NULL))
	return (NULL);
    switch (annItem->type) {
	case XML_SCHEMA_TYPE_ELEMENT: {
		xmlSchemaElementPtr item = (xmlSchemaElementPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTE: {
		xmlSchemaAttributePtr item = (xmlSchemaAttributePtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_ANY_ATTRIBUTE:
	case XML_SCHEMA_TYPE_ANY: {
		xmlSchemaWildcardPtr item = (xmlSchemaWildcardPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_PARTICLE:
	case XML_SCHEMA_TYPE_IDC_KEY:
	case XML_SCHEMA_TYPE_IDC_KEYREF:
	case XML_SCHEMA_TYPE_IDC_UNIQUE: {
		xmlSchemaAnnotItemPtr item = (xmlSchemaAnnotItemPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP: {
		xmlSchemaAttributeGroupPtr item =
		    (xmlSchemaAttributeGroupPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_NOTATION: {
		xmlSchemaNotationPtr item = (xmlSchemaNotationPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_FACET_MININCLUSIVE:
	case XML_SCHEMA_FACET_MINEXCLUSIVE:
	case XML_SCHEMA_FACET_MAXINCLUSIVE:
	case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	case XML_SCHEMA_FACET_TOTALDIGITS:
	case XML_SCHEMA_FACET_FRACTIONDIGITS:
	case XML_SCHEMA_FACET_PATTERN:
	case XML_SCHEMA_FACET_ENUMERATION:
	case XML_SCHEMA_FACET_WHITESPACE:
	case XML_SCHEMA_FACET_LENGTH:
	case XML_SCHEMA_FACET_MAXLENGTH:
	case XML_SCHEMA_FACET_MINLENGTH: {
		xmlSchemaFacetPtr item = (xmlSchemaFacetPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_SIMPLE:
	case XML_SCHEMA_TYPE_COMPLEX: {
		xmlSchemaTypePtr item = (xmlSchemaTypePtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_GROUP: {
		xmlSchemaModelGroupDefPtr item = (xmlSchemaModelGroupDefPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	case XML_SCHEMA_TYPE_SEQUENCE:
	case XML_SCHEMA_TYPE_CHOICE:
	case XML_SCHEMA_TYPE_ALL: {
		xmlSchemaModelGroupPtr item = (xmlSchemaModelGroupPtr) annItem;
		ADD_ANNOTATION(annot)
	    }
	    break;
	default:
	     xmlSchemaPCustomErr(NULL,
		XML_SCHEMAP_INTERNAL,
		NULL, NULL,
		"Internal error: xmlSchemaAddAnnotation, "
		"The item is not a annotated schema component", NULL);
	     break;
    }
    return (annot);
}

static xmlSchemaIDCSelectPtr
xmlSchemaParseIDCSelectorAndField(xmlSchemaParserCtxtPtr ctxt,
			  xmlSchemaIDCPtr idc,
			  xmlNodePtr node,
			  int isField)
{
    xmlSchemaIDCSelectPtr item;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "xpath"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    item = (xmlSchemaIDCSelectPtr) xmlMalloc(sizeof(xmlSchemaIDCSelect));
    if (item == NULL) {
        xmlSchemaPErrMemory(ctxt,
	    "allocating a 'selector' of an identity-constraint definition",
	    NULL);
        return (NULL);
    }
    memset(item, 0, sizeof(xmlSchemaIDCSelect));
    attr = xmlSchemaGetPropNode(node, "xpath");
    if (attr == NULL) {
    	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node,
	    "name", NULL);
    } else {
	item->xpath = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);

	if (xmlSchemaCheckCSelectorXPath(ctxt, idc, item, attr,
	    isField) == -1) {
	    xmlSchemaPErr(ctxt,
		(xmlNodePtr) attr,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaParseIDCSelectorAndField, "
		"validating the XPath expression of a IDC selector.\n",
		NULL, NULL);
	}

    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) idc,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
	child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?)");
    }

    return (item);
}

static xmlSchemaIDCPtr
xmlSchemaParseIDC(xmlSchemaParserCtxtPtr ctxt,
		  xmlSchemaPtr schema,
		  xmlNodePtr node,
		  xmlSchemaTypeType idcCategory,
		  const xmlChar *targetNamespace)
{
    xmlSchemaIDCPtr item = NULL;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *name = NULL;
    xmlSchemaIDCSelectPtr field = NULL, lastField = NULL;

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		((idcCategory != XML_SCHEMA_TYPE_IDC_KEYREF) ||
		 (!xmlStrEqual(attr->name, BAD_CAST "refer")))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    attr = xmlSchemaGetPropNode(node, "name");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node,
	    "name", NULL);
	return (NULL);
    } else if (xmlSchemaPValAttrNode(ctxt,
	NULL, attr,
	xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	return (NULL);
    }
    
    item = xmlSchemaAddIDC(ctxt, schema, name, targetNamespace,
	idcCategory, node);
    if (item == NULL)
	return(NULL);

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    if (idcCategory == XML_SCHEMA_TYPE_IDC_KEYREF) {
	attr = xmlSchemaGetPropNode(node, "refer");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, node,
		"refer", NULL);
	} else {
	    item->ref = xmlSchemaNewQNameRef(ctxt, XML_SCHEMA_TYPE_IDC_KEY,
		NULL, NULL);
	    if (item->ref == NULL)
		return (NULL);
	    xmlSchemaPValAttrNodeQName(ctxt, schema,
		NULL, attr,
		&(item->ref->targetNamespace),
		&(item->ref->name));
	    xmlSchemaCheckReference(ctxt, schema, node, attr,
		item->ref->targetNamespace);
	}
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	item->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
	child = child->next;
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_MISSING,
		NULL, node, child,
		"A child element is missing",
		"(annotation?, (selector, field+))");
    }
    if (IS_SCHEMA(child, "selector")) {
	item->selector = xmlSchemaParseIDCSelectorAndField(ctxt,
	    item, child, 0);
	child = child->next;
	if (IS_SCHEMA(child, "field")) {
	    do {
		field = xmlSchemaParseIDCSelectorAndField(ctxt,
		    item, child, 1);
		if (field != NULL) {
		    field->index = item->nbFields;
		    item->nbFields++;
		    if (lastField != NULL)
			lastField->next = field;
		    else
			item->fields = field;
		    lastField = field;
		}
		child = child->next;
	    } while (IS_SCHEMA(child, "field"));
	} else {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child,
		NULL, "(annotation?, (selector, field+))");
	}
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?, (selector, field+))");
    }

    return (item);
}

static xmlSchemaBasicItemPtr
xmlSchemaParseElement(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                      xmlNodePtr node, int *isElemRef, int topLevel)
{
    xmlSchemaElementPtr decl = NULL;
    xmlSchemaParticlePtr particle = NULL;
    xmlSchemaAnnotPtr annot = NULL;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr, nameAttr;
    int min, max, isRef = 0;
    xmlChar *des = NULL;

    
    

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    if (isElemRef != NULL)
	*isElemRef = 0;
    nameAttr = xmlSchemaGetPropNode(node, "name");
    attr = xmlSchemaGetPropNode(node, "ref");
    if ((topLevel) || (attr == NULL)) {
	if (nameAttr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, node, "name", NULL);
	    return (NULL);
	}
    } else
	isRef = 1;

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	annot = xmlSchemaParseAnnotation(ctxt, child, 1);
	child = child->next;
    }
    if (topLevel)
	goto declaration_part;
    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, "xs:nonNegativeInteger");
    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1, "(xs:nonNegativeInteger | unbounded)");
    xmlSchemaPCheckParticleCorrect_2(ctxt, NULL, node, min, max);
    particle = xmlSchemaAddParticle(ctxt, node, min, max);
    if (particle == NULL)
	goto return_null;

    

    if (isRef) {
	const xmlChar *refNs = NULL, *ref = NULL;
	xmlSchemaQNameRefPtr refer = NULL;
	if (isElemRef != NULL)
	    *isElemRef = 1;

	xmlSchemaPValAttrNodeQName(ctxt, schema,
	    NULL, attr, &refNs, &ref);
	xmlSchemaCheckReference(ctxt, schema, node, attr, refNs);
	if (nameAttr != NULL) {
	    xmlSchemaPMutualExclAttrErr(ctxt,
		XML_SCHEMAP_SRC_ELEMENT_2_1, NULL, nameAttr, "ref", "name");
	}
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (xmlStrEqual(attr->name, BAD_CAST "ref") ||
		    xmlStrEqual(attr->name, BAD_CAST "name") ||
		    xmlStrEqual(attr->name, BAD_CAST "id") ||
		    xmlStrEqual(attr->name, BAD_CAST "maxOccurs") ||
		    xmlStrEqual(attr->name, BAD_CAST "minOccurs"))
		{
		    attr = attr->next;
		    continue;
		} else {
		    
		    xmlSchemaPCustomAttrErr(ctxt,
			XML_SCHEMAP_SRC_ELEMENT_2_2,
			NULL, NULL, attr,
			"Only the attributes 'minOccurs', 'maxOccurs' and "
			"'id' are allowed in addition to 'ref'");
		    break;
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	    attr = attr->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL, "(annotation?)");
	}
	if ((min == 0) && (max == 0))
	    goto return_null;
	refer = xmlSchemaNewQNameRef(ctxt, XML_SCHEMA_TYPE_ELEMENT,
	    ref, refNs);
	if (refer == NULL)
	    goto return_null;
	particle->children = (xmlSchemaTreeItemPtr) refer;
	particle->annot = annot;
	WXS_ADD_PENDING(ctxt, particle);
	return ((xmlSchemaBasicItemPtr) particle);
    }
declaration_part:
    {
	const xmlChar *ns = NULL, *fixed, *name, *attrValue;
	xmlSchemaIDCPtr curIDC = NULL, lastIDC = NULL;

	if (xmlSchemaPValAttrNode(ctxt, NULL, nameAttr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0)
	    goto return_null;
	if (topLevel) {
	    ns = ctxt->targetNamespace;
	} else {
	    attr = xmlSchemaGetPropNode(node, "form");
	    if (attr != NULL) {
		attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
		if (xmlStrEqual(attrValue, BAD_CAST "qualified")) {
		    ns = ctxt->targetNamespace;
		} else if (!xmlStrEqual(attrValue, BAD_CAST "unqualified")) {
		    xmlSchemaPSimpleTypeErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			NULL, (xmlNodePtr) attr,
			NULL, "(qualified | unqualified)",
			attrValue, NULL, NULL, NULL);
		}
	    } else if (schema->flags & XML_SCHEMAS_QUALIF_ELEM)
		ns = ctxt->targetNamespace;
	}
	decl = xmlSchemaAddElement(ctxt, name, ns, node, topLevel);
	if (decl == NULL) {
	    goto return_null;
	}
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if ((!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "type")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "default")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "fixed")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "block")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "nillable")))
		{
		    if (topLevel == 0) {
			if ((!xmlStrEqual(attr->name, BAD_CAST "maxOccurs")) &&
			    (!xmlStrEqual(attr->name, BAD_CAST "minOccurs")) &&
			    (!xmlStrEqual(attr->name, BAD_CAST "form")))
			{
			    xmlSchemaPIllegalAttrErr(ctxt,
				XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
			}
		    } else if ((!xmlStrEqual(attr->name, BAD_CAST "final")) &&
			(!xmlStrEqual(attr->name, BAD_CAST "abstract")) &&
			(!xmlStrEqual(attr->name, BAD_CAST "substitutionGroup"))) {

			xmlSchemaPIllegalAttrErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		    }
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {

		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	    attr = attr->next;
	}
	if (topLevel) {
	    decl->flags |= XML_SCHEMAS_ELEM_GLOBAL;
	    decl->flags |= XML_SCHEMAS_ELEM_TOPLEVEL;
	    xmlSchemaPValAttrQName(ctxt, schema,
		NULL, node, "substitutionGroup",
		&(decl->substGroupNs), &(decl->substGroup));
	    if (xmlGetBooleanProp(ctxt, node, "abstract", 0))
		decl->flags |= XML_SCHEMAS_ELEM_ABSTRACT;
	    attr = xmlSchemaGetPropNode(node, "final");
	    if (attr == NULL) {
		if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_EXTENSION)
		    decl->flags |= XML_SCHEMAS_ELEM_FINAL_EXTENSION;
		if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION)
		    decl->flags |= XML_SCHEMAS_ELEM_FINAL_RESTRICTION;
	    } else {
		attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
		if (xmlSchemaPValAttrBlockFinal(attrValue, &(decl->flags),
		    -1,
		    XML_SCHEMAS_ELEM_FINAL_EXTENSION,
		    XML_SCHEMAS_ELEM_FINAL_RESTRICTION, -1, -1, -1) != 0) {
		    xmlSchemaPSimpleTypeErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			NULL, (xmlNodePtr) attr,
			NULL, "(#all | List of (extension | restriction))",
			attrValue, NULL, NULL, NULL);
		}
	    }
	}
	attr = xmlSchemaGetPropNode(node, "block");
	if (attr == NULL) {
	    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION)
		decl->flags |= XML_SCHEMAS_ELEM_BLOCK_RESTRICTION;
	    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION)
		decl->flags |= XML_SCHEMAS_ELEM_BLOCK_EXTENSION;
	    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION)
		decl->flags |= XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION;
	} else {
	    attrValue = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	    if (xmlSchemaPValAttrBlockFinal(attrValue, &(decl->flags),
		-1,
		XML_SCHEMAS_ELEM_BLOCK_EXTENSION,
		XML_SCHEMAS_ELEM_BLOCK_RESTRICTION,
		XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION, -1, -1) != 0) {
		xmlSchemaPSimpleTypeErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		    NULL, (xmlNodePtr) attr,
		    NULL, "(#all | List of (extension | "
		    "restriction | substitution))", attrValue,
		    NULL, NULL, NULL);
	    }
	}
	if (xmlGetBooleanProp(ctxt, node, "nillable", 0))
	    decl->flags |= XML_SCHEMAS_ELEM_NILLABLE;

	attr = xmlSchemaGetPropNode(node, "type");
	if (attr != NULL) {
	    xmlSchemaPValAttrNodeQName(ctxt, schema,
		NULL, attr,
		&(decl->namedTypeNs), &(decl->namedType));
	    xmlSchemaCheckReference(ctxt, schema, node,
		attr, decl->namedTypeNs);
	}
	decl->value = xmlSchemaGetProp(ctxt, node, "default");
	attr = xmlSchemaGetPropNode(node, "fixed");
	if (attr != NULL) {
	    fixed = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	    if (decl->value != NULL) {
		xmlSchemaPMutualExclAttrErr(ctxt,
		    XML_SCHEMAP_SRC_ELEMENT_1,
		    NULL, attr, "default", "fixed");
	    } else {
		decl->flags |= XML_SCHEMAS_ELEM_FIXED;
		decl->value = fixed;
	    }
	}
	if (IS_SCHEMA(child, "complexType")) {
	    if (decl->namedType != NULL) {
		xmlSchemaPContentErr(ctxt,
		    XML_SCHEMAP_SRC_ELEMENT_3,
		    NULL, node, child,
		    "The attribute 'type' and the <complexType> child are "
		    "mutually exclusive", NULL);
	    } else
		WXS_ELEM_TYPEDEF(decl) = xmlSchemaParseComplexType(ctxt, schema, child, 0);
	    child = child->next;
	} else if (IS_SCHEMA(child, "simpleType")) {
	    if (decl->namedType != NULL) {
		xmlSchemaPContentErr(ctxt,
		    XML_SCHEMAP_SRC_ELEMENT_3,
		    NULL, node, child,
		    "The attribute 'type' and the <simpleType> child are "
		    "mutually exclusive", NULL);
	    } else
		WXS_ELEM_TYPEDEF(decl) = xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    child = child->next;
	}
	while ((IS_SCHEMA(child, "unique")) ||
	    (IS_SCHEMA(child, "key")) || (IS_SCHEMA(child, "keyref"))) {
	    if (IS_SCHEMA(child, "unique")) {
		curIDC = xmlSchemaParseIDC(ctxt, schema, child,
		    XML_SCHEMA_TYPE_IDC_UNIQUE, decl->targetNamespace);
	    } else if (IS_SCHEMA(child, "key")) {
		curIDC = xmlSchemaParseIDC(ctxt, schema, child,
		    XML_SCHEMA_TYPE_IDC_KEY, decl->targetNamespace);
	    } else if (IS_SCHEMA(child, "keyref")) {
		curIDC = xmlSchemaParseIDC(ctxt, schema, child,
		    XML_SCHEMA_TYPE_IDC_KEYREF, decl->targetNamespace);
	    }
	    if (lastIDC != NULL)
		lastIDC->next = curIDC;
	    else
		decl->idcs = (void *) curIDC;
	    lastIDC = curIDC;
	    child = child->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child,
		NULL, "(annotation?, ((simpleType | complexType)?, "
		"(unique | key | keyref)*))");
	}
	decl->annot = annot;
    }
    FREE_AND_NULL(des)
    if (topLevel)
	return ((xmlSchemaBasicItemPtr) decl);
    else {
	particle->children = (xmlSchemaTreeItemPtr) decl;
	return ((xmlSchemaBasicItemPtr) particle);
    }

return_null:
    FREE_AND_NULL(des);
    if (annot != NULL) {
	if (particle != NULL)
	    particle->annot = NULL;
	if (decl != NULL)
	    decl->annot = NULL;
	xmlSchemaFreeAnnot(annot);
    }
    return (NULL);
}

static int
xmlSchemaParseUnion(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                    xmlNodePtr node)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *cur = NULL;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);
    
    type = ctxt->ctxtType;
    type->flags |= XML_SCHEMAS_TYPE_VARIETY_UNION;
    type->baseType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "memberTypes"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    attr = xmlSchemaGetPropNode(node, "memberTypes");
    if (attr != NULL) {
	const xmlChar *end;
	xmlChar *tmp;
	const xmlChar *localName, *nsName;
	xmlSchemaTypeLinkPtr link, lastLink = NULL;
	xmlSchemaQNameRefPtr ref;

	cur = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	type->base = cur;
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    tmp = xmlStrndup(cur, end - cur);
	    if (xmlSchemaPValAttrNodeQNameValue(ctxt, schema,
		NULL, attr, BAD_CAST tmp, &nsName, &localName) == 0) {
		link = (xmlSchemaTypeLinkPtr)
		    xmlMalloc(sizeof(xmlSchemaTypeLink));
		if (link == NULL) {
		    xmlSchemaPErrMemory(ctxt, "xmlSchemaParseUnion, "
			"allocating a type link", NULL);
		    return (-1);
		}
		link->type = NULL;
		link->next = NULL;
		if (lastLink == NULL)
		    type->memberTypes = link;
		else
		    lastLink->next = link;
		lastLink = link;
		ref = xmlSchemaNewQNameRef(ctxt, XML_SCHEMA_TYPE_SIMPLE,
		    localName, nsName);
		if (ref == NULL) {
		    FREE_AND_NULL(tmp)
		    return (-1);
		}
		link->type = (xmlSchemaTypePtr) ref;
	    }
	    FREE_AND_NULL(tmp)
	    cur = end;
	} while (*cur != 0);

    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
        child = child->next;
    }
    if (IS_SCHEMA(child, "simpleType")) {
	xmlSchemaTypePtr subtype, last = NULL;

	while (IS_SCHEMA(child, "simpleType")) {
	    subtype = (xmlSchemaTypePtr)
		xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    if (subtype != NULL) {
		if (last == NULL) {
		    type->subtypes = subtype;
		    last = subtype;
		} else {
		    last->next = subtype;
		    last = subtype;
		}
		last->next = NULL;
	    }
	    child = child->next;
	}
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL, "(annotation?, simpleType*)");
    }
    if ((attr == NULL) && (type->subtypes == NULL)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_UNION_MEMBERTYPES_OR_SIMPLETYPES,
	    NULL, node,
	    "Either the attribute 'memberTypes' or "
	    "at least one <simpleType> child must be present", NULL);
    }
    return (0);
}

static xmlSchemaTypePtr
xmlSchemaParseList(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                   xmlNodePtr node)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    
    type = ctxt->ctxtType;
    type->flags |= XML_SCHEMAS_TYPE_VARIETY_LIST;
    type->baseType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "itemType"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");

    xmlSchemaPValAttrQName(ctxt, schema, NULL,
	node, "itemType", &(type->baseNs), &(type->base));
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
        child = child->next;
    }
    if (IS_SCHEMA(child, "simpleType")) {
	if (type->base != NULL) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
		NULL, node,
		"The attribute 'itemType' and the <simpleType> child "
		"are mutually exclusive", NULL);
	} else {
	    type->subtypes = xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	}
        child = child->next;
    } else if (type->base == NULL) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
	    NULL, node,
	    "Either the attribute 'itemType' or the <simpleType> child "
	    "must be present", NULL);
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL, "(annotation?, simpleType?)");
    }
    if ((type->base == NULL) &&
	(type->subtypes == NULL) &&
	(xmlSchemaGetPropNode(node, "itemType") == NULL)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_SRC_SIMPLE_TYPE_1,
	    NULL, node,
	    "Either the attribute 'itemType' or the <simpleType> child "
	    "must be present", NULL);
    }
    return (NULL);
}

static xmlSchemaTypePtr
xmlSchemaParseSimpleType(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                         xmlNodePtr node, int topLevel)
{
    xmlSchemaTypePtr type, oldCtxtType;
    xmlNodePtr child = NULL;
    const xmlChar *attrValue = NULL;
    xmlAttrPtr attr;
    int hasRestriction = 0;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    if (topLevel) {
	attr = xmlSchemaGetPropNode(node, "name");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, node,
		"name", NULL);
	    return (NULL);
	} else {
	    if (xmlSchemaPValAttrNode(ctxt,
		NULL, attr,
		xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &attrValue) != 0)
		return (NULL);
	    if (ctxt->isS4S) {
		xmlSchemaTypePtr biType;

		if (ctxt->isRedefine) {
		    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_SRC_REDEFINE,
			NULL, node,
			"Redefinition of built-in simple types is not "
			"supported", NULL);
		    return(NULL);
		}
		biType = xmlSchemaGetPredefinedType(attrValue, xmlSchemaNs);
		if (biType != NULL)
		    return (biType);
	    }
	}
    }
    if (topLevel == 0) {
#ifdef ENABLE_NAMED_LOCALS
        char buf[40];
#endif
#ifdef ENABLE_NAMED_LOCALS
        snprintf(buf, 39, "#ST%d", ctxt->counter++ + 1);
	type = xmlSchemaAddType(ctxt, schema,
	    XML_SCHEMA_TYPE_SIMPLE,
	    xmlDictLookup(ctxt->dict, (const xmlChar *)buf, -1),
	    ctxt->targetNamespace, node, 0);
#else
	type = xmlSchemaAddType(ctxt, schema,
	    XML_SCHEMA_TYPE_SIMPLE,
	    NULL, ctxt->targetNamespace, node, 0);
#endif
	if (type == NULL)
	    return (NULL);
	type->type = XML_SCHEMA_TYPE_SIMPLE;
	type->contentType = XML_SCHEMA_CONTENT_SIMPLE;
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (!xmlStrEqual(attr->name, BAD_CAST "id")) {
		    xmlSchemaPIllegalAttrErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		    xmlSchemaPIllegalAttrErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	    attr = attr->next;
	}
    } else {
	type = xmlSchemaAddType(ctxt, schema, XML_SCHEMA_TYPE_SIMPLE,
	    attrValue, ctxt->targetNamespace, node, 1);
	if (type == NULL)
	    return (NULL);
	type->type = XML_SCHEMA_TYPE_SIMPLE;
	type->contentType = XML_SCHEMA_CONTENT_SIMPLE;
	type->flags |= XML_SCHEMAS_TYPE_GLOBAL;
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "final"))) {
		    xmlSchemaPIllegalAttrErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	    attr = attr->next;
	}
	attr = xmlSchemaGetPropNode(node, "final");
	if (attr == NULL) {
	    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION)
		type->flags |= XML_SCHEMAS_TYPE_FINAL_RESTRICTION;
	    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_LIST)
		type->flags |= XML_SCHEMAS_TYPE_FINAL_LIST;
	    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_UNION)
		type->flags |= XML_SCHEMAS_TYPE_FINAL_UNION;
	} else {
	    attrValue = xmlSchemaGetProp(ctxt, node, "final");
	    if (xmlSchemaPValAttrBlockFinal(attrValue, &(type->flags),
		-1, -1, XML_SCHEMAS_TYPE_FINAL_RESTRICTION, -1,
		XML_SCHEMAS_TYPE_FINAL_LIST,
		XML_SCHEMAS_TYPE_FINAL_UNION) != 0) {

		xmlSchemaPSimpleTypeErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		    WXS_BASIC_CAST type, (xmlNodePtr) attr,
		    NULL, "(#all | List of (list | union | restriction)",
		    attrValue, NULL, NULL, NULL);
	    }
	}
    }
    type->targetNamespace = ctxt->targetNamespace;
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    oldCtxtType = ctxt->ctxtType;

    ctxt->ctxtType = type;

    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        type->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_MISSING,
	    NULL, node, child, NULL,
	    "(annotation?, (restriction | list | union))");
    } else if (IS_SCHEMA(child, "restriction")) {
        xmlSchemaParseRestriction(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SIMPLE);
	hasRestriction = 1;
        child = child->next;
    } else if (IS_SCHEMA(child, "list")) {
        xmlSchemaParseList(ctxt, schema, child);
        child = child->next;
    } else if (IS_SCHEMA(child, "union")) {
        xmlSchemaParseUnion(ctxt, schema, child);
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt, XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?, (restriction | list | union))");
    }
    if (topLevel && ctxt->isRedefine && (! hasRestriction)) {
	xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_SRC_REDEFINE,
	    NULL, node, "This is a redefinition, thus the "
	    "<simpleType> must have a <restriction> child", NULL);
    }

    ctxt->ctxtType = oldCtxtType;
    return (type);
}

static xmlSchemaTreeItemPtr
xmlSchemaParseModelGroupDefRef(xmlSchemaParserCtxtPtr ctxt,
			       xmlSchemaPtr schema,
			       xmlNodePtr node)
{
    xmlSchemaParticlePtr item;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *ref = NULL, *refNs = NULL;
    int min, max;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    attr = xmlSchemaGetPropNode(node, "ref");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node, "ref", NULL);
	return (NULL);
    } else if (xmlSchemaPValAttrNodeQName(ctxt, schema, NULL,
	attr, &refNs, &ref) != 0) {
	return (NULL);
    }
    xmlSchemaCheckReference(ctxt, schema, node, attr, refNs);
    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, "xs:nonNegativeInteger");
    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1,
	"(xs:nonNegativeInteger | unbounded)");
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "ref")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "minOccurs")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "maxOccurs"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    item = xmlSchemaAddParticle(ctxt, node, min, max);
    if (item == NULL)
	return (NULL);
    item->children = (xmlSchemaTreeItemPtr)
	xmlSchemaNewQNameRef(ctxt, XML_SCHEMA_TYPE_GROUP, ref, refNs);
    xmlSchemaPCheckParticleCorrect_2(ctxt, item, node, min, max);
    child = node->children;
    
    if (IS_SCHEMA(child, "annotation")) {
	item->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
	child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?)");
    }
    if ((min == 0) && (max == 0))
	return (NULL);

    return ((xmlSchemaTreeItemPtr) item);
}

static xmlSchemaModelGroupDefPtr
xmlSchemaParseModelGroupDefinition(xmlSchemaParserCtxtPtr ctxt,
				   xmlSchemaPtr schema,
				   xmlNodePtr node)
{
    xmlSchemaModelGroupDefPtr item;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    const xmlChar *name;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    attr = xmlSchemaGetPropNode(node, "name");
    if (attr == NULL) {
	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node,
	    "name", NULL);
	return (NULL);
    } else if (xmlSchemaPValAttrNode(ctxt, NULL, attr,
	xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	return (NULL);
    }
    item = xmlSchemaAddModelGroupDefinition(ctxt, schema, name,
	ctxt->targetNamespace, node);
    if (item == NULL)
	return (NULL);
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "name")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "id"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	item->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
	child = child->next;
    }
    if (IS_SCHEMA(child, "all")) {
	item->children = xmlSchemaParseModelGroup(ctxt, schema, child,
	    XML_SCHEMA_TYPE_ALL, 0);
	child = child->next;
    } else if (IS_SCHEMA(child, "choice")) {
	item->children = xmlSchemaParseModelGroup(ctxt, schema, child,
	    XML_SCHEMA_TYPE_CHOICE, 0);
	child = child->next;
    } else if (IS_SCHEMA(child, "sequence")) {
	item->children = xmlSchemaParseModelGroup(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SEQUENCE, 0);
	child = child->next;
    }



    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?, (all | choice | sequence)?)");
    }
    return (item);
}

static void
xmlSchemaCleanupDoc(xmlSchemaParserCtxtPtr ctxt, xmlNodePtr root)
{
    xmlNodePtr delete, cur;

    if ((ctxt == NULL) || (root == NULL)) return;

    delete = NULL;
    cur = root;
    while (cur != NULL) {
        if (delete != NULL) {
            xmlUnlinkNode(delete);
            xmlFreeNode(delete);
            delete = NULL;
        }
        if (cur->type == XML_TEXT_NODE) {
            if (IS_BLANK_NODE(cur)) {
                if (xmlNodeGetSpacePreserve(cur) != 1) {
                    delete = cur;
                }
            }
        } else if ((cur->type != XML_ELEMENT_NODE) &&
                   (cur->type != XML_CDATA_SECTION_NODE)) {
            delete = cur;
            goto skip_children;
        }

        if (cur->children != NULL) {
            if ((cur->children->type != XML_ENTITY_DECL) &&
                (cur->children->type != XML_ENTITY_REF_NODE) &&
                (cur->children->type != XML_ENTITY_NODE)) {
                cur = cur->children;
                continue;
            }
        }
      skip_children:
        if (cur->next != NULL) {
            cur = cur->next;
            continue;
        }

        do {
            cur = cur->parent;
            if (cur == NULL)
                break;
            if (cur == root) {
                cur = NULL;
                break;
            }
            if (cur->next != NULL) {
                cur = cur->next;
                break;
            }
        } while (cur != NULL);
    }
    if (delete != NULL) {
        xmlUnlinkNode(delete);
        xmlFreeNode(delete);
        delete = NULL;
    }
}


static void
xmlSchemaClearSchemaDefaults(xmlSchemaPtr schema)
{
    if (schema->flags & XML_SCHEMAS_QUALIF_ELEM)
	schema->flags ^= XML_SCHEMAS_QUALIF_ELEM;

    if (schema->flags & XML_SCHEMAS_QUALIF_ATTR)
	schema->flags ^= XML_SCHEMAS_QUALIF_ATTR;

    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_EXTENSION)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_EXTENSION;
    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION;
    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_LIST)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_LIST;
    if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_UNION)
	schema->flags ^= XML_SCHEMAS_FINAL_DEFAULT_UNION;

    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION)
	schema->flags ^= XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION;
    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION)
	schema->flags ^= XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION;
    if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION)
	schema->flags ^= XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION;
}

static int
xmlSchemaParseSchemaElement(xmlSchemaParserCtxtPtr ctxt,
			     xmlSchemaPtr schema,
			     xmlNodePtr node)
{
    xmlAttrPtr attr;
    const xmlChar *val;
    int res = 0, oldErrs = ctxt->nberrors;

    res = xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    HFAILURE;

    attr = xmlSchemaGetPropNode(node, "targetNamespace");
    if (attr != NULL) {
	res = xmlSchemaPValAttrNode(ctxt, NULL, attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI), NULL);
	HFAILURE;
	if (res != 0) {
	    ctxt->stop = XML_SCHEMAP_S4S_ATTR_INVALID_VALUE;
	    goto exit;
	}
    }
    attr = xmlSchemaGetPropNode(node, "elementFormDefault");
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	res = xmlSchemaPValAttrFormDefault(val, &schema->flags,
	    XML_SCHEMAS_QUALIF_ELEM);
	HFAILURE;
	if (res != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_ELEMFORMDEFAULT_VALUE,
		NULL, (xmlNodePtr) attr, NULL,
		"(qualified | unqualified)", val, NULL, NULL, NULL);
	}
    }
    attr = xmlSchemaGetPropNode(node, "attributeFormDefault");
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	res = xmlSchemaPValAttrFormDefault(val, &schema->flags,
	    XML_SCHEMAS_QUALIF_ATTR);
	HFAILURE;
	if (res != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_ATTRFORMDEFAULT_VALUE,
		NULL, (xmlNodePtr) attr, NULL,
		"(qualified | unqualified)", val, NULL, NULL, NULL);
	}
    }
    attr = xmlSchemaGetPropNode(node, "finalDefault");
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	res = xmlSchemaPValAttrBlockFinal(val, &(schema->flags), -1,
	    XML_SCHEMAS_FINAL_DEFAULT_EXTENSION,
	    XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION,
	    -1,
	    XML_SCHEMAS_FINAL_DEFAULT_LIST,
	    XML_SCHEMAS_FINAL_DEFAULT_UNION);
	HFAILURE;
	if (res != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		NULL, (xmlNodePtr) attr, NULL,
		"(#all | List of (extension | restriction | list | union))",
		val, NULL, NULL, NULL);
	}
    }
    attr = xmlSchemaGetPropNode(node, "blockDefault");
    if (attr != NULL) {
	val = xmlSchemaGetNodeContent(ctxt, (xmlNodePtr) attr);
	res = xmlSchemaPValAttrBlockFinal(val, &(schema->flags), -1,
	    XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION,
	    XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION,
	    XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION, -1, -1);
	HFAILURE;
	if (res != 0) {
	    xmlSchemaPSimpleTypeErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
		NULL, (xmlNodePtr) attr, NULL,
		"(#all | List of (extension | restriction | substitution))",
		val, NULL, NULL, NULL);
	}
    }

exit:
    if (oldErrs != ctxt->nberrors)
	res = ctxt->err;
    return(res);
exit_failure:
    return(-1);
}

static int
xmlSchemaParseSchemaTopLevel(xmlSchemaParserCtxtPtr ctxt,
                             xmlSchemaPtr schema, xmlNodePtr nodes)
{
    xmlNodePtr child;
    xmlSchemaAnnotPtr annot;
    int res = 0, oldErrs, tmpOldErrs;

    if ((ctxt == NULL) || (schema == NULL) || (nodes == NULL))
        return(-1);

    oldErrs = ctxt->nberrors;
    child = nodes;
    while ((IS_SCHEMA(child, "include")) ||
	   (IS_SCHEMA(child, "import")) ||
	   (IS_SCHEMA(child, "redefine")) ||
	   (IS_SCHEMA(child, "annotation"))) {
	if (IS_SCHEMA(child, "annotation")) {
	    annot = xmlSchemaParseAnnotation(ctxt, child, 1);
	    if (schema->annot == NULL)
		schema->annot = annot;
	    else
		xmlSchemaFreeAnnot(annot);
	} else if (IS_SCHEMA(child, "import")) {
	    tmpOldErrs = ctxt->nberrors;
	    res = xmlSchemaParseImport(ctxt, schema, child);
	    HFAILURE;
	    HSTOP(ctxt);
	    if (tmpOldErrs != ctxt->nberrors)
		goto exit;
	} else if (IS_SCHEMA(child, "include")) {
	    tmpOldErrs = ctxt->nberrors;
	    res = xmlSchemaParseInclude(ctxt, schema, child);
	    HFAILURE;
	    HSTOP(ctxt);
	    if (tmpOldErrs != ctxt->nberrors)
		goto exit;
	} else if (IS_SCHEMA(child, "redefine")) {
	    tmpOldErrs = ctxt->nberrors;
	    res = xmlSchemaParseRedefine(ctxt, schema, child);
	    HFAILURE;
	    HSTOP(ctxt);
	    if (tmpOldErrs != ctxt->nberrors)
		goto exit;
	}
	child = child->next;
    }
    while (child != NULL) {
	if (IS_SCHEMA(child, "complexType")) {
	    xmlSchemaParseComplexType(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "simpleType")) {
	    xmlSchemaParseSimpleType(ctxt, schema, child, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "element")) {
	    xmlSchemaParseElement(ctxt, schema, child, NULL, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "attribute")) {
	    xmlSchemaParseGlobalAttribute(ctxt, schema, child);
	    child = child->next;
	} else if (IS_SCHEMA(child, "attributeGroup")) {
	    xmlSchemaParseAttributeGroupDefinition(ctxt, schema, child);
	    child = child->next;
	} else if (IS_SCHEMA(child, "group")) {
	    xmlSchemaParseModelGroupDefinition(ctxt, schema, child);
	    child = child->next;
	} else if (IS_SCHEMA(child, "notation")) {
	    xmlSchemaParseNotation(ctxt, schema, child);
	    child = child->next;
	} else {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, child->parent, child,
		NULL, "((include | import | redefine | annotation)*, "
		"(((simpleType | complexType | group | attributeGroup) "
		"| element | attribute | notation), annotation*)*)");
	    child = child->next;
	}
	while (IS_SCHEMA(child, "annotation")) {
	    annot = xmlSchemaParseAnnotation(ctxt, child, 1);
	    if (schema->annot == NULL)
		schema->annot = annot;
	    else
		xmlSchemaFreeAnnot(annot);
	    child = child->next;
	}
    }
exit:
    ctxt->ctxtType = NULL;
    if (oldErrs != ctxt->nberrors)
	res = ctxt->err;
    return(res);
exit_failure:
    return(-1);
}

static xmlSchemaSchemaRelationPtr
xmlSchemaSchemaRelationCreate(void)
{
    xmlSchemaSchemaRelationPtr ret;

    ret = (xmlSchemaSchemaRelationPtr)
	xmlMalloc(sizeof(xmlSchemaSchemaRelation));
    if (ret == NULL) {
	xmlSchemaPErrMemory(NULL, "allocating schema relation", NULL);
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaSchemaRelation));
    return(ret);
}

#if 0
static void
xmlSchemaSchemaRelationFree(xmlSchemaSchemaRelationPtr rel)
{
    xmlFree(rel);
}
#endif

static void
xmlSchemaRedefListFree(xmlSchemaRedefPtr redef)
{
    xmlSchemaRedefPtr prev;

    while (redef != NULL) {
	prev = redef;
	redef = redef->next;
	xmlFree(prev);
    }
}

static void
xmlSchemaConstructionCtxtFree(xmlSchemaConstructionCtxtPtr con)
{
    if (con->buckets != NULL)
	xmlSchemaItemListFree(con->buckets);
    if (con->pending != NULL)
	xmlSchemaItemListFree(con->pending);
    if (con->substGroups != NULL)
	xmlHashFree(con->substGroups,
	    (xmlHashDeallocator) xmlSchemaSubstGroupFree);
    if (con->redefs != NULL)
	xmlSchemaRedefListFree(con->redefs);
    if (con->dict != NULL)
	xmlDictFree(con->dict);
    xmlFree(con);
}

static xmlSchemaConstructionCtxtPtr
xmlSchemaConstructionCtxtCreate(xmlDictPtr dict)
{
    xmlSchemaConstructionCtxtPtr ret;

    ret = (xmlSchemaConstructionCtxtPtr)
	xmlMalloc(sizeof(xmlSchemaConstructionCtxt));
    if (ret == NULL) {
        xmlSchemaPErrMemory(NULL,
	    "allocating schema construction context", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaConstructionCtxt));

    ret->buckets = xmlSchemaItemListCreate();
    if (ret->buckets == NULL) {
	xmlSchemaPErrMemory(NULL,
	    "allocating list of schema buckets", NULL);
	xmlFree(ret);
        return (NULL);
    }
    ret->pending = xmlSchemaItemListCreate();
    if (ret->pending == NULL) {
	xmlSchemaPErrMemory(NULL,
	    "allocating list of pending global components", NULL);
	xmlSchemaConstructionCtxtFree(ret);
        return (NULL);
    }
    ret->dict = dict;
    xmlDictReference(dict);
    return(ret);
}

static xmlSchemaParserCtxtPtr
xmlSchemaParserCtxtCreate(void)
{
    xmlSchemaParserCtxtPtr ret;

    ret = (xmlSchemaParserCtxtPtr) xmlMalloc(sizeof(xmlSchemaParserCtxt));
    if (ret == NULL) {
        xmlSchemaPErrMemory(NULL, "allocating schema parser context",
                            NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaParserCtxt));
    ret->type = XML_SCHEMA_CTXT_PARSER;
    ret->attrProhibs = xmlSchemaItemListCreate();
    if (ret->attrProhibs == NULL) {
	xmlFree(ret);
	return(NULL);
    }
    return(ret);
}

static xmlSchemaParserCtxtPtr
xmlSchemaNewParserCtxtUseDict(const char *URL, xmlDictPtr dict)
{
    xmlSchemaParserCtxtPtr ret;

    ret = xmlSchemaParserCtxtCreate();
    if (ret == NULL)
        return (NULL);
    ret->dict = dict;
    xmlDictReference(dict);
    if (URL != NULL)
	ret->URL = xmlDictLookup(dict, (const xmlChar *) URL, -1);
    return (ret);
}

static int
xmlSchemaCreatePCtxtOnVCtxt(xmlSchemaValidCtxtPtr vctxt)
{
    if (vctxt->pctxt == NULL) {
        if (vctxt->schema != NULL)
	    vctxt->pctxt =
		xmlSchemaNewParserCtxtUseDict("*", vctxt->schema->dict);
	else
	    vctxt->pctxt = xmlSchemaNewParserCtxt("*");
	if (vctxt->pctxt == NULL) {
	    VERROR_INT("xmlSchemaCreatePCtxtOnVCtxt",
		"failed to create a temp. parser context");
	    return (-1);
	}
	
	xmlSchemaSetParserErrors(vctxt->pctxt, vctxt->error,
	    vctxt->warning, vctxt->errCtxt);
	xmlSchemaSetParserStructuredErrors(vctxt->pctxt, vctxt->serror,
	    vctxt->errCtxt);
    }
    return (0);
}

static xmlSchemaBucketPtr
xmlSchemaGetSchemaBucket(xmlSchemaParserCtxtPtr pctxt,
			    const xmlChar *schemaLocation)
{
    xmlSchemaBucketPtr cur;
    xmlSchemaItemListPtr list;

    list = pctxt->constructor->buckets;
    if (list->nbItems == 0)
	return(NULL);
    else {
	int i;
	for (i = 0; i < list->nbItems; i++) {
	    cur = (xmlSchemaBucketPtr) list->items[i];
	    
	    if (cur->schemaLocation == schemaLocation)
		return(cur);
	}
    }
    return(NULL);
}

static xmlSchemaBucketPtr
xmlSchemaGetChameleonSchemaBucket(xmlSchemaParserCtxtPtr pctxt,
				     const xmlChar *schemaLocation,
				     const xmlChar *targetNamespace)
{
    xmlSchemaBucketPtr cur;
    xmlSchemaItemListPtr list;

    list = pctxt->constructor->buckets;
    if (list->nbItems == 0)
	return(NULL);
    else {
	int i;
	for (i = 0; i < list->nbItems; i++) {
	    cur = (xmlSchemaBucketPtr) list->items[i];
	    
	    if ((cur->origTargetNamespace == NULL) &&
		(cur->schemaLocation == schemaLocation) &&
		(cur->targetNamespace == targetNamespace))
		return(cur);
	}
    }
    return(NULL);
}


#define IS_BAD_SCHEMA_DOC(b) \
    (((b)->doc == NULL) && ((b)->schemaLocation != NULL))

static xmlSchemaBucketPtr
xmlSchemaGetSchemaBucketByTNS(xmlSchemaParserCtxtPtr pctxt,
				 const xmlChar *targetNamespace,
				 int imported)
{
    xmlSchemaBucketPtr cur;
    xmlSchemaItemListPtr list;

    list = pctxt->constructor->buckets;
    if (list->nbItems == 0)
	return(NULL);
    else {
	int i;
	for (i = 0; i < list->nbItems; i++) {
	    cur = (xmlSchemaBucketPtr) list->items[i];
	    if ((! IS_BAD_SCHEMA_DOC(cur)) &&
		(cur->origTargetNamespace == targetNamespace) &&
		((imported && cur->imported) ||
		 ((!imported) && (!cur->imported))))
		return(cur);
	}
    }
    return(NULL);
}

static int
xmlSchemaParseNewDocWithContext(xmlSchemaParserCtxtPtr pctxt,
		     xmlSchemaPtr schema,
		     xmlSchemaBucketPtr bucket)
{
    int oldFlags;
    xmlDocPtr oldDoc;
    xmlNodePtr node;
    int ret, oldErrs;
    xmlSchemaBucketPtr oldbucket = pctxt->constructor->bucket;

    oldFlags = schema->flags;
    oldDoc = schema->doc;
    if (schema->flags != 0)
	xmlSchemaClearSchemaDefaults(schema);
    schema->doc = bucket->doc;
    pctxt->schema = schema;
    pctxt->targetNamespace = bucket->targetNamespace;
    WXS_CONSTRUCTOR(pctxt)->bucket = bucket;

    if ((bucket->targetNamespace != NULL) &&
	xmlStrEqual(bucket->targetNamespace, xmlSchemaNs)) {
	pctxt->isS4S = 1;
    }
    
    bucket->parsed++;
    
    node = xmlDocGetRootElement(bucket->doc);
    ret = xmlSchemaParseSchemaElement(pctxt, schema, node);
    if (ret != 0)
	goto exit;
    
    if (node->children == NULL)
	goto exit;
    oldErrs = pctxt->nberrors;
    ret = xmlSchemaParseSchemaTopLevel(pctxt, schema, node->children);
    if (ret != 0)
	goto exit;
    if ((ret == 0) && (oldErrs != pctxt->nberrors)) {
	ret = pctxt->err;
	goto exit;
    }

exit:
    WXS_CONSTRUCTOR(pctxt)->bucket = oldbucket;
    
    schema->doc = oldDoc;
    schema->flags = oldFlags;
    return(ret);
}

static int
xmlSchemaParseNewDoc(xmlSchemaParserCtxtPtr pctxt,
		     xmlSchemaPtr schema,
		     xmlSchemaBucketPtr bucket)
{
    xmlSchemaParserCtxtPtr newpctxt;
    int res = 0;

    if (bucket == NULL)
	return(0);
    if (bucket->parsed) {
	PERROR_INT("xmlSchemaParseNewDoc",
	    "reparsing a schema doc");
	return(-1);
    }
    if (bucket->doc == NULL) {
	PERROR_INT("xmlSchemaParseNewDoc",
	    "parsing a schema doc, but there's no doc");
	return(-1);
    }
    if (pctxt->constructor == NULL) {
	PERROR_INT("xmlSchemaParseNewDoc",
	    "no constructor");
	return(-1);
    }
    
    newpctxt = xmlSchemaNewParserCtxtUseDict(
	(const char *) bucket->schemaLocation, pctxt->dict);
    if (newpctxt == NULL)
	return(-1);
    newpctxt->constructor = pctxt->constructor;
    newpctxt->schema = schema;
    xmlSchemaSetParserErrors(newpctxt, pctxt->error, pctxt->warning,
	pctxt->errCtxt);
    xmlSchemaSetParserStructuredErrors(newpctxt, pctxt->serror,
	pctxt->errCtxt);
    newpctxt->counter = pctxt->counter;


    res = xmlSchemaParseNewDocWithContext(newpctxt, schema, bucket);

    
    if (res != 0)
	pctxt->err = res;
    pctxt->nberrors += newpctxt->nberrors;
    pctxt->counter = newpctxt->counter;
    newpctxt->constructor = NULL;
    
    xmlSchemaFreeParserCtxt(newpctxt);
    return(res);
}

static void
xmlSchemaSchemaRelationAddChild(xmlSchemaBucketPtr bucket,
				xmlSchemaSchemaRelationPtr rel)
{
    xmlSchemaSchemaRelationPtr cur = bucket->relations;

    if (cur == NULL) {
	bucket->relations = rel;
	return;
    }
    while (cur->next != NULL)
	cur = cur->next;
    cur->next = rel;
}


static const xmlChar *
xmlSchemaBuildAbsoluteURI(xmlDictPtr dict, const xmlChar* location,
			  xmlNodePtr ctxtNode)
{
    if (location != NULL) {
	if (ctxtNode == NULL)
	    return(location);
	else {
	    xmlChar *base, *URI;
	    const xmlChar *ret = NULL;

	    base = xmlNodeGetBase(ctxtNode->doc, ctxtNode);
	    if (base == NULL) {
		URI = xmlBuildURI(location, ctxtNode->doc->URL);
	    } else {
		URI = xmlBuildURI(location, base);
		xmlFree(base);
	    }
	    if (URI != NULL) {
		ret = xmlDictLookup(dict, URI, -1);
		xmlFree(URI);
		return(ret);
	    }
	}
    }
    return(NULL);
}




static int
xmlSchemaAddSchemaDoc(xmlSchemaParserCtxtPtr pctxt,
		int type, 
		const xmlChar *schemaLocation,
		xmlDocPtr schemaDoc,
		const char *schemaBuffer,
		int schemaBufferLen,
		xmlNodePtr invokingNode,
		const xmlChar *sourceTargetNamespace,
		const xmlChar *importNamespace,
		xmlSchemaBucketPtr *bucket)
{
    const xmlChar *targetNamespace = NULL;
    xmlSchemaSchemaRelationPtr relation = NULL;
    xmlDocPtr doc = NULL;
    int res = 0, err = 0, located = 0, preserveDoc = 0;
    xmlSchemaBucketPtr bkt = NULL;

    if (bucket != NULL)
	*bucket = NULL;

    switch (type) {
	case XML_SCHEMA_SCHEMA_IMPORT:
	case XML_SCHEMA_SCHEMA_MAIN:
	    err = XML_SCHEMAP_SRC_IMPORT;
	    break;
	case XML_SCHEMA_SCHEMA_INCLUDE:
	    err = XML_SCHEMAP_SRC_INCLUDE;
	    break;
	case XML_SCHEMA_SCHEMA_REDEFINE:
	    err = XML_SCHEMAP_SRC_REDEFINE;
	    break;
    }


    if ((type == XML_SCHEMA_SCHEMA_MAIN) || (! WXS_HAS_BUCKETS(pctxt)))
	goto doc_load;

    
    if (schemaLocation != NULL) {
	bkt = xmlSchemaGetSchemaBucket(pctxt, schemaLocation);
	if ((bkt != NULL) &&
	    (pctxt->constructor->bucket == bkt)) {
	    

	    xmlSchemaCustomErr(ACTXT_CAST pctxt, err,
		invokingNode, NULL,
		"The schema must not import/include/redefine itself",
		NULL, NULL);
	    goto exit;
	}
    }
    relation = xmlSchemaSchemaRelationCreate();
    if (relation == NULL)
	return(-1);
    xmlSchemaSchemaRelationAddChild(pctxt->constructor->bucket,
	relation);
    relation->type = type;

    if (WXS_IS_BUCKET_IMPMAIN(type)) {
	relation->importNamespace = importNamespace;
	if (schemaLocation == NULL) {
	    goto exit;
	}
	targetNamespace = importNamespace;
    }

    
    if (bkt != NULL) {
	if ((WXS_IS_BUCKET_IMPMAIN(type)) && (! bkt->imported)) {
	    if (schemaLocation == NULL)
		schemaLocation = BAD_CAST "in_memory_buffer";
	    if (!xmlStrEqual(schemaLocation,
		bkt->schemaLocation)) {
		xmlSchemaCustomErr(ACTXT_CAST pctxt, err,
		    invokingNode, NULL,
		    "The schema document '%s' cannot be imported, since "
		    "it was already included or redefined",
		    schemaLocation, NULL);
		goto exit;
	    }
	} else if ((! WXS_IS_BUCKET_IMPMAIN(type)) && (bkt->imported)) {
	    if (schemaLocation == NULL)
		schemaLocation = BAD_CAST "in_memory_buffer";
	    if (!xmlStrEqual(schemaLocation,
		bkt->schemaLocation)) {
		xmlSchemaCustomErr(ACTXT_CAST pctxt, err,
		    invokingNode, NULL,
		    "The schema document '%s' cannot be included or "
		    "redefined, since it was already imported",
		    schemaLocation, NULL);
		goto exit;
	    }
	}
    }

    if (WXS_IS_BUCKET_IMPMAIN(type)) {
	if (bkt != NULL) {
	    relation->bucket = bkt;
	    goto exit;
	}
	bkt = xmlSchemaGetSchemaBucketByTNS(pctxt,
	    importNamespace, 1);

	if (bkt != NULL) {
	    relation->bucket = bkt;
	    if (bkt->schemaLocation == NULL) {
		
		bkt->schemaLocation = schemaLocation;
	    } else {
		if (!xmlStrEqual(schemaLocation,
		    bkt->schemaLocation)) {
		    if (schemaLocation == NULL)
			schemaLocation = BAD_CAST "in_memory_buffer";

		    xmlSchemaCustomWarning(ACTXT_CAST pctxt,
			XML_SCHEMAP_WARN_SKIP_SCHEMA,
			invokingNode, NULL,
			"Skipping import of schema located at '%s' for the "
			"namespace '%s', since this namespace was already "
			"imported with the schema located at '%s'",
			schemaLocation, importNamespace, bkt->schemaLocation);
		}
		goto exit;
	    }
	}
    } else {
	
	if (bkt != NULL) {

	    if ((bkt->origTargetNamespace == NULL) &&
		(bkt->targetNamespace != sourceTargetNamespace)) {
		xmlSchemaBucketPtr chamel;

		chamel = xmlSchemaGetChameleonSchemaBucket(pctxt,
		    schemaLocation, sourceTargetNamespace);
		if (chamel != NULL) {
		    
		    relation->bucket = chamel;
		    goto exit;
		}
		bkt = NULL;
	    } else {
		relation->bucket = bkt;
		goto exit;
	    }
	}
    }
    if ((bkt != NULL) && (bkt->doc != NULL)) {
	PERROR_INT("xmlSchemaAddSchemaDoc",
	    "trying to load a schema doc, but a doc is already "
	    "assigned to the schema bucket");
	goto exit_failure;
    }

doc_load:
    if (schemaDoc != NULL) {
	doc = schemaDoc;
	
	preserveDoc = 1;
	
	if (schemaDoc->URL != NULL)
	    schemaLocation = xmlDictLookup(pctxt->dict,
		schemaDoc->URL, -1);
        else
	    schemaLocation = BAD_CAST "in_memory_buffer";
    } else if ((schemaLocation != NULL) || (schemaBuffer != NULL)) {
	xmlParserCtxtPtr parserCtxt;

	parserCtxt = xmlNewParserCtxt();
	if (parserCtxt == NULL) {
	    xmlSchemaPErrMemory(NULL, "xmlSchemaGetDoc, "
		"allocating a parser context", NULL);
	    goto exit_failure;
	}
	if ((pctxt->dict != NULL) && (parserCtxt->dict != NULL)) {
	    xmlDictFree(parserCtxt->dict);
	    parserCtxt->dict = pctxt->dict;
	    xmlDictReference(parserCtxt->dict);
	}
	if (schemaLocation != NULL) {
	    
	    doc = xmlCtxtReadFile(parserCtxt, (const char *) schemaLocation,
		NULL, SCHEMAS_PARSE_OPTIONS);
	} else if (schemaBuffer != NULL) {
	    
	    doc = xmlCtxtReadMemory(parserCtxt, schemaBuffer, schemaBufferLen,
		NULL, NULL, SCHEMAS_PARSE_OPTIONS);
	    schemaLocation = BAD_CAST "in_memory_buffer";
	    if (doc != NULL)
		doc->URL = xmlStrdup(schemaLocation);
	}
	if (doc == NULL) {
	    xmlErrorPtr lerr;
	    lerr = xmlGetLastError();
	    if ((lerr == NULL) || (lerr->domain != XML_FROM_IO)) {
		located = 1;
		
		res = XML_SCHEMAP_SRC_IMPORT_2_1;
		xmlSchemaCustomErr(ACTXT_CAST pctxt, res,
		    invokingNode, NULL,
		    "Failed to parse the XML resource '%s'",
		    schemaLocation, NULL);
	    }
	}
	xmlFreeParserCtxt(parserCtxt);
	if ((doc == NULL) && located)
	    goto exit_error;
    } else {
	xmlSchemaPErr(pctxt, NULL,
	    XML_SCHEMAP_NOTHING_TO_PARSE,
	    "No information for parsing was provided with the "
	    "given schema parser context.\n",
	    NULL, NULL);
	goto exit_failure;
    }
    if (doc != NULL) {
	xmlNodePtr docElem = NULL;

	located = 1;
	docElem = xmlDocGetRootElement(doc);
	if (docElem == NULL) {
	    xmlSchemaCustomErr(ACTXT_CAST pctxt, XML_SCHEMAP_NOROOT,
		invokingNode, NULL,
		"The document '%s' has no document element",
		schemaLocation, NULL);
	    goto exit_error;
	}
	xmlSchemaCleanupDoc(pctxt, docElem);
	if (!IS_SCHEMA(docElem, "schema")) {
	    xmlSchemaCustomErr(ACTXT_CAST pctxt, XML_SCHEMAP_NOT_SCHEMA,
		invokingNode, NULL,
		"The XML document '%s' is not a schema document",
		schemaLocation, NULL);
	    goto exit_error;
	}
	targetNamespace = xmlSchemaGetProp(pctxt, docElem,
	    "targetNamespace");
    }

    if ((bkt == NULL) && located) {
	
        bkt = xmlSchemaBucketCreate(pctxt, type,
	    targetNamespace);
	if (bkt == NULL)
	    goto exit_failure;
    }
    if (bkt != NULL) {
	bkt->schemaLocation = schemaLocation;
	bkt->located = located;
	if (doc != NULL) {
	    bkt->doc = doc;
	    bkt->targetNamespace = targetNamespace;
	    bkt->origTargetNamespace = targetNamespace;
	    if (preserveDoc)
		bkt->preserveDoc = 1;
	}
	if (WXS_IS_BUCKET_IMPMAIN(type))
	    bkt->imported++;
	if (relation != NULL)
	    relation->bucket = bkt;
    }

exit:
    if (bucket != NULL)
	*bucket = bkt;
    return (0);

exit_error:
    if ((doc != NULL) && (! preserveDoc)) {
	xmlFreeDoc(doc);
	if (bkt != NULL)
	    bkt->doc = NULL;
    }
    return(pctxt->err);

exit_failure:
    if ((doc != NULL) && (! preserveDoc)) {
	xmlFreeDoc(doc);
	if (bkt != NULL)
	    bkt->doc = NULL;
    }
    return (-1);
}

static int
xmlSchemaParseImport(xmlSchemaParserCtxtPtr pctxt, xmlSchemaPtr schema,
                     xmlNodePtr node)
{
    xmlNodePtr child;
    const xmlChar *namespaceName = NULL, *schemaLocation = NULL;
    const xmlChar *thisTargetNamespace;
    xmlAttrPtr attr;
    int ret = 0;
    xmlSchemaBucketPtr bucket = NULL;

    if ((pctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "namespace")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "schemaLocation"))) {
		xmlSchemaPIllegalAttrErr(pctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(pctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    if (xmlSchemaPValAttr(pctxt, NULL, node,
	"namespace", xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI),
	&namespaceName) != 0) {
	xmlSchemaPSimpleTypeErr(pctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    NULL, node,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI),
	    NULL, namespaceName, NULL, NULL, NULL);
	return (pctxt->err);
    }

    if (xmlSchemaPValAttr(pctxt, NULL, node,
	"schemaLocation", xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI),
	&schemaLocation) != 0) {
	xmlSchemaPSimpleTypeErr(pctxt,
	    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
	    NULL, node,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI),
	    NULL, namespaceName, NULL, NULL, NULL);
	return (pctxt->err);
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(pctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?)");
    }
    thisTargetNamespace = WXS_BUCKET(pctxt)->origTargetNamespace;
    if (namespaceName != NULL) {
	if (xmlStrEqual(thisTargetNamespace, namespaceName)) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_SRC_IMPORT_1_1,
		NULL, node,
		"The value of the attribute 'namespace' must not match "
		"the target namespace '%s' of the importing schema",
		thisTargetNamespace);
	    return (pctxt->err);
	}
    } else {
	if (thisTargetNamespace == NULL) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_SRC_IMPORT_1_2,
		NULL, node,
		"The attribute 'namespace' must be existent if "
		"the importing schema has no target namespace",
		NULL);
	    return (pctxt->err);
	}
    }
    if (schemaLocation != NULL)
	schemaLocation = xmlSchemaBuildAbsoluteURI(pctxt->dict,
	    schemaLocation, node);
    ret = xmlSchemaAddSchemaDoc(pctxt, XML_SCHEMA_SCHEMA_IMPORT,
	schemaLocation, NULL, NULL, 0, node, thisTargetNamespace,
	namespaceName, &bucket);

    if (ret != 0)
	return(ret);

    if ((bucket == NULL) && (schemaLocation != NULL)) {
	xmlSchemaCustomWarning(ACTXT_CAST pctxt,
	    XML_SCHEMAP_WARN_UNLOCATED_SCHEMA,
	    node, NULL,
	    "Failed to locate a schema at location '%s'. "
	    "Skipping the import", schemaLocation, NULL, NULL);
    }

    if ((bucket != NULL) && CAN_PARSE_SCHEMA(bucket)) {
	ret = xmlSchemaParseNewDoc(pctxt, schema, bucket);
    }

    return (ret);
}

static int
xmlSchemaParseIncludeOrRedefineAttrs(xmlSchemaParserCtxtPtr pctxt,
				     xmlSchemaPtr schema,
				     xmlNodePtr node,
				     xmlChar **schemaLocation,
				     int type)
{
    xmlAttrPtr attr;

    if ((pctxt == NULL) || (schema == NULL) || (node == NULL) ||
	(schemaLocation == NULL))
        return (-1);

    *schemaLocation = NULL;
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "schemaLocation"))) {
		xmlSchemaPIllegalAttrErr(pctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(pctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(pctxt, node, BAD_CAST "id");
    attr = xmlSchemaGetPropNode(node, "schemaLocation");
    if (attr != NULL) {
        xmlChar *base = NULL;
        xmlChar *uri = NULL;

	if (xmlSchemaPValAttrNode(pctxt, NULL, attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYURI),
	    (const xmlChar **) schemaLocation) != 0)
	    goto exit_error;
	base = xmlNodeGetBase(node->doc, node);
	if (base == NULL) {
	    uri = xmlBuildURI(*schemaLocation, node->doc->URL);
	} else {
	    uri = xmlBuildURI(*schemaLocation, base);
	    xmlFree(base);
	}
	if (uri == NULL) {
	    PERROR_INT("xmlSchemaParseIncludeOrRedefine",
		"could not build an URI from the schemaLocation")
	    goto exit_failure;
	}
	(*schemaLocation) = (xmlChar *) xmlDictLookup(pctxt->dict, uri, -1);
	xmlFree(uri);
    } else {
	xmlSchemaPMissingAttrErr(pctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node, "schemaLocation", NULL);
	goto exit_error;
    }
    if (xmlStrEqual(*schemaLocation, pctxt->URL)) {
	if (type == XML_SCHEMA_SCHEMA_REDEFINE) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_SRC_REDEFINE,
		NULL, node,
		"The schema document '%s' cannot redefine itself.",
		*schemaLocation);
	} else {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_SRC_INCLUDE,
		NULL, node,
		"The schema document '%s' cannot include itself.",
		*schemaLocation);
	}
	goto exit_error;
    }

    return(0);
exit_error:
    return(pctxt->err);
exit_failure:
    return(-1);
}

static int
xmlSchemaParseIncludeOrRedefine(xmlSchemaParserCtxtPtr pctxt,
				xmlSchemaPtr schema,
				xmlNodePtr node,
				int type)
{
    xmlNodePtr child = NULL;
    const xmlChar *schemaLocation = NULL;
    int res = 0; 
    int isChameleon = 0, wasChameleon = 0;
    xmlSchemaBucketPtr bucket = NULL;

    if ((pctxt == NULL) || (schema == NULL) || (node == NULL))
        return (-1);

    res = xmlSchemaParseIncludeOrRedefineAttrs(pctxt, schema,
	node, (xmlChar **) (&schemaLocation), type);
    if (res != 0)
	return(res);
    res = xmlSchemaAddSchemaDoc(pctxt, type, schemaLocation, NULL,
	NULL, 0, node, pctxt->targetNamespace, NULL, &bucket);
    if (res != 0)
	return(res);
    if ((bucket == NULL) || (bucket->doc == NULL)) {
	if (type == XML_SCHEMA_SCHEMA_INCLUDE) {
	    res = XML_SCHEMAP_SRC_INCLUDE;
	    xmlSchemaCustomErr(ACTXT_CAST pctxt, res,
		node, NULL,
		"Failed to load the document '%s' for inclusion",
		schemaLocation, NULL);
	} else {
	    res = XML_SCHEMAP_SRC_REDEFINE;
	    xmlSchemaCustomErr(ACTXT_CAST pctxt, res,
		node, NULL,
		"Failed to load the document '%s' for redefinition",
		schemaLocation, NULL);
	}
    } else {
	if (bucket->origTargetNamespace != NULL) {
	    if (pctxt->targetNamespace == NULL) {
		xmlSchemaCustomErr(ACTXT_CAST pctxt,
		    XML_SCHEMAP_SRC_INCLUDE,
		    node, NULL,
		    "The target namespace of the included/redefined schema "
		    "'%s' has to be absent, since the including/redefining "
		    "schema has no target namespace",
		    schemaLocation, NULL);
		goto exit_error;
	    } else if (!xmlStrEqual(bucket->origTargetNamespace,
		pctxt->targetNamespace)) {
		
		xmlSchemaPCustomErrExt(pctxt,
		    XML_SCHEMAP_SRC_INCLUDE,
		    NULL, node,
		    "The target namespace '%s' of the included/redefined "
		    "schema '%s' differs from '%s' of the "
		    "including/redefining schema",
		    bucket->origTargetNamespace, schemaLocation,
		    pctxt->targetNamespace);
		goto exit_error;
	    }
	} else if (pctxt->targetNamespace != NULL) {
	    isChameleon = 1;
	    if (bucket->parsed &&
		bucket->origTargetNamespace != NULL) {
		xmlSchemaCustomErr(ACTXT_CAST pctxt,
		    XML_SCHEMAP_SRC_INCLUDE,
		    node, NULL,
		    "The target namespace of the included/redefined schema "
		    "'%s' has to be absent or the same as the "
		    "including/redefining schema's target namespace",
		    schemaLocation, NULL);
		goto exit_error;
	    }
	    bucket->targetNamespace = pctxt->targetNamespace;
	}
    }
    if (bucket && (!bucket->parsed) && (bucket->doc != NULL)) {
	if (isChameleon) {
	    
	    if ((schema->flags & XML_SCHEMAS_INCLUDING_CONVERT_NS) == 0) {
		schema->flags |= XML_SCHEMAS_INCLUDING_CONVERT_NS;
	    } else
		wasChameleon = 1;
	}
	xmlSchemaParseNewDoc(pctxt, schema, bucket);
	
	if (isChameleon && (!wasChameleon))
	    schema->flags ^= XML_SCHEMAS_INCLUDING_CONVERT_NS;
    }
    child = node->children;
    if (type == XML_SCHEMA_SCHEMA_REDEFINE) {
	pctxt->redefined = bucket;
	pctxt->isRedefine = 1;
	while (IS_SCHEMA(child, "annotation") ||
	    IS_SCHEMA(child, "simpleType") ||
	    IS_SCHEMA(child, "complexType") ||
	    IS_SCHEMA(child, "group") ||
	    IS_SCHEMA(child, "attributeGroup")) {
	    if (IS_SCHEMA(child, "annotation")) {
	    } else if (IS_SCHEMA(child, "simpleType")) {
		xmlSchemaParseSimpleType(pctxt, schema, child, 1);
	    } else if (IS_SCHEMA(child, "complexType")) {
		xmlSchemaParseComplexType(pctxt, schema, child, 1);
		
	    } else if (IS_SCHEMA(child, "group")) {
		
		xmlSchemaParseModelGroupDefinition(pctxt,
		    schema, child);
	    } else if (IS_SCHEMA(child, "attributeGroup")) {
		
		xmlSchemaParseAttributeGroupDefinition(pctxt, schema,
		    child);
	    }
	    child = child->next;
	}
	pctxt->redefined = NULL;
	pctxt->isRedefine = 0;
    } else {
	if (IS_SCHEMA(child, "annotation")) {
	    child = child->next;
	}
    }
    if (child != NULL) {
	res = XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED;
	if (type == XML_SCHEMA_SCHEMA_REDEFINE) {
	    xmlSchemaPContentErr(pctxt, res,
		NULL, node, child, NULL,
		"(annotation | (simpleType | complexType | group | attributeGroup))*");
	} else {
	     xmlSchemaPContentErr(pctxt, res,
		NULL, node, child, NULL,
		"(annotation?)");
	}
    }
    return(res);

exit_error:
    return(pctxt->err);
}

static int
xmlSchemaParseRedefine(xmlSchemaParserCtxtPtr pctxt, xmlSchemaPtr schema,
                       xmlNodePtr node)
{
    int res;
#ifndef ENABLE_REDEFINE
    TODO
    return(0);
#endif
    res = xmlSchemaParseIncludeOrRedefine(pctxt, schema, node,
	XML_SCHEMA_SCHEMA_REDEFINE);
    if (res != 0)
	return(res);
    return(0);
}

static int
xmlSchemaParseInclude(xmlSchemaParserCtxtPtr pctxt, xmlSchemaPtr schema,
                       xmlNodePtr node)
{
    int res;

    res = xmlSchemaParseIncludeOrRedefine(pctxt, schema, node,
	XML_SCHEMA_SCHEMA_INCLUDE);
    if (res != 0)
	return(res);
    return(0);
}

static xmlSchemaTreeItemPtr
xmlSchemaParseModelGroup(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
			 xmlNodePtr node, xmlSchemaTypeType type,
			 int withParticle)
{
    xmlSchemaModelGroupPtr item;
    xmlSchemaParticlePtr particle = NULL;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;
    int min = 1, max = 1, isElemRef, hasRefs = 0;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    item = xmlSchemaAddModelGroup(ctxt, schema, type, node);
    if (item == NULL)
	return (NULL);

    if (withParticle) {
	if (type == XML_SCHEMA_TYPE_ALL) {
	    min = xmlGetMinOccurs(ctxt, node, 0, 1, 1, "(0 | 1)");
	    max = xmlGetMaxOccurs(ctxt, node, 1, 1, 1, "1");
	} else {
	    
	    min = xmlGetMinOccurs(ctxt, node, 0, -1, 1, "xs:nonNegativeInteger");
	    max = xmlGetMaxOccurs(ctxt, node, 0, UNBOUNDED, 1,
		"(xs:nonNegativeInteger | unbounded)");
	}
	xmlSchemaPCheckParticleCorrect_2(ctxt, NULL, node, min, max);
	particle = xmlSchemaAddParticle(ctxt, node, min, max);
	if (particle == NULL)
	    return (NULL);
	particle->children = (xmlSchemaTreeItemPtr) item;
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "maxOccurs")) &&
		    (!xmlStrEqual(attr->name, BAD_CAST "minOccurs"))) {
		    xmlSchemaPIllegalAttrErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	    attr = attr->next;
	}
    } else {
	attr = node->properties;
	while (attr != NULL) {
	    if (attr->ns == NULL) {
		if (!xmlStrEqual(attr->name, BAD_CAST "id")) {
		    xmlSchemaPIllegalAttrErr(ctxt,
			XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		}
	    } else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	    attr = attr->next;
	}
    }

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        item->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    if (type == XML_SCHEMA_TYPE_ALL) {
	xmlSchemaParticlePtr part, last = NULL;

	while (IS_SCHEMA(child, "element")) {
	    part = (xmlSchemaParticlePtr) xmlSchemaParseElement(ctxt,
		schema, child, &isElemRef, 0);
	    if (part != NULL) {
		if (isElemRef)
		    hasRefs++;
		if (part->minOccurs > 1) {
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_COS_ALL_LIMITED,
			NULL, child,
			"Invalid value for minOccurs (must be 0 or 1)",
			NULL);
		    
		    part->minOccurs = 1;
		}
		if (part->maxOccurs > 1) {
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_COS_ALL_LIMITED,
			NULL, child,
			"Invalid value for maxOccurs (must be 0 or 1)",
			NULL);
		    
		    part->maxOccurs = 1;
		}
		if (last == NULL)
		    item->children = (xmlSchemaTreeItemPtr) part;
		else
		    last->next = (xmlSchemaTreeItemPtr) part;
		last = part;
	    }
	    child = child->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, (annotation?, element*)");
	}
    } else {
	
	xmlSchemaTreeItemPtr part = NULL, last = NULL;

	while ((IS_SCHEMA(child, "element")) ||
	    (IS_SCHEMA(child, "group")) ||
	    (IS_SCHEMA(child, "any")) ||
	    (IS_SCHEMA(child, "choice")) ||
	    (IS_SCHEMA(child, "sequence"))) {

	    if (IS_SCHEMA(child, "element")) {
		part = (xmlSchemaTreeItemPtr)
		    xmlSchemaParseElement(ctxt, schema, child, &isElemRef, 0);
		if (part && isElemRef)
		    hasRefs++;
	    } else if (IS_SCHEMA(child, "group")) {
		part =
		    xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
		if (part != NULL)
		    hasRefs++;
		if (ctxt->isRedefine && ctxt->redef &&
		    (ctxt->redef->item->type == XML_SCHEMA_TYPE_GROUP) &&
		    part && part->children)
		{
		    if ((xmlSchemaGetQNameRefName(part->children) ==
			    ctxt->redef->refName) &&
			(xmlSchemaGetQNameRefTargetNs(part->children) ==
			    ctxt->redef->refTargetNs))
		    {
			if (ctxt->redefCounter != 0) {
			    xmlChar *str = NULL;

			    xmlSchemaCustomErr(ACTXT_CAST ctxt,
				XML_SCHEMAP_SRC_REDEFINE, child, NULL,
				"The redefining model group definition "
				"'%s' must not contain more than one "
				"reference to the redefined definition",
				xmlSchemaFormatQName(&str,
				    ctxt->redef->refTargetNs,
				    ctxt->redef->refName),
				NULL);
			    FREE_AND_NULL(str)
			    part = NULL;
			} else if (((WXS_PARTICLE(part))->minOccurs != 1) ||
			    ((WXS_PARTICLE(part))->maxOccurs != 1))
			{
			    xmlChar *str = NULL;
			    xmlSchemaCustomErr(ACTXT_CAST ctxt,
				XML_SCHEMAP_SRC_REDEFINE, child, NULL,
				"The redefining model group definition "
				"'%s' must not contain a reference to the "
				"redefined definition with a "
				"maxOccurs/minOccurs other than 1",
				xmlSchemaFormatQName(&str,
				    ctxt->redef->refTargetNs,
				    ctxt->redef->refName),
				NULL);
			    FREE_AND_NULL(str)
			    part = NULL;
			}
			ctxt->redef->reference = WXS_BASIC_CAST part;
			ctxt->redefCounter++;
		    }
		}
	    } else if (IS_SCHEMA(child, "any")) {
		part = (xmlSchemaTreeItemPtr)
		    xmlSchemaParseAny(ctxt, schema, child);
	    } else if (IS_SCHEMA(child, "choice")) {
		part = xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_CHOICE, 1);
	    } else if (IS_SCHEMA(child, "sequence")) {
		part = xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_SEQUENCE, 1);
	    }
	    if (part != NULL) {
		if (last == NULL)
		    item->children = part;
		else
		    last->next = part;
		last = part;
	    }
	    child = child->next;
	}
	if (child != NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, (element | group | choice | sequence | any)*)");
	}
    }
    if ((max == 0) && (min == 0))
	return (NULL);
    if (hasRefs) {
	WXS_ADD_PENDING(ctxt, item);
    }
    if (withParticle)
	return ((xmlSchemaTreeItemPtr) particle);
    else
	return ((xmlSchemaTreeItemPtr) item);
}

static xmlSchemaTypePtr
xmlSchemaParseRestriction(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                          xmlNodePtr node, xmlSchemaTypeType parentType)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    
    type = ctxt->ctxtType;
    type->flags |= XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION;

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "base"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
    if (xmlSchemaPValAttrQName(ctxt, schema, NULL, node, "base",
	&(type->baseNs), &(type->base)) == 0)
    {
	if ((type->base == NULL) && (type->type == XML_SCHEMA_TYPE_COMPLEX)) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING,
		NULL, node, "base", NULL);
	} else if ((ctxt->isRedefine) &&
	    (type->flags & XML_SCHEMAS_TYPE_GLOBAL))
	{
	    if (type->base == NULL) {
		xmlSchemaPMissingAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_MISSING,
		    NULL, node, "base", NULL);
	    } else if ((! xmlStrEqual(type->base, type->name)) ||
		(! xmlStrEqual(type->baseNs, type->targetNamespace)))
	    {
		xmlChar *str1 = NULL, *str2 = NULL;
		xmlSchemaPCustomErrExt(ctxt, XML_SCHEMAP_SRC_REDEFINE,
		    NULL, node, "This is a redefinition, but the QName "
		    "value '%s' of the 'base' attribute does not match the "
		    "type's designation '%s'",
		    xmlSchemaFormatQName(&str1, type->baseNs, type->base),
		    xmlSchemaFormatQName(&str2, type->targetNamespace,
			type->name), NULL);
		FREE_AND_NULL(str1);
		FREE_AND_NULL(str2);
		
		type->base = NULL;
		type->baseNs = NULL;
	    }
	}
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
        child = child->next;
    }
    if (parentType == XML_SCHEMA_TYPE_SIMPLE) {
	if (IS_SCHEMA(child, "simpleType")) {
	    if (type->base != NULL) {
		xmlSchemaPContentErr(ctxt,
		    XML_SCHEMAP_SRC_RESTRICTION_BASE_OR_SIMPLETYPE,
		    NULL, node, child,
		    "The attribute 'base' and the <simpleType> child are "
		    "mutually exclusive", NULL);
	    } else {
		type->baseType = (xmlSchemaTypePtr)
		    xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    }
	    child = child->next;
	} else if (type->base == NULL) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_SRC_RESTRICTION_BASE_OR_SIMPLETYPE,
		NULL, node, child,
		"Either the attribute 'base' or a <simpleType> child "
		"must be present", NULL);
	}
    } else if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	if (IS_SCHEMA(child, "all")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_ALL, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "choice")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt,
		    schema, child, XML_SCHEMA_TYPE_CHOICE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "sequence")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_SEQUENCE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "group")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
	    child = child->next;
	}
    } else if (parentType == XML_SCHEMA_TYPE_SIMPLE_CONTENT) {
	if (IS_SCHEMA(child, "simpleType")) {
	    type->contentTypeDef = (xmlSchemaTypePtr)
		xmlSchemaParseSimpleType(ctxt, schema, child, 0);
	    if ( type->contentTypeDef == NULL)
		return (NULL);
	    child = child->next;
	}
    }

    if ((parentType == XML_SCHEMA_TYPE_SIMPLE) ||
	(parentType == XML_SCHEMA_TYPE_SIMPLE_CONTENT)) {
	xmlSchemaFacetPtr facet, lastfacet = NULL;

	while ((IS_SCHEMA(child, "minInclusive")) ||
	    (IS_SCHEMA(child, "minExclusive")) ||
	    (IS_SCHEMA(child, "maxInclusive")) ||
	    (IS_SCHEMA(child, "maxExclusive")) ||
	    (IS_SCHEMA(child, "totalDigits")) ||
	    (IS_SCHEMA(child, "fractionDigits")) ||
	    (IS_SCHEMA(child, "pattern")) ||
	    (IS_SCHEMA(child, "enumeration")) ||
	    (IS_SCHEMA(child, "whiteSpace")) ||
	    (IS_SCHEMA(child, "length")) ||
	    (IS_SCHEMA(child, "maxLength")) ||
	    (IS_SCHEMA(child, "minLength"))) {
	    facet = xmlSchemaParseFacet(ctxt, schema, child);
	    if (facet != NULL) {
		if (lastfacet == NULL)
		    type->facets = facet;
		else
		    lastfacet->next = facet;
		lastfacet = facet;
		lastfacet->next = NULL;
	    }
	    child = child->next;
	}
	if (type->facets != NULL) {
	    xmlSchemaFacetLinkPtr facetLink, lastFacetLink = NULL;

	    facet = type->facets;
	    do {
		facetLink = (xmlSchemaFacetLinkPtr)
		    xmlMalloc(sizeof(xmlSchemaFacetLink));
		if (facetLink == NULL) {
		    xmlSchemaPErrMemory(ctxt, "allocating a facet link", NULL);
		    xmlFree(facetLink);
		    return (NULL);
		}
		facetLink->facet = facet;
		facetLink->next = NULL;
		if (lastFacetLink == NULL)
		    type->facetSet = facetLink;
		else
		    lastFacetLink->next = facetLink;
		lastFacetLink = facetLink;
		facet = facet->next;
	    } while (facet != NULL);
	}
    }
    if (type->type == XML_SCHEMA_TYPE_COMPLEX) {
	if (xmlSchemaParseLocalAttributes(ctxt, schema, &child,
	    (xmlSchemaItemListPtr *) &(type->attrUses),
	    XML_SCHEMA_TYPE_RESTRICTION, NULL) == -1)
	    return(NULL);
	if (IS_SCHEMA(child, "anyAttribute")) {
	    type->attributeWildcard =
		xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"annotation?, (group | all | choice | sequence)?, "
		"((attribute | attributeGroup)*, anyAttribute?))");
	} else if (parentType == XML_SCHEMA_TYPE_SIMPLE_CONTENT) {
	     xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, (simpleType?, (minExclusive | minInclusive | "
		"maxExclusive | maxInclusive | totalDigits | fractionDigits | "
		"length | minLength | maxLength | enumeration | whiteSpace | "
		"pattern)*)?, ((attribute | attributeGroup)*, anyAttribute?))");
	} else {
	    
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, (simpleType?, (minExclusive | minInclusive | "
		"maxExclusive | maxInclusive | totalDigits | fractionDigits | "
		"length | minLength | maxLength | enumeration | whiteSpace | "
		"pattern)*))");
	}
    }
    return (NULL);
}

static xmlSchemaTypePtr
xmlSchemaParseExtension(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                        xmlNodePtr node, xmlSchemaTypeType parentType)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);
    
    type = ctxt->ctxtType;
    type->flags |= XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION;

    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "base"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");

    if ((xmlSchemaPValAttrQName(ctxt, schema, NULL, node,
	"base", &(type->baseNs), &(type->base)) == 0) &&
	(type->base == NULL)) {
	xmlSchemaPMissingAttrErr(ctxt,
	    XML_SCHEMAP_S4S_ATTR_MISSING,
	    NULL, node, "base", NULL);
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
        child = child->next;
    }
    if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	if (IS_SCHEMA(child, "all")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema,
		    child, XML_SCHEMA_TYPE_ALL, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "choice")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema,
		    child, XML_SCHEMA_TYPE_CHOICE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "sequence")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema,
		child, XML_SCHEMA_TYPE_SEQUENCE, 1);
	    child = child->next;
	} else if (IS_SCHEMA(child, "group")) {
	    type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	if (xmlSchemaParseLocalAttributes(ctxt, schema, &child,
	    (xmlSchemaItemListPtr *) &(type->attrUses),
	    XML_SCHEMA_TYPE_EXTENSION, NULL) == -1)
	    return(NULL);
	if (IS_SCHEMA(child, "anyAttribute")) {
	    ctxt->ctxtType->attributeWildcard =
		xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	if (parentType == XML_SCHEMA_TYPE_COMPLEX_CONTENT) {
	    
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, ((group | all | choice | sequence)?, "
		"((attribute | attributeGroup)*, anyAttribute?)))");
	} else {
	    
	    xmlSchemaPContentErr(ctxt,
		XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
		NULL, node, child, NULL,
		"(annotation?, ((attribute | attributeGroup)*, "
		"anyAttribute?))");
	}
    }
    return (NULL);
}

static int
xmlSchemaParseSimpleContent(xmlSchemaParserCtxtPtr ctxt,
                            xmlSchemaPtr schema, xmlNodePtr node,
			    int *hasRestrictionOrExtension)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL) ||
	(hasRestrictionOrExtension == NULL))
        return (-1);
    *hasRestrictionOrExtension = 0;
    
    type = ctxt->ctxtType;
    type->contentType = XML_SCHEMA_CONTENT_SIMPLE;
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id"))) {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");

    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
        child = child->next;
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_MISSING,
	    NULL, node, NULL, NULL,
	    "(annotation?, (restriction | extension))");
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_MISSING,
	    NULL, node, NULL, NULL,
	    "(annotation?, (restriction | extension))");
    }
    if (IS_SCHEMA(child, "restriction")) {
        xmlSchemaParseRestriction(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SIMPLE_CONTENT);
	(*hasRestrictionOrExtension) = 1;
        child = child->next;
    } else if (IS_SCHEMA(child, "extension")) {
        xmlSchemaParseExtension(ctxt, schema, child,
	    XML_SCHEMA_TYPE_SIMPLE_CONTENT);
	(*hasRestrictionOrExtension) = 1;
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child, NULL,
	    "(annotation?, (restriction | extension))");
    }
    return (0);
}

static int
xmlSchemaParseComplexContent(xmlSchemaParserCtxtPtr ctxt,
                             xmlSchemaPtr schema, xmlNodePtr node,
			     int *hasRestrictionOrExtension)
{
    xmlSchemaTypePtr type;
    xmlNodePtr child = NULL;
    xmlAttrPtr attr;

    if ((ctxt == NULL) || (schema == NULL) || (node == NULL) ||
	(hasRestrictionOrExtension == NULL))
        return (-1);
    *hasRestrictionOrExtension = 0;
    
    type = ctxt->ctxtType;
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if ((!xmlStrEqual(attr->name, BAD_CAST "id")) &&
		(!xmlStrEqual(attr->name, BAD_CAST "mixed")))
	    {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }

    xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");

    if (xmlGetBooleanProp(ctxt, node, "mixed", 0))  {
	if ((type->flags & XML_SCHEMAS_TYPE_MIXED) == 0)
	    type->flags |= XML_SCHEMAS_TYPE_MIXED;
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
	xmlSchemaAddAnnotation((xmlSchemaAnnotItemPtr) type,
	    xmlSchemaParseAnnotation(ctxt, child, 1));
        child = child->next;
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_MISSING,
	    NULL, node, NULL,
	    NULL, "(annotation?, (restriction | extension))");
    }
    if (child == NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_MISSING,
	    NULL, node, NULL,
	    NULL, "(annotation?, (restriction | extension))");
    }
    if (IS_SCHEMA(child, "restriction")) {
        xmlSchemaParseRestriction(ctxt, schema, child,
	    XML_SCHEMA_TYPE_COMPLEX_CONTENT);
	(*hasRestrictionOrExtension) = 1;
        child = child->next;
    } else if (IS_SCHEMA(child, "extension")) {
        xmlSchemaParseExtension(ctxt, schema, child,
	    XML_SCHEMA_TYPE_COMPLEX_CONTENT);
	(*hasRestrictionOrExtension) = 1;
        child = child->next;
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?, (restriction | extension))");
    }
    return (0);
}

static xmlSchemaTypePtr
xmlSchemaParseComplexType(xmlSchemaParserCtxtPtr ctxt, xmlSchemaPtr schema,
                          xmlNodePtr node, int topLevel)
{
    xmlSchemaTypePtr type, ctxtType;
    xmlNodePtr child = NULL;
    const xmlChar *name = NULL;
    xmlAttrPtr attr;
    const xmlChar *attrValue;
#ifdef ENABLE_NAMED_LOCALS
    char buf[40];
#endif
    int final = 0, block = 0, hasRestrictionOrExtension = 0;


    if ((ctxt == NULL) || (schema == NULL) || (node == NULL))
        return (NULL);

    ctxtType = ctxt->ctxtType;

    if (topLevel) {
	attr = xmlSchemaGetPropNode(node, "name");
	if (attr == NULL) {
	    xmlSchemaPMissingAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_MISSING, NULL, node, "name", NULL);
	    return (NULL);
	} else if (xmlSchemaPValAttrNode(ctxt, NULL, attr,
	    xmlSchemaGetBuiltInType(XML_SCHEMAS_NCNAME), &name) != 0) {
	    return (NULL);
	}
    }

    if (topLevel == 0) {
#ifdef ENABLE_NAMED_LOCALS
        snprintf(buf, 39, "#CT%d", ctxt->counter++ + 1);
	type = xmlSchemaAddType(ctxt, schema,
	    XML_SCHEMA_TYPE_COMPLEX,
	    xmlDictLookup(ctxt->dict, (const xmlChar *)buf, -1),
	    ctxt->targetNamespace, node, 0);
#else
	type = xmlSchemaAddType(ctxt, schema,
	    XML_SCHEMA_TYPE_COMPLEX,
	    NULL, ctxt->targetNamespace, node, 0);
#endif
	if (type == NULL)
	    return (NULL);
	name = type->name;
	type->node = node;
	type->type = XML_SCHEMA_TYPE_COMPLEX;
    } else {
	type = xmlSchemaAddType(ctxt, schema,
	    XML_SCHEMA_TYPE_COMPLEX,
	    name, ctxt->targetNamespace, node, 1);
	if (type == NULL)
	    return (NULL);
	type->node = node;
	type->type = XML_SCHEMA_TYPE_COMPLEX;
	type->flags |= XML_SCHEMAS_TYPE_GLOBAL;
    }
    type->targetNamespace = ctxt->targetNamespace;
    attr = node->properties;
    while (attr != NULL) {
	if (attr->ns == NULL) {
	    if (xmlStrEqual(attr->name, BAD_CAST "id")) {
		xmlSchemaPValAttrID(ctxt, node, BAD_CAST "id");
	    } else if (xmlStrEqual(attr->name, BAD_CAST "mixed")) {
		if (xmlSchemaPGetBoolNodeValue(ctxt,
			NULL, (xmlNodePtr) attr))
		    type->flags |= XML_SCHEMAS_TYPE_MIXED;
	    } else if (topLevel) {
		if (xmlStrEqual(attr->name, BAD_CAST "name")) {
		    
		} else if (xmlStrEqual(attr->name, BAD_CAST "abstract")) {
		    if (xmlSchemaPGetBoolNodeValue(ctxt,
			    NULL, (xmlNodePtr) attr))
			type->flags |= XML_SCHEMAS_TYPE_ABSTRACT;
		} else if (xmlStrEqual(attr->name, BAD_CAST "final")) {
		    attrValue = xmlSchemaGetNodeContent(ctxt,
			(xmlNodePtr) attr);
		    if (xmlSchemaPValAttrBlockFinal(attrValue,
			&(type->flags),
			-1,
			XML_SCHEMAS_TYPE_FINAL_EXTENSION,
			XML_SCHEMAS_TYPE_FINAL_RESTRICTION,
			-1, -1, -1) != 0)
		    {
			xmlSchemaPSimpleTypeErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			    NULL, (xmlNodePtr) attr, NULL,
			    "(#all | List of (extension | restriction))",
			    attrValue, NULL, NULL, NULL);
		    } else
			final = 1;
		} else if (xmlStrEqual(attr->name, BAD_CAST "block")) {
		    attrValue = xmlSchemaGetNodeContent(ctxt,
			(xmlNodePtr) attr);
		    if (xmlSchemaPValAttrBlockFinal(attrValue, &(type->flags),
			-1,
			XML_SCHEMAS_TYPE_BLOCK_EXTENSION,
			XML_SCHEMAS_TYPE_BLOCK_RESTRICTION,
			-1, -1, -1) != 0) {
			xmlSchemaPSimpleTypeErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_INVALID_VALUE,
			    NULL, (xmlNodePtr) attr, NULL,
			    "(#all | List of (extension | restriction)) ",
			    attrValue, NULL, NULL, NULL);
		    } else
			block = 1;
		} else {
			xmlSchemaPIllegalAttrErr(ctxt,
			    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
		}
	    } else {
		xmlSchemaPIllegalAttrErr(ctxt,
		    XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	    }
	} else if (xmlStrEqual(attr->ns->href, xmlSchemaNs)) {
	    xmlSchemaPIllegalAttrErr(ctxt,
		XML_SCHEMAP_S4S_ATTR_NOT_ALLOWED, NULL, attr);
	}
	attr = attr->next;
    }
    if (! block) {
	if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION)
	    type->flags |= XML_SCHEMAS_TYPE_BLOCK_RESTRICTION;
	if (schema->flags & XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION)
	    type->flags |= XML_SCHEMAS_TYPE_BLOCK_EXTENSION;
    }
    if (! final) {
	if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION)
	    type->flags |= XML_SCHEMAS_TYPE_FINAL_RESTRICTION;
	if (schema->flags & XML_SCHEMAS_FINAL_DEFAULT_EXTENSION)
	    type->flags |= XML_SCHEMAS_TYPE_FINAL_EXTENSION;
    }
    child = node->children;
    if (IS_SCHEMA(child, "annotation")) {
        type->annot = xmlSchemaParseAnnotation(ctxt, child, 1);
        child = child->next;
    }
    ctxt->ctxtType = type;
    if (IS_SCHEMA(child, "simpleContent")) {
	if (type->flags & XML_SCHEMAS_TYPE_MIXED)
	    type->flags ^= XML_SCHEMAS_TYPE_MIXED;
        xmlSchemaParseSimpleContent(ctxt, schema, child,
	    &hasRestrictionOrExtension);
        child = child->next;
    } else if (IS_SCHEMA(child, "complexContent")) {
	type->contentType = XML_SCHEMA_CONTENT_EMPTY;
        xmlSchemaParseComplexContent(ctxt, schema, child,
	    &hasRestrictionOrExtension);
        child = child->next;
    } else {
	type->baseType = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
	type->flags |= XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION;
        if (IS_SCHEMA(child, "all")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_ALL, 1);
            child = child->next;
        } else if (IS_SCHEMA(child, "choice")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_CHOICE, 1);
            child = child->next;
        } else if (IS_SCHEMA(child, "sequence")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroup(ctxt, schema, child,
		    XML_SCHEMA_TYPE_SEQUENCE, 1);
            child = child->next;
        } else if (IS_SCHEMA(child, "group")) {
            type->subtypes = (xmlSchemaTypePtr)
		xmlSchemaParseModelGroupDefRef(ctxt, schema, child);
            child = child->next;
        }
        if (xmlSchemaParseLocalAttributes(ctxt, schema, &child,
	    (xmlSchemaItemListPtr *) &(type->attrUses),
	    XML_SCHEMA_TYPE_RESTRICTION, NULL) == -1)
	    return(NULL);
	if (IS_SCHEMA(child, "anyAttribute")) {
	    type->attributeWildcard = xmlSchemaParseAnyAttribute(ctxt, schema, child);
	    child = child->next;
	}
    }
    if (child != NULL) {
	xmlSchemaPContentErr(ctxt,
	    XML_SCHEMAP_S4S_ELEM_NOT_ALLOWED,
	    NULL, node, child,
	    NULL, "(annotation?, (simpleContent | complexContent | "
	    "((group | all | choice | sequence)?, ((attribute | "
	    "attributeGroup)*, anyAttribute?))))");
    }
    if (topLevel && ctxt->isRedefine && (! hasRestrictionOrExtension)) {
	xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_SRC_REDEFINE,
	    NULL, node, "This is a redefinition, thus the "
	    "<complexType> must have a <restriction> or <extension> "
	    "grand-child", NULL);
    }
    ctxt->ctxtType = ctxtType;
    return (type);
}



#if 0 
static int
xmlSchemaParserCtxtSetOptions(xmlSchemaParserCtxtPtr ctxt,
			      int options)

{
    int i;

    if (ctxt == NULL)
	return (-1);
    for (i = 1; i < (int) sizeof(int) * 8; i++) {
        if (options & 1<<i) {
	    return (-1);
        }
    }
    ctxt->options = options;
    return (0);
}

static int
xmlSchemaParserCtxtGetOptions(xmlSchemaParserCtxtPtr ctxt)

{
    if (ctxt == NULL)
	return (-1);
    else
	return (ctxt->options);
}
#endif

xmlSchemaParserCtxtPtr
xmlSchemaNewParserCtxt(const char *URL)
{
    xmlSchemaParserCtxtPtr ret;

    if (URL == NULL)
        return (NULL);

    ret = xmlSchemaParserCtxtCreate();
    if (ret == NULL)
	return(NULL);
    ret->dict = xmlDictCreate();
    ret->URL = xmlDictLookup(ret->dict, (const xmlChar *) URL, -1);
    return (ret);
}

xmlSchemaParserCtxtPtr
xmlSchemaNewMemParserCtxt(const char *buffer, int size)
{
    xmlSchemaParserCtxtPtr ret;

    if ((buffer == NULL) || (size <= 0))
        return (NULL);
    ret = xmlSchemaParserCtxtCreate();
    if (ret == NULL)
	return(NULL);
    ret->buffer = buffer;
    ret->size = size;
    ret->dict = xmlDictCreate();
    return (ret);
}

xmlSchemaParserCtxtPtr
xmlSchemaNewDocParserCtxt(xmlDocPtr doc)
{
    xmlSchemaParserCtxtPtr ret;

    if (doc == NULL)
      return (NULL);
    ret = xmlSchemaParserCtxtCreate();
    if (ret == NULL)
	return(NULL);
    ret->doc = doc;
    ret->dict = xmlDictCreate();
    
    ret->preserve = 1;

    return (ret);
}

void
xmlSchemaFreeParserCtxt(xmlSchemaParserCtxtPtr ctxt)
{
    if (ctxt == NULL)
        return;
    if (ctxt->doc != NULL && !ctxt->preserve)
        xmlFreeDoc(ctxt->doc);
    if (ctxt->vctxt != NULL) {
	xmlSchemaFreeValidCtxt(ctxt->vctxt);
    }
    if (ctxt->ownsConstructor && (ctxt->constructor != NULL)) {
	xmlSchemaConstructionCtxtFree(ctxt->constructor);
	ctxt->constructor = NULL;
	ctxt->ownsConstructor = 0;
    }
    if (ctxt->attrProhibs != NULL)
	xmlSchemaItemListFree(ctxt->attrProhibs);
    xmlDictFree(ctxt->dict);
    xmlFree(ctxt);
}


static int
xmlSchemaBuildContentModelForSubstGroup(xmlSchemaParserCtxtPtr pctxt,
	xmlSchemaParticlePtr particle, int counter, xmlAutomataStatePtr end)
{
    xmlAutomataStatePtr start, tmp;
    xmlSchemaElementPtr elemDecl, member;
    xmlSchemaSubstGroupPtr substGroup;
    int i;
    int ret = 0;

    elemDecl = (xmlSchemaElementPtr) particle->children;
    start = pctxt->state;
    if (end == NULL)
	end = xmlAutomataNewState(pctxt->am);
    substGroup = xmlSchemaSubstGroupGet(pctxt, elemDecl);
    if (substGroup == NULL) {
	xmlSchemaPErr(pctxt, WXS_ITEM_NODE(particle),
	    XML_SCHEMAP_INTERNAL,
	    "Internal error: xmlSchemaBuildContentModelForSubstGroup, "
	    "declaration is marked having a subst. group but none "
	    "available.\n", elemDecl->name, NULL);
	return(0);
    }
    if (counter >= 0) {
	tmp = xmlAutomataNewCountedTrans(pctxt->am, start, NULL, counter);
        xmlAutomataNewTransition2(pctxt->am, tmp, end,
	            elemDecl->name, elemDecl->targetNamespace, elemDecl);
	for (i = 0; i < substGroup->members->nbItems; i++) {
	    member = (xmlSchemaElementPtr) substGroup->members->items[i];
            xmlAutomataNewTransition2(pctxt->am, tmp, end,
		               member->name, member->targetNamespace, member);
	}
    } else if (particle->maxOccurs == 1) {
	xmlAutomataNewEpsilon(pctxt->am,
	    xmlAutomataNewTransition2(pctxt->am,
	    start, NULL,
	    elemDecl->name, elemDecl->targetNamespace, elemDecl), end);
	for (i = 0; i < substGroup->members->nbItems; i++) {
	    member = (xmlSchemaElementPtr) substGroup->members->items[i];
	    tmp = xmlAutomataNewTransition2(pctxt->am, start, NULL,
		member->name, member->targetNamespace, member);
	    xmlAutomataNewEpsilon(pctxt->am, tmp, end);
	}
    } else {
	xmlAutomataStatePtr hop;
	int maxOccurs = particle->maxOccurs == UNBOUNDED ?
	    UNBOUNDED : particle->maxOccurs - 1;
	int minOccurs = particle->minOccurs < 1 ? 0 : particle->minOccurs - 1;

	counter =
	    xmlAutomataNewCounter(pctxt->am, minOccurs,
	    maxOccurs);
	hop = xmlAutomataNewState(pctxt->am);

	xmlAutomataNewEpsilon(pctxt->am,
	    xmlAutomataNewTransition2(pctxt->am,
	    start, NULL,
	    elemDecl->name, elemDecl->targetNamespace, elemDecl),
	    hop);
	for (i = 0; i < substGroup->members->nbItems; i++) {
	    member = (xmlSchemaElementPtr) substGroup->members->items[i];
	    xmlAutomataNewEpsilon(pctxt->am,
		xmlAutomataNewTransition2(pctxt->am,
		start, NULL,
		member->name, member->targetNamespace, member),
		hop);
	}
	xmlAutomataNewCountedTrans(pctxt->am, hop, start, counter);
	xmlAutomataNewCounterTrans(pctxt->am, hop, end, counter);
    }
    if (particle->minOccurs == 0) {
	xmlAutomataNewEpsilon(pctxt->am, start, end);
        ret = 1;
    }
    pctxt->state = end;
    return(ret);
}

static int
xmlSchemaBuildContentModelForElement(xmlSchemaParserCtxtPtr ctxt,
				     xmlSchemaParticlePtr particle)
{
    int ret = 0;

    if (((xmlSchemaElementPtr) particle->children)->flags &
	XML_SCHEMAS_ELEM_SUBST_GROUP_HEAD) {
	ret = xmlSchemaBuildContentModelForSubstGroup(ctxt, particle, -1, NULL);
    } else {
	xmlSchemaElementPtr elemDecl;
	xmlAutomataStatePtr start;

	elemDecl = (xmlSchemaElementPtr) particle->children;

	if (elemDecl->flags & XML_SCHEMAS_ELEM_ABSTRACT)
	    return(0);
	if (particle->maxOccurs == 1) {
	    start = ctxt->state;
	    ctxt->state = xmlAutomataNewTransition2(ctxt->am, start, NULL,
		    elemDecl->name, elemDecl->targetNamespace, elemDecl);
	} else if ((particle->maxOccurs >= UNBOUNDED) &&
	           (particle->minOccurs < 2)) {
	    
	    start = ctxt->state;
	    ctxt->state = xmlAutomataNewTransition2(ctxt->am, start, NULL,
		elemDecl->name, elemDecl->targetNamespace, elemDecl);
	    ctxt->state = xmlAutomataNewTransition2(ctxt->am, ctxt->state, ctxt->state,
		elemDecl->name, elemDecl->targetNamespace, elemDecl);
	} else {
	    int counter;
	    int maxOccurs = particle->maxOccurs == UNBOUNDED ?
			    UNBOUNDED : particle->maxOccurs - 1;
	    int minOccurs = particle->minOccurs < 1 ?
			    0 : particle->minOccurs - 1;

	    start = xmlAutomataNewEpsilon(ctxt->am, ctxt->state, NULL);
	    counter = xmlAutomataNewCounter(ctxt->am, minOccurs, maxOccurs);
	    ctxt->state = xmlAutomataNewTransition2(ctxt->am, start, NULL,
		elemDecl->name, elemDecl->targetNamespace, elemDecl);
	    xmlAutomataNewCountedTrans(ctxt->am, ctxt->state, start, counter);
	    ctxt->state = xmlAutomataNewCounterTrans(ctxt->am, ctxt->state,
		NULL, counter);
	}
	if (particle->minOccurs == 0) {
	    xmlAutomataNewEpsilon(ctxt->am, start, ctxt->state);
            ret = 1;
        }
    }
    return(ret);
}

static int
xmlSchemaBuildAContentModel(xmlSchemaParserCtxtPtr pctxt,
			    xmlSchemaParticlePtr particle)
{
    int ret = 0, tmp2;

    if (particle == NULL) {
	PERROR_INT("xmlSchemaBuildAContentModel", "particle is NULL");
	return(1);
    }
    if (particle->children == NULL) {
	return(1);
    }

    switch (particle->children->type) {
	case XML_SCHEMA_TYPE_ANY: {
	    xmlAutomataStatePtr start, end;
	    xmlSchemaWildcardPtr wild;
	    xmlSchemaWildcardNsPtr ns;

	    wild = (xmlSchemaWildcardPtr) particle->children;

	    start = pctxt->state;
	    end = xmlAutomataNewState(pctxt->am);

	    if (particle->maxOccurs == 1) {
		if (wild->any == 1) {
		    pctxt->state =
			xmlAutomataNewTransition2(pctxt->am,
			start, NULL, BAD_CAST "*", BAD_CAST "*", wild);
		    xmlAutomataNewEpsilon(pctxt->am, pctxt->state, end);
		    pctxt->state =
			xmlAutomataNewTransition2(pctxt->am,
			start, NULL, BAD_CAST "*", NULL, wild);
		    xmlAutomataNewEpsilon(pctxt->am, pctxt->state, end);

		} else if (wild->nsSet != NULL) {
		    ns = wild->nsSet;
		    do {
			pctxt->state = start;
			pctxt->state = xmlAutomataNewTransition2(pctxt->am,
			    pctxt->state, NULL, BAD_CAST "*", ns->value, wild);
			xmlAutomataNewEpsilon(pctxt->am, pctxt->state, end);
			ns = ns->next;
		    } while (ns != NULL);

		} else if (wild->negNsSet != NULL) {
		    pctxt->state = xmlAutomataNewNegTrans(pctxt->am,
			start, end, BAD_CAST "*", wild->negNsSet->value,
			wild);
		}
	    } else {
		int counter;
		xmlAutomataStatePtr hop;
		int maxOccurs =
		    particle->maxOccurs == UNBOUNDED ? UNBOUNDED :
                                           particle->maxOccurs - 1;
		int minOccurs =
		    particle->minOccurs < 1 ? 0 : particle->minOccurs - 1;

		counter = xmlAutomataNewCounter(pctxt->am, minOccurs, maxOccurs);
		hop = xmlAutomataNewState(pctxt->am);
		if (wild->any == 1) {
		    pctxt->state =
			xmlAutomataNewTransition2(pctxt->am,
			start, NULL, BAD_CAST "*", BAD_CAST "*", wild);
		    xmlAutomataNewEpsilon(pctxt->am, pctxt->state, hop);
		    pctxt->state =
			xmlAutomataNewTransition2(pctxt->am,
			start, NULL, BAD_CAST "*", NULL, wild);
		    xmlAutomataNewEpsilon(pctxt->am, pctxt->state, hop);
		} else if (wild->nsSet != NULL) {
		    ns = wild->nsSet;
		    do {
			pctxt->state =
			    xmlAutomataNewTransition2(pctxt->am,
				start, NULL, BAD_CAST "*", ns->value, wild);
			xmlAutomataNewEpsilon(pctxt->am, pctxt->state, hop);
			ns = ns->next;
		    } while (ns != NULL);

		} else if (wild->negNsSet != NULL) {
		    pctxt->state = xmlAutomataNewNegTrans(pctxt->am,
			start, hop, BAD_CAST "*", wild->negNsSet->value,
			wild);
		}
		xmlAutomataNewCountedTrans(pctxt->am, hop, start, counter);
		xmlAutomataNewCounterTrans(pctxt->am, hop, end, counter);
	    }
	    if (particle->minOccurs == 0) {
		xmlAutomataNewEpsilon(pctxt->am, start, end);
                ret = 1;
	    }
	    pctxt->state = end;
            break;
	}
        case XML_SCHEMA_TYPE_ELEMENT:
	    ret = xmlSchemaBuildContentModelForElement(pctxt, particle);
	    break;
        case XML_SCHEMA_TYPE_SEQUENCE:{
            xmlSchemaTreeItemPtr sub;

            ret = 1;
            if ((particle->minOccurs == 1) && (particle->maxOccurs == 1)) {
                sub = particle->children->children;

                while (sub != NULL) {
                    tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                        (xmlSchemaParticlePtr) sub);
                    if (tmp2 != 1) ret = 0;
                    sub = sub->next;
                }
            } else {
                xmlAutomataStatePtr oldstate = pctxt->state;

                if (particle->maxOccurs >= UNBOUNDED) {
                    if (particle->minOccurs > 1) {
                        xmlAutomataStatePtr tmp;
                        int counter;

                        pctxt->state = xmlAutomataNewEpsilon(pctxt->am,
                            oldstate, NULL);
                        oldstate = pctxt->state;

                        counter = xmlAutomataNewCounter(pctxt->am,
                            particle->minOccurs - 1, UNBOUNDED);

                        sub = particle->children->children;
                        while (sub != NULL) {
                            tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                            (xmlSchemaParticlePtr) sub);
                            if (tmp2 != 1) ret = 0;
                            sub = sub->next;
                        }
                        tmp = pctxt->state;
                        xmlAutomataNewCountedTrans(pctxt->am, tmp,
                                                   oldstate, counter);
                        pctxt->state =
                            xmlAutomataNewCounterTrans(pctxt->am, tmp,
                                                       NULL, counter);
                        if (ret == 1)
                            xmlAutomataNewEpsilon(pctxt->am,
                                                oldstate, pctxt->state);

                    } else {
                        pctxt->state = xmlAutomataNewEpsilon(pctxt->am,
                            oldstate, NULL);
                        oldstate = pctxt->state;

                        sub = particle->children->children;
                        while (sub != NULL) {
                            tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                        (xmlSchemaParticlePtr) sub);
                            if (tmp2 != 1) ret = 0;
                            sub = sub->next;
                        }
                        xmlAutomataNewEpsilon(pctxt->am, pctxt->state,
                                              oldstate);
                        pctxt->state = xmlAutomataNewEpsilon(pctxt->am,
                                            pctxt->state, NULL);
                        if (particle->minOccurs == 0) {
                            xmlAutomataNewEpsilon(pctxt->am,
                                oldstate, pctxt->state);
                            ret = 1;
                        }
                    }
                } else if ((particle->maxOccurs > 1)
                           || (particle->minOccurs > 1)) {
                    xmlAutomataStatePtr tmp;
                    int counter;

                    pctxt->state = xmlAutomataNewEpsilon(pctxt->am,
                        oldstate, NULL);
                    oldstate = pctxt->state;

                    counter = xmlAutomataNewCounter(pctxt->am,
                        particle->minOccurs - 1,
                        particle->maxOccurs - 1);

                    sub = particle->children->children;
                    while (sub != NULL) {
                        tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                        (xmlSchemaParticlePtr) sub);
                        if (tmp2 != 1) ret = 0;
                        sub = sub->next;
                    }
                    tmp = pctxt->state;
                    xmlAutomataNewCountedTrans(pctxt->am,
                        tmp, oldstate, counter);
                    pctxt->state =
                        xmlAutomataNewCounterTrans(pctxt->am, tmp, NULL,
                                                   counter);
                    if ((particle->minOccurs == 0) || (ret == 1)) {
                        xmlAutomataNewEpsilon(pctxt->am,
                                            oldstate, pctxt->state);
                        ret = 1;
                    }
                } else {
                    sub = particle->children->children;
                    while (sub != NULL) {
                        tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                        (xmlSchemaParticlePtr) sub);
                        if (tmp2 != 1) ret = 0;
                        sub = sub->next;
                    }
                    if (particle->minOccurs == 0) {
                        xmlAutomataNewEpsilon(pctxt->am, oldstate,
                                              pctxt->state);
                        ret = 1;
                    }
                }
            }
            break;
        }
        case XML_SCHEMA_TYPE_CHOICE:{
            xmlSchemaTreeItemPtr sub;
            xmlAutomataStatePtr start, end;

            ret = 0;
            start = pctxt->state;
            end = xmlAutomataNewState(pctxt->am);

            if (particle->maxOccurs == 1) {
                sub = particle->children->children;
                while (sub != NULL) {
                    pctxt->state = start;
                    tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                        (xmlSchemaParticlePtr) sub);
                    if (tmp2 == 1) ret = 1;
                    xmlAutomataNewEpsilon(pctxt->am, pctxt->state, end);
                    sub = sub->next;
                }
            } else {
                int counter;
                xmlAutomataStatePtr hop, base;
                int maxOccurs = particle->maxOccurs == UNBOUNDED ?
                    UNBOUNDED : particle->maxOccurs - 1;
                int minOccurs =
                    particle->minOccurs < 1 ? 0 : particle->minOccurs - 1;

                counter =
                    xmlAutomataNewCounter(pctxt->am, minOccurs, maxOccurs);
                hop = xmlAutomataNewState(pctxt->am);
                base = xmlAutomataNewState(pctxt->am);

                sub = particle->children->children;
                while (sub != NULL) {
                    pctxt->state = base;
                    tmp2 = xmlSchemaBuildAContentModel(pctxt,
                                        (xmlSchemaParticlePtr) sub);
                    if (tmp2 == 1) ret = 1;
                    xmlAutomataNewEpsilon(pctxt->am, pctxt->state, hop);
                    sub = sub->next;
                }
                xmlAutomataNewEpsilon(pctxt->am, start, base);
                xmlAutomataNewCountedTrans(pctxt->am, hop, base, counter);
                xmlAutomataNewCounterTrans(pctxt->am, hop, end, counter);
                if (ret == 1)
                    xmlAutomataNewEpsilon(pctxt->am, base, end);
            }
            if (particle->minOccurs == 0) {
                xmlAutomataNewEpsilon(pctxt->am, start, end);
                ret = 1;
            }
            pctxt->state = end;
            break;
        }
        case XML_SCHEMA_TYPE_ALL:{
            xmlAutomataStatePtr start, tmp;
            xmlSchemaParticlePtr sub;
            xmlSchemaElementPtr elemDecl;

            ret = 1;

            sub = (xmlSchemaParticlePtr) particle->children->children;
            if (sub == NULL)
                break;

            ret = 0;

            start = pctxt->state;
            tmp = xmlAutomataNewState(pctxt->am);
            xmlAutomataNewEpsilon(pctxt->am, pctxt->state, tmp);
            pctxt->state = tmp;
            while (sub != NULL) {
                pctxt->state = tmp;

                elemDecl = (xmlSchemaElementPtr) sub->children;
                if (elemDecl == NULL) {
                    PERROR_INT("xmlSchemaBuildAContentModel",
                        "<element> particle has no term");
                    return(ret);
                };
                if (elemDecl->flags & XML_SCHEMAS_ELEM_SUBST_GROUP_HEAD) {
                    int counter;

                    counter = xmlAutomataNewCounter(pctxt->am,
                                       sub->minOccurs, sub->maxOccurs);
                    xmlSchemaBuildContentModelForSubstGroup(pctxt,
                                       sub, counter, pctxt->state);
                } else {
                    if ((sub->minOccurs == 1) &&
                        (sub->maxOccurs == 1)) {
                        xmlAutomataNewOnceTrans2(pctxt->am, pctxt->state,
                                                pctxt->state,
                                                elemDecl->name,
                                                elemDecl->targetNamespace,
                                                1, 1, elemDecl);
                    } else if ((sub->minOccurs == 0) &&
                        (sub->maxOccurs == 1)) {

                        xmlAutomataNewCountTrans2(pctxt->am, pctxt->state,
                                                 pctxt->state,
                                                 elemDecl->name,
                                                 elemDecl->targetNamespace,
                                                 0,
                                                 1,
                                                 elemDecl);
                    }
                }
                sub = (xmlSchemaParticlePtr) sub->next;
            }
            pctxt->state =
                xmlAutomataNewAllTrans(pctxt->am, pctxt->state, NULL, 0);
            if (particle->minOccurs == 0) {
                xmlAutomataNewEpsilon(pctxt->am, start, pctxt->state);
                ret = 1;
            }
            break;
        }
	case XML_SCHEMA_TYPE_GROUP:
            ret = 1;
	    break;
        default:
	    xmlSchemaInternalErr2(ACTXT_CAST pctxt,
		"xmlSchemaBuildAContentModel",
		"found unexpected term of type '%s' in content model",
		WXS_ITEM_TYPE_NAME(particle->children), NULL);
            return(ret);
    }
    return(ret);
}

static void
xmlSchemaBuildContentModel(xmlSchemaTypePtr type,
			   xmlSchemaParserCtxtPtr ctxt)
{
    if ((type->type != XML_SCHEMA_TYPE_COMPLEX) ||
	(type->contModel != NULL) ||
	((type->contentType != XML_SCHEMA_CONTENT_ELEMENTS) &&
	(type->contentType != XML_SCHEMA_CONTENT_MIXED)))
	return;

#ifdef DEBUG_CONTENT
    xmlGenericError(xmlGenericErrorContext,
                    "Building content model for %s\n", name);
#endif
    ctxt->am = NULL;
    ctxt->am = xmlNewAutomata();
    if (ctxt->am == NULL) {
        xmlGenericError(xmlGenericErrorContext,
	    "Cannot create automata for complex type %s\n", type->name);
        return;
    }
    ctxt->state = xmlAutomataGetInitState(ctxt->am);
    xmlSchemaBuildAContentModel(ctxt, WXS_TYPE_PARTICLE(type));
    xmlAutomataSetFinalState(ctxt->am, ctxt->state);
    type->contModel = xmlAutomataCompile(ctxt->am);
    if (type->contModel == NULL) {
        xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_INTERNAL,
	    WXS_BASIC_CAST type, type->node,
	    "Failed to compile the content model", NULL);
    } else if (xmlRegexpIsDeterminist(type->contModel) != 1) {
        xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_NOT_DETERMINISTIC,
	    
	    WXS_BASIC_CAST type, type->node,
	    "The content model is not determinist", NULL);
    } else {
#ifdef DEBUG_CONTENT_REGEXP
        xmlGenericError(xmlGenericErrorContext,
                        "Content model of %s:\n", type->name);
        xmlRegexpPrint(stderr, type->contModel);
#endif
    }
    ctxt->state = NULL;
    xmlFreeAutomata(ctxt->am);
    ctxt->am = NULL;
}

static void
xmlSchemaResolveElementReferences(xmlSchemaElementPtr elemDecl,
				  xmlSchemaParserCtxtPtr ctxt)
{
    if ((ctxt == NULL) || (elemDecl == NULL) ||
	((elemDecl != NULL) &&
	(elemDecl->flags & XML_SCHEMAS_ELEM_INTERNAL_RESOLVED)))
        return;
    elemDecl->flags |= XML_SCHEMAS_ELEM_INTERNAL_RESOLVED;

    if ((elemDecl->subtypes == NULL) && (elemDecl->namedType != NULL)) {
	xmlSchemaTypePtr type;

	type = xmlSchemaGetType(ctxt->schema, elemDecl->namedType,
	    elemDecl->namedTypeNs);
	if (type == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST elemDecl, elemDecl->node,
		"type", elemDecl->namedType, elemDecl->namedTypeNs,
		XML_SCHEMA_TYPE_BASIC, "type definition");
	} else
	    elemDecl->subtypes = type;
    }
    if (elemDecl->substGroup != NULL) {
	xmlSchemaElementPtr substHead;

	substHead = xmlSchemaGetElem(ctxt->schema, elemDecl->substGroup,
	    elemDecl->substGroupNs);
	if (substHead == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST elemDecl, NULL,
		"substitutionGroup", elemDecl->substGroup,
		elemDecl->substGroupNs, XML_SCHEMA_TYPE_ELEMENT, NULL);
	} else {
	    xmlSchemaResolveElementReferences(substHead, ctxt);
	    WXS_SUBST_HEAD(elemDecl) = substHead;
	    if (elemDecl->subtypes == NULL)
		elemDecl->subtypes = substHead->subtypes;
	}
    }
    if ((elemDecl->subtypes == NULL) &&
	(elemDecl->namedType == NULL) &&
	(elemDecl->substGroup == NULL))
	elemDecl->subtypes = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
}

static int
xmlSchemaResolveUnionMemberTypes(xmlSchemaParserCtxtPtr ctxt,
				 xmlSchemaTypePtr type)
{

    xmlSchemaTypeLinkPtr link, lastLink, newLink;
    xmlSchemaTypePtr memberType;

    link = type->memberTypes;
    lastLink = NULL;
    while (link != NULL) {
	const xmlChar *name, *nsName;

	name = ((xmlSchemaQNameRefPtr) link->type)->name;
	nsName = ((xmlSchemaQNameRefPtr) link->type)->targetNamespace;

	memberType = xmlSchemaGetType(ctxt->schema, name, nsName);
	if ((memberType == NULL) || (! WXS_IS_SIMPLE(memberType))) {
	    xmlSchemaPResCompAttrErr(ctxt, XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST type, type->node, "memberTypes",
		name, nsName, XML_SCHEMA_TYPE_SIMPLE, NULL);
	    if (lastLink == NULL)
		type->memberTypes = link->next;
	    else
		lastLink->next = link->next;
	    newLink = link;
	    link = link->next;
	    xmlFree(newLink);
	} else {
	    link->type = memberType;
	    lastLink = link;
	    link = link->next;
	}
    }
    memberType = type->subtypes;
    while (memberType != NULL) {
	link = (xmlSchemaTypeLinkPtr) xmlMalloc(sizeof(xmlSchemaTypeLink));
	if (link == NULL) {
	    xmlSchemaPErrMemory(ctxt, "allocating a type link", NULL);
	    return (-1);
	}
	link->type = memberType;
	link->next = NULL;
	if (lastLink == NULL)
	    type->memberTypes = link;
	else
	    lastLink->next = link;
	lastLink = link;
	memberType = memberType->next;
    }
    return (0);
}

static int
xmlSchemaIsDerivedFromBuiltInType(xmlSchemaTypePtr type, int valType)
{
    if (type == NULL)
	return (0);
    if (WXS_IS_COMPLEX(type))
	return (0);
    if (type->type == XML_SCHEMA_TYPE_BASIC) {
	if (type->builtInType == valType)
	    return(1);
	if ((type->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) ||
	    (type->builtInType == XML_SCHEMAS_ANYTYPE))
	    return (0);
	return(xmlSchemaIsDerivedFromBuiltInType(type->subtypes, valType));
    }
    return(xmlSchemaIsDerivedFromBuiltInType(type->subtypes, valType));
}

#if 0
static int
xmlSchemaIsUserDerivedFromBuiltInType(xmlSchemaTypePtr type, int valType)
{
    if (type == NULL)
	return (0);
    if (WXS_IS_COMPLEX(type))
	return (0);
    if (type->type == XML_SCHEMA_TYPE_BASIC) {
	if (type->builtInType == valType)
	    return(1);
	return (0);
    } else
	return(xmlSchemaIsDerivedFromBuiltInType(type->subtypes, valType));

    return (0);
}

static xmlSchemaTypePtr
xmlSchemaQueryBuiltInType(xmlSchemaTypePtr type)
{
    if (type == NULL)
	return (NULL);
    if (WXS_IS_COMPLEX(type))
	return (NULL);
    if (type->type == XML_SCHEMA_TYPE_BASIC)
	return(type);
    return(xmlSchemaQueryBuiltInType(type->subtypes));
}
#endif

static xmlSchemaTypePtr
xmlSchemaGetPrimitiveType(xmlSchemaTypePtr type)
{

    while (type != NULL) {
	if ((type->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) ||
	   (type->flags & XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE))
	    return (type);
	type = type->baseType;
    }

    return (NULL);
}

#if 0
static xmlSchemaTypePtr
xmlSchemaGetBuiltInTypeAncestor(xmlSchemaTypePtr type)
{
    if (WXS_IS_LIST(type) || WXS_IS_UNION(type))
	return (0);
    while (type != NULL) {
	if (type->type == XML_SCHEMA_TYPE_BASIC)
	    return (type);
	type = type->baseType;
    }

    return (NULL);
}
#endif

static int
xmlSchemaCloneWildcardNsConstraints(xmlSchemaParserCtxtPtr ctxt,
				    xmlSchemaWildcardPtr dest,
				    xmlSchemaWildcardPtr source)
{
    xmlSchemaWildcardNsPtr cur, tmp, last;

    if ((source == NULL) || (dest == NULL))
	return(-1);
    dest->any = source->any;
    cur = source->nsSet;
    last = NULL;
    while (cur != NULL) {
	tmp = xmlSchemaNewWildcardNsConstraint(ctxt);
	if (tmp == NULL)
	    return(-1);
	tmp->value = cur->value;
	if (last == NULL)
	    dest->nsSet = tmp;
	else
	    last->next = tmp;
	last = tmp;
	cur = cur->next;
    }
    if (dest->negNsSet != NULL)
	xmlSchemaFreeWildcardNsSet(dest->negNsSet);
    if (source->negNsSet != NULL) {
	dest->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
	if (dest->negNsSet == NULL)
	    return(-1);
	dest->negNsSet->value = source->negNsSet->value;
    } else
	dest->negNsSet = NULL;
    return(0);
}

static int
xmlSchemaUnionWildcards(xmlSchemaParserCtxtPtr ctxt,
			    xmlSchemaWildcardPtr completeWild,
			    xmlSchemaWildcardPtr curWild)
{
    xmlSchemaWildcardNsPtr cur, curB, tmp;

    if ((completeWild->any == curWild->any) &&
	((completeWild->nsSet == NULL) == (curWild->nsSet == NULL)) &&
	((completeWild->negNsSet == NULL) == (curWild->negNsSet == NULL))) {

	if ((completeWild->negNsSet == NULL) ||
	    (completeWild->negNsSet->value == curWild->negNsSet->value)) {

	    if (completeWild->nsSet != NULL) {
		int found = 0;

		cur = completeWild->nsSet;
		while (cur != NULL) {
		    found = 0;
		    curB = curWild->nsSet;
		    while (curB != NULL) {
			if (cur->value == curB->value) {
			    found = 1;
			    break;
			}
			curB = curB->next;
		    }
		    if (!found)
			break;
		    cur = cur->next;
		}
		if (found)
		    return(0);
	    } else
		return(0);
	}
    }
    if (completeWild->any != curWild->any) {
	if (completeWild->any == 0) {
	    completeWild->any = 1;
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    if (completeWild->negNsSet != NULL) {
		xmlFree(completeWild->negNsSet);
		completeWild->negNsSet = NULL;
	    }
	}
	return (0);
    }
    if ((completeWild->nsSet != NULL) && (curWild->nsSet != NULL)) {
	int found;
	xmlSchemaWildcardNsPtr start;

	cur = curWild->nsSet;
	start = completeWild->nsSet;
	while (cur != NULL) {
	    found = 0;
	    curB = start;
	    while (curB != NULL) {
		if (cur->value == curB->value) {
		    found = 1;
		    break;
		}
		curB = curB->next;
	    }
	    if (!found) {
		tmp = xmlSchemaNewWildcardNsConstraint(ctxt);
		if (tmp == NULL)
		    return (-1);
		tmp->value = cur->value;
		tmp->next = completeWild->nsSet;
		completeWild->nsSet = tmp;
	    }
	    cur = cur->next;
	}

	return(0);
    }
    if ((completeWild->negNsSet != NULL) &&
	(curWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != curWild->negNsSet->value)) {
	completeWild->negNsSet->value = NULL;

	return(0);
    }
    if (((completeWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != NULL) &&
	(curWild->nsSet != NULL)) ||
	((curWild->negNsSet != NULL) &&
	(curWild->negNsSet->value != NULL) &&
	(completeWild->nsSet != NULL))) {

	int nsFound, absentFound = 0;

	if (completeWild->nsSet != NULL) {
	    cur = completeWild->nsSet;
	    curB = curWild->negNsSet;
	} else {
	    cur = curWild->nsSet;
	    curB = completeWild->negNsSet;
	}
	nsFound = 0;
	while (cur != NULL) {
	    if (cur->value == NULL)
		absentFound = 1;
	    else if (cur->value == curB->value)
		nsFound = 1;
	    if (nsFound && absentFound)
		break;
	    cur = cur->next;
	}

	if (nsFound && absentFound) {
	    completeWild->any = 1;
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    if (completeWild->negNsSet != NULL) {
		xmlFree(completeWild->negNsSet);
		completeWild->negNsSet = NULL;
	    }
	} else if (nsFound && (!absentFound)) {
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    if (completeWild->negNsSet == NULL) {
		completeWild->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
		if (completeWild->negNsSet == NULL)
		    return (-1);
	    }
	    completeWild->negNsSet->value = NULL;
	} else if ((!nsFound) && absentFound) {
	    xmlSchemaPErr(ctxt, completeWild->node,
		XML_SCHEMAP_UNION_NOT_EXPRESSIBLE,
		"The union of the wilcard is not expressible.\n",
		NULL, NULL);
	    return(XML_SCHEMAP_UNION_NOT_EXPRESSIBLE);
	} else if ((!nsFound) && (!absentFound)) {
	    if (completeWild->negNsSet == NULL) {
		if (completeWild->nsSet != NULL) {
		    xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		    completeWild->nsSet = NULL;
		}
		completeWild->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
		if (completeWild->negNsSet == NULL)
		    return (-1);
		completeWild->negNsSet->value = curWild->negNsSet->value;
	    }
	}
	return (0);
    }
    if (((completeWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value == NULL) &&
	(curWild->nsSet != NULL)) ||
	((curWild->negNsSet != NULL) &&
	(curWild->negNsSet->value == NULL) &&
	(completeWild->nsSet != NULL))) {

	if (completeWild->nsSet != NULL) {
	    cur = completeWild->nsSet;
	} else {
	    cur = curWild->nsSet;
	}
	while (cur != NULL) {
	    if (cur->value == NULL) {
		completeWild->any = 1;
		if (completeWild->nsSet != NULL) {
		    xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		    completeWild->nsSet = NULL;
		}
		if (completeWild->negNsSet != NULL) {
		    xmlFree(completeWild->negNsSet);
		    completeWild->negNsSet = NULL;
		}
		return (0);
	    }
	    cur = cur->next;
	}
	if (completeWild->negNsSet == NULL) {
	    if (completeWild->nsSet != NULL) {
		xmlSchemaFreeWildcardNsSet(completeWild->nsSet);
		completeWild->nsSet = NULL;
	    }
	    completeWild->negNsSet = xmlSchemaNewWildcardNsConstraint(ctxt);
	    if (completeWild->negNsSet == NULL)
		return (-1);
	    completeWild->negNsSet->value = NULL;
	}
	return (0);
    }
    return (0);

}

static int
xmlSchemaIntersectWildcards(xmlSchemaParserCtxtPtr ctxt,
			    xmlSchemaWildcardPtr completeWild,
			    xmlSchemaWildcardPtr curWild)
{
    xmlSchemaWildcardNsPtr cur, curB, prev,  tmp;

    if ((completeWild->any == curWild->any) &&
	((completeWild->nsSet == NULL) == (curWild->nsSet == NULL)) &&
	((completeWild->negNsSet == NULL) == (curWild->negNsSet == NULL))) {

	if ((completeWild->negNsSet == NULL) ||
	    (completeWild->negNsSet->value == curWild->negNsSet->value)) {

	    if (completeWild->nsSet != NULL) {
		int found = 0;

		cur = completeWild->nsSet;
		while (cur != NULL) {
		    found = 0;
		    curB = curWild->nsSet;
		    while (curB != NULL) {
			if (cur->value == curB->value) {
			    found = 1;
			    break;
			}
			curB = curB->next;
		    }
		    if (!found)
			break;
		    cur = cur->next;
		}
		if (found)
		    return(0);
	    } else
		return(0);
	}
    }
    if ((completeWild->any != curWild->any) && (completeWild->any)) {
	if (xmlSchemaCloneWildcardNsConstraints(ctxt, completeWild, curWild) == -1)
	    return(-1);
	return(0);
    }
    if (((completeWild->negNsSet != NULL) && (curWild->nsSet != NULL)) ||
	((curWild->negNsSet != NULL) && (completeWild->nsSet != NULL))) {
	const xmlChar *neg;

	if (completeWild->nsSet == NULL) {
	    neg = completeWild->negNsSet->value;
	    if (xmlSchemaCloneWildcardNsConstraints(ctxt, completeWild, curWild) == -1)
		return(-1);
	} else
	    neg = curWild->negNsSet->value;
	prev = NULL;
	cur = completeWild->nsSet;
	while (cur != NULL) {
	    if (cur->value == NULL) {
		if (prev == NULL)
		    completeWild->nsSet = cur->next;
		else
		    prev->next = cur->next;
		xmlFree(cur);
		break;
	    }
	    prev = cur;
	    cur = cur->next;
	}
	if (neg != NULL) {
	    prev = NULL;
	    cur = completeWild->nsSet;
	    while (cur != NULL) {
		if (cur->value == neg) {
		    if (prev == NULL)
			completeWild->nsSet = cur->next;
		    else
			prev->next = cur->next;
		    xmlFree(cur);
		    break;
		}
		prev = cur;
		cur = cur->next;
	    }
	}

	return(0);
    }
    if ((completeWild->nsSet != NULL) && (curWild->nsSet != NULL)) {
	int found;

	cur = completeWild->nsSet;
	prev = NULL;
	while (cur != NULL) {
	    found = 0;
	    curB = curWild->nsSet;
	    while (curB != NULL) {
		if (cur->value == curB->value) {
		    found = 1;
		    break;
		}
		curB = curB->next;
	    }
	    if (!found) {
		if (prev == NULL)
		    completeWild->nsSet = cur->next;
		else
		    prev->next = cur->next;
		tmp = cur->next;
		xmlFree(cur);
		cur = tmp;
		continue;
	    }
	    prev = cur;
	    cur = cur->next;
	}

	return(0);
    }
    if ((completeWild->negNsSet != NULL) &&
	(curWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != curWild->negNsSet->value) &&
	(completeWild->negNsSet->value != NULL) &&
	(curWild->negNsSet->value != NULL)) {

	xmlSchemaPErr(ctxt, completeWild->node, XML_SCHEMAP_INTERSECTION_NOT_EXPRESSIBLE,
	    "The intersection of the wilcard is not expressible.\n",
	    NULL, NULL);
	return(XML_SCHEMAP_INTERSECTION_NOT_EXPRESSIBLE);
    }
    if ((completeWild->negNsSet != NULL) && (curWild->negNsSet != NULL) &&
	(completeWild->negNsSet->value != curWild->negNsSet->value) &&
	(completeWild->negNsSet->value == NULL)) {
	completeWild->negNsSet->value =  curWild->negNsSet->value;
    }
    return(0);
}

static int
xmlSchemaCheckCOSNSSubset(xmlSchemaWildcardPtr sub,
			  xmlSchemaWildcardPtr super)
{
    if (super->any)
	return (0);
    if ((sub->negNsSet != NULL) &&
	(super->negNsSet != NULL) &&
	(sub->negNsSet->value == super->negNsSet->value))
	return (0);
    if (sub->nsSet != NULL) {
	if (super->nsSet != NULL) {
	    xmlSchemaWildcardNsPtr cur, curB;
	    int found = 0;

	    cur = sub->nsSet;
	    while (cur != NULL) {
		found = 0;
		curB = super->nsSet;
		while (curB != NULL) {
		    if (cur->value == curB->value) {
			found = 1;
			break;
		    }
		    curB = curB->next;
		}
		if (!found)
		    return (1);
		cur = cur->next;
	    }
	    if (found)
		return (0);
	} else if (super->negNsSet != NULL) {
	    xmlSchemaWildcardNsPtr cur;
	    cur = sub->nsSet;
	    while (cur != NULL) {
		if (cur->value == super->negNsSet->value)
		    return (1);
		cur = cur->next;
	    }
	    return (0);
	}
    }
    return (1);
}

static int
xmlSchemaGetEffectiveValueConstraint(xmlSchemaAttributeUsePtr attruse,
				     int *fixed,
				     const xmlChar **value,
				     xmlSchemaValPtr *val)
{
    *fixed = 0;
    *value = NULL;
    if (val != 0)
	*val = NULL;

    if (attruse->defValue != NULL) {
	*value = attruse->defValue;
	if (val != NULL)
	    *val = attruse->defVal;
	if (attruse->flags & XML_SCHEMA_ATTR_USE_FIXED)
	    *fixed = 1;
	return(1);
    } else if ((attruse->attrDecl != NULL) &&
	(attruse->attrDecl->defValue != NULL)) {
	*value = attruse->attrDecl->defValue;
	if (val != NULL)
	    *val = attruse->attrDecl->defVal;
	if (attruse->attrDecl->flags & XML_SCHEMAS_ATTR_FIXED)
	    *fixed = 1;
	return(1);
    }
    return(0);
}
static int
xmlSchemaCheckCVCWildcardNamespace(xmlSchemaWildcardPtr wild,
				   const xmlChar* ns)
{
    if (wild == NULL)
	return(-1);

    if (wild->any)
	return(0);
    else if (wild->nsSet != NULL) {
	xmlSchemaWildcardNsPtr cur;

	cur = wild->nsSet;
	while (cur != NULL) {
	    if (xmlStrEqual(cur->value, ns))
		return(0);
	    cur = cur->next;
	}
    } else if ((wild->negNsSet != NULL) && (ns != NULL) &&
	(!xmlStrEqual(wild->negNsSet->value, ns)))
	return(0);

    return(1);
}

#define XML_SCHEMA_ACTION_DERIVE 0
#define XML_SCHEMA_ACTION_REDEFINE 1

#define WXS_ACTION_STR(a) \
((a) == XML_SCHEMA_ACTION_DERIVE) ? (const xmlChar *) "base" : (const xmlChar *) "redefined"

static int
xmlSchemaCheckDerivationOKRestriction2to4(xmlSchemaParserCtxtPtr pctxt,
				       int action,
				       xmlSchemaBasicItemPtr item,
				       xmlSchemaBasicItemPtr baseItem,
				       xmlSchemaItemListPtr uses,
				       xmlSchemaItemListPtr baseUses,
				       xmlSchemaWildcardPtr wild,
				       xmlSchemaWildcardPtr baseWild)
{
    xmlSchemaAttributeUsePtr cur = NULL, bcur;
    int i, j, found; 
    const xmlChar *bEffValue;
    int effFixed;

    if (uses != NULL) {
	for (i = 0; i < uses->nbItems; i++) {
	    cur = uses->items[i];
	    found = 0;
	    if (baseUses == NULL)
		goto not_found;
	    for (j = 0; j < baseUses->nbItems; j++) {
		bcur = baseUses->items[j];
		if ((WXS_ATTRUSE_DECL_NAME(cur) ==
			WXS_ATTRUSE_DECL_NAME(bcur)) &&
		    (WXS_ATTRUSE_DECL_TNS(cur) ==
			WXS_ATTRUSE_DECL_TNS(bcur)))
		{
		    found = 1;

		    if ((cur->occurs == XML_SCHEMAS_ATTR_USE_OPTIONAL) &&
			(bcur->occurs == XML_SCHEMAS_ATTR_USE_REQUIRED))
		    {
			xmlChar *str = NULL;
			xmlSchemaPAttrUseErr4(pctxt,
			    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_1,
			    WXS_ITEM_NODE(item), item, cur,
			    "The 'optional' attribute use is inconsistent "
			    "with the corresponding 'required' attribute use of "
			    "the %s %s",
			    WXS_ACTION_STR(action),
			    xmlSchemaGetComponentDesignation(&str, baseItem),
			    NULL, NULL);
			FREE_AND_NULL(str);
			
		    } else if (xmlSchemaCheckCOSSTDerivedOK(ACTXT_CAST pctxt,
			WXS_ATTRUSE_TYPEDEF(cur),
			WXS_ATTRUSE_TYPEDEF(bcur), 0) != 0)
		    {
			xmlChar *strA = NULL, *strB = NULL, *strC = NULL;

			xmlSchemaPAttrUseErr4(pctxt,
			    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_2,
			    WXS_ITEM_NODE(item), item, cur,
			    "The attribute declaration's %s "
			    "is not validly derived from "
			    "the corresponding %s of the "
			    "attribute declaration in the %s %s",
			    xmlSchemaGetComponentDesignation(&strA,
				WXS_ATTRUSE_TYPEDEF(cur)),
			    xmlSchemaGetComponentDesignation(&strB,
				WXS_ATTRUSE_TYPEDEF(bcur)),
			    WXS_ACTION_STR(action),
			    xmlSchemaGetComponentDesignation(&strC, baseItem));
			    
			FREE_AND_NULL(strA);
			FREE_AND_NULL(strB);
			FREE_AND_NULL(strC);
			
		    } else {
			xmlSchemaGetEffectiveValueConstraint(bcur,
			    &effFixed, &bEffValue, NULL);
			if ((bEffValue != NULL) &&
			    (effFixed == 1)) {
			    const xmlChar *rEffValue = NULL;

			    xmlSchemaGetEffectiveValueConstraint(bcur,
				&effFixed, &rEffValue, NULL);
			    if ((effFixed == 0) ||
				(! WXS_ARE_DEFAULT_STR_EQUAL(rEffValue, bEffValue)))
			    {
				xmlChar *str = NULL;

				xmlSchemaPAttrUseErr4(pctxt,
				    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_1_3,
				    WXS_ITEM_NODE(item), item, cur,
				    "The effective value constraint of the "
				    "attribute use is inconsistent with "
				    "its correspondent in the %s %s",
				    WXS_ACTION_STR(action),
				    xmlSchemaGetComponentDesignation(&str,
					baseItem),
				    NULL, NULL);
				FREE_AND_NULL(str);
				
			    }
			}
		    }
		    break;
		}
	    }
not_found:
	    if (!found) {
		if ((baseWild == NULL) ||
		    (xmlSchemaCheckCVCWildcardNamespace(baseWild,
		    (WXS_ATTRUSE_DECL(cur))->targetNamespace) != 0))
		{
		    xmlChar *str = NULL;

		    xmlSchemaPAttrUseErr4(pctxt,
			XML_SCHEMAP_DERIVATION_OK_RESTRICTION_2_2,
			WXS_ITEM_NODE(item), item, cur,
			"Neither a matching attribute use, "
			"nor a matching wildcard exists in the %s %s",
			WXS_ACTION_STR(action),
			xmlSchemaGetComponentDesignation(&str, baseItem),
			NULL, NULL);
		    FREE_AND_NULL(str);
		    
		}
	    }
	}
    }
    if (baseUses != NULL) {
	for (j = 0; j < baseUses->nbItems; j++) {
	    bcur = baseUses->items[j];
	    if (bcur->occurs != XML_SCHEMAS_ATTR_USE_REQUIRED)
		continue;
	    found = 0;
	    if (uses != NULL) {
		for (i = 0; i < uses->nbItems; i++) {
		    cur = uses->items[i];
		    if ((WXS_ATTRUSE_DECL_NAME(cur) ==
			WXS_ATTRUSE_DECL_NAME(bcur)) &&
			(WXS_ATTRUSE_DECL_TNS(cur) ==
			WXS_ATTRUSE_DECL_TNS(bcur))) {
			found = 1;
			break;
		    }
		}
	    }
	    if (!found) {
		xmlChar *strA = NULL, *strB = NULL;

		xmlSchemaCustomErr4(ACTXT_CAST pctxt,
		    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_3,
		    NULL, item,
		    "A matching attribute use for the "
		    "'required' %s of the %s %s is missing",
		    xmlSchemaGetComponentDesignation(&strA, bcur),
		    WXS_ACTION_STR(action),
		    xmlSchemaGetComponentDesignation(&strB, baseItem),
		    NULL);
		FREE_AND_NULL(strA);
		FREE_AND_NULL(strB);
	    }
	}
    }
    if (wild != NULL) {
	if (baseWild == NULL) {
	    xmlChar *str = NULL;

	    xmlSchemaCustomErr4(ACTXT_CAST pctxt,
		XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_1,
		NULL, item,
		"The %s has an attribute wildcard, "
		"but the %s %s '%s' does not have one",
		WXS_ITEM_TYPE_NAME(item),
		WXS_ACTION_STR(action),
		WXS_ITEM_TYPE_NAME(baseItem),
		xmlSchemaGetComponentQName(&str, baseItem));
	    FREE_AND_NULL(str);
	    return(pctxt->err);
	} else if ((baseWild->any == 0) &&
		xmlSchemaCheckCOSNSSubset(wild, baseWild))
	{
	    xmlChar *str = NULL;
	    xmlSchemaCustomErr4(ACTXT_CAST pctxt,
		XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_2,
		NULL, item,
		"The attribute wildcard is not a valid "
		"subset of the wildcard in the %s %s '%s'",
		WXS_ACTION_STR(action),
		WXS_ITEM_TYPE_NAME(baseItem),
		xmlSchemaGetComponentQName(&str, baseItem),
		NULL);
	    FREE_AND_NULL(str);
	    return(pctxt->err);
	}
	if ((! WXS_IS_ANYTYPE(baseItem)) &&
	    (wild->processContents < baseWild->processContents)) {
	    xmlChar *str = NULL;
	    xmlSchemaCustomErr4(ACTXT_CAST pctxt,
		XML_SCHEMAP_DERIVATION_OK_RESTRICTION_4_3,
		NULL, baseItem,
		"The {process contents} of the attribute wildcard is "
		"weaker than the one in the %s %s '%s'",
		WXS_ACTION_STR(action),
		WXS_ITEM_TYPE_NAME(baseItem),
		xmlSchemaGetComponentQName(&str, baseItem),
		NULL);
	    FREE_AND_NULL(str)
		return(pctxt->err);
	}
    }
    return(0);
}


static int
xmlSchemaExpandAttributeGroupRefs(xmlSchemaParserCtxtPtr pctxt,
				  xmlSchemaBasicItemPtr item,
				  xmlSchemaWildcardPtr *completeWild,
				  xmlSchemaItemListPtr list,
				  xmlSchemaItemListPtr prohibs);
static int
xmlSchemaFixupTypeAttributeUses(xmlSchemaParserCtxtPtr pctxt,
				  xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr baseType = NULL;
    xmlSchemaAttributeUsePtr use;
    xmlSchemaItemListPtr uses, baseUses, prohibs = NULL;

    if (type->baseType == NULL) {
	PERROR_INT("xmlSchemaFixupTypeAttributeUses",
	    "no base type");
        return (-1);
    }
    baseType = type->baseType;
    if (WXS_IS_TYPE_NOT_FIXED(baseType))
	if (xmlSchemaTypeFixup(baseType, ACTXT_CAST pctxt) == -1)
	    return(-1);

    uses = type->attrUses;
    baseUses = baseType->attrUses;
    if (uses != NULL) {
	if (WXS_IS_RESTRICTION(type)) {
	    if (xmlSchemaExpandAttributeGroupRefs(pctxt,
		WXS_BASIC_CAST type, &(type->attributeWildcard), uses,
		pctxt->attrProhibs) == -1)
	    {
		PERROR_INT("xmlSchemaFixupTypeAttributeUses",
		"failed to expand attributes");
	    }
	    if (pctxt->attrProhibs->nbItems != 0)
		prohibs = pctxt->attrProhibs;
	} else {
	    if (xmlSchemaExpandAttributeGroupRefs(pctxt,
		WXS_BASIC_CAST type, &(type->attributeWildcard), uses,
		NULL) == -1)
	    {
		PERROR_INT("xmlSchemaFixupTypeAttributeUses",
		"failed to expand attributes");
	    }
	}
    }
    if (baseUses != NULL) {
	int i, j;
	xmlSchemaAttributeUseProhibPtr pro;

	if (WXS_IS_RESTRICTION(type)) {
	    int usesCount;
	    xmlSchemaAttributeUsePtr tmp;

	    if (uses != NULL)
		usesCount = uses->nbItems;
	    else
		usesCount = 0;

	    
	    for (i = 0; i < baseUses->nbItems; i++) {
		use = baseUses->items[i];
		if (prohibs) {
		    for (j = 0; j < prohibs->nbItems; j++) {
			pro = prohibs->items[j];
			if ((WXS_ATTRUSE_DECL_NAME(use) == pro->name) &&
			    (WXS_ATTRUSE_DECL_TNS(use) ==
				pro->targetNamespace))
			{
			    goto inherit_next;
			}
		    }
		}
		if (usesCount) {
		    for (j = 0; j < usesCount; j++) {
			tmp = uses->items[j];
			if ((WXS_ATTRUSE_DECL_NAME(use) ==
				WXS_ATTRUSE_DECL_NAME(tmp)) &&
			    (WXS_ATTRUSE_DECL_TNS(use) ==
				WXS_ATTRUSE_DECL_TNS(tmp)))
			{
			    goto inherit_next;
			}
		    }
		}
		if (uses == NULL) {
		    type->attrUses = xmlSchemaItemListCreate();
		    if (type->attrUses == NULL)
			goto exit_failure;
		    uses = type->attrUses;
		}
		xmlSchemaItemListAddSize(uses, 2, use);
inherit_next: {}
	    }
	} else {
	    
	    for (i = 0; i < baseUses->nbItems; i++) {
		use = baseUses->items[i];
		if (uses == NULL) {
		    type->attrUses = xmlSchemaItemListCreate();
		    if (type->attrUses == NULL)
			goto exit_failure;
		    uses = type->attrUses;
		}
		xmlSchemaItemListAddSize(uses, baseUses->nbItems, use);
	    }
	}
    }
    if (uses) {
	if (uses->nbItems == 0) {
	    xmlSchemaItemListFree(uses);
	    type->attrUses = NULL;
	}
    }
    if (WXS_IS_EXTENSION(type)) {
	if (baseType->attributeWildcard != NULL) {
	    if (type->attributeWildcard != NULL) {
		if (xmlSchemaUnionWildcards(pctxt, type->attributeWildcard,
		    baseType->attributeWildcard) == -1)
		    goto exit_failure;
	    } else {
		type->attributeWildcard = baseType->attributeWildcard;
	    }
	} else {
	}
    } else {
    }

    return (0);

exit_failure:
    return(-1);
}

static int
xmlSchemaTypeFinalContains(xmlSchemaTypePtr type, int final)
{
    if (type == NULL)
	return (0);
    if (type->flags & final)
	return (1);
    else
	return (0);
}

static xmlSchemaTypeLinkPtr
xmlSchemaGetUnionSimpleTypeMemberTypes(xmlSchemaTypePtr type)
{
    while ((type != NULL) && (type->type == XML_SCHEMA_TYPE_SIMPLE)) {
	if (type->memberTypes != NULL)
	    return (type->memberTypes);
	else
	    type = type->baseType;
    }
    return (NULL);
}

static int
xmlSchemaGetParticleTotalRangeMin(xmlSchemaParticlePtr particle)
{
    if ((particle->children == NULL) ||
	(particle->minOccurs == 0))
	return (0);
    if (particle->children->type == XML_SCHEMA_TYPE_CHOICE) {
	int min = -1, cur;
	xmlSchemaParticlePtr part =
	    (xmlSchemaParticlePtr) particle->children->children;

	if (part == NULL)
	    return (0);
	while (part != NULL) {
	    if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
		(part->children->type == XML_SCHEMA_TYPE_ANY))
		cur = part->minOccurs;
	    else
		cur = xmlSchemaGetParticleTotalRangeMin(part);
	    if (cur == 0)
		return (0);
	    if ((min > cur) || (min == -1))
		min = cur;
	    part = (xmlSchemaParticlePtr) part->next;
	}
	return (particle->minOccurs * min);
    } else {
	
	int sum = 0;
	xmlSchemaParticlePtr part =
	    (xmlSchemaParticlePtr) particle->children->children;

	if (part == NULL)
	    return (0);
	do {
	    if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
		(part->children->type == XML_SCHEMA_TYPE_ANY))
		sum += part->minOccurs;
	    else
		sum += xmlSchemaGetParticleTotalRangeMin(part);
	    part = (xmlSchemaParticlePtr) part->next;
	} while (part != NULL);
	return (particle->minOccurs * sum);
    }
}

#if 0
static int
xmlSchemaGetParticleTotalRangeMax(xmlSchemaParticlePtr particle)
{
    if ((particle->children == NULL) ||
	(particle->children->children == NULL))
	return (0);
    if (particle->children->type == XML_SCHEMA_TYPE_CHOICE) {
	int max = -1, cur;
	xmlSchemaParticlePtr part =
	    (xmlSchemaParticlePtr) particle->children->children;

	for (; part != NULL; part = (xmlSchemaParticlePtr) part->next) {
	    if (part->children == NULL)
		continue;
	    if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
		(part->children->type == XML_SCHEMA_TYPE_ANY))
		cur = part->maxOccurs;
	    else
		cur = xmlSchemaGetParticleTotalRangeMax(part);
	    if (cur == UNBOUNDED)
		return (UNBOUNDED);
	    if ((max < cur) || (max == -1))
		max = cur;
	}
	
	return (particle->maxOccurs * max);
    } else {
	
	int sum = 0, cur;
	xmlSchemaParticlePtr part =
	    (xmlSchemaParticlePtr) particle->children->children;

	for (; part != NULL; part = (xmlSchemaParticlePtr) part->next) {
	    if (part->children == NULL)
		continue;
	    if ((part->children->type == XML_SCHEMA_TYPE_ELEMENT) ||
		(part->children->type == XML_SCHEMA_TYPE_ANY))
		cur = part->maxOccurs;
	    else
		cur = xmlSchemaGetParticleTotalRangeMax(part);
	    if (cur == UNBOUNDED)
		return (UNBOUNDED);
	    if ((cur > 0) && (particle->maxOccurs == UNBOUNDED))
		return (UNBOUNDED);
	    sum += cur;
	}
	
	return (particle->maxOccurs * sum);
    }
}
#endif

static int
xmlSchemaIsParticleEmptiable(xmlSchemaParticlePtr particle)
{
    if ((particle == NULL) || (particle->minOccurs == 0) ||
	(particle->children == NULL))
	return (1);
    if (WXS_IS_MODEL_GROUP(particle->children)) {
	if (xmlSchemaGetParticleTotalRangeMin(particle) == 0)
	    return (1);
    }
    return (0);
}

static int
xmlSchemaCheckCOSSTDerivedOK(xmlSchemaAbstractCtxtPtr actxt,
			     xmlSchemaTypePtr type,
			     xmlSchemaTypePtr baseType,
			     int subset)
{
    if (type == baseType)
	return (0);
    if (WXS_IS_TYPE_NOT_FIXED(type))
	if (xmlSchemaTypeFixup(type, actxt) == -1)
	    return(-1);
    if (WXS_IS_TYPE_NOT_FIXED(baseType))
	if (xmlSchemaTypeFixup(baseType, actxt) == -1)
	    return(-1);
    if ((subset & SUBSET_RESTRICTION) ||
	(xmlSchemaTypeFinalContains(type->baseType,
	    XML_SCHEMAS_TYPE_FINAL_RESTRICTION))) {
	return (XML_SCHEMAP_COS_ST_DERIVED_OK_2_1);
    }
    
    if (type->baseType == baseType) {
	return (0);
    }
    if ((! WXS_IS_ANYTYPE(type->baseType)) &&
	(xmlSchemaCheckCOSSTDerivedOK(actxt, type->baseType,
	    baseType, subset) == 0)) {
	return (0);
    }
    if (WXS_IS_ANY_SIMPLE_TYPE(baseType) &&
	(WXS_IS_LIST(type) || WXS_IS_UNION(type))) {
	return (0);
    }
    if (WXS_IS_UNION(baseType)) {
	xmlSchemaTypeLinkPtr cur;

	cur = baseType->memberTypes;
	while (cur != NULL) {
	    if (WXS_IS_TYPE_NOT_FIXED(cur->type))
		if (xmlSchemaTypeFixup(cur->type, actxt) == -1)
		    return(-1);
	    if (xmlSchemaCheckCOSSTDerivedOK(actxt,
		    type, cur->type, subset) == 0)
	    {
		return (0);
	    }
	    cur = cur->next;
	}
    }
    return (XML_SCHEMAP_COS_ST_DERIVED_OK_2_2);
}

static int
xmlSchemaCheckTypeDefCircularInternal(xmlSchemaParserCtxtPtr pctxt,
			   xmlSchemaTypePtr ctxtType,
			   xmlSchemaTypePtr ancestor)
{
    int ret;

    if ((ancestor == NULL) || (ancestor->type == XML_SCHEMA_TYPE_BASIC))
	return (0);

    if (ctxtType == ancestor) {
	xmlSchemaPCustomErr(pctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_2,
	    WXS_BASIC_CAST ctxtType, WXS_ITEM_NODE(ctxtType),
	    "The definition is circular", NULL);
	return (XML_SCHEMAP_ST_PROPS_CORRECT_2);
    }
    if (ancestor->flags & XML_SCHEMAS_TYPE_MARKED) {
	return (0);
    }
    ancestor->flags |= XML_SCHEMAS_TYPE_MARKED;
    ret = xmlSchemaCheckTypeDefCircularInternal(pctxt, ctxtType,
	ancestor->baseType);
    ancestor->flags ^= XML_SCHEMAS_TYPE_MARKED;
    return (ret);
}

static void
xmlSchemaCheckTypeDefCircular(xmlSchemaTypePtr item,
			      xmlSchemaParserCtxtPtr ctxt)
{
    if ((item == NULL) ||
	(item->type == XML_SCHEMA_TYPE_BASIC) ||
	(item->baseType == NULL))
	return;
    xmlSchemaCheckTypeDefCircularInternal(ctxt, item,
	item->baseType);
}

static int
xmlSchemaCheckUnionTypeDefCircularRecur(xmlSchemaParserCtxtPtr pctxt,
					xmlSchemaTypePtr ctxType,
					xmlSchemaTypeLinkPtr members)
{
    xmlSchemaTypeLinkPtr member;
    xmlSchemaTypePtr memberType;

    member = members;
    while (member != NULL) {
	memberType = member->type;
	while ((memberType != NULL) &&
	    (memberType->type != XML_SCHEMA_TYPE_BASIC)) {
	    if (memberType == ctxType) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_SRC_SIMPLE_TYPE_4,
		    WXS_BASIC_CAST ctxType, NULL,
		    "The union type definition is circular", NULL);
		return (XML_SCHEMAP_SRC_SIMPLE_TYPE_4);
	    }
	    if ((WXS_IS_UNION(memberType)) &&
		((memberType->flags & XML_SCHEMAS_TYPE_MARKED) == 0))
	    {
		int res;
		memberType->flags |= XML_SCHEMAS_TYPE_MARKED;
		res = xmlSchemaCheckUnionTypeDefCircularRecur(pctxt,
		    ctxType,
		    xmlSchemaGetUnionSimpleTypeMemberTypes(memberType));
		memberType->flags ^= XML_SCHEMAS_TYPE_MARKED;
		if (res != 0)
		    return(res);
	    }
	    memberType = memberType->baseType;
	}
	member = member->next;
    }
    return(0);
}

static int
xmlSchemaCheckUnionTypeDefCircular(xmlSchemaParserCtxtPtr pctxt,
				   xmlSchemaTypePtr type)
{
    if (! WXS_IS_UNION(type))
	return(0);
    return(xmlSchemaCheckUnionTypeDefCircularRecur(pctxt, type,
	type->memberTypes));
}

static void
xmlSchemaResolveTypeReferences(xmlSchemaTypePtr typeDef,
			 xmlSchemaParserCtxtPtr ctxt)
{
    if (typeDef == NULL)
	return;

    if (typeDef->baseType == NULL) {
	typeDef->baseType = xmlSchemaGetType(ctxt->schema,
	    typeDef->base, typeDef->baseNs);
	if (typeDef->baseType == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST typeDef, typeDef->node,
		"base", typeDef->base, typeDef->baseNs,
		XML_SCHEMA_TYPE_SIMPLE, NULL);
	    return;
	}
    }
    if (WXS_IS_SIMPLE(typeDef)) {
	if (WXS_IS_UNION(typeDef)) {
	    xmlSchemaResolveUnionMemberTypes(ctxt, typeDef);
	    return;
	} else if (WXS_IS_LIST(typeDef)) {
	    if ((typeDef->subtypes == NULL) && (typeDef->base != NULL)) {

		typeDef->subtypes = xmlSchemaGetType(ctxt->schema,
		    typeDef->base, typeDef->baseNs);

		if ((typeDef->subtypes == NULL) ||
		    (! WXS_IS_SIMPLE(typeDef->subtypes)))
		{
		    typeDef->subtypes = NULL;
		    xmlSchemaPResCompAttrErr(ctxt,
			XML_SCHEMAP_SRC_RESOLVE,
			WXS_BASIC_CAST typeDef, typeDef->node,
			"itemType", typeDef->base, typeDef->baseNs,
			XML_SCHEMA_TYPE_SIMPLE, NULL);
		}
	    }
	    return;
	}
    }
    else if ((WXS_TYPE_CONTENTTYPE(typeDef) != NULL) &&
	((WXS_TYPE_CONTENTTYPE(typeDef))->type ==
	    XML_SCHEMA_TYPE_PARTICLE) &&
	(WXS_TYPE_PARTICLE_TERM(typeDef) != NULL) &&
	((WXS_TYPE_PARTICLE_TERM(typeDef))->type ==
	    XML_SCHEMA_EXTRA_QNAMEREF))
    {
	xmlSchemaQNameRefPtr ref =
	    WXS_QNAME_CAST WXS_TYPE_PARTICLE_TERM(typeDef);
	xmlSchemaModelGroupDefPtr groupDef;

	WXS_TYPE_PARTICLE_TERM(typeDef) = NULL;
	groupDef =
	    WXS_MODEL_GROUPDEF_CAST xmlSchemaGetNamedComponent(ctxt->schema,
		ref->itemType, ref->name, ref->targetNamespace);
	if (groupDef == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt, XML_SCHEMAP_SRC_RESOLVE,
		NULL, WXS_ITEM_NODE(WXS_TYPE_PARTICLE(typeDef)),
		"ref", ref->name, ref->targetNamespace, ref->itemType,
		NULL);
	    
	    WXS_TYPE_CONTENTTYPE(typeDef) = NULL;
	} else if (WXS_MODELGROUPDEF_MODEL(groupDef) == NULL)
	    
	    WXS_TYPE_CONTENTTYPE(typeDef) = NULL;
	else {
	    WXS_TYPE_PARTICLE_TERM(typeDef) = WXS_MODELGROUPDEF_MODEL(groupDef);

	    if (WXS_MODELGROUPDEF_MODEL(groupDef)->type == XML_SCHEMA_TYPE_ALL) {
		if ((WXS_TYPE_PARTICLE(typeDef))->maxOccurs != 1) {
		    xmlSchemaCustomErr(ACTXT_CAST ctxt,
			
			XML_SCHEMAP_COS_ALL_LIMITED,
			WXS_ITEM_NODE(WXS_TYPE_PARTICLE(typeDef)), NULL,
			"The particle's {max occurs} must be 1, since the "
			"reference resolves to an 'all' model group",
			NULL, NULL);
		}
	    }
	}
    }
}



static int
xmlSchemaCheckSTPropsCorrect(xmlSchemaParserCtxtPtr ctxt,
			     xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr baseType = type->baseType;
    xmlChar *str = NULL;

    
    if (baseType == NULL) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    WXS_BASIC_CAST type, NULL,
	    "No base type existent", NULL);
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);

    }
    if (! WXS_IS_SIMPLE(baseType)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    WXS_BASIC_CAST type, NULL,
	    "The base type '%s' is not a simple type",
	    xmlSchemaGetComponentQName(&str, baseType));
	FREE_AND_NULL(str)
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);
    }
    if ( (WXS_IS_LIST(type) || WXS_IS_UNION(type)) &&
	 (WXS_IS_RESTRICTION(type) == 0) &&
	 (! WXS_IS_ANY_SIMPLE_TYPE(baseType))) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    WXS_BASIC_CAST type, NULL,
	    "A type, derived by list or union, must have "
	    "the simple ur-type definition as base type, not '%s'",
	    xmlSchemaGetComponentQName(&str, baseType));
	FREE_AND_NULL(str)
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);
    }
    if ((! WXS_IS_ATOMIC(type)) && (! WXS_IS_UNION(type)) &&
	(! WXS_IS_LIST(type))) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_1,
	    WXS_BASIC_CAST type, NULL,
	    "The variety is absent", NULL);
	return (XML_SCHEMAP_ST_PROPS_CORRECT_1);
    }
    

    if (xmlSchemaTypeFinalContains(baseType,
	XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_ST_PROPS_CORRECT_3,
	    WXS_BASIC_CAST type, NULL,
	    "The 'final' of its base type '%s' must not contain "
	    "'restriction'",
	    xmlSchemaGetComponentQName(&str, baseType));
	FREE_AND_NULL(str)
	return (XML_SCHEMAP_ST_PROPS_CORRECT_3);
    }

    return (0);
}

static int
xmlSchemaCheckCOSSTRestricts(xmlSchemaParserCtxtPtr pctxt,
			     xmlSchemaTypePtr type)
{
    xmlChar *str = NULL;

    if (type->type != XML_SCHEMA_TYPE_SIMPLE) {
	PERROR_INT("xmlSchemaCheckCOSSTRestricts",
	    "given type is not a user-derived simpleType");
	return (-1);
    }

    if (WXS_IS_ATOMIC(type)) {
	xmlSchemaTypePtr primitive;
	if (! WXS_IS_ATOMIC(type->baseType)) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_COS_ST_RESTRICTS_1_1,
		WXS_BASIC_CAST type, NULL,
		"The base type '%s' is not an atomic simple type",
		xmlSchemaGetComponentQName(&str, type->baseType));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_COS_ST_RESTRICTS_1_1);
	}
	
	if (xmlSchemaTypeFinalContains(type->baseType,
	    XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_COS_ST_RESTRICTS_1_2,
		WXS_BASIC_CAST type, NULL,
		"The final of its base type '%s' must not contain 'restriction'",
		xmlSchemaGetComponentQName(&str, type->baseType));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_COS_ST_RESTRICTS_1_2);
	}

	if (type->facets != NULL) {
	    xmlSchemaFacetPtr facet;
	    int ok = 1;

	    primitive = xmlSchemaGetPrimitiveType(type);
	    if (primitive == NULL) {
		PERROR_INT("xmlSchemaCheckCOSSTRestricts",
		    "failed to get primitive type");
		return (-1);
	    }
	    facet = type->facets;
	    do {
		if (xmlSchemaIsBuiltInTypeFacet(primitive, facet->type) == 0) {
		    ok = 0;
		    xmlSchemaPIllegalFacetAtomicErr(pctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_1_3_1,
			type, primitive, facet);
		}
		facet = facet->next;
	    } while (facet != NULL);
	    if (ok == 0)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_1_3_1);
	}
    } else if (WXS_IS_LIST(type)) {
	xmlSchemaTypePtr itemType = NULL;

	itemType = type->subtypes;
	if ((itemType == NULL) || (! WXS_IS_SIMPLE(itemType))) {
	    PERROR_INT("xmlSchemaCheckCOSSTRestricts",
		"failed to evaluate the item type");
	    return (-1);
	}
	if (WXS_IS_TYPE_NOT_FIXED(itemType))
	    xmlSchemaTypeFixup(itemType, ACTXT_CAST pctxt);
	if ((! WXS_IS_ATOMIC(itemType)) &&
	    (! WXS_IS_UNION(itemType))) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_COS_ST_RESTRICTS_2_1,
		WXS_BASIC_CAST type, NULL,
		"The item type '%s' does not have a variety of atomic or union",
		xmlSchemaGetComponentQName(&str, itemType));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_1);
	} else if (WXS_IS_UNION(itemType)) {
	    xmlSchemaTypeLinkPtr member;

	    member = itemType->memberTypes;
	    while (member != NULL) {
		if (! WXS_IS_ATOMIC(member->type)) {
		    xmlSchemaPCustomErr(pctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_2_1,
			WXS_BASIC_CAST type, NULL,
			"The item type is a union type, but the "
			"member type '%s' of this item type is not atomic",
			xmlSchemaGetComponentQName(&str, member->type));
		    FREE_AND_NULL(str)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_1);
		}
		member = member->next;
	    }
	}

	if (WXS_IS_ANY_SIMPLE_TYPE(type->baseType)) {
	    xmlSchemaFacetPtr facet;
	    if (xmlSchemaTypeFinalContains(itemType,
		XML_SCHEMAS_TYPE_FINAL_LIST)) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_1,
		    WXS_BASIC_CAST type, NULL,
		    "The final of its item type '%s' must not contain 'list'",
		    xmlSchemaGetComponentQName(&str, itemType));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_1);
	    }
	    if (type->facets != NULL) {
		facet = type->facets;
		do {
		    if (facet->type != XML_SCHEMA_FACET_WHITESPACE) {
			xmlSchemaPIllegalFacetListUnionErr(pctxt,
			    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_2,
			    type, facet);
			return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_1_2);
		    }
		    facet = facet->next;
		} while (facet != NULL);
	    }
	} else {
	    if (! WXS_IS_LIST(type->baseType)) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_1,
		    WXS_BASIC_CAST type, NULL,
		    "The base type '%s' must be a list type",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_1);
	    }
	    if (xmlSchemaTypeFinalContains(type->baseType,
		XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_2,
		    WXS_BASIC_CAST type, NULL,
		    "The 'final' of the base type '%s' must not contain 'restriction'",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_2);
	    }
	    {
		xmlSchemaTypePtr baseItemType;

		baseItemType = type->baseType->subtypes;
		if ((baseItemType == NULL) || (! WXS_IS_SIMPLE(baseItemType))) {
		    PERROR_INT("xmlSchemaCheckCOSSTRestricts",
			"failed to eval the item type of a base type");
		    return (-1);
		}
		if ((itemType != baseItemType) &&
		    (xmlSchemaCheckCOSSTDerivedOK(ACTXT_CAST pctxt, itemType,
			baseItemType, 0) != 0)) {
		    xmlChar *strBIT = NULL, *strBT = NULL;
		    xmlSchemaPCustomErrExt(pctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_3,
			WXS_BASIC_CAST type, NULL,
			"The item type '%s' is not validly derived from "
			"the item type '%s' of the base type '%s'",
			xmlSchemaGetComponentQName(&str, itemType),
			xmlSchemaGetComponentQName(&strBIT, baseItemType),
			xmlSchemaGetComponentQName(&strBT, type->baseType));

		    FREE_AND_NULL(str)
		    FREE_AND_NULL(strBIT)
		    FREE_AND_NULL(strBT)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_3);
		}
	    }

	    if (type->facets != NULL) {
		xmlSchemaFacetPtr facet;
		int ok = 1;
		facet = type->facets;
		do {
		    switch (facet->type) {
			case XML_SCHEMA_FACET_LENGTH:
			case XML_SCHEMA_FACET_MINLENGTH:
			case XML_SCHEMA_FACET_MAXLENGTH:
			case XML_SCHEMA_FACET_WHITESPACE:
			case XML_SCHEMA_FACET_PATTERN:
			case XML_SCHEMA_FACET_ENUMERATION:
			    break;
			default: {
			    xmlSchemaPIllegalFacetListUnionErr(pctxt,
				XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_4,
				type, facet);
			    ok = 0;
			}
		    }
		    facet = facet->next;
		} while (facet != NULL);
		if (ok == 0)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_2_3_2_4);
	    }
	}
    } else if (WXS_IS_UNION(type)) {
	xmlSchemaTypeLinkPtr member;

	member = type->memberTypes;
	while (member != NULL) {
	    if (WXS_IS_TYPE_NOT_FIXED(member->type))
		xmlSchemaTypeFixup(member->type, ACTXT_CAST pctxt);

	    if ((! WXS_IS_ATOMIC(member->type)) &&
		(! WXS_IS_LIST(member->type))) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_1,
		    WXS_BASIC_CAST type, NULL,
		    "The member type '%s' is neither an atomic, nor a list type",
		    xmlSchemaGetComponentQName(&str, member->type));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_1);
	    }
	    member = member->next;
	}
	if (type->baseType->builtInType == XML_SCHEMAS_ANYSIMPLETYPE) {
	    member = type->memberTypes;
	    while (member != NULL) {
		if (xmlSchemaTypeFinalContains(member->type,
		    XML_SCHEMAS_TYPE_FINAL_UNION)) {
		    xmlSchemaPCustomErr(pctxt,
			XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1,
			WXS_BASIC_CAST type, NULL,
			"The 'final' of member type '%s' contains 'union'",
			xmlSchemaGetComponentQName(&str, member->type));
		    FREE_AND_NULL(str)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1);
		}
		member = member->next;
	    }
	    if (type->facetSet != NULL) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1_2,
		    WXS_BASIC_CAST type, NULL,
		    "No facets allowed", NULL);
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_1_2);
	    }
	} else {
	    if (! WXS_IS_UNION(type->baseType)) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_1,
		    WXS_BASIC_CAST type, NULL,
		    "The base type '%s' is not a union type",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_1);
	    }
	    if (xmlSchemaTypeFinalContains(type->baseType,
		XML_SCHEMAS_TYPE_FINAL_RESTRICTION)) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_2,
		    WXS_BASIC_CAST type, NULL,
		    "The 'final' of its base type '%s' must not contain 'restriction'",
		    xmlSchemaGetComponentQName(&str, type->baseType));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_2);
	    }
	    {
		xmlSchemaTypeLinkPtr baseMember;

		if (type->memberTypes != NULL) {
		    member = type->memberTypes;
		    baseMember = xmlSchemaGetUnionSimpleTypeMemberTypes(type->baseType);
		    if ((member == NULL) && (baseMember != NULL)) {
			PERROR_INT("xmlSchemaCheckCOSSTRestricts",
			    "different number of member types in base");
		    }
		    while (member != NULL) {
			if (baseMember == NULL) {
			    PERROR_INT("xmlSchemaCheckCOSSTRestricts",
			    "different number of member types in base");
			} else if ((member->type != baseMember->type) &&
			    (xmlSchemaCheckCOSSTDerivedOK(ACTXT_CAST pctxt,
				member->type, baseMember->type, 0) != 0)) {
			    xmlChar *strBMT = NULL, *strBT = NULL;

			    xmlSchemaPCustomErrExt(pctxt,
				XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_3,
				WXS_BASIC_CAST type, NULL,
				"The member type %s is not validly "
				"derived from its corresponding member "
				"type %s of the base type %s",
				xmlSchemaGetComponentQName(&str, member->type),
				xmlSchemaGetComponentQName(&strBMT, baseMember->type),
				xmlSchemaGetComponentQName(&strBT, type->baseType));
			    FREE_AND_NULL(str)
			    FREE_AND_NULL(strBMT)
			    FREE_AND_NULL(strBT)
			    return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_3);
			}
			member = member->next;
                        if (baseMember != NULL)
                            baseMember = baseMember->next;
		    }
		}
	    }
	    if (type->facets != NULL) {
		xmlSchemaFacetPtr facet;
		int ok = 1;

		facet = type->facets;
		do {
		    if ((facet->type != XML_SCHEMA_FACET_PATTERN) &&
			(facet->type != XML_SCHEMA_FACET_ENUMERATION)) {
			xmlSchemaPIllegalFacetListUnionErr(pctxt,
				XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_4,
				type, facet);
			ok = 0;
		    }
		    facet = facet->next;
		} while (facet != NULL);
		if (ok == 0)
		    return (XML_SCHEMAP_COS_ST_RESTRICTS_3_3_2_4);

	    }
	}
    }

    return (0);
}

#if 0
static int
xmlSchemaCheckSRCSimpleType(xmlSchemaParserCtxtPtr ctxt,
			    xmlSchemaTypePtr type)
{
    if (WXS_IS_RESTRICTION(type)) {
    } else if (WXS_IS_LIST(type)) {
    } else if (WXS_IS_UNION(type)) {
    }
    return (0);
}
#endif

static int
xmlSchemaCreateVCtxtOnPCtxt(xmlSchemaParserCtxtPtr ctxt)
{
   if (ctxt->vctxt == NULL) {
	ctxt->vctxt = xmlSchemaNewValidCtxt(NULL);
	if (ctxt->vctxt == NULL) {
	    xmlSchemaPErr(ctxt, NULL,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaCreateVCtxtOnPCtxt, "
		"failed to create a temp. validation context.\n",
		NULL, NULL);
	    return (-1);
	}
	
	xmlSchemaSetValidErrors(ctxt->vctxt,
	    ctxt->error, ctxt->warning, ctxt->errCtxt);
	xmlSchemaSetValidStructuredErrors(ctxt->vctxt,
	    ctxt->serror, ctxt->errCtxt);
    }
    return (0);
}

static int
xmlSchemaVCheckCVCSimpleType(xmlSchemaAbstractCtxtPtr actxt,
			     xmlNodePtr node,
			     xmlSchemaTypePtr type,
			     const xmlChar *value,
			     xmlSchemaValPtr *retVal,
			     int fireErrors,
			     int normalize,
			     int isNormalized);

static int
xmlSchemaParseCheckCOSValidDefault(xmlSchemaParserCtxtPtr pctxt,
				   xmlNodePtr node,
				   xmlSchemaTypePtr type,
				   const xmlChar *value,
				   xmlSchemaValPtr *val)
{
    int ret = 0;

    if WXS_IS_COMPLEX(type) {
	if ((! WXS_HAS_SIMPLE_CONTENT(type)) &&
	    ((! WXS_HAS_MIXED_CONTENT(type)) || (! WXS_EMPTIABLE(type)))) {
	    
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_COS_VALID_DEFAULT_2_1,
		WXS_BASIC_CAST type, type->node,
		"For a string to be a valid default, the type definition "
		"must be a simple type or a complex type with mixed content "
		"and a particle emptiable", NULL);
	    return(XML_SCHEMAP_COS_VALID_DEFAULT_2_1);
	}
    }
    if (WXS_IS_SIMPLE(type))
	ret = xmlSchemaVCheckCVCSimpleType(ACTXT_CAST pctxt, node,
	    type, value, val, 1, 1, 0);
    else if (WXS_HAS_SIMPLE_CONTENT(type))
	ret = xmlSchemaVCheckCVCSimpleType(ACTXT_CAST pctxt, node,
	    type->contentTypeDef, value, val, 1, 1, 0);
    else
	return (ret);

    if (ret < 0) {
	PERROR_INT("xmlSchemaParseCheckCOSValidDefault",
	    "calling xmlSchemaVCheckCVCSimpleType()");
    }

    return (ret);
}

static int
xmlSchemaCheckCTPropsCorrect(xmlSchemaParserCtxtPtr pctxt,
			     xmlSchemaTypePtr type)
{
    if ((type->baseType != NULL) &&
	(WXS_IS_SIMPLE(type->baseType)) &&
	(WXS_IS_EXTENSION(type) == 0)) {
	xmlSchemaCustomErr(ACTXT_CAST pctxt,
	    XML_SCHEMAP_SRC_CT_1,
	    NULL, WXS_BASIC_CAST type,
	    "If the base type is a simple type, the derivation method must be "
	    "'extension'", NULL, NULL);
	return (XML_SCHEMAP_SRC_CT_1);
    }
    if (type->attrUses &&
	(((xmlSchemaItemListPtr) type->attrUses)->nbItems > 1))
    {
	xmlSchemaItemListPtr uses = (xmlSchemaItemListPtr) type->attrUses;
	xmlSchemaAttributeUsePtr use, tmp;
	int i, j, hasId = 0;

	for (i = uses->nbItems -1; i >= 0; i--) {
	    use = uses->items[i];

	    if (i > 0) {
		for (j = i -1; j >= 0; j--) {
		    tmp = uses->items[j];
		    if ((WXS_ATTRUSE_DECL_NAME(use) ==
			WXS_ATTRUSE_DECL_NAME(tmp)) &&
			(WXS_ATTRUSE_DECL_TNS(use) ==
			WXS_ATTRUSE_DECL_TNS(tmp)))
		    {
			xmlChar *str = NULL;

			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    XML_SCHEMAP_AG_PROPS_CORRECT,
			    NULL, WXS_BASIC_CAST type,
			    "Duplicate %s",
			    xmlSchemaGetComponentDesignation(&str, use),
			    NULL);
			FREE_AND_NULL(str);
			if (xmlSchemaItemListRemove(uses, i) == -1)
			    goto exit_failure;
			goto next_use;
		    }
		}
	    }
	    if (WXS_ATTRUSE_TYPEDEF(use) != NULL) {
		if (xmlSchemaIsDerivedFromBuiltInType(
		    WXS_ATTRUSE_TYPEDEF(use), XML_SCHEMAS_ID))
		{
		    if (hasId) {
			xmlChar *str = NULL;

			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    XML_SCHEMAP_AG_PROPS_CORRECT,
			    NULL, WXS_BASIC_CAST type,
			    "There must not exist more than one attribute "
			    "declaration of type 'xs:ID' "
			    "(or derived from 'xs:ID'). The %s violates this "
			    "constraint",
			    xmlSchemaGetComponentDesignation(&str, use),
			    NULL);
			FREE_AND_NULL(str);
			if (xmlSchemaItemListRemove(uses, i) == -1)
			    goto exit_failure;
		    }

		    hasId = 1;
		}
	    }
next_use: {}
	}
    }
    return (0);
exit_failure:
    return(-1);
}

static int
xmlSchemaAreEqualTypes(xmlSchemaTypePtr typeA,
		       xmlSchemaTypePtr typeB)
{
    if ((typeA == NULL) || (typeB == NULL))
	return (0);
    return (typeA == typeB);
}

static int
xmlSchemaCheckCOSCTDerivedOK(xmlSchemaAbstractCtxtPtr actxt,
			     xmlSchemaTypePtr type,
			     xmlSchemaTypePtr baseType,
			     int set)
{
    int equal = xmlSchemaAreEqualTypes(type, baseType);
    
    if (! equal) {
	if (((set & SUBSET_EXTENSION) && (WXS_IS_EXTENSION(type))) ||
	    ((set & SUBSET_RESTRICTION) && (WXS_IS_RESTRICTION(type))))
	    return (1);
    } else {
	return (0);
    }
    if (type->baseType == baseType)
	return (0);
    if (WXS_IS_ANYTYPE(type->baseType))
	return (1);

    if (WXS_IS_COMPLEX(type->baseType)) {
	return (xmlSchemaCheckCOSCTDerivedOK(actxt, type->baseType,
	    baseType, set));
    } else {
	return (xmlSchemaCheckCOSSTDerivedOK(actxt, type->baseType,
	    baseType, set));
    }
}

static int
xmlSchemaCheckCOSDerivedOK(xmlSchemaAbstractCtxtPtr actxt,
			   xmlSchemaTypePtr type,
			   xmlSchemaTypePtr baseType,
			   int set)
{
    if (WXS_IS_SIMPLE(type))
	return (xmlSchemaCheckCOSSTDerivedOK(actxt, type, baseType, set));
    else
	return (xmlSchemaCheckCOSCTDerivedOK(actxt, type, baseType, set));
}

static int
xmlSchemaCheckCOSCTExtends(xmlSchemaParserCtxtPtr ctxt,
			   xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base = type->baseType;
    if (WXS_IS_COMPLEX(base)) {
	if (base->flags & XML_SCHEMAS_TYPE_FINAL_EXTENSION) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		WXS_BASIC_CAST type, NULL,
		"The 'final' of the base type definition "
		"contains 'extension'", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}

#if 0
	if (base->attrUses != NULL) {
	    int i, j, found;
	    xmlSchemaAttributeUsePtr use, buse;

	    for (i = 0; i < (WXS_LIST_CAST base->attrUses)->nbItems; i ++) {
		buse = (WXS_LIST_CAST base->attrUses)->items[i];
		found = 0;
		if (type->attrUses != NULL) {
		    use = (WXS_LIST_CAST type->attrUses)->items[j];
		    for (j = 0; j < (WXS_LIST_CAST type->attrUses)->nbItems; j ++)
		    {
			if ((WXS_ATTRUSE_DECL_NAME(use) ==
				WXS_ATTRUSE_DECL_NAME(buse)) &&
			    (WXS_ATTRUSE_DECL_TNS(use) ==
				WXS_ATTRUSE_DECL_TNS(buse)) &&
			    (WXS_ATTRUSE_TYPEDEF(use) ==
				WXS_ATTRUSE_TYPEDEF(buse))
			{
			    found = 1;
			    break;
			}
		    }
		}
		if (! found) {
		    xmlChar *str = NULL;

		    xmlSchemaCustomErr(ACTXT_CAST ctxt,
			XML_SCHEMAP_COS_CT_EXTENDS_1_2,
			NULL, WXS_BASIC_CAST type,
			"This type is missing a matching correspondent "
			"for its {base type}'s %s in its {attribute uses}",
			xmlSchemaGetComponentDesignation(&str,
			    buse->children),
			NULL);
		    FREE_AND_NULL(str)
		}
	    }
	}

	if (base->attributeWildcard != NULL) {
	    if (type->attributeWilcard == NULL) {
		xmlChar *str = NULL;

		xmlSchemaCustomErr(ACTXT_CAST pctxt,
		    XML_SCHEMAP_COS_CT_EXTENDS_1_3,
		    NULL, type,
		    "The base %s has an attribute wildcard, "
		    "but this type is missing an attribute wildcard",
		    xmlSchemaGetComponentDesignation(&str, base));
		FREE_AND_NULL(str)

	    } else if (xmlSchemaCheckCOSNSSubset(
		base->attributeWildcard, type->attributeWildcard))
	    {
		xmlChar *str = NULL;

		xmlSchemaCustomErr(ACTXT_CAST pctxt,
		    XML_SCHEMAP_COS_CT_EXTENDS_1_3,
		    NULL, type,
		    "The attribute wildcard is not a valid "
		    "superset of the one in the base %s",
		    xmlSchemaGetComponentDesignation(&str, base));
		FREE_AND_NULL(str)
	    }
	}
#endif
	if ((type->contentTypeDef != NULL) &&
	    (type->contentTypeDef == base->contentTypeDef)) {
	} else if ((type->contentType == XML_SCHEMA_CONTENT_EMPTY) &&
	    (base->contentType == XML_SCHEMA_CONTENT_EMPTY) ) {
	} else {
	    if (type->subtypes == NULL) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		    WXS_BASIC_CAST type, NULL,
		    "The content type must specify a particle", NULL);
		return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	    }
	    if (base->contentType == XML_SCHEMA_CONTENT_EMPTY) {
	    } else {
		if ((type->contentType != base->contentType) ||
		    ((type->contentType != XML_SCHEMA_CONTENT_MIXED) &&
		    (type->contentType != XML_SCHEMA_CONTENT_ELEMENTS))) {
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_COS_CT_EXTENDS_1_1,
			WXS_BASIC_CAST type, NULL,
			"The content type of both, the type and its base "
			"type, must either 'mixed' or 'element-only'", NULL);
		    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
		}
	    }
	}
    } else {
	if (type->contentTypeDef != base) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		WXS_BASIC_CAST type, NULL,
		"The content type must be the simple base type", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}
	if (base->flags & XML_SCHEMAS_TYPE_FINAL_EXTENSION) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_COS_CT_EXTENDS_1_1,
		WXS_BASIC_CAST type, NULL,
		"The 'final' of the base type definition "
		"contains 'extension'", NULL);
	    return (XML_SCHEMAP_COS_CT_EXTENDS_1_1);
	}
    }
    return (0);
}

static int
xmlSchemaCheckDerivationOKRestriction(xmlSchemaParserCtxtPtr ctxt,
				      xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base;

    base = type->baseType;
    if (! WXS_IS_COMPLEX(base)) {
	xmlSchemaCustomErr(ACTXT_CAST ctxt,
	    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
	    type->node, WXS_BASIC_CAST type,
	    "The base type must be a complex type", NULL, NULL);
	return(ctxt->err);
    }
    if (base->flags & XML_SCHEMAS_TYPE_FINAL_RESTRICTION) {
	xmlSchemaCustomErr(ACTXT_CAST ctxt,
	    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
	    type->node, WXS_BASIC_CAST type,
	    "The 'final' of the base type definition "
	    "contains 'restriction'", NULL, NULL);
	return (ctxt->err);
    }
    if (xmlSchemaCheckDerivationOKRestriction2to4(ctxt,
	XML_SCHEMA_ACTION_DERIVE,
	WXS_BASIC_CAST type, WXS_BASIC_CAST base,
	type->attrUses, base->attrUses,
	type->attributeWildcard,
	base->attributeWildcard) == -1)
    {
	return(-1);
    }
    if (base->builtInType == XML_SCHEMAS_ANYTYPE) {
    } else if ((type->contentType == XML_SCHEMA_CONTENT_SIMPLE) ||
	    (type->contentType == XML_SCHEMA_CONTENT_BASIC)) {
	if ((base->contentType == XML_SCHEMA_CONTENT_SIMPLE) ||
	    (base->contentType == XML_SCHEMA_CONTENT_BASIC))
	{
	    int err;
	    err = xmlSchemaCheckCOSSTDerivedOK(ACTXT_CAST ctxt,
		type->contentTypeDef, base->contentTypeDef, 0);
	    if (err != 0) {
		xmlChar *strA = NULL, *strB = NULL;

		if (err == -1)
		    return(-1);
		xmlSchemaCustomErr(ACTXT_CAST ctxt,
		    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
		    NULL, WXS_BASIC_CAST type,
		    "The {content type} %s is not validly derived from the "
		    "base type's {content type} %s",
		    xmlSchemaGetComponentDesignation(&strA,
			type->contentTypeDef),
		    xmlSchemaGetComponentDesignation(&strB,
			base->contentTypeDef));
		FREE_AND_NULL(strA);
		FREE_AND_NULL(strB);
		return(ctxt->err);
	    }
	} else if ((base->contentType == XML_SCHEMA_CONTENT_MIXED) &&
	    (xmlSchemaIsParticleEmptiable(
		(xmlSchemaParticlePtr) base->subtypes))) {
	} else {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
		WXS_BASIC_CAST type, NULL,
		"The content type of the base type must be either "
		"a simple type or 'mixed' and an emptiable particle", NULL);
	    return (ctxt->err);
	}
    } else if (type->contentType == XML_SCHEMA_CONTENT_EMPTY) {
	if (base->contentType == XML_SCHEMA_CONTENT_EMPTY) {
	} else if (((base->contentType == XML_SCHEMA_CONTENT_ELEMENTS) ||
	    (base->contentType == XML_SCHEMA_CONTENT_MIXED)) &&
	    xmlSchemaIsParticleEmptiable(
		(xmlSchemaParticlePtr) base->subtypes)) {
	} else {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
		WXS_BASIC_CAST type, NULL,
		"The content type of the base type must be either "
		"empty or 'mixed' (or 'elements-only') and an emptiable "
		"particle", NULL);
	    return (ctxt->err);
	}
    } else if ((type->contentType == XML_SCHEMA_CONTENT_ELEMENTS) ||
	WXS_HAS_MIXED_CONTENT(type)) {
	if (WXS_HAS_MIXED_CONTENT(type) && (! WXS_HAS_MIXED_CONTENT(base))) {
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
		WXS_BASIC_CAST type, NULL,
		"If the content type is 'mixed', then the content type of the "
		"base type must also be 'mixed'", NULL);
	    return (ctxt->err);
	}
    } else {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_DERIVATION_OK_RESTRICTION_1,
	    WXS_BASIC_CAST type, NULL,
	    "The type is not a valid restriction of its base type", NULL);
	return (ctxt->err);
    }
    return (0);
}

static int
xmlSchemaCheckCTComponent(xmlSchemaParserCtxtPtr ctxt,
			  xmlSchemaTypePtr type)
{
    int ret;
    ret = xmlSchemaCheckCTPropsCorrect(ctxt, type);
    if (ret != 0)
	return (ret);
    if (WXS_IS_EXTENSION(type))
	ret = xmlSchemaCheckCOSCTExtends(ctxt, type);
    else
	ret = xmlSchemaCheckDerivationOKRestriction(ctxt, type);
    return (ret);
}

static int
xmlSchemaCheckSRCCT(xmlSchemaParserCtxtPtr ctxt,
		    xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base;
    int ret = 0;

    base = type->baseType;
    if (! WXS_HAS_SIMPLE_CONTENT(type)) {
	if (! WXS_IS_COMPLEX(base)) {
	    xmlChar *str = NULL;
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_CT_1,
		WXS_BASIC_CAST type, type->node,
		"If using <complexContent>, the base type is expected to be "
		"a complex type. The base type '%s' is a simple type",
		xmlSchemaFormatQName(&str, base->targetNamespace,
		base->name));
	    FREE_AND_NULL(str)
	    return (XML_SCHEMAP_SRC_CT_1);
	}
    } else {
	if (WXS_IS_SIMPLE(base)) {
	    if (WXS_IS_EXTENSION(type) == 0) {
		xmlChar *str = NULL;
		
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_SRC_CT_1,
		    WXS_BASIC_CAST type, NULL,
		    "If using <simpleContent> and <restriction>, the base "
		    "type must be a complex type. The base type '%s' is "
		    "a simple type",
		    xmlSchemaFormatQName(&str, base->targetNamespace,
			base->name));
		FREE_AND_NULL(str)
		return (XML_SCHEMAP_SRC_CT_1);
	    }
	} else {
	    
	    if ((base->contentType == XML_SCHEMA_CONTENT_SIMPLE) ||
		(base->contentType == XML_SCHEMA_CONTENT_BASIC)) {
		if (base->contentTypeDef == NULL) {
		    xmlSchemaPCustomErr(ctxt, XML_SCHEMAP_INTERNAL,
			WXS_BASIC_CAST type, NULL,
			"Internal error: xmlSchemaCheckSRCCT, "
			"'%s', base type has no content type",
			type->name);
		    return (-1);
		}
	    } else if ((base->contentType == XML_SCHEMA_CONTENT_MIXED) &&
		(WXS_IS_RESTRICTION(type))) {

		if (! xmlSchemaIsParticleEmptiable(
		    (xmlSchemaParticlePtr) base->subtypes)) {
		    ret = XML_SCHEMAP_SRC_CT_1;
		} else
		    if (type->contentTypeDef == NULL) {
		    xmlChar *str = NULL;
		    
		    xmlSchemaPCustomErr(ctxt,
			XML_SCHEMAP_SRC_CT_1,
			WXS_BASIC_CAST type, NULL,
			"A <simpleType> is expected among the children "
			"of <restriction>, if <simpleContent> is used and "
			"the base type '%s' is a complex type",
			xmlSchemaFormatQName(&str, base->targetNamespace,
			base->name));
		    FREE_AND_NULL(str)
		    return (XML_SCHEMAP_SRC_CT_1);
		}
	    } else {
		ret = XML_SCHEMAP_SRC_CT_1;
	    }
	}
	if (ret > 0) {
	    xmlChar *str = NULL;
	    if (WXS_IS_RESTRICTION(type)) {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_SRC_CT_1,
		    WXS_BASIC_CAST type, NULL,
		    "If <simpleContent> and <restriction> is used, the "
		    "base type must be a simple type or a complex type with "
		    "mixed content and particle emptiable. The base type "
		    "'%s' is none of those",
		    xmlSchemaFormatQName(&str, base->targetNamespace,
		    base->name));
	    } else {
		xmlSchemaPCustomErr(ctxt,
		    XML_SCHEMAP_SRC_CT_1,
		    WXS_BASIC_CAST type, NULL,
		    "If <simpleContent> and <extension> is used, the "
		    "base type must be a simple type. The base type '%s' "
		    "is a complex type",
		    xmlSchemaFormatQName(&str, base->targetNamespace,
		    base->name));
	    }
	    FREE_AND_NULL(str)
	}
    }
    return (ret);
}

#ifdef ENABLE_PARTICLE_RESTRICTION
static int
xmlSchemaCheckParticleRangeOK(int rmin, int rmax,
			      int bmin, int bmax)
{
    if (rmin < bmin)
	return (1);
    if ((bmax != UNBOUNDED) &&
	(rmax > bmax))
	return (1);
    return (0);
}

static int
xmlSchemaCheckRCaseNameAndTypeOK(xmlSchemaParserCtxtPtr ctxt,
				 xmlSchemaParticlePtr r,
				 xmlSchemaParticlePtr b)
{
    xmlSchemaElementPtr elemR, elemB;

    
    elemR = (xmlSchemaElementPtr) r->children;
    elemB = (xmlSchemaElementPtr) b->children;
    if ((elemR != elemB) &&
	((! xmlStrEqual(elemR->name, elemB->name)) ||
	(! xmlStrEqual(elemR->targetNamespace, elemB->targetNamespace))))
	return (1);
    if (xmlSchemaCheckParticleRangeOK(r->minOccurs, r->maxOccurs,
	    b->minOccurs, b->maxOccurs) != 0)
	return (1);
    if (elemR == elemB)
	return (0);
    if (((elemB->flags & XML_SCHEMAS_ELEM_NILLABLE) == 0) &&
	(elemR->flags & XML_SCHEMAS_ELEM_NILLABLE))
	 return (1);
    if ((elemB->value != NULL) && (elemB->flags & XML_SCHEMAS_ELEM_FIXED) &&
	((elemR->value == NULL) ||
	 ((elemR->flags & XML_SCHEMAS_ELEM_FIXED) == 0) ||
	 
	 (! xmlStrEqual(elemR->value, elemB->value))))
	 return (1);
    if (elemB->idcs != NULL) {
	
    }
    if (((elemB->flags & XML_SCHEMAS_ELEM_BLOCK_EXTENSION) &&
	 ((elemR->flags & XML_SCHEMAS_ELEM_BLOCK_EXTENSION) == 0)) ||
	((elemB->flags & XML_SCHEMAS_ELEM_BLOCK_RESTRICTION) &&
	 ((elemR->flags & XML_SCHEMAS_ELEM_BLOCK_RESTRICTION) == 0)) ||
	((elemB->flags & XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION) &&
	 ((elemR->flags & XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION) == 0)))
	 return (1);
    {
	int set = 0;

	set |= SUBSET_EXTENSION;
	set |= SUBSET_LIST;
	set |= SUBSET_UNION;
	if (xmlSchemaCheckCOSDerivedOK(ACTXT_CAST ctxt, elemR->subtypes,
	    elemB->subtypes, set) != 0)
	    return (1);
    }
    return (0);
}

static int
xmlSchemaCheckRCaseNSCompat(xmlSchemaParserCtxtPtr ctxt,
			    xmlSchemaParticlePtr r,
			    xmlSchemaParticlePtr b)
{
    
    if (xmlSchemaCheckCVCWildcardNamespace((xmlSchemaWildcardPtr) b->children,
	((xmlSchemaElementPtr) r->children)->targetNamespace) != 0)
	return (1);
    if (xmlSchemaCheckParticleRangeOK(r->minOccurs, r->maxOccurs,
	    b->minOccurs, b->maxOccurs) != 0)
	return (1);

    return (0);
}

static int
xmlSchemaCheckRCaseRecurseAsIfGroup(xmlSchemaParserCtxtPtr ctxt,
				    xmlSchemaParticlePtr r,
				    xmlSchemaParticlePtr b)
{
    
    TODO
    return (0);
}

static int
xmlSchemaCheckRCaseNSSubset(xmlSchemaParserCtxtPtr ctxt,
				    xmlSchemaParticlePtr r,
				    xmlSchemaParticlePtr b,
				    int isAnyTypeBase)
{
    
    if (xmlSchemaCheckParticleRangeOK(r->minOccurs, r->maxOccurs,
	    b->minOccurs, b->maxOccurs))
	return (1);
    if (xmlSchemaCheckCOSNSSubset((xmlSchemaWildcardPtr) r->children,
	(xmlSchemaWildcardPtr) b->children))
	return (1);
    if (! isAnyTypeBase) {
	if ( ((xmlSchemaWildcardPtr) r->children)->processContents <
	    ((xmlSchemaWildcardPtr) b->children)->processContents)
	    return (1);
    }

    return (0);
}

static int
xmlSchemaCheckCOSParticleRestrict(xmlSchemaParserCtxtPtr ctxt,
				  xmlSchemaParticlePtr r,
				  xmlSchemaParticlePtr b)
{
    int ret = 0;


    TODO

    if (r == b)
	return (0);


    return (0);
}

#if 0
static int
xmlSchemaCheckRCaseNSRecurseCheckCardinality(xmlSchemaParserCtxtPtr ctxt,
					     xmlSchemaParticlePtr r,
					     xmlSchemaParticlePtr b)
{
    xmlSchemaParticlePtr part;
    
    if ((r->children == NULL) || (r->children->children == NULL))
	return (-1);
    part = (xmlSchemaParticlePtr) r->children->children;
    do {
	if (xmlSchemaCheckCOSParticleRestrict(ctxt, part, b))
	    return (1);
	part = (xmlSchemaParticlePtr) part->next;
    } while (part != NULL);
    if (xmlSchemaCheckParticleRangeOK(
	    xmlSchemaGetParticleTotalRangeMin(r),
	    xmlSchemaGetParticleTotalRangeMax(r),
	    b->minOccurs, b->maxOccurs) != 0)
	return (1);
    return (0);
}
#endif

static int
xmlSchemaCheckRCaseRecurse(xmlSchemaParserCtxtPtr ctxt,
			   xmlSchemaParticlePtr r,
			   xmlSchemaParticlePtr b)
{
    
    
    if ((r->children == NULL) || (b->children == NULL) ||
	(r->children->type != b->children->type))
	return (-1);
    if (xmlSchemaCheckParticleRangeOK(r->minOccurs, r->maxOccurs,
	    b->minOccurs, b->maxOccurs))
	return (1);


    return (0);
}

#endif

#define FACET_RESTR_MUTUAL_ERR(fac1, fac2) \
    xmlSchemaPCustomErrExt(pctxt,      \
	XML_SCHEMAP_INVALID_FACET_VALUE, \
	WXS_BASIC_CAST fac1, fac1->node, \
	"It is an error for both '%s' and '%s' to be specified on the "\
	"same type definition", \
	BAD_CAST xmlSchemaFacetTypeToString(fac1->type), \
	BAD_CAST xmlSchemaFacetTypeToString(fac2->type), NULL);

#define FACET_RESTR_ERR(fac1, msg) \
    xmlSchemaPCustomErr(pctxt,      \
	XML_SCHEMAP_INVALID_FACET_VALUE, \
	WXS_BASIC_CAST fac1, fac1->node, \
	msg, NULL);

#define FACET_RESTR_FIXED_ERR(fac) \
    xmlSchemaPCustomErr(pctxt, \
	XML_SCHEMAP_INVALID_FACET_VALUE, \
	WXS_BASIC_CAST fac, fac->node, \
	"The base type's facet is 'fixed', thus the value must not " \
	"differ", NULL);

static void
xmlSchemaDeriveFacetErr(xmlSchemaParserCtxtPtr pctxt,
			xmlSchemaFacetPtr facet1,
			xmlSchemaFacetPtr facet2,
			int lessGreater,
			int orEqual,
			int ofBase)
{
    xmlChar *msg = NULL;

    msg = xmlStrdup(BAD_CAST "'");
    msg = xmlStrcat(msg, xmlSchemaFacetTypeToString(facet1->type));
    msg = xmlStrcat(msg, BAD_CAST "' has to be");
    if (lessGreater == 0)
	msg = xmlStrcat(msg, BAD_CAST " equal to");
    if (lessGreater == 1)
	msg = xmlStrcat(msg, BAD_CAST " greater than");
    else
	msg = xmlStrcat(msg, BAD_CAST " less than");

    if (orEqual)
	msg = xmlStrcat(msg, BAD_CAST " or equal to");
    msg = xmlStrcat(msg, BAD_CAST " '");
    msg = xmlStrcat(msg, xmlSchemaFacetTypeToString(facet2->type));
    if (ofBase)
	msg = xmlStrcat(msg, BAD_CAST "' of the base type");
    else
	msg = xmlStrcat(msg, BAD_CAST "'");

    xmlSchemaPCustomErr(pctxt,
	XML_SCHEMAP_INVALID_FACET_VALUE,
	WXS_BASIC_CAST facet1, NULL,
	(const char *) msg, NULL);

    if (msg != NULL)
	xmlFree(msg);
}

static int
xmlSchemaDeriveAndValidateFacets(xmlSchemaParserCtxtPtr pctxt,
				 xmlSchemaTypePtr type)
{
    xmlSchemaTypePtr base = type->baseType;
    xmlSchemaFacetLinkPtr link, cur, last = NULL;
    xmlSchemaFacetPtr facet, bfacet,
	flength = NULL, ftotdig = NULL, ffracdig = NULL,
	fmaxlen = NULL, fminlen = NULL, 
	fmininc = NULL, fmaxinc = NULL,
	fminexc = NULL, fmaxexc = NULL,
	bflength = NULL, bftotdig = NULL, bffracdig = NULL,
	bfmaxlen = NULL, bfminlen = NULL, 
	bfmininc = NULL, bfmaxinc = NULL,
	bfminexc = NULL, bfmaxexc = NULL;
    int res; 


    if ((type->facetSet == NULL) && (base->facetSet == NULL))
	return (0);

    last = type->facetSet;
    if (last != NULL)
	while (last->next != NULL)
	    last = last->next;

    for (cur = type->facetSet; cur != NULL; cur = cur->next) {
	facet = cur->facet;
	switch (facet->type) {
	    case XML_SCHEMA_FACET_LENGTH:
		flength = facet; break;
	    case XML_SCHEMA_FACET_MINLENGTH:
		fminlen = facet; break;
	    case XML_SCHEMA_FACET_MININCLUSIVE:
		fmininc = facet; break;
	    case XML_SCHEMA_FACET_MINEXCLUSIVE:
		fminexc = facet; break;
	    case XML_SCHEMA_FACET_MAXLENGTH:
		fmaxlen = facet; break;
	    case XML_SCHEMA_FACET_MAXINCLUSIVE:
		fmaxinc = facet; break;
	    case XML_SCHEMA_FACET_MAXEXCLUSIVE:
		fmaxexc = facet; break;
	    case XML_SCHEMA_FACET_TOTALDIGITS:
		ftotdig = facet; break;
	    case XML_SCHEMA_FACET_FRACTIONDIGITS:
		ffracdig = facet; break;
	    default:
		break;
	}
    }
    for (cur = base->facetSet; cur != NULL; cur = cur->next) {
	facet = cur->facet;
	switch (facet->type) {
	    case XML_SCHEMA_FACET_LENGTH:
		bflength = facet; break;
	    case XML_SCHEMA_FACET_MINLENGTH:
		bfminlen = facet; break;
	    case XML_SCHEMA_FACET_MININCLUSIVE:
		bfmininc = facet; break;
	    case XML_SCHEMA_FACET_MINEXCLUSIVE:
		bfminexc = facet; break;
	    case XML_SCHEMA_FACET_MAXLENGTH:
		bfmaxlen = facet; break;
	    case XML_SCHEMA_FACET_MAXINCLUSIVE:
		bfmaxinc = facet; break;
	    case XML_SCHEMA_FACET_MAXEXCLUSIVE:
		bfmaxexc = facet; break;
	    case XML_SCHEMA_FACET_TOTALDIGITS:
		bftotdig = facet; break;
	    case XML_SCHEMA_FACET_FRACTIONDIGITS:
		bffracdig = facet; break;
	    default:
		break;
	}
    }
    if (flength && (fminlen || fmaxlen)) {
	FACET_RESTR_ERR(flength, "It is an error for both 'length' and "
	    "either of 'minLength' or 'maxLength' to be specified on "
	    "the same type definition")
    }
    if ((fmaxinc) && (fmaxexc)) {
	FACET_RESTR_MUTUAL_ERR(fmaxinc, fmaxexc)
    }
    if ((fmininc) && (fminexc)) {
	FACET_RESTR_MUTUAL_ERR(fmininc, fminexc)
    }

    if (flength && bflength) {
	res = xmlSchemaCompareValues(flength->val, bflength->val);
	if (res == -2)
	    goto internal_error;
	if (res != 0)
	    xmlSchemaDeriveFacetErr(pctxt, flength, bflength, 0, 0, 1);
	if ((res != 0) && (bflength->fixed)) {
	    FACET_RESTR_FIXED_ERR(flength)
	}

    }
    if (fminlen && bfminlen) {
	res = xmlSchemaCompareValues(fminlen->val, bfminlen->val);
	if (res == -2)
	    goto internal_error;
	if (res == -1)
	    xmlSchemaDeriveFacetErr(pctxt, fminlen, bfminlen, 1, 1, 1);
	if ((res != 0) && (bfminlen->fixed)) {
	    FACET_RESTR_FIXED_ERR(fminlen)
	}
    }
    if (fmaxlen && bfmaxlen) {
	res = xmlSchemaCompareValues(fmaxlen->val, bfmaxlen->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, fmaxlen, bfmaxlen, -1, 1, 1);
	if ((res != 0) && (bfmaxlen->fixed)) {
	    FACET_RESTR_FIXED_ERR(fmaxlen)
	}
    }
    if (! flength)
	flength = bflength;
    if (flength) {
	if (! fminlen)
	    fminlen = bfminlen;
	if (fminlen) {
	    
	    res = xmlSchemaCompareValues(flength->val, fminlen->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1)
		xmlSchemaDeriveFacetErr(pctxt, flength, fminlen, 1, 1, 0);
	}
	if (! fmaxlen)
	    fmaxlen = bfmaxlen;
	if (fmaxlen) {
	    
	    res = xmlSchemaCompareValues(flength->val, fmaxlen->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1)
		xmlSchemaDeriveFacetErr(pctxt, flength, fmaxlen, -1, 1, 0);
	}
    }
    if (fmaxinc) {
	if (fmininc) {
	    
	    res = xmlSchemaCompareValues(fmaxinc->val, fmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, fmininc, 1, 1, 0);
	    }
	}
	if (bfmaxinc) {
	    
	    res = xmlSchemaCompareValues(fmaxinc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1)
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfmaxinc, -1, 1, 1);
	    if ((res != 0) && (bfmaxinc->fixed)) {
		FACET_RESTR_FIXED_ERR(fmaxinc)
	    }
	}
	if (bfmaxexc) {
	    
	    res = xmlSchemaCompareValues(fmaxinc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfmaxexc, -1, 0, 1);
	    }
	}
	if (bfmininc) {
	    
	    res = xmlSchemaCompareValues(fmaxinc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfmininc, 1, 1, 1);
	    }
	}
	if (bfminexc) {
	    
	    res = xmlSchemaCompareValues(fmaxinc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxinc, bfminexc, 1, 0, 1);
	    }
	}
    }
    if (fmaxexc) {
	if (fminexc) {
	    res = xmlSchemaCompareValues(fmaxexc->val, fminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, fminexc, 1, 1, 0);
	    }
	}
	if (bfmaxexc) {
	    
	    res = xmlSchemaCompareValues(fmaxexc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfmaxexc, -1, 1, 1);
	    }
	    if ((res != 0) && (bfmaxexc->fixed)) {
		FACET_RESTR_FIXED_ERR(fmaxexc)
	    }
	}
	if (bfmaxinc) {
	    
	    res = xmlSchemaCompareValues(fmaxexc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfmaxinc, -1, 1, 1);
	    }
	}
	if (bfmininc) {
	    
	    res = xmlSchemaCompareValues(fmaxexc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfmininc, 1, 0, 1);
	    }
	}
	if (bfminexc) {
	    
	    res = xmlSchemaCompareValues(fmaxexc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmaxexc, bfminexc, 1, 0, 1);
	    }
	}
    }
    if (fminexc) {
	if (fmaxinc) {
	    res = xmlSchemaCompareValues(fminexc->val, fmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, fmaxinc, -1, 0, 0);
	    }
	}
	if (bfminexc) {
	    
	    res = xmlSchemaCompareValues(fminexc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfminexc, 1, 1, 1);
	    }
	    if ((res != 0) && (bfminexc->fixed)) {
		FACET_RESTR_FIXED_ERR(fminexc)
	    }
	}
	if (bfmaxinc) {
	    
	    res = xmlSchemaCompareValues(fminexc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfmaxinc, -1, 1, 1);
	    }
	}
	if (bfmininc) {
	    
	    res = xmlSchemaCompareValues(fminexc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfmininc, 1, 1, 1);
	    }
	}
	if (bfmaxexc) {
	    
	    res = xmlSchemaCompareValues(fminexc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fminexc, bfmaxexc, -1, 0, 1);
	    }
	}
    }
    if (fmininc) {
	if (fmaxexc) {
	    res = xmlSchemaCompareValues(fmininc->val, fmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmininc, fmaxexc, -1, 0, 0);
	    }
	}
	if (bfmininc) {
	    
	    res = xmlSchemaCompareValues(fmininc->val, bfmininc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == -1) {
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfmininc, 1, 1, 1);
	    }
	    if ((res != 0) && (bfmininc->fixed)) {
		FACET_RESTR_FIXED_ERR(fmininc)
	    }
	}
	if (bfmaxinc) {
	    
	    res = xmlSchemaCompareValues(fmininc->val, bfmaxinc->val);
	    if (res == -2)
		goto internal_error;
	    if (res == 1) {
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfmaxinc, -1, 1, 1);
	    }
	}
	if (bfminexc) {
	    
	    res = xmlSchemaCompareValues(fmininc->val, bfminexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != 1)
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfminexc, 1, 0, 1);
	}
	if (bfmaxexc) {
	    
	    res = xmlSchemaCompareValues(fmininc->val, bfmaxexc->val);
	    if (res == -2)
		goto internal_error;
	    if (res != -1)
		xmlSchemaDeriveFacetErr(pctxt, fmininc, bfmaxexc, -1, 0, 1);
	}
    }
    if (ftotdig && bftotdig) {
	res = xmlSchemaCompareValues(ftotdig->val, bftotdig->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, ftotdig, bftotdig,
	    -1, 1, 1);
	if ((res != 0) && (bftotdig->fixed)) {
	    FACET_RESTR_FIXED_ERR(ftotdig)
	}
    }
    if (ffracdig && bffracdig) {
	res = xmlSchemaCompareValues(ffracdig->val, bffracdig->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, ffracdig, bffracdig,
	    -1, 1, 1);
	if ((res != 0) && (bffracdig->fixed)) {
	    FACET_RESTR_FIXED_ERR(ffracdig)
	}
    }
    if (! ftotdig)
	ftotdig = bftotdig;
    if (! ffracdig)
	ffracdig = bffracdig;
    if (ftotdig && ffracdig) {
	res = xmlSchemaCompareValues(ffracdig->val, ftotdig->val);
	if (res == -2)
	    goto internal_error;
	if (res == 1)
	    xmlSchemaDeriveFacetErr(pctxt, ffracdig, ftotdig,
		-1, 1, 0);
    }
    for (cur = base->facetSet; cur != NULL; cur = cur->next) {
	bfacet = cur->facet;
	if ((bfacet->type == XML_SCHEMA_FACET_PATTERN) ||
	    (bfacet->type == XML_SCHEMA_FACET_ENUMERATION))
	    continue;
	link = type->facetSet;
	
	
	while (link != NULL) {
	    facet = link->facet;
	    if (facet->type == bfacet->type) {
		switch (facet->type) {
		    case XML_SCHEMA_FACET_WHITESPACE:
			if (facet->whitespace < bfacet->whitespace) {
			    FACET_RESTR_ERR(facet,
				"The 'whitespace' value has to be equal to "
				"or stronger than the 'whitespace' value of "
				"the base type")
			}
			if ((bfacet->fixed) &&
			    (facet->whitespace != bfacet->whitespace)) {
			    FACET_RESTR_FIXED_ERR(facet)
			}
			break;
		    default:
			break;
		}
		
		break;
	    }
	    link = link->next;
	}
	if (link == NULL) {
	    link = (xmlSchemaFacetLinkPtr)
		xmlMalloc(sizeof(xmlSchemaFacetLink));
	    if (link == NULL) {
		xmlSchemaPErrMemory(pctxt,
		    "deriving facets, creating a facet link", NULL);
		return (-1);
	    }
	    link->facet = cur->facet;
	    link->next = NULL;
	    if (last == NULL)
		type->facetSet = link;
	    else
		last->next = link;
	    last = link;
	}

    }

    return (0);
internal_error:
    PERROR_INT("xmlSchemaDeriveAndValidateFacets",
	"an error occured");
    return (-1);
}

static int
xmlSchemaFinishMemberTypeDefinitionsProperty(xmlSchemaParserCtxtPtr pctxt,
					     xmlSchemaTypePtr type)
{
    xmlSchemaTypeLinkPtr link, lastLink, prevLink, subLink, newLink;
    link = type->memberTypes;
    while (link != NULL) {

	if (WXS_IS_TYPE_NOT_FIXED(link->type))
	    xmlSchemaTypeFixup(link->type, ACTXT_CAST pctxt);

	if (WXS_IS_UNION(link->type)) {
	    subLink = xmlSchemaGetUnionSimpleTypeMemberTypes(link->type);
	    if (subLink != NULL) {
		link->type = subLink->type;
		if (subLink->next != NULL) {
		    lastLink = link->next;
		    subLink = subLink->next;
		    prevLink = link;
		    while (subLink != NULL) {
			newLink = (xmlSchemaTypeLinkPtr)
			    xmlMalloc(sizeof(xmlSchemaTypeLink));
			if (newLink == NULL) {
			    xmlSchemaPErrMemory(pctxt, "allocating a type link",
				NULL);
			    return (-1);
			}
			newLink->type = subLink->type;
			prevLink->next = newLink;
			prevLink = newLink;
			newLink->next = lastLink;

			subLink = subLink->next;
		    }
		}
	    }
	}
	link = link->next;
    }
    return (0);
}

static void
xmlSchemaTypeFixupOptimFacets(xmlSchemaTypePtr type)
{
    int has = 0, needVal = 0, normVal = 0;

    has	= (type->baseType->flags & XML_SCHEMAS_TYPE_HAS_FACETS) ? 1 : 0;
    if (has) {
	needVal = (type->baseType->flags &
	    XML_SCHEMAS_TYPE_FACETSNEEDVALUE) ? 1 : 0;
	normVal = (type->baseType->flags &
	    XML_SCHEMAS_TYPE_NORMVALUENEEDED) ? 1 : 0;
    }
    if (type->facets != NULL) {
	xmlSchemaFacetPtr fac;

	for (fac = type->facets; fac != NULL; fac = fac->next) {
	    switch (fac->type) {
		case XML_SCHEMA_FACET_WHITESPACE:
		    break;
		case XML_SCHEMA_FACET_PATTERN:
		    normVal = 1;
		    has = 1;
		    break;
		case XML_SCHEMA_FACET_ENUMERATION:
		    needVal = 1;
		    normVal = 1;
		    has = 1;
		    break;
		default:
		    has = 1;
		    break;
	    }
	}
    }
    if (normVal)
	type->flags |= XML_SCHEMAS_TYPE_NORMVALUENEEDED;
    if (needVal)
	type->flags |= XML_SCHEMAS_TYPE_FACETSNEEDVALUE;
    if (has)
	type->flags |= XML_SCHEMAS_TYPE_HAS_FACETS;

    if (has && (! needVal) && WXS_IS_ATOMIC(type)) {
	xmlSchemaTypePtr prim = xmlSchemaGetPrimitiveType(type);
	if ((prim->builtInType != XML_SCHEMAS_ANYSIMPLETYPE) &&
	    (prim->builtInType != XML_SCHEMAS_STRING)) {
	    type->flags |= XML_SCHEMAS_TYPE_FACETSNEEDVALUE;
	}
    }
}

static int
xmlSchemaTypeFixupWhitespace(xmlSchemaTypePtr type)
{


    if (WXS_IS_LIST(type)) {
	type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_COLLAPSE;
	return (0);
    } else if (WXS_IS_UNION(type))
	return (0);

    if (type->facetSet != NULL) {
	xmlSchemaFacetLinkPtr lin;

	for (lin = type->facetSet; lin != NULL; lin = lin->next) {
	    if (lin->facet->type == XML_SCHEMA_FACET_WHITESPACE) {
		switch (lin->facet->whitespace) {
		case XML_SCHEMAS_FACET_PRESERVE:
		    type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_PRESERVE;
		    break;
		case XML_SCHEMAS_FACET_REPLACE:
		    type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_REPLACE;
		    break;
		case XML_SCHEMAS_FACET_COLLAPSE:
		    type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_COLLAPSE;
		    break;
		default:
		    return (-1);
		}
		return (0);
	    }
	}
    }
    {
	xmlSchemaTypePtr anc;

	for (anc = type->baseType; anc != NULL &&
		anc->builtInType != XML_SCHEMAS_ANYTYPE;
		anc = anc->baseType) {

	    if (anc->type == XML_SCHEMA_TYPE_BASIC) {
		if (anc->builtInType == XML_SCHEMAS_NORMSTRING) {
		    type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_REPLACE;

		} else if ((anc->builtInType == XML_SCHEMAS_STRING) ||
		    (anc->builtInType == XML_SCHEMAS_ANYSIMPLETYPE)) {
		    type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_PRESERVE;

		} else
		    type->flags |= XML_SCHEMAS_TYPE_WHITESPACE_COLLAPSE;
		break;
	    }
	}
    }
    return (0);
}

static int
xmlSchemaFixupSimpleTypeStageOne(xmlSchemaParserCtxtPtr pctxt,
			  xmlSchemaTypePtr type)
{
    if (type->type != XML_SCHEMA_TYPE_SIMPLE)
	return(0);
    if (! WXS_IS_TYPE_NOT_FIXED_1(type))
	return(0);
    type->flags |= XML_SCHEMAS_TYPE_FIXUP_1;

    if (WXS_IS_LIST(type)) {
	if (type->subtypes == NULL) {
	    PERROR_INT("xmlSchemaFixupSimpleTypeStageOne",
		"list type has no item-type assigned");
	    return(-1);
	}
    } else if (WXS_IS_UNION(type)) {
	if (type->memberTypes == NULL) {
	    PERROR_INT("xmlSchemaFixupSimpleTypeStageOne",
		"union type has no member-types assigned");
	    return(-1);
	}
    } else {
	if (type->baseType == NULL) {
	    PERROR_INT("xmlSchemaFixupSimpleTypeStageOne",
		"type has no base-type assigned");
	    return(-1);
	}
	if (WXS_IS_TYPE_NOT_FIXED_1(type->baseType))
	    if (xmlSchemaFixupSimpleTypeStageOne(pctxt, type->baseType) == -1)
		return(-1);
	if (WXS_IS_ATOMIC(type->baseType))
	    type->flags |= XML_SCHEMAS_TYPE_VARIETY_ATOMIC;
	else if (WXS_IS_LIST(type->baseType)) {
	    type->flags |= XML_SCHEMAS_TYPE_VARIETY_LIST;
	    type->subtypes = type->baseType->subtypes;
	} else if (WXS_IS_UNION(type->baseType)) {
	    type->flags |= XML_SCHEMAS_TYPE_VARIETY_UNION;
	}
    }
    return(0);
}

#ifdef DEBUG_TYPE
static void
xmlSchemaDebugFixedType(xmlSchemaParserCtxtPtr pctxt,
		       xmlSchemaTypePtr type)
{
    if (type->node != NULL) {
        xmlGenericError(xmlGenericErrorContext,
                        "Type of %s : %s:%d :", name,
                        type->node->doc->URL,
                        xmlGetLineNo(type->node));
    } else {
        xmlGenericError(xmlGenericErrorContext, "Type of %s :", name);
    }
    if ((WXS_IS_SIMPLE(type)) || (WXS_IS_COMPLEX(type))) {
	switch (type->contentType) {
	    case XML_SCHEMA_CONTENT_SIMPLE:
		xmlGenericError(xmlGenericErrorContext, "simple\n");
		break;
	    case XML_SCHEMA_CONTENT_ELEMENTS:
		xmlGenericError(xmlGenericErrorContext, "elements\n");
		break;
	    case XML_SCHEMA_CONTENT_UNKNOWN:
		xmlGenericError(xmlGenericErrorContext, "unknown !!!\n");
		break;
	    case XML_SCHEMA_CONTENT_EMPTY:
		xmlGenericError(xmlGenericErrorContext, "empty\n");
		break;
	    case XML_SCHEMA_CONTENT_MIXED:
		if (xmlSchemaIsParticleEmptiable((xmlSchemaParticlePtr)
		    type->subtypes))
		    xmlGenericError(xmlGenericErrorContext,
			"mixed as emptiable particle\n");
		else
		    xmlGenericError(xmlGenericErrorContext, "mixed\n");
		break;
		
	    case XML_SCHEMA_CONTENT_BASIC:
		xmlGenericError(xmlGenericErrorContext, "basic\n");
		break;
	    default:
		xmlGenericError(xmlGenericErrorContext,
		    "not registered !!!\n");
		break;
	}
    }
}
#endif

static int
xmlSchemaFixupSimpleTypeStageTwo(xmlSchemaParserCtxtPtr pctxt,
				 xmlSchemaTypePtr type)
{
    int res, olderrs = pctxt->nberrors;

    if (type->type != XML_SCHEMA_TYPE_SIMPLE)
	return(-1);

    if (! WXS_IS_TYPE_NOT_FIXED(type))
	return(0);

    type->flags |= XML_SCHEMAS_TYPE_INTERNAL_RESOLVED;
    type->contentType = XML_SCHEMA_CONTENT_SIMPLE;

    if (type->baseType == NULL) {
	PERROR_INT("xmlSchemaFixupSimpleTypeStageTwo",
	    "missing baseType");
	goto exit_failure;
    }
    if (WXS_IS_TYPE_NOT_FIXED(type->baseType))
	xmlSchemaTypeFixup(type->baseType, ACTXT_CAST pctxt);
    if ((type->memberTypes != NULL) &&
	(xmlSchemaFinishMemberTypeDefinitionsProperty(pctxt, type) == -1))
	return(-1);
    res = xmlSchemaCheckSTPropsCorrect(pctxt, type);
    HFAILURE HERROR
    res = xmlSchemaCheckCOSSTRestricts(pctxt, type);
    HFAILURE HERROR
    res = xmlSchemaCheckFacetValues(type, pctxt);
    HFAILURE HERROR
    if ((type->facetSet != NULL) ||
	(type->baseType->facetSet != NULL)) {
	res = xmlSchemaDeriveAndValidateFacets(pctxt, type);
	HFAILURE HERROR
    }
    res = xmlSchemaTypeFixupWhitespace(type);
    HFAILURE HERROR
    xmlSchemaTypeFixupOptimFacets(type);

exit_error:
#ifdef DEBUG_TYPE
    xmlSchemaDebugFixedType(pctxt, type);
#endif
    if (olderrs != pctxt->nberrors)
	return(pctxt->err);
    return(0);

exit_failure:
#ifdef DEBUG_TYPE
    xmlSchemaDebugFixedType(pctxt, type);
#endif
    return(-1);
}

static int
xmlSchemaFixupComplexType(xmlSchemaParserCtxtPtr pctxt,
			  xmlSchemaTypePtr type)
{
    int res = 0, olderrs = pctxt->nberrors;
    xmlSchemaTypePtr baseType = type->baseType;

    if (! WXS_IS_TYPE_NOT_FIXED(type))
	return(0);
    type->flags |= XML_SCHEMAS_TYPE_INTERNAL_RESOLVED;
    if (baseType == NULL) {
	PERROR_INT("xmlSchemaFixupComplexType",
	    "missing baseType");
	goto exit_failure;
    }
    if (WXS_IS_TYPE_NOT_FIXED(baseType))
	xmlSchemaTypeFixup(baseType, ACTXT_CAST pctxt);
    if (baseType->flags & XML_SCHEMAS_TYPE_INTERNAL_INVALID) {
	return(0);
    }
    res = xmlSchemaCheckSRCCT(pctxt, type);
    HFAILURE HERROR
    if (type->contentType == XML_SCHEMA_CONTENT_SIMPLE) {
	if ((WXS_IS_COMPLEX(baseType)) &&
	    (baseType->contentTypeDef != NULL) &&
	    (WXS_IS_RESTRICTION(type))) {
	    xmlSchemaTypePtr contentBase, content;
#ifdef ENABLE_NAMED_LOCALS
	    char buf[30];
	    const xmlChar *tmpname;
#endif
	    if (type->contentTypeDef != NULL) {
		contentBase = type->contentTypeDef;
		type->contentTypeDef = NULL;
	    } else {
		contentBase = baseType->contentTypeDef;
	    }
#ifdef ENABLE_NAMED_LOCALS
	    snprintf(buf, 29, "#scST%d", ++(pctxt->counter));
	    tmpname = xmlDictLookup(pctxt->dict, BAD_CAST buf, -1);
	    content = xmlSchemaAddType(pctxt, pctxt->schema,
		XML_SCHEMA_TYPE_SIMPLE, tmpname, type->targetNamespace,
		type->node, 0);
#else
	    content = xmlSchemaAddType(pctxt, pctxt->schema,
		XML_SCHEMA_TYPE_SIMPLE, NULL, type->targetNamespace,
		type->node, 0);
#endif
	    if (content == NULL)
		goto exit_failure;
	    content->type = XML_SCHEMA_TYPE_SIMPLE;
	    content->baseType = contentBase;
	    content->facets = type->facets;
	    type->facets = NULL;
	    content->facetSet = type->facetSet;
	    type->facetSet = NULL;

	    type->contentTypeDef = content;
	    if (WXS_IS_TYPE_NOT_FIXED(contentBase))
		xmlSchemaTypeFixup(contentBase, ACTXT_CAST pctxt);
	    res = xmlSchemaFixupSimpleTypeStageOne(pctxt, content);
	    HFAILURE HERROR
	    res = xmlSchemaFixupSimpleTypeStageTwo(pctxt, content);
	    HFAILURE HERROR

	} else if ((WXS_IS_COMPLEX(baseType)) &&
	    (baseType->contentType == XML_SCHEMA_CONTENT_MIXED) &&
	    (WXS_IS_RESTRICTION(type))) {
	    if ((type->contentTypeDef == NULL) ||
		(type->contentTypeDef->baseType == NULL)) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_INTERNAL,
		    WXS_BASIC_CAST type, NULL,
		    "Internal error: xmlSchemaTypeFixup, "
		    "complex type '%s': the <simpleContent><restriction> "
		    "is missing a <simpleType> child, but was not catched "
		    "by xmlSchemaCheckSRCCT()", type->name);
		goto exit_failure;
	    }
	} else if ((WXS_IS_COMPLEX(baseType)) && WXS_IS_EXTENSION(type)) {
	    if (baseType->contentTypeDef == NULL) {
		xmlSchemaPCustomErr(pctxt,
		    XML_SCHEMAP_INTERNAL,
		    WXS_BASIC_CAST type, NULL,
		    "Internal error: xmlSchemaTypeFixup, "
		    "complex type '%s': the <extension>ed base type is "
		    "a complex type with no simple content type",
		    type->name);
		goto exit_failure;
	    }
	    type->contentTypeDef = baseType->contentTypeDef;
	} else if ((WXS_IS_SIMPLE(baseType)) && WXS_IS_EXTENSION(type)) {
	    type->contentTypeDef = baseType;
	} else {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_INTERNAL,
		WXS_BASIC_CAST type, NULL,
		"Internal error: xmlSchemaTypeFixup, "
		"complex type '%s' with <simpleContent>: unhandled "
		"derivation case", type->name);
	    goto exit_failure;
	}
    } else {
	int dummySequence = 0;
	xmlSchemaParticlePtr particle =
	    (xmlSchemaParticlePtr) type->subtypes;
	if ((particle == NULL) ||
	    ((particle->type == XML_SCHEMA_TYPE_PARTICLE) &&
	    ((particle->children->type == XML_SCHEMA_TYPE_ALL) ||
	    (particle->children->type == XML_SCHEMA_TYPE_SEQUENCE) ||
	    ((particle->children->type == XML_SCHEMA_TYPE_CHOICE) &&
	    (particle->minOccurs == 0))) &&
	    ( ((xmlSchemaTreeItemPtr) particle->children)->children == NULL))) {
	    if (type->flags & XML_SCHEMAS_TYPE_MIXED) {
		if ((particle == NULL) ||
		    (particle->children->type != XML_SCHEMA_TYPE_SEQUENCE)) {
		    particle = xmlSchemaAddParticle(pctxt,
			type->node, 1, 1);
		    if (particle == NULL)
			goto exit_failure;
 
		    particle->children = (xmlSchemaTreeItemPtr)
			xmlSchemaAddModelGroup(pctxt, pctxt->schema,
			XML_SCHEMA_TYPE_SEQUENCE, type->node);
		    if (particle->children == NULL)
			goto exit_failure;

		    type->subtypes = (xmlSchemaTypePtr) particle;
		}
		dummySequence = 1;
		type->contentType = XML_SCHEMA_CONTENT_ELEMENTS;
	    } else {
		type->contentType = XML_SCHEMA_CONTENT_EMPTY;
	    }
	} else {
	    type->contentType = XML_SCHEMA_CONTENT_ELEMENTS;
	}
	if (WXS_IS_RESTRICTION(type)) {
	    if (type->contentType != XML_SCHEMA_CONTENT_EMPTY) {
		if (type->flags & XML_SCHEMAS_TYPE_MIXED)
		    type->contentType = XML_SCHEMA_CONTENT_MIXED;
	    }
	} else {
	    if (type->contentType == XML_SCHEMA_CONTENT_EMPTY) {
		type->contentType = baseType->contentType;
		type->subtypes = baseType->subtypes;
		type->contentTypeDef = baseType->contentTypeDef;
	    } else if (baseType->contentType == XML_SCHEMA_CONTENT_EMPTY) {
		if (type->flags & XML_SCHEMAS_TYPE_MIXED)
		    type->contentType = XML_SCHEMA_CONTENT_MIXED;
	    } else {
		if (type->flags & XML_SCHEMAS_TYPE_MIXED)
		    type->contentType = XML_SCHEMA_CONTENT_MIXED;
		if ((WXS_TYPE_PARTICLE(type) != NULL) &&
		    (WXS_TYPE_PARTICLE_TERM(type) != NULL) &&
		    ((WXS_TYPE_PARTICLE_TERM(type))->type ==
			XML_SCHEMA_TYPE_ALL))
		{
		    xmlSchemaCustomErr(ACTXT_CAST pctxt,
			
			XML_SCHEMAP_COS_ALL_LIMITED,
			WXS_ITEM_NODE(type), NULL,
			"The type has an 'all' model group in its "
			"{content type} and thus cannot be derived from "
			"a non-empty type, since this would produce a "
			"'sequence' model group containing the 'all' "
			"model group; 'all' model groups are not "
			"allowed to appear inside other model groups",
			NULL, NULL);

		} else if ((WXS_TYPE_PARTICLE(baseType) != NULL) &&
		    (WXS_TYPE_PARTICLE_TERM(baseType) != NULL) &&
		    ((WXS_TYPE_PARTICLE_TERM(baseType))->type ==
			XML_SCHEMA_TYPE_ALL))
		{
		    xmlSchemaCustomErr(ACTXT_CAST pctxt,
			
			XML_SCHEMAP_COS_ALL_LIMITED,
			WXS_ITEM_NODE(type), NULL,
			"A type cannot be derived by extension from a type "
			"which has an 'all' model group in its "
			"{content type}, since this would produce a "
			"'sequence' model group containing the 'all' "
			"model group; 'all' model groups are not "
			"allowed to appear inside other model groups",
			NULL, NULL);

		} else if (! dummySequence) {
		    xmlSchemaTreeItemPtr effectiveContent =
			(xmlSchemaTreeItemPtr) type->subtypes;
		    particle = xmlSchemaAddParticle(pctxt,
			type->node, 1, 1);
		    if (particle == NULL)
			goto exit_failure;
		    particle->children = (xmlSchemaTreeItemPtr)
			xmlSchemaAddModelGroup(pctxt, pctxt->schema,
			XML_SCHEMA_TYPE_SEQUENCE, type->node);
		    if (particle->children == NULL)
			goto exit_failure;
		    WXS_TYPE_CONTENTTYPE(type) = (xmlSchemaTypePtr) particle;
		    particle->children->children =
			(xmlSchemaTreeItemPtr) xmlSchemaAddParticle(pctxt,
			type->node,
			((xmlSchemaParticlePtr) type->subtypes)->minOccurs,
			((xmlSchemaParticlePtr) type->subtypes)->maxOccurs);
		    if (particle->children->children == NULL)
			goto exit_failure;
		    particle = (xmlSchemaParticlePtr)
			particle->children->children;
		    particle->children =
			((xmlSchemaParticlePtr) baseType->subtypes)->children;
		    particle->next = effectiveContent;
		} else {
		    particle->children->children =
			(xmlSchemaTreeItemPtr) baseType->subtypes;
		}
	    }
	}
    }
    res = xmlSchemaFixupTypeAttributeUses(pctxt, type);
    HFAILURE HERROR
    res = xmlSchemaCheckCTComponent(pctxt, type);
    HFAILURE HERROR

#ifdef DEBUG_TYPE
    xmlSchemaDebugFixedType(pctxt, type);
#endif
    if (olderrs != pctxt->nberrors)
	return(pctxt->err);
    else
	return(0);

exit_error:
    type->flags |= XML_SCHEMAS_TYPE_INTERNAL_INVALID;
#ifdef DEBUG_TYPE
    xmlSchemaDebugFixedType(pctxt, type);
#endif
    return(pctxt->err);

exit_failure:
    type->flags |= XML_SCHEMAS_TYPE_INTERNAL_INVALID;
#ifdef DEBUG_TYPE
    xmlSchemaDebugFixedType(pctxt, type);
#endif
    return(-1);
}


static int
xmlSchemaTypeFixup(xmlSchemaTypePtr type,
                   xmlSchemaAbstractCtxtPtr actxt)
{
    if (type == NULL)
        return(0);
    if (actxt->type != XML_SCHEMA_CTXT_PARSER) {
	AERROR_INT("xmlSchemaTypeFixup",
	    "this function needs a parser context");
	return(-1);
    }
    if (! WXS_IS_TYPE_NOT_FIXED(type))
	return(0);
    if (type->type == XML_SCHEMA_TYPE_COMPLEX)
	return(xmlSchemaFixupComplexType(PCTXT_CAST actxt, type));
    else if (type->type == XML_SCHEMA_TYPE_SIMPLE)
	return(xmlSchemaFixupSimpleTypeStageTwo(PCTXT_CAST actxt, type));
    return(0);
}

int
xmlSchemaCheckFacet(xmlSchemaFacetPtr facet,
                    xmlSchemaTypePtr typeDecl,
                    xmlSchemaParserCtxtPtr pctxt,
		    const xmlChar * name ATTRIBUTE_UNUSED)
{
    int ret = 0, ctxtGiven;

    if ((facet == NULL) || (typeDecl == NULL))
        return(-1);
    if (pctxt == NULL)
	ctxtGiven = 0;
    else
	ctxtGiven = 1;

    switch (facet->type) {
        case XML_SCHEMA_FACET_MININCLUSIVE:
        case XML_SCHEMA_FACET_MINEXCLUSIVE:
        case XML_SCHEMA_FACET_MAXINCLUSIVE:
        case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	case XML_SCHEMA_FACET_ENUMERATION: {
		xmlSchemaTypePtr base;

		if (typeDecl->type != XML_SCHEMA_TYPE_BASIC) {
		    base = typeDecl->baseType;
		    if (base == NULL) {
			PERROR_INT("xmlSchemaCheckFacet",
			    "a type user derived type has no base type");
			return (-1);
		    }
		} else
		    base = typeDecl;

		if (! ctxtGiven) {
		    pctxt = xmlSchemaNewParserCtxt("*");
		    if (pctxt == NULL)
			return (-1);
		}
		ret = xmlSchemaVCheckCVCSimpleType(
		    ACTXT_CAST pctxt, facet->node, base,
		    facet->value, &(facet->val), 1, 1, 0);
                if (ret != 0) {
		    if (ret < 0) {
			
			if (ctxtGiven) {
			    xmlSchemaCustomErr(ACTXT_CAST pctxt,
				XML_SCHEMAP_INTERNAL, facet->node, NULL,
				"Internal error: xmlSchemaCheckFacet, "
				"failed to validate the value '%s' of the "
				"facet '%s' against the base type",
				facet->value, xmlSchemaFacetTypeToString(facet->type));
			}
			goto internal_error;
		    }
		    ret = XML_SCHEMAP_INVALID_FACET_VALUE;
		    
		    if (ctxtGiven) {
			xmlChar *str = NULL;

			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    ret, facet->node, WXS_BASIC_CAST facet,
			    "The value '%s' of the facet does not validate "
			    "against the base type '%s'",
			    facet->value,
			    xmlSchemaFormatQName(&str,
				base->targetNamespace, base->name));
			FREE_AND_NULL(str);
		    }
		    goto exit;
                } else if (facet->val == NULL) {
		    if (ctxtGiven) {
			PERROR_INT("xmlSchemaCheckFacet",
			    "value was not computed");
		    }
		    TODO
		}
                break;
            }
        case XML_SCHEMA_FACET_PATTERN:
            facet->regexp = xmlRegexpCompile(facet->value);
            if (facet->regexp == NULL) {
		ret = XML_SCHEMAP_REGEXP_INVALID;
		
		if (ctxtGiven) {
		    xmlSchemaCustomErr(ACTXT_CAST pctxt,
			ret, facet->node, WXS_BASIC_CAST typeDecl,
			"The value '%s' of the facet 'pattern' is not a "
			"valid regular expression",
			facet->value, NULL);
		}
            }
            break;
        case XML_SCHEMA_FACET_TOTALDIGITS:
        case XML_SCHEMA_FACET_FRACTIONDIGITS:
        case XML_SCHEMA_FACET_LENGTH:
        case XML_SCHEMA_FACET_MAXLENGTH:
        case XML_SCHEMA_FACET_MINLENGTH:

	    if (facet->type == XML_SCHEMA_FACET_TOTALDIGITS) {
		ret = xmlSchemaValidatePredefinedType(
		    xmlSchemaGetBuiltInType(XML_SCHEMAS_PINTEGER),
		    facet->value, &(facet->val));
	    } else {
		ret = xmlSchemaValidatePredefinedType(
		    xmlSchemaGetBuiltInType(XML_SCHEMAS_NNINTEGER),
		    facet->value, &(facet->val));
	    }
	    if (ret != 0) {
		if (ret < 0) {
		    
		    if (ctxtGiven) {
			PERROR_INT("xmlSchemaCheckFacet",
			    "validating facet value");
		    }
		    goto internal_error;
		}
		ret = XML_SCHEMAP_INVALID_FACET_VALUE;
		
		if (ctxtGiven) {
		    
		    xmlSchemaCustomErr4(ACTXT_CAST pctxt,
			ret, facet->node, WXS_BASIC_CAST typeDecl,
			"The value '%s' of the facet '%s' is not a valid '%s'",
			facet->value,
			xmlSchemaFacetTypeToString(facet->type),
			(facet->type != XML_SCHEMA_FACET_TOTALDIGITS) ?
			    BAD_CAST "nonNegativeInteger" :
			    BAD_CAST "positiveInteger",
			NULL);
		}
	    }
	    break;

        case XML_SCHEMA_FACET_WHITESPACE:{
                if (xmlStrEqual(facet->value, BAD_CAST "preserve")) {
                    facet->whitespace = XML_SCHEMAS_FACET_PRESERVE;
                } else if (xmlStrEqual(facet->value, BAD_CAST "replace")) {
                    facet->whitespace = XML_SCHEMAS_FACET_REPLACE;
                } else if (xmlStrEqual(facet->value, BAD_CAST "collapse")) {
                    facet->whitespace = XML_SCHEMAS_FACET_COLLAPSE;
                } else {
		    ret = XML_SCHEMAP_INVALID_FACET_VALUE;
                    
		    if (ctxtGiven) {
			
			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    ret, facet->node, WXS_BASIC_CAST typeDecl,
			    "The value '%s' of the facet 'whitespace' is not "
			    "valid", facet->value, NULL);
                    }
                }
            }
        default:
            break;
    }
exit:
    if ((! ctxtGiven) && (pctxt != NULL))
	xmlSchemaFreeParserCtxt(pctxt);
    return (ret);
internal_error:
    if ((! ctxtGiven) && (pctxt != NULL))
	xmlSchemaFreeParserCtxt(pctxt);
    return (-1);
}

static int
xmlSchemaCheckFacetValues(xmlSchemaTypePtr typeDecl,
			  xmlSchemaParserCtxtPtr pctxt)
{
    int res, olderrs = pctxt->nberrors;
    const xmlChar *name = typeDecl->name;
    if (typeDecl->facets != NULL) {
	xmlSchemaFacetPtr facet = typeDecl->facets;

	if (pctxt->vctxt == NULL) {
	    if (xmlSchemaCreateVCtxtOnPCtxt(pctxt) == -1)
		return(-1);
	}
	pctxt->vctxt->schema = pctxt->schema;
	while (facet != NULL) {
	    res = xmlSchemaCheckFacet(facet, typeDecl, pctxt, name);
	    HFAILURE
	    facet = facet->next;
	}
	pctxt->vctxt->schema = NULL;
    }
    if (olderrs != pctxt->nberrors)
	return(pctxt->err);
    return(0);
exit_failure:
    return(-1);
}

static xmlSchemaTreeItemPtr
xmlSchemaGetCircModelGrDefRef(xmlSchemaModelGroupDefPtr groupDef,
			      xmlSchemaTreeItemPtr particle)
{
    xmlSchemaTreeItemPtr circ = NULL;
    xmlSchemaTreeItemPtr term;
    xmlSchemaModelGroupDefPtr gdef;

    for (; particle != NULL; particle = particle->next) {
	term = particle->children;
	if (term == NULL)
	    continue;
	switch (term->type) {
	    case XML_SCHEMA_TYPE_GROUP:
		gdef = (xmlSchemaModelGroupDefPtr) term;
		if (gdef == groupDef)
		    return (particle);
		if (gdef->flags & XML_SCHEMA_MODEL_GROUP_DEF_MARKED)
		    continue;
		if (gdef->children != NULL) {
		    gdef->flags |= XML_SCHEMA_MODEL_GROUP_DEF_MARKED;
		    circ = xmlSchemaGetCircModelGrDefRef(groupDef,
			gdef->children->children);
		    gdef->flags ^= XML_SCHEMA_MODEL_GROUP_DEF_MARKED;
		    if (circ != NULL)
			return (circ);
		}
		break;
	    case XML_SCHEMA_TYPE_SEQUENCE:
	    case XML_SCHEMA_TYPE_CHOICE:
	    case XML_SCHEMA_TYPE_ALL:
		circ = xmlSchemaGetCircModelGrDefRef(groupDef, term->children);
		if (circ != NULL)
		    return (circ);
		break;
	    default:
		break;
	}
    }
    return (NULL);
}

static void
xmlSchemaCheckGroupDefCircular(xmlSchemaModelGroupDefPtr item,
			       xmlSchemaParserCtxtPtr ctxt)
{
    if ((item == NULL) ||
	(item->type != XML_SCHEMA_TYPE_GROUP) ||
	(item->children == NULL))
	return;
    {
	xmlSchemaTreeItemPtr circ;

	circ = xmlSchemaGetCircModelGrDefRef(item, item->children->children);
	if (circ != NULL) {
	    xmlChar *str = NULL;
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_MG_PROPS_CORRECT_2,
		NULL, WXS_ITEM_NODE(circ),
		"Circular reference to the model group definition '%s' "
		"defined", xmlSchemaFormatQName(&str,
		    item->targetNamespace, item->name));
	    FREE_AND_NULL(str)
	    circ->children = NULL;
	}
    }
}

static void
xmlSchemaModelGroupToModelGroupDefFixup(
    xmlSchemaParserCtxtPtr ctxt ATTRIBUTE_UNUSED,
    xmlSchemaModelGroupPtr mg)
{
    xmlSchemaParticlePtr particle = WXS_MODELGROUP_PARTICLE(mg);

    while (particle != NULL) {
	if ((WXS_PARTICLE_TERM(particle) == NULL) ||
	    ((WXS_PARTICLE_TERM(particle))->type !=
		XML_SCHEMA_TYPE_GROUP))
	{
	    particle = WXS_PTC_CAST particle->next;
	    continue;
	}
	if (WXS_MODELGROUPDEF_MODEL(WXS_PARTICLE_TERM(particle)) == NULL) {
	    WXS_PARTICLE_TERM(particle) = NULL;
	    particle = WXS_PTC_CAST particle->next;
	    continue;
	}
	WXS_PARTICLE_TERM(particle) =
	    WXS_TREE_CAST WXS_MODELGROUPDEF_MODEL(WXS_PARTICLE_TERM(particle));

	particle = WXS_PTC_CAST particle->next;
    }
}

static xmlSchemaQNameRefPtr
xmlSchemaCheckAttrGroupCircularRecur(xmlSchemaAttributeGroupPtr ctxtGr,
				     xmlSchemaItemListPtr list)
{
    xmlSchemaAttributeGroupPtr gr;
    xmlSchemaQNameRefPtr ref, circ;
    int i;
    for (i = 0; i < list->nbItems; i++) {
	ref = list->items[i];
	if ((ref->type == XML_SCHEMA_EXTRA_QNAMEREF) &&
	    (ref->itemType == XML_SCHEMA_TYPE_ATTRIBUTEGROUP) &&
	    (ref->item != NULL))
	{
	    gr = WXS_ATTR_GROUP_CAST ref->item;
	    if (gr == ctxtGr)
		return(ref);
	    if (gr->flags & XML_SCHEMAS_ATTRGROUP_MARKED)
		continue;
	    if ((gr->attrUses) &&
		(gr->flags & XML_SCHEMAS_ATTRGROUP_HAS_REFS))
	    {
		gr->flags |= XML_SCHEMAS_ATTRGROUP_MARKED;
		circ = xmlSchemaCheckAttrGroupCircularRecur(ctxtGr,
		    (xmlSchemaItemListPtr) gr->attrUses);
		gr->flags ^= XML_SCHEMAS_ATTRGROUP_MARKED;
		if (circ != NULL)
		    return (circ);
	    }

	}
    }
    return (NULL);
}

static int
xmlSchemaCheckAttrGroupCircular(xmlSchemaAttributeGroupPtr attrGr,
				xmlSchemaParserCtxtPtr ctxt)
{
    if (attrGr->attrUses == NULL)
	return(0);
    else if ((attrGr->flags & XML_SCHEMAS_ATTRGROUP_HAS_REFS) == 0)
	return(0);
    else {
	xmlSchemaQNameRefPtr circ;

	circ = xmlSchemaCheckAttrGroupCircularRecur(attrGr,
	    (xmlSchemaItemListPtr) attrGr->attrUses);
	if (circ != NULL) {
	    xmlChar *str = NULL;
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_SRC_ATTRIBUTE_GROUP_3,
		NULL, WXS_ITEM_NODE(WXS_BASIC_CAST circ),
		"Circular reference to the attribute group '%s' "
		"defined", xmlSchemaGetComponentQName(&str, attrGr));
	    FREE_AND_NULL(str);
	    circ->item = NULL;
	    return(ctxt->err);
	}
    }
    return(0);
}

static int
xmlSchemaAttributeGroupExpandRefs(xmlSchemaParserCtxtPtr pctxt,
				  xmlSchemaAttributeGroupPtr attrGr);

static int
xmlSchemaExpandAttributeGroupRefs(xmlSchemaParserCtxtPtr pctxt,
				  xmlSchemaBasicItemPtr item,
				  xmlSchemaWildcardPtr *completeWild,
				  xmlSchemaItemListPtr list,
				  xmlSchemaItemListPtr prohibs)
{
    xmlSchemaAttributeGroupPtr gr;
    xmlSchemaAttributeUsePtr use;
    xmlSchemaItemListPtr sublist;
    int i, j;
    int created = (*completeWild == NULL) ? 0 : 1;

    if (prohibs)
	prohibs->nbItems = 0;

    for (i = 0; i < list->nbItems; i++) {
	use = list->items[i];

	if (use->type == XML_SCHEMA_EXTRA_ATTR_USE_PROHIB) {
	    if (prohibs == NULL) {
		PERROR_INT("xmlSchemaExpandAttributeGroupRefs",
		    "unexpected attr prohibition found");
		return(-1);
	    }
	    if (xmlSchemaItemListRemove(list, i) == -1)
		return(-1);
	    i--;
	    xmlSchemaItemListAddSize(prohibs, 2, use);
	    continue;
	}
	if ((use->type == XML_SCHEMA_EXTRA_QNAMEREF) &&
	    ((WXS_QNAME_CAST use)->itemType == XML_SCHEMA_TYPE_ATTRIBUTEGROUP))
	{
	    if ((WXS_QNAME_CAST use)->item == NULL)
		return(-1);
	    gr = WXS_ATTR_GROUP_CAST (WXS_QNAME_CAST use)->item;
	    if ((gr->flags & XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED) == 0) {
		if (xmlSchemaAttributeGroupExpandRefs(pctxt, gr) == -1)
		    return(-1);
	    }
	    if (gr->attributeWildcard != NULL) {
		if (*completeWild == NULL) {
		    *completeWild = gr->attributeWildcard;
		} else {
		    if (! created) {
			xmlSchemaWildcardPtr tmpWild;

			tmpWild =  xmlSchemaAddWildcard(pctxt, pctxt->schema,
			    XML_SCHEMA_TYPE_ANY_ATTRIBUTE,
			    WXS_ITEM_NODE(item));
			if (tmpWild == NULL)
			    return(-1);
			if (xmlSchemaCloneWildcardNsConstraints(pctxt,
			    tmpWild, *completeWild) == -1)
			    return (-1);
			tmpWild->processContents = (*completeWild)->processContents;
			*completeWild = tmpWild;
			created = 1;
		    }

		    if (xmlSchemaIntersectWildcards(pctxt, *completeWild,
			gr->attributeWildcard) == -1)
			return(-1);
		}
	    }
	    sublist = ((xmlSchemaItemListPtr) gr->attrUses);
	    if ((sublist == NULL) || sublist->nbItems == 0) {
		if (xmlSchemaItemListRemove(list, i) == -1)
		    return(-1);
		i--;
		continue;
	    }
	    list->items[i] = sublist->items[0];
	    if (sublist->nbItems != 1) {
		for (j = 1; j < sublist->nbItems; j++) {
		    i++;
		    if (xmlSchemaItemListInsert(list,
			    sublist->items[j], i) == -1)
			return(-1);
		}
	    }
	}

    }
    if (prohibs && (prohibs->nbItems != 0) && (list->nbItems != 0)) {
	xmlSchemaAttributeUseProhibPtr prohib;

	for (i = prohibs->nbItems -1; i >= 0; i--) {
	    prohib = prohibs->items[i];
	    for (j = 0; j < list->nbItems; j++) {
		use = list->items[j];

		if ((prohib->name == WXS_ATTRUSE_DECL_NAME(use)) &&
		    (prohib->targetNamespace == WXS_ATTRUSE_DECL_TNS(use)))
		{
		    xmlChar *str = NULL;

		    xmlSchemaCustomWarning(ACTXT_CAST pctxt,
			XML_SCHEMAP_WARN_ATTR_POINTLESS_PROH,
			prohib->node, NULL,
			"Skipping pointless attribute use prohibition "
			"'%s', since a corresponding attribute use "
			"exists already in the type definition",
			xmlSchemaFormatQName(&str,
			    prohib->targetNamespace, prohib->name),
			NULL, NULL);
		    FREE_AND_NULL(str);
		    if (xmlSchemaItemListRemove(prohibs, i) == -1)
			return(-1);
		    break;
		}
	    }
	}
    }
    return(0);
}

static int
xmlSchemaAttributeGroupExpandRefs(xmlSchemaParserCtxtPtr pctxt,
				  xmlSchemaAttributeGroupPtr attrGr)
{
    if ((attrGr->attrUses == NULL) ||
	(attrGr->flags & XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED))
	return(0);

    attrGr->flags |= XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED;
    if (xmlSchemaExpandAttributeGroupRefs(pctxt, WXS_BASIC_CAST attrGr,
	&(attrGr->attributeWildcard), attrGr->attrUses, NULL) == -1)
	return(-1);
    return(0);
}

static int
xmlSchemaCheckAGPropsCorrect(xmlSchemaParserCtxtPtr pctxt,
				  xmlSchemaAttributeGroupPtr attrGr)
{

    if ((attrGr->attrUses != NULL) &&
	(WXS_LIST_CAST attrGr->attrUses)->nbItems > 1)
    {
	xmlSchemaItemListPtr uses = WXS_LIST_CAST attrGr->attrUses;
	xmlSchemaAttributeUsePtr use, tmp;
	int i, j, hasId = 0;

	for (i = uses->nbItems -1; i >= 0; i--) {
	    use = uses->items[i];
	    if (i > 0) {
		for (j = i -1; j >= 0; j--) {
		    tmp = uses->items[j];
		    if ((WXS_ATTRUSE_DECL_NAME(use) ==
			WXS_ATTRUSE_DECL_NAME(tmp)) &&
			(WXS_ATTRUSE_DECL_TNS(use) ==
			WXS_ATTRUSE_DECL_TNS(tmp)))
		    {
			xmlChar *str = NULL;

			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    XML_SCHEMAP_AG_PROPS_CORRECT,
			    attrGr->node, WXS_BASIC_CAST attrGr,
			    "Duplicate %s",
			    xmlSchemaGetComponentDesignation(&str, use),
			    NULL);
			FREE_AND_NULL(str);
			if (xmlSchemaItemListRemove(uses, i) == -1)
			    return(-1);
			goto next_use;
		    }
		}
	    }
	    if (WXS_ATTRUSE_TYPEDEF(use) != NULL) {
		if (xmlSchemaIsDerivedFromBuiltInType(
		    WXS_ATTRUSE_TYPEDEF(use), XML_SCHEMAS_ID))
		{
		    if (hasId) {
			xmlChar *str = NULL;

			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    XML_SCHEMAP_AG_PROPS_CORRECT,
			    attrGr->node, WXS_BASIC_CAST attrGr,
			    "There must not exist more than one attribute "
			    "declaration of type 'xs:ID' "
			    "(or derived from 'xs:ID'). The %s violates this "
			    "constraint",
			    xmlSchemaGetComponentDesignation(&str, use),
			    NULL);
			FREE_AND_NULL(str);
			if (xmlSchemaItemListRemove(uses, i) == -1)
			    return(-1);
		    }
		    hasId = 1;
		}
	    }
next_use: {}
	}
    }
    return(0);
}

static int
xmlSchemaResolveAttrGroupReferences(xmlSchemaQNameRefPtr ref,
				    xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaAttributeGroupPtr group;

    if (ref->item != NULL)
        return(0);
    group = xmlSchemaGetAttributeGroup(ctxt->schema,
	ref->name,
	ref->targetNamespace);
    if (group == NULL) {
	xmlSchemaPResCompAttrErr(ctxt,
	    XML_SCHEMAP_SRC_RESOLVE,
	    NULL, ref->node,
	    "ref", ref->name, ref->targetNamespace,
	    ref->itemType, NULL);
	return(ctxt->err);
    }
    ref->item = WXS_BASIC_CAST group;
    return(0);
}

static int
xmlSchemaCheckAttrPropsCorrect(xmlSchemaParserCtxtPtr pctxt,
			       xmlSchemaAttributePtr attr)
{


    if (WXS_ATTR_TYPEDEF(attr) == NULL)
	return(0);

    if (attr->defValue != NULL) {
	int ret;

	if (xmlSchemaIsDerivedFromBuiltInType(
	    WXS_ATTR_TYPEDEF(attr), XML_SCHEMAS_ID))
	{
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_A_PROPS_CORRECT_3,
		NULL, WXS_BASIC_CAST attr,
		"Value constraints are not allowed if the type definition "
		"is or is derived from xs:ID",
		NULL, NULL);
	    return(pctxt->err);
	}
	ret = xmlSchemaVCheckCVCSimpleType(ACTXT_CAST pctxt,
	    attr->node, WXS_ATTR_TYPEDEF(attr),
	    attr->defValue, &(attr->defVal),
	    1, 1, 0);
	if (ret != 0) {
	    if (ret < 0) {
		PERROR_INT("xmlSchemaCheckAttrPropsCorrect",
		    "calling xmlSchemaVCheckCVCSimpleType()");
		return(-1);
	    }
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_A_PROPS_CORRECT_2,
		NULL, WXS_BASIC_CAST attr,
		"The value of the value constraint is not valid",
		NULL, NULL);
	    return(pctxt->err);
	}
    }

    return(0);
}

static xmlSchemaElementPtr
xmlSchemaCheckSubstGroupCircular(xmlSchemaElementPtr elemDecl,
				 xmlSchemaElementPtr ancestor)
{
    xmlSchemaElementPtr ret;

    if (WXS_SUBST_HEAD(ancestor) == NULL)
	return (NULL);
    if (WXS_SUBST_HEAD(ancestor) == elemDecl)
	return (ancestor);

    if (WXS_SUBST_HEAD(ancestor)->flags & XML_SCHEMAS_ELEM_CIRCULAR)
	return (NULL);
    WXS_SUBST_HEAD(ancestor)->flags |= XML_SCHEMAS_ELEM_CIRCULAR;
    ret = xmlSchemaCheckSubstGroupCircular(elemDecl,
	WXS_SUBST_HEAD(ancestor));
    WXS_SUBST_HEAD(ancestor)->flags ^= XML_SCHEMAS_ELEM_CIRCULAR;

    return (ret);
}

static int
xmlSchemaCheckElemPropsCorrect(xmlSchemaParserCtxtPtr pctxt,
			       xmlSchemaElementPtr elemDecl)
{
    int ret = 0;
    xmlSchemaTypePtr typeDef = WXS_ELEM_TYPEDEF(elemDecl);
    if (WXS_SUBST_HEAD(elemDecl) != NULL) {
	xmlSchemaElementPtr head = WXS_SUBST_HEAD(elemDecl), circ;

	xmlSchemaCheckElementDeclComponent(head, pctxt);
	if ((elemDecl->flags & XML_SCHEMAS_ELEM_GLOBAL) == 0) {
	    xmlSchemaPCustomErr(pctxt,
		XML_SCHEMAP_E_PROPS_CORRECT_3,
		WXS_BASIC_CAST elemDecl, NULL,
		"Only global element declarations can have a "
		"substitution group affiliation", NULL);
	    ret = XML_SCHEMAP_E_PROPS_CORRECT_3;
	}
	if (head == elemDecl)
	    circ = head;
	else if (WXS_SUBST_HEAD(head) != NULL)
	    circ = xmlSchemaCheckSubstGroupCircular(head, head);
	else
	    circ = NULL;
	if (circ != NULL) {
	    xmlChar *strA = NULL, *strB = NULL;

	    xmlSchemaPCustomErrExt(pctxt,
		XML_SCHEMAP_E_PROPS_CORRECT_6,
		WXS_BASIC_CAST circ, NULL,
		"The element declaration '%s' defines a circular "
		"substitution group to element declaration '%s'",
		xmlSchemaGetComponentQName(&strA, circ),
		xmlSchemaGetComponentQName(&strB, head),
		NULL);
	    FREE_AND_NULL(strA)
	    FREE_AND_NULL(strB)
	    ret = XML_SCHEMAP_E_PROPS_CORRECT_6;
	}

	if (typeDef != WXS_ELEM_TYPEDEF(WXS_SUBST_HEAD(elemDecl))) {
	    int set = 0;

	    if (head->flags & XML_SCHEMAS_ELEM_FINAL_EXTENSION)
		set |= SUBSET_EXTENSION;
	    if (head->flags & XML_SCHEMAS_ELEM_FINAL_RESTRICTION)
		set |= SUBSET_RESTRICTION;

	    if (xmlSchemaCheckCOSDerivedOK(ACTXT_CAST pctxt, typeDef,
		WXS_ELEM_TYPEDEF(head), set) != 0) {
		xmlChar *strA = NULL, *strB = NULL, *strC = NULL;

		ret = XML_SCHEMAP_E_PROPS_CORRECT_4;
		xmlSchemaPCustomErrExt(pctxt,
		    XML_SCHEMAP_E_PROPS_CORRECT_4,
		    WXS_BASIC_CAST elemDecl, NULL,
		    "The type definition '%s' was "
		    "either rejected by the substitution group "
		    "affiliation '%s', or not validly derived from its type "
		    "definition '%s'",
		    xmlSchemaGetComponentQName(&strA, typeDef),
		    xmlSchemaGetComponentQName(&strB, head),
		    xmlSchemaGetComponentQName(&strC, WXS_ELEM_TYPEDEF(head)));
		FREE_AND_NULL(strA)
		FREE_AND_NULL(strB)
		FREE_AND_NULL(strC)
	    }
	}
    }
    if ((elemDecl->value != NULL) &&
	((WXS_IS_SIMPLE(typeDef) &&
	  xmlSchemaIsDerivedFromBuiltInType(typeDef, XML_SCHEMAS_ID)) ||
	 (WXS_IS_COMPLEX(typeDef) &&
	  WXS_HAS_SIMPLE_CONTENT(typeDef) &&
	  xmlSchemaIsDerivedFromBuiltInType(typeDef->contentTypeDef,
	    XML_SCHEMAS_ID)))) {

	ret = XML_SCHEMAP_E_PROPS_CORRECT_5;
	xmlSchemaPCustomErr(pctxt,
	    XML_SCHEMAP_E_PROPS_CORRECT_5,
	    WXS_BASIC_CAST elemDecl, NULL,
	    "The type definition (or type definition's content type) is or "
	    "is derived from ID; value constraints are not allowed in "
	    "conjunction with such a type definition", NULL);
    } else if (elemDecl->value != NULL) {
	int vcret;
	xmlNodePtr node = NULL;

	if (typeDef == NULL) {
	    xmlSchemaPErr(pctxt, elemDecl->node,
		XML_SCHEMAP_INTERNAL,
		"Internal error: xmlSchemaCheckElemPropsCorrect, "
		"type is missing... skipping validation of "
		"the value constraint", NULL, NULL);
	    return (-1);
	}
	if (elemDecl->node != NULL) {
	    if (elemDecl->flags & XML_SCHEMAS_ELEM_FIXED)
		node = (xmlNodePtr) xmlHasProp(elemDecl->node,
		    BAD_CAST "fixed");
	    else
		node = (xmlNodePtr) xmlHasProp(elemDecl->node,
		    BAD_CAST "default");
	}
	vcret = xmlSchemaParseCheckCOSValidDefault(pctxt, node,
	    typeDef, elemDecl->value, &(elemDecl->defVal));
	if (vcret != 0) {
	    if (vcret < 0) {
		PERROR_INT("xmlSchemaElemCheckValConstr",
		    "failed to validate the value constraint of an "
		    "element declaration");
		return (-1);
	    }
	    return (vcret);
	}
    }

    return (ret);
}

static void
xmlSchemaCheckElemSubstGroup(xmlSchemaParserCtxtPtr ctxt,
			     xmlSchemaElementPtr elemDecl)
{
    if ((WXS_SUBST_HEAD(elemDecl) == NULL) ||
	
	(elemDecl->flags & XML_SCHEMAS_ELEM_ABSTRACT))
	return;
    {
	xmlSchemaElementPtr head;
	xmlSchemaTypePtr headType, type;
	int set, methSet;
	for (head = WXS_SUBST_HEAD(elemDecl); head != NULL;
	    head = WXS_SUBST_HEAD(head)) {
	    set = 0;
	    methSet = 0;
	    if (head->flags & XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION)
		continue;
	    headType = head->subtypes;
	    type = elemDecl->subtypes;
	    if (headType == type)
		goto add_member;
	    if (head->flags & XML_SCHEMAS_ELEM_BLOCK_RESTRICTION)
		set |= XML_SCHEMAS_TYPE_BLOCK_RESTRICTION;
	    if (head->flags & XML_SCHEMAS_ELEM_BLOCK_EXTENSION)
		set |= XML_SCHEMAS_TYPE_BLOCK_EXTENSION;
	    while ((type != NULL) && (type != headType)) {
		if ((WXS_IS_EXTENSION(type)) &&
		    ((methSet & XML_SCHEMAS_TYPE_BLOCK_RESTRICTION) == 0))
		    methSet |= XML_SCHEMAS_TYPE_BLOCK_EXTENSION;

		if (WXS_IS_RESTRICTION(type) &&
		    ((methSet & XML_SCHEMAS_TYPE_BLOCK_RESTRICTION) == 0))
		    methSet |= XML_SCHEMAS_TYPE_BLOCK_RESTRICTION;

		type = type->baseType;
	    }
	    type = elemDecl->subtypes->baseType;
	    while (type != NULL) {
		if (WXS_IS_COMPLEX(type)) {
		    if ((type->flags &
			    XML_SCHEMAS_TYPE_BLOCK_EXTENSION) &&
			((set & XML_SCHEMAS_TYPE_BLOCK_EXTENSION) == 0))
		    set |= XML_SCHEMAS_TYPE_BLOCK_EXTENSION;
		    if ((type->flags &
			    XML_SCHEMAS_TYPE_BLOCK_RESTRICTION) &&
			((set & XML_SCHEMAS_TYPE_BLOCK_RESTRICTION) == 0))
		    set |= XML_SCHEMAS_TYPE_BLOCK_RESTRICTION;
		} else
		    break;
		if (type == headType)
		    break;
		type = type->baseType;
	    }
	    if ((set != 0) &&
		(((set & XML_SCHEMAS_TYPE_BLOCK_EXTENSION) &&
		(methSet & XML_SCHEMAS_TYPE_BLOCK_EXTENSION)) ||
		((set & XML_SCHEMAS_TYPE_BLOCK_RESTRICTION) &&
		(methSet & XML_SCHEMAS_TYPE_BLOCK_RESTRICTION)))) {
		continue;
	    }
add_member:
	    xmlSchemaAddElementSubstitutionMember(ctxt, head, elemDecl);
	    if ((head->flags & XML_SCHEMAS_ELEM_SUBST_GROUP_HEAD) == 0)
		head->flags |= XML_SCHEMAS_ELEM_SUBST_GROUP_HEAD;
	}
    }
}

#ifdef WXS_ELEM_DECL_CONS_ENABLED 
static int
xmlSchemaCheckElementDeclConsistent(xmlSchemaParserCtxtPtr pctxt,
				    xmlSchemaBasicItemPtr ctxtComponent,
				    xmlSchemaParticlePtr ctxtParticle,
				    xmlSchemaParticlePtr searchParticle,
				    xmlSchemaParticlePtr curParticle,
				    int search)
{
    return(0);

    int ret = 0;
    xmlSchemaParticlePtr cur = curParticle;
    if (curParticle == NULL) {
	return(0);
    }
    if (WXS_PARTICLE_TERM(curParticle) == NULL) {
	return(0);
    }
    while (cur != NULL) {
	switch (WXS_PARTICLE_TERM(cur)->type) {
	    case XML_SCHEMA_TYPE_ANY:
		break;
	    case XML_SCHEMA_TYPE_ELEMENT:
		if (search == 0) {
		    ret = xmlSchemaCheckElementDeclConsistent(pctxt,
			ctxtComponent, ctxtParticle, cur, ctxtParticle, 1);
		    if (ret != 0)
			return(ret);
		} else {
		    xmlSchemaElementPtr elem =
			WXS_ELEM_CAST(WXS_PARTICLE_TERM(cur));
		    if (xmlStrEqual(WXS_PARTICLE_TERM_AS_ELEM(cur)->name,
			    WXS_PARTICLE_TERM_AS_ELEM(searchParticle)->name) &&
			xmlStrEqual(WXS_PARTICLE_TERM_AS_ELEM(cur)->targetNamespace,
			    WXS_PARTICLE_TERM_AS_ELEM(searchParticle)->targetNamespace))
		    {
			xmlChar *strA = NULL, *strB = NULL;

			xmlSchemaCustomErr(ACTXT_CAST pctxt,
			    
			    XML_SCHEMAP_COS_NONAMBIG,
			    WXS_ITEM_NODE(cur), NULL,
			    "In the content model of %s, there are multiple "
			    "element declarations for '%s' with different "
			    "type definitions",
			    xmlSchemaGetComponentDesignation(&strA,
				ctxtComponent),
			    xmlSchemaFormatQName(&strB,
				WXS_PARTICLE_TERM_AS_ELEM(cur)->targetNamespace,
				WXS_PARTICLE_TERM_AS_ELEM(cur)->name));
			FREE_AND_NULL(strA);
			FREE_AND_NULL(strB);
			return(XML_SCHEMAP_COS_NONAMBIG);
		    }
		}
		break;
	    case XML_SCHEMA_TYPE_SEQUENCE: {
		break;
		}
	    case XML_SCHEMA_TYPE_CHOICE:{
		break;
		}
	    case XML_SCHEMA_TYPE_ALL:
		break;
	    case XML_SCHEMA_TYPE_GROUP:
		break;
	    default:
		xmlSchemaInternalErr2(ACTXT_CAST pctxt,
		    "xmlSchemaCheckElementDeclConsistent",
		    "found unexpected term of type '%s' in content model",
		    WXS_ITEM_TYPE_NAME(WXS_PARTICLE_TERM(cur)), NULL);
		return(-1);
	}
	cur = (xmlSchemaParticlePtr) cur->next;
    }

exit:
    return(ret);
}
#endif

static void
xmlSchemaCheckElementDeclComponent(xmlSchemaElementPtr elemDecl,
				   xmlSchemaParserCtxtPtr ctxt)
{
    if (elemDecl == NULL)
	return;
    if (elemDecl->flags & XML_SCHEMAS_ELEM_INTERNAL_CHECKED)
	return;
    elemDecl->flags |= XML_SCHEMAS_ELEM_INTERNAL_CHECKED;
    if (xmlSchemaCheckElemPropsCorrect(ctxt, elemDecl) == 0) {
	xmlSchemaCheckElemSubstGroup(ctxt, elemDecl);
    }
}

static void
xmlSchemaResolveModelGroupParticleReferences(
    xmlSchemaParserCtxtPtr ctxt,
    xmlSchemaModelGroupPtr mg)
{
    xmlSchemaParticlePtr particle = WXS_MODELGROUP_PARTICLE(mg);
    xmlSchemaQNameRefPtr ref;
    xmlSchemaBasicItemPtr refItem;

    while (particle != NULL) {
	if ((WXS_PARTICLE_TERM(particle) == NULL) ||
	    ((WXS_PARTICLE_TERM(particle))->type !=
		XML_SCHEMA_EXTRA_QNAMEREF))
	{
	    goto next_particle;
	}
	ref = WXS_QNAME_CAST WXS_PARTICLE_TERM(particle);
	particle->children = NULL;

	refItem = xmlSchemaGetNamedComponent(ctxt->schema,
	    ref->itemType, ref->name, ref->targetNamespace);
	if (refItem == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt, XML_SCHEMAP_SRC_RESOLVE,
		NULL, WXS_ITEM_NODE(particle), "ref", ref->name,
		ref->targetNamespace, ref->itemType, NULL);
	    
	    goto next_particle;
	}
	if (refItem->type == XML_SCHEMA_TYPE_GROUP) {
	    if (WXS_MODELGROUPDEF_MODEL(refItem) == NULL)
		
		goto next_particle;
	    if ((WXS_MODELGROUPDEF_MODEL(refItem))->type ==
		    XML_SCHEMA_TYPE_ALL) {
		xmlSchemaCustomErr(ACTXT_CAST ctxt,
		    
		    XML_SCHEMAP_COS_ALL_LIMITED,
		    WXS_ITEM_NODE(particle), NULL,
		    "A model group definition is referenced, but "
		    "it contains an 'all' model group, which "
		    "cannot be contained by model groups",
		    NULL, NULL);
		
		goto next_particle;
	    }
	    particle->children = (xmlSchemaTreeItemPtr) refItem;
	} else {
	    particle->children = (xmlSchemaTreeItemPtr) refItem;
	}
next_particle:
	particle = WXS_PTC_CAST particle->next;
    }
}

static int
xmlSchemaAreValuesEqual(xmlSchemaValPtr x,
		       xmlSchemaValPtr y)
{
    xmlSchemaTypePtr tx, ty, ptx, pty;
    int ret;

    while (x != NULL) {
	
	tx = xmlSchemaGetBuiltInType(xmlSchemaGetValType(x));
	ty = xmlSchemaGetBuiltInType(xmlSchemaGetValType(y));
	ptx = xmlSchemaGetPrimitiveType(tx);
	pty = xmlSchemaGetPrimitiveType(ty);
	if (ptx != pty)
	    return(0);
	if ((ptx->builtInType == XML_SCHEMAS_STRING) ||
	    WXS_IS_ANY_SIMPLE_TYPE(ptx)) {
	    if (! xmlStrEqual(
		xmlSchemaValueGetAsString(x),
		xmlSchemaValueGetAsString(y)))
		return (0);
	} else {
	    ret = xmlSchemaCompareValuesWhtsp(
		x, XML_SCHEMA_WHITESPACE_PRESERVE,
		y, XML_SCHEMA_WHITESPACE_PRESERVE);
	    if (ret == -2)
		return(-1);
	    if (ret != 0)
		return(0);
	}
	x = xmlSchemaValueGetNext(x);
	if (x != NULL) {
	    y = xmlSchemaValueGetNext(y);
	    if (y == NULL)
		return (0);
	} else if (xmlSchemaValueGetNext(y) != NULL)
	    return (0);
	else
	    return (1);
    }
    return (0);
}

static int
xmlSchemaResolveAttrUseReferences(xmlSchemaAttributeUsePtr ause,
				  xmlSchemaParserCtxtPtr ctxt)
{
    if ((ctxt == NULL) || (ause == NULL))
	return(-1);
    if ((ause->attrDecl == NULL) ||
	(ause->attrDecl->type != XML_SCHEMA_EXTRA_QNAMEREF))
	return(0);

    {
	xmlSchemaQNameRefPtr ref = WXS_QNAME_CAST ause->attrDecl;

	ause->attrDecl = xmlSchemaGetAttributeDecl(ctxt->schema,
	    ref->name, ref->targetNamespace);
        if (ause->attrDecl == NULL) {
	    xmlSchemaPResCompAttrErr(ctxt,
	    	XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST ause, ause->node,
		"ref", ref->name, ref->targetNamespace,
		XML_SCHEMA_TYPE_ATTRIBUTE, NULL);
            return(ctxt->err);;
        }
    }
    return(0);
}

static int
xmlSchemaCheckAttrUsePropsCorrect(xmlSchemaParserCtxtPtr ctxt,
			     xmlSchemaAttributeUsePtr use)
{
    if ((ctxt == NULL) || (use == NULL))
	return(-1);
    if ((use->defValue == NULL) || (WXS_ATTRUSE_DECL(use) == NULL) ||
	((WXS_ATTRUSE_DECL(use))->type != XML_SCHEMA_TYPE_ATTRIBUTE))
	return(0);


    if (((WXS_ATTRUSE_DECL(use))->defValue != NULL) &&
	((WXS_ATTRUSE_DECL(use))->flags & XML_SCHEMAS_ATTR_FIXED) &&
        ((use->flags & XML_SCHEMA_ATTR_USE_FIXED) == 0))
    {
	xmlSchemaPCustomErr(ctxt,
	    XML_SCHEMAP_AU_PROPS_CORRECT_2,
	    WXS_BASIC_CAST use, NULL,
	    "The attribute declaration has a 'fixed' value constraint "
	    ", thus the attribute use must also have a 'fixed' value "
	    "constraint",
	    NULL);
	return(ctxt->err);
    }
    if ((use->defVal != NULL) && (WXS_ATTRUSE_TYPEDEF(use) != NULL)) {
	int ret;
	if (xmlSchemaIsDerivedFromBuiltInType(
	    WXS_ATTRUSE_TYPEDEF(use), XML_SCHEMAS_ID))
	{
	    xmlSchemaCustomErr(ACTXT_CAST ctxt,
		XML_SCHEMAP_AU_PROPS_CORRECT,
		NULL, WXS_BASIC_CAST use,
		"Value constraints are not allowed if the type definition "
		"is or is derived from xs:ID",
		NULL, NULL);
	    return(ctxt->err);
	}

	ret = xmlSchemaVCheckCVCSimpleType(ACTXT_CAST ctxt,
	    use->node, WXS_ATTRUSE_TYPEDEF(use),
	    use->defValue, &(use->defVal),
	    1, 1, 0);
	if (ret != 0) {
	    if (ret < 0) {
		PERROR_INT2("xmlSchemaCheckAttrUsePropsCorrect",
		    "calling xmlSchemaVCheckCVCSimpleType()");
		return(-1);
	    }
	    xmlSchemaCustomErr(ACTXT_CAST ctxt,
		XML_SCHEMAP_AU_PROPS_CORRECT,
		NULL, WXS_BASIC_CAST use,
		"The value of the value constraint is not valid",
		NULL, NULL);
	    return(ctxt->err);
	}
    }
    if (((WXS_ATTRUSE_DECL(use))->defVal != NULL) &&
	(((WXS_ATTRUSE_DECL(use))->flags & XML_SCHEMA_ATTR_USE_FIXED) == 0))
    {
	if (! xmlSchemaAreValuesEqual(use->defVal,
		(WXS_ATTRUSE_DECL(use))->defVal))
	{
	    xmlSchemaPCustomErr(ctxt,
		XML_SCHEMAP_AU_PROPS_CORRECT_2,
		WXS_BASIC_CAST use, NULL,
		"The 'fixed' value constraint of the attribute use "
		"must match the attribute declaration's value "
		"constraint '%s'",
		(WXS_ATTRUSE_DECL(use))->defValue);
	}
	return(ctxt->err);
    }
    return(0);
}




static int
xmlSchemaResolveAttrTypeReferences(xmlSchemaAttributePtr item,
				   xmlSchemaParserCtxtPtr ctxt)
{
    if (item->flags & XML_SCHEMAS_ATTR_INTERNAL_RESOLVED)
	return(0);
    item->flags |= XML_SCHEMAS_ATTR_INTERNAL_RESOLVED;
    if (item->subtypes != NULL)
        return(0);
    if (item->typeName != NULL) {
        xmlSchemaTypePtr type;

	type = xmlSchemaGetType(ctxt->schema, item->typeName,
	    item->typeNs);
	if ((type == NULL) || (! WXS_IS_SIMPLE(type))) {
	    xmlSchemaPResCompAttrErr(ctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST item, item->node,
		"type", item->typeName, item->typeNs,
		XML_SCHEMA_TYPE_SIMPLE, NULL);
	    return(ctxt->err);
	} else
	    item->subtypes = type;

    } else {
	item->subtypes = xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYSIMPLETYPE);
    }
    return(0);
}

static int
xmlSchemaResolveIDCKeyReferences(xmlSchemaIDCPtr idc,
			  xmlSchemaParserCtxtPtr pctxt)
{
    if (idc->type != XML_SCHEMA_TYPE_IDC_KEYREF)
        return(0);
    if (idc->ref->name != NULL) {
	idc->ref->item = (xmlSchemaBasicItemPtr)
	    xmlSchemaGetIDC(pctxt->schema, idc->ref->name,
		idc->ref->targetNamespace);
        if (idc->ref->item == NULL) {
	    xmlSchemaPResCompAttrErr(pctxt,
		XML_SCHEMAP_SRC_RESOLVE,
		WXS_BASIC_CAST idc, idc->node,
		"refer", idc->ref->name,
		idc->ref->targetNamespace,
		XML_SCHEMA_TYPE_IDC_KEY, NULL);
            return(pctxt->err);
	} else if (idc->ref->item->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_C_PROPS_CORRECT,
		NULL, WXS_BASIC_CAST idc,
		"The keyref references a keyref",
		NULL, NULL);
	    idc->ref->item = NULL;
	    return(pctxt->err);
	} else {
	    if (idc->nbFields !=
		((xmlSchemaIDCPtr) idc->ref->item)->nbFields) {
		xmlChar *str = NULL;
		xmlSchemaIDCPtr refer;

		refer = (xmlSchemaIDCPtr) idc->ref->item;
		xmlSchemaCustomErr(ACTXT_CAST pctxt,
		    XML_SCHEMAP_C_PROPS_CORRECT,
		    NULL, WXS_BASIC_CAST idc,
		    "The cardinality of the keyref differs from the "
		    "cardinality of the referenced key/unique '%s'",
		    xmlSchemaFormatQName(&str, refer->targetNamespace,
			refer->name),
		    NULL);
		FREE_AND_NULL(str)
		return(pctxt->err);
	    }
	}
    }
    return(0);
}

static int
xmlSchemaResolveAttrUseProhibReferences(xmlSchemaAttributeUseProhibPtr prohib,
				       xmlSchemaParserCtxtPtr pctxt)
{
    if (xmlSchemaGetAttributeDecl(pctxt->schema, prohib->name,
	prohib->targetNamespace) == NULL) {

	xmlSchemaPResCompAttrErr(pctxt,
	    XML_SCHEMAP_SRC_RESOLVE,
	    NULL, prohib->node,
	    "ref", prohib->name, prohib->targetNamespace,
	    XML_SCHEMA_TYPE_ATTRIBUTE, NULL);
	return(XML_SCHEMAP_SRC_RESOLVE);
    }
    return(0);
}

#define WXS_REDEFINED_TYPE(c) \
(((xmlSchemaTypePtr) item)->flags & XML_SCHEMAS_TYPE_REDEFINED)

#define WXS_REDEFINED_MODEL_GROUP_DEF(c) \
(((xmlSchemaModelGroupDefPtr) item)->flags & XML_SCHEMA_MODEL_GROUP_DEF_REDEFINED)

#define WXS_REDEFINED_ATTR_GROUP(c) \
(((xmlSchemaAttributeGroupPtr) item)->flags & XML_SCHEMAS_ATTRGROUP_REDEFINED)

static int
xmlSchemaCheckSRCRedefineFirst(xmlSchemaParserCtxtPtr pctxt)
{
    int err = 0;
    xmlSchemaRedefPtr redef = WXS_CONSTRUCTOR(pctxt)->redefs;
    xmlSchemaBasicItemPtr prev, item;
    int wasRedefined;

    if (redef == NULL)
	return(0);

    do {
	item = redef->item;
	prev = xmlSchemaFindRedefCompInGraph(
	    redef->targetBucket, item->type,
	    redef->refName, redef->refTargetNs);
	if (prev == NULL) {
	    xmlChar *str = NULL;
	    xmlNodePtr node;

	    if (redef->reference)
		node = WXS_ITEM_NODE(redef->reference);
	    else
		node = WXS_ITEM_NODE(item);
	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_SRC_REDEFINE, node, NULL,
		"The %s '%s' to be redefined could not be found in "
		"the redefined schema",
		WXS_ITEM_TYPE_NAME(item),
		xmlSchemaFormatQName(&str, redef->refTargetNs,
		    redef->refName));
	    FREE_AND_NULL(str);
	    err = pctxt->err;
	    redef = redef->next;
	    continue;
	}
	wasRedefined = 0;
	switch (item->type) {
	    case XML_SCHEMA_TYPE_COMPLEX:
	    case XML_SCHEMA_TYPE_SIMPLE:
		if ((WXS_TYPE_CAST prev)->flags &
		    XML_SCHEMAS_TYPE_REDEFINED)
		{
		    wasRedefined = 1;
		    break;
		}
		
		(WXS_TYPE_CAST prev)->flags |= XML_SCHEMAS_TYPE_REDEFINED;
		((xmlSchemaTypePtr) item)->baseType =
		    (xmlSchemaTypePtr) prev;
		break;
	    case XML_SCHEMA_TYPE_GROUP:
		if ((WXS_MODEL_GROUPDEF_CAST prev)->flags &
		    XML_SCHEMA_MODEL_GROUP_DEF_REDEFINED)
		{
		    wasRedefined = 1;
		    break;
		}
		
		(WXS_MODEL_GROUPDEF_CAST prev)->flags |=
		    XML_SCHEMA_MODEL_GROUP_DEF_REDEFINED;
		if (redef->reference != NULL) {
		    (WXS_PTC_CAST redef->reference)->children =
			WXS_TREE_CAST prev;
		}
		redef->target = prev;
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		if ((WXS_ATTR_GROUP_CAST prev)->flags &
		    XML_SCHEMAS_ATTRGROUP_REDEFINED)
		{
		    wasRedefined = 1;
		    break;
		}
		(WXS_ATTR_GROUP_CAST prev)->flags |=
		    XML_SCHEMAS_ATTRGROUP_REDEFINED;
		if (redef->reference != NULL) {
		    (WXS_QNAME_CAST redef->reference)->item = prev;
		    redef->target = NULL;
		} else {
		    redef->target = prev;
		}
		break;
	    default:
		PERROR_INT("xmlSchemaResolveRedefReferences",
		    "Unexpected redefined component type");
		return(-1);
	}
	if (wasRedefined) {
	    xmlChar *str = NULL;
	    xmlNodePtr node;

	    if (redef->reference)
		node = WXS_ITEM_NODE(redef->reference);
	    else
		node = WXS_ITEM_NODE(redef->item);

	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		
		XML_SCHEMAP_SRC_REDEFINE,
		node, NULL,
		"The referenced %s was already redefined. Multiple "
		"redefinition of the same component is not supported",
		xmlSchemaGetComponentDesignation(&str, prev),
		NULL);
	    FREE_AND_NULL(str)
	    err = pctxt->err;
	    redef = redef->next;
	    continue;
	}
	redef = redef->next;
    } while (redef != NULL);

    return(err);
}

static int
xmlSchemaCheckSRCRedefineSecond(xmlSchemaParserCtxtPtr pctxt)
{
    int err = 0;
    xmlSchemaRedefPtr redef = WXS_CONSTRUCTOR(pctxt)->redefs;
    xmlSchemaBasicItemPtr item;

    if (redef == NULL)
	return(0);

    do {
	if (redef->target == NULL) {
	    redef = redef->next;
	    continue;
	}
	item = redef->item;

	switch (item->type) {
	    case XML_SCHEMA_TYPE_SIMPLE:
	    case XML_SCHEMA_TYPE_COMPLEX:
		(WXS_TYPE_CAST redef->target)->name = NULL;

		break;
	    case XML_SCHEMA_TYPE_GROUP:
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		err = xmlSchemaCheckDerivationOKRestriction2to4(pctxt,
		    XML_SCHEMA_ACTION_REDEFINE,
		    item, redef->target,
		    (WXS_ATTR_GROUP_CAST item)->attrUses,
		    (WXS_ATTR_GROUP_CAST redef->target)->attrUses,
		    (WXS_ATTR_GROUP_CAST item)->attributeWildcard,
		    (WXS_ATTR_GROUP_CAST redef->target)->attributeWildcard);
		if (err == -1)
		    return(-1);
		break;
	    default:
		break;
	}
	redef = redef->next;
    } while (redef != NULL);
    return(0);
}


static int
xmlSchemaAddComponents(xmlSchemaParserCtxtPtr pctxt,
		       xmlSchemaBucketPtr bucket)
{
    xmlSchemaBasicItemPtr item;
    int err;
    xmlHashTablePtr *table;
    const xmlChar *name;
    int i;

#define WXS_GET_GLOBAL_HASH(c, slot) { \
    if (WXS_IS_BUCKET_IMPMAIN((c)->type)) \
	table = &(WXS_IMPBUCKET((c))->schema->slot); \
    else \
	table = &(WXS_INCBUCKET((c))->ownerImport->schema->slot); }

    if (bucket == NULL)
	return(-1);
    if (bucket->flags & XML_SCHEMA_BUCKET_COMPS_ADDED)
	return(0);
    bucket->flags |= XML_SCHEMA_BUCKET_COMPS_ADDED;

    for (i = 0; i < bucket->globals->nbItems; i++) {
	item = bucket->globals->items[i];
	table = NULL;
	switch (item->type) {
	    case XML_SCHEMA_TYPE_COMPLEX:
	    case XML_SCHEMA_TYPE_SIMPLE:
		if (WXS_REDEFINED_TYPE(item))
		    continue;
		name = (WXS_TYPE_CAST item)->name;
		WXS_GET_GLOBAL_HASH(bucket, typeDecl)
		break;
	    case XML_SCHEMA_TYPE_ELEMENT:
		name = (WXS_ELEM_CAST item)->name;
		WXS_GET_GLOBAL_HASH(bucket, elemDecl)
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTE:
		name = (WXS_ATTR_CAST item)->name;
		WXS_GET_GLOBAL_HASH(bucket, attrDecl)
		break;
	    case XML_SCHEMA_TYPE_GROUP:
		if (WXS_REDEFINED_MODEL_GROUP_DEF(item))
		    continue;
		name = (WXS_MODEL_GROUPDEF_CAST item)->name;
		WXS_GET_GLOBAL_HASH(bucket, groupDecl)
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		if (WXS_REDEFINED_ATTR_GROUP(item))
		    continue;
		name = (WXS_ATTR_GROUP_CAST item)->name;
		WXS_GET_GLOBAL_HASH(bucket, attrgrpDecl)
		break;
	    case XML_SCHEMA_TYPE_IDC_KEY:
	    case XML_SCHEMA_TYPE_IDC_UNIQUE:
	    case XML_SCHEMA_TYPE_IDC_KEYREF:
		name = (WXS_IDC_CAST item)->name;
		WXS_GET_GLOBAL_HASH(bucket, idcDef)
		break;
	    case XML_SCHEMA_TYPE_NOTATION:
		name = ((xmlSchemaNotationPtr) item)->name;
		WXS_GET_GLOBAL_HASH(bucket, notaDecl)
		break;
	    default:
		PERROR_INT("xmlSchemaAddComponents",
		    "Unexpected global component type");
		continue;
	}
	if (*table == NULL) {
	    *table = xmlHashCreateDict(10, pctxt->dict);
	    if (*table == NULL) {
		PERROR_INT("xmlSchemaAddComponents",
		    "failed to create a component hash table");
		return(-1);
	    }
	}
	err = xmlHashAddEntry(*table, name, item);
	if (err != 0) {
	    xmlChar *str = NULL;

	    xmlSchemaCustomErr(ACTXT_CAST pctxt,
		XML_SCHEMAP_REDEFINED_TYPE,
		WXS_ITEM_NODE(item),
		WXS_BASIC_CAST item,
		"A global %s '%s' does already exist",
		WXS_ITEM_TYPE_NAME(item),
		xmlSchemaGetComponentQName(&str, item));
	    FREE_AND_NULL(str);
	}
    }
    if (bucket->relations != NULL) {
	xmlSchemaSchemaRelationPtr rel = bucket->relations;
	do {
	    if ((rel->bucket != NULL) &&
		((rel->bucket->flags & XML_SCHEMA_BUCKET_COMPS_ADDED) == 0)) {
		if (xmlSchemaAddComponents(pctxt, rel->bucket) == -1)
		    return(-1);
	    }
	    rel = rel->next;
	} while (rel != NULL);
    }
    return(0);
}

static int
xmlSchemaFixupComponents(xmlSchemaParserCtxtPtr pctxt,
			 xmlSchemaBucketPtr rootBucket)
{
    xmlSchemaConstructionCtxtPtr con = pctxt->constructor;
    xmlSchemaTreeItemPtr item, *items;
    int nbItems, i, ret = 0;
    xmlSchemaBucketPtr oldbucket = con->bucket;
    xmlSchemaElementPtr elemDecl;

#define FIXHFAILURE if (pctxt->err == XML_SCHEMAP_INTERNAL) goto exit_failure;

    if ((con->pending == NULL) ||
	(con->pending->nbItems == 0))
	return(0);

    if (con->bucket == NULL)
	con->bucket = rootBucket;

    xmlSchemaCheckSRCRedefineFirst(pctxt);

    xmlSchemaAddComponents(pctxt, rootBucket);

    pctxt->ctxtType = NULL;
    items = (xmlSchemaTreeItemPtr *) con->pending->items;
    nbItems = con->pending->nbItems;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_ELEMENT:
		xmlSchemaResolveElementReferences(
		    (xmlSchemaElementPtr) item, pctxt);
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_TYPE_COMPLEX:
	    case XML_SCHEMA_TYPE_SIMPLE:
		xmlSchemaResolveTypeReferences(
		    (xmlSchemaTypePtr) item, pctxt);
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTE:
		xmlSchemaResolveAttrTypeReferences(
		    (xmlSchemaAttributePtr) item, pctxt);
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
		xmlSchemaResolveAttrUseReferences(
		    (xmlSchemaAttributeUsePtr) item, pctxt);
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_EXTRA_QNAMEREF:
		if ((WXS_QNAME_CAST item)->itemType ==
		    XML_SCHEMA_TYPE_ATTRIBUTEGROUP)
		{
		    xmlSchemaResolveAttrGroupReferences(
			WXS_QNAME_CAST item, pctxt);
		}
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_TYPE_SEQUENCE:
	    case XML_SCHEMA_TYPE_CHOICE:
	    case XML_SCHEMA_TYPE_ALL:
		xmlSchemaResolveModelGroupParticleReferences(pctxt,
		    WXS_MODEL_GROUP_CAST item);
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_TYPE_IDC_KEY:
	    case XML_SCHEMA_TYPE_IDC_UNIQUE:
	    case XML_SCHEMA_TYPE_IDC_KEYREF:
		xmlSchemaResolveIDCKeyReferences(
		    (xmlSchemaIDCPtr) item, pctxt);
		FIXHFAILURE;
		break;
	    case XML_SCHEMA_EXTRA_ATTR_USE_PROHIB:
		xmlSchemaResolveAttrUseProhibReferences(
		    WXS_ATTR_PROHIB_CAST item, pctxt);
		FIXHFAILURE;
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;

    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_COMPLEX:
	    case XML_SCHEMA_TYPE_SIMPLE:
		xmlSchemaCheckTypeDefCircular(
		    (xmlSchemaTypePtr) item, pctxt);
		FIXHFAILURE;
		if (pctxt->nberrors != 0)
		    goto exit_error;
		break;
	    case XML_SCHEMA_TYPE_GROUP:
		xmlSchemaCheckGroupDefCircular(
		    (xmlSchemaModelGroupDefPtr) item, pctxt);
		FIXHFAILURE;
		if (pctxt->nberrors != 0)
		    goto exit_error;
		break;
	    case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		xmlSchemaCheckAttrGroupCircular(
		    (xmlSchemaAttributeGroupPtr) item, pctxt);
		FIXHFAILURE;
		if (pctxt->nberrors != 0)
		    goto exit_error;
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_SEQUENCE:
	    case XML_SCHEMA_TYPE_CHOICE:
		xmlSchemaModelGroupToModelGroupDefFixup(pctxt,
		    WXS_MODEL_GROUP_CAST item);
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
            case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
		if ((! WXS_ATTR_GROUP_EXPANDED(item)) &&
		    WXS_ATTR_GROUP_HAS_REFS(item))
		{
		    xmlSchemaAttributeGroupExpandRefs(pctxt,
			WXS_ATTR_GROUP_CAST item);
		    FIXHFAILURE;
		}
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
            case XML_SCHEMA_TYPE_SIMPLE:
		if (WXS_IS_TYPE_NOT_FIXED_1((xmlSchemaTypePtr) item)) {
		    xmlSchemaFixupSimpleTypeStageOne(pctxt,
			(xmlSchemaTypePtr) item);
		    FIXHFAILURE;
		}
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
            case XML_SCHEMA_TYPE_SIMPLE:
		if (((xmlSchemaTypePtr) item)->memberTypes != NULL) {
		    xmlSchemaCheckUnionTypeDefCircular(pctxt,
			(xmlSchemaTypePtr) item);
		    FIXHFAILURE;
		}
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;

    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
            case XML_SCHEMA_TYPE_SIMPLE:
		if (WXS_IS_TYPE_NOT_FIXED(WXS_TYPE_CAST item)) {
		    xmlSchemaFixupSimpleTypeStageTwo(pctxt, WXS_TYPE_CAST item);
		    FIXHFAILURE;
		}
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_ATTRIBUTE:
		xmlSchemaCheckAttrPropsCorrect(pctxt, WXS_ATTR_CAST item);
		FIXHFAILURE;
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_ATTRIBUTE_USE:
		if (((xmlSchemaAttributeUsePtr)item)->defValue != NULL) {
		    xmlSchemaCheckAttrUsePropsCorrect(pctxt,
			WXS_ATTR_USE_CAST item);
		    FIXHFAILURE;
		}
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;

    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	case XML_SCHEMA_TYPE_ATTRIBUTEGROUP:
	    if (( (WXS_ATTR_GROUP_CAST item)->attrUses != NULL) &&
		( (WXS_LIST_CAST (WXS_ATTR_GROUP_CAST item)->attrUses)->nbItems > 1))
	    {
		xmlSchemaCheckAGPropsCorrect(pctxt, WXS_ATTR_GROUP_CAST item);
		FIXHFAILURE;
	    }
	    break;
	default:
	    break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;

    if (WXS_CONSTRUCTOR(pctxt)->redefs != NULL)
	xmlSchemaCheckSRCRedefineSecond(pctxt);
    if (pctxt->nberrors != 0)
	goto exit_error;

    for (i = 0; i < nbItems; i++) {
	item = con->pending->items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_COMPLEX:
		if (WXS_IS_TYPE_NOT_FIXED(WXS_TYPE_CAST item)) {
		    xmlSchemaFixupComplexType(pctxt, WXS_TYPE_CAST item);
		    FIXHFAILURE;
		}
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;

    items = (xmlSchemaTreeItemPtr *) con->pending->items;
    nbItems = con->pending->nbItems;

    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_ELEMENT:
		elemDecl = (xmlSchemaElementPtr) item;

		if ((elemDecl->flags & XML_SCHEMAS_ELEM_INTERNAL_CHECKED) == 0)
		{
		    xmlSchemaCheckElementDeclComponent(
			(xmlSchemaElementPtr) elemDecl, pctxt);
		    FIXHFAILURE;
		}

#ifdef WXS_ELEM_DECL_CONS_ENABLED
		if ((WXS_ELEM_TYPEDEF(elemDecl) != NULL) &&
		    (WXS_IS_COMPLEX(WXS_ELEM_TYPEDEF(elemDecl))) &&
		    (WXS_TYPE_IS_LOCAL(WXS_ELEM_TYPEDEF(elemDecl))))
		{
		    xmlSchemaCheckElementDeclConsistent(pctxt,
			WXS_BASIC_CAST elemDecl,
			WXS_TYPE_PARTICLE(WXS_ELEM_TYPEDEF(elemDecl)),
			NULL, NULL, 0);
		}
#endif
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;


    for (i = 0; i < nbItems; i++) {
	item = items[i];
	switch (item->type) {
	    case XML_SCHEMA_TYPE_COMPLEX:
		xmlSchemaBuildContentModel((xmlSchemaTypePtr) item, pctxt);
		
		break;
	    default:
		break;
	}
    }
    if (pctxt->nberrors != 0)
	goto exit_error;
    goto exit;

exit_error:
    ret = pctxt->err;
    goto exit;

exit_failure:
    ret = -1;

exit:
    con->bucket = oldbucket;
    con->pending->nbItems = 0;
    if (con->substGroups != NULL) {
	xmlHashFree(con->substGroups,
	    (xmlHashDeallocator) xmlSchemaSubstGroupFree);
	con->substGroups = NULL;
    }
    if (con->redefs != NULL) {
	xmlSchemaRedefListFree(con->redefs);
	con->redefs = NULL;
    }
    return(ret);
}
xmlSchemaPtr
xmlSchemaParse(xmlSchemaParserCtxtPtr ctxt)
{
    xmlSchemaPtr mainSchema = NULL;
    xmlSchemaBucketPtr bucket = NULL;
    int res;


    xmlSchemaInitTypes();

    if (ctxt == NULL)
        return (NULL);

    
    ctxt->nberrors = 0;
    ctxt->err = 0;
    ctxt->counter = 0;

    
    mainSchema = xmlSchemaNewSchema(ctxt);
    if (mainSchema == NULL)
	goto exit_failure;
    if (ctxt->constructor == NULL) {
	ctxt->constructor = xmlSchemaConstructionCtxtCreate(ctxt->dict);
	if (ctxt->constructor == NULL)
	    return(NULL);
	
	ctxt->ownsConstructor = 1;
    }
    ctxt->constructor->mainSchema = mainSchema;
    res = xmlSchemaAddSchemaDoc(ctxt, XML_SCHEMA_SCHEMA_MAIN,
	ctxt->URL, ctxt->doc, ctxt->buffer, ctxt->size, NULL,
	NULL, NULL, &bucket);
    if (res == -1)
	goto exit_failure;
    if (res != 0)
	goto exit;

    if (bucket == NULL) {
	
	if (ctxt->URL)
	    xmlSchemaCustomErr(ACTXT_CAST ctxt, XML_SCHEMAP_FAILED_LOAD,
		NULL, NULL,
		"Failed to locate the main schema resource at '%s'",
		ctxt->URL, NULL);
	else
	    xmlSchemaCustomErr(ACTXT_CAST ctxt, XML_SCHEMAP_FAILED_LOAD,
		NULL, NULL,
		"Failed to locate the main schema resource",
		    NULL, NULL);
	goto exit;
    }
    
    if (xmlSchemaParseNewDocWithContext(ctxt, mainSchema, bucket) == -1)
	goto exit_failure;
    if (ctxt->nberrors != 0)
	goto exit;

    mainSchema->doc = bucket->doc;
    mainSchema->preserve = ctxt->preserve;

    ctxt->schema = mainSchema;

    if (xmlSchemaFixupComponents(ctxt, WXS_CONSTRUCTOR(ctxt)->mainBucket) == -1)
	goto exit_failure;

exit:
    if (ctxt->nberrors != 0) {
	if (mainSchema) {
	    xmlSchemaFree(mainSchema);
	    mainSchema = NULL;
	}
	if (ctxt->constructor) {
	    xmlSchemaConstructionCtxtFree(ctxt->constructor);
	    ctxt->constructor = NULL;
	    ctxt->ownsConstructor = 0;
	}
    }
    ctxt->schema = NULL;
    return(mainSchema);
exit_failure:
    if (mainSchema) {
        xmlSchemaFree(mainSchema);
	mainSchema = NULL;
    }
    if (ctxt->constructor) {
	xmlSchemaConstructionCtxtFree(ctxt->constructor);
	ctxt->constructor = NULL;
	ctxt->ownsConstructor = 0;
    }
    PERROR_INT2("xmlSchemaParse",
	"An internal error occured");
    ctxt->schema = NULL;
    return(NULL);
}

void
xmlSchemaSetParserErrors(xmlSchemaParserCtxtPtr ctxt,
                         xmlSchemaValidityErrorFunc err,
                         xmlSchemaValidityWarningFunc warn, void *ctx)
{
    if (ctxt == NULL)
        return;
    ctxt->error = err;
    ctxt->warning = warn;
    ctxt->errCtxt = ctx;
    if (ctxt->vctxt != NULL)
	xmlSchemaSetValidErrors(ctxt->vctxt, err, warn, ctx);
}

void
xmlSchemaSetParserStructuredErrors(xmlSchemaParserCtxtPtr ctxt,
				   xmlStructuredErrorFunc serror,
				   void *ctx)
{
    if (ctxt == NULL)
	return;
    ctxt->serror = serror;
    ctxt->errCtxt = ctx;
    if (ctxt->vctxt != NULL)
	xmlSchemaSetValidStructuredErrors(ctxt->vctxt, serror, ctx);
}

int
xmlSchemaGetParserErrors(xmlSchemaParserCtxtPtr ctxt,
			 xmlSchemaValidityErrorFunc * err,
			 xmlSchemaValidityWarningFunc * warn, void **ctx)
{
	if (ctxt == NULL)
		return(-1);
	if (err != NULL)
		*err = ctxt->error;
	if (warn != NULL)
		*warn = ctxt->warning;
	if (ctx != NULL)
		*ctx = ctxt->errCtxt;
	return(0);
}

static const xmlChar *
xmlSchemaFacetTypeToString(xmlSchemaTypeType type)
{
    switch (type) {
        case XML_SCHEMA_FACET_PATTERN:
            return (BAD_CAST "pattern");
        case XML_SCHEMA_FACET_MAXEXCLUSIVE:
            return (BAD_CAST "maxExclusive");
        case XML_SCHEMA_FACET_MAXINCLUSIVE:
            return (BAD_CAST "maxInclusive");
        case XML_SCHEMA_FACET_MINEXCLUSIVE:
            return (BAD_CAST "minExclusive");
        case XML_SCHEMA_FACET_MININCLUSIVE:
            return (BAD_CAST "minInclusive");
        case XML_SCHEMA_FACET_WHITESPACE:
            return (BAD_CAST "whiteSpace");
        case XML_SCHEMA_FACET_ENUMERATION:
            return (BAD_CAST "enumeration");
        case XML_SCHEMA_FACET_LENGTH:
            return (BAD_CAST "length");
        case XML_SCHEMA_FACET_MAXLENGTH:
            return (BAD_CAST "maxLength");
        case XML_SCHEMA_FACET_MINLENGTH:
            return (BAD_CAST "minLength");
        case XML_SCHEMA_FACET_TOTALDIGITS:
            return (BAD_CAST "totalDigits");
        case XML_SCHEMA_FACET_FRACTIONDIGITS:
            return (BAD_CAST "fractionDigits");
        default:
            break;
    }
    return (BAD_CAST "Internal Error");
}

static xmlSchemaWhitespaceValueType
xmlSchemaGetWhiteSpaceFacetValue(xmlSchemaTypePtr type)
{
    if (type->type == XML_SCHEMA_TYPE_BASIC) {
	if ((type->builtInType == XML_SCHEMAS_STRING) ||
	    (type->builtInType == XML_SCHEMAS_ANYSIMPLETYPE))
	    return(XML_SCHEMA_WHITESPACE_PRESERVE);
	else if (type->builtInType == XML_SCHEMAS_NORMSTRING)
	    return(XML_SCHEMA_WHITESPACE_REPLACE);
	else {
	    return(XML_SCHEMA_WHITESPACE_COLLAPSE);
	}
    } else if (WXS_IS_LIST(type)) {
	return (XML_SCHEMA_WHITESPACE_COLLAPSE);
    } else if (WXS_IS_UNION(type)) {
	return (XML_SCHEMA_WHITESPACE_UNKNOWN);
    } else if (WXS_IS_ATOMIC(type)) {
	if (type->flags & XML_SCHEMAS_TYPE_WHITESPACE_PRESERVE)
	    return (XML_SCHEMA_WHITESPACE_PRESERVE);
	else if (type->flags & XML_SCHEMAS_TYPE_WHITESPACE_REPLACE)
	    return (XML_SCHEMA_WHITESPACE_REPLACE);
	else
	    return (XML_SCHEMA_WHITESPACE_COLLAPSE);
    }
    return (-1);
}




static int
xmlSchemaAssembleByLocation(xmlSchemaValidCtxtPtr vctxt,
			    xmlSchemaPtr schema,
			    xmlNodePtr node,
			    const xmlChar *nsName,
			    const xmlChar *location)
{
    int ret = 0;
    xmlSchemaParserCtxtPtr pctxt;
    xmlSchemaBucketPtr bucket = NULL;

    if ((vctxt == NULL) || (schema == NULL))
	return (-1);

    if (vctxt->pctxt == NULL) {
	VERROR_INT("xmlSchemaAssembleByLocation",
	    "no parser context available");
	return(-1);
    }
    pctxt = vctxt->pctxt;
    if (pctxt->constructor == NULL) {
	PERROR_INT("xmlSchemaAssembleByLocation",
	    "no constructor");
	return(-1);
    }
    location = xmlSchemaBuildAbsoluteURI(pctxt->dict,
	location, node);
    ret = xmlSchemaAddSchemaDoc(pctxt, XML_SCHEMA_SCHEMA_IMPORT,
	location, NULL, NULL, 0, node, NULL, nsName,
	&bucket);
    if (ret != 0)
	return(ret);
    if (bucket == NULL) {
	xmlSchemaCustomWarning(ACTXT_CAST vctxt, XML_SCHEMAV_MISC,
	    node, NULL,
	    "The document at location '%s' could not be acquired",
	    location, NULL, NULL);
	return(ret);
    }
    if ((bucket != NULL) &&
	(WXS_CONSTRUCTOR(pctxt)->bucket == NULL))
	WXS_CONSTRUCTOR(pctxt)->bucket = bucket;
    if ((bucket == NULL) || (! CAN_PARSE_SCHEMA(bucket)))
	return(0);
    pctxt->nberrors = 0;
    pctxt->err = 0;
    pctxt->doc = bucket->doc;

    ret = xmlSchemaParseNewDocWithContext(pctxt, schema, bucket);
    if (ret == -1) {
	pctxt->doc = NULL;
	goto exit_failure;
    }
    
    if ((ret == 0) && (pctxt->nberrors != 0))
	ret = pctxt->err;
    if (pctxt->nberrors == 0) {
	xmlSchemaFixupComponents(pctxt, bucket);
	ret = pctxt->err;
	if ((ret != 0) && (vctxt->err == 0))
	    vctxt->err = ret;
	vctxt->nberrors += pctxt->nberrors;
    } else {
	
	vctxt->nberrors += pctxt->nberrors;
    }
    pctxt->doc = NULL;
    return(ret);
exit_failure:
    pctxt->doc = NULL;
    return (-1);
}

static xmlSchemaAttrInfoPtr
xmlSchemaGetMetaAttrInfo(xmlSchemaValidCtxtPtr vctxt,
			 int metaType)
{
    if (vctxt->nbAttrInfos == 0)
	return (NULL);
    {
	int i;
	xmlSchemaAttrInfoPtr iattr;

	for (i = 0; i < vctxt->nbAttrInfos; i++) {
	    iattr = vctxt->attrInfos[i];
	    if (iattr->metaType == metaType)
		return (iattr);
	}

    }
    return (NULL);
}

static int
xmlSchemaAssembleByXSI(xmlSchemaValidCtxtPtr vctxt)
{
    const xmlChar *cur, *end;
    const xmlChar *nsname = NULL, *location;
    int count = 0;
    int ret = 0;
    xmlSchemaAttrInfoPtr iattr;

    iattr = xmlSchemaGetMetaAttrInfo(vctxt,
	XML_SCHEMA_ATTR_INFO_META_XSI_SCHEMA_LOC);
    if (iattr == NULL)
	iattr = xmlSchemaGetMetaAttrInfo(vctxt,
	XML_SCHEMA_ATTR_INFO_META_XSI_NO_NS_SCHEMA_LOC);
    if (iattr == NULL)
	return (0);
    cur = iattr->value;
    do {
	if (iattr->metaType == XML_SCHEMA_ATTR_INFO_META_XSI_SCHEMA_LOC) {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    count++; 
	    nsname = xmlDictLookup(vctxt->schema->dict, cur, end - cur);
	    cur = end;
	}
	while (IS_BLANK_CH(*cur))
	    cur++;
	end = cur;
	while ((*end != 0) && (!(IS_BLANK_CH(*end))))
	    end++;
	if (end == cur) {
	    if (iattr->metaType ==
		XML_SCHEMA_ATTR_INFO_META_XSI_SCHEMA_LOC)
	    {
		xmlSchemaCustomWarning(ACTXT_CAST vctxt, XML_SCHEMAV_MISC,
		    iattr->node, NULL,
		    "The value must consist of tuples: the target namespace "
		    "name and the document's URI", NULL, NULL, NULL);
	    }
	    break;
	}
	count++; 
	location = xmlDictLookup(vctxt->schema->dict, cur, end - cur);
	cur = end;
	ret = xmlSchemaAssembleByLocation(vctxt, vctxt->schema,
	    iattr->node, nsname, location);
	if (ret == -1) {
	    VERROR_INT("xmlSchemaAssembleByXSI",
		"assembling schemata");
	    return (-1);
	}
    } while (*cur != 0);
    return (ret);
}

static const xmlChar *
xmlSchemaLookupNamespace(xmlSchemaValidCtxtPtr vctxt,
			 const xmlChar *prefix)
{
    if (vctxt->sax != NULL) {
	int i, j;
	xmlSchemaNodeInfoPtr inode;

	for (i = vctxt->depth; i >= 0; i--) {
	    if (vctxt->elemInfos[i]->nbNsBindings != 0) {
		inode = vctxt->elemInfos[i];
		for (j = 0; j < inode->nbNsBindings * 2; j += 2) {
		    if (((prefix == NULL) &&
			    (inode->nsBindings[j] == NULL)) ||
			((prefix != NULL) && xmlStrEqual(prefix,
			    inode->nsBindings[j]))) {

			return (inode->nsBindings[j+1]);
		    }
		}
	    }
	}
	return (NULL);
#ifdef LIBXML_READER_ENABLED
    } else if (vctxt->reader != NULL) {
	xmlChar *nsName;

	nsName = xmlTextReaderLookupNamespace(vctxt->reader, prefix);
	if (nsName != NULL) {
	    const xmlChar *ret;

	    ret = xmlDictLookup(vctxt->dict, nsName, -1);
	    xmlFree(nsName);
	    return (ret);
	} else
	    return (NULL);
#endif
    } else {
	xmlNsPtr ns;

	if ((vctxt->inode->node == NULL) ||
	    (vctxt->inode->node->doc == NULL)) {
	    VERROR_INT("xmlSchemaLookupNamespace",
		"no node or node's doc avaliable");
	    return (NULL);
	}
	ns = xmlSearchNs(vctxt->inode->node->doc,
	    vctxt->inode->node, prefix);
	if (ns != NULL)
	    return (ns->href);
	return (NULL);
    }
}

static int
xmlSchemaValidateNotation(xmlSchemaValidCtxtPtr vctxt,
			  xmlSchemaPtr schema,
			  xmlNodePtr node,
			  const xmlChar *value,
			  xmlSchemaValPtr *val,
			  int valNeeded)
{
    int ret;

    if (vctxt && (vctxt->schema == NULL)) {
	VERROR_INT("xmlSchemaValidateNotation",
	    "a schema is needed on the validation context");
	return (-1);
    }
    ret = xmlValidateQName(value, 1);
    if (ret != 0)
	return (ret);
    {
	xmlChar *localName = NULL;
	xmlChar *prefix = NULL;

	localName = xmlSplitQName2(value, &prefix);
	if (prefix != NULL) {
	    const xmlChar *nsName = NULL;

	    if (vctxt != NULL)
		nsName = xmlSchemaLookupNamespace(vctxt, BAD_CAST prefix);
	    else if (node != NULL) {
		xmlNsPtr ns = xmlSearchNs(node->doc, node, prefix);
		if (ns != NULL)
		    nsName = ns->href;
	    } else {
		xmlFree(prefix);
		xmlFree(localName);
		return (1);
	    }
	    if (nsName == NULL) {
		xmlFree(prefix);
		xmlFree(localName);
		return (1);
	    }
	    if (xmlSchemaGetNotation(schema, localName, nsName) != NULL) {
		if ((valNeeded) && (val != NULL)) {
		    (*val) = xmlSchemaNewNOTATIONValue(xmlStrdup(localName),
						       xmlStrdup(nsName));
		    if (*val == NULL)
			ret = -1;
		}
	    } else
		ret = 1;
	    xmlFree(prefix);
	    xmlFree(localName);
	} else {
	    if (xmlSchemaGetNotation(schema, value, NULL) != NULL) {
		if (valNeeded && (val != NULL)) {
		    (*val) = xmlSchemaNewNOTATIONValue(
			BAD_CAST xmlStrdup(value), NULL);
		    if (*val == NULL)
			ret = -1;
		}
	    } else
		return (1);
	}
    }
    return (ret);
}

static int
xmlSchemaVAddNodeQName(xmlSchemaValidCtxtPtr vctxt,
		       const xmlChar* lname,
		       const xmlChar* nsname)
{
    int i;

    lname = xmlDictLookup(vctxt->dict, lname, -1);
    if (lname == NULL)
	return(-1);
    if (nsname != NULL) {
	nsname = xmlDictLookup(vctxt->dict, nsname, -1);
	if (nsname == NULL)
	    return(-1);
    }
    for (i = 0; i < vctxt->nodeQNames->nbItems; i += 2) {
	if ((vctxt->nodeQNames->items [i] == lname) &&
	    (vctxt->nodeQNames->items[i +1] == nsname))
	    
	    return(i);
    }
    
    i = vctxt->nodeQNames->nbItems;
    xmlSchemaItemListAdd(vctxt->nodeQNames, (void *) lname);
    xmlSchemaItemListAdd(vctxt->nodeQNames, (void *) nsname);
    return(i);
}


static void
xmlSchemaAugmentIDC(xmlSchemaIDCPtr idcDef,
		    xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaIDCAugPtr aidc;

    aidc = (xmlSchemaIDCAugPtr) xmlMalloc(sizeof(xmlSchemaIDCAug));
    if (aidc == NULL) {
	xmlSchemaVErrMemory(vctxt,
	    "xmlSchemaAugmentIDC: allocating an augmented IDC definition",
	    NULL);
	return;
    }
    aidc->keyrefDepth = -1;
    aidc->def = idcDef;
    aidc->next = NULL;
    if (vctxt->aidcs == NULL)
	vctxt->aidcs = aidc;
    else {
	aidc->next = vctxt->aidcs;
	vctxt->aidcs = aidc;
    }
    if ((vctxt->hasKeyrefs == 0) &&
	(idcDef->type == XML_SCHEMA_TYPE_IDC_KEYREF))
	vctxt->hasKeyrefs = 1;
}

static void
xmlSchemaAugmentImportedIDC(xmlSchemaImportPtr imported, xmlSchemaValidCtxtPtr vctxt) {
    if (imported->schema->idcDef != NULL) {
	    xmlHashScan(imported->schema->idcDef ,
	    (xmlHashScanner) xmlSchemaAugmentIDC, vctxt);
    }
}

static xmlSchemaPSVIIDCBindingPtr
xmlSchemaIDCNewBinding(xmlSchemaIDCPtr idcDef)
{
    xmlSchemaPSVIIDCBindingPtr ret;

    ret = (xmlSchemaPSVIIDCBindingPtr) xmlMalloc(
	    sizeof(xmlSchemaPSVIIDCBinding));
    if (ret == NULL) {
	xmlSchemaVErrMemory(NULL,
	    "allocating a PSVI IDC binding item", NULL);
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaPSVIIDCBinding));
    ret->definition = idcDef;
    return (ret);
}

static int
xmlSchemaIDCStoreNodeTableItem(xmlSchemaValidCtxtPtr vctxt,
			       xmlSchemaPSVIIDCNodePtr item)
{
    if (vctxt->idcNodes == NULL) {
	vctxt->idcNodes = (xmlSchemaPSVIIDCNodePtr *)
	    xmlMalloc(20 * sizeof(xmlSchemaPSVIIDCNodePtr));
	if (vctxt->idcNodes == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"allocating the IDC node table item list", NULL);
	    return (-1);
	}
	vctxt->sizeIdcNodes = 20;
    } else if (vctxt->sizeIdcNodes <= vctxt->nbIdcNodes) {
	vctxt->sizeIdcNodes *= 2;
	vctxt->idcNodes = (xmlSchemaPSVIIDCNodePtr *)
	    xmlRealloc(vctxt->idcNodes, vctxt->sizeIdcNodes *
	    sizeof(xmlSchemaPSVIIDCNodePtr));
	if (vctxt->idcNodes == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"re-allocating the IDC node table item list", NULL);
	    return (-1);
	}
    }
    vctxt->idcNodes[vctxt->nbIdcNodes++] = item;

    return (0);
}

static int
xmlSchemaIDCStoreKey(xmlSchemaValidCtxtPtr vctxt,
		     xmlSchemaPSVIIDCKeyPtr key)
{
    if (vctxt->idcKeys == NULL) {
	vctxt->idcKeys = (xmlSchemaPSVIIDCKeyPtr *)
	    xmlMalloc(40 * sizeof(xmlSchemaPSVIIDCKeyPtr));
	if (vctxt->idcKeys == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"allocating the IDC key storage list", NULL);
	    return (-1);
	}
	vctxt->sizeIdcKeys = 40;
    } else if (vctxt->sizeIdcKeys <= vctxt->nbIdcKeys) {
	vctxt->sizeIdcKeys *= 2;
	vctxt->idcKeys = (xmlSchemaPSVIIDCKeyPtr *)
	    xmlRealloc(vctxt->idcKeys, vctxt->sizeIdcKeys *
	    sizeof(xmlSchemaPSVIIDCKeyPtr));
	if (vctxt->idcKeys == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"re-allocating the IDC key storage list", NULL);
	    return (-1);
	}
    }
    vctxt->idcKeys[vctxt->nbIdcKeys++] = key;

    return (0);
}

static int
xmlSchemaIDCAppendNodeTableItem(xmlSchemaPSVIIDCBindingPtr bind,
				xmlSchemaPSVIIDCNodePtr ntItem)
{
    if (bind->nodeTable == NULL) {
	bind->sizeNodes = 10;
	bind->nodeTable = (xmlSchemaPSVIIDCNodePtr *)
	    xmlMalloc(10 * sizeof(xmlSchemaPSVIIDCNodePtr));
	if (bind->nodeTable == NULL) {
	    xmlSchemaVErrMemory(NULL,
		"allocating an array of IDC node-table items", NULL);
	    return(-1);
	}
    } else if (bind->sizeNodes <= bind->nbNodes) {
	bind->sizeNodes *= 2;
	bind->nodeTable = (xmlSchemaPSVIIDCNodePtr *)
	    xmlRealloc(bind->nodeTable, bind->sizeNodes *
		sizeof(xmlSchemaPSVIIDCNodePtr));
	if (bind->nodeTable == NULL) {
	    xmlSchemaVErrMemory(NULL,
		"re-allocating an array of IDC node-table items", NULL);
	    return(-1);
	}
    }
    bind->nodeTable[bind->nbNodes++] = ntItem;
    return(0);
}

static xmlSchemaPSVIIDCBindingPtr
xmlSchemaIDCAcquireBinding(xmlSchemaValidCtxtPtr vctxt,
			  xmlSchemaIDCMatcherPtr matcher)
{
    xmlSchemaNodeInfoPtr ielem;

    ielem = vctxt->elemInfos[matcher->depth];

    if (ielem->idcTable == NULL) {
	ielem->idcTable = xmlSchemaIDCNewBinding(matcher->aidc->def);
	if (ielem->idcTable == NULL)
	    return (NULL);
	return(ielem->idcTable);
    } else {
	xmlSchemaPSVIIDCBindingPtr bind = NULL;

	bind = ielem->idcTable;
	do {
	    if (bind->definition == matcher->aidc->def)
		return(bind);
	    if (bind->next == NULL) {
		bind->next = xmlSchemaIDCNewBinding(matcher->aidc->def);
		if (bind->next == NULL)
		    return (NULL);
		return(bind->next);
	    }
	    bind = bind->next;
	} while (bind != NULL);
    }
    return (NULL);
}

static xmlSchemaItemListPtr
xmlSchemaIDCAcquireTargetList(xmlSchemaValidCtxtPtr vctxt ATTRIBUTE_UNUSED,
			     xmlSchemaIDCMatcherPtr matcher)
{
    if (matcher->targets == NULL)
	matcher->targets = xmlSchemaItemListCreate();
    return(matcher->targets);
}

static void
xmlSchemaIDCFreeKey(xmlSchemaPSVIIDCKeyPtr key)
{
    if (key->val != NULL)
	xmlSchemaFreeValue(key->val);
    xmlFree(key);
}

static void
xmlSchemaIDCFreeBinding(xmlSchemaPSVIIDCBindingPtr bind)
{
    if (bind->nodeTable != NULL)
	xmlFree(bind->nodeTable);
    if (bind->dupls != NULL)
	xmlSchemaItemListFree(bind->dupls);
    xmlFree(bind);
}

static void
xmlSchemaIDCFreeIDCTable(xmlSchemaPSVIIDCBindingPtr bind)
{
    xmlSchemaPSVIIDCBindingPtr prev;

    while (bind != NULL) {
	prev = bind;
	bind = bind->next;
	xmlSchemaIDCFreeBinding(prev);
    }
}

static void
xmlSchemaIDCFreeMatcherList(xmlSchemaIDCMatcherPtr matcher)
{
    xmlSchemaIDCMatcherPtr next;

    while (matcher != NULL) {
	next = matcher->next;
	if (matcher->keySeqs != NULL) {
	    int i;
	    for (i = 0; i < matcher->sizeKeySeqs; i++)
		if (matcher->keySeqs[i] != NULL)
		    xmlFree(matcher->keySeqs[i]);
	    xmlFree(matcher->keySeqs);
	}
	if (matcher->targets != NULL) {
	    if (matcher->idcType == XML_SCHEMA_TYPE_IDC_KEYREF) {
		int i;
		xmlSchemaPSVIIDCNodePtr idcNode;
		for (i = 0; i < matcher->targets->nbItems; i++) {
		    idcNode =
			(xmlSchemaPSVIIDCNodePtr) matcher->targets->items[i];
		    xmlFree(idcNode->keys);
		    xmlFree(idcNode);
		}
	    }
	    xmlSchemaItemListFree(matcher->targets);
	}
	xmlFree(matcher);
	matcher = next;
    }
}

static void
xmlSchemaIDCReleaseMatcherList(xmlSchemaValidCtxtPtr vctxt,
			       xmlSchemaIDCMatcherPtr matcher)
{
    xmlSchemaIDCMatcherPtr next;

    while (matcher != NULL) {
	next = matcher->next;
	if (matcher->keySeqs != NULL) {
	    int i;
	    for (i = 0; i < matcher->sizeKeySeqs; i++)
		if (matcher->keySeqs[i] != NULL) {
		    xmlFree(matcher->keySeqs[i]);
		    matcher->keySeqs[i] = NULL;
		}
	}
	if (matcher->targets) {
	    if (matcher->idcType == XML_SCHEMA_TYPE_IDC_KEYREF) {
		int i;
		xmlSchemaPSVIIDCNodePtr idcNode;
		for (i = 0; i < matcher->targets->nbItems; i++) {
		    idcNode =
			(xmlSchemaPSVIIDCNodePtr) matcher->targets->items[i];
		    xmlFree(idcNode->keys);
		    xmlFree(idcNode);
		}
	    }
	    xmlSchemaItemListFree(matcher->targets);
	    matcher->targets = NULL;
	}
	matcher->next = NULL;
	if (vctxt->idcMatcherCache != NULL)
	    matcher->nextCached = vctxt->idcMatcherCache;
	vctxt->idcMatcherCache = matcher;

	matcher = next;
    }
}

static int
xmlSchemaIDCAddStateObject(xmlSchemaValidCtxtPtr vctxt,
			xmlSchemaIDCMatcherPtr matcher,
			xmlSchemaIDCSelectPtr sel,
			int type)
{
    xmlSchemaIDCStateObjPtr sto;

    if (vctxt->xpathStatePool != NULL) {
	sto = vctxt->xpathStatePool;
	vctxt->xpathStatePool = sto->next;
	sto->next = NULL;
    } else {
	sto = (xmlSchemaIDCStateObjPtr) xmlMalloc(sizeof(xmlSchemaIDCStateObj));
	if (sto == NULL) {
	    xmlSchemaVErrMemory(NULL,
		"allocating an IDC state object", NULL);
	    return (-1);
	}
	memset(sto, 0, sizeof(xmlSchemaIDCStateObj));
    }
    if (vctxt->xpathStates != NULL)
	sto->next = vctxt->xpathStates;
    vctxt->xpathStates = sto;

    if (sto->xpathCtxt != NULL)
	xmlFreeStreamCtxt((xmlStreamCtxtPtr) sto->xpathCtxt);

    sto->xpathCtxt = (void *) xmlPatternGetStreamCtxt(
	(xmlPatternPtr) sel->xpathComp);
    if (sto->xpathCtxt == NULL) {
	VERROR_INT("xmlSchemaIDCAddStateObject",
	    "failed to create an XPath validation context");
	return (-1);
    }
    sto->type = type;
    sto->depth = vctxt->depth;
    sto->matcher = matcher;
    sto->sel = sel;
    sto->nbHistory = 0;

#ifdef DEBUG_IDC
    xmlGenericError(xmlGenericErrorContext, "IDC:   STO push '%s'\n",
	sto->sel->xpath);
#endif
    return (0);
}

static int
xmlSchemaXPathEvaluate(xmlSchemaValidCtxtPtr vctxt,
		       xmlElementType nodeType)
{
    xmlSchemaIDCStateObjPtr sto, head = NULL, first;
    int res, resolved = 0, depth = vctxt->depth;

    if (vctxt->xpathStates == NULL)
	return (0);

    if (nodeType == XML_ATTRIBUTE_NODE)
	depth++;
#ifdef DEBUG_IDC
    {
	xmlChar *str = NULL;
	xmlGenericError(xmlGenericErrorContext,
	    "IDC: EVAL on %s, depth %d, type %d\n",
	    xmlSchemaFormatQName(&str, vctxt->inode->nsName,
		vctxt->inode->localName), depth, nodeType);
	FREE_AND_NULL(str)
    }
#endif
    first = vctxt->xpathStates;
    sto = first;
    while (sto != head) {
#ifdef DEBUG_IDC
	if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_SELECTOR)
	    xmlGenericError(xmlGenericErrorContext, "IDC:   ['%s'] selector '%s'\n",
		sto->matcher->aidc->def->name, sto->sel->xpath);
	else
	    xmlGenericError(xmlGenericErrorContext, "IDC:   ['%s'] field '%s'\n",
		sto->matcher->aidc->def->name, sto->sel->xpath);
#endif
	if (nodeType == XML_ELEMENT_NODE)
	    res = xmlStreamPush((xmlStreamCtxtPtr) sto->xpathCtxt,
		vctxt->inode->localName, vctxt->inode->nsName);
	else
	    res = xmlStreamPushAttr((xmlStreamCtxtPtr) sto->xpathCtxt,
		vctxt->inode->localName, vctxt->inode->nsName);

	if (res == -1) {
	    VERROR_INT("xmlSchemaXPathEvaluate",
		"calling xmlStreamPush()");
	    return (-1);
	}
	if (res == 0)
	    goto next_sto;
#ifdef DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:     "
	    "MATCH\n");
#endif
	if (sto->history == NULL) {
	    sto->history = (int *) xmlMalloc(5 * sizeof(int));
	    if (sto->history == NULL) {
		xmlSchemaVErrMemory(NULL,
		    "allocating the state object history", NULL);
		return(-1);
	    }
	    sto->sizeHistory = 5;
	} else if (sto->sizeHistory <= sto->nbHistory) {
	    sto->sizeHistory *= 2;
	    sto->history = (int *) xmlRealloc(sto->history,
		sto->sizeHistory * sizeof(int));
	    if (sto->history == NULL) {
		xmlSchemaVErrMemory(NULL,
		    "re-allocating the state object history", NULL);
		return(-1);
	    }
	}
	sto->history[sto->nbHistory++] = depth;

#ifdef DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:       push match '%d'\n",
	    vctxt->depth);
#endif

	if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_SELECTOR) {
	    xmlSchemaIDCSelectPtr sel;
#ifdef DEBUG_IDC
	    xmlGenericError(xmlGenericErrorContext, "IDC:     "
		"activating field states\n");
#endif
	    sel = sto->matcher->aidc->def->fields;
	    while (sel != NULL) {
		if (xmlSchemaIDCAddStateObject(vctxt, sto->matcher,
		    sel, XPATH_STATE_OBJ_TYPE_IDC_FIELD) == -1)
		    return (-1);
		sel = sel->next;
	    }
	} else if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_FIELD) {
#ifdef DEBUG_IDC
	    xmlGenericError(xmlGenericErrorContext,
		"IDC:     key found\n");
#endif
	    if (resolved == 0) {
		if ((vctxt->inode->flags &
		    XML_SCHEMA_NODE_INFO_VALUE_NEEDED) == 0)
		vctxt->inode->flags |= XML_SCHEMA_NODE_INFO_VALUE_NEEDED;
	    }
	    resolved++;
	}
next_sto:
	if (sto->next == NULL) {
	    head = first;
	    sto = vctxt->xpathStates;
	} else
	    sto = sto->next;
    }
    return (resolved);
}

static const xmlChar *
xmlSchemaFormatIDCKeySequence(xmlSchemaValidCtxtPtr vctxt,
			      xmlChar **buf,
			      xmlSchemaPSVIIDCKeyPtr *seq,
			      int count)
{
    int i, res;
    xmlChar *value = NULL;

    *buf = xmlStrdup(BAD_CAST "[");
    for (i = 0; i < count; i++) {
	*buf = xmlStrcat(*buf, BAD_CAST "'");
	res = xmlSchemaGetCanonValueWhtspExt(seq[i]->val,
	    xmlSchemaGetWhiteSpaceFacetValue(seq[i]->type),
	    &value);
	if (res == 0)
	    *buf = xmlStrcat(*buf, BAD_CAST value);
	else {
	    VERROR_INT("xmlSchemaFormatIDCKeySequence",
		"failed to compute a canonical value");
	    *buf = xmlStrcat(*buf, BAD_CAST "???");
	}
	if (i < count -1)
	    *buf = xmlStrcat(*buf, BAD_CAST "', ");
	else
	    *buf = xmlStrcat(*buf, BAD_CAST "'");
	if (value != NULL) {
	    xmlFree(value);
	    value = NULL;
	}
    }
    *buf = xmlStrcat(*buf, BAD_CAST "]");

    return (BAD_CAST *buf);
}

static int
xmlSchemaXPathPop(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaIDCStateObjPtr sto;
    int res;

    if (vctxt->xpathStates == NULL)
	return(0);
    sto = vctxt->xpathStates;
    do {
	res = xmlStreamPop((xmlStreamCtxtPtr) sto->xpathCtxt);
	if (res == -1)
	    return (-1);
	sto = sto->next;
    } while (sto != NULL);
    return(0);
}

static int
xmlSchemaXPathProcessHistory(xmlSchemaValidCtxtPtr vctxt,
			     int depth)
{
    xmlSchemaIDCStateObjPtr sto, nextsto;
    int res, matchDepth;
    xmlSchemaPSVIIDCKeyPtr key = NULL;
    xmlSchemaTypePtr type = vctxt->inode->typeDef, simpleType = NULL;

    if (vctxt->xpathStates == NULL)
	return (0);
    sto = vctxt->xpathStates;

#ifdef DEBUG_IDC
    {
	xmlChar *str = NULL;
	xmlGenericError(xmlGenericErrorContext,
	    "IDC: BACK on %s, depth %d\n",
	    xmlSchemaFormatQName(&str, vctxt->inode->nsName,
		vctxt->inode->localName), vctxt->depth);
	FREE_AND_NULL(str)
    }
#endif
    while (sto != NULL) {
	res = xmlStreamPop((xmlStreamCtxtPtr) sto->xpathCtxt);
	if (res == -1) {
	    VERROR_INT("xmlSchemaXPathProcessHistory",
		"calling xmlStreamPop()");
	    return (-1);
	}
#ifdef DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:   stream pop '%s'\n",
	    sto->sel->xpath);
#endif
	if (sto->nbHistory == 0)
	    goto deregister_check;

	matchDepth = sto->history[sto->nbHistory -1];

	if (matchDepth != depth) {
	    sto = sto->next;
	    continue;
	}
	if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_FIELD) {

	    if (WXS_IS_COMPLEX(type)) {
		if (WXS_HAS_SIMPLE_CONTENT(type)) {
		    simpleType = type->contentTypeDef;
		    if (simpleType == NULL) {
			VERROR_INT("xmlSchemaXPathProcessHistory",
			    "field resolves to a CT with simple content "
			    "but the CT is missing the ST definition");
			return (-1);
		    }
		} else
		    simpleType = NULL;
	    } else
		simpleType = type;
	    if (simpleType == NULL) {
		xmlChar *str = NULL;

		xmlSchemaCustomErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_CVC_IDC, NULL,
		    WXS_BASIC_CAST sto->matcher->aidc->def,
		    "The XPath '%s' of a field of %s does evaluate to a node of "
		    "non-simple type",
		    sto->sel->xpath,
		    xmlSchemaGetIDCDesignation(&str, sto->matcher->aidc->def));
		FREE_AND_NULL(str);
		sto->nbHistory--;
		goto deregister_check;
	    }

	    if ((key == NULL) && (vctxt->inode->val == NULL)) {
		VERROR(XML_SCHEMAV_CVC_IDC,
		    WXS_BASIC_CAST sto->matcher->aidc->def,
		    "Warning: No precomputed value available, the value "
		    "was either invalid or something strange happend");
		sto->nbHistory--;
		goto deregister_check;
	    } else {
		xmlSchemaIDCMatcherPtr matcher = sto->matcher;
		xmlSchemaPSVIIDCKeyPtr *keySeq;
		int pos, idx;

		pos = sto->depth - matcher->depth;
		idx = sto->sel->index;

		if (matcher->keySeqs == NULL) {
		    if (pos > 9)
			matcher->sizeKeySeqs = pos * 2;
		    else
			matcher->sizeKeySeqs = 10;
		    matcher->keySeqs = (xmlSchemaPSVIIDCKeyPtr **)
			xmlMalloc(matcher->sizeKeySeqs *
			sizeof(xmlSchemaPSVIIDCKeyPtr *));
		    if (matcher->keySeqs == NULL) {
			xmlSchemaVErrMemory(NULL,
			    "allocating an array of key-sequences",
			    NULL);
			return(-1);
		    }
		    memset(matcher->keySeqs, 0,
			matcher->sizeKeySeqs *
			sizeof(xmlSchemaPSVIIDCKeyPtr *));
		} else if (pos >= matcher->sizeKeySeqs) {
		    int i = matcher->sizeKeySeqs;

		    matcher->sizeKeySeqs *= 2;
		    matcher->keySeqs = (xmlSchemaPSVIIDCKeyPtr **)
			xmlRealloc(matcher->keySeqs,
			matcher->sizeKeySeqs *
			sizeof(xmlSchemaPSVIIDCKeyPtr *));
		    if (matcher->keySeqs == NULL) {
			xmlSchemaVErrMemory(NULL,
			    "reallocating an array of key-sequences",
			    NULL);
			return (-1);
		    }
		    for (; i < matcher->sizeKeySeqs; i++)
			matcher->keySeqs[i] = NULL;
		}

		keySeq = matcher->keySeqs[pos];
		if (keySeq == NULL) {
		    goto create_sequence;
		} else if (keySeq[idx] != NULL) {
		    xmlChar *str = NULL;
		    xmlSchemaCustomErr(ACTXT_CAST vctxt,
			XML_SCHEMAV_CVC_IDC, NULL,
			WXS_BASIC_CAST matcher->aidc->def,
			"The XPath '%s' of a field of %s evaluates to a "
			"node-set with more than one member",
			sto->sel->xpath,
			xmlSchemaGetIDCDesignation(&str, matcher->aidc->def));
		    FREE_AND_NULL(str);
		    sto->nbHistory--;
		    goto deregister_check;
		} else
		    goto create_key;

create_sequence:
		keySeq = (xmlSchemaPSVIIDCKeyPtr *) xmlMalloc(
		    matcher->aidc->def->nbFields *
		    sizeof(xmlSchemaPSVIIDCKeyPtr));
		if (keySeq == NULL) {
		    xmlSchemaVErrMemory(NULL,
			"allocating an IDC key-sequence", NULL);
		    return(-1);
		}
		memset(keySeq, 0, matcher->aidc->def->nbFields *
		    sizeof(xmlSchemaPSVIIDCKeyPtr));
		matcher->keySeqs[pos] = keySeq;
create_key:
		if (key == NULL) {
		    key = (xmlSchemaPSVIIDCKeyPtr) xmlMalloc(
			sizeof(xmlSchemaPSVIIDCKey));
		    if (key == NULL) {
			xmlSchemaVErrMemory(NULL,
			    "allocating a IDC key", NULL);
			xmlFree(keySeq);
			matcher->keySeqs[pos] = NULL;
			return(-1);
		    }
		    key->type = simpleType;
		    key->val = vctxt->inode->val;
		    vctxt->inode->val = NULL;
		    if (xmlSchemaIDCStoreKey(vctxt, key) == -1) {
			xmlSchemaIDCFreeKey(key);
			return (-1);
		    }
		}
		keySeq[idx] = key;
	    }
	} else if (sto->type == XPATH_STATE_OBJ_TYPE_IDC_SELECTOR) {

	    xmlSchemaPSVIIDCKeyPtr **keySeq = NULL;
	    
	    xmlSchemaPSVIIDCNodePtr ntItem;
	    xmlSchemaIDCMatcherPtr matcher;
	    xmlSchemaIDCPtr idc;
	    xmlSchemaItemListPtr targets;
	    int pos, i, j, nbKeys;
	    matcher = sto->matcher;
	    idc = matcher->aidc->def;
	    nbKeys = idc->nbFields;
	    pos = depth - matcher->depth;
	    if ((matcher->keySeqs == NULL) ||
		(matcher->sizeKeySeqs <= pos)) {
		if (idc->type == XML_SCHEMA_TYPE_IDC_KEY)
		    goto selector_key_error;
		else
		    goto selector_leave;
	    }

	    keySeq = &(matcher->keySeqs[pos]);
	    if (*keySeq == NULL) {
		if (idc->type == XML_SCHEMA_TYPE_IDC_KEY)
		    goto selector_key_error;
		else
		    goto selector_leave;
	    }

	    for (i = 0; i < nbKeys; i++) {
		if ((*keySeq)[i] == NULL) {
		    if (idc->type == XML_SCHEMA_TYPE_IDC_KEY) {
			goto selector_key_error;
		    }
		    goto selector_leave;
		}
	    }

#if 0
	    bind = xmlSchemaIDCAcquireBinding(vctxt, matcher);
#endif
	    targets = xmlSchemaIDCAcquireTargetList(vctxt, matcher);
	    if ((idc->type != XML_SCHEMA_TYPE_IDC_KEYREF) &&
		(targets->nbItems != 0)) {
		xmlSchemaPSVIIDCKeyPtr ckey, bkey, *bkeySeq;

		i = 0;
		res = 0;
		do {
		    bkeySeq =
			((xmlSchemaPSVIIDCNodePtr) targets->items[i])->keys;
		    for (j = 0; j < nbKeys; j++) {
			ckey = (*keySeq)[j];
			bkey = bkeySeq[j];
			res = xmlSchemaAreValuesEqual(ckey->val, bkey->val);
			if (res == -1) {
			    return (-1);
			} else if (res == 0) {
			    break;
			}
		    }
		    if (res == 1) {
			break;
		    }
		    i++;
		} while (i < targets->nbItems);
		if (i != targets->nbItems) {
		    xmlChar *str = NULL, *strB = NULL;
		    xmlSchemaCustomErr(ACTXT_CAST vctxt,
			XML_SCHEMAV_CVC_IDC, NULL,
			WXS_BASIC_CAST idc,
			"Duplicate key-sequence %s in %s",
			xmlSchemaFormatIDCKeySequence(vctxt, &str,
			    (*keySeq), nbKeys),
			xmlSchemaGetIDCDesignation(&strB, idc));
		    FREE_AND_NULL(str);
		    FREE_AND_NULL(strB);
		    goto selector_leave;
		}
	    }
	    ntItem = (xmlSchemaPSVIIDCNodePtr) xmlMalloc(
		sizeof(xmlSchemaPSVIIDCNode));
	    if (ntItem == NULL) {
		xmlSchemaVErrMemory(NULL,
		    "allocating an IDC node-table item", NULL);
		xmlFree(*keySeq);
		*keySeq = NULL;
		return(-1);
	    }
	    memset(ntItem, 0, sizeof(xmlSchemaPSVIIDCNode));

	    if (idc->type != XML_SCHEMA_TYPE_IDC_KEYREF) {
		if (xmlSchemaIDCStoreNodeTableItem(vctxt, ntItem) == -1) {
		    xmlFree(ntItem);
		    xmlFree(*keySeq);
		    *keySeq = NULL;
		    return (-1);
		}
		ntItem->nodeQNameID = -1;
	    } else {
		ntItem->nodeQNameID = xmlSchemaVAddNodeQName(vctxt,
		    vctxt->inode->localName, vctxt->inode->nsName);
		if (ntItem->nodeQNameID == -1) {
		    xmlFree(ntItem);
		    xmlFree(*keySeq);
		    *keySeq = NULL;
		    return (-1);
		}
	    }
	    ntItem->node = vctxt->node;
	    ntItem->nodeLine = vctxt->inode->nodeLine;
	    ntItem->keys = *keySeq;
	    *keySeq = NULL;
#if 0
	    if (xmlSchemaIDCAppendNodeTableItem(bind, ntItem) == -1)
#endif
	    if (xmlSchemaItemListAdd(targets, ntItem) == -1) {
		if (idc->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
		    xmlFree(ntItem->keys);
		    xmlFree(ntItem);
		}
		return (-1);
	    }

	    goto selector_leave;
selector_key_error:
	    {
		xmlChar *str = NULL;
		xmlSchemaCustomErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_CVC_IDC, NULL,
		    WXS_BASIC_CAST idc,
		    "Not all fields of %s evaluate to a node",
		    xmlSchemaGetIDCDesignation(&str, idc), NULL);
		FREE_AND_NULL(str);
	    }
selector_leave:
	    if ((keySeq != NULL) && (*keySeq != NULL)) {
		xmlFree(*keySeq);
		*keySeq = NULL;
	    }
	} 

	sto->nbHistory--;

deregister_check:
	if ((sto->nbHistory == 0) && (sto->depth == depth)) {
#ifdef DEBUG_IDC
	    xmlGenericError(xmlGenericErrorContext, "IDC:   STO pop '%s'\n",
		sto->sel->xpath);
#endif
	    if (vctxt->xpathStates != sto) {
		VERROR_INT("xmlSchemaXPathProcessHistory",
		    "The state object to be removed is not the first "
		    "in the list");
	    }
	    nextsto = sto->next;
	    vctxt->xpathStates = sto->next;
	    sto->next = vctxt->xpathStatePool;
	    vctxt->xpathStatePool = sto;
	    sto = nextsto;
	} else
	    sto = sto->next;
    } 
    return (0);
}

static int
xmlSchemaIDCRegisterMatchers(xmlSchemaValidCtxtPtr vctxt,
			     xmlSchemaElementPtr elemDecl)
{
    xmlSchemaIDCMatcherPtr matcher, last = NULL;
    xmlSchemaIDCPtr idc, refIdc;
    xmlSchemaIDCAugPtr aidc;

    idc = (xmlSchemaIDCPtr) elemDecl->idcs;
    if (idc == NULL)
	return (0);

#ifdef DEBUG_IDC
    {
	xmlChar *str = NULL;
	xmlGenericError(xmlGenericErrorContext,
	    "IDC: REGISTER on %s, depth %d\n",
	    (char *) xmlSchemaFormatQName(&str, vctxt->inode->nsName,
		vctxt->inode->localName), vctxt->depth);
	FREE_AND_NULL(str)
    }
#endif
    if (vctxt->inode->idcMatchers != NULL) {
	VERROR_INT("xmlSchemaIDCRegisterMatchers",
	    "The chain of IDC matchers is expected to be empty");
	return (-1);
    }
    do {
	if (idc->type == XML_SCHEMA_TYPE_IDC_KEYREF) {
	    refIdc = (xmlSchemaIDCPtr) idc->ref->item;
	    if (refIdc != NULL) {
		vctxt->inode->hasKeyrefs = 1;
		aidc = vctxt->aidcs;
		while (aidc != NULL) {
		    if (aidc->def == refIdc)
			break;
		    aidc = aidc->next;
		}
		if (aidc == NULL) {
		    VERROR_INT("xmlSchemaIDCRegisterMatchers",
			"Could not find an augmented IDC item for an IDC "
			"definition");
		    return (-1);
		}
		if ((aidc->keyrefDepth == -1) ||
		    (vctxt->depth < aidc->keyrefDepth))
		    aidc->keyrefDepth = vctxt->depth;
	    }
	}
	aidc = vctxt->aidcs;
	while (aidc != NULL) {
	    if (aidc->def == idc)
		break;
	    aidc = aidc->next;
	}
	if (aidc == NULL) {
	    VERROR_INT("xmlSchemaIDCRegisterMatchers",
		"Could not find an augmented IDC item for an IDC definition");
	    return (-1);
	}
	if (vctxt->idcMatcherCache != NULL) {
	    matcher = vctxt->idcMatcherCache;
	    vctxt->idcMatcherCache = matcher->nextCached;
	    matcher->nextCached = NULL;
	} else {
	    matcher = (xmlSchemaIDCMatcherPtr)
		xmlMalloc(sizeof(xmlSchemaIDCMatcher));
	    if (matcher == NULL) {
		xmlSchemaVErrMemory(vctxt,
		    "allocating an IDC matcher", NULL);
		return (-1);
	    }
	    memset(matcher, 0, sizeof(xmlSchemaIDCMatcher));
	}
	if (last == NULL)
	    vctxt->inode->idcMatchers = matcher;
	else
	    last->next = matcher;
	last = matcher;

	matcher->type = IDC_MATCHER;
	matcher->depth = vctxt->depth;
	matcher->aidc = aidc;
	matcher->idcType = aidc->def->type;
#ifdef DEBUG_IDC
	xmlGenericError(xmlGenericErrorContext, "IDC:   register matcher\n");
#endif
	if (xmlSchemaIDCAddStateObject(vctxt, matcher,
	    idc->selector, XPATH_STATE_OBJ_TYPE_IDC_SELECTOR) == -1)
	    return (-1);

	idc = idc->next;
    } while (idc != NULL);
    return (0);
}

static int
xmlSchemaIDCFillNodeTables(xmlSchemaValidCtxtPtr vctxt,
			   xmlSchemaNodeInfoPtr ielem)
{
    xmlSchemaPSVIIDCBindingPtr bind;
    int res, i, j, k, nbTargets, nbFields, nbDupls, nbNodeTable;
    xmlSchemaPSVIIDCKeyPtr *keys, *ntkeys;
    xmlSchemaPSVIIDCNodePtr *targets, *dupls;

    xmlSchemaIDCMatcherPtr matcher = ielem->idcMatchers;
    
    while (matcher != NULL) {
	if ((matcher->aidc->def->type == XML_SCHEMA_TYPE_IDC_KEYREF) ||
	    WXS_ILIST_IS_EMPTY(matcher->targets))
	{
	    matcher = matcher->next;
	    continue;
	}
	if ((! vctxt->createIDCNodeTables) &&
	    ((matcher->aidc->keyrefDepth == -1) ||
	     (matcher->aidc->keyrefDepth > vctxt->depth)))
	{
	    matcher = matcher->next;
	    continue;
	}
	bind = xmlSchemaIDCAcquireBinding(vctxt, matcher);

	if (! WXS_ILIST_IS_EMPTY(bind->dupls)) {
	    dupls = (xmlSchemaPSVIIDCNodePtr *) bind->dupls->items;
	    nbDupls = bind->dupls->nbItems;
	} else {
	    dupls = NULL;
	    nbDupls = 0;
	}
	if (bind->nodeTable != NULL) {
	    nbNodeTable = bind->nbNodes;
	} else {
	    nbNodeTable = 0;
	}

	if ((nbNodeTable == 0) && (nbDupls == 0)) {
	    bind->nodeTable =
		(xmlSchemaPSVIIDCNodePtr *) matcher->targets->items;
	    bind->sizeNodes = matcher->targets->sizeItems;
	    bind->nbNodes = matcher->targets->nbItems;

	    matcher->targets->items = NULL;
	    matcher->targets->sizeItems = 0;
	    matcher->targets->nbItems = 0;
	} else {
	    nbTargets = matcher->targets->nbItems;
	    targets = (xmlSchemaPSVIIDCNodePtr *) matcher->targets->items;
	    nbFields = matcher->aidc->def->nbFields;
	    i = 0;
	    do {
		keys = targets[i]->keys;
		if (nbDupls) {
		    j = 0;
		    do {
			if (nbFields == 1) {
			    res = xmlSchemaAreValuesEqual(keys[0]->val,
				dupls[j]->keys[0]->val);
			    if (res == -1)
				goto internal_error;
			    if (res == 1) {
				goto next_target;
			    }
			} else {
			    res = 0;
			    ntkeys = dupls[j]->keys;
			    for (k = 0; k < nbFields; k++) {
				res = xmlSchemaAreValuesEqual(keys[k]->val,
				    ntkeys[k]->val);
				if (res == -1)
				    goto internal_error;
				if (res == 0) {
				    break;
				}
			    }
			    if (res == 1) {
				goto next_target;
			    }
			}
			j++;
		    } while (j < nbDupls);
		}
		if (nbNodeTable) {
		    j = 0;
		    do {
			if (nbFields == 1) {
			    res = xmlSchemaAreValuesEqual(keys[0]->val,
				bind->nodeTable[j]->keys[0]->val);
			    if (res == -1)
				goto internal_error;
			    if (res == 0) {
				goto next_node_table_entry;
			    }
			} else {
			    res = 0;
			    ntkeys = bind->nodeTable[j]->keys;
			    for (k = 0; k < nbFields; k++) {
				res = xmlSchemaAreValuesEqual(keys[k]->val,
				    ntkeys[k]->val);
				if (res == -1)
				    goto internal_error;
				if (res == 0) {
				    goto next_node_table_entry;
				}
			    }
			}
			if (bind->dupls == NULL) {
			    bind->dupls = xmlSchemaItemListCreate();
			    if (bind->dupls == NULL)
				goto internal_error;
			}
			if (xmlSchemaItemListAdd(bind->dupls, bind->nodeTable[j]) == -1)
			    goto internal_error;
			bind->nodeTable[j] = bind->nodeTable[bind->nbNodes -1];
			bind->nbNodes--;

			goto next_target;

next_node_table_entry:
			j++;
		    } while (j < nbNodeTable);
		}
		if (xmlSchemaIDCAppendNodeTableItem(bind, targets[i]) == -1)
		    goto internal_error;

next_target:
		i++;
	    } while (i < nbTargets);
	}
	matcher = matcher->next;
    }
    return(0);

internal_error:
    return(-1);
}

static int
xmlSchemaBubbleIDCNodeTables(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaPSVIIDCBindingPtr bind; 
    xmlSchemaPSVIIDCBindingPtr *parTable, parBind = NULL; 
    xmlSchemaPSVIIDCNodePtr node, parNode = NULL, *dupls, *parNodes; 
    xmlSchemaIDCAugPtr aidc;
    int i, j, k, ret = 0, nbFields, oldNum, oldDupls;

    bind = vctxt->inode->idcTable;
    if (bind == NULL) {
	
	return (0);
    }

    parTable = &(vctxt->elemInfos[vctxt->depth -1]->idcTable);
    while (bind != NULL) {

	if ((bind->nbNodes == 0) && WXS_ILIST_IS_EMPTY(bind->dupls))
	    goto next_binding;
	if (! vctxt->createIDCNodeTables) {
	    aidc = vctxt->aidcs;
	    do {
		if (aidc->def == bind->definition) {
		    if ((aidc->keyrefDepth == -1) ||
			(aidc->keyrefDepth >= vctxt->depth)) {
			goto next_binding;
		    }
		    break;
		}
		aidc = aidc->next;
	    } while (aidc != NULL);
	}

	if (parTable != NULL)
	    parBind = *parTable;
	while (parBind != NULL) {
	    if (parBind->definition == bind->definition)
		break;
	    parBind = parBind->next;
	}

	if (parBind != NULL) {
	    oldNum = parBind->nbNodes; 

	    if (! WXS_ILIST_IS_EMPTY(parBind->dupls)) {
		oldDupls = parBind->dupls->nbItems;
		dupls = (xmlSchemaPSVIIDCNodePtr *) parBind->dupls->items;
	    } else {
		dupls = NULL;
		oldDupls = 0;
	    }

	    parNodes = parBind->nodeTable;
	    nbFields = bind->definition->nbFields;

	    for (i = 0; i < bind->nbNodes; i++) {
		node = bind->nodeTable[i];
		if (node == NULL)
		    continue;
		if (oldDupls) {
		    j = 0;
		    while (j < oldDupls) {
			if (nbFields == 1) {
			    ret = xmlSchemaAreValuesEqual(
				node->keys[0]->val,
				dupls[j]->keys[0]->val);
			    if (ret == -1)
				goto internal_error;
			    if (ret == 0) {
				j++;
				continue;
			    }
			} else {
			    parNode = dupls[j];
			    for (k = 0; k < nbFields; k++) {
				ret = xmlSchemaAreValuesEqual(
				    node->keys[k]->val,
				    parNode->keys[k]->val);
				if (ret == -1)
				    goto internal_error;
				if (ret == 0)
				    break;
			    }
			}
			if (ret == 1)
			    
			    break;
			j++;
		    }
		    if (j != oldDupls) {
			
			continue;
		    }
		}
		if (oldNum) {
		    j = 0;
		    while (j < oldNum) {
			parNode = parNodes[j];
			if (nbFields == 1) {
			    ret = xmlSchemaAreValuesEqual(
				node->keys[0]->val,
				parNode->keys[0]->val);
			    if (ret == -1)
				goto internal_error;
			    if (ret == 0) {
				j++;
				continue;
			    }
			} else {
			    for (k = 0; k < nbFields; k++) {
				ret = xmlSchemaAreValuesEqual(
				    node->keys[k]->val,
				    parNode->keys[k]->val);
				if (ret == -1)
				    goto internal_error;
				if (ret == 0)
				    break;
			    }
			}
			if (ret == 1)
			    
			    break;
			j++;
		    }
		    if (j != oldNum) {
			oldNum--;
			parBind->nbNodes--;
			parNodes[j] = parNodes[oldNum];

			if (parBind->nbNodes != oldNum) {
			    parNodes[oldNum] =
				parNodes[parBind->nbNodes];
			}
			if (parBind->dupls == NULL) {
			    parBind->dupls = xmlSchemaItemListCreate();
			    if (parBind->dupls == NULL)
				goto internal_error;
			}
			xmlSchemaItemListAdd(parBind->dupls, parNode);
		    } else {
			if (parBind->nodeTable == NULL) {
			    parBind->nodeTable = (xmlSchemaPSVIIDCNodePtr *)
				xmlMalloc(10 * sizeof(xmlSchemaPSVIIDCNodePtr));
			    if (parBind->nodeTable == NULL) {
				xmlSchemaVErrMemory(NULL,
				    "allocating IDC list of node-table items", NULL);
				goto internal_error;
			    }
			    parBind->sizeNodes = 1;
			} else if (parBind->nbNodes >= parBind->sizeNodes) {
			    parBind->sizeNodes *= 2;
			    parBind->nodeTable = (xmlSchemaPSVIIDCNodePtr *)
				xmlRealloc(parBind->nodeTable, parBind->sizeNodes *
				sizeof(xmlSchemaPSVIIDCNodePtr));
			    if (parBind->nodeTable == NULL) {
				xmlSchemaVErrMemory(NULL,
				    "re-allocating IDC list of node-table items", NULL);
				goto internal_error;
			    }
			}
			parNodes = parBind->nodeTable;
			parNodes[parBind->nbNodes++] = node;
		    }

		}

	    }
	} else {
	    parBind = xmlSchemaIDCNewBinding(bind->definition);
	    if (parBind == NULL)
		goto internal_error;

	    if (bind->nbNodes != 0) {
		if (! vctxt->psviExposeIDCNodeTables) {
		    parBind->nodeTable = bind->nodeTable;
		    bind->nodeTable = NULL;
		    parBind->sizeNodes = bind->sizeNodes;
		    bind->sizeNodes = 0;
		    parBind->nbNodes = bind->nbNodes;
		    bind->nbNodes = 0;
		} else {
		    parBind->nodeTable = (xmlSchemaPSVIIDCNodePtr *)
			xmlMalloc(bind->nbNodes *
			sizeof(xmlSchemaPSVIIDCNodePtr));
		    if (parBind->nodeTable == NULL) {
			xmlSchemaVErrMemory(NULL,
			    "allocating an array of IDC node-table "
			    "items", NULL);
			xmlSchemaIDCFreeBinding(parBind);
			goto internal_error;
		    }
		    parBind->sizeNodes = bind->nbNodes;
		    parBind->nbNodes = bind->nbNodes;
		    memcpy(parBind->nodeTable, bind->nodeTable,
			bind->nbNodes * sizeof(xmlSchemaPSVIIDCNodePtr));
		}
	    }
	    if (bind->dupls) {
		if (parBind->dupls != NULL)
		    xmlSchemaItemListFree(parBind->dupls);
		parBind->dupls = bind->dupls;
		bind->dupls = NULL;
	    }
            if (parTable != NULL) {
                if (*parTable == NULL)
                    *parTable = parBind;
                else {
                    parBind->next = *parTable;
                    *parTable = parBind;
                }
            }
	}

next_binding:
	bind = bind->next;
    }
    return (0);

internal_error:
    return(-1);
}

static int
xmlSchemaCheckCVCIDCKeyRef(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaIDCMatcherPtr matcher;
    xmlSchemaPSVIIDCBindingPtr bind;

    matcher = vctxt->inode->idcMatchers;
    while (matcher != NULL) {
	if ((matcher->idcType == XML_SCHEMA_TYPE_IDC_KEYREF) &&
	    matcher->targets &&
	    matcher->targets->nbItems)
	{
	    int i, j, k, res, nbFields, hasDupls;
	    xmlSchemaPSVIIDCKeyPtr *refKeys, *keys;
	    xmlSchemaPSVIIDCNodePtr refNode = NULL;

	    nbFields = matcher->aidc->def->nbFields;

	    bind = vctxt->inode->idcTable;
	    while (bind != NULL) {
		if ((xmlSchemaIDCPtr) matcher->aidc->def->ref->item ==
		    bind->definition)
		    break;
		bind = bind->next;
	    }
	    hasDupls = (bind && bind->dupls && bind->dupls->nbItems) ? 1 : 0;
	    for (i = 0; i < matcher->targets->nbItems; i++) {
		res = 0;
		refNode = matcher->targets->items[i];
		if (bind != NULL) {
		    refKeys = refNode->keys;
		    for (j = 0; j < bind->nbNodes; j++) {
			keys = bind->nodeTable[j]->keys;
			for (k = 0; k < nbFields; k++) {
			    res = xmlSchemaAreValuesEqual(keys[k]->val,
				refKeys[k]->val);
			    if (res == 0)
				break;
			    else if (res == -1) {
				return (-1);
			    }
			}
			if (res == 1) {
			    break;
			}
		    }
		    if ((res == 0) && hasDupls) {
			for (j = 0; j < bind->dupls->nbItems; j++) {
			    keys = ((xmlSchemaPSVIIDCNodePtr)
				bind->dupls->items[j])->keys;
			    for (k = 0; k < nbFields; k++) {
				res = xmlSchemaAreValuesEqual(keys[k]->val,
				    refKeys[k]->val);
				if (res == 0)
				    break;
				else if (res == -1) {
				    return (-1);
				}
			    }
			    if (res == 1) {
				xmlChar *str = NULL, *strB = NULL;
				xmlSchemaKeyrefErr(vctxt,
				    XML_SCHEMAV_CVC_IDC, refNode,
				    (xmlSchemaTypePtr) matcher->aidc->def,
				    "More than one match found for "
				    "key-sequence %s of keyref '%s'",
				    xmlSchemaFormatIDCKeySequence(vctxt, &str,
					refNode->keys, nbFields),
				    xmlSchemaGetComponentQName(&strB,
					matcher->aidc->def));
				FREE_AND_NULL(str);
				FREE_AND_NULL(strB);
				break;
			    }
			}
		    }
		}

		if (res == 0) {
		    xmlChar *str = NULL, *strB = NULL;
		    xmlSchemaKeyrefErr(vctxt,
			XML_SCHEMAV_CVC_IDC, refNode,
			(xmlSchemaTypePtr) matcher->aidc->def,
			"No match found for key-sequence %s of keyref '%s'",
			xmlSchemaFormatIDCKeySequence(vctxt, &str,
			    refNode->keys, nbFields),
			xmlSchemaGetComponentQName(&strB, matcher->aidc->def));
		    FREE_AND_NULL(str);
		    FREE_AND_NULL(strB);
		}
	    }
	}
	matcher = matcher->next;
    }
    
    return (0);
}


static xmlSchemaAttrInfoPtr
xmlSchemaGetFreshAttrInfo(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaAttrInfoPtr iattr;
    if (vctxt->attrInfos == NULL) {
	vctxt->attrInfos = (xmlSchemaAttrInfoPtr *)
	    xmlMalloc(sizeof(xmlSchemaAttrInfoPtr));
	vctxt->sizeAttrInfos = 1;
	if (vctxt->attrInfos == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"allocating attribute info list", NULL);
	    return (NULL);
	}
    } else if (vctxt->sizeAttrInfos <= vctxt->nbAttrInfos) {
	vctxt->sizeAttrInfos++;
	vctxt->attrInfos = (xmlSchemaAttrInfoPtr *)
	    xmlRealloc(vctxt->attrInfos,
		vctxt->sizeAttrInfos * sizeof(xmlSchemaAttrInfoPtr));
	if (vctxt->attrInfos == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"re-allocating attribute info list", NULL);
	    return (NULL);
	}
    } else {
	iattr = vctxt->attrInfos[vctxt->nbAttrInfos++];
	if (iattr->localName != NULL) {
	    VERROR_INT("xmlSchemaGetFreshAttrInfo",
		"attr info not cleared");
	    return (NULL);
	}
	iattr->nodeType = XML_ATTRIBUTE_NODE;
	return (iattr);
    }
    iattr = (xmlSchemaAttrInfoPtr)
	xmlMalloc(sizeof(xmlSchemaAttrInfo));
    if (iattr == NULL) {
	xmlSchemaVErrMemory(vctxt, "creating new attribute info", NULL);
	return (NULL);
    }
    memset(iattr, 0, sizeof(xmlSchemaAttrInfo));
    iattr->nodeType = XML_ATTRIBUTE_NODE;
    vctxt->attrInfos[vctxt->nbAttrInfos++] = iattr;

    return (iattr);
}

static int
xmlSchemaValidatorPushAttribute(xmlSchemaValidCtxtPtr vctxt,
			xmlNodePtr attrNode,
			int nodeLine,
			const xmlChar *localName,
			const xmlChar *nsName,
			int ownedNames,
			xmlChar *value,
			int ownedValue)
{
    xmlSchemaAttrInfoPtr attr;

    attr = xmlSchemaGetFreshAttrInfo(vctxt);
    if (attr == NULL) {
	VERROR_INT("xmlSchemaPushAttribute",
	    "calling xmlSchemaGetFreshAttrInfo()");
	return (-1);
    }
    attr->node = attrNode;
    attr->nodeLine = nodeLine;
    attr->state = XML_SCHEMAS_ATTR_UNKNOWN;
    attr->localName = localName;
    attr->nsName = nsName;
    if (ownedNames)
	attr->flags |= XML_SCHEMA_NODE_INFO_FLAG_OWNED_NAMES;
    if (nsName != NULL) {
	if (xmlStrEqual(localName, BAD_CAST "nil")) {
	    if (xmlStrEqual(attr->nsName, xmlSchemaInstanceNs)) {
		attr->metaType = XML_SCHEMA_ATTR_INFO_META_XSI_NIL;
	    }
	} else if (xmlStrEqual(localName, BAD_CAST "type")) {
	    if (xmlStrEqual(attr->nsName, xmlSchemaInstanceNs)) {
		attr->metaType = XML_SCHEMA_ATTR_INFO_META_XSI_TYPE;
	    }
	} else if (xmlStrEqual(localName, BAD_CAST "schemaLocation")) {
	    if (xmlStrEqual(attr->nsName, xmlSchemaInstanceNs)) {
		attr->metaType = XML_SCHEMA_ATTR_INFO_META_XSI_SCHEMA_LOC;
	    }
	} else if (xmlStrEqual(localName, BAD_CAST "noNamespaceSchemaLocation")) {
	    if (xmlStrEqual(attr->nsName, xmlSchemaInstanceNs)) {
		attr->metaType = XML_SCHEMA_ATTR_INFO_META_XSI_NO_NS_SCHEMA_LOC;
	    }
	} else if (xmlStrEqual(attr->nsName, xmlNamespaceNs)) {
	    attr->metaType = XML_SCHEMA_ATTR_INFO_META_XMLNS;
	}
    }
    attr->value = value;
    if (ownedValue)
	attr->flags |= XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES;
    if (attr->metaType != 0)
	attr->state = XML_SCHEMAS_ATTR_META;
    return (0);
}

static void
xmlSchemaClearElemInfo(xmlSchemaValidCtxtPtr vctxt,
		       xmlSchemaNodeInfoPtr ielem)
{
    ielem->hasKeyrefs = 0;
    ielem->appliedXPath = 0;
    if (ielem->flags & XML_SCHEMA_NODE_INFO_FLAG_OWNED_NAMES) {
	FREE_AND_NULL(ielem->localName);
	FREE_AND_NULL(ielem->nsName);
    } else {
	ielem->localName = NULL;
	ielem->nsName = NULL;
    }
    if (ielem->flags & XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES) {
	FREE_AND_NULL(ielem->value);
    } else {
	ielem->value = NULL;
    }
    if (ielem->val != NULL) {
	xmlSchemaFreeValue(ielem->val);
	ielem->val = NULL;
    }
    if (ielem->idcMatchers != NULL) {
	xmlSchemaIDCReleaseMatcherList(vctxt, ielem->idcMatchers);
#if 0
	xmlSchemaIDCFreeMatcherList(ielem->idcMatchers);
#endif
	ielem->idcMatchers = NULL;
    }
    if (ielem->idcTable != NULL) {
	xmlSchemaIDCFreeIDCTable(ielem->idcTable);
	ielem->idcTable = NULL;
    }
    if (ielem->regexCtxt != NULL) {
	xmlRegFreeExecCtxt(ielem->regexCtxt);
	ielem->regexCtxt = NULL;
    }
    if (ielem->nsBindings != NULL) {
	xmlFree((xmlChar **)ielem->nsBindings);
	ielem->nsBindings = NULL;
	ielem->nbNsBindings = 0;
	ielem->sizeNsBindings = 0;
    }
}

static xmlSchemaNodeInfoPtr
xmlSchemaGetFreshElemInfo(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaNodeInfoPtr info = NULL;

    if (vctxt->depth > vctxt->sizeElemInfos) {
	VERROR_INT("xmlSchemaGetFreshElemInfo",
	    "inconsistent depth encountered");
	return (NULL);
    }
    if (vctxt->elemInfos == NULL) {
	vctxt->elemInfos = (xmlSchemaNodeInfoPtr *)
	    xmlMalloc(10 * sizeof(xmlSchemaNodeInfoPtr));
	if (vctxt->elemInfos == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"allocating the element info array", NULL);
	    return (NULL);
	}
	memset(vctxt->elemInfos, 0, 10 * sizeof(xmlSchemaNodeInfoPtr));
	vctxt->sizeElemInfos = 10;
    } else if (vctxt->sizeElemInfos <= vctxt->depth) {
	int i = vctxt->sizeElemInfos;

	vctxt->sizeElemInfos *= 2;
	vctxt->elemInfos = (xmlSchemaNodeInfoPtr *)
	    xmlRealloc(vctxt->elemInfos, vctxt->sizeElemInfos *
	    sizeof(xmlSchemaNodeInfoPtr));
	if (vctxt->elemInfos == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"re-allocating the element info array", NULL);
	    return (NULL);
	}
	for (; i < vctxt->sizeElemInfos; i++)
	    vctxt->elemInfos[i] = NULL;
    } else
	info = vctxt->elemInfos[vctxt->depth];

    if (info == NULL) {
	info = (xmlSchemaNodeInfoPtr)
	    xmlMalloc(sizeof(xmlSchemaNodeInfo));
	if (info == NULL) {
	    xmlSchemaVErrMemory(vctxt,
		"allocating an element info", NULL);
	    return (NULL);
	}
	vctxt->elemInfos[vctxt->depth] = info;
    } else {
	if (info->localName != NULL) {
	    VERROR_INT("xmlSchemaGetFreshElemInfo",
		"elem info has not been cleared");
	    return (NULL);
	}
    }
    memset(info, 0, sizeof(xmlSchemaNodeInfo));
    info->nodeType = XML_ELEMENT_NODE;
    info->depth = vctxt->depth;

    return (info);
}

#define ACTIVATE_ATTRIBUTE(item) vctxt->inode = (xmlSchemaNodeInfoPtr) item;
#define ACTIVATE_ELEM vctxt->inode = vctxt->elemInfos[vctxt->depth];
#define ACTIVATE_PARENT_ELEM vctxt->inode = vctxt->elemInfos[vctxt->depth -1];

static int
xmlSchemaValidateFacets(xmlSchemaAbstractCtxtPtr actxt,
			xmlNodePtr node,
			xmlSchemaTypePtr type,
			xmlSchemaValType valType,
			const xmlChar * value,
			xmlSchemaValPtr val,
			unsigned long length,
			int fireErrors)
{
    int ret, error = 0;

    xmlSchemaTypePtr tmpType;
    xmlSchemaFacetLinkPtr facetLink;
    xmlSchemaFacetPtr facet;
    unsigned long len = 0;
    xmlSchemaWhitespaceValueType ws;

    if (type->type == XML_SCHEMA_TYPE_BASIC)
	return (0);

    if (type->facetSet == NULL)
	goto pattern_and_enum;

    if (! WXS_IS_ATOMIC(type)) {
	if (WXS_IS_LIST(type))
	    goto WXS_IS_LIST;
	else
	    goto pattern_and_enum;
    }
    tmpType = xmlSchemaGetPrimitiveType(type);
    if ((tmpType->builtInType == XML_SCHEMAS_STRING) ||
	WXS_IS_ANY_SIMPLE_TYPE(tmpType)) {
	ws = xmlSchemaGetWhiteSpaceFacetValue(type);
    } else
	ws = XML_SCHEMA_WHITESPACE_COLLAPSE;
    if (val == NULL)
	valType = valType;
    else
	valType = xmlSchemaGetValType(val);

    ret = 0;
    for (facetLink = type->facetSet; facetLink != NULL;
	facetLink = facetLink->next) {
	switch (facetLink->facet->type) {
	    case XML_SCHEMA_FACET_WHITESPACE:
	    case XML_SCHEMA_FACET_PATTERN:
	    case XML_SCHEMA_FACET_ENUMERATION:
		continue;
	    case XML_SCHEMA_FACET_LENGTH:
	    case XML_SCHEMA_FACET_MINLENGTH:
	    case XML_SCHEMA_FACET_MAXLENGTH:
		ret = xmlSchemaValidateLengthFacetWhtsp(facetLink->facet,
		    valType, value, val, &len, ws);
		break;
	    default:
		ret = xmlSchemaValidateFacetWhtsp(facetLink->facet, ws,
		    valType, value, val, ws);
		break;
	}
	if (ret < 0) {
	    AERROR_INT("xmlSchemaValidateFacets",
		"validating against a atomic type facet");
	    return (-1);
	} else if (ret > 0) {
	    if (fireErrors)
		xmlSchemaFacetErr(actxt, ret, node,
		value, len, type, facetLink->facet, NULL, NULL, NULL);
	    else
		return (ret);
	    if (error == 0)
		error = ret;
	}
	ret = 0;
    }

WXS_IS_LIST:
    if (! WXS_IS_LIST(type))
	goto pattern_and_enum;
    ret = 0;
    for (facetLink = type->facetSet; facetLink != NULL;
	facetLink = facetLink->next) {

	switch (facetLink->facet->type) {
	    case XML_SCHEMA_FACET_LENGTH:
	    case XML_SCHEMA_FACET_MINLENGTH:
	    case XML_SCHEMA_FACET_MAXLENGTH:
		ret = xmlSchemaValidateListSimpleTypeFacet(facetLink->facet,
		    value, length, NULL);
		break;
	    default:
		continue;
	}
	if (ret < 0) {
	    AERROR_INT("xmlSchemaValidateFacets",
		"validating against a list type facet");
	    return (-1);
	} else if (ret > 0) {
	    if (fireErrors)
		xmlSchemaFacetErr(actxt, ret, node,
		value, length, type, facetLink->facet, NULL, NULL, NULL);
	    else
		return (ret);
	    if (error == 0)
		error = ret;
	}
	ret = 0;
    }

pattern_and_enum:
    if (error >= 0) {
	int found = 0;
	ret = 0;
	tmpType = type;
	do {
	    for (facet = tmpType->facets; facet != NULL; facet = facet->next) {
		if (facet->type != XML_SCHEMA_FACET_ENUMERATION)
		    continue;
		found = 1;
		ret = xmlSchemaAreValuesEqual(facet->val, val);
		if (ret == 1)
		    break;
		else if (ret < 0) {
		    AERROR_INT("xmlSchemaValidateFacets",
			"validating against an enumeration facet");
		    return (-1);
		}
	    }
	    if (ret != 0)
		break;
	    if (found)
		break;
	    tmpType = tmpType->baseType;
	} while ((tmpType != NULL) &&
	    (tmpType->type != XML_SCHEMA_TYPE_BASIC));
	if (found && (ret == 0)) {
	    ret = XML_SCHEMAV_CVC_ENUMERATION_VALID;
	    if (fireErrors) {
		xmlSchemaFacetErr(actxt, ret, node,
		    value, 0, type, NULL, NULL, NULL, NULL);
	    } else
		return (ret);
	    if (error == 0)
		error = ret;
	}
    }

    if (error >= 0) {
	int found;
	tmpType = type;
	facet = NULL;
	do {
	    found = 0;
	    for (facetLink = tmpType->facetSet; facetLink != NULL;
		facetLink = facetLink->next) {
		if (facetLink->facet->type != XML_SCHEMA_FACET_PATTERN)
		    continue;
		found = 1;
		ret = xmlRegexpExec(facetLink->facet->regexp, value);
		if (ret == 1)
		    break;
		else if (ret < 0) {
		    AERROR_INT("xmlSchemaValidateFacets",
			"validating against a pattern facet");
		    return (-1);
		} else {
		    facet = facetLink->facet;
		}
	    }
	    if (found && (ret != 1)) {
		ret = XML_SCHEMAV_CVC_PATTERN_VALID;
		if (fireErrors) {
		    xmlSchemaFacetErr(actxt, ret, node,
			value, 0, type, facet, NULL, NULL, NULL);
		} else
		    return (ret);
		if (error == 0)
		    error = ret;
		break;
	    }
	    tmpType = tmpType->baseType;
	} while ((tmpType != NULL) && (tmpType->type != XML_SCHEMA_TYPE_BASIC));
    }

    return (error);
}

static xmlChar *
xmlSchemaNormalizeValue(xmlSchemaTypePtr type,
			const xmlChar *value)
{
    switch (xmlSchemaGetWhiteSpaceFacetValue(type)) {
	case XML_SCHEMA_WHITESPACE_COLLAPSE:
	    return (xmlSchemaCollapseString(value));
	case XML_SCHEMA_WHITESPACE_REPLACE:
	    return (xmlSchemaWhiteSpaceReplace(value));
	default:
	    return (NULL);
    }
}

static int
xmlSchemaValidateQName(xmlSchemaValidCtxtPtr vctxt,
		       const xmlChar *value,
		       xmlSchemaValPtr *val,
		       int valNeeded)
{
    int ret;
    const xmlChar *nsName;
    xmlChar *local, *prefix = NULL;

    ret = xmlValidateQName(value, 1);
    if (ret != 0) {
	if (ret == -1) {
	    VERROR_INT("xmlSchemaValidateQName",
		"calling xmlValidateQName()");
	    return (-1);
	}
	return( XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1);
    }
    local = xmlSplitQName2(value, &prefix);
    if (local == NULL)
	local = xmlStrdup(value);
    nsName = xmlSchemaLookupNamespace(vctxt, prefix);

    if (prefix != NULL) {
	xmlFree(prefix);
	if (nsName == NULL) {
	    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
	    xmlSchemaCustomErr(ACTXT_CAST vctxt, ret, NULL,
		WXS_BASIC_CAST xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME),
		"The QName value '%s' has no "
		"corresponding namespace declaration in "
		"scope", value, NULL);
	    if (local != NULL)
		xmlFree(local);
	    return (ret);
	}
    }
    if (valNeeded && val) {
	if (nsName != NULL)
	    *val = xmlSchemaNewQNameValue(
		BAD_CAST xmlStrdup(nsName), BAD_CAST local);
	else
	    *val = xmlSchemaNewQNameValue(NULL,
		BAD_CAST local);
    } else
	xmlFree(local);
    return (0);
}

static int
xmlSchemaVCheckCVCSimpleType(xmlSchemaAbstractCtxtPtr actxt,
			     xmlNodePtr node,
			     xmlSchemaTypePtr type,
			     const xmlChar *value,
			     xmlSchemaValPtr *retVal,
			     int fireErrors,
			     int normalize,
			     int isNormalized)
{
    int ret = 0, valNeeded = (retVal) ? 1 : 0;
    xmlSchemaValPtr val = NULL;
    
    xmlChar *normValue = NULL;

#define NORMALIZE(atype) \
    if ((! isNormalized) && \
    (normalize || (type->flags & XML_SCHEMAS_TYPE_NORMVALUENEEDED))) { \
	normValue = xmlSchemaNormalizeValue(atype, value); \
	if (normValue != NULL) \
	    value = normValue; \
	isNormalized = 1; \
    }

    if ((retVal != NULL) && (*retVal != NULL)) {
	xmlSchemaFreeValue(*retVal);
	*retVal = NULL;
    }
    if ((! valNeeded) && (type->flags & XML_SCHEMAS_TYPE_FACETSNEEDVALUE))
	valNeeded = 1;
    if (value == NULL)
	value = BAD_CAST "";
    if (WXS_IS_ANY_SIMPLE_TYPE(type) || WXS_IS_ATOMIC(type)) {
	xmlSchemaTypePtr biType; 
	NORMALIZE(type);
	if (type->type != XML_SCHEMA_TYPE_BASIC) {
	    biType = type->baseType;
	    while ((biType != NULL) &&
		(biType->type != XML_SCHEMA_TYPE_BASIC))
		biType = biType->baseType;

	    if (biType == NULL) {
		AERROR_INT("xmlSchemaVCheckCVCSimpleType",
		    "could not get the built-in type");
		goto internal_error;
	    }
	} else
	    biType = type;
	if (actxt->type == XML_SCHEMA_CTXT_VALIDATOR) {
	    switch (biType->builtInType) {
		case XML_SCHEMAS_NOTATION:
		    ret = xmlSchemaValidateNotation(
			(xmlSchemaValidCtxtPtr) actxt,
			((xmlSchemaValidCtxtPtr) actxt)->schema,
			NULL, value, &val, valNeeded);
		    break;
		case XML_SCHEMAS_QNAME:
		    ret = xmlSchemaValidateQName((xmlSchemaValidCtxtPtr) actxt,
			value, &val, valNeeded);
		    break;
		default:
		    
		    if (valNeeded)
			ret = xmlSchemaValPredefTypeNodeNoNorm(biType,
			    value, &val, node);
		    else
			ret = xmlSchemaValPredefTypeNodeNoNorm(biType,
			    value, NULL, node);
		    break;
	    }
	} else if (actxt->type == XML_SCHEMA_CTXT_PARSER) {
	    switch (biType->builtInType) {
		case XML_SCHEMAS_NOTATION:
		    ret = xmlSchemaValidateNotation(NULL,
			((xmlSchemaParserCtxtPtr) actxt)->schema, node,
			value, &val, valNeeded);
		    break;
		default:
		    
		    if (valNeeded)
			ret = xmlSchemaValPredefTypeNodeNoNorm(biType,
			    value, &val, node);
		    else
			ret = xmlSchemaValPredefTypeNodeNoNorm(biType,
			    value, NULL, node);
		    break;
	    }
	} else {
	    TODO
	    goto internal_error;
	}
	if (ret != 0) {
	    if (ret < 0) {
		AERROR_INT("xmlSchemaVCheckCVCSimpleType",
		    "validating against a built-in type");
		goto internal_error;
	    }
	    if (WXS_IS_LIST(type))
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
	    else
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
	}
	if ((ret == 0) && (type->flags & XML_SCHEMAS_TYPE_HAS_FACETS)) {
	    ret = xmlSchemaValidateFacets(actxt, node, type,
		(xmlSchemaValType) biType->builtInType, value, val,
		0, fireErrors);
	    if (ret != 0) {
		if (ret < 0) {
		    AERROR_INT("xmlSchemaVCheckCVCSimpleType",
			"validating facets of atomic simple type");
		    goto internal_error;
		}
		if (WXS_IS_LIST(type))
		    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
		else
		    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1;
	    }
	}
	if (fireErrors && (ret > 0))
	    xmlSchemaSimpleTypeErr(actxt, ret, node, value, type, 1);
    } else if (WXS_IS_LIST(type)) {

	xmlSchemaTypePtr itemType;
	const xmlChar *cur, *end;
	xmlChar *tmpValue = NULL;
	unsigned long len = 0;
	xmlSchemaValPtr prevVal = NULL, curVal = NULL;
	NORMALIZE(type);
	itemType = WXS_LIST_ITEMTYPE(type);
	cur = value;
	do {
	    while (IS_BLANK_CH(*cur))
		cur++;
	    end = cur;
	    while ((*end != 0) && (!(IS_BLANK_CH(*end))))
		end++;
	    if (end == cur)
		break;
	    tmpValue = xmlStrndup(cur, end - cur);
	    len++;

	    if (valNeeded)
		ret = xmlSchemaVCheckCVCSimpleType(actxt, node, itemType,
		    tmpValue, &curVal, fireErrors, 0, 1);
	    else
		ret = xmlSchemaVCheckCVCSimpleType(actxt, node, itemType,
		    tmpValue, NULL, fireErrors, 0, 1);
	    FREE_AND_NULL(tmpValue);
	    if (curVal != NULL) {
		if (val == NULL)
		    val = curVal;
		else
		    xmlSchemaValueAppend(prevVal, curVal);
		prevVal = curVal;
		curVal = NULL;
	    }
	    if (ret != 0) {
		if (ret < 0) {
		    AERROR_INT("xmlSchemaVCheckCVCSimpleType",
			"validating an item of list simple type");
		    goto internal_error;
		}
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
		break;
	    }
	    cur = end;
	} while (*cur != 0);
	FREE_AND_NULL(tmpValue);
	if ((ret == 0) && (type->flags & XML_SCHEMAS_TYPE_HAS_FACETS)) {
	    ret = xmlSchemaValidateFacets(actxt, node, type,
		XML_SCHEMAS_UNKNOWN, value, val,
		len, fireErrors);
	    if (ret != 0) {
		if (ret < 0) {
		    AERROR_INT("xmlSchemaVCheckCVCSimpleType",
			"validating facets of list simple type");
		    goto internal_error;
		}
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_2;
	    }
	}
	if (fireErrors && (ret > 0)) {
	    normalize = 1;
	    NORMALIZE(type);
	    xmlSchemaSimpleTypeErr(actxt, ret, node, value, type, 1);
	}
    } else if (WXS_IS_UNION(type)) {
	xmlSchemaTypeLinkPtr memberLink;

	memberLink = xmlSchemaGetUnionSimpleTypeMemberTypes(type);
	if (memberLink == NULL) {
	    AERROR_INT("xmlSchemaVCheckCVCSimpleType",
		"union simple type has no member types");
	    goto internal_error;
	}
	while (memberLink != NULL) {
	    if (valNeeded)
		ret = xmlSchemaVCheckCVCSimpleType(actxt, node,
		    memberLink->type, value, &val, 0, 1, 0);
	    else
		ret = xmlSchemaVCheckCVCSimpleType(actxt, node,
		    memberLink->type, value, NULL, 0, 1, 0);
	    if (ret <= 0)
		break;
	    memberLink = memberLink->next;
	}
	if (ret != 0) {
	    if (ret < 0) {
		AERROR_INT("xmlSchemaVCheckCVCSimpleType",
		    "validating members of union simple type");
		goto internal_error;
	    }
	    ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_3;
	}
	if ((ret == 0) && (type->flags & XML_SCHEMAS_TYPE_HAS_FACETS)) {
	    NORMALIZE(memberLink->type);
	    ret = xmlSchemaValidateFacets(actxt, node, type,
		XML_SCHEMAS_UNKNOWN, value, val,
		0, fireErrors);
	    if (ret != 0) {
		if (ret < 0) {
		    AERROR_INT("xmlSchemaVCheckCVCSimpleType",
			"validating facets of union simple type");
		    goto internal_error;
		}
		ret = XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_3;
	    }
	}
	if (fireErrors && (ret > 0))
	    xmlSchemaSimpleTypeErr(actxt, ret, node, value, type, 1);
    }

    if (normValue != NULL)
	xmlFree(normValue);
    if (ret == 0) {
	if (retVal != NULL)
	    *retVal = val;
	else if (val != NULL)
	    xmlSchemaFreeValue(val);
    } else if (val != NULL)
	xmlSchemaFreeValue(val);
    return (ret);
internal_error:
    if (normValue != NULL)
	xmlFree(normValue);
    if (val != NULL)
	xmlSchemaFreeValue(val);
    return (-1);
}

static int
xmlSchemaVExpandQName(xmlSchemaValidCtxtPtr vctxt,
			   const xmlChar *value,
			   const xmlChar **nsName,
			   const xmlChar **localName)
{
    int ret = 0;

    if ((nsName == NULL) || (localName == NULL))
	return (-1);
    *nsName = NULL;
    *localName = NULL;

    ret = xmlValidateQName(value, 1);
    if (ret == -1)
	return (-1);
    if (ret > 0) {
	xmlSchemaSimpleTypeErr(ACTXT_CAST vctxt,
	    XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1, NULL,
	    value, xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME), 1);
	return (1);
    }
    {
	xmlChar *local = NULL;
	xmlChar *prefix;

	local = xmlSplitQName2(value, &prefix);
	if (local == NULL)
	    *localName = xmlDictLookup(vctxt->dict, value, -1);
	else {
	    *localName = xmlDictLookup(vctxt->dict, local, -1);
	    xmlFree(local);
	}

	*nsName = xmlSchemaLookupNamespace(vctxt, prefix);

	if (prefix != NULL) {
	    xmlFree(prefix);
	    if (*nsName == NULL) {
		xmlSchemaCustomErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_CVC_DATATYPE_VALID_1_2_1, NULL,
		    WXS_BASIC_CAST xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME),
		    "The QName value '%s' has no "
		    "corresponding namespace declaration in scope",
		    value, NULL);
		return (2);
	    }
	}
    }
    return (0);
}

static int
xmlSchemaProcessXSIType(xmlSchemaValidCtxtPtr vctxt,
			xmlSchemaAttrInfoPtr iattr,
			xmlSchemaTypePtr *localType,
			xmlSchemaElementPtr elemDecl)
{
    int ret = 0;
    if (localType == NULL)
	return (-1);
    *localType = NULL;
    if (iattr == NULL)
	return (0);
    else {
	const xmlChar *nsName = NULL, *local = NULL;
	ACTIVATE_ATTRIBUTE(iattr);
	ret = xmlSchemaVExpandQName(vctxt, iattr->value,
	    &nsName, &local);
	if (ret != 0) {
	    if (ret < 0) {
		VERROR_INT("xmlSchemaValidateElementByDeclaration",
		    "calling xmlSchemaQNameExpand() to validate the "
		    "attribute 'xsi:type'");
		goto internal_error;
	    }
	    goto exit;
	}
	*localType = xmlSchemaGetType(vctxt->schema, local, nsName);
	if (*localType == NULL) {
	    xmlChar *str = NULL;

	    xmlSchemaCustomErr(ACTXT_CAST vctxt,
		XML_SCHEMAV_CVC_ELT_4_2, NULL,
		WXS_BASIC_CAST xmlSchemaGetBuiltInType(XML_SCHEMAS_QNAME),
		"The QName value '%s' of the xsi:type attribute does not "
		"resolve to a type definition",
		xmlSchemaFormatQName(&str, nsName, local), NULL);
	    FREE_AND_NULL(str);
	    ret = vctxt->err;
	    goto exit;
	}
	if (elemDecl != NULL) {
	    int set = 0;

	    if ((elemDecl->flags & XML_SCHEMAS_ELEM_BLOCK_EXTENSION) ||
		(elemDecl->subtypes->flags &
		    XML_SCHEMAS_TYPE_BLOCK_EXTENSION))
		set |= SUBSET_EXTENSION;

	    if ((elemDecl->flags & XML_SCHEMAS_ELEM_BLOCK_RESTRICTION) ||
		(elemDecl->subtypes->flags &
		    XML_SCHEMAS_TYPE_BLOCK_RESTRICTION))
		set |= SUBSET_RESTRICTION;


	    if (xmlSchemaCheckCOSDerivedOK(ACTXT_CAST vctxt, *localType,
		elemDecl->subtypes, set) != 0) {
		xmlChar *str = NULL;

		xmlSchemaCustomErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_CVC_ELT_4_3, NULL, NULL,
		    "The type definition '%s', specified by xsi:type, is "
		    "blocked or not validly derived from the type definition "
		    "of the element declaration",
		    xmlSchemaFormatQName(&str,
			(*localType)->targetNamespace,
			(*localType)->name),
		    NULL);
		FREE_AND_NULL(str);
		ret = vctxt->err;
		*localType = NULL;
	    }
	}
    }
exit:
    ACTIVATE_ELEM;
    return (ret);
internal_error:
    ACTIVATE_ELEM;
    return (-1);
}

static int
xmlSchemaValidateElemDecl(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaElementPtr elemDecl = vctxt->inode->decl;
    xmlSchemaTypePtr actualType;

    if (elemDecl == NULL) {
	VERROR(XML_SCHEMAV_CVC_ELT_1, NULL,
	    "No matching declaration available");
        return (vctxt->err);
    }
    actualType = WXS_ELEM_TYPEDEF(elemDecl);
    if (elemDecl->flags & XML_SCHEMAS_ELEM_ABSTRACT) {
	VERROR(XML_SCHEMAV_CVC_ELT_2, NULL,
	    "The element declaration is abstract");
        return (vctxt->err);
    }
    if (actualType == NULL) {
    	VERROR(XML_SCHEMAV_CVC_TYPE_1, NULL,
    	    "The type definition is absent");
    	return (XML_SCHEMAV_CVC_TYPE_1);
    }
    if (vctxt->nbAttrInfos != 0) {
	int ret;
	xmlSchemaAttrInfoPtr iattr;
	iattr = xmlSchemaGetMetaAttrInfo(vctxt,
	    XML_SCHEMA_ATTR_INFO_META_XSI_NIL);
	if (iattr) {
	    ACTIVATE_ATTRIBUTE(iattr);
	    ret = xmlSchemaVCheckCVCSimpleType(
		ACTXT_CAST vctxt, NULL,
		xmlSchemaGetBuiltInType(XML_SCHEMAS_BOOLEAN),
		iattr->value, &(iattr->val), 1, 0, 0);
	    ACTIVATE_ELEM;
	    if (ret < 0) {
		VERROR_INT("xmlSchemaValidateElemDecl",
		    "calling xmlSchemaVCheckCVCSimpleType() to "
		    "validate the attribute 'xsi:nil'");
		return (-1);
	    }
	    if (ret == 0) {
		if ((elemDecl->flags & XML_SCHEMAS_ELEM_NILLABLE) == 0) {
		    VERROR(XML_SCHEMAV_CVC_ELT_3_1, NULL,
			"The element is not 'nillable'");
		    
		} else {
		    if (xmlSchemaValueGetAsBoolean(iattr->val)) {
			if ((elemDecl->flags & XML_SCHEMAS_ELEM_FIXED) &&
			    (elemDecl->value != NULL)) {
			    VERROR(XML_SCHEMAV_CVC_ELT_3_2_2, NULL,
				"The element cannot be 'nilled' because "
				"there is a fixed value constraint defined "
				"for it");
			     
			} else
			    vctxt->inode->flags |=
				XML_SCHEMA_ELEM_INFO_NILLED;
		    }
		}
	    }
	}
	iattr = xmlSchemaGetMetaAttrInfo(vctxt,
	    XML_SCHEMA_ATTR_INFO_META_XSI_TYPE);
	if (iattr) {
	    xmlSchemaTypePtr localType = NULL;

	    ret = xmlSchemaProcessXSIType(vctxt, iattr, &localType,
		elemDecl);
	    if (ret != 0) {
		if (ret == -1) {
		    VERROR_INT("xmlSchemaValidateElemDecl",
			"calling xmlSchemaProcessXSIType() to "
			"process the attribute 'xsi:type'");
		    return (-1);
		}
		
	    }
	    if (localType != NULL) {
		vctxt->inode->flags |= XML_SCHEMA_ELEM_INFO_LOCAL_TYPE;
		actualType = localType;
	    }
	}
    }
    if ((elemDecl->idcs != NULL) &&
	(xmlSchemaIDCRegisterMatchers(vctxt, elemDecl) == -1))
	    return (-1);
    if (actualType == NULL) {
    	VERROR(XML_SCHEMAV_CVC_TYPE_1, NULL,
    	    "The type definition is absent");
    	return (XML_SCHEMAV_CVC_TYPE_1);
    }
    vctxt->inode->typeDef = actualType;

    return (0);
}

static int
xmlSchemaVAttributesSimple(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaAttrInfoPtr iattr;
    int ret = 0, i;

    if (vctxt->nbAttrInfos == 0)
	return (0);
    for (i = 0; i < vctxt->nbAttrInfos; i++) {
	iattr = vctxt->attrInfos[i];
	if (! iattr->metaType) {
	    ACTIVATE_ATTRIBUTE(iattr)
	    xmlSchemaIllegalAttrErr(ACTXT_CAST vctxt,
		XML_SCHEMAV_CVC_TYPE_3_1_1, iattr, NULL);
	    ret = XML_SCHEMAV_CVC_TYPE_3_1_1;
        }
    }
    ACTIVATE_ELEM
    return (ret);
}

static void
xmlSchemaClearAttrInfos(xmlSchemaValidCtxtPtr vctxt)
{
    int i;
    xmlSchemaAttrInfoPtr attr;

    if (vctxt->nbAttrInfos == 0)
	return;
    for (i = 0; i < vctxt->nbAttrInfos; i++) {
	attr = vctxt->attrInfos[i];
	if (attr->flags & XML_SCHEMA_NODE_INFO_FLAG_OWNED_NAMES) {
	    if (attr->localName != NULL)
		xmlFree((xmlChar *) attr->localName);
	    if (attr->nsName != NULL)
		xmlFree((xmlChar *) attr->nsName);
	}
	if (attr->flags & XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES) {
	    if (attr->value != NULL)
		xmlFree((xmlChar *) attr->value);
	}
	if (attr->val != NULL) {
	    xmlSchemaFreeValue(attr->val);
	    attr->val = NULL;
	}
	memset(attr, 0, sizeof(xmlSchemaAttrInfo));
    }
    vctxt->nbAttrInfos = 0;
}

static int
xmlSchemaVAttributesComplex(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaTypePtr type = vctxt->inode->typeDef;
    xmlSchemaItemListPtr attrUseList;
    xmlSchemaAttributeUsePtr attrUse = NULL;
    xmlSchemaAttributePtr attrDecl = NULL;
    xmlSchemaAttrInfoPtr iattr, tmpiattr;
    int i, j, found, nbAttrs, nbUses;
    int xpathRes = 0, res, wildIDs = 0, fixed;
    xmlNodePtr defAttrOwnerElem = NULL;

    attrUseList = (xmlSchemaItemListPtr) type->attrUses;
    nbAttrs = vctxt->nbAttrInfos;
    if (attrUseList != NULL)
	nbUses = attrUseList->nbItems;
    else
	nbUses = 0;
    for (i = 0; i < nbUses; i++) {
        found = 0;
	attrUse = attrUseList->items[i];
	attrDecl = WXS_ATTRUSE_DECL(attrUse);
        for (j = 0; j < nbAttrs; j++) {
	    iattr = vctxt->attrInfos[j];
	    if (iattr->metaType)
		continue;
	    if (iattr->localName[0] != attrDecl->name[0])
		continue;
	    if (!xmlStrEqual(iattr->localName, attrDecl->name))
		continue;
	    if (!xmlStrEqual(iattr->nsName, attrDecl->targetNamespace))
		continue;
	    found = 1;
	    iattr->state = XML_SCHEMAS_ATTR_ASSESSED;
	    iattr->use = attrUse;
	    iattr->decl = attrDecl;
	    iattr->typeDef = attrDecl->subtypes;
	    break;
	}

	if (found)
	    continue;

	if (attrUse->occurs == XML_SCHEMAS_ATTR_USE_REQUIRED) {
	    tmpiattr = xmlSchemaGetFreshAttrInfo(vctxt);
	    if (tmpiattr == NULL) {
		VERROR_INT(
		    "xmlSchemaVAttributesComplex",
		    "calling xmlSchemaGetFreshAttrInfo()");
		return (-1);
	    }
	    tmpiattr->state = XML_SCHEMAS_ATTR_ERR_MISSING;
	    tmpiattr->use = attrUse;
	    tmpiattr->decl = attrDecl;
	} else if ((attrUse->occurs == XML_SCHEMAS_ATTR_USE_OPTIONAL) &&
	    ((attrUse->defValue != NULL) ||
	     (attrDecl->defValue != NULL))) {
	    tmpiattr = xmlSchemaGetFreshAttrInfo(vctxt);
	    if (tmpiattr == NULL) {
		VERROR_INT(
		    "xmlSchemaVAttributesComplex",
		    "calling xmlSchemaGetFreshAttrInfo()");
		return (-1);
	    }
	    tmpiattr->state = XML_SCHEMAS_ATTR_DEFAULT;
	    tmpiattr->use = attrUse;
	    tmpiattr->decl = attrDecl;
	    tmpiattr->typeDef = attrDecl->subtypes;
	    tmpiattr->localName = attrDecl->name;
	    tmpiattr->nsName = attrDecl->targetNamespace;
	}
    }

    if (vctxt->nbAttrInfos == 0)
	return (0);
    if (type->attributeWildcard != NULL) {
	for (i = 0; i < nbAttrs; i++) {
	    iattr = vctxt->attrInfos[i];
	    if (iattr->state != XML_SCHEMAS_ATTR_UNKNOWN)
		continue;
	    if (xmlSchemaCheckCVCWildcardNamespace(type->attributeWildcard,
		    iattr->nsName) == 0) {
		if (type->attributeWildcard->processContents ==
		    XML_SCHEMAS_ANY_SKIP) {
		    iattr->state = XML_SCHEMAS_ATTR_WILD_SKIP;
		    continue;
		}
		iattr->decl = xmlSchemaGetAttributeDecl(vctxt->schema,
		    iattr->localName, iattr->nsName);
		if (iattr->decl != NULL) {
		    iattr->state = XML_SCHEMAS_ATTR_ASSESSED;
		    iattr->typeDef = WXS_ATTR_TYPEDEF(iattr->decl);
		    if (xmlSchemaIsDerivedFromBuiltInType(
			iattr->typeDef, XML_SCHEMAS_ID)) {
			if (wildIDs != 0) {
			    
			    iattr->state = XML_SCHEMAS_ATTR_ERR_WILD_DUPLICATE_ID;
			    TODO
			    continue;
			}
			wildIDs++;
                        if (attrUseList != NULL) {
                            for (j = 0; j < attrUseList->nbItems; j++) {
                                if (xmlSchemaIsDerivedFromBuiltInType(
                                    WXS_ATTRUSE_TYPEDEF(attrUseList->items[j]),
                                    XML_SCHEMAS_ID)) {
                                    
                            iattr->state = XML_SCHEMAS_ATTR_ERR_WILD_AND_USE_ID;
                                    TODO
                                    break;
                                }
                            }
                        }
		    }
		} else if (type->attributeWildcard->processContents ==
		    XML_SCHEMAS_ANY_LAX) {
		    iattr->state = XML_SCHEMAS_ATTR_WILD_LAX_NO_DECL;
		} else {
		    iattr->state = XML_SCHEMAS_ATTR_ERR_WILD_STRICT_NO_DECL;
		}
	    }
	}
    }

    if (vctxt->nbAttrInfos == 0)
	return (0);

    if (vctxt->options & XML_SCHEMA_VAL_VC_I_CREATE) {
	xmlSchemaNodeInfoPtr ielem = vctxt->elemInfos[vctxt->depth];
	if (ielem && ielem->node && ielem->node->doc)
	    defAttrOwnerElem = ielem->node;
    }
    for (i = 0; i < vctxt->nbAttrInfos; i++) {
	iattr = vctxt->attrInfos[i];
	if ((iattr->state != XML_SCHEMAS_ATTR_ASSESSED) &&
	    (iattr->state != XML_SCHEMAS_ATTR_DEFAULT))
	    continue;
	if (iattr->typeDef == NULL) {
	    iattr->state = XML_SCHEMAS_ATTR_ERR_NO_TYPE;
	    continue;
	}

	ACTIVATE_ATTRIBUTE(iattr);
	fixed = 0;
	xpathRes = 0;

	if (vctxt->xpathStates != NULL) {
	    xpathRes = xmlSchemaXPathEvaluate(vctxt,
		XML_ATTRIBUTE_NODE);
	    if (xpathRes == -1) {
		VERROR_INT("xmlSchemaVAttributesComplex",
		    "calling xmlSchemaXPathEvaluate()");
		goto internal_error;
	    }
	}

	if (iattr->state == XML_SCHEMAS_ATTR_DEFAULT) {
	    if ((xpathRes) || (defAttrOwnerElem)) {
		if (iattr->use->defValue != NULL) {
		    iattr->value = (xmlChar *) iattr->use->defValue;
		    iattr->val = iattr->use->defVal;
		} else {
		    iattr->value = (xmlChar *) iattr->decl->defValue;
		    iattr->val = iattr->decl->defVal;
		}
		if (iattr->val == NULL) {
		    VERROR_INT("xmlSchemaVAttributesComplex",
			"default/fixed value on an attribute use was "
			"not precomputed");
		    goto internal_error;
		}
		iattr->val = xmlSchemaCopyValue(iattr->val);
		if (iattr->val == NULL) {
		    VERROR_INT("xmlSchemaVAttributesComplex",
			"calling xmlSchemaCopyValue()");
		    goto internal_error;
		}
	    }

	    if (defAttrOwnerElem) {
		xmlChar *normValue;
		const xmlChar *value;

		value = iattr->value;
		normValue = xmlSchemaNormalizeValue(iattr->typeDef,
		    iattr->value);
		if (normValue != NULL)
		    value = BAD_CAST normValue;

		if (iattr->nsName == NULL) {
		    if (xmlNewProp(defAttrOwnerElem,
			iattr->localName, value) == NULL) {
			VERROR_INT("xmlSchemaVAttributesComplex",
			    "callling xmlNewProp()");
			if (normValue != NULL)
			    xmlFree(normValue);
			goto internal_error;
		    }
		} else {
		    xmlNsPtr ns;

		    ns = xmlSearchNsByHref(defAttrOwnerElem->doc,
			defAttrOwnerElem, iattr->nsName);
		    if (ns == NULL) {
			xmlChar prefix[12];
			int counter = 0;

			do {
			    snprintf((char *) prefix, 12, "p%d", counter++);
			    ns = xmlSearchNs(defAttrOwnerElem->doc,
				defAttrOwnerElem, BAD_CAST prefix);
			    if (counter > 1000) {
				VERROR_INT(
				    "xmlSchemaVAttributesComplex",
				    "could not compute a ns prefix for a "
				    "default/fixed attribute");
				if (normValue != NULL)
				    xmlFree(normValue);
				goto internal_error;
			    }
			} while (ns != NULL);
			ns = xmlNewNs(vctxt->validationRoot,
			    iattr->nsName, BAD_CAST prefix);
		    }
		    xmlNewNsProp(defAttrOwnerElem, ns, iattr->localName, value);
		}
		if (normValue != NULL)
		    xmlFree(normValue);
	    }
	    goto eval_idcs;
	}
	if (vctxt->value != NULL) {
	    xmlSchemaFreeValue(vctxt->value);
	    vctxt->value = NULL;
	}
	if ((iattr->decl->flags & XML_SCHEMAS_ATTR_FIXED) ||
	    ((iattr->use != NULL) &&
	     (iattr->use->flags & XML_SCHEMAS_ATTR_FIXED)))
	    fixed = 1;
	else
	    fixed = 0;
	if (xpathRes || fixed) {
	    iattr->flags |= XML_SCHEMA_NODE_INFO_VALUE_NEEDED;
	    res = xmlSchemaVCheckCVCSimpleType(
		ACTXT_CAST vctxt,
		iattr->node, iattr->typeDef, iattr->value, &(iattr->val),
		1, 1, 0);
	} else {
	    res = xmlSchemaVCheckCVCSimpleType(
		ACTXT_CAST vctxt,
		iattr->node, iattr->typeDef, iattr->value, NULL,
		1, 0, 0);
	}

	if (res != 0) {
	    if (res == -1) {
		VERROR_INT("xmlSchemaVAttributesComplex",
		    "calling xmlSchemaStreamValidateSimpleTypeValue()");
		goto internal_error;
	    }
	    iattr->state = XML_SCHEMAS_ATTR_INVALID_VALUE;
	    goto eval_idcs;
	}

	if (fixed) {
	    if (iattr->val == NULL) {
		
		TODO
		goto eval_idcs;
	    }
	    if ((iattr->use != NULL) &&
		(iattr->use->defValue != NULL)) {
		if (iattr->use->defVal == NULL) {
		    
		    TODO
		    goto eval_idcs;
		}
		iattr->vcValue = iattr->use->defValue;
		if (! xmlSchemaAreValuesEqual(iattr->val, iattr->use->defVal))
		    iattr->state = XML_SCHEMAS_ATTR_ERR_FIXED_VALUE;
	    } else {
		if (iattr->decl->defVal == NULL) {
		    
		    TODO
		    goto eval_idcs;
		}
		iattr->vcValue = iattr->decl->defValue;
		if (! xmlSchemaAreValuesEqual(iattr->val, iattr->decl->defVal))
		    iattr->state = XML_SCHEMAS_ATTR_ERR_FIXED_VALUE;
	    }
	}
eval_idcs:
	if (xpathRes) {
	    if (xmlSchemaXPathProcessHistory(vctxt,
		vctxt->depth +1) == -1) {
		VERROR_INT("xmlSchemaVAttributesComplex",
		    "calling xmlSchemaXPathEvaluate()");
		goto internal_error;
	    }
	} else if (vctxt->xpathStates != NULL)
	    xmlSchemaXPathPop(vctxt);
    }

    for (i = 0; i < vctxt->nbAttrInfos; i++) {
	iattr = vctxt->attrInfos[i];
	if ((iattr->state == XML_SCHEMAS_ATTR_META) ||
	    (iattr->state == XML_SCHEMAS_ATTR_ASSESSED) ||
	    (iattr->state == XML_SCHEMAS_ATTR_WILD_SKIP) ||
	    (iattr->state == XML_SCHEMAS_ATTR_WILD_LAX_NO_DECL))
	    continue;
	ACTIVATE_ATTRIBUTE(iattr);
	switch (iattr->state) {
	    case XML_SCHEMAS_ATTR_ERR_MISSING: {
		    xmlChar *str = NULL;
		    ACTIVATE_ELEM;
		    xmlSchemaCustomErr(ACTXT_CAST vctxt,
			XML_SCHEMAV_CVC_COMPLEX_TYPE_4, NULL, NULL,
			"The attribute '%s' is required but missing",
			xmlSchemaFormatQName(&str,
			    iattr->decl->targetNamespace,
			    iattr->decl->name),
			NULL);
		    FREE_AND_NULL(str)
		    break;
		}
	    case XML_SCHEMAS_ATTR_ERR_NO_TYPE:
		VERROR(XML_SCHEMAV_CVC_ATTRIBUTE_2, NULL,
		    "The type definition is absent");
		break;
	    case XML_SCHEMAS_ATTR_ERR_FIXED_VALUE:
		xmlSchemaCustomErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_CVC_AU, NULL, NULL,
		    "The value '%s' does not match the fixed "
		    "value constraint '%s'",
		    iattr->value, iattr->vcValue);
		break;
	    case XML_SCHEMAS_ATTR_ERR_WILD_STRICT_NO_DECL:
		VERROR(XML_SCHEMAV_CVC_WILDCARD, NULL,
		    "No matching global attribute declaration available, but "
		    "demanded by the strict wildcard");
		break;
	    case XML_SCHEMAS_ATTR_UNKNOWN:
		if (iattr->metaType)
		    break;
		if (type->attributeWildcard == NULL) {
		    xmlSchemaIllegalAttrErr(ACTXT_CAST vctxt,
			XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_1, iattr, NULL);
		} else {
		    xmlSchemaIllegalAttrErr(ACTXT_CAST vctxt,
			XML_SCHEMAV_CVC_COMPLEX_TYPE_3_2_2, iattr, NULL);
		}
		break;
	    default:
		break;
	}
    }

    ACTIVATE_ELEM;
    return (0);
internal_error:
    ACTIVATE_ELEM;
    return (-1);
}

static int
xmlSchemaValidateElemWildcard(xmlSchemaValidCtxtPtr vctxt,
			      int *skip)
{
    xmlSchemaWildcardPtr wild = (xmlSchemaWildcardPtr) vctxt->inode->decl;
    if ((skip == NULL) || (wild == NULL) ||
	(wild->type != XML_SCHEMA_TYPE_ANY)) {
	VERROR_INT("xmlSchemaValidateElemWildcard",
	    "bad arguments");
	return (-1);
    }
    *skip = 0;
    if (wild->processContents == XML_SCHEMAS_ANY_SKIP) {
	*skip = 1;
	return (0);
    }
    {
	xmlSchemaElementPtr decl = NULL;

	decl = xmlSchemaGetElem(vctxt->schema,
	    vctxt->inode->localName, vctxt->inode->nsName);
	if (decl != NULL) {
	    vctxt->inode->decl = decl;
	    return (0);
	}
    }
    if (wild->processContents == XML_SCHEMAS_ANY_STRICT) {
	
	VERROR(XML_SCHEMAV_CVC_ELT_1, NULL, 
	    "No matching global element declaration available, but "
	    "demanded by the strict wildcard");
	return (vctxt->err);
    }
    if (vctxt->nbAttrInfos != 0) {
	xmlSchemaAttrInfoPtr iattr;
	iattr = xmlSchemaGetMetaAttrInfo(vctxt,
	    XML_SCHEMA_ATTR_INFO_META_XSI_TYPE);
	if (iattr != NULL) {
	    if (xmlSchemaProcessXSIType(vctxt, iattr,
		&(vctxt->inode->typeDef), NULL) == -1) {
		VERROR_INT("xmlSchemaValidateElemWildcard",
		    "calling xmlSchemaProcessXSIType() to "
		    "process the attribute 'xsi:nil'");
		return (-1);
	    }
	    return (0);
	}
    }
    vctxt->inode->typeDef =
	xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
    return (0);
}


static int
xmlSchemaCheckCOSValidDefault(xmlSchemaValidCtxtPtr vctxt,
			      const xmlChar *value,
			      xmlSchemaValPtr *val)
{
    int ret = 0;
    xmlSchemaNodeInfoPtr inode = vctxt->inode;

    if WXS_IS_COMPLEX(inode->typeDef) {
	if ((! WXS_HAS_SIMPLE_CONTENT(inode->typeDef)) &&
	    ((! WXS_HAS_MIXED_CONTENT(inode->typeDef)) ||
	     (! WXS_EMPTIABLE(inode->typeDef)))) {
	    ret = XML_SCHEMAP_COS_VALID_DEFAULT_2_1;
	    
	    VERROR(ret, NULL,
		"For a string to be a valid default, the type definition "
		"must be a simple type or a complex type with simple content "
		"or mixed content and a particle emptiable");
	    return(ret);
	}
    }
    if (WXS_IS_SIMPLE(inode->typeDef)) {

	ret = xmlSchemaVCheckCVCSimpleType(ACTXT_CAST vctxt,
	    NULL, inode->typeDef, value, val, 1, 1, 0);

    } else if (WXS_HAS_SIMPLE_CONTENT(inode->typeDef)) {

	ret = xmlSchemaVCheckCVCSimpleType(ACTXT_CAST vctxt,
	    NULL, inode->typeDef->contentTypeDef, value, val, 1, 1, 0);
    }
    if (ret < 0) {
	VERROR_INT("xmlSchemaCheckCOSValidDefault",
	    "calling xmlSchemaVCheckCVCSimpleType()");
    }
    return (ret);
}

static void
xmlSchemaVContentModelCallback(xmlSchemaValidCtxtPtr vctxt ATTRIBUTE_UNUSED,
			       const xmlChar * name ATTRIBUTE_UNUSED,
			       xmlSchemaElementPtr item,
			       xmlSchemaNodeInfoPtr inode)
{
    inode->decl = item;
#ifdef DEBUG_CONTENT
    {
	xmlChar *str = NULL;

	if (item->type == XML_SCHEMA_TYPE_ELEMENT) {
	    xmlGenericError(xmlGenericErrorContext,
		"AUTOMATON callback for '%s' [declaration]\n",
		xmlSchemaFormatQName(&str,
		inode->localName, inode->nsName));
	} else {
	    xmlGenericError(xmlGenericErrorContext,
		    "AUTOMATON callback for '%s' [wildcard]\n",
		    xmlSchemaFormatQName(&str,
		    inode->localName, inode->nsName));

	}
	FREE_AND_NULL(str)
    }
#endif
}

static int
xmlSchemaValidatorPushElem(xmlSchemaValidCtxtPtr vctxt)
{
    vctxt->inode = xmlSchemaGetFreshElemInfo(vctxt);
    if (vctxt->inode == NULL) {
	VERROR_INT("xmlSchemaValidatorPushElem",
	    "calling xmlSchemaGetFreshElemInfo()");
	return (-1);
    }
    vctxt->nbAttrInfos = 0;
    return (0);
}

static int
xmlSchemaVCheckINodeDataType(xmlSchemaValidCtxtPtr vctxt,
			     xmlSchemaNodeInfoPtr inode,
			     xmlSchemaTypePtr type,
			     const xmlChar *value)
{
    if (inode->flags & XML_SCHEMA_NODE_INFO_VALUE_NEEDED)
	return (xmlSchemaVCheckCVCSimpleType(
	    ACTXT_CAST vctxt, NULL,
	    type, value, &(inode->val), 1, 1, 0));
    else
	return (xmlSchemaVCheckCVCSimpleType(
	    ACTXT_CAST vctxt, NULL,
	    type, value, NULL, 1, 0, 0));
}



static int
xmlSchemaValidatorPopElem(xmlSchemaValidCtxtPtr vctxt)
{
    int ret = 0;
    xmlSchemaNodeInfoPtr inode = vctxt->inode;

    if (vctxt->nbAttrInfos != 0)
	xmlSchemaClearAttrInfos(vctxt);
    if (inode->flags & XML_SCHEMA_NODE_INFO_ERR_NOT_EXPECTED) {
	vctxt->skipDepth = vctxt->depth -1;
	goto end_elem;
    }
    if ((inode->typeDef == NULL) ||
	(inode->flags & XML_SCHEMA_NODE_INFO_ERR_BAD_TYPE)) {
	goto end_elem;
    }
    if ((inode->typeDef->contentType == XML_SCHEMA_CONTENT_MIXED) ||
	(inode->typeDef->contentType == XML_SCHEMA_CONTENT_ELEMENTS)) {

	if (inode->typeDef->builtInType == XML_SCHEMAS_ANYTYPE)
	    goto character_content;

	if ((inode->flags & XML_SCHEMA_ELEM_INFO_ERR_BAD_CONTENT) == 0) {
	    xmlChar *values[10];
	    int terminal, nbval = 10, nbneg;

	    if (inode->regexCtxt == NULL) {
		inode->regexCtxt =
		    xmlRegNewExecCtxt(inode->typeDef->contModel,
		    (xmlRegExecCallbacks) xmlSchemaVContentModelCallback,
		    vctxt);
		if (inode->regexCtxt == NULL) {
		    VERROR_INT("xmlSchemaValidatorPopElem",
			"failed to create a regex context");
		    goto internal_error;
		}
#ifdef DEBUG_AUTOMATA
		xmlGenericError(xmlGenericErrorContext,
		    "AUTOMATON create on '%s'\n", inode->localName);
#endif
	    }
	    xmlRegExecNextValues(inode->regexCtxt,
		&nbval, &nbneg, &values[0], &terminal);
	    ret = xmlRegExecPushString(inode->regexCtxt, NULL, NULL);
	    if ((ret<0) || ((ret==0) && (!INODE_NILLED(inode)))) {
		ret = 1;
		inode->flags |=
		    XML_SCHEMA_ELEM_INFO_ERR_BAD_CONTENT;
		xmlSchemaComplexTypeErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_ELEMENT_CONTENT, NULL, NULL,
		    "Missing child element(s)",
		    nbval, nbneg, values);
#ifdef DEBUG_AUTOMATA
		xmlGenericError(xmlGenericErrorContext,
		    "AUTOMATON missing ERROR on '%s'\n",
		    inode->localName);
#endif
	    } else {
		ret = 0;
#ifdef DEBUG_AUTOMATA
		xmlGenericError(xmlGenericErrorContext,
		    "AUTOMATON succeeded on '%s'\n",
		    inode->localName);
#endif
	    }

	}
    }
    if (inode->typeDef->contentType == XML_SCHEMA_CONTENT_ELEMENTS)
	goto end_elem;

character_content:

    if (vctxt->value != NULL) {
	xmlSchemaFreeValue(vctxt->value);
	vctxt->value = NULL;
    }
    if (inode->decl == NULL) {
	if (WXS_IS_SIMPLE(inode->typeDef)) {
	    ret = xmlSchemaVCheckINodeDataType(vctxt,
		inode, inode->typeDef, inode->value);
	} else if (WXS_HAS_SIMPLE_CONTENT(inode->typeDef)) {
	    ret = xmlSchemaVCheckINodeDataType(vctxt,
		inode, inode->typeDef->contentTypeDef,
		inode->value);
	}
	if (ret < 0) {
	    VERROR_INT("xmlSchemaValidatorPopElem",
		"calling xmlSchemaVCheckCVCSimpleType()");
	    goto internal_error;
	}
	goto end_elem;
    }
    if ((inode->decl->value != NULL) &&
	(inode->flags & XML_SCHEMA_ELEM_INFO_EMPTY) &&
	(! INODE_NILLED(inode))) {
	if (inode->flags & XML_SCHEMA_ELEM_INFO_LOCAL_TYPE) {

	    ret = xmlSchemaCheckCOSValidDefault(vctxt,
		inode->decl->value, &(inode->val));
	    if (ret != 0) {
		if (ret < 0) {
		    VERROR_INT("xmlSchemaValidatorPopElem",
			"calling xmlSchemaCheckCOSValidDefault()");
		    goto internal_error;
		}
		goto end_elem;
	    }
	    goto default_psvi;
	}
	if (WXS_IS_SIMPLE(inode->typeDef)) {
	    ret = xmlSchemaVCheckINodeDataType(vctxt,
		inode, inode->typeDef, inode->decl->value);
	} else if (WXS_HAS_SIMPLE_CONTENT(inode->typeDef)) {
	    ret = xmlSchemaVCheckINodeDataType(vctxt,
		inode, inode->typeDef->contentTypeDef,
		inode->decl->value);
	}
	if (ret != 0) {
	    if (ret < 0) {
		VERROR_INT("xmlSchemaValidatorPopElem",
		    "calling xmlSchemaVCheckCVCSimpleType()");
		goto internal_error;
	    }
	    goto end_elem;
	}

default_psvi:
	if ((vctxt->options & XML_SCHEMA_VAL_VC_I_CREATE) &&
	    (inode->node != NULL)) {
	    xmlNodePtr textChild;
	    xmlChar *normValue;
	    normValue = xmlSchemaNormalizeValue(inode->typeDef,
		inode->decl->value);
	    if (normValue != NULL) {
		textChild = xmlNewText(BAD_CAST normValue);
		xmlFree(normValue);
	    } else
		textChild = xmlNewText(inode->decl->value);
	    if (textChild == NULL) {
		VERROR_INT("xmlSchemaValidatorPopElem",
		    "calling xmlNewText()");
		goto internal_error;
	    } else
		xmlAddChild(inode->node, textChild);
	}

    } else if (! INODE_NILLED(inode)) {
	if (WXS_IS_SIMPLE(inode->typeDef)) {
	    ret = xmlSchemaVCheckINodeDataType(vctxt,
		    inode, inode->typeDef, inode->value);
	} else if (WXS_HAS_SIMPLE_CONTENT(inode->typeDef)) {
	    ret = xmlSchemaVCheckINodeDataType(vctxt,
		inode, inode->typeDef->contentTypeDef, inode->value);
	}
	if (ret != 0) {
	    if (ret < 0) {
		VERROR_INT("xmlSchemaValidatorPopElem",
		    "calling xmlSchemaVCheckCVCSimpleType()");
		goto internal_error;
	    }
	    goto end_elem;
	}
	if ((inode->decl->value != NULL) &&
	    (inode->decl->flags & XML_SCHEMAS_ELEM_FIXED)) {

	    if (inode->flags &
		    XML_SCHEMA_ELEM_INFO_HAS_ELEM_CONTENT) {
		ret = XML_SCHEMAV_CVC_ELT_5_2_2_1;
		VERROR(ret, NULL,
		    "The content must not containt element nodes since "
		    "there is a fixed value constraint");
		goto end_elem;
	    } else {
		if (WXS_HAS_MIXED_CONTENT(inode->typeDef)) {
		    if (! xmlStrEqual(inode->value, inode->decl->value)){
			ret = XML_SCHEMAV_CVC_ELT_5_2_2_2_1;
			xmlSchemaCustomErr(ACTXT_CAST vctxt,
			    ret, NULL, NULL,
			    "The initial value '%s' does not match the fixed "
			    "value constraint '%s'",
			    inode->value, inode->decl->value);
			goto end_elem;
		    }
		} else if (WXS_HAS_SIMPLE_CONTENT(inode->typeDef)) {
		    if (! xmlStrEqual(inode->value,
			    inode->decl->value)) {
			ret = XML_SCHEMAV_CVC_ELT_5_2_2_2_2;
			xmlSchemaCustomErr(ACTXT_CAST vctxt,
			    ret, NULL, NULL,
			    "The actual value '%s' does not match the fixed "
			    "value constraint '%s'",
			    inode->value,
			    inode->decl->value);
			goto end_elem;
		    }
		}
	    }
	}
    }

end_elem:
    if (vctxt->depth < 0) {
	
	return (0);
    }
    if (vctxt->depth == vctxt->skipDepth)
	vctxt->skipDepth = -1;
    if (inode->appliedXPath &&
	(xmlSchemaXPathProcessHistory(vctxt, vctxt->depth) == -1))
	goto internal_error;

    if ((inode->idcMatchers != NULL) &&
	(vctxt->hasKeyrefs || vctxt->createIDCNodeTables))
    {
	if (xmlSchemaIDCFillNodeTables(vctxt, inode) == -1)
	    goto internal_error;
    }
    if (vctxt->inode->hasKeyrefs)
	if (xmlSchemaCheckCVCIDCKeyRef(vctxt) == -1)
	    goto internal_error;
    if (inode->idcTable != NULL) {
#ifdef DEBUG_IDC_NODE_TABLE
	xmlSchemaDebugDumpIDCTable(stdout,
	    inode->nsName,
	    inode->localName,
	    inode->idcTable);
#endif
	if ((vctxt->depth > 0) &&
	    (vctxt->hasKeyrefs || vctxt->createIDCNodeTables))
	{
	    if (xmlSchemaBubbleIDCNodeTables(vctxt) == -1)
		goto internal_error;
	}
    }
    xmlSchemaClearElemInfo(vctxt, inode);
    if (vctxt->depth == 0) {
	vctxt->depth--;
	vctxt->inode = NULL;
	return (0);
    }
    if (vctxt->aidcs != NULL) {
	xmlSchemaIDCAugPtr aidc = vctxt->aidcs;
	do {
	    if (aidc->keyrefDepth == vctxt->depth) {
		aidc->keyrefDepth = -1;
	    }
	    aidc = aidc->next;
	} while (aidc != NULL);
    }
    vctxt->depth--;
    vctxt->inode = vctxt->elemInfos[vctxt->depth];
    return (ret);

internal_error:
    vctxt->err = -1;
    return (-1);
}

static int
xmlSchemaValidateChildElem(xmlSchemaValidCtxtPtr vctxt)
{
    xmlSchemaNodeInfoPtr pielem;
    xmlSchemaTypePtr ptype;
    int ret = 0;

    if (vctxt->depth <= 0) {
	VERROR_INT("xmlSchemaValidateChildElem",
	    "not intended for the validation root");
	return (-1);
    }
    pielem = vctxt->elemInfos[vctxt->depth -1];
    if (pielem->flags & XML_SCHEMA_ELEM_INFO_EMPTY)
	pielem->flags ^= XML_SCHEMA_ELEM_INFO_EMPTY;
    if (INODE_NILLED(pielem)) {
	ACTIVATE_PARENT_ELEM;
	ret = XML_SCHEMAV_CVC_ELT_3_2_1;
	VERROR(ret, NULL,
	    "Neither character nor element content is allowed, "
	    "because the element was 'nilled'");
	ACTIVATE_ELEM;
	goto unexpected_elem;
    }

    ptype = pielem->typeDef;

    if (ptype->builtInType == XML_SCHEMAS_ANYTYPE) {
	vctxt->inode->decl = xmlSchemaGetElem(vctxt->schema,
	    vctxt->inode->localName,
	    vctxt->inode->nsName);

	if (vctxt->inode->decl == NULL) {
	    xmlSchemaAttrInfoPtr iattr;
	    iattr = xmlSchemaGetMetaAttrInfo(vctxt,
		XML_SCHEMA_ATTR_INFO_META_XSI_TYPE);
	    if (iattr != NULL) {
		ret = xmlSchemaProcessXSIType(vctxt, iattr,
		    &(vctxt->inode->typeDef), NULL);
		if (ret != 0) {
		    if (ret == -1) {
			VERROR_INT("xmlSchemaValidateChildElem",
			    "calling xmlSchemaProcessXSIType() to "
			    "process the attribute 'xsi:nil'");
			return (-1);
		    }
		    return (ret);
		}
	    } else {
		vctxt->inode->typeDef =
		    xmlSchemaGetBuiltInType(XML_SCHEMAS_ANYTYPE);
	    }
	}
	return (0);
    }

    switch (ptype->contentType) {
	case XML_SCHEMA_CONTENT_EMPTY:
	    ACTIVATE_PARENT_ELEM
	    ret = XML_SCHEMAV_CVC_COMPLEX_TYPE_2_1;
	    VERROR(ret, NULL,
		"Element content is not allowed, "
		"because the content type is empty");
	    ACTIVATE_ELEM
	    goto unexpected_elem;
	    break;

	case XML_SCHEMA_CONTENT_MIXED:
        case XML_SCHEMA_CONTENT_ELEMENTS: {
	    xmlRegExecCtxtPtr regexCtxt;
	    xmlChar *values[10];
	    int terminal, nbval = 10, nbneg;

	    

	    if (ptype->contModel == NULL) {
		VERROR_INT("xmlSchemaValidateChildElem",
		    "type has elem content but no content model");
		return (-1);
	    }
	    if (pielem->flags & XML_SCHEMA_ELEM_INFO_ERR_BAD_CONTENT) {
		VERROR_INT("xmlSchemaValidateChildElem",
		    "validating elem, but elem content is already invalid");
		return (-1);
	    }

	    regexCtxt = pielem->regexCtxt;
	    if (regexCtxt == NULL) {
		regexCtxt = xmlRegNewExecCtxt(ptype->contModel,
		    (xmlRegExecCallbacks) xmlSchemaVContentModelCallback,
		    vctxt);
		if (regexCtxt == NULL) {
		    VERROR_INT("xmlSchemaValidateChildElem",
			"failed to create a regex context");
		    return (-1);
		}
		pielem->regexCtxt = regexCtxt;
#ifdef DEBUG_AUTOMATA
		xmlGenericError(xmlGenericErrorContext, "AUTOMATA create on '%s'\n",
		    pielem->localName);
#endif
	    }

	    ret = xmlRegExecPushString2(regexCtxt,
		vctxt->inode->localName,
		vctxt->inode->nsName,
		vctxt->inode);
#ifdef DEBUG_AUTOMATA
	    if (ret < 0)
		xmlGenericError(xmlGenericErrorContext,
		"AUTOMATON push ERROR for '%s' on '%s'\n",
		vctxt->inode->localName, pielem->localName);
	    else
		xmlGenericError(xmlGenericErrorContext,
		"AUTOMATON push OK for '%s' on '%s'\n",
		vctxt->inode->localName, pielem->localName);
#endif
	    if (vctxt->err == XML_SCHEMAV_INTERNAL) {
		VERROR_INT("xmlSchemaValidateChildElem",
		    "calling xmlRegExecPushString2()");
		return (-1);
	    }
	    if (ret < 0) {
		xmlRegExecErrInfo(regexCtxt, NULL, &nbval, &nbneg,
		    &values[0], &terminal);
		xmlSchemaComplexTypeErr(ACTXT_CAST vctxt,
		    XML_SCHEMAV_ELEMENT_CONTENT, NULL,NULL,
		    "This element is not expected",
		    nbval, nbneg, values);
		ret = vctxt->err;
		goto unexpected_elem;
	    } else
		ret = 0;
	}
	    break;
	case XML_SCHEMA_CONTENT_SIMPLE:
	case XML_SCHEMA_CONTENT_BASIC:
	    ACTIVATE_PARENT_ELEM
	    if (WXS_IS_COMPLEX(ptype)) {
		ret = XML_SCHEMAV_CVC_COMPLEX_TYPE_2_2;
		VERROR(ret, NULL, "Element content is not allowed, "
		    "because the content type is a simple type definition");
	    } else {
		ret = XML_SCHEMAV_CVC_TYPE_3_1_2;
		VERROR(ret, NULL, "Element content is not allowed, "
		    "because the type definition is simple");
	    }
	    ACTIVATE_ELEM
	    ret = vctxt->err;
	    goto unexpected_elem;
	    break;

	default:
	    break;
    }
    return (ret);
unexpected_elem:
    vctxt->skipDepth = vctxt->depth;
    vctxt->inode->flags |= XML_SCHEMA_NODE_INFO_ERR_NOT_EXPECTED;
    pielem->flags |= XML_SCHEMA_ELEM_INFO_ERR_BAD_CONTENT;
    return (ret);
}

#define XML_SCHEMA_PUSH_TEXT_PERSIST 1
#define XML_SCHEMA_PUSH_TEXT_CREATED 2
#define XML_SCHEMA_PUSH_TEXT_VOLATILE 3

static int
xmlSchemaVPushText(xmlSchemaValidCtxtPtr vctxt,
		  int nodeType, const xmlChar *value, int len,
		  int mode, int *consumed)
{
    if (consumed != NULL)
	*consumed = 0;
    if (INODE_NILLED(vctxt->inode)) {
	VERROR(XML_SCHEMAV_CVC_ELT_3_2_1, NULL,
	    "Neither character nor element content is allowed "
	    "because the element is 'nilled'");
	return (vctxt->err);
    }
    if (vctxt->inode->typeDef->contentType ==
	    XML_SCHEMA_CONTENT_EMPTY) {
	VERROR(XML_SCHEMAV_CVC_COMPLEX_TYPE_2_1, NULL,
	    "Character content is not allowed, "
	    "because the content type is empty");
	return (vctxt->err);
    }

    if (vctxt->inode->typeDef->contentType ==
	    XML_SCHEMA_CONTENT_ELEMENTS) {
	if ((nodeType != XML_TEXT_NODE) ||
	    (! xmlSchemaIsBlank((xmlChar *) value, len))) {
	    VERROR(XML_SCHEMAV_CVC_COMPLEX_TYPE_2_3, NULL,
		"Character content other than whitespace is not allowed "
		"because the content type is 'element-only'");
	    return (vctxt->err);
	}
	return (0);
    }

    if ((value == NULL) || (value[0] == 0))
	return (0);
    if ((vctxt->inode->typeDef->contentType == XML_SCHEMA_CONTENT_MIXED) &&
	((vctxt->inode->decl == NULL) ||
	(vctxt->inode->decl->value == NULL)))
	return (0);

    if (vctxt->inode->value == NULL) {
	switch (mode) {
	    case XML_SCHEMA_PUSH_TEXT_PERSIST:
		vctxt->inode->value = value;
		break;
	    case XML_SCHEMA_PUSH_TEXT_CREATED:
		vctxt->inode->value = value;
		if (consumed != NULL)
		    *consumed = 1;
		vctxt->inode->flags |=
		    XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES;
		break;
	    case XML_SCHEMA_PUSH_TEXT_VOLATILE:
		if (len != -1)
		    vctxt->inode->value = BAD_CAST xmlStrndup(value, len);
		else
		    vctxt->inode->value = BAD_CAST xmlStrdup(value);
		vctxt->inode->flags |=
		    XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES;
		break;
	    default:
		break;
	}
    } else {
	if (len < 0)
	    len = xmlStrlen(value);
	if (vctxt->inode->flags & XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES) {
	    vctxt->inode->value = BAD_CAST xmlStrncat(
		(xmlChar *) vctxt->inode->value, value, len);
	} else {
	    vctxt->inode->value =
		BAD_CAST xmlStrncatNew(vctxt->inode->value, value, len);
	    vctxt->inode->flags |= XML_SCHEMA_NODE_INFO_FLAG_OWNED_VALUES;
	}
    }

    return (0);
}

static int
xmlSchemaValidateElem(xmlSchemaValidCtxtPtr vctxt)
{
    int ret = 0;

    if ((vctxt->skipDepth != -1) &&
	(vctxt->depth >= vctxt->skipDepth)) {
	VERROR_INT("xmlSchemaValidateElem",
	    "in skip-state");
	goto internal_error;
    }
    if (vctxt->xsiAssemble) {
	ret = xmlSchemaAssembleByXSI(vctxt);
	if (ret != 0) {
	    if (ret == -1)
		goto internal_error;
	    vctxt->skipDepth = 0;
	    return(ret);
	}
    }
    if (vctxt->depth > 0) {
	ret = xmlSchemaValidateChildElem(vctxt);
	if (ret != 0) {
	    if (ret < 0) {
		VERROR_INT("xmlSchemaValidateElem",
		    "calling xmlSchemaStreamValidateChildElement()");
		goto internal_error;
	    }
	    goto exit;
	}
	if (vctxt->depth == vctxt->skipDepth)
	    goto exit;
	if ((vctxt->inode->decl == NULL) &&
	    (vctxt->inode->typeDef == NULL)) {
	    VERROR_INT("xmlSchemaValidateElem",
		"the child element was valid but neither the "
		"declaration nor the type was set");
	    goto internal_error;
	}
    } else {
	vctxt->inode->decl = xmlSchemaGetElem(vctxt->schema,
	    vctxt->inode->localName,
	    vctxt->inode->nsName);
	if (vctxt->inode->decl == NULL) {
	    ret = XML_SCHEMAV_CVC_ELT_1;
	    VERROR(ret, NULL,
		"No matching global declaration available "
		"for the validation root");
	    goto exit;
	}
    }

    if (vctxt->inode->decl == NULL)
	goto type_validation;

    if (vctxt->inode->decl->type == XML_SCHEMA_TYPE_ANY) {
	int skip;
	ret = xmlSchemaValidateElemWildcard(vctxt, &skip);
	if (ret != 0) {
	    if (ret < 0) {
		VERROR_INT("xmlSchemaValidateElem",
		    "calling xmlSchemaValidateElemWildcard()");
		goto internal_error;
	    }
	    goto exit;
	}
	if (skip) {
	    vctxt->skipDepth = vctxt->depth;
	    goto exit;
	}
	if (vctxt->inode->decl->type != XML_SCHEMA_TYPE_ELEMENT) {
	    vctxt->inode->decl = NULL;
	    goto type_validation;
	}
    }
    ret = xmlSchemaValidateElemDecl(vctxt);
    if (ret != 0) {
	if (ret < 0) {
	    VERROR_INT("xmlSchemaValidateElem",
		"calling xmlSchemaValidateElemDecl()");
	    goto internal_error;
	}
	goto exit;
    }
type_validation:

    if (vctxt->inode->typeDef == NULL) {
	vctxt->inode->flags |= XML_SCHEMA_NODE_INFO_ERR_BAD_TYPE;
	ret = XML_SCHEMAV_CVC_TYPE_1;
    	VERROR(ret, NULL,
    	    "The type definition is absent");
	goto exit;
    }
    if (vctxt->inode->typeDef->flags & XML_SCHEMAS_TYPE_ABSTRACT) {
	vctxt->inode->flags |= XML_SCHEMA_NODE_INFO_ERR_BAD_TYPE;
	ret = XML_SCHEMAV_CVC_TYPE_2;
    	    VERROR(ret, NULL,
    	    "The type definition is abstract");
	goto exit;
    }
    if (vctxt->xpathStates != NULL) {
	ret = xmlSchemaXPathEvaluate(vctxt, XML_ELEMENT_NODE);
	vctxt->inode->appliedXPath = 1;
	if (ret == -1) {
	    VERROR_INT("xmlSchemaValidateElem",
		"calling xmlSchemaXPathEvaluate()");
	    goto internal_error;
	}
    }
    if (WXS_IS_COMPLEX(vctxt->inode->typeDef)) {
	if ((vctxt->nbAttrInfos != 0) ||
	    (vctxt->inode->typeDef->attrUses != NULL)) {

	    ret = xmlSchemaVAttributesComplex(vctxt);
	}
    } else if (vctxt->nbAttrInfos != 0) {

	ret = xmlSchemaVAttributesSimple(vctxt);
    }
    if (vctxt->nbAttrInfos != 0)
	xmlSchemaClearAttrInfos(vctxt);
    if (ret == -1) {
	VERROR_INT("xmlSchemaValidateElem",
	    "calling attributes validation");
	goto internal_error;
    }
    ret = 0;

exit:
    if (ret != 0)
	vctxt->skipDepth = vctxt->depth;
    return (ret);
internal_error:
    return (-1);
}

#ifdef XML_SCHEMA_READER_ENABLED
static int
xmlSchemaVReaderWalk(xmlSchemaValidCtxtPtr vctxt)
{
    const int WHTSP = 13, SIGN_WHTSP = 14, END_ELEM = 15;
    int depth, nodeType, ret = 0, consumed;
    xmlSchemaNodeInfoPtr ielem;

    vctxt->depth = -1;
    ret = xmlTextReaderRead(vctxt->reader);
    while (ret == 1) {
	nodeType = xmlTextReaderNodeType(vctxt->reader);
	if (nodeType == XML_ELEMENT_NODE)
	    goto root_found;
	ret = xmlTextReaderRead(vctxt->reader);
    }
    goto exit;

root_found:

    do {
	depth = xmlTextReaderDepth(vctxt->reader);
	nodeType = xmlTextReaderNodeType(vctxt->reader);

	if (nodeType == XML_ELEMENT_NODE) {

	    vctxt->depth++;
	    if (xmlSchemaValidatorPushElem(vctxt) == -1) {
		VERROR_INT("xmlSchemaVReaderWalk",
		    "calling xmlSchemaValidatorPushElem()");
		goto internal_error;
	    }
	    ielem = vctxt->inode;
	    ielem->localName = xmlTextReaderLocalName(vctxt->reader);
	    ielem->nsName = xmlTextReaderNamespaceUri(vctxt->reader);
	    ielem->flags |= XML_SCHEMA_NODE_INFO_FLAG_OWNED_NAMES;
	    ret = xmlTextReaderIsEmptyElement(vctxt->reader);
	    if (ret == -1) {
		VERROR_INT("xmlSchemaVReaderWalk",
		    "calling xmlTextReaderIsEmptyElement()");
		goto internal_error;
	    }
	    if (ret) {
		ielem->flags |= XML_SCHEMA_ELEM_INFO_EMPTY;
	    }
	    vctxt->nbAttrInfos = 0;
	    ret = xmlTextReaderMoveToFirstAttribute(vctxt->reader);
	    if (ret == -1) {
		VERROR_INT("xmlSchemaVReaderWalk",
		    "calling xmlTextReaderMoveToFirstAttribute()");
		goto internal_error;
	    }
	    if (ret == 1) {
		do {
		    if (xmlSchemaValidatorPushAttribute(vctxt, NULL,
			(const xmlChar *) xmlTextReaderLocalName(vctxt->reader),
			xmlTextReaderNamespaceUri(vctxt->reader), 1,
			xmlTextReaderValue(vctxt->reader), 1) == -1) {

			VERROR_INT("xmlSchemaVReaderWalk",
			    "calling xmlSchemaValidatorPushAttribute()");
			goto internal_error;
		    }
		    ret = xmlTextReaderMoveToNextAttribute(vctxt->reader);
		    if (ret == -1) {
			VERROR_INT("xmlSchemaVReaderWalk",
			    "calling xmlTextReaderMoveToFirstAttribute()");
			goto internal_error;
		    }
		} while (ret == 1);
		ret = xmlTextReaderMoveToElement(vctxt->reader);
		if (ret == -1) {
		    VERROR_INT("xmlSchemaVReaderWalk",
			"calling xmlTextReaderMoveToElement()");
		    goto internal_error;
		}
	    }
	    ret= xmlSchemaValidateElem(vctxt);
	    if (ret != 0) {
		if (ret == -1) {
		    VERROR_INT("xmlSchemaVReaderWalk",
			"calling xmlSchemaValidateElem()");
		    goto internal_error;
		}
		goto exit;
	    }
	    if (vctxt->depth == vctxt->skipDepth) {
		int curDepth;
		if ((ielem->flags & XML_SCHEMA_ELEM_INFO_EMPTY) == 0) {
		    ret = xmlTextReaderRead(vctxt->reader);
		    curDepth = xmlTextReaderDepth(vctxt->reader);
		    while ((ret == 1) && (curDepth != depth)) {
			ret = xmlTextReaderRead(vctxt->reader);
			curDepth = xmlTextReaderDepth(vctxt->reader);
		    }
		    if (ret < 0) {
			ret = 1;
			goto exit;
		    }
		}
		goto leave_elem;
	    }
	    if (ielem->flags & XML_SCHEMA_ELEM_INFO_EMPTY)
		goto leave_elem;
	} else if (nodeType == END_ELEM) {
leave_elem:
	    ret = xmlSchemaValidatorPopElem(vctxt);
	    if (ret != 0) {
		if (ret < 0) {
		    VERROR_INT("xmlSchemaVReaderWalk",
			"calling xmlSchemaValidatorPopElem()");
		    goto internal_error;
		}
		goto exit;
	    }
	    if (vctxt->depth >= 0)
		ielem = vctxt->inode;
	    else
		ielem = NULL;
	} else if ((nodeType == XML_TEXT_NODE) ||
	    (nodeType == XML_CDATA_SECTION_NODE) ||
	    (nodeType == WHTSP) ||
	    (nodeType == SIGN_WHTSP)) {
	    xmlChar *value;

	    if ((nodeType == WHTSP) || (nodeType == SIGN_WHTSP))
		nodeType = XML_TEXT_NODE;

	    value = xmlTextReaderValue(vctxt->reader);
	    ret = xmlSchemaVPushText(vctxt, nodeType, BAD_CAST value,
		-1, XML_SCHEMA_PUSH_TEXT_CREATED, &consumed);
	    if (! consumed)
		xmlFree(value);
	    if (ret == -1) {
		VERROR_INT("xmlSchemaVReaderWalk",
		    "calling xmlSchemaVPushText()");
		goto internal_error;
	    }
	} else if ((nodeType == XML_ENTITY_NODE) ||
	    (nodeType == XML_ENTITY_REF_NODE)) {
	    TODO
	}
	ret = xmlTextReaderRead(vctxt->reader);
    } while (ret == 1);

exit:
    return (ret);
internal_error:
    return (-1);
}
#endif


static void
xmlSchemaSAXHandleText(void *ctx,
		       const xmlChar * ch,
		       int len)
{
    xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) ctx;

    if (vctxt->depth < 0)
	return;
    if ((vctxt->skipDepth != -1) && (vctxt->depth >= vctxt->skipDepth))
	return;
    if (vctxt->inode->flags & XML_SCHEMA_ELEM_INFO_EMPTY)
	vctxt->inode->flags ^= XML_SCHEMA_ELEM_INFO_EMPTY;
    if (xmlSchemaVPushText(vctxt, XML_TEXT_NODE, ch, len,
	XML_SCHEMA_PUSH_TEXT_VOLATILE, NULL) == -1) {
	VERROR_INT("xmlSchemaSAXHandleCDataSection",
	    "calling xmlSchemaVPushText()");
	vctxt->err = -1;
	xmlStopParser(vctxt->parserCtxt);
    }
}

static void
xmlSchemaSAXHandleCDataSection(void *ctx,
			     const xmlChar * ch,
			     int len)
{
    xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) ctx;

    if (vctxt->depth < 0)
	return;
    if ((vctxt->skipDepth != -1) && (vctxt->depth >= vctxt->skipDepth))
	return;
    if (vctxt->inode->flags & XML_SCHEMA_ELEM_INFO_EMPTY)
	vctxt->inode->flags ^= XML_SCHEMA_ELEM_INFO_EMPTY;
    if (xmlSchemaVPushText(vctxt, XML_CDATA_SECTION_NODE, ch, len,
	XML_SCHEMA_PUSH_TEXT_VOLATILE, NULL) == -1) {
	VERROR_INT("xmlSchemaSAXHandleCDataSection",
	    "calling xmlSchemaVPushText()");
	vctxt->err = -1;
	xmlStopParser(vctxt->parserCtxt);
    }
}

static void
xmlSchemaSAXHandleReference(void *ctx ATTRIBUTE_UNUSED,
			    const xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) ctx;

    if (vctxt->depth < 0)
	return;
    if ((vctxt->skipDepth != -1) && (vctxt->depth >= vctxt->skipDepth))
	return;
    
    TODO
}

static void
xmlSchemaSAXHandleStartElementNs(void *ctx,
				 const xmlChar * localname,
				 const xmlChar * prefix ATTRIBUTE_UNUSED,
				 const xmlChar * URI,
				 int nb_namespaces,
				 const xmlChar ** namespaces,
				 int nb_attributes,
				 int nb_defaulted ATTRIBUTE_UNUSED,
				 const xmlChar ** attributes)
{
    xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) ctx;
    int ret;
    xmlSchemaNodeInfoPtr ielem;
    int i, j;

    vctxt->depth++;
    if ((vctxt->skipDepth != -1) && (vctxt->depth >= vctxt->skipDepth))
	return;
    if (xmlSchemaValidatorPushElem(vctxt) == -1) {
	VERROR_INT("xmlSchemaSAXHandleStartElementNs",
	    "calling xmlSchemaValidatorPushElem()");
	goto internal_error;
    }
    ielem = vctxt->inode;
    ielem->nodeLine = xmlSAX2GetLineNumber(vctxt->parserCtxt);
    ielem->localName = localname;
    ielem->nsName = URI;
    ielem->flags |= XML_SCHEMA_ELEM_INFO_EMPTY;
    if (nb_namespaces != 0) {
        for (i = 0, j = 0; i < nb_namespaces; i++, j += 2) {
	    if (ielem->nsBindings == NULL) {
		ielem->nsBindings =
		    (const xmlChar **) xmlMalloc(10 *
			sizeof(const xmlChar *));
		if (ielem->nsBindings == NULL) {
		    xmlSchemaVErrMemory(vctxt,
			"allocating namespace bindings for SAX validation",
			NULL);
		    goto internal_error;
		}
		ielem->nbNsBindings = 0;
		ielem->sizeNsBindings = 5;
	    } else if (ielem->sizeNsBindings <= ielem->nbNsBindings) {
		ielem->sizeNsBindings *= 2;
		ielem->nsBindings =
		    (const xmlChar **) xmlRealloc(
			(void *) ielem->nsBindings,
			ielem->sizeNsBindings * 2 * sizeof(const xmlChar *));
		if (ielem->nsBindings == NULL) {
		    xmlSchemaVErrMemory(vctxt,
			"re-allocating namespace bindings for SAX validation",
			NULL);
		    goto internal_error;
		}
	    }

	    ielem->nsBindings[ielem->nbNsBindings * 2] = namespaces[j];
	    if (namespaces[j+1][0] == 0) {
		ielem->nsBindings[ielem->nbNsBindings * 2 + 1] = NULL;
	    } else
		ielem->nsBindings[ielem->nbNsBindings * 2 + 1] =
		    namespaces[j+1];
	    ielem->nbNsBindings++;
	}
    }
    if (nb_attributes != 0) {
	xmlChar *value;

        for (j = 0, i = 0; i < nb_attributes; i++, j += 5) {
	    value = xmlStrndup(attributes[j+3],
		attributes[j+4] - attributes[j+3]);
	    ret = xmlSchemaValidatorPushAttribute(vctxt,
		NULL, ielem->nodeLine, attributes[j], attributes[j+2], 0,
		value, 1);
	    if (ret == -1) {
		VERROR_INT("xmlSchemaSAXHandleStartElementNs",
		    "calling xmlSchemaValidatorPushAttribute()");
		goto internal_error;
	    }
	}
    }
    ret = xmlSchemaValidateElem(vctxt);
    if (ret != 0) {
	if (ret == -1) {
	    VERROR_INT("xmlSchemaSAXHandleStartElementNs",
		"calling xmlSchemaValidateElem()");
	    goto internal_error;
	}
	goto exit;
    }

exit:
    return;
internal_error:
    vctxt->err = -1;
    xmlStopParser(vctxt->parserCtxt);
    return;
}

static void
xmlSchemaSAXHandleEndElementNs(void *ctx,
			       const xmlChar * localname ATTRIBUTE_UNUSED,
			       const xmlChar * prefix ATTRIBUTE_UNUSED,
			       const xmlChar * URI ATTRIBUTE_UNUSED)
{
    xmlSchemaValidCtxtPtr vctxt = (xmlSchemaValidCtxtPtr) ctx;
    int res;

    if (vctxt->skipDepth != -1) {
	if (vctxt->depth > vctxt->skipDepth) {
	    vctxt->depth--;
	    return;
	} else
	    vctxt->skipDepth = -1;
    }
    if ((!xmlStrEqual(vctxt->inode->localName, localname)) ||
	(!xmlStrEqual(vctxt->inode->nsName, URI))) {
	VERROR_INT("xmlSchemaSAXHandleEndElementNs",
	    "elem pop mismatch");
    }
    res = xmlSchemaValidatorPopElem(vctxt);
    if (res != 0) {
	if (res < 0) {
	    VERROR_INT("xmlSchemaSAXHandleEndElementNs",
		"calling xmlSchemaValidatorPopElem()");
	    goto internal_error;
	}
	goto exit;
    }
exit:
    return;
internal_error:
    vctxt->err = -1;
    xmlStopParser(vctxt->parserCtxt);
    return;
}


xmlSchemaValidCtxtPtr
xmlSchemaNewValidCtxt(xmlSchemaPtr schema)
{
    xmlSchemaValidCtxtPtr ret;

    ret = (xmlSchemaValidCtxtPtr) xmlMalloc(sizeof(xmlSchemaValidCtxt));
    if (ret == NULL) {
        xmlSchemaVErrMemory(NULL, "allocating validation context", NULL);
        return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaValidCtxt));
    ret->type = XML_SCHEMA_CTXT_VALIDATOR;
    ret->dict = xmlDictCreate();
    ret->nodeQNames = xmlSchemaItemListCreate();
    ret->schema = schema;
    return (ret);
}

static void
xmlSchemaClearValidCtxt(xmlSchemaValidCtxtPtr vctxt)
{
    if (vctxt == NULL)
        return;

    vctxt->flags = 0;
    vctxt->validationRoot = NULL;
    vctxt->doc = NULL;
#ifdef LIBXML_READER_ENABLED
    vctxt->reader = NULL;
#endif
    vctxt->hasKeyrefs = 0;

    if (vctxt->value != NULL) {
        xmlSchemaFreeValue(vctxt->value);
	vctxt->value = NULL;
    }
    if (vctxt->aidcs != NULL) {
	xmlSchemaIDCAugPtr cur = vctxt->aidcs, next;
	do {
	    next = cur->next;
	    xmlFree(cur);
	    cur = next;
	} while (cur != NULL);
	vctxt->aidcs = NULL;
    }
    if (vctxt->idcMatcherCache != NULL) {
	xmlSchemaIDCMatcherPtr matcher = vctxt->idcMatcherCache, tmp;

	while (matcher) {
	    tmp = matcher;
	    matcher = matcher->nextCached;
	    xmlSchemaIDCFreeMatcherList(tmp);
	}
	vctxt->idcMatcherCache = NULL;
    }


    if (vctxt->idcNodes != NULL) {
	int i;
	xmlSchemaPSVIIDCNodePtr item;

	for (i = 0; i < vctxt->nbIdcNodes; i++) {
	    item = vctxt->idcNodes[i];
	    xmlFree(item->keys);
	    xmlFree(item);
	}
	xmlFree(vctxt->idcNodes);
	vctxt->idcNodes = NULL;
	vctxt->nbIdcNodes = 0;
	vctxt->sizeIdcNodes = 0;
    }
    if (vctxt->xpathStates != NULL) {
	xmlSchemaFreeIDCStateObjList(vctxt->xpathStates);
	vctxt->xpathStates = NULL;
    }
    if (vctxt->nbAttrInfos != 0) {
	xmlSchemaClearAttrInfos(vctxt);
    }
    if (vctxt->elemInfos != NULL) {
	int i;
	xmlSchemaNodeInfoPtr ei;

	for (i = 0; i < vctxt->sizeElemInfos; i++) {
	    ei = vctxt->elemInfos[i];
	    if (ei == NULL)
		break;
	    xmlSchemaClearElemInfo(vctxt, ei);
	}
    }
    xmlSchemaItemListClear(vctxt->nodeQNames);
    
    xmlDictFree(vctxt->dict);
    vctxt->dict = xmlDictCreate();
}

void
xmlSchemaFreeValidCtxt(xmlSchemaValidCtxtPtr ctxt)
{
    if (ctxt == NULL)
        return;
    if (ctxt->value != NULL)
        xmlSchemaFreeValue(ctxt->value);
    if (ctxt->pctxt != NULL)
	xmlSchemaFreeParserCtxt(ctxt->pctxt);
    if (ctxt->idcNodes != NULL) {
	int i;
	xmlSchemaPSVIIDCNodePtr item;

	for (i = 0; i < ctxt->nbIdcNodes; i++) {
	    item = ctxt->idcNodes[i];
	    xmlFree(item->keys);
	    xmlFree(item);
	}
	xmlFree(ctxt->idcNodes);
    }
    if (ctxt->idcKeys != NULL) {
	int i;
	for (i = 0; i < ctxt->nbIdcKeys; i++)
	    xmlSchemaIDCFreeKey(ctxt->idcKeys[i]);
	xmlFree(ctxt->idcKeys);
    }

    if (ctxt->xpathStates != NULL) {
	xmlSchemaFreeIDCStateObjList(ctxt->xpathStates);
	ctxt->xpathStates = NULL;
    }
    if (ctxt->xpathStatePool != NULL) {
	xmlSchemaFreeIDCStateObjList(ctxt->xpathStatePool);
	ctxt->xpathStatePool = NULL;
    }

    if (ctxt->aidcs != NULL) {
	xmlSchemaIDCAugPtr cur = ctxt->aidcs, next;
	do {
	    next = cur->next;
	    xmlFree(cur);
	    cur = next;
	} while (cur != NULL);
    }
    if (ctxt->attrInfos != NULL) {
	int i;
	xmlSchemaAttrInfoPtr attr;

	
	if (ctxt->nbAttrInfos != 0)
	    xmlSchemaClearAttrInfos(ctxt);
	for (i = 0; i < ctxt->sizeAttrInfos; i++) {
	    attr = ctxt->attrInfos[i];
	    xmlFree(attr);
	}
	xmlFree(ctxt->attrInfos);
    }
    if (ctxt->elemInfos != NULL) {
	int i;
	xmlSchemaNodeInfoPtr ei;

	for (i = 0; i < ctxt->sizeElemInfos; i++) {
	    ei = ctxt->elemInfos[i];
	    if (ei == NULL)
		break;
	    xmlSchemaClearElemInfo(ctxt, ei);
	    xmlFree(ei);
	}
	xmlFree(ctxt->elemInfos);
    }
    if (ctxt->nodeQNames != NULL)
	xmlSchemaItemListFree(ctxt->nodeQNames);
    if (ctxt->dict != NULL)
	xmlDictFree(ctxt->dict);
    xmlFree(ctxt);
}

int
xmlSchemaIsValid(xmlSchemaValidCtxtPtr ctxt)
{
    if (ctxt == NULL)
        return(-1);
    return(ctxt->err == 0);
}

void
xmlSchemaSetValidErrors(xmlSchemaValidCtxtPtr ctxt,
                        xmlSchemaValidityErrorFunc err,
                        xmlSchemaValidityWarningFunc warn, void *ctx)
{
    if (ctxt == NULL)
        return;
    ctxt->error = err;
    ctxt->warning = warn;
    ctxt->errCtxt = ctx;
    if (ctxt->pctxt != NULL)
	xmlSchemaSetParserErrors(ctxt->pctxt, err, warn, ctx);
}

void
xmlSchemaSetValidStructuredErrors(xmlSchemaValidCtxtPtr ctxt,
				  xmlStructuredErrorFunc serror, void *ctx)
{
    if (ctxt == NULL)
        return;
	ctxt->serror = serror;
    ctxt->error = NULL;
    ctxt->warning = NULL;
    ctxt->errCtxt = ctx;
    if (ctxt->pctxt != NULL)
	xmlSchemaSetParserStructuredErrors(ctxt->pctxt, serror, ctx);
}

int
xmlSchemaGetValidErrors(xmlSchemaValidCtxtPtr ctxt,
			xmlSchemaValidityErrorFunc * err,
			xmlSchemaValidityWarningFunc * warn, void **ctx)
{
	if (ctxt == NULL)
		return (-1);
	if (err != NULL)
		*err = ctxt->error;
	if (warn != NULL)
		*warn = ctxt->warning;
	if (ctx != NULL)
		*ctx = ctxt->errCtxt;
	return (0);
}


int
xmlSchemaSetValidOptions(xmlSchemaValidCtxtPtr ctxt,
			 int options)

{
    int i;

    if (ctxt == NULL)
	return (-1);
    for (i = 1; i < (int) sizeof(int) * 8; i++) {
        if (options & 1<<i)
	    return (-1);
    }
    ctxt->options = options;
    return (0);
}

int
xmlSchemaValidCtxtGetOptions(xmlSchemaValidCtxtPtr ctxt)

{
    if (ctxt == NULL)
	return (-1);
    else
	return (ctxt->options);
}

static int
xmlSchemaVDocWalk(xmlSchemaValidCtxtPtr vctxt)
{
    xmlAttrPtr attr;
    int ret = 0;
    xmlSchemaNodeInfoPtr ielem = NULL;
    xmlNodePtr node, valRoot;
    const xmlChar *nsName;

    
    valRoot = xmlDocGetRootElement(vctxt->doc);
    if (valRoot == NULL) {
	
	VERROR(1, NULL, "The document has no document element");
	return (1);
    }
    vctxt->depth = -1;
    vctxt->validationRoot = valRoot;
    node = valRoot;
    while (node != NULL) {
	if ((vctxt->skipDepth != -1) && (vctxt->depth >= vctxt->skipDepth))
	    goto next_sibling;
	if (node->type == XML_ELEMENT_NODE) {

	    vctxt->depth++;
	    if (xmlSchemaValidatorPushElem(vctxt) == -1)
		goto internal_error;
	    ielem = vctxt->inode;
	    ielem->node = node;
	    ielem->nodeLine = node->line;
	    ielem->localName = node->name;
	    if (node->ns != NULL)
		ielem->nsName = node->ns->href;
	    ielem->flags |= XML_SCHEMA_ELEM_INFO_EMPTY;
	    vctxt->nbAttrInfos = 0;
	    if (node->properties != NULL) {
		attr = node->properties;
		do {
		    if (attr->ns != NULL)
			nsName = attr->ns->href;
		    else
			nsName = NULL;
		    ret = xmlSchemaValidatorPushAttribute(vctxt,
			(xmlNodePtr) attr,
			ielem->nodeLine,
			attr->name, nsName, 0,
			xmlNodeListGetString(attr->doc, attr->children, 1), 1);
		    if (ret == -1) {
			VERROR_INT("xmlSchemaDocWalk",
			    "calling xmlSchemaValidatorPushAttribute()");
			goto internal_error;
		    }
		    attr = attr->next;
		} while (attr);
	    }
	    ret = xmlSchemaValidateElem(vctxt);
	    if (ret != 0) {
		if (ret == -1) {
		    VERROR_INT("xmlSchemaDocWalk",
			"calling xmlSchemaValidateElem()");
		    goto internal_error;
		}
		goto leave_node;
	    }
	    if ((vctxt->skipDepth != -1) &&
		(vctxt->depth >= vctxt->skipDepth))
		goto leave_node;
	} else if ((node->type == XML_TEXT_NODE) ||
	    (node->type == XML_CDATA_SECTION_NODE)) {
	    if ((ielem != NULL) && (ielem->flags & XML_SCHEMA_ELEM_INFO_EMPTY))
		ielem->flags ^= XML_SCHEMA_ELEM_INFO_EMPTY;
	    ret = xmlSchemaVPushText(vctxt, node->type, node->content,
		-1, XML_SCHEMA_PUSH_TEXT_PERSIST, NULL);
	    if (ret < 0) {
		VERROR_INT("xmlSchemaVDocWalk",
		    "calling xmlSchemaVPushText()");
		goto internal_error;
	    }
	} else if ((node->type == XML_ENTITY_NODE) ||
	    (node->type == XML_ENTITY_REF_NODE)) {
	    VERROR_INT("xmlSchemaVDocWalk",
		"there is at least one entity reference in the node-tree "
		"currently being validated. Processing of entities with "
		"this XML Schema processor is not supported (yet). Please "
		"substitute entities before validation.");
	    goto internal_error;
	} else {
	    goto leave_node;
	}
	if (node->children != NULL) {
	    node = node->children;
	    continue;
	}
leave_node:
	if (node->type == XML_ELEMENT_NODE) {
	    if (node != vctxt->inode->node) {
		VERROR_INT("xmlSchemaVDocWalk",
		    "element position mismatch");
		goto internal_error;
	    }
	    ret = xmlSchemaValidatorPopElem(vctxt);
	    if (ret != 0) {
		if (ret < 0) {
		    VERROR_INT("xmlSchemaVDocWalk",
			"calling xmlSchemaValidatorPopElem()");
		    goto internal_error;
		}
	    }
	    if (node == valRoot)
		goto exit;
	}
next_sibling:
	if (node->next != NULL)
	    node = node->next;
	else {
	    node = node->parent;
	    goto leave_node;
	}
    }

exit:
    return (ret);
internal_error:
    return (-1);
}

static int
xmlSchemaPreRun(xmlSchemaValidCtxtPtr vctxt) {
    vctxt->err = 0;
    vctxt->nberrors = 0;
    vctxt->depth = -1;
    vctxt->skipDepth = -1;
    vctxt->xsiAssemble = 0;
    vctxt->hasKeyrefs = 0;
#ifdef ENABLE_IDC_NODE_TABLES_TEST
    vctxt->createIDCNodeTables = 1;
#else
    vctxt->createIDCNodeTables = 0;
#endif
    if (vctxt->schema == NULL) {
	xmlSchemaParserCtxtPtr pctxt;

	vctxt->xsiAssemble = 1;
	if ((vctxt->pctxt == NULL) &&
	   (xmlSchemaCreatePCtxtOnVCtxt(vctxt) == -1))
	   return (-1);
	pctxt = vctxt->pctxt;
	pctxt->xsiAssemble = 1;
	vctxt->schema = xmlSchemaNewSchema(pctxt);
	if (vctxt->schema == NULL)
	    return (-1);
	pctxt->constructor = xmlSchemaConstructionCtxtCreate(pctxt->dict);
	if (pctxt->constructor == NULL)
	    return(-1);
	pctxt->constructor->mainSchema = vctxt->schema;
	pctxt->ownsConstructor = 1;
    }
    xmlHashScan(vctxt->schema->schemasImports,(xmlHashScanner)xmlSchemaAugmentImportedIDC, vctxt);

    return(0);
}

static void
xmlSchemaPostRun(xmlSchemaValidCtxtPtr vctxt) {
    if (vctxt->xsiAssemble) {
	if (vctxt->schema != NULL) {
	    xmlSchemaFree(vctxt->schema);
	    vctxt->schema = NULL;
	}
    }
    xmlSchemaClearValidCtxt(vctxt);
}

static int
xmlSchemaVStart(xmlSchemaValidCtxtPtr vctxt)
{
    int ret = 0;

    if (xmlSchemaPreRun(vctxt) < 0)
        return(-1);

    if (vctxt->doc != NULL) {
	ret = xmlSchemaVDocWalk(vctxt);
#ifdef LIBXML_READER_ENABLED
    } else if (vctxt->reader != NULL) {
#ifdef XML_SCHEMA_READER_ENABLED
	ret = xmlSchemaVReaderWalk(vctxt);
#endif
#endif
    } else if ((vctxt->sax != NULL) && (vctxt->parserCtxt != NULL)) {
	ret = xmlParseDocument(vctxt->parserCtxt);
    } else {
	VERROR_INT("xmlSchemaVStart",
	    "no instance to validate");
	ret = -1;
    }

    xmlSchemaPostRun(vctxt);
    if (ret == 0)
	ret = vctxt->err;
    return (ret);
}

int
xmlSchemaValidateOneElement(xmlSchemaValidCtxtPtr ctxt, xmlNodePtr elem)
{
    if ((ctxt == NULL) || (elem == NULL) || (elem->type != XML_ELEMENT_NODE))
	return (-1);

    if (ctxt->schema == NULL)
	return (-1);

    ctxt->doc = elem->doc;
    ctxt->node = elem;
    ctxt->validationRoot = elem;
    return(xmlSchemaVStart(ctxt));
}

int
xmlSchemaValidateDoc(xmlSchemaValidCtxtPtr ctxt, xmlDocPtr doc)
{
    if ((ctxt == NULL) || (doc == NULL))
        return (-1);

    ctxt->doc = doc;
    ctxt->node = xmlDocGetRootElement(doc);
    if (ctxt->node == NULL) {
        xmlSchemaCustomErr(ACTXT_CAST ctxt,
	    XML_SCHEMAV_DOCUMENT_ELEMENT_MISSING,
	    (xmlNodePtr) doc, NULL,
	    "The document has no document element", NULL, NULL);
        return (ctxt->err);
    }
    ctxt->validationRoot = ctxt->node;
    return (xmlSchemaVStart(ctxt));
}


typedef struct _xmlSchemaSplitSAXData xmlSchemaSplitSAXData;
typedef xmlSchemaSplitSAXData *xmlSchemaSplitSAXDataPtr;

struct _xmlSchemaSplitSAXData {
    xmlSAXHandlerPtr      user_sax;
    void                 *user_data;
    xmlSchemaValidCtxtPtr ctxt;
    xmlSAXHandlerPtr      schemas_sax;
};

#define XML_SAX_PLUG_MAGIC 0xdc43ba21

struct _xmlSchemaSAXPlug {
    unsigned int magic;

    
    xmlSAXHandlerPtr     *user_sax_ptr;
    xmlSAXHandlerPtr      user_sax;
    void                **user_data_ptr;
    void                 *user_data;

    
    xmlSAXHandler         schemas_sax;
    xmlSchemaValidCtxtPtr ctxt;
};

static void
internalSubsetSplit(void *ctx, const xmlChar *name,
	       const xmlChar *ExternalID, const xmlChar *SystemID)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->internalSubset != NULL))
	ctxt->user_sax->internalSubset(ctxt->user_data, name, ExternalID,
	                               SystemID);
}

static int
isStandaloneSplit(void *ctx)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->isStandalone != NULL))
	return(ctxt->user_sax->isStandalone(ctxt->user_data));
    return(0);
}

static int
hasInternalSubsetSplit(void *ctx)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->hasInternalSubset != NULL))
	return(ctxt->user_sax->hasInternalSubset(ctxt->user_data));
    return(0);
}

static int
hasExternalSubsetSplit(void *ctx)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->hasExternalSubset != NULL))
	return(ctxt->user_sax->hasExternalSubset(ctxt->user_data));
    return(0);
}

static void
externalSubsetSplit(void *ctx, const xmlChar *name,
	       const xmlChar *ExternalID, const xmlChar *SystemID)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->externalSubset != NULL))
	ctxt->user_sax->externalSubset(ctxt->user_data, name, ExternalID,
	                               SystemID);
}

static xmlParserInputPtr
resolveEntitySplit(void *ctx, const xmlChar *publicId, const xmlChar *systemId)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->resolveEntity != NULL))
	return(ctxt->user_sax->resolveEntity(ctxt->user_data, publicId,
	                                     systemId));
    return(NULL);
}

static xmlEntityPtr
getEntitySplit(void *ctx, const xmlChar *name)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->getEntity != NULL))
	return(ctxt->user_sax->getEntity(ctxt->user_data, name));
    return(NULL);
}

static xmlEntityPtr
getParameterEntitySplit(void *ctx, const xmlChar *name)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->getParameterEntity != NULL))
	return(ctxt->user_sax->getParameterEntity(ctxt->user_data, name));
    return(NULL);
}


static void
entityDeclSplit(void *ctx, const xmlChar *name, int type,
          const xmlChar *publicId, const xmlChar *systemId, xmlChar *content)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->entityDecl != NULL))
	ctxt->user_sax->entityDecl(ctxt->user_data, name, type, publicId,
	                           systemId, content);
}

static void
attributeDeclSplit(void *ctx, const xmlChar * elem,
                   const xmlChar * name, int type, int def,
                   const xmlChar * defaultValue, xmlEnumerationPtr tree)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->attributeDecl != NULL)) {
	ctxt->user_sax->attributeDecl(ctxt->user_data, elem, name, type,
	                              def, defaultValue, tree);
    } else {
	xmlFreeEnumeration(tree);
    }
}

static void
elementDeclSplit(void *ctx, const xmlChar *name, int type,
	    xmlElementContentPtr content)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->elementDecl != NULL))
	ctxt->user_sax->elementDecl(ctxt->user_data, name, type, content);
}

static void
notationDeclSplit(void *ctx, const xmlChar *name,
	     const xmlChar *publicId, const xmlChar *systemId)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->notationDecl != NULL))
	ctxt->user_sax->notationDecl(ctxt->user_data, name, publicId,
	                             systemId);
}

static void
unparsedEntityDeclSplit(void *ctx, const xmlChar *name,
		   const xmlChar *publicId, const xmlChar *systemId,
		   const xmlChar *notationName)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->unparsedEntityDecl != NULL))
	ctxt->user_sax->unparsedEntityDecl(ctxt->user_data, name, publicId,
	                                   systemId, notationName);
}

static void
setDocumentLocatorSplit(void *ctx, xmlSAXLocatorPtr loc)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->setDocumentLocator != NULL))
	ctxt->user_sax->setDocumentLocator(ctxt->user_data, loc);
}

static void
startDocumentSplit(void *ctx)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->startDocument != NULL))
	ctxt->user_sax->startDocument(ctxt->user_data);
}

static void
endDocumentSplit(void *ctx)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->endDocument != NULL))
	ctxt->user_sax->endDocument(ctxt->user_data);
}

static void
processingInstructionSplit(void *ctx, const xmlChar *target,
                      const xmlChar *data)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->processingInstruction != NULL))
	ctxt->user_sax->processingInstruction(ctxt->user_data, target, data);
}

static void
commentSplit(void *ctx, const xmlChar *value)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->comment != NULL))
	ctxt->user_sax->comment(ctxt->user_data, value);
}


static void XMLCDECL
warningSplit(void *ctx, const char *msg ATTRIBUTE_UNUSED, ...) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->warning != NULL)) {
	TODO
    }
}
static void XMLCDECL
errorSplit(void *ctx, const char *msg ATTRIBUTE_UNUSED, ...) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->error != NULL)) {
	TODO
    }
}
static void XMLCDECL
fatalErrorSplit(void *ctx, const char *msg ATTRIBUTE_UNUSED, ...) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->fatalError != NULL)) {
	TODO
    }
}

static void
charactersSplit(void *ctx, const xmlChar *ch, int len)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if (ctxt == NULL)
        return;
    if ((ctxt->user_sax != NULL) && (ctxt->user_sax->characters != NULL))
	ctxt->user_sax->characters(ctxt->user_data, ch, len);
    if (ctxt->ctxt != NULL)
	xmlSchemaSAXHandleText(ctxt->ctxt, ch, len);
}

static void
ignorableWhitespaceSplit(void *ctx, const xmlChar *ch, int len)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if (ctxt == NULL)
        return;
    if ((ctxt->user_sax != NULL) &&
        (ctxt->user_sax->ignorableWhitespace != NULL))
	ctxt->user_sax->ignorableWhitespace(ctxt->user_data, ch, len);
    if (ctxt->ctxt != NULL)
	xmlSchemaSAXHandleText(ctxt->ctxt, ch, len);
}

static void
cdataBlockSplit(void *ctx, const xmlChar *value, int len)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if (ctxt == NULL)
        return;
    if ((ctxt->user_sax != NULL) &&
        (ctxt->user_sax->cdataBlock != NULL))
	ctxt->user_sax->cdataBlock(ctxt->user_data, value, len);
    if (ctxt->ctxt != NULL)
	xmlSchemaSAXHandleCDataSection(ctxt->ctxt, value, len);
}

static void
referenceSplit(void *ctx, const xmlChar *name)
{
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if (ctxt == NULL)
        return;
    if ((ctxt != NULL) && (ctxt->user_sax != NULL) &&
        (ctxt->user_sax->reference != NULL))
	ctxt->user_sax->reference(ctxt->user_data, name);
    if (ctxt->ctxt != NULL)
        xmlSchemaSAXHandleReference(ctxt->user_data, name);
}

static void
startElementNsSplit(void *ctx, const xmlChar * localname,
		    const xmlChar * prefix, const xmlChar * URI,
		    int nb_namespaces, const xmlChar ** namespaces,
		    int nb_attributes, int nb_defaulted,
		    const xmlChar ** attributes) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if (ctxt == NULL)
        return;
    if ((ctxt->user_sax != NULL) &&
        (ctxt->user_sax->startElementNs != NULL))
	ctxt->user_sax->startElementNs(ctxt->user_data, localname, prefix,
	                               URI, nb_namespaces, namespaces,
				       nb_attributes, nb_defaulted,
				       attributes);
    if (ctxt->ctxt != NULL)
	xmlSchemaSAXHandleStartElementNs(ctxt->ctxt, localname, prefix,
	                                 URI, nb_namespaces, namespaces,
					 nb_attributes, nb_defaulted,
					 attributes);
}

static void
endElementNsSplit(void *ctx, const xmlChar * localname,
		    const xmlChar * prefix, const xmlChar * URI) {
    xmlSchemaSAXPlugPtr ctxt = (xmlSchemaSAXPlugPtr) ctx;
    if (ctxt == NULL)
        return;
    if ((ctxt->user_sax != NULL) &&
        (ctxt->user_sax->endElementNs != NULL))
	ctxt->user_sax->endElementNs(ctxt->user_data, localname, prefix, URI);
    if (ctxt->ctxt != NULL)
	xmlSchemaSAXHandleEndElementNs(ctxt->ctxt, localname, prefix, URI);
}

xmlSchemaSAXPlugPtr
xmlSchemaSAXPlug(xmlSchemaValidCtxtPtr ctxt,
		 xmlSAXHandlerPtr *sax, void **user_data)
{
    xmlSchemaSAXPlugPtr ret;
    xmlSAXHandlerPtr old_sax;

    if ((ctxt == NULL) || (sax == NULL) || (user_data == NULL))
        return(NULL);

    old_sax = *sax;
    if ((old_sax != NULL) && (old_sax->initialized != XML_SAX2_MAGIC))
        return(NULL);
    if ((old_sax != NULL) &&
        (old_sax->startElementNs == NULL) && (old_sax->endElementNs == NULL) &&
        ((old_sax->startElement != NULL) || (old_sax->endElement != NULL)))
        return(NULL);

    ret = (xmlSchemaSAXPlugPtr) xmlMalloc(sizeof(xmlSchemaSAXPlugStruct));
    if (ret == NULL) {
        return(NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaSAXPlugStruct));
    ret->magic = XML_SAX_PLUG_MAGIC;
    ret->schemas_sax.initialized = XML_SAX2_MAGIC;
    ret->ctxt = ctxt;
    ret->user_sax_ptr = sax;
    ret->user_sax = old_sax;
    if (old_sax == NULL) {
	ret->schemas_sax.startElementNs = xmlSchemaSAXHandleStartElementNs;
	ret->schemas_sax.endElementNs = xmlSchemaSAXHandleEndElementNs;
	ret->schemas_sax.ignorableWhitespace = xmlSchemaSAXHandleText;
	ret->schemas_sax.characters = xmlSchemaSAXHandleText;

	ret->schemas_sax.cdataBlock = xmlSchemaSAXHandleCDataSection;
	ret->schemas_sax.reference = xmlSchemaSAXHandleReference;

	ret->user_data = ctxt;
	*user_data = ctxt;
    } else {
        if (old_sax->internalSubset != NULL)
            ret->schemas_sax.internalSubset = internalSubsetSplit;
        if (old_sax->isStandalone != NULL)
            ret->schemas_sax.isStandalone = isStandaloneSplit;
        if (old_sax->hasInternalSubset != NULL)
            ret->schemas_sax.hasInternalSubset = hasInternalSubsetSplit;
        if (old_sax->hasExternalSubset != NULL)
            ret->schemas_sax.hasExternalSubset = hasExternalSubsetSplit;
        if (old_sax->resolveEntity != NULL)
            ret->schemas_sax.resolveEntity = resolveEntitySplit;
        if (old_sax->getEntity != NULL)
            ret->schemas_sax.getEntity = getEntitySplit;
        if (old_sax->entityDecl != NULL)
            ret->schemas_sax.entityDecl = entityDeclSplit;
        if (old_sax->notationDecl != NULL)
            ret->schemas_sax.notationDecl = notationDeclSplit;
        if (old_sax->attributeDecl != NULL)
            ret->schemas_sax.attributeDecl = attributeDeclSplit;
        if (old_sax->elementDecl != NULL)
            ret->schemas_sax.elementDecl = elementDeclSplit;
        if (old_sax->unparsedEntityDecl != NULL)
            ret->schemas_sax.unparsedEntityDecl = unparsedEntityDeclSplit;
        if (old_sax->setDocumentLocator != NULL)
            ret->schemas_sax.setDocumentLocator = setDocumentLocatorSplit;
        if (old_sax->startDocument != NULL)
            ret->schemas_sax.startDocument = startDocumentSplit;
        if (old_sax->endDocument != NULL)
            ret->schemas_sax.endDocument = endDocumentSplit;
        if (old_sax->processingInstruction != NULL)
            ret->schemas_sax.processingInstruction = processingInstructionSplit;
        if (old_sax->comment != NULL)
            ret->schemas_sax.comment = commentSplit;
        if (old_sax->warning != NULL)
            ret->schemas_sax.warning = warningSplit;
        if (old_sax->error != NULL)
            ret->schemas_sax.error = errorSplit;
        if (old_sax->fatalError != NULL)
            ret->schemas_sax.fatalError = fatalErrorSplit;
        if (old_sax->getParameterEntity != NULL)
            ret->schemas_sax.getParameterEntity = getParameterEntitySplit;
        if (old_sax->externalSubset != NULL)
            ret->schemas_sax.externalSubset = externalSubsetSplit;

        ret->schemas_sax.characters = charactersSplit;
	if ((old_sax->ignorableWhitespace != NULL) &&
	    (old_sax->ignorableWhitespace != old_sax->characters))
	    ret->schemas_sax.ignorableWhitespace = ignorableWhitespaceSplit;
	else
	    ret->schemas_sax.ignorableWhitespace = charactersSplit;
        ret->schemas_sax.cdataBlock = cdataBlockSplit;
        ret->schemas_sax.reference = referenceSplit;
        ret->schemas_sax.startElementNs = startElementNsSplit;
        ret->schemas_sax.endElementNs = endElementNsSplit;

	ret->user_data_ptr = user_data;
	ret->user_data = *user_data;
	*user_data = ret;
    }

    *sax = &(ret->schemas_sax);
    ctxt->sax = *sax;
    ctxt->flags |= XML_SCHEMA_VALID_CTXT_FLAG_STREAM;
    xmlSchemaPreRun(ctxt);
    return(ret);
}

int
xmlSchemaSAXUnplug(xmlSchemaSAXPlugPtr plug)
{
    xmlSAXHandlerPtr *sax;
    void **user_data;

    if ((plug == NULL) || (plug->magic != XML_SAX_PLUG_MAGIC))
        return(-1);
    plug->magic = 0;

    xmlSchemaPostRun(plug->ctxt);
    
    sax = plug->user_sax_ptr;
    *sax = plug->user_sax;
    if (plug->user_sax != NULL) {
	user_data = plug->user_data_ptr;
	*user_data = plug->user_data;
    }

    
    xmlFree(plug);
    return(0);
}

int
xmlSchemaValidateStream(xmlSchemaValidCtxtPtr ctxt,
                        xmlParserInputBufferPtr input, xmlCharEncoding enc,
                        xmlSAXHandlerPtr sax, void *user_data)
{
    xmlSchemaSAXPlugPtr plug = NULL;
    xmlSAXHandlerPtr old_sax = NULL;
    xmlParserCtxtPtr pctxt = NULL;
    xmlParserInputPtr inputStream = NULL;
    int ret;

    if ((ctxt == NULL) || (input == NULL))
        return (-1);

    pctxt = xmlNewParserCtxt();
    if (pctxt == NULL)
        return (-1);
    old_sax = pctxt->sax;
    pctxt->sax = sax;
    pctxt->userData = user_data;
#if 0
    if (options)
        xmlCtxtUseOptions(pctxt, options);
#endif
    pctxt->linenumbers = 1;

    inputStream = xmlNewIOInputStream(pctxt, input, enc);;
    if (inputStream == NULL) {
        ret = -1;
	goto done;
    }
    inputPush(pctxt, inputStream);
    ctxt->parserCtxt = pctxt;
    ctxt->input = input;

    plug = xmlSchemaSAXPlug(ctxt, &(pctxt->sax), &(pctxt->userData));
    if (plug == NULL) {
        ret = -1;
	goto done;
    }
    ctxt->input = input;
    ctxt->enc = enc;
    ctxt->sax = pctxt->sax;
    ctxt->flags |= XML_SCHEMA_VALID_CTXT_FLAG_STREAM;
    ret = xmlSchemaVStart(ctxt);

    if ((ret == 0) && (! ctxt->parserCtxt->wellFormed)) {
	ret = ctxt->parserCtxt->errNo;
	if (ret == 0)
	    ret = 1;
    }

done:
    ctxt->parserCtxt = NULL;
    ctxt->sax = NULL;
    ctxt->input = NULL;
    if (plug != NULL) {
        xmlSchemaSAXUnplug(plug);
    }
    
    if (pctxt != NULL) {
	pctxt->sax = old_sax;
	xmlFreeParserCtxt(pctxt);
    }
    return (ret);
}

int
xmlSchemaValidateFile(xmlSchemaValidCtxtPtr ctxt,
                      const char * filename,
		      int options ATTRIBUTE_UNUSED)
{
    int ret;
    xmlParserInputBufferPtr input;

    if ((ctxt == NULL) || (filename == NULL))
        return (-1);

    input = xmlParserInputBufferCreateFilename(filename,
	XML_CHAR_ENCODING_NONE);
    if (input == NULL)
	return (-1);
    ret = xmlSchemaValidateStream(ctxt, input, XML_CHAR_ENCODING_NONE,
	NULL, NULL);
    return (ret);
}

xmlParserCtxtPtr
xmlSchemaValidCtxtGetParserCtxt(xmlSchemaValidCtxtPtr ctxt)
{
    if (ctxt == NULL)
        return(NULL);
    return (ctxt->parserCtxt);
}

#define bottom_xmlschemas
#include "elfgcchack.h"
#endif 
