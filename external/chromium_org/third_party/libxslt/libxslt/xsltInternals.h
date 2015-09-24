/*
 * Summary: internal data structures, constants and functions
 * Description: Internal data structures, constants and functions used
 *              by the XSLT engine. 
 *              They are not part of the API or ABI, i.e. they can change
 *              without prior notice, use carefully.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLT_INTERNALS_H__
#define __XML_XSLT_INTERNALS_H__

#include <libxml/tree.h>
#include <libxml/hash.h>
#include <libxml/xpath.h>
#include <libxml/xmlerror.h>
#include <libxml/dict.h>
#include <libxml/xmlstring.h>
#include <libxslt/xslt.h>
#include "xsltexports.h"
#include "xsltlocale.h"
#include "numbersInternals.h"

#ifdef __cplusplus
extern "C" {
#endif


#define XSLT_IS_TEXT_NODE(n) ((n != NULL) && \
    (((n)->type == XML_TEXT_NODE) || \
     ((n)->type == XML_CDATA_SECTION_NODE)))


#define XSLT_MARK_RES_TREE_FRAG(n) \
    (n)->name = (char *) xmlStrdup(BAD_CAST " fake node libxslt");

#define XSLT_IS_RES_TREE_FRAG(n) \
    ((n != NULL) && ((n)->type == XML_DOCUMENT_NODE) && \
     ((n)->name != NULL) && ((n)->name[0] == ' '))

#define XSLT_REFACTORED_KEYCOMP

#define XSLT_FAST_IF


#define XSLT_REFACTORED_VARS

#ifdef XSLT_REFACTORED

extern const xmlChar *xsltXSLTAttrMarker;





#define XSLT_REFACTORED_XPATHCOMP

#ifdef XSLT_REFACTORED_XSLT_NSCOMP

extern const xmlChar *xsltConstNamespaceNameXSLT;

#define IS_XSLT_ELEM_FAST(n) \
    (((n) != NULL) && ((n)->ns != NULL) && \
    ((n)->ns->href == xsltConstNamespaceNameXSLT))

#define IS_XSLT_ATTR_FAST(a) \
    (((a) != NULL) && ((a)->ns != NULL) && \
    ((a)->ns->href == xsltConstNamespaceNameXSLT))

#define XSLT_HAS_INTERNAL_NSMAP(s) \
    (((s) != NULL) && ((s)->principal) && \
     ((s)->principal->principalData) && \
     ((s)->principal->principalData->nsMap))

#define XSLT_GET_INTERNAL_NSMAP(s) ((s)->principal->principalData->nsMap)

#else 

#define IS_XSLT_ELEM_FAST(n) \
    (((n) != NULL) && ((n)->ns != NULL) && \
     (xmlStrEqual((n)->ns->href, XSLT_NAMESPACE)))

#define IS_XSLT_ATTR_FAST(a) \
    (((a) != NULL) && ((a)->ns != NULL) && \
     (xmlStrEqual((a)->ns->href, XSLT_NAMESPACE)))


#endif 



typedef struct _xsltPointerList xsltPointerList;
typedef xsltPointerList *xsltPointerListPtr;
struct _xsltPointerList {
    void **items;
    int number;
    int size;
};

#endif


#define XSLT_MAX_SORT 15

#define XSLT_PAT_NO_PRIORITY -12345789

typedef struct _xsltRuntimeExtra xsltRuntimeExtra;
typedef xsltRuntimeExtra *xsltRuntimeExtraPtr;
struct _xsltRuntimeExtra {
    void       *info;		
    xmlFreeFunc deallocate;	
    union {			
        void   *ptr;		
	int    ival;		
    } val;
};

#define XSLT_RUNTIME_EXTRA_LST(ctxt, nr) (ctxt)->extras[(nr)].info
#define XSLT_RUNTIME_EXTRA_FREE(ctxt, nr) (ctxt)->extras[(nr)].deallocate
#define	XSLT_RUNTIME_EXTRA(ctxt, nr, typ) (ctxt)->extras[(nr)].val.typ

typedef struct _xsltTemplate xsltTemplate;
typedef xsltTemplate *xsltTemplatePtr;
struct _xsltTemplate {
    struct _xsltTemplate *next;
    struct _xsltStylesheet *style;
    xmlChar *match;	
    float priority;	
    const xmlChar *name; 
    const xmlChar *nameURI; 
    const xmlChar *mode;
    const xmlChar *modeURI;
    xmlNodePtr content;	
    xmlNodePtr elem;	

    int inheritedNsNr;  
    xmlNsPtr *inheritedNs;

    
    int nbCalls;        
    unsigned long time; 
    void *params;       
};

typedef struct _xsltDecimalFormat xsltDecimalFormat;
typedef xsltDecimalFormat *xsltDecimalFormatPtr;
struct _xsltDecimalFormat {
    struct _xsltDecimalFormat *next; 
    xmlChar *name;
    
    xmlChar *digit;
    xmlChar *patternSeparator;
    
    xmlChar *minusSign;
    xmlChar *infinity;
    xmlChar *noNumber; 
    
    xmlChar *decimalPoint;
    xmlChar *grouping;
    xmlChar *percent;
    xmlChar *permille;
    xmlChar *zeroDigit;
};

typedef struct _xsltDocument xsltDocument;
typedef xsltDocument *xsltDocumentPtr;
struct _xsltDocument {
    struct _xsltDocument *next;	
    int main;			
    xmlDocPtr doc;		
    void *keys;			
    struct _xsltDocument *includes; 
    int preproc;		
    int nbKeysComputed;
};

typedef struct _xsltKeyDef xsltKeyDef;
typedef xsltKeyDef *xsltKeyDefPtr;
struct _xsltKeyDef {
    struct _xsltKeyDef *next;
    xmlNodePtr inst;
    xmlChar *name;
    xmlChar *nameURI;
    xmlChar *match;
    xmlChar *use;
    xmlXPathCompExprPtr comp;
    xmlXPathCompExprPtr usecomp;
    xmlNsPtr *nsList;           
    int nsNr;                   
};

typedef struct _xsltKeyTable xsltKeyTable;
typedef xsltKeyTable *xsltKeyTablePtr;
struct _xsltKeyTable {
    struct _xsltKeyTable *next;
    xmlChar *name;
    xmlChar *nameURI;
    xmlHashTablePtr keys;
};

typedef struct _xsltStylesheet xsltStylesheet;
typedef xsltStylesheet *xsltStylesheetPtr;

typedef struct _xsltTransformContext xsltTransformContext;
typedef xsltTransformContext *xsltTransformContextPtr;

typedef struct _xsltElemPreComp xsltElemPreComp;
typedef xsltElemPreComp *xsltElemPreCompPtr;

typedef void (*xsltTransformFunction) (xsltTransformContextPtr ctxt,
	                               xmlNodePtr node,
				       xmlNodePtr inst,
			               xsltElemPreCompPtr comp);

typedef void (*xsltSortFunc) (xsltTransformContextPtr ctxt, xmlNodePtr *sorts,
			      int nbsorts);

typedef enum {
    XSLT_FUNC_COPY=1,
    XSLT_FUNC_SORT,
    XSLT_FUNC_TEXT,
    XSLT_FUNC_ELEMENT,
    XSLT_FUNC_ATTRIBUTE,
    XSLT_FUNC_COMMENT,
    XSLT_FUNC_PI,
    XSLT_FUNC_COPYOF,
    XSLT_FUNC_VALUEOF,
    XSLT_FUNC_NUMBER,
    XSLT_FUNC_APPLYIMPORTS,
    XSLT_FUNC_CALLTEMPLATE,
    XSLT_FUNC_APPLYTEMPLATES,
    XSLT_FUNC_CHOOSE,
    XSLT_FUNC_IF,
    XSLT_FUNC_FOREACH,
    XSLT_FUNC_DOCUMENT,
    XSLT_FUNC_WITHPARAM,
    XSLT_FUNC_PARAM,
    XSLT_FUNC_VARIABLE,
    XSLT_FUNC_WHEN,
    XSLT_FUNC_EXTENSION
#ifdef XSLT_REFACTORED
    ,
    XSLT_FUNC_OTHERWISE,
    XSLT_FUNC_FALLBACK,
    XSLT_FUNC_MESSAGE,
    XSLT_FUNC_INCLUDE,
    XSLT_FUNC_ATTRSET,
    XSLT_FUNC_LITERAL_RESULT_ELEMENT,
    XSLT_FUNC_UNKOWN_FORWARDS_COMPAT
#endif
} xsltStyleType;

typedef void (*xsltElemPreCompDeallocator) (xsltElemPreCompPtr comp);

struct _xsltElemPreComp {
    xsltElemPreCompPtr next;		
    xsltStyleType type;		
    xsltTransformFunction func; 	
    xmlNodePtr inst;			

    
    xsltElemPreCompDeallocator free;	
};

typedef struct _xsltStylePreComp xsltStylePreComp;
typedef xsltStylePreComp *xsltStylePreCompPtr;

#ifdef XSLT_REFACTORED

XSLTPUBFUN xsltPointerListPtr XSLTCALL
		xsltPointerListCreate		(int initialSize);
XSLTPUBFUN void XSLTCALL
		xsltPointerListFree		(xsltPointerListPtr list);
XSLTPUBFUN void XSLTCALL
		xsltPointerListClear		(xsltPointerListPtr list);
XSLTPUBFUN int XSLTCALL
		xsltPointerListAddSize		(xsltPointerListPtr list,						 
						 void *item,
						 int initialSize);


typedef struct _xsltNsListContainer xsltNsListContainer;
typedef xsltNsListContainer *xsltNsListContainerPtr;
struct _xsltNsListContainer {
    xmlNsPtr *list;
    int totalNumber;
    int xpathNumber;    
};

#define XSLT_ITEM_COMPATIBILITY_FIELDS \
    xsltElemPreCompPtr next;\
    xsltStyleType type;\
    xsltTransformFunction func;\
    xmlNodePtr inst;

#define XSLT_ITEM_NAVIGATION_FIELDS

#define XSLT_ITEM_NSINSCOPE_FIELDS xsltNsListContainerPtr inScopeNs;

#define XSLT_ITEM_COMMON_FIELDS \
    XSLT_ITEM_COMPATIBILITY_FIELDS \
    XSLT_ITEM_NAVIGATION_FIELDS \
    XSLT_ITEM_NSINSCOPE_FIELDS

struct _xsltStylePreComp {
    xsltElemPreCompPtr next;    
    xsltStyleType type;          
    xsltTransformFunction func; 
    xmlNodePtr inst;		
    
    xsltNsListContainerPtr inScopeNs;
};

typedef struct _xsltStyleBasicEmptyItem xsltStyleBasicEmptyItem;
typedef xsltStyleBasicEmptyItem *xsltStyleBasicEmptyItemPtr;

struct _xsltStyleBasicEmptyItem {
    XSLT_ITEM_COMMON_FIELDS
};

typedef struct _xsltStyleBasicExpressionItem xsltStyleBasicExpressionItem;
typedef xsltStyleBasicExpressionItem *xsltStyleBasicExpressionItemPtr;

struct _xsltStyleBasicExpressionItem {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *select; 
    xmlXPathCompExprPtr comp; 
};


typedef struct _xsltStyleItemElement xsltStyleItemElement;
typedef xsltStyleItemElement *xsltStyleItemElementPtr;

struct _xsltStyleItemElement {
    XSLT_ITEM_COMMON_FIELDS 

    const xmlChar *use;
    int      has_use;
    const xmlChar *name;    
    int      has_name;
    const xmlChar *ns;
    const xmlChar *nsPrefix;
    int      has_ns;
};

typedef struct _xsltStyleItemAttribute xsltStyleItemAttribute;
typedef xsltStyleItemAttribute *xsltStyleItemAttributePtr;

struct _xsltStyleItemAttribute {
    XSLT_ITEM_COMMON_FIELDS
    const xmlChar *name;
    int      has_name;
    const xmlChar *ns;
    const xmlChar *nsPrefix;
    int      has_ns;
};

typedef struct _xsltStyleItemText xsltStyleItemText;
typedef xsltStyleItemText *xsltStyleItemTextPtr;

struct _xsltStyleItemText {
    XSLT_ITEM_COMMON_FIELDS
    int      noescape;		
};

typedef xsltStyleBasicEmptyItem xsltStyleItemComment;
typedef xsltStyleItemComment *xsltStyleItemCommentPtr;

typedef struct _xsltStyleItemPI xsltStyleItemPI;
typedef xsltStyleItemPI *xsltStyleItemPIPtr;

struct _xsltStyleItemPI {
    XSLT_ITEM_COMMON_FIELDS
    const xmlChar *name;
    int      has_name;
};

typedef xsltStyleBasicEmptyItem xsltStyleItemApplyImports;
typedef xsltStyleItemApplyImports *xsltStyleItemApplyImportsPtr;

typedef struct _xsltStyleItemApplyTemplates xsltStyleItemApplyTemplates;
typedef xsltStyleItemApplyTemplates *xsltStyleItemApplyTemplatesPtr;

struct _xsltStyleItemApplyTemplates {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *mode;	
    const xmlChar *modeURI;	
    const xmlChar *select;	
    xmlXPathCompExprPtr comp;	
    
};

typedef struct _xsltStyleItemCallTemplate xsltStyleItemCallTemplate;
typedef xsltStyleItemCallTemplate *xsltStyleItemCallTemplatePtr;

struct _xsltStyleItemCallTemplate {
    XSLT_ITEM_COMMON_FIELDS

    xsltTemplatePtr templ;	
    const xmlChar *name;	
    int      has_name;		
    const xmlChar *ns;		
    int      has_ns;		
    
};

typedef struct _xsltStyleItemCopy xsltStyleItemCopy;
typedef xsltStyleItemCopy *xsltStyleItemCopyPtr;

struct _xsltStyleItemCopy {
   XSLT_ITEM_COMMON_FIELDS
    const xmlChar *use;		
    int      has_use;		    
};

typedef struct _xsltStyleItemIf xsltStyleItemIf;
typedef xsltStyleItemIf *xsltStyleItemIfPtr;

struct _xsltStyleItemIf {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *test;	
    xmlXPathCompExprPtr comp;	
};


typedef xsltStyleBasicExpressionItem xsltStyleItemCopyOf;
typedef xsltStyleItemCopyOf *xsltStyleItemCopyOfPtr;

typedef struct _xsltStyleItemValueOf xsltStyleItemValueOf;
typedef xsltStyleItemValueOf *xsltStyleItemValueOfPtr;

struct _xsltStyleItemValueOf {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *select;
    xmlXPathCompExprPtr comp;	
    int      noescape;
};

typedef struct _xsltStyleItemNumber xsltStyleItemNumber;
typedef xsltStyleItemNumber *xsltStyleItemNumberPtr;

struct _xsltStyleItemNumber {
    XSLT_ITEM_COMMON_FIELDS
    xsltNumberData numdata;	
};

typedef xsltStyleBasicEmptyItem xsltStyleItemChoose;
typedef xsltStyleItemChoose *xsltStyleItemChoosePtr;

typedef xsltStyleBasicEmptyItem xsltStyleItemFallback;
typedef xsltStyleItemFallback *xsltStyleItemFallbackPtr;

typedef xsltStyleBasicExpressionItem xsltStyleItemForEach;
typedef xsltStyleItemForEach *xsltStyleItemForEachPtr;

typedef struct _xsltStyleItemMessage xsltStyleItemMessage;
typedef xsltStyleItemMessage *xsltStyleItemMessagePtr;

struct _xsltStyleItemMessage {
    XSLT_ITEM_COMMON_FIELDS    
    int terminate;
};

typedef struct _xsltStyleItemDocument xsltStyleItemDocument;
typedef xsltStyleItemDocument *xsltStyleItemDocumentPtr;

struct _xsltStyleItemDocument {
    XSLT_ITEM_COMMON_FIELDS
    int      ver11;		
    const xmlChar *filename;	
    int has_filename;
};   


typedef struct _xsltStyleBasicItemVariable xsltStyleBasicItemVariable;
typedef xsltStyleBasicItemVariable *xsltStyleBasicItemVariablePtr;

struct _xsltStyleBasicItemVariable {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *select;
    xmlXPathCompExprPtr comp;

    const xmlChar *name;
    int      has_name;
    const xmlChar *ns;
    int      has_ns;
};

typedef xsltStyleBasicItemVariable xsltStyleItemVariable;
typedef xsltStyleItemVariable *xsltStyleItemVariablePtr;

typedef struct _xsltStyleItemParam xsltStyleItemParam;
typedef xsltStyleItemParam *xsltStyleItemParamPtr;

struct _xsltStyleItemParam {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *select;
    xmlXPathCompExprPtr comp;

    const xmlChar *name;
    int      has_name;
    const xmlChar *ns;
    int      has_ns;    
};

typedef xsltStyleBasicItemVariable xsltStyleItemWithParam;
typedef xsltStyleItemWithParam *xsltStyleItemWithParamPtr;

typedef struct _xsltStyleItemSort xsltStyleItemSort;
typedef xsltStyleItemSort *xsltStyleItemSortPtr;

struct _xsltStyleItemSort {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *stype;       
    int      has_stype;		
    int      number;		
    const xmlChar *order;	
    int      has_order;		
    int      descending;	
    const xmlChar *lang;	
    int      has_lang;		
    xsltLocale locale;		
    const xmlChar *case_order;	
    int      lower_first;	

    const xmlChar *use;
    int      has_use;

    const xmlChar *select;	

    xmlXPathCompExprPtr comp;	
};


typedef struct _xsltStyleItemWhen xsltStyleItemWhen;
typedef xsltStyleItemWhen *xsltStyleItemWhenPtr;

struct _xsltStyleItemWhen {
    XSLT_ITEM_COMMON_FIELDS

    const xmlChar *test;
    xmlXPathCompExprPtr comp;
};

typedef struct _xsltStyleItemOtherwise xsltStyleItemOtherwise;
typedef xsltStyleItemOtherwise *xsltStyleItemOtherwisePtr;

struct _xsltStyleItemOtherwise {
    XSLT_ITEM_COMMON_FIELDS
};

typedef struct _xsltStyleItemInclude xsltStyleItemInclude;
typedef xsltStyleItemInclude *xsltStyleItemIncludePtr;

struct _xsltStyleItemInclude {
    XSLT_ITEM_COMMON_FIELDS
    xsltDocumentPtr include;
};


typedef struct _xsltStyleItemUknown xsltStyleItemUknown;
typedef xsltStyleItemUknown *xsltStyleItemUknownPtr;
struct _xsltStyleItemUknown {
    XSLT_ITEM_COMMON_FIELDS
};



typedef struct _xsltStyleItemExtElement xsltStyleItemExtElement;
typedef xsltStyleItemExtElement *xsltStyleItemExtElementPtr;
struct _xsltStyleItemExtElement {
    XSLT_ITEM_COMMON_FIELDS
    xsltElemPreCompPtr item;   
};


typedef struct _xsltEffectiveNs xsltEffectiveNs;
typedef xsltEffectiveNs *xsltEffectiveNsPtr;
struct _xsltEffectiveNs {
    xsltEffectiveNsPtr nextInStore; 
    xsltEffectiveNsPtr next; 
    const xmlChar *prefix;
    const xmlChar *nsName;
    int holdByElem;
};

typedef struct _xsltStyleItemLRElementInfo xsltStyleItemLRElementInfo;
typedef xsltStyleItemLRElementInfo *xsltStyleItemLRElementInfoPtr;
struct _xsltStyleItemLRElementInfo {
    XSLT_ITEM_COMMON_FIELDS
    xsltEffectiveNsPtr effectiveNs;

};

#ifdef XSLT_REFACTORED

typedef struct _xsltNsAlias xsltNsAlias;
typedef xsltNsAlias *xsltNsAliasPtr;
struct _xsltNsAlias {
    xsltNsAliasPtr next;     
    xmlNsPtr literalNs;
    xmlNsPtr targetNs;
    xmlDocPtr docOfTargetNs;
};
#endif

#ifdef XSLT_REFACTORED_XSLT_NSCOMP

typedef struct _xsltNsMap xsltNsMap;
typedef xsltNsMap *xsltNsMapPtr;
struct _xsltNsMap {
    xsltNsMapPtr next; 
    xmlDocPtr doc;
    xmlNodePtr elem; 
    xmlNsPtr ns; 
    const xmlChar *origNsName; 
    const xmlChar *newNsName;     
};
#endif


typedef struct _xsltPrincipalStylesheetData xsltPrincipalStylesheetData;
typedef xsltPrincipalStylesheetData *xsltPrincipalStylesheetDataPtr;

typedef struct _xsltNsList xsltNsList;
typedef xsltNsList *xsltNsListPtr;
struct _xsltNsList {
    xsltNsListPtr next; 
    xmlNsPtr ns;
};

typedef struct _xsltVarInfo xsltVarInfo;
typedef xsltVarInfo *xsltVarInfoPtr;
struct _xsltVarInfo {
    xsltVarInfoPtr next; 
    xsltVarInfoPtr prev;
    int depth; 
    const xmlChar *name;
    const xmlChar *nsName;
};

typedef struct _xsltCompilerNodeInfo xsltCompilerNodeInfo;
typedef xsltCompilerNodeInfo *xsltCompilerNodeInfoPtr;
struct _xsltCompilerNodeInfo {
    xsltCompilerNodeInfoPtr next;
    xsltCompilerNodeInfoPtr prev;
    xmlNodePtr node;
    int depth;
    xsltTemplatePtr templ;   
    int category;	     
    xsltStyleType type;
    xsltElemPreCompPtr item; 
    
    xsltNsListContainerPtr inScopeNs;
    
    xsltPointerListPtr exclResultNs; 
    
    xsltPointerListPtr extElemNs;

    
    xsltStyleItemLRElementInfoPtr litResElemInfo;
    int nsChanged;
    int preserveWhitespace;
    int stripWhitespace;
    int isRoot; 
    int forwardsCompat; 
    
    int extContentHandled;
    
    xsltStyleType curChildType;    
};

#define XSLT_CCTXT(style) ((xsltCompilerCtxtPtr) style->compCtxt) 

typedef enum {
    XSLT_ERROR_SEVERITY_ERROR = 0,
    XSLT_ERROR_SEVERITY_WARNING
} xsltErrorSeverityType;

typedef struct _xsltCompilerCtxt xsltCompilerCtxt;
typedef xsltCompilerCtxt *xsltCompilerCtxtPtr;
struct _xsltCompilerCtxt {
    void *errorCtxt;            
    xsltErrorSeverityType errSeverity;		
    int warnings;		
    int errors;			
    xmlDictPtr dict;
    xsltStylesheetPtr style;
    int simplified; 
    
    int depth; 
    
    xsltCompilerNodeInfoPtr inode;
    xsltCompilerNodeInfoPtr inodeList;
    xsltCompilerNodeInfoPtr inodeLast;
    xsltPointerListPtr tmpList; 
    int isInclude;
    int hasForwardsCompat; 
    int maxNodeInfos; 
    int maxLREs;  
    int strict;
    xsltPrincipalStylesheetDataPtr psData;
#ifdef XSLT_REFACTORED_XPATHCOMP
    xmlXPathContextPtr xpathCtxt;
#endif
    xsltStyleItemUknownPtr unknownItem;
    int hasNsAliases; 
    xsltNsAliasPtr nsAliases;
    xsltVarInfoPtr ivars; 
    xsltVarInfoPtr ivar; 
};   

#else 

struct _xsltStylePreComp {
    xsltElemPreCompPtr next;	
    xsltStyleType type;		
    xsltTransformFunction func; 
    xmlNodePtr inst;		


    const xmlChar *stype;       
    int      has_stype;		
    int      number;		
    const xmlChar *order;	
    int      has_order;		
    int      descending;	
    const xmlChar *lang;	
    int      has_lang;		
    xsltLocale locale;		
    const xmlChar *case_order;	
    int      lower_first;	

    const xmlChar *use;		
    int      has_use;		

    int      noescape;		

    const xmlChar *name;	
    int      has_name;		
    const xmlChar *ns;		
    int      has_ns;		

    const xmlChar *mode;	
    const xmlChar *modeURI;	

    const xmlChar *test;	

    xsltTemplatePtr templ;	

    const xmlChar *select;	

    int      ver11;		
    const xmlChar *filename;	
    int      has_filename;	

    xsltNumberData numdata;	

    xmlXPathCompExprPtr comp;	
    xmlNsPtr *nsList;		
    int nsNr;			
};

#endif 


typedef struct _xsltStackElem xsltStackElem;
typedef xsltStackElem *xsltStackElemPtr;
struct _xsltStackElem {
    struct _xsltStackElem *next;
    xsltStylePreCompPtr comp;   
    int computed;		
    const xmlChar *name;	
    const xmlChar *nameURI;	
    const xmlChar *select;	
    xmlNodePtr tree;		
    xmlXPathObjectPtr value;	
    xmlDocPtr fragment;		
    int level;                  
    xsltTransformContextPtr context; 
    int flags;
};

#ifdef XSLT_REFACTORED

struct _xsltPrincipalStylesheetData {
    xmlDictPtr namespaceDict;
    xsltPointerListPtr inScopeNamespaces;
    xsltPointerListPtr exclResultNamespaces;
    xsltPointerListPtr extElemNamespaces;
    xsltEffectiveNsPtr effectiveNs;
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
    xsltNsMapPtr nsMap;
#endif
};

    
#endif
struct _xsltStylesheet {
    struct _xsltStylesheet *parent;
    struct _xsltStylesheet *next;
    struct _xsltStylesheet *imports;

    xsltDocumentPtr docList;		

    xmlDocPtr doc;		
    xmlHashTablePtr stripSpaces;
    int             stripAll;	
    xmlHashTablePtr cdataSection;

    xsltStackElemPtr variables; 

    xsltTemplatePtr templates;	
    void *templatesHash;	
    void *rootMatch;		
    void *keyMatch;		
    void *elemMatch;		
    void *attrMatch;		
    void *parentMatch;		
    void *textMatch;		
    void *piMatch;		
    void *commentMatch;		
    
    xmlHashTablePtr nsAliases;	

    xmlHashTablePtr attributeSets;

    xmlHashTablePtr nsHash;     
    void           *nsDefs;     

    void *keys;			

    xmlChar *method;		
    xmlChar *methodURI;		
    xmlChar *version;		
    xmlChar *encoding;		
    int omitXmlDeclaration;     

    xsltDecimalFormatPtr decimalFormat;
    int standalone;             
    xmlChar *doctypePublic;     
    xmlChar *doctypeSystem;     
    int indent;			
    xmlChar *mediaType;		

    xsltElemPreCompPtr preComps;
    int warnings;		
    int errors;			

    xmlChar  *exclPrefix;	
    xmlChar **exclPrefixTab;	
    int       exclPrefixNr;	
    int       exclPrefixMax;	

    void     *_private;		

    xmlHashTablePtr extInfos;	
    int		    extrasNr;	

    xsltDocumentPtr includes;	

    xmlDictPtr dict;
    void *attVTs;
    const xmlChar *defaultAlias;
    int nopreproc;
    int internalized;
    int literal_result;
    xsltStylesheetPtr principal;
#ifdef XSLT_REFACTORED
    xsltCompilerCtxtPtr compCtxt; 

    xsltPrincipalStylesheetDataPtr principalData;    
#endif
};

typedef struct _xsltTransformCache xsltTransformCache;
typedef xsltTransformCache *xsltTransformCachePtr;
struct _xsltTransformCache {
    xmlDocPtr RVT;
    int nbRVT;
    xsltStackElemPtr stackItems;
    int nbStackItems;
#ifdef XSLT_DEBUG_PROFILE_CACHE
    int dbgCachedRVTs;
    int dbgReusedRVTs;
    int dbgCachedVars;
    int dbgReusedVars;
#endif
};

typedef enum {
    XSLT_OUTPUT_XML = 0,
    XSLT_OUTPUT_HTML,
    XSLT_OUTPUT_TEXT
} xsltOutputType;

typedef enum {
    XSLT_STATE_OK = 0,
    XSLT_STATE_ERROR,
    XSLT_STATE_STOPPED
} xsltTransformState;

struct _xsltTransformContext {
    xsltStylesheetPtr style;		
    xsltOutputType type;		

    xsltTemplatePtr  templ;		
    int              templNr;		
    int              templMax;		
    xsltTemplatePtr *templTab;		

    xsltStackElemPtr  vars;		
    int               varsNr;		
    int               varsMax;		
    xsltStackElemPtr *varsTab;		
    int               varsBase;		

    xmlHashTablePtr   extFunctions;	
    xmlHashTablePtr   extElements;	
    xmlHashTablePtr   extInfos;		

    const xmlChar *mode;		
    const xmlChar *modeURI;		

    xsltDocumentPtr docList;		

    xsltDocumentPtr document;		
    xmlNodePtr node;			
    xmlNodeSetPtr nodeList;		
    

    xmlDocPtr output;			
    xmlNodePtr insert;			

    xmlXPathContextPtr xpathCtxt;	
    xsltTransformState state;		

    xmlHashTablePtr   globalVars;	

    xmlNodePtr inst;			

    int xinclude;			

    const char *      outputFile;	

    int profile;                        
    long             prof;		
    int              profNr;		
    int              profMax;		
    long            *profTab;		

    void            *_private;		

    int              extrasNr;		
    int              extrasMax;		
    xsltRuntimeExtraPtr extras;		

    xsltDocumentPtr  styleList;		
    void                 * sec;		

    xmlGenericErrorFunc  error;		
    void              * errctx;		

    xsltSortFunc      sortfunc;		

    xmlDocPtr       tmpRVT;		
    xmlDocPtr       persistRVT;		
    int             ctxtflags;          

    const xmlChar  *lasttext;		
    unsigned int    lasttsize;		
    unsigned int    lasttuse;		
    int debugStatus;			
    unsigned long* traceCode;		

    int parserOptions;			

    xmlDictPtr dict;
    xmlDocPtr		tmpDoc; 
    int internalized;
    int nbKeys;
    int hasTemplKeyPatterns;
    xsltTemplatePtr currentTemplateRule; 
    xmlNodePtr initialContextNode;
    xmlDocPtr initialContextDoc;
    xsltTransformCachePtr cache;
    void *contextVariable; 
    xmlDocPtr localRVT; 
    xmlDocPtr localRVTBase;
    int keyInitLevel;   
    int funcLevel;      
};

#define CHECK_STOPPED if (ctxt->state == XSLT_STATE_STOPPED) return;

#define CHECK_STOPPEDE if (ctxt->state == XSLT_STATE_STOPPED) goto error;

#define CHECK_STOPPED0 if (ctxt->state == XSLT_STATE_STOPPED) return(0);

#ifndef XML_CAST_FPTR

#define XML_CAST_FPTR(fptr) fptr
#endif
XSLTPUBFUN xsltStylesheetPtr XSLTCALL	
			xsltNewStylesheet	(void);
XSLTPUBFUN xsltStylesheetPtr XSLTCALL	
			xsltParseStylesheetFile	(const xmlChar* filename);
XSLTPUBFUN void XSLTCALL			
			xsltFreeStylesheet	(xsltStylesheetPtr style);
XSLTPUBFUN int XSLTCALL			
			xsltIsBlank		(xmlChar *str);
XSLTPUBFUN void XSLTCALL			
			xsltFreeStackElemList	(xsltStackElemPtr elem);
XSLTPUBFUN xsltDecimalFormatPtr XSLTCALL	
			xsltDecimalFormatGetByName(xsltStylesheetPtr style,
						 xmlChar *name);

XSLTPUBFUN xsltStylesheetPtr XSLTCALL	
			xsltParseStylesheetProcess(xsltStylesheetPtr ret,
						 xmlDocPtr doc);
XSLTPUBFUN void XSLTCALL			
			xsltParseStylesheetOutput(xsltStylesheetPtr style,
						 xmlNodePtr cur);
XSLTPUBFUN xsltStylesheetPtr XSLTCALL	
			xsltParseStylesheetDoc	(xmlDocPtr doc);
XSLTPUBFUN xsltStylesheetPtr XSLTCALL	
			xsltParseStylesheetImportedDoc(xmlDocPtr doc,
						xsltStylesheetPtr style);
XSLTPUBFUN xsltStylesheetPtr XSLTCALL	
			xsltLoadStylesheetPI	(xmlDocPtr doc);
XSLTPUBFUN void XSLTCALL 			
			xsltNumberFormat	(xsltTransformContextPtr ctxt,
						 xsltNumberDataPtr data,
						 xmlNodePtr node);
XSLTPUBFUN xmlXPathError XSLTCALL		 
			xsltFormatNumberConversion(xsltDecimalFormatPtr self,
						 xmlChar *format,
						 double number,
						 xmlChar **result);

XSLTPUBFUN void XSLTCALL			
			xsltParseTemplateContent(xsltStylesheetPtr style,
						 xmlNodePtr templ);
XSLTPUBFUN int XSLTCALL			
			xsltAllocateExtra	(xsltStylesheetPtr style);
XSLTPUBFUN int XSLTCALL			
			xsltAllocateExtraCtxt	(xsltTransformContextPtr ctxt);
XSLTPUBFUN xmlDocPtr XSLTCALL		
			xsltCreateRVT		(xsltTransformContextPtr ctxt);
XSLTPUBFUN int XSLTCALL			
			xsltRegisterTmpRVT	(xsltTransformContextPtr ctxt,
						 xmlDocPtr RVT);
XSLTPUBFUN int XSLTCALL			
			xsltRegisterLocalRVT	(xsltTransformContextPtr ctxt,
						 xmlDocPtr RVT);
XSLTPUBFUN int XSLTCALL			
			xsltRegisterPersistRVT	(xsltTransformContextPtr ctxt,
						 xmlDocPtr RVT);
XSLTPUBFUN int XSLTCALL
			xsltExtensionInstructionResultRegister(
						 xsltTransformContextPtr ctxt,
						 xmlXPathObjectPtr obj);
XSLTPUBFUN int XSLTCALL
			xsltExtensionInstructionResultFinalize(
						 xsltTransformContextPtr ctxt);
XSLTPUBFUN void XSLTCALL
			xsltFreeRVTs		(xsltTransformContextPtr ctxt);
XSLTPUBFUN void XSLTCALL
			xsltReleaseRVT		(xsltTransformContextPtr ctxt,
						 xmlDocPtr RVT);
XSLTPUBFUN int XSLTCALL
			xsltTransStorageAdd	(xsltTransformContextPtr ctxt,
						 void *id,
						 void *data);
XSLTPUBFUN void * XSLTCALL
			xsltTransStorageRemove	(xsltTransformContextPtr ctxt,
						 void *id);

XSLTPUBFUN void XSLTCALL
			xsltCompileAttr		(xsltStylesheetPtr style,
						 xmlAttrPtr attr);
XSLTPUBFUN xmlChar * XSLTCALL
			xsltEvalAVT		(xsltTransformContextPtr ctxt,
						 void *avt,
						 xmlNodePtr node);
XSLTPUBFUN void XSLTCALL
			xsltFreeAVTList		(void *avt);


XSLTPUBFUN void XSLTCALL
			xsltUninit		(void);


#ifdef XSLT_REFACTORED  
XSLTPUBFUN void XSLTCALL
			xsltParseSequenceConstructor(
						 xsltCompilerCtxtPtr cctxt,
						 xmlNodePtr start);
XSLTPUBFUN int XSLTCALL
			xsltParseAnyXSLTElem	(xsltCompilerCtxtPtr cctxt,
						 xmlNodePtr elem);
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
XSLTPUBFUN int XSLTCALL
			xsltRestoreDocumentNamespaces(
						 xsltNsMapPtr ns,
						 xmlDocPtr doc);
#endif
#endif 

XSLTPUBFUN int XSLTCALL
			xsltInitCtxtKey		(xsltTransformContextPtr ctxt,
						 xsltDocumentPtr doc,
						 xsltKeyDefPtr keyd);
XSLTPUBFUN int XSLTCALL
			xsltInitAllDocKeys	(xsltTransformContextPtr ctxt);
#ifdef __cplusplus
}
#endif

#endif 

