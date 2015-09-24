// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_OPENSSL_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_OPENSSL_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/cert/cert_verify_result.h"
#include "net/socket/client_socket_handle.h"
#include "net/socket/ssl_client_socket.h"
#include "net/ssl/server_bound_cert_service.h"
#include "net/ssl/ssl_config_service.h"

typedef struct bio_st BIO;
typedef struct evp_pkey_st EVP_PKEY;
typedef struct ssl_st SSL;
typedef struct x509_st X509;

namespace net {

class CertVerifier;
class SingleRequestCertVerifier;
class SSLCertRequestInfo;
class SSLInfo;

class SSLClientSocketOpenSSL : public SSLClientSocket {
 public:
  
  
  
  
  SSLClientSocketOpenSSL(scoped_ptr<ClientSocketHandle> transport_socket,
                         const HostPortPair& host_and_port,
                         const SSLConfig& ssl_config,
                         const SSLClientSocketContext& context);
  virtual ~SSLClientSocketOpenSSL();

  const HostPortPair& host_and_port() const { return host_and_port_; }
  const std::string& ssl_session_cache_shard() const {
    return ssl_session_cache_shard_;
  }

  
  virtual void GetSSLCertRequestInfo(
      SSLCertRequestInfo* cert_request_info) OVERRIDE;
  virtual NextProtoStatus GetNextProto(std::string* proto,
                                       std::string* server_protos) OVERRIDE;
  virtual ServerBoundCertService* GetServerBoundCertService() const OVERRIDE;

  
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

  
  virtual int Read(IOBuffer* buf, int buf_len,
                   const CompletionCallback& callback) OVERRIDE;
  virtual int Write(IOBuffer* buf, int buf_len,
                    const CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

 private:
  class SSLContext;
  friend class SSLClientSocket;
  friend class SSLContext;

  bool Init();
  void DoReadCallback(int result);
  void DoWriteCallback(int result);

  bool DoTransportIO();
  int DoHandshake();
  int DoVerifyCert(int result);
  int DoVerifyCertComplete(int result);
  void DoConnectCallback(int result);
  X509Certificate* UpdateServerCert();

  void OnHandshakeIOComplete(int result);
  void OnSendComplete(int result);
  void OnRecvComplete(int result);

  int DoHandshakeLoop(int last_io_result);
  int DoReadLoop(int result);
  int DoWriteLoop(int result);
  int DoPayloadRead();
  int DoPayloadWrite();

  int BufferSend();
  int BufferRecv();
  void BufferSendComplete(int result);
  void BufferRecvComplete(int result);
  void TransportWriteComplete(int result);
  int TransportReadComplete(int result);

  
  
  int ClientCertRequestCallback(SSL* ssl, X509** x509, EVP_PKEY** pkey);

  
  
  void ChannelIDRequestCallback(SSL* ssl, EVP_PKEY** pkey);

  
  int SelectNextProtoCallback(unsigned char** out, unsigned char* outlen,
                              const unsigned char* in, unsigned int inlen);

  bool transport_send_busy_;
  bool transport_recv_busy_;
  bool transport_recv_eof_;

  scoped_refptr<DrainableIOBuffer> send_buffer_;
  scoped_refptr<IOBuffer> recv_buffer_;

  CompletionCallback user_connect_callback_;
  CompletionCallback user_read_callback_;
  CompletionCallback user_write_callback_;

  base::WeakPtrFactory<SSLClientSocketOpenSSL> weak_factory_;

  
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  scoped_refptr<IOBuffer> user_write_buf_;
  int user_write_buf_len_;

  
  
  
  
  
  
  int pending_read_error_;

  
  
  int transport_write_error_;

  
  scoped_refptr<X509Certificate> server_cert_;
  CertVerifyResult server_cert_verify_result_;
  bool completed_handshake_;

  
  
  bool client_auth_cert_needed_;
  
  
  std::vector<std::string> cert_authorities_;

  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;

  
  ServerBoundCertService* server_bound_cert_service_;

  
  SSL* ssl_;
  BIO* transport_bio_;

  scoped_ptr<ClientSocketHandle> transport_;
  const HostPortPair host_and_port_;
  SSLConfig ssl_config_;
  
  
  
  const std::string ssl_session_cache_shard_;

  
  bool trying_cached_session_;

  enum State {
    STATE_NONE,
    STATE_HANDSHAKE,
    STATE_VERIFY_CERT,
    STATE_VERIFY_CERT_COMPLETE,
  };
  State next_handshake_state_;
  NextProtoStatus npn_status_;
  std::string npn_proto_;
  std::string server_protos_;
  // Written by the |server_bound_cert_service_|.
  std::string channel_id_private_key_;
  std::string channel_id_cert_;
  
  int channel_id_request_return_value_;
  
  bool channel_id_xtn_negotiated_;
  
  ServerBoundCertService::RequestHandle channel_id_request_handle_;
  BoundNetLog net_log_;
};

}  

#endif  
