// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_FRAME_READER_H_
#define NET_SPDY_SPDY_FRAME_READER_H_

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE SpdyFrameReader {
 public:
  
  SpdyFrameReader(const char* data, const size_t len);

  
  ~SpdyFrameReader() {}

  
  
  
  bool ReadUInt8(uint8* result);

  
  
  
  bool ReadUInt16(uint16* result);

  
  
  
  bool ReadUInt32(uint32* result);

  
  
  
  
  
  bool ReadUInt31(uint32* result);

  
  
  
  bool ReadUInt24(uint32* result);

  
  
  
  
  
  
  
  bool ReadStringPiece16(base::StringPiece* result);

  
  
  
  
  
  
  
  bool ReadStringPiece32(base::StringPiece* result);

  
  
  
  
  bool ReadBytes(void* result, size_t size);

  
  
  
  
  bool Seek(size_t size);

  
  void Rewind() { ofs_ = 0; }

  
  
  bool IsDoneReading() const;

  
  size_t GetBytesConsumed() const { return ofs_; }

 private:
  
  
  bool CanRead(size_t bytes) const;

  
  void OnFailure();

  
  const char* data_;

  
  const size_t len_;

  
  size_t ofs_;
};

}  

#endif  
