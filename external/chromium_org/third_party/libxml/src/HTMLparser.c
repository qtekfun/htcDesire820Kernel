/*
 * HTMLparser.c : an HTML 4.0 non-verifying parser
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"
#ifdef LIBXML_HTML_ENABLED

#include <string.h>
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ZLIB_H
#include <zlib.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlerror.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/entities.h>
#include <libxml/encoding.h>
#include <libxml/valid.h>
#include <libxml/xmlIO.h>
#include <libxml/globals.h>
#include <libxml/uri.h>

#define HTML_MAX_NAMELEN 1000
#define HTML_PARSER_BIG_BUFFER_SIZE 1000
#define HTML_PARSER_BUFFER_SIZE 100


static int htmlOmittedDefaultValue = 1;

xmlChar * htmlDecodeEntities(htmlParserCtxtPtr ctxt, int len,
			     xmlChar end, xmlChar  end2, xmlChar end3);
static void htmlParseComment(htmlParserCtxtPtr ctxt);


static void
htmlErrMemory(xmlParserCtxtPtr ctxt, const char *extra)
{
    if ((ctxt != NULL) && (ctxt->disableSAX != 0) &&
        (ctxt->instate == XML_PARSER_EOF))
	return;
    if (ctxt != NULL) {
        ctxt->errNo = XML_ERR_NO_MEMORY;
        ctxt->instate = XML_PARSER_EOF;
        ctxt->disableSAX = 1;
    }
    if (extra)
        __xmlRaiseError(NULL, NULL, NULL, ctxt, NULL, XML_FROM_PARSER,
                        XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0, extra,
                        NULL, NULL, 0, 0,
                        "Memory allocation failed : %s\n", extra);
    else
        __xmlRaiseError(NULL, NULL, NULL, ctxt, NULL, XML_FROM_PARSER,
                        XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0, NULL,
                        NULL, NULL, 0, 0, "Memory allocation failed\n");
}

static void
htmlParseErr(xmlParserCtxtPtr ctxt, xmlParserErrors error,
             const char *msg, const xmlChar *str1, const xmlChar *str2)
{
    if ((ctxt != NULL) && (ctxt->disableSAX != 0) &&
        (ctxt->instate == XML_PARSER_EOF))
	return;
    if (ctxt != NULL)
	ctxt->errNo = error;
    __xmlRaiseError(NULL, NULL, NULL, ctxt, NULL, XML_FROM_HTML, error,
                    XML_ERR_ERROR, NULL, 0,
		    (const char *) str1, (const char *) str2,
		    NULL, 0, 0,
		    msg, str1, str2);
    if (ctxt != NULL)
	ctxt->wellFormed = 0;
}

static void
htmlParseErrInt(xmlParserCtxtPtr ctxt, xmlParserErrors error,
             const char *msg, int val)
{
    if ((ctxt != NULL) && (ctxt->disableSAX != 0) &&
        (ctxt->instate == XML_PARSER_EOF))
	return;
    if (ctxt != NULL)
	ctxt->errNo = error;
    __xmlRaiseError(NULL, NULL, NULL, ctxt, NULL, XML_FROM_HTML, error,
                    XML_ERR_ERROR, NULL, 0, NULL, NULL,
		    NULL, val, 0, msg, val);
    if (ctxt != NULL)
	ctxt->wellFormed = 0;
}


static int
htmlnamePush(htmlParserCtxtPtr ctxt, const xmlChar * value)
{
    if ((ctxt->html < 3) && (xmlStrEqual(value, BAD_CAST "head")))
        ctxt->html = 3;
    if ((ctxt->html < 10) && (xmlStrEqual(value, BAD_CAST "body")))
        ctxt->html = 10;
    if (ctxt->nameNr >= ctxt->nameMax) {
        ctxt->nameMax *= 2;
        ctxt->nameTab = (const xmlChar * *)
                         xmlRealloc((xmlChar * *)ctxt->nameTab,
                                    ctxt->nameMax *
                                    sizeof(ctxt->nameTab[0]));
        if (ctxt->nameTab == NULL) {
            htmlErrMemory(ctxt, NULL);
            return (0);
        }
    }
    ctxt->nameTab[ctxt->nameNr] = value;
    ctxt->name = value;
    return (ctxt->nameNr++);
}
static const xmlChar *
htmlnamePop(htmlParserCtxtPtr ctxt)
{
    const xmlChar *ret;

    if (ctxt->nameNr <= 0)
        return (NULL);
    ctxt->nameNr--;
    if (ctxt->nameNr < 0)
        return (NULL);
    if (ctxt->nameNr > 0)
        ctxt->name = ctxt->nameTab[ctxt->nameNr - 1];
    else
        ctxt->name = NULL;
    ret = ctxt->nameTab[ctxt->nameNr];
    ctxt->nameTab[ctxt->nameNr] = NULL;
    return (ret);
}

static int
htmlNodeInfoPush(htmlParserCtxtPtr ctxt, htmlParserNodeInfo *value)
{
    if (ctxt->nodeInfoNr >= ctxt->nodeInfoMax) {
        if (ctxt->nodeInfoMax == 0)
                ctxt->nodeInfoMax = 5;
        ctxt->nodeInfoMax *= 2;
        ctxt->nodeInfoTab = (htmlParserNodeInfo *)
                         xmlRealloc((htmlParserNodeInfo *)ctxt->nodeInfoTab,
                                    ctxt->nodeInfoMax *
                                    sizeof(ctxt->nodeInfoTab[0]));
        if (ctxt->nodeInfoTab == NULL) {
            htmlErrMemory(ctxt, NULL);
            return (0);
        }
    }
    ctxt->nodeInfoTab[ctxt->nodeInfoNr] = *value;
    ctxt->nodeInfo = &ctxt->nodeInfoTab[ctxt->nodeInfoNr];
    return (ctxt->nodeInfoNr++);
}

static htmlParserNodeInfo *
htmlNodeInfoPop(htmlParserCtxtPtr ctxt)
{
    if (ctxt->nodeInfoNr <= 0)
        return (NULL);
    ctxt->nodeInfoNr--;
    if (ctxt->nodeInfoNr < 0)
        return (NULL);
    if (ctxt->nodeInfoNr > 0)
        ctxt->nodeInfo = &ctxt->nodeInfoTab[ctxt->nodeInfoNr - 1];
    else
        ctxt->nodeInfo = NULL;
    return &ctxt->nodeInfoTab[ctxt->nodeInfoNr];
}


#define UPPER (toupper(*ctxt->input->cur))

#define SKIP(val) ctxt->nbChars += (val),ctxt->input->cur += (val),ctxt->input->col+=(val)

#define NXT(val) ctxt->input->cur[(val)]

#define UPP(val) (toupper(ctxt->input->cur[(val)]))

#define CUR_PTR ctxt->input->cur

#define SHRINK if ((ctxt->input->cur - ctxt->input->base > 2 * INPUT_CHUNK) && \
		   (ctxt->input->end - ctxt->input->cur < 2 * INPUT_CHUNK)) \
	xmlParserInputShrink(ctxt->input)

#define GROW if ((ctxt->progressive == 0) &&				\
		 (ctxt->input->end - ctxt->input->cur < INPUT_CHUNK))	\
	xmlParserInputGrow(ctxt->input, INPUT_CHUNK)

#define CURRENT ((int) (*ctxt->input->cur))

#define SKIP_BLANKS htmlSkipBlankChars(ctxt)


#define CUR ((int) (*ctxt->input->cur))
#define NEXT xmlNextChar(ctxt)

#define RAW (ctxt->token ? -1 : (*ctxt->input->cur))


#define NEXTL(l) do {							\
    if (*(ctxt->input->cur) == '\n') {					\
	ctxt->input->line++; ctxt->input->col = 1;			\
    } else ctxt->input->col++;						\
    ctxt->token = 0; ctxt->input->cur += l; ctxt->nbChars++;		\
  } while (0)


#define CUR_CHAR(l) htmlCurrentChar(ctxt, &l)
#define CUR_SCHAR(s, l) xmlStringCurrentChar(ctxt, s, &l)

#define COPY_BUF(l,b,i,v)						\
    if (l == 1) b[i++] = (xmlChar) v;					\
    else i += xmlCopyChar(l,&b[i],v)

static xmlChar *
htmlFindEncoding(xmlParserCtxtPtr ctxt) {
    const xmlChar *start, *cur, *end;

    if ((ctxt == NULL) || (ctxt->input == NULL) ||
        (ctxt->input->encoding != NULL) || (ctxt->input->buf == NULL) ||
        (ctxt->input->buf->encoder != NULL))
        return(NULL);
    if ((ctxt->input->cur == NULL) || (ctxt->input->end == NULL))
        return(NULL);

    start = ctxt->input->cur;
    end = ctxt->input->end;
    
    if (*end != 0)
        return(NULL);

    cur = xmlStrcasestr(start, BAD_CAST "HTTP-EQUIV");
    if (cur == NULL)
        return(NULL);
    cur = xmlStrcasestr(cur, BAD_CAST  "CONTENT");
    if (cur == NULL)
        return(NULL);
    cur = xmlStrcasestr(cur, BAD_CAST  "CHARSET=");
    if (cur == NULL)
        return(NULL);
    cur += 8;
    start = cur;
    while (((*cur >= 'A') && (*cur <= 'Z')) ||
           ((*cur >= 'a') && (*cur <= 'z')) ||
           ((*cur >= '0') && (*cur <= '9')) ||
           (*cur == '-') || (*cur == '_') || (*cur == ':') || (*cur == '/'))
           cur++;
    if (cur == start)
        return(NULL);
    return(xmlStrndup(start, cur - start));
}


static int
htmlCurrentChar(xmlParserCtxtPtr ctxt, int *len) {
    if (ctxt->instate == XML_PARSER_EOF)
	return(0);

    if (ctxt->token != 0) {
	*len = 0;
	return(ctxt->token);
    }
    if (ctxt->charset == XML_CHAR_ENCODING_UTF8) {
	const unsigned char *cur = ctxt->input->cur;
	unsigned char c;
	unsigned int val;

	c = *cur;
	if (c & 0x80) {
	    if (cur[1] == 0) {
		xmlParserInputGrow(ctxt->input, INPUT_CHUNK);
                cur = ctxt->input->cur;
            }
	    if ((cur[1] & 0xc0) != 0x80)
		goto encoding_error;
	    if ((c & 0xe0) == 0xe0) {

		if (cur[2] == 0) {
		    xmlParserInputGrow(ctxt->input, INPUT_CHUNK);
                    cur = ctxt->input->cur;
                }
		if ((cur[2] & 0xc0) != 0x80)
		    goto encoding_error;
		if ((c & 0xf0) == 0xf0) {
		    if (cur[3] == 0) {
			xmlParserInputGrow(ctxt->input, INPUT_CHUNK);
                        cur = ctxt->input->cur;
                    }
		    if (((c & 0xf8) != 0xf0) ||
			((cur[3] & 0xc0) != 0x80))
			goto encoding_error;
		    
		    *len = 4;
		    val = (cur[0] & 0x7) << 18;
		    val |= (cur[1] & 0x3f) << 12;
		    val |= (cur[2] & 0x3f) << 6;
		    val |= cur[3] & 0x3f;
		} else {
		  
		    *len = 3;
		    val = (cur[0] & 0xf) << 12;
		    val |= (cur[1] & 0x3f) << 6;
		    val |= cur[2] & 0x3f;
		}
	    } else {
	      
		*len = 2;
		val = (cur[0] & 0x1f) << 6;
		val |= cur[1] & 0x3f;
	    }
	    if (!IS_CHAR(val)) {
	        htmlParseErrInt(ctxt, XML_ERR_INVALID_CHAR,
				"Char 0x%X out of allowed range\n", val);
	    }
	    return(val);
	} else {
            if ((*ctxt->input->cur == 0) &&
                (ctxt->input->cur < ctxt->input->end)) {
                    htmlParseErrInt(ctxt, XML_ERR_INVALID_CHAR,
				"Char 0x%X out of allowed range\n", 0);
                *len = 1;
                return(' ');
            }
	    
	    *len = 1;
	    return((int) *ctxt->input->cur);
	}
    }
    *len = 1;
    if ((int) *ctxt->input->cur < 0x80)
	return((int) *ctxt->input->cur);

    {
        xmlChar * guess;
        xmlCharEncodingHandlerPtr handler;

        guess = htmlFindEncoding(ctxt);
        if (guess == NULL) {
            xmlSwitchEncoding(ctxt, XML_CHAR_ENCODING_8859_1);
        } else {
            if (ctxt->input->encoding != NULL)
                xmlFree((xmlChar *) ctxt->input->encoding);
            ctxt->input->encoding = guess;
            handler = xmlFindCharEncodingHandler((const char *) guess);
            if (handler != NULL) {
                xmlSwitchToEncoding(ctxt, handler);
            } else {
                htmlParseErr(ctxt, XML_ERR_INVALID_ENCODING,
                             "Unsupported encoding %s", guess, NULL);
            }
        }
        ctxt->charset = XML_CHAR_ENCODING_UTF8;
    }

    return(xmlCurrentChar(ctxt, len));

encoding_error:
    {
        char buffer[150];

	if (ctxt->input->end - ctxt->input->cur >= 4) {
	    snprintf(buffer, 149, "Bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n",
			    ctxt->input->cur[0], ctxt->input->cur[1],
			    ctxt->input->cur[2], ctxt->input->cur[3]);
	} else {
	    snprintf(buffer, 149, "Bytes: 0x%02X\n", ctxt->input->cur[0]);
	}
	htmlParseErr(ctxt, XML_ERR_INVALID_ENCODING,
		     "Input is not proper UTF-8, indicate encoding !\n",
		     BAD_CAST buffer, NULL);
    }

    ctxt->charset = XML_CHAR_ENCODING_8859_1;
    *len = 1;
    return((int) *ctxt->input->cur);
}


static int
htmlSkipBlankChars(xmlParserCtxtPtr ctxt) {
    int res = 0;

    while (IS_BLANK_CH(*(ctxt->input->cur))) {
	if ((*ctxt->input->cur == 0) &&
	    (xmlParserInputGrow(ctxt->input, INPUT_CHUNK) <= 0)) {
		xmlPopInput(ctxt);
	} else {
	    if (*(ctxt->input->cur) == '\n') {
		ctxt->input->line++; ctxt->input->col = 1;
	    } else ctxt->input->col++;
	    ctxt->input->cur++;
	    ctxt->nbChars++;
	    if (*ctxt->input->cur == 0)
		xmlParserInputGrow(ctxt->input, INPUT_CHUNK);
	}
	res++;
    }
    return(res);
}






#define FONTSTYLE "tt", "i", "b", "u", "s", "strike", "big", "small"
#define NB_FONTSTYLE 8
#define PHRASE "em", "strong", "dfn", "code", "samp", "kbd", "var", "cite", "abbr", "acronym"
#define NB_PHRASE 10
#define SPECIAL "a", "img", "applet", "embed", "object", "font", "basefont", "br", "script", "map", "q", "sub", "sup", "span", "bdo", "iframe"
#define NB_SPECIAL 16
#define INLINE FONTSTYLE, PHRASE, SPECIAL, FORMCTRL
#define NB_INLINE NB_PCDATA + NB_FONTSTYLE + NB_PHRASE + NB_SPECIAL + NB_FORMCTRL
#define BLOCK HEADING, LIST, "pre", "p", "dl", "div", "center", "noscript", "noframes", "blockquote", "form", "isindex", "hr", "table", "fieldset", "address"
#define NB_BLOCK NB_HEADING + NB_LIST + 14
#define FORMCTRL "input", "select", "textarea", "label", "button"
#define NB_FORMCTRL 5
#define PCDATA
#define NB_PCDATA 0
#define HEADING "h1", "h2", "h3", "h4", "h5", "h6"
#define NB_HEADING 6
#define LIST "ul", "ol", "dir", "menu"
#define NB_LIST 4
#define MODIFIER
#define NB_MODIFIER 0
#define FLOW BLOCK,INLINE
#define NB_FLOW NB_BLOCK + NB_INLINE
#define EMPTY NULL


static const char* const html_flow[] = { FLOW, NULL } ;
static const char* const html_inline[] = { INLINE, NULL } ;

static const char* const html_pcdata[] = { NULL } ;
#define html_cdata html_pcdata



#define COREATTRS "id", "class", "style", "title"
#define NB_COREATTRS 4
#define I18N "lang", "dir"
#define NB_I18N 2
#define EVENTS "onclick", "ondblclick", "onmousedown", "onmouseup", "onmouseover", "onmouseout", "onkeypress", "onkeydown", "onkeyup"
#define NB_EVENTS 9
#define ATTRS COREATTRS,I18N,EVENTS
#define NB_ATTRS NB_NB_COREATTRS + NB_I18N + NB_EVENTS
#define CELLHALIGN "align", "char", "charoff"
#define NB_CELLHALIGN 3
#define CELLVALIGN "valign"
#define NB_CELLVALIGN 1

static const char* const html_attrs[] = { ATTRS, NULL } ;
static const char* const core_i18n_attrs[] = { COREATTRS, I18N, NULL } ;
static const char* const core_attrs[] = { COREATTRS, NULL } ;
static const char* const i18n_attrs[] = { I18N, NULL } ;


static const char* const a_attrs[] = { ATTRS, "charset", "type", "name",
	"href", "hreflang", "rel", "rev", "accesskey", "shape", "coords",
	"tabindex", "onfocus", "onblur", NULL } ;
static const char* const target_attr[] = { "target", NULL } ;
static const char* const rows_cols_attr[] = { "rows", "cols", NULL } ;
static const char* const alt_attr[] = { "alt", NULL } ;
static const char* const src_alt_attrs[] = { "src", "alt", NULL } ;
static const char* const href_attrs[] = { "href", NULL } ;
static const char* const clear_attrs[] = { "clear", NULL } ;
static const char* const inline_p[] = { INLINE, "p", NULL } ;

static const char* const flow_param[] = { FLOW, "param", NULL } ;
static const char* const applet_attrs[] = { COREATTRS , "codebase",
		"archive", "alt", "name", "height", "width", "align",
		"hspace", "vspace", NULL } ;
static const char* const area_attrs[] = { "shape", "coords", "href", "nohref",
	"tabindex", "accesskey", "onfocus", "onblur", NULL } ;
static const char* const basefont_attrs[] =
	{ "id", "size", "color", "face", NULL } ;
static const char* const quote_attrs[] = { ATTRS, "cite", NULL } ;
static const char* const body_contents[] = { FLOW, "ins", "del", NULL } ;
static const char* const body_attrs[] = { ATTRS, "onload", "onunload", NULL } ;
static const char* const body_depr[] = { "background", "bgcolor", "text",
	"link", "vlink", "alink", NULL } ;
static const char* const button_attrs[] = { ATTRS, "name", "value", "type",
	"disabled", "tabindex", "accesskey", "onfocus", "onblur", NULL } ;


static const char* const col_attrs[] = { ATTRS, "span", "width", CELLHALIGN, CELLVALIGN, NULL } ;
static const char* const col_elt[] = { "col", NULL } ;
static const char* const edit_attrs[] = { ATTRS, "datetime", "cite", NULL } ;
static const char* const compact_attrs[] = { ATTRS, "compact", NULL } ;
static const char* const dl_contents[] = { "dt", "dd", NULL } ;
static const char* const compact_attr[] = { "compact", NULL } ;
static const char* const label_attr[] = { "label", NULL } ;
static const char* const fieldset_contents[] = { FLOW, "legend" } ;
static const char* const font_attrs[] = { COREATTRS, I18N, "size", "color", "face" , NULL } ;
static const char* const form_contents[] = { HEADING, LIST, INLINE, "pre", "p", "div", "center", "noscript", "noframes", "blockquote", "isindex", "hr", "table", "fieldset", "address", NULL } ;
static const char* const form_attrs[] = { ATTRS, "method", "enctype", "accept", "name", "onsubmit", "onreset", "accept-charset", NULL } ;
static const char* const frame_attrs[] = { COREATTRS, "longdesc", "name", "src", "frameborder", "marginwidth", "marginheight", "noresize", "scrolling" , NULL } ;
static const char* const frameset_attrs[] = { COREATTRS, "rows", "cols", "onload", "onunload", NULL } ;
static const char* const frameset_contents[] = { "frameset", "frame", "noframes", NULL } ;
static const char* const head_attrs[] = { I18N, "profile", NULL } ;
static const char* const head_contents[] = { "title", "isindex", "base", "script", "style", "meta", "link", "object", NULL } ;
static const char* const hr_depr[] = { "align", "noshade", "size", "width", NULL } ;
static const char* const version_attr[] = { "version", NULL } ;
static const char* const html_content[] = { "head", "body", "frameset", NULL } ;
static const char* const iframe_attrs[] = { COREATTRS, "longdesc", "name", "src", "frameborder", "marginwidth", "marginheight", "scrolling", "align", "height", "width", NULL } ;
static const char* const img_attrs[] = { ATTRS, "longdesc", "name", "height", "width", "usemap", "ismap", NULL } ;
static const char* const embed_attrs[] = { COREATTRS, "align", "alt", "border", "code", "codebase", "frameborder", "height", "hidden", "hspace", "name", "palette", "pluginspace", "pluginurl", "src", "type", "units", "vspace", "width", NULL } ;
static const char* const input_attrs[] = { ATTRS, "type", "name", "value", "checked", "disabled", "readonly", "size", "maxlength", "src", "alt", "usemap", "ismap", "tabindex", "accesskey", "onfocus", "onblur", "onselect", "onchange", "accept", NULL } ;
static const char* const prompt_attrs[] = { COREATTRS, I18N, "prompt", NULL } ;
static const char* const label_attrs[] = { ATTRS, "for", "accesskey", "onfocus", "onblur", NULL } ;
static const char* const legend_attrs[] = { ATTRS, "accesskey", NULL } ;
static const char* const align_attr[] = { "align", NULL } ;
static const char* const link_attrs[] = { ATTRS, "charset", "href", "hreflang", "type", "rel", "rev", "media", NULL } ;
static const char* const map_contents[] = { BLOCK, "area", NULL } ;
static const char* const name_attr[] = { "name", NULL } ;
static const char* const action_attr[] = { "action", NULL } ;
static const char* const blockli_elt[] = { BLOCK, "li", NULL } ;
static const char* const meta_attrs[] = { I18N, "http-equiv", "name", "scheme", NULL } ;
static const char* const content_attr[] = { "content", NULL } ;
static const char* const type_attr[] = { "type", NULL } ;
static const char* const noframes_content[] = { "body", FLOW MODIFIER, NULL } ;
static const char* const object_contents[] = { FLOW, "param", NULL } ;
static const char* const object_attrs[] = { ATTRS, "declare", "classid", "codebase", "data", "type", "codetype", "archive", "standby", "height", "width", "usemap", "name", "tabindex", NULL } ;
static const char* const object_depr[] = { "align", "border", "hspace", "vspace", NULL } ;
static const char* const ol_attrs[] = { "type", "compact", "start", NULL} ;
static const char* const option_elt[] = { "option", NULL } ;
static const char* const optgroup_attrs[] = { ATTRS, "disabled", NULL } ;
static const char* const option_attrs[] = { ATTRS, "disabled", "label", "selected", "value", NULL } ;
static const char* const param_attrs[] = { "id", "value", "valuetype", "type", NULL } ;
static const char* const width_attr[] = { "width", NULL } ;
static const char* const pre_content[] = { PHRASE, "tt", "i", "b", "u", "s", "strike", "a", "br", "script", "map", "q", "span", "bdo", "iframe", NULL } ;
static const char* const script_attrs[] = { "charset", "src", "defer", "event", "for", NULL } ;
static const char* const language_attr[] = { "language", NULL } ;
static const char* const select_content[] = { "optgroup", "option", NULL } ;
static const char* const select_attrs[] = { ATTRS, "name", "size", "multiple", "disabled", "tabindex", "onfocus", "onblur", "onchange", NULL } ;
static const char* const style_attrs[] = { I18N, "media", "title", NULL } ;
static const char* const table_attrs[] = { ATTRS, "summary", "width", "border", "frame", "rules", "cellspacing", "cellpadding", "datapagesize", NULL } ;
static const char* const table_depr[] = { "align", "bgcolor", NULL } ;
static const char* const table_contents[] = { "caption", "col", "colgroup", "thead", "tfoot", "tbody", "tr", NULL} ;
static const char* const tr_elt[] = { "tr", NULL } ;
static const char* const talign_attrs[] = { ATTRS, CELLHALIGN, CELLVALIGN, NULL} ;
static const char* const th_td_depr[] = { "nowrap", "bgcolor", "width", "height", NULL } ;
static const char* const th_td_attr[] = { ATTRS, "abbr", "axis", "headers", "scope", "rowspan", "colspan", CELLHALIGN, CELLVALIGN, NULL } ;
static const char* const textarea_attrs[] = { ATTRS, "name", "disabled", "readonly", "tabindex", "accesskey", "onfocus", "onblur", "onselect", "onchange", NULL } ;
static const char* const tr_contents[] = { "th", "td", NULL } ;
static const char* const bgcolor_attr[] = { "bgcolor", NULL } ;
static const char* const li_elt[] = { "li", NULL } ;
static const char* const ul_depr[] = { "type", "compact", NULL} ;
static const char* const dir_attr[] = { "dir", NULL} ;

#define DECL (const char**)

static const htmlElemDesc
html40ElementTable[] = {
{ "a",		0, 0, 0, 0, 0, 0, 1, "anchor ",
	DECL html_inline , NULL , DECL a_attrs , DECL target_attr, NULL
},
{ "abbr",	0, 0, 0, 0, 0, 0, 1, "abbreviated form",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "acronym",	0, 0, 0, 0, 0, 0, 1, "",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "address",	0, 0, 0, 0, 0, 0, 0, "information on author ",
	DECL inline_p  , NULL , DECL html_attrs, NULL, NULL
},
{ "applet",	0, 0, 0, 0, 1, 1, 2, "java applet ",
	DECL flow_param , NULL , NULL , DECL applet_attrs, NULL
},
{ "area",	0, 2, 2, 1, 0, 0, 0, "client-side image map area ",
	EMPTY ,  NULL , DECL area_attrs , DECL target_attr, DECL alt_attr
},
{ "b",		0, 3, 0, 0, 0, 0, 1, "bold text style",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "base",	0, 2, 2, 1, 0, 0, 0, "document base uri ",
	EMPTY , NULL , NULL , DECL target_attr, DECL href_attrs
},
{ "basefont",	0, 2, 2, 1, 1, 1, 1, "base font size " ,
	EMPTY , NULL , NULL, DECL basefont_attrs, NULL
},
{ "bdo",	0, 0, 0, 0, 0, 0, 1, "i18n bidi over-ride ",
	DECL html_inline , NULL , DECL core_i18n_attrs, NULL, DECL dir_attr
},
{ "big",	0, 3, 0, 0, 0, 0, 1, "large text style",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "blockquote",	0, 0, 0, 0, 0, 0, 0, "long quotation ",
	DECL html_flow , NULL , DECL quote_attrs , NULL, NULL
},
{ "body",	1, 1, 0, 0, 0, 0, 0, "document body ",
	DECL body_contents , "div" , DECL body_attrs, DECL body_depr, NULL
},
{ "br",		0, 2, 2, 1, 0, 0, 1, "forced line break ",
	EMPTY , NULL , DECL core_attrs, DECL clear_attrs , NULL
},
{ "button",	0, 0, 0, 0, 0, 0, 2, "push button ",
	DECL html_flow MODIFIER , NULL , DECL button_attrs, NULL, NULL
},
{ "caption",	0, 0, 0, 0, 0, 0, 0, "table caption ",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "center",	0, 3, 0, 0, 1, 1, 0, "shorthand for div align=center ",
	DECL html_flow , NULL , NULL, DECL html_attrs, NULL
},
{ "cite",	0, 0, 0, 0, 0, 0, 1, "citation",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "code",	0, 0, 0, 0, 0, 0, 1, "computer code fragment",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "col",	0, 2, 2, 1, 0, 0, 0, "table column ",
	EMPTY , NULL , DECL col_attrs , NULL, NULL
},
{ "colgroup",	0, 1, 0, 0, 0, 0, 0, "table column group ",
	DECL col_elt , "col" , DECL col_attrs , NULL, NULL
},
{ "dd",		0, 1, 0, 0, 0, 0, 0, "definition description ",
	DECL html_flow , NULL , DECL html_attrs, NULL, NULL
},
{ "del",	0, 0, 0, 0, 0, 0, 2, "deleted text ",
	DECL html_flow , NULL , DECL edit_attrs , NULL, NULL
},
{ "dfn",	0, 0, 0, 0, 0, 0, 1, "instance definition",
	DECL html_inline , NULL , DECL html_attrs, NULL, NULL
},
{ "dir",	0, 0, 0, 0, 1, 1, 0, "directory list",
	DECL blockli_elt, "li" , NULL, DECL compact_attrs, NULL
},
{ "div",	0, 0, 0, 0, 0, 0, 0, "generic language/style container",
	DECL html_flow, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "dl",		0, 0, 0, 0, 0, 0, 0, "definition list ",
	DECL dl_contents , "dd" , DECL html_attrs, DECL compact_attr, NULL
},
{ "dt",		0, 1, 0, 0, 0, 0, 0, "definition term ",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "em",		0, 3, 0, 0, 0, 0, 1, "emphasis",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "embed",	0, 1, 0, 0, 1, 1, 1, "generic embedded object ",
	EMPTY, NULL, DECL embed_attrs, NULL, NULL
},
{ "fieldset",	0, 0, 0, 0, 0, 0, 0, "form control group ",
	DECL fieldset_contents , NULL, DECL html_attrs, NULL, NULL
},
{ "font",	0, 3, 0, 0, 1, 1, 1, "local change to font ",
	DECL html_inline, NULL, NULL, DECL font_attrs, NULL
},
{ "form",	0, 0, 0, 0, 0, 0, 0, "interactive form ",
	DECL form_contents, "fieldset", DECL form_attrs , DECL target_attr, DECL action_attr
},
{ "frame",	0, 2, 2, 1, 0, 2, 0, "subwindow " ,
	EMPTY, NULL, NULL, DECL frame_attrs, NULL
},
{ "frameset",	0, 0, 0, 0, 0, 2, 0, "window subdivision" ,
	DECL frameset_contents, "noframes" , NULL , DECL frameset_attrs, NULL
},
{ "h1",		0, 0, 0, 0, 0, 0, 0, "heading ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "h2",		0, 0, 0, 0, 0, 0, 0, "heading ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "h3",		0, 0, 0, 0, 0, 0, 0, "heading ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "h4",		0, 0, 0, 0, 0, 0, 0, "heading ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "h5",		0, 0, 0, 0, 0, 0, 0, "heading ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "h6",		0, 0, 0, 0, 0, 0, 0, "heading ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "head",	1, 1, 0, 0, 0, 0, 0, "document head ",
	DECL head_contents, NULL, DECL head_attrs, NULL, NULL
},
{ "hr",		0, 2, 2, 1, 0, 0, 0, "horizontal rule " ,
	EMPTY, NULL, DECL html_attrs, DECL hr_depr, NULL
},
{ "html",	1, 1, 0, 0, 0, 0, 0, "document root element ",
	DECL html_content , NULL , DECL i18n_attrs, DECL version_attr, NULL
},
{ "i",		0, 3, 0, 0, 0, 0, 1, "italic text style",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "iframe",	0, 0, 0, 0, 0, 1, 2, "inline subwindow ",
	DECL html_flow, NULL, NULL, DECL iframe_attrs, NULL
},
{ "img",	0, 2, 2, 1, 0, 0, 1, "embedded image ",
	EMPTY, NULL, DECL img_attrs, DECL align_attr, DECL src_alt_attrs
},
{ "input",	0, 2, 2, 1, 0, 0, 1, "form control ",
	EMPTY, NULL, DECL input_attrs , DECL align_attr, NULL
},
{ "ins",	0, 0, 0, 0, 0, 0, 2, "inserted text",
	DECL html_flow, NULL, DECL edit_attrs, NULL, NULL
},
{ "isindex",	0, 2, 2, 1, 1, 1, 0, "single line prompt ",
	EMPTY, NULL, NULL, DECL prompt_attrs, NULL
},
{ "kbd",	0, 0, 0, 0, 0, 0, 1, "text to be entered by the user",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "label",	0, 0, 0, 0, 0, 0, 1, "form field label text ",
	DECL html_inline MODIFIER, NULL, DECL label_attrs , NULL, NULL
},
{ "legend",	0, 0, 0, 0, 0, 0, 0, "fieldset legend ",
	DECL html_inline, NULL, DECL legend_attrs , DECL align_attr, NULL
},
{ "li",		0, 1, 1, 0, 0, 0, 0, "list item ",
	DECL html_flow, NULL, DECL html_attrs, NULL, NULL
},
{ "link",	0, 2, 2, 1, 0, 0, 0, "a media-independent link ",
	EMPTY, NULL, DECL link_attrs, DECL target_attr, NULL
},
{ "map",	0, 0, 0, 0, 0, 0, 2, "client-side image map ",
	DECL map_contents , NULL, DECL html_attrs , NULL, DECL name_attr
},
{ "menu",	0, 0, 0, 0, 1, 1, 0, "menu list ",
	DECL blockli_elt , NULL, NULL, DECL compact_attrs, NULL
},
{ "meta",	0, 2, 2, 1, 0, 0, 0, "generic metainformation ",
	EMPTY, NULL, DECL meta_attrs , NULL , DECL content_attr
},
{ "noframes",	0, 0, 0, 0, 0, 2, 0, "alternate content container for non frame-based rendering ",
	DECL noframes_content, "body" , DECL html_attrs, NULL, NULL
},
{ "noscript",	0, 0, 0, 0, 0, 0, 0, "alternate content container for non script-based rendering ",
	DECL html_flow, "div", DECL html_attrs, NULL, NULL
},
{ "object",	0, 0, 0, 0, 0, 0, 2, "generic embedded object ",
	DECL object_contents , "div" , DECL object_attrs, DECL object_depr, NULL
},
{ "ol",		0, 0, 0, 0, 0, 0, 0, "ordered list ",
	DECL li_elt , "li" , DECL html_attrs, DECL ol_attrs, NULL
},
{ "optgroup",	0, 0, 0, 0, 0, 0, 0, "option group ",
	DECL option_elt , "option", DECL optgroup_attrs, NULL, DECL label_attr
},
{ "option",	0, 1, 0, 0, 0, 0, 0, "selectable choice " ,
	DECL html_pcdata, NULL, DECL option_attrs, NULL, NULL
},
{ "p",		0, 1, 0, 0, 0, 0, 0, "paragraph ",
	DECL html_inline, NULL, DECL html_attrs, DECL align_attr, NULL
},
{ "param",	0, 2, 2, 1, 0, 0, 0, "named property value ",
	EMPTY, NULL, DECL param_attrs, NULL, DECL name_attr
},
{ "pre",	0, 0, 0, 0, 0, 0, 0, "preformatted text ",
	DECL pre_content, NULL, DECL html_attrs, DECL width_attr, NULL
},
{ "q",		0, 0, 0, 0, 0, 0, 1, "short inline quotation ",
	DECL html_inline, NULL, DECL quote_attrs, NULL, NULL
},
{ "s",		0, 3, 0, 0, 1, 1, 1, "strike-through text style",
	DECL html_inline, NULL, NULL, DECL html_attrs, NULL
},
{ "samp",	0, 0, 0, 0, 0, 0, 1, "sample program output, scripts, etc.",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "script",	0, 0, 0, 0, 0, 0, 2, "script statements ",
	DECL html_cdata, NULL, DECL script_attrs, DECL language_attr, DECL type_attr
},
{ "select",	0, 0, 0, 0, 0, 0, 1, "option selector ",
	DECL select_content, NULL, DECL select_attrs, NULL, NULL
},
{ "small",	0, 3, 0, 0, 0, 0, 1, "small text style",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "span",	0, 0, 0, 0, 0, 0, 1, "generic language/style container ",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "strike",	0, 3, 0, 0, 1, 1, 1, "strike-through text",
	DECL html_inline, NULL, NULL, DECL html_attrs, NULL
},
{ "strong",	0, 3, 0, 0, 0, 0, 1, "strong emphasis",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "style",	0, 0, 0, 0, 0, 0, 0, "style info ",
	DECL html_cdata, NULL, DECL style_attrs, NULL, DECL type_attr
},
{ "sub",	0, 3, 0, 0, 0, 0, 1, "subscript",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "sup",	0, 3, 0, 0, 0, 0, 1, "superscript ",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "table",	0, 0, 0, 0, 0, 0, 0, "",
	DECL table_contents , "tr" , DECL table_attrs , DECL table_depr, NULL
},
{ "tbody",	1, 0, 0, 0, 0, 0, 0, "table body ",
	DECL tr_elt , "tr" , DECL talign_attrs, NULL, NULL
},
{ "td",		0, 0, 0, 0, 0, 0, 0, "table data cell",
	DECL html_flow, NULL, DECL th_td_attr, DECL th_td_depr, NULL
},
{ "textarea",	0, 0, 0, 0, 0, 0, 1, "multi-line text field ",
	DECL html_pcdata, NULL, DECL textarea_attrs, NULL, DECL rows_cols_attr
},
{ "tfoot",	0, 1, 0, 0, 0, 0, 0, "table footer ",
	DECL tr_elt , "tr" , DECL talign_attrs, NULL, NULL
},
{ "th",		0, 1, 0, 0, 0, 0, 0, "table header cell",
	DECL html_flow, NULL, DECL th_td_attr, DECL th_td_depr, NULL
},
{ "thead",	0, 1, 0, 0, 0, 0, 0, "table header ",
	DECL tr_elt , "tr" , DECL talign_attrs, NULL, NULL
},
{ "title",	0, 0, 0, 0, 0, 0, 0, "document title ",
	DECL html_pcdata, NULL, DECL i18n_attrs, NULL, NULL
},
{ "tr",		0, 0, 0, 0, 0, 0, 0, "table row ",
	DECL tr_contents , "td" , DECL talign_attrs, DECL bgcolor_attr, NULL
},
{ "tt",		0, 3, 0, 0, 0, 0, 1, "teletype or monospaced text style",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
},
{ "u",		0, 3, 0, 0, 1, 1, 1, "underlined text style",
	DECL html_inline, NULL, NULL, DECL html_attrs, NULL
},
{ "ul",		0, 0, 0, 0, 0, 0, 0, "unordered list ",
	DECL li_elt , "li" , DECL html_attrs, DECL ul_depr, NULL
},
{ "var",	0, 0, 0, 0, 0, 0, 1, "instance of a variable or program argument",
	DECL html_inline, NULL, DECL html_attrs, NULL, NULL
}
};

static const char * const htmlStartClose[] = {
"form",		"form", "p", "hr", "h1", "h2", "h3", "h4", "h5", "h6",
		"dl", "ul", "ol", "menu", "dir", "address", "pre",
		"listing", "xmp", "head", NULL,
"head",		"p", NULL,
"title",	"p", NULL,
"body",		"head", "style", "link", "title", "p", NULL,
"frameset",	"head", "style", "link", "title", "p", NULL,
"li",		"p", "h1", "h2", "h3", "h4", "h5", "h6", "dl", "address",
		"pre", "listing", "xmp", "head", "li", NULL,
"hr",		"p", "head", NULL,
"h1",		"p", "head", NULL,
"h2",		"p", "head", NULL,
"h3",		"p", "head", NULL,
"h4",		"p", "head", NULL,
"h5",		"p", "head", NULL,
"h6",		"p", "head", NULL,
"dir",		"p", "head", NULL,
"address",	"p", "head", "ul", NULL,
"pre",		"p", "head", "ul", NULL,
"listing",	"p", "head", NULL,
"xmp",		"p", "head", NULL,
"blockquote",	"p", "head", NULL,
"dl",		"p", "dt", "menu", "dir", "address", "pre", "listing",
		"xmp", "head", NULL,
"dt",		"p", "menu", "dir", "address", "pre", "listing", "xmp",
                "head", "dd", NULL,
"dd",		"p", "menu", "dir", "address", "pre", "listing", "xmp",
                "head", "dt", NULL,
"ul",		"p", "head", "ol", "menu", "dir", "address", "pre",
		"listing", "xmp", NULL,
"ol",		"p", "head", "ul", NULL,
"menu",		"p", "head", "ul", NULL,
"p",		"p", "head", "h1", "h2", "h3", "h4", "h5", "h6", FONTSTYLE, NULL,
"div",		"p", "head", NULL,
"noscript",	"p", "head", NULL,
"center",	"font", "b", "i", "p", "head", NULL,
"a",		"a", NULL,
"caption",	"p", NULL,
"colgroup",	"caption", "colgroup", "col", "p", NULL,
"col",		"caption", "col", "p", NULL,
"table",	"p", "head", "h1", "h2", "h3", "h4", "h5", "h6", "pre",
		"listing", "xmp", "a", NULL,
"th",		"th", "td", "p", "span", "font", "a", "b", "i", "u", NULL,
"td",		"th", "td", "p", "span", "font", "a", "b", "i", "u", NULL,
"tr",		"th", "td", "tr", "caption", "col", "colgroup", "p", NULL,
"thead",	"caption", "col", "colgroup", NULL,
"tfoot",	"th", "td", "tr", "caption", "col", "colgroup", "thead",
		"tbody", "p", NULL,
"tbody",	"th", "td", "tr", "caption", "col", "colgroup", "thead",
		"tfoot", "tbody", "p", NULL,
"optgroup",	"option", NULL,
"option",	"option", NULL,
"fieldset",	"legend", "p", "head", "h1", "h2", "h3", "h4", "h5", "h6",
		"pre", "listing", "xmp", "a", NULL,
NULL
};

static const char *const htmlNoContentElements[] = {
    "html",
    "head",
    NULL
};

static const char *const htmlScriptAttributes[] = {
    "onclick",
    "ondblclick",
    "onmousedown",
    "onmouseup",
    "onmouseover",
    "onmousemove",
    "onmouseout",
    "onkeypress",
    "onkeydown",
    "onkeyup",
    "onload",
    "onunload",
    "onfocus",
    "onblur",
    "onsubmit",
    "onrest",
    "onchange",
    "onselect"
};


typedef struct {
    const char *name;
    int priority;
} elementPriority;

static const elementPriority htmlEndPriority[] = {
    {"div",   150},
    {"td",    160},
    {"th",    160},
    {"tr",    170},
    {"thead", 180},
    {"tbody", 180},
    {"tfoot", 180},
    {"table", 190},
    {"head",  200},
    {"body",  200},
    {"html",  220},
    {NULL,    100} 
};

static const char** htmlStartCloseIndex[100];
static int htmlStartCloseIndexinitialized = 0;


void
htmlInitAutoClose(void) {
    int indx, i = 0;

    if (htmlStartCloseIndexinitialized) return;

    for (indx = 0;indx < 100;indx ++) htmlStartCloseIndex[indx] = NULL;
    indx = 0;
    while ((htmlStartClose[i] != NULL) && (indx < 100 - 1)) {
        htmlStartCloseIndex[indx++] = (const char**) &htmlStartClose[i];
	while (htmlStartClose[i] != NULL) i++;
	i++;
    }
    htmlStartCloseIndexinitialized = 1;
}

const htmlElemDesc *
htmlTagLookup(const xmlChar *tag) {
    unsigned int i;

    for (i = 0; i < (sizeof(html40ElementTable) /
                     sizeof(html40ElementTable[0]));i++) {
        if (!xmlStrcasecmp(tag, BAD_CAST html40ElementTable[i].name))
	    return((htmlElemDescPtr) &html40ElementTable[i]);
    }
    return(NULL);
}

static int
htmlGetEndPriority (const xmlChar *name) {
    int i = 0;

    while ((htmlEndPriority[i].name != NULL) &&
	   (!xmlStrEqual((const xmlChar *)htmlEndPriority[i].name, name)))
	i++;

    return(htmlEndPriority[i].priority);
}


static int
htmlCheckAutoClose(const xmlChar * newtag, const xmlChar * oldtag)
{
    int i, indx;
    const char **closed = NULL;

    if (htmlStartCloseIndexinitialized == 0)
        htmlInitAutoClose();

    
    for (indx = 0; indx < 100; indx++) {
        closed = htmlStartCloseIndex[indx];
        if (closed == NULL)
            return (0);
        if (xmlStrEqual(BAD_CAST * closed, newtag))
            break;
    }

    i = closed - htmlStartClose;
    i++;
    while (htmlStartClose[i] != NULL) {
        if (xmlStrEqual(BAD_CAST htmlStartClose[i], oldtag)) {
            return (1);
        }
        i++;
    }
    return (0);
}

static void
htmlAutoCloseOnClose(htmlParserCtxtPtr ctxt, const xmlChar * newtag)
{
    const htmlElemDesc *info;
    int i, priority;

    priority = htmlGetEndPriority(newtag);

    for (i = (ctxt->nameNr - 1); i >= 0; i--) {

        if (xmlStrEqual(newtag, ctxt->nameTab[i]))
            break;
        if (htmlGetEndPriority(ctxt->nameTab[i]) > priority)
            return;
    }
    if (i < 0)
        return;

    while (!xmlStrEqual(newtag, ctxt->name)) {
        info = htmlTagLookup(ctxt->name);
        if ((info != NULL) && (info->endTag == 3)) {
            htmlParseErr(ctxt, XML_ERR_TAG_NAME_MISMATCH,
	                 "Opening and ending tag mismatch: %s and %s\n",
			 newtag, ctxt->name);
        }
        if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
            ctxt->sax->endElement(ctxt->userData, ctxt->name);
	htmlnamePop(ctxt);
    }
}

static void
htmlAutoCloseOnEnd(htmlParserCtxtPtr ctxt)
{
    int i;

    if (ctxt->nameNr == 0)
        return;
    for (i = (ctxt->nameNr - 1); i >= 0; i--) {
        if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
            ctxt->sax->endElement(ctxt->userData, ctxt->name);
	htmlnamePop(ctxt);
    }
}

static void
htmlAutoClose(htmlParserCtxtPtr ctxt, const xmlChar * newtag)
{
    while ((newtag != NULL) && (ctxt->name != NULL) &&
           (htmlCheckAutoClose(newtag, ctxt->name))) {
        if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
            ctxt->sax->endElement(ctxt->userData, ctxt->name);
	htmlnamePop(ctxt);
    }
    if (newtag == NULL) {
        htmlAutoCloseOnEnd(ctxt);
        return;
    }
    while ((newtag == NULL) && (ctxt->name != NULL) &&
           ((xmlStrEqual(ctxt->name, BAD_CAST "head")) ||
            (xmlStrEqual(ctxt->name, BAD_CAST "body")) ||
            (xmlStrEqual(ctxt->name, BAD_CAST "html")))) {
        if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
            ctxt->sax->endElement(ctxt->userData, ctxt->name);
	htmlnamePop(ctxt);
    }
}

int
htmlAutoCloseTag(htmlDocPtr doc, const xmlChar *name, htmlNodePtr elem) {
    htmlNodePtr child;

    if (elem == NULL) return(1);
    if (xmlStrEqual(name, elem->name)) return(0);
    if (htmlCheckAutoClose(elem->name, name)) return(1);
    child = elem->children;
    while (child != NULL) {
        if (htmlAutoCloseTag(doc, name, child)) return(1);
	child = child->next;
    }
    return(0);
}

int
htmlIsAutoClosed(htmlDocPtr doc, htmlNodePtr elem) {
    htmlNodePtr child;

    if (elem == NULL) return(1);
    child = elem->children;
    while (child != NULL) {
	if (htmlAutoCloseTag(doc, elem->name, child)) return(1);
	child = child->next;
    }
    return(0);
}

static void
htmlCheckImplied(htmlParserCtxtPtr ctxt, const xmlChar *newtag) {
    int i;

    if (ctxt->options & HTML_PARSE_NOIMPLIED)
        return;
    if (!htmlOmittedDefaultValue)
	return;
    if (xmlStrEqual(newtag, BAD_CAST"html"))
	return;
    if (ctxt->nameNr <= 0) {
	htmlnamePush(ctxt, BAD_CAST"html");
	if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL))
	    ctxt->sax->startElement(ctxt->userData, BAD_CAST"html", NULL);
    }
    if ((xmlStrEqual(newtag, BAD_CAST"body")) || (xmlStrEqual(newtag, BAD_CAST"head")))
        return;
    if ((ctxt->nameNr <= 1) &&
        ((xmlStrEqual(newtag, BAD_CAST"script")) ||
	 (xmlStrEqual(newtag, BAD_CAST"style")) ||
	 (xmlStrEqual(newtag, BAD_CAST"meta")) ||
	 (xmlStrEqual(newtag, BAD_CAST"link")) ||
	 (xmlStrEqual(newtag, BAD_CAST"title")) ||
	 (xmlStrEqual(newtag, BAD_CAST"base")))) {
        if (ctxt->html >= 3) {
            
            return;
        }
        htmlnamePush(ctxt, BAD_CAST"head");
        if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL))
            ctxt->sax->startElement(ctxt->userData, BAD_CAST"head", NULL);
    } else if ((!xmlStrEqual(newtag, BAD_CAST"noframes")) &&
	       (!xmlStrEqual(newtag, BAD_CAST"frame")) &&
	       (!xmlStrEqual(newtag, BAD_CAST"frameset"))) {
        if (ctxt->html >= 10) {
            
            return;
        }
	for (i = 0;i < ctxt->nameNr;i++) {
	    if (xmlStrEqual(ctxt->nameTab[i], BAD_CAST"body")) {
		return;
	    }
	    if (xmlStrEqual(ctxt->nameTab[i], BAD_CAST"head")) {
		return;
	    }
	}

	htmlnamePush(ctxt, BAD_CAST"body");
	if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL))
	    ctxt->sax->startElement(ctxt->userData, BAD_CAST"body", NULL);
    }
}


static int
htmlCheckParagraph(htmlParserCtxtPtr ctxt) {
    const xmlChar *tag;
    int i;

    if (ctxt == NULL)
	return(-1);
    tag = ctxt->name;
    if (tag == NULL) {
	htmlAutoClose(ctxt, BAD_CAST"p");
	htmlCheckImplied(ctxt, BAD_CAST"p");
	htmlnamePush(ctxt, BAD_CAST"p");
	if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL))
	    ctxt->sax->startElement(ctxt->userData, BAD_CAST"p", NULL);
	return(1);
    }
    if (!htmlOmittedDefaultValue)
	return(0);
    for (i = 0; htmlNoContentElements[i] != NULL; i++) {
	if (xmlStrEqual(tag, BAD_CAST htmlNoContentElements[i])) {
	    htmlAutoClose(ctxt, BAD_CAST"p");
	    htmlCheckImplied(ctxt, BAD_CAST"p");
	    htmlnamePush(ctxt, BAD_CAST"p");
	    if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL))
		ctxt->sax->startElement(ctxt->userData, BAD_CAST"p", NULL);
	    return(1);
	}
    }
    return(0);
}

int
htmlIsScriptAttribute(const xmlChar *name) {
    unsigned int i;

    if (name == NULL)
      return(0);
    if ((name[0] != 'o') || (name[1] != 'n'))
      return(0);
    for (i = 0;
	 i < sizeof(htmlScriptAttributes)/sizeof(htmlScriptAttributes[0]);
	 i++) {
	if (xmlStrEqual(name, (const xmlChar *) htmlScriptAttributes[i]))
	    return(1);
    }
    return(0);
}



static const htmlEntityDesc  html40EntitiesTable[] = {
{ 34,	"quot",	"quotation mark = APL quote, U+0022 ISOnum" },
{ 38,	"amp",	"ampersand, U+0026 ISOnum" },
{ 39,	"apos",	"single quote" },
{ 60,	"lt",	"less-than sign, U+003C ISOnum" },
{ 62,	"gt",	"greater-than sign, U+003E ISOnum" },

{ 160,	"nbsp",	"no-break space = non-breaking space, U+00A0 ISOnum" },
{ 161,	"iexcl","inverted exclamation mark, U+00A1 ISOnum" },
{ 162,	"cent",	"cent sign, U+00A2 ISOnum" },
{ 163,	"pound","pound sign, U+00A3 ISOnum" },
{ 164,	"curren","currency sign, U+00A4 ISOnum" },
{ 165,	"yen",	"yen sign = yuan sign, U+00A5 ISOnum" },
{ 166,	"brvbar","broken bar = broken vertical bar, U+00A6 ISOnum" },
{ 167,	"sect",	"section sign, U+00A7 ISOnum" },
{ 168,	"uml",	"diaeresis = spacing diaeresis, U+00A8 ISOdia" },
{ 169,	"copy",	"copyright sign, U+00A9 ISOnum" },
{ 170,	"ordf",	"feminine ordinal indicator, U+00AA ISOnum" },
{ 171,	"laquo","left-pointing double angle quotation mark = left pointing guillemet, U+00AB ISOnum" },
{ 172,	"not",	"not sign, U+00AC ISOnum" },
{ 173,	"shy",	"soft hyphen = discretionary hyphen, U+00AD ISOnum" },
{ 174,	"reg",	"registered sign = registered trade mark sign, U+00AE ISOnum" },
{ 175,	"macr",	"macron = spacing macron = overline = APL overbar, U+00AF ISOdia" },
{ 176,	"deg",	"degree sign, U+00B0 ISOnum" },
{ 177,	"plusmn","plus-minus sign = plus-or-minus sign, U+00B1 ISOnum" },
{ 178,	"sup2",	"superscript two = superscript digit two = squared, U+00B2 ISOnum" },
{ 179,	"sup3",	"superscript three = superscript digit three = cubed, U+00B3 ISOnum" },
{ 180,	"acute","acute accent = spacing acute, U+00B4 ISOdia" },
{ 181,	"micro","micro sign, U+00B5 ISOnum" },
{ 182,	"para",	"pilcrow sign = paragraph sign, U+00B6 ISOnum" },
{ 183,	"middot","middle dot = Georgian comma Greek middle dot, U+00B7 ISOnum" },
{ 184,	"cedil","cedilla = spacing cedilla, U+00B8 ISOdia" },
{ 185,	"sup1",	"superscript one = superscript digit one, U+00B9 ISOnum" },
{ 186,	"ordm",	"masculine ordinal indicator, U+00BA ISOnum" },
{ 187,	"raquo","right-pointing double angle quotation mark right pointing guillemet, U+00BB ISOnum" },
{ 188,	"frac14","vulgar fraction one quarter = fraction one quarter, U+00BC ISOnum" },
{ 189,	"frac12","vulgar fraction one half = fraction one half, U+00BD ISOnum" },
{ 190,	"frac34","vulgar fraction three quarters = fraction three quarters, U+00BE ISOnum" },
{ 191,	"iquest","inverted question mark = turned question mark, U+00BF ISOnum" },
{ 192,	"Agrave","latin capital letter A with grave = latin capital letter A grave, U+00C0 ISOlat1" },
{ 193,	"Aacute","latin capital letter A with acute, U+00C1 ISOlat1" },
{ 194,	"Acirc","latin capital letter A with circumflex, U+00C2 ISOlat1" },
{ 195,	"Atilde","latin capital letter A with tilde, U+00C3 ISOlat1" },
{ 196,	"Auml",	"latin capital letter A with diaeresis, U+00C4 ISOlat1" },
{ 197,	"Aring","latin capital letter A with ring above = latin capital letter A ring, U+00C5 ISOlat1" },
{ 198,	"AElig","latin capital letter AE = latin capital ligature AE, U+00C6 ISOlat1" },
{ 199,	"Ccedil","latin capital letter C with cedilla, U+00C7 ISOlat1" },
{ 200,	"Egrave","latin capital letter E with grave, U+00C8 ISOlat1" },
{ 201,	"Eacute","latin capital letter E with acute, U+00C9 ISOlat1" },
{ 202,	"Ecirc","latin capital letter E with circumflex, U+00CA ISOlat1" },
{ 203,	"Euml",	"latin capital letter E with diaeresis, U+00CB ISOlat1" },
{ 204,	"Igrave","latin capital letter I with grave, U+00CC ISOlat1" },
{ 205,	"Iacute","latin capital letter I with acute, U+00CD ISOlat1" },
{ 206,	"Icirc","latin capital letter I with circumflex, U+00CE ISOlat1" },
{ 207,	"Iuml",	"latin capital letter I with diaeresis, U+00CF ISOlat1" },
{ 208,	"ETH",	"latin capital letter ETH, U+00D0 ISOlat1" },
{ 209,	"Ntilde","latin capital letter N with tilde, U+00D1 ISOlat1" },
{ 210,	"Ograve","latin capital letter O with grave, U+00D2 ISOlat1" },
{ 211,	"Oacute","latin capital letter O with acute, U+00D3 ISOlat1" },
{ 212,	"Ocirc","latin capital letter O with circumflex, U+00D4 ISOlat1" },
{ 213,	"Otilde","latin capital letter O with tilde, U+00D5 ISOlat1" },
{ 214,	"Ouml",	"latin capital letter O with diaeresis, U+00D6 ISOlat1" },
{ 215,	"times","multiplication sign, U+00D7 ISOnum" },
{ 216,	"Oslash","latin capital letter O with stroke latin capital letter O slash, U+00D8 ISOlat1" },
{ 217,	"Ugrave","latin capital letter U with grave, U+00D9 ISOlat1" },
{ 218,	"Uacute","latin capital letter U with acute, U+00DA ISOlat1" },
{ 219,	"Ucirc","latin capital letter U with circumflex, U+00DB ISOlat1" },
{ 220,	"Uuml",	"latin capital letter U with diaeresis, U+00DC ISOlat1" },
{ 221,	"Yacute","latin capital letter Y with acute, U+00DD ISOlat1" },
{ 222,	"THORN","latin capital letter THORN, U+00DE ISOlat1" },
{ 223,	"szlig","latin small letter sharp s = ess-zed, U+00DF ISOlat1" },
{ 224,	"agrave","latin small letter a with grave = latin small letter a grave, U+00E0 ISOlat1" },
{ 225,	"aacute","latin small letter a with acute, U+00E1 ISOlat1" },
{ 226,	"acirc","latin small letter a with circumflex, U+00E2 ISOlat1" },
{ 227,	"atilde","latin small letter a with tilde, U+00E3 ISOlat1" },
{ 228,	"auml",	"latin small letter a with diaeresis, U+00E4 ISOlat1" },
{ 229,	"aring","latin small letter a with ring above = latin small letter a ring, U+00E5 ISOlat1" },
{ 230,	"aelig","latin small letter ae = latin small ligature ae, U+00E6 ISOlat1" },
{ 231,	"ccedil","latin small letter c with cedilla, U+00E7 ISOlat1" },
{ 232,	"egrave","latin small letter e with grave, U+00E8 ISOlat1" },
{ 233,	"eacute","latin small letter e with acute, U+00E9 ISOlat1" },
{ 234,	"ecirc","latin small letter e with circumflex, U+00EA ISOlat1" },
{ 235,	"euml",	"latin small letter e with diaeresis, U+00EB ISOlat1" },
{ 236,	"igrave","latin small letter i with grave, U+00EC ISOlat1" },
{ 237,	"iacute","latin small letter i with acute, U+00ED ISOlat1" },
{ 238,	"icirc","latin small letter i with circumflex, U+00EE ISOlat1" },
{ 239,	"iuml",	"latin small letter i with diaeresis, U+00EF ISOlat1" },
{ 240,	"eth",	"latin small letter eth, U+00F0 ISOlat1" },
{ 241,	"ntilde","latin small letter n with tilde, U+00F1 ISOlat1" },
{ 242,	"ograve","latin small letter o with grave, U+00F2 ISOlat1" },
{ 243,	"oacute","latin small letter o with acute, U+00F3 ISOlat1" },
{ 244,	"ocirc","latin small letter o with circumflex, U+00F4 ISOlat1" },
{ 245,	"otilde","latin small letter o with tilde, U+00F5 ISOlat1" },
{ 246,	"ouml",	"latin small letter o with diaeresis, U+00F6 ISOlat1" },
{ 247,	"divide","division sign, U+00F7 ISOnum" },
{ 248,	"oslash","latin small letter o with stroke, = latin small letter o slash, U+00F8 ISOlat1" },
{ 249,	"ugrave","latin small letter u with grave, U+00F9 ISOlat1" },
{ 250,	"uacute","latin small letter u with acute, U+00FA ISOlat1" },
{ 251,	"ucirc","latin small letter u with circumflex, U+00FB ISOlat1" },
{ 252,	"uuml",	"latin small letter u with diaeresis, U+00FC ISOlat1" },
{ 253,	"yacute","latin small letter y with acute, U+00FD ISOlat1" },
{ 254,	"thorn","latin small letter thorn with, U+00FE ISOlat1" },
{ 255,	"yuml",	"latin small letter y with diaeresis, U+00FF ISOlat1" },

{ 338,	"OElig","latin capital ligature OE, U+0152 ISOlat2" },
{ 339,	"oelig","latin small ligature oe, U+0153 ISOlat2" },
{ 352,	"Scaron","latin capital letter S with caron, U+0160 ISOlat2" },
{ 353,	"scaron","latin small letter s with caron, U+0161 ISOlat2" },
{ 376,	"Yuml",	"latin capital letter Y with diaeresis, U+0178 ISOlat2" },

{ 402,	"fnof",	"latin small f with hook = function = florin, U+0192 ISOtech" },

{ 710,	"circ",	"modifier letter circumflex accent, U+02C6 ISOpub" },
{ 732,	"tilde","small tilde, U+02DC ISOdia" },

{ 913,	"Alpha","greek capital letter alpha, U+0391" },
{ 914,	"Beta",	"greek capital letter beta, U+0392" },
{ 915,	"Gamma","greek capital letter gamma, U+0393 ISOgrk3" },
{ 916,	"Delta","greek capital letter delta, U+0394 ISOgrk3" },
{ 917,	"Epsilon","greek capital letter epsilon, U+0395" },
{ 918,	"Zeta",	"greek capital letter zeta, U+0396" },
{ 919,	"Eta",	"greek capital letter eta, U+0397" },
{ 920,	"Theta","greek capital letter theta, U+0398 ISOgrk3" },
{ 921,	"Iota",	"greek capital letter iota, U+0399" },
{ 922,	"Kappa","greek capital letter kappa, U+039A" },
{ 923,	"Lambda", "greek capital letter lambda, U+039B ISOgrk3" },
{ 924,	"Mu",	"greek capital letter mu, U+039C" },
{ 925,	"Nu",	"greek capital letter nu, U+039D" },
{ 926,	"Xi",	"greek capital letter xi, U+039E ISOgrk3" },
{ 927,	"Omicron","greek capital letter omicron, U+039F" },
{ 928,	"Pi",	"greek capital letter pi, U+03A0 ISOgrk3" },
{ 929,	"Rho",	"greek capital letter rho, U+03A1" },
{ 931,	"Sigma","greek capital letter sigma, U+03A3 ISOgrk3" },
{ 932,	"Tau",	"greek capital letter tau, U+03A4" },
{ 933,	"Upsilon","greek capital letter upsilon, U+03A5 ISOgrk3" },
{ 934,	"Phi",	"greek capital letter phi, U+03A6 ISOgrk3" },
{ 935,	"Chi",	"greek capital letter chi, U+03A7" },
{ 936,	"Psi",	"greek capital letter psi, U+03A8 ISOgrk3" },
{ 937,	"Omega","greek capital letter omega, U+03A9 ISOgrk3" },

{ 945,	"alpha","greek small letter alpha, U+03B1 ISOgrk3" },
{ 946,	"beta",	"greek small letter beta, U+03B2 ISOgrk3" },
{ 947,	"gamma","greek small letter gamma, U+03B3 ISOgrk3" },
{ 948,	"delta","greek small letter delta, U+03B4 ISOgrk3" },
{ 949,	"epsilon","greek small letter epsilon, U+03B5 ISOgrk3" },
{ 950,	"zeta",	"greek small letter zeta, U+03B6 ISOgrk3" },
{ 951,	"eta",	"greek small letter eta, U+03B7 ISOgrk3" },
{ 952,	"theta","greek small letter theta, U+03B8 ISOgrk3" },
{ 953,	"iota",	"greek small letter iota, U+03B9 ISOgrk3" },
{ 954,	"kappa","greek small letter kappa, U+03BA ISOgrk3" },
{ 955,	"lambda","greek small letter lambda, U+03BB ISOgrk3" },
{ 956,	"mu",	"greek small letter mu, U+03BC ISOgrk3" },
{ 957,	"nu",	"greek small letter nu, U+03BD ISOgrk3" },
{ 958,	"xi",	"greek small letter xi, U+03BE ISOgrk3" },
{ 959,	"omicron","greek small letter omicron, U+03BF NEW" },
{ 960,	"pi",	"greek small letter pi, U+03C0 ISOgrk3" },
{ 961,	"rho",	"greek small letter rho, U+03C1 ISOgrk3" },
{ 962,	"sigmaf","greek small letter final sigma, U+03C2 ISOgrk3" },
{ 963,	"sigma","greek small letter sigma, U+03C3 ISOgrk3" },
{ 964,	"tau",	"greek small letter tau, U+03C4 ISOgrk3" },
{ 965,	"upsilon","greek small letter upsilon, U+03C5 ISOgrk3" },
{ 966,	"phi",	"greek small letter phi, U+03C6 ISOgrk3" },
{ 967,	"chi",	"greek small letter chi, U+03C7 ISOgrk3" },
{ 968,	"psi",	"greek small letter psi, U+03C8 ISOgrk3" },
{ 969,	"omega","greek small letter omega, U+03C9 ISOgrk3" },
{ 977,	"thetasym","greek small letter theta symbol, U+03D1 NEW" },
{ 978,	"upsih","greek upsilon with hook symbol, U+03D2 NEW" },
{ 982,	"piv",	"greek pi symbol, U+03D6 ISOgrk3" },

{ 8194,	"ensp",	"en space, U+2002 ISOpub" },
{ 8195,	"emsp",	"em space, U+2003 ISOpub" },
{ 8201,	"thinsp","thin space, U+2009 ISOpub" },
{ 8204,	"zwnj",	"zero width non-joiner, U+200C NEW RFC 2070" },
{ 8205,	"zwj",	"zero width joiner, U+200D NEW RFC 2070" },
{ 8206,	"lrm",	"left-to-right mark, U+200E NEW RFC 2070" },
{ 8207,	"rlm",	"right-to-left mark, U+200F NEW RFC 2070" },
{ 8211,	"ndash","en dash, U+2013 ISOpub" },
{ 8212,	"mdash","em dash, U+2014 ISOpub" },
{ 8216,	"lsquo","left single quotation mark, U+2018 ISOnum" },
{ 8217,	"rsquo","right single quotation mark, U+2019 ISOnum" },
{ 8218,	"sbquo","single low-9 quotation mark, U+201A NEW" },
{ 8220,	"ldquo","left double quotation mark, U+201C ISOnum" },
{ 8221,	"rdquo","right double quotation mark, U+201D ISOnum" },
{ 8222,	"bdquo","double low-9 quotation mark, U+201E NEW" },
{ 8224,	"dagger","dagger, U+2020 ISOpub" },
{ 8225,	"Dagger","double dagger, U+2021 ISOpub" },

{ 8226,	"bull",	"bullet = black small circle, U+2022 ISOpub" },
{ 8230,	"hellip","horizontal ellipsis = three dot leader, U+2026 ISOpub" },

{ 8240,	"permil","per mille sign, U+2030 ISOtech" },

{ 8242,	"prime","prime = minutes = feet, U+2032 ISOtech" },
{ 8243,	"Prime","double prime = seconds = inches, U+2033 ISOtech" },

{ 8249,	"lsaquo","single left-pointing angle quotation mark, U+2039 ISO proposed" },
{ 8250,	"rsaquo","single right-pointing angle quotation mark, U+203A ISO proposed" },

{ 8254,	"oline","overline = spacing overscore, U+203E NEW" },
{ 8260,	"frasl","fraction slash, U+2044 NEW" },

{ 8364,	"euro",	"euro sign, U+20AC NEW" },

{ 8465,	"image","blackletter capital I = imaginary part, U+2111 ISOamso" },
{ 8472,	"weierp","script capital P = power set = Weierstrass p, U+2118 ISOamso" },
{ 8476,	"real",	"blackletter capital R = real part symbol, U+211C ISOamso" },
{ 8482,	"trade","trade mark sign, U+2122 ISOnum" },
{ 8501,	"alefsym","alef symbol = first transfinite cardinal, U+2135 NEW" },
{ 8592,	"larr",	"leftwards arrow, U+2190 ISOnum" },
{ 8593,	"uarr",	"upwards arrow, U+2191 ISOnum" },
{ 8594,	"rarr",	"rightwards arrow, U+2192 ISOnum" },
{ 8595,	"darr",	"downwards arrow, U+2193 ISOnum" },
{ 8596,	"harr",	"left right arrow, U+2194 ISOamsa" },
{ 8629,	"crarr","downwards arrow with corner leftwards = carriage return, U+21B5 NEW" },
{ 8656,	"lArr",	"leftwards double arrow, U+21D0 ISOtech" },
{ 8657,	"uArr",	"upwards double arrow, U+21D1 ISOamsa" },
{ 8658,	"rArr",	"rightwards double arrow, U+21D2 ISOtech" },
{ 8659,	"dArr",	"downwards double arrow, U+21D3 ISOamsa" },
{ 8660,	"hArr",	"left right double arrow, U+21D4 ISOamsa" },

{ 8704,	"forall","for all, U+2200 ISOtech" },
{ 8706,	"part",	"partial differential, U+2202 ISOtech" },
{ 8707,	"exist","there exists, U+2203 ISOtech" },
{ 8709,	"empty","empty set = null set = diameter, U+2205 ISOamso" },
{ 8711,	"nabla","nabla = backward difference, U+2207 ISOtech" },
{ 8712,	"isin",	"element of, U+2208 ISOtech" },
{ 8713,	"notin","not an element of, U+2209 ISOtech" },
{ 8715,	"ni",	"contains as member, U+220B ISOtech" },
{ 8719,	"prod",	"n-ary product = product sign, U+220F ISOamsb" },
{ 8721,	"sum",	"n-ary summation, U+2211 ISOamsb" },
{ 8722,	"minus","minus sign, U+2212 ISOtech" },
{ 8727,	"lowast","asterisk operator, U+2217 ISOtech" },
{ 8730,	"radic","square root = radical sign, U+221A ISOtech" },
{ 8733,	"prop",	"proportional to, U+221D ISOtech" },
{ 8734,	"infin","infinity, U+221E ISOtech" },
{ 8736,	"ang",	"angle, U+2220 ISOamso" },
{ 8743,	"and",	"logical and = wedge, U+2227 ISOtech" },
{ 8744,	"or",	"logical or = vee, U+2228 ISOtech" },
{ 8745,	"cap",	"intersection = cap, U+2229 ISOtech" },
{ 8746,	"cup",	"union = cup, U+222A ISOtech" },
{ 8747,	"int",	"integral, U+222B ISOtech" },
{ 8756,	"there4","therefore, U+2234 ISOtech" },
{ 8764,	"sim",	"tilde operator = varies with = similar to, U+223C ISOtech" },
{ 8773,	"cong",	"approximately equal to, U+2245 ISOtech" },
{ 8776,	"asymp","almost equal to = asymptotic to, U+2248 ISOamsr" },
{ 8800,	"ne",	"not equal to, U+2260 ISOtech" },
{ 8801,	"equiv","identical to, U+2261 ISOtech" },
{ 8804,	"le",	"less-than or equal to, U+2264 ISOtech" },
{ 8805,	"ge",	"greater-than or equal to, U+2265 ISOtech" },
{ 8834,	"sub",	"subset of, U+2282 ISOtech" },
{ 8835,	"sup",	"superset of, U+2283 ISOtech" },
{ 8836,	"nsub",	"not a subset of, U+2284 ISOamsn" },
{ 8838,	"sube",	"subset of or equal to, U+2286 ISOtech" },
{ 8839,	"supe",	"superset of or equal to, U+2287 ISOtech" },
{ 8853,	"oplus","circled plus = direct sum, U+2295 ISOamsb" },
{ 8855,	"otimes","circled times = vector product, U+2297 ISOamsb" },
{ 8869,	"perp",	"up tack = orthogonal to = perpendicular, U+22A5 ISOtech" },
{ 8901,	"sdot",	"dot operator, U+22C5 ISOamsb" },
{ 8968,	"lceil","left ceiling = apl upstile, U+2308 ISOamsc" },
{ 8969,	"rceil","right ceiling, U+2309 ISOamsc" },
{ 8970,	"lfloor","left floor = apl downstile, U+230A ISOamsc" },
{ 8971,	"rfloor","right floor, U+230B ISOamsc" },
{ 9001,	"lang",	"left-pointing angle bracket = bra, U+2329 ISOtech" },
{ 9002,	"rang",	"right-pointing angle bracket = ket, U+232A ISOtech" },
{ 9674,	"loz",	"lozenge, U+25CA ISOpub" },

{ 9824,	"spades","black spade suit, U+2660 ISOpub" },
{ 9827,	"clubs","black club suit = shamrock, U+2663 ISOpub" },
{ 9829,	"hearts","black heart suit = valentine, U+2665 ISOpub" },
{ 9830,	"diams","black diamond suit, U+2666 ISOpub" },

};


#define growBuffer(buffer) {						\
    xmlChar *tmp;							\
    buffer##_size *= 2;							\
    tmp = (xmlChar *) xmlRealloc(buffer, buffer##_size * sizeof(xmlChar)); \
    if (tmp == NULL) {						\
	htmlErrMemory(ctxt, "growing buffer\n");			\
	xmlFree(buffer);						\
	return(NULL);							\
    }									\
    buffer = tmp;							\
}

const htmlEntityDesc *
htmlEntityLookup(const xmlChar *name) {
    unsigned int i;

    for (i = 0;i < (sizeof(html40EntitiesTable)/
                    sizeof(html40EntitiesTable[0]));i++) {
        if (xmlStrEqual(name, BAD_CAST html40EntitiesTable[i].name)) {
            return((htmlEntityDescPtr) &html40EntitiesTable[i]);
	}
    }
    return(NULL);
}

const htmlEntityDesc *
htmlEntityValueLookup(unsigned int value) {
    unsigned int i;

    for (i = 0;i < (sizeof(html40EntitiesTable)/
                    sizeof(html40EntitiesTable[0]));i++) {
        if (html40EntitiesTable[i].value >= value) {
	    if (html40EntitiesTable[i].value > value)
		break;
            return((htmlEntityDescPtr) &html40EntitiesTable[i]);
	}
    }
    return(NULL);
}

int
UTF8ToHtml(unsigned char* out, int *outlen,
              const unsigned char* in, int *inlen) {
    const unsigned char* processed = in;
    const unsigned char* outend;
    const unsigned char* outstart = out;
    const unsigned char* instart = in;
    const unsigned char* inend;
    unsigned int c, d;
    int trailing;

    if ((out == NULL) || (outlen == NULL) || (inlen == NULL)) return(-1);
    if (in == NULL) {
	*outlen = 0;
	*inlen = 0;
	return(0);
    }
    inend = in + (*inlen);
    outend = out + (*outlen);
    while (in < inend) {
	d = *in++;
	if      (d < 0x80)  { c= d; trailing= 0; }
	else if (d < 0xC0) {
	    
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
        } else if (d < 0xE0)  { c= d & 0x1F; trailing= 1; }
        else if (d < 0xF0)  { c= d & 0x0F; trailing= 2; }
        else if (d < 0xF8)  { c= d & 0x07; trailing= 3; }
	else {
	    
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
	}

	if (inend - in < trailing) {
	    break;
	}

	for ( ; trailing; trailing--) {
	    if ((in >= inend) || (((d= *in++) & 0xC0) != 0x80))
		break;
	    c <<= 6;
	    c |= d & 0x3F;
	}

	
	if (c < 0x80) {
	    if (out + 1 >= outend)
		break;
	    *out++ = c;
	} else {
	    int len;
	    const htmlEntityDesc * ent;
	    const char *cp;
	    char nbuf[16];


	    ent = htmlEntityValueLookup(c);
	    if (ent == NULL) {
	      snprintf(nbuf, sizeof(nbuf), "#%u", c);
	      cp = nbuf;
	    }
	    else
	      cp = ent->name;
	    len = strlen(cp);
	    if (out + 2 + len >= outend)
		break;
	    *out++ = '&';
	    memcpy(out, cp, len);
	    out += len;
	    *out++ = ';';
	}
	processed = in;
    }
    *outlen = out - outstart;
    *inlen = processed - instart;
    return(0);
}

int
htmlEncodeEntities(unsigned char* out, int *outlen,
		   const unsigned char* in, int *inlen, int quoteChar) {
    const unsigned char* processed = in;
    const unsigned char* outend;
    const unsigned char* outstart = out;
    const unsigned char* instart = in;
    const unsigned char* inend;
    unsigned int c, d;
    int trailing;

    if ((out == NULL) || (outlen == NULL) || (inlen == NULL) || (in == NULL))
        return(-1);
    outend = out + (*outlen);
    inend = in + (*inlen);
    while (in < inend) {
	d = *in++;
	if      (d < 0x80)  { c= d; trailing= 0; }
	else if (d < 0xC0) {
	    
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
        } else if (d < 0xE0)  { c= d & 0x1F; trailing= 1; }
        else if (d < 0xF0)  { c= d & 0x0F; trailing= 2; }
        else if (d < 0xF8)  { c= d & 0x07; trailing= 3; }
	else {
	    
	    *outlen = out - outstart;
	    *inlen = processed - instart;
	    return(-2);
	}

	if (inend - in < trailing)
	    break;

	while (trailing--) {
	    if (((d= *in++) & 0xC0) != 0x80) {
		*outlen = out - outstart;
		*inlen = processed - instart;
		return(-2);
	    }
	    c <<= 6;
	    c |= d & 0x3F;
	}

	
	if ((c < 0x80) && (c != (unsigned int) quoteChar) &&
	    (c != '&') && (c != '<') && (c != '>')) {
	    if (out >= outend)
		break;
	    *out++ = c;
	} else {
	    const htmlEntityDesc * ent;
	    const char *cp;
	    char nbuf[16];
	    int len;

	    ent = htmlEntityValueLookup(c);
	    if (ent == NULL) {
		snprintf(nbuf, sizeof(nbuf), "#%u", c);
		cp = nbuf;
	    }
	    else
		cp = ent->name;
	    len = strlen(cp);
	    if (out + 2 + len > outend)
		break;
	    *out++ = '&';
	    memcpy(out, cp, len);
	    out += len;
	    *out++ = ';';
	}
	processed = in;
    }
    *outlen = out - outstart;
    *inlen = processed - instart;
    return(0);
}


static htmlParserInputPtr
htmlNewInputStream(htmlParserCtxtPtr ctxt) {
    htmlParserInputPtr input;

    input = (xmlParserInputPtr) xmlMalloc(sizeof(htmlParserInput));
    if (input == NULL) {
        htmlErrMemory(ctxt, "couldn't allocate a new input stream\n");
	return(NULL);
    }
    memset(input, 0, sizeof(htmlParserInput));
    input->filename = NULL;
    input->directory = NULL;
    input->base = NULL;
    input->cur = NULL;
    input->buf = NULL;
    input->line = 1;
    input->col = 1;
    input->buf = NULL;
    input->free = NULL;
    input->version = NULL;
    input->consumed = 0;
    input->length = 0;
    return(input);
}


static const char *allowPCData[] = {
    "a", "abbr", "acronym", "address", "applet", "b", "bdo", "big",
    "blockquote", "body", "button", "caption", "center", "cite", "code",
    "dd", "del", "dfn", "div", "dt", "em", "font", "form", "h1", "h2",
    "h3", "h4", "h5", "h6", "i", "iframe", "ins", "kbd", "label", "legend",
    "li", "noframes", "noscript", "object", "p", "pre", "q", "s", "samp",
    "small", "span", "strike", "strong", "td", "th", "tt", "u", "var"
};


static int areBlanks(htmlParserCtxtPtr ctxt, const xmlChar *str, int len) {
    unsigned int i;
    int j;
    xmlNodePtr lastChild;
    xmlDtdPtr dtd;

    for (j = 0;j < len;j++)
        if (!(IS_BLANK_CH(str[j]))) return(0);

    if (CUR == 0) return(1);
    if (CUR != '<') return(0);
    if (ctxt->name == NULL)
	return(1);
    if (xmlStrEqual(ctxt->name, BAD_CAST"html"))
	return(1);
    if (xmlStrEqual(ctxt->name, BAD_CAST"head"))
	return(1);

    
    if (xmlStrEqual(ctxt->name, BAD_CAST "body") && ctxt->myDoc != NULL) {
        dtd = xmlGetIntSubset(ctxt->myDoc);
        if (dtd != NULL && dtd->ExternalID != NULL) {
            if (!xmlStrcasecmp(dtd->ExternalID, BAD_CAST "-//W3C//DTD HTML 4.01//EN") ||
                    !xmlStrcasecmp(dtd->ExternalID, BAD_CAST "-//W3C//DTD HTML 4//EN"))
                return(1);
        }
    }

    if (ctxt->node == NULL) return(0);
    lastChild = xmlGetLastChild(ctxt->node);
    while ((lastChild) && (lastChild->type == XML_COMMENT_NODE))
	lastChild = lastChild->prev;
    if (lastChild == NULL) {
        if ((ctxt->node->type != XML_ELEMENT_NODE) &&
            (ctxt->node->content != NULL)) return(0);
	for ( i = 0; i < sizeof(allowPCData)/sizeof(allowPCData[0]); i++ ) {
	    if ( xmlStrEqual(ctxt->name, BAD_CAST allowPCData[i]) ) {
		return(0);
	    }
	}
    } else if (xmlNodeIsText(lastChild)) {
        return(0);
    } else {
	for ( i = 0; i < sizeof(allowPCData)/sizeof(allowPCData[0]); i++ ) {
	    if ( xmlStrEqual(lastChild->name, BAD_CAST allowPCData[i]) ) {
		return(0);
	    }
	}
    }
    return(1);
}

htmlDocPtr
htmlNewDocNoDtD(const xmlChar *URI, const xmlChar *ExternalID) {
    xmlDocPtr cur;

    cur = (xmlDocPtr) xmlMalloc(sizeof(xmlDoc));
    if (cur == NULL) {
	htmlErrMemory(NULL, "HTML document creation failed\n");
	return(NULL);
    }
    memset(cur, 0, sizeof(xmlDoc));

    cur->type = XML_HTML_DOCUMENT_NODE;
    cur->version = NULL;
    cur->intSubset = NULL;
    cur->doc = cur;
    cur->name = NULL;
    cur->children = NULL;
    cur->extSubset = NULL;
    cur->oldNs = NULL;
    cur->encoding = NULL;
    cur->standalone = 1;
    cur->compression = 0;
    cur->ids = NULL;
    cur->refs = NULL;
    cur->_private = NULL;
    cur->charset = XML_CHAR_ENCODING_UTF8;
    cur->properties = XML_DOC_HTML | XML_DOC_USERBUILT;
    if ((ExternalID != NULL) ||
	(URI != NULL))
	xmlCreateIntSubset(cur, BAD_CAST "html", ExternalID, URI);
    return(cur);
}

htmlDocPtr
htmlNewDoc(const xmlChar *URI, const xmlChar *ExternalID) {
    if ((URI == NULL) && (ExternalID == NULL))
	return(htmlNewDocNoDtD(
		    BAD_CAST "http://www.w3.org/TR/REC-html40/loose.dtd",
		    BAD_CAST "-//W3C//DTD HTML 4.0 Transitional//EN"));

    return(htmlNewDocNoDtD(URI, ExternalID));
}




static const xmlChar * htmlParseNameComplex(xmlParserCtxtPtr ctxt);


static const xmlChar *
htmlParseHTMLName(htmlParserCtxtPtr ctxt) {
    int i = 0;
    xmlChar loc[HTML_PARSER_BUFFER_SIZE];

    if (!IS_ASCII_LETTER(CUR) && (CUR != '_') &&
        (CUR != ':') && (CUR != '.')) return(NULL);

    while ((i < HTML_PARSER_BUFFER_SIZE) &&
           ((IS_ASCII_LETTER(CUR)) || (IS_ASCII_DIGIT(CUR)) ||
	   (CUR == ':') || (CUR == '-') || (CUR == '_') ||
           (CUR == '.'))) {
	if ((CUR >= 'A') && (CUR <= 'Z')) loc[i] = CUR + 0x20;
        else loc[i] = CUR;
	i++;

	NEXT;
    }

    return(xmlDictLookup(ctxt->dict, loc, i));
}



static const xmlChar *
htmlParseHTMLName_nonInvasive(htmlParserCtxtPtr ctxt) {
    int i = 0;
    xmlChar loc[HTML_PARSER_BUFFER_SIZE];

    if (!IS_ASCII_LETTER(NXT(1)) && (NXT(1) != '_') &&
        (NXT(1) != ':')) return(NULL);

    while ((i < HTML_PARSER_BUFFER_SIZE) &&
           ((IS_ASCII_LETTER(NXT(1+i))) || (IS_ASCII_DIGIT(NXT(1+i))) ||
	   (NXT(1+i) == ':') || (NXT(1+i) == '-') || (NXT(1+i) == '_'))) {
	if ((NXT(1+i) >= 'A') && (NXT(1+i) <= 'Z')) loc[i] = NXT(1+i) + 0x20;
        else loc[i] = NXT(1+i);
	i++;
    }

    return(xmlDictLookup(ctxt->dict, loc, i));
}



static const xmlChar *
htmlParseName(htmlParserCtxtPtr ctxt) {
    const xmlChar *in;
    const xmlChar *ret;
    int count = 0;

    GROW;

    in = ctxt->input->cur;
    if (((*in >= 0x61) && (*in <= 0x7A)) ||
	((*in >= 0x41) && (*in <= 0x5A)) ||
	(*in == '_') || (*in == ':')) {
	in++;
	while (((*in >= 0x61) && (*in <= 0x7A)) ||
	       ((*in >= 0x41) && (*in <= 0x5A)) ||
	       ((*in >= 0x30) && (*in <= 0x39)) ||
	       (*in == '_') || (*in == '-') ||
	       (*in == ':') || (*in == '.'))
	    in++;
	if ((*in > 0) && (*in < 0x80)) {
	    count = in - ctxt->input->cur;
	    ret = xmlDictLookup(ctxt->dict, ctxt->input->cur, count);
	    ctxt->input->cur = in;
	    ctxt->nbChars += count;
	    ctxt->input->col += count;
	    return(ret);
	}
    }
    return(htmlParseNameComplex(ctxt));
}

static const xmlChar *
htmlParseNameComplex(xmlParserCtxtPtr ctxt) {
    int len = 0, l;
    int c;
    int count = 0;

    GROW;
    c = CUR_CHAR(l);
    if ((c == ' ') || (c == '>') || (c == '/') || 
	(!IS_LETTER(c) && (c != '_') &&
         (c != ':'))) {
	return(NULL);
    }

    while ((c != ' ') && (c != '>') && (c != '/') && 
	   ((IS_LETTER(c)) || (IS_DIGIT(c)) ||
            (c == '.') || (c == '-') ||
	    (c == '_') || (c == ':') ||
	    (IS_COMBINING(c)) ||
	    (IS_EXTENDER(c)))) {
	if (count++ > 100) {
	    count = 0;
	    GROW;
	}
	len += l;
	NEXTL(l);
	c = CUR_CHAR(l);
    }
    return(xmlDictLookup(ctxt->dict, ctxt->input->cur - len, len));
}



static xmlChar *
htmlParseHTMLAttribute(htmlParserCtxtPtr ctxt, const xmlChar stop) {
    xmlChar *buffer = NULL;
    int buffer_size = 0;
    xmlChar *out = NULL;
    const xmlChar *name = NULL;
    const xmlChar *cur = NULL;
    const htmlEntityDesc * ent;

    buffer_size = HTML_PARSER_BUFFER_SIZE;
    buffer = (xmlChar *) xmlMallocAtomic(buffer_size * sizeof(xmlChar));
    if (buffer == NULL) {
	htmlErrMemory(ctxt, "buffer allocation failed\n");
	return(NULL);
    }
    out = buffer;

    while ((CUR != 0) && (CUR != stop)) {
	if ((stop == 0) && (CUR == '>')) break;
	if ((stop == 0) && (IS_BLANK_CH(CUR))) break;
        if (CUR == '&') {
	    if (NXT(1) == '#') {
		unsigned int c;
		int bits;

		c = htmlParseCharRef(ctxt);
		if      (c <    0x80)
		        { *out++  = c;                bits= -6; }
		else if (c <   0x800)
		        { *out++  =((c >>  6) & 0x1F) | 0xC0;  bits=  0; }
		else if (c < 0x10000)
		        { *out++  =((c >> 12) & 0x0F) | 0xE0;  bits=  6; }
		else
		        { *out++  =((c >> 18) & 0x07) | 0xF0;  bits= 12; }

		for ( ; bits >= 0; bits-= 6) {
		    *out++  = ((c >> bits) & 0x3F) | 0x80;
		}

		if (out - buffer > buffer_size - 100) {
			int indx = out - buffer;

			growBuffer(buffer);
			out = &buffer[indx];
		}
	    } else {
		ent = htmlParseEntityRef(ctxt, &name);
		if (name == NULL) {
		    *out++ = '&';
		    if (out - buffer > buffer_size - 100) {
			int indx = out - buffer;

			growBuffer(buffer);
			out = &buffer[indx];
		    }
		} else if (ent == NULL) {
		    *out++ = '&';
		    cur = name;
		    while (*cur != 0) {
			if (out - buffer > buffer_size - 100) {
			    int indx = out - buffer;

			    growBuffer(buffer);
			    out = &buffer[indx];
			}
			*out++ = *cur++;
		    }
		} else {
		    unsigned int c;
		    int bits;

		    if (out - buffer > buffer_size - 100) {
			int indx = out - buffer;

			growBuffer(buffer);
			out = &buffer[indx];
		    }
		    c = ent->value;
		    if      (c <    0x80)
			{ *out++  = c;                bits= -6; }
		    else if (c <   0x800)
			{ *out++  =((c >>  6) & 0x1F) | 0xC0;  bits=  0; }
		    else if (c < 0x10000)
			{ *out++  =((c >> 12) & 0x0F) | 0xE0;  bits=  6; }
		    else
			{ *out++  =((c >> 18) & 0x07) | 0xF0;  bits= 12; }

		    for ( ; bits >= 0; bits-= 6) {
			*out++  = ((c >> bits) & 0x3F) | 0x80;
		    }
		}
	    }
	} else {
	    unsigned int c;
	    int bits, l;

	    if (out - buffer > buffer_size - 100) {
		int indx = out - buffer;

		growBuffer(buffer);
		out = &buffer[indx];
	    }
	    c = CUR_CHAR(l);
	    if      (c <    0x80)
		    { *out++  = c;                bits= -6; }
	    else if (c <   0x800)
		    { *out++  =((c >>  6) & 0x1F) | 0xC0;  bits=  0; }
	    else if (c < 0x10000)
		    { *out++  =((c >> 12) & 0x0F) | 0xE0;  bits=  6; }
	    else
		    { *out++  =((c >> 18) & 0x07) | 0xF0;  bits= 12; }

	    for ( ; bits >= 0; bits-= 6) {
		*out++  = ((c >> bits) & 0x3F) | 0x80;
	    }
	    NEXT;
	}
    }
    *out = 0;
    return(buffer);
}

const htmlEntityDesc *
htmlParseEntityRef(htmlParserCtxtPtr ctxt, const xmlChar **str) {
    const xmlChar *name;
    const htmlEntityDesc * ent = NULL;

    if (str != NULL) *str = NULL;
    if ((ctxt == NULL) || (ctxt->input == NULL)) return(NULL);

    if (CUR == '&') {
        NEXT;
        name = htmlParseName(ctxt);
	if (name == NULL) {
	    htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
	                 "htmlParseEntityRef: no name\n", NULL, NULL);
	} else {
	    GROW;
	    if (CUR == ';') {
	        if (str != NULL)
		    *str = name;

		ent = htmlEntityLookup(name);
		if (ent != NULL) 
		    NEXT;
	    } else {
		htmlParseErr(ctxt, XML_ERR_ENTITYREF_SEMICOL_MISSING,
		             "htmlParseEntityRef: expecting ';'\n",
			     NULL, NULL);
	        if (str != NULL)
		    *str = name;
	    }
	}
    }
    return(ent);
}


static xmlChar *
htmlParseAttValue(htmlParserCtxtPtr ctxt) {
    xmlChar *ret = NULL;

    if (CUR == '"') {
        NEXT;
	ret = htmlParseHTMLAttribute(ctxt, '"');
        if (CUR != '"') {
	    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_NOT_FINISHED,
	                 "AttValue: \" expected\n", NULL, NULL);
	} else
	    NEXT;
    } else if (CUR == '\'') {
        NEXT;
	ret = htmlParseHTMLAttribute(ctxt, '\'');
        if (CUR != '\'') {
	    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_NOT_FINISHED,
	                 "AttValue: ' expected\n", NULL, NULL);
	} else
	    NEXT;
    } else {
	ret = htmlParseHTMLAttribute(ctxt, 0);
	if (ret == NULL) {
	    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_WITHOUT_VALUE,
	                 "AttValue: no value found\n", NULL, NULL);
	}
    }
    return(ret);
}


static xmlChar *
htmlParseSystemLiteral(htmlParserCtxtPtr ctxt) {
    const xmlChar *q;
    xmlChar *ret = NULL;

    if (CUR == '"') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '"'))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    htmlParseErr(ctxt, XML_ERR_LITERAL_NOT_FINISHED,
			 "Unfinished SystemLiteral\n", NULL, NULL);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else if (CUR == '\'') {
        NEXT;
	q = CUR_PTR;
	while ((IS_CHAR_CH(CUR)) && (CUR != '\''))
	    NEXT;
	if (!IS_CHAR_CH(CUR)) {
	    htmlParseErr(ctxt, XML_ERR_LITERAL_NOT_FINISHED,
			 "Unfinished SystemLiteral\n", NULL, NULL);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
        }
    } else {
	htmlParseErr(ctxt, XML_ERR_LITERAL_NOT_STARTED,
	             " or ' expected\n", NULL, NULL);
    }

    return(ret);
}


static xmlChar *
htmlParsePubidLiteral(htmlParserCtxtPtr ctxt) {
    const xmlChar *q;
    xmlChar *ret = NULL;
    if (CUR == '"') {
        NEXT;
	q = CUR_PTR;
	while (IS_PUBIDCHAR_CH(CUR)) NEXT;
	if (CUR != '"') {
	    htmlParseErr(ctxt, XML_ERR_LITERAL_NOT_FINISHED,
	                 "Unfinished PubidLiteral\n", NULL, NULL);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
	}
    } else if (CUR == '\'') {
        NEXT;
	q = CUR_PTR;
	while ((IS_PUBIDCHAR_CH(CUR)) && (CUR != '\''))
	    NEXT;
	if (CUR != '\'') {
	    htmlParseErr(ctxt, XML_ERR_LITERAL_NOT_FINISHED,
	                 "Unfinished PubidLiteral\n", NULL, NULL);
	} else {
	    ret = xmlStrndup(q, CUR_PTR - q);
	    NEXT;
	}
    } else {
	htmlParseErr(ctxt, XML_ERR_LITERAL_NOT_STARTED,
	             "PubidLiteral \" or ' expected\n", NULL, NULL);
    }

    return(ret);
}

static void
htmlParseScript(htmlParserCtxtPtr ctxt) {
    xmlChar buf[HTML_PARSER_BIG_BUFFER_SIZE + 5];
    int nbchar = 0;
    int cur,l;

    SHRINK;
    cur = CUR_CHAR(l);
    while (IS_CHAR_CH(cur)) {
	if ((cur == '<') && (NXT(1) == '/')) {
            if (ctxt->recovery) {
                if (xmlStrncasecmp(ctxt->name, ctxt->input->cur+2,
				   xmlStrlen(ctxt->name)) == 0)
                {
                    break; 
                } else {
		    htmlParseErr(ctxt, XML_ERR_TAG_NAME_MISMATCH,
				 "Element %s embeds close tag\n",
		                 ctxt->name, NULL);
		}
            } else {
                if (((NXT(2) >= 'A') && (NXT(2) <= 'Z')) ||
                    ((NXT(2) >= 'a') && (NXT(2) <= 'z')))
                {
                    break; 
                }
            }
	}
	COPY_BUF(l,buf,nbchar,cur);
	if (nbchar >= HTML_PARSER_BIG_BUFFER_SIZE) {
	    if (ctxt->sax->cdataBlock!= NULL) {
		ctxt->sax->cdataBlock(ctxt->userData, buf, nbchar);
	    } else if (ctxt->sax->characters != NULL) {
		ctxt->sax->characters(ctxt->userData, buf, nbchar);
	    }
	    nbchar = 0;
	}
	GROW;
	NEXTL(l);
	cur = CUR_CHAR(l);
    }

    if ((!(IS_CHAR_CH(cur))) && (!((cur == 0) && (ctxt->progressive)))) {
	htmlParseErrInt(ctxt, XML_ERR_INVALID_CHAR,
	                "Invalid char in CDATA 0x%X\n", cur);
	NEXT;
    }

    if ((nbchar != 0) && (ctxt->sax != NULL) && (!ctxt->disableSAX)) {
	if (ctxt->sax->cdataBlock!= NULL) {
	    ctxt->sax->cdataBlock(ctxt->userData, buf, nbchar);
	} else if (ctxt->sax->characters != NULL) {
	    ctxt->sax->characters(ctxt->userData, buf, nbchar);
	}
    }
}



static void
htmlParseCharData(htmlParserCtxtPtr ctxt) {
    xmlChar buf[HTML_PARSER_BIG_BUFFER_SIZE + 5];
    int nbchar = 0;
    int cur, l;
    int chunk = 0;

    SHRINK;
    cur = CUR_CHAR(l);
    while (((cur != '<') || (ctxt->token == '<')) &&
           ((cur != '&') || (ctxt->token == '&')) &&
	   (cur != 0)) {
	if (!(IS_CHAR(cur))) {
	    htmlParseErrInt(ctxt, XML_ERR_INVALID_CHAR,
	                "Invalid char in CDATA 0x%X\n", cur);
	} else {
	    COPY_BUF(l,buf,nbchar,cur);
	}
	if (nbchar >= HTML_PARSER_BIG_BUFFER_SIZE) {
	    if ((ctxt->sax != NULL) && (!ctxt->disableSAX)) {
		if (areBlanks(ctxt, buf, nbchar)) {
		    if (ctxt->sax->ignorableWhitespace != NULL)
			ctxt->sax->ignorableWhitespace(ctxt->userData,
			                               buf, nbchar);
		} else {
		    htmlCheckParagraph(ctxt);
		    if (ctxt->sax->characters != NULL)
			ctxt->sax->characters(ctxt->userData, buf, nbchar);
		}
	    }
	    nbchar = 0;
	}
	NEXTL(l);
        chunk++;
        if (chunk > HTML_PARSER_BUFFER_SIZE) {
            chunk = 0;
            SHRINK;
            GROW;
        }
	cur = CUR_CHAR(l);
	if (cur == 0) {
	    SHRINK;
	    GROW;
	    cur = CUR_CHAR(l);
	}
    }
    if (nbchar != 0) {
        buf[nbchar] = 0;

	if ((ctxt->sax != NULL) && (!ctxt->disableSAX)) {
	    if (areBlanks(ctxt, buf, nbchar)) {
		if (ctxt->sax->ignorableWhitespace != NULL)
		    ctxt->sax->ignorableWhitespace(ctxt->userData, buf, nbchar);
	    } else {
		htmlCheckParagraph(ctxt);
		if (ctxt->sax->characters != NULL)
		    ctxt->sax->characters(ctxt->userData, buf, nbchar);
	    }
	}
    } else {
	if (cur == 0)
	    ctxt->instate = XML_PARSER_EOF;
    }
}


static xmlChar *
htmlParseExternalID(htmlParserCtxtPtr ctxt, xmlChar **publicID) {
    xmlChar *URI = NULL;

    if ((UPPER == 'S') && (UPP(1) == 'Y') &&
         (UPP(2) == 'S') && (UPP(3) == 'T') &&
	 (UPP(4) == 'E') && (UPP(5) == 'M')) {
        SKIP(6);
	if (!IS_BLANK_CH(CUR)) {
	    htmlParseErr(ctxt, XML_ERR_SPACE_REQUIRED,
	                 "Space required after 'SYSTEM'\n", NULL, NULL);
	}
        SKIP_BLANKS;
	URI = htmlParseSystemLiteral(ctxt);
	if (URI == NULL) {
	    htmlParseErr(ctxt, XML_ERR_URI_REQUIRED,
	                 "htmlParseExternalID: SYSTEM, no URI\n", NULL, NULL);
        }
    } else if ((UPPER == 'P') && (UPP(1) == 'U') &&
	       (UPP(2) == 'B') && (UPP(3) == 'L') &&
	       (UPP(4) == 'I') && (UPP(5) == 'C')) {
        SKIP(6);
	if (!IS_BLANK_CH(CUR)) {
	    htmlParseErr(ctxt, XML_ERR_SPACE_REQUIRED,
	                 "Space required after 'PUBLIC'\n", NULL, NULL);
	}
        SKIP_BLANKS;
	*publicID = htmlParsePubidLiteral(ctxt);
	if (*publicID == NULL) {
	    htmlParseErr(ctxt, XML_ERR_PUBID_REQUIRED,
	                 "htmlParseExternalID: PUBLIC, no Public Identifier\n",
			 NULL, NULL);
	}
        SKIP_BLANKS;
        if ((CUR == '"') || (CUR == '\'')) {
	    URI = htmlParseSystemLiteral(ctxt);
	}
    }
    return(URI);
}

static void
htmlParsePI(htmlParserCtxtPtr ctxt) {
    xmlChar *buf = NULL;
    int len = 0;
    int size = HTML_PARSER_BUFFER_SIZE;
    int cur, l;
    const xmlChar *target;
    xmlParserInputState state;
    int count = 0;

    if ((RAW == '<') && (NXT(1) == '?')) {
	state = ctxt->instate;
        ctxt->instate = XML_PARSER_PI;
	SKIP(2);
	SHRINK;

        target = htmlParseName(ctxt);
	if (target != NULL) {
	    if (RAW == '>') {
		SKIP(1);

		if ((ctxt->sax) && (!ctxt->disableSAX) &&
		    (ctxt->sax->processingInstruction != NULL))
		    ctxt->sax->processingInstruction(ctxt->userData,
		                                     target, NULL);
		ctxt->instate = state;
		return;
	    }
	    buf = (xmlChar *) xmlMallocAtomic(size * sizeof(xmlChar));
	    if (buf == NULL) {
		htmlErrMemory(ctxt, NULL);
		ctxt->instate = state;
		return;
	    }
	    cur = CUR;
	    if (!IS_BLANK(cur)) {
		htmlParseErr(ctxt, XML_ERR_SPACE_REQUIRED,
			  "ParsePI: PI %s space expected\n", target, NULL);
	    }
            SKIP_BLANKS;
	    cur = CUR_CHAR(l);
	    while (IS_CHAR(cur) && (cur != '>')) {
		if (len + 5 >= size) {
		    xmlChar *tmp;

		    size *= 2;
		    tmp = (xmlChar *) xmlRealloc(buf, size * sizeof(xmlChar));
		    if (tmp == NULL) {
			htmlErrMemory(ctxt, NULL);
			xmlFree(buf);
			ctxt->instate = state;
			return;
		    }
		    buf = tmp;
		}
		count++;
		if (count > 50) {
		    GROW;
		    count = 0;
		}
		COPY_BUF(l,buf,len,cur);
		NEXTL(l);
		cur = CUR_CHAR(l);
		if (cur == 0) {
		    SHRINK;
		    GROW;
		    cur = CUR_CHAR(l);
		}
	    }
	    buf[len] = 0;
	    if (cur != '>') {
		htmlParseErr(ctxt, XML_ERR_PI_NOT_FINISHED,
		      "ParsePI: PI %s never end ...\n", target, NULL);
	    } else {
		SKIP(1);

		if ((ctxt->sax) && (!ctxt->disableSAX) &&
		    (ctxt->sax->processingInstruction != NULL))
		    ctxt->sax->processingInstruction(ctxt->userData,
		                                     target, buf);
	    }
	    xmlFree(buf);
	} else {
	    htmlParseErr(ctxt, XML_ERR_PI_NOT_STARTED,
                         "PI is not started correctly", NULL, NULL);
	}
	ctxt->instate = state;
    }
}

static void
htmlParseComment(htmlParserCtxtPtr ctxt) {
    xmlChar *buf = NULL;
    int len;
    int size = HTML_PARSER_BUFFER_SIZE;
    int q, ql;
    int r, rl;
    int cur, l;
    xmlParserInputState state;

    if ((RAW != '<') || (NXT(1) != '!') ||
        (NXT(2) != '-') || (NXT(3) != '-')) return;

    state = ctxt->instate;
    ctxt->instate = XML_PARSER_COMMENT;
    SHRINK;
    SKIP(4);
    buf = (xmlChar *) xmlMallocAtomic(size * sizeof(xmlChar));
    if (buf == NULL) {
        htmlErrMemory(ctxt, "buffer allocation failed\n");
	ctxt->instate = state;
	return;
    }
    q = CUR_CHAR(ql);
    NEXTL(ql);
    r = CUR_CHAR(rl);
    NEXTL(rl);
    cur = CUR_CHAR(l);
    len = 0;
    while (IS_CHAR(cur) &&
           ((cur != '>') ||
	    (r != '-') || (q != '-'))) {
	if (len + 5 >= size) {
	    xmlChar *tmp;

	    size *= 2;
	    tmp = (xmlChar *) xmlRealloc(buf, size * sizeof(xmlChar));
	    if (tmp == NULL) {
	        xmlFree(buf);
	        htmlErrMemory(ctxt, "growing buffer failed\n");
		ctxt->instate = state;
		return;
	    }
	    buf = tmp;
	}
	COPY_BUF(ql,buf,len,q);
	q = r;
	ql = rl;
	r = cur;
	rl = l;
	NEXTL(l);
	cur = CUR_CHAR(l);
	if (cur == 0) {
	    SHRINK;
	    GROW;
	    cur = CUR_CHAR(l);
	}
    }
    buf[len] = 0;
    if (!IS_CHAR(cur)) {
	htmlParseErr(ctxt, XML_ERR_COMMENT_NOT_FINISHED,
	             "Comment not terminated \n<!--%.50s\n", buf, NULL);
	xmlFree(buf);
    } else {
        NEXT;
	if ((ctxt->sax != NULL) && (ctxt->sax->comment != NULL) &&
	    (!ctxt->disableSAX))
	    ctxt->sax->comment(ctxt->userData, buf);
	xmlFree(buf);
    }
    ctxt->instate = state;
}

int
htmlParseCharRef(htmlParserCtxtPtr ctxt) {
    int val = 0;

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseCharRef: context error\n",
		     NULL, NULL);
        return(0);
    }
    if ((CUR == '&') && (NXT(1) == '#') &&
        ((NXT(2) == 'x') || NXT(2) == 'X')) {
	SKIP(3);
	while (CUR != ';') {
	    if ((CUR >= '0') && (CUR <= '9'))
	        val = val * 16 + (CUR - '0');
	    else if ((CUR >= 'a') && (CUR <= 'f'))
	        val = val * 16 + (CUR - 'a') + 10;
	    else if ((CUR >= 'A') && (CUR <= 'F'))
	        val = val * 16 + (CUR - 'A') + 10;
	    else {
	        htmlParseErr(ctxt, XML_ERR_INVALID_HEX_CHARREF,
		             "htmlParseCharRef: missing semicolumn\n",
			     NULL, NULL);
		break;
	    }
	    NEXT;
	}
	if (CUR == ';')
	    NEXT;
    } else if  ((CUR == '&') && (NXT(1) == '#')) {
	SKIP(2);
	while (CUR != ';') {
	    if ((CUR >= '0') && (CUR <= '9'))
	        val = val * 10 + (CUR - '0');
	    else {
	        htmlParseErr(ctxt, XML_ERR_INVALID_DEC_CHARREF,
		             "htmlParseCharRef: missing semicolumn\n",
			     NULL, NULL);
		break;
	    }
	    NEXT;
	}
	if (CUR == ';')
	    NEXT;
    } else {
	htmlParseErr(ctxt, XML_ERR_INVALID_CHARREF,
	             "htmlParseCharRef: invalid value\n", NULL, NULL);
    }
    if (IS_CHAR(val)) {
        return(val);
    } else {
	htmlParseErrInt(ctxt, XML_ERR_INVALID_CHAR,
			"htmlParseCharRef: invalid xmlChar value %d\n",
			val);
    }
    return(0);
}



static void
htmlParseDocTypeDecl(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    xmlChar *ExternalID = NULL;
    xmlChar *URI = NULL;

    SKIP(9);

    SKIP_BLANKS;

    name = htmlParseName(ctxt);
    if (name == NULL) {
	htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
	             "htmlParseDocTypeDecl : no DOCTYPE name !\n",
		     NULL, NULL);
    }

    SKIP_BLANKS;

    URI = htmlParseExternalID(ctxt, &ExternalID);
    SKIP_BLANKS;

    if (CUR != '>') {
	htmlParseErr(ctxt, XML_ERR_DOCTYPE_NOT_FINISHED,
	             "DOCTYPE improperly terminated\n", NULL, NULL);
        
    }
    NEXT;

    if ((ctxt->sax != NULL) && (ctxt->sax->internalSubset != NULL) &&
	(!ctxt->disableSAX))
	ctxt->sax->internalSubset(ctxt->userData, name, ExternalID, URI);

    if (URI != NULL) xmlFree(URI);
    if (ExternalID != NULL) xmlFree(ExternalID);
}


static const xmlChar *
htmlParseAttribute(htmlParserCtxtPtr ctxt, xmlChar **value) {
    const xmlChar *name;
    xmlChar *val = NULL;

    *value = NULL;
    name = htmlParseHTMLName(ctxt);
    if (name == NULL) {
	htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
	             "error parsing attribute name\n", NULL, NULL);
        return(NULL);
    }

    SKIP_BLANKS;
    if (CUR == '=') {
        NEXT;
	SKIP_BLANKS;
	val = htmlParseAttValue(ctxt);
    }

    *value = val;
    return(name);
}

static void
htmlCheckEncoding(htmlParserCtxtPtr ctxt, const xmlChar *attvalue) {
    const xmlChar *encoding;

    if ((ctxt == NULL) || (attvalue == NULL))
	return;

    
    if (ctxt->input->encoding != NULL)
        return;

    encoding = xmlStrcasestr(attvalue, BAD_CAST"charset=");
    if (encoding != NULL) {
	encoding += 8;
    } else {
	encoding = xmlStrcasestr(attvalue, BAD_CAST"charset =");
	if (encoding != NULL)
	    encoding += 9;
    }
    if (encoding != NULL) {
	xmlCharEncoding enc;
	xmlCharEncodingHandlerPtr handler;

	while ((*encoding == ' ') || (*encoding == '\t')) encoding++;

	if (ctxt->input->encoding != NULL)
	    xmlFree((xmlChar *) ctxt->input->encoding);
	ctxt->input->encoding = xmlStrdup(encoding);

	enc = xmlParseCharEncoding((const char *) encoding);
	if (enc != XML_CHAR_ENCODING_ERROR) {
	    if (((enc == XML_CHAR_ENCODING_UTF16LE) ||
	         (enc == XML_CHAR_ENCODING_UTF16BE) ||
		 (enc == XML_CHAR_ENCODING_UCS4LE) ||
		 (enc == XML_CHAR_ENCODING_UCS4BE)) &&
		(ctxt->input->buf != NULL) &&
		(ctxt->input->buf->encoder == NULL)) {
		htmlParseErr(ctxt, XML_ERR_INVALID_ENCODING,
		             "htmlCheckEncoding: wrong encoding meta\n",
			     NULL, NULL);
	    } else {
		xmlSwitchEncoding(ctxt, enc);
	    }
	    ctxt->charset = XML_CHAR_ENCODING_UTF8;
	} else {
	    handler = xmlFindCharEncodingHandler((const char *) encoding);
	    if (handler != NULL) {
		xmlSwitchToEncoding(ctxt, handler);
		ctxt->charset = XML_CHAR_ENCODING_UTF8;
	    } else {
		ctxt->errNo = XML_ERR_UNSUPPORTED_ENCODING;
	    }
	}

	if ((ctxt->input->buf != NULL) &&
	    (ctxt->input->buf->encoder != NULL) &&
	    (ctxt->input->buf->raw != NULL) &&
	    (ctxt->input->buf->buffer != NULL)) {
	    int nbchars;
	    int processed;

	    processed = ctxt->input->cur - ctxt->input->base;
	    xmlBufferShrink(ctxt->input->buf->buffer, processed);
	    nbchars = xmlCharEncInFunc(ctxt->input->buf->encoder,
		                       ctxt->input->buf->buffer,
				       ctxt->input->buf->raw);
	    if (nbchars < 0) {
		htmlParseErr(ctxt, XML_ERR_INVALID_ENCODING,
		             "htmlCheckEncoding: encoder error\n",
			     NULL, NULL);
	    }
	    ctxt->input->base =
	    ctxt->input->cur = ctxt->input->buf->buffer->content;
            ctxt->input->end =
                          &ctxt->input->base[ctxt->input->buf->buffer->use];
	}
    }
}

static void
htmlCheckMeta(htmlParserCtxtPtr ctxt, const xmlChar **atts) {
    int i;
    const xmlChar *att, *value;
    int http = 0;
    const xmlChar *content = NULL;

    if ((ctxt == NULL) || (atts == NULL))
	return;

    i = 0;
    att = atts[i++];
    while (att != NULL) {
	value = atts[i++];
	if ((value != NULL) && (!xmlStrcasecmp(att, BAD_CAST"http-equiv"))
	 && (!xmlStrcasecmp(value, BAD_CAST"Content-Type")))
	    http = 1;
	else if ((value != NULL) && (!xmlStrcasecmp(att, BAD_CAST"content")))
	    content = value;
	att = atts[i++];
    }
    if ((http) && (content != NULL))
	htmlCheckEncoding(ctxt, content);

}


static int
htmlParseStartTag(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    const xmlChar *attname;
    xmlChar *attvalue;
    const xmlChar **atts;
    int nbatts = 0;
    int maxatts;
    int meta = 0;
    int i;
    int discardtag = 0;

    if (ctxt->instate == XML_PARSER_EOF)
        return(-1);
    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseStartTag: context error\n", NULL, NULL);
	return -1;
    }
    if (CUR != '<') return -1;
    NEXT;

    atts = ctxt->atts;
    maxatts = ctxt->maxatts;

    GROW;
    name = htmlParseHTMLName(ctxt);
    if (name == NULL) {
	htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
	             "htmlParseStartTag: invalid element name\n",
		     NULL, NULL);
	
	while ((IS_CHAR_CH(CUR)) && (CUR != '>') &&
               (ctxt->instate != XML_PARSER_EOF))
	    NEXT;
        return -1;
    }
    if (xmlStrEqual(name, BAD_CAST"meta"))
	meta = 1;

    htmlAutoClose(ctxt, name);

    htmlCheckImplied(ctxt, name);

    if ((ctxt->nameNr > 0) && (xmlStrEqual(name, BAD_CAST"html"))) {
	htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
	             "htmlParseStartTag: misplaced <html> tag\n",
		     name, NULL);
	discardtag = 1;
	ctxt->depth++;
    }
    if ((ctxt->nameNr != 1) &&
	(xmlStrEqual(name, BAD_CAST"head"))) {
	htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
	             "htmlParseStartTag: misplaced <head> tag\n",
		     name, NULL);
	discardtag = 1;
	ctxt->depth++;
    }
    if (xmlStrEqual(name, BAD_CAST"body")) {
	int indx;
	for (indx = 0;indx < ctxt->nameNr;indx++) {
	    if (xmlStrEqual(ctxt->nameTab[indx], BAD_CAST"body")) {
		htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
		             "htmlParseStartTag: misplaced <body> tag\n",
			     name, NULL);
		discardtag = 1;
		ctxt->depth++;
	    }
	}
    }

    SKIP_BLANKS;
    while ((IS_CHAR_CH(CUR)) &&
           (CUR != '>') &&
	   ((CUR != '/') || (NXT(1) != '>'))) {
	long cons = ctxt->nbChars;

	GROW;
	attname = htmlParseAttribute(ctxt, &attvalue);
        if (attname != NULL) {

	    for (i = 0; i < nbatts;i += 2) {
	        if (xmlStrEqual(atts[i], attname)) {
		    htmlParseErr(ctxt, XML_ERR_ATTRIBUTE_REDEFINED,
		                 "Attribute %s redefined\n", attname, NULL);
		    if (attvalue != NULL)
			xmlFree(attvalue);
		    goto failed;
		}
	    }

	    if (atts == NULL) {
	        maxatts = 22; 
	        atts = (const xmlChar **)
		       xmlMalloc(maxatts * sizeof(xmlChar *));
		if (atts == NULL) {
		    htmlErrMemory(ctxt, NULL);
		    if (attvalue != NULL)
			xmlFree(attvalue);
		    goto failed;
		}
		ctxt->atts = atts;
		ctxt->maxatts = maxatts;
	    } else if (nbatts + 4 > maxatts) {
	        const xmlChar **n;

	        maxatts *= 2;
	        n = (const xmlChar **) xmlRealloc((void *) atts,
					     maxatts * sizeof(const xmlChar *));
		if (n == NULL) {
		    htmlErrMemory(ctxt, NULL);
		    if (attvalue != NULL)
			xmlFree(attvalue);
		    goto failed;
		}
		atts = n;
		ctxt->atts = atts;
		ctxt->maxatts = maxatts;
	    }
	    atts[nbatts++] = attname;
	    atts[nbatts++] = attvalue;
	    atts[nbatts] = NULL;
	    atts[nbatts + 1] = NULL;
	}
	else {
	    if (attvalue != NULL)
	        xmlFree(attvalue);
	    while ((IS_CHAR_CH(CUR)) &&
	           !(IS_BLANK_CH(CUR)) && (CUR != '>') &&
		   ((CUR != '/') || (NXT(1) != '>')))
		NEXT;
	}

failed:
	SKIP_BLANKS;
        if (cons == ctxt->nbChars) {
	    htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
	                 "htmlParseStartTag: problem parsing attributes\n",
			 NULL, NULL);
	    break;
	}
    }

    if (meta && (nbatts != 0))
	htmlCheckMeta(ctxt, atts);

    if (!discardtag) {
	htmlnamePush(ctxt, name);
	if ((ctxt->sax != NULL) && (ctxt->sax->startElement != NULL)) {
	    if (nbatts != 0)
		ctxt->sax->startElement(ctxt->userData, name, atts);
	    else
		ctxt->sax->startElement(ctxt->userData, name, NULL);
	}
    }

    if (atts != NULL) {
        for (i = 1;i < nbatts;i += 2) {
	    if (atts[i] != NULL)
		xmlFree((xmlChar *) atts[i]);
	}
    }

    return(discardtag);
}


static int
htmlParseEndTag(htmlParserCtxtPtr ctxt)
{
    const xmlChar *name;
    const xmlChar *oldname;
    int i, ret;

    if ((CUR != '<') || (NXT(1) != '/')) {
        htmlParseErr(ctxt, XML_ERR_LTSLASH_REQUIRED,
	             "htmlParseEndTag: '</' not found\n", NULL, NULL);
        return (0);
    }
    SKIP(2);

    name = htmlParseHTMLName(ctxt);
    if (name == NULL)
        return (0);
    SKIP_BLANKS;
    if ((!IS_CHAR_CH(CUR)) || (CUR != '>')) {
        htmlParseErr(ctxt, XML_ERR_GT_REQUIRED,
	             "End tag : expected '>'\n", NULL, NULL);
	if (ctxt->recovery) {
	    while (CUR != '\0' && CUR != '>') NEXT;
	    NEXT;
	}
    } else
        NEXT;

    if ((ctxt->depth > 0) &&
        (xmlStrEqual(name, BAD_CAST "html") ||
         xmlStrEqual(name, BAD_CAST "body") ||
	 xmlStrEqual(name, BAD_CAST "head"))) {
	ctxt->depth--;
	return (0);
    }

    for (i = (ctxt->nameNr - 1); i >= 0; i--) {
        if (xmlStrEqual(name, ctxt->nameTab[i]))
            break;
    }
    if (i < 0) {
        htmlParseErr(ctxt, XML_ERR_TAG_NAME_MISMATCH,
	             "Unexpected end tag : %s\n", name, NULL);
        return (0);
    }



    htmlAutoCloseOnClose(ctxt, name);

    if (!xmlStrEqual(name, ctxt->name)) {
        if ((ctxt->name != NULL) && (!xmlStrEqual(ctxt->name, name))) {
            htmlParseErr(ctxt, XML_ERR_TAG_NAME_MISMATCH,
	                 "Opening and ending tag mismatch: %s and %s\n",
			 name, ctxt->name);
        }
    }

    oldname = ctxt->name;
    if ((oldname != NULL) && (xmlStrEqual(oldname, name))) {
        if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
            ctxt->sax->endElement(ctxt->userData, name);
        htmlnamePop(ctxt);
        ret = 1;
    } else {
        ret = 0;
    }

    return (ret);
}


static void
htmlParseReference(htmlParserCtxtPtr ctxt) {
    const htmlEntityDesc * ent;
    xmlChar out[6];
    const xmlChar *name;
    if (CUR != '&') return;

    if (NXT(1) == '#') {
	unsigned int c;
	int bits, i = 0;

	c = htmlParseCharRef(ctxt);
	if (c == 0)
	    return;

        if      (c <    0x80) { out[i++]= c;                bits= -6; }
        else if (c <   0x800) { out[i++]=((c >>  6) & 0x1F) | 0xC0;  bits=  0; }
        else if (c < 0x10000) { out[i++]=((c >> 12) & 0x0F) | 0xE0;  bits=  6; }
        else                  { out[i++]=((c >> 18) & 0x07) | 0xF0;  bits= 12; }

        for ( ; bits >= 0; bits-= 6) {
            out[i++]= ((c >> bits) & 0x3F) | 0x80;
        }
	out[i] = 0;

	htmlCheckParagraph(ctxt);
	if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL))
	    ctxt->sax->characters(ctxt->userData, out, i);
    } else {
	ent = htmlParseEntityRef(ctxt, &name);
	if (name == NULL) {
	    htmlCheckParagraph(ctxt);
	    if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL))
	        ctxt->sax->characters(ctxt->userData, BAD_CAST "&", 1);
	    return;
	}
	if ((ent == NULL) || !(ent->value > 0)) {
	    htmlCheckParagraph(ctxt);
	    if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL)) {
		ctxt->sax->characters(ctxt->userData, BAD_CAST "&", 1);
		ctxt->sax->characters(ctxt->userData, name, xmlStrlen(name));
		
	    }
	} else {
	    unsigned int c;
	    int bits, i = 0;

	    c = ent->value;
	    if      (c <    0x80)
	            { out[i++]= c;                bits= -6; }
	    else if (c <   0x800)
	            { out[i++]=((c >>  6) & 0x1F) | 0xC0;  bits=  0; }
	    else if (c < 0x10000)
	            { out[i++]=((c >> 12) & 0x0F) | 0xE0;  bits=  6; }
	    else
	            { out[i++]=((c >> 18) & 0x07) | 0xF0;  bits= 12; }

	    for ( ; bits >= 0; bits-= 6) {
		out[i++]= ((c >> bits) & 0x3F) | 0x80;
	    }
	    out[i] = 0;

	    htmlCheckParagraph(ctxt);
	    if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL))
		ctxt->sax->characters(ctxt->userData, out, i);
	}
    }
}


static void
htmlParseContent(htmlParserCtxtPtr ctxt) {
    xmlChar *currentNode;
    int depth;
    const xmlChar *name;

    currentNode = xmlStrdup(ctxt->name);
    depth = ctxt->nameNr;
    while (1) {
	long cons = ctxt->nbChars;

        GROW;

        if (ctxt->instate == XML_PARSER_EOF)
            break;

        if ((CUR == '<') && (NXT(1) == '/')) {
	    if (htmlParseEndTag(ctxt) &&
		((currentNode != NULL) || (ctxt->nameNr == 0))) {
		if (currentNode != NULL)
		    xmlFree(currentNode);
		return;
	    }
	    continue; 
        }

	else if ((CUR == '<') &&
	         ((IS_ASCII_LETTER(NXT(1))) ||
		  (NXT(1) == '_') || (NXT(1) == ':'))) {
	    name = htmlParseHTMLName_nonInvasive(ctxt);
	    if (name == NULL) {
	        htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
			 "htmlParseStartTag: invalid element name\n",
			 NULL, NULL);
	        
        while ((IS_CHAR_CH(CUR)) && (CUR != '>'))
	            NEXT;

	        if (currentNode != NULL)
	            xmlFree(currentNode);
	        return;
	    }

	    if (ctxt->name != NULL) {
	        if (htmlCheckAutoClose(name, ctxt->name) == 1) {
	            htmlAutoClose(ctxt, name);
	            continue;
	        }
	    }
	}

        if ((ctxt->nameNr > 0) && (depth >= ctxt->nameNr) &&
	    (!xmlStrEqual(currentNode, ctxt->name)))
	     {
	    if (currentNode != NULL) xmlFree(currentNode);
	    return;
	}

	if ((CUR != 0) && ((xmlStrEqual(currentNode, BAD_CAST"script")) ||
	    (xmlStrEqual(currentNode, BAD_CAST"style")))) {
	    htmlParseScript(ctxt);
	} else {
	    if ((CUR == '<') && (NXT(1) == '!') &&
		(UPP(2) == 'D') && (UPP(3) == 'O') &&
		(UPP(4) == 'C') && (UPP(5) == 'T') &&
		(UPP(6) == 'Y') && (UPP(7) == 'P') &&
		(UPP(8) == 'E')) {
		htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
		             "Misplaced DOCTYPE declaration\n",
			     BAD_CAST "DOCTYPE" , NULL);
		htmlParseDocTypeDecl(ctxt);
	    }

	    if ((CUR == '<') && (NXT(1) == '!') &&
		(NXT(2) == '-') && (NXT(3) == '-')) {
		htmlParseComment(ctxt);
	    }

	    else if ((CUR == '<') && (NXT(1) == '?')) {
		htmlParsePI(ctxt);
	    }

	    else if (CUR == '<') {
		htmlParseElement(ctxt);
	    }

	    else if (CUR == '&') {
		htmlParseReference(ctxt);
	    }

	    else if (CUR == 0) {
		htmlAutoCloseOnEnd(ctxt);
		break;
	    }

	    else {
		htmlParseCharData(ctxt);
	    }

	    if (cons == ctxt->nbChars) {
		if (ctxt->node != NULL) {
		    htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		                 "detected an error in element content\n",
				 NULL, NULL);
		}
		break;
	    }
	}
        GROW;
    }
    if (currentNode != NULL) xmlFree(currentNode);
}


void
htmlParseElement(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    xmlChar *currentNode = NULL;
    const htmlElemDesc * info;
    htmlParserNodeInfo node_info;
    int failed;
    int depth;
    const xmlChar *oldptr;

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseElement: context error\n", NULL, NULL);
	return;
    }

    if (ctxt->instate == XML_PARSER_EOF)
        return;

    
    if (ctxt->record_info) {
        node_info.begin_pos = ctxt->input->consumed +
                          (CUR_PTR - ctxt->input->base);
	node_info.begin_line = ctxt->input->line;
    }

    failed = htmlParseStartTag(ctxt);
    name = ctxt->name;
    if ((failed == -1) || (name == NULL)) {
	if (CUR == '>')
	    NEXT;
        return;
    }

    info = htmlTagLookup(name);
    if (info == NULL) {
	htmlParseErr(ctxt, XML_HTML_UNKNOWN_TAG,
	             "Tag %s invalid\n", name, NULL);
    }

    if ((CUR == '/') && (NXT(1) == '>')) {
        SKIP(2);
	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
	    ctxt->sax->endElement(ctxt->userData, name);
	htmlnamePop(ctxt);
	return;
    }

    if (CUR == '>') {
        NEXT;
    } else {
	htmlParseErr(ctxt, XML_ERR_GT_REQUIRED,
	             "Couldn't find end of Start Tag %s\n", name, NULL);

	if (xmlStrEqual(name, ctxt->name)) {
	    nodePop(ctxt);
	    htmlnamePop(ctxt);
	}

	if (ctxt->record_info) {
	   node_info.end_pos = ctxt->input->consumed +
			      (CUR_PTR - ctxt->input->base);
	   node_info.end_line = ctxt->input->line;
	   node_info.node = ctxt->node;
	   xmlParserAddNodeInfo(ctxt, &node_info);
	}
	return;
    }

    if ((info != NULL) && (info->empty)) {
	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
	    ctxt->sax->endElement(ctxt->userData, name);
	htmlnamePop(ctxt);
	return;
    }

    currentNode = xmlStrdup(ctxt->name);
    depth = ctxt->nameNr;
    while (IS_CHAR_CH(CUR)) {
	oldptr = ctxt->input->cur;
	htmlParseContent(ctxt);
	if (oldptr==ctxt->input->cur) break;
	if (ctxt->nameNr < depth) break;
    }

    if ( currentNode != NULL && ctxt->record_info ) {
       node_info.end_pos = ctxt->input->consumed +
                          (CUR_PTR - ctxt->input->base);
       node_info.end_line = ctxt->input->line;
       node_info.node = ctxt->node;
       xmlParserAddNodeInfo(ctxt, &node_info);
    }
    if (!IS_CHAR_CH(CUR)) {
	htmlAutoCloseOnEnd(ctxt);
    }

    if (currentNode != NULL)
	xmlFree(currentNode);
}

static void
htmlParserFinishElementParsing(htmlParserCtxtPtr ctxt) {
    if ( ctxt->node != NULL && ctxt->record_info ) {
       ctxt->nodeInfo->end_pos = ctxt->input->consumed +
                                (CUR_PTR - ctxt->input->base);
       ctxt->nodeInfo->end_line = ctxt->input->line;
       ctxt->nodeInfo->node = ctxt->node;
       xmlParserAddNodeInfo(ctxt, ctxt->nodeInfo);
       htmlNodeInfoPop(ctxt);
    }
    if (!IS_CHAR_CH(CUR)) {
       htmlAutoCloseOnEnd(ctxt);
    }
}


static void
htmlParseElementInternal(htmlParserCtxtPtr ctxt) {
    const xmlChar *name;
    const htmlElemDesc * info;
    htmlParserNodeInfo node_info;
    int failed;

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseElementInternal: context error\n", NULL, NULL);
	return;
    }

    if (ctxt->instate == XML_PARSER_EOF)
        return;

    
    if (ctxt->record_info) {
        node_info.begin_pos = ctxt->input->consumed +
                          (CUR_PTR - ctxt->input->base);
	node_info.begin_line = ctxt->input->line;
    }

    failed = htmlParseStartTag(ctxt);
    name = ctxt->name;
    if ((failed == -1) || (name == NULL)) {
	if (CUR == '>')
	    NEXT;
        return;
    }

    info = htmlTagLookup(name);
    if (info == NULL) {
	htmlParseErr(ctxt, XML_HTML_UNKNOWN_TAG,
	             "Tag %s invalid\n", name, NULL);
    }

    if ((CUR == '/') && (NXT(1) == '>')) {
        SKIP(2);
	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
	    ctxt->sax->endElement(ctxt->userData, name);
	htmlnamePop(ctxt);
	return;
    }

    if (CUR == '>') {
        NEXT;
    } else {
	htmlParseErr(ctxt, XML_ERR_GT_REQUIRED,
	             "Couldn't find end of Start Tag %s\n", name, NULL);

	if (xmlStrEqual(name, ctxt->name)) {
	    nodePop(ctxt);
	    htmlnamePop(ctxt);
	}

        if (ctxt->record_info)
            htmlNodeInfoPush(ctxt, &node_info);
        htmlParserFinishElementParsing(ctxt);
	return;
    }

    if ((info != NULL) && (info->empty)) {
	if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
	    ctxt->sax->endElement(ctxt->userData, name);
	htmlnamePop(ctxt);
	return;
    }

    if (ctxt->record_info)
        htmlNodeInfoPush(ctxt, &node_info);
}


static void
htmlParseContentInternal(htmlParserCtxtPtr ctxt) {
    xmlChar *currentNode;
    int depth;
    const xmlChar *name;

    currentNode = xmlStrdup(ctxt->name);
    depth = ctxt->nameNr;
    while (1) {
	long cons = ctxt->nbChars;

        GROW;

        if (ctxt->instate == XML_PARSER_EOF)
            break;

        if ((CUR == '<') && (NXT(1) == '/')) {
	    if (htmlParseEndTag(ctxt) &&
		((currentNode != NULL) || (ctxt->nameNr == 0))) {
		if (currentNode != NULL)
		    xmlFree(currentNode);

	        currentNode = xmlStrdup(ctxt->name);
	        depth = ctxt->nameNr;
	    }
	    continue; 
        }

	else if ((CUR == '<') &&
	         ((IS_ASCII_LETTER(NXT(1))) ||
		  (NXT(1) == '_') || (NXT(1) == ':'))) {
	    name = htmlParseHTMLName_nonInvasive(ctxt);
	    if (name == NULL) {
	        htmlParseErr(ctxt, XML_ERR_NAME_REQUIRED,
			 "htmlParseStartTag: invalid element name\n",
			 NULL, NULL);
	        
	        while ((IS_CHAR_CH(CUR)) && (CUR != '>'))
	            NEXT;

	        htmlParserFinishElementParsing(ctxt);
	        if (currentNode != NULL)
	            xmlFree(currentNode);

	        currentNode = xmlStrdup(ctxt->name);
	        depth = ctxt->nameNr;
	        continue;
	    }

	    if (ctxt->name != NULL) {
	        if (htmlCheckAutoClose(name, ctxt->name) == 1) {
	            htmlAutoClose(ctxt, name);
	            continue;
	        }
	    }
	}

        if ((ctxt->nameNr > 0) && (depth >= ctxt->nameNr) &&
	    (!xmlStrEqual(currentNode, ctxt->name)))
	     {
	    htmlParserFinishElementParsing(ctxt);
	    if (currentNode != NULL) xmlFree(currentNode);

	    currentNode = xmlStrdup(ctxt->name);
	    depth = ctxt->nameNr;
	    continue;
	}

	if ((CUR != 0) && ((xmlStrEqual(currentNode, BAD_CAST"script")) ||
	    (xmlStrEqual(currentNode, BAD_CAST"style")))) {
	    htmlParseScript(ctxt);
	} else {
	    if ((CUR == '<') && (NXT(1) == '!') &&
		(UPP(2) == 'D') && (UPP(3) == 'O') &&
		(UPP(4) == 'C') && (UPP(5) == 'T') &&
		(UPP(6) == 'Y') && (UPP(7) == 'P') &&
		(UPP(8) == 'E')) {
		htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
		             "Misplaced DOCTYPE declaration\n",
			     BAD_CAST "DOCTYPE" , NULL);
		htmlParseDocTypeDecl(ctxt);
	    }

	    if ((CUR == '<') && (NXT(1) == '!') &&
		(NXT(2) == '-') && (NXT(3) == '-')) {
		htmlParseComment(ctxt);
	    }

	    else if ((CUR == '<') && (NXT(1) == '?')) {
		htmlParsePI(ctxt);
	    }

	    else if (CUR == '<') {
		htmlParseElementInternal(ctxt);
		if (currentNode != NULL) xmlFree(currentNode);

		currentNode = xmlStrdup(ctxt->name);
		depth = ctxt->nameNr;
	    }

	    else if (CUR == '&') {
		htmlParseReference(ctxt);
	    }

	    else if (CUR == 0) {
		htmlAutoCloseOnEnd(ctxt);
		break;
	    }

	    else {
		htmlParseCharData(ctxt);
	    }

	    if (cons == ctxt->nbChars) {
		if (ctxt->node != NULL) {
		    htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		                 "detected an error in element content\n",
				 NULL, NULL);
		}
		break;
	    }
	}
        GROW;
    }
    if (currentNode != NULL) xmlFree(currentNode);
}


void
__htmlParseContent(void *ctxt) {
    if (ctxt != NULL)
	htmlParseContentInternal((htmlParserCtxtPtr) ctxt);
}


int
htmlParseDocument(htmlParserCtxtPtr ctxt) {
    xmlChar start[4];
    xmlCharEncoding enc;
    xmlDtdPtr dtd;

    xmlInitParser();

    htmlDefaultSAXHandlerInit();

    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseDocument: context error\n", NULL, NULL);
	return(XML_ERR_INTERNAL_ERROR);
    }
    ctxt->html = 1;
    ctxt->linenumbers = 1;
    GROW;
    if ((ctxt->sax) && (ctxt->sax->setDocumentLocator))
        ctxt->sax->setDocumentLocator(ctxt->userData, &xmlDefaultSAXLocator);

    if ((ctxt->encoding == (const xmlChar *)XML_CHAR_ENCODING_NONE) &&
        ((ctxt->input->end - ctxt->input->cur) >= 4)) {
	start[0] = RAW;
	start[1] = NXT(1);
	start[2] = NXT(2);
	start[3] = NXT(3);
	enc = xmlDetectCharEncoding(&start[0], 4);
	if (enc != XML_CHAR_ENCODING_NONE) {
	    xmlSwitchEncoding(ctxt, enc);
	}
    }

    SKIP_BLANKS;
    if (CUR == 0) {
	htmlParseErr(ctxt, XML_ERR_DOCUMENT_EMPTY,
	             "Document is empty\n", NULL, NULL);
    }

    if ((ctxt->sax) && (ctxt->sax->startDocument) && (!ctxt->disableSAX))
	ctxt->sax->startDocument(ctxt->userData);


    while (((CUR == '<') && (NXT(1) == '!') &&
            (NXT(2) == '-') && (NXT(3) == '-')) ||
	   ((CUR == '<') && (NXT(1) == '?'))) {
        htmlParseComment(ctxt);
        htmlParsePI(ctxt);
	SKIP_BLANKS;
    }


    if ((CUR == '<') && (NXT(1) == '!') &&
	(UPP(2) == 'D') && (UPP(3) == 'O') &&
	(UPP(4) == 'C') && (UPP(5) == 'T') &&
	(UPP(6) == 'Y') && (UPP(7) == 'P') &&
	(UPP(8) == 'E')) {
	htmlParseDocTypeDecl(ctxt);
    }
    SKIP_BLANKS;

    while (((CUR == '<') && (NXT(1) == '!') &&
            (NXT(2) == '-') && (NXT(3) == '-')) ||
	   ((CUR == '<') && (NXT(1) == '?'))) {
        htmlParseComment(ctxt);
        htmlParsePI(ctxt);
	SKIP_BLANKS;
    }

    htmlParseContentInternal(ctxt);

    if (CUR == 0)
	htmlAutoCloseOnEnd(ctxt);


    if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
        ctxt->sax->endDocument(ctxt->userData);

    if (ctxt->myDoc != NULL) {
	dtd = xmlGetIntSubset(ctxt->myDoc);
	if (dtd == NULL)
	    ctxt->myDoc->intSubset =
		xmlCreateIntSubset(ctxt->myDoc, BAD_CAST "html",
		    BAD_CAST "-//W3C//DTD HTML 4.0 Transitional//EN",
		    BAD_CAST "http://www.w3.org/TR/REC-html40/loose.dtd");
    }
    if (! ctxt->wellFormed) return(-1);
    return(0);
}




static int
htmlInitParserCtxt(htmlParserCtxtPtr ctxt)
{
    htmlSAXHandler *sax;

    if (ctxt == NULL) return(-1);
    memset(ctxt, 0, sizeof(htmlParserCtxt));

    ctxt->dict = xmlDictCreate();
    if (ctxt->dict == NULL) {
        htmlErrMemory(NULL, "htmlInitParserCtxt: out of memory\n");
	return(-1);
    }
    sax = (htmlSAXHandler *) xmlMalloc(sizeof(htmlSAXHandler));
    if (sax == NULL) {
        htmlErrMemory(NULL, "htmlInitParserCtxt: out of memory\n");
	return(-1);
    }
    else
        memset(sax, 0, sizeof(htmlSAXHandler));

    
    ctxt->inputTab = (htmlParserInputPtr *)
                      xmlMalloc(5 * sizeof(htmlParserInputPtr));
    if (ctxt->inputTab == NULL) {
        htmlErrMemory(NULL, "htmlInitParserCtxt: out of memory\n");
	ctxt->inputNr = 0;
	ctxt->inputMax = 0;
	ctxt->input = NULL;
	return(-1);
    }
    ctxt->inputNr = 0;
    ctxt->inputMax = 5;
    ctxt->input = NULL;
    ctxt->version = NULL;
    ctxt->encoding = NULL;
    ctxt->standalone = -1;
    ctxt->instate = XML_PARSER_START;

    
    ctxt->nodeTab = (htmlNodePtr *) xmlMalloc(10 * sizeof(htmlNodePtr));
    if (ctxt->nodeTab == NULL) {
        htmlErrMemory(NULL, "htmlInitParserCtxt: out of memory\n");
	ctxt->nodeNr = 0;
	ctxt->nodeMax = 0;
	ctxt->node = NULL;
	ctxt->inputNr = 0;
	ctxt->inputMax = 0;
	ctxt->input = NULL;
	return(-1);
    }
    ctxt->nodeNr = 0;
    ctxt->nodeMax = 10;
    ctxt->node = NULL;

    
    ctxt->nameTab = (const xmlChar **) xmlMalloc(10 * sizeof(xmlChar *));
    if (ctxt->nameTab == NULL) {
        htmlErrMemory(NULL, "htmlInitParserCtxt: out of memory\n");
	ctxt->nameNr = 0;
	ctxt->nameMax = 0;
	ctxt->name = NULL;
	ctxt->nodeNr = 0;
	ctxt->nodeMax = 0;
	ctxt->node = NULL;
	ctxt->inputNr = 0;
	ctxt->inputMax = 0;
	ctxt->input = NULL;
	return(-1);
    }
    ctxt->nameNr = 0;
    ctxt->nameMax = 10;
    ctxt->name = NULL;

    ctxt->nodeInfoTab = NULL;
    ctxt->nodeInfoNr  = 0;
    ctxt->nodeInfoMax = 0;

    if (sax == NULL) ctxt->sax = (xmlSAXHandlerPtr) &htmlDefaultSAXHandler;
    else {
        ctxt->sax = sax;
	memcpy(sax, &htmlDefaultSAXHandler, sizeof(xmlSAXHandlerV1));
    }
    ctxt->userData = ctxt;
    ctxt->myDoc = NULL;
    ctxt->wellFormed = 1;
    ctxt->replaceEntities = 0;
    ctxt->linenumbers = xmlLineNumbersDefaultValue;
    ctxt->html = 1;
    ctxt->vctxt.finishDtd = XML_CTXT_FINISH_DTD_0;
    ctxt->vctxt.userData = ctxt;
    ctxt->vctxt.error = xmlParserValidityError;
    ctxt->vctxt.warning = xmlParserValidityWarning;
    ctxt->record_info = 0;
    ctxt->validate = 0;
    ctxt->nbChars = 0;
    ctxt->checkIndex = 0;
    ctxt->catalogs = NULL;
    xmlInitNodeInfoSeq(&ctxt->node_seq);
    return(0);
}


void
htmlFreeParserCtxt(htmlParserCtxtPtr ctxt)
{
    xmlFreeParserCtxt(ctxt);
}


htmlParserCtxtPtr
htmlNewParserCtxt(void)
{
    xmlParserCtxtPtr ctxt;

    ctxt = (xmlParserCtxtPtr) xmlMalloc(sizeof(xmlParserCtxt));
    if (ctxt == NULL) {
        htmlErrMemory(NULL, "NewParserCtxt: out of memory\n");
	return(NULL);
    }
    memset(ctxt, 0, sizeof(xmlParserCtxt));
    if (htmlInitParserCtxt(ctxt) < 0) {
        htmlFreeParserCtxt(ctxt);
	return(NULL);
    }
    return(ctxt);
}

htmlParserCtxtPtr
htmlCreateMemoryParserCtxt(const char *buffer, int size) {
    xmlParserCtxtPtr ctxt;
    xmlParserInputPtr input;
    xmlParserInputBufferPtr buf;

    if (buffer == NULL)
	return(NULL);
    if (size <= 0)
	return(NULL);

    ctxt = htmlNewParserCtxt();
    if (ctxt == NULL)
	return(NULL);

    buf = xmlParserInputBufferCreateMem(buffer, size, XML_CHAR_ENCODING_NONE);
    if (buf == NULL) return(NULL);

    input = xmlNewInputStream(ctxt);
    if (input == NULL) {
	xmlFreeParserCtxt(ctxt);
	return(NULL);
    }

    input->filename = NULL;
    input->buf = buf;
    input->base = input->buf->buffer->content;
    input->cur = input->buf->buffer->content;
    input->end = &input->buf->buffer->content[input->buf->buffer->use];

    inputPush(ctxt, input);
    return(ctxt);
}

static htmlParserCtxtPtr
htmlCreateDocParserCtxt(const xmlChar *cur, const char *encoding) {
    int len;
    htmlParserCtxtPtr ctxt;

    if (cur == NULL)
	return(NULL);
    len = xmlStrlen(cur);
    ctxt = htmlCreateMemoryParserCtxt((char *)cur, len);
    if (ctxt == NULL)
	return(NULL);

    if (encoding != NULL) {
	xmlCharEncoding enc;
	xmlCharEncodingHandlerPtr handler;

	if (ctxt->input->encoding != NULL)
	    xmlFree((xmlChar *) ctxt->input->encoding);
	ctxt->input->encoding = xmlStrdup((const xmlChar *) encoding);

	enc = xmlParseCharEncoding(encoding);
	if (enc != XML_CHAR_ENCODING_ERROR) {
	    xmlSwitchEncoding(ctxt, enc);
	    if (ctxt->errNo == XML_ERR_UNSUPPORTED_ENCODING) {
		htmlParseErr(ctxt, XML_ERR_UNSUPPORTED_ENCODING,
		             "Unsupported encoding %s\n",
			     (const xmlChar *) encoding, NULL);
	    }
	} else {
	    handler = xmlFindCharEncodingHandler((const char *) encoding);
	    if (handler != NULL) {
		xmlSwitchToEncoding(ctxt, handler);
	    } else {
		htmlParseErr(ctxt, XML_ERR_UNSUPPORTED_ENCODING,
		             "Unsupported encoding %s\n",
			     (const xmlChar *) encoding, NULL);
	    }
	}
    }
    return(ctxt);
}

#ifdef LIBXML_PUSH_ENABLED

static int
htmlParseLookupSequence(htmlParserCtxtPtr ctxt, xmlChar first,
                        xmlChar next, xmlChar third, int iscomment,
                        int ignoreattrval)
{
    int base, len;
    htmlParserInputPtr in;
    const xmlChar *buf;
    int incomment = 0;
    int invalue = 0;
    char valdellim = 0x0;

    in = ctxt->input;
    if (in == NULL)
        return (-1);

    base = in->cur - in->base;
    if (base < 0)
        return (-1);

    if (ctxt->checkIndex > base)
        base = ctxt->checkIndex;

    if (in->buf == NULL) {
        buf = in->base;
        len = in->length;
    } else {
        buf = in->buf->buffer->content;
        len = in->buf->buffer->use;
    }

    
    if (third)
        len -= 2;
    else if (next)
        len--;
    for (; base < len; base++) {
        if ((!incomment) && (base + 4 < len) && (!iscomment)) {
            if ((buf[base] == '<') && (buf[base + 1] == '!') &&
                (buf[base + 2] == '-') && (buf[base + 3] == '-')) {
                incomment = 1;
                
                base += 2;
            }
        }
        if (ignoreattrval) {
            if (buf[base] == '"' || buf[base] == '\'') {
                if (invalue) {
                    if (buf[base] == valdellim) {
                        invalue = 0;
                        continue;
                    }
                } else {
                    valdellim = buf[base];
                    invalue = 1;
                    continue;
                }
            } else if (invalue) {
                continue;
            }
        }
        if (incomment) {
            if (base + 3 > len)
                return (-1);
            if ((buf[base] == '-') && (buf[base + 1] == '-') &&
                (buf[base + 2] == '>')) {
                incomment = 0;
                base += 2;
            }
            continue;
        }
        if (buf[base] == first) {
            if (third != 0) {
                if ((buf[base + 1] != next) || (buf[base + 2] != third))
                    continue;
            } else if (next != 0) {
                if (buf[base + 1] != next)
                    continue;
            }
            ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
            if (next == 0)
                xmlGenericError(xmlGenericErrorContext,
                                "HPP: lookup '%c' found at %d\n",
                                first, base);
            else if (third == 0)
                xmlGenericError(xmlGenericErrorContext,
                                "HPP: lookup '%c%c' found at %d\n",
                                first, next, base);
            else
                xmlGenericError(xmlGenericErrorContext,
                                "HPP: lookup '%c%c%c' found at %d\n",
                                first, next, third, base);
#endif
            return (base - (in->cur - in->base));
        }
    }
    if ((!incomment) && (!invalue))
        ctxt->checkIndex = base;
#ifdef DEBUG_PUSH
    if (next == 0)
        xmlGenericError(xmlGenericErrorContext,
                        "HPP: lookup '%c' failed\n", first);
    else if (third == 0)
        xmlGenericError(xmlGenericErrorContext,
                        "HPP: lookup '%c%c' failed\n", first, next);
    else
        xmlGenericError(xmlGenericErrorContext,
                        "HPP: lookup '%c%c%c' failed\n", first, next,
                        third);
#endif
    return (-1);
}

static int
htmlParseLookupChars(htmlParserCtxtPtr ctxt, const xmlChar * stop,
                     int stopLen)
{
    int base, len;
    htmlParserInputPtr in;
    const xmlChar *buf;
    int incomment = 0;
    int i;

    in = ctxt->input;
    if (in == NULL)
        return (-1);

    base = in->cur - in->base;
    if (base < 0)
        return (-1);

    if (ctxt->checkIndex > base)
        base = ctxt->checkIndex;

    if (in->buf == NULL) {
        buf = in->base;
        len = in->length;
    } else {
        buf = in->buf->buffer->content;
        len = in->buf->buffer->use;
    }

    for (; base < len; base++) {
        if (!incomment && (base + 4 < len)) {
            if ((buf[base] == '<') && (buf[base + 1] == '!') &&
                (buf[base + 2] == '-') && (buf[base + 3] == '-')) {
                incomment = 1;
                
                base += 2;
            }
        }
        if (incomment) {
            if (base + 3 > len)
                return (-1);
            if ((buf[base] == '-') && (buf[base + 1] == '-') &&
                (buf[base + 2] == '>')) {
                incomment = 0;
                base += 2;
            }
            continue;
        }
        for (i = 0; i < stopLen; ++i) {
            if (buf[base] == stop[i]) {
                ctxt->checkIndex = 0;
                return (base - (in->cur - in->base));
            }
        }
    }
    ctxt->checkIndex = base;
    return (-1);
}

static int
htmlParseTryOrFinish(htmlParserCtxtPtr ctxt, int terminate) {
    int ret = 0;
    htmlParserInputPtr in;
    int avail = 0;
    xmlChar cur, next;

#ifdef DEBUG_PUSH
    switch (ctxt->instate) {
	case XML_PARSER_EOF:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try EOF\n"); break;
	case XML_PARSER_START:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try START\n"); break;
	case XML_PARSER_MISC:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try MISC\n");break;
	case XML_PARSER_COMMENT:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try COMMENT\n");break;
	case XML_PARSER_PROLOG:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try PROLOG\n");break;
	case XML_PARSER_START_TAG:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try START_TAG\n");break;
	case XML_PARSER_CONTENT:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try CONTENT\n");break;
	case XML_PARSER_CDATA_SECTION:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try CDATA_SECTION\n");break;
	case XML_PARSER_END_TAG:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try END_TAG\n");break;
	case XML_PARSER_ENTITY_DECL:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try ENTITY_DECL\n");break;
	case XML_PARSER_ENTITY_VALUE:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try ENTITY_VALUE\n");break;
	case XML_PARSER_ATTRIBUTE_VALUE:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try ATTRIBUTE_VALUE\n");break;
	case XML_PARSER_DTD:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try DTD\n");break;
	case XML_PARSER_EPILOG:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try EPILOG\n");break;
	case XML_PARSER_PI:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try PI\n");break;
	case XML_PARSER_SYSTEM_LITERAL:
	    xmlGenericError(xmlGenericErrorContext,
		    "HPP: try SYSTEM_LITERAL\n");break;
    }
#endif

    while (1) {

	in = ctxt->input;
	if (in == NULL) break;
	if (in->buf == NULL)
	    avail = in->length - (in->cur - in->base);
	else
	    avail = in->buf->buffer->use - (in->cur - in->base);
	if ((avail == 0) && (terminate)) {
	    htmlAutoCloseOnEnd(ctxt);
	    if ((ctxt->nameNr == 0) && (ctxt->instate != XML_PARSER_EOF)) {
		ctxt->instate = XML_PARSER_EOF;
		if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
		    ctxt->sax->endDocument(ctxt->userData);
	    }
	}
        if (avail < 1)
	    goto done;
	cur = in->cur[0];
	if (cur == 0) {
	    SKIP(1);
	    continue;
	}

        switch (ctxt->instate) {
            case XML_PARSER_EOF:
	        goto done;
            case XML_PARSER_START:
		cur = in->cur[0];
		if (IS_BLANK_CH(cur)) {
		    SKIP_BLANKS;
		    if (in->buf == NULL)
			avail = in->length - (in->cur - in->base);
		    else
			avail = in->buf->buffer->use - (in->cur - in->base);
		}
		if ((ctxt->sax) && (ctxt->sax->setDocumentLocator))
		    ctxt->sax->setDocumentLocator(ctxt->userData,
						  &xmlDefaultSAXLocator);
		if ((ctxt->sax) && (ctxt->sax->startDocument) &&
	            (!ctxt->disableSAX))
		    ctxt->sax->startDocument(ctxt->userData);

		cur = in->cur[0];
		next = in->cur[1];
		if ((cur == '<') && (next == '!') &&
		    (UPP(2) == 'D') && (UPP(3) == 'O') &&
		    (UPP(4) == 'C') && (UPP(5) == 'T') &&
		    (UPP(6) == 'Y') && (UPP(7) == 'P') &&
		    (UPP(8) == 'E')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing internal subset\n");
#endif
		    htmlParseDocTypeDecl(ctxt);
		    ctxt->instate = XML_PARSER_PROLOG;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering PROLOG\n");
#endif
                } else {
		    ctxt->instate = XML_PARSER_MISC;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering MISC\n");
#endif
		}
		break;
            case XML_PARSER_MISC:
		SKIP_BLANKS;
		if (in->buf == NULL)
		    avail = in->length - (in->cur - in->base);
		else
		    avail = in->buf->buffer->use - (in->cur - in->base);
		if (avail < 2)
		    goto done;
		cur = in->cur[0];
		next = in->cur[1];
	        if ((cur == '<') && (next == '!') &&
		    (in->cur[2] == '-') && (in->cur[3] == '-')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '-', '-', '>', 1, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing Comment\n");
#endif
		    htmlParseComment(ctxt);
		    ctxt->instate = XML_PARSER_MISC;
	        } else if ((cur == '<') && (next == '?')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing PI\n");
#endif
		    htmlParsePI(ctxt);
		    ctxt->instate = XML_PARSER_MISC;
		} else if ((cur == '<') && (next == '!') &&
		    (UPP(2) == 'D') && (UPP(3) == 'O') &&
		    (UPP(4) == 'C') && (UPP(5) == 'T') &&
		    (UPP(6) == 'Y') && (UPP(7) == 'P') &&
		    (UPP(8) == 'E')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing internal subset\n");
#endif
		    htmlParseDocTypeDecl(ctxt);
		    ctxt->instate = XML_PARSER_PROLOG;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering PROLOG\n");
#endif
		} else if ((cur == '<') && (next == '!') &&
		           (avail < 9)) {
		    goto done;
		} else {
		    ctxt->instate = XML_PARSER_START_TAG;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering START_TAG\n");
#endif
		}
		break;
            case XML_PARSER_PROLOG:
		SKIP_BLANKS;
		if (in->buf == NULL)
		    avail = in->length - (in->cur - in->base);
		else
		    avail = in->buf->buffer->use - (in->cur - in->base);
		if (avail < 2)
		    goto done;
		cur = in->cur[0];
		next = in->cur[1];
		if ((cur == '<') && (next == '!') &&
		    (in->cur[2] == '-') && (in->cur[3] == '-')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '-', '-', '>', 1, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing Comment\n");
#endif
		    htmlParseComment(ctxt);
		    ctxt->instate = XML_PARSER_PROLOG;
	        } else if ((cur == '<') && (next == '?')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing PI\n");
#endif
		    htmlParsePI(ctxt);
		    ctxt->instate = XML_PARSER_PROLOG;
		} else if ((cur == '<') && (next == '!') &&
		           (avail < 4)) {
		    goto done;
		} else {
		    ctxt->instate = XML_PARSER_START_TAG;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering START_TAG\n");
#endif
		}
		break;
            case XML_PARSER_EPILOG:
		if (in->buf == NULL)
		    avail = in->length - (in->cur - in->base);
		else
		    avail = in->buf->buffer->use - (in->cur - in->base);
		if (avail < 1)
		    goto done;
		cur = in->cur[0];
		if (IS_BLANK_CH(cur)) {
		    htmlParseCharData(ctxt);
		    goto done;
		}
		if (avail < 2)
		    goto done;
		next = in->cur[1];
	        if ((cur == '<') && (next == '!') &&
		    (in->cur[2] == '-') && (in->cur[3] == '-')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '-', '-', '>', 1, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing Comment\n");
#endif
		    htmlParseComment(ctxt);
		    ctxt->instate = XML_PARSER_EPILOG;
	        } else if ((cur == '<') && (next == '?')) {
		    if ((!terminate) &&
		        (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			goto done;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: Parsing PI\n");
#endif
		    htmlParsePI(ctxt);
		    ctxt->instate = XML_PARSER_EPILOG;
		} else if ((cur == '<') && (next == '!') &&
		           (avail < 4)) {
		    goto done;
		} else {
		    ctxt->errNo = XML_ERR_DOCUMENT_END;
		    ctxt->wellFormed = 0;
		    ctxt->instate = XML_PARSER_EOF;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering EOF\n");
#endif
		    if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
			ctxt->sax->endDocument(ctxt->userData);
		    goto done;
		}
		break;
            case XML_PARSER_START_TAG: {
	        const xmlChar *name;
		int failed;
		const htmlElemDesc * info;

		if (avail < 2)
		    goto done;
		cur = in->cur[0];
	        if (cur != '<') {
		    ctxt->instate = XML_PARSER_CONTENT;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering CONTENT\n");
#endif
		    break;
		}
		if (in->cur[1] == '/') {
		    ctxt->instate = XML_PARSER_END_TAG;
		    ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering END_TAG\n");
#endif
		    break;
		}
		if ((!terminate) &&
		    (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
		    goto done;

		failed = htmlParseStartTag(ctxt);
		name = ctxt->name;
		if ((failed == -1) ||
		    (name == NULL)) {
		    if (CUR == '>')
			NEXT;
		    break;
		}

		info = htmlTagLookup(name);
		if (info == NULL) {
		    htmlParseErr(ctxt, XML_HTML_UNKNOWN_TAG,
		                 "Tag %s invalid\n", name, NULL);
		}

		if ((CUR == '/') && (NXT(1) == '>')) {
		    SKIP(2);
		    if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
			ctxt->sax->endElement(ctxt->userData, name);
		    htmlnamePop(ctxt);
		    ctxt->instate = XML_PARSER_CONTENT;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering CONTENT\n");
#endif
		    break;
		}

		if (CUR == '>') {
		    NEXT;
		} else {
		    htmlParseErr(ctxt, XML_ERR_GT_REQUIRED,
		                 "Couldn't find end of Start Tag %s\n",
				 name, NULL);

		    if (xmlStrEqual(name, ctxt->name)) {
			nodePop(ctxt);
			htmlnamePop(ctxt);
		    }

		    ctxt->instate = XML_PARSER_CONTENT;
#ifdef DEBUG_PUSH
		    xmlGenericError(xmlGenericErrorContext,
			    "HPP: entering CONTENT\n");
#endif
		    break;
		}

		if ((info != NULL) && (info->empty)) {
		    if ((ctxt->sax != NULL) && (ctxt->sax->endElement != NULL))
			ctxt->sax->endElement(ctxt->userData, name);
		    htmlnamePop(ctxt);
		}
		ctxt->instate = XML_PARSER_CONTENT;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
                break;
	    }
            case XML_PARSER_CONTENT: {
		long cons;
		if (ctxt->token != 0) {
		    xmlChar chr[2] = { 0 , 0 } ;

		    chr[0] = (xmlChar) ctxt->token;
		    htmlCheckParagraph(ctxt);
		    if ((ctxt->sax != NULL) && (ctxt->sax->characters != NULL))
			ctxt->sax->characters(ctxt->userData, chr, 1);
		    ctxt->token = 0;
		    ctxt->checkIndex = 0;
		}
		if ((avail == 1) && (terminate)) {
		    cur = in->cur[0];
		    if ((cur != '<') && (cur != '&')) {
			if (ctxt->sax != NULL) {
			    if (IS_BLANK_CH(cur)) {
				if (ctxt->sax->ignorableWhitespace != NULL)
				    ctxt->sax->ignorableWhitespace(
					    ctxt->userData, &cur, 1);
			    } else {
				htmlCheckParagraph(ctxt);
				if (ctxt->sax->characters != NULL)
				    ctxt->sax->characters(
					    ctxt->userData, &cur, 1);
			    }
			}
			ctxt->token = 0;
			ctxt->checkIndex = 0;
			in->cur++;
			break;
		    }
		}
		if (avail < 2)
		    goto done;
		cur = in->cur[0];
		next = in->cur[1];
		cons = ctxt->nbChars;
		if ((xmlStrEqual(ctxt->name, BAD_CAST"script")) ||
		    (xmlStrEqual(ctxt->name, BAD_CAST"style"))) {
		    if (!terminate) {
		        int idx;
			xmlChar val;

			idx = htmlParseLookupSequence(ctxt, '<', '/', 0, 0, 1);
			if (idx < 0)
			    goto done;
		        val = in->cur[idx + 2];
			if (val == 0) 
			    goto done;
		    }
		    htmlParseScript(ctxt);
		    if ((cur == '<') && (next == '/')) {
			ctxt->instate = XML_PARSER_END_TAG;
			ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: entering END_TAG\n");
#endif
			break;
		    }
		} else {
		    if ((cur == '<') && (next == '!') &&
			(UPP(2) == 'D') && (UPP(3) == 'O') &&
			(UPP(4) == 'C') && (UPP(5) == 'T') &&
			(UPP(6) == 'Y') && (UPP(7) == 'P') &&
			(UPP(8) == 'E')) {
			if ((!terminate) &&
			    (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			    goto done;
			htmlParseErr(ctxt, XML_HTML_STRUCURE_ERROR,
			             "Misplaced DOCTYPE declaration\n",
				     BAD_CAST "DOCTYPE" , NULL);
			htmlParseDocTypeDecl(ctxt);
		    } else if ((cur == '<') && (next == '!') &&
			(in->cur[2] == '-') && (in->cur[3] == '-')) {
			if ((!terminate) &&
			    (htmlParseLookupSequence(
				ctxt, '-', '-', '>', 1, 1) < 0))
			    goto done;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: Parsing Comment\n");
#endif
			htmlParseComment(ctxt);
			ctxt->instate = XML_PARSER_CONTENT;
		    } else if ((cur == '<') && (next == '?')) {
			if ((!terminate) &&
			    (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
			    goto done;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: Parsing PI\n");
#endif
			htmlParsePI(ctxt);
			ctxt->instate = XML_PARSER_CONTENT;
		    } else if ((cur == '<') && (next == '!') && (avail < 4)) {
			goto done;
		    } else if ((cur == '<') && (next == '/')) {
			ctxt->instate = XML_PARSER_END_TAG;
			ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: entering END_TAG\n");
#endif
			break;
		    } else if (cur == '<') {
			ctxt->instate = XML_PARSER_START_TAG;
			ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: entering START_TAG\n");
#endif
			break;
		    } else if (cur == '&') {
			if ((!terminate) &&
			    (htmlParseLookupChars(ctxt,
                                                  BAD_CAST "; >/", 4) < 0))
			    goto done;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: Parsing Reference\n");
#endif
			
			htmlParseReference(ctxt);
		    } else {
			if ((!terminate) &&
                            (htmlParseLookupChars(ctxt, BAD_CAST "<&", 2) < 0))
			    goto done;
			ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
			xmlGenericError(xmlGenericErrorContext,
				"HPP: Parsing char data\n");
#endif
			htmlParseCharData(ctxt);
		    }
		}
		if (cons == ctxt->nbChars) {
		    if (ctxt->node != NULL) {
			htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			             "detected an error in element content\n",
				     NULL, NULL);
		    }
		    NEXT;
		    break;
		}

		break;
	    }
            case XML_PARSER_END_TAG:
		if (avail < 2)
		    goto done;
		if ((!terminate) &&
		    (htmlParseLookupSequence(ctxt, '>', 0, 0, 0, 1) < 0))
		    goto done;
		htmlParseEndTag(ctxt);
		if (ctxt->nameNr == 0) {
		    ctxt->instate = XML_PARSER_EPILOG;
		} else {
		    ctxt->instate = XML_PARSER_CONTENT;
		}
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
	        break;
            case XML_PARSER_CDATA_SECTION:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == CDATA\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
            case XML_PARSER_DTD:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == DTD\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
            case XML_PARSER_COMMENT:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == COMMENT\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
            case XML_PARSER_PI:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == PI\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
            case XML_PARSER_ENTITY_DECL:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == ENTITY_DECL\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
            case XML_PARSER_ENTITY_VALUE:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == ENTITY_VALUE\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering DTD\n");
#endif
		break;
            case XML_PARSER_ATTRIBUTE_VALUE:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == ATTRIBUTE_VALUE\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_START_TAG;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering START_TAG\n");
#endif
		break;
	    case XML_PARSER_SYSTEM_LITERAL:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		    "HPP: internal error, state == XML_PARSER_SYSTEM_LITERAL\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
	    case XML_PARSER_IGNORE:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == XML_PARSER_IGNORE\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;
	    case XML_PARSER_PUBLIC_LITERAL:
		htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
			"HPP: internal error, state == XML_PARSER_LITERAL\n",
			     NULL, NULL);
		ctxt->instate = XML_PARSER_CONTENT;
		ctxt->checkIndex = 0;
#ifdef DEBUG_PUSH
		xmlGenericError(xmlGenericErrorContext,
			"HPP: entering CONTENT\n");
#endif
		break;

	}
    }
done:
    if ((avail == 0) && (terminate)) {
	htmlAutoCloseOnEnd(ctxt);
	if ((ctxt->nameNr == 0) && (ctxt->instate != XML_PARSER_EOF)) {
	    ctxt->instate = XML_PARSER_EOF;
	    if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
		ctxt->sax->endDocument(ctxt->userData);
	}
    }
    if ((ctxt->myDoc != NULL) &&
	((terminate) || (ctxt->instate == XML_PARSER_EOF) ||
	 (ctxt->instate == XML_PARSER_EPILOG))) {
	xmlDtdPtr dtd;
	dtd = xmlGetIntSubset(ctxt->myDoc);
	if (dtd == NULL)
	    ctxt->myDoc->intSubset =
		xmlCreateIntSubset(ctxt->myDoc, BAD_CAST "html",
		    BAD_CAST "-//W3C//DTD HTML 4.0 Transitional//EN",
		    BAD_CAST "http://www.w3.org/TR/REC-html40/loose.dtd");
    }
#ifdef DEBUG_PUSH
    xmlGenericError(xmlGenericErrorContext, "HPP: done %d\n", ret);
#endif
    return(ret);
}

int
htmlParseChunk(htmlParserCtxtPtr ctxt, const char *chunk, int size,
              int terminate) {
    if ((ctxt == NULL) || (ctxt->input == NULL)) {
	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
		     "htmlParseChunk: context error\n", NULL, NULL);
	return(XML_ERR_INTERNAL_ERROR);
    }
    if ((size > 0) && (chunk != NULL) && (ctxt->input != NULL) &&
        (ctxt->input->buf != NULL) && (ctxt->instate != XML_PARSER_EOF))  {
	int base = ctxt->input->base - ctxt->input->buf->buffer->content;
	int cur = ctxt->input->cur - ctxt->input->base;
	int res;

	res = xmlParserInputBufferPush(ctxt->input->buf, size, chunk);
	if (res < 0) {
	    ctxt->errNo = XML_PARSER_EOF;
	    ctxt->disableSAX = 1;
	    return (XML_PARSER_EOF);
	}
	ctxt->input->base = ctxt->input->buf->buffer->content + base;
	ctxt->input->cur = ctxt->input->base + cur;
	ctxt->input->end =
	  &ctxt->input->buf->buffer->content[ctxt->input->buf->buffer->use];
#ifdef DEBUG_PUSH
	xmlGenericError(xmlGenericErrorContext, "HPP: pushed %d\n", size);
#endif

#if 0
	if ((terminate) || (ctxt->input->buf->buffer->use > 80))
	    htmlParseTryOrFinish(ctxt, terminate);
#endif
    } else if (ctxt->instate != XML_PARSER_EOF) {
	if ((ctxt->input != NULL) && ctxt->input->buf != NULL) {
	    xmlParserInputBufferPtr in = ctxt->input->buf;
	    if ((in->encoder != NULL) && (in->buffer != NULL) &&
		    (in->raw != NULL)) {
		int nbchars;

		nbchars = xmlCharEncInFunc(in->encoder, in->buffer, in->raw);
		if (nbchars < 0) {
		    htmlParseErr(ctxt, XML_ERR_INVALID_ENCODING,
			         "encoder error\n", NULL, NULL);
		    return(XML_ERR_INVALID_ENCODING);
		}
	    }
	}
    }
    htmlParseTryOrFinish(ctxt, terminate);
    if (terminate) {
	if ((ctxt->instate != XML_PARSER_EOF) &&
	    (ctxt->instate != XML_PARSER_EPILOG) &&
	    (ctxt->instate != XML_PARSER_MISC)) {
	    ctxt->errNo = XML_ERR_DOCUMENT_END;
	    ctxt->wellFormed = 0;
	}
	if (ctxt->instate != XML_PARSER_EOF) {
	    if ((ctxt->sax) && (ctxt->sax->endDocument != NULL))
		ctxt->sax->endDocument(ctxt->userData);
	}
	ctxt->instate = XML_PARSER_EOF;
    }
    return((xmlParserErrors) ctxt->errNo);
}


htmlParserCtxtPtr
htmlCreatePushParserCtxt(htmlSAXHandlerPtr sax, void *user_data,
                         const char *chunk, int size, const char *filename,
			 xmlCharEncoding enc) {
    htmlParserCtxtPtr ctxt;
    htmlParserInputPtr inputStream;
    xmlParserInputBufferPtr buf;

    xmlInitParser();

    buf = xmlAllocParserInputBuffer(enc);
    if (buf == NULL) return(NULL);

    ctxt = htmlNewParserCtxt();
    if (ctxt == NULL) {
	xmlFreeParserInputBuffer(buf);
	return(NULL);
    }
    if(enc==XML_CHAR_ENCODING_UTF8 || buf->encoder)
	ctxt->charset=XML_CHAR_ENCODING_UTF8;
    if (sax != NULL) {
	if (ctxt->sax != (xmlSAXHandlerPtr) &htmlDefaultSAXHandler)
	    xmlFree(ctxt->sax);
	ctxt->sax = (htmlSAXHandlerPtr) xmlMalloc(sizeof(htmlSAXHandler));
	if (ctxt->sax == NULL) {
	    xmlFree(buf);
	    xmlFree(ctxt);
	    return(NULL);
	}
	memcpy(ctxt->sax, sax, sizeof(htmlSAXHandler));
	if (user_data != NULL)
	    ctxt->userData = user_data;
    }
    if (filename == NULL) {
	ctxt->directory = NULL;
    } else {
        ctxt->directory = xmlParserGetDirectory(filename);
    }

    inputStream = htmlNewInputStream(ctxt);
    if (inputStream == NULL) {
	xmlFreeParserCtxt(ctxt);
	xmlFree(buf);
	return(NULL);
    }

    if (filename == NULL)
	inputStream->filename = NULL;
    else
	inputStream->filename = (char *)
	    xmlCanonicPath((const xmlChar *) filename);
    inputStream->buf = buf;
    inputStream->base = inputStream->buf->buffer->content;
    inputStream->cur = inputStream->buf->buffer->content;
    inputStream->end =
	&inputStream->buf->buffer->content[inputStream->buf->buffer->use];

    inputPush(ctxt, inputStream);

    if ((size > 0) && (chunk != NULL) && (ctxt->input != NULL) &&
        (ctxt->input->buf != NULL))  {
	int base = ctxt->input->base - ctxt->input->buf->buffer->content;
	int cur = ctxt->input->cur - ctxt->input->base;

	xmlParserInputBufferPush(ctxt->input->buf, size, chunk);

	ctxt->input->base = ctxt->input->buf->buffer->content + base;
	ctxt->input->cur = ctxt->input->base + cur;
	ctxt->input->end =
	    &ctxt->input->buf->buffer->content[ctxt->input->buf->buffer->use];
#ifdef DEBUG_PUSH
	xmlGenericError(xmlGenericErrorContext, "HPP: pushed %d\n", size);
#endif
    }
    ctxt->progressive = 1;

    return(ctxt);
}
#endif 


htmlDocPtr
htmlSAXParseDoc(xmlChar *cur, const char *encoding, htmlSAXHandlerPtr sax, void *userData) {
    htmlDocPtr ret;
    htmlParserCtxtPtr ctxt;

    xmlInitParser();

    if (cur == NULL) return(NULL);


    ctxt = htmlCreateDocParserCtxt(cur, encoding);
    if (ctxt == NULL) return(NULL);
    if (sax != NULL) {
        if (ctxt->sax != NULL) xmlFree (ctxt->sax);
        ctxt->sax = sax;
        ctxt->userData = userData;
    }

    htmlParseDocument(ctxt);
    ret = ctxt->myDoc;
    if (sax != NULL) {
	ctxt->sax = NULL;
	ctxt->userData = NULL;
    }
    htmlFreeParserCtxt(ctxt);

    return(ret);
}


htmlDocPtr
htmlParseDoc(xmlChar *cur, const char *encoding) {
    return(htmlSAXParseDoc(cur, encoding, NULL, NULL));
}


htmlParserCtxtPtr
htmlCreateFileParserCtxt(const char *filename, const char *encoding)
{
    htmlParserCtxtPtr ctxt;
    htmlParserInputPtr inputStream;
    char *canonicFilename;
    
    xmlChar *content, *content_line = (xmlChar *) "charset=";

    if (filename == NULL)
        return(NULL);

    ctxt = htmlNewParserCtxt();
    if (ctxt == NULL) {
	return(NULL);
    }
    canonicFilename = (char *) xmlCanonicPath((const xmlChar *) filename);
    if (canonicFilename == NULL) {
#ifdef LIBXML_SAX1_ENABLED
	if (xmlDefaultSAXHandler.error != NULL) {
	    xmlDefaultSAXHandler.error(NULL, "out of memory\n");
	}
#endif
	xmlFreeParserCtxt(ctxt);
	return(NULL);
    }

    inputStream = xmlLoadExternalEntity(canonicFilename, NULL, ctxt);
    xmlFree(canonicFilename);
    if (inputStream == NULL) {
	xmlFreeParserCtxt(ctxt);
	return(NULL);
    }

    inputPush(ctxt, inputStream);

    
    if (encoding) {
        content = xmlMallocAtomic (xmlStrlen(content_line) + strlen(encoding) + 1);
	if (content) {
	    strcpy ((char *)content, (char *)content_line);
            strcat ((char *)content, (char *)encoding);
            htmlCheckEncoding (ctxt, content);
	    xmlFree (content);
	}
    }

    return(ctxt);
}


htmlDocPtr
htmlSAXParseFile(const char *filename, const char *encoding, htmlSAXHandlerPtr sax,
                 void *userData) {
    htmlDocPtr ret;
    htmlParserCtxtPtr ctxt;
    htmlSAXHandlerPtr oldsax = NULL;

    xmlInitParser();

    ctxt = htmlCreateFileParserCtxt(filename, encoding);
    if (ctxt == NULL) return(NULL);
    if (sax != NULL) {
	oldsax = ctxt->sax;
        ctxt->sax = sax;
        ctxt->userData = userData;
    }

    htmlParseDocument(ctxt);

    ret = ctxt->myDoc;
    if (sax != NULL) {
        ctxt->sax = oldsax;
        ctxt->userData = NULL;
    }
    htmlFreeParserCtxt(ctxt);

    return(ret);
}


htmlDocPtr
htmlParseFile(const char *filename, const char *encoding) {
    return(htmlSAXParseFile(filename, encoding, NULL, NULL));
}


int
htmlHandleOmittedElem(int val) {
    int old = htmlOmittedDefaultValue;

    htmlOmittedDefaultValue = val;
    return(old);
}

int
htmlElementAllowedHere(const htmlElemDesc* parent, const xmlChar* elt) {
  const char** p ;

  if ( ! elt || ! parent || ! parent->subelts )
	return 0 ;

  for ( p = parent->subelts; *p; ++p )
    if ( !xmlStrcmp((const xmlChar *)*p, elt) )
      return 1 ;

  return 0 ;
}
htmlStatus
htmlElementStatusHere(const htmlElemDesc* parent, const htmlElemDesc* elt) {
  if ( ! parent || ! elt )
    return HTML_INVALID ;
  if ( ! htmlElementAllowedHere(parent, (const xmlChar*) elt->name ) )
    return HTML_INVALID ;

  return ( elt->dtd == 0 ) ? HTML_VALID : HTML_DEPRECATED ;
}
htmlStatus
htmlAttrAllowed(const htmlElemDesc* elt, const xmlChar* attr, int legacy) {
  const char** p ;

  if ( !elt || ! attr )
	return HTML_INVALID ;

  if ( elt->attrs_req )
    for ( p = elt->attrs_req; *p; ++p)
      if ( !xmlStrcmp((const xmlChar*)*p, attr) )
        return HTML_REQUIRED ;

  if ( elt->attrs_opt )
    for ( p = elt->attrs_opt; *p; ++p)
      if ( !xmlStrcmp((const xmlChar*)*p, attr) )
        return HTML_VALID ;

  if ( legacy && elt->attrs_depr )
    for ( p = elt->attrs_depr; *p; ++p)
      if ( !xmlStrcmp((const xmlChar*)*p, attr) )
        return HTML_DEPRECATED ;

  return HTML_INVALID ;
}
htmlStatus
htmlNodeStatus(const htmlNodePtr node, int legacy) {
  if ( ! node )
    return HTML_INVALID ;

  switch ( node->type ) {
    case XML_ELEMENT_NODE:
      return legacy
	? ( htmlElementAllowedHere (
		htmlTagLookup(node->parent->name) , node->name
		) ? HTML_VALID : HTML_INVALID )
	: htmlElementStatusHere(
		htmlTagLookup(node->parent->name) ,
		htmlTagLookup(node->name) )
	;
    case XML_ATTRIBUTE_NODE:
      return htmlAttrAllowed(
	htmlTagLookup(node->parent->name) , node->name, legacy) ;
    default: return HTML_NA ;
  }
}
#define DICT_FREE(str)						\
	if ((str) && ((!dict) ||				\
	    (xmlDictOwns(dict, (const xmlChar *)(str)) == 0)))	\
	    xmlFree((char *)(str));

void
htmlCtxtReset(htmlParserCtxtPtr ctxt)
{
    xmlParserInputPtr input;
    xmlDictPtr dict;

    if (ctxt == NULL)
        return;

    xmlInitParser();
    dict = ctxt->dict;

    while ((input = inputPop(ctxt)) != NULL) { 
        xmlFreeInputStream(input);
    }
    ctxt->inputNr = 0;
    ctxt->input = NULL;

    ctxt->spaceNr = 0;
    if (ctxt->spaceTab != NULL) {
	ctxt->spaceTab[0] = -1;
	ctxt->space = &ctxt->spaceTab[0];
    } else {
	ctxt->space = NULL;
    }


    ctxt->nodeNr = 0;
    ctxt->node = NULL;

    ctxt->nameNr = 0;
    ctxt->name = NULL;

    DICT_FREE(ctxt->version);
    ctxt->version = NULL;
    DICT_FREE(ctxt->encoding);
    ctxt->encoding = NULL;
    DICT_FREE(ctxt->directory);
    ctxt->directory = NULL;
    DICT_FREE(ctxt->extSubURI);
    ctxt->extSubURI = NULL;
    DICT_FREE(ctxt->extSubSystem);
    ctxt->extSubSystem = NULL;
    if (ctxt->myDoc != NULL)
        xmlFreeDoc(ctxt->myDoc);
    ctxt->myDoc = NULL;

    ctxt->standalone = -1;
    ctxt->hasExternalSubset = 0;
    ctxt->hasPErefs = 0;
    ctxt->html = 1;
    ctxt->external = 0;
    ctxt->instate = XML_PARSER_START;
    ctxt->token = 0;

    ctxt->wellFormed = 1;
    ctxt->nsWellFormed = 1;
    ctxt->valid = 1;
    ctxt->vctxt.userData = ctxt;
    ctxt->vctxt.error = xmlParserValidityError;
    ctxt->vctxt.warning = xmlParserValidityWarning;
    ctxt->record_info = 0;
    ctxt->nbChars = 0;
    ctxt->checkIndex = 0;
    ctxt->inSubset = 0;
    ctxt->errNo = XML_ERR_OK;
    ctxt->depth = 0;
    ctxt->charset = XML_CHAR_ENCODING_NONE;
    ctxt->catalogs = NULL;
    xmlInitNodeInfoSeq(&ctxt->node_seq);

    if (ctxt->attsDefault != NULL) {
        xmlHashFree(ctxt->attsDefault, (xmlHashDeallocator) xmlFree);
        ctxt->attsDefault = NULL;
    }
    if (ctxt->attsSpecial != NULL) {
        xmlHashFree(ctxt->attsSpecial, NULL);
        ctxt->attsSpecial = NULL;
    }
}

int
htmlCtxtUseOptions(htmlParserCtxtPtr ctxt, int options)
{
    if (ctxt == NULL)
        return(-1);

    if (options & HTML_PARSE_NOWARNING) {
        ctxt->sax->warning = NULL;
        ctxt->vctxt.warning = NULL;
        options -= XML_PARSE_NOWARNING;
	ctxt->options |= XML_PARSE_NOWARNING;
    }
    if (options & HTML_PARSE_NOERROR) {
        ctxt->sax->error = NULL;
        ctxt->vctxt.error = NULL;
        ctxt->sax->fatalError = NULL;
        options -= XML_PARSE_NOERROR;
	ctxt->options |= XML_PARSE_NOERROR;
    }
    if (options & HTML_PARSE_PEDANTIC) {
        ctxt->pedantic = 1;
        options -= XML_PARSE_PEDANTIC;
	ctxt->options |= XML_PARSE_PEDANTIC;
    } else
        ctxt->pedantic = 0;
    if (options & XML_PARSE_NOBLANKS) {
        ctxt->keepBlanks = 0;
        ctxt->sax->ignorableWhitespace = xmlSAX2IgnorableWhitespace;
        options -= XML_PARSE_NOBLANKS;
	ctxt->options |= XML_PARSE_NOBLANKS;
    } else
        ctxt->keepBlanks = 1;
    if (options & HTML_PARSE_RECOVER) {
        ctxt->recovery = 1;
	options -= HTML_PARSE_RECOVER;
    } else
        ctxt->recovery = 0;
    if (options & HTML_PARSE_COMPACT) {
	ctxt->options |= HTML_PARSE_COMPACT;
        options -= HTML_PARSE_COMPACT;
    }
    if (options & XML_PARSE_HUGE) {
	ctxt->options |= XML_PARSE_HUGE;
        options -= XML_PARSE_HUGE;
    }
    ctxt->dictNames = 0;
    return (options);
}

static htmlDocPtr
htmlDoRead(htmlParserCtxtPtr ctxt, const char *URL, const char *encoding,
          int options, int reuse)
{
    htmlDocPtr ret;

    htmlCtxtUseOptions(ctxt, options);
    ctxt->html = 1;
    if (encoding != NULL) {
        xmlCharEncodingHandlerPtr hdlr;

	hdlr = xmlFindCharEncodingHandler(encoding);
	if (hdlr != NULL) {
	    xmlSwitchToEncoding(ctxt, hdlr);
	    if (ctxt->input->encoding != NULL)
	      xmlFree((xmlChar *) ctxt->input->encoding);
            ctxt->input->encoding = xmlStrdup((xmlChar *)encoding);
        }
    }
    if ((URL != NULL) && (ctxt->input != NULL) &&
        (ctxt->input->filename == NULL))
        ctxt->input->filename = (char *) xmlStrdup((const xmlChar *) URL);
    htmlParseDocument(ctxt);
    ret = ctxt->myDoc;
    ctxt->myDoc = NULL;
    if (!reuse) {
        if ((ctxt->dictNames) &&
	    (ret != NULL) &&
	    (ret->dict == ctxt->dict))
	    ctxt->dict = NULL;
	xmlFreeParserCtxt(ctxt);
    }
    return (ret);
}

htmlDocPtr
htmlReadDoc(const xmlChar * cur, const char *URL, const char *encoding, int options)
{
    htmlParserCtxtPtr ctxt;

    if (cur == NULL)
        return (NULL);

    xmlInitParser();
    ctxt = htmlCreateDocParserCtxt(cur, NULL);
    if (ctxt == NULL)
        return (NULL);
    return (htmlDoRead(ctxt, URL, encoding, options, 0));
}

htmlDocPtr
htmlReadFile(const char *filename, const char *encoding, int options)
{
    htmlParserCtxtPtr ctxt;

    xmlInitParser();
    ctxt = htmlCreateFileParserCtxt(filename, encoding);
    if (ctxt == NULL)
        return (NULL);
    return (htmlDoRead(ctxt, NULL, NULL, options, 0));
}

htmlDocPtr
htmlReadMemory(const char *buffer, int size, const char *URL, const char *encoding, int options)
{
    htmlParserCtxtPtr ctxt;

    xmlInitParser();
    ctxt = xmlCreateMemoryParserCtxt(buffer, size);
    if (ctxt == NULL)
        return (NULL);
    htmlDefaultSAXHandlerInit();
    if (ctxt->sax != NULL)
        memcpy(ctxt->sax, &htmlDefaultSAXHandler, sizeof(xmlSAXHandlerV1));
    return (htmlDoRead(ctxt, URL, encoding, options, 0));
}

htmlDocPtr
htmlReadFd(int fd, const char *URL, const char *encoding, int options)
{
    htmlParserCtxtPtr ctxt;
    xmlParserInputBufferPtr input;
    xmlParserInputPtr stream;

    if (fd < 0)
        return (NULL);

    xmlInitParser();
    input = xmlParserInputBufferCreateFd(fd, XML_CHAR_ENCODING_NONE);
    if (input == NULL)
        return (NULL);
    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL) {
        xmlFreeParserInputBuffer(input);
        return (NULL);
    }
    stream = xmlNewIOInputStream(ctxt, input, XML_CHAR_ENCODING_NONE);
    if (stream == NULL) {
        xmlFreeParserInputBuffer(input);
	xmlFreeParserCtxt(ctxt);
        return (NULL);
    }
    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, URL, encoding, options, 0));
}

htmlDocPtr
htmlReadIO(xmlInputReadCallback ioread, xmlInputCloseCallback ioclose,
          void *ioctx, const char *URL, const char *encoding, int options)
{
    htmlParserCtxtPtr ctxt;
    xmlParserInputBufferPtr input;
    xmlParserInputPtr stream;

    if (ioread == NULL)
        return (NULL);
    xmlInitParser();

    input = xmlParserInputBufferCreateIO(ioread, ioclose, ioctx,
                                         XML_CHAR_ENCODING_NONE);
    if (input == NULL)
        return (NULL);
    ctxt = htmlNewParserCtxt();
    if (ctxt == NULL) {
        xmlFreeParserInputBuffer(input);
        return (NULL);
    }
    stream = xmlNewIOInputStream(ctxt, input, XML_CHAR_ENCODING_NONE);
    if (stream == NULL) {
        xmlFreeParserInputBuffer(input);
	xmlFreeParserCtxt(ctxt);
        return (NULL);
    }
    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, URL, encoding, options, 0));
}

htmlDocPtr
htmlCtxtReadDoc(htmlParserCtxtPtr ctxt, const xmlChar * cur,
               const char *URL, const char *encoding, int options)
{
    xmlParserInputPtr stream;

    if (cur == NULL)
        return (NULL);
    if (ctxt == NULL)
        return (NULL);

    htmlCtxtReset(ctxt);

    stream = xmlNewStringInputStream(ctxt, cur);
    if (stream == NULL) {
        return (NULL);
    }
    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, URL, encoding, options, 1));
}

htmlDocPtr
htmlCtxtReadFile(htmlParserCtxtPtr ctxt, const char *filename,
                const char *encoding, int options)
{
    xmlParserInputPtr stream;

    if (filename == NULL)
        return (NULL);
    if (ctxt == NULL)
        return (NULL);

    htmlCtxtReset(ctxt);

    stream = xmlLoadExternalEntity(filename, NULL, ctxt);
    if (stream == NULL) {
        return (NULL);
    }
    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, NULL, encoding, options, 1));
}

htmlDocPtr
htmlCtxtReadMemory(htmlParserCtxtPtr ctxt, const char *buffer, int size,
                  const char *URL, const char *encoding, int options)
{
    xmlParserInputBufferPtr input;
    xmlParserInputPtr stream;

    if (ctxt == NULL)
        return (NULL);
    if (buffer == NULL)
        return (NULL);

    htmlCtxtReset(ctxt);

    input = xmlParserInputBufferCreateMem(buffer, size, XML_CHAR_ENCODING_NONE);
    if (input == NULL) {
	return(NULL);
    }

    stream = xmlNewIOInputStream(ctxt, input, XML_CHAR_ENCODING_NONE);
    if (stream == NULL) {
	xmlFreeParserInputBuffer(input);
	return(NULL);
    }

    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, URL, encoding, options, 1));
}

htmlDocPtr
htmlCtxtReadFd(htmlParserCtxtPtr ctxt, int fd,
              const char *URL, const char *encoding, int options)
{
    xmlParserInputBufferPtr input;
    xmlParserInputPtr stream;

    if (fd < 0)
        return (NULL);
    if (ctxt == NULL)
        return (NULL);

    htmlCtxtReset(ctxt);


    input = xmlParserInputBufferCreateFd(fd, XML_CHAR_ENCODING_NONE);
    if (input == NULL)
        return (NULL);
    stream = xmlNewIOInputStream(ctxt, input, XML_CHAR_ENCODING_NONE);
    if (stream == NULL) {
        xmlFreeParserInputBuffer(input);
        return (NULL);
    }
    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, URL, encoding, options, 1));
}

htmlDocPtr
htmlCtxtReadIO(htmlParserCtxtPtr ctxt, xmlInputReadCallback ioread,
              xmlInputCloseCallback ioclose, void *ioctx,
	      const char *URL,
              const char *encoding, int options)
{
    xmlParserInputBufferPtr input;
    xmlParserInputPtr stream;

    if (ioread == NULL)
        return (NULL);
    if (ctxt == NULL)
        return (NULL);

    htmlCtxtReset(ctxt);

    input = xmlParserInputBufferCreateIO(ioread, ioclose, ioctx,
                                         XML_CHAR_ENCODING_NONE);
    if (input == NULL)
        return (NULL);
    stream = xmlNewIOInputStream(ctxt, input, XML_CHAR_ENCODING_NONE);
    if (stream == NULL) {
        xmlFreeParserInputBuffer(input);
        return (NULL);
    }
    inputPush(ctxt, stream);
    return (htmlDoRead(ctxt, URL, encoding, options, 1));
}

#define bottom_HTMLparser
#include "elfgcchack.h"
#endif 
