// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


// Setting the LoggingSeverity (and lower) that should be written to file should


#ifndef THIRD_PARTY_LIBJINGLE_OVERRIDES_TALK_BASE_LOGGING_H_
#define THIRD_PARTY_LIBJINGLE_OVERRIDES_TALK_BASE_LOGGING_H_

#include <sstream>
#include <string>

#include "base/logging.h"
#include "third_party/libjingle/source/talk/base/scoped_ref_ptr.h"

namespace talk_base {


struct ConstantLabel {
  int value;
  const char* label;
};
#define KLABEL(x) { x, #x }
#define LASTLABEL { 0, 0 }

const char* FindLabel(int value, const ConstantLabel entries[]);
std::string ErrorName(int err, const ConstantLabel* err_table);

// anything lower than or equal to the current verbosity level is written to
// severity numbers than or equal to the current severity level are written to
enum LoggingSeverity { LS_ERROR = 1,
                       LS_WARNING = 2,
                       LS_INFO = 3,
                       LS_VERBOSE = 4,
                       LS_SENSITIVE = 5,
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

class DiagnosticLogMessage {
 public:
  DiagnosticLogMessage(const char* file, int line, LoggingSeverity severity,
                       bool log_to_chrome, LogErrorContext err_ctx, int err);
  DiagnosticLogMessage(const char* file, int line, LoggingSeverity severity,
                       bool log_to_chrome, LogErrorContext err_ctx, int err,
                       const char* module);
  ~DiagnosticLogMessage();

  void CreateTimestamp();

  std::ostream& stream() { return print_stream_; }

 private:
  const char* file_name_;
  const int line_;
  const LoggingSeverity severity_;
  const bool log_to_chrome_;

  std::string extra_;

  std::ostringstream print_stream_;
  std::ostringstream print_stream_with_timestamp_;
};

class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  
  
  void operator&(std::ostream&) { }
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

void InitDiagnosticLoggingDelegateFunction(
    void (*delegate)(const std::string&));

void SetExtraLoggingInit(
    void (*function)(void (*delegate)(const std::string&)));
}  


#if defined(LOGGING_INSIDE_LIBJINGLE)

#define DIAGNOSTIC_LOG(sev, ctx, err, ...) \
  talk_base::DiagnosticLogMessage( \
      __FILE__, __LINE__, sev, VLOG_IS_ON(sev), \
      talk_base::ERRCTX_ ## ctx, err, ##__VA_ARGS__).stream()

#define LOG_CHECK_LEVEL(sev) VLOG_IS_ON(talk_base::sev)
#define LOG_CHECK_LEVEL_V(sev) VLOG_IS_ON(sev)

#define LOG_V(sev) DIAGNOSTIC_LOG(sev, NONE, 0)
#undef LOG
#define LOG(sev) DIAGNOSTIC_LOG(talk_base::sev, NONE, 0)

#if defined(__GNUC__) && defined(_DEBUG)
#define LOG_F(sev) LOG(sev) << __PRETTY_FUNCTION__ << ": "
#else
#define LOG_F(sev) LOG(sev) << __FUNCTION__ << ": "
#endif

#define LOG_E(sev, ctx, err, ...) \
  DIAGNOSTIC_LOG(talk_base::sev, ctx, err, ##__VA_ARGS__)

#undef LOG_ERRNO_EX
#define LOG_ERRNO_EX(sev, err) LOG_E(sev, ERRNO, err)
#undef LOG_ERRNO
#define LOG_ERRNO(sev) LOG_ERRNO_EX(sev, errno)

#if defined(OS_WIN)
#define LOG_GLE_EX(sev, err) LOG_E(sev, HRESULT, err)
#define LOG_GLE(sev) LOG_GLE_EX(sev, GetLastError())
#define LOG_GLEM(sev, mod) LOG_E(sev, HRESULT, GetLastError(), mod)
#define LOG_ERR_EX(sev, err) LOG_GLE_EX(sev, err)
#define LOG_ERR(sev) LOG_GLE(sev)
#define LAST_SYSTEM_ERROR (::GetLastError())
#else
#define LOG_ERR_EX(sev, err) LOG_ERRNO_EX(sev, err)
#define LOG_ERR(sev) LOG_ERRNO(sev)
#define LAST_SYSTEM_ERROR (errno)
#endif  

#undef PLOG
#define PLOG(sev, err) LOG_ERR_EX(sev, err)

#endif  

#endif  
