/*
 * Summary: Implementation of the XSLT number functions
 * Description: Implementation of the XSLT number functions
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Bjorn Reese <breese@users.sourceforge.net> and Daniel Veillard
 */

#ifndef __XML_XSLT_NUMBERSINTERNALS_H__
#define __XML_XSLT_NUMBERSINTERNALS_H__

#include <libxml/tree.h>
#include "xsltexports.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xsltNumberData xsltNumberData;
typedef xsltNumberData *xsltNumberDataPtr;
    
struct _xsltNumberData {
    const xmlChar *level;
    const xmlChar *count;
    const xmlChar *from;
    const xmlChar *value;
    const xmlChar *format;
    int has_format;
    int digitsPerGroup;
    int groupingCharacter;
    int groupingCharacterLen;
    xmlDocPtr doc;
    xmlNodePtr node;

};

typedef struct _xsltFormatNumberInfo xsltFormatNumberInfo;
typedef xsltFormatNumberInfo *xsltFormatNumberInfoPtr;

struct _xsltFormatNumberInfo {
    int	    integer_hash;	
    int	    integer_digits;	
    int	    frac_digits;	
    int	    frac_hash;		
    int	    group;		
    int     multiplier;		
    char    add_decimal;	
    char    is_multiplier_set;	
    char    is_negative_pattern;
};

#ifdef __cplusplus
}
#endif
#endif 
