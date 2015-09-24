// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_WIN_IMM32_MANAGER_H
#define UI_BASE_IME_WIN_IMM32_MANAGER_H

#include <windows.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/i18n/rtl.h"
#include "base/strings/string16.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/rect.h"

namespace ui {

struct CompositionText;

//    instructions written in each subsection, and use the IMM32Manager class.

class UI_EXPORT IMM32Manager {
 public:
  IMM32Manager();
  virtual ~IMM32Manager();

  
  bool is_composing() const { return is_composing_; }

  
  
  
  
  
  
  bool SetInputLanguage();

  
  
  
  
  void CreateImeWindow(HWND window_handle);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //     All the window styles set in this function are over-written when
  
  
  LRESULT SetImeWindowStyle(HWND window_handle, UINT message,
                            WPARAM wparam, LPARAM lparam, BOOL* handled);

  
  
  
  
  void DestroyImeWindow(HWND window_handle);

  
  
  
  
  void UpdateImeWindow(HWND window_handle);

  
  
  
  
  
  void CleanupComposition(HWND window_handle);

  
  
  
  
  
  
  void ResetComposition(HWND window_handle);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetResult(HWND window_handle, LPARAM lparam, string16* result);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool GetComposition(HWND window_handle, LPARAM lparam,
                      CompositionText* composition);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void EnableIME(HWND window_handle);

  
  
  
  
  
  
  
  void DisableIME(HWND window_handle);

  
  
  
  
  void CancelIME(HWND window_handle);

  
  
  
  
  
  
  
  void UpdateCaretRect(HWND window_handle, const gfx::Rect& caret_rect);

  
  void SetUseCompositionWindow(bool use_composition_window);

  
  LANGID input_language_id() const { return input_language_id_; }

  
  std::string GetInputLanguageName() const;

  
  base::i18n::TextDirection GetTextDirection() const;

  
  virtual void SetTextInputMode(HWND window_handle, TextInputMode input_mode);

  

  
  static bool IsRTLKeyboardLayoutInstalled();

  
  
  
  
  static bool IsCtrlShiftPressed(base::i18n::TextDirection* direction);

  
  
  static void ConvertInputModeToImmFlags(TextInputMode input_mode,
                                         DWORD initial_conversion_mode,
                                         BOOL* open,
                                         DWORD* new_conversion_mode);


 protected:
  
  void GetCompositionInfo(HIMC imm_context, LPARAM lparam,
                          CompositionText* composition);

  
  void MoveImeWindow(HWND window_handle, HIMC imm_context);

  
  void CompleteComposition(HWND window_handle, HIMC imm_context);

  
  bool GetString(HIMC imm_context, WPARAM lparam, int type, string16* result);

 private:
  
  
  bool is_composing_;

  
  
  
  
  
  bool ime_status_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  LANGID input_language_id_;

  
  
  
  
  bool system_caret_;

  
  gfx::Rect caret_rect_;

  
  bool use_composition_window_;

  DISALLOW_COPY_AND_ASSIGN(IMM32Manager);
};

}  

#endif  
