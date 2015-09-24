/*
 * "Canonical XML" implementation 
 * http://www.w3.org/TR/xml-c14n
 * 
 * "Exclusive XML Canonicalization" implementation
 * http://www.w3.org/TR/xml-exc-c14n
 *
 * See Copyright for the status of this software.
 * 
 * Author: Aleksey Sanin <aleksey@aleksey.com>
 */
#define IN_LIBXML
#include "libxml.h"
#ifdef LIBXML_C14N_ENABLED
#ifdef LIBXML_OUTPUT_ENABLED

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <string.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/uri.h>
#include <libxml/xmlerror.h>
#include <libxml/globals.h>
#include <libxml/xpathInternals.h>
#include <libxml/c14n.h>


typedef enum {
    XMLC14N_BEFORE_DOCUMENT_ELEMENT = 0,
    XMLC14N_INSIDE_DOCUMENT_ELEMENT = 1,
    XMLC14N_AFTER_DOCUMENT_ELEMENT = 2
} xmlC14NPosition;

typedef struct _xmlC14NVisibleNsStack {
    int nsCurEnd;           
    int nsPrevStart;        
    int nsPrevEnd;          
    int nsMax;              
    xmlNsPtr 	*nsTab;	    	      
    xmlNodePtr	*nodeTab;   
} xmlC14NVisibleNsStack, *xmlC14NVisibleNsStackPtr;

typedef struct _xmlC14NCtx {
    
    xmlDocPtr doc;
    xmlC14NIsVisibleCallback is_visible_callback;
    void* user_data;    
    int with_comments;
    xmlOutputBufferPtr buf;

    
    xmlC14NPosition pos;
    int parent_is_doc;
    xmlC14NVisibleNsStackPtr ns_rendered;
    
    
    xmlC14NMode mode;

    
    xmlChar **inclusive_ns_prefixes;

    
    int error;
} xmlC14NCtx, *xmlC14NCtxPtr;

static xmlC14NVisibleNsStackPtr	xmlC14NVisibleNsStackCreate	(void);
static void     xmlC14NVisibleNsStackDestroy	(xmlC14NVisibleNsStackPtr cur);
static void     xmlC14NVisibleNsStackAdd	    (xmlC14NVisibleNsStackPtr cur, 
                                                 xmlNsPtr ns,
                                                 xmlNodePtr node);
static void 			xmlC14NVisibleNsStackSave	(xmlC14NVisibleNsStackPtr cur,
								 xmlC14NVisibleNsStackPtr state);
static void 			xmlC14NVisibleNsStackRestore	(xmlC14NVisibleNsStackPtr cur,
								 xmlC14NVisibleNsStackPtr state);
static void 			xmlC14NVisibleNsStackShift	(xmlC14NVisibleNsStackPtr cur);
static int			xmlC14NVisibleNsStackFind	(xmlC14NVisibleNsStackPtr cur, 
								 xmlNsPtr ns);
static int			xmlExcC14NVisibleNsStackFind	(xmlC14NVisibleNsStackPtr cur, 
								 xmlNsPtr ns,
								 xmlC14NCtxPtr ctx);

static int			xmlC14NIsNodeInNodeset		(xmlNodeSetPtr nodes,
								 xmlNodePtr node,
								 xmlNodePtr parent);



static int xmlC14NProcessNode(xmlC14NCtxPtr ctx, xmlNodePtr cur);
static int xmlC14NProcessNodeList(xmlC14NCtxPtr ctx, xmlNodePtr cur);
typedef enum {
    XMLC14N_NORMALIZE_ATTR = 0,
    XMLC14N_NORMALIZE_COMMENT = 1,
    XMLC14N_NORMALIZE_PI = 2,
    XMLC14N_NORMALIZE_TEXT = 3
} xmlC14NNormalizationMode;

static xmlChar *xmlC11NNormalizeString(const xmlChar * input,
                                       xmlC14NNormalizationMode mode);

#define 	xmlC11NNormalizeAttr( a ) \
    xmlC11NNormalizeString((a), XMLC14N_NORMALIZE_ATTR)
#define 	xmlC11NNormalizeComment( a ) \
    xmlC11NNormalizeString((a), XMLC14N_NORMALIZE_COMMENT)
#define 	xmlC11NNormalizePI( a )	\
    xmlC11NNormalizeString((a), XMLC14N_NORMALIZE_PI)
#define 	xmlC11NNormalizeText( a ) \
    xmlC11NNormalizeString((a), XMLC14N_NORMALIZE_TEXT)

#define 	xmlC14NIsVisible( ctx, node, parent ) \
     (((ctx)->is_visible_callback != NULL) ? \
	(ctx)->is_visible_callback((ctx)->user_data, \
		(xmlNodePtr)(node), (xmlNodePtr)(parent)) : 1)

#define 	xmlC14NIsExclusive( ctx ) \
    ( (ctx)->mode == XML_C14N_EXCLUSIVE_1_0 )


static void
xmlC14NErrMemory(const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_C14N,
		    XML_ERR_NO_MEMORY, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Memory allocation failed : %s\n", extra);
}

static void
xmlC14NErrParam(const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_C14N,
		    XML_ERR_INTERNAL_ERROR, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Invalid parameter : %s\n", extra);
}

static void
xmlC14NErrInternal(const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_C14N,
		    XML_ERR_INTERNAL_ERROR, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Internal error : %s\n", extra);
}

static void
xmlC14NErrInvalidNode(const char *node_type, const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_C14N,
		    XML_C14N_INVALID_NODE, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Node %s is invalid here : %s\n", node_type, extra);
}

static void
xmlC14NErrUnknownNode(int node_type, const char *extra)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_C14N,
		    XML_C14N_UNKNOW_NODE, XML_ERR_ERROR, NULL, 0, extra,
		    NULL, NULL, 0, 0,
		    "Unknown node type %d found : %s\n", node_type, extra);
}

static void
xmlC14NErrRelativeNamespace(const char *ns_uri)
{
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_C14N,
		    XML_C14N_RELATIVE_NAMESPACE, XML_ERR_ERROR, NULL, 0, NULL,
		    NULL, NULL, 0, 0,
		    "Relative namespace UR is invalid here : %s\n", ns_uri);
}



static void
xmlC14NErr(xmlC14NCtxPtr ctxt, xmlNodePtr node, int error,
           const char * msg)
{
    if (ctxt != NULL)
        ctxt->error = error;
    __xmlRaiseError(NULL, NULL, NULL,
		    ctxt, node, XML_FROM_C14N, error,
		    XML_ERR_ERROR, NULL, 0,
		    NULL, NULL, NULL, 0, 0, "%s", msg);
}

#define XML_NAMESPACES_DEFAULT		16

