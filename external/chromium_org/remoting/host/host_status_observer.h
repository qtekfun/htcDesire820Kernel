// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_HOST_STATUS_OBSERVER_H_
#define REMOTING_HOST_HOST_STATUS_OBSERVER_H_

#include <string>

namespace net {
class IPEndPoint;
}  

namespace remoting {
class SignalStrategy;

namespace protocol {
struct TransportRoute;
};

class HostStatusObserver {
 public:
  HostStatusObserver() { }
  virtual ~HostStatusObserver() { }

  
  virtual void OnAccessDenied(const std::string& jid) {}

  
  virtual void OnClientAuthenticated(const std::string& jid) {}

  
  virtual void OnClientConnected(const std::string& jid) {}

  
  virtual void OnClientDisconnected(const std::string& jid) {}

  
  
  virtual void OnClientRouteChange(const std::string& jid,
                                   const std::string& channel_name,
                                   const protocol::TransportRoute& route) {}

  
  virtual void OnStart(const std::string& xmpp_login) {}

  
  virtual void OnShutdown() {}
};

}  

#endif  
