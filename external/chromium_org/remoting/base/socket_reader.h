// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_SOCKET_READER_H_
#define REMOTING_BASE_SOCKET_READER_H_

#include "base/memory/ref_counted.h"
#include "net/base/completion_callback.h"
#include "base/memory/weak_ptr.h"

namespace net {
class IOBuffer;
class Socket;
}  

namespace remoting {

class SocketReader {
 public:
  
  
  typedef base::Callback<void(scoped_refptr<net::IOBuffer> data,
                              int result)> ReadResultCallback;

  SocketReader();
  ~SocketReader();

  
  
  
  void Init(net::Socket* socket, ReadResultCallback read_result_callback);

 private:
  void DoRead();
  void OnRead(int result);
  void HandleReadResult(int result);
  void CallCallback(scoped_refptr<net::IOBuffer> data, int result);

  net::Socket* socket_;
  ReadResultCallback read_result_callback_;
  scoped_refptr<net::IOBuffer> read_buffer_;

  base::WeakPtrFactory<SocketReader> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(SocketReader);
};

}  

#endif  
