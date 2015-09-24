// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_SOCKET_H_
#define NET_SOCKET_SSL_SOCKET_H_

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/socket/stream_socket.h"

namespace net {

class NET_EXPORT SSLSocket : public StreamSocket {
public:
  virtual ~SSLSocket() {}

  
  
  
  
  
  virtual int ExportKeyingMaterial(const base::StringPiece& label,
                                   bool has_context,
                                   const base::StringPiece& context,
                                   unsigned char* out,
                                   unsigned int outlen) = 0;

  
  virtual int GetTLSUniqueChannelBinding(std::string* out) = 0;
};

}  

#endif  
