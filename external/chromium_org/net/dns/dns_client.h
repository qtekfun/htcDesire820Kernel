// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_CLIENT_H_
#define NET_DNS_DNS_CLIENT_H_

#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class AddressSorter;
struct DnsConfig;
class DnsTransactionFactory;
class NetLog;

class NET_EXPORT DnsClient {
 public:
  virtual ~DnsClient() {}

  
  
  virtual void SetConfig(const DnsConfig& config) = 0;

  
  virtual const DnsConfig* GetConfig() const = 0;

  
  virtual DnsTransactionFactory* GetTransactionFactory() = 0;

  
  virtual AddressSorter* GetAddressSorter() = 0;

  
  static scoped_ptr<DnsClient> CreateClient(NetLog* net_log);
};

}  

#endif  

