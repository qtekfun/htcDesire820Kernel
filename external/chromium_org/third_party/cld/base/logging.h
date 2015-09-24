// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <errno.h>
#include <string.h>
#include <time.h>
#include <string>
#include <strstream>
#include <vector>

#ifndef COMPILER_MSVC
#include <unistd.h>   
#endif

#include "base/port.h"
#include "base/basictypes.h"
#include "base/commandlineflags.h"
#include "base/crash.h"
#include "base/dynamic_annotations.h"
#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "base/stl_decl_msvc.h"
#include "base/log_severity.h"
#include "base/vlog_is_on.h"
#include "global_strip_options.h"

// Unless otherwise specified, logs will be written to the filename


DECLARE_bool(logtostderr);

DECLARE_bool(alsologtostderr);

DECLARE_int32(stderrthreshold);

DECLARE_bool(log_prefix);

DECLARE_int32(logbuflevel);

DECLARE_int32(logbufsecs);

DECLARE_bool(logging);

DECLARE_int32(minloglevel);

// If specified, logfiles are written into this directory instead of the
DECLARE_string(log_dir);

DECLARE_string(log_link);

DECLARE_int32(max_log_size);

DECLARE_bool(threaded_logging);

DECLARE_bool(status_messages_to_status_file);

DECLARE_bool(stop_logging_if_full_disk);



#if STRIP_LOG == 0
#define COMPACT_GOOGLE_LOG_INFO LogMessage(__FILE__, __LINE__)
#define LOG_TO_STRING_INFO(message) LogMessage(__FILE__, __LINE__, INFO, \
                                               message)
#else
#define COMPACT_GOOGLE_LOG_INFO NullStream()
#define LOG_TO_STRING_INFO(message) NullStream()
#endif

#if STRIP_LOG <= 1
#define COMPACT_GOOGLE_LOG_WARNING LogMessage(__FILE__, __LINE__, WARNING)
#define LOG_TO_STRING_WARNING(message) LogMessage(__FILE__, __LINE__, \
                                                  WARNING, message)
#else
#define COMPACT_GOOGLE_LOG_WARNING NullStream()
#define LOG_TO_STRING_WARNING(message) NullStream()
#endif

#if STRIP_LOG <= 2
#define COMPACT_GOOGLE_LOG_ERROR LogMessage(__FILE__, __LINE__, ERROR)
#define LOG_TO_STRING_ERROR(message) LogMessage(__FILE__, __LINE__, ERROR, \
                                                message)
#else
#define COMPACT_GOOGLE_LOG_ERROR NullStream()
#define LOG_TO_STRING_ERROR(message) NullStream()
#endif

#if STRIP_LOG <= 3
#define COMPACT_GOOGLE_LOG_FATAL LogMessageFatal(__FILE__, __LINE__)
#define COMPACT_GOOGLE_LOG_QFATAL LogMessageQuietlyFatal(__FILE__, __LINE__)
#define LOG_TO_STRING_FATAL(message) LogMessage(__FILE__, __LINE__, FATAL, \
                                                message)
#else
#define COMPACT_GOOGLE_LOG_FATAL NullStreamFatal()
#define COMPACT_GOOGLE_LOG_QFATAL NullStreamFatal()
#define LOG_TO_STRING_FATAL(message) NullStreamFatal()
#endif

#ifdef NDEBUG
#define COMPACT_GOOGLE_LOG_DFATAL COMPACT_GOOGLE_LOG_ERROR
#elif STRIP_LOG <= 3
#define COMPACT_GOOGLE_LOG_DFATAL LogMessage(__FILE__, __LINE__, FATAL)
#else
#define COMPACT_GOOGLE_LOG_DFATAL NullStreamFatal()
#endif

#define GOOGLE_LOG_INFO(counter) \
  LogMessage(__FILE__, __LINE__, INFO, counter, &LogMessage::SendToLog)
#define SYSLOG_INFO(counter) \
  LogMessage(__FILE__, __LINE__, INFO, counter, \
  &LogMessage::SendToSyslogAndLog)
