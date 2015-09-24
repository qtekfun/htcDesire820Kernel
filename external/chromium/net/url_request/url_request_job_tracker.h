// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_JOB_TRACKER_H_
#define NET_URL_REQUEST_URL_REQUEST_JOB_TRACKER_H_
#pragma once

#include <vector>

#include "base/observer_list.h"
#include "net/url_request/url_request_status.h"

class GURL;

namespace net {
class URLRequestJob;

class URLRequestJobTracker {
 public:
  typedef std::vector<URLRequestJob*> JobList;
  typedef JobList::const_iterator JobIterator;

  
  class JobObserver {
   public:
    virtual ~JobObserver() {}

    
    virtual void OnJobAdded(URLRequestJob* job) = 0;

    
    virtual void OnJobRemoved(URLRequestJob* job) = 0;

    
    virtual void OnJobDone(URLRequestJob* job,
                           const URLRequestStatus& status) = 0;

    
    
    virtual void OnJobRedirect(URLRequestJob* job, const GURL& location,
                               int status_code) = 0;

    
    
    
    
    
    virtual void OnBytesRead(URLRequestJob* job, const char* buf,
                             int byte_count) = 0;
  };

  URLRequestJobTracker();
  ~URLRequestJobTracker();

  
  
  void AddObserver(JobObserver* observer) {
    observers_.AddObserver(observer);
  }
  void RemoveObserver(JobObserver* observer) {
    observers_.RemoveObserver(observer);
  }

  
  
  
  void AddNewJob(URLRequestJob* job);
  void RemoveJob(URLRequestJob* job);

  
  void OnJobDone(URLRequestJob* job, const URLRequestStatus& status);
  void OnJobRedirect(URLRequestJob* job, const GURL& location,
                     int status_code);

  
  void OnBytesRead(URLRequestJob* job, const char* buf, int byte_count);

  
  JobIterator begin() const {
    return active_jobs_.begin();
  }
  JobIterator end() const {
    return active_jobs_.end();
  }

 private:
  ObserverList<JobObserver> observers_;
  JobList active_jobs_;
};

extern URLRequestJobTracker g_url_request_job_tracker;

}  

#endif  
