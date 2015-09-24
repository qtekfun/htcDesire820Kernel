// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_DEBUG_TRACE_EVENT_IMPL_H_
#define BASE_DEBUG_TRACE_EVENT_IMPL_H_

#include <stack>
#include <string>
#include <vector>

#include "base/atomicops.h"
#include "base/callback.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted_memory.h"
#include "base/memory/scoped_vector.h"
#include "base/observer_list.h"
#include "base/strings/string_util.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "base/threading/thread_local.h"
#include "base/timer/timer.h"

#define TRACE_EVENT_BEGIN_ETW(name, id, extra) \
    base::debug::TraceLog::AddTraceEventEtw( \
        TRACE_EVENT_PHASE_BEGIN, \
        name, reinterpret_cast<const void*>(id), extra)

#define TRACE_EVENT_END_ETW(name, id, extra) \
    base::debug::TraceLog::AddTraceEventEtw( \
        TRACE_EVENT_PHASE_END, \
        name, reinterpret_cast<const void*>(id), extra)

#define TRACE_EVENT_INSTANT_ETW(name, id, extra) \
    base::debug::TraceLog::AddTraceEventEtw( \
        TRACE_EVENT_PHASE_INSTANT, \
        name, reinterpret_cast<const void*>(id), extra)

template <typename Type>
struct DefaultSingletonTraits;

#if defined(COMPILER_GCC)
namespace BASE_HASH_NAMESPACE {
template <>
struct hash<base::MessageLoop*> {
  std::size_t operator()(base::MessageLoop* value) const {
    return reinterpret_cast<std::size_t>(value);
  }
};
}  
#endif

namespace base {

class WaitableEvent;
class MessageLoop;

namespace debug {

class ConvertableToTraceFormat : public RefCounted<ConvertableToTraceFormat> {
 public:
  
  
  
  
  virtual void AppendAsTraceFormat(std::string* out) const = 0;

 protected:
  virtual ~ConvertableToTraceFormat() {}

 private:
  friend class RefCounted<ConvertableToTraceFormat>;
};

struct TraceEventHandle {
  uint32 chunk_seq;
  uint16 chunk_index;
  uint16 event_index;
};

const int kTraceMaxNumArgs = 2;

class BASE_EXPORT TraceEvent {
 public:
  union TraceValue {
    bool as_bool;
    unsigned long long as_uint;
    long long as_int;
    double as_double;
    const void* as_pointer;
    const char* as_string;
  };

  TraceEvent();
  ~TraceEvent();

  
  
  void CopyFrom(const TraceEvent& other);

  void Initialize(
      int thread_id,
      TimeTicks timestamp,
      TimeTicks thread_timestamp,
      char phase,
      const unsigned char* category_group_enabled,
      const char* name,
      unsigned long long id,
      int num_args,
      const char** arg_names,
      const unsigned char* arg_types,
      const unsigned long long* arg_values,
      const scoped_refptr<ConvertableToTraceFormat>* convertable_values,
      unsigned char flags);

  void Reset();

  void UpdateDuration(const TimeTicks& now, const TimeTicks& thread_now);

  
  static void AppendEventsAsJSON(const std::vector<TraceEvent>& events,
                                 size_t start,
                                 size_t count,
                                 std::string* out);
  void AppendAsJSON(std::string* out) const;
  void AppendPrettyPrinted(std::ostringstream* out) const;

  static void AppendValueAsJSON(unsigned char type,
                                TraceValue value,
                                std::string* out);

  TimeTicks timestamp() const { return timestamp_; }
  TimeTicks thread_timestamp() const { return thread_timestamp_; }
  char phase() const { return phase_; }
  int thread_id() const { return thread_id_; }
  TimeDelta duration() const { return duration_; }
  TimeDelta thread_duration() const { return thread_duration_; }
  unsigned long long id() const { return id_; }
  unsigned char flags() const { return flags_; }

  

  const base::RefCountedString* parameter_copy_storage() const {
    return parameter_copy_storage_.get();
  }

  const unsigned char* category_group_enabled() const {
    return category_group_enabled_;
  }

  const char* name() const { return name_; }

#if defined(OS_ANDROID)
  void SendToATrace();
#endif

 private:
  
  TimeTicks timestamp_;
  TimeTicks thread_timestamp_;
  TimeDelta duration_;
  TimeDelta thread_duration_;
  
