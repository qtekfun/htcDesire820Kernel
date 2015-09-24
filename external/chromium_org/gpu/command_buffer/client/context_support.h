// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_CLIENT_CONTEXT_SUPPORT_H_
#define GPU_COMMAND_BUFFER_CLIENT_CONTEXT_SUPPORT_H_

#include "base/callback.h"
#include "ui/gfx/rect.h"

namespace gpu {
struct ManagedMemoryStats;

class ContextSupport {
 public:
  
  virtual void SignalSyncPoint(uint32 sync_point,
                               const base::Closure& callback) = 0;

  
  
  virtual void SignalQuery(uint32 query, const base::Closure& callback) = 0;

  
  
  virtual void SetSurfaceVisible(bool visible) = 0;

  virtual void SendManagedMemoryStats(const ManagedMemoryStats& stats) = 0;

  virtual void Swap() = 0;
  virtual void PartialSwapBuffers(gfx::Rect sub_buffer) = 0;

  virtual void SetSwapBuffersCompleteCallback(
      const base::Closure& callback) = 0;

 protected:
  ContextSupport() {}
  virtual ~ContextSupport() {}
};

}

#endif  
