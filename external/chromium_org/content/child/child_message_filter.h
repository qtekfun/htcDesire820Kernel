// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_CROSS_MESSAGE_FILTER_H_
#define CONTENT_CHILD_CROSS_MESSAGE_FILTER_H_

#include "ipc/ipc_channel_proxy.h"

namespace base {
class TaskRunner;
}

namespace content {

class ThreadSafeSender;

class ChildMessageFilter
    : public base::RefCountedThreadSafe<ChildMessageFilter>,
      public IPC::Sender {
 public:
  
  virtual bool Send(IPC::Message* message) OVERRIDE;

  
  
  
  virtual base::TaskRunner* OverrideTaskRunnerForMessage(
      const IPC::Message& msg) = 0;

  
  
  virtual bool OnMessageReceived(const IPC::Message& msg) = 0;

  
  
  
  
  virtual void OnStaleMessageReceived(const IPC::Message& msg) {}

 protected:
  ChildMessageFilter();
  virtual ~ChildMessageFilter();

 private:
  class Internal;
  friend class ChildThread;
  friend class RenderThreadImpl;
  friend class WorkerThread;

  friend class base::RefCountedThreadSafe<ChildMessageFilter>;

  IPC::ChannelProxy::MessageFilter* GetFilter();

  
  
  Internal* internal_;

  scoped_refptr<ThreadSafeSender> thread_safe_sender_;

  DISALLOW_COPY_AND_ASSIGN(ChildMessageFilter);
};

}  

#endif  
