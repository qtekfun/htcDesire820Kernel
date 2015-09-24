// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OZONE_OZONE_PLATFORM_H_
#define UI_OZONE_OZONE_PLATFORM_H_

#include "base/memory/scoped_ptr.h"
#include "ui/events/ozone/event_factory_ozone.h"
#include "ui/gfx/ozone/surface_factory_ozone.h"
#include "ui/ozone/ozone_export.h"

namespace ui {

class OZONE_EXPORT OzonePlatform {
 public:
  OzonePlatform();
  virtual ~OzonePlatform();

  
  
  static void Initialize();

  
  
  
  virtual gfx::SurfaceFactoryOzone* GetSurfaceFactoryOzone() = 0;
  virtual ui::EventFactoryOzone* GetEventFactoryOzone() = 0;

 private:
  static OzonePlatform* instance_;

  DISALLOW_COPY_AND_ASSIGN(OzonePlatform);
};

}  

#endif  
