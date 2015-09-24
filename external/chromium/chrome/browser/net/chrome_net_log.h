// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_CHROME_NET_LOG_H_
#define CHROME_BROWSER_NET_CHROME_NET_LOG_H_
#pragma once

#include <vector>

#include "base/atomicops.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "net/base/net_log.h"

class LoadTimingObserver;
class NetLogLogger;
class PassiveLogCollector;

class ChromeNetLog : public net::NetLog {
 public:
  
  
  
  struct Entry {
    Entry(uint32 order,
          net::NetLog::EventType type,
          const base::TimeTicks& time,
          net::NetLog::Source source,
          net::NetLog::EventPhase phase,
          net::NetLog::EventParameters* params);
    ~Entry();

    uint32 order;
    net::NetLog::EventType type;
    base::TimeTicks time;
    net::NetLog::Source source;
    net::NetLog::EventPhase phase;
    scoped_refptr<net::NetLog::EventParameters> params;
  };

  typedef std::vector<Entry> EntryList;

  
  class ThreadSafeObserver {
   public:
    
    
    
    
    
    
    
    
    
    
    
    
    explicit ThreadSafeObserver(LogLevel log_level);

    virtual ~ThreadSafeObserver();

    
    
    
    
    
    
    
    virtual void OnAddEntry(EventType type,
                            const base::TimeTicks& time,
                            const Source& source,
                            EventPhase phase,
                            EventParameters* params) = 0;
    LogLevel log_level() const;

   protected:
    void AssertNetLogLockAcquired() const;

    
    void SetLogLevel(LogLevel log_level);

    
    
    ChromeNetLog* net_log_;

   private:
    friend class ChromeNetLog;
    LogLevel log_level_;
    DISALLOW_COPY_AND_ASSIGN(ThreadSafeObserver);
  };

  ChromeNetLog();
  ~ChromeNetLog();

  
  virtual void AddEntry(EventType type,
                        const base::TimeTicks& time,
                        const Source& source,
                        EventPhase phase,
                        EventParameters* params);
  virtual uint32 NextID();
  virtual LogLevel GetLogLevel() const;

  void AddObserver(ThreadSafeObserver* observer);
  void RemoveObserver(ThreadSafeObserver* observer);

  
  
  
  
  void AddObserverAndGetAllPassivelyCapturedEvents(ThreadSafeObserver* observer,
                                                   EntryList* passive_entries);

  void GetAllPassivelyCapturedEvents(EntryList* passive_entries);

  void ClearAllPassivelyCapturedEvents();

  LoadTimingObserver* load_timing_observer() {
    return load_timing_observer_.get();
  }

 private:
  void AddObserverWhileLockHeld(ThreadSafeObserver* observer);

  
  
  void UpdateLogLevel_();

  
  
  base::Lock lock_;

  
  base::subtle::Atomic32 last_id_;

  base::subtle::Atomic32 log_level_;

  
  scoped_ptr<PassiveLogCollector> passive_collector_;

  scoped_ptr<LoadTimingObserver> load_timing_observer_;
  scoped_ptr<NetLogLogger> net_log_logger_;

  
  ObserverList<ThreadSafeObserver, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(ChromeNetLog);
};

#endif  
