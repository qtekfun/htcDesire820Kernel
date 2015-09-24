// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_BYTE_QUEUE_H_
#define MEDIA_BASE_BYTE_QUEUE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT ByteQueue {
 public:
  ByteQueue();
  ~ByteQueue();

  
  void Reset();

  
  void Push(const uint8* data, int size);

  
  
  
  void Peek(const uint8** data, int* size) const;

  
  void Pop(int count);

 private:
  
  uint8* front() const;

  scoped_ptr<uint8[]> buffer_;

  
  size_t size_;

  
  size_t offset_;

  
  int used_;

  DISALLOW_COPY_AND_ASSIGN(ByteQueue);
};

}  

#endif  
