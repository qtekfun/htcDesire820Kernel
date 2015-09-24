// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_OZONE_DRI_DRI_SURFACE_FACTORY_H_
#define UI_GFX_OZONE_DRI_DRI_SURFACE_FACTORY_H_

#include "base/memory/scoped_ptr.h"
#include "ui/gfx/ozone/surface_factory_ozone.h"

namespace gfx {

class DriSurface;
class DriWrapper;
class HardwareDisplayController;

class GFX_EXPORT DriSurfaceFactory : public SurfaceFactoryOzone {
 public:
  DriSurfaceFactory();
  virtual ~DriSurfaceFactory();

  virtual HardwareState InitializeHardware() OVERRIDE;
  virtual void ShutdownHardware() OVERRIDE;

  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual gfx::AcceleratedWidget RealizeAcceleratedWidget(
      gfx::AcceleratedWidget w) OVERRIDE;

  virtual bool LoadEGLGLES2Bindings(
      AddGLLibraryCallback add_gl_library,
      SetGLGetProcAddressProcCallback set_gl_get_proc_address) OVERRIDE;

  virtual bool AttemptToResizeAcceleratedWidget(
      gfx::AcceleratedWidget w,
      const gfx::Rect& bounds) OVERRIDE;

  virtual bool SchedulePageFlip(gfx::AcceleratedWidget w) OVERRIDE;

  virtual SkCanvas* GetCanvasForWidget(gfx::AcceleratedWidget w) OVERRIDE;

  virtual gfx::VSyncProvider* GetVSyncProvider(
      gfx::AcceleratedWidget w) OVERRIDE;

 private:
  virtual DriSurface* CreateSurface(
    HardwareDisplayController* controller);

  virtual DriWrapper* CreateWrapper();

  virtual bool InitializeControllerForPrimaryDisplay(
    DriWrapper* drm,
    HardwareDisplayController* controller);

  
  
  
  
  virtual void WaitForPageFlipEvent(int fd);

  scoped_ptr<DriWrapper> drm_;

  HardwareState state_;

  
  scoped_ptr<HardwareDisplayController> controller_;

  DISALLOW_COPY_AND_ASSIGN(DriSurfaceFactory);
};

}  

#endif  
