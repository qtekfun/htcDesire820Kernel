// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_TOOLTIP_MANAGER_WIN_H_
#define UI_VIEWS_WIDGET_TOOLTIP_MANAGER_WIN_H_

#include <windows.h>
#include <commctrl.h>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/tooltip_manager.h"

namespace gfx {
class Point;
}

namespace views {

class View;
class Widget;

class TooltipManagerWin : public TooltipManager {
 public:
  
  explicit TooltipManagerWin(Widget* widget);
  virtual ~TooltipManagerWin();

  
  
  
  bool Init();

  
  virtual const gfx::FontList& TooltipManagerWin::GetFontList() const OVERRIDE;
  virtual void UpdateTooltip() OVERRIDE;
  virtual void TooltipTextChanged(View* view) OVERRIDE;

  
  virtual void OnMouse(UINT u_msg, WPARAM w_param, LPARAM l_param);
  LRESULT OnNotify(int w_param, NMHDR* l_param, bool* handled);

 protected:
  
  gfx::NativeView GetParent();

  
  void UpdateTooltip(const gfx::Point& location);

  
  HWND tooltip_hwnd_;

  
  TOOLINFO toolinfo_;

  
  gfx::Point last_mouse_pos_;

  
  bool tooltip_showing_;

 private:
  
  
  bool SetTooltipPosition(int text_x, int text_y);

  
  
  int CalcTooltipHeight();

  
  Widget* widget_;

  
  
  View* last_tooltip_view_;

  
  
  
  bool last_view_out_of_sync_;

  
  string16 tooltip_text_;

  
  string16 clipped_text_;

  
  int line_count_;

  
  int tooltip_width_;

  DISALLOW_COPY_AND_ASSIGN(TooltipManagerWin);
};

}  

#endif  
