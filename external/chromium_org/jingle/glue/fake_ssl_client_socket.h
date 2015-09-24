// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_FAKE_SSL_CLIENT_SOCKET_H_
#define JINGLE_GLUE_FAKE_SSL_CLIENT_SOCKET_H_

#include <cstddef>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/completion_callback.h"
#include "net/base/net_errors.h"
#include "net/socket/stream_socket.h"

namespace net {
class DrainableIOBuffer;
class SSLInfo;
}  

namespace jingle_glue {

class FakeSSLClientSocket : public net::StreamSocket {
 public:
  explicit FakeSSLClientSocket(scoped_ptr<net::StreamSocket> transport_socket);

  virtual ~FakeSSLClientSocket();

  
  static base::StringPiece GetSslClientHello();
  static base::StringPiece GetSslServerHello();

  
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    const net::CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;
  virtual int Connect(const net::CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual const net::BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual net::NextProto GetNegotiatedProtocol() const OVERRIDE;
  virtual bool GetSSLInfo(net::SSLInfo* ssl_info) OVERRIDE;

 private:
  enum HandshakeState {
    STATE_NONE,
    STATE_CONNECT,
    STATE_SEND_CLIENT_HELLO,
    STATE_VERIFY_SERVER_HELLO,
  };

  int DoHandshakeLoop();
  void RunUserConnectCallback(int status);
  void DoHandshakeLoopWithUserConnectCallback();

  int DoConnect();
  void OnConnectDone(int status);
  void ProcessConnectDone();

  int DoSendClientHello();
  void OnSendClientHelloDone(int status);
  void ProcessSendClientHelloDone(size_t written);

  int DoVerifyServerHello();
  void OnVerifyServerHelloDone(int status);
  net::Error ProcessVerifyServerHelloDone(size_t read);

  scoped_ptr<net::StreamSocket> transport_socket_;

  
  
  HandshakeState next_handshake_state_;

  
  bool handshake_completed_;

  
  net::CompletionCallback user_connect_callback_;

  scoped_refptr<net::DrainableIOBuffer> write_buf_;
  scoped_refptr<net::DrainableIOBuffer> read_buf_;
};

}  

#endif  
