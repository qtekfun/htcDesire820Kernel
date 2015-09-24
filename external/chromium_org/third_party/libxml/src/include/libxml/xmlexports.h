/*
 * Summary: macros for marking symbols as exportable/importable.
 * Description: macros for marking symbols as exportable/importable.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Igor Zlatovic <igor@zlatkovic.com>
 */

#ifndef __XML_EXPORTS_H__
#define __XML_EXPORTS_H__


#define XMLPUBFUN
#define XMLPUBVAR extern
#define XMLCALL
#define XMLCDECL


#if defined(_WIN32) && defined(_MSC_VER)
  #undef XMLPUBFUN
  #undef XMLPUBVAR
  #undef XMLCALL
  #undef XMLCDECL
  #if defined(IN_LIBXML) && !defined(LIBXML_STATIC)
    #define XMLPUBFUN __declspec(dllexport)
    #define XMLPUBVAR __declspec(dllexport)
  #else
    #define XMLPUBFUN
    #if !defined(LIBXML_STATIC)
      #define XMLPUBVAR __declspec(dllimport) extern
    #else
      #define XMLPUBVAR extern
    #endif
  #endif
  #if defined(LIBXML_FASTCALL)
    #define XMLCALL __fastcall
  #else
    #define XMLCALL __cdecl
  #endif
  #define XMLCDECL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__BORLANDC__)
  #undef XMLPUBFUN
  #undef XMLPUBVAR
  #undef XMLCALL
  #undef XMLCDECL
  #if defined(IN_LIBXML) && !defined(LIBXML_STATIC)
    #define XMLPUBFUN __declspec(dllexport)
    #define XMLPUBVAR __declspec(dllexport) extern
  #else
    #define XMLPUBFUN
    #if !defined(LIBXML_STATIC)
      #define XMLPUBVAR __declspec(dllimport) extern
    #else
      #define XMLPUBVAR extern
    #endif
  #endif
  #define XMLCALL __cdecl
  #define XMLCDECL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__MINGW32__)
  #undef XMLPUBFUN
  #undef XMLPUBVAR
  #undef XMLCALL
  #undef XMLCDECL
  #if defined(IN_LIBXML) && !defined(LIBXML_STATIC)
    #define XMLPUBFUN __declspec(dllexport)
    #define XMLPUBVAR __declspec(dllexport)
  #else
    #define XMLPUBFUN
    #if !defined(LIBXML_STATIC)
      #define XMLPUBVAR __declspec(dllimport) extern
    #else
      #define XMLPUBVAR extern
    #endif
  #endif
  #define XMLCALL __cdecl
  #define XMLCDECL __cdecl
  #if !defined _REENTRANT
    #define _REENTRANT
  #endif
#endif

#if defined(_WIN32) && defined(__CYGWIN__)
  #undef XMLPUBFUN
  #undef XMLPUBVAR
  #undef XMLCALL
  #undef XMLCDECL
  #if defined(IN_LIBXML) && !defined(LIBXML_STATIC)
    #define XMLPUBFUN __declspec(dllexport)
    #define XMLPUBVAR __declspec(dllexport)
  #else
    #define XMLPUBFUN
    #if !defined(LIBXML_STATIC)
      #define XMLPUBVAR __declspec(dllimport) extern
    #else
      #define XMLPUBVAR
    #endif
  #endif
  #define XMLCALL __cdecl
  #define XMLCDECL __cdecl
#endif

#if !defined(LIBXML_DLL_IMPORT)
#define LIBXML_DLL_IMPORT XMLPUBVAR
#endif

#endif 


