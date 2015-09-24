// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_OBSERVER_H_
#define UI_BASE_IME_INPUT_METHOD_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace ui {

class InputMethod;
class TextInputClient;

class UI_EXPORT InputMethodObserver {
 public:
  virtual ~InputMethodObserver() {}

  
  
  virtual void OnTextInputTypeChanged(const TextInputClient* client) = 0;

  
  
  virtual void OnFocus() = 0;

  
  
  virtual void OnBlur() = 0;

  
  
  virtual void OnCaretBoundsChanged(const TextInputClient* client) = 0;

  
  
  
  virtual void OnTextInputStateChanged(const TextInputClient* client) = 0;

  
  virtual void OnInputMethodDestroyed(const InputMethod* input_method) = 0;
};

}  

#endif  
