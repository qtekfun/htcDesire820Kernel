// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SESSION_POOL_H_
#define NET_SPDY_SPDY_SESSION_POOL_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "net/base/host_port_pair.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_errors.h"
#include "net/base/net_export.h"
#include "net/base/network_change_notifier.h"
#include "net/cert/cert_database.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_server.h"
#include "net/socket/next_proto.h"
#include "net/spdy/spdy_session_key.h"
#include "net/ssl/ssl_config_service.h"

namespace net {

class AddressList;
class BoundNetLog;
class ClientSocketHandle;
class HostResolver;
class HttpServerProperties;
class SpdySession;

class NET_EXPORT SpdySessionPool
    : public NetworkChangeNotifier::IPAddressObserver,
      public SSLConfigService::Observer,
      public CertDatabase::Observer {
 public:
  typedef base::TimeTicks (*TimeFunc)(void);

  
  
  
  SpdySessionPool(
      HostResolver* host_resolver,
      SSLConfigService* ssl_config_service,
      const base::WeakPtr<HttpServerProperties>& http_server_properties,
      bool force_single_domain,
      bool enable_ip_pooling,
      bool enable_compression,
      bool enable_ping_based_connection_checking,
      NextProto default_protocol,
      size_t stream_initial_recv_window_size,
      size_t initial_max_concurrent_streams,
      size_t max_concurrent_streams_limit,
      SpdySessionPool::TimeFunc time_func,
      const std::string& trusted_spdy_proxy);
  virtual ~SpdySessionPool();

  
  
  
  
  
  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  net::Error CreateAvailableSessionFromSocket(
      const SpdySessionKey& key,
      scoped_ptr<ClientSocketHandle> connection,
      const BoundNetLog& net_log,
      int certificate_error_code,
      base::WeakPtr<SpdySession>* available_session,
      bool is_secure);

  
  base::WeakPtr<SpdySession> FindAvailableSession(const SpdySessionKey& key,
                                                  const BoundNetLog& net_log);

  
  
  
  void MakeSessionUnavailable(
      const base::WeakPtr<SpdySession>& available_session);

  
  
  void RemoveUnavailableSession(
      const base::WeakPtr<SpdySession>& unavailable_session);

  
  
  
  void CloseCurrentSessions(net::Error error);

  
  
  
  void CloseCurrentIdleSessions();

  
  
  void CloseAllSessions();

  
  
  base::Value* SpdySessionPoolInfoToValue() const;

  base::WeakPtr<HttpServerProperties> http_server_properties() {
    return http_server_properties_;
  }

  

  
  
  
  virtual void OnIPAddressChanged() OVERRIDE;

  

  
  virtual void OnSSLConfigChanged() OVERRIDE;

  

  
  
  virtual void OnCertAdded(const X509Certificate* cert) OVERRIDE;
  virtual void OnCACertChanged(const X509Certificate* cert) OVERRIDE;

 private:
  friend class SpdySessionPoolPeer;  

  typedef std::set<SpdySession*> SessionSet;
  typedef std::vector<base::WeakPtr<SpdySession> > WeakSessionList;
  typedef std::map<SpdySessionKey, base::WeakPtr<SpdySession> >
      AvailableSessionMap;
  typedef std::map<IPEndPoint, SpdySessionKey> AliasMap;

  
  bool IsSessionAvailable(const base::WeakPtr<SpdySession>& session) const;

  
  
  const SpdySessionKey& NormalizeListKey(const SpdySessionKey& key) const;

  
  
  void MapKeyToAvailableSession(const SpdySessionKey& key,
                                const base::WeakPtr<SpdySession>& session);

  
  
  AvailableSessionMap::iterator LookupAvailableSessionByKey(
      const SpdySessionKey& key);

  
  void UnmapKey(const SpdySessionKey& key);

  
  void RemoveAliases(const SpdySessionKey& key);

  
  
  WeakSessionList GetCurrentSessions() const;

  
  
  
  void CloseCurrentSessionsHelper(
      Error error,
      const std::string& description,
      bool idle_only);

  const base::WeakPtr<HttpServerProperties> http_server_properties_;

  
  
  
  
  SessionSet sessions_;

  
  
  AvailableSessionMap available_sessions_;

  
  AliasMap aliases_;

  static bool g_force_single_domain;

  const scoped_refptr<SSLConfigService> ssl_config_service_;
  HostResolver* const resolver_;

  
  bool verify_domain_authentication_;
  bool enable_sending_initial_data_;
  bool force_single_domain_;
  bool enable_ip_pooling_;
  bool enable_compression_;
  bool enable_ping_based_connection_checking_;
  const NextProto default_protocol_;
  size_t stream_initial_recv_window_size_;
  size_t initial_max_concurrent_streams_;
  size_t max_concurrent_streams_limit_;
  TimeFunc time_func_;

  
  
  HostPortPair trusted_spdy_proxy_;

  DISALLOW_COPY_AND_ASSIGN(SpdySessionPool);
};

}  

#endif  
