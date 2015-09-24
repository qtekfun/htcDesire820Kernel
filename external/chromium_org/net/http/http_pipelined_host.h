// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_HOST_H_
#define NET_HTTP_HTTP_PIPELINED_HOST_H_

#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/http/http_pipelined_connection.h"
#include "net/http/http_pipelined_host_capability.h"

namespace base {
class Value;
}

namespace net {

class BoundNetLog;
class ClientSocketHandle;
class HostPortPair;
class HttpPipelinedStream;
class ProxyInfo;
struct SSLConfig;

class NET_EXPORT_PRIVATE HttpPipelinedHost {
 public:
  class NET_EXPORT_PRIVATE Key {
   public:
    Key(const HostPortPair& origin);

    
    const HostPortPair& origin() const { return origin_; }

    bool operator<(const Key& rhs) const;

   private:
    const HostPortPair origin_;
  };

  class Delegate {
   public:
    
    
    virtual void OnHostIdle(HttpPipelinedHost* host) = 0;

    
    
    virtual void OnHostHasAdditionalCapacity(HttpPipelinedHost* host) = 0;

    
    virtual void OnHostDeterminedCapability(
        HttpPipelinedHost* host,
        HttpPipelinedHostCapability capability) = 0;
  };

  class Factory {
   public:
    virtual ~Factory() {}

    
    virtual HttpPipelinedHost* CreateNewHost(
        Delegate* delegate, const Key& key,
        HttpPipelinedConnection::Factory* factory,
        HttpPipelinedHostCapability capability,
        bool force_pipelining) = 0;
  };

  virtual ~HttpPipelinedHost() {}

  
  
  virtual HttpPipelinedStream* CreateStreamOnNewPipeline(
      ClientSocketHandle* connection,
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      const BoundNetLog& net_log,
      bool was_npn_negotiated,
      NextProto protocol_negotiated) = 0;

  
  
  virtual HttpPipelinedStream* CreateStreamOnExistingPipeline() = 0;

  
  
  virtual bool IsExistingPipelineAvailable() const = 0;

  
  virtual const Key& GetKey() const = 0;

  
  
  virtual base::Value* PipelineInfoToValue() const = 0;
};

}  

#endif  
