// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_PROXY_H_
#define BASE_MESSAGE_LOOP_PROXY_H_
#pragma once

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"

namespace base {

struct MessageLoopProxyTraits;

class BASE_API MessageLoopProxy
    : public base::RefCountedThreadSafe<MessageLoopProxy,
                                        MessageLoopProxyTraits> {
 public:
  
  
  
  
  
  
  
  
  virtual bool PostTask(const tracked_objects::Location& from_here,
                        Task* task) = 0;
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               Task* task, int64 delay_ms) = 0;
  virtual bool PostNonNestableTask(const tracked_objects::Location& from_here,
                                   Task* task) = 0;
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      Task* task,
      int64 delay_ms) = 0;
  
  
  virtual bool BelongsToCurrentThread() = 0;

  template <class T>
  bool DeleteSoon(const tracked_objects::Location& from_here,
                  T* object) {
    return PostNonNestableTask(from_here, new DeleteTask<T>(object));
  }
  template <class T>
  bool ReleaseSoon(const tracked_objects::Location& from_here,
                   T* object) {
    return PostNonNestableTask(from_here, new ReleaseTask<T>(object));
  }

  
  
  static scoped_refptr<MessageLoopProxy> CreateForCurrentThread();

 protected:
  friend class RefCountedThreadSafe<MessageLoopProxy, MessageLoopProxyTraits>;
  friend struct MessageLoopProxyTraits;

  MessageLoopProxy();
  virtual ~MessageLoopProxy();

  
  
  virtual void OnDestruct() const;
};

struct MessageLoopProxyTraits {
  static void Destruct(const MessageLoopProxy* proxy) {
    proxy->OnDestruct();
  }
};

}  

#endif  
