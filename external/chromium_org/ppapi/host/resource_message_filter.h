// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_RESOURCE_MESSAGE_FILTER_H_
#define PPAPI_HOST_RESOURCE_MESSAGE_FILTER_H_

#include "base/memory/ref_counted.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/ppapi_host_export.h"
#include "ppapi/host/resource_message_handler.h"

namespace base {
class MessageLoopProxy;
class TaskRunner;
}

namespace IPC {
class Message;
}

namespace ppapi {
namespace host {

class ResourceHost;
class ResourceMessageFilter;

namespace internal {

struct PPAPI_HOST_EXPORT ResourceMessageFilterDeleteTraits {
  static void Destruct(const ResourceMessageFilter* filter);
};

}  

class PPAPI_HOST_EXPORT ResourceMessageFilter
    : public ResourceMessageHandler,
      public base::RefCountedThreadSafe<
          ResourceMessageFilter, internal::ResourceMessageFilterDeleteTraits> {
 public:
  
  
  
  
  
  
  ResourceMessageFilter();
  
  
  ResourceMessageFilter(
      scoped_refptr<base::MessageLoopProxy> reply_thread_message_loop_proxy);

  
  void OnFilterAdded(ResourceHost* resource_host);
  
  void OnFilterDestroyed();

  
  
  virtual bool HandleMessage(const IPC::Message& msg,
                             HostMessageContext* context) OVERRIDE;

  
  virtual void SendReply(const ReplyMessageContext& context,
      const IPC::Message& msg) OVERRIDE;

 protected:
  virtual ~ResourceMessageFilter();

  
  
  virtual scoped_refptr<base::TaskRunner> OverrideTaskRunnerForMessage(
      const IPC::Message& message);

 private:
  friend class base::DeleteHelper<ResourceMessageFilter>;
  friend class base::RefCountedThreadSafe<
      ResourceMessageFilter, internal::ResourceMessageFilterDeleteTraits>;
  friend struct internal::ResourceMessageFilterDeleteTraits;

  
  void DispatchMessage(const IPC::Message& msg,
                       HostMessageContext context);

  scoped_refptr<base::MessageLoopProxy> deletion_message_loop_proxy_;

  
  
  
  scoped_refptr<base::MessageLoopProxy> reply_thread_message_loop_proxy_;

  
  
  
  
  
  
  ResourceHost* resource_host_;

  DISALLOW_COPY_AND_ASSIGN(ResourceMessageFilter);
};

}  
}  

#endif  