#define GOOGLE_LOG_WARNING(counter)  \
  LogMessage(__FILE__, __LINE__, WARNING, counter, &LogMessage::SendToLog)
#define SYSLOG_WARNING(counter)  \
  LogMessage(__FILE__, __LINE__, WARNING, counter, \
  &LogMessage::SendToSyslogAndLog)
#define GOOGLE_LOG_ERROR(counter)  \
  LogMessage(__FILE__, __LINE__, ERROR, counter, &LogMessage::SendToLog)
#define SYSLOG_ERROR(counter)  \
  LogMessage(__FILE__, __LINE__, ERROR, counter, \
  &LogMessage::SendToSyslogAndLog)
#define GOOGLE_LOG_FATAL(counter) \
  LogMessage(__FILE__, __LINE__, FATAL, counter, &LogMessage::SendToLog)
#define SYSLOG_FATAL(counter) \
  LogMessage(__FILE__, __LINE__, FATAL, counter, \
  &LogMessage::SendToSyslogAndLog)
#define GOOGLE_LOG_DFATAL(counter) \
  LogMessage(__FILE__, __LINE__, DFATAL_LEVEL, counter, &LogMessage::SendToLog)
#define SYSLOG_DFATAL(counter) \
  LogMessage(__FILE__, __LINE__, DFATAL_LEVEL, counter, \
  &LogMessage::SendToSyslogAndLog)

#ifdef OS_WINDOWS
#define LOG_SYSRESULT(result) \
  if (FAILED(result)) { \
    LPTSTR message = NULL; \
    LPTSTR msg = reinterpret_cast<LPTSTR>(&message); \
    DWORD message_length = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | \
                         FORMAT_MESSAGE_FROM_SYSTEM, \
                         0, result, 0, msg, 100, NULL); \
    if (message_length > 0) { \
      LogMessage(__FILE__, __LINE__, ERROR, 0, \
                 &LogMessage::SendToLog).stream() << message; \
      LocalFree(message); \
    } \
  }
#endif

#define LOG(severity) COMPACT_GOOGLE_LOG_ ## severity.stream()
#define SYSLOG(severity) SYSLOG_ ## severity(0).stream()

#define LG LOG(INFO)

class LogSink;  

#define LOG_TO_SINK(sink, severity) \
  LogMessage(__FILE__, __LINE__, severity, \
             static_cast<LogSink*>(sink), true).stream()
#define LOG_TO_SINK_BUT_NOT_TO_LOGFILE(sink, severity) \
  LogMessage(__FILE__, __LINE__, severity, \
             static_cast<LogSink*>(sink), false).stream()

#define LOG_TO_STRING(severity, message) \
  LOG_TO_STRING_##severity(static_cast<string*>(message)).stream()

#define LOG_STRING(severity, outvec) \
  LOG_TO_STRING_##severity(static_cast<vector<string>*>(outvec)).stream()

#define LOG_IF(severity, condition) \
  !(condition) ? (void) 0 : LogMessageVoidify() & LOG(severity)
#define SYSLOG_IF(severity, condition) \
  !(condition) ? (void) 0 : LogMessageVoidify() & SYSLOG(severity)

#define LOG_ASSERT(condition)  \
  LOG_IF(FATAL, !(condition)) << "Assert failed: " #condition
#define SYSLOG_ASSERT(condition) \
  SYSLOG_IF(FATAL, !(condition)) << "Assert failed: " #condition

#define CHECK(condition)  \
      LOG_IF(FATAL, PREDICT_FALSE(!(condition))) \
             << "Check failed: " #condition " "

#define QCHECK(condition)  \
      LOG_IF(QFATAL, PREDICT_FALSE(!(condition))) \
             << "Check failed: " #condition " "

struct CheckOpString {
  CheckOpString(string* str) : str_(str) { }
  
  
  operator bool() const { return PREDICT_FALSE(str_ != NULL); }
  string* str_;
};

