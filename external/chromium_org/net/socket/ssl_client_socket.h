// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "net/base/completion_callback.h"
#include "net/base/load_flags.h"
#include "net/base/net_errors.h"
#include "net/socket/ssl_socket.h"
#include "net/socket/stream_socket.h"

namespace net {

class CertVerifier;
class CTVerifier;
class ServerBoundCertService;
class SSLCertRequestInfo;
struct SSLConfig;
class SSLInfo;
class TransportSecurityState;

struct SSLClientSocketContext {
  SSLClientSocketContext()
      : cert_verifier(NULL),
        server_bound_cert_service(NULL),
        transport_security_state(NULL),
        cert_transparency_verifier(NULL) {}

  SSLClientSocketContext(CertVerifier* cert_verifier_arg,
                         ServerBoundCertService* server_bound_cert_service_arg,
                         TransportSecurityState* transport_security_state_arg,
                         CTVerifier* cert_transparency_verifier_arg,
                         const std::string& ssl_session_cache_shard_arg)
      : cert_verifier(cert_verifier_arg),
        server_bound_cert_service(server_bound_cert_service_arg),
        transport_security_state(transport_security_state_arg),
        cert_transparency_verifier(cert_transparency_verifier_arg),
        ssl_session_cache_shard(ssl_session_cache_shard_arg) {}

  CertVerifier* cert_verifier;
  ServerBoundCertService* server_bound_cert_service;
  TransportSecurityState* transport_security_state;
  CTVerifier* cert_transparency_verifier;
  
  
  
  const std::string ssl_session_cache_shard;
};

class NET_EXPORT SSLClientSocket : public SSLSocket {
 public:
  SSLClientSocket();

  
  
  
  enum NextProtoStatus {
    

    kNextProtoUnsupported = 0,  
    kNextProtoNegotiated = 1,   
    kNextProtoNoOverlap = 2,    
                                
  };

  
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual NextProto GetNegotiatedProtocol() const OVERRIDE;

  
  
  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) = 0;

  
  
  
  
  
  
  
  
  virtual NextProtoStatus GetNextProto(std::string* proto,
                                       std::string* server_protos) = 0;

  static NextProto NextProtoFromString(const std::string& proto_string);

  static const char* NextProtoToString(NextProto next_proto);

  static const char* NextProtoStatusToString(const NextProtoStatus status);

  
  
  static std::string ServerProtosToString(const std::string& server_protos);

  static bool IgnoreCertError(int error, int load_flags);

  
  
  static void ClearSessionCache();

  virtual bool set_was_npn_negotiated(bool negotiated);

  virtual bool was_spdy_negotiated() const;

  virtual bool set_was_spdy_negotiated(bool negotiated);

  virtual void set_protocol_negotiated(NextProto protocol_negotiated);

  
  
  virtual ServerBoundCertService* GetServerBoundCertService() const = 0;

  
  
  
  
  
  
  virtual bool WasChannelIDSent() const;

 protected:
  virtual void set_channel_id_sent(bool channel_id_sent);

  virtual void set_signed_cert_timestamps_received(
      bool signed_cert_timestamps_received);

  virtual void set_stapled_ocsp_response_received(
      bool stapled_ocsp_response_received);

  
  
  static void RecordChannelIDSupport(
      ServerBoundCertService* server_bound_cert_service,
      bool negotiated_channel_id,
      bool channel_id_enabled,
      bool supports_ecc);

  
  static bool IsChannelIDEnabled(
      const SSLConfig& ssl_config,
      ServerBoundCertService* server_bound_cert_service);

 private:
  
  FRIEND_TEST_ALL_PREFIXES(SSLClientSocketTest,
                           ConnectSignedCertTimestampsEnabledTLSExtension);
  FRIEND_TEST_ALL_PREFIXES(SSLClientSocketTest,
                           ConnectSignedCertTimestampsEnabledOCSP);
  FRIEND_TEST_ALL_PREFIXES(SSLClientSocketTest,
                           ConnectSignedCertTimestampsDisabled);

  
  bool was_npn_negotiated_;
  
  bool was_spdy_negotiated_;
  
  NextProto protocol_negotiated_;
  
  bool channel_id_sent_;
  
  bool signed_cert_timestamps_received_;
  
  bool stapled_ocsp_response_received_;
};

}  

#endif  
