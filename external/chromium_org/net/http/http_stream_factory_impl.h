// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_IMPL_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_IMPL_H_

#include <map>
#include <set>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/http/http_pipelined_host_pool.h"
#include "net/http/http_stream_factory.h"
#include "net/proxy/proxy_server.h"
#include "net/socket/ssl_client_socket.h"
#include "net/spdy/spdy_session_key.h"

namespace net {

class HttpNetworkSession;
class HttpPipelinedHost;
class SpdySession;

class NET_EXPORT_PRIVATE HttpStreamFactoryImpl :
    public HttpStreamFactory,
    public HttpPipelinedHostPool::Delegate {
 public:
  
  
  
  HttpStreamFactoryImpl(HttpNetworkSession* session, bool for_websockets);
  virtual ~HttpStreamFactoryImpl();

  
  virtual HttpStreamRequest* RequestStream(
      const HttpRequestInfo& info,
      RequestPriority priority,
      const SSLConfig& server_ssl_config,
      const SSLConfig& proxy_ssl_config,
      HttpStreamRequest::Delegate* delegate,
      const BoundNetLog& net_log) OVERRIDE;

  virtual HttpStreamRequest* RequestWebSocketHandshakeStream(
      const HttpRequestInfo& info,
      RequestPriority priority,
      const SSLConfig& server_ssl_config,
      const SSLConfig& proxy_ssl_config,
      HttpStreamRequest::Delegate* delegate,
      WebSocketHandshakeStreamBase::CreateHelper* create_helper,
      const BoundNetLog& net_log) OVERRIDE;

  virtual void PreconnectStreams(int num_streams,
                                 const HttpRequestInfo& info,
                                 RequestPriority priority,
                                 const SSLConfig& server_ssl_config,
                                 const SSLConfig& proxy_ssl_config) OVERRIDE;
  virtual base::Value* PipelineInfoToValue() const OVERRIDE;
  virtual const HostMappingRules* GetHostMappingRules() const OVERRIDE;

  
  virtual void OnHttpPipelinedHostHasAdditionalCapacity(
      HttpPipelinedHost* host) OVERRIDE;

  size_t num_orphaned_jobs() const { return orphaned_job_set_.size(); }

 private:
  FRIEND_TEST_ALL_PREFIXES(HttpStreamFactoryImplRequestTest, SetPriority);

  class NET_EXPORT_PRIVATE Request;
  class NET_EXPORT_PRIVATE Job;

  typedef std::set<Request*> RequestSet;
  typedef std::vector<Request*> RequestVector;
  typedef std::map<SpdySessionKey, RequestSet> SpdySessionRequestMap;
  typedef std::map<HttpPipelinedHost::Key,
                   RequestVector> HttpPipeliningRequestMap;

  HttpStreamRequest* RequestStreamInternal(
      const HttpRequestInfo& info,
      RequestPriority priority,
      const SSLConfig& server_ssl_config,
      const SSLConfig& proxy_ssl_config,
      HttpStreamRequest::Delegate* delegate,
      WebSocketHandshakeStreamBase::CreateHelper* create_helper,
      const BoundNetLog& net_log);

  PortAlternateProtocolPair GetAlternateProtocolRequestFor(
      const GURL& original_url,
      GURL* alternate_url) const;

  
  void OrphanJob(Job* job, const Request* request);

  
  
  
  void OnNewSpdySessionReady(const base::WeakPtr<SpdySession>& spdy_session,
                             bool direct,
                             const SSLConfig& used_ssl_config,
                             const ProxyInfo& used_proxy_info,
                             bool was_npn_negotiated,
                             NextProto protocol_negotiated,
                             bool using_spdy,
                             const BoundNetLog& net_log);

  
  
  
  void OnBrokenAlternateProtocol(const Job*, const HostPortPair& origin);

  
  void OnOrphanedJobComplete(const Job* job);

  
  void OnPreconnectsComplete(const Job* job);

  
  virtual void OnPreconnectsCompleteInternal() {}

  void AbortPipelinedRequestsWithKey(const Job* job,
                                     const HttpPipelinedHost::Key& key,
                                     int status,
                                     const SSLConfig& used_ssl_config);

  HttpNetworkSession* const session_;

  
  
  
  std::map<const Job*, Request*> request_map_;

  SpdySessionRequestMap spdy_session_request_map_;
  HttpPipeliningRequestMap http_pipelining_request_map_;

  HttpPipelinedHostPool http_pipelined_host_pool_;

  
  
  
  
  
  std::set<const Job*> orphaned_job_set_;

  
  
  
  std::set<const Job*> preconnect_job_set_;

  const bool for_websockets_;
  DISALLOW_COPY_AND_ASSIGN(HttpStreamFactoryImpl);
};

}  

#endif  
