// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_IME_INPUT_METHOD_DELEGATE_H_
#define UI_VIEWS_IME_INPUT_METHOD_DELEGATE_H_

#include "ui/views/views_export.h"

namespace ui {
class KeyEvent;
}

namespace views {

namespace internal {

class VIEWS_EXPORT InputMethodDelegate {
 public:
  virtual ~InputMethodDelegate() {}

  
  virtual void DispatchKeyEventPostIME(const ui::KeyEvent& key) = 0;
};

}  
}  

#endif  