  unsigned long long id_;
  TraceValue arg_values_[kTraceMaxNumArgs];
  const char* arg_names_[kTraceMaxNumArgs];
  scoped_refptr<ConvertableToTraceFormat> convertable_values_[kTraceMaxNumArgs];
  const unsigned char* category_group_enabled_;
  const char* name_;
  scoped_refptr<base::RefCountedString> parameter_copy_storage_;
  int thread_id_;
  char phase_;
  unsigned char flags_;
  unsigned char arg_types_[kTraceMaxNumArgs];

  DISALLOW_COPY_AND_ASSIGN(TraceEvent);
};

class BASE_EXPORT TraceBufferChunk {
 public:
  TraceBufferChunk(uint32 seq)
      : next_free_(0),
        seq_(seq) {
  }

  void Reset(uint32 new_seq);
  TraceEvent* AddTraceEvent(size_t* event_index);
  bool IsFull() const { return next_free_ == kTraceBufferChunkSize; }

  uint32 seq() const { return seq_; }
  size_t capacity() const { return kTraceBufferChunkSize; }
  size_t size() const { return next_free_; }

  TraceEvent* GetEventAt(size_t index) {
    DCHECK(index < size());
    return &chunk_[index];
  }
  const TraceEvent* GetEventAt(size_t index) const {
    DCHECK(index < size());
    return &chunk_[index];
  }

  scoped_ptr<TraceBufferChunk> Clone() const;

  static const size_t kTraceBufferChunkSize = 64;

 private:
  size_t next_free_;
  TraceEvent chunk_[kTraceBufferChunkSize];
  uint32 seq_;
};

class BASE_EXPORT TraceBuffer {
 public:
  virtual ~TraceBuffer() {}

  virtual scoped_ptr<TraceBufferChunk> GetChunk(size_t *index) = 0;
  virtual void ReturnChunk(size_t index,
                           scoped_ptr<TraceBufferChunk> chunk) = 0;

  virtual bool IsFull() const = 0;
  virtual size_t Size() const = 0;
  virtual size_t Capacity() const = 0;
  virtual TraceEvent* GetEventByHandle(TraceEventHandle handle) = 0;

  
  virtual const TraceBufferChunk* NextChunk() = 0;

  virtual scoped_ptr<TraceBuffer> CloneForIteration() const = 0;
};

class BASE_EXPORT TraceResultBuffer {
 public:
  typedef base::Callback<void(const std::string&)> OutputCallback;

  
  
  
  struct BASE_EXPORT SimpleOutput {
    OutputCallback GetCallback();
    void Append(const std::string& json_string);

    
    
    std::string json_output;
  };

  TraceResultBuffer();
  ~TraceResultBuffer();

  
  
  
  
  void SetOutputCallback(const OutputCallback& json_chunk_callback);

  
  
  void Start();

  
  void AddFragment(const std::string& trace_fragment);

  
  
  void Finish();

 private:
  OutputCallback output_callback_;
  bool append_comma_;
};

class BASE_EXPORT CategoryFilter {
 public:
  
  
  
  static const char* kDefaultCategoryFilterString;

  
  
  
  
  
  
  
  
  
  
  explicit CategoryFilter(const std::string& filter_string);

  CategoryFilter(const CategoryFilter& cf);

  ~CategoryFilter();

  CategoryFilter& operator=(const CategoryFilter& rhs);

  
  
  
  
  
  std::string ToString() const;

  
  
  bool IsCategoryGroupEnabled(const char* category_group) const;

  
  void Merge(const CategoryFilter& nested_filter);

  
  
  
  
  
  
  void Clear();

 private:
  FRIEND_TEST_ALL_PREFIXES(TraceEventTestFixture, CategoryFilter);

  static bool IsEmptyOrContainsLeadingOrTrailingWhitespace(
      const std::string& str);

  typedef std::vector<std::string> StringList;

  void Initialize(const std::string& filter_string);
  void WriteString(const StringList& values,
                   std::string* out,
                   bool included) const;
  bool HasIncludedPatterns() const;

  bool DoesCategoryGroupContainCategory(const char* category_group,
                                        const char* category) const;

  StringList included_;
  StringList disabled_;
  StringList excluded_;
};

class TraceSamplingThread;

class BASE_EXPORT TraceLog {
 public:
  
  enum Options {
    
    RECORD_UNTIL_FULL = 1 << 0,

    
    
    RECORD_CONTINUOUSLY = 1 << 1,

    
    ENABLE_SAMPLING = 1 << 2,

    
    MONITOR_SAMPLING = 1 << 3,

    
    ECHO_TO_CONSOLE = 1 << 4,
  };

  
  
  
  enum CategoryGroupEnabledFlags {
    
