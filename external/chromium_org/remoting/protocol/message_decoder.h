// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_MESSAGE_DECODER_H_
#define REMOTING_PROTOCOL_MESSAGE_DECODER_H_

#include <deque>
#include <list>

#include "base/memory/ref_counted.h"
#include "net/base/io_buffer.h"
#include "remoting/base/compound_buffer.h"

#if defined(USE_SYSTEM_PROTOBUF)
#include <google/protobuf/message_lite.h>
#else
#include "third_party/protobuf/src/google/protobuf/message_lite.h"
#endif

namespace remoting {
namespace protocol {

class MessageDecoder {
 public:
  MessageDecoder();
  virtual ~MessageDecoder();

  
  
  void AddData(scoped_refptr<net::IOBuffer> data, int data_size);

  
  
  
  
  CompoundBuffer* GetNextMessage();

 private:
  
  
  
  bool GetPayloadSize(int* size);

  CompoundBuffer buffer_;

  
  
  
  int next_payload_;
  bool next_payload_known_;
};

}  
}  

#endif  
