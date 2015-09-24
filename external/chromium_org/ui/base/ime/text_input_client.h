// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_TEXT_INPUT_CLIENT_H_
#define UI_BASE_IME_TEXT_INPUT_CLIENT_H_

#include "base/basictypes.h"
#include "base/i18n/rtl.h"
#include "base/strings/string16.h"
#include "ui/base/ime/composition_text.h"
#include "ui/base/ime/text_input_mode.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/range/range.h"

namespace gfx {
class Rect;
}

namespace ui {

class UI_EXPORT TextInputClient {
 public:
  virtual ~TextInputClient();

  

  
  
  
  
  virtual void SetCompositionText(const ui::CompositionText& composition) = 0;

  
  virtual void ConfirmCompositionText() = 0;

  
  virtual void ClearCompositionText() = 0;

  
  
  
  virtual void InsertText(const base::string16& text) = 0;

  
  
  
  
  
  
  
  
  virtual void InsertChar(base::char16 ch, int flags) = 0;

  

  
  virtual gfx::NativeWindow GetAttachedWindow() const = 0;

  
  
  virtual ui::TextInputType GetTextInputType() const = 0;

  
  
  virtual ui::TextInputMode GetTextInputMode() const = 0;

  
  virtual bool CanComposeInline() const = 0;

  
  
  
  virtual gfx::Rect GetCaretBounds() const = 0;

  
  
  
  
  
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) const = 0;

  
  virtual bool HasCompositionText() const = 0;

  

  
  
  
  virtual bool GetTextRange(gfx::Range* range) const = 0;

  
  
  virtual bool GetCompositionTextRange(gfx::Range* range) const = 0;

  
  
  virtual bool GetSelectionRange(gfx::Range* range) const = 0;

  
  
  
  virtual bool SetSelectionRange(const gfx::Range& range) = 0;

  
  
  
  
  
  
  
  
  virtual bool DeleteRange(const gfx::Range& range) = 0;

  
  
  
  
  virtual bool GetTextFromRange(const gfx::Range& range,
                                base::string16* text) const = 0;

  

  
  
  virtual void OnInputMethodChanged() = 0;

  
  
  
  
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) = 0;

  
  
  
  
  
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) = 0;

  
  
  virtual void EnsureCaretInRect(const gfx::Rect& rect) = 0;

  
  virtual void OnCandidateWindowShown() = 0;
  
  virtual void OnCandidateWindowUpdated() = 0;
  
  virtual void OnCandidateWindowHidden() = 0;
};

}  

#endif  
