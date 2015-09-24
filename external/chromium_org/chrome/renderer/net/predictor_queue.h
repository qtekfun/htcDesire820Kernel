// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// It is written to be as fast as possible during push() operations, so


#ifndef CHROME_RENDERER_NET_PREDICTOR_QUEUE_H__
#define CHROME_RENDERER_NET_PREDICTOR_QUEUE_H__

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

class DnsQueue {
 public:
  
  typedef int32 BufferSize;

  enum PushResult { SUCCESSFUL_PUSH, OVERFLOW_PUSH, REDUNDANT_PUSH };

  
  
  
  
  
  
  
  explicit DnsQueue(BufferSize size);
  ~DnsQueue(void);

  size_t Size() const { return size_; }
  void Clear();

  
  
  
  
  
  // Exactly length chars are written, or the push fails (where
  // "fails" means nothing is written).
  // Returns true for success, false for failure (nothing written).
  PushResult Push(const char* source, const size_t length);

  PushResult Push(std::string source) {
    return Push(source.c_str(), source.length());
  }

  
  
  bool Pop(std::string* out_string);

 private:
  bool Validate();  

  const scoped_ptr<char[]> buffer_;  
  const BufferSize buffer_size_;  
  const BufferSize buffer_sentinel_;  

  
  BufferSize readable_;  
  BufferSize writeable_;  

  
  size_t size_;

  DISALLOW_COPY_AND_ASSIGN(DnsQueue);
};  

#endif  
