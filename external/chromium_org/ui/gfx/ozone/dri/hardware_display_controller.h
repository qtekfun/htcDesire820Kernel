// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_OZONE_DRI_HARDWARE_DISPLAY_CONTROLLER_H_
#define UI_GFX_OZONE_DRI_HARDWARE_DISPLAY_CONTROLLER_H_

#include <stddef.h>
#include <stdint.h>
#include <xf86drmMode.h>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/ozone/dri/dri_wrapper.h"

namespace gfx {

class DriSurface;

class GFX_EXPORT HardwareDisplayController {
 public:
  
  enum State {
    
    
    UNASSOCIATED,

    
    
    
    UNINITIALIZED,

    
    
    
    SURFACE_INITIALIZED,

    
    INITIALIZED,

    
    FAILED,
  };

  HardwareDisplayController();

  ~HardwareDisplayController();

  
  
  
  void SetControllerInfo(DriWrapper* drm,
                         uint32_t connector_id,
                         uint32_t crtc_id,
                         uint32_t dpms_property_id,
                         drmModeModeInfo mode);

  
  bool BindSurfaceToController(scoped_ptr<DriSurface> surface);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool SchedulePageFlip();

  
  
  
  
  
  
  void OnPageFlipEvent(unsigned int frame,
                       unsigned int seconds,
                       unsigned int useconds);

  State get_state() const { return state_; };

  int get_fd() const { return drm_->get_fd(); };

  const drmModeModeInfo& get_mode() const { return mode_; };

  DriSurface* get_surface() const { return surface_.get(); };

  uint64_t get_time_of_last_flip() const {
    return time_of_last_flip_;
  };

 private:
  
  
  DriWrapper* drm_;

  
  uint32_t connector_id_;

  uint32_t crtc_id_;

  uint32_t dpms_property_id_;

  
  drmModeModeInfo mode_;

  
  
  drmModeCrtc* saved_crtc_;

  State state_;

  scoped_ptr<DriSurface> surface_;

  uint64_t time_of_last_flip_;

  DISALLOW_COPY_AND_ASSIGN(HardwareDisplayController);
};

}  

#endif  
