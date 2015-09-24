#define IN_LIBEXSLT
#include "libexslt/libexslt.h"

#if defined(WIN32) && !defined (__CYGWIN__) && (!__MINGW32__)
#include <win32config.h>
#else
#include "config.h"
#endif

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libxslt/xsltconfig.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>

#ifdef HAVE_MATH_H
#include <math.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include "exslt.h"

static double
exsltMathMin (xmlNodeSetPtr ns) {
    double ret, cur;
    int i;

    if ((ns == NULL) || (ns->nodeNr == 0))
	return(xmlXPathNAN);
    ret = xmlXPathCastNodeToNumber(ns->nodeTab[0]);
    if (xmlXPathIsNaN(ret))
	return(xmlXPathNAN);
    for (i = 1; i < ns->nodeNr; i++) {
	cur = xmlXPathCastNodeToNumber(ns->nodeTab[i]);
	if (xmlXPathIsNaN(cur))
	    return(xmlXPathNAN);
	if (cur < ret)
	    ret = cur;
    }
    return(ret);
}

static void
exsltMathMinFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr ns;
    double ret;
    void *user = NULL;

    if (nargs != 1) {
	xsltGenericError(xsltGenericErrorContext,
			 "math:min: invalid number of arguments\n");
	ctxt->error = XPATH_INVALID_ARITY;
	return;
    }
    
    if ((ctxt->value != NULL) && (ctxt->value->boolval != 0)) {
        user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = NULL;
    }
    ns = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathMin(ns);

    xmlXPathFreeNodeSet(ns);
    if (user != NULL)
        xmlFreeNodeList((xmlNodePtr)user);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathMax (xmlNodeSetPtr ns) {
    double ret, cur;
    int i;

    if ((ns == NULL) || (ns->nodeNr == 0))
	return(xmlXPathNAN);
    ret = xmlXPathCastNodeToNumber(ns->nodeTab[0]);
    if (xmlXPathIsNaN(ret))
	return(xmlXPathNAN);
    for (i = 1; i < ns->nodeNr; i++) {
	cur = xmlXPathCastNodeToNumber(ns->nodeTab[i]);
	if (xmlXPathIsNaN(cur))
	    return(xmlXPathNAN);
	if (cur > ret)
	    ret = cur;
    }
    return(ret);
}

static void
exsltMathMaxFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr ns;
    double ret;
    void *user = NULL;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    
    if ((ctxt->value != NULL) && (ctxt->value->boolval != 0)) {
	user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = 0;
    }
    ns = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathMax(ns);

    xmlXPathFreeNodeSet(ns);

    if (user != NULL)
        xmlFreeNodeList((xmlNodePtr)user);
    xmlXPathReturnNumber(ctxt, ret);
}

static xmlNodeSetPtr
exsltMathHighest (xmlNodeSetPtr ns) {
    xmlNodeSetPtr ret = xmlXPathNodeSetCreate(NULL);
    double max, cur;
    int i;

    if ((ns == NULL) || (ns->nodeNr == 0))
	return(ret);

    max = xmlXPathCastNodeToNumber(ns->nodeTab[0]);
    if (xmlXPathIsNaN(max))
	return(ret);
    else
	xmlXPathNodeSetAddUnique(ret, ns->nodeTab[0]);

    for (i = 1; i < ns->nodeNr; i++) {
	cur = xmlXPathCastNodeToNumber(ns->nodeTab[i]);
	if (xmlXPathIsNaN(cur)) {
	    xmlXPathEmptyNodeSet(ret);
	    return(ret);
	}
	if (cur < max)
	    continue;
	if (cur > max) {
	    max = cur;
	    xmlXPathEmptyNodeSet(ret);
	    xmlXPathNodeSetAddUnique(ret, ns->nodeTab[i]);
	    continue;
	}
	xmlXPathNodeSetAddUnique(ret, ns->nodeTab[i]);
    }
    return(ret);
}

static void
exsltMathHighestFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr ns, ret;
    void *user = NULL;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    
    if ((ctxt->value != NULL) && ctxt->value->boolval != 0) {
        user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = NULL;
    }
    ns = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathHighest(ns);

    xmlXPathFreeNodeSet(ns);
    if (user != NULL)
        xmlFreeNodeList((xmlNodePtr)user);

    xmlXPathReturnNodeSet(ctxt, ret);
}

