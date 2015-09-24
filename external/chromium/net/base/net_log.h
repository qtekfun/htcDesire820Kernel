// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NET_LOG_H_
#define NET_BASE_NET_LOG_H_
#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

class Value;

namespace base {
class TimeTicks;
}

namespace net {


class NetLog {
 public:
  enum EventType {
#define EVENT_TYPE(label) TYPE_ ## label,
#include "net/base/net_log_event_type_list.h"
#undef EVENT_TYPE
  };

  
  
  enum EventPhase {
    PHASE_NONE,
    PHASE_BEGIN,
    PHASE_END,
  };

  
  enum SourceType {
#define SOURCE_TYPE(label, value) SOURCE_ ## label = value,
#include "net/base/net_log_source_type_list.h"
#undef SOURCE_TYPE
  };

  
  
  
  struct Source {
    static const uint32 kInvalidId = 0;

    Source() : type(SOURCE_NONE), id(kInvalidId) {}
    Source(SourceType type, uint32 id) : type(type), id(id) {}
    bool is_valid() const { return id != kInvalidId; }

    
    Value* ToValue() const;

    SourceType type;
    uint32 id;
  };

  
  
  
  class EventParameters : public base::RefCountedThreadSafe<EventParameters> {
   public:
    EventParameters() {}
    virtual ~EventParameters() {}

    
    
    
    virtual Value* ToValue() const = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(EventParameters);
  };

  
  enum LogLevel {
    
    
    LOG_ALL,

    
    
    LOG_ALL_BUT_BYTES,

    
    LOG_BASIC,
  };

  NetLog() {}
  virtual ~NetLog() {}

  
  
  
  
  
  
  
  
  
  
  virtual void AddEntry(EventType type,
                        const base::TimeTicks& time,
                        const Source& source,
                        EventPhase phase,
                        EventParameters* params) = 0;

  
  virtual uint32 NextID() = 0;

  
  
  virtual LogLevel GetLogLevel() const = 0;

  
  
  static std::string TickCountToString(const base::TimeTicks& time);

  
  static const char* EventTypeToString(EventType event_type);

  
  static std::vector<EventType> GetAllEventTypes();

  
  static const char* SourceTypeToString(SourceType source_type);

  
  static const char* EventPhaseToString(EventPhase event_phase);

  
  
  static Value* EntryToDictionaryValue(NetLog::EventType type,
                                       const base::TimeTicks& time,
                                       const NetLog::Source& source,
                                       NetLog::EventPhase phase,
                                       NetLog::EventParameters* params,
                                       bool use_strings);

 private:
  DISALLOW_COPY_AND_ASSIGN(NetLog);
};

class BoundNetLog {
 public:
  BoundNetLog() : net_log_(NULL) {}

  BoundNetLog(const NetLog::Source& source, NetLog* net_log)
      : source_(source), net_log_(net_log) {
  }

  
  
  void AddEntry(NetLog::EventType type,
                NetLog::EventPhase phase,
                const scoped_refptr<NetLog::EventParameters>& params) const;

  void AddEntryWithTime(
      NetLog::EventType type,
      const base::TimeTicks& time,
      NetLog::EventPhase phase,
      const scoped_refptr<NetLog::EventParameters>& params) const;

  
  
  
  void AddEvent(NetLog::EventType event_type,
                const scoped_refptr<NetLog::EventParameters>& params) const;
  void BeginEvent(NetLog::EventType event_type,
                  const scoped_refptr<NetLog::EventParameters>& params) const;
  void EndEvent(NetLog::EventType event_type,
                const scoped_refptr<NetLog::EventParameters>& params) const;

  
  
  
  
  void EndEventWithNetErrorCode(NetLog::EventType event_type,
                                int net_error) const;

  NetLog::LogLevel GetLogLevel() const;

  
  bool IsLoggingBytes() const;

  
  bool IsLoggingAllEvents() const;

  
  
  static BoundNetLog Make(NetLog* net_log, NetLog::SourceType source_type);

  const NetLog::Source& source() const { return source_; }
  NetLog* net_log() const { return net_log_; }

 private:
  NetLog::Source source_;
  NetLog* net_log_;
};

class NetLogStringParameter : public NetLog::EventParameters {
 public:
  
  NetLogStringParameter(const char* name, const std::string& value);
  virtual ~NetLogStringParameter();

  const std::string& value() const {
    return value_;
  }

  virtual Value* ToValue() const;

 private:
  const char* const name_;
  const std::string value_;
};

class NetLogIntegerParameter : public NetLog::EventParameters {
 public:
  
  NetLogIntegerParameter(const char* name, int value)
      : name_(name), value_(value) {}

  int value() const {
    return value_;
  }

  virtual Value* ToValue() const;

 private:
  const char* name_;
  const int value_;
};

class NetLogSourceParameter : public NetLog::EventParameters {
 public:
  
  NetLogSourceParameter(const char* name, const NetLog::Source& value)
      : name_(name), value_(value) {}

  const NetLog::Source& value() const {
    return value_;
  }

  virtual Value* ToValue() const;

 private:
  const char* name_;
  const NetLog::Source value_;
};

class ScopedNetLogEvent {
 public:
  ScopedNetLogEvent(const BoundNetLog& net_log,
                    NetLog::EventType event_type,
                    const scoped_refptr<NetLog::EventParameters>& params);

  ~ScopedNetLogEvent();

  
  
  
  void SetEndEventParameters(
      const scoped_refptr<NetLog::EventParameters>& end_event_params);

  const BoundNetLog& net_log() const;

 private:
  BoundNetLog net_log_;
  const NetLog::EventType event_type_;
  scoped_refptr<NetLog::EventParameters> end_event_params_;
};

}  

#endif  
