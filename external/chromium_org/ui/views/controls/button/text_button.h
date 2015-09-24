// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_TEXT_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_TEXT_BUTTON_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/border.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/native_theme_delegate.h"
#include "ui/views/painter.h"

namespace views {

class VIEWS_EXPORT TextButtonBorder : public Border {
 public:
  TextButtonBorder();
  virtual ~TextButtonBorder();

  void SetInsets(const gfx::Insets& insets);

  
  virtual void Paint(const View& view, gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual gfx::Size GetMinimumSize() const OVERRIDE;

 private:
  
  virtual TextButtonBorder* AsTextButtonBorder() OVERRIDE;
  virtual const TextButtonBorder* AsTextButtonBorder() const OVERRIDE;

  gfx::Insets insets_;

  DISALLOW_COPY_AND_ASSIGN(TextButtonBorder);
};


class VIEWS_EXPORT TextButtonDefaultBorder : public TextButtonBorder {
 public:
  TextButtonDefaultBorder();
  virtual ~TextButtonDefaultBorder();

  
  void set_normal_painter(Painter* painter) { normal_painter_.reset(painter); }
  void set_hot_painter(Painter* painter) { hot_painter_.reset(painter); }
  void set_pushed_painter(Painter* painter) { pushed_painter_.reset(painter); }

 private:
  
  virtual void Paint(const View& view, gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Size GetMinimumSize() const OVERRIDE;

  scoped_ptr<Painter> normal_painter_;
  scoped_ptr<Painter> hot_painter_;
  scoped_ptr<Painter> pushed_painter_;

  int vertical_padding_;

  DISALLOW_COPY_AND_ASSIGN(TextButtonDefaultBorder);
};


class VIEWS_EXPORT TextButtonNativeThemeBorder : public TextButtonBorder {
 public:
  explicit TextButtonNativeThemeBorder(NativeThemeDelegate* delegate);
  virtual ~TextButtonNativeThemeBorder();

  
  virtual void Paint(const View& view, gfx::Canvas* canvas) OVERRIDE;
  
  

 private:
  
  NativeThemeDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TextButtonNativeThemeBorder);
};


class VIEWS_EXPORT TextButtonBase : public CustomButton,
                                    public NativeThemeDelegate {
 public:
  
  static const char kViewClassName[];

  virtual ~TextButtonBase();

  
  
  
  virtual void SetText(const string16& text);
  const string16& text() const { return text_; }

  enum TextAlignment {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
  };

  void set_alignment(TextAlignment alignment) { alignment_ = alignment; }

  const gfx::Animation* GetAnimation() const;

  void SetIsDefault(bool is_default);
  bool is_default() const { return is_default_; }

  
  
  void SetMultiLine(bool multi_line);

  
  bool multi_line() const { return multi_line_; }

  
  
  
  void ClearMaxTextSize();

  void set_min_width(int min_width) { min_width_ = min_width; }
  void set_min_height(int min_height) { min_height_ = min_height; }
  void set_max_width(int max_width) { max_width_ = max_width; }
  void SetFont(const gfx::Font& font);
  
  gfx::Font font() const { return font_; }

  void SetEnabledColor(SkColor color);
  void SetDisabledColor(SkColor color);
  void SetHighlightColor(SkColor color);
  void SetHoverColor(SkColor color);

  
  void SetTextShadowColors(SkColor active_color, SkColor inactive_color);

  
  void SetTextShadowOffset(int x, int y);

  
  void ClearEmbellishing();

  
  
  bool show_multiple_icon_states() const { return show_multiple_icon_states_; }
  void SetShowMultipleIconStates(bool show_multiple_icon_states);

  void SetFocusPainter(scoped_ptr<Painter> focus_painter);
  Painter* focus_painter() { return focus_painter_.get(); }

  
  
  enum PaintButtonMode { PB_NORMAL, PB_FOR_DRAG };
  virtual void PaintButton(gfx::Canvas* canvas, PaintButtonMode mode);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual int GetHeightForWidth(int w) OVERRIDE;
  virtual void OnEnabledChanged() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

 protected:
  TextButtonBase(ButtonListener* listener, const string16& text);

  
  
  virtual void UpdateColor();

  
  
  void UpdateTextSize();

  
  void CalculateTextSize(gfx::Size* text_size, int max_width);

  void set_color_enabled(SkColor color) { color_enabled_ = color; }
  void set_color_disabled(SkColor color) { color_disabled_ = color; }
  void set_color_hover(SkColor color) { color_hover_ = color; }

  bool use_enabled_color_from_theme() const {
    return use_enabled_color_from_theme_;
  }

  bool use_disabled_color_from_theme() const {
    return use_disabled_color_from_theme_;
  }

  bool use_hover_color_from_theme() const {
    return use_hover_color_from_theme_;
  }

  
  virtual gfx::Rect GetThemePaintRect() const OVERRIDE;
  virtual ui::NativeTheme::State GetThemeState(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;
  virtual const gfx::Animation* GetThemeAnimation() const OVERRIDE;
  virtual ui::NativeTheme::State GetBackgroundThemeState(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;
  virtual ui::NativeTheme::State GetForegroundThemeState(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;

  
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  virtual void GetExtraParams(ui::NativeTheme::ExtraParams* params) const;

  virtual gfx::Rect GetTextBounds() const;

  int ComputeCanvasStringFlags() const;

  
  
  gfx::Rect GetContentBounds(int extra_width) const;

  
  string16 text_;

  
  gfx::Size text_size_;

  
  
  gfx::Size max_text_size_;

  
  TextAlignment alignment_;

  
  gfx::Font font_;

  
  bool has_text_shadow_;
  
  SkColor active_text_shadow_color_;
  SkColor inactive_text_shadow_color_;
  
  gfx::Point text_shadow_offset_;

  
  int min_width_;
  int min_height_;

  
  
  int max_width_;

  
  bool show_multiple_icon_states_;

  
  
  bool is_default_;

  
  bool multi_line_;

 private:
  
  SkColor color_;

  
  SkColor color_enabled_;
  SkColor color_disabled_;
  SkColor color_highlight_;
  SkColor color_hover_;

  
  bool use_enabled_color_from_theme_;
  bool use_disabled_color_from_theme_;
  bool use_highlight_color_from_theme_;
  bool use_hover_color_from_theme_;

  scoped_ptr<Painter> focus_painter_;

  DISALLOW_COPY_AND_ASSIGN(TextButtonBase);
};


class VIEWS_EXPORT TextButton : public TextButtonBase {
 public:
  
  static const char kViewClassName[];

  TextButton(ButtonListener* listener, const string16& text);
  virtual ~TextButton();

  void set_icon_text_spacing(int icon_text_spacing) {
    icon_text_spacing_ = icon_text_spacing;
  }

  
  virtual void SetIcon(const gfx::ImageSkia& icon);
  virtual void SetHoverIcon(const gfx::ImageSkia& icon);
  virtual void SetPushedIcon(const gfx::ImageSkia& icon);

  bool HasIcon() const { return !icon_.isNull(); }

  
  enum IconPlacement {
    ICON_ON_LEFT,
    ICON_ON_RIGHT,
    ICON_CENTERED  
  };

  IconPlacement icon_placement() { return icon_placement_; }
  void set_icon_placement(IconPlacement icon_placement) {
    
    DCHECK((icon_placement != ICON_CENTERED) || text_.empty());
    icon_placement_ = icon_placement;
  }

  void set_ignore_minimum_size(bool ignore_minimum_size);

  
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual const char* GetClassName() const OVERRIDE;

  
  virtual void PaintButton(gfx::Canvas* canvas, PaintButtonMode mode) OVERRIDE;

 protected:
  gfx::ImageSkia icon() const { return icon_; }

  virtual const gfx::ImageSkia& GetImageToPaint() const;

  
  virtual ui::NativeTheme::Part GetThemePart() const OVERRIDE;

  
  virtual void GetExtraParams(
      ui::NativeTheme::ExtraParams* params) const OVERRIDE;
  virtual gfx::Rect GetTextBounds() const OVERRIDE;

 private:
  
  IconPlacement icon_placement_;

  
  gfx::ImageSkia icon_;

  
  gfx::ImageSkia icon_hover_;
  bool has_hover_icon_;

  
  gfx::ImageSkia icon_pushed_;
  bool has_pushed_icon_;

  
  int icon_text_spacing_;

  
  
  bool ignore_minimum_size_;

  DISALLOW_COPY_AND_ASSIGN(TextButton);
};

}  

#endif  
