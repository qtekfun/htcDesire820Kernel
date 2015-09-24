// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_SOCKET_POOL_H_
#define NET_DNS_DNS_SOCKET_POOL_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"

namespace net {

class ClientSocketFactory;
class DatagramClientSocket;
class IPEndPoint;
class NetLog;
class StreamSocket;

class NET_EXPORT_PRIVATE DnsSocketPool {
 public:
  virtual ~DnsSocketPool() { }

  
  
  
  static scoped_ptr<DnsSocketPool> CreateDefault(
      ClientSocketFactory* factory);

  
  
  
  static scoped_ptr<DnsSocketPool> CreateNull(
      ClientSocketFactory* factory);

  
  
  
  
  
  
  virtual void Initialize(
      const std::vector<IPEndPoint>* nameservers,
      NetLog* net_log) = 0;

  
  
  
  
  virtual scoped_ptr<DatagramClientSocket> AllocateSocket(
      unsigned server_index) = 0;

  
  
  virtual void FreeSocket(
      unsigned server_index,
      scoped_ptr<DatagramClientSocket> socket) = 0;

  
  
  scoped_ptr<StreamSocket> CreateTCPSocket(
      unsigned server_index,
      const NetLog::Source& source);

 protected:
  DnsSocketPool(ClientSocketFactory* socket_factory);

  void InitializeInternal(
      const std::vector<IPEndPoint>* nameservers,
      NetLog* net_log);

  scoped_ptr<DatagramClientSocket> CreateConnectedSocket(
      unsigned server_index);

 private:
  ClientSocketFactory* socket_factory_;
  NetLog* net_log_;
  const std::vector<IPEndPoint>* nameservers_;
  bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(DnsSocketPool);
};

} 

#endif 
