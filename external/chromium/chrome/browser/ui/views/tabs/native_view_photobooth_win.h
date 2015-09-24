// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_NATIVE_VIEW_PHOTOBOOTH_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_NATIVE_VIEW_PHOTOBOOTH_WIN_H_
#pragma once

#include "chrome/browser/ui/views/tabs/native_view_photobooth.h"

namespace views {
class Widget;
}

class NativeViewPhotoboothWin : public NativeViewPhotobooth {
 public:
  
  
  
  
  
  explicit NativeViewPhotoboothWin(gfx::NativeView initial_view);

  
  virtual ~NativeViewPhotoboothWin();

  
  virtual void Replace(gfx::NativeView new_view) OVERRIDE;

  
  
  virtual void PaintScreenshotIntoCanvas(
      gfx::Canvas* canvas,
      const gfx::Rect& target_bounds) OVERRIDE;

 private:
  
  void CreateCaptureWindow(HWND initial_hwnd);

  
  views::Widget* capture_window_;

  
  HWND current_hwnd_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewPhotoboothWin);
};

#endif  
