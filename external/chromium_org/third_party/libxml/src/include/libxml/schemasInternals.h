/*
 * Summary: internal interfaces for XML Schemas
 * Description: internal interfaces for the XML Schemas handling
 *              and schema validity checking
 *		The Schemas development is a Work In Progress.
 *              Some of those interfaces are not garanteed to be API or ABI stable !
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */


#ifndef __XML_SCHEMA_INTERNALS_H__
#define __XML_SCHEMA_INTERNALS_H__

#include <libxml/xmlversion.h>

#ifdef LIBXML_SCHEMAS_ENABLED

#include <libxml/xmlregexp.h>
#include <libxml/hash.h>
#include <libxml/dict.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    XML_SCHEMAS_UNKNOWN = 0,
    XML_SCHEMAS_STRING,
    XML_SCHEMAS_NORMSTRING,
    XML_SCHEMAS_DECIMAL,
    XML_SCHEMAS_TIME,
    XML_SCHEMAS_GDAY,
    XML_SCHEMAS_GMONTH,
    XML_SCHEMAS_GMONTHDAY,
    XML_SCHEMAS_GYEAR,
    XML_SCHEMAS_GYEARMONTH,
    XML_SCHEMAS_DATE,
    XML_SCHEMAS_DATETIME,
    XML_SCHEMAS_DURATION,
    XML_SCHEMAS_FLOAT,
    XML_SCHEMAS_DOUBLE,
    XML_SCHEMAS_BOOLEAN,
    XML_SCHEMAS_TOKEN,
    XML_SCHEMAS_LANGUAGE,
    XML_SCHEMAS_NMTOKEN,
    XML_SCHEMAS_NMTOKENS,
    XML_SCHEMAS_NAME,
    XML_SCHEMAS_QNAME,
    XML_SCHEMAS_NCNAME,
    XML_SCHEMAS_ID,
    XML_SCHEMAS_IDREF,
    XML_SCHEMAS_IDREFS,
    XML_SCHEMAS_ENTITY,
    XML_SCHEMAS_ENTITIES,
    XML_SCHEMAS_NOTATION,
    XML_SCHEMAS_ANYURI,
    XML_SCHEMAS_INTEGER,
    XML_SCHEMAS_NPINTEGER,
    XML_SCHEMAS_NINTEGER,
    XML_SCHEMAS_NNINTEGER,
    XML_SCHEMAS_PINTEGER,
    XML_SCHEMAS_INT,
    XML_SCHEMAS_UINT,
    XML_SCHEMAS_LONG,
    XML_SCHEMAS_ULONG,
    XML_SCHEMAS_SHORT,
    XML_SCHEMAS_USHORT,
    XML_SCHEMAS_BYTE,
    XML_SCHEMAS_UBYTE,
    XML_SCHEMAS_HEXBINARY,
    XML_SCHEMAS_BASE64BINARY,
    XML_SCHEMAS_ANYTYPE,
    XML_SCHEMAS_ANYSIMPLETYPE
} xmlSchemaValType;

