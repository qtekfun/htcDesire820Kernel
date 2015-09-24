// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_LOG_H_
#define NET_BASE_NET_LOG_H_

#include <string>

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace net {

class NET_EXPORT NetLog {
 public:
  enum EventType {
#define EVENT_TYPE(label) TYPE_ ## label,
#include "net/base/net_log_event_type_list.h"
#undef EVENT_TYPE
    EVENT_COUNT
  };

  
  
  enum EventPhase {
    PHASE_NONE,
    PHASE_BEGIN,
    PHASE_END,
  };

  
  enum SourceType {
#define SOURCE_TYPE(label) SOURCE_ ## label,
#include "net/base/net_log_source_type_list.h"
#undef SOURCE_TYPE
    SOURCE_COUNT
  };

  
  
  
  
  
  enum LogLevel {
    
    
    LOG_ALL,

    
    
    LOG_ALL_BUT_BYTES,

    
    
    LOG_BASIC,

    
    LOG_NONE,
  };

  
  
  
  
  typedef base::Callback<base::Value*(LogLevel)> ParametersCallback;

  
  
  
  struct NET_EXPORT Source {
    static const uint32 kInvalidId;

    Source();
    Source(SourceType type, uint32 id);
    bool IsValid() const;

    
    
    void AddToEventParameters(base::DictionaryValue* event_params) const;

    
    
    ParametersCallback ToEventParametersCallback() const;

    
    
    
    
    static bool FromEventParameters(base::Value* event_params, Source* source);

    SourceType type;
    uint32 id;
  };

  class NET_EXPORT Entry {
   public:
    Entry(EventType type,
          Source source,
          EventPhase phase,
          base::TimeTicks time,
          const ParametersCallback* parameters_callback,
          LogLevel log_level);
    ~Entry();

    EventType type() const { return type_; }
    Source source() const { return source_; }
    EventPhase phase() const { return phase_; }

    
    
    
    base::Value* ToValue() const;

    
    
    base::Value* ParametersToValue() const;

   private:
    const EventType type_;
    const Source source_;
    const EventPhase phase_;
    const base::TimeTicks time_;
    const ParametersCallback* parameters_callback_;

    
    const LogLevel log_level_;

    
    
    
    DISALLOW_COPY_AND_ASSIGN(Entry);
  };

  
  
  class NET_EXPORT ThreadSafeObserver {
   public:
    
    
    
    
    
    
    
    
    
    ThreadSafeObserver();

    
    
    LogLevel log_level() const;

    
    
    NetLog* net_log() const;

    
    
    
    
    
    
    virtual void OnAddEntry(const Entry& entry) = 0;

   protected:
    virtual ~ThreadSafeObserver();

   private:
    friend class NetLog;

    
    LogLevel log_level_;
    NetLog* net_log_;

    DISALLOW_COPY_AND_ASSIGN(ThreadSafeObserver);
  };

  NetLog();
  virtual ~NetLog();

  
  void AddGlobalEntry(EventType type);
  void AddGlobalEntry(EventType type,
                      const NetLog::ParametersCallback& parameters_callback);

  
  
  uint32 NextID();

  
  
  LogLevel GetLogLevel() const;

  
  
  
  
  
  
  
  
  
  
  void AddThreadSafeObserver(ThreadSafeObserver* observer, LogLevel log_level);

  
  
  
  void SetObserverLogLevel(ThreadSafeObserver* observer, LogLevel log_level);

  
  
  
  
  
  void RemoveThreadSafeObserver(ThreadSafeObserver* observer);

  
  
  static std::string TickCountToString(const base::TimeTicks& time);

  
  static const char* EventTypeToString(EventType event_type);

  
  
  static base::Value* GetEventTypesAsValue();

  
  static const char* SourceTypeToString(SourceType source_type);

  
  
  static base::Value* GetSourceTypesAsValue();

  
  static const char* EventPhaseToString(EventPhase event_phase);

  
  
  static bool IsLoggingBytes(LogLevel log_level);

  
  
  
  static bool IsLoggingAllEvents(LogLevel log_level);

  
  
  
  static ParametersCallback IntegerCallback(const char* name, int value);

  
  
  
  
  static ParametersCallback Int64Callback(const char* name, int64 value);

  
  
  
  
  static ParametersCallback StringCallback(const char* name,
                                           const std::string* value);

  
  static ParametersCallback StringCallback(const char* name,
                                           const base::string16* value);

 protected:
  
  void SetBaseLogLevel(LogLevel log_level);

 private:
  friend class BoundNetLog;

  void AddEntry(EventType type,
                const Source& source,
                EventPhase phase,
                const NetLog::ParametersCallback* parameters_callback);

  
  
  void UpdateLogLevel();

  
  base::Lock lock_;

  
  base::subtle::Atomic32 last_id_;

  
  
  LogLevel base_log_level_;

  
  base::subtle::Atomic32 effective_log_level_;

  
  ObserverList<ThreadSafeObserver, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(NetLog);
};

class NET_EXPORT BoundNetLog {
 public:
  BoundNetLog() : net_log_(NULL) {}

  
  void AddEntry(NetLog::EventType type, NetLog::EventPhase phase) const;
  void AddEntry(NetLog::EventType type,
                NetLog::EventPhase phase,
                const NetLog::ParametersCallback& get_parameters) const;

  
  
  void BeginEvent(NetLog::EventType type) const;
  void BeginEvent(NetLog::EventType type,
                  const NetLog::ParametersCallback& get_parameters) const;

  void EndEvent(NetLog::EventType type) const;
  void EndEvent(NetLog::EventType type,
                const NetLog::ParametersCallback& get_parameters) const;

  void AddEvent(NetLog::EventType type) const;
  void AddEvent(NetLog::EventType type,
                const NetLog::ParametersCallback& get_parameters) const;

  
  
  
  
  void AddEventWithNetErrorCode(NetLog::EventType event_type,
                                int net_error) const;

  
  
  
  
  void EndEventWithNetErrorCode(NetLog::EventType event_type,
                                int net_error) const;

  
  
  void AddByteTransferEvent(NetLog::EventType event_type,
                            int byte_count, const char* bytes) const;

  NetLog::LogLevel GetLogLevel() const;

  
  bool IsLoggingBytes() const;

  
  bool IsLoggingAllEvents() const;

  
  
  static BoundNetLog Make(NetLog* net_log, NetLog::SourceType source_type);

  const NetLog::Source& source() const { return source_; }
  NetLog* net_log() const { return net_log_; }

 private:
  BoundNetLog(const NetLog::Source& source, NetLog* net_log)
      : source_(source), net_log_(net_log) {
  }

  NetLog::Source source_;
  NetLog* net_log_;
};

}  

#endif  
