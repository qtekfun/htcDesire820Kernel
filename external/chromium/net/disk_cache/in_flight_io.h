// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_IN_FLIGHT_IO_H_
#define NET_DISK_CACHE_IN_FLIGHT_IO_H_
#pragma once

#include <set>

#include "base/message_loop_proxy.h"
#include "base/synchronization/waitable_event.h"

namespace disk_cache {

class InFlightIO;

class BackgroundIO : public base::RefCountedThreadSafe<BackgroundIO> {
 public:
  
  
  
  
  
  explicit BackgroundIO(InFlightIO* controller);

  
  
  
  void OnIOSignalled();

  
  
  
  
  
  
  void Cancel();

  int result() { return result_; }

  base::WaitableEvent* io_completed() {
    return &io_completed_;
  }

 protected:
  virtual ~BackgroundIO();

  InFlightIO* controller_;  
  int result_;  

 private:
  friend class base::RefCountedThreadSafe<BackgroundIO>;

  
  
  void NotifyController();

  
  base::WaitableEvent io_completed_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundIO);
};

class InFlightIO {
 public:
  InFlightIO();
  virtual ~InFlightIO();

  
  
  void WaitForPendingIO();

  
  void OnIOComplete(BackgroundIO* operation);

  
  
  
  
  void InvokeCallback(BackgroundIO* operation, bool cancel_task);

 protected:
  
  
  
  virtual void OnOperationComplete(BackgroundIO* operation, bool cancel) = 0;

  
  
  
  void OnOperationPosted(BackgroundIO* operation);

 private:
  typedef std::set<scoped_refptr<BackgroundIO> > IOList;

  IOList io_list_;  
  scoped_refptr<base::MessageLoopProxy> callback_thread_;

  bool running_;  
  bool single_thread_;  

  DISALLOW_COPY_AND_ASSIGN(InFlightIO);
};

}  

#endif  
