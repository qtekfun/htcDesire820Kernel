// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_HOST_POOL_H_
#define NET_HTTP_HTTP_PIPELINED_HOST_POOL_H_

#include <map>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/http/http_pipelined_host.h"
#include "net/http/http_pipelined_host_capability.h"

namespace base {
class Value;
}

namespace net {

class HostPortPair;
class HttpPipelinedStream;
class HttpServerProperties;

class NET_EXPORT_PRIVATE HttpPipelinedHostPool
    : public HttpPipelinedHost::Delegate {
 public:
  class Delegate {
   public:
    
    
    virtual void OnHttpPipelinedHostHasAdditionalCapacity(
        HttpPipelinedHost* host) = 0;
  };

  HttpPipelinedHostPool(
      Delegate* delegate,
      HttpPipelinedHost::Factory* factory,
      const base::WeakPtr<HttpServerProperties>& http_server_properties,
      bool force_pipelining);
  virtual ~HttpPipelinedHostPool();

  
  
  bool IsKeyEligibleForPipelining(const HttpPipelinedHost::Key& key);

  
  
  HttpPipelinedStream* CreateStreamOnNewPipeline(
      const HttpPipelinedHost::Key& key,
      ClientSocketHandle* connection,
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      const BoundNetLog& net_log,
      bool was_npn_negotiated,
      NextProto protocol_negotiated);

  
  
  HttpPipelinedStream* CreateStreamOnExistingPipeline(
      const HttpPipelinedHost::Key& key);

  
  
  bool IsExistingPipelineAvailableForKey(const HttpPipelinedHost::Key& key);

  
  virtual void OnHostIdle(HttpPipelinedHost* host) OVERRIDE;

  virtual void OnHostHasAdditionalCapacity(HttpPipelinedHost* host) OVERRIDE;

  virtual void OnHostDeterminedCapability(
      HttpPipelinedHost* host,
      HttpPipelinedHostCapability capability) OVERRIDE;

  
  
  base::Value* PipelineInfoToValue() const;

 private:
  typedef std::map<HttpPipelinedHost::Key, HttpPipelinedHost*> HostMap;

  HttpPipelinedHost* GetPipelinedHost(const HttpPipelinedHost::Key& key,
                                      bool create_if_not_found);

  Delegate* delegate_;
  scoped_ptr<HttpPipelinedHost::Factory> factory_;
  HostMap host_map_;
  const base::WeakPtr<HttpServerProperties> http_server_properties_;
  bool force_pipelining_;

  DISALLOW_COPY_AND_ASSIGN(HttpPipelinedHostPool);
};

}  

#endif  
