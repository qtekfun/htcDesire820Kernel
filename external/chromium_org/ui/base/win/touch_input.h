// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_TOUCH_INPUT_H_
#define UI_BASE_WIN_TOUCH_INPUT_H_

#include <windows.h>

#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT BOOL GetTouchInputInfoWrapper(HTOUCHINPUT handle,
                                        UINT count,
                                        PTOUCHINPUT pointer,
                                        int size);

}  

#endif  
