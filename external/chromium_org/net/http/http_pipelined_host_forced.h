// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_PIPELINED_HOST_FORCED_H_
#define NET_HTTP_HTTP_PIPELINED_HOST_FORCED_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/http/http_pipelined_connection.h"
#include "net/http/http_pipelined_host.h"
#include "net/http/http_pipelined_host_capability.h"

namespace base {
class Value;
}

namespace net {

class BoundNetLog;
class ClientSocketHandle;
class HttpPipelinedStream;
class ProxyInfo;
struct SSLConfig;

class NET_EXPORT_PRIVATE HttpPipelinedHostForced
    : public HttpPipelinedHost,
      public HttpPipelinedConnection::Delegate {
 public:
  HttpPipelinedHostForced(HttpPipelinedHost::Delegate* delegate,
                          const Key& key,
                          HttpPipelinedConnection::Factory* factory);
  virtual ~HttpPipelinedHostForced();

  
  virtual HttpPipelinedStream* CreateStreamOnNewPipeline(
      ClientSocketHandle* connection,
      const SSLConfig& used_ssl_config,
      const ProxyInfo& used_proxy_info,
      const BoundNetLog& net_log,
      bool was_npn_negotiated,
      NextProto protocol_negotiated) OVERRIDE;

  virtual HttpPipelinedStream* CreateStreamOnExistingPipeline() OVERRIDE;

  virtual bool IsExistingPipelineAvailable() const OVERRIDE;

  virtual const Key& GetKey() const OVERRIDE;

  virtual base::Value* PipelineInfoToValue() const OVERRIDE;

  

  virtual void OnPipelineHasCapacity(
      HttpPipelinedConnection* pipeline) OVERRIDE;

  virtual void OnPipelineFeedback(
      HttpPipelinedConnection* pipeline,
      HttpPipelinedConnection::Feedback feedback) OVERRIDE;

 private:
  
  
  void OnPipelineEmpty(HttpPipelinedConnection* pipeline);

  HttpPipelinedHost::Delegate* delegate_;
  const Key key_;
  scoped_ptr<HttpPipelinedConnection> pipeline_;
  scoped_ptr<HttpPipelinedConnection::Factory> factory_;

  DISALLOW_COPY_AND_ASSIGN(HttpPipelinedHostForced);
};

}  

#endif  
