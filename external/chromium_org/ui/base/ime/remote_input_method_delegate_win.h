// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_REMOTE_INPUT_METHOD_DELEGATE_WIN_H_
#define UI_BASE_IME_REMOTE_INPUT_METHOD_DELEGATE_WIN_H_

#include <vector>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/rect.h"

namespace ui {
namespace internal {

class UI_EXPORT RemoteInputMethodDelegateWin {
 public:
  virtual ~RemoteInputMethodDelegateWin() {}

  
  virtual void CancelComposition() = 0;

  
  
  
  
  
  
  
  virtual void OnTextInputClientUpdated(
      const std::vector<int32>& input_scopes,
      const std::vector<gfx::Rect>& composition_character_bounds) = 0;
};

}  
}  

#endif  
