// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_MESSAGE_BOX_WIN_H_
#define UI_BASE_WIN_MESSAGE_BOX_WIN_H_

#include <windows.h>

#include "base/strings/string16.h"
#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT int MessageBox(HWND hwnd,
                         const string16& text,
                         const string16& caption,
                         UINT flags);

}  

#endif  