template <class T>
inline const T&       GetReferenceableValue(const T&           t) { return t; }
inline char           GetReferenceableValue(char               t) { return t; }
inline unsigned char  GetReferenceableValue(unsigned char      t) { return t; }
inline signed char    GetReferenceableValue(signed char        t) { return t; }
inline short          GetReferenceableValue(short              t) { return t; }
inline unsigned short GetReferenceableValue(unsigned short     t) { return t; }
inline int            GetReferenceableValue(int                t) { return t; }
inline unsigned int   GetReferenceableValue(unsigned int       t) { return t; }
inline long           GetReferenceableValue(long               t) { return t; }
inline unsigned long  GetReferenceableValue(unsigned long      t) { return t; }
inline long long      GetReferenceableValue(long long          t) { return t; }
inline unsigned long long GetReferenceableValue(unsigned long long t) {
  return t;
}

template<class t1, class t2>
string* MakeCheckOpString(const t1& v1, const t2& v2, const char* names) {
  strstream ss;
  ss << names << " (" << v1 << " vs. " << v2 << ")";
  return new string(ss.str(), ss.pcount());
}

#define DEFINE_CHECK_OP_IMPL(name, op) \
  template <class t1, class t2> \
  inline string* Check##name##Impl(const t1& v1, const t2& v2, \
                                   const char* names) { \
    if (v1 op v2) return NULL; \
    else return MakeCheckOpString(v1, v2, names); \
  } \
  inline string* Check##name##Impl(int v1, int v2, const char* names) { \
    return Check##name##Impl<int, int>(v1, v2, names); \
  }

DEFINE_CHECK_OP_IMPL(_EQ, ==)
DEFINE_CHECK_OP_IMPL(_NE, !=)
DEFINE_CHECK_OP_IMPL(_LE, <=)
DEFINE_CHECK_OP_IMPL(_LT, < )
DEFINE_CHECK_OP_IMPL(_GE, >=)
DEFINE_CHECK_OP_IMPL(_GT, > )
#undef DEFINE_CHECK_OP_IMPL


#if defined(STATIC_ANALYSIS)
#define CHECK_OP_LOG(name, op, val1, val2, log) CHECK((val1) op (val2))
#elif !defined(NDEBUG)

