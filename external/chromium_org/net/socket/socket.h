// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SOCKET_H_
#define NET_SOCKET_SOCKET_H_

#include "net/base/completion_callback.h"
#include "net/base/net_export.h"

namespace net {

class IOBuffer;

class NET_EXPORT Socket {
 public:
  virtual ~Socket() {}

  
  
  
  
  
  
  
  
  
  
  
  virtual int Read(IOBuffer* buf, int buf_len,
                   const CompletionCallback& callback) = 0;

  
  // written partially.  The number of bytes written is returned, or an error
  
  
  
  
  
  
  
  
  // of the actual buffer that is written to the socket.  If the socket is
  
  virtual int Write(IOBuffer* buf, int buf_len,
                    const CompletionCallback& callback) = 0;

  
  
  
  virtual bool SetReceiveBufferSize(int32 size) = 0;

  
  
  
  virtual bool SetSendBufferSize(int32 size) = 0;
};

}  

#endif  
