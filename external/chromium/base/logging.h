// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_
#pragma once

#include <string>
#include <cstring>
#include <sstream>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "build/build_config.h"




namespace logging {

enum LoggingDestination { LOG_NONE,
                          LOG_ONLY_TO_FILE,
                          LOG_ONLY_TO_SYSTEM_DEBUG_LOG,
                          LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG };

// Indicates that the log file should be locked when being written to.
enum LogLockingState { LOCK_LOG_FILE, DONT_LOCK_LOG_FILE };

enum OldFileDeletionState { DELETE_OLD_LOG_FILE, APPEND_TO_OLD_LOG_FILE };

enum DcheckState {
  DISABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS,
  ENABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS
};

#if defined(OS_WIN)
typedef wchar_t PathChar;
#else
typedef char PathChar;
#endif

#if NDEBUG
#define BaseInitLoggingImpl BaseInitLoggingImpl_built_with_NDEBUG
#else
#define BaseInitLoggingImpl BaseInitLoggingImpl_built_without_NDEBUG
#endif

BASE_API bool BaseInitLoggingImpl(const PathChar* log_file,
                                  LoggingDestination logging_dest,
                                  LogLockingState lock_log,
                                  OldFileDeletionState delete_old,
                                  DcheckState dcheck_state);

inline bool InitLogging(const PathChar* log_file,
                        LoggingDestination logging_dest,
                        LogLockingState lock_log,
                        OldFileDeletionState delete_old,
                        DcheckState dcheck_state) {
  return BaseInitLoggingImpl(log_file, logging_dest, lock_log,
                             delete_old, dcheck_state);
}

// Sets the log level. Anything at or above this level will be written to the
BASE_API void SetMinLogLevel(int level);

BASE_API int GetMinLogLevel();

BASE_API int GetVlogVerbosity();


BASE_API int GetVlogLevelHelper(const char* file_start, size_t N);

template <size_t N>
int GetVlogLevel(const char (&file)[N]) {
  return GetVlogLevelHelper(file, N);
}

BASE_API void SetLogItems(bool enable_process_id, bool enable_thread_id,
                          bool enable_timestamp, bool enable_tickcount);

BASE_API void SetShowErrorDialogs(bool enable_dialogs);

typedef void (*LogAssertHandlerFunction)(const std::string& str);
BASE_API void SetLogAssertHandler(LogAssertHandlerFunction handler);

typedef void (*LogReportHandlerFunction)(const std::string& str);
BASE_API void SetLogReportHandler(LogReportHandlerFunction handler);

typedef bool (*LogMessageHandlerFunction)(int severity,
    const char* file, int line, size_t message_start, const std::string& str);
BASE_API void SetLogMessageHandler(LogMessageHandlerFunction handler);
BASE_API LogMessageHandlerFunction GetLogMessageHandler();

typedef int LogSeverity;
const LogSeverity LOG_VERBOSE = -1;  
const LogSeverity LOG_INFO = 0;
const LogSeverity LOG_WARNING = 1;
const LogSeverity LOG_ERROR = 2;
const LogSeverity LOG_ERROR_REPORT = 3;
const LogSeverity LOG_FATAL = 4;
const LogSeverity LOG_NUM_SEVERITIES = 5;

#ifdef NDEBUG
const LogSeverity LOG_DFATAL = LOG_ERROR;
#else
const LogSeverity LOG_DFATAL = LOG_FATAL;
#endif

#define COMPACT_GOOGLE_LOG_EX_INFO(ClassName, ...) \
  logging::ClassName(__FILE__, __LINE__, logging::LOG_INFO , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_WARNING(ClassName, ...) \
  logging::ClassName(__FILE__, __LINE__, logging::LOG_WARNING , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_ERROR(ClassName, ...) \
  logging::ClassName(__FILE__, __LINE__, logging::LOG_ERROR , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_ERROR_REPORT(ClassName, ...) \
  logging::ClassName(__FILE__, __LINE__, \
                     logging::LOG_ERROR_REPORT , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_FATAL(ClassName, ...) \
  logging::ClassName(__FILE__, __LINE__, logging::LOG_FATAL , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_DFATAL(ClassName, ...) \
  logging::ClassName(__FILE__, __LINE__, logging::LOG_DFATAL , ##__VA_ARGS__)

#define COMPACT_GOOGLE_LOG_INFO \
  COMPACT_GOOGLE_LOG_EX_INFO(LogMessage)
#define COMPACT_GOOGLE_LOG_WARNING \
  COMPACT_GOOGLE_LOG_EX_WARNING(LogMessage)
#define COMPACT_GOOGLE_LOG_ERROR \
  COMPACT_GOOGLE_LOG_EX_ERROR(LogMessage)
#define COMPACT_GOOGLE_LOG_ERROR_REPORT \
  COMPACT_GOOGLE_LOG_EX_ERROR_REPORT(LogMessage)
#define COMPACT_GOOGLE_LOG_FATAL \
  COMPACT_GOOGLE_LOG_EX_FATAL(LogMessage)
#define COMPACT_GOOGLE_LOG_DFATAL \
  COMPACT_GOOGLE_LOG_EX_DFATAL(LogMessage)

#define ERROR 0
#define COMPACT_GOOGLE_LOG_EX_0(ClassName, ...) \
  COMPACT_GOOGLE_LOG_EX_ERROR(ClassName , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_0 COMPACT_GOOGLE_LOG_ERROR
const LogSeverity LOG_0 = LOG_ERROR;

#define LOG_IS_ON(severity) \
  ((::logging::LOG_ ## severity) >= ::logging::GetMinLogLevel())

#define VLOG_IS_ON(verboselevel) \
  ((verboselevel) <= ::logging::GetVlogLevel(__FILE__))

#define LAZY_STREAM(stream, condition)                                  \
  !(condition) ? (void) 0 : ::logging::LogMessageVoidify() & (stream)

#define LOG_STREAM(severity) COMPACT_GOOGLE_LOG_ ## severity.stream()

#define LOG(severity) LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity))
#define LOG_IF(severity, condition) \
  LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity) && (condition))

#define SYSLOG(severity) LOG(severity)
#define SYSLOG_IF(severity, condition) LOG_IF(severity, condition)

#define VLOG_STREAM(verbose_level) \
  logging::LogMessage(__FILE__, __LINE__, -verbose_level).stream()

#define VLOG(verbose_level) \
  LAZY_STREAM(VLOG_STREAM(verbose_level), VLOG_IS_ON(verbose_level))

#define VLOG_IF(verbose_level, condition) \
  LAZY_STREAM(VLOG_STREAM(verbose_level), \
      VLOG_IS_ON(verbose_level) && (condition))

#if defined (OS_WIN)
#define VPLOG_STREAM(verbose_level) \
  logging::Win32ErrorLogMessage(__FILE__, __LINE__, -verbose_level, \
    ::logging::GetLastSystemErrorCode()).stream()
#elif defined(OS_POSIX)
#define VPLOG_STREAM(verbose_level) \
  logging::ErrnoLogMessage(__FILE__, __LINE__, -verbose_level, \
    ::logging::GetLastSystemErrorCode()).stream()
#endif

#define VPLOG(verbose_level) \
  LAZY_STREAM(VPLOG_STREAM(verbose_level), VLOG_IS_ON(verbose_level))

#define VPLOG_IF(verbose_level, condition) \
  LAZY_STREAM(VPLOG_STREAM(verbose_level), \
    VLOG_IS_ON(verbose_level) && (condition))


#define LOG_ASSERT(condition)  \
  LOG_IF(FATAL, !(condition)) << "Assert failed: " #condition ". "
#define SYSLOG_ASSERT(condition) \
  SYSLOG_IF(FATAL, !(condition)) << "Assert failed: " #condition ". "

#if defined(OS_WIN)
#define LOG_GETLASTERROR_STREAM(severity) \
  COMPACT_GOOGLE_LOG_EX_ ## severity(Win32ErrorLogMessage, \
      ::logging::GetLastSystemErrorCode()).stream()
#define LOG_GETLASTERROR(severity) \
  LAZY_STREAM(LOG_GETLASTERROR_STREAM(severity), LOG_IS_ON(severity))
#define LOG_GETLASTERROR_MODULE_STREAM(severity, module) \
  COMPACT_GOOGLE_LOG_EX_ ## severity(Win32ErrorLogMessage, \
      ::logging::GetLastSystemErrorCode(), module).stream()
#define LOG_GETLASTERROR_MODULE(severity, module)                       \
  LAZY_STREAM(LOG_GETLASTERROR_STREAM(severity, module),                \
              LOG_IS_ON(severity))
#define PLOG_STREAM(severity) LOG_GETLASTERROR_STREAM(severity)
#elif defined(OS_POSIX)
#define LOG_ERRNO_STREAM(severity) \
  COMPACT_GOOGLE_LOG_EX_ ## severity(ErrnoLogMessage, \
      ::logging::GetLastSystemErrorCode()).stream()
#define LOG_ERRNO(severity) \
  LAZY_STREAM(LOG_ERRNO_STREAM(severity), LOG_IS_ON(severity))
#define PLOG_STREAM(severity) LOG_ERRNO_STREAM(severity)
#endif

#define PLOG(severity)                                          \
  LAZY_STREAM(PLOG_STREAM(severity), LOG_IS_ON(severity))

#define PLOG_IF(severity, condition) \
  LAZY_STREAM(PLOG_STREAM(severity), LOG_IS_ON(severity) && (condition))

#define CHECK(condition)                       \
  LAZY_STREAM(LOG_STREAM(FATAL), !(condition)) \
  << "Check failed: " #condition ". "

#define PCHECK(condition) \
  LAZY_STREAM(PLOG_STREAM(FATAL), !(condition)) \
  << "Check failed: " #condition ". "

template<class t1, class t2>
#ifdef ANDROID
BASE_API
#endif
std::string* MakeCheckOpString(const t1& v1, const t2& v2, const char* names) {
  std::ostringstream ss;
  ss << names << " (" << v1 << " vs. " << v2 << ")";
  std::string* msg = new std::string(ss.str());
  return msg;
}

#if !defined(COMPILER_MSVC)
extern template std::string* MakeCheckOpString<int, int>(
    const int&, const int&, const char* names);
extern template std::string* MakeCheckOpString<unsigned long, unsigned long>(
    const unsigned long&, const unsigned long&, const char* names);
extern template std::string* MakeCheckOpString<unsigned long, unsigned int>(
    const unsigned long&, const unsigned int&, const char* names);
extern template std::string* MakeCheckOpString<unsigned int, unsigned long>(
    const unsigned int&, const unsigned long&, const char* names);
extern template std::string* MakeCheckOpString<std::string, std::string>(
    const std::string&, const std::string&, const char* name);
#endif

#define CHECK_OP(name, op, val1, val2)                          \
  if (std::string* _result =                                    \
      logging::Check##name##Impl((val1), (val2),                \
                                 #val1 " " #op " " #val2))      \
    logging::LogMessage(__FILE__, __LINE__, _result).stream()

#define DEFINE_CHECK_OP_IMPL(name, op) \
  template <class t1, class t2> \
  inline std::string* Check##name##Impl(const t1& v1, const t2& v2, \
                                        const char* names) { \
    if (v1 op v2) return NULL; \
    else return MakeCheckOpString(v1, v2, names); \
  } \
  inline std::string* Check##name##Impl(int v1, int v2, const char* names) { \
    if (v1 op v2) return NULL; \
    else return MakeCheckOpString(v1, v2, names); \
  }
DEFINE_CHECK_OP_IMPL(EQ, ==)
DEFINE_CHECK_OP_IMPL(NE, !=)
DEFINE_CHECK_OP_IMPL(LE, <=)
DEFINE_CHECK_OP_IMPL(LT, < )
DEFINE_CHECK_OP_IMPL(GE, >=)
DEFINE_CHECK_OP_IMPL(GT, > )
#undef DEFINE_CHECK_OP_IMPL

#define CHECK_EQ(val1, val2) CHECK_OP(EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(LT, < , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(GT, > , val1, val2)

#if ( defined(OS_WIN) && defined(OFFICIAL_BUILD)) || \
    (!defined(OS_WIN) && defined(NDEBUG) && defined(GOOGLE_CHROME_BUILD))
#define LOGGING_IS_OFFICIAL_BUILD

#define ENABLE_DLOG 0
#define ENABLE_DCHECK 0

#elif defined(NDEBUG)
#define ENABLE_DLOG 0
#define ENABLE_DCHECK 1

#else
#define ENABLE_DLOG 1
#define ENABLE_DCHECK 1
#endif


#if ENABLE_DLOG

#define DLOG_IS_ON(severity) LOG_IS_ON(severity)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#define DLOG_ASSERT(condition) LOG_ASSERT(condition)
#define DPLOG_IF(severity, condition) PLOG_IF(severity, condition)
#define DVLOG_IF(verboselevel, condition) VLOG_IF(verboselevel, condition)
#define DVPLOG_IF(verboselevel, condition) VPLOG_IF(verboselevel, condition)

#else  


#define DLOG_EAT_STREAM_PARAMETERS                                      \
  true ? (void) 0 : ::logging::LogMessageVoidify() & LOG_STREAM(FATAL)

#define DLOG_IS_ON(severity) false
#define DLOG_IF(severity, condition) DLOG_EAT_STREAM_PARAMETERS
#define DLOG_ASSERT(condition) DLOG_EAT_STREAM_PARAMETERS
#define DPLOG_IF(severity, condition) DLOG_EAT_STREAM_PARAMETERS
#define DVLOG_IF(verboselevel, condition) DLOG_EAT_STREAM_PARAMETERS
#define DVPLOG_IF(verboselevel, condition) DLOG_EAT_STREAM_PARAMETERS

#endif  

enum { DEBUG_MODE = ENABLE_DLOG };

#undef ENABLE_DLOG

#define DLOG(severity)                                          \
  LAZY_STREAM(LOG_STREAM(severity), DLOG_IS_ON(severity))

#if defined(OS_WIN)
#define DLOG_GETLASTERROR(severity) \
  LAZY_STREAM(LOG_GETLASTERROR_STREAM(severity), DLOG_IS_ON(severity))
#define DLOG_GETLASTERROR_MODULE(severity, module)                      \
  LAZY_STREAM(LOG_GETLASTERROR_STREAM(severity, module),                \
              DLOG_IS_ON(severity))
#elif defined(OS_POSIX)
#define DLOG_ERRNO(severity)                                    \
  LAZY_STREAM(LOG_ERRNO_STREAM(severity), DLOG_IS_ON(severity))
#endif

#define DPLOG(severity)                                         \
  LAZY_STREAM(PLOG_STREAM(severity), DLOG_IS_ON(severity))

#define DVLOG(verboselevel) DLOG_IF(INFO, VLOG_IS_ON(verboselevel))

#define DVPLOG(verboselevel) DVPLOG_IF(verboselevel, VLOG_IS_ON(verboselevel))


#if ENABLE_DCHECK

#if defined(NDEBUG)

#define COMPACT_GOOGLE_LOG_EX_DCHECK(ClassName, ...) \
  COMPACT_GOOGLE_LOG_EX_ERROR_REPORT(ClassName , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_DCHECK COMPACT_GOOGLE_LOG_ERROR_REPORT
const LogSeverity LOG_DCHECK = LOG_ERROR_REPORT;
extern DcheckState g_dcheck_state;
#define DCHECK_IS_ON()                                                  \
  ((::logging::g_dcheck_state ==                                        \
    ::logging::ENABLE_DCHECK_FOR_NON_OFFICIAL_RELEASE_BUILDS) &&        \
   LOG_IS_ON(DCHECK))

#else  

#define COMPACT_GOOGLE_LOG_EX_DCHECK(ClassName, ...) \
  COMPACT_GOOGLE_LOG_EX_FATAL(ClassName , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_DCHECK COMPACT_GOOGLE_LOG_FATAL
const LogSeverity LOG_DCHECK = LOG_FATAL;
#define DCHECK_IS_ON() true

#endif  

#else  

#define COMPACT_GOOGLE_LOG_EX_DCHECK(ClassName, ...) \
  COMPACT_GOOGLE_LOG_EX_INFO(ClassName , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_DCHECK COMPACT_GOOGLE_LOG_INFO
const LogSeverity LOG_DCHECK = LOG_INFO;
#define DCHECK_IS_ON() false

#endif  
#undef ENABLE_DCHECK


#define DCHECK(condition)                                           \
  LAZY_STREAM(LOG_STREAM(DCHECK), DCHECK_IS_ON() && !(condition))   \
  << "Check failed: " #condition ". "

#define DPCHECK(condition)                                          \
  LAZY_STREAM(PLOG_STREAM(DCHECK), DCHECK_IS_ON() && !(condition))  \
  << "Check failed: " #condition ". "

#define DCHECK_OP(name, op, val1, val2)                         \
  if (DCHECK_IS_ON())                                           \
    if (std::string* _result =                                  \
        logging::Check##name##Impl((val1), (val2),              \
                                   #val1 " " #op " " #val2))    \
      logging::LogMessage(                                      \
          __FILE__, __LINE__, ::logging::LOG_DCHECK,            \
          _result).stream()


#define DCHECK_EQ(val1, val2) DCHECK_OP(EQ, ==, val1, val2)
#define DCHECK_NE(val1, val2) DCHECK_OP(NE, !=, val1, val2)
#define DCHECK_LE(val1, val2) DCHECK_OP(LE, <=, val1, val2)
#define DCHECK_LT(val1, val2) DCHECK_OP(LT, < , val1, val2)
#define DCHECK_GE(val1, val2) DCHECK_OP(GE, >=, val1, val2)
#define DCHECK_GT(val1, val2) DCHECK_OP(GT, > , val1, val2)

#define NOTREACHED() DCHECK(false)

#undef assert
#define assert(x) DLOG_ASSERT(x)

class BASE_API LogMessage {
 public:
  LogMessage(const char* file, int line, LogSeverity severity, int ctr);

  
  
  
  
  
  
  
  
  LogMessage(const char* file, int line);

  
  
  
  
  
  LogMessage(const char* file, int line, LogSeverity severity);

  
  
  
  LogMessage(const char* file, int line, std::string* result);

  
  
  LogMessage(const char* file, int line, LogSeverity severity,
             std::string* result);

  ~LogMessage();

  std::ostream& stream() { return stream_; }

 private:
  void Init(const char* file, int line);

  LogSeverity severity_;
  std::ostringstream stream_;
  size_t message_start_;  
                          
  
  const char* file_;
  const int line_;

#if defined(OS_WIN)
  
  
  
  
  
  class SaveLastError {
   public:
    SaveLastError();
    ~SaveLastError();

    unsigned long get_error() const { return last_error_; }

   protected:
    unsigned long last_error_;
  };

  SaveLastError last_error_;
#endif

  DISALLOW_COPY_AND_ASSIGN(LogMessage);
};

inline void LogAtLevel(int const log_level, std::string const &msg) {
  LogMessage(__FILE__, __LINE__, log_level).stream() << msg;
}

class BASE_API LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  
  
  void operator&(std::ostream&) { }
};

#if defined(OS_WIN)
typedef unsigned long SystemErrorCode;
#elif defined(OS_POSIX)
typedef int SystemErrorCode;
#endif

BASE_API SystemErrorCode GetLastSystemErrorCode();

#if defined(OS_WIN)
class BASE_API Win32ErrorLogMessage {
 public:
  Win32ErrorLogMessage(const char* file,
                       int line,
                       LogSeverity severity,
                       SystemErrorCode err,
                       const char* module);

  Win32ErrorLogMessage(const char* file,
                       int line,
                       LogSeverity severity,
                       SystemErrorCode err);

  
  ~Win32ErrorLogMessage();

  std::ostream& stream() { return log_message_.stream(); }

 private:
  SystemErrorCode err_;
  
  const char* module_;
  LogMessage log_message_;

  DISALLOW_COPY_AND_ASSIGN(Win32ErrorLogMessage);
};
#elif defined(OS_POSIX)
class ErrnoLogMessage {
 public:
  ErrnoLogMessage(const char* file,
                  int line,
                  LogSeverity severity,
                  SystemErrorCode err);

  
  ~ErrnoLogMessage();

  std::ostream& stream() { return log_message_.stream(); }

 private:
  SystemErrorCode err_;
  LogMessage log_message_;

  DISALLOW_COPY_AND_ASSIGN(ErrnoLogMessage);
};
#endif  

BASE_API void CloseLogFile();

BASE_API void RawLog(int level, const char* message);

#define RAW_LOG(level, message) logging::RawLog(logging::LOG_ ## level, message)

#define RAW_CHECK(condition)                                                   \
  do {                                                                         \
    if (!(condition))                                                          \
      logging::RawLog(logging::LOG_FATAL, "Check failed: " #condition "\n");   \
  } while (0)

}  

BASE_API std::ostream& operator<<(std::ostream& out, const wchar_t* wstr);
inline std::ostream& operator<<(std::ostream& out, const std::wstring& wstr) {
  return out << wstr.c_str();
}


#ifndef NOTIMPLEMENTED_POLICY
#define NOTIMPLEMENTED_POLICY 4
#endif

#if defined(COMPILER_GCC)
#define NOTIMPLEMENTED_MSG "Not implemented reached in " << __PRETTY_FUNCTION__
#else
#define NOTIMPLEMENTED_MSG "NOT IMPLEMENTED"
#endif

#if NOTIMPLEMENTED_POLICY == 0
#define NOTIMPLEMENTED() ;
#elif NOTIMPLEMENTED_POLICY == 1
#define NOTIMPLEMENTED() COMPILE_ASSERT(false, NOT_IMPLEMENTED)
#elif NOTIMPLEMENTED_POLICY == 2
#define NOTIMPLEMENTED() COMPILE_ASSERT(false, NOT_IMPLEMENTED)
#elif NOTIMPLEMENTED_POLICY == 3
#define NOTIMPLEMENTED() NOTREACHED()
#elif NOTIMPLEMENTED_POLICY == 4
#define NOTIMPLEMENTED() LOG(ERROR) << NOTIMPLEMENTED_MSG
#elif NOTIMPLEMENTED_POLICY == 5
#define NOTIMPLEMENTED() do {\
  static int count = 0;\
  LOG_IF(ERROR, 0 == count++) << NOTIMPLEMENTED_MSG;\
} while(0)
#endif

namespace base {

class StringPiece;

BASE_API std::ostream& operator<<(std::ostream& o, const StringPiece& piece);

}  

#endif  