typedef enum {
    XML_SCHEMA_TYPE_BASIC = 1, 
    XML_SCHEMA_TYPE_ANY,
    XML_SCHEMA_TYPE_FACET,
    XML_SCHEMA_TYPE_SIMPLE,
    XML_SCHEMA_TYPE_COMPLEX,
    XML_SCHEMA_TYPE_SEQUENCE = 6,
    XML_SCHEMA_TYPE_CHOICE,
    XML_SCHEMA_TYPE_ALL,
    XML_SCHEMA_TYPE_SIMPLE_CONTENT,
    XML_SCHEMA_TYPE_COMPLEX_CONTENT,
    XML_SCHEMA_TYPE_UR,
    XML_SCHEMA_TYPE_RESTRICTION,
    XML_SCHEMA_TYPE_EXTENSION,
    XML_SCHEMA_TYPE_ELEMENT,
    XML_SCHEMA_TYPE_ATTRIBUTE,
    XML_SCHEMA_TYPE_ATTRIBUTEGROUP,
    XML_SCHEMA_TYPE_GROUP,
    XML_SCHEMA_TYPE_NOTATION,
    XML_SCHEMA_TYPE_LIST,
    XML_SCHEMA_TYPE_UNION,
    XML_SCHEMA_TYPE_ANY_ATTRIBUTE,
    XML_SCHEMA_TYPE_IDC_UNIQUE,
    XML_SCHEMA_TYPE_IDC_KEY,
    XML_SCHEMA_TYPE_IDC_KEYREF,
    XML_SCHEMA_TYPE_PARTICLE = 25, 
    XML_SCHEMA_TYPE_ATTRIBUTE_USE, 
    XML_SCHEMA_FACET_MININCLUSIVE = 1000,
    XML_SCHEMA_FACET_MINEXCLUSIVE,
    XML_SCHEMA_FACET_MAXINCLUSIVE,
    XML_SCHEMA_FACET_MAXEXCLUSIVE,
    XML_SCHEMA_FACET_TOTALDIGITS,
    XML_SCHEMA_FACET_FRACTIONDIGITS,
    XML_SCHEMA_FACET_PATTERN,
    XML_SCHEMA_FACET_ENUMERATION,
    XML_SCHEMA_FACET_WHITESPACE,
    XML_SCHEMA_FACET_LENGTH,
    XML_SCHEMA_FACET_MAXLENGTH,
    XML_SCHEMA_FACET_MINLENGTH,
    XML_SCHEMA_EXTRA_QNAMEREF = 2000,
    XML_SCHEMA_EXTRA_ATTR_USE_PROHIB
} xmlSchemaTypeType;

typedef enum {
    XML_SCHEMA_CONTENT_UNKNOWN = 0,
    XML_SCHEMA_CONTENT_EMPTY = 1,
    XML_SCHEMA_CONTENT_ELEMENTS,
    XML_SCHEMA_CONTENT_MIXED,
    XML_SCHEMA_CONTENT_SIMPLE,
    XML_SCHEMA_CONTENT_MIXED_OR_ELEMENTS, 
    XML_SCHEMA_CONTENT_BASIC,
    XML_SCHEMA_CONTENT_ANY
} xmlSchemaContentType;

typedef struct _xmlSchemaVal xmlSchemaVal;
typedef xmlSchemaVal *xmlSchemaValPtr;

typedef struct _xmlSchemaType xmlSchemaType;
typedef xmlSchemaType *xmlSchemaTypePtr;

typedef struct _xmlSchemaFacet xmlSchemaFacet;
typedef xmlSchemaFacet *xmlSchemaFacetPtr;

typedef struct _xmlSchemaAnnot xmlSchemaAnnot;
typedef xmlSchemaAnnot *xmlSchemaAnnotPtr;
struct _xmlSchemaAnnot {
    struct _xmlSchemaAnnot *next;
    xmlNodePtr content;         
};

#define XML_SCHEMAS_ANYATTR_SKIP        1
#define XML_SCHEMAS_ANYATTR_LAX                2
#define XML_SCHEMAS_ANYATTR_STRICT        3
#define XML_SCHEMAS_ANY_SKIP        1
#define XML_SCHEMAS_ANY_LAX                2
#define XML_SCHEMAS_ANY_STRICT        3
#define XML_SCHEMAS_ATTR_USE_PROHIBITED 0
#define XML_SCHEMAS_ATTR_USE_REQUIRED 1
#define XML_SCHEMAS_ATTR_USE_OPTIONAL 2
#define XML_SCHEMAS_ATTR_GLOBAL        1 << 0
#define XML_SCHEMAS_ATTR_NSDEFAULT        1 << 7
#define XML_SCHEMAS_ATTR_INTERNAL_RESOLVED        1 << 8
#define XML_SCHEMAS_ATTR_FIXED        1 << 9


typedef struct _xmlSchemaAttribute xmlSchemaAttribute;
typedef xmlSchemaAttribute *xmlSchemaAttributePtr;
struct _xmlSchemaAttribute {
    xmlSchemaTypeType type;
    struct _xmlSchemaAttribute *next; 
    const xmlChar *name; 
    const xmlChar *id; 
    const xmlChar *ref; 
    const xmlChar *refNs; 
    const xmlChar *typeName; 
    const xmlChar *typeNs; 
    xmlSchemaAnnotPtr annot;

    xmlSchemaTypePtr base; 
    int occurs; 
    const xmlChar *defValue; 
    xmlSchemaTypePtr subtypes; 
    xmlNodePtr node;
    const xmlChar *targetNamespace;
    int flags;
    const xmlChar *refPrefix; 
    xmlSchemaValPtr defVal; 
    xmlSchemaAttributePtr refDecl; 
};

