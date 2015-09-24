// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_TRACE_EVENT_WIN_H_
#define BASE_DEBUG_TRACE_EVENT_WIN_H_

#include <string>

#include "base/base_export.h"
#include "base/debug/trace_event.h"
#include "base/win/event_trace_provider.h"

template <typename Type>
struct StaticMemorySingletonTraits;

namespace base {
namespace debug {

class BASE_EXPORT TraceEventETWProvider : public base::win::EtwTraceProvider {
 public:
  
  
  static bool StartTracing();

  
  
  
  
  
  
  
  static void Trace(const char* name,
                    size_t name_len,
                    char type,
                    const void* id,
                    const char* extra,
                    size_t extra_len);

  
  static void Trace(const char* name,
                    char type,
                    const void* id,
                    const std::string& extra) {
    return Trace(name, -1, type, id, extra.c_str(), extra.length());
  }

  
  
  static void Trace(const char* name,
                    char type,
                    const void* id,
                    const char* extra) {
    return Trace(name, -1, type, id, extra, -1);
  }

  
  
  static TraceEventETWProvider* GetInstance();

  
  bool IsTracing() {
    return enable_level() >= TRACE_LEVEL_INFORMATION;
  }

  
  
  
  
  
  void TraceEvent(const char* name,
                  size_t name_len,
                  char type,
                  const void* id,
                  const char* extra,
                  size_t extra_len);

  
  
  static void Resurrect();

 private:
  
  friend struct StaticMemorySingletonTraits<TraceEventETWProvider>;
  TraceEventETWProvider();

  DISALLOW_COPY_AND_ASSIGN(TraceEventETWProvider);
};

BASE_EXPORT extern const GUID kChromeTraceProviderName;

BASE_EXPORT extern const GUID kTraceEventClass32;

BASE_EXPORT extern const GUID kTraceEventClass64;

const base::win::EtwEventType kTraceEventTypeBegin = 0x10;
const base::win::EtwEventType kTraceEventTypeEnd = 0x11;
const base::win::EtwEventType kTraceEventTypeInstant = 0x12;

enum TraceEventETWFlags {
  CAPTURE_STACK_TRACE = 0x0001,
};


}  
}  

#endif  
