// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_TOOLTIP_H_
#define UI_VIEWS_COREWM_TOOLTIP_H_

#include "base/strings/string16.h"
#include "ui/views/views_export.h"

namespace aura {
class Window;
}

namespace gfx {
class Point;
}

namespace views {
namespace corewm {

class VIEWS_EXPORT Tooltip {
 public:
  virtual ~Tooltip() {}

  
  virtual void SetText(aura::Window* window,
                       const string16& tooltip_text,
                       const gfx::Point& location) = 0;

  
  virtual void Show() = 0;

  
  virtual void Hide() = 0;

  
  virtual bool IsVisible() = 0;
};

}  
}  

#endif  
