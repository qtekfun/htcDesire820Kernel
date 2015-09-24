// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_ASH_NATIVE_CURSOR_MANAGER_H_
#define ASH_WM_ASH_NATIVE_CURSOR_MANAGER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/views/corewm/native_cursor_manager.h"
#include "ui/views/corewm/native_cursor_manager_delegate.h"

namespace ash {

namespace test {
class CursorManagerTestApi;
}

class ImageCursors;

class ASH_EXPORT AshNativeCursorManager
    : public views::corewm::NativeCursorManager {
 public:
  AshNativeCursorManager();
  virtual ~AshNativeCursorManager();

 private:
  friend class test::CursorManagerTestApi;

  
  virtual void SetDisplay(
      const gfx::Display& display,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetCursor(
      gfx::NativeCursor cursor,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetVisibility(
      bool visible,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetScale(
      float scale,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetCursorSet(
      ui::CursorSetType cursor_set,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;
  virtual void SetMouseEventsEnabled(
      bool enabled,
      views::corewm::NativeCursorManagerDelegate* delegate) OVERRIDE;

  
  gfx::Point disabled_cursor_location_;

  scoped_ptr<ImageCursors> image_cursors_;

  DISALLOW_COPY_AND_ASSIGN(AshNativeCursorManager);
};

}  

#endif  
