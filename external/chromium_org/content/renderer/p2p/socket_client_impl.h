// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_P2P_SOCKET_CLIENT_IMPL_H_
#define CONTENT_RENDERER_P2P_SOCKET_CLIENT_IMPL_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "content/public/common/p2p_socket_type.h"
#include "content/public/renderer/p2p_socket_client.h"
#include "net/base/ip_endpoint.h"

namespace base {
class MessageLoopProxy;
}  

namespace content {

class P2PSocketDispatcher;

class P2PSocketClientImpl : public P2PSocketClient {
 public:
  explicit P2PSocketClientImpl(P2PSocketDispatcher* dispatcher);

  
  
  
  virtual void Init(P2PSocketType type,
                    const net::IPEndPoint& local_address,
                    const net::IPEndPoint& remote_address,
                    P2PSocketClientDelegate* delegate);

  
  virtual void Send(const net::IPEndPoint& address,
                    const std::vector<char>& data) OVERRIDE;

  
  
  virtual void SendWithDscp(const net::IPEndPoint& address,
                            const std::vector<char>& data,
                            net::DiffServCodePoint dscp) OVERRIDE;

  
  
  virtual void Close() OVERRIDE;

  virtual int GetSocketID() const OVERRIDE;

  virtual void SetDelegate(P2PSocketClientDelegate* delegate) OVERRIDE;

 private:
  enum State {
    STATE_UNINITIALIZED,
    STATE_OPENING,
    STATE_OPEN,
    STATE_CLOSED,
    STATE_ERROR,
  };

  friend class P2PSocketDispatcher;

  virtual ~P2PSocketClientImpl();

  
  void OnSocketCreated(const net::IPEndPoint& address);
  void OnIncomingTcpConnection(const net::IPEndPoint& address);
  void OnSendComplete(int packet_id);
  void OnSendComplete();
  void OnError();
  void OnDataReceived(const net::IPEndPoint& address,
                      const std::vector<char>& data,
                      const base::TimeTicks& timestamp);

  
  void DeliverOnSocketCreated(const net::IPEndPoint& address);
  void DeliverOnIncomingTcpConnection(
      const net::IPEndPoint& address,
      scoped_refptr<P2PSocketClient> new_client);
  void DeliverOnSendComplete();
  void DeliverOnError();
  void DeliverOnDataReceived(const net::IPEndPoint& address,
                             const std::vector<char>& data,
                             const base::TimeTicks& timestamp);

  
  void DoInit(P2PSocketType type,
              const net::IPEndPoint& local_address,
              const net::IPEndPoint& remote_address);

  
  void DoClose();

  
  void Detach();

  P2PSocketDispatcher* dispatcher_;
  scoped_refptr<base::MessageLoopProxy> ipc_message_loop_;
  scoped_refptr<base::MessageLoopProxy> delegate_message_loop_;
  int socket_id_;
  P2PSocketClientDelegate* delegate_;
  State state_;

  
  uint32 random_socket_id_;
  uint32 next_packet_id_;

  DISALLOW_COPY_AND_ASSIGN(P2PSocketClientImpl);
};

}  

#endif  
