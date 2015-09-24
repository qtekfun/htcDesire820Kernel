// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SYS_COLOR_CHANGE_LISTENER_H_
#define UI_GFX_SYS_COLOR_CHANGE_LISTENER_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

GFX_EXPORT bool IsInvertedColorScheme();

class GFX_EXPORT SysColorChangeListener {
 public:
  virtual void OnSysColorChange() = 0;

 protected:
  virtual ~SysColorChangeListener() {}
};

class GFX_EXPORT ScopedSysColorChangeListener {
 public:
  explicit ScopedSysColorChangeListener(SysColorChangeListener* listener);
  ~ScopedSysColorChangeListener();

 private:
  SysColorChangeListener* listener_;

  DISALLOW_COPY_AND_ASSIGN(ScopedSysColorChangeListener);
};

}  

#endif  
