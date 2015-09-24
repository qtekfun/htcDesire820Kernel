// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_CLIENT_GPU_MEMORY_BUFFER_IMPL_SHM_H_
#define CONTENT_COMMON_GPU_CLIENT_GPU_MEMORY_BUFFER_IMPL_SHM_H_

#include "content/common/gpu/client/gpu_memory_buffer_impl.h"

namespace content {

class GpuMemoryBufferImplShm : public GpuMemoryBufferImpl {
 public:
  GpuMemoryBufferImplShm(gfx::Size size, unsigned internalformat);
  virtual ~GpuMemoryBufferImplShm();

  bool Initialize(gfx::GpuMemoryBufferHandle handle);
  bool InitializeFromSharedMemory(
      scoped_ptr<base::SharedMemory> shared_memory);

  
  virtual void Map(AccessMode mode, void** vaddr) OVERRIDE;
  virtual void Unmap() OVERRIDE;
  virtual gfx::GpuMemoryBufferHandle GetHandle() const OVERRIDE;

 private:
  scoped_ptr<base::SharedMemory> shared_memory_;

  DISALLOW_COPY_AND_ASSIGN(GpuMemoryBufferImplShm);
};

}  

#endif  
