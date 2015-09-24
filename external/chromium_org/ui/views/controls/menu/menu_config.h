// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_CONFIG_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_CONFIG_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font.h"
#include "ui/views/views_export.h"

namespace ui {
class NativeTheme;
}

namespace views {

struct VIEWS_EXPORT MenuConfig {
  explicit MenuConfig(const ui::NativeTheme* theme);
  ~MenuConfig();

  static const MenuConfig& instance(const ui::NativeTheme* theme);

  
  gfx::Font font;

  
  SkColor text_color;

  
  SkColor arrow_color;

  
  int menu_vertical_border_size;
  int menu_horizontal_border_size;

  
  
  
  int submenu_horizontal_inset;

  
  int item_top_margin;

  
  int item_bottom_margin;

  
  int item_no_icon_top_margin;
  int item_no_icon_bottom_margin;

  
  int item_left_margin;

  
  int label_to_arrow_padding;

  
  int arrow_to_edge_padding;

  
  int icon_to_label_padding;

  
  int gutter_to_label;

  
  int check_width;
  int check_height;

  
  int radio_width;
  int radio_height;

  
  int arrow_height;
  int arrow_width;

  
  int gutter_width;

  
  int separator_height;

  
  int separator_upper_height;

  
  int separator_lower_height;

  
  int separator_spacing_height;

  
  
  bool render_gutter;

  
  bool show_mnemonics;

  
  int scroll_arrow_height;

  
  
  int label_to_minor_text_padding;

  
  int item_min_height;

  
  bool show_accelerators;

  
  bool always_use_icon_to_label_padding;

  
  bool align_arrow_and_shortcut;

  
  bool offset_context_menus;

  const ui::NativeTheme* native_theme;

  
  
  int show_delay;

  
  int corner_radius;

 private:
  
  void Init(const ui::NativeTheme* theme);

  
#if defined(USE_AURA)
  void InitAura(const ui::NativeTheme* theme);
#endif
};

}  

#endif  