typedef string _Check_string;
#define CHECK_OP_LOG(name, op, val1, val2, log) \
  while (_Check_string* _result = \
         Check##name##Impl(GetReferenceableValue(val1), \
                           GetReferenceableValue(val2), \
                           #val1 " " #op " " #val2)) \
    log(__FILE__, __LINE__, CheckOpString(_result)).stream()
#else
#define CHECK_OP_LOG(name, op, val1, val2, log) \
  while (CheckOpString _result = \
         Check##name##Impl(GetReferenceableValue(val1), \
                           GetReferenceableValue(val2), \
                           #val1 " " #op " " #val2)) \
    log(__FILE__, __LINE__, _result).stream()
#endif  

#if STRIP_LOG <= 3
#define CHECK_OP(name, op, val1, val2) \
  CHECK_OP_LOG(name, op, val1, val2, LogMessageFatal)
#else
#define CHECK_OP(name, op, val1, val2) \
  CHECK_OP_LOG(name, op, val1, val2, NullStreamFatal)
#endif 
#define QCHECK_OP(name, op, val1, val2) \
  CHECK_OP_LOG(name, op, val1, val2, LogMessageQuietlyFatal)


#define CHECK_EQ(val1, val2) CHECK_OP(_EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(_NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(_LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(_LT, < , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(_GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(_GT, > , val1, val2)

#define QCHECK_EQ(val1, val2) QCHECK_OP(_EQ, ==, val1, val2)
#define QCHECK_NE(val1, val2) QCHECK_OP(_NE, !=, val1, val2)
#define QCHECK_LE(val1, val2) QCHECK_OP(_LE, <=, val1, val2)
#define QCHECK_LT(val1, val2) QCHECK_OP(_LT, < , val1, val2)
#define QCHECK_GE(val1, val2) QCHECK_OP(_GE, >=, val1, val2)
#define QCHECK_GT(val1, val2) QCHECK_OP(_GT, > , val1, val2)



#define CHECK_NOTNULL(val) \
  CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

#define DECLARE_CHECK_STROP_IMPL(func, expected) \
  string* Check##func##expected##Impl(const char* s1, const char* s2, \
                                      const char* names);
DECLARE_CHECK_STROP_IMPL(strcmp, true)
DECLARE_CHECK_STROP_IMPL(strcmp, false)
DECLARE_CHECK_STROP_IMPL(strcasecmp, true)
DECLARE_CHECK_STROP_IMPL(strcasecmp, false)
#undef DECLARE_CHECK_STROP_IMPL

#define CHECK_STROP(func, op, expected, s1, s2) \
  while (CheckOpString _result = \
         Check##func##expected##Impl((s1), (s2), \
                                     #s1 " " #op " " #s2)) \
    LOG(FATAL) << *_result.str_
#define QCHECK_STROP(func, op, expected, s1, s2) \
  while (CheckOpString _result = \
         Check##func##expected##Impl((s1), (s2), \
                                     #s1 " " #op " " #s2)) \
    LOG(QFATAL) << *_result.str_



#define CHECK_STREQ(s1, s2) CHECK_STROP(strcmp, ==, true, s1, s2)
#define CHECK_STRNE(s1, s2) CHECK_STROP(strcmp, !=, false, s1, s2)
#define CHECK_STRCASEEQ(s1, s2) CHECK_STROP(strcasecmp, ==, true, s1, s2)
#define CHECK_STRCASENE(s1, s2) CHECK_STROP(strcasecmp, !=, false, s1, s2)

#define CHECK_INDEX(I,A) CHECK(I < (sizeof(A)/sizeof(A[0])))
#define CHECK_BOUND(B,A) CHECK(B <= (sizeof(A)/sizeof(A[0])))

#define QCHECK_STREQ(s1, s2) QCHECK_STROP(strcmp, ==, true, s1, s2)
#define QCHECK_STRNE(s1, s2) QCHECK_STROP(strcmp, !=, false, s1, s2)
#define QCHECK_STRCASEEQ(s1, s2) QCHECK_STROP(strcasecmp, ==, true, s1, s2)
#define QCHECK_STRCASENE(s1, s2) QCHECK_STROP(strcasecmp, !=, false, s1, s2)

#define QCHECK_INDEX(I,A) QCHECK(I < (sizeof(A)/sizeof(A[0])))
#define QCHECK_BOUND(B,A) QCHECK(B <= (sizeof(A)/sizeof(A[0])))

#define CHECK_DOUBLE_EQ(val1, val2)              \
  do {                                           \
    CHECK_LE((val1), (val2)+0.000000000000001L); \
    CHECK_GE((val1), (val2)-0.000000000000001L); \
  } while (0)

#define CHECK_NEAR(val1, val2, margin)           \
  do {                                           \
    CHECK_LE((val1), (val2)+(margin));           \
    CHECK_GE((val1), (val2)-(margin));           \
  } while (0)


#define PLOG(severity) GOOGLE_PLOG(severity, 0).stream()

#define GOOGLE_PLOG(severity, counter)  \
  ErrnoLogMessage(__FILE__, __LINE__, severity, counter, \
                  &LogMessage::SendToLog)

#define PLOG_IF(severity, condition) \
  !(condition) ? (void) 0 : LogMessageVoidify() & PLOG(severity)

#define PCHECK(condition)  \
      PLOG_IF(FATAL, PREDICT_FALSE(!(condition))) \
              << "Check failed: " #condition " "

#define CHECK_ERR(invocation)                                          \
PLOG_IF(FATAL, PREDICT_FALSE((invocation) == -1)) << #invocation

#define LOG_EVERY_N_VARNAME(base, line) LOG_EVERY_N_VARNAME_CONCAT(base, line)
#define LOG_EVERY_N_VARNAME_CONCAT(base, line) base ## line

#define LOG_OCCURRENCES LOG_EVERY_N_VARNAME(occurrences_, __LINE__)
#define LOG_OCCURRENCES_MOD_N LOG_EVERY_N_VARNAME(occurrences_mod_n_, __LINE__)

#define SOME_KIND_OF_LOG_EVERY_N(severity, n, what_to_do) \
  static int LOG_OCCURRENCES = 0, LOG_OCCURRENCES_MOD_N = 0; \
  ++LOG_OCCURRENCES; \
  if (++LOG_OCCURRENCES_MOD_N > n) LOG_OCCURRENCES_MOD_N -= n; \
  if (LOG_OCCURRENCES_MOD_N == 1) \
    LogMessage(__FILE__, __LINE__, severity, LOG_OCCURRENCES, \
               &what_to_do).stream()

#define SOME_KIND_OF_LOG_IF_EVERY_N(severity, condition, n, what_to_do) \
  static int LOG_OCCURRENCES = 0, LOG_OCCURRENCES_MOD_N = 0; \
  ANNOTATE_BENIGN_RACE(&LOG_OCCURRENCES, "logging"); \
  ANNOTATE_BENIGN_RACE(&LOG_OCCURRENCES_MOD_N, "logging"); \
  ++LOG_OCCURRENCES; \
  if (condition && \
      ((LOG_OCCURRENCES_MOD_N=(LOG_OCCURRENCES_MOD_N + 1) % n) == (1 % n))) \
    LogMessage(__FILE__, __LINE__, severity, LOG_OCCURRENCES, \
                 &what_to_do).stream()

#define SOME_KIND_OF_PLOG_EVERY_N(severity, n, what_to_do) \
  static int LOG_OCCURRENCES = 0, LOG_OCCURRENCES_MOD_N = 0; \
  ANNOTATE_BENIGN_RACE(&LOG_OCCURRENCES, "logging"); \
  ANNOTATE_BENIGN_RACE(&LOG_OCCURRENCES_MOD_N, "logging"); \
  ++LOG_OCCURRENCES; \
  if (++LOG_OCCURRENCES_MOD_N > n) LOG_OCCURRENCES_MOD_N -= n; \
  if (LOG_OCCURRENCES_MOD_N == 1) \
    ErrnoLogMessage(__FILE__, __LINE__, severity, LOG_OCCURRENCES, \
                    &what_to_do).stream()

#define SOME_KIND_OF_LOG_FIRST_N(severity, n, what_to_do) \
  static int LOG_OCCURRENCES = 0; \
  ANNOTATE_BENIGN_RACE(&LOG_OCCURRENCES, "logging"); \
  if (LOG_OCCURRENCES <= n) \
    ++LOG_OCCURRENCES; \
  if (LOG_OCCURRENCES <= n) \
    LogMessage(__FILE__, __LINE__, severity, LOG_OCCURRENCES, \
               &what_to_do).stream()

#define LOG_EVERY_N(severity, n) \
  COMPILE_ASSERT(severity < NUM_SEVERITIES, \
                 INVALID_REQUESTED_LOG_SEVERITY); \
  SOME_KIND_OF_LOG_EVERY_N(severity, (n), LogMessage::SendToLog)

#define SYSLOG_EVERY_N(severity, n) \
  SOME_KIND_OF_LOG_EVERY_N(severity, (n), LogMessage::SendToSyslogAndLog)

#define PLOG_EVERY_N(severity, n) \
  SOME_KIND_OF_PLOG_EVERY_N(severity, (n), LogMessage::SendToLog)

#define LOG_FIRST_N(severity, n) \
  SOME_KIND_OF_LOG_FIRST_N(severity, (n), LogMessage::SendToLog)

#define LOG_IF_EVERY_N(severity, condition, n) \
  SOME_KIND_OF_LOG_IF_EVERY_N(severity, (condition), (n), LogMessage::SendToLog)

enum PRIVATE_Counter {COUNTER};



#ifndef NDEBUG

#define DLOG(severity) LOG(severity)
#define DVLOG(verboselevel) VLOG(verboselevel)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#define DLOG_EVERY_N(severity, n) LOG_EVERY_N(severity, n)
#define DLOG_IF_EVERY_N(severity, condition, n) \
  LOG_IF_EVERY_N(severity, condition, n)
#define DLOG_ASSERT(condition) LOG_ASSERT(condition)

#define DCHECK(condition) CHECK(condition)
#define DCHECK_EQ(val1, val2) CHECK_EQ(val1, val2)
#define DCHECK_NE(val1, val2) CHECK_NE(val1, val2)
#define DCHECK_LE(val1, val2) CHECK_LE(val1, val2)
#define DCHECK_LT(val1, val2) CHECK_LT(val1, val2)
#define DCHECK_GE(val1, val2) CHECK_GE(val1, val2)
#define DCHECK_GT(val1, val2) CHECK_GT(val1, val2)
#define DCHECK_STREQ(str1, str2) CHECK_STREQ(str1, str2)
#define DCHECK_STRCASEEQ(str1, str2) CHECK_STRCASEEQ(str1, str2)
#define DCHECK_STRNE(str1, str2) CHECK_STRNE(str1, str2)
#define DCHECK_STRCASENE(str1, str2) CHECK_STRCASENE(str1, str2)

#else  

#define DLOG(severity) \
  true ? (void) 0 : LogMessageVoidify() & LOG(severity)

#define DVLOG(verboselevel) \
  (true || !VLOG_IS_ON(verboselevel)) ?\
    (void) 0 : LogMessageVoidify() & LOG(INFO)

#define DLOG_IF(severity, condition) \
  (true || !(condition)) ? (void) 0 : LogMessageVoidify() & LOG(severity)

#define DLOG_EVERY_N(severity, n) \
  true ? (void) 0 : LogMessageVoidify() & LOG(severity)

#define DLOG_IF_EVERY_N(severity, condition, n) \
  (true || !(condition))? (void) 0 : LogMessageVoidify() & LOG(severity)

#define DLOG_ASSERT(condition) \
  true ? (void) 0 : LOG_ASSERT(condition)

#define DCHECK(condition) \
  while (false) \
    CHECK(condition)

#define DCHECK_EQ(val1, val2) \
  while (false) \
    CHECK_EQ(val1, val2)

#define DCHECK_NE(val1, val2) \
  while (false) \
    CHECK_NE(val1, val2)

#define DCHECK_LE(val1, val2) \
  while (false) \
    CHECK_LE(val1, val2)

#define DCHECK_LT(val1, val2) \
  while (false) \
    CHECK_LT(val1, val2)

#define DCHECK_GE(val1, val2) \
  while (false) \
    CHECK_GE(val1, val2)

#define DCHECK_GT(val1, val2) \
  while (false) \
    CHECK_GT(val1, val2)

#define DCHECK_STREQ(str1, str2) \
  while (false) \
    CHECK_STREQ(str1, str2)

#define DCHECK_STRCASEEQ(str1, str2) \
  while (false) \
    CHECK_STRCASEEQ(str1, str2)

#define DCHECK_STRNE(str1, str2) \
  while (false) \
    CHECK_STRNE(str1, str2)

#define DCHECK_STRCASENE(str1, str2) \
  while (false) \
    CHECK_STRCASENE(str1, str2)


#endif  


#define VLOG(verboselevel) LOG_IF(INFO, VLOG_IS_ON(verboselevel))

#define VLOG_IF(verboselevel, condition) \
  LOG_IF(INFO, (condition) && VLOG_IS_ON(verboselevel))

#define VLOG_EVERY_N(verboselevel, n) \
  LOG_IF_EVERY_N(INFO, VLOG_IS_ON(verboselevel), n)

#define VLOG_IF_EVERY_N(verboselevel, condition, n) \
  LOG_IF_EVERY_N(INFO, (condition) && VLOG_IS_ON(verboselevel), n)


#define MLOG(verboselevel) LOG_IF(INFO, MODULE_FLAG >= (verboselevel))

#undef assert
#define assert(x) DLOG_ASSERT(x)

class LogMessage {
public:
  enum {
    
    
    
    
    
    kNoLogPrefix = -1
  };

  class LogStream : public ostrstream {
  public:
    LogStream(char *buf, int len, int ctr)
      : ostrstream(buf, len),
        ctr_(ctr) {
      self_ = this;
    }

    int ctr() const { return ctr_; }
    void set_ctr(int ctr) { ctr_ = ctr; }
    LogStream* self() const { return self_; }

  private:
    int ctr_;  
    LogStream *self_;  
  };

public:
  
  typedef void (LogMessage::*SendMethod)();

  LogMessage(const char* file, int line, LogSeverity severity, int ctr,
             SendMethod send_method);

  
  

  
  
  
  
  
  LogMessage(const char* file, int line);

  
  
  
  
  
  LogMessage(const char* file, int line, LogSeverity severity);

  
  
  
  LogMessage(const char* file, int line, LogSeverity severity, LogSink* sink,
             bool also_send_to_log);

  
  
  
  LogMessage(const char* file, int line, LogSeverity severity,
             vector<string>* outvec);

  
  
  
  LogMessage(const char* file, int line, LogSeverity severity,
             string* message);

  
  LogMessage(const char* file, int line, const CheckOpString& result);

  ~LogMessage();

  
  
  
  void Flush();

  
  
  static const size_t kMaxLogMessageLen;

  
  
  void SendToLog();  
  void SendToSyslogAndLog();  

  
  
  static void Fail() ATTRIBUTE_NORETURN;

  
  
  // written the trace as appropriate.
  static void FailWithoutStackTrace() ATTRIBUTE_NORETURN;

  
  
  static void FailQuietly() ATTRIBUTE_NORETURN;

  ostream& stream() { return *(data_->stream_); }

  int preserved_errno() const { return data_->preserved_errno_; }

  
  static int64 num_messages(int severity);

private:
  
  void SendToSinkAndLog();  
  void SendToSink();  

  
  void WriteToStringAndLog();

  void SaveOrSendToLog();  

  void Init(const char* file, int line, LogSeverity severity,
            void (LogMessage::*send_method)());

  
  void RecordCrashReason(base::CrashReason* reason);

  
  static int64 num_messages_[NUM_SEVERITIES];  

  
  
  struct LogMessageData {
    LogMessageData() {};

    int preserved_errno_;         
    scoped_array<char> buf_;      
    char* message_text_;          
    scoped_ptr<LogStream> stream_alloc_;
    LogStream* stream_;
    char severity_;               
    int line_;                    
    void (LogMessage::*send_method_)();  
    union {  
      LogSink* sink_;             
      vector<string>* outvec_;    
      string* message_;           
    };
    time_t timestamp_;            
    struct tm tm_time_;           
    size_t num_prefix_chars_;     
    size_t num_chars_to_log_;     
    size_t num_chars_to_syslog_;  
    const char* basename_;        
    const char* fullname_;        
    bool has_been_flushed_;       
    bool first_fatal_;            

   private:
    DISALLOW_EVIL_CONSTRUCTORS(LogMessageData);
  };

  static LogMessageData fatal_msg_data_exclusive_;
  static LogMessageData fatal_msg_data_shared_;

  scoped_ptr<LogMessageData> allocated_;
  LogMessageData* data_;

  friend class LogDestination;

  DISALLOW_EVIL_CONSTRUCTORS(LogMessage);

protected:
  
  
  
  bool fail_quietly_;
};

class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line);
  LogMessageFatal(const char* file, int line, const CheckOpString& result);
  ~LogMessageFatal() ATTRIBUTE_NORETURN;
};

class LogMessageQuietlyFatal : public LogMessage {
 public:
  LogMessageQuietlyFatal(const char* file, int line);
  LogMessageQuietlyFatal(const char* file, int line,
                         const CheckOpString& result);
  ~LogMessageQuietlyFatal() ATTRIBUTE_NORETURN;
};

inline void LogAtLevel(int const severity, string const &msg) {
  LogMessage(__FILE__, __LINE__, severity).stream() << msg;
}

#define LOG_AT_LEVEL(severity) LogMessage(__FILE__, __LINE__, severity).stream()

template <typename T>
T* CheckNotNull(const char *file, int line, const char *names, T* t) {
  return CheckNotNullCommon(file, line, names, t);
}

template <typename T>
T& CheckNotNull(const char *file, int line, const char *names, T& t) {
  return CheckNotNullCommon(file, line, names, t);
}

template <typename T>
T& CheckNotNullCommon(const char *file, int line, const char *names, T& t) {
  if (t == NULL) {
    LogMessageFatal(file, line, new string(names));
  }
  return t;
}

ostream& operator<<(ostream &os, const PRIVATE_Counter&);


#ifndef NDEBUG
inline ostream& operator<<(ostream& o, const DocId& d) {
  return (o << DocidForPrintf(d));
}

inline ostream& operator<<(ostream& o, const DocId32Bit& d) {
  return (o << Docid32BitForPrintf(d));
}
#endif  


class ErrnoLogMessage : public LogMessage {
 public:

  ErrnoLogMessage(const char* file, int line, LogSeverity severity, int ctr,
                  void (LogMessage::*send_method)());

  
  ~ErrnoLogMessage();

 private:

  DISALLOW_EVIL_CONSTRUCTORS(ErrnoLogMessage);
};



class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  
  
  void operator&(ostream&) { }
};


