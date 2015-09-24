// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_FLIP_SERVER_RING_BUFFER_H__
#define NET_TOOLS_FLIP_SERVER_RING_BUFFER_H__
#pragma once

#include "base/memory/scoped_ptr.h"
#include "net/tools/flip_server/buffer_interface.h"

namespace net {


class RingBuffer : public BufferInterface {
 public:
  explicit RingBuffer(int buffer_size);
  virtual ~RingBuffer();

  
  
  
  
  
  void Resize(int buffer_size);

  
  
  
  virtual int ReadableBytes() const;
  virtual int BufferSize() const;
  virtual int BytesFree() const;

  virtual bool Empty() const;
  virtual bool Full() const;

  // returns the number of characters written.
  
  virtual int Write(const char * bytes, int size);

  
  // characters which are allowed to be written in *size.
  
  virtual void GetWritablePtr(char** ptr, int* size) const;

  
  
  
  virtual void GetReadablePtr(char** ptr, int* size) const;

  
  virtual int Read(char* bytes, int size);

  
  virtual void Clear();

  
  
  
  
  
  
  
  
  
  
  
  virtual bool Reserve(int size);

  
  
  virtual void AdvanceReadablePtr(int amount_to_advance);

  
  // here is expected to already be resident (as if it was Written).
  virtual void AdvanceWritablePtr(int amount_to_advance);

 protected:
  int read_idx() const { return read_idx_; }
  int write_idx() const { return write_idx_; }
  int bytes_used() const { return bytes_used_; }
  int buffer_size() const { return buffer_size_; }
  const char* buffer() const { return buffer_.get(); }

  int set_read_idx(int idx) { return read_idx_ = idx; }
  int set_write_idx(int idx) { return write_idx_ = idx; }

 private:
  scoped_array<char> buffer_;
  int buffer_size_;
  int bytes_used_;
  int read_idx_;
  int write_idx_;

  RingBuffer(const RingBuffer&);
  void operator=(const RingBuffer&);
};

}  

#endif  

