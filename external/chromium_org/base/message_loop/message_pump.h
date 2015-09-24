// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"

namespace base {

class TimeDelta;
class TimeTicks;

class BASE_EXPORT MessagePump : public NonThreadSafe {
 public:
  
  
  class BASE_EXPORT Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    virtual bool DoWork() = 0;

    
    
    
    
    
    
    
    
    virtual bool DoDelayedWork(TimeTicks* next_delayed_work_time) = 0;

    
    
    virtual bool DoIdleWork() = 0;

    
    
    
    
    
    
    
    
    
    
    virtual void GetQueueingInformation(size_t* queue_count,
                                        TimeDelta* queueing_delay) {}
  };

  MessagePump();
  virtual ~MessagePump();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Run(Delegate* delegate) = 0;

  
  
  virtual void Quit() = 0;

  
  
  
  
  virtual void ScheduleWork() = 0;

  
  
  
  virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time) = 0;
};

}  

#endif  
