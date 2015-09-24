// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_SERVER_THREAD_H_
#define NET_TOOLS_QUIC_SERVER_THREAD_H_

#include "base/threading/simple_thread.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/quic_config.h"
#include "net/tools/quic/quic_server.h"

namespace net {
namespace tools {
namespace test {

class ServerThread : public base::SimpleThread {
 public:
  ServerThread(IPEndPoint address,
               const QuicConfig& config,
               const QuicVersionVector& supported_versions,
               bool strike_register_no_startup_period);

  virtual ~ServerThread();

  
  virtual void Run() OVERRIDE;

  
  void WaitForServerStartup();

  
  void WaitForCryptoHandshakeConfirmed();

  
  
  void Pause();

  
  
  void Resume();

  
  
  void Quit();

  
  
  
  QuicServer* server() { return &server_; }

  
  int GetPort();

 private:
  void MaybeNotifyOfHandshakeConfirmation();

  base::WaitableEvent listening_;  
  base::WaitableEvent confirmed_;  
                                   
  base::WaitableEvent pause_;      
  base::WaitableEvent paused_;     
  base::WaitableEvent resume_;     
  base::WaitableEvent quit_;       

  tools::QuicServer server_;
  IPEndPoint address_;
  base::Lock port_lock_;
  int port_;

  DISALLOW_COPY_AND_ASSIGN(ServerThread);
};

}  
}  
}  

#endif  
