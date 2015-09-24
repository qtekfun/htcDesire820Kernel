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
#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/uri.h>

#include <libxslt/xsltconfig.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>

#include "exslt.h"

static void
exsltStrTokenizeFunction(xmlXPathParserContextPtr ctxt, int nargs)
{
    xsltTransformContextPtr tctxt;
    xmlChar *str, *delimiters, *cur;
    const xmlChar *token, *delimiter;
    xmlNodePtr node;
    xmlDocPtr container;
    xmlXPathObjectPtr ret = NULL;
    int clen;

    if ((nargs < 1) || (nargs > 2)) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    if (nargs == 2) {
        delimiters = xmlXPathPopString(ctxt);
        if (xmlXPathCheckError(ctxt))
            return;
    } else {
        delimiters = xmlStrdup((const xmlChar *) "\t\r\n ");
    }
    if (delimiters == NULL)
        return;

    str = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt) || (str == NULL)) {
        xmlFree(delimiters);
        return;
    }

    
    tctxt = xsltXPathGetTransformContext(ctxt);
    if (tctxt == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
	      "exslt:tokenize : internal error tctxt == NULL\n");
	goto fail;
    }

    container = xsltCreateRVT(tctxt);
    if (container != NULL) {
        xsltRegisterLocalRVT(tctxt, container);
        ret = xmlXPathNewNodeSet(NULL);
        if (ret != NULL) {
            for (cur = str, token = str; *cur != 0; cur += clen) {
	        clen = xmlUTF8Size(cur);
		if (*delimiters == 0) {	
		    xmlChar ctmp;
		    ctmp = *(cur+clen);
		    *(cur+clen) = 0;
                    node = xmlNewDocRawNode(container, NULL,
                                       (const xmlChar *) "token", cur);
		    xmlAddChild((xmlNodePtr) container, node);
		    xmlXPathNodeSetAddUnique(ret->nodesetval, node);
                    *(cur+clen) = ctmp; 
                    token = cur + clen;
                } else for (delimiter = delimiters; *delimiter != 0;
				delimiter += xmlUTF8Size(delimiter)) {
                    if (!xmlUTF8Charcmp(cur, delimiter)) {
                        if (cur == token) {
                            
                            token = cur + clen;
                            break;
                        }
                        *cur = 0;	
                        node = xmlNewDocRawNode(container, NULL,
                                           (const xmlChar *) "token", token);
			xmlAddChild((xmlNodePtr) container, node);
			xmlXPathNodeSetAddUnique(ret->nodesetval, node);
                        *cur = *delimiter; 
                        token = cur + clen;
                        break;
                    }
                }
            }
            if (token != cur) {
	    	node = xmlNewDocRawNode(container, NULL,
				    (const xmlChar *) "token", token);
                xmlAddChild((xmlNodePtr) container, node);
	        xmlXPathNodeSetAddUnique(ret->nodesetval, node);
            }
	    xsltExtensionInstructionResultRegister(tctxt, ret);
        }
    }

fail:
    if (str != NULL)
        xmlFree(str);
    if (delimiters != NULL)
        xmlFree(delimiters);
    if (ret != NULL)
        valuePush(ctxt, ret);
    else
        valuePush(ctxt, xmlXPathNewNodeSet(NULL));
}

