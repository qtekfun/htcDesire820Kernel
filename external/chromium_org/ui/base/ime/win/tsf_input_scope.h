// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_WIN_TSF_INPUT_SCOPE_H_
#define UI_BASE_IME_WIN_TSF_INPUT_SCOPE_H_

#include <InputScope.h>
#include <Windows.h>
#include <vector>

#include "base/basictypes.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ui_export.h"

namespace ui {
namespace tsf_inputscope {

UI_EXPORT std::vector<InputScope> GetInputScopes(TextInputType text_input_type,
                                                 TextInputMode text_input_mode);

UI_EXPORT ITfInputScope* CreateInputScope(TextInputType text_input_type,
                                          TextInputMode text_input_mode);

UI_EXPORT void SetInputScopeForTsfUnawareWindow(
    HWND window_handle,
    TextInputType text_input_type,
    TextInputMode text_input_mode);

}  
}  

#endif  
