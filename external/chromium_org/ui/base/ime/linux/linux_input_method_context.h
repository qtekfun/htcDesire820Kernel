// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_LINUX_LINUX_INPUT_METHOD_CONTEXT_H_
#define UI_BASE_IME_LINUX_LINUX_INPUT_METHOD_CONTEXT_H_

#include "base/i18n/rtl.h"
#include "base/strings/string16.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ui_export.h"

namespace gfx {
class Rect;
}

namespace ui {

struct CompositionText;
class KeyEvent;

class UI_EXPORT LinuxInputMethodContext {
 public:
  virtual ~LinuxInputMethodContext() {}

  
  
  
  virtual bool DispatchKeyEvent(const ui::KeyEvent& key_event) = 0;

  
  
  virtual void Reset() = 0;

  
  virtual base::i18n::TextDirection GetInputTextDirection() const = 0;

  
  virtual void OnTextInputTypeChanged(TextInputType text_input_type) = 0;

  
  
  virtual void OnCaretBoundsChanged(const gfx::Rect& caret_bounds) = 0;
};

class UI_EXPORT LinuxInputMethodContextDelegate {
 public:
  virtual ~LinuxInputMethodContextDelegate() {}

  
  virtual void OnCommit(const base::string16& text) = 0;

  
  virtual void OnPreeditChanged(const CompositionText& composition_text) = 0;

  
  
  virtual void OnPreeditEnd() = 0;

  
  virtual void OnPreeditStart() = 0;
};

}  

#endif  
