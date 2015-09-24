// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_NATIVE_CURSOR_MANAGER_H_
#define UI_VIEWS_COREWM_NATIVE_CURSOR_MANAGER_H_

#include "base/strings/string16.h"
#include "ui/views/corewm/native_cursor_manager_delegate.h"
#include "ui/views/views_export.h"

namespace gfx {
class Display;
}

namespace views {
namespace corewm {

class VIEWS_EXPORT NativeCursorManager {
 public:
  virtual ~NativeCursorManager() {}

  
  virtual void SetDisplay(
      const gfx::Display& display,
      views::corewm::NativeCursorManagerDelegate* delegate) = 0;

  
  
  
  virtual void SetCursor(
      gfx::NativeCursor cursor,
      views::corewm::NativeCursorManagerDelegate* delegate) = 0;

  
  
  
  virtual void SetVisibility(
    bool visible,
    views::corewm::NativeCursorManagerDelegate* delegate) = 0;

  
  virtual void SetScale(
      float scale,
      views::corewm::NativeCursorManagerDelegate* delegate) = 0;

  
  virtual void SetCursorSet(
      ui::CursorSetType cursor_set,
      views::corewm::NativeCursorManagerDelegate* delegate) = 0;

  
  
  
  virtual void SetMouseEventsEnabled(
      bool enabled,
      views::corewm::NativeCursorManagerDelegate* delegate) = 0;
};

}  
}  

#endif  