static int			
xmlC14NIsNodeInNodeset(xmlNodeSetPtr nodes, xmlNodePtr node, xmlNodePtr parent) {
    if((nodes != NULL) && (node != NULL)) {
	if(node->type != XML_NAMESPACE_DECL) {
	    return(xmlXPathNodeSetContains(nodes, node));
	} else {
	    xmlNs ns;
	    
	    memcpy(&ns, node, sizeof(ns)); 
	    
	    
	    if((parent != NULL) && (parent->type == XML_ATTRIBUTE_NODE)) {
		ns.next = (xmlNsPtr)parent->parent;
	    } else {
		ns.next = (xmlNsPtr)parent; 
	    }

	    return(xmlXPathNodeSetContains(nodes, (xmlNodePtr)&ns));
	}
    }
    return(1);
}

static xmlC14NVisibleNsStackPtr
xmlC14NVisibleNsStackCreate(void) {
    xmlC14NVisibleNsStackPtr ret;

    ret = (xmlC14NVisibleNsStackPtr) xmlMalloc(sizeof(xmlC14NVisibleNsStack));
    if (ret == NULL) {
        xmlC14NErrMemory("creating namespaces stack");
	return(NULL);
    }
    memset(ret, 0 , (size_t) sizeof(xmlC14NVisibleNsStack));
    return(ret);
}

static void
xmlC14NVisibleNsStackDestroy(xmlC14NVisibleNsStackPtr cur) {
    if(cur == NULL) {
        xmlC14NErrParam("destroying namespaces stack");
        return;
    }
    if(cur->nsTab != NULL) {
	memset(cur->nsTab, 0, cur->nsMax * sizeof(xmlNsPtr));
	xmlFree(cur->nsTab);
    }
    if(cur->nodeTab != NULL) {
	memset(cur->nodeTab, 0, cur->nsMax * sizeof(xmlNodePtr));
	xmlFree(cur->nodeTab);
    }
    memset(cur, 0, sizeof(xmlC14NVisibleNsStack));
    xmlFree(cur);
    
}

static void 
xmlC14NVisibleNsStackAdd(xmlC14NVisibleNsStackPtr cur, xmlNsPtr ns, xmlNodePtr node) {
    if((cur == NULL) || 
       ((cur->nsTab == NULL) && (cur->nodeTab != NULL)) ||
       ((cur->nsTab != NULL) && (cur->nodeTab == NULL))) {
        xmlC14NErrParam("adding namespace to stack");
	return;
    }

    if ((cur->nsTab == NULL) && (cur->nodeTab == NULL)) {
        cur->nsTab = (xmlNsPtr*) xmlMalloc(XML_NAMESPACES_DEFAULT * sizeof(xmlNsPtr));
        cur->nodeTab = (xmlNodePtr*) xmlMalloc(XML_NAMESPACES_DEFAULT * sizeof(xmlNodePtr));
	if ((cur->nsTab == NULL) || (cur->nodeTab == NULL)) {
	    xmlC14NErrMemory("adding node to stack");
	    return;
	}
	memset(cur->nsTab, 0 , XML_NAMESPACES_DEFAULT * sizeof(xmlNsPtr));
	memset(cur->nodeTab, 0 , XML_NAMESPACES_DEFAULT * sizeof(xmlNodePtr));
        cur->nsMax = XML_NAMESPACES_DEFAULT;
    } else if(cur->nsMax == cur->nsCurEnd) {
	void *tmp;	
	int tmpSize;
	
	tmpSize = 2 * cur->nsMax;
	tmp = xmlRealloc(cur->nsTab, tmpSize * sizeof(xmlNsPtr));
	if (tmp == NULL) {
	    xmlC14NErrMemory("adding node to stack");
	    return;
	}
	cur->nsTab = (xmlNsPtr*)tmp;

	tmp = xmlRealloc(cur->nodeTab, tmpSize * sizeof(xmlNodePtr));
	if (tmp == NULL) {
	    xmlC14NErrMemory("adding node to stack");
	    return;
	}
	cur->nodeTab = (xmlNodePtr*)tmp;

	cur->nsMax = tmpSize;
    }
    cur->nsTab[cur->nsCurEnd] = ns;
    cur->nodeTab[cur->nsCurEnd] = node;

    ++cur->nsCurEnd;
}

static void
xmlC14NVisibleNsStackSave(xmlC14NVisibleNsStackPtr cur, xmlC14NVisibleNsStackPtr state) {
    if((cur == NULL) || (state == NULL)) {
        xmlC14NErrParam("saving namespaces stack");
	return;
    }
    
    state->nsCurEnd = cur->nsCurEnd;
    state->nsPrevStart = cur->nsPrevStart;
    state->nsPrevEnd = cur->nsPrevEnd;
}

static void
xmlC14NVisibleNsStackRestore(xmlC14NVisibleNsStackPtr cur, xmlC14NVisibleNsStackPtr state) {
    if((cur == NULL) || (state == NULL)) {
        xmlC14NErrParam("restoring namespaces stack");
	return;
    }
    cur->nsCurEnd = state->nsCurEnd;
    cur->nsPrevStart = state->nsPrevStart;
    cur->nsPrevEnd = state->nsPrevEnd;
}

static void 
xmlC14NVisibleNsStackShift(xmlC14NVisibleNsStackPtr cur) {
    if(cur == NULL) {
        xmlC14NErrParam("shifting namespaces stack");
	return;
    }
    cur->nsPrevStart = cur->nsPrevEnd;
    cur->nsPrevEnd = cur->nsCurEnd;
}

static int
xmlC14NStrEqual(const xmlChar *str1, const xmlChar *str2) {
    if (str1 == str2) return(1);
    if (str1 == NULL) return((*str2) == '\0');
    if (str2 == NULL) return((*str1) == '\0');
    do {
	if (*str1++ != *str2) return(0);
    } while (*str2++);
    return(1);
}

static int
xmlC14NVisibleNsStackFind(xmlC14NVisibleNsStackPtr cur, xmlNsPtr ns)
{
    int i;
    const xmlChar *prefix;
    const xmlChar *href;
    int has_empty_ns;
        
    if(cur == NULL) {
        xmlC14NErrParam("searching namespaces stack (c14n)");
        return (0);
    }

    prefix = ((ns == NULL) || (ns->prefix == NULL)) ? BAD_CAST "" : ns->prefix;
    href = ((ns == NULL) || (ns->href == NULL)) ? BAD_CAST "" : ns->href;
    has_empty_ns = (xmlC14NStrEqual(prefix, NULL) && xmlC14NStrEqual(href, NULL));

    if (cur->nsTab != NULL) {
	int start = (has_empty_ns) ? 0 : cur->nsPrevStart;
        for (i = cur->nsCurEnd - 1; i >= start; --i) {
            xmlNsPtr ns1 = cur->nsTab[i];
	    
	    if(xmlC14NStrEqual(prefix, (ns1 != NULL) ? ns1->prefix : NULL)) {
		return(xmlC14NStrEqual(href, (ns1 != NULL) ? ns1->href : NULL));
	    }
        }
    }
    return(has_empty_ns);
}

