// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_PLUGIN_MESSAGE_FILTER_H_
#define PPAPI_PROXY_PLUGIN_MESSAGE_FILTER_H_

#include <set>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sender.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/proxy/ppapi_proxy_export.h"

namespace ppapi {
namespace proxy {

class ResourceMessageReplyParams;
class ResourceReplyThreadRegistrar;

class PPAPI_PROXY_EXPORT PluginMessageFilter
    : public IPC::ChannelProxy::MessageFilter,
      public IPC::Sender {
 public:
  
  
  
  
  
  
  
  PluginMessageFilter(
      std::set<PP_Instance>* seen_instance_ids,
      scoped_refptr<ResourceReplyThreadRegistrar> thread_registrar);
  virtual ~PluginMessageFilter();

  
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  
  static void DispatchResourceReplyForTest(
      const ResourceMessageReplyParams& reply_params,
      const IPC::Message& nested_msg);

 private:
  void OnMsgReserveInstanceId(PP_Instance instance, bool* usable);
  void OnMsgResourceReply(const ResourceMessageReplyParams& reply_params,
                          const IPC::Message& nested_msg);

  
  
  static void DispatchResourceReply(
      const ResourceMessageReplyParams& reply_params,
      const IPC::Message& nested_msg);

  
  
  
  std::set<PP_Instance>* seen_instance_ids_;

  scoped_refptr<ResourceReplyThreadRegistrar> resource_reply_thread_registrar_;

  
  
  IPC::Channel* channel_;
};

}  
}  

#endif  
