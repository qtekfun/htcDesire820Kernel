// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_PUMP_H_
#define BASE_MESSAGE_PUMP_H_
#pragma once

#include "base/base_api.h"
#include "base/memory/ref_counted.h"

namespace base {

class TimeTicks;

class BASE_API MessagePump : public RefCountedThreadSafe<MessagePump> {
 public:
  
  
  class BASE_API Delegate {
   public:
    virtual ~Delegate() {}

    
    
    
    
    virtual bool DoWork() = 0;

    
    
    
    
    
    
    
    
    virtual bool DoDelayedWork(TimeTicks* next_delayed_work_time) = 0;

    
    
    virtual bool DoIdleWork() = 0;
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