static void
exsltStrSplitFunction(xmlXPathParserContextPtr ctxt, int nargs) {
    xsltTransformContextPtr tctxt;
    xmlChar *str, *delimiter, *cur;
    const xmlChar *token;
    xmlNodePtr node;
    xmlDocPtr container;
    xmlXPathObjectPtr ret = NULL;
    int delimiterLength;

    if ((nargs < 1) || (nargs > 2)) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    if (nargs == 2) {
        delimiter = xmlXPathPopString(ctxt);
        if (xmlXPathCheckError(ctxt))
            return;
    } else {
        delimiter = xmlStrdup((const xmlChar *) " ");
    }
    if (delimiter == NULL)
        return;
    delimiterLength = xmlStrlen (delimiter);

    str = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt) || (str == NULL)) {
        xmlFree(delimiter);
        return;
    }

    
    tctxt = xsltXPathGetTransformContext(ctxt);
    if (tctxt == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
	      "exslt:tokenize : internal error tctxt == NULL\n");
	goto fail;
    }

    container = xsltCreateRVT(tctxt);
    if (container != NULL) {
        xsltRegisterLocalRVT(tctxt, container);
        ret = xmlXPathNewNodeSet(NULL);
        if (ret != NULL) {
            for (cur = str, token = str; *cur != 0; cur++) {
		if (delimiterLength == 0) {
		    if (cur != token) {
			xmlChar tmp = *cur;
			*cur = 0;
                        node = xmlNewDocRawNode(container, NULL,
                                           (const xmlChar *) "token", token);
			xmlAddChild((xmlNodePtr) container, node);
			xmlXPathNodeSetAddUnique(ret->nodesetval, node);
			*cur = tmp;
			token++;
		    }
		}
		else if (!xmlStrncasecmp(cur, delimiter, delimiterLength)) {
		    if (cur == token) {
			
			cur = cur + delimiterLength - 1;
			token = cur + 1;
			continue;
		    }
		    *cur = 0;
		    node = xmlNewDocRawNode(container, NULL,
				       (const xmlChar *) "token", token);
		    xmlAddChild((xmlNodePtr) container, node);
		    xmlXPathNodeSetAddUnique(ret->nodesetval, node);
		    *cur = *delimiter;
		    cur = cur + delimiterLength - 1;
		    token = cur + 1;
                }
            }
	    if (token != cur) {
		node = xmlNewDocRawNode(container, NULL,
				   (const xmlChar *) "token", token);
		xmlAddChild((xmlNodePtr) container, node);
		xmlXPathNodeSetAddUnique(ret->nodesetval, node);
	    }
	    xsltExtensionInstructionResultRegister(tctxt, ret);
        }
    }

fail:
    if (str != NULL)
        xmlFree(str);
    if (delimiter != NULL)
        xmlFree(delimiter);
    if (ret != NULL)
        valuePush(ctxt, ret);
    else
        valuePush(ctxt, xmlXPathNewNodeSet(NULL));
}

static void
exsltStrEncodeUriFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    int escape_all = 1, str_len = 0;
    xmlChar *str = NULL, *ret = NULL, *tmp;

    if ((nargs < 2) || (nargs > 3)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs >= 3) {
        tmp = xmlXPathPopString(ctxt);
        if (xmlUTF8Strlen(tmp) != 5 || xmlStrcmp((const xmlChar *)"UTF-8",tmp)) {
	    xmlXPathReturnEmptyString(ctxt);
	    xmlFree(tmp);
	    return;
	}
	xmlFree(tmp);
    }

    escape_all = xmlXPathPopBoolean(ctxt);

    str = xmlXPathPopString(ctxt);
    str_len = xmlUTF8Strlen(str);

    if (str_len == 0) {
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	return;
    }

    ret = xmlURIEscapeStr(str,(const xmlChar *)(escape_all?"-_.!~*'()":"-_.!~*'();/?:@&=+$,[]"));
    xmlXPathReturnString(ctxt, ret);

    if (str != NULL)
	xmlFree(str);
}

static void
exsltStrDecodeUriFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    int str_len = 0;
    xmlChar *str = NULL, *ret = NULL, *tmp;

    if ((nargs < 1) || (nargs > 2)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs >= 2) {
        tmp = xmlXPathPopString(ctxt);
        if (xmlUTF8Strlen(tmp) != 5 || xmlStrcmp((const xmlChar *)"UTF-8",tmp)) {
	    xmlXPathReturnEmptyString(ctxt);
	    xmlFree(tmp);
	    return;
	}
	xmlFree(tmp);
    }

    str = xmlXPathPopString(ctxt);
    str_len = xmlUTF8Strlen(str);

    if (str_len == 0) {
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	return;
    }

    ret = (xmlChar *) xmlURIUnescapeString((const char *)str,0,NULL);
    if (!xmlCheckUTF8(ret)) {
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	xmlFree(ret);
	return;
    }
    
    xmlXPathReturnString(ctxt, ret);

    if (str != NULL)
	xmlFree(str);
}

