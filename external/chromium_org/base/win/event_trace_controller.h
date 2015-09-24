// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef BASE_WIN_EVENT_TRACE_CONTROLLER_H_
#define BASE_WIN_EVENT_TRACE_CONTROLLER_H_

#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>
#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace win {

class BASE_EXPORT EtwTraceProperties {
 public:
  EtwTraceProperties();

  EVENT_TRACE_PROPERTIES* get() {
    return &properties_;
  }

  const EVENT_TRACE_PROPERTIES* get() const {
    return reinterpret_cast<const EVENT_TRACE_PROPERTIES*>(&properties_);
  }

  const wchar_t* GetLoggerName() const {
    return reinterpret_cast<const wchar_t *>(buffer_ + get()->LoggerNameOffset);
  }

  
  HRESULT SetLoggerName(const wchar_t* logger_name);
  const wchar_t* GetLoggerFileName() const {
    return reinterpret_cast<const wchar_t*>(buffer_ + get()->LogFileNameOffset);
  }

  
  HRESULT SetLoggerFileName(const wchar_t* logger_file_name);

  
  static const size_t kMaxStringLen = 1024;
  
  
  static const size_t kBufSize = sizeof(EVENT_TRACE_PROPERTIES)
      + 2 * sizeof(wchar_t) * (kMaxStringLen);

 private:
  
  
  
  union {
   public:
    
    EVENT_TRACE_PROPERTIES properties_;
    
    char buffer_[kBufSize];
  };

  DISALLOW_COPY_AND_ASSIGN(EtwTraceProperties);
};

class BASE_EXPORT EtwTraceController {
 public:
  EtwTraceController();
  ~EtwTraceController();

  
  HRESULT Start(const wchar_t* session_name, EtwTraceProperties* prop);

  
  HRESULT StartFileSession(const wchar_t* session_name,
                           const wchar_t* logfile_path,
                           bool realtime = false);

  
  HRESULT StartRealtimeSession(const wchar_t* session_name,
                               size_t buffer_size);

  
  
  
  HRESULT EnableProvider(const GUID& provider, UCHAR level,
                         ULONG flags = 0xFFFFFFFF);
  
  HRESULT DisableProvider(const GUID& provider);

  
  
  HRESULT Stop(EtwTraceProperties* properties);

  
  
  HRESULT Flush(EtwTraceProperties* properties);

  
  
  static HRESULT Start(const wchar_t* session_name,
                       EtwTraceProperties* properties,
                       TRACEHANDLE* session_handle);

  static HRESULT Query(const wchar_t* session_name,
                       EtwTraceProperties* properties);

  static HRESULT Update(const wchar_t* session_name,
                        EtwTraceProperties* properties);

  static HRESULT Stop(const wchar_t* session_name,
                      EtwTraceProperties* properties);
  static HRESULT Flush(const wchar_t* session_name,
                       EtwTraceProperties* properties);

  
  TRACEHANDLE session() const { return session_; }
  const wchar_t* session_name() const { return session_name_.c_str(); }

 private:
  std::wstring session_name_;
  TRACEHANDLE session_;

  DISALLOW_COPY_AND_ASSIGN(EtwTraceController);
};

}  
}  

#endif  
