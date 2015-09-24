// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_HOST_MESSAGE_CONTEXT_H_
#define PPAPI_HOST_HOST_MESSAGE_CONTEXT_H_

#include "ipc/ipc_message.h"
#include "ppapi/host/ppapi_host_export.h"
#include "ppapi/proxy/resource_message_params.h"

namespace ppapi {
namespace host {

struct PPAPI_HOST_EXPORT ReplyMessageContext {
  ReplyMessageContext();
  ReplyMessageContext(
      const ppapi::proxy::ResourceMessageReplyParams& cp,
      IPC::Message* sync_reply_msg,
      int routing_id);
  ~ReplyMessageContext();

  
  
  ppapi::proxy::ResourceMessageReplyParams params;

  
  
  
  
  IPC::Message* sync_reply_msg;

  
  
  
  int routing_id;
};

struct PPAPI_HOST_EXPORT HostMessageContext {
  explicit HostMessageContext(
      const ppapi::proxy::ResourceMessageCallParams& cp);
  HostMessageContext(
      int routing_id,
      const ppapi::proxy::ResourceMessageCallParams& cp);
  HostMessageContext(
      const ppapi::proxy::ResourceMessageCallParams& cp,
      IPC::Message* sync_reply_msg);
  ~HostMessageContext();

  
  ReplyMessageContext MakeReplyMessageContext() const;

  
  
  ppapi::proxy::ResourceMessageCallParams params;

  
  
  
  
  IPC::Message reply_msg;

  
  
  IPC::Message* sync_reply_msg;

  
  
  
  int routing_id;
};

}  
}  

#endif  
