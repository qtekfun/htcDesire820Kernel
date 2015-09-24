// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_BUFFER_H_
#define NET_SPDY_SPDY_BUFFER_H_

#include <cstddef>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class IOBuffer;
class SpdyFrame;

// SpdyBuffer is a class to hold data read from or to be written to a
class NET_EXPORT_PRIVATE SpdyBuffer {
 public:
  
  enum ConsumeSource {
    
    CONSUME,
    
    DISCARD
  };

  
  
  
  
  
  
  typedef base::Callback<void(size_t, ConsumeSource)> ConsumeCallback;

  
  
  explicit SpdyBuffer(scoped_ptr<SpdyFrame> frame);

  
  
  SpdyBuffer(const char* data, size_t size);

  
  
  ~SpdyBuffer();

  
  const char* GetRemainingData() const;

  
  size_t GetRemainingSize() const;

  
  
  
  
  void AddConsumeCallback(const ConsumeCallback& consume_callback);

  
  
  void Consume(size_t consume_size);

  
  
  
  
  
  
  // may be written to even after the socket itself is destroyed. (See
  
  IOBuffer* GetIOBufferForRemainingData();

 private:
  void ConsumeHelper(size_t consume_size, ConsumeSource consume_source);

  
  
  typedef base::RefCountedData<scoped_ptr<SpdyFrame> > SharedFrame;

  class SharedFrameIOBuffer;

  const scoped_refptr<SharedFrame> shared_frame_;
  std::vector<ConsumeCallback> consume_callbacks_;
  size_t offset_;

  DISALLOW_COPY_AND_ASSIGN(SpdyBuffer);
};

}  

#endif  
