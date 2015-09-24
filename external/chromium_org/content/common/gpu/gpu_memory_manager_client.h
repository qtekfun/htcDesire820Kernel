// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_GPU_GPU_MEMORY_MANAGER_CLIENT_H_
#define CONTENT_COMMON_GPU_GPU_MEMORY_MANAGER_CLIENT_H_

#include <list>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "gpu/command_buffer/common/gpu_memory_allocation.h"
#include "gpu/command_buffer/service/memory_tracking.h"
#include "ui/gfx/size.h"

namespace content {

class GpuMemoryManager;
class GpuMemoryTrackingGroup;

class CONTENT_EXPORT GpuMemoryManagerClient {
 public:
  virtual ~GpuMemoryManagerClient() {}

  
  virtual gfx::Size GetSurfaceSize() const = 0;

  
  virtual gpu::gles2::MemoryTracker* GetMemoryTracker() const = 0;

  
  virtual void SetMemoryAllocation(
      const gpu::MemoryAllocation& allocation) = 0;

  virtual void SuggestHaveFrontBuffer(bool suggest_have_frontbuffer) = 0;

  
  
  
  virtual bool GetTotalGpuMemory(uint64* bytes) = 0;
};

class CONTENT_EXPORT GpuMemoryManagerClientState {
 public:
  ~GpuMemoryManagerClientState();
  void SetVisible(bool visible);
  void SetManagedMemoryStats(const gpu::ManagedMemoryStats& stats);

 private:
  friend class GpuMemoryManager;

  GpuMemoryManagerClientState(GpuMemoryManager* memory_manager,
                              GpuMemoryManagerClient* client,
                              GpuMemoryTrackingGroup* tracking_group,
                              bool has_surface,
                              bool visible);

  
  GpuMemoryManager* memory_manager_;

  
  GpuMemoryManagerClient* client_;

  
  GpuMemoryTrackingGroup* tracking_group_;

  
  const bool has_surface_;

  
  bool visible_;

  
  
  
  
  std::list<GpuMemoryManagerClientState*>::iterator list_iterator_;
  bool list_iterator_valid_;

  
  gpu::ManagedMemoryStats managed_memory_stats_;
  bool managed_memory_stats_received_;

  
  
  uint64 bytes_nicetohave_limit_low_;
  uint64 bytes_nicetohave_limit_high_;

  
  
  uint64 bytes_allocation_when_visible_;

  
  
  uint64 bytes_allocation_ideal_nicetohave_;
  uint64 bytes_allocation_ideal_required_;
  uint64 bytes_allocation_ideal_minimum_;

  
  
  bool hibernated_;
};

}  

#endif  