    ENABLED_FOR_RECORDING = 1 << 0,
    
    ENABLED_FOR_EVENT_CALLBACK = 1 << 1,
  };

  static TraceLog* GetInstance();

  
  
  void GetKnownCategoryGroups(std::vector<std::string>* category_groups);

  
  CategoryFilter GetCurrentCategoryFilter();

  Options trace_options() const {
    return static_cast<Options>(subtle::NoBarrier_Load(&trace_options_));
  }

  
  
  
  
  void SetEnabled(const CategoryFilter& category_filter, Options options);

  
  void SetDisabled();

  bool IsEnabled() { return enabled_; }

  
  
  
  
  
  int GetNumTracesRecorded();

#if defined(OS_ANDROID)
  void StartATrace();
  void StopATrace();
  void AddClockSyncMetadataEvent();
#endif

  
  
  
  class EnabledStateObserver {
   public:
    
    
    virtual void OnTraceLogEnabled() = 0;

    
    
    virtual void OnTraceLogDisabled() = 0;
  };
  void AddEnabledStateObserver(EnabledStateObserver* listener);
  void RemoveEnabledStateObserver(EnabledStateObserver* listener);
  bool HasEnabledStateObserver(EnabledStateObserver* listener) const;

  float GetBufferPercentFull() const;
  bool BufferIsFull() const;

  
  
  
  
  
  
  
  
  
  typedef void (*EventCallback)(TimeTicks timestamp,
                                char phase,
                                const unsigned char* category_group_enabled,
                                const char* name,
                                unsigned long long id,
                                int num_args,
                                const char* const arg_names[],
                                const unsigned char arg_types[],
                                const unsigned long long arg_values[],
                                unsigned char flags);

  
  void SetEventCallbackEnabled(const CategoryFilter& category_filter,
                               EventCallback cb);
  void SetEventCallbackDisabled();

  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(const scoped_refptr<base::RefCountedString>&,
                              bool has_more_events)> OutputCallback;
  void Flush(const OutputCallback& cb);
  void FlushButLeaveBufferIntact(const OutputCallback& flush_output_callback);

  
  
  
  static const unsigned char* GetCategoryGroupEnabled(const char* name);
  static const char* GetCategoryGroupName(
      const unsigned char* category_group_enabled);

  
  
  
  TraceEventHandle AddTraceEvent(
      char phase,
      const unsigned char* category_group_enabled,
      const char* name,
      unsigned long long id,
      int num_args,
      const char** arg_names,
      const unsigned char* arg_types,
      const unsigned long long* arg_values,
      const scoped_refptr<ConvertableToTraceFormat>* convertable_values,
      unsigned char flags);
  TraceEventHandle AddTraceEventWithThreadIdAndTimestamp(
      char phase,
      const unsigned char* category_group_enabled,
      const char* name,
      unsigned long long id,
      int thread_id,
      const TimeTicks& timestamp,
      int num_args,
      const char** arg_names,
      const unsigned char* arg_types,
      const unsigned long long* arg_values,
      const scoped_refptr<ConvertableToTraceFormat>* convertable_values,
      unsigned char flags);
  static void AddTraceEventEtw(char phase,
                               const char* category_group,
                               const void* id,
                               const char* extra);
  static void AddTraceEventEtw(char phase,
                               const char* category_group,
                               const void* id,
                               const std::string& extra);

  void UpdateTraceEventDuration(const unsigned char* category_group_enabled,
                                const char* name,
                                TraceEventHandle handle);

  
  typedef base::Callback<void()> WatchEventCallback;
  void SetWatchEvent(const std::string& category_name,
                     const std::string& event_name,
                     const WatchEventCallback& callback);
  
  
  void CancelWatchEvent();

  int process_id() const { return process_id_; }

  

  void WaitSamplingEventForTesting();

  
  static void DeleteForTesting();

  
  size_t GetEventsSize() const { return logged_events_->Size(); }
  TraceEvent* GetEventByHandle(TraceEventHandle handle);

  void SetProcessID(int process_id);

  
  
  
  void SetProcessSortIndex(int sort_index);

  
  void SetProcessName(const std::string& process_name);

  
  
  void UpdateProcessLabel(int label_id, const std::string& current_label);
  void RemoveProcessLabel(int label_id);

  
  
  
  void SetThreadSortIndex(PlatformThreadId , int sort_index);

  
  
