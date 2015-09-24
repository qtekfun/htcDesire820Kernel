// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_HOST_RESOURCE_MESSAGE_HANDLER_H_
#define PPAPI_HOST_RESOURCE_MESSAGE_HANDLER_H_

#include "base/basictypes.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/host/ppapi_host_export.h"

namespace IPC {
class Message;
}

namespace ppapi {
namespace host {

struct HostMessageContext;
struct ReplyMessageContext;

class PPAPI_HOST_EXPORT ResourceMessageHandler {
 public:
  ResourceMessageHandler();
  virtual ~ResourceMessageHandler();

  
  
  
  
  
  virtual bool HandleMessage(const IPC::Message& msg,
                             HostMessageContext* context) = 0;

  
  virtual void SendReply(const ReplyMessageContext& context,
                         const IPC::Message& msg) = 0;

 protected:
  
  void RunMessageHandlerAndReply(const IPC::Message& msg,
                                 HostMessageContext* context);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int32_t OnResourceMessageReceived(const IPC::Message& msg,
                                            HostMessageContext* context);

 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceMessageHandler);
};

}  
}  

#endif  
