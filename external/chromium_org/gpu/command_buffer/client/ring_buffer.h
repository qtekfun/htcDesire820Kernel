// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_CLIENT_RING_BUFFER_H_
#define GPU_COMMAND_BUFFER_CLIENT_RING_BUFFER_H_

#include <deque>

#include "base/logging.h"
#include "gpu/command_buffer/common/types.h"
#include "gpu/gpu_export.h"

namespace gpu {
class CommandBufferHelper;

class GPU_EXPORT RingBuffer {
 public:
  typedef unsigned int Offset;

  
  
  
  
  
  RingBuffer(
      Offset base_offset, unsigned int size, CommandBufferHelper* helper);

  ~RingBuffer();

  
  
  
  
  
  
  
  
  
  Offset Alloc(unsigned int size);

  
  
  
  
  
  
  void FreePendingToken(Offset offset, unsigned int token);

  
  unsigned int GetLargestFreeSizeNoWaiting();

  
  
  
  unsigned int GetLargestFreeOrPendingSize() {
    return size_;
  }

 private:
  enum State {
    IN_USE,
    PADDING,
    FREE_PENDING_TOKEN
  };
  
  struct Block {
    Block(Offset _offset, unsigned int _size, State _state)
        : offset(_offset),
          size(_size),
          token(0),
          state(_state) {
    }
    Offset offset;
    unsigned int size;
    unsigned int token;  
    State state;
  };

  typedef std::deque<Block> Container;
  typedef unsigned int BlockIndex;

  void FreeOldestBlock();

  CommandBufferHelper* helper_;

  
  Container blocks_;

  
  Offset base_offset_;

  
  Offset size_;

  
  Offset free_offset_;

  
  
  Offset in_use_offset_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(RingBuffer);
};

class RingBufferWrapper {
 public:
  
  
  
  
  
  RingBufferWrapper(RingBuffer::Offset base_offset,
                    unsigned int size,
                    CommandBufferHelper* helper,
                    void* base)
      : allocator_(base_offset, size, helper),
        base_(static_cast<int8*>(base) - base_offset) {
  }

  
  
  
  
  
  
  
  
  
  
  void* Alloc(unsigned int size) {
    RingBuffer::Offset offset = allocator_.Alloc(size);
    return GetPointer(offset);
  }

  
  
  
  
  
  
  
  
  
  
  
  template <typename T> T* AllocTyped(unsigned int count) {
    return static_cast<T*>(Alloc(count * sizeof(T)));
  }

  
  
  
  
  
  
  void FreePendingToken(void* pointer, unsigned int token) {
    DCHECK(pointer);
    allocator_.FreePendingToken(GetOffset(pointer), token);
  }

  
  void* GetPointer(RingBuffer::Offset offset) const {
    return static_cast<int8*>(base_) + offset;
  }

  
  RingBuffer::Offset GetOffset(void* pointer) const {
    return static_cast<int8*>(pointer) - static_cast<int8*>(base_);
  }

  
  unsigned int GetLargestFreeSizeNoWaiting() {
    return allocator_.GetLargestFreeSizeNoWaiting();
  }

  
  
  unsigned int GetLargestFreeOrPendingSize() {
    return allocator_.GetLargestFreeOrPendingSize();
  }

 private:
  RingBuffer allocator_;
  void* base_;
  DISALLOW_IMPLICIT_CONSTRUCTORS(RingBufferWrapper);
};

}  

#endif  