static int			
xmlExcC14NVisibleNsStackFind(xmlC14NVisibleNsStackPtr cur, xmlNsPtr ns, xmlC14NCtxPtr ctx) {
    int i;
    const xmlChar *prefix;
    const xmlChar *href;
    int has_empty_ns;
        
    if(cur == NULL) {
        xmlC14NErrParam("searching namespaces stack (exc c14n)");
        return (0);
    }

    prefix = ((ns == NULL) || (ns->prefix == NULL)) ? BAD_CAST "" : ns->prefix;
    href = ((ns == NULL) || (ns->href == NULL)) ? BAD_CAST "" : ns->href;
    has_empty_ns = (xmlC14NStrEqual(prefix, NULL) && xmlC14NStrEqual(href, NULL));

    if (cur->nsTab != NULL) {
	int start = 0;
        for (i = cur->nsCurEnd - 1; i >= start; --i) {
            xmlNsPtr ns1 = cur->nsTab[i];
	    
	    if(xmlC14NStrEqual(prefix, (ns1 != NULL) ? ns1->prefix : NULL)) {
		if(xmlC14NStrEqual(href, (ns1 != NULL) ? ns1->href : NULL)) {
	    	    return(xmlC14NIsVisible(ctx, ns1, cur->nodeTab[i]));
		} else {
		    return(0);
		}
	    }
        }
    }
    return(has_empty_ns);
}





static int
xmlC14NIsXmlNs(xmlNsPtr ns)
{
    return ((ns != NULL) &&
            (xmlStrEqual(ns->prefix, BAD_CAST "xml")) &&
            (xmlStrEqual(ns->href, XML_XML_NAMESPACE)));
}


static int
xmlC14NNsCompare(xmlNsPtr ns1, xmlNsPtr ns2)
{
    if (ns1 == ns2)
        return (0);
    if (ns1 == NULL)
        return (-1);
    if (ns2 == NULL)
        return (1);

    return (xmlStrcmp(ns1->prefix, ns2->prefix));
}


static int
xmlC14NPrintNamespaces(const xmlNsPtr ns, xmlC14NCtxPtr ctx)
{

    if ((ns == NULL) || (ctx == NULL)) {
        xmlC14NErrParam("writing namespaces");
        return 0;
    }

    if (ns->prefix != NULL) {
        xmlOutputBufferWriteString(ctx->buf, " xmlns:");
        xmlOutputBufferWriteString(ctx->buf, (const char *) ns->prefix);
        xmlOutputBufferWriteString(ctx->buf, "=\"");
    } else {
        xmlOutputBufferWriteString(ctx->buf, " xmlns=\"");
    }
    if(ns->href != NULL) {
	xmlOutputBufferWriteString(ctx->buf, (const char *) ns->href);
    }
    xmlOutputBufferWriteString(ctx->buf, "\"");
    return (1);
}

static int
xmlC14NProcessNamespacesAxis(xmlC14NCtxPtr ctx, xmlNodePtr cur, int visible)
{
    xmlNodePtr n;
    xmlNsPtr ns, tmp;
    xmlListPtr list;
    int already_rendered;
    int has_empty_ns = 0;
    
    if ((ctx == NULL) || (cur == NULL) || (cur->type != XML_ELEMENT_NODE)) {
        xmlC14NErrParam("processing namespaces axis (c14n)");
        return (-1);
    }

    list = xmlListCreate(NULL, (xmlListDataCompare) xmlC14NNsCompare);
    if (list == NULL) {
        xmlC14NErrInternal("creating namespaces list (c14n)");
        return (-1);
    }

    
    for(n = cur; n != NULL; n = n->parent) {
	for(ns = n->nsDef; ns != NULL; ns = ns->next) {
	    tmp = xmlSearchNs(cur->doc, cur, ns->prefix);
	    
	    if((tmp == ns) && !xmlC14NIsXmlNs(ns) && xmlC14NIsVisible(ctx, ns, cur)) {
		already_rendered = xmlC14NVisibleNsStackFind(ctx->ns_rendered, ns);
		if(visible) {
        	    xmlC14NVisibleNsStackAdd(ctx->ns_rendered, ns, cur);
		}
		if(!already_rendered) {
		    xmlListInsert(list, ns); 
		}
    		if(xmlStrlen(ns->prefix) == 0) {
		    has_empty_ns = 1;
		}
	    }
	}
    }
	
    if(visible && !has_empty_ns) {
        static xmlNs ns_default;

        memset(&ns_default, 0, sizeof(ns_default));
        if(!xmlC14NVisibleNsStackFind(ctx->ns_rendered, &ns_default)) {
    	    xmlC14NPrintNamespaces(&ns_default, ctx);
	}
    }
	
    
    xmlListWalk(list, (xmlListWalker) xmlC14NPrintNamespaces, (const void *) ctx);

    xmlListDelete(list);
    return (0);
}


static int
xmlExcC14NProcessNamespacesAxis(xmlC14NCtxPtr ctx, xmlNodePtr cur, int visible)
{
    xmlNsPtr ns;
    xmlListPtr list;
    xmlAttrPtr attr;
    int already_rendered;
    int has_empty_ns = 0;
    int has_visibly_utilized_empty_ns = 0;
    int has_empty_ns_in_inclusive_list = 0;
        
    if ((ctx == NULL) || (cur == NULL) || (cur->type != XML_ELEMENT_NODE)) {
        xmlC14NErrParam("processing namespaces axis (exc c14n)");
        return (-1);
    }

    if(!xmlC14NIsExclusive(ctx)) {
        xmlC14NErrParam("processing namespaces axis (exc c14n)");
        return (-1);

    }

    list = xmlListCreate(NULL, (xmlListDataCompare) xmlC14NNsCompare);
    if (list == NULL) {
        xmlC14NErrInternal("creating namespaces list (exc c14n)");
        return (-1);
    }

    if(ctx->inclusive_ns_prefixes != NULL) {
	xmlChar *prefix; 
	int i;
	
	for (i = 0; ctx->inclusive_ns_prefixes[i] != NULL; ++i) {
	    prefix = ctx->inclusive_ns_prefixes[i];
            if (xmlStrEqual(prefix, BAD_CAST "#default")
                || xmlStrEqual(prefix, BAD_CAST "")) {
                prefix = NULL;
		has_empty_ns_in_inclusive_list = 1;
            }
	
	    ns = xmlSearchNs(cur->doc, cur, prefix);	    
	    if((ns != NULL) && !xmlC14NIsXmlNs(ns) && xmlC14NIsVisible(ctx, ns, cur)) {
		already_rendered = xmlC14NVisibleNsStackFind(ctx->ns_rendered, ns);
		if(visible) {
    	    	    xmlC14NVisibleNsStackAdd(ctx->ns_rendered, ns, cur);
		}
		if(!already_rendered) {
	    	    xmlListInsert(list, ns); 
		}
    		if(xmlStrlen(ns->prefix) == 0) {
		    has_empty_ns = 1;
		}
	    }
	}
    }
    
    
    if(cur->ns != NULL) {
	ns = cur->ns;
    } else {
        ns = xmlSearchNs(cur->doc, cur, NULL);
	has_visibly_utilized_empty_ns = 1;
    }
    if((ns != NULL) && !xmlC14NIsXmlNs(ns)) {
	if(visible && xmlC14NIsVisible(ctx, ns, cur)) { 
	    if(!xmlExcC14NVisibleNsStackFind(ctx->ns_rendered, ns, ctx)) {
		xmlListInsert(list, ns);
	    }
	}
	if(visible) {
    	    xmlC14NVisibleNsStackAdd(ctx->ns_rendered, ns, cur); 
	}
	if(xmlStrlen(ns->prefix) == 0) {
	    has_empty_ns = 1;
	}
    }
    
        
    
    for(attr = cur->properties; attr != NULL; attr = attr->next) {
	if((attr->ns != NULL) && !xmlC14NIsXmlNs(attr->ns) && xmlC14NIsVisible(ctx, attr, cur)) {
	    already_rendered = xmlExcC14NVisibleNsStackFind(ctx->ns_rendered, attr->ns, ctx);
	    xmlC14NVisibleNsStackAdd(ctx->ns_rendered, attr->ns, cur); 
	    if(!already_rendered && visible) {
		xmlListInsert(list, attr->ns); 
	    }
	    if(xmlStrlen(attr->ns->prefix) == 0) {
		has_empty_ns = 1;
	    }
	} else if((attr->ns != NULL) && (xmlStrlen(attr->ns->prefix) == 0) && (xmlStrlen(attr->ns->href) == 0)) {
	    has_visibly_utilized_empty_ns = 1;
	}
    }

    if(visible && has_visibly_utilized_empty_ns && 
	    !has_empty_ns && !has_empty_ns_in_inclusive_list) {
        static xmlNs ns_default;

        memset(&ns_default, 0, sizeof(ns_default));
	
        already_rendered = xmlExcC14NVisibleNsStackFind(ctx->ns_rendered, &ns_default, ctx);
	if(!already_rendered) {
    	    xmlC14NPrintNamespaces(&ns_default, ctx);
	}
    } else if(visible && !has_empty_ns && has_empty_ns_in_inclusive_list) {
        static xmlNs ns_default;

        memset(&ns_default, 0, sizeof(ns_default));
        if(!xmlC14NVisibleNsStackFind(ctx->ns_rendered, &ns_default)) {
    	    xmlC14NPrintNamespaces(&ns_default, ctx);
	}
    }

    

    xmlListWalk(list, (xmlListWalker) xmlC14NPrintNamespaces, (const void *) ctx);

    xmlListDelete(list);
    return (0);
}



