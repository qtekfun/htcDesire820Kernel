// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_POOL_MANAGER_H_
#define NET_SOCKET_CLIENT_SOCKET_POOL_MANAGER_H_

#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"
#include "net/http/http_network_session.h"

class GURL;

namespace base {
class Value;
}

namespace net {

typedef base::Callback<int(const AddressList&, const BoundNetLog& net_log)>
OnHostResolutionCallback;

class BoundNetLog;
class ClientSocketHandle;
class HostPortPair;
class HttpNetworkSession;
class HttpProxyClientSocketPool;
class HttpRequestHeaders;
class ProxyInfo;
class TransportClientSocketPool;
class SOCKSClientSocketPool;
class SSLClientSocketPool;

struct SSLConfig;

enum DefaultMaxValues { kDefaultMaxSocketsPerProxyServer = 32 };

class NET_EXPORT_PRIVATE ClientSocketPoolManager {
 public:
  ClientSocketPoolManager();
  virtual ~ClientSocketPoolManager();

  
  
  
  static int max_sockets_per_pool(HttpNetworkSession::SocketPoolType pool_type);
  static void set_max_sockets_per_pool(
      HttpNetworkSession::SocketPoolType pool_type,
      int socket_count);

  static int max_sockets_per_group(
      HttpNetworkSession::SocketPoolType pool_type);
  static void set_max_sockets_per_group(
      HttpNetworkSession::SocketPoolType pool_type,
      int socket_count);

  static int max_sockets_per_proxy_server(
      HttpNetworkSession::SocketPoolType pool_type);
  static void set_max_sockets_per_proxy_server(
      HttpNetworkSession::SocketPoolType pool_type,
      int socket_count);

  virtual void FlushSocketPoolsWithError(int error) = 0;
  virtual void CloseIdleSockets() = 0;
  virtual TransportClientSocketPool* GetTransportSocketPool() = 0;
  virtual SSLClientSocketPool* GetSSLSocketPool() = 0;
  virtual SOCKSClientSocketPool* GetSocketPoolForSOCKSProxy(
      const HostPortPair& socks_proxy) = 0;
  virtual HttpProxyClientSocketPool* GetSocketPoolForHTTPProxy(
      const HostPortPair& http_proxy) = 0;
  virtual SSLClientSocketPool* GetSocketPoolForSSLWithProxy(
      const HostPortPair& proxy_server) = 0;
  
  
  virtual base::Value* SocketPoolInfoToValue() const = 0;
};

int InitSocketHandleForHttpRequest(
    const GURL& request_url,
    const HttpRequestHeaders& request_extra_headers,
    int request_load_flags,
    RequestPriority request_priority,
    HttpNetworkSession* session,
    const ProxyInfo& proxy_info,
    bool force_spdy_over_ssl,
    bool want_spdy_over_npn,
    const SSLConfig& ssl_config_for_origin,
    const SSLConfig& ssl_config_for_proxy,
    PrivacyMode privacy_mode,
    const BoundNetLog& net_log,
    ClientSocketHandle* socket_handle,
    const OnHostResolutionCallback& resolution_callback,
    const CompletionCallback& callback);

int InitSocketHandleForWebSocketRequest(
    const GURL& request_url,
    const HttpRequestHeaders& request_extra_headers,
    int request_load_flags,
    RequestPriority request_priority,
    HttpNetworkSession* session,
    const ProxyInfo& proxy_info,
    bool force_spdy_over_ssl,
    bool want_spdy_over_npn,
    const SSLConfig& ssl_config_for_origin,
    const SSLConfig& ssl_config_for_proxy,
    PrivacyMode privacy_mode,
    const BoundNetLog& net_log,
    ClientSocketHandle* socket_handle,
    const OnHostResolutionCallback& resolution_callback,
    const CompletionCallback& callback);

NET_EXPORT int InitSocketHandleForRawConnect(
    const HostPortPair& host_port_pair,
    HttpNetworkSession* session,
    const ProxyInfo& proxy_info,
    const SSLConfig& ssl_config_for_origin,
    const SSLConfig& ssl_config_for_proxy,
    PrivacyMode privacy_mode,
    const BoundNetLog& net_log,
    ClientSocketHandle* socket_handle,
    const CompletionCallback& callback);

NET_EXPORT int InitSocketHandleForTlsConnect(
    const HostPortPair& host_port_pair,
    HttpNetworkSession* session,
    const ProxyInfo& proxy_info,
    const SSLConfig& ssl_config_for_origin,
    const SSLConfig& ssl_config_for_proxy,
    PrivacyMode privacy_mode,
    const BoundNetLog& net_log,
    ClientSocketHandle* socket_handle,
    const CompletionCallback& callback);

int PreconnectSocketsForHttpRequest(
    const GURL& request_url,
    const HttpRequestHeaders& request_extra_headers,
    int request_load_flags,
    RequestPriority request_priority,
    HttpNetworkSession* session,
    const ProxyInfo& proxy_info,
    bool force_spdy_over_ssl,
    bool want_spdy_over_npn,
    const SSLConfig& ssl_config_for_origin,
    const SSLConfig& ssl_config_for_proxy,
    PrivacyMode privacy_mode,
    const BoundNetLog& net_log,
    int num_preconnect_streams);

}  

#endif  
