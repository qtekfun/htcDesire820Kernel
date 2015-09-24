// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYNCHRONIZATION_WAITABLE_EVENT_WATCHER_H_
#define BASE_SYNCHRONIZATION_WAITABLE_EVENT_WATCHER_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include "base/win/object_watcher.h"
#else
#include "base/message_loop.h"
#include "base/synchronization/waitable_event.h"
#endif

#include "base/base_api.h"

namespace base {

class Flag;
class AsyncWaiter;
class AsyncCallbackTask;
class WaitableEvent;


class BASE_API WaitableEventWatcher
#if defined(OS_POSIX)
    : public MessageLoop::DestructionObserver
#endif
{
 public:

  WaitableEventWatcher();
  ~WaitableEventWatcher();

  class Delegate {
   public:
    virtual ~Delegate() { }

    
    
    
    
    
    
    
    
    virtual void OnWaitableEventSignaled(WaitableEvent* waitable_event) = 0;
  };

  
  
  
  
  
  bool StartWatching(WaitableEvent* event, Delegate* delegate);

  
  
  
  
  
  
  
  
  
  void StopWatching();

  
  
  
  
  WaitableEvent* GetWatchedEvent();

  
  
  
  Delegate* delegate() {
    return delegate_;
  }

 private:
#if defined(OS_WIN)
  
  
  
  
  
  
  class ObjectWatcherHelper : public win::ObjectWatcher::Delegate {
   public:
    ObjectWatcherHelper(WaitableEventWatcher* watcher);

    
    
    
    void OnObjectSignaled(HANDLE h);

   private:
    WaitableEventWatcher *const watcher_;
  };

  void OnObjectSignaled();

  ObjectWatcherHelper helper_;
  win::ObjectWatcher watcher_;
#else
  
  
  
  virtual void WillDestroyCurrentMessageLoop();

  MessageLoop* message_loop_;
  scoped_refptr<Flag> cancel_flag_;
  AsyncWaiter* waiter_;
  AsyncCallbackTask* callback_task_;
  scoped_refptr<WaitableEvent::WaitableEventKernel> kernel_;
#endif

  WaitableEvent* event_;

  Delegate* delegate_;
};

}  

#endif  
