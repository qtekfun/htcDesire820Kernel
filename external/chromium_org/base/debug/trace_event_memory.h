// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_DEBUG_TRACE_EVENT_MEMORY_H_
#define BASE_DEBUG_TRACE_EVENT_MEMORY_H_

#include "base/base_export.h"
#include "base/debug/trace_event_impl.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"

#if !defined(NO_TCMALLOC) && !defined(OS_NACL) && \
    (defined(OS_LINUX) || defined(OS_ANDROID))
#define TCMALLOC_TRACE_MEMORY_SUPPORTED 1
#endif

namespace base {

class MessageLoopProxy;

namespace debug {

class BASE_EXPORT TraceMemoryController
    : public TraceLog::EnabledStateObserver {
 public:
  typedef int (*StackGeneratorFunction)(int skip_count, void** stack);
  typedef void (*HeapProfilerStartFunction)(StackGeneratorFunction callback);
  typedef void (*HeapProfilerStopFunction)();
  typedef char* (*GetHeapProfileFunction)();

  
  
  
  
  TraceMemoryController(
      scoped_refptr<MessageLoopProxy> message_loop_proxy,
      HeapProfilerStartFunction heap_profiler_start_function,
      HeapProfilerStopFunction heap_profiler_stop_function,
      GetHeapProfileFunction get_heap_profile_function);
  virtual ~TraceMemoryController();

  
  virtual void OnTraceLogEnabled() OVERRIDE;
  virtual void OnTraceLogDisabled() OVERRIDE;

  
  void StartProfiling();

  
  void DumpMemoryProfile();

  
  void StopProfiling();

 private:
  FRIEND_TEST_ALL_PREFIXES(TraceMemoryTest, TraceMemoryController);

  bool IsTimerRunningForTest() const;

  
  scoped_refptr<MessageLoopProxy> message_loop_proxy_;

  
  
  HeapProfilerStartFunction heap_profiler_start_function_;
  HeapProfilerStopFunction heap_profiler_stop_function_;
  GetHeapProfileFunction get_heap_profile_function_;

  
  RepeatingTimer<TraceMemoryController> dump_timer_;

  WeakPtrFactory<TraceMemoryController> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(TraceMemoryController);
};


class BASE_EXPORT ScopedTraceMemory {
 public:
  struct ScopeData {
    const char* category;
    const char* name;
  };

  
  
  ScopedTraceMemory(const char* category, const char* name) {
    if (!enabled_)
      return;
    Initialize(category, name);
  }
  ~ScopedTraceMemory() {
    if (!enabled_)
      return;
    Destroy();
  }

  
  static void set_enabled(bool enabled) { enabled_ = enabled; }

  
  static void InitForTest();
  static void CleanupForTest();
  static int GetStackDepthForTest();
  static ScopeData GetScopeDataForTest(int stack_index);

 private:
  void Initialize(const char* category, const char* name);
  void Destroy();

  static bool enabled_;
  DISALLOW_COPY_AND_ASSIGN(ScopedTraceMemory);
};


BASE_EXPORT void AppendHeapProfileAsTraceFormat(const char* input,
                                                std::string* output);

BASE_EXPORT void AppendHeapProfileTotalsAsTraceFormat(const std::string& line,
                                                      std::string* output);

BASE_EXPORT bool AppendHeapProfileLineAsTraceFormat(const std::string& line,
                                                    std::string* output);

BASE_EXPORT const char* StringFromHexAddress(const std::string& hex_address);

}  
}  

#define INTERNAL_TRACE_MEMORY_ID3(line) trace_memory_unique_##line
#define INTERNAL_TRACE_MEMORY_ID2(line) INTERNAL_TRACE_MEMORY_ID3(line)
#define INTERNAL_TRACE_MEMORY_ID INTERNAL_TRACE_MEMORY_ID2(__LINE__)

#if defined(TCMALLOC_TRACE_MEMORY_SUPPORTED)
#define INTERNAL_TRACE_MEMORY(category, name) \
  base::debug::ScopedTraceMemory INTERNAL_TRACE_MEMORY_ID(category, name);
#else
#define INTERNAL_TRACE_MEMORY(category, name)
#endif  

#define TRACE_MEMORY_IGNORE "trace-memory-ignore"

#endif  
