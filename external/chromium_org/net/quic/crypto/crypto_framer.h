// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CRYPTO_CRYPTO_FRAMER_H_
#define NET_QUIC_CRYPTO_CRYPTO_FRAMER_H_

#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/crypto/crypto_protocol.h"
#include "net/quic/quic_protocol.h"

namespace net {

class CryptoFramer;
class QuicData;
class QuicDataReader;
class QuicDataWriter;

class NET_EXPORT_PRIVATE CryptoFramerVisitorInterface {
 public:
  virtual ~CryptoFramerVisitorInterface() {}

  
  virtual void OnError(CryptoFramer* framer) = 0;

  
  virtual void OnHandshakeMessage(
      const CryptoHandshakeMessage& message) = 0;
};

class NET_EXPORT_PRIVATE CryptoFramer {
 public:
  CryptoFramer();

  virtual ~CryptoFramer();

  
  
  
  static CryptoHandshakeMessage* ParseMessage(base::StringPiece in);

  
  
  
  
  void set_visitor(CryptoFramerVisitorInterface* visitor) {
    visitor_ = visitor;
  }

  QuicErrorCode error() const { return error_; }

  
  
  bool ProcessInput(base::StringPiece input);

  
  
  size_t InputBytesRemaining() const { return buffer_.length(); }

  
  
  static QuicData* ConstructHandshakeMessage(
      const CryptoHandshakeMessage& message);

 private:
  
  void Clear();

  
  
  QuicErrorCode Process(base::StringPiece input);

  static bool WritePadTag(QuicDataWriter* writer,
                          size_t pad_length,
                          uint32* end_offset);

  
  enum CryptoFramerState {
    STATE_READING_TAG,
    STATE_READING_NUM_ENTRIES,
    STATE_READING_TAGS_AND_LENGTHS,
    STATE_READING_VALUES
  };

  
  CryptoFramerVisitorInterface* visitor_;
  
  QuicErrorCode error_;
  
  std::string buffer_;
  
  CryptoFramerState state_;
  
  CryptoHandshakeMessage message_;
  
  uint16 num_entries_;
  
  
  std::vector<std::pair<QuicTag, size_t> > tags_and_lengths_;
  
  size_t values_len_;
};

}  

#endif  
