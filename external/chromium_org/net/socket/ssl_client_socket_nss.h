// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_NSS_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_NSS_H_

#include <certt.h>
#include <keyt.h>
#include <nspr.h>
#include <nss.h>

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/nss_memio.h"
#include "net/cert/cert_verify_result.h"
#include "net/cert/ct_verify_result.h"
#include "net/cert/x509_certificate.h"
#include "net/socket/ssl_client_socket.h"
#include "net/ssl/server_bound_cert_service.h"
#include "net/ssl/ssl_config_service.h"

namespace base {
class SequencedTaskRunner;
}

namespace net {

class BoundNetLog;
class CertVerifier;
class CTVerifier;
class ClientSocketHandle;
class ServerBoundCertService;
class SingleRequestCertVerifier;
class TransportSecurityState;
class X509Certificate;

class SSLClientSocketNSS : public SSLClientSocket {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  SSLClientSocketNSS(base::SequencedTaskRunner* nss_task_runner,
                     scoped_ptr<ClientSocketHandle> transport_socket,
                     const HostPortPair& host_and_port,
                     const SSLConfig& ssl_config,
                     const SSLClientSocketContext& context);
  virtual ~SSLClientSocketNSS();

  
  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) OVERRIDE;
  virtual NextProtoStatus GetNextProto(std::string* proto,
                                       std::string* server_protos) OVERRIDE;

  
  virtual int ExportKeyingMaterial(const base::StringPiece& label,
                                   bool has_context,
                                   const base::StringPiece& context,
                                   unsigned char* out,
                                   unsigned int outlen) OVERRIDE;
  virtual int GetTLSUniqueChannelBinding(std::string* out) OVERRIDE;

  
  virtual int Connect(const CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(IPEndPoint* address) const OVERRIDE;
  virtual const BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool GetSSLInfo(SSLInfo* ssl_info) OVERRIDE;

  
  virtual int Read(IOBuffer* buf,
                   int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf,
                    int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;
  virtual ServerBoundCertService* GetServerBoundCertService() const OVERRIDE;

 private:
  
  
  class Core;

  enum State {
    STATE_NONE,
    STATE_HANDSHAKE,
    STATE_HANDSHAKE_COMPLETE,
    STATE_VERIFY_CERT,
    STATE_VERIFY_CERT_COMPLETE,
  };

  int Init();
  void InitCore();

  
  int InitializeSSLOptions();

  
  int InitializeSSLPeerName();

  void DoConnectCallback(int result);
  void OnHandshakeIOComplete(int result);

  int DoHandshakeLoop(int last_io_result);
  int DoHandshake();
  int DoHandshakeComplete(int result);
  int DoVerifyCert(int result);
  int DoVerifyCertComplete(int result);

  void VerifyCT();

  void LogConnectionTypeMetrics() const;

  
  
  void EnsureThreadIdAssigned() const;
  bool CalledOnValidThread() const;

  
  
  
  
  
  void AddSCTInfoToSSLInfo(SSLInfo* ssl_info) const;

  
  scoped_refptr<base::SequencedTaskRunner> nss_task_runner_;
  scoped_ptr<ClientSocketHandle> transport_;
  HostPortPair host_and_port_;
  SSLConfig ssl_config_;

  scoped_refptr<Core> core_;

  CompletionCallback user_connect_callback_;

  CertVerifyResult server_cert_verify_result_;
  HashValueVector side_pinned_public_keys_;

  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;

  
  ct::CTVerifyResult ct_verify_result_;
  CTVerifier* cert_transparency_verifier_;

  
  ServerBoundCertService* server_bound_cert_service_;

  
  
  
  const std::string ssl_session_cache_shard_;

  
  bool completed_handshake_;

  State next_handshake_state_;

  
  
  
  PRFileDesc* nss_fd_;

  BoundNetLog net_log_;

  base::TimeTicks start_cert_verification_time_;

  TransportSecurityState* transport_security_state_;

  
  
  
  mutable base::Lock lock_;
  
  
  mutable base::PlatformThreadId valid_thread_id_;
};

}  

#endif  
