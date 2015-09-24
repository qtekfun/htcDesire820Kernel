// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_BINDINGS_LIB_BUFFER_H_
#define MOJO_PUBLIC_BINDINGS_LIB_BUFFER_H_

#include <stddef.h>

#include <deque>

#include "mojo/public/system/macros.h"

namespace mojo {

class Buffer {
 public:
  typedef void (*Destructor)(void* address);

  Buffer();
  virtual ~Buffer();

  virtual void* Allocate(size_t num_bytes, Destructor func = NULL) = 0;

  static Buffer* current();

 private:
  Buffer* previous_;
};

namespace internal {

class ScratchBuffer : public Buffer {
 public:
  ScratchBuffer();
  virtual ~ScratchBuffer();

  virtual void* Allocate(size_t num_bytes, Destructor func = NULL)
      MOJO_OVERRIDE;

 private:
  enum { kMinSegmentSize = 512 };

  struct Segment {
    Segment* next;
    char* cursor;
    char* end;
  };

  void* AllocateInSegment(Segment* segment, size_t num_bytes);
  void AddOverflowSegment(size_t delta);

  char fixed_data_[kMinSegmentSize];
  Segment fixed_;
  Segment* overflow_;

  struct PendingDestructor {
    Destructor func;
    void* address;
  };
  std::deque<PendingDestructor> pending_dtors_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(ScratchBuffer);
};

class FixedBuffer : public Buffer {
 public:
  explicit FixedBuffer(size_t size);
  virtual ~FixedBuffer();

  
  
  
  virtual void* Allocate(size_t num_bytes, Destructor func = NULL)
      MOJO_OVERRIDE;

  size_t size() const { return size_; }

  
  
  
  
  void* Leak();

 private:
  char* ptr_;
  size_t cursor_;
  size_t size_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(FixedBuffer);
};

}  

class AllocationScope {
 public:
  AllocationScope() {}
  ~AllocationScope() {}

  Buffer* buffer() { return &buffer_; }

 private:
  internal::ScratchBuffer buffer_;

  MOJO_DISALLOW_COPY_AND_ASSIGN(AllocationScope);
};

}  

#endif  
