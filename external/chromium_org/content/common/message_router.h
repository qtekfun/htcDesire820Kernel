// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_MESSAGE_ROUTER_H_
#define CONTENT_COMMON_MESSAGE_ROUTER_H_

#include "base/id_map.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"


namespace content {

class MessageRouter : public IPC::Listener, public IPC::Sender {
 public:
  MessageRouter();
  virtual ~MessageRouter();

  
  virtual bool OnControlMessageReceived(const IPC::Message& msg);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

  
  
  
  virtual bool RouteMessage(const IPC::Message& msg);

  
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  
  void AddRoute(int32 routing_id, IPC::Listener* listener);
  void RemoveRoute(int32 routing_id);

 private:
  
  IDMap<IPC::Listener> routes_;

  DISALLOW_COPY_AND_ASSIGN(MessageRouter);
};

}  

#endif  