static xmlNodeSetPtr
exsltMathLowest (xmlNodeSetPtr ns) {
    xmlNodeSetPtr ret = xmlXPathNodeSetCreate(NULL);
    double min, cur;
    int i;

    if ((ns == NULL) || (ns->nodeNr == 0))
	return(ret);

    min = xmlXPathCastNodeToNumber(ns->nodeTab[0]);
    if (xmlXPathIsNaN(min))
	return(ret);
    else
	xmlXPathNodeSetAddUnique(ret, ns->nodeTab[0]);

    for (i = 1; i < ns->nodeNr; i++) {
	cur = xmlXPathCastNodeToNumber(ns->nodeTab[i]);
	if (xmlXPathIsNaN(cur)) {
	    xmlXPathEmptyNodeSet(ret);
	    return(ret);
	}
        if (cur > min)
	    continue;
	if (cur < min) {
	    min = cur;
	    xmlXPathEmptyNodeSet(ret);
	    xmlXPathNodeSetAddUnique(ret, ns->nodeTab[i]);
            continue;
	}
	xmlXPathNodeSetAddUnique(ret, ns->nodeTab[i]);
    }
    return(ret);
}

static void
exsltMathLowestFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlNodeSetPtr ns, ret;
    void *user = NULL;
    

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    
    if ((ctxt->value != NULL) && (ctxt->value->boolval != 0)) {
        user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = NULL;
    }
    ns = xmlXPathPopNodeSet(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathLowest(ns);

    xmlXPathFreeNodeSet(ns);
    if (user != NULL)
        xmlFreeNodeList((xmlNodePtr)user);

    xmlXPathReturnNodeSet(ctxt, ret);
}


#define EXSLT_PI        (const xmlChar *) \
			"3.1415926535897932384626433832795028841971693993751"
#define EXSLT_E         (const xmlChar *) \
			"2.71828182845904523536028747135266249775724709369996"
#define EXSLT_SQRRT2    (const xmlChar *) \
			"1.41421356237309504880168872420969807856967187537694"
#define EXSLT_LN2       (const xmlChar *) \
			"0.69314718055994530941723212145817656807550013436025"
#define EXSLT_LN10      (const xmlChar *) \
			"2.30258509299404568402"
#define EXSLT_LOG2E     (const xmlChar *) \
			"1.4426950408889634074"
#define EXSLT_SQRT1_2   (const xmlChar *) \
			"0.70710678118654752440"

static double
exsltMathConstant (xmlChar *name, double precision) {
    xmlChar *str;
    double ret;

    if ((name == NULL) || (xmlXPathIsNaN(precision)) || (precision < 1.0)) {
        return xmlXPathNAN;
    }

    if (xmlStrEqual(name, BAD_CAST "PI")) {
        int len = xmlStrlen(EXSLT_PI);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_PI, 0, len);

    } else if (xmlStrEqual(name, BAD_CAST "E")) {
        int len = xmlStrlen(EXSLT_E);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_E, 0, len);

    } else if (xmlStrEqual(name, BAD_CAST "SQRRT2")) {
        int len = xmlStrlen(EXSLT_SQRRT2);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_SQRRT2, 0, len);

    } else if (xmlStrEqual(name, BAD_CAST "LN2")) {
        int len = xmlStrlen(EXSLT_LN2);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_LN2, 0, len);

    } else if (xmlStrEqual(name, BAD_CAST "LN10")) {
        int len = xmlStrlen(EXSLT_LN10);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_LN10, 0, len);

    } else if (xmlStrEqual(name, BAD_CAST "LOG2E")) {
        int len = xmlStrlen(EXSLT_LOG2E);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_LOG2E, 0, len);

    } else if (xmlStrEqual(name, BAD_CAST "SQRT1_2")) {
        int len = xmlStrlen(EXSLT_SQRT1_2);

        if (precision <= len)
            len = (int)precision;
        
        str = xmlStrsub(EXSLT_SQRT1_2, 0, len);

    } else {
	str = NULL;
    }
    if (str == NULL)
        return xmlXPathNAN;
    ret = xmlXPathCastStringToNumber(str);
    xmlFree(str);
    return ret;
}

static void
exsltMathConstantFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double   ret;
    xmlChar *name;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    name = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathConstant(name, ret);
    if (name != NULL)
	xmlFree(name);

    xmlXPathReturnNumber(ctxt, ret);
}

#if defined(HAVE_STDLIB_H) && defined(RAND_MAX)

