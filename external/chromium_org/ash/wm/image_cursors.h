// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_IMAGE_CURSORS_H_
#define ASH_WM_IMAGE_CURSORS_H_

#include "ash/ash_export.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Display;
}

namespace ui {
class CursorLoader;
}

namespace ash {

class ASH_EXPORT ImageCursors {
 public:
  ImageCursors();
  ~ImageCursors();

  
  
  gfx::Display GetDisplay() const;

  
  
  
  
  bool SetDisplay(const gfx::Display& display);

  
  void SetScale(float scale);

  
  void SetCursorSet(ui::CursorSetType cursor_set);

  
  void SetPlatformCursor(gfx::NativeCursor* cursor);

 private:
  
  void ReloadCursors();

  scoped_ptr<ui::CursorLoader> cursor_loader_;
  float scale_;
  ui::CursorSetType cursor_set_;

  DISALLOW_COPY_AND_ASSIGN(ImageCursors);
};

}  

#endif  
