// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GPU_MEMORY_BUFFER_H_
#define UI_GFX_GPU_MEMORY_BUFFER_H_

#include "base/memory/shared_memory.h"
#include "build/build_config.h"
#include "ui/gfx/gfx_export.h"

#if defined(OS_ANDROID)
#include <third_party/khronos/EGL/egl.h>
#endif

namespace gfx {

enum GpuMemoryBufferType {
  EMPTY_BUFFER,
  SHARED_MEMORY_BUFFER,
  EGL_CLIENT_BUFFER,
  IO_SURFACE_BUFFER
};

struct GpuMemoryBufferHandle {
  GpuMemoryBufferHandle()
      : type(EMPTY_BUFFER),
        handle(base::SharedMemory::NULLHandle())
#if defined(OS_ANDROID)
        , native_buffer(NULL)
#endif
#if defined(OS_MACOSX)
        , io_surface_id(0)
#endif
  {
  }
  bool is_null() const { return type == EMPTY_BUFFER; }
  GpuMemoryBufferType type;
  base::SharedMemoryHandle handle;
#if defined(OS_ANDROID)
  EGLClientBuffer native_buffer;
#endif
#if defined(OS_MACOSX)
  uint32 io_surface_id;
#endif

};

class GFX_EXPORT GpuMemoryBuffer {
 public:
  enum AccessMode {
    READ_ONLY,
    WRITE_ONLY,
    READ_WRITE,
  };

  GpuMemoryBuffer();
  virtual ~GpuMemoryBuffer();

  
  
  
  virtual void Map(AccessMode mode, void** vaddr) = 0;

  
  
  virtual void Unmap() = 0;

  
  virtual bool IsMapped() const = 0;

  
  virtual uint32 GetStride() const = 0;

  
  virtual GpuMemoryBufferHandle GetHandle() const = 0;
};

}  

#endif  
