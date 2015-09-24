// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_H_
#define UI_BASE_IME_INPUT_METHOD_H_

#include <string>

#include "base/basictypes.h"
#include "base/event_types.h"
#include "base/i18n/rtl.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"

namespace ui {

namespace internal {
class InputMethodDelegate;
}  

class InputMethodObserver;
class KeyEvent;
class TextInputClient;

class InputMethod {
 public:
  
#if defined(OS_WIN)
  typedef LRESULT NativeEventResult;
#else
  typedef int32 NativeEventResult;
#endif

  virtual ~InputMethod() {}

  
  
  virtual void SetDelegate(internal::InputMethodDelegate* delegate) = 0;

  
  
  virtual void Init(bool focused) = 0;

  
  virtual void OnFocus() = 0;

  
  virtual void OnBlur() = 0;

  
  
  
  
  virtual bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                        NativeEventResult* result) = 0;

  
  
  
  
  
  virtual void SetFocusedTextInputClient(TextInputClient* client) = 0;

  
  
  
  virtual void DetachTextInputClient(TextInputClient* client) = 0;

  
  virtual TextInputClient* GetTextInputClient() const = 0;

  
  
  
  
  
  virtual bool DispatchKeyEvent(const ui::KeyEvent& event) = 0;

  
  
  
  
  
  virtual void OnTextInputTypeChanged(const TextInputClient* client) = 0;

  
  
  virtual void OnCaretBoundsChanged(const TextInputClient* client) = 0;

  
  
  
  virtual void CancelComposition(const TextInputClient* client) = 0;

  
  
  
  
  
  virtual void OnInputLocaleChanged() = 0;

  
  
  virtual std::string GetInputLocale() = 0;

  
  
  virtual base::i18n::TextDirection GetInputTextDirection() = 0;

  
  
  
  
  
  
  
  virtual bool IsActive() = 0;

  
  
  
  
  
  
  virtual TextInputType GetTextInputType() const = 0;

  
  
  virtual TextInputMode GetTextInputMode() const = 0;

  
  virtual bool CanComposeInline() const = 0;

  
  
  
  virtual bool IsCandidatePopupOpen() const = 0;

  
  virtual void AddObserver(InputMethodObserver* observer) = 0;
  virtual void RemoveObserver(InputMethodObserver* observer) = 0;
};

}  

#endif  