static double
exsltMathRandom (void) {
    double ret;
    int num;

    num = rand();
    ret = (double)num / (double)RAND_MAX;
    return(ret);
}

static void
exsltMathRandomFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 0) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    ret = exsltMathRandom();

    xmlXPathReturnNumber(ctxt, ret);
}

#endif 

#if HAVE_MATH_H

static double
exsltMathAbs (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = fabs(num);
    return(ret);
}

static void
exsltMathAbsFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathAbs(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathSqrt (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = sqrt(num);
    return(ret);
}

static void
exsltMathSqrtFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathSqrt(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathPower (double base, double power) {
    double ret;

    if ((xmlXPathIsNaN(base) || xmlXPathIsNaN(power)))
	return(xmlXPathNAN);
    ret = pow(base, power);
    return(ret);
}

static void
exsltMathPowerFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret, base;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    
    base = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathPower(base, ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathLog (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = log(num);
    return(ret);
}

static void
exsltMathLogFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathLog(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathSin (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = sin(num);
    return(ret);
}

static void
exsltMathSinFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathSin(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathCos (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = cos(num);
    return(ret);
}

static void
exsltMathCosFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathCos(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathTan (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = tan(num);
    return(ret);
}

static void
exsltMathTanFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathTan(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathAsin (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = asin(num);
    return(ret);
}

static void
exsltMathAsinFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathAsin(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathAcos (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = acos(num);
    return(ret);
}

static void
exsltMathAcosFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathAcos(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathAtan (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = atan(num);
    return(ret);
}

static void
exsltMathAtanFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathAtan(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathAtan2 (double y, double x) {
    double ret;

    if ((xmlXPathIsNaN(y) || xmlXPathIsNaN(x)))
	return(xmlXPathNAN);
    ret = atan2(y, x);
    return(ret);
}

static void
exsltMathAtan2Function (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret, x;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    x = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathAtan2(ret, x);

    xmlXPathReturnNumber(ctxt, ret);
}

static double
exsltMathExp (double num) {
    double ret;

    if (xmlXPathIsNaN(num))
	return(xmlXPathNAN);
    ret = exp(num);
    return(ret);
}

static void
exsltMathExpFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    double ret;

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ret = xmlXPathPopNumber(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    ret = exsltMathExp(ret);

    xmlXPathReturnNumber(ctxt, ret);
}

#endif 


void
exsltMathRegister (void) {
    xsltRegisterExtModuleFunction ((const xmlChar *) "min",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathMinFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "max",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathMaxFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "highest",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathHighestFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "lowest",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathLowestFunction);
    
    xsltRegisterExtModuleFunction ((const xmlChar *) "constant",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathConstantFunction);
#ifdef HAVE_STDLIB_H
    xsltRegisterExtModuleFunction ((const xmlChar *) "random",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathRandomFunction);
#endif
#if HAVE_MATH_H
    xsltRegisterExtModuleFunction ((const xmlChar *) "abs",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathAbsFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "sqrt",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathSqrtFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "power",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathPowerFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "log",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathLogFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "sin",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathSinFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "cos",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathCosFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "tan",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathTanFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "asin",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathAsinFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "acos",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathAcosFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "atan",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathAtanFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "atan2",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathAtan2Function);
    xsltRegisterExtModuleFunction ((const xmlChar *) "exp",
				   EXSLT_MATH_NAMESPACE,
				   exsltMathExpFunction);
#endif
}

int
exsltMathXpathCtxtRegister (xmlXPathContextPtr ctxt, const xmlChar *prefix)
{
    if (ctxt
        && prefix
        && !xmlXPathRegisterNs(ctxt,
                               prefix,
                               (const xmlChar *) EXSLT_MATH_NAMESPACE)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "min",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathMinFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "max",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathMaxFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "highest",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathHighestFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "lowest",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathLowestFunction)
#ifdef HAVE_STDLIB_H
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "random",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathRandomFunction)
#endif
#if HAVE_MATH_H
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "abs",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathAbsFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "sqrt",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathSqrtFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "power",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathPowerFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "log",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathLogFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "sin",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathSinFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "cos",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathCosFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "tan",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathTanFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "asin",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathAsinFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "acos",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathAcosFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "atan",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathAtanFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "atan2",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathAtan2Function)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "exp",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathExpFunction)
#endif
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "constant",
                                   (const xmlChar *) EXSLT_MATH_NAMESPACE,
                                   exsltMathConstantFunction)) {
        return 0;
    }
    return -1;
}
