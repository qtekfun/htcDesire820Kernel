// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_CONNECTION_H_
#define PPAPI_PROXY_CONNECTION_H_

#include "ipc/ipc_message.h"

namespace IPC {
class Sender;
}

namespace ppapi {
namespace proxy {

struct Connection {
  Connection() : browser_sender(0),
                 renderer_sender(0),
                 in_process(false),
                 browser_sender_routing_id(MSG_ROUTING_NONE) {
  }
  Connection(IPC::Sender* browser, IPC::Sender* renderer)
      : browser_sender(browser),
        renderer_sender(renderer),
        in_process(false),
        browser_sender_routing_id(MSG_ROUTING_NONE) {
  }
  Connection(IPC::Sender* browser, IPC::Sender* renderer, int routing_id)
      : browser_sender(browser),
        renderer_sender(renderer),
        in_process(true),
        browser_sender_routing_id(routing_id) {
  }

  IPC::Sender* browser_sender;
  IPC::Sender* renderer_sender;
  bool in_process;
  
  
  
  int browser_sender_routing_id;
};

}  
}  


#endif  

