// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_PROXY_CLIENT_SOCKET_H_
#define NET_HTTP_PROXY_CLIENT_SOCKET_H_

#include <string>

#include "net/socket/ssl_client_socket.h"
#include "net/socket/stream_socket.h"

class GURL;

namespace net {

class HostPortPair;
class HttpAuthController;
class HttpStream;
class HttpResponseInfo;
class HttpRequestHeaders;
struct HttpRequestInfo;
class HttpAuthController;

class NET_EXPORT_PRIVATE ProxyClientSocket : public StreamSocket {
 public:
  ProxyClientSocket() {}
  virtual ~ProxyClientSocket() {}

  
  
  virtual const HttpResponseInfo* GetConnectResponseInfo() const = 0;

  
  
  virtual HttpStream* CreateConnectResponseStream() = 0;

  
  
  virtual const scoped_refptr<HttpAuthController>& GetAuthController() const
      = 0;

  
  
  
  
  
  virtual int RestartWithAuth(const CompletionCallback& callback) = 0;

  
  virtual bool IsUsingSpdy() const = 0;

  
  virtual NextProto GetProtocolNegotiated() const = 0;

 protected:
  
  
  
  static void BuildTunnelRequest(const HttpRequestInfo& request_info,
                                 const HttpRequestHeaders& auth_headers,
                                 const HostPortPair& endpoint,
                                 std::string* request_line,
                                 HttpRequestHeaders* request_headers);

  
  
  static int HandleProxyAuthChallenge(HttpAuthController* auth,
                                      HttpResponseInfo* response,
                                      const BoundNetLog& net_log);

  
  static void LogBlockedTunnelResponse(int http_response_code,
                                       const GURL& url,
                                       bool is_https_proxy);

  
  
  
  
  
  static bool SanitizeProxyRedirect(HttpResponseInfo* response,
                                    const GURL& url);

 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyClientSocket);
};

}  

#endif  