static int
xmlC14NIsXmlAttr(xmlAttrPtr attr)
{
    return ((attr->ns != NULL) && 
           (xmlC14NIsXmlNs(attr->ns) != 0));
}


static int
xmlC14NAttrsCompare(xmlAttrPtr attr1, xmlAttrPtr attr2)
{
    int ret = 0;

    if (attr1 == attr2)
        return (0);
    if (attr1 == NULL)
        return (-1);
    if (attr2 == NULL)
        return (1);
    if (attr1->ns == attr2->ns) {
        return (xmlStrcmp(attr1->name, attr2->name));
    }

    if (attr1->ns == NULL)
        return (-1);
    if (attr2->ns == NULL)
        return (1);
    if (attr1->ns->prefix == NULL)
        return (-1);
    if (attr2->ns->prefix == NULL)
        return (1);

    ret = xmlStrcmp(attr1->ns->href, attr2->ns->href);
    if (ret == 0) {
        ret = xmlStrcmp(attr1->name, attr2->name);
    }
    return (ret);
}


static int
xmlC14NPrintAttrs(const xmlAttrPtr attr, xmlC14NCtxPtr ctx)
{
    xmlChar *value;
    xmlChar *buffer;

    if ((attr == NULL) || (ctx == NULL)) {
        xmlC14NErrParam("writing attributes");
        return (0);
    }

    xmlOutputBufferWriteString(ctx->buf, " ");
    if (attr->ns != NULL && xmlStrlen(attr->ns->prefix) > 0) {
        xmlOutputBufferWriteString(ctx->buf,
                                   (const char *) attr->ns->prefix);
        xmlOutputBufferWriteString(ctx->buf, ":");
    }
    xmlOutputBufferWriteString(ctx->buf, (const char *) attr->name);
    xmlOutputBufferWriteString(ctx->buf, "=\"");

    value = xmlNodeListGetString(ctx->doc, attr->children, 1);
    
    if (value != NULL) {
        buffer = xmlC11NNormalizeAttr(value);
        xmlFree(value);
        if (buffer != NULL) {
            xmlOutputBufferWriteString(ctx->buf, (const char *) buffer);
            xmlFree(buffer);
        } else {
            xmlC14NErrInternal("normalizing attributes axis");
            return (0);
        }
    }
    xmlOutputBufferWriteString(ctx->buf, "\"");
    return (1);
}

static xmlAttrPtr
xmlC14NFindHiddenParentAttr(xmlC14NCtxPtr ctx, xmlNodePtr cur, const xmlChar * name, const xmlChar * ns)
{
    xmlAttrPtr res;
    while((cur != NULL) && (!xmlC14NIsVisible(ctx, cur, cur->parent))) {
        res = xmlHasNsProp(cur, name, ns);
        if(res != NULL) {
            return res;
        }

        cur = cur->parent;
    }

    return NULL;
}

static xmlAttrPtr
xmlC14NFixupBaseAttr(xmlC14NCtxPtr ctx, xmlAttrPtr xml_base_attr)
{    
    xmlChar * res = NULL;
    xmlNodePtr cur;
    xmlAttrPtr attr;
    xmlChar * tmp_str;
    xmlChar * tmp_str2;
    int tmp_str_len;

    if ((ctx == NULL) || (xml_base_attr == NULL) || (xml_base_attr->parent == NULL)) {
        xmlC14NErrParam("processing xml:base attribute");
        return (NULL);
    }

    
    res = xmlNodeListGetString(ctx->doc, xml_base_attr->children, 1);
    if(res == NULL) {
        xmlC14NErrInternal("processing xml:base attribute - can't get attr value");
        return (NULL);
    }

    
    cur = xml_base_attr->parent->parent;
    while((cur != NULL) && (!xmlC14NIsVisible(ctx, cur, cur->parent))) {
        attr = xmlHasNsProp(cur, BAD_CAST "base", XML_XML_NAMESPACE);
        if(attr != NULL) {
            
            tmp_str = xmlNodeListGetString(ctx->doc, attr->children, 1);
            if(tmp_str == NULL) {
                xmlFree(res);

                xmlC14NErrInternal("processing xml:base attribute - can't get attr value");
                return (NULL);
            } 

            tmp_str_len = xmlStrlen(tmp_str);
            if(tmp_str_len > 1 && tmp_str[tmp_str_len - 2] == '.') {
                tmp_str2 = xmlStrcat(tmp_str, BAD_CAST "/");
                if(tmp_str2 == NULL) {
                    xmlFree(tmp_str);
                    xmlFree(res);

                    xmlC14NErrInternal("processing xml:base attribute - can't modify uri");
                    return (NULL);
                }

                tmp_str = tmp_str2;
            }

            
            tmp_str2 = xmlBuildURI(res, tmp_str); 
            if(tmp_str2 == NULL) {
                xmlFree(tmp_str);
                xmlFree(res);

                xmlC14NErrInternal("processing xml:base attribute - can't construct uri");
                return (NULL);
            }

            
            xmlFree(tmp_str);
            xmlFree(res);
            res = tmp_str2;
        }

        
        cur = cur->parent;
    }

    
    if((res == NULL) || xmlStrEqual(res, BAD_CAST "")) {
        xmlFree(res);
        return (NULL);
    }

    
    attr = xmlNewNsProp(NULL, xml_base_attr->ns, BAD_CAST "base", res);
    if(attr == NULL) {
        xmlFree(res);

        xmlC14NErrInternal("processing xml:base attribute - can't construct attribute");
        return (NULL);
    }
 
    
    xmlFree(res);
    return (attr);
}

