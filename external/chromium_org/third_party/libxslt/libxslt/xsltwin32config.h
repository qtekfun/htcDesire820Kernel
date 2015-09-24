/*
 * Summary: compile-time version informations for the XSLT engine 
 *          when compiled on windows
 * Description: compile-time version informations for the XSLT engine
 *              when compiled on windows. This file is generated.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XSLTWIN32CONFIG_H__
#define __XML_XSLTWIN32CONFIG_H__

#include "win32config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIBXSLT_DOTTED_VERSION "1.1.26"

#define LIBXSLT_VERSION 10126

#define LIBXSLT_VERSION_STRING "10126"

#define LIBXSLT_VERSION_EXTRA "-win32"

#if 1
#define WITH_XSLT_DEBUG
#endif

#if 1
#ifndef WITH_MODULES
#define WITH_MODULES
#endif
#define LIBXSLT_PLUGINS_PATH() getenv("LIBXSLT_PLUGINS_PATH")
#endif

#if 0
#define DEBUG_MEMORY

#define DEBUG_MEMORY_LOCATION
#endif

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED
#endif

#ifdef __cplusplus
}
#endif

#endif 
