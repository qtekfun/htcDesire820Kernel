// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_CLIENT_TRANSFER_BUFFER_H_
#define GPU_COMMAND_BUFFER_CLIENT_TRANSFER_BUFFER_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/client/ring_buffer.h"
#include "gpu/command_buffer/common/buffer.h"
#include "gpu/command_buffer/common/gles2_cmd_utils.h"
#include "gpu/gpu_export.h"

namespace gpu {

class CommandBufferHelper;

class AlignedRingBuffer : public RingBufferWrapper {
 public:
  AlignedRingBuffer(
      unsigned int alignment,
      int32 shm_id,
      RingBuffer::Offset base_offset,
      unsigned int size,
      CommandBufferHelper* helper,
      void* base)
      : RingBufferWrapper(base_offset, size, helper, base),
        alignment_(alignment),
        shm_id_(shm_id) {
  }
  ~AlignedRingBuffer();

  
  void* Alloc(unsigned int size) {
    return RingBufferWrapper::Alloc(RoundToAlignment(size));
  }

  int32 GetShmId() const {
    return shm_id_;
  }

 private:
  unsigned int RoundToAlignment(unsigned int size) {
    return (size + alignment_ - 1) & ~(alignment_ - 1);
  }

  unsigned int alignment_;
  int32 shm_id_;
};

class GPU_EXPORT TransferBufferInterface {
 public:
  TransferBufferInterface() { }
  virtual ~TransferBufferInterface() { }

  virtual bool Initialize(
      unsigned int buffer_size,
      unsigned int result_size,
      unsigned int min_buffer_size,
      unsigned int max_buffer_size,
      unsigned int alignment,
      unsigned int size_to_flush) = 0;

  virtual int GetShmId() = 0;
  virtual void* GetResultBuffer() = 0;
  virtual int GetResultOffset() = 0;

  virtual void Free() = 0;

  virtual bool HaveBuffer() const = 0;

  
  virtual void* AllocUpTo(unsigned int size, unsigned int* size_allocated) = 0;

  
  
  virtual void* Alloc(unsigned int size) = 0;

  virtual RingBuffer::Offset GetOffset(void* pointer) const = 0;

  virtual void FreePendingToken(void* p, unsigned int token) = 0;
};

class GPU_EXPORT TransferBuffer : public TransferBufferInterface {
 public:
  TransferBuffer(CommandBufferHelper* helper);
  virtual ~TransferBuffer();

  
  virtual bool Initialize(
      unsigned int default_buffer_size,
      unsigned int result_size,
      unsigned int min_buffer_size,
      unsigned int max_buffer_size,
      unsigned int alignment,
      unsigned int size_to_flush) OVERRIDE;
  virtual int GetShmId() OVERRIDE;
  virtual void* GetResultBuffer() OVERRIDE;
  virtual int GetResultOffset() OVERRIDE;
  virtual void Free() OVERRIDE;
  virtual bool HaveBuffer() const OVERRIDE;
  virtual void* AllocUpTo(
      unsigned int size, unsigned int* size_allocated) OVERRIDE;
  virtual void* Alloc(unsigned int size) OVERRIDE;
  virtual RingBuffer::Offset GetOffset(void* pointer) const OVERRIDE;
  virtual void FreePendingToken(void* p, unsigned int token) OVERRIDE;

  
  unsigned int GetCurrentMaxAllocationWithoutRealloc() const;
  unsigned int GetMaxAllocation() const;

 private:
  
  void ReallocateRingBuffer(unsigned int size);

  void AllocateRingBuffer(unsigned int size);

  CommandBufferHelper* helper_;
  scoped_ptr<AlignedRingBuffer> ring_buffer_;

  
  unsigned int result_size_;

  
  unsigned int default_buffer_size_;

  
  unsigned int min_buffer_size_;

  
  unsigned int max_buffer_size_;

  
  unsigned int alignment_;

  
  unsigned int size_to_flush_;

  
  unsigned int bytes_since_last_flush_;

  
  gpu::Buffer buffer_;

  
  int32 buffer_id_;

  
  void* result_buffer_;

  
  uint32 result_shm_offset_;

  
  bool usable_;
};

class GPU_EXPORT ScopedTransferBufferPtr {
 public:
  ScopedTransferBufferPtr(
      unsigned int size,
      CommandBufferHelper* helper,
      TransferBufferInterface* transfer_buffer)
      : buffer_(NULL),
        size_(0),
        helper_(helper),
        transfer_buffer_(transfer_buffer) {
    Reset(size);
  }

  ~ScopedTransferBufferPtr() {
    Release();
  }

  bool valid() const {
    return buffer_ != NULL;
  }

  unsigned int size() const {
    return size_;
  }

  int shm_id() const {
    return transfer_buffer_->GetShmId();
  }

  RingBuffer::Offset offset() const {
    return transfer_buffer_->GetOffset(buffer_);
  }

  void* address() const {
    return buffer_;
  }

  void Release();

  void Reset(unsigned int new_size);

 private:
  void* buffer_;
  unsigned int size_;
  CommandBufferHelper* helper_;
  TransferBufferInterface* transfer_buffer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedTransferBufferPtr);
};

template <typename T>
class ScopedTransferBufferArray : public ScopedTransferBufferPtr {
 public:
  ScopedTransferBufferArray(
      unsigned int num_elements,
      CommandBufferHelper* helper, TransferBufferInterface* transfer_buffer)
      : ScopedTransferBufferPtr(
          num_elements * sizeof(T), helper, transfer_buffer) {
  }

  T* elements() {
    return static_cast<T*>(address());
  }

  unsigned int num_elements() const {
    return size() / sizeof(T);
  }
};

}  

#endif  
