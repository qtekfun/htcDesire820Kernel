// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WORKER_PROCESS_IPC_DELEGATE_H_
#define REMOTING_HOST_WORKER_PROCESS_IPC_DELEGATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"

namespace IPC {
class Message;
} 

namespace remoting {

class WorkerProcessIpcDelegate {
 public:
  virtual ~WorkerProcessIpcDelegate() {}

  
  virtual void OnChannelConnected(int32 peer_pid) = 0;

  
  virtual bool OnMessageReceived(const IPC::Message& message) = 0;

  
  virtual void OnPermanentError(int exit_code) = 0;
};

}  

#endif  
