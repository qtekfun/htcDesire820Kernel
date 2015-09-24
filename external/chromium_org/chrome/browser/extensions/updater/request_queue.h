// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_UPDATER_REQUEST_QUEUE_H_
#define CHROME_BROWSER_EXTENSIONS_UPDATER_REQUEST_QUEUE_H_

#include <deque>
#include <utility>

#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/backoff_entry.h"

namespace extensions {

template<typename T>
class RequestQueue {
 public:
  class iterator;

  RequestQueue(const net::BackoffEntry::Policy* backoff_policy,
               const base::Closure& start_request_callback);
  ~RequestQueue();

  
  T* active_request();

  
  int active_request_failure_count();

  
  scoped_ptr<T> reset_active_request();

  
  
  void ScheduleRequest(scoped_ptr<T> request);

  bool empty() const;
  size_t size() const;

  
  base::TimeTicks NextReleaseTime() const;

  
  
  
  
  void StartNextRequest();

  
  
  
  
  void RetryRequest(const base::TimeDelta& min_backoff_delay);

  iterator begin();
  iterator end();

  
  void set_backoff_policy(const net::BackoffEntry::Policy* backoff_policy);

 private:
  struct Request {
    Request(net::BackoffEntry* backoff_entry, T* request)
        : backoff_entry(backoff_entry), request(request) {}
    linked_ptr<net::BackoffEntry> backoff_entry;
    linked_ptr<T> request;
  };

  
  static bool CompareRequests(const Request& a,
                              const Request& b);

  
  void PushImpl(scoped_ptr<T> request,
                scoped_ptr<net::BackoffEntry> backoff_entry);

  
  const net::BackoffEntry::Policy* backoff_policy_;

  
  base::Closure start_request_callback_;

  
  
  std::deque<Request> pending_requests_;

  
  scoped_ptr<T> active_request_;
  scoped_ptr<net::BackoffEntry> active_backoff_entry_;

  
  
  base::Timer timer_;
};

template<typename T>
class RequestQueue<T>::iterator {
 public:
  iterator() {}

  T* operator*() { return it_->request.get(); }
  T* operator->() { return it_->request.get(); }
  iterator& operator++() {
    ++it_;
    return *this;
  }
  bool operator!=(const iterator& b) const {
    return it_ != b.it_;
  }

 private:
  friend class RequestQueue<T>;
  typedef std::deque<typename RequestQueue<T>::Request> Container;

  explicit iterator(const typename Container::iterator& it)
      : it_(it) {}

  typename Container::iterator it_;
};


}  

#endif  
