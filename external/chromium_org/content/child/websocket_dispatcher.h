// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_CHILD_WEBSOCKET_DISPATCHER_H_
#define CONTENT_CHILD_WEBSOCKET_DISPATCHER_H_

#include <stdint.h>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ipc/ipc_listener.h"

namespace content {

class WebSocketBridge;

class WebSocketDispatcher : public IPC::Listener {
 public:
  WebSocketDispatcher();
  virtual ~WebSocketDispatcher();

  
  int AddBridge(WebSocketBridge* bridge);
  void RemoveBridge(int channel_id);

  
  virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

 private:
  WebSocketBridge* GetBridge(int channel_id, uint32 type);

  std::map<int, WebSocketBridge*> bridges_;
  int channel_id_max_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketDispatcher);
};

}  

#endif  
