// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_STREAM_FACTORY_H_
#define NET_QUIC_QUIC_STREAM_FACTORY_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "net/base/address_list.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/base/network_change_notifier.h"
#include "net/cert/cert_database.h"
#include "net/proxy/proxy_server.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_crypto_stream.h"
#include "net/quic/quic_http_stream.h"
#include "net/quic/quic_protocol.h"

namespace net {

class CertVerifier;
class ClientSocketFactory;
class HostResolver;
class HttpServerProperties;
class QuicClock;
class QuicClientSession;
class QuicConnectionHelper;
class QuicCryptoClientStreamFactory;
class QuicRandom;
class QuicStreamFactory;

namespace test {
class QuicStreamFactoryPeer;
}  

class NET_EXPORT_PRIVATE QuicStreamRequest {
 public:
  explicit QuicStreamRequest(QuicStreamFactory* factory);
  ~QuicStreamRequest();

  
  int Request(const HostPortProxyPair& host_port_proxy_pair,
              bool is_https,
              CertVerifier* cert_verifier,
              const BoundNetLog& net_log,
              const CompletionCallback& callback);

  void OnRequestComplete(int rv);

  scoped_ptr<QuicHttpStream> ReleaseStream();

  void set_stream(scoped_ptr<QuicHttpStream> stream);

  const BoundNetLog& net_log() const{
    return net_log_;
  }

 private:
  QuicStreamFactory* factory_;
  HostPortProxyPair host_port_proxy_pair_;
  bool is_https_;
  CertVerifier* cert_verifier_;
  BoundNetLog net_log_;
  CompletionCallback callback_;
  scoped_ptr<QuicHttpStream> stream_;

  DISALLOW_COPY_AND_ASSIGN(QuicStreamRequest);
};

class NET_EXPORT_PRIVATE QuicStreamFactory
    : public NetworkChangeNotifier::IPAddressObserver,
      public CertDatabase::Observer {
 public:
  QuicStreamFactory(
      HostResolver* host_resolver,
      ClientSocketFactory* client_socket_factory,
      base::WeakPtr<HttpServerProperties> http_server_properties,
      QuicCryptoClientStreamFactory* quic_crypto_client_stream_factory,
      QuicRandom* random_generator,
      QuicClock* clock,
      size_t max_packet_length);
  virtual ~QuicStreamFactory();

  
  
  
  
  
  
  
  int Create(const HostPortProxyPair& host_port_proxy_pair,
             bool is_https,
             CertVerifier* cert_verifier,
             const BoundNetLog& net_log,
             QuicStreamRequest* request);

  
  
  scoped_ptr<QuicHttpStream> CreateIfSessionExists(
      const HostPortProxyPair& host_port_proxy_pair,
      const BoundNetLog& net_log);

  
  void OnIdleSession(QuicClientSession* session);

  
  
  void OnSessionGoingAway(QuicClientSession* session);

  
  void OnSessionClosed(QuicClientSession* session);

  
  void CancelRequest(QuicStreamRequest* request);

  
  void CloseAllSessions(int error);

  base::Value* QuicStreamFactoryInfoToValue() const;

  

  
  
  virtual void OnIPAddressChanged() OVERRIDE;

  

  
  virtual void OnCertAdded(const X509Certificate* cert) OVERRIDE;
  virtual void OnCACertChanged(const X509Certificate* cert) OVERRIDE;

  bool require_confirmation() const { return require_confirmation_; }

  void set_require_confirmation(bool require_confirmation) {
    require_confirmation_ = require_confirmation;
  }

  QuicConnectionHelper* helper() { return helper_.get(); }

 private:
  class Job;
  friend class test::QuicStreamFactoryPeer;

  typedef std::map<HostPortProxyPair, QuicClientSession*> SessionMap;
  typedef std::set<HostPortProxyPair> AliasSet;
  typedef std::map<QuicClientSession*, AliasSet> SessionAliasMap;
  typedef std::set<QuicClientSession*> SessionSet;
  typedef std::map<HostPortProxyPair, QuicCryptoClientConfig*> CryptoConfigMap;
  typedef std::map<HostPortPair, HostPortProxyPair> CanonicalHostMap;
  typedef std::map<HostPortProxyPair, Job*> JobMap;
  typedef std::map<QuicStreamRequest*, Job*> RequestMap;
  typedef std::set<QuicStreamRequest*> RequestSet;
  typedef std::map<Job*, RequestSet> JobRequestsMap;

  void OnJobComplete(Job* job, int rv);
  bool HasActiveSession(const HostPortProxyPair& host_port_proxy_pair);
  bool HasActiveJob(const HostPortProxyPair& host_port_proxy_pair);
  int CreateSession(const HostPortProxyPair& host_port_proxy_pair,
                    bool is_https,
                    CertVerifier* cert_verifier,
                    const AddressList& address_list,
                    const BoundNetLog& net_log,
                    QuicClientSession** session);
  void ActivateSession(const HostPortProxyPair& host_port_proxy_pair,
                       QuicClientSession* session);

  QuicCryptoClientConfig* GetOrCreateCryptoConfig(
      const HostPortProxyPair& host_port_proxy_pair);

  
  
  
  
  void PopulateFromCanonicalConfig(
      const HostPortProxyPair& host_port_proxy_pair,
      QuicCryptoClientConfig* crypto_config);

  bool require_confirmation_;
  HostResolver* host_resolver_;
  ClientSocketFactory* client_socket_factory_;
  base::WeakPtr<HttpServerProperties> http_server_properties_;
  QuicCryptoClientStreamFactory* quic_crypto_client_stream_factory_;
  QuicRandom* random_generator_;
  scoped_ptr<QuicClock> clock_;
  const size_t max_packet_length_;

  
  scoped_ptr<QuicConnectionHelper> helper_;

  
  SessionSet all_sessions_;
  
  
  SessionMap active_sessions_;
  SessionAliasMap session_aliases_;

  
  
  
  
  CryptoConfigMap all_crypto_configs_;

  
  
  
  
  CanonicalHostMap canonical_hostname_to_origin_map_;

  
  
  std::vector<std::string> cannoncial_suffixes_;

  QuicConfig config_;

  JobMap active_jobs_;
  JobRequestsMap job_requests_map_;
  RequestMap active_requests_;

  base::WeakPtrFactory<QuicStreamFactory> weak_factory_;

  
  
  
  
  
  
  uint64 port_seed_;

  DISALLOW_COPY_AND_ASSIGN(QuicStreamFactory);
};

}  

#endif  
