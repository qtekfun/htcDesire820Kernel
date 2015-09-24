// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_PROXY_IMPL_H_
#define BASE_MESSAGE_LOOP_PROXY_IMPL_H_
#pragma once

#include "base/base_api.h"
#include "base/message_loop.h"
#include "base/message_loop_proxy.h"
#include "base/synchronization/lock.h"

namespace base {

class BASE_API MessageLoopProxyImpl : public MessageLoopProxy,
                                      public MessageLoop::DestructionObserver {
 public:
  virtual ~MessageLoopProxyImpl();

  
  virtual bool PostTask(const tracked_objects::Location& from_here,
                        Task* task);
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               Task* task, int64 delay_ms);
  virtual bool PostNonNestableTask(const tracked_objects::Location& from_here,
                                   Task* task);
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      Task* task,
      int64 delay_ms);
  virtual bool BelongsToCurrentThread();

  
  virtual void WillDestroyCurrentMessageLoop();

 protected:
  
  
  virtual void OnDestruct() const;

 private:
  MessageLoopProxyImpl();
  bool PostTaskHelper(const tracked_objects::Location& from_here,
                      Task* task, int64 delay_ms, bool nestable);

  
  friend class MessageLoopProxy;

  
  mutable base::Lock message_loop_lock_;
  MessageLoop* target_message_loop_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopProxyImpl);
};

}  

#endif  

