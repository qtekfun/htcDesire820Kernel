// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_INITIALIZER_H_
#define UI_BASE_IME_INPUT_METHOD_INITIALIZER_H_

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

namespace ui {

UI_EXPORT void InitializeInputMethod();

UI_EXPORT void ShutdownInputMethod();

UI_EXPORT void InitializeInputMethodForTesting();

UI_EXPORT void ShutdownInputMethodForTesting();

}  

#endif  
