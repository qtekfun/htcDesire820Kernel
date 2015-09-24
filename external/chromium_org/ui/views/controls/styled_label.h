// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_STYLED_LABEL_H_
#define UI_VIEWS_CONTROLS_STYLED_LABEL_H_

#include <map>
#include <queue>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/size.h"
#include "ui/views/controls/link_listener.h"
#include "ui/views/view.h"

namespace views {

class Link;
class StyledLabelListener;

class VIEWS_EXPORT StyledLabel : public View, public LinkListener {
 public:
  
  struct VIEWS_EXPORT RangeStyleInfo {
    RangeStyleInfo();
    ~RangeStyleInfo();

    
    static RangeStyleInfo CreateForLink();

    
    
    int font_style;

    
    SkColor color;

    
    string16 tooltip;

    
    bool disable_line_wrapping;

    
    bool is_link;
  };

  
  StyledLabel(const string16& text, StyledLabelListener* listener);
  virtual ~StyledLabel();

  
  void SetText(const string16& text);

  
  
  void AddStyleRange(const gfx::Range& range, const RangeStyleInfo& style_info);

  
  
  void SetDefaultStyle(const RangeStyleInfo& style_info);

  
  
  
  void SetDisplayedOnBackgroundColor(SkColor color);
  SkColor displayed_on_background_color() const {
    return displayed_on_background_color_;
  }

  void set_auto_color_readability_enabled(bool auto_color_readability) {
    auto_color_readability_enabled_ = auto_color_readability;
  }

  
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual int GetHeightForWidth(int w) OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void PreferredSizeChanged() OVERRIDE;

  
  virtual void LinkClicked(Link* source, int event_flags) OVERRIDE;

 private:
  struct StyleRange {
    StyleRange(const gfx::Range& range,
               const RangeStyleInfo& style_info)
        : range(range),
          style_info(style_info) {
    }
    ~StyleRange() {}

    bool operator<(const StyleRange& other) const;

    gfx::Range range;
    RangeStyleInfo style_info;
  };

  
  
  
  
  int CalculateAndDoLayout(int width, bool dry_run);

  
  string16 text_;

  
  
  RangeStyleInfo default_style_info_;

  
  StyledLabelListener* listener_;

  
  std::priority_queue<StyleRange> style_ranges_;

  
  
  std::map<View*, gfx::Range> link_targets_;

  
  
  gfx::Size calculated_size_;

  
  SkColor displayed_on_background_color_;
  bool displayed_on_background_color_set_;

  
  
  bool auto_color_readability_enabled_;

  DISALLOW_COPY_AND_ASSIGN(StyledLabel);
};

}  

#endif  
