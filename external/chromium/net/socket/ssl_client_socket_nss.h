// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_NSS_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_NSS_H_
#pragma once

#include <certt.h>
#include <keyt.h>
#include <nspr.h>
#include <nss.h>

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"
#include "base/time.h"
#include "base/timer.h"
#include "net/base/cert_verify_result.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/base/nss_memio.h"
#include "net/base/ssl_config_service.h"
#include "net/base/x509_certificate.h"
#include "net/socket/ssl_client_socket.h"

namespace net {

class BoundNetLog;
class CertVerifier;
class ClientSocketHandle;
class DnsCertProvenanceChecker;
class SingleRequestCertVerifier;
class SSLHostInfo;
class X509Certificate;

class SSLClientSocketNSS : public SSLClientSocket {
 public:
  
  
  
  
  
  
  SSLClientSocketNSS(ClientSocketHandle* transport_socket,
                     const HostPortPair& host_and_port,
                     const SSLConfig& ssl_config,
                     SSLHostInfo* ssl_host_info,
                     CertVerifier* cert_verifier,
                     DnsCertProvenanceChecker* dnsrr_resolver);
  ~SSLClientSocketNSS();

  
  static void ClearSessionCache();

  
  virtual void GetSSLInfo(SSLInfo* ssl_info);
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);
  virtual NextProtoStatus GetNextProto(std::string* proto);
  virtual void UseDNSSEC(DNSSECProvider* provider);

  
  virtual int Connect(CompletionCallback* callback
#ifdef ANDROID
                      , bool wait_for_connect
#endif
                     );
  virtual void Disconnect();
  virtual bool IsConnected() const;
  virtual bool IsConnectedAndIdle() const;
  virtual int GetPeerAddress(AddressList* address) const;
  virtual int GetLocalAddress(IPEndPoint* address) const;
  virtual const BoundNetLog& NetLog() const;
  virtual void SetSubresourceSpeculation();
  virtual void SetOmniboxSpeculation();
  virtual bool WasEverUsed() const;
  virtual bool UsingTCPFastOpen() const;

  
  virtual int Read(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual int Write(IOBuffer* buf, int buf_len, CompletionCallback* callback);
  virtual bool SetReceiveBufferSize(int32 size);
  virtual bool SetSendBufferSize(int32 size);

 private:
  enum State {
    STATE_NONE,
    STATE_HANDSHAKE,
    STATE_VERIFY_DNSSEC,
    STATE_VERIFY_DNSSEC_COMPLETE,
    STATE_VERIFY_CERT,
    STATE_VERIFY_CERT_COMPLETE,
  };

  int Init();

  
  int InitializeSSLOptions();

  
  int InitializeSSLPeerName();

#if defined(OS_MACOSX) || defined(OS_WIN)
  
  static X509Certificate::OSCertHandle CreateOSCert(const SECItem& der_cert);
#endif
  X509Certificate* UpdateServerCert();
  void UpdateConnectionStatus();
  void DoReadCallback(int result);
  void DoWriteCallback(int result);
  void DoConnectCallback(int result);
  void OnHandshakeIOComplete(int result);
  void OnSendComplete(int result);
  void OnRecvComplete(int result);

  int DoHandshakeLoop(int last_io_result);
  int DoReadLoop(int result);
  int DoWriteLoop(int result);

  int DoHandshake();

  int DoVerifyDNSSEC(int result);
  int DoVerifyDNSSECComplete(int result);
  int DoVerifyCert(int result);
  int DoVerifyCertComplete(int result);
  int DoPayloadRead();
  int DoPayloadWrite();
  void LogConnectionTypeMetrics() const;
  void SaveSSLHostInfo();
  void UncorkAfterTimeout();

  bool DoTransportIO();
  int BufferSend(void);
  void BufferSendComplete(int result);
  int BufferRecv(void);
  void BufferRecvComplete(int result);

  
  
  int HandleNSSError(PRErrorCode error, bool handshake_error);

  
  
  static SECStatus OwnAuthCertHandler(void* arg, PRFileDesc* socket,
                                      PRBool checksig, PRBool is_server);
  
#if defined(NSS_PLATFORM_CLIENT_AUTH)
  static SECStatus PlatformClientAuthHandler(void* arg,
                                             PRFileDesc* socket,
                                             CERTDistNames* ca_names,
                                             CERTCertList** result_certs,
                                             void** result_private_key);
#else
  static SECStatus ClientAuthHandler(void* arg,
                                     PRFileDesc* socket,
                                     CERTDistNames* ca_names,
                                     CERTCertificate** result_certificate,
                                     SECKEYPrivateKey** result_private_key);
#endif
  
  
  static void HandshakeCallback(PRFileDesc* socket, void* arg);

  
  
  void EnsureThreadIdAssigned() const;
  bool CalledOnValidThread() const;

  CompletionCallbackImpl<SSLClientSocketNSS> buffer_send_callback_;
  CompletionCallbackImpl<SSLClientSocketNSS> buffer_recv_callback_;
  bool transport_send_busy_;
  bool transport_recv_busy_;
  
  
  bool corked_;
  
  
  base::OneShotTimer<SSLClientSocketNSS> uncork_timer_;
  scoped_refptr<IOBuffer> recv_buffer_;

  CompletionCallbackImpl<SSLClientSocketNSS> handshake_io_callback_;
  scoped_ptr<ClientSocketHandle> transport_;
  HostPortPair host_and_port_;
  SSLConfig ssl_config_;

  CompletionCallback* user_connect_callback_;
  CompletionCallback* user_read_callback_;
  CompletionCallback* user_write_callback_;

  
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  scoped_refptr<IOBuffer> user_write_buf_;
  int user_write_buf_len_;

  
  
  
  scoped_refptr<X509Certificate> server_cert_;
  CERTCertificate* server_cert_nss_;
  
  
  
  const CertVerifyResult* server_cert_verify_result_;
  CertVerifyResult local_server_cert_verify_result_;
  int ssl_connection_status_;

  
  
  std::vector<scoped_refptr<X509Certificate> > client_certs_;
  bool client_auth_cert_needed_;

  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;

  
  bool handshake_callback_called_;

  
  bool completed_handshake_;

  
  
  bool eset_mitm_detected_;

  
  
  bool predicted_cert_chain_correct_;

  
  bool peername_initialized_;

  
  DNSSECProvider* dnssec_provider_;
  
  base::Time dnssec_wait_start_time_;

  State next_handshake_state_;

  
  PRFileDesc* nss_fd_;

  
  memio_Private* nss_bufs_;

  BoundNetLog net_log_;

  base::TimeTicks start_cert_verification_time_;

  scoped_ptr<SSLHostInfo> ssl_host_info_;
  DnsCertProvenanceChecker* const dns_cert_checker_;

  
  
  
  mutable base::Lock lock_;
  
  
  mutable base::PlatformThreadId valid_thread_id_;
};

}  

#endif  
