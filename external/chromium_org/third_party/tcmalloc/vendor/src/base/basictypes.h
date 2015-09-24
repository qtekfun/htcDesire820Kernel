// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef _BASICTYPES_H_
#define _BASICTYPES_H_

#include <config.h>
#include <string.h>       
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>     
#endif


#ifdef HAVE_INTTYPES_H
#include <inttypes.h>           
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>             
#endif
#include <sys/types.h>          

typedef signed char         schar;
typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef int64_t             int64;


typedef uint8_t            uint8;
typedef uint16_t           uint16;
typedef uint32_t           uint32;
typedef uint64_t           uint64;

const uint16 kuint16max = (   (uint16) 0xFFFF);
const uint32 kuint32max = (   (uint32) 0xFFFFFFFF);
const uint64 kuint64max = ( (((uint64) kuint32max) << 32) | kuint32max );

const  int8  kint8max   = (   (  int8) 0x7F);
const  int16 kint16max  = (   ( int16) 0x7FFF);
const  int32 kint32max  = (   ( int32) 0x7FFFFFFF);
const  int64 kint64max =  ( ((( int64) kint32max) << 32) | kuint32max );

const  int8  kint8min   = (   (  int8) 0x80);
const  int16 kint16min  = (   ( int16) 0x8000);
const  int32 kint32min  = (   ( int32) 0x80000000);
const  int64 kint64min =  ( ((( int64) kint32min) << 32) | 0 );

#ifndef PRIx64
#define PRIx64 "llx"
#endif
#ifndef SCNx64
#define SCNx64 "llx"
#endif
#ifndef PRId64
#define PRId64 "lld"
#endif
#ifndef SCNd64
#define SCNd64 "lld"
#endif
#ifndef PRIu64
#define PRIu64 "llu"
#endif
#ifndef PRIxPTR
#define PRIxPTR "lx"
#endif

#define GPRIuPTHREAD "lu"
#define GPRIxPTHREAD "lx"
#if defined(__CYGWIN__) || defined(__CYGWIN32__) || defined(__APPLE__) || defined(__FreeBSD__)
#define PRINTABLE_PTHREAD(pthreadt) reinterpret_cast<uintptr_t>(pthreadt)
#else
#define PRINTABLE_PTHREAD(pthreadt) pthreadt
#endif

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
  TypeName(const TypeName&);                    \
  void operator=(const TypeName&)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) DISALLOW_EVIL_CONSTRUCTORS(TypeName)

//   to work around a bug in gcc 3.4.4 and 4.0.1.  If we had written

template <bool>
struct CompileAssert {
};

#define COMPILE_ASSERT(expr, msg)                               \
  typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]

#define arraysize(a)  (sizeof(a) / sizeof(*(a)))

#define OFFSETOF_MEMBER(strct, field)                                   \
   (reinterpret_cast<char*>(&reinterpret_cast<strct*>(16)->field) -     \
    reinterpret_cast<char*>(16))


template <class Dest, class Source>
inline Dest bit_cast(const Source& source) {
  COMPILE_ASSERT(sizeof(Dest) == sizeof(Source), bitcasting_unequal_sizes);
  Dest dest;
  memcpy(&dest, &source, sizeof(dest));
  return dest;
}

#ifdef HAVE___ATTRIBUTE__
# define ATTRIBUTE_WEAK      __attribute__((weak))
# define ATTRIBUTE_NOINLINE  __attribute__((noinline))
#else
# define ATTRIBUTE_WEAK
# define ATTRIBUTE_NOINLINE
#endif


#if defined(HAVE___ATTRIBUTE__) && defined(__ELF__)
# define ATTRIBUTE_SECTION(name) __attribute__ ((section (#name)))

  
  
  
# define DECLARE_ATTRIBUTE_SECTION_VARS(name) \
    extern char __start_##name[] ATTRIBUTE_WEAK; \
    extern char __stop_##name[] ATTRIBUTE_WEAK
# define INIT_ATTRIBUTE_SECTION_VARS(name)     
# define DEFINE_ATTRIBUTE_SECTION_VARS(name)   

  
  
  
# define ATTRIBUTE_SECTION_START(name) (reinterpret_cast<void*>(__start_##name))
# define ATTRIBUTE_SECTION_STOP(name) (reinterpret_cast<void*>(__stop_##name))
# define HAVE_ATTRIBUTE_SECTION_START 1

#elif defined(HAVE___ATTRIBUTE__) && defined(__MACH__)
# define ATTRIBUTE_SECTION(name) __attribute__ ((section ("__TEXT, " #name)))

#include <mach-o/getsect.h>
#include <mach-o/dyld.h>
class AssignAttributeStartEnd {
 public:
  AssignAttributeStartEnd(const char* name, char** pstart, char** pend) {
    
    if (_dyld_present()) {
      for (int i = _dyld_image_count() - 1; i >= 0; --i) {
        const mach_header* hdr = _dyld_get_image_header(i);
#ifdef MH_MAGIC_64
        if (hdr->magic == MH_MAGIC_64) {
          uint64_t len;
          *pstart = getsectdatafromheader_64((mach_header_64*)hdr,
                                             "__TEXT", name, &len);
          if (*pstart) {   
            *pstart += _dyld_get_image_vmaddr_slide(i);   
            *pend = *pstart + len;
            return;
          }
        }
#endif
        if (hdr->magic == MH_MAGIC) {
          uint32_t len;
          *pstart = getsectdatafromheader(hdr, "__TEXT", name, &len);
          if (*pstart) {   
            *pstart += _dyld_get_image_vmaddr_slide(i);   
            *pend = *pstart + len;
            return;
          }
        }
      }
    }
    
    unsigned long len;    
    *pstart = getsectdata("__TEXT", name, &len);
    *pend = *pstart + len;
  }
};

#define DECLARE_ATTRIBUTE_SECTION_VARS(name)    \
  extern char* __start_##name;                  \
  extern char* __stop_##name

#define INIT_ATTRIBUTE_SECTION_VARS(name)               \
  DECLARE_ATTRIBUTE_SECTION_VARS(name);                 \
  static const AssignAttributeStartEnd __assign_##name( \
    #name, &__start_##name, &__stop_##name)

#define DEFINE_ATTRIBUTE_SECTION_VARS(name)     \
  char* __start_##name, *__stop_##name;         \
  INIT_ATTRIBUTE_SECTION_VARS(name)

# define ATTRIBUTE_SECTION_START(name) (reinterpret_cast<void*>(__start_##name))
# define ATTRIBUTE_SECTION_STOP(name) (reinterpret_cast<void*>(__stop_##name))
# define HAVE_ATTRIBUTE_SECTION_START 1

#else  
# define ATTRIBUTE_SECTION(name)
# define DECLARE_ATTRIBUTE_SECTION_VARS(name)
# define INIT_ATTRIBUTE_SECTION_VARS(name)
# define DEFINE_ATTRIBUTE_SECTION_VARS(name)
# define ATTRIBUTE_SECTION_START(name) (reinterpret_cast<void*>(0))
# define ATTRIBUTE_SECTION_STOP(name) (reinterpret_cast<void*>(0))

#endif  

#if defined(HAVE___ATTRIBUTE__) && (defined(__i386__) || defined(__x86_64__))
# define CACHELINE_ALIGNED __attribute__((aligned(64)))
#else
# define CACHELINE_ALIGNED
#endif  


namespace base {
enum LinkerInitialized { LINKER_INITIALIZED };
}

#endif  
