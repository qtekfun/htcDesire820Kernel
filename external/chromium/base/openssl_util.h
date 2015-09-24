// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_OPENSSL_UTIL_H_
#define BASE_OPENSSL_UTIL_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/tracked.h"

namespace base {

template <typename T, void (*destructor)(T*)>
class ScopedOpenSSL {
 public:
  ScopedOpenSSL() : ptr_(NULL) { }
  explicit ScopedOpenSSL(T* ptr) : ptr_(ptr) { }
  ~ScopedOpenSSL() {
    reset(NULL);
  }

  T* get() const { return ptr_; }
  void reset(T* ptr) {
    if (ptr != ptr_) {
      if (ptr_) (*destructor)(ptr_);
      ptr_ = ptr;
    }
  }
  T* release() WARN_UNUSED_RESULT {
    T* result = ptr_;
    ptr_ = NULL;
    return result;
  }

 private:
  T* ptr_;

  DISALLOW_COPY_AND_ASSIGN(ScopedOpenSSL);
};

template<int MIN_SIZE>
class ScopedOpenSSLSafeSizeBuffer {
 public:
  ScopedOpenSSLSafeSizeBuffer(unsigned char* output, size_t output_len)
      : output_(output),
        output_len_(output_len) {
  }

  ~ScopedOpenSSLSafeSizeBuffer() {
    if (output_len_ < MIN_SIZE) {
      
      memcpy(output_, min_sized_buffer_, output_len_);
    }
    
  }

  unsigned char* safe_buffer() {
    return output_len_ < MIN_SIZE ? min_sized_buffer_ : output_;
  }

 private:
  
  // written via safe_buffer() will [eventually] end up.
  unsigned char* output_;
  size_t output_len_;

  
  
  unsigned char min_sized_buffer_[MIN_SIZE];

  DISALLOW_COPY_AND_ASSIGN(ScopedOpenSSLSafeSizeBuffer);
};

void EnsureOpenSSLInit();

void ClearOpenSSLERRStack(const tracked_objects::Location& location);

class OpenSSLErrStackTracer {
 public:
  
  
  
  explicit OpenSSLErrStackTracer(const tracked_objects::Location& location)
      : location_(location) {
    EnsureOpenSSLInit();
  }
  ~OpenSSLErrStackTracer() {
    ClearOpenSSLERRStack(location_);
  }

 private:
  const tracked_objects::Location location_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(OpenSSLErrStackTracer);
};

}  

#endif  
