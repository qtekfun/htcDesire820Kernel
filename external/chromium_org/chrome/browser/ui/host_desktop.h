// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_HOST_DESKTOP_H_
#define CHROME_BROWSER_UI_HOST_DESKTOP_H_

#include "ui/gfx/native_widget_types.h"

class Browser;

namespace chrome {

enum HostDesktopType {
  HOST_DESKTOP_TYPE_FIRST = 0,

  
  HOST_DESKTOP_TYPE_NATIVE = HOST_DESKTOP_TYPE_FIRST,

  
#if defined(OS_CHROMEOS)
  HOST_DESKTOP_TYPE_ASH = HOST_DESKTOP_TYPE_NATIVE,
#else
  HOST_DESKTOP_TYPE_ASH,
#endif

  HOST_DESKTOP_TYPE_COUNT
};

HostDesktopType GetHostDesktopTypeForNativeView(gfx::NativeView native_view);
HostDesktopType GetHostDesktopTypeForNativeWindow(
    gfx::NativeWindow native_window);

HostDesktopType GetActiveDesktop();

}  

#endif  
