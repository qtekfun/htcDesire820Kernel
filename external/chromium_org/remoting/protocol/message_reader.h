// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_MESSAGE_READER_H_
#define REMOTING_PROTOCOL_MESSAGE_READER_H_

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/completion_callback.h"
#include "remoting/base/compound_buffer.h"
#include "remoting/protocol/message_decoder.h"

namespace net {
class IOBuffer;
class Socket;
}  

namespace remoting {
namespace protocol {

class MessageReader : public base::NonThreadSafe {
 public:
  typedef base::Callback<void(scoped_ptr<CompoundBuffer>, const base::Closure&)>
      MessageReceivedCallback;

  MessageReader();
  virtual ~MessageReader();

  
  
  void Init(net::Socket* socket, const MessageReceivedCallback& callback);

 private:
  void DoRead();
  void OnRead(int result);
  void HandleReadResult(int result);
  void OnDataReceived(net::IOBuffer* data, int data_size);
  void RunCallback(scoped_ptr<CompoundBuffer> message);
  void OnMessageDone();

  net::Socket* socket_;

  
  
  bool read_pending_;

  
  
  
  int pending_messages_;

  bool closed_;
  scoped_refptr<net::IOBuffer> read_buffer_;

  MessageDecoder message_decoder_;

  
  MessageReceivedCallback message_received_callback_;

  base::WeakPtrFactory<MessageReader> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(MessageReader);
};

template <class T>
class ProtobufMessageReader {
 public:
  
  
  typedef typename base::Callback<void(scoped_ptr<T> message,
                                       const base::Closure& done_task)>
      MessageReceivedCallback;

  ProtobufMessageReader() { };
  ~ProtobufMessageReader() { };

  void Init(net::Socket* socket, const MessageReceivedCallback& callback) {
    DCHECK(!callback.is_null());
    message_received_callback_ = callback;
    message_reader_.reset(new MessageReader());
    message_reader_->Init(
        socket, base::Bind(&ProtobufMessageReader<T>::OnNewData,
                           base::Unretained(this)));
  }

 private:
  void OnNewData(scoped_ptr<CompoundBuffer> buffer,
                 const base::Closure& done_task) {
    scoped_ptr<T> message(new T());
    CompoundBufferInputStream stream(buffer.get());
    bool ret = message->ParseFromZeroCopyStream(&stream);
    if (!ret) {
      LOG(WARNING) << "Received message that is not a valid protocol buffer.";
    } else {
      DCHECK_EQ(stream.position(), buffer->total_bytes());
      message_received_callback_.Run(message.Pass(), done_task);
    }
  }

  scoped_ptr<MessageReader> message_reader_;
  MessageReceivedCallback message_received_callback_;
};

}  
}  

#endif  
