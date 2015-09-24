// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <config.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>    
#endif
#include <string.h>    
#include <assert.h>
#include <errno.h>     
#include "base/commandlineflags.h"

#if defined(_MSC_VER)
#define WRITE_TO_STDERR(buf, len) WriteToStderr(buf, len);  
#elif defined(HAVE_SYS_SYSCALL_H)
#include <sys/syscall.h>
#define WRITE_TO_STDERR(buf, len) syscall(SYS_write, STDERR_FILENO, buf, len)
#else
#define WRITE_TO_STDERR(buf, len) write(STDERR_FILENO, buf, len)
#endif

#ifndef _WIN32
# define perftools_vsnprintf vsnprintf
#endif


DECLARE_int32(verbose);

#define CHECK(condition)                                                \
  do {                                                                  \
    if (!(condition)) {                                                 \
      WRITE_TO_STDERR("Check failed: " #condition "\n",                 \
                      sizeof("Check failed: " #condition "\n")-1);      \
      abort();                                                          \
    }                                                                   \
  } while (0)

#define RAW_CHECK(condition, message)                                          \
  do {                                                                         \
    if (!(condition)) {                                                        \
      WRITE_TO_STDERR("Check failed: " #condition ": " message "\n",           \
                      sizeof("Check failed: " #condition ": " message "\n")-1);\
      abort();                                                                 \
    }                                                                          \
  } while (0)

#ifdef NDEBUG
enum { DEBUG_MODE = 0 };
#define RAW_DCHECK(condition, message)
#else
enum { DEBUG_MODE = 1 };
#define RAW_DCHECK(condition, message)  RAW_CHECK(condition, message)
#endif

#define PCHECK(condition)                                               \
  do {                                                                  \
    if (!(condition)) {                                                 \
      const int err_no = errno;                                         \
      WRITE_TO_STDERR("Check failed: " #condition ": ",                 \
                      sizeof("Check failed: " #condition ": ")-1);      \
      WRITE_TO_STDERR(strerror(err_no), strlen(strerror(err_no)));      \
      WRITE_TO_STDERR("\n", sizeof("\n")-1);                            \
      abort();                                                          \
    }                                                                   \
  } while (0)



#define CHECK_OP(op, val1, val2)                                        \
  do {                                                                  \
    if (!((val1) op (val2))) {                                          \
      fprintf(stderr, "Check failed: %s %s %s\n", #val1, #op, #val2);   \
      abort();                                                          \
    }                                                                   \
  } while (0)

#define CHECK_EQ(val1, val2) CHECK_OP(==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(!=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(<=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(< , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(>=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(> , val1, val2)

#define EXPECT_EQ(val1, val2) CHECK_EQ(val1, val2)
#define EXPECT_NE(val1, val2) CHECK_NE(val1, val2)
#define EXPECT_LE(val1, val2) CHECK_LE(val1, val2)
#define EXPECT_LT(val1, val2) CHECK_LT(val1, val2)
#define EXPECT_GE(val1, val2) CHECK_GE(val1, val2)
#define EXPECT_GT(val1, val2) CHECK_GT(val1, val2)
#define ASSERT_EQ(val1, val2) EXPECT_EQ(val1, val2)
#define ASSERT_NE(val1, val2) EXPECT_NE(val1, val2)
#define ASSERT_LE(val1, val2) EXPECT_LE(val1, val2)
#define ASSERT_LT(val1, val2) EXPECT_LT(val1, val2)
#define ASSERT_GE(val1, val2) EXPECT_GE(val1, val2)
#define ASSERT_GT(val1, val2) EXPECT_GT(val1, val2)
#define EXPECT_TRUE(cond)     CHECK(cond)
#define EXPECT_FALSE(cond)    CHECK(!(cond))
#define EXPECT_STREQ(a, b)    CHECK(strcmp(a, b) == 0)
#define ASSERT_TRUE(cond)     EXPECT_TRUE(cond)
#define ASSERT_FALSE(cond)    EXPECT_FALSE(cond)
#define ASSERT_STREQ(a, b)    EXPECT_STREQ(a, b)

#define CHECK_ERR(invocation)  PCHECK((invocation) != -1)

#ifdef NDEBUG
#define DCHECK_EQ(val1, val2)
#define DCHECK_NE(val1, val2)
#define DCHECK_LE(val1, val2)
#define DCHECK_LT(val1, val2)
#define DCHECK_GE(val1, val2)
#define DCHECK_GT(val1, val2)
#else
#define DCHECK_EQ(val1, val2)  CHECK_EQ(val1, val2)
#define DCHECK_NE(val1, val2)  CHECK_NE(val1, val2)
#define DCHECK_LE(val1, val2)  CHECK_LE(val1, val2)
#define DCHECK_LT(val1, val2)  CHECK_LT(val1, val2)
#define DCHECK_GE(val1, val2)  CHECK_GE(val1, val2)
#define DCHECK_GT(val1, val2)  CHECK_GT(val1, val2)
#endif


#ifdef ERROR
#undef ERROR      
#endif
enum LogSeverity {INFO = -1, WARNING = -2, ERROR = -3, FATAL = -4};

inline void LogPrintf(int severity, const char* pat, va_list ap) {
  
  
  char buf[600];
  perftools_vsnprintf(buf, sizeof(buf)-1, pat, ap);
  if (buf[0] != '\0' && buf[strlen(buf)-1] != '\n') {
    assert(strlen(buf)+1 < sizeof(buf));
    strcat(buf, "\n");
  }
  WRITE_TO_STDERR(buf, strlen(buf));
  if ((severity) == FATAL)
    abort(); 
}

#define VLOG_IS_ON(severity) (FLAGS_verbose >= severity)

#define LOG_PRINTF(severity, pat) do {          \
  if (VLOG_IS_ON(severity)) {                   \
    va_list ap;                                 \
    va_start(ap, pat);                          \
    LogPrintf(severity, pat, ap);               \
    va_end(ap);                                 \
  }                                             \
} while (0)

inline void RAW_LOG(int lvl, const char* pat, ...)  { LOG_PRINTF(lvl, pat); }
inline void RAW_VLOG(int lvl, const char* pat, ...) { LOG_PRINTF(lvl, pat); }
inline void LOG(int lvl, const char* pat, ...)      { LOG_PRINTF(lvl, pat); }
inline void VLOG(int lvl, const char* pat, ...)     { LOG_PRINTF(lvl, pat); }
inline void LOG_IF(int lvl, bool cond, const char* pat, ...) {
  if (cond)  LOG_PRINTF(lvl, pat);
}

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__CYGWIN32__)
#ifndef NOMINMAX
#define NOMINMAX     
#endif
#include <windows.h>
typedef HANDLE RawFD;
const RawFD kIllegalRawFD = INVALID_HANDLE_VALUE;
#else
typedef int RawFD;
const RawFD kIllegalRawFD = -1;   
#endif  

RawFD RawOpenForWriting(const char* filename);   
void RawWrite(RawFD fd, const char* buf, size_t len);
void RawClose(RawFD fd);

#endif 
