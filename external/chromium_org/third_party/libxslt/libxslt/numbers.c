/*
 * numbers.c: Implementation of the XSLT number functions
 *
 * Reference:
 *   http://www.w3.org/TR/1999/REC-xslt-19991116
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 * Bjorn Reese <breese@users.sourceforge.net>
 */

#define IN_LIBXSLT
#include "libxslt.h"

#include <math.h>
#include <limits.h>
#include <float.h>
#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parserInternals.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/encoding.h>
#include "xsltutils.h"
#include "pattern.h"
#include "templates.h"
#include "transform.h"
#include "numbersInternals.h"

#ifndef FALSE
# define FALSE (0 == 1)
# define TRUE (1 == 1)
#endif

#define SYMBOL_QUOTE		((xmlChar)'\'')

#define DEFAULT_TOKEN		(xmlChar)'0'
#define DEFAULT_SEPARATOR	"."

#define MAX_TOKENS		1024

typedef struct _xsltFormatToken xsltFormatToken;
typedef xsltFormatToken *xsltFormatTokenPtr;
struct _xsltFormatToken {
    xmlChar	*separator;
    xmlChar	 token;
    int		 width;
};

typedef struct _xsltFormat xsltFormat;
typedef xsltFormat *xsltFormatPtr;
struct _xsltFormat {
    xmlChar		*start;
    xsltFormatToken	 tokens[MAX_TOKENS];
    int			 nTokens;
    xmlChar		*end;
};

