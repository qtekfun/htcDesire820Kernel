// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_OZONE_SURFACE_LNUX_FACTORY_OZONE_H_
#define UI_GFX_OZONE_SURFACE_LNUX_FACTORY_OZONE_H_

#include "base/callback.h"
#include "base/native_library.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

class SkCanvas;

namespace gfx {
class Screen;
class VSyncProvider;

class GFX_EXPORT SurfaceFactoryOzone {
 public:
  
  enum HardwareState {
    UNINITIALIZED,
    INITIALIZED,
    FAILED,
  };

  typedef void*(*GLGetProcAddressProc)(const char* name);
  typedef base::Callback<void(base::NativeLibrary)> AddGLLibraryCallback;
  typedef base::Callback<void(GLGetProcAddressProc)>
      SetGLGetProcAddressProcCallback;

  SurfaceFactoryOzone();
  virtual ~SurfaceFactoryOzone();

  
  static SurfaceFactoryOzone* GetInstance();

  
  
  virtual const char* DefaultDisplaySpec();

  
  static void SetInstance(SurfaceFactoryOzone* impl);

  
  
  virtual gfx::Screen* CreateDesktopScreen();

  
  
  virtual HardwareState InitializeHardware() = 0;

  
  
  virtual void ShutdownHardware() = 0;

  
  
  virtual intptr_t GetNativeDisplay();

  
  
  
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() = 0;

  
  
  
  virtual gfx::AcceleratedWidget RealizeAcceleratedWidget(
      gfx::AcceleratedWidget w) = 0;

  
  
  virtual bool LoadEGLGLES2Bindings(
      AddGLLibraryCallback add_gl_library,
      SetGLGetProcAddressProcCallback set_gl_get_proc_address) = 0;

  
  
  
  virtual bool AttemptToResizeAcceleratedWidget(
      gfx::AcceleratedWidget w,
      const gfx::Rect& bounds) = 0;

  
  
  virtual bool SchedulePageFlip(gfx::AcceleratedWidget w);

  
  
  
  
  virtual SkCanvas* GetCanvasForWidget(gfx::AcceleratedWidget w);

  
  
  
  
  
  virtual gfx::VSyncProvider* GetVSyncProvider(gfx::AcceleratedWidget w) = 0;

  
  
  
  
  
  virtual const int32* GetEGLSurfaceProperties(const int32* desired_list);

  
  static SurfaceFactoryOzone* CreateTestHelper();

 private:
  static SurfaceFactoryOzone* impl_; 
};

}  

#endif  
