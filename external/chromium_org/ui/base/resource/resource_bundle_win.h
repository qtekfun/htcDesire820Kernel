// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_RESOURCE_RESOURCE_BUNDLE_WIN_
#define UI_BASE_RESOURCE_RESOURCE_BUNDLE_WIN_

#include "build/build_config.h"

#include <windows.h>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT void SetResourcesDataDLL(HINSTANCE handle);

UI_EXPORT HICON LoadThemeIconFromResourcesDataDLL(int icon_id);

}  

#endif  
