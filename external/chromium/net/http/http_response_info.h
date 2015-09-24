// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_HTTP_RESPONSE_INFO_H_
#define NET_HTTP_HTTP_RESPONSE_INFO_H_
#pragma once

#include "base/time.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/base/ssl_info.h"
#include "net/http/http_vary_data.h"

class Pickle;

namespace net {

class AuthChallengeInfo;
class HttpResponseHeaders;
class IOBufferWithSize;
class SSLCertRequestInfo;

class NET_EXPORT HttpResponseInfo {
 public:
  HttpResponseInfo();
  HttpResponseInfo(const HttpResponseInfo& rhs);
  ~HttpResponseInfo();
  HttpResponseInfo& operator=(const HttpResponseInfo& rhs);
  
  

  
  bool InitFromPickle(const Pickle& pickle, bool* response_truncated);

  
  void Persist(Pickle* pickle,
               bool skip_transient_headers,
               bool response_truncated) const;

  
  
  
  
  
  
  
  bool was_cached;

  
  bool was_fetched_via_spdy;

  
  bool was_npn_negotiated;

  
  
  
  bool was_fetched_via_proxy;

  
  
  
  
  
  
  
  HostPortPair socket_address;

  
  
  base::Time request_time;

  
  
  base::Time response_time;

  
  
  scoped_refptr<AuthChallengeInfo> auth_challenge;

  
  
  
  
  scoped_refptr<SSLCertRequestInfo> cert_request_info;

  
  SSLInfo ssl_info;

  
  scoped_refptr<HttpResponseHeaders> headers;

  
  HttpVaryData vary_data;

  
  scoped_refptr<IOBufferWithSize> metadata;
};

}  

#endif  