static void
exsltStrPaddingFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    int number, str_len = 0;
    xmlChar *str = NULL, *ret = NULL, *tmp;

    if ((nargs < 1) || (nargs > 2)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 2) {
	str = xmlXPathPopString(ctxt);
	str_len = xmlUTF8Strlen(str);
    }
    if (str_len == 0) {
	if (str != NULL) xmlFree(str);
	str = xmlStrdup((const xmlChar *) " ");
	str_len = 1;
    }

    number = (int) xmlXPathPopNumber(ctxt);

    if (number <= 0) {
	xmlXPathReturnEmptyString(ctxt);
	xmlFree(str);
	return;
    }

    while (number >= str_len) {
	ret = xmlStrncat(ret, str, str_len);
	number -= str_len;
    }
    tmp = xmlUTF8Strndup (str, number);
    ret = xmlStrcat(ret, tmp);
    if (tmp != NULL)
	xmlFree (tmp);

    xmlXPathReturnString(ctxt, ret);

    if (str != NULL)
	xmlFree(str);
}

static void
exsltStrAlignFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlChar *str, *padding, *alignment, *ret;
    int str_l, padding_l;

    if ((nargs < 2) || (nargs > 3)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 3)
	alignment = xmlXPathPopString(ctxt);
    else
	alignment = NULL;

    padding = xmlXPathPopString(ctxt);
    str = xmlXPathPopString(ctxt);

    str_l = xmlUTF8Strlen (str);
    padding_l = xmlUTF8Strlen (padding);

    if (str_l == padding_l) {
	xmlXPathReturnString (ctxt, str);
	xmlFree(padding);
	xmlFree(alignment);
	return;
    }

    if (str_l > padding_l) {
	ret = xmlUTF8Strndup (str, padding_l);
    } else {
	if (xmlStrEqual(alignment, (const xmlChar *) "right")) {
	    ret = xmlUTF8Strndup (padding, padding_l - str_l);
	    ret = xmlStrcat (ret, str);
	} else if (xmlStrEqual(alignment, (const xmlChar *) "center")) {
	    int left = (padding_l - str_l) / 2;
	    int right_start;

	    ret = xmlUTF8Strndup (padding, left);
	    ret = xmlStrcat (ret, str);

	    right_start = xmlUTF8Strsize (padding, left + str_l);
	    ret = xmlStrcat (ret, padding + right_start);
	} else {
	    int str_s;

	    str_s = xmlStrlen (str);
	    ret = xmlStrdup (str);
	    ret = xmlStrcat (ret, padding + str_s);
	}
    }

    xmlXPathReturnString (ctxt, ret);

    xmlFree(str);
    xmlFree(padding);
    xmlFree(alignment);
}

static void
exsltStrConcatFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlXPathObjectPtr obj;
    xmlChar *ret = NULL;
    int i;

    if (nargs  != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (!xmlXPathStackIsNodeSet(ctxt)) {
	xmlXPathSetTypeError(ctxt);
	return;
    }

    obj = valuePop (ctxt);

    if (xmlXPathNodeSetIsEmpty(obj->nodesetval)) {
	xmlXPathReturnEmptyString(ctxt);
	return;
    }

    for (i = 0; i < obj->nodesetval->nodeNr; i++) {
	xmlChar *tmp;
	tmp = xmlXPathCastNodeToString(obj->nodesetval->nodeTab[i]);

	ret = xmlStrcat (ret, tmp);

	xmlFree(tmp);
    }

    xmlXPathFreeObject (obj);

    xmlXPathReturnString(ctxt, ret);
}

