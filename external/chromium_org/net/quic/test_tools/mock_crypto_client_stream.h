// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_TEST_TOOLS_MOCK_CRYPTO_CLIENT_STREAM_H_
#define NET_QUIC_TEST_TOOLS_MOCK_CRYPTO_CLIENT_STREAM_H_

#include <string>

#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/quic_crypto_client_stream.h"
#include "net/quic/quic_session.h"

namespace net {

class MockCryptoClientStream : public QuicCryptoClientStream {
 public:
  
  
  enum HandshakeMode {
    
    
    
    CONFIRM_HANDSHAKE,

    
    
    ZERO_RTT,

    
    
    COLD_START,
  };

  MockCryptoClientStream(
      const string& server_hostname,
      QuicSession* session,
      QuicCryptoClientConfig* crypto_config,
      HandshakeMode handshake_mode);
  virtual ~MockCryptoClientStream();

  
  virtual void OnHandshakeMessage(
      const CryptoHandshakeMessage& message) OVERRIDE;

  
  virtual bool CryptoConnect() OVERRIDE;

  
  
  void SendOnCryptoHandshakeEvent(QuicSession::CryptoHandshakeEvent event);

  HandshakeMode handshake_mode_;

 private:
  void SetConfigNegotiated();
};

}  

#endif  
