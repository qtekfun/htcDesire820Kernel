// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_BASE_SEEKABLE_BUFFER_H_
#define MEDIA_BASE_SEEKABLE_BUFFER_H_

#include <list>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "media/base/buffers.h"

namespace media {

class DataBuffer;

class MEDIA_EXPORT SeekableBuffer {
 public:
  
  
  SeekableBuffer(int backward_capacity, int forward_capacity);

  ~SeekableBuffer();

  
  void Clear();

  
  
  
  
  
  int Read(uint8* data, int size);

  
  
  
  int Peek(uint8* data, int size) { return Peek(data, size, 0); }
  int Peek(uint8* data, int size, int forward_offset);

  
  
  
  
  
  bool GetCurrentChunk(const uint8** data, int* size) const;

  
  
  
  bool Append(const scoped_refptr<DataBuffer>& buffer_in);

  
  
  bool Append(const uint8* data, int size);

  
  
  
  
  
  
  
  
  
  
  bool Seek(int32 offset);

  
  int forward_bytes() const { return forward_bytes_; }

  
  
  int backward_bytes() const { return backward_bytes_; }

  
  void set_forward_capacity(int new_forward_capacity) {
    forward_capacity_ = new_forward_capacity;
  }

  
  void set_backward_capacity(int new_backward_capacity) {
    backward_capacity_ = new_backward_capacity;
  }

  
  
  int forward_capacity() const { return forward_capacity_; }

  
  
  int backward_capacity() const { return backward_capacity_; }

  
  
  
  
  
  
  base::TimeDelta current_time() const { return current_time_; }

 private:
  
  typedef std::list<scoped_refptr<DataBuffer> > BufferQueue;

  
  
  void EvictBackwardBuffers();

  
  
  
  
  
  int InternalRead(
      uint8* data, int size, bool advance_position, int forward_offset);

  
  
  
  
  
  bool SeekForward(int size);

  
  
  
  
  
  bool SeekBackward(int size);

  
  
  void UpdateCurrentTime(BufferQueue::iterator buffer, int offset);

  BufferQueue::iterator current_buffer_;
  BufferQueue buffers_;
  int current_buffer_offset_;

  int backward_capacity_;
  int backward_bytes_;

  int forward_capacity_;
  int forward_bytes_;

  
  
  base::TimeDelta current_time_;

  DISALLOW_COPY_AND_ASSIGN(SeekableBuffer);
};

}  

#endif  
