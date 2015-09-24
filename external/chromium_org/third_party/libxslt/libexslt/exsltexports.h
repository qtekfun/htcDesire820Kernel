/*
 * exsltexports.h : macros for marking symbols as exportable/importable.
 *
 * See Copyright for the status of this software.
 *
 * igor@zlatkovic.com
 */

#ifndef __EXSLT_EXPORTS_H__
#define __EXSLT_EXPORTS_H__


#define EXSLTPUBFUN
#define EXSLTPUBVAR extern
#define EXSLTCALL


#if defined(_WIN32) && defined(_MSC_VER)
  #undef EXSLTPUBFUN
  #undef EXSLTPUBVAR
  #undef EXSLTCALL
  #if defined(IN_LIBEXSLT) && !defined(LIBEXSLT_STATIC)
    #define EXSLTPUBFUN __declspec(dllexport)
    #define EXSLTPUBVAR __declspec(dllexport)
  #else
    #define EXSLTPUBFUN
    #if !defined(LIBEXSLT_STATIC)
      #define EXSLTPUBVAR __declspec(dllimport) extern
    #else
      #define EXSLTPUBVAR extern
    #endif
  #endif
  #define EXSLTCALL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__BORLANDC__)
  #undef EXSLTPUBFUN
  #undef EXSLTPUBVAR
  #undef EXSLTCALL
  #if defined(IN_LIBEXSLT) && !defined(LIBEXSLT_STATIC)
    #define EXSLTPUBFUN __declspec(dllexport)
    #define EXSLTPUBVAR __declspec(dllexport) extern
  #else
    #define EXSLTPUBFUN
    #if !defined(LIBEXSLT_STATIC)
      #define EXSLTPUBVAR __declspec(dllimport) extern
    #else
      #define EXSLTPUBVAR extern
    #endif
  #endif
  #define EXSLTCALL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__MINGW32__)
  #undef EXSLTPUBFUN
  #undef EXSLTPUBVAR
  #undef EXSLTCALL
  #if !defined(LIBEXSLT_STATIC)
    #define EXSLTPUBFUN __declspec(dllexport)
    #define EXSLTPUBVAR __declspec(dllexport) extern
  #else
    #define EXSLTPUBFUN
    #if !defined(LIBEXSLT_STATIC)
      #define EXSLTPUBVAR __declspec(dllimport) extern
    #else
      #define EXSLTPUBVAR extern
    #endif
  #endif
  #define EXSLTCALL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__CYGWIN__)
  #undef EXSLTPUBFUN
  #undef EXSLTPUBVAR
  #undef EXSLTCALL
  #if defined(IN_LIBEXSLT) && !defined(LIBEXSLT_STATIC)
    #define EXSLTPUBFUN __declspec(dllexport)
    #define EXSLTPUBVAR __declspec(dllexport)
  #else
    #define EXSLTPUBFUN
    #if !defined(LIBEXSLT_STATIC)
      #define EXSLTPUBVAR __declspec(dllimport) extern
    #else
      #define EXSLTPUBVAR
    #endif
  #endif
  #define EXSLTCALL __cdecl
#endif

#if !defined(LIBEXSLT_PUBLIC)
#define LIBEXSLT_PUBLIC EXSLTPUBVAR
#endif

#endif 


