/*
 * schemastypes.c : implementation of the XML Schema Datatypes
 *             definition and validity checking
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
#include <libxml/valid.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>

#include <libxml/xmlschemas.h>
#include <libxml/schemasInternals.h>
#include <libxml/xmlschemastypes.h>

#ifdef HAVE_MATH_H
#include <math.h>
#endif
#ifdef HAVE_FLOAT_H
#include <float.h>
#endif

#define DEBUG

#ifndef LIBXML_XPATH_ENABLED
extern double xmlXPathNAN;
extern double xmlXPathPINF;
extern double xmlXPathNINF;
#endif

#define TODO 								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);

#define XML_SCHEMAS_NAMESPACE_NAME \
    (const xmlChar *)"http://www.w3.org/2001/XMLSchema"

#define IS_WSP_REPLACE_CH(c)	((((c) == 0x9) || ((c) == 0xa)) || \
				 ((c) == 0xd))

#define IS_WSP_SPACE_CH(c)	((c) == 0x20)

#define IS_WSP_BLANK_CH(c) IS_BLANK_CH(c)

typedef struct _xmlSchemaValDate xmlSchemaValDate;
typedef xmlSchemaValDate *xmlSchemaValDatePtr;
struct _xmlSchemaValDate {
    long		year;
    unsigned int	mon	:4;	
    unsigned int	day	:5;	
    unsigned int	hour	:5;	
    unsigned int	min	:6;	
    double		sec;
    unsigned int	tz_flag	:1;	
    signed int		tzo	:12;	
};

typedef struct _xmlSchemaValDuration xmlSchemaValDuration;
typedef xmlSchemaValDuration *xmlSchemaValDurationPtr;
struct _xmlSchemaValDuration {
    long	        mon;		
    long        	day;
    double		sec;            
};

typedef struct _xmlSchemaValDecimal xmlSchemaValDecimal;
typedef xmlSchemaValDecimal *xmlSchemaValDecimalPtr;
struct _xmlSchemaValDecimal {
    
    unsigned long lo;
    unsigned long mi;
    unsigned long hi;
    unsigned int extra;
    unsigned int sign:1;
    unsigned int frac:7;
    unsigned int total:8;
};

typedef struct _xmlSchemaValQName xmlSchemaValQName;
typedef xmlSchemaValQName *xmlSchemaValQNamePtr;
struct _xmlSchemaValQName {
    xmlChar *name;
    xmlChar *uri;
};

typedef struct _xmlSchemaValHex xmlSchemaValHex;
typedef xmlSchemaValHex *xmlSchemaValHexPtr;
struct _xmlSchemaValHex {
    xmlChar     *str;
    unsigned int total;
};

typedef struct _xmlSchemaValBase64 xmlSchemaValBase64;
typedef xmlSchemaValBase64 *xmlSchemaValBase64Ptr;
struct _xmlSchemaValBase64 {
    xmlChar     *str;
    unsigned int total;
};

struct _xmlSchemaVal {
    xmlSchemaValType type;
    struct _xmlSchemaVal *next;
    union {
	xmlSchemaValDecimal     decimal;
        xmlSchemaValDate        date;
        xmlSchemaValDuration    dur;
	xmlSchemaValQName	qname;
	xmlSchemaValHex		hex;
	xmlSchemaValBase64	base64;
	float			f;
	double			d;
	int			b;
	xmlChar                *str;
    } value;
};

static int xmlSchemaTypesInitialized = 0;
static xmlHashTablePtr xmlSchemaTypesBank = NULL;

static xmlSchemaTypePtr xmlSchemaTypeStringDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeAnyTypeDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeAnySimpleTypeDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeDecimalDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeDatetimeDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeDateDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeTimeDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeGYearDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeGYearMonthDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeGDayDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeGMonthDayDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeGMonthDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeDurationDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeFloatDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeBooleanDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeDoubleDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeHexBinaryDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeBase64BinaryDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeAnyURIDef = NULL;

static xmlSchemaTypePtr xmlSchemaTypePositiveIntegerDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNonPositiveIntegerDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNegativeIntegerDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNonNegativeIntegerDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeIntegerDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeLongDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeIntDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeShortDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeByteDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeUnsignedLongDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeUnsignedIntDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeUnsignedShortDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeUnsignedByteDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNormStringDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeTokenDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeLanguageDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNameDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeQNameDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNCNameDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeIdDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeIdrefDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeIdrefsDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeEntityDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeEntitiesDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNotationDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNmtokenDef = NULL;
static xmlSchemaTypePtr xmlSchemaTypeNmtokensDef = NULL;

static void
xmlSchemaTypeErrMemory(xmlNodePtr node, const char *extra)
{
    __xmlSimpleError(XML_FROM_DATATYPE, XML_ERR_NO_MEMORY, node, NULL, extra);
}


static xmlSchemaValPtr
xmlSchemaNewValue(xmlSchemaValType type) {
    xmlSchemaValPtr value;

    value = (xmlSchemaValPtr) xmlMalloc(sizeof(xmlSchemaVal));
    if (value == NULL) {
	return(NULL);
    }
    memset(value, 0, sizeof(xmlSchemaVal));
    value->type = type;
    return(value);
}

static xmlSchemaFacetPtr
xmlSchemaNewMinLengthFacet(int value)
{
    xmlSchemaFacetPtr ret;

    ret = xmlSchemaNewFacet();
    if (ret == NULL) {
        return(NULL);
    }
    ret->type = XML_SCHEMA_FACET_MINLENGTH;
    ret->val = xmlSchemaNewValue(XML_SCHEMAS_NNINTEGER);
    ret->val->value.decimal.lo = value;
    return (ret);
}

static xmlSchemaTypePtr
xmlSchemaInitBasicType(const char *name, xmlSchemaValType type, 
		       xmlSchemaTypePtr baseType) {
    xmlSchemaTypePtr ret;

    ret = (xmlSchemaTypePtr) xmlMalloc(sizeof(xmlSchemaType));
    if (ret == NULL) {
        xmlSchemaTypeErrMemory(NULL, "could not initialize basic types");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaType));
    ret->name = (const xmlChar *)name;
    ret->targetNamespace = XML_SCHEMAS_NAMESPACE_NAME;
    ret->type = XML_SCHEMA_TYPE_BASIC;
    ret->baseType = baseType;	
    ret->contentType = XML_SCHEMA_CONTENT_BASIC;
    switch (type) {		
	case XML_SCHEMAS_STRING:            
	case XML_SCHEMAS_DECIMAL:    
	case XML_SCHEMAS_DATE:    
	case XML_SCHEMAS_DATETIME:    
	case XML_SCHEMAS_TIME:    
	case XML_SCHEMAS_GYEAR:    
	case XML_SCHEMAS_GYEARMONTH:    
	case XML_SCHEMAS_GMONTH:    
	case XML_SCHEMAS_GMONTHDAY:    
	case XML_SCHEMAS_GDAY:    
	case XML_SCHEMAS_DURATION:    
	case XML_SCHEMAS_FLOAT:    
	case XML_SCHEMAS_DOUBLE:    
	case XML_SCHEMAS_BOOLEAN:    
	case XML_SCHEMAS_ANYURI:    
	case XML_SCHEMAS_HEXBINARY:    
	case XML_SCHEMAS_BASE64BINARY:	
	case XML_SCHEMAS_QNAME:	
	case XML_SCHEMAS_NOTATION:	
	    ret->flags |= XML_SCHEMAS_TYPE_BUILTIN_PRIMITIVE;
	    break;
	default:
	    break;
    }
    switch (type) {
	case XML_SCHEMAS_ANYTYPE:
	case XML_SCHEMAS_ANYSIMPLETYPE:
	    break;
	case XML_SCHEMAS_IDREFS:
	case XML_SCHEMAS_NMTOKENS:
	case XML_SCHEMAS_ENTITIES:
	    ret->flags |= XML_SCHEMAS_TYPE_VARIETY_LIST;
	    ret->facets = xmlSchemaNewMinLengthFacet(1);
	    ret->flags |= XML_SCHEMAS_TYPE_HAS_FACETS;	    
	    break;
	default:
	    ret->flags |= XML_SCHEMAS_TYPE_VARIETY_ATOMIC;
	    break;
    }
    xmlHashAddEntry2(xmlSchemaTypesBank, ret->name,
	             XML_SCHEMAS_NAMESPACE_NAME, ret);
    ret->builtInType = type;
    return(ret);
}

#define UNBOUNDED (1 << 30)

typedef struct _xmlSchemaTreeItem xmlSchemaTreeItem;
typedef xmlSchemaTreeItem *xmlSchemaTreeItemPtr;
struct _xmlSchemaTreeItem {
    xmlSchemaTypeType type;
    xmlSchemaAnnotPtr annot;
    xmlSchemaTreeItemPtr next;
    xmlSchemaTreeItemPtr children;
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

static xmlSchemaParticlePtr
xmlSchemaAddParticle(void)
{
    xmlSchemaParticlePtr ret = NULL;

    ret = (xmlSchemaParticlePtr)
	xmlMalloc(sizeof(xmlSchemaParticle));
    if (ret == NULL) {
	xmlSchemaTypeErrMemory(NULL, "allocating particle component");
	return (NULL);
    }
    memset(ret, 0, sizeof(xmlSchemaParticle));
    ret->type = XML_SCHEMA_TYPE_PARTICLE;
    ret->minOccurs = 1;
    ret->maxOccurs = 1;
    return (ret);
}

void
xmlSchemaInitTypes(void)
{
    if (xmlSchemaTypesInitialized != 0)
        return;
    xmlSchemaTypesBank = xmlHashCreate(40);

    
    xmlSchemaTypeAnyTypeDef = xmlSchemaInitBasicType("anyType",
                                                     XML_SCHEMAS_ANYTYPE, 
						     NULL);
    xmlSchemaTypeAnyTypeDef->baseType = xmlSchemaTypeAnyTypeDef;
    xmlSchemaTypeAnyTypeDef->contentType = XML_SCHEMA_CONTENT_MIXED;
    xmlSchemaTypeAnyTypeDef->contentType = XML_SCHEMA_CONTENT_MIXED;    
    {
	xmlSchemaParticlePtr particle;
	xmlSchemaModelGroupPtr sequence;
	xmlSchemaWildcardPtr wild;
	
	particle = xmlSchemaAddParticle();
	if (particle == NULL)
	    return;
	xmlSchemaTypeAnyTypeDef->subtypes = (xmlSchemaTypePtr) particle;
	
	sequence = (xmlSchemaModelGroupPtr)
	    xmlMalloc(sizeof(xmlSchemaModelGroup));
	if (sequence == NULL) {
	    xmlSchemaTypeErrMemory(NULL, "allocating model group component");
	    return;
	}
	memset(sequence, 0, sizeof(xmlSchemaModelGroup));
	sequence->type = XML_SCHEMA_TYPE_SEQUENCE;	
	particle->children = (xmlSchemaTreeItemPtr) sequence;
	
	particle = xmlSchemaAddParticle();
	if (particle == NULL)
	    return;
	particle->minOccurs = 0;
	particle->maxOccurs = UNBOUNDED;
	sequence->children = (xmlSchemaTreeItemPtr) particle;
	
	wild = (xmlSchemaWildcardPtr) xmlMalloc(sizeof(xmlSchemaWildcard));
	if (wild == NULL) {
	    xmlSchemaTypeErrMemory(NULL, "allocating wildcard component");
	    return;
	}
	memset(wild, 0, sizeof(xmlSchemaWildcard));
	wild->type = XML_SCHEMA_TYPE_ANY;
	wild->any = 1;	
	wild->processContents = XML_SCHEMAS_ANY_LAX;	
	particle->children = (xmlSchemaTreeItemPtr) wild;    
	wild = (xmlSchemaWildcardPtr) xmlMalloc(sizeof(xmlSchemaWildcard));
	if (wild == NULL) {
	    xmlSchemaTypeErrMemory(NULL, "could not create an attribute "
		"wildcard on anyType");
	    return;
	}
	memset(wild, 0, sizeof(xmlSchemaWildcard));
	wild->any = 1;
	wild->processContents = XML_SCHEMAS_ANY_LAX;	
	xmlSchemaTypeAnyTypeDef->attributeWildcard = wild;
    }
    xmlSchemaTypeAnySimpleTypeDef = xmlSchemaInitBasicType("anySimpleType", 
                                                           XML_SCHEMAS_ANYSIMPLETYPE,
							   xmlSchemaTypeAnyTypeDef);
    xmlSchemaTypeStringDef = xmlSchemaInitBasicType("string",
                                                    XML_SCHEMAS_STRING,
						    xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeDecimalDef = xmlSchemaInitBasicType("decimal",
                                                     XML_SCHEMAS_DECIMAL,
						     xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeDateDef = xmlSchemaInitBasicType("date",
                                                  XML_SCHEMAS_DATE,
						  xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeDatetimeDef = xmlSchemaInitBasicType("dateTime",
                                                      XML_SCHEMAS_DATETIME,
						      xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeTimeDef = xmlSchemaInitBasicType("time",
                                                  XML_SCHEMAS_TIME,
						  xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeGYearDef = xmlSchemaInitBasicType("gYear",
                                                   XML_SCHEMAS_GYEAR,
						   xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeGYearMonthDef = xmlSchemaInitBasicType("gYearMonth",
                                                        XML_SCHEMAS_GYEARMONTH,
							xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeGMonthDef = xmlSchemaInitBasicType("gMonth",
                                                    XML_SCHEMAS_GMONTH,
						    xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeGMonthDayDef = xmlSchemaInitBasicType("gMonthDay",
                                                       XML_SCHEMAS_GMONTHDAY,
						       xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeGDayDef = xmlSchemaInitBasicType("gDay",
                                                  XML_SCHEMAS_GDAY,
						  xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeDurationDef = xmlSchemaInitBasicType("duration",
                                                      XML_SCHEMAS_DURATION,
						      xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeFloatDef = xmlSchemaInitBasicType("float",
                                                   XML_SCHEMAS_FLOAT,
						   xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeDoubleDef = xmlSchemaInitBasicType("double",
                                                    XML_SCHEMAS_DOUBLE,
						    xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeBooleanDef = xmlSchemaInitBasicType("boolean",
                                                     XML_SCHEMAS_BOOLEAN,
						     xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeAnyURIDef = xmlSchemaInitBasicType("anyURI",
                                                    XML_SCHEMAS_ANYURI,
						    xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeHexBinaryDef = xmlSchemaInitBasicType("hexBinary",
                                                     XML_SCHEMAS_HEXBINARY,
						     xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeBase64BinaryDef
        = xmlSchemaInitBasicType("base64Binary", XML_SCHEMAS_BASE64BINARY,
	xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeNotationDef = xmlSchemaInitBasicType("NOTATION",
                                                    XML_SCHEMAS_NOTATION,
						    xmlSchemaTypeAnySimpleTypeDef);    
    xmlSchemaTypeQNameDef = xmlSchemaInitBasicType("QName",
                                                   XML_SCHEMAS_QNAME,
						   xmlSchemaTypeAnySimpleTypeDef);

    xmlSchemaTypeIntegerDef = xmlSchemaInitBasicType("integer",
                                                     XML_SCHEMAS_INTEGER,
						     xmlSchemaTypeDecimalDef);
    xmlSchemaTypeNonPositiveIntegerDef =
        xmlSchemaInitBasicType("nonPositiveInteger",
                               XML_SCHEMAS_NPINTEGER,
			       xmlSchemaTypeIntegerDef);
    xmlSchemaTypeNegativeIntegerDef =
        xmlSchemaInitBasicType("negativeInteger", XML_SCHEMAS_NINTEGER,
	xmlSchemaTypeNonPositiveIntegerDef);
    xmlSchemaTypeLongDef =
        xmlSchemaInitBasicType("long", XML_SCHEMAS_LONG,
	xmlSchemaTypeIntegerDef);
    xmlSchemaTypeIntDef = xmlSchemaInitBasicType("int", XML_SCHEMAS_INT,
	xmlSchemaTypeLongDef);
    xmlSchemaTypeShortDef = xmlSchemaInitBasicType("short",
                                                   XML_SCHEMAS_SHORT,
						   xmlSchemaTypeIntDef);
    xmlSchemaTypeByteDef = xmlSchemaInitBasicType("byte",
                                                  XML_SCHEMAS_BYTE,
						  xmlSchemaTypeShortDef);
    xmlSchemaTypeNonNegativeIntegerDef =
        xmlSchemaInitBasicType("nonNegativeInteger",
                               XML_SCHEMAS_NNINTEGER,
			       xmlSchemaTypeIntegerDef);
    xmlSchemaTypeUnsignedLongDef =
        xmlSchemaInitBasicType("unsignedLong", XML_SCHEMAS_ULONG,
	xmlSchemaTypeNonNegativeIntegerDef);
    xmlSchemaTypeUnsignedIntDef =
        xmlSchemaInitBasicType("unsignedInt", XML_SCHEMAS_UINT,
	xmlSchemaTypeUnsignedLongDef);
    xmlSchemaTypeUnsignedShortDef =
        xmlSchemaInitBasicType("unsignedShort", XML_SCHEMAS_USHORT,
	xmlSchemaTypeUnsignedIntDef);
    xmlSchemaTypeUnsignedByteDef =
        xmlSchemaInitBasicType("unsignedByte", XML_SCHEMAS_UBYTE,
	xmlSchemaTypeUnsignedShortDef);
    xmlSchemaTypePositiveIntegerDef =
        xmlSchemaInitBasicType("positiveInteger", XML_SCHEMAS_PINTEGER,
	xmlSchemaTypeNonNegativeIntegerDef);
    xmlSchemaTypeNormStringDef = xmlSchemaInitBasicType("normalizedString",
                                                        XML_SCHEMAS_NORMSTRING,
							xmlSchemaTypeStringDef);
    xmlSchemaTypeTokenDef = xmlSchemaInitBasicType("token",
                                                   XML_SCHEMAS_TOKEN,
						   xmlSchemaTypeNormStringDef);
    xmlSchemaTypeLanguageDef = xmlSchemaInitBasicType("language",
                                                      XML_SCHEMAS_LANGUAGE,
						      xmlSchemaTypeTokenDef);
    xmlSchemaTypeNameDef = xmlSchemaInitBasicType("Name",
                                                  XML_SCHEMAS_NAME,
						  xmlSchemaTypeTokenDef);
    xmlSchemaTypeNmtokenDef = xmlSchemaInitBasicType("NMTOKEN",
                                                     XML_SCHEMAS_NMTOKEN,
						     xmlSchemaTypeTokenDef);
    xmlSchemaTypeNCNameDef = xmlSchemaInitBasicType("NCName",
                                                    XML_SCHEMAS_NCNAME,
						    xmlSchemaTypeNameDef);
    xmlSchemaTypeIdDef = xmlSchemaInitBasicType("ID", XML_SCHEMAS_ID,
						    xmlSchemaTypeNCNameDef);
    xmlSchemaTypeIdrefDef = xmlSchemaInitBasicType("IDREF",
                                                   XML_SCHEMAS_IDREF,
						   xmlSchemaTypeNCNameDef);        
    xmlSchemaTypeEntityDef = xmlSchemaInitBasicType("ENTITY",
                                                    XML_SCHEMAS_ENTITY,
						    xmlSchemaTypeNCNameDef);
    
    xmlSchemaTypeEntitiesDef = xmlSchemaInitBasicType("ENTITIES",
                                                      XML_SCHEMAS_ENTITIES,
						      xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeEntitiesDef->subtypes = xmlSchemaTypeEntityDef;
    
    xmlSchemaTypeIdrefsDef = xmlSchemaInitBasicType("IDREFS",
                                                    XML_SCHEMAS_IDREFS,
						    xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeIdrefsDef->subtypes = xmlSchemaTypeIdrefDef;

    
    xmlSchemaTypeNmtokensDef = xmlSchemaInitBasicType("NMTOKENS",
                                                      XML_SCHEMAS_NMTOKENS,
						      xmlSchemaTypeAnySimpleTypeDef);
    xmlSchemaTypeNmtokensDef->subtypes = xmlSchemaTypeNmtokenDef;

    xmlSchemaTypesInitialized = 1;
}

void	
xmlSchemaCleanupTypes(void) {
    if (xmlSchemaTypesInitialized == 0)
	return;
    {
	xmlSchemaParticlePtr particle;
	
	xmlSchemaFreeWildcard(xmlSchemaTypeAnyTypeDef->attributeWildcard);
	
	particle = (xmlSchemaParticlePtr) xmlSchemaTypeAnyTypeDef->subtypes;
	
	xmlSchemaFreeWildcard((xmlSchemaWildcardPtr) 
	    particle->children->children->children);
	xmlFree((xmlSchemaParticlePtr) particle->children->children);
	
	xmlFree((xmlSchemaModelGroupPtr) particle->children);
	xmlFree((xmlSchemaParticlePtr) particle);
	xmlSchemaTypeAnyTypeDef->subtypes = NULL;	
    }
    xmlHashFree(xmlSchemaTypesBank, (xmlHashDeallocator) xmlSchemaFreeType);
    xmlSchemaTypesInitialized = 0;
}

int
xmlSchemaIsBuiltInTypeFacet(xmlSchemaTypePtr type, int facetType)
{
    if (type == NULL)
	return (-1);
    if (type->type != XML_SCHEMA_TYPE_BASIC)
	return (-1);
    switch (type->builtInType) {
	case XML_SCHEMAS_BOOLEAN:
	    if ((facetType == XML_SCHEMA_FACET_PATTERN) ||
		(facetType == XML_SCHEMA_FACET_WHITESPACE))
		return (1);
	    else
		return (0);	
	case XML_SCHEMAS_STRING:
	case XML_SCHEMAS_NOTATION:
	case XML_SCHEMAS_QNAME:
	case XML_SCHEMAS_ANYURI:	    
	case XML_SCHEMAS_BASE64BINARY:    
	case XML_SCHEMAS_HEXBINARY:
	    if ((facetType == XML_SCHEMA_FACET_LENGTH) ||
		(facetType == XML_SCHEMA_FACET_MINLENGTH) ||
		(facetType == XML_SCHEMA_FACET_MAXLENGTH) ||
		(facetType == XML_SCHEMA_FACET_PATTERN) ||
		(facetType == XML_SCHEMA_FACET_ENUMERATION) ||
		(facetType == XML_SCHEMA_FACET_WHITESPACE))
		return (1);
	    else
		return (0);
	case XML_SCHEMAS_DECIMAL:
	    if ((facetType == XML_SCHEMA_FACET_TOTALDIGITS) ||
		(facetType == XML_SCHEMA_FACET_FRACTIONDIGITS) ||
		(facetType == XML_SCHEMA_FACET_PATTERN) ||
		(facetType == XML_SCHEMA_FACET_WHITESPACE) ||
		(facetType == XML_SCHEMA_FACET_ENUMERATION) ||
		(facetType == XML_SCHEMA_FACET_MAXINCLUSIVE) ||
		(facetType == XML_SCHEMA_FACET_MAXEXCLUSIVE) ||
		(facetType == XML_SCHEMA_FACET_MININCLUSIVE) ||
		(facetType == XML_SCHEMA_FACET_MINEXCLUSIVE))
		return (1);
	    else
		return (0); 
	case XML_SCHEMAS_TIME:
	case XML_SCHEMAS_GDAY: 
	case XML_SCHEMAS_GMONTH:
	case XML_SCHEMAS_GMONTHDAY: 
	case XML_SCHEMAS_GYEAR: 
	case XML_SCHEMAS_GYEARMONTH:
	case XML_SCHEMAS_DATE:
	case XML_SCHEMAS_DATETIME:
	case XML_SCHEMAS_DURATION:
	case XML_SCHEMAS_FLOAT:
	case XML_SCHEMAS_DOUBLE:
	    if ((facetType == XML_SCHEMA_FACET_PATTERN) ||
		(facetType == XML_SCHEMA_FACET_ENUMERATION) ||
		(facetType == XML_SCHEMA_FACET_WHITESPACE) ||
		(facetType == XML_SCHEMA_FACET_MAXINCLUSIVE) ||
		(facetType == XML_SCHEMA_FACET_MAXEXCLUSIVE) ||
		(facetType == XML_SCHEMA_FACET_MININCLUSIVE) ||
		(facetType == XML_SCHEMA_FACET_MINEXCLUSIVE))
		return (1);
	    else
		return (0);	    				 
	default:
	    break;
    }
    return (0);
}

xmlSchemaTypePtr
xmlSchemaGetBuiltInType(xmlSchemaValType type)
{
    if (xmlSchemaTypesInitialized == 0)
	xmlSchemaInitTypes();
    switch (type) {
	
	case XML_SCHEMAS_ANYSIMPLETYPE:
	    return (xmlSchemaTypeAnySimpleTypeDef);
	case XML_SCHEMAS_STRING:
	    return (xmlSchemaTypeStringDef);
	case XML_SCHEMAS_NORMSTRING:
	    return (xmlSchemaTypeNormStringDef);
	case XML_SCHEMAS_DECIMAL:
	    return (xmlSchemaTypeDecimalDef);
	case XML_SCHEMAS_TIME:
	    return (xmlSchemaTypeTimeDef);
	case XML_SCHEMAS_GDAY:
	    return (xmlSchemaTypeGDayDef);
	case XML_SCHEMAS_GMONTH:
	    return (xmlSchemaTypeGMonthDef);
	case XML_SCHEMAS_GMONTHDAY:
    	    return (xmlSchemaTypeGMonthDayDef);
	case XML_SCHEMAS_GYEAR:
	    return (xmlSchemaTypeGYearDef);
	case XML_SCHEMAS_GYEARMONTH:
	    return (xmlSchemaTypeGYearMonthDef);
	case XML_SCHEMAS_DATE:
	    return (xmlSchemaTypeDateDef);
	case XML_SCHEMAS_DATETIME:
	    return (xmlSchemaTypeDatetimeDef);
	case XML_SCHEMAS_DURATION:
	    return (xmlSchemaTypeDurationDef);
	case XML_SCHEMAS_FLOAT:
	    return (xmlSchemaTypeFloatDef);
	case XML_SCHEMAS_DOUBLE:
	    return (xmlSchemaTypeDoubleDef);
	case XML_SCHEMAS_BOOLEAN:
	    return (xmlSchemaTypeBooleanDef);
	case XML_SCHEMAS_TOKEN:
	    return (xmlSchemaTypeTokenDef);
	case XML_SCHEMAS_LANGUAGE:
	    return (xmlSchemaTypeLanguageDef);
	case XML_SCHEMAS_NMTOKEN:
	    return (xmlSchemaTypeNmtokenDef);
	case XML_SCHEMAS_NMTOKENS:
	    return (xmlSchemaTypeNmtokensDef);
	case XML_SCHEMAS_NAME:
	    return (xmlSchemaTypeNameDef);
	case XML_SCHEMAS_QNAME:
	    return (xmlSchemaTypeQNameDef);
	case XML_SCHEMAS_NCNAME:
	    return (xmlSchemaTypeNCNameDef);
	case XML_SCHEMAS_ID:
	    return (xmlSchemaTypeIdDef);
	case XML_SCHEMAS_IDREF:
	    return (xmlSchemaTypeIdrefDef);
	case XML_SCHEMAS_IDREFS:
	    return (xmlSchemaTypeIdrefsDef);
	case XML_SCHEMAS_ENTITY:
	    return (xmlSchemaTypeEntityDef);
	case XML_SCHEMAS_ENTITIES:
	    return (xmlSchemaTypeEntitiesDef);
	case XML_SCHEMAS_NOTATION:
	    return (xmlSchemaTypeNotationDef);
	case XML_SCHEMAS_ANYURI:
	    return (xmlSchemaTypeAnyURIDef);
	case XML_SCHEMAS_INTEGER:
	    return (xmlSchemaTypeIntegerDef);
	case XML_SCHEMAS_NPINTEGER:
	    return (xmlSchemaTypeNonPositiveIntegerDef);
	case XML_SCHEMAS_NINTEGER:
	    return (xmlSchemaTypeNegativeIntegerDef);
	case XML_SCHEMAS_NNINTEGER:
	    return (xmlSchemaTypeNonNegativeIntegerDef);
	case XML_SCHEMAS_PINTEGER:
	    return (xmlSchemaTypePositiveIntegerDef);
	case XML_SCHEMAS_INT:
	    return (xmlSchemaTypeIntDef);
	case XML_SCHEMAS_UINT:
	    return (xmlSchemaTypeUnsignedIntDef);
	case XML_SCHEMAS_LONG:
	    return (xmlSchemaTypeLongDef);
	case XML_SCHEMAS_ULONG:
	    return (xmlSchemaTypeUnsignedLongDef);
	case XML_SCHEMAS_SHORT:
	    return (xmlSchemaTypeShortDef);
	case XML_SCHEMAS_USHORT:
	    return (xmlSchemaTypeUnsignedShortDef);
	case XML_SCHEMAS_BYTE:
	    return (xmlSchemaTypeByteDef);
	case XML_SCHEMAS_UBYTE:
	    return (xmlSchemaTypeUnsignedByteDef);
	case XML_SCHEMAS_HEXBINARY:
	    return (xmlSchemaTypeHexBinaryDef);
	case XML_SCHEMAS_BASE64BINARY:
	    return (xmlSchemaTypeBase64BinaryDef);
	case XML_SCHEMAS_ANYTYPE:
	    return (xmlSchemaTypeAnyTypeDef);	    
	default:
	    return (NULL);
    }
}

int
xmlSchemaValueAppend(xmlSchemaValPtr prev, xmlSchemaValPtr cur) {

    if ((prev == NULL) || (cur == NULL))
	return (-1);
    prev->next = cur;
    return (0);
}

xmlSchemaValPtr
xmlSchemaValueGetNext(xmlSchemaValPtr cur) {

    if (cur == NULL)
	return (NULL);
    return (cur->next);
}

const xmlChar *
xmlSchemaValueGetAsString(xmlSchemaValPtr val)
{    
    if (val == NULL)
	return (NULL);
    switch (val->type) {
	case XML_SCHEMAS_STRING:
	case XML_SCHEMAS_NORMSTRING:
	case XML_SCHEMAS_ANYSIMPLETYPE:
	case XML_SCHEMAS_TOKEN:
        case XML_SCHEMAS_LANGUAGE:
        case XML_SCHEMAS_NMTOKEN:
        case XML_SCHEMAS_NAME:
        case XML_SCHEMAS_NCNAME:
        case XML_SCHEMAS_ID:
        case XML_SCHEMAS_IDREF:
        case XML_SCHEMAS_ENTITY:
        case XML_SCHEMAS_ANYURI:
	    return (BAD_CAST val->value.str);
	default:
	    break;
    }
    return (NULL);
}

int
xmlSchemaValueGetAsBoolean(xmlSchemaValPtr val)
{    
    if ((val == NULL) || (val->type != XML_SCHEMAS_BOOLEAN))
	return (0);
    return (val->value.b);
}

xmlSchemaValPtr
xmlSchemaNewStringValue(xmlSchemaValType type,
			const xmlChar *value)
{
    xmlSchemaValPtr val;

    if (type != XML_SCHEMAS_STRING)
	return(NULL);
    val = (xmlSchemaValPtr) xmlMalloc(sizeof(xmlSchemaVal));
    if (val == NULL) {
	return(NULL);
    }
    memset(val, 0, sizeof(xmlSchemaVal));
    val->type = type;
    val->value.str = (xmlChar *) value;
    return(val);
}

xmlSchemaValPtr
xmlSchemaNewNOTATIONValue(const xmlChar *name,
			  const xmlChar *ns)
{
    xmlSchemaValPtr val;

    val = xmlSchemaNewValue(XML_SCHEMAS_NOTATION);
    if (val == NULL)
	return (NULL);

    val->value.qname.name = (xmlChar *)name;
    if (ns != NULL)
	val->value.qname.uri = (xmlChar *)ns;
    return(val);
}

xmlSchemaValPtr
xmlSchemaNewQNameValue(const xmlChar *namespaceName,
		       const xmlChar *localName)
{
    xmlSchemaValPtr val;

    val = xmlSchemaNewValue(XML_SCHEMAS_QNAME);
    if (val == NULL)
	return (NULL);

    val->value.qname.name = (xmlChar *) localName;
    val->value.qname.uri = (xmlChar *) namespaceName;
    return(val);
}

void	
xmlSchemaFreeValue(xmlSchemaValPtr value) {
    xmlSchemaValPtr prev;

    while (value != NULL) {	
	switch (value->type) {
	    case XML_SCHEMAS_STRING:
	    case XML_SCHEMAS_NORMSTRING:
	    case XML_SCHEMAS_TOKEN:
	    case XML_SCHEMAS_LANGUAGE:
	    case XML_SCHEMAS_NMTOKEN:
	    case XML_SCHEMAS_NMTOKENS:
	    case XML_SCHEMAS_NAME:
	    case XML_SCHEMAS_NCNAME:
	    case XML_SCHEMAS_ID:
	    case XML_SCHEMAS_IDREF:
	    case XML_SCHEMAS_IDREFS:
	    case XML_SCHEMAS_ENTITY:
	    case XML_SCHEMAS_ENTITIES:        
	    case XML_SCHEMAS_ANYURI:
	    case XML_SCHEMAS_ANYSIMPLETYPE:
		if (value->value.str != NULL)
		    xmlFree(value->value.str);
		break;
	    case XML_SCHEMAS_NOTATION:
	    case XML_SCHEMAS_QNAME:
		if (value->value.qname.uri != NULL)
		    xmlFree(value->value.qname.uri);
		if (value->value.qname.name != NULL)
		    xmlFree(value->value.qname.name);
		break;
	    case XML_SCHEMAS_HEXBINARY:
		if (value->value.hex.str != NULL)
		    xmlFree(value->value.hex.str);
		break;
	    case XML_SCHEMAS_BASE64BINARY:
		if (value->value.base64.str != NULL)
		    xmlFree(value->value.base64.str);
		break;
	    default:
		break;
	}
	prev = value;
	value = value->next;
	xmlFree(prev);
    }    
}

xmlSchemaTypePtr
xmlSchemaGetPredefinedType(const xmlChar *name, const xmlChar *ns) {
    if (xmlSchemaTypesInitialized == 0)
	xmlSchemaInitTypes();
    if (name == NULL)
	return(NULL);
    return((xmlSchemaTypePtr) xmlHashLookup2(xmlSchemaTypesBank, name, ns));
}

xmlSchemaTypePtr
xmlSchemaGetBuiltInListSimpleTypeItemType(xmlSchemaTypePtr type)
{
    if ((type == NULL) || (type->type != XML_SCHEMA_TYPE_BASIC))
	return (NULL);
    switch (type->builtInType) {
	case XML_SCHEMAS_NMTOKENS: 
	    return (xmlSchemaTypeNmtokenDef );
	case XML_SCHEMAS_IDREFS: 
	    return (xmlSchemaTypeIdrefDef);
	case XML_SCHEMAS_ENTITIES:
	    return (xmlSchemaTypeEntityDef);
	default:
	    return (NULL);
    }
}


#define IS_TZO_CHAR(c)						\
	((c == 0) || (c == 'Z') || (c == '+') || (c == '-'))

#define VALID_YEAR(yr)          (yr != 0)
#define VALID_MONTH(mon)        ((mon >= 1) && (mon <= 12))
#define VALID_DAY(day)          ((day >= 1) && (day <= 31))
#define VALID_HOUR(hr)          ((hr >= 0) && (hr <= 23))
#define VALID_MIN(min)          ((min >= 0) && (min <= 59))
#define VALID_SEC(sec)          ((sec >= 0) && (sec < 60))
#define VALID_TZO(tzo)          ((tzo > -840) && (tzo < 840))
#define IS_LEAP(y)						\
	(((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))

static const unsigned int daysInMonth[12] =
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const unsigned int daysInMonthLeap[12] =
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define MAX_DAYINMONTH(yr,mon)                                  \
        (IS_LEAP(yr) ? daysInMonthLeap[mon - 1] : daysInMonth[mon - 1])

#define VALID_MDAY(dt)						\
	(IS_LEAP(dt->year) ?				        \
	    (dt->day <= daysInMonthLeap[dt->mon - 1]) :	        \
	    (dt->day <= daysInMonth[dt->mon - 1]))

#define VALID_DATE(dt)						\
	(VALID_YEAR(dt->year) && VALID_MONTH(dt->mon) && VALID_MDAY(dt))

#define VALID_TIME(dt)						\
	(VALID_HOUR(dt->hour) && VALID_MIN(dt->min) &&		\
	 VALID_SEC(dt->sec) && VALID_TZO(dt->tzo))

#define VALID_DATETIME(dt)					\
	(VALID_DATE(dt) && VALID_TIME(dt))

#define SECS_PER_MIN            (60)
#define SECS_PER_HOUR           (60 * SECS_PER_MIN)
#define SECS_PER_DAY            (24 * SECS_PER_HOUR)

static const long dayInYearByMonth[12] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
static const long dayInLeapYearByMonth[12] =
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

#define DAY_IN_YEAR(day, month, year)				\
        ((IS_LEAP(year) ?					\
                dayInLeapYearByMonth[month - 1] :		\
                dayInYearByMonth[month - 1]) + day)

#ifdef DEBUG
#define DEBUG_DATE(dt)                                                  \
    xmlGenericError(xmlGenericErrorContext,                             \
        "type=%o %04ld-%02u-%02uT%02u:%02u:%03f",                       \
        dt->type,dt->value.date.year,dt->value.date.mon,                \
        dt->value.date.day,dt->value.date.hour,dt->value.date.min,      \
        dt->value.date.sec);                                            \
    if (dt->value.date.tz_flag)                                         \
        if (dt->value.date.tzo != 0)                                    \
            xmlGenericError(xmlGenericErrorContext,                     \
                "%+05d\n",dt->value.date.tzo);                          \
        else                                                            \
            xmlGenericError(xmlGenericErrorContext, "Z\n");             \
    else                                                                \
        xmlGenericError(xmlGenericErrorContext,"\n")
#else
#define DEBUG_DATE(dt)
#endif

static int
_xmlSchemaParseGYear (xmlSchemaValDatePtr dt, const xmlChar **str) {
    const xmlChar *cur = *str, *firstChar;
    int isneg = 0, digcnt = 0;

    if (((*cur < '0') || (*cur > '9')) &&
	(*cur != '-') && (*cur != '+'))
	return -1;

    if (*cur == '-') {
	isneg = 1;
	cur++;
    }

    firstChar = cur;

    while ((*cur >= '0') && (*cur <= '9')) {
	dt->year = dt->year * 10 + (*cur - '0');
	cur++;
	digcnt++;
    }

    if ((digcnt < 4) || ((digcnt > 4) && (*firstChar == '0')))
	return 1;

    if (isneg)
	dt->year = - dt->year;

    if (!VALID_YEAR(dt->year))
	return 2;

    *str = cur;
    return 0;
}

#define PARSE_2_DIGITS(num, cur, invalid)			\
	if ((cur[0] < '0') || (cur[0] > '9') ||			\
	    (cur[1] < '0') || (cur[1] > '9'))			\
	    invalid = 1;					\
	else							\
	    num = (cur[0] - '0') * 10 + (cur[1] - '0');		\
	cur += 2;

#define PARSE_FLOAT(num, cur, invalid)				\
	PARSE_2_DIGITS(num, cur, invalid);			\
	if (!invalid && (*cur == '.')) {			\
	    double mult = 1;				        \
	    cur++;						\
	    if ((*cur < '0') || (*cur > '9'))			\
		invalid = 1;					\
	    while ((*cur >= '0') && (*cur <= '9')) {		\
		mult /= 10;					\
		num += (*cur - '0') * mult;			\
		cur++;						\
	    }							\
	}

static int
_xmlSchemaParseGMonth (xmlSchemaValDatePtr dt, const xmlChar **str) {
    const xmlChar *cur = *str;
    int ret = 0;
    unsigned int value = 0;

    PARSE_2_DIGITS(value, cur, ret);
    if (ret != 0)
	return ret;

    if (!VALID_MONTH(value))
	return 2;

    dt->mon = value;

    *str = cur;
    return 0;
}

static int
_xmlSchemaParseGDay (xmlSchemaValDatePtr dt, const xmlChar **str) {
    const xmlChar *cur = *str;
    int ret = 0;
    unsigned int value = 0;

    PARSE_2_DIGITS(value, cur, ret);
    if (ret != 0)
	return ret;

    if (!VALID_DAY(value))
	return 2;

    dt->day = value;
    *str = cur;
    return 0;
}

static int
_xmlSchemaParseTime (xmlSchemaValDatePtr dt, const xmlChar **str) {
    const xmlChar *cur = *str;    
    int ret = 0;
    int value = 0;

    PARSE_2_DIGITS(value, cur, ret);
    if (ret != 0)
	return ret;    
    if (*cur != ':')
	return 1;
    if (!VALID_HOUR(value))
	return 2;
    cur++;

    
    dt->hour = value;

    PARSE_2_DIGITS(value, cur, ret);
    if (ret != 0)
	return ret;
    if (!VALID_MIN(value))
	return 2;
    dt->min = value;

    if (*cur != ':')
	return 1;
    cur++;

    PARSE_FLOAT(dt->sec, cur, ret);
    if (ret != 0)
	return ret;

    if ((!VALID_SEC(dt->sec)) || (!VALID_TZO(dt->tzo)))
	return 2;

    *str = cur;
    return 0;
}

static int
_xmlSchemaParseTimeZone (xmlSchemaValDatePtr dt, const xmlChar **str) {
    const xmlChar *cur;
    int ret = 0;

    if (str == NULL)
	return -1;
    cur = *str;

    switch (*cur) {
    case 0:
	dt->tz_flag = 0;
	dt->tzo = 0;
	break;

    case 'Z':
	dt->tz_flag = 1;
	dt->tzo = 0;
	cur++;
	break;

    case '+':
    case '-': {
	int isneg = 0, tmp = 0;
	isneg = (*cur == '-');

	cur++;

	PARSE_2_DIGITS(tmp, cur, ret);
	if (ret != 0)
	    return ret;
	if (!VALID_HOUR(tmp))
	    return 2;

	if (*cur != ':')
	    return 1;
	cur++;

	dt->tzo = tmp * 60;

	PARSE_2_DIGITS(tmp, cur, ret);
	if (ret != 0)
	    return ret;
	if (!VALID_MIN(tmp))
	    return 2;

	dt->tzo += tmp;
	if (isneg)
	    dt->tzo = - dt->tzo;

	if (!VALID_TZO(dt->tzo))
	    return 2;

	dt->tz_flag = 1;
	break;
      }
    default:
	return 1;
    }

    *str = cur;
    return 0;
}

static int
_xmlSchemaBase64Decode (const xmlChar ch) {
    if (('A' <= ch) && (ch <= 'Z')) return ch - 'A';
    if (('a' <= ch) && (ch <= 'z')) return ch - 'a' + 26;
    if (('0' <= ch) && (ch <= '9')) return ch - '0' + 52;
    if ('+' == ch) return 62;
    if ('/' == ch) return 63;
    if ('=' == ch) return 64;
    return -1;
}


#define PARSE_DIGITS(num, cur, num_type)	                \
	if ((*cur < '0') || (*cur > '9'))			\
	    num_type = -1;					\
        else                                                    \
	    while ((*cur >= '0') && (*cur <= '9')) {		\
	        num = num * 10 + (*cur - '0');		        \
	        cur++;                                          \
            }

#define PARSE_NUM(num, cur, num_type)				\
        num = 0;                                                \
	PARSE_DIGITS(num, cur, num_type);	                \
	if (!num_type && (*cur == '.')) {			\
	    double mult = 1;				        \
	    cur++;						\
	    if ((*cur < '0') || (*cur > '9'))			\
		num_type = -1;					\
            else                                                \
                num_type = 1;                                   \
	    while ((*cur >= '0') && (*cur <= '9')) {		\
		mult /= 10;					\
		num += (*cur - '0') * mult;			\
		cur++;						\
	    }							\
	}

static int
xmlSchemaValidateDates (xmlSchemaValType type,
	                const xmlChar *dateTime, xmlSchemaValPtr *val,
			int collapse) {
    xmlSchemaValPtr dt;
    int ret;
    const xmlChar *cur = dateTime;

#define RETURN_TYPE_IF_VALID(t)					\
    if (IS_TZO_CHAR(*cur)) {					\
	ret = _xmlSchemaParseTimeZone(&(dt->value.date), &cur);	\
	if (ret == 0) {						\
	    if (*cur != 0)					\
		goto error;					\
	    dt->type = t;					\
	    goto done;						\
	}							\
    }

    if (dateTime == NULL)
	return -1;

    if (collapse)
	while IS_WSP_BLANK_CH(*cur) cur++;

    if ((*cur != '-') && (*cur < '0') && (*cur > '9'))
	return 1;

    dt = xmlSchemaNewValue(XML_SCHEMAS_UNKNOWN);
    if (dt == NULL)
	return -1;

    if ((cur[0] == '-') && (cur[1] == '-')) {
	cur += 2;

	
	if (*cur == '-') {
	    if (type == XML_SCHEMAS_GMONTH)
		goto error;
	  ++cur;
	    ret = _xmlSchemaParseGDay(&(dt->value.date), &cur);
	    if (ret != 0)
		goto error;

	    RETURN_TYPE_IF_VALID(XML_SCHEMAS_GDAY);

	    goto error;
	}

	ret = _xmlSchemaParseGMonth(&(dt->value.date), &cur);
	if (ret != 0)
	    goto error;

        if (*cur == '-') {
            const xmlChar *rewnd = cur;
            cur++;

  	    ret = _xmlSchemaParseGDay(&(dt->value.date), &cur);
            if ((ret == 0) && ((*cur == 0) || (*cur != ':'))) {

                if (VALID_MDAY((&(dt->value.date)))) {

	            RETURN_TYPE_IF_VALID(XML_SCHEMAS_GMONTHDAY);

                    goto error;
                }
            }

            cur = rewnd;
        }

	RETURN_TYPE_IF_VALID(XML_SCHEMAS_GMONTH);

	goto error;
    }

    if ((*cur >= '0') && (*cur <= '9')) {
	ret = _xmlSchemaParseTime(&(dt->value.date), &cur);
	if (ret == 0) {
	    
	    RETURN_TYPE_IF_VALID(XML_SCHEMAS_TIME);
	}
    }

    
    cur = dateTime;

    ret = _xmlSchemaParseGYear(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    
    RETURN_TYPE_IF_VALID(XML_SCHEMAS_GYEAR);

    if (*cur != '-')
	goto error;
    cur++;

    ret = _xmlSchemaParseGMonth(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    
    RETURN_TYPE_IF_VALID(XML_SCHEMAS_GYEARMONTH);

    if (*cur != '-')
	goto error;
    cur++;

    ret = _xmlSchemaParseGDay(&(dt->value.date), &cur);
    if ((ret != 0) || !VALID_DATE((&(dt->value.date))))
	goto error;

    
    RETURN_TYPE_IF_VALID(XML_SCHEMAS_DATE);

    if (*cur != 'T')
	goto error;
    cur++;

    
    ret = _xmlSchemaParseTime(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    ret = _xmlSchemaParseTimeZone(&(dt->value.date), &cur);
    if (collapse)
	while IS_WSP_BLANK_CH(*cur) cur++;
    if ((ret != 0) || (*cur != 0) || (!(VALID_DATETIME((&(dt->value.date))))))
	goto error;


    dt->type = XML_SCHEMAS_DATETIME;

done:
#if 1
    if ((type != XML_SCHEMAS_UNKNOWN) && (type != dt->type))
        goto error;
#else
    if ((type != XML_SCHEMAS_UNKNOWN) && (type != dt->type)) {

        
        if ((type == XML_SCHEMAS_TIME) && (dt->type == XML_SCHEMAS_TIME))
            goto error;

        if ((type == XML_SCHEMAS_DATETIME) &&
            ((dt->type != XML_SCHEMAS_DATE) ||
             (dt->type != XML_SCHEMAS_GYEARMONTH) ||
             (dt->type != XML_SCHEMAS_GYEAR)))
            goto error;

        if ((type == XML_SCHEMAS_DATE) &&
            ((dt->type != XML_SCHEMAS_GYEAR) ||
             (dt->type != XML_SCHEMAS_GYEARMONTH)))
            goto error;

        if ((type == XML_SCHEMAS_GYEARMONTH) && (dt->type != XML_SCHEMAS_GYEAR))
            goto error;

        if ((type == XML_SCHEMAS_GMONTHDAY) && (dt->type != XML_SCHEMAS_GMONTH))
            goto error;
    }
#endif

    if (val != NULL)
        *val = dt;
    else
	xmlSchemaFreeValue(dt);

    return 0;

error:
    if (dt != NULL)
	xmlSchemaFreeValue(dt);
    return 1;
}

static int
xmlSchemaValidateDuration (xmlSchemaTypePtr type ATTRIBUTE_UNUSED,
	                   const xmlChar *duration, xmlSchemaValPtr *val,
			   int collapse) {
    const xmlChar  *cur = duration;
    xmlSchemaValPtr dur;
    int isneg = 0;
    unsigned int seq = 0;
    double         num;
    int            num_type = 0;  
    const xmlChar  desig[]  = {'Y', 'M', 'D', 'H', 'M', 'S'};
    const double   multi[]  = { 0.0, 0.0, 86400.0, 3600.0, 60.0, 1.0, 0.0};

    if (duration == NULL)
	return -1;

    if (collapse)
	while IS_WSP_BLANK_CH(*cur) cur++;

    if (*cur == '-') {
        isneg = 1;
        cur++;
    }

    
    if (*cur++ != 'P')
	return 1;

    if (*cur == 0)
	return 1;

    dur = xmlSchemaNewValue(XML_SCHEMAS_DURATION);
    if (dur == NULL)
	return -1;

    while (*cur != 0) {

        
        if (seq >= sizeof(desig))
            goto error;

        
        if (*cur == 'T') {
            if (seq <= 3) {
                seq = 3;
                cur++;
            } else
                return 1;
        } else if (seq == 3)
            goto error;

        
        PARSE_NUM(num, cur, num_type);

        if ((num_type == -1) || (*cur == 0))
            goto error;

        
        while (seq < sizeof(desig)) {
            if (*cur == desig[seq]) {

                
                if ((num_type != 0) && (seq < (sizeof(desig)-1)))
                    goto error;

                switch (seq) {
                    case 0:
                        dur->value.dur.mon = (long)num * 12;
                        break;
                    case 1:
                        dur->value.dur.mon += (long)num;
                        break;
                    default:
                        
                        dur->value.dur.sec += num * multi[seq];
                        seq++;
                        break;
                }

                break;          
            }
            
            if ((++seq == 3) || (seq == 6))
                goto error;
        }
	cur++;
	if (collapse)
	    while IS_WSP_BLANK_CH(*cur) cur++;        
    }

    if (isneg) {
        dur->value.dur.mon = -dur->value.dur.mon;
        dur->value.dur.day = -dur->value.dur.day;
        dur->value.dur.sec = -dur->value.dur.sec;
    }

    if (val != NULL)
        *val = dur;
    else
	xmlSchemaFreeValue(dur);

    return 0;

error:
    if (dur != NULL)
	xmlSchemaFreeValue(dur);
    return 1;
}

static xmlChar *
xmlSchemaStrip(const xmlChar *value) {
    const xmlChar *start = value, *end, *f;

    if (value == NULL) return(NULL);
    while ((*start != 0) && (IS_BLANK_CH(*start))) start++;
    end = start;
    while (*end != 0) end++;
    f = end;
    end--;
    while ((end > start) && (IS_BLANK_CH(*end))) end--;
    end++;
    if ((start == value) && (f == end)) return(NULL);
    return(xmlStrndup(start, end - start));
}

xmlChar *
xmlSchemaWhiteSpaceReplace(const xmlChar *value) {
    const xmlChar *cur = value;    
    xmlChar *ret = NULL, *mcur; 

    if (value == NULL) 
	return(NULL);
    
    while ((*cur != 0) && 
	(((*cur) != 0xd) && ((*cur) != 0x9) && ((*cur) != 0xa))) {
	cur++;
    }
    if (*cur == 0)
	return (NULL);
    ret = xmlStrdup(value);
    
    mcur = (xmlChar *)  (ret + (cur - value));
    do {
	if ( ((*mcur) == 0xd) || ((*mcur) == 0x9) || ((*mcur) == 0xa) )
	    *mcur = ' ';
	mcur++;
    } while (*mcur != 0);	    
    return(ret);
}

xmlChar *
xmlSchemaCollapseString(const xmlChar *value) {
    const xmlChar *start = value, *end, *f;
    xmlChar *g;
    int col = 0;

    if (value == NULL) return(NULL);
    while ((*start != 0) && (IS_BLANK_CH(*start))) start++;
    end = start;
    while (*end != 0) {
	if ((*end == ' ') && (IS_BLANK_CH(end[1]))) {
	    col = end - start;
	    break;
	} else if ((*end == 0xa) || (*end == 0x9) || (*end == 0xd)) {
	    col = end - start;
	    break;
	}
	end++;
    }
    if (col == 0) {
	f = end;
	end--;
	while ((end > start) && (IS_BLANK_CH(*end))) end--;
	end++;
	if ((start == value) && (f == end)) return(NULL);
	return(xmlStrndup(start, end - start));
    }
    start = xmlStrdup(start);
    if (start == NULL) return(NULL);
    g = (xmlChar *) (start + col);
    end = g;
    while (*end != 0) {
	if (IS_BLANK_CH(*end)) {
	    end++;
	    while (IS_BLANK_CH(*end)) end++;
	    if (*end != 0)
		*g++ = ' ';
	} else
	    *g++ = *end++;
    }
    *g = 0;
    return((xmlChar *) start);
}

static int
xmlSchemaValAtomicListNode(xmlSchemaTypePtr type, const xmlChar *value,
	                   xmlSchemaValPtr *ret, xmlNodePtr node) {
    xmlChar *val, *cur, *endval;
    int nb_values = 0;
    int tmp = 0;

    if (value == NULL) {
	return(-1);
    }
    val = xmlStrdup(value);
    if (val == NULL) {
	return(-1);
    }
    if (ret != NULL) {
        *ret = NULL;
    }
    cur = val;
    while (IS_BLANK_CH(*cur)) *cur++ = 0;
    while (*cur != 0) {
	if (IS_BLANK_CH(*cur)) {
	    *cur = 0;
	    cur++;
	    while (IS_BLANK_CH(*cur)) *cur++ = 0;
	} else {
	    nb_values++;
	    cur++;
	    while ((*cur != 0) && (!IS_BLANK_CH(*cur))) cur++;
	}
    }
    if (nb_values == 0) {
	xmlFree(val);
	return(nb_values);
    }
    endval = cur;
    cur = val;
    while ((*cur == 0) && (cur != endval)) cur++;
    while (cur != endval) {
	tmp = xmlSchemaValPredefTypeNode(type, cur, NULL, node);
	if (tmp != 0)
	    break;
	while (*cur != 0) cur++;
	while ((*cur == 0) && (cur != endval)) cur++;
    }
    xmlFree(val);
    if (tmp == 0)
	return(nb_values);
    return(-1);
}

static int
xmlSchemaParseUInt(const xmlChar **str, unsigned long *llo,
                   unsigned long *lmi, unsigned long *lhi) {
    unsigned long lo = 0, mi = 0, hi = 0;
    const xmlChar *tmp, *cur = *str;
    int ret = 0, i = 0;

    if (!((*cur >= '0') && (*cur <= '9'))) 
        return(-2);

    while (*cur == '0') {        
        cur++;
    }
    tmp = cur;
    while ((*tmp != 0) && (*tmp >= '0') && (*tmp <= '9')) {
        i++;tmp++;ret++;
    }
    if (i > 24) {
        *str = tmp;
        return(-1);
    }
    while (i > 16) {
        hi = hi * 10 + (*cur++ - '0');
        i--;
    }
    while (i > 8) {
        mi = mi * 10 + (*cur++ - '0');
        i--;
    }
    while (i > 0) {
        lo = lo * 10 + (*cur++ - '0');
        i--;
    }

    *str = cur;
    *llo = lo;
    *lmi = mi;
    *lhi = hi;
    return(ret);
}

static int
xmlSchemaValAtomicType(xmlSchemaTypePtr type, const xmlChar * value,
                       xmlSchemaValPtr * val, xmlNodePtr node, int flags,
		       xmlSchemaWhitespaceValueType ws,
		       int normOnTheFly, int applyNorm, int createStringValue)
{
    xmlSchemaValPtr v;
    xmlChar *norm = NULL;
    int ret = 0;

    if (xmlSchemaTypesInitialized == 0)
        xmlSchemaInitTypes();
    if (type == NULL)
        return (-1);

    if (value == NULL)
        value = BAD_CAST "";

    if (val != NULL)
        *val = NULL;
    if ((flags == 0) && (value != NULL)) {

        if ((type->builtInType != XML_SCHEMAS_STRING) &&
	  (type->builtInType != XML_SCHEMAS_ANYTYPE) && 
	  (type->builtInType != XML_SCHEMAS_ANYSIMPLETYPE)) {
	    if (type->builtInType == XML_SCHEMAS_NORMSTRING)
		norm = xmlSchemaWhiteSpaceReplace(value);
            else
		norm = xmlSchemaCollapseString(value);
            if (norm != NULL)
                value = norm;
        }
    }

    switch (type->builtInType) {
        case XML_SCHEMAS_UNKNOWN:            
            goto error;
	case XML_SCHEMAS_ANYTYPE:
	case XML_SCHEMAS_ANYSIMPLETYPE:
	    if ((createStringValue) && (val != NULL)) {
		v = xmlSchemaNewValue(XML_SCHEMAS_ANYSIMPLETYPE);
		if (v != NULL) {
		    v->value.str = xmlStrdup(value);
		    *val = v;
		} else {
		    goto error;
		}		
	    }
	    goto return0;
        case XML_SCHEMAS_STRING:		
	    if (! normOnTheFly) {
		const xmlChar *cur = value;

		if (ws == XML_SCHEMA_WHITESPACE_REPLACE) {
		    while (*cur != 0) {
			if ((*cur == 0xd) || (*cur == 0xa) || (*cur == 0x9)) {
			    goto return1;
			} else {
			    cur++;
			}
		    }
		} else if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE) {
		    while (*cur != 0) {
			if ((*cur == 0xd) || (*cur == 0xa) || (*cur == 0x9)) {
			    goto return1;
			} else if IS_WSP_SPACE_CH(*cur) {
			    cur++;
			    if IS_WSP_SPACE_CH(*cur)
				goto return1;
			} else {
			    cur++;
			}
		    }
		}
	    }
	    if (createStringValue && (val != NULL)) {
		if (applyNorm) {
		    if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			norm = xmlSchemaCollapseString(value);
		    else if (ws == XML_SCHEMA_WHITESPACE_REPLACE)
			norm = xmlSchemaWhiteSpaceReplace(value);
		    if (norm != NULL)
			value = norm;
		}
		v = xmlSchemaNewValue(XML_SCHEMAS_STRING);
		if (v != NULL) {
		    v->value.str = xmlStrdup(value);
		    *val = v;
		} else {
		    goto error;
		}
	    }
            goto return0;
        case XML_SCHEMAS_NORMSTRING:{
		if (normOnTheFly) {
		    if (applyNorm) {
			if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			    norm = xmlSchemaCollapseString(value);
			else
			    norm = xmlSchemaWhiteSpaceReplace(value);
			if (norm != NULL)
			    value = norm;
		    }
		} else {
		    const xmlChar *cur = value;
		    while (*cur != 0) {
			if ((*cur == 0xd) || (*cur == 0xa) || (*cur == 0x9)) {
			    goto return1;
			} else {
			    cur++;
			}
		    }
		}
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_NORMSTRING);
                    if (v != NULL) {
                        v->value.str = xmlStrdup(value);
                        *val = v;
                    } else {
                        goto error;
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_DECIMAL:{
                const xmlChar *cur = value;
                unsigned int len, neg, integ, hasLeadingZeroes;
		xmlChar cval[25];
		xmlChar *cptr = cval;		

                if ((cur == NULL) || (*cur == 0))
                    goto return1;

		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;

		neg = 0;
                if (*cur == '-') {
		    neg = 1;
                    cur++;
		} else if (*cur == '+')
                    cur++;
		if (*cur == 0)
		    goto return1;
		len = 0;
		integ = ~0u;
		hasLeadingZeroes = 0;
		while (*cur == '0') {
		    cur++;
		    hasLeadingZeroes = 1;
		}
		if (*cur != 0) {
		    do {
			if ((*cur >= '0') && (*cur <= '9')) {
			    *cptr++ = *cur++;
			    len++;
			} else if (*cur == '.') {
			    cur++;
			    integ = len;
			    do {
				if ((*cur >= '0') && (*cur <= '9')) {
				    *cptr++ = *cur++;
				    len++;
				} else
				    break;
			    } while (len < 24);
			    if ((len == 0) && (!hasLeadingZeroes))
				goto return1;
			    break;
			} else
			    break;
		    } while (len < 24);
		}
		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;
		if (*cur != 0)
		    goto return1; 
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_DECIMAL);
                    if (v != NULL) {
			if (len != 0) {
			    
			    if (integ != ~0u) {
				while ((len != integ) && (*(cptr-1) == '0')) {
				    cptr--;
				    len--;
				}
			    }
			    if (len != 0) {
				*cptr = 0;
				cptr = cval;

				xmlSchemaParseUInt((const xmlChar **)&cptr,
				    &v->value.decimal.lo,
				    &v->value.decimal.mi,
				    &v->value.decimal.hi);
			    }
			}
                        v->value.decimal.sign = neg;
			if (len == 0) {
			    
			    v->value.decimal.total = 1;
			} else {
			    v->value.decimal.total = len;
			    if (integ == ~0u)
				v->value.decimal.frac = 0;
			    else
				v->value.decimal.frac = len - integ;
			}
                        *val = v;
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_TIME:
        case XML_SCHEMAS_GDAY:
        case XML_SCHEMAS_GMONTH:
        case XML_SCHEMAS_GMONTHDAY:
        case XML_SCHEMAS_GYEAR:
        case XML_SCHEMAS_GYEARMONTH:
        case XML_SCHEMAS_DATE:
        case XML_SCHEMAS_DATETIME:
            ret = xmlSchemaValidateDates(type->builtInType, value, val,
		normOnTheFly);
            break;
        case XML_SCHEMAS_DURATION:
            ret = xmlSchemaValidateDuration(type, value, val,
		normOnTheFly);
            break;
        case XML_SCHEMAS_FLOAT:
        case XML_SCHEMAS_DOUBLE:{
                const xmlChar *cur = value;
                int neg = 0;

		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;

                if ((cur[0] == 'N') && (cur[1] == 'a') && (cur[2] == 'N')) {
                    cur += 3;
                    if (*cur != 0)
                        goto return1;
                    if (val != NULL) {
                        if (type == xmlSchemaTypeFloatDef) {
                            v = xmlSchemaNewValue(XML_SCHEMAS_FLOAT);
                            if (v != NULL) {
                                v->value.f = (float) xmlXPathNAN;
                            } else {
                                xmlSchemaFreeValue(v);
                                goto error;
                            }
                        } else {
                            v = xmlSchemaNewValue(XML_SCHEMAS_DOUBLE);
                            if (v != NULL) {
                                v->value.d = xmlXPathNAN;
                            } else {
                                xmlSchemaFreeValue(v);
                                goto error;
                            }
                        }
                        *val = v;
                    }
                    goto return0;
                }
                if (*cur == '-') {
                    neg = 1;
                    cur++;
                }
                if ((cur[0] == 'I') && (cur[1] == 'N') && (cur[2] == 'F')) {
                    cur += 3;
                    if (*cur != 0)
                        goto return1;
                    if (val != NULL) {
                        if (type == xmlSchemaTypeFloatDef) {
                            v = xmlSchemaNewValue(XML_SCHEMAS_FLOAT);
                            if (v != NULL) {
                                if (neg)
                                    v->value.f = (float) xmlXPathNINF;
                                else
                                    v->value.f = (float) xmlXPathPINF;
                            } else {
                                xmlSchemaFreeValue(v);
                                goto error;
                            }
                        } else {
                            v = xmlSchemaNewValue(XML_SCHEMAS_DOUBLE);
                            if (v != NULL) {
                                if (neg)
                                    v->value.d = xmlXPathNINF;
                                else
                                    v->value.d = xmlXPathPINF;
                            } else {
                                xmlSchemaFreeValue(v);
                                goto error;
                            }
                        }
                        *val = v;
                    }
                    goto return0;
                }
                if ((neg == 0) && (*cur == '+'))
                    cur++;
                if ((cur[0] == 0) || (cur[0] == '+') || (cur[0] == '-'))
                    goto return1;
                while ((*cur >= '0') && (*cur <= '9')) {
                    cur++;
                }
                if (*cur == '.') {
                    cur++;
                    while ((*cur >= '0') && (*cur <= '9'))
                        cur++;
                }
                if ((*cur == 'e') || (*cur == 'E')) {
                    cur++;
                    if ((*cur == '-') || (*cur == '+'))
                        cur++;
                    while ((*cur >= '0') && (*cur <= '9'))
                        cur++;
                }
		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;

                if (*cur != 0)
                    goto return1;
                if (val != NULL) {
                    if (type == xmlSchemaTypeFloatDef) {
                        v = xmlSchemaNewValue(XML_SCHEMAS_FLOAT);
                        if (v != NULL) {
                            if (sscanf((const char *) value, "%f",
                                 &(v->value.f)) == 1) {
                                *val = v;
                            } else {
                                xmlSchemaFreeValue(v);
                                goto return1;
                            }
                        } else {
                            goto error;
                        }
                    } else {
                        v = xmlSchemaNewValue(XML_SCHEMAS_DOUBLE);
                        if (v != NULL) {
                            if (sscanf((const char *) value, "%lf",
                                 &(v->value.d)) == 1) {
                                *val = v;
                            } else {
                                xmlSchemaFreeValue(v);
                                goto return1;
                            }
                        } else {
                            goto error;
                        }
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_BOOLEAN:{
                const xmlChar *cur = value;

		if (normOnTheFly) {
		    while IS_WSP_BLANK_CH(*cur) cur++;
		    if (*cur == '0') {
			ret = 0;
			cur++;
		    } else if (*cur == '1') {
			ret = 1;
			cur++;
		    } else if (*cur == 't') {
			cur++;
			if ((*cur++ == 'r') && (*cur++ == 'u') &&
			    (*cur++ == 'e')) {
			    ret = 1;
			} else
			    goto return1;
		    } else if (*cur == 'f') {
			cur++;
			if ((*cur++ == 'a') && (*cur++ == 'l') &&
			    (*cur++ == 's') && (*cur++ == 'e')) {
			    ret = 0;
			} else
			    goto return1;
		    } else
			goto return1;
		    if (*cur != 0) {
			while IS_WSP_BLANK_CH(*cur) cur++;
			if (*cur != 0)
			    goto return1;
		    }
		} else {
		    if ((cur[0] == '0') && (cur[1] == 0))
			ret = 0;
		    else if ((cur[0] == '1') && (cur[1] == 0))
			ret = 1;
		    else if ((cur[0] == 't') && (cur[1] == 'r')
			&& (cur[2] == 'u') && (cur[3] == 'e')
			&& (cur[4] == 0))
			ret = 1;
		    else if ((cur[0] == 'f') && (cur[1] == 'a')
			&& (cur[2] == 'l') && (cur[3] == 's')
			&& (cur[4] == 'e') && (cur[5] == 0))
			ret = 0;
		    else
			goto return1;
		}
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_BOOLEAN);
                    if (v != NULL) {
                        v->value.b = ret;
                        *val = v;
                    } else {
                        goto error;
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_TOKEN:{
                const xmlChar *cur = value;

		if (! normOnTheFly) {
		    while (*cur != 0) {
			if ((*cur == 0xd) || (*cur == 0xa) || (*cur == 0x9)) {
			    goto return1;
			} else if (*cur == ' ') {
			    cur++;
			    if (*cur == 0)
				goto return1;
			    if (*cur == ' ')
				goto return1;
			} else {
			    cur++;
			}
		    }		    
		}                
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_TOKEN);
                    if (v != NULL) {
                        v->value.str = xmlStrdup(value);
                        *val = v;
                    } else {
                        goto error;
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_LANGUAGE:
	    if (normOnTheFly) {		    
		norm = xmlSchemaCollapseString(value);
		if (norm != NULL)
		    value = norm;
	    }
            if (xmlCheckLanguageID(value) == 1) {
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_LANGUAGE);
                    if (v != NULL) {
                        v->value.str = xmlStrdup(value);
                        *val = v;
                    } else {
                        goto error;
                    }
                }
                goto return0;
            }
            goto return1;
        case XML_SCHEMAS_NMTOKEN:
            if (xmlValidateNMToken(value, 1) == 0) {
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_NMTOKEN);
                    if (v != NULL) {
                        v->value.str = xmlStrdup(value);
                        *val = v;
                    } else {
                        goto error;
                    }
                }
                goto return0;
            }
            goto return1;
        case XML_SCHEMAS_NMTOKENS:
            ret = xmlSchemaValAtomicListNode(xmlSchemaTypeNmtokenDef,
                                             value, val, node);
            if (ret > 0)
                ret = 0;
            else
                ret = 1;
            goto done;
        case XML_SCHEMAS_NAME:
            ret = xmlValidateName(value, 1);
            if ((ret == 0) && (val != NULL) && (value != NULL)) {
		v = xmlSchemaNewValue(XML_SCHEMAS_NAME);
		if (v != NULL) {
		     const xmlChar *start = value, *end;
		     while (IS_BLANK_CH(*start)) start++;
		     end = start;
		     while ((*end != 0) && (!IS_BLANK_CH(*end))) end++;
		     v->value.str = xmlStrndup(start, end - start);
		    *val = v;
		} else {
		    goto error;
		}
            }
            goto done;
        case XML_SCHEMAS_QNAME:{
                const xmlChar *uri = NULL;
                xmlChar *local = NULL;

                ret = xmlValidateQName(value, 1);
		if (ret != 0)
		    goto done;
                if (node != NULL) {
                    xmlChar *prefix;
		    xmlNsPtr ns;

                    local = xmlSplitQName2(value, &prefix);
		    ns = xmlSearchNs(node->doc, node, prefix);
		    if ((ns == NULL) && (prefix != NULL)) {
			xmlFree(prefix);
			if (local != NULL)
			    xmlFree(local);
			goto return1;
		    }
		    if (ns != NULL)
			uri = ns->href;
                    if (prefix != NULL)
                        xmlFree(prefix);
                }
                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_QNAME);
                    if (v == NULL) {
			if (local != NULL)
			    xmlFree(local);
			goto error;
		    }
		    if (local != NULL)
			v->value.qname.name = local;
		    else
			v->value.qname.name = xmlStrdup(value);
		    if (uri != NULL)
			v->value.qname.uri = xmlStrdup(uri);
		    *val = v;
                } else
		    if (local != NULL)
			xmlFree(local);
                goto done;
            }
        case XML_SCHEMAS_NCNAME:
            ret = xmlValidateNCName(value, 1);
            if ((ret == 0) && (val != NULL)) {
                v = xmlSchemaNewValue(XML_SCHEMAS_NCNAME);
                if (v != NULL) {
                    v->value.str = xmlStrdup(value);
                    *val = v;
                } else {
                    goto error;
                }
            }
            goto done;
        case XML_SCHEMAS_ID:
            ret = xmlValidateNCName(value, 1);
            if ((ret == 0) && (val != NULL)) {
                v = xmlSchemaNewValue(XML_SCHEMAS_ID);
                if (v != NULL) {
                    v->value.str = xmlStrdup(value);
                    *val = v;
                } else {
                    goto error;
                }
            }
            if ((ret == 0) && (node != NULL) &&
                (node->type == XML_ATTRIBUTE_NODE)) {
                xmlAttrPtr attr = (xmlAttrPtr) node;

                if (attr->atype != XML_ATTRIBUTE_ID) {
                    xmlIDPtr res;
                    xmlChar *strip;

                    strip = xmlSchemaStrip(value);
                    if (strip != NULL) {
                        res = xmlAddID(NULL, node->doc, strip, attr);
                        xmlFree(strip);
                    } else
                        res = xmlAddID(NULL, node->doc, value, attr);
                    if (res == NULL) {
                        ret = 2;
                    } else {
                        attr->atype = XML_ATTRIBUTE_ID;
                    }
                }
            }
            goto done;
        case XML_SCHEMAS_IDREF:
            ret = xmlValidateNCName(value, 1);
            if ((ret == 0) && (val != NULL)) {
		v = xmlSchemaNewValue(XML_SCHEMAS_IDREF);
		if (v == NULL)
		    goto error;
		v->value.str = xmlStrdup(value);
		*val = v;
            }
            if ((ret == 0) && (node != NULL) &&
                (node->type == XML_ATTRIBUTE_NODE)) {
                xmlAttrPtr attr = (xmlAttrPtr) node;
                xmlChar *strip;

                strip = xmlSchemaStrip(value);
                if (strip != NULL) {
                    xmlAddRef(NULL, node->doc, strip, attr);
                    xmlFree(strip);
                } else
                    xmlAddRef(NULL, node->doc, value, attr);
                attr->atype = XML_ATTRIBUTE_IDREF;
            }
            goto done;
        case XML_SCHEMAS_IDREFS:
            ret = xmlSchemaValAtomicListNode(xmlSchemaTypeIdrefDef,
                                             value, val, node);
            if (ret < 0)
                ret = 2;
            else
                ret = 0;
            if ((ret == 0) && (node != NULL) &&
                (node->type == XML_ATTRIBUTE_NODE)) {
                xmlAttrPtr attr = (xmlAttrPtr) node;

                attr->atype = XML_ATTRIBUTE_IDREFS;
            }
            goto done;
        case XML_SCHEMAS_ENTITY:{
                xmlChar *strip;

                ret = xmlValidateNCName(value, 1);
                if ((node == NULL) || (node->doc == NULL))
                    ret = 3;
                if (ret == 0) {
                    xmlEntityPtr ent;

                    strip = xmlSchemaStrip(value);
                    if (strip != NULL) {
                        ent = xmlGetDocEntity(node->doc, strip);
                        xmlFree(strip);
                    } else {
                        ent = xmlGetDocEntity(node->doc, value);
                    }
                    if ((ent == NULL) ||
                        (ent->etype !=
                         XML_EXTERNAL_GENERAL_UNPARSED_ENTITY))
                        ret = 4;
                }
                if ((ret == 0) && (val != NULL)) {
                    TODO;
                }
                if ((ret == 0) && (node != NULL) &&
                    (node->type == XML_ATTRIBUTE_NODE)) {
                    xmlAttrPtr attr = (xmlAttrPtr) node;

                    attr->atype = XML_ATTRIBUTE_ENTITY;
                }
                goto done;
            }
        case XML_SCHEMAS_ENTITIES:
            if ((node == NULL) || (node->doc == NULL))
                goto return3;
            ret = xmlSchemaValAtomicListNode(xmlSchemaTypeEntityDef,
                                             value, val, node);
            if (ret <= 0)
                ret = 1;
            else
                ret = 0;
            if ((ret == 0) && (node != NULL) &&
                (node->type == XML_ATTRIBUTE_NODE)) {
                xmlAttrPtr attr = (xmlAttrPtr) node;

                attr->atype = XML_ATTRIBUTE_ENTITIES;
            }
            goto done;
        case XML_SCHEMAS_NOTATION:{
                xmlChar *uri = NULL;
                xmlChar *local = NULL;

                ret = xmlValidateQName(value, 1);
                if ((ret == 0) && (node != NULL)) {
                    xmlChar *prefix;

                    local = xmlSplitQName2(value, &prefix);
                    if (prefix != NULL) {
                        xmlNsPtr ns;

                        ns = xmlSearchNs(node->doc, node, prefix);
                        if (ns == NULL)
                            ret = 1;
                        else if (val != NULL)
                            uri = xmlStrdup(ns->href);
                    }
                    if ((local != NULL) && ((val == NULL) || (ret != 0)))
                        xmlFree(local);
                    if (prefix != NULL)
                        xmlFree(prefix);
                }
                if ((node == NULL) || (node->doc == NULL))
                    ret = 3;
                if (ret == 0) {
                    ret = xmlValidateNotationUse(NULL, node->doc, value);
                    if (ret == 1)
                        ret = 0;
                    else
                        ret = 1;
                }
                if ((ret == 0) && (val != NULL)) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_NOTATION);
                    if (v != NULL) {
                        if (local != NULL)
                            v->value.qname.name = local;
                        else
                            v->value.qname.name = xmlStrdup(value);
                        if (uri != NULL)
                            v->value.qname.uri = uri;

                        *val = v;
                    } else {
                        if (local != NULL)
                            xmlFree(local);
                        if (uri != NULL)
                            xmlFree(uri);
                        goto error;
                    }
                }
                goto done;
            }
        case XML_SCHEMAS_ANYURI:{		
                if (*value != 0) {
		    xmlURIPtr uri;
		    xmlChar *tmpval, *cur;
		    if (normOnTheFly) {		    
			norm = xmlSchemaCollapseString(value);
			if (norm != NULL)
			    value = norm;
		    }
		    tmpval = xmlStrdup(value);
		    for (cur = tmpval; *cur; ++cur) {
			if (*cur < 32 || *cur >= 127 || *cur == ' ' ||
			    *cur == '<' || *cur == '>' || *cur == '"' ||
			    *cur == '{' || *cur == '}' || *cur == '|' ||
			    *cur == '\\' || *cur == '^' || *cur == '`' ||
			    *cur == '\'')
			    *cur = '_';
		    }
                    uri = xmlParseURI((const char *) tmpval);
		    xmlFree(tmpval);
                    if (uri == NULL)
                        goto return1;
                    xmlFreeURI(uri);
                }

                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_ANYURI);
                    if (v == NULL)
                        goto error;
                    v->value.str = xmlStrdup(value);
                    *val = v;
                }
                goto return0;
            }
        case XML_SCHEMAS_HEXBINARY:{
                const xmlChar *cur = value, *start;
                xmlChar *base;
                int total, i = 0;

                if (cur == NULL)
                    goto return1;

		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;

		start = cur;
                while (((*cur >= '0') && (*cur <= '9')) ||
                       ((*cur >= 'A') && (*cur <= 'F')) ||
                       ((*cur >= 'a') && (*cur <= 'f'))) {
                    i++;
                    cur++;
                }
		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;

                if (*cur != 0)
                    goto return1;
                if ((i % 2) != 0)
                    goto return1;

                if (val != NULL) {

                    v = xmlSchemaNewValue(XML_SCHEMAS_HEXBINARY);
                    if (v == NULL)
                        goto error;
                    cur = xmlStrndup(start, i);
                    if (cur == NULL) {
		        xmlSchemaTypeErrMemory(node, "allocating hexbin data");
                        xmlFree(v);
                        goto return1;
                    }

                    total = i / 2;      

                    base = (xmlChar *) cur;
                    while (i-- > 0) {
                        if (*base >= 'a')
                            *base = *base - ('a' - 'A');
                        base++;
                    }

                    v->value.hex.str = (xmlChar *) cur;
                    v->value.hex.total = total;
                    *val = v;
                }
                goto return0;
            }
        case XML_SCHEMAS_BASE64BINARY:{
                const xmlChar *cur = value;
                xmlChar *base;
                int total, i = 0, pad = 0;

                if (cur == NULL)
                    goto return1;

                for (; *cur; ++cur) {
                    int decc;

                    decc = _xmlSchemaBase64Decode(*cur);
                    if (decc < 0) ;
                    else if (decc < 64)
                        i++;
                    else
                        break;
                }
                for (; *cur; ++cur) {
                    int decc;

                    decc = _xmlSchemaBase64Decode(*cur);
                    if (decc < 0) ;
                    else if (decc < 64)
                        goto return1;
                    if (decc == 64)
                        pad++;
                }


                total = 3 * (i / 4);
                if (pad == 0) {
                    if (i % 4 != 0)
                        goto return1;
                } else if (pad == 1) {
                    int decc;

                    if (i % 4 != 3)
                        goto return1;
                    for (decc = _xmlSchemaBase64Decode(*cur);
                         (decc < 0) || (decc > 63);
                         decc = _xmlSchemaBase64Decode(*cur))
                        --cur;
                    
                    
                    if (decc & ~0x3c)
                        goto return1;
                    total += 2;
                } else if (pad == 2) {
                    int decc;

                    if (i % 4 != 2)
                        goto return1;
                    for (decc = _xmlSchemaBase64Decode(*cur);
                         (decc < 0) || (decc > 63);
                         decc = _xmlSchemaBase64Decode(*cur))
                        --cur;
                    
                    
                    if (decc & ~0x30)
                        goto return1;
                    total += 1;
                } else
                    goto return1;

                if (val != NULL) {
                    v = xmlSchemaNewValue(XML_SCHEMAS_BASE64BINARY);
                    if (v == NULL)
                        goto error;
                    base =
                        (xmlChar *) xmlMallocAtomic((i + pad + 1) *
                                                    sizeof(xmlChar));
                    if (base == NULL) {
		        xmlSchemaTypeErrMemory(node, "allocating base64 data");
                        xmlFree(v);
                        goto return1;
                    }
                    v->value.base64.str = base;
                    for (cur = value; *cur; ++cur)
                        if (_xmlSchemaBase64Decode(*cur) >= 0) {
                            *base = *cur;
                            ++base;
                        }
                    *base = 0;
                    v->value.base64.total = total;
                    *val = v;
                }
                goto return0;
            }
        case XML_SCHEMAS_INTEGER:
        case XML_SCHEMAS_PINTEGER:
        case XML_SCHEMAS_NPINTEGER:
        case XML_SCHEMAS_NINTEGER:
        case XML_SCHEMAS_NNINTEGER:{
                const xmlChar *cur = value;
                unsigned long lo, mi, hi;
                int sign = 0;

                if (cur == NULL)
                    goto return1;
		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;
                if (*cur == '-') {
                    sign = 1;
                    cur++;
                } else if (*cur == '+')
                    cur++;
                ret = xmlSchemaParseUInt(&cur, &lo, &mi, &hi);
                if (ret < 0)
                    goto return1;
		if (normOnTheFly)
		    while IS_WSP_BLANK_CH(*cur) cur++;
                if (*cur != 0)
                    goto return1;
                if (type->builtInType == XML_SCHEMAS_NPINTEGER) {
                    if ((sign == 0) &&
                        ((hi != 0) || (mi != 0) || (lo != 0)))
                        goto return1;
                } else if (type->builtInType == XML_SCHEMAS_PINTEGER) {
                    if (sign == 1)
                        goto return1;
                    if ((hi == 0) && (mi == 0) && (lo == 0))
                        goto return1;
                } else if (type->builtInType == XML_SCHEMAS_NINTEGER) {
                    if (sign == 0)
                        goto return1;
                    if ((hi == 0) && (mi == 0) && (lo == 0))
                        goto return1;
                } else if (type->builtInType == XML_SCHEMAS_NNINTEGER) {
                    if ((sign == 1) &&
                        ((hi != 0) || (mi != 0) || (lo != 0)))
                        goto return1;
                }
                if (val != NULL) {
                    v = xmlSchemaNewValue(type->builtInType);
                    if (v != NULL) {
			if (ret == 0)
			    ret++;
                        v->value.decimal.lo = lo;
                        v->value.decimal.mi = mi;
                        v->value.decimal.hi = hi;
                        v->value.decimal.sign = sign;
                        v->value.decimal.frac = 0;
                        v->value.decimal.total = ret;
                        *val = v;
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_LONG:
        case XML_SCHEMAS_BYTE:
        case XML_SCHEMAS_SHORT:
        case XML_SCHEMAS_INT:{
                const xmlChar *cur = value;
                unsigned long lo, mi, hi;
                int sign = 0;

                if (cur == NULL)
                    goto return1;
                if (*cur == '-') {
                    sign = 1;
                    cur++;
                } else if (*cur == '+')
                    cur++;
                ret = xmlSchemaParseUInt(&cur, &lo, &mi, &hi);
                if (ret < 0)
                    goto return1;
                if (*cur != 0)
                    goto return1;
                if (type->builtInType == XML_SCHEMAS_LONG) {
                    if (hi >= 922) {
                        if (hi > 922)
                            goto return1;
                        if (mi >= 33720368) {
                            if (mi > 33720368)
                                goto return1;
                            if ((sign == 0) && (lo > 54775807))
                                goto return1;
                            if ((sign == 1) && (lo > 54775808))
                                goto return1;
                        }
                    }
                } else if (type->builtInType == XML_SCHEMAS_INT) {
                    if (hi != 0)
                        goto return1;
                    if (mi >= 21) {
                        if (mi > 21)
                            goto return1;
                        if ((sign == 0) && (lo > 47483647))
                            goto return1;
                        if ((sign == 1) && (lo > 47483648))
                            goto return1;
                    }
                } else if (type->builtInType == XML_SCHEMAS_SHORT) {
                    if ((mi != 0) || (hi != 0))
                        goto return1;
                    if ((sign == 1) && (lo > 32768))
                        goto return1;
                    if ((sign == 0) && (lo > 32767))
                        goto return1;
                } else if (type->builtInType == XML_SCHEMAS_BYTE) {
                    if ((mi != 0) || (hi != 0))
                        goto return1;
                    if ((sign == 1) && (lo > 128))
                        goto return1;
                    if ((sign == 0) && (lo > 127))
                        goto return1;
                }
                if (val != NULL) {
                    v = xmlSchemaNewValue(type->builtInType);
                    if (v != NULL) {
                        v->value.decimal.lo = lo;
                        v->value.decimal.mi = mi;
                        v->value.decimal.hi = hi;
                        v->value.decimal.sign = sign;
                        v->value.decimal.frac = 0;
                        v->value.decimal.total = ret;
                        *val = v;
                    }
                }
                goto return0;
            }
        case XML_SCHEMAS_UINT:
        case XML_SCHEMAS_ULONG:
        case XML_SCHEMAS_USHORT:
        case XML_SCHEMAS_UBYTE:{
                const xmlChar *cur = value;
                unsigned long lo, mi, hi;

                if (cur == NULL)
                    goto return1;
                ret = xmlSchemaParseUInt(&cur, &lo, &mi, &hi);
                if (ret < 0)
                    goto return1;
                if (*cur != 0)
                    goto return1;
                if (type->builtInType == XML_SCHEMAS_ULONG) {
                    if (hi >= 1844) {
                        if (hi > 1844)
                            goto return1;
                        if (mi >= 67440737) {
                            if (mi > 67440737)
                                goto return1;
                            if (lo > 9551615)
                                goto return1;
                        }
                    }
                } else if (type->builtInType == XML_SCHEMAS_UINT) {
                    if (hi != 0)
                        goto return1;
                    if (mi >= 42) {
                        if (mi > 42)
                            goto return1;
                        if (lo > 94967295)
                            goto return1;
                    }
                } else if (type->builtInType == XML_SCHEMAS_USHORT) {
                    if ((mi != 0) || (hi != 0))
                        goto return1;
                    if (lo > 65535)
                        goto return1;
                } else if (type->builtInType == XML_SCHEMAS_UBYTE) {
                    if ((mi != 0) || (hi != 0))
                        goto return1;
                    if (lo > 255)
                        goto return1;
                }
                if (val != NULL) {
                    v = xmlSchemaNewValue(type->builtInType);
                    if (v != NULL) {
                        v->value.decimal.lo = lo;
                        v->value.decimal.mi = mi;
                        v->value.decimal.hi = hi;
                        v->value.decimal.sign = 0;
                        v->value.decimal.frac = 0;
                        v->value.decimal.total = ret;
                        *val = v;
                    }
                }
                goto return0;
            }
    }

  done:
    if (norm != NULL)
        xmlFree(norm);
    return (ret);
  return3:
    if (norm != NULL)
        xmlFree(norm);
    return (3);
  return1:
    if (norm != NULL)
        xmlFree(norm);
    return (1);
  return0:
    if (norm != NULL)
        xmlFree(norm);
    return (0);
  error:
    if (norm != NULL)
        xmlFree(norm);
    return (-1);
}

int
xmlSchemaValPredefTypeNode(xmlSchemaTypePtr type, const xmlChar *value,
	                   xmlSchemaValPtr *val, xmlNodePtr node) {
    return(xmlSchemaValAtomicType(type, value, val, node, 0,
	XML_SCHEMA_WHITESPACE_UNKNOWN, 1, 1, 0));
}

int
xmlSchemaValPredefTypeNodeNoNorm(xmlSchemaTypePtr type, const xmlChar *value,
				 xmlSchemaValPtr *val, xmlNodePtr node) {
    return(xmlSchemaValAtomicType(type, value, val, node, 1,
	XML_SCHEMA_WHITESPACE_UNKNOWN, 1, 0, 1));
}

int
xmlSchemaValidatePredefinedType(xmlSchemaTypePtr type, const xmlChar *value,
	                        xmlSchemaValPtr *val) {
    return(xmlSchemaValPredefTypeNode(type, value, val, NULL));
}

static int
xmlSchemaCompareDecimals(xmlSchemaValPtr x, xmlSchemaValPtr y)
{
    xmlSchemaValPtr swp;
    int order = 1, integx, integy, dlen;
    unsigned long hi, mi, lo;

    if ((x->value.decimal.sign) && 
	((x->value.decimal.lo != 0) ||
	 (x->value.decimal.mi != 0) ||
	 (x->value.decimal.hi != 0))) {
	if ((y->value.decimal.sign) &&
	    ((y->value.decimal.lo != 0) ||
	     (y->value.decimal.mi != 0) ||
	     (y->value.decimal.hi != 0)))
	    order = -1;
	else
	    return (-1);
    } else if ((y->value.decimal.sign) &&
	       ((y->value.decimal.lo != 0) ||
		(y->value.decimal.mi != 0) ||
		(y->value.decimal.hi != 0))) {
        return (1);
    }
    integx = x->value.decimal.total - x->value.decimal.frac;
    integy = y->value.decimal.total - y->value.decimal.frac;
    if (integx == 1) {
	if (x->value.decimal.lo == 0) {
	    if (integy != 1)
		return -order;
	    else if (y->value.decimal.lo != 0)
		return -order;
	    else
		return(0);
	}
    }
    if (integy == 1) {
	if (y->value.decimal.lo == 0) {
	    if (integx != 1)
		return order;
	    else if (x->value.decimal.lo != 0)
		return order;
	    else
		return(0);
	}
    }

    if (integx > integy)
	return order;
    else if (integy > integx)
	return -order;

    dlen = x->value.decimal.total - y->value.decimal.total;
    if (dlen < 0) {	
	swp = x;
	hi = y->value.decimal.hi;
	mi = y->value.decimal.mi;
	lo = y->value.decimal.lo;
	dlen = -dlen;
	order = -order;
    } else {		
	swp = y;
	hi = x->value.decimal.hi;
	mi = x->value.decimal.mi;
	lo = x->value.decimal.lo;
    }
    while (dlen > 8) {	
	lo = mi;
	mi = hi;
	hi = 0;
	dlen -= 8;
    }
    while (dlen > 0) {
	unsigned long rem1, rem2;
	rem1 = (hi % 10) * 100000000L;
	hi = hi / 10;
	rem2 = (mi % 10) * 100000000L;
	mi = (mi + rem1) / 10;
	lo = (lo + rem2) / 10;
	dlen--;
    }
    if (hi > swp->value.decimal.hi) {
	return order;
    } else if (hi == swp->value.decimal.hi) {
	if (mi > swp->value.decimal.mi) {
	    return order;
	} else if (mi == swp->value.decimal.mi) {
	    if (lo > swp->value.decimal.lo) {
		return order;
	    } else if (lo == swp->value.decimal.lo) {
		if (x->value.decimal.total == y->value.decimal.total) {
		    return 0;
		} else {
		    return order;
		}
	    }
	}
    }
    return -order;
}

static int
xmlSchemaCompareDurations(xmlSchemaValPtr x, xmlSchemaValPtr y)
{
    long carry, mon, day;
    double sec;
    int invert = 1;
    long xmon, xday, myear, minday, maxday;
    static const long dayRange [2][12] = {
        { 0, 28, 59, 89, 120, 150, 181, 212, 242, 273, 303, 334, },
        { 0, 31, 62, 92, 123, 153, 184, 215, 245, 276, 306, 337} };

    if ((x == NULL) || (y == NULL))
        return -2;

    
    mon = x->value.dur.mon - y->value.dur.mon;

    
    sec = x->value.dur.sec - y->value.dur.sec;
    carry = (long)sec / SECS_PER_DAY;
    sec -= (double)(carry * SECS_PER_DAY);

    
    day = x->value.dur.day - y->value.dur.day + carry;

    
    if (mon == 0) {
        if (day == 0)
            if (sec == 0.0)
                return 0;
            else if (sec < 0.0)
                return -1;
            else
                return 1;
        else if (day < 0)
            return -1;
        else
            return 1;
    }

    if (mon > 0) {
        if ((day >= 0) && (sec >= 0.0))
            return 1;
        else {
            xmon = mon;
            xday = -day;
        }
    } else if ((day <= 0) && (sec <= 0.0)) {
        return -1;
    } else {
	invert = -1;
        xmon = -mon;
        xday = day;
    }

    myear = xmon / 12;
    if (myear == 0) {
	minday = 0;
	maxday = 0;
    } else {
	maxday = 366 * ((myear + 3) / 4) +
	         365 * ((myear - 1) % 4);
	minday = maxday - 1;
    }

    xmon = xmon % 12;
    minday += dayRange[0][xmon];
    maxday += dayRange[1][xmon];

    if ((maxday == minday) && (maxday == xday))
	return(0); 
    if (maxday < xday)
        return(-invert);
    if (minday > xday)
        return(invert);

    
    return 2;
}

#define FQUOTIENT(a,b)                  (floor(((double)a/(double)b)))
#define MODULO(a,b)                     (a - FQUOTIENT(a,b) * b)
#define FQUOTIENT_RANGE(a,low,high)     (FQUOTIENT((a-low),(high-low)))
#define MODULO_RANGE(a,low,high)        ((MODULO((a-low),(high-low)))+low)

static xmlSchemaValPtr
xmlSchemaDupVal (xmlSchemaValPtr v)
{
    xmlSchemaValPtr ret = xmlSchemaNewValue(v->type);
    if (ret == NULL)
        return NULL;
    
    memcpy(ret, v, sizeof(xmlSchemaVal));
    ret->next = NULL;
    return ret;
}

xmlSchemaValPtr
xmlSchemaCopyValue(xmlSchemaValPtr val)
{
    xmlSchemaValPtr ret = NULL, prev = NULL, cur;

    while (val != NULL) {
	switch (val->type) {
	    case XML_SCHEMAS_ANYTYPE:
	    case XML_SCHEMAS_IDREFS:
	    case XML_SCHEMAS_ENTITIES:
	    case XML_SCHEMAS_NMTOKENS:
		xmlSchemaFreeValue(ret);
		return (NULL);
	    case XML_SCHEMAS_ANYSIMPLETYPE:
	    case XML_SCHEMAS_STRING:
	    case XML_SCHEMAS_NORMSTRING:
	    case XML_SCHEMAS_TOKEN:
	    case XML_SCHEMAS_LANGUAGE:
	    case XML_SCHEMAS_NAME:
	    case XML_SCHEMAS_NCNAME:
	    case XML_SCHEMAS_ID:
	    case XML_SCHEMAS_IDREF:
	    case XML_SCHEMAS_ENTITY:
	    case XML_SCHEMAS_NMTOKEN:
	    case XML_SCHEMAS_ANYURI:
		cur = xmlSchemaDupVal(val);
		if (val->value.str != NULL)
		    cur->value.str = xmlStrdup(BAD_CAST val->value.str);
		break;
	    case XML_SCHEMAS_QNAME:        
	    case XML_SCHEMAS_NOTATION:
		cur = xmlSchemaDupVal(val);
		if (val->value.qname.name != NULL)
		    cur->value.qname.name =
                    xmlStrdup(BAD_CAST val->value.qname.name);
		if (val->value.qname.uri != NULL)
		    cur->value.qname.uri =
                    xmlStrdup(BAD_CAST val->value.qname.uri);
		break;
	    case XML_SCHEMAS_HEXBINARY:
		cur = xmlSchemaDupVal(val);
		if (val->value.hex.str != NULL)
		    cur->value.hex.str = xmlStrdup(BAD_CAST val->value.hex.str);
		break;
	    case XML_SCHEMAS_BASE64BINARY:
		cur = xmlSchemaDupVal(val);
		if (val->value.base64.str != NULL)
		    cur->value.base64.str =
                    xmlStrdup(BAD_CAST val->value.base64.str);
		break;
	    default:
		cur = xmlSchemaDupVal(val);
		break;
	}
	if (ret == NULL)
	    ret = cur;
	else
	    prev->next = cur;
	prev = cur;
	val = val->next;
    }
    return (ret);
}

static xmlSchemaValPtr
_xmlSchemaDateAdd (xmlSchemaValPtr dt, xmlSchemaValPtr dur)
{
    xmlSchemaValPtr ret, tmp;
    long carry, tempdays, temp;
    xmlSchemaValDatePtr r, d;
    xmlSchemaValDurationPtr u;

    if ((dt == NULL) || (dur == NULL))
        return NULL;

    ret = xmlSchemaNewValue(dt->type);
    if (ret == NULL)
        return NULL;

    
    tmp = xmlSchemaDupVal(dt);
    if (tmp == NULL) {
        xmlSchemaFreeValue(ret);
        return NULL;
    }

    r = &(ret->value.date);
    d = &(tmp->value.date);
    u = &(dur->value.dur);

    
    if (d->mon == 0)
        d->mon = 1;

    
    u->sec -= (d->tzo * 60);
    d->tzo = 0;

    
    if (d->day == 0)
        d->day = 1;

    
    carry  = d->mon + u->mon;
    r->mon = (unsigned int) MODULO_RANGE(carry, 1, 13);
    carry  = (long) FQUOTIENT_RANGE(carry, 1, 13);

    
    r->year = d->year + carry;
    if (r->year == 0) {
        if (d->year > 0)
            r->year--;
        else
            r->year++;
    }

    
    r->tzo     = d->tzo;
    r->tz_flag = d->tz_flag;

    
    r->sec = d->sec + u->sec;
    carry  = (long) FQUOTIENT((long)r->sec, 60);
    if (r->sec != 0.0) {
        r->sec = MODULO(r->sec, 60.0);
    }

    
    carry += d->min;
    r->min = (unsigned int) MODULO(carry, 60);
    carry  = (long) FQUOTIENT(carry, 60);

    
    carry  += d->hour;
    r->hour = (unsigned int) MODULO(carry, 24);
    carry   = (long)FQUOTIENT(carry, 24);

    if ((VALID_YEAR(r->year)) && (VALID_MONTH(r->mon)) &&
                  (d->day > MAX_DAYINMONTH(r->year, r->mon)))
        tempdays = MAX_DAYINMONTH(r->year, r->mon);
    else if (d->day < 1)
        tempdays = 1;
    else
        tempdays = d->day;

    tempdays += u->day + carry;

    while (1) {
        if (tempdays < 1) {
            long tmon = (long) MODULO_RANGE((int)r->mon-1, 1, 13);
            long tyr  = r->year + (long)FQUOTIENT_RANGE((int)r->mon-1, 1, 13);
            if (tyr == 0)
                tyr--;
	    if (tmon < 0)
	        tmon = 0;
	    if (tmon > 12)
	        tmon = 12;
            tempdays += MAX_DAYINMONTH(tyr, tmon);
            carry = -1;
        } else if (tempdays > (long) MAX_DAYINMONTH(r->year, r->mon)) {
            tempdays = tempdays - MAX_DAYINMONTH(r->year, r->mon);
            carry = 1;
        } else
            break;

        temp = r->mon + carry;
        r->mon = (unsigned int) MODULO_RANGE(temp, 1, 13);
        r->year = r->year + (unsigned int) FQUOTIENT_RANGE(temp, 1, 13);
        if (r->year == 0) {
            if (temp < 1)
                r->year--;
            else
                r->year++;
	}
    }
    
    r->day = tempdays;

    if (ret->type != XML_SCHEMAS_DATETIME) {
        if ((r->hour) || (r->min) || (r->sec))
            ret->type = XML_SCHEMAS_DATETIME;
        else if (ret->type != XML_SCHEMAS_DATE) {
            if ((r->mon != 1) && (r->day != 1))
                ret->type = XML_SCHEMAS_DATE;
            else if ((ret->type != XML_SCHEMAS_GYEARMONTH) && (r->mon != 1))
                ret->type = XML_SCHEMAS_GYEARMONTH;
        }
    }

    xmlSchemaFreeValue(tmp);

    return ret;
}

static xmlSchemaValPtr
xmlSchemaDateNormalize (xmlSchemaValPtr dt, double offset)
{
    xmlSchemaValPtr dur, ret;

    if (dt == NULL)
        return NULL;

    if (((dt->type != XML_SCHEMAS_TIME) &&
         (dt->type != XML_SCHEMAS_DATETIME) &&
	 (dt->type != XML_SCHEMAS_DATE)) || (dt->value.date.tzo == 0))
        return xmlSchemaDupVal(dt);

    dur = xmlSchemaNewValue(XML_SCHEMAS_DURATION);
    if (dur == NULL)
        return NULL;

    dur->value.date.sec -= offset;

    ret = _xmlSchemaDateAdd(dt, dur);
    if (ret == NULL)
        return NULL;

    xmlSchemaFreeValue(dur);

    
    return ret;
}

static long
_xmlSchemaDateCastYMToDays (const xmlSchemaValPtr dt)
{
    long ret;
    int mon;

    mon = dt->value.date.mon;
    if (mon <= 0) mon = 1; 

    if (dt->value.date.year <= 0)
        ret = (dt->value.date.year * 365) +
              (((dt->value.date.year+1)/4)-((dt->value.date.year+1)/100)+
               ((dt->value.date.year+1)/400)) +
              DAY_IN_YEAR(0, mon, dt->value.date.year);
    else
        ret = ((dt->value.date.year-1) * 365) +
              (((dt->value.date.year-1)/4)-((dt->value.date.year-1)/100)+
               ((dt->value.date.year-1)/400)) +
              DAY_IN_YEAR(0, mon, dt->value.date.year);

    return ret;
}

#define TIME_TO_NUMBER(dt)                              \
    ((double)((dt->value.date.hour * SECS_PER_HOUR) +   \
              (dt->value.date.min * SECS_PER_MIN) +	\
              (dt->value.date.tzo * SECS_PER_MIN)) +	\
               dt->value.date.sec)

static int
xmlSchemaCompareDates (xmlSchemaValPtr x, xmlSchemaValPtr y)
{
    unsigned char xmask, ymask, xor_mask, and_mask;
    xmlSchemaValPtr p1, p2, q1, q2;
    long p1d, p2d, q1d, q2d;

    if ((x == NULL) || (y == NULL))
        return -2;

    if (x->value.date.tz_flag) {

        if (!y->value.date.tz_flag) {
            p1 = xmlSchemaDateNormalize(x, 0);
            p1d = _xmlSchemaDateCastYMToDays(p1) + p1->value.date.day;
            
            q1 = xmlSchemaDateNormalize(y, (14 * SECS_PER_HOUR));

            q1d = _xmlSchemaDateCastYMToDays(q1) + q1->value.date.day;
            if (p1d < q1d) {
		xmlSchemaFreeValue(p1);
		xmlSchemaFreeValue(q1);
                return -1;
	    } else if (p1d == q1d) {
                double sec;

                sec = TIME_TO_NUMBER(p1) - TIME_TO_NUMBER(q1);
                if (sec < 0.0) {
		    xmlSchemaFreeValue(p1);
		    xmlSchemaFreeValue(q1);
                    return -1;
		} else {
		    int ret = 0;
                    
                    q2 = xmlSchemaDateNormalize(y, -(14 * SECS_PER_HOUR));
                    q2d = _xmlSchemaDateCastYMToDays(q2) + q2->value.date.day;
                    if (p1d > q2d)
                        ret = 1;
                    else if (p1d == q2d) {
                        sec = TIME_TO_NUMBER(p1) - TIME_TO_NUMBER(q2);
                        if (sec > 0.0)
                            ret = 1;
                        else
                            ret = 2; 
                    }
		    xmlSchemaFreeValue(p1);
		    xmlSchemaFreeValue(q1);
		    xmlSchemaFreeValue(q2);
		    if (ret != 0)
		        return(ret);
                }
            } else {
		xmlSchemaFreeValue(p1);
		xmlSchemaFreeValue(q1);
	    }
        }
    } else if (y->value.date.tz_flag) {
        q1 = xmlSchemaDateNormalize(y, 0);
        q1d = _xmlSchemaDateCastYMToDays(q1) + q1->value.date.day;

        
        p1 = xmlSchemaDateNormalize(x, -(14 * SECS_PER_HOUR));
        p1d = _xmlSchemaDateCastYMToDays(p1) + p1->value.date.day;

        if (p1d < q1d) {
	    xmlSchemaFreeValue(p1);
	    xmlSchemaFreeValue(q1);
            return -1;
	} else if (p1d == q1d) {
            double sec;

            sec = TIME_TO_NUMBER(p1) - TIME_TO_NUMBER(q1);
            if (sec < 0.0) {
		xmlSchemaFreeValue(p1);
		xmlSchemaFreeValue(q1);
                return -1;
	    } else {
	        int ret = 0;
                
                p2 = xmlSchemaDateNormalize(x, (14 * SECS_PER_HOUR));
                p2d = _xmlSchemaDateCastYMToDays(p2) + p2->value.date.day;

                if (p2d > q1d) {
                    ret = 1;
		} else if (p2d == q1d) {
                    sec = TIME_TO_NUMBER(p2) - TIME_TO_NUMBER(q1);
                    if (sec > 0.0)
                        ret = 1;
                    else
                        ret = 2; 
                }
		xmlSchemaFreeValue(p1);
		xmlSchemaFreeValue(q1);
		xmlSchemaFreeValue(p2);
		if (ret != 0)
		    return(ret);
            }
	} else {
	    xmlSchemaFreeValue(p1);
	    xmlSchemaFreeValue(q1);
        }
    }

    if (x->type == y->type) {
        int ret = 0;
        q1 = xmlSchemaDateNormalize(y, 0);
        q1d = _xmlSchemaDateCastYMToDays(q1) + q1->value.date.day;

        p1 = xmlSchemaDateNormalize(x, 0);
        p1d = _xmlSchemaDateCastYMToDays(p1) + p1->value.date.day;

        if (p1d < q1d) {
            ret = -1;
	} else if (p1d > q1d) {
            ret = 1;
	} else {
            double sec;

            sec = TIME_TO_NUMBER(p1) - TIME_TO_NUMBER(q1);
            if (sec < 0.0)
                ret = -1;
            else if (sec > 0.0)
                ret = 1;
            
        }
	xmlSchemaFreeValue(p1);
	xmlSchemaFreeValue(q1);
        return(ret);
    }

    switch (x->type) {
        case XML_SCHEMAS_DATETIME:
            xmask = 0xf;
            break;
        case XML_SCHEMAS_DATE:
            xmask = 0x7;
            break;
        case XML_SCHEMAS_GYEAR:
            xmask = 0x1;
            break;
        case XML_SCHEMAS_GMONTH:
            xmask = 0x2;
            break;
        case XML_SCHEMAS_GDAY:
            xmask = 0x3;
            break;
        case XML_SCHEMAS_GYEARMONTH:
            xmask = 0x3;
            break;
        case XML_SCHEMAS_GMONTHDAY:
            xmask = 0x6;
            break;
        case XML_SCHEMAS_TIME:
            xmask = 0x8;
            break;
        default:
            xmask = 0;
            break;
    }

    switch (y->type) {
        case XML_SCHEMAS_DATETIME:
            ymask = 0xf;
            break;
        case XML_SCHEMAS_DATE:
            ymask = 0x7;
            break;
        case XML_SCHEMAS_GYEAR:
            ymask = 0x1;
            break;
        case XML_SCHEMAS_GMONTH:
            ymask = 0x2;
            break;
        case XML_SCHEMAS_GDAY:
            ymask = 0x3;
            break;
        case XML_SCHEMAS_GYEARMONTH:
            ymask = 0x3;
            break;
        case XML_SCHEMAS_GMONTHDAY:
            ymask = 0x6;
            break;
        case XML_SCHEMAS_TIME:
            ymask = 0x8;
            break;
        default:
            ymask = 0;
            break;
    }

    xor_mask = xmask ^ ymask;           
    and_mask = xmask & ymask;           

    
    if (xor_mask & 1)
        return 2; 
    else if (and_mask & 1) {
        if (x->value.date.year < y->value.date.year)
            return -1;
        else if (x->value.date.year > y->value.date.year)
            return 1;
    }

    
    if (xor_mask & 2)
        return 2; 
    else if (and_mask & 2) {
        if (x->value.date.mon < y->value.date.mon)
            return -1;
        else if (x->value.date.mon > y->value.date.mon)
            return 1;
    }

    
    if (xor_mask & 4)
        return 2; 
    else if (and_mask & 4) {
        if (x->value.date.day < y->value.date.day)
            return -1;
        else if (x->value.date.day > y->value.date.day)
            return 1;
    }

    
    if (xor_mask & 8)
        return 2; 
    else if (and_mask & 8) {
        if (x->value.date.hour < y->value.date.hour)
            return -1;
        else if (x->value.date.hour > y->value.date.hour)
            return 1;
        else if (x->value.date.min < y->value.date.min)
            return -1;
        else if (x->value.date.min > y->value.date.min)
            return 1;
        else if (x->value.date.sec < y->value.date.sec)
            return -1;
        else if (x->value.date.sec > y->value.date.sec)
            return 1;
    }

    return 0;
}

static int
xmlSchemaComparePreserveReplaceStrings(const xmlChar *x,
				       const xmlChar *y,
				       int invert)
{
    int tmp;
    
    while ((*x != 0) && (*y != 0)) {
	if (IS_WSP_REPLACE_CH(*y)) {
	    if (! IS_WSP_SPACE_CH(*x)) {
		if ((*x - 0x20) < 0) {
		    if (invert)
			return(1);
		    else
			return(-1);
		} else {
		    if (invert)
			return(-1);
		    else
			return(1);
		}
	    }	    
	} else {
	    tmp = *x - *y;
	    if (tmp < 0) {
		if (invert)
		    return(1);
		else
		    return(-1);
	    }
	    if (tmp > 0) {
		if (invert)
		    return(-1);
		else
		    return(1);
	    }
	}
	x++;
	y++;
    }
    if (*x != 0) {
	if (invert)
	    return(-1);
	else
	    return(1);
    }
    if (*y != 0) {
	if (invert)
	    return(1);
	else
	    return(-1);
    }
    return(0);
}

static int
xmlSchemaComparePreserveCollapseStrings(const xmlChar *x,
				        const xmlChar *y,
					int invert)
{
    int tmp;

    while IS_WSP_BLANK_CH(*y)
	y++;

    while ((*x != 0) && (*y != 0)) {
	if IS_WSP_BLANK_CH(*y) {
	    if (! IS_WSP_SPACE_CH(*x)) {
		if ((*x - 0x20) < 0) {
		    if (invert)
			return(1);
		    else
			return(-1);
		} else {
		    if (invert)
			return(-1);
		    else
			return(1);
		}
	    }
	    x++;
	    y++;
	    while IS_WSP_BLANK_CH(*y)
		y++;
	} else {
	    tmp = *x++ - *y++;
	    if (tmp < 0) {
		if (invert)
		    return(1);
		else
		    return(-1);
	    }
	    if (tmp > 0) {
		if (invert)
		    return(-1);
		else
		    return(1);
	    }
	}
    }
    if (*x != 0) {
	 if (invert)
	     return(-1);
	 else
	     return(1);
    }
    if (*y != 0) {
	while IS_WSP_BLANK_CH(*y)
	    y++;
	if (*y != 0) {
	    if (invert)
		return(1);
	    else
		return(-1);
	}
    }
    return(0);
}

static int
xmlSchemaCompareReplaceCollapseStrings(const xmlChar *x,
				       const xmlChar *y,
				       int invert)
{
    int tmp;

    while IS_WSP_BLANK_CH(*y)
	y++;
    
    while ((*x != 0) && (*y != 0)) {
	if IS_WSP_BLANK_CH(*y) {
	    if (! IS_WSP_BLANK_CH(*x)) {
		if ((*x - 0x20) < 0) {
		    if (invert)
			return(1);
		    else
			return(-1);
		} else {
		    if (invert)
			return(-1);
		    else
			return(1);
		}
	    }
	    x++;
	    y++;	    
	    while IS_WSP_BLANK_CH(*y)
		y++;
	} else {
	    if IS_WSP_BLANK_CH(*x) {
		if ((0x20 - *y) < 0) {
		    if (invert)
			return(1);
		    else
			return(-1);
		} else {
		    if (invert)
			return(-1);
		    else
			return(1);
		}
	    }
	    tmp = *x++ - *y++;
	    if (tmp < 0)
		return(-1);
	    if (tmp > 0)
		return(1);
	}
    }
    if (*x != 0) {
	 if (invert)
	     return(-1);
	 else
	     return(1);
    }   
    if (*y != 0) {
	while IS_WSP_BLANK_CH(*y)
	    y++;
	if (*y != 0) {
	    if (invert)
		return(1);
	    else
		return(-1);
	}
    }
    return(0);
}


static int
xmlSchemaCompareReplacedStrings(const xmlChar *x,
				const xmlChar *y)
{
    int tmp;
   
    while ((*x != 0) && (*y != 0)) {
	if IS_WSP_BLANK_CH(*y) {
	    if (! IS_WSP_BLANK_CH(*x)) {
		if ((*x - 0x20) < 0)
    		    return(-1);
		else
		    return(1);
	    }	    
	} else {
	    if IS_WSP_BLANK_CH(*x) {
		if ((0x20 - *y) < 0)
    		    return(-1);
		else
		    return(1);
	    }
	    tmp = *x - *y;
	    if (tmp < 0)
    		return(-1);
	    if (tmp > 0)
    		return(1);
	}
	x++;
	y++;
    }
    if (*x != 0)
        return(1);
    if (*y != 0)
        return(-1);
    return(0);
}

static int
xmlSchemaCompareNormStrings(const xmlChar *x,
			    const xmlChar *y) {
    int tmp;
    
    while (IS_BLANK_CH(*x)) x++;
    while (IS_BLANK_CH(*y)) y++;
    while ((*x != 0) && (*y != 0)) {
	if (IS_BLANK_CH(*x)) {
	    if (!IS_BLANK_CH(*y)) {
		tmp = *x - *y;
		return(tmp);
	    }
	    while (IS_BLANK_CH(*x)) x++;
	    while (IS_BLANK_CH(*y)) y++;
	} else {
	    tmp = *x++ - *y++;
	    if (tmp < 0)
		return(-1);
	    if (tmp > 0)
		return(1);
	}
    }
    if (*x != 0) {
	while (IS_BLANK_CH(*x)) x++;
	if (*x != 0)
	    return(1);
    }
    if (*y != 0) {
	while (IS_BLANK_CH(*y)) y++;
	if (*y != 0)
	    return(-1);
    }
    return(0);
}

static int
xmlSchemaCompareFloats(xmlSchemaValPtr x, xmlSchemaValPtr y) {
    double d1, d2;

    if ((x == NULL) || (y == NULL))
	return(-2);

    if (x->type == XML_SCHEMAS_DOUBLE)
	d1 = x->value.d;
    else if (x->type == XML_SCHEMAS_FLOAT)
	d1 = x->value.f;
    else
	return(-2);

    if (y->type == XML_SCHEMAS_DOUBLE)
	d2 = y->value.d;
    else if (y->type == XML_SCHEMAS_FLOAT)
	d2 = y->value.f;
    else
	return(-2);

    if (xmlXPathIsNaN(d1)) {
	if (xmlXPathIsNaN(d2))
	    return(0);
	return(1);
    }
    if (xmlXPathIsNaN(d2))
	return(-1);
    if (d1 == xmlXPathPINF) {
	if (d2 == xmlXPathPINF)
	    return(0);
        return(1);
    }
    if (d2 == xmlXPathPINF)
        return(-1);
    if (d1 == xmlXPathNINF) {
	if (d2 == xmlXPathNINF)
	    return(0);
        return(-1);
    }
    if (d2 == xmlXPathNINF)
        return(1);

    if (d1 < d2)
	return(-1);
    if (d1 > d2)
	return(1);
    if (d1 == d2)
	return(0);
    return(2);
}

static int
xmlSchemaCompareValuesInternal(xmlSchemaValType xtype,
			       xmlSchemaValPtr x,
			       const xmlChar *xvalue,
			       xmlSchemaWhitespaceValueType xws,
			       xmlSchemaValType ytype,
			       xmlSchemaValPtr y,
			       const xmlChar *yvalue,
			       xmlSchemaWhitespaceValueType yws)
{
    switch (xtype) {
	case XML_SCHEMAS_UNKNOWN:
	case XML_SCHEMAS_ANYTYPE:
	    return(-2);
        case XML_SCHEMAS_INTEGER:
        case XML_SCHEMAS_NPINTEGER:
        case XML_SCHEMAS_NINTEGER:
        case XML_SCHEMAS_NNINTEGER:
        case XML_SCHEMAS_PINTEGER:
        case XML_SCHEMAS_INT:
        case XML_SCHEMAS_UINT:
        case XML_SCHEMAS_LONG:
        case XML_SCHEMAS_ULONG:
        case XML_SCHEMAS_SHORT:
        case XML_SCHEMAS_USHORT:
        case XML_SCHEMAS_BYTE:
        case XML_SCHEMAS_UBYTE:
	case XML_SCHEMAS_DECIMAL:
	    if ((x == NULL) || (y == NULL))
		return(-2);
	    if (ytype == xtype)
		return(xmlSchemaCompareDecimals(x, y));
	    if ((ytype == XML_SCHEMAS_DECIMAL) ||
		(ytype == XML_SCHEMAS_INTEGER) ||
		(ytype == XML_SCHEMAS_NPINTEGER) ||
		(ytype == XML_SCHEMAS_NINTEGER) ||
		(ytype == XML_SCHEMAS_NNINTEGER) ||
		(ytype == XML_SCHEMAS_PINTEGER) ||
		(ytype == XML_SCHEMAS_INT) ||
		(ytype == XML_SCHEMAS_UINT) ||
		(ytype == XML_SCHEMAS_LONG) ||
		(ytype == XML_SCHEMAS_ULONG) ||
		(ytype == XML_SCHEMAS_SHORT) ||
		(ytype == XML_SCHEMAS_USHORT) ||
		(ytype == XML_SCHEMAS_BYTE) ||
		(ytype == XML_SCHEMAS_UBYTE))
		return(xmlSchemaCompareDecimals(x, y));
	    return(-2);
        case XML_SCHEMAS_DURATION:
	    if ((x == NULL) || (y == NULL))
		return(-2);
	    if (ytype == XML_SCHEMAS_DURATION)
                return(xmlSchemaCompareDurations(x, y));
            return(-2);
        case XML_SCHEMAS_TIME:
        case XML_SCHEMAS_GDAY:
        case XML_SCHEMAS_GMONTH:
        case XML_SCHEMAS_GMONTHDAY:
        case XML_SCHEMAS_GYEAR:
        case XML_SCHEMAS_GYEARMONTH:
        case XML_SCHEMAS_DATE:
        case XML_SCHEMAS_DATETIME:
	    if ((x == NULL) || (y == NULL))
		return(-2);
            if ((ytype == XML_SCHEMAS_DATETIME)  ||
                (ytype == XML_SCHEMAS_TIME)      ||
                (ytype == XML_SCHEMAS_GDAY)      ||
                (ytype == XML_SCHEMAS_GMONTH)    ||
                (ytype == XML_SCHEMAS_GMONTHDAY) ||
                (ytype == XML_SCHEMAS_GYEAR)     ||
                (ytype == XML_SCHEMAS_DATE)      ||
                (ytype == XML_SCHEMAS_GYEARMONTH))
                return (xmlSchemaCompareDates(x, y));
            return (-2);
	case XML_SCHEMAS_ANYSIMPLETYPE:
	case XML_SCHEMAS_STRING:
        case XML_SCHEMAS_NORMSTRING:		
        case XML_SCHEMAS_TOKEN:
        case XML_SCHEMAS_LANGUAGE:
        case XML_SCHEMAS_NMTOKEN:
        case XML_SCHEMAS_NAME:
        case XML_SCHEMAS_NCNAME:
        case XML_SCHEMAS_ID:
        case XML_SCHEMAS_IDREF:
        case XML_SCHEMAS_ENTITY:
        case XML_SCHEMAS_ANYURI:
	{
	    const xmlChar *xv, *yv;

	    if (x == NULL)
		xv = xvalue;
	    else
		xv = x->value.str;
	    if (y == NULL)
		yv = yvalue;
	    else
		yv = y->value.str;
	    if (ytype == XML_SCHEMAS_QNAME) {		
		TODO
		if (y == NULL)
		    return(-2);    
		return (-2);
	    }
            if ((ytype == XML_SCHEMAS_ANYSIMPLETYPE) ||
		(ytype == XML_SCHEMAS_STRING) ||
		(ytype == XML_SCHEMAS_NORMSTRING) ||
                (ytype == XML_SCHEMAS_TOKEN) ||
                (ytype == XML_SCHEMAS_LANGUAGE) ||
                (ytype == XML_SCHEMAS_NMTOKEN) ||
                (ytype == XML_SCHEMAS_NAME) ||
                (ytype == XML_SCHEMAS_NCNAME) ||
                (ytype == XML_SCHEMAS_ID) ||
                (ytype == XML_SCHEMAS_IDREF) ||
                (ytype == XML_SCHEMAS_ENTITY) ||
                (ytype == XML_SCHEMAS_ANYURI)) {

		if (xws == XML_SCHEMA_WHITESPACE_PRESERVE) {

		    if (yws == XML_SCHEMA_WHITESPACE_PRESERVE) {
			
			if (xmlStrEqual(xv, yv))
			    return (0);
			else 
			    return (2);
		    } else if (yws == XML_SCHEMA_WHITESPACE_REPLACE)
			return (xmlSchemaComparePreserveReplaceStrings(xv, yv, 0));
		    else if (yws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			return (xmlSchemaComparePreserveCollapseStrings(xv, yv, 0));

		} else if (xws == XML_SCHEMA_WHITESPACE_REPLACE) {

		    if (yws == XML_SCHEMA_WHITESPACE_PRESERVE)
			return (xmlSchemaComparePreserveReplaceStrings(yv, xv, 1));
		    if (yws == XML_SCHEMA_WHITESPACE_REPLACE)
			return (xmlSchemaCompareReplacedStrings(xv, yv));
		    if (yws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			return (xmlSchemaCompareReplaceCollapseStrings(xv, yv, 0));

		} else if (xws == XML_SCHEMA_WHITESPACE_COLLAPSE) {

		    if (yws == XML_SCHEMA_WHITESPACE_PRESERVE)
			return (xmlSchemaComparePreserveCollapseStrings(yv, xv, 1));
		    if (yws == XML_SCHEMA_WHITESPACE_REPLACE)
			return (xmlSchemaCompareReplaceCollapseStrings(yv, xv, 1));
		    if (yws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			return (xmlSchemaCompareNormStrings(xv, yv));
		} else
		    return (-2);
                
	    }
            return (-2);
	}
        case XML_SCHEMAS_QNAME:
	case XML_SCHEMAS_NOTATION:
	    if ((x == NULL) || (y == NULL))
		return(-2);
            if ((ytype == XML_SCHEMAS_QNAME) ||
		(ytype == XML_SCHEMAS_NOTATION)) {
		if ((xmlStrEqual(x->value.qname.name, y->value.qname.name)) &&
		    (xmlStrEqual(x->value.qname.uri, y->value.qname.uri)))
		    return(0);
		return(2);
	    }
	    return (-2);
        case XML_SCHEMAS_FLOAT:
        case XML_SCHEMAS_DOUBLE:
	    if ((x == NULL) || (y == NULL))
		return(-2);
            if ((ytype == XML_SCHEMAS_FLOAT) ||
                (ytype == XML_SCHEMAS_DOUBLE))
                return (xmlSchemaCompareFloats(x, y));
            return (-2);
        case XML_SCHEMAS_BOOLEAN:
	    if ((x == NULL) || (y == NULL))
		return(-2);
            if (ytype == XML_SCHEMAS_BOOLEAN) {
		if (x->value.b == y->value.b)
		    return(0);
		if (x->value.b == 0)
		    return(-1);
		return(1);
	    }
	    return (-2);
        case XML_SCHEMAS_HEXBINARY:
	    if ((x == NULL) || (y == NULL))
		return(-2);
            if (ytype == XML_SCHEMAS_HEXBINARY) {
	        if (x->value.hex.total == y->value.hex.total) {
		    int ret = xmlStrcmp(x->value.hex.str, y->value.hex.str);
		    if (ret > 0)
			return(1);
		    else if (ret == 0)
			return(0);
		}
		else if (x->value.hex.total > y->value.hex.total)
		    return(1);

		return(-1);
            }
            return (-2);
        case XML_SCHEMAS_BASE64BINARY:
	    if ((x == NULL) || (y == NULL))
		return(-2);
            if (ytype == XML_SCHEMAS_BASE64BINARY) {
                if (x->value.base64.total == y->value.base64.total) {
                    int ret = xmlStrcmp(x->value.base64.str,
		                        y->value.base64.str);
                    if (ret > 0)
                        return(1);
                    else if (ret == 0)
                        return(0);
		    else
		        return(-1);
                }
                else if (x->value.base64.total > y->value.base64.total)
                    return(1);
                else
                    return(-1);
            }
            return (-2);    
        case XML_SCHEMAS_IDREFS:
        case XML_SCHEMAS_ENTITIES:
        case XML_SCHEMAS_NMTOKENS:
	    TODO
	    break;
    }
    return -2;
}

int
xmlSchemaCompareValues(xmlSchemaValPtr x, xmlSchemaValPtr y) {
    xmlSchemaWhitespaceValueType xws, yws;

    if ((x == NULL) || (y == NULL))
        return(-2);
    if (x->type == XML_SCHEMAS_STRING)
	xws = XML_SCHEMA_WHITESPACE_PRESERVE;
    else if (x->type == XML_SCHEMAS_NORMSTRING)
        xws = XML_SCHEMA_WHITESPACE_REPLACE;
    else
        xws = XML_SCHEMA_WHITESPACE_COLLAPSE;

    if (y->type == XML_SCHEMAS_STRING)
	yws = XML_SCHEMA_WHITESPACE_PRESERVE;
    else if (x->type == XML_SCHEMAS_NORMSTRING)
        yws = XML_SCHEMA_WHITESPACE_REPLACE;
    else
        yws = XML_SCHEMA_WHITESPACE_COLLAPSE;

    return(xmlSchemaCompareValuesInternal(x->type, x, NULL, xws, y->type,
	y, NULL, yws));
}

int
xmlSchemaCompareValuesWhtsp(xmlSchemaValPtr x,
			    xmlSchemaWhitespaceValueType xws,
			    xmlSchemaValPtr y,
			    xmlSchemaWhitespaceValueType yws)
{
    if ((x == NULL) || (y == NULL))
	return(-2);
    return(xmlSchemaCompareValuesInternal(x->type, x, NULL, xws, y->type,
	y, NULL, yws));
}

static int
xmlSchemaCompareValuesWhtspExt(xmlSchemaValType xtype,
			       xmlSchemaValPtr x,
			       const xmlChar *xvalue,
			       xmlSchemaWhitespaceValueType xws,
			       xmlSchemaValType ytype,
			       xmlSchemaValPtr y,
			       const xmlChar *yvalue,
			       xmlSchemaWhitespaceValueType yws)
{
    return(xmlSchemaCompareValuesInternal(xtype, x, xvalue, xws, ytype, y,
	yvalue, yws));
}

static int
xmlSchemaNormLen(const xmlChar *value) {
    const xmlChar *utf;
    int ret = 0;

    if (value == NULL)
	return(-1);
    utf = value;
    while (IS_BLANK_CH(*utf)) utf++;
    while (*utf != 0) {
	if (utf[0] & 0x80) {
	    if ((utf[1] & 0xc0) != 0x80)
		return(-1);
	    if ((utf[0] & 0xe0) == 0xe0) {
		if ((utf[2] & 0xc0) != 0x80)
		    return(-1);
		if ((utf[0] & 0xf0) == 0xf0) {
		    if ((utf[0] & 0xf8) != 0xf0 || (utf[3] & 0xc0) != 0x80)
			return(-1);
		    utf += 4;
		} else {
		    utf += 3;
		}
	    } else {
		utf += 2;
	    }
	} else if (IS_BLANK_CH(*utf)) {
	    while (IS_BLANK_CH(*utf)) utf++;
	    if (*utf == 0)
		break;
	} else {
	    utf++;
	}
	ret++;
    }
    return(ret);
}

unsigned long
xmlSchemaGetFacetValueAsULong(xmlSchemaFacetPtr facet)
{
    if (facet == NULL)
        return 0;
    return ((unsigned long) facet->val->value.decimal.lo);
}

int
xmlSchemaValidateListSimpleTypeFacet(xmlSchemaFacetPtr facet,
				     const xmlChar *value,
				     unsigned long actualLen,
				     unsigned long *expectedLen)
{
    if (facet == NULL)
        return(-1);
    if (facet->type == XML_SCHEMA_FACET_LENGTH) {
	if (actualLen != facet->val->value.decimal.lo) {
	    if (expectedLen != NULL)
		*expectedLen = facet->val->value.decimal.lo;
	    return (XML_SCHEMAV_CVC_LENGTH_VALID);
	}	
    } else if (facet->type == XML_SCHEMA_FACET_MINLENGTH) {
	if (actualLen < facet->val->value.decimal.lo) {
	    if (expectedLen != NULL)
		*expectedLen = facet->val->value.decimal.lo;
	    return (XML_SCHEMAV_CVC_MINLENGTH_VALID);
	}
    } else if (facet->type == XML_SCHEMA_FACET_MAXLENGTH) {
	if (actualLen > facet->val->value.decimal.lo) {
	    if (expectedLen != NULL)
		*expectedLen = facet->val->value.decimal.lo;
	    return (XML_SCHEMAV_CVC_MAXLENGTH_VALID);
	}
    } else
	return(xmlSchemaValidateFacet(NULL, facet, value, NULL));   
    return (0);
}

static int
xmlSchemaValidateLengthFacetInternal(xmlSchemaFacetPtr facet,
				     xmlSchemaValType valType,
				     const xmlChar *value,
				     xmlSchemaValPtr val,
				     unsigned long *length,
				     xmlSchemaWhitespaceValueType ws)  
{
    unsigned int len = 0;

    if ((length == NULL) || (facet == NULL))
        return (-1);
    *length = 0;
    if ((facet->type != XML_SCHEMA_FACET_LENGTH) &&
	(facet->type != XML_SCHEMA_FACET_MAXLENGTH) &&
	(facet->type != XML_SCHEMA_FACET_MINLENGTH))
	return (-1);
	
    if ((facet->val == NULL) ||
	((facet->val->type != XML_SCHEMAS_DECIMAL) &&
	 (facet->val->type != XML_SCHEMAS_NNINTEGER)) ||
	(facet->val->value.decimal.frac != 0)) {
	return(-1);
    }
    if ((val != NULL) && (val->type == XML_SCHEMAS_HEXBINARY))
	len = val->value.hex.total;
    else if ((val != NULL) && (val->type == XML_SCHEMAS_BASE64BINARY))
	len = val->value.base64.total;
    else {
	switch (valType) {
	    case XML_SCHEMAS_STRING:
	    case XML_SCHEMAS_NORMSTRING:
		if (ws == XML_SCHEMA_WHITESPACE_UNKNOWN) {
		    if (valType == XML_SCHEMAS_STRING)
			len = xmlUTF8Strlen(value);
		    else
			len = xmlSchemaNormLen(value);
		} else if (value != NULL) {
		    if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
			len = xmlSchemaNormLen(value);
		    else
		    len = xmlUTF8Strlen(value);
		}
		break;
	    case XML_SCHEMAS_IDREF:
	    case XML_SCHEMAS_TOKEN:
	    case XML_SCHEMAS_LANGUAGE:
	    case XML_SCHEMAS_NMTOKEN:
	    case XML_SCHEMAS_NAME:
	    case XML_SCHEMAS_NCNAME:
	    case XML_SCHEMAS_ID:		
	    case XML_SCHEMAS_ANYURI:
		if (value != NULL)
		    len = xmlSchemaNormLen(value);
		break;
	    case XML_SCHEMAS_QNAME:
 	    case XML_SCHEMAS_NOTATION:
		return (0);
	    default:
		TODO
	}
    }
    *length = (unsigned long) len;
    if (facet->type == XML_SCHEMA_FACET_LENGTH) {
	if (len != facet->val->value.decimal.lo)
	    return(XML_SCHEMAV_CVC_LENGTH_VALID);
    } else if (facet->type == XML_SCHEMA_FACET_MINLENGTH) {
	if (len < facet->val->value.decimal.lo)
	    return(XML_SCHEMAV_CVC_MINLENGTH_VALID);
    } else {
	if (len > facet->val->value.decimal.lo)
	    return(XML_SCHEMAV_CVC_MAXLENGTH_VALID);
    }
    
    return (0);
}

int
xmlSchemaValidateLengthFacet(xmlSchemaTypePtr type, 
			     xmlSchemaFacetPtr facet,
			     const xmlChar *value,
			     xmlSchemaValPtr val,
			     unsigned long *length)  
{
    if (type == NULL)
        return(-1);
    return (xmlSchemaValidateLengthFacetInternal(facet,
	type->builtInType, value, val, length,
	XML_SCHEMA_WHITESPACE_UNKNOWN));
}

int
xmlSchemaValidateLengthFacetWhtsp(xmlSchemaFacetPtr facet,
				  xmlSchemaValType valType,
				  const xmlChar *value,
				  xmlSchemaValPtr val,
				  unsigned long *length,
				  xmlSchemaWhitespaceValueType ws)
{
    return (xmlSchemaValidateLengthFacetInternal(facet, valType, value, val,
	length, ws));
}

static int
xmlSchemaValidateFacetInternal(xmlSchemaFacetPtr facet,
			       xmlSchemaWhitespaceValueType fws,
			       xmlSchemaValType valType,			       
			       const xmlChar *value,
			       xmlSchemaValPtr val,
			       xmlSchemaWhitespaceValueType ws)
{
    int ret;

    if (facet == NULL)
	return(-1);

    switch (facet->type) {
	case XML_SCHEMA_FACET_PATTERN:
	    if (value == NULL)
		return(-1);
	    ret = xmlRegexpExec(facet->regexp, value);
	    if (ret == 1)
		return(0);
	    if (ret == 0)
		return(XML_SCHEMAV_CVC_PATTERN_VALID);
	    return(ret);
	case XML_SCHEMA_FACET_MAXEXCLUSIVE:
	    ret = xmlSchemaCompareValues(val, facet->val);
	    if (ret == -2)
		return(-1);
	    if (ret == -1)
		return(0);
	    return(XML_SCHEMAV_CVC_MAXEXCLUSIVE_VALID);
	case XML_SCHEMA_FACET_MAXINCLUSIVE:
	    ret = xmlSchemaCompareValues(val, facet->val);
	    if (ret == -2)
		return(-1);
	    if ((ret == -1) || (ret == 0))
		return(0);
	    return(XML_SCHEMAV_CVC_MAXINCLUSIVE_VALID);
	case XML_SCHEMA_FACET_MINEXCLUSIVE:
	    ret = xmlSchemaCompareValues(val, facet->val);
	    if (ret == -2)
		return(-1);
	    if (ret == 1)
		return(0);
	    return(XML_SCHEMAV_CVC_MINEXCLUSIVE_VALID);
	case XML_SCHEMA_FACET_MININCLUSIVE:
	    ret = xmlSchemaCompareValues(val, facet->val);
	    if (ret == -2)
		return(-1);
	    if ((ret == 1) || (ret == 0))
		return(0);
	    return(XML_SCHEMAV_CVC_MININCLUSIVE_VALID);
	case XML_SCHEMA_FACET_WHITESPACE:
	    
	    return(0);
	case  XML_SCHEMA_FACET_ENUMERATION:
	    if (ws == XML_SCHEMA_WHITESPACE_UNKNOWN) {
		if ((facet->value != NULL) &&
		    (xmlStrEqual(facet->value, value)))
		    return(0);
	    } else {
		ret = xmlSchemaCompareValuesWhtspExt(facet->val->type,
		    facet->val, facet->value, fws, valType, val,
		    value, ws);
		if (ret == -2)
		    return(-1);
		if (ret == 0)
		    return(0);
	    }
	    return(XML_SCHEMAV_CVC_ENUMERATION_VALID);
	case XML_SCHEMA_FACET_LENGTH:
	    if ((valType == XML_SCHEMAS_QNAME) ||
		(valType == XML_SCHEMAS_NOTATION))
		return (0);
	    
	case XML_SCHEMA_FACET_MAXLENGTH:
	case XML_SCHEMA_FACET_MINLENGTH: {
	    unsigned int len = 0;

	    if ((valType == XML_SCHEMAS_QNAME) ||
		(valType == XML_SCHEMAS_NOTATION))
		return (0);
	    if ((facet->val == NULL) ||
		((facet->val->type != XML_SCHEMAS_DECIMAL) &&
		 (facet->val->type != XML_SCHEMAS_NNINTEGER)) ||
		(facet->val->value.decimal.frac != 0)) {
		return(-1);
	    }
	    if ((val != NULL) && (val->type == XML_SCHEMAS_HEXBINARY))
		len = val->value.hex.total;
	    else if ((val != NULL) && (val->type == XML_SCHEMAS_BASE64BINARY))
		len = val->value.base64.total;
	    else {
		switch (valType) {
		    case XML_SCHEMAS_STRING:
		    case XML_SCHEMAS_NORMSTRING:			
			if (ws == XML_SCHEMA_WHITESPACE_UNKNOWN) {
			    if (valType == XML_SCHEMAS_STRING)
				len = xmlUTF8Strlen(value);
			    else
				len = xmlSchemaNormLen(value);
			} else if (value != NULL) {
			    if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
				len = xmlSchemaNormLen(value);
			    else
				len = xmlUTF8Strlen(value);
			}
			break;
	    	    case XML_SCHEMAS_IDREF:		    
		    case XML_SCHEMAS_TOKEN:
		    case XML_SCHEMAS_LANGUAGE:
		    case XML_SCHEMAS_NMTOKEN:
		    case XML_SCHEMAS_NAME:
		    case XML_SCHEMAS_NCNAME:
		    case XML_SCHEMAS_ID:
		    case XML_SCHEMAS_ANYURI:
			if (value != NULL)
		    	    len = xmlSchemaNormLen(value);
		    	break;		   
		    default:
		        TODO
	    	}
	    }
	    if (facet->type == XML_SCHEMA_FACET_LENGTH) {
		if (len != facet->val->value.decimal.lo)
		    return(XML_SCHEMAV_CVC_LENGTH_VALID);
	    } else if (facet->type == XML_SCHEMA_FACET_MINLENGTH) {
		if (len < facet->val->value.decimal.lo)
		    return(XML_SCHEMAV_CVC_MINLENGTH_VALID);
	    } else {
		if (len > facet->val->value.decimal.lo)
		    return(XML_SCHEMAV_CVC_MAXLENGTH_VALID);
	    }
	    break;
	}
	case XML_SCHEMA_FACET_TOTALDIGITS:
	case XML_SCHEMA_FACET_FRACTIONDIGITS:

	    if ((facet->val == NULL) ||
		((facet->val->type != XML_SCHEMAS_PINTEGER) &&
		 (facet->val->type != XML_SCHEMAS_NNINTEGER)) ||
		(facet->val->value.decimal.frac != 0)) {
		return(-1);
	    }
	    if ((val == NULL) ||
		((val->type != XML_SCHEMAS_DECIMAL) &&
		 (val->type != XML_SCHEMAS_INTEGER) &&
		 (val->type != XML_SCHEMAS_NPINTEGER) &&
		 (val->type != XML_SCHEMAS_NINTEGER) &&
		 (val->type != XML_SCHEMAS_NNINTEGER) &&
		 (val->type != XML_SCHEMAS_PINTEGER) &&
		 (val->type != XML_SCHEMAS_INT) &&
		 (val->type != XML_SCHEMAS_UINT) &&
		 (val->type != XML_SCHEMAS_LONG) &&
		 (val->type != XML_SCHEMAS_ULONG) &&
		 (val->type != XML_SCHEMAS_SHORT) &&
		 (val->type != XML_SCHEMAS_USHORT) &&
		 (val->type != XML_SCHEMAS_BYTE) &&
		 (val->type != XML_SCHEMAS_UBYTE))) {
		return(-1);
	    }
	    if (facet->type == XML_SCHEMA_FACET_TOTALDIGITS) {
	        if (val->value.decimal.total > facet->val->value.decimal.lo)
	            return(XML_SCHEMAV_CVC_TOTALDIGITS_VALID);

	    } else if (facet->type == XML_SCHEMA_FACET_FRACTIONDIGITS) {
	        if (val->value.decimal.frac > facet->val->value.decimal.lo)
		    return(XML_SCHEMAV_CVC_FRACTIONDIGITS_VALID);
	    }
	    break;
	default:
	    TODO
    }
    return(0);

}

int
xmlSchemaValidateFacet(xmlSchemaTypePtr base,
	               xmlSchemaFacetPtr facet,
	               const xmlChar *value,
		       xmlSchemaValPtr val)
{
    if (val != NULL)
	return(xmlSchemaValidateFacetInternal(facet,
	    XML_SCHEMA_WHITESPACE_UNKNOWN, val->type, value, val,
	    XML_SCHEMA_WHITESPACE_UNKNOWN));
    else if (base != NULL)
	return(xmlSchemaValidateFacetInternal(facet,
	    XML_SCHEMA_WHITESPACE_UNKNOWN, base->builtInType, value, val,
	    XML_SCHEMA_WHITESPACE_UNKNOWN));
    return(-1);
}

int
xmlSchemaValidateFacetWhtsp(xmlSchemaFacetPtr facet,
			    xmlSchemaWhitespaceValueType fws,
			    xmlSchemaValType valType,			    
			    const xmlChar *value,
			    xmlSchemaValPtr val,
			    xmlSchemaWhitespaceValueType ws)
{
     return(xmlSchemaValidateFacetInternal(facet, fws, valType,
	 value, val, ws));
}

#if 0
#ifndef DBL_DIG
#define DBL_DIG 16
#endif
#ifndef DBL_EPSILON
#define DBL_EPSILON 1E-9
#endif

#define INTEGER_DIGITS DBL_DIG
#define FRACTION_DIGITS (DBL_DIG + 1)
#define EXPONENT_DIGITS (3 + 2)

static void
xmlSchemaFormatFloat(double number, char buffer[], int buffersize)
{
    switch (xmlXPathIsInf(number)) {
    case 1:
	if (buffersize > (int)sizeof("INF"))
	    snprintf(buffer, buffersize, "INF");
	break;
    case -1:
	if (buffersize > (int)sizeof("-INF"))
	    snprintf(buffer, buffersize, "-INF");
	break;
    default:
	if (xmlXPathIsNaN(number)) {
	    if (buffersize > (int)sizeof("NaN"))
		snprintf(buffer, buffersize, "NaN");
	} else if (number == 0) {
	    snprintf(buffer, buffersize, "0.0E0");
	} else {
	    
	    char work[DBL_DIG + EXPONENT_DIGITS + 3];
	    int integer_place, fraction_place;
	    char *ptr;
	    char *after_fraction;
	    double absolute_value;
	    int size;

	    absolute_value = fabs(number);

	    integer_place = DBL_DIG + EXPONENT_DIGITS + 1;
	    fraction_place = DBL_DIG - 1;
	    snprintf(work, sizeof(work),"%*.*e",
		integer_place, fraction_place, number);
	    after_fraction = strchr(work + DBL_DIG, 'e');	    
	    
	    ptr = after_fraction;
	    while (*(--ptr) == '0')
		;
	    if (*ptr != '.')
	        ptr++;
	    while ((*ptr++ = *after_fraction++) != 0);

	    
	    size = strlen(work) + 1;
	    if (size > buffersize) {
		work[buffersize - 1] = 0;
		size = buffersize;
	    }
	    memmove(buffer, work, size);
	}
	break;
    }
}
#endif

int
xmlSchemaGetCanonValue(xmlSchemaValPtr val, const xmlChar **retValue)
{
    if ((retValue == NULL) || (val == NULL))
	return (-1);
    *retValue = NULL;
    switch (val->type) {
	case XML_SCHEMAS_STRING:
	    if (val->value.str == NULL)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "");
	    else
		*retValue = 
		    BAD_CAST xmlStrdup((const xmlChar *) val->value.str);
	    break;
	case XML_SCHEMAS_NORMSTRING:
	    if (val->value.str == NULL)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "");
	    else {
		*retValue = xmlSchemaWhiteSpaceReplace(
		    (const xmlChar *) val->value.str);
		if ((*retValue) == NULL)
		    *retValue = BAD_CAST xmlStrdup(
			(const xmlChar *) val->value.str);
	    }
	    break;
	case XML_SCHEMAS_TOKEN:
	case XML_SCHEMAS_LANGUAGE:
	case XML_SCHEMAS_NMTOKEN:
	case XML_SCHEMAS_NAME:	
	case XML_SCHEMAS_NCNAME:
	case XML_SCHEMAS_ID:
	case XML_SCHEMAS_IDREF:
	case XML_SCHEMAS_ENTITY:
	case XML_SCHEMAS_NOTATION: 
	case XML_SCHEMAS_ANYURI:   
	    if (val->value.str == NULL)
		return (-1);
	    *retValue = 
		BAD_CAST xmlSchemaCollapseString(BAD_CAST val->value.str);
	    if (*retValue == NULL)
		*retValue = 
		    BAD_CAST xmlStrdup((const xmlChar *) val->value.str);
	    break;
	case XML_SCHEMAS_QNAME:
	    
	    if (val->value.qname.uri == NULL) {
		*retValue = BAD_CAST xmlStrdup(BAD_CAST val->value.qname.name);
		return (0);
	    } else {
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "{");
		*retValue = BAD_CAST xmlStrcat((xmlChar *) (*retValue),
		    BAD_CAST val->value.qname.uri);
		*retValue = BAD_CAST xmlStrcat((xmlChar *) (*retValue),
		    BAD_CAST "}");
		*retValue = BAD_CAST xmlStrcat((xmlChar *) (*retValue),
		    BAD_CAST val->value.qname.uri);
	    }
	    break;
	case XML_SCHEMAS_DECIMAL:
	    if ((val->value.decimal.total == 1) && 
		(val->value.decimal.lo == 0)) {
		*retValue = xmlStrdup(BAD_CAST "0.0");
	    } else {
		xmlSchemaValDecimal dec = val->value.decimal;
		int bufsize;
		char *buf = NULL, *offs;

		
		bufsize = dec.total + 2;
		if (dec.sign)
		    bufsize++;
		
		if ((dec.frac == 0) || (dec.frac == dec.total))
		    bufsize++;
		buf = xmlMalloc(bufsize);
		if (buf == NULL)
		    return(-1);
		offs = buf;
		if (dec.sign)
		    *offs++ = '-';
		if (dec.frac == dec.total) {
		    *offs++ = '0';
		    *offs++ = '.';
		}
		if (dec.hi != 0)
		    snprintf(offs, bufsize - (offs - buf),
			"%lu%lu%lu", dec.hi, dec.mi, dec.lo);
		else if (dec.mi != 0)
		    snprintf(offs, bufsize - (offs - buf),
			"%lu%lu", dec.mi, dec.lo);
		else
		    snprintf(offs, bufsize - (offs - buf),
			"%lu", dec.lo);
			
		if (dec.frac != 0) {
		    if (dec.frac != dec.total) {
			int diff = dec.total - dec.frac;
			memmove(offs + diff + 1, offs + diff, dec.frac +1);
			offs[diff] = '.';
		    } else {
			unsigned int i = 0;
			
			while (*(offs + i) != 0)
			    i++;
			if (i < dec.total) {
			    memmove(offs + (dec.total - i), offs, i +1);
			    memset(offs, '0', dec.total - i);
			}
		    }
		} else {
		    offs = buf + bufsize - 1;
		    *offs-- = 0;
		    *offs-- = '0';
		    *offs-- = '.';
		}
		*retValue = BAD_CAST buf;
	    }
	    break;
	case XML_SCHEMAS_INTEGER:
        case XML_SCHEMAS_PINTEGER:
        case XML_SCHEMAS_NPINTEGER:
        case XML_SCHEMAS_NINTEGER:
        case XML_SCHEMAS_NNINTEGER:
	case XML_SCHEMAS_LONG:
        case XML_SCHEMAS_BYTE:
        case XML_SCHEMAS_SHORT:
        case XML_SCHEMAS_INT:
	case XML_SCHEMAS_UINT:
        case XML_SCHEMAS_ULONG:
        case XML_SCHEMAS_USHORT:
        case XML_SCHEMAS_UBYTE:
	    if ((val->value.decimal.total == 1) &&
		(val->value.decimal.lo == 0))
		*retValue = xmlStrdup(BAD_CAST "0");
	    else {
		xmlSchemaValDecimal dec = val->value.decimal;
		int bufsize = dec.total + 1;

		
		if (dec.sign)
		    bufsize++;
		*retValue = xmlMalloc(bufsize);
		if (*retValue == NULL)
		    return(-1);
		if (dec.hi != 0) {
		    if (dec.sign)
			snprintf((char *) *retValue, bufsize,
			    "-%lu%lu%lu", dec.hi, dec.mi, dec.lo);
		    else
			snprintf((char *) *retValue, bufsize,
			    "%lu%lu%lu", dec.hi, dec.mi, dec.lo);
		} else if (dec.mi != 0) {
		    if (dec.sign)
			snprintf((char *) *retValue, bufsize,
			    "-%lu%lu", dec.mi, dec.lo);
		    else
			snprintf((char *) *retValue, bufsize,
			    "%lu%lu", dec.mi, dec.lo);
		} else {
		    if (dec.sign)
			snprintf((char *) *retValue, bufsize, "-%lu", dec.lo);
		    else
			snprintf((char *) *retValue, bufsize, "%lu", dec.lo);
		}
	    }
	    break;
	case XML_SCHEMAS_BOOLEAN:
	    if (val->value.b)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "true");
	    else
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "false");
	    break;
	case XML_SCHEMAS_DURATION: {
		char buf[100];
		unsigned long year;
		unsigned long mon, day, hour = 0, min = 0;
		double sec = 0, left;

		
		year = (unsigned long) FQUOTIENT(labs(val->value.dur.mon), 12);
		mon = labs(val->value.dur.mon) - 12 * year;

		day = (unsigned long) FQUOTIENT(fabs(val->value.dur.sec), 86400);
		left = fabs(val->value.dur.sec) - day * 86400;
		if (left > 0) {
		    hour = (unsigned long) FQUOTIENT(left, 3600);
		    left = left - (hour * 3600);
		    if (left > 0) {
			min = (unsigned long) FQUOTIENT(left, 60);
			sec = left - (min * 60);
		    }
		}
		if ((val->value.dur.mon < 0) || (val->value.dur.sec < 0))
		    snprintf(buf, 100, "P%luY%luM%luDT%luH%luM%.14gS",
			year, mon, day, hour, min, sec);
		else
		    snprintf(buf, 100, "-P%luY%luM%luDT%luH%luM%.14gS",
			year, mon, day, hour, min, sec);
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }
	    break;
	case XML_SCHEMAS_GYEAR: {
		char buf[30];
		
		
		snprintf(buf, 30, "%04ld", val->value.date.year);
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }
	    break;
	case XML_SCHEMAS_GMONTH: {
		
		
		*retValue = xmlMalloc(6);
		if (*retValue == NULL)
		    return(-1);
		snprintf((char *) *retValue, 6, "--%02u",
		    val->value.date.mon);
	    }
	    break;
        case XML_SCHEMAS_GDAY: {
		
		
		*retValue = xmlMalloc(6);
		if (*retValue == NULL)
		    return(-1);
		snprintf((char *) *retValue, 6, "---%02u",
		    val->value.date.day);
	    }
	    break;        
        case XML_SCHEMAS_GMONTHDAY: {
		
		
		*retValue = xmlMalloc(8);
		if (*retValue == NULL)
		    return(-1);
		snprintf((char *) *retValue, 8, "--%02u-%02u",
		    val->value.date.mon, val->value.date.day);
	    }
	    break;
        case XML_SCHEMAS_GYEARMONTH: {
		char buf[35];
		
		
		if (val->value.date.year < 0)
		    snprintf(buf, 35, "-%04ld-%02u",
			labs(val->value.date.year), 
			val->value.date.mon);
		else
		    snprintf(buf, 35, "%04ld-%02u",
			val->value.date.year, val->value.date.mon);
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }
	    break;		
	case XML_SCHEMAS_TIME:
	    {
		char buf[30];

		if (val->value.date.tz_flag) {
		    xmlSchemaValPtr norm;

		    norm = xmlSchemaDateNormalize(val, 0);
		    if (norm == NULL)
			return (-1);
		    snprintf(buf, 30,
			"%02u:%02u:%02.14gZ",
			norm->value.date.hour,
			norm->value.date.min,
			norm->value.date.sec);
		    xmlSchemaFreeValue(norm);
		} else {
		    snprintf(buf, 30,
			"%02u:%02u:%02.14g",
			val->value.date.hour,
			val->value.date.min,
			val->value.date.sec);
		}
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }	    
	    break;
        case XML_SCHEMAS_DATE:
	    {
		char buf[30];

		if (val->value.date.tz_flag) {
		    xmlSchemaValPtr norm;

		    norm = xmlSchemaDateNormalize(val, 0);
		    if (norm == NULL)
			return (-1);
		    snprintf(buf, 30,
			"%04ld:%02u:%02uZ",
			norm->value.date.year, norm->value.date.mon,
			norm->value.date.day);
		    xmlSchemaFreeValue(norm);
		} else {
		    snprintf(buf, 30,
			"%04ld:%02u:%02u",
			val->value.date.year, val->value.date.mon,
			val->value.date.day);
		}
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }	    
	    break;
        case XML_SCHEMAS_DATETIME:
	    {
		char buf[50];

		if (val->value.date.tz_flag) {
		    xmlSchemaValPtr norm;

		    norm = xmlSchemaDateNormalize(val, 0);
		    if (norm == NULL)
			return (-1);
		    snprintf(buf, 50,
			"%04ld:%02u:%02uT%02u:%02u:%02.14gZ",
			norm->value.date.year, norm->value.date.mon,
			norm->value.date.day, norm->value.date.hour,
			norm->value.date.min, norm->value.date.sec);
		    xmlSchemaFreeValue(norm);
		} else {
		    snprintf(buf, 50,
			"%04ld:%02u:%02uT%02u:%02u:%02.14g",
			val->value.date.year, val->value.date.mon,
			val->value.date.day, val->value.date.hour,
			val->value.date.min, val->value.date.sec);
		}
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }
	    break;
	case XML_SCHEMAS_HEXBINARY:
	    *retValue = BAD_CAST xmlStrdup(BAD_CAST val->value.hex.str);
	    break;
	case XML_SCHEMAS_BASE64BINARY:
	    *retValue = BAD_CAST xmlStrdup(BAD_CAST val->value.base64.str);
	    break;
	case XML_SCHEMAS_FLOAT: {
		char buf[30];		
		snprintf(buf, 30, "%01.14e", val->value.f);
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }
	    break;
	case XML_SCHEMAS_DOUBLE: {
		char buf[40];
		
		snprintf(buf, 40, "%01.14e", val->value.d);
		*retValue = BAD_CAST xmlStrdup(BAD_CAST buf);
	    }
	    break;	
	default:
	    *retValue = BAD_CAST xmlStrdup(BAD_CAST "???");
	    return (1);
    }
    if (*retValue == NULL)
	return(-1);
    return (0);
}

int
xmlSchemaGetCanonValueWhtsp(xmlSchemaValPtr val,
			    const xmlChar **retValue,
			    xmlSchemaWhitespaceValueType ws)
{
    if ((retValue == NULL) || (val == NULL))
	return (-1);
    if ((ws == XML_SCHEMA_WHITESPACE_UNKNOWN) ||
	(ws > XML_SCHEMA_WHITESPACE_COLLAPSE))
	return (-1);

    *retValue = NULL;
    switch (val->type) {
	case XML_SCHEMAS_STRING:
	    if (val->value.str == NULL)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "");
	    else if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
		*retValue = xmlSchemaCollapseString(val->value.str);
	    else if (ws == XML_SCHEMA_WHITESPACE_REPLACE)
		*retValue = xmlSchemaWhiteSpaceReplace(val->value.str);
	    if ((*retValue) == NULL)
		*retValue = BAD_CAST xmlStrdup(val->value.str);
	    break;
	case XML_SCHEMAS_NORMSTRING:
	    if (val->value.str == NULL)
		*retValue = BAD_CAST xmlStrdup(BAD_CAST "");
	    else {
		if (ws == XML_SCHEMA_WHITESPACE_COLLAPSE)
		    *retValue = xmlSchemaCollapseString(val->value.str);
		else
		    *retValue = xmlSchemaWhiteSpaceReplace(val->value.str);
		if ((*retValue) == NULL)
		    *retValue = BAD_CAST xmlStrdup(val->value.str);
	    }
	    break;
	default:
	    return (xmlSchemaGetCanonValue(val, retValue));
    }    
    return (0);
}

xmlSchemaValType
xmlSchemaGetValType(xmlSchemaValPtr val)
{
    if (val == NULL)
        return(XML_SCHEMAS_UNKNOWN);
    return (val->type);
}

#define bottom_xmlschemastypes
#include "elfgcchack.h"
#endif 
