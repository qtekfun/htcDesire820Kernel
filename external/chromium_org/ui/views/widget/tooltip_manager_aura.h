// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_TOOLTIP_MANAGER_AURA_H_
#define UI_VIEWS_WIDGET_TOOLTIP_MANAGER_AURA_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/tooltip_manager.h"

namespace aura {
class Window;
}

namespace gfx {
class FontList;
}

namespace views {

class Widget;

class TooltipManagerAura : public TooltipManager {
 public:
  explicit TooltipManagerAura(Widget* widget);
  virtual ~TooltipManagerAura();

  
  
  
  
  static void UpdateTooltipManagerForCapture(Widget* source);

  
  static const gfx::FontList& GetDefaultFontList();

  
  virtual const gfx::FontList& GetFontList() const OVERRIDE;
  virtual void UpdateTooltip() OVERRIDE;
  virtual void TooltipTextChanged(View* view) OVERRIDE;

 private:
  View* GetViewUnderPoint(const gfx::Point& point);
  void UpdateTooltipForTarget(View* target,
                              const gfx::Point& point,
                              aura::Window* root_window);

  
  aura::Window* GetWindow();

  Widget* widget_;
  string16 tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(TooltipManagerAura);
};

}  

#endif  
