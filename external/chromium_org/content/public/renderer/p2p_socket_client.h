// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_RENDERER_P2P_SOCKET_CLIENT_H_
#define CONTENT_PUBLIC_RENDERER_P2P_SOCKET_CLIENT_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "content/public/common/p2p_socket_type.h"
#include "net/base/ip_endpoint.h"

namespace content {

class P2PSocketClientDelegate;

class CONTENT_EXPORT P2PSocketClient :
      public base::RefCountedThreadSafe<P2PSocketClient> {
 public:
  
  
  
  
  static scoped_refptr<P2PSocketClient> Create(
      P2PSocketType type,
      const net::IPEndPoint& local_address,
      const net::IPEndPoint& remote_address,
      P2PSocketClientDelegate* delegate);

  P2PSocketClient() {}

  
  virtual void Send(const net::IPEndPoint& address,
                    const std::vector<char>& data) = 0;

  
  
  virtual void SendWithDscp(const net::IPEndPoint& address,
                            const std::vector<char>& data,
                            net::DiffServCodePoint dscp) = 0;

  
  virtual void Close() = 0;

  virtual int GetSocketID() const = 0;
  virtual void SetDelegate(P2PSocketClientDelegate* delegate) = 0;

 protected:
  virtual ~P2PSocketClient() {}

 private:
  
  friend class base::RefCountedThreadSafe<P2PSocketClient>;
};
}  

#endif  
