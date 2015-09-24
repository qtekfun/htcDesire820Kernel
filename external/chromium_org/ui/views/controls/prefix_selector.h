// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_PREFIX_SELECTOR_H_
#define UI_VIEWS_CONTROLS_PREFIX_SELECTOR_H_

#include "base/strings/string16.h"
#include "base/time/time.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/views/views_export.h"

namespace views {

class PrefixDelegate;

class VIEWS_EXPORT PrefixSelector : public ui::TextInputClient {
 public:
  explicit PrefixSelector(PrefixDelegate* delegate);
  virtual ~PrefixSelector();

  
  void OnViewBlur();

  
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual gfx::NativeWindow GetAttachedWindow() const OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual ui::TextInputMode GetTextInputMode() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual gfx::Rect GetCaretBounds() const OVERRIDE;
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) const OVERRIDE;
  virtual bool HasCompositionText() const OVERRIDE;
  virtual bool GetTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetCompositionTextRange(gfx::Range* range) const OVERRIDE;
  virtual bool GetSelectionRange(gfx::Range* range) const OVERRIDE;
  virtual bool SetSelectionRange(const gfx::Range& range) OVERRIDE;
  virtual bool DeleteRange(const gfx::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(const gfx::Range& range,
                                string16* text) const OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) OVERRIDE;
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE;
  virtual void OnCandidateWindowShown() OVERRIDE;
  virtual void OnCandidateWindowUpdated() OVERRIDE;
  virtual void OnCandidateWindowHidden() OVERRIDE;

 private:
  
  void OnTextInput(const string16& text);

  
  bool TextAtRowMatchesText(int row, const string16& lower_text);

  
  void ClearText();

  PrefixDelegate* prefix_delegate_;

  
  base::TimeTicks time_of_last_key_;

  string16 current_text_;

  DISALLOW_COPY_AND_ASSIGN(PrefixSelector);
};

}  

#endif  
