// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_OPENSSL_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_OPENSSL_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "net/base/cert_verify_result.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/ssl_config_service.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/client_socket_handle.h"

typedef struct bio_st BIO;
typedef struct evp_pkey_st EVP_PKEY;
typedef struct ssl_st SSL;
typedef struct x509_st X509;

namespace net {

class CertVerifier;
class SingleRequestCertVerifier;
class SSLCertRequestInfo;
class SSLConfig;
class SSLInfo;

class SSLClientSocketOpenSSL : public SSLClientSocket {
 public:
  
  
  
  
  SSLClientSocketOpenSSL(ClientSocketHandle* transport_socket,
                         const HostPortPair& host_and_port,
                         const SSLConfig& ssl_config,
                         CertVerifier* cert_verifier);
  ~SSLClientSocketOpenSSL();

  const HostPortPair& host_and_port() const { return host_and_port_; }

  
  
  int ClientCertRequestCallback(SSL* ssl, X509** x509, EVP_PKEY** pkey);

  
  int SelectNextProtoCallback(unsigned char** out, unsigned char* outlen,
                              const unsigned char* in, unsigned int inlen);

  
  virtual void GetSSLInfo(SSLInfo* ssl_info);
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);
  virtual NextProtoStatus GetNextProto(std::string* proto);

  
  virtual int Connect(CompletionCallback* callback
#ifdef ANDROID
                      , bool wait_for_connect
                      , bool valid_uid
                      , uid_t calling_uid
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

  CompletionCallbackImpl<SSLClientSocketOpenSSL> buffer_send_callback_;
  CompletionCallbackImpl<SSLClientSocketOpenSSL> buffer_recv_callback_;
  bool transport_send_busy_;
  scoped_refptr<DrainableIOBuffer> send_buffer_;
  bool transport_recv_busy_;
  bool transport_recv_eof_;
  scoped_refptr<IOBuffer> recv_buffer_;

  base::WeakPtrFactory<SSLClientSocketOpenSSL> weak_factory_;

  CompletionCallback* user_connect_callback_;
  CompletionCallback* user_read_callback_;
  CompletionCallback* user_write_callback_;

  
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  scoped_refptr<IOBuffer> user_write_buf_;
  int user_write_buf_len_;

  int pending_read_error_;

  
  
  int transport_write_error_;

  
  scoped_refptr<X509Certificate> server_cert_;
  CertVerifyResult server_cert_verify_result_;
  bool completed_handshake_;

  
  
  std::vector<scoped_refptr<X509Certificate> > client_certs_;
  bool client_auth_cert_needed_;

  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;
  CompletionCallbackImpl<SSLClientSocketOpenSSL> handshake_io_callback_;

  
  SSL* ssl_;
  BIO* transport_bio_;

  scoped_ptr<ClientSocketHandle> transport_;
  const HostPortPair host_and_port_;
  SSLConfig ssl_config_;

  
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
  BoundNetLog net_log_;
};

}  

#endif  