typedef struct _xmlSchemaAttributeLink xmlSchemaAttributeLink;
typedef xmlSchemaAttributeLink *xmlSchemaAttributeLinkPtr;
struct _xmlSchemaAttributeLink {
    struct _xmlSchemaAttributeLink *next;
    struct _xmlSchemaAttribute *attr;
};

#define XML_SCHEMAS_WILDCARD_COMPLETE 1 << 0

typedef struct _xmlSchemaWildcardNs xmlSchemaWildcardNs;
typedef xmlSchemaWildcardNs *xmlSchemaWildcardNsPtr;
struct _xmlSchemaWildcardNs {
    struct _xmlSchemaWildcardNs *next;
    const xmlChar *value;
};

typedef struct _xmlSchemaWildcard xmlSchemaWildcard;
typedef xmlSchemaWildcard *xmlSchemaWildcardPtr;
struct _xmlSchemaWildcard {
    xmlSchemaTypeType type;        
    const xmlChar *id; 
    xmlSchemaAnnotPtr annot;
    xmlNodePtr node;
    int minOccurs; 
    int maxOccurs; 
    int processContents;
    int any; 
    xmlSchemaWildcardNsPtr nsSet; 
    xmlSchemaWildcardNsPtr negNsSet; 
    int flags;
};

#define XML_SCHEMAS_ATTRGROUP_WILDCARD_BUILDED 1 << 0
#define XML_SCHEMAS_ATTRGROUP_GLOBAL 1 << 1
#define XML_SCHEMAS_ATTRGROUP_MARKED 1 << 2

#define XML_SCHEMAS_ATTRGROUP_REDEFINED 1 << 3
#define XML_SCHEMAS_ATTRGROUP_HAS_REFS 1 << 4

typedef struct _xmlSchemaAttributeGroup xmlSchemaAttributeGroup;
typedef xmlSchemaAttributeGroup *xmlSchemaAttributeGroupPtr;
struct _xmlSchemaAttributeGroup {
    xmlSchemaTypeType type;        
    struct _xmlSchemaAttribute *next;
    const xmlChar *name;
    const xmlChar *id;
    const xmlChar *ref; 
    const xmlChar *refNs; 
    xmlSchemaAnnotPtr annot;

    xmlSchemaAttributePtr attributes; 
    xmlNodePtr node;
    int flags;
    xmlSchemaWildcardPtr attributeWildcard;
    const xmlChar *refPrefix; 
    xmlSchemaAttributeGroupPtr refItem; 
    const xmlChar *targetNamespace;
    void *attrUses;
};

typedef struct _xmlSchemaTypeLink xmlSchemaTypeLink;
typedef xmlSchemaTypeLink *xmlSchemaTypeLinkPtr;
struct _xmlSchemaTypeLink {
    struct _xmlSchemaTypeLink *next;
    xmlSchemaTypePtr type;
};

typedef struct _xmlSchemaFacetLink xmlSchemaFacetLink;
typedef xmlSchemaFacetLink *xmlSchemaFacetLinkPtr;
struct _xmlSchemaFacetLink {
    struct _xmlSchemaFacetLink *next;
    xmlSchemaFacetPtr facet;
};

