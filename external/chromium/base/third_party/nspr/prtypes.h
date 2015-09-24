/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape Portable Runtime (NSPR).
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#ifndef prtypes_h___
#define prtypes_h___

#ifdef MDCPUCFG
#include MDCPUCFG
#else
#include "base/third_party/nspr/prcpucfg.h"
#endif

#include <stddef.h>

#if 1


#define PR_EXPORT(__type) extern __type
#define PR_EXPORT_DATA(__type) extern __type
#define PR_IMPORT(__type) extern __type
#define PR_IMPORT_DATA(__type) extern __type

#define PR_EXTERN(__type) extern __type
#define PR_IMPLEMENT(__type) __type
#define PR_EXTERN_DATA(__type) extern __type
#define PR_IMPLEMENT_DATA(__type) __type

#define PR_CALLBACK
#define PR_CALLBACK_DECL
#define PR_STATIC_CALLBACK(__x) static __x

#elif defined(WIN32)

#define PR_EXPORT(__type) extern __declspec(dllexport) __type
#define PR_EXPORT_DATA(__type) extern __declspec(dllexport) __type
#define PR_IMPORT(__type) __declspec(dllimport) __type
#define PR_IMPORT_DATA(__type) __declspec(dllimport) __type

#define PR_EXTERN(__type) extern __declspec(dllexport) __type
#define PR_IMPLEMENT(__type) __declspec(dllexport) __type
#define PR_EXTERN_DATA(__type) extern __declspec(dllexport) __type
#define PR_IMPLEMENT_DATA(__type) __declspec(dllexport) __type

#define PR_CALLBACK
#define PR_CALLBACK_DECL
#define PR_STATIC_CALLBACK(__x) static __x

#elif defined(XP_BEOS)

#define PR_EXPORT(__type) extern __declspec(dllexport) __type
#define PR_EXPORT_DATA(__type) extern __declspec(dllexport) __type
#define PR_IMPORT(__type) extern __declspec(dllexport) __type
#define PR_IMPORT_DATA(__type) extern __declspec(dllexport) __type

#define PR_EXTERN(__type) extern __declspec(dllexport) __type
#define PR_IMPLEMENT(__type) __declspec(dllexport) __type
#define PR_EXTERN_DATA(__type) extern __declspec(dllexport) __type
#define PR_IMPLEMENT_DATA(__type) __declspec(dllexport) __type

#define PR_CALLBACK
#define PR_CALLBACK_DECL
#define PR_STATIC_CALLBACK(__x) static __x

#elif defined(XP_OS2) && defined(__declspec)

#define PR_EXPORT(__type) extern __declspec(dllexport) __type
#define PR_EXPORT_DATA(__type) extern __declspec(dllexport) __type
#define PR_IMPORT(__type) extern  __declspec(dllimport) __type
#define PR_IMPORT_DATA(__type) extern __declspec(dllimport) __type

#define PR_EXTERN(__type) extern __declspec(dllexport) __type
#define PR_IMPLEMENT(__type) __declspec(dllexport) __type
#define PR_EXTERN_DATA(__type) extern __declspec(dllexport) __type
#define PR_IMPLEMENT_DATA(__type) __declspec(dllexport) __type

#define PR_CALLBACK
#define PR_CALLBACK_DECL
#define PR_STATIC_CALLBACK(__x) static __x

#elif defined(SYMBIAN)

#define PR_EXPORT(__type) extern __declspec(dllexport) __type
#define PR_EXPORT_DATA(__type) extern __declspec(dllexport) __type
#ifdef __WINS__
#define PR_IMPORT(__type) extern __declspec(dllexport) __type
#define PR_IMPORT_DATA(__type) extern __declspec(dllexport) __type
#else
#define PR_IMPORT(__type) extern __declspec(dllimport) __type
#define PR_IMPORT_DATA(__type) extern __declspec(dllimport) __type
#endif

#define PR_EXTERN(__type) extern __type
#define PR_IMPLEMENT(__type) __type
#define PR_EXTERN_DATA(__type) extern __type
#define PR_IMPLEMENT_DATA(__type) __type

#define PR_CALLBACK
#define PR_CALLBACK_DECL
#define PR_STATIC_CALLBACK(__x) static __x

#else 

