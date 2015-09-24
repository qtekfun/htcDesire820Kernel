/*
 * xmlmodule.c : basic API for dynamic module loading added 2.6.17
 *
 * See Copyright for the status of this software.
 *
 * joelwreed@comcast.net
 *
 * http://www.fortran-2000.com/ArnaudRecipes/sharedlib.html
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlmodule.h>
#include <libxml/globals.h>

#ifdef LIBXML_MODULES_ENABLED

struct _xmlModule {
    unsigned char *name;
    void *handle;
};

static void *xmlModulePlatformOpen(const char *name);
static int xmlModulePlatformClose(void *handle);
static int xmlModulePlatformSymbol(void *handle, const char *name, void **result);


static void
xmlModuleErrMemory(xmlModulePtr module, const char *extra)
{
    const char *name = NULL;

    if (module != NULL) {
        name = (const char *) module->name;
    }

    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                    XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0, extra,
                    name, NULL, 0, 0,
                    "Memory allocation failed : %s\n", extra);
}

xmlModulePtr
xmlModuleOpen(const char *name, int options ATTRIBUTE_UNUSED)
{
    xmlModulePtr module;

    module = (xmlModulePtr) xmlMalloc(sizeof(xmlModule));
    if (module == NULL) {
        xmlModuleErrMemory(NULL, "creating module");
        return (NULL);
    }

    memset(module, 0, sizeof(xmlModule));

    module->handle = xmlModulePlatformOpen(name);

    if (module->handle == NULL) {
        xmlFree(module);
        __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                        XML_MODULE_OPEN, XML_ERR_FATAL, NULL, 0, 0,
                        name, NULL, 0, 0, "failed to open %s\n", name);
        return(NULL);
    }

    module->name = xmlStrdup((const xmlChar *) name);
    return (module);
}

int
xmlModuleSymbol(xmlModulePtr module, const char *name, void **symbol)
{
    int rc = -1;
	
    if ((NULL == module) || (symbol == NULL)) {
        __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                        XML_MODULE_OPEN, XML_ERR_FATAL, NULL, 0, 0,
                        NULL, NULL, 0, 0, "null parameter\n");
        return rc;
    }

    rc = xmlModulePlatformSymbol(module->handle, name, symbol);

    if (rc == -1) {
        __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                        XML_MODULE_OPEN, XML_ERR_FATAL, NULL, 0, 0,
                        name, NULL, 0, 0,
                        "failed to find symbol: %s\n",
			(name == NULL ? "NULL" : name));
        return rc;
    }

    return rc;
}

int
xmlModuleClose(xmlModulePtr module)
{
    int rc;

    if (NULL == module) {
        __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                        XML_MODULE_CLOSE, XML_ERR_FATAL, NULL, 0, 0,
                        NULL, NULL, 0, 0, "null module pointer\n");
        return -1;
    }

    rc = xmlModulePlatformClose(module->handle);

    if (rc != 0) {
        __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                        XML_MODULE_CLOSE, XML_ERR_FATAL, NULL, 0, 0,
                        (const char *) module->name, NULL, 0, 0,
                        "failed to close: %s\n", module->name);
        return -2;
    }

    rc = xmlModuleFree(module);
    return (rc);
}

int
xmlModuleFree(xmlModulePtr module)
{
    if (NULL == module) {
        __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_MODULE,
                        XML_MODULE_CLOSE, XML_ERR_FATAL, NULL, 0, NULL,
                        NULL, NULL, 0, 0, "null module pointer\n");
        return -1;
    }

    xmlFree(module->name);
    xmlFree(module);

    return (0);
}

#if defined(HAVE_DLOPEN) && !defined(_WIN32)
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifndef RTLD_GLOBAL            
#define RTLD_GLOBAL 0
#endif


static void *
xmlModulePlatformOpen(const char *name)
{
    return dlopen(name, RTLD_GLOBAL | RTLD_NOW);
}


static int
xmlModulePlatformClose(void *handle)
{
    return dlclose(handle);
}


static int
xmlModulePlatformSymbol(void *handle, const char *name, void **symbol)
{
    *symbol = dlsym(handle, name);
    if (dlerror() != NULL) {
	return -1;
    }
    return 0;
}

#else 

#ifdef HAVE_SHLLOAD             
#ifdef HAVE_DL_H
#include <dl.h>
#endif

static void *
xmlModulePlatformOpen(const char *name)
{
    return shl_load(name, BIND_IMMEDIATE, 0L);
}


static int
xmlModulePlatformClose(void *handle)
{
    return shl_unload(handle);
}


static int
xmlModulePlatformSymbol(void *handle, const char *name, void **symbol)
{
    int rc;

    errno = 0;
    rc = shl_findsym(&handle, name, TYPE_UNDEFINED, symbol);
    return rc;
}

#endif 
#endif 

#ifdef _WIN32

#include <windows.h>


static void *
xmlModulePlatformOpen(const char *name)
{
    return LoadLibraryA(name);
}


static int
xmlModulePlatformClose(void *handle)
{
    int rc;

    rc = FreeLibrary(handle);
    return (0 == rc);
}


static int
xmlModulePlatformSymbol(void *handle, const char *name, void **symbol)
{
    *symbol = GetProcAddress(handle, name);
    return (NULL == *symbol) ? -1 : 0;
}

#endif 

#ifdef HAVE_BEOS

#include <kernel/image.h>


static void *
xmlModulePlatformOpen(const char *name)
{
    return (void *) load_add_on(name);
}


static int
xmlModulePlatformClose(void *handle)
{
    status_t rc;

    rc = unload_add_on((image_id) handle);

    if (rc == B_OK)
        return 0;
    else
        return -1;
}


static int
xmlModulePlatformSymbol(void *handle, const char *name, void **symbol)
{
    status_t rc;

    rc = get_image_symbol((image_id) handle, name, B_SYMBOL_TYPE_ANY, symbol);

    return (rc == B_OK) ? 0 : -1;
}

#endif 

#ifdef HAVE_OS2

#include <os2.h>


static void *
xmlModulePlatformOpen(const char *name)
{
    char errbuf[256];
    void *handle;
    int rc;

    rc = DosLoadModule(errbuf, sizeof(errbuf) - 1, name, &handle);

    if (rc)
        return 0;
    else
        return (handle);
}


static int
xmlModulePlatformClose(void *handle)
{
    return DosFreeModule(handle);
}


static int
xmlModulePlatformSymbol(void *handle, const char *name, void **symbol)
{
    int rc;

    rc = DosQueryProcAddr(handle, 0, name, symbol);

    return (rc == NO_ERROR) ? 0 : -1;
}

#endif 

#define bottom_xmlmodule
#include "elfgcchack.h"
#endif 
