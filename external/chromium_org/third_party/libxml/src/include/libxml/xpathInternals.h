/*
 * Summary: internal interfaces for XML Path Language implementation
 * Description: internal interfaces for XML Path Language implementation
 *              used to build new modules on top of XPath like XPointer and
 *              XSLT
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XPATH_INTERNALS_H__
#define __XML_XPATH_INTERNALS_H__

#include <libxml/xmlversion.h>
#include <libxml/xpath.h>

#ifdef LIBXML_XPATH_ENABLED

#ifdef __cplusplus
extern "C" {
#endif


#define xmlXPathSetError(ctxt, err)					\
    { xmlXPatherror((ctxt), __FILE__, __LINE__, (err));			\
      if ((ctxt) != NULL) (ctxt)->error = (err); }

#define xmlXPathSetArityError(ctxt)					\
    xmlXPathSetError((ctxt), XPATH_INVALID_ARITY)

#define xmlXPathSetTypeError(ctxt)					\
    xmlXPathSetError((ctxt), XPATH_INVALID_TYPE)

#define xmlXPathGetError(ctxt)	  ((ctxt)->error)

#define xmlXPathCheckError(ctxt)  ((ctxt)->error != XPATH_EXPRESSION_OK)

#define xmlXPathGetDocument(ctxt)	((ctxt)->context->doc)

#define xmlXPathGetContextNode(ctxt)	((ctxt)->context->node)

XMLPUBFUN int XMLCALL		
		xmlXPathPopBoolean	(xmlXPathParserContextPtr ctxt);
XMLPUBFUN double XMLCALL		
    		xmlXPathPopNumber	(xmlXPathParserContextPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL	
    		xmlXPathPopString	(xmlXPathParserContextPtr ctxt);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
    		xmlXPathPopNodeSet	(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void * XMLCALL		
    		xmlXPathPopExternal	(xmlXPathParserContextPtr ctxt);

#define xmlXPathReturnBoolean(ctxt, val)				\
    valuePush((ctxt), xmlXPathNewBoolean(val))

#define xmlXPathReturnTrue(ctxt)   xmlXPathReturnBoolean((ctxt), 1)

#define xmlXPathReturnFalse(ctxt)  xmlXPathReturnBoolean((ctxt), 0)

#define xmlXPathReturnNumber(ctxt, val)					\
    valuePush((ctxt), xmlXPathNewFloat(val))

#define xmlXPathReturnString(ctxt, str)					\
    valuePush((ctxt), xmlXPathWrapString(str))

#define xmlXPathReturnEmptyString(ctxt)					\
    valuePush((ctxt), xmlXPathNewCString(""))

#define xmlXPathReturnNodeSet(ctxt, ns)					\
    valuePush((ctxt), xmlXPathWrapNodeSet(ns))

#define xmlXPathReturnEmptyNodeSet(ctxt)				\
    valuePush((ctxt), xmlXPathNewNodeSet(NULL))

#define xmlXPathReturnExternal(ctxt, val)				\
    valuePush((ctxt), xmlXPathWrapExternal(val))

#define xmlXPathStackIsNodeSet(ctxt)					\
    (((ctxt)->value != NULL)						\
     && (((ctxt)->value->type == XPATH_NODESET)				\
         || ((ctxt)->value->type == XPATH_XSLT_TREE)))

#define xmlXPathStackIsExternal(ctxt)					\
	((ctxt->value != NULL) && (ctxt->value->type == XPATH_USERS))

#define xmlXPathEmptyNodeSet(ns)					\
    { while ((ns)->nodeNr > 0) (ns)->nodeTab[(ns)->nodeNr--] = NULL; }

#define CHECK_ERROR							\
    if (ctxt->error != XPATH_EXPRESSION_OK) return

#define CHECK_ERROR0							\
    if (ctxt->error != XPATH_EXPRESSION_OK) return(0)

#define XP_ERROR(X)							\
    { xmlXPathErr(ctxt, X); return; }

#define XP_ERROR0(X)							\
    { xmlXPathErr(ctxt, X); return(0); }

#define CHECK_TYPE(typeval)						\
    if ((ctxt->value == NULL) || (ctxt->value->type != typeval))	\
        XP_ERROR(XPATH_INVALID_TYPE)

#define CHECK_TYPE0(typeval)						\
    if ((ctxt->value == NULL) || (ctxt->value->type != typeval))	\
        XP_ERROR0(XPATH_INVALID_TYPE)

#define CHECK_ARITY(x)							\
    if (ctxt == NULL) return;						\
    if (nargs != (x))							\
        XP_ERROR(XPATH_INVALID_ARITY);

#define CAST_TO_STRING							\
    if ((ctxt->value != NULL) && (ctxt->value->type != XPATH_STRING))	\
        xmlXPathStringFunction(ctxt, 1);

#define CAST_TO_NUMBER							\
    if ((ctxt->value != NULL) && (ctxt->value->type != XPATH_NUMBER))	\
        xmlXPathNumberFunction(ctxt, 1);

#define CAST_TO_BOOLEAN							\
    if ((ctxt->value != NULL) && (ctxt->value->type != XPATH_BOOLEAN))	\
        xmlXPathBooleanFunction(ctxt, 1);


XMLPUBFUN void XMLCALL	
	xmlXPathRegisterVariableLookup	(xmlXPathContextPtr ctxt,
					 xmlXPathVariableLookupFunc f,
					 void *data);


XMLPUBFUN void XMLCALL	
	    xmlXPathRegisterFuncLookup	(xmlXPathContextPtr ctxt,
					 xmlXPathFuncLookupFunc f,
					 void *funcCtxt);

XMLPUBFUN void XMLCALL		
		xmlXPatherror	(xmlXPathParserContextPtr ctxt,
				 const char *file,
				 int line,
				 int no);

XMLPUBFUN void XMLCALL
		xmlXPathErr	(xmlXPathParserContextPtr ctxt,
				 int error);

#ifdef LIBXML_DEBUG_ENABLED
XMLPUBFUN void XMLCALL		
		xmlXPathDebugDumpObject	(FILE *output,
					 xmlXPathObjectPtr cur,
					 int depth);
XMLPUBFUN void XMLCALL		
	    xmlXPathDebugDumpCompExpr(FILE *output,
					 xmlXPathCompExprPtr comp,
					 int depth);
#endif
XMLPUBFUN int XMLCALL		
		xmlXPathNodeSetContains		(xmlNodeSetPtr cur,
						 xmlNodePtr val);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathDifference		(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathIntersection		(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);

XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathDistinctSorted		(xmlNodeSetPtr nodes);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathDistinct		(xmlNodeSetPtr nodes);

XMLPUBFUN int XMLCALL		
		xmlXPathHasSameNodes		(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);

XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathNodeLeadingSorted	(xmlNodeSetPtr nodes,
						 xmlNodePtr node);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathLeadingSorted		(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathNodeLeading		(xmlNodeSetPtr nodes,
						 xmlNodePtr node);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathLeading			(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);

XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathNodeTrailingSorted	(xmlNodeSetPtr nodes,
						 xmlNodePtr node);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathTrailingSorted		(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathNodeTrailing		(xmlNodeSetPtr nodes,
						 xmlNodePtr node);
XMLPUBFUN xmlNodeSetPtr XMLCALL	
		xmlXPathTrailing		(xmlNodeSetPtr nodes1,
						 xmlNodeSetPtr nodes2);



XMLPUBFUN int XMLCALL		   
		xmlXPathRegisterNs		(xmlXPathContextPtr ctxt,
						 const xmlChar *prefix,
						 const xmlChar *ns_uri);
XMLPUBFUN const xmlChar * XMLCALL	   
		xmlXPathNsLookup		(xmlXPathContextPtr ctxt,
						 const xmlChar *prefix);
XMLPUBFUN void XMLCALL		   
		xmlXPathRegisteredNsCleanup	(xmlXPathContextPtr ctxt);

XMLPUBFUN int XMLCALL		   
		xmlXPathRegisterFunc		(xmlXPathContextPtr ctxt,
						 const xmlChar *name,
						 xmlXPathFunction f);
XMLPUBFUN int XMLCALL		   
		xmlXPathRegisterFuncNS		(xmlXPathContextPtr ctxt,
						 const xmlChar *name,
						 const xmlChar *ns_uri,
						 xmlXPathFunction f);
XMLPUBFUN int XMLCALL		   
		xmlXPathRegisterVariable	(xmlXPathContextPtr ctxt,
						 const xmlChar *name,
						 xmlXPathObjectPtr value);
XMLPUBFUN int XMLCALL		   
		xmlXPathRegisterVariableNS	(xmlXPathContextPtr ctxt,
						 const xmlChar *name,
						 const xmlChar *ns_uri,
						 xmlXPathObjectPtr value);
XMLPUBFUN xmlXPathFunction XMLCALL   
		xmlXPathFunctionLookup		(xmlXPathContextPtr ctxt,
						 const xmlChar *name);
XMLPUBFUN xmlXPathFunction XMLCALL   
		xmlXPathFunctionLookupNS	(xmlXPathContextPtr ctxt,
						 const xmlChar *name,
						 const xmlChar *ns_uri);
XMLPUBFUN void XMLCALL		   
		xmlXPathRegisteredFuncsCleanup	(xmlXPathContextPtr ctxt);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		xmlXPathVariableLookup		(xmlXPathContextPtr ctxt,
						 const xmlChar *name);
XMLPUBFUN xmlXPathObjectPtr XMLCALL  
		xmlXPathVariableLookupNS	(xmlXPathContextPtr ctxt,
						 const xmlChar *name,
						 const xmlChar *ns_uri);
XMLPUBFUN void XMLCALL		   
		xmlXPathRegisteredVariablesCleanup(xmlXPathContextPtr ctxt);

XMLPUBFUN xmlXPathParserContextPtr XMLCALL
		  xmlXPathNewParserContext	(const xmlChar *str,
			  			 xmlXPathContextPtr ctxt);
XMLPUBFUN void XMLCALL		  
		xmlXPathFreeParserContext	(xmlXPathParserContextPtr ctxt);

XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		valuePop			(xmlXPathParserContextPtr ctxt);
XMLPUBFUN int XMLCALL		  
		valuePush			(xmlXPathParserContextPtr ctxt,
					 	 xmlXPathObjectPtr value);

XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewString		(const xmlChar *val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewCString		(const char *val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathWrapString		(xmlChar *val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathWrapCString		(char * val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewFloat		(double val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewBoolean		(int val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewNodeSet		(xmlNodePtr val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewValueTree		(xmlNodePtr val);
XMLPUBFUN void XMLCALL		  
		xmlXPathNodeSetAdd		(xmlNodeSetPtr cur,
						 xmlNodePtr val);
XMLPUBFUN void XMLCALL              
		xmlXPathNodeSetAddUnique	(xmlNodeSetPtr cur,
						 xmlNodePtr val);
XMLPUBFUN void XMLCALL		  
		xmlXPathNodeSetAddNs		(xmlNodeSetPtr cur, 
						 xmlNodePtr node, 
						 xmlNsPtr ns);
XMLPUBFUN void XMLCALL              
		xmlXPathNodeSetSort		(xmlNodeSetPtr set);

XMLPUBFUN void XMLCALL		  
		xmlXPathRoot			(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void XMLCALL		  
		xmlXPathEvalExpr		(xmlXPathParserContextPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL	  
		xmlXPathParseName		(xmlXPathParserContextPtr ctxt);
XMLPUBFUN xmlChar * XMLCALL	  
		xmlXPathParseNCName		(xmlXPathParserContextPtr ctxt);

XMLPUBFUN double XMLCALL 
		xmlXPathStringEvalNumber	(const xmlChar *str);
XMLPUBFUN int XMLCALL 
		xmlXPathEvaluatePredicateResult (xmlXPathParserContextPtr ctxt, 
						 xmlXPathObjectPtr res);
XMLPUBFUN void XMLCALL 
		xmlXPathRegisterAllFunctions	(xmlXPathContextPtr ctxt);
XMLPUBFUN xmlNodeSetPtr XMLCALL 
		xmlXPathNodeSetMerge		(xmlNodeSetPtr val1, 
						 xmlNodeSetPtr val2);
XMLPUBFUN void XMLCALL 
		xmlXPathNodeSetDel		(xmlNodeSetPtr cur, 
						 xmlNodePtr val);
XMLPUBFUN void XMLCALL 
		xmlXPathNodeSetRemove		(xmlNodeSetPtr cur, 
						 int val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathNewNodeSetList		(xmlNodeSetPtr val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathWrapNodeSet		(xmlNodeSetPtr val);
XMLPUBFUN xmlXPathObjectPtr XMLCALL 
		xmlXPathWrapExternal		(void *val);

XMLPUBFUN int XMLCALL xmlXPathEqualValues(xmlXPathParserContextPtr ctxt);
XMLPUBFUN int XMLCALL xmlXPathNotEqualValues(xmlXPathParserContextPtr ctxt);
XMLPUBFUN int XMLCALL xmlXPathCompareValues(xmlXPathParserContextPtr ctxt, int inf, int strict);
XMLPUBFUN void XMLCALL xmlXPathValueFlipSign(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void XMLCALL xmlXPathAddValues(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void XMLCALL xmlXPathSubValues(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void XMLCALL xmlXPathMultValues(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void XMLCALL xmlXPathDivValues(xmlXPathParserContextPtr ctxt);
XMLPUBFUN void XMLCALL xmlXPathModValues(xmlXPathParserContextPtr ctxt);

XMLPUBFUN int XMLCALL xmlXPathIsNodeType(const xmlChar *name);

XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextSelf(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextChild(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextDescendant(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextDescendantOrSelf(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextParent(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextAncestorOrSelf(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextFollowingSibling(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextFollowing(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextNamespace(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextAttribute(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextPreceding(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextAncestor(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN xmlNodePtr XMLCALL xmlXPathNextPrecedingSibling(xmlXPathParserContextPtr ctxt,
			xmlNodePtr cur);
XMLPUBFUN void XMLCALL xmlXPathLastFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathPositionFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathCountFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathIdFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathLocalNameFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathNamespaceURIFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathStringFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathStringLengthFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathConcatFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathContainsFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathStartsWithFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathSubstringFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathSubstringBeforeFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathSubstringAfterFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathNormalizeFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathTranslateFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathNotFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathTrueFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathFalseFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathLangFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathNumberFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathSumFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathFloorFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathCeilingFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathRoundFunction(xmlXPathParserContextPtr ctxt, int nargs);
XMLPUBFUN void XMLCALL xmlXPathBooleanFunction(xmlXPathParserContextPtr ctxt, int nargs);

XMLPUBFUN void XMLCALL xmlXPathNodeSetFreeNs(xmlNsPtr ns);
 
#ifdef __cplusplus
}
#endif

#endif 
#endif 
