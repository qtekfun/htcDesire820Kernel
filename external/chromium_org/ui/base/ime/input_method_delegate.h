// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_DELEGATE_H_
#define UI_BASE_IME_INPUT_METHOD_DELEGATE_H_

#include "base/event_types.h"
#include "ui/base/ui_export.h"
#include "ui/events/event_constants.h"
#include "ui/events/keycodes/keyboard_codes.h"

namespace ui {
namespace internal {

class UI_EXPORT InputMethodDelegate {
 public:
  virtual ~InputMethodDelegate() {}

  
  
  virtual bool DispatchKeyEventPostIME(
      const base::NativeEvent& native_key_event) = 0;

  
  virtual bool DispatchFabricatedKeyEventPostIME(ui::EventType type,
                                                 ui::KeyboardCode key_code,
                                                 int flags) = 0;
};

}  
}  

#endif  
