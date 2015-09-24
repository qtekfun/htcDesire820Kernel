// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_PRIORITIZED_DISPATCHER_H_
#define NET_BASE_PRIORITIZED_DISPATCHER_H_

#include <vector>

#include "net/base/net_export.h"
#include "net/base/priority_queue.h"

namespace net {

class NET_EXPORT_PRIVATE PrioritizedDispatcher {
 public:
  class Job;
  typedef PriorityQueue<Job*>::Priority Priority;

  
  
  
  
  
  
  
  
  struct NET_EXPORT_PRIVATE Limits {
    Limits(Priority num_priorities, size_t total_jobs);
    ~Limits();

    
    size_t total_jobs;
    
    
    std::vector<size_t> reserved_slots;
  };

  
  
  
  
  class Job {
   public:
    
    virtual ~Job() {}
    
    
    virtual void Start() = 0;
  };

  
  
  typedef PriorityQueue<Job*>::Pointer Handle;

  
  explicit PrioritizedDispatcher(const Limits& limits);

  ~PrioritizedDispatcher();

  size_t num_running_jobs() const { return num_running_jobs_; }
  size_t num_queued_jobs() const { return queue_.size(); }
  size_t num_priorities() const { return max_running_jobs_.size(); }

  
  
  
  
  Handle Add(Job* job, Priority priority);

  
  
  Handle AddAtHead(Job* job, Priority priority);

  
  
  void Cancel(const Handle& handle);

  
  
  Job* EvictOldestLowest();

  
  
  
  Handle ChangePriority(const Handle& handle, Priority priority);

  
  void OnJobFinished();

  
  
  
  
  Limits GetLimits() const;

  
  
  void SetLimits(const Limits& limits);

  
  void SetLimitsToZero();

 private:
  
  
  
  bool MaybeDispatchJob(const Handle& handle, Priority priority);

  
  
  bool MaybeDispatchNextJob();

  
  PriorityQueue<Job*> queue_;
  
  
  
  std::vector<size_t> max_running_jobs_;
  
  size_t num_running_jobs_;

  DISALLOW_COPY_AND_ASSIGN(PrioritizedDispatcher);
};

}  

#endif  
