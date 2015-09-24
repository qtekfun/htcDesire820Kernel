// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_RENDERER_RENDER_PROCESS_IMPL_H_
#define CONTENT_RENDERER_RENDER_PROCESS_IMPL_H_

#include "base/timer/timer.h"
#include "content/renderer/render_process.h"

class SkCanvas;

namespace content {

class RenderProcessImpl : public RenderProcess {
 public:
  RenderProcessImpl();
  virtual ~RenderProcessImpl();

  
  virtual SkCanvas* GetDrawingCanvas(
      TransportDIB** memory,
      const gfx::Rect& rect) OVERRIDE;
  virtual void ReleaseTransportDIB(TransportDIB* memory) OVERRIDE;
  virtual bool UseInProcessPlugins() const OVERRIDE;
  virtual void AddBindings(int bindings) OVERRIDE;
  virtual int GetEnabledBindings() const OVERRIDE;
  virtual TransportDIB* CreateTransportDIB(size_t size) OVERRIDE;
  virtual void FreeTransportDIB(TransportDIB*) OVERRIDE;

  
  
  
  static bool InProcessPlugins();

 private:
  
  
  
  
  bool GetTransportDIBFromCache(TransportDIB** result, size_t size);

  
  
  
  bool PutSharedMemInCache(TransportDIB* memory);

  void ClearTransportDIBCache();

  
  
  
  int FindFreeCacheSlot(size_t size);

  
  
  TransportDIB* shared_mem_cache_[2];

  
  base::DelayTimer<RenderProcessImpl> shared_mem_cache_cleaner_;

  
  uint32 transport_dib_next_sequence_number_;

  bool in_process_plugins_;

  
  
  int enabled_bindings_;

  DISALLOW_COPY_AND_ASSIGN(RenderProcessImpl);
};

}  

#endif  
