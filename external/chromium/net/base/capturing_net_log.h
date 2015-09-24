// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CAPTURING_NET_LOG_H_
#define NET_BASE_CAPTURING_NET_LOG_H_
#pragma once

#include <vector>

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "net/base/net_log.h"

namespace net {

class CapturingNetLog : public NetLog {
 public:
  struct Entry {
    Entry(EventType type,
          const base::TimeTicks& time,
          Source source,
          EventPhase phase,
          EventParameters* extra_parameters);
    ~Entry();

    EventType type;
    base::TimeTicks time;
    Source source;
    EventPhase phase;
    scoped_refptr<EventParameters> extra_parameters;
  };

  
  typedef std::vector<Entry> EntryList;

  enum { kUnbounded = -1 };

  
  
  explicit CapturingNetLog(size_t max_num_entries);
  virtual ~CapturingNetLog();

  
  void GetEntries(EntryList* entry_list) const;

  void Clear();

  void SetLogLevel(NetLog::LogLevel log_level);

  
  virtual void AddEntry(EventType type,
                        const base::TimeTicks& time,
                        const Source& source,
                        EventPhase phase,
                        EventParameters* extra_parameters);
  virtual uint32 NextID();
  virtual LogLevel GetLogLevel() const;

 private:
  
  mutable base::Lock lock_;

  
  base::subtle::Atomic32 last_id_;

  size_t max_num_entries_;
  EntryList entries_;

  NetLog::LogLevel log_level_;

  DISALLOW_COPY_AND_ASSIGN(CapturingNetLog);
};

class CapturingBoundNetLog {
 public:
  CapturingBoundNetLog(const NetLog::Source& source, CapturingNetLog* net_log);

  explicit CapturingBoundNetLog(size_t max_num_entries);

  ~CapturingBoundNetLog();

  
  BoundNetLog bound() const {
    return BoundNetLog(source_, capturing_net_log_.get());
  }

  
  void GetEntries(CapturingNetLog::EntryList* entry_list) const;

  void Clear();

  
  void SetLogLevel(NetLog::LogLevel log_level);

 private:
  NetLog::Source source_;
  scoped_ptr<CapturingNetLog> capturing_net_log_;

  DISALLOW_COPY_AND_ASSIGN(CapturingBoundNetLog);
};

}  

#endif  

