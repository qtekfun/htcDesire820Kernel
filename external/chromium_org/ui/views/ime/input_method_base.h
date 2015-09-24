// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_IME_INPUT_METHOD_BASE_H_
#define UI_VIEWS_IME_INPUT_METHOD_BASE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/ime/input_method.h"
#include "ui/views/ime/input_method_delegate.h"

namespace gfx {
class Rect;
}

namespace ui {
class KeyEvent;
}

namespace views {

class VIEWS_EXPORT InputMethodBase : public InputMethod,
                                     public FocusChangeListener {
 public:
  InputMethodBase();
  virtual ~InputMethodBase();

  
  virtual void SetDelegate(internal::InputMethodDelegate* delegate) OVERRIDE;
  virtual void Init(Widget* widget) OVERRIDE;
  virtual void OnTextInputTypeChanged(View* view) OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() const OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual bool IsMock() const OVERRIDE;

  
  virtual void OnWillChangeFocus(View* focused_before, View* focused) OVERRIDE;
  virtual void OnDidChangeFocus(View* focused_before, View* focused) OVERRIDE;

 protected:
  internal::InputMethodDelegate* delegate() const { return delegate_; }
  Widget* widget() const { return widget_; }
  View* GetFocusedView() const;

  
  bool IsViewFocused(View* view) const;

  
  bool IsTextInputTypeNone() const;

  
  
  void OnInputMethodChanged() const;

  
  void DispatchKeyEventPostIME(const ui::KeyEvent& key) const;

  
  
  bool GetCaretBoundsInWidget(gfx::Rect* rect) const;

  
  
  
  void DetachFromWidget();

 private:
  internal::InputMethodDelegate* delegate_;
  Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodBase);
};

}  

#endif  
