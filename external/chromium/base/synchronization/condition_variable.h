// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_SYNCHRONIZATION_CONDITION_VARIABLE_H_
#define BASE_SYNCHRONIZATION_CONDITION_VARIABLE_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <pthread.h>
#endif

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/synchronization/lock.h"

namespace base {

class TimeDelta;

class BASE_API ConditionVariable {
 public:
  
  explicit ConditionVariable(Lock* user_lock);

  ~ConditionVariable();

  
  
  void Wait();
  void TimedWait(const TimeDelta& max_time);

  
  void Broadcast();
  
  void Signal();

 private:

#if defined(OS_WIN)

  
  
  
  
  
  
  class Event {
   public:
    
    Event();
    ~Event();

    
    void InitListElement();

    
    bool IsEmpty() const;
    void PushBack(Event* other);
    Event* PopFront();
    Event* PopBack();

    
    
    HANDLE handle() const;
    
    Event* Extract();

    
    bool IsSingleton() const;

   private:
    
    bool ValidateAsDistinct(Event* other) const;
    bool ValidateAsItem() const;
    bool ValidateAsList() const;
    bool ValidateLinks() const;

    HANDLE handle_;
    Event* next_;
    Event* prev_;
    DISALLOW_COPY_AND_ASSIGN(Event);
  };

  
  
  enum RunState { SHUTDOWN = 0, RUNNING = 64213 };

  
  Event* GetEventForWaiting();
  void RecycleEvent(Event* used_event);

  RunState run_state_;

  
  base::Lock internal_lock_;

  
  base::Lock& user_lock_;

  
  Event waiting_list_;

  
  Event recycling_list_;
  int recycling_list_size_;

  
  int allocation_counter_;

#elif defined(OS_POSIX)

  pthread_cond_t condition_;
  pthread_mutex_t* user_mutex_;
#if !defined(NDEBUG)
  base::Lock* user_lock_;     
#endif

#endif

  DISALLOW_COPY_AND_ASSIGN(ConditionVariable);
};

}  

#endif  
