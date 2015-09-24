/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#ifndef TALK_BASE_LOGGING_H_
#define TALK_BASE_LOGGING_H_

#ifdef HAVE_CONFIG_H
#include "config.h"  
#endif

#include <list>
#include <sstream>
#include <string>
#include <utility>
#include "talk/base/basictypes.h"
#include "talk/base/criticalsection.h"

namespace talk_base {

class StreamInterface;


struct ConstantLabel { int value; const char * label; };
#define KLABEL(x) { x, #x }
#define TLABEL(x, y) { x, y }
#define LASTLABEL { 0, 0 }

const char * FindLabel(int value, const ConstantLabel entries[]);
std::string ErrorName(int err, const ConstantLabel* err_table);


enum LoggingSeverity { LS_SENSITIVE, LS_VERBOSE, LS_INFO, LS_WARNING, LS_ERROR,
                       INFO = LS_INFO,
                       WARNING = LS_WARNING,
                       LERROR = LS_ERROR };

enum LogErrorContext {
  ERRCTX_NONE,
  ERRCTX_ERRNO,     
  ERRCTX_HRESULT,   
  ERRCTX_OSSTATUS,  

  
  ERRCTX_EN = ERRCTX_ERRNO,     
  ERRCTX_HR = ERRCTX_HRESULT,   
  ERRCTX_OS = ERRCTX_OSSTATUS,  
};

class LogMessage {
 public:
  static const int NO_LOGGING;

  LogMessage(const char* file, int line, LoggingSeverity sev,
             LogErrorContext err_ctx = ERRCTX_NONE, int err = 0,
             const char* module = NULL);
  ~LogMessage();

  static inline bool Loggable(LoggingSeverity sev) { return (sev >= min_sev_); }
  std::ostream& stream() { return print_stream_; }

  
  
  static void LogContext(int min_sev);
  
  static void LogThreads(bool on = true);
  
  static void LogTimestamps(bool on = true);

  
  
  
  static void ResetTimestamps();

  
  
  static void LogToDebug(int min_sev);
  static int GetLogToDebug() { return dbg_sev_; }

  
  
  
  
  
  
  
  static void LogToStream(StreamInterface* stream, int min_sev);
  static int GetLogToStream(StreamInterface* stream = NULL);
  static void AddLogToStream(StreamInterface* stream, int min_sev);
  static void RemoveLogToStream(StreamInterface* stream);

  
  
  static int GetMinLogSeverity() { return min_sev_; }

  static void SetDiagnosticMode(bool f) { is_diagnostic_mode_ = f; }
  static bool IsDiagnosticMode() { return is_diagnostic_mode_; }

  
  
  
  static void ConfigureLogging(const char* params, const char* filename);

  
  static int ParseLogSeverity(const std::string& value);

 private:
  typedef std::list<std::pair<StreamInterface*, int> > StreamList;

  
  static void UpdateMinLogSeverity();

  
  static const char* Describe(LoggingSeverity sev);
  static const char* DescribeFile(const char* file);

  
  static void OutputToDebug(const std::string& msg, LoggingSeverity severity_);
  static void OutputToStream(StreamInterface* stream, const std::string& msg);

  
  std::ostringstream print_stream_;

  
  LoggingSeverity severity_;

  
  
  std::string extra_;

  
  static CriticalSection crit_;

  
  
  
  
  
  static int min_sev_, dbg_sev_, ctx_sev_;

  
  static StreamList streams_;

  
  static bool thread_, timestamp_;

  
  static uint32 start_;

  
  static bool is_diagnostic_mode_;

  DISALLOW_EVIL_CONSTRUCTORS(LogMessage);
};


class LogMultilineState {
 public:
  size_t unprintable_count_[2];
  LogMultilineState() {
    unprintable_count_[0] = unprintable_count_[1] = 0;
  }
};

void LogMultiline(LoggingSeverity level, const char* label, bool input,
                  const void* data, size_t len, bool hex_mode,
                  LogMultilineState* state);


#if !defined(LOGGING)
#if defined(_DEBUG) && !defined(NDEBUG)
#define LOGGING 1
#else
#define LOGGING 0
#endif
#endif  

#ifndef LOG
#if LOGGING



class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  
  
  void operator&(std::ostream&) { }
};

#define LOG_SEVERITY_PRECONDITION(sev) \
  !(talk_base::LogMessage::Loggable(sev)) \
    ? (void) 0 \
    : talk_base::LogMessageVoidify() &

#define LOG(sev) \
  LOG_SEVERITY_PRECONDITION(talk_base::sev) \
    talk_base::LogMessage(__FILE__, __LINE__, talk_base::sev).stream()

#define LOG_V(sev) \
  LOG_SEVERITY_PRECONDITION(sev) \
    talk_base::LogMessage(__FILE__, __LINE__, sev).stream()

#if defined(__GNUC__) && defined(_DEBUG)
#define LOG_F(sev) LOG(sev) << __PRETTY_FUNCTION__ << ": "
#else
#define LOG_F(sev) LOG(sev) << __FUNCTION__ << ": "
#endif

#define LOG_CHECK_LEVEL(sev) \
  talk_base::LogCheckLevel(talk_base::sev)
#define LOG_CHECK_LEVEL_V(sev) \
  talk_base::LogCheckLevel(sev)
inline bool LogCheckLevel(LoggingSeverity sev) {
  return (LogMessage::GetMinLogSeverity() <= sev);
}

#define LOG_E(sev, ctx, err, ...) \
  LOG_SEVERITY_PRECONDITION(talk_base::sev) \
    talk_base::LogMessage(__FILE__, __LINE__, talk_base::sev, \
                          talk_base::ERRCTX_ ## ctx, err , ##__VA_ARGS__) \
        .stream()

#else  

#define LOG(sev) \
  while (false)talk_base:: LogMessage(NULL, 0, talk_base::sev).stream()
#define LOG_V(sev) \
  while (false) talk_base::LogMessage(NULL, 0, sev).stream()
#define LOG_F(sev) LOG(sev) << __FUNCTION__ << ": "
#define LOG_CHECK_LEVEL(sev) \
  false
#define LOG_CHECK_LEVEL_V(sev) \
  false

#define LOG_E(sev, ctx, err, ...) \
  while (false) talk_base::LogMessage(__FILE__, __LINE__, talk_base::sev, \
                          talk_base::ERRCTX_ ## ctx, err , ##__VA_ARGS__) \
      .stream()

#endif  

#define LOG_ERRNO_EX(sev, err) \
  LOG_E(sev, ERRNO, err)
#define LOG_ERRNO(sev) \
  LOG_ERRNO_EX(sev, errno)

#ifdef WIN32
#define LOG_GLE_EX(sev, err) \
  LOG_E(sev, HRESULT, err)
#define LOG_GLE(sev) \
  LOG_GLE_EX(sev, GetLastError())
#define LOG_GLEM(sev, mod) \
  LOG_E(sev, HRESULT, GetLastError(), mod)
#define LOG_ERR_EX(sev, err) \
  LOG_GLE_EX(sev, err)
#define LOG_ERR(sev) \
  LOG_GLE(sev)
#define LAST_SYSTEM_ERROR \
  (::GetLastError())
#elif POSIX
#define LOG_ERR_EX(sev, err) \
  LOG_ERRNO_EX(sev, err)
#define LOG_ERR(sev) \
  LOG_ERRNO(sev)
#define LAST_SYSTEM_ERROR \
  (errno)
#endif  

#define PLOG(sev, err) \
  LOG_ERR_EX(sev, err)


#endif  

}  

#endif  
