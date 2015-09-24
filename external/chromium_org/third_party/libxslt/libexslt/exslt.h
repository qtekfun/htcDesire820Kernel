
#ifndef __EXSLT_H__
#define __EXSLT_H__

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include "exsltexports.h"
#include <libexslt/exsltconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

EXSLTPUBVAR const char *exsltLibraryVersion;
EXSLTPUBVAR const int exsltLibexsltVersion;
EXSLTPUBVAR const int exsltLibxsltVersion;
EXSLTPUBVAR const int exsltLibxmlVersion;

#define EXSLT_COMMON_NAMESPACE ((const xmlChar *) "http://exslt.org/common")
#define EXSLT_CRYPTO_NAMESPACE ((const xmlChar *) "http://exslt.org/crypto")
#define EXSLT_MATH_NAMESPACE ((const xmlChar *) "http://exslt.org/math")
#define EXSLT_SETS_NAMESPACE ((const xmlChar *) "http://exslt.org/sets")
#define EXSLT_FUNCTIONS_NAMESPACE ((const xmlChar *) "http://exslt.org/functions")
#define EXSLT_STRINGS_NAMESPACE ((const xmlChar *) "http://exslt.org/strings")
#define EXSLT_DATE_NAMESPACE ((const xmlChar *) "http://exslt.org/dates-and-times")
#define EXSLT_DYNAMIC_NAMESPACE ((const xmlChar *) "http://exslt.org/dynamic")

#define SAXON_NAMESPACE ((const xmlChar *) "http://icl.com/saxon")

EXSLTPUBFUN void EXSLTCALL exsltCommonRegister (void);
#ifdef EXSLT_CRYPTO_ENABLED
EXSLTPUBFUN void EXSLTCALL exsltCryptoRegister (void);
#endif
EXSLTPUBFUN void EXSLTCALL exsltMathRegister (void);
EXSLTPUBFUN void EXSLTCALL exsltSetsRegister (void);
EXSLTPUBFUN void EXSLTCALL exsltFuncRegister (void);
EXSLTPUBFUN void EXSLTCALL exsltStrRegister (void);
EXSLTPUBFUN void EXSLTCALL exsltDateRegister (void);
EXSLTPUBFUN void EXSLTCALL exsltSaxonRegister (void);
EXSLTPUBFUN void EXSLTCALL exsltDynRegister(void);

EXSLTPUBFUN void EXSLTCALL exsltRegisterAll (void);

EXSLTPUBFUN int EXSLTCALL exsltDateXpathCtxtRegister (xmlXPathContextPtr ctxt,
                                                      const xmlChar *prefix);
EXSLTPUBFUN int EXSLTCALL exsltMathXpathCtxtRegister (xmlXPathContextPtr ctxt,
                                                      const xmlChar *prefix);
EXSLTPUBFUN int EXSLTCALL exsltSetsXpathCtxtRegister (xmlXPathContextPtr ctxt,
                                                      const xmlChar *prefix);
EXSLTPUBFUN int EXSLTCALL exsltStrXpathCtxtRegister (xmlXPathContextPtr ctxt,
                                                     const xmlChar *prefix);

#ifdef __cplusplus
}
#endif
#endif 