void FlushLogFiles(LogSeverity min_severity);

void FlushLogFilesUnsafe(LogSeverity min_severity);

void SetLogDestination(LogSeverity severity, const char* base_filename);

void SetLogSymlink(LogSeverity severity, const char* symlink_basename);

class LogSink {
 public:
  virtual ~LogSink();

  
  
  
  virtual void send(LogSeverity severity, const char* full_filename,
                    const char* base_filename, int line,
                    const struct tm* tm_time,
                    const char* message, size_t message_len) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void WaitTillSent();

  
  
  static string ToString(LogSeverity severity, const char* file, int line,
                         const struct tm* tm_time,
                         const char* message, size_t message_len);
};

void AddLogSink(LogSink *destination);
void RemoveLogSink(LogSink *destination);

void SetLogFilenameExtension(const char* filename_extension);

void SetStderrLogging(LogSeverity min_severity);

void LogToStderr();

void SetEmailLogging(LogSeverity min_severity, const char* addresses);

void StatusMessage(int64 done, int64 total);

void GWQStatusMessage(const char* msg);

bool SendEmail(const char*dest, const char *subject, const char*body);

const vector<string>& GetLoggingDirectories();

void TestOnly_ClearLoggingDirectoriesList();

void GetExistingTempDirectories(vector<string>* list);

