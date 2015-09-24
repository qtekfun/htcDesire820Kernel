// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_INTERNAL_LOGGING_H_
#define TCMALLOC_INTERNAL_LOGGING_H_

#include <config.h>
#include <stddef.h>                     
#if defined HAVE_STDINT_H
#include <stdint.h>
#elif defined HAVE_INTTYPES_H
#include <inttypes.h>
#else
#include <sys/types.h>
#endif



namespace tcmalloc {
enum LogMode {
  kLog,                       
  kCrash,                     
  kCrashWithStats             
};

class Logger;

class LogItem {
 public:
  LogItem()                     : tag_(kEnd)      { }
  LogItem(const char* v)        : tag_(kStr)      { u_.str = v; }
  LogItem(int v)                : tag_(kSigned)   { u_.snum = v; }
  LogItem(long v)               : tag_(kSigned)   { u_.snum = v; }
  LogItem(long long v)          : tag_(kSigned)   { u_.snum = v; }
  LogItem(unsigned int v)       : tag_(kUnsigned) { u_.unum = v; }
  LogItem(unsigned long v)      : tag_(kUnsigned) { u_.unum = v; }
  LogItem(unsigned long long v) : tag_(kUnsigned) { u_.unum = v; }
  LogItem(const void* v)        : tag_(kPtr)      { u_.ptr = v; }
 private:
  friend class Logger;
  enum Tag {
    kStr,
    kSigned,
    kUnsigned,
    kPtr,
    kEnd
  };
  Tag tag_;
  union {
    const char* str;
    const void* ptr;
    int64_t snum;
    uint64_t unum;
  } u_;
};

extern PERFTOOLS_DLL_DECL void Log(LogMode mode, const char* filename, int line,
                LogItem a, LogItem b = LogItem(),
                LogItem c = LogItem(), LogItem d = LogItem());

extern PERFTOOLS_DLL_DECL void (*log_message_writer)(const char* msg, int length);

}  

#undef CHECK_CONDITION
#define CHECK_CONDITION(cond)                                            \
do {                                                                     \
  if (!(cond)) {                                                         \
    ::tcmalloc::Log(::tcmalloc::kCrash, __FILE__, __LINE__, #cond);      \
  }                                                                      \
} while (0)

#ifndef NDEBUG
#define ASSERT(cond) CHECK_CONDITION(cond)
#else
#define ASSERT(cond) ((void) 0)
#endif

class TCMalloc_Printer {
 private:
  char* buf_;           
  int   left_;          

 public:
  
  TCMalloc_Printer(char* buf, int length) : buf_(buf), left_(length) {
    buf[0] = '\0';
  }

  void printf(const char* format, ...)
#ifdef HAVE___ATTRIBUTE__
    __attribute__ ((__format__ (__printf__, 2, 3)))
#endif
;
};

#endif  
