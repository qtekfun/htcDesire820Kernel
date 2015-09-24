// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WM_TYPES_H_
#define ASH_WM_WM_TYPES_H_

#include "ui/base/ui_base_types.h"

namespace ash {
namespace wm {

enum WindowShowType {
  
  SHOW_TYPE_DEFAULT = 0,

  
  
  SHOW_TYPE_NORMAL,
  SHOW_TYPE_MINIMIZED,
  SHOW_TYPE_MAXIMIZED,
  SHOW_TYPE_INACTIVE,
  SHOW_TYPE_FULLSCREEN,
  SHOW_TYPE_DETACHED,
  SHOW_TYPE_END,  

  

  SHOW_TYPE_LEFT_SNAPPED,
  SHOW_TYPE_RIGHT_SNAPPED,

  
  
  
  SHOW_TYPE_AUTO_POSITIONED,
};

WindowShowType ToWindowShowType(ui::WindowShowState state);
ui::WindowShowState ToWindowShowState(WindowShowType type);

}  
}  

#endif  
