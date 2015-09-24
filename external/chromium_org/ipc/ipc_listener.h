// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_LISTENER_H_
#define IPC_IPC_LISTENER_H_

#include "base/basictypes.h"
#include "build/build_config.h"
#include "ipc/ipc_export.h"

namespace IPC {

class Message;

class IPC_EXPORT Listener {
 public:
  
  
  virtual bool OnMessageReceived(const Message& message) = 0;

  
  
  virtual void OnChannelConnected(int32 peer_pid) {}

  
  
  virtual void OnChannelError() {}

#if defined(OS_POSIX)
  
  
  virtual void OnChannelDenied() {}

  
  
  virtual void OnChannelListenError() {}
#endif  

 protected:
  virtual ~Listener() {}
};

}  

#endif  
