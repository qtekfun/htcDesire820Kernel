// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef BASE_WIN_EVENT_TRACE_CONSUMER_H_
#define BASE_WIN_EVENT_TRACE_CONSUMER_H_

#include <windows.h>
#include <wmistr.h>
#include <evntrace.h>
#include <vector>
#include "base/basictypes.h"

namespace base {
namespace win {

template <class ImplClass>
class EtwTraceConsumerBase {
 public:
  
  EtwTraceConsumerBase() {
  }

  ~EtwTraceConsumerBase() {
    Close();
  }

  
  
  
  
  
  HRESULT OpenRealtimeSession(const wchar_t* session_name);

  
  
  
  
  HRESULT OpenFileSession(const wchar_t* file_name);

  
  HRESULT Consume();

  
  HRESULT Close();

 protected:
  
  static void ProcessEvent(EVENT_TRACE* event) {
  }
  
  static bool ProcessBuffer(EVENT_TRACE_LOGFILE* buffer) {
    return true;  
  }

 protected:
  
  std::vector<TRACEHANDLE> trace_handles_;

 private:
  
  static void WINAPI ProcessEventCallback(EVENT_TRACE* event) {
    ImplClass::ProcessEvent(event);
  }
  static ULONG WINAPI ProcessBufferCallback(PEVENT_TRACE_LOGFILE buffer) {
    return ImplClass::ProcessBuffer(buffer);
  }

  DISALLOW_COPY_AND_ASSIGN(EtwTraceConsumerBase);
};

template <class ImplClass> inline
HRESULT EtwTraceConsumerBase<ImplClass>::OpenRealtimeSession(
    const wchar_t* session_name) {
  EVENT_TRACE_LOGFILE logfile = {};
  logfile.LoggerName = const_cast<wchar_t*>(session_name);
  logfile.LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
  logfile.BufferCallback = &ProcessBufferCallback;
  logfile.EventCallback = &ProcessEventCallback;
  logfile.Context = this;
  TRACEHANDLE trace_handle = ::OpenTrace(&logfile);
  if (reinterpret_cast<TRACEHANDLE>(INVALID_HANDLE_VALUE) == trace_handle)
    return HRESULT_FROM_WIN32(::GetLastError());

  trace_handles_.push_back(trace_handle);
  return S_OK;
}

template <class ImplClass> inline
HRESULT EtwTraceConsumerBase<ImplClass>::OpenFileSession(
    const wchar_t* file_name) {
  EVENT_TRACE_LOGFILE logfile = {};
  logfile.LogFileName = const_cast<wchar_t*>(file_name);
  logfile.BufferCallback = &ProcessBufferCallback;
  logfile.EventCallback = &ProcessEventCallback;
  logfile.Context = this;
  TRACEHANDLE trace_handle = ::OpenTrace(&logfile);
  if (reinterpret_cast<TRACEHANDLE>(INVALID_HANDLE_VALUE) == trace_handle)
    return HRESULT_FROM_WIN32(::GetLastError());

  trace_handles_.push_back(trace_handle);
  return S_OK;
}

template <class ImplClass> inline
HRESULT EtwTraceConsumerBase<ImplClass>::Consume() {
  ULONG err = ::ProcessTrace(&trace_handles_[0],
                             trace_handles_.size(),
                             NULL,
                             NULL);
  return HRESULT_FROM_WIN32(err);
}

template <class ImplClass> inline
HRESULT EtwTraceConsumerBase<ImplClass>::Close() {
  HRESULT hr = S_OK;
  for (size_t i = 0; i < trace_handles_.size(); ++i) {
    if (NULL != trace_handles_[i]) {
      ULONG ret = ::CloseTrace(trace_handles_[i]);
      trace_handles_[i] = NULL;

      if (FAILED(HRESULT_FROM_WIN32(ret)))
        hr = HRESULT_FROM_WIN32(ret);
    }
  }
  trace_handles_.clear();

  return hr;
}

}  
}  

#endif  
