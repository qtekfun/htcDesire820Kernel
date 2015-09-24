// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CONTENT_RENDERER_P2P_SOCKET_DISPATCHER_H_
#define CONTENT_RENDERER_P2P_SOCKET_DISPATCHER_H_

#include <vector>

#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/id_map.h"
#include "base/observer_list_threadsafe.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "content/public/common/p2p_socket_type.h"
#include "ipc/ipc_channel_proxy.h"
#include "net/base/net_util.h"

namespace base {
class MessageLoopProxy;
}  

namespace net {
class IPEndPoint;
}  

namespace content {

class NetworkListObserver;
class RenderViewImpl;
class P2PHostAddressRequest;
class P2PSocketClientImpl;

class CONTENT_EXPORT P2PSocketDispatcher
    : public IPC::ChannelProxy::MessageFilter {
 public:
  P2PSocketDispatcher(base::MessageLoopProxy* ipc_message_loop);

  
  
  
  
  void AddNetworkListObserver(NetworkListObserver* network_list_observer);

  
  
  void RemoveNetworkListObserver(NetworkListObserver* network_list_observer);

 protected:
  virtual ~P2PSocketDispatcher();

 private:
  friend class P2PHostAddressRequest;
  friend class P2PSocketClientImpl;

  
  virtual void Send(IPC::Message* message);

  
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual void OnChannelClosing() OVERRIDE;

  
  base::MessageLoopProxy* message_loop();

  
  int RegisterClient(P2PSocketClientImpl* client);
  void UnregisterClient(int id);
  void SendP2PMessage(IPC::Message* msg);

  
  int RegisterHostAddressRequest(P2PHostAddressRequest* request);
  void UnregisterHostAddressRequest(int id);

  
  void OnNetworkListChanged(const net::NetworkInterfaceList& networks);
  void OnGetHostAddressResult(int32 request_id,
                              const net::IPAddressNumber& address);
  void OnSocketCreated(int socket_id, const net::IPEndPoint& address);
  void OnIncomingTcpConnection(int socket_id, const net::IPEndPoint& address);
  void OnSendComplete(int socket_id);
  void OnError(int socket_id);
  void OnDataReceived(int socket_id, const net::IPEndPoint& address,
                      const std::vector<char>& data,
                      const base::TimeTicks& timestamp);

  P2PSocketClientImpl* GetClient(int socket_id);

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  IDMap<P2PSocketClientImpl> clients_;

  IDMap<P2PHostAddressRequest> host_address_requests_;

  bool network_notifications_started_;
  scoped_refptr<ObserverListThreadSafe<NetworkListObserver> >
      network_list_observers_;

  IPC::Channel* channel_;

  DISALLOW_COPY_AND_ASSIGN(P2PSocketDispatcher);
};

}  

#endif  
