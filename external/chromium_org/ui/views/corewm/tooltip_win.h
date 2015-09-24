// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_TOOLTIP_WIN_H_
#define UI_VIEWS_COREWM_TOOLTIP_WIN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/gfx/point.h"
#include "ui/views/corewm/tooltip.h"

#include <windows.h>
#include <commctrl.h>

namespace views {
namespace corewm {

class VIEWS_EXPORT TooltipWin : public Tooltip {
 public:
  explicit TooltipWin(HWND parent);
  virtual ~TooltipWin();

  
  
  bool HandleNotify(int w_param, NMHDR* l_param, LRESULT* l_result);

 private:
  
  
  bool EnsureTooltipWindow();

  
  void PositionTooltip();

  
  virtual void SetText(aura::Window* window,
                       const base::string16& tooltip_text,
                       const gfx::Point& location) OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsVisible() OVERRIDE;

  
  HWND parent_hwnd_;

  
  HWND tooltip_hwnd_;

  
  TOOLINFO toolinfo_;

  
  bool showing_;

  
  
  
  gfx::Point location_;

  DISALLOW_COPY_AND_ASSIGN(TooltipWin);
};

}  
}  

#endif  
