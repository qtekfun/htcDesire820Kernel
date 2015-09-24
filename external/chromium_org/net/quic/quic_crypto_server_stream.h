// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CRYPTO_SERVER_STREAM_H_
#define NET_QUIC_QUIC_CRYPTO_SERVER_STREAM_H_

#include <string>

#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/crypto/quic_crypto_server_config.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_crypto_stream.h"

namespace net {

class CryptoHandshakeMessage;
class QuicCryptoServerConfig;
class QuicSession;

namespace test {
class CryptoTestUtils;
}  

class NET_EXPORT_PRIVATE QuicCryptoServerStream : public QuicCryptoStream {
 public:
  QuicCryptoServerStream(const QuicCryptoServerConfig& crypto_config,
                         QuicSession* session);
  explicit QuicCryptoServerStream(QuicSession* session);
  virtual ~QuicCryptoServerStream();

  
  virtual void OnHandshakeMessage(
      const CryptoHandshakeMessage& message) OVERRIDE;

  
  
  
  bool GetBase64SHA256ClientChannelID(std::string* output) const;

 protected:
  virtual QuicErrorCode ProcessClientHello(
      const CryptoHandshakeMessage& message,
      const ValidateClientHelloResultCallback::Result& result,
      CryptoHandshakeMessage* reply,
      std::string* error_details);

 private:
  friend class test::CryptoTestUtils;

  class ValidateCallback : public ValidateClientHelloResultCallback {
   public:
    explicit ValidateCallback(QuicCryptoServerStream* parent);
    
    void Cancel();

    
    virtual void RunImpl(const CryptoHandshakeMessage& client_hello,
                         const Result& result) OVERRIDE;

   private:
    QuicCryptoServerStream* parent_;

    DISALLOW_COPY_AND_ASSIGN(ValidateCallback);
  };

  
  
  
  void FinishProcessingHandshakeMessage(
      const CryptoHandshakeMessage& message,
      const ValidateClientHelloResultCallback::Result& result);

  
  const QuicCryptoServerConfig& crypto_config_;

  
  
  
  
  ValidateCallback* validate_client_hello_cb_;
};

}  

#endif  
