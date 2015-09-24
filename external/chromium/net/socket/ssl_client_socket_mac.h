// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_MAC_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_MAC_H_
#pragma once

#include <Security/Security.h>

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "net/base/cert_verify_result.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/base/ssl_config_service.h"
#include "net/socket/ssl_client_socket.h"

namespace net {

class CertVerifier;
class ClientSocketHandle;
class SingleRequestCertVerifier;

class SSLClientSocketMac : public SSLClientSocket {
 public:
  
  
  
  
  
  
  SSLClientSocketMac(ClientSocketHandle* transport_socket,
                     const HostPortPair& host_and_port,
                     const SSLConfig& ssl_config,
                     CertVerifier* cert_verifier);
  ~SSLClientSocketMac();

  
  virtual void GetSSLInfo(SSLInfo* ssl_info);
  virtual void GetSSLCertRequestInfo(SSLCertRequestInfo* cert_request_info);
  virtual NextProtoStatus GetNextProto(std::string* proto);

  
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
  bool completed_handshake() const {
    return next_handshake_state_ == STATE_COMPLETED_HANDSHAKE;
  }
  
  int InitializeSSLContext();

  void DoConnectCallback(int result);
  void DoReadCallback(int result);
  void DoWriteCallback(int result);
  void OnHandshakeIOComplete(int result);
  void OnTransportReadComplete(int result);
  void OnTransportWriteComplete(int result);

  int DoHandshakeLoop(int last_io_result);

  int DoPayloadRead();
  int DoPayloadWrite();
  int DoHandshake();
  int DoVerifyCert();
  int DoVerifyCertComplete(int result);
  int DoCompletedRenegotiation(int result);

  void DidCompleteRenegotiation();
  int DidCompleteHandshake();

  int SetClientCert();

  static OSStatus SSLReadCallback(SSLConnectionRef connection,
                                  void* data,
                                  size_t* data_length);
  static OSStatus SSLWriteCallback(SSLConnectionRef connection,
                                   const void* data,
                                   size_t* data_length);

  CompletionCallbackImpl<SSLClientSocketMac> handshake_io_callback_;
  CompletionCallbackImpl<SSLClientSocketMac> transport_read_callback_;
  CompletionCallbackImpl<SSLClientSocketMac> transport_write_callback_;

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

  enum State {
    STATE_NONE,
    STATE_HANDSHAKE,
    STATE_VERIFY_CERT,
    STATE_VERIFY_CERT_COMPLETE,
    STATE_COMPLETED_RENEGOTIATION,
    STATE_COMPLETED_HANDSHAKE,
    
    
    
    
    
    
    
    
    
    
  };
  State next_handshake_state_;

  scoped_refptr<X509Certificate> server_cert_;
  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;
  CertVerifyResult server_cert_verify_result_;

  
  
  bool renegotiating_;
  bool client_cert_requested_;
  SSLContextRef ssl_context_;

  
  
  size_t bytes_read_after_renegotiation_;

  
  
  std::vector<char> send_buffer_;
  int pending_send_error_;
  std::vector<char> recv_buffer_;

  
  scoped_refptr<IOBuffer> read_io_buf_;
  scoped_refptr<IOBuffer> write_io_buf_;

  BoundNetLog net_log_;
};

}  

#endif  
