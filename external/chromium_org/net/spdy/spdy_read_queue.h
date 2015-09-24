// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_BUFFER_QUEUE_H_
#define NET_SPDY_SPDY_BUFFER_QUEUE_H_

#include <cstddef>
#include <deque>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"

namespace net {

class SpdyBuffer;

class NET_EXPORT_PRIVATE SpdyReadQueue {
 public:
  SpdyReadQueue();
  ~SpdyReadQueue();

  
  bool IsEmpty() const;

  
  size_t GetTotalSize() const;

  
  void Enqueue(scoped_ptr<SpdyBuffer> buffer);

  
  
  size_t Dequeue(char* out, size_t len);

  
  void Clear();

 private:
  std::deque<SpdyBuffer*> queue_;
  size_t total_size_;

  DISALLOW_COPY_AND_ASSIGN(SpdyReadQueue);
};

}  

#endif  
