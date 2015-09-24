// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2012, The Linux Foundation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_STREAM_FACTORY_IMPL_H_
#define NET_HTTP_HTTP_STREAM_FACTORY_IMPL_H_

#include <map>
#include <set>

#include "base/memory/ref_counted.h"
#include "net/base/host_port_pair.h"
#include "net/http/http_stream_factory.h"
#include "net/base/net_log.h"
#include "net/proxy/proxy_server.h"

namespace net {

class HttpNetworkSession;
class SpdySession;

class HttpStreamFactoryImpl : public HttpStreamFactory {
 public:
  explicit HttpStreamFactoryImpl(HttpNetworkSession* session);
  virtual ~HttpStreamFactoryImpl();

  
  virtual HttpStreamRequest* RequestStream(
      const HttpRequestInfo& info,
      const SSLConfig& ssl_config,
      HttpStreamRequest::Delegate* delegate,
      const BoundNetLog& net_log);

  virtual void PreconnectStreams(int num_streams,
                                 const HttpRequestInfo& info,
                                 const SSLConfig& ssl_config,
                                 const BoundNetLog& net_log);
  virtual int PreconnectStreams(int num_streams,
                                 const HttpRequestInfo& info,
                                 const SSLConfig& ssl_config,
                                 const BoundNetLog& net_log,
                                 CompletionCallback* callback);
  virtual void AddTLSIntolerantServer(const HostPortPair& server);
  virtual bool IsTLSIntolerantServer(const HostPortPair& server) const;

 private:
  class Request;
  class Job;

  typedef std::map<const Job*, CompletionCallback*> RequestCallbackMap;
  RequestCallbackMap request_callback_map_;

  typedef std::set<Request*> RequestSet;
  typedef std::map<HostPortProxyPair, RequestSet> SpdySessionRequestMap;

  bool GetAlternateProtocolRequestFor(const GURL& original_url,
                                      GURL* alternate_url) const;

  
  void OrphanJob(Job* job, const Request* request);

  
  
  
  void OnSpdySessionReady(scoped_refptr<SpdySession> spdy_session,
                          bool direct,
                          const SSLConfig& used_ssl_config,
                          const ProxyInfo& used_proxy_info,
                          bool was_npn_negotiated,
                          bool using_spdy,
                          const NetLog::Source& source);

  
  
  
  void OnBrokenAlternateProtocol(const Job*, const HostPortPair& origin);

  
  void OnOrphanedJobComplete(const Job* job);

  
  void OnPreconnectsComplete(const Job* job);

  
  virtual void OnPreconnectsCompleteInternal() {}

  HttpNetworkSession* const session_;

  std::set<HostPortPair> tls_intolerant_servers_;

  
  
  
  std::map<const Job*, Request*> request_map_;

  SpdySessionRequestMap spdy_session_request_map_;

  
  
  
  
  
  std::set<const Job*> orphaned_job_set_;

  
  
  
  std::set<const Job*> preconnect_job_set_;

  DISALLOW_COPY_AND_ASSIGN(HttpStreamFactoryImpl);
};

}  

#endif  