static int
xmlC14NProcessAttrsAxis(xmlC14NCtxPtr ctx, xmlNodePtr cur, int parent_visible)
{
    xmlAttrPtr attr;
    xmlListPtr list;    
    xmlAttrPtr attrs_to_delete = NULL;
    
    
    xmlAttrPtr xml_base_attr = NULL;
    xmlAttrPtr xml_lang_attr = NULL;
    xmlAttrPtr xml_space_attr = NULL;

    if ((ctx == NULL) || (cur == NULL) || (cur->type != XML_ELEMENT_NODE)) {
        xmlC14NErrParam("processing attributes axis");
        return (-1);
    }

    list = xmlListCreate(NULL, (xmlListDataCompare) xmlC14NAttrsCompare);
    if (list == NULL) {
        xmlC14NErrInternal("creating attributes list");
        return (-1);
    }

    switch(ctx->mode) {
    case XML_C14N_1_0:
    
        attr = cur->properties;
        while (attr != NULL) {
            
            if (xmlC14NIsVisible(ctx, attr, cur)) {
                xmlListInsert(list, attr);
            }
            attr = attr->next;
        }

        if (parent_visible && (cur->parent != NULL) && 
            (!xmlC14NIsVisible(ctx, cur->parent, cur->parent->parent))) 
        {
            xmlNodePtr tmp;

            tmp = cur->parent;
            while (tmp != NULL) {
                attr = tmp->properties;
                while (attr != NULL) {
                    if (xmlC14NIsXmlAttr(attr) != 0) {
                        if (xmlListSearch(list, attr) == NULL) {
                            xmlListInsert(list, attr);
                        }
                    }
                    attr = attr->next;
                }
                tmp = tmp->parent;
            }
        }

        
        break;
    case XML_C14N_EXCLUSIVE_1_0:

        attr = cur->properties;
        while (attr != NULL) {
            
            if (xmlC14NIsVisible(ctx, attr, cur)) {
                xmlListInsert(list, attr);
            }
            attr = attr->next;
        }

        
        break;
    case XML_C14N_1_1:

        attr = cur->properties;
        while (attr != NULL) {
            
            if ((!parent_visible) || (xmlC14NIsXmlAttr(attr) == 0)) {
                
                if (xmlC14NIsVisible(ctx, attr, cur)) {
                    xmlListInsert(list, attr);
                }
            } else {
                int matched = 0;

                
                if((!matched) && (xml_lang_attr == NULL) && xmlStrEqual(attr->name, BAD_CAST "lang")) {
                    xml_lang_attr = attr;
                    matched = 1;
                } 
                if((!matched) && (xml_space_attr == NULL) && xmlStrEqual(attr->name, BAD_CAST "space")) {
                    xml_space_attr = attr;
                    matched = 1;
                }

                
                if((!matched) && (xml_base_attr == NULL) && xmlStrEqual(attr->name, BAD_CAST "base")) {
                    xml_base_attr = attr;
                    matched = 1;
                }

                
                if((!matched) && xmlC14NIsVisible(ctx, attr, cur)) {
                    xmlListInsert(list, attr);
                }
            }
         
            
            attr = attr->next;
        }
            
        
        if ((parent_visible)) {

            
            if(xml_lang_attr == NULL) {
                xml_lang_attr = xmlC14NFindHiddenParentAttr(ctx, cur->parent, BAD_CAST "lang", XML_XML_NAMESPACE);
            }
            if(xml_lang_attr != NULL) {
                xmlListInsert(list, xml_lang_attr);
            }
            if(xml_space_attr == NULL) {
                xml_space_attr = xmlC14NFindHiddenParentAttr(ctx, cur->parent, BAD_CAST "space", XML_XML_NAMESPACE);
            }
            if(xml_space_attr != NULL) {
                xmlListInsert(list, xml_space_attr);
            }

            
            if(xml_base_attr == NULL) {
                
                xml_base_attr = xmlC14NFindHiddenParentAttr(ctx, cur->parent, BAD_CAST "base", XML_XML_NAMESPACE);
            }
            if(xml_base_attr != NULL) {
                xml_base_attr = xmlC14NFixupBaseAttr(ctx, xml_base_attr);
                if(xml_base_attr != NULL) {                    
                    xmlListInsert(list, xml_base_attr);

                    
                    xml_base_attr->next = attrs_to_delete;
                    attrs_to_delete = xml_base_attr;
                }
            }
        }

        
        break;
    }

    xmlListWalk(list, (xmlListWalker) xmlC14NPrintAttrs, (const void *) ctx);

    xmlFreePropList(attrs_to_delete);
    xmlListDelete(list);
    return (0);
}

static int
xmlC14NCheckForRelativeNamespaces(xmlC14NCtxPtr ctx, xmlNodePtr cur)
{
    xmlNsPtr ns;

    if ((ctx == NULL) || (cur == NULL) || (cur->type != XML_ELEMENT_NODE)) {
        xmlC14NErrParam("checking for relative namespaces");
        return (-1);
    }

    ns = cur->nsDef;
    while (ns != NULL) {
        if (xmlStrlen(ns->href) > 0) {
            xmlURIPtr uri;

            uri = xmlParseURI((const char *) ns->href);
            if (uri == NULL) {
                xmlC14NErrInternal("parsing namespace uri");
                return (-1);
            }
            if (xmlStrlen((const xmlChar *) uri->scheme) == 0) {
                xmlC14NErrRelativeNamespace(uri->scheme);
                xmlFreeURI(uri);
                return (-1);
            }
            if ((xmlStrcasecmp((const xmlChar *) uri->scheme, BAD_CAST "urn") != 0)
                && (xmlStrcasecmp((const xmlChar *) uri->scheme, BAD_CAST "dav") !=0)
                && (xmlStrlen((const xmlChar *) uri->server) == 0)) {
                xmlC14NErrRelativeNamespace(uri->scheme);
                xmlFreeURI(uri);
                return (-1);
            }
            xmlFreeURI(uri);
        }
        ns = ns->next;
    }
    return (0);
}

