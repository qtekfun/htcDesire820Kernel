// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_NATIVE_THEME_NATIVE_THEME_H_
#define UI_NATIVE_THEME_NATIVE_THEME_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/native_theme/native_theme_export.h"

class SkCanvas;

namespace gfx {
class Rect;
class Size;
}

namespace ui {

class NATIVE_THEME_EXPORT NativeTheme {
 public:
  
  enum Part {
    kCheckbox,
    kInnerSpinButton,
    kMenuList,
    kMenuCheck,
    kMenuCheckBackground,
    kMenuPopupArrow,
    kMenuPopupBackground,
    kMenuPopupGutter,
    kMenuPopupSeparator,
    kMenuItemBackground,
    kProgressBar,
    kPushButton,
    kRadio,

    
    
    kScrollbarDownArrow,
    kScrollbarLeftArrow,
    kScrollbarRightArrow,
    kScrollbarUpArrow,

    kScrollbarHorizontalThumb,
    kScrollbarVerticalThumb,
    kScrollbarHorizontalTrack,
    kScrollbarVerticalTrack,
    kScrollbarHorizontalGripper,
    kScrollbarVerticalGripper,
    kSliderTrack,
    kSliderThumb,
    kTabPanelBackground,
    kTextField,
    kTrackbarThumb,
    kTrackbarTrack,
    kWindowResizeGripper,
    kMaxPart,
  };

  
  enum State {
    kDisabled,
    kHovered,
    kNormal,
    kPressed,
    kMaxState,
  };

  
  

  struct ButtonExtraParams {
    bool checked;
    bool indeterminate;  
    bool is_default;  
    bool is_focused;
    bool has_border;
    int classic_state;  
    SkColor background_color;
  };

  struct InnerSpinButtonExtraParams {
    bool spin_up;
    bool read_only;
    int classic_state;  
  };

  struct MenuArrowExtraParams {
    bool pointing_right;
    
    
    bool is_selected;
  };

  struct MenuCheckExtraParams {
    bool is_radio;
    
    
    bool is_selected;
  };

  struct MenuItemExtraParams {
    bool is_selected;
  };

  struct MenuListExtraParams {
    bool has_border;
    bool has_border_radius;
    int arrow_x;
    int arrow_y;
    SkColor background_color;
    int classic_state;  
  };

  struct MenuSeparatorExtraParams {
    bool has_gutter;
  };

  struct MenuBackgroundExtraParams {
    int corner_radius;
  };

  struct ProgressBarExtraParams {
    double animated_seconds;
    bool determinate;
    int value_rect_x;
    int value_rect_y;
    int value_rect_width;
    int value_rect_height;
  };

  struct ScrollbarArrowExtraParams {
    bool is_hovering;
  };

  struct ScrollbarTrackExtraParams {
    bool is_upper;
    int track_x;
    int track_y;
    int track_width;
    int track_height;
    int classic_state;  
  };

  struct ScrollbarThumbExtraParams {
    bool is_hovering;
  };

  struct SliderExtraParams {
    bool vertical;
    bool in_drag;
  };

  struct TextFieldExtraParams {
    bool is_text_area;
    bool is_listbox;
    SkColor background_color;
    bool is_read_only;
    bool is_focused;
    bool fill_content_area;
    bool draw_edges;
    int classic_state;  
  };

  struct TrackbarExtraParams {
    bool vertical;
    int classic_state;  
  };

  union ExtraParams {
    ButtonExtraParams button;
    InnerSpinButtonExtraParams inner_spin;
    MenuArrowExtraParams menu_arrow;
    MenuCheckExtraParams menu_check;
    MenuItemExtraParams menu_item;
    MenuListExtraParams menu_list;
    MenuSeparatorExtraParams menu_separator;
    MenuBackgroundExtraParams menu_background;
    ProgressBarExtraParams progress_bar;
    ScrollbarArrowExtraParams scrollbar_arrow;
    ScrollbarTrackExtraParams scrollbar_track;
    ScrollbarThumbExtraParams scrollbar_thumb;
    SliderExtraParams slider;
    TextFieldExtraParams text_field;
    TrackbarExtraParams trackbar;
  };

  
  virtual gfx::Size GetPartSize(Part part,
                                State state,
                                const ExtraParams& extra) const = 0;

  
  virtual void Paint(SkCanvas* canvas,
                     Part part,
                     State state,
                     const gfx::Rect& rect,
                     const ExtraParams& extra) const = 0;

  
  void SetScrollbarColors(unsigned inactive_color,
                          unsigned active_color,
                          unsigned track_color);

  
  enum ColorId {
    
    kColorId_WindowBackground,
    
    kColorId_DialogBackground,
    
    kColorId_FocusedBorderColor,
    kColorId_UnfocusedBorderColor,
    
    kColorId_ButtonBackgroundColor,
    kColorId_ButtonEnabledColor,
    kColorId_ButtonDisabledColor,
    kColorId_ButtonHighlightColor,
    kColorId_ButtonHoverColor,
    
    kColorId_EnabledMenuItemForegroundColor,
    kColorId_DisabledMenuItemForegroundColor,
    kColorId_SelectedMenuItemForegroundColor,
    kColorId_FocusedMenuItemBackgroundColor,
    kColorId_HoverMenuItemBackgroundColor,
    kColorId_MenuSeparatorColor,
    kColorId_MenuBackgroundColor,
    kColorId_MenuBorderColor,
    
    kColorId_EnabledMenuButtonBorderColor,
    kColorId_FocusedMenuButtonBorderColor,
    kColorId_HoverMenuButtonBorderColor,
    
    kColorId_LabelEnabledColor,
    kColorId_LabelDisabledColor,
    kColorId_LabelBackgroundColor,
    
    kColorId_TextfieldDefaultColor,
    kColorId_TextfieldDefaultBackground,
    kColorId_TextfieldReadOnlyColor,
    kColorId_TextfieldReadOnlyBackground,
    kColorId_TextfieldSelectionColor,
    kColorId_TextfieldSelectionBackgroundFocused,
    
    kColorId_TreeBackground,
    kColorId_TreeText,
    kColorId_TreeSelectedText,
    kColorId_TreeSelectedTextUnfocused,
    kColorId_TreeSelectionBackgroundFocused,
    kColorId_TreeSelectionBackgroundUnfocused,
    kColorId_TreeArrow,
    
    kColorId_TableBackground,
    kColorId_TableText,
    kColorId_TableSelectedText,
    kColorId_TableSelectedTextUnfocused,
    kColorId_TableSelectionBackgroundFocused,
    kColorId_TableSelectionBackgroundUnfocused,
    kColorId_TableGroupingIndicatorColor,
    
  };

  
  virtual SkColor GetSystemColor(ColorId color_id) const = 0;

  
  
  
  
  
  static NativeTheme* instance();

 protected:
  NativeTheme();
  virtual ~NativeTheme();

  unsigned int thumb_inactive_color_;
  unsigned int thumb_active_color_;
  unsigned int track_color_;

  DISALLOW_COPY_AND_ASSIGN(NativeTheme);
};

}  

#endif  
