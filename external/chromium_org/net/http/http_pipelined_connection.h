// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_CONNECTION_H_
#define NET_HTTP_HTTP_PIPELINED_CONNECTION_H_

#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/socket/ssl_client_socket.h"

namespace net {

class BoundNetLog;
class ClientSocketHandle;
class HostPortPair;
class HttpPipelinedStream;
class ProxyInfo;
struct SSLConfig;

class NET_EXPORT_PRIVATE HttpPipelinedConnection {
 public:
  enum Feedback {
    OK,
    PIPELINE_SOCKET_ERROR,
    OLD_HTTP_VERSION,
    MUST_CLOSE_CONNECTION,
    AUTHENTICATION_REQUIRED,
  };

  class Delegate {
   public:
    
    
    
    virtual void OnPipelineHasCapacity(HttpPipelinedConnection* pipeline) = 0;

    
    
    virtual void OnPipelineFeedback(HttpPipelinedConnection* pipeline,
                                    Feedback feedback) = 0;
  };

  class Factory {
   public:
    virtual ~Factory() {}

    virtual HttpPipelinedConnection* CreateNewPipeline(
        ClientSocketHandle* connection,
        Delegate* delegate,
        const HostPortPair& origin,
        const SSLConfig& used_ssl_config,
        const ProxyInfo& used_proxy_info,
        const BoundNetLog& net_log,
        bool was_npn_negotiated,
        NextProto protocol_negotiated) = 0;
  };

  virtual ~HttpPipelinedConnection() {}

  
  virtual HttpPipelinedStream* CreateNewStream() = 0;

  
  virtual int depth() const = 0;

  
  
  virtual bool usable() const = 0;

  
  
  virtual bool active() const = 0;

  
  virtual const SSLConfig& used_ssl_config() const = 0;

  
  virtual const ProxyInfo& used_proxy_info() const = 0;

  
  virtual const BoundNetLog& net_log() const = 0;

  
  virtual bool was_npn_negotiated() const = 0;

  
  virtual NextProto protocol_negotiated() const = 0;
};

}  

#endif  
