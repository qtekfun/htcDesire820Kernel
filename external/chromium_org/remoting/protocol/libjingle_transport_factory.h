// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_LIBJINGLE_TRANSPORT_FACTORY_H_
#define REMOTING_PROTOCOL_LIBJINGLE_TRANSPORT_FACTORY_H_

#include <list>

#include "base/callback_forward.h"
#include "remoting/jingle_glue/network_settings.h"
#include "remoting/protocol/transport.h"

namespace cricket {
class HttpPortAllocatorBase;
class PortAllocator;
}  

namespace net {
class URLRequestContextGetter;
}  

namespace talk_base {
class NetworkManager;
class PacketSocketFactory;
class SocketAddress;
}  

namespace remoting {

class SignalStrategy;
class JingleInfoRequest;

namespace protocol {

class LibjingleTransportFactory : public TransportFactory {
 public:
  
  
  
  LibjingleTransportFactory(
      SignalStrategy* signal_strategy,
      scoped_ptr<cricket::HttpPortAllocatorBase> port_allocator,
      const NetworkSettings& network_settings);

  virtual ~LibjingleTransportFactory();

  
  virtual void PrepareTokens() OVERRIDE;
  virtual scoped_ptr<StreamTransport> CreateStreamTransport() OVERRIDE;
  virtual scoped_ptr<DatagramTransport> CreateDatagramTransport() OVERRIDE;

 private:
  void EnsureFreshJingleInfo();
  void OnJingleInfo(const std::string& relay_token,
                    const std::vector<std::string>& relay_hosts,
                    const std::vector<talk_base::SocketAddress>& stun_hosts);

  SignalStrategy* signal_strategy_;
  scoped_ptr<cricket::HttpPortAllocatorBase> port_allocator_;
  NetworkSettings network_settings_;

  base::TimeTicks last_jingle_info_update_time_;
  scoped_ptr<JingleInfoRequest> jingle_info_request_;

  
  
  std::list<base::Closure> on_jingle_info_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(LibjingleTransportFactory);
};

}  
}  

#endif  
