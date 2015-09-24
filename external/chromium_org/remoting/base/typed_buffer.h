// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_BASE_TYPED_BUFFER_H_
#define REMOTING_BASE_TYPED_BUFFER_H_

#include <assert.h>

#include <algorithm>

#include "base/basictypes.h"
#include "base/move.h"

namespace remoting {

template <typename T>
class TypedBuffer {
  MOVE_ONLY_TYPE_FOR_CPP_03(TypedBuffer, RValue)

 public:
  TypedBuffer() : buffer_(NULL), length_(0) {
  }

  
  explicit TypedBuffer(uint32 length) : buffer_(NULL), length_(length) {
    if (length_ > 0)
      buffer_ = reinterpret_cast<T*>(new uint8[length_]);
  }

  
  TypedBuffer(RValue rvalue) : buffer_(NULL), length_(0) {
    TypedBuffer temp;
    temp.Swap(*rvalue.object);
    Swap(temp);
  }

  ~TypedBuffer() {
    if (buffer_) {
      delete[] reinterpret_cast<uint8*>(buffer_);
      buffer_ = NULL;
    }
  }

  
  TypedBuffer& operator=(RValue rvalue) {
    TypedBuffer temp;
    temp.Swap(*rvalue.object);
    Swap(temp);
    return *this;
  }

  
  
  T& operator*() const {
    assert(buffer_ != NULL);
    return *buffer_;
  }
  T* operator->() const  {
    assert(buffer_ != NULL);
    return buffer_;
  }
  T* get() const { return buffer_; }

  uint32 length() const { return length_; }

  
  
  T* GetAtOffset(uint32 offset) {
    return reinterpret_cast<T*>(reinterpret_cast<uint8*>(buffer_) + offset);
  }

  
  
  typedef T* TypedBuffer::*Testable;
  operator Testable() const { return buffer_ ? &TypedBuffer::buffer_ : NULL; }

  
  void Swap(TypedBuffer& other) {
    std::swap(buffer_, other.buffer_);
    std::swap(length_, other.length_);
  }

 private:
  
  T* buffer_;

  
  uint32 length_;
};

}  

#endif  
