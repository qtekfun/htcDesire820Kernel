// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ACCELERATORS_MENU_LABEL_ACCELERATOR_UTIL_LINUX_H_
#define UI_BASE_ACCELERATORS_MENU_LABEL_ACCELERATOR_UTIL_LINUX_H_

#include <string>

#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT std::string ConvertAcceleratorsFromWindowsStyle(
    const std::string& label);

UI_EXPORT std::string RemoveWindowsStyleAccelerators(const std::string& label);

UI_EXPORT std::string EscapeWindowsStyleAccelerators(const std::string& label);

}  

#endif  
