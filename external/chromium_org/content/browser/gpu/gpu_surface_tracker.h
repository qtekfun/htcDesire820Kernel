// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_GPU_GPU_SURFACE_TRACKER_H_
#define CONTENT_BROWSER_GPU_GPU_SURFACE_TRACKER_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "content/common/gpu/gpu_surface_lookup.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

namespace content {

class GpuSurfaceTracker : public GpuSurfaceLookup {
 public:
  
  
  
  
  
  
  
  
  
  
  
  class SurfaceRef : public base::RefCountedThreadSafe<SurfaceRef> {
   protected:
    SurfaceRef() { }
    virtual ~SurfaceRef() { }

   private:
    friend class base::RefCountedThreadSafe<SurfaceRef>;
    DISALLOW_COPY_AND_ASSIGN(SurfaceRef);
  };

  
  
  virtual gfx::AcceleratedWidget AcquireNativeWidget(int surface_id) OVERRIDE;

  
  static GpuSurfaceTracker* Get() { return GetInstance(); }

  
  
  
  int AddSurfaceForRenderer(int renderer_id, int render_widget_id);

  
  
  
  int LookupSurfaceForRenderer(int renderer_id, int render_widget_id);

  
  int AddSurfaceForNativeWidget(gfx::AcceleratedWidget widget);

  
  void RemoveSurface(int surface_id);

  
  
  
  bool GetRenderWidgetIDForSurface(int surface_id,
                                   int* renderer_id,
                                   int* render_widget_id);

  
  
  void SetSurfaceHandle(int surface_id, const gfx::GLSurfaceHandle& handle);

  
  void SetNativeWidget(
      int surface_id,
      gfx::AcceleratedWidget widget,
      SurfaceRef* surface_ref);

  
  
  gfx::GLSurfaceHandle GetSurfaceHandle(int surface_id);

  
  std::size_t GetSurfaceCount();

  
  
  static GpuSurfaceTracker* GetInstance();

  scoped_refptr<SurfaceRef> GetSurfaceRefForSurface(int surface_id) {
    return surface_map_[surface_id].surface_ref;
  }

 private:
  struct SurfaceInfo {
    SurfaceInfo();
    SurfaceInfo(int renderer_id,
                int render_widget_id,
                const gfx::AcceleratedWidget& native_widget,
                const gfx::GLSurfaceHandle& handle,
                const scoped_refptr<SurfaceRef>& surface_ref);
    ~SurfaceInfo();
    int renderer_id;
    int render_widget_id;
    gfx::AcceleratedWidget native_widget;
    gfx::GLSurfaceHandle handle;
    scoped_refptr<SurfaceRef> surface_ref;
  };
  typedef std::map<int, SurfaceInfo> SurfaceMap;

  friend struct DefaultSingletonTraits<GpuSurfaceTracker>;

  GpuSurfaceTracker();
  virtual ~GpuSurfaceTracker();

  base::Lock lock_;
  SurfaceMap surface_map_;
  int next_surface_id_;

  DISALLOW_COPY_AND_ASSIGN(GpuSurfaceTracker);
};

}  

#endif  
