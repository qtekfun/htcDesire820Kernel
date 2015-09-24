// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_WIN_H_
#define IPC_IPC_CHANNEL_WIN_H_

#include "ipc/ipc_channel.h"

#include <queue>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop.h"
#include "ipc/ipc_channel_reader.h"

namespace base {
class ThreadChecker;
}

namespace IPC {

class Channel::ChannelImpl : public internal::ChannelReader,
                             public base::MessageLoopForIO::IOHandler {
 public:
  
  ChannelImpl(const IPC::ChannelHandle &channel_handle, Mode mode,
              Listener* listener);
  ~ChannelImpl();
  bool Connect();
  void Close();
  bool Send(Message* message);
  static bool IsNamedServerInitialized(const std::string& channel_id);
  base::ProcessId peer_pid() const { return peer_pid_; }

 private:
  
  virtual ReadState ReadData(char* buffer,
                             int buffer_len,
                             int* bytes_read) OVERRIDE;
  virtual bool WillDispatchInputMessage(Message* msg) OVERRIDE;
  bool DidEmptyInputBuffers() OVERRIDE;
  virtual void HandleInternalMessage(const Message& msg) OVERRIDE;

  static const string16 PipeName(const std::string& channel_id,
                                 int32* secret);
  bool CreatePipe(const IPC::ChannelHandle &channel_handle, Mode mode);

  bool ProcessConnection();
  bool ProcessOutgoingMessages(base::MessageLoopForIO::IOContext* context,
                               DWORD bytes_written);

  
  virtual void OnIOCompleted(base::MessageLoopForIO::IOContext* context,
                             DWORD bytes_transfered,
                             DWORD error);

 private:
  struct State {
    explicit State(ChannelImpl* channel);
    ~State();
    base::MessageLoopForIO::IOContext context;
    bool is_pending;
  };

  State input_state_;
  State output_state_;

  HANDLE pipe_;

  base::ProcessId peer_pid_;

  
  std::queue<Message*> output_queue_;

  
  
  
  bool waiting_connect_;

  
  
  
  bool processing_incoming_;

  
  bool validate_client_;

  
  
  
  
  int32 client_secret_;


  base::WeakPtrFactory<ChannelImpl> weak_factory_;

  scoped_ptr<base::ThreadChecker> thread_check_;

  DISALLOW_COPY_AND_ASSIGN(ChannelImpl);
};

}  

#endif  