static char alpha_upper_list[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static char alpha_lower_list[] = "abcdefghijklmnopqrstuvwxyz";
static xsltFormatToken default_token;


static int
xsltUTF8Size(xmlChar *utf) {
    xmlChar mask;
    int len;

    if (utf == NULL)
        return -1;
    if (*utf < 0x80)
        return 1;
    
    if (!(*utf & 0x40))
        return -1;
    
    len = 2;
    for (mask=0x20; mask != 0; mask>>=1) {
        if (!(*utf & mask))
            return len;
        len++;
    }
    return -1;
}

static int
xsltUTF8Charcmp(xmlChar *utf1, xmlChar *utf2) {

    if (utf1 == NULL ) {
        if (utf2 == NULL)
            return 0;
        return -1;
    }
    return xmlStrncmp(utf1, utf2, xsltUTF8Size(utf1));
}


#define IS_SPECIAL(self,letter)			\
    ((xsltUTF8Charcmp((letter), (self)->zeroDigit) == 0)	    ||	\
     (xsltUTF8Charcmp((letter), (self)->digit) == 0)	    ||	\
     (xsltUTF8Charcmp((letter), (self)->decimalPoint) == 0)  ||	\
     (xsltUTF8Charcmp((letter), (self)->grouping) == 0)	    ||	\
     (xsltUTF8Charcmp((letter), (self)->patternSeparator) == 0))

#define IS_DIGIT_ZERO(x) xsltIsDigitZero(x)
#define IS_DIGIT_ONE(x) xsltIsDigitZero((xmlChar)(x)-1)

static int
xsltIsDigitZero(unsigned int ch)
{
    switch (ch) {
    case 0x0030: case 0x0660: case 0x06F0: case 0x0966:
    case 0x09E6: case 0x0A66: case 0x0AE6: case 0x0B66:
    case 0x0C66: case 0x0CE6: case 0x0D66: case 0x0E50:
    case 0x0E60: case 0x0F20: case 0x1040: case 0x17E0:
    case 0x1810: case 0xFF10:
	return TRUE;
    default:
	return FALSE;
    }
}

static void
xsltNumberFormatDecimal(xmlBufferPtr buffer,
			double number,
			int digit_zero,
			int width,
			int digitsPerGroup,
			int groupingCharacter,
			int groupingCharacterLen)
{
    xmlChar temp_string[500];
    xmlChar *pointer;
    xmlChar temp_char[6];
    int i;
    int val;
    int len;

    
    pointer = &temp_string[sizeof(temp_string)] - 1;	
    *pointer = 0;
    i = 0;
    while (pointer > temp_string) {
	if ((i >= width) && (fabs(number) < 1.0))
	    break; 
	if ((i > 0) && (groupingCharacter != 0) &&
	    (digitsPerGroup > 0) &&
	    ((i % digitsPerGroup) == 0)) {
	    if (pointer - groupingCharacterLen < temp_string) {
	        i = -1;		
		break;
	    }
	    pointer -= groupingCharacterLen;
	    xmlCopyCharMultiByte(pointer, groupingCharacter);
	}
	
	val = digit_zero + (int)fmod(number, 10.0);
	if (val < 0x80) {			
	    if (pointer <= temp_string) {	
	        i = -1;
		break;
	    }
	    *(--pointer) = val;
	}
	else {
	    len = xmlCopyCharMultiByte(temp_char, val);
	    if ( (pointer - len) < temp_string ) {
	        i = -1;
		break;
	    }
	    pointer -= len;
	    memcpy(pointer, temp_char, len);
	}
	number /= 10.0;
	++i;
    }
    if (i < 0)
        xsltGenericError(xsltGenericErrorContext,
		"xsltNumberFormatDecimal: Internal buffer size exceeded");
    xmlBufferCat(buffer, pointer);
}

static void
xsltNumberFormatAlpha(xmlBufferPtr buffer,
		      double number,
		      int is_upper)
{
    char temp_string[sizeof(double) * CHAR_BIT * sizeof(xmlChar) + 1];
    char *pointer;
    int i;
    char *alpha_list;
    double alpha_size = (double)(sizeof(alpha_upper_list) - 1);

    
    pointer = &temp_string[sizeof(temp_string)];
    *(--pointer) = 0;
    alpha_list = (is_upper) ? alpha_upper_list : alpha_lower_list;
    
    for (i = 1; i < (int)sizeof(temp_string); i++) {
	number--;
	*(--pointer) = alpha_list[((int)fmod(number, alpha_size))];
	number /= alpha_size;
	if (fabs(number) < 1.0)
	    break; 
    }
    xmlBufferCCat(buffer, pointer);
}

static void
xsltNumberFormatRoman(xmlBufferPtr buffer,
		      double number,
		      int is_upper)
{
    while (number >= 1000.0) {
	xmlBufferCCat(buffer, (is_upper) ? "M" : "m");
	number -= 1000.0;
    }
    if (number >= 900.0) {
	xmlBufferCCat(buffer, (is_upper) ? "CM" : "cm");
	number -= 900.0;
    }
    while (number >= 500.0) {
	xmlBufferCCat(buffer, (is_upper) ? "D" : "d");
	number -= 500.0;
    }
    if (number >= 400.0) {
	xmlBufferCCat(buffer, (is_upper) ? "CD" : "cd");
	number -= 400.0;
    }
    while (number >= 100.0) {
	xmlBufferCCat(buffer, (is_upper) ? "C" : "c");
	number -= 100.0;
    }
    if (number >= 90.0) {
	xmlBufferCCat(buffer, (is_upper) ? "XC" : "xc");
	number -= 90.0;
    }
    while (number >= 50.0) {
	xmlBufferCCat(buffer, (is_upper) ? "L" : "l");
	number -= 50.0;
    }
    if (number >= 40.0) {
	xmlBufferCCat(buffer, (is_upper) ? "XL" : "xl");
	number -= 40.0;
    }
    while (number >= 10.0) {
	xmlBufferCCat(buffer, (is_upper) ? "X" : "x");
	number -= 10.0;
    }
    if (number >= 9.0) {
	xmlBufferCCat(buffer, (is_upper) ? "IX" : "ix");
	number -= 9.0;
    }
    while (number >= 5.0) {
	xmlBufferCCat(buffer, (is_upper) ? "V" : "v");
	number -= 5.0;
    }
    if (number >= 4.0) {
	xmlBufferCCat(buffer, (is_upper) ? "IV" : "iv");
	number -= 4.0;
    }
    while (number >= 1.0) {
	xmlBufferCCat(buffer, (is_upper) ? "I" : "i");
	number--;
    }
}

static void
xsltNumberFormatTokenize(const xmlChar *format,
			 xsltFormatPtr tokens)
{
    int ix = 0;
    int j;
    int val;
    int len;

    default_token.token = DEFAULT_TOKEN;
    default_token.width = 1;
    default_token.separator = BAD_CAST(DEFAULT_SEPARATOR);


    tokens->start = NULL;
    tokens->tokens[0].separator = NULL;
    tokens->end = NULL;

    while (! (IS_LETTER(val=xmlStringCurrentChar(NULL, format+ix, &len)) ||
    	      IS_DIGIT(val)) ) {
	if (format[ix] == 0)		
	    break; 
	ix += len;
    }
    if (ix > 0)
	tokens->start = xmlStrndup(format, ix);


    for (tokens->nTokens = 0; tokens->nTokens < MAX_TOKENS;
	 tokens->nTokens++) {
	if (format[ix] == 0)
	    break; 

	if (tokens->nTokens > 0) {
	    tokens->tokens[tokens->nTokens].separator = tokens->end;
	    tokens->end = NULL;
	}

	val = xmlStringCurrentChar(NULL, format+ix, &len);
	if (IS_DIGIT_ONE(val) ||
		 IS_DIGIT_ZERO(val)) {
	    tokens->tokens[tokens->nTokens].width = 1;
	    while (IS_DIGIT_ZERO(val)) {
		tokens->tokens[tokens->nTokens].width++;
		ix += len;
		val = xmlStringCurrentChar(NULL, format+ix, &len);
	    }
	    if (IS_DIGIT_ONE(val)) {
		tokens->tokens[tokens->nTokens].token = val - 1;
		ix += len;
		val = xmlStringCurrentChar(NULL, format+ix, &len);
	    }
	} else if ( (val == (xmlChar)'A') ||
		    (val == (xmlChar)'a') ||
		    (val == (xmlChar)'I') ||
		    (val == (xmlChar)'i') ) {
	    tokens->tokens[tokens->nTokens].token = val;
	    ix += len;
	    val = xmlStringCurrentChar(NULL, format+ix, &len);
	} else {
	    tokens->tokens[tokens->nTokens].token = (xmlChar)'0';
	    tokens->tokens[tokens->nTokens].width = 1;
	}
	while (IS_LETTER(val) || IS_DIGIT(val)) {
	    ix += len;
	    val = xmlStringCurrentChar(NULL, format+ix, &len);
	}

	j = ix;
	while (! (IS_LETTER(val) || IS_DIGIT(val))) {
	    if (val == 0)
		break; 
	    ix += len;
	    val = xmlStringCurrentChar(NULL, format+ix, &len);
	}
	if (ix > j)
	    tokens->end = xmlStrndup(&format[j], ix - j);
    }
}

static void
xsltNumberFormatInsertNumbers(xsltNumberDataPtr data,
			      double *numbers,
			      int numbers_max,
			      xsltFormatPtr tokens,
			      xmlBufferPtr buffer)
{
    int i = 0;
    double number;
    xsltFormatTokenPtr token;

    if (tokens->start != NULL)
	 xmlBufferCat(buffer, tokens->start);

    for (i = 0; i < numbers_max; i++) {
	
	number = numbers[(numbers_max - 1) - i];
	if (i < tokens->nTokens) {
	  token = &(tokens->tokens[i]);
	} else if (tokens->nTokens > 0) {
	  token = &(tokens->tokens[tokens->nTokens - 1]);
	} else {
	  token = &default_token;
	}

	
	if (i > 0) {
	    if (token->separator != NULL)
		xmlBufferCat(buffer, token->separator);
	    else
		xmlBufferCCat(buffer, DEFAULT_SEPARATOR);
	}

	switch (xmlXPathIsInf(number)) {
	case -1:
	    xmlBufferCCat(buffer, "-Infinity");
	    break;
	case 1:
	    xmlBufferCCat(buffer, "Infinity");
	    break;
	default:
	    if (xmlXPathIsNaN(number)) {
		xmlBufferCCat(buffer, "NaN");
	    } else {

		switch (token->token) {
		case 'A':
		    xsltNumberFormatAlpha(buffer,
					  number,
					  TRUE);

		    break;
		case 'a':
		    xsltNumberFormatAlpha(buffer,
					  number,
					  FALSE);

		    break;
		case 'I':
		    xsltNumberFormatRoman(buffer,
					  number,
					  TRUE);

		    break;
		case 'i':
		    xsltNumberFormatRoman(buffer,
					  number,
					  FALSE);

		    break;
		default:
		    if (IS_DIGIT_ZERO(token->token)) {
			xsltNumberFormatDecimal(buffer,
						number,
						token->token,
						token->width,
						data->digitsPerGroup,
						data->groupingCharacter,
						data->groupingCharacterLen);
		    }
		    break;
		}
	    }

	}
    }

    if (tokens->end != NULL)
	 xmlBufferCat(buffer, tokens->end);

}

static int
xsltNumberFormatGetAnyLevel(xsltTransformContextPtr context,
			    xmlNodePtr node,
			    const xmlChar *count,
			    const xmlChar *from,
			    double *array,
			    xmlDocPtr doc,
			    xmlNodePtr elem)
{
    int amount = 0;
    int cnt = 0;
    xmlNodePtr cur;
    xsltCompMatchPtr countPat = NULL;
    xsltCompMatchPtr fromPat = NULL;

    if (count != NULL)
	countPat = xsltCompilePattern(count, doc, elem, NULL, context);
    if (from != NULL)
	fromPat = xsltCompilePattern(from, doc, elem, NULL, context);
	
    
    switch (node->type) {
	case XML_ELEMENT_NODE:
	    cur = node;
	    break;
	case XML_ATTRIBUTE_NODE:
	    cur = ((xmlAttrPtr) node)->parent;
	    break;
	case XML_TEXT_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	    cur = node->parent;
	    break;
	default:
	    cur = NULL;
	    break;
    }

    while (cur != NULL) {
	
	if (count == NULL) {
	    if ((node->type == cur->type) &&
		
		xmlStrEqual(node->name, cur->name)) {
		    if ((node->ns == cur->ns) ||
		        ((node->ns != NULL) &&
			 (cur->ns != NULL) &&
		         (xmlStrEqual(node->ns->href,
		             cur->ns->href) )))
		        cnt++;
	    }
	} else {
	    if (xsltTestCompMatchList(context, cur, countPat))
		cnt++;
	}
	if ((from != NULL) &&
	    xsltTestCompMatchList(context, cur, fromPat)) {
	    break; 
	}

	
	if ((cur->type == XML_DOCUMENT_NODE) ||
#ifdef LIBXML_DOCB_ENABLED
            (cur->type == XML_DOCB_DOCUMENT_NODE) ||
#endif
            (cur->type == XML_HTML_DOCUMENT_NODE))
	    break; 

	while ((cur->prev != NULL) && ((cur->prev->type == XML_DTD_NODE) ||
	       (cur->prev->type == XML_XINCLUDE_START) ||
	       (cur->prev->type == XML_XINCLUDE_END)))
	    cur = cur->prev;
	if (cur->prev != NULL) {
	    for (cur = cur->prev; cur->last != NULL; cur = cur->last);
	} else {
	    cur = cur->parent;
	}

    }

    array[amount++] = (double) cnt;

    if (countPat != NULL)
	xsltFreeCompMatchList(countPat);
    if (fromPat != NULL)
	xsltFreeCompMatchList(fromPat);
    return(amount);
}

static int
xsltNumberFormatGetMultipleLevel(xsltTransformContextPtr context,
				 xmlNodePtr node,
				 const xmlChar *count,
				 const xmlChar *from,
				 double *array,
				 int max,
				 xmlDocPtr doc,
				 xmlNodePtr elem)
{
    int amount = 0;
    int cnt;
    xmlNodePtr ancestor;
    xmlNodePtr preceding;
    xmlXPathParserContextPtr parser;
    xsltCompMatchPtr countPat;
    xsltCompMatchPtr fromPat;

    if (count != NULL)
	countPat = xsltCompilePattern(count, doc, elem, NULL, context);
    else
	countPat = NULL;
    if (from != NULL)
	fromPat = xsltCompilePattern(from, doc, elem, NULL, context);
    else
	fromPat = NULL;
    context->xpathCtxt->node = node;
    parser = xmlXPathNewParserContext(NULL, context->xpathCtxt);
    if (parser) {
	
	for (ancestor = node;
	     (ancestor != NULL) && (ancestor->type != XML_DOCUMENT_NODE);
	     ancestor = xmlXPathNextAncestor(parser, ancestor)) {
	    
	    if ((from != NULL) &&
		xsltTestCompMatchList(context, ancestor, fromPat))
		break; 
	    
	    if ((count == NULL && node->type == ancestor->type && 
		xmlStrEqual(node->name, ancestor->name)) ||
		xsltTestCompMatchList(context, ancestor, countPat)) {
		
		cnt = 0;
		for (preceding = ancestor;
		     preceding != NULL;
		     preceding = 
		        xmlXPathNextPrecedingSibling(parser, preceding)) {
		    if (count == NULL) {
			if ((preceding->type == ancestor->type) &&
			    xmlStrEqual(preceding->name, ancestor->name)){
			    if ((preceding->ns == ancestor->ns) ||
			        ((preceding->ns != NULL) &&
				 (ancestor->ns != NULL) &&
			         (xmlStrEqual(preceding->ns->href,
			             ancestor->ns->href) )))
			        cnt++;
			}
		    } else {
			if (xsltTestCompMatchList(context, preceding,
				                  countPat))
			    cnt++;
		    }
		}
		array[amount++] = (double)cnt;
		if (amount >= max)
		    break; 
	    }
	}
	xmlXPathFreeParserContext(parser);
    }
    xsltFreeCompMatchList(countPat);
    xsltFreeCompMatchList(fromPat);
    return amount;
}

static int
xsltNumberFormatGetValue(xmlXPathContextPtr context,
			 xmlNodePtr node,
			 const xmlChar *value,
			 double *number)
{
    int amount = 0;
    xmlBufferPtr pattern;
    xmlXPathObjectPtr obj;
    
    pattern = xmlBufferCreate();
    if (pattern != NULL) {
	xmlBufferCCat(pattern, "number(");
	xmlBufferCat(pattern, value);
	xmlBufferCCat(pattern, ")");
	context->node = node;
	obj = xmlXPathEvalExpression(xmlBufferContent(pattern),
				     context);
	if (obj != NULL) {
	    *number = obj->floatval;
	    amount++;
	    xmlXPathFreeObject(obj);
	}
	xmlBufferFree(pattern);
    }
    return amount;
}

void
xsltNumberFormat(xsltTransformContextPtr ctxt,
		 xsltNumberDataPtr data,
		 xmlNodePtr node)
{
    xmlBufferPtr output = NULL;
    int amount, i;
    double number;
    xsltFormat tokens;
    int tempformat = 0;

    if ((data->format == NULL) && (data->has_format != 0)) {
	data->format = xsltEvalAttrValueTemplate(ctxt, data->node,
					     (const xmlChar *) "format",
					     XSLT_NAMESPACE);
	tempformat = 1;
    }
    if (data->format == NULL) {
	return;
    }

    output = xmlBufferCreate();
    if (output == NULL)
	goto XSLT_NUMBER_FORMAT_END;

    xsltNumberFormatTokenize(data->format, &tokens);

    if (data->value) {
	amount = xsltNumberFormatGetValue(ctxt->xpathCtxt,
					  node,
					  data->value,
					  &number);
	if (amount == 1) {
	    xsltNumberFormatInsertNumbers(data,
					  &number,
					  1,
					  &tokens,
					  output);
	}
	
    } else if (data->level) {
	
	if (xmlStrEqual(data->level, (const xmlChar *) "single")) {
	    amount = xsltNumberFormatGetMultipleLevel(ctxt,
						      node,
						      data->count,
						      data->from,
						      &number,
						      1,
						      data->doc,
						      data->node);
	    if (amount == 1) {
		xsltNumberFormatInsertNumbers(data,
					      &number,
					      1,
					      &tokens,
					      output);
	    }
	} else if (xmlStrEqual(data->level, (const xmlChar *) "multiple")) {
	    double numarray[1024];
	    int max = sizeof(numarray)/sizeof(numarray[0]);
	    amount = xsltNumberFormatGetMultipleLevel(ctxt,
						      node,
						      data->count,
						      data->from,
						      numarray,
						      max,
						      data->doc,
						      data->node);
	    if (amount > 0) {
		xsltNumberFormatInsertNumbers(data,
					      numarray,
					      amount,
					      &tokens,
					      output);
	    }
	} else if (xmlStrEqual(data->level, (const xmlChar *) "any")) {
	    amount = xsltNumberFormatGetAnyLevel(ctxt,
						 node,
						 data->count,
						 data->from,
						 &number, 
						 data->doc,
						 data->node);
	    if (amount > 0) {
		xsltNumberFormatInsertNumbers(data,
					      &number,
					      1,
					      &tokens,
					      output);
	    }
	}
    }
    
    xsltCopyTextString(ctxt, ctxt->insert, xmlBufferContent(output), 0);

    if (tokens.start != NULL)
	xmlFree(tokens.start);
    if (tokens.end != NULL)
	xmlFree(tokens.end);
    for (i = 0;i < tokens.nTokens;i++) {
	if (tokens.tokens[i].separator != NULL)
	    xmlFree(tokens.tokens[i].separator);
    }
    
XSLT_NUMBER_FORMAT_END:
    if (tempformat == 1) {
	
	data->format = NULL;
    }
    if (output != NULL)
	xmlBufferFree(output);
}

static int
xsltFormatNumberPreSuffix(xsltDecimalFormatPtr self, xmlChar **format, xsltFormatNumberInfoPtr info)
{
    int	count=0;	
    int len;

    while (1) {
	if (**format == 0)
	    return count;
	
	if (**format == SYMBOL_QUOTE) {
	    if (*++(*format) == 0)
		return -1;
	}
	else if (IS_SPECIAL(self, *format))
	    return count;
	else {
	    if (xsltUTF8Charcmp(*format, self->percent) == 0) {
		if (info->is_multiplier_set)
		    return -1;
		info->multiplier = 100;
		info->is_multiplier_set = TRUE;
	    } else if (xsltUTF8Charcmp(*format, self->permille) == 0) {
		if (info->is_multiplier_set)
		    return -1;
		info->multiplier = 1000;
		info->is_multiplier_set = TRUE;
	    }
	}
	
	if ((len=xsltUTF8Size(*format)) < 1)
	    return -1;
	count += len;
	*format += len;
    }
}
	    
xmlXPathError
xsltFormatNumberConversion(xsltDecimalFormatPtr self,
			   xmlChar *format,
			   double number,
			   xmlChar **result)
{
    xmlXPathError status = XPATH_EXPRESSION_OK;
    xmlBufferPtr buffer;
    xmlChar *the_format, *prefix = NULL, *suffix = NULL;
    xmlChar *nprefix, *nsuffix = NULL;
    xmlChar pchar;
    int	    prefix_length, suffix_length = 0, nprefix_length, nsuffix_length;
    double  scale;
    int	    j, len;
    int     self_grouping_len;
    xsltFormatNumberInfo format_info;
    int		delayed_multiplier = 0;
    
    char	default_sign = 0;
    
    char	found_error = 0;

    if (xmlStrlen(format) <= 0) {
	xsltTransformError(NULL, NULL, NULL,
                "xsltFormatNumberConversion : "
		"Invalid format (0-length)\n");
    }
    *result = NULL;
    switch (xmlXPathIsInf(number)) {
	case -1:
	    if (self->minusSign == NULL)
		*result = xmlStrdup(BAD_CAST "-");
	    else
		*result = xmlStrdup(self->minusSign);
	    
	case 1:
	    if ((self == NULL) || (self->infinity == NULL))
		*result = xmlStrcat(*result, BAD_CAST "Infinity");
	    else
		*result = xmlStrcat(*result, self->infinity);
	    return(status);
	default:
	    if (xmlXPathIsNaN(number)) {
		if ((self == NULL) || (self->noNumber == NULL))
		    *result = xmlStrdup(BAD_CAST "NaN");
		else
		    *result = xmlStrdup(self->noNumber);
		return(status);
	    }
    }

    buffer = xmlBufferCreate();
    if (buffer == NULL) {
	return XPATH_MEMORY_ERROR;
    }

    format_info.integer_hash = 0;
    format_info.integer_digits = 0;
    format_info.frac_digits = 0;
    format_info.frac_hash = 0;
    format_info.group = -1;
    format_info.multiplier = 1;
    format_info.add_decimal = FALSE;
    format_info.is_multiplier_set = FALSE;
    format_info.is_negative_pattern = FALSE;

    the_format = format;

    prefix = the_format;
    prefix_length = xsltFormatNumberPreSuffix(self, &the_format, &format_info);
    if (prefix_length < 0) {
	found_error = 1;
	goto OUTPUT_NUMBER;
    }

    self_grouping_len = xmlStrlen(self->grouping);
    while ((*the_format != 0) &&
	   (xsltUTF8Charcmp(the_format, self->decimalPoint) != 0) &&
	   (xsltUTF8Charcmp(the_format, self->patternSeparator) != 0)) {
	
	if (delayed_multiplier != 0) {
	    format_info.multiplier = delayed_multiplier;
	    format_info.is_multiplier_set = TRUE;
	    delayed_multiplier = 0;
	}
	if (xsltUTF8Charcmp(the_format, self->digit) == 0) {
	    if (format_info.integer_digits > 0) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    format_info.integer_hash++;
	    if (format_info.group >= 0)
		format_info.group++;
	} else if (xsltUTF8Charcmp(the_format, self->zeroDigit) == 0) {
	    format_info.integer_digits++;
	    if (format_info.group >= 0)
		format_info.group++;
	} else if ((self_grouping_len > 0) &&
	    (!xmlStrncmp(the_format, self->grouping, self_grouping_len))) {
	    
	    format_info.group = 0;
	    the_format += self_grouping_len;
	    continue;
	} else if (xsltUTF8Charcmp(the_format, self->percent) == 0) {
	    if (format_info.is_multiplier_set) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    delayed_multiplier = 100;
	} else  if (xsltUTF8Charcmp(the_format, self->permille) == 0) {
	    if (format_info.is_multiplier_set) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    delayed_multiplier = 1000;
	} else
	    break; 
	
	if ((len=xsltUTF8Size(the_format)) < 1) {
	    found_error = 1;
	    goto OUTPUT_NUMBER;
	}
	the_format += len;

    }

    
    if (xsltUTF8Charcmp(the_format, self->decimalPoint) == 0) {
        format_info.add_decimal = TRUE;
	the_format += xsltUTF8Size(the_format);	
    }
    
    while (*the_format != 0) {
	
	if (xsltUTF8Charcmp(the_format, self->zeroDigit) == 0) {
	    if (format_info.frac_hash != 0) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    format_info.frac_digits++;
	} else if (xsltUTF8Charcmp(the_format, self->digit) == 0) {
	    format_info.frac_hash++;
	} else if (xsltUTF8Charcmp(the_format, self->percent) == 0) {
	    if (format_info.is_multiplier_set) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    delayed_multiplier = 100;
	    if ((len = xsltUTF8Size(the_format)) < 1) {
	        found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    the_format += len;
	    continue; 
	} else if (xsltUTF8Charcmp(the_format, self->permille) == 0) {
	    if (format_info.is_multiplier_set) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    delayed_multiplier = 1000;
	    if  ((len = xsltUTF8Size(the_format)) < 1) {
	        found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    the_format += len;
	    continue; 
	} else if (xsltUTF8Charcmp(the_format, self->grouping) != 0) {
	    break; 
	}
	if ((len = xsltUTF8Size(the_format)) < 1) {
	    found_error = 1;
	    goto OUTPUT_NUMBER;
	}
	the_format += len;
	if (delayed_multiplier != 0) {
	    format_info.multiplier = delayed_multiplier;
	    delayed_multiplier = 0;
	    format_info.is_multiplier_set = TRUE;
	}
    }

    if (delayed_multiplier != 0) {
	the_format -= len;
	delayed_multiplier = 0;
    }

    suffix = the_format;
    suffix_length = xsltFormatNumberPreSuffix(self, &the_format, &format_info);
    if ( (suffix_length < 0) ||
	 ((*the_format != 0) && 
	  (xsltUTF8Charcmp(the_format, self->patternSeparator) != 0)) ) {
	found_error = 1;
	goto OUTPUT_NUMBER;
    }

    if (number < 0) {
        j =  xmlUTF8Strloc(format, self->patternSeparator);
	if (j < 0) {
	
	    default_sign = 1;
	}
	else {
	    
	    the_format = (xmlChar *)xmlUTF8Strpos(format, j + 1);
	    format_info.is_negative_pattern = TRUE;
	    format_info.is_multiplier_set = FALSE;

	    
	    nprefix = the_format;
	    nprefix_length = xsltFormatNumberPreSuffix(self, 
	    				&the_format, &format_info);
	    if (nprefix_length<0) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }

	    while (*the_format != 0) {
		if ( (xsltUTF8Charcmp(the_format, (self)->percent) == 0) ||
		     (xsltUTF8Charcmp(the_format, (self)->permille)== 0) ) {
		    if (format_info.is_multiplier_set) {
			found_error = 1;
			goto OUTPUT_NUMBER;
		    }
		    format_info.is_multiplier_set = TRUE;
		    delayed_multiplier = 1;
		}
		else if (IS_SPECIAL(self, the_format))
		    delayed_multiplier = 0;
		else
		    break; 
		if ((len = xsltUTF8Size(the_format)) < 1) {
		    found_error = 1;
		    goto OUTPUT_NUMBER;
		}
		the_format += len;
	    }
	    if (delayed_multiplier != 0) {
		format_info.is_multiplier_set = FALSE;
		the_format -= len;
	    }

	    
	    if (*the_format != 0) {
		nsuffix = the_format;
		nsuffix_length = xsltFormatNumberPreSuffix(self, 
					&the_format, &format_info);
		if (nsuffix_length < 0) {
		    found_error = 1;
		    goto OUTPUT_NUMBER;
		}
	    }
	    else
		nsuffix_length = 0;
	    if (*the_format != 0) {
		found_error = 1;
		goto OUTPUT_NUMBER;
	    }
	    if ((nprefix_length != prefix_length) ||
	    	(nsuffix_length != suffix_length) ||
		((nprefix_length > 0) && 
		 (xmlStrncmp(nprefix, prefix, prefix_length) !=0 )) ||
		((nsuffix_length > 0) && 
		 (xmlStrncmp(nsuffix, suffix, suffix_length) !=0 ))) {
	 	prefix = nprefix;
		prefix_length = nprefix_length;
		suffix = nsuffix;
		suffix_length = nsuffix_length;
	    } 
	}
    }

OUTPUT_NUMBER:
    if (found_error != 0) {
	xsltTransformError(NULL, NULL, NULL,
                "xsltFormatNumberConversion : "
		"error in format string '%s', using default\n", format);
	default_sign = (number < 0.0) ? 1 : 0;
	prefix_length = suffix_length = 0;
	format_info.integer_hash = 0;
	format_info.integer_digits = 1;
	format_info.frac_digits = 1;
	format_info.frac_hash = 4;
	format_info.group = -1;
	format_info.multiplier = 1;
	format_info.add_decimal = TRUE;
    }

    
    if (default_sign != 0)
	xmlBufferAdd(buffer, self->minusSign, xsltUTF8Size(self->minusSign));

    
    for (j = 0; j < prefix_length; j++) {
	if ((pchar = *prefix++) == SYMBOL_QUOTE) {
	    len = xsltUTF8Size(prefix);
	    xmlBufferAdd(buffer, prefix, len);
	    prefix += len;
	    j += len - 1;	
	} else
	    xmlBufferAdd(buffer, &pchar, 1);
    }

    
    number = fabs(number) * (double)format_info.multiplier;
    scale = pow(10.0, (double)(format_info.frac_digits + format_info.frac_hash));
    number = floor((scale * number + 0.5)) / scale;
    if ((self->grouping != NULL) && 
        (self->grouping[0] != 0)) {
	
	len = xmlStrlen(self->grouping);
	pchar = xsltGetUTF8Char(self->grouping, &len);
	xsltNumberFormatDecimal(buffer, floor(number), self->zeroDigit[0],
				format_info.integer_digits,
				format_info.group,
				pchar, len);
    } else
	xsltNumberFormatDecimal(buffer, floor(number), self->zeroDigit[0],
				format_info.integer_digits,
				format_info.group,
				',', 1);

    
    if ((format_info.integer_digits + format_info.integer_hash +
	 format_info.frac_digits == 0) && (format_info.frac_hash > 0)) {
        ++format_info.frac_digits;
	--format_info.frac_hash;
    }

    
    if ((floor(number) == 0) &&
	(format_info.integer_digits + format_info.frac_digits == 0)) {
        xmlBufferAdd(buffer, self->zeroDigit, xsltUTF8Size(self->zeroDigit));
    }

    
    if (format_info.frac_digits + format_info.frac_hash == 0) {
        if (format_info.add_decimal)
	    xmlBufferAdd(buffer, self->decimalPoint, 
	    		 xsltUTF8Size(self->decimalPoint));
    }
    else {
      number -= floor(number);
	if ((number != 0) || (format_info.frac_digits != 0)) {
	    xmlBufferAdd(buffer, self->decimalPoint,
	    		 xsltUTF8Size(self->decimalPoint));
	    number = floor(scale * number + 0.5);
	    for (j = format_info.frac_hash; j > 0; j--) {
		if (fmod(number, 10.0) >= 1.0)
		    break; 
		number /= 10.0;
	    }
	    xsltNumberFormatDecimal(buffer, floor(number), self->zeroDigit[0],
				format_info.frac_digits + j,
				0, 0, 0);
	}
    }
    
    for (j = 0; j < suffix_length; j++) {
	if ((pchar = *suffix++) == SYMBOL_QUOTE) {
            len = xsltUTF8Size(suffix);
	    xmlBufferAdd(buffer, suffix, len);
	    suffix += len;
	    j += len - 1;	
	} else
	    xmlBufferAdd(buffer, &pchar, 1);
    }

    *result = xmlStrdup(xmlBufferContent(buffer));
    xmlBufferFree(buffer);
    return status;
}

