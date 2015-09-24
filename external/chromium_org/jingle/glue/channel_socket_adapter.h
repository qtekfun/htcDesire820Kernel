// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_CHANNEL_SOCKET_ADAPTER_H_
#define JINGLE_GLUE_CHANNEL_SOCKET_ADAPTER_H_

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "net/socket/socket.h"
#include "third_party/libjingle/source/talk/base/asyncpacketsocket.h"
#include "third_party/libjingle/source/talk/base/socketaddress.h"
#include "third_party/libjingle/source/talk/base/sigslot.h"

namespace base {
class MessageLoop;
}

namespace cricket {
class TransportChannel;
}  

namespace jingle_glue {

class TransportChannelSocketAdapter : public net::Socket,
                                      public sigslot::has_slots<> {
 public:
  
  explicit TransportChannelSocketAdapter(cricket::TransportChannel* channel);
  virtual ~TransportChannelSocketAdapter();

  
  
  
  void SetOnDestroyedCallback(const base::Closure& callback);

  
  
  
  void Close(int error_code);

  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    const net::CompletionCallback& callback) OVERRIDE;

  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

 private:
  void OnNewPacket(cricket::TransportChannel* channel,
                   const char* data,
                   size_t data_size,
                   const talk_base::PacketTime& packet_time,
                   int flags);
  void OnWritableState(cricket::TransportChannel* channel);
  void OnChannelDestroyed(cricket::TransportChannel* channel);

  base::MessageLoop* message_loop_;

  cricket::TransportChannel* channel_;

  base::Closure destruction_callback_;

  net::CompletionCallback read_callback_;
  scoped_refptr<net::IOBuffer> read_buffer_;
  int read_buffer_size_;

  net::CompletionCallback write_callback_;
  scoped_refptr<net::IOBuffer> write_buffer_;
  int write_buffer_size_;

  int closed_error_code_;

  DISALLOW_COPY_AND_ASSIGN(TransportChannelSocketAdapter);
};

}  

#endif  