static int
xmlC14NProcessElementNode(xmlC14NCtxPtr ctx, xmlNodePtr cur, int visible)
{
    int ret;
    xmlC14NVisibleNsStack state;
    int parent_is_doc = 0;

    if ((ctx == NULL) || (cur == NULL) || (cur->type != XML_ELEMENT_NODE)) {
        xmlC14NErrParam("processing element node");
        return (-1);
    }

    if (xmlC14NCheckForRelativeNamespaces(ctx, cur) < 0) {
        xmlC14NErrInternal("checking for relative namespaces");
        return (-1);
    }


    memset(&state, 0, sizeof(state));
    xmlC14NVisibleNsStackSave(ctx->ns_rendered, &state);

    if (visible) {	
        if (ctx->parent_is_doc) {
	    
	    parent_is_doc = ctx->parent_is_doc;
	    ctx->parent_is_doc = 0;
            ctx->pos = XMLC14N_INSIDE_DOCUMENT_ELEMENT;
        }
        xmlOutputBufferWriteString(ctx->buf, "<");

        if ((cur->ns != NULL) && (xmlStrlen(cur->ns->prefix) > 0)) {
            xmlOutputBufferWriteString(ctx->buf,
                                       (const char *) cur->ns->prefix);
            xmlOutputBufferWriteString(ctx->buf, ":");
        }
        xmlOutputBufferWriteString(ctx->buf, (const char *) cur->name);
    }

    if (!xmlC14NIsExclusive(ctx)) {
        ret = xmlC14NProcessNamespacesAxis(ctx, cur, visible);
    } else {
        ret = xmlExcC14NProcessNamespacesAxis(ctx, cur, visible);
    }
    if (ret < 0) {
        xmlC14NErrInternal("processing namespaces axis");
        return (-1);
    }
    
    if(visible) {
	xmlC14NVisibleNsStackShift(ctx->ns_rendered);
    }
    
    ret = xmlC14NProcessAttrsAxis(ctx, cur, visible);
    if (ret < 0) {
	xmlC14NErrInternal("processing attributes axis");
    	return (-1);
    }

    if (visible) { 
        xmlOutputBufferWriteString(ctx->buf, ">");
    }
    if (cur->children != NULL) {
        ret = xmlC14NProcessNodeList(ctx, cur->children);
        if (ret < 0) {
            xmlC14NErrInternal("processing childrens list");
            return (-1);
        }
    }
    if (visible) {
        xmlOutputBufferWriteString(ctx->buf, "</");
        if ((cur->ns != NULL) && (xmlStrlen(cur->ns->prefix) > 0)) {
            xmlOutputBufferWriteString(ctx->buf,
                                       (const char *) cur->ns->prefix);
            xmlOutputBufferWriteString(ctx->buf, ":");
        }
        xmlOutputBufferWriteString(ctx->buf, (const char *) cur->name);
        xmlOutputBufferWriteString(ctx->buf, ">");
        if (parent_is_doc) {
	    
            ctx->parent_is_doc = parent_is_doc;
	    ctx->pos = XMLC14N_AFTER_DOCUMENT_ELEMENT;
        }
    }

    xmlC14NVisibleNsStackRestore(ctx->ns_rendered, &state);
    return (0);
}

static int
xmlC14NProcessNode(xmlC14NCtxPtr ctx, xmlNodePtr cur)
{
    int ret = 0;
    int visible;

    if ((ctx == NULL) || (cur == NULL)) {
        xmlC14NErrParam("processing node");
        return (-1);
    }

    visible = xmlC14NIsVisible(ctx, cur, cur->parent);
    switch (cur->type) {
        case XML_ELEMENT_NODE:
            ret = xmlC14NProcessElementNode(ctx, cur, visible);
            break;
        case XML_CDATA_SECTION_NODE:
        case XML_TEXT_NODE:
            
            
            if ((visible) && (cur->content != NULL)) {
                xmlChar *buffer;

                buffer = xmlC11NNormalizeText(cur->content);
                if (buffer != NULL) {
                    xmlOutputBufferWriteString(ctx->buf,
                                               (const char *) buffer);
                    xmlFree(buffer);
                } else {
                    xmlC14NErrInternal("normalizing text node");
                    return (-1);
                }
            }
            break;
        case XML_PI_NODE:
            if (visible) {
                if (ctx->pos == XMLC14N_AFTER_DOCUMENT_ELEMENT) {
                    xmlOutputBufferWriteString(ctx->buf, "\x0A<?");
                } else {
                    xmlOutputBufferWriteString(ctx->buf, "<?");
                }

                xmlOutputBufferWriteString(ctx->buf,
                                           (const char *) cur->name);
                if ((cur->content != NULL) && (*(cur->content) != '\0')) {
                    xmlChar *buffer;

                    xmlOutputBufferWriteString(ctx->buf, " ");

                    
                    buffer = xmlC11NNormalizePI(cur->content);
                    if (buffer != NULL) {
                        xmlOutputBufferWriteString(ctx->buf,
                                                   (const char *) buffer);
                        xmlFree(buffer);
                    } else {
                        xmlC14NErrInternal("normalizing pi node");
                        return (-1);
                    }
                }

                if (ctx->pos == XMLC14N_BEFORE_DOCUMENT_ELEMENT) {
                    xmlOutputBufferWriteString(ctx->buf, "?>\x0A");
                } else {
                    xmlOutputBufferWriteString(ctx->buf, "?>");
                }
            }
            break;
        case XML_COMMENT_NODE:
            if (visible && ctx->with_comments) {
                if (ctx->pos == XMLC14N_AFTER_DOCUMENT_ELEMENT) {
                    xmlOutputBufferWriteString(ctx->buf, "\x0A<!--");
                } else {
                    xmlOutputBufferWriteString(ctx->buf, "<!--");
                }

                if (cur->content != NULL) {
                    xmlChar *buffer;

                    
                    buffer = xmlC11NNormalizeComment(cur->content);
                    if (buffer != NULL) {
                        xmlOutputBufferWriteString(ctx->buf,
                                                   (const char *) buffer);
                        xmlFree(buffer);
                    } else {
                        xmlC14NErrInternal("normalizing comment node");
                        return (-1);
                    }
                }

                if (ctx->pos == XMLC14N_BEFORE_DOCUMENT_ELEMENT) {
                    xmlOutputBufferWriteString(ctx->buf, "-->\x0A");
                } else {
                    xmlOutputBufferWriteString(ctx->buf, "-->");
                }
            }
            break;
        case XML_DOCUMENT_NODE:
        case XML_DOCUMENT_FRAG_NODE:   
#ifdef LIBXML_DOCB_ENABLED
        case XML_DOCB_DOCUMENT_NODE:   
#endif
#ifdef LIBXML_HTML_ENABLED
        case XML_HTML_DOCUMENT_NODE:   
#endif
            if (cur->children != NULL) {
                ctx->pos = XMLC14N_BEFORE_DOCUMENT_ELEMENT;
                ctx->parent_is_doc = 1;
                ret = xmlC14NProcessNodeList(ctx, cur->children);
            }
            break;

        case XML_ATTRIBUTE_NODE:
            xmlC14NErrInvalidNode("XML_ATTRIBUTE_NODE", "processing node");
            return (-1);
        case XML_NAMESPACE_DECL:
            xmlC14NErrInvalidNode("XML_NAMESPACE_DECL", "processing node");
            return (-1);
        case XML_ENTITY_REF_NODE:
            xmlC14NErrInvalidNode("XML_ENTITY_REF_NODE", "processing node");
            return (-1);
        case XML_ENTITY_NODE:
            xmlC14NErrInvalidNode("XML_ENTITY_NODE", "processing node");
            return (-1);

        case XML_DOCUMENT_TYPE_NODE:
        case XML_NOTATION_NODE:
        case XML_DTD_NODE:
        case XML_ELEMENT_DECL:
        case XML_ATTRIBUTE_DECL:
        case XML_ENTITY_DECL:
#ifdef LIBXML_XINCLUDE_ENABLED
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
#endif
            break;
        default:
            xmlC14NErrUnknownNode(cur->type, "processing node");
            return (-1);
    }

    return (ret);
}

