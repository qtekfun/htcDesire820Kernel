/*
 * extensions.c: Implemetation of the extensions support
 *
 * Reference:
 *   http://www.w3.org/TR/1999/REC-xslt-19991116
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXSLT
#include "libxslt.h"

#include <string.h>
#include <limits.h>

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/hash.h>
#include <libxml/xmlerror.h>
#include <libxml/parserInternals.h>
#include <libxml/xpathInternals.h>
#ifdef WITH_MODULES
#include <libxml/xmlmodule.h>
#endif
#include <libxml/list.h>
#include <libxml/xmlIO.h>
#include "xslt.h"
#include "xsltInternals.h"
#include "xsltutils.h"
#include "imports.h"
#include "extensions.h"

#ifdef _WIN32
#include <stdlib.h>             
#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif
#endif

#ifdef WITH_XSLT_DEBUG
#define WITH_XSLT_DEBUG_EXTENSIONS
#endif


typedef struct _xsltExtDef xsltExtDef;
typedef xsltExtDef *xsltExtDefPtr;
struct _xsltExtDef {
    struct _xsltExtDef *next;
    xmlChar *prefix;
    xmlChar *URI;
    void *data;
};

typedef struct _xsltExtModule xsltExtModule;
typedef xsltExtModule *xsltExtModulePtr;
struct _xsltExtModule {
    xsltExtInitFunction initFunc;
    xsltExtShutdownFunction shutdownFunc;
    xsltStyleExtInitFunction styleInitFunc;
    xsltStyleExtShutdownFunction styleShutdownFunc;
};

typedef struct _xsltExtData xsltExtData;
typedef xsltExtData *xsltExtDataPtr;
struct _xsltExtData {
    xsltExtModulePtr extModule;
    void *extData;
};

typedef struct _xsltExtElement xsltExtElement;
typedef xsltExtElement *xsltExtElementPtr;
struct _xsltExtElement {
    xsltPreComputeFunction precomp;
    xsltTransformFunction transform;
};

static xmlHashTablePtr xsltExtensionsHash = NULL;
static xmlHashTablePtr xsltFunctionsHash = NULL;
static xmlHashTablePtr xsltElementsHash = NULL;
static xmlHashTablePtr xsltTopLevelsHash = NULL;
static xmlHashTablePtr xsltModuleHash = NULL;
static xmlMutexPtr xsltExtMutex = NULL;


static xsltExtDefPtr
xsltNewExtDef(const xmlChar * prefix, const xmlChar * URI)
{
    xsltExtDefPtr cur;

    cur = (xsltExtDefPtr) xmlMalloc(sizeof(xsltExtDef));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtDef : malloc failed\n");
        return (NULL);
    }
    memset(cur, 0, sizeof(xsltExtDef));
    if (prefix != NULL)
        cur->prefix = xmlStrdup(prefix);
    if (URI != NULL)
        cur->URI = xmlStrdup(URI);
    return (cur);
}

static void
xsltFreeExtDef(xsltExtDefPtr extensiond)
{
    if (extensiond == NULL)
        return;
    if (extensiond->prefix != NULL)
        xmlFree(extensiond->prefix);
    if (extensiond->URI != NULL)
        xmlFree(extensiond->URI);
    xmlFree(extensiond);
}

static void
xsltFreeExtDefList(xsltExtDefPtr extensiond)
{
    xsltExtDefPtr cur;

    while (extensiond != NULL) {
        cur = extensiond;
        extensiond = extensiond->next;
        xsltFreeExtDef(cur);
    }
}

static xsltExtModulePtr
xsltNewExtModule(xsltExtInitFunction initFunc,
                 xsltExtShutdownFunction shutdownFunc,
                 xsltStyleExtInitFunction styleInitFunc,
                 xsltStyleExtShutdownFunction styleShutdownFunc)
{
    xsltExtModulePtr cur;

    cur = (xsltExtModulePtr) xmlMalloc(sizeof(xsltExtModule));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtModule : malloc failed\n");
        return (NULL);
    }
    cur->initFunc = initFunc;
    cur->shutdownFunc = shutdownFunc;
    cur->styleInitFunc = styleInitFunc;
    cur->styleShutdownFunc = styleShutdownFunc;
    return (cur);
}

static void
xsltFreeExtModule(xsltExtModulePtr ext)
{
    if (ext == NULL)
        return;
    xmlFree(ext);
}

static xsltExtDataPtr
xsltNewExtData(xsltExtModulePtr extModule, void *extData)
{
    xsltExtDataPtr cur;

    if (extModule == NULL)
        return (NULL);
    cur = (xsltExtDataPtr) xmlMalloc(sizeof(xsltExtData));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtData : malloc failed\n");
        return (NULL);
    }
    cur->extModule = extModule;
    cur->extData = extData;
    return (cur);
}

static void
xsltFreeExtData(xsltExtDataPtr ext)
{
    if (ext == NULL)
        return;
    xmlFree(ext);
}

static xsltExtElementPtr
xsltNewExtElement(xsltPreComputeFunction precomp,
                  xsltTransformFunction transform)
{
    xsltExtElementPtr cur;

    if (transform == NULL)
        return (NULL);

    cur = (xsltExtElementPtr) xmlMalloc(sizeof(xsltExtElement));
    if (cur == NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltNewExtElement : malloc failed\n");
        return (NULL);
    }
    cur->precomp = precomp;
    cur->transform = transform;
    return (cur);
}

static void
xsltFreeExtElement(xsltExtElementPtr ext)
{
    if (ext == NULL)
        return;
    xmlFree(ext);
}


#ifdef WITH_MODULES
typedef void (*exsltRegisterFunction) (void);

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


static int
xsltExtModuleRegisterDynamic(const xmlChar * URI)
{

    xmlModulePtr m;
    exsltRegisterFunction regfunc;
    xmlChar *ext_name;
    char module_filename[PATH_MAX];
    const xmlChar *ext_directory = NULL;
    const xmlChar *protocol = NULL;
    xmlChar *i, *regfunc_name;
    void *vregfunc;
    int rc;

    
    if (URI == NULL)
        return (-1);

    if (NULL == xsltModuleHash) {
        xsltModuleHash = xmlHashCreate(5);
        if (xsltModuleHash == NULL)
            return (-1);
    }

    xmlMutexLock(xsltExtMutex);

    
    if (xmlHashLookup(xsltModuleHash, URI) != NULL) {
        xmlMutexUnlock(xsltExtMutex);
        return (-1);
    }
    xmlMutexUnlock(xsltExtMutex);

    
    protocol = xmlStrstr(URI, BAD_CAST "://");
    if (protocol == NULL) {
        ext_name = xmlStrdup(URI);
    } else {
        ext_name = xmlStrdup(protocol + 3);
    }
    if (ext_name == NULL) {
        return (-1);
    }

    i = ext_name;
    while ('\0' != *i) {
        if (('/' == *i) || ('\\' == *i) || ('.' == *i) || ('-' == *i))
            *i = '_';
        i++;
    }

    if (*(i - 1) == '_')
        *i = '\0';

    
    ext_directory = (xmlChar *) getenv("LIBXSLT_PLUGINS_PATH");

    if (NULL == ext_directory) {
        ext_directory = BAD_CAST LIBXSLT_DEFAULT_PLUGINS_PATH();
	if (NULL == ext_directory)
	  return (-1);
    }
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    else
      xsltGenericDebug(xsltGenericDebugContext,
		       "LIBXSLT_PLUGINS_PATH is %s\n", ext_directory);
#endif

    
    xmlStrPrintf((xmlChar *) module_filename, sizeof(module_filename),
                 BAD_CAST "%s/%s%s",
                 ext_directory, ext_name, LIBXML_MODULE_EXTENSION);

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
                     "Attempting to load plugin: %s for URI: %s\n", 
                     module_filename, URI);
#endif

    if (1 != xmlCheckFilename(module_filename)) {

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
                     "xmlCheckFilename failed for plugin: %s\n", module_filename);
#endif

        xmlFree(ext_name);
        return (-1);
    }

    
    m = xmlModuleOpen(module_filename, 0);
    if (NULL == m) {

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
                     "xmlModuleOpen failed for plugin: %s\n", module_filename);
#endif

        xmlFree(ext_name);
        return (-1);
    }

    
    regfunc_name = xmlStrdup(ext_name);
    regfunc_name = xmlStrcat(regfunc_name, BAD_CAST "_init");

    vregfunc = NULL;
    rc = xmlModuleSymbol(m, (const char *) regfunc_name, &vregfunc);
    regfunc = vregfunc;
    if (0 == rc) {
        (*regfunc) ();

        
        xmlMutexLock(xsltExtMutex);
        xmlHashAddEntry(xsltModuleHash, URI, (void *) m);
        xmlMutexUnlock(xsltExtMutex);
    } else {

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
                     "xmlModuleSymbol failed for plugin: %s, regfunc: %s\n", 
                     module_filename, regfunc_name);
#endif

        
        xmlModuleClose(m);
    }

    xmlFree(ext_name);
    xmlFree(regfunc_name);
    return (NULL == regfunc) ? -1 : 0;
}
#else
static int
xsltExtModuleRegisterDynamic(const xmlChar * URI ATTRIBUTE_UNUSED)
{
  return -1;
}
#endif



void
xsltFreeExts(xsltStylesheetPtr style)
{
    if (style->nsDefs != NULL)
        xsltFreeExtDefList((xsltExtDefPtr) style->nsDefs);
}

int
xsltRegisterExtPrefix(xsltStylesheetPtr style,
                      const xmlChar * prefix, const xmlChar * URI)
{
    xsltExtDefPtr def, ret;

    if ((style == NULL) || (URI == NULL))
        return (-1);

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
	"Registering extension namespace '%s'.\n", URI);
#endif
    def = (xsltExtDefPtr) style->nsDefs;
#ifdef XSLT_REFACTORED
    while (def != NULL) {
        if (xmlStrEqual(URI, def->URI))
            return (1);
        def = def->next;
    }
#else
    while (def != NULL) {
        if (xmlStrEqual(prefix, def->prefix))
            return (-1);
        def = def->next;
    }
#endif
    ret = xsltNewExtDef(prefix, URI);
    if (ret == NULL)
        return (-1);
    ret->next = (xsltExtDefPtr) style->nsDefs;
    style->nsDefs = ret;

#ifdef XSLT_REFACTORED
#else
    if (xsltExtensionsHash != NULL) {
        xsltExtModulePtr module;

        xmlMutexLock(xsltExtMutex);
        module = xmlHashLookup(xsltExtensionsHash, URI);
        xmlMutexUnlock(xsltExtMutex);
        if (NULL == module) {
            if (!xsltExtModuleRegisterDynamic(URI)) {
                xmlMutexLock(xsltExtMutex);
                module = xmlHashLookup(xsltExtensionsHash, URI);
                xmlMutexUnlock(xsltExtMutex);
            }
        }
        if (module != NULL) {
            xsltStyleGetExtData(style, URI);
        }
    }
#endif
    return (0);
}


int
xsltRegisterExtFunction(xsltTransformContextPtr ctxt, const xmlChar * name,
                        const xmlChar * URI, xmlXPathFunction function)
{
    int ret;

    if ((ctxt == NULL) || (name == NULL) ||
        (URI == NULL) || (function == NULL))
        return (-1);
    if (ctxt->xpathCtxt != NULL) {
        xmlXPathRegisterFuncNS(ctxt->xpathCtxt, name, URI, function);
    }
    if (ctxt->extFunctions == NULL)
        ctxt->extFunctions = xmlHashCreate(10);
    if (ctxt->extFunctions == NULL)
        return (-1);

    ret = xmlHashAddEntry2(ctxt->extFunctions, name, URI,
                           XML_CAST_FPTR(function));

    return(ret);
}

int
xsltRegisterExtElement(xsltTransformContextPtr ctxt, const xmlChar * name,
                       const xmlChar * URI, xsltTransformFunction function)
{
    if ((ctxt == NULL) || (name == NULL) ||
        (URI == NULL) || (function == NULL))
        return (-1);
    if (ctxt->extElements == NULL)
        ctxt->extElements = xmlHashCreate(10);
    if (ctxt->extElements == NULL)
        return (-1);
    return (xmlHashAddEntry2
            (ctxt->extElements, name, URI, XML_CAST_FPTR(function)));
}

void
xsltFreeCtxtExts(xsltTransformContextPtr ctxt)
{
    if (ctxt->extElements != NULL)
        xmlHashFree(ctxt->extElements, NULL);
    if (ctxt->extFunctions != NULL)
        xmlHashFree(ctxt->extFunctions, NULL);
}

static xsltExtDataPtr
xsltStyleInitializeStylesheetModule(xsltStylesheetPtr style,
				     const xmlChar * URI)
{
    xsltExtDataPtr dataContainer;
    void *userData = NULL;
    xsltExtModulePtr module;
    
    if ((style == NULL) || (URI == NULL))	
	return(NULL);

    if (xsltExtensionsHash == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Not registered extension module: %s\n", URI);
#endif
	return(NULL);
    }

    xmlMutexLock(xsltExtMutex);

    module = xmlHashLookup(xsltExtensionsHash, URI);

    xmlMutexUnlock(xsltExtMutex);

    if (module == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Not registered extension module: %s\n", URI);
#endif
	return (NULL);
    }
    if (style->extInfos == NULL) {
	style->extInfos = xmlHashCreate(10);
	if (style->extInfos == NULL)
	    return (NULL);
    }
    if (module->styleInitFunc == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Initializing module with *no* callback: %s\n", URI);
#endif
    } else {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
	xsltGenericDebug(xsltGenericDebugContext,
	    "Initializing module with callback: %s\n", URI);
#endif
	userData = module->styleInitFunc(style, URI);
    }    
    dataContainer = xsltNewExtData(module, userData);
    if (dataContainer == NULL)
	return (NULL);

    if (xmlHashAddEntry(style->extInfos, URI,
	(void *) dataContainer) < 0)
    {
	xsltTransformError(NULL, style, NULL,	    
	    "Failed to register module '%s'.\n", URI);
	style->errors++;
	if (module->styleShutdownFunc)
	    module->styleShutdownFunc(style, URI, userData);
	xsltFreeExtData(dataContainer);
	return (NULL);
    }

    return(dataContainer);
}

void *
xsltStyleGetExtData(xsltStylesheetPtr style, const xmlChar * URI)
{
    xsltExtDataPtr dataContainer = NULL;
    xsltStylesheetPtr tmpStyle;

    if ((style == NULL) || (URI == NULL) ||
	(xsltExtensionsHash == NULL))
	return (NULL);

    
#ifdef XSLT_REFACTORED
    tmpStyle = style;
    while (tmpStyle->parent != NULL)
	tmpStyle = tmpStyle->parent;
    if (tmpStyle->extInfos != NULL) {
	dataContainer =
	    (xsltExtDataPtr) xmlHashLookup(tmpStyle->extInfos, URI);
	if (dataContainer != NULL) {
	    return(dataContainer->extData);
	}
    }
#else
    tmpStyle = style;
    while (tmpStyle != NULL) {
	if (tmpStyle->extInfos != NULL) {
	    dataContainer =
		(xsltExtDataPtr) xmlHashLookup(tmpStyle->extInfos, URI);
	    if (dataContainer != NULL) {
		return(dataContainer->extData);
	    }
	}
	tmpStyle = xsltNextImport(tmpStyle);
    }
    tmpStyle = style;
#endif

    dataContainer =
        xsltStyleInitializeStylesheetModule(tmpStyle, URI);
    if (dataContainer != NULL)
	return (dataContainer->extData);
    return(NULL);
}

#ifdef XSLT_REFACTORED
void *
xsltStyleStylesheetLevelGetExtData(xsltStylesheetPtr style,
				   const xmlChar * URI)
{
    xsltExtDataPtr dataContainer = NULL;

    if ((style == NULL) || (URI == NULL) ||
	(xsltExtensionsHash == NULL))
	return (NULL);

    if (style->extInfos != NULL) {
	dataContainer = (xsltExtDataPtr) xmlHashLookup(style->extInfos, URI);
	if (dataContainer)
	    return(dataContainer->extData);
    }  

    dataContainer =
        xsltStyleInitializeStylesheetModule(style, URI);
    if (dataContainer != NULL)
	return (dataContainer->extData);
    return(NULL);
}
#endif

void *
xsltGetExtData(xsltTransformContextPtr ctxt, const xmlChar * URI)
{
    xsltExtDataPtr data;

    if ((ctxt == NULL) || (URI == NULL))
        return (NULL);
    if (ctxt->extInfos == NULL) {
        ctxt->extInfos = xmlHashCreate(10);
        if (ctxt->extInfos == NULL)
            return (NULL);
        data = NULL;
    } else {
        data = (xsltExtDataPtr) xmlHashLookup(ctxt->extInfos, URI);
    }
    if (data == NULL) {
        void *extData;
        xsltExtModulePtr module;

        xmlMutexLock(xsltExtMutex);

        module = xmlHashLookup(xsltExtensionsHash, URI);

        xmlMutexUnlock(xsltExtMutex);

        if (module == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
            xsltGenericDebug(xsltGenericDebugContext,
                             "Not registered extension module: %s\n", URI);
#endif
            return (NULL);
        } else {
            if (module->initFunc == NULL)
                return (NULL);

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
            xsltGenericDebug(xsltGenericDebugContext,
                             "Initializing module: %s\n", URI);
#endif

            extData = module->initFunc(ctxt, URI);
            if (extData == NULL)
                return (NULL);

            data = xsltNewExtData(module, extData);
            if (data == NULL)
                return (NULL);
            if (xmlHashAddEntry(ctxt->extInfos, URI, (void *) data) < 0) {
                xsltTransformError(ctxt, NULL, NULL,
                                   "Failed to register module data: %s\n",
                                   URI);
                if (module->shutdownFunc)
                    module->shutdownFunc(ctxt, URI, extData);
                xsltFreeExtData(data);
                return (NULL);
            }
        }
    }
    return (data->extData);
}

typedef struct _xsltInitExtCtxt xsltInitExtCtxt;
struct _xsltInitExtCtxt {
    xsltTransformContextPtr ctxt;
    int ret;
};

static void
xsltInitCtxtExt(xsltExtDataPtr styleData, xsltInitExtCtxt * ctxt,
                const xmlChar * URI)
{
    xsltExtModulePtr module;
    xsltExtDataPtr ctxtData;
    void *extData;

    if ((styleData == NULL) || (ctxt == NULL) || (URI == NULL) ||
        (ctxt->ret == -1)) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: NULL param or error\n");
#endif
        return;
    }
    module = styleData->extModule;
    if ((module == NULL) || (module->initFunc == NULL)) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: no module or no initFunc\n");
#endif
        return;
    }

    ctxtData = (xsltExtDataPtr) xmlHashLookup(ctxt->ctxt->extInfos, URI);
    if (ctxtData != NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: already initialized\n");
#endif
        return;
    }

    extData = module->initFunc(ctxt->ctxt, URI);
    if (extData == NULL) {
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltInitCtxtExt: no extData\n");
#endif
    }
    ctxtData = xsltNewExtData(module, extData);
    if (ctxtData == NULL) {
        ctxt->ret = -1;
        return;
    }

    if (ctxt->ctxt->extInfos == NULL)
        ctxt->ctxt->extInfos = xmlHashCreate(10);
    if (ctxt->ctxt->extInfos == NULL) {
        ctxt->ret = -1;
        return;
    }

    if (xmlHashAddEntry(ctxt->ctxt->extInfos, URI, ctxtData) < 0) {
        xsltGenericError(xsltGenericErrorContext,
                         "Failed to register module data: %s\n", URI);
        if (module->shutdownFunc)
            module->shutdownFunc(ctxt->ctxt, URI, extData);
        xsltFreeExtData(ctxtData);
        ctxt->ret = -1;
        return;
    }
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext, "Registered module %s\n",
                     URI);
#endif
    ctxt->ret++;
}

int
xsltInitCtxtExts(xsltTransformContextPtr ctxt)
{
    xsltStylesheetPtr style;
    xsltInitExtCtxt ctx;

    if (ctxt == NULL)
        return (-1);

    style = ctxt->style;
    if (style == NULL)
        return (-1);

    ctx.ctxt = ctxt;
    ctx.ret = 0;

    while (style != NULL) {
        if (style->extInfos != NULL) {
            xmlHashScan(style->extInfos,
                        (xmlHashScanner) xsltInitCtxtExt, &ctx);
            if (ctx.ret == -1)
                return (-1);
        }
        style = xsltNextImport(style);
    }
#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext, "Registered %d modules\n",
                     ctx.ret);
#endif
    return (ctx.ret);
}

static void
xsltShutdownCtxtExt(xsltExtDataPtr data, xsltTransformContextPtr ctxt,
                    const xmlChar * URI)
{
    xsltExtModulePtr module;

    if ((data == NULL) || (ctxt == NULL) || (URI == NULL))
        return;
    module = data->extModule;
    if ((module == NULL) || (module->shutdownFunc == NULL))
        return;

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
                     "Shutting down module : %s\n", URI);
#endif
    module->shutdownFunc(ctxt, URI, data->extData);
}

void
xsltShutdownCtxtExts(xsltTransformContextPtr ctxt)
{
    if (ctxt == NULL)
        return;
    if (ctxt->extInfos == NULL)
        return;
    xmlHashScan(ctxt->extInfos, (xmlHashScanner) xsltShutdownCtxtExt,
                ctxt);
    xmlHashFree(ctxt->extInfos, (xmlHashDeallocator) xsltFreeExtData);
    ctxt->extInfos = NULL;
}

static void
xsltShutdownExt(xsltExtDataPtr data, xsltStylesheetPtr style,
                const xmlChar * URI)
{
    xsltExtModulePtr module;

    if ((data == NULL) || (style == NULL) || (URI == NULL))
        return;
    module = data->extModule;
    if ((module == NULL) || (module->styleShutdownFunc == NULL))
        return;

#ifdef WITH_XSLT_DEBUG_EXTENSIONS
    xsltGenericDebug(xsltGenericDebugContext,
                     "Shutting down module : %s\n", URI);
#endif
    module->styleShutdownFunc(style, URI, data->extData);
    
}

void
xsltShutdownExts(xsltStylesheetPtr style)
{
    if (style == NULL)
        return;
    if (style->extInfos == NULL)
        return;
    xmlHashScan(style->extInfos, (xmlHashScanner) xsltShutdownExt, style);
    xmlHashFree(style->extInfos, (xmlHashDeallocator) xsltFreeExtData);
    style->extInfos = NULL;
}

int
xsltCheckExtPrefix(xsltStylesheetPtr style, const xmlChar * URI)
{    
#ifdef XSLT_REFACTORED
    if ((style == NULL) || (style->compCtxt == NULL) ||
	(XSLT_CCTXT(style)->inode == NULL) ||
	(XSLT_CCTXT(style)->inode->extElemNs == NULL))
        return (0);    
    if (XSLT_CCTXT(style)->inode->extElemNs != NULL) {
	int i;
	xsltPointerListPtr list = XSLT_CCTXT(style)->inode->extElemNs;

	for (i = 0; i < list->number; i++) {
	    if (xmlStrEqual((const xmlChar *) list->items[i],
		URI))
	    {
		return(1);
	    }	    
	}
    }
#else
    xsltExtDefPtr cur;

    if ((style == NULL) || (style->nsDefs == NULL))
        return (0);
    if (URI == NULL)
        URI = BAD_CAST "#default";
    cur = (xsltExtDefPtr) style->nsDefs;
    while (cur != NULL) {
        if (xmlStrEqual(URI, cur->prefix))
            return (1);
        cur = cur->next;
    }
#endif
    return (0);
}

int
xsltCheckExtURI(xsltStylesheetPtr style, const xmlChar * URI)
{
    xsltExtDefPtr cur;

    if ((style == NULL) || (style->nsDefs == NULL))
        return (0);
    if (URI == NULL)
        return (0);
    cur = (xsltExtDefPtr) style->nsDefs;
    while (cur != NULL) {
        if (xmlStrEqual(URI, cur->URI))
            return (1);
        cur = cur->next;
    }
    return (0);
}

int
xsltRegisterExtModuleFull(const xmlChar * URI,
                          xsltExtInitFunction initFunc,
                          xsltExtShutdownFunction shutdownFunc,
                          xsltStyleExtInitFunction styleInitFunc,
                          xsltStyleExtShutdownFunction styleShutdownFunc)
{
    int ret;
    xsltExtModulePtr module;

    if ((URI == NULL) || (initFunc == NULL))
        return (-1);
    if (xsltExtensionsHash == NULL)
        xsltExtensionsHash = xmlHashCreate(10);

    if (xsltExtensionsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    module = xmlHashLookup(xsltExtensionsHash, URI);
    if (module != NULL) {
        if ((module->initFunc == initFunc) &&
            (module->shutdownFunc == shutdownFunc))
            ret = 0;
        else
            ret = -1;
        goto done;
    }
    module = xsltNewExtModule(initFunc, shutdownFunc,
                              styleInitFunc, styleShutdownFunc);
    if (module == NULL) {
        ret = -1;
        goto done;
    }
    ret = xmlHashAddEntry(xsltExtensionsHash, URI, (void *) module);

done:
    xmlMutexUnlock(xsltExtMutex);
    return (ret);
}

int
xsltRegisterExtModule(const xmlChar * URI,
                      xsltExtInitFunction initFunc,
                      xsltExtShutdownFunction shutdownFunc)
{
    return xsltRegisterExtModuleFull(URI, initFunc, shutdownFunc,
                                     NULL, NULL);
}

int
xsltUnregisterExtModule(const xmlChar * URI)
{
    int ret;

    if (URI == NULL)
        return (-1);
    if (xsltExtensionsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry(xsltExtensionsHash, URI,
                             (xmlHashDeallocator) xsltFreeExtModule);

    xmlMutexUnlock(xsltExtMutex);

    return (ret);
}

static void
xsltUnregisterAllExtModules(void)
{
    if (xsltExtensionsHash == NULL)
        return;

    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltExtensionsHash,
                (xmlHashDeallocator) xsltFreeExtModule);
    xsltExtensionsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}

xsltTransformContextPtr
xsltXPathGetTransformContext(xmlXPathParserContextPtr ctxt)
{
    if ((ctxt == NULL) || (ctxt->context == NULL))
        return (NULL);
    return (ctxt->context->extra);
}

int
xsltRegisterExtModuleFunction(const xmlChar * name, const xmlChar * URI,
                              xmlXPathFunction function)
{
    if ((name == NULL) || (URI == NULL) || (function == NULL))
        return (-1);

    if (xsltFunctionsHash == NULL)
        xsltFunctionsHash = xmlHashCreate(10);
    if (xsltFunctionsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    xmlHashUpdateEntry2(xsltFunctionsHash, name, URI,
                        XML_CAST_FPTR(function), NULL);

    xmlMutexUnlock(xsltExtMutex);

    return (0);
}

xmlXPathFunction
xsltExtModuleFunctionLookup(const xmlChar * name, const xmlChar * URI)
{
    xmlXPathFunction ret;

    if ((xsltFunctionsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    XML_CAST_FPTR(ret) = xmlHashLookup2(xsltFunctionsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

    
    if (NULL == ret) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            XML_CAST_FPTR(ret) =
                xmlHashLookup2(xsltFunctionsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    return ret;
}

int
xsltUnregisterExtModuleFunction(const xmlChar * name, const xmlChar * URI)
{
    int ret;

    if ((xsltFunctionsHash == NULL) || (name == NULL) || (URI == NULL))
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry2(xsltFunctionsHash, name, URI, NULL);

    xmlMutexUnlock(xsltExtMutex);

    return(ret);
}

static void
xsltUnregisterAllExtModuleFunction(void)
{
    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltFunctionsHash, NULL);
    xsltFunctionsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}


xsltElemPreCompPtr
xsltNewElemPreComp(xsltStylesheetPtr style, xmlNodePtr inst,
                   xsltTransformFunction function)
{
    xsltElemPreCompPtr cur;

    cur = (xsltElemPreCompPtr) xmlMalloc(sizeof(xsltElemPreComp));
    if (cur == NULL) {
        xsltTransformError(NULL, style, NULL,
                           "xsltNewExtElement : malloc failed\n");
        return (NULL);
    }
    memset(cur, 0, sizeof(xsltElemPreComp));

    xsltInitElemPreComp(cur, style, inst, function,
                        (xsltElemPreCompDeallocator) xmlFree);

    return (cur);
}

void
xsltInitElemPreComp(xsltElemPreCompPtr comp, xsltStylesheetPtr style,
                    xmlNodePtr inst, xsltTransformFunction function,
                    xsltElemPreCompDeallocator freeFunc)
{
    comp->type = XSLT_FUNC_EXTENSION;
    comp->func = function;
    comp->inst = inst;
    comp->free = freeFunc;

    comp->next = style->preComps;
    style->preComps = comp;
}

xsltElemPreCompPtr
xsltPreComputeExtModuleElement(xsltStylesheetPtr style, xmlNodePtr inst)
{
    xsltExtElementPtr ext;
    xsltElemPreCompPtr comp = NULL;

    if ((style == NULL) || (inst == NULL) ||
        (inst->type != XML_ELEMENT_NODE) || (inst->ns == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    ext = (xsltExtElementPtr)
        xmlHashLookup2(xsltElementsHash, inst->name, inst->ns->href);

    xmlMutexUnlock(xsltExtMutex);

    if (ext == NULL)
        return (NULL);

    if (ext->precomp != NULL) {
        comp = ext->precomp(style, inst, ext->transform);
    }
    if (comp == NULL) {
        comp = xsltNewElemPreComp(style, inst, ext->transform);
    }

    return (comp);
}

int
xsltRegisterExtModuleElement(const xmlChar * name, const xmlChar * URI,
                             xsltPreComputeFunction precomp,
                             xsltTransformFunction transform)
{
    int ret;

    xsltExtElementPtr ext;

    if ((name == NULL) || (URI == NULL) || (transform == NULL))
        return (-1);

    if (xsltElementsHash == NULL)
        xsltElementsHash = xmlHashCreate(10);
    if (xsltElementsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ext = xsltNewExtElement(precomp, transform);
    if (ext == NULL) {
        ret = -1;
        goto done;
    }

    xmlHashUpdateEntry2(xsltElementsHash, name, URI, (void *) ext,
                        (xmlHashDeallocator) xsltFreeExtElement);

done:
    xmlMutexUnlock(xsltExtMutex);

    return (0);
}

xsltTransformFunction
xsltExtElementLookup(xsltTransformContextPtr ctxt,
                     const xmlChar * name, const xmlChar * URI)
{
    xsltTransformFunction ret;

    if ((name == NULL) || (URI == NULL))
        return (NULL);

    if ((ctxt != NULL) && (ctxt->extElements != NULL)) {
        XML_CAST_FPTR(ret) = xmlHashLookup2(ctxt->extElements, name, URI);
        if (ret != NULL) {
            return(ret);
        }
    }

    ret = xsltExtModuleElementLookup(name, URI);

    return (ret);
}

xsltTransformFunction
xsltExtModuleElementLookup(const xmlChar * name, const xmlChar * URI)
{
    xsltExtElementPtr ext;

    if ((xsltElementsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    ext = (xsltExtElementPtr) xmlHashLookup2(xsltElementsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

    if (NULL == ext) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            ext = (xsltExtElementPtr)
	          xmlHashLookup2(xsltElementsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    if (ext == NULL)
        return (NULL);
    return (ext->transform);
}

xsltPreComputeFunction
xsltExtModuleElementPreComputeLookup(const xmlChar * name,
                                     const xmlChar * URI)
{
    xsltExtElementPtr ext;

    if ((xsltElementsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    ext = (xsltExtElementPtr) xmlHashLookup2(xsltElementsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

    if (ext == NULL) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            ext = (xsltExtElementPtr)
	          xmlHashLookup2(xsltElementsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    if (ext == NULL)
        return (NULL);
    return (ext->precomp);
}

int
xsltUnregisterExtModuleElement(const xmlChar * name, const xmlChar * URI)
{
    int ret;

    if ((xsltElementsHash == NULL) || (name == NULL) || (URI == NULL))
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry2(xsltElementsHash, name, URI,
                              (xmlHashDeallocator) xsltFreeExtElement);

    xmlMutexUnlock(xsltExtMutex);

    return(ret);
}

static void
xsltUnregisterAllExtModuleElement(void)
{
    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltElementsHash, (xmlHashDeallocator) xsltFreeExtElement);
    xsltElementsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}

int
xsltRegisterExtModuleTopLevel(const xmlChar * name, const xmlChar * URI,
                              xsltTopLevelFunction function)
{
    if ((name == NULL) || (URI == NULL) || (function == NULL))
        return (-1);

    if (xsltTopLevelsHash == NULL)
        xsltTopLevelsHash = xmlHashCreate(10);
    if (xsltTopLevelsHash == NULL)
        return (-1);

    xmlMutexLock(xsltExtMutex);

    xmlHashUpdateEntry2(xsltTopLevelsHash, name, URI,
                        XML_CAST_FPTR(function), NULL);

    xmlMutexUnlock(xsltExtMutex);

    return (0);
}

xsltTopLevelFunction
xsltExtModuleTopLevelLookup(const xmlChar * name, const xmlChar * URI)
{
    xsltTopLevelFunction ret;

    if ((xsltTopLevelsHash == NULL) || (name == NULL) || (URI == NULL))
        return (NULL);

    xmlMutexLock(xsltExtMutex);

    XML_CAST_FPTR(ret) = xmlHashLookup2(xsltTopLevelsHash, name, URI);

    xmlMutexUnlock(xsltExtMutex);

    
    if (NULL == ret) {
        if (!xsltExtModuleRegisterDynamic(URI)) {
            xmlMutexLock(xsltExtMutex);

            XML_CAST_FPTR(ret) = xmlHashLookup2(xsltTopLevelsHash, name, URI);

            xmlMutexUnlock(xsltExtMutex);
        }
    }

    return (ret);
}

int
xsltUnregisterExtModuleTopLevel(const xmlChar * name, const xmlChar * URI)
{
    int ret;

    if ((xsltTopLevelsHash == NULL) || (name == NULL) || (URI == NULL))
        return (-1);

    xmlMutexLock(xsltExtMutex);

    ret = xmlHashRemoveEntry2(xsltTopLevelsHash, name, URI, NULL);

    xmlMutexUnlock(xsltExtMutex);

    return(ret);
}

static void
xsltUnregisterAllExtModuleTopLevel(void)
{
    xmlMutexLock(xsltExtMutex);

    xmlHashFree(xsltTopLevelsHash, NULL);
    xsltTopLevelsHash = NULL;

    xmlMutexUnlock(xsltExtMutex);
}

xmlHashTablePtr
xsltGetExtInfo(xsltStylesheetPtr style, const xmlChar * URI)
{
    xsltExtDataPtr data;


    if (style != NULL && style->extInfos != NULL) {
        data = xmlHashLookup(style->extInfos, URI);
        if (data != NULL && data->extData != NULL)
            return data->extData;
    }
    return NULL;
}



static xmlChar *testData = NULL;
static xmlChar *testStyleData = NULL;

static void
xsltExtFunctionTest(xmlXPathParserContextPtr ctxt,
                    int nargs ATTRIBUTE_UNUSED)
{
    xsltTransformContextPtr tctxt;
    void *data = NULL;

    tctxt = xsltXPathGetTransformContext(ctxt);

    if (testData == NULL) {
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltExtFunctionTest: not initialized,"
                         " calling xsltGetExtData\n");
        data = xsltGetExtData(tctxt, (const xmlChar *) XSLT_DEFAULT_URL);
        if (data == NULL) {
            xsltTransformError(tctxt, NULL, NULL,
                               "xsltExtElementTest: not initialized\n");
            return;
        }
    }
    if (tctxt == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                           "xsltExtFunctionTest: failed to get the transformation context\n");
        return;
    }
    if (data == NULL)
        data = xsltGetExtData(tctxt, (const xmlChar *) XSLT_DEFAULT_URL);
    if (data == NULL) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                           "xsltExtFunctionTest: failed to get module data\n");
        return;
    }
    if (data != testData) {
        xsltTransformError(xsltXPathGetTransformContext(ctxt), NULL, NULL,
                           "xsltExtFunctionTest: got wrong module data\n");
        return;
    }
#ifdef WITH_XSLT_DEBUG_FUNCTION
    xsltGenericDebug(xsltGenericDebugContext,
                     "libxslt:test() called with %d args\n", nargs);
#endif
}

static xsltElemPreCompPtr
xsltExtElementPreCompTest(xsltStylesheetPtr style, xmlNodePtr inst,
                          xsltTransformFunction function)
{
    xsltElemPreCompPtr ret;

    if (style == NULL) {
        xsltTransformError(NULL, NULL, inst,
                           "xsltExtElementTest: no transformation context\n");
        return (NULL);
    }
    if (testStyleData == NULL) {
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltExtElementPreCompTest: not initialized,"
                         " calling xsltStyleGetExtData\n");
        xsltStyleGetExtData(style, (const xmlChar *) XSLT_DEFAULT_URL);
        if (testStyleData == NULL) {
            xsltTransformError(NULL, style, inst,
                               "xsltExtElementPreCompTest: not initialized\n");
            if (style != NULL)
                style->errors++;
            return (NULL);
        }
    }
    if (inst == NULL) {
        xsltTransformError(NULL, style, inst,
                           "xsltExtElementPreCompTest: no instruction\n");
        if (style != NULL)
            style->errors++;
        return (NULL);
    }
    ret = xsltNewElemPreComp(style, inst, function);
    return (ret);
}

static void
xsltExtElementTest(xsltTransformContextPtr ctxt, xmlNodePtr node,
                   xmlNodePtr inst,
                   xsltElemPreCompPtr comp ATTRIBUTE_UNUSED)
{
    xmlNodePtr commentNode;

    if (testData == NULL) {
        xsltGenericDebug(xsltGenericDebugContext,
                         "xsltExtElementTest: not initialized,"
                         " calling xsltGetExtData\n");
        xsltGetExtData(ctxt, (const xmlChar *) XSLT_DEFAULT_URL);
        if (testData == NULL) {
            xsltTransformError(ctxt, NULL, inst,
                               "xsltExtElementTest: not initialized\n");
            return;
        }
    }
    if (ctxt == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no transformation context\n");
        return;
    }
    if (node == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no current node\n");
        return;
    }
    if (inst == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no instruction\n");
        return;
    }
    if (ctxt->insert == NULL) {
        xsltTransformError(ctxt, NULL, inst,
                           "xsltExtElementTest: no insertion point\n");
        return;
    }
    commentNode = xmlNewComment((const xmlChar *)
                                "libxslt:test element test worked");
    xmlAddChild(ctxt->insert, commentNode);
}

static void *
xsltExtInitTest(xsltTransformContextPtr ctxt, const xmlChar * URI)
{
    if (testStyleData == NULL) {
        xsltGenericDebug(xsltGenericErrorContext,
                         "xsltExtInitTest: not initialized,"
                         " calling xsltStyleGetExtData\n");
        testStyleData = xsltStyleGetExtData(ctxt->style, URI);
        if (testStyleData == NULL) {
            xsltTransformError(ctxt, NULL, NULL,
                               "xsltExtInitTest: not initialized\n");
            return (NULL);
        }
    }
    if (testData != NULL) {
        xsltTransformError(ctxt, NULL, NULL,
                           "xsltExtInitTest: already initialized\n");
        return (NULL);
    }
    testData = (void *) "test data";
    xsltGenericDebug(xsltGenericDebugContext,
                     "Registered test module : %s\n", URI);
    return (testData);
}


static void
xsltExtShutdownTest(xsltTransformContextPtr ctxt,
                    const xmlChar * URI, void *data)
{
    if (testData == NULL) {
        xsltTransformError(ctxt, NULL, NULL,
                           "xsltExtShutdownTest: not initialized\n");
        return;
    }
    if (data != testData) {
        xsltTransformError(ctxt, NULL, NULL,
                           "xsltExtShutdownTest: wrong data\n");
    }
    testData = NULL;
    xsltGenericDebug(xsltGenericDebugContext,
                     "Unregistered test module : %s\n", URI);
}

static void *
xsltExtStyleInitTest(xsltStylesheetPtr style ATTRIBUTE_UNUSED,
                     const xmlChar * URI)
{
    if (testStyleData != NULL) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltExtInitTest: already initialized\n");
        return (NULL);
    }
    testStyleData = (void *) "test data";
    xsltGenericDebug(xsltGenericDebugContext,
                     "Registered test module : %s\n", URI);
    return (testStyleData);
}


static void
xsltExtStyleShutdownTest(xsltStylesheetPtr style ATTRIBUTE_UNUSED,
                         const xmlChar * URI, void *data)
{
    if (testStyleData == NULL) {
        xsltGenericError(xsltGenericErrorContext,
                         "xsltExtShutdownTest: not initialized\n");
        return;
    }
    if (data != testStyleData) {
        xsltTransformError(NULL, NULL, NULL,
                           "xsltExtShutdownTest: wrong data\n");
    }
    testStyleData = NULL;
    xsltGenericDebug(xsltGenericDebugContext,
                     "Unregistered test module : %s\n", URI);
}

void
xsltRegisterTestModule(void)
{
    xsltInitGlobals();
    xsltRegisterExtModuleFull((const xmlChar *) XSLT_DEFAULT_URL,
                              xsltExtInitTest, xsltExtShutdownTest,
                              xsltExtStyleInitTest,
                              xsltExtStyleShutdownTest);
    xsltRegisterExtModuleFunction((const xmlChar *) "test",
                                  (const xmlChar *) XSLT_DEFAULT_URL,
                                  xsltExtFunctionTest);
    xsltRegisterExtModuleElement((const xmlChar *) "test",
                                 (const xmlChar *) XSLT_DEFAULT_URL,
                                 xsltExtElementPreCompTest,
                                 xsltExtElementTest);
}

static void
xsltHashScannerModuleFree(void *payload ATTRIBUTE_UNUSED,
                          void *data ATTRIBUTE_UNUSED,
                          xmlChar * name ATTRIBUTE_UNUSED)
{
#ifdef WITH_MODULES
    xmlModuleClose(payload);
#endif
}

void
xsltInitGlobals(void)
{
    if (xsltExtMutex == NULL) {
        xsltExtMutex = xmlNewMutex();
    }
}

void
xsltCleanupGlobals(void)
{
    xsltUnregisterAllExtModules();
    xsltUnregisterAllExtModuleFunction();
    xsltUnregisterAllExtModuleElement();
    xsltUnregisterAllExtModuleTopLevel();

    xmlMutexLock(xsltExtMutex);
    
    if (NULL != xsltModuleHash) {
        xmlHashScan(xsltModuleHash, xsltHashScannerModuleFree, 0);
        xmlHashFree(xsltModuleHash, NULL);
        xsltModuleHash = NULL;
    }
    xmlMutexUnlock(xsltExtMutex);

    xmlFreeMutex(xsltExtMutex);
    xsltExtMutex = NULL;
    xsltUninit();
}

static void
xsltDebugDumpExtensionsCallback(void *function ATTRIBUTE_UNUSED,
                                FILE * output, const xmlChar * name,
                                const xmlChar * URI,
                                const xmlChar * not_used ATTRIBUTE_UNUSED)
{
    if (!name || !URI)
        return;
    fprintf(output, "{%s}%s\n", URI, name);
}

static void
xsltDebugDumpExtModulesCallback(void *function ATTRIBUTE_UNUSED,
                                FILE * output, const xmlChar * URI,
                                const xmlChar * not_used ATTRIBUTE_UNUSED,
                                const xmlChar * not_used2 ATTRIBUTE_UNUSED)
{
    if (!URI)
        return;
    fprintf(output, "%s\n", URI);
}

void
xsltDebugDumpExtensions(FILE * output)
{
    if (output == NULL)
        output = stdout;
    fprintf(output,
            "Registered XSLT Extensions\n--------------------------\n");
    if (!xsltFunctionsHash)
        fprintf(output, "No registered extension functions\n");
    else {
        fprintf(output, "Registered Extension Functions:\n");
        xmlMutexLock(xsltExtMutex);
        xmlHashScanFull(xsltFunctionsHash,
                        (xmlHashScannerFull)
                        xsltDebugDumpExtensionsCallback, output);
        xmlMutexUnlock(xsltExtMutex);
    }
    if (!xsltElementsHash)
        fprintf(output, "\nNo registered extension elements\n");
    else {
        fprintf(output, "\nRegistered Extension Elements:\n");
        xmlMutexLock(xsltExtMutex);
        xmlHashScanFull(xsltElementsHash,
                        (xmlHashScannerFull)
                        xsltDebugDumpExtensionsCallback, output);
        xmlMutexUnlock(xsltExtMutex);
    }
    if (!xsltExtensionsHash)
        fprintf(output, "\nNo registered extension modules\n");
    else {
        fprintf(output, "\nRegistered Extension Modules:\n");
        xmlMutexLock(xsltExtMutex);
        xmlHashScanFull(xsltExtensionsHash,
                        (xmlHashScannerFull)
                        xsltDebugDumpExtModulesCallback, output);
        xmlMutexUnlock(xsltExtMutex);
    }

}
