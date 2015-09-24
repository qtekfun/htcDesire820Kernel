/*
 * Summary: macros for marking symbols as exportable/importable.
 * Description: macros for marking symbols as exportable/importable.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Igor Zlatkovic <igor@zlatkovic.com>
 */

#ifndef __XSLT_EXPORTS_H__
#define __XSLT_EXPORTS_H__


#define XSLTPUBFUN
#define XSLTPUBVAR extern
#define XSLTCALL


#if defined(_WIN32) && defined(_MSC_VER)
  #undef XSLTPUBFUN
  #undef XSLTPUBVAR
  #undef XSLTCALL
  #if defined(IN_LIBXSLT) && !defined(LIBXSLT_STATIC)
    #define XSLTPUBFUN __declspec(dllexport)
    #define XSLTPUBVAR __declspec(dllexport)
  #else
    #define XSLTPUBFUN
    #if !defined(LIBXSLT_STATIC)
      #define XSLTPUBVAR __declspec(dllimport) extern
    #else
      #define XSLTPUBVAR extern
    #endif
  #endif
  #define XSLTCALL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__BORLANDC__)
  #undef XSLTPUBFUN
  #undef XSLTPUBVAR
  #undef XSLTCALL
  #if defined(IN_LIBXSLT) && !defined(LIBXSLT_STATIC)
    #define XSLTPUBFUN __declspec(dllexport)
    #define XSLTPUBVAR __declspec(dllexport) extern
  #else
    #define XSLTPUBFUN
    #if !defined(LIBXSLT_STATIC)
      #define XSLTPUBVAR __declspec(dllimport) extern
    #else
      #define XSLTPUBVAR extern
    #endif
  #endif
  #define XSLTCALL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__MINGW32__)
  #undef XSLTPUBFUN
  #undef XSLTPUBVAR
  #undef XSLTCALL
  #if !defined(LIBXSLT_STATIC)
    #define XSLTPUBFUN __declspec(dllexport)
    #define XSLTPUBVAR __declspec(dllexport) extern
  #else
    #define XSLTPUBFUN
    #if !defined(LIBXSLT_STATIC)
      #define XSLTPUBVAR __declspec(dllimport) extern
    #else
      #define XSLTPUBVAR extern
    #endif
  #endif
  #define XSLTCALL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__CYGWIN__)
  #undef XSLTPUBFUN
  #undef XSLTPUBVAR
  #undef XSLTCALL
  #if defined(IN_LIBXSLT) && !defined(LIBXSLT_STATIC)
    #define XSLTPUBFUN __declspec(dllexport)
    #define XSLTPUBVAR __declspec(dllexport)
  #else
    #define XSLTPUBFUN
    #if !defined(LIBXSLT_STATIC)
      #define XSLTPUBVAR __declspec(dllimport) extern
    #else
      #define XSLTPUBVAR
    #endif
  #endif
  #define XSLTCALL __cdecl
#endif

#if !defined(LIBXSLT_PUBLIC)
#define LIBXSLT_PUBLIC XSLTPUBVAR
#endif

#endif 


