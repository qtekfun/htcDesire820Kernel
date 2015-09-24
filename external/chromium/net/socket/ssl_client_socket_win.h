// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_CLIENT_SOCKET_WIN_H_
#define NET_SOCKET_SSL_CLIENT_SOCKET_WIN_H_
#pragma once

#define SECURITY_WIN32  

#include <windows.h>
#include <wincrypt.h>
#include <security.h>

#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/cert_verify_result.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/base/ssl_config_service.h"
#include "net/socket/ssl_client_socket.h"

namespace net {

class BoundNetLog;
class CertVerifier;
class ClientSocketHandle;
class HostPortPair;
class SingleRequestCertVerifier;

class SSLClientSocketWin : public SSLClientSocket {
 public:
  
  
  
  
  
  
  SSLClientSocketWin(ClientSocketHandle* transport_socket,
                     const HostPortPair& host_and_port,
                     const SSLConfig& ssl_config,
                     CertVerifier* cert_verifier);
  ~SSLClientSocketWin();

  
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
  virtual const BoundNetLog& NetLog() const { return net_log_; }
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
    return next_state_ == STATE_COMPLETED_HANDSHAKE;
  }

  
  int InitializeSSLContext();

  void OnHandshakeIOComplete(int result);
  void OnReadComplete(int result);
  void OnWriteComplete(int result);

  int DoLoop(int last_io_result);
  int DoHandshakeRead();
  int DoHandshakeReadComplete(int result);
  int DoHandshakeWrite();
  int DoHandshakeWriteComplete(int result);
  int DoVerifyCert();
  int DoVerifyCertComplete(int result);

  int DoPayloadRead();
  int DoPayloadReadComplete(int result);
  int DoPayloadDecrypt();
  int DoPayloadEncrypt();
  int DoPayloadWrite();
  int DoPayloadWriteComplete(int result);
  int DoCompletedRenegotiation(int result);

  int DidCallInitializeSecurityContext();
  int DidCompleteHandshake();
  void DidCompleteRenegotiation();
  void LogConnectionTypeMetrics() const;
  void FreeSendBuffer();

  
  CompletionCallbackImpl<SSLClientSocketWin> handshake_io_callback_;
  CompletionCallbackImpl<SSLClientSocketWin> read_callback_;
  CompletionCallbackImpl<SSLClientSocketWin> write_callback_;

  scoped_ptr<ClientSocketHandle> transport_;
  HostPortPair host_and_port_;
  SSLConfig ssl_config_;

  
  CompletionCallback* user_connect_callback_;

  
  CompletionCallback* user_read_callback_;
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  CompletionCallback* user_write_callback_;
  scoped_refptr<IOBuffer> user_write_buf_;
  int user_write_buf_len_;

  
  scoped_refptr<IOBuffer> transport_read_buf_;
  scoped_refptr<IOBuffer> transport_write_buf_;

  enum State {
    STATE_NONE,
    STATE_HANDSHAKE_READ,
    STATE_HANDSHAKE_READ_COMPLETE,
    STATE_HANDSHAKE_WRITE,
    STATE_HANDSHAKE_WRITE_COMPLETE,
    STATE_VERIFY_CERT,
    STATE_VERIFY_CERT_COMPLETE,
    STATE_COMPLETED_RENEGOTIATION,
    STATE_COMPLETED_HANDSHAKE
    
    
    
  };
  State next_state_;

  SecPkgContext_StreamSizes stream_sizes_;
  scoped_refptr<X509Certificate> server_cert_;
  CertVerifier* const cert_verifier_;
  scoped_ptr<SingleRequestCertVerifier> verifier_;
  CertVerifyResult server_cert_verify_result_;

  CredHandle* creds_;
  CtxtHandle ctxt_;
  SecBuffer in_buffers_[2];  
  SecBuffer send_buffer_;  
  SECURITY_STATUS isc_status_;  
  scoped_array<char> payload_send_buffer_;
  int payload_send_buffer_len_;
  int bytes_sent_;

  
  
  
  
  
  
  
  scoped_array<char> recv_buffer_;
  char* decrypted_ptr_;  
  int bytes_decrypted_;  
  char* received_ptr_;  
  int bytes_received_;  

  
  // false if we're writing a subsequent token.  After we have written a token
  
  
  bool writing_first_token_;

  
  
  
  
  
  
  
  
  
  bool ignore_ok_result_;

  
  bool renegotiating_;

  
  bool need_more_data_;

  BoundNetLog net_log_;
};

}  

#endif  
