// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OZONE_PLATFORM_DRI_OZONE_PLATFORM_DRI_H_
#define UI_OZONE_PLATFORM_DRI_OZONE_PLATFORM_DRI_H_

#include "ui/events/ozone/evdev/event_factory.h"
#include "ui/gfx/ozone/dri/dri_surface_factory.h"
#include "ui/ozone/ozone_platform.h"

namespace ui {

class OzonePlatformDri : public OzonePlatform {
 public:
  OzonePlatformDri();
  virtual ~OzonePlatformDri();

  virtual gfx::SurfaceFactoryOzone* GetSurfaceFactoryOzone() OVERRIDE;
  virtual ui::EventFactoryOzone* GetEventFactoryOzone() OVERRIDE;

 private:
  gfx::DriSurfaceFactory surface_factory_ozone_;
  ui::EventFactoryEvdev event_factory_ozone_;

  DISALLOW_COPY_AND_ASSIGN(OzonePlatformDri);
};

OzonePlatform* CreateOzonePlatformDri();

}  

#endif  
