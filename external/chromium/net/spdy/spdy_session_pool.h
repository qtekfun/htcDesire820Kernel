// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SESSION_POOL_H_
#define NET_SPDY_SPDY_SESSION_POOL_H_
#pragma once

#include <map>
#include <list>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/cert_database.h"
#include "net/base/host_port_pair.h"
#include "net/base/ip_endpoint.h"
#include "net/base/net_errors.h"
#include "net/base/network_change_notifier.h"
#include "net/base/ssl_config_service.h"
#include "net/proxy/proxy_config.h"
#include "net/proxy/proxy_server.h"
#include "net/spdy/spdy_settings_storage.h"

namespace net {

class AddressList;
class BoundNetLog;
class ClientSocketHandle;
class HostResolver;
class HttpNetworkSession;
class SpdySession;

class SpdySessionPool
    : public NetworkChangeNotifier::IPAddressObserver,
      public SSLConfigService::Observer,
      public CertDatabase::Observer {
 public:
  explicit SpdySessionPool(HostResolver* host_resolver,
                           SSLConfigService* ssl_config_service);
  virtual ~SpdySessionPool();

  
  
  scoped_refptr<SpdySession> Get(
      const HostPortProxyPair& host_port_proxy_pair,
      const BoundNetLog& net_log);

  
  static void set_max_sessions_per_domain(int max) {
    if (max >= 1)
      g_max_sessions_per_domain = max;
  }

  
  
  
  
  
  
  
  
  
  
  net::Error GetSpdySessionFromSocket(
      const HostPortProxyPair& host_port_proxy_pair,
      ClientSocketHandle* connection,
      const BoundNetLog& net_log,
      int certificate_error_code,
      scoped_refptr<SpdySession>* spdy_session,
      bool is_secure);

  
  
  bool HasSession(const HostPortProxyPair& host_port_proxy_pair) const;

  
  
  void CloseAllSessions();
  
  
  void CloseCurrentSessions();
  
  void CloseIdleSessions();

  
  
  void Remove(const scoped_refptr<SpdySession>& session);

  
  
  Value* SpdySessionPoolInfoToValue() const;

  SpdySettingsStorage* mutable_spdy_settings() { return &spdy_settings_; }
  const SpdySettingsStorage& spdy_settings() const { return spdy_settings_; }

  

  
  
  
  virtual void OnIPAddressChanged();

  

  
  virtual void OnSSLConfigChanged();

  
  static void ForceSingleDomain() { g_force_single_domain = true; }

  
  
  static void enable_ip_pooling(bool value) { g_enable_ip_pooling = value; }

  
  virtual void OnUserCertAdded(const X509Certificate* cert);
  virtual void OnCertTrustChanged(const X509Certificate* cert);

 private:
  friend class SpdySessionPoolPeer;  
  friend class SpdyNetworkTransactionTest;  
  FRIEND_TEST_ALL_PREFIXES(SpdyNetworkTransactionTest, WindowUpdateOverflow);

  typedef std::list<scoped_refptr<SpdySession> > SpdySessionList;
  typedef std::map<HostPortProxyPair, SpdySessionList*> SpdySessionsMap;
  typedef std::map<IPEndPoint, HostPortProxyPair> SpdyAliasMap;

  scoped_refptr<SpdySession> GetExistingSession(
      SpdySessionList* list,
      const BoundNetLog& net_log) const;
  scoped_refptr<SpdySession> GetFromAlias(
      const HostPortProxyPair& host_port_proxy_pair,
      const BoundNetLog& net_log,
      bool record_histograms) const;

  
  const HostPortProxyPair& NormalizeListPair(
      const HostPortProxyPair& host_port_proxy_pair) const;
  SpdySessionList* AddSessionList(
      const HostPortProxyPair& host_port_proxy_pair);
  SpdySessionList* GetSessionList(
      const HostPortProxyPair& host_port_proxy_pair) const;
  void RemoveSessionList(const HostPortProxyPair& host_port_proxy_pair);

  
  
  bool LookupAddresses(const HostPortProxyPair& pair,
                       AddressList* addresses) const;

  
  void AddAliases(const AddressList& addresses, const HostPortProxyPair& pair);

  
  void RemoveAliases(const HostPortProxyPair& pair);

  SpdySettingsStorage spdy_settings_;

  
  SpdySessionsMap sessions_;
  
  SpdyAliasMap aliases_;

  static size_t g_max_sessions_per_domain;
  static bool g_force_single_domain;
  static bool g_enable_ip_pooling;

  const scoped_refptr<SSLConfigService> ssl_config_service_;
  HostResolver* resolver_;

  DISALLOW_COPY_AND_ASSIGN(SpdySessionPool);
};

}  

#endif  
