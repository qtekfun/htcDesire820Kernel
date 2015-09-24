// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_TRACE_EVENT_WIN_H_
#define BASE_DEBUG_TRACE_EVENT_WIN_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/win/event_trace_provider.h"

#define TRACE_EVENT_BEGIN(name, id, extra) \
  base::debug::TraceLog::Trace( \
      name, \
      base::debug::TraceLog::EVENT_BEGIN, \
      reinterpret_cast<const void*>(id), \
      extra);

#define TRACE_EVENT_END(name, id, extra) \
  base::debug::TraceLog::Trace( \
      name, \
      base::debug::TraceLog::EVENT_END, \
      reinterpret_cast<const void*>(id), \
      extra);

#define TRACE_EVENT_INSTANT(name, id, extra) \
  base::debug::TraceLog::Trace( \
      name, \
      base::debug::TraceLog::EVENT_INSTANT, \
      reinterpret_cast<const void*>(id), \
      extra);

template <typename Type>
struct StaticMemorySingletonTraits;

namespace base {
namespace debug {

class BASE_API TraceLog : public base::win::EtwTraceProvider {
 public:
  enum EventType {
    EVENT_BEGIN,
    EVENT_END,
    EVENT_INSTANT
  };

  
  
  static bool StartTracing();

  
  
  
  
  
  
  
  static void Trace(const char* name,
                    size_t name_len,
                    EventType type,
                    const void* id,
                    const char* extra,
                    size_t extra_len);

  
  static void Trace(const char* name,
                    EventType type,
                    const void* id,
                    const std::string& extra) {
    return Trace(name, -1, type, id, extra.c_str(), extra.length());
  }

  
  
  static void Trace(const char* name,
                    EventType type,
                    const void* id,
                    const char* extra) {
    return Trace(name, -1, type, id, extra, -1);
  }

  
  
  static TraceLog* GetInstance();

  
  bool IsTracing() {
    return enable_level() >= TRACE_LEVEL_INFORMATION;
  }

  
  
  
  
  
  void TraceEvent(const char* name,
                  size_t name_len,
                  EventType type,
                  const void* id,
                  const char* extra,
                  size_t extra_len);

  
  
  static void Resurrect();

 private:
  
  friend struct StaticMemorySingletonTraits<TraceLog>;
  TraceLog();

  DISALLOW_COPY_AND_ASSIGN(TraceLog);
};

BASE_API extern const GUID kChromeTraceProviderName;

BASE_API extern const GUID kTraceEventClass32;

BASE_API extern const GUID kTraceEventClass64;

const base::win::EtwEventType kTraceEventTypeBegin = 0x10;
const base::win::EtwEventType kTraceEventTypeEnd = 0x11;
const base::win::EtwEventType kTraceEventTypeInstant = 0x12;

enum TraceEventFlags {
  CAPTURE_STACK_TRACE = 0x0001,
};


struct TraceLogSingletonTraits;

}  
}  

#endif  
