// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_PATH_WIN_H_
#define UI_GFX_PATH_WIN_H_

#include <windows.h>

#include "ui/gfx/gfx_export.h"

class SkPath;
class SkRegion;

namespace gfx {

GFX_EXPORT HRGN CreateHRGNFromSkRegion(const SkRegion& path);

GFX_EXPORT HRGN CreateHRGNFromSkPath(const SkPath& path);

}  

#endif  
