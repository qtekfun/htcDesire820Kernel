// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_CHILD_RESOURCE_MESSAGE_FILTER_H_
#define CONTENT_CHILD_CHILD_RESOURCE_MESSAGE_FILTER_H_

#include "base/memory/ref_counted.h"
#include "ipc/ipc_channel_proxy.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace content {
class ResourceDispatcher;

class ChildResourceMessageFilter : public IPC::ChannelProxy::MessageFilter {
 public:
  explicit ChildResourceMessageFilter(ResourceDispatcher* resource_dispatcher);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

 private:
  virtual ~ChildResourceMessageFilter();

  scoped_refptr<base::SingleThreadTaskRunner> main_thread_task_runner_;
  ResourceDispatcher* resource_dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(ChildResourceMessageFilter);
};

}  

#endif  