#define XML_SCHEMAS_TYPE_MIXED                1 << 0
#define XML_SCHEMAS_TYPE_DERIVATION_METHOD_EXTENSION                1 << 1
#define XML_SCHEMAS_TYPE_DERIVATION_METHOD_RESTRICTION                1 << 2
#define XML_SCHEMAS_TYPE_GLOBAL                1 << 3
#define XML_SCHEMAS_TYPE_OWNED_ATTR_WILDCARD    1 << 4 
#define XML_SCHEMAS_TYPE_VARIETY_ABSENT    1 << 5
#define XML_SCHEMAS_TYPE_VARIETY_LIST    1 << 6
#define XML_SCHEMAS_TYPE_VARIETY_UNION    1 << 7
#define XML_SCHEMAS_TYPE_VARIETY_ATOMIC    1 << 8
#define XML_SCHEMAS_TYPE_FINAL_EXTENSION    1 << 9
#define XML_SCHEMAS_TYPE_FINAL_RESTRICTION    1 << 10
#define XML_SCHEMAS_TYPE_FINAL_LIST    1 << 11
#define XML_SCHEMAS_TYPE_FINAL_UNION    1 << 12
#define XML_SCHEMAS_TYPE_FINAL_DEFAULT    1 << 13
#define XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE    1 << 14
#define XML_SCHEMAS_TYPE_MARKED        1 << 16
#define XML_SCHEMAS_TYPE_BLOCK_DEFAULT    1 << 17
#define XML_SCHEMAS_TYPE_BLOCK_EXTENSION    1 << 18
#define XML_SCHEMAS_TYPE_BLOCK_RESTRICTION    1 << 19
#define XML_SCHEMAS_TYPE_ABSTRACT    1 << 20
#define XML_SCHEMAS_TYPE_FACETSNEEDVALUE    1 << 21
#define XML_SCHEMAS_TYPE_INTERNAL_RESOLVED    1 << 22
#define XML_SCHEMAS_TYPE_INTERNAL_INVALID    1 << 23
#define XML_SCHEMAS_TYPE_WHITESPACE_PRESERVE    1 << 24
#define XML_SCHEMAS_TYPE_WHITESPACE_REPLACE    1 << 25
#define XML_SCHEMAS_TYPE_WHITESPACE_COLLAPSE    1 << 26
#define XML_SCHEMAS_TYPE_HAS_FACETS    1 << 27
#define XML_SCHEMAS_TYPE_NORMVALUENEEDED    1 << 28

#define XML_SCHEMAS_TYPE_FIXUP_1    1 << 29

#define XML_SCHEMAS_TYPE_REDEFINED    1 << 30

struct _xmlSchemaType {
    xmlSchemaTypeType type; 
    struct _xmlSchemaType *next; 
    const xmlChar *name;
    const xmlChar *id ; 
    const xmlChar *ref; 
    const xmlChar *refNs; 
    xmlSchemaAnnotPtr annot;
    xmlSchemaTypePtr subtypes;
    xmlSchemaAttributePtr attributes; 
    xmlNodePtr node;
    int minOccurs; 
    int maxOccurs; 

    int flags;
    xmlSchemaContentType contentType;
    const xmlChar *base; 
    const xmlChar *baseNs; 
    xmlSchemaTypePtr baseType; 
    xmlSchemaFacetPtr facets; 
    struct _xmlSchemaType *redef; 
    int recurse; 
    xmlSchemaAttributeLinkPtr *attributeUses; 
    xmlSchemaWildcardPtr attributeWildcard;
    int builtInType; 
    xmlSchemaTypeLinkPtr memberTypes; 
    xmlSchemaFacetLinkPtr facetSet; 
    const xmlChar *refPrefix; 
    xmlSchemaTypePtr contentTypeDef; 
    xmlRegexpPtr contModel; 
    const xmlChar *targetNamespace;
    void *attrUses;
};

#define XML_SCHEMAS_ELEM_NILLABLE        1 << 0
#define XML_SCHEMAS_ELEM_GLOBAL                1 << 1
#define XML_SCHEMAS_ELEM_DEFAULT        1 << 2
#define XML_SCHEMAS_ELEM_FIXED                1 << 3
#define XML_SCHEMAS_ELEM_ABSTRACT        1 << 4
#define XML_SCHEMAS_ELEM_TOPLEVEL        1 << 5
#define XML_SCHEMAS_ELEM_REF                1 << 6
#define XML_SCHEMAS_ELEM_NSDEFAULT        1 << 7
#define XML_SCHEMAS_ELEM_INTERNAL_RESOLVED        1 << 8
#define XML_SCHEMAS_ELEM_CIRCULAR        1 << 9
#define XML_SCHEMAS_ELEM_BLOCK_ABSENT        1 << 10
#define XML_SCHEMAS_ELEM_BLOCK_EXTENSION        1 << 11
#define XML_SCHEMAS_ELEM_BLOCK_RESTRICTION        1 << 12
#define XML_SCHEMAS_ELEM_BLOCK_SUBSTITUTION        1 << 13
#define XML_SCHEMAS_ELEM_FINAL_ABSENT        1 << 14
#define XML_SCHEMAS_ELEM_FINAL_EXTENSION        1 << 15
#define XML_SCHEMAS_ELEM_FINAL_RESTRICTION        1 << 16
#define XML_SCHEMAS_ELEM_SUBST_GROUP_HEAD        1 << 17
#define XML_SCHEMAS_ELEM_INTERNAL_CHECKED        1 << 18

