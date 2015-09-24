// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CLOUD_PRINT_GCP20_PROTOTYPE_DNS_RESPONSE_BUILDER_H_
#define CLOUD_PRINT_GCP20_PROTOTYPE_DNS_RESPONSE_BUILDER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "net/base/net_util.h"
#include "net/dns/dns_protocol.h"

namespace net {

class IOBufferWithSize;

}  

struct DnsResponseRecord {
  DnsResponseRecord();
  ~DnsResponseRecord();

  std::string name;  
  uint16 type;
  uint16 klass;
  uint32 ttl;
  std::string rdata;
};

class DnsResponseBuilder {
 public:
  
  explicit DnsResponseBuilder(uint16 id);

  
  ~DnsResponseBuilder();

  
  void AppendPtr(const std::string& service_type, uint32 ttl,
                 const std::string& service_name);
  void AppendSrv(const std::string& service_name, uint32 ttl, uint16 priority,
                 uint16 weight, uint16 http_port,
                 const std::string& service_domain_name);
  void AppendA(const std::string& service_domain_name, uint32 ttl,
               net::IPAddressNumber http_ipv4);
  void AppendTxt(const std::string& service_name, uint32 ttl,
                 const std::vector<std::string>& metadata);

  
  scoped_refptr<net::IOBufferWithSize> Build();

 private:
  
  void AddResponse(const std::string& name, uint16 type, uint32 ttl,
                   const std::string& rdata);

  std::vector<DnsResponseRecord> responses_;

  
  net::dns_protocol::Header header_;

  DISALLOW_COPY_AND_ASSIGN(DnsResponseBuilder);
};

#endif  

