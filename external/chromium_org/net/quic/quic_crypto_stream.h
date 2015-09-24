// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CRYPTO_STREAM_H_
#define NET_QUIC_QUIC_CRYPTO_STREAM_H_

#include "net/quic/crypto/crypto_framer.h"
#include "net/quic/crypto/crypto_utils.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/reliable_quic_stream.h"

namespace net {

class CryptoHandshakeMessage;
class QuicSession;

class NET_EXPORT_PRIVATE QuicCryptoStream
    : public ReliableQuicStream,
      public CryptoFramerVisitorInterface {
 public:
  explicit QuicCryptoStream(QuicSession* session);

  
  virtual void OnError(CryptoFramer* framer) OVERRIDE;
  virtual void OnHandshakeMessage(
      const CryptoHandshakeMessage& message) OVERRIDE;

  
  virtual uint32 ProcessRawData(const char* data, uint32 data_len) OVERRIDE;
  virtual QuicPriority EffectivePriority() const OVERRIDE;

  
  
  void SendHandshakeMessage(const CryptoHandshakeMessage& message);

  bool encryption_established() { return encryption_established_; }
  bool handshake_confirmed() { return handshake_confirmed_; }

  const QuicCryptoNegotiatedParameters& crypto_negotiated_params() const;

 protected:
  bool encryption_established_;
  bool handshake_confirmed_;

  QuicCryptoNegotiatedParameters crypto_negotiated_params_;

 private:
  CryptoFramer crypto_framer_;

  DISALLOW_COPY_AND_ASSIGN(QuicCryptoStream);
};

}  

#endif  
