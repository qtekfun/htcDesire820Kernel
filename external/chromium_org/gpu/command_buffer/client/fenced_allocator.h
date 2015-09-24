// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef GPU_COMMAND_BUFFER_CLIENT_FENCED_ALLOCATOR_H_
#define GPU_COMMAND_BUFFER_CLIENT_FENCED_ALLOCATOR_H_

#include <vector>

#include "base/logging.h"
#include "gpu/command_buffer/common/types.h"
#include "gpu/gpu_export.h"

namespace gpu {
class CommandBufferHelper;

class GPU_EXPORT FencedAllocator {
 public:
  typedef unsigned int Offset;
  
  static const Offset kInvalidOffset = 0xffffffffU;

  
  
  FencedAllocator(unsigned int size,
                  CommandBufferHelper *helper);

  ~FencedAllocator();

  
  
  
  
  
  
  
  
  
  
  Offset Alloc(unsigned int size);

  
  
  
  
  void Free(Offset offset);

  
  
  
  
  
  
  void FreePendingToken(Offset offset, int32 token);

  
  void FreeUnused();

  
  unsigned int GetLargestFreeSize();

  
  
  
  unsigned int GetLargestFreeOrPendingSize();

  
  
  bool CheckConsistency();

  
  bool InUse();

  
  size_t bytes_in_use() const { return bytes_in_use_; }

 private:
  
  enum State {
    IN_USE,
    FREE,
    FREE_PENDING_TOKEN
  };

  
  struct Block {
    State state;
    Offset offset;
    unsigned int size;
    int32 token;  
  };

  
  class OffsetCmp {
   public:
    bool operator() (const Block &left, const Block &right) {
      return left.offset < right.offset;
    }
  };

  typedef std::vector<Block> Container;
  typedef unsigned int BlockIndex;

  static const int32 kUnusedToken = 0;

  
  BlockIndex GetBlockByOffset(Offset offset);

  
  
  
  BlockIndex CollapseFreeBlock(BlockIndex index);

  
  
  
  BlockIndex WaitForTokenAndFreeBlock(BlockIndex index);

  
  
  
  
  
  Offset AllocInBlock(BlockIndex index, unsigned int size);

  CommandBufferHelper *helper_;
  Container blocks_;
  size_t bytes_in_use_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(FencedAllocator);
};

class FencedAllocatorWrapper {
 public:
  FencedAllocatorWrapper(unsigned int size,
                         CommandBufferHelper* helper,
                         void* base)
      : allocator_(size, helper),
        base_(base) { }

  
  
  
  
  
  
  
  
  
  
  void *Alloc(unsigned int size) {
    FencedAllocator::Offset offset = allocator_.Alloc(size);
    return GetPointer(offset);
  }

  
  
  
  
  
  
  
  
  
  
  
  template <typename T> T *AllocTyped(unsigned int count) {
    return static_cast<T *>(Alloc(count * sizeof(T)));
  }

  
  
  
  
  void Free(void *pointer) {
    DCHECK(pointer);
    allocator_.Free(GetOffset(pointer));
  }

  
  
  
  
  
  
  void FreePendingToken(void *pointer, int32 token) {
    DCHECK(pointer);
    allocator_.FreePendingToken(GetOffset(pointer), token);
  }

  
  void FreeUnused() {
    allocator_.FreeUnused();
  }

  
  
  void *GetPointer(FencedAllocator::Offset offset) {
    return (offset == FencedAllocator::kInvalidOffset) ?
        NULL : static_cast<char *>(base_) + offset;
  }

  
  
  FencedAllocator::Offset GetOffset(void *pointer) {
    return pointer ?
        static_cast<FencedAllocator::Offset>(
            static_cast<char*>(pointer) - static_cast<char*>(base_)) :
        FencedAllocator::kInvalidOffset;
  }

  
  unsigned int GetLargestFreeSize() {
    return allocator_.GetLargestFreeSize();
  }

  
  
  unsigned int GetLargestFreeOrPendingSize() {
    return allocator_.GetLargestFreeOrPendingSize();
  }

  
  
  bool CheckConsistency() {
    return allocator_.CheckConsistency();
  }

  
  bool InUse() {
    return allocator_.InUse();
  }

  FencedAllocator &allocator() { return allocator_; }

  size_t bytes_in_use() const { return allocator_.bytes_in_use(); }

 private:
  FencedAllocator allocator_;
  void* base_;
  DISALLOW_IMPLICIT_CONSTRUCTORS(FencedAllocatorWrapper);
};

}  

#endif  
