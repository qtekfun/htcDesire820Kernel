// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_DEBUG_TRACE_EVENT_H_
#define BASE_DEBUG_TRACE_EVENT_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include "base/debug/trace_event_win.h"
#else  

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "base/timer.h"

#ifndef CHROMIUM_ENABLE_TRACE_EVENT
#define TRACE_EVENT_BEGIN(name, id, extra) ((void) 0)
#define TRACE_EVENT_END(name, id, extra) ((void) 0)
#define TRACE_EVENT_INSTANT(name, id, extra) ((void) 0)

#else  

#define TRACE_EVENT_BEGIN(name, id, extra) \
  base::debug::TraceLog::GetInstance()->Trace( \
      name, \
      base::debug::TraceLog::EVENT_BEGIN, \
      reinterpret_cast<const void*>(id), \
      extra, \
      __FILE__, \
      __LINE__)

#define TRACE_EVENT_END(name, id, extra) \
  base::debug::TraceLog::GetInstance()->Trace( \
      name, \
      base::debug::TraceLog::EVENT_END, \
      reinterpret_cast<const void*>(id), \
      extra, \
      __FILE__, \
      __LINE__)

#define TRACE_EVENT_INSTANT(name, id, extra) \
  base::debug::TraceLog::GetInstance()->Trace( \
      name, \
      base::debug::TraceLog::EVENT_INSTANT, \
      reinterpret_cast<const void*>(id), \
      extra, \
      __FILE__, \
      __LINE__)
#endif  

namespace base {

class ProcessMetrics;

namespace debug {

class TraceLog {
 public:
  enum EventType {
    EVENT_BEGIN,
    EVENT_END,
    EVENT_INSTANT
  };

  static TraceLog* GetInstance();

  
  static bool IsTracing();
  
  static bool StartTracing();
  
  static void StopTracing();

  
  void Trace(const std::string& name,
             EventType type,
             const void* id,
             const std::wstring& extra,
             const char* file,
             int line);
  void Trace(const std::string& name,
             EventType type,
             const void* id,
             const std::string& extra,
             const char* file,
             int line);

 private:
  
  
  friend struct DefaultSingletonTraits<TraceLog>;

  TraceLog();
  ~TraceLog();
  bool OpenLogFile();
  void CloseLogFile();
  bool Start();
  void Stop();
  void Heartbeat();
  void Log(const std::string& msg);

  bool enabled_;
  FILE* log_file_;
  base::Lock file_lock_;
  TimeTicks trace_start_time_;
#ifndef ANDROID
  scoped_ptr<base::ProcessMetrics> process_metrics_;
#endif
  RepeatingTimer<TraceLog> timer_;
};

}  
}  

#endif  

#endif  
