// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/callback.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/proxy/connection.h"
#include "ppapi/proxy/resource_message_params.h"

namespace IPC {
class Message;
class MessageReplyDeserializer;
}  

namespace content {

class RendererPpapiHostImpl;

class PepperInProcessRouter {
 public:
  
  PepperInProcessRouter(RendererPpapiHostImpl* host_impl);
  ~PepperInProcessRouter();

  
  
  IPC::Sender* GetPluginToRendererSender();
  IPC::Sender* GetRendererToPluginSender();

  
  
  
  ppapi::proxy::Connection GetPluginConnection(PP_Instance instance);

  
  static bool OnPluginMsgReceived(const IPC::Message& msg);

 private:
  bool SendToHost(IPC::Message *msg);
  bool SendToPlugin(IPC::Message *msg);
  void DispatchHostMsg(IPC::Message* msg);
  void DispatchPluginMsg(IPC::Message* msg);
  bool SendToBrowser(IPC::Message *msg);

  RendererPpapiHostImpl* host_impl_;

  class Channel;
  scoped_ptr<Channel> browser_channel_;

  
  scoped_ptr<Channel> host_to_plugin_router_;

  
  scoped_ptr<Channel> plugin_to_host_router_;

  
  int pending_message_id_;

  
  scoped_ptr<IPC::MessageReplyDeserializer> reply_deserializer_;

  
  bool reply_result_;

  base::WeakPtrFactory<PepperInProcessRouter> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperInProcessRouter);
};

}  
