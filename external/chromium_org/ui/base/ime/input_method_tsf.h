// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_TSF_H_
#define UI_BASE_IME_INPUT_METHOD_TSF_H_

#include <windows.h>

#include <string>

#include "base/memory/scoped_ptr.h"
#include "ui/base/ime/input_method_win.h"

namespace ui {

class TSFEventRouter;

class UI_EXPORT InputMethodTSF : public InputMethodWin {
 public:
  InputMethodTSF(internal::InputMethodDelegate* delegate,
                 HWND toplevel_window_handle);
  virtual ~InputMethodTSF();

  
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                        NativeEventResult* result) OVERRIDE;
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;
  virtual void OnCaretBoundsChanged(const TextInputClient* client) OVERRIDE;
  virtual void CancelComposition(const TextInputClient* client) OVERRIDE;
  virtual void DetachTextInputClient(TextInputClient* client) OVERRIDE;
  virtual bool IsCandidatePopupOpen() const OVERRIDE;

  
  virtual void OnWillChangeFocusedClient(TextInputClient* focused_before,
                                         TextInputClient* focused) OVERRIDE;
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) OVERRIDE;

 private:
  class TSFEventObserver;

  
  void ConfirmCompositionText();

  
  scoped_ptr<TSFEventObserver> tsf_event_observer_;
  scoped_ptr<TSFEventRouter> tsf_event_router_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodTSF);
};

}  

#endif  
