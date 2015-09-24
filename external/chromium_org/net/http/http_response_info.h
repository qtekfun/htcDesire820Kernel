// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_RESPONSE_INFO_H_
#define NET_HTTP_HTTP_RESPONSE_INFO_H_

#include <string>

#include "base/time/time.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/http/http_vary_data.h"
#include "net/socket/next_proto.h"
#include "net/ssl/ssl_info.h"

class Pickle;

namespace net {

class AuthChallengeInfo;
class HttpResponseHeaders;
class IOBufferWithSize;
class SSLCertRequestInfo;

class NET_EXPORT HttpResponseInfo {
 public:
  
  
  
  
  
  
  
  enum ConnectionInfo {
    CONNECTION_INFO_UNKNOWN = 0,
    CONNECTION_INFO_HTTP1 = 1,
    CONNECTION_INFO_DEPRECATED_SPDY2 = 2,
    CONNECTION_INFO_SPDY3 = 3,
    CONNECTION_INFO_SPDY4A2 = 4,
    CONNECTION_INFO_QUIC1_SPDY3 = 5,
    CONNECTION_INFO_HTTP2_DRAFT_04 = 6,
    NUM_OF_CONNECTION_INFOS,
  };

  HttpResponseInfo();
  HttpResponseInfo(const HttpResponseInfo& rhs);
  ~HttpResponseInfo();
  HttpResponseInfo& operator=(const HttpResponseInfo& rhs);
  
  

  
  bool InitFromPickle(const Pickle& pickle, bool* response_truncated);

  
  void Persist(Pickle* pickle,
               bool skip_transient_headers,
               bool response_truncated) const;

  
  
  
  
  
  
  
  bool was_cached;

  
  
  
  bool server_data_unavailable;

  
  
  bool network_accessed;

  
  bool was_fetched_via_spdy;

  
  bool was_npn_negotiated;

  
  
  
  bool was_fetched_via_proxy;

  
  bool did_use_http_auth;

  
  
  
  
  
  
  
  HostPortPair socket_address;

  
  std::string npn_negotiated_protocol;

  
  ConnectionInfo connection_info;

  
  
  base::Time request_time;

  
  
  base::Time response_time;

  
  
  scoped_refptr<AuthChallengeInfo> auth_challenge;

  
  
  
  
  scoped_refptr<SSLCertRequestInfo> cert_request_info;

  
  SSLInfo ssl_info;

  
  scoped_refptr<HttpResponseHeaders> headers;

  
  HttpVaryData vary_data;

  
  scoped_refptr<IOBufferWithSize> metadata;

  static ConnectionInfo ConnectionInfoFromNextProto(NextProto next_proto);

  static std::string ConnectionInfoToString(ConnectionInfo connection_info);
};

}  

#endif  
