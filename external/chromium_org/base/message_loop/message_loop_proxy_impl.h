// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_MESSAGE_LOOP_MESSAGE_LOOP_PROXY_IMPL_H_
#define BASE_MESSAGE_LOOP_MESSAGE_LOOP_PROXY_IMPL_H_

#include "base/base_export.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/pending_task.h"
#include "base/threading/platform_thread.h"

namespace base {
namespace internal {

class IncomingTaskQueue;

class BASE_EXPORT MessageLoopProxyImpl : public MessageLoopProxy {
 public:
  explicit MessageLoopProxyImpl(
      scoped_refptr<IncomingTaskQueue> incoming_queue);

  
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               const base::Closure& task,
                               base::TimeDelta delay) OVERRIDE;
  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      const base::Closure& task,
      base::TimeDelta delay) OVERRIDE;
  virtual bool RunsTasksOnCurrentThread() const OVERRIDE;

 private:
  friend class RefCountedThreadSafe<MessageLoopProxyImpl>;
  virtual ~MessageLoopProxyImpl();

  
  scoped_refptr<IncomingTaskQueue> incoming_queue_;

  
  PlatformThreadId valid_thread_id_;

  DISALLOW_COPY_AND_ASSIGN(MessageLoopProxyImpl);
};

}  
}  

#endif  