static int
xmlC14NProcessNodeList(xmlC14NCtxPtr ctx, xmlNodePtr cur)
{
    int ret;

    if (ctx == NULL) {
        xmlC14NErrParam("processing node list");
        return (-1);
    }

    for (ret = 0; cur != NULL && ret >= 0; cur = cur->next) {
        ret = xmlC14NProcessNode(ctx, cur);
    }
    return (ret);
}



static void
xmlC14NFreeCtx(xmlC14NCtxPtr ctx)
{
    if (ctx == NULL) {
        xmlC14NErrParam("freeing context");
        return;
    }

    if (ctx->ns_rendered != NULL) {
        xmlC14NVisibleNsStackDestroy(ctx->ns_rendered);
    }
    xmlFree(ctx);
}

static xmlC14NCtxPtr
xmlC14NNewCtx(xmlDocPtr doc,  
	      xmlC14NIsVisibleCallback is_visible_callback, void* user_data,
              xmlC14NMode mode, xmlChar ** inclusive_ns_prefixes,
              int with_comments, xmlOutputBufferPtr buf)
{
    xmlC14NCtxPtr ctx = NULL;

    if ((doc == NULL) || (buf == NULL)) {
        xmlC14NErrParam("creating new context");
        return (NULL);
    }

    if (buf->encoder != NULL) {
        xmlC14NErr(ctx, (xmlNodePtr) doc, XML_C14N_REQUIRES_UTF8,
"xmlC14NNewCtx: output buffer encoder != NULL but C14N requires UTF8 output\n");
        return (NULL);
    }

    if (doc->charset != XML_CHAR_ENCODING_UTF8) {
        xmlC14NErr(ctx, (xmlNodePtr) doc, XML_C14N_REQUIRES_UTF8,
		   "xmlC14NNewCtx: source document not in UTF8\n");
        return (NULL);
    }

    ctx = (xmlC14NCtxPtr) xmlMalloc(sizeof(xmlC14NCtx));
    if (ctx == NULL) {
	xmlC14NErrMemory("creating context");
        return (NULL);
    }
    memset(ctx, 0, sizeof(xmlC14NCtx));

    ctx->doc = doc;
    ctx->with_comments = with_comments;
    ctx->is_visible_callback = is_visible_callback;
    ctx->user_data = user_data;
    ctx->buf = buf;
    ctx->parent_is_doc = 1;
    ctx->pos = XMLC14N_BEFORE_DOCUMENT_ELEMENT;
    ctx->ns_rendered = xmlC14NVisibleNsStackCreate();

    if(ctx->ns_rendered == NULL) {
        xmlC14NErr(ctx, (xmlNodePtr) doc, XML_C14N_CREATE_STACK,
		   "xmlC14NNewCtx: xmlC14NVisibleNsStackCreate failed\n");
	xmlC14NFreeCtx(ctx);
        return (NULL);
    }

    ctx->mode = mode;
    if(xmlC14NIsExclusive(ctx)) {
        ctx->inclusive_ns_prefixes = inclusive_ns_prefixes;
    }
    return (ctx);
}

int 		
xmlC14NExecute(xmlDocPtr doc, xmlC14NIsVisibleCallback is_visible_callback,
	 void* user_data, int mode, xmlChar **inclusive_ns_prefixes,
	 int with_comments, xmlOutputBufferPtr buf) {

    xmlC14NCtxPtr ctx;
    xmlC14NMode c14n_mode = XML_C14N_1_0;
    int ret;

    if ((buf == NULL) || (doc == NULL)) {
        xmlC14NErrParam("executing c14n");
        return (-1);
    }

    switch(mode) {
    case XML_C14N_1_0:
    case XML_C14N_EXCLUSIVE_1_0:
    case XML_C14N_1_1: 
         c14n_mode = (xmlC14NMode)mode;
         break;
    default:       
        xmlC14NErrParam("invalid mode for executing c14n");
        return (-1);
    }

    if (buf->encoder != NULL) {
        xmlC14NErr(NULL, (xmlNodePtr) doc, XML_C14N_REQUIRES_UTF8,
"xmlC14NExecute: output buffer encoder != NULL but C14N requires UTF8 output\n");
        return (-1);
    }

    ctx = xmlC14NNewCtx(doc, is_visible_callback, user_data, 
	            c14n_mode, inclusive_ns_prefixes,
                    with_comments, buf);
    if (ctx == NULL) {
        xmlC14NErr(NULL, (xmlNodePtr) doc, XML_C14N_CREATE_CTXT,
		   "xmlC14NExecute: unable to create C14N context\n");
        return (-1);
    }



    if (doc->children != NULL) {
        ret = xmlC14NProcessNodeList(ctx, doc->children);
        if (ret < 0) {
            xmlC14NErrInternal("processing docs children list");
            xmlC14NFreeCtx(ctx);
            return (-1);
        }
    }

    /*
     * Flush buffer to get number of bytes written
     */
    ret = xmlOutputBufferFlush(buf);
    if (ret < 0) {
        xmlC14NErrInternal("flushing output buffer");
        xmlC14NFreeCtx(ctx);
        return (-1);
    }

    xmlC14NFreeCtx(ctx);
    return (ret);
}

int
xmlC14NDocSaveTo(xmlDocPtr doc, xmlNodeSetPtr nodes,
                 int mode, xmlChar ** inclusive_ns_prefixes,
                 int with_comments, xmlOutputBufferPtr buf) {
    return(xmlC14NExecute(doc, 
			(xmlC14NIsVisibleCallback)xmlC14NIsNodeInNodeset,
			nodes,
			mode,
			inclusive_ns_prefixes,
			with_comments,
			buf));
}


/**
 * xmlC14NDocDumpMemory:
 * @doc: 		the XML document for canonization
 * @nodes: 		the nodes set to be included in the canonized image
 *      		or NULL if all document nodes should be included
 * @mode:		the c14n mode (see @xmlC14NMode)
 * @inclusive_ns_prefixes: the list of inclusive namespace prefixes 
 *			ended with a NULL or NULL if there is no
 *			inclusive namespaces (only for exclusive 
 *			canonicalization, ignored otherwise)
 * @with_comments: 	include comments in the result (!=0) or not (==0)
 * @doc_txt_ptr: 	the memory pointer for allocated canonical XML text;
 *			the caller of this functions is responsible for calling
 *			xmlFree() to free allocated memory 
 *  		
 * Dumps the canonized image of given XML document into memory.
 * For details see "Canonical XML" (http://www.w3.org/TR/xml-c14n) or
 * "Exclusive XML Canonicalization" (http://www.w3.org/TR/xml-exc-c14n)
 *
 * Returns the number of bytes written on success or a negative value on fail  
 */
