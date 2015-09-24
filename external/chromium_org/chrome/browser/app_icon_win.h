// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_APP_ICON_WIN_H_
#define CHROME_BROWSER_APP_ICON_WIN_H_

#include <windows.h>

#include "base/memory/scoped_ptr.h"

class SkBitmap;

HICON GetAppIcon();

scoped_ptr<SkBitmap> GetAppIconForSize(int size);

#endif  
