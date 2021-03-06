// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_AURALINUX_H_
#define UI_BASE_IME_INPUT_METHOD_AURALINUX_H_

#include "base/memory/scoped_ptr.h"
#include "ui/base/ime/input_method_base.h"
#include "ui/base/ime/linux/linux_input_method_context.h"

namespace ui {

class InputMethodAuraLinux : public InputMethodBase,
                             public LinuxInputMethodContextDelegate {
 public:
  explicit InputMethodAuraLinux(internal::InputMethodDelegate* delegate);
  virtual ~InputMethodAuraLinux();

  
  
  
  static void Initialize();

  
  virtual void Init(bool focused) OVERRIDE;
  virtual bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                        NativeEventResult* result) OVERRIDE;
  virtual bool DispatchKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;
  virtual void OnCaretBoundsChanged(const TextInputClient* client) OVERRIDE;
  virtual void CancelComposition(const TextInputClient* client) OVERRIDE;
  virtual void OnInputLocaleChanged() OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;
  virtual bool IsCandidatePopupOpen() const OVERRIDE;

  
  virtual void OnCommit(const base::string16& text) OVERRIDE;
  virtual void OnPreeditChanged(const CompositionText& composition_text)
      OVERRIDE;
  virtual void OnPreeditEnd() OVERRIDE;
  virtual void OnPreeditStart() OVERRIDE;

 protected:
  
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) OVERRIDE;

  scoped_ptr<LinuxInputMethodContext> input_method_context_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodAuraLinux);
};

}  

#endif  