static xmlChar*
exsltStrReplaceInternal(const xmlChar* str, const xmlChar* searchStr, 
                        const xmlChar* replaceStr)
{
    const xmlChar *curr, *next;
    xmlChar *ret = NULL;
    int searchStrSize;

    curr = str;
    searchStrSize = xmlStrlen(searchStr);

    do {
      next = xmlStrstr(curr, searchStr);
      if (next == NULL) {
        ret = xmlStrcat (ret, curr);
        break;
      }

      ret = xmlStrncat (ret, curr, next - curr);
      ret = xmlStrcat (ret, replaceStr);
      curr = next + searchStrSize;
    } while (*curr != 0);

    return ret;
}
static void
exsltStrReplaceFunction (xmlXPathParserContextPtr ctxt, int nargs) {
    xmlChar *str = NULL, *searchStr = NULL, *replaceStr = NULL;
    xmlNodeSetPtr replaceSet = NULL, searchSet = NULL;
    xmlChar *ret = NULL, *retSwap = NULL;
    int i;

    if (nargs  != 3) {
      xmlXPathSetArityError(ctxt);
      return;
    }

    
    if (!xmlXPathStackIsNodeSet(ctxt)) {
      replaceStr = xmlXPathPopString(ctxt);
    }
		else {
      replaceSet = xmlXPathPopNodeSet(ctxt);
      if (xmlXPathCheckError(ctxt)) {
        xmlXPathSetTypeError(ctxt);
        goto fail;
      }
    }

    
    if (!xmlXPathStackIsNodeSet(ctxt)) {
      searchStr = xmlXPathPopString(ctxt);
      str = xmlXPathPopString(ctxt);

      if (replaceStr == NULL) {
        xmlXPathSetTypeError(ctxt);
        goto fail;
      }

      ret = exsltStrReplaceInternal(str, searchStr, replaceStr);
    }
		else {
      searchSet = xmlXPathPopNodeSet(ctxt);
      if (searchSet == NULL || xmlXPathCheckError(ctxt)) {
        xmlXPathSetTypeError(ctxt);
        goto fail;
      }

      str = xmlXPathPopString(ctxt);
      ret = xmlStrdup(str);

      for (i = 0; i < searchSet->nodeNr; i++) {
	searchStr = xmlXPathCastNodeToString(searchSet->nodeTab[i]);

        if (replaceSet != NULL) {
          replaceStr = NULL;
          if (i < replaceSet->nodeNr) {
            replaceStr = xmlXPathCastNodeToString(replaceSet->nodeTab[i]);
          }

          retSwap = exsltStrReplaceInternal(ret, searchStr, replaceStr);
          
          if (replaceStr != NULL) {
            xmlFree(replaceStr);
            replaceStr = NULL;
          }
        }
        else {
          retSwap = exsltStrReplaceInternal(ret, searchStr, replaceStr);
        }

				xmlFree(ret);
        if (searchStr != NULL) {
          xmlFree(searchStr);
          searchStr = NULL;
        }

				ret = retSwap;
			}

      if (replaceSet != NULL)
        xmlXPathFreeNodeSet(replaceSet);

      if (searchSet != NULL)
        xmlXPathFreeNodeSet(searchSet);
		}

    xmlXPathReturnString(ctxt, ret);

 fail:
    if (replaceStr != NULL)
      xmlFree(replaceStr);

    if (searchStr != NULL)
      xmlFree(searchStr);

    if (str != NULL)
      xmlFree(str);
}


void
exsltStrRegister (void) {
    xsltRegisterExtModuleFunction ((const xmlChar *) "tokenize",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrTokenizeFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "split",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrSplitFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "encode-uri",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrEncodeUriFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "decode-uri",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrDecodeUriFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "padding",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrPaddingFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "align",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrAlignFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "concat",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrConcatFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "replace",
				   EXSLT_STRINGS_NAMESPACE,
				   exsltStrReplaceFunction);
}

int
exsltStrXpathCtxtRegister (xmlXPathContextPtr ctxt, const xmlChar *prefix)
{
    if (ctxt
        && prefix
        && !xmlXPathRegisterNs(ctxt,
                               prefix,
                               (const xmlChar *) EXSLT_STRINGS_NAMESPACE)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "encode-uri",
                                   (const xmlChar *) EXSLT_STRINGS_NAMESPACE,
                                   exsltStrEncodeUriFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "decode-uri",
                                   (const xmlChar *) EXSLT_STRINGS_NAMESPACE,
                                   exsltStrDecodeUriFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "padding",
                                   (const xmlChar *) EXSLT_STRINGS_NAMESPACE,
                                   exsltStrPaddingFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "align",
                                   (const xmlChar *) EXSLT_STRINGS_NAMESPACE,
                                   exsltStrAlignFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "concat",
                                   (const xmlChar *) EXSLT_STRINGS_NAMESPACE,
                                   exsltStrConcatFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "replace",
                                   (const xmlChar *) EXSLT_STRINGS_NAMESPACE,
                                   exsltStrReplaceFunction)) {
        return 0;
    }
    return -1;
}
