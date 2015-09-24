// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LAYOUT_BOX_LAYOUT_H_
#define UI_VIEWS_LAYOUT_BOX_LAYOUT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/insets.h"
#include "ui/views/layout/layout_manager.h"

namespace gfx {
class Size;
}

namespace views {

class View;

class VIEWS_EXPORT BoxLayout : public LayoutManager {
 public:
  enum Orientation {
    kHorizontal,
    kVertical,
  };

  
  
  
  
  BoxLayout(Orientation orientation,
            int inside_border_horizontal_spacing,
            int inside_border_vertical_spacing,
            int between_child_spacing);
  virtual ~BoxLayout();

  void set_spread_blank_space(bool spread) {
    spread_blank_space_ = spread;
  }

  
  virtual void Layout(View* host) OVERRIDE;
  virtual gfx::Size GetPreferredSize(View* host) OVERRIDE;
  virtual int GetPreferredHeightForWidth(View* host, int width) OVERRIDE;

 private:
  
  gfx::Size GetPreferredSizeForChildWidth(View* host, int child_area_width);

  
  
  gfx::Size NonChildSize(View* host);

  const Orientation orientation_;

  
  gfx::Insets inside_border_insets_;

  
  const int between_child_spacing_;

  
  
  bool spread_blank_space_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(BoxLayout);
};

} 

#endif  
