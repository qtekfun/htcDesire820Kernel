/*
 * Summary: XML Path Language implementation
 * Description: API for the XML Path Language implementation
 *
 * XML Path Language implementation
 * XPath is a language for addressing parts of an XML document,
 * designed to be used by both XSLT and XPointer
 *     http://www.w3.org/TR/xpath
 *
 * Implements
 * W3C Recommendation 16 November 1999
 *     http://www.w3.org/TR/1999/REC-xpath-19991116
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XPATH_H__
#define __XML_XPATH_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_XPATH_ENABLED

#include <libxml/xmlerror.h>
#include <libxml/tree.h>
#include <libxml/hash.h>
#endif 

#if defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
#ifdef __cplusplus
extern "C" {
#endif
#endif 
	
#ifdef LIBXML_XPATH_ENABLED

typedef struct _xmlXPathContext xmlXPathContext;
typedef xmlXPathContext *xmlXPathContextPtr;
typedef struct _xmlXPathParserContext xmlXPathParserContext;
typedef xmlXPathParserContext *xmlXPathParserContextPtr;


typedef enum {
    XPATH_EXPRESSION_OK = 0,
    XPATH_NUMBER_ERROR,
    XPATH_UNFINISHED_LITERAL_ERROR,
    XPATH_START_LITERAL_ERROR,
    XPATH_VARIABLE_REF_ERROR,
    XPATH_UNDEF_VARIABLE_ERROR,
    XPATH_INVALID_PREDICATE_ERROR,
    XPATH_EXPR_ERROR,
    XPATH_UNCLOSED_ERROR,
    XPATH_UNKNOWN_FUNC_ERROR,
    XPATH_INVALID_OPERAND,
    XPATH_INVALID_TYPE,
    XPATH_INVALID_ARITY,
    XPATH_INVALID_CTXT_SIZE,
    XPATH_INVALID_CTXT_POSITION,
    XPATH_MEMORY_ERROR,
    XPTR_SYNTAX_ERROR,
    XPTR_RESOURCE_ERROR,
    XPTR_SUB_RESOURCE_ERROR,
    XPATH_UNDEF_PREFIX_ERROR,
    XPATH_ENCODING_ERROR,
    XPATH_INVALID_CHAR_ERROR,
    XPATH_INVALID_CTXT
} xmlXPathError;

typedef struct _xmlNodeSet xmlNodeSet;
typedef xmlNodeSet *xmlNodeSetPtr;
struct _xmlNodeSet {
    int nodeNr;			
    int nodeMax;		
    xmlNodePtr *nodeTab;	
    
};


typedef enum {
    XPATH_UNDEFINED = 0,
    XPATH_NODESET = 1,
    XPATH_BOOLEAN = 2,
    XPATH_NUMBER = 3,
    XPATH_STRING = 4,
    XPATH_POINT = 5,
    XPATH_RANGE = 6,
    XPATH_LOCATIONSET = 7,
    XPATH_USERS = 8,
    XPATH_XSLT_TREE = 9  
} xmlXPathObjectType;

typedef struct _xmlXPathObject xmlXPathObject;
typedef xmlXPathObject *xmlXPathObjectPtr;
struct _xmlXPathObject {
    xmlXPathObjectType type;
    xmlNodeSetPtr nodesetval;
    int boolval;
    double floatval;
    xmlChar *stringval;
    void *user;
    int index;
    void *user2;
    int index2;
};

typedef int (*xmlXPathConvertFunc) (xmlXPathObjectPtr obj, int type);


typedef struct _xmlXPathType xmlXPathType;
typedef xmlXPathType *xmlXPathTypePtr;
struct _xmlXPathType {
    const xmlChar         *name;		
    xmlXPathConvertFunc func;		
};


typedef struct _xmlXPathVariable xmlXPathVariable;
typedef xmlXPathVariable *xmlXPathVariablePtr;
struct _xmlXPathVariable {
    const xmlChar       *name;		
    xmlXPathObjectPtr value;		
};


typedef void (*xmlXPathEvalFunc)(xmlXPathParserContextPtr ctxt,
	                         int nargs);


typedef struct _xmlXPathFunct xmlXPathFunct;
typedef xmlXPathFunct *xmlXPathFuncPtr;
struct _xmlXPathFunct {
    const xmlChar      *name;		
    xmlXPathEvalFunc func;		
};


typedef xmlXPathObjectPtr (*xmlXPathAxisFunc) (xmlXPathParserContextPtr ctxt,
				 xmlXPathObjectPtr cur);


typedef struct _xmlXPathAxis xmlXPathAxis;
typedef xmlXPathAxis *xmlXPathAxisPtr;
struct _xmlXPathAxis {
    const xmlChar      *name;		
    xmlXPathAxisFunc func;		
};


typedef void (*xmlXPathFunction) (xmlXPathParserContextPtr ctxt, int nargs);


typedef xmlXPathObjectPtr (*xmlXPathVariableLookupFunc) (void *ctxt,
                                         const xmlChar *name,
                                         const xmlChar *ns_uri);

typedef xmlXPathFunction (*xmlXPathFuncLookupFunc) (void *ctxt,
					 const xmlChar *name,
					 const xmlChar *ns_uri);

#define XML_XPATH_CHECKNS (1<<0)
#define XML_XPATH_NOVAR	  (1<<1)


struct _xmlXPathContext {
    xmlDocPtr doc;			
    xmlNodePtr node;			

    int nb_variables_unused;		
    int max_variables_unused;		
    xmlHashTablePtr varHash;		

    int nb_types;			
    int max_types;			
    xmlXPathTypePtr types;		

    int nb_funcs_unused;		
    int max_funcs_unused;		
    xmlHashTablePtr funcHash;		

    int nb_axis;			
    int max_axis;			
    xmlXPathAxisPtr axis;		

    
    xmlNsPtr *namespaces;		
    int nsNr;				
    void *user;				

    
    int contextSize;			
    int proximityPosition;		

    
    int xptr;				
    xmlNodePtr here;			
    xmlNodePtr origin;			

    
    xmlHashTablePtr nsHash;		
    xmlXPathVariableLookupFunc varLookupFunc;
    void *varLookupData;		

    
    void *extra;                        

    
    const xmlChar *function;
    const xmlChar *functionURI;

    
    xmlXPathFuncLookupFunc funcLookupFunc;
    void *funcLookupData;		

    
    xmlNsPtr *tmpNsList;		
    int tmpNsNr;			

    
    void *userData;                     
    xmlStructuredErrorFunc error;       
    xmlError lastError;			
    xmlNodePtr debugNode;		

    
    xmlDictPtr dict;			

    int flags;				

    
    void *cache;
};


typedef struct _xmlXPathCompExpr xmlXPathCompExpr;
typedef xmlXPathCompExpr *xmlXPathCompExprPtr;

struct _xmlXPathParserContext {
    const xmlChar *cur;			
    const xmlChar *base;			

    int error;				

    xmlXPathContextPtr  context;	
    xmlXPathObjectPtr     value;	
    int                 valueNr;	
    int                valueMax;	
    xmlXPathObjectPtr *valueTab;	

    xmlXPathCompExprPtr comp;		
    int xptr;				
    xmlNodePtr         ancestor;	
};



XMLPUBVAR double xmlXPathNAN;
XMLPUBVAR double xmlXPathPINF;
XMLPUBVAR double xmlXPathNINF;

#define xmlXPathNodeSetGetLength(ns) ((ns) ? (ns)->nodeNr : 0)
#define xmlXPathNodeSetItem(ns, index)				\
		((((ns) != NULL) && 				\
		  ((index) >= 0) && ((index) < (ns)->nodeNr)) ?	\
		 (ns)->nodeTab[(index)]				\
		 : NULL)
#define xmlXPathNodeSetIsEmpty(ns)                                      \
    (((ns) == NULL) || ((ns)->nodeNr == 0) || ((ns)->nodeTab == NULL))


XMLPUBFUN void XMLCALL		   
		    xmlXPathFreeObject		(xmlXPathObjectPtr obj);
XMLPUBFUN xmlNodeSetPtr XMLCALL	   
		    xmlXPathNodeSetCreate	(xmlNodePtr val);
XMLPUBFUN void XMLCALL		   
		    xmlXPathFreeNodeSetList	(xmlXPathObjectPtr obj);
XMLPUBFUN void XMLCALL		   
		    xmlXPathFreeNodeSet		(xmlNodeSetPtr obj);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		    xmlXPathObjectCopy		(xmlXPathObjectPtr val);
XMLPUBFUN int XMLCALL		   
		    xmlXPathCmpNodes		(xmlNodePtr node1,
						 xmlNodePtr node2);
XMLPUBFUN int XMLCALL		   
		    xmlXPathCastNumberToBoolean	(double val);
XMLPUBFUN int XMLCALL		   
		    xmlXPathCastStringToBoolean	(const xmlChar * val);
XMLPUBFUN int XMLCALL		   
		    xmlXPathCastNodeSetToBoolean(xmlNodeSetPtr ns);
XMLPUBFUN int XMLCALL		   
		    xmlXPathCastToBoolean	(xmlXPathObjectPtr val);

XMLPUBFUN double XMLCALL		   
		    xmlXPathCastBooleanToNumber	(int val);
XMLPUBFUN double XMLCALL		   
		    xmlXPathCastStringToNumber	(const xmlChar * val);
XMLPUBFUN double XMLCALL		   
		    xmlXPathCastNodeToNumber	(xmlNodePtr node);
XMLPUBFUN double XMLCALL		   
		    xmlXPathCastNodeSetToNumber	(xmlNodeSetPtr ns);
XMLPUBFUN double XMLCALL		   
		    xmlXPathCastToNumber	(xmlXPathObjectPtr val);

XMLPUBFUN xmlChar * XMLCALL	   
		    xmlXPathCastBooleanToString	(int val);
XMLPUBFUN xmlChar * XMLCALL	   
		    xmlXPathCastNumberToString	(double val);
XMLPUBFUN xmlChar * XMLCALL	   
		    xmlXPathCastNodeToString	(xmlNodePtr node);
XMLPUBFUN xmlChar * XMLCALL	   
		    xmlXPathCastNodeSetToString	(xmlNodeSetPtr ns);
XMLPUBFUN xmlChar * XMLCALL	   
		    xmlXPathCastToString	(xmlXPathObjectPtr val);

XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		    xmlXPathConvertBoolean	(xmlXPathObjectPtr val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		    xmlXPathConvertNumber	(xmlXPathObjectPtr val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		    xmlXPathConvertString	(xmlXPathObjectPtr val);

XMLPUBFUN xmlXPathContextPtr XMLCALL 
		    xmlXPathNewContext		(xmlDocPtr doc);
XMLPUBFUN void XMLCALL
		    xmlXPathFreeContext		(xmlXPathContextPtr ctxt);
XMLPUBFUN int XMLCALL
		    xmlXPathContextSetCache(xmlXPathContextPtr ctxt,
				            int active,
					    int value,
					    int options);
XMLPUBFUN long XMLCALL               
		    xmlXPathOrderDocElems	(xmlDocPtr doc);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		    xmlXPathEval		(const xmlChar *str,
						 xmlXPathContextPtr ctx);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		    xmlXPathEvalExpression	(const xmlChar *str,
						 xmlXPathContextPtr ctxt);
XMLPUBFUN int XMLCALL                
		    xmlXPathEvalPredicate	(xmlXPathContextPtr ctxt,
						 xmlXPathObjectPtr res);
XMLPUBFUN xmlXPathCompExprPtr XMLCALL 
		    xmlXPathCompile		(const xmlChar *str);
XMLPUBFUN xmlXPathCompExprPtr XMLCALL 
		    xmlXPathCtxtCompile		(xmlXPathContextPtr ctxt,
		    				 const xmlChar *str);
XMLPUBFUN xmlXPathObjectPtr XMLCALL   
		    xmlXPathCompiledEval	(xmlXPathCompExprPtr comp,
						 xmlXPathContextPtr ctx);
XMLPUBFUN int XMLCALL   
		    xmlXPathCompiledEvalToBoolean(xmlXPathCompExprPtr comp,
						 xmlXPathContextPtr ctxt);
XMLPUBFUN void XMLCALL                
		    xmlXPathFreeCompExpr	(xmlXPathCompExprPtr comp);
#endif 
#if defined(LIBXML_XPATH_ENABLED) || defined(LIBXML_SCHEMAS_ENABLED)
XMLPUBFUN void XMLCALL		   
		    xmlXPathInit		(void);
XMLPUBFUN int XMLCALL
		xmlXPathIsNaN	(double val);
XMLPUBFUN int XMLCALL
		xmlXPathIsInf	(double val);

#ifdef __cplusplus
}
#endif

#endif 
#endif 