  void SetTimeOffset(TimeDelta offset);

  size_t GetObserverCountForTest() const;

  
  
  
  void SetCurrentThreadBlocksMessageLoop();

 private:
  FRIEND_TEST_ALL_PREFIXES(TraceEventTestFixture,
                           TraceBufferRingBufferGetReturnChunk);
  FRIEND_TEST_ALL_PREFIXES(TraceEventTestFixture,
                           TraceBufferRingBufferHalfIteration);
  FRIEND_TEST_ALL_PREFIXES(TraceEventTestFixture,
                           TraceBufferRingBufferFullIteration);

  
  
  friend struct DefaultSingletonTraits<TraceLog>;

  
  
  
  
  
  void UpdateCategoryGroupEnabledFlags();
  void UpdateCategoryGroupEnabledFlag(int category_index);

  class ThreadLocalEventBuffer;
  class OptionalAutoLock;

  TraceLog();
  ~TraceLog();
  const unsigned char* GetCategoryGroupEnabledInternal(const char* name);
  void AddMetadataEventsWhileLocked();

  TraceBuffer* trace_buffer() const { return logged_events_.get(); }
  TraceBuffer* CreateTraceBuffer();

  std::string EventToConsoleMessage(unsigned char phase,
                                    const TimeTicks& timestamp,
                                    TraceEvent* trace_event);

  TraceEvent* AddEventToThreadSharedChunkWhileLocked(TraceEventHandle* handle,
                                                     bool check_buffer_is_full);
  void CheckIfBufferIsFullWhileLocked();
  void SetDisabledWhileLocked();

  TraceEvent* GetEventByHandleInternal(TraceEventHandle handle,
                                       OptionalAutoLock* lock);

  
  
  void FlushCurrentThread(int generation);
  void ConvertTraceEventsToTraceFormat(scoped_ptr<TraceBuffer> logged_events,
      const TraceLog::OutputCallback& flush_output_callback);
  void FinishFlush(int generation);
  void OnFlushTimeout(int generation);

  int generation() const {
    return static_cast<int>(subtle::NoBarrier_Load(&generation_));
  }
  bool CheckGeneration(int generation) const {
    return generation == this->generation();
  }
  void UseNextTraceBuffer();

  TimeTicks OffsetNow() const {
    return OffsetTimestamp(TimeTicks::NowFromSystemTraceTime());
  }
  TimeTicks OffsetTimestamp(const TimeTicks& timestamp) const {
    return timestamp - time_offset_;
  }

  
  
  mutable Lock lock_;
  
  
  Lock thread_info_lock_;
  int locked_line_;
  bool enabled_;
  int num_traces_recorded_;
  scoped_ptr<TraceBuffer> logged_events_;
  subtle::AtomicWord  event_callback_;
  bool dispatching_to_observer_list_;
  std::vector<EnabledStateObserver*> enabled_state_observer_list_;

  std::string process_name_;
  base::hash_map<int, std::string> process_labels_;
  int process_sort_index_;
  base::hash_map<int, int> thread_sort_indices_;
  base::hash_map<int, std::string> thread_names_;

  
  base::hash_map<int, std::stack<TimeTicks> > thread_event_start_times_;
  base::hash_map<std::string, int> thread_colors_;

  
  unsigned long long process_id_hash_;

  int process_id_;

  TimeDelta time_offset_;

  
  WatchEventCallback watch_event_callback_;
  subtle::AtomicWord  watch_category_;
  std::string watch_event_name_;

  subtle::AtomicWord  trace_options_;

  
  scoped_ptr<TraceSamplingThread> sampling_thread_;
  PlatformThreadHandle sampling_thread_handle_;

  CategoryFilter category_filter_;
  CategoryFilter event_callback_category_filter_;

  ThreadLocalPointer<ThreadLocalEventBuffer> thread_local_event_buffer_;
  ThreadLocalBoolean thread_blocks_message_loop_;
  ThreadLocalBoolean thread_is_in_trace_event_;

  
  
  
  hash_set<MessageLoop*> thread_message_loops_;

  
  
  scoped_ptr<TraceBufferChunk> thread_shared_chunk_;
  size_t thread_shared_chunk_index_;

  
  OutputCallback flush_output_callback_;
  scoped_refptr<MessageLoopProxy> flush_message_loop_proxy_;
  subtle::AtomicWord generation_;

  DISALLOW_COPY_AND_ASSIGN(TraceLog);
};

}  
}  

#endif  
