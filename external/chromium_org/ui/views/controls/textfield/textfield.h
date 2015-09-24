// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/controls/textfield/native_textfield_wrapper.h"
#include "ui/views/view.h"

#if !defined(OS_LINUX)
#include "base/logging.h"
#endif

namespace gfx {
class Range;
class ImageSkia;
}

namespace ui {
class TextInputClient;
}  

namespace views {

class ImageView;
class Painter;
class TextfieldController;

class VIEWS_EXPORT Textfield : public View {
 public:
  
  static const char kViewClassName[];

  enum StyleFlags {
    STYLE_DEFAULT   = 0,
    STYLE_OBSCURED  = 1 << 0,
    STYLE_LOWERCASE = 1 << 1
  };

  
  static size_t GetCaretBlinkMs();

  Textfield();
  explicit Textfield(StyleFlags style);
  virtual ~Textfield();

  
  void SetController(TextfieldController* controller);
  TextfieldController* GetController() const;

  
  bool read_only() const { return read_only_; }
  void SetReadOnly(bool read_only);

  
  
  bool IsObscured() const;
  void SetObscured(bool obscured);

  
  
  
  
  const base::TimeDelta& obscured_reveal_duration() const {
    return obscured_reveal_duration_;
  }
  void set_obscured_reveal_duration(const base::TimeDelta& duration) {
    obscured_reveal_duration_ = duration;
  }

  
  ui::TextInputType GetTextInputType() const;
  void SetTextInputType(ui::TextInputType type);

  
  const string16& text() const { return text_; }

  
  
  
  
  void SetText(const string16& text);

  
  void AppendText(const string16& text);

  
  void InsertOrReplaceText(const string16& text);

  
  base::i18n::TextDirection GetTextDirection() const;

  
  string16 GetSelectedText() const;

  
  
  
  void SelectAll(bool reversed);

  
  void ClearSelection() const;

  
  bool HasSelection() const;

  
  StyleFlags style() const { return style_; }

  
  
  SkColor GetTextColor() const;
  void SetTextColor(SkColor color);
  void UseDefaultTextColor();

  
  
  SkColor GetBackgroundColor() const;
  void SetBackgroundColor(SkColor color);
  void UseDefaultBackgroundColor();

  
  bool GetCursorEnabled() const;
  void SetCursorEnabled(bool enabled);

  
  const gfx::FontList& font_list() const { return font_list_; }
  void SetFontList(const gfx::FontList& font_list);
  const gfx::Font& GetPrimaryFont() const;
  void SetFont(const gfx::Font& font);

  
  
  
  void SetHorizontalMargins(int left, int right);

  
  
  void SetVerticalMargins(int top, int bottom);

  
  void set_default_width_in_chars(int default_width) {
    default_width_in_chars_ = default_width;
  }

  
  bool draw_border() const { return draw_border_; }
  void RemoveBorder();

  
  void set_placeholder_text(const string16& text) {
    placeholder_text_ = text;
  }
  virtual base::string16 GetPlaceholderText() const;

  SkColor placeholder_text_color() const { return placeholder_text_color_; }
  void set_placeholder_text_color(SkColor color) {
    placeholder_text_color_ = color;
  }

  
  
  bool GetHorizontalMargins(int* left, int* right);

  
  
  bool GetVerticalMargins(int* top, int* bottom);

  
  
  void UpdateAllProperties();

  
  
  
  
  void SyncText();

  
  bool IsIMEComposing() const;

  
  
  
  gfx::Range GetSelectedRange() const;

  
  
  void SelectRange(const gfx::Range& range);

  
  
  
  gfx::SelectionModel GetSelectionModel() const;

  
  
  void SelectSelectionModel(const gfx::SelectionModel& sel);

  
  
  size_t GetCursorPosition() const;

  
  
  
  void SetColor(SkColor value);
  void ApplyColor(SkColor value, const gfx::Range& range);

  
  
  
  
  void SetStyle(gfx::TextStyle style, bool value);
  void ApplyStyle(gfx::TextStyle style, bool value, const gfx::Range& range);

  
  void ClearEditHistory();

  
  void SetAccessibleName(const string16& name);

  
  void ExecuteCommand(int command_id);

  void SetFocusPainter(scoped_ptr<Painter> focus_painter);

  
  gfx::NativeView GetTestingHandle() const {
    return native_wrapper_ ? native_wrapper_->GetTestingHandle() : NULL;
  }
  NativeTextfieldWrapper* GetNativeWrapperForTesting() const {
    return native_wrapper_;
  }

  
  bool HasTextBeingDragged();

  
  virtual void Layout() OVERRIDE;
  virtual int GetBaseline() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void AboutToRequestFocusFromTabTraversal(bool reverse) OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) OVERRIDE;
  virtual void OnEnabledChanged() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& e) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() OVERRIDE;
  virtual gfx::Point GetKeyboardContextMenuLocation() OVERRIDE;

 protected:
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  NativeTextfieldWrapper* native_wrapper_;

 private:
  
  gfx::Insets GetTextInsets() const;

  
  
  
  void AccessibilitySetValue(const string16& new_value);

  
  TextfieldController* controller_;

  
  StyleFlags style_;

  
  gfx::FontList font_list_;

  
  string16 text_;

  
  bool read_only_;

  
  
  int default_width_in_chars_;

  
  bool draw_border_;

  
  SkColor text_color_;

  
  bool use_default_text_color_;

  
  SkColor background_color_;

  
  bool use_default_background_color_;

  
  gfx::Insets margins_;

  
  bool horizontal_margins_were_set_;
  bool vertical_margins_were_set_;

  
  string16 placeholder_text_;

  
  SkColor placeholder_text_color_;

  
  string16 accessible_name_;

  
  ui::TextInputType text_input_type_;

  
  base::TimeDelta obscured_reveal_duration_;

  
  base::WeakPtrFactory<Textfield> weak_ptr_factory_;

  scoped_ptr<Painter> focus_painter_;

  DISALLOW_COPY_AND_ASSIGN(Textfield);
};

}  

#endif  
