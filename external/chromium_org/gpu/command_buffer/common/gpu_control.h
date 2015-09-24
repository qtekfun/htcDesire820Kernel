// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_COMMON_GPU_CONTROL_H_
#define GPU_COMMAND_BUFFER_COMMON_GPU_CONTROL_H_

#include <vector>

#include "base/callback.h"
#include "gpu/command_buffer/common/capabilities.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "gpu/command_buffer/common/types.h"
#include "gpu/gpu_export.h"

namespace gfx {
class GpuMemoryBuffer;
}

namespace gpu {
struct ManagedMemoryStats;

class GPU_EXPORT GpuControl {
 public:
  GpuControl() {}
  virtual ~GpuControl() {}

  virtual Capabilities GetCapabilities() = 0;

  
  
  virtual gfx::GpuMemoryBuffer* CreateGpuMemoryBuffer(
      size_t width,
      size_t height,
      unsigned internalformat,
      int32* id) = 0;

  
  virtual void DestroyGpuMemoryBuffer(int32 id) = 0;

  
  
  virtual bool GenerateMailboxNames(unsigned num,
                                    std::vector<gpu::Mailbox>* names) = 0;

  
  
  virtual uint32 InsertSyncPoint() = 0;

  
  virtual void SignalSyncPoint(uint32 sync_point,
                               const base::Closure& callback) = 0;

  
  
  virtual void SignalQuery(uint32 query, const base::Closure& callback) = 0;

  virtual void SetSurfaceVisible(bool visible) = 0;

  virtual void SendManagedMemoryStats(const ManagedMemoryStats& stats) = 0;

  
  virtual void Echo(const base::Closure& callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(GpuControl);
};

}  

#endif  
