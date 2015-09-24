// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_P2P_SOCKET_CLIENT_DELEGATE_H_
#define CONTENT_PUBLIC_RENDERER_P2P_SOCKET_CLIENT_DELEGATE_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "content/public/common/p2p_socket_type.h"
#include "net/base/ip_endpoint.h"

namespace content {

class P2PSocketClient;

class P2PSocketClientDelegate {
 public:
  virtual ~P2PSocketClientDelegate() { }

  
  
  
  virtual void OnOpen(const net::IPEndPoint& address) = 0;

  
  
  virtual void OnIncomingTcpConnection(const net::IPEndPoint& address,
                                       P2PSocketClient* client) = 0;

  
  virtual void OnSendComplete() = 0;

  
  virtual void OnError() = 0;

  
  virtual void OnDataReceived(const net::IPEndPoint& address,
                              const std::vector<char>& data,
                              const base::TimeTicks& timestamp) = 0;
};

}  

#endif  
