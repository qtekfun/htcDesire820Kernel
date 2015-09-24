// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NATIVE_DIALOG_WINDOW_H_
#define CHROME_BROWSER_CHROMEOS_NATIVE_DIALOG_WINDOW_H_
#pragma once

#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Rect;
class Size;
}  

namespace chromeos {

enum NativeDialogFlags {
  DIALOG_FLAG_DEFAULT    = 0x00,  
  DIALOG_FLAG_RESIZEABLE = 0x01,  
  DIALOG_FLAG_MODAL      = 0x02,  
};

void ShowNativeDialog(gfx::NativeWindow parent,
                      gfx::NativeView native_dialog,
                      int flags,
                      const gfx::Size& size,
                      const gfx::Size& min_size);

gfx::NativeWindow GetNativeDialogWindow(gfx::NativeView native_dialog);

gfx::Rect GetNativeDialogContentsBounds(gfx::NativeView native_dialog);

}  

#endif  
