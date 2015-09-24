// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_WORKER_THREAD_TICKER_H_
#define CHROME_COMMON_WORKER_THREAD_TICKER_H_
#pragma once

#include <vector>

#include "base/synchronization/lock.h"
#include "base/threading/thread.h"

class WorkerThreadTicker {
 public:
  
  
  class Callback {
   public:
    
    virtual void OnTick() = 0;

   protected:
    virtual ~Callback() {}
  };

  
  
  explicit WorkerThreadTicker(int tick_interval);

  ~WorkerThreadTicker();

  
  
  
  bool RegisterTickHandler(Callback *tick_handler);

  
  
  bool UnregisterTickHandler(Callback *tick_handler);

  
  
  bool Start();
  
  
  
  
  bool Stop();
  bool IsRunning() const {
    return is_running_;
  }

  void set_tick_interval(int tick_interval) {
    tick_interval_ = tick_interval;
  }

  int tick_interval() const {
    return tick_interval_;
  }

 private:
  class TimerTask;

  void ScheduleTimerTask();

  
  typedef std::vector<Callback*> TickHandlerListType;

  
  base::Lock lock_;

  base::Thread timer_thread_;
  bool is_running_;

  
  int tick_interval_;

  
  TickHandlerListType tick_handler_list_;

  DISALLOW_COPY_AND_ASSIGN(WorkerThreadTicker);
};

#endif  
