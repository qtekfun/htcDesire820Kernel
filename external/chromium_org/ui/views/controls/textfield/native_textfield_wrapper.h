// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WRAPPER_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WRAPPER_H_

#include "base/i18n/rtl.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/selection_model.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/views_export.h"

namespace gfx {
class Insets;
class Point;
}  

namespace ui {
class KeyEvent;
class TextInputClient;
}  

namespace views {

class Textfield;
class View;

class VIEWS_EXPORT NativeTextfieldWrapper {
 public:
  
  
  virtual ~NativeTextfieldWrapper() {}

  
  virtual string16 GetText() const = 0;

  
  virtual void UpdateText() = 0;

  
  
  virtual void AppendText(const string16& text) = 0;

  
  virtual void InsertOrReplaceText(const string16& text) = 0;

  
  virtual base::i18n::TextDirection GetTextDirection() const = 0;

  
  virtual string16 GetSelectedText() const = 0;

  
  
  
  virtual void SelectAll(bool reversed) = 0;

  
  virtual void ClearSelection() = 0;

  
  virtual void UpdateBorder() = 0;

  
  virtual void UpdateTextColor() = 0;

  
  virtual void UpdateBackgroundColor() = 0;

  
  virtual void UpdateReadOnly() = 0;

  
  virtual void UpdateFont() = 0;

  
  virtual void UpdateIsObscured() = 0;

  
  virtual void UpdateEnabled() = 0;

  
  virtual gfx::Insets CalculateInsets() = 0;

  
  virtual void UpdateHorizontalMargins() = 0;

  
  virtual void UpdateVerticalMargins() = 0;

  
  
  virtual bool SetFocus() = 0;

  
  virtual View* GetView() = 0;

  
  virtual gfx::NativeView GetTestingHandle() const = 0;

  
  virtual bool IsIMEComposing() const = 0;

  
  virtual gfx::Range GetSelectedRange() const = 0;

  
  virtual void SelectRange(const gfx::Range& range) = 0;

  
  virtual gfx::SelectionModel GetSelectionModel() const = 0;

  
  virtual void SelectSelectionModel(const gfx::SelectionModel& sel) = 0;

  
  virtual size_t GetCursorPosition() const = 0;

  
  virtual bool GetCursorEnabled() const = 0;
  virtual void SetCursorEnabled(bool enabled) = 0;

  
  
  

  
  
  
  
  virtual bool HandleKeyPressed(const ui::KeyEvent& e) = 0;
  virtual bool HandleKeyReleased(const ui::KeyEvent& e) = 0;

  
  
  virtual void HandleFocus() = 0;
  virtual void HandleBlur() = 0;

  
  
  virtual ui::TextInputClient* GetTextInputClient() = 0;

  
  virtual void SetColor(SkColor value) = 0;
  virtual void ApplyColor(SkColor value, const gfx::Range& range) = 0;

  
  virtual void SetStyle(gfx::TextStyle style, bool value) = 0;
  virtual void ApplyStyle(gfx::TextStyle style,
                          bool value,
                          const gfx::Range& range) = 0;

  
  virtual void ClearEditHistory() = 0;

  
  virtual int GetFontHeight() = 0;

  
  
  virtual int GetTextfieldBaseline() const = 0;

  
  
  virtual int GetWidthNeededForText() const = 0;

  
  
  virtual void ExecuteTextCommand(int command_id) = 0;

  
  virtual bool HasTextBeingDragged() = 0;

  
  virtual gfx::Point GetContextMenuLocation() = 0;

  
  static NativeTextfieldWrapper* CreateWrapper(Textfield* field);
};

}  

#endif  
