// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_FRAME_BUILDER_H_
#define NET_SPDY_SPDY_FRAME_BUILDER_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "net/base/sys_byteorder.h"
#include "net/spdy/spdy_protocol.h"

namespace spdy {

// track of the type of data written to it.
class SpdyFrameBuilder {
 public:
  SpdyFrameBuilder();

  
  
  
  
  SpdyFrameBuilder(const char* data, int data_len);

  ~SpdyFrameBuilder();

  
  int length() const { return length_; }

  
  SpdyFrame* take() {
    SpdyFrame* rv = new SpdyFrame(buffer_, true);
    buffer_ = NULL;
    capacity_ = 0;
    length_ = 0;
    return rv;
  }

  
  
  
  
  bool ReadUInt16(void** iter, uint16* result) const;
  bool ReadUInt32(void** iter, uint32* result) const;
  bool ReadString(void** iter, std::string* result) const;
  bool ReadBytes(void** iter, const char** data, uint16 length) const;
  bool ReadData(void** iter, const char** data, uint16* length) const;

  
  
  
  
  bool WriteUInt16(uint16 value) {
    value = htons(value);
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteUInt32(uint32 value) {
    value = htonl(value);
    return WriteBytes(&value, sizeof(value));
  }
  bool WriteString(const std::string& value);
  bool WriteBytes(const void* data, uint16 data_len);

  
  bool WriteUInt32ToOffset(int offset, uint32 value) {
    value = htonl(value);
    return WriteBytesToOffset(offset, &value, sizeof(value));
  }

  
  bool WriteBytesToOffset(int offset, const void* data, uint32 data_len) {
    if (offset + data_len > length_)
      return false;
    char *ptr = buffer_ + offset;
    memcpy(ptr, data, data_len);
    return true;
  }

  
  
  
  
  
  
  
  
  char* BeginWriteData(uint16 length);

  
  
  
  bool IteratorHasRoomFor(const void* iter, int len) const {
    const char* end_of_region = reinterpret_cast<const char*>(iter) + len;
    if (len < 0 ||
        iter < buffer_ ||
        iter > end_of_payload() ||
        iter > end_of_region ||
        end_of_region > end_of_payload())
      return false;

    
    return (iter <= end_of_region) && (end_of_region <= end_of_payload());
  }

 protected:
  size_t capacity() const {
    return capacity_;
  }

  const char* end_of_payload() const { return buffer_ + length_; }

  
  // location that the data should be written at is returned, or NULL if there
  
  
  char* BeginWrite(size_t length);

  
  
  // have to be called after the data is written.
  void EndWrite(char* dest, int length);

  
  
  
  
  bool Resize(size_t new_capacity);

  
  static void UpdateIter(void** iter, int bytes) {
    *iter = static_cast<char*>(*iter) + bytes;
  }

  
  static const int kInitialPayload = 1024;

 private:
  char* buffer_;
  size_t capacity_;  
  size_t length_;    
  size_t variable_buffer_offset_;  
};

}  

#endif  
