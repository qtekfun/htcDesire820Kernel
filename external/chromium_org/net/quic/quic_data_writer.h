// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_DATA_WRITER_H_
#define NET_QUIC_QUIC_DATA_WRITER_H_

#include <string>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/port.h"
#include "base/strings/string_piece.h"
#include "net/base/int128.h"
#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"

namespace net {

class NET_EXPORT_PRIVATE QuicDataWriter {
 public:
  explicit QuicDataWriter(size_t length);

  ~QuicDataWriter();

  
  size_t length() const { return length_; }

  
  char* take();

  
  // of the QuicDataWriter payload. Note - binary integers are written in
  
  bool WriteUInt8(uint8 value);
  bool WriteUInt16(uint16 value);
  bool WriteUInt32(uint32 value);
  bool WriteUInt48(uint64 value);
  bool WriteUInt64(uint64 value);
  
  
  
  bool WriteUFloat16(uint64 value);
  bool WriteStringPiece16(base::StringPiece val);
  bool WriteIOVector(const IOVector& data);
  bool WriteBytes(const void* data, size_t data_len);
  bool WriteRepeatedByte(uint8 byte, size_t count);
  
  void WritePadding();

  
  
  
  bool WriteUInt8ToOffset(uint8 value, size_t offset);
  bool WriteUInt32ToOffset(uint32 value, size_t offset);
  bool WriteUInt48ToOffset(uint64 value, size_t offset);

  size_t capacity() const {
    return capacity_;
  }

 private:
  // Returns the location that the data should be written at, or NULL if there
  
  
  char* BeginWrite(size_t length);

  char* buffer_;
  size_t capacity_;  
  size_t length_;    
};

}  

#endif  
