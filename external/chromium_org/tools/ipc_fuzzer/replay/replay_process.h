// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_IPC_FUZZER_REPLAY_REPLAY_PROCESS_H_
#define TOOLS_IPC_FUZZER_REPLAY_REPLAY_PROCESS_H_

#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "base/timer/timer.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_message.h"
#include "tools/ipc_fuzzer/message_lib/message_file.h"

namespace ipc_fuzzer {

class ReplayProcess : public IPC::Listener {
 public:
  ReplayProcess();
  virtual ~ReplayProcess();

  
  
  bool Initialize(int argc, const char** argv);

  
  void OpenChannel();

  
  
  bool OpenTestcase();

  
  void Run();

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

 private:
  void SendNextMessage();

  scoped_ptr<IPC::ChannelProxy> channel_;
  base::MessageLoop main_loop_;
  base::Thread io_thread_;
  base::WaitableEvent shutdown_event_;
  scoped_ptr<base::Timer> timer_;
  MessageVector messages_;
  size_t message_index_;

  DISALLOW_COPY_AND_ASSIGN(ReplayProcess);
};

}  

#endif  
