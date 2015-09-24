// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_JOB_QUEUE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_JOB_QUEUE_H_

#include <deque>
#include <set>
#include <vector>

#include "chrome/browser/chromeos/drive/job_list.h"

namespace drive {

class JobQueue {
 public:
  
  
  JobQueue(size_t num_max_concurrent_jobs, size_t num_priority_levels);
  ~JobQueue();

  
  
  
  void Push(JobID id, int priority);

  
  
  
  
  
  
  
  
  bool PopForRun(int accepted_priority, JobID* id);

  
  void GetQueuedJobs(int priority, std::vector<JobID>* jobs) const;

  
  
  void MarkFinished(JobID id);

  
  std::string ToString() const;

  
  size_t GetNumberOfJobs() const;

  
  void Remove(JobID id);

 private:
  size_t num_max_concurrent_jobs_;
  std::vector<std::deque<JobID> > queue_;
  std::set<JobID> running_;

  DISALLOW_COPY_AND_ASSIGN(JobQueue);
};

}  

#endif  
