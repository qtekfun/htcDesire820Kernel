// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYNCHRONIZATION_WAITABLE_EVENT_WATCHER_H_
#define BASE_SYNCHRONIZATION_WAITABLE_EVENT_WATCHER_H_

#include "base/base_export.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include "base/win/object_watcher.h"
#else
#include "base/callback.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/waitable_event.h"
#endif

namespace base {

class Flag;
class AsyncWaiter;
class AsyncCallbackTask;
class WaitableEvent;


class BASE_EXPORT WaitableEventWatcher
#if defined(OS_WIN)
    : public win::ObjectWatcher::Delegate {
#else
    : public MessageLoop::DestructionObserver {
#endif
 public:
  typedef Callback<void(WaitableEvent*)> EventCallback;
  WaitableEventWatcher();
  virtual ~WaitableEventWatcher();

  
  
  bool StartWatching(WaitableEvent* event, const EventCallback& callback);

  
  
  
  
  
  
  
  void StopWatching();

  
  
  WaitableEvent* GetWatchedEvent();

  
  
  const EventCallback& callback() const { return callback_; }

 private:
#if defined(OS_WIN)
  virtual void OnObjectSignaled(HANDLE h) OVERRIDE;
  win::ObjectWatcher watcher_;
#else
  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  MessageLoop* message_loop_;
  scoped_refptr<Flag> cancel_flag_;
  AsyncWaiter* waiter_;
  base::Closure internal_callback_;
  scoped_refptr<WaitableEvent::WaitableEventKernel> kernel_;
#endif

  WaitableEvent* event_;
  EventCallback callback_;
};

}  

#endif  
