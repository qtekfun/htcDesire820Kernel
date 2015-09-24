/**
 * uri.c: set of generic URI related routines 
 *
 * Reference: RFCs 3986, 2732 and 2373
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/uri.h>
#include <libxml/globals.h>
#include <libxml/xmlerror.h>

static void xmlCleanURI(xmlURIPtr uri);

#define IS_ALPHA(x) (IS_LOWALPHA(x) || IS_UPALPHA(x))



#define IS_LOWALPHA(x) (((x) >= 'a') && ((x) <= 'z'))

#define IS_UPALPHA(x) (((x) >= 'A') && ((x) <= 'Z'))

#ifdef IS_DIGIT
#undef IS_DIGIT
#endif
#define IS_DIGIT(x) (((x) >= '0') && ((x) <= '9'))


#define IS_ALPHANUM(x) (IS_ALPHA(x) || IS_DIGIT(x))


#define IS_MARK(x) (((x) == '-') || ((x) == '_') || ((x) == '.') ||     \
    ((x) == '!') || ((x) == '~') || ((x) == '*') || ((x) == '\'') ||    \
    ((x) == '(') || ((x) == ')'))


#define IS_UNWISE(p)                                                    \
      (((*(p) == '{')) || ((*(p) == '}')) || ((*(p) == '|')) ||         \
       ((*(p) == '\\')) || ((*(p) == '^')) || ((*(p) == '[')) ||        \
       ((*(p) == ']')) || ((*(p) == '`')))

#define IS_RESERVED(x) (((x) == ';') || ((x) == '/') || ((x) == '?') || \
        ((x) == ':') || ((x) == '@') || ((x) == '&') || ((x) == '=') || \
        ((x) == '+') || ((x) == '$') || ((x) == ',') || ((x) == '[') || \
        ((x) == ']'))


#define IS_UNRESERVED(x) (IS_ALPHANUM(x) || IS_MARK(x))


#define NEXT(p) ((*p == '%')? p += 3 : p++)


#define STRNDUP(s, n) (char *) xmlStrndup((const xmlChar *)(s), (n))


#define ISA_DIGIT(p) ((*(p) >= '0') && (*(p) <= '9'))
#define ISA_ALPHA(p) (((*(p) >= 'a') && (*(p) <= 'z')) ||		\
                      ((*(p) >= 'A') && (*(p) <= 'Z')))
#define ISA_HEXDIG(p)							\
       (ISA_DIGIT(p) || ((*(p) >= 'a') && (*(p) <= 'f')) ||		\
        ((*(p) >= 'A') && (*(p) <= 'F')))

#define ISA_SUB_DELIM(p)						\
      (((*(p) == '!')) || ((*(p) == '$')) || ((*(p) == '&')) ||		\
       ((*(p) == '(')) || ((*(p) == ')')) || ((*(p) == '*')) ||		\
       ((*(p) == '+')) || ((*(p) == ',')) || ((*(p) == ';')) ||		\
       ((*(p) == '=')))

#define ISA_GEN_DELIM(p)						\
      (((*(p) == ':')) || ((*(p) == '/')) || ((*(p) == '?')) ||         \
       ((*(p) == '#')) || ((*(p) == '[')) || ((*(p) == ']')) ||         \
       ((*(p) == '@')))

#define ISA_RESERVED(p) (ISA_GEN_DELIM(p) || (ISA_SUB_DELIM(p)))

#define ISA_UNRESERVED(p)						\
      ((ISA_ALPHA(p)) || (ISA_DIGIT(p)) || ((*(p) == '-')) ||		\
       ((*(p) == '.')) || ((*(p) == '_')) || ((*(p) == '~')))

#define ISA_PCT_ENCODED(p)						\
     ((*(p) == '%') && (ISA_HEXDIG(p + 1)) && (ISA_HEXDIG(p + 2)))

#define ISA_PCHAR(p)							\
     (ISA_UNRESERVED(p) || ISA_PCT_ENCODED(p) || ISA_SUB_DELIM(p) ||	\
      ((*(p) == ':')) || ((*(p) == '@')))

static int
xmlParse3986Scheme(xmlURIPtr uri, const char **str) {
    const char *cur;

    if (str == NULL)
	return(-1);

    cur = *str;
    if (!ISA_ALPHA(cur))
	return(2);
    cur++;
    while (ISA_ALPHA(cur) || ISA_DIGIT(cur) ||
           (*cur == '+') || (*cur == '-') || (*cur == '.')) cur++;
    if (uri != NULL) {
	if (uri->scheme != NULL) xmlFree(uri->scheme);
	uri->scheme = STRNDUP(*str, cur - *str);
    }
    *str = cur;
    return(0);
}

static int
xmlParse3986Fragment(xmlURIPtr uri, const char **str)
{
    const char *cur;

    if (str == NULL)
        return (-1);

    cur = *str;

    while ((ISA_PCHAR(cur)) || (*cur == '/') || (*cur == '?') ||
           (*cur == '[') || (*cur == ']') ||
           ((uri != NULL) && (uri->cleanup & 1) && (IS_UNWISE(cur))))
        NEXT(cur);
    if (uri != NULL) {
        if (uri->fragment != NULL)
            xmlFree(uri->fragment);
	if (uri->cleanup & 2)
	    uri->fragment = STRNDUP(*str, cur - *str);
	else
	    uri->fragment = xmlURIUnescapeString(*str, cur - *str, NULL);
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986Query(xmlURIPtr uri, const char **str)
{
    const char *cur;

    if (str == NULL)
        return (-1);

    cur = *str;

    while ((ISA_PCHAR(cur)) || (*cur == '/') || (*cur == '?') ||
           ((uri != NULL) && (uri->cleanup & 1) && (IS_UNWISE(cur))))
        NEXT(cur);
    if (uri != NULL) {
        if (uri->query != NULL)
            xmlFree(uri->query);
	if (uri->cleanup & 2)
	    uri->query = STRNDUP(*str, cur - *str);
	else
	    uri->query = xmlURIUnescapeString(*str, cur - *str, NULL);

	if (uri->query_raw != NULL)
	    xmlFree (uri->query_raw);
	uri->query_raw = STRNDUP (*str, cur - *str);
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986Port(xmlURIPtr uri, const char **str)
{
    const char *cur = *str;

    if (ISA_DIGIT(cur)) {
	if (uri != NULL)
	    uri->port = 0;
	while (ISA_DIGIT(cur)) {
	    if (uri != NULL)
		uri->port = uri->port * 10 + (*cur - '0');
	    cur++;
	}
	*str = cur;
	return(0);
    }
    return(1);
}

static int
xmlParse3986Userinfo(xmlURIPtr uri, const char **str)
{
    const char *cur;

    cur = *str;
    while (ISA_UNRESERVED(cur) || ISA_PCT_ENCODED(cur) ||
           ISA_SUB_DELIM(cur) || (*cur == ':'))
	NEXT(cur);
    if (*cur == '@') {
	if (uri != NULL) {
	    if (uri->user != NULL) xmlFree(uri->user);
	    if (uri->cleanup & 2)
		uri->user = STRNDUP(*str, cur - *str);
	    else
		uri->user = xmlURIUnescapeString(*str, cur - *str, NULL);
	}
	*str = cur;
	return(0);
    }
    return(1);
}

static int
xmlParse3986DecOctet(const char **str) {
    const char *cur = *str;

    if (!(ISA_DIGIT(cur)))
        return(1);
    if (!ISA_DIGIT(cur+1))
	cur++;
    else if ((*cur != '0') && (ISA_DIGIT(cur + 1)) && (!ISA_DIGIT(cur+2)))
	cur += 2;
    else if ((*cur == '1') && (ISA_DIGIT(cur + 1)) && (ISA_DIGIT(cur + 2)))
	cur += 3;
    else if ((*cur == '2') && (*(cur + 1) >= '0') &&
	     (*(cur + 1) <= '4') && (ISA_DIGIT(cur + 2)))
	cur += 3;
    else if ((*cur == '2') && (*(cur + 1) == '5') &&
	     (*(cur + 2) >= '0') && (*(cur + 1) <= '5'))
	cur += 3;
    else
        return(1);
    *str = cur;
    return(0);
}
static int
xmlParse3986Host(xmlURIPtr uri, const char **str)
{
    const char *cur = *str;
    const char *host;

    host = cur;
    if (*cur == '[') {
        cur++;
	while ((*cur != ']') && (*cur != 0))
	    cur++;
	if (*cur != ']')
	    return(1);
	cur++;
	goto found;
    }
    if (ISA_DIGIT(cur)) {
        if (xmlParse3986DecOctet(&cur) != 0)
	    goto not_ipv4;
	if (*cur != '.')
	    goto not_ipv4;
	cur++;
        if (xmlParse3986DecOctet(&cur) != 0)
	    goto not_ipv4;
	if (*cur != '.')
	    goto not_ipv4;
        if (xmlParse3986DecOctet(&cur) != 0)
	    goto not_ipv4;
	if (*cur != '.')
	    goto not_ipv4;
        if (xmlParse3986DecOctet(&cur) != 0)
	    goto not_ipv4;
	goto found;
not_ipv4:
        cur = *str;
    }
    while (ISA_UNRESERVED(cur) || ISA_PCT_ENCODED(cur) || ISA_SUB_DELIM(cur))
        NEXT(cur);
found:
    if (uri != NULL) {
	if (uri->authority != NULL) xmlFree(uri->authority);
	uri->authority = NULL;
	if (uri->server != NULL) xmlFree(uri->server);
	if (cur != host) {
	    if (uri->cleanup & 2)
		uri->server = STRNDUP(host, cur - host);
	    else
		uri->server = xmlURIUnescapeString(host, cur - host, NULL);
	} else
	    uri->server = NULL;
    }
    *str = cur;
    return(0);
}

static int
xmlParse3986Authority(xmlURIPtr uri, const char **str)
{
    const char *cur;
    int ret;

    cur = *str;
    ret = xmlParse3986Userinfo(uri, &cur);
    if ((ret != 0) || (*cur != '@'))
        cur = *str;
    else
        cur++;
    ret = xmlParse3986Host(uri, &cur);
    if (ret != 0) return(ret);
    if (*cur == ':') {
        cur++;
        ret = xmlParse3986Port(uri, &cur);
	if (ret != 0) return(ret);
    }
    *str = cur;
    return(0);
}

static int
xmlParse3986Segment(const char **str, char forbid, int empty)
{
    const char *cur;

    cur = *str;
    if (!ISA_PCHAR(cur)) {
        if (empty)
	    return(0);
	return(1);
    }
    while (ISA_PCHAR(cur) && (*cur != forbid))
        NEXT(cur);
    *str = cur;
    return (0);
}

static int
xmlParse3986PathAbEmpty(xmlURIPtr uri, const char **str)
{
    const char *cur;
    int ret;

    cur = *str;

    while (*cur == '/') {
        cur++;
	ret = xmlParse3986Segment(&cur, 0, 1);
	if (ret != 0) return(ret);
    }
    if (uri != NULL) {
	if (uri->path != NULL) xmlFree(uri->path);
        if (*str != cur) {
            if (uri->cleanup & 2)
                uri->path = STRNDUP(*str, cur - *str);
            else
                uri->path = xmlURIUnescapeString(*str, cur - *str, NULL);
        } else {
            uri->path = NULL;
        }
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986PathAbsolute(xmlURIPtr uri, const char **str)
{
    const char *cur;
    int ret;

    cur = *str;

    if (*cur != '/')
        return(1);
    cur++;
    ret = xmlParse3986Segment(&cur, 0, 0);
    if (ret == 0) {
	while (*cur == '/') {
	    cur++;
	    ret = xmlParse3986Segment(&cur, 0, 1);
	    if (ret != 0) return(ret);
	}
    }
    if (uri != NULL) {
	if (uri->path != NULL) xmlFree(uri->path);
        if (cur != *str) {
            if (uri->cleanup & 2)
                uri->path = STRNDUP(*str, cur - *str);
            else
                uri->path = xmlURIUnescapeString(*str, cur - *str, NULL);
        } else {
            uri->path = NULL;
        }
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986PathRootless(xmlURIPtr uri, const char **str)
{
    const char *cur;
    int ret;

    cur = *str;

    ret = xmlParse3986Segment(&cur, 0, 0);
    if (ret != 0) return(ret);
    while (*cur == '/') {
        cur++;
	ret = xmlParse3986Segment(&cur, 0, 1);
	if (ret != 0) return(ret);
    }
    if (uri != NULL) {
	if (uri->path != NULL) xmlFree(uri->path);
        if (cur != *str) {
            if (uri->cleanup & 2)
                uri->path = STRNDUP(*str, cur - *str);
            else
                uri->path = xmlURIUnescapeString(*str, cur - *str, NULL);
        } else {
            uri->path = NULL;
        }
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986PathNoScheme(xmlURIPtr uri, const char **str)
{
    const char *cur;
    int ret;

    cur = *str;

    ret = xmlParse3986Segment(&cur, ':', 0);
    if (ret != 0) return(ret);
    while (*cur == '/') {
        cur++;
	ret = xmlParse3986Segment(&cur, 0, 1);
	if (ret != 0) return(ret);
    }
    if (uri != NULL) {
	if (uri->path != NULL) xmlFree(uri->path);
        if (cur != *str) {
            if (uri->cleanup & 2)
                uri->path = STRNDUP(*str, cur - *str);
            else
                uri->path = xmlURIUnescapeString(*str, cur - *str, NULL);
        } else {
            uri->path = NULL;
        }
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986HierPart(xmlURIPtr uri, const char **str)
{
    const char *cur;
    int ret;

    cur = *str;

    if ((*cur == '/') && (*(cur + 1) == '/')) {
        cur += 2;
	ret = xmlParse3986Authority(uri, &cur);
	if (ret != 0) return(ret);
	ret = xmlParse3986PathAbEmpty(uri, &cur);
	if (ret != 0) return(ret);
	*str = cur;
	return(0);
    } else if (*cur == '/') {
        ret = xmlParse3986PathAbsolute(uri, &cur);
	if (ret != 0) return(ret);
    } else if (ISA_PCHAR(cur)) {
        ret = xmlParse3986PathRootless(uri, &cur);
	if (ret != 0) return(ret);
    } else {
	
	if (uri != NULL) {
	    if (uri->path != NULL) xmlFree(uri->path);
	    uri->path = NULL;
	}
    }
    *str = cur;
    return (0);
}

static int
xmlParse3986RelativeRef(xmlURIPtr uri, const char *str) {
    int ret;

    if ((*str == '/') && (*(str + 1) == '/')) {
        str += 2;
	ret = xmlParse3986Authority(uri, &str);
	if (ret != 0) return(ret);
	ret = xmlParse3986PathAbEmpty(uri, &str);
	if (ret != 0) return(ret);
    } else if (*str == '/') {
	ret = xmlParse3986PathAbsolute(uri, &str);
	if (ret != 0) return(ret);
    } else if (ISA_PCHAR(str)) {
        ret = xmlParse3986PathNoScheme(uri, &str);
	if (ret != 0) return(ret);
    } else {
	
	if (uri != NULL) {
	    if (uri->path != NULL) xmlFree(uri->path);
	    uri->path = NULL;
	}
    }

    if (*str == '?') {
	str++;
	ret = xmlParse3986Query(uri, &str);
	if (ret != 0) return(ret);
    }
    if (*str == '#') {
	str++;
	ret = xmlParse3986Fragment(uri, &str);
	if (ret != 0) return(ret);
    }
    if (*str != 0) {
	xmlCleanURI(uri);
	return(1);
    }
    return(0);
}


static int
xmlParse3986URI(xmlURIPtr uri, const char *str) {
    int ret;

    ret = xmlParse3986Scheme(uri, &str);
    if (ret != 0) return(ret);
    if (*str != ':') {
	return(1);
    }
    str++;
    ret = xmlParse3986HierPart(uri, &str);
    if (ret != 0) return(ret);
    if (*str == '?') {
	str++;
	ret = xmlParse3986Query(uri, &str);
	if (ret != 0) return(ret);
    }
    if (*str == '#') {
	str++;
	ret = xmlParse3986Fragment(uri, &str);
	if (ret != 0) return(ret);
    }
    if (*str != 0) {
	xmlCleanURI(uri);
	return(1);
    }
    return(0);
}

static int
xmlParse3986URIReference(xmlURIPtr uri, const char *str) {
    int ret;

    if (str == NULL)
	return(-1);
    xmlCleanURI(uri);

    ret = xmlParse3986URI(uri, str);
    if (ret != 0) {
	xmlCleanURI(uri);
        ret = xmlParse3986RelativeRef(uri, str);
	if (ret != 0) {
	    xmlCleanURI(uri);
	    return(ret);
	}
    }
    return(0);
}

xmlURIPtr
xmlParseURI(const char *str) {
    xmlURIPtr uri;
    int ret;

    if (str == NULL)
	return(NULL);
    uri = xmlCreateURI();
    if (uri != NULL) {
	ret = xmlParse3986URIReference(uri, str);
        if (ret) {
	    xmlFreeURI(uri);
	    return(NULL);
	}
    }
    return(uri);
}

int
xmlParseURIReference(xmlURIPtr uri, const char *str) {
    return(xmlParse3986URIReference(uri, str));
}

xmlURIPtr
xmlParseURIRaw(const char *str, int raw) {
    xmlURIPtr uri;
    int ret;

    if (str == NULL)
	return(NULL);
    uri = xmlCreateURI();
    if (uri != NULL) {
        if (raw) {
	    uri->cleanup |= 2;
	}
	ret = xmlParseURIReference(uri, str);
        if (ret) {
	    xmlFreeURI(uri);
	    return(NULL);
	}
    }
    return(uri);
}


xmlURIPtr
xmlCreateURI(void) {
    xmlURIPtr ret;

    ret = (xmlURIPtr) xmlMalloc(sizeof(xmlURI));
    if (ret == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlCreateURI: out of memory\n");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlURI));
    return(ret);
}

xmlChar *
xmlSaveUri(xmlURIPtr uri) {
    xmlChar *ret = NULL;
    xmlChar *temp;
    const char *p;
    int len;
    int max;

    if (uri == NULL) return(NULL);


    max = 80;
    ret = (xmlChar *) xmlMallocAtomic((max + 1) * sizeof(xmlChar));
    if (ret == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlSaveUri: out of memory\n");
	return(NULL);
    }
    len = 0;

    if (uri->scheme != NULL) {
	p = uri->scheme;
	while (*p != 0) {
	    if (len >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret, (max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
		    xmlGenericError(xmlGenericErrorContext,
			    "xmlSaveUri: out of memory\n");
		    xmlFree(ret);
		    return(NULL);
		}
		ret = temp;
	    }
	    ret[len++] = *p++;
	}
	if (len >= max) {
	    max *= 2;
	    temp = (xmlChar *) xmlRealloc(ret, (max + 1) * sizeof(xmlChar));
	    if (temp == NULL) {
		xmlGenericError(xmlGenericErrorContext,
			"xmlSaveUri: out of memory\n");
		xmlFree(ret);
		return(NULL);
	    }
	    ret = temp;
	}
	ret[len++] = ':';
    }
    if (uri->opaque != NULL) {
	p = uri->opaque;
	while (*p != 0) {
	    if (len + 3 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret, (max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
		    xmlGenericError(xmlGenericErrorContext,
			    "xmlSaveUri: out of memory\n");
		    xmlFree(ret);
		    return(NULL);
		}
		ret = temp;
	    }
	    if (IS_RESERVED(*(p)) || IS_UNRESERVED(*(p)))
		ret[len++] = *p++;
	    else {
		int val = *(unsigned char *)p++;
		int hi = val / 0x10, lo = val % 0x10;
		ret[len++] = '%';
		ret[len++] = hi + (hi > 9? 'A'-10 : '0');
		ret[len++] = lo + (lo > 9? 'A'-10 : '0');
	    }
	}
    } else {
	if (uri->server != NULL) {
	    if (len + 3 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret, (max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
		    xmlGenericError(xmlGenericErrorContext,
			    "xmlSaveUri: out of memory\n");
                  xmlFree(ret);  
		    return(NULL);
		}
		ret = temp;
	    }
	    ret[len++] = '/';
	    ret[len++] = '/';
	    if (uri->user != NULL) {
		p = uri->user;
		while (*p != 0) {
		    if (len + 3 >= max) {
			max *= 2;
			temp = (xmlChar *) xmlRealloc(ret,
				(max + 1) * sizeof(xmlChar));
			if (temp == NULL) {
			    xmlGenericError(xmlGenericErrorContext,
				    "xmlSaveUri: out of memory\n");
			    xmlFree(ret);
			    return(NULL);
			}
			ret = temp;
		    }
		    if ((IS_UNRESERVED(*(p))) ||
			((*(p) == ';')) || ((*(p) == ':')) ||
			((*(p) == '&')) || ((*(p) == '=')) ||
			((*(p) == '+')) || ((*(p) == '$')) ||
			((*(p) == ',')))
			ret[len++] = *p++;
		    else {
			int val = *(unsigned char *)p++;
			int hi = val / 0x10, lo = val % 0x10;
			ret[len++] = '%';
			ret[len++] = hi + (hi > 9? 'A'-10 : '0');
			ret[len++] = lo + (lo > 9? 'A'-10 : '0');
		    }
		}
		if (len + 3 >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
			xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		ret[len++] = '@';
	    }
	    p = uri->server;
	    while (*p != 0) {
		if (len >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
			xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		ret[len++] = *p++;
	    }
	    if (uri->port > 0) {
		if (len + 10 >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		len += snprintf((char *) &ret[len], max - len, ":%d", uri->port);
	    }
	} else if (uri->authority != NULL) {
	    if (len + 3 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret,
			(max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
	    }
	    ret[len++] = '/';
	    ret[len++] = '/';
	    p = uri->authority;
	    while (*p != 0) {
		if (len + 3 >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		if ((IS_UNRESERVED(*(p))) ||
                    ((*(p) == '$')) || ((*(p) == ',')) || ((*(p) == ';')) ||
                    ((*(p) == ':')) || ((*(p) == '@')) || ((*(p) == '&')) ||
                    ((*(p) == '=')) || ((*(p) == '+')))
		    ret[len++] = *p++;
		else {
		    int val = *(unsigned char *)p++;
		    int hi = val / 0x10, lo = val % 0x10;
		    ret[len++] = '%';
		    ret[len++] = hi + (hi > 9? 'A'-10 : '0');
		    ret[len++] = lo + (lo > 9? 'A'-10 : '0');
		}
	    }
	} else if (uri->scheme != NULL) {
	    if (len + 3 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret,
			(max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
	    }
	    ret[len++] = '/';
	    ret[len++] = '/';
	}
	if (uri->path != NULL) {
	    p = uri->path;
	    if ((uri->scheme != NULL) &&
		(p[0] == '/') &&
		(((p[1] >= 'a') && (p[1] <= 'z')) ||
		 ((p[1] >= 'A') && (p[1] <= 'Z'))) &&
		(p[2] == ':') &&
	        (xmlStrEqual(BAD_CAST uri->scheme, BAD_CAST "file"))) {
		if (len + 3 >= max) {
		    max *= 2;
		    ret = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (ret == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
			return(NULL);
		    }
		}
		ret[len++] = *p++;
		ret[len++] = *p++;
		ret[len++] = *p++;
	    }
	    while (*p != 0) {
		if (len + 3 >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		if ((IS_UNRESERVED(*(p))) || ((*(p) == '/')) ||
                    ((*(p) == ';')) || ((*(p) == '@')) || ((*(p) == '&')) ||
	            ((*(p) == '=')) || ((*(p) == '+')) || ((*(p) == '$')) ||
	            ((*(p) == ',')))
		    ret[len++] = *p++;
		else {
		    int val = *(unsigned char *)p++;
		    int hi = val / 0x10, lo = val % 0x10;
		    ret[len++] = '%';
		    ret[len++] = hi + (hi > 9? 'A'-10 : '0');
		    ret[len++] = lo + (lo > 9? 'A'-10 : '0');
		}
	    }
	}
	if (uri->query_raw != NULL) {
	    if (len + 1 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret,
			(max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
	    }
	    ret[len++] = '?';
	    p = uri->query_raw;
	    while (*p != 0) {
		if (len + 1 >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		ret[len++] = *p++;
	    }
	} else if (uri->query != NULL) {
	    if (len + 3 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret,
			(max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
	    }
	    ret[len++] = '?';
	    p = uri->query;
	    while (*p != 0) {
		if (len + 3 >= max) {
		    max *= 2;
		    temp = (xmlChar *) xmlRealloc(ret,
			    (max + 1) * sizeof(xmlChar));
		    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
		}
		if ((IS_UNRESERVED(*(p))) || (IS_RESERVED(*(p)))) 
		    ret[len++] = *p++;
		else {
		    int val = *(unsigned char *)p++;
		    int hi = val / 0x10, lo = val % 0x10;
		    ret[len++] = '%';
		    ret[len++] = hi + (hi > 9? 'A'-10 : '0');
		    ret[len++] = lo + (lo > 9? 'A'-10 : '0');
		}
	    }
	}
    }
    if (uri->fragment != NULL) {
	if (len + 3 >= max) {
	    max *= 2;
	    temp = (xmlChar *) xmlRealloc(ret,
		    (max + 1) * sizeof(xmlChar));
	    if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
	}
	ret[len++] = '#';
	p = uri->fragment;
	while (*p != 0) {
	    if (len + 3 >= max) {
		max *= 2;
		temp = (xmlChar *) xmlRealloc(ret,
			(max + 1) * sizeof(xmlChar));
		if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
	    }
	    if ((IS_UNRESERVED(*(p))) || (IS_RESERVED(*(p)))) 
		ret[len++] = *p++;
	    else {
		int val = *(unsigned char *)p++;
		int hi = val / 0x10, lo = val % 0x10;
		ret[len++] = '%';
		ret[len++] = hi + (hi > 9? 'A'-10 : '0');
		ret[len++] = lo + (lo > 9? 'A'-10 : '0');
	    }
	}
    }
    if (len >= max) {
	max *= 2;
	temp = (xmlChar *) xmlRealloc(ret, (max + 1) * sizeof(xmlChar));
	if (temp == NULL) {
			xmlGenericError(xmlGenericErrorContext,
				"xmlSaveUri: out of memory\n");
                     xmlFree(ret);
			return(NULL);
		    }
		    ret = temp;
    }
    ret[len] = 0;
    return(ret);
}

void
xmlPrintURI(FILE *stream, xmlURIPtr uri) {
    xmlChar *out;

    out = xmlSaveUri(uri);
    if (out != NULL) {
	fprintf(stream, "%s", (char *) out);
	xmlFree(out);
    }
}

static void
xmlCleanURI(xmlURIPtr uri) {
    if (uri == NULL) return;

    if (uri->scheme != NULL) xmlFree(uri->scheme);
    uri->scheme = NULL;
    if (uri->server != NULL) xmlFree(uri->server);
    uri->server = NULL;
    if (uri->user != NULL) xmlFree(uri->user);
    uri->user = NULL;
    if (uri->path != NULL) xmlFree(uri->path);
    uri->path = NULL;
    if (uri->fragment != NULL) xmlFree(uri->fragment);
    uri->fragment = NULL;
    if (uri->opaque != NULL) xmlFree(uri->opaque);
    uri->opaque = NULL;
    if (uri->authority != NULL) xmlFree(uri->authority);
    uri->authority = NULL;
    if (uri->query != NULL) xmlFree(uri->query);
    uri->query = NULL;
    if (uri->query_raw != NULL) xmlFree(uri->query_raw);
    uri->query_raw = NULL;
}

void
xmlFreeURI(xmlURIPtr uri) {
    if (uri == NULL) return;

    if (uri->scheme != NULL) xmlFree(uri->scheme);
    if (uri->server != NULL) xmlFree(uri->server);
    if (uri->user != NULL) xmlFree(uri->user);
    if (uri->path != NULL) xmlFree(uri->path);
    if (uri->fragment != NULL) xmlFree(uri->fragment);
    if (uri->opaque != NULL) xmlFree(uri->opaque);
    if (uri->authority != NULL) xmlFree(uri->authority);
    if (uri->query != NULL) xmlFree(uri->query);
    if (uri->query_raw != NULL) xmlFree(uri->query_raw);
    xmlFree(uri);
}


int
xmlNormalizeURIPath(char *path) {
    char *cur, *out;

    if (path == NULL)
	return(-1);

    cur = path;
    while (cur[0] == '/')
      ++cur;
    if (cur[0] == '\0')
      return(0);

    
    out = cur;

    while (cur[0] != '\0') {
	if ((cur[0] == '.') && (cur[1] == '/')) {
	    cur += 2;
	    
	    while (cur[0] == '/')
		cur++;
	    continue;
	}

	if ((cur[0] == '.') && (cur[1] == '\0'))
	    break;

	
	while (cur[0] != '/') {
            if (cur[0] == '\0')
              goto done_cd;
	    (out++)[0] = (cur++)[0];
	}
	
	while ((cur[0] == '/') && (cur[1] == '/'))
	    cur++;

        (out++)[0] = (cur++)[0];
    }
 done_cd:
    out[0] = '\0';

    
    cur = path;
    while (cur[0] == '/')
      ++cur;
    if (cur[0] == '\0')
	return(0);

    while (1) {
        char *segp, *tmp;


        
        segp = cur;
        while ((segp[0] != '/') && (segp[0] != '\0'))
          ++segp;

        if (segp[0] == '\0')
          break;

        ++segp;
        if (((cur[0] == '.') && (cur[1] == '.') && (segp == cur+3))
            || ((segp[0] != '.') || (segp[1] != '.')
                || ((segp[2] != '/') && (segp[2] != '\0')))) {
          cur = segp;
          continue;
        }


        
        if (segp[2] == '\0') {
          cur[0] = '\0';
          break;
        }
        
	
	tmp = cur;
	segp += 3;
	while ((*tmp++ = *segp++) != 0);

        
        segp = cur;
        while ((segp > path) && ((--segp)[0] == '/'))
          ;
        if (segp == path)
          continue;

        cur = segp;
        while ((cur > path) && (cur[-1] != '/'))
          --cur;
    }
    out[0] = '\0';

    if (path[0] == '/') {
      cur = path;
      while ((cur[0] == '/') && (cur[1] == '.') && (cur[2] == '.')
             && ((cur[3] == '/') || (cur[3] == '\0')))
	cur += 3;

      if (cur != path) {
	out = path;
	while (cur[0] != '\0')
          (out++)[0] = (cur++)[0];
	out[0] = 0;
      }
    }

    return(0);
}

static int is_hex(char c) {
    if (((c >= '0') && (c <= '9')) ||
        ((c >= 'a') && (c <= 'f')) ||
        ((c >= 'A') && (c <= 'F')))
	return(1);
    return(0);
}

char *
xmlURIUnescapeString(const char *str, int len, char *target) {
    char *ret, *out;
    const char *in;

    if (str == NULL)
	return(NULL);
    if (len <= 0) len = strlen(str);
    if (len < 0) return(NULL);

    if (target == NULL) {
	ret = (char *) xmlMallocAtomic(len + 1);
	if (ret == NULL) {
	    xmlGenericError(xmlGenericErrorContext,
		    "xmlURIUnescapeString: out of memory\n");
	    return(NULL);
	}
    } else
	ret = target;
    in = str;
    out = ret;
    while(len > 0) {
	if ((len > 2) && (*in == '%') && (is_hex(in[1])) && (is_hex(in[2]))) {
	    in++;
	    if ((*in >= '0') && (*in <= '9')) 
	        *out = (*in - '0');
	    else if ((*in >= 'a') && (*in <= 'f'))
	        *out = (*in - 'a') + 10;
	    else if ((*in >= 'A') && (*in <= 'F'))
	        *out = (*in - 'A') + 10;
	    in++;
	    if ((*in >= '0') && (*in <= '9')) 
	        *out = *out * 16 + (*in - '0');
	    else if ((*in >= 'a') && (*in <= 'f'))
	        *out = *out * 16 + (*in - 'a') + 10;
	    else if ((*in >= 'A') && (*in <= 'F'))
	        *out = *out * 16 + (*in - 'A') + 10;
	    in++;
	    len -= 3;
	    out++;
	} else {
	    *out++ = *in++;
	    len--;
	}
    }
    *out = 0;
    return(ret);
}

xmlChar *
xmlURIEscapeStr(const xmlChar *str, const xmlChar *list) {
    xmlChar *ret, ch;
    xmlChar *temp;
    const xmlChar *in;

    unsigned int len, out;

    if (str == NULL)
	return(NULL);
    if (str[0] == 0)
	return(xmlStrdup(str));
    len = xmlStrlen(str);
    if (!(len > 0)) return(NULL);

    len += 20;
    ret = (xmlChar *) xmlMallocAtomic(len);
    if (ret == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlURIEscapeStr: out of memory\n");
	return(NULL);
    }
    in = (const xmlChar *) str;
    out = 0;
    while(*in != 0) {
	if (len - out <= 3) {
	    len += 20;
	    temp = (xmlChar *) xmlRealloc(ret, len);
	    if (temp == NULL) {
		xmlGenericError(xmlGenericErrorContext,
			"xmlURIEscapeStr: out of memory\n");
		xmlFree(ret);
		return(NULL);
	    }
	    ret = temp;
	}

	ch = *in;

	if ((ch != '@') && (!IS_UNRESERVED(ch)) && (!xmlStrchr(list, ch))) {
	    unsigned char val;
	    ret[out++] = '%';
	    val = ch >> 4;
	    if (val <= 9)
		ret[out++] = '0' + val;
	    else
		ret[out++] = 'A' + val - 0xA;
	    val = ch & 0xF;
	    if (val <= 9)
		ret[out++] = '0' + val;
	    else
		ret[out++] = 'A' + val - 0xA;
	    in++;
	} else {
	    ret[out++] = *in++;
	}

    }
    ret[out] = 0;
    return(ret);
}

xmlChar *
xmlURIEscape(const xmlChar * str)
{
    xmlChar *ret, *segment = NULL;
    xmlURIPtr uri;
    int ret2;

#define NULLCHK(p) if(!p) { \
                   xmlGenericError(xmlGenericErrorContext, \
                        "xmlURIEscape: out of memory\n"); \
                        xmlFreeURI(uri); \
                        return NULL; } \

    if (str == NULL)
        return (NULL);

    uri = xmlCreateURI();
    if (uri != NULL) {
        uri->cleanup = 1;
        ret2 = xmlParseURIReference(uri, (const char *)str);
        if (ret2) {
            xmlFreeURI(uri);
            return (NULL);
        }
    }

    if (!uri)
        return NULL;

    ret = NULL;

    if (uri->scheme) {
        segment = xmlURIEscapeStr(BAD_CAST uri->scheme, BAD_CAST "+-.");
        NULLCHK(segment)
        ret = xmlStrcat(ret, segment);
        ret = xmlStrcat(ret, BAD_CAST ":");
        xmlFree(segment);
    }

    if (uri->authority) {
        segment =
            xmlURIEscapeStr(BAD_CAST uri->authority, BAD_CAST "/?;:@");
        NULLCHK(segment)
        ret = xmlStrcat(ret, BAD_CAST "//");
        ret = xmlStrcat(ret, segment);
        xmlFree(segment);
    }

    if (uri->user) {
        segment = xmlURIEscapeStr(BAD_CAST uri->user, BAD_CAST ";:&=+$,");
        NULLCHK(segment)
		ret = xmlStrcat(ret,BAD_CAST "//");	
        ret = xmlStrcat(ret, segment);
        ret = xmlStrcat(ret, BAD_CAST "@");
        xmlFree(segment);
    }

    if (uri->server) {
        segment = xmlURIEscapeStr(BAD_CAST uri->server, BAD_CAST "/?;:@");
        NULLCHK(segment)
		if (uri->user == NULL)
		ret = xmlStrcat(ret, BAD_CAST "//");
        ret = xmlStrcat(ret, segment);
        xmlFree(segment);
    }

    if (uri->port) {
        xmlChar port[10];

        snprintf((char *) port, 10, "%d", uri->port);
        ret = xmlStrcat(ret, BAD_CAST ":");
        ret = xmlStrcat(ret, port);
    }

    if (uri->path) {
        segment =
            xmlURIEscapeStr(BAD_CAST uri->path, BAD_CAST ":@&=+$,/?;");
        NULLCHK(segment)
        ret = xmlStrcat(ret, segment);
        xmlFree(segment);
    }

    if (uri->query_raw) {
        ret = xmlStrcat(ret, BAD_CAST "?");
        ret = xmlStrcat(ret, BAD_CAST uri->query_raw);
    }
    else if (uri->query) {
        segment =
            xmlURIEscapeStr(BAD_CAST uri->query, BAD_CAST ";/?:@&=+,$");
        NULLCHK(segment)
        ret = xmlStrcat(ret, BAD_CAST "?");
        ret = xmlStrcat(ret, segment);
        xmlFree(segment);
    }

    if (uri->opaque) {
        segment = xmlURIEscapeStr(BAD_CAST uri->opaque, BAD_CAST "");
        NULLCHK(segment)
        ret = xmlStrcat(ret, segment);
        xmlFree(segment);
    }

    if (uri->fragment) {
        segment = xmlURIEscapeStr(BAD_CAST uri->fragment, BAD_CAST "#");
        NULLCHK(segment)
        ret = xmlStrcat(ret, BAD_CAST "#");
        ret = xmlStrcat(ret, segment);
        xmlFree(segment);
    }

    xmlFreeURI(uri);
#undef NULLCHK

    return (ret);
}


xmlChar *
xmlBuildURI(const xmlChar *URI, const xmlChar *base) {
    xmlChar *val = NULL;
    int ret, len, indx, cur, out;
    xmlURIPtr ref = NULL;
    xmlURIPtr bas = NULL;
    xmlURIPtr res = NULL;

    if (URI == NULL) 
	ret = -1;
    else {
	if (*URI) {
	    ref = xmlCreateURI();
	    if (ref == NULL)
		goto done;
	    ret = xmlParseURIReference(ref, (const char *) URI);
	}
	else
	    ret = 0;
    }
    if (ret != 0)
	goto done;
    if ((ref != NULL) && (ref->scheme != NULL)) {
	val = xmlStrdup(URI);
	goto done;
    }
    if (base == NULL)
	ret = -1;
    else {
	bas = xmlCreateURI();
	if (bas == NULL)
	    goto done;
	ret = xmlParseURIReference(bas, (const char *) base);
    }
    if (ret != 0) {
	if (ref)
	    val = xmlSaveUri(ref);
	goto done;
    }
    if (ref == NULL) {
	if (bas->fragment != NULL) {
	    xmlFree(bas->fragment);
	    bas->fragment = NULL;
	}
	val = xmlSaveUri(bas);
	goto done;
    }

    res = xmlCreateURI();
    if (res == NULL)
	goto done;
    if ((ref->scheme == NULL) && (ref->path == NULL) &&
	((ref->authority == NULL) && (ref->server == NULL))) {
	if (bas->scheme != NULL)
	    res->scheme = xmlMemStrdup(bas->scheme);
	if (bas->authority != NULL)
	    res->authority = xmlMemStrdup(bas->authority);
	else if (bas->server != NULL) {
	    res->server = xmlMemStrdup(bas->server);
	    if (bas->user != NULL)
		res->user = xmlMemStrdup(bas->user);
	    res->port = bas->port;		
	}
	if (bas->path != NULL)
	    res->path = xmlMemStrdup(bas->path);
	if (ref->query_raw != NULL)
	    res->query_raw = xmlMemStrdup (ref->query_raw);
	else if (ref->query != NULL)
	    res->query = xmlMemStrdup(ref->query);
	else if (bas->query_raw != NULL)
	    res->query_raw = xmlMemStrdup(bas->query_raw);
	else if (bas->query != NULL)
	    res->query = xmlMemStrdup(bas->query);
	if (ref->fragment != NULL)
	    res->fragment = xmlMemStrdup(ref->fragment);
	goto step_7;
    }

    if (ref->scheme != NULL) {
	val = xmlSaveUri(ref);
	goto done;
    }
    if (bas->scheme != NULL)
	res->scheme = xmlMemStrdup(bas->scheme);
 
    if (ref->query_raw != NULL)
	res->query_raw = xmlMemStrdup(ref->query_raw);
    else if (ref->query != NULL)
	res->query = xmlMemStrdup(ref->query);
    if (ref->fragment != NULL)
	res->fragment = xmlMemStrdup(ref->fragment);

    if ((ref->authority != NULL) || (ref->server != NULL)) {
	if (ref->authority != NULL)
	    res->authority = xmlMemStrdup(ref->authority);
	else {
	    res->server = xmlMemStrdup(ref->server);
	    if (ref->user != NULL)
		res->user = xmlMemStrdup(ref->user);
            res->port = ref->port;		
	}
	if (ref->path != NULL)
	    res->path = xmlMemStrdup(ref->path);
	goto step_7;
    }
    if (bas->authority != NULL)
	res->authority = xmlMemStrdup(bas->authority);
    else if (bas->server != NULL) {
	res->server = xmlMemStrdup(bas->server);
	if (bas->user != NULL)
	    res->user = xmlMemStrdup(bas->user);
	res->port = bas->port;		
    }

    if ((ref->path != NULL) && (ref->path[0] == '/')) {
	res->path = xmlMemStrdup(ref->path);
	goto step_7;
    }


    len = 2; 
    if (ref->path != NULL)
	len += strlen(ref->path);
    if (bas->path != NULL)
	len += strlen(bas->path);
    res->path = (char *) xmlMallocAtomic(len);
    if (res->path == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlBuildURI: out of memory\n");
	goto done;
    }
    res->path[0] = 0;

    cur = 0;
    out = 0;
    if (bas->path != NULL) {
	while (bas->path[cur] != 0) {
	    while ((bas->path[cur] != 0) && (bas->path[cur] != '/'))
		cur++;
	    if (bas->path[cur] == 0)
		break;

	    cur++;
	    while (out < cur) {
		res->path[out] = bas->path[out];
		out++;
	    }
	}
    }
    res->path[out] = 0;

    if (ref->path != NULL && ref->path[0] != 0) {
	indx = 0;
	if ((out == 0) && (bas->server != NULL))
	    res->path[out++] = '/';
	while (ref->path[indx] != 0) {
	    res->path[out++] = ref->path[indx++];
	}
    }
    res->path[out] = 0;

    xmlNormalizeURIPath(res->path);

step_7:

    val = xmlSaveUri(res);

done:
    if (ref != NULL)
	xmlFreeURI(ref);
    if (bas != NULL)
	xmlFreeURI(bas);
    if (res != NULL)
	xmlFreeURI(res);
    return(val);
}

xmlChar *
xmlBuildRelativeURI (const xmlChar * URI, const xmlChar * base)
{
    xmlChar *val = NULL;
    int ret;
    int ix;
    int pos = 0;
    int nbslash = 0;
    int len;
    xmlURIPtr ref = NULL;
    xmlURIPtr bas = NULL;
    xmlChar *bptr, *uptr, *vptr;
    int remove_path = 0;

    if ((URI == NULL) || (*URI == 0))
	return NULL;

    ref = xmlCreateURI ();
    if (ref == NULL)
	return NULL;
    
    if (URI[0] != '.') {
	ret = xmlParseURIReference (ref, (const char *) URI);
	if (ret != 0)
	    goto done;		
    } else
	ref->path = (char *)xmlStrdup(URI);

    if ((base == NULL) || (*base == 0)) {
	val = xmlStrdup (URI);
	goto done;
    }
    bas = xmlCreateURI ();
    if (bas == NULL)
	goto done;
    if (base[0] != '.') {
	ret = xmlParseURIReference (bas, (const char *) base);
	if (ret != 0)
	    goto done;		
    } else
	bas->path = (char *)xmlStrdup(base);

    if ((ref->scheme != NULL) &&
	((bas->scheme == NULL) ||
	 (xmlStrcmp ((xmlChar *)bas->scheme, (xmlChar *)ref->scheme)) ||
	 (xmlStrcmp ((xmlChar *)bas->server, (xmlChar *)ref->server)))) {
	val = xmlStrdup (URI);
	goto done;
    }
    if (xmlStrEqual((xmlChar *)bas->path, (xmlChar *)ref->path)) {
	val = xmlStrdup(BAD_CAST "");
	goto done;
    }
    if (bas->path == NULL) {
	val = xmlStrdup((xmlChar *)ref->path);
	goto done;
    }
    if (ref->path == NULL) {
        ref->path = (char *) "/";
	remove_path = 1;
    }

    if (bas->path == NULL) {
	if (ref->path != NULL) {
	    uptr = (xmlChar *) ref->path;
	    if (*uptr == '/')
		uptr++;
	    
	    val = xmlURIEscapeStr(uptr, BAD_CAST "/;&=+$,");
	}
	goto done;
    }
    bptr = (xmlChar *)bas->path;
    if (ref->path == NULL) {
	for (ix = 0; bptr[ix] != 0; ix++) {
	    if (bptr[ix] == '/')
		nbslash++;
	}
	uptr = NULL;
	len = 1;	
    } else {
	if ((ref->path[pos] == '.') && (ref->path[pos+1] == '/'))
            pos += 2;
	if ((*bptr == '.') && (bptr[1] == '/'))
            bptr += 2;
	else if ((*bptr == '/') && (ref->path[pos] != '/'))
	    bptr++;
	while ((bptr[pos] == ref->path[pos]) && (bptr[pos] != 0))
	    pos++;

	if (bptr[pos] == ref->path[pos]) {
	    val = xmlStrdup(BAD_CAST "");
	    goto done;		
	}

	ix = pos;
	if ((ref->path[ix] == '/') && (ix > 0))
	    ix--;
	else if ((ref->path[ix] == 0) && (ix > 1) && (ref->path[ix - 1] == '/'))
	    ix -= 2;
	for (; ix > 0; ix--) {
	    if (ref->path[ix] == '/')
		break;
	}
	if (ix == 0) {
	    uptr = (xmlChar *)ref->path;
	} else {
	    ix++;
	    uptr = (xmlChar *)&ref->path[ix];
	}

	if (bptr[pos] != ref->path[pos]) {
	    for (; bptr[ix] != 0; ix++) {
		if (bptr[ix] == '/')
		    nbslash++;
	    }
	}
	len = xmlStrlen (uptr) + 1;
    }
    
    if (nbslash == 0) {
	if (uptr != NULL)
	    
	    val = xmlURIEscapeStr(uptr, BAD_CAST "/;&=+$,");
	goto done;
    }

    val = (xmlChar *) xmlMalloc (len + 3 * nbslash);
    if (val == NULL) {
	xmlGenericError(xmlGenericErrorContext,
		"xmlBuildRelativeURI: out of memory\n");
	goto done;
    }
    vptr = val;
    for (; nbslash>0; nbslash--) {
	*vptr++ = '.';
	*vptr++ = '.';
	*vptr++ = '/';
    }
    if (uptr != NULL) {
        if ((vptr > val) && (len > 0) &&
	    (uptr[0] == '/') && (vptr[-1] == '/')) {
	    memcpy (vptr, uptr + 1, len - 1);
	    vptr[len - 2] = 0;
	} else {
	    memcpy (vptr, uptr, len);
	    vptr[len - 1] = 0;
	}
    } else {
	vptr[len - 1] = 0;
    }

    
    vptr = val;
	
    val = xmlURIEscapeStr(vptr, BAD_CAST "/;&=+$,");
    xmlFree(vptr);

done:
    if (remove_path != 0)
        ref->path = NULL;
    if (ref != NULL)
	xmlFreeURI (ref);
    if (bas != NULL)
	xmlFreeURI (bas);

    return val;
}

#define IS_WINDOWS_PATH(p) 					\
	((p != NULL) &&						\
	 (((p[0] >= 'a') && (p[0] <= 'z')) ||			\
	  ((p[0] >= 'A') && (p[0] <= 'Z'))) &&			\
	 (p[1] == ':') && ((p[2] == '/') || (p[2] == '\\')))
xmlChar *
xmlCanonicPath(const xmlChar *path)
{
#if defined(_WIN32) && !defined(__CYGWIN__)    
    int len = 0;
    int i = 0;
    xmlChar *p = NULL;
#endif
    xmlURIPtr uri;
    xmlChar *ret;
    const xmlChar *absuri;

    if (path == NULL)
	return(NULL);

    
    if ((path[0] == '/') && (path[1] == '/') && (path[2] != '/'))
        path++;

    if ((uri = xmlParseURI((const char *) path)) != NULL) {
	xmlFreeURI(uri);
	return xmlStrdup(path);
    }

    
    absuri = xmlStrstr(path, BAD_CAST "://");
    if (absuri != NULL) {
        int l, j;
	unsigned char c;
	xmlChar *escURI;

	l = absuri - path;
	
	if ((l <= 0) || (l > 20))
	    goto path_processing;
	
	for (j = 0;j < l;j++) {
	    c = path[j];
	    if (!(((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))))
	        goto path_processing;
	}

	
        escURI = xmlURIEscapeStr(path, BAD_CAST ":/?_.#&;=");
	if (escURI != NULL) {
	    
	    uri = xmlParseURI((const char *) escURI);
	    
	    if (uri != NULL) {
	        xmlFreeURI(uri);
		return escURI;
	    }
	}
    }

path_processing:
#if defined(_WIN32) && !defined(__CYGWIN__)    
    uri = xmlCreateURI();
    if (uri == NULL) {		
        return(NULL);
    }

    len = xmlStrlen(path);
    if ((len > 2) && IS_WINDOWS_PATH(path)) {
        
	uri->scheme = xmlStrdup(BAD_CAST "file");
	
	uri->path = xmlMallocAtomic(len + 2);
	if (uri->path == NULL) {
	    xmlFreeURI(uri);	
	    return(NULL);
	}
	
	uri->path[0] = '/';
	p = uri->path + 1;
	strncpy(p, path, len + 1);
    } else {
	uri->path = xmlStrdup(path);
	if (uri->path == NULL) {
	    xmlFreeURI(uri);
	    return(NULL);
	}
	p = uri->path;
    }
    
    while (*p != '\0') {
	if (*p == '\\')
	    *p = '/';
	p++;
    }

    if (uri->scheme == NULL) {
	ret = xmlStrdup((const xmlChar *) uri->path);
    } else {
	ret = xmlSaveUri(uri);
    }

    xmlFreeURI(uri);
#else
    ret = xmlStrdup((const xmlChar *) path);
#endif
    return(ret);
}

xmlChar *
xmlPathToURI(const xmlChar *path)
{
    xmlURIPtr uri;
    xmlURI temp;
    xmlChar *ret, *cal;

    if (path == NULL)
        return(NULL);

    if ((uri = xmlParseURI((const char *) path)) != NULL) {
	xmlFreeURI(uri);
	return xmlStrdup(path);
    }
    cal = xmlCanonicPath(path);
    if (cal == NULL)
        return(NULL);
#if defined(_WIN32) && !defined(__CYGWIN__)
    if ((uri = xmlParseURI((const char *) cal)) != NULL) {
	xmlFreeURI(uri);
	return cal;
    }
    ret = cal;
    while (*ret != '\0') {
	if (*ret == '\\')
	    *ret = '/';
	ret++;
    }
#endif
    memset(&temp, 0, sizeof(temp));
    temp.path = (char *) cal;
    ret = xmlSaveUri(&temp);
    xmlFree(cal);
    return(ret);
}
#define bottom_uri
#include "elfgcchack.h"