typedef struct _xmlSchemaElement xmlSchemaElement;
typedef xmlSchemaElement *xmlSchemaElementPtr;
struct _xmlSchemaElement {
    xmlSchemaTypeType type; 
    struct _xmlSchemaType *next; 
    const xmlChar *name;
    const xmlChar *id; 
    const xmlChar *ref; 
    const xmlChar *refNs; 
    xmlSchemaAnnotPtr annot;
    xmlSchemaTypePtr subtypes; 
    xmlSchemaAttributePtr attributes;
    xmlNodePtr node;
    int minOccurs; 
    int maxOccurs; 

    int flags;
    const xmlChar *targetNamespace;
    const xmlChar *namedType;
    const xmlChar *namedTypeNs;
    const xmlChar *substGroup;
    const xmlChar *substGroupNs;
    const xmlChar *scope;
    const xmlChar *value; 
    struct _xmlSchemaElement *refDecl; 
    xmlRegexpPtr contModel; 
    xmlSchemaContentType contentType;
    const xmlChar *refPrefix; 
    xmlSchemaValPtr defVal; 
    void *idcs; 
};

#define XML_SCHEMAS_FACET_UNKNOWN        0
#define XML_SCHEMAS_FACET_PRESERVE        1
#define XML_SCHEMAS_FACET_REPLACE        2
#define XML_SCHEMAS_FACET_COLLAPSE        3
struct _xmlSchemaFacet {
    xmlSchemaTypeType type;        
    struct _xmlSchemaFacet *next;
    const xmlChar *value; 
    const xmlChar *id; 
    xmlSchemaAnnotPtr annot;
    xmlNodePtr node;
    int fixed; 
    int whitespace;
    xmlSchemaValPtr val; 
    xmlRegexpPtr    regexp; 
};

typedef struct _xmlSchemaNotation xmlSchemaNotation;
typedef xmlSchemaNotation *xmlSchemaNotationPtr;
struct _xmlSchemaNotation {
    xmlSchemaTypeType type; 
    const xmlChar *name;
    xmlSchemaAnnotPtr annot;
    const xmlChar *identifier;
    const xmlChar *targetNamespace;
};

#define XML_SCHEMAS_QUALIF_ELEM                1 << 0
#define XML_SCHEMAS_QUALIF_ATTR            1 << 1
#define XML_SCHEMAS_FINAL_DEFAULT_EXTENSION        1 << 2
#define XML_SCHEMAS_FINAL_DEFAULT_RESTRICTION            1 << 3
#define XML_SCHEMAS_FINAL_DEFAULT_LIST            1 << 4
#define XML_SCHEMAS_FINAL_DEFAULT_UNION            1 << 5
#define XML_SCHEMAS_BLOCK_DEFAULT_EXTENSION            1 << 6
#define XML_SCHEMAS_BLOCK_DEFAULT_RESTRICTION            1 << 7
#define XML_SCHEMAS_BLOCK_DEFAULT_SUBSTITUTION            1 << 8
#define XML_SCHEMAS_INCLUDING_CONVERT_NS            1 << 9
struct _xmlSchema {
    const xmlChar *name; 
    const xmlChar *targetNamespace; 
    const xmlChar *version;
    const xmlChar *id; 
    xmlDocPtr doc;
    xmlSchemaAnnotPtr annot;
    int flags;

    xmlHashTablePtr typeDecl;
    xmlHashTablePtr attrDecl;
    xmlHashTablePtr attrgrpDecl;
    xmlHashTablePtr elemDecl;
    xmlHashTablePtr notaDecl;

    xmlHashTablePtr schemasImports;

    void *_private;        
    xmlHashTablePtr groupDecl;
    xmlDictPtr      dict;
    void *includes;     
    int preserve;        
    int counter; 
    xmlHashTablePtr idcDef; 
    void *volatiles; 
};

XMLPUBFUN void XMLCALL         xmlSchemaFreeType        (xmlSchemaTypePtr type);
XMLPUBFUN void XMLCALL         xmlSchemaFreeWildcard(xmlSchemaWildcardPtr wildcard);

#ifdef __cplusplus
}
#endif

#endif 
#endif 
