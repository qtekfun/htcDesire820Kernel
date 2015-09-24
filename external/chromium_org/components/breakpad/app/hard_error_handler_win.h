// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BREAKPAD_APP_HARD_ERROR_HANDLER_WIN_H_
#define COMPONENTS_BREAKPAD_APP_HARD_ERROR_HANDLER_WIN_H_

#include <windows.h>

namespace breakpad {

bool HardErrorHandler(EXCEPTION_POINTERS* ex_info);

}  

#endif  