#if (__GNUC__ >= 4) || \
    (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
#define PR_VISIBILITY_DEFAULT __attribute__((visibility("default")))
#else
#define PR_VISIBILITY_DEFAULT
#endif

#define PR_EXPORT(__type) extern PR_VISIBILITY_DEFAULT __type
#define PR_EXPORT_DATA(__type) extern PR_VISIBILITY_DEFAULT __type
#define PR_IMPORT(__type) extern PR_VISIBILITY_DEFAULT __type
#define PR_IMPORT_DATA(__type) extern PR_VISIBILITY_DEFAULT __type

#define PR_EXTERN(__type) extern PR_VISIBILITY_DEFAULT __type
#define PR_IMPLEMENT(__type) PR_VISIBILITY_DEFAULT __type
#define PR_EXTERN_DATA(__type) extern PR_VISIBILITY_DEFAULT __type
#define PR_IMPLEMENT_DATA(__type) PR_VISIBILITY_DEFAULT __type
#define PR_CALLBACK
#define PR_CALLBACK_DECL
#define PR_STATIC_CALLBACK(__x) static __x

#endif

#if defined(_NSPR_BUILD_)
#define NSPR_API(__type) PR_EXPORT(__type)
#define NSPR_DATA_API(__type) PR_EXPORT_DATA(__type)
#else
#define NSPR_API(__type) PR_IMPORT(__type)
#define NSPR_DATA_API(__type) PR_IMPORT_DATA(__type)
#endif

#define PR_BEGIN_MACRO  do {
#define PR_END_MACRO    } while (0)

#ifdef __cplusplus
#define PR_BEGIN_EXTERN_C       extern "C" {
#define PR_END_EXTERN_C         }
#else
#define PR_BEGIN_EXTERN_C
#define PR_END_EXTERN_C
#endif

#define PR_BIT(n)       ((PRUint32)1 << (n))
#define PR_BITMASK(n)   (PR_BIT(n) - 1)

#define PR_ROUNDUP(x,y) ((((x)+((y)-1))/(y))*(y))
#define PR_MIN(x,y)     ((x)<(y)?(x):(y))
#define PR_MAX(x,y)     ((x)>(y)?(x):(y))
#define PR_ABS(x)       ((x)<0?-(x):(x))

PR_BEGIN_EXTERN_C

#if PR_BYTES_PER_BYTE == 1
typedef unsigned char PRUint8;
#if (defined(HPUX) && defined(__cplusplus) \
        && !defined(__GNUC__) && __cplusplus < 199707L) \
    || (defined(SCO) && defined(__cplusplus) \
        && !defined(__GNUC__) && __cplusplus == 1L)
typedef char PRInt8;
#else
typedef signed char PRInt8;
#endif
#else
#error No suitable type for PRInt8/PRUint8
#endif


#define PR_INT8_MAX 127
#define PR_INT8_MIN (-128)
#define PR_UINT8_MAX 255U

#if PR_BYTES_PER_SHORT == 2
typedef unsigned short PRUint16;
typedef short PRInt16;
#else
#error No suitable type for PRInt16/PRUint16
#endif


#define PR_INT16_MAX 32767
#define PR_INT16_MIN (-32768)
#define PR_UINT16_MAX 65535U

#if PR_BYTES_PER_INT == 4
typedef unsigned int PRUint32;
typedef int PRInt32;
#define PR_INT32(x)  x
#define PR_UINT32(x) x ## U
#elif PR_BYTES_PER_LONG == 4
typedef unsigned long PRUint32;
typedef long PRInt32;
#define PR_INT32(x)  x ## L
#define PR_UINT32(x) x ## UL
#else
#error No suitable type for PRInt32/PRUint32
#endif


#define PR_INT32_MAX PR_INT32(2147483647)
#define PR_INT32_MIN (-PR_INT32_MAX - 1)
#define PR_UINT32_MAX PR_UINT32(4294967295)

#ifdef HAVE_LONG_LONG
#if PR_BYTES_PER_LONG == 8 && !defined(__APPLE__)
typedef long PRInt64;
typedef unsigned long PRUint64;
#elif defined(WIN32) && !defined(__GNUC__)
typedef __int64  PRInt64;
typedef unsigned __int64 PRUint64;
#else
typedef long long PRInt64;
typedef unsigned long long PRUint64;
#endif 
#else  
typedef struct {
#ifdef IS_LITTLE_ENDIAN
    PRUint32 lo, hi;
#else
    PRUint32 hi, lo;
#endif
} PRInt64;
typedef PRInt64 PRUint64;
#endif 

#if PR_BYTES_PER_INT >= 2
typedef int PRIntn;
typedef unsigned int PRUintn;
#else
#error 'sizeof(int)' not sufficient for platform use
#endif

typedef double          PRFloat64;

typedef size_t PRSize;


typedef PRInt32 PROffset32;
typedef PRInt64 PROffset64;

typedef ptrdiff_t PRPtrdiff;

#ifdef _WIN64
typedef PRUint64 PRUptrdiff;
#else
typedef unsigned long PRUptrdiff;
#endif

typedef PRIntn PRBool;
#define PR_TRUE 1
#define PR_FALSE 0

typedef PRUint8 PRPackedBool;

typedef enum { PR_FAILURE = -1, PR_SUCCESS = 0 } PRStatus;

#ifndef __PRUNICHAR__
#define __PRUNICHAR__
#ifdef WIN32
typedef wchar_t PRUnichar;
#else
typedef PRUint16 PRUnichar;
#endif
#endif

#ifdef _WIN64
typedef PRInt64 PRWord;
typedef PRUint64 PRUword;
#else
typedef long PRWord;
typedef unsigned long PRUword;
#endif

#if defined(NO_NSPR_10_SUPPORT)
#else

#define PR_PUBLIC_API		PR_IMPLEMENT

#define NSPR_BEGIN_MACRO        do {
#define NSPR_END_MACRO          } while (0)

#ifdef NSPR_BEGIN_EXTERN_C
#undef NSPR_BEGIN_EXTERN_C
#endif
#ifdef NSPR_END_EXTERN_C
#undef NSPR_END_EXTERN_C
#endif

#ifdef __cplusplus
#define NSPR_BEGIN_EXTERN_C     extern "C" {
#define NSPR_END_EXTERN_C       }
#else
#define NSPR_BEGIN_EXTERN_C
#define NSPR_END_EXTERN_C
#endif

#endif 

#define PR_STATIC_ASSERT(condition) \
    extern void pr_static_assert(int arg[(condition) ? 1 : -1])

PR_END_EXTERN_C

#if !defined(NO_NSPR_10_SUPPORT)
#include "base/basictypes.h"
#endif

#endif 

