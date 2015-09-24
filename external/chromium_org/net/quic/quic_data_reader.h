// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_DATA_READER_H_
#define NET_QUIC_QUIC_DATA_READER_H_

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/int128.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE QuicDataReader {
 public:
  
  QuicDataReader(const char* data, const size_t len);

  
  ~QuicDataReader() {}

  
  
  
  bool ReadUInt16(uint16* result);

  
  
  
  bool ReadUInt32(uint32* result);

  
  
  
  bool ReadUInt48(uint64* result);

  
  
  
  bool ReadUInt64(uint64* result);

  
  
  
  bool ReadUInt128(uint128* result);

  
  
  
  bool ReadUFloat16(uint64* result);

  
  
  
  
  
  
  
  bool ReadStringPiece16(base::StringPiece* result);

  
  
  
  
  bool ReadStringPiece(base::StringPiece* result, size_t len);

  
  
  
  
  
  
  base::StringPiece ReadRemainingPayload();

  
  
  
  
  
  
  base::StringPiece PeekRemainingPayload();

  
  
  
  
  bool ReadBytes(void* result, size_t size);

  
  
  bool IsDoneReading() const;

  
  size_t BytesRemaining() const;

 private:
  
  
  bool CanRead(size_t bytes) const;

  
  void OnFailure();

  
  const char* data_;

  
  const size_t len_;

  
  size_t pos_;
};

}  

#endif  
