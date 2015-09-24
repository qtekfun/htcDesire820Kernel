/*
 * exsltconfig.h: compile-time version informations for the EXSLT library
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#ifndef __XML_EXSLTCONFIG_H__
#define __XML_EXSLTCONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LIBEXSLT_DOTTED_VERSION "1.1.26"

#define LIBEXSLT_VERSION 815

#define LIBEXSLT_VERSION_STRING "815"

#define	LIBEXSLT_VERSION_EXTRA ""

#if 1
#define EXSLT_CRYPTO_ENABLED
#endif

#ifdef __GNUC__
#ifdef HAVE_ANSIDECL_H
#include <ansidecl.h>
#endif
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((unused))
#endif
#else
#define ATTRIBUTE_UNUSED
#endif

#ifdef __cplusplus
}
#endif

#endif 
