// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_IME_MOCK_INPUT_METHOD_H_
#define UI_VIEWS_IME_MOCK_INPUT_METHOD_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/ime/composition_text.h"
#include "ui/views/ime/input_method_base.h"
#include "ui/views/view.h"

namespace views {

class VIEWS_EXPORT MockInputMethod : public InputMethodBase {
 public:
  MockInputMethod();
  explicit MockInputMethod(internal::InputMethodDelegate* delegate);
  virtual ~MockInputMethod();

  
  virtual void Init(Widget* widget) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                        NativeEventResult* result) OVERRIDE;
  virtual void DispatchKeyEvent(const ui::KeyEvent& key) OVERRIDE;
  virtual void OnTextInputTypeChanged(View* view) OVERRIDE;
  virtual void OnCaretBoundsChanged(View* view) OVERRIDE;
  virtual void CancelComposition(View* view) OVERRIDE;
  virtual void OnInputLocaleChanged() OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;
  virtual bool IsCandidatePopupOpen() const OVERRIDE;
  virtual bool IsMock() const OVERRIDE;

  bool focus_changed() const { return focus_changed_; }
  bool untranslated_ime_message_called() const {
    return untranslated_ime_message_called_;
  }
  bool text_input_type_changed() const { return text_input_type_changed_; }
  bool caret_bounds_changed() const { return caret_bounds_changed_; }
  bool cancel_composition_called() const { return cancel_composition_called_; }
  bool input_locale_changed() const { return input_locale_changed_; }

  
  void Clear();

  void SetCompositionTextForNextKey(const ui::CompositionText& composition);
  void SetResultTextForNextKey(const string16& result);

  void SetInputLocale(const std::string& locale);
  void SetInputTextDirection(base::i18n::TextDirection direction);
  void SetActive(bool active);

 private:
  
  virtual void OnWillChangeFocus(View* focused_before, View* focused) OVERRIDE;

  
  void ClearStates();

  
  void ClearResult();

  
  
  ui::CompositionText composition_;
  bool composition_changed_;

  
  
  string16 result_text_;

  
  
  bool focus_changed_;
  bool untranslated_ime_message_called_;
  bool text_input_type_changed_;
  bool caret_bounds_changed_;
  bool cancel_composition_called_;
  bool input_locale_changed_;

  
  std::string locale_;
  base::i18n::TextDirection direction_;
  bool active_;

  DISALLOW_COPY_AND_ASSIGN(MockInputMethod);
};

}  

#endif  