int
xmlC14NDocDumpMemory(xmlDocPtr doc, xmlNodeSetPtr nodes,
                     int mode, xmlChar ** inclusive_ns_prefixes,
                     int with_comments, xmlChar ** doc_txt_ptr)
{
    int ret;
    xmlOutputBufferPtr buf;

    if (doc_txt_ptr == NULL) {
        xmlC14NErrParam("dumping doc to memory");
        return (-1);
    }

    *doc_txt_ptr = NULL;

    buf = xmlAllocOutputBuffer(NULL);
    if (buf == NULL) {
        xmlC14NErrMemory("creating output buffer");
        return (-1);
    }

    ret = xmlC14NDocSaveTo(doc, nodes, mode, inclusive_ns_prefixes,
                           with_comments, buf);
    if (ret < 0) {
        xmlC14NErrInternal("saving doc to output buffer");
        (void) xmlOutputBufferClose(buf);
        return (-1);
    }

    ret = buf->buffer->use;
    if (ret > 0) {
        *doc_txt_ptr = xmlStrndup(buf->buffer->content, ret);
    }
    (void) xmlOutputBufferClose(buf);

    if ((*doc_txt_ptr == NULL) && (ret > 0)) {
        xmlC14NErrMemory("coping canonicanized document");
        return (-1);
    }
    return (ret);
}

/**
 * xmlC14NDocSave:
 * @doc: 		the XML document for canonization
 * @nodes: 		the nodes set to be included in the canonized image
 *      		or NULL if all document nodes should be included
 * @mode:		the c14n mode (see @xmlC14NMode)
 * @inclusive_ns_prefixes: the list of inclusive namespace prefixes 
 *			ended with a NULL or NULL if there is no
 *			inclusive namespaces (only for exclusive 
 *			canonicalization, ignored otherwise)
 * @with_comments: 	include comments in the result (!=0) or not (==0)
 * @filename: 		the filename to store canonical XML image
 * @compression:	the compression level (zlib requred): 
 *				-1 - libxml default,
 *				 0 - uncompressed, 
 *				>0 - compression level
 *  		
 * Dumps the canonized image of given XML document into the file.
 * For details see "Canonical XML" (http://www.w3.org/TR/xml-c14n) or
 * "Exclusive XML Canonicalization" (http://www.w3.org/TR/xml-exc-c14n)
 *
 * Returns the number of bytes written success or a negative value on fail  
 */
int
xmlC14NDocSave(xmlDocPtr doc, xmlNodeSetPtr nodes,
               int mode, xmlChar ** inclusive_ns_prefixes,
               int with_comments, const char *filename, int compression)
{
    xmlOutputBufferPtr buf;
    int ret;

    if (filename == NULL) {
        xmlC14NErrParam("saving doc");
        return (-1);
    }
#ifdef HAVE_ZLIB_H
    if (compression < 0)
        compression = xmlGetCompressMode();
#endif

    buf = xmlOutputBufferCreateFilename(filename, NULL, compression);
    if (buf == NULL) {
        xmlC14NErrInternal("creating temporary filename");
        return (-1);
    }

    ret = xmlC14NDocSaveTo(doc, nodes, mode, inclusive_ns_prefixes,
                           with_comments, buf);
    if (ret < 0) {
        xmlC14NErrInternal("cannicanize document to buffer");
        (void) xmlOutputBufferClose(buf);
        return (-1);
    }

    /* 
     * get the numbers of bytes written 
     */
    ret = xmlOutputBufferClose(buf);
    return (ret);
}



#define growBufferReentrant() {						\
    buffer_size *= 2;							\
    buffer = (xmlChar *)						\
    		xmlRealloc(buffer, buffer_size * sizeof(xmlChar));	\
    if (buffer == NULL) {						\
	xmlC14NErrMemory("growing buffer");				\
	return(NULL);							\
    }									\
}

static xmlChar *
xmlC11NNormalizeString(const xmlChar * input,
                       xmlC14NNormalizationMode mode)
{
    const xmlChar *cur = input;
    xmlChar *buffer = NULL;
    xmlChar *out = NULL;
    int buffer_size = 0;

    if (input == NULL)
        return (NULL);

    buffer_size = 1000;
    buffer = (xmlChar *) xmlMallocAtomic(buffer_size * sizeof(xmlChar));
    if (buffer == NULL) {
	xmlC14NErrMemory("allocating buffer");
        return (NULL);
    }
    out = buffer;

    while (*cur != '\0') {
        if ((out - buffer) > (buffer_size - 10)) {
            int indx = out - buffer;

            growBufferReentrant();
            out = &buffer[indx];
        }

        if ((*cur == '<') && ((mode == XMLC14N_NORMALIZE_ATTR) ||
                              (mode == XMLC14N_NORMALIZE_TEXT))) {
            *out++ = '&';
            *out++ = 'l';
            *out++ = 't';
            *out++ = ';';
        } else if ((*cur == '>') && (mode == XMLC14N_NORMALIZE_TEXT)) {
            *out++ = '&';
            *out++ = 'g';
            *out++ = 't';
            *out++ = ';';
        } else if ((*cur == '&') && ((mode == XMLC14N_NORMALIZE_ATTR) ||
                                     (mode == XMLC14N_NORMALIZE_TEXT))) {
            *out++ = '&';
            *out++ = 'a';
            *out++ = 'm';
            *out++ = 'p';
            *out++ = ';';
        } else if ((*cur == '"') && (mode == XMLC14N_NORMALIZE_ATTR)) {
            *out++ = '&';
            *out++ = 'q';
            *out++ = 'u';
            *out++ = 'o';
            *out++ = 't';
            *out++ = ';';
        } else if ((*cur == '\x09') && (mode == XMLC14N_NORMALIZE_ATTR)) {
            *out++ = '&';
            *out++ = '#';
            *out++ = 'x';
            *out++ = '9';
            *out++ = ';';
        } else if ((*cur == '\x0A') && (mode == XMLC14N_NORMALIZE_ATTR)) {
            *out++ = '&';
            *out++ = '#';
            *out++ = 'x';
            *out++ = 'A';
            *out++ = ';';
        } else if ((*cur == '\x0D') && ((mode == XMLC14N_NORMALIZE_ATTR) ||
                                        (mode == XMLC14N_NORMALIZE_TEXT) ||
                                        (mode == XMLC14N_NORMALIZE_COMMENT) ||
					(mode == XMLC14N_NORMALIZE_PI))) {
            *out++ = '&';
            *out++ = '#';
            *out++ = 'x';
            *out++ = 'D';
            *out++ = ';';
        } else {
            *out++ = *cur;
        }
        cur++;
    }
    *out = 0;
    return (buffer);
}
#endif 
#define bottom_c14n
#include "elfgcchack.h"
#endif 
