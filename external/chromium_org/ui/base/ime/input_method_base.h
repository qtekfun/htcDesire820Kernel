// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_BASE_H_
#define UI_BASE_IME_INPUT_METHOD_BASE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "ui/base/ime/input_method.h"
#include "ui/base/ui_export.h"

namespace gfx {
class Rect;
}  

namespace ui {

class InputMethodObserver;
class KeyEvent;
class TextInputClient;

class UI_EXPORT InputMethodBase
   : NON_EXPORTED_BASE(public InputMethod),
     public base::SupportsWeakPtr<InputMethodBase> {
 public:
  InputMethodBase();
  virtual ~InputMethodBase();

  
  virtual void SetDelegate(internal::InputMethodDelegate* delegate) OVERRIDE;
  virtual void Init(bool focused) OVERRIDE;
  
  
  
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void SetFocusedTextInputClient(TextInputClient* client) OVERRIDE;
  virtual void DetachTextInputClient(TextInputClient* client) OVERRIDE;
  virtual TextInputClient* GetTextInputClient() const OVERRIDE;

  
  
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;

  virtual TextInputType GetTextInputType() const OVERRIDE;
  virtual TextInputMode GetTextInputMode() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;

  virtual void AddObserver(InputMethodObserver* observer) OVERRIDE;
  virtual void RemoveObserver(InputMethodObserver* observer) OVERRIDE;

 protected:
  virtual void OnWillChangeFocusedClient(TextInputClient* focused_before,
                                         TextInputClient* focused) {}
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) {}

  
  bool IsTextInputClientFocused(const TextInputClient* client);

  
  
  
  bool IsTextInputTypeNone() const;

  
  
  
  void OnInputMethodChanged() const;

  
  
  bool DispatchKeyEventPostIME(const ui::KeyEvent& event) const;

  
  void NotifyTextInputStateChanged(const TextInputClient* client);

  
  
  
  
  void OnCandidateWindowShown();
  void OnCandidateWindowUpdated();
  void OnCandidateWindowHidden();

  bool system_toplevel_window_focused() const {
    return system_toplevel_window_focused_;
  }

 private:
  void SetFocusedTextInputClientInternal(TextInputClient* client);

  
  
  void CandidateWindowShownCallback();
  void CandidateWindowUpdatedCallback();
  void CandidateWindowHiddenCallback();

  internal::InputMethodDelegate* delegate_;
  TextInputClient* text_input_client_;

  ObserverList<InputMethodObserver> observer_list_;

  bool system_toplevel_window_focused_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodBase);
};

}  

#endif  
