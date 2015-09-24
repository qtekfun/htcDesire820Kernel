// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_READER_H_
#define IPC_IPC_CHANNEL_READER_H_

#include "base/basictypes.h"
#include "ipc/ipc_channel.h"

namespace IPC {
namespace internal {

class ChannelReader {
 public:
  explicit ChannelReader(Listener* listener);
  virtual ~ChannelReader();

  void set_listener(Listener* listener) { listener_ = listener; }

  
  
  
  bool ProcessIncomingMessages();

  
  
  
  
  
  bool AsyncReadComplete(int bytes_read);

  
  
  bool IsInternalMessage(const Message& m) const;

  
  
  bool IsHelloMessage(const Message& m) const;

 protected:
  enum ReadState { READ_SUCCEEDED, READ_FAILED, READ_PENDING };

  Listener* listener() const { return listener_; }

  
  
  
  
  
  
  
  
  
  
  
  
  virtual ReadState ReadData(char* buffer, int buffer_len, int* bytes_read) = 0;

  
  
  
  
  
  virtual bool WillDispatchInputMessage(Message* msg) = 0;

  
  
  virtual bool DidEmptyInputBuffers() = 0;

  
  virtual void HandleInternalMessage(const Message& msg) = 0;

 private:
  
  
  
  
  bool DispatchInputData(const char* input_data, int input_data_len);

  Listener* listener_;

  
  
  char input_buf_[Channel::kReadBufferSize];

  
  
  std::string input_overflow_buf_;

  DISALLOW_COPY_AND_ASSIGN(ChannelReader);
};

}  
}  

#endif  
