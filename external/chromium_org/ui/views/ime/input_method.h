// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_IME_INPUT_METHOD_H_
#define UI_VIEWS_IME_INPUT_METHOD_H_

#include <string>

#include "base/basictypes.h"
#include "base/event_types.h"
#include "base/i18n/rtl.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/views/views_export.h"

namespace ui {
class KeyEvent;
class TextInputClient;
}  

namespace views {

namespace internal {
class InputMethodDelegate;
}  

class View;
class Widget;

class VIEWS_EXPORT InputMethod {
 public:
  
#if defined(OS_WIN)
  typedef LRESULT NativeEventResult;
#else
  typedef int32 NativeEventResult;
#endif

  virtual ~InputMethod() {}

  
  
  virtual void SetDelegate(internal::InputMethodDelegate* delegate) = 0;

  
  
  virtual void Init(Widget* widget) = 0;

  
  
  virtual void OnFocus() = 0;
  virtual void OnBlur() = 0;

  
  
  
  
  virtual bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                        NativeEventResult* result) = 0;

  
  
  
  
  
  virtual void DispatchKeyEvent(const ui::KeyEvent& key) = 0;

  
  
  
  
  virtual void OnTextInputTypeChanged(View* view) = 0;

  
  
  virtual void OnCaretBoundsChanged(View* view) = 0;

  
  
  virtual void CancelComposition(View* view) = 0;

  
  
  
  
  virtual void OnInputLocaleChanged() = 0;

  
  
  virtual std::string GetInputLocale() = 0;

  
  
  virtual base::i18n::TextDirection GetInputTextDirection() = 0;

  
  
  
  
  
  virtual bool IsActive() = 0;

  
  
  virtual ui::TextInputClient* GetTextInputClient() const = 0;

  
  
  virtual ui::TextInputType GetTextInputType() const = 0;

  
  
  
  virtual bool IsCandidatePopupOpen() const = 0;

  
  virtual bool IsMock() const = 0;

  
};

}  

#endif  
