// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_CURSOR_CURSOR_LOADER_X11_H_
#define UI_BASE_CURSOR_CURSOR_LOADER_X11_H_

#include <X11/Xcursor/Xcursor.h>
#include <map>

#include "base/compiler_specific.h"
#include "ui/base/cursor/cursor.h"
#include "ui/base/cursor/cursor_loader.h"
#include "ui/base/ui_export.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/display.h"

namespace ui {

class UI_EXPORT CursorLoaderX11 : public CursorLoader {
 public:
  CursorLoaderX11();
  virtual ~CursorLoaderX11();

  
  virtual void LoadImageCursor(int id,
                               int resource_id,
                               const gfx::Point& hot) OVERRIDE;
  virtual void LoadAnimatedCursor(int id,
                                  int resource_id,
                                  const gfx::Point& hot,
                                  int frame_delay_ms) OVERRIDE;
  virtual void UnloadAll() OVERRIDE;
  virtual void SetPlatformCursor(gfx::NativeCursor* cursor) OVERRIDE;

 private:
  
  bool IsImageCursor(gfx::NativeCursor native_cursor);

  
  ::Cursor ImageCursorFromNative(gfx::NativeCursor native_cursor);

  
  typedef std::map<int, ::Cursor> ImageCursorMap;
  ImageCursorMap cursors_;

  
  
  
  typedef std::map<int, std::pair< ::Cursor, XcursorImages*> >
      AnimatedCursorMap;
  AnimatedCursorMap animated_cursors_;

  const XScopedCursor invisible_cursor_;

  DISALLOW_COPY_AND_ASSIGN(CursorLoaderX11);
};

UI_EXPORT void ScaleAndRotateCursorBitmapAndHotpoint(
    float scale,
    gfx::Display::Rotation rotation,
    SkBitmap* bitmap_in_out,
    gfx::Point* hotpoint_in_out);

}  

#endif  
