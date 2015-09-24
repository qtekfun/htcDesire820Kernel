// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_SERVER_SOCKET_NSS_H_
#define NET_SOCKET_SSL_SERVER_SOCKET_NSS_H_
#pragma once

#include <certt.h>
#include <keyt.h>
#include <nspr.h>
#include <nss.h>

#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_log.h"
#include "net/base/nss_memio.h"
#include "net/base/ssl_config_service.h"
#include "net/socket/ssl_server_socket.h"

namespace net {

class SSLServerSocketNSS : public SSLServerSocket {
 public:
  
  
  
  
  
  
  
  SSLServerSocketNSS(Socket* transport_socket,
                     scoped_refptr<X509Certificate> cert,
                     crypto::RSAPrivateKey* key,
                     const SSLConfig& ssl_config);
  virtual ~SSLServerSocketNSS();

  
  virtual int Accept(CompletionCallback* callback);
  virtual int Read(IOBuffer* buf, int buf_len,
                   CompletionCallback* callback);
  virtual int Write(IOBuffer* buf, int buf_len,
                    CompletionCallback* callback);
  virtual bool SetReceiveBufferSize(int32 size);
  virtual bool SetSendBufferSize(int32 size);

 private:
  enum State {
    STATE_NONE,
    STATE_HANDSHAKE,
  };

  int InitializeSSLOptions();

  void OnSendComplete(int result);
  void OnRecvComplete(int result);
  void OnHandshakeIOComplete(int result);

  int BufferSend();
  void BufferSendComplete(int result);
  int BufferRecv();
  void BufferRecvComplete(int result);
  bool DoTransportIO();
  int DoPayloadRead();
  int DoPayloadWrite();

  int DoHandshakeLoop(int last_io_result);
  int DoReadLoop(int result);
  int DoWriteLoop(int result);
  int DoHandshake();
  void DoAcceptCallback(int result);
  void DoReadCallback(int result);
  void DoWriteCallback(int result);

  static SECStatus OwnAuthCertHandler(void* arg,
                                      PRFileDesc* socket,
                                      PRBool checksig,
                                      PRBool is_server);
  static void HandshakeCallback(PRFileDesc* socket, void* arg);

  virtual int Init();

  
  CompletionCallbackImpl<SSLServerSocketNSS> buffer_send_callback_;
  CompletionCallbackImpl<SSLServerSocketNSS> buffer_recv_callback_;
  bool transport_send_busy_;
  bool transport_recv_busy_;

  scoped_refptr<IOBuffer> recv_buffer_;

  BoundNetLog net_log_;

  CompletionCallback* user_accept_callback_;
  CompletionCallback* user_read_callback_;
  CompletionCallback* user_write_callback_;

  
  scoped_refptr<IOBuffer> user_read_buf_;
  int user_read_buf_len_;

  
  scoped_refptr<IOBuffer> user_write_buf_;
  int user_write_buf_len_;

  
  PRFileDesc* nss_fd_;

  
  memio_Private* nss_bufs_;

  
  scoped_ptr<Socket> transport_socket_;

  
  
  
  SSLConfig ssl_config_;

  
  scoped_refptr<X509Certificate> cert_;

  
  scoped_ptr<crypto::RSAPrivateKey> key_;

  State next_handshake_state_;
  bool completed_handshake_;

  DISALLOW_COPY_AND_ASSIGN(SSLServerSocketNSS);
};

}  

#endif  
