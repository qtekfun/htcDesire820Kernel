// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_DISPLAY_OBSERVER_H_
#define UI_GFX_DISPLAY_OBSERVER_H_

#include "ui/gfx/gfx_export.h"

namespace gfx {
class Display;

class GFX_EXPORT DisplayObserver {
 public:
  
  virtual void OnDisplayBoundsChanged(const Display& display) = 0;

  
  virtual void OnDisplayAdded(const Display& new_display) = 0;

  
  virtual void OnDisplayRemoved(const Display& old_display) = 0;

 protected:
  virtual ~DisplayObserver();
};

}  

#endif  
