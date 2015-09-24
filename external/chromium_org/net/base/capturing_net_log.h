// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CAPTURING_NET_LOG_H_
#define NET_BASE_CAPTURING_NET_LOG_H_

#include <string>
#include <vector>

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "net/base/net_log.h"

namespace base {
class DictionaryValue;
}

namespace net {

class CapturingNetLog : public NetLog {
 public:
  struct CapturedEntry {
    CapturedEntry(EventType type,
                  const base::TimeTicks& time,
                  Source source,
                  EventPhase phase,
                  scoped_ptr<base::DictionaryValue> params);
    
    
    CapturedEntry(const CapturedEntry& entry);

    ~CapturedEntry();

    
    
    CapturedEntry& operator=(const CapturedEntry& entry);

    
    
    
    bool GetStringValue(const std::string& name, std::string* value) const;
    bool GetIntegerValue(const std::string& name, int* value) const;

    
    
    bool GetNetErrorCode(int* value) const;

    
    
    std::string GetParamsJson() const;

    EventType type;
    base::TimeTicks time;
    Source source;
    EventPhase phase;
    scoped_ptr<base::DictionaryValue> params;
  };

  
  typedef std::vector<CapturedEntry> CapturedEntryList;

  CapturingNetLog();
  virtual ~CapturingNetLog();

  void SetLogLevel(LogLevel log_level);

  
  void GetEntries(CapturedEntryList* entry_list) const;
  void GetEntriesForSource(Source source, CapturedEntryList* entry_list) const;
  size_t GetSize() const;
  void Clear();

 private:
  
  
  
  class Observer : public NetLog::ThreadSafeObserver {
   public:
    Observer();
    virtual ~Observer();

    
    void GetEntries(CapturedEntryList* entry_list) const;

    
    void GetEntriesForSource(Source source,
                             CapturedEntryList* entry_list) const;

    
    size_t GetSize() const;

    void Clear();

   private:
    
    virtual void OnAddEntry(const Entry& entry) OVERRIDE;

    
    mutable base::Lock lock_;

    CapturedEntryList captured_entries_;

    DISALLOW_COPY_AND_ASSIGN(Observer);
  };

  Observer capturing_net_log_observer_;

  DISALLOW_COPY_AND_ASSIGN(CapturingNetLog);
};

class CapturingBoundNetLog {
 public:
  CapturingBoundNetLog();
  ~CapturingBoundNetLog();

  
  BoundNetLog bound() const { return net_log_; }

  
  void GetEntries(CapturingNetLog::CapturedEntryList* entry_list) const;

  
  void GetEntriesForSource(
      NetLog::Source source,
      CapturingNetLog::CapturedEntryList* entry_list) const;

  
  size_t GetSize() const;

  void Clear();

  
  void SetLogLevel(NetLog::LogLevel log_level);

 private:
  CapturingNetLog capturing_net_log_;
  const BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(CapturingBoundNetLog);
};

}  

#endif  