void ReprintFatalMessage();

void TruncateLogFile(const char *path, int64 limit, int64 keep);

void TruncateStdoutStderr();

const char* GetLogSeverityName(LogSeverity severity);



namespace base {

class Logger {
 public:
  virtual ~Logger();

  
  
  
  
  
  
  
  
  virtual void Write(bool force_flush,
                     time_t timestamp,
                     const char* message,
                     int message_len) = 0;

  
  virtual void Flush() = 0;

  
  
  
  virtual uint32 LogSize() = 0;
};

extern Logger* GetLogger(LogSeverity level);

extern void SetLogger(LogSeverity level, Logger* logger);

}

int posix_strerror_r(int err, char *buf, size_t len);


class NullStream : public LogMessage::LogStream {
 public:
  // Initialize the LogStream so the messages can be written somewhere
  
  
  
  NullStream() : LogMessage::LogStream(message_buffer_, 1, 0) { }
  NullStream(const char* , int ,
             const CheckOpString& ) :
      LogMessage::LogStream(message_buffer_, 1, 0) { }
  NullStream &stream() { return *this; }
 private:
  
  
  
  char message_buffer_[2];
};

template<class T>
inline NullStream& operator<<(NullStream &str, const T &value) { return str; }

class NullStreamFatal : public NullStream {
 public:
  NullStreamFatal() { }
  NullStreamFatal(const char* file, int line, const CheckOpString& result) :
      NullStream(file, line, result) { }
  ~NullStreamFatal() ATTRIBUTE_NORETURN { _exit(1); }
};

#endif 
