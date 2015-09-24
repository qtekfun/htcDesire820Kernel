// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_WIN_H_
#define UI_BASE_IME_INPUT_METHOD_WIN_H_

#include <windows.h>

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/ime/input_method_base.h"
#include "ui/base/ime/win/imm32_manager.h"

namespace ui {

class UI_EXPORT InputMethodWin : public InputMethodBase {
 public:
  InputMethodWin(internal::InputMethodDelegate* delegate,
                 HWND toplevel_window_handle);

  
  virtual void Init(bool focused) OVERRIDE;
  virtual bool DispatchKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnInputLocaleChanged() OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;

 protected:
  
  
  
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) OVERRIDE;

  
  
  
  LRESULT OnImeRequest(UINT message,
                       WPARAM wparam,
                       LPARAM lparam,
                       BOOL* handled);
  
  LRESULT OnChar(HWND window_handle,
                 UINT message,
                 WPARAM wparam,
                 LPARAM lparam,
                 BOOL* handled);
  
  
  
  LRESULT OnDeadChar(UINT message, WPARAM wparam, LPARAM lparam, BOOL* handled);

  LRESULT OnDocumentFeed(RECONVERTSTRING* reconv);
  LRESULT OnReconvertString(RECONVERTSTRING* reconv);
  LRESULT OnQueryCharPosition(IMECHARPOSITION* char_positon);

  
  
  HWND GetAttachedWindowHandle(const TextInputClient* text_input_client) const;

  
  
  bool IsWindowFocused(const TextInputClient* client) const;

  
  bool active_;

  
  
  ui::IMM32Manager imm32_manager_;

 private:
  bool DispatchFabricatedKeyEvent(const ui::KeyEvent& event);

  
  
  const HWND toplevel_window_handle_;

  
  std::string locale_;

  
  base::i18n::TextDirection direction_;

  
  
  
  base::i18n::TextDirection pending_requested_direction_;

  
  
  
  
  bool accept_carriage_return_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodWin);
};

}  

#endif  
