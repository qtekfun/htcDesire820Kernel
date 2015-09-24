// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_GCM_ENGINE_CONNECTION_FACTORY_IMPL_H_
#define GOOGLE_APIS_GCM_ENGINE_CONNECTION_FACTORY_IMPL_H_

#include "google_apis/gcm/engine/connection_factory.h"

#include "base/memory/weak_ptr.h"
#include "google_apis/gcm/protocol/mcs.pb.h"
#include "net/base/backoff_entry.h"
#include "net/base/network_change_notifier.h"
#include "net/socket/client_socket_handle.h"
#include "url/gurl.h"

namespace net {
class HttpNetworkSession;
class NetLog;
}

namespace gcm {

class ConnectionHandlerImpl;

class GCM_EXPORT ConnectionFactoryImpl :
    public ConnectionFactory,
    public net::NetworkChangeNotifier::ConnectionTypeObserver,
    public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  ConnectionFactoryImpl(
      const GURL& mcs_endpoint,
      scoped_refptr<net::HttpNetworkSession> network_session,
      net::NetLog* net_log);
  virtual ~ConnectionFactoryImpl();

  
  virtual void Initialize(
      const BuildLoginRequestCallback& request_builder,
      const ConnectionHandler::ProtoReceivedCallback& read_callback,
      const ConnectionHandler::ProtoSentCallback& write_callback) OVERRIDE;
  virtual ConnectionHandler* GetConnectionHandler() const OVERRIDE;
  virtual void Connect() OVERRIDE;
  virtual bool IsEndpointReachable() const OVERRIDE;
  virtual base::TimeTicks NextRetryAttempt() const OVERRIDE;

  
  virtual void OnConnectionTypeChanged(
      net::NetworkChangeNotifier::ConnectionType type) OVERRIDE;
  virtual void OnIPAddressChanged() OVERRIDE;

 protected:
  
  
  
  
  virtual void ConnectImpl();

  
  
  virtual void InitHandler();

  
  
  virtual scoped_ptr<net::BackoffEntry> CreateBackoffEntry(
      const net::BackoffEntry::Policy* const policy);

  
  void OnConnectDone(int result);

 private:
  
  void ConnectionHandlerCallback(int result);

  
  const GURL mcs_endpoint_;

  
  
  const scoped_refptr<net::HttpNetworkSession> network_session_;
  
  net::NetLog* const net_log_;
  
  net::ClientSocketHandle socket_handle_;
  
  scoped_ptr<net::BackoffEntry> backoff_entry_;

  
  scoped_ptr<ConnectionHandlerImpl> connection_handler_;

  
  BuildLoginRequestCallback request_builder_;

  base::WeakPtrFactory<ConnectionFactoryImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ConnectionFactoryImpl);
};

}  

#endif  
