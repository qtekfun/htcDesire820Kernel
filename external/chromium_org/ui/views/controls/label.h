// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_LABEL_H_
#define UI_VIEWS_CONTROLS_LABEL_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/view.h"

namespace views {

class VIEWS_EXPORT Label : public View {
 public:
  
  static const char kViewClassName[];

  
  
  
  
  
  
  
  
  
  
  enum DirectionalityMode {
    USE_UI_DIRECTIONALITY = 0,
    AUTO_DETECT_DIRECTIONALITY
  };

  enum ElideBehavior {
    NO_ELIDE,         
    ELIDE_IN_MIDDLE,  
    ELIDE_AT_END,     
    ELIDE_AS_EMAIL,   
  };

  Label();
  explicit Label(const string16& text);
  Label(const string16& text, const gfx::FontList& font_list);
  Label(const string16& text, const gfx::Font& font);  
  virtual ~Label();

  
  const gfx::FontList& font_list() const { return font_list_; }
  virtual void SetFontList(const gfx::FontList& font_list);
  
  const gfx::Font& font() const;  
  virtual void SetFont(const gfx::Font& font);  

  
  const string16& text() const { return text_; }
  void SetText(const string16& text);

  
  
  
  
  void SetAutoColorReadabilityEnabled(bool enabled);

  
  
  virtual void SetEnabledColor(SkColor color);
  void SetDisabledColor(SkColor color);

  SkColor enabled_color() const { return actual_enabled_color_; }

  
  
  void SetBackgroundColor(SkColor color);
  SkColor background_color() const { return background_color_; }

  
  void SetShadowColors(SkColor enabled_color, SkColor disabled_color);

  
  void SetShadowOffset(int x, int y);

  
  void ClearEmbellishing();

  
  
  
  
  
  
  
  void SetHorizontalAlignment(gfx::HorizontalAlignment alignment);

  gfx::HorizontalAlignment horizontal_alignment() const {
    return horizontal_alignment_;
  }

  
  
  
  
  void set_directionality_mode(DirectionalityMode mode) {
    directionality_mode_ = mode;
  }

  DirectionalityMode directionality_mode() const {
    return directionality_mode_;
  }

  
  
  
  int line_height() const { return line_height_; }
  void SetLineHeight(int height);

  
  bool is_multi_line() const { return is_multi_line_; }
  void SetMultiLine(bool multi_line);

  
  
  void SetAllowCharacterBreak(bool allow_character_break);

  
  
  
  void SetElideBehavior(ElideBehavior elide_behavior);

  
  
  
  
  void SetTooltipText(const string16& tooltip_text);

  
  
  
  
  
  
  void SizeToFit(int max_width);

  
  
  
  void set_collapse_when_hidden(bool value) { collapse_when_hidden_ = value; }
  bool collapse_when_hidden() const { return collapse_when_hidden_; }

  void SetHasFocusBorder(bool has_focus_border);

  
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual int GetBaseline() const OVERRIDE;
  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  
  
  
  virtual int GetHeightForWidth(int w) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual View* GetTooltipHandlerForPoint(const gfx::Point& point) OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  
  
  
  
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;

 protected:
  
  
  virtual void PaintText(gfx::Canvas* canvas,
                         const string16& text,
                         const gfx::Rect& text_bounds,
                         int flags);

  virtual gfx::Size GetTextSize() const;

  SkColor disabled_color() const { return actual_disabled_color_; }

  
  
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

 private:
  
  
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawSingleLineString);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawMultiLineString);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawSingleLineStringInRTL);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawMultiLineStringInRTL);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, AutoDetectDirectionality);

  
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DisableSubpixelRendering);

  void Init(const string16& text, const gfx::FontList& font_list);

  void RecalculateColors();

  
  gfx::Rect GetTextBounds() const;

  int ComputeDrawStringFlags() const;

  gfx::Rect GetAvailableRect() const;

  
  void CalculateDrawStringParams(string16* paint_text,
                                 gfx::Rect* text_bounds,
                                 int* flags) const;

  
  void UpdateColorsFromTheme(const ui::NativeTheme* theme);

  
  
  void ResetCachedSize();

  bool ShouldShowDefaultTooltip() const;

  string16 text_;
  gfx::FontList font_list_;
  SkColor requested_enabled_color_;
  SkColor actual_enabled_color_;
  SkColor requested_disabled_color_;
  SkColor actual_disabled_color_;
  SkColor background_color_;

  
  bool enabled_color_set_;
  bool disabled_color_set_;
  bool background_color_set_;

  bool auto_color_readability_;
  mutable gfx::Size text_size_;
  mutable bool text_size_valid_;
  int line_height_;
  bool is_multi_line_;
  bool allow_character_break_;
  ElideBehavior elide_behavior_;
  gfx::HorizontalAlignment horizontal_alignment_;
  string16 tooltip_text_;
  
  bool collapse_when_hidden_;
  
  
  
  DirectionalityMode directionality_mode_;
  
  
  
  bool has_focus_border_;

  
  SkColor enabled_shadow_color_;
  SkColor disabled_shadow_color_;

  
  gfx::Point shadow_offset_;

  
  bool has_shadow_;

  
  std::vector<gfx::Size> cached_heights_;
  int cached_heights_cursor_;

  DISALLOW_COPY_AND_ASSIGN(Label);
};

}  

#endif  
