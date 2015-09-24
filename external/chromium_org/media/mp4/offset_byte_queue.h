// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MP4_OFFSET_BYTE_QUEUE_H_
#define MEDIA_MP4_OFFSET_BYTE_QUEUE_H_

#include "base/basictypes.h"
#include "media/base/byte_queue.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT OffsetByteQueue {
 public:
  OffsetByteQueue();
  ~OffsetByteQueue();

  
  void Reset();
  void Push(const uint8* buf, int size);
  void Peek(const uint8** buf, int* size);
  void Pop(int count);

  
  
  
  
  
  
  void PeekAt(int64 offset, const uint8** buf, int* size);

  
  
  
  
  
  
  
  
  bool Trim(int64 max_offset);

  
  
  int64 head() { return head_; }
  int64 tail() { return head_ + size_; }

 private:
  
  void Sync();

  ByteQueue queue_;
  const uint8* buf_;
  int size_;
  int64 head_;

  DISALLOW_COPY_AND_ASSIGN(OffsetByteQueue);
};

}  

#endif  
