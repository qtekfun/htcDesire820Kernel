// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_LABEL_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_LABEL_BUTTON_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/label.h"
#include "ui/views/native_theme_delegate.h"

namespace views {

class Painter;

class VIEWS_EXPORT LabelButton : public CustomButton,
                                 public NativeThemeDelegate {
 public:
  
  static const int kHoverAnimationDurationMs;

  static const char kViewClassName[];

  LabelButton(ButtonListener* listener, const string16& text);
  virtual ~LabelButton();

  
  
  virtual const gfx::ImageSkia& GetImage(ButtonState for_state);
  void SetImage(ButtonState for_state, const gfx::ImageSkia& image);

  
  const string16& GetText() const;
  void SetText(const string16& text);

  
  void SetTextColor(ButtonState for_state, SkColor color);

  
  bool GetTextMultiLine() const;
  void SetTextMultiLine(bool text_multi_line);

  
  const gfx::Font& GetFont() const;
  void SetFont(const gfx::Font& font);

  
  void SetElideBehavior(Label::ElideBehavior elide_behavior);

  
  
  gfx::HorizontalAlignment GetHorizontalAlignment() const;
  void SetHorizontalAlignment(gfx::HorizontalAlignment alignment);

  
  void set_min_size(const gfx::Size& min_size) { min_size_ = min_size; }
  void set_max_size(const gfx::Size& max_size) { max_size_ = max_size; }

  
  bool is_default() const { return is_default_; }
  void SetIsDefault(bool is_default);

  
  ButtonStyle style() const { return style_; }
  void SetStyle(ButtonStyle style);

  void SetFocusPainter(scoped_ptr<Painter> focus_painter);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

 protected:
  ImageView* image() const { return image_; }
  Label* label() const { return label_; }

  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  
  virtual void GetExtraParams(ui::NativeTheme::ExtraParams* params) const;

  
  virtual void ResetColorsFromNativeTheme();

  
  void UpdateImage();

  
  virtual gfx::Rect GetThemePaintRect() const OVERRIDE;

 private:
  FRIEND_TEST_ALL_PREFIXES(LabelButtonTest, Init);
  FRIEND_TEST_ALL_PREFIXES(LabelButtonTest, Label);
  FRIEND_TEST_ALL_PREFIXES(LabelButtonTest, Image);
  FRIEND_TEST_ALL_PREFIXES(LabelButtonTest, LabelAndImage);
  FRIEND_TEST_ALL_PREFIXES(LabelButtonTest, Font);

  
  virtual void StateChanged() OVERRIDE;

  
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

  
  virtual ui::NativeTheme::Part GetThemePart() const OVERRIDE;
  virtual ui::NativeTheme::State GetThemeState(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;
  virtual const gfx::Animation* GetThemeAnimation() const OVERRIDE;
  virtual ui::NativeTheme::State GetBackgroundThemeState(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;
  virtual ui::NativeTheme::State GetForegroundThemeState(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;

  
  ImageView* image_;
  Label* label_;

  
  gfx::ImageSkia button_state_images_[STATE_COUNT];
  SkColor button_state_colors_[STATE_COUNT];

  
  bool explicitly_set_colors_[STATE_COUNT];

  
  gfx::Size min_size_;
  
  gfx::Size max_size_;

  
  
  
  bool is_default_;

  
  ButtonStyle style_;

  scoped_ptr<Painter> focus_painter_;

  DISALLOW_COPY_AND_ASSIGN(LabelButton);
};

}  

#endif  
