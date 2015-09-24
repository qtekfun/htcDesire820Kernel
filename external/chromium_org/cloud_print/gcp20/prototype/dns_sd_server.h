// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_DNS_SD_SERVER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_DNS_SD_SERVER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "cloud_print/gcp20/prototype/service_parameters.h"
#include "net/base/ip_endpoint.h"
#include "net/udp/udp_socket.h"

namespace net {

class IOBufferWithSize;

}  

struct DnsQueryRecord;
class DnsResponseBuilder;

class DnsSdServer : public base::SupportsWeakPtr<DnsSdServer> {
 public:
  
  DnsSdServer();

  
  ~DnsSdServer();

  
  
  bool Start(const ServiceParameters& serv_params,
             uint32 full_ttl,
             const std::vector<std::string>& metadata) WARN_UNUSED_RESULT;

  
  void Update();

  
  void Shutdown();

  
  bool IsOnline() { return !!socket_; }

  
  void UpdateMetadata(const std::vector<std::string>& metadata);

 private:
  
  bool CreateSocket();

  
  void ProccessQuery(uint32 current_ttl,
                     const DnsQueryRecord& query,
                     DnsResponseBuilder* builder) const;

  
  void ProcessMessage(int len, net::IOBufferWithSize* buf);

  
  void DoLoop(int rv);

  
  void OnDatagramReceived();

  
  void SendAnnouncement(uint32 ttl);

  
  
  uint32 GetCurrentTLL() const;

  
  scoped_ptr<net::UDPSocket> socket_;

  
  net::IPEndPoint multicast_address_;

  
  base::Time time_until_live_;

  
  ServiceParameters serv_params_;

  
  scoped_refptr<net::IOBufferWithSize> recv_buf_;

  
  net::IPEndPoint recv_address_;

  
  std::vector<std::string> metadata_;

  
  uint32 full_ttl_;

  DISALLOW_COPY_AND_ASSIGN(DnsSdServer);
};

#endif  

